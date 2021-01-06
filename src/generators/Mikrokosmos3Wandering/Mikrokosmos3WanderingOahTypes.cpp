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

#include "messagesHandling.h"

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
string mkkGenerateCodeKindAsString (
  mkkGenerateCodeKind generateCodeKind)
{
  // no CamelCase here, these strings are used in the command line options
  string result;

  switch (generateCodeKind) {
    case k_NoGenerateCode:
      result = "*noGenerateCode*";
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

EXP mkkGenerateCodeKind mkkGenerateCodeKindFromString (
  const string& theString)
{
  mkkGenerateCodeKind result = k_NoGenerateCode;

  if      (theString == "guido") {
    result = kGuido;
  }
  else if (theString == "lilypond") {
    result = kLilyPond;
  }
  else if (theString == "braille") {
    result = kBrailleMusic;
  }
  else if (theString == "musicxml") {
    result = kMusicXML;
  }
  else {
    stringstream s;

    s <<
      "the string \"" <<
      theString <<
      "\" is no valid generate code kind";

    msgAssert (false, s.str ());
  }

  return result;
}

map<string, mkkGenerateCodeKind>
  gGlobalGenerateCodeKindsMap;

void initializeGenerateCodeKindsMap ()
{
  gGlobalGenerateCodeKindsMap ["guido"]    = kGuido;
  gGlobalGenerateCodeKindsMap ["lilypond"] = kLilyPond;
  gGlobalGenerateCodeKindsMap ["braille"]  = kBrailleMusic;
  gGlobalGenerateCodeKindsMap ["musicxml"] = kMusicXML;
}

string existingGenerateCodeKinds (
  unsigned int namesListMaxLength)
{
  stringstream s;

  unsigned int brailleOutputKindsMapSize =
    gGlobalGenerateCodeKindsMap.size ();

  if (brailleOutputKindsMapSize) {
    unsigned int nextToLast =
      brailleOutputKindsMapSize - 1;

    unsigned int count = 0;
    unsigned int cumulatedLength = 0;

    for (
      map<string, mkkGenerateCodeKind>::const_iterator i =
        gGlobalGenerateCodeKindsMap.begin ();
      i != gGlobalGenerateCodeKindsMap.end ();
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
