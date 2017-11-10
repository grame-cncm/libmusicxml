/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef __utilities__
#define __utilities__

#include <string>
#include <cassert>

#include <iostream>
#include <sstream>

#include <ctime>

#include <set>
#include <list>
<<<<<<< HEAD

#include <functional> 
#include <algorithm> 
=======
#include <algorithm>
#include <ctime>
#include <functional>
>>>>>>> a9badbde74ce22074ff142d1a9265c7a168a65d6

#include "smartpointer.h"
#include "basevisitor.h"
<<<<<<< HEAD

=======
#include "exports.h"
>>>>>>> a9badbde74ce22074ff142d1a9265c7a168a65d6

namespace MusicXML2 
{

/*!
\addtogroup utilities
@{
*/


//______________________________________________________________________________
/*!
\internal
\brief Provides easy indentation of text output.
*/
class EXP timingItem : public smartable
{
  public:
    enum timingItemKind { kMandatory, kOptional };
      
    static SMARTP<timingItem> createTimingItem (
      std::string    activity,
      std::string    description,
      timingItemKind kind,
      std::clock_t   startClock,
      std::clock_t   endClock);

    timingItem (
      std::string    activity,
      std::string    description,
      timingItemKind kind,
      std::clock_t   startClock,
      std::clock_t   endClock);
      
<<<<<<< HEAD
    std::string           fActivity;
    std::string           fDescription;
    timingItemKind        fKind;
    clock_t               fStartClock;
    clock_t               fEndClock;
=======
    std::string  	fActivity;
    std::string 	fDescription;
    timingItemKind	fKind;
    std::clock_t 	fStartClock;
    std::clock_t  	fEndClock;
>>>>>>> a9badbde74ce22074ff142d1a9265c7a168a65d6
};

typedef SMARTP<timingItem> S_timingItem;

class EXP timing {
  public:
			 timing ();
    virtual ~timing ();

    // global variable for general use
    static timing gTiming; 

    // add an item
<<<<<<< HEAD
    void                  appendTimingItem (
                            std::string    activity,
                            std::string    description,
                            timingItem::timingItemKind
                                           kind,
                            clock_t        startClock,
                            clock_t        endClock);
      
    // print
    void                  print (std::ostream& os) const;

  private:

    std::list<S_timingItem>
                          fTimingItemsList;
=======
    void    appendTimingItem (
						std::string    activity,
						std::string    description,
						timingItem::timingItemKind kind,
						std::clock_t   startClock,
						std::clock_t   endClock);
      
    // print
    void    print (std::ostream& os) const;

  private:
    std::list<S_timingItem>    fTimingItemsList;
>>>>>>> a9badbde74ce22074ff142d1a9265c7a168a65d6
};
std::ostream& operator<< (std::ostream& os, const timing& tim);

//______________________________________________________________________________
/*!
\internal
\brief Provides easy indentation of text output.
*/
class EXP indenter
{
  public:
<<<<<<< HEAD

    indenter (std::string spacer = "  ");
=======
			  indenter (std::string spacer = "  ");
>>>>>>> a9badbde74ce22074ff142d1a9265c7a168a65d6
    virtual ~indenter ();

    // get the indent
    int                   getIndent () const 	{ return fIndent; }
                         
    // increase the indentation by 1
    indenter&             operator++ (const int value);
    
    // decrease the indentation by 1
    indenter&             operator-- (const int value);

    indenter&             increment (int value);
    indenter&             decrement (int value);

    // reset the indentation
    void                  resetToZero ()		{ fIndent = 0; }

    // output as much space as specified
    void                  print (std::ostream& os) const;

    // get a spacer for adhoc uses, without increasing the indentation
<<<<<<< HEAD
    std::string           getSpacer () const
                              { return fSpacer; }

    // indent a multiline 'R"(...)"' std::string
=======
    std::string           getSpacer () const	{ return fSpacer; }

    // indent a multiline 'R"(...)"' string
>>>>>>> a9badbde74ce22074ff142d1a9265c7a168a65d6
    std::string                indentMultiLineString (std::string value);
    
    // global variable for general use
    static indenter       gIndenter; 

  private:
    int                   fIndent;
    std::string           fSpacer;
};
std::ostream& operator<< (std::ostream& os, const indenter& idtr);

// useful shortcut macros
#define gIndenter indenter::gIndenter
#define gTab      indenter::gIndenter.getSpacer ()

//______________________________________________________________________________
<<<<<<< HEAD
class indentedOstream: public std::ostream
=======
class EXP indentedOstream: public std::ostream
>>>>>>> a9badbde74ce22074ff142d1a9265c7a168a65d6
{
/*
Reference:
 https://stackoverflow.com/questions/2212776/overload-handling-of-stdendl

Usage:
  indentedOstream myStream (std::cout);
   
  myStream <<
    1 << 2 << 3 << std::endl <<
    5 << 6 << std::endl <<
    7 << 8 << std::endl;
*/
 
  // a stream buffer that prefixes each line
  // with the current indentation
  class indentedStreamBuf: public std::stringbuf
  {
    private:
<<<<<<< HEAD
    
      std::ostream& fOutput;
      indenter&     fIndenter;
=======
      std::ostream&  fOutput;
      indenter&      fIndenter;
>>>>>>> a9badbde74ce22074ff142d1a9265c7a168a65d6

    public:
      // constructor
<<<<<<< HEAD
      indentedStreamBuf (
        std::ostream& str,
        indenter&     idtr)
        : fOutput (str),
          fIndenter (idtr)
        {}
=======
      indentedStreamBuf ( std::ostream& str, indenter& idtr) : fOutput (str), fIndenter (idtr)	{}
>>>>>>> a9badbde74ce22074ff142d1a9265c7a168a65d6

      // flush
      void flush ()		{ fOutput.flush (); }
    
      // When we sync the stream with fOutput:
      // 1) uutput the indentation then the buffer
      // 2) reset the buffer
      // 3) flush the actual output stream we are using.
      virtual int sync ()
      {
        fOutput << fIndenter << str ();
        str ("");
        fOutput.flush ();
        return 0;
      }
  };

  private:
    // indentedOstream just uses a version of indentedStreamBuf
    indentedStreamBuf     fIndentedStreamBuf;
  
  public:
    // constructor
<<<<<<< HEAD
    indentedOstream (
      std::ostream&  str,
      indenter&      idtr)
      : std::ostream (&fIndentedStreamBuf),
        fIndentedStreamBuf (
          str, idtr)
=======
    indentedOstream ( std::ostream&  str  = std::cerr, indenter& idtr = indenter::gIndenter)
      : std::ostream (&fIndentedStreamBuf), fIndentedStreamBuf (str, idtr)
>>>>>>> a9badbde74ce22074ff142d1a9265c7a168a65d6
      {}

    // destructor
    virtual ~indentedOstream () {};

    // flush
    void flush () 			{ fIndentedStreamBuf.flush (); }
    
<<<<<<< HEAD
    // global variables for general use
    static indentedOstream
                          gOutputIndentedOstream; 
    static indentedOstream
                          gLogIndentedOstream; 
};

// useful shortcut macros
#define gOutputIOstream indentedOstream::gOutputIndentedOstream
#define gLogIOstream    indentedOstream::gLogIndentedOstream
=======
    // global variable for general use
    static indentedOstream gLogIndentedOstream;
};

// useful shortcut macro
#define gLogIOstream indentedOstream::gLogIndentedOstream

//______________________________________________________________________________
/*!
\internal
\brief Provides easy control of output lines length.
*/
class outputLineElementsCounter
{
  public:

    outputLineElementsCounter (
      std::ostream& ostream,
      int      maxElementsPerLine = 10);
      
    virtual ~outputLineElementsCounter ();

    // get the counter
    int                   getElementsCounter () const
                              { return fElementsCounter; }
                         
    // increase the counter by 1
    outputLineElementsCounter&
                          operator++ (int value);
    
    // increase the counter by 'value' at once without checking
    // for the maximun number of elements per line in-between
    outputLineElementsCounter&
                          increment (int value);
    
    // set the maximum number of elements per line
    void                  setMaxElementsPerLine (int maxElementsPerLine)
                              {
                                fMaxElementsPerLine = maxElementsPerLine;
                              }

    
    // reset the counter
    void                  resetToZero ()
                              { fElementsCounter = 0; }
    
    // compare the counter with a value
    bool                  operator<  (int value)
                              { return fElementsCounter < value; }
    bool                  operator<= (int value)
                              { return fElementsCounter <= value; }
    bool                  operator== (int value)
                              { return fElementsCounter == value; }
    bool                  operator!= (int value)
                              { return fElementsCounter != value; }
    bool                  operator>= (int value)
                              { return fElementsCounter >= value; }
    bool                  operator>  (int value)
                              { return fElementsCounter > value; }
    
    // global variable for general use
    static outputLineElementsCounter
                          gOutputLineElementsCounter; 

  private:
    std::ostream&         fOstream;
    int                   fElementsCounter;
    int                   fMaxElementsPerLine;
};
>>>>>>> a9badbde74ce22074ff142d1a9265c7a168a65d6

/*!
\brief A utility to escape quotes in strings.
*/
//______________________________________________________________________________
struct stringQuoteEscaper
{
  /* usage:
      string dest = "";
      for_each( source.begin (), source.end (), stringQuoteEscaper (dest));
  */

  std::string&            target;
  
<<<<<<< HEAD
  explicit                stringQuoteEscaper (std::string& t)
                            : target (t)
                              {}
=======
  explicit                stringQuoteEscaper (std::string& t) : target (t) {}
>>>>>>> a9badbde74ce22074ff142d1a9265c7a168a65d6

  void                    operator() (char ch) const
                              {
                                 if( ch == '"') {
                                   // or switch on any character that 
                                   // needs escaping like '\' itself
                                    target.push_back ('\\');
                                 }
                                 target.push_back (ch);
                              }
};

/*!
\brief A utility to remove spaces in strings.
*/
//______________________________________________________________________________
struct stringSpaceRemover
{
  /* usage:
      std::string dest = "";
      for_each (
        source.begin (),
        source.end (),
        stringSpaceRemover (dest));
  */

  std::string&            target;
  
  explicit                stringSpaceRemover (std::string& t)
                            : target (t)
                              {}

  void                    operator() (char ch) const
                              {
                                if (ch != ' ') {
                                  target.push_back (ch);
                                }
                              }
};

/*!
\brief A utility to replace spaces in strings.
*/
//______________________________________________________________________________
struct stringSpaceReplacer
{
  /* usage:
      std::string dest = "";
      for_each (
        source.begin (),
        source.end (),
        stringSpaceReplacer (dest, ersatz));
  */

  std::string&            target;
  char                    ersatz;
  
  explicit                stringSpaceReplacer (std::string& t, char ch)
                            : target (t), ersatz (ch)
                              {}

  void                    operator() (char ch) const
                              {
                                if (ch == ' ')
                                  target.push_back (ersatz);
                                else
                                  target.push_back (ch);
                              }
};

/*!
\brief A utility to convert small positive integers to English words.
*/
//______________________________________________________________________________
<<<<<<< HEAD
std::string replicateString (
  std::string str,
  int    times);
=======
std::string replicateString ( std::string str, int times);
>>>>>>> a9badbde74ce22074ff142d1a9265c7a168a65d6

/*!
\brief A utility to convert small positive integers to English words.
*/
//______________________________________________________________________________
std::string int2EnglishWord (int n);

/*!
\brief A utility to escape quotes in strings.
*/
//______________________________________________________________________________
std::string stringNumbersToEnglishWords (std::string str);

/*!
\brief A utility to build a list of number from a specification.
  A specification can be:
    7,15-19,^16-17,
    meaning that 7, 15, 18 and 19 have to enlisted
*/
//______________________________________________________________________________
<<<<<<< HEAD
std::set<int> decipherNumbersSetSpecification (
  std::string theSpecification,
  bool   debugMode = false);
=======
std::set<int> decipherNumbersSetSpecification ( std::string theSpecification, bool   debugMode = false);
>>>>>>> a9badbde74ce22074ff142d1a9265c7a168a65d6

//______________________________________________________________________________
std::list<int> extractNumbersFromString (
  std::string theString, // can contain "1, 2, 17"
  bool   debugMode = false);

//______________________________________________________________________________
// from http://stackoverflow.com/questions/216823/whats-the-best-way-to-trim-stdstring
// trim string from start
inline std::string &ltrim (std::string &s) {
<<<<<<< HEAD
  std::function <int (int)> checkSpace =
    [] (int x) { return isspace (x); };

  s.erase (
    s.begin (),
    find_if (
      s.begin (),
      s.end (),
      std::not1 (checkSpace)
      )
    );
          
=======
  s.erase ( s.begin (), find_if (s.begin (), s.end (), not1 (std::ptr_fun<int, int> (isspace))));
>>>>>>> a9badbde74ce22074ff142d1a9265c7a168a65d6
  return s;
}

// trim string from end
inline std::string &rtrim (std::string &s) {
<<<<<<< HEAD
  std::function <int (int)> checkSpace =
    [] (int x) { return isspace (x); };

  s.erase (
    find_if (
      s.rbegin (),
      s.rend (),
      std::not1 (checkSpace)
      ).base(),
    s.end ()
    );
          
=======
  s.erase ( find_if (s.rbegin (), s.rend (), not1 (std::ptr_fun<int, int> (isspace)) ).base(), s.end () );
>>>>>>> a9badbde74ce22074ff142d1a9265c7a168a65d6
  return s;
}




// trim string from both ends
inline std::string &trim (std::string &s) {
  return ltrim (rtrim (s));
}

//______________________________________________________________________________
std::pair<std::string, std::string> extractNamesPairFromString (
  std::string theString, // can contain "P1 = Bassoon"
  char   separator,
  bool   debugMode = false);

//______________________________________________________________________________
<<<<<<< HEAD
std::string quoteStringIfNonAlpha (
  std::string theString);

std::string quoteString (
  std::string theString);
=======
std::string quoteStringIfNonAlpha (std::string theString);
std::string quoteString (std::string theString);
>>>>>>> a9badbde74ce22074ff142d1a9265c7a168a65d6

//______________________________________________________________________________
std::string booleanAsString (bool value);

//______________________________________________________________________________
<<<<<<< HEAD
std::string singularOrPlural (
  int number, std::string singularName, std::string pluralName);

std::string singularOrPluralWithoutNumber (
  int number, std::string singularName, std::string pluralName);

//______________________________________________________________________________
void optionError (std::string errorMessage);

//______________________________________________________________________________
void convertHTMLEntitiesToPlainCharacters (std::string& s);

//______________________________________________________________________________
void splitStringContainingEndOfLines (
  std::string        theString,
  std::list<std::string>& chunksList);
=======
std::string singularOrPlural (int number, std::string singularName, std::string pluralName);

std::string singularOrPluralWithoutNumber ( int number, std::string singularName, std::string pluralName);

//______________________________________________________________________________
void optionError (std::string errorMessage);

//______________________________________________________________________________
void convertHTMLEntitiesToPlainCharacters (std::string& s);
>>>>>>> a9badbde74ce22074ff142d1a9265c7a168a65d6

//______________________________________________________________________________
std::string baseName (const std::string &filename);
  // wait until c++17 for a standard library containing basename()...

//______________________________________________________________________________
<<<<<<< HEAD
std::string makeSingleWordFromString (const std::string& theString);
=======
std::string makeSingleWordFromString (const std::string &string);
>>>>>>> a9badbde74ce22074ff142d1a9265c7a168a65d6


} // namespace MusicXML2


#endif
