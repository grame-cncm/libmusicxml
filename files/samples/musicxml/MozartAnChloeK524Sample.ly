\version "2.19.83"

% Comment or adapt next line as needed (default is 20)
#(set-global-staff-size 20)

\header {
  opus               = "K. 524"
  title              = "An Chloe (Page 1)"
  rights             = "Copyright 2002 Recordare LLC"
  composer           = "Wolfgang Amadeus Mozart"
  poet               = "Johann Georg Jacobi"
  software           = "Finale 2002 for Windows"
  encodingDate       = "2002-03-15"
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
  \partial 4
  \key ees \major
  \time 2/2
  
  \clef "treble"
  r%{2%}4 | % 1
  \barNumberCheck #1
  R%{1%}1 | % 2
  \barNumberCheck #2
  R%{1%}1 | % 3
  \barNumberCheck #3
  R%{1%}1 | % 4
  \barNumberCheck #4
  R%{1%}1 | % 5
  \barNumberCheck #5
  \myBreak | % 5
  
  R%{1%}1 | % 6
  \barNumberCheck #6
  r%{2%}2 r%{2%}4 g'8 [ aes' ] | % 7
  \barNumberCheck #7
  bes'4 bes' bes' bes' | % 8
  \barNumberCheck #8
  bes'4. ( ees''8 ) ees''4 r%{2%} | % 9
  \barNumberCheck #9
  g'4. ( bes'8 ) bes'4 r%{2%} | % 10
  \barNumberCheck #10
  \myBreak | % 10
  
  ees'2  ~ ees'8 [ f' g' aes' ] | % 11
  \barNumberCheck #11
  g'4 ( f' ) r%{2%} g'8 [ aes' ] | % 12
  \barNumberCheck #12
  bes'4 bes' bes' bes' | % 13
  \barNumberCheck #13
  bes'4. ( ees''8 ) ees''4 r%{2%} | % 14
  \barNumberCheck #14
  \myBreak | % 14
  
  g'2  ~ g'8 [ ( aes' ) a'! ( bes' ] ) | % 15
  \barNumberCheck #15
  ees'4 ees' g'8. [ ( aes'!16 ) f'8. ( g'16 ] ) | % 16
  \barNumberCheck #16
  ees'4 r%{2%} r%{2%} g'8 [ a'! ] | % 17
  \barNumberCheck #17
  bes'4 f' c'' f' | % 0
  \barNumberCheck #18
}

Part_POne_Staff_One_Voice_One_Stanza_One = \lyricmode {
  \set associatedVoice = #"Part_POne_Staff_One_Voice_One"
  \set ignoreMelismata = ##t
    \skip4 \skip1 \skip1 \skip1 \skip1 %{ syllableLineBreak, line 93 %} %| % -1
    \skip1 \skip2 \skip4 Wenn8 die8 lieb4 aus4 dei4 -- nen4 blau4. -- \skip8 "en,"4 \skip4 hel4. -- \skip8 "len,"4 \skip4 off2 -- %{ syllableLineBreak, line 322 %} %| % -1
    \skip8 nen8 Au8 -- gen8 "sieht,"4 __ \skip4 \skip4 und8 vor8 Lust4 hin4 -- ein4 zu4 schau4. -- \skip8 en4 \skip4 mirs2 __ %{ syllableLineBreak, line 594 %} %| % -1
\skip8 \skip8 im8 __ \skip8 Her4 -- zen4 klopft8. __ \skip16 und8. __ \skip16 glht4 \skip4 \skip4 und8 ich8 hal4 -- te4 dich4 und4
}

Part_PTwo_Staff_One_Voice_One = \absolute {
  \language "nederlands"
  \partial 4
  \key ees \major
  \time 2/2
  
  \clef "treble"
  g'8 ^\markup { \bold "Allegretto" } \p [ ( aes' ] ) | % 1
  \barNumberCheck #1
  bes'4 bes' bes' bes' | % 2
  \barNumberCheck #2
  bes'4 \turn _\markup { \aNatural } ( ees'' ) r%{2%}2 | % 3
  \barNumberCheck #3
  g'4 \turn _\markup { \aSharp } ( bes' ) r%{2%}2 | % 4
  \barNumberCheck #4
  ees'4.4.*1/2 s%{16%}4.*1/2\turn \f f'16 [ g' ] aes' [ bes' c'' d'' ] ees'' [ f'' g'' aes'' ] | % 5
  \barNumberCheck #5
  \myBreak | % 5
  
  \tuplet 3/2 {
  bes''8 [ ( g'' ees'' ] ) }
  \tuplet 3/2 {
  bes'8 [ ( g' ees' ] ) }
  g' r%{2%} <aes bes d' f' > -\p r%{2%} | % 6
  \barNumberCheck #6
  <aes bes  ~ d' f' >2 ( <g bes ees' >4 ) r%{2%} | % 7
  \barNumberCheck #7
  g8 [ ( bes ees' bes ] aes [ bes f' bes ] ) | % 8
  \barNumberCheck #8
  g8 [ ( bes ees' bes ] g [ bes ees' bes ] ) | % 9
  \barNumberCheck #9
  g8 [ ( bes ees' bes ] g [ bes ees' bes ] ) | % 10
  \barNumberCheck #10
  \myBreak | % 10
  
  ees'8 [ ( bes g bes ] ees' [ <d' f' > <ees' g' > <f' aes' > ] ) | % 11
  \barNumberCheck #11
  <ees' g' >4 ( <d' f' > ) r%{2%}2 | % 12
  \barNumberCheck #12
  g8 [ ( bes ees' bes ] aes [ bes f' bes ] ) | % 13
  \barNumberCheck #13
  g8 [ ( bes ees' bes ] g [ bes ees' bes ] ) | % 14
  \barNumberCheck #14
  \myBreak | % 14
  
  g8 [ ( bes ees' bes ] g [ bes ees' bes ] ) | % 15
  \barNumberCheck #15
  g8 [ ( bes ees' bes ] ) <bes ees' > r%{2%} <aes d' > r%{2%} | % 16
  \barNumberCheck #16
  <g ees' >4 r%{2%} r%{2%}2 | % 17
  \barNumberCheck #17
  bes8 [ ( d' f' d' ] ) c' [ ( ees' f' ees' ] ) | % 0
  \barNumberCheck #18
}

Part_PTwo_Staff_Two_Voice_Three = \absolute {
  \language "nederlands"
  \partial 4
  \key ees \major
  \time 2/2
  
  \clef "bass"
  r%{2%}4 | % 1
  \barNumberCheck #1
  ees8 \p [ ( bes g bes ] d [ aes f aes ] ) | % 2
  \barNumberCheck #2
  ees8 [ ( bes g bes ] ees [ bes g bes ] ) | % 3
  \barNumberCheck #3
  ees8 [ ( bes g bes ] ees [ bes g bes ] ) | % 4
  \barNumberCheck #4
  bes,8 [ ( g ees g ] bes, [ g ees g ] ) | % 5
  \barNumberCheck #5
  bes,4 r%{2%} r%{2%} bes,8 r%{2%} | % 6
  \barNumberCheck #6
  <ees,  ~ ees  ~ >2 <ees, ees >4 r%{2%} | % 7
  \barNumberCheck #7
  <ees, ees >2 <d, d > | % 8
  \barNumberCheck #8
  <ees, ees >4 r%{2%} r%{2%}2 | % 9
  \barNumberCheck #9
  <ees, ees >4 r%{2%} r%{2%}2 | % 10
  \barNumberCheck #10
  g,2 ( ees, ) | % 11
  \barNumberCheck #11
  bes,4 bes bes, r%{2%} | % 12
  \barNumberCheck #12
  <ees, ees >2 <d, d > | % 13
  \barNumberCheck #13
  <ees, ees >4 r%{2%} r%{2%}2 | % 14
  \barNumberCheck #14
  <ees, ees >4 r%{2%} r%{2%}2 | % 15
  \barNumberCheck #15
  <bes,, bes, >2 r%{2%}4 bes,8 r%{2%} | % 16
  \barNumberCheck #16
  ees4 bes, ees, r%{2%} | % 17
  \barNumberCheck #17
  <d, d >2 <a,,! a,! > | % 18
  \barNumberCheck #18
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
          \new Lyrics
            \with {
            }
            \Part_POne_Staff_One_Voice_One_Stanza_One
        >>
        \new PianoStaff
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
        \new Staff = "Part_PTwo_Staff_Two"
        \with {
        }
        <<
          \context Voice = "Part_PTwo_Staff_Two_Voice_Three" <<
            \Part_PTwo_Staff_Two_Voice_Three
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

