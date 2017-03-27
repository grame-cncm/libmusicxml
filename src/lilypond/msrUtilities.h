/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef __msrUtilities__
#define __msrUtilities__

#include <string>
#include <cassert>
#include <iostream>
#include <set>
#include <list>
#include <algorithm> 

#include "smartpointer.h"

#include "basevisitor.h"

using namespace std;
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
class timingItem : public smartable {
  public:

    enum timingItemKind {
      kMandatory, kOptional};
      
    timingItem (
      string         activity,
      timingItemKind kind,
      clock_t        startClock,
      clock_t        endClock);
      
    static SMARTP<timingItem> createTimingItem (
      string                     activity,
      timingItem::timingItemKind kind,
      clock_t                    startClock,
      clock_t                    endClock);

    string          fActivity;
    timingItemKind  fKind;
    clock_t         fStartClock;
    clock_t         fEndClock;
};

typedef SMARTP<timingItem> S_timingItem;

class timing {
  public:

    timing ();
      
    virtual ~timing();

    // global variable for general use
    static timing gTiming; 

    // add an item
    void appendTimingItem (
      string                     activity,
      timingItem::timingItemKind kind,
      clock_t                    startClock,
      clock_t                    endClock);
      
    // print
    void print (ostream& os) const;

  private:

    list<S_timingItem>  fTimingItemsList;
};
ostream& operator<< (ostream& os, const timing& tim);

//______________________________________________________________________________
/*!
\internal
\brief Provides easy indentation of text output.
*/
class indenter {
  public:

    indenter (string spacer = "  ");
    virtual ~indenter();

    // increase the indentation
    indenter& operator++ (const int value);
    
    // decrease the indentation
    indenter& operator-- (const int value);

    // output as much space as specified
    void print (ostream& os) const;

    // global variable for general use
    static indenter gIndenter; 

  private:

    int    fIndent;
    string fSpacer;
};
ostream& operator<< (ostream& os, const indenter& idtr);

//______________________________________________________________________________
/*!
\internal
\brief Provides easy control of output lines length.
*/
class outputLineElementsCounter {
  public:

    outputLineElementsCounter (int maxElementsPerLine = 10);
    virtual ~outputLineElementsCounter();

    // increase the counter
    outputLineElementsCounter& operator++ (int value);
    
    // set the maximum number of elements per line
    void setMaxElementsPerLine (int maxElementsPerLine);
    
    // reset the counter
    void reset (int value = 0);
    
    // compare the counter with a value
    bool operator< (int value);
    bool operator<= (int value);
    bool operator== (int value);
    bool operator!= (int value);
    bool operator>= (int value);
    bool operator> (int value);
    
    // global variable for general use
    static outputLineElementsCounter gOutputLineElementsCounter; 

  private:

    int    fElementsCounter;
    int    fMaxElementsPerLine;
};

/*!
\brief A utility to escape quotes in strings.
*/
//______________________________________________________________________________
struct stringQuoteEscaper
{
  /* usage:
      string dest = "";
      for_each( source.begin(), source.end(), stringQuoteEscaper (dest));
  */

  string&  target;
  explicit stringQuoteEscaper (string& t) : target (t) {}

  void operator() (char ch) const
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
        source.begin(),
        source.end(),
        stringSpaceRemover (dest));
  */

  string&  target;
  
  explicit stringSpaceRemover (string& t)
    : target (t)
    {}

  void operator() (char ch) const
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
        source.begin(),
        source.end(),
        stringSpaceReplacer (dest, ersatz));
  */

  string&  target;
  char     ersatz;
  
  explicit stringSpaceReplacer (string& t, char ch)
    : target (t), ersatz (ch)
    {}

  void operator() (char ch) const
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
string int2EnglishWord (int n);

/*!
\brief A utility to escape quotes in strings.
*/
//______________________________________________________________________________
string stringNumbersToEnglishWords (string str);

/*!
\brief A utility to build a list of number from a specification.
  A specification can be:
    7,15-19,^16-17,
    meaning that 7, 15, 18 and 19 have to enlisted
*/
//______________________________________________________________________________
set<int> decipherNumbersSetSpecification (
  string theSpecification,
  bool   debugMode = false);

//______________________________________________________________________________
list<int> extractNumbersFromString (
  string theString, // can contain "1, 2, 17"
  bool   debugMode = false);

//______________________________________________________________________________
// from http://stackoverflow.com/questions/216823/whats-the-best-way-to-trim-stdstring
// trim string from start
inline string &ltrim (string &s) {
  s.erase (
    s.begin(),
    find_if (
      s.begin(),
      s.end(),
      not1 (ptr_fun<int, int> (isspace))
      )
    );
          
  return s;
}

// trim string from end
inline string &rtrim (string &s) {
  s.erase (
    find_if (
      s.rbegin(),
      s.rend(),
      not1 (ptr_fun<int, int> (isspace))
      ).base(),
    s.end()
    );
          
  return s;
}

// trim string from both ends
inline string &trim (string &s) {
  return ltrim (rtrim (s));
}

//______________________________________________________________________________
pair<string, string> extractNamesPairFromString (
  string theString, // can contain "P1 = Bassoon"
  char   separator,
  bool   debugMode = false);

//______________________________________________________________________________
string quoteStringIfNonAlpha (
  string theString);

//______________________________________________________________________________
string booleanAsString (bool value);

//______________________________________________________________________________
string singularOrPlural (
  int number, string singularName, string pluralName);

//______________________________________________________________________________
void optionError (string errorMessage);


} // namespace MusicXML2


#endif
