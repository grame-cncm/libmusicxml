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
#include <set>

#include "basevisitor.h"

namespace MusicXML2 
{

//______________________________________________________________________________
/*!
\internal
\brief Provides easy indentation of text output.
*/
class indenter {
  public:

    indenter (std::string spacer = "  ");
    virtual ~indenter();

    // increase the indentation
    indenter& operator++ (int) { fIndent++; return *this; }
    
    // decrease the indentation
    indenter& operator-- (int) { fIndent--; return *this; }
    
    // reset the indentation to none
    void print(std::ostream& os) const;

 // JMI   static indenter gIndenter; // global variable for general use

  private:

    int         fIndent;
    std::string fSpacer;
};
std::ostream& operator<< (std::ostream& os, const indenter& number);

/*!
\internal
\brief A macro to simplify indentation
*/
//______________________________________________________________________________
// JMI #define idtr indenter::gIndenter

/*!
\brief A utility to escape quotes in strings.
*/
//______________________________________________________________________________
struct stringQuoteEscaper
{
  /* usage:
      std::string dest = "";
      std::for_each( source.begin(), source.end(), stringQuoteEscaper (dest));
  */

  std::string& target;
  explicit     stringQuoteEscaper( std::string& t ) : target( t ) {}

  void operator()( char ch ) const
  {
     if( ch == '\'') // or switch on any character that 
                     // needs escaping like \ itself
     {
        target.push_back('\\');
     }
     target.push_back( ch );
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
      std::for_each( source.begin(), source.end(), stringSpaceRemover (dest));
  */

  std::string& target;
  explicit     stringSpaceRemover( std::string& t ) : target( t ) {}

  void operator()( char ch ) const
  {
     if( ch != ' ') {
       target.push_back( ch );
     }
  }
};

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
std::set<int> decipherNumbersSpecification (
  char* theString,
  bool  debugMode = false);


} // namespace MusicXML2


#endif
