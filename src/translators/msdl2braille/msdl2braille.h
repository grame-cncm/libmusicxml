/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef ___msdl2braille___
#define ___msdl2braille___

#include "libmusicxml.h" // for xmlErr


using namespace std;

namespace MusicXML2
{

//_______________________________________________________________________________
EXP xmlErr msdlStream2brailleWithOptionsVector (
  istream&             inputStream,
  const optionsVector& options,
  std::ostream&        out,
  std::ostream&        err);

EXP xmlErr msdlStream2brailleWithHandler (
  istream&     inputStream,
  S_oahHandler handler,
  ostream&     out,
  ostream&     err);

//_______________________________________________________________________________
EXP xmlErr msdlFile2brailleWithOptionsVector (
  string               fileName,
  const optionsVector& options,
  std::ostream&        out,
  std::ostream&        err);

EXP xmlErr msdlFile2brailleWithHandler (
  string       fileName,
  S_oahHandler handler,
  ostream&     out,
  ostream&     err);

//_______________________________________________________________________________
EXP xmlErr msdlString2brailleWithOptionsVector (
  const char *         buffer,
  const optionsVector& options,
  std::ostream&        out,
  std::ostream&        err);

EXP xmlErr msdlString2brailleWithHandler (
  const char * buffer,
  S_oahHandler handler,
  ostream&     out,
  ostream&     err);


}


#endif
