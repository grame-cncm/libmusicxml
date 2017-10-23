/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef __mxml2MsrTranslatorInterface__
#define __mxml2MsrTranslatorInterface__


#ifdef VC6
# pragma warning (disable : 4786)
#endif

#include <iostream>
#include <map>

#include "smartpointer.h"
#include "rational.h"

#include "msrOptions.h"

#include "msr.h"


namespace MusicXML2 
{

//#ifdef __cplusplus
//extern "C" {
//#endif

/*!
\addtogroup Converting MusicXML to MSR format

The library includes a high level API to convert
  from the MusicXML format to the MSR
  (Music Score Representation) format.
@{
*/

//______________________________________________________________________________
/*!
  \brief Converts a MusicXML representation to an MSR.
  \param file a file name 
  \param msrOpts the MSR options to be used
  \param out the output stream
  \return an error code (\c kNoErr when success)
*/
EXP S_msrScore populateMSRSkeletonFromFile (
  const char*      fileName,
  S_msrOptions&    msrOpts,
  S_msrScore       scoreSkeleton,
  indentedOstream& logIOstream);

//______________________________________________________________________________
S_msrScore populateMSRSkeletonFromElementsTree (
  S_msrOptions&    msrOpts,
  Sxmlelement      xmlTree,
  S_msrScore       msrSkeleton,
  indentedOstream& logIOstream);

//______________________________________________________________________________
void displayMSRSkeleton (
  S_msrOptions&    msrOpts,
  S_msrScore       mScore,
  indentedOstream& logIOstream);

//______________________________________________________________________________
void displayMSRSkeletonSummary (
  S_msrOptions&    msrOpts,
  S_msrScore       mScore,
  indentedOstream& logIOstream);

/*! @} */

//#ifdef __cplusplus
//}
//#endif

} // namespace MusicXML2


#endif
