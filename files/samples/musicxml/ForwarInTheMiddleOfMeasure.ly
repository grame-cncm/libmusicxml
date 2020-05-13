\version "2.19.83"

% Comment or adapt next line as needed (default is 20)
#(set-global-staff-size 20.000000)

\header {
  software           = "IMUTUS Score Editor module"
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

Part_POne_Staff_One_Voice_One = \absolute {
  \language "nederlands"
  \key c \major
  \numericTimeSignature \time 4/4
  
  \clef "treble"
  \tempo \markup {
    \concat {
       \smaller \general-align #Y #DOWN \note #"2" #UP
      " = "
      120
    }
  }
  e''4 b' s%{44%} b'
  \bar "|."
  \bar ".|:" | % 2
  \barNumberCheck #2
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
      
      \new Staff = "Part_POne_Staff_One"
      \with {
      }
      <<
        \context Voice = "Part_POne_Staff_One_Voice_One" <<
          \Part_POne_Staff_One_Voice_One
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
  
}

