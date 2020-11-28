/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef __msr2bsrInterface__
#define __msr2bsrInterface__


#include "bsr.h"
#include "exports.h"

#include "generalOah.h"
#include "msrOah.h"
#include "bsrOah.h"


namespace MusicXML2
{
//_______________________________________________________________________________
S_bsrScore convertMsrScoreToBsrScore (
  S_msrScore mScore,
  S_msrOahGroup   msrOpts,
  S_bsrOahGroup   bsrOpts,
  string     passNumber);

} // namespace MusicXML2


#endif
