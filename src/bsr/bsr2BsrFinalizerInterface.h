/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef __bsr2BsrFinalizerInterface__
#define __bsr2BsrFinalizerInterface__


#include "bsr.h"
#include "exports.h"

#include "msrOah.h"
#include "bsrOah.h"


namespace MusicXML2
{

//_______________________________________________________________________________
void displayFinalizedBsrScore_OptionalPass (
  S_bsrScore bScore,
  S_msrOah   msrOpts,
  S_bsrOah   bsrOpts);

//_______________________________________________________________________________
EXP void displayFinalizedBsrScore (
  const S_bsrScore bScore,
  S_msrOah         msrOpts,
  S_bsrOah         bsrOpts,
  ostream&         logOstream);

//_______________________________________________________________________________
EXP S_bsrScore generateFinalizedBsrScoreFromBsrScore (
  const S_bsrScore bScore,
  S_bsrOah         bsrOpts,
  ostream&         logOstream,
  string           passNumber);

//_______________________________________________________________________________
void displayBsrFirstScore_OptionalPass (
  S_bsrScore bScore,
  S_msrOah   msrOpts,
  S_bsrOah   bsrOpts);

} // namespace MusicXML2


#endif
