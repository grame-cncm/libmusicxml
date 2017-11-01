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


#ifdef VC6
# pragma warning (disable : 4786)
#endif

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
EXP void generateLilypondCodeFromLpsrScore (
  const S_lpsrScore lpScore,
  S_msrOptions      msrOpts,
  S_lpsrOptions     lpsrOpts,
  indentedOstream&  logIOstream,
  indentedOstream&  lilypondCodeIOstream);

/*! @} */

//#ifdef __cplusplus
//}
//#endif

} // namespace MusicXML2


#endif
