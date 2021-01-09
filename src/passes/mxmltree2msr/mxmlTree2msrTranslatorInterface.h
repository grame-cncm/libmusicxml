/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef __mxml2xmlTranslatorInterface__
#define __mxml2xmlTranslatorInterface__


#include <iostream>

#include "exports.h"

#include "utilities.h"

#include "messagesHandling.h"

#include "msr.h"

#include "msrOah.h"


namespace MusicXML2
{

//______________________________________________________________________________
EXP void populateMsrSkeletonFromMxmlTree (
  Sxmlelement mxmlTree,
  S_msrScore  scoreSkeletonToBePopulated,
  string      passNumber,
  string      passDescription);

//_______________________________________________________________________________
void displayMsrScore_OptionalPass (
  S_msrScore theMsrScore);

void displayMsrScoreShort_OptionalPass (
  S_msrScore    theMsrScore,
  S_msrOahGroup msrOpts);

//______________________________________________________________________________
EXP void displayMsrScoreSummary (
  S_msrOahGroup& msrOpts,
  S_msrScore     theMsrScore);

//______________________________________________________________________________
EXP void displayMsrScoreNames (
  S_msrOahGroup& msrOpts,
  S_msrScore     theMsrScore);


} // namespace MusicXML2


#endif
