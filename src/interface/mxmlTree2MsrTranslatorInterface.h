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

#include "exports.h"

#include "utilities.h"

#include "messagesHandling.h"

#include "msrOptions.h"

#include "msr.h"


namespace MusicXML2 
{

//______________________________________________________________________________
EXP void populateMsrSkeletonFromMxmlTree (
  S_msrOptions&    msrOpts,
  Sxmlelement      mxmlTree,
  S_msrScore       scoreSkeleton,
  indentedOstream& logIOstream);

//______________________________________________________________________________
EXP void displayMSRPopulatedScore (
  S_msrOptions&    msrOpts,
  S_msrScore       mScore,
  indentedOstream& logIOstream);

//______________________________________________________________________________
EXP void displayMSRPopulatedScoreSummary (
  S_msrOptions&    msrOpts,
  S_msrScore       mScore,
  indentedOstream& logIOstream);

//______________________________________________________________________________
EXP void displayMSRPopulatedScoreNames (
  S_msrOptions&    msrOpts,
  S_msrScore       mScore,
  indentedOstream& logIOstream);


} // namespace MusicXML2


#endif
