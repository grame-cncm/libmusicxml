/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef __msdrKeywords__
#define __msdrKeywords__

#include <string>
#include <map>

#include "smartpointer.h"


using namespace std;

namespace MusicXML2
{

// the MSDL keywords languages
//______________________________________________________________________________
enum msdrKeywordsLanguageKind {
  k_NoKeyword,

  kKeywordsEnglish, // MSDL default
  kKeywordsFrench,
  kKeywordsItalian,
  kKeywordsGerman,
  kKeywordsSpanish,
  kKeywordsNederlands
};

string msdrKeywordsLanguageKindAsString (
  msdrKeywordsLanguageKind languageKind);

msdrKeywordsLanguageKind msdrKeywordsLanguageKindFromString (string theString);

extern map<string, msdrKeywordsLanguageKind>
  gGlobalMsdrKeywordsLanguageKindsMap;

extern map<string, msdrKeywordsLanguageKind>
  gGlobalKeywordsLanguagesKindsMap;

void initializeMsdrKeywordsLanguageKinds ();

string existingMsdrKeywordsLanguageKinds (unsigned int namesListMaxLength);

void initializeMsdrKeywordsLanguageKindsMap ();

// the MSDL keywords
//______________________________________________________________________________
enum msdrKeywordKind {
  k_NoMsdrKeywordKind,

	kMsdrKeywordPitches,

	kMsdrKeywordScore,
	kMsdrKeywordPartGroup,
	kMsdrKeywordGroup,
	kMsdrKeywordStaff,
  kMsdrKeywordVoice,
  kMsdrKeywordFragment,

  kMsdrKeywordAnacrusis,

  kMsdrKeywordClef,
  kMsdrKeywordTreble,
  kMsdrKeywordAlto,
  kMsdrKeywordTenor,
  kMsdrKeywordBaryton,
  kMsdrKeywordBass,

  kMsdrKeywordKey,

  kMsdrKeywordTime,

	kMsdrKeywordDoubleBar, // superfluous since there is '||' ??? JMI
	kMsdrKeywordFinalBar
};

string msdrKeywordKindAsString (msdrKeywordKind keywordKind);

msdrKeywordKind msdrKeywordKindFromString (
  msdrKeywordsLanguageKind languageKind,
  string                   keywordName);

extern map<msdrKeywordKind, string> gGlobalEnglishKeywordsNamesMap;
extern map<msdrKeywordKind, string> gGlobalFrenchKeywordsNamesMap;
extern map<msdrKeywordKind, string> gGlobalItalianKeywordsNamesMap;
extern map<msdrKeywordKind, string> gGlobalGermanKeywordsNamesMap;
extern map<msdrKeywordKind, string> gGlobalSpanishKeywordsNamesMap;
extern map<msdrKeywordKind, string> gGlobalDutchKeywordsNamesMap;

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
