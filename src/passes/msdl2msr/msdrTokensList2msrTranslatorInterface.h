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

#include "msdrTokens.h"

#include "msrOah.h"


namespace MusicXML2
{

//______________________________________________________________________________
EXP void createMsrFromMsdrTokensList (
  S_msdrTokensList tokensList,
  string           passNumber,
  string           passDescription);

//_______________________________________________________________________________
void displayCreatedMsrScore_OptionalPass (
  S_msdrTokensList tokensList);


} // namespace MusicXML2


#endif
