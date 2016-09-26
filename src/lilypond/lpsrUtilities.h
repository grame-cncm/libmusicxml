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

#include "basevisitor.h"

namespace MusicXML2 
{

//______________________________________________________________________________
/*!
\internal
\brief A function to exit under certain circumstances
*/
void lpsrAssert (bool condition, std::string messageIfFalse);

//______________________________________________________________________________
/*!
\internal
\brief To be used in place of std::endl
  to provide easy indentation of text output.
  The name is a pun on endl...
*/
class haendel {
  public:

    haendel(std::string spcr = "  ") : fIndent(0), fSpacer(spcr) {}
    virtual ~haendel() {}

    //! increase the indentation
    haendel& operator++ (int)  { fIndent++; return *this; }
    //! decrease the indentation
    haendel& operator-- (int)  { fIndent--; return *this; }
    //! reset the indentation to none
    void print(std::ostream& os) const;

  private:

    int         fIndent;
    std::string fSpacer;
};
std::ostream& operator<< (std::ostream& os, const haendel& eol);

/*!
\brief A utility to escape quotes in strings.
*/
//______________________________________________________________________________
struct stringQuoteEscaper
{
  /* usage:
      std::string dest;
      std::for_each( source.begin(), source.end(), stringQuoteEscaper(dest));
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
\brief A utility to convert small positive integers to English words.
*/
//______________________________________________________________________________
std::string int2EnglishWord (int n);

/*!
\brief A utility to escape quotes in strings.
*/
//______________________________________________________________________________
std::string stringNumbersToEnglishWords (std::string str);


} // namespace MusicXML2


#endif
