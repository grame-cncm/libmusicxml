/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#include "msrBasicTypes.h"

#include "msdlTokens.h"

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
  msdlKeywordsLanguageKind result = msdlKeywordsLanguageKind::kKeywordsEnglish; // MSDL default

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
    gGlobalMsdlKeywordsLanguageKindsMap ["english"]   =
      msdlKeywordsLanguageKind::kKeywordsEnglish;

    gGlobalMsdlKeywordsLanguageKindsMap ["francais"]  =
      msdlKeywordsLanguageKind::kKeywordsFrench;

    gGlobalMsdlKeywordsLanguageKindsMap ["italiano"]  =
      msdlKeywordsLanguageKind::kKeywordsItalian;

    gGlobalMsdlKeywordsLanguageKindsMap ["deutsch"]   =
      msdlKeywordsLanguageKind::kKeywordsGerman;

    gGlobalMsdlKeywordsLanguageKindsMap ["espanol"]   =
      msdlKeywordsLanguageKind::kKeywordsSpanish;

    gGlobalMsdlKeywordsLanguageKindsMap ["dutch"] =
      msdlKeywordsLanguageKind::kKeywordsNederlands;

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
    case msdlKeywordsLanguageKind::k_NoKeywordsLanguage: // default value
      result = "*noKeywordsLanguage*";
      break;

    case msdlKeywordsLanguageKind::kKeywordsEnglish: // default value
      result = "english";
      break;
    case msdlKeywordsLanguageKind::kKeywordsFrench:
      result = "francais";
      break;
    case msdlKeywordsLanguageKind::kKeywordsItalian:
      result = "italiano";
      break;
    case msdlKeywordsLanguageKind::kKeywordsGerman:
      result = "deutsch";
      break;
    case msdlKeywordsLanguageKind::kKeywordsSpanish:
      result = "espanol";
      break;
    case msdlKeywordsLanguageKind::kKeywordsNederlands:
      result = "dutch";
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

      ++count;

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
  gGlobalEnglishKeywordsNamesMap [msdlKeywordKind::kKeywordTitle]     = "title";
  gGlobalEnglishKeywordsNamesMap [msdlKeywordKind::kKeywordComposer]  = "composer";
  gGlobalEnglishKeywordsNamesMap [msdlKeywordKind::kKeywordOpus]      = "opus";

  gGlobalEnglishKeywordsNamesMap [msdlKeywordKind::kKeywordPitches]   = "pitches";

  gGlobalEnglishKeywordsNamesMap [msdlKeywordKind::kKeywordAnacrusis] = "anacrusis";

  gGlobalEnglishKeywordsNamesMap [msdlKeywordKind::kKeywordBook]      = "book";
  gGlobalEnglishKeywordsNamesMap [msdlKeywordKind::kKeywordScore]     = "score";
  gGlobalEnglishKeywordsNamesMap [msdlKeywordKind::kKeywordPartGroup] = "partgroup";
  gGlobalEnglishKeywordsNamesMap [msdlKeywordKind::kKeywordPart]      = "part";
  gGlobalEnglishKeywordsNamesMap [msdlKeywordKind::kKeywordMusic]     = "music";
  gGlobalEnglishKeywordsNamesMap [msdlKeywordKind::kKeywordFragment]  = "fragment";

  gGlobalEnglishKeywordsNamesMap [msdlKeywordKind::kKeywordClef]      = "clef";
  gGlobalEnglishKeywordsNamesMap [msdlKeywordKind::kKeywordTreble]    = "treble";
  gGlobalEnglishKeywordsNamesMap [msdlKeywordKind::kKeywordSoprano]   = "soprano";
  gGlobalEnglishKeywordsNamesMap [msdlKeywordKind::kKeywordAlto]      = "alto";
  gGlobalEnglishKeywordsNamesMap [msdlKeywordKind::kKeywordTenor]     = "tenor";
  gGlobalEnglishKeywordsNamesMap [msdlKeywordKind::kKeywordBaryton]   = "baryton";
  gGlobalEnglishKeywordsNamesMap [msdlKeywordKind::kKeywordBass]      = "bass";

  gGlobalEnglishKeywordsNamesMap [msdlKeywordKind::kKeywordKey]       = "key";

  gGlobalEnglishKeywordsNamesMap [msdlKeywordKind::kKeywordTime]      = "time";
}

void initializeFrenchKeywordsNamesMap ()
{
  // french
  gGlobalFrenchKeywordsNamesMap [msdlKeywordKind::kKeywordTitle]     = "titre";
  gGlobalFrenchKeywordsNamesMap [msdlKeywordKind::kKeywordComposer]  = "compositeur";
  gGlobalFrenchKeywordsNamesMap [msdlKeywordKind::kKeywordOpus]      = "opus";

  gGlobalFrenchKeywordsNamesMap [msdlKeywordKind::kKeywordPitches]   = "hauteurs";

  gGlobalFrenchKeywordsNamesMap [msdlKeywordKind::kKeywordAnacrusis] = "anacrouse";

  gGlobalFrenchKeywordsNamesMap [msdlKeywordKind::kKeywordBook]      = "livre";
  gGlobalFrenchKeywordsNamesMap [msdlKeywordKind::kKeywordScore]     = "partition";
  gGlobalFrenchKeywordsNamesMap [msdlKeywordKind::kKeywordPartGroup] = "groupeDeParties";
  gGlobalFrenchKeywordsNamesMap [msdlKeywordKind::kKeywordPart]      = "partie";
  gGlobalFrenchKeywordsNamesMap [msdlKeywordKind::kKeywordMusic]     = "musique";
  gGlobalFrenchKeywordsNamesMap [msdlKeywordKind::kKeywordFragment]  = "fragment";

  gGlobalFrenchKeywordsNamesMap [msdlKeywordKind::kKeywordClef]      = "clé";
  gGlobalFrenchKeywordsNamesMap [msdlKeywordKind::kKeywordTreble]    = "sol";
  gGlobalFrenchKeywordsNamesMap [msdlKeywordKind::kKeywordSoprano]   = "soprano";
  gGlobalFrenchKeywordsNamesMap [msdlKeywordKind::kKeywordAlto]      = "alto";
  gGlobalFrenchKeywordsNamesMap [msdlKeywordKind::kKeywordTenor]     = "ténor";
  gGlobalFrenchKeywordsNamesMap [msdlKeywordKind::kKeywordBaryton]   = "baryton";
  gGlobalFrenchKeywordsNamesMap [msdlKeywordKind::kKeywordBass]      = "fa";

  gGlobalFrenchKeywordsNamesMap [msdlKeywordKind::kKeywordKey]       = "armure";

  gGlobalFrenchKeywordsNamesMap [msdlKeywordKind::kKeywordTime]      = "métrique";
}

void initializeItalianKeywordsNamesMap ()
{
  // italian
  gGlobalItalianKeywordsNamesMap [msdlKeywordKind::kKeywordTitle]     = "title";
  gGlobalItalianKeywordsNamesMap [msdlKeywordKind::kKeywordComposer]  = "composer";
  gGlobalItalianKeywordsNamesMap [msdlKeywordKind::kKeywordOpus]      = "opus";

  gGlobalItalianKeywordsNamesMap [msdlKeywordKind::kKeywordPitches]   = "pitches";

  gGlobalItalianKeywordsNamesMap [msdlKeywordKind::kKeywordAnacrusis] = "anacrusis";

  gGlobalItalianKeywordsNamesMap [msdlKeywordKind::kKeywordBook]      = "libro";
  gGlobalItalianKeywordsNamesMap [msdlKeywordKind::kKeywordScore]     = "partitura";
  gGlobalItalianKeywordsNamesMap [msdlKeywordKind::kKeywordPartGroup] = "partgroup";
  gGlobalItalianKeywordsNamesMap [msdlKeywordKind::kKeywordPart]      = "part";
  gGlobalItalianKeywordsNamesMap [msdlKeywordKind::kKeywordMusic]     = "music";
  gGlobalItalianKeywordsNamesMap [msdlKeywordKind::kKeywordFragment]  = "fragment";

  gGlobalItalianKeywordsNamesMap [msdlKeywordKind::kKeywordClef]      = "clef";
  gGlobalItalianKeywordsNamesMap [msdlKeywordKind::kKeywordTreble]    = "treble";
  gGlobalItalianKeywordsNamesMap [msdlKeywordKind::kKeywordSoprano]   = "soprano";
  gGlobalItalianKeywordsNamesMap [msdlKeywordKind::kKeywordAlto]      = "alto";
  gGlobalItalianKeywordsNamesMap [msdlKeywordKind::kKeywordTenor]     = "tenor";
  gGlobalItalianKeywordsNamesMap [msdlKeywordKind::kKeywordBaryton]   = "baryton";
  gGlobalItalianKeywordsNamesMap [msdlKeywordKind::kKeywordBass]      = "bass";

  gGlobalItalianKeywordsNamesMap [msdlKeywordKind::kKeywordKey]       = "key";

  gGlobalItalianKeywordsNamesMap [msdlKeywordKind::kKeywordTime]      = "time";
}

void initializeGermanKeywordsNamesMap ()
{
  // german
  gGlobalGermanKeywordsNamesMap [msdlKeywordKind::kKeywordTitle]     = "title";
  gGlobalGermanKeywordsNamesMap [msdlKeywordKind::kKeywordComposer]  = "composer";
  gGlobalGermanKeywordsNamesMap [msdlKeywordKind::kKeywordOpus]      = "opus";

  gGlobalGermanKeywordsNamesMap [msdlKeywordKind::kKeywordPitches]   = "pitches";

  gGlobalGermanKeywordsNamesMap [msdlKeywordKind::kKeywordAnacrusis] = "anacrusis";

  gGlobalGermanKeywordsNamesMap [msdlKeywordKind::kKeywordBook]      = "Buch";
  gGlobalGermanKeywordsNamesMap [msdlKeywordKind::kKeywordScore]     = "score";
  gGlobalGermanKeywordsNamesMap [msdlKeywordKind::kKeywordPartGroup] = "partgroup";
  gGlobalGermanKeywordsNamesMap [msdlKeywordKind::kKeywordPart]      = "part";
  gGlobalGermanKeywordsNamesMap [msdlKeywordKind::kKeywordMusic]     = "music";
  gGlobalGermanKeywordsNamesMap [msdlKeywordKind::kKeywordFragment]  = "fragment";

  gGlobalGermanKeywordsNamesMap [msdlKeywordKind::kKeywordClef]      = "clef";
  gGlobalGermanKeywordsNamesMap [msdlKeywordKind::kKeywordTreble]    = "treble";
  gGlobalGermanKeywordsNamesMap [msdlKeywordKind::kKeywordSoprano]   = "soprano";
  gGlobalGermanKeywordsNamesMap [msdlKeywordKind::kKeywordAlto]      = "alto";
  gGlobalGermanKeywordsNamesMap [msdlKeywordKind::kKeywordTenor]     = "tenor";
  gGlobalGermanKeywordsNamesMap [msdlKeywordKind::kKeywordBaryton]   = "baryton";
  gGlobalGermanKeywordsNamesMap [msdlKeywordKind::kKeywordBass]      = "bass";

  gGlobalGermanKeywordsNamesMap [msdlKeywordKind::kKeywordKey]       = "key";

  gGlobalGermanKeywordsNamesMap [msdlKeywordKind::kKeywordTime]      = "time";
}

void initializeSpanishKeywordsNamesMap ()
{
  // spanish
  gGlobalSpanishKeywordsNamesMap [msdlKeywordKind::kKeywordTitle]     = "title";
  gGlobalSpanishKeywordsNamesMap [msdlKeywordKind::kKeywordComposer]  = "composer";
  gGlobalSpanishKeywordsNamesMap [msdlKeywordKind::kKeywordOpus]      = "opus";

  gGlobalSpanishKeywordsNamesMap [msdlKeywordKind::kKeywordPitches]   = "pitches";

  gGlobalSpanishKeywordsNamesMap [msdlKeywordKind::kKeywordAnacrusis] = "anacrusis";

  gGlobalSpanishKeywordsNamesMap [msdlKeywordKind::kKeywordBook]      = "book";
  gGlobalSpanishKeywordsNamesMap [msdlKeywordKind::kKeywordScore]     = "score";
  gGlobalSpanishKeywordsNamesMap [msdlKeywordKind::kKeywordPartGroup] = "partgroup";
  gGlobalSpanishKeywordsNamesMap [msdlKeywordKind::kKeywordPart]      = "part";
  gGlobalSpanishKeywordsNamesMap [msdlKeywordKind::kKeywordMusic]     = "music";
  gGlobalSpanishKeywordsNamesMap [msdlKeywordKind::kKeywordFragment]  = "fragment";

  gGlobalSpanishKeywordsNamesMap [msdlKeywordKind::kKeywordClef]      = "clef";
  gGlobalSpanishKeywordsNamesMap [msdlKeywordKind::kKeywordTreble]    = "treble";
  gGlobalSpanishKeywordsNamesMap [msdlKeywordKind::kKeywordSoprano]   = "soprano";
  gGlobalSpanishKeywordsNamesMap [msdlKeywordKind::kKeywordAlto]      = "alto";
  gGlobalSpanishKeywordsNamesMap [msdlKeywordKind::kKeywordTenor]     = "tenor";
  gGlobalSpanishKeywordsNamesMap [msdlKeywordKind::kKeywordBaryton]   = "baryton";
  gGlobalSpanishKeywordsNamesMap [msdlKeywordKind::kKeywordBass]      = "bass";

  gGlobalSpanishKeywordsNamesMap [msdlKeywordKind::kKeywordKey]       = "key";

  gGlobalSpanishKeywordsNamesMap [msdlKeywordKind::kKeywordTime]      = "time";
}

void initializeDutchKeywordsNamesMap ()
{
  // dutch
  gGlobalDutchKeywordsNamesMap [msdlKeywordKind::kKeywordTitle]     = "title";
  gGlobalDutchKeywordsNamesMap [msdlKeywordKind::kKeywordComposer]  = "composer";
  gGlobalDutchKeywordsNamesMap [msdlKeywordKind::kKeywordOpus]      = "opus";

  gGlobalDutchKeywordsNamesMap [msdlKeywordKind::kKeywordPitches]   = "pitches";

  gGlobalDutchKeywordsNamesMap [msdlKeywordKind::kKeywordAnacrusis] = "anacrusis";

  gGlobalDutchKeywordsNamesMap [msdlKeywordKind::kKeywordBook]      = "book";
  gGlobalDutchKeywordsNamesMap [msdlKeywordKind::kKeywordScore]     = "score";
  gGlobalDutchKeywordsNamesMap [msdlKeywordKind::kKeywordPartGroup] = "partgroup";
  gGlobalDutchKeywordsNamesMap [msdlKeywordKind::kKeywordPart]      = "part";
  gGlobalDutchKeywordsNamesMap [msdlKeywordKind::kKeywordMusic]     = "music";
  gGlobalDutchKeywordsNamesMap [msdlKeywordKind::kKeywordFragment]  = "fragment";

  gGlobalDutchKeywordsNamesMap [msdlKeywordKind::kKeywordClef]      = "clef";
  gGlobalDutchKeywordsNamesMap [msdlKeywordKind::kKeywordTreble]    = "treble";
  gGlobalDutchKeywordsNamesMap [msdlKeywordKind::kKeywordSoprano]   = "soprano";
  gGlobalDutchKeywordsNamesMap [msdlKeywordKind::kKeywordAlto]      = "alto";
  gGlobalDutchKeywordsNamesMap [msdlKeywordKind::kKeywordTenor]     = "tenor";
  gGlobalDutchKeywordsNamesMap [msdlKeywordKind::kKeywordBaryton]   = "baryton";
  gGlobalDutchKeywordsNamesMap [msdlKeywordKind::kKeywordBass]      = "bass";

  gGlobalDutchKeywordsNamesMap [msdlKeywordKind::kKeywordKey]       = "key";

  gGlobalDutchKeywordsNamesMap [msdlKeywordKind::kKeywordTime]      = "time";
}


// the MSDL keywords
//______________________________________________________________________________
string msdlKeywordKindAsString (
  msdlKeywordKind keywordKind)
{
  string result;

  switch (keywordKind) {
    case msdlKeywordKind::k_NoKeywordKind:
      result = "*noMsdlKeywordKind*";
      break;

    case msdlKeywordKind::kKeywordTitle:
      result = "msdlKeywordTitle";
      break;
    case msdlKeywordKind::kKeywordComposer:
      result = "msdlKeywordComposer";
      break;
    case msdlKeywordKind::kKeywordOpus:
      result = "msdlKeywordOpus";
      break;

    case msdlKeywordKind::kKeywordPitches:
      result = "msdlKeywordPitches";
      break;

    case msdlKeywordKind::kKeywordBook:
      result = "msdlKeywordBook";
      break;
    case msdlKeywordKind::kKeywordScore:
      result = "msdlKeywordScore";
      break;
    case msdlKeywordKind::kKeywordPartGroup:
      result = "msdlKeywordPartGroup";
      break;
    case msdlKeywordKind::kKeywordPart:
      result = "msdlKeywordPart";
      break;
    case msdlKeywordKind::kKeywordMusic:
      result = "msdlKeywordMusic";
      break;
    case msdlKeywordKind::kKeywordFragment:
      result = "msdlKeywordFragment";
      break;

    case msdlKeywordKind::kKeywordAnacrusis:
      result = "msdlKeywordAnacrusis";
      break;

    case msdlKeywordKind::kKeywordClef:
      result = "msdlKeywordClef";
      break;
    case msdlKeywordKind::kKeywordTreble:
      result = "msdlKeywordTreble";
      break;
    case msdlKeywordKind::kKeywordSoprano:
      result = "msdlKeywordSoprano";
      break;
    case msdlKeywordKind::kKeywordAlto:
      result = "msdlKeywordAlto";
      break;
    case msdlKeywordKind::kKeywordTenor:
      result = "msdlKeywordTenor";
      break;
    case msdlKeywordKind::kKeywordBaryton:
      result = "msdlKeywordBaryton";
      break;
    case msdlKeywordKind::kKeywordBass:
      result = "msdlKeywordBass";
      break;

    case msdlKeywordKind::kKeywordKey:
      result = "msdlKeywordKey";
      break;

    case msdlKeywordKind::kKeywordTime:
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
  if (gGlobalMsdl2msdrOahGroup->getTraceTokensDetails ()) {
    gLogStream <<
      "--> msdlKeywordKindAsMsdlString()" <<
      ", languageKind: " << msdlKeywordsLanguageKindAsString (languageKind) <<
      ", keywordKind: \"" << msdlKeywordKindAsString (keywordKind) << "\"" <<
      endl;
  }
#endif

  string result;

  map<msdlKeywordKind, string> *keywordsNamesMapPTR = nullptr;

  // select the relevant keywords names map
  switch (languageKind) {
    case msdlKeywordsLanguageKind::k_NoKeywordsLanguage:
      // should not occur
      break;

    case msdlKeywordsLanguageKind::kKeywordsEnglish:
      keywordsNamesMapPTR = &gGlobalEnglishKeywordsNamesMap;
      break;
    case msdlKeywordsLanguageKind::kKeywordsFrench:
      keywordsNamesMapPTR = &gGlobalFrenchKeywordsNamesMap;
      break;
    case msdlKeywordsLanguageKind::kKeywordsItalian:
      keywordsNamesMapPTR = &gGlobalItalianKeywordsNamesMap;
      break;
    case msdlKeywordsLanguageKind::kKeywordsGerman:
      keywordsNamesMapPTR = &gGlobalGermanKeywordsNamesMap;
      break;
    case msdlKeywordsLanguageKind::kKeywordsSpanish:
      keywordsNamesMapPTR = &gGlobalSpanishKeywordsNamesMap;
      break;
    case msdlKeywordsLanguageKind::kKeywordsNederlands:
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

#ifdef TRACING_IS_ENABLED
  if (gGlobalMsdl2msdrOahGroup->getTraceTokensDetails ()) {
    gLogStream <<
      "<-- msdlKeywordKindAsMsdlString()" <<
      ", result: \"" << result << "\"" <<
      endl;
  }
#endif

  return result;
}

msdlKeywordKind msdlKeywordKindFromTokenKind (
  msdlTokenKind tokenKind)
{
  msdlKeywordKind result = msdlKeywordKind::k_NoKeywordKind;

  switch (tokenKind) {
    // language-dependent keywords
    // ------------------------------------

    case msdlTokenKind::kTokenTitle:
      result = msdlKeywordKind::kKeywordTitle;
      break;
    case msdlTokenKind::kTokenComposer:
      result = msdlKeywordKind::kKeywordComposer;
      break;
    case msdlTokenKind::kTokenOpus:
      result = msdlKeywordKind::kKeywordOpus;
      break;

    case msdlTokenKind::kTokenPitches:
      result = msdlKeywordKind::kKeywordPitches;
      break;

    case msdlTokenKind::kTokenAnacrusis:
      result = msdlKeywordKind::kKeywordAnacrusis;
      break;

    case msdlTokenKind::kTokenBook:
      result = msdlKeywordKind::kKeywordBook;
      break;
    case msdlTokenKind::kTokenScore:
      result = msdlKeywordKind::kKeywordScore;
      break;
    case msdlTokenKind::kTokenPartGroup:
      result = msdlKeywordKind::kKeywordPartGroup;
      break;
    case msdlTokenKind::kTokenPart:
      result = msdlKeywordKind::kKeywordPart;
      break;
    case msdlTokenKind::kTokenMusic:
      result = msdlKeywordKind::kKeywordMusic;
      break;
    case msdlTokenKind::kTokenFragment:
      result = msdlKeywordKind::kKeywordFragment;
      break;

    case msdlTokenKind::kTokenClef:
      result = msdlKeywordKind::kKeywordClef;
      break;

    case msdlTokenKind::kTokenKey:
      result = msdlKeywordKind::kKeywordKey;
      break;
    case msdlTokenKind::kTokenTreble:
      result = msdlKeywordKind::kKeywordTreble;
      break;
    case msdlTokenKind::kTokenSoprano:
      result = msdlKeywordKind::kKeywordSoprano;
      break;
    case msdlTokenKind::kTokenAlto:
      result = msdlKeywordKind::kKeywordAlto;
      break;
    case msdlTokenKind::kTokenTenor:
      result = msdlKeywordKind::kKeywordTenor;
      break;
    case msdlTokenKind::kTokenBaryton:
      result = msdlKeywordKind::kKeywordBaryton;
      break;
    case msdlTokenKind::kTokenBass:
      result = msdlKeywordKind::kKeywordBass;
      break;

    case msdlTokenKind::kTokenTime:
      result = msdlKeywordKind::kKeywordTime;
      break;

    default:
      ;
  } // switch

  return result;
}

msdlKeywordKind msdlKeywordKindFromString (
  msdlKeywordsLanguageKind languageKind,
  string                   theString)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsdl2msdrOahGroup->getTraceTokensDetails ()) {
    gLogStream <<
      "--> msdlKeywordKindFromString()" <<
      ", languageKind: " << msdlKeywordsLanguageKindAsString (languageKind) <<
      ", theString: \"" << theString << "\"" <<
      endl;
  }
#endif

  msdlKeywordKind result = msdlKeywordKind::k_NoKeywordKind;

  map<msdlKeywordKind, string> *keywordsNamesMapPTR = nullptr;

  // select the relevant keywords names map
  switch (languageKind) {
    case msdlKeywordsLanguageKind::k_NoKeywordsLanguage:
      // should not occur
      break;

    case msdlKeywordsLanguageKind::kKeywordsEnglish:
      keywordsNamesMapPTR = &gGlobalEnglishKeywordsNamesMap;
      break;
    case msdlKeywordsLanguageKind::kKeywordsFrench:
      keywordsNamesMapPTR = &gGlobalFrenchKeywordsNamesMap;
      break;
    case msdlKeywordsLanguageKind::kKeywordsItalian:
      keywordsNamesMapPTR = &gGlobalItalianKeywordsNamesMap;
      break;
    case msdlKeywordsLanguageKind::kKeywordsGerman:
      keywordsNamesMapPTR = &gGlobalGermanKeywordsNamesMap;
      break;
    case msdlKeywordsLanguageKind::kKeywordsSpanish:
      keywordsNamesMapPTR = &gGlobalSpanishKeywordsNamesMap;
      break;
    case msdlKeywordsLanguageKind::kKeywordsNederlands:
      keywordsNamesMapPTR = &gGlobalDutchKeywordsNamesMap;
      break;
  } // switch

  if (keywordsNamesMapPTR) {
    // is theString present in the map?
    if (keywordsNamesMapPTR->size ()) {
      map<msdlKeywordKind, string>::const_iterator
        iBegin = (*keywordsNamesMapPTR).begin (),
        iEnd   = (*keywordsNamesMapPTR).end (),
        i      = iBegin;

      for ( ; ; ) {
        if ((*i).second == theString) {
          result = (*i).first;
          break;
        }
        if (++i == iEnd) break;
      } // for
    }
  }

#ifdef TRACING_IS_ENABLED
  if (gGlobalMsdl2msdrOahGroup->getTraceTokensDetails ()) {
    gLogStream <<
      "<-- msdlKeywordKindFromString()" <<
      ", languageKind: " << msdlKeywordsLanguageKindAsString (languageKind) <<
      ", result: \"" << msdlKeywordKindAsString (result) << "\"" <<
      endl;
  }
#endif

  return result;
}

string existingKeywordsInLanguage (
  msdlKeywordsLanguageKind keywordsLanguageKind,
  unsigned int             namesListMaxLength)
{
  stringstream s;

  map<msdlKeywordKind, string> *keywordsNamesMapPTR = nullptr;

  // select the relevant keywords names map
  switch (keywordsLanguageKind) {
    case msdlKeywordsLanguageKind::k_NoKeywordsLanguage:
      // should not occur
      break;

    case msdlKeywordsLanguageKind::kKeywordsEnglish:
      keywordsNamesMapPTR = &gGlobalEnglishKeywordsNamesMap;
      break;
    case msdlKeywordsLanguageKind::kKeywordsFrench:
      keywordsNamesMapPTR = &gGlobalFrenchKeywordsNamesMap;
      break;
    case msdlKeywordsLanguageKind::kKeywordsItalian:
      keywordsNamesMapPTR = &gGlobalItalianKeywordsNamesMap;
      break;
    case msdlKeywordsLanguageKind::kKeywordsGerman:
      keywordsNamesMapPTR = &gGlobalGermanKeywordsNamesMap;
      break;
    case msdlKeywordsLanguageKind::kKeywordsSpanish:
      keywordsNamesMapPTR = &gGlobalSpanishKeywordsNamesMap;
      break;
    case msdlKeywordsLanguageKind::kKeywordsNederlands:
      keywordsNamesMapPTR = &gGlobalDutchKeywordsNamesMap;
      break;
  } // switch

  if (keywordsNamesMapPTR) {
    unsigned int
      keywordsNameMapSize =
        (*keywordsNamesMapPTR).size ();

    unsigned int
      nextToLast =
        keywordsNameMapSize - 1;

    unsigned int count = 0;
    unsigned int cumulatedLength = 0;

    for (
      map<msdlKeywordKind, string>::const_iterator i =
        (*keywordsNamesMapPTR).begin ();
      i != (*keywordsNamesMapPTR).end ();
      ++i
    ) {
      string theString = (*i).second;

      ++count;

      cumulatedLength += theString.size ();
      if (cumulatedLength >= namesListMaxLength) {
        s << "\n";
        cumulatedLength = 0;
      }

      s << theString;

      if (count == nextToLast) {
        s << " ";
      }
      else if (count != keywordsNameMapSize) {
        s << " ";
      }
    } // for
  }

  return s.str ();
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


    pThisMethodHasBeenRun = true;
  }
}


}
