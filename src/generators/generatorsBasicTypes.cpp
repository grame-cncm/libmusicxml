/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#include "visitor.h"

#include "generatorsBasicTypes.h"


using namespace std;

namespace MusicXML2
{

//______________________________________________________________________________
string generatorOutputKindAsString (
  generatorOutputKind generatorOutputKind)
{
  // no CamelCase here, these strings are used in the command line options
  string result;

  switch (generatorOutputKind) {
    case k_NoOutput:
      result = "*noGenerateOutputKind*";
      break;
    case kGuidoOutput:
      result = "Guido";
      break;
    case kLilyPondOutput:
      result = "LilyPond";
      break;
    case kBrailleOutput:
      result = "braille";
      break;
    case kMusicXMLOutput:
      result = "MusicXML";
      break;
    case kMidiOutput:
      result = "MIDI";
      break;
  } // switch

  return result;
}

EXP generatorOutputKind generatorOutputKindFromString (
  const string& theString)
{
  generatorOutputKind result = k_NoOutput;

  if      (theString == "guido") {
    result = kGuidoOutput;
  }
  else if (theString == "lilypond") {
    result = kLilyPondOutput;
  }
  else if (theString == "braille") {
    result = kBrailleOutput;
  }
  else if (theString == "musicxml") {
    result = kMusicXMLOutput;
  }
  else if (theString == "midi") {
    result = kMidiOutput;
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

map<string, generatorOutputKind>
  gGlobalGeneratorOutputKindsMap;

void initializeGeneratorOutputKindsMap ()
{
  gGlobalGeneratorOutputKindsMap ["guido"]    = kGuidoOutput;
  gGlobalGeneratorOutputKindsMap ["lilypond"] = kLilyPondOutput;
  gGlobalGeneratorOutputKindsMap ["braille"]  = kBrailleOutput;
  gGlobalGeneratorOutputKindsMap ["musicxml"] = kMusicXMLOutput;
  gGlobalGeneratorOutputKindsMap ["midi"] = kMidiOutput;
}

string existingGeneratorOutputKinds (
  unsigned int namesListMaxLength)
{
  stringstream s;

  unsigned int brailleOutputKindsMapSize =
    gGlobalGeneratorOutputKindsMap.size ();

  if (brailleOutputKindsMapSize) {
    unsigned int nextToLast =
      brailleOutputKindsMapSize - 1;

    unsigned int count = 0;
    unsigned int cumulatedLength = 0;

    for (
      map<string, generatorOutputKind>::const_iterator i =
        gGlobalGeneratorOutputKindsMap.begin ();
      i != gGlobalGeneratorOutputKindsMap.end ();
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
