/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#include "version.h"
#include "utilities.h"

#include "enableTracingIfDesired.h"
#ifdef TRACING_IS_ENABLED
  #include "traceOah.h"
#endif

#include "oahOah.h"

#include "exNihiloOahTypes.h"


using namespace std;

namespace MusicXML2
{

//______________________________________________________________________________
string generatedCodeKindAsString (generatedCodeKind kind)
{
  string result;

  switch (kind) {
    case kNoGeneratedCode:
      result = "*NoGeneratedCode*";
      break;
    case kGuido:
      result = "Guido";
      break;
    case kLilyPond:
      result = "LilyPond";
      break;
    case kBrailleMusic:
      result = "BrailleMusic";
      break;
    case kMusicXML:
      result = "MusicXML";
      break;
  } // switch

  return result;
}

//______________________________________________________________________________
generatedCodeKind gGeneratedCodeKind = kNoGeneratedCode;

void registerGgeneratedCodeKind (generatedCodeKind kind)
{
  if (gGeneratedCodeKind != kNoGeneratedCode) {
    cerr << "only one of '-guido', '-lilypond', '-braille' and '-musicxml' can be used" << endl;
    exit (2);
  }
  else {
    gGeneratedCodeKind = kind;
  }
}


}
