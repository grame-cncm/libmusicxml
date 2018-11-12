\version "2.18.2"
% also works with 2.19.x and later

#(define (get-keysig-alt-count alt-alist)
   "Return number of sharps/flats in key sig, (+) for sharps, (-) for flats."
   (if (null? alt-alist)
       0
       (* (length alt-alist) 2 (cdr (car alt-alist)))))

#(define (get-major-tonic alt-count)
   "Return number of the tonic note 0-6, as if the key sig were major."
   ;; (alt-count major-tonic)
   ;; (-7 0) (-5 1) (-3 2) (-1 3) (1 4) (3 5) (5 6) (7 0)
   ;; (-6 4) (-4 5) (-2 6) (0 0) (2 1) (4 2) (6 3)
   (if (odd? alt-count)
       (modulo (- (/ (+ alt-count 1) 2) 4) 7)
       (modulo (/ alt-count 2) 7)))

#(define (get-key-alts context)
   "Needed because LilyPond 2.19 and 2.20 has 'keyAlterations
    and 2.18 has 'keySignature for the same context property.
    No way to test for existence of context property..."
   (define key-alts (ly:context-property context 'keyAlterations '()))
   (if (equal? key-alts '())
       (ly:context-property context 'keySignature '())
       key-alts))

#(define (stencil-flip axis stl)
   "Flip stencil @var{stl} in the direction of @var{axis}. 
    @var{axis} is 0 for X, 1 for Y."
   (let* (
           ;; center stl on 0,0 to prepare for flipping,
           ;; and calculate how far it moved.
           (centered-stl (ly:stencil-aligned-to stl axis CENTER))
           (original-ext (ly:stencil-extent stl axis))
           (centered-ext (ly:stencil-extent centered-stl axis))
           (offset (- (car original-ext) (car centered-ext)))

           ;; scale centered-stl using -1 to flip it
           (xy (if (= axis X) '(-1 . 1) '(1 . -1)))
           (flipped-stl (ly:stencil-scale centered-stl (car xy) (cdr xy)))

           ;; restore flipped stl to original position
           (replaced-stl (ly:stencil-translate-axis flipped-stl offset axis)))

     ;; for testing...
     ;; (display original-ext) (display centered-ext)
     ;; (display (ly:stencil-extent replaced-stl axis))(newline)

     replaced-stl))


jianpuMusic =
#(define-music-function (parser location mus) (ly:music?)
   "This music function modifies the music input for better jianpu output."
   (let ((major-tonic-pitch (ly:make-pitch 0 0 0))
         (prev-pitch (ly:make-pitch 0 0 0)))
     (music-map
      (lambda (m)
        (cond

         ;; KEY CHANGE
         ((music-is-of-type? m 'key-change-event)
          (let*
           ((pitch-alist (ly:music-property m 'pitch-alist))
            (key-alts (filter (lambda (a) (not (= 0 (cdr a)))) pitch-alist))
            (keysig-alt-count (get-keysig-alt-count key-alts))
            (major-tonic-number (get-major-tonic keysig-alt-count))
            (tonic (ly:music-property m 'tonic))
            (tonic-num (ly:pitch-notename tonic))
            ;; mode 1-7, 1=major 6=minor
            (mode (+ 1 (modulo (- tonic-num major-tonic-number) 7))))

           (set! major-tonic-pitch
                 (ly:make-pitch 0
                   major-tonic-number
                   (or (assoc-ref key-alts major-tonic-number) 0)))
           ;; store the actual key of the music for use in jianpu key signatures
           (ly:music-set-property! m 'jianpu-key-sig (cons mode tonic))

           ;; convert the key signature in effect to c major
           (ly:music-set-property! m 'pitch-alist '((0 . 0) (1 . 0) (2 . 0) (3 . 0) (4 . 0) (5 . 0) (6 . 0)))
           (ly:music-set-property! m 'tonic (ly:make-pitch 0 0 0))
           ))

         ;; REST AND MULTI-MEASURE REST
         ((or (music-is-of-type? m 'rest-event)
              (music-is-of-type? m 'multi-measure-rest))
          (let ((dur (ly:moment-main (ly:music-duration-length m))))
            (if (>= dur 1/2)
                ;; split whole and half rests into quarter notes
                (let ((q (make-music 'NoteEvent
                           'duration (ly:make-duration 2 0 1)
                           'jianpu-rest #t)))
                  ;; double dotted half rest is special case
                  ;; TODO: generalize this for triple dots, other durations, etc.?
                  (if (= dur 7/8)
                      (let ((q-dotted (ly:music-deep-copy q)))
                        (ly:music-set-property! q-dotted 'duration (ly:make-duration 2 1 1))
                        (set! m (make-sequential-music (append (make-list 2 q) (list q-dotted)))))
                      ;; else
                      (set! m (make-sequential-music (make-list (* 4 dur) q)))))

                ;; shorter rests are just converted into notes without pitches
                ;; and with a jianpu-rest property added
                (begin
                 (ly:music-set-property! m 'name 'NoteEvent)
                 (ly:music-set-property! m 'types
                   (append
                    (delete 'rest-event (ly:music-property m 'types))
                    '(note-event melodic-event)))
                 (ly:music-set-property! m 'jianpu-rest #t)
                 ))))

         ;; NOTE
         ((music-is-of-type? m 'note-event)
          ;; handle pitchless note events and transpose pitches to c major
          ;; which gives us correct accidentals and octaves "for free"
          (if (ly:pitch? (ly:music-property m 'pitch))
              (begin
               (ly:music-set-property! m 'pitch
                 (ly:pitch-transpose (ly:music-property m 'pitch)
                   (ly:pitch-negate (ly:pitch-diff major-tonic-pitch (ly:make-pitch 0 0 0)))))
               (set! prev-pitch (ly:music-property m 'pitch)))
              ;; else
              (ly:music-set-property! m 'pitch prev-pitch))

          (let* ((octave (ly:pitch-octave (ly:music-property m 'pitch)))
                 (octave-dir (if (> octave 0) 1 -1))
                 (artics (ly:music-property m 'articulations '()))
                 (dur (ly:moment-main (ly:music-duration-length m)))
                 (dot (ly:stencil-translate
                       (make-circle-stencil 0.2 0.001 #t)
                       (cons 0.6 0)))
                 (dot-two (ly:stencil-stack dot Y DOWN dot 0.5 0))
                 (dot-three (ly:stencil-stack dot-two Y DOWN dot 0.5 0))
                 (dot-four (ly:stencil-stack dot-three Y DOWN dot 0.5 0)))

            ;; add octave dot markups as articulations
            ;; from jianpu-ly python script: "-\tweak #'X-offset #0.6 _."
            (if (not (= 0 octave))
                (ly:music-set-property! m 'articulations
                  (append artics
                    (list
                     (make-music 'TextScriptEvent
                       'direction octave-dir
                       'text (markup #:stencil
                               (case (abs octave)
                                 ((1) dot)
                                 ((2) dot-two)
                                 ((3) dot-three)
                                 ((4) dot-four)
                                 (else dot-four)
                                 )))))))

            ;; split whole and half notes into quarter notes, some dashed
            (if (>= dur 1/2)
                (let* ((dash-num (+ -1 (* 4 dur)))
                       (q-dur (ly:make-duration 2 0 1))
                       (q (make-music 'NoteEvent
                            'duration q-dur
                            'pitch (ly:music-property m 'pitch)
                            'jianpu-dash-note #t)))

                  (ly:music-set-property! m 'duration q-dur)

                  (if (= dash-num 5/2)
                      ;; special fix for double-dotted half note
                      ;; TODO: generalize this for triple dots, other durations, etc.?
                      (let ((q-dotted (ly:music-deep-copy q)))
                        (ly:music-set-property! q-dotted 'duration (ly:make-duration 2 1 1))
                        (set! m (make-sequential-music
                                 (append (list m q) (list q-dotted)))))
                      ;; else
                      (set! m (make-sequential-music
                               (append (list m) (make-list dash-num q)))))
                  )))))
        m)
      mus)))


% Note: when checking custom event properties like jianpu-rest or
% jianpu-key-sig we have to use (if (equal? #t jianpu-rest) ...
% and not (if jianpu-rest ...  the latter results in "false positives".


#(define Jianpu_note_head_engraver
   (make-engraver
    (acknowledgers
     ((note-head-interface engraver grob source-engraver)
      ;; make sure \omit is not in effect (stencil is not #f)
      (if (ly:grob-property-data grob 'stencil)
          (let* ((staff-context (ly:translator-context engraver))
                 (event (event-cause grob))
                 (jianpu-dash-note (ly:event-property event 'jianpu-dash-note)))
            (if (equal? #t jianpu-dash-note)
                (ly:grob-set-property! grob 'stencil
                  (ly:stencil-translate
                   (make-connected-path-stencil '((1 0)) 0.3 1 1 #f #f)
                   (cons 0 1)))
                ;; else create number stencil based on the scale degree of the note
                ;; TODO: handle notes without pitches that aren't jianpu-rests
                (let*
                 ((grob-pitch (ly:event-property event 'pitch))
                  (note-number
                   (if (ly:pitch? grob-pitch)
                       (ly:pitch-notename grob-pitch) #f))
                  (jianpu-rest (ly:event-property event 'jianpu-rest))
                  (glyph-string
                   ;; check jianpu-rest first, because apparently
                   ;; sometimes there is a note-number for rests.
                   (if (equal? #t jianpu-rest)
                       "zero"
                       (if (number? note-number)
                           (case (+ 1 note-number)
                             ((1) "one")
                             ((2) "two")
                             ((3) "three")
                             ((4) "four")
                             ((5) "five")
                             ((6) "six")
                             ((7) "seven")
                             (else "zero"))
                           #f)))
                  (stl
                   (if (string? glyph-string)
                       (grob-interpret-markup grob
                         (markup #:musicglyph glyph-string)))))

                 (ly:grob-set-property! grob 'stencil stl)
                 ))))))))


#(define Jianpu_flag_engraver
   (make-engraver
    (acknowledgers
     ((flag-interface engraver grob source-engraver)
      ;; make sure \omit is not in effect (stencil is not #f)
      (if (ly:grob-property-data grob 'stencil)
          (let* ((glyph-name (ly:grob-property grob 'glyph-name))
                 (padding 0.35)
                 ;; TODO: width is hard-coded, would be better to match note-head width
                 ;; TODO: stencil creation could be coded better
                 (flag-one (make-connected-path-stencil '((1.2 0)) 0.15 1 1 #f #f))
                 (flag-two (ly:stencil-stack flag-one Y DOWN flag-one padding 0))
                 (flag-three (ly:stencil-stack flag-two Y DOWN flag-one padding 0))
                 (flag-four (ly:stencil-stack flag-three Y DOWN flag-one padding 0))
                 (flag-five (ly:stencil-stack flag-four Y DOWN flag-one padding 0))
                 (new-stl (cond
                           ((string= glyph-name "flags.d3") flag-one)
                           ((string= glyph-name "flags.d4") flag-two)
                           ((string= glyph-name "flags.d5") flag-three)
                           ((string= glyph-name "flags.d6") flag-four)
                           ((string= glyph-name "flags.d7") flag-five)
                           (else empty-stencil))))

            (ly:grob-set-property! grob 'stencil new-stl)
            (ly:grob-set-property! grob 'Y-offset -2)
            ;; TODO: are these needed?
            ;; (ly:grob-set-property! grob 'X-extent
            ;;  (ly:stencil-extent (ly:grob-property grob 'stencil) X))
            ;; (ly:grob-set-property! grob 'Y-extent
            ;;  (ly:stencil-extent (ly:grob-property grob 'stencil) Y))
            ))))))


#(define (jianpu-beam-adjust grob)
   "Adjusts the width etc. of beams."
   ;; We calculate the amount to scale based on width of beam
   ;; TODO: improve this, maybe by using stems
   (let* ((x-ext (interval-length (ly:stencil-extent (ly:grob-property grob 'stencil) X)))
          (x-scale (+ 1 (/ 1.4 x-ext))))
     ;; (display x-ext)(newline)
     (ly:grob-set-property! grob 'stencil
       ;; (ly:stencil-translate
       (stencil-flip Y
         (ly:stencil-scale
          (ly:grob-property grob 'stencil)
          x-scale 1))
       ;; (cons 0 0))
       )))


#(define Jianpu_key_engraver
   (let ((current-key-sig (cons 1 (ly:make-pitch 0 0 0))))
     (make-engraver
      (acknowledgers
       ((key-signature-interface engraver grob source-engraver)
        ;; make sure \omit is not in effect (stencil is not #f)
        (if (ly:grob-property-data grob 'stencil)
            (let* ((event (event-cause grob))
                   (jianpu-key-sig
                    (if event
                        (ly:event-property event 'jianpu-key-sig)
                        current-key-sig))
                   (tonic-num (car jianpu-key-sig))
                   (tonic-pitch (cdr jianpu-key-sig))
                   (key-sig-number-string (ly:number->string tonic-num))
                   (key-sig-note
                    (case (ly:pitch-notename tonic-pitch)
                      ((0) "C")
                      ((1) "D")
                      ((2) "E")
                      ((3) "F")
                      ((4) "G")
                      ((5) "A")
                      ((6) "B")
                      (else "X")))
                   (key-sig-alt
                    (case (ly:pitch-alteration tonic-pitch)
                      ((1/2) #{ \markup \sharp #})
                      ((-1/2) #{ \markup \flat #})
                      (else ""))))

              (set! current-key-sig jianpu-key-sig)

              (ly:grob-set-property! grob 'stencil
                (grob-interpret-markup grob
                  ;; TODO: scale the flat or sharp size, based on current font size
                  (markup key-sig-number-string "=" key-sig-alt key-sig-note)
                  )))))))))


% Custom "JianpuStaff" Context

\layout {
  \context {
    % \Staff lets us start with all standard staff settings
    \Staff
    % \name gives the custom staff context its name
    \name JianpuStaff
    % \alias Staff tells LilyPond that commands that work on a standard
    % Staff context should also work with this custom context
    \alias Staff
    % customizations
    \consists \Jianpu_note_head_engraver
    \consists \Jianpu_flag_engraver
    \consists \Jianpu_key_engraver

    \override Beam.after-line-breaking = #jianpu-beam-adjust
    \override KeySignature.break-visibility = ##(#f #f #f)

    \override Accidental.font-size = #-2.5
    \override Clef.stencil = ##f
    \override StaffSymbol.line-count = #0
    \override BarLine.bar-extent = #'(-2 . 2)
    \override TimeSignature.style = #'numbered
    % \override Stem.transparent = ##t
    \override Stem.length = #0
    \override Stem.length-fraction = #0
    \override NoteHead.Y-offset = #-1
    \override Beam.transparent = ##f
    \override Stem.direction = #DOWN
    \override Beam.beam-thickness = #0.15
    \override Beam.length-fraction = #0.5
    \override Tie.staff-position = #2.5
    \override TupletBracket.bracket-visibility = ##t
    \tupletUp
    \slurUp
    % \hide Stem
    \omit Stem
    \override Dots.staff-position = #2
  }
  % define the "parent" contexts that will accept the custom context
  \context { \Score \accepts JianpuStaff }
  \context { \ChoirStaff \accepts JianpuStaff }
  \context { \GrandStaff \accepts JianpuStaff }
  \context { \PianoStaff \accepts JianpuStaff }
  \context { \StaffGroup \accepts JianpuStaff }
}

% And for MIDI
% to avoid warnings it has to be defined for each type of output desired
% but we actually can't use midi with the jianpu music function, so this
% is rather pointless... leaving it in for now.
\midi {
  \context {
    \Staff
    \name JianpuStaff
    \alias Staff
  }
  % since the customizations are for visual output only,
  % there is no need to include them for midi
  \context { \Score \accepts JianpuStaff }
  \context { \ChoirStaff \accepts JianpuStaff }
  \context { \GrandStaff \accepts JianpuStaff }
  \context { \PianoStaff \accepts JianpuStaff }
  \context { \StaffGroup \accepts JianpuStaff }
}

