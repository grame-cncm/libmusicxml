/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef __msdl2msdrTokensListTranslatorInterface__
#define __msdl2msdrTokensListTranslatorInterface__

#include "msdlTokens.h"

#include "msrOah.h" // JMI ???

namespace MusicXML2
{
//_______________________________________________________________________________
EXP S_msdrTokensList convertMsdlToMsdlTokensList (
  S_msrOahGroup& msrOpts,
  string         passNumber,
  string         passDescription);

//_______________________________________________________________________________
EXP void displayMsdlTokensList (
  S_msrOahGroup&   msrOpts,
  S_msdrTokensList theMsdlTokensList,
  string           passNumber,
  string           passDescription);


} // namespace MusicXML2


#endif
