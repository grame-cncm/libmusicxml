\version "2.19.83"
% automatically converted by musicxml2ly from 32a-Notations_musicxml2ly.xml


%% additional definitions required by the score:
sfffz = #(make-dynamic-script "sfffz")
sfp = #(make-dynamic-script "sfp")

\header {
    texidoc =
    "All <notation> elements
          defined in MusicXML. The lyrics show the notation assigned to each
          note."
    title =  "MusicXML notations (attached to note)"
    }

\layout {
    \context { \Score
        autoBeaming = ##f
        }
    }
PartPOneVoiceOne =  \relative c'' {
    \clef "treble" \key c \major | % 1
    c4 ^\fermata c4 \fermata c4 \shortfermata c4 \longfermata | % 2
    c4 _\fermata <c e g>4 \arpeggio \arpeggio \arpeggio \arpeggioBracket
    \arpeggioBracket <c e g>4 \arpeggio \arpeggio \arpeggioNormal
    \arpeggioNormal c4 ^\markup { \doublesharp } \bar "||"
    c4 -> c4 -^ c4 -. c4 -- | % 4
    c4 -_ c4 -! c4 c4 | % 5
    c4 c4 -\bendAfter #4 c4 -\bendAfter #-4 c4 \breathe | % 6
    c4 ^\markup { \musicglyph #"scripts.caesura.straight" } c4 c4 r4
    \bar "||"
    c4 \trill c4 \turn c4 c4 \reverseturn | % 8
    c4 \once \override TextSpanner #'style = #'trill c4 \startTextSpan c4
    \stopTextSpan \startTextSpan c4 \stopTextSpan | % 9
    c4 \mordent c4 \prall c4 c4 :32 | \barNumberCheck #10
    c4 \turn -\markup { \natural } c4 \turn ^\markup { \sharp } ^\markup
    { \sesquiflat } r2 \bar "||"
    c4 \upbow c4 \downbow c4 \flageolet c4 \flageolet | % 12
    c4 \flageolet c4 \flageolet c4 \flageolet c4 \flageolet | % 13
    c4 \open c4 \thumb c4 c4 -1 | % 14
    c4 -2 c4 -3 c4 -4 c4 -5 | % 15
    c4 -something c4 -5 -3 -2 c4 c4 | % 16
    c4 c4 c4 -+ c4 \snappizzicato | % 17
    c4 c4 c4 \1 c4 \5 | % 18
    c4 c4 c4 c4 | % 19
    c4 -\bendAfter #4 c4 -\bendAfter #3 c4 -\bendAfter #-0.5 c4
    -\bendAfter #3.5 | \barNumberCheck #20
    c4 c4 c4 c4 | % 21
    c4 r4 r2 \bar "||"
    c4 -\f c4 -\ppp c4 -\sfp c4 -\sfffz | % 23
    g4 ^^ ^. g4 _> _- ^. r2 \bar "|."
    }

PartPOneVoiceOneLyricsOne =  \lyricmode {\set ignoreMelismata = ##t
    "ferm." "normal ferm." "angled ferm." "square ferm." "inv.ferm."
    "arp." "non-arp." "acc.mark" "acc." "str.-acc." "stacc." "ten."
    "det.-leg." "stacc.ss" "spicc." scoop plop doit falloff breath
    "caes." stress "unstr." "tr." turn "del.turn" "inv.turn" shake wavy
    -- wavy -- line "mord." "inv.mord." "schl." "trem." "turn+acc."
    "turn+acc.(ab.+bel./rel to turn)" "up-b." "down-b." "harm."
    "nat.harm." "art.harm." "nat.h./base" "nat.h./touching"
    "nat.h./sounding" "open-str." "thumb-pos." "empty fing." "fing.1"
    "fing.2" "fing.3" "fing.4" "fing.5" "fing.sth." "mult.fing."
    "empty pluck" "pluck a" "dbl.tng." "trpl.tng." stopped "snp.pizz."
    "empty fret" "fret0" "empty str." "str. 5" hammer -- on pull -- off
    bend "b.3 with-bar" "pre-b. -0.5" "b. release 3.5" tap "tap T" heel
    toe "fingern." f ppp sfp "Oth.dyn." "both above" "ab./bel./bel."
    }


% The score definition
\score {
    <<

        \new Staff
        <<

            \context Staff <<
                \mergeDifferentlyDottedOn\mergeDifferentlyHeadedOn
                \context Voice = "PartPOneVoiceOne" {  \PartPOneVoiceOne }
                \new Lyrics \lyricsto "PartPOneVoiceOne" { \set stanza = "1." \PartPOneVoiceOneLyricsOne }
                >>
            >>

        >>
    \layout {}
    % To create MIDI output, uncomment the following line:
    %  \midi {\tempo 4 = 100 }
    }

