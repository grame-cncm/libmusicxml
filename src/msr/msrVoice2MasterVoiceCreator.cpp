/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#include <sstream>
#include <climits>      // INT_MIN, INT_MAX
#include <algorithm>    // for_each

#include "conversions.h"

#include "msrVoice2MasterVoiceCreator.h"

#include "generalOah.h"

#include "setTraceOahIfDesired.h"
#ifdef TRACE_OAH
  #include "traceOah.h"
#endif

#include "mxmlTreeOah.h"
#include "msrOah.h"
#include "lpsr2LilypondOah.h"


using namespace std;

namespace MusicXML2
{

//________________________________________________________________________
msrVoice2MasterVoiceCreator::msrVoice2MasterVoiceCreator (
  ostream&   os,
  S_msrVoice mVoice)
#ifdef TRACE_OAH
    : fLogOutputStream (os)
#endif
{
  // the MSR score we're visiting
  fVisitedMsrVoice = mVoice;

  // create an empty clone of fVisitedMsrVoice
  fMasterVoice =
    fVisitedMsrVoice->
      createVoiceNewbornClone (nullptr); // JMI ??? CRASH

  // voices
  fOnGoingHarmonyVoice     = false;
  fOnGoingFiguredBassVoice = false;

  // harmonies
  fOnGoingHarmony = false;

  // repeats
  // JMI ???

  // notes
  fOnGoingNote = false;

  // chords
  fOnGoingChord = false;
};

msrVoice2MasterVoiceCreator::~msrVoice2MasterVoiceCreator ()
{}

//________________________________________________________________________
void msrVoice2MasterVoiceCreator::buildMsrMasterFromMsrVoice ()
{
  if (fVisitedMsrVoice) {
    // create a msrVoice browser
    msrBrowser<msrVoice> browser (this);

    // browse the visited voice with the browser
    browser.browse (*fVisitedMsrVoice);
  }
}

//________________________________________________________________________
void msrVoice2MasterVoiceCreator::visitStart (S_msrVoice& elt)
{
#ifdef TRACE_OAH
  int inputLineNumber =
    elt->getInputLineNumber ();

  if (gGlobalMsrOah->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> Start visiting msrVoice \"" <<
      elt->asString () << "\"" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  gIndenter++;

  switch (elt->getVoiceKind ()) { // JMI ???

    case msrVoice::kVoiceRegular:
      break;

    case msrVoice::kVoiceHarmony:
      {
      }
      break;

    case msrVoice::kVoiceFiguredBass:
      {
      }
      break;
  } // switch

  fFirstNoteCloneInVoice = nullptr;
}

void msrVoice2MasterVoiceCreator::visitEnd (S_msrVoice& elt)
{
  gIndenter--;

#ifdef TRACE_OAH
  if (gGlobalMsrOah->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> End visiting msrVoice \"" <<
      elt->getVoiceName () << "\"" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  switch (elt->getVoiceKind ()) {
    case msrVoice::kVoiceRegular:
      // JMI
      break;

    case msrVoice::kVoiceHarmony:
      fOnGoingHarmonyVoice = false;
      break;

    case msrVoice::kVoiceFiguredBass:
      fOnGoingFiguredBassVoice = false;
      break;
  } // switch
}

//________________________________________________________________________
void msrVoice2MasterVoiceCreator::visitStart (S_msrSegment& elt)
{
#ifdef TRACE_OAH
  if (gGlobalMsrOah->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> Start visiting msrSegment '" <<
      elt->getSegmentAbsoluteNumber () << "'" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  // create a clone of the segment
  fCurrentSegmentClone =
    elt->createSegmentNewbornClone (
      fMasterVoice);

  // set it as the new voice last segment
  fMasterVoice->
    setVoiceLastSegmentInVoiceClone (
      fCurrentSegmentClone);
}

void msrVoice2MasterVoiceCreator::visitEnd (S_msrSegment& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACE_OAH
  if (gGlobalMsrOah->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> End visiting msrSegment '" <<
      elt->getSegmentAbsoluteNumber () << "'" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  fMasterVoice->
    handleSegmentCloneEndInVoiceClone (
      inputLineNumber,
      fCurrentSegmentClone);

  // forget current segment clone
  fCurrentSegmentClone = nullptr;
}

//________________________________________________________________________
void msrVoice2MasterVoiceCreator::visitStart (S_msrHarmony& elt)
{
#ifdef TRACE_OAH
  if (gGlobalMsrOah->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> Start visiting msrHarmony '" <<
      elt->asString () <<
      ", fOnGoingNote = " << booleanAsString (fOnGoingNote) <<
      ", fOnGoingChord = " << booleanAsString (fOnGoingChord) <<
      ", fOnGoingHarmonyVoice = " << booleanAsString (fOnGoingHarmonyVoice) <<
      ", fOnGoingHarmony = " << booleanAsString (fOnGoingHarmony) <<
      "', line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  // create a harmony new born clone
  fCurrentHarmonyClone =
    elt->
      createHarmonyNewbornClone (
        fMasterVoice);

  if (fOnGoingNote) {
    // register the harmony in the current non-grace note clone
    fCurrentNonGraceNoteClone->
      appendHarmonyToNoteHarmoniesList (
        fCurrentHarmonyClone);

    // don't append the harmony to the part harmony,
    // this has been done in pass2b // JMI ???
  }

  else if (fOnGoingChord) {
    // register the harmony in the current chord clone
    fCurrentChordClone->
      appendHarmonyToChord (fCurrentHarmonyClone); // JMI
  }

  else if (fOnGoingHarmonyVoice) {
  /* JMI
    // get the harmony whole notes offset
    rational
      harmonyWholeNotesOffset =
        elt->getHarmonyWholeNotesOffset ();

    // is harmonyWholeNotesOffset not equal to 0?
    if (harmonyWholeNotesOffset.getNumerator () != 0) {
      // create skip with duration harmonyWholeNotesOffset
      S_msrNote
        skip =
          msrNote::createSkipNote (
            elt->                getInputLineNumber (),
            "666", // JMI elt->                getHarmonyMeasureNumber (),
            elt->                getHarmonyDisplayWholeNotes (), // would be 0/1 otherwise JMI
            elt->                getHarmonyDisplayWholeNotes (),
            0, // JMI elt->                getHarmonyDotsNumber (),
            fMasterVoice-> getRegularVoiceStaffSequentialNumber (), // JMI
            fMasterVoice-> getVoiceNumber ());

      // append it to the current voice clone
      // to 'push' the harmony aside
      fMasterVoice->
        appendNoteToVoice (skip);
    }
*/

    // append the harmony to the current voice clone
    fMasterVoice->
      appendHarmonyToVoiceClone (
        fCurrentHarmonyClone);
  }

  fOnGoingHarmony = true;
}

void msrVoice2MasterVoiceCreator::visitStart (S_msrHarmonyDegree& elt)
{
#ifdef TRACE_OAH
  if (gGlobalMsrOah->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_msrHarmonyDegree '" <<
      elt->asString () <<
      ", fOnGoingNote = " << booleanAsString (fOnGoingNote) <<
      ", fOnGoingChord = " << booleanAsString (fOnGoingChord) <<
      ", fOnGoingHarmonyVoice = " << booleanAsString (fOnGoingHarmonyVoice) <<
      ", fOnGoingHarmony = " << booleanAsString (fOnGoingHarmony) <<
      "', line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  // append the harmony degree to the current harmony clone
  fCurrentHarmonyClone->
    appendHarmonyDegreeToHarmony (
      elt);
}

void msrVoice2MasterVoiceCreator::visitEnd (S_msrHarmony& elt)
{
#ifdef TRACE_OAH
  if (gGlobalMsrOah->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> End visiting msrHarmony '" <<
      elt->asString () <<
      "'" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  fCurrentHarmonyClone = nullptr;
  fOnGoingHarmony = false;
}

//________________________________________________________________________
void msrVoice2MasterVoiceCreator::visitStart (S_msrFrame& elt)
{
#ifdef TRACE_OAH
  if (gGlobalMsrOah->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> Start visiting msrFrame '" <<
      elt->asString () <<
      "'" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  if (fOnGoingHarmony) {
    // register the frame in the current harmony clone
    fCurrentHarmonyClone->
      setHarmonyFrame (elt);
  }
}

//________________________________________________________________________
void msrVoice2MasterVoiceCreator::visitStart (S_msrFiguredBass& elt)
{
#ifdef TRACE_OAH
  if (gGlobalMsrOah->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> Start visiting msrFiguredBass '" <<
      elt->asString () <<
      "'" <<
      ", fOnGoingFiguredBassVoice = " << booleanAsString (fOnGoingFiguredBassVoice) <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  // create a figured bass new born clone
  fCurrentFiguredBassClone =
    elt->
      createFiguredBassNewbornClone (
        fMasterVoice);

  if (fOnGoingNote) {
    // append the figured bass to the current non-grace note clone
    fCurrentNonGraceNoteClone->
      appendFiguredBassToNoteFiguredBassesList (fCurrentFiguredBassClone);

    // don't append the figured bass to the part figured bass,  JMI ???
    // this will be done below
  }

  /* JMI
  else if (fOnGoingChord) {
    // register the figured bass in the current chord clone
    fCurrentChordClone->
      setChordFiguredBass (fCurrentFiguredBassClone); // JMI
  }
  */

  else if (fOnGoingFiguredBassVoice) { // JMI
    /*
    // register the figured bass in the part clone figured bass
    fCurrentPartClone->
      appendFiguredBassToPartClone (
        fMasterVoice,
        fCurrentFiguredBassClone);
        */
    // append the figured bass to the current voice clone
    fMasterVoice->
      appendFiguredBassToVoiceClone (
        fCurrentFiguredBassClone);
  }
}

void msrVoice2MasterVoiceCreator::visitStart (S_msrFigure& elt)
{
#ifdef TRACE_OAH
  if (gGlobalMsrOah->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> Start visiting msrFigure '" <<
      elt->asString () <<
      "'" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  // append the figure to the current figured bass
  fCurrentFiguredBassClone->
    appendFigureToFiguredBass (
      elt);
}

void msrVoice2MasterVoiceCreator::visitEnd (S_msrFiguredBass& elt)
{
#ifdef TRACE_OAH
  if (gGlobalMsrOah->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> End visiting msrFiguredBass '" <<
      elt->asString () <<
      "'" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  fCurrentFiguredBassClone = nullptr;
}

//________________________________________________________________________
void msrVoice2MasterVoiceCreator::visitStart (S_msrMeasure& elt)
{
  int
    inputLineNumber =
      elt->getInputLineNumber ();

  string
    measureNumber =
      elt->getMeasureElementMeasureNumber ();

  int
    measurePuristNumber =
      elt->getMeasurePuristNumber ();

#ifdef TRACE_OAH
  if (gGlobalMsrOah->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> Start visiting msrMeasure '" <<
      measureNumber <<
      "', measurePuristNumber = '" <<
      measurePuristNumber <<
      "', line " << inputLineNumber <<
      endl;
  }
#endif

#ifdef TRACE_OAH
  if (gGlobalTraceOahGroup->fTraceMeasures) {
    fLogOutputStream <<
      endl <<
      "<!--=== measure '" << measureNumber <<
      "', voice \"" <<
      fMasterVoice->getVoiceName () <<
      "\"" <<
      ", line " << inputLineNumber << " ===-->" <<
      endl;
  }
#endif

  // create a clone of the measure
  fCurrentMeasureClone =
    elt->
      createMeasureNewbornClone (
        fCurrentSegmentClone);

  // is this a full measures rest?
  if (elt->getMeasureIsAFullMeasureRest ()) {
    // yes

    // should we compress full measures rests?
    if (gGlobalLpsr2LilypondOah->fCompressFullMeasureRests) {
      // yes

      if (! fCurrentRestMeasure) {
        // this is the first full measure rest in the sequence

        // create a rest measures  containing fCurrentMeasureClone
        fCurrentRestMeasures =
          msrRestMeasures::create (
            inputLineNumber,
            fCurrentMeasureClone,
            fMasterVoice);

/* JMI
        // append the current rest measures to the current voice clone
        fMasterVoice->
          appendRestMeasuresToVoice (
            inputLineNumber,
            fCurrentRestMeasures);
            */
      }

      else {
        // this is a subsequent full measure rest, merely append it
        fCurrentRestMeasures->
          appendMeasureCloneToRestMeasures (
            fCurrentMeasureClone);
      }

      fCurrentRestMeasure = fCurrentMeasureClone;
    }

    else {
      // no

      // append current measure clone to the current voice clone
      fMasterVoice->
        appendMeasureCloneToVoiceClone (
          inputLineNumber,
          fCurrentMeasureClone);
    }
  }

  else {
    // no

    // append current measure clone to the current voice clone
    fMasterVoice->
      appendMeasureCloneToVoiceClone (
        inputLineNumber,
        fCurrentMeasureClone);
  }

  // should the last bar check's measure purist number be set?
  if (fLastBarCheck) {
    fLastBarCheck->
      setNextBarPuristNumber (
        measurePuristNumber);

    fLastBarCheck = nullptr;
  }
}

void msrVoice2MasterVoiceCreator::visitEnd (S_msrMeasure& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

  string
    measureNumber =
      elt->getMeasureElementMeasureNumber ();

  string
    nextMeasureNumber =
      elt->getNextMeasureNumber ();

  int
    measurePuristNumber =
      elt->getMeasurePuristNumber ();

#ifdef TRACE_OAH
  if (gGlobalMsrOah->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> End visiting msrMeasure '" <<
      measureNumber <<
      "', nextMeasureNumber = '" <<
      nextMeasureNumber <<
      "', measurePuristNumber = '" <<
      measurePuristNumber <<
      "', line " << inputLineNumber <<
      endl;
  }
#endif

  fCurrentMeasureClone->
    finalizeMeasureClone (
      inputLineNumber,
      elt, // original measure
      fMasterVoice);

  bool doCreateABarCheck = false;

  switch (elt->getMeasureKind ()) {
    case msrMeasure::kMeasureKindUnknown:
      {
        stringstream s;

        s <<
          "measure '" << measureNumber <<
          "' in voice \"" <<
          elt->
            fetchMeasureVoiceUpLink ()->
              getVoiceName () <<
          "\" is of unknown kind in msrVoice2MasterVoiceCreator";

      // JMI  msrInternalError (
        msrInternalWarning (
          gGlobalOahOahGroup->fInputSourceName,
          inputLineNumber,
  //        __FILE__, __LINE__,
          s.str ());
      }
      break;

    case msrMeasure::kMeasureKindRegular:
      doCreateABarCheck = true;
      break;

    case msrMeasure::kMeasureKindAnacrusis:
      doCreateABarCheck = true;
      break;

    case msrMeasure::kMeasureKindIncompleteStandalone:
    case msrMeasure::kMeasureKindIncompleteLastInRepeatCommonPart:
    case msrMeasure::kMeasureKindIncompleteLastInRepeatHookedEnding:
    case msrMeasure::kMeasureKindIncompleteLastInRepeatHooklessEnding:
    case msrMeasure::kMeasureKindIncompleteNextMeasureAfterCommonPart:
    case msrMeasure::kMeasureKindIncompleteNextMeasureAfterHookedEnding:
    case msrMeasure::kMeasureKindIncompleteNextMeasureAfterHooklessEnding:
      // generate a bar check if relevant
      switch (elt-> getMeasureEndRegularKind ()) {
        case msrMeasure::kMeasureEndRegularKindUnknown:
          break;
        case msrMeasure::kMeasureEndRegularKindYes:
          doCreateABarCheck = true;
          break;
        case msrMeasure::kMeasureEndRegularKindNo:
          break;
      } // switch
      break;

    case msrMeasure::kMeasureKindOvercomplete:
      doCreateABarCheck = true;
      break;

    case msrMeasure::kMeasureKindCadenza:
      doCreateABarCheck = true;
      break;

    case msrMeasure::kMeasureKindMusicallyEmpty:
      // JMI
      break;
  } // switch

  // is this a full measures rest?
  if (elt->getMeasureIsAFullMeasureRest ()) {
    // yes JMI
  }

  else {
    // no

    // should we compress full measures rests?
    if (gGlobalLpsr2LilypondOah->fCompressFullMeasureRests) {
      // yes

      if (fCurrentRestMeasures) {
        // append the current rest measures to the current voice clone
        fMasterVoice->
          appendRestMeasuresToVoice (
            inputLineNumber,
            fCurrentRestMeasures);

        // forget about the current rest measure
        fCurrentRestMeasure = nullptr;

        // forget about the current rest measures
        fCurrentRestMeasures = nullptr;
      }

      else {
        stringstream s;

        s <<
          "fCurrentRestMeasures is null upon full measure rest end" <<
          measureNumber <<
          "', measurePuristNumber = '" <<
          measurePuristNumber <<
          "', line " << inputLineNumber;

        msrInternalError (
          gGlobalOahOahGroup->fInputSourceName,
          inputLineNumber,
          __FILE__, __LINE__,
          s.str ());
      }
    }
  }

  if (doCreateABarCheck) {
    // create a bar check
    int voiceCurrentMeasurePuristNumber =
      fMasterVoice->
        getVoiceCurrentMeasurePuristNumber ();

    fLastBarCheck =
      msrBarCheck::createWithNextBarPuristNumber (
        inputLineNumber,
        nextMeasureNumber,
        voiceCurrentMeasurePuristNumber);

    // append it to the current voice clone
    fMasterVoice->
      appendBarCheckToVoice (fLastBarCheck);

    // create a bar number check
    // should NOT be done in cadenza, SEE TO IT JMI
    S_msrBarNumberCheck
      barNumberCheck_ =
        msrBarNumberCheck::create (
          inputLineNumber,
          nextMeasureNumber,
          voiceCurrentMeasurePuristNumber);

    // append it to the current voice clone
    fMasterVoice->
      appendBarNumberCheckToVoice (barNumberCheck_);
  }
}

//________________________________________________________________________
void msrVoice2MasterVoiceCreator::visitStart (S_msrKey& elt)
{
#ifdef TRACE_OAH
  if (gGlobalMsrOah->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> Start visiting msrKey" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  fMasterVoice->
    appendKeyToVoice (elt);
}

void msrVoice2MasterVoiceCreator::visitEnd (S_msrKey& elt)
{
#ifdef TRACE_OAH
  if (gGlobalMsrOah->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> End visiting msrKey" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif
}

//________________________________________________________________________
void msrVoice2MasterVoiceCreator::visitStart (S_msrTime& elt)
{
#ifdef TRACE_OAH
  if (gGlobalMsrOah->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> Start visiting msrTime" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  // append time to voice clone
  fMasterVoice->
    appendTimeToVoice (elt);
}

void msrVoice2MasterVoiceCreator::visitEnd (S_msrTime& elt)
{
#ifdef TRACE_OAH
  if (gGlobalMsrOah->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> End visiting msrTime" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif
}

//________________________________________________________________________
void msrVoice2MasterVoiceCreator::visitStart (S_msrTempo& elt)
{
#ifdef TRACE_OAH
  if (gGlobalMsrOah->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> Start visiting msrTempo" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  fMasterVoice->
    appendTempoToVoice (elt);
}

void msrVoice2MasterVoiceCreator::visitEnd (S_msrTempo& elt)
{
#ifdef TRACE_OAH
  if (gGlobalMsrOah->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> End visiting msrTempo" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif
}

//________________________________________________________________________
void msrVoice2MasterVoiceCreator::visitStart (S_msrRehearsal& elt)
{
#ifdef TRACE_OAH
  if (gGlobalMsrOah->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> Start visiting msrRehearsal" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  fMasterVoice->
    appendRehearsalToVoice (elt);
}

void msrVoice2MasterVoiceCreator::visitEnd (S_msrRehearsal& elt)
{
#ifdef TRACE_OAH
  if (gGlobalMsrOah->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> End visiting msrRehearsal" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif
}

//________________________________________________________________________
void msrVoice2MasterVoiceCreator::visitStart (S_msrFermata& elt)
{
#ifdef TRACE_OAH
  if (gGlobalMsrOah->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> Start visiting msrFermata" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  // a fermata is an articulation

  if (fOnGoingNote) {
    fCurrentNonGraceNoteClone->
      appendArticulationToNote (elt);
  }
  else if (fOnGoingChord) {
    fCurrentChordClone->
      appendArticulationToChord (elt);
  }
}

//________________________________________________________________________
void msrVoice2MasterVoiceCreator::visitStart (S_msrSlash& elt)
{
#ifdef TRACE_OAH
  if (gGlobalMsrOah->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> Start visiting msrSlash" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  if (fOnGoingNote) {
    fCurrentNonGraceNoteClone->
      appendSlashToNote (elt);
  }

  else if (fOnGoingChord) {
    fCurrentChordClone->
      appendSlashToChord (elt);
  }
}

//________________________________________________________________________
void msrVoice2MasterVoiceCreator::visitStart (S_msrNote& elt)
{
#ifdef TRACE_OAH
  if (gGlobalMsrOah->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> Start visiting msrNote '" <<
      elt->asString () <<
      "'" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  // create the note clone
  S_msrNote
    noteClone =
      elt->createNoteNewbornClone (
        nullptr); // JMI ??? CRASH

  switch (elt->getNoteKind ()) {

    case msrNote::kGraceNote:
    case msrNote::kGraceSkipNote:
    case msrNote::kGraceChordMemberNote:
    case msrNote::kGraceTupletMemberNote:
      break;

    default:
      fCurrentNonGraceNoteClone = noteClone;

      if (! fFirstNoteCloneInVoice) {
        fFirstNoteCloneInVoice =
          fCurrentNonGraceNoteClone;

#ifdef TRACE_OAH
        if (gGlobalTraceOahGroup->fTraceNotes || gGlobalTraceOahGroup->fTraceVoices) {
          fLogOutputStream <<
            "The first note of voice clone RJIRWR '" <<
            fMasterVoice->getVoiceName () <<
            "' is '" <<
            fFirstNoteCloneInVoice->asShortString () <<
             "'" <<
            endl;
        }
#endif
      }

      fOnGoingNote = true;
  } // switch

/* JMI
  // can we optimize graceNotesGroup into afterGraceNotesGroup?
  if (
    elt->getNoteIsFollowedByGraceNotesGroup ()
      &&
    elt->getNoteTrillOrnament ()) {
    // yes, create the after grace notes
#ifdef TRACE_OAH
    if (gGlobalTraceOahGroup->fTraceNotesGroup) {
      fLogOutputStream <<
        "Optimizing grace notes on trilled note '" <<
        elt->asShortString () <<
        "' as after grace notes " <<
        ", line " << inputLineNumber <<
        endl;
    }
#endif

    fPendingAfterGraceNotesGroup =
      msrAfterGraceNotesGroup::create (
        inputLineNumber,
        fCurrentNonGraceNoteClone,
        false, // aftergracenoteIsSlashed, may be updated later
        fMasterVoice);

    // register current afterGraceNotesGroup element
    fCurrentAfterGraceNotesGroupElement =
      fCurrentNonGraceNoteClone;
  }
*/
}

void msrVoice2MasterVoiceCreator::visitEnd (S_msrNote& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACE_OAH
  if (gGlobalMsrOah->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> End visiting msrNote " <<
      elt->asString () <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

#ifdef TRACE_OAH
  if (gGlobalTraceOahGroup->fTraceNotesDetails) {
    fLogOutputStream <<
      "FAA fCurrentNonGraceNoteClone = " <<
      endl;
    if (fCurrentNonGraceNoteClone) {
      fLogOutputStream <<
        fCurrentNonGraceNoteClone;
    }
    else {
      fLogOutputStream <<
        "nullptr" <<
        endl;
    }
  }
#endif

  switch (elt->getNoteKind ()) {

    case msrNote::k_NoNoteKind:
      break;

    case msrNote::kRestNote:
#ifdef TRACE_OAH
      if (gGlobalTraceOahGroup->fTraceNotes) {
        fLogOutputStream <<
          "Appending rest note clone '" <<
          fCurrentNonGraceNoteClone->asShortString () << "' to voice clone " <<
          fMasterVoice->getVoiceName () <<
          endl;
      }
#endif

      fMasterVoice->
        appendNoteToVoiceClone (
          fCurrentNonGraceNoteClone);
      break;

    case msrNote::kSkipNote: // JMI
#ifdef TRACE_OAH
      if (gGlobalTraceOahGroup->fTraceNotes) {
        fLogOutputStream <<
          "Appending skip note clone '" <<
          fCurrentNonGraceNoteClone->asShortString () << "' to voice clone " <<
          fMasterVoice->getVoiceName () <<
          endl;
      }
#endif

      fMasterVoice->
        appendNoteToVoiceClone (
          fCurrentNonGraceNoteClone);
      break;

    case msrNote::kUnpitchedNote:
#ifdef TRACE_OAH
      if (gGlobalTraceOahGroup->fTraceNotes) {
        fLogOutputStream <<
          "Appending unpitched note clone '" <<
          fCurrentNonGraceNoteClone->asShortString () << "' to voice clone " <<
          fMasterVoice->getVoiceName () <<
          endl;
      }
#endif

      fMasterVoice->
        appendNoteToVoiceClone (
          fCurrentNonGraceNoteClone);
      break;

    case msrNote::kRegularNote:
#ifdef TRACE_OAH
      if (gGlobalTraceOahGroup->fTraceNotes) {
        fLogOutputStream <<
          "Appending regular note clone '" <<
          fCurrentNonGraceNoteClone->asShortString () << "' to voice clone " <<
          fMasterVoice->getVoiceName () <<
          endl;
      }
#endif

      fMasterVoice->
        appendNoteToVoiceClone (
          fCurrentNonGraceNoteClone);
      break;

    case msrNote::kDoubleTremoloMemberNote:
      break;

    case msrNote::kGraceNote:
    case msrNote::kGraceSkipNote:
      break;

    case msrNote::kChordMemberNote:
      if (fOnGoingChord) {
        fCurrentChordClone->
          addAnotherNoteToChord (
            fCurrentNonGraceNoteClone,
            fMasterVoice);
      }

      else {
        stringstream s;

        s <<
          "msrVoice2MasterVoiceCreator:::visitEnd (S_msrNote& elt): chord member note " <<
          elt->asString () <<
          " appears outside of a chord";

        msrInternalError (
          gGlobalOahOahGroup->fInputSourceName,
          inputLineNumber,
          __FILE__, __LINE__,
          s.str ());
        }
      break;

    case msrNote::kGraceChordMemberNote:
      if (fOnGoingChord) {
      /* JMI
        fCurrentChordClone->
          addAnotherNoteToChord (
            fCurrentGraceNoteClone,
            fMasterVoice);
            */
      }

      else {
        stringstream s;

        s <<
          "msrVoice2MasterVoiceCreator:::visitEnd (S_msrNote& elt): chord member note " <<
          elt->asString () <<
          " appears outside of a chord";

        msrInternalError (
          gGlobalOahOahGroup->fInputSourceName,
          inputLineNumber,
          __FILE__, __LINE__,
          s.str ());
        }
      break;

    case msrNote::kTupletMemberNote:
    case msrNote::kTupletRestMemberNote:
    case msrNote::kGraceTupletMemberNote:
    case msrNote::kTupletUnpitchedMemberNote:
#ifdef TRACE_OAH
      if (gGlobalTraceOahGroup->fTraceNotes) {
        fLogOutputStream <<
          "Appending note clone '" <<
          fCurrentNonGraceNoteClone->asShortString () << "'' to voice clone " <<
          fMasterVoice->getVoiceName () <<
          endl;
      }
#endif

      fTupletClonesStack.top ()->
        appendNoteToTuplet (
          fCurrentNonGraceNoteClone,
          fMasterVoice);
      break;
  } // switch

  fOnGoingNote = false;
}

//________________________________________________________________________
void msrVoice2MasterVoiceCreator::visitStart (S_msrChord& elt)
{
#ifdef TRACE_OAH
  int inputLineNumber =
    elt->getInputLineNumber ();

  if (gGlobalMsrOah->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> Start visiting msrChord" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  fCurrentChordClone =
    elt->createChordNewbornClone (
      nullptr); // JMI ??? CRASH

/* JMI
  if (fTupletClonesStack.size ()) {
    // a chord in a tuplet is handled as part of the tuplet JMI
    fTupletClonesStack.top ()->
      appendChordToTuplet (
        fCurrentChordClone);
  }

  else if (fOnGoingDoubleTremolo) {
    if (elt->getChordIsFirstChordInADoubleTremolo ()) {
    / * JMI
      // replace double tremolo's first element by chord
      fCurrentDoubleTremoloClone->
        setDoubleTremoloChordFirstElement (
          elt)
          * /
    }

    else if (elt->getChordIsSecondChordInADoubleTremolo ()) {
    / * JMI
      // replace double tremolo's second element by chord
      fCurrentDoubleTremoloClone->
        setDoubleTremoloChordSecondElement (
          elt);
          * /
    }

    else {
      stringstream s;

      s <<
        "chord '" << elt->asString () <<
        "' belongs to a double tremolo, but is not marked as such";

      msrInternalError (
        gGlobalOahOahGroup->fInputSourceName,
        inputLineNumber,
        __FILE__, __LINE__,
        s.str ());
    }
  }

  else if (fCurrentGraceNotesGroupClone) {
    // append the chord to the grace notes
    fCurrentGraceNotesGroupClone->
      appendChordToGraceNotesGroup (
        fCurrentChordClone);
  }

  else {
    // appending the chord to the voice clone at once
    fMasterVoice->
      appendChordToVoice (
        fCurrentChordClone);
  }
  */

  fOnGoingChord = true;
}

void msrVoice2MasterVoiceCreator::visitEnd (S_msrChord& elt)
{
#ifdef TRACE_OAH
  if (gGlobalMsrOah->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> End visiting msrChord" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  fOnGoingChord = false;
}

//________________________________________________________________________
void msrVoice2MasterVoiceCreator::visitStart (S_msrTuplet& elt)
{
#ifdef TRACE_OAH
  if (gGlobalMsrOah->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> Start visiting msrTuplet" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  // create the tuplet clone
  S_msrTuplet
    tupletClone =
      elt->createTupletNewbornClone ();

  // register it in this visitor
#ifdef TRACE_OAH
  if (gGlobalTraceOahGroup->fTraceTuplets) {
    fLogOutputStream <<
      "++> pushing tuplet '" <<
      tupletClone->asString () <<
      "' to tuplets stack" <<
      endl;
  }
#endif

  fTupletClonesStack.push (tupletClone);
}

void msrVoice2MasterVoiceCreator::visitEnd (S_msrTuplet& elt)
{
#ifdef TRACE_OAH
  if (gGlobalMsrOah->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> End visiting msrTuplet" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

#ifdef TRACE_OAH
  if (gGlobalTraceOahGroup->fTraceTuplets) {
    fLogOutputStream <<
      "Popping tuplet '" <<
      elt->asString () <<
      "' from tuplets stack" <<
      endl;
  }
#endif

  fTupletClonesStack.pop ();

  if (fTupletClonesStack.size ()) {
    // tuplet is a nested tuplet
#ifdef TRACE_OAH
    if (gGlobalTraceOahGroup->fTraceTuplets) {
      fLogOutputStream <<
        "Adding nested tuplet '" <<
      elt->asString () <<
        "' to stack top tuplet '" <<
      fTupletClonesStack.top ()->asString () <<
      "'" <<
      endl;
    }
#endif

    fTupletClonesStack.top ()->
      addTupletToTupletClone (elt);
  }

  else {
    // tuplet is a top level tuplet

#ifdef TRACE_OAH
    if (gGlobalTraceOahGroup->fTraceTuplets) {
      fLogOutputStream <<
        "Adding top level tuplet '" <<
      elt->asString () <<
      "' to voice" <<
      fMasterVoice->getVoiceName () <<
      endl;
    }
#endif

    fMasterVoice->
      appendTupletToVoice (elt);
  }
}

//________________________________________________________________________
void msrVoice2MasterVoiceCreator::visitStart (S_msrSegno& elt)
{
#ifdef TRACE_OAH
  if (gGlobalMsrOah->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> Start visiting msrSegno" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  fMasterVoice->
    appendSegnoToVoice (elt);
}

void msrVoice2MasterVoiceCreator::visitStart (S_msrCoda& elt)
{
#ifdef TRACE_OAH
  if (gGlobalMsrOah->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> Start visiting msrCoda" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  fMasterVoice->
    appendCodaToVoice (elt);
}

//________________________________________________________________________
void msrVoice2MasterVoiceCreator::visitStart (S_msrScordatura& elt)
{
#ifdef TRACE_OAH
  if (gGlobalMsrOah->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> Start visiting scordatura" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  fCurrentNonGraceNoteClone->
    appendScordaturaToNote (elt);
}

//________________________________________________________________________
void msrVoice2MasterVoiceCreator::visitStart (S_msrBarCheck& elt)
{
#ifdef TRACE_OAH
  if (gGlobalMsrOah->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> Start visiting msrBarCheck" <<
      ", nextBarNumber: " <<
      elt->getNextBarPuristNumber () <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  fMasterVoice->
    appendBarCheckToVoice (elt);
}

void msrVoice2MasterVoiceCreator::visitEnd (S_msrBarCheck& elt)
{
#ifdef TRACE_OAH
  if (gGlobalMsrOah->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> End visiting msrBarCheck" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif
}

//________________________________________________________________________
void msrVoice2MasterVoiceCreator::visitStart (S_msrBarNumberCheck& elt)
{
#ifdef TRACE_OAH
  if (gGlobalMsrOah->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> Start visiting msrBarNumberCheck" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  fMasterVoice->
    appendBarNumberCheckToVoice (elt);
}

void msrVoice2MasterVoiceCreator::visitEnd (S_msrBarNumberCheck& elt)
{
#ifdef TRACE_OAH
  if (gGlobalMsrOah->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> End visiting msrBarNumberCheck" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif
}

//________________________________________________________________________
void msrVoice2MasterVoiceCreator::visitStart (S_msrLineBreak& elt)
{
#ifdef TRACE_OAH
  if (gGlobalMsrOah->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> Start visiting msrLineBreak" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  fMasterVoice->
    appendLineBreakToVoice (elt);
}

void msrVoice2MasterVoiceCreator::visitEnd (S_msrLineBreak& elt)
{
#ifdef TRACE_OAH
  if (gGlobalMsrOah->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> End visiting msrLineBreak" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif
}

//________________________________________________________________________
void msrVoice2MasterVoiceCreator::visitStart (S_msrPageBreak& elt)
{
#ifdef TRACE_OAH
  if (gGlobalMsrOah->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> Start visiting msrPageBreak" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  fMasterVoice->
    appendPageBreakToVoice (elt);
}

void msrVoice2MasterVoiceCreator::visitEnd (S_msrPageBreak& elt)
{
#ifdef TRACE_OAH
  if (gGlobalMsrOah->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> End visiting msrPageBreak" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif
}

//________________________________________________________________________
void msrVoice2MasterVoiceCreator::visitStart (S_msrRepeat& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACE_OAH
  if (gGlobalMsrOah->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> Start visiting msrRepeat" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

#ifdef TRACE_OAH
  if (gGlobalTraceOahGroup->fTraceRepeats) {
    fLogOutputStream <<
      "Handling repeat start in voice clone \"" <<
      fMasterVoice->getVoiceName () <<
      "\"" <<
      endl;
  }
#endif

  fMasterVoice->
    handleRepeatStartInVoiceClone (
      inputLineNumber,
      elt);
}

void msrVoice2MasterVoiceCreator::visitEnd (S_msrRepeat& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACE_OAH
  if (gGlobalMsrOah->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> End visiting msrRepeat" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

#ifdef TRACE_OAH
  if (gGlobalTraceOahGroup->fTraceRepeats) {
    fLogOutputStream <<
      "Handling repeat end in voice clone \"" <<
      fMasterVoice->getVoiceName () <<
//      "\" in part \"" <<
//      fCurrentPartClone->getPartCombinedName () <<
      "\"" <<
      endl;
  }
#endif

  fMasterVoice->
    handleRepeatEndInVoiceClone (
      inputLineNumber);
}

//________________________________________________________________________
void msrVoice2MasterVoiceCreator::visitStart (S_msrRepeatCommonPart& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACE_OAH
  if (gGlobalMsrOah->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> Start visiting msrRepeatCommonPart" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  fMasterVoice->
    handleRepeatCommonPartStartInVoiceClone (
      inputLineNumber);
}

void msrVoice2MasterVoiceCreator::visitEnd (S_msrRepeatCommonPart& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACE_OAH
  if (gGlobalMsrOah->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> End visiting msrRepeatCommonPart" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  fMasterVoice->
    handleRepeatCommonPartEndInVoiceClone (
      inputLineNumber);
}

//________________________________________________________________________
void msrVoice2MasterVoiceCreator::visitStart (S_msrRepeatEnding& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACE_OAH
  if (gGlobalMsrOah->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> Start visiting msrRepeatEnding" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  // handle the repeat ending start in the voice clone
#ifdef TRACE_OAH
  if (gGlobalTraceOahGroup->fTraceRepeats) {
    fLogOutputStream <<
      "Handling a repeat ending start in voice clone \"" <<
      fMasterVoice->getVoiceName () <<
      "\"" <<
      endl;
  }
#endif

  fMasterVoice->
    handleRepeatEndingStartInVoiceClone (
      inputLineNumber,
      elt->getRepeatEndingKind (),
      elt->getRepeatEndingNumber ());
}

void msrVoice2MasterVoiceCreator::visitEnd (S_msrRepeatEnding& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACE_OAH
  if (gGlobalMsrOah->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> End visiting msrRepeatEnding" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  // handle the repeat ending end in the voice clone
#ifdef TRACE_OAH
  if (gGlobalTraceOahGroup->fTraceRepeats) {
    fLogOutputStream <<
      "Handling a repeat ending end in voice clone \"" <<
      fMasterVoice->getVoiceName () <<
      "\"" <<
      endl;
  }
#endif

  fMasterVoice->
    handleRepeatEndingEndInVoiceClone (
      inputLineNumber,
      elt->getRepeatEndingNumber (),
      elt->getRepeatEndingKind ());
}

//________________________________________________________________________
void msrVoice2MasterVoiceCreator::visitStart (S_msrRestMeasures& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACE_OAH
  if (gGlobalMsrOah->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> Start visiting msrRestMeasures" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  gIndenter++;

#ifdef TRACE_OAH
  if (gGlobalTraceOahGroup->fTraceRestMeasures) {
    fLogOutputStream <<
      "Handling multiple rest start in voice clone \"" <<
      fMasterVoice->getVoiceName () <<
      "\"" <<
      endl;
  }
#endif

  fMasterVoice->
    handleRestMeasuresStartInVoiceClone (
      inputLineNumber,
      elt);
}

void msrVoice2MasterVoiceCreator::visitEnd (S_msrRestMeasures& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACE_OAH
  if (gGlobalMsrOah->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> End visiting msrRestMeasures" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  gIndenter--;

#ifdef TRACE_OAH
  if (gGlobalTraceOahGroup->fTraceRestMeasures) {
    fLogOutputStream <<
      "Handling multiple rest start in voice clone \"" <<
      fMasterVoice->getVoiceName () <<
      "\"" <<
      endl;
  }
#endif

  fMasterVoice->
    handleRestMeasuresEndInVoiceClone (
      inputLineNumber);
}

//________________________________________________________________________
void msrVoice2MasterVoiceCreator::visitStart (S_msrRestMeasuresContents& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACE_OAH
  if (gGlobalMsrOah->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> Start visiting msrRestMeasuresContents" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  gIndenter++;

#ifdef TRACE_OAH
  if (gGlobalTraceOahGroup->fTraceRestMeasures) {
    fMasterVoice->
      displayVoice (
        inputLineNumber,
        "Upon visitStart (S_msrRestMeasuresContents&)");
  }
#endif

  fMasterVoice->
    handleRestMeasuresContentsStartInVoiceClone (
      inputLineNumber);
}

void msrVoice2MasterVoiceCreator::visitEnd (S_msrRestMeasuresContents& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACE_OAH
  if (gGlobalMsrOah->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> End visiting msrRestMeasuresContents" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  gIndenter--;

#ifdef TRACE_OAH
  if (gGlobalTraceOahGroup->fTraceRestMeasures) {
    fMasterVoice->
      displayVoice (
        inputLineNumber,
        "Upon visitEnd (S_msrRestMeasuresContents&) 1");
  }
#endif

  fMasterVoice->
    handleRestMeasuresContentsEndInVoiceClone (
      inputLineNumber);
}

//________________________________________________________________________
void msrVoice2MasterVoiceCreator::visitStart (S_msrMeasuresRepeat& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACE_OAH
  if (gGlobalMsrOah->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> Start visiting msrMeasuresRepeat" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  gIndenter++;

#ifdef TRACE_OAH
  if (gGlobalTraceOahGroup->fTraceMeasuresRepeats) {
    fLogOutputStream <<
      "Handling measures repeat start in voice clone \"" <<
      fMasterVoice->getVoiceName () <<
      "\"" <<
      endl;
  }
#endif

  fMasterVoice->
    handleMeasuresRepeatStartInVoiceClone (
      inputLineNumber,
      elt);
}

void msrVoice2MasterVoiceCreator::visitEnd (S_msrMeasuresRepeat& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACE_OAH
  if (gGlobalMsrOah->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> End visiting msrMeasuresRepeat" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  gIndenter--;

/* JMI
  // set last segment as the measures repeat pattern segment
#ifdef TRACE_OAH
  if (gGlobalTraceOahGroup->fTraceMeasuresRepeats) {
    fLogOutputStream <<
      "Setting current last segment as measures repeat pattern segment in voice \"" <<
      fMasterVoice->getVoiceName () <<
      "\"" <<
      endl;
  }
#endif
*/

#ifdef TRACE_OAH
  if (gGlobalTraceOahGroup->fTraceMeasuresRepeats) {
    fLogOutputStream <<
      "Handling measures repeat end in voice clone \"" <<
      fMasterVoice->getVoiceName () <<
      "\"" <<
      endl;
  }
#endif

  fMasterVoice->
    handleMeasuresRepeatEndInVoiceClone (
      inputLineNumber);
}

//________________________________________________________________________
void msrVoice2MasterVoiceCreator::visitStart (S_msrMeasuresRepeatPattern& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACE_OAH
  if (gGlobalMsrOah->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> Start visiting msrMeasuresRepeatPattern" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  gIndenter++;

#ifdef TRACE_OAH
  if (gGlobalTraceOahGroup->fTraceMeasuresRepeats) {
    fMasterVoice->
      displayVoice (
        inputLineNumber,
        "Upon visitStart (S_msrMeasuresRepeatPattern&)");
  }
#endif

  fMasterVoice->
    handleMeasuresRepeatPatternStartInVoiceClone (
      inputLineNumber);
}

void msrVoice2MasterVoiceCreator::visitEnd (S_msrMeasuresRepeatPattern& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACE_OAH
  if (gGlobalMsrOah->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> End visiting msrMeasuresRepeatPattern" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  gIndenter--;

#ifdef TRACE_OAH
  if (gGlobalTraceOahGroup->fTraceMeasuresRepeats) {
    fMasterVoice->
      displayVoice (
        inputLineNumber,
        "Upon visitEnd (S_msrMeasuresRepeatPattern&) 1");
  }
#endif

  fMasterVoice->
    handleMeasuresRepeatPatternEndInVoiceClone (
      inputLineNumber);
}

//________________________________________________________________________
void msrVoice2MasterVoiceCreator::visitStart (S_msrMeasuresRepeatReplicas& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACE_OAH
  if (gGlobalMsrOah->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> Start visiting msrMeasuresRepeatReplicas" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  gIndenter++;

#ifdef TRACE_OAH
  if (gGlobalTraceOahGroup->fTraceMeasuresRepeats) {
    fMasterVoice->
      displayVoice (
        inputLineNumber,
        "Upon visitStart (S_msrMeasuresRepeatReplicas&)");
  }
#endif

  fMasterVoice->
    handleMeasuresRepeatReplicasStartInVoiceClone (
      inputLineNumber);
}

void msrVoice2MasterVoiceCreator::visitEnd (S_msrMeasuresRepeatReplicas& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACE_OAH
  if (gGlobalMsrOah->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> End visiting S_msrMeasuresRepeatReplicas" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  gIndenter--;


#ifdef TRACE_OAH
  if (gGlobalTraceOahGroup->fTraceMeasuresRepeats) {
    fMasterVoice->
      displayVoice (
        inputLineNumber,
        "Upon visitEnd (S_msrMeasuresRepeatReplicas&) 1");
  }
#endif

  fMasterVoice->
    handleMeasuresRepeatReplicasEndInVoiceClone (
      inputLineNumber);
}

//________________________________________________________________________
void msrVoice2MasterVoiceCreator::visitStart (S_msrBarline& elt)
{
#ifdef TRACE_OAH
  int inputLineNumber =
    elt->getInputLineNumber ();
#endif

#ifdef TRACE_OAH
  if (gGlobalMsrOah->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> Start visiting msrBarline" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

#ifdef TRACE_OAH
  if (gGlobalTraceOahGroup->fTraceBarlines) {
    fLogOutputStream <<
      "Handling '" <<
      msrBarline::barlineCategoryKindAsString (
        elt->getBarlineCategory ()) <<
      "' in voice \"" <<
      fMasterVoice->getVoiceName () << "\"" <<
      endl;
  }
#endif

  // append the barline to the current voice clone
  fMasterVoice->
    appendBarlineToVoice (elt);
}

void msrVoice2MasterVoiceCreator::visitEnd (S_msrBarline& elt)
{
#ifdef TRACE_OAH
  if (gGlobalMsrOah->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> End visiting msrBarline" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif
}


} // namespace
