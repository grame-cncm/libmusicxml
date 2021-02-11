/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

/*
#include <iostream>
#include <sstream>

#include "utilities.h"
*/

#include "msrBasicTypes.h"

#include "msdrKeywords.h"

#include "enableTracingIfDesired.h"
#ifdef TRACING_IS_ENABLED
  #include "traceOah.h"
#endif

#include "generalOah.h"


using namespace std;

namespace MusicXML2
{

// the MSDL keywords languages
//______________________________________________________________________________

map<string, msdrKeywordsLanguageKind>
  gGlobalMsdrKeywordsLanguageKindsMap;

msdrKeywordsLanguageKind msdrKeywordsLanguageKindFromString (std::string theString)
{
  msdrKeywordsLanguageKind result = kKeywordsEnglish; // MSDL default

  map<string, msdrKeywordsLanguageKind>::const_iterator
    it =
      gGlobalMsdrKeywordsLanguageKindsMap.find (
        theString);

  if (it == gGlobalMsdrKeywordsLanguageKindsMap.end ()) {
    // no, keywords language kind is unknown in the map
    stringstream s;

    s <<
      "MSDL language kind '" << theString <<
      "' is unknown" <<
      endl <<
      "The " <<
      gGlobalMsdrKeywordsLanguageKindsMap.size () - 1 <<
      " known MSDL language kinds are:" <<
      endl;

    ++gIndenter;

    s <<
      existingMsdrKeywordsLanguageKinds (K_NAMES_LIST_MAX_LENGTH);

    --gIndenter;

// JMI    oahError (s.str ());
  }

  result = (*it).second;

  return result;
}

void initializeMsdrKeywordsLanguageKindsMap ()
{
  // protect library against multiple initializations
  static bool pThisMethodHasBeenRun = false;

  if (! pThisMethodHasBeenRun) {
    gGlobalKeywordsLanguagesKindsMap ["english"]   = kKeywordsEnglish;
    gGlobalKeywordsLanguagesKindsMap ["francais"]  = kKeywordsFrench;
    gGlobalKeywordsLanguagesKindsMap ["italiano"]  = kKeywordsItalian;
    gGlobalKeywordsLanguagesKindsMap ["francais"]  = kKeywordsGerman;
    gGlobalKeywordsLanguagesKindsMap ["espanol"]   = kKeywordsSpanish;
    gGlobalKeywordsLanguagesKindsMap ["nedelands"] = kKeywordsNederlands;

    initializeEnglishKeywordsNamesMap ();
    initializeFrenchKeywordsNamesMap ();
    initializeItalianKeywordsNamesMap ();
    initializeGermanKeywordsNamesMap ();
    initializeSpanishKeywordsNamesMap ();
    initializeDutchKeywordsNamesMap ();

    pThisMethodHasBeenRun = true;
  }
}

string msdrKeywordsLanguageKindAsString (
  msdrKeywordsLanguageKind languageKind)
{
  string result;

  switch (languageKind) {
    case k_NoKeyword: // default value
      result = "*noKeyword*";
      break;
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
    case kKeywordsSpanish:
      result = "keywordsSpanish";
      break;
    case kKeywordsNederlands:
      result = "keywordsNederlands";
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

map<string, msdrKeywordsLanguageKind> gGlobalKeywordsLanguagesKindsMap;

map<msdrKeywordKind, string> gGlobalEnglishKeywordsNamesMap;
map<msdrKeywordKind, string> gGlobalFrenchKeywordsNamesMap;
map<msdrKeywordKind, string> gGlobalItalianKeywordsNamesMap;
map<msdrKeywordKind, string> gGlobalGermanKeywordsNamesMap;
map<msdrKeywordKind, string> gGlobalSpanishKeywordsNamesMap;
map<msdrKeywordKind, string> gGlobalDutchKeywordsNamesMap;

// the MSDL keywords
//______________________________________________________________________________
void initializeEnglishKeywordsNamesMap ()
{
  // english
  gGlobalEnglishKeywordsNamesMap [kMsdrKeywordPitches]   = "pitches";

  gGlobalEnglishKeywordsNamesMap [kMsdrKeywordAnacrusis] = "anacrusis";

  gGlobalEnglishKeywordsNamesMap [kMsdrKeywordBook]      = "book";
  gGlobalEnglishKeywordsNamesMap [kMsdrKeywordScore]     = "score";
  gGlobalEnglishKeywordsNamesMap [kMsdrKeywordPartGroup] = "partgroup";
  gGlobalEnglishKeywordsNamesMap [kMsdrKeywordGroup]     = "group";
  gGlobalEnglishKeywordsNamesMap [kMsdrKeywordStaff]     = "staff";
  gGlobalEnglishKeywordsNamesMap [kMsdrKeywordVoice]     = "voice";
  gGlobalEnglishKeywordsNamesMap [kMsdrKeywordFragment]  = "fragment";

  gGlobalEnglishKeywordsNamesMap [kMsdrKeywordClef]      = "clef";
  gGlobalEnglishKeywordsNamesMap [kMsdrKeywordTreble]    = "soprano";
  gGlobalEnglishKeywordsNamesMap [kMsdrKeywordAlto]      = "alto";
  gGlobalEnglishKeywordsNamesMap [kMsdrKeywordTenor]     = "tenor";
  gGlobalEnglishKeywordsNamesMap [kMsdrKeywordBaryton]   = "baryton";
  gGlobalEnglishKeywordsNamesMap [kMsdrKeywordBass]      = "bass";

  gGlobalEnglishKeywordsNamesMap [kMsdrKeywordKey]       = "key";

  gGlobalEnglishKeywordsNamesMap [kMsdrKeywordTime]      = "time";

  gGlobalEnglishKeywordsNamesMap [kMsdrKeywordDoubleBar] = "doubleBar";
  gGlobalEnglishKeywordsNamesMap [kMsdrKeywordFinalBar]  = "finalBar";
}

void initializeFrenchKeywordsNamesMap ()
{
  // french
  gGlobalEnglishKeywordsNamesMap [kMsdrKeywordPitches]   = "hauteurDesNotes";

  gGlobalEnglishKeywordsNamesMap [kMsdrKeywordAnacrusis] = "anacrouse";

  gGlobalEnglishKeywordsNamesMap [kMsdrKeywordBook]      = "livre";
  gGlobalEnglishKeywordsNamesMap [kMsdrKeywordScore]     = "partition";
  gGlobalEnglishKeywordsNamesMap [kMsdrKeywordPartGroup] = "partgroup";
  gGlobalEnglishKeywordsNamesMap [kMsdrKeywordGroup]     = "groupe";
  gGlobalEnglishKeywordsNamesMap [kMsdrKeywordStaff]     = "portée";
  gGlobalEnglishKeywordsNamesMap [kMsdrKeywordVoice]     = "voix";
  gGlobalEnglishKeywordsNamesMap [kMsdrKeywordFragment]  = "fragment";

  gGlobalEnglishKeywordsNamesMap [kMsdrKeywordClef]      = "clé";
  gGlobalEnglishKeywordsNamesMap [kMsdrKeywordTreble]    = "soprano";
  gGlobalEnglishKeywordsNamesMap [kMsdrKeywordAlto]      = "alto";
  gGlobalEnglishKeywordsNamesMap [kMsdrKeywordTenor]     = "ténor";
  gGlobalEnglishKeywordsNamesMap [kMsdrKeywordBaryton]   = "baryton";
  gGlobalEnglishKeywordsNamesMap [kMsdrKeywordBass]      = "basse";

  gGlobalEnglishKeywordsNamesMap [kMsdrKeywordKey]       = "armure";

  gGlobalEnglishKeywordsNamesMap [kMsdrKeywordTime]      = "métrique";

  gGlobalEnglishKeywordsNamesMap [kMsdrKeywordDoubleBar] = "barreDouble";
  gGlobalEnglishKeywordsNamesMap [kMsdrKeywordFinalBar]  = "barreFinale";
}

void initializeItalianKeywordsNamesMap ()
{
  // english
  gGlobalEnglishKeywordsNamesMap [kMsdrKeywordPitches]   = "pitches";

  gGlobalEnglishKeywordsNamesMap [kMsdrKeywordAnacrusis] = "anacrusis";

  gGlobalEnglishKeywordsNamesMap [kMsdrKeywordBook]      = "libro";
  gGlobalEnglishKeywordsNamesMap [kMsdrKeywordScore]     = "partitura";
  gGlobalEnglishKeywordsNamesMap [kMsdrKeywordPartGroup] = "partgroup";
  gGlobalEnglishKeywordsNamesMap [kMsdrKeywordGroup]     = "group";
  gGlobalEnglishKeywordsNamesMap [kMsdrKeywordStaff]     = "staff";
  gGlobalEnglishKeywordsNamesMap [kMsdrKeywordVoice]     = "voice";
  gGlobalEnglishKeywordsNamesMap [kMsdrKeywordFragment]  = "fragment";

  gGlobalEnglishKeywordsNamesMap [kMsdrKeywordClef]      = "clef";
  gGlobalEnglishKeywordsNamesMap [kMsdrKeywordTreble]    = "soprano";
  gGlobalEnglishKeywordsNamesMap [kMsdrKeywordAlto]      = "alto";
  gGlobalEnglishKeywordsNamesMap [kMsdrKeywordTenor]     = "tenor";
  gGlobalEnglishKeywordsNamesMap [kMsdrKeywordBaryton]   = "baryton";
  gGlobalEnglishKeywordsNamesMap [kMsdrKeywordBass]      = "bass";

  gGlobalEnglishKeywordsNamesMap [kMsdrKeywordKey]       = "key";

  gGlobalEnglishKeywordsNamesMap [kMsdrKeywordTime]      = "time";

  gGlobalEnglishKeywordsNamesMap [kMsdrKeywordDoubleBar] = "doubleBar";
  gGlobalEnglishKeywordsNamesMap [kMsdrKeywordFinalBar]  = "finalBar";
}

void initializeGermanKeywordsNamesMap ()
{
  // english
  gGlobalEnglishKeywordsNamesMap [kMsdrKeywordPitches]   = "pitches";

  gGlobalEnglishKeywordsNamesMap [kMsdrKeywordAnacrusis] = "anacrusis";

  gGlobalEnglishKeywordsNamesMap [kMsdrKeywordBook]      = "Buch";
  gGlobalEnglishKeywordsNamesMap [kMsdrKeywordScore]     = "score";
  gGlobalEnglishKeywordsNamesMap [kMsdrKeywordPartGroup] = "partgroup";
  gGlobalEnglishKeywordsNamesMap [kMsdrKeywordGroup]     = "group";
  gGlobalEnglishKeywordsNamesMap [kMsdrKeywordStaff]     = "staff";
  gGlobalEnglishKeywordsNamesMap [kMsdrKeywordVoice]     = "Stimme";
  gGlobalEnglishKeywordsNamesMap [kMsdrKeywordFragment]  = "fragment";

  gGlobalEnglishKeywordsNamesMap [kMsdrKeywordClef]      = "clef";
  gGlobalEnglishKeywordsNamesMap [kMsdrKeywordTreble]    = "soprano";
  gGlobalEnglishKeywordsNamesMap [kMsdrKeywordAlto]      = "alto";
  gGlobalEnglishKeywordsNamesMap [kMsdrKeywordTenor]     = "tenor";
  gGlobalEnglishKeywordsNamesMap [kMsdrKeywordBaryton]   = "baryton";
  gGlobalEnglishKeywordsNamesMap [kMsdrKeywordBass]      = "bass";

  gGlobalEnglishKeywordsNamesMap [kMsdrKeywordKey]       = "key";

  gGlobalEnglishKeywordsNamesMap [kMsdrKeywordTime]      = "time";

  gGlobalEnglishKeywordsNamesMap [kMsdrKeywordDoubleBar] = "doubleBar";
  gGlobalEnglishKeywordsNamesMap [kMsdrKeywordFinalBar]  = "finalBar";
}

void initializeSpanishKeywordsNamesMap ()
{
  // english
  gGlobalEnglishKeywordsNamesMap [kMsdrKeywordPitches]   = "pitches";

  gGlobalEnglishKeywordsNamesMap [kMsdrKeywordAnacrusis] = "anacrusis";

  gGlobalEnglishKeywordsNamesMap [kMsdrKeywordBook]      = "book";
  gGlobalEnglishKeywordsNamesMap [kMsdrKeywordScore]     = "score";
  gGlobalEnglishKeywordsNamesMap [kMsdrKeywordPartGroup] = "partgroup";
  gGlobalEnglishKeywordsNamesMap [kMsdrKeywordGroup]     = "group";
  gGlobalEnglishKeywordsNamesMap [kMsdrKeywordStaff]     = "staff";
  gGlobalEnglishKeywordsNamesMap [kMsdrKeywordVoice]     = "voice";
  gGlobalEnglishKeywordsNamesMap [kMsdrKeywordFragment]  = "fragment";

  gGlobalEnglishKeywordsNamesMap [kMsdrKeywordClef]      = "clef";
  gGlobalEnglishKeywordsNamesMap [kMsdrKeywordTreble]    = "soprano";
  gGlobalEnglishKeywordsNamesMap [kMsdrKeywordAlto]      = "alto";
  gGlobalEnglishKeywordsNamesMap [kMsdrKeywordTenor]     = "tenor";
  gGlobalEnglishKeywordsNamesMap [kMsdrKeywordBaryton]   = "baryton";
  gGlobalEnglishKeywordsNamesMap [kMsdrKeywordBass]      = "bass";

  gGlobalEnglishKeywordsNamesMap [kMsdrKeywordKey]       = "key";

  gGlobalEnglishKeywordsNamesMap [kMsdrKeywordTime]      = "time";

  gGlobalEnglishKeywordsNamesMap [kMsdrKeywordDoubleBar] = "doubleBar";
  gGlobalEnglishKeywordsNamesMap [kMsdrKeywordFinalBar]  = "finalBar";
}

void initializeDutchKeywordsNamesMap ()
{
  // english
  gGlobalEnglishKeywordsNamesMap [kMsdrKeywordPitches]   = "pitches";

  gGlobalEnglishKeywordsNamesMap [kMsdrKeywordAnacrusis] = "anacrusis";

  gGlobalEnglishKeywordsNamesMap [kMsdrKeywordBook]      = "book";
  gGlobalEnglishKeywordsNamesMap [kMsdrKeywordScore]     = "score";
  gGlobalEnglishKeywordsNamesMap [kMsdrKeywordPartGroup] = "partgroup";
  gGlobalEnglishKeywordsNamesMap [kMsdrKeywordGroup]     = "group";
  gGlobalEnglishKeywordsNamesMap [kMsdrKeywordStaff]     = "staff";
  gGlobalEnglishKeywordsNamesMap [kMsdrKeywordVoice]     = "voice";
  gGlobalEnglishKeywordsNamesMap [kMsdrKeywordFragment]  = "fragment";

  gGlobalEnglishKeywordsNamesMap [kMsdrKeywordClef]      = "clef";
  gGlobalEnglishKeywordsNamesMap [kMsdrKeywordTreble]    = "soprano";
  gGlobalEnglishKeywordsNamesMap [kMsdrKeywordAlto]      = "alto";
  gGlobalEnglishKeywordsNamesMap [kMsdrKeywordTenor]     = "tenor";
  gGlobalEnglishKeywordsNamesMap [kMsdrKeywordBaryton]   = "baryton";
  gGlobalEnglishKeywordsNamesMap [kMsdrKeywordBass]      = "bass";

  gGlobalEnglishKeywordsNamesMap [kMsdrKeywordKey]       = "key";

  gGlobalEnglishKeywordsNamesMap [kMsdrKeywordTime]      = "time";

  gGlobalEnglishKeywordsNamesMap [kMsdrKeywordDoubleBar] = "doubleBar";
  gGlobalEnglishKeywordsNamesMap [kMsdrKeywordFinalBar]  = "finalBar";
}


// the MSDL keywords
//______________________________________________________________________________
string msdrKeywordKindAsString (msdrKeywordKind keywordKind)
{
  string result;

  switch (keywordKind) {
    case k_NoMsdrKeywordKind:
      result = "*noMsdrKeywordKind*";
      break;

    case kMsdrKeywordPitches:
      result = "msdrKeywordPitches";
      break;

    case kMsdrKeywordBook:
      result = "msdrKeywordBook";
      break;
    case kMsdrKeywordScore:
      result = "msdrKeywordScore";
      break;
    case kMsdrKeywordPartGroup:
      result = "msdrKeywordPartGroup";
      break;
    case kMsdrKeywordGroup:
      result = "msdrKeywordGroup";
      break;
    case kMsdrKeywordStaff:
      result = "msdrKeywordStaff";
      break;
    case kMsdrKeywordVoice:
      result = "msdrKeywordVoice";
      break;
    case kMsdrKeywordFragment:
      result = "msdrKeywordFragment";
      break;

    case kMsdrKeywordAnacrusis:
      result = "msdrKeywordAnacrusis";
      break;

    case kMsdrKeywordClef:
      result = "msdrKeywordClef";
      break;
    case kMsdrKeywordTreble:
      result = "msdrKeywordTreble";
      break;
    case kMsdrKeywordAlto:
      result = "msdrKeywordAlto";
      break;
    case kMsdrKeywordTenor:
      result = "msdrKeywordTenor";
      break;
    case kMsdrKeywordBaryton:
      result = "msdrKeywordBaryton";
      break;
    case kMsdrKeywordBass:
      result = "msdrKeywordBass";
      break;

    case kMsdrKeywordKey:
      result = "msdrKeywordKey";
      break;

    case kMsdrKeywordTime:
      result = "msdrKeywordTime";
      break;

    case kMsdrKeywordDoubleBar:
      result = "msdrKeywordDoubleBar";
      break;
    case kMsdrKeywordFinalBar:
      result = "msdrKeywordFinalBar";
      break;
  } // switch

  return result;
}

msdrKeywordKind msdrKeywordKindFromString (
  msdrKeywordsLanguageKind languageKind,
  string                   keywordName)
{
  msdrKeywordKind result = k_NoMsdrKeywordKind;

  map<msdrKeywordKind, string> *keywordsNamesMapPTR = nullptr;

  // select the relevant keywords names map
  switch (languageKind) {
    case k_NoKeyword:
      break;

    case kKeywordsEnglish:
      keywordsNamesMapPTR = &gGlobalEnglishKeywordsNamesMap;
      break;
    case kKeywordsFrench:
      keywordsNamesMapPTR = &gGlobalFrenchKeywordsNamesMap;
      break;
    case kKeywordsItalian:
      keywordsNamesMapPTR = &gGlobalItalianKeywordsNamesMap;
      break;
    case kKeywordsGerman:
      keywordsNamesMapPTR = &gGlobalGermanKeywordsNamesMap;
      break;
    case kKeywordsSpanish:
      keywordsNamesMapPTR = &gGlobalSpanishKeywordsNamesMap;
      break;
    case kKeywordsNederlands:
      keywordsNamesMapPTR = &gGlobalDutchKeywordsNamesMap;
      break;
  } // switch

  if (keywordsNamesMapPTR) {
    // is quarterToneskeywordsName present in the map?
    if (keywordsNamesMapPTR->size ()) {
      map<msdrKeywordKind, string>::const_iterator
        iBegin = (*keywordsNamesMapPTR).begin (),
        iEnd   = (*keywordsNamesMapPTR).end (),
        i      = iBegin;
      for ( ; ; ) {
        if ((*i).second == keywordName) {
          result = (*i).first;
          break;
        }
        if (++i == iEnd) break;
      } // for
    }
  }

  return result;
}

//______________________________________________________________________________
void initializeMSDLKeywords ()
{
  // protect library against multiple initializations
  static bool pThisMethodHasBeenRun = false;

  if (! pThisMethodHasBeenRun) {
#ifdef TRACING_IS_ENABLED
    if (gGlobalTraceOahGroup->getTraceOah () && ! gGlobalGeneralOahGroup->getQuiet ()) {
      gLogStream <<
        "Initializing MSR basic types handling" <<
        endl;
  }
#endif

    // keywords languages handling
    // ------------------------------------------------------

    initializeMsdrKeywordsLanguageKindsMap ();

    // keywords handling
    // ------------------------------------------------------

    initializeEnglishKeywordsNamesMap ();
    initializeFrenchKeywordsNamesMap ();
    initializeItalianKeywordsNamesMap ();
    initializeGermanKeywordsNamesMap ();
    initializeSpanishKeywordsNamesMap ();
    initializeDutchKeywordsNamesMap ();

    pThisMethodHasBeenRun = true;
  }
}


}
