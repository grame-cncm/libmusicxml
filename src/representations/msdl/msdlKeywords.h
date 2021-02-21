/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef __msdlKeywords__
#define __msdlKeywords__

#include <string>
#include <map>

#include "smartpointer.h"


using namespace std;

namespace MusicXML2
{

// the MSDL keywords languages
//______________________________________________________________________________
enum msdlKeywordsLanguageKind {
  k_NoKeywordLanguage,

  kKeywordsEnglish, // MSDL default
  kKeywordsFrench,
  kKeywordsItalian,
  kKeywordsGerman,
  kKeywordsSpanish,
  kKeywordsNederlands
};

string msdlKeywordsLanguageKindAsString (
  msdlKeywordsLanguageKind languageKind);

msdlKeywordsLanguageKind msdlKeywordsLanguageKindFromString (
  string theString);

extern map<string, msdlKeywordsLanguageKind>
  gGlobalMsdlKeywordsLanguageKindsMap;

void initializeMsdlKeywordsLanguageKinds ();

string existingMsdlKeywordsLanguageKinds (unsigned int namesListMaxLength);

void initializeMsdlKeywordsLanguageKindsMap ();

// the MSDL keywords
//______________________________________________________________________________
enum msdlKeywordKind {
  k_NoMsdlKeywordKind,

  kMsdlKeywordTitle,
  kMsdlKeywordComposer,
  kMsdlKeywordOpus,

	kMsdlKeywordPitches,

  kMsdlKeywordAnacrusis,

	kMsdlKeywordBook,
	kMsdlKeywordScore,
	kMsdlKeywordPartGroup,
	kMsdlKeywordPart,
	kMsdlKeywordStaff,
  kMsdlKeywordVoice,
  kMsdlKeywordFragment,

  kMsdlKeywordClef,
  kMsdlKeywordTreble,
  kMsdlKeywordSoprano,
  kMsdlKeywordAlto,
  kMsdlKeywordTenor,
  kMsdlKeywordBaryton,
  kMsdlKeywordBass,

  kMsdlKeywordKey,

  kMsdlKeywordTime
};

string msdlKeywordKindAsString (
  msdlKeywordKind keywordKind);

string msdlKeywordKindAsMsdlString (
  msdlKeywordsLanguageKind languageKind,
  msdlKeywordKind          keywordKind);

msdlKeywordKind msdlKeywordKindFromString (
  msdlKeywordsLanguageKind languageKind,
  string                   keywordName);

extern map<msdlKeywordKind, string> gGlobalEnglishKeywordsNamesMap;
extern map<msdlKeywordKind, string> gGlobalFrenchKeywordsNamesMap;
extern map<msdlKeywordKind, string> gGlobalItalianKeywordsNamesMap;
extern map<msdlKeywordKind, string> gGlobalGermanKeywordsNamesMap;
extern map<msdlKeywordKind, string> gGlobalSpanishKeywordsNamesMap;
extern map<msdlKeywordKind, string> gGlobalDutchKeywordsNamesMap;

void initializeEnglishKeywordsNamesMap ();
void initializeFrenchKeywordsNamesMap ();
void initializeItalianKeywordsNamesMap ();
void initializeGermanKeywordsNamesMap ();
void initializeSpanishKeywordsNamesMap ();
void initializeDutchKeywordsNamesMap ();

string existingKeywordsLanguageKinds (unsigned int namesListMaxLength);

// initialization
//______________________________________________________________________________
void initializeMSDLKeywords ();


}


#endif
