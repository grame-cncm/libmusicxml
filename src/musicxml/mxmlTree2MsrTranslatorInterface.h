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


#include <iostream>

#include "exports.h"

#include "utilities.h"

#include "messagesHandling.h"

#include "msr.h"

#include "msrOah.h"


namespace MusicXML2
{

//_______________________________________________________________________________
void displayMsrScore_OptionalPass (
  S_msrScore mScore,
  S_msrOah   msrOpts);

//______________________________________________________________________________
EXP void populateMsrSkeletonFromMxmlTree (
  Sxmlelement      mxmlTree,
  S_msrScore       scoreSkeleton,
  indentedOstream& logOstream,
  string           passNumber);

//______________________________________________________________________________
EXP void displayMSRPopulatedScore (
  S_msrOah&        msrOpts,
  S_msrScore       mScore,
  indentedOstream& logOstream);

//______________________________________________________________________________
EXP void displayMSRPopulatedScoreSummary (
  S_msrOah&        msrOpts,
  S_msrScore       mScore,
  indentedOstream& logOstream);

//______________________________________________________________________________
EXP void displayMSRPopulatedScoreNames (
  S_msrOah&       msrOpts,
  S_msrScore       mScore,
  indentedOstream& logOstream);


} // namespace MusicXML2


#endif
