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
#include <sstream>
#include <iomanip>      // setw, setprecision, ...
#include <math.h>

#include "version.h"

#include "msr.h"

#ifdef TRACE_OPTIONS
  #include "traceOptions.h"
#endif

#include "musicXMLOptions.h"
#include "msrOptions.h"

#include "xml2lyOptionsHandling.h"


using namespace std;


namespace MusicXML2 
{

//______________________________________________________________________________
S_msrOrnament msrOrnament::create (
  int              inputLineNumber,
  msrOrnamentKind  ornamentKind,
  msrPlacementKind ornamentPlacementKind)
{
  msrOrnament* o =
    new msrOrnament (
      inputLineNumber, ornamentKind, ornamentPlacementKind);
  assert (o!=0);
  return o;
}

msrOrnament::msrOrnament (
  int              inputLineNumber,
  msrOrnamentKind  ornamentKind,
  msrPlacementKind ornamentPlacementKind)
    : msrElement (inputLineNumber)
{
  fOrnamentKind = ornamentKind;

  fOrnamentPlacementKind = ornamentPlacementKind;

  fOrnamentAccidentalMark = k_NoAlteration;
}

msrOrnament::~msrOrnament ()
{}

string msrOrnament::ornamentKindAsString () const
{
  string result;
  
  switch (fOrnamentKind) {
    case msrOrnament::kOrnamentTrill:
      result = "ornamentTrill";
      break;
    case msrOrnament::kOrnamentDashes:
      result = "ornamentDashes";
      break;
    case msrOrnament::kOrnamentTurn:
      result = "ornamentTurn";
      break;
    case msrOrnament::kOrnamentInvertedTurn:
      result = "ornamentInvertedTurn";
      break;
    case msrOrnament::kOrnamentDelayedTurn:
      result = "ornamentDelayedTurn";
      break;
    case msrOrnament::kOrnamentDelayedInvertedTurn:
      result = "ornamentDelayedInvertedTurn";
      break;
    case msrOrnament::kOrnamentVerticalTurn:
      result = "ornamentVerticalTurn";
      break;
    case msrOrnament::kOrnamentMordent:
      result = "ornamentMordent";
      break;
    case msrOrnament::kOrnamentInvertedMordent:
      result = "ornamentInvertedMordent";
      break;
    case msrOrnament::kOrnamentSchleifer:
      result = "ornamentSchleifer";
      break;
    case msrOrnament::kOrnamentShake:
      result = "ornamentShake";
      break;
    case msrOrnament::kOrnamentAccidentalMark:
      result = "ornamentAccidentalMark";
      break;
  } // switch

  return result;
}

string msrOrnament::ornamentPlacementKindAsString () const
{
   return
    msrPlacementKindAsString (
      fOrnamentPlacementKind);
}

string msrOrnament::ornamentAccidentalMarkAsString () const
{
  string result;
  
  switch (fOrnamentAccidentalMark) {
    case kTripleFlat:
      result = "tripleFlat";
      break;
    case kDoubleFlat:
      result = "doubleFlat";
      break;
    case kSesquiFlat:
      result = "sesquiFlat";
      break;
    case kFlat:
      result = "flat";
      break;
    case kSemiFlat:
      result = "semiFlat";
      break;
    case kNatural:
      result = "natural";
      break;
    case kSemiSharp:
      result = "semiSharp";
      break;
    case kSharp:
      result = "sharp";
      break;
    case kSesquiSharp:
      result = "sesquiSharp";
      break;
    case kDoubleSharp:
      result = "doubleSharp";
      break;
    case kTripleSharp:
      result = "tripleSharp";
      break;
    case k_NoAlteration:
      result = "noAlteration";
      break;
  } // switch

  return result;
}

void msrOrnament::acceptIn (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrOrnament::acceptIn ()" <<
      endl;
  }
      
  if (visitor<S_msrOrnament>*
    p =
      dynamic_cast<visitor<S_msrOrnament>*> (v)) {
        S_msrOrnament elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrOrnament::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrOrnament::acceptOut (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrOrnament::acceptOut ()" <<
      endl;
  }

  if (visitor<S_msrOrnament>*
    p =
      dynamic_cast<visitor<S_msrOrnament>*> (v)) {
        S_msrOrnament elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrOrnament::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrOrnament::browseData (basevisitor* v)
{}

ostream& operator<< (ostream& os, const S_msrOrnament& elt)
{
  elt->print (os);
  return os;
}

void msrOrnament::print (ostream& os)
{
  os <<
    "Ornament" <<
    ", " << ornamentKindAsString () <<
    ", line " << fInputLineNumber <<
    endl;

  gIndenter++;

  const int fieldWidth = 16;
  
  os <<
    setw (fieldWidth) <<
    "placement" << " : " <<
    ornamentPlacementKindAsString () <<
    endl <<
    setw (fieldWidth) <<
    "accidental mark" << " : " <<
    ornamentAccidentalMarkAsString () <<
    endl <<
    setw (fieldWidth) <<
    "noteUplink" << " : " <<
    fOrnamentNoteUplink->asShortString () <<
    endl;

  gIndenter--;
}

//______________________________________________________________________________
S_msrDoubleTremolo msrDoubleTremolo::create (
  int                  inputLineNumber,
  msrDoubleTremoloKind doubleTremoloKind,
  msrTremoloTypeKind   doubleTremoloTypeKind,
  int                  doubleTremoloMarksNumber,
  msrPlacementKind     doubleTremoloPlacementKind)
{
  msrDoubleTremolo* o =
    new msrDoubleTremolo (
      inputLineNumber,
      doubleTremoloKind,
      doubleTremoloTypeKind,
      doubleTremoloMarksNumber,
      doubleTremoloPlacementKind);
  assert (o!=0);
  return o;
}

msrDoubleTremolo::msrDoubleTremolo (
  int                  inputLineNumber,
  msrDoubleTremoloKind doubleTremoloKind,
  msrTremoloTypeKind   doubleTremoloTypeKind,
  int                  doubleTremoloMarksNumber,
  msrPlacementKind     doubleTremoloPlacementKind)
    : msrElement (inputLineNumber)
{
  fDoubleTremoloKind          = doubleTremoloKind;
  fDoubleTremoloTypeKind      = doubleTremoloTypeKind;
  fDoubleTremoloMarksNumber   = doubleTremoloMarksNumber;
  fDoubleTremoloPlacementKind = doubleTremoloPlacementKind;
  
  fDoubleTremoloSoundingWholeNotes =
    rational (-1, 1); // will be set later

  fDoubleTremoloNumberOfRepeats = -1; // will be set later
}

S_msrDoubleTremolo msrDoubleTremolo::createDoubleTremoloNewbornClone (
  S_msrElement doubleTremoloFirstElement,
  S_msrElement doubleTremoloSecondElement)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceTremolos) {
    gLogIOstream <<
      "Creating a newborn clone of a double tremolo" <<
      endl;
  }
#endif
  
  // sanity check
  msrAssert(
    doubleTremoloFirstElement != nullptr,
    "doubleTremoloFirstElement is null");
  msrAssert(
    doubleTremoloSecondElement != nullptr,
    "doubleTremoloSecondElement is null");
    
  S_msrDoubleTremolo
    newbornClone =
      msrDoubleTremolo::create (
        fInputLineNumber,
        fDoubleTremoloKind,
        fDoubleTremoloTypeKind,
        fDoubleTremoloMarksNumber,
        fDoubleTremoloPlacementKind);

  newbornClone->fDoubleTremoloFirstElement =
    doubleTremoloFirstElement;
  newbornClone->fDoubleTremoloSecondElement =
    doubleTremoloSecondElement;
    
  newbornClone->fDoubleTremoloSoundingWholeNotes =
    fDoubleTremoloSoundingWholeNotes;
        
  newbornClone->fDoubleTremoloElementsDuration =
    fDoubleTremoloElementsDuration;
        
  newbornClone->fDoubleTremoloNumberOfRepeats =
    fDoubleTremoloNumberOfRepeats;
        
  newbornClone->fDoubleTremoloPlacementKind =
    fDoubleTremoloPlacementKind;
        
  return newbornClone;
}

msrDoubleTremolo::~msrDoubleTremolo ()
{}

void msrDoubleTremolo::setDoubleTremoloNoteFirstElement (S_msrNote note)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceTremolos) {
    gLogIOstream <<
      "Setting note " << note->asShortString () <<
      " as first element of double tremolo " <<
      asShortString () <<
      endl;
  }
#endif

  int inputLineNumber =
    note->getInputLineNumber ();
    
  // register note as first element of this double tremolo
  fDoubleTremoloFirstElement = note;

  // mark it as being a double tremolo first element
  note->
    setNoteIsFirstNoteInADoubleTremolo ();

  // fetch note display whole notes
  rational
    noteDisplayWholeNotes =
      note->
        getNoteDisplayWholeNotes ();
  
  // set double tremolo sounding whole notes
  // to the note's display whole notes,
   // i.e. the duration of each of the two notes
  fDoubleTremoloSoundingWholeNotes =
    noteDisplayWholeNotes;
  fDoubleTremoloSoundingWholeNotes.rationalise ();

  // compute double tremolo elements duration
  // the marks number determines the duration of the two elements:
  // '8' for 1, '16' for 2, etc
  fDoubleTremoloElementsDuration =
    rational (
      1,
      /* JMI
      int (
        pow (
          2,
          fDoubleTremoloMarksNumber + 2)));
          */
      1 << (fDoubleTremoloMarksNumber + 2));

  // set note's sounding whole notes
  note->
    setNoteSoundingWholeNotes (
      fDoubleTremoloElementsDuration);

  // setting number of repeats
  rational
    numberOfRepeatsAsRational =
      fDoubleTremoloSoundingWholeNotes
        /
      fDoubleTremoloElementsDuration
        /
      2; // there are two repeated notes
  numberOfRepeatsAsRational.rationalise ();
      
  if (numberOfRepeatsAsRational.getDenominator () != 1) {
    stringstream s;

    s <<
      "cannot handle numberOfRepeatsAsRational.getDenominator () != 1" <<
      endl <<
      "numberOfRepeatsAsRational = " <<
      numberOfRepeatsAsRational <<
      endl <<
      "fDoubleTremoloSoundingWholeNotes = '" <<
      fDoubleTremoloSoundingWholeNotes << "'" <<
      endl <<
      "fDoubleTremoloElementsDuration = '" <<
      fDoubleTremoloElementsDuration << "'" <<
      endl <<
      "line " << inputLineNumber;

    msrInternalError (
      gXml2lyOptions->fInputSourceName,
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  }
    
  fDoubleTremoloNumberOfRepeats =
    numberOfRepeatsAsRational.getNumerator ();
  
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceTremolos) {
    gLogIOstream <<
      "Setting notes double tremolo number of repeats to '" <<
      fDoubleTremoloNumberOfRepeats <<
      "', fDoubleTremoloSoundingWholeNotes = '" <<
      fDoubleTremoloSoundingWholeNotes << "'" <<
      ", fDoubleTremoloElementsDuration = '" <<
      fDoubleTremoloElementsDuration << "'" <<
      "', line " << inputLineNumber <<
      endl;
  }
#endif
}

void msrDoubleTremolo::setDoubleTremoloNoteSecondElement (
  S_msrNote note)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceTremolos) {
    gLogIOstream <<
      "Setting note " << note->asShortString () <<
      " as second element of double tremolo " <<
      asShortString () <<
      endl;
  }
#endif

  int inputLineNumber =
    note->getInputLineNumber ();
    
  // register note as second element of this double tremolo
  fDoubleTremoloSecondElement = note;

  // mark it as being a double tremolo second element
  note->
    setNoteIsSecondNoteInADoubleTremolo ();

  // fetch note display whole notes
  rational
    noteDisplayWholeNotes =
      note->
        getNoteDisplayWholeNotes ();

  // compute expected double tremolo sounding whole notes
  rational
    expectedDoubleTremoloSoundingWholeNotes =
      noteDisplayWholeNotes;
  expectedDoubleTremoloSoundingWholeNotes.rationalise ();
        
  // check that expected double tremolo sounding whole notes
  // match the known double tremolo sounding whole notes
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceTremolos) {
    gLogIOstream <<
      "Checking notes double tremolo second note duration"<<
      ", doubleTremoloNumberOfRepeats = '" <<
      fDoubleTremoloNumberOfRepeats <<
      "', doubleTremoloSoundingWholeNotes = '" <<
      fDoubleTremoloSoundingWholeNotes <<
      "', doubleTremoloElementsDuration = '" <<
      fDoubleTremoloElementsDuration << "'" <<
      "', line " << inputLineNumber <<
      endl;
  }
#endif

  if (
    expectedDoubleTremoloSoundingWholeNotes
      !=
    fDoubleTremoloSoundingWholeNotes) {
    stringstream s;

    s <<
      "attempt to set notes double tremolo whole notes both to " <<
      fDoubleTremoloSoundingWholeNotes << " (existing)" <<
      " and " <<
      expectedDoubleTremoloSoundingWholeNotes <<
      " on note second element:" <<
      endl;

    gIndenter++;

    s <<
      note <<
      endl;
    
    gIndenter--;
      
    msrInternalError (
      gXml2lyOptions->fInputSourceName,
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  }
  
  // set note's sounding whole notes
  note->
    setNoteSoundingWholeNotes (
      fDoubleTremoloElementsDuration);
}

void msrDoubleTremolo::setDoubleTremoloChordFirstElement (
  S_msrChord chord)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceTremolos || gTraceOptions->fTraceChords) {
    gLogIOstream <<
      "Setting chord " << chord->asString () <<
      " as first element of double tremolo " <<
      asShortString () <<
      endl;
  }
#endif

  // register chord as first element of this double tremolo
  fDoubleTremoloFirstElement = chord;

  // mark it as being a double tremolo first element
  chord->
    setChordIsFirstChordInADoubleTremolo ();

  // fetch chord displayed whole notes
  rational
    chordDisplayWholeNotes =
      chord->
        getChordDisplayWholeNotes ();

  // compute expected double tremolo sounding whole notes
  rational
    expectedDoubleTremoloSoundingWholeNotes =
      chordDisplayWholeNotes * 2;
  expectedDoubleTremoloSoundingWholeNotes.rationalise ();
    
  // set double tremolo whole notes to the chords's displayed whole notes
  if (fDoubleTremoloSoundingWholeNotes.getNumerator () != 0) {
    if (
      expectedDoubleTremoloSoundingWholeNotes
        !=
      fDoubleTremoloSoundingWholeNotes) {
      stringstream s;

      s <<
        "attempt to set chord double tremolo sounding whole notes both to " <<
        fDoubleTremoloSoundingWholeNotes << " (existing)" <<
        " and " <<
        expectedDoubleTremoloSoundingWholeNotes <<
        " on chord first element:" <<
        endl;

      gIndenter++;

      s <<
        chord <<
        endl;
      
      gIndenter--;
                
      msrInternalError (
        gXml2lyOptions->fInputSourceName,
        chord->getInputLineNumber (),
        __FILE__, __LINE__,
        s.str ());
    }
  }

  fDoubleTremoloSoundingWholeNotes =
    chordDisplayWholeNotes * 2; // taking the second note into account
  fDoubleTremoloSoundingWholeNotes.rationalise ();
}

void msrDoubleTremolo::setDoubleTremoloChordSecondElement (S_msrChord chord)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceTremolos || gTraceOptions->fTraceChords) {
    gLogIOstream <<
      "Setting chord " << chord->asString () <<
      " as second element of double tremolo " <<
      asShortString () <<
      endl;
  }
#endif

  // register chord as second element of this double tremolo
  fDoubleTremoloSecondElement = chord;

  // mark it as being a double tremolo second element
  chord->
    setChordIsSecondChordInADoubleTremolo ();

  // fetch chord displayed whole notes
  rational
    chordDisplayWholeNotes =
      chord->
        getChordDisplayWholeNotes ();
    
  // set double tremolo whole notes to the chords's displayed whole notes
  if (fDoubleTremoloSoundingWholeNotes.getNumerator () != 0) {
    if (chordDisplayWholeNotes != fDoubleTremoloSoundingWholeNotes) { // JMI
      stringstream s;

     s <<
        "attempt to set double tremolo whole notes both to " <<
        fDoubleTremoloSoundingWholeNotes << " (existing)" <<
        " and " <<
        chordDisplayWholeNotes <<
        " on chord second element:" << " (chord)" <<
        endl;

      gIndenter++;

      s <<
        chord <<
        endl;
      
      gIndenter--;
        
      msrInternalError (
        gXml2lyOptions->fInputSourceName,
        chord->getInputLineNumber (),
        __FILE__, __LINE__,
        s.str ());
    }
  }
  
  else {
    fDoubleTremoloSoundingWholeNotes =
      chordDisplayWholeNotes;
  }
}

string msrDoubleTremolo::msrDoubleTremoloKindAsString (
  msrDoubleTremolo::msrDoubleTremoloKind doubleTremolotKind)
{
  string result;
  
  switch (doubleTremolotKind) {
    case msrDoubleTremolo::kNotesDoubleTremolo:
      result = "Notes";
      break;
    case msrDoubleTremolo::kChordsDoubleTremolo:
      result = "Chords";
      break;
  } // switch

  return result;
}

string msrDoubleTremolo::doubleTremoloPlacementKindAsString () const
{
  return
    msrPlacementKindAsString (
      fDoubleTremoloPlacementKind);
}

void msrDoubleTremolo::acceptIn (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrDoubleTremolo::acceptIn ()" <<
      endl;
  }
      
  if (visitor<S_msrDoubleTremolo>*
    p =
      dynamic_cast<visitor<S_msrDoubleTremolo>*> (v)) {
        S_msrDoubleTremolo elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrDoubleTremolo::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrDoubleTremolo::acceptOut (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrDoubleTremolo::acceptOut ()" <<
      endl;
  }

  if (visitor<S_msrDoubleTremolo>*
    p =
      dynamic_cast<visitor<S_msrDoubleTremolo>*> (v)) {
        S_msrDoubleTremolo elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrDoubleTremolo::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrDoubleTremolo::browseData (basevisitor* v)
{
  if (fDoubleTremoloFirstElement) {
    // browse the first element
    msrBrowser<msrElement> browser (v);
    browser.browse (*fDoubleTremoloFirstElement);
  }

  if (fDoubleTremoloSecondElement) {
    // browse the second element
    msrBrowser<msrElement> browser (v);
    browser.browse (*fDoubleTremoloSecondElement);
  }
}

ostream& operator<< (ostream& os, const S_msrDoubleTremolo& elt)
{
  elt->print (os);
  return os;
}

string msrDoubleTremolo::asShortString () const
{
  stringstream s;
  
  s <<
    "DoubleTremolo"<<
    ", " << msrTremoloTypeKindAsString (fDoubleTremoloTypeKind) <<
    ", line " << fInputLineNumber <<
    ", " <<
    singularOrPlural (
      fDoubleTremoloMarksNumber, "mark", "marks") <<
    ", placement" << " = " << doubleTremoloPlacementKindAsString ();

  return s.str ();
}

string msrDoubleTremolo::asString () const
{
  stringstream s;
  
  s <<
    "DoubleTremolo" " " <<
    ", " << msrTremoloTypeKindAsString (fDoubleTremoloTypeKind) <<
    ", line " << fInputLineNumber <<
    fDoubleTremoloMarksNumber << " marks" <<
    ", placement" << " = " << doubleTremoloPlacementKindAsString () <<
    ", " << fDoubleTremoloSoundingWholeNotes << " sound whole notes";

  if (fDoubleTremoloFirstElement) { // it may not be set yet
    s <<
      ", first element " << " = ";
      
    switch (fDoubleTremoloKind) {
      case msrDoubleTremolo::kNotesDoubleTremolo:
        if (
          S_msrNote
            note =
              dynamic_cast<msrNote*>(&(*fDoubleTremoloFirstElement))
          ) {    
          note->asShortString ();
        }
        else {
          msrInternalError (
            gXml2lyOptions->fInputSourceName,
            fInputLineNumber,
            __FILE__, __LINE__,
            "notes double tremolo first element should be a note");
        }
        break;
        
      case msrDoubleTremolo::kChordsDoubleTremolo:
        if (
          S_msrChord
            chord =
              dynamic_cast<msrChord*>(&(*fDoubleTremoloFirstElement))
          ) {
          chord->asString ();
        }
        else {
          msrInternalError (
            gXml2lyOptions->fInputSourceName,
            fInputLineNumber,
            __FILE__, __LINE__,
            "chords double tremolo first element should be a chord");
        }
        break;
    } // switch
  }
      
  if (fDoubleTremoloSecondElement) { // it may not be set yet
    s <<
      ", second element " << " = ";
      
    switch (fDoubleTremoloKind) {
      case msrDoubleTremolo::kNotesDoubleTremolo:
        if (
          S_msrNote
            note =
              dynamic_cast<msrNote*>(&(*fDoubleTremoloSecondElement))
          ) {    
          note->asShortString ();
        }
        else {
          msrInternalError (
            gXml2lyOptions->fInputSourceName,
            fInputLineNumber,
            __FILE__, __LINE__,
            "notes double tremolo second element should be a note");
        }
        break;
        
      case msrDoubleTremolo::kChordsDoubleTremolo:
        if (
          S_msrChord
            chord =
              dynamic_cast<msrChord*>(&(*fDoubleTremoloSecondElement))
          ) {
          chord->asString ();
        }
        else {
          msrInternalError (
            gXml2lyOptions->fInputSourceName,
            fInputLineNumber,
            __FILE__, __LINE__,
            "chords double tremolo second element should be a chord");
        }
        break;
    } // switch
  }

  return s.str ();
}

void msrDoubleTremolo::print (ostream& os)
{
  os <<
    "DoubleTremolo" <<
    ", " << msrTremoloTypeKindAsString (fDoubleTremoloTypeKind) <<
    ", on " << msrDoubleTremoloKindAsString (fDoubleTremoloKind) <<
    ", line " << fInputLineNumber <<
    endl;

  const int fieldWidth = 32;

  gIndenter++;
  
  os << left <<
    setw (fieldWidth) <<
    "doubleTremoloSoundingWholeNotes" << " : " <<
    fDoubleTremoloSoundingWholeNotes <<
    endl <<
    
    setw (fieldWidth) <<
    "doubleTremoloMarksNumber" << " : " <<
    fDoubleTremoloMarksNumber <<
    endl <<
    
    setw (fieldWidth) <<
    "doubleTremoloElementsDuration" << " : " <<
    fDoubleTremoloElementsDuration <<
    endl <<
    
    setw (fieldWidth) <<
    "numberOfRepeats" << " : " <<
    fDoubleTremoloNumberOfRepeats <<
    endl <<
    
    setw (fieldWidth) <<
    "placement" << " : " <<
    doubleTremoloPlacementKindAsString () <<
    endl;

  os <<
    "First element:";
  if (fDoubleTremoloFirstElement) { // it may not yet be set
    os <<
      endl;
      
    gIndenter++;
    os <<
      fDoubleTremoloFirstElement;
    gIndenter--;
  }
  else {
    os <<
      " none" <<
      endl;
  }
      
  os <<
    "Second element:";
  if (fDoubleTremoloSecondElement) { // it may not yet be set
    os <<
      endl;

    gIndenter++;
    os <<
      fDoubleTremoloSecondElement;
    gIndenter--;
  }
  else {
    os <<
      " none" <<
      endl;
  }

  gIndenter--;
}

//______________________________________________________________________________
S_msrSpanner msrSpanner::create (
  int                inputLineNumber,
  int                spannerNumber,
  msrSpannerKind     spannerKind,
  msrSpannerTypeKind spannerTypeKind,
  msrPlacementKind   spannerPlacementKind,
  S_msrNote          spannerNoteUplink)
{
  msrSpanner* o =
    new msrSpanner (
      inputLineNumber,
      spannerNumber,
      spannerKind,
      spannerTypeKind,
      spannerPlacementKind,
      spannerNoteUplink);
  assert (o!=0);
  return o;
}

msrSpanner::msrSpanner (
  int                inputLineNumber,
  int                spannerNumber,
  msrSpannerKind     spannerKind,
  msrSpannerTypeKind spannerTypeKind,
  msrPlacementKind   spannerPlacementKind,
  S_msrNote          spannerNoteUplink)
    : msrElement (inputLineNumber)
{
  fSpannerNoteUplink = spannerNoteUplink;

  fSpannerNumber = spannerNumber;
  
  fSpannerKind = spannerKind;
  
  fSpannerTypeKind = spannerTypeKind;

  fSpannerPlacementKind = spannerPlacementKind;
}

msrSpanner::~msrSpanner ()
{}

void msrSpanner::setSpannerOtherEndSidelink (
  S_msrSpanner otherEndSideLink)
{
  // set the two-way sidelink between both ends of the spanner
  fSpannerOtherEndSidelink =
    otherEndSideLink;
    
  otherEndSideLink->fSpannerOtherEndSidelink =
    this;
}

string msrSpanner::spannerKindAsString (
  msrSpannerKind spannerKind)
{
  string result;
  
  switch (spannerKind) {
    case msrSpanner::kSpannerDashes:
      result = "spannerDashes";
      break;
    case msrSpanner::kSpannerWavyLine:
      result = "spannerWavyLine";
      break;
  } // switch

  return result;
}

string msrSpanner::spannerTypeKindAsString () const
{
  return
    msrSpannerTypeKindAsString (
      fSpannerTypeKind);
}

string msrSpanner::spannerKindAsString () const
{
  return
    spannerKindAsString (
      fSpannerKind);
}

string msrSpanner::spannerPlacementKindAsString () const
{
  return
    msrPlacementKindAsString (
      fSpannerPlacementKind);
}

void msrSpanner::acceptIn (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrSpanner::acceptIn ()" <<
      endl;
  }
      
  if (visitor<S_msrSpanner>*
    p =
      dynamic_cast<visitor<S_msrSpanner>*> (v)) {
        S_msrSpanner elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrSpanner::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrSpanner::acceptOut (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrSpanner::acceptOut ()" <<
      endl;
  }

  if (visitor<S_msrSpanner>*
    p =
      dynamic_cast<visitor<S_msrSpanner>*> (v)) {
        S_msrSpanner elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrSpanner::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrSpanner::browseData (basevisitor* v)
{}

ostream& operator<< (ostream& os, const S_msrSpanner& elt)
{
  elt->print (os);
  return os;
}

void msrSpanner::print (ostream& os)
{
  os <<
    "Spanner" <<
    ", spannerNumber = " << fSpannerNumber <<
    ", " << spannerKindAsString () <<
    ", line " << fInputLineNumber <<
    endl;

  gIndenter++;

  const int fieldWidth = 21;
  
  os << left <<
    setw (fieldWidth) <<
    "spannerPlacementKind" << " : " <<
    spannerPlacementKindAsString () <<
    endl <<
    setw (fieldWidth) <<
    "spannerTypeKind" << " : " <<
    spannerTypeKindAsString () <<
    endl <<
    setw (fieldWidth) <<
    "spannerNoteUplink" << " : " <<
    endl;

  gIndenter++;
    os <<
      fSpannerNoteUplink->asString () <<
      endl;
  gIndenter--;
    
  gIndenter--;
}

//______________________________________________________________________________
S_msrGraceNotes msrGraceNotes::create (
  int        inputLineNumber,
  bool       gracenoteIsSlashed,
  S_msrVoice graceNotesVoiceUplink)
{
  msrGraceNotes* o =
    new msrGraceNotes (
      inputLineNumber,
      gracenoteIsSlashed,
      graceNotesVoiceUplink);
  assert(o!=0);

  return o;
}

msrGraceNotes::msrGraceNotes (
  int        inputLineNumber,
  bool       gracenoteIsSlashed,
  S_msrVoice graceNotesVoiceUplink)
    : msrElement (inputLineNumber)
{
  // sanity check
  msrAssert(
    graceNotesVoiceUplink != nullptr,
    "graceNotesVoiceUplink is null");

  fGraceNotesVoiceUplink =
    graceNotesVoiceUplink;    
    
  fGraceNotesIsSlashed = gracenoteIsSlashed;

  fGraceNotesIsTied = false;

  // grace notes are followed by notes
  // unless they are last in a measure
  fGraceNotesIsFollowedByNotes = true;
}

msrGraceNotes::~msrGraceNotes ()
{}

S_msrGraceNotes msrGraceNotes::createGraceNotesNewbornClone (
  S_msrVoice containingVoice)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceGraceNotes) {
    gLogIOstream <<
      "Creating a newborn clone of grace notes" <<
      asShortString () <<
      endl;
  }
#endif
  
  // sanity check
  msrAssert(
    containingVoice != nullptr,
    "containingVoice is null");
    
  S_msrGraceNotes
    newbornClone =
      msrGraceNotes::create (
        fInputLineNumber,
        fGraceNotesIsSlashed,
        containingVoice);

  newbornClone->fGraceNotesIsTied =
    fGraceNotesIsTied;
    
  newbornClone->fGraceNotesIsFollowedByNotes =
    fGraceNotesIsFollowedByNotes;
    
  return newbornClone;
}

S_msrPart msrGraceNotes::graceNotesPartUplink () const
{
  return
    fGraceNotesVoiceUplink->
      fetchVoicePartUplink ();
}

S_msrGraceNotes msrGraceNotes::createSkipGraceNotesClone (
  S_msrVoice containingVoice)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceGraceNotes) {
    gLogIOstream <<
      "Creating a skip clone of grace notes" <<
      asShortString () <<
      endl;
  }
#endif
  
  S_msrGraceNotes
    clone =
      msrGraceNotes::create (
        fInputLineNumber,
        fGraceNotesIsSlashed,
        containingVoice);

  clone->fGraceNotesIsTied =
    fGraceNotesIsTied;
    
  clone->fGraceNotesIsFollowedByNotes =
    fGraceNotesIsFollowedByNotes;
    
  // populating the clone with skips
  for (
    list<S_msrElement>::const_iterator i=fGraceNotesElementsList.begin ();
    i!=fGraceNotesElementsList.end ();
    i++) {      
    if (
      S_msrNote note = dynamic_cast<msrNote*>(&(*(*i)))
      ) {
      // create skip with same duration as note
      S_msrNote
        skip =
          msrNote::createSkipNote (
            note->            getInputLineNumber (),
            note->            getNoteDisplayWholeNotes (), // would be 0/1 otherwise JMI
            note->            getNoteDisplayWholeNotes (),
            note->            getNoteDotsNumber (),
            containingVoice-> getRegularVoiceStaffSequentialNumber (), // JMI
            containingVoice-> getVoiceNumber ());

      // append it to the grace notes
      clone->
        appendNoteToGraceNotes (skip);
    }
  
    else if (
      S_msrChord chord = dynamic_cast<msrChord*>(&(*(*i)))
      ) {
      // JMI ???
    }
    
    else {
      msrInternalError (
        gXml2lyOptions->fInputSourceName,
        fInputLineNumber,
        __FILE__, __LINE__,
        "grace notes element should be a note or a chord");
    }    
  } // for
    
  return clone;
}

void msrGraceNotes::appendNoteToGraceNotes (S_msrNote note)
{
  fGraceNotesElementsList.push_back (note);

  // is this grace note tied?
  if (note->getNoteTie ()) {
    fGraceNotesIsTied = true;
  }
}

void msrGraceNotes::appendChordToGraceNotes (S_msrChord chord)
{
  fGraceNotesElementsList.push_back (chord);
}

S_msrNote msrGraceNotes::removeLastNoteFromGraceNotes (
  int inputLineNumber)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceGraceNotes) {
    gLogIOstream <<
      "Removing last note from grace notes " <<
      " in voice \"" <<
      fGraceNotesVoiceUplink->getVoiceName () <<
      "\"" <<
      endl;
  }
#endif

  // sanity check
  msrAssert (
    fGraceNotesElementsList.size () != 0,
    "fGraceNotesElementsList.size () == 0");

  S_msrNote result;

  if (
    S_msrNote note = dynamic_cast<msrNote*>(&(*fGraceNotesElementsList.back ()))
    ) {
    result = note;
  }

  else {
    msrInternalError (
      gXml2lyOptions->fInputSourceName,
      fInputLineNumber,
      __FILE__, __LINE__,
      "removeLastNoteFromGraceNotes (): grace notes element should be a note");
  }   

  fGraceNotesElementsList.pop_back ();

  return result;
}

void msrGraceNotes::acceptIn (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrGraceNotes::acceptIn ()" <<
      endl;
  }
      
  if (visitor<S_msrGraceNotes>*
    p =
      dynamic_cast<visitor<S_msrGraceNotes>*> (v)) {
        S_msrGraceNotes elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrGraceNotes::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrGraceNotes::acceptOut (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrGraceNotes::acceptOut ()" <<
      endl;
  }

  if (visitor<S_msrGraceNotes>*
    p =
      dynamic_cast<visitor<S_msrGraceNotes>*> (v)) {
        S_msrGraceNotes elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrGraceNotes::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrGraceNotes::browseData (basevisitor* v)
{
  list<S_msrElement>::const_iterator i;

  for (
    i=fGraceNotesElementsList.begin ();
    i!=fGraceNotesElementsList.end ();
    i++) {
    // browse the element (note or chord)
    msrBrowser<msrElement> browser (v);
    browser.browse (*(*i));
  } // for
}

ostream& operator<< (ostream& os, const S_msrGraceNotes& elt)
{
  elt->print (os);
  return os;
}

string msrGraceNotes::asShortString () const
{
  stringstream s;

  s <<
    "GraceNotes " <<
    "fGraceNotesMeasureNumber \"" << fGraceNotesMeasureNumber <<
    "\", ";

  if (fGraceNotesElementsList.size ()) {
    list<S_msrElement>::const_iterator
      iBegin = fGraceNotesElementsList.begin (),
      iEnd   = fGraceNotesElementsList.end (),
      i      = iBegin;
    for ( ; ; ) {
      s << (*i)->asShortString ();
      if (++i == iEnd) break;
      s << " ";
    } // for
  }
  
  return s.str ();
}

void msrGraceNotes::print (ostream& os)
{
  os <<
    "GraceNotes" <<
    ", line " << fInputLineNumber <<
    ", " <<
    singularOrPlural (
      fGraceNotesElementsList.size (), "element", "elementss") <<
    ", slashed: " <<
    booleanAsString (fGraceNotesIsSlashed) <<
    ", tied: " <<
    booleanAsString (fGraceNotesIsTied) <<
    ", followedByNotes: " <<
    booleanAsString (fGraceNotesIsFollowedByNotes) <<
    ", fGraceNotesMeasureNumber: \"" <<
    fGraceNotesMeasureNumber <<
    "\"";
  
  if (fGraceNotesElementsList.size ()) {
    os <<
      endl;
      
    gIndenter++;
  
    list<S_msrElement>::const_iterator
      iBegin = fGraceNotesElementsList.begin (),
      iEnd   = fGraceNotesElementsList.end (),
      i      = iBegin;
      
    for ( ; ; ) {
      os << (*i);
      if (++i == iEnd) break;
      os << endl;
    } // for
  
    gIndenter--;
  }
  else {
    os <<
      "*** no grace notes elements ***" <<
      endl;
  }
}

//______________________________________________________________________________
S_msrAfterGraceNotesContents msrAfterGraceNotesContents::create (
  int        inputLineNumber,
  S_msrVoice afterGraceNotesContentsVoiceUplink)
{
  msrAfterGraceNotesContents* o =
    new msrAfterGraceNotesContents (
      inputLineNumber,
      afterGraceNotesContentsVoiceUplink);
  assert(o!=0);

  return o;
}

msrAfterGraceNotesContents::msrAfterGraceNotesContents (
  int        inputLineNumber,
  S_msrVoice afterGraceNotesContentsVoiceUplink)
    : msrElement (inputLineNumber)
{
  // sanity check
  msrAssert(
    afterGraceNotesContentsVoiceUplink != nullptr,
    "afterGraceNotesContentsVoiceUplink is null");
  
  // set after notes contents's voice uplink
  fAfterGraceNotesContentsVoiceUplink =
    afterGraceNotesContentsVoiceUplink;
}

msrAfterGraceNotesContents::~msrAfterGraceNotesContents ()
{}

S_msrPart msrAfterGraceNotesContents::fetchAfterGraceNotesContentsPartUplink () const
{
  return
    fAfterGraceNotesContentsVoiceUplink->
      fetchVoicePartUplink ();
}

S_msrAfterGraceNotesContents msrAfterGraceNotesContents::createAfterGraceNotesContentsNewbornClone (
  S_msrVoice containingVoice)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceGraceNotes) {
    gLogIOstream <<
      "Creating a newborn clone of after grace notes" <<
      endl;
  }
#endif

  // sanity check
  msrAssert(
    containingVoice != nullptr,
    "containingVoice is null");
        
  S_msrAfterGraceNotesContents
    newbornClone =
      msrAfterGraceNotesContents::create (
        fInputLineNumber,
        containingVoice);
    
  return newbornClone;
}

void msrAfterGraceNotesContents::appendNoteToAfterGraceNotesContents (
  S_msrNote note)
{
  fAfterGraceNotesContentsNotesList.push_back (note);
}

void msrAfterGraceNotesContents::acceptIn (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrAfterGraceNotesContents::acceptIn ()" <<
      endl;
  }
      
  if (visitor<S_msrAfterGraceNotesContents>*
    p =
      dynamic_cast<visitor<S_msrAfterGraceNotesContents>*> (v)) {
        S_msrAfterGraceNotesContents elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrAfterGraceNotesContents::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrAfterGraceNotesContents::acceptOut (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrAfterGraceNotesContents::acceptOut ()" <<
      endl;
  }

  if (visitor<S_msrAfterGraceNotesContents>*
    p =
      dynamic_cast<visitor<S_msrAfterGraceNotesContents>*> (v)) {
        S_msrAfterGraceNotesContents elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrAfterGraceNotesContents::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrAfterGraceNotesContents::browseData (basevisitor* v)
{
  list<S_msrNote>::const_iterator i;

  for (
    i=fAfterGraceNotesContentsNotesList.begin ();
    i!=fAfterGraceNotesContentsNotesList.end ();
    i++) {
    // browse the note
    msrBrowser<msrNote> browser (v);
    browser.browse (*(*i));
  } // for
}

ostream& operator<< (ostream& os, const S_msrAfterGraceNotesContents& elt)
{
  elt->print (os);
  return os;
}

string msrAfterGraceNotesContents::asShortString () const
{
  stringstream s;

  s <<
    "AfterGraceNotesContents" <<
    ", " <<
    singularOrPlural (
      fAfterGraceNotesContentsNotesList.size (), "note", "notes");


  if (fAfterGraceNotesContentsNotesList.size ()) {
    list<S_msrNote>::const_iterator
      iBegin = fAfterGraceNotesContentsNotesList.begin (),
      iEnd   = fAfterGraceNotesContentsNotesList.end (),
      i      = iBegin;
      
    for ( ; ; ) {
      s << (*i)->asShortString ();
      if (++i == iEnd) break;
      s << " ";
    } // for
  }
  
  return s.str ();
}

void msrAfterGraceNotesContents::print (ostream& os)
{
  os <<
    "AfterGraceNotesContents" <<
    ", " <<
    singularOrPlural (
      fAfterGraceNotesContentsNotesList.size (), "note", "notes") <<
    ", line " << fInputLineNumber <<
    endl;
  
  gIndenter++;

  if (fAfterGraceNotesContentsNotesList.size ()) {
    list<S_msrNote>::const_iterator
      iBegin = fAfterGraceNotesContentsNotesList.begin (),
      iEnd   = fAfterGraceNotesContentsNotesList.end (),
      i      = iBegin;
      
    for ( ; ; ) {
      os << (*i);
      if (++i == iEnd) break;
   // JMI   os << endl;
    } // for
  }
  
  gIndenter--;
}

//______________________________________________________________________________
S_msrAfterGraceNotes msrAfterGraceNotes::create (
  int          inputLineNumber,
  S_msrElement afterGraceNotesElement,
  bool         aftergracenoteIsSlashed,
  S_msrVoice   afterGraceNotesVoiceUplink)
{
  msrAfterGraceNotes* o =
    new msrAfterGraceNotes (
      inputLineNumber,
      afterGraceNotesElement,
      aftergracenoteIsSlashed,
      afterGraceNotesVoiceUplink);
  assert(o!=0);

  return o;
}

msrAfterGraceNotes::msrAfterGraceNotes (
  int          inputLineNumber,
  S_msrElement afterGraceNotesElement,
  bool         aftergracenoteIsSlashed,
  S_msrVoice   afterGraceNotesVoiceUplink)
    : msrElement (inputLineNumber)
{
  // sanity check
  msrAssert(
    afterGraceNotesVoiceUplink != nullptr,
    "afterGraceNotesVoiceUplink is null");
  
  // set gracenote's voice uplink
  fAfterGraceNotesVoiceUplink =
    afterGraceNotesVoiceUplink;

  // pupulate this after grace notes
  fAfterGraceNotesElement =
    afterGraceNotesElement;
    
  fAfterGraceNotesIsSlashed =
    aftergracenoteIsSlashed;

  // create the after grace notes contents
  fAfterGraceNotesContents =
    msrAfterGraceNotesContents::create (
      inputLineNumber,
      afterGraceNotesVoiceUplink);
}

msrAfterGraceNotes::~msrAfterGraceNotes ()
{}

S_msrPart msrAfterGraceNotes::fetchAfterGraceNotesPartUplink () const
{
  return
    fAfterGraceNotesVoiceUplink->
      fetchVoicePartUplink ();
}

S_msrAfterGraceNotes msrAfterGraceNotes::createAfterGraceNotesNewbornClone (
  S_msrNote  noteClone,
  S_msrVoice containingVoice)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceGraceNotes) {
    gLogIOstream <<
      "Creating a newborn clone of after grace notes" <<
      endl;
  }
#endif

  // sanity check
  msrAssert(
    noteClone != nullptr,
    "noteClone is null");
    
  // sanity check
  msrAssert(
    containingVoice != nullptr,
    "containingVoice is null");
        
  S_msrAfterGraceNotes
    newbornClone =
      msrAfterGraceNotes::create (
        fInputLineNumber,
        noteClone,
        fAfterGraceNotesIsSlashed,
        containingVoice);
  
  return newbornClone;
}

void msrAfterGraceNotes::appendNoteToAfterGraceNotesContents (
  S_msrNote note)
{
  fAfterGraceNotesContents->
    appendNoteToAfterGraceNotesContents (
      note);
}

void msrAfterGraceNotes::acceptIn (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrAfterGraceNotes::acceptIn ()" <<
      endl;
  }
      
  if (visitor<S_msrAfterGraceNotes>*
    p =
      dynamic_cast<visitor<S_msrAfterGraceNotes>*> (v)) {
        S_msrAfterGraceNotes elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrAfterGraceNotes::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrAfterGraceNotes::acceptOut (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrAfterGraceNotes::acceptOut ()" <<
      endl;
  }

  if (visitor<S_msrAfterGraceNotes>*
    p =
      dynamic_cast<visitor<S_msrAfterGraceNotes>*> (v)) {
        S_msrAfterGraceNotes elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrAfterGraceNotes::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrAfterGraceNotes::browseData (basevisitor* v)
{
  list<S_msrNote>::const_iterator i;

  {
    // browse the afterGraceNotes note
    msrBrowser<msrElement> browser (v);
    browser.browse (*fAfterGraceNotesElement);
  }

  {
    // browse the afterGraceNotesContents
    msrBrowser<msrAfterGraceNotesContents> browser (v);
    browser.browse (*fAfterGraceNotesContents);
  }
}

ostream& operator<< (ostream& os, const S_msrAfterGraceNotes& elt)
{
  elt->print (os);
  return os;
}

string msrAfterGraceNotes::asShortString () const
{
  stringstream s;

  s <<
    "AfterGraceNotes " <<
    ", AfterGraceNotesElement: " <<
    "JMI ???fAfterGraceNotesElement->asShortString ()" <<
    ", fAfterGraceNotesContents: " <<
    fAfterGraceNotesContents->asShortString ();

  return s.str ();
}

void msrAfterGraceNotes::print (ostream& os)
{
  os <<
    "AfterGraceNotes" <<
    ", line " << fInputLineNumber <<
    ", slashed: " <<
    booleanAsString (fAfterGraceNotesIsSlashed) <<
    endl;
  
  gIndenter++;

  // print the afterGraceNotes element
  os <<
    "Element:" <<
    endl;
  gIndenter++;
  os <<
    fAfterGraceNotesElement;
  gIndenter--;

  // print the afterGraceNotes contents
  os <<
    fAfterGraceNotesContents;

  gIndenter--;
}

//______________________________________________________________________________
S_msrNote msrNote::create (
  int                        inputLineNumber,
  
  msrNoteKind                noteKind,

  msrQuarterTonesPitchKind   noteQuarterTonesPitchKind,
  
  rational                   noteSoundingWholeNotes,
  rational                   noteDisplayWholeNotes,
  
  int                        noteDotsNumber,
  
  msrDurationKind            noteGraphicDurationKind,
  
  int                        noteOctave,
  
  msrQuarterTonesPitchKind   noteQuarterTonesDisplayPitchKind,
  int                        noteDisplayOctave,
  
  bool                       noteIsARest,
  bool                       noteIsUnpitched,

  bool                       noteIsACueNote,
  bool                       noteIsAGraceNote,

  msrNotePrintKind           msrNotePrintKind,
  
  msrNoteHeadKind            noteHeadKind,
  msrNoteHeadFilledKind      noteHeadFilledKind,
  msrNoteHeadParenthesesKind noteHeadParenthesesKind)
{  
  msrNote * o =
    new msrNote (
      inputLineNumber,
      
      noteKind,
      
      noteQuarterTonesPitchKind,
      
      noteSoundingWholeNotes,
      noteDisplayWholeNotes,
      
      noteDotsNumber,
      
      noteGraphicDurationKind,
      
      noteOctave,

      noteQuarterTonesDisplayPitchKind,
      noteDisplayOctave,
      
      noteIsARest,
      noteIsUnpitched,

      noteIsACueNote,
      noteIsAGraceNote,

      msrNotePrintKind,

      noteHeadKind,
      noteHeadFilledKind,
      noteHeadParenthesesKind);
  assert(o!=0);

  return o;
}

msrNote::msrNote (
  int                        inputLineNumber,
  
  msrNoteKind                noteKind,

  msrQuarterTonesPitchKind   noteQuarterTonesPitchKind,
  
  rational                   noteSoundingWholeNotes,
  rational                   noteDisplayWholeNotes,
  
  int                        noteDotsNumber,
  
  msrDurationKind            noteGraphicDurationKind,
  
  int                        noteOctave,
  
  msrQuarterTonesPitchKind   noteQuarterTonesDisplayPitchKind,
  int                        noteDisplayOctave,
  
  bool                       noteIsARest,
  bool                       noteIsUnpitched,

  bool                       noteIsACueNote,
  bool                       noteIsAGraceNote,

  msrNotePrintKind           notePrintKind,
  
  msrNoteHeadKind            noteHeadKind,
  msrNoteHeadFilledKind      noteHeadFilledKind,
  msrNoteHeadParenthesesKind noteHeadParenthesesKind)
  : msrElement (inputLineNumber)
{
  // basic note description
  fNoteKind = noteKind;

  fNoteQuarterTonesPitchKind  = noteQuarterTonesPitchKind;
  
  fNoteSoundingWholeNotes = noteSoundingWholeNotes;
  fNoteDisplayWholeNotes  = noteDisplayWholeNotes;
  
  fNoteDotsNumber = noteDotsNumber;
  
  fNoteGraphicDurationKind = noteGraphicDurationKind;
  
  fNoteOctave = noteOctave;

  fNoteQuarterTonesDisplayPitchKind = noteQuarterTonesDisplayPitchKind;
  fNoteDisplayOctave                = noteDisplayOctave;

  fNoteIsARest     = noteIsARest;
  fNoteIsUnpitched = noteIsUnpitched;

  fNoteIsACueNote   = noteIsACueNote;
  fNoteIsAGraceNote = noteIsAGraceNote;

  fNotePrintKind = notePrintKind;
  
  fNoteHeadKind            = noteHeadKind;
  fNoteHeadFilledKind      = noteHeadFilledKind;
  fNoteHeadParenthesesKind = noteHeadParenthesesKind;

  // do other initializations
  initializeNote ();
}

void msrNote::initializeNote ()
{
  // rests handling
  // ------------------------------------------------------
  
  if (fNoteIsARest && fNoteDisplayOctave != K_NO_OCTAVE) {
    // this note is a pitched rest:
    // copy the display octave to the the note octave, // JMI
    // to be used in octave relative code generation
    fNoteQuarterTonesPitchKind = fNoteQuarterTonesDisplayPitchKind;
    fNoteOctave = fNoteDisplayOctave;
  }

  // note accidentals
  // ------------------------------------------------------

  fNoteAccidentalKind =
    msrNote::kNoteAccidentalNone; // default value
  
  fNoteEditorialAccidentalKind =
    msrNote::kNoteEditorialAccidentalNo; // default value
  
  fNoteCautionaryAccidentalKind =
    msrNote::kNoteCautionaryAccidentalNo; // default value
  
  // note context
  // ------------------------------------------------------

  fNoteStaffNumber = 0;
  fNoteVoiceNumber = 0;

  fNoteBelongsToAChord = false;
  fNoteBelongsToATuplet = false;

  // note lyrics
  // ------------------------------------------------------
  
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceNotesDetails) {
    gLogIOstream <<
      endl <<
      "Initializing a note" <<
      ", kind: ";
    if (fNoteKind == k_NoNoteKind)
      gLogIOstream <<
        "not yet known";
    else
      gLogIOstream <<
        noteKindAsString (fNoteKind);
    gLogIOstream <<
      ", line " << fInputLineNumber << ":" <<
      endl;

    gIndenter++;
    
    const int fieldWidth = 30;
    
    gLogIOstream <<
      left <<
        setw (fieldWidth) <<
        "fNoteQuarterTonesPitch" << " = " <<
        msrQuarterTonesPitchKindAsString (
          gMsrOptions->fMsrQuarterTonesPitchesLanguageKind,
          fNoteQuarterTonesPitchKind) <<
        endl;

    gLogIOstream <<
      left <<
        setw (fieldWidth) <<
        "fNoteSoundingWholeNotes" << " = " <<
        fNoteSoundingWholeNotes <<
        endl;
      
    gLogIOstream <<
      left <<
        setw (fieldWidth) <<
        "fNoteDisplayWholeNotes" << " = " <<
        fNoteDisplayWholeNotes <<
        endl;
      
    gLogIOstream <<
      left <<
        setw (fieldWidth) <<
        "fNoteDotsNumber" << " = " <<
        fNoteDotsNumber <<
        endl <<

      left <<
        setw (fieldWidth) <<
        "fNoteGraphicDuration" << " = ";
      if (fNoteGraphicDurationKind != k_NoDuration)
        gLogIOstream <<
          msrDurationKindAsString (
            fNoteGraphicDurationKind);
      else
        gLogIOstream << "none";

    gLogIOstream <<
      endl <<

      left <<
        setw (fieldWidth) <<
        "fNoteOctave" << " = " <<
        fNoteOctave <<
        endl <<

      left <<
        setw (fieldWidth) <<
        "fNoteDisplayOctave" << " = " <<
        fNoteDisplayOctave <<
        endl <<

      left <<
        setw (fieldWidth) <<
        "fNoteIsARest" << " = " <<
        booleanAsString (fNoteIsARest) <<
        endl <<
      left <<
        setw (fieldWidth) <<
        "fNoteIsUnpitched" << " = " <<
         booleanAsString (fNoteIsUnpitched) <<
        endl <<

      left <<
        setw (fieldWidth) <<
        "fNoteIsACueNote" << " = " <<
         booleanAsString (fNoteIsACueNote) <<
        endl <<
      left <<
        setw (fieldWidth) <<
        "fNoteIsAGraceNote" << " = " <<
         booleanAsString (fNoteIsAGraceNote) <<
        endl <<

      left <<
        setw (fieldWidth) <<
        "fNotePrintKind" << " = " <<
         notePrintKindAsString (fNotePrintKind) <<
        endl <<

      left <<
        setw (fieldWidth) <<
        "fNoteHeadKind" << " = " <<
         noteHeadKindAsString (fNoteHeadKind) <<
        endl <<
      left <<
        setw (fieldWidth) <<
        "fNoteHeadFilledKind" << " = " <<
         noteHeadFilledKindAsString (fNoteHeadFilledKind) <<
        endl <<
      left <<
        setw (fieldWidth) <<
        "fNoteHeadParenthesesKind" << " = " <<
         noteHeadParenthesesKindAsString (fNoteHeadParenthesesKind) <<
        endl <<

      left <<
        setw (fieldWidth) <<
        "fNoteAccidentalKind" << " = " <<
        noteAccidentalKindAsString (
          fNoteAccidentalKind) <<
        endl <<
      left <<
        setw (fieldWidth) <<
        "fNoteEditorialAccidentalKind" << " = " <<
        noteEditorialAccidentalKindAsString (
          fNoteEditorialAccidentalKind) <<
        endl <<
      left <<
        setw (fieldWidth) <<
        "fNoteCautionaryAccidentalKind" << " = " <<
        noteCautionaryAccidentalKindAsString (
          fNoteCautionaryAccidentalKind) <<
        endl <<
            
      left <<
        setw (fieldWidth) <<
        "fStaffNumber" << " = " <<
        fNoteStaffNumber <<
        endl <<
      left <<
        setw (fieldWidth) <<
        "fNoteVoiceNumber" << " = " <<
        fNoteVoiceNumber <<
        endl <<

      left <<
        setw (fieldWidth) <<
        "fNoteBelongsToAChord" << " = " <<
         booleanAsString (fNoteBelongsToAChord) <<
        endl <<
            
      left <<
        setw (fieldWidth) <<
        "fNoteBelongsToATuplet" << " = " <<
         booleanAsString (fNoteBelongsToATuplet) <<
        endl <<
      endl;
          
    gIndenter--;

/*
    gLogIOstream <<
      "% <==" <<
      endl << endl;
*/
  }
#endif

  // note measure information
  // ------------------------------------------------------

  fNoteMeasureNumber     = K_NO_MEASURE_NUMBER;
  fNotePositionInMeasure = K_NO_POSITION_MEASURE_NUMBER;
  
  fNoteOccupiesAFullMeasure = false;

  // note redundant information (for speed)
  // ------------------------------------------------------

  fNoteIsStemless = false;
  
  fNoteIsAChordsFirstMemberNote = false;
  
  fNoteIsAChordsFirstMemberNote = false;

  fNoteIsFirstNoteInADoubleTremolo  = false;
  fNoteIsSecondNoteInADoubleTremolo = false;
  
  fNoteIsFollowedByGraceNotes = false;
}

msrNote::~msrNote ()
{}

void msrNote::setNoteSoundingWholeNotes (
  rational wholeNotes)
{
  fNoteSoundingWholeNotes = wholeNotes;

  // is wholeNotes the shortest one in this voice?      
/* JMI
  if (fNoteSoundingWholeNotes < fVoiceShortestNoteDuration) {
    fVoiceShortestNoteDuration = fNoteSoundingWholeNotes;
  }
  */
}

void msrNote::setNoteDisplayWholeNotes (
  rational wholeNotes)
{
  fNoteDisplayWholeNotes = wholeNotes;

/* JMI
  // is wholeNotes the shortest one in this voice?
  if (fNoteDisplayWholeNotes < fVoiceShortestNoteDuration) {
    fVoiceShortestNoteDuration = fNoteDisplayWholeNotes;
  }
  */
}

S_msrNote msrNote::createNoteNewbornClone (
  S_msrPart containingPart)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceNotes) {
    gLogIOstream <<
      "Creating a newborn clone of note " <<
      asString () <<
      " in part " <<
      containingPart->
        getPartCombinedName () << 
      endl;
  }
#endif

  // sanity check
  msrAssert(
    containingPart != nullptr,
    "containingPart is null");
    
  S_msrNote
    newbornClone =
      msrNote::create (
        fInputLineNumber,
        
        fNoteKind,
        
        fNoteQuarterTonesPitchKind,
        
        fNoteSoundingWholeNotes,
        fNoteDisplayWholeNotes,
        
        fNoteDotsNumber,
        
        fNoteGraphicDurationKind,
        
        fNoteOctave,
        
        fNoteQuarterTonesDisplayPitchKind,
        fNoteDisplayOctave,
        
        fNoteIsARest,
        fNoteIsUnpitched,

        fNoteIsACueNote,
        fNoteIsAGraceNote,

        fNotePrintKind,
  
        fNoteHeadKind,
        fNoteHeadFilledKind,
        fNoteHeadParenthesesKind);

  // basic note description
  // ------------------------------------------------------

  newbornClone->fNoteOctaveShift =
    fNoteOctaveShift;

  // note print kind
  // ------------------------------------------------------

  newbornClone->fNotePrintKind =
    fNotePrintKind;

  // note head
  // ------------------------------------------------------

  newbornClone->fNoteHeadKind =
    fNoteHeadKind;
  newbornClone->fNoteHeadFilledKind =
    fNoteHeadFilledKind;
  newbornClone->fNoteHeadParenthesesKind =
    fNoteHeadParenthesesKind;

  // accidentals
  // ------------------------------------------------------

  newbornClone->fNoteAccidentalKind =
    fNoteAccidentalKind;
  
  newbornClone->fNoteEditorialAccidentalKind =
    fNoteEditorialAccidentalKind;
  
  newbornClone->fNoteCautionaryAccidentalKind =
    fNoteCautionaryAccidentalKind;
  
  // staff and voice context
  // ------------------------------------------------------

  newbornClone->
    fNoteStaffNumber =
      fNoteStaffNumber;
  newbornClone->
    fNoteVoiceNumber =
      fNoteVoiceNumber;

  // chord member?
  // ------------------------------------------------------

  newbornClone->
    fNoteBelongsToAChord =
      fNoteBelongsToAChord;

  // tuplet member?
  // ------------------------------------------------------

  newbornClone->
    fNoteBelongsToATuplet =
      fNoteBelongsToATuplet;

  // multiple rest member?
  // ------------------------------------------------------

  newbornClone->
    fNoteBelongsToAMultipleRest =
      fNoteBelongsToAMultipleRest;

  newbornClone->
    fNoteMultipleRestSequenceNumber =
      fNoteMultipleRestSequenceNumber;

  // note lyrics
  // ------------------------------------------------------

  // stem
  // ------------------------------------------------------

  newbornClone->fNoteStem = // JMI
    // share this data
    fNoteStem;

  // beams
  // ------------------------------------------------------

  // articulations
  // ------------------------------------------------------

  // technicals
  // ------------------------------------------------------

  // ornaments
  // ------------------------------------------------------

  // single tremolo
  // ------------------------------------------------------

  // tie
  // ------------------------------------------------------

  newbornClone->fNoteTie = // JMI
    // share this data
    fNoteTie;

  // dynamics
  // ------------------------------------------------------

  // words
  // ------------------------------------------------------

  // slurs
  // ------------------------------------------------------

  // ligatures
  // ------------------------------------------------------

  // harmony
  // ------------------------------------------------------

  newbornClone->fNoteHarmony = // JMI
    fNoteHarmony;

  // frame
  // ------------------------------------------------------

  newbornClone->fNoteFrame = // JMI
    fNoteFrame;

  // figured bass
  // ------------------------------------------------------

  newbornClone->fNoteFiguredBass = // JMI
    fNoteFiguredBass;

  // note measure information
  // ------------------------------------------------------

  newbornClone->
    fNoteMeasureNumber =
      fNoteMeasureNumber;
  newbornClone->
    fNotePositionInMeasure =
      fNotePositionInMeasure;
  newbornClone->
    fNoteOccupiesAFullMeasure =
      fNoteOccupiesAFullMeasure;
    
  // note redundant information (for speed)
  // ------------------------------------------------------

  newbornClone->fNoteIsStemless =
    fNoteIsStemless;

  newbornClone->fNoteIsAChordsFirstMemberNote =
    fNoteIsAChordsFirstMemberNote;

  newbornClone->fNoteIsFirstNoteInADoubleTremolo =
    fNoteIsFirstNoteInADoubleTremolo;
  newbornClone->fNoteIsSecondNoteInADoubleTremolo =
    fNoteIsSecondNoteInADoubleTremolo;

  newbornClone->fNoteTrillOrnament =
    fNoteTrillOrnament;
  newbornClone->fNoteIsFollowedByGraceNotes =
    fNoteIsFollowedByGraceNotes;

  newbornClone->fNoteDashesOrnament =
    fNoteDashesOrnament;

  newbornClone->fNoteWavyLineSpannerStart =
    fNoteWavyLineSpannerStart;
  newbornClone->fNoteWavyLineSpannerStop =
    fNoteWavyLineSpannerStop;

  newbornClone->fNoteDelayedTurnOrnament =
    fNoteDelayedTurnOrnament;
  newbornClone->fNoteDelayedInvertedTurnOrnament =
    fNoteDelayedInvertedTurnOrnament;

  // uplinks
  // ------------------------------------------------------

  /* JMI
    S_msrTuplet           fNoteTupletUplink;

    S_msrMeasure          fNoteMeasureUplink;
*/

  return newbornClone;
}

S_msrNote msrNote::createNoteDeepCopy (
  S_msrVoice containingVoice)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceNotes) {
    gLogIOstream <<
      "Creating a deep copy of note " <<
      asString () <<
      /* JMI
      " in part " <<
      containingPart->
        getPartCombinedName () <<
        */
      endl;
  }
#endif

/* JMI
  // sanity check
  msrAssert(
    containingVoice != nullptr,
    "containingVoice is null");
    */
    
  S_msrNote
    noteDeepCopy =
      msrNote::create (
        fInputLineNumber,
        
        fNoteKind,
        
        fNoteQuarterTonesPitchKind,
        
        fNoteSoundingWholeNotes,
        fNoteDisplayWholeNotes,
        
        fNoteDotsNumber,
        
        fNoteGraphicDurationKind,
        
        fNoteOctave,
        
        fNoteQuarterTonesDisplayPitchKind,
        fNoteDisplayOctave,
        
        fNoteIsARest,
        fNoteIsUnpitched,

        fNoteIsACueNote,
        fNoteIsAGraceNote,
  
        fNotePrintKind,
        
        fNoteHeadKind,
        fNoteHeadFilledKind,
        fNoteHeadParenthesesKind);

  // basic note description
  // ------------------------------------------------------

  noteDeepCopy->fNoteOctaveShift = // JMI
    fNoteOctaveShift;

  // note print kind
  // ------------------------------------------------------

  noteDeepCopy->fNotePrintKind =
    fNotePrintKind;

  // note head
  // ------------------------------------------------------

  noteDeepCopy->fNoteHeadKind =
    fNoteHeadKind;
  noteDeepCopy->fNoteHeadFilledKind =
    fNoteHeadFilledKind;
  noteDeepCopy->fNoteHeadParenthesesKind =
    fNoteHeadParenthesesKind;

  // accidentals
  // ------------------------------------------------------

  noteDeepCopy->fNoteAccidentalKind =
    fNoteAccidentalKind;
  
  noteDeepCopy->fNoteEditorialAccidentalKind =
    fNoteEditorialAccidentalKind;
  
  noteDeepCopy->fNoteCautionaryAccidentalKind =
    fNoteCautionaryAccidentalKind;
  
  // staff and voice context
  // ------------------------------------------------------

  noteDeepCopy->
    fNoteStaffNumber =
      fNoteStaffNumber;
  noteDeepCopy->
    fNoteVoiceNumber =
      fNoteVoiceNumber;

  // chord member?
  // ------------------------------------------------------

  noteDeepCopy->
    fNoteBelongsToAChord =
      fNoteBelongsToAChord;

  // tuplet member?
  // ------------------------------------------------------

  noteDeepCopy->
    fNoteBelongsToATuplet =
      fNoteBelongsToATuplet;

  // multiple rest member?
  // ------------------------------------------------------

  noteDeepCopy->
    fNoteBelongsToAMultipleRest =
      fNoteBelongsToAMultipleRest;

  noteDeepCopy->
    fNoteMultipleRestSequenceNumber =
      fNoteMultipleRestSequenceNumber;

  // note lyrics
  // ------------------------------------------------------

/* JMI
  {
    list<S_msrSyllable>::const_iterator i;
    for (i=fNoteSyllables.begin (); i!=fNoteSyllables.end (); i++) {
      // share this data
      noteDeepCopy->
        fNoteSyllables.push_back ((*i));
    } // for
  }
*/

  // stem
  // ------------------------------------------------------

  noteDeepCopy->fNoteStem = // JMI
    // share this data
    fNoteStem;

  // beams
  // ------------------------------------------------------

  {
    list<S_msrBeam>::const_iterator i;
    for (i=fNoteBeams.begin (); i!=fNoteBeams.end (); i++) {
      // share this data
      noteDeepCopy->
        fNoteBeams.push_back ((*i));
    } // for
  }
  
  // articulations
  // ------------------------------------------------------

  for (
    list<S_msrArticulation>::const_iterator i=
      fNoteArticulations.begin ();
      i!=fNoteArticulations.end ();
      i++) {
    // share this data
    noteDeepCopy->
      fNoteArticulations.push_back ((*i));
  } // for

  // spanners
  // ------------------------------------------------------

  for (
    list<S_msrSpanner>::const_iterator i=
      fNoteSpanners.begin ();
      i!=fNoteSpanners.end ();
      i++) {
    // share this data
    noteDeepCopy->
      fNoteSpanners.push_back ((*i));
  } // for

  // technicals
  // ------------------------------------------------------

  {
    list<S_msrTechnical>::const_iterator i;
    for (
      i=fNoteTechnicals.begin ();
      i!=fNoteTechnicals.end ();
      i++) {
      // share this data
      noteDeepCopy->
        fNoteTechnicals.push_back ((*i));
    } // for
  }
  
  {
    list<S_msrTechnicalWithInteger>::const_iterator i;
    for (
      i=fNoteTechnicalWithIntegers.begin ();
      i!=fNoteTechnicalWithIntegers.end ();
      i++) {
      // share this data
      noteDeepCopy->
        fNoteTechnicalWithIntegers.push_back ((*i));
    } // for
  }
  
  {
    list<S_msrTechnicalWithString>::const_iterator i;
    for (
      i=fNoteTechnicalWithStrings.begin ();
      i!=fNoteTechnicalWithStrings.end ();
      i++) {
      // share this data
      noteDeepCopy->
        fNoteTechnicalWithStrings.push_back ((*i));
    } // for
  }
  
  // ornaments
  // ------------------------------------------------------

  {
    list<S_msrOrnament>::const_iterator i;
    for (i=fNoteOrnaments.begin (); i!=fNoteOrnaments.end (); i++) {
      // share this data
      noteDeepCopy->
        fNoteOrnaments.push_back ((*i));
    } // for
  }
  
  // glissandos
  // ------------------------------------------------------

  {
    list<S_msrGlissando>::const_iterator i;
    for (i=fNoteGlissandos.begin (); i!=fNoteGlissandos.end (); i++) {
      // share this data
      noteDeepCopy->
        fNoteGlissandos.push_back ((*i));
    } // for
  }
  
  // slides
  // ------------------------------------------------------

  {
    list<S_msrSlide>::const_iterator i;
    for (i=fNoteSlides.begin (); i!=fNoteSlides.end (); i++) {
      // share this data
      noteDeepCopy->
        fNoteSlides.push_back ((*i));
    } // for
  }
  
  // single tremolo
  // ------------------------------------------------------

  noteDeepCopy->fNoteSingleTremolo =
    fNoteSingleTremolo;

  // tie
  // ------------------------------------------------------

  noteDeepCopy->fNoteTie = // JMI
    // share this data
    fNoteTie;

  // dynamics
  // ------------------------------------------------------

  {
    list<S_msrDynamics>::const_iterator i;
    for (i=fNoteDynamics.begin (); i!=fNoteDynamics.end (); i++) {
      // share this data
      noteDeepCopy->
        fNoteDynamics.push_back ((*i));
    } // for
  }

  {
    list<S_msrOtherDynamics>::const_iterator i;
    for (i=fNoteOtherDynamics.begin (); i!=fNoteOtherDynamics.end (); i++) {
      // share this data
      noteDeepCopy->
        fNoteOtherDynamics.push_back ((*i));
    } // for
  }
  
  // slashes
  // ------------------------------------------------------

  {
    list<S_msrSlash>::const_iterator i;
    for (i=fNoteSlashes.begin (); i!=fNoteSlashes.end (); i++) {
      // share this data
      noteDeepCopy->
        fNoteSlashes.push_back ((*i));
    } // for
  }
  
  // wedges
  // ------------------------------------------------------

  {
    list<S_msrWedge>::const_iterator i;
    for (i=fNoteWedges.begin (); i!=fNoteWedges.end (); i++) {
      // share this data
      noteDeepCopy->
        fNoteWedges.push_back ((*i));
    } // for
  }
  
  // eyeglasses
  // ------------------------------------------------------

  {
    list<S_msrEyeGlasses>::const_iterator i;
    for (i=fNoteEyeGlasses.begin (); i!=fNoteEyeGlasses.end (); i++) {
      // share this data
      noteDeepCopy->
        fNoteEyeGlasses.push_back ((*i));
    } // for
  }
  
  // damps
  // ------------------------------------------------------

  {
    list<S_msrDamp>::const_iterator i;
    for (i=fNoteDamps.begin (); i!=fNoteDamps.end (); i++) {
      // share this data
      noteDeepCopy->
        fNoteDamps.push_back ((*i));
    } // for
  }
  
  // damp alls
  // ------------------------------------------------------

  {
    list<S_msrDampAll>::const_iterator i;
    for (i=fNoteDampAlls.begin (); i!=fNoteDampAlls.end (); i++) {
      // share this data
      noteDeepCopy->
        fNoteDampAlls.push_back ((*i));
    } // for
  }
  
  // scordaturas
  // ------------------------------------------------------

  {
    list<S_msrScordatura>::const_iterator i;
    for (i=fNoteScordaturas.begin (); i!=fNoteScordaturas.end (); i++) {
      // share this data
      noteDeepCopy->
        fNoteScordaturas.push_back ((*i));
    } // for
  }
  
  // words
  // ------------------------------------------------------

  {
    list<S_msrWords>::const_iterator i;
    for (i=fNoteWords.begin (); i!=fNoteWords.end (); i++) {
      // share this data
      noteDeepCopy->
        fNoteWords.push_back ((*i));
    } // for
  }
  
  // slurs
  // ------------------------------------------------------

  {
    list<S_msrSlur>::const_iterator i;
    for (i=fNoteSlurs.begin (); i!=fNoteSlurs.end (); i++) {
      // share this data
      noteDeepCopy->
        fNoteSlurs.push_back ((*i));
    } // for
  }
  
  // ligatures
  // ------------------------------------------------------

  { 
    list<S_msrLigature>::const_iterator i;
    for (i=fNoteLigatures.begin (); i!=fNoteLigatures.end (); i++) {
      // share this data
      noteDeepCopy->
        fNoteLigatures.push_back ((*i));
    } // for
  }
  
  // pedals
  // ------------------------------------------------------

  { 
    list<S_msrPedal>::const_iterator i;
    for (i=fNotePedals.begin (); i!=fNotePedals.end (); i++) {
      // share this data
      noteDeepCopy->
        fNotePedals.push_back ((*i));
    } // for
  }

  // harmony
  // ------------------------------------------------------

  if (fNoteHarmony) {
    noteDeepCopy->fNoteHarmony =
      fNoteHarmony->
        createHarmonyDeepCopy (
          containingVoice);
  }

  // harmony
  // ------------------------------------------------------

  if (fNoteFrame) {
    noteDeepCopy->fNoteFrame =
      fNoteFrame;
  }

  // figured bass
  // ------------------------------------------------------

  if (fNoteFiguredBass) {
    noteDeepCopy->fNoteFiguredBass =
      fNoteFiguredBass->
        createFiguredBassDeepCopy (
          containingVoice->fetchVoicePartUplink ()); // JMI
  }

  // note measure information
  // ------------------------------------------------------

  noteDeepCopy->
    fNoteMeasureNumber =
      fNoteMeasureNumber;
  noteDeepCopy->
    fNotePositionInMeasure =
      fNotePositionInMeasure;
      
  noteDeepCopy->
    fNoteOccupiesAFullMeasure =
      fNoteOccupiesAFullMeasure;

  // note redundant information (for speed)
  // ------------------------------------------------------

  noteDeepCopy->fNoteIsStemless =
    fNoteIsStemless;

  noteDeepCopy->fNoteIsAChordsFirstMemberNote =
    fNoteIsAChordsFirstMemberNote;

  noteDeepCopy->fNoteIsFirstNoteInADoubleTremolo =
    fNoteIsFirstNoteInADoubleTremolo;
  noteDeepCopy->fNoteIsSecondNoteInADoubleTremolo =
    fNoteIsSecondNoteInADoubleTremolo;

  noteDeepCopy->fNoteTrillOrnament =
    fNoteTrillOrnament;
  noteDeepCopy->fNoteIsFollowedByGraceNotes =
    fNoteIsFollowedByGraceNotes;

  noteDeepCopy->fNoteDashesOrnament =
    fNoteDashesOrnament;

  noteDeepCopy->fNoteWavyLineSpannerStart =
    fNoteWavyLineSpannerStart;
  noteDeepCopy->fNoteWavyLineSpannerStop =
    fNoteWavyLineSpannerStop;

  noteDeepCopy->fNoteDelayedTurnOrnament =
    fNoteDelayedTurnOrnament;
  noteDeepCopy->fNoteDelayedInvertedTurnOrnament =
    fNoteDelayedInvertedTurnOrnament;

  // uplinks
  // ------------------------------------------------------

  /* JMI
  
    S_msrTuplet           fNoteTupletUplink;

    S_msrMeasure          fNoteMeasureUplink;
*/

  return noteDeepCopy;
}

S_msrNote msrNote::createRestNote (
  int       inputLineNumber,
  rational  soundingWholeNotes,
  rational  displayWholeNotes,
  int       dotsNumber,
  int       staffNumber,
  int       voiceNumber)
{    
  msrNote * o =
    new msrNote (
      inputLineNumber,
      
      kRestNote, // noteKind
      
      k_NoQuarterTonesPitch_QTP,
      
      soundingWholeNotes,
      displayWholeNotes,
      
      dotsNumber,
      
      k_NoDuration, // noteGraphicDuration
      
      K_NO_OCTAVE, // noteOctave,
      
      k_NoQuarterTonesPitch_QTP, // noteDisplayQuarterTonesPitch
      K_NO_OCTAVE, // noteDisplayOctave,
      
      false, // noteIsARest
      false, // noteIsUnpitched
      
      false, // noteIsACueNote
      false, // noteIsAGraceNote

      kNotePrintYes, // JMI

      kNoteHeadNormal, // JMI
      kNoteHeadFilledYes, // JMI
      kNoteHeadParenthesesNo); // JMI
  assert(o!=0);
  
  return o;
}    

S_msrNote msrNote::createSkipNote (
  int       inputLineNumber,
  rational  soundingWholeNotes,
  rational  displayWholeNotes,
  int       dotsNumber,
  int       staffNumber,
  int       voiceNumber)
{    
  msrNote * o =
    new msrNote (
      inputLineNumber,
      
      kSkipNote, // noteKind
      
      k_NoQuarterTonesPitch_QTP,
      
      soundingWholeNotes,
      displayWholeNotes,
      
      dotsNumber,
      
      k_NoDuration, // noteGraphicDuration
      
      K_NO_OCTAVE, // noteOctave,
      
      k_NoQuarterTonesPitch_QTP, // noteDisplayQuarterTonesPitch
      K_NO_OCTAVE, // noteDisplayOctave,
      
      false, // noteIsARest
      false, // noteIsUnpitched
      
      false, // noteIsACueNote
      false, // noteIsAGraceNote

      kNotePrintYes, // JMI

      kNoteHeadNormal, // JMI
      kNoteHeadFilledYes, // JMI
      kNoteHeadParenthesesNo); // JMI
  assert(o!=0);
  
  return o;
}    

void msrNote::setNoteOccupiesAFullMeasure ()
{
  fNoteOccupiesAFullMeasure = true;

  if (fNoteIsARest) {
    // the measure it belongs to is a single-measure rest
    fNoteMeasureUplink->
      setMeasureIsASingleMeasureRest ();
  }
}

string msrNote::noteSoundingWholeNotesAsMsrString () const
{
  string result;

  if (fNoteSoundingWholeNotes.getNumerator () == 0)
    result = " (no sounding whole notes)";
  else
    result =
      wholeNotesAsMsrString (
        fInputLineNumber,
        fNoteSoundingWholeNotes);

  return result;
  }

string msrNote::noteDisplayWholeNotesAsMsrString () const
{
  string result;

  if (fNoteDisplayWholeNotes.getNumerator () == 0) {
    result = "(no display whole notes)";
  }
  else {
    result =
      wholeNotesAsMsrString (
        fInputLineNumber,
        fNoteDisplayWholeNotes);
  }

  return result;
}

string msrNote::noteKindAsString (
  msrNoteKind noteKind)
{
  string result;
  
  switch (noteKind) {
    case msrNote::k_NoNoteKind:
      result = "noNoteKind???";
      break;
      
    case msrNote::kRestNote:
      result = "restNote";
      break;
      
    case msrNote::kSkipNote:
      result = "skipNote";
      break;
      
    case msrNote::kUnpitchedNote:
      result = "unpitchedNote";
      break;
      
    case msrNote::kStandaloneNote:
      result = "standaloneNote";
      break;
      
    case msrNote::kDoubleTremoloMemberNote:
      result = "doubleTremoloMemberNote";
      break;
      
    case msrNote::kGraceNote:
      result = "graceNote";
      break;
      
    case msrNote::kGraceChordMemberNote:
      result = "graceChordMemberNote";
      break;
      
    case msrNote::kChordMemberNote:
      result = "chordMemberNote";
      break;
      
    case msrNote::kTupletMemberNote:
      result = "tupletMemberNote";
      break;
      
    case msrNote::kTupletMemberUnpitchedNote:
      result = "tupletMemberUnpitchedNote";
      break;
  } // switch

  return result;
}

// accidentals
//______________________________________________________________________________
string msrNote::noteAccidentalKindAsString (
  msrNoteAccidentalKind noteAccidentalKind)
{
  string result;
  
  switch (noteAccidentalKind) {
    case kNoteAccidentalNone:
      result = "noteAccidentalNone";
      break;
      
    case kNoteAccidentalSharp:
      result = "noteAccidentalSharp";
      break;
    case kNoteAccidentalNatural:
      result = "noteAccidentalNatural";
      break;
    case kNoteAccidentalFlat:
      result = "noteAccidentalFlat";
      break;
    case kNoteAccidentaldoubleSharp:
      result = "noteAccidentaldoubleSharp";
      break;
    case kNoteAccidentalSharpSharp:
      result = "noteAccidentalSharpSharp";
      break;
    case kNoteAccidentalFlatFlat:
      result = "noteAccidentalFlatFlat";
      break;
    case kNoteAccidentalNaturalSharp:
      result = "noteAccidentalNaturalSharp";
      break;
    case kNoteAccidentalNaturalFlat:
      result = "noteAccidentalNaturalFlat";
      break;
    case kNoteAccidentalQuarterFlat:
      result = "noteAccidentalQuarterFlat";
      break;
    case kNoteAccidentalQuarterSharp:
      result = "noteAccidentalQuarterSharp";
      break;
    case kNoteAccidentalThreeQuartersFlat:
      result = "noteAccidentalThreeQuartersFlat";
      break;
    case kNoteAccidentalThreeQuartersSharp:
      result = "noteAccidentalThreeQuartersSharp";
      break;
      
    case kNoteAccidentalSharpDown:
      result = "noteAccidentalSharpDown";
      break;
    case kNoteAccidentalSharpUp:
      result = "noteAccidentalSharpUp";
      break;
    case kNoteAccidentalNaturalDown:
      result = "noteAccidentalNaturalDown";
      break;
    case kNoteAccidentalNaturalUp:
      result = "noteAccidentalNaturalUp";
      break;
    case kNoteAccidentalFlatDown:
      result = "noteAccidentalFlatDown";
      break;
    case kNoteAccidentalFlatUp:
      result = "noteAccidentalFlatUp";
      break;
    case kNoteAccidentalTripleSharp:
      result = "noteAccidentalTripleSharp";
      break;
    case kNoteAccidentalTripleFlat:
      result = "noteAccidentalTripleFlat";
      break;
    case kNoteAccidentalSlashQuarterSharp:
      result = "noteAccidentalSlashQuarterSharp";
      break;
    case kNoteAccidentalSlashSharp:
      result = "noteAccidentalSlashSharp";
      break;
    case kNoteAccidentalSlashFlat:
      result = "noteAccidentalSlashFlat";
      break;
    case kNoteAccidentaldoubleSlashFlat:
      result = "noteAccidentaldoubleSlashFlat";
      break;
    case kNoteAccidentalSharp_1:
      result = "noteAccidentalSharp_1";
      break;
    case kNoteAccidentalSharp_2:
      result = "noteAccidentalSharp_2";
      break;
    case kNoteAccidentalSharp_3:
      result = "noteAccidentalSharp_3";
      break;
    case kNoteAccidentalSharp_5:
      result = "noteAccidentalSharp_5";
      break;
    case kNoteAccidentalFlat_1:
      result = "noteAccidentalFlat_1";
      break;
    case kNoteAccidentalFlat_2:
      result = "noteAccidentalFlat_2";
      break;
    case kNoteAccidentalFlat_3:
      result = "noteAccidentalFlat_3";
      break;
    case kNoteAccidentalFlat_4:
      result = "noteAccidentalFlat_4";
      break;
    case kNoteAccidentalSori:
      result = "noteAccidentalSori";
      break;
    case kNoteAccidentalKoron:
      result = "noteAccidentalKoron";
      break;
  } // switch

  return result;
}

string msrNote::noteEditorialAccidentalKindAsString (
  msrNoteEditorialAccidentalKind noteEditorialAccidentalKind)
{
  string result;
  
  switch (noteEditorialAccidentalKind) {
    case msrNote::kNoteEditorialAccidentalYes:
      result = "noteEditorialAccidentalYes";
      break;
    case msrNote::kNoteEditorialAccidentalNo:
      result = "noteEditorialAccidentalNo";
      break;
  } // switch

  return result;
}

string msrNote::noteCautionaryAccidentalKindAsString (
  msrNoteCautionaryAccidentalKind noteCautionaryAccidentalKind)
{
  string result;
  
  switch (noteCautionaryAccidentalKind) {
    case msrNote::kNoteCautionaryAccidentalYes:
      result = "noteCautionaryAccidentalYes";
      break;
    case msrNote::kNoteCautionaryAccidentalNo:
      result = "noteCautionaryAccidentalNo";
      break;
  } // switch

  return result;
}

string msrNote::notePrintKindAsString (
  msrNotePrintKind notePrintKind)
{
  string result;
  
  switch (notePrintKind) {
    case msrNote::kNotePrintYes:
      result = "notePrintYes";
      break;
    case msrNote::kNotePrintNo:
      result = "notePrintNo";
      break;
  } // switch

  return result;
}

string msrNote::noteHeadKindAsString (
  msrNoteHeadKind noteHeadKind)
{
  string result;
 
  switch (noteHeadKind) {
    case msrNote::kNoteHeadSlash:
      result = "noteHeadSlash";
      break;
    case msrNote::kNoteHeadTriangle:
      result = "noteHeadTriangle";
      break;
    case msrNote::kNoteHeadDiamond:
      result = "noteHeadDiamond";
      break;
    case msrNote::kNoteHeadSquare:
      result = "noteHeadSquare";
      break;
    case msrNote::kNoteHeadCross:
      result = "noteHeadCross";
      break;
    case msrNote::kNoteHeadX:
      result = "noteHeadX";
      break;
    case msrNote::kNoteHeadCircleX:
      result = "noteHeadCircleX";
      break;
    case msrNote::kNoteHeadInvertedTriangle:
      result = "noteHeadInvertedTriangle";
      break;
    case msrNote::kNoteHeadArrowDown:
      result = "noteHeadArrowDown";
      break;
    case msrNote::kNoteHeadArrowUp:
      result = "noteHeadArrowUp";
      break;
    case msrNote::kNoteHeadSlashed:
      result = "noteHeadSlashed";
      break;
    case msrNote::kNoteHeadBackSlashed:
      result = "noteHeadBackSlashed";
      break;
    case msrNote::kNoteHeadNormal:
      result = "noteHeadNormal";
      break;
    case msrNote::kNoteHeadCluster:
      result = "noteHeadCluster";
      break;
    case msrNote::kNoteHeadCircleDot:
      result = "noteHeadCircleDot";
      break;
    case msrNote::kNoteHeadLeftTriangle:
      result = "noteHeadLeftTriangle";
      break;
    case msrNote::kNoteHeadRectangle:
      result = "noteHeadRectangle";
      break;
    case msrNote::kNoteHeadNone:
      result = "noteHeadNone";
      break;
    case msrNote::kNoteHeadDo:
      result = "noteHeadDo";
      break;
    case msrNote::kNoteHeadRe:
      result = "noteHeadRe";
      break;
    case msrNote::kNoteHeadMi:
      result = "noteHeadMi";
      break;
    case msrNote::kNoteHeadFa:
      result = "noteHeadFa";
      break;
    case msrNote::kNoteHeadFaUp:
      result = "noteHeadFaUp";
      break;
    case msrNote::kNoteHeadSo:
      result = "noteHeadSo";
      break;
    case msrNote::kNoteHeadLa:
      result = "noteHeadLa";
      break;
    case msrNote::kNoteHeadTi:
      result = "noteHeadTi";
      break;
  } // switch

  return result;
}

string msrNote::noteHeadFilledKindAsString (
  msrNoteHeadFilledKind noteHeadFilledKind)
{
  string result;
 
  switch (noteHeadFilledKind) {
    case msrNote::kNoteHeadFilledYes:
      result = "noteHeadFilledYes";
      break;
    case msrNote::kNoteHeadFilledNo:
      result = "noteHeadFilledNo";
      break;
  } // switch

  return result;
}

string msrNote::noteHeadParenthesesKindAsString (
  msrNoteHeadParenthesesKind noteHeadParenthesesKind)
{
  string result;
 
  switch (noteHeadParenthesesKind) {
    case msrNote::kNoteHeadParenthesesYes:
      result = "noteHeadParenthesesYes";
      break;
    case msrNote::kNoteHeadParenthesesNo:
      result = "noteHeadParenthesesNo";
      break;
  } // switch

  return result;
}

string msrNote::notePrintKindAsString () const
{
  return
    notePrintKindAsString (
      fNotePrintKind);
}

string msrNote::noteHeadKindAsString () const
{
  return
    noteHeadKindAsString (
      fNoteHeadKind);
}

string msrNote::noteHeadFilledKindAsString () const
{
  return
    noteHeadFilledKindAsString (
      fNoteHeadFilledKind);
}

string msrNote::noteHeadParenthesesKindAsString () const
{
  return
    noteHeadParenthesesKindAsString (
      fNoteHeadParenthesesKind);
}

msrDiatonicPitchKind msrNote::noteDiatonicPitchKind (
  int inputLineNumber) const
{
  return
    diatonicPitchKindFromQuarterTonesPitchKind (
      inputLineNumber,
      fNoteQuarterTonesPitchKind);
}

string msrNote::noteDisplayOctaveAsString () const
{
  return
    fNoteDisplayOctave == K_NO_OCTAVE
      ? "no display octave"
      : to_string (fNoteDisplayOctave);
}

bool msrNote::noteIsAPitchedRest () const
{
  return
    fNoteIsARest && fNoteDisplayOctave != K_NO_OCTAVE;
}

void msrNote::setNoteStem (S_msrStem stem)
{
  // register note stem
  fNoteStem = stem;

  // mark note as stemless if relevant
  if (stem->getStemKind () == msrStem::kStemNone)
    fNoteIsStemless = true;
}

void msrNote::setNoteBelongsToAChord ()
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceChords) {
    gLogIOstream <<
      "Setting note '" <<
      asShortStringWithRawWholeNotes () <<
      "' to belong to a chord"
      ", line " << fInputLineNumber <<
      endl;
  }
#endif

  fNoteBelongsToAChord = true;
  fNoteKind = msrNote::kChordMemberNote;
}

void msrNote::determineTupletMemberSoundingFromDisplayWholeNotes (
  int actualNotes, int normalNotes)
{
  /*
  Type indicates the graphic note type, Valid values (from shortest to longest) are 1024th, 512th, 256th, 128th, 64th, 32nd, 16th, eighth, quarter, half, whole, breve, long, and maxima. The size attribute indicates full, cue, or large size, with full the default for regular notes and cue the default for cue and grace notes.

  Quarter note in a triplet, sounds 2/3 of a quarter:
      <note>
        <pitch>
          <step>B</step>
          <octave>4</octave>
        </pitch>
        <duration>20</duration>
        <voice>1</voice>
        <type>quarter</type>
        <time-modification>
          <actual-notes>3</actual-notes>
          <normal-notes>2</normal-notes>
        </time-modification>
        <notations>
          <tuplet number="1" type="start" />
        </notations>
      </note>
  */

#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceTuplets || gTraceOptions->fTraceNotes) {
    gLogIOstream <<
      "Determining tuplet sounding from display whole notes" <<
      " for note '" <<
      asShortString () <<
      ", factor is '" <<
      actualNotes << "/" << normalNotes <<
      "', line " << fInputLineNumber <<
      endl;
  }
#endif

  // the display whole notes are known, compute the sounding ones
  fNoteSoundingWholeNotes =
    fNoteDisplayWholeNotes
      *
    normalNotes
      /
    actualNotes;

  fNoteSoundingWholeNotes.rationalise ();

#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceTuplets || gTraceOptions->fTraceNotes) {
    gLogIOstream <<
      "The result is: '" <<
      asShortString () <<
      endl;
  }
#endif
}

void msrNote::appendBeamToNote (S_msrBeam beam)
{
  fNoteBeams.push_back (beam);
}

void msrNote::appendArticulationToNote (S_msrArticulation art)
{
  fNoteArticulations.push_back (art);
}

void msrNote::appendSpannerToNote (S_msrSpanner spanner)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceSpanners || gTraceOptions->fTraceNotes) {
    gLogIOstream <<
      "Adding spanner '" <<
      spanner->spannerKindAsString () <<
      "' to note '" <<
      asShortString () <<
      "'" <<
      endl;
  }
#endif

  // register note has having a wavy line start
  switch (spanner->getSpannerKind ()) {
    case msrSpanner::kSpannerDashes:
      break;
      
    case msrSpanner::kSpannerWavyLine:
      switch (spanner->getSpannerTypeKind ()) {
        case kSpannerTypeStart:
          fNoteWavyLineSpannerStart = spanner;
          break;
        case kSpannerTypeStop:
          fNoteWavyLineSpannerStop = spanner;
          break;
        case kSpannerTypeContinue:
          break;
        case k_NoSpannerType:
          // JMI ???
          break;
      } // switch
      break;
  } // switch

  // append spanner to note spanners
  fNoteSpanners.push_back (spanner);
}

void msrNote::appendTechnicalToNote (S_msrTechnical technical)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceNotes || gTraceOptions->fTraceTechnicals) {
    gLogIOstream <<
      "Adding technical '" <<
      technical->asString () <<
      "' to note '" << asShortString () <<
      "', line " << fInputLineNumber <<
      endl;
  }
#endif
  
  // append the technical to the note technicals list
  fNoteTechnicals.push_back (technical);
}

void msrNote::appendTechnicalWithIntegerToNote (
  S_msrTechnicalWithInteger technicalWithInteger)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceNotes || gTraceOptions->fTraceTechnicals) {
    gLogIOstream <<
      "Adding technical with integer '" <<
      technicalWithInteger->asString () <<
      "' to note '" << asShortString () <<
      "', line " << fInputLineNumber <<
      endl;
  }
#endif

  // append the technical with integer to the note technicals with integers list
  fNoteTechnicalWithIntegers.push_back (
    technicalWithInteger);
}

void msrNote::appendTechnicalWithFloatToNote (
  S_msrTechnicalWithFloat technicalWithFloat)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceNotes || gTraceOptions->fTraceTechnicals) {
    gLogIOstream <<
      "Adding technical with float '" <<
      technicalWithFloat->asString () <<
      "' to note '" << asShortString () <<
      "', line " << fInputLineNumber <<
      endl;
  }
#endif

  // append the technical with float to the note technicals with floats list
  fNoteTechnicalWithFloats.push_back (
    technicalWithFloat);
}

void msrNote::appendTechnicalWithStringToNote (
  S_msrTechnicalWithString technicalWithString)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceNotes || gTraceOptions->fTraceTechnicals) {
    gLogIOstream <<
      "Adding technical with string'" <<
      technicalWithString->asString () <<
      "' to note '" << asShortString () <<
      "', line " << fInputLineNumber <<
      endl;
  }
#endif

  // append the technical with string to the note technicals with strings list
  fNoteTechnicalWithStrings.push_back (
    technicalWithString);
}

void msrNote::appendOrnamentToNote (S_msrOrnament ornament)
{
  // append the ornament to the note ornaments list
  fNoteOrnaments.push_back (ornament);

  switch (ornament->getOrnamentKind ()) {
    case msrOrnament::kOrnamentTrill:
      fNoteTrillOrnament = ornament;
      break;

    case msrOrnament::kOrnamentDashes:
      fNoteDashesOrnament = ornament;
      break;

    case msrOrnament::kOrnamentDelayedTurn:
      fNoteDelayedTurnOrnament = ornament;
      break;

    case msrOrnament::kOrnamentDelayedInvertedTurn:
      fNoteDelayedInvertedTurnOrnament = ornament;
      break;

    default:
      {}
  } // switch

  // set ornament's note uplink
  ornament->
    setOrnamentNoteUplink (this);
}

void msrNote::appendGlissandoToNote (S_msrGlissando glissando)

{
  // append the glissando to the note glissandos list
  fNoteGlissandos.push_back (glissando);
}

void msrNote::appendSlideToNote (S_msrSlide slide)

{
  // append the slide to the note glissandos list
  fNoteSlides.push_back (slide);
}

void msrNote::setNoteSingleTremolo (S_msrSingleTremolo trem)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceTremolos) {
    gLogIOstream <<
      "Adding singleTremolo '" << trem->asString () <<
      "' to note '" << asShortString () <<
      "', line " << trem->getInputLineNumber () <<
      endl;
  }
#endif

  // register the singleTremolo in the note
  fNoteSingleTremolo = trem;
}

void msrNote::appendDynamicsToNote (S_msrDynamics dynamics)
{
  fNoteDynamics.push_back (dynamics);
}
void msrNote::appendOtherDynamicsToNote (S_msrOtherDynamics otherDynamics)
{
  fNoteOtherDynamics.push_back (otherDynamics);
}

void msrNote::appendWordsToNote (S_msrWords words)
{
  fNoteWords.push_back (words);
}

void msrNote::appendSlurToNote (S_msrSlur slur)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceSlurs) {
    gLogIOstream <<
      "Adding slur '" << slur <<
      "' to note '" << asString () << "'" <<
      endl;
  }
#endif

  fNoteSlurs.push_back (slur);
}

void msrNote::appendLigatureToNote (S_msrLigature ligature)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceLigatures) {
    gLogIOstream <<
      "Appending ligature " << ligature << " to note " << asString () <<
       endl;
  }
#endif

  if (fNoteLigatures.size ()) {
    if (
      fNoteLigatures.back ()->getLigatureKind () == msrLigature::kLigatureStart
        &&
      ligature->getLigatureKind () == msrLigature::kLigatureStop
        &&
      fNoteLigatures.back ()->getLigatureNumber () == ligature->getLigatureNumber ()
      ) {
      // it may happen that a given note has a 'ligature start'
      // and a 'ligature stop' in sequence, ignore both

      stringstream s;
      
      s <<
        "a 'ligature start' is immediately followed by a 'ligature stop'" <<
        endl <<
        "with the same number, ignoring both of them at line " <<
        ligature->getInputLineNumber ();
        
      msrMusicXMLWarning (
        gXml2lyOptions->fInputSourceName,
        ligature->getInputLineNumber (),
        s.str ());
        
      // rmeove 'ligature start'
      fNoteLigatures.pop_back ();

      // don't register 'ligature stop'
    }

    else
      fNoteLigatures.push_back (ligature);
  }

  else
    fNoteLigatures.push_back (ligature);
}

void msrNote::appendPedalToNote (S_msrPedal pedal)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTracePedals) {
    gLogIOstream <<
      "Appending pedal " << pedal << " to note " << asString () <<
       endl;
  }
#endif

  if (fNotePedals.size ()) {
    if (
      fNotePedals.back ()->getPedalTypeKind () == msrPedal::kPedalStart
        &&
      pedal->getPedalTypeKind () == msrPedal::kPedalStop
      ) {
      // it may happen that a given note has a 'pedal start'
      // and a 'pedal stop' in sequence, ignore both            // JMI ???

      stringstream s;
      
      s <<
        "a 'pedal start' is immediately followed by a 'pedal stop'" <<
        endl <<
        "with the same number, ignoring both of them at line " <<
        pedal->getInputLineNumber ();
        
      msrMusicXMLWarning (
        gXml2lyOptions->fInputSourceName,
        pedal->getInputLineNumber (),
        s.str ());
        
      // rmeove 'pedal start'
      fNotePedals.pop_back ();

      // don't register 'pedal stop'
    }

    else
      fNotePedals.push_back (pedal);
  }

  else
    fNotePedals.push_back (pedal);
}

void msrNote::appendSlashToNote (S_msrSlash slash)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceSlashes) {
    gLogIOstream <<
      "Appending slash '" <<
      slash <<
      "' to note '" <<
      asShortString () <<
      "'" <<
      endl;
  }
#endif

  fNoteSlashes.push_back (slash);
}

void msrNote::appendWedgeToNote (S_msrWedge wedge)
{
  fNoteWedges.push_back (wedge);
}

void msrNote::appendEyeGlassesToNote (S_msrEyeGlasses eyeGlasses)
{
  fNoteEyeGlasses.push_back (eyeGlasses);
}

void msrNote::appendDampToNote (S_msrDamp damp)
{
  fNoteDamps.push_back (damp);
}

void msrNote::appendDampAllToNote (S_msrDampAll dampAll)
{
  fNoteDampAlls.push_back (dampAll);
}

void msrNote::appendScordaturaToNote (S_msrScordatura scordatura)
{
  fNoteScordaturas.push_back (scordatura);
}

S_msrDynamics msrNote::removeFirstDynamics () // JMI
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceDynamics) {
    gLogIOstream <<
      "Removing first dynamics from note '" <<
      asShortString () <<
      "'" <<
      endl;
  }
#endif

  // sanity check
  msrAssert (
    fNoteDynamics.size () > 0,
    "fNoteDynamics is empty");

  S_msrDynamics dyn = fNoteDynamics.front ();
  fNoteDynamics.pop_front ();
  
  return dyn;
}

S_msrWedge msrNote::removeFirstWedge () // JMI
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceDynamics) {
    gLogIOstream <<
      "Removing first wedge from note '" <<
      asShortString () <<
      endl;
  }
#endif

  // sanity check
  msrAssert (
    fNoteDynamics.size () > 0,
    "fNoteDynamics is empty");

  S_msrWedge wedge = fNoteWedges.front ();
  fNoteWedges.pop_front ();
  return wedge;
}

void msrNote::appendSyllableToNote (S_msrSyllable syllable)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceLyrics) {
    gLogIOstream <<
      "Appending syllable '" <<
      syllable->asString () <<
      "' to note '" << asString () <<
      "'" <<
      endl;
  }
#endif
  
  fNoteSyllables.push_back (syllable);
}

void msrNote::setNoteHarmony (S_msrHarmony harmony)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceNotes || gTraceOptions->fTraceHarmonies) {
    gLogIOstream <<
      "Setting note '" << asShortString ()  << "'" <<
      " harmony to '" << harmony->asString () << "'" <<
      ", line " << fInputLineNumber <<
      endl;
  }
#endif
  
  fNoteHarmony = harmony;

  // register this note as the harmony uplink
  harmony->
    setHarmonyNoteUplink (this);
}

void msrNote::setNoteFrame (S_msrFrame frame)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceNotes || gTraceOptions->fTraceHarmonies) {
    gLogIOstream <<
      "Setting note '" << asShortString ()  << "'" <<
      " frame to '" << frame->asString () << "'" <<
      endl;
  }
#endif
  
  fNoteFrame = frame;
}

void msrNote::setNoteFiguredBass (S_msrFiguredBass figuredBass)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceNotes || gTraceOptions->fTraceHarmonies) {
    gLogIOstream <<
      "Setting note '" << asShortString ()  << "'" <<
      " figured bass to '" << figuredBass->asString () << "'" <<
      endl;
  }
#endif
      
  fNoteFiguredBass = figuredBass;
}

void msrNote::acceptIn (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrNote::acceptIn ()" <<
      endl;
  }
      
  if (visitor<S_msrNote>*
    p =
      dynamic_cast<visitor<S_msrNote>*> (v)) {
        S_msrNote elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrNote::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrNote::acceptOut (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrNote::acceptOut ()" <<
      endl;
  }

  if (visitor<S_msrNote>*
    p =
      dynamic_cast<visitor<S_msrNote>*> (v)) {
        S_msrNote elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrNote::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrNote::browseData (basevisitor* v)
{
  if (fNoteOctaveShift) {
    // browse the octave shift
    msrBrowser<msrOctaveShift> browser (v);
    browser.browse (*fNoteOctaveShift);
  }

  if (fNoteStem) {
    // browse the stem
    msrBrowser<msrStem> browser (v);
    browser.browse (*fNoteStem);
  }

  // browse the beams if any
  if (fNoteBeams.size ()) {
    gIndenter++;
    list<S_msrBeam>::const_iterator i;
    for (i=fNoteBeams.begin (); i!=fNoteBeams.end (); i++) {
      // browse the beam
      msrBrowser<msrBeam> browser (v);
      browser.browse (*(*i));
    } // for
    gIndenter--;
  }
  
  // browse the articulations if any
  if (fNoteArticulations.size ()) {
    gIndenter++;
    list<S_msrArticulation>::const_iterator i;
    for (i=fNoteArticulations.begin (); i!=fNoteArticulations.end (); i++) {
      // browse the articulation
      msrBrowser<msrArticulation> browser (v);
      browser.browse (*(*i));
    } // for
    gIndenter--;
  }
  
  // browse the spanners if any
  if (fNoteSpanners.size ()) {
    gIndenter++;
    list<S_msrSpanner>::const_iterator i;
    for (i=fNoteSpanners.begin (); i!=fNoteSpanners.end (); i++) {
      // browse the spanner
      msrBrowser<msrSpanner> browser (v);
      browser.browse (*(*i));
    } // for
    gIndenter--;
  }
  
  // browse the technicals if any
  if (fNoteTechnicals.size ()) {
    gIndenter++;
    list<S_msrTechnical>::const_iterator i;
    for (
      i=fNoteTechnicals.begin ();
      i!=fNoteTechnicals.end ();
      i++) {
      // browse the technical
      msrBrowser<msrTechnical> browser (v);
      browser.browse (*(*i));
    } // for
    gIndenter--;
  }
  
  // browse the technicals with integer if any
  if (fNoteTechnicalWithIntegers.size ()) {
    gIndenter++;
    list<S_msrTechnicalWithInteger>::const_iterator i;
    for (
      i=fNoteTechnicalWithIntegers.begin ();
      i!=fNoteTechnicalWithIntegers.end ();
      i++) {
      // browse the technical
      msrBrowser<msrTechnicalWithInteger> browser (v);
      browser.browse (*(*i));
    } // for
    gIndenter--;
  }
  
  // browse the technicals with float if any
  if (fNoteTechnicalWithFloats.size ()) {
    gIndenter++;
    list<S_msrTechnicalWithFloat>::const_iterator i;
    for (
      i=fNoteTechnicalWithFloats.begin ();
      i!=fNoteTechnicalWithFloats.end ();
      i++) {
      // browse the technical
      msrBrowser<msrTechnicalWithFloat> browser (v);
      browser.browse (*(*i));
    } // for
    gIndenter--;
  }
  
  // browse the technicals with string if any
  if (fNoteTechnicalWithStrings.size ()) {
    gIndenter++;
    list<S_msrTechnicalWithString>::const_iterator i;
    for (
      i=fNoteTechnicalWithStrings.begin ();
      i!=fNoteTechnicalWithStrings.end ();
      i++) {
      // browse the technical
      msrBrowser<msrTechnicalWithString> browser (v);
      browser.browse (*(*i));
    } // for
    gIndenter--;
  }
  
  // browse the ornaments if any
  if (fNoteOrnaments.size ()) {
    gIndenter++;
    list<S_msrOrnament>::const_iterator i;
    for (i=fNoteOrnaments.begin (); i!=fNoteOrnaments.end (); i++) {
      // browse the ornament
      msrBrowser<msrOrnament> browser (v);
      browser.browse (*(*i));
    } // for
    gIndenter--;
  }
  
  // browse the glissandos if any
  if (fNoteGlissandos.size ()) {
    gIndenter++;
    list<S_msrGlissando>::const_iterator i;
    for (i=fNoteGlissandos.begin (); i!=fNoteGlissandos.end (); i++) {
      // browse the glissando
      msrBrowser<msrGlissando> browser (v);
      browser.browse (*(*i));
    } // for
    gIndenter--;
  }
  
  // browse the slides if any
  if (fNoteSlides.size ()) {
    gIndenter++;
    list<S_msrSlide>::const_iterator i;
    for (i=fNoteSlides.begin (); i!=fNoteSlides.end (); i++) {
      // browse the glissando
      msrBrowser<msrSlide> browser (v);
      browser.browse (*(*i));
    } // for
    gIndenter--;
  }
  
  if (fNoteSingleTremolo) {
    // browse the singleTremolo
    msrBrowser<msrSingleTremolo> browser (v);
    browser.browse (*fNoteSingleTremolo);
  }
  
  if (fNoteTie) {
    // browse the tie
    msrBrowser<msrTie> browser (v);
    browser.browse (*fNoteTie);
  }

  // browse the dynamics if any
  if (fNoteDynamics.size ()) {
    gIndenter++;
    list<S_msrDynamics>::const_iterator i;
    for (i=fNoteDynamics.begin (); i!=fNoteDynamics.end (); i++) {
      // browse the dynamics
      msrBrowser<msrDynamics> browser (v);
      browser.browse (*(*i));
    } // for
    gIndenter--;
  }

  // browse the other dynamics if any
  if (fNoteOtherDynamics.size ()) {
    gIndenter++;
    list<S_msrOtherDynamics>::const_iterator i;
    for (i=fNoteOtherDynamics.begin (); i!=fNoteOtherDynamics.end (); i++) {
      // browse the other dynamics
      msrBrowser<msrOtherDynamics> browser (v);
      browser.browse (*(*i));
    } // for
    gIndenter--;
  }

  // browse the words if any
  if (fNoteWords.size ()) {
    gIndenter++;
    list<S_msrWords>::const_iterator i;
    for (i=fNoteWords.begin (); i!=fNoteWords.end (); i++) {
      // browse the words
      msrBrowser<msrWords> browser (v);
      browser.browse (*(*i));
    } // for
    gIndenter--;
  }
  
  // browse the slurs if any
  if (fNoteSlurs.size ()) {
    gIndenter++;
    list<S_msrSlur>::const_iterator i;
    for (i=fNoteSlurs.begin (); i!=fNoteSlurs.end (); i++) {
      // browse the slur
      msrBrowser<msrSlur> browser (v);
      browser.browse (*(*i));
    } // for
    gIndenter--;
  }

  // browse the ligatures if any
  if (fNoteLigatures.size ()) {
    gIndenter++;
    list<S_msrLigature>::const_iterator i;
    for (i=fNoteLigatures.begin (); i!=fNoteLigatures.end (); i++) {
      // browse the ligature
      msrBrowser<msrLigature> browser (v);
      browser.browse (*(*i));
    } // for
    gIndenter--;
  }

  // browse the pedals if any
  if (fNotePedals.size ()) {
    gIndenter++;
    list<S_msrPedal>::const_iterator i;
    for (i=fNotePedals.begin (); i!=fNotePedals.end (); i++) {
      // browse the pedal
      msrBrowser<msrPedal> browser (v);
      browser.browse (*(*i));
    } // for
    gIndenter--;
  }

  // browse the wedges if any
  if (fNoteSlashes.size ()) {
    gIndenter++;
    list<S_msrSlash>::const_iterator i;
    for (i=fNoteSlashes.begin (); i!=fNoteSlashes.end (); i++) {
      // browse the wedge
      msrBrowser<msrSlash> browser (v);
      browser.browse (*(*i));
    } // for
    gIndenter--;
  }

  // browse the wedges if any
  if (fNoteWedges.size ()) {
    gIndenter++;
    list<S_msrWedge>::const_iterator i;
    for (i=fNoteWedges.begin (); i!=fNoteWedges.end (); i++) {
      // browse the wedge
      msrBrowser<msrWedge> browser (v);
      browser.browse (*(*i));
    } // for
    gIndenter--;
  }

  // browse the eyeglasses if any
  if (fNoteEyeGlasses.size ()) {
    gIndenter++;
    list<S_msrEyeGlasses>::const_iterator i;
    for (i=fNoteEyeGlasses.begin (); i!=fNoteEyeGlasses.end (); i++) {
      // browse the eyeglasses
      msrBrowser<msrEyeGlasses> browser (v);
      browser.browse (*(*i));
    } // for
    gIndenter--;
  }

  // browse the damps if any
  if (fNoteDamps.size ()) {
    gIndenter++;
    list<S_msrDamp>::const_iterator i;
    for (i=fNoteDamps.begin (); i!=fNoteDamps.end (); i++) {
      // browse the damp
      msrBrowser<msrDamp> browser (v);
      browser.browse (*(*i));
    } // for
    gIndenter--;
  }

  // browse the damp alls if any
  if (fNoteDampAlls.size ()) {
    gIndenter++;
    list<S_msrDampAll>::const_iterator i;
    for (i=fNoteDampAlls.begin (); i!=fNoteDampAlls.end (); i++) {
      // browse the damp all
      msrBrowser<msrDampAll> browser (v);
      browser.browse (*(*i));
    } // for
    gIndenter--;
  }

  // browse the scordaturas if any
  if (fNoteScordaturas.size ()) {
    gIndenter++;
    list<S_msrScordatura>::const_iterator i;
    for (i=fNoteScordaturas.begin (); i!=fNoteScordaturas.end (); i++) {
      // browse the scordatura
      msrBrowser<msrScordatura> browser (v);
      browser.browse (*(*i));
    } // for
    gIndenter--;
  }

  // browse the syllables if any
  if (fNoteSyllables.size ()) {
    gIndenter++;
    list<S_msrSyllable>::const_iterator i;
    for (i=fNoteSyllables.begin (); i!=fNoteSyllables.end (); i++) {
      // browse the syllable
      msrBrowser<msrSyllable> browser (v);
      browser.browse (*(*i));
    } // for
    gIndenter--;
  }

  if (fNoteHarmony) {
    // browse the harmony
    msrBrowser<msrHarmony> browser (v);
    browser.browse (*fNoteHarmony);
  }

  if (fNoteFrame) {
    // browse the frame
    msrBrowser<msrFrame> browser (v);
    browser.browse (*fNoteFrame);
  }

  if (fNoteFiguredBass) {
    // browse the figured bass
    msrBrowser<msrFiguredBass> browser (v);
    browser.browse (*fNoteFiguredBass);
  }
}

string msrNote::notePitchAsString () const
{
  stringstream s;
  
  if (fNoteIsUnpitched)
    s << "unpitched ";
  else {
    s <<
      msrQuarterTonesPitchKindAsString (
        gMsrOptions->fMsrQuarterTonesPitchesLanguageKind,
        fNoteQuarterTonesPitchKind);
  }
  
  return s.str ();
}

string msrNote::noteDisplayPitchKindAsString () const
{
  stringstream s;
  
  s <<
    msrQuarterTonesPitchKindAsString (
      gMsrOptions->fMsrQuarterTonesPitchesLanguageKind,
      fNoteQuarterTonesDisplayPitchKind);  
  
  return s.str ();
}

string msrNote::noteGraphicDurationAsMsrString () const
{
  string
    result =
      msrDurationKindAsString (
        fNoteGraphicDurationKind);
    
  return result;
}

string msrNote::tupletNoteGraphicDurationAsMsrString ( // JMI
  int actualNotes, int normalNotes) const
{
  return
    wholeNotesAsMsrString (
      fInputLineNumber,
      fNoteSoundingWholeNotes
        *
      rational (actualNotes, normalNotes));
}

string msrNote::noteDiatonicPitchKindAsString (
  int inputLineNumber) const
{
  return
    msrDiatonicPitchKindAsString (
      gMsrOptions->fMsrQuarterTonesPitchesLanguageKind,
      noteDiatonicPitchKind (
        fInputLineNumber));
}

string msrNote::asShortStringWithRawWholeNotes () const
{
  stringstream s;

  switch (fNoteKind) {
    case msrNote::k_NoNoteKind:
      s <<
        "noNoteKind";
      break;
      
    case msrNote::kRestNote:
      s <<
        "restNote" <<
        "[octave: " << fNoteOctave << ", " << noteDisplayOctaveAsString () << "]" <<
        ":" <<
        ", whole notes: " <<
        fNoteSoundingWholeNotes <<
        " sound, " <<
        fNoteDisplayWholeNotes <<
        " disp";
      break;
      
    case msrNote::kSkipNote:
      s <<
        "skipNote" <<
        ":" <<
        ", whole notes: " <<
        fNoteSoundingWholeNotes <<
        " sound, " <<
        fNoteDisplayWholeNotes <<
        " disp";
      break;
      
    case msrNote::kUnpitchedNote:
      s <<
        notePitchAsString () <<
        "unpitchedNote" <<
        ":" <<
        ", whole notes: " <<
        fNoteSoundingWholeNotes <<
        " sound, " <<
        fNoteDisplayWholeNotes <<
        " disp";
      break;
      
    case msrNote::kStandaloneNote:
      s <<
        "standaloneNote '" <<
        notePitchAsString () <<
        "' " <<
        "[octave: " << fNoteOctave << ", " << noteDisplayOctaveAsString () << "]" <<
        ":" <<
        ", whole notes: " <<
        fNoteSoundingWholeNotes <<
        " sound, " <<
        fNoteDisplayWholeNotes <<
        " disp";
      break;
      
    case msrNote::kDoubleTremoloMemberNote:
      s <<
        "doubleTremoloMemberNote '" <<
        notePitchAsString () <<
        "' " <<
        "[octave: " << fNoteOctave << ", " << noteDisplayOctaveAsString () << "]" <<
        ":" <<
        " whole notes: " <<
        fNoteSoundingWholeNotes <<
        " sound, " <<
        fNoteDisplayWholeNotes <<
        " disp";
      break;
      
    case msrNote::kGraceNote:
      s <<
        "graceNote '" <<
        notePitchAsString () <<
        "' " <<
        noteGraphicDurationAsMsrString () <<
        "[octave: " << fNoteOctave << ", " << noteDisplayOctaveAsString () << "]";
        
      for (int i = 0; i < fNoteDotsNumber; i++) {
        s << "."; // JMI
      } // for
      break;

    case msrNote::kGraceChordMemberNote:
      s <<
        "graceChordMemberNote '" <<
        notePitchAsString () <<
        "' " <<
        noteGraphicDurationAsMsrString () <<
        "[octave: " << fNoteOctave << ", " << noteDisplayOctaveAsString () << "]";
        
      for (int i = 0; i < fNoteDotsNumber; i++) {
        s << "."; // JMI
      } // for
      break;
      
    case msrNote::kChordMemberNote:
      s <<
        "chordMemberNote '" <<
        notePitchAsString () <<
        "' " <<
        "[octave: " << fNoteOctave << ", " << noteDisplayOctaveAsString () << "]" <<
        ", whole notes: " <<
        fNoteSoundingWholeNotes <<
        " sound, " <<
        fNoteDisplayWholeNotes <<
        " disp";
      break;
      
    case msrNote::kTupletMemberNote:
      s <<
        "tupletMemberNote '" <<
        notePitchAsString () <<
        "' " <<
        noteGraphicDurationAsMsrString ();

      if (! fNoteIsARest) {
        s <<
        "[octave: " << fNoteOctave << ", " << noteDisplayOctaveAsString () << "]";
      }
      
      s <<
        ", whole notes: " <<
        fNoteSoundingWholeNotes <<
        " sound, " <<
        fNoteDisplayWholeNotes <<
        " disp";
      break;
      
    case msrNote::kTupletMemberUnpitchedNote:
      s <<
        "tupletMemberUnpitchedNote " <<
        noteGraphicDurationAsMsrString () <<
        ", whole notes: " <<
        fNoteSoundingWholeNotes <<
        " sound, " <<
        fNoteDisplayWholeNotes <<
        " disp";
      break;
  } // switch

  s <<
    ", line " << fInputLineNumber;

  return s.str ();
}

string msrNote::asShortString () const
{
  stringstream s;

  switch (fNoteKind) {
    case msrNote::k_NoNoteKind:
      s <<
        "noNoteKind" <<
        ":" <<
        noteSoundingWholeNotesAsMsrString ();
      break;
      
    case msrNote::kRestNote:
      s <<
        "restNote" <<
        ":" <<
        noteSoundingWholeNotesAsMsrString ();
      break;
      
    case msrNote::kSkipNote:
      s <<
        "skipNote" <<
        ":" <<
        noteSoundingWholeNotesAsMsrString ();
      break;
      
    case msrNote::kUnpitchedNote:
      s <<
        "unpitchedNote" <<
        ":" <<
        noteSoundingWholeNotesAsMsrString ();
      break;
      
    case msrNote::kStandaloneNote:
      s <<
        "standaloneNote" <<
        ":" <<
        notePitchAsString () <<
        " " <<
        noteSoundingWholeNotesAsMsrString () <<
        " [octave: " << fNoteOctave << ", " << noteDisplayOctaveAsString () << "]";
      break;
      
    case msrNote::kDoubleTremoloMemberNote:
      s <<
        "doubleTremoloMemberNote" <<
        notePitchAsString () <<
        " " <<
        noteSoundingWholeNotesAsMsrString () <<
        " [octave: " << fNoteOctave << ", " << noteDisplayOctaveAsString () << "]";
      break;
      
    case msrNote::kGraceNote:
       s <<
         "graceNote" <<
       notePitchAsString () <<
        " " <<
        noteGraphicDurationAsMsrString () <<
        " [octave: " << fNoteOctave << ", " << noteDisplayOctaveAsString () << "]";
        
      for (int i = 0; i < fNoteDotsNumber; i++) {
        s << "."; // JMI
      } // for
      break;

   case msrNote::kGraceChordMemberNote:
      s <<
         "graceChordMemberNote" <<
       notePitchAsString () <<
        " " <<
        noteGraphicDurationAsMsrString () <<
        " [octave: " << fNoteOctave << ", " << noteDisplayOctaveAsString () << "]";
        
      for (int i = 0; i < fNoteDotsNumber; i++) {
        s << "."; // JMI
      } // for
      break;
      
    case msrNote::kChordMemberNote:
      s <<
        "chordMemberNote" <<
        notePitchAsString () <<
        " " <<
        ", " <<
        noteSoundingWholeNotesAsMsrString () <<
        " [octave: " << fNoteOctave << ", " << noteDisplayOctaveAsString () << "]";
      break;
      
    case msrNote::kTupletMemberNote:
      s <<
        "tupletMemberNote" <<
        notePitchAsString () <<
        " " <<
        ", whole notes: " <<
        fNoteSoundingWholeNotes <<
        " sound, " <<
        fNoteDisplayWholeNotes <<
        " disp";
        /* JMI
        notePartUplink ()->
          tupletSoundingWholeNotesAsMsrString (
            fInputLineNumber,
            fNoteSoundingWholeNotes,
            fNoteTupletUplink->getTupletActualNotes (),
            fNoteTupletUplink->getTupletNormalNotes ());
            */

      if (! fNoteIsARest)
        s <<
        "[octave: " << fNoteOctave << ", " << noteDisplayOctaveAsString () << "]";
      break;
      
    case msrNote::kTupletMemberUnpitchedNote:
      s <<
        "tupletMemberUnpitchedNote" <<
        ", whole notes: " <<
        fNoteSoundingWholeNotes <<
        " sound, " <<
        fNoteDisplayWholeNotes <<
        " disp";
        /* JMI
        notePartUplink ()->
          tupletSoundingWholeNotesAsMsrString (
            fInputLineNumber,
            fNoteSoundingWholeNotes,
            fNoteTupletUplink->getTupletActualNotes (),
            fNoteTupletUplink->getTupletNormalNotes ());
            */
      break;
  } // switch

  s <<
    ", line " << fInputLineNumber;

  return s.str ();
}

string msrNote::asString () const
{
  stringstream s;

  s <<
    "[=== ";
    
  switch (fNoteKind) {
    case msrNote::k_NoNoteKind:
      s <<
        "noNoteKind???";
      break;
      
    case msrNote::kRestNote:
      s <<
        "restNote, ";
      if (fNoteOccupiesAFullMeasure) {
        s <<
          "R" <<
          multipleRestWholeNoteAsMsrString (
            fInputLineNumber,
            fNoteSoundingWholeNotes);
      }
      else {
        s <<
          "r" <<
          noteSoundingWholeNotesAsMsrString ();
      }

      s <<
        " (" <<
        noteDisplayPitchKindAsString () <<
        noteSoundingWholeNotesAsMsrString () <<
        ", octave" " "<< noteDisplayOctaveAsString () <<
        ")" <<
        ", whole notes: " <<
        fNoteSoundingWholeNotes <<
        " sound, " <<
        fNoteDisplayWholeNotes <<
        " disp" <<
        ":" <<
        noteSoundingWholeNotesAsMsrString ();
      break;
      
    case msrNote::kSkipNote:
      s <<
        "skipNote" <<
        ":" <<
        noteSoundingWholeNotesAsMsrString ();
      break;
      
    case msrNote::kUnpitchedNote:
      s <<
        "unpitchedNote"<<
        " " <<
        noteSoundingWholeNotesAsMsrString ();
      break;
      
    case msrNote::kStandaloneNote:
      s <<
        "standaloneNote '"<<
        notePitchAsString () <<
        " " <<
        noteSoundingWholeNotesAsMsrString () <<
        "', [octave " << fNoteOctave << ", " << noteDisplayOctaveAsString () << "]";
      break;
      
    case msrNote::kDoubleTremoloMemberNote:
      s <<
        "doubleTremoloMemberNote "<<
        notePitchAsString () <<
        " " <<
        noteSoundingWholeNotesAsMsrString () <<
        " [octave" " " << fNoteOctave << ", " << noteDisplayOctaveAsString () << "]";
      break;
      
    case msrNote::kGraceNote:
      s <<
        "graceNote "<<
        notePitchAsString () <<
        " " <<
 // JMI       noteGraphicDurationAsMsrString () <<
        noteDisplayWholeNotesAsMsrString () <<
        " [octave" " " << fNoteOctave << ", " << noteDisplayOctaveAsString () << "]";
        
      for (int i = 0; i < fNoteDotsNumber; i++) {
        s << "."; // JMI
      } // for
      break;

    case msrNote::kGraceChordMemberNote:
      s <<
        "graceChordMemberNote "<<
        notePitchAsString () <<
        " " <<
 // JMI       noteGraphicDurationAsMsrString () <<
        noteDisplayWholeNotesAsMsrString () <<
        " [octave" " " << fNoteOctave << ", " << noteDisplayOctaveAsString () << "]";
        
      for (int i = 0; i < fNoteDotsNumber; i++) {
        s << "."; // JMI
      } // for
      break;
      
    case msrNote::kChordMemberNote:
      s <<
        "chordMemberNote "<<
        notePitchAsString () <<
        " " <<
 // JMI       ", " << fNoteSoundingWholeNotes << " sound whole notes, " <<
        " [octave" " " << fNoteOctave << ", " << noteDisplayOctaveAsString () << "]";
      break;
      
    case msrNote::kTupletMemberNote:
      s <<
        "tupletMemberNote "<<
        notePitchAsString () <<
        " " <<
        ", whole notes: " <<
        fNoteSoundingWholeNotes <<
        " sound, " <<
        fNoteDisplayWholeNotes <<
        " disp";
/* JMI
        notePartUplink ()->
          tupletSoundingWholeNotesAsMsrString (
            fInputLineNumber,
            fNoteSoundingWholeNotes,
            fNoteTupletUplink->getTupletActualNotes (),
            fNoteTupletUplink->getTupletNormalNotes ())
            */

      if (! fNoteIsARest) {
        s <<
        " [octave" " " << fNoteOctave << ", " << noteDisplayOctaveAsString () << "]";
      }
      break;
      
    case msrNote::kTupletMemberUnpitchedNote:
      s <<
        "tupletMemberUnpitchedNote "<<
        ", whole notes: " <<
        fNoteSoundingWholeNotes <<
        " sound, " <<
        fNoteDisplayWholeNotes <<
        " disp";
/* JMI
        notePartUplink ()->
          tupletSoundingWholeNotesAsMsrString (
            fInputLineNumber,
            fNoteSoundingWholeNotes,
            fNoteTupletUplink->getTupletActualNotes (),
            fNoteTupletUplink->getTupletNormalNotes ())
            */
      break;
  } // switch

  s <<
    ", line " << fInputLineNumber;

  if (fNoteOccupiesAFullMeasure)
    s <<
      ", full measure";

  if (fNoteTrillOrnament)
    s <<
      ", has a trill ornament";
  
  if (fNoteDashesOrnament)
    s <<
      ", has a dashes ornament";
  
  if (fNoteWavyLineSpannerStart)
    s <<
      ", has a wavy line spanner start";
  
  if (fNoteWavyLineSpannerStop)
    s <<
      ", has a wavy line spanner stop";
  
  if (fNoteIsFollowedByGraceNotes)
    s <<
      ", followed by grace notes";
  
  if (fNoteTie) {
    s <<
      ", " << fNoteTie->tieKindAsString ();
  }

  if (fNoteIsACueNote) {
    s <<
      ", " << "cue note";
  }

  s <<
    " ===]";
  
  return s.str ();
}

ostream& operator<< (ostream& os, const S_msrNote& elt)
{
  elt->print (os);
  return os;
}

void msrNote::print (ostream& os)
{
  rational
    measureFullLength =
      fNoteMeasureUplink
        ? 
          fNoteMeasureUplink->
            getMeasureFullLength ()
        : rational (0, 1); // JMI
  
  // print the note itself and its positionInMeasure
  os <<
    asString () <<
    endl;

  const int fieldWidth = 34;
    
  {
    // print sounding and displayed whole notes
    gIndenter++;

    switch (fNoteKind) {
      case msrNote::k_NoNoteKind:
      case msrNote::kRestNote:
      case msrNote::kSkipNote:
      case msrNote::kUnpitchedNote:
      case msrNote::kStandaloneNote:
      case msrNote::kDoubleTremoloMemberNote:
      case msrNote::kChordMemberNote:
        os << left <<
          setw (fieldWidth) <<
          "noteSoundingWholeNotes" << " : " <<
          fNoteSoundingWholeNotes <<
          endl <<
          setw (fieldWidth) <<
          "noteDisplayWholeNotes" << " : " <<
          fNoteDisplayWholeNotes <<
          endl;
        break;
  
      case msrNote::kGraceNote:
      case msrNote::kGraceChordMemberNote:
        os <<
          setw (fieldWidth) <<
          "noteDisplayWholeNotes" << " : " <<
          fNoteDisplayWholeNotes <<
          endl;
        break;
  
      case msrNote::kTupletMemberNote:
      case msrNote::kTupletMemberUnpitchedNote:
        os <<
          setw (fieldWidth) <<
          "noteSoundingWholeNotes" << " : " <<
          fNoteSoundingWholeNotes <<
          endl <<
          setw (fieldWidth) <<
          "noteDisplayWholeNotes" << " : " <<
          fNoteDisplayWholeNotes <<
          endl <<
          setw (fieldWidth) <<
          "tupletSoundingWholeNotes" << " : " <<
          wholeNotesAsMsrString (
            fInputLineNumber,
            getNoteTupletUplink ()->
              getTupletSoundingWholeNotes ()) <<
          endl;
        break;
      } // switch

    // full measure length,
    // may be unknown if there is no time signature
    os << left <<
      setw (fieldWidth) <<
      "measureFullLength" << " : ";
    if (measureFullLength.getNumerator () == 0) {
      os <<
        "unknown, there's no time signature";
    }
    else {
      os <<
        measureFullLength;
    }
    os <<
      endl;

    // short cuts for efficiency
    os << left <<
      setw (fieldWidth) <<
      "noteIsARest" << " : " <<
      booleanAsString (fNoteIsARest) <<
      endl <<
      setw (fieldWidth) <<
      "noteIsUnpitched" << " : " <<
      booleanAsString (fNoteIsUnpitched) <<
      endl <<
      setw (fieldWidth) <<
      "noteIsACueNote" << " : " <<
      booleanAsString (fNoteIsACueNote) <<
      endl <<
      setw (fieldWidth) <<
      "noteIsAGraceNote" << " : " <<
      booleanAsString (fNoteIsAGraceNote) <<
      endl;

    // chord member?
    os << left <<
      setw (fieldWidth) <<
      "noteBelongsToAChord" << " : " <<
      booleanAsString (fNoteBelongsToAChord) <<
      endl;

    // tuplet member?
    os << left <<
      setw (fieldWidth) <<
      "noteBelongsToATuplet" << " : " <<
      booleanAsString (fNoteBelongsToATuplet) <<
      endl;

    // multiple rest member?
    os << left <<
      setw (fieldWidth) <<
      "noteBelongsToAMultipleRest" << " : " <<
      booleanAsString (fNoteBelongsToAMultipleRest) <<
      endl;
    os << left <<
      setw (fieldWidth) <<
      "noteMultipleRestSequenceNumber" << " : " <<
      fNoteMultipleRestSequenceNumber <<
      endl;

    // note print kind
    os << left <<
      setw (fieldWidth) <<
     "notePrintKind" << " : " <<
      notePrintKindAsString () <<
      endl;

    // note head
    os << left <<
      setw (fieldWidth) <<
      "noteHeadKind" << " : " <<
      noteHeadKindAsString () <<
      endl <<
      setw (fieldWidth) <<
      "noteHeadFilledKind" << " : " <<
      noteHeadFilledKindAsString () <<
      endl <<
      setw (fieldWidth) <<
      "noteHeadParentheses" << " : " <<
      noteHeadParenthesesKindAsString () <<
      endl;
        
    // accidentals
    os << left <<
      setw (fieldWidth) <<
      "noteAccidentalKind" << " : " <<
      noteAccidentalKindAsString (
        fNoteAccidentalKind) <<
      endl;

    os << left <<
      setw (fieldWidth) <<
      "noteEditorialAccidentalKind" << " : " << 
      noteEditorialAccidentalKindAsString (
        fNoteEditorialAccidentalKind) <<
      endl <<
      setw (fieldWidth) <<
      "noteCautionaryAccidentalKind" << " : " << 
      noteCautionaryAccidentalKindAsString (
        fNoteCautionaryAccidentalKind) <<
      endl;

    // print measure related information
    os << left <<
      setw (fieldWidth) <<
      "noteMeasureNumber" << " : ";
    if (fNoteMeasureNumber == K_NO_MEASURE_NUMBER)
      os <<
        "unknown";
    else if (fNoteMeasureNumber.size ())
      os <<
        fNoteMeasureNumber;
    else {
      stringstream s;

      s <<
        "note measure number is empty in note '" <<
        this->asString () <<
        "'";
      
 // JMI     msrInternalError (
      msrInternalWarning (
        gXml2lyOptions->fInputSourceName,
        fInputLineNumber,
        s.str ());
    }
    os <<
      endl;

    os << left <<
      setw (fieldWidth) <<
      "notePositionInMeasure" << " : ";
    if (fNotePositionInMeasure == K_NO_POSITION_MEASURE_NUMBER)
      os << "unknown";
    else
      os << fNotePositionInMeasure;
      
    // print rationalized position in measure if relevant JMI ???
    if (fNoteMeasureUplink) {
      // the note measure uplink may not have been set yet
      rational
        notePositionBis =
          fNotePositionInMeasure;
      notePositionBis.rationalise ();
      
      if (
        notePositionBis.getNumerator ()
          !=
        fNotePositionInMeasure.getNumerator ()) {
        // print rationalized rational view
        os <<
          " ( = " << notePositionBis << ")";
      }
    }
  
    os <<
      endl ;
      
    gIndenter--;
  }

  {
    // note redundant information (for speed)

    stringstream s;
    
    gIndenter++;

    if (fNoteIsStemless)
      os <<
        "note  is stemless" <<
        endl;
  
    if (fNoteIsFirstNoteInADoubleTremolo)
      os <<
        "first note in a double tremolo" <<
        endl;
    if (fNoteIsSecondNoteInADoubleTremolo)
      os <<
        "second note in a double tremolo" <<
        endl;
  
    if (fNoteTrillOrnament)
      os <<
        "note has a trill ornament" <<
        endl;
        
    if (fNoteDashesOrnament)
      os <<
        "note has a dashes ornament" <<
        endl;
          
    if (fNoteDelayedTurnOrnament)
      os <<
        "note has a delayed turn ornament" <<
        endl;
    if (fNoteDelayedInvertedTurnOrnament)
      os <<
        "note has a delayed inverted turn ornament" <<
        endl;

    if (fNoteWavyLineSpannerStart)
      os <<
        "note has a wavy line spanner start" <<
        endl;
    if (fNoteWavyLineSpannerStop)
      os <<
        "note has a wavy line spanner stop" <<
        endl;
        
    if (fNoteIsFollowedByGraceNotes)
      os <<
        "note is followed by graceNotes" <<
        endl;

     gIndenter--;
  }
  
  {
    // note MSR strings

    gIndenter++;
    
    // print whole notes durations as MSR string
    switch (fNoteKind) {
      case msrNote::k_NoNoteKind:
        {
        }
        break;
        
      case msrNote::kRestNote:
        {
          os << left <<
            setw (fieldWidth) <<
            "noteSoundingWholeNotesAsMsrString" << " : \"" <<
            noteSoundingWholeNotesAsMsrString () <<
            "\"" <<
            endl;
        }
        break;
        
      case msrNote::kSkipNote:
        {
          os << left <<
            setw (fieldWidth) <<
            "noteSoundingWholeNotesAsMsrString" << " : \"" <<
            noteSoundingWholeNotesAsMsrString () <<
            "\"" <<
            endl;
        }
        break;
        
      case msrNote::kUnpitchedNote:
        {
          os << left <<
            setw (fieldWidth) <<
            "noteSoundingWholeNotesAsMsrString" << " : \"" <<
            noteSoundingWholeNotesAsMsrString () <<
            "\"" <<
            endl <<
            setw (fieldWidth) <<
            "noteDisplayWholeNotesAsMsrString" << " : \"" <<
            noteDisplayWholeNotesAsMsrString () <<
            "\"" <<
            endl <<
            setw (fieldWidth) <<
            "noteGraphicDurationAsMsrString" << " : \"" <<
            noteGraphicDurationAsMsrString () <<
            "\"" <<
            endl;
        }
        break;
        
      case msrNote::kStandaloneNote:
        {
          os << left <<
            setw (fieldWidth) <<
            "noteSoundingWholeNotesAsMsrString" << " : \"" <<
            noteSoundingWholeNotesAsMsrString () <<
            "\"" <<
            endl <<
            setw (fieldWidth) <<
            "noteDisplayWholeNotesAsMsrString" << " : \"" <<
            noteDisplayWholeNotesAsMsrString () <<
            "\"" <<
            endl <<
            setw (fieldWidth) <<
            "noteGraphicDurationAsMsrString" << " : \"" <<
            noteGraphicDurationAsMsrString () <<
            "\"" <<
            endl;
        }
        break;
        
      case msrNote::kDoubleTremoloMemberNote:
        {
          // JMI
        }
        break;
        
      case msrNote::kGraceNote:
      case msrNote::kGraceChordMemberNote:
        {
          os << left <<
            setw (fieldWidth) <<
            "noteGraphicDurationAsMsrString" << " : \"" <<
            noteGraphicDurationAsMsrString () <<
            "\"" <<
            endl;
        }
        break;
        
      case msrNote::kChordMemberNote:
        {
          // JMI
        }
        break;
        
      case msrNote::kTupletMemberNote:
      case msrNote::kTupletMemberUnpitchedNote:
        {
          os << left <<
          /* JMI
            setw (fieldWidth) <<
            "noteTupletNoteGraphicDurationAsMsrString" << " : \"" <<
            fNoteTupletNoteGraphicDurationAsMsrString <<
            "\"" <<
            endl <<
              */
            setw (fieldWidth) <<
            "noteTupletNoteSoundingWholeNotesAsMsrString" << " : \"" <<
            wholeNotesAsMsrString (
              fInputLineNumber,
              getNoteTupletUplink ()->
                getTupletSoundingWholeNotes ()) <<
            "\"" <<
            endl <<
            setw (fieldWidth) <<
            "noteGraphicDurationAsMsrString" << " : \"" <<
            noteGraphicDurationAsMsrString () <<
            "\"" <<
            endl;
        }
        break;
      } // switch
  
    gIndenter--;
  }

  // print the syllables associated to this note if any
  if (fNoteSyllables.size ()) {
    gIndenter++;

    os <<
      "Note syllables:" <<
      endl;
      
    gIndenter++;
    
    list<S_msrSyllable>::const_iterator
      iBegin = fNoteSyllables.begin (),
      iEnd   = fNoteSyllables.end (),
      i      = iBegin;
    for ( ; ; ) {
      S_msrSyllable
        syllable = (*i);

      os <<
        syllable;
        
/* JMI
      os <<
        syllable->syllableKindAsString () <<
        ", " <<
        syllable->syllableExtendKindAsString () <<
        " : ";

      msrSyllable::writeTextsList (
        syllable->getSyllableTextsList (),
        os);

      os <<
        ", stanza " <<
        syllable->
          getSyllableStanzaUplink ()->
            getStanzaNumber () <<
        ", line " << syllable->getInputLineNumber () <<
        ", noteUpLink: " <<
        syllable->
          getSyllableNoteUplink ()->
            asShortString ();
*/
            
      if (++i == iEnd) break;
      // no endl here
    } // for

 // JMI   os <<
  //    endl;

    gIndenter--;    

    gIndenter--;
  }

/* JMI
  // print the note's voice uplink JMI
  gIndenter++;
  os <<
    "NoteVoiceUplink" " = " <<
    fNoteMeasureUplink->fetchMeasureVoiceUplink () <<
    endl;
  gIndenter--;
*/

  // print the octave shift if any
  if (fNoteOctaveShift) {
    gIndenter++;
    os <<
      fNoteOctaveShift;
    gIndenter--;
  }

  // print the stem if any
  if (fNoteStem) {
    gIndenter++;
    os <<
      fNoteStem;
    gIndenter--;
  }
    
  // print the beams if any
  if (fNoteBeams.size ()) {
    gIndenter++;
    os <<
      "Note beams:" <<
      endl;
      
    gIndenter++;

    list<S_msrBeam>::const_iterator
      iBegin = fNoteBeams.begin (),
      iEnd   = fNoteBeams.end (),
      i      = iBegin;
    for ( ; ; ) {
      os << (*i);
      if (++i == iEnd) break;
      // no endl here;
    } // for
        
    gIndenter--;
    gIndenter--;
  }
  
  // print the articulations if any
  if (fNoteArticulations.size ()) {
    gIndenter++;
    os <<
      "Note articulations:" <<
      endl;
      
    gIndenter++;

    list<S_msrArticulation>::const_iterator
      iBegin = fNoteArticulations.begin (),
      iEnd   = fNoteArticulations.end (),
      i      = iBegin;
    for ( ; ; ) {
      os << (*i);
      if (++i == iEnd) break;
      // no endl here;
    } // for
        
    gIndenter--;
    gIndenter--;
  }
  
  // print the spanners if any
  if (fNoteSpanners.size ()) {
    gIndenter++;
    os <<
      "Note spanners:" <<
      endl;
      
    gIndenter++;

    list<S_msrSpanner>::const_iterator
      iBegin = fNoteSpanners.begin (),
      iEnd   = fNoteSpanners.end (),
      i      = iBegin;
    for ( ; ; ) {
      os << (*i);
      if (++i == iEnd) break;
      // no endl here;
    } // for
        
    gIndenter--;
    gIndenter--;
  }
  
  // print the technicals if any
  if (fNoteTechnicals.size ()) {
    gIndenter++;
    os <<
      "Note technicals:" <<
      endl;
      
    gIndenter++;

    list<S_msrTechnical>::const_iterator
      iBegin = fNoteTechnicals.begin (),
      iEnd   = fNoteTechnicals.end (),
      i      = iBegin;
    for ( ; ; ) {
      os << (*i);
      if (++i == iEnd) break;
      // no endl here;
    } // for
        
    gIndenter--;
    gIndenter--;
  }
  
  // print the technicals with integer if any
  if (fNoteTechnicalWithIntegers.size ()) {
    gIndenter++;
    os <<
      "Note technicals with integers:" <<
      endl;
      
    gIndenter++;

    list<S_msrTechnicalWithInteger>::const_iterator
      iBegin = fNoteTechnicalWithIntegers.begin (),
      iEnd   = fNoteTechnicalWithIntegers.end (),
      i      = iBegin;
    for ( ; ; ) {
      os << (*i);
      if (++i == iEnd) break;
      // no endl here;
    } // for
        
    gIndenter--;
    gIndenter--;
  }
  
  // print the technicals with float if any
  if (fNoteTechnicalWithFloats.size ()) {
    gIndenter++;
    os <<
      "Note technicals with integers:" <<
      endl;
      
    gIndenter++;

    list<S_msrTechnicalWithFloat>::const_iterator
      iBegin = fNoteTechnicalWithFloats.begin (),
      iEnd   = fNoteTechnicalWithFloats.end (),
      i      = iBegin;
    for ( ; ; ) {
      os << (*i);
      if (++i == iEnd) break;
      // no endl here;
    } // for
        
    gIndenter--;
    gIndenter--;
  }
  
  // print the technicals with string if any
  if (fNoteTechnicalWithStrings.size ()) {
    gIndenter++;
    os <<
      "Note technicals with strings:" <<
      endl;
      
    gIndenter++;

    list<S_msrTechnicalWithString>::const_iterator
      iBegin = fNoteTechnicalWithStrings.begin (),
      iEnd   = fNoteTechnicalWithStrings.end (),
      i      = iBegin;
    for ( ; ; ) {
      os << (*i);
      if (++i == iEnd) break;
      // no endl here;
    } // for
        
    gIndenter--;
    gIndenter--;
  }
  
  // print the ornaments if any
  if (fNoteOrnaments.size ()) {
    gIndenter++;
    os <<
      "Note ornaments:" <<
      endl;
      
    gIndenter++;

    list<S_msrOrnament>::const_iterator
      iBegin = fNoteOrnaments.begin (),
      iEnd   = fNoteOrnaments.end (),
      i      = iBegin;
    for ( ; ; ) {
      os << (*i);
      if (++i == iEnd) break;
      // no endl here;
    } // for
        
    gIndenter--;
    gIndenter--;
  }
  
  // print the glissandos if any
  if (fNoteGlissandos.size ()) {
    gIndenter++;
    os <<
      "Note glissandos:" <<
      endl;
      
    gIndenter++;

    list<S_msrGlissando>::const_iterator
      iBegin = fNoteGlissandos.begin (),
      iEnd   = fNoteGlissandos.end (),
      i      = iBegin;
    for ( ; ; ) {
      os << (*i);
      if (++i == iEnd) break;
      // no endl here;
    } // for
        
    gIndenter--;
    gIndenter--;
  }
  
  // print the slides if any
  if (fNoteSlides.size ()) {
    gIndenter++;
    os <<
      "Note slides:" <<
      endl;
      
    gIndenter++;

    list<S_msrSlide>::const_iterator
      iBegin = fNoteSlides.begin (),
      iEnd   = fNoteSlides.end (),
      i      = iBegin;
    for ( ; ; ) {
      os << (*i);
      if (++i == iEnd) break;
      // no endl here;
    } // for
        
    gIndenter--;
    gIndenter--;
  }
  
  // print the singleTremolo if any
  if (fNoteSingleTremolo) {
    gIndenter++;
    os <<
      "Note single tremolos:" <<
      endl;
      
    gIndenter++;
    os << fNoteSingleTremolo;
    gIndenter--;
    gIndenter--;
  }
  
  // print the tie if any
  if (fNoteTie) {
    gIndenter++;
    os <<
      "Note tie:" <<
      endl;
      
    gIndenter++;
    os <<
      fNoteTie;
    gIndenter--;
    gIndenter--;
  }

  // print the dynamics if any
  if (fNoteDynamics.size ()) {
    gIndenter++;
    os <<
      "Note dynamics:" <<
      endl;
      
    gIndenter++;
    
    list<S_msrDynamics>::const_iterator
      iBegin = fNoteDynamics.begin (),
      iEnd   = fNoteDynamics.end (),
      i      = iBegin;
    for ( ; ; ) {
      os << (*i);
      if (++i == iEnd) break;
      // no endl here;
    } // for

    gIndenter--;
    gIndenter--;
  }

  // print the other dynamics if any
  if (fNoteOtherDynamics.size ()) {
    gIndenter++;
    os <<
      "Note other dynamics:" <<
      endl;
      
    gIndenter++;
    
    list<S_msrOtherDynamics>::const_iterator
      iBegin = fNoteOtherDynamics.begin (),
      iEnd   = fNoteOtherDynamics.end (),
      i      = iBegin;
    for ( ; ; ) {
      os << (*i);
      if (++i == iEnd) break;
      // no endl here;
    } // for

    gIndenter--;
    gIndenter--;
  }

  // print the words if any
  if (fNoteWords.size ()) {
    gIndenter++;
    os <<
      "Note words:" <<
      endl;
      
    gIndenter++;
    
    list<S_msrWords>::const_iterator
      iBegin = fNoteWords.begin (),
      iEnd   = fNoteWords.end (),
      i      = iBegin;
    for ( ; ; ) {
      os << (*i);
      if (++i == iEnd) break;
      // no endl here;
    } // for

    gIndenter--;
    gIndenter--;
  }

  // print the slurs if any
  if (fNoteSlurs.size ()) {
    gIndenter++;
    os <<
      "Note slurs:" <<
      endl;
      
    gIndenter++;
    
    list<S_msrSlur>::const_iterator
      iBegin = fNoteSlurs.begin (),
      iEnd   = fNoteSlurs.end (),
      i      = iBegin;
    for ( ; ; ) {
      os << (*i);
      if (++i == iEnd) break;
      // no endl here;
    } // for
    
    gIndenter--;
    gIndenter--;
  }
  
  // print the ligatures if any
  if (fNoteLigatures.size ()) {
    gIndenter++;
    os <<
      "Note ligatures:" <<
      endl;
      
    gIndenter++;
    
    list<S_msrLigature>::const_iterator
      iBegin = fNoteLigatures.begin (),
      iEnd   = fNoteLigatures.end (),
      i      = iBegin;
    for ( ; ; ) {
      os << (*i);
      if (++i == iEnd) break;
      // no endl here;
    } // for
    
    gIndenter--;
    gIndenter--;
  }
  
  // print the pedals if any
  if (fNotePedals.size ()) {
    gIndenter++;
    os <<
      "Note pedals:" <<
      endl;
      
    gIndenter++;
    
    list<S_msrPedal>::const_iterator
      iBegin = fNotePedals.begin (),
      iEnd   = fNotePedals.end (),
      i      = iBegin;
    for ( ; ; ) {
      os << (*i);
      if (++i == iEnd) break;
      // no endl here;
    } // for
    
    gIndenter--;
    gIndenter--;
  }
  
  // print the slashes if any
  if (fNoteSlashes.size ()) {
    gIndenter++;
    os <<
      "Note slashes:" <<
      endl;
      
    gIndenter++;
    
    list<S_msrSlash>::const_iterator
      iBegin = fNoteSlashes.begin (),
      iEnd   = fNoteSlashes.end (),
      i      = iBegin;
    for ( ; ; ) {
      os << (*i);
      if (++i == iEnd) break;
      // no endl here;
    } // for
    
    gIndenter--;
    gIndenter--;
  }

  // print the wedges if any
  if (fNoteWedges.size ()) {
    gIndenter++;
    os <<
      "Note wedges:" <<
      endl;
      
    gIndenter++;
    
    list<S_msrWedge>::const_iterator
      iBegin = fNoteWedges.begin (),
      iEnd   = fNoteWedges.end (),
      i      = iBegin;
    for ( ; ; ) {
      os << (*i);
      if (++i == iEnd) break;
      // no endl here;
    } // for
    
    gIndenter--;
    gIndenter--;
  }

  // print the eyeglasses if any
  if (fNoteEyeGlasses.size ()) {
    gIndenter++;
    os <<
      "Note eyeGlasses:" <<
      endl;
      
    gIndenter++;
    
    list<S_msrEyeGlasses>::const_iterator
      iBegin = fNoteEyeGlasses.begin (),
      iEnd   = fNoteEyeGlasses.end (),
      i      = iBegin;
    for ( ; ; ) {
      os << (*i);
      if (++i == iEnd) break;
      // no endl here;
    } // for
    
    gIndenter--;
    gIndenter--;
  }

  // print the damps if any
  if (fNoteDamps.size ()) {
    gIndenter++;
    os <<
      "Note damps:" <<
      endl;
      
    gIndenter++;
    
    list<S_msrDamp>::const_iterator
      iBegin = fNoteDamps.begin (),
      iEnd   = fNoteDamps.end (),
      i      = iBegin;
    for ( ; ; ) {
      os << (*i);
      if (++i == iEnd) break;
      // no endl here;
    } // for
    
    gIndenter--;
    gIndenter--;
  }

  // print the damps alls if any
  if (fNoteDampAlls.size ()) {
    gIndenter++;
    os <<
      "Note damp alls:" <<
      endl;
      
    gIndenter++;
    
    list<S_msrDampAll>::const_iterator
      iBegin = fNoteDampAlls.begin (),
      iEnd   = fNoteDampAlls.end (),
      i      = iBegin;
    for ( ; ; ) {
      os << (*i);
      if (++i == iEnd) break;
      // no endl here;
    } // for
    
    gIndenter--;
    gIndenter--;
  }

  // print the scordaturas if any
  if (fNoteScordaturas.size ()) {
    os <<
      "Note scordaturas:" <<
      endl;
      
    gIndenter++;
    
    list<S_msrScordatura>::const_iterator
      iBegin = fNoteScordaturas.begin (),
      iEnd   = fNoteScordaturas.end (),
      i      = iBegin;
    for ( ; ; ) {
      os << (*i);
      if (++i == iEnd) break;
      // no endl here;
    } // for
    
    gIndenter--;
  }

  // print the harmony if any
  if (fNoteHarmony) {
    gIndenter++;
    os <<
      "Note harmony:" <<
      endl;
      
    gIndenter++;
    os <<
      fNoteHarmony <<
      endl;
    gIndenter--;
    gIndenter--;
  }

  // print the frame if any
  if (fNoteFrame) {
    gIndenter++;
    os <<
      "Note frame:" <<
      endl;
      
    gIndenter++;
    os <<
      fNoteFrame <<
      endl;
    gIndenter--;
    gIndenter--;
  }

  // print the figured bass if any
  if (fNoteFiguredBass) {
    gIndenter++;
    os <<
      "Note figures bass:" <<
      endl;
      
    gIndenter++;
    os <<
      fNoteFiguredBass <<
      endl;
    gIndenter--;
    gIndenter--;
  }
}

//______________________________________________________________________________
S_msrChord msrChord::create (
  int             inputLineNumber,
  rational        chordSoundingWholeNotes,
  rational        chordDisplayWholeNotes,
  msrDurationKind chordGraphicDurationKind)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceChords) {
    gLogIOstream <<
      "Creating a chord" <<
      ", chordSoundingWholeNotes = " << chordSoundingWholeNotes <<
      ", chordDisplayWholeNotes = " << chordDisplayWholeNotes <<
      ", chordGraphicDuration = " <<
      msrDurationKindAsString (chordGraphicDurationKind) <<
      endl;
 }
#endif
   
  msrChord* o =
    new msrChord (
      inputLineNumber,
      chordSoundingWholeNotes, chordDisplayWholeNotes,
      chordGraphicDurationKind);
  assert(o!=0);

  return o;
}

msrChord::msrChord (
  int             inputLineNumber,
  rational        chordSoundingWholeNotes,
  rational        chordDisplayWholeNotes,
  msrDurationKind chordGraphicDurationKind)
    : msrElement (inputLineNumber)
{
  fChordSoundingWholeNotes = chordSoundingWholeNotes;
  fChordDisplayWholeNotes  = chordDisplayWholeNotes;
  
  fChordGraphicDurationKind = chordGraphicDurationKind;

  fChordIsFirstChordInADoubleTremolo  = false;
  fChordIsSecondChordInADoubleTremolo = false;
}

msrChord::~msrChord ()
{}

S_msrChord msrChord::createChordNewbornClone (
  S_msrPart containingPart)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceChords) {
    gLogIOstream <<
      "Creating a newborn clone of chord '" <<
      asString () <<
      "'" <<
      endl;
  }
#endif

  // sanity check
  msrAssert(
    containingPart != nullptr,
    "containingPart is null");
    
  S_msrChord
    newbornClone =
      msrChord::create (
        fInputLineNumber,
        fChordSoundingWholeNotes,
        fChordDisplayWholeNotes,
        fChordGraphicDurationKind);
    
  newbornClone->
    fChordPositionInMeasure =
      fChordPositionInMeasure;
    
  newbornClone->
    fChordIsFirstChordInADoubleTremolo =
      fChordIsFirstChordInADoubleTremolo;
    
  newbornClone->
    fChordIsSecondChordInADoubleTremolo =
      fChordIsSecondChordInADoubleTremolo;
    
  newbornClone->fChordTie =
    fChordTie; // JMI

  return newbornClone;
}

void msrChord::setChordSoundingWholeNotes (
  rational wholeNotes)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceChords) {
    gLogIOstream <<
      "Setting chord sounding whole notes to '" <<
      wholeNotes <<
      "' for chord '" <<
      asString () <<
      "'" <<
      endl;
  }
#endif

  fChordSoundingWholeNotes = wholeNotes;
}

void msrChord::setChordDisplayWholeNotes (
  rational wholeNotes)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceChords) {
    gLogIOstream <<
      "Setting chord displayed whole notes to '" <<
      wholeNotes <<
      "' for chord '" <<
      asString () <<
      "'" <<
      endl;
  }
#endif

  fChordDisplayWholeNotes = wholeNotes;
}

string msrChord::chordSoundingWholeNotesAsMsrString ()
{
  return
    wholeNotesAsMsrString (
      fInputLineNumber,
      fChordSoundingWholeNotes);
}

string msrChord::chordDisplayWholeNotesAsMsrString ()
{
  return
    wholeNotesAsMsrString (
      fInputLineNumber,
      fChordDisplayWholeNotes);
}

/* JMI
string msrChord::chordGraphicDurationAsMsrString () const
{
  string result;

  result =
    msrDurationAsString (
      fChordGraphicDuration);

  return result;
}
*/

void msrChord::addFirstNoteToChord (S_msrNote note)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceChords) {
    gLogIOstream <<
      "Adding first note '" <<
      note->asString () <<
      "' to chord '" <<
      asString () <<
      "'" <<
      endl;
  }
#endif

  // append note to chord notes
  fChordNotesVector.push_back (note);

  // mark note as belonging to a chord
  note->setNoteIsAChordsFirstMemberNote ();
  
  // mark note as being the first one in the chord
  note->setNoteBelongsToAChord ();
  
  // populate note's measure number
  note->
    setNoteMeasureNumber (
      fChordMeasureNumber);

  // populate note's position in measure // JMI
  note->
    setNotePositionInMeasure (
      fChordPositionInMeasure);
}

void msrChord::addAnotherNoteToChord (S_msrNote note)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceChords) {
    gLogIOstream <<
      "Adding another note '" <<
      note->asShortStringWithRawWholeNotes () <<
      "' to chord '" <<
      asString () <<
      "'" <<
      endl;
  }
#endif

  fChordNotesVector.push_back (note);
  
  note->setNoteBelongsToAChord ();
  
  // populate note's measure number
  note->setNoteMeasureNumber (
    fChordMeasureNumber);

  // populate note's position in measure
  note->setNotePositionInMeasure (
    fChordPositionInMeasure);
}

void msrChord::setChordFirstNotePositionInMeasure (
  rational positionInMeasure)
{
  if (fChordNotesVector.size ()) { // JMI
    fChordNotesVector.front ()->
      setNotePositionInMeasure (positionInMeasure);
  }
  else {
    // exit (44); JMI
 }
}
                    
void msrChord::setChordFirstNoteMeasureNumber (
  string measureNumber)
{
  if (fChordNotesVector.size ()) { // JMI
    fChordNotesVector.front ()->
      setNoteMeasureNumber (measureNumber);
  }
  else {
    // exit (44); JMI
 }
}
                    
void msrChord::appendArticulationToChord (S_msrArticulation art)
{
  msrArticulation::msrArticulationKind
    articulationKind =
      art->
        getArticulationKind ();

  // don't append the same articulation several times
  for (
    list<S_msrArticulation>::const_iterator i = fChordArticulations.begin ();
    i!=fChordArticulations.end ();
    i++) {
      if ((*i)->getArticulationKind () == articulationKind)
        return;
  } // for

#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceArticulations || gTraceOptions->fTraceChords) {
    gLogIOstream <<
      "Appending articulation '" <<
      art->articulationKindAsString () <<
      "' to chord" <<
      endl;
  }
#endif

  fChordArticulations.push_back (art);
}

void msrChord::appendSpannerToChord (S_msrSpanner span)
{
  msrSpanner::msrSpannerKind
    spannerKind =
      span->
        getSpannerKind ();

  // don't append the same spanner several times
  for (
    list<S_msrSpanner>::const_iterator i = fChordSpanners.begin ();
    i!=fChordSpanners.end ();
    i++) {
      if ((*i)->getSpannerKind () == spannerKind)
        return;
  } // for

#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceSpanners || gTraceOptions->fTraceChords) {
    gLogIOstream <<
      "Appending spanner '" <<
      span->spannerKindAsString () <<
      "' to chord '" <<
      asString () <<
      "'" <<
      endl;
  }
#endif

  fChordSpanners.push_back (span);
}

void msrChord::setChordSingleTremolo (S_msrSingleTremolo trem)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceTremolos || gTraceOptions->fTraceChords) {
    gLogIOstream <<
      "Adding singleTremolo '" <<
      trem->asString () <<
      "' to chord" <<
      endl;
  }
#endif

  fChordSingleTremolo = trem;
}

void msrChord::appendTechnicalToChord (S_msrTechnical tech)
{
  msrTechnical::msrTechnicalKind
    technicalKind =
      tech->
        getTechnicalKind ();

  // don't append the same technical several times
  for (
    list<S_msrTechnical>::const_iterator i =
      fChordTechnicals.begin ();
    i!=fChordTechnicals.end ();
    i++) {
      if ((*i)->getTechnicalKind () == technicalKind)
        return;
  } // for

#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceChords) {
    gLogIOstream <<
      "Appending technical '" <<
      tech->asString () <<
      "' to chord" <<
      endl;
  }
#endif

  fChordTechnicals.push_back (tech);
}

void msrChord::appendTechnicalWithIntegerToChord (
  S_msrTechnicalWithInteger tech)
{
  msrTechnicalWithInteger::msrTechnicalWithIntegerKind
    technicalWithIntegerKind =
      tech->
        getTechnicalWithIntegerKind ();

  // don't append the same technical several times
  for (
    list<S_msrTechnicalWithInteger>::const_iterator i =
      fChordTechnicalWithIntegers.begin ();
    i!=fChordTechnicalWithIntegers.end ();
    i++) {
      if ((*i)->getTechnicalWithIntegerKind () == technicalWithIntegerKind)
        return;
  } // for

#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceChords) {
    gLogIOstream <<
      "Appending technical with integer '" <<
      tech->asString () <<
      "' to chord" <<
      endl;
  }
#endif

  fChordTechnicalWithIntegers.push_back (tech);
}

void msrChord::appendTechnicalWithFloatToChord (
  S_msrTechnicalWithFloat tech)
{
  msrTechnicalWithFloat::msrTechnicalWithFloatKind
    technicalWithFloatKind =
      tech->
        getTechnicalWithFloatKind ();

  // don't append the same technical several times
  for (
    list<S_msrTechnicalWithFloat>::const_iterator i =
      fChordTechnicalWithFloats.begin ();
    i!=fChordTechnicalWithFloats.end ();
    i++) {
      if ((*i)->getTechnicalWithFloatKind () == technicalWithFloatKind)
        return;
  } // for

#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceChords) {
    gLogIOstream <<
      "Appending technical with integer '" <<
      tech->asString () <<
      "' to chord" <<
      endl;
  }
#endif

  fChordTechnicalWithFloats.push_back (tech);
}

void msrChord::appendTechnicalWithStringToChord (
  S_msrTechnicalWithString tech)
{
  msrTechnicalWithString::msrTechnicalWithStringKind
    technicalWithStringKind =
      tech->
        getTechnicalWithStringKind ();

  // don't append the same technical several times
  for (
    list<S_msrTechnicalWithString>::const_iterator i =
      fChordTechnicalWithStrings.begin ();
    i!=fChordTechnicalWithStrings.end ();
    i++) {
      if ((*i)->getTechnicalWithStringKind () == technicalWithStringKind)
        return;
  } // for

#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceChords) {
    gLogIOstream <<
      "Appending technical with string '" <<
      tech->asString () <<
      "' to chord" <<
      endl;
  }
#endif

  fChordTechnicalWithStrings.push_back (tech);
}

void msrChord::appendOrnamentToChord (S_msrOrnament orn)
{
  msrOrnament::msrOrnamentKind
    ornamentKind =
      orn->
        getOrnamentKind ();

  // don't append the same ornament several times
  for (
    list<S_msrOrnament>::const_iterator i = fChordOrnaments.begin ();
    i!=fChordOrnaments.end ();
    i++) {
      if ((*i)->getOrnamentKind () == ornamentKind)
        return;
  } // for

#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceChords) {
    gLogIOstream <<
      "Appending ornament '" <<
      orn->ornamentKindAsString () <<
      "' to chord" <<
      endl;
  }
#endif

  fChordOrnaments.push_back (orn);
}

void msrChord::appendGlissandoToChord (S_msrGlissando gliss)
{
  msrGlissando::msrGlissandoTypeKind
    glissandoTypeKind =
      gliss->
        getGlissandoTypeKind ();

  // don't append the same slissando several times
  for (
    list<S_msrGlissando>::const_iterator i = fChordGlissandos.begin ();
    i!=fChordGlissandos.end ();
    i++) {
      if ((*i)->getGlissandoTypeKind () == glissandoTypeKind)
        return;
  } // for

#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceChords) {
    gLogIOstream <<
      "Appending glissando '" <<
      gliss->asString () <<
      "' to chord" <<
      endl;
  }
#endif

  fChordGlissandos.push_back (gliss);
}

void msrChord::appendSlideToChord (S_msrSlide slide)
{
  msrSlide::msrSlideTypeKind
    slideTypeKind =
      slide->
        getSlideTypeKind ();

  // don't append the same slide several times
  for (
    list<S_msrSlide>::const_iterator i = fChordSlides.begin ();
    i!=fChordSlides.end ();
    i++) {
      if ((*i)->getSlideTypeKind () == slideTypeKind)
        return;
  } // for

#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceChords) {
    gLogIOstream <<
      "Appending slide '" <<
      slide->asString () <<
      "' to chord" <<
      endl;
  }
#endif

  fChordSlides.push_back (slide);
}

void msrChord::acceptIn (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrChord::acceptIn ()" <<
      endl;
  }
      
  if (visitor<S_msrChord>*
    p =
      dynamic_cast<visitor<S_msrChord>*> (v)) {
        S_msrChord elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrChord::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrChord::acceptOut (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrChord::acceptOut ()" <<
      endl;
  }

  if (visitor<S_msrChord>*
    p =
      dynamic_cast<visitor<S_msrChord>*> (v)) {
        S_msrChord elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrChord::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrChord::browseData (basevisitor* v)
{
  for (
    vector<S_msrNote>::const_iterator i = fChordNotesVector.begin ();
    i != fChordNotesVector.end ();
    i++ ) {
    // browse chord note
    msrBrowser<msrNote> browser (v);
    browser.browse (*(*i));
  } // for

  for (
    list<S_msrArticulation>::const_iterator i = fChordArticulations.begin ();
    i != fChordArticulations.end ();
    i++ ) {
    // browse the articulation
    msrBrowser<msrArticulation> browser (v);
    browser.browse (*(*i));
  } // for

  for (
    list<S_msrSpanner>::const_iterator i = fChordSpanners.begin ();
    i != fChordSpanners.end ();
    i++ ) {
    // browse the spanner
    msrBrowser<msrSpanner> browser (v);
    browser.browse (*(*i));
  } // for

  for (
    list<S_msrTechnical>::const_iterator i = fChordTechnicals.begin ();
    i != fChordTechnicals.end ();
    i++ ) {
    // browse the technical
    msrBrowser<msrTechnical> browser (v);
    browser.browse (*(*i));
  } // for

  for (
    list<S_msrOrnament>::const_iterator i = fChordOrnaments.begin ();
    i != fChordOrnaments.end ();
    i++ ) {
    // browse the ornament
    msrBrowser<msrOrnament> browser (v);
    browser.browse (*(*i));
  } // for

  for (
    list<S_msrGlissando>::const_iterator i = fChordGlissandos.begin ();
    i != fChordGlissandos.end ();
    i++ ) {
    // browse the glissando
    msrBrowser<msrGlissando> browser (v);
    browser.browse (*(*i));
  } // for

  for (
    list<S_msrSlide>::const_iterator i = fChordSlides.begin ();
    i != fChordSlides.end ();
    i++ ) {
    // browse the slide
    msrBrowser<msrSlide> browser (v);
    browser.browse (*(*i));
  } // for

  for (
    list<S_msrDynamics>::const_iterator i = fChordDynamics.begin ();
    i != fChordDynamics.end ();
    i++ ) {
    // browse the dynamics
    msrBrowser<msrDynamics> browser (v);
    browser.browse (*(*i));
  } // for

  for (
    list<S_msrOtherDynamics>::const_iterator i = fChordOtherDynamics.begin ();
    i != fChordOtherDynamics.end ();
    i++ ) {
    // browse the other dynamics
    msrBrowser<msrOtherDynamics> browser (v);
    browser.browse (*(*i));
  } // for

  for (
    list<S_msrWords>::const_iterator i = fChordWords.begin ();
    i != fChordWords.end ();
    i++ ) {
    // browse the words
    msrBrowser<msrWords> browser (v);
    browser.browse (*(*i));
  } // for

  for (
    list<S_msrSlur>::const_iterator i = fChordSlurs.begin ();
    i != fChordSlurs.end ();
    i++ ) {
    // browse the slur
    msrBrowser<msrSlur> browser (v);
    browser.browse (*(*i));
  } // for
  
  for (
    list<S_msrLigature>::const_iterator i = fChordLigatures.begin ();
    i != fChordLigatures.end ();
    i++ ) {
    // browse the ligature
    msrBrowser<msrLigature> browser (v);
    browser.browse (*(*i));
  } // for
  
  for (
    list<S_msrSlash>::const_iterator i = fChordSlashes.begin ();
    i != fChordSlashes.end ();
    i++ ) {
    // browse the slash
    msrBrowser<msrSlash> browser (v);
    browser.browse (*(*i));
  } // for

  for (
    list<S_msrWedge>::const_iterator i = fChordWedges.begin ();
    i != fChordWedges.end ();
    i++ ) {
    // browse the wedge
    msrBrowser<msrWedge> browser (v);
    browser.browse (*(*i));
  } // for

  if (fChordHarmony) {
    // browse the harmony
    msrBrowser<msrHarmony> browser (v);
    browser.browse (*fChordHarmony);
  }

  if (fChordFiguredBass) {
    // browse the figured bass
    msrBrowser<msrFiguredBass> browser (v);
    browser.browse (*fChordFiguredBass);
  }
}

ostream& operator<< (ostream& os, const S_msrChord& elt)
{
  elt->print (os);
  return os;
}


/* JMI
void msrChord::applyTupletMemberDisplayFactorToChordMembers (
  int actualNotes, int normalNotes)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceTuplets || gTraceOptions->fTraceChords) {
    gLogIOstream <<
      "Applying tuplet member sounding factor '" <<
      actualNotes << "/" << normalNotes <<
      "' to the members of chord '" << asStringwithRawDivisions () <<
      "', line " << fInputLineNumber <<
      endl;

  for (
    vector<S_msrNote>::const_iterator i = fChordNotesVector.begin ();
    i != fChordNotesVector.end ();
    ++i) {
    (*i)->
      applyTupletMemberSoundingFactorToNote (
        actualNotes, normalNotes);
  } // for
}
#endif
*/

string msrChord::asStringwithRawDivisions () const
{
  stringstream s;

  s << "<";

  if (fChordNotesVector.size ()) {
    vector<S_msrNote>::const_iterator
      iBegin = fChordNotesVector.begin (),
      iEnd   = fChordNotesVector.end (),
      i      = iBegin;
    for ( ; ; ) {
      S_msrNote
        note = (*i);
        
      s <<
      /* JMI
        note->notePitchAsString () <<
        note->noteSoundingWholeNotesAsMsrString () <<
        "[" << note->getNoteOctave () << "]"
        */

        note->asShortStringWithRawWholeNotes ();
        
      if (++i == iEnd) break;
      s << " ";
    } // for
  }

  s << ">";
  
  return s.str ();
}

string msrChord::asString () const
{
  stringstream s;

  s << "<";

  if (fChordNotesVector.size ()) {
    vector<S_msrNote>::const_iterator
      iBegin = fChordNotesVector.begin (),
      iEnd   = fChordNotesVector.end (),
      i      = iBegin;
    for ( ; ; ) {
      S_msrNote
        note = (*i);
        
      s <<
        note->notePitchAsString () <<
        ", whole notes: " <<
        note->getNoteSoundingWholeNotes () <<
        " sound, " <<
        note->getNoteDisplayWholeNotes () <<
        " disp," <<
        "[" << note->getNoteOctave () << "]";
        
      if (++i == iEnd) break;
      s << " ";
    } // for
  }

  s << ">";
  
  return s.str ();
}

string msrChord::asShortString () const
{
  stringstream s;

  s << "<";

  if (fChordNotesVector.size ()) {
    vector<S_msrNote>::const_iterator
      iBegin = fChordNotesVector.begin (),
      iEnd   = fChordNotesVector.end (),
      i      = iBegin;
    for ( ; ; ) {
      S_msrNote
        note = (*i);
        
      s <<
        "'" <<
        note->notePitchAsString () <<
        "', whole notes: " <<
        note->getNoteSoundingWholeNotes () <<
        " sound, " <<
        note->getNoteDisplayWholeNotes () <<
        " disp," <<
        "[" << note->getNoteOctave () << "]";
        
      if (++i == iEnd) break;
      s << " ";
    } // for
  }

  s << ">";
  
  return s.str ();
}

void msrChord::print (ostream& os)
{
  rational
    chordMeasureFullLength =
      fChordMeasureUplink
        ? 
          fChordMeasureUplink->
            getMeasureFullLength ()
        : rational (0, 1); // JMI
    
  os <<
    "Chord, " <<
    singularOrPlural (
      fChordNotesVector.size (), "note", "notes") <<
    ", line " << fInputLineNumber <<
    endl;
    
  gIndenter++;

  const int fieldWidth = 35;
  
  os << left <<
    setw (fieldWidth) <<
    "chordSoundingWholeNotes" << " : " << fChordSoundingWholeNotes <<
    endl <<
    setw (fieldWidth) <<
    "chordDisplayWholeNotes" << " : " << fChordDisplayWholeNotes <<
    endl <<
    setw (fieldWidth) <<
    "chordMeasureNumber" << " : " << fChordMeasureNumber <<
    endl <<
    setw (fieldWidth) <<
    "chordPositionInMeasure" << " : " << fChordPositionInMeasure <<
    endl <<
    setw (fieldWidth) <<
    "chordMeasureFullLength" << " : " << chordMeasureFullLength <<
    endl;

  // print simplified position in measure if relevant
// JMI  if (fChordMeasureUplink) {
    // the chord measure uplink may not have been set yet
    rational
      chordPositionBis =
        fChordPositionInMeasure;
    fChordPositionInMeasure.rationalise ();
    
    if (
      chordPositionBis.getNumerator ()
        !=
      fChordPositionInMeasure.getNumerator ()) {
      // print rationalized rational view
      os << left <<
        setw (fieldWidth) <<
        "fChordPositionInMeasure" << " : " << chordPositionBis <<
        endl;
    }

  os << left <<
    setw (fieldWidth) <<
    "chordIsFirstChordInADoubleTremolo" << " : " <<
    booleanAsString (fChordIsFirstChordInADoubleTremolo) <<
    endl <<
    setw (fieldWidth) <<
    "chordIsSecondChordInADoubleTremolo" << " : " <<
    booleanAsString (fChordIsSecondChordInADoubleTremolo) <<
    endl;

  os <<
    endl;
    
  // print the member notes if any
  if (fChordNotesVector.size ()) {
    vector<S_msrNote>::const_iterator
      iBegin = fChordNotesVector.begin (),
      iEnd   = fChordNotesVector.end (),
      i      = iBegin;
      
    for ( ; ; ) {
      os << (*i);
      if (++i == iEnd) break;
      os << endl;
    } // for
  }
  
  // print the articulations if any
  if (fChordArticulations.size ()) {
    list<S_msrArticulation>::const_iterator i;
    for (i=fChordArticulations.begin (); i!=fChordArticulations.end (); i++) {
      os << (*i);
    } // for
  }

  // print the spanners if any
  if (fChordSpanners.size ()) {
    list<S_msrSpanner>::const_iterator i;
    for (i=fChordSpanners.begin (); i!=fChordSpanners.end (); i++) {
      os << (*i);
    } // for
  }

  // print the technicals if any
  if (fChordTechnicals.size ()) {
    list<S_msrTechnical>::const_iterator i;
    for (i=fChordTechnicals.begin (); i!=fChordTechnicals.end (); i++) {
      os << (*i);
    } // for
  }

  // print the ornaments if any
  if (fChordOrnaments.size ()) {
    list<S_msrOrnament>::const_iterator i;
    for (i=fChordOrnaments.begin (); i!=fChordOrnaments.end (); i++) {
      os << (*i);
    } // for
  }

  // print the glissandos if any
  if (fChordGlissandos.size ()) {
    list<S_msrGlissando>::const_iterator i;
    for (i=fChordGlissandos.begin (); i!=fChordGlissandos.end (); i++) {
      os << (*i);
    } // for
  }

  // print the slides if any
  if (fChordSlides.size ()) {
    list<S_msrSlide>::const_iterator i;
    for (i=fChordSlides.begin (); i!=fChordSlides.end (); i++) {
      os << (*i);
    } // for
  }

  // print the dynamics if any
  if (fChordDynamics.size ()) {
    list<S_msrDynamics>::const_iterator i;
    for (i=fChordDynamics.begin (); i!=fChordDynamics.end (); i++) {
      os << (*i);
    } // for
  }

  // print the other dynamics if any
  if (fChordOtherDynamics.size ()) {
    list<S_msrOtherDynamics>::const_iterator i;
    for (i=fChordOtherDynamics.begin (); i!=fChordOtherDynamics.end (); i++) {
      os << (*i);
    } // for
  }

  // print the stems if any
  if (fChordStems.size ()) {
    list<S_msrStem>::const_iterator i;
    for (i=fChordStems.begin (); i!=fChordStems.end (); i++) {
      os << (*i);
    } // for
  }

  // print the beams if any
  if (fChordBeams.size ()) {
    list<S_msrBeam>::const_iterator i;
    for (i=fChordBeams.begin (); i!=fChordBeams.end (); i++) {
      os << (*i);
    } // for
  }

  // print the words if any
  if (fChordWords.size ()) {
    list<S_msrWords>::const_iterator i;
    for (i=fChordWords.begin (); i!=fChordWords.end (); i++) {
      os << (*i);
    } // for
  }

  // print the slurs if any
  if (fChordSlurs.size ()) {
    list<S_msrSlur>::const_iterator i;
    for (i=fChordSlurs.begin (); i!=fChordSlurs.end (); i++) {
      os << (*i);
    } // for
  }

  // print the ligatures if any
  if (fChordLigatures.size ()) {
    list<S_msrLigature>::const_iterator i;
    for (i=fChordLigatures.begin (); i!=fChordLigatures.end (); i++) {
      os << (*i);
    } // for
  }

  // print the slashes if any
  if (fChordSlashes.size ()) {
    list<S_msrSlash>::const_iterator i;
    for (i=fChordSlashes.begin (); i!=fChordSlashes.end (); i++) {
      os << (*i);
    } // for
  }

  // print the wedges if any
  if (fChordWedges.size ()) {
    list<S_msrWedge>::const_iterator i;
    for (i=fChordWedges.begin (); i!=fChordWedges.end (); i++) {
      os << (*i);
    } // for
  }

  // print the harmony if any
  if (fChordHarmony) {
    os <<
      "Chord harmony: " <<
      endl;
        
    gIndenter++;

    os <<
      fChordHarmony->asString () <<
      endl;
      
    gIndenter--;
  }

  // print the figured bass if any
  if (fChordFiguredBass) {
    os <<
      "Chord fibured bass: " <<
      endl;
        
    gIndenter++;

    os <<
      fChordFiguredBass->asString () <<
      endl;
      
    gIndenter--;
  }

  os <<
    endl;

  gIndenter--;
}

//______________________________________________________________________________
S_msrTuplet msrTuplet::create (
  int                     inputLineNumber,
  int                     tupletNumber,
  msrTupletBracketKind    tupletBracketKind,
  msrTupletLineShapeKind  tupletLineShapeKind,
  msrTupletShowNumberKind tupletShowNumberKind,
  msrTupletShowTypeKind   tupletShowTypeKind,
  msrTupletFactor         tupletFactor,
  /*
  int                     tupletActualNotes,
  int                     tupletNormalNotes,
  */
  rational                memberNotesSoundingWholeNotes,
  rational                memberNotesDisplayWholeNotes,
  rational                notePositionInMeasure)
{
  msrTuplet* o =
    new msrTuplet (
      inputLineNumber,
      tupletNumber,
      tupletBracketKind,
      tupletLineShapeKind,
      tupletShowNumberKind,
      tupletShowTypeKind,
      tupletFactor,
      /*
      tupletActualNotes,
      tupletNormalNotes,
      */
      memberNotesSoundingWholeNotes,
      memberNotesDisplayWholeNotes,
      notePositionInMeasure);
  assert(o!=0);
  return o;
}

msrTuplet::msrTuplet (
  int                     inputLineNumber,
  int                     tupletNumber,
  msrTupletBracketKind    tupletBracketKind,
  msrTupletLineShapeKind  tupletLineShapeKind,
  msrTupletShowNumberKind tupletShowNumberKind,
  msrTupletShowTypeKind   tupletShowTypeKind,
  msrTupletFactor         tupletFactor,
  /*
  int                     tupletActualNotes,
  int                     tupletNormalNotes,
  */
  rational                memberNotesSoundingWholeNotes,
  rational                memberNotesDisplayWholeNotes,
  rational                notePositionInMeasure)
    : msrElement (inputLineNumber)
{  
  fTupletNumber = tupletNumber;
  
  fTupletBracketKind    = tupletBracketKind;
  fTupletLineShapeKind  = tupletLineShapeKind;
  fTupletShowNumberKind = tupletShowNumberKind;
  fTupletShowTypeKind   = tupletShowTypeKind;

  fTupletFactor = tupletFactor;
//  fTupletActualNotes = tupletActualNotes;
//  fTupletNormalNotes = tupletNormalNotes;

  fMemberNotesSoundingWholeNotes = memberNotesSoundingWholeNotes;
  fMemberNotesDisplayWholeNotes  = memberNotesDisplayWholeNotes;
  
  fTupletSoundingWholeNotes = rational (0, 1);
  fTupletDisplayWholeNotes  = rational (0, 1);

  fTupletPositionInMeasure = notePositionInMeasure;
}

msrTuplet::~msrTuplet ()
{}

S_msrTuplet msrTuplet::createTupletNewbornClone ()
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceTuplets) {
    gLogIOstream <<
      "Creating a newborn clone of tuplet '" <<
      asString () <<
      "'" <<
      endl;
  }
#endif

  S_msrTuplet
    newbornClone =
      msrTuplet::create (
        fInputLineNumber,
        fTupletNumber,
        fTupletBracketKind,
        fTupletLineShapeKind,
        fTupletShowNumberKind,
        fTupletShowTypeKind,
        fTupletFactor,
      //  fTupletActualNotes = tupletActualNotes;
      //  fTupletNormalNotes = tupletNormalNotes;
        fMemberNotesSoundingWholeNotes,
        fMemberNotesDisplayWholeNotes,
        fTupletPositionInMeasure);

  newbornClone->fTupletSoundingWholeNotes =
    fTupletSoundingWholeNotes;

  newbornClone->fTupletDisplayWholeNotes =
    fTupletDisplayWholeNotes;

  newbornClone->fTupletMeasureNumber =
    fTupletMeasureNumber;

  return newbornClone;
}

string msrTuplet::tupletTypeKindAsString (
  msrTupletTypeKind tupletTypeKind)
{
  string result;

  switch (tupletTypeKind) {
    case msrTuplet::kTupletTypeNone:
      result = "tupletTypeNone";
      break;
    case msrTuplet::kTupletTypeStart:
      result = "tupletTypeStart";
      break;
    case msrTuplet::kTupletTypeContinue:
      result = "tupletTypeContinue";
      break;
    case msrTuplet::kTupletTypeStop:
      result = "tupletTypeStop";
      break;
    case msrTuplet::kTupletTypeStartAndStopInARow:
      result = "tupletTypeStartAndStopInARow";
      break;
  } // switch

  return result;
}

string msrTuplet::tupletBracketKindAsString (
  msrTupletBracketKind tupletBracketKind)
{
  string result;

  switch (tupletBracketKind) {
    case msrTuplet::kTupletBracketYes:
      result = "tupletBracketYes";
      break;
    case msrTuplet::kTupletBracketNo:
      result = "tupletBracketNo";
      break;
  } // switch

  return result;
}

string msrTuplet::tupletLineShapeKindAsString (
  msrTupletLineShapeKind tupletLineShapeKind)
{
  string result;

  switch (tupletLineShapeKind) {
    case msrTuplet::kTupletLineShapeStraight:
      result = "tupletLineShapeStraight";
      break;
    case msrTuplet::kTupletLineShapeCurved:
      result = "tupletLineShapeCurved";
      break;
  } // switch

  return result;
}

string msrTuplet::tupletShowNumberKindAsString (
  msrTupletShowNumberKind tupletShowNumberKind)
{
  string result;
  
  switch (tupletShowNumberKind) {
    case msrTuplet::kTupletShowNumberActual:
      result = "tupletShowNumberActual";
      break;
    case msrTuplet::kTupletShowNumberBoth:
      result = "tupletShowNumberBoth";
      break;
    case msrTuplet::kTupletShowNumberNone:
      result = "tupletShowNumberNone";
      break;
  } // switch

  return result;
}

string msrTuplet::tupletShowTypeKindAsString (
  msrTupletShowTypeKind tupletShowTypeKind)
{
  string result;
  
  switch (tupletShowTypeKind) {
    case msrTuplet::kTupletShowTypeActual:
      result = "tupletShowTypeActual";
      break;
    case msrTuplet::kTupletShowTypeBoth:
      result = "tupletShowTypeBoth";
      break;
    case msrTuplet::kTupletShowTypeNone:
      result = "tupletShowTypeNone";
      break;
  } // switch

  return result;
}
      
void msrTuplet::addNoteToTuplet (S_msrNote note)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceTuplets) {
    gLogIOstream <<
      "Adding note '" <<
      note->asShortStringWithRawWholeNotes () <<
      // the information is missing to display it the normal way
      "' to tuplet '" <<
      asString () <<
      "'" <<
      endl;
  }
#endif

  fTupletElementsList.push_back (note);

  // register note's tuplet uplink
  note->
    setNoteTupletUplink (this);

  // account for note duration
  fTupletSoundingWholeNotes +=
    note->getNoteSoundingWholeNotes ();
  fTupletSoundingWholeNotes.rationalise ();
  
  fTupletDisplayWholeNotes += // JMI
    note->getNoteDisplayWholeNotes ();  
  fTupletDisplayWholeNotes.rationalise ();
    
  // populate note's measure number
  note->setNoteMeasureNumber (
    fTupletMeasureNumber);

  // populate note's position in measure
  note->setNotePositionInMeasure (
    fTupletPositionInMeasure);
}

void msrTuplet::addChordToTuplet (S_msrChord chord)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceChords || gTraceOptions->fTraceTuplets) {
    gLogIOstream <<
      "Adding chord '" <<
      chord->asString () <<
      "' to tuplet '" <<
      asString () <<
      "'" <<
      endl;
  }
#endif

  fTupletElementsList.push_back (chord);
    
  // account for chord duration
  fTupletSoundingWholeNotes +=
    chord->getChordSoundingWholeNotes ();
  fTupletSoundingWholeNotes.rationalise ();

  fTupletDisplayWholeNotes += // JMI
    chord->getChordDisplayWholeNotes ();  
  fTupletDisplayWholeNotes.rationalise ();
    
  // populate chord's measure number
  chord->setChordMeasureNumber (
    fTupletMeasureNumber);

  // populate chord's position in measure
  chord->setChordPositionInMeasure (
    fTupletPositionInMeasure);
}

void msrTuplet::addTupletToTuplet (S_msrTuplet tuplet)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceTuplets) {
    gLogIOstream <<
      "Adding tuplet '" <<
      tuplet->asString () <<
      "' to tuplet '" <<
      asString () <<
      "'" <<
      endl;
  }
#endif

/* JMI
  // unapply containing tuplet factor,
  // i.e 3/2 inside 5/4 becomes 15/8 in MusicXML...
  tuplet->
    unapplySoundingFactorToTupletMembers (
      this->fTupletActualNotes,
      this->fTupletNormalNotes);
  */
  
  // register tuplet in elements list
  fTupletElementsList.push_back (tuplet);
    
  // account for tuplet duration
  fTupletSoundingWholeNotes +=
    tuplet->getTupletSoundingWholeNotes ();
  fTupletSoundingWholeNotes.rationalise ();

  fTupletDisplayWholeNotes += // JMI
    tuplet->getTupletDisplayWholeNotes ();
  fTupletDisplayWholeNotes.rationalise ();
    
    /*
  fTupletDisplayWholeNotes += // JMI
    tuplet->getTupletDisplayWholeNotes ();  
    */

  // don't populate tuplet's measure number nor position in measure,
  // this will be done in setTupletMeasureNumber ()
  /* JMI
  tuplet->setTupletMeasureNumber (
    fTupletMeasureNumber);

  // populate tuplet's position in measure
  tuplet->setTupletPositionInMeasure (
    fTupletPositionInMeasure);
    */
}

void msrTuplet::addTupletToTupletClone (S_msrTuplet tuplet)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceTuplets) {
    gLogIOstream <<
      "Adding tuplet '" <<
      tuplet->asString () <<
      "' to tuplet '" <<
      asString () <<
      "'" <<
      endl;
  }
#endif

  // dont' unapply containing tuplet factor,
  // this has been done when building the MSR from MusicXML
  
  // register tuplet in elements list
  fTupletElementsList.push_back (tuplet);
    
  // account for tuplet duration
  fTupletSoundingWholeNotes +=
    tuplet->getTupletSoundingWholeNotes ();
  fTupletSoundingWholeNotes.rationalise ();

  fTupletDisplayWholeNotes +=
    tuplet->getTupletDisplayWholeNotes ();
  fTupletDisplayWholeNotes.rationalise ();
}

S_msrNote msrTuplet::removeFirstNoteFromTuplet (
  int       inputLineNumber)
{
  S_msrNote result;
  
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceTuplets) {
    gLogIOstream <<
      "Removing first note from tuplet '" <<
      asString () <<
      "'" <<
      endl;
  }
#endif

  if (fTupletElementsList.size ()) {
    S_msrElement
      firstTupletElement =
        fTupletElementsList.front ();

    if (
      S_msrNote note = dynamic_cast<msrNote*>(&(*firstTupletElement))
      ) {
      fTupletElementsList.pop_front ();
      result = note;
    }
    
    else {
      if (true) {
        this->print (gLogIOstream);
      }
      
      msrInternalError (
        gXml2lyOptions->fInputSourceName,
        fInputLineNumber,
        __FILE__, __LINE__,
        "removeFirstNoteFromTuplet () expects a note as the first tuplet element");
    }

/* JMI
    for (
      list<S_msrElement>::iterator i=fTupletElementsList.begin ();
      i!=fTupletElementsList.end ();
      ++i) {
      if ((*i) == note) {
        // found note, erase it
        fTupletElementsList.erase (i);
        
        // account for note duration
        fTupletSoundingWholeNotes -=
          note->getNoteSoundingWholeNotes ();
        fTupletSoundingWholeNotes.rationalise ();

        fTupletDisplayWholeNotes -= // JMI
          note->getNoteDisplayWholeNotes ();  
        fTupletDisplayWholeNotes.rationalise ();

        // don't update measure number nor position in measure: // JMI
        // they have not been set yet
  
        // return from function
        return;
      }
    } // for

    stringstream s;

    s <<
      "cannot remove note " <<
      note <<
      " from tuplet " << asString () <<
      "' in voice \"" <<
      fTupletMeasureUplink->
        getMeasureSegmentUplink ()->
          getSegmentVoiceUplink ()->
            getVoiceName () <<
      "\"," <<
      " since it has not been found";

    msrInternalError (
      gXml2lyOptions->fInputSourceName,
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  */
  }
  
  else {
    stringstream s;

    s <<
      "cannot remove the first note of an empty tuplet " <<
      "' in voice \"" <<
      fTupletMeasureUplink->
        getMeasureSegmentUplink ()->
          getSegmentVoiceUplink ()->
            getVoiceName () <<
      "\"";

    msrInternalError (
      gXml2lyOptions->fInputSourceName,
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  }

  return result;
}

S_msrNote msrTuplet::removeLastNoteFromTuplet (
  int       inputLineNumber)
{
  S_msrNote result;
  
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceTuplets) {
    gLogIOstream <<
      "Removing last note from tuplet '" <<
      asString () <<
      "'" <<
      endl;
  }
#endif

  if (fTupletElementsList.size ()) {
    S_msrElement
      lastTupletElement =
        fTupletElementsList.back ();

    if (
      S_msrNote note = dynamic_cast<msrNote*>(&(*lastTupletElement))
      ) {
      fTupletElementsList.pop_back ();
      result = note;
    }
    
    else {
      if (true) {
        this->print (gLogIOstream);
      }
      
      msrInternalError (
        gXml2lyOptions->fInputSourceName,
        fInputLineNumber,
        __FILE__, __LINE__,
        "removeLastNoteFromTuplet () expects a note as the last tuplet element");
    }

/* JMI
    for (
      list<S_msrElement>::iterator i=fTupletElementsList.begin ();
      i!=fTupletElementsList.end ();
      ++i) {
      if ((*i) == note) {
        // found note, erase it
        fTupletElementsList.erase (i);
        
        // account for note duration
        fTupletSoundingWholeNotes -=
          note->getNoteSoundingWholeNotes ();
        fTupletSoundingWholeNotes.rationalise ();

        fTupletDisplayWholeNotes -= // JMI
          note->getNoteDisplayWholeNotes ();  
        fTupletDisplayWholeNotes.rationalise ();

        // don't update measure number nor position in measure: // JMI
        // they have not been set yet
  
        // return from function
        return;
      }
    } // for

    stringstream s;

    s <<
      "cannot remove note " <<
      note <<
      " from tuplet " << asString () <<
      "' in voice \"" <<
      fTupletMeasureUplink->
        getMeasureSegmentUplink ()->
          getSegmentVoiceUplink ()->
            getVoiceName () <<
      "\"," <<
      " since it has not been found";

    msrInternalError (
      gXml2lyOptions->fInputSourceName,
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  */
  }
  
  else {
    stringstream s;

    s <<
      "cannot remove the last note of an empty tuplet " <<
      "' in voice \"" <<
      fTupletMeasureUplink->
        getMeasureSegmentUplink ()->
          getSegmentVoiceUplink ()->
            getVoiceName () <<
      "\"";

    msrInternalError (
      gXml2lyOptions->fInputSourceName,
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  }

  return result;
}

void msrTuplet::setTupletMeasureNumber (string measureNumber) // JMI
{
  fTupletMeasureNumber = measureNumber;

  // propagate measure number to the tuplets elements
  for (
    list<S_msrElement>::const_iterator i = fTupletElementsList.begin ();
    i != fTupletElementsList.end ();
    i++ ) {
    // set tuplet element measure number

  //  SMARTP<msrNote>* note = dynamic_cast<SMARTP<msrNote>*>(&(*tupletMember));

  // BON   SMARTP<msrNote> note = dynamic_cast<msrNote*>(&(*tupletMember));

    if (
      S_msrNote note = dynamic_cast<msrNote*>(&(*(*i)))
      ) {
      note->setNoteMeasureNumber (measureNumber);
    }
  
    else if (
      S_msrChord chord = dynamic_cast<msrChord*>(&(*(*i)))
      ) {
      chord->setChordMeasureNumber (measureNumber);
    }
    
    else if (
      S_msrTuplet tuplet = dynamic_cast<msrTuplet*>(&(*(*i)))
      ) {
      tuplet->setTupletMeasureNumber (measureNumber);
    }
    
    else {
      msrInternalError (
        gXml2lyOptions->fInputSourceName,
        fInputLineNumber,
        __FILE__, __LINE__,
        "tuplet member should be a note, a chord or another tuplet");
    }
  } // for
}

rational msrTuplet::setTupletPositionInMeasure (
  rational positionInMeasure)
  // returns the position in measure after the tuplet
{
  fTupletPositionInMeasure = positionInMeasure;

  rational currentPosition = positionInMeasure;
  
  // compute position in measure for the tuplets elements
  for (
    list<S_msrElement>::const_iterator i = fTupletElementsList.begin ();
    i != fTupletElementsList.end ();
    i++ ) {
    // set tuplet element position in measure
    
    if (
      S_msrNote note = dynamic_cast<msrNote*>(&(*(*i)))
      ) {    
      note->
        setNotePositionInMeasure (currentPosition);
        
      currentPosition +=
        note->
          getNoteSoundingWholeNotes ();
    }
  
    else if (
      S_msrChord chord = dynamic_cast<msrChord*>(&(*(*i)))
      ) {
      chord->
        setChordPositionInMeasure (currentPosition);
        
      currentPosition +=
        chord->
          getChordSoundingWholeNotes ();
    }
    
    else if (
      S_msrTuplet tuplet = dynamic_cast<msrTuplet*>(&(*(*i)))
      ) {
      currentPosition =
        tuplet->
          setTupletPositionInMeasure (currentPosition);
    }
    
    else {
      msrInternalError (
        gXml2lyOptions->fInputSourceName,
        fInputLineNumber,
        __FILE__, __LINE__,
        "tuplet member should be a note, a chord or another tuplet");
    }

  } // for

  return currentPosition;
}

/* JMI
void msrTuplet::applyDisplayFactorToTupletMembers ()
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceTuplets) {
    gLogIOstream <<
      "% ==> applyDisplayFactorToTupletMembers ()" <<
      endl;

    gIndenter++;
    
    gLogIOstream <<
      "% fTupletActualNotes = " <<
      fTupletActualNotes <<
      ", fTupletNormalNotes = " <<
      fTupletNormalNotes <<
      endl <<
      endl;

    gIndenter--;
  }
#endif

  for (
    list<S_msrElement>::const_iterator i = fTupletElementsList.begin ();
    i != fTupletElementsList.end ();
    i++ ) {
    // apply sounding factor to tuplet element
    
    if (
      S_msrNote note = dynamic_cast<msrNote*>(&(*(*i)))
      ) {    
      note->
        applyTupletMemberSoundingFactorToNote (
          fTupletActualNotes, fTupletNormalNotes);
     }
  
    else if (
      S_msrChord chord = dynamic_cast<msrChord*>(&(*(*i)))
      ) {
      chord->
        applyTupletMemberDisplayFactorToChordMembers (
          fTupletActualNotes, fTupletNormalNotes);
    }
    
    else if (
      S_msrTuplet tuplet = dynamic_cast<msrTuplet*>(&(*(*i)))
      ) {
      // don't apply the sounding factor to nested tuplets
 // JMI     tuplet->
       // applyDisplayFactorToTupletMembers ();
    }
    
    else {
      msrInternalError (
        fInputLineNumber,
        "tuplet member should be a note, a chord or another tuplet");
    }

  } // for
}
*/

void msrTuplet::unapplySoundingFactorToTupletMembers (
  const msrTupletFactor& containingTupletFactor)
  /*
  int containingTupletActualNotes,
  int containingTupletNormalNotes)
  */
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceTuplets) {
    gLogIOstream <<
      "unapplySoundingFactorToTupletMembers ()" <<
      endl;

    gIndenter++;
    
    gLogIOstream <<
      "% fTupletFactor = " << fTupletFactor.asString () <<
      /*
      "% fTupletActualNotes = " <<
      fTupletActualNotes <<
      ", fTupletNormalNotes = " <<
      fTupletNormalNotes <<
      */
      endl <<
      "% containingTupletFactor = " << containingTupletFactor.asString () <<
      /*
      "% containingTupletActualNotes = " <<
      containingTupletActualNotes <<
      ", containingTupletNormalNotes = " <<
      containingTupletNormalNotes <<
      */
      endl <<
      endl;

    gIndenter--;
  }
#endif

  fTupletFactor.fTupletActualNotes /=
    containingTupletFactor.fTupletActualNotes;
  fTupletFactor.fTupletNormalNotes /=
    containingTupletFactor.fTupletNormalNotes;

    /*
  fTupletActualNotes /=
    containingTupletActualNotes;
  fTupletNormalNotes /=
    containingTupletNormalNotes;
    */
}

void msrTuplet::acceptIn (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrTuplet::acceptIn ()" <<
      endl;
  }
      
  if (visitor<S_msrTuplet>*
    p =
      dynamic_cast<visitor<S_msrTuplet>*> (v)) {
        S_msrTuplet elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrTuplet::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrTuplet::acceptOut (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrTuplet::acceptOut ()" <<
      endl;
  }

  if (visitor<S_msrTuplet>*
    p =
      dynamic_cast<visitor<S_msrTuplet>*> (v)) {
        S_msrTuplet elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrTuplet::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrTuplet::browseData (basevisitor* v)
{
  for (
    list<S_msrElement>::const_iterator i = fTupletElementsList.begin ();
    i != fTupletElementsList.end ();
    i++ ) {
    // browse tuplet element
    msrBrowser<msrElement> browser (v);
    browser.browse (*(*i));
  } // for
}

ostream& operator<< (ostream& os, const S_msrTuplet& elt)
{
  elt->print (os);
  return os;
}

string msrTuplet::asString () const
{
  stringstream s;

  s <<
    "Tuplet " <<
    fTupletFactor.asString () <<
    " " << fTupletSoundingWholeNotes << " tupletSoundingWholeNotes" <<
    " @meas "<<
    fTupletMeasureNumber <<
    ":";

  if (fTupletPositionInMeasure.getNumerator () < 0)
    s << "?";
  else
    s << fTupletPositionInMeasure;
  
  s << "[[";

  if (fTupletElementsList.size ()) {
    list<S_msrElement>::const_iterator
      iBegin = fTupletElementsList.begin (),
      iEnd   = fTupletElementsList.end (),
      i      = iBegin;
    for ( ; ; ) {
      
      if (
        S_msrNote note = dynamic_cast<msrNote*>(&(*(*i)))
        ) {    
        s <<
          note->asShortStringWithRawWholeNotes ();
      }
    
      else if (
        S_msrChord chord = dynamic_cast<msrChord*>(&(*(*i)))
        ) {
        s <<
          chord->asString ();
      }
      
      else if (
        S_msrTuplet tuplet = dynamic_cast<msrTuplet*>(&(*(*i)))
        ) {
        s <<
          tuplet->asString ();
      }
      
      else {
        msrInternalError (
          gXml2lyOptions->fInputSourceName,
          fInputLineNumber,
          __FILE__, __LINE__,
          "tuplet member should be a note, a chord or another tuplet");
      }
  
      if (++i == iEnd) break;
      s << " ";
      
    } // for
  }

  s << "]]";
  
  return s.str ();
}

/* JMI
string msrTuplet::asString () const
{
  stringstream s;

  s <<
    "Tuplet " <<
    fTupletActualNotes << "/" << fTupletNormalNotes <<
    " " << fTupletSoundingWholeNotes << " sound whole notes" <<
    " @meas "<<
    fTupletMeasureNumber <<
    ":";

  if (fTupletPositionInMeasure.getNumerator () < 0)
    s << "?";
  else
    s << fTupletPositionInMeasure;
  
  s << "[[";

  if (fTupletElementsList.size ()) {
    list<S_msrElement>::const_iterator
      iBegin = fTupletElementsList.begin (),
      iEnd   = fTupletElementsList.end (),
      i      = iBegin;
    for ( ; ; ) {
      
      if (
        S_msrNote note = dynamic_cast<msrNote*>(&(*(*i)))
        ) {    
        s <<
          note->asShortStringWithRawWholeNotes ();
      }
    
      else if (
        S_msrChord chord = dynamic_cast<msrChord*>(&(*(*i)))
        ) {
        s <<
          chord->asString ();
      }
      
      else if (
        S_msrTuplet tuplet = dynamic_cast<msrTuplet*>(&(*(*i)))
        ) {
        s <<
          tuplet->asString ();
      }
      
      else {
        msrInternalError (
          gXml2lyOptions->fInputSourceName,
          fInputLineNumber,
          __FILE__, __LINE__,
          "tuplet member should be a note, a chord or another tuplet");
      }
  
      if (++i == iEnd) break;
      s << " ";
    } // for
  }

  s << "]]";
  
  return s.str ();
}
*/

void msrTuplet::print (ostream& os)
{
  os <<
    "Tuplet " <<
    fTupletFactor.asString () <<
    ", " <<
    singularOrPlural (
      fTupletElementsList.size (), "element", "elements") <<
    ", whole notes: " <<
    fTupletSoundingWholeNotes << " sound, " <<
    fTupletDisplayWholeNotes << " disp" <<
    ", meas "<<
    fTupletMeasureNumber <<
    ", line " << fInputLineNumber <<
    endl;

  gIndenter++;
  
  const int fieldWidth = 30;

  os << left <<
    setw (fieldWidth) <<
    "TupletBracketKind" << " : " <<
    tupletBracketKindAsString (
      fTupletBracketKind) <<
    endl <<
    setw (fieldWidth) <<
    "TupletLineShapeKind" << " : " <<
    tupletLineShapeKindAsString (
      fTupletLineShapeKind) <<
    endl <<
    setw (fieldWidth) <<
    "TupletShowNumberKind" << " : " <<
    tupletShowNumberKindAsString (
      fTupletShowNumberKind) <<
    endl <<
    setw (fieldWidth) <<
    "TupletShowTypeKind" << " : " <<
    tupletShowTypeKindAsString (
      fTupletShowTypeKind) <<
    endl <<
    setw (fieldWidth) <<
    "MemberNotesSoundingWholeNotes" << " : " <<
    fMemberNotesSoundingWholeNotes <<
    endl <<
    setw (fieldWidth) <<
    "MemberNotesDisplayWholeNotes" << " : " <<
    fMemberNotesDisplayWholeNotes <<
    endl <<
    endl;

/* JMI ???
  os << left <<
    setw (fieldWidth) <<
    "(position in measure" << " : ";
  if (fTupletPositionInMeasure.getNumerator () < 0)
    os << "???)";
  else
    os << fTupletPositionInMeasure << ")";
  os <<
    endl;
    */

  gIndenter--;

  if (fTupletElementsList.size ()) {
    gIndenter++;

    list<S_msrElement>::const_iterator
      iBegin = fTupletElementsList.begin (),
      iEnd   = fTupletElementsList.end (),
      i      = iBegin;
    for ( ; ; ) {
      os << (*i);
      if (++i == iEnd) break;
      // no endl here
    } // for
    
    gIndenter--;
    
  // JMI  os << endl;
  }
}

void msrTuplet::printShort (ostream& os)
{
  os <<
    "Tuplet " <<
    fTupletFactor.asString () <<
    ", " <<
    singularOrPlural (
      fTupletElementsList.size (), "element", "elements") <<
    ", whole notes: " <<
    fTupletSoundingWholeNotes << " sound, " <<
    fTupletDisplayWholeNotes << " disp" <<
    ", meas "<<
    fTupletMeasureNumber <<
    ", line " << fInputLineNumber <<
    endl;

  gIndenter++;
  
  const int fieldWidth = 30;

  os << left <<
    setw (fieldWidth) <<
    "TupletBracketKind" << " : " <<
    tupletBracketKindAsString (
      fTupletBracketKind) <<
    endl <<
    setw (fieldWidth) <<
    "TupletLineShapeKind" << " : " <<
    tupletLineShapeKindAsString (
      fTupletLineShapeKind) <<
    endl <<
    setw (fieldWidth) <<
    "TupletShowNumberKind" << " : " <<
    tupletShowNumberKindAsString (
      fTupletShowNumberKind) <<
    endl <<
    setw (fieldWidth) <<
    "TupletShowTypeKind" << " : " <<
    tupletShowTypeKindAsString (
      fTupletShowTypeKind) <<
    endl <<
    setw (fieldWidth) <<
    "MemberNotesSoundingWholeNotes" << " : " <<
    fMemberNotesSoundingWholeNotes <<
    endl <<
    setw (fieldWidth) <<
    "MemberNotesDisplayWholeNotes" << " : " <<
    fMemberNotesDisplayWholeNotes <<
    endl <<
    endl;

/* JMI ???
  os << left <<
    setw (fieldWidth) <<
    "(position in measure" << " : ";
  if (fTupletPositionInMeasure.getNumerator () < 0)
    os << "???)";
  else
    os << fTupletPositionInMeasure << ")";
  os <<
    endl;
    */

  gIndenter--;

  if (fTupletElementsList.size ()) {
    os <<
      "TupletElements:" <<
      endl;
      
    gIndenter++;

    list<S_msrElement>::const_iterator
      iBegin = fTupletElementsList.begin (),
      iEnd   = fTupletElementsList.end (),
      i      = iBegin;
    for ( ; ; ) {
      os <<
        (*i)->asShortString () <<
        endl;
      if (++i == iEnd) break;
      os << endl;
    } // for
    
    gIndenter--;
    
  // JMI  os << endl;
  }
}

//______________________________________________________________________________
S_msrGlissando msrGlissando::create (
  int                  inputLineNumber,
  int                  glissandoNumber,
  msrGlissandoTypeKind glissandoTypeKind,
  msrLineTypeKind      glissandoLineTypeKind,
  string               glissandoTextValue)
{
  msrGlissando* o =
    new msrGlissando (
      inputLineNumber,
      glissandoNumber,
      glissandoTypeKind,
      glissandoLineTypeKind,
      glissandoTextValue);
  assert(o!=0);
  return o;
}

msrGlissando::msrGlissando (
  int                  inputLineNumber,
  int                  glissandoNumber,
  msrGlissandoTypeKind glissandoTypeKind,
  msrLineTypeKind      glissandoLineTypeKind,
  string               glissandoTextValue)
    : msrElement (inputLineNumber)
{  
  fGlissandoNumber = glissandoNumber;
  
  fGlissandoTypeKind     = glissandoTypeKind;
  fGlissandoLineTypeKind = glissandoLineTypeKind;

  fGlissandoTextValue = glissandoTextValue;
}

msrGlissando::~msrGlissando ()
{}

S_msrGlissando msrGlissando::createGlissandoNewbornClone ()
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceGlissandos) {
    gLogIOstream <<
      "Creating a newborn clone of glissando '" <<
      asString () <<
      "'" <<
      endl;
  }
#endif

  S_msrGlissando
    newbornClone =
      msrGlissando::create (
        fInputLineNumber,
        fGlissandoNumber,
        fGlissandoTypeKind,
        fGlissandoLineTypeKind,
        fGlissandoTextValue);

  return newbornClone;
}

string msrGlissando::glissandoTypeKindAsString (
  msrGlissandoTypeKind glissandoTypeKind)
{
  string result;

  switch (glissandoTypeKind) {
    case msrGlissando::kGlissandoTypeNone:
      result = "glissandoTypeNone";
      break;
    case msrGlissando::kGlissandoTypeStart:
      result = "glissandoTypeStart";
      break;
    case msrGlissando::kGlissandoTypeStop:
      result = "glissandoTypeStop";
      break;
  } // switch

  return result;
}

void msrGlissando::acceptIn (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrGlissando::acceptIn ()" <<
      endl;
  }
      
  if (visitor<S_msrGlissando>*
    p =
      dynamic_cast<visitor<S_msrGlissando>*> (v)) {
        S_msrGlissando elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrGlissando::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrGlissando::acceptOut (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrGlissando::acceptOut ()" <<
      endl;
  }

  if (visitor<S_msrGlissando>*
    p =
      dynamic_cast<visitor<S_msrGlissando>*> (v)) {
        S_msrGlissando elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrGlissando::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrGlissando::browseData (basevisitor* v)
{}

ostream& operator<< (ostream& os, const S_msrGlissando& elt)
{
  elt->print (os);
  return os;
}

string msrGlissando::asString () const
{
  stringstream s;

  s <<
    "Glissando" <<
    ", fGlissandoNumber " << fGlissandoNumber <<
    ", " << glissandoTypeKindAsString (
      fGlissandoTypeKind) <<
    ", " << msrLineTypeKindAsString (
      fGlissandoLineTypeKind) <<
    ", \"" << fGlissandoTextValue <<
    "\", line " << fInputLineNumber;
        
  return s.str ();
}

void msrGlissando::print (ostream& os)
{
  os <<
    "Glissando" <<
    ", line " << fInputLineNumber <<
    endl;

  gIndenter++;

  const int fieldWidth = 22;
  
  os << left <<
    setw (fieldWidth) <<
    "glissandoNumber " <<
    fGlissandoNumber <<
    endl <<
    setw (fieldWidth) <<
    "glissandoTypeKind" <<
    glissandoTypeKindAsString (
      fGlissandoTypeKind) <<
    endl <<
    setw (fieldWidth) <<
    "glissandoLineTypeKind" <<
    msrLineTypeKindAsString (
      fGlissandoLineTypeKind) <<
    endl <<
    setw (fieldWidth) <<
    "fGlissandoTextValue" << " : \"" <<
    fGlissandoTextValue <<
     "\"" <<
    endl;

  gIndenter--;
}

//______________________________________________________________________________
S_msrSlide msrSlide::create (
  int              inputLineNumber,
  int              slideNumber,
  msrSlideTypeKind slideTypeKind,
  msrLineTypeKind  slideLineTypeKind,
  string           slideTextValue)
{
  msrSlide* o =
    new msrSlide (
      inputLineNumber,
      slideNumber,
      slideTypeKind,
      slideLineTypeKind,
      slideTextValue);
  assert(o!=0);
  return o;
}

msrSlide::msrSlide (
  int              inputLineNumber,
  int              slideNumber,
  msrSlideTypeKind slideTypeKind,
  msrLineTypeKind  slideLineTypeKind,
  string           slideTextValue)
    : msrElement (inputLineNumber)
{  
  fSlideNumber = slideNumber;
  
  fSlideTypeKind     = slideTypeKind;
  fSlideLineTypeKind = slideLineTypeKind;

  fSlideTextValue = slideTextValue;
}

msrSlide::~msrSlide ()
{}

S_msrSlide msrSlide::createSlideNewbornClone ()
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceSlides) {
    gLogIOstream <<
      "Creating a newborn clone of slide '" <<
      asString () <<
      "'" <<
      endl;
  }
#endif

  S_msrSlide
    newbornClone =
      msrSlide::create (
        fInputLineNumber,
        fSlideNumber,
        fSlideTypeKind,
        fSlideLineTypeKind,
        fSlideTextValue);

  return newbornClone;
}

string msrSlide::slideTypeKindAsString (
  msrSlideTypeKind slideTypeKind)
{
  string result;

  switch (slideTypeKind) {
    case msrSlide::kSlideTypeNone:
      result = "slideTypeNone";
      break;
    case msrSlide::kSlideTypeStart:
      result = "slideTypeStart";
      break;
    case msrSlide::kSlideTypeStop:
      result = "slideTypeStop";
      break;
  } // switch

  return result;
}

void msrSlide::acceptIn (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrSlide::acceptIn ()" <<
      endl;
  }
      
  if (visitor<S_msrSlide>*
    p =
      dynamic_cast<visitor<S_msrSlide>*> (v)) {
        S_msrSlide elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrSlide::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrSlide::acceptOut (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrSlide::acceptOut ()" <<
      endl;
  }

  if (visitor<S_msrSlide>*
    p =
      dynamic_cast<visitor<S_msrSlide>*> (v)) {
        S_msrSlide elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrSlide::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrSlide::browseData (basevisitor* v)
{}

ostream& operator<< (ostream& os, const S_msrSlide& elt)
{
  elt->print (os);
  return os;
}

string msrSlide::asString () const
{
  stringstream s;

  s <<
    "Slide" <<
    ", fSlideNumber " << fSlideNumber <<
    ", " << slideTypeKindAsString (
      fSlideTypeKind) <<
    ", " << msrLineTypeKindAsString (
      fSlideLineTypeKind) <<
    ", \"" << fSlideTextValue <<
    "\", line " << fInputLineNumber;
        
  return s.str ();
}

void msrSlide::print (ostream& os)
{
  os <<
    "Slide" <<
    ", line " << fInputLineNumber <<
    endl;

  gIndenter++;

  const int fieldWidth = 22;
  
  os << left <<
    setw (fieldWidth) <<
    "slideNumber " <<
    fSlideNumber <<
    endl <<
    setw (fieldWidth) <<
    "slideTypeKind" <<
    slideTypeKindAsString (
      fSlideTypeKind) <<
    endl <<
    setw (fieldWidth) <<
    "slideLineTypeKind" <<
    msrLineTypeKindAsString (
      fSlideLineTypeKind) <<
    endl <<
    setw (fieldWidth) <<
    "slideTextValue" << " : \"" <<
    fSlideTextValue <<
     "\"" <<
    endl;

  gIndenter--;
}

//______________________________________________________________________________
S_msrSyllable msrSyllable::create (
  int                   inputLineNumber,
  msrSyllableKind       syllableKind,
  msrSyllableExtendKind syllableExtendKind,
  rational              syllableWholeNotes,
  msrTupletFactor       syllableTupletFactor,
  S_msrStanza           syllableStanzaUplink)
{
  msrSyllable* o =
    new msrSyllable (
      inputLineNumber,
      syllableKind,
      syllableExtendKind,
      syllableWholeNotes,
      syllableTupletFactor,
      syllableStanzaUplink);
  assert(o!=0);

  return o;
}

msrSyllable::msrSyllable (
  int                   inputLineNumber,
  msrSyllableKind       syllableKind,
  msrSyllableExtendKind syllableExtendKind,
  rational              syllableWholeNotes,
  msrTupletFactor       syllableTupletFactor,
  S_msrStanza           syllableStanzaUplink)
    : msrElement (inputLineNumber)
{
  // sanity check
  msrAssert(
    syllableStanzaUplink != nullptr,
    "syllableStanzaUplink is null");
    
  // set syllable's stanza uplink
  fSyllableStanzaUplink =
    syllableStanzaUplink;
    
  fSyllableKind = syllableKind;
  
  fSyllableExtendKind = syllableExtendKind;
  
  // fSyllableNoteUplink will be set
  // by appendSyllableToNoteAndSetItsUplink () later

  fSyllableWholeNotes = syllableWholeNotes;

  fSyllableTupletFactor = syllableTupletFactor;

#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceLyrics) {
    gLogIOstream <<
      "Creating a syllable containing:" <<
      endl;

    gIndenter++;

    print (gLogIOstream);

    gIndenter--;
  }
#endif
}

msrSyllable::~msrSyllable ()
{}

S_msrSyllable msrSyllable::createSyllableNewbornClone (
  S_msrPart containingPart)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceLyrics) {
    gLogIOstream <<
      "Creating a newborn clone of syllable '" <<
      asString () <<
      "'" <<
      endl;
  }
#endif

  // sanity check
  msrAssert(
    containingPart != nullptr,
    "containingPart is null");
    
  S_msrSyllable
    newbornClone =
      msrSyllable::create (
        fInputLineNumber,
        fSyllableKind,
        fSyllableExtendKind,
        fSyllableWholeNotes,
        fSyllableTupletFactor,
        fSyllableStanzaUplink);
    
  // append the lyric texts to the syllable clone
  for (
    list<string>::const_iterator i = fSyllableTextsList.begin ();
    i!=fSyllableTextsList.end ();
    i++) {
    newbornClone->
      appendLyricTextToSyllable ((*i));
  } // for

  // dont't set 'newbornClone->fSyllableStanzaUplink'
  // nor 'newbornClone->fSyllableNoteUplink',
  // this will be done by the caller

  newbornClone->fSyllableNoteUplink =
    fSyllableNoteUplink; // TEMP
  
  return newbornClone;
}

S_msrSyllable msrSyllable::createSyllableDeepCopy (
  S_msrPart containingPart)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceLyrics) {
    gLogIOstream <<
      "Creating a newborn clone of syllable '" <<
      asString () <<
      "'" <<
      endl;
  }
#endif

  // sanity check
  msrAssert(
    containingPart != nullptr,
    "containingPart is null");
    
  S_msrSyllable
    syllableDeepCopy =
      msrSyllable::create (
        fInputLineNumber,
        fSyllableKind,
        fSyllableExtendKind,
        fSyllableWholeNotes,
        fSyllableTupletFactor,
        fSyllableStanzaUplink);
      
  // append the lyric texts to the syllable deep copy
  for (
    list<string>::const_iterator i = fSyllableTextsList.begin ();
    i!=fSyllableTextsList.end ();
    i++) {
    syllableDeepCopy->
      appendLyricTextToSyllable ((*i));
  } // for

  // dont't set 'newbornClone->fSyllableStanzaUplink'
  // nor 'newbornClone->fSyllableNoteUplink',
  // this will be done by the caller

  syllableDeepCopy->fSyllableNoteUplink =
    fSyllableNoteUplink; // TEMP
  
  return syllableDeepCopy;
}

void msrSyllable::writeTextsList (
  const list<string>& textsList,
  ostream&            os)
{
  os << "[";

  if (textsList.size ()) {
    list<string>::const_iterator
      iBegin = textsList.begin (),
      iEnd   = textsList.end (),
      i      = iBegin;
      
    for ( ; ; ) {
      os << "\"" << quoteStringIfNonAlpha (*i) << "\"";
      if (++i == iEnd) break;
      os << ", ";
    } // for
  }

  os << "]";
} 

void msrSyllable::appendLyricTextToSyllable (string text)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceLyrics) {
    gLogIOstream <<
      "Appending text \"" <<
      text <<
      "\" to the texts list of syllable '" <<
      asString () <<
      "'" <<
      endl;
  }
#endif

  fSyllableTextsList.push_back (
    text);
}

void msrSyllable::appendSyllableToNoteAndSetItsUplink (
  S_msrNote note)
{
  // sanity check
  msrAssert (
    note != nullptr,
    "note is empty");
    
  fSyllableNoteUplink = note;

/*
  // sanity check JMI ???
  msrAssert (
    fSyllableTextsList.size () != 0,
    "fSyllableTextsList is empty");
    */
    
  // append syllable to note
  note->
    appendSyllableToNote (this);

  // set it uplink to note
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceLyrics) {
    gLogIOstream <<
      "Setting syllable note uplink for:" <<
      endl;

    gIndenter++;
    
    gLogIOstream <<
      asString () <<
    // JMI    "to '" << note->asString () <<
      ", line " << note->getInputLineNumber () <<
      endl;

    gIndenter--;
      
/*
    if (note) { // JMI
      gLogIOstream <<
        note->asString () <<
        ", line " << note->getInputLineNumber ();
    }
    else {
      gLogIOstream << "none";
    }
    
    gLogIOstream <<
      endl;
      */
  }
#endif
}

void msrSyllable::acceptIn (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrSyllable::acceptIn ()" <<
      endl;
  }
      
  if (visitor<S_msrSyllable>*
    p =
      dynamic_cast<visitor<S_msrSyllable>*> (v)) {
        S_msrSyllable elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrSyllable::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrSyllable::acceptOut (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrSyllable::acceptOut ()" <<
      endl;
  }

  if (visitor<S_msrSyllable>*
    p =
      dynamic_cast<visitor<S_msrSyllable>*> (v)) {
        S_msrSyllable elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrSyllable::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrSyllable::browseData (basevisitor* v)
{}

string msrSyllable::syllableWholeNotesAsMsrString () const
{
  string result;

  if (fSyllableNoteUplink) { // JMI
    switch (fSyllableNoteUplink->getNoteKind ()) {
      case msrNote::k_NoNoteKind:
      case msrNote::kRestNote:
      case msrNote::kUnpitchedNote:
      case msrNote::kStandaloneNote:
      case msrNote::kDoubleTremoloMemberNote:
      case msrNote::kGraceNote:
      case msrNote::kGraceChordMemberNote:
      case msrNote::kChordMemberNote:
        result =
          wholeNotesAsMsrString (
            fInputLineNumber,
            fSyllableWholeNotes);
        break;
        
      case msrNote::kSkipNote:
      case msrNote::kTupletMemberNote:
      case msrNote::kTupletMemberUnpitchedNote:
        {
          stringstream s;

          s <<
            fSyllableNoteUplink->
      // JMI        noteSoundingWholeNotesAsMsrString () <<
              noteDisplayWholeNotesAsMsrString () <<
            "*" <<
            fSyllableTupletFactor.fTupletNormalNotes <<
            "/" <<
            fSyllableTupletFactor.fTupletActualNotes;

          result = s.str ();
        }
        break;
    } // switch
  }

  else {
    result =
      wholeNotesAsMsrString (
        fInputLineNumber,
          fSyllableWholeNotes);
  }
      
  return result;
}

string msrSyllable::syllableKindAsString (
  msrSyllableKind syllableKind)
{
  string result;
    
  switch (syllableKind) {
    case msrSyllable::kSyllableSingle:
      result = "syllableSingle";
      break;
      
    case msrSyllable::kSyllableBegin:
      result = "syllableBegin";
      break;
    case msrSyllable::kSyllableMiddle:
      result = "syllableMiddle";
      break;
    case msrSyllable::kSyllableEnd:
      result = "syllableEnd";
      break;
      
    case msrSyllable::kSyllableSkip:
      result = "syllableSkip";
      break;
      
    case msrSyllable::kSyllableMeasureEnd:
      result = "syllableMeasureEnd";
      break;
      
    case msrSyllable::kSyllableLineBreak:
      result = "syllableLineBreak";
      break;
    case msrSyllable::kSyllablePageBreak:
      result = "syllablePageBreak";
      break;
      
    case msrSyllable::kSyllableNone:
      result = "syllableNone";
      break;
  } // switch

  return result;
}

string msrSyllable::syllableKindAsString () const
{
  return syllableKindAsString (fSyllableKind);
}

string msrSyllable::syllableExtendKindAsString (
  msrSyllableExtendKind syllableExtendKind)
{
  string result;
  
  switch (syllableExtendKind) {
    case msrSyllable::kSyllableExtendSingle:
      result = "syllableExtendSingle";
      break;
    case msrSyllable::kSyllableExtendStart:
      result = "syllableExtendStart";
      break;
    case msrSyllable::kSyllableExtendContinue:
      result = "syllableExtendContinue";
      break;
    case msrSyllable::kSyllableExtendStop:
      result = "syllableExtendStop";
      break;
    case msrSyllable::kSyllableExtendNone:
      result = "syllableExtendNone";
      break;
  } // switch

  return result;
}

string msrSyllable::syllableExtendKindAsString () const
{
  return syllableExtendKindAsString (fSyllableExtendKind);
}

string msrSyllable::syllableNoteUplinkAsString () const
{
  string result;

  if (fSyllableNoteUplink)
    result = fSyllableNoteUplink->asString ();
  else
    result = "none";

  return result;
}

string msrSyllable::asString () const
{
  stringstream s;

  s <<
    "Syllable '" <<
    syllableKindAsString () <<
    "', syllableExtendKind: " <<
      syllableExtendKindAsString (fSyllableExtendKind) <<
    ", whole notes:" <<
    syllableWholeNotesAsMsrString () <<
    " (" << fSyllableWholeNotes << ")" <<
    ", syllableTupletFactor: " << fSyllableTupletFactor <<
    ", line " << fInputLineNumber <<
    ", texts list: ";
    
  writeTextsList (
    fSyllableTextsList,
    s);

  s <<
    ", " <<
    syllableNoteUplinkAsString ();

  switch (fSyllableKind) {
    case msrSyllable::kSyllableSingle:
    case msrSyllable::kSyllableBegin:
    case msrSyllable::kSyllableMiddle:
    case msrSyllable::kSyllableEnd:
    case msrSyllable::kSyllableSkip:
      break;
 
    case msrSyllable::kSyllableMeasureEnd:
      // fSyllableText contains the measure number
      s << 
        " measure ";

      writeTextsList (
        fSyllableTextsList,
        s);
      break;
      
    case msrSyllable::kSyllableLineBreak:
      // fSyllableText contains the measure number
      s << 
        " measure ";

      writeTextsList (
        fSyllableTextsList,
        s);
      break;
      
    case msrSyllable::kSyllablePageBreak:
      // fSyllableText contains the measure number JMI ???
      s << 
        " measure ";

      writeTextsList (
        fSyllableTextsList,
        s);
      break;
      
    case msrSyllable::kSyllableNone:
      msrInternalError (
        gXml2lyOptions->fInputSourceName,
        fInputLineNumber,
        __FILE__, __LINE__,
        "syllable type has not been set");
      break;
  } // switch

  s <<
    ", in stanza " <<
    fSyllableStanzaUplink->getStanzaName ();

  return s.str ();
}

void msrSyllable::print (ostream& os)
{  
  os <<
    "Syllable" <<
    ", syllableKind: " <<
    syllableKindAsString () <<
    ", line " << fInputLineNumber <<
    endl;

  gIndenter++;

  const int fieldWidth = 19;
  os << left <<
    setw (fieldWidth) <<
    "syllableExtendKind" << " : " <<
    syllableExtendKindAsString (
      fSyllableExtendKind) <<
    endl <<
    setw (fieldWidth) <<
    "texts list" << " : ";
    
    writeTextsList (
      fSyllableTextsList,
      os);

  os << left <<
    endl <<
    setw (fieldWidth) <<
    "noteUplink" << " : " <<
    syllableNoteUplinkAsString () <<      
    endl <<
    setw (fieldWidth) <<
    "syllableKind" << " : " <<
    syllableKindAsString (fSyllableKind) <<
    endl;
    
  switch (fSyllableKind) { // JMI
    case msrSyllable::kSyllableSingle:
    case msrSyllable::kSyllableBegin:
    case msrSyllable::kSyllableMiddle:
    case msrSyllable::kSyllableEnd:
    case msrSyllable::kSyllableSkip:
      os << left <<
        setw (fieldWidth) <<
       "syllableWholeNotes" << " : " <<
        syllableWholeNotesAsMsrString () <<
        " (" << fSyllableWholeNotes << ")" <<
        endl <<
        setw (fieldWidth) <<
       "syllableTupletFactor" << " : " <<
        fSyllableTupletFactor;
     break;
      
    case kSyllableMeasureEnd:
    /* JMI
      // fSyllableText contains the measure number
      os << left <<
        endl <<
        setw (fieldWidth) <<
        "fSyllableTextsList [0]" << " : ";
        "measure '" << fSyllableTextsList.front () << "'";
        */
      break;
      
    case kSyllableLineBreak:
    /* JMI
      // fSyllableText contains the measure number
      os << left <<
        endl <<
        setw (fieldWidth) <<
        "measure '" << "fSyllableText ???" << "'";
        */
      break;
      
    case kSyllablePageBreak:
    /* JMI
      // fSyllableText contains the measure number JMI ???
      os << left <<
        endl <<
        setw (fieldWidth) <<
        "measure '" << "fSyllableText ???" << "'";
        */
      break;
      
    case kSyllableNone:
      msrInternalError (
        gXml2lyOptions->fInputSourceName,
        fInputLineNumber,
        __FILE__, __LINE__,
        "syllable type has not been set");
      break;
  } // switch
  os <<
    endl;

  os << left <<
    setw (fieldWidth) <<
    "Stanza" << " : " <<
    fSyllableStanzaUplink->getStanzaName () <<
    endl;

  gIndenter--;
}

ostream& operator<< (ostream& os, const S_msrSyllable& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_msrStanza msrStanza::create (
  int           inputLineNumber,
  string        stanzaNumber,
  S_msrVoice    stanzaVoiceUplink)
{
  msrStanza* o =
    new msrStanza (
      inputLineNumber,
      stanzaNumber,
      stanzaVoiceUplink);
  assert(o!=0);

  return o;
}

msrStanza::msrStanza (
  int           inputLineNumber,
  string        stanzaNumber,
  S_msrVoice    stanzaVoiceUplink)
    : msrElement (inputLineNumber)
{
  // set stanza number and kind
  fStanzaNumber = stanzaNumber;

  // sanity check
  msrAssert(
    stanzaVoiceUplink != nullptr,
    "stanzaVoiceUplink is null");

  // set stanza's voice uplink
  fStanzaVoiceUplink =
    stanzaVoiceUplink;
  
  // do other initializations
  initializeStanza ();
}

void msrStanza::initializeStanza ()
{
  fStanzaName =
    fStanzaVoiceUplink->getVoiceName () +
    "_Stanza_" + 
      stringNumbersToEnglishWords (
        makeSingleWordFromString (
          fStanzaNumber));
 
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceLyrics) {
    gLogIOstream <<
      "Initializing stanza " << getStanzaName () <<
      endl;
  }
#endif

  fStanzaTextPresent = false;

  fStanzaCurrentMeasureLength = rational (0, 1);
}

msrStanza::~msrStanza ()
{}

S_msrStanza msrStanza::createStanzaNewbornClone (
  S_msrVoice containingVoice)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceLyrics) {
    gLogIOstream <<
      "Creating a newborn clone of stanza \"" <<
      getStanzaName () <<
      "\" in voice \"" <<
      containingVoice->getVoiceName () <<
      "\"" <<
      endl;
  }
#endif

  // sanity check
  msrAssert(
    containingVoice != nullptr,
    "containingVoice is null");
    
  S_msrStanza
    newbornClone =
      msrStanza::create (
        fInputLineNumber,
        fStanzaNumber,
        containingVoice);

  // number

  // kind

  // name
  newbornClone->fStanzaName =
    fStanzaName;

  // contents
  newbornClone->fStanzaTextPresent =
    fStanzaTextPresent;

  // uplinks
  newbornClone->fStanzaVoiceUplink =
    containingVoice;
  
  return newbornClone;
}

S_msrStanza msrStanza::createStanzaDeepCopy (
  S_msrVoice containingVoice)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceLyrics) {
    gLogIOstream <<
      "Creating a deep copy of stanza \"" <<
      getStanzaName () <<
      "\" in voice \"" <<
      containingVoice->getVoiceName () <<
      "\"" <<
      endl;
  }
#endif

  // sanity check
  msrAssert(
    containingVoice != nullptr,
    "containingVoice is null");
    
  S_msrStanza
    stanzaDeepCopy =
      msrStanza::create (
        fInputLineNumber,
        fStanzaNumber,
        containingVoice);

  // number

  // kind

  // name
  stanzaDeepCopy->fStanzaName =
    fStanzaName;

  // contents
  int n = fSyllables.size ();
  for (int i = 0; i < n; i++) {
    stanzaDeepCopy->fSyllables.push_back (
      fSyllables [i]->
        createSyllableDeepCopy (
          containingVoice->
            fetchVoicePartUplink ()));
  } // for

  stanzaDeepCopy->fStanzaTextPresent =
    fStanzaTextPresent;

  // uplinks
  stanzaDeepCopy->fStanzaVoiceUplink =
    containingVoice;
  
  return stanzaDeepCopy;
}

void msrStanza::appendSyllableToStanza (
  S_msrSyllable syllable)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceLyrics) {
    gLogIOstream <<
      "Appending syllable '" << syllable->asString () <<
      "' to stanza " << getStanzaName () <<
      endl;
  }
#endif
      
  // append the syllable to this stanza
  fSyllables.push_back (syllable);

  // does this stanza contain text?
  switch (syllable->getSyllableKind ()) {
    
    case msrSyllable::kSyllableSingle:
    case msrSyllable::kSyllableBegin:
    case msrSyllable::kSyllableMiddle:
    case msrSyllable::kSyllableEnd:
      // only now, in case addSyllableToStanza () is called
      // from LPSR for example
      fStanzaTextPresent = true;
      break;
      
    case msrSyllable::kSyllableSkip:
    case msrSyllable::kSyllableMeasureEnd:
    case msrSyllable::kSyllableLineBreak:
    case msrSyllable::kSyllablePageBreak:
      break;
      
    case msrSyllable::kSyllableNone:
      msrInternalError (
        gXml2lyOptions->fInputSourceName,
        fInputLineNumber,
        __FILE__, __LINE__,
        "syllable type has not been set");
      break;
  } // switch

/* JMI
  // get the syllable's sounding whole notes
  rational
    syllableSoundingWholeNotes =
      syllable->
        getSyllableNoteUplink ()->
          getNoteSoundingWholeNotes ();

  // update the stanza's current measure length
  fStanzaCurrentMeasureLength +=syllableSoundingWholeNotes;
  */
}

S_msrSyllable msrStanza::appendRestSyllableToStanza (
  int      inputLineNumber,
  rational wholeNotes)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceLyrics) {
    gLogIOstream <<
      "Appending 'Rest' syllable" <<
      " to stanza " << getStanzaName () <<
      ", whole notes = " << wholeNotes <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  gIndenter++;
  
  // create stanza rest syllable
  S_msrSyllable
    syllable =
      msrSyllable::create (
        inputLineNumber,
        msrSyllable::kSyllableSkip, // JMI ??? kSyllableRest,
        msrSyllable::kSyllableExtendNone,
        wholeNotes,
        msrTupletFactor (),
        this);

  // append syllable to this stanza
  appendSyllableToStanza (syllable);

  gIndenter--;
  
  // and return it
  return syllable;
}

S_msrSyllable msrStanza::appendSkipSyllableToStanza (
  int      inputLineNumber,
  rational wholeNotes)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceLyrics) {
    gLogIOstream <<
      "Appending 'Skip' syllable " <<
      " to stanza " << getStanzaName () <<
      ", whole notes = " << wholeNotes <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  gIndenter++;
  
  // create stanza skip syllable
  S_msrSyllable
    syllable =
      msrSyllable::create (
        inputLineNumber,
        msrSyllable::kSyllableSkip,
        msrSyllable::kSyllableExtendNone,
        wholeNotes,
        msrTupletFactor (),
        this);

  // append syllable to this stanza
  appendSyllableToStanza (syllable);

  gIndenter--;
  
  // and return it
  return syllable;
}

S_msrSyllable msrStanza::appendMeasureEndSyllableToStanza (
  int inputLineNumber)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceLyrics) {
    gLogIOstream <<
      "Appending 'Measure end' syllable " <<
      " to stanza " << getStanzaName () <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  gIndenter++;
  
  // create stanza skip syllable
  S_msrSyllable
    syllable =
      msrSyllable::create (
        inputLineNumber,
        msrSyllable::kSyllableMeasureEnd,
        msrSyllable::kSyllableExtendNone,
        0, // wholeNotes
        msrTupletFactor (),
        this);

  // append syllable to this stanza
  appendSyllableToStanza (syllable);

  // reset current measure length
  fStanzaCurrentMeasureLength = rational (0, 1);

  gIndenter--;
  
  // and return it
  return syllable;
}

S_msrSyllable msrStanza::appendMelismaSyllableToStanza (
  int             inputLineNumber,
  msrSyllable::msrSyllableKind
                  syllableKind,
  rational        wholeNotes)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceLyrics) {
    gLogIOstream <<
      "Appending '" <<
      msrSyllable::syllableKindAsString (syllableKind) <<
      "' syllable" <<
      " to stanza " << getStanzaName () <<
      ", whole notes = " << wholeNotes <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  gIndenter++;
  
  // create stanza melisma syllable
  S_msrSyllable
    syllable =
      msrSyllable::create (
        inputLineNumber,
        syllableKind,
        msrSyllable::kSyllableExtendNone,
        wholeNotes,
        msrTupletFactor (),
        this);

  // append syllable to this stanza
  appendSyllableToStanza (syllable);

  gIndenter--;
  
  // and return it
  return syllable;
}

S_msrSyllable msrStanza::appendLineBreakSyllableToStanza (
  int inputLineNumber)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceLyrics) {
    gLogIOstream <<
      "Appending 'line break' syllable" <<
      " to stanza " << getStanzaName () <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  gIndenter++;
  
  // create stanza line break syllable
  S_msrSyllable
    syllable =
      msrSyllable::create (
        inputLineNumber,
        msrSyllable::kSyllableLineBreak,
        msrSyllable::kSyllableExtendNone,
        0, // whole notes
        msrTupletFactor (),
        this);

  // append syllable to this stanza
  appendSyllableToStanza (syllable);

  gIndenter--;
  
  // and return it
  return syllable;
}

/* JMI
S_msrSyllable msrStanza::appendTiedSyllableToStanza (
  int      inputLineNumber,
  rational wholeNotes)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceLyrics) {
    gLogIOstream <<
      "Appending 'Tied' syllable" <<
      " to stanza " << getStanzaName () <<
      ", whole notes = " << wholeNotes <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  gIndenter++;
  * 
  // create stanza tied syllable
  S_msrSyllable
    syllable =
      msrSyllable::create (
        inputLineNumber,
        msrSyllable::kTiedSyllable,
        msrSyllable::kSyllableExtendNone,
        wholeNotes,
        this);
        
  // append syllable to this stanza
  appendSyllableToStanza (syllable);

  gIndenter--;
  * 
  // and return it
  return syllable;
}

S_msrSyllable msrStanza::appendSlurSyllableToStanza (
  int      inputLineNumber,
  rational wholeNotes)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceLyrics) {
    gLogIOstream <<
      "Appending 'Slur' syllable" <<
      " to stanza " << getStanzaName () <<
      ", whole notes = " << wholeNotes <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  gIndenter++;
   
  // create stanza slur syllable
  S_msrSyllable
    syllable =
      msrSyllable::create (
        inputLineNumber,
        msrSyllable::kSlurSyllable,
        msrSyllable::kSyllableExtendNone,
        wholeNotes,
        this);
        
  // append syllable to this stanza
  appendSyllableToStanza (syllable);

  gIndenter--;
  
  // and return it
  return syllable;
}

S_msrSyllable msrStanza::appendSlurBeyondEndSyllableToStanza (
  int      inputLineNumber,
  rational wholeNotes)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceLyrics) {
    gLogIOstream <<
      "Appending a 'SlurBeyondEnd' syllable" <<
      " to stanza " << getStanzaName () <<
      ", whole notes = " << wholeNotes <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif
  
  // create stanza slur syllable
  S_msrSyllable
    syllable =
      msrSyllable::create (
        inputLineNumber,
        msrSyllable::kSlurBeyondEndSyllable,
        msrSyllable::kSyllableExtendNone,
        wholeNotes,
        this);
        
  // append syllable to this stanza
  appendSyllableToStanza (syllable);

  // and return it
  return syllable;
}

S_msrSyllable msrStanza::appendLigatureSyllableToStanza (
  int      inputLineNumber,
  rational wholeNotes)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceLyrics) {
    gLogIOstream <<
      "Appending 'Ligature' syllable:" <<
      " to stanza " << getStanzaName () <<
      ", whole notes = " << wholeNotes <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif
  
  // create stanza ligature syllable
  S_msrSyllable
    syllable =
      msrSyllable::create (
        inputLineNumber,
        msrSyllable::kLigatureSyllable,
        msrSyllable::kSyllableExtendNone,
        wholeNotes,
        this);
        
  // append syllable to this stanza
  appendSyllableToStanza (syllable);

  // and return it
  return syllable;
}

S_msrSyllable msrStanza::appendLigatureBeyondEndSyllableToStanza (
  int      inputLineNumber,
  rational wholeNotes)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceLyrics) {
    gLogIOstream <<
      "Appending a 'LigatureBeyondEnd' syllable" <<
      " to stanza " << getStanzaName () <<
      ", whole notes = " << wholeNotes <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif
  
  // create stanza ligature syllable
  S_msrSyllable
    syllable =
      msrSyllable::create (
        inputLineNumber,
        msrSyllable::kLigatureBeyondEndSyllable,
        msrSyllable::kSyllableExtendNone,
        wholeNotes,
        this);
        
  // append syllable to this stanza
  appendSyllableToStanza (syllable);

  // and return it
  return syllable;
}

S_msrSyllable msrStanza::appendBarcheckSyllableToStanza (
  int    inputLineNumber,
  string nextMeasureNumber)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceLyrics) {
    gLogIOstream <<
      "Appending a 'barcheck' syllable" <<
      " to stanza " << getStanzaName () <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  // create stanza break syllable
  S_msrSyllable
    syllable =
      msrSyllable::create (
        inputLineNumber,
        msrSyllable::kBarcheckSyllable,
 // JMI       nextMeasureNumber,
        msrSyllable::kSyllableExtendNone,
        0,  // wholeNotes
        this);
       
  // append syllable to this stanza
  appendSyllableToStanza (syllable);

  // and return it
  return syllable;
}

S_msrSyllable msrStanza::appendBarNumberCheckSyllableToStanza (
  int    inputLineNumber,
  string nextMeasureNumber)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceLyrics) {
    gLogIOstream <<
      "Appending a 'bar number check' syllable" <<
      " to stanza " << getStanzaName () <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  // create stanza break syllable        
  S_msrSyllable
    syllable =
      msrSyllable::create (
        inputLineNumber,
        msrSyllable::kBarNumberCheckSyllable,
 // JMI  nextMeasureNumber,
        msrSyllable::kSyllableExtendNone,
        0,  // wholeNotes
        this);
       
  // append syllable to this stanza
  appendSyllableToStanza (syllable);

  // and return it
  return syllable;
}
*/

S_msrSyllable msrStanza::appendLineBreakSyllableToStanza (
  int    inputLineNumber,
  string nextMeasureNumber)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceLyrics) {
    gLogIOstream <<
      "Appending a 'LineBreak' syllable" <<
      " to stanza " << getStanzaName () <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  gIndenter++;
  
  // create stanza break syllable
  S_msrSyllable
    syllable =
      msrSyllable::create (
        inputLineNumber,
        msrSyllable::kSyllableLineBreak,
 // JMI  nextMeasureNumber,
        msrSyllable::kSyllableExtendNone,
        0, // whole notes
        msrTupletFactor (),
        this);
        
  // append syllable to this stanza
  appendSyllableToStanza (syllable);

  gIndenter--;
  
  // and return it
  return syllable;
}

S_msrSyllable msrStanza::appendPageBreakSyllableToStanza (
  int    inputLineNumber,
  string nextMeasureNumber)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceLyrics) {
    gLogIOstream <<
      "Appending a 'PageBreak' syllable" <<
      " to stanza " << getStanzaName () <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  gIndenter++;
  
  // create stanza break syllable
  S_msrSyllable
    syllable =
      msrSyllable::create (
        inputLineNumber,
        msrSyllable::kSyllablePageBreak,
 // JMI  nextMeasureNumber,
        msrSyllable::kSyllableExtendNone,
        0, // whole notes
        msrTupletFactor (),
        this);
        
  // append syllable to this stanza
  appendSyllableToStanza (syllable);

  gIndenter--;
  
  // and return it
  return syllable;
}

void msrStanza::padUpToMeasureLengthInStanza (
  int      inputLineNumber,
  rational measureLength)
{
  /* JMI
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceLyrics || gTraceOptions->fTraceMeasures) {
    gLogIOstream <<
      "Padding up to measure length '" << measureLength <<
      "' in stanza \"" <<
      fStanzaName <<
      "\" in voice \"" <<
      fStanzaVoiceUplink->getVoiceName () <<
      "\", line " << inputLineNumber <<
      endl;
  }
#endif

  // JMI ???
  rational
    paddingLength =
      measureLength - fStanzaCurrentMeasureLength;

  gIndenter++;
  
  if (fMeasureLength < measureLength) {
    // appending a padding rest or skip to this measure to reach measureLength
    rational
      missingDuration =
        measureLength - fMeasureLength;
    
    // fetch the measure voice
    S_msrVoice
      measureVoice =
        fMeasureSegmentUplink->
          getSegmentVoiceUplink ();
      
    // create a rest or a skip depending on measureVoice kind
    S_msrNote
      paddingNote =
        createPaddingNoteForVoice (
          inputLineNumber,
          missingDuration,
          measureVoice);

    // register rest's measure length
    paddingNote->
      setNotePositionInMeasure (
        fMeasureLength);
           
  #ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceMeasures || gTraceOptions->fTraceDivisions) {
      gLogIOstream <<
       "Appending rest" << paddingNote->asString () <<
       " (missingDuration " << missingDuration <<
       " whole notes) to skip from length '" << fMeasureLength <<
       " to length '" << measureLength << "'"
       " in measure '" << fMeasureNumber <<
       "in voice \"" << measureVoice->getVoiceName () <<
       endl;
    }
#endif

    // append the rest to the measure elements list
    // only now to make it possible to remove it afterwards
    // if it happens to be the first note of a chord
    appendNoteToMeasure (paddingNote);
  }
  */
}

void msrStanza::acceptIn (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrStanza::acceptIn ()" <<
      endl;
  }
      
  if (visitor<S_msrStanza>*
    p =
      dynamic_cast<visitor<S_msrStanza>*> (v)) {
        S_msrStanza elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrStanza::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrStanza::acceptOut (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrStanza::acceptOut ()" <<
      endl;
  }

  if (visitor<S_msrStanza>*
    p =
      dynamic_cast<visitor<S_msrStanza>*> (v)) {
        S_msrStanza elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrStanza::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrStanza::browseData (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrStanza::browseData ()" <<
      endl;
  }

  gIndenter++;
  
  // browse the syllables
  int n = fSyllables.size ();
  for (int i = 0; i < n; i++) {
    // browse the syllable
    msrBrowser<msrSyllable> browser (v);
    browser.browse (*fSyllables [i]);
  } // for
  gLogIOstream <<
    endl;

  gIndenter--;

  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% <== msrStanza::browseData ()" <<
      endl;
  }
}

ostream& operator<< (ostream& os, const S_msrStanza& elt)
{
  elt->print (os);
  return os;
}

void msrStanza::print (ostream& os)
{
  os <<
    "Stanza " << getStanzaName () <<
    " (number \"" <<
    fStanzaNumber <<
    "\", " <<
    fSyllables.size () << " syllables)" <<
    endl;
    
  gIndenter++;

  if (! fStanzaTextPresent) {
    os <<
      "(No actual text)" <<
      endl;
  }

  else {
    if (fSyllables.size ()) {
      vector<S_msrSyllable>::const_iterator
        iBegin = fSyllables.begin (),
        iEnd   = fSyllables.end (),
        i      = iBegin;
        
      for ( ; ; ) {
        os << (*i);
        if (++i == iEnd) break;
        // no endl here
      } // for
    }
  }

  gIndenter--;
}

//______________________________________________________________________________
S_msrHarmonyDegree msrHarmonyDegree::create (
  int                      inputLineNumber,
  int                      harmonyDegreeValue,
  msrAlterationKind        harmonyDegreeAlterationKind,
  msrHarmonyDegreeTypeKind harmonyDegreeTypeKind)
{
  msrHarmonyDegree* o =
    new msrHarmonyDegree (
      inputLineNumber,
      harmonyDegreeValue,
      harmonyDegreeAlterationKind,
      harmonyDegreeTypeKind);
  assert(o!=0);

  return o;
}

msrHarmonyDegree::msrHarmonyDegree (
  int                      inputLineNumber,
  int                      harmonyDegreeValue,
  msrAlterationKind        harmonyDegreeAlterationKind,
  msrHarmonyDegreeTypeKind harmonyDegreeTypeKind)
    : msrElement (inputLineNumber)
{
  fHarmonyDegreeValue          = harmonyDegreeValue;
  fHarmonyDegreeAlterationKind = harmonyDegreeAlterationKind;
  fHarmonyDegreeTypeKind       = harmonyDegreeTypeKind;

#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceHarmonies) {
    gLogIOstream <<
      "Creating harmony degree '" <<
      asString () <<
      "', line " << inputLineNumber <<
      endl;
  }
#endif
}

msrHarmonyDegree::~msrHarmonyDegree ()
{}

void msrHarmonyDegree::setHarmonyDegreeHarmonyUplink (
  S_msrHarmony harmonyUplink)
{
  // sanity check
  msrAssert(
    harmonyUplink != nullptr,
     "harmonyUplink is null");
     
  fHarmonyDegreeHarmonyUplink =
    harmonyUplink;
}

int msrHarmonyDegree::harmonyDegreeAsSemitones () const
{
/*
  Kind indicates the type of chord. Degree elements
  can then add, subtract, or alter from these
  starting points.
*/

  int result = -1;

/*
  switch (fHarmonyDegreeAlteration) {
    case kDoubleFlat:
      result = "DoubleFlat";
      break;
    case kSesquiFlat:
      result = "SesquiFlat";
      break;
    case kFlat:
      result = "Flat";
      break;
    case kSemiFlat:
      result = "SemiFlat";
      break;
    case kNatural:
      result = "Natural";
      break;
    case kSemiSharp:
      result = "SemiSharp";
      break;
    case kSharp:
      result = "Sharp";
      break;
    case kSesquiSharp:
      result = "SesquiSharp";
      break;
    case kDoubleSharp:
      result = "DoubleSharp";
      break;
    case k_NoAlteration:
      result = "alteration???";
      break;
  } // switch

  switch (fHarmonyDegreeTypeKind) {
    case msrHarmonyDegree::kHarmonyDegreeAddType:
      result = "Add";
      break;
    case msrHarmonyDegree::kHarmonyDegreeAlterType:
      result = "Alter";
      break;
    case msrHarmonyDegree::kHarmonyDegreeSubtractType:
      result = "Subtract";
      break;
  } // switch
*/

  // determine the value for an unaltered degree value
  switch (fHarmonyDegreeValue) {
    case 0:
      result = 0;
      break;
    case 1:
      result = 0;
      break;
    case 2:
      result = 0;
      break;
    case 3:
      result = 0;
      break;
    case 4:
      result = 0;
      break;
    case 5:
      result = 0;
      break;
    case 6:
      result = 0;
      break;
    case 7:
      result = 0;
      break;
    case 8:
      result = 0;
      break;
    case 9:
      result = 0;
      break;
    case 10:
      result = 0;
      break;
    case 11:
      result = 0;
      break;
    case 12:
      result = 0;
      break;
    case 13:
      result = 0;
      break;
  } // switch

  return result;
}

void msrHarmonyDegree::acceptIn (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrHarmonyDegree::acceptIn ()" <<
      endl;
  }
      
  if (visitor<S_msrHarmonyDegree>*
    p =
      dynamic_cast<visitor<S_msrHarmonyDegree>*> (v)) {
        S_msrHarmonyDegree elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrHarmonyDegree::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrHarmonyDegree::acceptOut (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrHarmonyDegree::acceptOut ()" <<
      endl;
  }

  if (visitor<S_msrHarmonyDegree>*
    p =
      dynamic_cast<visitor<S_msrHarmonyDegree>*> (v)) {
        S_msrHarmonyDegree elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrHarmonyDegree::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrHarmonyDegree::browseData (basevisitor* v)
{}

string msrHarmonyDegree::harmonyDegreeTypeKindAsString (
  msrHarmonyDegreeTypeKind harmonyDegreeTypeKind)
{
  string result;

  switch (harmonyDegreeTypeKind) {
    case msrHarmonyDegree::kHarmonyDegreeTypeAdd:
      result = "harmonyDegreeTypeAdd";
      break;
    case msrHarmonyDegree::kHarmonyDegreeTypeAlter:
      result = "harmonyDegreeTypeAlter";
      break;
    case msrHarmonyDegree::kHarmonyDegreeTypeSubstract:
      result = "harmonyDegreeTypeSubstract";
      break;
  } // switch

  return result;
}

string msrHarmonyDegree::harmonyDegreeKindAsShortString () const
{
  string result;
  
  switch (fHarmonyDegreeTypeKind) {
    case msrHarmonyDegree::kHarmonyDegreeTypeAdd:
      result = "degreeAdd";
      break;
    case msrHarmonyDegree::kHarmonyDegreeTypeAlter:
      result = "degreeAlter";
      break;
    case msrHarmonyDegree::kHarmonyDegreeTypeSubstract:
      result = "degreeSubtract";
      break;
  } // switch

  return result;
}

string msrHarmonyDegree::asString () const
{
  stringstream s;

  s <<
    "HarmonyDegree" <<
    ", type: " << harmonyDegreeKindAsShortString () <<
    ", value: " << fHarmonyDegreeValue <<
    ", alteration: " <<
    msrAlterationKindAsString (
      fHarmonyDegreeAlterationKind) <<
    ", line: " << fInputLineNumber;

  return s.str ();
}

void msrHarmonyDegree::print (ostream& os)
{  
  os <<
    asString () <<
    endl;
}

ostream& operator<< (ostream& os, const S_msrHarmonyDegree& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_msrHarmony msrHarmony::create (
  int                      inputLineNumber,
  // no harmonyVoiceUplink yet
  msrQuarterTonesPitchKind harmonyRootQuarterTonesPitchKind,
  msrHarmonyKind           harmonyKind,
  string                   harmonyKindText,
  int                      harmonyInversion,
  msrQuarterTonesPitchKind harmonyBassQuarterTonesPitchKind,
  rational                 harmonySoundingWholeNotes)
{
  msrHarmony* o =
    new msrHarmony (
      inputLineNumber,
      nullptr,
      harmonyRootQuarterTonesPitchKind,
      harmonyKind,
      harmonyKindText,
      harmonyInversion,
      harmonyBassQuarterTonesPitchKind,
      harmonySoundingWholeNotes);
  assert(o!=0);

  return o;
}

S_msrHarmony msrHarmony::create (
  int                      inputLineNumber,
  S_msrVoice               harmonyVoiceUplink,
  msrQuarterTonesPitchKind harmonyRootQuarterTonesPitchKind,
  msrHarmonyKind           harmonyKind,
  string                   harmonyKindText,
  int                      harmonyInversion,
  msrQuarterTonesPitchKind harmonyBassQuarterTonesPitchKind,
  rational                 harmonySoundingWholeNotes)
{
  msrHarmony* o =
    new msrHarmony (
      inputLineNumber,
      harmonyVoiceUplink,
      harmonyRootQuarterTonesPitchKind,
      harmonyKind,
      harmonyKindText,
      harmonyInversion,
      harmonyBassQuarterTonesPitchKind,
      harmonySoundingWholeNotes);
  assert(o!=0);

  return o;
}

msrHarmony::msrHarmony (
  int                      inputLineNumber,
  S_msrVoice               harmonyVoiceUplink,
  msrQuarterTonesPitchKind harmonyRootQuarterTonesPitchKind,
  msrHarmonyKind           harmonyKind,
  string                   harmonyKindText,
  int                      harmonyInversion,
  msrQuarterTonesPitchKind harmonyBassQuarterTonesPitchKind,
  rational                 harmonySoundingWholeNotes)
    : msrElement (inputLineNumber)
{
  /* JMI
  // sanity check
  msrAssert(
    harmonyVoiceUplink != nullptr,
     "harmonyVoiceUplink is null");
     */

  // set harmony's voice uplink
  fHarmonyVoiceUplink =
    harmonyVoiceUplink;
    
  fHarmonyRootQuarterTonesPitchKind =
    harmonyRootQuarterTonesPitchKind;
 
  fHarmonyKind     = harmonyKind;
  fHarmonyKindText = harmonyKindText;
 
  fHarmonyInversion = harmonyInversion;

  fHarmonyBassQuarterTonesPitchKind =
    harmonyBassQuarterTonesPitchKind;
 
  fHarmonySoundingWholeNotes =
    harmonySoundingWholeNotes;

#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceHarmonies) {
    gLogIOstream <<
      "Creating harmony '" <<
      asString () <<
      "'" <<
      endl;
  }
#endif

  // handle harmony inversion if any
  /*
    Inversion is a number indicating which inversion is used:
    0 for root position, 1 for first inversion, etc.
  */
  if (fHarmonyInversion > 0) {
    // fetch the chord intervals
    S_msrChordStructure
      chordStructure =
        msrChordStructure::create (
  // JMI        inputLineNumber,
          fHarmonyKind);

    // fetch the bass chord item for the inversion
    S_msrChordInterval
      bassChordInterval =
        chordStructure->
          bassChordIntervalForChordInversion (
            inputLineNumber,
            fHarmonyInversion);

    // fetch the inverted chord bass semitones pitch
    msrQuarterTonesPitchKind
      invertedChordBassQuarterTonesPitchKind =
        noteAtIntervalFromQuarterTonesPitch (
          inputLineNumber,
          bassChordInterval->getChordIntervalIntervalKind (),
          fHarmonyRootQuarterTonesPitchKind);

    // is this compatible with bass quartertones pitch if specified?
    if (fHarmonyBassQuarterTonesPitchKind != k_NoQuarterTonesPitch_QTP) {
      if (
        invertedChordBassQuarterTonesPitchKind
          !=
        fHarmonyBassQuarterTonesPitchKind
        ) {
        stringstream s;
    
        s <<
          "inversion '" <<
          fHarmonyInversion <<
          "' is not compatible with bass quaternotes pitch '" <<
          msrQuarterTonesPitchKindAsString (
            gMsrOptions->fMsrQuarterTonesPitchesLanguageKind,
            fHarmonyBassQuarterTonesPitchKind) <<
          "'";
          
        msrMusicXMLError (
          gXml2lyOptions->fInputSourceName,
          inputLineNumber,
          __FILE__, __LINE__,
          s.str ());        
      }
    }

    // set the bass quartertones pitch according to the inversion
    fHarmonyBassQuarterTonesPitchKind =
      invertedChordBassQuarterTonesPitchKind;
  }
}

msrHarmony::~msrHarmony ()
{}

S_msrHarmony msrHarmony::createHarmonyNewbornClone (
  S_msrVoice containingVoice)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceHarmonies) {
    gLogIOstream <<
      "Creating a newborn clone of harmony '" <<
      msrHarmonyKindAsShortString (fHarmonyKind) <<
      "', line " << fInputLineNumber <<
      endl;
  }
#endif

  // sanity check
  msrAssert(
    containingVoice != nullptr,
    "containingVoice is null");
    
  S_msrHarmony
    newbornClone =
      msrHarmony::create (
        fInputLineNumber,
        containingVoice,
        fHarmonyRootQuarterTonesPitchKind,
        fHarmonyKind,
        fHarmonyKindText,
        fHarmonyInversion,
        fHarmonyBassQuarterTonesPitchKind,
        fHarmonySoundingWholeNotes);
        
  return newbornClone;
}

S_msrHarmony msrHarmony::createHarmonyDeepCopy (
  S_msrVoice containingVoice)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceHarmonies) {
    gLogIOstream <<
      "Creating a deep copy of harmony '" <<
      msrHarmonyKindAsShortString (fHarmonyKind) <<
      "', line " << fInputLineNumber <<
      endl;
  }
#endif

  // sanity check
  msrAssert(
    containingVoice != nullptr,
    "containingVoice is null");
    
  S_msrHarmony
    harmonyDeepCopy =
      msrHarmony::create (
        fInputLineNumber,
        containingVoice,
        fHarmonyRootQuarterTonesPitchKind,
        fHarmonyKind, fHarmonyKindText,
        fHarmonyInversion,
        fHarmonyBassQuarterTonesPitchKind,
        fHarmonySoundingWholeNotes);
        
  return harmonyDeepCopy;
}

string msrHarmony::asString () const
{
  stringstream s;

  s <<
    "Harmony" <<
    ", line " << fInputLineNumber <<
    ":" <<
    msrQuarterTonesPitchKindAsString ( // JMI XXL
      gMsrOptions->fMsrQuarterTonesPitchesLanguageKind,
      fHarmonyRootQuarterTonesPitchKind) <<          
    msrHarmonyKindAsShortString (fHarmonyKind) <<
    ", duration: " <<
    wholeNotesAsMsrString (
      fInputLineNumber,
      fHarmonySoundingWholeNotes);

  if (fHarmonyKindText.size ())
    s <<
      " (" <<fHarmonyKindText << ")";

  s << ", inversion: ";
  if (fHarmonyInversion == K_HARMONY_NO_INVERSION)
    s << "none";
  else
    s << fHarmonyInversion;
    
  if (fHarmonyBassQuarterTonesPitchKind != k_NoQuarterTonesPitch_QTP)
    s <<
      "/" <<
    msrQuarterTonesPitchKindAsString (
      gMsrOptions->fMsrQuarterTonesPitchesLanguageKind,
      fHarmonyBassQuarterTonesPitchKind);    

  if (fHarmonyDegreesList.size ()) {
    list<S_msrHarmonyDegree>::const_iterator
      iBegin = fHarmonyDegreesList.begin (),
      iEnd   = fHarmonyDegreesList.end (),
      i      = iBegin;
      
    for ( ; ; ) {
      s << (*i);
      if (++i == iEnd) break;
      s << " ";
    } // for
  }

  return s.str ();
}

void msrHarmony::acceptIn (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrHarmony::acceptIn ()" <<
      endl;
  }
      
  if (visitor<S_msrHarmony>*
    p =
      dynamic_cast<visitor<S_msrHarmony>*> (v)) {
        S_msrHarmony elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrHarmony::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrHarmony::acceptOut (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrHarmony::acceptOut ()" <<
      endl;
  }

  if (visitor<S_msrHarmony>*
    p =
      dynamic_cast<visitor<S_msrHarmony>*> (v)) {
        S_msrHarmony elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrHarmony::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrHarmony::browseData (basevisitor* v)
{
  // browse harmony degrees if any
  if (fHarmonyDegreesList.size ())
    for (
      list<S_msrHarmonyDegree>::const_iterator i = fHarmonyDegreesList.begin ();
      i != fHarmonyDegreesList.end ();
      i++) {
      // browse the harmony degree
      msrBrowser<msrHarmonyDegree> browser (v);
      browser.browse (*(*i));
    } // for
}

ostream& operator<< (ostream& os, const S_msrHarmony& elt)
{
  elt->print (os);
  return os;
}

void msrHarmony::print (ostream& os)
{  
  os <<
    "Harmony" <<
    ", " <<
    wholeNotesAsMsrString (
      fInputLineNumber,
      fHarmonySoundingWholeNotes) <<
    " (" << fHarmonySoundingWholeNotes << " sounding whole notes)" <<
     ", line " << fInputLineNumber <<
    endl;
    
  gIndenter++;

  const int fieldWidth = 15;

  os << left <<
    setw (fieldWidth) <<
    "HarmonyRoot" << " = " <<
    msrQuarterTonesPitchKindAsString (
      gMsrOptions->fMsrQuarterTonesPitchesLanguageKind,
      fHarmonyRootQuarterTonesPitchKind) <<
    endl <<
    setw (fieldWidth) <<
    "HarmonyKind" << " = " <<
    msrHarmonyKindAsString (fHarmonyKind) <<
    endl <<
    setw (fieldWidth) <<
    "HarmonyKindText" << " = \"" <<
    fHarmonyKindText <<
    "\"" <<
    endl <<
    setw (fieldWidth) <<
    "HarmonyBass" << " = " <<
    msrQuarterTonesPitchKindAsString (
      gMsrOptions->fMsrQuarterTonesPitchesLanguageKind,
      fHarmonyBassQuarterTonesPitchKind) <<
    endl;

  os <<
    "inversion: ";
  if (fHarmonyInversion == K_HARMONY_NO_INVERSION)
    os << "none";
  else
    os << fHarmonyInversion;
  os <<
    endl;

  // print harmony degrees if any
  if (fHarmonyDegreesList.size ()) {
    os <<
      "Harmony degrees:" <<
      endl;

    gIndenter++;
    
    list<S_msrHarmonyDegree>::const_iterator
      iBegin = fHarmonyDegreesList.begin (),
      iEnd   = fHarmonyDegreesList.end (),
      i      = iBegin;
      
    for ( ; ; ) {
      os <<
        (*i)->asString ();
      if (++i == iEnd) break;
      os << endl;
    } // for
 // JMI ???   os << endl;
    
    gIndenter--;
  }

  gIndenter--;
}

//______________________________________________________________________________
S_msrFigure msrFigure::create (
  int                 inputLineNumber,
  S_msrPart           figurePartUplink,
  msrFigurePrefixKind figurePrefixKind,
  int                 figureNumber,
  msrFigureSuffixKind figureSuffixKind)
{
  msrFigure* o =
    new msrFigure (
      inputLineNumber,
      figurePartUplink,
      figurePrefixKind,
      figureNumber,
      figureSuffixKind);
  assert(o!=0);

  return o;
}

msrFigure::msrFigure (
  int                 inputLineNumber,
  S_msrPart           figurePartUplink,
  msrFigurePrefixKind figurePrefixKind,
  int                 figureNumber,
  msrFigureSuffixKind figureSuffixKind)
    : msrElement (inputLineNumber)
{
  // sanity check
  msrAssert(
    figurePartUplink != nullptr,
    "figurePartUplink is null");
     
  // set figured's part uplink
  fFigurePartUplink =
    figurePartUplink;
 
  fFigurePrefixKind = figurePrefixKind;
  fFigureNumber     = figureNumber;
  fFigureSuffixKind = figureSuffixKind;
 
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceFiguredBass) {
    gLogIOstream <<
      "Creating figure '" <<
      asString () <<
      "'" <<
      endl;
  }
#endif
}

msrFigure::~msrFigure ()
{}

S_msrFigure msrFigure::createFigureNewbornClone (
  S_msrPart containingPart)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceFiguredBass) {
    gLogIOstream <<
      "Creating a newborn clone of figure '" <<
      asString () <<
      "'" <<
      endl;
  }
#endif

  // sanity check
  msrAssert(
    containingPart != nullptr,
    "containingPart is null");
    
  S_msrFigure
    newbornClone =
      msrFigure::create (
        fInputLineNumber,
        containingPart,
        fFigurePrefixKind,
        fFigureNumber,
        fFigureSuffixKind);
        
  return newbornClone;
}

S_msrFigure msrFigure::createFigureDeepCopy (
  S_msrPart containingPart)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceFiguredBass) {
    gLogIOstream <<
      "Creating a deep copy of figure '" <<
      asString () <<
      "'" <<
      endl;
  }
#endif

  // sanity check
  msrAssert(
    containingPart != nullptr,
    "containingPart is null");
    
  S_msrFigure
    figureDeepCopy =
      msrFigure::create (
        fInputLineNumber,
        containingPart,
        fFigurePrefixKind,
        fFigureNumber,
        fFigureSuffixKind);
        
  return figureDeepCopy;
}

string msrFigure::figurePrefixKindAsString (
  msrFigurePrefixKind figurePrefixKind)
{
  string result;
  
  switch (figurePrefixKind) {
    case msrFigure::k_NoFigurePrefix:
      result = "none";
      break;
    case msrFigure::kDoubleFlatPrefix:
      result = "double flat";
      break;
    case msrFigure::kFlatPrefix:
      result = "flat";
      break;
    case msrFigure::kFlatFlatPrefix:
      result = "flat flat";
      break;
    case msrFigure::kNaturalPrefix:
      result = "natural";
      break;
    case msrFigure::kSharpSharpPrefix:
      result = "sharp sharp";
      break;
    case msrFigure::kSharpPrefix:
      result = "sharp";
      break;
    case msrFigure::kDoubleSharpPrefix:
      result = "souble sharp";
      break;
  } // switch

  return result;
}

string msrFigure::figureSuffixKindAsString (
  msrFigureSuffixKind figureSuffixKind)
{
  string result;
  
  switch (figureSuffixKind) {
    case msrFigure::k_NoFigureSuffix:
      result = "none";
      break;
    case msrFigure::kDoubleFlatSuffix:
      result = "double flat";
      break;
    case msrFigure::kFlatSuffix:
      result = "flat";
      break;
    case msrFigure::kFlatFlatSuffix:
      result = "flat flat";
      break;
    case msrFigure::kNaturalSuffix:
      result = "natural";
      break;
    case msrFigure::kSharpSharpSuffix:
      result = "sharp sharp";
      break;
    case msrFigure::kSharpSuffix:
      result = "sharp";
      break;
    case msrFigure::kDoubleSharpSuffix:
      result = "souble sharp";
      break;
    case msrFigure::kSlashSuffix:
      result = "slash";
      break;
  } // switch

  return result;
}

string msrFigure::asString () const
{
  stringstream s;

  s <<
    "Figure" <<
      "'" << fFigureNumber <<
    "', prefix: " <<
    figurePrefixKindAsString (
      fFigurePrefixKind) <<
    ", suffix: " <<
    figureSuffixKindAsString (
      fFigureSuffixKind);

/* JMI
  if (fFigurePartUplink) // JMI ???
    s <<
      ":" <<
      wholeNotesAsMsrString (
        fInputLineNumber,
        fFigureSoundingWholeNotes);
*/

  return s.str ();
}

void msrFigure::acceptIn (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrFigure::acceptIn ()" <<
      endl;
  }
      
  if (visitor<S_msrFigure>*
    p =
      dynamic_cast<visitor<S_msrFigure>*> (v)) {
        S_msrFigure elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrFigure::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrFigure::acceptOut (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrFigure::acceptOut ()" <<
      endl;
  }

  if (visitor<S_msrFigure>*
    p =
      dynamic_cast<visitor<S_msrFigure>*> (v)) {
        S_msrFigure elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrFigure::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrFigure::browseData (basevisitor* v)
{}

ostream& operator<< (ostream& os, const S_msrFigure& elt)
{
  elt->print (os);
  return os;
}

void msrFigure::print (ostream& os)
{  
  os <<
    "Figure" <<
    " '" << fFigureNumber <<
    "', prefix: " <<
    figurePrefixKindAsString (
      fFigurePrefixKind) <<
    ", suffix: " <<
    figureSuffixKindAsString (
      fFigureSuffixKind) <<
     ", line " << fInputLineNumber <<
    endl;
}

//______________________________________________________________________________
S_msrFiguredBass msrFiguredBass::create (
  int       inputLineNumber,
  S_msrPart figuredBassPartUplink)
{
  msrFiguredBass* o =
    new msrFiguredBass (
      inputLineNumber,
      figuredBassPartUplink,
      rational (0, 1),    // figuredBassSoundingWholeNotes
      kFiguredBassParenthesesNo);
  assert(o!=0);

  return o;
}

S_msrFiguredBass msrFiguredBass::create (
  int       inputLineNumber,
  S_msrPart figuredBassPartUplink,
  rational  figuredBassSoundingWholeNotes,
    msrFiguredBassParenthesesKind
              figuredBassParenthesesKind)
{
  msrFiguredBass* o =
    new msrFiguredBass (
      inputLineNumber,
      figuredBassPartUplink,
      figuredBassSoundingWholeNotes,
      figuredBassParenthesesKind);
  assert(o!=0);

  return o;
}

msrFiguredBass::msrFiguredBass (
  int       inputLineNumber,
  S_msrPart figuredBassPartUplink,
  rational  figuredBassSoundingWholeNotes,
  msrFiguredBassParenthesesKind
            figuredBassParenthesesKind)
    : msrElement (inputLineNumber)
{
  // sanity check
  msrAssert(
    figuredBassPartUplink != nullptr,
    "figuredBassPartUplink is null");
     
  // set figuredBass's part uplink
  fFiguredBassPartUplink =
    figuredBassPartUplink;

  fFiguredBassSoundingWholeNotes =
    figuredBassSoundingWholeNotes;

  fFiguredBassParenthesesKind =
    figuredBassParenthesesKind;
    
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceFiguredBass) {
    gLogIOstream <<
      "Creating figuredBass '" <<
      asString () <<
      "'" <<
      endl;
  }
#endif
}

msrFiguredBass::~msrFiguredBass ()
{}

S_msrFiguredBass msrFiguredBass::createFiguredBassNewbornClone (
  S_msrPart containingPart)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceFiguredBass) {
    gLogIOstream <<
      "Creating a newborn clone of figuredBass '" <<
      asString () <<
      "'" <<
      endl;
  }
#endif

  // sanity check
  msrAssert(
    containingPart != nullptr,
    "containingPart is null");
    
  S_msrFiguredBass
    newbornClone =
      msrFiguredBass::create (
        fInputLineNumber,
        containingPart,
        fFiguredBassSoundingWholeNotes,
        fFiguredBassParenthesesKind);
        
  return newbornClone;
}

S_msrFiguredBass msrFiguredBass::createFiguredBassDeepCopy (
  S_msrPart containingPart)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceFiguredBass) {
    gLogIOstream <<
      "Creating a deep copy of figuredBass '" <<
      asString () <<
      "'" <<
      endl;
  }
#endif

  // sanity check
  msrAssert(
    containingPart != nullptr,
    "containingPart is null");
    
  S_msrFiguredBass
    figuredBassDeepCopy =
      msrFiguredBass::create (
        fInputLineNumber,
        containingPart,
        fFiguredBassSoundingWholeNotes,
        fFiguredBassParenthesesKind);
        
  return figuredBassDeepCopy;
}

void msrFiguredBass::appendFiguredFigureToFiguredBass (
  S_msrFigure figure)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceFiguredBass) {
    gLogIOstream <<
      "Appending figure'" << figure->asString () <<
      "' to figuredBass '" <<
      asString () <<
      "'" <<
      endl;
  }
#endif

  fFiguredBassFiguresList.push_back (figure);
}

string msrFiguredBass::figuredBassParenthesesKindAsString (
  msrFiguredBassParenthesesKind figuredBassParenthesesKind)
{
  string result;
  
  switch (figuredBassParenthesesKind) {
    case msrFiguredBass::kFiguredBassParenthesesYes:
      result = "figuredBassParenthesesYes";
      break;
    case msrFiguredBass::kFiguredBassParenthesesNo:
      result = "figuredBassParenthesesNo";
      break;
  } // switch

  return result;
}

string msrFiguredBass::asString () const
{
  stringstream s;

  s <<
    "Figured bass" <<
    ": " <<
    wholeNotesAsMsrString (
      fInputLineNumber,
      fFiguredBassSoundingWholeNotes) <<
    " sounding whole notes" <<
    ", " <<
    figuredBassParenthesesKindAsString (
      fFiguredBassParenthesesKind) <<
    ", line " << fInputLineNumber;

  if (fFiguredBassFiguresList.size ()) {
    s << ", ";

    list<S_msrFigure>::const_iterator
      iBegin = fFiguredBassFiguresList.begin (),
      iEnd   = fFiguredBassFiguresList.end (),
      i      = iBegin;
      
    for ( ; ; ) {
      s << (*i);
      if (++i == iEnd) break;
      s << " ";
    } // for
  }

/* JMI
  if (fFiguredBassPartUplink) // JMI ???
    s <<
      ":" <<
      wholeNotesAsMsrString (
        fInputLineNumber,
        fFiguredBassSoundingWholeNotes);
*/

  return s.str ();
}

void msrFiguredBass::acceptIn (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrFiguredBass::acceptIn ()" <<
      endl;
  }
      
  if (visitor<S_msrFiguredBass>*
    p =
      dynamic_cast<visitor<S_msrFiguredBass>*> (v)) {
        S_msrFiguredBass elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrFiguredBass::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrFiguredBass::acceptOut (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrFiguredBass::acceptOut ()" <<
      endl;
  }

  if (visitor<S_msrFiguredBass>*
    p =
      dynamic_cast<visitor<S_msrFiguredBass>*> (v)) {
        S_msrFiguredBass elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrFiguredBass::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrFiguredBass::browseData (basevisitor* v)
{
  for (
    list<S_msrFigure>::const_iterator i = fFiguredBassFiguresList.begin ();
    i != fFiguredBassFiguresList.end ();
    i++) {
    // browse the figure
    msrBrowser<msrFigure> browser (v);
    browser.browse (*(*i));
  } // for
}

ostream& operator<< (ostream& os, const S_msrFiguredBass& elt)
{
  elt->print (os);
  return os;
}

void msrFiguredBass::print (ostream& os)
{  
  os <<
    "FiguredBass" <<
    ": " <<
    wholeNotesAsMsrString (
      fInputLineNumber,
      fFiguredBassSoundingWholeNotes) <<
    " sounding whole notes" <<
    ", " <<
    figuredBassParenthesesKindAsString (
      fFiguredBassParenthesesKind) <<
      ", line " << fInputLineNumber <<
    endl;

  if (fFiguredBassFiguresList.size ()) {
    gIndenter++;

    list<S_msrFigure>::const_iterator
      iBegin = fFiguredBassFiguresList.begin (),
      iEnd   = fFiguredBassFiguresList.end (),
      i      = iBegin;
      
    for ( ; ; ) {
      os << (*i);
      if (++i == iEnd) break;
 // JMI     os << endl;
    } // for

    gIndenter--;
  }
}

//______________________________________________________________________________
S_msrMeasure msrMeasure::create (
  int           inputLineNumber,
  string        measureNumber,
  S_msrSegment  measureSegmentUplink)
{
  msrMeasure* o =
    new msrMeasure (
      inputLineNumber,
      measureNumber,
      measureSegmentUplink);
  assert(o!=0);

  return o;
}

msrMeasure::msrMeasure (
  int           inputLineNumber,
  string        measureNumber,
  S_msrSegment  measureSegmentUplink)
    : msrElement (inputLineNumber)
{
  // sanity check
  msrAssert(
    measureSegmentUplink != nullptr,
    "measureSegmentUplink is null");

  // set measure's segment uplink
  fMeasureSegmentUplink =
    measureSegmentUplink;

  // set measure numbers
  fMeasureNumber = measureNumber;
  fMeasureOrdinalNumber = -1;
  fNextMeasureNumber = "";
  
  // do other initializations
  initializeMeasure ();
}

void msrMeasure::initializeMeasure ()
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceMeasures) {
    gLogIOstream <<
      "Initializing measure '" << fMeasureNumber <<
      "' in segment '" <<
      fMeasureSegmentUplink->getSegmentAbsoluteNumber () <<
      "' in voice \"" <<
      fMeasureSegmentUplink->
        getSegmentVoiceUplink ()->
          getVoiceName () <<
      "\"" <<
      ", line " << fInputLineNumber <<
      endl;
  }
#endif

  static int counter = 0;
  
  counter++;
      
  if (
    false &&
    fMeasureNumber == "11"
      &&
    fMeasureSegmentUplink->getSegmentAbsoluteNumber () == 4
      &&
    counter == 1
    ) {
    gLogIOstream <<
      endl <<
      "======================= initializeMeasure()" <<
      endl <<

      fMeasureSegmentUplink->
        getSegmentVoiceUplink () <<
      
      "=======================" <<
      endl <<
      endl;

    abort ();
  }
  
  // measure kind
  fMeasureKind = kUnknownMeasureKind;

  // measure 'first in segment' kind
  fMeasureFirstInSegmentKind = kMeasureFirstInSegmentUnknown;

  // measure 'created for a repeat' kind
  fMeasureCreatedForARepeatKind = kMeasureCreatedForARepeatNo;

  // single-measure rest?
  fMeasureIsASingleMeasureRest = false;
  
  // initialize measure position
  setMeasureLength (
    fInputLineNumber,
    rational (0, 1)); // ready to receive the first note


  // fetch the staff
  S_msrStaff
    staff =
      fMeasureSegmentUplink->
        getSegmentVoiceUplink ()->
          getVoiceStaffUplink ();

  // get the staff time
  S_msrTime
    time =
      staff->
        getStaffCurrentTime ();
        
  // set the measure full length if relevant
  if (time) {
    setMeasureFullLengthFromTime (
      time);
    }

  // measure doesn't contain music yet
  fMeasureContainsMusic = false;
}

msrMeasure::~msrMeasure ()
{}

S_msrPart msrMeasure::fetchMeasurePartUplink () const
{
  return
    fMeasureSegmentUplink->
      fetchSegmentPartUplink ();
}

S_msrVoice msrMeasure::fetchMeasureVoiceUplink () const
{
  return
    fMeasureSegmentUplink->
      getSegmentVoiceUplink ();
}

S_msrMeasure msrMeasure::createMeasureNewbornClone (
  S_msrSegment containingSegment)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceMeasures) {
    gLogIOstream <<
      "Creating a newborn clone of measure '" <<
      fMeasureNumber <<
      "'" <<
      " in segment " <<
      containingSegment->asString () <<
      " in voice \"" <<
        containingSegment->
          getSegmentVoiceUplink ()->
            getVoiceName () <<
      "\"" <<
      endl;
  }
#endif
  
  // sanity check
  msrAssert(
    containingSegment != nullptr,
    "containingSegment is null");

  // create newborn clone
  S_msrMeasure
    newbornClone =
      msrMeasure::create (
        fInputLineNumber,
        fMeasureNumber,
        containingSegment);

  // lengthes
  newbornClone->fMeasureFullLength =
    fMeasureFullLength;
    
  // don't take fMeasureLength over,
  // it will be computed on the fly
  // while appending notes to the measure newborn clone

  // measure kind
  newbornClone->fMeasureKind =
    fMeasureKind;

  // next measure number
  newbornClone->fNextMeasureNumber =
    fNextMeasureNumber;

  // measure 'first in segment' kind
  newbornClone->fMeasureFirstInSegmentKind =
    fMeasureFirstInSegmentKind;

  // measure 'created after a repeat' kind
  newbornClone->fMeasureCreatedForARepeatKind =
    fMeasureCreatedForARepeatKind;

  // single-measure rest?
  newbornClone->fMeasureIsASingleMeasureRest =
    fMeasureIsASingleMeasureRest;
  
  // chords handling

  // elements

  // uplinks

  return newbornClone;
}

S_msrMeasure msrMeasure::createMeasureDeepCopy (
  S_msrSegment containingSegment)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceMeasures) {
    gLogIOstream <<
      "Creating a deep copy of measure '" <<
      fMeasureNumber <<
      "'" <<
      " in segment " <<
      containingSegment->asString () <<
      " in voice \"" <<
        containingSegment->
          getSegmentVoiceUplink ()->
            getVoiceName () <<
      "\"" <<
      endl;
  }
#endif
  
  // sanity check
  msrAssert(
    containingSegment != nullptr,
    "containingSegment is null");

  // create deep copy
  S_msrMeasure
    measureDeepCopy =
      msrMeasure::create (
        fInputLineNumber,
        fMeasureNumber,
        containingSegment);

  // lengthes
  measureDeepCopy->fMeasureFullLength =
    fMeasureFullLength;
    
  measureDeepCopy->fMeasureLength =
    fMeasureLength;
    
  // measure kind
  measureDeepCopy->fMeasureKind =
    fMeasureKind;

  // next measure number
  measureDeepCopy->fNextMeasureNumber =
    fNextMeasureNumber;

  // measure 'first in segment' kind
  measureDeepCopy->fMeasureFirstInSegmentKind =
    fMeasureFirstInSegmentKind;

  // measure 'created after a repeat' kind
  measureDeepCopy->fMeasureCreatedForARepeatKind =
    fMeasureCreatedForARepeatKind;

  // elements

  int numberOfMeasureElements =
    fMeasureElementsList.size ();

  if (numberOfMeasureElements) {
#ifdef TRACE_OPTIONS
    if (gTraceOptions->fTraceMeasures) {
      gLogIOstream <<
        singularOrPluralWithoutNumber (
          numberOfMeasureElements, "There is", "There are") <<
        " " <<
        singularOrPlural (
          numberOfMeasureElements, "element", "elements") <<
        " to be deep copied" <<
        " in measure " <<
        fMeasureNumber <<
        " in segment " <<
        containingSegment->asString () <<
        " in voice \"" <<
          containingSegment->
            getSegmentVoiceUplink ()->
              getVoiceName () <<
        "\"" <<
        endl;
    }
#endif
    
    for (
      list<S_msrElement>::const_iterator i = fMeasureElementsList.begin ();
      i != fMeasureElementsList.end ();
      i++ ) {
      S_msrElement element = (*i);
      
      // handlle deep copying
      S_msrElement
        elementDeepCopy;
        
      if (
        S_msrNote note = dynamic_cast<msrNote*>(&(*element))
        ) {    
        // create the note deep copy
        elementDeepCopy =
          note->createNoteDeepCopy (
            fetchMeasureVoiceUplink ());

/* JMI
        // append the element deep copy to the measure deep copy
        measureDeepCopy->
          fMeasureElementsList.push_back (
            elementDeepCopy);
*/
      }
    
      else if (
        S_msrTime time = dynamic_cast<msrTime*>(&(*element))
        ) {
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceTimes || gTraceOptions->fTraceMeasures) {
          gLogIOstream <<
            "Sharing time '" <<
            time->asShortString () <<
            "' in measure '" <<
            fMeasureNumber <<
            "'deep copy" <<
            ", line " << fInputLineNumber <<
            endl;
        }
#endif
          
        // share the element with the original measure
        elementDeepCopy = time;
      }
    
      else {
        // share the element with the original measure
        elementDeepCopy = element;
      }

      // append the element deep copy to the measure deep copy
      measureDeepCopy->
        fMeasureElementsList.push_back (
          elementDeepCopy);
    } // for
  }
  
  else {
#ifdef TRACE_OPTIONS
    if (gTraceOptions->fTraceMeasures) {
      gLogIOstream <<
        "There are no elements in measure to be deep copied" <<
        " in segment " <<
        containingSegment->asString () <<
        " in voice \"" <<
          containingSegment->
            getSegmentVoiceUplink ()->
              getVoiceName () <<
        "\"" <<
        endl;
    }
#endif
  }

  // uplinks

  // fMeasureSegmentUplink JMI ???
  
  return measureDeepCopy;
}

void msrMeasure::setMeasureCreatedForARepeatKind (
  msrMeasureCreatedForARepeatKind
    measureCreatedForARepeatKind)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceMeasures) {
    gLogIOstream <<
      "Setting measureCreatedForARepeatKind in measure '" <<
      fMeasureNumber <<
      "' in segment '" <<
      fMeasureSegmentUplink-> asString () <<
      /* JMI
      "' in voice \"" <<
      fSegmentVoiceUplink->getVoiceName () <<
      "\"" <<
      */
      "' to '" <<
      msrMeasure::measureCreatedForARepeatKindAsString (
        measureCreatedForARepeatKind) << 
      "', line " << fInputLineNumber <<
      endl;
  }
#endif

  fMeasureCreatedForARepeatKind =
    measureCreatedForARepeatKind;
}

void msrMeasure::setNextMeasureNumber (string nextMeasureNumber)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceMeasures) {
    gLogIOstream <<
      "Setting next measure number to '" << nextMeasureNumber <<
      "' in measure '" <<
      fMeasureNumber <<
      "' in segment '" <<
      fMeasureSegmentUplink-> asString () <<
      /* JMI
      "' in voice \"" <<
      fSegmentVoiceUplink->getVoiceName () <<
      "\"" <<
      */
      "', line " << fInputLineNumber <<
      endl;
  }
#endif

  fNextMeasureNumber = nextMeasureNumber;
}

string msrMeasure::measureFullLengthAsMSRString ()
{
  return
    wholeNotesAsMsrString (
      fInputLineNumber,
      fMeasureFullLength);
}

void msrMeasure::setMeasureLength (
  int      inputLineNumber,
  rational measureLength)
{
  // rationalise the measure length
  rational rationalisedMeasureLength = measureLength;
  rationalisedMeasureLength.rationalise ();
  
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceMeasures || gTraceOptions->fTraceDivisions) {
    gLogIOstream <<
      "Setting measure '" << fMeasureNumber <<
      "' measure length to '"  << rationalisedMeasureLength << "'";

    if (
      rationalisedMeasureLength.getDenominator ()
        !=
      measureLength.getDenominator ()) {
      gLogIOstream <<
        " (was '" << measureLength << "')";
    }

    gLogIOstream <<
      " in voice \"" <<
      fMeasureSegmentUplink->
        getSegmentVoiceUplink ()->
          getVoiceName () <<
      "\"" <<
      "', line " << inputLineNumber <<
      endl;
  }
#endif

  // set measure length
  fMeasureLength = rationalisedMeasureLength;
}

string msrMeasure::measureLengthAsMSRString ()
{
  return
    wholeNotesAsMsrString (
      fInputLineNumber,
      fMeasureLength);
}

void msrMeasure::appendClefToMeasure (S_msrClef clef)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceClefs || gTraceOptions->fTraceMeasures) {
    gLogIOstream <<
      "Appending clef '" << clef->asString () <<
      "' to measure " << fMeasureNumber <<
      ", in voice \"" <<
      fetchMeasureVoiceUplink ()->getVoiceName () <<
      "\"" <<
      endl;
  }
#endif
          
  // append it to the measure elements list
  fMeasureElementsList.push_back (clef);
}

void msrMeasure::appendKeyToMeasure (S_msrKey key)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceKeys || gTraceOptions->fTraceMeasures) {
    gLogIOstream <<
      "Appending key '" << key->asString () <<
      "' to measure " << fMeasureNumber <<
      ", in voice \"" <<
      fetchMeasureVoiceUplink ()->getVoiceName () <<
      "\"" <<
      endl;
  }
#endif
          
  // append it to the measure elements list
  fMeasureElementsList.push_back (key);
}

void msrMeasure::appendTimeToMeasure (S_msrTime time)
{
  // sanity check
  msrAssert(
    time != nullptr,
    "time is null");

#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceTimes || gTraceOptions->fTraceMeasures) {
    gLogIOstream <<
      "Appending time '" <<
      endl;

    gIndenter++;

    gLogIOstream <<
      time;

    gIndenter--;

    gLogIOstream <<
      "' to measure '" << fMeasureNumber <<
      "' in voice \"" <<
      fMeasureSegmentUplink->
        getSegmentVoiceUplink ()->
          getVoiceName () <<
      "\"" <<
      endl;
  }
#endif

  gIndenter++;
  
  // append time to the measure elements list
  fMeasureElementsList.push_back (time);

/*
  // set the measure kind, in case there is a left-over from the previous measure???
  switch (timeSymbolKind) {
    case msrTime::kTimeSymbolCommon:
      result = "timeSymbolCommon";
      break;
    case msrTime::kTimeSymbolCut:
      result = "timeSymbolCut";
      break;
    case msrTime::kTimeSymbolNote:
      result = "timeSymbolNote";
      break;
    case msrTime::kTimeSymbolDottedNote:
      result = "timeSymbolDottedNote";
      break;
    case msrTime::kTimeSymbolSingleNumber:
      result = "timeSymbolSingleNumber";
      break;
    case msrTime::kTimeSymbolSenzaMisura:
      result = "timeSymbolSenzaMisura";
      break;
    case msrTime::kTimeSymbolNone:
      result = "timeSymbolNone";
      break;
  } // switch
*/
  
  // set the measure whole notes per full measure
  setMeasureFullLengthFromTime (
    time);
    
/* JMI
  if (time->getTimeSymbolKind () == msrTime::kTimeSymbolSenzaMisura) {
    
    // this measure is senza misura
    
  #ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceMeasures) {
      gLogIOstream <<
        "Measure '" << fMeasureNumber <<
        "' in voice \"" <<
        fMeasureSegmentUplink->
          getSegmentVoiceUplink ()->
            getVoiceName () <<
        "\"" <<
        " is senza misura" <<
        endl;
    }
#endif

    fMeasureKind = kSenzaMisuraMeasureKind;
    
    fMeasureFullLength = INT_MAX; // JMI
  }
  
  else {
    
    // this measure is con misura
    
    int partDivisionsPerQuarterNote =
      fetchMeasurePartUplink->
        getPartDivisionsPerQuarterNote ();
  
    rational
      wholeNotesPerMeasure =
        time->wholeNotesPerMeasure ();
        
  #ifdef TRACE_OPTIONS
    if (gTraceOptions->fTraceTimes) {
      gLogIOstream <<
        time;
  
      gLogIOstream <<
        "has " <<
        wholeNotesPerMeasure.getNumerator () <<
        "/" <<
        wholeNotesPerMeasure.getDenominator () <<
        " whole note(s) per measure" <<
        ", line " << fInputLineNumber <<
        endl;
    }
#endif
    
    fMeasureFullLength =
      wholeNotesPerMeasure.getNumerator ()
        *
      partDivisionsPerQuarterNote * 4 // hence a whole note
        /
      wholeNotesPerMeasure.getDenominator ();
    
  
  #ifdef TRACE_OPTIONS
    if (gTraceOptions->fTraceMeasures) {
      gLogIOstream <<
        "Measure '" << fMeasureNumber <<
        "' in voice \"" <<
        fMeasureSegmentUplink->
          getSegmentVoiceUplink ()->
            getVoiceName () <<
        "\"" <<
        " has " <<
        singularOrPlural (
          fMeasureFullLength,
          "wholeNotes per full measure",
          "division per full measure") <<
        endl;
  }
#endif
  */
  
  gIndenter--;
}

void msrMeasure::appendTimeToMeasureClone (S_msrTime time)
{
  // sanity check
  msrAssert(
    time != nullptr,
    "time is null");

#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceTimes || gTraceOptions->fTraceMeasures) {
    gLogIOstream <<
      "Appending time:" <<
      endl;

    gIndenter++;

    gLogIOstream <<
      time;

    gIndenter--;

    gLogIOstream <<
      "to measure clone '" << fMeasureNumber <<
      "' in voice \"" <<
      fMeasureSegmentUplink->
        getSegmentVoiceUplink ()->
          getVoiceName () <<
      "\"" <<
      endl;
  }
#endif
    
  // append time to the measure elements list
  fMeasureElementsList.push_back (time);
}

void msrMeasure::setMeasureFullLengthFromTime (
  S_msrTime time)
{
  // sanity check
  msrAssert(
    time != nullptr,
    "time is null");

#ifdef TRACE_OPTIONS
  if (
    gTraceOptions->fTraceDivisions
      ||
    gTraceOptions->fTraceTimes
      ||
    gTraceOptions->fTraceMeasures) {
    gLogIOstream <<
      "Setting measure full measure length from time:" <<
      endl;

    gIndenter++;

    gLogIOstream <<
      time;

    gIndenter--;

    gLogIOstream <<
      "for measure '" << fMeasureNumber <<
      "' in voice \"" <<
      fMeasureSegmentUplink->
        getSegmentVoiceUplink ()->
          getVoiceName () <<
      "\"" <<
      ", line " << fInputLineNumber <<
      endl;
  }
#endif

  switch (time->getTimeSymbolKind ()) {
    case msrTime::kTimeSymbolCommon:
    case msrTime::kTimeSymbolCut:
    case msrTime::kTimeSymbolNote:
    case msrTime::kTimeSymbolDottedNote:
    case msrTime::kTimeSymbolSingleNumber:
    case msrTime::kTimeSymbolNone:
      {
        // this measure is con misura
        
        rational
          wholeNotesPerMeasure =
            time->
              wholeNotesPerMeasure ();
            
#ifdef TRACE_OPTIONS
        if (
          gTraceOptions->fTraceDivisions
            ||
          gTraceOptions->fTraceTimes
            ||
          gTraceOptions->fTraceMeasures) {
          gLogIOstream <<
            "There are " <<
            wholeNotesPerMeasure <<
            " whole note(s) per measure in time:" <<
            endl;
    
          gIndenter++;
          
          gLogIOstream <<
            time;
                      
          gIndenter--;
    
          gLogIOstream <<
            "in measure '" << fMeasureNumber << "'" <<
            ", line " << fInputLineNumber <<
            "' in voice \"" <<
            fMeasureSegmentUplink->
              getSegmentVoiceUplink ()->
                getVoiceName () <<
            "\"" <<
            endl;
        }
#endif
    
        // set full measure length
        fMeasureFullLength =
          wholeNotesPerMeasure;
    
#ifdef TRACE_OPTIONS
        if (
          gTraceOptions->fTraceDivisions
            ||
          gTraceOptions->fTraceTimes
            ||
          gTraceOptions->fTraceMeasures) {
          gLogIOstream <<
            "Measure '" << fMeasureNumber <<
            "' in voice \"" <<
            fMeasureSegmentUplink->
              getSegmentVoiceUplink ()->
                getVoiceName () <<
            "\"" <<
            " has full measure length " <<
            fMeasureFullLength <<
            " whole notes" <<
            endl;
        }
#endif
      }
      break;
          
    case msrTime::kTimeSymbolSenzaMisura:
        
      // this measure is senza misura
      
#ifdef TRACE_OPTIONS
      if (
        gTraceOptions->fTraceDivisions
          ||
        gTraceOptions->fTraceTimes
          ||
        gTraceOptions->fTraceMeasures) {
        gLogIOstream <<
          "Measure '" << fMeasureNumber <<
          "' in voice \"" <<
          fMeasureSegmentUplink->
            getSegmentVoiceUplink ()->
              getVoiceName () <<
          "\"" <<
          " is senza misura" <<
          endl;
      }
#endif
  
      fMeasureKind = kSenzaMisuraMeasureKind;
      
      fMeasureFullLength =
        rational (INT_MAX, 1);
      break;
  } // switch
}

void msrMeasure::appendTransposeToMeasure (
  S_msrTranspose transpose)
{    
  // append it to the measure elements list
  fMeasureElementsList.push_back (transpose);
}

void msrMeasure::appendPartNameDisplayToMeasure (
  S_msrPartNameDisplay partNameDisplay)
{    
  // append it to the measure elements list
  fMeasureElementsList.push_back (partNameDisplay);
}

void msrMeasure::appendPartAbbreviationDisplayToMeasure (
  S_msrPartAbbreviationDisplay partAbbreviationDisplay)
{    
  // append it to the measure elements list
  fMeasureElementsList.push_back (partAbbreviationDisplay);
}

void msrMeasure::appendBarlineToMeasure (S_msrBarline barline)
{
  /* JMI NO, a barline may appear anywhere}
  // the measure may have to be padded
  padUpToPartMeasureLengthHighTide (
    barline->getInputLineNumber ());
    */
    
  // append it to the measure elements list
  fMeasureElementsList.push_back (barline);
}

void msrMeasure::prependBarlineToMeasure (S_msrBarline barline)
{
  // append it to the measure elements list
  fMeasureElementsList.push_front (barline);
}

void msrMeasure::appendSegnoToMeasure (S_msrSegno segno)
{    
  // append it to the measure elements list
  fMeasureElementsList.push_back (segno);
}

void msrMeasure::appendCodaToMeasure (S_msrCoda coda)
{    
  // append it to the measure elements list
  fMeasureElementsList.push_back (coda);
}

void msrMeasure::appendEyeGlassesToMeasure (
  S_msrEyeGlasses eyeGlasses)
{    
  // append it to the measure elements list
  fMeasureElementsList.push_back (eyeGlasses);
}

void msrMeasure::appendPedalToMeasure (S_msrPedal pedal)
{    
  // append it to the measure elements list
  fMeasureElementsList.push_back (pedal);
}

void msrMeasure::appendDampToMeasure (S_msrDamp damp)
{    
  // append it to the measure elements list
  fMeasureElementsList.push_back (damp);
}

void msrMeasure::appendDampAllToMeasure (S_msrDampAll dampAll)
{    
  // append it to the measure elements list
  fMeasureElementsList.push_back (dampAll);
}

void msrMeasure::appendBarCheckToMeasure (S_msrBarCheck barCheck)
{    
  // append it to the measure elements list
  fMeasureElementsList.push_back (barCheck);
}

void msrMeasure::appendVoiceStaffChangeToMeasure (
  S_msrVoiceStaffChange voiceStaffChange)
{    
  // append it to the measure elements list
  fMeasureElementsList.push_back (voiceStaffChange);
}

void msrMeasure::appendNoteToMeasure (S_msrNote note)
{
  int inputLineNumber =
    note->getInputLineNumber ();

#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceNotes || gTraceOptions->fTraceMeasures) {
    gLogIOstream <<
      "Appending note '" << note->asShortString () <<
      "' to measure '" << fMeasureNumber <<
      "' in voice \"" <<
      fMeasureSegmentUplink->
        getSegmentVoiceUplink ()->
          getVoiceName () <<
      "\"" <<
      endl;
  }
#endif

  gIndenter++;
  
  // populate measure uplink
  note->
    setNoteMeasureUplink (this);

  // register note measure number
  note->
    setNoteMeasureNumber (fMeasureNumber);
  
  // register note measure position in measure
  rational
    noteMeasurePosition =
      fMeasureLength; // for harmony voice
  
  note->
    setNotePositionInMeasure (
      noteMeasurePosition);
  
  // fetch note sounding whole notes
  rational noteSoundingWholeNotes =
    note->getNoteSoundingWholeNotes ();
    
  string noteSoundingWholeNotesAsMsrString =
    note->noteSoundingWholeNotesAsMsrString ();
    
  // account for note duration in measure length
  setMeasureLength (
    inputLineNumber,
    fMeasureLength + noteSoundingWholeNotes);

  // update part measure length high tide if need be
  fetchMeasurePartUplink ()->
    updatePartMeasureLengthHighTide (
      inputLineNumber,
      fMeasureLength);

/* JMI
  // determine whether the note occupies a full measure
  if (noteSoundingWholeNotes == fMeasureFullLength)
    note->
      setNoteOccupiesAFullMeasure ();
  */
  
  // append the note to the measure elements list
// JMI  // only now to make it possible to remove it afterwards
  // if it happens to be the first note of a chord
  fMeasureElementsList.push_back (note);
  
  // register note as the last one in this measure
  fMeasureLastHandledNote = note;

  // is this note the longest one in this measure?
  if (! fMeasureLongestNote) {
    fMeasureLongestNote = note;
  }
  else {
    if (
      note->getNoteSoundingWholeNotes ()
        >
      fMeasureLongestNote->getNoteSoundingWholeNotes ()
    ) {
      fMeasureLongestNote = note;
    }
  
    if (
      note->getNoteDisplayWholeNotes ()
        >
      fMeasureLongestNote->getNoteSoundingWholeNotes ()
    ) {
      fMeasureLongestNote = note;
    }
  }

  // this measure contains music
  fMeasureContainsMusic = true;

  gIndenter--;
}

void msrMeasure::appendNoteToMeasureClone (S_msrNote note)
{
  int inputLineNumber =
    note->getInputLineNumber ();

#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceNotes || gTraceOptions->fTraceMeasures) {
    gLogIOstream <<
      "Appending note '" << note->asShortString () <<
      "' to measure clone '" << fMeasureNumber <<
      "' in voice clone \"" <<
      fMeasureSegmentUplink->
        getSegmentVoiceUplink ()->
          getVoiceName () <<
      "\"" <<
      endl;
  }
#endif

  gIndenter++;
  
    /* JMI
  if (appendMeasureIfOverflow (inputLineNumber)) {
    // a new measure has been appended to the segment
    // append note to it via the segment
    fMeasureSegmentUplink->
      appendNoteToSegment (note);
  }

  else {
  */
    // regular insertion in current measure
    
    // populate measure uplink
    note->setNoteMeasureUplink (
      this);

    // register note measure number
    note->setNoteMeasureNumber (
      fMeasureNumber);
    
    // register note measure position
    rational
      noteMeasurePosition =
        fMeasureLength; // for harmony voice
    
    note->
      setNotePositionInMeasure (
        noteMeasurePosition);
    
    // fetch note sounding whole notes
    rational
      noteSoundingWholeNotes =
        note->getNoteSoundingWholeNotes ();

    // account for note duration in measure length
    setMeasureLength (
      inputLineNumber,
      fMeasureLength + noteSoundingWholeNotes);
  
    // update part measure length high tide if need be
    fetchMeasurePartUplink ()->
      updatePartMeasureLengthHighTide (
        inputLineNumber,
        fMeasureLength);

  /* JMI
    // determine whether the note occupies a full measure
    if (noteSoundingWholeNotes == fMeasureFullLength)
      note->
        setNoteOccupiesAFullMeasure ();
      */
      
    // append the note to the measure elements list
  // JMI  // only now to make it possible to remove it afterwards
    // if it happens to be the first note of a chord
    fMeasureElementsList.push_back (note);

    // register note as the last one in this measure
    fMeasureLastHandledNote = note;

  // this measure contains music
  fMeasureContainsMusic = true;
 // JMI }

  gIndenter--;
}

void msrMeasure::appendDoubleTremoloToMeasure (
  S_msrDoubleTremolo doubleTremolo)
{
  int inputLineNumber =
    doubleTremolo->getInputLineNumber ();
  
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceTremolos || gTraceOptions->fTraceMeasures) {
    gLogIOstream <<
      "Appending double tremolo '" <<
      doubleTremolo->asShortString () <<
      "' to measure '" << fMeasureNumber <<
      "' in voice \"" <<
      fMeasureSegmentUplink->
        getSegmentVoiceUplink ()->
          getVoiceName () <<
      "\"" <<
      endl;
  }
#endif

  gIndenter++;
  
  // register doubleTremolo measure number
  doubleTremolo->
    setDoubleTremoloMeasureNumber (
      fMeasureNumber);
  
  // register doubleTremolo measure position in measure
  doubleTremolo->
    setDoubleTremoloPositionInMeasure (
      fMeasureLength);

  // copy measure number to first note, that was created beforehand
  doubleTremolo->
    setDoubleTremoloMeasureNumber (
      fMeasureNumber);
  
  // copy measure position to first note, that was created beforehand
  doubleTremolo->
    setDoubleTremoloPositionInMeasure (
      fMeasureLength);

  // fetch doubleTremolo sounding whole notes
  rational
    doubleTremoloSoundingWholeNotes =
      doubleTremolo->
        getDoubleTremoloSoundingWholeNotes ();
    
  // account for doubleTremolo duration in measure length
  setMeasureLength (
    inputLineNumber,
    fMeasureLength + doubleTremoloSoundingWholeNotes);

  // update part measure length high tide if need be
  fetchMeasurePartUplink ()->
    updatePartMeasureLengthHighTide (
      inputLineNumber,
      fMeasureLength);

  // determine if the doubleTremolo occupies a full measure
// XXL  JMI  if (doubleTremoloSoundingWholeNotes == fMeasureDivisionsPerWholeMeasure)
    // doubleTremolo->setDoubleTremoloOccupiesAFullMeasure ();

  // append the doubleTremolo to the measure elements list
  fMeasureElementsList.push_back (doubleTremolo);

  // this measure contains music
  fMeasureContainsMusic = true;

/* JMI
  // bring harmony voice to the same measure length
  fetchMeasurePartUplink->
    getPartHarmonyVoice ()->
      padUpToMeasureLengthInVoice (
        inputLineNumber,
        fMeasureLength);
        */

  gIndenter--;
}

void msrMeasure::appendMeasuresRepeatToMeasure (
  S_msrMeasuresRepeat measuresRepeat)
{
  /*
  int inputLineNumber =
    measuresRepeat->getInputLineNumber ();
    */
    
/* JMI  
  // populate measure uplink
  measuresRepeat->setMeasuresRepeatMeasureUplink (this);
*/

#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceRepeats || gTraceOptions->fTraceMeasures) {
    gLogIOstream <<
      "Appending multiple rest '" <<
      measuresRepeat->asString () <<
      "' to measure '" << fMeasureNumber <<
      "' in voice \"" <<
      fMeasureSegmentUplink->
        getSegmentVoiceUplink ()->
          getVoiceName () <<
      "\"" <<
      endl;
  }
#endif
  
/* JMI
  // register measuresRepeat measure number
  measuresRepeat->
    setmeasuresRepeatMeasureNumber (fMeasureNumber);
  
  // register measuresRepeat measure position
  measuresRepeat->
    setmeasuresRepeatPositionInMeasure (
      fMeasureLength);

  // copy measure number to first note, that was created beforehand
  measuresRepeat->
    setmeasuresRepeatMeasureNumber (
      fMeasureNumber);
  
  // copy measure position to first note, that was created beforehand
  measuresRepeat->
    setmeasuresRepeatPositionInMeasure (
      fMeasureLength);

  // fetch measuresRepeat sounding whole notes
  int measuresRepeatSoundingWholeNotes =
    measuresRepeat->getmeasuresRepeatSoundingWholeNotes ();
    
  // account for measuresRepeat duration in measure length
  setMeasureLength (
    inputLineNumber,
    fMeasureLength + measuresRepeatSoundingWholeNotes);

  // update part measure length high tide if need be
  fMeasureDirectPartUplink->
    updatePartMeasureLengthHighTide (
      inputLineNumber,
      fMeasureLength);

  // determine if the measuresRepeat occupies a full measure
// XXL  JMI  if (measuresRepeatSoundingWholeNotes == fMeasureDivisionsPerWholeMeasure)
    // measuresRepeat->setmeasuresRepeatOccupiesAFullMeasure ();
*/

  // append the measuresRepeat to the measure elements list
  fMeasureElementsList.push_back (measuresRepeat);

  // this measure contains music
  fMeasureContainsMusic = true;
//*/
}

void msrMeasure::appendMultipleRestToMeasure (
  S_msrMultipleRest multipleRest)
{
  /* JMI
  int inputLineNumber =
    multipleRest->getInputLineNumber ();
    */
    
  /* JMI  
    // populate measure uplink
    multipleRest->setMultipleRestMeasureUplink (this);
*/

#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceRepeats || gTraceOptions->fTraceMeasures) {
    gLogIOstream <<
      "Appending multiple rest '" <<
      multipleRest->asString () <<
      "' to measure '" << fMeasureNumber <<
      "' in voice \"" <<
      fMeasureSegmentUplink->
        getSegmentVoiceUplink ()->
          getVoiceName () <<
      "\"" <<
      endl;
  }
#endif
  
/* JMI
  // register multipleRest measure number
  multipleRest->
    setMultipleRestMeasureNumber (fMeasureNumber);
  
  // register multipleRest measure position in measure
  multipleRest->
    setMultipleRestPositionInMeasure (
      fMeasureLength);

  // copy measure number to first note, that was created beforehand
  multipleRest->
    setMultipleRestMeasureNumber (
      fMeasureNumber);
  
  // copy measure position in measure to first note, that was created beforehand
  multipleRest->
    setMultipleRestPositionInMeasure (
      fMeasureLength);

  // fetch multipleRest sounding whole notes
  int multipleRestSoundingWholeNotes =
    multipleRest->getmultipleRestSoundingWholeNotes ();
    
  // account for multipleRest duration in measure length
  setMeasureLength (
    inputLineNumber,
    fMeasureLength + multipleRestSoundingWholeNotes);

  // update part measure length high tide if need be
  fMeasureDirectPartUplink->
    updatePartMeasureLengthHighTide (
      inputLineNumber,
      fMeasureLength);
*/

  // determine if the multipleRest occupies a full measure
// XXL  JMI  if (multipleRestSoundingWholeNotes == fMeasureDivisionsPerWholeMeasure)
    // multipleRest->setmultipleRestOccupiesAFullMeasure ();

  // append the multipleRest to the measure elements list
  fMeasureElementsList.push_back (multipleRest);

  // this measure contains music
  fMeasureContainsMusic = true;
}

void msrMeasure::appendChordToMeasure (S_msrChord chord) // JMI XXL
{
  int inputLineNumber =
    chord->getInputLineNumber ();
    
  // populate measure uplink
  chord->setChordMeasureUplink (this);

#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceChords || gTraceOptions->fTraceMeasures) {
    gLogIOstream <<
      "Appending chord '" << chord->asString () <<
      "' to measure '" << fMeasureNumber <<
      "' in voice \"" <<
      fMeasureSegmentUplink->
        getSegmentVoiceUplink ()->
          getVoiceName () <<
      "\"" <<
      endl;
  }
#endif

  // register chord measure number
  chord->
    setChordMeasureNumber (fMeasureNumber);
  
  // register chord measure position in measure
  chord->
    setChordPositionInMeasure (
      fMeasureLength);

  // copy measure number to first note, that was created beforehand
  chord->
    setChordFirstNoteMeasureNumber (
      fMeasureNumber);
  
  // copy measure position in measure to first note, that was created beforehand
  chord->
    setChordFirstNotePositionInMeasure (
      fMeasureLength);

  // fetch chord sounding whole notes
  rational
    chordSoundingWholeNotes =
      chord->
        getChordSoundingWholeNotes ();
    
  // account for chord duration in measure length
  setMeasureLength (
    inputLineNumber,
    fMeasureLength + chordSoundingWholeNotes);

  // update part measure length high tide if need be
  fetchMeasurePartUplink ()->
    updatePartMeasureLengthHighTide (
      inputLineNumber,
      fMeasureLength);

  // determine if the chord occupies a full measure
// XXL  JMI  if (chordSoundingWholeNotes == fMeasureDivisionsPerWholeMeasure)
    // chord->setChordOccupiesAFullMeasure ();

  // append the chord to the measure elements list
  fMeasureElementsList.push_back (chord);

  // this measure contains music
  fMeasureContainsMusic = true;
}

void msrMeasure::appendTupletToMeasure (S_msrTuplet tuplet)
{
  int inputLineNumber =
    tuplet->getInputLineNumber ();
    
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceTuplets || gTraceOptions->fTraceMeasures) {
    gLogIOstream <<
      "Appending tuplet '" << tuplet->asString () <<
      "' to measure '" << fMeasureNumber <<
      "' in voice \"" <<
      fMeasureSegmentUplink->
        getSegmentVoiceUplink ()->
          getVoiceName () <<
      "\"" <<
      endl;
  }
#endif

  gIndenter++;

  // populate measure uplink
  tuplet->setTupletMeasureUplink (this);

  // register tuplet measure number
  tuplet->
    setTupletMeasureNumber (fMeasureNumber);

  /* JMI ???
  // register tuplet measure position in measure
  rational
    dummy = // JMI
      tuplet->
        setTupletPositionInMeasure (
          fMeasureLength);
          */

/* JMI
  // copy measure number to first note, that was created beforehand
  tuplet->
    setTupletFirstNoteMeasureNumber (fMeasureNumber);
  
  // copy measure position in measure to first note, that was created beforehand
  tuplet->
    setTupletFirstNotePositionInMeasure (
      fMeasureLength);
 */
  
  // fetch tuplet sousnding whole notes
  rational
    tupletSoundingWholeNotes =
      tuplet->
        getTupletSoundingWholeNotes ();
    
  // account for tuplet duration in measure length
  setMeasureLength (
    inputLineNumber,
    fMeasureLength + tupletSoundingWholeNotes);

  // update part measure length high tide if need be
  fetchMeasurePartUplink ()->
    updatePartMeasureLengthHighTide (
      inputLineNumber,
      fMeasureLength);

/* JMI
  // set tuplet members' displayed whole notes
  tuplet->
    applyDisplayFactorToTupletMembers ();
    */

  // determine if the tuplet occupies a full measure
// JMI    if (tupletSoundingWholeNotes == fMeasureDivisionsPerWholeMeasure)
    // tuplet->setTupletOccupiesAFullMeasure ();

  // append the tuplet to the measure elements list
  fMeasureElementsList.push_back (tuplet);

  // this measure contains music
  fMeasureContainsMusic = true;

/* JMI
  // bring harmony voice to the new measure length
  fetchMeasurePartUplink->
    getPartHarmonyVoice ()->
      padUpToMeasureLengthInVoice (
        inputLineNumber,
        fMeasureLength);
        */

  gIndenter--;
}

void msrMeasure::appendHarmonyToMeasure (S_msrHarmony harmony)
{
  int inputLineNumber =
    harmony->getInputLineNumber ();
    
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceHarmonies || gTraceOptions->fTraceMeasures) {
    gLogIOstream <<
      "Appending harmony '" << harmony->asString () <<
      "' to measure '" << fMeasureNumber <<
      "' in voice \"" <<
      fMeasureSegmentUplink->
        getSegmentVoiceUplink ()->
          getVoiceName () <<
      "\"" <<
      ", measureLength = " << fMeasureLength <<
      endl;
  }
#endif

  // fetch harmony sounding whole notes
  rational
    harmonySoundingWholeNotes =
      harmony->
        getHarmonySoundingWholeNotes ();
    
  // account for harmony duration in measure length
  setMeasureLength (
    inputLineNumber,
    fMeasureLength + harmonySoundingWholeNotes);

  // update part measure length high tide if need be
  fetchMeasurePartUplink ()->
    updatePartMeasureLengthHighTide (
      inputLineNumber,
      fMeasureLength);
  
  // append the harmony to the measure elements list
  fMeasureElementsList.push_back (harmony);

  // this measure contains music
  fMeasureContainsMusic = true;
}

void msrMeasure::appendHarmonyToMeasureClone (S_msrHarmony harmony)
{
  int inputLineNumber =
    harmony->getInputLineNumber ();
    
  // regular insertion in current measure
  
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceHarmonies || gTraceOptions->fTraceMeasures) {
    gLogIOstream <<
      "Appending harmony '" << harmony->asString () <<
      "' to measure clone '" << fMeasureNumber <<
      "' in voice clone \"" <<
      fMeasureSegmentUplink->
        getSegmentVoiceUplink ()->
          getVoiceName () <<
      "\"" <<
      ", measureLength = " << fMeasureLength <<
      endl;
  }
#endif
      
  // fetch harmony sounding whole notes
  rational
    harmonySoundingWholeNotes =
      harmony->
        getHarmonySoundingWholeNotes ();
    
  // account for harmony duration in measure length
  setMeasureLength (
    inputLineNumber,
    fMeasureLength + harmonySoundingWholeNotes);

  // update part measure length high tide if need be
  fetchMeasurePartUplink ()->
    updatePartMeasureLengthHighTide (
      inputLineNumber,
      fMeasureLength);
  
  // append the harmony to the measure elements list
  fMeasureElementsList.push_back (harmony);

  // this measure contains music
  fMeasureContainsMusic = true;
}

void msrMeasure::appendFiguredBassToMeasure (
  S_msrFiguredBass figuredBass)
{
  int inputLineNumber =
    figuredBass->getInputLineNumber ();
    
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceHarmonies || gTraceOptions->fTraceMeasures) {
    gLogIOstream <<
      "Appending figured bass '" << figuredBass->asString () <<
      "' to measure '" << fMeasureNumber <<
      "' in voice \"" <<
      fMeasureSegmentUplink->
        getSegmentVoiceUplink ()->
          getVoiceName () <<
      "\"" <<
      ", measureLength = " << fMeasureLength <<
      endl;
  }
#endif

  // fetch harmony sounding whole notes
  rational
    figuredBassSoundingWholeNotes =
      figuredBass->
        getFiguredBassSoundingWholeNotes ();
    
  // account for harmony duration in measure length
  setMeasureLength (
    inputLineNumber,
    fMeasureLength + figuredBassSoundingWholeNotes);

  // update part measure length high tide if need be
  fetchMeasurePartUplink ()->
    updatePartMeasureLengthHighTide (
      inputLineNumber,
      fMeasureLength);
  
  // append the harmony to the measure elements list
  fMeasureElementsList.push_back (figuredBass);

  // this measure contains music
  fMeasureContainsMusic = true;
}

void msrMeasure::appendFiguredBassToMeasureClone (
  S_msrFiguredBass figuredBass)
{
  int inputLineNumber =
    figuredBass->getInputLineNumber ();
    
  // regular insertion in current measure
  
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceHarmonies || gTraceOptions->fTraceMeasures) {
    gLogIOstream <<
      "Appending figured bass '" << figuredBass->asString () <<
      "' to measure clone '" << fMeasureNumber <<
      "' in voice clone \"" <<
      fMeasureSegmentUplink->
        getSegmentVoiceUplink ()->
          getVoiceName () <<
      "\"" <<
      ", measureLength = " << fMeasureLength <<
      endl;
  }
#endif
      
  // fetch harmony sounding whole notes
  rational
    figuredBassSoundingWholeNotes =
      figuredBass->
        getFiguredBassSoundingWholeNotes ();
    
  // account for harmony duration in measure length
  setMeasureLength (
    inputLineNumber,
    fMeasureLength + figuredBassSoundingWholeNotes);

  // update part measure length high tide if need be
  fetchMeasurePartUplink ()->
    updatePartMeasureLengthHighTide (
      inputLineNumber,
      fMeasureLength);
  
  // append the harmony to the measure elements list
  fMeasureElementsList.push_back (figuredBass);

  // this measure contains music
  fMeasureContainsMusic = true;
}

S_msrNote msrMeasure::createPaddingNoteForVoice (
  int        inputLineNumber,
  rational   duration,
  S_msrVoice voice)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceMeasures) {
    gLogIOstream <<
      "Creating a padding note for voice \"" <<
      voice->getVoiceName () <<
      "\" in measure '" <<
      fMeasureNumber <<
      "', duration = '" <<
      duration <<
      "', line " << inputLineNumber <<
      endl;
  }
#endif

 // JMI abort ();
  
  // create a rest or a skip depending on measureVoice kind
  S_msrNote paddingNote;

  switch (voice->getVoiceKind ()) {
    case msrVoice::kRegularVoice:
      paddingNote =
        msrNote::createRestNote (
          inputLineNumber,
          duration,
          duration,
          0, // dots number JMI ???
          voice->
            getVoiceStaffUplink ()->getStaffNumber (),
          voice->
            getVoiceNumber ());
      break;
      
    case msrVoice::kHarmonyVoice:
    case msrVoice::kFiguredBassVoice:
      paddingNote =
        msrNote::createSkipNote (
          inputLineNumber,
          duration,
          duration,
          0, // dots number JMI ???
          voice->
            getVoiceStaffUplink ()->getStaffNumber (),
          voice->
            getVoiceNumber ());
      break;
  } // switch

  return paddingNote;
}

void msrMeasure::padUpToMeasureLengthInMeasure (
  int      inputLineNumber,
  rational measureLength)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceMeasures) {
    gLogIOstream <<
      "Padding from measure length '" << fMeasureLength <<
      "' to '" << measureLength <<
      "' in measure " <<
      fMeasureNumber <<
      "' in segment " <<
      fMeasureSegmentUplink->getSegmentAbsoluteNumber () << 
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  gIndenter++;
  
  if (fMeasureLength < measureLength) {
    // appending a padding rest or skip to this measure to reach measureLength
    rational
      missingDuration =
        measureLength - fMeasureLength;
    
    // fetch the measure voice
    S_msrVoice
      measureVoice =
        fMeasureSegmentUplink->
          getSegmentVoiceUplink ();
      
    // create a rest or a skip depending on measureVoice kind
    S_msrNote
      paddingNote =
        createPaddingNoteForVoice (
          inputLineNumber,
          missingDuration,
          measureVoice);

/* JMI
    switch (measureVoice->getVoiceKind ()) {
      case msrVoice::kRegularVoice:
        paddingNote =
          msrNote::createRestNote (
            inputLineNumber,
            missingDuration,
            missingDuration,
            0, // dots number JMI ???
            measureVoice->
              getVoiceStaffUplink ()->getStaffNumber (),
            measureVoice->
              getVoiceNumber ());
        break;
        
      case msrVoice::kHarmonyVoice:
      case msrVoice::kFiguredBassVoice:
        paddingNote =
          msrNote::createSkipNote (
            inputLineNumber,
            missingDuration,
            missingDuration,
            0, // dots number JMI ???
            measureVoice->
              getVoiceStaffUplink ()->getStaffNumber (),
            measureVoice->
              getVoiceNumber ());
        break;
    } // switch
*/

/* JMI
    // does the rest occupy a full measure?
    if (missingDuration == fMeasureFullLength)
      paddingNote->
        setNoteOccupiesAFullMeasure ();
  */
  
    // register rest's measure length
    paddingNote->
      setNotePositionInMeasure (
        fMeasureLength);
           
#ifdef TRACE_OPTIONS
    if (gTraceOptions->fTraceMeasures || gTraceOptions->fTraceDivisions) {
      gLogIOstream <<
       "Appending rest" << paddingNote->asString () <<
       " (missingDuration " << missingDuration <<
       " whole notes) to skip from length '" << fMeasureLength <<
       " to length '" << measureLength << "'"
       " in measure '" << fMeasureNumber <<
       "in voice \"" << measureVoice->getVoiceName () <<
       endl;
    }
#endif

    // append the rest to the measure elements list
    // only now to make it possible to remove it afterwards
    // if it happens to be the first note of a chord
    appendNoteToMeasure (paddingNote);

    // set this measure as being padded
    this->
      setMeasureCreatedForARepeatKind (
        msrMeasure::kMeasureCreatedForARepeatPadded);
    
    // this measure contains music
    fMeasureContainsMusic = true;

/*
    // account for rest duration in measure length
    setMeasureLength (
      inputLineNumber, fMeasureLength + missingDuration);
*/
  }

  gIndenter--;
}

void msrMeasure::appendGraceNotesToMeasure (
  S_msrGraceNotes graceNotes)
{
  fMeasureElementsList.push_back (graceNotes);

  // set graceNotes' measure number
  graceNotes->
    setGraceNotesMeasureNumber (
      this->getMeasureNumber ());
    
  // this measure contains music
  fMeasureContainsMusic = true;
}
  
void msrMeasure::prependGraceNotesToMeasure (
  S_msrGraceNotes graceNotes)
{
  // in order to work around LilyPond issue 34,
  // we need to insert the skip grace notes
  // after clef, key and time signature if any

  for (
    list<S_msrElement>::iterator i = fMeasureElementsList.begin ();
    i != fMeasureElementsList.end ();
    i++ ) {

    if (
      S_msrClef clef = dynamic_cast<msrClef*>(&(*(*i)))
      ) {
    }
  
    else if (
      S_msrKey key = dynamic_cast<msrKey*>(&(*(*i)))
      ) {
    }
    
    else if (
      S_msrTime time = dynamic_cast<msrTime*>(&(*(*i)))
      ) {
    }
    
    else {
       // insert graceNotes before (*i) in the list
       // JMI what about further such occurrences???
      fMeasureElementsList.insert (
        i, graceNotes);

      break;
    }
  } // for

  // this measure contains music
  fMeasureContainsMusic = true;
}
  
void msrMeasure::appendAfterGraceNotesToMeasure (
  S_msrAfterGraceNotes afterGraceNotes)
{
  fMeasureElementsList.push_back (afterGraceNotes);

  // this measure contains music
  fMeasureContainsMusic = true;
}
  
void msrMeasure::prependAfterGraceNotesToMeasure (
  S_msrAfterGraceNotes afterGraceNotes)
{
  // in order to work around LilyPond issue 34,
  // we need to insert the skip after grace notes
  // after clef, key and time signature if any

  for (
    list<S_msrElement>::iterator i = fMeasureElementsList.begin ();
    i != fMeasureElementsList.end ();
    i++ ) {

    if (
      S_msrClef clef = dynamic_cast<msrClef*>(&(*(*i)))
      ) {
    }
  
    else if (
      S_msrKey key = dynamic_cast<msrKey*>(&(*(*i)))
      ) {
    }
    
    else if (
      S_msrTime time = dynamic_cast<msrTime*>(&(*(*i)))
      ) {
    }
    
    else {
       // insert afterGraceNotes before (*i) in the list
      fMeasureElementsList.insert (
        i, afterGraceNotes);

      break;
    }
  } // for

  // this measure contains music
  fMeasureContainsMusic = true;
}

void msrMeasure::appendTempoToMeasure (
  S_msrTempo tempo)
{
  fMeasureElementsList.push_back (tempo);
}

void msrMeasure::appendRehearsalToMeasure (
  S_msrRehearsal rehearsal)
{
  fMeasureElementsList.push_back (rehearsal);
}

void msrMeasure::appendOctaveShiftToMeasure (
  S_msrOctaveShift octaveShift)
{
  fMeasureElementsList.push_back (octaveShift);
}

void msrMeasure::appendScordaturaToMeasure (
  S_msrScordatura scordatura)
{
  fMeasureElementsList.push_back (scordatura);
}

void msrMeasure::appendAccordionRegistrationToMeasure (
  S_msrAccordionRegistration
    accordionRegistration)
{
  fMeasureElementsList.push_back (accordionRegistration);
}    

void msrMeasure::appendHarpPedalsTuningToMeasure (
  S_msrHarpPedalsTuning
    harpPedalsTuning)
{
  fMeasureElementsList.push_back (harpPedalsTuning);
}    

void msrMeasure::appendLineBreakToMeasure (S_msrLineBreak lineBreak)
{
  fMeasureElementsList.push_back (lineBreak);
}

void msrMeasure::appendPageBreakToMeasure (S_msrPageBreak pageBreak)
{
  fMeasureElementsList.push_back (pageBreak);
}

void msrMeasure::appendStaffDetailsToMeasure (
  S_msrStaffDetails staffDetails)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceStaves || gTraceOptions->fTraceMeasures) { 
    gLogIOstream <<
      "Appending staff details '" << staffDetails->asShortString () <<
      "' to measure '" << fMeasureNumber <<
      "' in voice \"" <<
      fMeasureSegmentUplink->
        getSegmentVoiceUplink ()->
          getVoiceName () <<
      "\"" <<
      endl;
  }
#endif

  fMeasureElementsList.push_back (staffDetails);
}

void msrMeasure::appendBarNumberCheckToMeasure (
  S_msrBarNumberCheck barNumberCheck)
{
  fMeasureElementsList.push_back (barNumberCheck);
}

void msrMeasure::prependOtherElementToMeasure (S_msrElement elem)
{
  fMeasureElementsList.push_front (elem); // JMI

  // this measure contains music
  fMeasureContainsMusic = true;
}

void msrMeasure::appendOtherElementToMeasure  (S_msrElement elem)
{
  fMeasureElementsList.push_back (elem);

  // this measure contains music
  fMeasureContainsMusic = true;
}

void msrMeasure::removeNoteFromMeasure (
  int       inputLineNumber,
  S_msrNote note)
{  
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceChords || gTraceOptions->fTraceMeasures) {
    gLogIOstream <<
      "Removing note '" <<
      note->asShortString () <<
      "' from measure '" << fMeasureNumber <<
      "' in voice \"" <<
      fMeasureSegmentUplink->
        getSegmentVoiceUplink ()->
          getVoiceName () <<
      "\"," <<
      "fMeasureLastHandledNote:" <<
      endl <<
      fMeasureLastHandledNote->asShortString () <<
      endl;
    gIndenter--;
  }
#endif

  for (
    list<S_msrElement>::iterator i=fMeasureElementsList.begin ();
    i!=fMeasureElementsList.end ();
    ++i) {
    if ((*i) == note) {
      // found note, erase it
      fMeasureElementsList.erase (i);
      
      // update measure length
      setMeasureLength (
        inputLineNumber,
        fMeasureLength
          -
        fMeasureLastHandledNote->getNoteSoundingWholeNotes ());

      // return from function
      return;
    }
  } // for

  S_msrVoice
    segmentVoiceUplink =
      fMeasureSegmentUplink->
        getSegmentVoiceUplink ();

  S_msrPart
    segmentVoicePart =
      segmentVoiceUplink->
        getVoiceStaffUplink ()->
        getStaffPartUplink ();
      
  gLogIOstream <<
    endl <<
    endl <<
    "@@@@@@@@@@@@@@@@@ segmentVoicePart" <<
    endl <<
    segmentVoicePart <<
    endl <<
    "@@@@@@@@@@@@@@@@@" <<
    endl <<
    endl;
    
  gLogIOstream <<
    endl <<
    endl <<
    "@@@@@@@@@@@@@@@@@ segmentVoiceUplink" <<
    endl <<
    segmentVoiceUplink <<
    endl <<
    "@@@@@@@@@@@@@@@@@" <<
    endl <<
    endl;
    
  stringstream s;

  s <<
    "cannot remove note " <<
    note <<
    " from measure " << fMeasureNumber <<
    "' in voice \"" <<
    segmentVoiceUplink->getVoiceName () <<
    "\"," <<
    " since it has not been found";

  msrInternalError (
    gXml2lyOptions->fInputSourceName,
    inputLineNumber,
    __FILE__, __LINE__,
    s.str ());
}

void msrMeasure::removeElementFromMeasure (
  int          inputLineNumber,
  S_msrElement element)
{  
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceChords || gTraceOptions->fTraceMeasures) {
    gLogIOstream <<
      "Removing element:" <<
      endl;

    gIndenter++;
    gLogIOstream <<
      " JMI ??? element->elementAsString ()" << endl;
    gIndenter--;
    
    gLogIOstream <<
      endl <<
      " from measure '" << fMeasureNumber <<
      "' in voice \"" <<
      fMeasureSegmentUplink->
        getSegmentVoiceUplink ()->
          getVoiceName () <<
      "\"," <<
      endl;

    gIndenter++;
    gLogIOstream <<
      "fMeasureLastHandledNote:" <<
      endl <<
      fMeasureLastHandledNote <<
      endl;
    gIndenter--;
  }
#endif
  
  for (
    list<S_msrElement>::iterator i=fMeasureElementsList.begin ();
    i!=fMeasureElementsList.end ();
    ++i) {
    if ((*i) == element) {
      // found element, erase it
      fMeasureElementsList.erase (i);
      
      // update measure length
      setMeasureLength (
        inputLineNumber,
        fMeasureLength
          -
        fMeasureLastHandledNote->getNoteSoundingWholeNotes ());

      // return from function
      return;
    }
  } // for
  
  stringstream s;

  s <<
    "cannot remove element " <<
    element <<
    " from measure " << fMeasureNumber <<
    "' in voice \"" <<
    fMeasureSegmentUplink->
      getSegmentVoiceUplink ()->
        getVoiceName () <<
    "\"," <<
    " since it has not been found";

  msrInternalError (
    gXml2lyOptions->fInputSourceName,
    inputLineNumber,
    __FILE__, __LINE__,
    s.str ());
}

/* JMI
S_msrElement msrMeasure::removeLastElementFromMeasure (
  int inputLineNumber)
{
  // fetching measure last element
  S_msrElement
    measureLastElement =
      fMeasureElementsList.back ();
      
  if (gGeneralOptions->fDebug) {
//  if (gGeneralOptions->fDebug) {
    gLogIOstream <<
      "% --> removing last element:" <<
      endl;

    gIndenter++;
    gLogIOstream <<
      measureLastElement;
    gIndenter--;
    
    gLogIOstream <<
      endl <<
      " from measure '" <<
      fMeasureNumber <<
      "' in voice \"" <<
      fMeasureSegmentUplink->
        getSegmentVoiceUplink ()->
          getVoiceName () <<
      "\"," <<
      endl;

    gIndenter++;
    gLogIOstream <<
      "fMeasureLastHandledNote:" <<
      endl <<
      fMeasureLastHandledNote <<
      endl;
    gIndenter--;
  }
  
  if (fMeasureElementsList.size ()) {

    if (fMeasureLastHandledNote) {
      // there's at least a note in the meastuer
      
      if (fMeasureLastHandledNote == measureLastElement) {
        // remove last element
        fMeasureElementsList.pop_back ();

        // update measure length
        fMeasureLength -=
          fMeasureLastHandledNote->getNoteSoundingWholeNotes ();
/ *
// JMI
        // set note's measure position, needed for chord handling
        fMeasureLastHandledNote->
          setNotePositionInMeasure (fMeasureLength);
* /
      }

      else {
        msrInternalError (
          inputLineNumber,
          "cannot removeLastElementFromMeasure () since "
          "fMeasureLastHandledNote is not the last element");
        }
    }

    else {
      msrInternalError (
        inputLineNumber,
        "cannot removeLastElementFromMeasure () since "
        "fMeasureLastHandledNote is null");
    }
  }
  
  else {
    msrInternalError (
      inputLineNumber,
      "cannot removeLastElementFromMeasure () "
      "since fMeasureElementsList is empty");
  }

  return measureLastElement;
}
*/

void msrMeasure::determineMeasureKind (
  int inputLineNumber)
{
  // fetch the voice
  S_msrVoice
    voice =
      fMeasureSegmentUplink->
        getSegmentVoiceUplink ();
    
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceMeasures) {
    gLogIOstream <<
    "Determining the measure kind of measure '" <<
    fMeasureNumber <<
    "' in voice \"" << voice->getVoiceName () <<
    ", line " << inputLineNumber <<
    endl;
  }
#endif

  gIndenter++;

  // determine the measure kind
  if (fMeasureLength.getNumerator () == 0) { // JMI
    // empty measure
#ifdef TRACE_OPTIONS
    if (gTraceOptions->fTraceMeasures) {
      gLogIOstream <<
      "Measure '" << fMeasureNumber <<
      "' in voice \"" << voice->getVoiceName () <<
      "\", is of kind '" <<
      measureKindAsString (fMeasureKind) <<
      "', line " << inputLineNumber <<
      endl;
    }
#endif

    fMeasureKind = kEmptyMeasureKind;
  }
  
  else if (fMeasureLength == fMeasureFullLength) {
    // full measure
#ifdef TRACE_OPTIONS
    if (gTraceOptions->fTraceMeasures) {
      gLogIOstream <<
      "Measure '" << fMeasureNumber <<
      "' in voice \"" << voice->getVoiceName () <<
      "\", is of kind '" <<
      measureKindAsString (fMeasureKind) <<
      "', line " << inputLineNumber <<
      endl;
    }
#endif

    fMeasureKind = kFullMeasureKind;
  }
  
  else if (fMeasureLength < fMeasureFullLength) {
    //  incomplete measure
    switch (fMeasureFirstInSegmentKind) {
      case msrMeasure::kMeasureFirstInSegmentUnknown:
#ifdef TRACE_OPTIONS
        if (gTraceOptions->fTraceMeasures) {
          gLogIOstream <<
          "Measure '" << fMeasureNumber <<
          "' in voice \"" << voice->getVoiceName () <<
          "\", is of kind '" <<
          measureKindAsString (fMeasureKind) <<
          "', line " << inputLineNumber <<
          endl;
        }
#endif
    
        fMeasureKind = kUpbeatMeasureKind;
        break;
        
      case msrMeasure::kMeasureFirstInSegmentYes:
#ifdef TRACE_OPTIONS
        if (gTraceOptions->fTraceMeasures) {
          gLogIOstream <<
          "Measure '" << fMeasureNumber <<
          "' in voice \"" << voice->getVoiceName () <<
          "\", is of kind '" <<
          measureKindAsString (fMeasureKind) <<
          "', line " << inputLineNumber <<
          endl;
        }
#endif
    
        fMeasureKind = kUpbeatMeasureKind;
        break;
        
      case msrMeasure::kMeasureFirstInSegmentNo:
#ifdef TRACE_OPTIONS
        if (gTraceOptions->fTraceMeasures) {
          gLogIOstream <<
          "Measure '" << fMeasureNumber <<
          "' in voice \"" << voice->getVoiceName () <<
          "\", is of kind '" <<
          measureKindAsString (fMeasureKind) <<
          "', line " << inputLineNumber <<
          endl;
        }
#endif
    
        fMeasureKind = kUnderfullMeasureKind;
        break;
    } // switch
  }

  else if (fMeasureLength > fMeasureFullLength) {
    // overfull measure
#ifdef TRACE_OPTIONS
    if (gTraceOptions->fTraceMeasures) {
      gLogIOstream <<
      "Measure '" << fMeasureNumber <<
      "' in voice \"" << voice->getVoiceName () <<
      "\", is **overfull**" <<
      ", line " << inputLineNumber <<
      endl;
    }
#endif

    fMeasureKind = kOverfullMeasureKind;
  }
  
  else {
    // strange measure...
#ifdef TRACE_OPTIONS
    if (gTraceOptions->fTraceMeasures) {
      gLogIOstream <<
      "Measure '" << fMeasureNumber <<
      "' in voice \"" << voice->getVoiceName () <<
      "\", is in a **STRANGE** state" <<
      ", fMeasureKind = " <<
      measureKindAsString (fMeasureKind) <<
      ", line " << inputLineNumber <<
      endl;
    }
#endif
  }

  gIndenter--;
}

void msrMeasure::padUpToPartMeasureLengthHighTide (
  int inputLineNumber)
{
  gIndenter++;
  
  // fetch the part measure length high tide
  rational
    partMeasureLengthHighTide =
      fetchMeasurePartUplink ()->
        getPartMeasureLengthHighTide ();
    
  rational
    lengthToReach =
      partMeasureLengthHighTide;

  if (fMeasureLength < lengthToReach) {
    // appending a rest to this measure to reach lengthToReach 
    rational
      missingDuration =
        lengthToReach - fMeasureLength;
      
    // fetch the voice
    S_msrVoice
      measureVoice =
        fMeasureSegmentUplink->
          getSegmentVoiceUplink ();
    
    // create a rest or a skip depending on measureVoice kind
    S_msrNote
      paddingNote =
        createPaddingNoteForVoice (
          inputLineNumber,
          missingDuration,
          measureVoice);

/* JMI

    switch (measureVoice->getVoiceKind ()) {
      case msrVoice::kRegularVoice:
        paddingNote =
          msrNote::createRestNote (
            inputLineNumber,
            missingDuration,
            missingDuration,
            0, // dots number JMI ???
            measureVoice->
              getVoiceStaffUplink ()->getStaffNumber (),
            measureVoice->
              getVoiceNumber ());
        break;
        
      case msrVoice::kHarmonyVoice:
      case msrVoice::kFiguredBassVoice:
        paddingNote =
          msrNote::createSkipNote (
            inputLineNumber,
            missingDuration,
            missingDuration,
            0, // dots number JMI ???
            measureVoice->
              getVoiceStaffUplink ()->getStaffNumber (),
            measureVoice->
              getVoiceNumber ());
        break;
    } // switch
*/

/* JMI
    // does the rest occupy a full measure?
    if (missingDuration == fMeasureFullLength)
      paddingNote->
        setNoteOccupiesAFullMeasure ();
  */
  
    // register rest's position in measure
    paddingNote->
      setNotePositionInMeasure (fMeasureLength);
           
#ifdef TRACE_OPTIONS
    if (gTraceOptions->fTraceMeasures) {
      gLogIOstream <<
       "Appending '" << paddingNote->asString () <<
       " (" << missingDuration << " whole notes)'" <<
       " to finalize \"" << measureVoice->getVoiceName () <<
       "\" measure: @" << fMeasureNumber << ":" << fMeasureLength <<
       " % --> @" << fMeasureNumber << // JMI
       ":" << partMeasureLengthHighTide <<
        ", missingDuration = " << missingDuration <<
       endl;
   }
#endif

    // append the rest to the measure elements list
    // only now to make it possible to remove it afterwards
    // if it happens to be the first note of a chord
    appendNoteToMeasure (paddingNote);

    // this measure contains music
    fMeasureContainsMusic = true;
  }

  gIndenter--;
}

void msrMeasure::finalizeMeasure (
  int inputLineNumber)
{
  // fetch the voice
  S_msrVoice
    voice =
      fMeasureSegmentUplink->
        getSegmentVoiceUplink ();
    
  // fetch the part measure length high tide
  rational
    partMeasureLengthHighTide =
      fetchMeasurePartUplink ()->
        getPartMeasureLengthHighTide ();
    
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceMeasures) {
    gLogIOstream <<
      "Finalizing measure '" <<
      fMeasureNumber <<
      "' in segment '" <<
      fMeasureSegmentUplink->getSegmentAbsoluteNumber () <<
      "' in voice \"" <<
      voice->getVoiceName () <<
      "\", line " << inputLineNumber <<
      endl;

    gIndenter++;

    const int fieldWidth = 26;
    
    gLogIOstream << left <<
      setw (fieldWidth) <<
      "measureLength" << " = " << fMeasureLength <<
      endl <<
      setw (fieldWidth) <<
      "fMeasureFullLength" << " = " << fMeasureFullLength <<
      endl <<
      setw (fieldWidth) <<
      "partMeasureLengthHighTide" << " = " <<
      partMeasureLengthHighTide <<
      endl;
        
    gIndenter--;
  }
#endif

  gIndenter++;

  // pad measure up to part measure length high tide
  // and determine its measure kind if relevant
  switch (fMeasureKind) {
    case msrMeasure::kSenzaMisuraMeasureKind:
      break;

    case msrMeasure::kOverfullMeasureKind:
    case msrMeasure::kUpbeatMeasureKind:
    case msrMeasure::kFullMeasureKind:
    case msrMeasure::kUnderfullMeasureKind: // JMI
      break;

    case msrMeasure::kUnknownMeasureKind:
    case msrMeasure::kEmptyMeasureKind:
      switch (fMeasureCreatedForARepeatKind) {
        case msrMeasure::kMeasureCreatedForARepeatNo:
          padUpToPartMeasureLengthHighTide (
            inputLineNumber);
          break;

        case msrMeasure::kMeasureCreatedForARepeatBefore:
        case msrMeasure::kMeasureCreatedForARepeatAfter:
          // such a measure should not be padded with a rest
          break;

        case msrMeasure::kMeasureCreatedForARepeatPadded:
          // should not occur
          break;
      } // switch
  
      determineMeasureKind ( // JMI ???
        inputLineNumber);
      break;
  } // switch

  // is there a single note or rest occupying the full measure?
  if (fMeasureLongestNote) {
    if (
      fMeasureLongestNote-> getNoteSoundingWholeNotes ()
        ==
      fMeasureFullLength
    ) {
#ifdef TRACE_OPTIONS
      if (gTraceOptions->fTraceMeasures) {
        gLogIOstream <<
          "Note '" <<
          fMeasureLongestNote->asShortString () <<
          "' occupies measure '" <<
          fMeasureNumber <<
          "' fully in segment '" <<
          fMeasureSegmentUplink->getSegmentAbsoluteNumber () <<
          "' in voice \"" <<
          voice->getVoiceName () <<
          "\", line " << inputLineNumber <<
          endl;
      }
#endif

      fMeasureLongestNote->
        setNoteOccupiesAFullMeasure ();
    }
  }

  gIndenter--;

  if (true && fMeasureNumber == "16") { // JMI
    gLogIOstream <<
      endl <<
      endl <<
      endl <<
      "+++++++++++++++++ finalizeMeasure '" <<
      fMeasureNumber <<
      /* JMI
      "', score:" <<
      endl <<
      fSegmentVoiceUplink->
        getVoiceStaffUplink ()->
          getStaffPartUplink ()->
            getPartPartGroupUplink ()->
              getPartGroupScoreUplink () <<
              */
      "', voice:" <<
      endl <<
      fMeasureSegmentUplink->getSegmentVoiceUplink () <<
      endl <<
      endl <<
      ", line " << inputLineNumber <<
      endl;

  //  abort ();
  }
}

void msrMeasure::acceptIn (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrMeasure::acceptIn ()" <<
      endl;
  }
      
  if (visitor<S_msrMeasure>*
    p =
      dynamic_cast<visitor<S_msrMeasure>*> (v)) {
        S_msrMeasure elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrMeasure::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrMeasure::acceptOut (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrMeasure::acceptOut ()" <<
      endl;
  }

  if (visitor<S_msrMeasure>*
    p =
      dynamic_cast<visitor<S_msrMeasure>*> (v)) {
        S_msrMeasure elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrMeasure::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrMeasure::browseData (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrMeasure::browseData ()" <<
      endl;
  }

  for (
    list<S_msrElement>::const_iterator i = fMeasureElementsList.begin ();
    i != fMeasureElementsList.end ();
    i++) {
    // browse the element
    msrBrowser<msrElement> browser (v);
    browser.browse (*(*i));
  } // for

  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% <== msrMeasure::browseData ()" <<
      endl;
  }
}

string msrMeasure::measureKindAsString (
  msrMeasureKind measureKind)
{
  string result;

  switch (measureKind) {
    case msrMeasure::kUnknownMeasureKind:
      result = "**unknownMeasureKind**";
      break;
    case msrMeasure::kFullMeasureKind:
      result = "fullMeasureKind";
      break;
    case msrMeasure::kUpbeatMeasureKind:
      result = "**upbeatMeasureKind**";
      break;
    case msrMeasure::kUnderfullMeasureKind:
      result = "**underfullMeasureKind**";
      break;
    case msrMeasure::kOverfullMeasureKind:
      result = "**overfullMeasureKind**";
      break;
    case msrMeasure::kSenzaMisuraMeasureKind:
      result = "**senzaMisuraMeasureKind**";
      break;
    case msrMeasure::kEmptyMeasureKind:
      result = "**emptyMeasureKind**";
      break;
  } // switch

  return result;
}

string msrMeasure::measureImplicitKindAsString (
  msrMeasureImplicitKind measureImplicitKind)
{
  string result;

  switch (measureImplicitKind) {
    case msrMeasure::kMeasureImplicitYes:
      result = "measureImplicitYes";
      break;
    case msrMeasure::kMeasureImplicitNo:
      result = "measureImplicitNo";
      break;
  } // switch

  return result;
}

string msrMeasure::measureFirstInSegmentKindAsString (
    msrMeasureFirstInSegmentKind measureFirstInSegmentKind)
{
  string result;

  switch (measureFirstInSegmentKind) {
    case msrMeasure::kMeasureFirstInSegmentUnknown:
      result = "measureFirstInSegmentUnknown";
      break;
    case msrMeasure::kMeasureFirstInSegmentYes:
      result = "measureFirstInSegmentYes";
      break;
    case msrMeasure::kMeasureFirstInSegmentNo:
      result = "measureFirstInSegmentNo";
      break;
  } // switch

  return result;
}
      
string msrMeasure::measureCreatedForARepeatKindAsString (
  msrMeasureCreatedForARepeatKind measureCreatedForARepeatKind)
{
  string result;

  switch (measureCreatedForARepeatKind) {
    case msrMeasure::kMeasureCreatedForARepeatNo:
      result = "measureCreatedForRepeatNo";
      break;
    case msrMeasure::kMeasureCreatedForARepeatBefore:
      result = "measureCreatedForRepeatBefore";
      break;
    case msrMeasure::kMeasureCreatedForARepeatAfter:
      result = "measureCreatedForRepeatAfter";
      break;
    case msrMeasure::kMeasureCreatedForARepeatPadded:
      result = "measureCreatedForARepeatPadded";
      break;
  } // switch

  return result;
}

string msrMeasure::measureKindAsString () const
{
  return
    measureKindAsString (fMeasureKind);
}

void msrMeasure::print (ostream& os)
{
  os <<
    endl <<
    "Measure '" << fMeasureNumber <<
    "', " << measureKindAsString () <<
/* JMI
    ", measureOrdinalNumber = " << fMeasureOrdinalNumber <<
    ", measureLengthAsMSRString: " <<
    measureLengthAsMSRString () <<
    ", measureFullLengthAsMSRString: " <<
    measureFullLengthAsMSRString () <<
    ", " << fMeasureFullLength << " per full measure" <<
    */
    ", " <<
    singularOrPlural (
      fMeasureElementsList.size (), "element", "elements") <<
    ", line " << fInputLineNumber <<
    endl;

  gIndenter++;

  const int fieldWidth = 28;
  
  os << left <<
    setw (fieldWidth) <<
    "segmentUplink" << " : " <<
    fMeasureSegmentUplink->asShortString () <<
    endl <<
    
    setw (fieldWidth) <<
    "measureFirstInSegment" << " : " <<
    msrMeasure::measureFirstInSegmentKindAsString (
      fMeasureFirstInSegmentKind) << 
    endl;
    
  os << left <<
    setw (fieldWidth) <<
    "measureLongestNote" << " : ";
  if (fMeasureLongestNote) {
    os <<
      fMeasureLongestNote->asShortString ();
  }
  else {
    os <<
      "none";
  }
  os <<
    endl;
    
  os << left <<
    setw (fieldWidth) <<
    "measureContainsMusic" << " : " <<
    booleanAsString (
      fMeasureContainsMusic) << 
    endl <<
    
    setw (fieldWidth) <<
    "measureCreatedAfterARepeat" << " : " <<
    msrMeasure::measureCreatedForARepeatKindAsString (
      fMeasureCreatedForARepeatKind) << 
    endl <<
    
    setw (fieldWidth) <<
    "measureIsASingleMeasureRest" << " : " <<
    booleanAsString (fMeasureIsASingleMeasureRest) <<
    endl <<
    
    setw (fieldWidth) <<
    "measureLength" << " : " << fMeasureLength << " whole notes" <<
    endl <<
    
    setw (fieldWidth) <<
    "measureFullLength" << " : ";
    
    // fetch the staff
    S_msrStaff
      staff =
        getMeasureSegmentUplink ()->
          getSegmentVoiceUplink ()->
            getVoiceStaffUplink ();
        
    // get the staff current time
    S_msrTime
      time =
        staff->
          getStaffCurrentTime ();

    if (time) {
      os <<
        fMeasureFullLength << " whole notes";
    }
    else {
      os <<
        "IRRELEVANT";
    }
    os <<
      endl;

  os <<
    setw (fieldWidth) <<
    "nextMeasureNumber" << " : \"" <<
    fNextMeasureNumber <<
    "\"" <<
    endl;

  int
    measureElementsListSize =
      fMeasureElementsList.size ();

  os <<
    setw (fieldWidth) <<
    "fMeasureElementsList" << " : " <<
    singularOrPlural (
      measureElementsListSize, "element", "elements") <<
    endl;

  if (measureElementsListSize) {
    os <<
      endl;
    
    gIndenter++;
    
    list<S_msrElement>::const_iterator
      iBegin = fMeasureElementsList.begin (),
      iEnd   = fMeasureElementsList.end (),
      i      = iBegin;
    for ( ; ; ) {
      os << (*i);
      if (++i == iEnd) break;
      os << endl;
    } // for
    
    gIndenter--;
  }

  gIndenter--;
} 

ostream& operator<< (ostream& os, const S_msrMeasure& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
int msrSegment::gSegmentsCounter = 0;

S_msrSegment msrSegment::create (
  int        inputLineNumber,
  S_msrVoice segmentVoicekUplink)
{
  msrSegment* o =
    new msrSegment (
      inputLineNumber,
      segmentVoicekUplink);
  assert(o!=0);
  
  return o;
}

msrSegment::msrSegment (
  int        inputLineNumber,
  S_msrVoice segmentVoicekUplink)
    : msrElement (inputLineNumber)
{
  // sanity check
  msrAssert(
    segmentVoicekUplink != nullptr,
    "segmentVoicekUplink is null");
    
  // set segment's voice uplink
  fSegmentVoiceUplink =
    segmentVoicekUplink;

  // do other initializations
  initializeSegment ();
}

msrSegment::~msrSegment ()
{}

void msrSegment::initializeSegment ()
{
  fSegmentAbsoluteNumber = ++gSegmentsCounter;
  
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceSegments) {
    gLogIOstream <<
      "% ==> Initializing new segment, gets absolute number '" <<
      fSegmentAbsoluteNumber <<
      "'" <<
      ", in voice \"" <<
      fSegmentVoiceUplink->getVoiceName () <<
      "\"" <<
      endl;
  }
#endif

/* OLD STUFF, NO GOOD JMI
  // create a first measure // JMI
  S_msrMeasure
    measure =
      msrMeasure::create (
        fInputLineNumber,
        "9999",
        this);

  // append the measure to the segment
  appendMeasureToSegment (measure);
*/

  // segment's measure number has not been set yet
  fMeasureNumberHasBeenSetInSegment = false;
}

S_msrPart msrSegment::fetchSegmentPartUplink () const
{
  return
    fSegmentVoiceUplink->
      fetchVoicePartUplink ();
}

S_msrSegment msrSegment::createSegmentNewbornClone (
  S_msrVoice containingVoice)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceSegments) {
    gLogIOstream <<
      "Creating a newborn clone of segment " <<
      asString () <<
      endl;
  }
#endif

  // sanity check
  msrAssert(
    containingVoice != nullptr,
    "containingVoice is null");
    
  S_msrSegment
    newbornClone =
      msrSegment::create (
        fInputLineNumber,
        containingVoice);

  // absolute number
  newbornClone->fSegmentAbsoluteNumber =
    fSegmentAbsoluteNumber;
    
  // number
  newbornClone->fSegmentMeasureNumber =
    fSegmentMeasureNumber;
  newbornClone->fMeasureNumberHasBeenSetInSegment =
    fMeasureNumberHasBeenSetInSegment;

  // the measures in the segment contain the mmusic

  // uplinks

  return newbornClone;
}

S_msrSegment msrSegment::createSegmentDeepCopy (
  S_msrVoice containingVoice)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceSegments) {
    gLogIOstream <<
      "Creating a deep copy of segment " <<
      asString () <<
      endl;
  }
#endif

  // sanity check
  msrAssert(
    containingVoice != nullptr,
    "containingVoice is null");
    
  S_msrSegment
    segmentDeepCopy =
      msrSegment::create (
        fInputLineNumber,
        containingVoice);
  
  // absolute number
  segmentDeepCopy->fSegmentAbsoluteNumber =
    fSegmentAbsoluteNumber;
    
  // number
  segmentDeepCopy->fSegmentMeasureNumber =
    fSegmentMeasureNumber;
  segmentDeepCopy->fMeasureNumberHasBeenSetInSegment =
    fMeasureNumberHasBeenSetInSegment;

  // the measures in the segment contain the mmusic
  int numberOfSegmentMeasures =
   fSegmentMeasuresList.size () ;

  if (numberOfSegmentMeasures) {
#ifdef TRACE_OPTIONS
    if (gTraceOptions->fTraceVoices) {
      gLogIOstream <<
        "There are " <<
        numberOfSegmentMeasures <<
        " measures in segment to be deep copied" <<
        endl;
    }
#endif
    
    for (
      list<S_msrMeasure>::const_iterator i = fSegmentMeasuresList.begin ();
      i != fSegmentMeasuresList.end ();
      i++) {
      // append a deep copy of the measure to the deep copy
      segmentDeepCopy->
        appendMeasureToSegment (
          (*i)->
            createMeasureDeepCopy (
              this));
    } // for
  }
  
  else {
#ifdef TRACE_OPTIONS
    if (gTraceOptions->fTraceSegments) {
      gLogIOstream <<
        "There are no measures in segment to be deep copied" <<
        endl;
    }
#endif
  }

  // uplinks
  segmentDeepCopy->fSegmentVoiceUplink =
    containingVoice;
  
  return segmentDeepCopy;
}

void msrSegment::createMeasureAndAppendItToSegment (
  int    inputLineNumber,
  string measureNumber,
  int    measureOrdinalNumber,
  msrMeasure::msrMeasureImplicitKind
         measureImplicitKind)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceMeasures || gTraceOptions->fTraceSegments) {
    gLogIOstream <<
      "Creating and appending a measure '" << measureNumber <<
      "', to segment '" << asString () <<
      "' in voice \"" <<
      fSegmentVoiceUplink->getVoiceName () <<
      "\"" <<
      "', line " << inputLineNumber <<
      endl;
  }
#endif

  gIndenter++;
  
  fSegmentMeasureNumber = measureNumber; // JMI

  // determine new measure 'first in segment' kind
  msrMeasure::msrMeasureFirstInSegmentKind
    measureFirstInSegmentKind;

  if (fSegmentMeasuresList.size () == 0) {
    // this is the first measure in the segment
    measureFirstInSegmentKind =
      msrMeasure::kMeasureFirstInSegmentYes;
  }
  else {
    // this is not the first measure in the segment
    measureFirstInSegmentKind =
      msrMeasure::kMeasureFirstInSegmentNo;
  }
      
  // create a measure
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceMeasures || gTraceOptions->fTraceSegments) {
    gLogIOstream <<
      "Creating measure '" << measureNumber <<
      "' in segment " << asString () <<
      ", in voice \"" <<
      fSegmentVoiceUplink->getVoiceName () <<
      "\"" <<
      ", line " << inputLineNumber <<
      ", measure is " <<
      msrMeasure::measureFirstInSegmentKindAsString (
        measureFirstInSegmentKind) <<
      endl;
  }
#endif

  S_msrMeasure
    newMeasure =
      msrMeasure::create (
        inputLineNumber,
        measureNumber,
        this);

  // set 'first in segment' kind
  newMeasure->
    setMeasureFirstInSegmentKind (
      measureFirstInSegmentKind);
  
  // append it to the segment's measures list
  fSegmentMeasuresList.push_back (
    newMeasure);
  
  fMeasureNumberHasBeenSetInSegment = true;

  gIndenter--;
}

void msrSegment::setNextMeasureNumberInSegment (
  int    inputLineNumber,
  string nextMeasureNumber)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceMeasures || gTraceOptions->fTraceSegments) {
    gLogIOstream <<
      "Setting next measure number to '" << nextMeasureNumber <<
      "' in segment '" << asString () <<
      "' in voice \"" <<
      fSegmentVoiceUplink->getVoiceName () <<
      "\"" <<
      "', line " << inputLineNumber <<
      endl;
  }
#endif

  gIndenter++;
  
  if (fSegmentMeasuresList.size ()) { // JMI ???
#ifdef TRACE_OPTIONS
    if (gTraceOptions->fTraceMeasures || gTraceOptions->fTraceSegments) {
      gLogIOstream <<
        "Setting next measure number to '" << nextMeasureNumber <<
        "' in segment '" << asString () <<
        "'s last measure " <<
        "' in voice \"" <<
        fSegmentVoiceUplink->getVoiceName () <<
        "\"" <<
        "', line " << inputLineNumber <<
        endl;
    }
#endif
    
    fSegmentMeasuresList.back ()->
      setNextMeasureNumber (
        nextMeasureNumber);
  }

/* JMI
  else { // JMI
    gLogIOstream <<
      endl <<
      endl <<
      endl <<
      fSegmentVoiceUplink <<
      endl <<
      endl;

    exit (1);
  }
  */

  gIndenter--;   
}

void msrSegment::finalizeCurrentMeasureInSegment (
  int    inputLineNumber)
{
  string
    currentMeasureNumber =
      fetchSegmentPartUplink ()->
        getPartCurrentMeasureNumber ();
      
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceMeasures || gTraceOptions->fTraceSegments) {
    gLogIOstream <<
      "Finalizing current measure '" <<
      currentMeasureNumber <<
      "' in segment " <<
      asString () <<
      ", in voice \"" <<
      fSegmentVoiceUplink->getVoiceName () <<
      "\"" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  gIndenter++;
  
  // don't finalize it it ain't been created
  if (fSegmentMeasuresList.size ()) {
    // finalize or remove segment's last measure
    S_msrMeasure
      lastMeasure =
        fSegmentMeasuresList.back ();

    // is last measure an empty measure that was created for a repeat?
    msrMeasure::msrMeasureCreatedForARepeatKind
      lastMeasureCreatedForARepeatKind =
        lastMeasure->
          getMeasureCreatedForARepeatKind ();
        
    switch (lastMeasureCreatedForARepeatKind) {
      case msrMeasure::kMeasureCreatedForARepeatNo:
        // is the last measure empty?
 //       if (lastMeasure->getMeasureLength ().getNumerator () == 0) { // JMI ???
        if (lastMeasure->getMeasureElementsList ().size () == 0) { // JMI ???
          // yes, remove it
#ifdef TRACE_OPTIONS
          if (
            gTraceOptions->fTraceMeasures
              ||
            gTraceOptions->fTraceSegments
              ||
            gTraceOptions->fTraceRepeats) {
            stringstream s;
        
            gLogIOstream <<
              "Removing empty measure '" <<
              lastMeasure->getMeasureNumber () <<
              ") in segment '" <<
              asString () <<
              "'";
          }
#endif
    
          fSegmentMeasuresList.pop_back ();
        }
        
        else {
          // no, finalize it
          lastMeasure->
            finalizeMeasure (
              inputLineNumber);
        }
        break;

      case msrMeasure::kMeasureCreatedForARepeatBefore:
      case msrMeasure::kMeasureCreatedForARepeatAfter:
        if (lastMeasure->getMeasureLength ().getNumerator () == 0) {
          // yes, remove it
#ifdef TRACE_OPTIONS
          if (
            gTraceOptions->fTraceMeasures
              ||
            gTraceOptions->fTraceSegments
              ||
            gTraceOptions->fTraceRepeats) {
            stringstream s;
        
            gLogIOstream <<
              "Removing empty measure '" <<
              lastMeasure->getMeasureNumber () <<
              "' that was created for a repeat (" <<
                msrMeasure::measureCreatedForARepeatKindAsString (
                  lastMeasureCreatedForARepeatKind) <<
              ") in segment '" <<
              asString () <<
              "'";
          }
#endif
    
          fSegmentMeasuresList.pop_back ();
        }
        else {
          // no, finalize it
          lastMeasure->
            finalizeMeasure (
              inputLineNumber);
        }
        break;

      case msrMeasure::kMeasureCreatedForARepeatPadded:
          // finalize it JMI ???
          lastMeasure->
            finalizeMeasure (
              inputLineNumber);
        break;
    } // switch

    /* JMI
    else {
#ifdef TRACE_OPTIONS
      if (gTraceOptions->fTraceMeasures) {
        stringstream s;
    
        gLogIOstream <<
          "Cannot finalize current measure '" <<
          lastMeasure->getMeasureNumber () <<
          "' in segment '" <<
          asString () <<
          "' since it is empty, removing it";
      }
#endif
    
      fSegmentMeasuresList.pop_back ();
    }
  */
  
#ifdef TRACE_OPTIONS
    if (
      gTraceOptions->fTraceMeasuresDetails
        ||
      gTraceOptions->fTraceSegmentsDetails) {
      gLogIOstream <<
        endl <<
        "*********>> Current voice \"" <<
        fSegmentVoiceUplink->getVoiceName () <<
        "\"" <<
        ", line " << inputLineNumber <<
        " contains:" <<
        endl <<
        fSegmentVoiceUplink <<
        "<<*********" <<
        endl <<
        endl;
    }
#endif
  }

  else {
    /*
#ifdef TRACE_OPTIONS
    if (
      gTraceOptions->fTraceMeasuresDetails
        ||
      gTraceOptions->fTraceSegmentsDetails) {
      gLogIOstream <<
        endl <<
        "*********>> HOUHOU Current voice \"" <<
        fSegmentVoiceUplink->getVoiceName () <<
        "\"" <<
        ", line " << inputLineNumber <<
        " contains:" <<
        endl <<
        fSegmentVoiceUplink <<
        "<<*********" <<
        endl <<
        endl;
    }
#endif
    */

    gLogIOstream <<
      "Segment '" <<
      fSegmentAbsoluteNumber <<
      "' has no measures to finalize" <<
      endl;
  }

  gIndenter--;
}

void msrSegment::appendClefToSegment (S_msrClef clef)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceClefs || gTraceOptions->fTraceSegments) {
    gLogIOstream <<
      "Appending clef '" << clef->asString () <<
      "' to segment " << asString () <<
      ", in voice \"" <<
      fSegmentVoiceUplink->getVoiceName () <<
      "\"" <<
      endl;
  }
#endif

  gIndenter++;
  
      /* JMI
  fetchSegmentPartUplink ()->
    print (gLogIOstream);
*/

  // sanity check
  if (fSegmentMeasuresList.size () == 0) {
    stringstream s;

    s <<
      "SegmentMeasuresList is empty"  <<
      " in segment '" <<
      fSegmentAbsoluteNumber <<
      "' in voice \"" <<
      fSegmentVoiceUplink->getVoiceName () <<
      "\"";

    gLogIOstream <<
      "SegmentVoiceUplink:" <<
      endl;
    gIndenter++;
    gLogIOstream <<
      fSegmentVoiceUplink <<
      endl;
    gIndenter--;
    
    msrInternalError (
      gXml2lyOptions->fInputSourceName,
      clef->getInputLineNumber (),
      __FILE__, __LINE__,
      s.str ());
  }
        
  // register clef in segments's current measure
  fSegmentMeasuresList.back ()->
    appendClefToMeasure (clef);

  gIndenter--;
}

void msrSegment::prependClefToSegment (S_msrClef clef) // JMI
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceClefs || gTraceOptions->fTraceSegments) {
    gLogIOstream <<
      "Prepending clef '" << clef->asString () <<
      "' to segment " << asString () <<
      ", in voice \"" <<
      fSegmentVoiceUplink->getVoiceName () <<
      "\"" <<
      endl;
  }
#endif

  gIndenter++;
  
      /* JMI
  fetchSegmentPartUplink ()->
    print (gLogIOstream);
*/

  // sanity check
  if (fSegmentMeasuresList.size () == 0) {
    stringstream s;

    s <<
      "SegmentMeasuresList is empty"  <<
      " in segment '" <<
      fSegmentAbsoluteNumber <<
      "' in voice \"" <<
      fSegmentVoiceUplink->getVoiceName () <<
      "\"";

    gLogIOstream <<
      "SegmentVoiceUplink:" <<
      endl;
    gIndenter++;
    gLogIOstream <<
      fSegmentVoiceUplink <<
      endl;
    gIndenter--;
    
    msrInternalError (
      gXml2lyOptions->fInputSourceName,
      clef->getInputLineNumber (),
      __FILE__, __LINE__,
      s.str ());
  }
        
  // register clef in segments's current measure
  fSegmentMeasuresList.front ()->
    appendClefToMeasure (clef);

  gIndenter--;
}

void msrSegment::appendKeyToSegment (S_msrKey key)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceKeys || gTraceOptions->fTraceSegments) {
    gLogIOstream <<
      "Appending key '" << key->asString () <<
      "' to segment " << asString () <<
    ", in voice \"" <<
    fSegmentVoiceUplink->getVoiceName () <<
    "\"" <<
      endl;
  }
#endif
      
  // register key in segment
// JMI  fSegmentKey = key;

  // sanity check
  msrAssert (
    fSegmentMeasuresList.size () > 0,
    "fSegmentMeasuresList is empty");

  gIndenter++;
  
  // register key in segments's current measure
  fSegmentMeasuresList.back ()->
    appendKeyToMeasure (key);

  gIndenter--;
}
    
void msrSegment::appendTimeToSegment (S_msrTime time)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceTimes || gTraceOptions->fTraceSegments) {
    gLogIOstream <<
      "Appending time:" <<
      endl;

    gIndenter++;

    gLogIOstream <<
      time;

    gIndenter--;

    gLogIOstream <<
      "to segment " << asString () <<
      ", in voice \"" <<
      fSegmentVoiceUplink->getVoiceName () <<
      "\"" <<
      endl;
  }
#endif
      
  // sanity check
  msrAssert (
    fSegmentMeasuresList.size () > 0,
    "fSegmentMeasuresList is empty");

  gIndenter++;
  
  // append time to segments's current measure
  fSegmentMeasuresList.back ()->
    appendTimeToMeasure (time);

  gIndenter--;
}

void msrSegment::appendTimeToSegmentClone (S_msrTime time)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceTimes || gTraceOptions->fTraceSegments) {
    gLogIOstream <<
      "Appending time:" <<
      endl;

    gIndenter++;

    gLogIOstream <<
      time;

    gIndenter--;

    gLogIOstream <<
      "to segment clone " << asString () <<
      ", in voice \"" <<
      fSegmentVoiceUplink->getVoiceName () <<
      "\"" <<
      endl;
  }
#endif

  gIndenter++;
  
  // sanity check
  msrAssert (
    fSegmentMeasuresList.size () > 0,
    "fSegmentMeasuresList is empty");
    
  // append time to segments's current measure
  fSegmentMeasuresList.back ()->
    appendTimeToMeasureClone (time);

  gIndenter--;
}

void msrSegment::appendHarmonyToSegment (S_msrHarmony harmony)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceHarmonies || gTraceOptions->fTraceSegments) {
    gLogIOstream <<
      "Appending harmony " << harmony->asString () <<
      " to segment " << asString () <<
      "' in voice \"" <<
      fSegmentVoiceUplink->getVoiceName () <<
      "\"" <<
      endl;
  }
#endif

  gIndenter++;
  
  // sanity check
  msrAssert (
    fSegmentMeasuresList.size () > 0,
    "fSegmentMeasuresList is empty");
    
  // append it to this segment
  fSegmentMeasuresList.back ()-> // JMI ???
    appendHarmonyToMeasure (harmony);

  gIndenter--;
}

void msrSegment::appendHarmonyToSegmentClone (S_msrHarmony harmony)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceHarmonies || gTraceOptions->fTraceSegments) {
    gLogIOstream <<
      "Appending harmony " << harmony->asString () <<
      " to segment clone " << asString () <<
      "' in voice clone \"" <<
      fSegmentVoiceUplink->getVoiceName () <<
      "\"" <<
      endl;
  }
#endif
      
  // sanity check
  msrAssert (
    fSegmentMeasuresList.size () > 0,
    "fSegmentMeasuresList is empty");

  gIndenter++;
  
  // append it to this segment
  fSegmentMeasuresList.back ()->
    appendHarmonyToMeasureClone (harmony);

  gIndenter--;
}

void msrSegment::appendFiguredBassToSegment (
  S_msrFiguredBass figuredBass)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceFiguredBass || gTraceOptions->fTraceSegments) {
    gLogIOstream <<
      "Appending figured bass " << figuredBass->asString () <<
      " to segment " << asString () <<
      "' in voice \"" <<
      fSegmentVoiceUplink->getVoiceName () <<
      "\"" <<
      endl;
  }
#endif
      
  // sanity check
  msrAssert (
    fSegmentMeasuresList.size () > 0,
    "fSegmentMeasuresList is empty");

  gIndenter++;
  
  // append it to this segment
  fSegmentMeasuresList.back ()-> // JMI ???
    appendFiguredBassToMeasure (figuredBass);

  gIndenter--;
}

void msrSegment::appendFiguredBassToSegmentClone (
  S_msrFiguredBass figuredBass)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceFiguredBass || gTraceOptions->fTraceSegments) {
    gLogIOstream <<
      "Appending figured bass " << figuredBass->asString () <<
      " to segment clone " << asString () <<
      "' in voice clone \"" <<
      fSegmentVoiceUplink->getVoiceName () <<
      "\"" <<
      endl;
  }
#endif
      
  // sanity check
  msrAssert (
    fSegmentMeasuresList.size () > 0,
    "fSegmentMeasuresList is empty");

  gIndenter++;
  
  // append it to this segment
  fSegmentMeasuresList.back ()->
    appendFiguredBassToMeasure (figuredBass);

  gIndenter--;
}

void msrSegment::appendSegnoToSegment (S_msrSegno segno)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceHarmonies || gTraceOptions->fTraceSegments) {
    gLogIOstream <<
      "Appending segno " <<
      " to segment " << asString () <<
      "' in voice \"" <<
      fSegmentVoiceUplink->getVoiceName () <<
      "\"" <<
      endl;
  }
#endif
      
  // sanity check
  msrAssert (
    fSegmentMeasuresList.size () > 0,
    "fSegmentMeasuresList is empty");

  gIndenter++;
  
  // append it to this segment
  fSegmentMeasuresList.back ()->
    appendSegnoToMeasure (segno);

  gIndenter--;
}

void msrSegment::appendCodaToSegment (S_msrCoda coda)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceHarmonies || gTraceOptions->fTraceSegments) {
    gLogIOstream <<
      "Appending coda " <<
      " to segment " << asString () <<
      "' in voice \"" <<
      fSegmentVoiceUplink->getVoiceName () <<
      "\"" <<
      endl;
  }
#endif
      
  // sanity check
  msrAssert (
    fSegmentMeasuresList.size () > 0,
    "fSegmentMeasuresList is empty");

  gIndenter++;
  
  // append it to this segment
  fSegmentMeasuresList.back ()->
    appendCodaToMeasure (coda);

  gIndenter--;
}

void msrSegment::appendEyeGlassesToSegment (
  S_msrEyeGlasses eyeGlasses)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceHarmonies || gTraceOptions->fTraceSegments) {
    gLogIOstream <<
      "Appending eyeGlasses " <<
      " to segment " << asString () <<
      "' in voice \"" <<
      fSegmentVoiceUplink->getVoiceName () <<
      "\"" <<
      endl;
  }
#endif
      
  // sanity check
  msrAssert (
    fSegmentMeasuresList.size () > 0,
    "fSegmentMeasuresList is empty");

  gIndenter++;
  
  // append it to this segment
  fSegmentMeasuresList.back ()->
    appendEyeGlassesToMeasure (eyeGlasses);

  gIndenter--;
}

void msrSegment::appendPedalToSegment (S_msrPedal pedal)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceHarmonies || gTraceOptions->fTraceSegments) {
    gLogIOstream <<
      "Appending pedal " <<
      " to segment " << asString () <<
      "' in voice \"" <<
      fSegmentVoiceUplink->getVoiceName () <<
      "\"" <<
      endl;
  }
      
#endif
  // sanity check
  msrAssert (
    fSegmentMeasuresList.size () > 0,
    "fSegmentMeasuresList is empty");

  gIndenter++;
  
  // append it to this segment
  fSegmentMeasuresList.back ()->
    appendPedalToMeasure (pedal);

  gIndenter--;
}

void msrSegment::appendDampToSegment (S_msrDamp damp)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceHarmonies || gTraceOptions->fTraceSegments) {
    gLogIOstream <<
      "Appending damp " <<
      " to segment " << asString () <<
      "' in voice \"" <<
      fSegmentVoiceUplink->getVoiceName () <<
      "\"" <<
      endl;
  }
#endif
      
  // sanity check
  msrAssert (
    fSegmentMeasuresList.size () > 0,
    "fSegmentMeasuresList is empty");

  gIndenter++;
  
  // append it to this segment
  fSegmentMeasuresList.back ()->
    appendDampToMeasure (damp);

  gIndenter--;
}

void msrSegment::appendDampAllToSegment (S_msrDampAll dampAll)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceHarmonies || gTraceOptions->fTraceSegments) {
    gLogIOstream <<
      "Appending damp all " <<
      " to segment " << asString () <<
      "' in voice \"" <<
      fSegmentVoiceUplink->getVoiceName () <<
      "\"" <<
      endl;
  }
#endif
      
  // sanity check
  msrAssert (
    fSegmentMeasuresList.size () > 0,
    "fSegmentMeasuresList is empty");

  gIndenter++;
  
  // append it to this segment
  fSegmentMeasuresList.back ()->
    appendDampAllToMeasure (dampAll);

  gIndenter--;
}

void msrSegment::appendTransposeToSegment (
  S_msrTranspose transpose)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceTranspositions || gTraceOptions->fTraceSegments) {
    gLogIOstream <<
      "Appending transpose " <<
      " to segment " << asString () <<
      "' in voice \"" <<
      fSegmentVoiceUplink->getVoiceName () <<
      "\"" <<
      endl;
  }
#endif
      
  // sanity check
  msrAssert (
    fSegmentMeasuresList.size () > 0,
    "fSegmentMeasuresList is empty");

  gIndenter++;
  
  // append it to this segment
  fSegmentMeasuresList.back ()->
    appendTransposeToMeasure (transpose);

  gIndenter--;
}

void msrSegment::appendPartNameDisplayToSegment (
  S_msrPartNameDisplay partNameDisplay)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceTranspositions || gTraceOptions->fTraceSegments) {
    gLogIOstream <<
      "Appending part name display " <<
      " to segment " << asString () <<
      "' in voice \"" <<
      fSegmentVoiceUplink->getVoiceName () <<
      "\"" <<
      endl;
  }
#endif
      
  // sanity check
  msrAssert (
    fSegmentMeasuresList.size () > 0,
    "fSegmentMeasuresList is empty");

  gIndenter++;
  
  // append it to this segment
  fSegmentMeasuresList.back ()->
    appendPartNameDisplayToMeasure (partNameDisplay);

  gIndenter--;
}

void msrSegment::appendPartAbbreviationDisplayToSegment (
  S_msrPartAbbreviationDisplay partAbbreviationDisplay)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceTranspositions || gTraceOptions->fTraceSegments) {
    gLogIOstream <<
      "Appending part abbreviation display " <<
      
      " to segment " << asString () <<
      "' in voice \"" <<
      fSegmentVoiceUplink->getVoiceName () <<
      "\"" <<
      endl;
  }
#endif
      
  // sanity check
  msrAssert (
    fSegmentMeasuresList.size () > 0,
    "fSegmentMeasuresList is empty");

  gIndenter++;
  
  // append it to this segment
  fSegmentMeasuresList.back ()->
    appendPartAbbreviationDisplayToMeasure (
      partAbbreviationDisplay);

  gIndenter--;
}

void msrSegment::appendStaffDetailsToSegment (
  S_msrStaffDetails staffDetails)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceHarmonies || gTraceOptions->fTraceSegments) {
    gLogIOstream <<
      "Appending staff details " <<
      " to segment " << asString () <<
      "' in voice \"" <<
      fSegmentVoiceUplink->getVoiceName () <<
      "\"" <<
      endl;
  }
#endif

  gIndenter++;
  
  // sanity check
  if (fSegmentMeasuresList.size () == 0) { // JMI
    stringstream s;

    s <<
      "fSegmentMeasuresList is empty" <<
      " while appending staff details:" <<
      endl <<
      staffDetails <<
      "segment contains:";

    this->print (s);
    
    msrAssert (
      fSegmentMeasuresList.size () > 0,
      s.str ());
  }
  
  msrAssert (
    fSegmentMeasuresList.size () > 0,
    "fSegmentMeasuresList is empty");
    
  // append it to this segment
  fSegmentMeasuresList.back ()->
    appendStaffDetailsToMeasure (staffDetails);

  gIndenter--;
}

void msrSegment::appendLineBreakToSegment (S_msrLineBreak lineBreak)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceHarmonies || gTraceOptions->fTraceSegments) {
    gLogIOstream <<
      "Appending break " <<
      " to segment " << asString () <<
      "' in voice \"" <<
      fSegmentVoiceUplink->getVoiceName () <<
      "\"" <<
      endl;
  }
#endif
      
  // sanity check
  msrAssert (
    fSegmentMeasuresList.size () > 0,
    "fSegmentMeasuresList is empty");

  gIndenter++;
  
  // append it to this segment
  fSegmentMeasuresList.back ()->
    appendLineBreakToMeasure (lineBreak);

  gIndenter--;
}

void msrSegment::appendPageBreakToSegment (S_msrPageBreak pageBreak)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceHarmonies || gTraceOptions->fTraceSegments) {
    gLogIOstream <<
      "Appending break " <<
      " to segment " << asString () <<
      "' in voice \"" <<
      fSegmentVoiceUplink->getVoiceName () <<
      "\"" <<
      endl;
  }
#endif
      
  // sanity check
  msrAssert (
    fSegmentMeasuresList.size () > 0,
    "fSegmentMeasuresList is empty");

  gIndenter++;
  
  // append it to this segment
  fSegmentMeasuresList.back ()->
    appendPageBreakToMeasure (pageBreak);

  gIndenter--;
}

void msrSegment::appendBarNumberCheckToSegment (
  S_msrBarNumberCheck barNumberCheck)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceHarmonies || gTraceOptions->fTraceSegments) {
    gLogIOstream <<
      "Appending bar number check " <<
      " to segment " << asString () <<
      "' in voice \"" <<
      fSegmentVoiceUplink->getVoiceName () <<
      "\"" <<
      endl;
  }
#endif
      
  // sanity check
  msrAssert (
    fSegmentMeasuresList.size () > 0,
    "fSegmentMeasuresList is empty");

  gIndenter++;
  
  // append it to this segment
  fSegmentMeasuresList.back ()->
    appendBarNumberCheckToMeasure (barNumberCheck);

  gIndenter--;
}

void msrSegment::appendTempoToSegment (
  S_msrTempo tempo)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceHarmonies || gTraceOptions->fTraceSegments) {
    gLogIOstream <<
      "Appending tempo " <<
      " to segment " << asString () <<
      "' in voice \"" <<
      fSegmentVoiceUplink->getVoiceName () <<
      "\"" <<
      endl;
  }
#endif
      
  // sanity check
  msrAssert (
    fSegmentMeasuresList.size () > 0,
    "fSegmentMeasuresList is empty");

  gIndenter++;
  
  // append it to this segment
  fSegmentMeasuresList.back ()->
    appendTempoToMeasure (tempo);

  gIndenter--;
}

void msrSegment::appendRehearsalToSegment (
  S_msrRehearsal rehearsal)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceHarmonies || gTraceOptions->fTraceSegments) {
    gLogIOstream <<
      "Appending rehearsal " <<
      " to segment " << asString () <<
      "' in voice \"" <<
      fSegmentVoiceUplink->getVoiceName () <<
      "\"" <<
      endl;
  }
#endif
      
  // sanity check
  msrAssert (
    fSegmentMeasuresList.size () > 0,
    "fSegmentMeasuresList is empty");

  gIndenter++;
  
  // append it to this segment
  fSegmentMeasuresList.back ()->
    appendRehearsalToMeasure (rehearsal);

  gIndenter--;
}

void msrSegment::appendOctaveShiftToSegment (
  S_msrOctaveShift octaveShift)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceHarmonies || gTraceOptions->fTraceSegments) {
    gLogIOstream <<
      "Appending octave shift '" <<
      octaveShift->octaveShiftKindAsString () <<
      "' to segment " << asString () <<
      "' in voice \"" <<
      fSegmentVoiceUplink->getVoiceName () <<
      "\"" <<
      endl;
  }
#endif
      
  // sanity check
  msrAssert (
    fSegmentMeasuresList.size () > 0,
    "fSegmentMeasuresList is empty");

  gIndenter++;
  
  // append it to this segment
  fSegmentMeasuresList.back ()->
    appendOctaveShiftToMeasure (octaveShift);

  gIndenter--;
}

void msrSegment::appendScordaturaToSegment (
  S_msrScordatura scordatura)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceHarmonies || gTraceOptions->fTraceSegments) {
    gLogIOstream <<
      "Appending scordatura '" <<
  // JMI ???    scordatura->octaveShiftKindAsString () <<
      "' to segment " << asString () <<
      "' in voice \"" <<
      fSegmentVoiceUplink->getVoiceName () <<
      "\"" <<
      endl;
  }
#endif
      
  // sanity check
  msrAssert (
    fSegmentMeasuresList.size () > 0,
    "fSegmentMeasuresList is empty");

  gIndenter++;
  
  // append it to this segment
  fSegmentMeasuresList.back ()->
    appendScordaturaToMeasure (scordatura);

  gIndenter--;
}

void msrSegment::appendAccordionRegistrationToSegment (
  S_msrAccordionRegistration
    accordionRegistration)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTracePasses || gTraceOptions->fTraceSegments) {
    gLogIOstream <<
      "Appending accordion registration '" <<
      accordionRegistration->asString () <<
      "' to segment " << asString () <<
      "' in voice \"" <<
      fSegmentVoiceUplink->getVoiceName () <<
      "\"" <<
      endl;
  }
#endif
      
  // sanity check
  msrAssert (
    fSegmentMeasuresList.size () > 0,
    "fSegmentMeasuresList is empty");

  gIndenter++;
  
  // append it to this segment
  fSegmentMeasuresList.back ()->
    appendAccordionRegistrationToMeasure (
      accordionRegistration);

  gIndenter--;
}

void msrSegment::appendHarpPedalsTuningToSegment (
  S_msrHarpPedalsTuning
    harpPedalsTuning)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTracePasses || gTraceOptions->fTraceSegments) {
    gLogIOstream <<
      "Appending staff pedals tuning '" <<
      harpPedalsTuning->asString () <<
      "' to segment " << asString () <<
      "' in voice \"" <<
      fSegmentVoiceUplink->getVoiceName () <<
      "\"" <<
      endl;
  }
#endif
      
  // sanity check
  msrAssert (
    fSegmentMeasuresList.size () > 0,
    "fSegmentMeasuresList is empty");

  gIndenter++;
  
  // append it to this segment
  fSegmentMeasuresList.back ()->
    appendHarpPedalsTuningToMeasure (
      harpPedalsTuning);

  gIndenter--;
}

void msrSegment::padUpToMeasureLengthInSegment (
  int      inputLineNumber,
  rational measureLength)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceSegments || gTraceOptions->fTraceMeasures) {
    gLogIOstream <<
      "Padding up to measure length '" << measureLength <<
      "' in segment '" <<
      fSegmentAbsoluteNumber <<
      "' in voice \"" <<
      fSegmentVoiceUplink->getVoiceName () <<
      "\", line " << inputLineNumber <<
      endl;
  }
#endif

  if (false && fSegmentMeasuresList.size () == 0) { // JMI
    stringstream s;

    s <<
      "SegmentMeasuresList is empty"  <<
      " in segment '" <<
      fSegmentAbsoluteNumber <<
      "' in voice \"" <<
      fSegmentVoiceUplink->getVoiceName () <<
      "\"";

    gLogIOstream <<
      "SegmentVoiceUplink:" <<
      endl;
    gIndenter++;
    gLogIOstream <<
      fSegmentVoiceUplink <<
      endl;
    gIndenter--;
    
    gLogIOstream <<
      "Part:" <<
      endl;
    gIndenter++;
    gLogIOstream <<
      fSegmentVoiceUplink->fetchVoicePartUplink () <<
      endl;
    gIndenter--;
    
    msrInternalError (
      gXml2lyOptions->fInputSourceName,
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  }

  if (fSegmentMeasuresList.size ()) { // JMI BOFBOF
    // bring last measure to this length
    fSegmentMeasuresList.back ()->
      padUpToMeasureLengthInMeasure (
        inputLineNumber, measureLength);
  }
}

void msrSegment::appendMeasureToSegment (S_msrMeasure measure)
{
  int inputLineNumber =
    measure->getInputLineNumber ();
    
  string measureNumber =
    measure->getMeasureNumber ();
  
  string currentMeasureNumber =
    fSegmentMeasuresList.size () == 0
      ? ""
      : fSegmentMeasuresList.back ()->getMeasureNumber ();
    
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceMeasures || gTraceOptions->fTraceSegments) {
    gLogIOstream <<
      "Appending measure '" << measureNumber <<
      "' to segment " << asString ();

    if (fSegmentMeasuresList.size () == 0)
      gLogIOstream <<
        ", as first measure FII"; // JMI
    else
      gLogIOstream <<
      ", after measure number '" << currentMeasureNumber << "'";

    gLogIOstream <<
      "' in voice \"" <<
      fSegmentVoiceUplink->getVoiceName () <<
      "\"" <<
      ", line " << measure->getInputLineNumber () <<
      endl;
  }
#endif

  if (measureNumber == currentMeasureNumber) {
    stringstream s;

    s <<
      "measure number '" << measureNumber <<
      "' occurs twice in a row";

    // JMI  msrInternalError (
    msrInternalWarning (
      gXml2lyOptions->fInputSourceName,
      inputLineNumber,
      s.str ());
  }

  else { // JMI TEMP
    // append measure to the segment
    fSegmentMeasuresList.push_back (measure);
  }
}

void msrSegment::prependMeasureToSegment (S_msrMeasure measure)
{
  int inputLineNumber =
    measure->getInputLineNumber ();
    
  string measureNumber =
    measure->getMeasureNumber ();
  
  string currentMeasureNumber =
    fSegmentMeasuresList.size () == 0
      ? ""
      : fSegmentMeasuresList.back ()->getMeasureNumber ();
    
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceMeasures || gTraceOptions->fTraceSegments) {
    gLogIOstream <<
      "Prepending measure " << measureNumber <<
      " to segment " << asString ();

    if (fSegmentMeasuresList.size () == 0) {
      gLogIOstream <<
        ", as first measure FOO"; // JMI
    }
    
/* JMI
    else
      gLogIOstream <<
      ", after measure number '" << currentMeasureNumber << "'";
*/

    gLogIOstream <<
      " in voice \"" <<
      fSegmentVoiceUplink->getVoiceName () <<
      "\"," <<
      ", line " << measure->getInputLineNumber () <<
      endl;
  }
#endif

  if (measureNumber == currentMeasureNumber) {
    stringstream s;

    s <<
      "measure number '" << measureNumber <<
      "' occurs twice in a row";

  // JMI  msrInternalError (
    msrInternalWarning (
      gXml2lyOptions->fInputSourceName,
      inputLineNumber,
      s.str ());
  }

  else { // JMI TEMP
    // append measure to the segment
    fSegmentMeasuresList.push_back (measure); // push_front ??? JMI
  }
}

void msrSegment::appendMeasureToSegmentIfNotYetDone ( // JMI
  int    inputLineNumber,
  string measureNumber)
{
  /* JMI BOF
  if (! fSegmentMeasuresList.size ()) {
    // create a new measure
#ifdef TRACE_OPTIONS
    if (gTraceOptions->fTraceMeasures || gTraceOptions->fTraceSegments) {
      gLogIOstream <<
        "Appending a new measure '" << measureNumber <<
        "' to segment '" << asString () << "'" <<
        "' in voice \"" <<
        fSegmentVoiceUplink->getVoiceName () <<
        "\"," <<
        ", line " << inputLineNumber <<
        endl;
    }
#endif

    S_msrMeasure
      newMeasure =
        msrMeasure::create (
          inputLineNumber,
          fSegmentPartUplink,
          measureNumber,
          this);

    // append it to the segment
    appendMeasureToSegment (
      newMeasure);
  }
  */
}

void msrSegment::prependBarlineToSegment (S_msrBarline barline)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceMeasures || gTraceOptions->fTraceSegments) {
    gLogIOstream <<
      "Prepending barline '" << barline->asString () <<
      "' to segment '" << asString () << "'" <<
      "' in voice \"" <<
      fSegmentVoiceUplink->getVoiceName () <<
      "\"," <<
      endl;
  }
#endif

  // sanity check
  msrAssert (
    fSegmentMeasuresList.size () > 0,
    "fSegmentMeasuresList is empty");

  // prepend barline to this segment
  gIndenter++;
  
  fSegmentMeasuresList.front ()->
    prependBarlineToMeasure (barline);

  gIndenter--;
}

void msrSegment::appendBarlineToSegment (S_msrBarline barline)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceMeasures || gTraceOptions->fTraceSegments) {
    gLogIOstream <<
      "Appending barline '" << barline->asString () <<
      "' to segment '" << asString () << "'" <<
      "' in voice \"" <<
      fSegmentVoiceUplink->getVoiceName () <<
      "\"," <<
      endl;
  }
#endif

  if (! fSegmentMeasuresList.size ()) { // JMI
#ifdef TRACE_OPTIONS
    if (
      gTraceOptions->fTraceBarlinesDetails
        ||
      gTraceOptions->fTraceSegmentsDetails) { // JMI
      gLogIOstream <<
        endl <<
        "*********>> Current voice \"" <<
        fSegmentVoiceUplink->getVoiceName () <<
        "\"" <<
        ", line " << barline->getInputLineNumber () <<
        " contains:" <<
        endl <<
        fSegmentVoiceUplink <<
        "<<*********" <<
        endl <<
        endl;
    }
#endif
  }
  
  // sanity check
  msrAssert (
    fSegmentMeasuresList.size () > 0,
    "fSegmentMeasuresList is empty");

  gIndenter++;
  
  // append barline to this segment
  fSegmentMeasuresList.back ()->
    appendBarlineToMeasure (barline);

  gIndenter--;
}

void msrSegment::appendBarCheckToSegment (S_msrBarCheck barCheck)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceMeasures || gTraceOptions->fTraceSegments) {
    gLogIOstream <<
      "Appending bar check " << barCheck->asString () <<
      " to segment '" << asString () << "'" <<
      "' in voice \"" <<
      fSegmentVoiceUplink->getVoiceName () <<
      "\"," <<
      endl;
  }
#endif
      
  // sanity check
  msrAssert (
    fSegmentMeasuresList.size () > 0,
    "fSegmentMeasuresList is empty");
    
  fSegmentMeasuresList.back ()->
    appendBarCheckToMeasure (barCheck);
}

void msrSegment::appendVoiceStaffChangeToSegment (
  S_msrVoiceStaffChange voiceStaffChange)
{
#ifdef TRACE_OPTIONS
  if (
    gTraceOptions->fTraceVoices
      ||
    gTraceOptions->fTraceStaves
      ||
    gTraceOptions->fTraceSegments) {
    gLogIOstream <<
      "Appending voice staff change '" <<
      voiceStaffChange->asString () <<
      "' to segment \"" << asString () << "\"" <<
      ", line " << voiceStaffChange->getInputLineNumber () <<
      endl;
  }
#endif
  
  // sanity check
  msrAssert (
    fSegmentMeasuresList.size () > 0,
    "fSegmentMeasuresList is empty");
    
  fSegmentMeasuresList.back ()->
    appendVoiceStaffChangeToMeasure (
      voiceStaffChange);
}

void msrSegment::appendNoteToSegment (S_msrNote note)
{
  appendMeasureToSegmentIfNotYetDone (
    note->getInputLineNumber (),
    fSegmentMeasureNumber);


  if (! fSegmentMeasuresList.size ()) { // JMI
    gLogIOstream <<
      endl <<
      endl <<
      endl <<
      "+++++++++++++++++ appendNoteToSegment '" <<
      fSegmentAbsoluteNumber <<
      /* JMI
      "', score:" <<
      endl <<
      fSegmentVoiceUplink->
        getVoiceStaffUplink ()->
          getStaffPartUplink ()->
            getPartPartGroupUplink ()->
              getPartGroupScoreUplink () <<
              */
      "', voice:" <<
      endl <<
      fSegmentVoiceUplink <<
      endl <<
      endl <<
      endl;
  }

  // sanity check
  msrAssert (
    fSegmentMeasuresList.size () > 0,
    "fSegmentMeasuresList is empty");
    
  fSegmentMeasuresList.back ()->
    appendNoteToMeasure (note);
}

void msrSegment::appendNoteToSegmentClone (S_msrNote note)
{
  fSegmentMeasuresList.back ()->
    appendNoteToMeasureClone (note);
}

void msrSegment::appendDoubleTremoloToSegment ( // JMI
  S_msrDoubleTremolo doubleTremolo)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceTremolos || gTraceOptions->fTraceSegments) {
    gLogIOstream <<
      "Appending double tremolo " <<
      doubleTremolo->asShortString () <<
      " to segment '" << asString () << "'" <<
      "' in voice \"" <<
      fSegmentVoiceUplink->getVoiceName () <<
      "\"," <<
      endl;
  }
#endif
      
  // sanity check
  msrAssert (
    fSegmentMeasuresList.size () > 0,
    "fSegmentMeasuresList is empty");
    
  fSegmentMeasuresList.back ()->
    appendDoubleTremoloToMeasure (doubleTremolo);
}

void msrSegment::appendMeasuresRepeatToSegment (
  S_msrMeasuresRepeat measuresRepeat)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceRepeats || gTraceOptions->fTraceSegments) {
    gLogIOstream <<
      "Appending measure repeat '" <<
      measuresRepeat->asString () <<
      "' to segment '" << asString () << "'" <<
      "' in voice \"" <<
      fSegmentVoiceUplink->getVoiceName () <<
      "\"," <<
      endl;
  }
#endif
      
  // sanity check
  msrAssert (
    fSegmentMeasuresList.size () > 0,
    "fSegmentMeasuresList is empty");
    
  fSegmentMeasuresList.back ()->
    appendMeasuresRepeatToMeasure (measuresRepeat);
}

void msrSegment::appendMultipleRestToSegment (
  S_msrMultipleRest multipleRest)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceRepeats || gTraceOptions->fTraceSegments) {
    gLogIOstream <<
      "Appending multiple rest '" <<
      multipleRest->asString () <<
      "' to segment '" << asString () << "'" <<
      "' in voice \"" <<
      fSegmentVoiceUplink->getVoiceName () <<
      "\"," <<
      endl;
  }
#endif
      
  // sanity check
  msrAssert (
    fSegmentMeasuresList.size () > 0,
    "fSegmentMeasuresList is empty");
    
  fSegmentMeasuresList.back ()->
    appendMultipleRestToMeasure (multipleRest);
}

void msrSegment::appendChordToSegment (S_msrChord chord) // JMI
{
  // sanity check
  msrAssert (
    fSegmentMeasuresList.size () > 0,
    "fSegmentMeasuresList is empty");
    
  fSegmentMeasuresList.back ()->
    appendChordToMeasure (chord);
}

void msrSegment::appendTupletToSegment (S_msrTuplet tuplet) // JMI
{
  // sanity check
  msrAssert (
    fSegmentMeasuresList.size () > 0,
    "fSegmentMeasuresList is empty");
    
  fSegmentMeasuresList.back ()->
    appendTupletToMeasure (tuplet);
}

void msrSegment::appendGraceNotesToSegment (
  S_msrGraceNotes graceNotes)
{
  // sanity check
  msrAssert (
    fSegmentMeasuresList.size () > 0,
    "fSegmentMeasuresList is empty");
    
  fSegmentMeasuresList.back ()->
    appendGraceNotesToMeasure (graceNotes);
}

void msrSegment::prependGraceNotesToSegment (
  S_msrGraceNotes graceNotes)

{
  // sanity check
  msrAssert (
    fSegmentMeasuresList.size () > 0,
    "fSegmentMeasuresList is empty");
    
  fSegmentMeasuresList.front ()->
    prependGraceNotesToMeasure (graceNotes); // JMI
}

void msrSegment::appendAfterGraceNotesToSegment (
  S_msrAfterGraceNotes afterGraceNotes)
{
  // sanity check
  msrAssert (
    fSegmentMeasuresList.size () > 0,
    "fSegmentMeasuresList is empty");
    
  fSegmentMeasuresList.back ()->
    appendAfterGraceNotesToMeasure (afterGraceNotes);
}

void msrSegment::prependAfterGraceNotesToSegment (
  S_msrAfterGraceNotes afterGraceNotes)

{
  // sanity check
  msrAssert (
    fSegmentMeasuresList.size () > 0,
    "fSegmentMeasuresList is empty");
    
  fSegmentMeasuresList.front ()->
    prependAfterGraceNotesToMeasure (afterGraceNotes); // JMI
}

void msrSegment::prependOtherElementToSegment (S_msrElement elem)
{
  appendMeasureToSegmentIfNotYetDone ( // JMI
    elem->getInputLineNumber (),
    fSegmentMeasureNumber); // +1??? JMI
  
  // sanity check
  msrAssert (
    fSegmentMeasuresList.size () > 0,
    "fSegmentMeasuresList is empty");
    
  fSegmentMeasuresList.back ()->
    prependOtherElementToMeasure (elem);
}

void msrSegment::appendOtherElementToSegment (S_msrElement elem)
{
  appendMeasureToSegmentIfNotYetDone ( // JMI
    elem->getInputLineNumber (),
    fSegmentMeasureNumber); // +1??? JMI
  
  // sanity check
  msrAssert (
    fSegmentMeasuresList.size () > 0,
    "fSegmentMeasuresList is empty");
    
  fSegmentMeasuresList.back ()->
    appendOtherElementToMeasure (elem);
}

/*
S_msrElement msrSegment::removeLastElementFromSegment (
  int inputLineNumber)
{
  // this last element can be a note or a tuplet,
  // this method is used when the seconde note of a chord is mest
  
  if (fSegmentMeasuresList.size ()) {
    return
      fSegmentMeasuresList.back ()->
        removeLastElementFromMeasure (
          inputLineNumber);
  }
  
  else {
    msrInternalError (
      inputLineNumber,
      "cannot removeLastElementFromSegment () " <<
      asString () <<
      " since it is empty");
  }
}
*/

void msrSegment::removeNoteFromSegment (
  int       inputLineNumber,
  S_msrNote note)
{  
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceNotes || gTraceOptions->fTraceSegments) {
    gLogIOstream <<
      "Removing note '" <<
      note->asString () <<
      "'from segment '" <<
      asString () <<
      "'" <<
      endl;
  }
#endif

  gIndenter++;
  
  if (fSegmentMeasuresList.size ()) {
    fSegmentMeasuresList.back ()->
      removeNoteFromMeasure (
        inputLineNumber,
        note);
  }
  
  else {
    stringstream s;

    s <<
      "cannot remove note from segment " <<
      asString () <<
      "' in voice \"" <<
      fSegmentVoiceUplink->getVoiceName () <<
      "\"," <<
      " since it is empty";

    msrInternalError (
      gXml2lyOptions->fInputSourceName,
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  }

  gIndenter--;
}

void msrSegment::removeElementFromSegment (
  int          inputLineNumber,
  S_msrElement element)
{  
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceSegments) {
    gLogIOstream <<
      "Removing element '" <<
      element->asString () <<
      "'from segment '" <<
      asString () <<
      "'" <<
      endl;
  }
#endif

 if (fSegmentMeasuresList.size ()) {
    fSegmentMeasuresList.back ()->
      removeElementFromMeasure (
        inputLineNumber,
        element);
  }
  
  else {
    stringstream s;

    s <<
      "cannot remove note from segment " <<
      asString () <<
      "' in voice \"" <<
      fSegmentVoiceUplink->getVoiceName () <<
      "\"," <<
      " since it is empty";

    msrInternalError (
      gXml2lyOptions->fInputSourceName,
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  }
}

S_msrMeasure msrSegment::removeLastMeasureFromSegment (
  int inputLineNumber)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceMeasures || gTraceOptions->fTraceSegments) {
    gLogIOstream <<
      "Removing last measure from segment '" <<
      fSegmentAbsoluteNumber <<
      "', line " << inputLineNumber <<
      endl;
  }
#endif

  if (! fSegmentMeasuresList.size ()) {
    stringstream s;

    s <<
      "cannot remove last measure from segment '" <<
      fSegmentAbsoluteNumber <<
      "' in voice \"" <<
      fSegmentVoiceUplink->getVoiceName () <<
      "\"," <<
      "' since it is empty";

    msrInternalError (
      gXml2lyOptions->fInputSourceName,
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  }

  S_msrMeasure
    result =
      fSegmentMeasuresList.back ();

#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceMeasures || gTraceOptions->fTraceSegments) {
    gLogIOstream <<
      endl <<
      "This measure contains:" <<
      endl;

    gIndenter++;

    gLogIOstream <<
      result <<
      endl;

    gIndenter--;
  }
#endif

  if (false) { // JMI
    gLogIOstream <<
      endl <<
      endl <<
      endl <<
      "+++++++++++++++++ removeLastMeasureFromSegment '" <<
      fSegmentAbsoluteNumber <<
      /* JMI
      "', score:" <<
      endl <<
      fSegmentVoiceUplink->
        getVoiceStaffUplink ()->
          getStaffPartUplink ()->
            getPartPartGroupUplink ()->
              getPartGroupScoreUplink () <<
              */
      "', voice:" <<
      endl <<
      fSegmentVoiceUplink <<
      endl <<
      endl <<
      ", line " << inputLineNumber <<
      endl;

    abort ();
  }

  fSegmentMeasuresList.pop_back ();
  
  return result;
}

void msrSegment::acceptIn (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrSegment::acceptIn ()" <<
      endl;
  }
      
  if (visitor<S_msrSegment>*
    p =
      dynamic_cast<visitor<S_msrSegment>*> (v)) {
        S_msrSegment elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrSegment::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrSegment::acceptOut (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrSegment::acceptOut ()" <<
      endl;
  }

  if (visitor<S_msrSegment>*
    p =
      dynamic_cast<visitor<S_msrSegment>*> (v)) {
        S_msrSegment elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrSegment::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrSegment::browseData (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrSegment::browseData ()" <<
      endl;
  }

/* JMI
  gLogIOstream <<
    gTab <<
    "==========>>> " <<
    fSegmentMeasuresList.size () << " measures" << endl;
    */
    
  for (
    list<S_msrMeasure>::const_iterator i = fSegmentMeasuresList.begin ();
    i != fSegmentMeasuresList.end ();
    i++) {
    // browse the element
    msrBrowser<msrMeasure> browser (v);
    browser.browse (*(*i));
  } // for

  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% <== msrSegment::browseData ()" <<
      endl;
  }
}

string msrSegment::asString () const
{
  stringstream s;

  s <<
    "Segment '" <<
    fSegmentAbsoluteNumber <<
    "' in voice \"" <<
    fSegmentVoiceUplink->getVoiceName () <<
    "\"";
  
  if (! fSegmentMeasuresList.size ()) {
    s <<
      " (0 measures)";
  }
  else {
    s <<
      " (" <<
      singularOrPlural (
        fSegmentMeasuresList.size (), "measure", " measures") <<
      ")";
  }

  return s.str ();
}

string msrSegment::asShortString () const
{
  stringstream s;

  s <<
 // JMI   "Segment " <<
    "'" << fSegmentAbsoluteNumber <<
 // JMI   " in voice \"" <<
    "' in \"" <<
    fSegmentVoiceUplink->getVoiceName () <<
    "\"";

  return s.str ();
}

ostream& operator<< (ostream& os, const S_msrSegment& elt)
{
  elt->print (os);
  return os;
}

void msrSegment::print (ostream& os)
{  
  os <<
    "Segment '" <<
    fSegmentAbsoluteNumber <<
    "', " <<
    singularOrPlural (
      fSegmentMeasuresList.size (), "measure", "measures") <<
    endl;

  gIndenter++;

  const int fieldWidth = 20;
  
  os <<
    setw (fieldWidth) << "voiceUplink" << " : " <<
    "\"" <<
    fSegmentVoiceUplink->getVoiceName () <<
    "\"" <<
    endl;

  if (! fSegmentMeasuresList.size ()) {
    os <<
      setw (fieldWidth) << "measures" << " : " << "none" <<
      endl;
  }
  
  else {        
    list<S_msrMeasure>::const_iterator
      iBegin = fSegmentMeasuresList.begin (),
      iEnd   = fSegmentMeasuresList.end (),
      i      = iBegin;
      
    for ( ; ; ) {
      os << (*i);
      if (++i == iEnd) break;
 // JMI     os << endl;
    } // for
  }
    
  gIndenter--;
}

//______________________________________________________________________________
S_msrRepeatCommonPart msrRepeatCommonPart::create (
  int                 inputLineNumber,
  S_msrRepeat         repeatUplink)
{
  msrRepeatCommonPart* o =
    new msrRepeatCommonPart (
      inputLineNumber,
      repeatUplink);
  assert(o!=0);
  return o;
}

msrRepeatCommonPart::msrRepeatCommonPart (
  int                 inputLineNumber,
  S_msrRepeat         repeatUplink)
    : msrElement (inputLineNumber)
{
  // sanity check
  msrAssert (
    repeatUplink != nullptr,
    "repeatUplink is null");
    
  fRepeatCommonPartRepeatUplink = repeatUplink;
}

msrRepeatCommonPart::~msrRepeatCommonPart ()
{}

/* JMI
void msrRepeatCommonPart::appendElementToRepeatCommonPart (
  S_msrElement elem) // JMI ???
{
  fRepeatCommonPartSegment->
    appendOtherElementToSegment (elem);
}
*/

void msrRepeatCommonPart::acceptIn (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrRepeatCommonPart::acceptIn ()" <<
      endl;
  }
      
  if (visitor<S_msrRepeatCommonPart>*
    p =
      dynamic_cast<visitor<S_msrRepeatCommonPart>*> (v)) {
        S_msrRepeatCommonPart elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrRepeatCommonPart::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrRepeatCommonPart::acceptOut (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrRepeatCommonPart::acceptOut ()" <<
      endl;
  }

  if (visitor<S_msrRepeatCommonPart>*
    p =
      dynamic_cast<visitor<S_msrRepeatCommonPart>*> (v)) {
        S_msrRepeatCommonPart elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrRepeatCommonPart::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrRepeatCommonPart::browseData (basevisitor* v)
{
  // browse the elements
  if (fRepeatCommonPartElementsList.size ()) {
    for (
      list<S_msrElement>::const_iterator i = fRepeatCommonPartElementsList.begin ();
      i != fRepeatCommonPartElementsList.end ();
      i++) {
      // browse the element
      msrBrowser<msrElement> browser (v);
      browser.browse (*(*i));
    } // for
  }
}

ostream& operator<< (ostream& os, const S_msrRepeatCommonPart& elt)
{
  elt->print (os);
  return os;
}

void msrRepeatCommonPart::appendElementToRepeatCommonPart (
  S_msrElement elem)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceVoices) {
    gLogIOstream <<
      "Appending element '" << elem <<
      "' to repeat common part '" << asString () << "'" <<
      endl;
  }
#endif

  // sanity check
  msrAssert (
    elem != nullptr,
    "elem is null");

  fRepeatCommonPartElementsList.push_back (elem);
}

string msrRepeatCommonPart::asString () const
{
  stringstream s;

  s <<
    "RepeatCommonPart" <<
    ", repeat uplink: '" <<
    fRepeatCommonPartRepeatUplink->
      asString () <<
    "', line " << fInputLineNumber <<
    endl;

  return s.str ();
}

void msrRepeatCommonPart::print (ostream& os)
{
  os <<
    endl <<
    "RepeatCommonPart" <<
    ", line " << fInputLineNumber <<
    endl;
 
  gIndenter++;

  os <<
    "Repeat uplink: '" <<
    fRepeatCommonPartRepeatUplink->
      asString () <<
      "'" <<
    endl <<
    endl;


  // print the elements
  int elementsNumber =
    fRepeatCommonPartElementsList.size ();
    
  os <<
    "Elements: ";
  if (elementsNumber)
    os << "(" << elementsNumber << ")";
  else
    os << "none";
  os <<
    endl;
    
  if (elementsNumber) {
    gIndenter++;

    list<S_msrElement>::const_iterator
      iBegin = fRepeatCommonPartElementsList.begin (),
      iEnd   = fRepeatCommonPartElementsList.end (),
      i      = iBegin;
      
    for ( ; ; ) {
      // print the element
      os << (*i);
      if (++i == iEnd) break;
      os << endl;
    } // for

    gIndenter--;
  }
  
  gIndenter--;
}

//______________________________________________________________________________
S_msrRepeatEnding msrRepeatEnding::create (
  int                 inputLineNumber,
  string              repeatEndingNumber, // may be "1, 2"
  msrRepeatEndingKind repeatEndingKind,
  S_msrSegment        segment,
  S_msrRepeat         repeatUplink)
{
  msrRepeatEnding* o =
    new msrRepeatEnding (
      inputLineNumber,
      repeatEndingNumber,
      repeatEndingKind,
      segment,
      repeatUplink);
  assert(o!=0);
  return o;
}

msrRepeatEnding::msrRepeatEnding (
  int                 inputLineNumber,
  string              repeatEndingNumber, // may be "1, 2"
  msrRepeatEndingKind repeatEndingKind,
  S_msrSegment        segment,
  S_msrRepeat         repeatUplink)
    : msrElement (inputLineNumber)
{
  fRepeatEndingNumber = repeatEndingNumber;
  
  fRepeatEndingInternalNumber = 0;
    // will be set by msrRepeat::addRepeatEnding ()
  
  fRepeatEndingKind = repeatEndingKind;
  
  fRepeatEndingSegment = segment;
  
  fRepeatEndingRepeatUplink = repeatUplink;
}

msrRepeatEnding::~msrRepeatEnding ()
{}

/* JMI
void msrRepeatEnding::appendElementToRepeatEnding (
  S_msrElement elem) // JMI ???
{
  fRepeatEndingSegment->
    appendOtherElementToSegment (elem);
}
*/

void msrRepeatEnding::acceptIn (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrRepeatEnding::acceptIn ()" <<
      endl;
  }
      
  if (visitor<S_msrRepeatEnding>*
    p =
      dynamic_cast<visitor<S_msrRepeatEnding>*> (v)) {
        S_msrRepeatEnding elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrRepeatEnding::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrRepeatEnding::acceptOut (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrRepeatEnding::acceptOut ()" <<
      endl;
  }

  if (visitor<S_msrRepeatEnding>*
    p =
      dynamic_cast<visitor<S_msrRepeatEnding>*> (v)) {
        S_msrRepeatEnding elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrRepeatEnding::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrRepeatEnding::browseData (basevisitor* v)
{
  // browse the segment
  msrBrowser<msrSegment> browser (v);
  browser.browse (*fRepeatEndingSegment);
}

ostream& operator<< (ostream& os, const S_msrRepeatEnding& elt)
{
  elt->print (os);
  return os;
}

string msrRepeatEnding::repeatEndingKindAsString (
  msrRepeatEndingKind repeatEndingKind)
{
  string result;
  
  switch (repeatEndingKind) {
    case msrRepeatEnding::kHookedEnding:
      result = "hooked";
      break;
    case msrRepeatEnding::kHooklessEnding:
      result = "hookless";
      break;
  } // switch
  
  return result;
}

string msrRepeatEnding::asString () const
{
  stringstream s;

  s <<
    "RepeatEnding" <<
    ", " <<
    repeatEndingKindAsString (
      fRepeatEndingKind) <<
    ", repeatEndingRepeatUplink: '" <<
    fRepeatEndingRepeatUplink->
      asString () <<
    ", repeatEndingNumber: " << fRepeatEndingNumber <<
    ", repeatEndingInternalNumber: " << fRepeatEndingInternalNumber <<
    "', line " << fInputLineNumber <<
    endl;

  return s.str ();
}

void msrRepeatEnding::print (ostream& os)
{
  os <<
    endl <<
    "RepeatEnding" <<
    ", line " << fInputLineNumber <<
    endl;

  gIndenter++;

  const int fieldWidth = 27;

  os << left <<
    setw (fieldWidth) <<
    "repeatEndingKind" << " : " <<
    repeatEndingKindAsString (
      fRepeatEndingKind) <<
    endl <<
    setw (fieldWidth) <<
    "repeatEndingNumber" <<  " : " <<fRepeatEndingNumber <<
    endl <<
    setw (fieldWidth) <<
    "repeatEndingInternalNumber" <<  " : " <<fRepeatEndingInternalNumber <<
    endl <<
    setw (fieldWidth) <<
    "Repeat uplink: " << " : '" <<
    fRepeatEndingRepeatUplink->
      asString () <<
    "'" <<
    endl <<
    endl;

  os <<
    "RepeatEndingSegment:" <<
    endl;
  gIndenter++;
  os <<
    fRepeatEndingSegment;
  gIndenter--;
  
  gIndenter--;
}

//______________________________________________________________________________
S_msrRepeat msrRepeat::create (
  int        inputLineNumber,
  int        repeatTimes,
  S_msrVoice voiceUplink)
{
  msrRepeat* o =
    new msrRepeat (
      inputLineNumber,
      repeatTimes,
      voiceUplink);
  assert(o!=0);
  return o;
}

msrRepeat::msrRepeat (
  int        inputLineNumber,
  int        repeatTimes,
  S_msrVoice voiceUplink)
    : msrElement (inputLineNumber)
{
  fRepeatEndingsInternalCounter = 0;
  
  // sanity check
  msrAssert(
    voiceUplink != nullptr,
    "voiceUplink is null");
    
  fRepeatVoiceUplink = voiceUplink;

  fRepeatTimes = repeatTimes;
}

msrRepeat::~msrRepeat ()
{}

void msrRepeat::setRepeatCommonPart (
  S_msrRepeatCommonPart repeatCommonPart)
{
  // sanity check
  msrAssert(
    repeatCommonPart != nullptr,
    "repeatCommonPart is null");
    
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceRepeats) {
    gLogIOstream <<
      "Setting repeat common part containing " <<
      singularOrPlural (
        repeatCommonPart->
          getRepeatCommonPartElementsList ().size (),
        "element",
        "elements") <<
      endl;
  }
#endif
      
  fRepeatCommonPart = repeatCommonPart;
}
/*
void msrRepeat::setRepeatCommonSegment (
  S_msrSegment repeatCommonSegment)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceRepeats) {
    gLogIOstream <<
      "Setting repeat common segment containing " <<
      singularOrPlural (
        repeatCommonSegment->
          getSegmentMeasuresList ().size (),
        "measure",
        "measures") <<
      endl;
  }
#endif
      
  // sanity check
  msrAssert(
    repeatCommonSegment != nullptr,
    "repeatCommonSegment is null");
    
  fRepeatCommonSegment = repeatCommonSegment;
}
*/

void msrRepeat::addRepeatEnding (
  S_msrRepeatEnding repeatEnding)
{
  // sanity check
  msrAssert(
    repeatEnding != nullptr,
    "repeatEnding is null");
    
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceRepeats) {
    gLogIOstream <<
      "Adding ending '" <<
      repeatEnding->asString () <<
      "' to repeat" <<
      endl;
  }
#endif
      
  fRepeatEndings.push_back (repeatEnding);

  // set repeat ending internal number
  repeatEnding->
    setRepeatEndingInternalNumber (
      ++ fRepeatEndingsInternalCounter);
}

void msrRepeat::acceptIn (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrRepeat::acceptIn ()" <<
      endl;
  }
      
  if (visitor<S_msrRepeat>*
    p =
      dynamic_cast<visitor<S_msrRepeat>*> (v)) {
        S_msrRepeat elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrRepeat::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrRepeat::acceptOut (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrRepeat::acceptOut ()" <<
      endl;
  }

  if (visitor<S_msrRepeat>*
    p =
      dynamic_cast<visitor<S_msrRepeat>*> (v)) {
        S_msrRepeat elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrRepeat::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrRepeat::browseData (basevisitor* v)
{
  if (fRepeatCommonPart) {
  // browse the common part
    msrBrowser<msrRepeatCommonPart> browser (v);
    browser.browse (*fRepeatCommonPart);
  }
  
  // browse the alternatives
  for (
    vector<S_msrRepeatEnding>::const_iterator i = fRepeatEndings.begin ();
    i != fRepeatEndings.end ();
    i++) {
    // browse the alternative
    msrBrowser<msrRepeatEnding> browser (v);
    browser.browse (*(*i));
  } // for
}

ostream& operator<< (ostream& os, const S_msrRepeat& elt)
{
  elt->print (os);
  return os;
}

string msrRepeat::asString () const
{
  stringstream s;

  s <<
    "Repeat" <<
    ", " << fRepeatTimes << " times" <<
    ", " <<
    singularOrPlural (
      fRepeatEndings.size (), "repeat ending", "repeat endings") <<
    ", line " << fInputLineNumber;

  return s.str ();
}

void msrRepeat::print (ostream& os)
{
  os <<
    "Repeat" <<
    ", " << fRepeatTimes << " times" <<
    ", " <<
    singularOrPlural (
      fRepeatEndings.size (),
      "repeat ending",
      "repeat endings") <<
    ", line " << fInputLineNumber <<
    endl;
  
  gIndenter++;
  
  // print the repeat common part
  if (! fRepeatCommonPart) {
    os <<
      "Common part: none" <<
      endl;
  }
  else {    
    os <<
      fRepeatCommonPart <<
      endl;
  }
  
/* JMI
  // print the repeat common segment
  os <<
    "Common segment: ";
  if (! fRepeatCommonSegment)
    os << "none";
  os << endl;

  if (fRepeatCommonSegment) {
    gIndenter++;
    
    os <<
      fRepeatCommonSegment <<
      endl;

    gIndenter--;
  }
*/
  // print the repeat endings
  int endingsNumber =
    fRepeatEndings.size ();
    
  os <<
    "Repeat endings: ";
  if (endingsNumber)
    os << "(" << endingsNumber << ")";
  else
    os << "none";
  os << endl;
    
  if (endingsNumber) {
    gIndenter++;
    
    vector<S_msrRepeatEnding>::const_iterator
      iBegin = fRepeatEndings.begin (),
      iEnd   = fRepeatEndings.end (),
      i      = iBegin;
      
    for ( ; ; ) {
      // print the repeat ending
      os << (*i);
      if (++i == iEnd) break;
  // JMI    os << endl;
    } // for

    gIndenter--;
  }

 // JMI os << endl;
      
  gIndenter--;
}

//______________________________________________________________________________
S_msrMeasuresRepeatPattern msrMeasuresRepeatPattern::create (
  int                 inputLineNumber,
  S_msrMeasuresRepeat measuresRepeatUplink)
{
  msrMeasuresRepeatPattern* o =
    new msrMeasuresRepeatPattern (
      inputLineNumber,
      measuresRepeatUplink);
  assert(o!=0);
  return o;
}

msrMeasuresRepeatPattern::msrMeasuresRepeatPattern (
  int                 inputLineNumber,
  S_msrMeasuresRepeat measuresRepeatUplink)
    : msrElement (inputLineNumber)
{
  // sanity check
  msrAssert (
    measuresRepeatUplink != nullptr,
    "measuresRepeatUplink is null");
    
  fMeasuresRepeatUplink = measuresRepeatUplink;
}

msrMeasuresRepeatPattern::~msrMeasuresRepeatPattern ()
{}

void msrMeasuresRepeatPattern::setMeasuresRepeatPatternSegment (
    S_msrSegment measuresRepeatPatternSegment)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceRepeats) {
    gLogIOstream <<
      "Setting measure repeat pattern segment containing " <<
      singularOrPlural (
          measuresRepeatPatternMeasuresNumber (),
        "measure",
        "measures") <<
      endl;
  }
#endif
      
  // sanity check
  msrAssert (
    measuresRepeatPatternSegment != nullptr,
    "measuresRepeatPatternSegment is null");

  fMeasuresRepeatPatternSegment =
    measuresRepeatPatternSegment;
}

int msrMeasuresRepeatPattern::measuresRepeatPatternMeasuresNumber () const
{
  int result;

  if (fMeasuresRepeatPatternSegment)
    result =
      fMeasuresRepeatPatternSegment->
      getSegmentMeasuresList ().size ();
  else
    result = 0;

  return result;
}

void msrMeasuresRepeatPattern::acceptIn (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrMeasuresRepeatPattern::acceptIn ()" <<
      endl;
  }
      
  if (visitor<S_msrMeasuresRepeatPattern>*
    p =
      dynamic_cast<visitor<S_msrMeasuresRepeatPattern>*> (v)) {
        S_msrMeasuresRepeatPattern elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrMeasuresRepeatPattern::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrMeasuresRepeatPattern::acceptOut (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrMeasuresRepeatPattern::acceptOut ()" <<
      endl;
  }

  if (visitor<S_msrMeasuresRepeatPattern>*
    p =
      dynamic_cast<visitor<S_msrMeasuresRepeatPattern>*> (v)) {
        S_msrMeasuresRepeatPattern elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrMeasuresRepeatPattern::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrMeasuresRepeatPattern::browseData (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrMeasuresRepeatPattern::browseData ()" <<
      endl;
  }

  if (fMeasuresRepeatPatternSegment) {
    // browse the pattern segment
    msrBrowser<msrSegment> browser (v);
    browser.browse (*fMeasuresRepeatPatternSegment);
  }
}

ostream& operator<< (ostream& os, const S_msrMeasuresRepeatPattern& elt)
{
  elt->print (os);
  return os;
}

string msrMeasuresRepeatPattern::asString () const
{
  stringstream s;

  s <<

    "MeasuresRepeatPattern" <<
    ", line " << fInputLineNumber <<
    " (" <<
    singularOrPlural (
      measuresRepeatPatternMeasuresNumber (),
      "repeated measure",
      "repeated measures") <<
    ")"; 

  return s.str ();
}

void msrMeasuresRepeatPattern::print (ostream& os)
{
  os <<
    endl <<
    asString () <<
    endl <<
    endl;
  
  gIndenter++;
  
  // print the pattern segment
  os <<
    "Pattern segment:";

  if (! fMeasuresRepeatPatternSegment) {
    os <<
      " none" <<
      endl;
  }
  else {
    os <<
      endl;
      
    gIndenter++;
    
    os <<
      fMeasuresRepeatPatternSegment;

    gIndenter--;
  }
      
  gIndenter--;
}

//______________________________________________________________________________
S_msrMeasuresRepeatReplicas msrMeasuresRepeatReplicas::create (
  int                 inputLineNumber,
  S_msrMeasuresRepeat measuresRepeatUplink)
{
  msrMeasuresRepeatReplicas* o =
    new msrMeasuresRepeatReplicas (
      inputLineNumber,
      measuresRepeatUplink);
  assert(o!=0);
  return o;
}

msrMeasuresRepeatReplicas::msrMeasuresRepeatReplicas (
  int                 inputLineNumber,
  S_msrMeasuresRepeat measuresRepeatUplink)
    : msrElement (inputLineNumber)
{
  // sanity check
  msrAssert (
    measuresRepeatUplink != nullptr,
    "measuresRepeatUplink is null");
    
  fMeasuresRepeatUplink = measuresRepeatUplink;
}

msrMeasuresRepeatReplicas::~msrMeasuresRepeatReplicas ()
{}

void msrMeasuresRepeatReplicas::setMeasuresRepeatReplicasSegment (
    S_msrSegment measuresRepeatReplicasSegment)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceRepeats) {
    gLogIOstream <<
      "Setting measure repeat replicas segment containing " <<
      singularOrPlural (
          measuresRepeatReplicasMeasuresNumber (),
        "measure",
        "measures") <<
      endl;
  }
#endif
      
  // sanity check
  msrAssert (
    measuresRepeatReplicasSegment != nullptr,
    "measuresRepeatReplicasSegment is null");

  fMeasuresRepeatReplicasSegment =
    measuresRepeatReplicasSegment;
}

int msrMeasuresRepeatReplicas::measuresRepeatReplicasMeasuresNumber () const
{
  int result;

  if (fMeasuresRepeatReplicasSegment)
    result =
      fMeasuresRepeatReplicasSegment->
      getSegmentMeasuresList ().size ();
  else
    result = 0;

  return result;
}

void msrMeasuresRepeatReplicas::acceptIn (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrMeasuresRepeatReplicas::acceptIn ()" <<
      endl;
  }
      
  if (visitor<S_msrMeasuresRepeatReplicas>*
    p =
      dynamic_cast<visitor<S_msrMeasuresRepeatReplicas>*> (v)) {
        S_msrMeasuresRepeatReplicas elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrMeasuresRepeatReplicas::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrMeasuresRepeatReplicas::acceptOut (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrMeasuresRepeatReplicas::acceptOut ()" <<
      endl;
  }

  if (visitor<S_msrMeasuresRepeatReplicas>*
    p =
      dynamic_cast<visitor<S_msrMeasuresRepeatReplicas>*> (v)) {
        S_msrMeasuresRepeatReplicas elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrMeasuresRepeatReplicas::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrMeasuresRepeatReplicas::browseData (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrMeasuresRepeatReplicas::browseData ()" <<
      endl;
  }

  if (fMeasuresRepeatReplicasSegment) {
  // browse the replicas segment
    msrBrowser<msrSegment> browser (v);
    browser.browse (*fMeasuresRepeatReplicasSegment);
  }
}

ostream& operator<< (ostream& os, const S_msrMeasuresRepeatReplicas& elt)
{
  elt->print (os);
  return os;
}

string msrMeasuresRepeatReplicas::asString () const
{
  stringstream s;

  s <<

    "MeasuresRepeatReplicas" <<
    ", line " << fInputLineNumber <<
    " (" <<
    singularOrPlural (
      measuresRepeatReplicasMeasuresNumber (),
      "replicas measure",
      "replicas measures") <<
    ")"; 

  return s.str ();
}

void msrMeasuresRepeatReplicas::print (ostream& os)
{
  os <<
    endl <<
    asString () <<
    endl <<
    endl;
  
  gIndenter++;
  
  // print the replicas segment
  os <<
    "Replicas segment:";

  if (! fMeasuresRepeatReplicasSegment) {
    os <<
      " none" <<
      endl;
  }
  else {
    os <<
      endl;
      
    gIndenter++;
    
    os <<
      fMeasuresRepeatReplicasSegment;

    gIndenter--;
  }

  gIndenter--;
}

//______________________________________________________________________________
S_msrMeasuresRepeat msrMeasuresRepeat::create (
  int        inputLineNumber,
  int        measuresRepeatMeasuresNumber,
  int        measuresRepeatSlashesNumber,
  S_msrVoice voiceUplink)
{
  msrMeasuresRepeat* o =
    new msrMeasuresRepeat (
      inputLineNumber,
      measuresRepeatMeasuresNumber,
      measuresRepeatSlashesNumber,
      voiceUplink);
  assert(o!=0);
  return o;
}

msrMeasuresRepeat::msrMeasuresRepeat (
  int        inputLineNumber,
  int        measuresRepeatMeasuresNumber,
  int        measuresRepeatSlashesNumber,
  S_msrVoice voiceUplink)
    : msrElement (inputLineNumber)
{
  // sanity check
  msrAssert (
    measuresRepeatMeasuresNumber > 0,
    "measuresRepeatMeasuresNumber is not positive");

  fMeasuresRepeatMeasuresNumber = measuresRepeatMeasuresNumber;

  // sanity check
  msrAssert (
    measuresRepeatSlashesNumber > 0,
    "measuresRepeatSlashesNumber is not positive");

  fMeasuresRepeatSlashesNumber  = measuresRepeatSlashesNumber;

  fMeasuresRepeatVoiceUplink = voiceUplink;
}

msrMeasuresRepeat::~msrMeasuresRepeat ()
{}

void msrMeasuresRepeat::setMeasuresRepeatPattern (
  S_msrMeasuresRepeatPattern measuresRepeatPattern)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceRepeats) {
    gLogIOstream <<
      "Setting measure repeat pattern containing " <<
      singularOrPlural (
        measuresRepeatPattern->
          measuresRepeatPatternMeasuresNumber (),
        "measure",
        "measures") <<
      endl;
  }
#endif
      
  // sanity check
  msrAssert (
    measuresRepeatPattern != nullptr,
    "measuresRepeatPattern is null");

  fMeasuresRepeatPattern = measuresRepeatPattern;
}

void msrMeasuresRepeat::setMeasuresRepeatReplicas (
  S_msrMeasuresRepeatReplicas measuresRepeatReplicas)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceRepeats) {
    gLogIOstream <<
      "Setting measure repeat replicas containing " <<
      singularOrPlural (
        measuresRepeatReplicas->
          measuresRepeatReplicasMeasuresNumber (),
        "measure",
        "measures") <<
      endl;
  }
#endif
      
  // sanity check
  msrAssert (
    measuresRepeatReplicas != nullptr,
    "measuresRepeatReplicas is null");

  fMeasuresRepeatReplicas = measuresRepeatReplicas;
}

int msrMeasuresRepeat::measuresRepeatReplicasNumber () const
{  
  int patternMeasuresNumber =
    measuresRepeatPatternMeasuresNumber ();
    
  // sanity check
  msrAssert (
    patternMeasuresNumber > 0,
    "patternMeasuresNumber is not positive");

  return
    measuresRepeatReplicasMeasuresNumber ()
      /
    patternMeasuresNumber;    
}

void msrMeasuresRepeat::acceptIn (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrMeasuresRepeat::acceptIn ()" <<
      endl;
  }
      
  if (visitor<S_msrMeasuresRepeat>*
    p =
      dynamic_cast<visitor<S_msrMeasuresRepeat>*> (v)) {
        S_msrMeasuresRepeat elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrMeasuresRepeat::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrMeasuresRepeat::acceptOut (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrMeasuresRepeat::acceptOut ()" <<
      endl;
  }

  if (visitor<S_msrMeasuresRepeat>*
    p =
      dynamic_cast<visitor<S_msrMeasuresRepeat>*> (v)) {
        S_msrMeasuresRepeat elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrMeasuresRepeat::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrMeasuresRepeat::browseData (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrMeasuresRepeat::browseData ()" <<
      endl;
  }

  if (fMeasuresRepeatPattern) {
  // browse the measdure repeat pattern
    msrBrowser<msrMeasuresRepeatPattern> browser (v);
    browser.browse (*fMeasuresRepeatPattern);
  }

  // fetch the score
  S_msrScore
    score =
      fMeasuresRepeatVoiceUplink->
        fetchVoicePartUplink ()->
          getPartPartGroupUplink ()->
            getPartGroupScoreUplink ();
              
  bool inhibitMeasuresRepeatReplicasBrowsing =
    score->getInhibitMeasuresRepeatReplicasBrowsing ();

  if (inhibitMeasuresRepeatReplicasBrowsing) {
    if (
      gMsrOptions->fTraceMsrVisitors
#ifdef TRACE_OPTIONS
        ||
      gTraceOptions->fTraceRepeats
#endif
    ) {
      gLogIOstream <<
        "% ==> visiting measure repeat replicas is inhibited" <<
        endl;
    }
  }

  if (fMeasuresRepeatReplicas) {
    if (! inhibitMeasuresRepeatReplicasBrowsing) {
      // browse the measure repeat replicas
      msrBrowser<msrMeasuresRepeatReplicas> browser (v);
      browser.browse (*fMeasuresRepeatReplicas);
    }
  }
}

ostream& operator<< (ostream& os, const S_msrMeasuresRepeat& elt)
{
  elt->print (os);
  return os;
}

string msrMeasuresRepeat::asString () const
{
  stringstream s;

  s <<

    "MeasuresRepeat" <<
    ", line " << fInputLineNumber <<
    " (" <<
    singularOrPlural (
      measuresRepeatPatternMeasuresNumber (),
      "repeated measure",
      "repeated measures") <<
    ", " <<
    singularOrPlural (
      measuresRepeatReplicasMeasuresNumber (),
      "replicas measure",
      "replicas measures") <<
    ", " <<
    measuresRepeatReplicasNumber () << " replicas" <<
    ")"; 

  return s.str ();
}

void msrMeasuresRepeat::print (ostream& os)
{
  os <<
    endl <<
    "MeasuresRepeat" <<
    ", line " << fInputLineNumber <<
    " (" <<
    singularOrPlural (
      measuresRepeatPatternMeasuresNumber (),
      "pattern measure",
      "pattern measures") <<
    ", " <<
    /* JMI ???
    singularOrPlural (
      measuresRepeatReplicasMeasuresNumber (),
      "replica measure",
      "replicas measures") <<
      */
    ")" <<
    endl;
  
  gIndenter++;
  
  // print the measure repeat pattern
  if (! fMeasuresRepeatPattern) {
    os <<
      "Measure repeat pattern: none" <<
      endl;
  }

  else {
    os <<
      fMeasuresRepeatPattern <<
      endl;
  }
  
  // print the measure repeat replicas
  if (! fMeasuresRepeatReplicas) {
    os <<
      "Measure repeat replicas: none" <<
      endl;
  }

  else {
    os <<
      fMeasuresRepeatReplicas;
  }
      
  gIndenter--;
}

//______________________________________________________________________________
S_msrMultipleRestContents msrMultipleRestContents::create (
  int        inputLineNumber,
  S_msrVoice voiceUplink)
{
  msrMultipleRestContents* o =
    new msrMultipleRestContents (
      inputLineNumber,
      voiceUplink);
  assert(o!=0);
  return o;
}

msrMultipleRestContents::msrMultipleRestContents (
  int        inputLineNumber,
  S_msrVoice voiceUplink)
    : msrElement (inputLineNumber)
{
  fMultipleRestContentsVoiceUplink = voiceUplink;
}

msrMultipleRestContents::~msrMultipleRestContents ()
{}

S_msrMultipleRestContents msrMultipleRestContents::createMultipleRestContentsNewbornClone (
  S_msrVoice containingVoice)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceRepeats) {
    gLogIOstream <<
      "Creating a newborn clone of a multiple rest contents" <<
      endl;
  }
#endif
  
  // sanity check
  msrAssert(
    containingVoice != nullptr,
    "containingVoice is null");
    
  S_msrMultipleRestContents
    newbornClone =
      msrMultipleRestContents::create (
        fInputLineNumber,
        containingVoice);

  return newbornClone;
}

void msrMultipleRestContents::setMultipleRestContentsSegment (
    S_msrSegment multipleRestContentsSegment)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceRepeats) {
    gLogIOstream <<
      "Setting multiple rest contents segment containing " <<
      singularOrPlural (
          multipleRestContentsMeasuresNumber (),
        "measure",
        "measures") <<
      endl;
  }
#endif
      
  // sanity check
  msrAssert (
    multipleRestContentsSegment != nullptr,
    "multipleRestContentsSegment is null");

  fMultipleRestContentsSegment =
    multipleRestContentsSegment;
}

int msrMultipleRestContents::multipleRestContentsMeasuresNumber () const
{
  int result;

  if (fMultipleRestContentsSegment)
    result =
      fMultipleRestContentsSegment->
      getSegmentMeasuresList ().size ();
  else
    result = 0;

  return result;
}

void msrMultipleRestContents::acceptIn (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrMultipleRestContents::acceptIn ()" <<
      endl;
  }
      
  if (visitor<S_msrMultipleRestContents>*
    p =
      dynamic_cast<visitor<S_msrMultipleRestContents>*> (v)) {
        S_msrMultipleRestContents elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrMultipleRestContents::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrMultipleRestContents::acceptOut (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrMultipleRestContents::acceptOut ()" <<
      endl;
  }

  if (visitor<S_msrMultipleRestContents>*
    p =
      dynamic_cast<visitor<S_msrMultipleRestContents>*> (v)) {
        S_msrMultipleRestContents elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrMultipleRestContents::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrMultipleRestContents::browseData (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrMultipleRestContents::browseData ()" <<
      endl;
  }

  if (fMultipleRestContentsSegment) {
    // browse the pattern segment
    msrBrowser<msrSegment> browser (v);
    browser.browse (*fMultipleRestContentsSegment);
  }
}

ostream& operator<< (ostream& os, const S_msrMultipleRestContents& elt)
{
  elt->print (os);
  return os;
}

string msrMultipleRestContents::asString () const
{
  stringstream s;

  s <<

    "MultipleRestContents" <<
    ", line " << fInputLineNumber <<
    " (" <<
    singularOrPlural (
      multipleRestContentsMeasuresNumber (),
      "repeated measure",
      "repeated measures") <<
    ")"; 

  return s.str ();
}

void msrMultipleRestContents::print (ostream& os)
{
  os <<
    endl <<
    asString () <<
    endl <<
    endl;
  
  gIndenter++;
  
  // print the pattern segment
  os <<
    "Contents segment:";

  if (! fMultipleRestContentsSegment) {
    os <<
      " none" <<
      endl;
  }
  else {
    os <<
      endl;
      
    gIndenter++;
    
    os <<
      fMultipleRestContentsSegment;

    gIndenter--;
  }
      
  gIndenter--;
}

//______________________________________________________________________________
S_msrMultipleRest msrMultipleRest::create (
  int        inputLineNumber,
  rational   multipleRestMeasureSoundingNotes,
  int        multipleRestMeasuresNumber,
  S_msrVoice voiceUplink)
{
  msrMultipleRest* o =
    new msrMultipleRest (
      inputLineNumber,
      multipleRestMeasureSoundingNotes,
      multipleRestMeasuresNumber,
      voiceUplink);
  assert(o!=0);
  return o;
}

msrMultipleRest::msrMultipleRest (
  int        inputLineNumber,
  rational   multipleRestMeasureSoundingNotes,
  int        multipleRestMeasuresNumber,
  S_msrVoice voiceUplink)
    : msrElement (inputLineNumber)
{
  fMultipleRestMeasureSoundingNotes = multipleRestMeasureSoundingNotes;
  fMultipleRestMeasuresNumber       = multipleRestMeasuresNumber;

  fMultipleRestVoiceUplink = voiceUplink;
}

msrMultipleRest::~msrMultipleRest ()
{}

S_msrMultipleRest msrMultipleRest::createMultipleRestNewbornClone (
  S_msrVoice containingVoice)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceRepeats) {
    gLogIOstream <<
      "Creating a newborn clone of multiple rest " <<
      asString () <<
      endl;
  }
#endif
  
  // sanity check
  msrAssert(
    containingVoice != nullptr,
    "containingVoice is null");
    
  S_msrMultipleRest
    newbornClone =
      msrMultipleRest::create (
        fInputLineNumber,
        fMultipleRestMeasureSoundingNotes,
        fMultipleRestMeasuresNumber,
        containingVoice);

  newbornClone->fMultipleRestNextMeasureNumber =
    fMultipleRestNextMeasureNumber;
    
  return newbornClone;
}

void msrMultipleRest::setMultipleRestContents (
  S_msrMultipleRestContents multipleRestContents)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceRepeats) {
    gLogIOstream <<
      "Setting multiple rest segment containing " <<
      singularOrPlural (
        multipleRestContents->
          multipleRestContentsMeasuresNumber (),
        "measure",
        "measures") <<
      endl;
  }
#endif
      
  // sanity check
  msrAssert (
    multipleRestContents != nullptr,
    "multipleRestContents is null");

  fMultipleRestContents = multipleRestContents;
}

void msrMultipleRest::setMultipleRestNextMeasureNumber (
  string measureNumber)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceRepeats) {
    gLogIOstream <<
      "Setting multiple rest next measure number to'" <<
      "' " <<
      measureNumber <<
      endl;
  }
#endif

  fMultipleRestNextMeasureNumber =
    measureNumber;
}

void msrMultipleRest::acceptIn (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrMultipleRest::acceptIn ()" <<
      endl;
  }
      
  if (visitor<S_msrMultipleRest>*
    p =
      dynamic_cast<visitor<S_msrMultipleRest>*> (v)) {
        S_msrMultipleRest elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrMultipleRest::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrMultipleRest::acceptOut (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrMultipleRest::acceptOut ()" <<
      endl;
  }

  if (visitor<S_msrMultipleRest>*
    p =
      dynamic_cast<visitor<S_msrMultipleRest>*> (v)) {
        S_msrMultipleRest elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrMultipleRest::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrMultipleRest::browseData (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrMultipleRest::browseData ()" <<
      endl;
  }

/* JMI
  // get the part uplink
  S_msrPart
    partUplink =
      fMultipleRestVoiceUplink->
        fetchVoicePartUplink ();

  // get the part group uplink
  S_msrPartGroup
    partGroupUplink =
      partUplink->
        getPartPartGroupUplink ();

  // get the score uplink
  S_msrScore
    scoreUplink =
      partGroupUplink->
        getPartGroupScoreUplink ();

  bool inhibitMultipleRestMeasuresBrowsing =
    scoreUplink->
      getInhibitMultipleRestMeasuresBrowsing ();

  if (inhibitMultipleRestMeasuresBrowsing) {
    if (gMsrOptions->fTraceMsrVisitors || gTraceOptions->fTraceRepeats) {
      gLogIOstream <<
        "% ==> visiting multiple rest measures is inhibited" <<
        endl;
    }
  }
*/

  if (fMultipleRestContents) {
 // JMI   if (! inhibitMultipleRestMeasuresBrowsing) {
      // browse the multiple rest contents
      msrBrowser<msrMultipleRestContents> browser (v);
      browser.browse (*fMultipleRestContents);
//    }
  }
}

string msrMultipleRest::asString () const
{
  stringstream s;

  s <<
    "MultipleRest" <<
    ", line " << fInputLineNumber <<
    ", multipleRestMeasureSoundingNotes: " <<
    fMultipleRestMeasureSoundingNotes <<
    ", " <<
    singularOrPlural (
      fMultipleRestMeasuresNumber,
        "rest measure",
        "rest measures") <<
    ", next measure number: '" <<
    fMultipleRestNextMeasureNumber <<
    "'";
    
  return s.str ();
}

void msrMultipleRest::print (ostream& os)
{
  os <<
    "MultipleRest" <<
    ", line " << fInputLineNumber <<
    endl;

  gIndenter++;

  const int fieldWidth = 33;

  os << left <<
    setw (fieldWidth) <<
    "multipleRestMeasureSoundingNotes" << " : " <<
    fMultipleRestMeasureSoundingNotes <<
    endl <<
    setw (fieldWidth) <<
    "multipleRestMeasuresNumber" << " : " <<
    fMultipleRestMeasuresNumber <<
    endl <<
    setw (fieldWidth) <<
    "multipleRestNextMeasureNumber" << " : '" <<
    fMultipleRestNextMeasureNumber <<
    "'" <<
    endl;
  
  // print the voice uplink
  os << left <<
    setw (fieldWidth) <<
    "multipleRestVoiceUplink" << " : " <<
    "\"" <<
    fMultipleRestVoiceUplink->getVoiceName () <<
    "\"" <<
    endl;
    
  // print the rests contents
  if (! fMultipleRestContents) {
    os << left <<
      setw (fieldWidth) <<
      "multipleRestContents" << " : " << "none" <<
      endl;
  }

  else {
    os <<
      fMultipleRestContents;
  }
      
  gIndenter--;
}

ostream& operator<< (ostream& os, const S_msrMultipleRest& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_msrRepeatCoda msrRepeatCoda::create (
  int                 inputLineNumber,
  S_msrSegment        repeatCodaSegment,
  S_msrRepeat         repeatUplink)
{
  msrRepeatCoda* o =
    new msrRepeatCoda (
      inputLineNumber,
      repeatCodaSegment,
      repeatUplink);
  assert(o!=0);
  return o;
}

msrRepeatCoda::msrRepeatCoda (
  int                 inputLineNumber,
  S_msrSegment        repeatCodaSegment,
  S_msrRepeat         repeatUplink)
    : msrElement (inputLineNumber)
{
  // sanity check
  msrAssert (
    repeatCodaSegment != nullptr,
    "repeatCodaSegment is null");
    
  fRepeatCodaSegment = repeatCodaSegment;
  
  // sanity check
  msrAssert (
    repeatUplink != nullptr,
    "repeatUplink is null");
    
  fRepeatCodaRepeatUplink = repeatUplink;
}

msrRepeatCoda::~msrRepeatCoda ()
{}

S_msrRepeatCoda msrRepeatCoda::createRepeatCodaNewbornClone (
  S_msrRepeat containingRepeat)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceRepeats) {
    gLogIOstream <<
      "Creating a newborn clone of a " <<
      asString () <<
      endl;
  }
#endif
  
  // sanity check
  msrAssert(
    containingRepeat != nullptr,
    "containingRepeat is null");
    
  S_msrRepeatCoda
    newbornClone = 0; // JMI
    /*
      msrRepeatCoda::create (
        fInputLineNumber,
        containingRepeat->
          getRepeatCoda ()->
            getRepeatCodaSegment (), // JMI
        containingRepeat);
      */
      
  // segment

  // uplinks

  return newbornClone;
}

S_msrRepeatCoda msrRepeatCoda::createRepeatCodaDeepCopy (
  S_msrRepeat containingRepeat)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceRepeats) {
    gLogIOstream <<
      "Creating a newborn clone of a " <<
      asString () <<
      endl;
  }
#endif
  
  // sanity check
  msrAssert(
    containingRepeat != nullptr,
    "containingRepeat is null");
    
  S_msrRepeatCoda
    repeatCodaDeepCopy = 0; // JMI
    /* JMI
      msrRepeatCoda::create (
        fInputLineNumber,
        containingRepeat->
          getRepeatCoda ()->
            getRepeatCodaSegment (), // JMI
        containingRepeat);
    */
    
  // segment
  repeatCodaDeepCopy->fRepeatCodaSegment =
    fRepeatCodaSegment->
      createSegmentDeepCopy (
        fRepeatCodaRepeatUplink->
          getRepeatVoiceUplink ());
    
  // uplinks
  repeatCodaDeepCopy->fRepeatCodaRepeatUplink =
    containingRepeat;

  return repeatCodaDeepCopy;
}

/* JMI
void msrRepeatCoda::appendElementToRepeatCoda (
  S_msrElement elem) // JMI ???
{
  fRepeatCodaSegment->
    appendOtherElementToSegment (elem);
}
*/

void msrRepeatCoda::acceptIn (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrRepeatCoda::acceptIn ()" <<
      endl;
  }
      
  if (visitor<S_msrRepeatCoda>*
    p =
      dynamic_cast<visitor<S_msrRepeatCoda>*> (v)) {
        S_msrRepeatCoda elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrRepeatCoda::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrRepeatCoda::acceptOut (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrRepeatCoda::acceptOut ()" <<
      endl;
  }

  if (visitor<S_msrRepeatCoda>*
    p =
      dynamic_cast<visitor<S_msrRepeatCoda>*> (v)) {
        S_msrRepeatCoda elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrRepeatCoda::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrRepeatCoda::browseData (basevisitor* v)
{
  // browse the segment
  msrBrowser<msrSegment> browser (v);
  browser.browse (*fRepeatCodaSegment);
}

ostream& operator<< (ostream& os, const S_msrRepeatCoda& elt)
{
  elt->print (os);
  return os;
}

string msrRepeatCoda::asString () const
{
  stringstream s;

  s <<
    "RepeatCoda" <<
    ", line " << fInputLineNumber <<
    endl;

  return s.str ();
}

void msrRepeatCoda::print (ostream& os)
{
  os <<
    endl <<
    asString () <<
    endl;
 
  gIndenter++;

  os <<
    fRepeatCodaSegment;

  gIndenter--;
}

//______________________________________________________________________________ 
// global variable
int msrVoice::gVoicesCounter = 0;

S_msrVoice msrVoice::create (
  int          inputLineNumber,
  msrVoiceKind voiceKind,
  int          voiceNumber,
  msrVoiceCreateInitialLastSegmentKind
               voiceCreateInitialLastSegmentKind,
  S_msrStaff   voiceStaffUplink)
{
  msrVoice* o =
    new msrVoice (
      inputLineNumber,
      voiceKind,
      voiceNumber,
      voiceCreateInitialLastSegmentKind,
      voiceStaffUplink);
  assert(o!=0);

  return o;
}

msrVoice::msrVoice (
  int          inputLineNumber,
  msrVoiceKind voiceKind,
  int          voiceNumber,
  msrVoiceCreateInitialLastSegmentKind
               voiceCreateInitialLastSegmentKind,
  S_msrStaff   voiceStaffUplink)
    : msrElement (inputLineNumber)
{  
  // sanity check
  msrAssert(
    voiceStaffUplink != nullptr,
    "voiceStaffUplink is null");

  // set voice staff uplink
  fVoiceStaffUplink = voiceStaffUplink;

  // set voice kind
  fVoiceKind = voiceKind;

  // set voice number
  fVoiceNumber = voiceNumber;
  
  // do other initializations
  initializeVoice (
    voiceCreateInitialLastSegmentKind);
}

msrVoice::~msrVoice ()
{}

S_msrPart msrVoice::fetchVoicePartUplink () const
{
 return
  fVoiceStaffUplink->
    getStaffPartUplink ();
}

void msrVoice::setVoiceNameFromNumber (
  int inputLineNumber,
  int voiceNumber)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceVoices) {
    gLogIOstream <<
      "Setting the name of '" <<
      voiceKindAsString () <<
      "' voice \"" <<
      "\"" <<
      " from number: " << voiceNumber <<
      endl;
  }
#endif

  switch (fVoiceKind) {
    case msrVoice::kRegularVoice:
      fVoiceName =
        fVoiceStaffUplink->getStaffName () +
        "_Voice_" +
        int2EnglishWord (voiceNumber);
      break;
      
    case msrVoice::kHarmonyVoice:
      fVoiceName =
        fVoiceStaffUplink->getStaffName () +
        "_Voice_" +
        int2EnglishWord (
          voiceNumber - K_VOICE_HARMONY_VOICE_BASE_NUMBER) +
          "_HARMONY";
      break;
      
    case msrVoice::kFiguredBassVoice:
      {
        stringstream s;

        s <<
          "A '" <<
          getVoiceName () <<
          "' voice cannot get its name from its number";

        msrInternalError (
          gXml2lyOptions->fInputSourceName,
          inputLineNumber,
          __FILE__, __LINE__,
          s.str ());
      }
      break;
  } // switch
}

void msrVoice::setVoiceCloneLastSegment (
  S_msrSegment segment)
{
  fVoiceLastSegment = segment;

  if (! fVoiceFirstSegment) {
      fVoiceFirstSegment = fVoiceLastSegment;
  }
}

void msrVoice::initializeVoice (
  msrVoiceCreateInitialLastSegmentKind
    voiceCreateInitialLastSegmentKind)
{
  // the voice staff sequential number will be set
  // when regular voices are added to a staff
  // in registerVoiceInRegularVoicesMap ()
  fRegularVoiceStaffSequentialNumber = -1;

  gIndenter++;
  
  // compute voice number
  int voiceNumber =
    gMsrOptions->
      fCreateVoicesStaffRelativeNumbers // JMI use it
        ? fRegularVoiceStaffSequentialNumber
        : fVoiceNumber;
  
  // set voice name
  switch (fVoiceKind) {
    case msrVoice::kRegularVoice:
      setVoiceNameFromNumber (
        fInputLineNumber,
        voiceNumber);
      break;
      
    case msrVoice::kHarmonyVoice:
      setVoiceNameFromNumber (
        fInputLineNumber,
        voiceNumber);
      break;
      
    case msrVoice::kFiguredBassVoice:
      fVoiceName =
        fVoiceStaffUplink->getStaffName () +
        "_FIGURED_BASS_Voice";
      break;
  } // switch
  
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceVoices) {
    gLogIOstream <<
      "Initializing voice \"" << fVoiceName <<
      "\" in staff \"" <<
      fVoiceStaffUplink->getStaffName () <<
      "\"" <<
      endl;
  }
#endif

  // check voice number
  switch (fVoiceKind) {
    case msrVoice::kRegularVoice:
      // the voice number should be positive
      if (fVoiceNumber < 1 || fVoiceNumber > 4) {
        stringstream s;
    
        s <<
          "regular voice number " << fVoiceNumber <<
          " is not in the 1..4 range";

          /* JMI ???
        msrMusicXMLError (
          gXml2lyOptions->fInputSourceName,
          fInputLineNumber,
          __FILE__, __LINE__,
          s.str ());
          */
      }
      break;
      
    case msrVoice::kHarmonyVoice:
      break;
      
    case msrVoice::kFiguredBassVoice:
      if (fVoiceNumber != K_PART_FIGURED_BASS_VOICE_NUMBER) {
        stringstream s;
    
        s <<
          "figured bass voice number " << fVoiceNumber <<
          " is not equal to " << K_PART_FIGURED_BASS_VOICE_NUMBER;
          
        msrInternalError (
          gXml2lyOptions->fInputSourceName,
          fInputLineNumber,
          __FILE__, __LINE__,
          s.str ());
      }
      break;
  } // switch

  // voice shortest note
  fVoiceShortestNoteDuration =
    rational (INT_MAX, 1);
  fVoiceShortestNoteTupletFactor =
    rational (1, 1);
    
  // should the initial last segment be created?
  switch (voiceCreateInitialLastSegmentKind) {
    case msrVoice::kCreateInitialLastSegmentYes:
#ifdef TRACE_OPTIONS
      if (gTraceOptions->fTraceVoices || gTraceOptions->fTraceSegments) {
        gLogIOstream <<
          "Creating an initial voice last segment for voice \"" <<
          fVoiceName <<
          "\" in staff \"" <<
          fVoiceStaffUplink->getStaffName () <<
          "\"" <<
          endl;
      }
#endif
      
      createNewLastSegmentForVoice (
        fInputLineNumber);
      break;
      
    case msrVoice::kCreateInitialLastSegmentNo:
#ifdef TRACE_OPTIONS
      if (gTraceOptions->fTraceVoices || gTraceOptions->fTraceSegments) {
        gLogIOstream <<
          "NO initial voice last segment is created for voice \"" <<
          fVoiceName <<
          "\" in staff \"" <<
          fVoiceStaffUplink->getStaffName () <<
          "\"" <<
          endl;
      }
#endif
      break;
  } // switch
  
  // set voice number
  fVoiceCurrentMeasureNumber = // JMI "??";
    fetchVoicePartUplink ()->
      getPartCurrentMeasureNumber ();

  // music has not been inserted in voice yet
  fMusicHasBeenInsertedInVoice = false;

  // counters
  fVoiceActualNotesCounter     = 0;
  fVoiceRestsCounter           = 0;
  fVoiceSkipsCounter           = 0;
  fVoiceActualHarmoniesCounter = 0;

  // multiple rests
  fVoiceRemainingRestMeasures = 0;

  // get the initial staff details from the staff if any
  S_msrStaffDetails
    staffStaffDetails =
      fVoiceStaffUplink->
        getStaffStaffDetails ();

  if (staffStaffDetails) {
    // append it to the voice
    appendStaffDetailsToVoice (
      staffStaffDetails);
  }

#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceVoices) {
    gLogIOstream <<
      "Initial contents of voice \"" << fVoiceName <<
      "\" in staff \"" <<
      fVoiceStaffUplink->getStaffName () <<
      "\":" <<
      endl;

    gIndenter++;

    this->print (gLogIOstream);

    gIndenter--;
  }
#endif

  gIndenter--;
}

void msrVoice::changeVoiceIdentity ( // after a deep copy
  int voiceNumber)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceVoices) {
    gLogIOstream <<
      "Changing identity of voice \"" <<
      getVoiceName () <<
      "\"" <<
      ", number: " << voiceNumber <<
      endl;
  }
#endif

  // make it a regular voice
  setVoiceKind (
    msrVoice::kRegularVoice);

  // set its voice number
  setVoiceNumber (
    voiceNumber);

  // set its name
  setVoiceNameFromNumber (
    fInputLineNumber,
    voiceNumber);
}
    
S_msrVoice msrVoice::createVoiceNewbornClone (
  S_msrStaff staffClone)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceVoices) {
    gLogIOstream <<
      "Creating a newborn clone of voice \"" <<
      getVoiceName () <<
      "\"" <<
      endl;
  }
#endif

  // sanity check
  msrAssert(
    staffClone != nullptr,
    "staffClone is null");
    
  S_msrVoice
    newbornClone =
      msrVoice::create (
        fInputLineNumber,
        fVoiceKind,
        fVoiceNumber,
        msrVoice::kCreateInitialLastSegmentNo,
          // initial segment will be created upon a later segment visit
        staffClone);

  // voice numbers
  newbornClone->fRegularVoiceStaffSequentialNumber =
    fRegularVoiceStaffSequentialNumber;

  // voice name
  newbornClone->fVoiceName =
    fVoiceName;

  // counters
  newbornClone->fVoiceActualNotesCounter =
    fVoiceActualNotesCounter;

  newbornClone->fVoiceRestsCounter =
    fVoiceRestsCounter;

  newbornClone->fVoiceSkipsCounter =
    fVoiceSkipsCounter;

  newbornClone->fVoiceActualHarmoniesCounter =
    fVoiceActualHarmoniesCounter;

  // measures
  newbornClone->fVoiceCurrentMeasureNumber =
    fVoiceCurrentMeasureNumber;

  // musically empty voices
  newbornClone->fMusicHasBeenInsertedInVoice =
    fMusicHasBeenInsertedInVoice;

  // initial elements

  // first segment

  // repeats
  
  // multple rests
  newbornClone->fVoiceContainsMultipleRests =
    fVoiceContainsMultipleRests;

  // stanzas

  // uplinks

  return newbornClone;
}

S_msrVoice msrVoice::createVoiceDeepCopy (
  int          inputLineNumber,
  msrVoiceKind voiceKind,
  int          voiceNumber,
  S_msrStaff   containingStaff)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceVoices) {
    gLogIOstream <<
      "Creating a deep copy of voice \"" <<
      getVoiceName () <<
      "\"" <<
      ", to be placed in containing staff \"" <<
      containingStaff->getStaffName () << "\"" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  // sanity check
  msrAssert(
    containingStaff != nullptr,
    "containingStaff is null");
    
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceVoicesDetails) {
    gLogIOstream <<
      "****" <<
      " BEFORE voiceDeepCopy = " <<
      endl;
  
    print (gLogIOstream);
    
    gLogIOstream <<
      endl <<
      "****" <<
      endl;
  }
#endif
    
  S_msrVoice
    voiceDeepCopy =
      msrVoice::create (
        fInputLineNumber,
        voiceKind,
        voiceNumber,
        msrVoice::kCreateInitialLastSegmentNo,
          // the voice initial last segment
          // will be created by deep cloning below
        containingStaff);

  // voice numbers
  voiceDeepCopy->fRegularVoiceStaffSequentialNumber =
    fRegularVoiceStaffSequentialNumber;

  // voice name
  if (false) // JMI
  voiceDeepCopy->fVoiceName =
    fVoiceName;

/* JMI
  // set voice name // JMI
  voiceDeepCopy->
    setVoiceNameFromNumber (
      fInputLineNumber,
      voiceNumber);
*/

  // counters
  voiceDeepCopy->fVoiceActualNotesCounter =
    fVoiceActualNotesCounter;

  voiceDeepCopy->fVoiceRestsCounter =
    fVoiceRestsCounter;

  voiceDeepCopy->fVoiceSkipsCounter =
    fVoiceSkipsCounter;

  voiceDeepCopy->fVoiceActualHarmoniesCounter =
    fVoiceActualHarmoniesCounter;

  // measures
  voiceDeepCopy->fVoiceCurrentMeasureNumber =
    fVoiceCurrentMeasureNumber;

  // musically empty voices
  voiceDeepCopy->fMusicHasBeenInsertedInVoice =
    fMusicHasBeenInsertedInVoice;

  // initial elements
  int numberOfInitialElements =
    fVoiceInitialElementsList.size ();
    
  if (numberOfInitialElements) {
#ifdef TRACE_OPTIONS
    if (gTraceOptions->fTraceVoices) {
      gLogIOstream <<
        "There are " <<
        numberOfInitialElements <<
        " initial elements in voice \"" <<
        getVoiceName () <<
        "\"" <<
        endl;
    }
#endif

   list<S_msrElement>::const_iterator
      iBegin = fVoiceInitialElementsList.begin (),
      iEnd   = fVoiceInitialElementsList.end (),
      i      = iBegin;
      
    for ( ; ; ) {
      // handle the deep copy
      S_msrElement
        elementDeepCopy;
        
      if (
        S_msrRepeat repeat = dynamic_cast<msrRepeat*>(&(*(*i)))
        ) {
          /* JMI ???
        // create the repeat deep copy
        elementDeepCopy =
          repeat->createRepeatDeepCopy (
            voiceDeepCopy);
            */
      }
    
      else if (
        // create the segment deep copy
        S_msrSegment segment = dynamic_cast<msrSegment*>(&(*(*i)))
        ) {
        elementDeepCopy =
          segment->createSegmentDeepCopy (
            voiceDeepCopy);
      }
      
      else {
        stringstream s;

        s <<
          "voice  \"" <<
          getVoiceName () <<
          "\" initial elements element should be a repeat or a segment";

        msrInternalError (
          gXml2lyOptions->fInputSourceName,
          fInputLineNumber,
          __FILE__, __LINE__,
          s.str ());
      }
  
      // append the element deep copy to the voice deep copy
      voiceDeepCopy->
        fVoiceInitialElementsList.push_back (
          elementDeepCopy);
        
      if (++i == iEnd) break;
    } // for
  }

  else {    
#ifdef TRACE_OPTIONS
    if (gTraceOptions->fTraceVoices) {
      gLogIOstream <<
        "There are no initial elements in voice \"" <<
        getVoiceName () <<
        "\" to be deep copied" <<
        endl;
    }
#endif
  }

  // last segment
  if (fVoiceLastSegment) { // JMI
    voiceDeepCopy->fVoiceLastSegment =
      fVoiceLastSegment->
        createSegmentDeepCopy (
          voiceDeepCopy);

    if (! voiceDeepCopy->fVoiceFirstSegment) {
      voiceDeepCopy->fVoiceFirstSegment =
        voiceDeepCopy->fVoiceLastSegment;
    }
  }
  else {    
#ifdef TRACE_OPTIONS
    if (gTraceOptions->fTraceVoices) {
      gLogIOstream <<
        "There is no last segment in voice \"" <<
        getVoiceName () <<
        "\" to be deep copied" <<
        endl;
    }
#endif
  }
  
  // multiple rests
  voiceDeepCopy->fVoiceContainsMultipleRests =
    fVoiceContainsMultipleRests;

  for (
    map<string, S_msrStanza>::const_iterator i = fVoiceStanzasMap.begin ();
    i != fVoiceStanzasMap.end ();
    i++) {
    S_msrStanza stanza = (*i).second;

    voiceDeepCopy->
      addStanzaToVoiceWithoutCatchUp ( // JMI
        stanza->
          createStanzaDeepCopy (
            voiceDeepCopy));
  } // for

  // uplinks
  voiceDeepCopy->fVoiceStaffUplink =
    containingStaff;

#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceVoicesDetails) {
    gLogIOstream <<
      "****" <<
      " AFTER voiceDeepCopy = " <<
      endl <<
      voiceDeepCopy <<
      endl <<
      "****" <<
      endl;
  }
#endif
    
  return voiceDeepCopy;
}

void msrVoice::appendAFirstMeasureToVoiceIfNotYetDone (
  int inputLineNumber)
{ // JMI EMPTY
}

void msrVoice::createMeasureAndAppendItToVoice (
  int    inputLineNumber,
  string measureNumber,
  int    measureOrdinalNumber,
  msrMeasure::msrMeasureImplicitKind
         measureImplicitKind)
{
  fVoiceCurrentMeasureNumber = measureNumber;

#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceMeasures || gTraceOptions->fTraceVoices) {
    gLogIOstream <<
      "Creating measure '" <<
      measureNumber <<
      "' and appending it to voice \"" << getVoiceName () << "\"" <<
      "', line " << inputLineNumber <<
      endl;
  }
#endif

  gIndenter++;
  
  if (! fVoiceLastSegment) {
    // create the voice last segment if not yet done // JMI
    createNewLastSegmentForVoice (
      inputLineNumber);
  }

  // append new measure with given number
  fVoiceLastSegment->
    createMeasureAndAppendItToSegment (
      inputLineNumber,
      measureNumber,
      measureOrdinalNumber,
      measureImplicitKind);

  // handle voice kind
  switch (fVoiceKind) {
    case msrVoice::kHarmonyVoice:
      break;
    case msrVoice::kRegularVoice:
    /* JMI
      // append a new measure with given number
      fVoiceLastSegment->
        createMeasureAndAppendItToSegment (
          inputLineNumber,
          measureNumber,
          measureOrdinalNumber,
          measureImplicitKind);
*/

      // append new measure with given number to voice harmony voice if any
      if (fHarmonyVoiceForRegularVoice) {
        fHarmonyVoiceForRegularVoice->
          createMeasureAndAppendItToVoice (
            inputLineNumber,
            measureNumber,
            measureOrdinalNumber,
            measureImplicitKind);
      }
      break;
    case msrVoice::kFiguredBassVoice:
      break;
  } // switch

  gIndenter--;
}

void msrVoice::setNextMeasureNumberInVoice (
  int    inputLineNumber,
  string nextMeasureNumber)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceMeasures || gTraceOptions->fTraceVoices) {
    gLogIOstream <<
      "Setting next measure number to '" <<
      nextMeasureNumber <<
      ", in voice \"" << getVoiceName () << "\"" <<
      "', line " << inputLineNumber <<
      endl;
  }
#endif

  gIndenter++;
  
  fVoiceLastSegment->
    setNextMeasureNumberInSegment (
      inputLineNumber,
      nextMeasureNumber);

  // is there a pending multiple rest in this voice?
  if (fVoiceMultipleRestWaitingForItsNextMeasureNumber) {      
    // yes
#ifdef TRACE_OPTIONS
    if (gTraceOptions->fTraceRepeats) {
      gLogIOstream <<
        "There is a multiple rest waiting for its next measure number" <<
        ", fVoiceRemainingRestMeasures = " <<
        fVoiceRemainingRestMeasures <<
        "' in voice \"" <<
        fVoiceName << "\"" <<
        endl;
    }
#endif

    fVoiceRemainingRestMeasures--;

    // is this the last measure in the row?
    if (fVoiceRemainingRestMeasures == 0) {
      // yes, set waiting multiple rest's next measure number
#ifdef TRACE_OPTIONS
      if (gTraceOptions->fTraceRepeats) {
        gLogIOstream <<
          "Setting multiple rest next measure number to '" <<
          nextMeasureNumber <<
          "' in voice \"" <<
          fVoiceName << "\"" <<
          endl;
      }
#endif

      fVoiceMultipleRestWaitingForItsNextMeasureNumber->
        setMultipleRestNextMeasureNumber (
          nextMeasureNumber);

      // forget about this waiting multiple rest
      fVoiceMultipleRestWaitingForItsNextMeasureNumber = nullptr;
    }
  }

  gIndenter--;
}

void msrVoice::createNewLastSegmentForVoice (
  int inputLineNumber)
{
  // create the segment
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceSegments || gTraceOptions->fTraceVoices) {
    gLogIOstream <<
      "Creating a new segment for voice \"" <<
      getVoiceName () << "\"" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  fVoiceLastSegment =
    msrSegment::create (
      inputLineNumber,
      this);

  if (! fVoiceFirstSegment) {
    fVoiceFirstSegment = fVoiceLastSegment;
  }
}

void msrVoice::createNewLastSegmentFromFirstMeasureForVoice (
  int          inputLineNumber,
  S_msrMeasure firstMeasure)
{
  // create the segment
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceVoices) {
    gLogIOstream <<
      "Creating a new segment with first measure '" <<
      firstMeasure->getMeasureNumber () <<
      "' for voice \"" <<
      getVoiceName () << "\"" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  // create the last segment
  fVoiceLastSegment =
    msrSegment::create (
      inputLineNumber,
      this);

  if (! fVoiceFirstSegment) {
    fVoiceFirstSegment = fVoiceLastSegment;
  }
  // append firstMeasure to it
  fVoiceLastSegment->
    appendMeasureToSegment (firstMeasure);
}

void msrVoice::createNewLastSegmentAndANewMeasureBeforeARepeat (
  int inputLineNumber,
  int measureFullLength)
{
/*
  This method is used for repeats
  It may lead to several measures having the same number,
  in case a bar line cuts them apart
  If this measure remains empty because the end of measure
  follows the barline, it will be removed upon finalizeMeasure ()
*/

  // finalize the current measure
  finalizeCurrentMeasureInVoice (
    inputLineNumber);

  // create the segment
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceVoices) {
    gLogIOstream <<
      "Creating a new segment containing a new measure '" <<
      fVoiceCurrentMeasureNumber <<
      "'for voice \"" <<
      getVoiceName () << "\"" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  // create the new voice last segment
  fVoiceLastSegment =
    msrSegment::create (
      inputLineNumber,
      this);

  if (! fVoiceFirstSegment) {
    fVoiceFirstSegment = fVoiceLastSegment;
  }

  // create the new measure with number newMeasureMeasureNumber
  S_msrMeasure
    newMeasure =
      msrMeasure::create (
        inputLineNumber,
        fVoiceCurrentMeasureNumber,
        fVoiceLastSegment);

  // set it's full measure length
  newMeasure->setMeasureFullLength (
    measureFullLength);
    
  // set it as being created before a repeat
  newMeasure->
    setMeasureCreatedForARepeatKind (
      msrMeasure::kMeasureCreatedForARepeatBefore);
    
  // append new measure to new voice last segment
  fVoiceLastSegment->
    appendMeasureToSegment (newMeasure);
}

void msrVoice::createNewLastSegmentAndANewMeasureAfterARepeat (
  int inputLineNumber,
  int measureFullLength)
{
/*
  This method is used for repeats
  It may lead to several measures having the same number,
  in case a bar line cuts them apart
  If this measure remains empty because the end of measure
  follows the barline, it will be removed upon finalizeMeasure ()
*/

  // finalize the current measure
  finalizeCurrentMeasureInVoice (
    inputLineNumber);

  // create the segment
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceVoices) {
    gLogIOstream <<
      "Creating a new segment containing a new measure '" <<
      fVoiceCurrentMeasureNumber <<
      "'for voice \"" <<
      getVoiceName () << "\"" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  // create the new voice last segment
  fVoiceLastSegment =
    msrSegment::create (
      inputLineNumber,
      this);

  if (! fVoiceFirstSegment) {
    fVoiceFirstSegment = fVoiceLastSegment;
  }

  // create the new measure with number newMeasureMeasureNumber
  S_msrMeasure
    newMeasure =
      msrMeasure::create (
        inputLineNumber,
        fVoiceCurrentMeasureNumber,
        fVoiceLastSegment);

  // set it's full measure length
  newMeasure->setMeasureFullLength (
    measureFullLength);
    
  // set it as being created after a repeat
  newMeasure->
    setMeasureCreatedForARepeatKind (
      msrMeasure::kMeasureCreatedForARepeatAfter);
    
  // append new measure to new voice last segment
  fVoiceLastSegment->
    appendMeasureToSegment (newMeasure);
}

S_msrVoice msrVoice::createHarmonyVoiceForRegularVoice (
  int    inputLineNumber,
  string currentMeasureNumber)
{
  if (fHarmonyVoiceForRegularVoice) {
    stringstream s;

    s <<
      "Voice \"" <<
      getVoiceName () <<
      "\" already has a harmony voice";

    msrInternalError (
      gXml2lyOptions->fInputSourceName,
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  }

  // create the voice harmony voice
  int harmonyVoiceForRegularVoiceNumber =
    K_VOICE_HARMONY_VOICE_BASE_NUMBER + fVoiceNumber;
    
#ifdef TRACE_OPTIONS
  if (
    gTraceOptions->fTraceHarmonies
      ||
    gTraceOptions->fTraceVoices
      ||
    gTraceOptions->fTraceStaves) {
    gLogIOstream <<
      "Creating harmony voice for regular voice \"" << getVoiceName () << "\"" <<
      " with voice number " << harmonyVoiceForRegularVoiceNumber <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  fHarmonyVoiceForRegularVoice =
    msrVoice::create (
      inputLineNumber,
      msrVoice::kHarmonyVoice,
      harmonyVoiceForRegularVoiceNumber,
      msrVoice::kCreateInitialLastSegmentYes,
      fVoiceStaffUplink);

  // register it in the staff
  fVoiceStaffUplink->
    registerVoiceInStaff (
      inputLineNumber,
      fHarmonyVoiceForRegularVoice);

  // set back link
  fHarmonyVoiceForRegularVoice->
    fRegularVoiceForHarmonyVoice = this;

  return fHarmonyVoiceForRegularVoice;
}

S_msrStanza msrVoice::addStanzaToVoiceByItsNumber (
  int    inputLineNumber,
  string stanzaNumber)
{
  if (fVoiceStanzasMap.count (stanzaNumber)) {
    stringstream s;
    
    s <<
      "stanza " << stanzaNumber <<
      " already exists in voice \"" <<
      getVoiceName () << "\"" <<
      ", line " << inputLineNumber <<
      endl;

    msrInternalError (
      gXml2lyOptions->fInputSourceName,
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  }

  // create the stanza
  S_msrStanza
    stanza =
      msrStanza::create (
        inputLineNumber,
        stanzaNumber,
        this);

  // add the stanza to this voice
  addStanzaToVoice (stanza);

  // return it
  return stanza;
}

void msrVoice::addStanzaToVoice (S_msrStanza stanza)
{
  // get stanza number
  string stanzaNumber =
    stanza->getStanzaNumber ();
    
  // register stanza in this voice
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceLyrics) {
    gLogIOstream <<
      "Adding stanza " << stanza->getStanzaName () <<
      " (" << stanzaNumber <<
      ") to voice \"" << getVoiceName () << "\"" <<
      endl;
  }
#endif

  // add the stanza to this voice
  fVoiceStanzasMap [stanzaNumber] = stanza;
}

void msrVoice::addStanzaToVoiceWithoutCatchUp (S_msrStanza stanza)
{
  // get stanza number
  string stanzaNumber =
    stanza->getStanzaNumber ();
    
  // register stanza in this voice
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceLyrics) {
    gLogIOstream <<
      "Adding stanza " << stanza->getStanzaName () <<
      " (" << stanzaNumber <<
      ") to voice \"" << getVoiceName () << "\"" <<
      endl;
  }
#endif

  // add the stanza to this voice
  fVoiceStanzasMap [stanzaNumber] = stanza;
}

S_msrStanza msrVoice::createStanzaInVoiceIfNotYetDone (
  int    inputLineNumber,
  string stanzaNumber,
  string stanzaName) // JMI
{
  S_msrStanza stanza;

  // is stanzaNumber already known in voice?
  if (fVoiceStanzasMap.count (stanzaNumber)) {
    // yes, use it
    stanza =
      fVoiceStanzasMap [stanzaNumber];
  }
  
  else {
    // no, create it and add it to the voice
#ifdef TRACE_OPTIONS
    if (gTraceOptions->fTraceVoices || gTraceOptions->fTraceLyrics) {
      gLogIOstream <<
        "Creating stanza" <<
        " number " << stanzaNumber <<
        ", name \"" << stanzaName << "\"" <<
        ", in voice \"" << getVoiceName () << "\"" <<
        ", line " << inputLineNumber <<
        ", fVoiceStanzasMap.size () = " << fVoiceStanzasMap.size () <<
        endl;
    }
#endif

    stanza =
      addStanzaToVoiceByItsNumber (
        inputLineNumber, stanzaNumber);
  }

  return stanza;
}

S_msrStanza msrVoice::fetchStanzaInVoice (
  int    inputLineNumber,
  string stanzaNumber,
  string stanzaName) // JMI
{
  S_msrStanza stanza;

  // is stanzaNumber known in voice?
  if (fVoiceStanzasMap.count (stanzaNumber)) {
    // yes, use it
    stanza =
      fVoiceStanzasMap [stanzaNumber];
  }
  
  else {
    stringstream s;
    
    gLogIOstream <<
      endl << endl << endl <<
      "VOICEVOICEVOICEVOICEVOICEVOICEVOICEVOICE" <<
      endl;

    print (gLogIOstream);
      
    gLogIOstream <<
      "VOICEVOICEVOICEVOICEVOICEVOICEVOICEVOICE" <<
      endl << endl << endl <<
      endl;


    s <<
      "Stanza" <<
      " number " << stanzaNumber <<
      ", name \"" << stanzaName << "\"" <<
      ", not found in voice \"" << getVoiceName () << "\"" <<
      ", line " << inputLineNumber <<
      ", fVoiceStanzasMap.size () = " << fVoiceStanzasMap.size () <<
      endl;

    msrInternalError (
      gXml2lyOptions->fInputSourceName,
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  }

  return stanza;
}

void msrVoice::appendClefToVoice (S_msrClef clef)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceClefs || gTraceOptions->fTraceVoices) {
    gLogIOstream <<
      "Appending clef '" << clef->asString () <<
      "' to voice \"" << getVoiceName () << "\"" <<
      endl;
  }
#endif

  gIndenter++;
  
  // create the voice last segment and first measure if needed
  appendAFirstMeasureToVoiceIfNotYetDone (
    clef->getInputLineNumber ());
  
  if (fMusicHasBeenInsertedInVoice) {
    // append clef to last segment
    fVoiceLastSegment->
      appendClefToSegment (clef);
  }

  else {
    // moving clefs to the left, thus prepend to last segment
    fVoiceLastSegment->
      prependClefToSegment (clef);
  }

  gIndenter--;
}

void msrVoice::appendKeyToVoice (S_msrKey key)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceKeys || gTraceOptions->fTraceVoices) {
    gLogIOstream <<
      "Appending key '" << key->asString () <<
      "' to voice \"" << getVoiceName () << "\"" <<
      endl;
  }
#endif

  gIndenter++;
  
  // create the voice last segment and first measure if needed
  appendAFirstMeasureToVoiceIfNotYetDone (
    key->getInputLineNumber ());

  // append key to last segment
  fVoiceLastSegment->
    appendKeyToSegment (key);

  gIndenter--;
}

void msrVoice::appendTimeToVoice (S_msrTime time)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceTimes || gTraceOptions->fTraceVoices) {
    gLogIOstream <<
      "Appending time '" << time->asString () <<
      "' to voice \"" << getVoiceName () << "\"" <<
      endl;
  }
#endif

  gIndenter++;
  
  // create the voice last segment and first measure if needed
  appendAFirstMeasureToVoiceIfNotYetDone (
    time->getInputLineNumber ());

  // append time to the last segment
  fVoiceLastSegment->
    appendTimeToSegment (time);

  gIndenter--;
}

void msrVoice::appendTimeToVoiceClone (S_msrTime time)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceTimes || gTraceOptions->fTraceVoices) {
    gLogIOstream <<
      "Appending time '" << time->asString () <<
      "' to voice clone \"" << getVoiceName () << "\"" <<
      endl;
  }
#endif

  gIndenter++;
  
  // create the voice last segment and first measure if needed
  appendAFirstMeasureToVoiceIfNotYetDone (
    time->getInputLineNumber ());

  // append time to the last segment
  fVoiceLastSegment->
    appendTimeToSegmentClone (time);

  gIndenter--;
}

void msrVoice::appendHarmonyToVoice (S_msrHarmony harmony)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceHarmonies || gTraceOptions->fTraceVoices) {
    gLogIOstream <<
      "Appending harmony '" << harmony->asString () <<
      "' to voice \"" << getVoiceName () << "\"" <<
      endl;
  }
#endif

  int inputLineNumber =
    harmony->getInputLineNumber ();
    
  switch (fVoiceKind) {
    case msrVoice::kHarmonyVoice:
      // create the voice last segment and first measure if needed
      appendAFirstMeasureToVoiceIfNotYetDone (
        inputLineNumber);

      // skip to harmony note position in the voice
      padUpToMeasureLengthInVoice (
        inputLineNumber,
        harmony->
          getHarmonyNoteUplink ()->
            getNotePositionInMeasure ());

      // append the harmony to the voice last segment
      fVoiceLastSegment->
        appendHarmonyToSegment (harmony);
    
      // register harmony
      fVoiceActualHarmoniesCounter++;
      fMusicHasBeenInsertedInVoice = true;

      break;
      
    case msrVoice::kRegularVoice:
    case msrVoice::kFiguredBassVoice:
      {
        stringstream s;

        s <<
          "cannot append a harmony to " <<
          voiceKindAsString () <<
          " voice \"" <<
          getVoiceName () <<
          "\"";

        msrInternalError (
          gXml2lyOptions->fInputSourceName,
          inputLineNumber,
          __FILE__, __LINE__,
          s.str ());
      }
      break;
  } // switch
}

void msrVoice::appendHarmonyToVoiceClone (S_msrHarmony harmony)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceHarmonies || gTraceOptions->fTraceVoices) {
    gLogIOstream <<
      "Appending harmony '" << harmony->asString () <<
      "' to voice clone \"" << getVoiceName () << "\"" <<
      endl;
  }
#endif
      
  int inputLineNumber =
    harmony->getInputLineNumber ();
    
  switch (fVoiceKind) {
    case msrVoice::kHarmonyVoice:
   // /* JMI DON'T
      // create the voice last segment and first measure if needed
      appendAFirstMeasureToVoiceIfNotYetDone (
        inputLineNumber);
//*/

      fVoiceLastSegment->
        appendHarmonyToSegmentClone (harmony);
    
      // register harmony
      fVoiceActualHarmoniesCounter++;
      fMusicHasBeenInsertedInVoice = true;
      break;
      
    case msrVoice::kRegularVoice:
    case msrVoice::kFiguredBassVoice:
      {
        stringstream s;

        s <<
          "cannot append a harmony to " <<
          voiceKindAsString () <<
          " voice clone \"" <<
          getVoiceName () <<
          "\"";

        msrInternalError (
          gXml2lyOptions->fInputSourceName,
          inputLineNumber,
          __FILE__, __LINE__,
          s.str ());
      }
      break;
  } // switch
}

void msrVoice::appendFiguredBassToVoice (
  S_msrFiguredBass figuredBass)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceFiguredBass || gTraceOptions->fTraceVoices) {
    gLogIOstream <<
      "Appending figured bass '" << figuredBass->asString () <<
      "' to voice \"" << getVoiceName () << "\"" <<
      endl;
  }
#endif

  switch (fVoiceKind) {
    case msrVoice::kFiguredBassVoice:
      // create the voice last segment and first measure if needed
      appendAFirstMeasureToVoiceIfNotYetDone (
        figuredBass->getInputLineNumber ());

      fVoiceLastSegment->
        appendFiguredBassToSegment (figuredBass);
    
      // register harmony
      fVoiceActualFiguredBassCounter++;
      fMusicHasBeenInsertedInVoice = true;
      break;

    case msrVoice::kRegularVoice:
    case msrVoice::kHarmonyVoice:
      {
        stringstream s;

        s <<
          "cannot append a figured bass to " <<
          voiceKindAsString () <<
          " voice \"" <<
          getVoiceName () <<
          "\"";

        msrInternalError (
          gXml2lyOptions->fInputSourceName,
          figuredBass->getInputLineNumber (),
          __FILE__, __LINE__,
          s.str ());
      }
      break;
  } // switch
}

void msrVoice::appendFiguredBassToVoiceClone (
  S_msrFiguredBass figuredBass)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceFiguredBass || gTraceOptions->fTraceVoices) {
    gLogIOstream <<
      "Appending figured bass '" << figuredBass->asString () <<
      "' to voice clone \"" << getVoiceName () << "\"" <<
      endl;
  }
#endif
      
  switch (fVoiceKind) {
    case msrVoice::kFiguredBassVoice:
   // /* JMI DON'T
      // create the voice last segment and first measure if needed
      appendAFirstMeasureToVoiceIfNotYetDone (
        figuredBass->getInputLineNumber ());
//*/

      fVoiceLastSegment->
        appendFiguredBassToSegmentClone (figuredBass);
    
      // register figured bass
      fVoiceActualFiguredBassCounter++;
      fMusicHasBeenInsertedInVoice = true;
      break;
      
    case msrVoice::kRegularVoice:
    case msrVoice::kHarmonyVoice:
      {
        stringstream s;

        s <<
          "cannot append a figured bass to " <<
          voiceKindAsString () <<
          " voice clone \"" <<
          getVoiceName () <<
          "\"";

        msrInternalError (
          gXml2lyOptions->fInputSourceName,
          figuredBass->getInputLineNumber (),
          __FILE__, __LINE__,
          s.str ());
      }
      break;
  } // switch
}

void msrVoice::padUpToMeasureLengthInVoice (
  int      inputLineNumber,
  rational measureLength)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceVoices || gTraceOptions->fTraceMeasures) {
    gLogIOstream <<
      "Padding up to measure length '" << measureLength <<
      "' in voice \"" <<
      getVoiceName () <<
      "\", line " << inputLineNumber <<
      endl;
  }
#endif

  gIndenter++;
  
  // pad up the voice's last segment
  fVoiceLastSegment->
    padUpToMeasureLengthInSegment (
      inputLineNumber, measureLength);
      
  // pad up the voice's stanzas
  if (fVoiceStanzasMap.size ()) {
    for (
      map<string, S_msrStanza>::const_iterator i = fVoiceStanzasMap.begin ();
      i != fVoiceStanzasMap.end ();
      i++) {
      S_msrStanza stanza = (*i).second;

      stanza->padUpToMeasureLengthInStanza (
        inputLineNumber, measureLength);
    } // for
  }

  gIndenter--;
}

void msrVoice::appendTransposeToVoice (
  S_msrTranspose transpose)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceTranspositions || gTraceOptions->fTraceVoices) {
    gLogIOstream <<
      "Appending transpose '" <<
      transpose->asString () <<
      "' to voice \"" << getVoiceName () << "\"" <<
      endl;
  }
#endif

  // create the voice last segment and first measure if needed
  appendAFirstMeasureToVoiceIfNotYetDone (
    transpose->getInputLineNumber ());

  fVoiceLastSegment->
    appendTransposeToSegment (transpose);
}

void msrVoice::appendPartNameDisplayToVoice (
  S_msrPartNameDisplay partNameDisplay)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceTranspositions || gTraceOptions->fTraceVoices) {
    gLogIOstream <<
      "Appending part name display '" <<
      partNameDisplay->asString () <<
      "' to voice \"" << getVoiceName () << "\"" <<
      endl;
  }
#endif

  // create the voice last segment and first measure if needed
  appendAFirstMeasureToVoiceIfNotYetDone (
    partNameDisplay->getInputLineNumber ());

  fVoiceLastSegment->
    appendPartNameDisplayToSegment (partNameDisplay);
}

void msrVoice::appendPartAbbreviationDisplayToVoice (
  S_msrPartAbbreviationDisplay partAbbreviationDisplay)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceTranspositions || gTraceOptions->fTraceVoices) {
    gLogIOstream <<
      "Appending part abbreviation display '" <<
      partAbbreviationDisplay->asString () <<
      "' to voice \"" << getVoiceName () << "\"" <<
      endl;
  }
#endif

  // create the voice last segment and first measure if needed
  appendAFirstMeasureToVoiceIfNotYetDone (
    partAbbreviationDisplay->getInputLineNumber ());

  fVoiceLastSegment->
    appendPartAbbreviationDisplayToSegment (partAbbreviationDisplay);
}

void msrVoice::appendStaffDetailsToVoice (
  S_msrStaffDetails staffDetails)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceStaves || gTraceOptions->fTraceVoices) {
    gLogIOstream <<
      "Appending staff details '" <<
      staffDetails->asShortString () <<
      "' to voice \"" << getVoiceName () << "\"" <<
      endl;
  }
#endif

/* JMI
  // register staff details in voice
  fCurrentVoicefStaffDetails = staffDetails;
  */
  
 // create the voice last segment and first measure if needed
  appendAFirstMeasureToVoiceIfNotYetDone (
    staffDetails->getInputLineNumber ());

  if (fVoiceLastSegment) fVoiceLastSegment->appendStaffDetailsToSegment (staffDetails);
}

/* JMI
void msrVoice::appendWordsToVoice (S_msrWords words)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceWords) {
    gLogIOstream <<
      "Appending words '" << words->asString () <<
      "' to voice " << getVoiceName () << endl;
  }
#endif

  S_msrElement w = words;
  fVoiceLastSegment->
    appendWordsToSegment (w);
}
*/

void msrVoice::appendTempoToVoice (S_msrTempo tempo)
{
  if (gMsrOptions->fTraceMsr) {
    gLogIOstream <<
      "Appending tempo '" << tempo->asString () <<
      "' to voice \"" << getVoiceName () << "\"" <<
      endl;
  }

  // create the voice last segment and first measure if needed
  appendAFirstMeasureToVoiceIfNotYetDone (
    tempo->getInputLineNumber ());

  fVoiceLastSegment->
    appendTempoToSegment (tempo);
}

void msrVoice::appendOctaveShiftToVoice (
  S_msrOctaveShift octaveShift)
{
  if (gMsrOptions->fTraceMsr) {
    gLogIOstream <<
      "Appending octave shift '" <<
      octaveShift->octaveShiftKindAsString () <<
      "', size: " << octaveShift->getOctaveShiftSize () <<
      "' to voice \"" << getVoiceName () << "\"" <<
      endl;
  }

  // create the voice last segment and first measure if needed
  appendAFirstMeasureToVoiceIfNotYetDone (
    octaveShift->getInputLineNumber ());

  fVoiceLastSegment->
    appendOctaveShiftToSegment (octaveShift);
}

void msrVoice::appendScordaturaToVoice (
  S_msrScordatura scordatura)
{
  if (gMsrOptions->fTraceMsr) {
    gLogIOstream <<
      "Appending scordatura '" <<
      scordatura->asString () <<
      "' to voice \"" << getVoiceName () << "\"" <<
      endl;
  }

  // create the voice last segment and first measure if needed
  appendAFirstMeasureToVoiceIfNotYetDone (
    scordatura->getInputLineNumber ());

  fVoiceLastSegment->
    appendScordaturaToSegment (scordatura);
}

void msrVoice::appendAccordionRegistrationToVoice (
  S_msrAccordionRegistration
    accordionRegistration)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTracePasses || gTraceOptions->fTraceVoices) {
    gLogIOstream <<
      "Appending accordion registration '" <<
      accordionRegistration->asString () <<
      "' to voice \"" << getVoiceName () << "\"" <<
      endl;
  }
#endif

  // create the voice last segment and first measure if needed
  appendAFirstMeasureToVoiceIfNotYetDone (
    accordionRegistration->getInputLineNumber ());

  fVoiceLastSegment->
    appendAccordionRegistrationToSegment (
      accordionRegistration);
}

void msrVoice::appendHarpPedalsTuningToVoice (
  S_msrHarpPedalsTuning
    harpPedalsTuning)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTracePasses || gTraceOptions->fTraceVoices) {
    gLogIOstream <<
      "Appending harp pedals tuning '" <<
      harpPedalsTuning->asString () <<
      "' to voice \"" << getVoiceName () << "\"" <<
      endl;
  }
#endif

  // create the voice last segment and first measure if needed
  appendAFirstMeasureToVoiceIfNotYetDone (
    harpPedalsTuning->getInputLineNumber ());

  fVoiceLastSegment->
    appendHarpPedalsTuningToSegment (
      harpPedalsTuning);
}

void msrVoice::appendRehearsalToVoice (S_msrRehearsal rehearsal)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceRepeats) {
    gLogIOstream <<
      "Appending rehearsal '" << rehearsal->getRehearsalText () <<
      "' to voice \"" << getVoiceName () << "\"" <<
      endl;
  }
#endif

  // create the voice last segment and first measure if needed
  appendAFirstMeasureToVoiceIfNotYetDone (
    rehearsal->getInputLineNumber ());

  fVoiceLastSegment->
    appendRehearsalToSegment (rehearsal);
}

void msrVoice::appendVoiceStaffChangeToVoice (
  S_msrVoiceStaffChange voiceStaffChange)
{
  int inputLineNumber =
    voiceStaffChange->getInputLineNumber ();
    
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceVoices || gTraceOptions->fTraceStaves) {
    gLogIOstream <<
      "Appending voice staff change '" <<
      voiceStaffChange->asString () <<
      "' to voice " <<
      " \"" << getVoiceName () << "\"" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  // create the voice last segment and first measure if needed
  appendAFirstMeasureToVoiceIfNotYetDone (
    inputLineNumber);

  fVoiceLastSegment->
    appendVoiceStaffChangeToSegment (
      voiceStaffChange);
}

void msrVoice::appendNoteToVoice (S_msrNote note) {
  int inputLineNumber =
    note->getInputLineNumber ();
    
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceNotes || gTraceOptions->fTraceVoices) {
    gLogIOstream <<
      "Appending note '" <<
      endl;

    gIndenter++;
    
    gLogIOstream <<
      note <<
        "' to voice \"" << getVoiceName () << "\"" <<
        ", line " << inputLineNumber <<
        endl;

    gIndenter--;
  }
#endif

  // register whether music (i.e. not just skips)
  // has been inserted into the voice
  switch (note->getNoteKind ()) {
    case msrNote::k_NoNoteKind:
      break;

    case msrNote::kRestNote:
      // register rest
      fVoiceRestsCounter++;
      fMusicHasBeenInsertedInVoice = true;
      break;
      
    case msrNote::kSkipNote:
      // don't account skips as music
      fVoiceSkipsCounter++;
      break;
      
    case msrNote::kUnpitchedNote:
      // register actual note
      fVoiceActualNotesCounter++;
      fMusicHasBeenInsertedInVoice = true;
      break;
      
    case msrNote::kStandaloneNote:
      // register actual note
      fVoiceActualNotesCounter++;
      fMusicHasBeenInsertedInVoice = true;
      break;
      
    case msrNote::kDoubleTremoloMemberNote:
      // register actual note
      fVoiceActualNotesCounter++;
      fMusicHasBeenInsertedInVoice = true;
      break;
      
    case msrNote::kGraceNote:
    case msrNote::kGraceChordMemberNote:
      // register actual note
      fVoiceActualNotesCounter++;
      fMusicHasBeenInsertedInVoice = true;
      break;
      
    case msrNote::kChordMemberNote:
      // register actual note
      fVoiceActualNotesCounter++;
      fMusicHasBeenInsertedInVoice = true;
      break;
      
    case msrNote::kTupletMemberNote:
    case msrNote::kTupletMemberUnpitchedNote:
      // register actual note
      fVoiceActualNotesCounter++;
      fMusicHasBeenInsertedInVoice = true;
      break;
  } // switch
  
  // is this note the shortest one in this voice?
  rational
    noteSoundingWholeNotes =
      note->getNoteSoundingWholeNotes (),
    noteDisplayWholeNotes =
      note->getNoteDisplayWholeNotes (); // JMI
      
  if (noteSoundingWholeNotes < fVoiceShortestNoteDuration) {
    fVoiceShortestNoteDuration = noteSoundingWholeNotes;
  }
  if (noteDisplayWholeNotes < fVoiceShortestNoteDuration) {
    fVoiceShortestNoteDuration = noteDisplayWholeNotes;
  }

  // create the voice last segment and first measure if needed
  appendAFirstMeasureToVoiceIfNotYetDone (
    inputLineNumber);

  // append the note to the last segment
  fVoiceLastSegment->
    appendNoteToSegment (note);

  // register note as the last appended one into this voice
  fVoiceLastAppendedNote = note;
}

void msrVoice::appendNoteToVoiceClone (S_msrNote note) {
  int inputLineNumber =
    note->getInputLineNumber ();
    
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceNotes || gTraceOptions->fTraceVoices) {
    gLogIOstream <<
      "Appending note '" <<
      endl;

    gIndenter++;
    
    gLogIOstream <<
      note <<
        "' to voice clone \"" << getVoiceName () << "\"" <<
        ", line " << inputLineNumber <<
        endl;

    gIndenter--;
  }
#endif

  // register whether music (i.e. not just skips)
  // has been inserted into the voice
  switch (note->getNoteKind ()) {
    case msrNote::k_NoNoteKind:
      break;

    case msrNote::kRestNote:
      // register rest
      fVoiceRestsCounter++;
      fMusicHasBeenInsertedInVoice = true;
      break;
      
    case msrNote::kSkipNote:
      // don't account skips as music
      fVoiceSkipsCounter++;
      break;
      
    case msrNote::kUnpitchedNote:
      // register actual note
      fVoiceActualNotesCounter++;
      fMusicHasBeenInsertedInVoice = true;
      break;
      
    case msrNote::kStandaloneNote:
      // register actual note
      fVoiceActualNotesCounter++;
      fMusicHasBeenInsertedInVoice = true;
      break;
      
    case msrNote::kDoubleTremoloMemberNote:
      // register actual note
      fVoiceActualNotesCounter++;
      fMusicHasBeenInsertedInVoice = true;
      break;
      
    case msrNote::kGraceNote:
    case msrNote::kGraceChordMemberNote:
      // register actual note
      fVoiceActualNotesCounter++;
      fMusicHasBeenInsertedInVoice = true;
      break;
      
    case msrNote::kChordMemberNote:
      // register actual note
      fVoiceActualNotesCounter++;
      fMusicHasBeenInsertedInVoice = true;
      break;
      
    case msrNote::kTupletMemberNote:
    case msrNote::kTupletMemberUnpitchedNote:
      // register actual note
      fVoiceActualNotesCounter++;
      fMusicHasBeenInsertedInVoice = true;
      break;
  } // switch
  
  // append the note to the last segment
  fVoiceLastSegment->
    appendNoteToSegmentClone (note);

  // is this note the shortest one in this voice?
  rational
    noteSoundingWholeNotes =
      note->getNoteSoundingWholeNotes (),
    noteDisplayWholeNotes =
      note->getNoteDisplayWholeNotes (); // JMI
      
  if (noteSoundingWholeNotes < fVoiceShortestNoteDuration) {
    fVoiceShortestNoteDuration = noteSoundingWholeNotes;
  }
  if (noteDisplayWholeNotes < fVoiceShortestNoteDuration) {
    fVoiceShortestNoteDuration = noteDisplayWholeNotes;
  }

  // register note as the last appended one into this voice
  fVoiceLastAppendedNote = note;
}

void msrVoice::appendDoubleTremoloToVoice (
  S_msrDoubleTremolo doubleTremolo)

{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceTremolos) {
    gLogIOstream <<
      "Appending double tremolo '" <<
      doubleTremolo->asShortString () <<
      "' to voice \"" << getVoiceName () << "\"" <<
      endl;
  }
#endif

  // create the voice last segment and first measure if needed
  appendAFirstMeasureToVoiceIfNotYetDone (
    doubleTremolo->getInputLineNumber ());

  fVoiceLastSegment->
    appendDoubleTremoloToSegment (doubleTremolo);

  fMusicHasBeenInsertedInVoice = true;
}

void msrVoice::appendChordToVoice (S_msrChord chord)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceChords) {
    gLogIOstream <<
      "Appending chord '" << chord->asString () <<
      "' to voice \"" << getVoiceName () << "\"" <<
      endl;
  }
#endif

  // create the voice last segment and first measure if needed
  appendAFirstMeasureToVoiceIfNotYetDone (
    chord->getInputLineNumber ());

  fVoiceLastSegment->
    appendChordToSegment (chord);

  fMusicHasBeenInsertedInVoice = true;
}

void msrVoice::appendTupletToVoice (S_msrTuplet tuplet)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceTuplets) {
    gLogIOstream <<
      "Appending tuplet '" << tuplet->asString () <<
      "' to voice \"" << getVoiceName () << "\"" <<
      endl;
  }
#endif

  // create the voice last segment and first measure if needed
  appendAFirstMeasureToVoiceIfNotYetDone (
    tuplet->getInputLineNumber ());

  fVoiceLastSegment->
    appendTupletToSegment (tuplet);

  fMusicHasBeenInsertedInVoice = true;
}

void msrVoice::appendGraceNotesToVoice (S_msrGraceNotes graceNotes)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceGraceNotes) {
    gLogIOstream <<
      "Appending grace notes " << // JMI graceNotes <<
      " to voice \"" << getVoiceName () << "\"" <<
      endl;
  }
#endif

  // create the voice last segment and first measure if needed
  appendAFirstMeasureToVoiceIfNotYetDone (
    graceNotes->getInputLineNumber ());

  fVoiceLastSegment->
    appendGraceNotesToSegment (graceNotes);

  fMusicHasBeenInsertedInVoice = true;
}

void msrVoice::prependGraceNotesToVoice (S_msrGraceNotes graceNotes)
{
  int inputLineNumber =
    graceNotes->getInputLineNumber ();
  
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceGraceNotes) {
    gLogIOstream <<
      "Prepending grace notes '" <<
      graceNotes->asString () <<
      "' to voice \"" << getVoiceName () << "\"" <<
      endl;
  }
#endif

/* JMI
  gLogIOstream <<
    endl <<
    "======================= prependGraceNotesToVoice" <<
    endl;
  this->print (gLogIOstream);
  gLogIOstream <<
    "=======================" <<
    endl <<
    endl;
  */

  // create the voice last segment and first measure if needed
  appendAFirstMeasureToVoiceIfNotYetDone (
    graceNotes->getInputLineNumber ());

  if (! fVoiceFirstSegment) {
    // these graceNotes appear at the beginning of the voice:
    // create a first segment
    createNewLastSegmentForVoice (
      inputLineNumber);
    fVoiceFirstSegment = fVoiceLastSegment;

    // the create the first measure
    createMeasureAndAppendItToVoice (
      inputLineNumber,
      graceNotes->
        getGraceNotesMeasureNumber (),
      1, //    measureOrdinalNumber,
      msrMeasure::kMeasureImplicitNo);
  }
  
  fVoiceFirstSegment->
    prependGraceNotesToSegment (graceNotes);

  fMusicHasBeenInsertedInVoice = true;
}

void msrVoice::appendAfterGraceNotesToVoice (
  S_msrAfterGraceNotes afterGraceNotes)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceGraceNotes) {
    gLogIOstream <<
      "Appending after grace notes " << // JMI AfterGraceNotes <<
      " to voice \"" << getVoiceName () << "\"" <<
      endl;
  }
#endif

  // create the voice last segment and first measure if needed
  appendAFirstMeasureToVoiceIfNotYetDone (
    afterGraceNotes->getInputLineNumber ());

  fVoiceLastSegment->
    appendAfterGraceNotesToSegment (afterGraceNotes);

  fMusicHasBeenInsertedInVoice = true;
}

void msrVoice::prependAfterGraceNotesToVoice (
  S_msrAfterGraceNotes afterGraceNotes)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceGraceNotes) {
    gLogIOstream <<
      "Prepending after grace notes " << // JMI AfterGraceNotes <<
      " to voice \"" << getVoiceName () << "\"" <<
      endl;
  }
#endif

  // create the voice last segment and first measure if needed
  appendAFirstMeasureToVoiceIfNotYetDone (
    afterGraceNotes->getInputLineNumber ());

  fVoiceFirstSegment->
    prependAfterGraceNotesToSegment (afterGraceNotes);

  fMusicHasBeenInsertedInVoice = true;
}

void msrVoice::appendSyllableToVoice (
  int           inputLineNumber,
  string        stanzaNumber,
  string        stanzaName,
  S_msrSyllable syllable)
{
  // append syllable to this voice
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceLyrics) {
    gLogIOstream <<
      "Appending syllable '" <<
      syllable->asString () <<
      "' to voice \"" << fVoiceName << "\"" <<
      endl;
  }
#endif

  // fetch stanzaNumber in this voice
  S_msrStanza
    stanza =
      createStanzaInVoiceIfNotYetDone (
        inputLineNumber,
        stanzaNumber,
        stanzaName);

  // add the syllable to the stanza
  stanza->
    appendSyllableToStanza (
      syllable);
}

void msrVoice::appendBarCheckToVoice (S_msrBarCheck barCheck)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceMeasures) {
    gLogIOstream <<
      "Appending bar check '" << barCheck->asString () <<
      "' to voice \"" << getVoiceName () <<  "\"" <<
      endl;
  }
#endif

  // create the voice last segment and first measure if needed
  appendAFirstMeasureToVoiceIfNotYetDone (
    barCheck->getInputLineNumber ());

  fVoiceLastSegment->
    appendBarCheckToSegment (barCheck);
}

void msrVoice::appendBarNumberCheckToVoice (
  S_msrBarNumberCheck barNumberCheck)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceMeasures) {
    gLogIOstream <<
      "Appending barnumber check '" <<
      barNumberCheck->asString () <<
      "' to voice \"" << getVoiceName () <<  "\"" <<
      endl;
  }
#endif

  // create the voice last segment and first measure if needed
  appendAFirstMeasureToVoiceIfNotYetDone (
    barNumberCheck->getInputLineNumber ());

  fVoiceLastSegment->
    appendBarNumberCheckToSegment (barNumberCheck);
}

void msrVoice::appendLineBreakToVoice (S_msrLineBreak lineBreak)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceMeasures) {
    gLogIOstream <<
      "Appending line break '" << lineBreak->asString () <<
      "' to voice \"" << getVoiceName () << "\"" <<
      endl;
  }
#endif

  // create the voice last segment and first measure if needed
  appendAFirstMeasureToVoiceIfNotYetDone (
    lineBreak->getInputLineNumber ());

  fVoiceLastSegment->
    appendLineBreakToSegment (lineBreak);

  // propagate this lineBreak to the voice stanzas if any
  if (fVoiceStanzasMap.size ()) {
    for (
      map<string, S_msrStanza>::const_iterator i = fVoiceStanzasMap.begin ();
      i != fVoiceStanzasMap.end ();
      i++) {
      S_msrStanza stanza = (*i).second;

      stanza->appendLineBreakSyllableToStanza (
        lineBreak->getInputLineNumber ());
    } // for
  }
}

void msrVoice::appendPageBreakToVoice (S_msrPageBreak pageBreak)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceMeasures) {
    gLogIOstream <<
      "Appending page break '" << pageBreak->asString () <<
      "' to voice \"" << getVoiceName () << "\"" <<
      endl;
  }
#endif

  // create the voice last segment and first measure if needed
  appendAFirstMeasureToVoiceIfNotYetDone (
    pageBreak->getInputLineNumber ());

  fVoiceLastSegment->
    appendPageBreakToSegment (pageBreak);
}

void msrVoice::prependOtherElementToVoice (S_msrElement elem) {
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceVoices) {
    gLogIOstream <<
      "Prepending other element '" << elem <<
      "' to voice \"" << getVoiceName () << "\"" <<
      endl;
  }
#endif

  // create the voice last segment and first measure if needed
  appendAFirstMeasureToVoiceIfNotYetDone (
    elem->getInputLineNumber ());

  fVoiceLastSegment->
    prependOtherElementToSegment (elem);
}

void msrVoice::appendOtherElementToVoice (S_msrElement elem) {
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceVoices) {
    gLogIOstream <<
      "Appending other element '" << elem <<
      "' to voice \"" << getVoiceName () << "\"" <<
      endl;
  }
#endif

  // create the voice last segment and first measure if needed
  appendAFirstMeasureToVoiceIfNotYetDone (
    elem->getInputLineNumber ());

  fVoiceLastSegment->
    appendOtherElementToSegment (elem);
}

S_msrMeasure msrVoice::fetchVoiceLastMeasure (
  int inputLineNumber) const
{
  S_msrMeasure result;
  
  msrAssert (
    fVoiceLastSegment != 0,
    "fVoiceLastSegment is null");

  const list<S_msrMeasure>&
    lastSegmentMeasuresList =
      fVoiceLastSegment->getSegmentMeasuresList ();
      
  if (lastSegmentMeasuresList.size ()) {
    result = lastSegmentMeasuresList.back ();
  }
  else {
    stringstream s;

    s <<
      "attempting to fetch voice last measure in an empty measures list";

    msrInternalError (
      gXml2lyOptions->fInputSourceName,
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  }

  return result;
}

S_msrElement msrVoice::fetchVoiceLastElement (
  int inputLineNumber) const
{
  S_msrElement result;
  
  S_msrMeasure
    lastMeasure =
      fetchVoiceLastMeasure (inputLineNumber);

  const list<S_msrElement>&
    lastMeasureElementsList =
      lastMeasure->getMeasureElementsList ();
      
  if (lastMeasureElementsList.size ()) {
    result = lastMeasureElementsList.back ();
  }
  
  else {
    stringstream s;

    s <<
      "attempting to fetch voice last element in an empty elements list";

    msrInternalError (
      gXml2lyOptions->fInputSourceName,
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  }

  return result;
}

void msrVoice::prepareForRepeatInVoice (
  int inputLineNumber)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceRepeats) {
    gLogIOstream <<
      "Preparing for repeat in voice \"" <<
      getVoiceName () <<
      "\" in staff \"" <<
      fVoiceStaffUplink->getStaffName () <<
      "\" in part " <<
      fVoiceStaffUplink->
        getStaffPartUplink ()->getPartCombinedName () <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  // forget about voice current repeat if any
  if (fVoiceCurrentRepeat)
    fVoiceCurrentRepeat = nullptr;

  // do the job
  switch (fVoiceKind) {
    case msrVoice::kRegularVoice:
    case msrVoice::kHarmonyVoice:
    case msrVoice::kFiguredBassVoice:
      // is there a voice last segment?
      if (fVoiceLastSegment) {
        
        // are there measures in the voice last segment?
        if (fVoiceLastSegment->getSegmentMeasuresList ().size ()) {
          
          // fetch last measure's full measure length
          int measureFullLength =
            fVoiceLastSegment->
              getSegmentMeasuresList ().back ()->
                getMeasureFullLength ();
                
          // finalize current measure in voice
          finalizeCurrentMeasureInVoice (
            inputLineNumber);
  
#ifdef TRACE_OPTIONS
          if (
            gTraceOptions->fTraceRepeatsDetails
              ||
            gTraceOptions->fTraceVoicesDetails) {
            gLogIOstream <<
              endl <<
              "*********>> Current voice ZZZ \"" <<
              getVoiceName () <<
              "\"" <<
              ", line " << inputLineNumber <<
              " contains:" <<
              endl;
  
            print (gLogIOstream);
  
            gLogIOstream <<
              "<<*********" <<
              endl <<
              endl;
          }
#endif
  
          // move current last segment to the list of initial elements
#ifdef TRACE_OPTIONS
          if (gTraceOptions->fTraceRepeats) {
            gLogIOstream <<
              "Appending voice last segment to the initial elements in voice \"" <<
              getVoiceName () <<
              "\"" <<
              ", line " << inputLineNumber <<
              endl;
          }
#endif
                        
          fVoiceInitialElementsList.push_back (
            fVoiceLastSegment);
  
          // create a new last segment containing a new measure for the voice
#ifdef TRACE_OPTIONS
          if (gTraceOptions->fTraceSegments || gTraceOptions->fTraceVoices) {
            gLogIOstream <<
              "Creating a new last segment containing a new measure for voice \"" <<
              fVoiceName << "\"" <<
              ", line " << inputLineNumber <<
              endl;
          }
#endif
  
if (true)
          createNewLastSegmentAndANewMeasureBeforeARepeat (
            inputLineNumber,
            measureFullLength);
else
          createNewLastSegmentForVoice (
            inputLineNumber);
  
#ifdef TRACE_OPTIONS
          if (
            gTraceOptions->fTraceRepeatsDetails
              ||
            gTraceOptions->fTraceVoicesDetails) {
            gLogIOstream <<
              endl <<
              "*********>> Current voice TTT \"" <<
              getVoiceName () <<
              "\"" <<
              ", line " << inputLineNumber <<
              " contains:" <<
              endl;
  
            print (gLogIOstream);
  
            gLogIOstream <<
              "<<*********" <<
              endl <<
              endl;
          }
#endif
        }
      }
      break;
  } // switch
}

void msrVoice::nestContentsIntoNewRepeatInVoice (
  int inputLineNumber)
{
  switch (fVoiceKind) {
    case msrVoice::kRegularVoice:
    case msrVoice::kHarmonyVoice:
    case msrVoice::kFiguredBassVoice:
      // is there a voice last segment?
      if (fVoiceLastSegment) {
        
        // are there measures in the voice last segment?
        if (fVoiceLastSegment->getSegmentMeasuresList ().size ()) {
          
          // fetch last measure's full measure length
          int measureFullLength =
            fVoiceLastSegment->
              getSegmentMeasuresList ().back ()->
                  getMeasureFullLength ();
                
          // finalize current measure in voice
          finalizeCurrentMeasureInVoice (
            inputLineNumber);
  
#ifdef TRACE_OPTIONS
          if (
            gTraceOptions->fTraceRepeatsDetails
              ||
            gTraceOptions->fTraceVoicesDetails) {
            gLogIOstream <<
              endl <<
              "*********>> Current voice BBB \"" <<
              getVoiceName () <<
              "\"" <<
              ", line " << inputLineNumber <<
              " contains:" <<
              endl;
  
            print (gLogIOstream);
  
            gLogIOstream <<
              "<<*********" <<
              endl <<
              endl;
          }
#endif
  
          // move current last segment to the list of initial elements
#ifdef TRACE_OPTIONS
          if (gTraceOptions->fTraceRepeats) {
            gLogIOstream <<
              "Appending voice last segment to the initial elements in voice \"" <<
              getVoiceName () <<
              "\"" <<
              ", line " << inputLineNumber <<
              endl;
          }
#endif
                        
          fVoiceInitialElementsList.push_back (
            fVoiceLastSegment);
  
          // create a new last segment containing a new measure for the voice
#ifdef TRACE_OPTIONS
          if (gTraceOptions->fTraceSegments || gTraceOptions->fTraceVoices) {
            gLogIOstream <<
              "Creating a new last segment containing a new measure for voice \"" <<
              fVoiceName << "\"" <<
              ", line " << inputLineNumber <<
              endl;
          }
#endif
  
          createNewLastSegmentAndANewMeasureAfterARepeat (
            inputLineNumber,
            measureFullLength);
  
#ifdef TRACE_OPTIONS
          if (
            gTraceOptions->fTraceRepeatsDetails
              ||
            gTraceOptions->fTraceVoicesDetails) {
            gLogIOstream <<
              endl <<
              "*********>> Current voice QQQ \"" <<
              getVoiceName () <<
              "\"" <<
              ", line " << inputLineNumber <<
              " contains:" <<
              endl;
  
            print (gLogIOstream);
  
            gLogIOstream <<
              "<<*********" <<
              endl <<
              endl;
          }
#endif
        }
      }
      break;
  } // switch
}

void msrVoice::createRepeatUponItsEndAndAppendItToVoice (
  int    inputLineNumber,
  string measureNumber,
  int    repeatTimes)
{
  switch (fVoiceKind) {
    case msrVoice::kRegularVoice:
    case msrVoice::kHarmonyVoice:
    case msrVoice::kFiguredBassVoice:
      {
        // finalize current measure in voice
        finalizeCurrentMeasureInVoice (
          inputLineNumber);
          
#ifdef TRACE_OPTIONS
        if (
          gTraceOptions->fTraceRepeatsDetails
            ||
          gTraceOptions->fTraceVoicesDetails) {
          gLogIOstream <<
            endl <<
            "*********>> Current voice III \"" <<
            getVoiceName () <<
            "\"" <<
            ", line " << inputLineNumber <<
            " contains:" <<
            endl;

          print (gLogIOstream);

          gLogIOstream <<
            "<<*********" <<
            endl <<
            endl;
        }
#endif

        // create the repeat
#ifdef TRACE_OPTIONS
        if (gTraceOptions->fTraceRepeats) {
          gLogIOstream <<
            "Creating and appending a repeat in voice \"" <<
            getVoiceName () <<
            "\"" <<
            ", line " << inputLineNumber <<
            endl;
        }
#endif

        S_msrRepeat
          newRepeat =
            msrRepeat::create (
              inputLineNumber,
              repeatTimes,
              this);

        // create a repeat common part from the new segment
#ifdef TRACE_OPTIONS
        if (gTraceOptions->fTraceRepeats) {
          gLogIOstream <<
            "Creating a repeat common part in voice \"" <<
            getVoiceName () <<
            "\"" <<
            ", line " << inputLineNumber <<
            endl;
        }
#endif
        
        S_msrRepeatCommonPart
          repeatCommonPart =
            msrRepeatCommonPart::create (
              inputLineNumber,
              newRepeat);

        // is there another repeat to nest newRepeat into?
        if (fVoiceCurrentRepeat) {
          // yes
          // move the voice initial elements to the new repeat common part
#ifdef TRACE_OPTIONS
          if (gTraceOptions->fTraceRepeats) {
            gLogIOstream <<
              "Moving the initial elements in voice \"" <<
              getVoiceName () <<
              "\" to the new repeat common part" <<
              ", line " << inputLineNumber <<
              endl;
          }
#endif
          
          for (
            list<S_msrElement>::iterator i = fVoiceInitialElementsList.begin ();
            i != fVoiceInitialElementsList.end ();
            i++) {
            S_msrElement element = (*i);
  
            // append the element to the new segment
            repeatCommonPart->
              appendElementToRepeatCommonPart (
                (*i));
  
  //            // remove it from the voic initial elements
  // // JMI            fVoiceInitialElementsList.erase (i);
          } // for
  
          fVoiceInitialElementsList.clear ();
  
          // move voice last segment into the repeat common part
#ifdef TRACE_OPTIONS
          if (gTraceOptions->fTraceRepeats) {
            gLogIOstream <<
              "Append the voice last segment to the repeat common part in voice \"" <<
              getVoiceName () <<
              "\"" <<
              ", line " << inputLineNumber <<
              endl;
          }
#endif
        
          repeatCommonPart->
            appendElementToRepeatCommonPart (
              fVoiceLastSegment);
        }
        
        else {
          // no
          // append the voice last segment to the new repeat common part
#ifdef TRACE_OPTIONS
          if (gTraceOptions->fTraceRepeats) {
            gLogIOstream <<
              "Appending the voice last segment in voice \"" <<
              getVoiceName () <<
              "\" to the new repeat common part" <<
              ", line " << inputLineNumber <<
              endl;
          }
#endif

          gIndenter++;
          
          repeatCommonPart->
            appendElementToRepeatCommonPart (
            fVoiceLastSegment);

          gIndenter--;
        }

        // set newRepeat's common part
#ifdef TRACE_OPTIONS
        if (gTraceOptions->fTraceRepeats) {
          gLogIOstream <<
            "Setting repeat common part in voice \"" <<
            getVoiceName () <<
            "\"" <<
            ", line " << inputLineNumber <<
            endl;
        }
#endif

        newRepeat->
          setRepeatCommonPart (
            repeatCommonPart);

        // append newRepeat to the list of initial elements
#ifdef TRACE_OPTIONS
        if (gTraceOptions->fTraceRepeats) {
          gLogIOstream <<
            "Appending new repeat to the initial elements in voice \"" <<
            getVoiceName () <<
            "\"" <<
            ", line " << inputLineNumber <<
            endl;
        }
#endif
                      
        fVoiceInitialElementsList.push_back (
          newRepeat);
      
        // set voice current repeat
        fVoiceCurrentRepeat =
          newRepeat;

        // create a new last segment containing a new measure for the voice
#ifdef TRACE_OPTIONS
        if (gTraceOptions->fTraceSegments || gTraceOptions->fTraceVoices) {
          gLogIOstream <<
    // JMI        "Creating a new last segment containing a new measure for voice \"" <<
            "Creating a new last segment for voice \"" <<
            fVoiceName << "\"" <<
            ", line " << inputLineNumber <<
            endl;
        }
#endif

        gIndenter++;

/* JMI
        createNewLastSegmentAndANewMeasureAfterARepeat (
          inputLineNumber,
          measureFullLength);
          */
        createNewLastSegmentForVoice (
          inputLineNumber);

        // append a new measure with the same number as the voice last measure
        // to the voice,
        // in case the barline is not at the end of the measure
        // it will be removed if it is empty in finalizeCurrentMeasureInSegment()
        createMeasureAndAppendItToVoice (
          inputLineNumber,
          measureNumber,
          999, // measure ordinal number JMI
          msrMeasure::kMeasureImplicitNo);

        // fetch the voice's last measure
        S_msrMeasure
          voiceLastMeasure =
            fetchVoiceLastMeasure (
              inputLineNumber);

        // set it as created after a repeat
        voiceLastMeasure->
          setMeasureCreatedForARepeatKind (
            msrMeasure::kMeasureCreatedForARepeatAfter);

#ifdef TRACE_OPTIONS
        if (
          gTraceOptions->fTraceRepeatsDetails
            ||
          gTraceOptions->fTraceVoicesDetails) {
          gLogIOstream <<
            endl <<
            "*********>> Current voice EEE \"" <<
            getVoiceName () <<
            "\"" <<
            ", line " << inputLineNumber <<
            " contains:" <<
            endl;

          print (gLogIOstream);

          gLogIOstream <<
            "<<*********" <<
            endl <<
            endl;
        }
#endif

        gIndenter--;
      }
      break;
  } // switch
}

void msrVoice::createRepeatUponItsEndAndAppendItToVoiceClone (
  int    inputLineNumber,
  int    repeatTimes)
{
  switch (fVoiceKind) {
    case msrVoice::kRegularVoice:
    case msrVoice::kHarmonyVoice:
    case msrVoice::kFiguredBassVoice:
      {
        /* JMI
        // fetch last measure's full measure length
        int measureFullLength =
          fVoiceLastSegment->
            getSegmentMeasuresList ().back ()->
                getMeasureFullLength ();
              */
              
        // finalize current measure in voice
        finalizeCurrentMeasureInVoice (
          inputLineNumber);
          
#ifdef TRACE_OPTIONS
        if (
          gTraceOptions->fTraceRepeatsDetails
            ||
          gTraceOptions->fTraceVoicesDetails) {
          gLogIOstream <<
            endl <<
            "*********>> Current voice III \"" <<
            getVoiceName () <<
            "\"" <<
            ", line " << inputLineNumber <<
            " contains:" <<
            endl;

          print (gLogIOstream);

          gLogIOstream <<
            "<<*********" <<
            endl <<
            endl;
        }
#endif

        // create the repeat
#ifdef TRACE_OPTIONS
        if (gTraceOptions->fTraceRepeats) {
          gLogIOstream <<
            "Creating and appending a repeat in voice \"" <<
            getVoiceName () <<
            "\"" <<
            ", line " << inputLineNumber <<
            endl;
        }
#endif

        S_msrRepeat
          newRepeat =
            msrRepeat::create (
              inputLineNumber,
              repeatTimes,
              this);

        // create a repeat common part from the new segment
#ifdef TRACE_OPTIONS
        if (gTraceOptions->fTraceRepeats) {
          gLogIOstream <<
            "Creating a repeat common part in voice \"" <<
            getVoiceName () <<
            "\"" <<
            ", line " << inputLineNumber <<
            endl;
        }
#endif
        
        S_msrRepeatCommonPart
          repeatCommonPart =
            msrRepeatCommonPart::create (
              inputLineNumber,
              newRepeat);

        // is there another repeat to nest newRepeat into?
        if (fVoiceCurrentRepeat) {
          // yes
          // move the voice initial elements to the new repeat common part
#ifdef TRACE_OPTIONS
          if (gTraceOptions->fTraceRepeats) {
            gLogIOstream <<
              "Moving the initial elements in voice \"" <<
              getVoiceName () <<
              "\" to the new repeat common part" <<
              ", line " << inputLineNumber <<
              endl;
          }
#endif
          
          for (
            list<S_msrElement>::iterator i = fVoiceInitialElementsList.begin ();
            i != fVoiceInitialElementsList.end ();
            i++) {
            S_msrElement element = (*i);
  
            // append the element to the new segment
            repeatCommonPart->
              appendElementToRepeatCommonPart (
                (*i));
  
  //            // remove it from the voic initial elements
  // // JMI            fVoiceInitialElementsList.erase (i);
          } // for
  
          fVoiceInitialElementsList.clear ();
  
          // move voice last segment into the repeat common part
#ifdef TRACE_OPTIONS
          if (gTraceOptions->fTraceRepeats) {
            gLogIOstream <<
              "Append the voice last segment to the repeat common part in voice \"" <<
              getVoiceName () <<
              "\"" <<
              ", line " << inputLineNumber <<
              endl;
          }
#endif
        
          repeatCommonPart->
            appendElementToRepeatCommonPart (
              fVoiceLastSegment);
        }
        
        else {
          // no
          // append the voice last segment to the new repeat common part
#ifdef TRACE_OPTIONS
          if (gTraceOptions->fTraceRepeats) {
            gLogIOstream <<
              "Appending the voice last segment in voice \"" <<
              getVoiceName () <<
              "\" to the new repeat common part" <<
              ", line " << inputLineNumber <<
              endl;
          }
#endif

          gIndenter++;
          
          repeatCommonPart->
            appendElementToRepeatCommonPart (
            fVoiceLastSegment);

          gIndenter--;
        }

        // set newRepeat's common part
#ifdef TRACE_OPTIONS
        if (gTraceOptions->fTraceRepeats) {
          gLogIOstream <<
            "Setting repeat common part in voice \"" <<
            getVoiceName () <<
            "\"" <<
            ", line " << inputLineNumber <<
            endl;
        }
#endif

        newRepeat->
          setRepeatCommonPart (
            repeatCommonPart);

        // append newRepeat to the list of initial elements
#ifdef TRACE_OPTIONS
        if (gTraceOptions->fTraceRepeats) {
          gLogIOstream <<
            "Appending new repeat to the initial elements in voice \"" <<
            getVoiceName () <<
            "\"" <<
            ", line " << inputLineNumber <<
            endl;
        }
#endif
                      
        fVoiceInitialElementsList.push_back (
          newRepeat);
      
        // set voice current repeat
        fVoiceCurrentRepeat =
          newRepeat;

        // create a new last segment containing a new measure for the voice
#ifdef TRACE_OPTIONS
        if (gTraceOptions->fTraceSegments || gTraceOptions->fTraceVoices) {
          gLogIOstream <<
    // JMI        "Creating a new last segment containing a new measure for voice \"" <<
            "Creating a new last segment for voice \"" <<
            fVoiceName << "\"" <<
            ", line " << inputLineNumber <<
            endl;
        }
#endif

        gIndenter++;

/* JMI
        createNewLastSegmentAndANewMeasureAfterARepeat (
          inputLineNumber,
          measureFullLength);
          */
        createNewLastSegmentForVoice (
          inputLineNumber);

#ifdef TRACE_OPTIONS
        if (
          gTraceOptions->fTraceRepeatsDetails
            ||
          gTraceOptions->fTraceVoicesDetails) {
          gLogIOstream <<
            endl <<
            "*********>> Current voice EEE \"" <<
            getVoiceName () <<
            "\"" <<
            ", line " << inputLineNumber <<
            " contains:" <<
            endl;

          print (gLogIOstream);

          gLogIOstream <<
            "<<*********" <<
            endl <<
            endl;
        }
#endif

        gIndenter--;
      }
      break;
  } // switch
}

void msrVoice::createRegularRepeatUponItsFirstEndingInVoice (
  int inputLineNumber,
  int repeatTimes)
{
  switch (fVoiceKind) {
    case msrVoice::kRegularVoice:
    case msrVoice::kHarmonyVoice:
    case msrVoice::kFiguredBassVoice:
      {
        /*
          grab the just created last measure from the voice,
          (i.e. the one containing something like:
          <barline location="left">
            <ending number="1" type="start"/>
          </barline>
          which is the first ebding measure
        */
        
        S_msrMeasure
          firstEndingMeasure =
            removeLastMeasureFromVoice (
              inputLineNumber);

/* JMI
        // fetch last measure's full measure length
        int measureFullLength =
          fVoiceLastSegment->
            getSegmentMeasuresList ().back ()->
                getMeasureFullLength ();
              
        // finalize current measure in voice
        finalizeCurrentMeasureInVoice (
          inputLineNumber);
          */
          
        // create the repeat
#ifdef TRACE_OPTIONS
        if (gTraceOptions->fTraceRepeats) {
          gLogIOstream <<
            "Creating and appending a repeat in voice \"" <<
            getVoiceName () <<
            "\"" <<
            ", line " << inputLineNumber <<
            endl;
        }
#endif

        gIndenter++;
        
        fVoiceCurrentRepeat =
          msrRepeat::create (
            inputLineNumber,
            repeatTimes,
            this);

        // create a repeat common part
#ifdef TRACE_OPTIONS
        if (gTraceOptions->fTraceRepeats) {
          gLogIOstream <<
            "Creating a repeat common part from current last segment in voice \"" <<
            getVoiceName () <<
            "\"" <<
            ", line " << inputLineNumber <<
            endl;
        }
#endif
      
        S_msrRepeatCommonPart
          repeatCommonPart =
            msrRepeatCommonPart::create (
              inputLineNumber,
              fVoiceCurrentRepeat);

        // move voice last segment into the repeat common part
#ifdef TRACE_OPTIONS
        if (gTraceOptions->fTraceRepeats) {
          gLogIOstream <<
            "Append the voice last segment to the repeat common part in voice \"" <<
            getVoiceName () <<
            "\"" <<
            ", line " << inputLineNumber <<
            endl;
        }
#endif
      
        repeatCommonPart->
          appendElementToRepeatCommonPart (
            fVoiceLastSegment);

        // set fVoiceCurrentRepeat's common part
#ifdef TRACE_OPTIONS
        if (gTraceOptions->fTraceRepeats) {
          gLogIOstream <<
            "Setting repeat common part in voice \"" <<
            getVoiceName () <<
            "\"" <<
            ", line " << inputLineNumber <<
            endl;
        }
#endif

        fVoiceCurrentRepeat->
          setRepeatCommonPart (
            repeatCommonPart);
          
        // append the repeat to the list of initial elements
#ifdef TRACE_OPTIONS
        if (gTraceOptions->fTraceRepeats) {
          gLogIOstream <<
            "Appending repeat to the initial elements in voice \"" <<
            getVoiceName () <<
            "\"" <<
            ", line " << inputLineNumber <<
            endl;
        }
#endif
                      
        fVoiceInitialElementsList.push_back (
          fVoiceCurrentRepeat);
      
        // create a new last segment containing a new measure for the voice
#ifdef TRACE_OPTIONS
        if (gTraceOptions->fTraceSegments || gTraceOptions->fTraceVoices) {
          gLogIOstream <<
            "Creating a new last segment with the first ending measure for voice \"" <<
            fVoiceName << "\"" <<
            ", line " << inputLineNumber <<
            endl;
        }
#endif

        createNewLastSegmentFromFirstMeasureForVoice (
          inputLineNumber,
          firstEndingMeasure);

#ifdef TRACE_OPTIONS
        if (
          gTraceOptions->fTraceRepeatsDetails
            ||
          gTraceOptions->fTraceVoicesDetails) {
          gLogIOstream <<
            endl <<
            "*********>> Current voice \"" <<
            getVoiceName () <<
            "\"" <<
            ", line " << inputLineNumber <<
            " contains:" <<
            endl;

          print (gLogIOstream);

          gLogIOstream <<
            "<<*********" <<
            endl <<
            endl;
        }
#endif

        gIndenter--;
      }
      break;
  } // switch
}

void msrVoice::createEnclosingRepeatUponItsFirstEndingInVoice (
  int inputLineNumber,
  int repeatTimes)
{
  switch (fVoiceKind) {
    case msrVoice::kRegularVoice:
    case msrVoice::kHarmonyVoice:
    case msrVoice::kFiguredBassVoice:
      {
        /*
          grab the just created last measure from the voice,
          (i.e. the one containing something like:
          <barline location="left">
            <ending number="1" type="start"/>
          </barline>
          which is the first ebding measure
        */
        
        S_msrMeasure
          firstEndingMeasure =
            removeLastMeasureFromVoice (
              inputLineNumber);

/* JMI
        // fetch last measure's full measure length
        int measureFullLength =
          fVoiceLastSegment->
            getSegmentMeasuresList ().back ()->
                getMeasureFullLength ();
              
        // finalize current measure in voice
        finalizeCurrentMeasureInVoice (
          inputLineNumber);
          */
          
        // create the repeat
#ifdef TRACE_OPTIONS
        if (gTraceOptions->fTraceRepeats) {
          gLogIOstream <<
            "Creating and appending a repeat in voice \"" <<
            getVoiceName () <<
            "\"" <<
            ", line " << inputLineNumber <<
            endl;
        }
#endif

        gIndenter++;
      
        fVoiceCurrentRepeat =
          msrRepeat::create (
            inputLineNumber,
            repeatTimes,
            this);

        // create a repeat common part
#ifdef TRACE_OPTIONS
        if (gTraceOptions->fTraceRepeats) {
          gLogIOstream <<
            "Creating a repeat common part from current last segment in voice \"" <<
            getVoiceName () <<
            "\"" <<
            ", line " << inputLineNumber <<
            endl;
        }
#endif
      
        S_msrRepeatCommonPart
          repeatCommonPart =
            msrRepeatCommonPart::create (
              inputLineNumber,
              fVoiceCurrentRepeat);

        // move voice initial elements into the repeat common part
#ifdef TRACE_OPTIONS
        if (gTraceOptions->fTraceRepeats) {
          gLogIOstream <<
            "Moving the voice initial elements to the repeat common part in voice \"" <<
            getVoiceName () <<
            "\"" <<
            ", line " << inputLineNumber <<
            endl;
        }
#endif

        gIndenter++;
        
        for (
          list<S_msrElement>::const_iterator i = fVoiceInitialElementsList.begin ();
          i != fVoiceInitialElementsList.end ();
          i++) {
          repeatCommonPart->
            appendElementToRepeatCommonPart ((*i));
        } // for

        gIndenter--;
        
        // empty the initial elements list
        fVoiceInitialElementsList.resize (0);

/* JMI
        // finalize the voice's last segement
        fVoiceLastSegment->
          finalizeCurrentMeasureInSegment (
            inputLineNumber);
        */
        
        // move voice last segment into the repeat common part
#ifdef TRACE_OPTIONS
        if (gTraceOptions->fTraceRepeats) {
          gLogIOstream <<
            "Append the voice last segment to the repeat common part in voice \"" <<
            getVoiceName () <<
            "\"" <<
            ", line " << inputLineNumber <<
            endl;
        }
#endif

        gIndenter++;
        
        repeatCommonPart->
          appendElementToRepeatCommonPart (
            fVoiceLastSegment);

        gIndenter--;
        
        // set fVoiceCurrentRepeat's common part
#ifdef TRACE_OPTIONS
        if (gTraceOptions->fTraceRepeats) {
          gLogIOstream <<
            "Setting repeat common part in voice \"" <<
            getVoiceName () <<
            "\"" <<
            ", line " << inputLineNumber <<
            endl;
        }
#endif

        gIndenter++;
        
        fVoiceCurrentRepeat->
          setRepeatCommonPart (
            repeatCommonPart);

        gIndenter--;
          
        // append the repeat to the list of initial elements
#ifdef TRACE_OPTIONS
        if (gTraceOptions->fTraceRepeats) {
          gLogIOstream <<
            "Appending repeat to the initial elements in voice \"" <<
            getVoiceName () <<
            "\"" <<
            ", line " << inputLineNumber <<
            endl;
        }
#endif
                      
        fVoiceInitialElementsList.push_back (
          fVoiceCurrentRepeat);
      
        // create a new last segment containing a new measure for the voice
#ifdef TRACE_OPTIONS
        if (gTraceOptions->fTraceSegments || gTraceOptions->fTraceVoices) {
          gLogIOstream <<
            "Creating a new last segment with the first ending measure for voice \"" <<
            fVoiceName << "\"" <<
            ", line " << inputLineNumber <<
            endl;
        }
#endif

        gIndenter++;
        
        createNewLastSegmentFromFirstMeasureForVoice (
          inputLineNumber,
          firstEndingMeasure);

        gIndenter--;

#ifdef TRACE_OPTIONS
        if (
          gTraceOptions->fTraceRepeatsDetails
            ||
          gTraceOptions->fTraceVoicesDetails) {
          gLogIOstream <<
            endl <<
            "*********>> Current voice \"" <<
            getVoiceName () <<
            "\"" <<
            ", line " << inputLineNumber <<
            " contains:" <<
            endl;

          print (gLogIOstream);

          gLogIOstream <<
            "<<*********" <<
            endl <<
            endl;
        }
#endif

        gIndenter--;
      }
      break;
  } // switch
}

void msrVoice::createMeasuresRepeatFromItsFirstMeasuresInVoice (
  int inputLineNumber,
  int measuresRepeatMeasuresNumber,
  int measuresRepeatSlashes)
{
  switch (fVoiceKind) {
    case msrVoice::kRegularVoice:
    case msrVoice::kHarmonyVoice:
    case msrVoice::kFiguredBassVoice:
      {
        // create a measure repeat
#ifdef TRACE_OPTIONS
        if (
          gTraceOptions->fTraceMeasures
            ||
          gTraceOptions->fTraceRepeats
            ||
          gTraceOptions->fTraceVoices
          ) {
          gLogIOstream <<
            "Creating a " <<
            measuresRepeatMeasuresNumber <<
            " measure, " <<
            measuresRepeatSlashes <<
            " slashes repeat from it's first measure in voice \"" <<
            getVoiceName () <<
            "\"" <<
            ", line " << inputLineNumber <<
            endl;
        }
#endif
      
        // print current voice contents
#ifdef TRACE_OPTIONS
        if (
          gTraceOptions->fTraceMeasures
            ||
          gTraceOptions->fTraceRepeats
            ||
          gTraceOptions->fTraceSegments
            ||
          gTraceOptions->fTraceVoices
          ) {
          gLogIOstream <<
            endl <<
            "The current contents of voice \"" <<
            fVoiceName << "\" is:" <<
            endl;
        }
#endif

        gIndenter++;
        print (gLogIOstream);
        gIndenter--;

        // this occurs after an empty measure has just been created,
        // hence the repeated measure/measures is/are the
        // measuresRepeatMeasuresNumber measures preceding the last one

        int
          lastSegmentMeasuresNumber =
            fVoiceLastSegment->
              getSegmentMeasuresList ().size (),
          availableMeasuresNumber =
            lastSegmentMeasuresNumber - 1;
            
        if (
          availableMeasuresNumber < measuresRepeatMeasuresNumber) {
          stringstream s;

          s <<
            "attempting to create a measure repeat with " <<
            measuresRepeatMeasuresNumber <<
            " measures while current last segment only has " <<
            availableMeasuresNumber <<
            " available";

          msrInternalError (
            gXml2lyOptions->fInputSourceName,
            inputLineNumber,
            __FILE__, __LINE__,
            s.str ());
        }

        // grab the just created last measure from the voice,
        // (i.e. the one containing:
        //   <measure-repeat ... type="start">2</measure-repeat>)
        // which is the first replica measure
        S_msrMeasure
          firstReplicaMeasure =
            removeLastMeasureFromVoice (
              inputLineNumber);

        // create the measure repeat repeated segment
        S_msrSegment
          repeatedSegment =
            msrSegment::create (
              inputLineNumber,
              this);

        if (! fVoiceFirstSegment) {
          fVoiceFirstSegment = fVoiceLastSegment;
        }

        // remove the repeated measure(s) for the last segment
        // and prepend them to the repeated segment
#ifdef TRACE_OPTIONS
        if (
          gTraceOptions->fTraceMeasures
            ||
          gTraceOptions->fTraceRepeats
            ||
          gTraceOptions->fTraceSegments
            ||
          gTraceOptions->fTraceVoices
          ) {
          gLogIOstream <<
            "Removing the last " <<
            measuresRepeatMeasuresNumber <<
            " measures (to be repeated) from voice \"" <<
            fVoiceName <<
            endl;
        }
#endif

        for (int i = 0; i< measuresRepeatMeasuresNumber; i++) {
          S_msrMeasure
            lastMeasure =
              removeLastMeasureFromVoice (
                inputLineNumber);

          repeatedSegment->
            prependMeasureToSegment (
              lastMeasure);
        } // for

        // create the measure repeat
        if (fVoiceCurrentMeasuresRepeat) {
          stringstream s;

          s <<
            "attempting to create a measure repeat while another one is pending";

          msrInternalError (
            gXml2lyOptions->fInputSourceName,
            inputLineNumber,
            __FILE__, __LINE__,
            s.str ());
        }
        
        fVoiceCurrentMeasuresRepeat =
          msrMeasuresRepeat::create (
            inputLineNumber,
            measuresRepeatMeasuresNumber,
            measuresRepeatSlashes,
            this);

        // create the measures repeat pattern
#ifdef TRACE_OPTIONS
        if (
          gTraceOptions->fTraceMeasures
            ||
          gTraceOptions->fTraceRepeats
            ||
          gTraceOptions->fTraceSegments
            ||
          gTraceOptions->fTraceVoices
          ) {
          gLogIOstream <<
            "Creating a measure repeat pattern for voice \"" <<
            fVoiceName << "\" is:" <<
            endl;
        }
#endif
        
        S_msrMeasuresRepeatPattern
          measuresRepeatPattern =
            msrMeasuresRepeatPattern::create (
              inputLineNumber,
              fVoiceCurrentMeasuresRepeat);

        // set the repeated segment as the measure repeat pattern segment
        measuresRepeatPattern->
          setMeasuresRepeatPatternSegment (
            repeatedSegment);
            
        // set the measure repeat pattern
        fVoiceCurrentMeasuresRepeat->
          setMeasuresRepeatPattern (
            measuresRepeatPattern);
        
        // create a new last segment to collect the measure repeat replicas,
        // containing the first, yet incomplete, replica
#ifdef TRACE_OPTIONS
        if (
          gTraceOptions->fTraceMeasures
            ||
          gTraceOptions->fTraceRepeats
            ||
          gTraceOptions->fTraceSegments
            ||
          gTraceOptions->fTraceVoices
          ) {
          gLogIOstream <<
            "Creating a new last segment with the first replica measure for voice \"" <<
            fVoiceName << "\"" <<
            ", line " << inputLineNumber <<
            endl;
        }
#endif
            
        createNewLastSegmentFromFirstMeasureForVoice (
          inputLineNumber,
          firstReplicaMeasure);

        // print resulting voice contents
#ifdef TRACE_OPTIONS
        if (
          gTraceOptions->fTraceMeasures
            ||
          gTraceOptions->fTraceRepeats
            ||
          gTraceOptions->fTraceSegments
            ||
          gTraceOptions->fTraceVoices
          ) {
          gLogIOstream <<
            "The resulting voice contents of voice \"" <<
            fVoiceName << "\" is:" <<
            endl;
        }
#endif

        gIndenter++;
        print (gLogIOstream);
        gIndenter--;

        // keep the measure repeat pending
      }
      break;
  } // switch
}

void msrVoice::appendPendingMeasuresRepeatToVoice (
  int inputLineNumber)
{
  switch (fVoiceKind) {
    case msrVoice::kRegularVoice:
    case msrVoice::kHarmonyVoice:
    case msrVoice::kFiguredBassVoice:
      {
        // does the current measures repeat exist?
        if (! fVoiceCurrentMeasuresRepeat) {
          stringstream s;

          s <<
            "attempting to append a pending measure repeat which doesn't exist";

          msrInternalError (
            gXml2lyOptions->fInputSourceName,
            inputLineNumber,
            __FILE__, __LINE__,
            s.str ());
        }

        // fetch the last segment's measure list
        list<S_msrMeasure>&
          voiceLastSegmentMeasureList =
            fVoiceLastSegment->
              getSegmentMeasuresListToModify ();
       
        // grab the just created last measure in the last segment's measure list,
        // (i.e. the one containing:
        //   <measure-repeat type="stop"/>)
        // which is the next measure after the measure repeat
        if (! voiceLastSegmentMeasureList.size ()) {
          stringstream s;

          s <<
            "attempting to grab first measure of voice last segment, that contains none";

          msrInternalError (
            gXml2lyOptions->fInputSourceName,
            inputLineNumber,
            __FILE__, __LINE__,
            s.str ());
        }

        S_msrMeasure
          nextMeasureAfterMeasuresRepeat =
            voiceLastSegmentMeasureList.back ();

// BOFBOFBOF
        // remove the next measure from the last segment's measure list
        voiceLastSegmentMeasureList.pop_back ();

        // create the measure repeat replicas contents
#ifdef TRACE_OPTIONS
        if (
          gTraceOptions->fTraceMeasures
            ||
          gTraceOptions->fTraceRepeats
            ||
          gTraceOptions->fTraceSegments
            ||
          gTraceOptions->fTraceVoices
          ) {
          gLogIOstream <<
            "Creating a measure repeat replicas FAA contents for voice \"" <<
            fVoiceName << "\" is:" <<
            endl;
        }
#endif

        S_msrMeasuresRepeatReplicas
          measuresRepeatReplicas =
            msrMeasuresRepeatReplicas::create (
              inputLineNumber,
              fVoiceCurrentMeasuresRepeat);

        // set the voice last segment as the measure repeat replicas segment
        measuresRepeatReplicas->
          setMeasuresRepeatReplicasSegment (
            fVoiceLastSegment);

        fVoiceCurrentMeasuresRepeat->
          setMeasuresRepeatReplicas (
            measuresRepeatReplicas);

#ifdef TRACE_OPTIONS
        if (
          gTraceOptions->fTraceMeasures
            ||
          gTraceOptions->fTraceRepeats
            ||
          gTraceOptions->fTraceSegments
            ||
          gTraceOptions->fTraceVoices
          ) {
          gLogIOstream <<
            "Setting pending measure repeat replicas segment in voice \"" <<
            getVoiceName () <<
            "\"" <<
            endl;
        }
#endif
        
        fVoiceCurrentMeasuresRepeat->
          setMeasuresRepeatReplicas (
            measuresRepeatReplicas);

#ifdef TRACE_OPTIONS
        if (
          gTraceOptions->fTraceMeasures
            ||
          gTraceOptions->fTraceRepeats
            ||
          gTraceOptions->fTraceSegments
            ||
          gTraceOptions->fTraceVoices
          ) {
          gLogIOstream <<
            "Setting measures repeat segment to voice last segment CCC for voice \"" <<
            fVoiceName << "\"" <<
            endl;
        }
#endif

        fVoiceCurrentMeasuresRepeat->
          getMeasuresRepeatReplicas ()->
            setMeasuresRepeatReplicasSegment (
              fVoiceLastSegment);

        // append pending measure repeat to the list of initial elements
        fVoiceInitialElementsList.push_back (
          fVoiceCurrentMeasuresRepeat);

        // create a new last segment to collect the remainder of the voice,
        // containing the next, yet incomplete, measure
#ifdef TRACE_OPTIONS
        if (
          gTraceOptions->fTraceMeasures
            ||
          gTraceOptions->fTraceRepeats
            ||
          gTraceOptions->fTraceSegments
            ||
          gTraceOptions->fTraceVoices
          ) {
          gLogIOstream <<
            "Creating a new last segment with the AAA measures repeat next measure for voice \"" <<
            fVoiceName << "\"" <<
            ", line " << inputLineNumber <<
            endl;
        }
#endif
            
        createNewLastSegmentFromFirstMeasureForVoice (
          inputLineNumber,
          nextMeasureAfterMeasuresRepeat);

/* JMI
        // print resulting voice contents
#ifdef TRACE_OPTIONS
        if (gTraceOptions->fTraceSegments || gTraceOptions->fTraceVoices) {
          gLogIOstream <<
            "==================> The current voice contents of voice \"" <<
            fVoiceName << "\" is:" <<
            endl;
        }
#endif

        gIndenter++;
        print (gLogIOstream);
        gIndenter--;
        
        // forget about this pending measure repeat
 // JMI ???       fVoiceCurrentMeasuresRepeat = nullptr;
 */
      }
      break;
  } // switch
}

void msrVoice::createMeasuresRepeatAndAppendItToVoiceClone (
  int inputLineNumber,
  int measuresRepeatMeasuresNumber,
  int measuresRepeatSlashesNumber)
{
  switch (fVoiceKind) {
    case msrVoice::kRegularVoice:
    case msrVoice::kHarmonyVoice:
    case msrVoice::kFiguredBassVoice:
      {
        // fetch last measure's full measure length
        int measureFullLength =
          fVoiceLastSegment->
            getSegmentMeasuresList ().back ()->
                getMeasureFullLength ();

        // does the pending measure repeat exist?
        if (fVoiceCurrentMeasuresRepeat) {
          stringstream s;

          s <<
            "attempting to create a measures repeat while another one is pending";

          msrInternalError (
            gXml2lyOptions->fInputSourceName,
            inputLineNumber,
            __FILE__, __LINE__,
            s.str ());
        }

        // create the measures repeat
#ifdef TRACE_OPTIONS
        if (gTraceOptions->fTraceRepeats) {
          gLogIOstream <<
            "Creating and appending a measures repeat in voice \"" <<
            getVoiceName () <<
            "\"" <<
            ", line " << inputLineNumber <<
            endl;
        }
#endif
      
        fVoiceCurrentMeasuresRepeat =
          msrMeasuresRepeat::create (
            inputLineNumber,
            measuresRepeatMeasuresNumber,
            measuresRepeatSlashesNumber,
            this);

        // create a measures repeat pattern from current last segment
#ifdef TRACE_OPTIONS
        if (gTraceOptions->fTraceRepeats) {
          gLogIOstream <<
            "Creating a measures repeat pattern from current last segment in voice \"" <<
            getVoiceName () <<
            "\"" <<
            ", line " << inputLineNumber <<
            endl;
        }
#endif
      
        S_msrMeasuresRepeatPattern
          measuresRepeatPattern =
            msrMeasuresRepeatPattern::create (
              inputLineNumber,
              fVoiceCurrentMeasuresRepeat);

        // set current last segment as the measures repeat pattern segment
        measuresRepeatPattern->
          setMeasuresRepeatPatternSegment (
            fVoiceLastSegment);
            

        // set the measures repeat pattern
#ifdef TRACE_OPTIONS
        if (gTraceOptions->fTraceRepeats) {
          gLogIOstream <<
            "Setting repeat common part in voice \"" <<
            getVoiceName () <<
            "\"" <<
            endl;
        }
#endif

        fVoiceCurrentMeasuresRepeat->
          setMeasuresRepeatPattern (
            measuresRepeatPattern);

        // create a new last segment containing a new measure for the voice
#ifdef TRACE_OPTIONS
        if (gTraceOptions->fTraceSegments || gTraceOptions->fTraceVoices) {
          gLogIOstream <<
            "Creating a new last segment containing a new measure for voice \"" <<
            fVoiceName << "\"" <<
            ", line " << inputLineNumber <<
            endl;
        }
#endif

        createNewLastSegmentAndANewMeasureAfterARepeat (
          inputLineNumber,
          measureFullLength);

        // append the measures repeat to the list of initial elements
#ifdef TRACE_OPTIONS
        if (gTraceOptions->fTraceRepeats) {
          gLogIOstream <<
            "Appending measures repeat to the list of initial elements in voice \"" <<
            getVoiceName () <<
            "\"" <<
            endl;
        }
#endif
                      
        fVoiceInitialElementsList.push_back (
          fVoiceCurrentMeasuresRepeat);
      
        // create a new last segment to collect the remainder of the voice,
        // containing the next, yet incomplete, measure
#ifdef TRACE_OPTIONS
        if (
          gTraceOptions->fTraceMeasures
            ||
          gTraceOptions->fTraceRepeats
            ||
          gTraceOptions->fTraceSegments
            ||
          gTraceOptions->fTraceVoices
          ) {
          gLogIOstream <<
            "Creating a new last segment for voice \"" <<
            fVoiceName << "\"" <<
            ", line " << inputLineNumber <<
            endl;
        }
#endif
            
        createNewLastSegmentForVoice (
          inputLineNumber);

/* JMI
        // print resulting voice contents
#ifdef TRACE_OPTIONS
        if (gTraceOptions->fTraceSegments || gTraceOptions->fTraceVoices) {
          gLogIOstream <<
            "==================> The current voice contents of voice \"" <<
            fVoiceName << "\" is:" <<
            endl;
        }
#endif

        gIndenter++;
        print (gLogIOstream);
        gIndenter--;
        
        // forget about this pending measure repeat
    // JMI ???    fVoiceCurrentMeasuresRepeat = nullptr;
    */
      }
      break;
  } // switch
}

void msrVoice::setVoiceContainsMultipleRests (
  int inputLineNumber)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceRepeats) {
    gLogIOstream <<
      "Voice \"" <<
      getVoiceName () <<
      "\"" <<
      ", line " << inputLineNumber <<
      ", contains multiple rests" <<
      endl;
  }
#endif

  fVoiceContainsMultipleRests = true;
}

void msrVoice::createMultipleRestInVoice (
  int inputLineNumber,
  int multipleRestMeasuresNumber)
{
  switch (fVoiceKind) {
    case msrVoice::kRegularVoice:
    case msrVoice::kHarmonyVoice:
    case msrVoice::kFiguredBassVoice:
      {
        // create a multiple rest
#ifdef TRACE_OPTIONS
        if (gTraceOptions->fTraceRepeats) {
          gLogIOstream <<
            "Creating a multiple rest in voice \"" <<
            getVoiceName () <<
            "\"" <<
            ", line " << inputLineNumber <<
            ", " <<
            singularOrPlural (
              multipleRestMeasuresNumber, "measure", "measures") <<
            endl;
        }
#endif
      
        // grab the just created last measure from the voice,
        // (i.e. the one containing:
        //   <multiple-rest ... type="start">2</multiple-rest>)
        // which is the first rest measure
        S_msrMeasure
          firstRestMeasure =
            removeLastMeasureFromVoice (
              inputLineNumber);

        // move the current voice last segment to the initial elements list
        fVoiceInitialElementsList.push_back (
          fVoiceLastSegment);
          
/* JMI
        // create the multiple rest rests segment
        S_msrSegment
          restsSegment =
            msrSegment::create (
              inputLineNumber,
              this);

  if (! fVoiceFirstSegment) {
    fVoiceFirstSegment = fVoiceLastSegment;
  }
*/

        // create the multiple rest
        if (fVoicePendingMultipleRest) {
          stringstream s;

          s <<
            "attempting to create a multiple rest while another one is pending";

          msrInternalError (
            gXml2lyOptions->fInputSourceName,
            inputLineNumber,
            __FILE__, __LINE__,
            s.str ());
        }
        
        fVoicePendingMultipleRest =
          msrMultipleRest::create (
            inputLineNumber,
            firstRestMeasure->getMeasureFullLength (),
            multipleRestMeasuresNumber,
            this);

         // remember fVoicePendingMultipleRest for later next measure number setting
#ifdef TRACE_OPTIONS
        if (gTraceOptions->fTraceRepeats) {
          gLogIOstream <<
            "Registering multiple rest as waiting for its next measure number" <<
            ", multipleRestMeasuresNumber = " <<
            multipleRestMeasuresNumber <<
            " in voice \"" <<
            fVoiceName << "\"" <<
            endl;
        }
#endif
        
        fVoiceMultipleRestWaitingForItsNextMeasureNumber =
          fVoicePendingMultipleRest;

        fVoiceRemainingRestMeasures =
          multipleRestMeasuresNumber;

#ifdef TRACE_OPTIONS
        if (gTraceOptions->fTraceRepeats) {
          gLogIOstream <<
            "Setting fVoiceRemainingRestMeasures to '" <<
            fVoiceRemainingRestMeasures <<
            "' in voice \"" <<
            fVoiceName << "\"" <<
            endl;
        }
#endif

        // create a new segment to collect the multiple rest measures,
        // containing the first, rest measure
#ifdef TRACE_OPTIONS
        if (gTraceOptions->fTraceSegments || gTraceOptions->fTraceVoices) {
          gLogIOstream <<
            "Creating a new last segment containing the first rest measure in voice \"" <<
            fVoiceName << "\"" <<
            ", line " << inputLineNumber <<
            endl;
        }
#endif
            
        createNewLastSegmentFromFirstMeasureForVoice (
          inputLineNumber,
          firstRestMeasure);

/* JMI
        // append the first rest measure to the new last segment
        fVoiceLastSegment->
          appendMeasureToSegment (
            firstRestMeasure);
*/

        // force multiple measure rests compression JMI ???
        this->setVoiceContainsMultipleRests (
          inputLineNumber);
            
        // print resulting voice contents
#ifdef TRACE_OPTIONS
        if (gTraceOptions->fTraceSegments || gTraceOptions->fTraceVoices) {
          gLogIOstream <<
            "The resulting voice contents of voice \"" <<
            fVoiceName << "\" is:" <<
            endl;
  
          gIndenter++;
          print (gLogIOstream);
          gIndenter--;
        }
#endif

        // keep the multiple rest pending
      }
      break;
  } // switch
}

void msrVoice::appendPendingMultipleRestToVoice (
  int inputLineNumber)
{
  switch (fVoiceKind) {
    case msrVoice::kRegularVoice:
    case msrVoice::kHarmonyVoice:
    case msrVoice::kFiguredBassVoice:
      {
#ifdef TRACE_OPTIONS
        if (gTraceOptions->fTraceRepeats) {
          gLogIOstream <<
            "Appending the pending multiple rest to voice \"" <<
            getVoiceName () <<
            "\"" <<
            ", line " << inputLineNumber <<
            endl;
        }
#endif

/* JMI
        // print current voice contents
#ifdef TRACE_OPTIONS
        if (gTraceOptions->fTraceSegments || gTraceOptions->fTraceVoices) {
          gLogIOstream <<
            "==================> The current voice contents of voice \"" <<
            fVoiceName << "\" is:" <<
            endl;

          gIndenter++;
          print (gLogIOstream);
          gIndenter--;
        }
#endif
*/

        // does the pending multiple rest exist?
        if (! fVoicePendingMultipleRest) {
          stringstream s;

          s <<
            "attempting to append a pending multiple rest which doesn't exist";

          msrInternalError (
            gXml2lyOptions->fInputSourceName,
            inputLineNumber,
            __FILE__, __LINE__,
            s.str ());
        }

        /* JMI
        // fetch the last segment's measure list
        list<S_msrMeasure>&
          voiceLastSegmentMeasureList =
            fVoiceLastSegment->
              getSegmentMeasuresListToModify ();
       
        // grab the just created last measure in the last segment's measure list,
        // which is the next measure after the multiple rest
        if (! voiceLastSegmentMeasureList.size ()) {
          stringstream s;

          s <<
            "attempting to grab first measure of voice last segment, that contains none";

          msrInternalError (
            gXml2lyOptions->fInputSourceName,
            inputLineNumber,
            __FILE__, __LINE__,
            s.str ());
        }

        S_msrMeasure
          nextMeasureAfterMultipleRest =
            voiceLastSegmentMeasureList.back ();

        if (true || gTraceOptions->fTraceSegments || gTraceOptions->fTraceVoices) {
          gLogIOstream <<
            endl <<
            "==========> nextMeasureAfterMultipleRest:" <<
            endl;
  
            
          nextMeasureAfterMultipleRest->
            print (gLogIOstream);
            
          gLogIOstream <<
            endl;
        }
        */
          
        // remove the next measure from the last segment's measure list
    // JMI    voiceLastSegmentMeasureList.pop_back ();

        // create the multiple rest contents
#ifdef TRACE_OPTIONS
        if (gTraceOptions->fTraceSegments || gTraceOptions->fTraceVoices) {
          gLogIOstream <<
            "Creating a measure pattern for voice \"" <<
            fVoiceName << "\" is:" <<
            endl;
        }
#endif

        S_msrMultipleRestContents
          multipleRestContents =
            msrMultipleRestContents::create (
              inputLineNumber,
              this);

        // set voice last segment as the multiple rest segment
#ifdef TRACE_OPTIONS
        if (gTraceOptions->fTraceRepeats) {
          gLogIOstream <<
            "Setting current last segment as multiple rest segment in voice \"" <<
            getVoiceName () <<
            "\"" <<
            endl;
        }
#endif
      
        multipleRestContents->
          setMultipleRestContentsSegment (
            fVoiceLastSegment);

        // set multipleRestContents as the multiple rest contents
#ifdef TRACE_OPTIONS
        if (gTraceOptions->fTraceRepeats) {
          gLogIOstream <<
            "Setting multiple rest contents in voice \"" <<
            getVoiceName () <<
            "\"" <<
            endl;
        }
#endif
      
        fVoicePendingMultipleRest->
          setMultipleRestContents (
            multipleRestContents);

        // append pending multiple rest to the list of initial elements
        fVoiceInitialElementsList.push_back (
          fVoicePendingMultipleRest);

/* JMI
        // print current voice contents
#ifdef TRACE_OPTIONS
        if (gTraceOptions->fTraceSegments || gTraceOptions->fTraceVoices) {
          gLogIOstream <<
            "==================> The current voice contents of voice \"" <<
            fVoiceName << "\" is:" <<
            endl;
  
          gIndenter++;
          print (gLogIOstream);
          gIndenter--;

          gLogIOstream <<
            "<================= The current voice contents of voice \"" <<
            endl;
        }
#endif
*/

        // create a new segment to collect the remainder of the voice,
        // containing the next, yet incomplete, measure
#ifdef TRACE_OPTIONS
        if (gTraceOptions->fTraceSegments || gTraceOptions->fTraceVoices) {
          gLogIOstream <<
            "Creating a new last segment with the measure after the multiple rests for voice \"" <<
            fVoiceName << "\"" <<
            ", line " << inputLineNumber <<
            endl;
        }
#endif
            
        createNewLastSegmentForVoice (
          inputLineNumber);

/* JMI
        // append the next measure after the multiple rest
        // to the new last segment
        fVoiceLastSegment->
          appendMeasureToSegment (
            nextMeasureAfterMultipleRest);
*/
        // print resulting voice contents
#ifdef TRACE_OPTIONS
        if (gTraceOptions->fTraceSegments || gTraceOptions->fTraceVoices) {
          gLogIOstream <<
            "The resulting voice contents of voice \"" <<
            fVoiceName << "\" is:" <<
            endl;

          gIndenter++;
          print (gLogIOstream);
          gIndenter--;
        }
#endif

        // forget about this pending multiple rest
        fVoicePendingMultipleRest = nullptr;
      }
      break;
  } // switch
}

void msrVoice::appendMultipleRestCloneToVoice (
  int               inputLineNumber,
  S_msrMultipleRest multipleRest)
{
  switch (fVoiceKind) {
    case msrVoice::kRegularVoice:
    case msrVoice::kHarmonyVoice:
    case msrVoice::kFiguredBassVoice:
      {
#ifdef TRACE_OPTIONS
        if (gTraceOptions->fTraceRepeats) {
          gLogIOstream <<
            "Appending multiple rest '" <<
            multipleRest->asString () <<
            " to voice clone \"" <<
            getVoiceName () <<
            "\"" <<
            ", line " << inputLineNumber <<
            endl;
        }
#endif

/*
        // print current voice contents
#ifdef TRACE_OPTIONS
        if (gTraceOptions->fTraceSegments || gTraceOptions->fTraceVoices) {
          gLogIOstream <<
            "==================> The current voice contents of voice \"" <<
            fVoiceName << "\" is:" <<
            endl;

          gIndenter++;
          print (gLogIOstream);
          gIndenter--;
        }
#endif
*/

        // append pending multiple rest to the list of initial elements
        fVoiceInitialElementsList.push_back (
          multipleRest);

        // print resulting voice contents
#ifdef TRACE_OPTIONS
        if (gTraceOptions->fTraceSegments || gTraceOptions->fTraceVoices) {
          gLogIOstream <<
            "The resulting voice contents of voice \"" <<
            fVoiceName << "\" is:" <<
            endl;
        }
#endif

        gIndenter++;
        print (gLogIOstream);
        gIndenter--;
      }
      break;
  } // switch
}

void msrVoice::prepareForMultipleRestInVoiceClone (
  int inputLineNumber)
{
  switch (fVoiceKind) {
    case msrVoice::kRegularVoice:
    case msrVoice::kHarmonyVoice:
    case msrVoice::kFiguredBassVoice:
      // is there a voice last segment?
      if (fVoiceLastSegment) {
        
        // are there measures in the voice last segment?
        if (fVoiceLastSegment->getSegmentMeasuresList ().size ()) {
          
          // fetch last measure's full measure length
          int measureFullLength =
            fVoiceLastSegment->
              getSegmentMeasuresList ().back ()->
                  getMeasureFullLength ();
                
          // finalize current measure in voice
          finalizeCurrentMeasureInVoice (
            inputLineNumber);
  
#ifdef TRACE_OPTIONS
          if (
            gTraceOptions->fTraceRepeatsDetails
              ||
            gTraceOptions->fTraceVoicesDetails) {
            gLogIOstream <<
              endl <<
              "*********>> Current voice HHH \"" <<
              getVoiceName () <<
              "\"" <<
              ", line " << inputLineNumber <<
              " contains:" <<
              endl;
  
            print (gLogIOstream);
  
            gLogIOstream <<
              "<<*********" <<
              endl <<
              endl;
          }
#endif
  
          // move current last segment to the list of initial elements
#ifdef TRACE_OPTIONS
          if (gTraceOptions->fTraceRepeats) {
            gLogIOstream <<
              "Appending voice last segment to the initial elements in voice \"" <<
              getVoiceName () <<
              "\"" <<
              ", line " << inputLineNumber <<
              endl;
          }
#endif
                        
          fVoiceInitialElementsList.push_back (
            fVoiceLastSegment);
  
          // create a new last segment containing a new measure for the voice
#ifdef TRACE_OPTIONS
          if (gTraceOptions->fTraceSegments || gTraceOptions->fTraceVoices) {
            gLogIOstream <<
              "Creating a new last segment containing a new measure for voice \"" <<
              fVoiceName << "\"" <<
              ", line " << inputLineNumber <<
              endl;
          }
#endif
  
          createNewLastSegmentAndANewMeasureAfterARepeat (
            inputLineNumber,
            measureFullLength);
  
#ifdef TRACE_OPTIONS
          if (
            gTraceOptions->fTraceRepeatsDetails
              ||
            gTraceOptions->fTraceVoicesDetails) {
            gLogIOstream <<
              endl <<
              "*********>> Current voice PPP \"" <<
              getVoiceName () <<
              "\"" <<
              ", line " << inputLineNumber <<
              " contains:" <<
              endl;
  
            print (gLogIOstream);
  
            gLogIOstream <<
              "<<*********" <<
              endl <<
              endl;
          }
#endif
        }
      }
      break;
  } // switch
}

void msrVoice::appendRepeatCloneToVoice (
  int         inputLineNumber,
  S_msrRepeat repeatCLone)
{
  // sanity check
  msrAssert(
    repeatCLone != nullptr,
    "repeatCLone is null");
      
  switch (fVoiceKind) {
    case msrVoice::kRegularVoice:
    case msrVoice::kHarmonyVoice:
    case msrVoice::kFiguredBassVoice:
      {
#ifdef TRACE_OPTIONS
        if (gTraceOptions->fTraceRepeats) {
          gLogIOstream <<
            "Appending repeat clone to voice clone \"" <<
            getVoiceName () <<  "\"" <<
            endl;
        }
#endif
      
        // create a repeat common part from current last segment
#ifdef TRACE_OPTIONS
        if (gTraceOptions->fTraceRepeats) {
          gLogIOstream <<
            "Creating a repeat clone common part from current last segment in voice \"" <<
            getVoiceName () <<
            "\"" <<
            ", line " << inputLineNumber <<
            endl;
        }
#endif
      
        S_msrRepeatCommonPart
          repeatCommonPart =
            msrRepeatCommonPart::create (
              inputLineNumber,
              repeatCLone);

        // move voice last segment into the repeat common part
#ifdef TRACE_OPTIONS
        if (gTraceOptions->fTraceRepeats) {
          gLogIOstream <<
            "Append the segment to use to the repeat common part in voice \"" <<
            getVoiceName () <<
            "\"" <<
            ", line " << inputLineNumber <<
            endl;
        }
#endif
      
        repeatCommonPart->
          appendElementToRepeatCommonPart (
            repeatCommonPart);

        // set repeatCLone's common part
#ifdef TRACE_OPTIONS
        if (gTraceOptions->fTraceRepeats) {
          gLogIOstream <<
            "Setting repeat clone common part in voice \"" <<
            getVoiceName () <<
            "\"" <<
            endl;
        }
#endif

        repeatCLone->
          setRepeatCommonPart (
            repeatCommonPart);

/* JMI
        // set current last segment as the repeat common segment
#ifdef TRACE_OPTIONS
        if (gTraceOptions->fTraceRepeats) {
          gLogIOstream <<
            "Segging current last segment as repeat common segment in voice \"" <<
            getVoiceName () <<
            "\"" <<
            endl;
        }
#endif
      
        repeatCLone->
          setRepeatCommonSegment (
            fVoiceLastSegment);
 */         
        // register repeat clone as the (new) current repeat
#ifdef TRACE_OPTIONS
        if (gTraceOptions->fTraceRepeats) {
          gLogIOstream <<
            "Registering repeat clone as the new current repeat in voice \"" <<
            getVoiceName () <<
            "\"" <<
            endl;
        }
#endif
      
        fVoiceCurrentRepeat =
          repeatCLone;
      
        // append it to the list of initial elements
        fVoiceInitialElementsList.push_back (
          repeatCLone);
      
        // create a new segment for the voice
#ifdef TRACE_OPTIONS
        if (gTraceOptions->fTraceSegments || gTraceOptions->fTraceVoices) {
          gLogIOstream <<
            "Creating a new last segment for voice BOF JMI \"" <<
            fVoiceName << "\"" <<
             ", line " << inputLineNumber <<
           endl;
        }
#endif
            
        createNewLastSegmentForVoice (
          inputLineNumber);
        }
      break;
  } // switch
}

void msrVoice::appendRepeatEndingToVoice (
  int       inputLineNumber,
  string    repeatEndingNumber, // may be "1, 2"
  msrRepeatEnding::msrRepeatEndingKind
            repeatEndingKind)
{
  switch (fVoiceKind) {
    case msrVoice::kRegularVoice:
    case msrVoice::kHarmonyVoice:
    case msrVoice::kFiguredBassVoice:
      {
#ifdef TRACE_OPTIONS
        if (gTraceOptions->fTraceRepeats) {
          gLogIOstream <<
            "Appending a repeat ending to voice \"" <<
            getVoiceName () <<  "\"" <<
            ", line " << inputLineNumber <<
            endl;
        }
#endif

        gIndenter++;

/* JMI        
        // fetch last measure's full measure length
        int measureFullLength =
          fVoiceLastSegment->
            getSegmentMeasuresList ().back ()->
                getMeasureFullLength ();
*/

        // sanity check
        msrAssert (
          fVoiceCurrentRepeat != nullptr,
          "fVoiceCurrentRepeat is null");

        // create a repeat ending
        S_msrRepeatEnding
          repeatEnding =
            msrRepeatEnding::create (
              inputLineNumber,
              repeatEndingNumber,
              repeatEndingKind,
              fVoiceLastSegment,
              fVoiceCurrentRepeat);
      
        // add the repeat ending it to the voice current repeat
#ifdef TRACE_OPTIONS
        if (gTraceOptions->fTraceRepeats) {
          gLogIOstream <<
            "Appending a " <<
            msrRepeatEnding::repeatEndingKindAsString (
              repeatEndingKind) <<
            " repeat ending to current repeat in voice \"" <<
            fVoiceName <<
            "\"" <<
            endl;
        }
#endif
            
        fVoiceCurrentRepeat->
          addRepeatEnding (repeatEnding);
      
        // create a new last segment containing a new measure for the voice
#ifdef TRACE_OPTIONS
        if (
          gTraceOptions->fTraceRepeats
            ||
          gTraceOptions->fTraceVoices
            ||
          gTraceOptions->fTraceSegments) {
          gLogIOstream <<
     // JMI       "Creating a new last segment containing a new measure for voice \"" <<
            "Creating a new last segment for voice \"" <<
            fVoiceName << "\"" <<
            ", line " << inputLineNumber <<
            endl;
        }
#endif

        switch (repeatEndingKind) { // JMI
          case msrRepeatEnding::kHookedEnding:
            createNewLastSegmentForVoice (
              inputLineNumber);
/* JMI ???
            createNewLastSegmentAndANewMeasureAfterARepeat (
              inputLineNumber,
              1); // JMI measureFullLength);
              */
            break;
          case msrRepeatEnding::kHooklessEnding:
            createNewLastSegmentAndANewMeasureAfterARepeat (
              inputLineNumber,
              1); // JMI measureFullLength);
          /*
            createNewLastSegmentForVoice (
              inputLineNumber);
              */
            break;
        } // switch

        gIndenter--;
      }
      break;
  } // switch
}

void msrVoice::appendMeasuresRepeatReplicaToVoice (
  int       inputLineNumber)
{
  switch (fVoiceKind) {
    case msrVoice::kRegularVoice:
    case msrVoice::kHarmonyVoice:
    case msrVoice::kFiguredBassVoice:
      {
#ifdef TRACE_OPTIONS
        if (gTraceOptions->fTraceRepeats) {
          gLogIOstream <<
            "Appending a measures repeat replica to voice \"" <<
            getVoiceName () <<  "\"" <<
            ", line " << inputLineNumber <<
            endl;
        }
#endif
      
        // fetch last measure's full measure length
        int measureFullLength =
          fVoiceLastSegment->
            getSegmentMeasuresList ().back ()->
              getMeasureFullLength ();

#ifdef TRACE_OPTIONS
        if (gTraceOptions->fTraceRepeats) { // JMI
          gLogIOstream <<
            endl <<
            "***********" <<
            endl <<
            endl;
          print (gLogIOstream);
          gLogIOstream <<
            "***********" <<
            endl <<
            endl;
        }
#endif
        
        // sanity check
        msrAssert (
          fVoiceCurrentMeasuresRepeat != nullptr,
          "fVoiceCurrentMeasuresRepeat is null");
                
        // create a measures repeat replicas
        S_msrMeasuresRepeatReplicas
          measuresRepeatReplicas =
            msrMeasuresRepeatReplicas::create (
              inputLineNumber,
              fVoiceCurrentMeasuresRepeat);

        // set the voice last segment as the measure repeat replicas segment
        measuresRepeatReplicas->
          setMeasuresRepeatReplicasSegment (
            fVoiceLastSegment);
        
        // set the measures repeat replicas in the voice current measures repeat
#ifdef TRACE_OPTIONS
        if (gTraceOptions->fTraceRepeats) {
          gLogIOstream <<
            "Setting the measures repeat replica to current measures repeat BBB in voice \"" <<
            fVoiceName <<
            "\"" <<
            endl;
        }
#endif
            
        fVoiceCurrentMeasuresRepeat->
          setMeasuresRepeatReplicas (
            measuresRepeatReplicas);

        // create a new last segment containing a new measure for the voice
#ifdef TRACE_OPTIONS
        if (gTraceOptions->fTraceSegments || gTraceOptions->fTraceVoices) {
          gLogIOstream <<
            "Creating a new last segment containing a new measure for voice \"" <<
            fVoiceName << "\"" <<
            ", line " << inputLineNumber <<
            endl;
        }
#endif
            
        createNewLastSegmentAndANewMeasureAfterARepeat (
          inputLineNumber,
          measureFullLength);
      }
      break;
  } // switch
}

void msrVoice:: appendRepeatEndingCloneToVoice ( // JMI
  S_msrRepeatEnding repeatEndingClone)
{
  gIndenter++;
  
  switch (fVoiceKind) {
    case msrVoice::kRegularVoice:
    case msrVoice::kHarmonyVoice:
    case msrVoice::kFiguredBassVoice:
      {
        // add the repeat ending it to the voice current repeat
#ifdef TRACE_OPTIONS
        if (gTraceOptions->fTraceRepeats) {
          gLogIOstream <<
            "Appending repeat ending clone to current repeat in voice \"" <<
            fVoiceName <<
            "\"" <<
            endl;
        }
#endif
            
        // sanity check
        msrAssert(
          fVoiceCurrentRepeat != nullptr,
          "fVoiceCurrentRepeat is null");
          
        fVoiceCurrentRepeat->
          addRepeatEnding (repeatEndingClone);
      
        // create a new segment for the voice
#ifdef TRACE_OPTIONS
        if (gTraceOptions->fTraceSegments || gTraceOptions->fTraceVoices) {
          gLogIOstream <<
            "Creating a new last segment for voice \"" <<
            fVoiceName <<
            "\"" <<
            ", line " << repeatEndingClone->getInputLineNumber () <<
            endl;
        }
#endif
            
        createNewLastSegmentForVoice (
          repeatEndingClone->getInputLineNumber ());
      }
      break;
  } // switch

  gIndenter--;
}

void msrVoice::prependBarlineToVoice (S_msrBarline barline)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceRepeats) {
    gLogIOstream <<
      "Prepending barline '" <<
      barline->asString () <<
      "' to voice \"" << getVoiceName () << "\"" <<
      ":" <<
      endl;
  }
#endif
      
  // create the voice last segment and first measure if needed
  appendAFirstMeasureToVoiceIfNotYetDone (
    barline->getInputLineNumber ());

  gIndenter++;
  
  fVoiceLastSegment->
    prependBarlineToSegment (barline);

  gIndenter--;
}

void msrVoice::appendBarlineToVoice (S_msrBarline barline)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceRepeats) {
    gLogIOstream <<
      "Appending barline '" <<
      barline->asString () <<
      "' to voice \"" << getVoiceName () << "\"" <<
      ":" <<
      endl;
  }
#endif
      
  // create the voice last segment and first measure if needed
 // appendAFirstMeasureToVoiceIfNotYetDone (
 //JMI   barline->getInputLineNumber ());

  gIndenter++;
  
  fVoiceLastSegment->
    appendBarlineToSegment (barline);

  gIndenter--;
}

void msrVoice::appendSegnoToVoice (S_msrSegno segno)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceRepeats) {
    gLogIOstream <<
      "Appending a segno to voice \"" << getVoiceName () << "\"" <<
      endl;
  }
#endif

  // create the voice last segment and first measure if needed
  appendAFirstMeasureToVoiceIfNotYetDone (
    segno->getInputLineNumber ());

  fVoiceLastSegment->
    appendSegnoToSegment (segno);
}

void msrVoice::appendCodaToVoice (S_msrCoda coda)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceRepeats) {
    gLogIOstream <<
      "Appending a coda to voice \"" << getVoiceName () << "\"" <<
      ":" <<
      endl;
  }
#endif

  // create the voice last segment and first measure if needed
  appendAFirstMeasureToVoiceIfNotYetDone (
    coda->getInputLineNumber ());

  fVoiceLastSegment->
    appendCodaToSegment (coda);
}

void msrVoice::appendEyeGlassesToVoice (S_msrEyeGlasses eyeGlasses)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceNotes) {
    gLogIOstream <<
      "Appending a eyeGlasses to voice \"" << getVoiceName () << "\"" <<
      endl;
  }
#endif

  // create the voice last segment and first measure if needed
  appendAFirstMeasureToVoiceIfNotYetDone (
    eyeGlasses->getInputLineNumber ());

  fVoiceLastSegment->
    appendEyeGlassesToSegment (eyeGlasses);
}

void msrVoice::appendPedalToVoice (S_msrPedal pedal)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceNotes) {
    gLogIOstream <<
      "Appending a pedal to voice \"" << getVoiceName () << "\"" <<
      endl;
  }
#endif

  // create the voice last segment and first measure if needed
  appendAFirstMeasureToVoiceIfNotYetDone (
    pedal->getInputLineNumber ());

  fVoiceLastSegment->
    appendPedalToSegment (pedal);
}

void msrVoice::appendDampToVoice (S_msrDamp damp)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceNotes) {
    gLogIOstream <<
      "Appending a damp to voice \"" << getVoiceName () << "\"" <<
      endl;
  }
#endif

  // create the voice last segment and first measure if needed
  appendAFirstMeasureToVoiceIfNotYetDone (
    damp->getInputLineNumber ());

  fVoiceLastSegment->
    appendDampToSegment (damp);
}

void msrVoice::appendDampAllToVoice (S_msrDampAll dampAll)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceNotes) {
    gLogIOstream <<
      "Appending a damp all to voice \"" << getVoiceName () << "\"" <<
      endl;
  }
#endif

  // create the voice last segment and first measure if needed
  appendAFirstMeasureToVoiceIfNotYetDone (
    dampAll->getInputLineNumber ());

  fVoiceLastSegment->
    appendDampAllToSegment (dampAll);
}

/*
S_msrElement msrVoice::removeLastElementFromVoice (  // JMI
  int inputLineNumber)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceVoices) {
    gLogIOstream <<
      "Removing last note" <<
      " from voice " << getVoiceName () <<
      endl;
  }
#endif

  return
    fVoiceLastSegment->
      removeLastElementFromSegment (inputLineNumber);
}
*/

void msrVoice::removeNoteFromVoice (
  int       inputLineNumber,
  S_msrNote note)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceChords) {
    gLogIOstream <<
      "Removing note '" <<
      note->asShortString () <<
      "' from voice \"" << getVoiceName () << "\"" <<
      endl;
  }
#endif

  gIndenter++;
  
  fVoiceLastSegment->
    removeNoteFromSegment (
      inputLineNumber,
      note);

  gIndenter--;
}

void msrVoice::removeElementFromVoice (
  int          inputLineNumber,
  S_msrElement element)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceChords || gTraceOptions->fTraceGraceNotes) {
    gLogIOstream <<
      "Removing element '" <<
      element->asShortString () <<
      "' from voice \"" << getVoiceName () << "\"" <<
      endl;
  }
#endif

  gIndenter++;
  
  fVoiceLastSegment->
    removeElementFromSegment (
      inputLineNumber,
      element);

  gIndenter--;
}

S_msrMeasure msrVoice::removeLastMeasureFromVoice (
  int inputLineNumber)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceMeasures || gTraceOptions->fTraceVoices) {
    gLogIOstream <<
      "Removing last measure from voice \"" <<
      getVoiceName () <<
      "\"" <<
      endl;
  }
#endif

  gIndenter++;

  // remove (new) last measure
  S_msrMeasure result =
    fVoiceLastSegment->
      removeLastMeasureFromSegment (
        inputLineNumber);

  gIndenter--;

  // return it
  return result;
}

void msrVoice::finalizeCurrentMeasureInVoice (
  int inputLineNumber)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceMeasures || gTraceOptions->fTraceVoices) {
    gLogIOstream <<
      "Finalizing current measure in voice \"" <<
      getVoiceName () <<
      "\"" <<
      ", line " << inputLineNumber <<
      endl;
      
    gIndenter++;

    const int fieldWidth = 26;
    
    gLogIOstream << left <<
      setw (fieldWidth) <<
      "partMeasureLengthHighTide" << " = " <<
      fetchVoicePartUplink ()->
        getPartMeasureLengthHighTide () <<
      endl;
        
    gIndenter--;
  }
#endif

  gIndenter++;
  
  // make sure the voice's last segment exists
  appendAFirstMeasureToVoiceIfNotYetDone (
    inputLineNumber);

  // finalize last segment' current measure
  fVoiceLastSegment->
    finalizeCurrentMeasureInSegment (
      inputLineNumber);

  // append a measure end syllable to the voice stanzas if any
  if (fVoiceStanzasMap.size ()) {
    map<string, S_msrStanza>::const_iterator
      iBegin = fVoiceStanzasMap.begin (),
      iEnd   = fVoiceStanzasMap.end (),
      i      = iBegin;
      
    for ( ; ; ) {
      S_msrStanza
        stanza = (*i).second;

      stanza->
        appendMeasureEndSyllableToStanza (
          inputLineNumber);

      if (++i == iEnd) break;
    } // for
  }

  gIndenter--;
}

void msrVoice::finalizeVoice ( // JMI ???
  int inputLineNumber)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceVoices) {
    gLogIOstream <<
      "Finalizing voice \"" <<
      getVoiceName () <<
      "\", line " << inputLineNumber <<
      endl;
  }
#endif

  // is this voice totally empty? this should be rare...
  if (
    fVoiceInitialElementsList.size () == 0
      &&
    fVoiceLastSegment->getSegmentMeasuresList ().size () == 0
    ) {
    stringstream s;
    
    s <<
      "Voice \"" <<
      getVoiceName () <<
      "\" is totally empty, no contents ever specified for it" <<
      endl;

    msrMusicXMLWarning (
      gXml2lyOptions->fInputSourceName,
      inputLineNumber,
      s.str ());
  }
}

void msrVoice::acceptIn (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrVoice::acceptIn ()" <<
      endl;
  }
      
  if (visitor<S_msrVoice>*
    p =
      dynamic_cast<visitor<S_msrVoice>*> (v)) {
        S_msrVoice elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrVoice::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrVoice::acceptOut (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrVoice::acceptOut ()" <<
      endl;
  }

  if (visitor<S_msrVoice>*
    p =
      dynamic_cast<visitor<S_msrVoice>*> (v)) {
        S_msrVoice elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrVoice::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrVoice::browseData (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrVoice::browseData ()" <<
      endl;
  }

  // browse the voice initial elements
  if (fVoiceInitialElementsList.size ()) {
    for (
      list<S_msrElement>::const_iterator i = fVoiceInitialElementsList.begin ();
      i != fVoiceInitialElementsList.end ();
      i++) {
      // browse the element
      msrBrowser<msrElement> browser (v);
      browser.browse (*(*i));
    } // for
  }

  // browse the voice last segment
  msrBrowser<msrSegment> browser (v);
  browser.browse (*fVoiceLastSegment);

  // browse the voice stanzas
  if (fVoiceStanzasMap.size ()) {
    for (
      map<string, S_msrStanza>::const_iterator i = fVoiceStanzasMap.begin ();
      i != fVoiceStanzasMap.end ();
      i++) {
      S_msrStanza stanza = (*i).second;

      if (stanza->getStanzaTextPresent ()) {
        // browse the stanza
        msrBrowser<msrStanza> browser (v);
        browser.browse (*(stanza));
      }
    } // for
  }

  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% <== msrVoice::browseData ()" <<
      endl;
  }
}

string msrVoice::voiceKindAsString (
  msrVoiceKind voiceKind)
{
  string result;
  
  switch (voiceKind) {
    case msrVoice::kRegularVoice:
      result = "regular";
      break;
    case msrVoice::kHarmonyVoice:
      result = "harmony";
      break;
    case msrVoice::kFiguredBassVoice:
      result = "figured bass";
      break;
  } // switch

  return result;
}

string msrVoice::voiceNumberAsString () const
{
  string result;
  
  switch (fVoiceNumber) {
    case K_PART_FIGURED_BASS_VOICE_NUMBER:
      result = "K_PART_FIGURED_BASS_VOICE_NUMBER";
      break;
    default:
      result = to_string (fVoiceNumber);
  } // switch
  
  return result;
}

string msrVoice::regularVoiceStaffSequentialNumberAsString () const
{
  string result;
  
  switch (fRegularVoiceStaffSequentialNumber) {
    case K_PART_FIGURED_BASS_VOICE_NUMBER:
      result = "K_PART_FIGURED_BASS_VOICE_NUMBER";
      break;
    default:
      result = to_string (fRegularVoiceStaffSequentialNumber);
  } // switch
  
  return result;
}
         
string msrVoice::voiceFinalizationStatusKindAsString (
  msrVoiceFinalizationStatusKind voiceFinalizationStatusKind)
{
  string result;
  
  switch (voiceFinalizationStatusKind) {
    case msrVoice::kKeepVoice:
      result = "keep";
      break;
    case msrVoice::kEraseVoice:
      result = "erase";
      break;
  } // switch

  return result;
}  

string msrVoice::voiceKindAsString () const
{
  return voiceKindAsString (fVoiceKind);
}
      
ostream& operator<< (ostream& os, const S_msrVoice& elt)
{
  elt->print (os);
  return os;
}

void msrVoice::print (ostream& os)
{
  os <<
    "Voice \"" << getVoiceName () << "\", " <<
    voiceKindAsString (fVoiceKind) <<
 // JMI   ", this: " << this <<
    endl;

  gIndenter++;

  os << left <<
    "(" <<
    singularOrPlural (
      fVoiceActualHarmoniesCounter, "harmony", "harmonies") <<
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

  const int fieldWidth = 34;

  os << left <<
    setw (fieldWidth) << "staffUplink" << " : " <<
    fVoiceStaffUplink->getStaffName () <<
    endl <<
    setw (fieldWidth) << "voiceNumber" << " : " <<
    voiceNumberAsString () <<
    endl <<
    setw (fieldWidth) << "regularVoiceStaffSequentialNumber" << " : " <<
    regularVoiceStaffSequentialNumberAsString () <<
    endl;

  // print the harmony voice name if any
  os << left <<
    setw (fieldWidth) << "HarmonyVoice" << " : ";
  if (fHarmonyVoiceForRegularVoice) {    
    os <<
      fHarmonyVoiceForRegularVoice->getVoiceName ();
  }
  else {
    os <<
      "none";
  }
  os <<
    endl;

  os << left <<
    setw (fieldWidth) << "voiceShortestNoteDuration" << " : " <<
    fVoiceShortestNoteDuration <<
    endl <<
    setw (fieldWidth) << "voiceShortestNoteTupletFactor" << " : " <<
    fVoiceShortestNoteTupletFactor <<
    endl;
    
  os << left <<
    setw (fieldWidth) << "musicHasBeenInsertedInVoice" << " : " <<
    booleanAsString (fMusicHasBeenInsertedInVoice) <<
    endl <<
    setw (fieldWidth) << "voiceContainsMultipleRests" << " : " <<
    booleanAsString (fVoiceContainsMultipleRests) <<
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
  os <<
    endl;

  // print the voice last appended note
  os <<
    setw (fieldWidth) << "voiceLastAppendedNote" << " : ";
  if (fVoiceLastAppendedNote) {
    os <<
      endl;

    gIndenter++;

    os <<
      fVoiceLastAppendedNote;

    gIndenter--;
  }
  else {
    os <<
      "none" <<
      endl;
  }
  os <<
    endl;
  
  // print the voice initial elements
  int initialElementsNumber =
    fVoiceInitialElementsList.size ();
    
  os <<
    "Initial elements: ";
  if (initialElementsNumber) {
    os << "(" << initialElementsNumber << ")";
  }
  else {
    os << "none";
  }
  os <<
    endl;
    
  if (initialElementsNumber) {
    os <<
      endl;
      
    gIndenter++;

    list<S_msrElement>::const_iterator
      iBegin = fVoiceInitialElementsList.begin (),
      iEnd   = fVoiceInitialElementsList.end (),
      i      = iBegin;
      
    for ( ; ; ) {
      // print the element
      os << (*i);
      if (++i == iEnd) break;
      os << endl;
    } // for

    gIndenter--;
  }
  
  os <<
    endl;

  /* JMI
  // sanity check
  msrAssert (
    fVoiceLastSegment != nullptr,
    "fVoiceLastSegment is null");
    */

  // print the last segment
  if (fVoiceLastSegment) {
    os <<
      "Voice last segment:" <<
      endl;
      
    gIndenter++;
    os <<
      fVoiceLastSegment <<
      endl;
    gIndenter--;
  }
  
  else {
    os <<
      "*** Last segment is null ***" << // JMI
      endl;
  }
  
  // print the stanzas if any
  if (fVoiceStanzasMap.size ()) {
    os <<
      "Stanzas:" <<
      endl;
      
    gIndenter++;
    
    map<string, S_msrStanza>::const_iterator
      iBegin = fVoiceStanzasMap.begin (),
      iEnd   = fVoiceStanzasMap.end (),
      i      = iBegin;
      
    for ( ; ; ) {
      os << (*i).second;
      if (++i == iEnd) break;
      // no endl here
    } // for

    gIndenter--;
  }
  
  gIndenter--;
}

//______________________________________________________________________________
int msrStaff::gStaffMaxRegularVoices = 4;

S_msrStaff msrStaff::create (
  int          inputLineNumber,
  msrStaffKind staffKind,
  int          staffNumber,
  S_msrPart    staffPartUplink)
{
  msrStaff* o =
    new msrStaff (
      inputLineNumber,
      staffKind,
      staffNumber,
      staffPartUplink);
  assert(o!=0);

  return o;
}

msrStaff::msrStaff (
  int          inputLineNumber,
  msrStaffKind staffKind,
  int          staffNumber,
  S_msrPart    staffPartUplink)
    : msrElement (inputLineNumber)
{
  // sanity check
  msrAssert(
    staffPartUplink != nullptr,
    "staffPartUplink is null");

  // set staff part uplink
  fStaffPartUplink =
    staffPartUplink;

  // set staff kind and number
  fStaffKind   = staffKind;
  fStaffNumber = staffNumber;

  // do other initializations
  initializeStaff ();
}

void msrStaff::initializeStaff ()
{
  fStaffRegularVoicesCounter = 0;

  // set staff name
  switch (fStaffKind) {
    case msrStaff::kRegularStaff:
      fStaffName =
        fStaffPartUplink->getPartMsrName () +
        "_Staff_" +
        int2EnglishWord (fStaffNumber);
      break;
      
    case msrStaff::kTablatureStaff:
        fStaffPartUplink->getPartMsrName () +
        "_Tablature_" +
        int2EnglishWord (fStaffNumber);
      break;
      
    case msrStaff::kHarmonyStaff:
      fStaffName =
        fStaffPartUplink->getPartMsrName () +
        "_HARMONY_Staff";
      break;
      
    case msrStaff::kFiguredBassStaff:
      fStaffName =
        fStaffPartUplink->getPartMsrName () +
        "_FIGURED_BASS_Staff";
      break;
      
    case msrStaff::kDrumStaff:
      fStaffName =
        fStaffPartUplink->getPartMsrName () +
        "_DRUM_Staff";
      break;
      
    case msrStaff::kRythmicStaff:
      fStaffName =
        fStaffPartUplink->getPartMsrName () +
        "_RYTHMIC_Staff";
      break;
  } // switch

#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceStaves) {
    gLogIOstream <<
      "Initializing staff \"" << fStaffName <<
      "\" in part " <<
      fStaffPartUplink->getPartCombinedName () <<
      endl;
  }
#endif

  gIndenter++;
  
  // check the staff number
  switch (fStaffKind) {
    case msrStaff::kRegularStaff:
      // the staff number should not be negative
      if (fStaffNumber < 0) {
        stringstream s;
    
        s <<
          "regular staff number " << fStaffNumber <<
          " is not positive";
          
        msrAssert (
          false,
          s.str ());
      }
      break;
      
    case msrStaff::kTablatureStaff:
      break;
      
    case msrStaff::kHarmonyStaff:
      break;
      
    case msrStaff::kFiguredBassStaff:
      if (fStaffNumber != K_PART_FIGURED_BASS_STAFF_NUMBER) {
        stringstream s;
    
        s <<
          "figured bass staff number " << fStaffNumber <<
          " is not equal to " << K_PART_FIGURED_BASS_STAFF_NUMBER;
          
        msrInternalError (
          gXml2lyOptions->fInputSourceName,
          fInputLineNumber,
          __FILE__, __LINE__,
          s.str ());
      }
      break;
      
    case msrStaff::kDrumStaff:
      break;
      
    case msrStaff::kRythmicStaff:
      break;
  } // switch

/* JMI
  // set fStaffCurrentClef to the default treble clef,
  // but dont append it to the staff:
  // it's used to check for a clef change in the MusicXML data
  fStaffCurrentClef =
      msrClef::create (
        0, // inputLineNumber
        msrClef::kTrebleClef);
*/

  // get the initial staff details from the part if any
  S_msrStaffDetails
    partStaffDetails =
      fStaffPartUplink->
        getCurrentPartStaffDetails ();

  if (partStaffDetails) {
    // append it to the staff
    appendStaffDetailsToStaff (partStaffDetails);
  }
    
  // get the initial clef from the part if any
  {
    S_msrClef
      clef =
        fStaffPartUplink->
          getPartCurrentClef ();
  
    if (clef) {
#ifdef TRACE_OPTIONS
      if (gTraceOptions->fTraceClefs || gTraceOptions->fTraceStaves) {
        gLogIOstream <<
          "Appending part clef '" << clef->asString () <<
          "' as initial clef to staff \"" <<
          getStaffName () <<
          "\" in part " <<
          fStaffPartUplink->getPartCombinedName () <<
          endl;
      }
#endif

      appendClefToStaff (clef); // JMI
    }
  }
    
  // get the initial key from the part if any
  {
    //* JMI
    S_msrKey
      key =
        fStaffPartUplink->
          getPartCurrentKey ();
  
    if (key) {
#ifdef TRACE_OPTIONS
      if (gTraceOptions->fTraceStaves || gTraceOptions->fTraceKeys) {
        gLogIOstream <<
          "Appending part key '" << key->asString () <<
          "' as initial key to staff \"" <<
          getStaffName () <<
          "\" in part " <<
          fStaffPartUplink->getPartCombinedName () <<
          endl;
      }
#endif

      appendKeyToStaff (key);
    }
      /* JMI
    else {
#ifdef TRACE_OPTIONS
      if (gTraceOptions->fTraceStaves) {
        gLogIOstream <<
          "Appending default C major key " <<
          " to staff \"" <<
          getStaffName () <<
          "\" in part " <<
          fStaffPartUplink->getPartCombinedName () <<
          endl;
      }
#endif
          
      // create the implicit initial C major key
      appendKeyToStaff (
        msrKey::createTraditional (
          fInputLineNumber,
          k_cNatural,
          msrKey::kMajorMode,
          0)); // keyCancel
    }
     */
  }
  
  // get the initial time from the part if any
  {
    /* JMI
    S_msrTime
      time =
        fStaffPartUplink->
          getPartCurrentTime ();

    if (time) {
#ifdef TRACE_OPTIONS
      if (gTraceOptions->fTraceStaves || gTraceOptions->fTraceTimes) {
        gLogIOstream <<
          "Appending part time '" << time->asString () <<
          "' as initial time to staff \"" <<
          getStaffName () <<
          "\" in part " <<
          fStaffPartUplink->getPartCombinedName () <<
          endl;
      }
#endif

      appendTimeToStaff (time);
    }
    */
  }
  
  // get the initial transpose from the part if any
  {
    S_msrTranspose
      transpose =
        fStaffPartUplink->
          getPartCurrentTranspose ();
        
    if (transpose) {
#ifdef TRACE_OPTIONS
      if (gTraceOptions->fTraceStaves /* JMI ||       gTraceOptions->fTraceTransposes */) {
        gLogIOstream <<
          "Appending part transpose '" << transpose->asString () <<
          "' as initial transpose to staff \"" <<
          getStaffName () <<
          "\" in part " <<
          fStaffPartUplink->getPartCombinedName () <<
          endl;
      }
#endif
      
      fStaffCurrentTranspose = transpose;
      
      appendTransposeToAllStaffVoices (transpose);
    }
  }

  // set staff instrument names default values // JMI
  fStaffInstrumentName =
    fStaffPartUplink->
      getPartInstrumentName ();
  fStaffInstrumentAbbreviation =
    fStaffPartUplink->
      getPartInstrumentAbbreviation ();

  gIndenter--;
}

msrStaff::~msrStaff ()
{}

S_msrStaff msrStaff::createStaffNewbornClone (
  S_msrPart containingPart)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceStaves) {
    gLogIOstream <<
      "Creating a newborn clone of staff \"" <<
      fStaffName <<
      "\"" <<
      endl;
  }
#endif
  
  // sanity check
  msrAssert(
    containingPart != nullptr,
    "containingPart is null");
    
  S_msrStaff
    newbornClone =
      msrStaff::create (
        fInputLineNumber,
        fStaffKind,
        fStaffNumber,
        containingPart);

  newbornClone->fStaffName =
    fStaffName;
    
  newbornClone->fStaffNumber =
    fStaffNumber;
    
  newbornClone->fStaffInstrumentName =
    fStaffInstrumentName;
    
  newbornClone->fStaffInstrumentAbbreviation =
    fStaffInstrumentAbbreviation;
        
  return newbornClone;
}

void msrStaff::setStaffCurrentClef (S_msrClef clef)
{
  fStaffCurrentClef = clef;
};

void msrStaff::setStaffCurrentKey (S_msrKey key)
{
  fStaffCurrentKey = key;
};

string msrStaff::staffNumberAsString ()
{
  string result;
  
  switch (fStaffNumber) {
    case K_PART_FIGURED_BASS_STAFF_NUMBER:
      result = "K_PART_FIGURED_BASS_STAFF_NUMBER";
      break;
    default:
      result = to_string (fStaffNumber);
  } // switch
  
  return result;
}

/* KEEP JMI
const int msrStaff::getStaffNumberOfMusicVoices () const
{
  int result = 0;

  for (
    map<int, S_msrVoice>::const_iterator i =
      fStaffRegularVoicesMap.begin ();
    i != fStaffRegularVoicesMap.end ();
    i++) {
      S_msrVoice
        voice =
          (*i).second;

      switch (voice->getVoiceKind ()) {
        case msrVoice::kRegularVoice:
          if (voice->getMusicHasBeenInsertedInVoice ())
            result++;
          break;
          
        case msrVoice::kHarmonyVoice: // JMI
          break;
          
        case msrVoice::kFiguredBassVoice: // JMI
          break;
      } // switch
      
  } // for

  return result;
}
*/

void msrStaff::createMeasureAndAppendItToStaff (
  int    inputLineNumber,
  string measureNumber,
  int    measureOrdinalNumber,
  msrMeasure::msrMeasureImplicitKind
         measureImplicitKind)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceMeasures || gTraceOptions->fTraceStaves) {
    gLogIOstream <<
      "Creating and appending measure '" <<
      measureNumber <<
      ", in staff \"" << getStaffName () << "\"" <<
      "', line " << inputLineNumber <<
      endl;
  }
#endif

  // propagate it to all staves
  for (
    map<int, S_msrVoice>::const_iterator i = fStaffAllVoicesMap.begin ();
    i != fStaffAllVoicesMap.end ();
    i++) {
    S_msrVoice
      voice = (*i).second;

    // sanity check
    msrAssert (
      voice != nullptr,
      "voice is null");
    
  switch (voice->getVoiceKind ()) {
    case msrVoice::kRegularVoice:
#ifdef TRACE_OPTIONS
      if (gTraceOptions->fTraceMeasures || gTraceOptions->fTraceStaves) {
        gLogIOstream <<
          "Propagating the creation and appending of measure '" <<
          measureNumber <<
          "', line " << inputLineNumber <<
          ", to voice \"" << voice->getVoiceName () << "\"" <<
          endl;
      }
#endif
  
      voice->
        createMeasureAndAppendItToVoice (
          inputLineNumber,
          measureNumber,
          measureOrdinalNumber,
          measureImplicitKind);
      break;
      
    case msrVoice::kHarmonyVoice:
      break;
      
    case msrVoice::kFiguredBassVoice:
      break;
  } // switch

/* JMI
  #ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceMeasures || gTraceOptions->fTraceStaves) {
      gLogIOstream <<
        "Propagating the creation and appending of measure '" <<
        measureNumber <<
        "', line " << inputLineNumber <<
        ", to voice \"" << voice->getVoiceName () << "\"" <<
        endl;
    }
#endif

    voice->
      createMeasureAndAppendItToVoice (
        inputLineNumber,
        measureNumber,
        measureOrdinalNumber,
        measureImplicitKind);
        */
  } // for
}

void msrStaff::setNextMeasureNumberInStaff (
  int    inputLineNumber,
  string nextMeasureNumber)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceMeasures || gTraceOptions->fTraceStaves) {
    gLogIOstream <<
      "Setting next measure number to '" <<
      nextMeasureNumber <<
      ", in staff \"" << getStaffName () << "\"" <<
      "', line " << inputLineNumber <<
      endl;
  }
#endif

  gIndenter++;
  
  // propagate it to all staves
  for (
    map<int, S_msrVoice>::const_iterator i = fStaffAllVoicesMap.begin ();
    i != fStaffAllVoicesMap.end ();
    i++) {
    S_msrVoice voice = (*i).second;

    // sanity check
    msrAssert (
      voice != nullptr,
      "voice is null");
    
#ifdef TRACE_OPTIONS
    if (gTraceOptions->fTraceMeasures || gTraceOptions->fTraceStaves) {
      gLogIOstream <<
        "Propagating the setting of next measure number '" <<
        nextMeasureNumber <<
        "', line " << inputLineNumber <<
        ", in voice \"" << voice->getVoiceName () << "\"" <<
        endl;
    }
#endif

    voice->
      setNextMeasureNumberInVoice (
        inputLineNumber,
        nextMeasureNumber);
  } // for

  gIndenter--;
}

S_msrVoice msrStaff::createVoiceInStaffByItsNumber (
  int                    inputLineNumber,
  msrVoice::msrVoiceKind voiceKind,
  int                    voiceNumber,
  string                 currentMeasureNumber)
{
  // take this new voice into account if relevant
  switch (voiceKind) {
    case msrVoice::kRegularVoice:
      fStaffRegularVoicesCounter++;

#ifdef TRACE_OPTIONS
      if (gTraceOptions->fTraceStaves || gTraceOptions->fTraceVoices) {
        gLogIOstream <<
          "Creating regular voice number '" <<
          voiceNumber <<
          "', voiceKind '" <<
          msrVoice::voiceKindAsString (voiceKind) <<
          "' as regular voice '" <<
          fStaffRegularVoicesCounter <<
          "' of staff \"" << getStaffName () <<
          "\", line " << inputLineNumber <<
          "\", current measure number: " <<
          currentMeasureNumber <<
     // JMI     " in part " << fStaffPartUplink->getPartCombinedName () <<
          endl;
      }
#endif
      break;
      
    case msrVoice::kHarmonyVoice:
#ifdef TRACE_OPTIONS
      if (gTraceOptions->fTraceStaves || gTraceOptions->fTraceVoices) {
        gLogIOstream <<
          "Creating harmony voice number '" <<
          voiceNumber <<
          "', voiceKind '" <<
          msrVoice::voiceKindAsString (voiceKind) <<
          "' in staff \"" << getStaffName () <<
          "\", line " << inputLineNumber <<
          "\", current measure number: " <<
          currentMeasureNumber <<
     // JMI     " in part " << fStaffPartUplink->getPartCombinedName () <<
          endl;
      }
#endif
      break;
      
    case msrVoice::kFiguredBassVoice:
#ifdef TRACE_OPTIONS
      if (gTraceOptions->fTraceStaves || gTraceOptions->fTraceVoices) {
        gLogIOstream <<
          "Creating figured bass voice number '" <<
          voiceNumber <<
          "', voiceKind '" <<
          msrVoice::voiceKindAsString (voiceKind) <<
          "' in staff \"" << getStaffName () <<
          "\", line " << inputLineNumber <<
          "\", current measure number: " <<
          currentMeasureNumber <<
     // JMI     " in part " << fStaffPartUplink->getPartCombinedName () <<
          endl;
      }
#endif
      break;
  } // switch


  // are there too many regular voices in this staff? 
  if (fStaffRegularVoicesCounter > msrStaff::gStaffMaxRegularVoices) {
    stringstream s;
    
    s <<
      "staff \"" << getStaffName () <<
      "\" is already filled up with " <<
      msrStaff::gStaffMaxRegularVoices << " regular voices" <<
      endl <<
      ". voice number " <<
      voiceNumber <<
      " overflows it" <<
      endl <<
      ", fStaffRegularVoicesCounter = " <<
      fStaffRegularVoicesCounter <<
      ", msrStaff::gStaffMaxRegularVoices = " <<
      msrStaff::gStaffMaxRegularVoices <<
      endl;

      /* JMI ???
    msrMusicXMLError (
// JMI    msrMusicXMLWarning ( JMI
      gXml2lyOptions->fInputSourceName,
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());
      */
  }

  // create the voice
  S_msrVoice
    voice =
      msrVoice::create (
        inputLineNumber,
        voiceKind,
        voiceNumber,
        msrVoice::kCreateInitialLastSegmentYes,
        this);
          
  // take this new voice into account if relevant
  switch (voiceKind) {      
    case msrVoice::kRegularVoice:
      // register the voice by its relative number
#ifdef TRACE_OPTIONS
      if (gTraceOptions->fTraceVoices) {
        gLogIOstream <<
          "Voice " << voiceNumber <<
          " in staff " << getStaffName () <<
          " gets staff regular voice number " <<
          fStaffRegularVoicesCounter <<
          endl;
      }
#endif
        
      registerVoiceInRegularVoicesMap (
        voiceNumber,
        voice);
      break;

    case msrVoice::kHarmonyVoice:
      break;
            
    case msrVoice::kFiguredBassVoice:
      break;
  } // switch

  // register it in staff by its number
  registerVoiceByItsNumber (
    inputLineNumber,
    voiceNumber,
    voice);
    
  return voice;
}

void msrStaff::registerVoiceByItsNumber (
  int        inputLineNumber,
  int        voiceNumber,
  S_msrVoice voice)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceVoices || gTraceOptions->fTraceStaves) {
    gLogIOstream <<
      "Registering voice number '" << voiceNumber <<
      "', named \"" << voice->getVoiceName () <<
      "\" in staff " << getStaffName () <<
      endl;
  }
#endif

  // register voice in all voices map
  fStaffAllVoicesMap [voiceNumber] =
    voice;

  // register it in all voices list
  fStaffAllVoicesList.push_back (voice);

  // sort the list if necessary
  switch (voice->getVoiceKind ()) {      
    case msrVoice::kRegularVoice:
      break;

    case msrVoice::kHarmonyVoice:
#ifdef TRACE_OPTIONS
      if (gTraceOptions->fTraceStaves || gTraceOptions->fTraceVoices) {
        gLogIOstream <<
          "Sorting the voices in staff \"" <<
          getStaffName () << "\"" <<
          ", line " << inputLineNumber <<
          endl;
      }
#endif

    /* JMI
      gLogIOstream <<
        endl <<
        endl <<
        "@@@@@@@@@@@@@@@@ fStaffAllVoicesList contains initially:" <<
        endl <<
        endl;
    
      for (
        list<S_msrVoice>::const_iterator i = fStaffAllVoicesList.begin ();
        i != fStaffAllVoicesList.end ();
        i++) {
        S_msrVoice
          voice = (*i);
          
        gLogIOstream <<
          voice->getVoiceName () <<
          endl;
      } // for
      gLogIOstream <<
        endl <<
        endl;
      */
      
      // sort fStaffAllVoicesList, to have harmonies just before
      // the corresponding voice
      if (fStaffAllVoicesList.size ()) {
        fStaffAllVoicesList.sort (
          compareVoicesToHaveHarmoniesAboveCorrespondingVoice);
      }

    /* JMI
      gLogIOstream <<
        endl <<
        endl <<
        "@@@@@@@@@@@@@@@@ fStaffAllVoicesList contains after sort:" <<
        endl <<
        endl;
    
      for (
        list<S_msrVoice>::const_iterator i = fStaffAllVoicesList.begin ();
        i != fStaffAllVoicesList.end ();
        i++) {
        S_msrVoice
          voice = (*i);
          
        gLogIOstream <<
          voice->getVoiceName () <<
          endl;
      } // for
      gLogIOstream <<
        endl <<
        endl;
        */
      break;
            
    case msrVoice::kFiguredBassVoice:
      break;
  } // switch  
}

void msrStaff::registerVoiceInRegularVoicesMap (
  int        voiceNumber,
  S_msrVoice voice)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceVoices || gTraceOptions->fTraceStaves) {
    gLogIOstream <<
      "Registering regular voice number '" << voiceNumber <<
      "', named \"" << voice->getVoiceName () <<
      "\" in staff " << getStaffName () <<
      "'s regular voices map" <<
      endl;
  }
#endif

  fStaffRegularVoicesMap [voiceNumber] =
    voice;

  // set voice staff sequential number
  voice->
    setRegularVoiceStaffSequentialNumber (
      fStaffRegularVoicesCounter);
}

void msrStaff::registerVoiceInAllVoicesList (
  int        voiceNumber,
  S_msrVoice voice)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceVoices || gTraceOptions->fTraceStaves) {
    gLogIOstream <<
      "Registering voice number '" << voiceNumber <<
      "', named \"" << voice->getVoiceName () <<
      "\" in staff " << getStaffName () <<
      "'s all voices list" <<
      endl;
  }
#endif

  fStaffAllVoicesList.push_back (voice);
}

S_msrVoice msrStaff::fetchVoiceFromStaffByItsNumber (
  int inputLineNumber,
  int voiceNumber)
{
  S_msrVoice result; // JMI avoid repetivite messages!

#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceVoices || gTraceOptions->fTraceStaves) {
    gLogIOstream <<
      "Fetching voice number " <<
      voiceNumber <<
     " in staff \"" << getStaffName () <<
      "\", line " << inputLineNumber <<
      " in part " <<
      fStaffPartUplink->getPartCombinedName () <<
      endl;
  }
#endif

  for (
    map<int, S_msrVoice>::const_iterator i =
      fStaffRegularVoicesMap.begin ();
    i != fStaffRegularVoicesMap.end ();
    i++) {
    if (
      (*i).second->getVoiceNumber ()
        ==
      voiceNumber) {
#ifdef TRACE_OPTIONS
        if (gTraceOptions->fTraceVoices || gTraceOptions->fTraceStaves) {
          gLogIOstream <<
            "Voice " << voiceNumber <<
            " in staff \"" << getStaffName () << "\"" <<
            " has staff relative number " << (*i).first <<
            endl;
        }
#endif
        
        result = (*i).second;
        break;
    }
  } // for

  return result;
}

void msrStaff::addAVoiceToStaffIfItHasNone (
  int inputLineNumber)
{
  if (fStaffAllVoicesMap.size () == 0) {
#ifdef TRACE_OPTIONS
    if (gTraceOptions->fTraceStaves || gTraceOptions->fTraceVoices) {
      gLogIOstream <<
        "Staff \"" <<
        getStaffName () <<
        "\" doesn't contain any voice, adding one" <<
        ", line " << inputLineNumber <<
        endl;
    }
#endif

    this->
      createVoiceInStaffByItsNumber (
        inputLineNumber,
        msrVoice::kRegularVoice,
        1,    // voiceNumber,
        "1"); // fCurrentMeasureNumber
  }
}

void msrStaff::registerVoiceInStaff (
  int        inputLineNumber,
  S_msrVoice voice)
{
  // sanity check
  msrAssert (
    voice != nullptr,
    "voice is null");

  // get voice kind
  msrVoice::msrVoiceKind voiceKind =
    voice->getVoiceKind ();
    
  // take this new voice into account if relevant
  switch (voiceKind) {
    case msrVoice::kRegularVoice:
      // take that regular voice into account
      fStaffRegularVoicesCounter++;

      // are there too many voices in this staff? 
      if (fStaffRegularVoicesCounter > msrStaff::gStaffMaxRegularVoices) {
        stringstream s;
        
        s <<
          "staff \"" << getStaffName () <<
          "\" is already filled up with " <<
          msrStaff::gStaffMaxRegularVoices << " regular voices," <<
          endl <<
          "the voice named \"" << voice->getVoiceName () << "\" overflows it" <<
          endl <<
          ", fStaffRegularVoicesCounter = " <<
          fStaffRegularVoicesCounter <<
          ", msrStaff::gStaffMaxRegularVoices = " <<
          msrStaff::gStaffMaxRegularVoices <<
          endl;

          /* JMI ???
        msrMusicXMLError (
    // JMI    msrMusicXMLWarning ( JMI
          gXml2lyOptions->fInputSourceName,
          inputLineNumber,
          __FILE__, __LINE__,
          s.str ());
          */
      }
      break;
            
    case msrVoice::kHarmonyVoice:
      break;
    case msrVoice::kFiguredBassVoice:
      break;
  } // switch

  // register voice in this staff
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceStaves || gTraceOptions->fTraceVoices) {
    gLogIOstream <<
      "Registering voice \"" << voice->getVoiceName () <<
      "\" as relative voice " <<
      fStaffRegularVoicesCounter <<
      " of staff \"" << getStaffName () <<
      "\", line " << inputLineNumber <<
// JMI       " in part " << fStaffPartUplink->getPartCombinedName () <<
      endl;
  }
#endif

  // register it in staff by its number
  registerVoiceByItsNumber (
    inputLineNumber,
    voice->getVoiceNumber (),
    voice);

  // is voice a regular voice???
  switch (voiceKind) {
    case msrVoice::kRegularVoice:
      {
        int voiceNumber = voice->getVoiceNumber ();
        
        // register the voice by its number
#ifdef TRACE_OPTIONS
        if (gTraceOptions->fTraceVoices) {
          gLogIOstream <<
            "Registering regular voice '" << voiceNumber <<
            "' " << voice->getVoiceName () <<
            " with staff regular voice number " <<
            fStaffRegularVoicesCounter <<
            " in staff " << getStaffName () <<
            endl;
        }
#endif

        registerVoiceInRegularVoicesMap (
          voiceNumber,
          voice);
      }
      break;
      
    case msrVoice::kHarmonyVoice:
      break;
      
    case msrVoice::kFiguredBassVoice:
      break;
  } // switch
}

void msrStaff::padUpToMeasureLengthInStaff (
  int      inputLineNumber,
  rational measureLength)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceStaves || gTraceOptions->fTraceMeasures) {
    gLogIOstream <<
      "Padding up to measure length '" << measureLength <<
      "' in staff \"" <<
      getStaffName () <<
      "\", line " << inputLineNumber <<
      endl;
  }
#endif

  for (
    map<int, S_msrVoice>::const_iterator i = fStaffAllVoicesMap.begin ();
    i != fStaffAllVoicesMap.end ();
    i++) {
    (*i).second-> // JMI msrAssert???
      padUpToMeasureLengthInVoice (
        inputLineNumber,
        measureLength);
  } // for
}

void msrStaff::appendClefToStaff (S_msrClef clef)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceClefs || gTraceOptions->fTraceStaves) {
    gLogIOstream <<
      "Appending clef '" << clef->asString () <<
      "' to staff \"" <<
      getStaffName () <<
      "\" in part " <<
      fStaffPartUplink->getPartCombinedName () <<
      endl;
  }
#endif

  gIndenter++;
  
  // append clef to the staff,
  // unless we should ignore redundant clefs
  // and a clef equal to the current clef is met
  bool doAppendClefToStaff = true;
    
  if (fStaffCurrentClef) {
    if (
      gMusicXMLOptions->fIgnoreRedundantClefs
        &&
      clef->isEqualTo (fStaffCurrentClef)) {
      doAppendClefToStaff = false;
    }
  }
  
  if (doAppendClefToStaff) {
    // register clef as current staff clef
    fStaffCurrentClef = clef;

/* JMI ??? should there be a staff lines number change for 00f-Basics-Clefs.xml???
    // set staff kind accordingly if relevant
    switch (clef->getClefKind ()) {
      case msrClef::kPercussionClef:
        fStaffKind = msrStaff::kDrumStaff;
        break;
      case msrClef::kTablature4Clef:
      case msrClef::kTablature5Clef:
      case msrClef::kTablature6Clef:
      case msrClef::kTablature7Clef:
        fStaffKind = msrStaff::kTablatureStaff;
        break;
      default:
        ;
    } // switch
*/

    // propagate clef to all voices
    for (
      map<int, S_msrVoice>::const_iterator i = fStaffAllVoicesMap.begin ();
      i != fStaffAllVoicesMap.end ();
      i++) {
      (*i).second-> // JMI msrAssert???
        appendClefToVoice (clef);
    } // for
  }

  else {
#ifdef TRACE_OPTIONS
    if (gTraceOptions->fTraceClefs || gTraceOptions->fTraceStaves) {
      gLogIOstream <<
        "Clef '" <<
        clef->asString () <<
        "' ignored because it is already present in staff " <<
        getStaffName () <<
        "\" in part " <<
        fStaffPartUplink->getPartCombinedName () <<
        endl;
    }
#endif
  }

  gIndenter--;
}

void msrStaff::appendKeyToStaff (S_msrKey  key)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceKeys || gTraceOptions->fTraceStaves) {
    gLogIOstream <<
      "Appending key '" << key->asString () <<
      "' to staff \"" <<
      getStaffName () <<
      "\" in part " <<
      fStaffPartUplink->getPartCombinedName () <<
      endl;
  }
#endif

  gIndenter++;
  
  // append key to staff?
  bool doAppendKeyToStaff = true;
    
  if (fStaffCurrentKey) {
    if (
      gMusicXMLOptions->fIgnoreRedundantKeys
        &&
      fStaffCurrentKey->isEqualTo (key)) {
      doAppendKeyToStaff = false;
    }
    
    else {
      if (key->isEqualTo (fStaffCurrentKey)) {
#ifdef TRACE_OPTIONS
        if (gTraceOptions->fTraceKeys || gTraceOptions->fTraceStaves) {
          gLogIOstream <<
            "Key '" <<
            key->asString () <<
            "' ignored because it is already present in staff " <<
            getStaffName () <<
            "\" in part " <<
            fStaffPartUplink->getPartCombinedName () <<
            endl;
        }
#endif
  
        doAppendKeyToStaff = false;
      }
    }
  }
  
  if (doAppendKeyToStaff) {
    // register key as current staff key
    fStaffCurrentKey = key;
  
    // propagate it to all voices
    for (
      map<int, S_msrVoice>::const_iterator i = fStaffAllVoicesMap.begin ();
      i != fStaffAllVoicesMap.end ();
      i++) {
      (*i).second-> // JMI msrAssert???
        appendKeyToVoice (key);
    } // for
  }

  gIndenter--;
}

void msrStaff::appendTimeToStaff (S_msrTime time)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceTimes || gTraceOptions->fTraceStaves) {
    gLogIOstream <<
      "Appending time '" << time->asString () <<
      "' to staff \"" <<
      getStaffName () <<
      "\" in part " <<
      fStaffPartUplink->getPartCombinedName () <<
      endl;
  }
#endif

  gIndenter++;
  
  // append time to staff?
  bool doAppendTimeToStaff = true;

  if (fStaffCurrentTime) {
    if (
      gMusicXMLOptions->fIgnoreRedundantTimes
        &&
      fStaffCurrentTime->isEqualTo (time)) {
      doAppendTimeToStaff = false;
    }
    
    else {
      if (time->isEqualTo (fStaffCurrentTime)) {
#ifdef TRACE_OPTIONS
        if (gTraceOptions->fTraceTimes || gTraceOptions->fTraceStaves) {
          gLogIOstream <<
            "Time '" <<
            time->asString () <<
            "' ignored because it is already present in staff " <<
            getStaffName () <<
            "\" in part " <<
            fStaffPartUplink->getPartCombinedName () <<
            endl;
        }
#endif
  
        doAppendTimeToStaff = false;
      }
    }
  }
  
  if (doAppendTimeToStaff) {
    // register time as current staff time
    fStaffCurrentTime = time;

    // propagate it to all voices
    for (
      map<int, S_msrVoice>::const_iterator i = fStaffAllVoicesMap.begin ();
      i != fStaffAllVoicesMap.end ();
      i++) {
      (*i).second-> // JMI msrAssert???
        appendTimeToVoice (time);
    } // for
  }

  gIndenter--;
}    

void msrStaff::appendTimeToStaffClone (S_msrTime time)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceTimes || gTraceOptions->fTraceStaves) {
    gLogIOstream <<
      "Appending time '" << time->asString () <<
      "' to staff clone \"" <<
      getStaffName () <<
      "\" in part " <<
      fStaffPartUplink->getPartCombinedName () <<
      endl;
  }
#endif

  gIndenter++;
  
  // set staff time
  fStaffCurrentTime = time;

  // propagate it to all voices
  for (
    map<int, S_msrVoice>::const_iterator i = fStaffAllVoicesMap.begin ();
    i != fStaffAllVoicesMap.end ();
    i++) {
    (*i).second-> // JMI msrAssert???
      appendTimeToVoiceClone (time);
  } // for

  gIndenter--;
}    

void msrStaff::prepareForRepeatInStaff (
  int inputLineNumber)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceRepeats) {
    gLogIOstream <<
      "Preparing for repeat in staff \"" <<
      getStaffName () <<
      "\" in part " <<
      fStaffPartUplink->getPartCombinedName () <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  gIndenter++;
  
  for (
    map<int, S_msrVoice>::const_iterator i = fStaffAllVoicesMap.begin ();
    i != fStaffAllVoicesMap.end ();
    i++) {
    (*i).second->
      prepareForRepeatInVoice (
        inputLineNumber);
  } // for

  gIndenter--;
}

void msrStaff::nestContentsIntoNewRepeatInStaff (
  int inputLineNumber)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceRepeats) {
    gLogIOstream <<
      "Nesting contents into new repeat in staff \"" <<
      getStaffName () <<
      "\" in part " <<
      fStaffPartUplink->getPartCombinedName () <<
      endl;
  }
#endif

  for (
    map<int, S_msrVoice>::const_iterator i = fStaffAllVoicesMap.begin ();
    i != fStaffAllVoicesMap.end ();
    i++) {
    (*i).second->
      nestContentsIntoNewRepeatInVoice (
        inputLineNumber);
  } // for
}

void msrStaff::createRepeatUponItsEndAndAppendItToStaff (
  int    inputLineNumber,
  string measureNumber,
  int    repeatTimes)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceRepeats) {
    gLogIOstream <<
      "Creating and appending a repeat to staff \"" <<
      getStaffName () <<
      "\" in part " <<
      fStaffPartUplink->getPartCombinedName () <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  gIndenter++;
  
  for (
    map<int, S_msrVoice>::const_iterator i = fStaffAllVoicesMap.begin ();
    i != fStaffAllVoicesMap.end ();
    i++) {
    (*i).second->
      createRepeatUponItsEndAndAppendItToVoice (
        inputLineNumber,
        measureNumber,
        repeatTimes);
  } // for

  gIndenter--;
}

void msrStaff::createRegularRepeatUponItsFirstEndingInStaff (
  int inputLineNumber,
  int repeatTimes)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceRepeats) {
    gLogIOstream <<
      "Creating and appending a repeat to staff \"" <<
      getStaffName () <<
      "\" in part " <<
      fStaffPartUplink->getPartCombinedName () <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  gIndenter++;
  
  for (
    map<int, S_msrVoice>::const_iterator i = fStaffAllVoicesMap.begin ();
    i != fStaffAllVoicesMap.end ();
    i++) {
    (*i).second->
      createRegularRepeatUponItsFirstEndingInVoice (
        inputLineNumber,
        repeatTimes);
  } // for

  gIndenter--;
}

void msrStaff::createEnclosingRepeatUponItsFirstEndingInStaff (
  int inputLineNumber,
  int repeatTimes)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceRepeats) {
    gLogIOstream <<
      "Creating and appending a repeat to staff \"" <<
      getStaffName () <<
      "\" in part " <<
      fStaffPartUplink->getPartCombinedName () <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  gIndenter++;
  
  for (
    map<int, S_msrVoice>::const_iterator i = fStaffAllVoicesMap.begin ();
    i != fStaffAllVoicesMap.end ();
    i++) {
    (*i).second->
      createEnclosingRepeatUponItsFirstEndingInVoice (
        inputLineNumber,
        repeatTimes);
  } // for

  gIndenter--;
}

void msrStaff::appendRepeatEndingToStaff (
  int       inputLineNumber,
  string    repeatEndingNumber, // may be "1, 2"
  msrRepeatEnding::msrRepeatEndingKind
            repeatEndingKind)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceRepeats) {
    gLogIOstream <<
      "Appending a repeat ending to staff \"" <<
      getStaffName () <<
      "\" in part " <<
      fStaffPartUplink->getPartCombinedName () <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  gIndenter++;
  
  for (
    map<int, S_msrVoice>::const_iterator i = fStaffAllVoicesMap.begin ();
    i != fStaffAllVoicesMap.end ();
    i++) {
    (*i).second->
      appendRepeatEndingToVoice (
        inputLineNumber,
        repeatEndingNumber,
        repeatEndingKind);
  } // for

  gIndenter--;
}

void msrStaff::createMeasuresRepeatFromItsFirstMeasuresInStaff (
  int inputLineNumber,
  int measuresRepeatMeasuresNumber,
  int measuresRepeatSlashes)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceRepeats) {
    gLogIOstream <<
      "Creating a measure repeat from it's first measure in staff \"" <<
      getStaffName () <<
      "\" in part " <<
      fStaffPartUplink->getPartCombinedName () <<
      endl;
  }
#endif

  for (
    map<int, S_msrVoice>::const_iterator i = fStaffAllVoicesMap.begin ();
    i != fStaffAllVoicesMap.end ();
    i++) {
    (*i).second->
      createMeasuresRepeatFromItsFirstMeasuresInVoice (
        inputLineNumber,
        measuresRepeatMeasuresNumber,
        measuresRepeatSlashes);
  } // for
}

void msrStaff::appendPendingMeasuresRepeatToStaff (
  int inputLineNumber)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceRepeats) {
    gLogIOstream <<
      "Appending the pending measure repeat to staff \"" <<
      getStaffName () <<
      "\" in part " <<
      fStaffPartUplink->getPartCombinedName () <<
      endl;
  }
#endif

  for (
    map<int, S_msrVoice>::const_iterator i = fStaffAllVoicesMap.begin ();
    i != fStaffAllVoicesMap.end ();
    i++) {
    (*i).second->
      appendPendingMeasuresRepeatToVoice (
        inputLineNumber);
  } // for
}

void msrStaff::createMultipleRestInStaff (
  int inputLineNumber,
  int multipleRestMeasuresNumber)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceRepeats) {
    gLogIOstream <<
      "Creating a multiple rest in staff \"" <<
      getStaffName () <<
      "\" in part " <<
      fStaffPartUplink->getPartCombinedName () <<
      ", " <<
      singularOrPlural (
        multipleRestMeasuresNumber, "measure", "measures") <<
      endl;
  }
#endif

  for (
    map<int, S_msrVoice>::const_iterator i = fStaffAllVoicesMap.begin ();
    i != fStaffAllVoicesMap.end ();
    i++) {
    (*i).second->
      createMultipleRestInVoice (
        inputLineNumber,
        multipleRestMeasuresNumber);
  } // for
}

void msrStaff::appendPendingMultipleRestToStaff (
  int inputLineNumber)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceRepeats) {
    gLogIOstream <<
      "Appending the pending multiple rest to staff \"" <<
      getStaffName () <<
      "\" in part " <<
      fStaffPartUplink->getPartCombinedName () <<
      endl;
  }
#endif

  for (
    map<int, S_msrVoice>::const_iterator i = fStaffAllVoicesMap.begin ();
    i != fStaffAllVoicesMap.end ();
    i++) {
    (*i).second->
      appendPendingMultipleRestToVoice (
        inputLineNumber);
  } // for
}

void msrStaff::appendMultipleRestCloneToStaff (
  int               inputLineNumber,
  S_msrMultipleRest multipleRest)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceRepeats) {
    gLogIOstream <<
      "Appending multiple rest '" <<
      multipleRest->asString () <<
      "' to staff clone \"" <<
      getStaffName () <<
      "\"" <<
      endl;
  }
#endif

  for (
    map<int, S_msrVoice>::const_iterator i = fStaffAllVoicesMap.begin ();
    i != fStaffAllVoicesMap.end ();
    i++) {
    (*i).second-> // JMI msrAssert???
      appendMultipleRestCloneToVoice (
        inputLineNumber,
        multipleRest);
  } // for
}

void msrStaff::appendRepeatCloneToStaff (
  int         inputLineNumber,
  S_msrRepeat repeatCLone)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceRepeats) {
    gLogIOstream <<
      "Appending repeat clone to staff \"" <<
      getStaffName () <<
      "\" in part " <<
      fStaffPartUplink->getPartCombinedName () <<
      endl;
  }
#endif

  for (
    map<int, S_msrVoice>::const_iterator i = fStaffAllVoicesMap.begin ();
    i != fStaffAllVoicesMap.end ();
    i++) {
    (*i).second-> // JMI msrAssert???
      appendRepeatCloneToVoice (
        inputLineNumber, repeatCLone);
  } // for
}

void msrStaff::appendRepeatEndingCloneToStaff (
  S_msrRepeatEnding repeatEndingClone)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceRepeats) {
    gLogIOstream <<
      "Appending a repeat ending clone to staff \"" <<
      getStaffName () <<
      "\" in part " <<
      fStaffPartUplink->getPartCombinedName () <<
      ", line " << repeatEndingClone->getInputLineNumber () <<
      endl;
  }
#endif

  gIndenter++;
  
  for (
    map<int, S_msrVoice>::const_iterator i = fStaffAllVoicesMap.begin ();
    i != fStaffAllVoicesMap.end ();
    i++) {
    (*i).second-> // JMI msrAssert???
      appendRepeatEndingCloneToVoice (
        repeatEndingClone);
  } // for

  gIndenter--;
}

void msrStaff::appendBarlineToStaff (S_msrBarline barline)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceMeasures) {
    gLogIOstream <<
      "Appending barline '" << barline->asString () <<
      "' to staff " <<
      getStaffName () <<
      "\" in part " <<
      fStaffPartUplink->getPartCombinedName () <<
      endl;
  }
#endif

  gIndenter++;
  
  for (
    map<int, S_msrVoice>::const_iterator i = fStaffAllVoicesMap.begin ();
    i != fStaffAllVoicesMap.end ();
    i++) {
    (*i).second->
      appendBarlineToVoice (barline);
  } // for

  gIndenter--;
}

void msrStaff::appendTransposeToStaff (
  S_msrTranspose transpose)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceTranspositions || gTraceOptions->fTraceStaves) {
    gLogIOstream <<
      "Setting transpose '" <<
      transpose->asString () <<
      "' in staff " <<
      getStaffName () <<
      "\" in part " <<
      fStaffPartUplink->getPartCombinedName () <<
      endl;
  }
#endif

  // set staff transpose
  bool doAppendTransposeToStaff = true;
  
  if (! fStaffCurrentTranspose) {
    doAppendTransposeToStaff = true;
  }
  
  else {
    if (transpose->isEqualTo (fStaffCurrentTranspose)) {
#ifdef TRACE_OPTIONS
      if (gTraceOptions->fTraceTranspositions || gTraceOptions->fTraceStaves) {
        gLogIOstream <<
          "Transpose '" <<
          transpose->asString () <<
          "' ignored because it is already present in staff " <<
          getStaffName () <<
          "\" in part " <<
          fStaffPartUplink->getPartCombinedName () <<
          endl;
      }
#endif

      doAppendTransposeToStaff = false;
    }
  }
  
  if (doAppendTransposeToStaff) {
    // register transpose as current staff transpose
    fStaffCurrentTranspose = transpose;
  
    // propagate it to all voices
    appendTransposeToAllStaffVoices (transpose);
  }
}

void msrStaff::appendPartNameDisplayToStaff (
  S_msrPartNameDisplay partNameDisplay)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceTranspositions || gTraceOptions->fTraceStaves) {
    gLogIOstream <<
      "Setting part name display '" <<
      partNameDisplay->asString () <<
      "' in staff " <<
      getStaffName () <<
      "\" in part " <<
      fStaffPartUplink->getPartCombinedName () <<
      endl;
  }
#endif

  // set staff transpose
  bool doAppendPartNameDisplayToStaff = true;

  /* JMI ???
  if (! fStaffCurrentTranspose) {
    doAppendPartNameDisplayToStaff = true;
  }
  
  else {
    if (partNameDisplay->isEqualTo (fStaffCurrentTranspose)) {
#ifdef TRACE_OPTIONS
      if (gTraceOptions->fTraceTranspositions || gTraceOptions->fTraceStaves) {
        gLogIOstream <<
          "Transpose '" <<
          partNameDisplay->asString () <<
          "' ignored because it is already present in staff " <<
          getStaffName () <<
          "\" in part " <<
          fStaffPartUplink->getPartCombinedName () <<
          endl;
      }
#endif

      doAppendPartNameDisplayToStaff = false;
    }
  }
  */
  
  if (doAppendPartNameDisplayToStaff) {
    // register transpose as current staff transpose
 // JMI   fStaffCurrentTranspose = partNameDisplay;
  
    // propagate it to all voices
    appendPartNameDisplayToAllStaffVoices (partNameDisplay);
  }
}

void msrStaff::appendPartAbbreviationDisplayToStaff (
  S_msrPartAbbreviationDisplay partAbbreviationDisplay)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceTranspositions || gTraceOptions->fTraceStaves) {
    gLogIOstream <<
      "Setting part abbreviation display '" <<
      partAbbreviationDisplay->asString () <<
      "' in staff " <<
      getStaffName () <<
      "\" in part " <<
      fStaffPartUplink->getPartCombinedName () <<
      endl;
  }
#endif

  // set staff transpose
  bool doAppendPartAbbreviationDisplayToStaff = true;
  
/* JMI ???
  if (! fStaffCurrentTranspose) {
    doAppendPartAbbreviationDisplayToStaff = true;
  }
  
  else {
    if (partAbbreviationDisplay->isEqualTo (fStaffCurrentTranspose)) {
#ifdef TRACE_OPTIONS
      if (gTraceOptions->fTraceTranspositions || gTraceOptions->fTraceStaves) {
        gLogIOstream <<
          "Transpose '" <<
          transpose->asString () <<
          "' ignored because it is already present in staff " <<
          getStaffName () <<
          "\" in part " <<
          fStaffPartUplink->getPartCombinedName () <<
          endl;
      }
#endif

      doAppendPartAbbreviationDisplayToStaff = false;
    }
  }
  */
  
  if (doAppendPartAbbreviationDisplayToStaff) {
    // register transpose as current staff transpose
 // JMI   fStaffCurrentTranspose = partAbbreviationDisplay;
  
    // propagate it to all voices
    appendPartAbbreviationDisplayToAllStaffVoices (partAbbreviationDisplay);
  }
}

void msrStaff::appendStaffDetailsToStaff (
  S_msrStaffDetails staffDetails)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceStaves) {
    gLogIOstream <<
      "Appending staff details '" <<
      staffDetails->asShortString () <<
      "' to staff \"" <<
      getStaffName () <<
      "\" in part " <<
      fStaffPartUplink->getPartCombinedName () <<
      endl;
  }
#endif

  // sanity check
  msrAssert (
    staffDetails != nullptr,
    "staffDetails is null");
    
  // register staff details in staff
  fStaffStaffDetails = staffDetails;

  // set staff kind accordingly if relevant
  switch (staffDetails->getStaffLinesNumber ()) {
    case 1:
      fStaffKind = msrStaff::kRythmicStaff;
      break;
    default:
      ;
  } // switch
  
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceStaves) {
    gLogIOstream <<
      "Setting staff kind to '" <<
      staffKindAsString (fStaffKind) <<
      "' in staff \"" <<
      getStaffName () <<
      "\" in part " <<
      fStaffPartUplink->getPartCombinedName () <<
      endl;
  }
#endif

  for (
    map<int, S_msrVoice>::const_iterator i = fStaffAllVoicesMap.begin ();
    i != fStaffAllVoicesMap.end ();
    i++) {
    (*i).second->
      appendStaffDetailsToVoice (staffDetails);
  } // for
}

void msrStaff::appendTransposeToAllStaffVoices (
  S_msrTranspose transpose)
{
  for (
    map<int, S_msrVoice>::const_iterator i = fStaffAllVoicesMap.begin ();
    i != fStaffAllVoicesMap.end ();
    i++) {
    (*i).second->
      appendTransposeToVoice (transpose);
  } // for
}

void msrStaff::appendPartNameDisplayToAllStaffVoices (
  S_msrPartNameDisplay partNameDisplay)
{
  for (
    map<int, S_msrVoice>::const_iterator i = fStaffAllVoicesMap.begin ();
    i != fStaffAllVoicesMap.end ();
    i++) {
    (*i).second->
      appendPartNameDisplayToVoice (partNameDisplay);
  } // for
}

void msrStaff::appendPartAbbreviationDisplayToAllStaffVoices (
  S_msrPartAbbreviationDisplay partAbbreviationDisplay)
{
  for (
    map<int, S_msrVoice>::const_iterator i = fStaffAllVoicesMap.begin ();
    i != fStaffAllVoicesMap.end ();
    i++) {
    (*i).second->
      appendPartAbbreviationDisplayToVoice (partAbbreviationDisplay);
  } // for
}

void msrStaff::appendScordaturaToStaff (
  S_msrScordatura scordatura)
{
  for (
    map<int, S_msrVoice>::const_iterator i = fStaffAllVoicesMap.begin ();
    i != fStaffAllVoicesMap.end ();
    i++) {
    (*i).second->
      appendScordaturaToVoice (
        scordatura);
  } // for
}

void msrStaff::appendAccordionRegistrationToStaff (
  S_msrAccordionRegistration
    accordionRegistration)
{
  for (
    map<int, S_msrVoice>::const_iterator i = fStaffAllVoicesMap.begin ();
    i != fStaffAllVoicesMap.end ();
    i++) {
    (*i).second->
      appendAccordionRegistrationToVoice (
        accordionRegistration);
  } // for
}

void msrStaff::appendHarpPedalsTuningToStaff (
  S_msrHarpPedalsTuning
    harpPedalsTuning)
{
  for (
    map<int, S_msrVoice>::const_iterator i = fStaffAllVoicesMap.begin ();
    i != fStaffAllVoicesMap.end ();
    i++) {
    (*i).second->
      appendHarpPedalsTuningToVoice (
        harpPedalsTuning);
  } // for
}

void msrStaff::finalizeCurrentMeasureInStaff (
  int inputLineNumber)
{
  rational
    partMeasureLengthHighTide =
      fStaffPartUplink->
        getPartMeasureLengthHighTide ();
      
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceMeasures || gTraceOptions->fTraceStaves) {
    gLogIOstream <<
      "Finalizing current measure in staff \"" <<
      getStaffName () <<
      "\", line " << inputLineNumber <<
      ", partMeasureLengthHighTide = " <<
      partMeasureLengthHighTide <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  gIndenter++;
  
  // finalize all the registered voices
  for (
    map<int, S_msrVoice>::const_iterator i = fStaffAllVoicesMap.begin ();
    i != fStaffAllVoicesMap.end ();
    i++) {
    S_msrVoice
      voice =
        (*i).second;

    switch (voice->getVoiceKind ()) {
      case msrVoice::kRegularVoice:
      case msrVoice::kHarmonyVoice:
      case msrVoice::kFiguredBassVoice:
        voice->
          finalizeCurrentMeasureInVoice (
            inputLineNumber);
        break;
    } // switch
  } // for

  gIndenter--;
}

bool msrStaff::compareVoicesToHaveHarmoniesAboveCorrespondingVoice (
  const S_msrVoice& first,
  const S_msrVoice& second)
{
  int
    firstVoiceNumber =
      first->getVoiceNumber (),
    secondVoiceNumber =
      second->getVoiceNumber ();

  if (firstVoiceNumber > K_VOICE_HARMONY_VOICE_BASE_NUMBER) {
    firstVoiceNumber -= K_VOICE_HARMONY_VOICE_BASE_NUMBER + 1;
  }
  if (secondVoiceNumber > K_VOICE_HARMONY_VOICE_BASE_NUMBER) {
    secondVoiceNumber -= K_VOICE_HARMONY_VOICE_BASE_NUMBER + 1;
  }

  bool result =
    firstVoiceNumber < secondVoiceNumber;

  return result;

  /* JMI
  switch (firstVoiceNumber) {
    case msrVoice::kRegularVoice:
      switch (secondVoiceNumber) {
        case msrVoice::kRegularVoice:
          break;
  
        case msrVoice::kHarmonyVoice:
          result =
            secondVoiceNumber - K_VOICE_HARMONY_VOICE_BASE_NUMBER
              >
            firstVoiceNumber;
          break;
  
        case msrVoice::kFiguredBassVoice:
          break;
      } // switch
      break;

    case msrVoice::kHarmonyVoice:
      switch (secondVoiceNumber) {
        case msrVoice::kRegularVoice:
          result =
            firstVoiceNumber - K_VOICE_HARMONY_VOICE_BASE_NUMBER
              >
            secondVoiceNumber;
          break;
  
        case msrVoice::kHarmonyVoice:
          break;
  
        case msrVoice::kFiguredBassVoice:
          break;
      } // switch
      break;

    case msrVoice::kFiguredBassVoice:
      switch (secondVoiceNumber) {
        case msrVoice::kRegularVoice:
          break;
  
        case msrVoice::kHarmonyVoice:
          break;
  
        case msrVoice::kFiguredBassVoice:
          break;
      } // switch
      break;
  } // switch

  return result;
  */
}

void msrStaff::finalizeStaff (int inputLineNumber)
{  
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceStaves) {
    gLogIOstream <<
      "Finalizing staff \"" <<
      getStaffName () << "\"" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  // finalize the voices
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceStaves || gTraceOptions->fTraceVoices) {
    gLogIOstream <<
      "Finalizing the voices in staff \"" <<
      getStaffName () << "\"" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  gIndenter++;
  
  for (
    map<int, S_msrVoice>::const_iterator i = fStaffAllVoicesMap.begin ();
    i != fStaffAllVoicesMap.end ();
    i++) {
    S_msrVoice
      voice = (*i).second;

    voice->
      finalizeVoice (
        inputLineNumber);

/* ??? JMI
    // set staff instrument names if still empty
    if (fStaffInstrumentName.size () == 0) {
      fStaffInstrumentName =
        fStaffPartUplink->
          getPartName ();
    }

    if (fStaffInstrumentName.size () == 0) {
      fStaffInstrumentAbbreviation =
        fStaffPartUplink->
          getPartAbbreviation ();
    }
    */
  } // for

  gIndenter--;
}

void msrStaff::acceptIn (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrStaff::acceptIn ()" <<
      endl;
  }
      
  if (visitor<S_msrStaff>*
    p =
      dynamic_cast<visitor<S_msrStaff>*> (v)) {
        S_msrStaff elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrStaff::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrStaff::acceptOut (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrStaff::acceptOut ()" <<
      endl;
  }

  if (visitor<S_msrStaff>*
    p =
      dynamic_cast<visitor<S_msrStaff>*> (v)) {
        S_msrStaff elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrStaff::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrStaff::browseData (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrStaff::browseData ()" <<
      endl;
  }

  /*
    fPartCurrentClef, fPartCurrentKey and fPartCurrentTime are used
    to populate newly created voices, not to create music proper:
    they're thus not browsed
  */

  /*
    fCurrentPartStaffDetails is used
    to populate newly created voices, not to create music proper:
    it is thus not browsed
  */

/*
  if (fStaffTuningsList.size ()) {
    for (
      list<S_msrStaffTuning>::const_iterator i = fStaffTuningsList.begin ();
      i != fStaffTuningsList.end ();
      i++) {
      // browse the voice
      msrBrowser<msrStaffTuning> browser (v);
      browser.browse (*(*i));
    } // for
    gIndenter--;
  }
*/

/* JMI may be useful???
  if (fStaffAllVoicesMap.size ()) {
    for (
      map<int, S_msrVoice>::const_iterator i = fStaffAllVoicesMap.begin ();
      i != fStaffAllVoicesMap.end ();
      i++) {
        msrBrowser<msrVoice> browser (v);
        browser.browse (*((*i).second));
    } // for
  }
  */

  if (fStaffAllVoicesList.size ()) {
    for (
      list<S_msrVoice>::const_iterator i = fStaffAllVoicesList.begin ();
      i != fStaffAllVoicesList.end ();
      i++) {
        msrBrowser<msrVoice> browser (v);
        browser.browse (*(*i));
    } // for
  }

  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% <== msrStaff::browseData ()" <<
      endl;
  }
}

ostream& operator<< (ostream& os, const S_msrStaff& elt)
{
  elt->print (os);
  return os;
}


string msrStaff::staffKindAsString (
  msrStaffKind staffKind)
{
  string result;
  
  switch (staffKind) {
    case msrStaff::kRegularStaff:
      result = "regular";
      break;
    case msrStaff::kTablatureStaff:
      result = "tablature";
      break;
    case msrStaff::kHarmonyStaff:
      result = "harmony";
      break;
    case msrStaff::kFiguredBassStaff:
      result = "figured bass";
      break;
    case msrStaff::kDrumStaff:
      result = "drum";
      break;
    case msrStaff::kRythmicStaff:
      result = "rythmic";
      break;
  } // switch

  return result;
}

string msrStaff::staffKindAsString () const
{
  return staffKindAsString (fStaffKind);
}

void msrStaff::print (ostream& os)
{
  os <<
    "Staff" " " << getStaffName () <<
    ", " << staffKindAsString () <<
    ", (number '" <<
    fStaffNumber <<
    "', " <<
    singularOrPlural (
      fStaffAllVoicesMap.size (), "voice", "voices") <<
    ", " <<
    singularOrPlural (
      fStaffRegularVoicesCounter,
      "regular voice",
      "regular voices") << // JMI
    ")" <<
    endl;

  gIndenter++;

  const int fieldwidth = 28;
  
  os <<
    setw (fieldwidth) <<
    "staffPartUplink" << " : " <<
    fStaffPartUplink->getPartCombinedName () <<
    endl <<
    setw (fieldwidth) <<
    "staffInstrumentName" << " : \"" <<
    fStaffInstrumentName <<
    "\"" <<
    endl <<
    setw (fieldwidth) <<
    "staffInstrumentAbbreviation" << " : \"" <<
    fStaffInstrumentAbbreviation <<
    "\"" <<
    endl;

  // print the staff details if any
  if (fStaffStaffDetails) {
    os <<
      fStaffStaffDetails;
  }
  else {
    os << left <<
      setw (fieldwidth) <<
      "staff details" << " : " << "none";
  }
  os <<
    endl;

/* JMI
  // print the staff tunings if any
  if (fStaffTuningsList.size ()) {
    os <<
      "Staff tunings:" <<
      endl;
      
    list<S_msrStaffTuning>::const_iterator
      iBegin = fStaffTuningsList.begin (),
      iEnd   = fStaffTuningsList.end (),
      i      = iBegin;
      
    gIndenter++;
    for ( ; ; ) {
      os <<
        (*i)->asString ();
      if (++i == iEnd) break;
      os << endl;
    } // for
    os << endl;
    gIndenter--;
  }
  os <<
    endl;
*/

  // print the  voices
  if (fStaffAllVoicesMap.size ()) {
    os <<
      endl;
    
    map<int, S_msrVoice>::const_iterator
      iBegin = fStaffAllVoicesMap.begin (),
      iEnd   = fStaffAllVoicesMap.end (),
      i      = iBegin;
      
    for ( ; ; ) {
      S_msrVoice voice = (*i).second;

/* JMI
os <<
  endl <<
  "================= voice :" <<
  endl <<
  voice <<
  endl <<
  endl;
*/

      os <<
        voice;

        /* JMI
      switch (voice->getVoiceKind ()) {
        case msrVoice::kRegularVoice:
          os <<
            voice;
          break;

        case msrVoice::kHarmonyVoice:
          if (
            gMsrOptions->fShowHarmonyVoices
              ||
            voice->getMusicHasBeenInsertedInVoice ())
            os <<
              voice;
          break;
          
        case msrVoice::kFiguredBassVoice:
          if (
            gMsrOptions->fShowFiguredBassVoices
              ||
            voice->getMusicHasBeenInsertedInVoice ())
            os <<
              voice;
          break;
      } // switch
        */

      if (++i == iEnd) break;

      os <<
        endl;
    } // for
  }

  gIndenter--;
}

void msrStaff::printSummary (ostream& os)
{
  os <<
    "Staff" " " << getStaffName () <<
    ", " << staffKindAsString () <<
    " (" <<
    singularOrPlural (
      fStaffAllVoicesMap.size (), "voice", "voices") <<
    ")" <<
    endl;

  gIndenter++;

/* JMI
  os <<
    "StaffInstrumentName: \"" <<
    fStaffInstrumentName << "\"" << endl;
*/

/* JMI
  if (fStaffTuningsList.size ()) {
    os <<
      "Staff tunings:" <<
      endl;
      
    list<S_msrStaffTuning>::const_iterator
      iBegin = fStaffTuningsList.begin (),
      iEnd   = fStaffTuningsList.end (),
      i      = iBegin;
      
    gIndenter++;
    for ( ; ; ) {
      os << (*i)->asString ();
      if (++i == iEnd) break;
      os << endl;
    } // for
    os << endl;
    gIndenter--;
  }
*/

  // print the voices names
  if (fStaffAllVoicesMap.size ()) {
    os <<
      "Voices:" <<
      endl;
  
    gIndenter++;
    
    map<int, S_msrVoice>::const_iterator
      iBegin = fStaffAllVoicesMap.begin (),
      iEnd   = fStaffAllVoicesMap.end (),
      i      = iBegin;
      
    for ( ; ; ) {
      S_msrVoice
        voice =
          (*i).second;
          
      os <<
        left <<
          voice->getVoiceName () <<
          " (" <<
          singularOrPlural (
            voice->getVoiceActualNotesCounter (),
            "actual note",
            "actual notes") <<
          ", " <<
          singularOrPlural (
            voice->getVoiceStanzasMap ().size (),
            "stanza",
            "stanzas") <<
          ")";
      if (++i == iEnd) break;
      os << endl;
    } // for

    gIndenter --;
  }

  gIndenter--;

  os <<
    endl;
}

//______________________________________________________________________________
S_msrVoiceStaffChange msrVoiceStaffChange::create (
  int        inputLineNumber,
  S_msrStaff staffToChangeTo)
{
  msrVoiceStaffChange* o =
    new msrVoiceStaffChange (
      inputLineNumber, staffToChangeTo);
  assert(o!=0);
  return o;
}

msrVoiceStaffChange::msrVoiceStaffChange (
  int        inputLineNumber,
  S_msrStaff staffToChangeTo)
    : msrElement (inputLineNumber)
{
  fStaffToChangeTo = staffToChangeTo;
}

msrVoiceStaffChange::~msrVoiceStaffChange ()
{}

S_msrVoiceStaffChange msrVoiceStaffChange::createStaffChangeNewbornClone ()
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceStaffTuning) {
    gLogIOstream <<
      "Creating a newborn clone of staff change '" <<
      asString () <<
      "'" <<
      endl;
  }
#endif

 S_msrVoiceStaffChange
    newbornClone =
      msrVoiceStaffChange::create (
        fInputLineNumber,
        fStaffToChangeTo);
  
  return newbornClone;
}

void msrVoiceStaffChange::acceptIn (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrVoiceStaffChange::acceptIn ()" <<
      endl;
  }
      
  if (visitor<S_msrVoiceStaffChange>*
    p =
      dynamic_cast<visitor<S_msrVoiceStaffChange>*> (v)) {
        S_msrVoiceStaffChange elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrVoiceStaffChange::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrVoiceStaffChange::acceptOut (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrVoiceStaffChange::acceptOut ()" <<
      endl;
  }

  if (visitor<S_msrVoiceStaffChange>*
    p =
      dynamic_cast<visitor<S_msrVoiceStaffChange>*> (v)) {
        S_msrVoiceStaffChange elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrVoiceStaffChange::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrVoiceStaffChange::browseData (basevisitor* v)
{}

string msrVoiceStaffChange::asString () const
{
  stringstream s;

  s <<
    "VoiceStaffChange" <<
    ", line " << fInputLineNumber <<
    ", " <<
    "staffToChangeTo: \"" << fStaffToChangeTo->getStaffName () << "\"";
    
  return s.str ();
}

ostream& operator<< (ostream& os, const S_msrVoiceStaffChange& elt)
{
  elt->print (os);
  return os;
}

void msrVoiceStaffChange::print (ostream& os)
{
  os <<
    asString () <<
    endl;
}

//______________________________________________________________________________ 
S_msrPart msrPart::create (
  int            inputLineNumber,
  string         partID,
  S_msrPartGroup partPartGroupUplink)
{
  msrPart* o =
    new msrPart (
      inputLineNumber,
      partID,
      partPartGroupUplink);
  assert(o!=0);
  return o;
}

msrPart::msrPart (
  int            inputLineNumber,
  string         partID,
  S_msrPartGroup partPartGroupUplink)
    : msrElement (inputLineNumber)
{
  // replace spaces in partID to set fPartID
  for_each (
    partID.begin (),
    partID.end (),
    stringSpaceReplacer (fPartID, '_'));

/* JMI
  // sanity check
  msrAssert(
    partPartGroupUplink != nullptr,
    "partPartGroupUplink is null");
    */
    
  // set part's part group uplink
  fPartPartGroupUplink = partPartGroupUplink;

  // do other initializations
  initializePart ();
}

void msrPart::initializePart ()
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceParts) {
    gLogIOstream <<
      "Creating part \"" << getPartCombinedName () << "\"" <<
      endl;
  }
#endif
  
  // is this part name in the part renaming map?
  map<string, string>::const_iterator
    it =
      gMsrOptions->fPartsRenamingMap.find (fPartID);
        
  if (it != gMsrOptions->fPartsRenamingMap.end ()) {
    // yes, rename the part accordinglingly
    fPartMsrName = (*it).second;
  }
  else {
    // coin the name from the argument
    fPartMsrName =
      "P_"+stringNumbersToEnglishWords (fPartID);
  }
  
  // initialize part number of measures
  fPartNumberOfMeasures = 0;
  
  // initialize part measure length high tide
  setPartMeasureLengthHighTide (
    fInputLineNumber,
    rational (0, 1));
}

msrPart::~msrPart ()
{}

S_msrPart msrPart::createPartNewbornClone (S_msrPartGroup partGroupClone)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceParts) {
    gLogIOstream <<
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

void msrPart::createPartFiguredStaffAndVoiceIfNotYetDone (
  int inputLineNumber)
{
  if (! fPartFiguredBassStaff) {    
    // create the part figured bass staff
#ifdef TRACE_OPTIONS
    if (
      gTraceOptions->fTraceParts
        ||
      gTraceOptions->fTraceFiguredBass
        ||
      gTraceOptions->fTraceStaves
        ||
      gTraceOptions->fTraceVoices) {
      gLogIOstream <<
        "Creating the figured bass staff" <<
        " with number " << K_PART_FIGURED_BASS_STAFF_NUMBER <<
        " for part " <<
        getPartCombinedName () <<
        ", line " << inputLineNumber <<
        endl;
    }
#endif

    gIndenter++;

    fPartFiguredBassStaff =
      addStaffToPartByItsNumber (
        inputLineNumber,
        msrStaff::kFiguredBassStaff,
        K_PART_FIGURED_BASS_STAFF_NUMBER);
      
    // create the part figured bass voice  
#ifdef TRACE_OPTIONS
    if (gTraceOptions->fTraceHarmonies || gTraceOptions->fTraceVoices) {
      gLogIOstream <<
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
        msrVoice::kFiguredBassVoice,
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

/* JMI
  gLogIOstream <<
    endl <<
    "***********" <<
    endl <<
    endl;
  print (gLogIOstream);
  gLogIOstream <<
    "***********" <<
    endl <<
    endl;
  */  
}

void msrPart::addAVoiceToStavesThatHaveNone (
  int inputLineNumber)
{
  for (
    map<int, S_msrStaff>::const_iterator i = fPartStavesMap.begin ();
    i != fPartStavesMap.end ();
    i++) {
    (*i).second->
      addAVoiceToStaffIfItHasNone (
        inputLineNumber);
  } // for
}

void msrPart::setPartMeasureLengthHighTide (
  int      inputLineNumber,
  rational measureLength)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceDivisions || gTraceOptions->fTraceMeasures) {
    gLogIOstream <<
      "Setting measure length high tide for part \"" <<
      getPartCombinedName () <<
      "\" to " << measureLength <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  fPartMeasureLengthHighTide = measureLength;
}

void msrPart::updatePartMeasureLengthHighTide (
  int      inputLineNumber,
  rational measureLength)
{
  if (measureLength > fPartMeasureLengthHighTide) {
#ifdef TRACE_OPTIONS
    if (gTraceOptions->fTraceDivisions || gTraceOptions->fTraceMeasures) {
      gLogIOstream <<
        "Updating measure length high tide for part \"" <<
        getPartCombinedName () <<
        "\" to " << measureLength <<
        ", line " << inputLineNumber <<
        endl;
    }
#endif

    fPartMeasureLengthHighTide = measureLength;
  }
}

void msrPart::padUpToMeasureLengthInPart (
  int      inputLineNumber,
  rational measureLength)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceParts || gTraceOptions->fTraceMeasures) {
    gLogIOstream <<
      "Padding up to measure length '" << measureLength <<
      "' in part \"" <<
      getPartCombinedName () <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  gIndenter++;
  
  // pad the registered staves up to measure length  
  for (
    map<int, S_msrStaff>::const_iterator i = fPartStavesMap.begin ();
    i != fPartStavesMap.end ();
    i++) {
    (*i).second->
      padUpToMeasureLengthInStaff (
        inputLineNumber,
        measureLength);
  } // for

  gIndenter--;
}

void msrPart::setPartMsrName (string partMsrName)
{
  // is this part name in the part renaming map?
  map<string, string>::const_iterator
    it =
      gMsrOptions->fPartsRenamingMap.find (fPartMsrName);
        
  if (it != gMsrOptions->fPartsRenamingMap.end ()) {
    // yes, rename the part accordinglingly
    fPartMsrName = (*it).second;

#ifdef TRACE_OPTIONS
    if (gTraceOptions->fTraceParts) {
      gLogIOstream <<
        "Setting part name of " << getPartCombinedName () <<
        " to \"" << fPartMsrName << "\"" <<
         endl;
    }
#endif
  }
  else {
    // use the argument
    fPartMsrName = partMsrName;

#ifdef TRACE_OPTIONS
    if (gTraceOptions->fTraceParts) {
      gLogIOstream <<
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

  if (! fPartMsrName.size ())
    s <<
      "[empty name]";
  else
    s <<
      "\"" << fPartMsrName << "\"";

  s <<
    " (partID \"" << fPartID << "\"";

  if (fPartName.size ())
    s <<
      ", partName \"" << fPartName << "\"";

  s <<
    ")";

  return s.str ();
}

void msrPart::createMeasureAndAppendItToPart (
  int    inputLineNumber,
  string measureNumber,
  int    measureOrdinalNumber,
  msrMeasure::msrMeasureImplicitKind
         measureImplicitKind)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceMeasures) {
    gLogIOstream <<
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
    i++) {
    S_msrStaff
      staff = (*i).second;
      
    staff->
      createMeasureAndAppendItToStaff (
        inputLineNumber,
        measureNumber,
        measureOrdinalNumber,
        measureImplicitKind);
  } // for

  gIndenter--;
}

void msrPart::setNextMeasureNumberInPart (
  int    inputLineNumber,
  string nextMeasureNumber)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceMeasures) {
    gLogIOstream <<
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
    i++) {
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
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceMeasures) {
    gLogIOstream <<
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
    i++) {
    S_msrStaff
      staff = (*i).second;
      / * JMI
    staff->
      createMeasureAndAppendItToStaff (
        inputLineNumber,
        measureNumber,
        measureOrdinalNumber,
        measureImplicitKind);
  } // for
}
*/

void msrPart::appendStaffDetailsToPart (
  S_msrStaffDetails staffDetails)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceStaves || gTraceOptions->fTraceParts) {
    gLogIOstream <<
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
    i++) {
    S_msrStaff
      staff = (*i).second;

    staff->
      appendStaffDetailsToStaff (
        staffDetails);
  } // for
}

void msrPart::appendClefToPart (S_msrClef clef)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceParts || gTraceOptions->fTraceClefs) {
    gLogIOstream <<
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
    i++) {
    (*i).second->
      appendClefToStaff (
        clef);
  } // for

  gIndenter--;
}

void msrPart::appendKeyToPart  (S_msrKey  key)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceKeys || gTraceOptions->fTraceParts) {
    gLogIOstream <<
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
    i++) {
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
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceTimes || gTraceOptions->fTraceParts) {
    gLogIOstream <<
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
    i++) {
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
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceTimes || gTraceOptions->fTraceParts) {
    gLogIOstream <<
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
    i++) {
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
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceTranspositions || gTraceOptions->fTraceParts) {
    gLogIOstream <<
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
    i++) {
    S_msrStaff
      staff = (*i).second;

    staff->
      appendTransposeToStaff (transpose);
  } // for
}

void msrPart::appendPartNameDisplayToPart (
  S_msrPartNameDisplay partNameDisplay)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceTranspositions || gTraceOptions->fTraceParts) {
    gLogIOstream <<
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
    i++) {
    S_msrStaff
      staff = (*i).second;

    staff->
      appendPartNameDisplayToStaff (partNameDisplay);
  } // for
}

void msrPart::appendPartAbbreviationDisplayToPart (
  S_msrPartAbbreviationDisplay partAbbreviationDisplay)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceTranspositions || gTraceOptions->fTraceParts) {
    gLogIOstream <<
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
    i++) {
    S_msrStaff
      staff = (*i).second;

    staff->
      appendPartAbbreviationDisplayToStaff (partAbbreviationDisplay);
  } // for
}

void msrPart::prepareForRepeatInPart (
  int inputLineNumber)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceRepeats) {
    gLogIOstream <<
      "Preparing for repeat in part " <<
      getPartCombinedName () <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  gIndenter++;
  
  for (
    map<int, S_msrStaff>::const_iterator i = fPartStavesMap.begin ();
    i != fPartStavesMap.end ();
    i++) {
    (*i).second->
      prepareForRepeatInStaff (
        inputLineNumber);
  } // for

  gIndenter--;
}

void msrPart::nestContentsIntoNewRepeatInPart (
  int inputLineNumber)
{
  for (
    map<int, S_msrStaff>::const_iterator i = fPartStavesMap.begin ();
    i != fPartStavesMap.end ();
    i++) {
    (*i).second->
      nestContentsIntoNewRepeatInStaff (
        inputLineNumber);
  } // for
}

void msrPart::createRepeatUponItsEndAndAppendItToPart (
  int    inputLineNumber,
  string measureNumber,
  int    repeatTimes)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceRepeats) {
    gLogIOstream <<
      "Creating and appending a repeat to part \"" <<
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
    i++) {
    (*i).second->
      createRepeatUponItsEndAndAppendItToStaff (
        inputLineNumber,
        measureNumber,
        repeatTimes);
  } // for

  gIndenter--;
}

void msrPart::createRegularRepeatUponItsFirstEndingInPart (
  int inputLineNumber,
  int repeatTimes)
{
  // create repeat and append it to registered staves
  for (
    map<int, S_msrStaff>::const_iterator i = fPartStavesMap.begin ();
    i != fPartStavesMap.end ();
    i++) {
    (*i).second->
      createRegularRepeatUponItsFirstEndingInStaff (
        inputLineNumber,
        repeatTimes);
  } // for
}

void msrPart::createEnclosingRepeatUponItsFirstEndingInPart (
  int inputLineNumber,
  int repeatTimes)
{
  // create repeat and append it to registered staves
  for (
    map<int, S_msrStaff>::const_iterator i = fPartStavesMap.begin ();
    i != fPartStavesMap.end ();
    i++) {
    (*i).second->
      createEnclosingRepeatUponItsFirstEndingInStaff (
        inputLineNumber,
        repeatTimes);
  } // for
}

void msrPart::appendRepeatEndingToPart (
  int       inputLineNumber,
  string    repeatEndingNumber, // may be "1, 2"
  msrRepeatEnding::msrRepeatEndingKind
            repeatEndingKind)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceRepeats) {
    gLogIOstream <<
      "Appending a repeat ending to part \"" <<
      getPartCombinedName () <<
      "\"" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  gIndenter++;
  
  // append repeat ending to registered staves
  for (
    map<int, S_msrStaff>::const_iterator i = fPartStavesMap.begin ();
    i != fPartStavesMap.end ();
    i++) {
    (*i).second->
      appendRepeatEndingToStaff (
        inputLineNumber,
        repeatEndingNumber,
        repeatEndingKind);
  } // for

  gIndenter--;
}

void msrPart::appendRepeatCloneToPart (
  int         inputLineNumber,
  S_msrRepeat repeatCLone)
{
  for (
    map<int, S_msrStaff>::const_iterator i = fPartStavesMap.begin ();
    i != fPartStavesMap.end ();
    i++) {
    S_msrStaff
      staff =
        (*i).second;

    switch (staff->getStaffKind ()) {
      case msrStaff::kRegularStaff:
        staff->appendRepeatCloneToStaff (
          inputLineNumber, repeatCLone);
        break;
        
      case msrStaff::kTablatureStaff:
        break;
        
      case msrStaff::kHarmonyStaff:
        break;
        
      case msrStaff::kFiguredBassStaff:
        break;

      case msrStaff::kDrumStaff:
        staff->appendRepeatCloneToStaff (
          inputLineNumber, repeatCLone);
        break;

      case msrStaff::kRythmicStaff:
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
    i++) {
    (*i).second->
      appendRepeatEndingCloneToStaff (repeatEndingCLone);
  } // for
}

void msrPart::createMeasuresRepeatFromItsFirstMeasuresInPart (
  int inputLineNumber,
  int measuresRepeatMeasuresNumber,
  int measuresRepeatSlashes)
{
  // create measure repeat from its first measure in registered staves
  for (
    map<int, S_msrStaff>::const_iterator i = fPartStavesMap.begin ();
    i != fPartStavesMap.end ();
    i++) {
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
  // append pending measure repeat to registered staves
  for (
    map<int, S_msrStaff>::const_iterator i = fPartStavesMap.begin ();
    i != fPartStavesMap.end ();
    i++) {
    (*i).second->
      appendPendingMeasuresRepeatToStaff (
        inputLineNumber);
  } // for
}

void msrPart::createMultipleRestInPart (
  int inputLineNumber,
  int multipleRestMeasuresNumber)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceRepeats || gTraceOptions->fTraceParts) {
    gLogIOstream <<
      "Creating a multiple rest in part " <<
      getPartCombinedName () <<
      ", " <<
      singularOrPlural (
        multipleRestMeasuresNumber, "measure", "measures") <<
      endl;
  }
#endif

  // create multiple rest in registered staves
  for (
    map<int, S_msrStaff>::const_iterator i = fPartStavesMap.begin ();
    i != fPartStavesMap.end ();
    i++) {
    (*i).second->
      createMultipleRestInStaff (
        inputLineNumber,
        multipleRestMeasuresNumber);
  } // for
}

void msrPart::appendPendingMultipleRestToPart (
  int inputLineNumber)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceRepeats || gTraceOptions->fTraceParts) {
    gLogIOstream <<
      "Appending the pending multiple rest to part " <<
      getPartCombinedName () <<
      endl;
  }
#endif

  // append pending multiple rest to registered staves
  for (
    map<int, S_msrStaff>::const_iterator i = fPartStavesMap.begin ();
    i != fPartStavesMap.end ();
    i++) {
    (*i).second->
      appendPendingMultipleRestToStaff (
        inputLineNumber);
  } // for
}

void msrPart::appendMultipleRestCloneToPart (
  int               inputLineNumber,
  S_msrMultipleRest multipleRest)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceStaves || gTraceOptions->fTraceParts) {
    gLogIOstream <<
      "Appending multiple rest '" <<
      multipleRest->asString () <<
      "' to part clone " <<
      getPartCombinedName () <<
      endl;
  }
#endif

  for (
    map<int, S_msrStaff>::const_iterator i = fPartStavesMap.begin ();
    i != fPartStavesMap.end ();
    i++) {
    (*i).second->
      appendMultipleRestCloneToStaff (
        inputLineNumber,
        multipleRest);
  } // for
}

void msrPart::appendBarlineToPart (S_msrBarline barline)
{
  // append barline to registered staves
  for (
    map<int, S_msrStaff>::const_iterator i = fPartStavesMap.begin ();
    i != fPartStavesMap.end ();
    i++) {
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
      gXml2lyOptions->fInputSourceName,
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());
      
    return fPartStavesMap [staffNumber];
  }

#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceStaves || gTraceOptions->fTraceParts) {
    gLogIOstream <<
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
    case msrStaff::kRegularStaff:
    case msrStaff::kTablatureStaff:
    case msrStaff::kHarmonyStaff:
    case msrStaff::kFiguredBassStaff:
    case msrStaff::kDrumStaff:
    case msrStaff::kRythmicStaff:
      fPartStavesMap [staffNumber] = staff;
      break;
  } // switch
  
  // initialize staff current clef and key if relevant // JMI
  switch (staffKind) {
    case msrStaff::kRegularStaff:
    case msrStaff::kTablatureStaff:
    case msrStaff::kHarmonyStaff:
    case msrStaff::kFiguredBassStaff:
    case msrStaff::kDrumStaff:
    case msrStaff::kRythmicStaff:
      staff->setStaffCurrentClef (fPartCurrentClef);
      staff->setStaffCurrentKey (fPartCurrentKey);
      break;
  } // switch
  
  // return staff
  return staff;
}

void msrPart::addStaffToPartCloneByItsNumber (S_msrStaff staff)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceStaves || gTraceOptions->fTraceParts) {
    gLogIOstream <<
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
    case msrVoice::kRegularVoice:
      // create the figured bass staff and voice if not yet done
      createPartFiguredStaffAndVoiceIfNotYetDone (
        inputLineNumber);

      /* JMI ???
      // register this voice as the part figured bass supplier voice
      setPartFiguredBassSupplierVoice (
        inputLineNumber,
        figuredBassSupplierVoice);
    */
    
      // append the figured bass to the part figured bass voice
#ifdef TRACE_OPTIONS
      if (gTraceOptions->fTraceHarmonies || gTraceOptions->fTraceParts) {
        gLogIOstream <<
          "Appending figured bass '" <<
          figuredBass->asString () <<
          "' to part " <<
          getPartCombinedName () <<
          ", line " << inputLineNumber <<
          endl;
      }
#endif
    
      fPartFiguredBassVoice->
        appendFiguredBassToVoice (figuredBass);
      break;
      
    case msrVoice::kHarmonyVoice:
    case msrVoice::kFiguredBassVoice:
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
          gXml2lyOptions->fInputSourceName,
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
    case msrVoice::kFiguredBassVoice:
      // create the figured bass staff and voice if not yet done
      createPartFiguredStaffAndVoiceIfNotYetDone (
        inputLineNumber);

      /* JMI NON
      // register this voice as the part figuredBass supplier voice
      setPartfiguredBassSupplierVoice (
        inputLineNumber,
        figuredBassSupplierVoice);
        */
    
      // append the figured bass to the part figured bass voice
#ifdef TRACE_OPTIONS
      if (gTraceOptions->fTraceFiguredBass || gTraceOptions->fTraceParts) {
        gLogIOstream <<
          "Appending figured bass '" <<
          figuredBass->asString () <<
          "' to part clone " <<
          getPartCombinedName () <<
          ", line " << inputLineNumber <<
          endl;
      }
#endif
    
      fPartFiguredBassVoice->
        appendFiguredBassToVoiceClone (figuredBass);
      break;
      
    case msrVoice::kRegularVoice:
    case msrVoice::kHarmonyVoice:
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
          gXml2lyOptions->fInputSourceName,
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
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTracePasses || gTraceOptions->fTraceParts) {
    gLogIOstream <<
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
    i++) {
    (*i).second->
      appendScordaturaToStaff (
        scordatura);
  } // for
}

void msrPart::appendAccordionRegistrationToPart (
  S_msrAccordionRegistration
    accordionRegistration)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTracePasses || gTraceOptions->fTraceParts) {
    gLogIOstream <<
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
    i++) {
    (*i).second->
      appendAccordionRegistrationToStaff (
        accordionRegistration);
  } // for
}

void msrPart::appendHarpPedalsTuningToPart (
  S_msrHarpPedalsTuning
    harpPedalsTuning)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTracePasses || gTraceOptions->fTraceParts) {
    gLogIOstream <<
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
    i++) {
    (*i).second->
      appendHarpPedalsTuningToStaff (
        harpPedalsTuning);
  } // for
}

void msrPart:: handleBackup (
  int inputLineNumber,
  int divisions,
  int divisionsPerQuarterNote)
{
#ifdef TRACE_OPTIONS
  if (
    gTraceOptions->fTraceParts
      ||
    gTraceOptions->fTraceDivisions
      ||
    gTraceOptions->fTraceMeasures) {
    gLogIOstream <<
      "Handling backup, divisions = '" <<
      divisions <<
      "' in part " <<
      getPartCombinedName () <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  // compute the backup step length
  rational
    backupStepLength =
      rational (
        divisions,
        divisionsPerQuarterNote * 4); // hence a whole note    
  
  // determine the measure position 'divisions' backward
  rational
    measurePosition =
      fPartMeasureLengthHighTide - backupStepLength;

  measurePosition.rationalise ();

  // bring the part back to that measure position
  padUpToMeasureLengthInPart (
    inputLineNumber,
    measurePosition);
}

void msrPart:: handleForward (
  int        inputLineNumber,
  int        divisions,
  int        divisionsPerQuarterNote,
  S_msrVoice voice)
{
#ifdef TRACE_OPTIONS
  if (
    gTraceOptions->fTraceParts
      ||
    gTraceOptions->fTraceDivisions
      ||
    gTraceOptions->fTraceMeasures) {
    gLogIOstream <<
      "Handling forward, divisions = '" <<
      divisions <<
      " in voice \"" << voice->getVoiceName () << "\"" <<
      "' in part " <<
      getPartCombinedName () <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

/* Don't do anything... JMI
  // compute the backup step length
  rational
    forwardStepLength =
      rational (
        divisions,
        divisionsPerQuarterNote * 4); // hence a whole note    
  
  // determine the measure position 'divisions' backward
  rational
    measurePosition =
      forwardStepLength; // + what ??? JMI

  measurePosition.rationalise ();

  // bring the voice forward to that measure position
  padUpToMeasureLengthInPart (
    inputLineNumber,
    measurePosition);
    */
}

/* JMI
void msrPart::appendSkipGraceNotesToVoicesClones ( // JMI ???
  S_msrVoice      graceNotesOriginVoice,
  S_msrGraceNotes skipGraceNotes)
{
  int inputLineNumber =
    skipGraceNotes->getInputLineNumber ();

  rational
    graceNotesOriginVoiceMeasureLength =
      graceNotesOriginVoice->
        getVoiceLastSegment ()->
          getSegmentMeasuresList ().back ()->
            getMeasureLength ();
        
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceMeasures || gTraceOptions->fTraceParts) {
    gLogIOstream <<
      "appendSkipGraceNotesToVoicesClones () in " <<
      getPartCombinedName () <<
      ", graceNotesOriginVoiceMeasureLength = " <<
      graceNotesOriginVoiceMeasureLength <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  for (
    map<int, S_msrStaff>::const_iterator i=fPartStavesMap.begin ();
    i!=fPartStavesMap.end ();
    i++) {

    map<int, S_msrVoice>
      staffAllVoicesMap =
        (*i).second->
          getStaffAllVoicesMap ();
          
    for (
      map<int, S_msrVoice>::const_iterator j=staffAllVoicesMap.begin ();
      j!=staffAllVoicesMap.end ();
      j++) {

      S_msrVoice voice = (*j).second;
      
      if (voice != graceNotesOriginVoice) {
        voice->
          appendAFirstMeasureToVoiceIfNotYetDone ( // JMI
            inputLineNumber);
            
        // bring voice to the same measure length as graceNotesOriginVoice
        voice->
          padUpToMeasureLengthInVoice (
            inputLineNumber,
            graceNotesOriginVoiceMeasureLength);
        
        // append skip grace notes to voice
        voice->
          appendGraceNotesToVoice (
            skipGraceNotes);
      }
    } // for

  } // for
}
*/

void msrPart::prependSkipGraceNotesToVoicesClones (
  S_msrVoice      graceNotesOriginVoice,
  S_msrGraceNotes skipGraceNotes)
{
  int inputLineNumber =
    skipGraceNotes->getInputLineNumber ();

  rational
    graceNotesOriginVoiceMeasureLength =
      graceNotesOriginVoice->
        getVoiceLastSegment ()->
          getSegmentMeasuresList ().back ()->
            getMeasureLength ();
        
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceMeasures || gTraceOptions->fTraceParts) {
    gLogIOstream <<
      "prependSkipGraceNotesToVoicesClones () in " <<
      getPartCombinedName () <<
      ", graceNotesOriginVoiceMeasureLength = " <<
      graceNotesOriginVoiceMeasureLength <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  for (
    map<int, S_msrStaff>::const_iterator i=fPartStavesMap.begin ();
    i!=fPartStavesMap.end ();
    i++) {

    map<int, S_msrVoice>
      staffAllVoicesMap =
        (*i).second->
          getStaffAllVoicesMap ();
          
    for (
      map<int, S_msrVoice>::const_iterator j=staffAllVoicesMap.begin ();
      j!=staffAllVoicesMap.end ();
      j++) {

      S_msrVoice voice = (*j).second;
      
      if (voice != graceNotesOriginVoice) {
        voice->
          appendAFirstMeasureToVoiceIfNotYetDone ( // JMI
            inputLineNumber);
            
        // bring voice to the same measure length as graceNotesOriginVoice
        voice->
          padUpToMeasureLengthInVoice (
            inputLineNumber,
            graceNotesOriginVoiceMeasureLength);
        
        // append skip grace notes to voice
        voice->
          prependGraceNotesToVoice (
            skipGraceNotes);
      }
    } // for

  } // for
}

void msrPart::finalizeCurrentMeasureInPart (
  int    inputLineNumber)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceMeasures || gTraceOptions->fTraceParts) {
    gLogIOstream <<
      "Finalizing current measure in part " <<
      getPartCombinedName () <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  gIndenter++;
  
  // finalize current measure in registered staves
  for (
    map<int, S_msrStaff>::const_iterator i = fPartStavesMap.begin ();
    i != fPartStavesMap.end ();
    i++) {
    (*i).second->
      finalizeCurrentMeasureInStaff (
        inputLineNumber);
  } // for

  // reset measure length high tide
  setPartMeasureLengthHighTide (
    fInputLineNumber,
    rational (0, 1));

  gIndenter--;
}

void msrPart::setPartInstrumentNamesMaxLengthes ()
{
  S_msrScore
    score =
      fPartPartGroupUplink->
        getPartGroupScoreUplink ();
        
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

string msrPart::asString () const
{
  stringstream s;

  s <<
    "Part \"" <<
    getPartName () <<
    "\", line " << fInputLineNumber;

  return s.str ();
}

void msrPart::finalizePart (
  int inputLineNumber)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceParts) {
    gLogIOstream <<
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
      gXml2lyOptions->fInputSourceName,
      inputLineNumber,
      s.str ());
  }
  
  else {
    // finalize registered staves
    for (
      map<int, S_msrStaff>::const_iterator i = fPartStavesMap.begin ();
      i != fPartStavesMap.end ();
      i++) {
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
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceParts) {
    gLogIOstream <<
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
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrPart::acceptIn ()" <<
      endl;
  }
      
  if (visitor<S_msrPart>*
    p =
      dynamic_cast<visitor<S_msrPart>*> (v)) {
        S_msrPart elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrPart::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrPart::acceptOut (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrPart::acceptOut ()" <<
      endl;
  }

  if (visitor<S_msrPart>*
    p =
      dynamic_cast<visitor<S_msrPart>*> (v)) {
        S_msrPart elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrPart::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrPart::browseData (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrPart::browseData ()" <<
      endl;
  }

  // browse all non figured bass staves
  for (
    map<int, S_msrStaff>::const_iterator i = fPartStavesMap.begin ();
    i != fPartStavesMap.end ();
    i++) {
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

ostream& operator<< (ostream& os, const S_msrPart& elt)
{
  elt->print (os);
  return os;
}

void msrPart::print (ostream& os)
{
  os <<
    "Part" << " " << getPartCombinedName () <<
    " (" <<
    singularOrPlural (
      fPartStavesMap.size (), "staff", "staves") <<
    ")" <<
    endl;
    
  gIndenter++;
  
  const int fieldWidth = 27;

  os << left <<
    setw (fieldWidth) <<
    "partGroupUplink" << " : ";
  if (fPartPartGroupUplink) {
    // it may be empty
    os <<
      fPartPartGroupUplink->getPartGroupCombinedName ();
  }
  else {
    os << "none";
  }
  os <<
    endl;
  
  os << left <<
    setw (fieldWidth) <<
    "PartMsrName" << " : \"" <<
    fPartMsrName << "\"" <<
    endl <<
    
    setw (fieldWidth) <<
    "PartName" << " : \"" <<
    fPartName << "\"" <<
    endl <<
    setw (fieldWidth) <<
    "PartNameDisplayText" << " : \"" <<
    fPartNameDisplayText << "\"" <<
    endl <<
    
    setw (fieldWidth) <<
    "PartAbbrevation" << " : \"" <<
    fPartAbbreviation << "\"" <<
    endl <<
    setw (fieldWidth) <<
    "PartAbbreviationDisplayText" << " : \"" <<
    fPartAbbreviationDisplayText << "\"" <<
    endl <<
    
    setw (fieldWidth) <<
     "PartInstrumentName" << " : \"" <<
    fPartInstrumentName << "\"" <<
    endl <<    
    setw (fieldWidth) <<
     "PartInstrumentAbbreviation" << " : \"" <<
    fPartInstrumentAbbreviation << "\"" <<
    endl <<
    
    setw (fieldWidth) <<
    "PartNumberOfMeasures" << " : " <<
    fPartNumberOfMeasures <<
    endl <<
    endl;

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
          
      msrStaff::msrStaffKind
        staffKind =
          staff->getStaffKind ();

      switch (staffKind) { // JMI
        case msrStaff::kRegularStaff:
          os <<
            staff;
          break;
          
        case msrStaff::kTablatureStaff:
          os <<
            staff;
          break;
          
        case msrStaff::kHarmonyStaff:
    // JMI      if (gMsrOptions->fShowHarmonyVoices)
          os <<
            staff;
          break;
          
        case msrStaff::kFiguredBassStaff:
    // JMI      if (gMsrOptions->fShowFiguredBassVoices)
          os <<
            staff;
          break;

        case msrStaff::kDrumStaff:
          os <<
            staff;
          break;
          
        case msrStaff::kRythmicStaff:
          os <<
            staff;
          break;          
      } // switch
      
      if (++i == iEnd) break;
      
      os <<
        endl;
    } // for

    gIndenter--;
  }

  gIndenter--;
}

void msrPart::printSummary (ostream& os)
{
  os <<
    "Part" << " " << getPartCombinedName () <<
    " (" <<
    singularOrPlural (
      fPartStavesMap.size (), "staff", "staves") <<
    ", " <<
    singularOrPlural (
      fPartNumberOfMeasures, "measure", "measure") <<
    ", length high tide " << fPartMeasureLengthHighTide <<
    ")" <<
    endl;
    
  gIndenter++;
  
  const int fieldWidth = 27;

  os << left <<
    setw (fieldWidth) <<
    "PartMsrName" << " : \"" <<
    fPartMsrName << "\"" <<
    endl <<
    
    setw (fieldWidth) <<
    "PartName" << " : \"" <<
    fPartName << "\"" <<
    endl <<
    setw (fieldWidth) <<
    "PartNameDisplayText" << " : \"" <<
    fPartNameDisplayText << "\"" <<
    endl <<
    
    setw (fieldWidth) <<
    "PartAbbrevation" << " : \"" <<
    fPartAbbreviation << "\"" <<
    endl <<
    setw (fieldWidth) <<
    "PartAbbreviationDisplayText" << " : \"" <<
    fPartAbbreviationDisplayText << "\"" <<
    endl <<
    
    setw (fieldWidth) <<
     "PartInstrumentName" << " : \"" <<
    fPartInstrumentName << "\"" <<
    endl <<    
    setw (fieldWidth) <<
     "PartInstrumentAbbreviation" << " : \"" <<
    fPartInstrumentAbbreviation << "\"" <<
    endl <<
    
    setw (fieldWidth) <<
    "PartNumberOfMeasures" << " : " <<
    fPartNumberOfMeasures <<
    endl <<
    endl;

  // print the staves
  if (fPartStavesMap.size ()) {
    os << 
      "Part staves" <<
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
      "Figured bass staff" <<
      endl;
            
    gIndenter++;
    os <<
      fPartFiguredBassStaff;
    gIndenter--;
  }

  gIndenter--;
}

//______________________________________________________________________________
S_msrPartGroup msrPartGroup::create (
  int                      inputLineNumber,
  int                      partGroupNumber,
  int                      partGroupAbsoluteNumber,
  string                   partGroupName,
  string                   partGroupNameDisplayText,
  string                   partGroupAccidentalText,
  string                   partGroupAbbreviation,
  msrPartGroupSymbolKind   partGroupSymbolKind,
  int                      partGroupSymbolDefaultX,
  msrPartGroupBarlineKind  partGroupBarlineKind,
  S_msrPartGroup           partGroupPartGroupUplink,
  S_msrScore               partGroupScoreUplink)
{
  msrPartGroup* o =
    new msrPartGroup (
      inputLineNumber,
      partGroupNumber,
      partGroupAbsoluteNumber,
      partGroupName,
      partGroupNameDisplayText,
      partGroupAccidentalText,
      partGroupAbbreviation,
      partGroupSymbolKind,
      partGroupSymbolDefaultX,
      msrPartGroup::kPartGroupImplicitNo,
      partGroupBarlineKind,
      partGroupPartGroupUplink,
      partGroupScoreUplink);
  assert(o!=0);
  return o;
}

S_msrPartGroup msrPartGroup::createImplicitPartGroup (
  int                      partGroupNumber,
  int                      partGroupAbsoluteNumber,
  string                   partGroupName,
  string                   partGroupNameDisplayText,
  string                   partGroupAccidentalText,
  string                   partGroupAbbreviation,
  msrPartGroupBarlineKind  partGroupBarlineKind,
  S_msrScore               partGroupScoreUplink)
{
  msrPartGroup* o =
    new msrPartGroup (
      NO_INPUT_LINE_NUMBER,
      partGroupNumber,
      partGroupAbsoluteNumber,
      partGroupName,
      partGroupNameDisplayText,
      partGroupAccidentalText,
      partGroupAbbreviation,
      msrPartGroup::kPartGroupSymbolNone, // partGroupSymbolKind
      0,                                  // partGroupSymbolDefaultX,
      msrPartGroup::kPartGroupImplicitYes,
      partGroupBarlineKind,
      0,                                  // partGroupPartGroupUplink,
                                          // will be set below
      partGroupScoreUplink);
  assert(o!=0);

  // the implicit part group it the top-most one:
  // set its group uplink points to itself
  o->fPartGroupPartGroupUplink = o;
  
  return o;
}

msrPartGroup::msrPartGroup (
  int                      inputLineNumber,
  int                      partGroupNumber,
  int                      partGroupAbsoluteNumber,
  string                   partGroupName,
  string                   partGroupNameDisplayText,
  string                   partGroupAccidentalText,
  string                   partGroupAbbreviation,
  msrPartGroupSymbolKind   partGroupSymbolKind,
  int                      partGroupSymbolDefaultX,
  msrPartGroupImplicitKind partGroupImplicitKind,
  msrPartGroupBarlineKind  partGroupBarlineKind,
  S_msrPartGroup           partGroupPartGroupUplink,
  S_msrScore               partGroupScoreUplink)
    : msrElement (inputLineNumber)
{
  // no sanity check on partGroupPartGroupUplink here,
  // it will be set after all 'real' (i.e. not implicit)
  // part groups and part have been analyzed
  fPartGroupPartGroupUplink = partGroupPartGroupUplink;

/* JMI
  // sanity check
  msrAssert (
    fPartGroupScoreUplink != nullptr,
    "fPartGroupScoreUplink is null");
    */
    
  fPartGroupScoreUplink     = partGroupScoreUplink;

  // other fields
  fPartGroupNumber          = partGroupNumber;
  fPartGroupAbsoluteNumber  = partGroupAbsoluteNumber;
          
  fPartGroupName            = partGroupName;

  int partGroupNameLength =
    fPartGroupName.size ();
  
  if (
    partGroupNameLength
      >
    fPartGroupScoreUplink->getScorePartGroupNamesMaxLength ()) {  // JMI sanity check ???
    fPartGroupScoreUplink->
      setScorePartGroupNamesMaxLength (
        partGroupNameLength);
  }
          
  fPartGroupNameDisplayText = partGroupNameDisplayText;
  
  fPartGroupAccidentalText  = partGroupAccidentalText;
  
  fPartGroupAbbreviation    = partGroupAbbreviation;

  fPartGroupSymbolKind      = partGroupSymbolKind;
  fPartGroupSymbolDefaultX  = partGroupSymbolDefaultX;

  fPartGroupImplicitKind    = partGroupImplicitKind;
    
  fPartGroupBarlineKind     = partGroupBarlineKind;

#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTracePartGroups) {
    gLogIOstream <<
      "--------------------------------------------" <<
      endl <<
      "Creating part group '" << fPartGroupNumber << "'" <<
      ", partGroupAbsoluteNumber = " << fPartGroupAbsoluteNumber <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif
}

msrPartGroup::~msrPartGroup ()
{}

S_msrPartGroup msrPartGroup::createPartGroupNewbornClone (
  S_msrPartGroup partGroupClone,
  S_msrScore     scoreClone)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTracePartGroups) {
    gLogIOstream <<
      "--------------------------------------------" <<
      endl <<
      "Creating a newborn clone part group " <<
      getPartGroupCombinedName () <<
      endl;
  }
#endif

  // don't check partGroupClone against 0, since the partGroup stack JMI
  // that it comes from may be empty
/* JMI
  // sanity check
  msrAssert(
    partGroupClone != nullptr,
    "partGroupClone is null");
    */
    
  // sanity check
  msrAssert(
    scoreClone != nullptr,
    "scoreClone is null");
    
  S_msrPartGroup
    newbornClone =
      msrPartGroup::create (
        fInputLineNumber,
        fPartGroupNumber,
        fPartGroupAbsoluteNumber,
        fPartGroupName,
        fPartGroupNameDisplayText,
        fPartGroupAccidentalText,
        fPartGroupAbbreviation,
        fPartGroupSymbolKind,
        fPartGroupSymbolDefaultX,
        fPartGroupBarlineKind,
        partGroupClone,
        scoreClone);
  
  newbornClone->fPartGroupImplicitKind =
    fPartGroupImplicitKind;
  
  newbornClone->fPartGroupInstrumentName =
    fPartGroupInstrumentName;
  
  return newbornClone;
}

string msrPartGroup::getPartGroupCombinedName () const
{
  stringstream s;

  s <<
    "PartGroup_" << fPartGroupAbsoluteNumber <<
    " ('" << fPartGroupNumber <<
    "', partGroupName \"" << fPartGroupName << "\")";

  return s.str ();
}

void msrPartGroup::setPartGroupInstrumentName (
  string partGroupInstrumentName)
{
  fPartGroupInstrumentName = partGroupInstrumentName;

  S_msrScore
    score =
      fPartGroupScoreUplink;
        
  int partGroupInstrumentNameLength = fPartGroupInstrumentName.size ();
  
  if (
    partGroupInstrumentNameLength
      >
    score->getScoreInstrumentNamesMaxLength ())
    score->
      setScoreInstrumentNamesMaxLength (
        partGroupInstrumentNameLength);
}

S_msrPart msrPartGroup::appendPartToPartGroupByItsPartID (
  int    inputLineNumber,
  string partID)
{
  // sanity check
  msrAssert (
    partID.size () > 0,
    "partID is empty");

  // has this partID already been added to this part?
  if (fPartGroupPartsMap.count (partID)) {
    stringstream s;

    s <<
      "partID \"" << partID <<
      "\" already exists in part group " <<
      getPartGroupCombinedName ();

    msrMusicXMLWarning ( // JMI
      gXml2lyOptions->fInputSourceName,
      inputLineNumber,
      s.str ());

    return fPartGroupPartsMap [partID];
  }

  // create the part
  S_msrPart
    part =
      msrPart::create (
        inputLineNumber,
        partID,
        this);

  // register it in this part group
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTracePartGroups) {
    gLogIOstream <<
      "Adding part " <<
      part->getPartCombinedName () <<
      " to part group '" <<
      fPartGroupNumber <<
      "'" <<
      endl;
  }
#endif
  
  fPartGroupPartsMap [partID] = part;
  fPartGroupElements.push_back (part);

#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTracePartGroupsDetails) {
    gLogIOstream <<
      endl <<
      "After appendPartToPartGroupByItsID, fPartGroupPartsMap contains:" <<
      endl;
        
    gIndenter++;
    
    for (
        map<string, S_msrPart>::const_iterator i = fPartGroupPartsMap.begin ();
        i != fPartGroupPartsMap.end ();
        i++) {
      gLogIOstream <<
        "\"" << (*i).first << "\" --% --> " <<
        (*i).second->
          getPartCombinedName () <<
        endl;
    } // for
    
    gIndenter--;
    
    gLogIOstream <<
      "After appendPartToPartGroupByItsID, fPartGroupElements contains:" <<
      endl;
      
    gIndenter++;

    if (fPartGroupElements.size ()) {
      list<S_msrElement>::const_iterator
        iBegin = fPartGroupElements.begin (),
        iEnd   = fPartGroupElements.end (),
        i      = iBegin;
        
      for ( ; ; ) {
        gLogIOstream <<
          (*i);
        if (++i == iEnd) break;
        gLogIOstream <<
          endl;
      } // for
    }
    
    gIndenter--;
  }
#endif
  
  // return the part
  return part;
}

void msrPartGroup::appendPartToPartGroup (S_msrPart part)
{
  // register part in this part group
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTracePartGroups) {
    gLogIOstream <<
      "Adding part " <<
      part->getPartCombinedName () <<
      " to part group " << fPartGroupNumber <<
      endl;
  }
#endif

  // register part into this part group's data
  fPartGroupPartsMap [part->getPartID ()] = part;
  fPartGroupElements.push_back (part);

  // set part's partgroup uplink
  part->setPartPartGroupUplink (this);
}

void msrPartGroup::removePartFromPartGroup (
  int       inputLineNumber,
  S_msrPart partToBeRemoved)
{
  // register part in this part group
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTracePartGroups) {
    gLogIOstream <<
      "Removing part " <<
      partToBeRemoved->getPartCombinedName () <<
      " from part group " << fPartGroupNumber <<
      endl;
  }
#endif

  gIndenter++;
  
  for (
    list<S_msrElement>::iterator i = fPartGroupElements.begin ();
    i != fPartGroupElements.end ();
    i++) {
    S_msrElement
      element = (*i);

    if (
      S_msrPartGroup
        nestedPartGroup =
          dynamic_cast<msrPartGroup*>(&(*element))
      ) {
      // this is a part group          
    }

    else if (
      S_msrPart
        part =
          dynamic_cast<msrPart*>(&(*element))
      ) {
      // this is a part
      if (part == partToBeRemoved) {
        fPartGroupElements.erase (i);
        break;
      }
    }

    else {
      stringstream s;

      s <<
        "an element of partgroup " <<
        getPartGroupCombinedName () <<
        " is not a part group nor a part";

      msrInternalError (
        gXml2lyOptions->fInputSourceName,
        inputLineNumber,
        __FILE__, __LINE__,
        s.str ());
    }
  } // for

  gIndenter--;
}

void msrPartGroup::prependSubPartGroupToPartGroup (
  S_msrPartGroup partGroup)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTracePartGroups) {
    gLogIOstream <<
      "Prepending (sub-)part group " << partGroup->getPartGroupNumber () <<
      " to part group " << getPartGroupNumber ()  << endl;
  }
#endif

  // register it in this part group
  fPartGroupElements.push_front (partGroup);
}

void msrPartGroup::appendSubPartGroupToPartGroup (
  S_msrPartGroup partGroup)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTracePartGroups) {
    gLogIOstream <<
      "Appending (sub-)part group " << partGroup->getPartGroupNumber () <<
      " to part group " << getPartGroupNumber ()  << endl;
  }
#endif

  // register it in this part group
  fPartGroupElements.push_back (partGroup);
}


void msrPartGroup::printPartGroupParts (
  int      inputLineNumber,
  ostream& os)
{
  for (
    list<S_msrElement>::const_iterator i = fPartGroupElements.begin ();
    i != fPartGroupElements.end ();
    i++) {
    S_msrElement
      element = (*i);

    if (
      S_msrPartGroup
        nestedPartGroup =
          dynamic_cast<msrPartGroup*>(&(*element))
      ) {
      // this is a part group          
      gLogIOstream <<
        nestedPartGroup->
          getPartGroupCombinedName () <<
        endl;

      gIndenter++;

      nestedPartGroup->
        printPartGroupParts (
          inputLineNumber,
          os);
      
      gIndenter--;
    }

    else if (
      S_msrPart
        part =
          dynamic_cast<msrPart*>(&(*element))
      ) {
      // this is a part
      gLogIOstream <<
        part->
          getPartCombinedName () <<
        endl;
    }

    else {
      stringstream s;

      s <<
        "an element of partgroup " <<
        getPartGroupCombinedName () <<
        " is not a part group nor a part";

      msrInternalError (
        gXml2lyOptions->fInputSourceName,
        inputLineNumber,
        __FILE__, __LINE__,
        s.str ());
    }
  } // for
}

S_msrPart msrPartGroup::fetchPartFromPartGroupByItsPartID (
  int    inputLineNumber,
  string partID)
{
  S_msrPart result;
  
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTracePartGroupsDetails) {    
    gLogIOstream <<
      "fetchPartFromPartGroupByItsPartID(" << partID << "), fPartGroupElements contains:" <<
      endl;

    gIndenter++;

    printPartGroupParts (
      inputLineNumber,
      gLogIOstream);
    
    gIndenter--;

    gLogIOstream <<
      "<=- fetchPartFromPartGroupByItsPartID(" << partID << ")" <<
      endl <<
      endl;
  }
#endif

/* JMI
  list<S_msrElement>::const_iterator
    iBegin = fPartGroupElements.begin (),
    iEnd   = fPartGroupElements.end (),
    i      = iBegin;
  */
    
  for (
    list<S_msrElement>::const_iterator i = fPartGroupElements.begin ();
    i != fPartGroupElements.end ();
    i++) {
    S_msrElement
      element = (*i);

    if (
      S_msrPartGroup
        partGroup =
          dynamic_cast<msrPartGroup*>(&(*element))
      ) {
      // this is a part group          
      S_msrPart
        inter =
          partGroup->
            fetchPartFromPartGroupByItsPartID (
              inputLineNumber,
              partID);

      if (inter) {
        result = inter;
        break;
      }
    }

    else if (
      S_msrPart
        part =
          dynamic_cast<msrPart*>(&(*element))
      ) {
      // this is a part
      if (part->getPartID () == partID) {
        result = part;
        break;
      }
    }

    else {
      stringstream s;

      s <<
        "an element of partgroup " <<
        getPartGroupCombinedName () <<
        " is not a part group nor a part";

      msrInternalError (
        gXml2lyOptions->fInputSourceName,
        inputLineNumber,
        __FILE__, __LINE__,
        s.str ());
    }
  } // for

  return result;
}

void msrPartGroup::collectPartGroupPartsList (
  int              inputLineNumber,
  list<S_msrPart>& partsList)
{
  for (
    list<S_msrElement>::const_iterator i = fPartGroupElements.begin ();
    i != fPartGroupElements.end ();
    i++) {
    S_msrElement
      element = (*i);

    if (
      S_msrPartGroup
        partGroup =
          dynamic_cast<msrPartGroup*>(&(*element))
      ) {
      // this is a part group          
      partGroup->
        collectPartGroupPartsList (
          inputLineNumber,
          partsList);
    }

    else if (
      S_msrPart
        part =
          dynamic_cast<msrPart*>(&(*element))
      ) {
      // this is a part
      partsList.push_back (part);
    }

    else {
      stringstream s;

      s <<
        "an element of partgroup " <<
        getPartGroupCombinedName () <<
        " is not a part group nor a part";

      msrInternalError (
        gXml2lyOptions->fInputSourceName,
        inputLineNumber,
        __FILE__, __LINE__,
        s.str ());
    }
  } // for
}

void msrPartGroup::acceptIn (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrPartGroup::acceptIn ()" <<
      endl;
  }
      
  if (visitor<S_msrPartGroup>*
    p =
      dynamic_cast<visitor<S_msrPartGroup>*> (v)) {
        S_msrPartGroup elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrPartGroup::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrPartGroup::acceptOut (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrPartGroup::acceptOut ()" <<
      endl;
  }

  if (visitor<S_msrPartGroup>*
    p =
      dynamic_cast<visitor<S_msrPartGroup>*> (v)) {
        S_msrPartGroup elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrPartGroup::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrPartGroup::browseData (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrPartGroup::browseData ()" <<
      endl;
  }
  
  for (
    list<S_msrElement>::const_iterator i = fPartGroupElements.begin ();
    i != fPartGroupElements.end ();
    i++) {
    // browse the part element
    msrBrowser<msrElement> browser (v);
    browser.browse (*(*i));
  } // for

  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% <== msrPartGroup::browseData ()" <<
      endl;
  }
}

string msrPartGroup::partGroupImplicitKindAsString (
  msrPartGroupImplicitKind partGroupImplicitKind)
{
  string result;
  
  switch (partGroupImplicitKind) {
    case msrPartGroup::kPartGroupImplicitYes:
      result = "partGroupImplicitYes";
      break;
    case msrPartGroup::kPartGroupImplicitNo:
      result = "partGroupImplicitNo";
      break;
  } // switch

  return result;
}

string msrPartGroup::partGroupImplicitKindAsString () const
{
  return
    partGroupImplicitKindAsString (
      fPartGroupImplicitKind);
}

string msrPartGroup::partGroupTypeKindAsString (
  msrPartGroupTypeKind partGroupTypeKind)
{
  string result;
  
  switch (partGroupTypeKind) {
    case msrPartGroup::kPartGroupTypeNone:
      result = "partGroupTypeNone";
      break;
    case msrPartGroup::kPartGroupTypeStart:
      result = "partGroupTypeStart";
      break;
    case msrPartGroup::kPartGroupTypeStop:
      result = "partGroupTypeStop";
      break;
  } // switch

  return result;
}

string msrPartGroup::partGroupBarlineKindAsString () const
{
  return
    partGroupBarlineKindAsString (
      fPartGroupBarlineKind);
}

string msrPartGroup::asString () const
{
  stringstream s;

  s <<
    "PartGroup \"" <<
    getPartGroupCombinedName () <<
    "\", line " << fInputLineNumber;

  return s.str ();
}

string msrPartGroup::partGroupSymbolKindAsString (
  msrPartGroupSymbolKind partGroupSymbolKind)
{
  string result;
  
  switch (partGroupSymbolKind) {
    case msrPartGroup::kPartGroupSymbolNone:
      result = "partGroupSymbolNone";
      break;
    case msrPartGroup::kPartGroupSymbolBrace:
      result = "partGroupSymbolBrace";
      break;
    case msrPartGroup::kPartGroupSymbolBracket:
      result = "partGroupSymbolBracket";
      break;
    case msrPartGroup::kPartGroupSymbolLine:
      result = "partGroupSymbolLine";
      break;
    case msrPartGroup::kPartGroupSymbolSquare:
      result = "partGroupSymbolSquare";
      break;
  } // switch

  return result;
}

string msrPartGroup::partGroupBarlineKindAsString (
  msrPartGroupBarlineKind partGroupBarlineKind)
{
  string result;
  
  switch (partGroupBarlineKind) {
    case msrPartGroup::kPartGroupBarlineYes:
      result = "partGroupBarlineYes";
      break;
    case msrPartGroup::kPartGroupBarlineNo:
      result = "partGroupBarlineNo";
      break;
  } // switch

  return result;
}

void msrPartGroup::print (ostream& os)
{
  os <<
    "PartGroup" " \"" << getPartGroupCombinedName () <<
    "\" (" <<
    singularOrPlural (
      fPartGroupPartsMap.size (), "part", "parts") <<
    ")" <<
    endl;
    
  gIndenter++;

  const int fieldWidth = 25;
  
  os << left <<
    setw (fieldWidth) <<
    "partGroupPartGroupUplink" << " : ";

  if (fPartGroupPartGroupUplink) {
    // it may be empty
    os <<
      fPartGroupPartGroupUplink->
        getPartGroupCombinedName ();
  }
  else {
    os <<
      "none";
  }
  os <<
    endl;

  os << left <<
    setw (fieldWidth) <<
    "partGroupName" << " : \"" <<
    fPartGroupName <<
    "\"" <<
    endl;

   os << left <<
    setw (fieldWidth) <<
    "partGroupPartGroupUplink" << " : ";
  if (fPartGroupPartGroupUplink)
    os <<
      "\"" <<
      fPartGroupPartGroupUplink->
        getPartGroupCombinedName () <<
      "\"";
  else
    os <<
    "none";
  os <<
    endl;

  os << left <<
   setw (fieldWidth) <<
    "partGroupNameDisplayText" << " : \"" <<
    fPartGroupNameDisplayText <<
    "\"" <<
    endl <<
    setw (fieldWidth) <<
    "partGroupAccidentalText" << " : \"" <<
    fPartGroupAccidentalText <<
    "\"" <<
    endl <<
    setw (fieldWidth) <<
    "partGroupAbbrevation" << " : \"" <<
    fPartGroupAbbreviation <<
    "\"" <<
    endl <<
    setw (fieldWidth) <<
    "partGroupSymbolDefaultX" << " : " <<
    fPartGroupSymbolDefaultX <<
      endl <<
    setw (fieldWidth) <<
    "partGroupSymbolKind" << " : " <<
    partGroupSymbolKindAsString (
      fPartGroupSymbolKind) <<
    endl;
    
  os << left <<
    setw (fieldWidth) <<
    "partGroupImplicit" << " : " <<
    partGroupImplicitKindAsString (
      fPartGroupImplicitKind) <<
    endl;

  os << left <<
    setw (fieldWidth) <<
    "partGroupBarline" << " : " <<
    partGroupBarlineKindAsString (
      fPartGroupBarlineKind) <<
    endl;

  if (fPartGroupElements.size ()) {
    os << endl;
    list<S_msrElement>::const_iterator
      iBegin = fPartGroupElements.begin (),
      iEnd   = fPartGroupElements.end (),
      i      = iBegin;
      
    for ( ; ; ) {
      os << (*i);
      if (++i == iEnd) break;
      os << endl;
    } // for
  }
  
  gIndenter--;
}

ostream& operator<< (ostream& os, const S_msrPartGroup& elt)
{
  elt->print (os);
  return os;
}
          
void msrPartGroup::printSummary (ostream& os)
{
  os <<
    "PartGroup" " \"" << getPartGroupCombinedName () <<
    "\" (" <<
    singularOrPlural (
      fPartGroupPartsMap.size (), "part", "parts") <<
    ")" <<
    endl;
    
  gIndenter++;

  const int fieldWidth = 24;
  
  os << left <<
    setw (fieldWidth) <<
    "PartGroupName" << " : \"" <<
    fPartGroupName <<
    "\"" <<
    endl <<
    setw (fieldWidth) <<
    "PartGroupAbbrevation" << " : \"" <<
    fPartGroupAbbreviation <<
    "\"" <<
    endl <<
    setw (fieldWidth) <<
    "fPartGroupSymbolDefaultX" << " : " <<
    fPartGroupSymbolDefaultX <<
      endl <<
    setw (fieldWidth) <<
    "fPartGroupSymbolKind" << " : \"" <<
    partGroupSymbolKindAsString (
      fPartGroupSymbolKind) <<
    "\"" <<
    endl <<
    
    setw (fieldWidth) <<
    "PartGroupImplicit" << " : " <<
    partGroupImplicitKindAsString (
      fPartGroupImplicitKind) <<
    endl <<

    setw (fieldWidth) <<
    "PartGroupBarline" << " : " <<
    partGroupBarlineKindAsString (
      fPartGroupBarlineKind) <<
    endl;

  if (fPartGroupElements.size ()) {
    os << endl;
    list<S_msrElement>::const_iterator
      iBegin = fPartGroupElements.begin (),
      iEnd   = fPartGroupElements.end (),
      i      = iBegin;
      
    gIndenter++;
    for ( ; ; ) {
      (*i)->printSummary (os);
      if (++i == iEnd) break;
      os << endl;
    } // for
    gIndenter--;
  }
  
  gIndenter--;
}

//______________________________________________________________________________
S_msrScore msrScore::create (
  int inputLineNumber)
{
  msrScore* o =
    new msrScore (
      inputLineNumber);
  assert(o!=0);
  return o;
}

msrScore::msrScore (
  int inputLineNumber)
    : msrElement (inputLineNumber)
{
  // create the identification
  fIdentification =
    msrIdentification::create (
      inputLineNumber);

  // create the page geometry
  fPageGeometry =
    msrPageGeometry::create (
      inputLineNumber);

  // number of measures
  fScoreNumberOfMeasures = -1;

  // part group names max length
  fScorePartGroupNamesMaxLength = -1;

  // part names max length
  fScorePartNamesMaxLength = -1;
  
  // set instrument names max lengthes
  fScoreInstrumentNamesMaxLength      = -1;
  fScoreInstrumentAbbreviationsMaxLength = -1;
  
  // measure repeats replicas should be browsed by default
  fInhibitMeasuresRepeatReplicasBrowsing = false;

  // multiple rest measures should be browsed by default
  fInhibitMultipleRestMeasuresBrowsing = false;
}

msrScore::~msrScore ()
{}

S_msrScore msrScore::createScoreNewbornClone ()
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceScore) {
    gLogIOstream <<
      "Creating a newborn clone of a score" <<
      endl;
  }
#endif

  S_msrScore
    newbornClone =
      msrScore::create (
        fInputLineNumber);

/* JMI
  for (
    list<S_msrPartGroup>::const_iterator i = fCreditsList.begin ();
    i != fCreditsList.end ();
    i++) {
    // append the credit to the clone
    appendCreditToScore (*i);
  } // for
*/

  // number of measures
  newbornClone->fScoreNumberOfMeasures =
    fScoreNumberOfMeasures;

  // part group names max length

  newbornClone->fScorePartGroupNamesMaxLength =
    fScorePartGroupNamesMaxLength;

  // part names max length

  newbornClone->fScorePartNamesMaxLength =
    fScorePartNamesMaxLength;

  // instrument names max lengthes
  
  newbornClone->fScoreInstrumentNamesMaxLength =
    fScoreInstrumentNamesMaxLength;
    
  newbornClone->fScoreInstrumentAbbreviationsMaxLength =
    fScoreInstrumentAbbreviationsMaxLength;
    
  // inhibiting browsing

  newbornClone->fInhibitMeasuresRepeatReplicasBrowsing =
    fInhibitMeasuresRepeatReplicasBrowsing;
    
  newbornClone->fInhibitMultipleRestMeasuresBrowsing =
    fInhibitMultipleRestMeasuresBrowsing;
    
  return newbornClone;
}

void msrScore::addPartGroupToScore (S_msrPartGroup partGroup)
{
  if (fScorePartGroupsSet.count (partGroup)) {      
    stringstream s;
    
    s <<      
      "part group '" <<
      partGroup->getPartGroupCombinedName () <<
      "' already exists in this score";

    msrInternalError (
      gXml2lyOptions->fInputSourceName,
      partGroup->getInputLineNumber (),
      __FILE__, __LINE__,
      s.str ());
  }

  // register it in this score
  fScorePartGroupsSet.insert (partGroup);
  fPartGroupsList.push_back (partGroup);
}

void msrScore::appendCreditToScore (S_msrCredit credit)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceCredits) {
    gLogIOstream <<
      "Appending credit '" <<
      credit->asString () <<
      "' to score" <<
      endl;
  }
#endif
  
  fCreditsList.push_back (credit);
}

S_msrPart msrScore::fetchPartFromScoreByItsPartID (
  int    inputLineNumber,
  string partID)
{
  S_msrPart result;
  
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTracePartGroupsDetails) {
    gLogIOstream <<
      "fetchPartFromScoreByItsPartID(" << partID << "), fPartGroupsList contains:" <<
      endl;
      
    gIndenter++;

    for (
      list<S_msrPartGroup>::const_iterator i = fPartGroupsList.begin ();
        i != fPartGroupsList.end ();
        i++) {
      gLogIOstream <<
        (*i)->getPartGroupCombinedName () <<
        ", " <<
        (*i)->getPartGroupName () <<
        endl;
    } // for
  
    gIndenter--;

    gLogIOstream <<
      "<=- fetchPartFromScoreByItsPartID(" << partID << ")" <<
      endl <<
      endl;
  }
#endif

  for (
    list<S_msrPartGroup>::const_iterator i = fPartGroupsList.begin ();
    i != fPartGroupsList.end ();
    i++) {
    S_msrPart
      part =
        (*i)->
          fetchPartFromPartGroupByItsPartID (
            inputLineNumber,
            partID);

    if (part) {
      result = part;
      break;
    }
  } // for

  return result;
}

void msrScore::collectScorePartsList (
  int              inputLineNumber,
  list<S_msrPart>& partsList)
{
  S_msrPart result;

  for (
    list<S_msrPartGroup>::const_iterator i = fPartGroupsList.begin ();
    i != fPartGroupsList.end ();
    i++) {
    S_msrPartGroup
      partGroup = (*i);
      partGroup->
        collectPartGroupPartsList (
          inputLineNumber,
          partsList);
  } // for
}

/*
S_msrPartGroup msrScore::fetchScorePartGroup (
  int partGroupNumber)
{
  S_msrPartGroup result;
  
  if (fScorePartGroupsMap.count (partGroupNumber)) {
    result = fScorePartGroupsMap [partGroupNumber];
  }

  return result;
}
*/

void msrScore::acceptIn (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrScore::acceptIn ()" <<
      endl;
  }
      
  if (visitor<S_msrScore>*
    p =
      dynamic_cast<visitor<S_msrScore>*> (v)) {
        S_msrScore elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrScore::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrScore::acceptOut (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrScore::acceptOut ()" <<
      endl;
  }

  if (visitor<S_msrScore>*
    p =
      dynamic_cast<visitor<S_msrScore>*> (v)) {
        S_msrScore elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrScore::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrScore::browseData (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrScore::browseData ()" <<
      endl;
  }
    
  if (fIdentification) {
    // browse identification
    msrBrowser<msrIdentification> browser (v);
    browser.browse (*fIdentification);
  }

  if (fPageGeometry) {
    // browse page geometry
    msrBrowser<msrPageGeometry> browser (v);
    browser.browse (*fPageGeometry);
  }

  for (
    list<S_msrCredit>::const_iterator i = fCreditsList.begin ();
    i != fCreditsList.end ();
    i++) {
    // browse the part credit
    msrBrowser<msrCredit> browser (v);
    browser.browse (*(*i));
  } // for

  for (
    list<S_msrPartGroup>::const_iterator i = fPartGroupsList.begin ();
    i != fPartGroupsList.end ();
    i++) {
    // browse the part group
    msrBrowser<msrPartGroup> browser (v);
    browser.browse (*(*i));
  } // for

  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% <== msrScore::browseData ()" <<
      endl;
  }
}

ostream& operator<< (ostream& os, const S_msrScore& elt)
{
  elt->print (os);
  return os;
}

void msrScore::print (ostream& os)
{
  os <<
    "MSR Score" <<
    endl;

  gIndenter++;

  const int fieldWidth = 38;

  int partGroupsListSize =
    fPartGroupsList.size ();
    
  os << left <<
    setw (fieldWidth) <<
    singularOrPluralWithoutNumber (
      partGroupsListSize,
      "part group", "part groups") <<
    " : " <<
    partGroupsListSize <<
    endl <<

    setw (fieldWidth) <<
    singularOrPluralWithoutNumber (
      fScoreNumberOfMeasures,
      "measure", "measures") <<
    " : " <<
    fScoreNumberOfMeasures <<
    endl <<

    setw (fieldWidth) <<
    "scorePartGroupNamesMaxLength" <<  " : " <<
    fScorePartGroupNamesMaxLength <<
    endl<<

    setw (fieldWidth) <<
    "scorePartNamesMaxLength" <<  " : " <<
    fScorePartNamesMaxLength <<
    endl<<

    setw (fieldWidth) <<
    "scoreInstrumentNamesMaxLength" <<  " : " <<
    fScoreInstrumentNamesMaxLength <<
    endl<<

    setw (fieldWidth) <<
    "scoreInstrumentAbbreviationsMaxLength" <<  " : " <<
    fScoreInstrumentAbbreviationsMaxLength <<
    endl<<

    setw (fieldWidth) <<
    "inhibitMultipleRestMeasuresBrowsing" <<  " : " <<
    booleanAsString (
      fInhibitMultipleRestMeasuresBrowsing) <<
    endl<<
    endl;

  // print the identification if any
  if (fIdentification) {
    os <<
      fIdentification;
  }
  
  // print the page geometry if any
  if (fPageGeometry) {
    os <<
      fPageGeometry;
  }

  // print the credits if any
  if (fCreditsList.size ()) {
    list<S_msrCredit>::const_iterator
      iBegin = fCreditsList.begin (),
      iEnd   = fCreditsList.end (),
      i      = iBegin;
    for ( ; ; ) {
      os << (*i);
      if (++i == iEnd) break;
      os << endl;
    } // for
  }

  // print the part groups if any
  if (partGroupsListSize) {
    list<S_msrPartGroup>::const_iterator
      iBegin = fPartGroupsList.begin (),
      iEnd   = fPartGroupsList.end (),
      i      = iBegin;
    for ( ; ; ) {
      os << (*i);
      if (++i == iEnd) break;
      os << endl;
    } // for
  }
  else {
    os <<
      "There are no part groups in the list" <<
      endl;
  }
  
  gIndenter--;
}

void msrScore::printSummary (ostream& os)
{
  os <<
    "MSR component" <<
    endl;

  gIndenter++;

  const int fieldWidth = 38;

  int partGroupsListSize =
    fPartGroupsList.size ();

  // print global information
  os << left <<
    setw (fieldWidth) <<
    singularOrPluralWithoutNumber (
      partGroupsListSize,
      "part group", "part groups") <<
    " : " <<
    partGroupsListSize <<
    endl <<

    setw (fieldWidth) <<
    singularOrPluralWithoutNumber (
      fScoreNumberOfMeasures,
      "measure", "measures") <<
    " : " <<
    fScoreNumberOfMeasures <<
    endl <<

    setw (fieldWidth) <<
    "scoreInstrumentNamesMaxLength" <<  " : " <<
    fScoreInstrumentNamesMaxLength <<
    endl <<

    setw (fieldWidth) <<
    "scoreInstrumentAbbreviationsMaxLength" <<  " : " <<
    fScoreInstrumentAbbreviationsMaxLength <<
    endl<<

    setw (fieldWidth) <<
    "inhibitMultipleRestMeasuresBrowsing" <<  " : " <<
    booleanAsString (
      fInhibitMultipleRestMeasuresBrowsing) <<
    endl<<
    endl;

  // print the implicit part group contents
  // it is the only element in fPartGroupsList JMI single variable
  if (partGroupsListSize) {
    os <<
      "Parts and part groups structure:" <<
      endl;
  
    gIndenter++;
    
    fPartGroupsList.front () ->
      printPartGroupParts (
        fInputLineNumber,
        os);
  
    gIndenter--;

    os <<
      endl;
  }

  // print the identification if any
  if (fIdentification) {
    os <<
      fIdentification;
  }
  
  // print the page geometry if any
  if (fPageGeometry) {
    os <<
      fPageGeometry;
  }
  
  // print the credits if any
  if (fCreditsList.size ()) {
    list<S_msrCredit>::const_iterator
      iBegin = fCreditsList.begin (),
      iEnd   = fCreditsList.end (),
      i      = iBegin;
    for ( ; ; ) {
      os << (*i);
      if (++i == iEnd) break;
      os << endl;
    } // for
  }
  
  if (partGroupsListSize) {
    list<S_msrPartGroup>::const_iterator
      iBegin = fPartGroupsList.begin (),
      iEnd   = fPartGroupsList.end (),
      i      = iBegin;
    for ( ; ; ) {
      (*i)->
        printSummary (os);
      if (++i == iEnd) break;
      os << endl;
    } // for
  }
  
  gIndenter--;
}

//______________________________________________________________________________
void initializeMSR ()
{
  initializeMSRBasicTypes ();
}


}


/*

S_msrMeasuresRepeat msrMeasuresRepeat::createMeasuresRepeatNewbornClone (
  S_msrVoice containingVoice)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceRepeats) {
    gLogIOstream <<
      "Creating a newborn clone of a measure repeat" <<
      endl;
  }
#endif
  
  // sanity check
  msrAssert(
    containingVoice != nullptr,
    "containingVoice is null");
    
  S_msrMeasuresRepeat
    newbornClone =
      msrMeasuresRepeat::create (
        fInputLineNumber,
        fMeasuresRepeatMeasuresNumber,
        fMeasuresRepeatSlashesNumber,
        containingVoice);

  // numbers
  
  // repeat pattern

  // repeat replicas

  // uplinks

  return newbornClone;
}

S_msrMeasuresRepeat msrMeasuresRepeat::createMeasuresRepeatDeepCopy (
  S_msrVoice containingVoice)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceRepeats) {
    gLogIOstream <<
      "Creating a deep copy of a measure repeat" <<
      endl;
  }
#endif
  
  // sanity check
  msrAssert(
    containingVoice != nullptr,
    "containingVoice is null");
    
  S_msrMeasuresRepeat
    measuresRepeatDeepCopy =
      msrMeasuresRepeat::create (
        fInputLineNumber,
        fMeasuresRepeatMeasuresNumber,
        fMeasuresRepeatSlashesNumber,
        containingVoice);

  // numbers

  // repeat pattern
  measuresRepeatDeepCopy->fMeasuresRepeatPattern =
    fMeasuresRepeatPattern->
      createMeasuresRepeatPatternDeepCopy (
        containingVoice);
        
  // repeat replicas

   measuresRepeatDeepCopy->fMeasuresRepeatReplicas =
    fMeasuresRepeatReplicas->
      createMeasuresRepeatReplicasDeepCopy (
        containingVoice);
        
  // uplinks
  measuresRepeatDeepCopy->fMeasuresRepeatVoiceUplink =
    containingVoice;
    
  return measuresRepeatDeepCopy;
}


S_msrMeasuresRepeatReplicas msrMeasuresRepeatReplicas::createMeasuresRepeatReplicasNewbornClone (
  S_msrVoice containingVoice)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceRepeats) {
    gLogIOstream <<
      "Creating a newborn clone of a measure repeat replicas" <<
      endl;
  }
#endif
  
  // sanity check
  msrAssert(
    containingVoice != nullptr,
    "containingVoice is null");
    
  S_msrMeasuresRepeatReplicas
    newbornClone =
      msrMeasuresRepeatReplicas::create (
        fInputLineNumber,
        fMeasuresRepeatUplink); // JMI

  // segment

  // uplinks
  newbornClone->fMeasuresRepeatUplink =
    fMeasuresRepeatUplink; // JMI
    
  return newbornClone;
}

S_msrMeasuresRepeatReplicas msrMeasuresRepeatReplicas::createMeasuresRepeatReplicasDeepCopy (
  S_msrVoice containingVoice)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceRepeats) {
    gLogIOstream <<
      "Creating a deep copy of a measure repeat replicas" <<
      endl;
  }
#endif
  
  // sanity check
  msrAssert(
    containingVoice != nullptr,
    "containingVoice is null");
    
  S_msrMeasuresRepeatReplicas
    measuresRepeatReplicasDeepCopy =
      msrMeasuresRepeatReplicas::create (
        fInputLineNumber,
        fMeasuresRepeatUplink); // JMI

  // segment
  measuresRepeatReplicasDeepCopy->fMeasuresRepeatReplicasSegment =
    fMeasuresRepeatReplicasSegment->
      createSegmentDeepCopy (
        containingVoice); // JMI

  // uplinks
  measuresRepeatReplicasDeepCopy->fMeasuresRepeatUplink =
    fMeasuresRepeatUplink; // JMI
    
  return measuresRepeatReplicasDeepCopy;
}


S_msrMeasuresRepeatPattern msrMeasuresRepeatPattern::createMeasuresRepeatPatternNewbornClone (
  S_msrVoice containingVoice)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceRepeats) {
    gLogIOstream <<
      "Creating a newborn clone of a measure repeat pattern" <<
      endl;
  }
#endif
  
  // sanity check
  msrAssert(
    containingVoice != nullptr,
    "containingVoice is null");
    
  S_msrMeasuresRepeatPattern
    newbornClone =
      msrMeasuresRepeatPattern::create (
        fInputLineNumber,
        fMeasuresRepeatUplink); // JMI

  // segment

  // uplinks
  newbornClone->fMeasuresRepeatUplink =
    fMeasuresRepeatUplink; // JMI
    
  return newbornClone;
}

S_msrMeasuresRepeatPattern msrMeasuresRepeatPattern::createMeasuresRepeatPatternDeepCopy (
  S_msrVoice containingVoice)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceRepeats) {
    gLogIOstream <<
      "Creating a newborn clone of a measure repeat pattern" <<
      endl;
  }
#endif
  
  // sanity check
  msrAssert(
    containingVoice != nullptr,
    "containingVoice is null");
    
  S_msrMeasuresRepeatPattern
    measuresRepeatPatternDeepCopy =
      msrMeasuresRepeatPattern::create (
        fInputLineNumber,
        fMeasuresRepeatUplink); // JMI

  // segment
  // fMeasuresRepeatPatternSegment JMI ???
  
  // uplinks
  measuresRepeatPatternDeepCopy->fMeasuresRepeatUplink =
    fMeasuresRepeatUplink; // JMI
    
  return measuresRepeatPatternDeepCopy;
}


S_msrRepeat msrRepeat::createRepeatNewbornClone (
  S_msrVoice containingVoice)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceRepeats) {
    gLogIOstream <<
      "Creating a newborn clone of a repeat" <<
      endl;
  }
#endif
  
  // sanity check
  msrAssert(
    containingVoice != nullptr,
    "containingVoice is null");
    
  S_msrRepeat
    newbornClone =
      msrRepeat::create (
        fInputLineNumber,
        repeatTimes,
        containingVoice);

  // common segment

  // repeat endings
  newbornClone->fRepeatEndingsInternalCounter =
    fRepeatEndingsInternalCounter;
    
  // uplinks

  return newbornClone;
}

S_msrRepeat msrRepeat::createRepeatDeepCopy (
  S_msrVoice containingVoice)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceRepeats) {
    gLogIOstream <<
      "Creating a deep copy of a repeat" <<
      endl;
  }
#endif
  
  // sanity check
  msrAssert(
    containingVoice != nullptr,
    "containingVoice is null");
    
  S_msrRepeat
    repeatDeepCopy =
      msrRepeat::create (
        fInputLineNumber,
        repeatTimes,
        containingVoice);
/ *
  // common segment
  repeatDeepCopy->fRepeatCommonSegment =
    fRepeatCommonSegment->
      createSegmentDeepCopy (
        containingVoice);
  * /      
  // common part
  repeatDeepCopy->fRepeatCommonPart =
    fRepeatCommonPart->
      createRepeatCommonPartDeepCopy (
        repeatDeepCopy);
        
  // repeat endings
  for (
    vector<S_msrRepeatEnding>::const_iterator i = fRepeatEndings.begin ();
    i != fRepeatEndings.end ();
    i++) {
    repeatDeepCopy->
      addRepeatEnding (
        (*i)->createRepeatEndingDeepCopy (
          repeatDeepCopy));
  } // for

  repeatDeepCopy->fRepeatEndingsInternalCounter =
    fRepeatEndingsInternalCounter;

  // uplinks
  
  return repeatDeepCopy;
}

S_msrRepeatEnding msrRepeatEnding::createRepeatEndingNewbornClone (
  S_msrRepeat containingRepeat)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceRepeats) {
    gLogIOstream <<
      "Creating a newborn clone of a " <<
      asString () <<
      endl;
  }
#endif
  
  // sanity check
  msrAssert(
    containingRepeat != nullptr,
    "containingRepeat is null");
    
  S_msrRepeatEnding
    newbornClone =
      msrRepeatEnding::create (
        fInputLineNumber,
        fRepeatEndingNumber,
        fRepeatEndingKind,
        containingRepeat->
          getRepeatCommonPart ()->
            getRepeatCommonPartSegment (), // JMI
        containingRepeat);
  
  // numbers
  newbornClone->fRepeatEndingNumber =
    fRepeatEndingNumber;
    
  newbornClone->fRepeatEndingInternalNumber =
    fRepeatEndingInternalNumber;
    
  // kind
  newbornClone->fRepeatEndingKind =
    fRepeatEndingKind;
    
  // segment

  // uplinks

  return newbornClone;
}

S_msrRepeatEnding msrRepeatEnding::createRepeatEndingDeepCopy (
  S_msrRepeat containingRepeat)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceRepeats) {
    gLogIOstream <<
      "Creating a newborn clone of a " <<
      asString () <<
      endl;
  }
#endif
  
  // sanity check
  msrAssert(
    containingRepeat != nullptr,
    "containingRepeat is null");
    
  S_msrRepeatEnding
    repeatEndingDeepCopy =
      msrRepeatEnding::create (
        fInputLineNumber,
        fRepeatEndingNumber,
        fRepeatEndingKind,
        containingRepeat->
          getRepeatCommonPart ()->
            getRepeatCommonPartSegment (), // JMI
        containingRepeat);
    
  // numbers
  repeatEndingDeepCopy->fRepeatEndingInternalNumber =
    fRepeatEndingInternalNumber;
    
  // kind
  repeatEndingDeepCopy->fRepeatEndingKind =
    fRepeatEndingKind;
    
  // segment
  repeatEndingDeepCopy->fRepeatEndingSegment =
    fRepeatEndingSegment->
      createSegmentDeepCopy (
        fRepeatEndingRepeatUplink->
          getRepeatVoiceUplink ());
    
  // uplinks
  repeatEndingDeepCopy->fRepeatEndingRepeatUplink =
    containingRepeat;

  return repeatEndingDeepCopy;
}

S_msrRepeatCommonPart msrRepeatCommonPart::createRepeatCommonPartNewbornClone (
  S_msrRepeat containingRepeat)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceRepeats) {
    gLogIOstream <<
      "Creating a newborn clone of a " <<
      asString () <<
      endl;
  }
#endif
  
  // sanity check
  msrAssert(
    containingRepeat != nullptr,
    "containingRepeat is null");
    
  S_msrRepeatCommonPart
    newbornClone =
      msrRepeatCommonPart::create (
        fInputLineNumber,
        containingRepeat->
          getRepeatCommonPart ()->
            getRepeatCommonPartSegment (), // JMI
        containingRepeat);
      
  // segment

  // uplinks

  return newbornClone;
}

S_msrRepeatCommonPart msrRepeatCommonPart::createRepeatCommonPartDeepCopy (
  S_msrRepeat containingRepeat)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceRepeats) {
    gLogIOstream <<
      "Creating a newborn clone of a " <<
      asString () <<
      endl;
  }
#endif
  
  // sanity check
  msrAssert(
    containingRepeat != nullptr,
    "containingRepeat is null");
    
  S_msrRepeatCommonPart
    repeatCommonPartDeepCopy =
      msrRepeatCommonPart::create (
        fInputLineNumber,
        containingRepeat->
          getRepeatCommonPart ()->
            getRepeatCommonPartSegment (), // JMI
        containingRepeat);
    
  // segment
  repeatCommonPartDeepCopy->fRepeatCommonPartSegment =
    fRepeatCommonPartSegment->
      createSegmentDeepCopy (
        fRepeatCommonPartRepeatUplink->
          getRepeatVoiceUplink ());
    
  // uplinks
  repeatCommonPartDeepCopy->fRepeatCommonPartRepeatUplink =
    containingRepeat;

  return repeatCommonPartDeepCopy;
}


 
*/

/* JMI
  bool doAddSlurToNote = true;
  
  if (fNoteSlurs.size ()) {
    // this notes has several slurs attached to it
    S_msrSlur previousSlur = fNoteSlurs.back ();

    switch (previousSlur->getSlurKind ()) {
      case msrSlur::kRegularSlurStart:
      case msrSlur::kPhrasingSlurStart:
        break;
      case msrSlur::kSlurContinue:
        break;
      case msrSlur::kRegularSlurStop:
      case msrSlur::kPhrasingSlurStop:
        switch (previousSlur->getSlurKind ()) {
          case msrSlur::kRegularSlurStart:
          case msrSlur::kPhrasingSlurStart:
            {
            / *
              Only the  first note of the chord should get the slur notation.
              Some applications print out the slur for all notes,
              i.e. a stop and a start in sequqnce:
              these should be ignored
              * /
              stringstream s;
              
              s <<
                "a 'slur start' is immediately followed by a 'slur stop'," <<
                endl <<
                "ignoring both of them at line " <<
                slur->getInputLineNumber ();
                
              msrMusicXMLWarning (
                gXml2lyOptions->fInputSourceName,
                slur->getInputLineNumber (),
                s.str ());
                
              // rmeove the 'slur start'
              fNoteSlurs.pop_back ();
        
              // don't register 'slur stop'
              doAddSlurToNote = false;
            }
            break;
          case msrSlur::kSlurContinue:
            break;
          case msrSlur::kRegularSlurStop:
            break;
          case msrSlur::kPhrasingSlurStop:
            break;
          case msrSlur::k_NoSlur:
            break;
        } // switch
        break;
      case msrSlur::k_NoSlur:
        break;
    } // switch
  }

  if (doAddSlurToNote) {
    fNoteSlurs.push_back (slur);
  }
  */

/*
   int
    segmentMeasuresListSize =
      fSegmentMeasuresList.size ();

  if (segmentMeasuresListSize == 0) {
*/

