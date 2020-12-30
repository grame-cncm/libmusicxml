/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

/*
  This file declares classes that cannot be placed in 'individual' headers
  due to mutual dependencies, which C++ requires to be in one and the same file.

  It includes the corresponding msr*.h files,
  which are meant to be included only in this one.
*/

#ifndef ___msrMutualDependencies___
#define ___msrMutualDependencies___

#include "typedefs.h"
#include "tree_browser.h"
#include "exports.h"

#include "utilities.h"
#include "messagesHandling.h"

#include "msrElements.h"

#include "msrTupletElements.h"

#include "msrVoiceElements.h"

#include "msrIdentification.h"

#include "msrMusicXMLSpecifics.h"
#include "msrScaling.h"

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
#include "msrSegnosAndCodas.h"

#include "msrTies.h"
#include "msrSlurs.h"
#include "msrLigatures.h"

#include "msrSingleTremolos.h"

#include "msrSlashes.h"

#include "msrPartGroups.h"

#include "msrScores.h"

#include "msrMidi.h"

#include "generalOah.h"

//______________________________________________________________________________

#include "msrOrnaments.h"

#include "msrDoubleTremolos.h"

#include "msrSpanners.h"

#include "msrMeasures.h"

#include "msrSegments.h"

#include "msrGraceNotes.h"

#include "msrAfterGraceNotes.h"

#include "msrHarmonies.h"

#include "msrFiguredBasses.h"

#include "msrGlissandos.h"

#include "msrSlides.h"

#include "msrNotes.h"

#include "msrChords.h"

#include "msrTuplets.h"

#include "msrLyrics.h"

#include "msrRepeats.h"

#include "msrMeasuresRepeats.h"

#include "msrRestMeasures.h"

#include "msrBeatsRepeats.h"

#include "msrRepeatCodas.h"

#include "msrVoices.h"

#include "msrStaves.h"

#include "msrParts.h"


#endif
