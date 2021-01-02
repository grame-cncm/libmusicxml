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
enum generatedCodeKind {
  k_NoGeneratedCode,
  kGuido, kLilyPond, kBrailleMusic, kMusicXML
};

string generatedCodeKindAsString (generatedCodeKind kind);

extern map<string, generatedCodeKind>
  gGlobalGeneratedCodeKindsMap;

string existingGeneratedCodeKinds (unsigned int namesListMaxLength);

void initializeGeneratedCodeKindsMap ();


}


#endif
