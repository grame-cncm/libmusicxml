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

#include "Mikrokosmos3WanderingOahTypes.h"


using namespace std;

namespace MusicXML2
{

//______________________________________________________________________________
string generatedCodeKindAsString (generatedCodeKind kind)
{
  // no CamelCase here, these strings are used in the command line options
  string result;

  switch (kind) {
    case k_NoGeneratedCode:
      result = "*noGeneratedCode*";
      break;
    case kGuido:
      result = "guido";
      break;
    case kLilyPond:
      result = "lilypond";
      break;
    case kBrailleMusic:
      result = "braille";
      break;
    case kMusicXML:
      result = "musicxml";
      break;
  } // switch

  return result;
}

map<string, generatedCodeKind>
  gGlobalGeneratedCodeKindsMap;

void initializeGeneratedCodeKindsMap ()
{
  gGlobalGeneratedCodeKindsMap ["guido"]    = kGuido;
  gGlobalGeneratedCodeKindsMap ["lilypond"] = kLilyPond;     // default
  gGlobalGeneratedCodeKindsMap ["braille"]  = kBrailleMusic;
  gGlobalGeneratedCodeKindsMap ["musicxml"] = kMusicXML;
}

string existingGeneratedCodeKinds (unsigned int namesListMaxLength)
{
  stringstream s;

  int
    brailleOutputKindsMapSize =
      gGlobalGeneratedCodeKindsMap.size ();

  if (brailleOutputKindsMapSize) {
    unsigned int nextToLast =
      brailleOutputKindsMapSize - 1;

    unsigned int count = 0;
    unsigned int cumulatedLength = 0;

    for (
      map<string, generatedCodeKind>::const_iterator i =
        gGlobalGeneratedCodeKindsMap.begin ();
      i != gGlobalGeneratedCodeKindsMap.end ();
      ++i
    ) {
      string theString = (*i).first;

      ++count;

      cumulatedLength += theString.size ();
      if (cumulatedLength >= namesListMaxLength) {
        s << "\n";
        cumulatedLength = 0;
      }

      s << theString;

      if (count == nextToLast) {
        s << " and ";
      }
      else if (count != brailleOutputKindsMapSize) {
        s << ", ";
      }
    } // for
  }

  return s.str ();
}


}
