/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#include <iomanip>      // setw, setprecision, ...

#include "msrMutualDependencies.h"

#include "setTraceOahIfDesired.h"
#ifdef TRACE_OAH
  #include "traceOah.h"
#endif

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
  assert(o!=0);
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
  msrAssert(
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
#ifdef TRACE_OAH
  if (gTraceOah->fTraceParts) {
    gLogOstream <<
      "Creating part \"" << asString () << "\"" <<
      endl;

    gMsrOah->printMsrOahValues (40); // JMI
  }
#endif

  // is this part name in the part renaming map?
  map<string, string>::const_iterator
    it =
      gMsrOah->fPartsRenamingMap.find (fPartID);

  if (it != gMsrOah->fPartsRenamingMap.end ()) {
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

#ifdef TRACE_OAH
  if (gTraceOah->fTraceParts) {
    gLogOstream <<
      "Creating part \"" << asString () << "\"" <<
      endl;
  }
#endif
}

msrPart::~msrPart ()
{}

S_msrPart msrPart::createPartNewbornClone (S_msrPartGroup partGroupClone)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceParts) {
    gLogOstream <<
      "Creating a newborn clone of part " <<
      getPartCombinedName () <<
      endl;
  }
#endif

  // sanity check
  msrAssert(
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

  return newbornClone;
}

/* JMI
void msrPart::setPartInstrumentAbbreviation (
  string partInstrumentAbbreviation)
    {
      fPartInstrumentAbbreviation =
        partInstrumentAbbreviation;
    }
*/

/* JMI
void msrPart::createPartFiguredBassStaffAndVoiceIfNotYetDone (
  int inputLineNumber)
{
#ifdef TRACE_OAH
  if (
    gTraceOah->fTraceParts
      ||
    gTraceOah->fTraceFiguredBasses
      ||
    gTraceOah->fTraceStaves
      ||
    gTraceOah->fTraceVoices) {
    gLogOstream <<
      "Creating the figured bass staff" <<
      " with number " << K_PART_FIGURED_BASS_STAFF_NUMBER <<
      " and corredponding voice for part " <<
      getPartCombinedName () <<
      " if not yet done" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

#ifdef TRACE_OAH
  if (gTraceOah->fTraceHarmonies || gTraceOah->fTraceVoices) {
    gLogOstream <<
      endl <<
      "*********** createPartFiguredBassStaffAndVoiceIfNotYetDone() 1" <<
      endl <<
      endl;
    print (gLogOstream);
    gLogOstream <<
      "***********" <<
      endl <<
      endl;
  }
#endif

  if (! fPartFiguredBassStaff) {
    gIndenter++;

    // create the part figured bass staff
    fPartFiguredBassStaff =
      addStaffToPartByItsNumber (
        inputLineNumber,
        msrStaff::kStaffFiguredBass,
        K_PART_FIGURED_BASS_STAFF_NUMBER);

    // create the part figured bass voice
#ifdef TRACE_OAH
    if (gTraceOah->fTraceHarmonies || gTraceOah->fTraceVoices) {
      gLogOstream <<
        "Creating figured bass voice " <<
        " number " << K_PART_FIGURED_BASS_VOICE_NUMBER <<
        " for part " <<
        getPartCombinedName () <<
        ", line " << inputLineNumber <<
        endl;
    }
#endif

    // create the figured bass voice
    fPartFiguredBassVoice =
      msrVoice::create (
        inputLineNumber,
        msrVoice::kVoiceFiguredBass,
        K_PART_FIGURED_BASS_VOICE_NUMBER,
        msrVoice::kCreateInitialLastSegmentYes,
        fPartFiguredBassStaff);

    // register it in figured bass staff
    fPartFiguredBassStaff->
      registerVoiceInStaff (
        inputLineNumber,
        fPartFiguredBassVoice );

    if (fPartCurrentTime) {
      // append part current time to figured bass voice
      fPartFiguredBassVoice->
        appendTimeToVoice (
          fPartCurrentTime);
    }

    gIndenter--;
  }

#ifdef TRACE_OAH
  if (gTraceOah->fTraceHarmonies || gTraceOah->fTraceVoices) {
    gLogOstream <<
      endl <<
      "*********** createPartFiguredBassStaffAndVoiceIfNotYetDone() 2" <<
      endl <<
      endl;
    print (gLogOstream);
    gLogOstream <<
      "***********" <<
      endl <<
      endl;
  }
#endif
}
*/

void msrPart::setPartCurrentPositionInMeasure (
  int      inputLineNumber,
  rational positionInMeasure)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTracePositionsInMeasures) {
    gLogOstream <<
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
      gOahOah->fInputSourceName,
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

#ifdef TRACE_OAH
  if (gTraceOah->fTracePositionsInMeasures) {
    gLogOstream <<
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
#ifdef TRACE_OAH
  if (gTraceOah->fTracePositionsInMeasures) {
    gLogOstream <<
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
      gOahOah->fInputSourceName,
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  }
}

void msrPart::assignSequentialNumbersToRegularVoicesInPart (
  int inputLineNumber)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceParts || gTraceOah->fTraceVoices) {
    gLogOstream <<
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

void msrPart::setPartMsrName (string partMsrName)
{
  // is this part name in the part renaming map?
  map<string, string>::const_iterator
    it =
      gMsrOah->fPartsRenamingMap.find (fPartMsrName);

  if (it != gMsrOah->fPartsRenamingMap.end ()) {
    // yes, rename the part accordinglingly
    fPartMsrName = (*it).second;

#ifdef TRACE_OAH
    if (gTraceOah->fTraceParts) {
      gLogOstream <<
        "Setting part name of " << getPartCombinedName () <<
        " to \"" << fPartMsrName << "\"" <<
         endl;
    }
#endif
  }
  else {
    // use the argument
    fPartMsrName = partMsrName;

#ifdef TRACE_OAH
    if (gTraceOah->fTraceParts) {
      gLogOstream <<
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
  msrMeasure::msrMeasureImplicitKind
         measureImplicitKind)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceMeasures) {
    gLogOstream <<
      "Creating and appending measure '" <<
      measureNumber <<
      "' to part " <<
      getPartCombinedName () <<
      "', line " << inputLineNumber <<
      endl;
  }
#endif

  gIndenter++;

  // set part current measure number
  fPartCurrentMeasureNumber = measureNumber;

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
#ifdef TRACE_OAH
  if (gTraceOah->fTraceMeasures) {
    gLogOstream <<
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

/* JMI
void msrPart::complementPartVoicesUpToMeasure (
  int    inputLineNumber,
  string measureNumber)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceMeasures) {
    gLogOstream <<
      "Complement part voices up to measure number " << measureNumber <<
      ", line " << inputLineNumber <<
      ", in part " << getPartCombinedName () <<
      endl;
  }
#endif

  // propagate to all staves
  for (
    map<int, S_msrStaff>::const_iterator i = fPartStavesMap.begin ();
    i != fPartStavesMap.end ();
    i++
  ) {
    S_msrStaff
      staff = (*i).second;
      / * JMI
    staff->
      createMeasureAndAppendItToStaff (
        inputLineNumber,
        measureNumber,
        measureImplicitKind);
  } // for
}
*/

void msrPart::appendStaffDetailsToPart (
  S_msrStaffDetails staffDetails)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceStaffDetails) {
    gLogOstream <<
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
#ifdef TRACE_OAH
  if (gTraceOah->fTraceClefs) {
    gLogOstream <<
      "Appending clef '" <<
      clef->asString () <<
      "' to part " << getPartCombinedName () <<
    endl;
  }
#endif

  gIndenter++;

  // append clef to registered staves
  for (
    map<int, S_msrStaff>::const_iterator i = fPartStavesMap.begin ();
    i != fPartStavesMap.end ();
    i++
  ) {
    (*i).second->
      appendClefToStaff (
        clef);
  } // for

  gIndenter--;
}

void msrPart::appendKeyToPart  (S_msrKey  key)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceKeys) {
    gLogOstream <<
      "Appending key '" <<
      key->asString () <<
      "' to part " << getPartCombinedName () <<
      endl;
  }
#endif

  // set part key
  fPartCurrentKey = key;

  gIndenter++;

  // append key to registered staves
  for (
    map<int, S_msrStaff>::const_iterator i = fPartStavesMap.begin ();
    i != fPartStavesMap.end ();
    i++
  ) {
    S_msrStaff
      staff = (*i).second;

    staff->
      appendKeyToStaff (
        key);
  } // for

  gIndenter--;
}

void msrPart::appendTimeToPart (S_msrTime time)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceTimes) {
    gLogOstream <<
      "Appending time '" <<
      time->asString () <<
      "' to part " << getPartCombinedName () <<
      endl;
  }
#endif

  // set part time
  fPartCurrentTime = time;

  gIndenter++;

  // append time to registered staves
  for (
    map<int, S_msrStaff>::const_iterator i = fPartStavesMap.begin ();
    i != fPartStavesMap.end ();
    i++
  ) {
    S_msrStaff
      staff = (*i).second;

    staff->
      appendTimeToStaff (
        time);
  } // for

  gIndenter--;
}

void msrPart::appendTimeToPartClone (S_msrTime time)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceTimes) {
    gLogOstream <<
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

void msrPart::appendTransposeToPart (
  S_msrTranspose transpose)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceTranspositions) {
    gLogOstream <<
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
#ifdef TRACE_OAH
  if (gTraceOah->fTraceParts) {
    gLogOstream <<
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
#ifdef TRACE_OAH
  if (gTraceOah->fTraceParts) {
    gLogOstream <<
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
#ifdef TRACE_OAH
  if (gTraceOah->fTraceRepeats) {
    gLogOstream <<
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
#ifdef TRACE_OAH
  if (gTraceOah->fTraceRepeats) {
    gLogOstream <<
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
#ifdef TRACE_OAH
  if (gTraceOah->fTraceRepeats) {
    gLogOstream <<
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
  msrRepeatEnding::msrRepeatEndingKind
            repeatEndingKind)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceRepeats) {
    gLogOstream <<
      "Handling a " <<
      msrRepeatEnding::repeatEndingKindAsString (
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
#ifdef TRACE_OAH
  if (gTraceOah->fTraceRepeats) {
    gLogOstream <<
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
      case msrStaff::kStaffRegular:
        staff->appendRepeatCloneToStaff (
          inputLineNumber, repeatCLone);
        break;

      case msrStaff::kStaffTablature:
        break;

      case msrStaff::kStaffHarmony:
        break;

      case msrStaff::kStaffFiguredBass:
        break;

      case msrStaff::kStaffDrum:
        staff->appendRepeatCloneToStaff (
          inputLineNumber, repeatCLone);
        break;

      case msrStaff::kStaffRythmic:
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
#ifdef TRACE_OAH
  if (gTraceOah->fTraceRestMeasures) {
    gLogOstream <<
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
  int inputLineNumber,
  int multipleRestMeasuresMeasuresNumber)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceRestMeasures) {
    gLogOstream <<
      "Creating multiple rest measures in part " <<
      getPartCombinedName () <<
      ", " <<
      singularOrPlural (
        multipleRestMeasuresMeasuresNumber, "measure", "measures") <<
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
        multipleRestMeasuresMeasuresNumber);
  } // for
}

void msrPart::appendPendingRestMeasuresToPart (
  int inputLineNumber)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceRestMeasures) {
    gLogOstream <<
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
#ifdef TRACE_OAH
  if (gTraceOah->fTraceRestMeasures) {
    gLogOstream <<
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
  int                    inputLineNumber,
  msrStaff::msrStaffKind staffKind,
  int                    staffNumber)
{
  if (fPartStavesMap.count (staffNumber)) {
    stringstream s;

    s <<
      "staffNumber " << staffNumber <<
      " already exists in part " << getPartCombinedName () <<
      ", line " << inputLineNumber;

    msrInternalError ( // JMI ???
      gOahOah->fInputSourceName,
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());

    return fPartStavesMap [staffNumber];
  }

#ifdef TRACE_OAH
  if (gTraceOah->fTraceStaves) {
    gLogOstream <<
      "Adding " <<
      msrStaff::staffKindAsString (staffKind) <<
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
    case msrStaff::kStaffRegular:
    case msrStaff::kStaffTablature:
    case msrStaff::kStaffHarmony:
    case msrStaff::kStaffFiguredBass:
    case msrStaff::kStaffDrum:
    case msrStaff::kStaffRythmic:
      fPartStavesMap [staffNumber] = staff;
      break;
  } // switch

  // initialize staff current clef and key if relevant // JMI
  switch (staffKind) {
    case msrStaff::kStaffRegular:
    case msrStaff::kStaffTablature:
    case msrStaff::kStaffHarmony:
    case msrStaff::kStaffFiguredBass:
    case msrStaff::kStaffDrum:
    case msrStaff::kStaffRythmic:
      staff->setStaffCurrentClef (fPartCurrentClef);
      staff->setStaffCurrentKey (fPartCurrentKey);
      break;
  } // switch

  // return staff
  return staff;
}

void msrPart::addStaffToPartCloneByItsNumber (S_msrStaff staff)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceStaves) {
    gLogOstream <<
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

void msrPart::appendFiguredBassToPart (
  S_msrVoice       figuredBassSupplierVoice,
  S_msrFiguredBass figuredBass)
{
  int inputLineNumber =
    figuredBass->getInputLineNumber ();

  switch (figuredBassSupplierVoice->getVoiceKind ()) {
    case msrVoice::kVoiceRegular:
      // append the figured bass to the part figured bass voice
#ifdef TRACE_OAH
      if (gTraceOah->fTraceFiguredBasses) {
        gLogOstream <<
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

    case msrVoice::kVoiceHarmony:
    case msrVoice::kVoiceFiguredBass:
      {
        stringstream s;

        s <<
          "figured bass cannot by supplied to part by " <<
          msrVoice::voiceKindAsString (
            figuredBassSupplierVoice->getVoiceKind ()) <<
          " voice \" " <<
          figuredBassSupplierVoice->getVoiceName () <<
          "\"";

        msrInternalError (
          gOahOah->fInputSourceName,
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
    case msrVoice::kVoiceFiguredBass:
      // append the figured bass to the part figured bass voice
#ifdef TRACE_OAH
      if (gTraceOah->fTraceFiguredBasses) {
        gLogOstream <<
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

    case msrVoice::kVoiceRegular:
    case msrVoice::kVoiceHarmony:
      {
        stringstream s;

        s <<
          "figured bass cannot by supplied to part clone by " <<
          msrVoice::voiceKindAsString (
            figuredBassSupplierVoice->getVoiceKind ()) <<
          " voice \" " <<
          figuredBassSupplierVoice->getVoiceName () <<
          "\"";

        msrInternalError (
          gOahOah->fInputSourceName,
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
#ifdef TRACE_OAH
  if (gTraceOah->fTraceParts) {
    gLogOstream <<
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
#ifdef TRACE_OAH
  if (gTraceOah->fTraceParts) {
    gLogOstream <<
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
#ifdef TRACE_OAH
  if (gTraceOah->fTraceParts) {
    gLogOstream <<
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
#ifdef TRACE_OAH
  int inputLineNumber =
    skipGraceNotesGroup->getInputLineNumber ();
#endif

#ifdef TRACE_OAH
  if (
    gTraceOah->fTraceMeasures
      ||
    gTraceOah->fTraceGraceNotes
      ||
    gTraceOah->fTraceParts
    ) {
    gLogOstream <<
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
          getStaffAllVoicesMap ();

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

void msrPart::finalizeCurrentMeasureInPart (
  int    inputLineNumber)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceMeasures) {
    gLogOstream <<
      "Finalizing current measure in part " <<
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
      finalizeCurrentMeasureInStaff (
        inputLineNumber);
  } // for

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
#ifdef TRACE_OAH
  if (gTraceOah->fTraceParts) {
    gLogOstream <<
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
      gOahOah->fInputSourceName,
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
#ifdef TRACE_OAH
  if (gTraceOah->fTraceParts) {
    gLogOstream <<
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
  if (gMsrOah->fTraceMsrVisitors) {
    gLogOstream <<
      "% ==> msrPart::acceptIn ()" <<
      endl;
  }

  if (visitor<S_msrPart>*
    p =
      dynamic_cast<visitor<S_msrPart>*> (v)) {
        S_msrPart elem = this;

        if (gMsrOah->fTraceMsrVisitors) {
          gLogOstream <<
            "% ==> Launching msrPart::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrPart::acceptOut (basevisitor* v)
{
  if (gMsrOah->fTraceMsrVisitors) {
    gLogOstream <<
      "% ==> msrPart::acceptOut ()" <<
      endl;
  }

  if (visitor<S_msrPart>*
    p =
      dynamic_cast<visitor<S_msrPart>*> (v)) {
        S_msrPart elem = this;

        if (gMsrOah->fTraceMsrVisitors) {
          gLogOstream <<
            "% ==> Launching msrPart::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrPart::browseData (basevisitor* v)
{
  if (gMsrOah->fTraceMsrVisitors) {
    gLogOstream <<
      "% ==> msrPart::browseData ()" <<
      endl;
  }

  // browse all non figured bass staves
  for (
    map<int, S_msrStaff>::const_iterator i = fPartStavesMap.begin ();
    i != fPartStavesMap.end ();
    i++
  ) {
    S_msrStaff
      staff =
        (*i).second;

    if (staff != fPartFiguredBassStaff) {
      // browse the staff
      msrBrowser<msrStaff> browser (v);
      browser.browse (*staff);
    }
  } // for

  // browse the part figured bass only now if any,
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

  const int fieldWidth = 27;

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
#ifdef TRACE_OAH
  if (gTraceOah->fTraceClefs) {
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
#ifdef TRACE_OAH
  if (gTraceOah->fTraceKeys) {
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
#ifdef TRACE_OAH
  if (gTraceOah->fTraceTimes) {
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
      os <<
        "none";
    }

    os << endl;
  }
#endif

  os << endl;

  // print the registered staves
  if (fPartStavesMap.size ()) {
    gIndenter++;

    map<int, S_msrStaff>::const_iterator
      iBegin = fPartStavesMap.begin (),
      iEnd   = fPartStavesMap.end (),
      i      = iBegin;

    for ( ; ; ) {
      S_msrStaff
        staff =
          (*i).second;

      // sanity check
      msrAssert (
        staff != nullptr,
        "staff is null");

      msrStaff::msrStaffKind
        staffKind =
          staff->getStaffKind ();

      switch (staffKind) { // JMI
        case msrStaff::kStaffRegular:
          os <<
            staff;
          break;

        case msrStaff::kStaffTablature:
          os <<
            staff;
          break;

        case msrStaff::kStaffHarmony:
    // JMI      if (gMsrOah->fShowHarmonyVoices) {}
          os <<
            staff;
          break;

        case msrStaff::kStaffFiguredBass:
    // JMI      if (gMsrOah->fShowFiguredBassVoices) {}
          os <<
            staff;
          break;

        case msrStaff::kStaffDrum:
          os <<
            staff;
          break;

        case msrStaff::kStaffRythmic:
          os <<
            staff;
          break;
      } // switch

      if (++i == iEnd) break;

      os << endl;
    } // for

    gIndenter--;
  }

  gIndenter--;
}

void msrPart::printSummary (ostream& os)
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

  const int fieldWidth = 27;

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

  // print the figured bass staff if any // JMI
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
