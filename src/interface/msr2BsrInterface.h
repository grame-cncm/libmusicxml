/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef __msr2BsrInterface__
#define __msr2BsrInterface__


#include "bsr.h"
#include "exports.h"

#include "generalOah.h"
#include "msrOah.h"
#include "bsrOah.h"


namespace MusicXML2
{

/*!
\addtogroup Converting MSR to BSR format

The library includes a high level API to convert
  from the MusicXML format to the MSR
  (Music Score Representation) format.
@{
*/

//______________________________________________________________________________
EXP S_bsrScore buildBsrScoreFromMsrScore (
  const S_msrScore mScore,
  S_msrOah     msrOpts,
  S_bsrOah     bsrOpts,
  indentedOstream& logOstream);

//_______________________________________________________________________________
EXP void displayFirstBsrScore (
  const S_bsrScore bScore,
  S_msrOah     msrOpts,
  S_bsrOah     bsrOpts,
  indentedOstream& logOstream);

/*! @} */


} // namespace MusicXML2


#endif
