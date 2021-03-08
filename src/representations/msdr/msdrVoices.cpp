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

#include <vector>

#include "waeMessagesHandling.h"

#include "msdrVoices.h"

#include "enableTracingIfDesired.h"
#ifdef TRACING_IS_ENABLED
  #include "traceOah.h"
#endif

#include "oahOah.h"

#include "msrOah.h"


using namespace std;

namespace MusicXML2
{

//______________________________________________________________________________
S_msdrVoice msdrVoice::create (
  int          inputLineNumber,
  msrVoiceKind voiceKind,
  int          voiceNumber,
  msrVoiceCreateInitialLastSegmentKind
               voiceCreateInitialLastSegmentKind,
  S_msrStaff   voiceStaffUpLink)
{
  msdrVoice* o =
    new msdrVoice (
      inputLineNumber,
      voiceKind,
      voiceNumber,
      voiceCreateInitialLastSegmentKind,
      voiceStaffUpLink);
  assert (o != nullptr);

  return o;
}

S_msdrVoice msdrVoice::createRegularVoice (
  int        inputLineNumber,
  int        voiceNumber,
  S_msrStaff voiceStaffUpLink)
{
  return
    msdrVoice::create (
      inputLineNumber,
      kVoiceRegular,
      voiceNumber,
      msrVoiceCreateInitialLastSegmentKind::kCreateInitialLastSegmentYes,
        // the initial last segment is ready to receive music
      voiceStaffUpLink);
}

S_msdrVoice msdrVoice::createHarmonyVoice (
  int        inputLineNumber,
  int        voiceNumber,
  S_msrStaff voiceStaffUpLink)
{
  return
    msdrVoice::create (
      inputLineNumber,
      kVoiceHarmony,
      voiceNumber,
      msrVoiceCreateInitialLastSegmentKind::kCreateInitialLastSegmentYes,
        // the initial last segment is ready to receive music
      voiceStaffUpLink);
}

S_msdrVoice msdrVoice::createFiguredBassVoice (
  int        inputLineNumber,
  int        voiceNumber,
  S_msrStaff voiceStaffUpLink)
{
  return
    msdrVoice::create (
      inputLineNumber,
      kVoiceFiguredBass,
      voiceNumber,
      msrVoiceCreateInitialLastSegmentKind::kCreateInitialLastSegmentYes,
        // the initial last segment is ready to receive music
      voiceStaffUpLink);
}

msdrVoice::msdrVoice (
  int          inputLineNumber,
  msrVoiceKind voiceKind,
  int          voiceNumber,
  msrVoiceCreateInitialLastSegmentKind
               voiceCreateInitialLastSegmentKind,
  S_msrStaff   voiceStaffUpLink)
    : msrVoice (
      inputLineNumber,
      voiceKind,
      voiceNumber,
      voiceCreateInitialLastSegmentKind,
      voiceStaffUpLink)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceVoices ()) {
    gLogStream <<
      "Creating MSDR voice \"" << asString () << "\"" <<
      endl;
  }
#endif
}

msdrVoice::msdrVoice (
  int          inputLineNumber,
  msrVoiceKind voiceKind,
  int          voiceNumber)
    : msrVoice (
        inputLineNumber,
        voiceKind,
        voiceNumber)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceVoices ()) {
    gLogStream <<
      "Creating MSDR voice \"" << asString () << "\"" <<
      endl;
  }
#endif
}

void msdrVoice::initializeVoice (
  msrVoiceCreateInitialLastSegmentKind
   voiceCreateInitialLastSegmentKind)
{
  // JMI
}

msdrVoice::~msdrVoice ()
{}

void msdrVoice::acceptIn (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msdrVoice::acceptIn ()" <<
      endl;
  }

  if (visitor<S_msrVoice>*
    p =
      dynamic_cast<visitor<S_msrVoice>*> (v)) {
        S_msrVoice elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msdrVoice::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msdrVoice::acceptOut (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msdrVoice::acceptOut ()" <<
      endl;
  }

  if (visitor<S_msrVoice>*
    p =
      dynamic_cast<visitor<S_msrVoice>*> (v)) {
        S_msrVoice elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msdrVoice::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msdrVoice::browseData (basevisitor* v)
{
/* JMI
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msdrVoice::browseData ()" <<
      endl;
  }

  // browse the voice initial elements
  if (fInitialVoiceElementsList.size ()) {
    for (
      list<S_msrVoiceElement>::const_iterator i = fInitialVoiceElementsList.begin ();
      i != fInitialVoiceElementsList.end ();
      ++i
    ) {
      // browse the element
      msrBrowser<msrVoiceElement> browser (v);
      browser.browse (*(*i));
    } // for
  }

  if (fVoiceLastSegment) {
    // browse the voice last segment
    msrBrowser<msrSegment> browser (v);
    browser.browse (*fVoiceLastSegment);
  }

  // browse the voice stanzas
  if (fVoiceStanzasMap.size ()) {
    for (
      map<string, S_msrStanza>::const_iterator i = fVoiceStanzasMap.begin ();
      i != fVoiceStanzasMap.end ();
      ++i
    ) {
      S_msrStanza stanza = (*i).second;

      if (stanza->getStanzaTextPresent ()) {
        // browse the stanza
        msrBrowser<msrStanza> browser (v);
        browser.browse (*(stanza));
      }
    } // for
  }

  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% <== msdrVoice::browseData ()" <<
      endl;
  }
  */
}

string msdrVoice::asShortString () const
{
  stringstream s;

  s <<
    "MSDR voice \"" << getVoiceName () << "\", " <<
//    msrVoiceKindAsString (fVoiceKind) <<
    ", line " << fInputLineNumber;

  return s.str ();
}

string msdrVoice::asString () const
{
  stringstream s;

  s <<
    "MSDR voice \"" << getVoiceName () << "\", " <<
//    msrVoiceKindAsString (fVoiceKind) <<
    ", line " << fInputLineNumber;

  return s.str ();
}

void msdrVoice::displayVoice (
  int    inputLineNumber,
  string context) const
{
  gLogStream <<
    endl <<
    "*********>> Displaying MSDR voice \"" <<
    getVoiceName () <<
    "\" (" << context << ")" <<
    ", line " << inputLineNumber <<
    " contains:" <<
    endl;

  ++gIndenter;
  print (gLogStream);
  --gIndenter;

  gLogStream <<
    " <<*********" <<
    endl << endl;
}

void msdrVoice::print (ostream& os) const
{
  os <<
    "MSDR voice number '" <<
    fVoiceNumber <<
    "', \"" << getVoiceName () <<
//    "\", " << msrVoiceKindAsString (fVoiceKind) <<
    ", line " << fInputLineNumber <<
    endl;

  ++gIndenter;

/* JMI
  os << left <<
    "(" <<
    singularOrPlural (
      fVoiceActualHarmoniesCounter, "harmony", "harmonies") <<
     ", " <<
    singularOrPlural (
      fVoiceActualFiguredBassesCounter, "figured bass", "figured basses") <<
     ", " <<
    singularOrPlural (
      fVoiceActualNotesCounter, "actual note", "actual notes") <<
     ", " <<
    singularOrPlural (
      fVoiceRestsCounter, "rest", "rests") <<
     ", " <<
    singularOrPlural (
      fVoiceSkipsCounter, "skip", "skips") <<
     ", " <<
    singularOrPlural (
      fVoiceStanzasMap.size (), "stanza", "stanzas") <<
    ")" <<
    endl;

  const unsigned int fieldWidth = 41;

  os << left <<
    setw (fieldWidth) <<
    "staffUpLink" << " : " <<
    fVoiceStaffUpLink->getStaffName () <<
    endl <<

    setw (fieldWidth) <<
    "voiceCurrentMeasureNumber" << " : \"" <<
    fVoiceCurrentMeasureNumber <<
    "\"" <<
    endl <<

    setw (fieldWidth) <<
    "voiceCurrentMeasureOrdinalNumber" << " : " <<
    fVoiceCurrentMeasureOrdinalNumber <<
    endl <<

    setw (fieldWidth) <<
    "voiceCurrentMeasurePuristNumber" << " : " <<
    fVoiceCurrentMeasurePuristNumber <<
    endl <<

    setw (fieldWidth) <<
    "regularVoiceStaffSequentialNumber" << " : " <<
    regularVoiceStaffSequentialNumberAsString () <<
    endl;

#ifdef TRACING_IS_ENABLED
  // regular measure ends detection
  os << left <<
    setw (fieldWidth) <<
    "wholeNotesSinceLastRegularMeasureEnd" << " : " <<
    fWholeNotesSinceLastRegularMeasureEnd <<
    endl;

  // incomplete measures after repeats detection
  os << left <<
    setw (fieldWidth) <<
    "currentVoiceRepeatPhaseKind" << " : " <<
    voiceRepeatPhaseKindAsString (
      fCurrentVoiceRepeatPhaseKind) <<
    endl;

  // print the voice first clef, and the current clef, key and time
  os << left <<
    setw (fieldWidth) << "voiceFirstClef" << " : ";
  if (fVoiceFirstClef) {
    os <<
      fVoiceFirstClef;
  }
  else {
    os << "null" << endl;
  }
  os << left <<
    setw (fieldWidth) << "voiceCurrentClef" << " : ";
  if (fVoiceCurrentClef) {
    os <<
      fVoiceCurrentClef;
  }
  else {
    os << "null" << endl;
  }

  os << left <<
    setw (fieldWidth) << "voiceCurrentKey" << " : ";
  if (fVoiceCurrentKey) {
    os <<
      fVoiceCurrentKey;
  }
  else {
    os << "null" << endl;
  }

  os << left <<
    setw (fieldWidth) << "voiceCurrentTime" << " : ";
  if (fVoiceCurrentTime) {
    ++gIndenter;
    os <<
      endl <<
      fVoiceCurrentTime;
    --gIndenter;
  }
  else {
    os << "null" << endl;
  }
#endif

/ * JMI
  // print the harmony voice name if any
  os << left <<
    setw (fieldWidth) << "regularVoiceHarmonyVoice" << " : ";
  if (fRegularVoiceHarmonyVoiceForwardLink) {
    os <<
      fRegularVoiceHarmonyVoiceForwardLink->getVoiceName ();
  }
  else {
    os <<
      "none";
  }
  os << endl;

  // print the figured bass voice name if any
  os << left <<
    setw (fieldWidth) << "regularVoiceFiguredBassVoice" << " : ";
  if (fRegularVoiceFiguredBassVoiceForwardLink) {
    os <<
      fRegularVoiceFiguredBassVoiceForwardLink->getVoiceName ();
  }
  else {
    os <<
      "none";
  }
  os << endl;
* /

  os << left <<
    setw (fieldWidth) << "voiceShortestNoteDuration" << " : " <<
    fVoiceShortestNoteDuration <<
    endl <<
    setw (fieldWidth) << "voiceShortestNoteTupletFactor" << " : " <<
    endl;

  ++gIndenter;
  os <<
    fVoiceShortestNoteTupletFactor;
  --gIndenter;

  os << left <<
    setw (fieldWidth) << "voiceHasBeenFinalized" << " : " <<
    booleanAsString (fVoiceHasBeenFinalized) <<
    endl;

  os << left <<
    setw (fieldWidth) << "currentPositionInVoice" << " : " <<
    fCurrentPositionInVoice <<
    endl <<
    setw (fieldWidth) << "currentMomentInVoice" << " : " <<
    fCurrentMomentInVoice <<
    endl;

  os << left <<
    setw (fieldWidth) << "musicHasBeenInsertedInVoice" << " : " <<
    booleanAsString (fMusicHasBeenInsertedInVoice) <<
    endl <<
    setw (fieldWidth) << "voiceContainsRestMeasures" << " : " <<
    booleanAsString (fVoiceContainsRestMeasures) <<
    endl <<
    setw (fieldWidth) << "voiceContainsMeasuresRepeats" << " : " <<
    booleanAsString (fVoiceContainsMeasuresRepeats) <<
    endl;

  // print the voice first segment if any
  os <<
    setw (fieldWidth) << "voiceFirstSegment" << " : ";
  if (fVoiceFirstSegment) {
    os <<
      "'" <<
      fVoiceFirstSegment->getSegmentAbsoluteNumber () <<
      "'";
    }
  else {
    os <<
      "none";
  }
  os << endl;

  // print the voice last appended measure if any
  os <<
    setw (fieldWidth) << "voiceLastAppendedMeasure" << " : ";
  if (fVoiceLastAppendedMeasure) {
    os <<
      "'" <<
      fVoiceLastAppendedMeasure->asShortString () <<
      "'";
    }
  else {
    os <<
      "none";
  }
  os << endl;

  // print the voice first measure if any
  os <<
    setw (fieldWidth) << "voiceFirstMeasure" << " : ";
  if (fVoiceFirstMeasure) {
    os <<
      "'" <<
      fVoiceFirstMeasure->asShortString () <<
      "'";
    }
  else {
    os <<
      "none";
  }
  os << endl;

  // print this voice's first non-grace note
  {
    S_msrNote
      voiceFirstNonGraceNote =
        this->
          fetchVoiceFirstNonGraceNote ();

    os <<
      setw (fieldWidth) <<
      "voiceFirstNonGraceNote" << " : ";
    if (voiceFirstNonGraceNote) {
      os <<
        voiceFirstNonGraceNote->asShortString ();
    }
    else {
      os <<
        "none";
    }
    os << endl;
  }

  // print the voice last appended note
  os <<
    setw (fieldWidth) <<
    "voiceLastAppendedNote" << " : ";
  if (fVoiceLastAppendedNote) {
    os << gTab <<
      fVoiceLastAppendedNote->asShortString ();
  }
  else {
    os <<
      "none";
  }
  os << endl;

#ifdef TRACING_IS_ENABLED
  // print the voice measures flat list
  unsigned int voiceMeasuresFlatListSize =
    fVoiceMeasuresFlatList.size ();

  os <<
    setw (fieldWidth) <<
    "voiceMeasuresFlatList";
  if (voiceMeasuresFlatListSize) {
    os <<
      " : " <<  voiceMeasuresFlatListSize << " elements";
  }
  else {
    os <<
      " : " << "empty";
  }
  os << endl;

  if (voiceMeasuresFlatListSize) {
    ++gIndenter;

    list<S_msrMeasure>::const_iterator
      iBegin = fVoiceMeasuresFlatList.begin (),
      iEnd   = fVoiceMeasuresFlatList.end (),
      i      = iBegin;

    for ( ; ; ) {
      // print the measure
      if (gGlobalTraceOahGroup->getTraceMeasures ()) {
        os << (*i)->asShortString ();
      }
      else {
        os << (*i)->getMeasureElementMeasureNumber ();
      }
      if (++i == iEnd) break;
      os << ' ';
    } // for
    os << endl;

    --gIndenter;
  }
#endif

  // print the voice initial elements
  unsigned int voiceInitialElementsListSize =
    fInitialVoiceElementsList.size ();

  os <<
    endl <<
    setw (fieldWidth) <<
    "voiceInitialElementsList";
  if (voiceInitialElementsListSize) {
    os << " : " <<  voiceInitialElementsListSize << " elements";
  }
  else {
    os << " : " << "none";
  }

  if (voiceInitialElementsListSize) {
    os << endl;

    ++gIndenter;

    list<S_msrVoiceElement>::const_iterator
      iBegin = fInitialVoiceElementsList.begin (),
      iEnd   = fInitialVoiceElementsList.end (),
      i      = iBegin;

    for ( ; ; ) {
      // print the element
      os << (*i);
      if (++i == iEnd) break;
      os << endl;
    } // for

    --gIndenter;
  }
  os << endl;

  / * JMI
  // sanity check
  msgAssert (
    __FILE__, __LINE__,
    fVoiceLastSegment != nullptr,
    "fVoiceLastSegment is null");
    * /

  // print the last segment
  if (fVoiceLastSegment) {
    os <<
      endl <<
      setw (fieldWidth) <<
      "voiceLastSegment" <<
      endl;

    ++gIndenter;
    os <<
      fVoiceLastSegment <<
      endl;
    --gIndenter;
  }
  else {
    os <<
      "*** voiceLastSegment is null ***" << // JMI
      endl;
  }
  os << endl;

  // print the stanzas if any
  if (fVoiceStanzasMap.size ()) {
    os <<
      "Stanzas:" <<
      endl;

    ++gIndenter;

    map<string, S_msrStanza>::const_iterator
      iBegin = fVoiceStanzasMap.begin (),
      iEnd   = fVoiceStanzasMap.end (),
      i      = iBegin;

    for ( ; ; ) {
      os << (*i).second;
      if (++i == iEnd) break;
      // no endl here
    } // for

    --gIndenter;
  }
*/

  --gIndenter;
}

void msdrVoice::printShort (ostream& os) const
{
  os <<
    "MSDR voice number '" <<
    fVoiceNumber <<
    "', \"" << getVoiceName () <<
//    "\", " << msrVoiceKindAsString (fVoiceKind) <<
    ", line " << fInputLineNumber <<
    endl;

  ++gIndenter;

/* JMI
  os << left <<
    "(" <<
    singularOrPlural (
      fVoiceActualHarmoniesCounter, "harmony", "harmonies") <<
     ", " <<
    singularOrPlural (
      fVoiceActualFiguredBassesCounter, "figured bass", "figured basses") <<
     ", " <<
    singularOrPlural (
      fVoiceActualNotesCounter, "actual note", "actual notes") <<
     ", " <<
    singularOrPlural (
      fVoiceRestsCounter, "rest", "rests") <<
     ", " <<
    singularOrPlural (
      fVoiceSkipsCounter, "skip", "skips") <<
     ", " <<
    singularOrPlural (
      fVoiceStanzasMap.size (), "stanza", "stanzas") <<
    ")" <<
    endl;

  const unsigned int fieldWidth = 41;

#ifdef TRACING_IS_ENABLED
  // print the voice measures flat list
  unsigned int voiceMeasuresFlatListSize =
    fVoiceMeasuresFlatList.size ();

  os <<
    setw (fieldWidth) <<
    "voiceMeasuresFlatList";
  if (voiceMeasuresFlatListSize) {
    os <<
      " : " <<  voiceMeasuresFlatListSize << " elements";
  }
  else {
    os <<
      " : " << "empty";
  }
  os << endl;

  if (voiceMeasuresFlatListSize) {
    ++gIndenter;

    list<S_msrMeasure>::const_iterator
      iBegin = fVoiceMeasuresFlatList.begin (),
      iEnd   = fVoiceMeasuresFlatList.end (),
      i      = iBegin;

    for ( ; ; ) {
      // print the measure
      if (gGlobalTraceOahGroup->getTraceMeasures ()) {
        os << (*i)->asShortString ();
      }
      else {
        os << (*i)->getMeasureElementMeasureNumber ();
      }
      if (++i == iEnd) break;
      os << ' ';
    } // for
    os << endl;

    --gIndenter;
  }
#endif

  // print the voice initial elements
  unsigned int voiceInitialElementsListSize =
    fInitialVoiceElementsList.size ();

  os <<
    endl <<
    setw (fieldWidth) <<
    "voiceInitialElementsList";
  if (voiceInitialElementsListSize) {
    os << " : " <<  voiceInitialElementsListSize << " elements";
  }
  else {
    os << " : " << "none";
  }

  if (voiceInitialElementsListSize) {
    os << endl;

    ++gIndenter;

    list<S_msrVoiceElement>::const_iterator
      iBegin = fInitialVoiceElementsList.begin (),
      iEnd   = fInitialVoiceElementsList.end (),
      i      = iBegin;

    for ( ; ; ) {
      // print the element
      (*i)->printShort (os);
      if (++i == iEnd) break;
      os << endl;
    } // for

    --gIndenter;
  }
  os << endl;

  / * JMI
  // sanity check
  msgAssert (
    __FILE__, __LINE__,
    fVoiceLastSegment != nullptr,
    "fVoiceLastSegment is null");
    * /

  // print the last segment
  if (fVoiceLastSegment) {
    os <<
      endl <<
      setw (fieldWidth) <<
      "voiceLastSegment" <<
      endl;

    ++gIndenter;
    fVoiceLastSegment->printShort (os);
    os <<
      endl;
    --gIndenter;
  }
  else {
    os <<
      "*** voiceLastSegment is null ***" << // JMI
      endl;
  }
  os << endl;

  // print the stanzas if any
  if (fVoiceStanzasMap.size ()) {
    os <<
      "Stanzas:" <<
      endl;

    ++gIndenter;

    map<string, S_msrStanza>::const_iterator
      iBegin = fVoiceStanzasMap.begin (),
      iEnd   = fVoiceStanzasMap.end (),
      i      = iBegin;

    for ( ; ; ) {
      (*i).second->printShort (os);
      if (++i == iEnd) break;
      // no endl here
    } // for

    --gIndenter;
  }
*/

  --gIndenter;
}

ostream& operator<< (ostream& os, const S_msdrVoice& elt)
{
  elt->print (os);
  return os;
}


}
