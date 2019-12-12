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

//#ifdef __cplusplus
//extern "C" {
//#endif

/*!
\addtogroup Converting MSR to LPSR format

The library includes a high level API to convert
  from the MusicXML format to the MSR
  (Music Score Representation) format.
@{
*/

//______________________________________________________________________________
EXP S_msrVoice buildMsrMasterFromMsrVoice (
  const S_msrVoice mVoice,
  S_msrOah         msrOpts,
  indentedOstream& logOstream);

//_______________________________________________________________________________
EXP void displayMsrMasterVoice (
  const S_msrVoice masterVoice,
  S_msrOah         msrOpts,
  indentedOstream& logOstream);

/*! @} */

//#ifdef __cplusplus
//}
//#endif

} // namespace MusicXML2


#endif
