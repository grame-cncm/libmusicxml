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
#include <sstream>

#include <climits>      // INT_MIN, INT_MAX

#include "utilities.h"

#include "msrBasicTypes.h"
#include "msdlBasicTypes.h"

#include "enableTracingIfDesired.h"
#ifdef TRACING_IS_ENABLED
  #include "traceOah.h"
#endif

#include "oahOah.h"
#include "generalOah.h"

#include "msdlOah.h"

#include "waeMessagesHandling.h"


using namespace std;

namespace MusicXML2
{

// pitches languages
//______________________________________________________________________________

map<string, msdlPitchesLanguageKind>
  gGlobalMsdlPitchesLanguageKindsMap;

void initializeMsdlPitchesLanguageKindsMap ()
{
  gGlobalMsdlPitchesLanguageKindsMap ["english"]  = kPitchesEnglish; // default
  gGlobalMsdlPitchesLanguageKindsMap ["francais"] = kPitchesFrench;
  gGlobalMsdlPitchesLanguageKindsMap ["italiano"] = kPitchesItalian;
  gGlobalMsdlPitchesLanguageKindsMap ["deutsch"]  = kPitchesGerman;
  gGlobalMsdlPitchesLanguageKindsMap ["espanol"]  = kPitchesEspanol;
}

string msdlPitchesLanguageKindAsString (
  msdlPitchesLanguageKind languageKind)
{
  string result;

  switch (languageKind) {
    case kPitchesEnglish: // default value
      result = "pitchesEnglish";
      break;
    case kPitchesFrench:
      result = "pitchesFrench";
      break;
    case kPitchesItalian:
      result = "pitchesItalian";
      break;
    case kPitchesGerman:
      result = "pitchesGerman";
      break;
    case kPitchesEspanol:
      result = "pitchesEspanol";
      break;
  } // switch

  return result;
}

string existingMsdlPitchesLanguageKinds (unsigned int namesListMaxLength)
{
  stringstream s;

  unsigned int
    msdlPitchesLanguageKindsMapSize =
      gGlobalMsdlPitchesLanguageKindsMap.size ();

  if (msdlPitchesLanguageKindsMapSize) {
    unsigned int nextToLast =
      msdlPitchesLanguageKindsMapSize - 1;

    unsigned int count = 0;
    unsigned int cumulatedLength = 0;

    for (
      map<string, msdlPitchesLanguageKind>::const_iterator i =
        gGlobalMsdlPitchesLanguageKindsMap.begin ();
      i != gGlobalMsdlPitchesLanguageKindsMap.end ();
      ++i
    ) {
      string theString = (*i).first;

      cumulatedLength += theString.size ();
      if (cumulatedLength >= namesListMaxLength) {
        s << "\n";
        cumulatedLength = 0;
        break;
      }

      s << theString;

      if (count == nextToLast) {
        s << " and ";
      }
      else if (count != msdlPitchesLanguageKindsMapSize) {
        s << ", ";
      }
    } // for
  }

  return s.str ();
}

//______________________________________________________________________________
void initializeMSDLBasicTypes ()
{
  // protect library against multiple initializations
  static bool pThisMethodHasBeenRun = false;

  if (! pThisMethodHasBeenRun) {
#ifdef TRACING_IS_ENABLED
    if (getTraceOah () && ! gGlobalGeneralOahGroup->getQuiet ()) {
      gLogStream <<
        "Initializing MSDL basic types handling" <<
        endl;
    }
#endif

    // MSDL keywords languages handling
    // ------------------------------------------------------

    initializeMsdlKeywordsLanguageKindsMap ();

    // MSDL pitches languages handling
    // ------------------------------------------------------

    initializeMsdlPitchesLanguageKindsMap ();

    pThisMethodHasBeenRun = true;
  }
}


}
