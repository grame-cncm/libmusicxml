/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef __mxmlTree2msrSkeletonBuilderInterface__
#define __mxmlTree2msrSkeletonBuilderInterface__


#include "msr.h"


namespace MusicXML2
{
//_______________________________________________________________________________
EXP S_msrScore convertMxmlTreeToMsrScoreSkeleton (
  Sxmlelement mxmlTree,
  S_msrOahGroup&   msrOpts,
  string      passNumber);

//_______________________________________________________________________________
EXP void displayMsrSkeleton (
  S_msrOahGroup&  msrOpts,
  S_msrScore theMsrScore,
  string     passNumber);


} // namespace MusicXML2


#endif
