/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef __msr2lpsrInterface__
#define __msr2lpsrInterface__


#include "generalOah.h"
#include "msrOah.h"
#include "lpsrOah.h"

#include "lpsr.h"
#include "exports.h"


namespace MusicXML2
{
//_______________________________________________________________________________
S_lpsrScore convertMsrScoreToLpsrScore (
  S_msrScore     originalMsrScore,
  S_msrOahGroup  msrOpts,
  S_lpsrOahGroup lpsrOpts,
  string         passNumber);

//_______________________________________________________________________________
EXP void displayLpsrScore (
  const S_lpsrScore originalLpsrScore,
  S_msrOahGroup          msrOpts,
  S_lpsrOahGroup         lpsrOpts);

EXP void displayLpsrScoreShort (
  const S_lpsrScore originalLpsrScore,
  S_msrOahGroup          msrOpts,
  S_lpsrOahGroup         lpsrOpts);

//_______________________________________________________________________________
void displayLpsrScore_OptionalPass (
  S_lpsrScore originalLpsrScore,
  S_msrOahGroup    msrOpts,
  S_lpsrOahGroup   lpsrOpts);

void displayLpsrScoreShort_OptionalPass (
  S_lpsrScore originalLpsrScore,
  S_msrOahGroup    msrOpts,
  S_lpsrOahGroup   lpsrOpts);


} // namespace MusicXML2


#endif
