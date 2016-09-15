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

/*!
\brief A utility to escape quotes in strings.
*/
//______________________________________________________________________________
struct stringquoteescaper
{
  std::string& target;
  explicit stringquoteescaper( std::string& t ) : target( t ) {}

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

//std::string dest;
//std::for_each( source.begin(), source.end(), stringquoteescaper(dest));

/*!
\brief A utility to convert small positive integers to English words.
*/
//______________________________________________________________________________
std::string int2EnglishWord (int n);


} // namespace MusicXML2


#endif
