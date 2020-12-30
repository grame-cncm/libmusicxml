/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#include <climits>      // INT_MIN, INT_MAX
#include <iomanip>      // setw, setprecision, ...
#include <algorithm>    // for_each

#include "msrParts.h"

#include "enableTracingIfDesired.h"
#ifdef TRACING_IS_ENABLED
  #include "traceOah.h"
#endif

#include "oahOah.h"

#include "mxmlTreeOah.h"
#include "msrOah.h"

using namespace std;

namespace MusicXML2
{

//______________________________________________________________________________
int msrPart::gPartsCounter = 0;

S_msrPart msrPart::create (
  int            inputLineNumber,
  string         partID,
  S_msrPartGroup partPartGroupUpLink)
{
  msrPart* o =
    new msrPart (
      inputLineNumber,
      partID,
      partPartGroupUpLink);
  assert (o!=0);
  return o;
}

msrPart::msrPart (
  int            inputLineNumber,
  string         partID,
  S_msrPartGroup partPartGroupUpLink)
    : msrPartGroupElement (inputLineNumber)
{
  // replace spaces in partID to set fPartID
  for_each (
    partID.begin (),
    partID.end (),
    stringSpaceReplacer (fPartID, '_'));

/* JMI
  // sanity check
  msgAssert(
    partPartGroupUpLink != nullptr,
    "partPartGroupUpLink is null");
    */

  // set part number
  fPartAbsoluteNumber = ++gPartsCounter;

  // set part's part group upLink
  fPartPartGroupUpLink = partPartGroupUpLink;

  // do other initializations
  initializePart ();
}

void msrPart::initializePart ()
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceParts ()) {
    gLogStream <<
      "Creating part \"" << asString () << "\"" <<
      endl;

    gGlobalMsrOahGroup->printMsrOahValues (40); // JMI
  }
#endif

  // is this part name in the part renaming map?
  map<string, string>::const_iterator
    it =
      gGlobalMsrOahGroup->getPartsRenamingMap ().find (fPartID);

  if (it != gGlobalMsrOahGroup->getPartsRenamingMap ().end ()) {
    // yes, rename the part accordinglingly
    fPartMsrName = (*it).second;
  }
  else {
    // coin the name from the argument
    fPartMsrName =
      "Part_"+stringNumbersToEnglishWords (fPartID);
  }

  // initialize part's number of measures
  fPartNumberOfMeasures = 0;

  // rest measures
  fPartContainsRestMeasures = false;

  // current position in measure
  fPartCurrentPositionInMeasure = rational (0,0);

  // part shortest note duration
  fPartShortestNoteDuration = rational (INT_MAX, 1);

  // part shortest note tuplet factor
  fPartShortestNoteTupletFactor = rational (1, 1);

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceParts ()) {
    gLogStream <<
      "Creating part \"" << asString () << "\"" <<
      endl;
  }
#endif
}

msrPart::~msrPart ()
{}

S_msrScore  msrPart::fetchPartScoreUpLink () const
{
  return
    fPartPartGroupUpLink->
      getPartGroupScoreUpLink ();
}

S_msrPart msrPart::createPartNewbornClone (S_msrPartGroup partGroupClone)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceParts ()) {
    gLogStream <<
      "Creating a newborn clone of part " <<
      getPartCombinedName () <<
      endl;
  }
#endif

  // sanity check
  msgAssert(
    partGroupClone != nullptr,
    "partGroupClone is null");

  S_msrPart
    newbornClone =
      msrPart::create (
        fInputLineNumber,
        fPartID,
        partGroupClone);

  newbornClone->fPartMsrName =
    fPartMsrName;

  newbornClone->fPartName =
    fPartName;
  newbornClone->fPartNameDisplayText =
    fPartNameDisplayText;

  newbornClone->fPartAbbreviation =
    fPartAbbreviation;
  newbornClone->fPartAbbreviationDisplayText =
    fPartAbbreviationDisplayText;

  newbornClone->fPartInstrumentName =
    fPartInstrumentName;
  newbornClone->fPartInstrumentAbbreviation =
    fPartInstrumentAbbreviation;

  // set the newbornClone's number of measures
  newbornClone->
    setPartNumberOfMeasures (
      fPartNumberOfMeasures);

  return newbornClone;
}

void msrPart::setPartCurrentPositionInMeasure (
  int      inputLineNumber,
  rational positionInMeasure)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTracePositionsInMeasures ()) {
    gLogStream <<
      "Setting part current position in measure to " <<
      positionInMeasure <<
      " in part " <<
      getPartCombinedName () <<
      endl;
  }
#endif

  if (positionInMeasure.getNumerator () < 0) {
    stringstream s;

    s <<
      "cannot set part current position in measure to " <<
      positionInMeasure <<
      " in part " <<
      getPartCombinedName () <<
      " since it is negative";

    msrInternalError (
      gGlobalOahOahGroup->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  }

  fPartCurrentPositionInMeasure =
    positionInMeasure;
}

void msrPart::incrementPartCurrentPositionInMeasure (
  int      inputLineNumber,
  rational duration)
{
  fPartCurrentPositionInMeasure += duration;
  fPartCurrentPositionInMeasure.rationalise ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTracePositionsInMeasures ()) {
    gLogStream <<
      "Incrementing part current position in measure by " <<
      duration <<
      " in part " <<
      getPartCombinedName () <<
      ", thus setting it to " <<
      fPartCurrentPositionInMeasure <<
      endl;
  }
#endif
}

void msrPart::decrementPartCurrentPositionInMeasure (
  int      inputLineNumber,
  rational duration)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTracePositionsInMeasures ()) {
    gLogStream <<
      "Decrementing part current position in measure by " <<
      duration <<
      " in part " <<
      getPartCombinedName () <<
      endl;
  }
#endif

  fPartCurrentPositionInMeasure -= duration;
  fPartCurrentPositionInMeasure.rationalise ();

  if (fPartCurrentPositionInMeasure.getNumerator () < 0) {
    stringstream s;

    s <<
      "cannot decrement part current position in measure by " <<
      duration <<
      " in part " <<
      getPartCombinedName () <<
      " since that sets it to " <<
      fPartCurrentPositionInMeasure <<
      ", which is negative ";

    msrInternalError (
      gGlobalOahOahGroup->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  }

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTracePositionsInMeasures ()) {
    gLogStream <<
      "The new part current position in measure is " <<
      fPartCurrentPositionInMeasure <<
      " in part " <<
      getPartCombinedName () <<
      endl;
  }
#endif
}

void msrPart::setPartShortestNoteDuration (
  rational duration)
{
#ifdef TRACING_IS_ENABLED
  if (
    gGlobalTraceOahGroup->getTraceNotes ()
      ||
    gGlobalMsrOahGroup->getTraceMsrDurations ()
  ) {
    gLogStream <<
      "Setting the shortest note duration of part " <<
      fPartName <<
      " to " <<
      duration <<
      endl;
  }
#endif

  fPartShortestNoteDuration = duration;
}

void msrPart::setPartShortestNoteTupletFactor (
  const msrTupletFactor& noteTupletFactor)
{
#ifdef TRACING_IS_ENABLED
  if (
    gGlobalTraceOahGroup->getTraceNotes ()
      ||
    gGlobalMsrOahGroup->getTraceMsrDurations ()
  ) {
    gLogStream <<
      "Setting the shortest note tuplet factor of part " <<
      fPartName <<
      " to " <<
      noteTupletFactor <<
      endl;
  }
#endif

  fPartShortestNoteTupletFactor = noteTupletFactor;
}

void msrPart::assignSequentialNumbersToRegularVoicesInPart (
  int inputLineNumber)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceParts () || gGlobalTraceOahGroup->getTraceVoices ()) {
    gLogStream <<
      "Assigning sequential numbers to the regular voices in part \"" <<
      fPartID <<
      ", " <<
      fPartName <<
      "\"" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  for (
    map<int, S_msrStaff>::const_iterator i = fPartStavesMap.begin ();
    i != fPartStavesMap.end ();
    i++
  ) {
    (*i).second->
      assignSequentialNumbersToRegularVoicesInStaff (
        inputLineNumber);
  } // for
}

/* JMI
void msrPart::addAVoiceToStavesThatHaveNone (
  int inputLineNumber)
{
  for (
    map<int, S_msrStaff>::const_iterator i = fPartStavesMap.begin ();
    i != fPartStavesMap.end ();
    i++
  ) {
    (*i).second->
      addAVoiceToStaffIfItHasNone (
        inputLineNumber);
  } // for
}
*/

void msrPart::setPartMsrName (string partMsrName)
{
  // is this part name in the part renaming map?
  map<string, string>::const_iterator
    it =
      gGlobalMsrOahGroup->getPartsRenamingMap ().find (fPartMsrName);

  if (it != gGlobalMsrOahGroup->getPartsRenamingMap ().end ()) {
    // yes, rename the part accordinglingly
    fPartMsrName = (*it).second;

#ifdef TRACING_IS_ENABLED
    if (gGlobalTraceOahGroup->getTraceParts ()) {
      gLogStream <<
        "Setting part name of " << getPartCombinedName () <<
        " to \"" << fPartMsrName << "\"" <<
         endl;
    }
#endif
  }
  else {
    // use the argument
    fPartMsrName = partMsrName;

#ifdef TRACING_IS_ENABLED
    if (gGlobalTraceOahGroup->getTraceParts ()) {
      gLogStream <<
        "Keeping partID \"" << partMsrName <<
        "\" as part name  for " << getPartCombinedName () <<
      endl;
    }
#endif
  }
}

string msrPart::getPartCombinedName () const
{
  stringstream s;

  if (! fPartMsrName.size ()) {
    s <<
      "[empty name]";
  }
  else {
    s <<
     // JMI "\"" <<
      fPartMsrName
     // JMI << "\""
      ;
  }

  s <<
    " (partID \"" << fPartID << "\"";

  if (fPartName.size ()) {
    s <<
      ", partName \"" << fPartName << "\"";
  }

  s <<
    ")";

  return s.str ();
}

void msrPart::createMeasureAndAppendItToPart (
  int    inputLineNumber,
  string measureNumber,
  msrMeasureImplicitKind
         measureImplicitKind)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceMeasures ()) {
    gLogStream <<
      "Creating measure '" <<
      measureNumber <<
      "' and appending it to part " <<
      getPartCombinedName () <<
      "', line " << inputLineNumber <<
      endl;
  }
#endif

  gIndenter++;

  // set part current measure number
  fPartCurrentMeasureNumber = measureNumber;

  // create and append measure to part-level staves if any
  if (fPartHarmoniesStaff) {
    fPartHarmoniesStaff->
      createMeasureAndAppendItToStaff (
        inputLineNumber,
        measureNumber,
        measureImplicitKind);
  }
  if (fPartFiguredBassStaff) {
    fPartFiguredBassStaff->
      createMeasureAndAppendItToStaff (
        inputLineNumber,
        measureNumber,
        measureImplicitKind);
  }

  // create and append measure to registered staves
  for (
    map<int, S_msrStaff>::const_iterator i = fPartStavesMap.begin ();
    i != fPartStavesMap.end ();
    i++
  ) {
    S_msrStaff
      staff = (*i).second;

    staff->
      createMeasureAndAppendItToStaff (
        inputLineNumber,
        measureNumber,
        measureImplicitKind);
  } // for

  gIndenter--;
}

void msrPart::setNextMeasureNumberInPart (
  int    inputLineNumber,
  string nextMeasureNumber)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceMeasures ()) {
    gLogStream <<
      "Setting next measure number to '" <<
      nextMeasureNumber <<
      "' in part " <<
      getPartCombinedName () <<
      "', line " << inputLineNumber <<
      endl;
  }
#endif

  gIndenter++;

  // set next measure number in registered staves
  for (
    map<int, S_msrStaff>::const_iterator i = fPartStavesMap.begin ();
    i != fPartStavesMap.end ();
    i++
  ) {
    S_msrStaff
      staff = (*i).second;

    staff->
      setNextMeasureNumberInStaff (
        inputLineNumber,
        nextMeasureNumber);
  } // for

  gIndenter--;
}

void msrPart::setPartNumberOfMeasures (unsigned int partNumberOfMeasures)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceMeasures ()) {
    gLogStream <<
      "Setting the the number of measures in part " <<
      getPartCombinedName () <<
      " to " <<
      partNumberOfMeasures <<
    endl;
  }
#endif

  fPartNumberOfMeasures = partNumberOfMeasures;

  unsigned int
    fPartMeasuresWholeNotesDurationsVectorSize =
      fPartMeasuresWholeNotesDurationsVector.size ();

  if (partNumberOfMeasures > fPartMeasuresWholeNotesDurationsVectorSize) {
#ifdef TRACING_IS_ENABLED
    if (gGlobalTraceOahGroup->getTraceMeasures ()) {
      gLogStream <<
        "Resizing fPartMeasuresWholeNotesDurationsVector in part " << // JMI ???
        getPartCombinedName () <<
        " from " <<
        fPartMeasuresWholeNotesDurationsVectorSize <<
        " to " <<
        partNumberOfMeasures <<
        " measures" <<
        endl;
    }
#endif

    fPartMeasuresWholeNotesDurationsVector.clear ();
    fPartMeasuresWholeNotesDurationsVector.resize (
      fPartNumberOfMeasures,
      rational (0, 1));
  }
}

void msrPart::registerOrdinalMeasureNumberWholeNotesDuration (
  int      inputLineNumber,
  int      measureOrdinalNumber,
  rational wholeNotesDuration)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceMeasures ()) {
    gLogStream <<
      "Registering the whole notes duration of the measure with ordinal number '" <<
      measureOrdinalNumber <<
      "' as " <<
      wholeNotesDuration <<
      " in part " << getPartCombinedName () <<
      ", measureOrdinalNumber: " << measureOrdinalNumber <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

#ifdef TRACING_IS_ENABLED
  if (
    gGlobalTraceOahGroup->getTraceMeasures ()
      ||
    gGlobalTraceOahGroup->getTracePositionsInMeasures ()
  ) {
    gLogStream <<
      "===> fPartMeasuresWholeNotesDurationsVector contents: " <<
      endl;
    for (
      vector<rational>::const_iterator i =
        fPartMeasuresWholeNotesDurationsVector.begin ();
      i != fPartMeasuresWholeNotesDurationsVector.end ();
      i++
    ) {
      gIndenter++;
      gLogStream <<
        (*i) <<
        endl;
      gIndenter--;
    } // for
    gLogStream <<
      "<==== end of fPartMeasuresWholeNotesDurationsVector contents " <<
      endl;
  }
#endif

  unsigned int
    fPartMeasuresWholeNotesDurationsVectorSize =
      fPartMeasuresWholeNotesDurationsVector.size ();

  unsigned int index = measureOrdinalNumber - 1;

  // caution for first registration in the vector
  if (
    index >= 0
      &&
    index < fPartMeasuresWholeNotesDurationsVectorSize
  ) {
    rational
      currentValue =
        fPartMeasuresWholeNotesDurationsVector [index];

    if (currentValue.getNumerator () != 0) {
      if (currentValue != wholeNotesDuration) {
        // allow for polymetrics in non-MusicXML contexts? JMI
#ifdef TRACING_IS_ENABLED
        if (
          gGlobalTraceOahGroup->getTraceMeasures ()
            ||
          gGlobalTraceOahGroup->getTracePositionsInMeasures ()
        ) {
          gLogStream <<
            "The measure with ordinal number " <<
            measureOrdinalNumber <<
            " was known with a whole notes duration of " <<
            currentValue <<
            ", now registering it with a duration of " <<
            wholeNotesDuration <<
            "in part " << getPartCombinedName () <<
            endl;
        }
#endif
        }
        // else it's OK JMI ???
      }
    else {
      fPartMeasuresWholeNotesDurationsVector [index] =
        wholeNotesDuration;
    }
  }
}

void msrPart::appendStaffDetailsToPart (
  S_msrStaffDetails staffDetails)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceStaffDetails ()) {
    gLogStream <<
      "Appending staff details\"" <<
      staffDetails->asShortString () <<
      "\" to part " << getPartCombinedName () <<
    endl;
  }
#endif

  // register staff details in part
  fCurrentPartStaffDetails = staffDetails;

  // append staff details to registered staves
  for (
    map<int, S_msrStaff>::const_iterator i = fPartStavesMap.begin ();
    i != fPartStavesMap.end ();
    i++
  ) {
    S_msrStaff
      staff = (*i).second;

    staff->
      appendStaffDetailsToStaff (
        staffDetails);
  } // for
}

void msrPart::appendClefToPart (S_msrClef clef)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceClefs ()) {
    gLogStream <<
      "Appending clef '" <<
      clef->asString () <<
      "' to part " << getPartCombinedName () <<
    endl;
  }
#endif

  gIndenter++;

  // append clef to part-level staves if any
  if (fPartHarmoniesStaff) {
    fPartHarmoniesStaff->
      appendClefToStaff (clef);
  }
  if (fPartFiguredBassStaff) {
    fPartFiguredBassStaff->
      appendClefToStaff (clef);
  }

  // append clef to registered staves
  for (
    map<int, S_msrStaff>::const_iterator i = fPartStavesMap.begin ();
    i != fPartStavesMap.end ();
    i++
  ) {
    (*i).second->
      appendClefToStaff (clef);
  } // for

  gIndenter--;
}

void msrPart::appendKeyToPart  (S_msrKey  key)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceKeys ()) {
    gLogStream <<
      "Appending key " <<
      key->asString () <<
      " to part " << getPartCombinedName () <<
      endl;
  }
#endif

  // set part key
  fPartCurrentKey = key;

  gIndenter++;

  // append key to part-level staves if any
  if (fPartHarmoniesStaff) {
    fPartHarmoniesStaff->
      appendKeyToStaff (key);
  }
  if (fPartFiguredBassStaff) {
    fPartFiguredBassStaff->
      appendKeyToStaff (key);
  }

  // append key to registered staves
  for (
    map<int, S_msrStaff>::const_iterator i = fPartStavesMap.begin ();
    i != fPartStavesMap.end ();
    i++
  ) {
    S_msrStaff
      staff = (*i).second;

    staff->
      appendKeyToStaff (key);
  } // for

  gIndenter--;
}

void msrPart::appendTimeToPart (S_msrTime time)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceTimes ()) {
    gLogStream <<
      "Appending time '" <<
      time->asString () <<
      "' to part " << getPartCombinedName () <<
      endl;
  }
#endif

  // set part time
  fPartCurrentTime = time;

  gIndenter++;

  // append time to part-level staves if any
  if (fPartHarmoniesStaff) {
    fPartHarmoniesStaff->
      appendTimeToStaff (time);
  }
  if (fPartFiguredBassStaff) {
    fPartFiguredBassStaff->
      appendTimeToStaff (time);
  }

  // append time to registered staves
  for (
    map<int, S_msrStaff>::const_iterator i = fPartStavesMap.begin ();
    i != fPartStavesMap.end ();
    i++
  ) {
    S_msrStaff
      staff = (*i).second;

    staff->
      appendTimeToStaff (time);
  } // for

  gIndenter--;
}

void msrPart::appendTimeToPartClone (S_msrTime time)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceTimes ()) {
    gLogStream <<
      "Appending time '" <<
      time->asString () <<
      "' to part clone " << getPartCombinedName () <<
      endl;
  }
#endif

  // set part time
  fPartCurrentTime = time;

  gIndenter++;

  // propagate it to all staves
  for (
    map<int, S_msrStaff>::const_iterator i = fPartStavesMap.begin ();
    i != fPartStavesMap.end ();
    i++
  ) {
    S_msrStaff
      staff = (*i).second;

    staff->
      appendTimeToStaffClone (time);
  } // for

  gIndenter--;
}

void msrPart::insertHiddenMeasureAndBarlineInPartClone (
  int      inputLineNumber,
  rational positionInMeasure)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceDalSegnos () || gGlobalTraceOahGroup->getTracePositionsInMeasures ()) {
    gLogStream <<
      "Inserting hidden measure and barline at position " <<
      positionInMeasure <<
      "' in part clone " << getPartCombinedName () <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  gIndenter++;

  // propagate it to all staves
  for (
    map<int, S_msrStaff>::const_iterator i = fPartStavesMap.begin ();
    i != fPartStavesMap.end ();
    i++
  ) {
    S_msrStaff
      staff = (*i).second;

    staff->
      insertHiddenMeasureAndBarlineInStaffClone (
        inputLineNumber,
        positionInMeasure);
  } // for

  gIndenter--;
}

void msrPart::appendTransposeToPart (
  S_msrTranspose transpose)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceTranspositions ()) {
    gLogStream <<
      "Appending transpose \"" <<
      transpose->asString () <<
      "\" to part " << getPartCombinedName () <<
    endl;
  }
#endif

  // set part current transpose
  fPartCurrentTranspose = transpose;

  // propagate it to all staves
  for (
    map<int, S_msrStaff>::const_iterator i = fPartStavesMap.begin ();
    i != fPartStavesMap.end ();
    i++
  ) {
    S_msrStaff
      staff = (*i).second;

    staff->
      appendTransposeToStaff (transpose);
  } // for
}

void msrPart::appendPartNameDisplayToPart (
  S_msrPartNameDisplay partNameDisplay)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceParts ()) {
    gLogStream <<
      "Appending part name display \"" <<
      partNameDisplay->asString () <<
      "\" to part " << getPartCombinedName () <<
    endl;
  }
#endif

  // set part current transpose
// JMI ???  fPartCurrentTranspose = partNameDisplay;

  // propagate it to all staves
  for (
    map<int, S_msrStaff>::const_iterator i = fPartStavesMap.begin ();
    i != fPartStavesMap.end ();
    i++
  ) {
    S_msrStaff
      staff = (*i).second;

    staff->
      appendPartNameDisplayToStaff (partNameDisplay);
  } // for
}

void msrPart::appendPartAbbreviationDisplayToPart (
  S_msrPartAbbreviationDisplay partAbbreviationDisplay)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceParts ()) {
    gLogStream <<
      "Appending part abbreviation display \"" <<
      partAbbreviationDisplay->asString () <<
      "\" to part " << getPartCombinedName () <<
    endl;
  }
#endif

  // set part current transpose
// JMI ???  fPartCurrentTranspose = partAbbreviationDisplay;

  // propagate it to all staves
  for (
    map<int, S_msrStaff>::const_iterator i = fPartStavesMap.begin ();
    i != fPartStavesMap.end ();
    i++
  ) {
    S_msrStaff
      staff = (*i).second;

    staff->
      appendPartAbbreviationDisplayToStaff (partAbbreviationDisplay);
  } // for
}

/* JMI
void msrPart::nestContentsIntoNewRepeatInPart (
  int inputLineNumber)
{
  for (
    map<int, S_msrStaff>::const_iterator i = fPartStavesMap.begin ();
    i != fPartStavesMap.end ();
    i++
  ) {
    (*i).second->
      nestContentsIntoNewRepeatInStaff (
        inputLineNumber);
  } // for
}
*/

void msrPart::handleRepeatStartInPart (
  int inputLineNumber)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceRepeats ()) {
    gLogStream <<
      "Handling a repeat start in part \"" <<
      getPartCombinedName () <<
      "\", line " << inputLineNumber <<
      endl;
  }
#endif

  gIndenter++;

  for (
    map<int, S_msrStaff>::const_iterator i = fPartStavesMap.begin ();
    i != fPartStavesMap.end ();
    i++
  ) {
    (*i).second->
      handleRepeatStartInStaff (
        inputLineNumber);
  } // for

  gIndenter--;
}

void msrPart::handleRepeatEndInPart (
  int    inputLineNumber,
  string measureNumber,
  int    repeatTimes)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceRepeats ()) {
    gLogStream <<
      "Handling a repeat end in part \"" <<
      getPartCombinedName () <<
      "\"" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  gIndenter++;

  for (
    map<int, S_msrStaff>::const_iterator i = fPartStavesMap.begin ();
    i != fPartStavesMap.end ();
    i++
  ) {
    (*i).second->
      handleRepeatEndInStaff (
        inputLineNumber,
        measureNumber,
        repeatTimes);
  } // for

  gIndenter--;
}

void msrPart::handleRepeatEndingStartInPart (
  int    inputLineNumber)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceRepeats ()) {
    gLogStream <<
      "Handling a repeat ending start in part \"" <<
      getPartCombinedName () <<
      "\"" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  gIndenter++;

  for (
    map<int, S_msrStaff>::const_iterator i = fPartStavesMap.begin ();
    i != fPartStavesMap.end ();
    i++
  ) {
    (*i).second->
      handleRepeatEndingStartInStaff (
        inputLineNumber);
  } // for

  gIndenter--;
}

void msrPart::handleRepeatEndingEndInPart (
  int       inputLineNumber,
  string    repeatEndingNumber, // may be "1, 2"
  msrRepeatEndingKind
            repeatEndingKind)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceRepeats ()) {
    gLogStream <<
      "Handling a " <<
      repeatEndingKindAsString (
        repeatEndingKind) <<
      " repeat ending end in part \"" <<
      getPartCombinedName () <<
      "\"" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  gIndenter++;

  for (
    map<int, S_msrStaff>::const_iterator i = fPartStavesMap.begin ();
    i != fPartStavesMap.end ();
    i++
  ) {
    (*i).second->
      handleRepeatEndingEndInStaff (
        inputLineNumber,
        repeatEndingNumber,
        repeatEndingKind);
  } // for

  gIndenter--;
}

/* JMI
void msrPart::finalizeRepeatEndInPart (
  int    inputLineNumber,
  string measureNumber,
  int    repeatTimes)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceRepeats ()) {
    gLogStream <<
      "Finalizing a repeat upon its end in part \"" <<
      getPartCombinedName () <<
      "\"" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  gIndenter++;

  // create repeat and append it to registered staves
  for (
    map<int, S_msrStaff>::const_iterator i = fPartStavesMap.begin ();
    i != fPartStavesMap.end ();
    i++
  ) {
    (*i).second->
      finalizeRepeatEndInStaff (
        inputLineNumber,
        measureNumber,
        repeatTimes);
  } // for

  gIndenter--;
}
*/

void msrPart::appendRepeatCloneToPart (
  int         inputLineNumber,
  S_msrRepeat repeatCLone)
{
  for (
    map<int, S_msrStaff>::const_iterator i = fPartStavesMap.begin ();
    i != fPartStavesMap.end ();
    i++
  ) {
    S_msrStaff
      staff =
        (*i).second;

    switch (staff->getStaffKind ()) {
      case kStaffRegular:
        staff->appendRepeatCloneToStaff (
          inputLineNumber, repeatCLone);
        break;

      case kStaffTablature:
        break;

      case kStaffHarmony:
        break;

      case kStaffFiguredBass:
        break;

      case kStaffDrum:
        staff->appendRepeatCloneToStaff (
          inputLineNumber, repeatCLone);
        break;

      case kStaffRythmic:
        staff->appendRepeatCloneToStaff (
          inputLineNumber, repeatCLone);
        break;
    } // switch
  } // for
}

void msrPart::appendRepeatEndingCloneToPart (
  S_msrRepeatEnding repeatEndingCLone)
{
  for (
    map<int, S_msrStaff>::const_iterator i = fPartStavesMap.begin ();
    i != fPartStavesMap.end ();
    i++
  ) {
    (*i).second->
      appendRepeatEndingCloneToStaff (repeatEndingCLone);
  } // for
}

void msrPart::createMeasuresRepeatFromItsFirstMeasuresInPart (
  int inputLineNumber,
  int measuresRepeatMeasuresNumber,
  int measuresRepeatSlashes)
{
  // create measures repeat from its first measure in registered staves
  for (
    map<int, S_msrStaff>::const_iterator i = fPartStavesMap.begin ();
    i != fPartStavesMap.end ();
    i++
  ) {
    (*i).second->
      createMeasuresRepeatFromItsFirstMeasuresInStaff (
        inputLineNumber,
        measuresRepeatMeasuresNumber,
        measuresRepeatSlashes);
  } // for
}

void msrPart::appendPendingMeasuresRepeatToPart (
  int inputLineNumber)
{
  // append pending measures repeat to registered staves
  for (
    map<int, S_msrStaff>::const_iterator i = fPartStavesMap.begin ();
    i != fPartStavesMap.end ();
    i++
  ) {
    (*i).second->
      appendPendingMeasuresRepeatToStaff (
        inputLineNumber);
  } // for
}

void msrPart::createRestMeasuresInPart (
  int inputLineNumber,
  int multipleRestMeasuresMeasuresNumber)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceRestMeasures ()) {
    gLogStream <<
      "Creating multiple rest measures in part " <<
      getPartCombinedName () <<
      ", " <<
      singularOrPlural (
        multipleRestMeasuresMeasuresNumber, "measure", "measures") <<
      endl;
  }
#endif

  fPartContainsRestMeasures = true;

  // create multiple rest in registered staves
  for (
    map<int, S_msrStaff>::const_iterator i = fPartStavesMap.begin ();
    i != fPartStavesMap.end ();
    i++
  ) {
    (*i).second->
      createRestMeasuresInStaff (
        inputLineNumber,
        multipleRestMeasuresMeasuresNumber);
  } // for
}

void msrPart::addRestMeasuresToPart (
  int    inputLineNumber,
  string previousMeasureNumber,
  int    restMeasuresNumber)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceRestMeasures ()) {
    gLogStream <<
      "Creating multiple rest measures in part " <<
      getPartCombinedName () <<
      ", " <<
      singularOrPlural (
        restMeasuresNumber, "measure", "measures") <<
      endl;
  }
#endif

  fPartContainsRestMeasures = true;

  // add multiple rest to registered staves
  for (
    map<int, S_msrStaff>::const_iterator i = fPartStavesMap.begin ();
    i != fPartStavesMap.end ();
    i++
  ) {
    (*i).second->
      addRestMeasuresToStaff (
        inputLineNumber,
        previousMeasureNumber,
        restMeasuresNumber);
  } // for
}

void msrPart::appendPendingRestMeasuresToPart (
  int inputLineNumber)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceRestMeasures ()) {
    gLogStream <<
      "Appending the pending multiple rest to part " <<
      getPartCombinedName () <<
      endl;
  }
#endif

  // append pending multiple rest to registered staves
  for (
    map<int, S_msrStaff>::const_iterator i = fPartStavesMap.begin ();
    i != fPartStavesMap.end ();
    i++
  ) {
    (*i).second->
      appendPendingRestMeasuresToStaff (
        inputLineNumber);
  } // for
}

void msrPart::appendRestMeasuresCloneToPart (
  int               inputLineNumber,
  S_msrRestMeasures multipleRestMeasures)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceRestMeasures ()) {
    gLogStream <<
      "Appending multiple rest '" <<
      multipleRestMeasures->asString () <<
      "' to part clone " <<
      getPartCombinedName () <<
      endl;
  }
#endif

  for (
    map<int, S_msrStaff>::const_iterator i = fPartStavesMap.begin ();
    i != fPartStavesMap.end ();
    i++
  ) {
    (*i).second->
      appendRestMeasuresCloneToStaff (
        inputLineNumber,
        multipleRestMeasures);
  } // for
}

void msrPart::appendBarlineToPart (S_msrBarline barline)
{
  // append barline to registered staves
  for (
    map<int, S_msrStaff>::const_iterator i = fPartStavesMap.begin ();
    i != fPartStavesMap.end ();
    i++
  ) {
    (*i).second->
      appendBarlineToStaff (barline);
  } // for
}

S_msrStaff msrPart::addStaffToPartByItsNumber (
  int          inputLineNumber,
  msrStaffKind staffKind,
  int          staffNumber)
{
  if (fPartStavesMap.count (staffNumber)) {
    stringstream s;

    s <<
      "staffNumber " << staffNumber <<
      " already exists in part " << getPartCombinedName () <<
      ", line " << inputLineNumber;

    msrInternalError ( // JMI ???
      gGlobalOahOahGroup->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());

    return fPartStavesMap [staffNumber];
  }

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceStaves ()) {
    gLogStream <<
      "Adding " <<
      staffKindAsString (staffKind) <<
      " staff " << staffNumber <<
      " to part " << getPartCombinedName () <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  // create the staff
  S_msrStaff
    staff =
      msrStaff::create (
        inputLineNumber,
        staffKind,
        staffNumber,
        this);

  // register staff in this part if relevant
  switch (staffKind) {
    case kStaffRegular:
    case kStaffTablature:
    case kStaffHarmony:
    case kStaffFiguredBass:
    case kStaffDrum:
    case kStaffRythmic:
      fPartStavesMap [staffNumber] = staff;
      break;
  } // switch

  // initialize staff current clef and key if relevant // JMI
  switch (staffKind) {
    case kStaffRegular:
    case kStaffTablature:
    case kStaffHarmony:
    case kStaffFiguredBass:
    case kStaffDrum:
    case kStaffRythmic:
      staff->setStaffCurrentClef (fPartCurrentClef);
      staff->setStaffCurrentKey (fPartCurrentKey);
      break;
  } // switch

  return staff;
}

S_msrStaff msrPart::addPartLevelStaffToPart (
  int          inputLineNumber,
  msrStaffKind staffKind,
  int          staffNumber)
{
/* JMI
  if (fPartStavesMap.count (staffNumber)) {
    stringstream s;

    s <<
      "staffNumber " << staffNumber <<
      " already exists in part " << getPartCombinedName () <<
      ", line " << inputLineNumber;

    msrInternalError ( // JMI ???
      gGlobalOahOahGroup->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());

    return fPartStavesMap [staffNumber];
  }
*/

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceStaves ()) {
    gLogStream <<
      "Adding " <<
      staffKindAsString (staffKind) <<
      " staff " << staffNumber <<
      " to part " << getPartCombinedName () <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  // create the staff
  S_msrStaff
    staff =
      msrStaff::create (
        inputLineNumber,
        staffKind,
        staffNumber,
        this);

/* JMI
  // register staff in this part if relevant
  switch (staffKind) {
    case kStaffRegular:
    case kStaffTablature:
    case kStaffHarmony:
    case kStaffFiguredBass:
    case kStaffDrum:
    case kStaffRythmic:
      fPartStavesMap [staffNumber] = staff;
      break;
  } // switch

  // initialize staff current clef and key if relevant // JMI
  switch (staffKind) {
    case kStaffRegular:
    case kStaffTablature:
    case kStaffHarmony:
    case kStaffFiguredBass:
    case kStaffDrum:
    case kStaffRythmic:
      staff->setStaffCurrentClef (fPartCurrentClef);
      staff->setStaffCurrentKey (fPartCurrentKey);
      break;
  } // switch
*/

  return staff;
}

void msrPart::addStaffToPartCloneByItsNumber (S_msrStaff staff)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceStaves ()) {
    gLogStream <<
      "Adding staff \"" << staff->getStaffName () <<
      "\" to part clone " << getPartCombinedName () <<
      endl;
  }
#endif

  // register staff in this part
  fPartStavesMap [staff->getStaffNumber ()] = staff;
}

S_msrStaff msrPart::fetchStaffFromPart (
  int staffNumber)
{
  S_msrStaff result;

  if (fPartStavesMap.count (staffNumber)) {
    result = fPartStavesMap [staffNumber];
  }

  return result;
}

void msrPart::registerVoiceInPartAllVoicesList (
  S_msrVoice voice)
{
  // register voice in this staff
  fPartAllVoicesList.push_back (voice);

  // register it in the partgroup uplink
  fPartPartGroupUpLink->
    registerVoiceInPartGroupAllVoicesList (voice);
}

S_msrVoice msrPart::createPartHarmonyVoice (
  int    inputLineNumber,
  string currentMeasureNumber)
{
  if (fPartHarmoniesVoice) {
    stringstream s;

    s <<
      "Part \"" <<
      getPartCombinedName () <<
      "\" already has a harmony voice";

    msrInternalError (
      gGlobalOahOahGroup->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  }

  // create the part harmonies staff
  int partHarmonyStaffNumber =
    K_PART_HARMONY_STAFF_NUMBER;

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceHarmonies ()) {
    gLogStream <<
      "Creating harmony staff for part \"" <<
      getPartCombinedName () <<
      "\" with staff number " <<
      partHarmonyStaffNumber <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  fPartHarmoniesStaff =
    addPartLevelStaffToPart (
      inputLineNumber,
      kStaffHarmony,
      partHarmonyStaffNumber);

  // create the part harmony voice
  int partHarmonyVoiceNumber =
    K_PART_HARMONY_VOICE_NUMBER;

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceHarmonies ()) {
    gLogStream <<
      "Creating harmony voice for part \"" <<
      getPartCombinedName () <<
      "\" with voice number " <<
      partHarmonyVoiceNumber <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  fPartHarmoniesVoice =
    msrVoice::create (
      inputLineNumber,
      kVoiceHarmony,
      partHarmonyVoiceNumber,
      msrVoice::kCreateInitialLastSegmentYes,
      fPartHarmoniesStaff);

  // register it in the staff
  fPartHarmoniesStaff->
    registerVoiceInStaff (
      inputLineNumber,
      fPartHarmoniesVoice);

/* JMI
  // set backward link
  fPartHarmoniesVoice->
    setHarmonyVoicePartBackwardLink (this);
*/

  return fPartHarmoniesVoice;
}

void msrPart::appendHarmonyToPart (
  S_msrVoice   harmonySupplierVoice,
  S_msrHarmony harmony)
{
  int inputLineNumber =
    harmony->getInputLineNumber ();

  switch (harmonySupplierVoice->getVoiceKind ()) {
    case kVoiceRegular:
      // append the figured bass to the part figured bass voice
#ifdef TRACING_IS_ENABLED
      if (gGlobalTraceOahGroup->getTraceHarmonies ()) {
        gLogStream <<
          "Appending figured bass " <<
          harmony->asString () <<
          " to part " <<
          getPartCombinedName () <<
          ", line " << inputLineNumber <<
          endl;
      }
#endif

      fPartHarmoniesVoice->
        appendHarmonyToVoice (harmony);
      break;

    case kVoiceHarmony:
    case kVoiceFiguredBass:
      {
        stringstream s;

        s <<
          "figured bass cannot by supplied to part by " <<
          voiceKindAsString (
            harmonySupplierVoice->getVoiceKind ()) <<
          " voice \" " <<
          harmonySupplierVoice->getVoiceName () <<
          "\"";

        msrInternalError (
          gGlobalOahOahGroup->getInputSourceName (),
          inputLineNumber,
          __FILE__, __LINE__,
          s.str ());
      }
      break;
  } // switch
}

void msrPart::appendHarmonyToPartClone (
  S_msrVoice   harmonySupplierVoice,
  S_msrHarmony harmony)
{
  int inputLineNumber =
    harmony->getInputLineNumber ();

  switch (harmonySupplierVoice->getVoiceKind ()) {
    case kVoiceFiguredBass:
      // append the figured bass to the part figured bass voice
#ifdef TRACING_IS_ENABLED
      if (gGlobalTraceOahGroup->getTraceHarmonies ()) {
        gLogStream <<
          "Appending figured bass " <<
          harmony->asString () <<
          " to part clone " <<
          getPartCombinedName () <<
          ", line " << inputLineNumber <<
          endl;
      }
#endif

      fPartHarmoniesVoice->
        appendHarmonyToVoiceClone (harmony);
      break;

    case kVoiceRegular:
    case kVoiceHarmony:
      {
        stringstream s;

        s <<
          "figured bass cannot by supplied to part clone by " <<
          voiceKindAsString (
            harmonySupplierVoice->getVoiceKind ()) <<
          " voice \" " <<
          harmonySupplierVoice->getVoiceName () <<
          "\"";

        msrInternalError (
          gGlobalOahOahGroup->getInputSourceName (),
          inputLineNumber,
          __FILE__, __LINE__,
          s.str ());
      }
      break;
  } // switch
}

S_msrVoice msrPart::createPartFiguredBassVoice (
  int    inputLineNumber,
  string currentMeasureNumber)
{
  if (fPartFiguredBassVoice) {
    stringstream s;

    s <<
      "Part \"" <<
      getPartCombinedName () <<
      "\" already has a figured bass voice";

    msrInternalError (
      gGlobalOahOahGroup->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  }

  // create the part fibured bass staff
  int partFiguredBassStaffNumber =
    K_PART_FIGURED_BASS_STAFF_NUMBER;

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceHarmonies ()) {
    gLogStream <<
      "Creating figured bass staff for part \"" <<
      getPartCombinedName () <<
      "\" with staff number " <<
      partFiguredBassStaffNumber <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  fPartFiguredBassStaff =
    addPartLevelStaffToPart (
      inputLineNumber,
      kStaffFiguredBass,
      partFiguredBassStaffNumber);

  // create the voice figured bass voice
  int partFiguredBassVoiceNumber =
    K_PART_FIGURED_BASS_VOICE_NUMBER;

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceFiguredBasses ()) {
    gLogStream <<
      "Creating figured bass voice for part \"" <<
      getPartCombinedName () <<
      "\" with voice number " <<
      partFiguredBassVoiceNumber <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  fPartFiguredBassVoice =
    msrVoice::create (
      inputLineNumber,
      kVoiceFiguredBass,
      partFiguredBassVoiceNumber,
      msrVoice::kCreateInitialLastSegmentYes,
      fPartFiguredBassStaff);

  // register it in the staff
  fPartFiguredBassStaff->
    registerVoiceInStaff (
      inputLineNumber,
      fPartFiguredBassVoice);

/* JMI
  // set backward link
  fPartFiguredBassVoice->
    setFiguredBassVoicePartBackwardLink (this);
*/

  return fPartFiguredBassVoice;
}

void msrPart::appendFiguredBassToPart (
  S_msrVoice       figuredBassSupplierVoice,
  S_msrFiguredBass figuredBass)
{
  int inputLineNumber =
    figuredBass->getInputLineNumber ();

  switch (figuredBassSupplierVoice->getVoiceKind ()) {
    case kVoiceRegular:
      // append the figured bass to the part figured bass voice
#ifdef TRACING_IS_ENABLED
      if (gGlobalTraceOahGroup->getTraceFiguredBasses ()) {
        gLogStream <<
          "Appending figured bass " <<
          figuredBass->asString () <<
          " to part " <<
          getPartCombinedName () <<
          ", line " << inputLineNumber <<
          endl;
      }
#endif

      fPartFiguredBassVoice->
        appendFiguredBassToVoice (figuredBass);
      break;

    case kVoiceHarmony:
    case kVoiceFiguredBass:
      {
        stringstream s;

        s <<
          "figured bass cannot by supplied to part by " <<
          voiceKindAsString (
            figuredBassSupplierVoice->getVoiceKind ()) <<
          " voice \" " <<
          figuredBassSupplierVoice->getVoiceName () <<
          "\"";

        msrInternalError (
          gGlobalOahOahGroup->getInputSourceName (),
          inputLineNumber,
          __FILE__, __LINE__,
          s.str ());
      }
      break;
  } // switch
}

void msrPart::appendFiguredBassToPartClone (
  S_msrVoice       figuredBassSupplierVoice,
  S_msrFiguredBass figuredBass)
{
  int inputLineNumber =
    figuredBass->getInputLineNumber ();

  switch (figuredBassSupplierVoice->getVoiceKind ()) {
    case kVoiceFiguredBass:
      // append the figured bass to the part figured bass voice
#ifdef TRACING_IS_ENABLED
      if (gGlobalTraceOahGroup->getTraceFiguredBasses ()) {
        gLogStream <<
          "Appending figured bass " <<
          figuredBass->asString () <<
          " to part clone " <<
          getPartCombinedName () <<
          ", line " << inputLineNumber <<
          endl;
      }
#endif

      fPartFiguredBassVoice->
        appendFiguredBassToVoiceClone (figuredBass);
      break;

    case kVoiceRegular:
    case kVoiceHarmony:
      {
        stringstream s;

        s <<
          "figured bass cannot by supplied to part clone by " <<
          voiceKindAsString (
            figuredBassSupplierVoice->getVoiceKind ()) <<
          " voice \" " <<
          figuredBassSupplierVoice->getVoiceName () <<
          "\"";

        msrInternalError (
          gGlobalOahOahGroup->getInputSourceName (),
          inputLineNumber,
          __FILE__, __LINE__,
          s.str ());
      }
      break;
  } // switch
}

void msrPart::appendScordaturaToPart (
  S_msrScordatura scordatura)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceParts ()) {
    gLogStream <<
      "Appending scordatura '" <<
      scordatura->asString () <<
      "' to part " <<
      getPartCombinedName () <<
      endl;
  }
#endif

  for (
    map<int, S_msrStaff>::const_iterator i = fPartStavesMap.begin ();
    i != fPartStavesMap.end ();
    i++
  ) {
    (*i).second->
      appendScordaturaToStaff (
        scordatura);
  } // for
}

void msrPart::appendAccordionRegistrationToPart (
  S_msrAccordionRegistration
    accordionRegistration)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceParts ()) {
    gLogStream <<
      "Appending accordion registration '" <<
      accordionRegistration->asString () <<
      "' to part " <<
      getPartCombinedName () <<
      endl;
  }
#endif

  for (
    map<int, S_msrStaff>::const_iterator i = fPartStavesMap.begin ();
    i != fPartStavesMap.end ();
    i++
  ) {
    (*i).second->
      appendAccordionRegistrationToStaff (
        accordionRegistration);
  } // for
}

void msrPart::appendHarpPedalsTuningToPart (
  S_msrHarpPedalsTuning
    harpPedalsTuning)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceParts ()) {
    gLogStream <<
      "Appending harp pedals tuning '" <<
      harpPedalsTuning->asString () <<
      "' to part " <<
      getPartCombinedName () <<
      endl;
  }
#endif

  for (
    map<int, S_msrStaff>::const_iterator i = fPartStavesMap.begin ();
    i != fPartStavesMap.end ();
    i++
  ) {
    (*i).second->
      appendHarpPedalsTuningToStaff (
        harpPedalsTuning);
  } // for
}

void msrPart::addSkipGraceNotesGroupBeforeAheadOfVoicesClonesIfNeeded (
  S_msrVoice           graceNotesGroupOriginVoice,
  S_msrGraceNotesGroup skipGraceNotesGroup)
{
#ifdef TRACING_IS_ENABLED
  int inputLineNumber =
    skipGraceNotesGroup->getInputLineNumber ();
#endif

#ifdef TRACING_IS_ENABLED
  if (
    gGlobalTraceOahGroup->getTraceMeasures ()
      ||
    gGlobalTraceOahGroup->getTraceGraceNotes ()
      ||
    gGlobalTraceOahGroup->getTraceParts ()
    ) {
    gLogStream <<
      "addSkipGraceNotesGroupBeforeAheadOfVoicesClonesIfNeeded () in " <<
      getPartCombinedName () <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  for (
    map<int, S_msrStaff>::const_iterator i=fPartStavesMap.begin ();
    i!=fPartStavesMap.end ();
    i++
  ) {

    map<int, S_msrVoice>
      staffAllVoicesMap =
        (*i).second->
          getStaffVoiceNumberToAnyVoiceMap ();

    for (
      map<int, S_msrVoice>::const_iterator j=staffAllVoicesMap.begin ();
      j!=staffAllVoicesMap.end ();
      j++
    ) {
      S_msrVoice voice = (*j).second;

      if (voice != graceNotesGroupOriginVoice) {
        // add skip grace notes group ahead of voice
        voice->
          addGraceNotesGroupBeforeAheadOfVoiceIfNeeded (
            skipGraceNotesGroup);
      }
    } // for
  } // for
}

void msrPart::handleBackupInPart (
  int      inputLineNumber,
  rational backupStepLength)
{
  // account for backup in part
  decrementPartCurrentPositionInMeasure (
    inputLineNumber,
    backupStepLength);
}

void msrPart::finalizeLastAppendedMeasureInPart (
  int    inputLineNumber)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceMeasures ()) {
    gLogStream <<
      "Finalizing last appended measure in part " <<
      getPartCombinedName () <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  gIndenter++;

  // reset current position in measure
  fPartCurrentPositionInMeasure = rational (0,0);

  // finalize current measure in registered staves
  for (
    map<int, S_msrStaff>::const_iterator i = fPartStavesMap.begin ();
    i != fPartStavesMap.end ();
    i++
  ) {
    (*i).second->
      finalizeLastAppendedMeasureInStaff (
        inputLineNumber);
  } // for

  // finilize current measure in part-level staves if any
  if (fPartHarmoniesStaff) {
    fPartHarmoniesStaff->
      finalizeLastAppendedMeasureInStaff (
        inputLineNumber);
  }
  if (fPartFiguredBassStaff) {
    fPartFiguredBassStaff->
      finalizeLastAppendedMeasureInStaff (
        inputLineNumber);
  }

  gIndenter--;
}

void msrPart::setPartInstrumentNamesMaxLengthes ()
{
  S_msrScore
    score =
      fPartPartGroupUpLink->
        getPartGroupScoreUpLink ();

  int partInstrumentNameLength =
    fPartInstrumentName.size ();

  if (
    partInstrumentNameLength
      >
    score->getScoreInstrumentNamesMaxLength ()) {
    score->
      setScoreInstrumentNamesMaxLength (
        partInstrumentNameLength);
  }

  int partInstrumentAbbreviationLength =
    fPartInstrumentAbbreviation.size ();

  if (
    partInstrumentAbbreviationLength
      >
    score->getScoreInstrumentAbbreviationsMaxLength ()) {
    score->
      setScoreInstrumentAbbreviationsMaxLength (
        partInstrumentAbbreviationLength);
  }
}

void msrPart::finalizePart (
  int inputLineNumber)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceParts ()) {
    gLogStream <<
      "Finalizing part " <<
      getPartCombinedName () <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  gIndenter++;

  if (! fPartStavesMap.size ()) {
    stringstream s;

    s <<
      "Part " <<
      getPartCombinedName () <<
      " appears in the part list, but doesn't contain any stave";

    msrMusicXMLWarning (
      gGlobalOahOahGroup->getInputSourceName (),
      inputLineNumber,
      s.str ());
  }

  else {
    // finalize registered staves
    for (
      map<int, S_msrStaff>::const_iterator i = fPartStavesMap.begin ();
      i != fPartStavesMap.end ();
      i++
  ) {
      (*i).second->
        finalizeStaff (
          inputLineNumber);
    } // for
  }

  // set score instrument names max lengthes if relevant
  setPartInstrumentNamesMaxLengthes ();

  gIndenter--;
}

void msrPart::finalizePartClone (
  int inputLineNumber)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceParts ()) {
    gLogStream <<
      "Finalizing part clone " <<
      getPartCombinedName () <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  // set score instrument names max lengthes if relevant
  setPartInstrumentNamesMaxLengthes ();
}

void msrPart::acceptIn (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrPart::acceptIn ()" <<
      endl;
  }

  if (visitor<S_msrPart>*
    p =
      dynamic_cast<visitor<S_msrPart>*> (v)) {
        S_msrPart elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrPart::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrPart::acceptOut (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrPart::acceptOut ()" <<
      endl;
  }

  if (visitor<S_msrPart>*
    p =
      dynamic_cast<visitor<S_msrPart>*> (v)) {
        S_msrPart elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrPart::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrPart::browseData (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrPart::browseData ()" <<
      endl;
  }

  // browse the part harmonies staff if any right now,
  // to place it after the corresponding part
  if (fPartHarmoniesStaff) {
    msrBrowser<msrStaff> browser (v);
    browser.browse (*fPartHarmoniesStaff);
  }

  // browse all non harmonies nor figured bass staves
  for (
    map<int, S_msrStaff>::const_iterator i = fPartStavesMap.begin ();
    i != fPartStavesMap.end ();
    i++
  ) {
    S_msrStaff
      staff =
        (*i).second;

    if (staff != fPartHarmoniesStaff && staff != fPartFiguredBassStaff) {
      // browse the staff
      msrBrowser<msrStaff> browser (v);
      browser.browse (*staff);
    }
  } // for

  // browse the part figured bass staff if any only now,
  // to place it after the corresponding part
  if (fPartFiguredBassStaff) {
    msrBrowser<msrStaff> browser (v);
    browser.browse (*fPartFiguredBassStaff);
  }
}

string msrPart::asString () const
{
  stringstream s;

  s <<
    "Part" <<
    ", partID = \"" <<
    fPartID <<
    "\", partName = \"" <<
    fPartName <<
    "\", partMsrName: " << fPartMsrName <<
    fPartName <<
    "\", partStaves: " << fPartStavesMap.size () <<
    ", line " << fInputLineNumber;

  return s.str ();
}

void msrPart::printPartMeasuresWholeNotesDurationsVector (
  ostream&      os,
  unsigned int fieldWidth) const
{
  os << left <<
    setw (fieldWidth) <<
    "partMeasuresWholeNotesDurationsVector (ordinal number wise)" << " : " <<
    endl;

  gIndenter++;

  if (fPartNumberOfMeasures == 0) {
    os << "empty" << endl;
  }
  else {
    for (unsigned int i = 0; i < fPartNumberOfMeasures; ++i) {
      int j = i + 1;
      os << left <<
        "ordinal number " << setw (3) << right <<
        j << " : " <<
        setw (4) <<
        fPartMeasuresWholeNotesDurationsVector [ i ].toString ();
      if (j % 4 == 0) {
        os << endl;
      }
      else {
        os << "    ";
      }
    } // for
  }
  os << endl;

  gIndenter--;
}

void msrPart::print (ostream& os) const
{
  os <<
    "Part" << " " << fPartMsrName <<
    " (" <<
    singularOrPlural (
      fPartStavesMap.size (), "staff", "staves") <<
    ")" <<
    ", line " << fInputLineNumber <<
    endl;

  gIndenter++;

  const unsigned int fieldWidth = 29;

  os << left <<
    setw (fieldWidth) <<
    "partGroupUpLink" << " : ";
  if (fPartPartGroupUpLink) {
    // it may be empty
    os <<
      fPartPartGroupUpLink->getPartGroupCombinedName ();
  }
  else {
    os << "none";
  }
  os << endl;

  os << left <<
    setw (fieldWidth) <<
    "partID" << " : \"" <<
    fPartID << "\"" <<
    endl <<

    setw (fieldWidth) <<
    "partMsrName" << " : \"" <<
    fPartMsrName << "\"" <<
    endl <<

    setw (fieldWidth) <<
    "partAbsoluteNumber" << " : " <<
    fPartAbsoluteNumber <<
    endl <<

    setw (fieldWidth) <<
    "partName" << " : \"" <<
    fPartName << "\"" <<
    endl <<
    setw (fieldWidth) <<
    "partNameDisplayText" << " : \"" <<
    fPartNameDisplayText << "\"" <<
    endl <<

    setw (fieldWidth) <<
    "partAbbrevation" << " : \"" <<
    fPartAbbreviation << "\"" <<
    endl <<
    setw (fieldWidth) <<
    "partAbbreviationDisplayText" << " : \"" <<
    fPartAbbreviationDisplayText << "\"" <<
    endl <<

    setw (fieldWidth) <<
    "partInstrumentName" << " : \"" <<
    fPartInstrumentName << "\"" <<
    endl <<
    setw (fieldWidth) <<
    "partInstrumentAbbreviation" << " : \"" <<
    fPartInstrumentAbbreviation << "\"" <<
    endl <<

    setw (fieldWidth) <<
    "partNumberOfMeasures" << " : " <<
    fPartNumberOfMeasures <<
    endl <<

    setw (fieldWidth) <<
    "partContainsRestMeasures" << " : " <<
    booleanAsString (fPartContainsRestMeasures) <<
    endl <<

    setw (fieldWidth) <<
    "partCurrentPositionInMeasure" << " : " <<
    fPartCurrentPositionInMeasure <<
    endl;

  // print current the part clef if any
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceClefs ()) {
    os << left <<
      setw (fieldWidth) <<
      "partCurrentClef" << " : ";

    if (fPartCurrentClef) {
      os <<
        "'" <<
        fPartCurrentClef->asShortString () <<
        "'";
    }
    else {
      os <<
        "none";
    }

    os << endl;
  }
#endif

  // print the current part key if any
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceKeys ()) {
    os << left <<
      setw (fieldWidth) <<
      "partCurrentKey" << " : ";

    if (fPartCurrentKey) {
      os <<
        "'" <<
        fPartCurrentKey->asShortString () <<
        "'";
    }
    else {
      os <<
        "none";
    }

    os << endl;
  }
#endif

  // print the current part time if any
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceTimes ()) {
    os << left <<
      setw (fieldWidth) <<
      "partCurrentTime" << " : ";

    if (fPartCurrentTime) {
      os <<
        "'" <<
        fPartCurrentTime->asShortString () <<
        "'";
    }
    else {
      os << "none";
    }

    os << endl;
  }
#endif

  os << left <<
    setw (fieldWidth) << "partShortestNoteDuration" << " : " <<
    fPartShortestNoteDuration <<
    endl <<
    setw (fieldWidth) << "partShortestNoteTupletFactor" << " : " <<
    endl;

  gIndenter++;
  os <<
    fPartShortestNoteTupletFactor <<
    endl;
  gIndenter--;

  // print the part harmonies staff if any
  os << left <<
    setw (fieldWidth) <<
    "partHarmoniesStaff" << " : ";
  if (fPartHarmoniesStaff) {
    os << endl;

    gIndenter++;

    os << fPartHarmoniesStaff;

    gIndenter--;
  }
  else {
    os << "none";
  }
  os << endl;

  // print the part figured bass staff if any
  os << left <<
    setw (fieldWidth) <<
    "partFiguredBassStaff" << " : ";
  if (fPartFiguredBassStaff) {
    os << endl;

    gIndenter++;

    os << fPartFiguredBassStaff;

    gIndenter--;
  }
  else {
    os << "none";
  }
  os << endl;

  // print the part measure' whole notes durations
  printPartMeasuresWholeNotesDurationsVector (
    os,
    fieldWidth);

  // print all the voices if any
  unsigned int partAllVoicesListSize = fPartAllVoicesList.size ();

  os <<
    setw (fieldWidth) <<
    "PartAllVoicesList";
  if (partAllVoicesListSize) {
    os << endl;
    gIndenter++;

    list<S_msrVoice>::const_iterator
      iBegin = fPartAllVoicesList.begin (),
      iEnd   = fPartAllVoicesList.end (),
      i      = iBegin;
    for ( ; ; ) {
      S_msrVoice voice = (*i);

      os << voice->getVoiceName () << endl;
      if (++i == iEnd) break;
      // os << endl;
    } // for
    os << endl;

    gIndenter--;
  }
  else {
    os <<
      " : " << "none" <<
      endl;
  }

  // print the registered staves
  if (fPartStavesMap.size ()) {
    os << endl;

    map<int, S_msrStaff>::const_iterator
      iBegin = fPartStavesMap.begin (),
      iEnd   = fPartStavesMap.end (),
      i      = iBegin;

    for ( ; ; ) {
      S_msrStaff
        staff =
          (*i).second;

      // sanity check
      msgAssert (
        staff != nullptr,
        "staff is null");

      msrStaffKind
        staffKind =
          staff->getStaffKind ();

      switch (staffKind) { // JMI
        case kStaffRegular:
          os << staff;
          break;

        case kStaffTablature:
          os << staff;
          break;

        case kStaffHarmony:
    // JMI      if (gGlobalMsrOahGroup->getShowHarmonyVoices ()) {}
          os << staff;
          break;

        case kStaffFiguredBass:
    // JMI      if (gGlobalMsrOahGroup->getShowFiguredBassVoices ()) {}
          os << staff;
          break;

        case kStaffDrum:
          os << staff;
          break;

        case kStaffRythmic:
          os << staff;
          break;
      } // switch

      if (++i == iEnd) break;

      os << endl;
    } // for
  }

  gIndenter--;
}

void msrPart::printShort (ostream& os) const
{
  os <<
    "Part" << " " << fPartMsrName <<
    " (" <<
    singularOrPlural (
      fPartStavesMap.size (), "staff", "staves") <<
    ")" <<
    ", line " << fInputLineNumber <<
    endl;

  gIndenter++;

  const unsigned int fieldWidth = 29;

/*
  os << left <<
    setw (fieldWidth) <<
    "partGroupUpLink" << " : ";
  if (fPartPartGroupUpLink) {
    // it may be empty
    os <<
      fPartPartGroupUpLink->getPartGroupCombinedName ();
  }
  else {
    os << "none";
  }
  os << endl;
*/

  os << left <<
    setw (fieldWidth) <<
    "partID" << " : \"" <<
    fPartID << "\"" <<
    endl <<

    setw (fieldWidth) <<
    "partMsrName" << " : \"" <<
    fPartMsrName << "\"" <<
    endl <<

    setw (fieldWidth) <<
    "partAbsoluteNumber" << " : " <<
    fPartAbsoluteNumber <<
    endl <<

    setw (fieldWidth) <<
    "partName" << " : \"" <<
    fPartName << "\"" <<
    endl;

/*
  os << left <<
    setw (fieldWidth) <<
    "partNameDisplayText" << " : \"" <<
    fPartNameDisplayText << "\"" <<
    endl <<

    setw (fieldWidth) <<
    "partAbbrevation" << " : \"" <<
    fPartAbbreviation << "\"" <<
    endl <<
    setw (fieldWidth) <<
    "partAbbreviationDisplayText" << " : \"" <<
    fPartAbbreviationDisplayText << "\"" <<
    endl <<

    setw (fieldWidth) <<
    "partInstrumentName" << " : \"" <<
    fPartInstrumentName << "\"" <<
    endl <<
    setw (fieldWidth) <<
    "partInstrumentAbbreviation" << " : \"" <<
    fPartInstrumentAbbreviation << "\"" <<
    endl <<

    setw (fieldWidth) <<
    "partNumberOfMeasures" << " : " <<
    fPartNumberOfMeasures <<
    endl <<

    setw (fieldWidth) <<
    "partContainsRestMeasures" << " : " <<
    booleanAsString (fPartContainsRestMeasures) <<
    endl <<

    setw (fieldWidth) <<
    "partCurrentPositionInMeasure" << " : " <<
    fPartCurrentPositionInMeasure <<
    endl;

  // print current the part clef if any
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceClefs ()) {
    os << left <<
      setw (fieldWidth) <<
      "partCurrentClef" << " : ";

    if (fPartCurrentClef) {
      os <<
        "'" <<
        fPartCurrentClef->asShortString () <<
        "'";
    }
    else {
      os <<
        "none";
    }

    os << endl;
  }
#endif

  // print the current part key if any
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceKeys ()) {
    os << left <<
      setw (fieldWidth) <<
      "partCurrentKey" << " : ";

    if (fPartCurrentKey) {
      os <<
        "'" <<
        fPartCurrentKey->asShortString () <<
        "'";
    }
    else {
      os <<
        "none";
    }

    os << endl;
  }
#endif

  // print the current part time if any
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceTimes ()) {
    os << left <<
      setw (fieldWidth) <<
      "partCurrentTime" << " : ";

    if (fPartCurrentTime) {
      os <<
        "'" <<
        fPartCurrentTime->asShortString () <<
        "'";
    }
    else {
      os << "none";
    }

    os << endl;
  }
#endif

  os << left <<
    setw (fieldWidth) << "partShortestNoteDuration" << " : " <<
    fPartShortestNoteDuration <<
    endl <<
    setw (fieldWidth) << "partShortestNoteTupletFactor" << " : " <<
    endl;

  gIndenter++;
  os <<
    fPartShortestNoteTupletFactor <<
    endl;
  gIndenter--;

  // print the part harmonies staff if any
  os << left <<
    setw (fieldWidth) <<
    "partHarmoniesStaff" << " : ";
  if (fPartHarmoniesStaff) {
    os << endl;

    gIndenter++;
    fPartHarmoniesStaff->printShort (os);
    gIndenter--;
  }
  else {
    os << "none";
  }
  os << endl;

  // print the part figured bass staff if any
  os << left <<
    setw (fieldWidth) <<
    "partFiguredBassStaff" << " : ";
  if (fPartFiguredBassStaff) {
    os << endl;

    gIndenter++;
    fPartFiguredBassStaff->printShort (os);
    gIndenter--;
  }
  else {
    os << "none";
  }
  os << endl;

  // print the part measure' whole notes durations
  printPartMeasuresWholeNotesDurationsVector (
    os,
    fieldWidth);

  // print all the voices if any
  unsigned int partAllVoicesListSize = fPartAllVoicesList.size ();

  os <<
    setw (fieldWidth) <<
    "PartAllVoicesList";
  if (partAllVoicesListSize) {
    os << endl;
    gIndenter++;

    list<S_msrVoice>::const_iterator
      iBegin = fPartAllVoicesList.begin (),
      iEnd   = fPartAllVoicesList.end (),
      i      = iBegin;
    for ( ; ; ) {
      S_msrVoice voice = (*i);

      os << voice->getVoiceName () << endl;
      if (++i == iEnd) break;
      // os << endl;
    } // for
    os << endl;

    gIndenter--;
  }
  else {
    os <<
      " : " << "none" <<
      endl;
  }
*/

  // print the registered staves
  if (fPartStavesMap.size ()) {
    os << endl;

    map<int, S_msrStaff>::const_iterator
      iBegin = fPartStavesMap.begin (),
      iEnd   = fPartStavesMap.end (),
      i      = iBegin;

    for ( ; ; ) {
      S_msrStaff staff = (*i).second;

      // sanity check
      msgAssert (
        staff != nullptr,
        "staff is null");

      msrStaffKind
        staffKind =
          staff->getStaffKind ();

      switch (staffKind) { // JMI
        case kStaffRegular:
          staff->printShort (os);
          break;

        case kStaffTablature:
          staff->printShort (os);
          break;

        case kStaffHarmony:
    // JMI      if (gGlobalMsrOahGroup->getShowHarmonyVoices ()) {}
          staff->printShort (os);
          break;

        case kStaffFiguredBass:
    // JMI      if (gGlobalMsrOahGroup->getShowFiguredBassVoices ()) {}
          staff->printShort (os);
          break;

        case kStaffDrum:
          staff->printShort (os);
          break;

        case kStaffRythmic:
          staff->printShort (os);
          break;
      } // switch

      if (++i == iEnd) break;

      os << endl;
    } // for
  }

  gIndenter--;
}

void msrPart::printSummary (ostream& os) const
{
  os <<
    "Part" << " " << fPartMsrName <<
    " (" <<
    singularOrPlural (
      fPartStavesMap.size (), "staff", "staves") <<
    ", " <<
    singularOrPlural (
      fPartNumberOfMeasures, "measure", "measure") <<
    ")" <<
    endl;

  gIndenter++;

  const unsigned int fieldWidth = 27;

  os << left <<
    setw (fieldWidth) <<
    "partID" << " : \"" <<
    fPartID << "\"" <<
    endl <<

    setw (fieldWidth) <<
    "partMsrName" << " : \"" <<
    fPartMsrName << "\"" <<
    endl <<

    setw (fieldWidth) <<
    "partName" << " : \"" <<
    fPartName << "\"" <<
    endl <<

    setw (fieldWidth) <<
    "partAbsoluteNumber" << " : " <<
    fPartAbsoluteNumber <<
    endl <<

    setw (fieldWidth) <<
    "partNameDisplayText" << " : \"" <<
    fPartNameDisplayText << "\"" <<
    endl <<

    setw (fieldWidth) <<
    "partAbbrevation" << " : \"" <<
    fPartAbbreviation << "\"" <<
    endl <<
    setw (fieldWidth) <<
    "partAbbreviationDisplayText" << " : \"" <<
    fPartAbbreviationDisplayText << "\"" <<
    endl <<

    setw (fieldWidth) <<
    "partInstrumentName" << " : \"" <<
    fPartInstrumentName << "\"" <<
    endl <<
    setw (fieldWidth) <<
    "partInstrumentAbbreviation" << " : \"" <<
    fPartInstrumentAbbreviation << "\"" <<
    endl <<

    setw (fieldWidth) <<
    "partNumberOfMeasures" << " : " <<
    fPartNumberOfMeasures <<
    endl <<

    setw (fieldWidth) <<
    "partContainsRestMeasures" << " : " <<
    booleanAsString (fPartContainsRestMeasures) <<
    endl <<

    setw (fieldWidth) <<
    "partCurrentPositionInMeasure" << " : " <<
    fPartCurrentPositionInMeasure <<
    endl;

  // print the staves
  if (fPartStavesMap.size ()) {
    os <<
      "partStavesMap" <<
      endl;

    gIndenter++;

    map<int, S_msrStaff>::const_iterator
      iBegin = fPartStavesMap.begin (),
      iEnd   = fPartStavesMap.end (),
      i      = iBegin;

    for ( ; ; ) {
      (*i).second->printSummary (os);
      if (++i == iEnd) break;
      os << endl;
    } // for

    gIndenter--;
  }

  // print the harmonies staff if any
  if (fPartHarmoniesStaff) {
    os <<
      "partHarmoniesStaff" <<
      endl;

    gIndenter++;
    os <<
      fPartHarmoniesStaff;
    gIndenter--;
  }

  // print the figured bass staff if any
  if (fPartFiguredBassStaff) {
    os <<
      "partFiguredBassStaff" <<
      endl;

    gIndenter++;
    os <<
      fPartFiguredBassStaff;
    gIndenter--;
  }

  gIndenter--;
}

ostream& operator<< (ostream& os, const S_msrPart& elt)
{
  elt->print (os);
  return os;
}


}
