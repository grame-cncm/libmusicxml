/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef __lpsr2lilypondInterfact__
#define __lpsr2lilypondInterfact__


#include "exports.h"

#include "lpsr.h"


namespace MusicXML2
{

//______________________________________________________________________________
EXP void convertLpsrScoreToLilypondCode (
  const S_lpsrScore theLpsrScore,
  S_msrOahGroup     msrOpts,
  S_lpsrOahGroup    lpsrOpts,
  string            passNumber,
  string            passDescription,
  ostream&          lilypondCodeStream);

EXP void convertLpsrScoreToLilypondCodeWithHandler (
  const S_lpsrScore theLpsrScore,
  S_msrOahGroup     msrOpts,
  S_lpsrOahGroup    lpsrOpts,
  string            passNumber,
  string            passDescription,
  S_oahHandler      handler,
  ostream&          out,
  ostream&          err);


} // namespace MusicXML2


#endif
