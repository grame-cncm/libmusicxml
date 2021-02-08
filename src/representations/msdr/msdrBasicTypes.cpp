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
#include "msdrBasicTypes.h"

#include "enableTracingIfDesired.h"
#ifdef TRACING_IS_ENABLED
  #include "traceOah.h"
#endif

#include "oahOah.h"
#include "generalOah.h"

#include "msdrOah.h"

#include "messagesHandling.h"


using namespace std;

namespace MusicXML2
{

// pitchs languages
//______________________________________________________________________________

map<string, msdrKeywordsLanguageKind>
  gGlobalMsdrKeywordsLanguageKindsMap;

void initializeMsdrKeywordsLanguageKindsMap ()
{
  gGlobalMsdrKeywordsLanguageKindsMap ["english"]  = kKeywordsEnglish; // default
  gGlobalMsdrKeywordsLanguageKindsMap ["francais"] = kKeywordsFrench;
  gGlobalMsdrKeywordsLanguageKindsMap ["italiano"] = kKeywordsItalian;
  gGlobalMsdrKeywordsLanguageKindsMap ["deutsch"]  = kKeywordsGerman;
  gGlobalMsdrKeywordsLanguageKindsMap ["espanol"]  = kKeywordsEspanol;
}

string msdrKeywordsLanguageKindAsString (
  msdrKeywordsLanguageKind languageKind)
{
  string result;

  switch (languageKind) {
    case kKeywordsEnglish: // default value
      result = "keywordsEnglish";
      break;
    case kKeywordsFrench:
      result = "keywordsFrench";
      break;
    case kKeywordsItalian:
      result = "keywordsItalian";
      break;
    case kKeywordsGerman:
      result = "keywordsGerman";
      break;
    case kKeywordsEspanol:
      result = "keywordsEspanol";
      break;
  } // switch

  return result;
}

string existingMsdrKeywordsLanguageKinds (unsigned int namesListMaxLength)
{
  stringstream s;

  unsigned int
    msdrKeywordsLanguageKindsMapSize =
      gGlobalMsdrKeywordsLanguageKindsMap.size ();

  if (msdrKeywordsLanguageKindsMapSize) {
    unsigned int nextToLast =
      msdrKeywordsLanguageKindsMapSize - 1;

    unsigned int count = 0;
    unsigned int cumulatedLength = 0;

    for (
      map<string, msdrKeywordsLanguageKind>::const_iterator i =
        gGlobalMsdrKeywordsLanguageKindsMap.begin ();
      i != gGlobalMsdrKeywordsLanguageKindsMap.end ();
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
      else if (count != msdrKeywordsLanguageKindsMapSize) {
        s << ", ";
      }
    } // for
  }

  return s.str ();
}

// pitches languages
//______________________________________________________________________________

map<string, msdrPitchesLanguageKind>
  gGlobalMsdrPitchesLanguageKindsMap;

void initializeMsdrPitchesLanguageKindsMap ()
{
  gGlobalMsdrPitchesLanguageKindsMap ["english"]  = kPitchesEnglish; // default
  gGlobalMsdrPitchesLanguageKindsMap ["francais"] = kPitchesFrench;
  gGlobalMsdrPitchesLanguageKindsMap ["italiano"] = kPitchesItalian;
  gGlobalMsdrPitchesLanguageKindsMap ["deutsch"]  = kPitchesGerman;
  gGlobalMsdrPitchesLanguageKindsMap ["espanol"]  = kPitchesEspanol;
}

string msdrPitchesLanguageKindAsString (
  msdrPitchesLanguageKind languageKind)
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

string existingMsdrPitchesLanguageKinds (unsigned int namesListMaxLength)
{
  stringstream s;

  unsigned int
    msdrPitchesLanguageKindsMapSize =
      gGlobalMsdrPitchesLanguageKindsMap.size ();

  if (msdrPitchesLanguageKindsMapSize) {
    unsigned int nextToLast =
      msdrPitchesLanguageKindsMapSize - 1;

    unsigned int count = 0;
    unsigned int cumulatedLength = 0;

    for (
      map<string, msdrPitchesLanguageKind>::const_iterator i =
        gGlobalMsdrPitchesLanguageKindsMap.begin ();
      i != gGlobalMsdrPitchesLanguageKindsMap.end ();
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
      else if (count != msdrPitchesLanguageKindsMapSize) {
        s << ", ";
      }
    } // for
  }

  return s.str ();
}

//______________________________________________________________________________
void initializeMSDRBasicTypes ()
{
  // protect library against multiple initializations
  static bool pThisMethodHasBeenRun = false;

  if (! pThisMethodHasBeenRun) {
#ifdef TRACING_IS_ENABLED
    if (gGlobalTraceOahGroup->getTraceOah () && ! gGlobalGeneralOahGroup->getQuiet ()) {
      gLogStream <<
        "Initializing MSDR basic types handling" <<
        endl;
    }
#endif

    // MSDR keywords languages handling
    // ------------------------------------------------------

    initializeMsdrKeywordsLanguageKindsMap ();

    // MSDR pitches languages handling
    // ------------------------------------------------------

    initializeMsdrPitchesLanguageKindsMap ();

    pThisMethodHasBeenRun = true;
  }
}


}
