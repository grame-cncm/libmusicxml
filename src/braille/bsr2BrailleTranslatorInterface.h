/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef __bsr2BrailleTranslatorInterface__
#define __bsr2BrailleTranslatorInterface__


#include "bsr.h"
#include "exports.h"

#include "msrOah.h"
#include "bsrOah.h"


namespace MusicXML2
{

//______________________________________________________________________________
EXP void generateBrailleCodeFromBsrScore (
  const S_bsrScore bScore,
  S_bsrOah         bsrOpts,
  ostream&         logOstream,
  ostream&         brailleCodeOutputStream,
  string           passNumber);

//_______________________________________________________________________________
void convertBsrScoreToBrailleText (
  string     outputFileName,
  S_bsrScore bScore,
  string     passNumber);

//_______________________________________________________________________________
S_bsrScore convertBsrScoreToFinalizedBsrScore (
  S_bsrScore bScore,
  string     passNumber);


} // namespace MusicXML2


#endif
