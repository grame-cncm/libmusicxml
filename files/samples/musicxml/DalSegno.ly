\version "2.19.83"

% Comment or adapt next line as needed (default is 20)
#(set-global-staff-size 20.8571)

\header {
  title              = "Dal segno example"
  subtitle           = ""
  rights             = ""
  software           =
  \markup {
    \column {
      "soundnotation"
      "Dolet 6.6"
    }
  }
  
  encodingDate       = "2019-05-15"
}

\paper {
  paper-height                   = 297\mm
  paper-width                    = 210\mm
  
  %left-margin                    = 0\mm
  %right-margin                   = 0\mm
  %top-margin                     = 0\mm
  %bottom-margin                  = 0\mm
  
  %horizontal-shift               = 0.0\mm
  %indent                         = 0.0\mm
  %short-indent                   = 0.0\mm
  
  %markup-system-spacing.padding  = 0.0\mm
  %between-system-space           = 0.0\mm
  %page-top-space                 = 0.0\mm
  
  %page-count                     = -1
  %system-count                   = -1
  
  oddHeaderMarkup                = ""
  evenHeaderMarkup               = ""
  oddFooterMarkup                = ""
  evenFooterMarkup               = ""
}

\layout {
  \context {
    \Score
    autoBeaming = ##f % to display tuplets brackets
  }
  \context {
    \Voice
  }
}

% Pick your choice from the next two lines as needed
myBreak = { \break }
%myBreak = { }

% Pick your choice from the next two lines as needed
myPageBreak = { \pageBreak }
%myPageBreak = { }

Part_POne_HARMONY_Staff_Voice_Eleven_HARMONY = \chordmode {
  \language "nederlands"
  \key c \major
  \numericTimeSignature \time 4/4
  c8:5.3 s%{44%}2.. | % 2
  \barNumberCheck #2
  s%{44%}2 a8:m7 s%{44%}4. | % 3
  \barNumberCheck #3
  g8:7 s%{44%}4. c2:5.3 | % 4
  \barNumberCheck #4
  s%{44%}2 a8:m7 s%{44%}4. | % 1
  \barNumberCheck #5
}

Part_POne_Staff_One_Voice_One = \absolute {
  \language "nederlands"
  
  \repeat volta 2 {
    \key c \major
    \numericTimeSignature \time 4/4
    
    \clef "treble"
    \tempo \markup {
      \concat {
         \smaller \general-align #Y #DOWN \note #"4" #UP
        " = "
        120
      }
    }
    e''8 [ c'' b' a' ] g' [ c'' ] e''4 | % 2
    \barNumberCheck #2
    \mark \markup { \musicglyph #"scripts.segno" }
    c''8 [ c'' c'' c'' ] c'' [ c'' ] e''4 | % 3
    \barNumberCheck #3
    \bar ".|:" \myBreak | % 3
    
    d''8 [ d'' b' f' ] e'2 | % 4
    \barNumberCheck #4
  
  }
  
  \myBreak | % 4
  
  c''8 [ \mark \markup { \musicglyph #"scripts.coda" }
  c'' c'' c'' ] c'' [ c'' ] e''4
  \bar "|."
  \bar ".|:" | % 1
  \barNumberCheck #5
}

Part_POne_Staff_Two_Voice_Five = \absolute {
  \language "nederlands"
  \key c \major
  \numericTimeSignature \time 4/4
  
  \clef "bass"
  c8 [ g g g ] c [ g g g ] | % 2
  \barNumberCheck #2
  c8 [ g g b, ] a, [ g g g ] | % 3
  \barNumberCheck #3
  
  \repeat volta 2 {
    \bar ".|:" g,8 ^\markup { "D.S." } [ g g g ] c [ g g g ] | % 4
    \barNumberCheck #4
  
  }
  
  c8 [ g g b, ] a, [ g g g ]
  \bar "|."
  \bar ".|:" | % 5
  \barNumberCheck #5
}

\book {
  \header {
  }
  
  \score {
    <<
      \new StaffGroup
      \with {
      }
       <<
      
      \new PianoStaff
      \with {
      }
      <<
      \context ChordNames = "Part_POne_HARMONY_Staff_Voice_Eleven_HARMONY"
      \Part_POne_HARMONY_Staff_Voice_Eleven_HARMONY
      
      \new Staff = "Part_POne_Staff_One"
      \with {
      }
      <<
        \context Voice = "Part_POne_Staff_One_Voice_One" <<
          \Part_POne_Staff_One_Voice_One
        >>
      >>
      
      \new Staff = "Part_POne_Staff_Two"
      \with {
      }
      <<
        \context Voice = "Part_POne_Staff_Two_Voice_Five" <<
          \Part_POne_Staff_Two_Voice_Five
        >>
      >>
      
      >>
      >>
      
    >>
    
    \layout {
      \context {
        \Score
        autoBeaming = ##f % to display tuplets brackets
      }
      \context {
        \Voice
      }
    }
    
    \midi {
      \tempo 4 = 90
    }
  }
  
  \paper {
    paper-height                   = 297\mm
    paper-width                    = 210\mm
    
    %left-margin                    = 0\mm
    %right-margin                   = 0\mm
    %top-margin                     = 0\mm
    %bottom-margin                  = 0\mm
    
    %horizontal-shift               = 0.0\mm
    %indent                         = 0.0\mm
    %short-indent                   = 0.0\mm
    
    %markup-system-spacing.padding  = 0.0\mm
    %between-system-space           = 0.0\mm
    %page-top-space                 = 0.0\mm
    
    %page-count                     = -1
    %system-count                   = -1
    
    oddHeaderMarkup                = ""
    evenHeaderMarkup               = ""
    oddFooterMarkup                = ""
    evenFooterMarkup               = ""
  }
  
}

