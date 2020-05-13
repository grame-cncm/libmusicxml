\version "2.19.83"
% automatically converted by musicxml2ly from Harmonies_musicxml2ly.xml


\header {
    encodingsoftware =  "MuseScore 2.0.3.1"
    encodingdate =  "2017-03-06"
    title =  Harmonies
    }

#(set-global-staff-size 20.1587428571)
\paper {

    paper-width = 21.01\cm
    paper-height = 29.69\cm
    top-margin = 1.0\cm
    bottom-margin = 2.0\cm
    left-margin = 1.0\cm
    right-margin = 1.0\cm
    indent = 1.61615384615\cm
    short-indent = 1.29292307692\cm
    }
\layout {
    \context { \Score
        autoBeaming = ##f
        }
    }
PartPOneVoiceOne =  \relative f' {
    \clef "treble" \key c \major \numericTimeSignature\time 4/4 | % 1
    \stemUp f4 \stemUp e8 [ \stemUp c8 ] \stemUp d4 \stemUp g4 | % 2
    \stemDown <a c e>2 \stemUp a2 \bar "|."
    }

PartPOneVoiceOneChords =  \chordmode {
    | % 1
    c4:m5 f8:m7 s8 g4:maj7 cis4:aug5/+gis | % 2
    a2:m7 b2:5 \bar "|."
    }


% The score definition
\score {
    <<

        \context ChordNames = "PartPOneVoiceOneChords" { \PartPOneVoiceOneChords}
        \new Staff
        <<
            \set Staff.instrumentName = "Piano"
            \set Staff.shortInstrumentName = "Pno."

            \context Staff <<
                \mergeDifferentlyDottedOn\mergeDifferentlyHeadedOn
                \context Voice = "PartPOneVoiceOne" {  \PartPOneVoiceOne }
                >>
            >>

        >>
    \layout {}
    % To create MIDI output, uncomment the following line:
    %  \midi {\tempo 4 = 100 }
    }

