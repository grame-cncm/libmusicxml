\version "2.19.83"
% automatically converted by musicxml2ly from DalSegno_musicxml2ly.xml
\pointAndClickOff

\header {
    encodingsoftware =  soundnotation
    encodingdate =  "2019-05-15"
    title =  "Dal segno example"
    }

#(set-global-staff-size 20.8571428571)
\paper {
    
    paper-width = 21.01\cm
    paper-height = 28.0\cm
    top-margin = 0.5\cm
    bottom-margin = 0.5\cm
    left-margin = 1.0\cm
    right-margin = 1.0\cm
    between-system-space = 1.69\cm
    page-top-space = 0.5\cm
    indent = 1.61615384615\cm
    short-indent = 1.29292307692\cm
    }
\layout {
    \context { \Score
        autoBeaming = ##f
        }
    }
PartPOneVoiceOne =  \relative e'' {
    \clef "treble" \key c \major \numericTimeSignature\time 4/4 | % 1
    \tempo 4=120 | % 1
    \stemDown e8 [ \stemDown c8 \stemDown b8 \stemDown a8 ] \stemUp g8 [
    \stemUp c8 ] \stemDown e4 | % 14
    \mark \markup { \musicglyph #"scripts.segno" } | % 14
    \stemDown <c e>8 [ \stemDown <c e>8 \stemDown <c e>8 \stemDown <c e>8
    ] \stemDown <c e>8 [ \stemDown <c e>8 ] \stemDown <e g>4 \break
    \repeat volta 2 {
        | % 21
        \stemDown <d f>8 [ \stemDown <d f>8 \stemDown <b d>8 \stemDown
        <f b>8 ] \stemUp <e c'>2 }
    \break ^ "D.S." | % 25
    \stemDown <c' e>8 [ \mark \markup { \musicglyph #"scripts.coda" }
    \stemDown <c e>8 \stemDown <c e>8 \stemDown <c e>8 ] \stemDown <c e>8
    [ \stemDown <c e>8 ] \stemDown <e g>4 }

PartPOneVoiceOneChords =  \chordmode {
    | % 1
    c8:5 s8 s8 s8 s8 s8 s4 | % 14
    s8 s8 s8 s8 a8:m7 s8 s4 \repeat volta 2 {
        | % 21
        g8:7 s8 s8 s8 c2:5 }
    | % 25
    s8 s8 s8 s8 a8:m7 s8 }

PartPOneVoiceFive =  \relative c {
    \clef "bass" \key c \major \numericTimeSignature\time 4/4 \stemDown
    c8 [ \stemDown <g' c e>8 \stemDown <g c e>8 \stemDown <g c e>8 ]
    \stemDown c,8 [ \stemDown <g' c e>8 \stemDown <g c e>8 \stemDown <g
        c e>8 ] \stemDown c,8 [ \stemDown <g' c e>8 \stemDown <g c e>8
    \stemDown b,8 ] \stemDown a8 [ \stemDown <g' c e>8 \stemDown <g c e>8
    \stemDown <g c e>8 ] \break \repeat volta 2 {
        \stemDown g,8 [ \stemDown <g' d' f>8 \stemDown <g d' f>8
        \stemDown <g d' f>8 ] \stemDown c,8 [ \stemDown <g' c e>8
        \stemDown <g c e>8 \stemDown <g c e>8 ] }
    \break \stemDown c,8 [ \stemDown <g' c e>8 \stemDown <g c e>8
    \stemDown b,8 ] \stemDown a8 [ \stemDown <g' c e>8 \stemDown <g c e>8
    \stemDown <g c e>8 ] }


% The score definition
\score {
    <<
        
        \context ChordNames = "PartPOneVoiceOneChords" { \PartPOneVoiceOneChords}
        \new PianoStaff
        <<
            \set PianoStaff.instrumentName = "Piano"
            \set PianoStaff.shortInstrumentName = "Pno."
            
            \context Staff = "1" << 
                \mergeDifferentlyDottedOn\mergeDifferentlyHeadedOn
                \context Voice = "PartPOneVoiceOne" {  \PartPOneVoiceOne }
                >> \context Staff = "2" <<
                \mergeDifferentlyDottedOn\mergeDifferentlyHeadedOn
                \context Voice = "PartPOneVoiceFive" {  \PartPOneVoiceFive }
                >>
            >>
        
        >>
    \layout {}
    % To create MIDI output, uncomment the following line:
    %  \midi {\tempo 4 = 120 }
    }

