/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef __msr2mxmlTreeInterface__
#define __msr2mxmlTreeInterface__

#include "elements.h"
#include "factory.h"
#include "xml.h"
#include "xmlfile.h"

#include "exports.h"

#include "msr.h"

#include "generalOah.h"
#include "msrOah.h"


namespace MusicXML2
{
//______________________________________________________________________________
EXP Sxmlelement convertMsrScoreTodMxmltree (
  const S_msrScore           mScore,
  S_msrOahGroup                   msrOpts,
  string                     passNumber,
  timingItem::timingItemKind timingItemKind);


} // namespace MusicXML2


#endif
