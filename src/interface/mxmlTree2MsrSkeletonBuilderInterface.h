/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef __mxmlTree2MsrSkeletonBuilderInterface__
#define __mxmlTree2MsrSkeletonBuilderInterface__


#include "msr.h"

#include "msrOah.h"


namespace MusicXML2
{

/*!
\addtogroup Converting MusicXML to MSR format

The library includes a high level API to convert
  from the MusicXML format to the MSR
  (Music Score Representation) format.
@{
*/

//_____________________________________________@__________________________________
EXP S_msrScore buildMsrSkeletonFromElementsTree (
  S_msrOah&    msrOpts,
  Sxmlelement      mxmlTree,
  indentedOstream& logOstream);

//_______________________________________________________________________________
EXP void displayMsrSkeleton (
  S_msrOah&    msrOpts,
  S_msrScore       mScore,
  indentedOstream& logOstream);

//_______________________________________________________________________________
/* JMI
EXP void displayMsrSkeletonSummary (
  S_msrOah&    msrOpts,
  S_msrScore       mScore,
  indentedOstream& logOstream);
*/


/*! @} */


} // namespace MusicXML2


#endif
