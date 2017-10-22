/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#include "utilities.h"

#include "messagesHandling.h"

#include "generalOptions.h"


using namespace std;

namespace MusicXML2 
{

//______________________________________________________________________________
void msrWarning (
  string context,
  string inputSourceName,
  int    inputLineNumber,
  string message)
{
  const int fieldWidth = 18;

  gLogIOstream << left <<
    endl <<
    "!!! " << context << " WARNING !!!" <<
    endl <<
    setw (fieldWidth) <<
    "input source name" << " : " <<
    inputSourceName <<
    endl <<
    setw (fieldWidth) <<
    "input line" << " : " << inputLineNumber <<
    endl <<

  gIndenter++;

  gLogIOstream <<
    message <<
    endl;

  gIndenter--;
}

//______________________________________________________________________________
void msrMusicXMLWarning (
  int    inputLineNumber,
  string message)
{
  msrWarning (
    "MusicXML",
    gGeneralOptions->fInputSourceName,
    inputLineNumber,
    message);
}

//______________________________________________________________________________
void msrInternalWarning (
  int    inputLineNumber,
  string message)
{
  msrWarning (
    "INTERNAL",
    gGeneralOptions->fInputSourceName,
    inputLineNumber,
    message);
}

//______________________________________________________________________________
void msrError (
  string context,
  string inputSourceName,
  int    inputLineNumber,
  string message)
{
  const int fieldWidth = 18;

  gLogIOstream << left <<
    endl <<
    "### " << context << " ERROR ###" <<
    endl <<
    setw (fieldWidth) <<
    "input source name" << " : " <<
    inputSourceName <<
    endl <<
    setw (fieldWidth) <<
    "input line" << " : " << inputLineNumber <<
    endl <<

  gIndenter++;

  gLogIOstream <<
    message <<
    endl;

  gIndenter--;
}


}
