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

// user languages
//______________________________________________________________________________

map<string, msdlUserLanguageKind>
  gGlobalMsdlUserLanguageKindsMap;

void initializeMsdlUserLanguageKindsMap ()
{
  gGlobalMsdlUserLanguageKindsMap ["english"]  = kUserLanguageEnglish; // default
  gGlobalMsdlUserLanguageKindsMap ["francais"] = kUserLanguageFrench;
  gGlobalMsdlUserLanguageKindsMap ["italiano"] = kUserLanguageItalian;
  gGlobalMsdlUserLanguageKindsMap ["deutsch"]  = kUserLanguageGerman;
  gGlobalMsdlUserLanguageKindsMap ["espanol"]  = kUserLanguageSpanish;
}

string msdlUserLanguageKindAsString (
  msdlUserLanguageKind languageKind)
{
  string result;

  switch (languageKind) {
    case kUserLanguageEnglish: // default value
      result = "userLanguageEnglish";
      break;
    case kUserLanguageFrench:
      result = "userLanguageFrench";
      break;
    case kUserLanguageItalian:
      result = "userLanguageItalian";
      break;
    case kUserLanguageGerman:
      result = "userLanguageGerman";
      break;
    case kUserLanguageSpanish:
      result = "userLanguageSpanish";
      break;
    case kUserLanguageDutch:
      result = "userLanguageDutch";
      break;
  } // switch

  return result;
}

msdlUserLanguageKind msdlUserLanguageKindFromString (std::string theString)
{
  msdlUserLanguageKind result = kUserLanguageEnglish; // MSDL default

  map<string, msdlUserLanguageKind>::const_iterator
    it =
      gGlobalMsdlUserLanguageKindsMap.find (
        theString);

  if (it == gGlobalMsdlUserLanguageKindsMap.end ()) {
    // no, User language kind is unknown in the map
    stringstream s;

    s <<
      "MSDL language kind '" << theString <<
      "' is unknown" <<
      endl <<
      "The " <<
      gGlobalMsdlUserLanguageKindsMap.size () - 1 <<
      " known MSDL language kinds are:" <<
      endl;

    ++gIndenter;

    s <<
      existingMsdlUserLanguageKinds (K_NAMES_LIST_MAX_LENGTH);

    --gIndenter;

// JMI    oahError (s.str ());
  }

  result = (*it).second;

  return result;
}

string existingMsdlUserLanguageKinds (unsigned int namesListMaxLength)
{
  stringstream s;

  unsigned int
    msdlUserLanguageKindsMapSize =
      gGlobalMsdlUserLanguageKindsMap.size ();

  if (msdlUserLanguageKindsMapSize) {
    unsigned int nextToLast =
      msdlUserLanguageKindsMapSize - 1;

    unsigned int count = 0;
    unsigned int cumulatedLength = 0;

    for (
      map<string, msdlUserLanguageKind>::const_iterator i =
        gGlobalMsdlUserLanguageKindsMap.begin ();
      i != gGlobalMsdlUserLanguageKindsMap.end ();
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
      else if (count != msdlUserLanguageKindsMapSize) {
        s << ", ";
      }
    } // for
  }

  return s.str ();
}

// comments types
//______________________________________________________________________________

map<string, msdlCommentsTypeKind>
  gGlobalMsdlCommentsTypeKindsMap;

msdlCommentsTypeKind msdlCommentsTypeKindFromString (std::string theString)
{
  msdlCommentsTypeKind result = kCommentsTypePercent; // MSDL default

  map<string, msdlCommentsTypeKind>::const_iterator
    it =
      gGlobalMsdlCommentsTypeKindsMap.find (
        theString);

  if (it == gGlobalMsdlCommentsTypeKindsMap.end ()) {
    // no, keywords language kind is unknown in the map
    stringstream s;

    s <<
      "MSDL language kind '" << theString <<
      "' is unknown" <<
      endl <<
      "The " <<
      gGlobalMsdlCommentsTypeKindsMap.size () - 1 <<
      " known MSDL language kinds are:" <<
      endl;

    ++gIndenter;

    s <<
      existingMsdlCommentsTypeKinds (K_NAMES_LIST_MAX_LENGTH);

    --gIndenter;

// JMI    oahError (s.str ());
  }

  result = (*it).second;

  return result;
}

void initializeMsdlCommentsTypeKindsMap ()
{
  // protect library against multiple initializations
  static bool pThisMethodHasBeenRun = false;

  if (! pThisMethodHasBeenRun) {
    gGlobalMsdlCommentsTypeKindsMap ["percent"] = kCommentsTypePercent;
    gGlobalMsdlCommentsTypeKindsMap ["star"]    = kCommentsTypeStar;

    pThisMethodHasBeenRun = true;
  }
}

string msdlCommentsTypeKindAsString (
  msdlCommentsTypeKind languageKind)
{
  string result;

  switch (languageKind) {
    case kCommentsTypePercent: // MSDL default
      result = "commentsTypePercent";
      break;
    case kCommentsTypeStar:
      result = "commentsTypePercent";
      break;
  } // switch

  return result;
}

string existingMsdlCommentsTypeKinds (unsigned int namesListMaxLength)
{
  stringstream s;

  unsigned int
    msdlCommentsTypeKindsMapSize =
      gGlobalMsdlCommentsTypeKindsMap.size ();

  if (msdlCommentsTypeKindsMapSize) {
    unsigned int nextToLast =
      msdlCommentsTypeKindsMapSize - 1;

    unsigned int count = 0;
    unsigned int cumulatedLength = 0;

    for (
      map<string, msdlCommentsTypeKind>::const_iterator i =
        gGlobalMsdlCommentsTypeKindsMap.begin ();
      i != gGlobalMsdlCommentsTypeKindsMap.end ();
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
      else if (count != msdlCommentsTypeKindsMapSize) {
        s << ", ";
      }
    } // for
  }

  return s.str ();
}

// pitches languages
//______________________________________________________________________________

map<string, msdlPitchesLanguageKind>
  gGlobalMsdlPitchesLanguageKindsMap;

void initializeMsdlPitchesLanguageKindsMap ()
{
  gGlobalMsdlPitchesLanguageKindsMap ["english"]  = kPitchesLanguageEnglish; // default
  gGlobalMsdlPitchesLanguageKindsMap ["francais"] = kPitchesLanguageFrench;
  gGlobalMsdlPitchesLanguageKindsMap ["italiano"] = kPitchesLanguageItalian;
  gGlobalMsdlPitchesLanguageKindsMap ["deutsch"]  = kPitchesLanguageGerman;
  gGlobalMsdlPitchesLanguageKindsMap ["espanol"]  = kPitchesLanguageSpanish;
}

string msdlPitchesLanguageKindAsString (
  msdlPitchesLanguageKind languageKind)
{
  string result;

  switch (languageKind) {
    case kPitchesLanguageEnglish: // default value
      result = "pitchesEnglish";
      break;
    case kPitchesLanguageFrench:
      result = "pitchesFrench";
      break;
    case kPitchesLanguageItalian:
      result = "pitchesItalian";
      break;
    case kPitchesLanguageGerman:
      result = "pitchesGerman";
      break;
    case kPitchesLanguageSpanish:
      result = "pitchesEspanol";
      break;
    case kPitchesLanguageDutch:
      result = "pitchesDutch";
      break;
  } // switch

  return result;
}

msdlPitchesLanguageKind msdlPitchesLanguageKindFromString (std::string theString)
{
  msdlPitchesLanguageKind result = kPitchesLanguageEnglish; // MSDL default

  map<string, msdlPitchesLanguageKind>::const_iterator
    it =
      gGlobalMsdlPitchesLanguageKindsMap.find (
        theString);

  if (it == gGlobalMsdlPitchesLanguageKindsMap.end ()) {
    // no, Pitches language kind is unknown in the map
    stringstream s;

    s <<
      "MSDL language kind '" << theString <<
      "' is unknown" <<
      endl <<
      "The " <<
      gGlobalMsdlPitchesLanguageKindsMap.size () - 1 <<
      " known MSDL language kinds are:" <<
      endl;

    ++gIndenter;

    s <<
      existingMsdlPitchesLanguageKinds (K_NAMES_LIST_MAX_LENGTH);

    --gIndenter;

// JMI    oahError (s.str ());
  }

  result = (*it).second;

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

    // MSDL user languages handling
    // ------------------------------------------------------

    initializeMsdlUserLanguageKindsMap ();

    // MSDL keywords languages handling
    // ------------------------------------------------------

    initializeMsdlKeywordsLanguageKindsMap ();

    // MSDL comments types handling
    // ------------------------------------------------------

    initializeMsdlCommentsTypeKindsMap ();

    // MSDL pitches languages handling
    // ------------------------------------------------------

    initializeMsdlPitchesLanguageKindsMap ();

    pThisMethodHasBeenRun = true;
  }
}


}
