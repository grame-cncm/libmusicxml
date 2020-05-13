\version "2.19.83"
% automatically converted by musicxml2ly from NestedPartGroups_musicxml2ly.xml
\pointAndClickOff

\header {
    title =  "Nested part groups"
    }

#(set-global-staff-size 11.416)
\paper {
    
    paper-width = 27.94\cm
    paper-height = 35.56\cm
    top-margin = 1.27\cm
    bottom-margin = 0.45\cm
    left-margin = 1.77\cm
    right-margin = 0.79\cm
    between-system-space = 0.39\cm
    page-top-space = 0.39\cm
    indent = 2.14923076923\cm
    }
\layout {
    \context { \Score
        skipBars = ##t
        autoBeaming = ##f
        }
    }
PartPOneVoiceOne =  \relative b' {
    \clef "treble" \key c \major \time 4/4 | % 1
    b1 | % 2
    a1 | % 3
    R1 \bar "|."
    }

PartPTwoVoiceOne =  \relative a' {
    \clef "treble" \key c \major \time 4/4 | % 1
    a1 | % 2
    a1 | % 3
    R1 \bar "|."
    }

PartPThreeVoiceOne =  \relative a' {
    \clef "treble" \key c \major \time 4/4 | % 1
    a1 | % 2
    a1 | % 3
    R1 \bar "|."
    }

PartPFourVoiceOne =  \relative f' {
    \clef "treble" \key c \major \time 4/4 | % 1
    f1 | % 2
    b1 | % 3
    R1 \bar "|."
    }

PartPFiveVoiceOne =  \relative d'' {
    \clef "treble" \key c \major \time 4/4 | % 1
    d1 | % 2
    f,1 | % 3
    R1 \bar "|."
    }


% The score definition
\score {
    <<
        
        \new Staff
        <<
            \set Staff.instrumentName = "Violin"
            
            \context Staff << 
                \mergeDifferentlyDottedOn\mergeDifferentlyHeadedOn
                \context Voice = "PartPOneVoiceOne" {  \PartPOneVoiceOne }
                >>
            >>
        \new StaffGroup \with { systemStartDelimiter =
            #'SystemStartSquare }
        
        <<
            \new Staff
            <<
                \set Staff.instrumentName = "Flute"
                
                \context Staff << 
                    \mergeDifferentlyDottedOn\mergeDifferentlyHeadedOn
                    \context Voice = "PartPTwoVoiceOne" {  \PartPTwoVoiceOne }
                    >>
                >>
            \new StaffGroup
            <<
                \new Staff
                <<
                    \set Staff.instrumentName = "Oboe I"
                    
                    \context Staff << 
                        \mergeDifferentlyDottedOn\mergeDifferentlyHeadedOn
                        \context Voice = "PartPThreeVoiceOne" {  \PartPThreeVoiceOne }
                        >>
                    >>
                \new Staff
                <<
                    \set Staff.instrumentName = "Oboe II"
                    
                    \context Staff << 
                        \mergeDifferentlyDottedOn\mergeDifferentlyHeadedOn
                        \context Voice = "PartPFourVoiceOne" {  \PartPFourVoiceOne }
                        >>
                    >>
                
                >>
            
            >>
        \new Staff
        <<
            \set Staff.instrumentName = "English horn"
            
            \context Staff << 
                \mergeDifferentlyDottedOn\mergeDifferentlyHeadedOn
                \context Voice = "PartPFiveVoiceOne" {  \PartPFiveVoiceOne }
                >>
            >>
        
        >>
    \layout {}
    % To create MIDI output, uncomment the following line:
    %  \midi {\tempo 4 = 100 }
    }

