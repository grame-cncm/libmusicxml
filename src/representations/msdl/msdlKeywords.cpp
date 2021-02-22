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

#include "msdlKeywords.h"

#include "enableTracingIfDesired.h"
#ifdef TRACING_IS_ENABLED
  #include "traceOah.h"
#endif

#include "generalOah.h"

#include "msdl2msdrOah.h"


using namespace std;

namespace MusicXML2
{

// the MSDL keywords languages
//______________________________________________________________________________

map<string, msdlKeywordsLanguageKind>
  gGlobalMsdlKeywordsLanguageKindsMap;

map<msdlKeywordKind, string> gGlobalEnglishKeywordsNamesMap;
map<msdlKeywordKind, string> gGlobalFrenchKeywordsNamesMap;
map<msdlKeywordKind, string> gGlobalItalianKeywordsNamesMap;
map<msdlKeywordKind, string> gGlobalGermanKeywordsNamesMap;
map<msdlKeywordKind, string> gGlobalSpanishKeywordsNamesMap;
map<msdlKeywordKind, string> gGlobalDutchKeywordsNamesMap;

msdlKeywordsLanguageKind msdlKeywordsLanguageKindFromString (std::string theString)
{
  msdlKeywordsLanguageKind result = kKeywordsLanguageEnglish; // MSDL default

  map<string, msdlKeywordsLanguageKind>::const_iterator
    it =
      gGlobalMsdlKeywordsLanguageKindsMap.find (
        theString);

  if (it == gGlobalMsdlKeywordsLanguageKindsMap.end ()) {
    // no, keywords language kind is unknown in the map
    stringstream s;

    s <<
      "MSDL language kind '" << theString <<
      "' is unknown" <<
      endl <<
      "The " <<
      gGlobalMsdlKeywordsLanguageKindsMap.size () - 1 <<
      " known MSDL language kinds are:" <<
      endl;

    ++gIndenter;

    s <<
      existingMsdlKeywordsLanguageKinds (K_NAMES_LIST_MAX_LENGTH);

    --gIndenter;

// JMI    oahError (s.str ());
  }

  result = (*it).second;

  return result;
}

void initializeMsdlKeywordsLanguageKindsMap ()
{
  // protect library against multiple initializations
  static bool pThisMethodHasBeenRun = false;

  if (! pThisMethodHasBeenRun) {
    gGlobalMsdlKeywordsLanguageKindsMap ["english"]   = kKeywordsLanguageEnglish;
    gGlobalMsdlKeywordsLanguageKindsMap ["francais"]  = kKeywordsLanguageFrench;
    gGlobalMsdlKeywordsLanguageKindsMap ["italiano"]  = kKeywordsLanguageItalian;
    gGlobalMsdlKeywordsLanguageKindsMap ["deutsch"]   = kKeywordsLanguageGerman;
    gGlobalMsdlKeywordsLanguageKindsMap ["espanol"]   = kKeywordsLanguageSpanish;
    gGlobalMsdlKeywordsLanguageKindsMap ["nedelands"] = kKeywordsLanguageNederlands;

    initializeEnglishKeywordsNamesMap ();
    initializeFrenchKeywordsNamesMap ();
    initializeItalianKeywordsNamesMap ();
    initializeGermanKeywordsNamesMap ();
    initializeSpanishKeywordsNamesMap ();
    initializeDutchKeywordsNamesMap ();

    pThisMethodHasBeenRun = true;
  }
}

string msdlKeywordsLanguageKindAsString (
  msdlKeywordsLanguageKind languageKind)
{
  string result;

  switch (languageKind) {
    case k_NoKeywordsLanguage: // default value
      result = "*noKeywordsLanguage*";
      break;

    case kKeywordsLanguageEnglish: // default value
      result = "keywordsEnglish";
      break;
    case kKeywordsLanguageFrench:
      result = "keywordsFrench";
      break;
    case kKeywordsLanguageItalian:
      result = "keywordsItalian";
      break;
    case kKeywordsLanguageGerman:
      result = "keywordsGerman";
      break;
    case kKeywordsLanguageSpanish:
      result = "keywordsSpanish";
      break;
    case kKeywordsLanguageNederlands:
      result = "keywordsNederlands";
      break;
  } // switch

  return result;
}

string existingMsdlKeywordsLanguageKinds (unsigned int namesListMaxLength)
{
  stringstream s;

  unsigned int
    msdlKeywordsLanguageKindsMapSize =
      gGlobalMsdlKeywordsLanguageKindsMap.size ();

  if (msdlKeywordsLanguageKindsMapSize) {
    unsigned int nextToLast =
      msdlKeywordsLanguageKindsMapSize - 1;

    unsigned int count = 0;
    unsigned int cumulatedLength = 0;

    for (
      map<string, msdlKeywordsLanguageKind>::const_iterator i =
        gGlobalMsdlKeywordsLanguageKindsMap.begin ();
      i != gGlobalMsdlKeywordsLanguageKindsMap.end ();
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
      else if (count != msdlKeywordsLanguageKindsMapSize) {
        s << ", ";
      }
    } // for
  }

  return s.str ();
}

// the MSDL keywords
//______________________________________________________________________________
void initializeEnglishKeywordsNamesMap ()
{

  // english
  gGlobalEnglishKeywordsNamesMap [kMsdlKeywordTitle]     = "title";
  gGlobalEnglishKeywordsNamesMap [kMsdlKeywordComposer]  = "composer";
  gGlobalEnglishKeywordsNamesMap [kMsdlKeywordOpus]      = "opus";

  gGlobalEnglishKeywordsNamesMap [kMsdlKeywordPitches]   = "pitches";

  gGlobalEnglishKeywordsNamesMap [kMsdlKeywordAnacrusis] = "anacrusis";

  gGlobalEnglishKeywordsNamesMap [kMsdlKeywordBook]      = "book";
  gGlobalEnglishKeywordsNamesMap [kMsdlKeywordScore]     = "score";
  gGlobalEnglishKeywordsNamesMap [kMsdlKeywordPartGroup] = "partgroup";
  gGlobalEnglishKeywordsNamesMap [kMsdlKeywordPart]      = "part";
  gGlobalEnglishKeywordsNamesMap [kMsdlKeywordStaff]     = "staff";
  gGlobalEnglishKeywordsNamesMap [kMsdlKeywordVoice]     = "voice";
  gGlobalEnglishKeywordsNamesMap [kMsdlKeywordFragment]  = "fragment";

  gGlobalEnglishKeywordsNamesMap [kMsdlKeywordClef]      = "clef";
  gGlobalEnglishKeywordsNamesMap [kMsdlKeywordTreble]    = "treble";
  gGlobalEnglishKeywordsNamesMap [kMsdlKeywordSoprano]   = "soprano";
  gGlobalEnglishKeywordsNamesMap [kMsdlKeywordAlto]      = "alto";
  gGlobalEnglishKeywordsNamesMap [kMsdlKeywordTenor]     = "tenor";
  gGlobalEnglishKeywordsNamesMap [kMsdlKeywordBaryton]   = "baryton";
  gGlobalEnglishKeywordsNamesMap [kMsdlKeywordBass]      = "bass";

  gGlobalEnglishKeywordsNamesMap [kMsdlKeywordKey]       = "key";

  gGlobalEnglishKeywordsNamesMap [kMsdlKeywordTime]      = "time";
}

void initializeFrenchKeywordsNamesMap ()
{
  // french
  gGlobalFrenchKeywordsNamesMap [kMsdlKeywordTitle]     = "titre";
  gGlobalFrenchKeywordsNamesMap [kMsdlKeywordComposer]  = "compositeur";
  gGlobalFrenchKeywordsNamesMap [kMsdlKeywordOpus]      = "opus";

  gGlobalFrenchKeywordsNamesMap [kMsdlKeywordPitches]   = "hauteurDesNotes";

  gGlobalFrenchKeywordsNamesMap [kMsdlKeywordAnacrusis] = "anacrouse";

  gGlobalFrenchKeywordsNamesMap [kMsdlKeywordBook]      = "livre";
  gGlobalFrenchKeywordsNamesMap [kMsdlKeywordScore]     = "partition";
  gGlobalFrenchKeywordsNamesMap [kMsdlKeywordPartGroup] = "groupDeParties";
  gGlobalFrenchKeywordsNamesMap [kMsdlKeywordPart]      = "partie";
  gGlobalFrenchKeywordsNamesMap [kMsdlKeywordStaff]     = "portée";
  gGlobalFrenchKeywordsNamesMap [kMsdlKeywordVoice]     = "voix";
  gGlobalFrenchKeywordsNamesMap [kMsdlKeywordFragment]  = "fragment";

  gGlobalFrenchKeywordsNamesMap [kMsdlKeywordClef]      = "clé";
  gGlobalFrenchKeywordsNamesMap [kMsdlKeywordTreble]    = "sol";
  gGlobalFrenchKeywordsNamesMap [kMsdlKeywordSoprano]   = "soprano";
  gGlobalFrenchKeywordsNamesMap [kMsdlKeywordAlto]      = "alto";
  gGlobalFrenchKeywordsNamesMap [kMsdlKeywordTenor]     = "ténor";
  gGlobalFrenchKeywordsNamesMap [kMsdlKeywordBaryton]   = "baryton";
  gGlobalFrenchKeywordsNamesMap [kMsdlKeywordBass]      = "fa";

  gGlobalFrenchKeywordsNamesMap [kMsdlKeywordKey]       = "armure";

  gGlobalFrenchKeywordsNamesMap [kMsdlKeywordTime]      = "métrique";
}

void initializeItalianKeywordsNamesMap ()
{
  // italian
  gGlobalItalianKeywordsNamesMap [kMsdlKeywordTitle]     = "title";
  gGlobalItalianKeywordsNamesMap [kMsdlKeywordComposer]  = "composer";
  gGlobalItalianKeywordsNamesMap [kMsdlKeywordOpus]      = "opus";

  gGlobalItalianKeywordsNamesMap [kMsdlKeywordPitches]   = "pitches";

  gGlobalItalianKeywordsNamesMap [kMsdlKeywordAnacrusis] = "anacrusis";

  gGlobalItalianKeywordsNamesMap [kMsdlKeywordBook]      = "libro";
  gGlobalItalianKeywordsNamesMap [kMsdlKeywordScore]     = "partitura";
  gGlobalItalianKeywordsNamesMap [kMsdlKeywordPartGroup] = "partgroup";
  gGlobalItalianKeywordsNamesMap [kMsdlKeywordPart]      = "part";
  gGlobalItalianKeywordsNamesMap [kMsdlKeywordStaff]     = "staff";
  gGlobalItalianKeywordsNamesMap [kMsdlKeywordVoice]     = "voice";
  gGlobalItalianKeywordsNamesMap [kMsdlKeywordFragment]  = "fragment";

  gGlobalItalianKeywordsNamesMap [kMsdlKeywordClef]      = "clef";
  gGlobalItalianKeywordsNamesMap [kMsdlKeywordTreble]    = "treble";
  gGlobalItalianKeywordsNamesMap [kMsdlKeywordSoprano]   = "soprano";
  gGlobalItalianKeywordsNamesMap [kMsdlKeywordAlto]      = "alto";
  gGlobalItalianKeywordsNamesMap [kMsdlKeywordTenor]     = "tenor";
  gGlobalItalianKeywordsNamesMap [kMsdlKeywordBaryton]   = "baryton";
  gGlobalItalianKeywordsNamesMap [kMsdlKeywordBass]      = "bass";

  gGlobalItalianKeywordsNamesMap [kMsdlKeywordKey]       = "key";

  gGlobalItalianKeywordsNamesMap [kMsdlKeywordTime]      = "time";
}

void initializeGermanKeywordsNamesMap ()
{
  // german
  gGlobalGermanKeywordsNamesMap [kMsdlKeywordTitle]     = "title";
  gGlobalGermanKeywordsNamesMap [kMsdlKeywordComposer]  = "composer";
  gGlobalGermanKeywordsNamesMap [kMsdlKeywordOpus]      = "opus";

  gGlobalGermanKeywordsNamesMap [kMsdlKeywordPitches]   = "pitches";

  gGlobalGermanKeywordsNamesMap [kMsdlKeywordAnacrusis] = "anacrusis";

  gGlobalGermanKeywordsNamesMap [kMsdlKeywordBook]      = "Buch";
  gGlobalGermanKeywordsNamesMap [kMsdlKeywordScore]     = "score";
  gGlobalGermanKeywordsNamesMap [kMsdlKeywordPartGroup] = "partgroup";
  gGlobalGermanKeywordsNamesMap [kMsdlKeywordPart]      = "part";
  gGlobalGermanKeywordsNamesMap [kMsdlKeywordStaff]     = "staff";
  gGlobalGermanKeywordsNamesMap [kMsdlKeywordVoice]     = "Stimme";
  gGlobalGermanKeywordsNamesMap [kMsdlKeywordFragment]  = "fragment";

  gGlobalGermanKeywordsNamesMap [kMsdlKeywordClef]      = "clef";
  gGlobalGermanKeywordsNamesMap [kMsdlKeywordTreble]    = "treble";
  gGlobalGermanKeywordsNamesMap [kMsdlKeywordSoprano]   = "soprano";
  gGlobalGermanKeywordsNamesMap [kMsdlKeywordAlto]      = "alto";
  gGlobalGermanKeywordsNamesMap [kMsdlKeywordTenor]     = "tenor";
  gGlobalGermanKeywordsNamesMap [kMsdlKeywordBaryton]   = "baryton";
  gGlobalGermanKeywordsNamesMap [kMsdlKeywordBass]      = "bass";

  gGlobalGermanKeywordsNamesMap [kMsdlKeywordKey]       = "key";

  gGlobalGermanKeywordsNamesMap [kMsdlKeywordTime]      = "time";
}

void initializeSpanishKeywordsNamesMap ()
{
  // spanish
  gGlobalSpanishKeywordsNamesMap [kMsdlKeywordTitle]     = "title";
  gGlobalSpanishKeywordsNamesMap [kMsdlKeywordComposer]  = "composer";
  gGlobalSpanishKeywordsNamesMap [kMsdlKeywordOpus]      = "opus";

  gGlobalSpanishKeywordsNamesMap [kMsdlKeywordPitches]   = "pitches";

  gGlobalSpanishKeywordsNamesMap [kMsdlKeywordAnacrusis] = "anacrusis";

  gGlobalSpanishKeywordsNamesMap [kMsdlKeywordBook]      = "book";
  gGlobalSpanishKeywordsNamesMap [kMsdlKeywordScore]     = "score";
  gGlobalSpanishKeywordsNamesMap [kMsdlKeywordPartGroup] = "partgroup";
  gGlobalSpanishKeywordsNamesMap [kMsdlKeywordPart]      = "part";
  gGlobalSpanishKeywordsNamesMap [kMsdlKeywordStaff]     = "staff";
  gGlobalSpanishKeywordsNamesMap [kMsdlKeywordVoice]     = "voice";
  gGlobalSpanishKeywordsNamesMap [kMsdlKeywordFragment]  = "fragment";

  gGlobalSpanishKeywordsNamesMap [kMsdlKeywordClef]      = "clef";
  gGlobalSpanishKeywordsNamesMap [kMsdlKeywordTreble]    = "treble";
  gGlobalSpanishKeywordsNamesMap [kMsdlKeywordSoprano]   = "soprano";
  gGlobalSpanishKeywordsNamesMap [kMsdlKeywordAlto]      = "alto";
  gGlobalSpanishKeywordsNamesMap [kMsdlKeywordTenor]     = "tenor";
  gGlobalSpanishKeywordsNamesMap [kMsdlKeywordBaryton]   = "baryton";
  gGlobalSpanishKeywordsNamesMap [kMsdlKeywordBass]      = "bass";

  gGlobalSpanishKeywordsNamesMap [kMsdlKeywordKey]       = "key";

  gGlobalSpanishKeywordsNamesMap [kMsdlKeywordTime]      = "time";
}

void initializeDutchKeywordsNamesMap ()
{
  // dutch
  gGlobalDutchKeywordsNamesMap [kMsdlKeywordTitle]     = "title";
  gGlobalDutchKeywordsNamesMap [kMsdlKeywordComposer]  = "composer";
  gGlobalDutchKeywordsNamesMap [kMsdlKeywordOpus]      = "opus";

  gGlobalDutchKeywordsNamesMap [kMsdlKeywordPitches]   = "pitches";

  gGlobalDutchKeywordsNamesMap [kMsdlKeywordAnacrusis] = "anacrusis";

  gGlobalDutchKeywordsNamesMap [kMsdlKeywordBook]      = "book";
  gGlobalDutchKeywordsNamesMap [kMsdlKeywordScore]     = "score";
  gGlobalDutchKeywordsNamesMap [kMsdlKeywordPartGroup] = "partgroup";
  gGlobalDutchKeywordsNamesMap [kMsdlKeywordPart]      = "part";
  gGlobalDutchKeywordsNamesMap [kMsdlKeywordStaff]     = "staff";
  gGlobalDutchKeywordsNamesMap [kMsdlKeywordVoice]     = "voice";
  gGlobalDutchKeywordsNamesMap [kMsdlKeywordFragment]  = "fragment";

  gGlobalDutchKeywordsNamesMap [kMsdlKeywordClef]      = "clef";
  gGlobalDutchKeywordsNamesMap [kMsdlKeywordTreble]    = "treble";
  gGlobalDutchKeywordsNamesMap [kMsdlKeywordSoprano]   = "soprano";
  gGlobalDutchKeywordsNamesMap [kMsdlKeywordAlto]      = "alto";
  gGlobalDutchKeywordsNamesMap [kMsdlKeywordTenor]     = "tenor";
  gGlobalDutchKeywordsNamesMap [kMsdlKeywordBaryton]   = "baryton";
  gGlobalDutchKeywordsNamesMap [kMsdlKeywordBass]      = "bass";

  gGlobalDutchKeywordsNamesMap [kMsdlKeywordKey]       = "key";

  gGlobalDutchKeywordsNamesMap [kMsdlKeywordTime]      = "time";
}


// the MSDL keywords
//______________________________________________________________________________
string msdlKeywordKindAsString (
  msdlKeywordKind keywordKind)
{
  string result;

  switch (keywordKind) {
    case k_NoMsdlKeywordKind:
      result = "*noMsdlKeywordKind*";
      break;

    case kMsdlKeywordTitle:
      result = "msdlKeywordTitle";
      break;
    case kMsdlKeywordComposer:
      result = "msdlKeywordComposer";
      break;
    case kMsdlKeywordOpus:
      result = "msdlKeywordOpus";
      break;

    case kMsdlKeywordPitches:
      result = "msdlKeywordPitches";
      break;

    case kMsdlKeywordBook:
      result = "msdlKeywordBook";
      break;
    case kMsdlKeywordScore:
      result = "msdlKeywordScore";
      break;
    case kMsdlKeywordPartGroup:
      result = "msdlKeywordPartGroup";
      break;
    case kMsdlKeywordPart:
      result = "msdlKeywordPart";
      break;
    case kMsdlKeywordStaff:
      result = "msdlKeywordStaff";
      break;
    case kMsdlKeywordVoice:
      result = "msdlKeywordVoice";
      break;
    case kMsdlKeywordFragment:
      result = "msdlKeywordFragment";
      break;

    case kMsdlKeywordAnacrusis:
      result = "msdlKeywordAnacrusis";
      break;

    case kMsdlKeywordClef:
      result = "msdlKeywordClef";
      break;
    case kMsdlKeywordTreble:
      result = "msdlKeywordTreble";
      break;
    case kMsdlKeywordSoprano:
      result = "msdlKeywordSoprano";
      break;
    case kMsdlKeywordAlto:
      result = "msdlKeywordAlto";
      break;
    case kMsdlKeywordTenor:
      result = "msdlKeywordTenor";
      break;
    case kMsdlKeywordBaryton:
      result = "msdlKeywordBaryton";
      break;
    case kMsdlKeywordBass:
      result = "msdlKeywordBass";
      break;

    case kMsdlKeywordKey:
      result = "msdlKeywordKey";
      break;

    case kMsdlKeywordTime:
      result = "msdlKeywordTime";
      break;
  } // switch

  return result;
}

string msdlKeywordKindAsMsdlString (
  msdlKeywordsLanguageKind languageKind,
  msdlKeywordKind          keywordKind)
{

#ifdef TRACING_IS_ENABLED
  if (gGlobalMsdl2msdrOahGroup->getTraceTokens ()) {
    gLogStream <<
      "--> msdlKeywordKindFromString()" <<
      ", languageKind: " << msdlKeywordsLanguageKindAsString (languageKind) <<
      ", keywordKind: \"" << msdlKeywordKindAsString (keywordKind) << "\"" <<
      endl;
  }
#endif

  string result;

  map<msdlKeywordKind, string> *keywordsNamesMapPTR = nullptr;

  // select the relevant keywords names map
  switch (languageKind) {
    case k_NoKeywordsLanguage:
      // should not occur
      break;

    case kKeywordsLanguageEnglish:
      keywordsNamesMapPTR = &gGlobalEnglishKeywordsNamesMap;
      break;
    case kKeywordsLanguageFrench:
      keywordsNamesMapPTR = &gGlobalFrenchKeywordsNamesMap;
      break;
    case kKeywordsLanguageItalian:
      keywordsNamesMapPTR = &gGlobalItalianKeywordsNamesMap;
      break;
    case kKeywordsLanguageGerman:
      keywordsNamesMapPTR = &gGlobalGermanKeywordsNamesMap;
      break;
    case kKeywordsLanguageSpanish:
      keywordsNamesMapPTR = &gGlobalSpanishKeywordsNamesMap;
      break;
    case kKeywordsLanguageNederlands:
      keywordsNamesMapPTR = &gGlobalDutchKeywordsNamesMap;
      break;
  } // switch

  if (keywordsNamesMapPTR) {
    // is keywordKind present in the map?
    if (keywordsNamesMapPTR->size ()) {
      map<msdlKeywordKind, string>::const_iterator
        iBegin = (*keywordsNamesMapPTR).begin (),
        iEnd   = (*keywordsNamesMapPTR).end (),
        i      = iBegin;

      for ( ; ; ) {
        if ((*i).first == keywordKind) {
          result = (*i).second;
          break;
        }
        if (++i == iEnd) break;
      } // for
    }
  }

  return result;
}

msdlKeywordKind msdlKeywordKindFromString (
  msdlKeywordsLanguageKind languageKind,
  string                   keywordName)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsdl2msdrOahGroup->getTraceTokens ()) {
    gLogStream <<
      "--> msdlKeywordKindFromString()" <<
      ", languageKind: " << msdlKeywordsLanguageKindAsString (languageKind) <<
      ", keywordName: \"" << keywordName << "\"" <<
      endl;
  }
#endif

  msdlKeywordKind result = k_NoMsdlKeywordKind;

  map<msdlKeywordKind, string> *keywordsNamesMapPTR = nullptr;

  // select the relevant keywords names map
  switch (languageKind) {
    case k_NoKeywordsLanguage:
      // should not occur
      break;

    case kKeywordsLanguageEnglish:
      keywordsNamesMapPTR = &gGlobalEnglishKeywordsNamesMap;
      break;
    case kKeywordsLanguageFrench:
      keywordsNamesMapPTR = &gGlobalFrenchKeywordsNamesMap;
      break;
    case kKeywordsLanguageItalian:
      keywordsNamesMapPTR = &gGlobalItalianKeywordsNamesMap;
      break;
    case kKeywordsLanguageGerman:
      keywordsNamesMapPTR = &gGlobalGermanKeywordsNamesMap;
      break;
    case kKeywordsLanguageSpanish:
      keywordsNamesMapPTR = &gGlobalSpanishKeywordsNamesMap;
      break;
    case kKeywordsLanguageNederlands:
      keywordsNamesMapPTR = &gGlobalDutchKeywordsNamesMap;
      break;
  } // switch

  if (keywordsNamesMapPTR) {
    // is keywordName present in the map?
    if (keywordsNamesMapPTR->size ()) {
      map<msdlKeywordKind, string>::const_iterator
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
    if (getTraceOah () && ! gGlobalGeneralOahGroup->getQuiet ()) {
      gLogStream <<
        "Initializing MSR basic types handling" <<
        endl;
  }
#endif

    // keywords languages handling
    // ------------------------------------------------------

    initializeMsdlKeywordsLanguageKindsMap ();

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
