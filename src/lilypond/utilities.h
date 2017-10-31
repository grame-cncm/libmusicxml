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

#include <set>
#include <list>
#include <algorithm>
#include <ctime>

#include "smartpointer.h"
#include "basevisitor.h"

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
class timingItem : public smartable
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
      
    std::string  	fActivity;
    std::string 	fDescription;
    timingItemKind	fKind;
    std::clock_t 	fStartClock;
    std::clock_t  	fEndClock;
};

typedef SMARTP<timingItem> S_timingItem;

class timing {
  public:
			 timing ();
    virtual ~timing ();

    // global variable for general use
    static timing gTiming; 

    // add an item
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
};
std::ostream& operator<< (std::ostream& os, const timing& tim);

//______________________________________________________________________________
/*!
\internal
\brief Provides easy indentation of text output.
*/
class indenter
{
  public:
			  indenter (std::string spacer = "  ");
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
    std::string           getSpacer () const	{ return fSpacer; }

    // indent a multiline 'R"(...)"' string
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
class indentedOstream: public std::ostream
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
      std::ostream&  fOutput;
      indenter&      fIndenter;

    public:
      // constructor
      indentedStreamBuf ( std::ostream& str, indenter& idtr) : fOutput (str), fIndenter (idtr)	{}

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
    indentedOstream ( std::ostream&  str  = std::cerr, indenter& idtr = indenter::gIndenter)
      : std::ostream (&fIndentedStreamBuf), fIndentedStreamBuf (str, idtr)
      {}

    // destructor
    virtual ~indentedOstream () {};

    // flush
    void flush () 			{ fIndentedStreamBuf.flush (); }
    
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
  
  explicit                stringQuoteEscaper (std::string& t) : target (t) {}

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
      string dest = "";
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
      string dest = "";
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
std::string replicateString ( std::string str, int times);

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
std::set<int> decipherNumbersSetSpecification ( std::string theSpecification, bool   debugMode = false);

//______________________________________________________________________________
std::list<int> extractNumbersFromString (
  std::string theString, // can contain "1, 2, 17"
  bool   debugMode = false);

//______________________________________________________________________________
// from http://stackoverflow.com/questions/216823/whats-the-best-way-to-trim-stdstring
// trim string from start
inline std::string &ltrim (std::string &s) {
  s.erase ( s.begin (), find_if (s.begin (), s.end (), not1 (std::ptr_fun<int, int> (isspace))));
  return s;
}

// trim string from end
inline std::string &rtrim (std::string &s) {
  s.erase ( find_if (s.rbegin (), s.rend (), not1 (std::ptr_fun<int, int> (isspace)) ).base(), s.end () );
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
std::string quoteStringIfNonAlpha (std::string theString);
std::string quoteString (std::string theString);

//______________________________________________________________________________
std::string booleanAsString (bool value);

//______________________________________________________________________________
std::string singularOrPlural (int number, std::string singularName, std::string pluralName);

std::string singularOrPluralWithoutNumber ( int number, std::string singularName, std::string pluralName);

//______________________________________________________________________________
void optionError (std::string errorMessage);

//______________________________________________________________________________
void convertHTMLEntitiesToPlainCharacters (std::string& s);

//______________________________________________________________________________
std::string baseName (const std::string &filename);
  // wait until c++17 for a standard library containing basename()...

//______________________________________________________________________________
std::string makeSingleWordFromString (const std::string &string);


} // namespace MusicXML2


#endif
