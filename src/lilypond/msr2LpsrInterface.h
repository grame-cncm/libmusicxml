/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef __msr2LpsrInterface__
#define __msr2LpsrInterface__


#include "generalOah.h"
#include "msrOah.h"
#include "lpsrOah.h"

#include "lpsr.h"
#include "exports.h"


namespace MusicXML2
{
//______________________________________________________________________________
EXP S_lpsrScore buildLpsrScoreFromMsrScore (
  const S_msrScore mScore,
  S_msrOah         msrOpts,
  S_lpsrOah        lpsrOpts,
  ostream&         logOstream);

//_______________________________________________________________________________
EXP void displayLpsrScore (
  const S_lpsrScore lpScore,
  S_msrOah          msrOpts,
  S_lpsrOah         lpsrOpts,
  ostream&          logOstream);

EXP void displayLpsrScoreShort (
  const S_lpsrScore lpScore,
  S_msrOah          msrOpts,
  S_lpsrOah         lpsrOpts,
  ostream&          logOstream);

//_______________________________________________________________________________
void displayLpsrScore_OptionalPass (
  S_lpsrScore lpScore,
  S_msrOah    msrOpts,
  S_lpsrOah   lpsrOpts);

void displayLpsrScoreShort_OptionalPass (
  S_lpsrScore lpScore,
  S_msrOah    msrOpts,
  S_lpsrOah   lpsrOpts);

//_______________________________________________________________________________
S_lpsrScore convertMsrScoreToLpsrScore (
  S_msrScore mScore,
  string     passNumber);


} // namespace MusicXML2


#endif
