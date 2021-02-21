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

// pitches languages
//______________________________________________________________________________
enum msdlPitchesLanguageKind {
  kPitchesEnglish, // MSDL default

  kPitchesFrench,
  kPitchesItalian,
  kPitchesGerman,
  kPitchesEspanol
};

string msdlPitchesLanguageKindAsString (
  msdlPitchesLanguageKind languageKind);

extern map<string, msdlPitchesLanguageKind>
  gGlobalMsdlPitchesLanguageKindsMap;

string existingMsdlPitchesLanguageKinds (unsigned int namesListMaxLength);

void initializeMsdlPitchesLanguageKindsMap ();

// initialization
//______________________________________________________________________________
void initializeMSDLBasicTypes ();


} // namespace MusicXML2


#endif
