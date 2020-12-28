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

  It includes the corresponding msr*_MUT_DEP.h files,
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
// PRE-declarations for class mutual dependencies
/*
class msrScore;
typedef SMARTP<msrScore> S_msrScore;

class msrPartGroup;
typedef SMARTP<msrPartGroup> S_msrPartGroup;

class msrPart;
typedef SMARTP<msrPart> S_msrPart;

class msrStaff;
typedef SMARTP<msrStaff> S_msrStaff;

class msrVoice;
typedef SMARTP<msrVoice> S_msrVoice;

class msrSegment;
typedef SMARTP<msrSegment> S_msrSegment;

class msrMeasure;
typedef SMARTP<msrMeasure> S_msrMeasure;

class msrVoiceStaffChange;
typedef SMARTP<msrVoiceStaffChange> S_msrVoiceStaffChange;

class msrNote;
typedef SMARTP<msrNote> S_msrNote;

class msrChord;
typedef SMARTP<msrChord> S_msrChord;

class msrTuplet;
typedef SMARTP<msrTuplet> S_msrTuplet;

class msrGraceNotesGroup;
typedef SMARTP<msrGraceNotesGroup> S_msrGraceNotesGroup;

class msrAfterGraceNotesGroup;
typedef SMARTP<msrAfterGraceNotesGroup> S_msrAfterGraceNotesGroup;

class msrHarmony;
typedef SMARTP<msrHarmony> S_msrHarmony;

class msrFiguredBass;
typedef SMARTP<msrFiguredBass> S_msrFiguredBass;

class msrRestMeasures;
typedef SMARTP<msrRestMeasures> S_msrRestMeasures;

class msrMeasuresRepeat;
typedef SMARTP<msrMeasuresRepeat> S_msrMeasuresRepeat;

class msrBeatsRepeat;
typedef SMARTP<msrBeatsRepeat> S_msrBeatsRepeat;

class msrRepeat;
typedef SMARTP<msrRepeat> S_msrRepeat;

class msrStanza;
typedef SMARTP<msrStanza> S_msrStanza;
*/

//______________________________________________________________________________

#include "msrOrnaments_MUT_DEP.h"

#include "msrDoubleTremolos_MUT_DEP.h"

#include "msrSpanners_MUT_DEP.h"

#include "msrMeasures_MUT_DEP.h"

#include "msrSegments_MUT_DEP.h"

#include "msrGraceNotes_MUT_DEP.h"

#include "msrAfterGraceNotes_MUT_DEP.h"

#include "msrSyllables_MUT_DEP.h"

#include "msrHarmonies_MUT_DEP.h"

#include "msrFiguredBasses_MUT_DEP.h"

#include "msrGlissandos_MUT_DEP.h"

#include "msrSlides_MUT_DEP.h"

#include "msrNotes_MUT_DEP.h"

#include "msrChords_MUT_DEP.h"

#include "msrTuplets_MUT_DEP.h"

#include "msrStanzas_MUT_DEP.h"

#include "msrRepeats_MUT_DEP.h"

#include "msrMeasuresRepeats_MUT_DEP.h"

#include "msrRestMeasures_MUT_DEP.h"

#include "msrBeatsRepeats_MUT_DEP.h"

#include "msrRepeatCodas_MUT_DEP.h"

#include "msrVoices_MUT_DEP.h"

#include "msrStaves_MUT_DEP.h"

#include "msrParts_MUT_DEP.h"


#endif
