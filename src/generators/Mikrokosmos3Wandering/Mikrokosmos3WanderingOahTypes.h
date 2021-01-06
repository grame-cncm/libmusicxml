/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef ___Mikrokosmos3WanderingOahTypes___
#define ___Mikrokosmos3WanderingOahTypes___

#include "oahBasicTypes.h"


namespace MusicXML2
{

//______________________________________________________________________________
enum mkkGenerateCodeKind {
  k_NoGenerateCode,
  kGuido, kLilyPond, kBrailleMusic, kMusicXML
};

EXP extern string mkkGenerateCodeKindAsString (
  mkkGenerateCodeKind generateCodeKind);

EXP extern mkkGenerateCodeKind mkkGenerateCodeKindFromString (
  const string& theString);

extern map<string, mkkGenerateCodeKind>
  gGlobalGenerateCodeKindsMap;

string existingGenerateCodeKinds (unsigned int namesListMaxLength);

void initializeGenerateCodeKindsMap ();


}


#endif
