/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef __msr2msrInterface__
#define __msr2msrInterface__

#include "exports.h"

#include "msr.h"

#include "generalOah.h"
#include "msrOah.h"
#include "lpsrOah.h"


namespace MusicXML2
{
//_______________________________________________________________________________
S_msrScore convertMsrScoreToMsrScore (
  S_msrScore        originalMsrScore,
  S_msrOahGroup     msrOpts,
  S_msr2msrOahGroup msr2msrOpts,
  string            passNumber,
  string            passDescription);

//_______________________________________________________________________________
EXP void displayMsrScore (
  const S_msrScore originalMsrScore,
  S_msrOahGroup    msrOpts,
  S_msr2msrOahGroup   msr2msrOpts);

EXP void displayMsrScoreShort (
  const S_msrScore  originalMsrScore,
  S_msrOahGroup     msrOpts,
  S_msr2msrOahGroup msr2msrOpts);

//_______________________________________________________________________________
EXP void displayMsrScore_OptionalPass (
  const S_msrScore  originalMsrScore,
  S_msrOahGroup     msrOpts,
  S_msr2msrOahGroup msr2msrOpts);

EXP void displayMsrScoreShort_OptionalPass (
  const S_msrScore  originalMsrScore,
  S_msrOahGroup     msrOpts,
  S_msr2msrOahGroup msr2msrOpts);


} // namespace MusicXML2


#endif
