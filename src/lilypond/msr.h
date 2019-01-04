/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef ___msr___
#define ___msr___

#include "typedefs.h"
#include "tree_browser.h"
#include "exports.h"

#include "utilities.h"
#include "messagesHandling.h"

#include "msrElements.h"

#include "msrVarValAssocs.h"

#include "msrIdentification.h"
#include "msrPageGeometry.h"
#include "msrLayout.h"

#include "msrCredits.h"

#include "msrDivisions.h"

#include "msrPartNamesDisplays.h"

#include "msrStavesDetails.h"

#include "msrClefs.h"
#include "msrKeys.h"
#include "msrTimes.h"

#include "msrTempos.h"

#include "msrRehearsals.h"

#include "msrWords.h"

#include "msrTranspositions.h"

#include "msrBreaks.h"

#include "msrStems.h"
#include "msrBeams.h"

#include "msrTechnicals.h"

#include "msrBars.h"

#include "msrDynamics.h"
#include "msrArticulations.h"

#include "msrTablatures.h"
#include "msrInstruments.h"
#include "msrOtherElements.h"

#include "msrTies.h"
#include "msrSlurs.h"
#include "msrLigatures.h"

#include "msrTremolos.h"

#include "msrSlashes.h"

#include "msrScores.h"
#include "msrPartGroups.h"
// JMI #include "msrParts.h"

#include "msrMidi.h"

#include "msrMutuallyDependent.h"

#include "generalOptions.h"


namespace MusicXML2 
{

//______________________________________________________________________________
void EXP initializeMSR ();


}

#endif
