/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef __lpsr2LilypondInterfact__
#define __lpsr2LilypondInterfact__


#include "lpsr.h"
#include "exports.h"


namespace MusicXML2
{
//______________________________________________________________________________
EXP void generateLilypondCodeFromLpsrScore (
  const S_lpsrScore lpScore,
  S_msrOah          msrOpts,
  S_lpsrOah         lpsrOpts,
  ostream&  logOstream,
  ostream&  lilypondCodeOstream,
  string            passNumber);

//_______________________________________________________________________________
void convertLpsrScoreToLilypondCode (
  string      outputFileName,
  S_lpsrScore lpScore,
  string      passNumber);


} // namespace MusicXML2


#endif
