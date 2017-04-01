/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#include <iostream>
#include <iomanip>      // setw, set::precision, ...

#include "msrVersion.h"

#include "msrUtilities.h"

using namespace std;

namespace MusicXML2 
{

//_______________________________________________________________________________
int         MSRVersionNumber ()
{
  return 010;
}

const char* MSRVersionNumberAsString ()
{
  return "0.1.0";
}

//______________________________________________________________________________
versionInfo::versionInfo (
  string  versionNumber,
  string  versionDate,
  string  versionDescription)
{
  fVersionNumber      = versionNumber;
  fVersionDate        = versionDate;
  fVersionDescription = versionDescription;
}

versionInfo::~versionInfo () {}

void versionInfo::print (ostream& os)
{
  os <<
    setw(10) << fVersionNumber <<
    "(" << fVersionDate << ")" <<
    endl <<
    
  indenter::gIndenter++;

  os <<
    fVersionDescription;

  indenter::gIndenter--;  
}


}
