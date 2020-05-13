\version "2.19.83"
% automatically converted by musicxml2ly from LilyPondIssue34_musicxml2ly.xml
\pointAndClickOff

\header {
    copyright =  "Copyright © 2003 Recordare LLC"
    encodingdate =  "2003-03-14"
    title =  "Piano Sonata in A Major"
    opus =  "K. 331"
    composer =  "Wolfgang Amadeus Mozart"
    encodingsoftware =  "Finale 2003 for Windows"
    }

\layout {
    \context { \Score
        autoBeaming = ##f
        }
    }
PartPOneVoiceOne =  \relative cis''' {
    \clef "treble" \key a \major \time 2/4 | % 1
    \stemUp cis2 \arpeggio }

PartPOneVoiceThree =  \relative a, {
    \clef "bass" \key a \major \time 2/4 \grace { \stemUp a16 ( [
        \stemUp cis16 \stemUp e16 ] } \stemDown a8 ) [ \stemDown g8
    \stemDown f8 \stemDown e8 ] }

PartPOneVoiceTwo =  \relative cis'' {
    \clef "treble" \key a \major \time 2/4 \stemDown <cis e a>4
    \arpeggio \arpeggio \arpeggio }


% The score definition
\score {
    <<
        
        \new PianoStaff
        <<
            \set PianoStaff.instrumentName = "Piano"
            
            \context Staff = "1" << 
                \mergeDifferentlyDottedOn\mergeDifferentlyHeadedOn
                \context Voice = "PartPOneVoiceOne" {  \voiceOne \PartPOneVoiceOne }
                \context Voice = "PartPOneVoiceTwo" {  \voiceTwo \PartPOneVoiceTwo }
                >> \context Staff = "2" <<
                \mergeDifferentlyDottedOn\mergeDifferentlyHeadedOn
                \context Voice = "PartPOneVoiceThree" {  \PartPOneVoiceThree }
                >>
            >>
        
        >>
    \layout {}
    % To create MIDI output, uncomment the following line:
    %  \midi {\tempo 4 = 100 }
    }

