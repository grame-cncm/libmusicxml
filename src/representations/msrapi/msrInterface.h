/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef __msrInterface__
#define __msrInterface__

#include "msdr.h"


namespace MusicXML2
{

//_______________________________________________________________________________

// books
// ------------------------------------------------------
S_msrBook createBook ();

// scores
// ------------------------------------------------------
S_msrScore createScore ();

// parts
// ------------------------------------------------------
S_msrPart createPartInScore (S_msrScore theMsrScore);

// staves
// ------------------------------------------------------
S_msrStaff createStaffInPart (
  int staffNumber,
  S_msrPart part);

// voices
// ------------------------------------------------------
S_msrVoice createRegularVoiceInStaff (
  int          inputLineNumber,
  int          voiceNumber,
  S_msrStaff   staff);

// measures
// ------------------------------------------------------
S_msrMeasure createMeasureInVoice (
  int        inputLineNumber,
  string     measureNumber,
             S_msrVoice voice);


} // namespace MusicXML2


#endif
