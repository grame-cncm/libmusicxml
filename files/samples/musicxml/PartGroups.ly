\version "2.19.83"

% Comment or adapt next line as needed (default is 20)
#(set-global-staff-size 11.416)

\header {
  title              = "Prelude to a Tragedy"
  subtitle           = ""
  rights             = "© 2004 Polygames.     All Rights Reserved."
  composer           = "Lee Actor"
  software           =
  \markup {
    \column {
      "Finale 2005 for Windows"
      "Dolet 4.0 Beta 4 for Finale"
    }
  }
  
  encodingDate       = "2007-06-19"
}

\paper {
  paper-height                   = 297\mm
  paper-width                    = 210\mm
  
  %left-margin                    = 0.0\mm
  %right-margin                   = 0.0\mm
  %top-margin                     = 0.0\mm
  %bottom-margin                  = 0.0\mm
  
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
  \numericTimeSignature \time 3/4
  
  \clef "treble"
  
  \stopStaff
  \override Staff.StaffSymbol.line-count = 5
  \startStaff
  \transposition c''
  \tempo \markup {
    \concat {
        " "(\smaller \general-align #Y #DOWN \note #"4" #UP
        " = "
        85)
      
    }
  }
  R%{1%}2. | % 2
  \barNumberCheck #2
  \numericTimeSignature \time 4/4
  R%{1%}1 | % 3
  \barNumberCheck #3
  R%{1%}1 | % 4
  \barNumberCheck #4
  \numericTimeSignature \time 3/4
  R%{1%}2. | % 5
  \barNumberCheck #5
  \numericTimeSignature \time 2/4
  R%{1%}2 | % 6
  \barNumberCheck #6
  \numericTimeSignature \time 4/4
  R%{1%}1 | % 7
  \barNumberCheck #7
  \numericTimeSignature \time 3/4
  b''2.  ~ \p | % 8
  \barNumberCheck #8
  \numericTimeSignature \time 2/4
  b''2  ~ | % 1
  \barNumberCheck #9
}

Part_PTwo_Staff_One_Voice_One = \absolute {
  \language "nederlands"
  \key c \major
  \numericTimeSignature \time 3/4
  
  \clef "treble"
  
  \stopStaff
  \override Staff.StaffSymbol.line-count = 5
  \startStaff
  R%{1%}2. | % 2
  \barNumberCheck #2
  \numericTimeSignature \time 4/4
  R%{1%}1 | % 3
  \barNumberCheck #3
  R%{1%}1 | % 4
  \barNumberCheck #4
  \numericTimeSignature \time 3/4
  R%{1%}2. | % 5
  \barNumberCheck #5
  \numericTimeSignature \time 2/4
  R%{1%}2 | % 6
  \barNumberCheck #6
  \numericTimeSignature \time 4/4
  R%{1%}1 | % 7
  \barNumberCheck #7
  \numericTimeSignature \time 3/4
  <b''  ~ fis'''!  ~ >2. -\p | % 8
  \barNumberCheck #8
  \numericTimeSignature \time 2/4
  <b'' _\< fis''' >2 \< | % 1
  \barNumberCheck #9
}

Part_PThree_Staff_One_Voice_One = \absolute {
  \language "nederlands"
  \key c \major
  \numericTimeSignature \time 3/4
  
  \clef "treble"
  
  \stopStaff
  \override Staff.StaffSymbol.line-count = 5
  \startStaff
  R%{1%}2. \! | % 2
  \barNumberCheck #2
  \numericTimeSignature \time 4/4
  R%{1%}1 | % 3
  \barNumberCheck #3
  R%{1%}1 | % 4
  \barNumberCheck #4
  \numericTimeSignature \time 3/4
  R%{1%}2. | % 5
  \barNumberCheck #5
  \numericTimeSignature \time 2/4
  R%{1%}2 | % 6
  \barNumberCheck #6
  \numericTimeSignature \time 4/4
  R%{1%}1 | % 7
  \barNumberCheck #7
  \numericTimeSignature \time 3/4
  <b''  ~ d'''  ~ >2. -\p | % 8
  \barNumberCheck #8
  \numericTimeSignature \time 2/4
  <b'' _\< d''' >2 \< | % 1
  \barNumberCheck #9
}

Part_PFour_Staff_One_Voice_One = \absolute {
  \language "nederlands"
  \key c \major
  \numericTimeSignature \time 3/4
  
  \clef "treble"
  
  \stopStaff
  \override Staff.StaffSymbol.line-count = 5
  \startStaff
  \transposition f
  R%{1%}2. \! | % 2
  \barNumberCheck #2
  \numericTimeSignature \time 4/4
  R%{1%}1 | % 3
  \barNumberCheck #3
  R%{1%}1 | % 4
  \barNumberCheck #4
  \numericTimeSignature \time 3/4
  R%{1%}2. | % 5
  \barNumberCheck #5
  \numericTimeSignature \time 2/4
  R%{1%}2 | % 6
  \barNumberCheck #6
  \numericTimeSignature \time 4/4
  R%{1%}1 | % 7
  \barNumberCheck #7
  \numericTimeSignature \time 3/4
  R%{1%}2. | % 8
  \barNumberCheck #8
  \numericTimeSignature \time 2/4
  R%{1%}2 | % 1
  \barNumberCheck #9
}

Part_PFive_Staff_One_Voice_One = \absolute {
  \language "nederlands"
  \key c \major
  \numericTimeSignature \time 3/4
  
  \clef "treble"
  
  \stopStaff
  \override Staff.StaffSymbol.line-count = 5
  \startStaff
  \transposition bes
  R%{1%}2. | % 2
  \barNumberCheck #2
  \numericTimeSignature \time 4/4
  R%{1%}1 | % 3
  \barNumberCheck #3
  R%{1%}1 | % 4
  \barNumberCheck #4
  \numericTimeSignature \time 3/4
  R%{1%}2. | % 5
  \barNumberCheck #5
  \numericTimeSignature \time 2/4
  R%{1%}2 | % 6
  \barNumberCheck #6
  \numericTimeSignature \time 4/4
  R%{1%}1 | % 7
  \barNumberCheck #7
  \numericTimeSignature \time 3/4
  <cis'''!  ~ e'''  ~ >2. -\p | % 8
  \barNumberCheck #8
  \numericTimeSignature \time 2/4
  <cis''' _\< e''' >2 \< | % 1
  \barNumberCheck #9
}

Part_PSix_Staff_One_Voice_One = \absolute {
  \language "nederlands"
  \key c \major
  \numericTimeSignature \time 3/4
  
  \clef "treble"
  
  \stopStaff
  \override Staff.StaffSymbol.line-count = 5
  \startStaff
  \transposition bes,
  R%{1%}2. \! | % 2
  \barNumberCheck #2
  \numericTimeSignature \time 4/4
  R%{1%}1 | % 3
  \barNumberCheck #3
  R%{1%}1 | % 4
  \barNumberCheck #4
  \numericTimeSignature \time 3/4
  R%{1%}2. | % 5
  \barNumberCheck #5
  \numericTimeSignature \time 2/4
  R%{1%}2 | % 6
  \barNumberCheck #6
  \numericTimeSignature \time 4/4
  R%{1%}1 | % 7
  \barNumberCheck #7
  \numericTimeSignature \time 3/4
  R%{1%}2. | % 8
  \barNumberCheck #8
  \numericTimeSignature \time 2/4
  R%{1%}2 | % 1
  \barNumberCheck #9
}

Part_PSeven_Staff_One_Voice_One = \absolute {
  \language "nederlands"
  \key c \major
  \numericTimeSignature \time 3/4
  
  \clef "bass"
  
  \stopStaff
  \override Staff.StaffSymbol.line-count = 5
  \startStaff
  R%{1%}2. | % 2
  \barNumberCheck #2
  \numericTimeSignature \time 4/4
  R%{1%}1 | % 3
  \barNumberCheck #3
  R%{1%}1 | % 4
  \barNumberCheck #4
  \numericTimeSignature \time 3/4
  R%{1%}2. | % 5
  \barNumberCheck #5
  \numericTimeSignature \time 2/4
  R%{1%}2 | % 6
  \barNumberCheck #6
  \numericTimeSignature \time 4/4
  R%{1%}1 | % 7
  \barNumberCheck #7
  \numericTimeSignature \time 3/4
  R%{1%}2. | % 8
  \barNumberCheck #8
  \numericTimeSignature \time 2/4
  R%{1%}2 | % 9
  \barNumberCheck #9
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
      
      \new StaffGroup <<
      
        \new Staff = "Part_POne_Staff_One"
        \with {
        }
        <<
          \context Voice = "Part_POne_Staff_One_Voice_One" <<
            \Part_POne_Staff_One_Voice_One
          >>
        >>
        \new StaffGroup
        \with {
        }
         <<
        
        \new Staff = "Part_PTwo_Staff_One"
        \with {
        }
        <<
          \context Voice = "Part_PTwo_Staff_One_Voice_One" <<
            \Part_PTwo_Staff_One_Voice_One
          >>
        >>
        
        >>
        
        \new StaffGroup
        \with {
        }
         <<
        
        \new Staff = "Part_PThree_Staff_One"
        \with {
        }
        <<
          \context Voice = "Part_PThree_Staff_One_Voice_One" <<
            \Part_PThree_Staff_One_Voice_One
          >>
        >>
        
        >>
        
        \new Staff = "Part_PFour_Staff_One"
        \with {
        }
        <<
          \context Voice = "Part_PFour_Staff_One_Voice_One" <<
            \Part_PFour_Staff_One_Voice_One
          >>
        >>
        
        \new StaffGroup
        \with {
        }
         <<
        
        \new Staff = "Part_PFive_Staff_One"
        \with {
        }
        <<
          \context Voice = "Part_PFive_Staff_One_Voice_One" <<
            \Part_PFive_Staff_One_Voice_One
          >>
        >>
        
        >>
        
        \new Staff = "Part_PSix_Staff_One"
        \with {
        }
        <<
          \context Voice = "Part_PSix_Staff_One_Voice_One" <<
            \Part_PSix_Staff_One_Voice_One
          >>
        >>
        
        \new StaffGroup
        \with {
        }
         <<
        
        \new Staff = "Part_PSeven_Staff_One"
        \with {
        }
        <<
          \context Voice = "Part_PSeven_Staff_One_Voice_One" <<
            \Part_PSeven_Staff_One_Voice_One
          >>
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
    
    %left-margin                    = 0.0\mm
    %right-margin                   = 0.0\mm
    %top-margin                     = 0.0\mm
    %bottom-margin                  = 0.0\mm
    
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

