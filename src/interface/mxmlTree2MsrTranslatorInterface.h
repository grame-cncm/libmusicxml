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

//_______________________________________________________________________________
void populateMsrSkeletonFromMxmlTree (
  const Sxmlelement mxmlTree);

//______________________________________________________________________________
S_msrScore populateMsrSkeletonFromElementsTree (
  S_msrOptions&    msrOpts,
  Sxmlelement      mxmlTree,
  S_msrScore       scoreSkeleton,
  indentedOstream& logIOstream);

//______________________________________________________________________________
void displayMSRPopulatedScore (
  S_msrOptions&    msrOpts,
  S_msrScore       mScore,
  indentedOstream& logIOstream);

//______________________________________________________________________________
void displayMSRPopulatedScoreSummary (
  S_msrOptions&    msrOpts,
  S_msrScore       mScore,
  indentedOstream& logIOstream);


/*! @} */


} // namespace MusicXML2


#endif
