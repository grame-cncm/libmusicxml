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

#include "basevisitor.h"

using namespace std;
namespace MusicXML2 
{

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
    indenter& operator++ (int)
        {
          fIndent++;
          return *this;
        }
    
    // decrease the indentation
    indenter& operator-- (int)
        {
          fIndent--;
          
          if (fIndent < 0) {
            cerr <<
              endl <<
              "### Indentation has become negative..." << endl <<
              endl;
            assert(false);
          }
            
          return *this;
        }
    
    // reset the indentation to none
    void print (ostream& os) const;

    // global variable for general use
    static indenter gIndenter; 

  private:

    int    fIndent;
    string fSpacer;
};
ostream& operator<< (ostream& os, const indenter& number);

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
int noteTypeAsDivisions (
  string  noteType,
  int     divisionsPerWholeNote,
  string& errorMessage,
  bool    debugMode = false);

//______________________________________________________________________________
string divisionsAsMSRDuration (
  int     divisions,
  int     divisionsPerWholeNote,
  string& errorMessage,
  bool    debugMode = false);

string divisionsAsMSRDuration (
  int     divisions,
  int     divisionsPerWholeNote,
  int     inputSourceSuppliedNumberOfDots,
  int&    computedNumberOfDots,
  string& errorMessage,
  bool    debugMode = false);

//______________________________________________________________________________
string noteTypeAsMSRDuration (
  string  noteType,
  string& errorMessage);

//______________________________________________________________________________
string quoteStringIfNonAlpha (
  string     theString);


} // namespace MusicXML2


#endif
