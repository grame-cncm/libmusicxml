\version "2.19.83"
% automatically converted by musicxml2ly from HarmoniesAndFiguredBass_musicxml2ly.xml


\header {
    texidoc =
    "Some figured bass containing
          alterated figures, bracketed figures and slashed figures. The last
          note contains an empty <figured-bass> element, which is
          invalid MusicXML, to check how well applications cope with malformed
          files.

          Note that this file does not contain any extenders!"
    }

\layout {
    \context { \Score
        autoBeaming = ##f
        }
    }
PartPOneVoiceOne =  \relative c' {
    \clef "treble" \key c \major \time 4/4 | % 1
    c4 d4 e4 f4 }

PartPOneVoiceOneChords =  \chordmode {
    | % 1
    f4:m7 s4 g4:maj7 }

PartPOneVoiceOneFiguredBass =  \figuremode {
    | % 1
    <3>4 s4 <1+ 3- 5!>4 <[6]>4 }


% The score definition
\score {
    <<

        \context ChordNames = "PartPOneVoiceOneChords" { \PartPOneVoiceOneChords}
        \new Staff
        <<

            \context Staff <<
                \mergeDifferentlyDottedOn\mergeDifferentlyHeadedOn
                \context Voice = "PartPOneVoiceOne" {  \PartPOneVoiceOne }
                \context FiguredBass = "PartPOneVoiceOneFiguredBass" \PartPOneVoiceOneFiguredBass
                >>
            >>

        >>
    \layout {}
    % To create MIDI output, uncomment the following line:
    %  \midi {\tempo 4 = 100 }
    }

