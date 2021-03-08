/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef __msr2msdrInterface__
#define __msr2msdrInterface__

#include "exports.h"

#include "libmusicxml.h" // for xmlErr

//#include "msdr.h"

#include "msdlParser.h"

#include "generalOah.h"
#include "msrOah.h"


namespace MusicXML2
{
//_______________________________________________________________________________
EXP xmlErr istream2msdrWithHandler (
  string       inputSourceName,
  istream&     inputStream,
  ostream&     out,
  ostream&     err,
  S_oahHandler handler);

//_______________________________________________________________________________
EXP xmlErr msdlFile2msdrWithHandler (
  string       fileName,
  ostream&     out,
  ostream&     err,
  S_oahHandler handler);

//_______________________________________________________________________________
EXP xmlErr msdlString2msdrWithHandler (
  const char * buffer,
  ostream&     out,
  ostream&     err,
  S_oahHandler handler);

/* JMI
//_______________________________________________________________________________
S_msrScore convertMsdlScoreToMsdrScore (
  const char * fileName,
  istream&     inputStream,
  string       passNumber,
  string       passDescription);

//_______________________________________________________________________________
EXP void displayMsdrScore (
  const S_msdrScore theMsdrScore,
  S_msrOahGroup     msrOpts,
  S_msdrOahGroup    msdrOpts);

EXP void displayMsdrScoreShort (
  const S_msdrScore theMsdrScore,
  S_msrOahGroup     msrOpts,
  S_msdrOahGroup    msdrOpts);

//_______________________________________________________________________________
EXP void displayMsdrScore_OptionalPass (
  const S_msdrScore theMsdrScore,
  S_msrOahGroup     msrOpts,
  S_msdrOahGroup    msdrOpts);

EXP void displayMsdrScoreShort_OptionalPass (
  const S_msdrScore theMsdrScore,
  S_msrOahGroup     msrOpts,
  S_msdrOahGroup    msdrOpts);
*/


} // namespace MusicXML2


#endif
