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

#include "utilities.h"
#include "waeMessagesHandling.h"

#include "msrBasicTypes.h"
#include "msrElements.h"

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
#include "msrDoubleTremolos.h"

#include "msrOrnaments.h"

#include "msrSpanners.h"

#include "msrGraceNotes.h"
#include "msrAfterGraceNotes.h"

#include "msrHarmonies.h"
#include "msrFiguredBasses.h"

#include "msrGlissandos.h"

#include "msrSlides.h"

#include "msrArpeggiatos.h"

#include "msrNotes.h"

#include "msrChords.h"

#include "msrTupletElements.h"
#include "msrTuplets.h"

#include "msrLyrics.h"

#include "msrRepeats.h"
#include "msrRepeatCodas.h"

#include "msrRestMeasures.h"
#include "msrMeasuresRepeats.h"
#include "msrBeatsRepeats.h"
#include "msrSlashes.h"

#include "msrMeasureElements.h"
#include "msrMeasures.h"

#include "msrSegments.h"

#include "msrVoiceElements.h"
#include "msrVoices.h"

#include "msrStaves.h"

#include "msrParts.h"

#include "msrPartGroupElements.h"
#include "msrPartGroups.h"

#include "msrBookElements.h"
#include "msrBooks.h"

#include "msrScores.h"

#include "msrBookElements.h"
#include "msrBooks.h"

#include "msrMidi.h"

#include "msr2namesVisitor.h"
#include "msr2summaryVisitor.h"


namespace MusicXML2
{

//______________________________________________________________________________
void EXP initializeMSR ();


}


#endif
