/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef ___msdlBasicTypes___
#define ___msdlBasicTypes___

#include <list>
#include <map>

#include "smartpointer.h"

#include "rational.h"

#include "msrBasicTypes.h"


namespace MusicXML2
{

// user languages
//______________________________________________________________________________
enum class msdlUserLanguageKind {
  kUserLanguageEnglish, // MSDL default
  kUserLanguageFrench,
  kUserLanguageItalian,
  kUserLanguageGerman,
  kUserLanguageSpanish,
  kUserLanguageDutch
};

string msdlUserLanguageKindAsString (
  msdlUserLanguageKind languageKind);

msdlUserLanguageKind msdlUserLanguageKindFromString (
  string theString);

extern map<string, msdlUserLanguageKind>
  gGlobalMsdlUserLanguageKindsMap;

string existingMsdlUserLanguageKinds (unsigned int namesListMaxLength);

void initializeMsdlUserLanguageKindsMap ();

// comments types
//______________________________________________________________________________
enum class msdlCommentsTypeKind {
  kCommentsTypePercent, // MSDL default
  kCommentsTypeStar
};

string msdlCommentsTypeKindAsString (
  msdlCommentsTypeKind languageKind);

msdlCommentsTypeKind msdlCommentsTypeKindFromString (
  string theString);

extern map<string, msdlCommentsTypeKind>
  gGlobalMsdlCommentsTypeKindsMap;

void initializeMsdlCommentsTypeKinds ();

string existingMsdlCommentsTypeKinds (unsigned int namesListMaxLength);

void initializeMsdlCommentsTypeKindsMap ();

// initialization
//______________________________________________________________________________
void initializeMSDLBasicTypes ();


} // namespace MusicXML2


#endif
