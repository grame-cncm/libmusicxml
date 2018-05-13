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

#include "traceOptions.h"
#include "musicXMLOptions.h"
#include "msrOptions.h"

#include "xml2lyOptionsHandling.h"


using namespace std;


namespace MusicXML2 
{

//______________________________________________________________________________
msrElement::msrElement (
  int inputLineNumber)
{
  fInputLineNumber = inputLineNumber;  
}

msrElement::~msrElement()
{}

void msrElement::acceptIn (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrElement::acceptIn()" <<
      endl;
  }
      
  if (visitor<S_msrElement>*
    p =
      dynamic_cast<visitor<S_msrElement>*> (v)) {
        S_msrElement elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrElement::visitStart()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrElement::acceptOut (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrElement::acceptOut()" <<
      endl;
  }

  if (visitor<S_msrElement>*
    p =
      dynamic_cast<visitor<S_msrElement>*> (v)) {
        S_msrElement elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrElement::visitEnd()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

string msrElement::asString () const
{
  // this is overriden all in actual elements
  return "??? Element ???";
}

string msrElement::asShortString () const
{
  // this is overriden in actual elements only if needed
  return asString ();
}

ostream& operator<< (ostream& os, const S_msrElement& elt)
{
  elt->print (os);
  return os;
}

void msrElement::print (ostream& os)
{
  os << asString () << endl;
}

//______________________________________________________________________________
S_msrComment msrComment::create (
  int    inputLineNumber,
  string commentText)
{
  msrComment* o =
    new msrComment (
      inputLineNumber, commentText);
  assert(o!=0);
  return o;
}

msrComment::msrComment (
  int    inputLineNumber,
  string commentText)
    : msrElement (inputLineNumber)
{
  fCommentText = commentText;
}

msrComment::~msrComment ()
{}

void msrComment::acceptIn (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrComment::acceptIn()" <<
      endl;
  }
  
  if (visitor<S_msrComment>*
    p =
      dynamic_cast<visitor<S_msrComment>*> (v)) {
        S_msrComment elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrComment::visitStart()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrComment::acceptOut (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrComment::acceptOut()" <<
      endl;
  }

  if (visitor<S_msrComment>*
    p =
      dynamic_cast<visitor<S_msrComment>*> (v)) {
        S_msrComment elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrComment::visitEnd()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrComment::browseData (basevisitor* v)
{}

ostream& operator<< (ostream& os, const S_msrComment& elt)
{
  elt->print (os);
  return os;
}

string msrComment::asString () const
{
  return
    "Comment '" + fCommentText + "'";
}

void msrComment::print (ostream& os)
{
  os <<
    "Comment" <<
    endl;
    
  gIndenter++;
  
  os <<
    fCommentText <<
    endl;

  gIndenter--;
}

//______________________________________________________________________________
S_msrOctaveShift msrOctaveShift::create (
  int                inputLineNumber,
  msrOctaveShiftKind octaveShiftKind,
  int                octaveShiftSize)
{
  msrOctaveShift* o =
    new msrOctaveShift (
      inputLineNumber, octaveShiftKind, octaveShiftSize);
  assert(o!=0);
  return o;
}

msrOctaveShift::msrOctaveShift (
  int                inputLineNumber,
  msrOctaveShiftKind octaveShiftKind,
  int                octaveShiftSize)
    : msrElement (inputLineNumber)
{
  fOctaveShiftKind = octaveShiftKind;

  fOctaveShiftSize = octaveShiftSize;
}

msrOctaveShift::~msrOctaveShift ()
{}

void msrOctaveShift::acceptIn (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrOctaveShift::acceptIn()" <<
      endl;
  }
  
  if (visitor<S_msrOctaveShift>*
    p =
      dynamic_cast<visitor<S_msrOctaveShift>*> (v)) {
        S_msrOctaveShift elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrOctaveShift::visitStart()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrOctaveShift::acceptOut (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrOctaveShift::acceptOut()" <<
      endl;
  }

  if (visitor<S_msrOctaveShift>*
    p =
      dynamic_cast<visitor<S_msrOctaveShift>*> (v)) {
        S_msrOctaveShift elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrOctaveShift::visitEnd()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrOctaveShift::browseData (basevisitor* v)
{}

ostream& operator<< (ostream& os, const S_msrOctaveShift& elt)
{
  elt->print (os);
  return os;
}

string msrOctaveShift::octaveShiftKindAsString () const
{
  string result;
  
  switch (fOctaveShiftKind) {
    case k_NoOctaveShift:
      result = "none";
      break;
    case kOctaveShiftUp:
      result = "up";
      break;
    case kOctaveShiftDown:
      result = "down";
      break;
    case kOctaveShiftStop:
      result = "stop";
      break;
    case kOctaveShiftContinue:
      result = "continue";
      break;
  } // switch

  return result;
}

void msrOctaveShift::print (ostream& os)
{
  gIndenter++;
  
  os <<
    "OctaveShift" <<
    ", kind: " << octaveShiftKindAsString () <<
    ", size: " << fOctaveShiftSize <<
    endl;

  gIndenter--;
}

//______________________________________________________________________________
S_msrStringTuning msrStringTuning::create (
  int                  inputLineNumber,
  int                  stringTuningNumber,
  msrDiatonicPitchKind stringTuningDiatonicPitchKind,
  msrAlterationKind    stringTuningAlterationKind,
  int                  stringTuningOctave)
{
  msrStringTuning* o =
    new msrStringTuning (
      inputLineNumber,
      stringTuningNumber,
      stringTuningDiatonicPitchKind,
      stringTuningAlterationKind,
      stringTuningOctave);
  assert(o!=0);
  return o;
}

msrStringTuning::msrStringTuning (
  int                  inputLineNumber,
  int                  stringTuningNumber,
  msrDiatonicPitchKind stringTuningDiatonicPitchKind,
  msrAlterationKind    stringTuningAlterationKind,
  int                  stringTuningOctave)
    : msrElement (inputLineNumber)
{
  fStringTuningNumber = stringTuningNumber;

  fStringTuningDiatonicPitchKind = stringTuningDiatonicPitchKind;
  fStringTuningAlterationKind    = stringTuningAlterationKind;
  fStringTuningOctave            = stringTuningOctave;
}

msrStringTuning::~msrStringTuning ()
{}

void msrStringTuning::acceptIn (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrStringTuning::acceptIn()" <<
      endl;
  }
  
  if (visitor<S_msrStringTuning>*
    p =
      dynamic_cast<visitor<S_msrStringTuning>*> (v)) {
        S_msrStringTuning elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrStringTuning::visitStart()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrStringTuning::acceptOut (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrStringTuning::acceptOut()" <<
      endl;
  }

  if (visitor<S_msrStringTuning>*
    p =
      dynamic_cast<visitor<S_msrStringTuning>*> (v)) {
        S_msrStringTuning elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrStringTuning::visitEnd()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrStringTuning::browseData (basevisitor* v)
{}

ostream& operator<< (ostream& os, const S_msrStringTuning& elt)
{
  elt->print (os);
  return os;
}

void msrStringTuning::print (ostream& os)
{
  gIndenter++;

  const int fieldWidth = 30;
  
  os << left <<
    setw (fieldWidth) <<
    "stringTuningNumber" << " : " << fStringTuningNumber <<
    endl <<
    setw (fieldWidth) <<
    "stringTuningDiatonicPitchKind" << " : " <<
    msrDiatonicPitchKindAsString (
      fStringTuningDiatonicPitchKind) <<
    endl <<
    setw (fieldWidth) <<
    "stringTuningAlterationKind" << " : " <<
    msrAlterationKindAsString (
      fStringTuningAlterationKind) <<
    endl <<
    setw (fieldWidth) <<
    "stringTuningOctave" << " : " << fStringTuningOctave <<
    endl <<
    endl;

  gIndenter--;
}

//______________________________________________________________________________
S_msrScordatura msrScordatura::create (
  int inputLineNumber)
{
  msrScordatura* o =
    new msrScordatura (
      inputLineNumber);
  assert(o!=0);
  return o;
}

msrScordatura::msrScordatura (
  int inputLineNumber)
    : msrElement (inputLineNumber)
{}

msrScordatura::~msrScordatura ()
{}

void msrScordatura::addStringTuningToScordatura (
  S_msrStringTuning stringTuning)
{
  fScordaturaStringTuningsList.push_back (
    stringTuning);
}

void msrScordatura::acceptIn (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrScordatura::acceptIn()" <<
      endl;
  }
  
  if (visitor<S_msrScordatura>*
    p =
      dynamic_cast<visitor<S_msrScordatura>*> (v)) {
        S_msrScordatura elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrScordatura::visitStart()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrScordatura::acceptOut (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrScordatura::acceptOut()" <<
      endl;
  }

  if (visitor<S_msrScordatura>*
    p =
      dynamic_cast<visitor<S_msrScordatura>*> (v)) {
        S_msrScordatura elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrScordatura::visitEnd()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrScordatura::browseData (basevisitor* v)
{}

ostream& operator<< (ostream& os, const S_msrScordatura& elt)
{
  elt->print (os);
  return os;
}


void msrScordatura::print (ostream& os)
{
  gIndenter++;
  
  os <<
    "Scordatura";

  if (fScordaturaStringTuningsList.size ()) {
    os <<
      endl;

    gIndenter++;

    list<S_msrStringTuning>::const_iterator
      iBegin = fScordaturaStringTuningsList.begin (),
      iEnd   = fScordaturaStringTuningsList.end (),
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
      " : no string tunings" <<
      endl;
  }
}

//______________________________________________________________________________
void msrPolyphony::print (ostream& os)
{
/* JMI
  const int fieldWidth = 9;

  os << left <<
    setw (fieldWidth) <<
    "fBeatUnit" << " = " << fBeatUnit <<
    endl <<
    setw (fieldWidth) <<
    "fDots" << " = " << fDots <<
    endl;
      */
};

//______________________________________________________________________________
S_msrAccordionRegistration msrAccordionRegistration::create (
  int inputLineNumber,
  int highDotsNumber,
  int middleDotsNumber,
  int lowDotsNumber)
{
  msrAccordionRegistration* o =
    new msrAccordionRegistration (
      inputLineNumber,
      highDotsNumber, middleDotsNumber, lowDotsNumber);
  assert(o!=0);
  return o;
}

msrAccordionRegistration::msrAccordionRegistration (
  int inputLineNumber,
  int highDotsNumber,
  int middleDotsNumber,
  int lowDotsNumber)
    : msrElement (inputLineNumber)
{
  fHighDotsNumber   = highDotsNumber;
  fMiddleDotsNumber = middleDotsNumber;
  fLowDotsNumber    = lowDotsNumber;
}

msrAccordionRegistration::~msrAccordionRegistration()
{}

void msrAccordionRegistration::acceptIn (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrAccordionRegistration::acceptIn()" <<
      endl;
  }
      
  if (visitor<S_msrAccordionRegistration>*
    p =
      dynamic_cast<visitor<S_msrAccordionRegistration>*> (v)) {
        S_msrAccordionRegistration elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrAccordionRegistration::visitStart()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrAccordionRegistration::acceptOut (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrAccordionRegistration::acceptOut()" <<
      endl;
  }

  if (visitor<S_msrAccordionRegistration>*
    p =
      dynamic_cast<visitor<S_msrAccordionRegistration>*> (v)) {
        S_msrAccordionRegistration elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrAccordionRegistration::visitEnd()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrAccordionRegistration::browseData (basevisitor* v)
{}

ostream& operator<< (ostream& os, const S_msrAccordionRegistration& elt)
{
  elt->print (os);
  return os;
}

string msrAccordionRegistration::asString () const
{
  stringstream s;

  s <<
    "AccordionRegistration" <<
    ", highDotsNumber: " << fHighDotsNumber <<
    ", middleDotsNumber: " << fMiddleDotsNumber <<
    ", lowDotsNumber: " << fLowDotsNumber <<
    ", line " << fInputLineNumber;

  return s.str ();
}

void msrAccordionRegistration::print (ostream& os)
{
  gIndenter++;
  
  os <<
    asString () <<
    endl;

  gIndenter--;
}

//______________________________________________________________________________
S_msrStem msrStem::create (
  int           inputLineNumber,
  msrStemKind   stemKind)
{
  msrStem* o =
    new msrStem (
      inputLineNumber, stemKind);
  assert(o!=0);
  return o;
}

msrStem::msrStem (
  int           inputLineNumber,
  msrStemKind   stemKind)
    : msrElement (inputLineNumber)
{
  fStemKind   = stemKind; 
}

msrStem::~msrStem()
{}

void msrStem::acceptIn (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrStem::acceptIn()" <<
      endl;
  }
      
  if (visitor<S_msrStem>*
    p =
      dynamic_cast<visitor<S_msrStem>*> (v)) {
        S_msrStem elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrStem::visitStart()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrStem::acceptOut (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrStem::acceptOut()" <<
      endl;
  }

  if (visitor<S_msrStem>*
    p =
      dynamic_cast<visitor<S_msrStem>*> (v)) {
        S_msrStem elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrStem::visitEnd()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrStem::browseData (basevisitor* v)
{}

ostream& operator<< (ostream& os, const S_msrStem& elt)
{
  elt->print (os);
  return os;
}

string msrStem::stemKindAsString (
  msrStemKind stemKind)
{
  string result;
  
  switch (stemKind) {
    case k_NoStem:
      result = "none";
      break;
    case kStemUp:
      result = "up";
      break;
    case kStemDown:
      result = "down";
      break;
    case kStemNone:
      result = "none";
      break;
    case kStemDouble:
      result = "double";
      break;
  } // switch

  return result;
}

void msrStem::print (ostream& os)
{
  gIndenter++;
  
  os <<
    "Stem" " " <<
    stemKindAsString (fStemKind) <<
    ", line " << fInputLineNumber <<
    endl;

  gIndenter--;
}

//______________________________________________________________________________
S_msrBeam msrBeam::create (
  int         inputLineNumber,
  int         number,
  msrBeamKind beamKind)
{
  msrBeam* o =
    new msrBeam (
      inputLineNumber, number, beamKind);
  assert(o!=0);
  return o;
}

msrBeam::msrBeam (
  int         inputLineNumber,
  int         number,
  msrBeamKind beamKind)
    : msrElement (inputLineNumber)
{
  fBeamNumber = number;
  fBeamKind   = beamKind; 

  if (gTraceOptions->fTraceBeams) {
    gLogIOstream <<
      "Creating beam '" <<
      this->asString () <<
      "'" <<
      endl;
  }
}

msrBeam::~msrBeam ()
{}

void msrBeam::acceptIn (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrBeam::acceptIn()" <<
      endl;
  }
      
  if (visitor<S_msrBeam>*
    p =
      dynamic_cast<visitor<S_msrBeam>*> (v)) {
        S_msrBeam elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrBeam::visitStart()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrBeam::acceptOut (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrBeam::acceptOut()" <<
      endl;
  }

  if (visitor<S_msrBeam>*
    p =
      dynamic_cast<visitor<S_msrBeam>*> (v)) {
        S_msrBeam elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrBeam::visitEnd()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrBeam::browseData (basevisitor* v)
{}

ostream& operator<< (ostream& os, const S_msrBeam& elt)
{
  elt->print (os);
  return os;
}

string msrBeam::beamKindAsString (
  msrBeamKind beamKind)
{
  string result;
  
  switch (beamKind) {
    case kBeginBeam:
      result = "begin";
      break;
    case kContinueBeam:
      result = "continue";
      break;
    case kEndBeam:
      result = "end";
      break;
    case kForwardHookBeam:
      result = "forward";
      break;
    case kBackwardHookBeam:
      result = "backward";
      break;
    case k_NoBeam:
      result = "### none ###";
      break;
  } // switch

  return result;
}

string msrBeam::asString () const
{
  stringstream s;

  s <<
    "Beam" <<
    " " << beamKindAsString (fBeamKind) <<
    ", number " << fBeamNumber <<
    ", line " << fInputLineNumber;

  return s.str ();
}

void msrBeam::print (ostream& os)
{  
  os <<
    asString () <<
    endl;
}

//______________________________________________________________________________
S_msrArticulation msrArticulation::create (
  int                 inputLineNumber,
  msrArticulationKind articulationKind,
  msrPlacementKind    articulationPlacementKind)
{
  msrArticulation* o =
    new msrArticulation (
      inputLineNumber,
      articulationKind,
      articulationPlacementKind);
  assert (o!=0);
  return o;
}

msrArticulation::msrArticulation (
  int                 inputLineNumber,
  msrArticulationKind articulationKind,
  msrPlacementKind    articulationPlacementKind)
    : msrElement (inputLineNumber)
{
  fArticulationKind = articulationKind;

  fArticulationPlacementKind = articulationPlacementKind;
}

msrArticulation::~msrArticulation()
{}

string msrArticulation::articulationKindAsString (
  msrArticulationKind articulationKind)
{
  string result;
  
  switch (articulationKind) {
    case msrArticulation::kAccent:
      result = "accent";
      break;
    case msrArticulation::kBreathMark:
      result = "breathMark";
      break;
    case msrArticulation::kCaesura:
      result = "caesura";
      break;
    case msrArticulation::kSpiccato:
      result = "spiccato";
      break;
    case msrArticulation::kStaccato:
      result = "staccato";
      break;
    case msrArticulation::kStaccatissimo:
      result = "staccatissimo";
      break;
    case msrArticulation::kStress:
      result = "stress";
      break;
    case msrArticulation::kUnstress:
      result = "unstress";
      break;
    case msrArticulation::kDetachedLegato:
      result = "detached legato";
      break;
    case msrArticulation::kStrongAccent:
      result = "strong accent";
      break;
    case msrArticulation::kTenuto:
      result = "tenuto";
      break;
    case msrArticulation::kFermata:
      result = "fermata";
      break;
    case msrArticulation::kArpeggiato:
      result = "arpeggiato";
      break;
    case msrArticulation::kNonArpeggiato:
      result = "nonArpeggiato";
      break;
    case msrArticulation::kDoit:
      result = "doit";
      break;
    case msrArticulation::kFalloff:
      result = "falloff";
      break;
    case msrArticulation::kPlop:
      result = "plop";
      break;
    case msrArticulation::kScoop:
      result = "scoop";
      break;
  } // switch

  return result;
}

string msrArticulation::articulationKindAsString () const
{
  return
    articulationKindAsString (
      fArticulationKind);
}

string msrArticulation::articulationPlacementKindAsString () const
{
  return
    msrPlacementKindAsString (
      fArticulationPlacementKind);
}

void msrArticulation::acceptIn (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrArticulation::acceptIn()" <<
      endl;
  }
      
  if (visitor<S_msrArticulation>*
    p =
      dynamic_cast<visitor<S_msrArticulation>*> (v)) {
        S_msrArticulation elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrArticulation::visitStart()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrArticulation::acceptOut (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrArticulation::acceptOut()" <<
      endl;
  }

  if (visitor<S_msrArticulation>*
    p =
      dynamic_cast<visitor<S_msrArticulation>*> (v)) {
        S_msrArticulation elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrArticulation::visitEnd()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrArticulation::browseData (basevisitor* v)
{}

ostream& operator<< (ostream& os, const S_msrArticulation& elt)
{
  elt->print (os);
  return os;
}

void msrArticulation::print (ostream& os)
{
  os <<
    "Articulation" " " <<
    articulationKindAsString () <<
    ", " <<
    articulationPlacementKindAsString () <<
    ", line " << fInputLineNumber <<
    endl;
}

//______________________________________________________________________________
S_msrArpeggiato msrArpeggiato::create (
  int              inputLineNumber,
  msrPlacementKind arpeggiatoPlacementKind,
  msrDirectionKind arpeggiatoDirectionKind,
  int              arpeggiatoNumber)
{
  msrArpeggiato* o =
    new msrArpeggiato (
      inputLineNumber,
      arpeggiatoPlacementKind,
      arpeggiatoDirectionKind,
      arpeggiatoNumber);
  assert (o!=0);
  return o;
}

msrArpeggiato::msrArpeggiato (
  int              inputLineNumber,
  msrPlacementKind arpeggiatoPlacementKind,
  msrDirectionKind arpeggiatoDirectionKind,
  int              arpeggiatoNumber)
    : msrArticulation (
      inputLineNumber,
      kArpeggiato,
      arpeggiatoPlacementKind)
{  
  fArpeggiatoDirectionKind = arpeggiatoDirectionKind;
  
  fArpeggiatoNumber = arpeggiatoNumber;
}

msrArpeggiato::~msrArpeggiato()
{}

string msrArpeggiato::arpeggiatoDirectionKindAsString () const
{
  return
    msrDirectionKindAsString (fArpeggiatoDirectionKind);
}

void msrArpeggiato::acceptIn (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrArpeggiato::acceptIn()" <<
      endl;
  }
      
  if (visitor<S_msrArpeggiato>*
    p =
      dynamic_cast<visitor<S_msrArpeggiato>*> (v)) {
        S_msrArpeggiato elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrArpeggiato::visitStart()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrArpeggiato::acceptOut (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrArpeggiato::acceptOut()" <<
      endl;
  }

  if (visitor<S_msrArpeggiato>*
    p =
      dynamic_cast<visitor<S_msrArpeggiato>*> (v)) {
        S_msrArpeggiato elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrArpeggiato::visitEnd()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrArpeggiato::browseData (basevisitor* v)
{}

ostream& operator<< (ostream& os, const S_msrArpeggiato& elt)
{
  elt->print (os);
  return os;
}

void msrArpeggiato::print (ostream& os)
{
  os <<
    "Arpeggiato" " " <<
    articulationKindAsString () <<
    ", " <<
    articulationPlacementKindAsString () <<
    ", " <<
    arpeggiatoDirectionKindAsString () <<
    ", number: " <<
    fArpeggiatoNumber <<
    ", line " << fInputLineNumber <<
    endl;
}

//______________________________________________________________________________
S_msrNonArpeggiato msrNonArpeggiato::create (
  int                      inputLineNumber,
  msrPlacementKind         nonArpeggiatoPlacementKind,
  msrNonArpeggiatoTypeKind nonArpeggiatoTypeKind,
  int                      nonArpeggiatoNumber)
{
  msrNonArpeggiato* o =
    new msrNonArpeggiato (
      inputLineNumber,
      nonArpeggiatoPlacementKind,
      nonArpeggiatoTypeKind,
      nonArpeggiatoNumber);
  assert (o!=0);
  return o;
}

msrNonArpeggiato::msrNonArpeggiato (
  int                      inputLineNumber,
  msrPlacementKind         nonArpeggiatoPlacementKind,
  msrNonArpeggiatoTypeKind nonArpeggiatoTypeKind,
  int                      nonArpeggiatoNumber)
    : msrArticulation (
      inputLineNumber,
      kNonArpeggiato,
      nonArpeggiatoPlacementKind)
{  
  fNonArpeggiatoTypeKind = nonArpeggiatoTypeKind;
  
  fNonArpeggiatoNumber = nonArpeggiatoNumber;
}

msrNonArpeggiato::~msrNonArpeggiato()
{}

string msrNonArpeggiato::nonArpeggiatoTypeKindAsString (
  msrNonArpeggiatoTypeKind nonArpeggiatoTypeKind)
{
  string result;
  
  switch (nonArpeggiatoTypeKind) {
    case msrNonArpeggiato::kNonArpeggiatoTypeNone:
      result = "nonArpeggiatoTypeNone";
      break;
    case msrNonArpeggiato::kNonArpeggiatoTypeTop:
      result = "nonArpeggiatoTypeTop";
      break;
    case msrNonArpeggiato::kNonArpeggiatoTypeBottom:
      result = "nonArpeggiatoTypeBottom";
      break;
  } // switch

  return result;
}

string msrNonArpeggiato::nonArpeggiatoTypeKindAsString () const
{
  return
    nonArpeggiatoTypeKindAsString (fNonArpeggiatoTypeKind);
}

void msrNonArpeggiato::acceptIn (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrNonArpeggiato::acceptIn()" <<
      endl;
  }
      
  if (visitor<S_msrNonArpeggiato>*
    p =
      dynamic_cast<visitor<S_msrNonArpeggiato>*> (v)) {
        S_msrNonArpeggiato elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrNonArpeggiato::visitStart()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrNonArpeggiato::acceptOut (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrNonArpeggiato::acceptOut()" <<
      endl;
  }

  if (visitor<S_msrNonArpeggiato>*
    p =
      dynamic_cast<visitor<S_msrNonArpeggiato>*> (v)) {
        S_msrNonArpeggiato elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrNonArpeggiato::visitEnd()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrNonArpeggiato::browseData (basevisitor* v)
{}

ostream& operator<< (ostream& os, const S_msrNonArpeggiato& elt)
{
  elt->print (os);
  return os;
}

void msrNonArpeggiato::print (ostream& os)
{
  os <<
    "NonArpeggiato" " " <<
    articulationKindAsString () <<
    ", " <<
    articulationPlacementKindAsString () <<
    ", " <<
    nonArpeggiatoTypeKindAsString () <<
    ", number: " <<
    fNonArpeggiatoNumber <<
    ", line " << fInputLineNumber <<
    endl;
}

//______________________________________________________________________________
S_msrTechnical msrTechnical::create (
  int              inputLineNumber,
  msrTechnicalKind technicalKind,
  msrPlacementKind technicalPlacementKind)
{
  msrTechnical* o =
    new msrTechnical (
      inputLineNumber,
      technicalKind,
      technicalPlacementKind);
  assert (o!=0);
  return o;
}

msrTechnical::msrTechnical (
  int              inputLineNumber,
  msrTechnicalKind technicalKind,
  msrPlacementKind technicalPlacementKind)
    : msrElement (inputLineNumber)
{
  fTechnicalKind = technicalKind;

  fTechnicalPlacementKind = technicalPlacementKind;
}

msrTechnical::~msrTechnical()
{}

string msrTechnical::technicalKindAsString () const
{
  string result;
  
  switch (fTechnicalKind) {
    case msrTechnical::kArrow:
      result = "Arrow";
      break;
    case msrTechnical::kDoubleTongue:
      result = "DoubleTongue";
      break;
    case msrTechnical::kDownBow:
      result = "DownBow";
      break;
    case msrTechnical::kFingernails:
      result = "Fingernails";
      break;
    case msrTechnical::kHarmonic:
      result = "Harmonic";
      break;
    case msrTechnical::kHeel:
      result = "Heel";
      break;
    case msrTechnical::kHole:
      result = "Hole";
      break;
    case msrTechnical::kOpenString:
      result = "OpenString";
      break;
    case msrTechnical::kSnapPizzicato:
      result = "SnapPizzicato";
      break;
    case msrTechnical::kStopped:
      result = "Stopped";
      break;
    case msrTechnical::kTap:
      result = "Tap";
      break;
    case msrTechnical::kThumbPosition:
      result = "ThumbPosition";
      break;
    case msrTechnical::kToe:
      result = "Toe";
      break;
    case msrTechnical::kTripleTongue:
      result = "TripleTongue";
      break;
    case msrTechnical::kUpBow:
      result = "UpBow";
      break;
  } // switch

  return result;
}

string msrTechnical::technicalPlacementKindAsString () const
{
  return
    msrPlacementKindAsString (
      fTechnicalPlacementKind);
}

/*
string msrTechnical::technicalAccidentalMarkKindAsString () const
{
  string result;
  
  switch (fTechnicalAccidentalMarkKind) {
    case msrTechnical::kNatural:
      result = "natural";
      break;
    case msrTechnical::kSharp:
      result = "sharp";
      break;
    case msrTechnical::kFlat:
      result = "flat";
      break;
  } // switch

  return result;
}
*/

void msrTechnical::acceptIn (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrTechnical::acceptIn()" <<
      endl;
  }
      
  if (visitor<S_msrTechnical>*
    p =
      dynamic_cast<visitor<S_msrTechnical>*> (v)) {
        S_msrTechnical elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrTechnical::visitStart()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrTechnical::acceptOut (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrTechnical::acceptOut()" <<
      endl;
  }

  if (visitor<S_msrTechnical>*
    p =
      dynamic_cast<visitor<S_msrTechnical>*> (v)) {
        S_msrTechnical elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrTechnical::visitEnd()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrTechnical::browseData (basevisitor* v)
{}

ostream& operator<< (ostream& os, const S_msrTechnical& elt)
{
  elt->print (os);
  return os;
}

string msrTechnical::asString () const
{
  return
    msrPlacementKindAsString (
      fTechnicalPlacementKind);
}

void msrTechnical::print (ostream& os)
{
  os <<
    "Technical" " " <<
    asString () <<
    ", line " << fInputLineNumber <<
//    ", accidental mark" << " = " << technicalAccidentalMarkKindAsString () <<
    ", note uplink" << " = " <<
    fTechnicalNoteUplink->asShortString () <<
    endl;
}

//______________________________________________________________________________
S_msrTechnicalWithInteger msrTechnicalWithInteger::create (
  int                         inputLineNumber,
  msrTechnicalWithIntegerKind technicalWithIntegerKind,
  int                         technicalWithIntegerValue,
  msrPlacementKind            technicalWithIntegerPlacementKind)
{
  msrTechnicalWithInteger* o =
    new msrTechnicalWithInteger (
      inputLineNumber,
      technicalWithIntegerKind,
      technicalWithIntegerValue,
      technicalWithIntegerPlacementKind);
  assert (o!=0);
  return o;
}

msrTechnicalWithInteger::msrTechnicalWithInteger (
  int                         inputLineNumber,
  msrTechnicalWithIntegerKind technicalWithIntegerKind,
  int                         technicalWithIntegerValue,
  msrPlacementKind            technicalWithIntegerPlacementKind)
    : msrElement (inputLineNumber)
{
  fTechnicalWithIntegerKind = technicalWithIntegerKind;

  fTechnicalWithIntegerValue = technicalWithIntegerValue;

  fTechnicalWithIntegerPlacementKind = technicalWithIntegerPlacementKind;
}

msrTechnicalWithInteger::~msrTechnicalWithInteger()
{}

string msrTechnicalWithInteger::technicalWithIntegerKindAsString () const
{
  string result;
  
  switch (fTechnicalWithIntegerKind) {
    case msrTechnicalWithInteger::kBend:
      result = "bend";
      break;
    case msrTechnicalWithInteger::kFingering:
      result = "fingering";
      break;
    case msrTechnicalWithInteger::kFret:
      result = "fret";
      break;
    case msrTechnicalWithInteger::kString:
      result = "string";
      break;
  } // switch

  return result;
}

string msrTechnicalWithInteger::technicalWithIntegerPlacementKindAsString () const
{
  return
    msrPlacementKindAsString (
      fTechnicalWithIntegerPlacementKind);
}

void msrTechnicalWithInteger::acceptIn (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrTechnicalWithInteger::acceptIn()" <<
      endl;
  }
      
  if (visitor<S_msrTechnicalWithInteger>*
    p =
      dynamic_cast<visitor<S_msrTechnicalWithInteger>*> (v)) {
        S_msrTechnicalWithInteger elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrTechnicalWithInteger::visitStart()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrTechnicalWithInteger::acceptOut (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrTechnicalWithInteger::acceptOut()" <<
      endl;
  }

  if (visitor<S_msrTechnicalWithInteger>*
    p =
      dynamic_cast<visitor<S_msrTechnicalWithInteger>*> (v)) {
        S_msrTechnicalWithInteger elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrTechnicalWithInteger::visitEnd()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrTechnicalWithInteger::browseData (basevisitor* v)
{}

ostream& operator<< (ostream& os, const S_msrTechnicalWithInteger& elt)
{
  elt->print (os);
  return os;
}

string msrTechnicalWithInteger::asString () const
{
  stringstream s;

  s <<
    technicalWithIntegerKindAsString () <<
    ", value '" <<
    fTechnicalWithIntegerValue <<
    "', placement " <<
    technicalWithIntegerPlacementKindAsString ();

  return s.str ();
}

void msrTechnicalWithInteger::print (ostream& os)
{
  os <<
    "TechnicalWithInteger" <<
    ", " << technicalWithIntegerKindAsString () <<
    ", line " << fInputLineNumber <<
    endl;

  gIndenter++;
  
  const int fieldWidth = 14;
  
  os << left <<
    setw (fieldWidth) <<
    "value" << " : " <<
    fTechnicalWithIntegerValue <<
    endl <<
    
    setw (fieldWidth) <<
    "placement" << " : " <<
    technicalWithIntegerPlacementKindAsString () <<
    endl <<

    setw (fieldWidth) <<
    "note uplink" << " = " <<
    fTechnicalWithIntegerNoteUplink->asShortString () <<
    endl;

  gIndenter--;
}

//______________________________________________________________________________
S_msrTechnicalWithString msrTechnicalWithString::create (
  int                        inputLineNumber,
  msrTechnicalWithStringKind technicalWithStringKind,
  msrTechnicalTypeKind       technicalWithStringTypeKind,
  string                     technicalWithStringValue,
  msrPlacementKind           technicalWithStringPlacementKind)
{
  msrTechnicalWithString* o =
    new msrTechnicalWithString (
      inputLineNumber,
      technicalWithStringKind,
      technicalWithStringTypeKind,
      technicalWithStringValue,
      technicalWithStringPlacementKind);
  assert (o!=0);
  return o;
}

msrTechnicalWithString::msrTechnicalWithString (
  int                        inputLineNumber,
  msrTechnicalWithStringKind technicalWithStringKind,
  msrTechnicalTypeKind       technicalWithStringTypeKind,
  string                     technicalWithStringValue,
  msrPlacementKind           technicalWithStringPlacementKind)
    : msrElement (inputLineNumber)
{
  fTechnicalWithStringKind = technicalWithStringKind;
  
  fTechnicalWithStringTypeKind = technicalWithStringTypeKind;

  fTechnicalWithStringValue = technicalWithStringValue;

  fTechnicalWithStringPlacementKind = technicalWithStringPlacementKind;
}

msrTechnicalWithString::~msrTechnicalWithString()
{}

string msrTechnicalWithString::technicalWithStringKindAsString () const
{
  string result;
  
  switch (fTechnicalWithStringKind) {
    case msrTechnicalWithString::kHammerOn:
      result = "HammerOn";
      break;
    case msrTechnicalWithString::kHandbell:
      result = "Handbell";
      break;
    case msrTechnicalWithString::kOtherTechnical:
      result = "OtherTechnical";
      break;
    case msrTechnicalWithString::kPluck:
      result = "Pluck";
      break;
    case msrTechnicalWithString::kPullOff:
      result = "PullOff";
      break;
  } // switch

  result +=
    " \"" + fTechnicalWithStringValue + "\"";

  return result;
}

string msrTechnicalWithString::technicalWithStringTypeKindAsString () const
{
  return
    msrTechnicalTypeKindAsString (
      fTechnicalWithStringTypeKind);
}

string msrTechnicalWithString::technicalWithStringPlacementKindAsString () const
{
  return
    msrPlacementKindAsString (
      fTechnicalWithStringPlacementKind);
}

void msrTechnicalWithString::acceptIn (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrTechnicalWithString::acceptIn()" <<
      endl;
  }
      
  if (visitor<S_msrTechnicalWithString>*
    p =
      dynamic_cast<visitor<S_msrTechnicalWithString>*> (v)) {
        S_msrTechnicalWithString elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrTechnicalWithString::visitStart()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrTechnicalWithString::acceptOut (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrTechnicalWithString::acceptOut()" <<
      endl;
  }

  if (visitor<S_msrTechnicalWithString>*
    p =
      dynamic_cast<visitor<S_msrTechnicalWithString>*> (v)) {
        S_msrTechnicalWithString elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrTechnicalWithString::visitEnd()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrTechnicalWithString::browseData (basevisitor* v)
{}

ostream& operator<< (ostream& os, const S_msrTechnicalWithString& elt)
{
  elt->print (os);
  return os;
}

string msrTechnicalWithString::asString () const
{
  stringstream s;

  s <<
    technicalWithStringKindAsString () <<
    ", " << technicalWithStringTypeKindAsString () <<
    ", value \"" <<
    fTechnicalWithStringValue <<
    "\", placement " <<
    technicalWithStringPlacementKindAsString ();

  return s.str ();
}

void msrTechnicalWithString::print (ostream& os)
{
  os <<
    "TechnicalWithString" <<
    ", " << technicalWithStringKindAsString () <<
    ", " << technicalWithStringTypeKindAsString () <<
    ", line " << fInputLineNumber <<
    endl;

  gIndenter++;
  
  const int fieldWidth = 14;
  
  os << left <<
    setw (fieldWidth) <<
    "value" << " : \"" <<
    fTechnicalWithStringValue <<
    "\"" <<
    endl <<
    
    setw (fieldWidth) <<
    "placement" << " : " <<
    technicalWithStringPlacementKindAsString () <<
    endl <<

    setw (fieldWidth) <<
    "note uplink" << " = " <<
    fTechnicalWithStringNoteUplink->asShortString () <<
    endl;

  gIndenter--;
}

//______________________________________________________________________________
S_msrFermata msrFermata::create (
  int                inputLineNumber,
  msrFermataKind     fermataKind,
  msrFermataTypeKind fermataTypeKind)
{
  msrFermata* o =
    new msrFermata (
      inputLineNumber,
      fermataKind,
      fermataTypeKind);
  assert (o!=0);
  return o;
}

msrFermata::msrFermata (
    int                inputLineNumber,
    msrFermataKind     fermataKind,
    msrFermataTypeKind fermataTypeKind)
    : msrArticulation (
      inputLineNumber,
      msrArticulation::kFermata,
      kPlacementNone) // temporary, JMI TEMP
{
  fFermataKind = fermataKind;
  fFermataTypeKind = fermataTypeKind;
}

msrFermata::~msrFermata()
{}

void msrFermata::acceptIn (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrFermata::acceptIn()" <<
      endl;
  }
      
  if (visitor<S_msrFermata>*
    p =
      dynamic_cast<visitor<S_msrFermata>*> (v)) {
        S_msrFermata elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrFermata::visitStart()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrFermata::acceptOut (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrFermata::acceptOut()" <<
      endl;
  }

  if (visitor<S_msrFermata>*
    p =
      dynamic_cast<visitor<S_msrFermata>*> (v)) {
        S_msrFermata elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrFermata::visitEnd()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrFermata::browseData (basevisitor* v)
{}

string msrFermata::fermataKindAsString (
  msrFermataKind fermataKind)
{
  string result;
  
  switch (fermataKind) {
    case msrFermata::kNormalFermataKind:
      result = "fermata kind: normal";
      break;
    case msrFermata::kAngledFermataKind:
      result = "fermata kind: angled";
      break;
    case msrFermata::kSquareFermataKind:
      result = "fermata kind: square";
      break;
  } // switch

  return result;
}
      
string msrFermata::fermataTypeKindAsString (
  msrFermataTypeKind fermataTypeKind)
{
  string result;
  
  switch (fermataTypeKind) {
    case msrFermata::kFermataTypeNone:
      result = "fermataTypeNone";
      break;
    case msrFermata::kFermataTypeUpright:
      result = "fermataTypeUpright";
      break;
    case msrFermata::kFermataTypeInverted:
      result = "fermataTypeInverted";
      break;
  } // switch

  return result;
}
      
ostream& operator<< (ostream& os, const S_msrFermata& elt)
{
  elt->print (os);
  return os;
}

string msrFermata::asString () const
{
  stringstream s;

  s <<
    "Fermata" <<
    ", " <<
    fermataKindAsString (fFermataKind) <<
    ", " <<
    fermataTypeKindAsString (fFermataTypeKind) <<
    ", line " << fInputLineNumber;

  return s.str ();
}

void msrFermata::print (ostream& os)
{
  os <<
    asString () <<
    endl;
}

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

msrOrnament::~msrOrnament()
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
      result = "none";
      break;
  } // switch

  return result;
}

void msrOrnament::acceptIn (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrOrnament::acceptIn()" <<
      endl;
  }
      
  if (visitor<S_msrOrnament>*
    p =
      dynamic_cast<visitor<S_msrOrnament>*> (v)) {
        S_msrOrnament elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrOrnament::visitStart()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrOrnament::acceptOut (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrOrnament::acceptOut()" <<
      endl;
  }

  if (visitor<S_msrOrnament>*
    p =
      dynamic_cast<visitor<S_msrOrnament>*> (v)) {
        S_msrOrnament elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrOrnament::visitEnd()" <<
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
    "note uplink" << " : " <<
    fOrnamentNoteUplink->asShortString () <<
    endl;

  gIndenter--;
}

//______________________________________________________________________________
S_msrSingleTremolo msrSingleTremolo::create (
  int              inputLineNumber,
  int              singleTremoloMarksNumber,
  msrPlacementKind singleTremoloPlacementKind)
{
  msrSingleTremolo* o =
    new msrSingleTremolo (
      inputLineNumber,
      singleTremoloMarksNumber, singleTremoloPlacementKind);
  assert (o!=0);
  return o;
}

S_msrSingleTremolo msrSingleTremolo::createSingleTremoloDeepCopy (
  S_msrNote noteUplink)
{
  if (gTraceOptions->fTraceNotes) {
    gLogIOstream <<
      "Creating a deep copy of single tremolo " <<
      asString () <<
      " in note '" <<
      noteUplink->
        asShortString () <<
      "'" <<
      endl;
  }

  // sanity check
  msrAssert(
    noteUplink != nullptr,
    "noteUplink is null");

  S_msrSingleTremolo
    singleTremoloDeepCopy =
      msrSingleTremolo::create (
        fInputLineNumber,
        fSingleTremoloMarksNumber,
        fSingleTremoloPlacementKind);

  singleTremoloDeepCopy->fSingleTremoloNoteUplink =
    noteUplink;
    
  return singleTremoloDeepCopy;
}

msrSingleTremolo::msrSingleTremolo (
  int              inputLineNumber,
  int              singleTremoloMarksNumber,
  msrPlacementKind singleTremoloPlacementKind)
    : msrElement (inputLineNumber)
{
  fSingleTremoloMarksNumber   = singleTremoloMarksNumber;
  fSingleTremoloPlacementKind = singleTremoloPlacementKind;
}

msrSingleTremolo::~msrSingleTremolo()
{}

string msrSingleTremolo::singleTremoloPlacementKindAsString () const
{
  return
    msrPlacementKindAsString (
      fSingleTremoloPlacementKind);
}

void msrSingleTremolo::acceptIn (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrSingleTremolo::acceptIn()" <<
      endl;
  }
      
  if (visitor<S_msrSingleTremolo>*
    p =
      dynamic_cast<visitor<S_msrSingleTremolo>*> (v)) {
        S_msrSingleTremolo elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrSingleTremolo::visitStart()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrSingleTremolo::acceptOut (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrSingleTremolo::acceptOut()" <<
      endl;
  }

  if (visitor<S_msrSingleTremolo>*
    p =
      dynamic_cast<visitor<S_msrSingleTremolo>*> (v)) {
        S_msrSingleTremolo elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrSingleTremolo::visitEnd()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrSingleTremolo::browseData (basevisitor* v)
{}

ostream& operator<< (ostream& os, const S_msrSingleTremolo& elt)
{
  elt->print (os);
  return os;
}

string msrSingleTremolo::asString () const
{
  stringstream s;
  
  s <<
    "SingleTremolo" <<
    ", line " << fInputLineNumber <<
    ", " << fSingleTremoloMarksNumber << " marks" <<
    ", placement" << " = " << singleTremoloPlacementKindAsString ();

  if (fSingleTremoloNoteUplink) // it may not yet be set
    s <<
      ", note uplink" << " = " <<
      fSingleTremoloNoteUplink->
        asShortStringWithRawWholeNotes ();

  return s.str ();
}

void msrSingleTremolo::print (ostream& os)
{
  os <<
    asString () <<
    endl;
}

//______________________________________________________________________________
S_msrDoubleTremolo msrDoubleTremolo::create (
  int                  inputLineNumber,
  msrDoubleTremoloKind doubleTremoloKind,
  msrTremoloTypeKind   doubleTremoloTypeKind,
  int                  doubleTremoloMarksNumber,
  msrPlacementKind     doubleTremoloPlacementKind,
  S_msrVoice           voiceUplink)
{
  msrDoubleTremolo* o =
    new msrDoubleTremolo (
      inputLineNumber,
      doubleTremoloKind,
      doubleTremoloTypeKind,
      doubleTremoloMarksNumber,
      doubleTremoloPlacementKind,
      voiceUplink);
  assert (o!=0);
  return o;
}

msrDoubleTremolo::msrDoubleTremolo (
  int                  inputLineNumber,
  msrDoubleTremoloKind doubleTremoloKind,
  msrTremoloTypeKind   doubleTremoloTypeKind,
  int                  doubleTremoloMarksNumber,
  msrPlacementKind     doubleTremoloPlacementKind,
  S_msrVoice           voiceUplink)
    : msrElement (inputLineNumber)
{
  // sanity check
  msrAssert (
    voiceUplink != nullptr,
    "voiceUplink is null");
    
  // set the double tremolo voice uplink
  fDoubleTremoloVoiceUplink = voiceUplink;
  
  fDoubleTremoloKind          = doubleTremoloKind;
  fDoubleTremoloTypeKind      = doubleTremoloTypeKind;
  fDoubleTremoloMarksNumber   = doubleTremoloMarksNumber;
  fDoubleTremoloPlacementKind = doubleTremoloPlacementKind;
  
  fDoubleTremoloSoundingWholeNotes =
    rational (-1, 1); // will be set later

  fDoubleTremoloNumberOfRepeats = -1; // will be set later
}

S_msrDoubleTremolo msrDoubleTremolo::createDoubleTremoloNewbornClone (
  S_msrVoice containingVoice)
{
  if (gTraceOptions->fTraceTremolos) {
    gLogIOstream <<
      "Creating a newborn clone of a double tremolo" <<
      endl;
  }
  
  // sanity check
  msrAssert(
    containingVoice != nullptr,
    "containingVoice is null");
    
  S_msrDoubleTremolo
    newbornClone =
      msrDoubleTremolo::create (
        fInputLineNumber,
        fDoubleTremoloKind,
        fDoubleTremoloTypeKind,
        fDoubleTremoloMarksNumber,
        fDoubleTremoloPlacementKind,
        containingVoice);

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

msrDoubleTremolo::~msrDoubleTremolo()
{}

void msrDoubleTremolo::setDoubleTremoloNoteFirstElement (S_msrNote note)
{
  if (gTraceOptions->fTraceTremolos) {
    gLogIOstream <<
      "Setting note " << note->asShortString () <<
      " as first element of double tremolo " <<
      asShortString () <<
      endl;
  }

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
      int (
        pow (
          2,
          fDoubleTremoloMarksNumber + 2)));

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
}

void msrDoubleTremolo::setDoubleTremoloNoteSecondElement (
  S_msrNote note)
{
  if (gTraceOptions->fTraceTremolos) {
    gLogIOstream <<
      "Setting note " << note->asShortString () <<
      " as second element of double tremolo " <<
      asShortString () <<
      endl;
  }

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
  if (gTraceOptions->fTraceTremolos || gTraceOptions->fTraceChords) {
    gLogIOstream <<
      "Setting chord " << chord->asString () <<
      " as first element of double tremolo " <<
      asShortString () <<
      endl;
  }

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
  if (gTraceOptions->fTraceTremolos || gTraceOptions->fTraceChords) {
    gLogIOstream <<
      "Setting chord " << chord->asString () <<
      " as second element of double tremolo " <<
      asShortString () <<
      endl;
  }

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
      "% ==> msrDoubleTremolo::acceptIn()" <<
      endl;
  }
      
  if (visitor<S_msrDoubleTremolo>*
    p =
      dynamic_cast<visitor<S_msrDoubleTremolo>*> (v)) {
        S_msrDoubleTremolo elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrDoubleTremolo::visitStart()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrDoubleTremolo::acceptOut (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrDoubleTremolo::acceptOut()" <<
      endl;
  }

  if (visitor<S_msrDoubleTremolo>*
    p =
      dynamic_cast<visitor<S_msrDoubleTremolo>*> (v)) {
        S_msrDoubleTremolo elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrDoubleTremolo::visitEnd()" <<
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
  msrSpannerKind     spannerKind,
  msrSpannerTypeKind spannerTypeKind,
  msrPlacementKind   spannerPlacementKind,
  S_msrNote          spannerNoteUplink)
{
  msrSpanner* o =
    new msrSpanner (
      inputLineNumber,
      spannerKind,
      spannerTypeKind,
      spannerPlacementKind,
      spannerNoteUplink);
  assert (o!=0);
  return o;
}

msrSpanner::msrSpanner (
  int                inputLineNumber,
  msrSpannerKind     spannerKind,
  msrSpannerTypeKind spannerTypeKind,
  msrPlacementKind   spannerPlacementKind,
  S_msrNote          spannerNoteUplink)
    : msrElement (inputLineNumber)
{
  fSpannerNoteUplink = spannerNoteUplink;
  
  fSpannerKind = spannerKind;
  
  fSpannerTypeKind = spannerTypeKind;

  fSpannerPlacementKind = spannerPlacementKind;
}

msrSpanner::~msrSpanner ()
{}

string msrSpanner::spannerKindAsString (
  msrSpannerKind spannerKind)
{
  string result;
  
  switch (spannerKind) {
    case msrSpanner::kSpannerTrill:
      result = "spannerTrill";
      break;
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
      "% ==> msrSpanner::acceptIn()" <<
      endl;
  }
      
  if (visitor<S_msrSpanner>*
    p =
      dynamic_cast<visitor<S_msrSpanner>*> (v)) {
        S_msrSpanner elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrSpanner::visitStart()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrSpanner::acceptOut (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrSpanner::acceptOut()" <<
      endl;
  }

  if (visitor<S_msrSpanner>*
    p =
      dynamic_cast<visitor<S_msrSpanner>*> (v)) {
        S_msrSpanner elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrSpanner::visitEnd()" <<
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
S_msrRehearsal msrRehearsal::create (
  int              inputLineNumber,
  msrRehearsalKind rehearsalKind,
  string           rehearsalText,
  msrPlacementKind rehearsalPlacementKind)
{
  msrRehearsal* o =
    new msrRehearsal (
      inputLineNumber,
      rehearsalKind,
      rehearsalText,
      rehearsalPlacementKind);
  assert (o!=0);
  return o;
}

msrRehearsal::msrRehearsal (
  int              inputLineNumber,
  msrRehearsalKind rehearsalKind,
  string           rehearsalText,
  msrPlacementKind rehearsalPlacementKind)
    : msrElement (inputLineNumber)
{
  fRehearsalKind = rehearsalKind;

  fRehearsalText = rehearsalText;

  fRehearsalPlacementKind = rehearsalPlacementKind;
}

msrRehearsal::~msrRehearsal()
{}

void msrRehearsal::acceptIn (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrRehearsal::acceptIn()" <<
      endl;
  }
      
  if (visitor<S_msrRehearsal>*
    p =
      dynamic_cast<visitor<S_msrRehearsal>*> (v)) {
        S_msrRehearsal elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrRehearsal::visitStart()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrRehearsal::acceptOut (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrRehearsal::acceptOut()" <<
      endl;
  }

  if (visitor<S_msrRehearsal>*
    p =
      dynamic_cast<visitor<S_msrRehearsal>*> (v)) {
        S_msrRehearsal elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrRehearsal::visitEnd()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrRehearsal::browseData (basevisitor* v)
{}

ostream& operator<< (ostream& os, const S_msrRehearsal& elt)
{
  elt->print (os);
  return os;
}

string msrRehearsal::rehearsalKindAsString (
  msrRehearsalKind rehearsalKind)
{
  string result;
  
  switch (rehearsalKind) {
    case msrRehearsal::kNone:
      result = "none";
      break;
    case msrRehearsal::kRectangle:
      result = "rectangle";
      break;
    case msrRehearsal::kOval:
      result = "oval";
      break;
    case msrRehearsal::kCircle:
      result = "circle";
      break;
    case msrRehearsal::kBracket:
      result = "bracket";
      break;
    case msrRehearsal::kTriangle:
      result = "triangle";
      break;
    case msrRehearsal::kDiamond:
      result = "diamond";
      break;
  } // switch

  return result;
}

void msrRehearsal::print (ostream& os)
{
  os <<
    "Rehearsal" << " " << fRehearsalText <<
    " kind: " <<
    rehearsalKindAsString (fRehearsalKind) <<
    " rehearsalPlacementKind: " <<
    msrPlacementKindAsString (fRehearsalPlacementKind) <<
    endl;
}

//______________________________________________________________________________
S_msrDynamics msrDynamics::create (
  int              inputLineNumber,
  msrDynamicsKind  dynamicsKind,
  msrPlacementKind dynamicsPlacementKind)
{
  msrDynamics* o =
    new msrDynamics (
      inputLineNumber,
      dynamicsKind,
      dynamicsPlacementKind);
    assert(o!=0);
  return o;
}

msrDynamics::msrDynamics (
  int              inputLineNumber,
  msrDynamicsKind  dynamicsKind,
  msrPlacementKind dynamicsPlacementKind)
    : msrElement (inputLineNumber)
{
  fDynamicsKind = dynamicsKind;

  fDynamicsPlacementKind = dynamicsPlacementKind;
}

msrDynamics::~msrDynamics()
{}

string msrDynamics::dynamicsKindAsString (
  msrDynamicsKind dynamicsKind)
{
  string result;
  
  switch (dynamicsKind) {
    case msrDynamics::kF:
      result = "f";
      break;
    case msrDynamics::kFF:
      result = "ff";
      break;
    case msrDynamics::kFFF:
      result = "fff";
      break;
    case msrDynamics::kFFFF:
      result = "ffff";
      break;
    case msrDynamics::kFFFFF:
      result = "fffff";
      break;
    case msrDynamics::kFFFFFF:
      result = "ffffff";
      break;

    case msrDynamics::kP:
      result = "p";
      break;
    case msrDynamics::kPP:
      result = "pp";
      break;
    case msrDynamics::kPPP:
      result = "ppp";
      break;
    case msrDynamics::kPPPP:
      result = "pppp";
      break;
    case msrDynamics::kPPPPP:
      result = "ppppp";
      break;
    case msrDynamics::kPPPPPP:
      result = "pppppp";
      break;

    case msrDynamics::kMF:
      result = "mf";
      break;
    case msrDynamics::kMP:
      result = "mp";
      break;
    case msrDynamics::kFP:
      result = "fp";
      break;
    case msrDynamics::kFZ:
      result = "fz";
      break;
    case msrDynamics::kRF:
      result = "rf";
      break;
    case msrDynamics::kSF:
      result = "sf";
      break;

    case msrDynamics::kRFZ:
      result = "rfz";
      break;
    case msrDynamics::kSFZ:
      result = "sfz";
      break;
    case msrDynamics::kSFP:
      result = "sfp";
      break;
    case msrDynamics::kSFPP:
      result = "sfpp";
      break;
    case msrDynamics::kSFFZ:
      result = "sffz";
      break;
    case msrDynamics::k_NoDynamics:
      result = "k_NoDynamics???";
  } // switch
  
  return result;
}

string msrDynamics::dynamicsKindAsString () const
{
  return
    dynamicsKindAsString (fDynamicsKind);
}

string msrDynamics::dynamicsPlacementKindAsString () const
{
  return
    msrPlacementKindAsString (
      fDynamicsPlacementKind);
}

void msrDynamics::acceptIn (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrDynamics::acceptIn()" <<
      endl;
  }
      
  if (visitor<S_msrDynamics>*
    p =
      dynamic_cast<visitor<S_msrDynamics>*> (v)) {
        S_msrDynamics elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrDynamics::visitStart()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrDynamics::acceptOut (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrDynamics::acceptOut()" <<
      endl;
  }

  if (visitor<S_msrDynamics>*
    p =
      dynamic_cast<visitor<S_msrDynamics>*> (v)) {
        S_msrDynamics elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrDynamics::visitEnd()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}


void msrDynamics::browseData (basevisitor* v)
{}

ostream& operator<< (ostream& os, const S_msrDynamics& elt)
{
  elt->print (os);
  return os;
}

void msrDynamics::print (ostream& os)
{
  os <<
    "Dynamics" " " <<
    dynamicsKindAsString () <<
    ", " <<
    dynamicsPlacementKindAsString () <<
    ", line " << fInputLineNumber <<
    endl;
}

//______________________________________________________________________________
S_msrOtherDynamics msrOtherDynamics::create (
  int    inputLineNumber,
  string otherDynamicsString)
{
  msrOtherDynamics* o =
    new msrOtherDynamics (
      inputLineNumber, otherDynamicsString);
    assert(o!=0);
  return o;
}

msrOtherDynamics::msrOtherDynamics (
  int    inputLineNumber,
  string otherDynamicsString)
    : msrElement (inputLineNumber)
{
  fOtherDynamicsString = otherDynamicsString; 
}

msrOtherDynamics::~msrOtherDynamics()
{}

void msrOtherDynamics::acceptIn (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrOtherDynamics::acceptIn()" <<
      endl;
  }
      
  if (visitor<S_msrOtherDynamics>*
    p =
      dynamic_cast<visitor<S_msrOtherDynamics>*> (v)) {
        S_msrOtherDynamics elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrOtherDynamics::visitStart()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrOtherDynamics::acceptOut (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrOtherDynamics::acceptOut()" <<
      endl;
  }

  if (visitor<S_msrOtherDynamics>*
    p =
      dynamic_cast<visitor<S_msrOtherDynamics>*> (v)) {
        S_msrOtherDynamics elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrOtherDynamics::visitEnd()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}


void msrOtherDynamics::browseData (basevisitor* v)
{}

ostream& operator<< (ostream& os, const S_msrOtherDynamics& elt)
{
  elt->print (os);
  return os;
}

string msrOtherDynamics::asString () const
{
  stringstream s;
  
  s <<
    "OtherDynamics '" << fOtherDynamicsString <<
    "', line " << fInputLineNumber;

  return s.str ();
}

void msrOtherDynamics::print (ostream& os)
{
  os <<
    asString () <<
    endl;
}

//______________________________________________________________________________
S_msrWedge msrWedge::create (
  int                inputLineNumber,
  msrWedgeKind       wedgeKind,
  msrWedgeNienteKind wedgeNienteKind,
  msrLineTypeKind    wedgeLineTypeKind,
  msrPlacementKind   wedgePlacementKind)
{
  msrWedge* o =
    new msrWedge (
      inputLineNumber,
      wedgeKind,
      wedgeNienteKind,
      wedgeLineTypeKind,
      wedgePlacementKind);
  assert(o!=0);
  return o;
}

msrWedge::msrWedge (
  int                inputLineNumber,
  msrWedgeKind       wedgeKind,
  msrWedgeNienteKind wedgeNienteKind,
  msrLineTypeKind    wedgeLineTypeKind,
  msrPlacementKind   wedgePlacementKind)
    : msrElement (inputLineNumber)
{
  fWedgeKind          = wedgeKind; 
  fWedgeNienteKind    = wedgeNienteKind; 
  fWedgeLineTypeKind  = wedgeLineTypeKind; 
  fWedgePlacementKind = wedgePlacementKind; 
}

msrWedge::~msrWedge()
{}

string msrWedge::wedgeKindAsString ()
{
  string result;
  
  switch (fWedgeKind) {
    case msrWedge::k_NoWedgeKind:
      result = "none";
      break;
    case msrWedge::kCrescendoWedge:
      result = "crescendo";
      break;
    case msrWedge::kDecrescendoWedge:
      result = "decrescendo";
      break;
    case msrWedge::kStopWedge:
      result = "stop";
      break;
  } // switch
    
  return result;
}
    
string msrWedge::wedgeNienteKindAsString (
  msrWedgeNienteKind wedgeNienteKind)
{
  string result;
  
  switch (wedgeNienteKind) {
    case msrWedge::kWedgeNienteYes:
      result = "kWedgeNienteYes";
      break;
    case msrWedge::kWedgeNienteNo:
      result = "kWedgeNienteNo";
      break;
  } // switch
    
  return result;
}

void msrWedge::acceptIn (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrWedge::acceptIn()" <<
      endl;
  }
      
  if (visitor<S_msrWedge>*
    p =
      dynamic_cast<visitor<S_msrWedge>*> (v)) {
        S_msrWedge elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrWedge::visitStart()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrWedge::acceptOut (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrWedge::acceptOut()" <<
      endl;
  }

  if (visitor<S_msrWedge>*
    p =
      dynamic_cast<visitor<S_msrWedge>*> (v)) {
        S_msrWedge elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrWedge::visitEnd()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrWedge::browseData (basevisitor* v)
{}

ostream& operator<< (ostream& os, const S_msrWedge& elt)
{
  elt->print (os);
  return os;
}

void msrWedge::print (ostream& os)
{
  os <<
    "Wedge" " " << wedgeKindAsString () <<
    ", line " << fInputLineNumber <<
    endl;

  gIndenter++;

  const int fieldWidth = 19;

  os << left <<
    setw (fieldWidth) <<
    "wedgeNienteKind" << " : " <<
    wedgeNienteKindAsString (
      fWedgeNienteKind) << 
    endl <<
    setw (fieldWidth) <<
    "wedgeLineTypeKind" << " : " <<
    msrLineTypeKindAsString (
      fWedgeLineTypeKind) << 
    endl <<
    setw (fieldWidth) <<
    "wedgePlacementKind" << " : " <<
    msrPlacementKindAsString (
      fWedgePlacementKind) << 
    endl;

  gIndenter--;
}

//______________________________________________________________________________
S_msrTie msrTie::create (
  int        inputLineNumber,
  msrTieKind tieKind)
{
  msrTie* o =
    new msrTie (
      inputLineNumber, tieKind);
  assert(o!=0);
  return o;
}

msrTie::msrTie (
  int        inputLineNumber,
  msrTieKind tieKind)
    : msrElement (inputLineNumber)
{
  fTieKind = tieKind; 
}

msrTie::~msrTie()
{}

string msrTie::tieKindAsString (msrTieKind tieKind)
{
  stringstream s;
  
  switch (tieKind) {
    case kTieStart:
      s << "tieStart";
      break;
    case kTieContinue:
      s << "tieContinue";
      break;
    case kTieStop:
      s << "tieStop";
      break;
    case k_NoTie:
      s << "k_NoTie???";
  } // switch
    
  return s.str ();
}

void msrTie::acceptIn (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrTie::acceptIn()" <<
      endl;
  }
      
  if (visitor<S_msrTie>*
    p =
      dynamic_cast<visitor<S_msrTie>*> (v)) {
        S_msrTie elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrTie::visitStart()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrTie::acceptOut (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrTie::acceptOut()" <<
      endl;
  }

  if (visitor<S_msrTie>*
    p =
      dynamic_cast<visitor<S_msrTie>*> (v)) {
        S_msrTie elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrTie::visitEnd()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrTie::browseData (basevisitor* v)
{}

ostream& operator<< (ostream& os, const S_msrTie& elt)
{
  elt->print (os);
  return os;
}

void msrTie::print (ostream& os)
{
  os <<
    "Tie" << " " << tieKindAsString () <<
    ", line " << fInputLineNumber <<
    endl;
}

//______________________________________________________________________________
S_msrSlur msrSlur::create (
  int             inputLineNumber,
  int             slurNumber,
  msrSlurTypeKind slurTypeKind,
  msrLineTypeKind slurLineTypeKind)
{
  msrSlur* o =
    new msrSlur (
      inputLineNumber, slurNumber, slurTypeKind, slurLineTypeKind);
  assert(o!=0);
  return o;
}

msrSlur::msrSlur (
  int             inputLineNumber,
  int             slurNumber,
  msrSlurTypeKind slurTypeKind,
  msrLineTypeKind slurLineTypeKind)
    : msrElement (inputLineNumber)
{
  fSlurNumber       = slurNumber;
  fSlurTypeKind     = slurTypeKind;
  fSlurLineTypeKind = slurLineTypeKind;
}

msrSlur::~msrSlur()
{}

void msrSlur::acceptIn (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrSlur::acceptIn()" <<
      endl;
  }
      
  if (visitor<S_msrSlur>*
    p =
      dynamic_cast<visitor<S_msrSlur>*> (v)) {
        S_msrSlur elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrSlur::visitStart()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrSlur::acceptOut (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrSlur::acceptOut()" <<
      endl;
  }

  if (visitor<S_msrSlur>*
    p =
      dynamic_cast<visitor<S_msrSlur>*> (v)) {
        S_msrSlur elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrSlur::visitEnd()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}


void msrSlur::browseData (basevisitor* v)
{}

string msrSlur::slurTypeKindAsString (
  msrSlurTypeKind slurTypeKind)
{
  stringstream s;
  
  switch (slurTypeKind) {
    case msrSlur::kRegularSlurStart:
      s << "regularSlurStart";
      break;
    case msrSlur::kPhrasingSlurStart:
      s << "phrasingSlurStart";
      break;
    case msrSlur::kSlurContinue:
      s << "slurContinue";
      break;
    case msrSlur::kRegularSlurStop:
      s << "regularSlurStop";
      break;
    case msrSlur::kPhrasingSlurStop:
      s << "phrasingSlurStop";
      break;
    case msrSlur::k_NoSlur:
      s << "Slur" << slurTypeKind << "???";
  } // switch
    
  return s.str ();
}
      
string msrSlur::slurTypeKindAsString () const
{
  return slurTypeKindAsString (fSlurTypeKind);
}

string msrSlur::asString () const
{
  stringstream s;

  s <<
    "Slur" " " << slurTypeKindAsString () <<
    ", slurLineTypeKind = " <<
    msrLineTypeKindAsString (
      fSlurLineTypeKind) <<
    ", slurNumber = " << fSlurNumber <<
    ", line " << fInputLineNumber;
  
  return s.str ();
}

ostream& operator<< (ostream& os, const S_msrSlur& elt)
{
  elt->print (os);
  return os;
}

void msrSlur::print (ostream& os)
{
  os <<
    asString () <<
    endl;
}

//______________________________________________________________________________
S_msrLigature msrLigature::create (
  int                    inputLineNumber,
  int                    ligatureNumber,
  msrLigatureKind        ligatureKind,
  msrLigatureLineEndKind ligatureLineEndKind,
  msrLineTypeKind        ligatureLineTypeKind,
  msrPlacementKind       ligaturePlacementKind)
{
  msrLigature* o =
    new msrLigature (
      inputLineNumber,
      ligatureNumber,
      ligatureKind,
      ligatureLineEndKind,
      ligatureLineTypeKind,
      ligaturePlacementKind);
  assert(o!=0);
  return o;
}

msrLigature::msrLigature (
  int                    inputLineNumber,
  int                    ligatureNumber,
  msrLigatureKind        ligatureKind,
  msrLigatureLineEndKind ligatureLineEndKind,
  msrLineTypeKind        ligatureLineTypeKind,
  msrPlacementKind       ligaturePlacementKind)
    : msrElement (inputLineNumber)
{
  fLigatureNumber        = ligatureNumber;
  fLigatureKind          = ligatureKind; 
  fLigatureLineEndKind   = ligatureLineEndKind; 
  fLigatureLineTypeKind  = ligatureLineTypeKind; 
  fLigaturePlacementKind = ligaturePlacementKind; 
}

msrLigature::~msrLigature()
{}

string msrLigature::ligatureKindAsString (
  msrLigatureKind ligatureKind)
{
  stringstream s;
  
  switch (ligatureKind) {
    case msrLigature::kLigatureStart:
      s << "ligatureStart";
      break;
    case msrLigature::kLigatureContinue:
      s << "ligatureContinue";
      break;
    case msrLigature::kLigatureStop:
      s << "ligatureStop";
      break;
    case k_NoLigature:
      s << "none";
  } // switch
    
  return s.str ();
  
}
     
string msrLigature::ligatureLineEndKindAsString (
  msrLigatureLineEndKind ligatureLineEndKind)
{
  stringstream s;
  
  switch (ligatureLineEndKind) {
    case msrLigature::kLigatureLineEndUp:
      s << "ligatureLineEndUp";
      break;
    case msrLigature::kLigatureLineEndDown:
      s << "ligatureLineEndDown";
      break;
    case msrLigature::kLigatureLineEndBoth:
      s << "ligatureLineEndBoth";
      break;
    case msrLigature::kLigatureLineEndArrow:
      s << "ligatureLineEndArrow";
      break;
    case msrLigature::kLigatureLineEndNone:
      s << "ligatureLineEndNone";
      break;
    case msrLigature::k_NoLigatureLineEnd:
      s << "k_NoLigatureLineEnd ???";
      break;
  } // switch
    
  return s.str ();
  
}
      
string msrLigature::ligatureKindAsString ()
{
  return ligatureKindAsString (fLigatureKind);
}

void msrLigature::acceptIn (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrLigature::acceptIn()" <<
      endl;
  }
      
  if (visitor<S_msrLigature>*
    p =
      dynamic_cast<visitor<S_msrLigature>*> (v)) {
        S_msrLigature elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrLigature::visitStart()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrLigature::acceptOut (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrLigature::acceptOut()" <<
      endl;
  }

  if (visitor<S_msrLigature>*
    p =
      dynamic_cast<visitor<S_msrLigature>*> (v)) {
        S_msrLigature elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrLigature::visitEnd()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}


void msrLigature::browseData (basevisitor* v)
{}

ostream& operator<< (ostream& os, const S_msrLigature& elt)
{
  elt->print (os);
  return os;
}

void msrLigature::print (ostream& os)
{
  os <<
    "Ligature" " " << ligatureKindAsString () <<
    ", line " << fInputLineNumber <<
    endl;

  gIndenter++;

  const int fieldWidth = 22;

  os << left <<
    setw (fieldWidth) <<
    "ligatureLineEndKind" << " : " <<
    ligatureLineEndKindAsString (
      fLigatureLineEndKind) << 
    endl <<
    setw (fieldWidth) <<
    "ligatureLineTypeKind" << " : " <<
    msrLineTypeKindAsString (
      fLigatureLineTypeKind) << 
    endl <<
    setw (fieldWidth) <<
    "ligaturePlacementKind" << " : " <<
    msrPlacementKindAsString (
      fLigaturePlacementKind) << 
    endl;

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

msrGraceNotes::~msrGraceNotes()
{}

S_msrGraceNotes msrGraceNotes::createGraceNotesNewbornClone (
  S_msrVoice containingVoice)
{
  if (gTraceOptions->fTraceGraceNotes) {
    gLogIOstream <<
      "Creating a newborn clone of grace notes" <<
      asShortString () <<
      endl;
  }
  
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
  if (gTraceOptions->fTraceGraceNotes) {
    gLogIOstream <<
      "Creating a skip clone of grace notes" <<
      asShortString () <<
      endl;
  }
  
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
      "removeLastNoteFromGraceNotes(): grace notes element should be a note");
  }   

  fGraceNotesElementsList.pop_back ();

  return result;
}

void msrGraceNotes::acceptIn (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrGraceNotes::acceptIn()" <<
      endl;
  }
      
  if (visitor<S_msrGraceNotes>*
    p =
      dynamic_cast<visitor<S_msrGraceNotes>*> (v)) {
        S_msrGraceNotes elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrGraceNotes::visitStart()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrGraceNotes::acceptOut (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrGraceNotes::acceptOut()" <<
      endl;
  }

  if (visitor<S_msrGraceNotes>*
    p =
      dynamic_cast<visitor<S_msrGraceNotes>*> (v)) {
        S_msrGraceNotes elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrGraceNotes::visitEnd()" <<
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

msrAfterGraceNotesContents::~msrAfterGraceNotesContents()
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
  if (gTraceOptions->fTraceGraceNotes) {
    gLogIOstream <<
      "Creating a newborn clone of after grace notes" <<
      endl;
  }

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
      "% ==> msrAfterGraceNotesContents::acceptIn()" <<
      endl;
  }
      
  if (visitor<S_msrAfterGraceNotesContents>*
    p =
      dynamic_cast<visitor<S_msrAfterGraceNotesContents>*> (v)) {
        S_msrAfterGraceNotesContents elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrAfterGraceNotesContents::visitStart()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrAfterGraceNotesContents::acceptOut (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrAfterGraceNotesContents::acceptOut()" <<
      endl;
  }

  if (visitor<S_msrAfterGraceNotesContents>*
    p =
      dynamic_cast<visitor<S_msrAfterGraceNotesContents>*> (v)) {
        S_msrAfterGraceNotesContents elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrAfterGraceNotesContents::visitEnd()" <<
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

msrAfterGraceNotes::~msrAfterGraceNotes()
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
  if (gTraceOptions->fTraceGraceNotes) {
    gLogIOstream <<
      "Creating a newborn clone of after grace notes" <<
      endl;
  }

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
      "% ==> msrAfterGraceNotes::acceptIn()" <<
      endl;
  }
      
  if (visitor<S_msrAfterGraceNotes>*
    p =
      dynamic_cast<visitor<S_msrAfterGraceNotes>*> (v)) {
        S_msrAfterGraceNotes elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrAfterGraceNotes::visitStart()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrAfterGraceNotes::acceptOut (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrAfterGraceNotes::acceptOut()" <<
      endl;
  }

  if (visitor<S_msrAfterGraceNotes>*
    p =
      dynamic_cast<visitor<S_msrAfterGraceNotes>*> (v)) {
        S_msrAfterGraceNotes elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrAfterGraceNotes::visitEnd()" <<
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
    msrNote::k_NoNoteAccidental; // default value
  
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

        // fNoteOctaveShift JMI ???

      // accidentals
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

S_msrNote msrNote::createNoteNewbornClone (
  S_msrPart containingPart)
{
  if (gTraceOptions->fTraceNotes) {
    gLogIOstream <<
      "Creating a newborn clone of note " <<
      asString () <<
      " in part " <<
      containingPart->
        getPartCombinedName () << 
      endl;
  }

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

  if (fNoteSingleTremolo) {
    noteDeepCopy->fNoteSingleTremolo =
      fNoteSingleTremolo->
        createSingleTremoloDeepCopy (
          this);
  }

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
  
  {
    list<S_msrWedge>::const_iterator i;
    for (i=fNoteWedges.begin (); i!=fNoteWedges.end (); i++) {
      // share this data
      noteDeepCopy->
        fNoteWedges.push_back ((*i));
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
      result = "k_NoNoteKind???";
      break;
      
    case msrNote::kRestNote:
      result = "rest";
      break;
      
    case msrNote::kSkipNote:
      result = "skip";
      break;
      
    case msrNote::kStandaloneNote:
      result = "standalone";
      break;
      
    case msrNote::kDoubleTremoloMemberNote:
      result = "double tremolo member note";
      break;
      
    case msrNote::kGraceNote:
      result = "grace note";
      break;
      
    case msrNote::kGraceChordMemberNote:
      result = "grace chord member";
      break;
      
    case msrNote::kChordMemberNote:
      result = "chord member";
      break;
      
    case msrNote::kTupletMemberNote:
      result = "tuplet member";
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
    case k_NoNoteAccidental:
      result = "none";
      break;
      
    case kNoteAccidentalSharp:
      result = "NoteAccidentalSharp";
      break;
    case kNoteAccidentalNatural:
      result = "NoteAccidentalNatural";
      break;
    case kNoteAccidentalFlat:
      result = "NoteAccidentalFlat";
      break;
    case kNoteAccidentaldoubleSharp:
      result = "NoteAccidentaldoubleSharp";
      break;
    case kNoteAccidentalSharpSharp:
      result = "NoteAccidentalSharpSharp";
      break;
    case kNoteAccidentalFlatFlat:
      result = "NoteAccidentalFlatFlat";
      break;
    case kNoteAccidentalNaturalSharp:
      result = "NoteAccidentalNaturalSharp";
      break;
    case kNoteAccidentalNaturalFlat:
      result = "NoteAccidentalNaturalFlat";
      break;
    case kNoteAccidentalQuarterFlat:
      result = "NoteAccidentalQuarterFlat";
      break;
    case kNoteAccidentalQuarterSharp:
      result = "NoteAccidentalQuarterSharp";
      break;
    case kNoteAccidentalThreeQuartersFlat:
      result = "NoteAccidentalThreeQuartersFlat";
      break;
    case kNoteAccidentalThreeQuartersSharp:
      result = "NoteAccidentalThreeQuartersSharp";
      break;
      
    case kNoteAccidentalSharpDown:
      result = "NoteAccidentalSharpDown";
      break;
    case kNoteAccidentalSharpUp:
      result = "NoteAccidentalSharpUp";
      break;
    case kNoteAccidentalNaturalDown:
      result = "NoteAccidentalNaturalDown";
      break;
    case kNoteAccidentalNaturalUp:
      result = "NoteAccidentalNaturalUp";
      break;
    case kNoteAccidentalFlatDown:
      result = "NoteAccidentalFlatDown";
      break;
    case kNoteAccidentalFlatUp:
      result = "NoteAccidentalFlatUp";
      break;
    case kNoteAccidentalTripleSharp:
      result = "NoteAccidentalTripleSharp";
      break;
    case kNoteAccidentalTripleFlat:
      result = "NoteAccidentalTripleFlat";
      break;
    case kNoteAccidentalSlashQuarterSharp:
      result = "NoteAccidentalSlashQuarterSharp";
      break;
    case kNoteAccidentalSlashSharp:
      result = "NoteAccidentalSlashSharp";
      break;
    case kNoteAccidentalSlashFlat:
      result = "NoteAccidentalSlashFlat";
      break;
    case kNoteAccidentaldoubleSlashFlat:
      result = "NoteAccidentaldoubleSlashFlat";
      break;
    case kNoteAccidentalSharp_1:
      result = "NoteAccidentalSharp_1";
      break;
    case kNoteAccidentalSharp_2:
      result = "NoteAccidentalSharp_2";
      break;
    case kNoteAccidentalSharp_3:
      result = "NoteAccidentalSharp_3";
      break;
    case kNoteAccidentalSharp_5:
      result = "NoteAccidentalSharp_5";
      break;
    case kNoteAccidentalFlat_1:
      result = "NoteAccidentalFlat_1";
      break;
    case kNoteAccidentalFlat_2:
      result = "NoteAccidentalFlat_2";
      break;
    case kNoteAccidentalFlat_3:
      result = "NoteAccidentalFlat_3";
      break;
    case kNoteAccidentalFlat_4:
      result = "NoteAccidentalFlat_4";
      break;
    case kNoteAccidentalSori:
      result = "NoteAccidentalSori";
      break;
    case kNoteAccidentalKoron:
      result = "NoteAccidentalKoron";
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
  if (gTraceOptions->fTraceChords) {
    gLogIOstream <<
      "Setting note '" <<
      asShortStringWithRawWholeNotes () <<
      "' to belong to a chord"
      ", line " << fInputLineNumber <<
      endl;
  }

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

  // the display whole notes are known, compute the sounding ones
  fNoteSoundingWholeNotes =
    fNoteDisplayWholeNotes
      *
    normalNotes
      /
    actualNotes;

  fNoteSoundingWholeNotes.rationalise ();

  if (gTraceOptions->fTraceTuplets || gTraceOptions->fTraceNotes) {
    gLogIOstream <<
      "The result is: '" <<
      asShortString () <<
      endl;
  }
}

void msrNote::addBeamToNote (S_msrBeam beam)
{
  fNoteBeams.push_back (beam);
}

void msrNote::addArticulationToNote (S_msrArticulation art)
{
  fNoteArticulations.push_back (art);
}

void msrNote::addSpannerToNote (S_msrSpanner spanner)
{
  if (gTraceOptions->fTraceSpanners || gTraceOptions->fTraceNotes) {
    gLogIOstream <<
      "Adding spanner '" <<
      spanner->spannerKindAsString () <<
      "' to note '" <<
      asShortString () <<
      "'" <<
      endl;
  }

  // register note has having a wavy line start
  switch (spanner->getSpannerKind ()) {
    case msrSpanner::kSpannerTrill:
      break;
      
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

void msrNote::addTechnicalToNote (S_msrTechnical technical)
{
  if (gTraceOptions->fTraceNotes || gTraceOptions->fTraceTechnicals) {
    gLogIOstream <<
      "Adding technical '" <<
      technical->asString () <<
      "' to note '" << asShortString () <<
      "', line " << fInputLineNumber <<
      endl;
  }
  
  // append the technical to the note technicals list
  fNoteTechnicals.push_back (technical);

  // set technical's note uplink
  technical->
    setTechnicalNoteUplink (this);
}

void msrNote::addTechnicalWithIntegerToNote (
  S_msrTechnicalWithInteger technicalWithInteger)
{
  if (gTraceOptions->fTraceNotes || gTraceOptions->fTraceTechnicals) {
    gLogIOstream <<
      "Adding technical with integer '" <<
      technicalWithInteger->asString () <<
      "' to note '" << asShortString () <<
      "', line " << fInputLineNumber <<
      endl;
  }

  // append the technical with integer to the note technicals with integers list
  fNoteTechnicalWithIntegers.push_back (technicalWithInteger);

  // set technical's note uplink
  technicalWithInteger->
    setTechnicalWithIntegerNoteUplink (this);
}

void msrNote::addTechnicalWithStringToNote (
  S_msrTechnicalWithString technicalWithString)
{
  if (gTraceOptions->fTraceNotes || gTraceOptions->fTraceTechnicals) {
    gLogIOstream <<
      "Adding technical with string'" <<
      technicalWithString->asString () <<
      "' to note '" << asShortString () <<
      "', line " << fInputLineNumber <<
      endl;
  }

  // append the technical with integer to the note technicals with integers list
  fNoteTechnicalWithStrings.push_back (technicalWithString);

  // set technical's note uplink
  technicalWithString->
    setTechnicalWithStringNoteUplink (this);
}

void msrNote::addOrnamentToNote (S_msrOrnament ornament)
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

void msrNote::addGlissandoToNote (S_msrGlissando glissando)

{
  // append the glissando to the note glissandos list
  fNoteGlissandos.push_back (glissando);
}

void msrNote::addSlideToNote (S_msrSlide slide)

{
  // append the slide to the note glissandos list
  fNoteSlides.push_back (slide);
}

void msrNote::addSingleTremoloToNote (S_msrSingleTremolo trem)
{
  if (gTraceOptions->fTraceTremolos) {
    gLogIOstream <<
      "Adding singleTremolo " << trem->asString () <<
      " to note " << asShortString () <<
      ", line " << trem->getInputLineNumber () <<
      endl;
  }

  // register the singleTremolo in the note
  fNoteSingleTremolo = trem;

  // set singleTremolo's note uplink
  trem->
    setSingleTremoloNoteUplink (this);
}

void msrNote::addDynamicsToNote (S_msrDynamics dynamics)
{
  fNoteDynamics.push_back (dynamics);
}
void msrNote::addOtherDynamicsToNote (S_msrOtherDynamics otherDynamics)
{
  fNoteOtherDynamics.push_back (otherDynamics);
}

void msrNote::addWordsToNote (S_msrWords words)
{
  fNoteWords.push_back (words);
}

void msrNote::addSlurToNote (S_msrSlur slur)
{
  if (gTraceOptions->fTraceSlurs) {
    gLogIOstream <<
      "Adding slur '" << slur <<
      "' to note '" << asString () << "'" <<
      endl;
  }

  fNoteSlurs.push_back (slur);
}

void msrNote::addLigatureToNote (S_msrLigature ligature)
{
  if (gTraceOptions->fTraceLigatures) {
    gLogIOstream <<
      "Adding ligature " << ligature << " to note " << asString () <<
       endl;
  }

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

void msrNote::addWedgeToNote (S_msrWedge wedge)
{
  fNoteWedges.push_back (wedge);
}

S_msrDynamics msrNote::removeFirstDynamics () // JMI
{
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
  if (gTraceOptions->fTraceLyrics) {
    gLogIOstream <<
      "Appending syllable '" <<
      syllable->asString () <<
      "' to note " << asString () <<
      endl;
  }
  
  fNoteSyllables.push_back (syllable);
}

void msrNote::setNoteHarmony (S_msrHarmony harmony)
{
  if (gTraceOptions->fTraceNotes || gTraceOptions->fTraceHarmonies) {
    gLogIOstream <<
      "Setting note '" << asShortString ()  << "'" <<
      " harmony to '" << harmony->asString () << "'" <<
      ", line " << fInputLineNumber <<
      endl;
  }
  
  fNoteHarmony = harmony;

  // register this note as the harmony uplink
  harmony->
    setHarmonyNoteUplink (this);
}

void msrNote::setNoteFrame (S_msrFrame frame)
{
  if (gTraceOptions->fTraceNotes || gTraceOptions->fTraceHarmonies) {
    gLogIOstream <<
      "Setting note '" << asShortString ()  << "'" <<
      " frame to '" << frame->asString () << "'" <<
      endl;
  }
  
  fNoteFrame = frame;
}

void msrNote::setNoteFiguredBass (S_msrFiguredBass figuredBass)
{
  if (gTraceOptions->fTraceNotes || gTraceOptions->fTraceHarmonies) {
    gLogIOstream <<
      "Setting note '" << asShortString ()  << "'" <<
      " figured bass to '" << figuredBass->asString () << "'" <<
      endl;
  }
      
  fNoteFiguredBass = figuredBass;
}

void msrNote::acceptIn (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrNote::acceptIn()" <<
      endl;
  }
      
  if (visitor<S_msrNote>*
    p =
      dynamic_cast<visitor<S_msrNote>*> (v)) {
        S_msrNote elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrNote::visitStart()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrNote::acceptOut (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrNote::acceptOut()" <<
      endl;
  }

  if (visitor<S_msrNote>*
    p =
      dynamic_cast<visitor<S_msrNote>*> (v)) {
        S_msrNote elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrNote::visitEnd()" <<
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
  
  /*
  gLogIOstream << "msrNote::notePitchAsString (), isRest = " <<
    fNoteIsARest <<
    ", fQuarterTonesPitch = " << fQuarterTonesPitch << endl;
  */

  /*
  if (fNoteIsARest) {
    if (fNoteOccupiesAFullMeasure) // JMI
      s << "R";
    else
      s << "r";
  }
  
  else
  */
  
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
  
  /* JMI
  gLogIOstream << "msrNote::notePitchAsString (), isRest = " <<
    fNoteIsARest <<
    ", fQuarterTonesPitch = " << fQuarterTonesPitch << endl;

  
  if (fNoteIsARest) {
    if (fNoteOccupiesAFullMeasure) // JMI
      s << "R";
    else
      s << "r";
  }
  
  else if (fNoteIsUnpitched)

    s << "unpitched ";

  else {
  */
    s <<
      msrQuarterTonesPitchKindAsString (
        gMsrOptions->fMsrQuarterTonesPitchesLanguageKind,
        fNoteQuarterTonesDisplayPitchKind);  

 // JMI }
  
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
        "???";
      break;
      
    case msrNote::kRestNote:
      s <<
        "R" <<
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
        "S" <<
        ":" <<
        ", whole notes: " <<
        fNoteSoundingWholeNotes <<
        " sound, " <<
        fNoteDisplayWholeNotes <<
        " disp";
      break;
      
    case msrNote::kStandaloneNote:
      s <<
        notePitchAsString () <<
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
        notePitchAsString () <<
        "[octave: " << fNoteOctave << ", " << noteDisplayOctaveAsString () << "]" <<
        ":" <<
        " whole notes: " <<
        fNoteSoundingWholeNotes <<
        " sound, " <<
        fNoteDisplayWholeNotes <<
        " disp";
      break;
      
    case msrNote::kGraceNote:
    case msrNote::kGraceChordMemberNote:
      s <<
        notePitchAsString () <<
        noteGraphicDurationAsMsrString () <<
        "[octave: " << fNoteOctave << ", " << noteDisplayOctaveAsString () << "]";
        
      for (int i = 0; i < fNoteDotsNumber; i++) {
        s << "."; // JMI
      } // for
      break;
      
    case msrNote::kChordMemberNote:
      s <<
        notePitchAsString () <<
        "[octave: " << fNoteOctave << ", " << noteDisplayOctaveAsString () << "]" <<
        ", whole notes: " <<
        fNoteSoundingWholeNotes <<
        " sound, " <<
        fNoteDisplayWholeNotes <<
        " disp";
      break;
      
    case msrNote::kTupletMemberNote:
      s <<
        notePitchAsString () <<
        noteGraphicDurationAsMsrString ();

      if (! fNoteIsARest)
        s <<
        "[octave: " << fNoteOctave << ", " << noteDisplayOctaveAsString () << "]";

      s <<
        ", whole notes: " <<
        fNoteSoundingWholeNotes <<
        " sound, " <<
        fNoteDisplayWholeNotes <<
        " disp";
      break;
  } // switch

  return s.str ();
}

string msrNote::asShortString () const
{
  stringstream s;

  switch (fNoteKind) {
    case msrNote::k_NoNoteKind:
      s <<
        "note kind: unknown" <<
        ":" <<
        noteSoundingWholeNotesAsMsrString ();
      break;
      
    case msrNote::kRestNote:
      s <<
        "Rest" <<
        ":" <<
        noteSoundingWholeNotesAsMsrString ();
      break;
      
    case msrNote::kSkipNote:
      s <<
        "Skip" <<
        ":" <<
        noteSoundingWholeNotesAsMsrString ();
      break;
      
    case msrNote::kStandaloneNote:
      s <<
        notePitchAsString () <<
        noteSoundingWholeNotesAsMsrString () <<
        " [octave: " << fNoteOctave << ", " << noteDisplayOctaveAsString () << "]";
      break;
      
    case msrNote::kDoubleTremoloMemberNote:
      s <<
        notePitchAsString () <<
        noteSoundingWholeNotesAsMsrString () <<
        " [octave: " << fNoteOctave << ", " << noteDisplayOctaveAsString () << "]";
      break;
      
    case msrNote::kGraceNote:
    case msrNote::kGraceChordMemberNote:
      s <<
        notePitchAsString () <<
        noteGraphicDurationAsMsrString () <<
        " [octave: " << fNoteOctave << ", " << noteDisplayOctaveAsString () << "]";
        
      for (int i = 0; i < fNoteDotsNumber; i++) {
        s << "."; // JMI
      } // for
      break;
      
    case msrNote::kChordMemberNote:
      s <<
        notePitchAsString () <<
        ", " <<
        noteSoundingWholeNotesAsMsrString () <<
        " [octave: " << fNoteOctave << ", " << noteDisplayOctaveAsString () << "]";
      break;
      
    case msrNote::kTupletMemberNote:
      s <<
        notePitchAsString () <<
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

      s <<
        ", line " << fInputLineNumber;
      break;
  } // switch

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
        "k_NoNoteKind???";
      break;
      
    case msrNote::kRestNote:
      s <<
        "Rest, ";
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

      if (fNoteDisplayOctave < 0)
        s <<
          ", unpitched";
      else
        s <<
          " (" <<
          noteDisplayPitchKindAsString () <<
          noteSoundingWholeNotesAsMsrString () <<
          ", octave" " "<< noteDisplayOctaveAsString () <<
          ")";

      s <<
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
        "Skip" <<
        ":" <<
        noteSoundingWholeNotesAsMsrString ();
      break;
      
    case msrNote::kStandaloneNote:
      s <<
        "Standalone note" " "<<
        notePitchAsString () <<
        noteSoundingWholeNotesAsMsrString () <<
        " [octave" " " << fNoteOctave << ", " << noteDisplayOctaveAsString () << "]";
      break;
      
    case msrNote::kDoubleTremoloMemberNote:
      s <<
        "Double tremolo note "<<
        notePitchAsString () <<
        noteSoundingWholeNotesAsMsrString () <<
        " [octave" " " << fNoteOctave << ", " << noteDisplayOctaveAsString () << "]";
      break;
      
    case msrNote::kGraceNote:
    case msrNote::kGraceChordMemberNote:
      s <<
        "Grace note" " "<<
        notePitchAsString () <<
 // JMI       noteGraphicDurationAsMsrString () <<
        noteDisplayWholeNotesAsMsrString () <<
        " [octave" " " << fNoteOctave << ", " << noteDisplayOctaveAsString () << "]";
        
      for (int i = 0; i < fNoteDotsNumber; i++) {
        s << "."; // JMI
      } // for
      break;
      
    case msrNote::kChordMemberNote:
      s <<
        "Chord member note" " "<<
        notePitchAsString () <<
 // JMI       ", " << fNoteSoundingWholeNotes << " sound whole notes, " <<
        " [octave" " " << fNoteOctave << ", " << noteDisplayOctaveAsString () << "]";
      break;
      
    case msrNote::kTupletMemberNote:
      s <<
        "Tuplet member note"  " "<<
        notePitchAsString () <<
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

      if (! fNoteIsARest)
        s <<
        " [octave" " " << fNoteOctave << ", " << noteDisplayOctaveAsString () << "]";
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
    fullMeasureLength =
      fNoteMeasureUplink
        ? 
          fNoteMeasureUplink->
            getFullMeasureLength ()
        : rational (0, 1); // JMI
  
  // print the note itself and its positionInMeasure
  os <<
    asString () <<
    ", line " << fInputLineNumber;

  {
    // print displayed whole notes
    os <<
      endl;
  
    gIndenter++;
        
    switch (fNoteKind) {
      case msrNote::k_NoNoteKind:
      case msrNote::kRestNote:
      case msrNote::kSkipNote:
      case msrNote::kStandaloneNote:
      case msrNote::kDoubleTremoloMemberNote:
      case msrNote::kChordMemberNote:
        os <<
          "Whole notes: " <<
          fNoteSoundingWholeNotes <<
          " sounding, " <<
          fNoteDisplayWholeNotes<<
          " display";
        break;
  
      case msrNote::kGraceNote:
      case msrNote::kGraceChordMemberNote:
        os <<
          "Whole notes: " <<
          fNoteDisplayWholeNotes <<
          " display";
        break;
  
      case msrNote::kTupletMemberNote:
        os <<
          "Whole notes: " <<
          fNoteSoundingWholeNotes <<
          " sounding, " <<
          fNoteDisplayWholeNotes<<
          " display" <<
          ", note tuplet sounding: " <<
          wholeNotesAsMsrString (
            fInputLineNumber,
            getNoteTupletUplink ()->
              getTupletSoundingWholeNotes ());
        break;
      } // switch

    // full measure length,
    // may be unknown if there is no time signature
    if (fullMeasureLength.getNumerator () == 0) {
      os <<
        ", full measure length unknown, no time signature";
    }
    else {
      os <<
        ", " <<
        fullMeasureLength <<
        " per full measure";
    }

    os <<
      endl;

    // print kind

    os <<
      notePrintKindAsString ();

    // note head
    os <<
      ", " <<
        noteHeadKindAsString () <<
      ", " <<
        noteHeadFilledKindAsString () <<
      ", " <<
        noteHeadParenthesesKindAsString ();
  
    os <<
      endl;
      
    // accidentals
    os <<
      "accidental kind: " <<
      noteAccidentalKindAsString (
        fNoteAccidentalKind) <<
      endl;

    os <<
      noteEditorialAccidentalKindAsString (
        fNoteEditorialAccidentalKind) <<
      ", " <<
      noteCautionaryAccidentalKindAsString (
        fNoteCautionaryAccidentalKind) <<
      endl;

    // print measure related information
    os <<
      "Measure: ";
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
      ", position in measure: ";
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
          const int fieldWidth = 46;

          os << left <<
            setw (fieldWidth) <<
            "noteSoundingWholeNotesAsMsrString" << " = \"" <<
            noteSoundingWholeNotesAsMsrString () <<
            "\"" <<
            endl;
        }
        break;
        
      case msrNote::kSkipNote:
        {
          const int fieldWidth = 46;

          os << left <<
            setw (fieldWidth) <<
            "noteSoundingWholeNotesAsMsrString" << " = \"" <<
            noteSoundingWholeNotesAsMsrString () <<
            "\"" <<
            endl;
        }
        break;
        
      case msrNote::kStandaloneNote:
        {
          const int fieldWidth = 36;

          os << left <<
            setw (fieldWidth) <<
            "noteSoundingWholeNotesAsMsrString" << " = \"" <<
            noteSoundingWholeNotesAsMsrString () <<
            "\"" <<
            endl <<
            setw (fieldWidth) <<
            "noteDisplayWholeNotesAsMsrString" << " = \"" <<
            noteDisplayWholeNotesAsMsrString () <<
            "\"" <<
            endl <<
            setw (fieldWidth) <<
            "noteGraphicDurationAsMsrString" << " = \"" <<
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
          const int fieldWidth = 46;

          os << left <<
            setw (fieldWidth) <<
            "noteGraphicDurationAsMsrString" << " = \"" <<
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
        {
          const int fieldWidth = 46;

          os << left <<
          /* JMI
            setw (fieldWidth) <<
            "noteTupletNoteGraphicDurationAsMsrString" << " = \"" <<
            fNoteTupletNoteGraphicDurationAsMsrString <<
            "\"" <<
            endl <<
              */
            setw (fieldWidth) <<
            "noteTupletNoteSoundingWholeNotes" << " = \"" <<
            wholeNotesAsMsrString (
              fInputLineNumber,
              getNoteTupletUplink ()->
                getTupletSoundingWholeNotes ()) <<
            "\"" <<
            endl <<
            setw (fieldWidth) <<
            "noteGraphicDurationAsMsrString" << " = \"" <<
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
        ": ";

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
  if (gTraceOptions->fTraceChords) {
    gLogIOstream <<
      "Creating a chord" <<
      ", chordSoundingWholeNotes = " << chordSoundingWholeNotes <<
      ", chordDisplayWholeNotes = " << chordDisplayWholeNotes <<
      ", chordGraphicDuration = " <<
      msrDurationKindAsString (chordGraphicDurationKind) <<
      endl;
 }
   
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

msrChord::~msrChord()
{}

S_msrChord msrChord::createChordNewbornClone (
  S_msrPart containingPart)
{
  if (gTraceOptions->fTraceChords) {
    gLogIOstream <<
      "Creating a newborn clone of chord '" <<
      asString () <<
      "'" <<
      endl;
  }

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
  if (gTraceOptions->fTraceChords) {
    gLogIOstream <<
      "Setting chord sounding whole notes to '" <<
      wholeNotes <<
      "' for chord '" <<
      asString () <<
      "'" <<
      endl;
  }

  fChordSoundingWholeNotes = wholeNotes;
}

void msrChord::setChordDisplayWholeNotes (
  rational wholeNotes)
{
  if (gTraceOptions->fTraceChords) {
    gLogIOstream <<
      "Setting chord displayed whole notes to '" <<
      wholeNotes <<
      "' for chord '" <<
      asString () <<
      "'" <<
      endl;
  }

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
  if (gTraceOptions->fTraceChords) {
    gLogIOstream <<
      "Adding first note '" <<
      note->asString () <<
      "' to chord '" <<
      asString () <<
      "'" <<
      endl;
  }

  // append note to chord notes
  fChordNotes.push_back (note);

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
  if (gTraceOptions->fTraceChords) {
    gLogIOstream <<
      "Adding another note '" <<
      note->asShortStringWithRawWholeNotes () <<
      "' to chord '" <<
      asString () <<
      "'" <<
      endl;
  }

  fChordNotes.push_back (note);
  
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
  if (fChordNotes.size ()) { // JMI
    fChordNotes.front ()->
      setNotePositionInMeasure (positionInMeasure);
  }
  else {
    // exit (44); JMI
 }
}
                    
void msrChord::setChordFirstNoteMeasureNumber (
  string measureNumber)
{
  if (fChordNotes.size ()) { // JMI
    fChordNotes.front ()->
      setNoteMeasureNumber (measureNumber);
  }
  else {
    // exit (44); JMI
 }
}
                    
void msrChord::addArticulationToChord (S_msrArticulation art)
{
  msrArticulation::msrArticulationKind
    articulationKind =
      art->
        getArticulationKind ();

  // don't add the same articulation several times
  for (
    list<S_msrArticulation>::const_iterator i = fChordArticulations.begin ();
    i!=fChordArticulations.end ();
    i++) {
      if ((*i)->getArticulationKind () == articulationKind)
        return;
  } // for

  if (gTraceOptions->fTraceTremolos || gTraceOptions->fTraceChords) { // JMI
    gLogIOstream <<
      "Adding articulation '" <<
      art->articulationKindAsString () <<
      "' to chord" <<
      endl;
  }

  fChordArticulations.push_back (art);
}

void msrChord::addSpannerToChord (S_msrSpanner span)
{
  msrSpanner::msrSpannerKind
    spannerKind =
      span->
        getSpannerKind ();

  // don't add the same spanner several times
  for (
    list<S_msrSpanner>::const_iterator i = fChordSpanners.begin ();
    i!=fChordSpanners.end ();
    i++) {
      if ((*i)->getSpannerKind () == spannerKind)
        return;
  } // for

  if (gTraceOptions->fTraceSpanners || gTraceOptions->fTraceChords) {
    gLogIOstream <<
      "Adding spanner '" <<
      span->spannerKindAsString () <<
      "' to chord '" <<
      asString () <<
      "'" <<
      endl;
  }

  fChordSpanners.push_back (span);
}

void msrChord::addSingleTremoloToChord (S_msrSingleTremolo trem)
{
  if (gTraceOptions->fTraceTremolos || gTraceOptions->fTraceChords) {
    gLogIOstream <<
      "Adding singleTremolo '" <<
      trem->asString () <<
      "' to chord" <<
      endl;
  }

  fChordSingleTremolo = trem;
}

void msrChord::addTechnicalToChord (S_msrTechnical tech)
{
  msrTechnical::msrTechnicalKind
    technicalKind =
      tech->
        getTechnicalKind ();

  // don't add the same technical several times
  for (
    list<S_msrTechnical>::const_iterator i =
      fChordTechnicals.begin ();
    i!=fChordTechnicals.end ();
    i++) {
      if ((*i)->getTechnicalKind () == technicalKind)
        return;
  } // for

  if (gTraceOptions->fTraceChords) {
    gLogIOstream <<
      "Adding technical '" <<
      tech->asString () <<
      "' to chord" <<
      endl;
  }

  fChordTechnicals.push_back (tech);
}

void msrChord::addTechnicalWithIntegerToChord (
  S_msrTechnicalWithInteger tech)
{
  msrTechnicalWithInteger::msrTechnicalWithIntegerKind
    technicalWithIntegerKind =
      tech->
        getTechnicalWithIntegerKind ();

  // don't add the same technical several times
  for (
    list<S_msrTechnicalWithInteger>::const_iterator i =
      fChordTechnicalWithIntegers.begin ();
    i!=fChordTechnicalWithIntegers.end ();
    i++) {
      if ((*i)->getTechnicalWithIntegerKind () == technicalWithIntegerKind)
        return;
  } // for

  if (gTraceOptions->fTraceChords) {
    gLogIOstream <<
      "Adding technical with integer '" <<
      tech->asString () <<
      "' to chord" <<
      endl;
  }

  fChordTechnicalWithIntegers.push_back (tech);
}

void msrChord::addTechnicalWithStringToChord (
  S_msrTechnicalWithString tech)
{
  msrTechnicalWithString::msrTechnicalWithStringKind
    technicalWithStringKind =
      tech->
        getTechnicalWithStringKind ();

  // don't add the same technical several times
  for (
    list<S_msrTechnicalWithString>::const_iterator i =
      fChordTechnicalWithStrings.begin ();
    i!=fChordTechnicalWithStrings.end ();
    i++) {
      if ((*i)->getTechnicalWithStringKind () == technicalWithStringKind)
        return;
  } // for

  if (gTraceOptions->fTraceChords) {
    gLogIOstream <<
      "Adding technical with string '" <<
      tech->asString () <<
      "' to chord" <<
      endl;
  }

  fChordTechnicalWithStrings.push_back (tech);
}

void msrChord::addOrnamentToChord (S_msrOrnament orn)
{
  msrOrnament::msrOrnamentKind
    ornamentKind =
      orn->
        getOrnamentKind ();

  // don't add the same ornament several times
  for (
    list<S_msrOrnament>::const_iterator i = fChordOrnaments.begin ();
    i!=fChordOrnaments.end ();
    i++) {
      if ((*i)->getOrnamentKind () == ornamentKind)
        return;
  } // for

  if (gTraceOptions->fTraceChords) {
    gLogIOstream <<
      "Adding ornament '" <<
      orn->ornamentKindAsString () <<
      "' to chord" <<
      endl;
  }

  fChordOrnaments.push_back (orn);
}

void msrChord::addGlissandoToChord (S_msrGlissando gliss)
{
  msrGlissando::msrGlissandoTypeKind
    glissandoTypeKind =
      gliss->
        getGlissandoTypeKind ();

  // don't add the same slissando several times
  for (
    list<S_msrGlissando>::const_iterator i = fChordGlissandos.begin ();
    i!=fChordGlissandos.end ();
    i++) {
      if ((*i)->getGlissandoTypeKind () == glissandoTypeKind)
        return;
  } // for

  if (gTraceOptions->fTraceChords) {
    gLogIOstream <<
      "Adding glissando '" <<
      gliss->asString () <<
      "' to chord" <<
      endl;
  }

  fChordGlissandos.push_back (gliss);
}

void msrChord::addSlideToChord (S_msrSlide slide)
{
  msrSlide::msrSlideTypeKind
    slideTypeKind =
      slide->
        getSlideTypeKind ();

  // don't add the same slide several times
  for (
    list<S_msrSlide>::const_iterator i = fChordSlides.begin ();
    i!=fChordSlides.end ();
    i++) {
      if ((*i)->getSlideTypeKind () == slideTypeKind)
        return;
  } // for

  if (gTraceOptions->fTraceChords) {
    gLogIOstream <<
      "Adding slide '" <<
      slide->asString () <<
      "' to chord" <<
      endl;
  }

  fChordSlides.push_back (slide);
}

void msrChord::acceptIn (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrChord::acceptIn()" <<
      endl;
  }
      
  if (visitor<S_msrChord>*
    p =
      dynamic_cast<visitor<S_msrChord>*> (v)) {
        S_msrChord elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrChord::visitStart()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrChord::acceptOut (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrChord::acceptOut()" <<
      endl;
  }

  if (visitor<S_msrChord>*
    p =
      dynamic_cast<visitor<S_msrChord>*> (v)) {
        S_msrChord elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrChord::visitEnd()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrChord::browseData (basevisitor* v)
{
  for (
    vector<S_msrNote>::const_iterator i = fChordNotes.begin ();
    i != fChordNotes.end ();
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
    list<S_msrArticulation>::const_iterator i = fChordArticulations.begin ();
    i != fChordArticulations.end ();
    i++ ) {
    // browse the spanner
    msrBrowser<msrArticulation> browser (v);
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
  if (gTraceOptions->fTraceTuplets || gTraceOptions->fTraceChords) {
    gLogIOstream <<
      "Applying tuplet member sounding factor '" <<
      actualNotes << "/" << normalNotes <<
      "' to the members of chord '" << asStringwithRawDivisions () <<
      "', line " << fInputLineNumber <<
      endl;

  for (
    vector<S_msrNote>::const_iterator i = fChordNotes.begin ();
    i != fChordNotes.end ();
    ++i) {
    (*i)->
      applyTupletMemberSoundingFactorToNote (
        actualNotes, normalNotes);
  } // for
}
*/

string msrChord::asStringwithRawDivisions () const
{
  stringstream s;

  s << "<";

  if (fChordNotes.size ()) {
    vector<S_msrNote>::const_iterator
      iBegin = fChordNotes.begin (),
      iEnd   = fChordNotes.end (),
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

  if (fChordNotes.size ()) {
    vector<S_msrNote>::const_iterator
      iBegin = fChordNotes.begin (),
      iEnd   = fChordNotes.end (),
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

void msrChord::print (ostream& os)
{
  rational
    fullMeasureLength =
      fChordMeasureUplink
        ? 
          fChordMeasureUplink->
            getFullMeasureLength ()
        : rational (0, 1); // JMI
    
  os <<
    "Chord: " <<
    singularOrPlural (
      fChordNotes.size (), "note", "notes") <<
    ", " <<
    fChordSoundingWholeNotes << " sound whole notes" <<
    ", " <<
    fChordDisplayWholeNotes << " disp whole notes" <<
    ", measure '"<<
    getChordMeasureNumber () <<
    "':" <<
    fChordPositionInMeasure <<
    ", fullMeasureLength: " <<
    fullMeasureLength;

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
      os <<
        " ( = " << chordPositionBis << ")";
    }

  if (fChordIsFirstChordInADoubleTremolo)
    os <<
      ", chord is first chord in a double tremolo";
      
  if (fChordIsSecondChordInADoubleTremolo)
    os <<
      ", chord is second chord in a double tremolo";
      
  os <<
    endl;
    
  gIndenter++;
  
  // print the member notes if any
  if (fChordNotes.size ()) {
    vector<S_msrNote>::const_iterator i;
    for (i=fChordNotes.begin (); i!=fChordNotes.end (); i++) {
      os << (*i);
    } // for

/* JMI   vector<S_msrNote>::const_iterator
      iBegin = fChordNotes.begin (),
      iEnd   = fChordNotes.end (),
      i      = iBegin;
    for ( ; ; ) {
   //   os << (*i)->notePitchAsString (); JMI
      os << (*i);
      if (++i == iEnd) break;
 //     os << endl;
    } // for
    */
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

  // print the slidess if any
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

  gIndenter--;
}

//______________________________________________________________________________
S_msrDivisions msrDivisions::create (
  int inputLineNumber,
  int divisionsPerQuarterNote)
{
  msrDivisions* o =
    new msrDivisions (
      inputLineNumber, divisionsPerQuarterNote);
  assert(o!=0);
  return o;
}

S_msrDivisions msrDivisions::createDivisionsNewbornClone ()
{
 if (gTraceOptions->fTraceDivisions) {
    gLogIOstream <<
      "Creating a newborn clone of divisions '" <<
      divisionsAsString () <<
      "'" <<
      endl;
  }

  S_msrDivisions
    newbornClone =
      msrDivisions::create (
        fInputLineNumber,
        fDivisionsPerQuarterNote);

  return newbornClone;
}

msrDivisions::msrDivisions (
  int inputLineNumber,
  int divisionsPerQuarterNote)
    : msrElement (inputLineNumber)
{
  // set divisions per quarter note
  fDivisionsPerQuarterNote = divisionsPerQuarterNote;

  // initialize the divisions
  initializeDivisions ();
}

void msrDivisions::initializeDivisions ()
{
  if (gTraceOptions->fTraceDivisions) {
  gLogIOstream <<
    "Initializing divisions" <<
    ", divisionsPerQuarterNote = " << fDivisionsPerQuarterNote <<
    ", line " << fInputLineNumber <<
    endl;
  }
  
  // forget fDurationsToDivisions's contents
  fDurationKindsToDivisions.clear ();
  
  // positive powers of 2 of a quarter note
  int bigDivisions = fDivisionsPerQuarterNote;
  
  for (int i = kQuarter; i >= kMaxima; i--) {
    /*
    gLogIOstream <<
      msrDurationAsString (msrDuration (i)) <<
      " -> " <<
      bigDivisions <<
      endl;
    */
    
    fDurationKindsToDivisions.push_front (
      make_pair (
        msrDurationKind (i), bigDivisions));
      
    bigDivisions *= 2;
  } // for

  if (fDivisionsPerQuarterNote > 1) {
    // negative powers of 2 of a quarter note
    int
      smallDivisions =
        fDivisionsPerQuarterNote / 2;
    msrDurationKind
      currentDurationKind =
        kEighth;
    
    while (smallDivisions >= 1) {
      /*
      gLogIOstream <<
        msrDurationAsString (currentDuration) <<
        " % --> " <<
        smallDivisions <<
        endl;
      */

      fDurationKindsToDivisions.push_back (
        make_pair (currentDurationKind, smallDivisions));
        
      currentDurationKind =
        msrDurationKind (
          currentDurationKind + 1);
      smallDivisions /= 2;
    } // while
  }

  // print the durations divisions if needed
  if (gTraceOptions->fTraceDivisions) {
    printDurationKindsDivisions (gLogIOstream);
  }
}

int msrDivisions::durationKindAsDivisions (
  int             inputLineNumber,
  msrDurationKind durationKind)
{
  for (
    list<pair<msrDurationKind, int> >::const_iterator i =
      fDurationKindsToDivisions.begin ();
    i != fDurationKindsToDivisions.end ();
    i++) {
    if ((*i).first == durationKind)
      return
        (*i).second;
  } // for

  stringstream s;

  s <<
    "duration " << durationKind <<
    " cannot be converted to divisions with " <<
    fDivisionsPerQuarterNote << " divisions per quarter note" <<
    endl;

  printDurationKindsDivisions (s);
  
  msrInternalError (
    gXml2lyOptions->fInputSourceName,
    inputLineNumber,
    __FILE__, __LINE__,
    s.str ());

  return -1; // never reached
}

void msrDivisions::printDurationKindsDivisions (ostream& os)
{
  os <<
    "The mapping of durations to divisions with " <<
    fDivisionsPerQuarterNote << " divisions per quarter note" <<
    " is:" <<
    endl;

  gIndenter++;
  
  if (fDurationKindsToDivisions.size ()) {
    list<pair<msrDurationKind, int> >::const_iterator
      iBegin = fDurationKindsToDivisions.begin (),
      iEnd   = fDurationKindsToDivisions.end (),
      i      = iBegin;
          
    for ( ; ; ) {
      os <<
        setw (6) << left <<
        msrDurationKindAsString (
          msrDurationKind ((*i).first)) <<
        ": " <<
        setw (4) << right <<
        (*i).second;

      if (++i == iEnd) break;
      
      os << endl;
    } // for

/* JMI

    for (
      list<pair<msrDuration, int> >::const_iterator i =
        fDurationsToDivisions.begin ();
      i != fDurationsToDivisions.end ();
      i++) {
      os <<
        setw (6) << left <<
        msrDurationAsString (msrDuration((*i).first)) <<
        ": " <<
        setw (4) << right <<
        (*i).second <<
        endl;
    } // for
*/
  }
  
  else
    os <<
      "an empty list";

  os << endl;

  gIndenter--;
}

string msrDivisions::divisionsAsMsrString (
  int  inputLineNumber,
  int  divisions,
  int& numberOfDotsNeeded)
{  
  string result;

  // the result is a base duration, followed by a suffix made of
  // either a sequence of dots or a multiplication factor

  const int fieldWidth = 22;
  
  if (gTraceOptions->fTraceDivisions) {
    gLogIOstream <<
     "--> divisionsAsMsrString ():" <<
      endl <<
      gTab << setw (fieldWidth) <<
      "inputLineNumber" << " = " << inputLineNumber <<
      endl <<
      gTab << setw (fieldWidth) <<
      "divisions" << " = " << divisions <<
      endl;
  }
    
  msrDurationKind baseDurationKind          = k1024th;
  int             baseDurationDivisions = -1;
  
  // search fDurationsToDivisions in longer to shortest order
  list<pair<msrDurationKind, int> >::const_iterator
    iBegin = fDurationKindsToDivisions.begin (),
    iEnd   = fDurationKindsToDivisions.end (),
    i      = iBegin;
    
  for ( ; ; ) {
    if (i == iEnd) {
      stringstream s;

      s <<
        "divisions " << divisions <<
        " could not be handled by divisionsAsMsrString () with:" <<
        endl;

      printDurationKindsDivisions (gLogIOstream);

      msrInternalError (
        gXml2lyOptions->fInputSourceName,
        inputLineNumber,
        __FILE__, __LINE__,
        s.str ());
      break;
    }

    if ((*i).second <= divisions) {
      // found base duration in list
      baseDurationKind      = (*i).first;
      baseDurationDivisions = (*i).second;

      result =
        msrDurationKindAsString (baseDurationKind);
      
      if (gTraceOptions->fTraceDivisions) {
        gLogIOstream <<
            gTab << setw (fieldWidth) <<
          "divisions" << " = " << divisions <<
          endl <<
          endl <<
            gTab << setw (fieldWidth) <<
          "baseDurationDivisions" << " = " << baseDurationDivisions <<
          endl <<
            gTab << setw (fieldWidth) <<
          "result" << " = " << result <<
          endl <<
          endl;
      }

      break;
    }
        
    // next please!
    i++;
  } // for

  int dotsNumber = 0;

  if (divisions > baseDurationDivisions) {
    // divisions is not a power of 2 of a quarter note
    
    // the next element in the list is half as long as (*i)
    int remainingDivisions =
      divisions - baseDurationDivisions;
    int nextDivisionsInList =
      baseDurationDivisions / 2;

    if (gTraceOptions->fTraceDivisions) {
      gLogIOstream <<
        gTab << setw (fieldWidth) <<
        "divisions" << " = " << divisions <<
        endl <<
        gTab << setw (fieldWidth) <<
      "baseDurationDivisions" << " = " << baseDurationDivisions <<
        endl <<
        gTab << setw (fieldWidth) <<
      "nextDivisionsInList" << " = " << nextDivisionsInList <<
        endl <<
        gTab << setw (fieldWidth) <<
      "remainingDivisions" << " = " << remainingDivisions <<
        endl <<
        endl;
    }

    if (remainingDivisions < nextDivisionsInList) {
      // the suffix is a multiplication factor
      rational r (
        divisions,
        baseDurationDivisions);
      r.rationalise ();

      if (gTraceOptions->fTraceDivisions) { // JMI
        gLogIOstream <<
          gTab << setw (fieldWidth) <<
          "divisions" << " = " << divisions <<
          endl <<
          gTab << setw (fieldWidth) <<
          "baseDurationDivisions" << " = " << baseDurationDivisions <<
          endl <<
          gTab << setw (fieldWidth) <<
          "r" << " = " << r <<
          endl <<
          endl;
      }
      
      result +=
        "*" + r.toString ();
    }

    else {
      dotsNumber = 1; // account for next element in the list
      
      while (remainingDivisions > nextDivisionsInList) {
        dotsNumber++;
        remainingDivisions -= nextDivisionsInList;
        nextDivisionsInList /= 2;
  
        if (gTraceOptions->fTraceDivisions) {
          gLogIOstream <<
            gTab << setw (fieldWidth) <<
            "divisions" << " = " << divisions <<
            endl <<
            gTab << setw (fieldWidth) <<
            "baseDurationDivisions" << " = " << baseDurationDivisions <<
            endl <<
            gTab << setw (fieldWidth) <<
            "nextDivisionsInList" << " = " << nextDivisionsInList <<
            endl <<
            gTab << setw (fieldWidth) <<
            "remainingDivisions" << " = " << remainingDivisions <<
            endl <<
            gTab << setw (fieldWidth) <<
            "dotsNumber" << " = " << dotsNumber <<
            endl <<
            endl;
        }
          
        if (dotsNumber > 5 )
          break; // JMI
      } // while
  
      if (gTraceOptions->fTraceDivisions) {
        gLogIOstream <<
          gTab << setw (fieldWidth) <<
          "divisions" << " = " << divisions <<
          endl <<
          gTab << setw (fieldWidth) <<
          "baseDurationDivisions" << " = " << baseDurationDivisions <<
          endl <<
          gTab << setw (fieldWidth) <<
          "nextDivisionsInList" << " = " << nextDivisionsInList <<
          endl <<
          gTab << setw (fieldWidth) <<
          "remainingDivisions" << " = " << remainingDivisions <<
          endl <<
          gTab << setw (fieldWidth) <<
          "dotsNumber" << " = " << dotsNumber <<
          endl <<
          endl;
      }
          
      if (remainingDivisions - nextDivisionsInList == 0) {
        // the suffix is composed of dots
      for (int k = 0; k < dotsNumber; k++)
        result += ".";
      }
    }
  }

  numberOfDotsNeeded = dotsNumber;

  if (gTraceOptions->fTraceDivisions) {
    gLogIOstream <<
      "<-- divisionsAsMsrString (): returns " << result <<
      endl <<
      endl;
  }
  
  return result;
}

/* JMI
string msrDivisions::wholeNotesAsMsrString (
  int      inputLineNumber,
  rational wholeNotes,
  int&     numberOfDotsNeeded)
{  
  string result;

  // the result is a base duration, followed by a suffix made of
  // either a sequence of dots or a multiplication factor

  const int fieldWidth = 22;
  
  if (gTraceOptions->fTraceDivisions) {
    gLogIOstream << left <<
      endl <<
      "--> wholeNotesAsMsrString ():" <<
      endl <<
      gTab << setw (fieldWidth) <<
      "inputLineNumber" << " = " << inputLineNumber <<
      endl <<
      gTab << setw (fieldWidth) <<
      "wholeNotes" << " = " << wholeNotes <<
      endl <<
      endl;
  }

  // are there 0 whole notes?
  if (wholeNotes.getNumerator () == 0)
    return "ZERO wholeNotes";
    
  // compute note's number of divisions
  int divisions =
    wholeNotes.getNumerator ()
      *
    fDivisionsPerQuarterNote * 4 // hence a whole note
      /
    wholeNotes.getDenominator ();      
  
  if (gTraceOptions->fTraceDivisions) {
    gLogIOstream << left <<
      gTab << "divisions" << " = " << divisions <<
      endl;
  }
    
  msrDurationKind baseDurationKind          = k1024th;
  int             baseDurationDivisions = -1;
  
  // search fDurationsToDivisions in longer to shortest order
  list<pair<msrDurationKind, int> >::const_iterator
    iBegin = fDurationKindsToDivisions.begin (),
    iEnd   = fDurationKindsToDivisions.end (),
    i      = iBegin;
    
  for ( ; ; ) {
    if (i == iEnd) {
      stringstream s;

      s <<
        "divisions " << divisions <<
        " could not be handled by wholeNotesAsMsrString () with:" <<
        endl;

      printDurationKindsDivisions (gLogIOstream);

      msrInternalError (
        gXml2lyOptions->fInputSourceName,
        inputLineNumber,
        __FILE__, __LINE__,
        s.str ());
      break;
    }

    if ((*i).second <= divisions) {
      // found base duration in list
      baseDurationKind      = (*i).first;
      baseDurationDivisions = (*i).second;

      result =
        msrDurationKindAsString (baseDurationKind);
      
      if (gTraceOptions->fTraceDivisions) {
        gLogIOstream << left <<
          gTab << setw (fieldWidth) <<
          "divisions" << " = " << divisions <<
          endl <<
          gTab << setw (fieldWidth) <<
          "baseDuration" << " = " <<
          msrDurationKindAsString (baseDurationKind) <<
          endl <<
          gTab << setw (fieldWidth) <<
          "baseDurationDivisions" << " = " <<
          baseDurationDivisions <<
          endl <<
          gTab << setw (fieldWidth) <<
          "result" << " = " << result <<
          endl <<
          endl;
      }
      break;
    }
        
    // next please!
    i++;
  } // for

  int dotsNumber = 0;

  if (divisions > baseDurationDivisions) {
    // divisions is not a power of 2 of a quarter note
    
    // the next element in the list is half as long as (*i)
    int remainingDivisions =
      divisions - baseDurationDivisions;
    int nextDivisionsInList =
      baseDurationDivisions / 2;

    if (gTraceOptions->fTraceDivisions) {
      gLogIOstream << left <<
        gTab << setw (fieldWidth) <<
        "divisions" << " = " << divisions <<
        endl <<
        gTab << setw (fieldWidth) <<
        "baseDurationDivisions" << " = " << baseDurationDivisions <<
        endl <<
        gTab << setw (fieldWidth) <<
        "nextDivisionsInList" << " = " << nextDivisionsInList <<
        endl <<
        gTab << setw (fieldWidth) <<
        "remainingDivisions" << " = " << remainingDivisions <<
        endl <<
        endl;
    }

    if (remainingDivisions < nextDivisionsInList) {
      // the suffix is a multiplication factor
      rational r (
        divisions,
        baseDurationDivisions);
      r.rationalise ();

      if (gTraceOptions->fTraceDivisions) {
        gLogIOstream << left <<
          gTab << setw (fieldWidth) <<
          "divisions" << " = " << divisions <<
          endl <<
          gTab << setw (fieldWidth) <<
          "baseDurationDivisions" << " = " << baseDurationDivisions <<
          endl <<
          gTab << setw (fieldWidth) <<
          "r" << " = " << r <<
          endl <<
        endl;
      }
      
      result +=
        "*" + r.toString ();
    }

    else {
      dotsNumber = 1; // account for next element in the list
      
      while (remainingDivisions > nextDivisionsInList) {
        dotsNumber++;
        remainingDivisions -= nextDivisionsInList;
        nextDivisionsInList /= 2;
  
        if (gTraceOptions->fTraceDivisions) {
          gLogIOstream << left <<
            gTab << setw (fieldWidth) <<
            "divisions" << " = " << divisions <<
            endl <<
            gTab << setw (fieldWidth) <<
            "baseDurationDivisions" << " = " << baseDurationDivisions <<
            endl <<
            gTab << setw (fieldWidth) <<
            "nextDivisionsInList" << " = " << nextDivisionsInList <<
            endl <<
            gTab << setw (fieldWidth) <<
            "remainingDivisions" << " = " << remainingDivisions <<
            endl <<
            gTab << setw (fieldWidth) <<
            "dotsNumber" << " = " << dotsNumber <<
            endl <<
            endl;
        }
          
        if (dotsNumber > 5 )
          break; // JMI
      } // while
  
      if (gTraceOptions->fTraceDivisions) {
        gLogIOstream << left <<
          gTab << setw (fieldWidth) <<
          "divisions" << " = " << divisions <<
          endl <<
          gTab << setw (fieldWidth) <<
          "baseDurationDivisions" << " = " << baseDurationDivisions <<
          endl <<
          gTab << setw (fieldWidth) <<
          "nextDivisionsInList" << " = " << nextDivisionsInList <<
          endl <<
          gTab << setw (fieldWidth) <<
          "remainingDivisions" << " = " << remainingDivisions <<
          endl <<
          gTab << setw (fieldWidth) <<
          "dotsNumber" << " = " << dotsNumber <<
          endl <<
          endl;
      }
          
      if (remainingDivisions - nextDivisionsInList == 0) {
        // the suffix is composed of dots
        for (int k = 0; k < dotsNumber; k++)
          result += ".";
      }
    }
  }

  numberOfDotsNeeded = dotsNumber;

  if (gTraceOptions->fTraceDivisions) {
    gLogIOstream <<
      "<-- wholeNotesAsMsrString (): returns " << result <<
      endl <<
      endl;
  }
  
  return result;
}

string msrDivisions::wholeNotesAsMsrString (
  int      inputLineNumber,
  rational wholeNotes)
{  
  int numberOfDots; // to be ignored

  return
    wholeNotesAsMsrString (
      inputLineNumber,
      wholeNotes,
      numberOfDots);
}
*/

string msrDivisions::divisionsAsMsrString (
  int  inputLineNumber,
  int  divisions)
{
  int numberOfDots; // to be ignored

  return
    divisionsAsMsrString (
      inputLineNumber,
      divisions,
      numberOfDots);
}

string msrDivisions::tupletDivisionsAsMsrString (
  int inputLineNumber,
  int divisions,
  int actualNotes,
  int normalNotes)
{
  return
    divisionsAsMsrString (
      inputLineNumber,
      divisions * actualNotes / normalNotes);
}

string msrDivisions::tupletWholeNotesAsMsrString (
  int      inputLineNumber,
  rational wholeNotes,
  int      actualNotes,
  int      normalNotes)
{
  return
    wholeNotesAsMsrString (
      inputLineNumber,
      wholeNotes
        *
      rational (actualNotes, normalNotes));
}

msrDivisions::~msrDivisions()
{}

void msrDivisions::acceptIn (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrDivisions::acceptIn()" <<
      endl;
  }
      
  if (visitor<S_msrDivisions>*
    p =
      dynamic_cast<visitor<S_msrDivisions>*> (v)) {
        S_msrDivisions elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrDivisions::visitStart()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrDivisions::acceptOut (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrDivisions::acceptOut()" <<
      endl;
  }

  if (visitor<S_msrDivisions>*
    p =
      dynamic_cast<visitor<S_msrDivisions>*> (v)) {
        S_msrDivisions elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrDivisions::visitEnd()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrDivisions::browseData (basevisitor* v)
{}

ostream& operator<< (ostream& os, const S_msrDivisions& elt)
{
  elt->print (os);
  return os;
}

string msrDivisions::divisionsAsString () const
{
  stringstream s;

  s <<
    "Divisions" <<
    ", " << fDivisionsPerQuarterNote <<
    " per quarter note";

  return s.str ();
}

void msrDivisions::print (ostream& os)
{
  os <<
    divisionsAsString () <<
    endl;
}

//______________________________________________________________________________
S_msrBarCheck msrBarCheck::create (
  int    inputLineNumber)
{
  msrBarCheck* o =
    new msrBarCheck (
      inputLineNumber);
  assert(o!=0);
  return o;
}

S_msrBarCheck msrBarCheck::createWithNextBarNumber (
  int    inputLineNumber,
  string nextBarNumber)
{
  msrBarCheck* o =
    new msrBarCheck (
      inputLineNumber, nextBarNumber);
  assert(o!=0);
  return o;
}

msrBarCheck::msrBarCheck (
  int    inputLineNumber)
    : msrElement (inputLineNumber)
{
  if (gTraceOptions->fTraceMeasures) {
    gLogIOstream <<
      "Creating a bar check without next bar number" <<
      endl;
  }
}

msrBarCheck::msrBarCheck (
  int    inputLineNumber,
  string nextBarNumber)
    : msrElement (inputLineNumber)
{
  fNextBarNumber = nextBarNumber; 

  if (gTraceOptions->fTraceMeasures) {
    gLogIOstream <<
      "Creating a bar check with next bar number '" <<
      fNextBarNumber <<
      "'" <<
      endl;
  }
}

msrBarCheck::~msrBarCheck()
{}

void msrBarCheck::setNextBarNumber (string number)
{
  if (gTraceOptions->fTraceMeasures) {
    gLogIOstream <<
      "Setting bar check next bar number to '" <<
      number <<
      "'" <<
      endl;
  }

  fNextBarNumber = number;
}

string msrBarCheck::asString () const
{
  stringstream s;

  s <<
    "BarCheck" <<
    ", next bar number = \"" << fNextBarNumber << "\"" <<
    ", line " << fInputLineNumber;

  return s.str ();
}

void msrBarCheck::acceptIn (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrBarCheck::acceptIn()" <<
      endl;
  }
      
  if (visitor<S_msrBarCheck>*
    p =
      dynamic_cast<visitor<S_msrBarCheck>*> (v)) {
        S_msrBarCheck elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrBarCheck::visitStart()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrBarCheck::acceptOut (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrBarCheck::acceptOut()" <<
      endl;
  }

  if (visitor<S_msrBarCheck>*
    p =
      dynamic_cast<visitor<S_msrBarCheck>*> (v)) {
        S_msrBarCheck elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrBarCheck::visitEnd()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrBarCheck::browseData (basevisitor* v)
{}

ostream& operator<< (ostream& os, const S_msrBarCheck& elt)
{
  elt->print (os);
  return os;
}

void msrBarCheck::print (ostream& os)
{
  os <<
    asString () <<
    endl;
}

//______________________________________________________________________________
S_msrBarNumberCheck msrBarNumberCheck::create (
  int    inputLineNumber,
  string nextBarNumber)
{
  msrBarNumberCheck* o =
    new msrBarNumberCheck (
      inputLineNumber, nextBarNumber);
  assert(o!=0);
  return o;
}

msrBarNumberCheck::msrBarNumberCheck (
  int    inputLineNumber,
  string nextBarNumber)
    : msrElement (inputLineNumber)
{
  fNextBarNumber=nextBarNumber; 
}

msrBarNumberCheck::~msrBarNumberCheck()
{}

string msrBarNumberCheck::asString () const
{
  stringstream s;

  s <<
    "BarNumberCheck" <<
    ", next bar number = \"" << fNextBarNumber << "\"";

  return s.str ();
}

void msrBarNumberCheck::acceptIn (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrBarNumberCheck::acceptIn()" <<
      endl;
  }
      
  if (visitor<S_msrBarNumberCheck>*
    p =
      dynamic_cast<visitor<S_msrBarNumberCheck>*> (v)) {
        S_msrBarNumberCheck elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrBarNumberCheck::visitStart()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrBarNumberCheck::acceptOut (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrBarNumberCheck::acceptOut()" <<
      endl;
  }

  if (visitor<S_msrBarNumberCheck>*
    p =
      dynamic_cast<visitor<S_msrBarNumberCheck>*> (v)) {
        S_msrBarNumberCheck elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrBarNumberCheck::visitEnd()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrBarNumberCheck::browseData (basevisitor* v)
{}

ostream& operator<< (ostream& os, const S_msrBarNumberCheck& elt)
{
  elt->print (os);
  return os;
}

void msrBarNumberCheck::print (ostream& os)
{
  os <<
    asString () <<
    endl;
}

//______________________________________________________________________________
S_msrLineBreak msrLineBreak::create (
  int    inputLineNumber,
  string nextBarNumber)
{
  msrLineBreak* o =
    new msrLineBreak (
      inputLineNumber, nextBarNumber);
  assert(o!=0);
  return o;
}

msrLineBreak::msrLineBreak (
  int    inputLineNumber,
  string nextBarNumber)
    : msrElement (inputLineNumber)
{
  fNextBarNumber = nextBarNumber; 

  if (gTraceOptions->fTraceMeasures) {
    gLogIOstream <<
      "Creating a break before measure " << fNextBarNumber <<
      endl;
  }
}

msrLineBreak::~msrLineBreak()
{}

string msrLineBreak::asString () const
{
  stringstream s;

  s <<
    "Line break" <<
    ", next bar number = \"" << fNextBarNumber << "\"";

  return s.str ();
}

void msrLineBreak::acceptIn (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrLineBreak::acceptIn()" <<
      endl;
  }
      
  if (visitor<S_msrLineBreak>*
    p =
      dynamic_cast<visitor<S_msrLineBreak>*> (v)) {
        S_msrLineBreak elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrLineBreak::visitStart()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrLineBreak::acceptOut (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrLineBreak::acceptOut()" <<
      endl;
  }

  if (visitor<S_msrLineBreak>*
    p =
      dynamic_cast<visitor<S_msrLineBreak>*> (v)) {
        S_msrLineBreak elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrLineBreak::visitEnd()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrLineBreak::browseData (basevisitor* v)
{}

ostream& operator<< (ostream& os, const S_msrLineBreak& elt)
{
  elt->print (os);
  return os;
}

void msrLineBreak::print (ostream& os)
{
  os <<
    asString () <<
    endl;
}

//______________________________________________________________________________
S_msrPageBreak msrPageBreak::create (
  int inputLineNumber)
{
  msrPageBreak* o =
    new msrPageBreak (
      inputLineNumber);
  assert(o!=0);
  return o;
}

msrPageBreak::msrPageBreak (
  int inputLineNumber)
    : msrElement (inputLineNumber)
{
  if (gTraceOptions->fTraceMeasures) {
    gLogIOstream <<
      "Creating a page break" <<
      endl;
  }
}

msrPageBreak::~msrPageBreak()
{}

string msrPageBreak::asString () const
{
  stringstream s;

  s <<
    "Page break";

  return s.str ();
}

void msrPageBreak::acceptIn (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrPageBreak::acceptIn()" <<
      endl;
  }
      
  if (visitor<S_msrPageBreak>*
    p =
      dynamic_cast<visitor<S_msrPageBreak>*> (v)) {
        S_msrPageBreak elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrPageBreak::visitStart()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrPageBreak::acceptOut (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrPageBreak::acceptOut()" <<
      endl;
  }

  if (visitor<S_msrPageBreak>*
    p =
      dynamic_cast<visitor<S_msrPageBreak>*> (v)) {
        S_msrPageBreak elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrPageBreak::visitEnd()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrPageBreak::browseData (basevisitor* v)
{}

ostream& operator<< (ostream& os, const S_msrPageBreak& elt)
{
  elt->print (os);
  return os;
}

void msrPageBreak::print (ostream& os)
{
  os <<
    asString () <<
    endl;
}

//______________________________________________________________________________
S_msrTuplet msrTuplet::create (
  int                     inputLineNumber,
  int                     tupletNumber,
  msrTupletBracketKind    tupletBracketKind,
  msrTupletLineShapeKind  tupletLineShapeKind,
  msrTupletShowNumberKind tupletShowNumberKind,
  msrTupletShowTypeKind   tupletShowTypeKind,
  int                     tupletActualNotes,
  int                     tupletNormalNotes,
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
      tupletActualNotes,
      tupletNormalNotes,
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
  int                     tupletActualNotes,
  int                     tupletNormalNotes,
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

  fTupletActualNotes = tupletActualNotes;
  fTupletNormalNotes = tupletNormalNotes;

  fMemberNotesSoundingWholeNotes = memberNotesSoundingWholeNotes;
  fMemberNotesDisplayWholeNotes  = memberNotesDisplayWholeNotes;
  
  fTupletSoundingWholeNotes = rational (0, 1);
  fTupletDisplayWholeNotes  = rational (0, 1);

  fTupletPositionInMeasure = notePositionInMeasure;
}

msrTuplet::~msrTuplet()
{}

S_msrTuplet msrTuplet::createTupletNewbornClone ()
{
  if (gTraceOptions->fTraceTuplets) {
    gLogIOstream <<
      "Creating a newborn clone of tuplet '" <<
      asString () <<
      "'" <<
      endl;
  }

  S_msrTuplet
    newbornClone =
      msrTuplet::create (
        fInputLineNumber,
        fTupletNumber,
        fTupletBracketKind,
        fTupletLineShapeKind,
        fTupletShowNumberKind,
        fTupletShowTypeKind,
        fTupletActualNotes,
        fTupletNormalNotes,
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
    case msrTuplet::k_NoTupletType:
      result = "none";
      break;
    case msrTuplet::kTupletTypeStart:
      result = "start";
      break;
    case msrTuplet::kTupletTypeContinue:
      result = "continue";
      break;
    case msrTuplet::kTupletTypeStop:
      result = "stop";
      break;
    case msrTuplet::kTupletTypeStartAndStopInARow:
      result = "startAndStopInARow";
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

  fTupletElements.push_back (note);

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
  if (gTraceOptions->fTraceChords || gTraceOptions->fTraceTuplets) {
    gLogIOstream <<
      "Adding chord '" <<
      chord->asString () <<
      "' to tuplet '" <<
      asString () <<
      "'" <<
      endl;
  }

  fTupletElements.push_back (chord);
    
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
  if (gTraceOptions->fTraceTuplets) {
    gLogIOstream <<
      "Adding tuplet '" <<
      tuplet->asString () <<
      "' to tuplet '" <<
      asString () <<
      "'" <<
      endl;
  }

/* JMI
  // unapply containing tuplet factor,
  // i.e 3/2 inside 5/4 becomes 15/8 in MusicXML...
  tuplet->
    unapplySoundingFactorToTupletMembers (
      this->fTupletActualNotes,
      this->fTupletNormalNotes);
  */
  
  // register tuplet in elements list
  fTupletElements.push_back (tuplet);
    
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
  // this will be done in setTupletMeasureNumber()
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
  if (gTraceOptions->fTraceTuplets) {
    gLogIOstream <<
      "Adding tuplet '" <<
      tuplet->asString () <<
      "' to tuplet '" <<
      asString () <<
      "'" <<
      endl;
  }

  // dont' unapply containing tuplet factor,
  // this has been done when building the MSR from MusicXML
  
  // register tuplet in elements list
  fTupletElements.push_back (tuplet);
    
  // account for tuplet duration
  fTupletSoundingWholeNotes +=
    tuplet->getTupletSoundingWholeNotes ();
  fTupletSoundingWholeNotes.rationalise ();

  fTupletDisplayWholeNotes +=
    tuplet->getTupletDisplayWholeNotes ();
  fTupletDisplayWholeNotes.rationalise ();
}

void msrTuplet::removeFirstNoteFromTuplet (
  int       inputLineNumber,
  S_msrNote note)
{
  if (gTraceOptions->fTraceTuplets) {
    gLogIOstream <<
      "Removing first note '" <<
      note->asShortStringWithRawWholeNotes () <<
      "' from tuplet '" <<
      asString () <<
      "'" <<
      endl;
  }

  if (fTupletElements.size ()) {
    S_msrElement
      firstTupletElement =
        fTupletElements.front ();

    for (
      list<S_msrElement>::iterator i=fTupletElements.begin ();
      i!=fTupletElements.end ();
      ++i) {
      if ((*i) == note) {
        // found note, erase it
        fTupletElements.erase (i);
        
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
  }
  
  else {
    stringstream s;

    s <<
      "cannot remove note " <<
      note <<
      " from empty tuplet " <<
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
  }
}

void msrTuplet::setTupletMeasureNumber (string measureNumber) // JMI
{
  fTupletMeasureNumber = measureNumber;

  // propagate measure number to the tuplets elements
  for (
    list<S_msrElement>::const_iterator i = fTupletElements.begin ();
    i != fTupletElements.end ();
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
    list<S_msrElement>::const_iterator i = fTupletElements.begin ();
    i != fTupletElements.end ();
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
  if (gTraceOptions->fTraceTuplets) {
    gLogIOstream <<
      "% ==> applyDisplayFactorToTupletMembers()" <<
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

  for (
    list<S_msrElement>::const_iterator i = fTupletElements.begin ();
    i != fTupletElements.end ();
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
  int containingTupletActualNotes,
  int containingTupletNormalNotes)
{
  if (gTraceOptions->fTraceTuplets) {
    gLogIOstream <<
      "unapplySoundingFactorToTupletMembers()" <<
      endl;

    gIndenter++;
    
    gLogIOstream <<
      "% fTupletActualNotes = " <<
      fTupletActualNotes <<
      ", fTupletNormalNotes = " <<
      fTupletNormalNotes <<
      endl <<
      "% containingTupletActualNotes = " <<
      containingTupletActualNotes <<
      ", containingTupletNormalNotes = " <<
      containingTupletNormalNotes <<
      endl <<
      endl;

    gIndenter--;
  }

  fTupletActualNotes /=
    containingTupletActualNotes;
  fTupletNormalNotes /=
    containingTupletNormalNotes;
}

void msrTuplet::acceptIn (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrTuplet::acceptIn()" <<
      endl;
  }
      
  if (visitor<S_msrTuplet>*
    p =
      dynamic_cast<visitor<S_msrTuplet>*> (v)) {
        S_msrTuplet elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrTuplet::visitStart()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrTuplet::acceptOut (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrTuplet::acceptOut()" <<
      endl;
  }

  if (visitor<S_msrTuplet>*
    p =
      dynamic_cast<visitor<S_msrTuplet>*> (v)) {
        S_msrTuplet elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrTuplet::visitEnd()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrTuplet::browseData (basevisitor* v)
{
  for (
    list<S_msrElement>::const_iterator i = fTupletElements.begin ();
    i != fTupletElements.end ();
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

  if (fTupletElements.size ()) {
    list<S_msrElement>::const_iterator
      iBegin = fTupletElements.begin (),
      iEnd   = fTupletElements.end (),
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

  if (fTupletElements.size ()) {
    list<S_msrElement>::const_iterator
      iBegin = fTupletElements.begin (),
      iEnd   = fTupletElements.end (),
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
    fTupletActualNotes << "/" << fTupletNormalNotes <<
    ", " <<
    singularOrPlural (
      fTupletElements.size (), "element", "elements") <<
    ", whole notes: " <<
    fTupletSoundingWholeNotes << " sound, " <<
    fTupletDisplayWholeNotes << " disp" <<
    ", meas "<<
    fTupletMeasureNumber <<
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

  if (fTupletElements.size ()) {
    gIndenter++;

    list<S_msrElement>::const_iterator
      iBegin = fTupletElements.begin (),
      iEnd   = fTupletElements.end (),
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

//______________________________________________________________________________
S_msrGlissando msrGlissando::create (
  int                  inputLineNumber,
  int                  glissandoNumber,
  msrGlissandoTypeKind glissandoTypeKind,
  msrLineTypeKind      glissandoLineTypeKind)
{
  msrGlissando* o =
    new msrGlissando (
      inputLineNumber,
      glissandoNumber,
      glissandoTypeKind,
      glissandoLineTypeKind);
  assert(o!=0);
  return o;
}

msrGlissando::msrGlissando (
  int                  inputLineNumber,
  int                  glissandoNumber,
  msrGlissandoTypeKind glissandoTypeKind,
  msrLineTypeKind      glissandoLineTypeKind)
    : msrElement (inputLineNumber)
{  
  fGlissandoNumber = glissandoNumber;
  
  fGlissandoTypeKind     = glissandoTypeKind;
  fGlissandoLineTypeKind = glissandoLineTypeKind;
}

msrGlissando::~msrGlissando ()
{}

S_msrGlissando msrGlissando::createGlissandoNewbornClone ()
{
  if (gTraceOptions->fTraceGlissandos) {
    gLogIOstream <<
      "Creating a newborn clone of glissando '" <<
      asString () <<
      "'" <<
      endl;
  }

  S_msrGlissando
    newbornClone =
      msrGlissando::create (
        fInputLineNumber,
        fGlissandoNumber,
        fGlissandoTypeKind,
        fGlissandoLineTypeKind);

  return newbornClone;
}

string msrGlissando::glissandoTypeKindAsString (
  msrGlissandoTypeKind glissandoTypeKind)
{
  string result;

  switch (glissandoTypeKind) {
    case msrGlissando::k_NoGlissandoType:
      result = "none";
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
      "% ==> msrGlissando::acceptIn()" <<
      endl;
  }
      
  if (visitor<S_msrGlissando>*
    p =
      dynamic_cast<visitor<S_msrGlissando>*> (v)) {
        S_msrGlissando elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrGlissando::visitStart()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrGlissando::acceptOut (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrGlissando::acceptOut()" <<
      endl;
  }

  if (visitor<S_msrGlissando>*
    p =
      dynamic_cast<visitor<S_msrGlissando>*> (v)) {
        S_msrGlissando elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrGlissando::visitEnd()" <<
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
    ", number " << fGlissandoNumber <<
    ", " << glissandoTypeKindAsString (
      fGlissandoTypeKind) <<
    ", " << msrLineTypeKindAsString (
      fGlissandoLineTypeKind) <<
    ", line " << fInputLineNumber;
        
  return s.str ();
}

void msrGlissando::print (ostream& os)
{
  os << asString () << endl;
}

//______________________________________________________________________________
S_msrSlide msrSlide::create (
  int              inputLineNumber,
  int              slideNumber,
  msrSlideTypeKind slideTypeKind,
  msrLineTypeKind  slideLineTypeKind)
{
  msrSlide* o =
    new msrSlide (
      inputLineNumber,
      slideNumber,
      slideTypeKind,
      slideLineTypeKind);
  assert(o!=0);
  return o;
}

msrSlide::msrSlide (
  int              inputLineNumber,
  int              slideNumber,
  msrSlideTypeKind slideTypeKind,
  msrLineTypeKind  slideLineTypeKind)
    : msrElement (inputLineNumber)
{  
  fSlideNumber = slideNumber;
  
  fSlideTypeKind     = slideTypeKind;
  fSlideLineTypeKind = slideLineTypeKind;
}

msrSlide::~msrSlide ()
{}

S_msrSlide msrSlide::createSlideNewbornClone ()
{
  if (gTraceOptions->fTraceSlides) {
    gLogIOstream <<
      "Creating a newborn clone of slide '" <<
      asString () <<
      "'" <<
      endl;
  }

  S_msrSlide
    newbornClone =
      msrSlide::create (
        fInputLineNumber,
        fSlideNumber,
        fSlideTypeKind,
        fSlideLineTypeKind);

  return newbornClone;
}

string msrSlide::slideTypeKindAsString (
  msrSlideTypeKind slideTypeKind)
{
  string result;

  switch (slideTypeKind) {
    case msrSlide::k_NoSlideType:
      result = "none";
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
      "% ==> msrSlide::acceptIn()" <<
      endl;
  }
      
  if (visitor<S_msrSlide>*
    p =
      dynamic_cast<visitor<S_msrSlide>*> (v)) {
        S_msrSlide elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrSlide::visitStart()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrSlide::acceptOut (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrSlide::acceptOut()" <<
      endl;
  }

  if (visitor<S_msrSlide>*
    p =
      dynamic_cast<visitor<S_msrSlide>*> (v)) {
        S_msrSlide elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrSlide::visitEnd()" <<
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
    ", number " << fSlideNumber <<
    ", " << slideTypeKindAsString (
      fSlideTypeKind) <<
    ", " << msrLineTypeKindAsString (
      fSlideLineTypeKind) <<
    ", line " << fInputLineNumber;
        
  return s.str ();
}

void msrSlide::print (ostream& os)
{
  os << asString () << endl;
}

//______________________________________________________________________________
S_msrPageGeometry msrPageGeometry::create (
  int inputLineNumber)
{
  msrPageGeometry* o =
    new msrPageGeometry (
      inputLineNumber);
  assert(o!=0);
  return o;
}

msrPageGeometry::msrPageGeometry (
  int inputLineNumber)
    : msrElement (inputLineNumber)
{
  fPaperWidth   = 21.0; // cm
  fPaperHeight  = 29.7; // cm
  
  fTopMargin    = -1.0; // cm
  fBottomMargin = -1.0; // cm
  fLeftMargin   = -1.0; // cm
  fRightMargin  = -1.0; // cm
    
  fBetweenSystemSpace = -1.0; // cm
  fPageTopSpace       = -1.0; // cm

  fMillimeters = -1;
  fTenths      = -1;
}

msrPageGeometry::~msrPageGeometry()
{}

S_msrPageGeometry msrPageGeometry::createGeometryNewbornClone ()
{
  S_msrPageGeometry
    newbornClone =
      msrPageGeometry::create (
        fInputLineNumber);
    
  newbornClone->fPaperWidth =
    fPaperWidth;
  newbornClone->fPaperHeight =
    fPaperHeight;
  
  newbornClone->fTopMargin =
    fTopMargin;
  newbornClone->fBottomMargin =
    fBottomMargin;
  newbornClone->fLeftMargin =
    fLeftMargin;
  newbornClone->fRightMargin =
    fRightMargin;

  newbornClone->fBetweenSystemSpace =
    fBetweenSystemSpace;
  newbornClone->fPageTopSpace =
    fPageTopSpace;

  newbornClone->fMillimeters =
    fMillimeters;
  newbornClone->fTenths =
    fTenths;

  return newbornClone;
}

float msrPageGeometry::globalStaffSize () const
{  
  const float lilyPondDefaultStaffSize = 20.0;
  const float defaultTenthsToMillimetersRatio = 0.175;

  /*
        mm = scaling.get_named_child('millimeters')
        mm = float(mm.get_text())
        tn = scaling.get_maybe_exist_named_child('tenths')
        tn = float(tn.get_text())
        # The variable 'tenths' is actually a ratio, NOT the value of <tenths>.
        # TODO: rename and replace.
        tenths = mm / tn
        ratio = tenths / default_tenths_to_millimeters_ratio
        staff_size = default_staff_size * ratio

        if 1 < staff_size < 100:
            paper.global_staff_size = staff_size
        else:
            msg = "paper.global_staff_size {} is too large, using defaults=20".format(
                staff_size)
            warnings.warn(msg)
            paper.global_staff_size = 20
   */

  float millimetersOverTenths = fMillimeters / fTenths;
  float ratio = millimetersOverTenths / defaultTenthsToMillimetersRatio;
  float staffSize = lilyPondDefaultStaffSize * ratio;

  if (gTraceOptions->fTraceGeometry) {
    gLogIOstream <<
      "globalStaffSize():" <<
      endl;

    gIndenter++;

    gLogIOstream <<
      "lilyPondDefaultStaffSize" << " : " <<
      lilyPondDefaultStaffSize <<
      endl <<
      "defaultTenthsToMillimetersRatio" << " : " <<
      defaultTenthsToMillimetersRatio <<
      endl <<
      "millimetersOverTenths" << " : " <<
      millimetersOverTenths <<
      endl <<
      "ratio" << " : " <<
      ratio <<
      endl <<
      "staffSize" << " : " <<
      staffSize <<
      endl;
    
    gIndenter--;
  }
  
  if (staffSize < 1.0 || staffSize > 100.0) {
    if (gTraceOptions->fTraceGeometry) {
      stringstream s;

      s <<
        "staffSize " << staffSize <<
        " is not between 1.0 and 100.0, replaced by 20.0:" <<
        endl;

      msrMusicXMLWarning (
        gXml2lyOptions->fInputSourceName,
        fInputLineNumber,
        s.str ());
    }

    staffSize = lilyPondDefaultStaffSize;
  }
    
  return staffSize;
  
  /* JMI ???
  if (fMillimeters > 0)
    return fMillimeters * 72.27 / 25.4;
  else
    return 20.0; // LilyPond default
    */
}

void msrPageGeometry::acceptIn (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrPageGeometry::acceptIn()" <<
      endl;
  }
      
  if (visitor<S_msrPageGeometry>*
    p =
      dynamic_cast<visitor<S_msrPageGeometry>*> (v)) {
        S_msrPageGeometry elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrPageGeometry::visitStart()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrPageGeometry::acceptOut (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrPageGeometry::acceptOut()" <<
      endl;
  }

  if (visitor<S_msrPageGeometry>*
    p =
      dynamic_cast<visitor<S_msrPageGeometry>*> (v)) {
        S_msrPageGeometry elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrPageGeometry::visitEnd()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrPageGeometry::browseData (basevisitor* v)
{
  /* JMI
  if (fPaperWidth) {
    // browse identification
    msrBrowser<fPaperWidth> browser (v);
    browser.browse (*fPaperWidth);
  }

  if (fPaperHeight) {
    // browse identification
    msrBrowser<fPaperWidth> browser (v);
    browser.browse (*fPaperHeight);
  }

  if (fTopMargin) {
    // browse identification
    msrBrowser<fPaperWidth> browser (v);
    browser.browse (*fTopMargin);
  }

  if (fBottomMargin) {
    // browse identification
    msrBrowser<fPaperWidth> browser (v);
    browser.browse (*fPaperWidth);
  }

  if (fLeftMargin) {
    // browse identification
    msrBrowser<fPaperWidth> browser (v);
    browser.browse (*fPaperWidth);
  }

  if (fRightMargin) {
    // browse identification
    msrBrowser<fPaperWidth> browser (v);
    browser.browse (*fRightMargin);
  }
  */
}

ostream& operator<< (ostream& os, const S_msrPageGeometry& elt) {
  elt->print (os);
  return os;
}

void msrPageGeometry::print (ostream& os) {
  os <<
    "PageGeometry" <<
    endl;

  bool emptyGeometry = true;
  
  gIndenter++;
  
  const int fieldWidth = 12;

  if (fPaperWidth > 0) {
    os << left <<
      setw (fieldWidth) <<
      "PaperWidth" << " = " <<
      setprecision (2) << fPaperWidth << " cm" <<
      endl;

    emptyGeometry = false;
  }
  
  if (fPaperHeight > 0) {
    os << left <<
      setw (fieldWidth) <<
      "PaperHeight" << " = " <<
      setprecision (2) << fPaperHeight << " cm" <<
      endl;

    emptyGeometry = false;
  }
  
  if (fTopMargin > 0) {
    os << left <<
      setw (fieldWidth) <<
      "TopMargin" << " = " <<
      setprecision (2) << fTopMargin << " cm" <<
      endl;

    emptyGeometry = false;
  }
  
  if (fBottomMargin > 0) {
    os << left <<
      setw (fieldWidth) <<
      "BottomMargin" << " = " <<
      setprecision (2) << fBottomMargin << " cm" <<
      endl;

    emptyGeometry = false;
  }
  
  if (fLeftMargin > 0) {
    os << left <<
      setw (fieldWidth) <<
      "LeftMargin" << " = " <<
      setprecision (2) << fLeftMargin << " cm" <<
      endl;

    emptyGeometry = false;
  }
  
  if (fRightMargin > 0) {
    os << left <<
      setw (fieldWidth) <<
      "RightMargin" << " = " <<
      setprecision (2) << fRightMargin << " cm" <<
      endl;

    emptyGeometry = false;
  }


  if (fMillimeters > 0) {
    os << left <<
      setw (fieldWidth) <<
      "Millimeters" << " = " <<
      setprecision (2) << fMillimeters <<
      endl;

    emptyGeometry = false;
  }

  if (fTenths > 0) {
    os << left <<
      setw (fieldWidth) <<
      "Tenths" << " = " <<
      setprecision (2) << fTenths <<
      endl;

    emptyGeometry = false;
  }

  if (emptyGeometry) {
    os <<
      " " << "nothing specified" <<
      endl <<
      endl;
  }
  else {
    os <<
      endl;
  }
  
  gIndenter--;
}

/*
  if (fBetweenSystemSpace > 0) {
    os <<
      "between-system-space = " <<
      setprecision (2) << fBetweenSystemSpace << "\\cm" << endl;
  }

  if (fPageTopSpace > 0) {
    os <<
      "page-top-space = " <<
      setprecision (2) << fPageTopSpace << "\\cm" << endl;
  }
*/

//______________________________________________________________________________
S_msrCreditWords msrCreditWords::create (
  int         inputLineNumber,
  string      creditWordsContents,
  string      creditWordsFontFamily,
  float       creditWordsFontSize,
  string      creditWordsFontWeight,
  string      creditWordsFontJustify,
  string      creditWordsFontHAlign,
  string      creditWordsFontVAlign,
  string      creditWordsFontXMLLanguage)
{
  msrCreditWords* o =
    new msrCreditWords (
      inputLineNumber,
      creditWordsContents,
      creditWordsFontFamily,
      creditWordsFontSize,
      creditWordsFontWeight,
      creditWordsFontJustify,
      creditWordsFontHAlign,
      creditWordsFontVAlign,
      creditWordsFontXMLLanguage);
  assert(o!=0);
  return o;
}

msrCreditWords::msrCreditWords (
  int         inputLineNumber,
  string      creditWordsContents,
  string      creditWordsFontFamily,
  float       creditWordsFontSize,
  string      creditWordsFontWeight,
  string      creditWordsFontJustify,
  string      creditWordsFontHAlign,
  string      creditWordsFontVAlign,
  string      creditWordsFontXMLLanguage)
    : msrElement (inputLineNumber)
{
  fCreditWordsContents        = creditWordsContents;

  fCreditWordsFontFamily      = creditWordsFontFamily;
  fCreditWordsFontSize        = creditWordsFontSize;
  fCreditWordsFontWeight      = creditWordsFontWeight;
  fCreditWordsFontJustify     = creditWordsFontJustify;
  fCreditWordsFontHAlign      = creditWordsFontHAlign;
  fCreditWordsFontVAlign      = creditWordsFontVAlign;
  fCreditWordsFontXMLLanguage = creditWordsFontXMLLanguage;
}

msrCreditWords::~msrCreditWords()
{}

void msrCreditWords::acceptIn (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrCreditWords::acceptIn()" <<
      endl;
  }
      
  if (visitor<S_msrCreditWords>*
    p =
      dynamic_cast<visitor<S_msrCreditWords>*> (v)) {
        S_msrCreditWords elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrCreditWords::visitStart()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrCreditWords::acceptOut (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrCreditWords::acceptOut()" <<
      endl;
  }

  if (visitor<S_msrCreditWords>*
    p =
      dynamic_cast<visitor<S_msrCreditWords>*> (v)) {
        S_msrCreditWords elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrCreditWords::visitEnd()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrCreditWords::browseData (basevisitor* v)
{}

ostream& operator<< (ostream& os, const S_msrCreditWords& elt)
{
  elt->print (os);
  return os;
}

void msrCreditWords::print (ostream& os)
{
  os <<
    "CreditWords" <<
    endl;

  gIndenter++;

  const int fieldWidth = 26;
  
  os <<
    setw (fieldWidth) <<
    "creditWordsContents" << " = \"" <<
    fCreditWordsContents <<
    "\"" <<
    endl <<
    setw (fieldWidth) <<
    "creditWordsFontFamily" << " = \"" <<
    fCreditWordsFontFamily <<
    "\"" <<
    endl <<
    setw (fieldWidth) <<
    "creditWordsFontSize" << " = " <<
    fCreditWordsFontSize <<
    endl <<
    setw (fieldWidth) <<
    "creditWordsFontWeight" << " = \"" <<
    fCreditWordsFontWeight <<
    "\"" <<
    endl <<
    setw (fieldWidth) <<
    "creditWordsFontJustify" << " = \"" <<
    fCreditWordsFontJustify <<
    "\"" <<
    endl <<
    setw (fieldWidth) <<
    "creditWordsFontHAlign" << " = \"" <<
    fCreditWordsFontHAlign <<
    "\"" <<
    endl <<
    setw (fieldWidth) <<
    "creditWordsFontVAlign" << " = \"" <<
    fCreditWordsFontVAlign <<
    "\"" <<
    endl <<
    setw (fieldWidth) <<
    "creditWordsFontXMLLanguage" << " = \"" <<
    fCreditWordsFontXMLLanguage <<
    "\"" <<
    endl <<
    endl;

  gIndenter--;
}

//______________________________________________________________________________
S_msrCredit msrCredit::create (
  int inputLineNumber,
  int creditPageNumber)
{
  msrCredit* o =
    new msrCredit (
      inputLineNumber, creditPageNumber);
  assert(o!=0);
  return o;
}

msrCredit::msrCredit (
  int inputLineNumber,
  int creditPageNumber)
    : msrElement (inputLineNumber)
{
  fCreditPageNumber = creditPageNumber;
}

msrCredit::~msrCredit()
{}

void msrCredit::appendCreditWordsToCredit (
  S_msrCreditWords creditWords)
{
  fCreditWordsList.push_back (creditWords);
}

void msrCredit::acceptIn (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrCredit::acceptIn()" <<
      endl;
  }
      
  if (visitor<S_msrCredit>*
    p =
      dynamic_cast<visitor<S_msrCredit>*> (v)) {
        S_msrCredit elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrCredit::visitStart()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrCredit::acceptOut (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrCredit::acceptOut()" <<
      endl;
  }

  if (visitor<S_msrCredit>*
    p =
      dynamic_cast<visitor<S_msrCredit>*> (v)) {
        S_msrCredit elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrCredit::visitEnd()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrCredit::browseData (basevisitor* v)
{
  int n1 = fCreditWordsList.size ();
  
  for (int i = 0; i < n1; i++ ) {
    // browse the credit words
    msrBrowser<msrCreditWords> browser (v);
    browser.browse (*fCreditWordsList [i]);
  } // for
}

ostream& operator<< (ostream& os, const S_msrCredit& elt)
{
  elt->print (os);
  return os;
}

string msrCredit::asString () const
{
  stringstream s;

  s <<
    "Credit" <<
    ", fCreditPageNumber" << " = " << fCreditPageNumber << ", ";

  if (fCreditWordsList.size ()) {
    s << "[";
    
    vector<S_msrCreditWords>::const_iterator
      iBegin = fCreditWordsList.begin (),
      iEnd   = fCreditWordsList.end (),
      i      = iBegin;
  
    for ( ; ; ) {
      s << "\"" << (*i)->getCreditWordsContents () << "\"";
      if (++i == iEnd) break;
      s << ", ";
    } // for

    s << "]";
  }
  
  else
    s << "no credit words";
    
  return s.str ();
}

void msrCredit::print (ostream& os)
{
  os <<
    "Credit" <<
    endl;

  gIndenter++;
  
  os <<
    "fCreditPageNumber" << " = " << fCreditPageNumber <<
    endl <<
    endl;

  if (fCreditWordsList.size ()) {
    vector<S_msrCreditWords>::const_iterator
      iBegin = fCreditWordsList.begin (),
      iEnd   = fCreditWordsList.end (),
      i      = iBegin;
  
    for ( ; ; ) {
      os << (*i);
      if (++i == iEnd) break;
  // JMI    os << endl;
    } // for
  }
  
  gIndenter--;
}

//______________________________________________________________________________
S_msrVarValAssoc msrVarValAssoc::create (
  int                inputLineNumber,
  msrVarValAssocKind varValAssocKind,
  string             value)
{
  msrVarValAssoc* o =
    new msrVarValAssoc(
      inputLineNumber,
      varValAssocKind, value);
  assert(o!=0);
  return o;
}

msrVarValAssoc::msrVarValAssoc (
  int                inputLineNumber,
  msrVarValAssocKind varValAssocKind,
  string             value)
    : msrElement (inputLineNumber)
{
  fVarValAssocKind = varValAssocKind;
  fVariableValue   = value;
}

msrVarValAssoc::~msrVarValAssoc()
{}

string msrVarValAssoc::varValAssocKindAsString (
  msrVarValAssocKind varValAssocKind)
{
  string result;

  switch (varValAssocKind) {
    case msrVarValAssoc::kWorkNumber:
      result = "workNumber";
      break;
    case msrVarValAssoc::kWorkTitle:
      result = "workTitle";
      break;
    case msrVarValAssoc::kMovementNumber:
      result = "movementNumber";
      break;
    case msrVarValAssoc::kMovementTitle:
      result = "movementTitle";
      break;
    case msrVarValAssoc::kEncodingDate:
      result = "encodingDate";
      break;
    case msrVarValAssoc::kScoreInstrument:
      result = "scoreInstrument";
      break;
    case msrVarValAssoc::kMiscellaneousField:
      result = "miscellaneousField";
      break;
  } // switch
  
  return result;
}

void msrVarValAssoc::acceptIn (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrVarValAssoc::acceptIn()" <<
      endl;
  }
      
  if (visitor<S_msrVarValAssoc>*
    p =
      dynamic_cast<visitor<S_msrVarValAssoc>*> (v)) {
        S_msrVarValAssoc elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrVarValAssoc::visitStart()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrVarValAssoc::acceptOut (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrVarValAssoc::acceptOut()" <<
      endl;
  }

  if (visitor<S_msrVarValAssoc>*
    p =
      dynamic_cast<visitor<S_msrVarValAssoc>*> (v)) {
        S_msrVarValAssoc elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrVarValAssoc::visitEnd()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrVarValAssoc::browseData (basevisitor* v)
{}

ostream& operator<< (ostream& os, const S_msrVarValAssoc& elt) {
  elt->print (os);
  return os;
}

void msrVarValAssoc::print (ostream& os)
{
  os <<
    "MSR VarValAssoc" <<
    endl;
  
  gIndenter++;

  string variableValue;

  for_each (
    fVariableValue.begin (),
    fVariableValue.end (),
    stringQuoteEscaper (variableValue));

  // print resulting strings
  const int fieldWidth = 16;
  
  os << left <<
    setw (fieldWidth) <<
    "variable kind" << " : " <<
    varValAssocKindAsString () <<
    endl <<
    setw (fieldWidth) <<
    "variable value" << " : " <<
    "\"" << variableValue << "\"" <<
    endl;
  
  gIndenter--;
}

//______________________________________________________________________________
S_msrVarValsListAssoc msrVarValsListAssoc::create (
  int                inputLineNumber,
  msrVarValsListAssocKind varValsListAssocKind)
{
  msrVarValsListAssoc* o =
    new msrVarValsListAssoc(
      inputLineNumber,
      varValsListAssocKind);
  assert(o!=0);
  return o;
}

msrVarValsListAssoc::msrVarValsListAssoc (
  int                inputLineNumber,
  msrVarValsListAssocKind varValsListAssocKind)
    : msrElement (inputLineNumber)
{
  fVarValsListAssocKind = varValsListAssocKind;
}

msrVarValsListAssoc::~msrVarValsListAssoc()
{}

string msrVarValsListAssoc::varValsListAssocKindAsString (
  msrVarValsListAssocKind varValsListAssocKind)
{
  string result;

  switch (varValsListAssocKind) {
    case msrVarValsListAssoc::kRights:
      result = "rights";
      break;
    case msrVarValsListAssoc::kComposer:
      result = "kcomposer";
      break;
    case msrVarValsListAssoc::kArranger:
      result = "arranger";
      break;
    case msrVarValsListAssoc::kLyricist:
      result = "lyricist";
      break;
    case msrVarValsListAssoc::kPoet:
      result = "poet";
      break;
    case msrVarValsListAssoc::kTranslator:
      result = "translator";
      break;
    case msrVarValsListAssoc::kSoftware:
      result = "software";
      break;
  } // switch
  
  return result;
}

string msrVarValsListAssoc::varValsListAssocValuesAsString () const
{
  stringstream s;
  
  list<string>::const_iterator
    iBegin = fVariableValuesList.begin (),
    iEnd   = fVariableValuesList.end (),
    i      = iBegin;

  s << "[";
  if (fVariableValuesList.size ()) {
    for ( ; ; ) {
      s << "\"" << (*i) << "\""; 
      if (++i == iEnd) break;
      s << ", ";
    } // for
  }
  s << "]";
  
  return s.str ();
}

void msrVarValsListAssoc::acceptIn (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrVarValsListAssoc::acceptIn()" <<
      endl;
  }
      
  if (visitor<S_msrVarValsListAssoc>*
    p =
      dynamic_cast<visitor<S_msrVarValsListAssoc>*> (v)) {
        S_msrVarValsListAssoc elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrVarValsListAssoc::visitStart()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrVarValsListAssoc::acceptOut (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrVarValsListAssoc::acceptOut()" <<
      endl;
  }

  if (visitor<S_msrVarValsListAssoc>*
    p =
      dynamic_cast<visitor<S_msrVarValsListAssoc>*> (v)) {
        S_msrVarValsListAssoc elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrVarValsListAssoc::visitEnd()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrVarValsListAssoc::browseData (basevisitor* v)
{}

ostream& operator<< (ostream& os, const S_msrVarValsListAssoc& elt) {
  elt->print (os);
  return os;
}

void msrVarValsListAssoc::print (ostream& os)
{
  os <<
    "MSR VarValsListAssoc" <<
    endl;
  
  gIndenter++;

  const int fieldWidth = 16;

  os << left <<
    setw (fieldWidth) <<
    "assoc kind" << " : " <<
    varValsListAssocKindAsString () <<
    endl <<
    setw (fieldWidth) <<
    
    "variable values" << " : " <<
    endl;
    
  gIndenter++;

  if (fVariableValuesList.size ()) {
    list<string>::const_iterator
      iBegin = fVariableValuesList.begin (),
      iEnd   = fVariableValuesList.end (),
      i      = iBegin;
  
    for ( ; ; ) {
      os << "\"" << (*i) << "\""; 
      if (++i == iEnd) break;
      os << endl;
    } // for
  }
  
  os <<
    endl;
  
  gIndenter--;

  gIndenter--;
}

//______________________________________________________________________________
S_msrLayout msrLayout::create (
  int           inputLineNumber)
{
  msrLayout* o =
    new msrLayout (
      inputLineNumber);
  assert(o!=0);
  return o;
}

msrLayout::msrLayout (
  int           inputLineNumber)
    : msrElement (inputLineNumber)
{}

msrLayout::~msrLayout()
{}

void msrLayout::acceptIn (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrLayout::acceptIn()" <<
      endl;
  }
      
  if (visitor<S_msrLayout>*
    p =
      dynamic_cast<visitor<S_msrLayout>*> (v)) {
        S_msrLayout elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrLayout::visitStart()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrLayout::acceptOut (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrLayout::acceptOut()" <<
      endl;
  }

  if (visitor<S_msrLayout>*
    p =
      dynamic_cast<visitor<S_msrLayout>*> (v)) {
        S_msrLayout elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrLayout::visitEnd()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrLayout::browseData (basevisitor* v)
{
  int n1 = fVarValAssocs.size ();
  
  for (int i = 0; i < n1; i++ ) {
    // browse the stanza
    msrBrowser<msrVarValAssoc> browser (v);
    browser.browse (*fVarValAssocs [i]);
  } // for
}

ostream& operator<< (ostream& os, const S_msrLayout& elt)
{
  elt->print (os);
  return os;
}

void msrLayout::print (ostream& os)
{
  os << "Layout" << endl;

  gIndenter++;

  int n1 = fVarValAssocs.size ();
  
  for (int i = 0; i < n1; i++ ) {
    os << fVarValAssocs [i];
  } // for

    /* JMI
  int n2 = fMsrSchemeVarValAssocs.size ();
  for (int i = 0; i < n2; i++ ) {
    os << fMsrSchemeVarValAssocs[i];
  } // for
  */
  
  gIndenter--;
}

//______________________________________________________________________________
S_msrClef msrClef::create (
  int         inputLineNumber,
  msrClefKind clefKind)
{
  msrClef* o =
    new msrClef (
      inputLineNumber, clefKind);
  assert(o!=0);
  return o;
}

msrClef::msrClef (
  int         inputLineNumber,
  msrClefKind clefKind)
    : msrElement (inputLineNumber)
{
  fClefKind = clefKind;
}

msrClef::~msrClef()
{}

bool msrClef::clefIsATablatureClef () const
{
  switch (fClefKind) {
    case msrClef::kTablature4Clef:
    case msrClef::kTablature5Clef:
    case msrClef::kTablature6Clef:
    case msrClef::kTablature7Clef:
      return true;
      break;
    default:
      return false;
  } // switch
}

bool msrClef::clefIsAPercussionClef () const
{
  switch (fClefKind) {
    case msrClef::kPercussionClef:
      return true;
      break;
    default:
      return false;
  } // switch
}

bool msrClef::isEqualTo (S_msrClef otherClef) const
{
  if (! otherClef)
    return false;
    
  return fClefKind == otherClef->fClefKind;
}

void msrClef::acceptIn (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrClef::acceptIn()" <<
      endl;
  }
      
  if (visitor<S_msrClef>*
    p =
      dynamic_cast<visitor<S_msrClef>*> (v)) {
        S_msrClef elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrClef::visitStart()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrClef::acceptOut (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrClef::acceptOut()" <<
      endl;
  }

  if (visitor<S_msrClef>*
    p =
      dynamic_cast<visitor<S_msrClef>*> (v)) {
        S_msrClef elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrClef::visitEnd()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrClef::browseData (basevisitor* v)
{}

ostream& operator<< (ostream& os, const S_msrClef& elt)
{
  elt->print (os);
  return os;
}

string msrClef::asString () const
{
  stringstream s;

  s <<
    "Clef" " \"";
   
  switch (fClefKind) {
    case msrClef::k_NoClef:
      s << "none";
      break;
    case msrClef::kTrebleClef:
      s << "treble";
      break;
    case msrClef::kSopranoClef:
      s << "soprano";
      break;
    case msrClef::kMezzoSopranoClef:
      s << "mezzo soprano";
      break;
    case msrClef::kAltoClef:
      s << "alto";
      break;
    case msrClef::kTenorClef:
      s << "tenor";
      break;
    case msrClef::kBaritoneClef:
      s << "baritone";
      break;
    case msrClef::kBassClef:
      s << "bass";
      break;
    case msrClef::kTrebleLine1Clef:
      s << "treble line 1";
      break;
    case msrClef::kTrebleMinus15Clef:
      s << "treble -15";
      break;
    case msrClef::kTrebleMinus8Clef:
      s << "treble -8";
      break;
    case msrClef::kTreblePlus8Clef:
      s << "treble +8";
      break;
    case msrClef::kTreblePlus15Clef:
      s << "treble +15";
      break;
    case msrClef::kBassMinus15Clef:
      s << "bass -15";
      break;
    case msrClef::kBassMinus8Clef:
      s << "bass -8";
      break;
    case msrClef::kBassPlus8Clef:
      s << "bass +8";
      break;
    case msrClef::kBassPlus15Clef:
      s << "bass +15";
      break;
    case msrClef::kVarbaritoneClef:
      s << "varbaritone";
      break;
    case msrClef::kTablature4Clef:
      s << "tablature 4 lines";
      break;
    case msrClef::kTablature5Clef:
      s << "tablature 5 lines";
      break;
    case msrClef::kTablature6Clef:
      s << "tablature 6 lines";
      break;
    case msrClef::kTablature7Clef:
      s << "tablature 7 lines";
      break;
    case msrClef::kPercussionClef:
      s << "percussion";
      break;
    case msrClef::kJianpuClef:
      s << "jianpu";
      break;
  } // switch

  s <<
    "\", line " << fInputLineNumber;

  return s.str ();
}

void msrClef::print (ostream& os)
{
  os <<
    asString () <<
    endl;
}

//______________________________________________________________________________
S_msrHumdrumScotKeyItem msrHumdrumScotKeyItem::create (
  int inputLineNumber)
{
  msrHumdrumScotKeyItem* o =
    new msrHumdrumScotKeyItem (
      inputLineNumber);
  assert (o!=0);

  return o;
}

msrHumdrumScotKeyItem::msrHumdrumScotKeyItem (
  int inputLineNumber)
    : msrElement (inputLineNumber)
{
  if (gTraceOptions->fTraceKeys) {
    gLogIOstream <<
      "Creating Humdrum/Scot key item" <<
      ", line = " << inputLineNumber <<
      endl;
  }

  fKeyDiatonicPitchKind = k_NoDiatonicPitch;
  fKeyAlterationKind    = k_NoAlteration;
  fKeyOctave            = -1; // actual MusicXML octaves are non-negative
}

msrHumdrumScotKeyItem::~msrHumdrumScotKeyItem()
{}

bool msrHumdrumScotKeyItem::isEqualTo (
  S_msrHumdrumScotKeyItem
    otherHumdrumScotKeyItem) const
{
  if (! otherHumdrumScotKeyItem)
    return false;
    
  return
    fKeyDiatonicPitchKind == otherHumdrumScotKeyItem->fKeyDiatonicPitchKind
      &&
    fKeyAlterationKind == otherHumdrumScotKeyItem->fKeyAlterationKind
      &&
    fKeyOctave == otherHumdrumScotKeyItem->fKeyOctave;
}

void msrHumdrumScotKeyItem::setKeyItemDiatonicPitchKind (
  msrDiatonicPitchKind diatonicPitchKind)
{
  if (gTraceOptions->fTraceKeys) {
    gLogIOstream <<
      "Setting Humdrum/Scot key item diatonic pitch to '" <<
      msrDiatonicPitchKindAsString (diatonicPitchKind) <<
      "'" <<
      endl;
  }
  
  fKeyDiatonicPitchKind = diatonicPitchKind;
}

void msrHumdrumScotKeyItem::setKeyItemAlterationKind (
  msrAlterationKind alterationKind)
{
  if (gTraceOptions->fTraceKeys) {
    gLogIOstream <<
      "Setting Humdrum/Scot key item alteration to '" <<
      msrAlterationKindAsString (alterationKind) <<
      "'" <<
      endl;
  }
  
  fKeyAlterationKind = alterationKind;
}

void msrHumdrumScotKeyItem::setKeyItemOctave (int keyOctave)
{
  if (gTraceOptions->fTraceKeys) {
    gLogIOstream <<
      "Setting Humdrum/Scot key item octave to '" <<
      keyOctave <<
      "'" <<
      endl;
  }
  
  fKeyOctave = keyOctave;
}

void msrHumdrumScotKeyItem::acceptIn (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrHumdrumScotKeyItem::acceptIn()" <<
      endl;
  }
      
  if (visitor<S_msrHumdrumScotKeyItem>*
    p =
      dynamic_cast<visitor<S_msrHumdrumScotKeyItem>*> (v)) {
        S_msrHumdrumScotKeyItem elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrHumdrumScotKeyItem::visitStart()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrHumdrumScotKeyItem::acceptOut (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrHumdrumScotKeyItem::acceptOut()" <<
      endl;
  }

  if (visitor<S_msrHumdrumScotKeyItem>*
    p =
      dynamic_cast<visitor<S_msrHumdrumScotKeyItem>*> (v)) {
        S_msrHumdrumScotKeyItem elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrHumdrumScotKeyItem::visitEnd()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrHumdrumScotKeyItem::browseData (basevisitor* v)
{}

ostream& operator<< (ostream& os, const S_msrHumdrumScotKeyItem& elt)
{
  elt->print (os);
  return os;
}

string msrHumdrumScotKeyItem::asString () const
{
  stringstream s;

  s <<
    "HumdrumScotKeyItem" <<
    ", KeyDiatonicPitch" << ": " <<
    msrDiatonicPitchKindAsString (fKeyDiatonicPitchKind) <<
    ", KeyAlteration" << ": " <<
    msrAlterationKindAsString (fKeyAlterationKind) <<
    ", KeyOctave" << ": " << fKeyOctave <<
    ", line " << fInputLineNumber;
     
  return s.str ();
}

void msrHumdrumScotKeyItem::print (ostream& os)
{
  os <<
    asString () <<
    endl;
}

//______________________________________________________________________________
S_msrKey msrKey::createTraditional (
  int                      inputLineNumber,
  msrQuarterTonesPitchKind keyTonicQuarterTonesPitchKind,
  msrKeyModeKind           keyModeKind,
  int                      keyCancel)
{
  msrKey* o =
    new msrKey (
      inputLineNumber,
      keyTonicQuarterTonesPitchKind, keyModeKind,
      keyCancel);
  assert (o!=0);

  return o;
}

S_msrKey msrKey::createHumdrumScot (
  int inputLineNumber)
{
  msrKey* o =
    new msrKey (
      inputLineNumber);
  assert (o!=0);

  return o;
}

msrKey::msrKey ( // for traditional keys
  int                      inputLineNumber,
  msrQuarterTonesPitchKind keyTonicQuarterTonesPitchKind,
  msrKeyModeKind           keyModeKind,
  int                      keyCancel)
    : msrElement (inputLineNumber)
{
  // this is a traditional key
  fKeyKind = kTraditionalKind;
  
  fKeyTonicQuarterTonesPitchKind = keyTonicQuarterTonesPitchKind;
  fKeyModeKind                   = keyModeKind;
  
  fKeyCancel = keyCancel;

  // initialization in all cases
  fKeyItemsOctavesAreSpecified = false;
}

msrKey::msrKey ( // for Humdrum/Scot keys
  int                  inputLineNumber)
    : msrElement (inputLineNumber)
{
  // this is a Humdrum/Scot key
  fKeyKind = kHumdrumScotKind;

  // initialization in all cases
  fKeyItemsOctavesAreSpecified = false;
}

msrKey::~msrKey()
{}

bool msrKey::isEqualTo (S_msrKey otherKey) const
{
  if (! otherKey)
    return false;
    
  if (
    ! (
        fKeyKind == otherKey->fKeyKind
          &&
        fKeyTonicQuarterTonesPitchKind
          ==
        otherKey->fKeyTonicQuarterTonesPitchKind
          &&
        fKeyModeKind == otherKey->fKeyModeKind
          &&
        fKeyCancel == otherKey->fKeyCancel
      )
    ) {
    return false;
  }
    
  switch (fKeyKind) {
    case msrKey::kTraditionalKind:
      break;
    case msrKey::kHumdrumScotKind:
      {
        if (
          ! (
              fKeyItemsOctavesAreSpecified
                ==
              otherKey->fKeyItemsOctavesAreSpecified
                  &&
              fHumdrumScotKeyItemsVector.size ()
                ==
              otherKey->fHumdrumScotKeyItemsVector.size ()
            )
          ) {
          return false;
        }

        for (unsigned int i = 0; i < fHumdrumScotKeyItemsVector.size (); i++) {
          if (
            ! (
              fHumdrumScotKeyItemsVector [i]->isEqualTo (
                otherKey->fHumdrumScotKeyItemsVector [i])
              )
            ) {
            return false;
          }
        } // for
      }
      break;
  } // switch

  return true;
}

msrClef::msrClefKind msrClef::clefKindFromString (
  string theString)
{
  msrClef::msrClefKind result = msrClef::k_NoClef;

  if      (theString == "treble")
    result = msrClef::kTrebleClef;
  else if (theString == "bass")
    result = msrClef::kBassClef;
  else if (theString == "alto")
    result = msrClef::kAltoClef;
  else if (theString == "tenor")
    result = msrClef::kTenorClef;
  else if (theString == "soprano")
    result = msrClef::kSopranoClef;
  else if (theString == "mezzosoprano")
    result = msrClef::kMezzoSopranoClef;
  else if (theString == "baritone")
    result = msrClef::kBaritoneClef;
  else if (theString == "treble^8")
    result = msrClef::kTreblePlus8Clef;
  else if (theString == "treble_8")
    result = msrClef::kTrebleMinus8Clef;
  else if (theString == "bass_8")
    result = msrClef::kBassMinus8Clef;
  else if (theString == "bass^8")
    result = msrClef::kBassPlus8Clef;
  else if (theString == "varbaritone")
    result = msrClef::kVarbaritoneClef;
  else if (theString == "treble^15")
    result = msrClef::kTreblePlus15Clef;
  else if (theString == "treble_15")
    result = msrClef::kTrebleMinus15Clef;
  else if (theString == "bass^15")
    result = msrClef::kBassPlus15Clef;
  else if (theString == "bass_15")
    result = msrClef::kBassMinus15Clef;
  else if (theString == "tab4")
    result = msrClef::kTablature4Clef;
  else if (theString == "tab5")
    result = msrClef::kTablature5Clef;
  else if (theString == "tab6")
    result = msrClef::kTablature6Clef;
  else if (theString == "tab7")
    result = msrClef::kTablature7Clef;
  else if (theString == "percussion")
    result = msrClef::kPercussionClef;
  else if (theString == "jianpu")
    result = msrClef::kJianpuClef;

  return result;
}

void msrKey::appendHumdrumScotKeyItem (
  S_msrHumdrumScotKeyItem item)
{
  if (gTraceOptions->fTraceKeys) {
    gLogIOstream <<
      "Append item '" <<
      item->asString () <<
      "' to key '" <<
      "'" <<
      endl;
    }

  // have key items octaves been specified?
  if (item->getKeyItemOctave () >= 0)
    fKeyItemsOctavesAreSpecified = true;

  // append the item to the vector
  fHumdrumScotKeyItemsVector.insert (
    fHumdrumScotKeyItemsVector.end (), item);
}

void msrKey::acceptIn (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrKey::acceptIn()" <<
      endl;
  }
      
  if (visitor<S_msrKey>*
    p =
      dynamic_cast<visitor<S_msrKey>*> (v)) {
        S_msrKey elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrKey::visitStart()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrKey::acceptOut (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrKey::acceptOut()" <<
      endl;
  }

  if (visitor<S_msrKey>*
    p =
      dynamic_cast<visitor<S_msrKey>*> (v)) {
        S_msrKey elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrKey::visitEnd()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrKey::browseData (basevisitor* v)
{}

ostream& operator<< (ostream& os, const S_msrKey& elt)
{
  elt->print (os);
  return os;
}

string msrKey::keyKindAsString (
  msrKeyKind keyKind)
{
  string result;
  
  switch (keyKind) {
    case msrKey::kTraditionalKind:
      result = "traditional";
      break;
    case msrKey::kHumdrumScotKind:
      result = "Humdrum/Scot";
      break;
  } // switch

  return result;
}

string msrKey::keyModeKindAsString (
  msrKeyModeKind keyModeKind)
{
  string result;
  
  switch (keyModeKind) {
    case msrKey::kMajorMode:
      result = "major";
      break;
    case msrKey::kMinorMode:
      result = "minor";
      break;
    case msrKey::kIonianMode:
      result = "ionian";
      break;
    case msrKey::kDorianMode:
      result = "dorian";
      break;
    case msrKey::kPhrygianMode:
      result = "phrygian";
      break;
    case msrKey::kLydianMode:
      result = "lydian";
      break;
    case msrKey::kMixolydianMode:
      result = "mixolydian";
      break;
    case msrKey::kAeolianMode:
      result = "aeolian";
      break;
    case msrKey::kLocrianMode:
      result = "locrian";
      break;
  } // switch

  return result;
}

string msrKey::asString () const
{
  stringstream s;

  s <<
    "Key" <<
    ", " << keyKindAsString (fKeyKind) <<
    ", ";

  switch (fKeyKind) {
    case msrKey::kTraditionalKind:
      s <<
        msrQuarterTonesPitchKindAsString (
          gMsrOptions->fMsrQuarterTonesPitchesLanguageKind,
          fKeyTonicQuarterTonesPitchKind) <<
        " " <<
        keyModeKindAsString (fKeyModeKind);
      break;
      
    case msrKey::kHumdrumScotKind:
      s <<
        fHumdrumScotKeyItemsVector.size () << "items" <<
        ", keyItemsOctavesAreSpecified: " <<
        booleanAsString (
          fKeyItemsOctavesAreSpecified);
      break;
  } // switch

  s <<
    ", line " << fInputLineNumber;

  return s.str ();
}

void msrKey::print (ostream& os)
{
  os <<
    "Key" <<
    ", " << keyKindAsString (fKeyKind) <<
    ",";

  switch (fKeyKind) {
    case msrKey::kTraditionalKind:
      os <<
        " " <<
        msrQuarterTonesPitchKindAsString (
          gMsrOptions->fMsrQuarterTonesPitchesLanguageKind,
          fKeyTonicQuarterTonesPitchKind) <<
        " " <<
        keyModeKindAsString (fKeyModeKind) <<
        ", line " << fInputLineNumber <<
        endl;
      break;
      
    case msrKey::kHumdrumScotKind:
      os <<
        ", keyItemsOctavesAreSpecified: " <<
        booleanAsString (
          fKeyItemsOctavesAreSpecified) <<
        ", " <<
        fHumdrumScotKeyItemsVector.size () <<
        " items" <<
        ", line " << fInputLineNumber <<
        endl;

      if (fHumdrumScotKeyItemsVector.size ()) {
        os <<
          endl;
          
        gIndenter++;
        
        vector<S_msrHumdrumScotKeyItem>::const_iterator
          iBegin = fHumdrumScotKeyItemsVector.begin (),
          iEnd   = fHumdrumScotKeyItemsVector.end (),
          i      = iBegin;
          
        for ( ; ; ) {
          os << (*i);
          if (++i == iEnd) break;
    // JMI     os << endl;
        } // for
        
        gIndenter--;
      }
      
      else
        {
          os <<
            " none";
        }
      break;
  } // switch
}

//______________________________________________________________________________
S_msrTimeItem msrTimeItem::create (
  int inputLineNumber)
{
  msrTimeItem* o =
    new msrTimeItem (
      inputLineNumber);
  assert (o!=0);

  return o;
}

msrTimeItem::msrTimeItem (
  int inputLineNumber)
    : msrElement (inputLineNumber)
{
  fTimeBeatValue = -1;
  
  if (gTraceOptions->fTraceTimes) {
    gLogIOstream <<
      "Creating time item" <<
      ", line = " << inputLineNumber <<
      endl;
  }
}

msrTimeItem::~msrTimeItem()
{}

bool msrTimeItem::isEqualTo (S_msrTimeItem otherTimeItem) const
{  
  if (! otherTimeItem)
    return false;
    
  if (
    ! (
        fTimeBeatValue == otherTimeItem->fTimeBeatValue
            &&
        fTimeBeatsNumbersVector.size ()
          ==
        otherTimeItem->fTimeBeatsNumbersVector.size ()
      )
    ) {
    return false;
  }
    
  for (unsigned int i = 0; i < fTimeBeatsNumbersVector.size (); i++) {
    if (
      ! (
        fTimeBeatsNumbersVector [i]
          ==
        otherTimeItem->fTimeBeatsNumbersVector [i]
        )
      ) {
      return false;
    }
  } // for
 
  return true;
}

void msrTimeItem::appendBeatsNumber (int beatsNumber)
{
  if (gTraceOptions->fTraceTimes) {
    gLogIOstream <<
      "Append beat number '" <<
      beatsNumber <<
      "' to time '" << // JMI
      "'" <<
      endl;
    }

  fTimeBeatsNumbersVector.insert (
    fTimeBeatsNumbersVector.end (),
    beatsNumber);
}

int msrTimeItem::getTimeBeatsNumber () const
{
  int result = 0;
  
  for (unsigned int i = 0; i < fTimeBeatsNumbersVector.size (); i++) {
    result +=
      fTimeBeatsNumbersVector [i];
    } // for

  return result;
}

void msrTimeItem::acceptIn (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrTimeItem::acceptIn()" <<
      endl;
  }
      
  if (visitor<S_msrTimeItem>*
    p =
      dynamic_cast<visitor<S_msrTimeItem>*> (v)) {
        S_msrTimeItem elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrTimeItem::visitStart()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrTimeItem::acceptOut (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrTimeItem::acceptOut()" <<
      endl;
  }

  if (visitor<S_msrTimeItem>*
    p =
      dynamic_cast<visitor<S_msrTimeItem>*> (v)) {
        S_msrTimeItem elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrTimeItem::visitEnd()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrTimeItem::browseData (basevisitor* v)
{}

ostream& operator<< (ostream& os, const S_msrTimeItem& elt)
{
  elt->print (os);
  return os;
}

string msrTimeItem::asString () const
{
  stringstream s;

  s <<
    "TimeItem ";

  int vectorSize =
    fTimeBeatsNumbersVector.size ();

  switch (vectorSize) {
    case 0:
      msrInternalError (
        gXml2lyOptions->fInputSourceName,
        fInputLineNumber,
        __FILE__, __LINE__,
        "time item beats numbers vector is empty");
      break;
      
    case 1:
      s <<
        fTimeBeatsNumbersVector [0] << "/" << fTimeBeatValue;
      break;
      
    default:
      s <<
        "beats numbers: ";

      for (int i = 0; i < vectorSize; i++) {
        s <<
          fTimeBeatsNumbersVector [i];
  
        if (i != vectorSize - 1)
          s <<
            " ";
        } // for

      s <<
        ", beat value: " << fTimeBeatValue;
  } // switch
  
  s <<
    ", line " << fInputLineNumber;
     
  return s.str ();
}

void msrTimeItem::print (ostream& os)
{
  os <<
    asString () <<
    endl;
}

//______________________________________________________________________________
S_msrTime msrTime::create (
  int               inputLineNumber,
  msrTimeSymbolKind timeSymbolKind)
{
  msrTime* o =
    new msrTime (
      inputLineNumber, timeSymbolKind);
  assert (o!=0);
  return o;
}

msrTime::msrTime (
  int               inputLineNumber,
  msrTimeSymbolKind timeSymbolKind)
    : msrElement (inputLineNumber)
{
  fTimeSymbolKind = timeSymbolKind;
    
  fTimeIsCompound = false;
}

bool msrTime::isEqualTo (S_msrTime otherTime) const
{
  if (! otherTime)
    return false;
    
  if (
    ! (
        fTimeSymbolKind == otherTime->fTimeSymbolKind
          &&
        fTimeIsCompound == otherTime->fTimeIsCompound
          &&
        fTimeItemsVector.size () == otherTime->fTimeItemsVector.size ()
      )
    ) {
    return false;
  }
    
  for (unsigned int i = 0; i < fTimeItemsVector.size (); i++) {
    if (
      ! (
        fTimeItemsVector [i]->isEqualTo (
          otherTime->fTimeItemsVector [i])
        )
      ) {
      return false;
    }
  } // for
 
  return true;
}

S_msrTime msrTime::createFourQuartersTime (
  int inputLineNumber)
{
  // create the time
  S_msrTime
    time =
      msrTime::create (
        inputLineNumber,
        msrTime::k_NoTimeSymbol);

  // create a four quarters time item
  S_msrTimeItem
    timeItem =
      msrTimeItem::create (
        inputLineNumber);

  timeItem->
    appendBeatsNumber (4);
  timeItem->
    setTimeBeatValue (4);
        
  // append the time item to the time
  time->
    appendTimeItem (timeItem);

  // return the time
  return time;
}

msrTime::~msrTime()
{}

void msrTime::appendTimeItem (
  S_msrTimeItem timeItem)
{
  if (gTraceOptions->fTraceTimes) {
    gLogIOstream <<
      "Append item '" <<
      timeItem->asString () <<
      "' to time '" <<
      "JMI" <<
      "'" <<
      endl;
    }

  if (! fTimeItemsVector.size ()) {
    // this is the first item inserted
    if (
      timeItem->
        getTimeBeatsNumbersVector (). size ()
        >
      1) {
      // this time is compound
      fTimeIsCompound = true;
    }
  }
  
  else {
    // there are several time items, this time is compound
    fTimeIsCompound = true;
  }

  // append the time item to the vector
  fTimeItemsVector.insert (
    fTimeItemsVector.end (), timeItem);
}

rational msrTime::wholeNotesPerMeasure () const
{
  rational result (0, 1); // addition neutral element

  int vectorSize = fTimeItemsVector.size ();
  
  if (vectorSize) {
    /* JMI
    // start with first item
    result =
      rational (
        fTimeItemsVector [0]->getTimeBeatsNumber (),
        fTimeItemsVector [0]->getTimeBeatValue ());
*/

/* JMI
    gLogIOstream <<
      endl <<
      endl <<
      "result1 = " <<
      result.getNumerator () <<
      "/" <<
      result.getDenominator () <<
      endl <<
      endl;
*/

    // iterate over the others
    for (int i = 0; i < vectorSize; i++) {
      result +=
        rational (
          fTimeItemsVector [i]->getTimeBeatsNumber (),
          fTimeItemsVector [i]->getTimeBeatValue ());

/* JMI
      gLogIOstream <<
        endl <<
        endl <<
        "result2 = " <<
        result.getNumerator () <<
        "/" <<
        result.getDenominator () <<
        endl <<
        endl;
        */
        
    } // for
  }

  else {
    msrInternalError (
      gXml2lyOptions->fInputSourceName,
      fInputLineNumber,
      __FILE__, __LINE__,
      "time items vector is empty");
  }

  // rationalize result
  result.rationalise ();

  // return it
  return result;
}

void msrTime::acceptIn (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrTime::acceptIn()" <<
      endl;
  }
      
  if (visitor<S_msrTime>*
    p =
      dynamic_cast<visitor<S_msrTime>*> (v)) {
        S_msrTime elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrTime::visitStart()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrTime::acceptOut (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrTime::acceptOut()" <<
      endl;
  }

  if (visitor<S_msrTime>*
    p =
      dynamic_cast<visitor<S_msrTime>*> (v)) {
        S_msrTime elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrTime::visitEnd()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrTime::browseData (basevisitor* v)
{}

string msrTime::timeSymbolKindAsString (
  msrTimeSymbolKind timeSymbolKind)
{
  string result;
 
  switch (timeSymbolKind) {
    case msrTime::kTimeSymbolCommon:
      result = "common";
      break;
    case msrTime::kTimeSymbolCut:
      result = "cut";
      break;
    case msrTime::kTimeSymbolNote:
      result = "note";
      break;
    case msrTime::kTimeSymbolDottedNote:
      result = "dotted note";
      break;
    case msrTime::kTimeSymbolSingleNumber:
      result = "single number";
      break;
    case msrTime::kTimeSymbolSenzaMisura:
      result = "senza misura";
      break;
    case msrTime::k_NoTimeSymbol:
      result = "none";
      break;
  } // switch

  return result;
}

string msrTime::timeSeparatorKindAsString (
  msrTimeSeparatorKind timeSeparatorKind)
{
  string result;
 
  switch (timeSeparatorKind) {
    case msrTime::k_NoTimeSeparator:
      result = "none";
      break;
    case msrTime::kTimeSeparatorHorizontal:
      result = "horizontal";
      break;
    case msrTime::kTimeSeparatorDiagonal:
      result = "diagonal";
      break;
    case msrTime::kTimeSeparatorVertical:
      result = "vertical";
      break;
    case msrTime::kTimeSeparatorAdjacent:
      result = "adjacent";
      break;
  } // switch

  return result;
}

string msrTime::timeRelationKindAsString (
  msrTimeRelationKind timeRelationKind)
{
  string result;
 
  switch (timeRelationKind) {
    case msrTime::k_NoTimeRelation:
      result = "none";
      break;
    case msrTime::kTimeRelationParentheses:
      result = "parentheses";
      break;
    case msrTime::kTimeRelationBracket:
      result = "bracket";
      break;
    case msrTime::kTimeRelationEquals:
      result = "equals";
      break;
    case msrTime::kTimeRelationSlash:
      result = "slash";
      break;
    case msrTime::kTimeRelationSpace:
      result = "space";
      break;
    case msrTime::kTimeRelationHyphen:
      result = "hyphen";
      break;
  } // switch

  return result;
}

string msrTime::asShortString () const
{
  /* JMI
  stringstream s;

  s <<
    "Time, " <<
    ", timeSymbolKind: " <<
    timeSymbolKindAsString (fTimeSymbolKind) <<
    ", compound " <<
    booleanAsString (
      fTimeIsCompound) <<
    ", " <<
    singularOrPlural (
      fTimeItemsVector.size (), "item", "items") <<
    ", line " << fInputLineNumber;

  return s.str ();
  */

  return asString ();
}

string msrTime::asString () const
{
  stringstream s;

  s <<
    "Time, " << 
    ", timeSymbolKind: " <<
    timeSymbolKindAsString (fTimeSymbolKind) <<
    ", compound: " <<
    booleanAsString (
      fTimeIsCompound) <<
    ", " <<
    singularOrPlural (
      fTimeItemsVector.size (), "item", "items") <<
    ", line " << fInputLineNumber;

  if (fTimeItemsVector.size ()) {
    s <<
      ", ";

    vector<S_msrTimeItem>::const_iterator
      iBegin = fTimeItemsVector.begin (),
      iEnd   = fTimeItemsVector.end (),
      i      = iBegin;
  
    for ( ; ; ) {
      s << (*i);
      if (++i == iEnd) break;
      s << ", ";
    } // for
  }
  else {
    if (fTimeSymbolKind != msrTime::kTimeSymbolSenzaMisura) {
      msrInternalError (
        gXml2lyOptions->fInputSourceName,
        fInputLineNumber,
        __FILE__, __LINE__,
        "time  items vector is empty");
    }
  }

  return s.str ();
}

ostream& operator<< (ostream& os, const S_msrTime& elt)
{
  elt->print (os);
  return os;
}

void msrTime::print (ostream& os)
{
  os <<
    "Time" <<
    ", timeSymbolKind: " <<
    timeSymbolKindAsString (fTimeSymbolKind) <<
    ", compound: " <<
    booleanAsString (
      fTimeIsCompound) <<
    ", " <<
    singularOrPlural (
      fTimeItemsVector.size (), "item", "items") <<
    ", line " << fInputLineNumber <<
    ":";

  if (fTimeItemsVector.size ()) {
    os <<
      endl;
      
    gIndenter++;
    
    vector<S_msrTimeItem>::const_iterator
      iBegin = fTimeItemsVector.begin (),
      iEnd   = fTimeItemsVector.end (),
      i      = iBegin;
      
    for ( ; ; ) {
      os << (*i);
      if (++i == iEnd) break;
 // JMI     os << endl;
    } // for
    
    gIndenter--;
  }
  
  else
    {
      os <<
        " none";
    }
}

//______________________________________________________________________________
S_msrTranspose msrTranspose::create (
  int  inputLineNumber,
  int  transposeDiatonic,
  int  transposeChromatic,
  int  transposeOctaveChange,
  bool transposeDouble)
{
  msrTranspose* o =
    new msrTranspose (
      inputLineNumber,
      transposeDiatonic,
      transposeChromatic,
      transposeOctaveChange,
      transposeDouble);
  assert (o!=0);
  return o;
}

msrTranspose::msrTranspose (
  int  inputLineNumber,
  int  transposeDiatonic,
  int  transposeChromatic,
  int  transposeOctaveChange,
  bool transposeDouble)
    : msrElement (inputLineNumber)
{
  fTransposeDiatonic     = transposeDiatonic;
  fTransposeChromatic    = transposeChromatic;
  fTransposeOctaveChange = transposeOctaveChange;
  fTransposeDouble       = transposeDouble;

  if (gTraceOptions->fTraceTranspositions) {
    gLogIOstream <<
      "Creating transpose '" <<
      asString () <<
      "'" <<
      endl;
  }
}

msrTranspose::~msrTranspose()
{}

bool msrTranspose::isEqualTo (S_msrTranspose otherTranspose) const
{
  if (! otherTranspose)
    return false;
    
  return
    fTransposeDiatonic ==
      otherTranspose->fTransposeDiatonic
      &&
    fTransposeChromatic ==
      otherTranspose->fTransposeChromatic
      &&
    fTransposeOctaveChange ==
      otherTranspose->fTransposeOctaveChange
      &&
    fTransposeDouble ==
      otherTranspose->fTransposeDouble;
}

void msrTranspose::acceptIn (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrTranspose::acceptIn()" <<
      endl;
  }
      
  if (visitor<S_msrTranspose>*
    p =
      dynamic_cast<visitor<S_msrTranspose>*> (v)) {
        S_msrTranspose elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrTranspose::visitStart()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrTranspose::acceptOut (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrTranspose::acceptOut()" <<
      endl;
  }

  if (visitor<S_msrTranspose>*
    p =
      dynamic_cast<visitor<S_msrTranspose>*> (v)) {
        S_msrTranspose elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrTranspose::visitEnd()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrTranspose::browseData (basevisitor* v)
{}

ostream& operator<< (ostream& os, const S_msrTranspose& elt)
{
  elt->print (os);
  return os;
}

string msrTranspose::asString () const
{
  stringstream s;

  s <<
    "Transpose" << 
    ", diatonic = " << fTransposeDiatonic <<
    ", chromatic = " << fTransposeChromatic <<
    ", transposeOctaveChange = " << fTransposeOctaveChange <<
    ", transposeDouble = " << fTransposeDouble <<
    ", line " << fInputLineNumber;

  return s.str ();
}

void msrTranspose::print (ostream& os)
{
  const int fieldWidth = 22;
  
  os <<
    "Transpose" <<
    ", line " << fInputLineNumber <<
    endl;

  gIndenter++;
  
  os << left <<
    setw (fieldWidth) <<
    "Diatonic" << " = " << fTransposeDiatonic <<
    endl <<
    setw (fieldWidth) <<
    "Chromatic" << " = " << fTransposeChromatic <<
    endl <<
    setw (fieldWidth) <<
    "TransposeOctaveChange" << " = " << fTransposeOctaveChange <<
    endl <<
    setw (fieldWidth) <<
    "TransposeDouble" << " = " << fTransposeDouble <<
    endl <<
    endl;
    
  gIndenter--;
}

//______________________________________________________________________________
S_msrWords msrWords::create (
  int                      inputLineNumber,
  msrPlacementKind         wordsPlacementKind,
  string                   wordsContents,
  msrJustifyKind           wordsJustifyKind,
  msrVerticalAlignmentKind wordsVerticalAlignmentKind,
  msrFontStyleKind         wordsFontStyleKind,
  S_msrFontSize            wordsFontSize,
  msrFontWeightKind        wordsFontWeightKind,
  msrWordsXMLLangKind      wordsXMLLangKind)
{
  msrWords* o =
    new msrWords (
      inputLineNumber,
      wordsPlacementKind,
      wordsContents,
      wordsJustifyKind,
      wordsVerticalAlignmentKind,
      wordsFontStyleKind,
      wordsFontSize,
      wordsFontWeightKind,
      wordsXMLLangKind);
  assert(o!=0);
  return o;
}

msrWords::msrWords (
  int                      inputLineNumber,
  msrPlacementKind         wordsPlacementKind,
  string                   wordsContents,
  msrJustifyKind           wordsJustifyKind,
  msrVerticalAlignmentKind wordsVerticalAlignmentKind,
  msrFontStyleKind         wordsFontStyleKind,
  S_msrFontSize            wordsFontSize,
  msrFontWeightKind        wordsFontWeightKind,
  msrWordsXMLLangKind      wordsXMLLangKind)
    : msrElement (inputLineNumber)
{
  fWordsPlacementKind  = wordsPlacementKind;
  
  fWordsContents       = wordsContents;
  
  fWordsJustifyKind    = wordsJustifyKind;
  fWordsVerticalAlignmentKind = wordsVerticalAlignmentKind;

  fWordsFontStyleKind  = wordsFontStyleKind;
  fWordsFontSize       = wordsFontSize;
  fWordsFontWeightKind = wordsFontWeightKind;
  
  fWordsXMLLangKind    = wordsXMLLangKind;
}

msrWords::~msrWords()
{}

void msrWords::acceptIn (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrWords::acceptIn()" <<
      endl;
  }
      
  if (visitor<S_msrWords>*
    p =
      dynamic_cast<visitor<S_msrWords>*> (v)) {
        S_msrWords elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrWords::visitStart()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrWords::acceptOut (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrWords::acceptOut()" <<
      endl;
  }

  if (visitor<S_msrWords>*
    p =
      dynamic_cast<visitor<S_msrWords>*> (v)) {
        S_msrWords elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrWords::visitEnd()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrWords::browseData (basevisitor* v)
{}

ostream& operator<< (ostream& os, const S_msrWords& elt)
{
  elt->print (os);
  return os;
}

string msrWords::wordsPlacementKindAsString () const
{
  return
    msrPlacementKindAsString (
      fWordsPlacementKind);
}

string msrWords::wordsJustifyKindAsString () const
{
  return msrJustifyKindAsString (fWordsJustifyKind);
}

string msrWords::wordsVerticalAlignmentKindAsString () const
{
  return
    msrVerticalAlignmentKindAsString (
      fWordsVerticalAlignmentKind);
}

string msrWords::wordsFontStyleKindAsString () const
{
  return msrFontStyleKindAsString (fWordsFontStyleKind);
}

string msrWords::wordsFontSizeAsString () const
{
  return fWordsFontSize->fontSizeAsString ();
}

string msrWords::wordsFontWeightKindAsString () const
{
  return msrFontWeightKindAsString (fWordsFontWeightKind);
}

string msrWords::msrWordsXMLLangKindAsString (
  msrWordsXMLLangKind wordsXMLLangKind)
{
  string result;

  switch (wordsXMLLangKind) {
    case kItLang:
      result = "it";
      break;
    case kEnLang:
      result = "en";
      break;
    case kDeLang:
      result = "de";
      break;
    case kFrLang:
      result = "fr";
    case kJaLang:
      result = "ja";
      break;
    case kLaLang:
      result = "la";
      break;
  } // switch

  return result;
}

string msrWords::asString () const
{
  stringstream s;

  s <<
    "Words" << " " <<
    fWordsContents << ", placement = " <<
    msrPlacementKindAsString (fWordsPlacementKind);

  return s.str ();
}

void msrWords::print (ostream& os)
{
// JMI  os << asString () << endl;

  os <<
    "Words" <<
    endl;

  gIndenter++;

  const int fieldWidth = 27;

  os << left <<
    setw (fieldWidth) <<
    "wordsContents" << " = \"" << fWordsContents << "\"" <<
    endl <<
    setw (fieldWidth) <<
    "placement" << " = " <<
    msrPlacementKindAsString (fWordsPlacementKind) <<
    endl <<
    setw (fieldWidth) <<
    "wordsJustifyKind" << " = " <<
    msrJustifyKindAsString (fWordsJustifyKind) <<
    endl <<
    setw (fieldWidth) <<
    "wordsVerticalAlignmentKind" << " = " <<
    msrVerticalAlignmentKindAsString (fWordsVerticalAlignmentKind) <<
    endl <<
    setw (fieldWidth) <<
    "wordsFontStyle" << " = " <<
    msrFontStyleKindAsString (fWordsFontStyleKind) <<
    endl <<
    setw (fieldWidth) <<
    "wordsFontSize" << " = " <<
    fWordsFontSize->fontSizeAsString () <<
    endl <<
    setw (fieldWidth) <<
    "wordsFontWeight" << " = " <<
    msrFontWeightKindAsString (fWordsFontWeightKind) <<
    endl <<
    setw (fieldWidth) <<
    "wordsFontXMLLang" << " = " <<
    msrWordsXMLLangKindAsString (fWordsXMLLangKind) <<
    endl;
  
  gIndenter--;
}

//______________________________________________________________________________
S_msrTempoNote msrTempoNote::create (
  int      inputLineNumber,
  rational tempoNoteWholeNotes,
  bool     tempoNoteBelongsToATuplet)
{  
  msrTempoNote * o =
    new msrTempoNote (
      inputLineNumber,
      tempoNoteWholeNotes,
      tempoNoteBelongsToATuplet);
  assert(o!=0);

  return o;
}

msrTempoNote::msrTempoNote (
  int      inputLineNumber,
  rational tempoNoteWholeNotes,
  bool     tempoNoteBelongsToATuplet)
    : msrElement (inputLineNumber)
{
  fTempoNoteWholeNotes = tempoNoteWholeNotes;
  
  fTempoNoteBelongsToATuplet = tempoNoteBelongsToATuplet;
}

msrTempoNote::~msrTempoNote ()
{}

void msrTempoNote::addBeamToTempoNote (S_msrBeam beam)
{
  fTempoNoteBeams.push_back (beam);
}

void msrTempoNote::acceptIn (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrTempoNote::acceptIn()" <<
      endl;
  }
      
  if (visitor<S_msrTempoNote>*
    p =
      dynamic_cast<visitor<S_msrTempoNote>*> (v)) {
        S_msrTempoNote elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrTempoNote::visitStart()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrTempoNote::acceptOut (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrTempoNote::acceptOut()" <<
      endl;
  }

  if (visitor<S_msrTempoNote>*
    p =
      dynamic_cast<visitor<S_msrTempoNote>*> (v)) {
        S_msrTempoNote elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrTempoNote::visitEnd()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrTempoNote::browseData (basevisitor* v)
{
  for (
    list<S_msrBeam>::const_iterator i = fTempoNoteBeams.begin ();
    i != fTempoNoteBeams.end ();
    i++ ) {
    // browse tempoTuplet element
    msrBrowser<msrBeam> browser (v);
    browser.browse (*(*i));
  } // for
}

string msrTempoNote::asString () const
{
  stringstream s;

  s <<
    "Tempo note" <<
    ", tempoNoteWholeNotes = " << fTempoNoteWholeNotes <<
    ", tempoNoteBelongsToATuplet = " <<
    booleanAsString (fTempoNoteBelongsToATuplet);

  return s.str ();
}

ostream& operator<< (ostream& os, const S_msrTempoNote& elt)
{
  elt->print (os);
  return os;
}

void msrTempoNote::print (ostream& os)
{  
  os <<
    "TempoNote" <<
    ", line " << fInputLineNumber <<
    endl;

  gIndenter++;

  const int fieldWidth = 26;
  
  os << left <<
    setw (fieldWidth) <<
    "tempoNoteWholeNotes" << " : " << fTempoNoteWholeNotes <<
    endl <<
    setw (fieldWidth) <<
    "tempoNoteBelongsToATuplet" << " : " <<
    booleanAsString (fTempoNoteBelongsToATuplet) <<
    endl <<
    setw (fieldWidth) <<
    "tempoNoteBeams";
    
    if (fTempoNoteBeams.size ()) {
      os <<
        endl;

      gIndenter++;
      
      list<S_msrBeam>::const_iterator
        iBegin = fTempoNoteBeams.begin (),
        iEnd   = fTempoNoteBeams.end (),
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
        " : " <<
       "none" <<
        endl;
    }

  gIndenter--;
}

//______________________________________________________________________________
S_msrTempoTuplet msrTempoTuplet::create (
  int                          inputLineNumber,
  int                          tempoTupletNumber,
  msrTempoTupletBracketKind    tempoTupletBracketKind,
  msrTempoTupletShowNumberKind tempoTupletShowNumberKind,
  int                          tempoTupletActualNotes,
  int                          tempoTupletNormalNotes,
  rational                     memberNotesDisplayWholeNotes)
{
  msrTempoTuplet* o =
    new msrTempoTuplet (
      inputLineNumber,
      tempoTupletNumber,
      tempoTupletBracketKind,
      tempoTupletShowNumberKind,
      tempoTupletActualNotes,
      tempoTupletNormalNotes,
      memberNotesDisplayWholeNotes);
  assert(o!=0);
  return o;
}

msrTempoTuplet::msrTempoTuplet (
  int                          inputLineNumber,
  int                          tempoTupletNumber,
  msrTempoTupletBracketKind    tempoTupletBracketKind,
  msrTempoTupletShowNumberKind tempoTupletShowNumberKind,
  int                          tempoTupletActualNotes,
  int                          tempoTupletNormalNotes,
  rational                     memberNotesDisplayWholeNotes)
    : msrElement (inputLineNumber)
{  
  fTempoTupletNumber = tempoTupletNumber;
  
  fTempoTupletBracketKind    = tempoTupletBracketKind;
  fTempoTupletShowNumberKind = tempoTupletShowNumberKind;

  fTempoTupletActualNotes = tempoTupletActualNotes;
  fTempoTupletNormalNotes = tempoTupletNormalNotes;

  fMemberNotesDisplayWholeNotes  = memberNotesDisplayWholeNotes;
  
  fTempoTupletDisplayWholeNotes  = rational (0, 1);

  if (gTraceOptions->fTraceTempos){
    gLogIOstream <<
      "Creating tempo tuplet '" <<
      this->asString () <<
      "'" <<
      endl;
  }
}

msrTempoTuplet::~msrTempoTuplet ()
{}

string msrTempoTuplet::tempoTupletTypeKindAsString (
  msrTempoTupletTypeKind tempoTupletTypeKind)
{
  string result;

  switch (tempoTupletTypeKind) {
    case msrTempoTuplet::k_NoTempoTupletType:
      result = "none";
      break;
    case msrTempoTuplet::kTempoTupletTypeStart:
      result = "start";
      break;
    case msrTempoTuplet::kTempoTupletTypeStop:
      result = "stop";
      break;
  } // switch

  return result;
}

string msrTempoTuplet::tempoTupletBracketKindAsString (
  msrTempoTupletBracketKind tempoTupletBracketKind)
{
  string result;

  switch (tempoTupletBracketKind) {
    case msrTempoTuplet::kTempoTupletBracketYes:
      result = "tempoTupletBracketYes";
      break;
    case msrTempoTuplet::kTempoTupletBracketNo:
      result = "tempoTupletBracketNo";
      break;
  } // switch

  return result;
}

string msrTempoTuplet::tempoTupletShowNumberKindAsString (
  msrTempoTupletShowNumberKind tempoTupletShowNumberKind)
{
  string result;
  
  switch (tempoTupletShowNumberKind) {
    case msrTempoTuplet::kTempoTupletShowNumberActual:
      result = "tempoTupletShowNumberActual";
      break;
    case msrTempoTuplet::kTempoTupletShowNumberBoth:
      result = "tempoTupletShowNumberBoth";
      break;
    case msrTempoTuplet::kTempoTupletShowNumberNone:
      result = "tempoTupletShowNumberNone";
      break;
  } // switch

  return result;
}

void msrTempoTuplet::addTempoNoteToTempoTuplet (S_msrTempoNote tempoNote)
{
  if (gTraceOptions->fTraceTempos) {
    gLogIOstream <<
      "Adding tempoNote '" <<
      tempoNote->asShortString () <<
      // the information is missing to display it the normal way JMI ???
      "' to tempoTuplet '" <<
      asString () <<
      "'" <<
      endl;
  }

  fTempoTupletElements.push_back (tempoNote);

/* JMI
  // register note's tempoTuplet uplink
  note->
    setNoteTempoTupletUplink (this);
  
  fTupletDisplayWholeNotes += // JMI
    note->getNoteDisplayWholeNotes ();  
  fTempoTupletDisplayWholeNotes.rationalise ();
    
    */
}

/*
void msrTempoTuplet::addTempoTupletToTempoTuplet (S_msrTempoTuplet tempoTuplet)
{
  if (gTraceOptions->fTraceTempos) {
    gLogIOstream <<
      "Adding tempoTuplet '" <<
      tempoTuplet->asString () <<
      "' to tempoTuplet '" <<
      asString () <<
      "'" <<
      endl;
  }

  // register tempoTuplet in elements list
  fTempoTupletElements.push_back (tempoTuplet);
    
  // account for tempoTuplet duration
  fTempoTupletDisplayWholeNotes += // JMI
    tempoTuplet->getTempoTupletDisplayWholeNotes ();
  fTempoTupletDisplayWholeNotes.rationalise ();
    
    / *
  fTempoTupletDisplayWholeNotes += // JMI
    tempoTuplet->getTempoTupletDisplayWholeNotes ();  
    * /

  // don't populate tempoTuplet's measure number nor position in measure,
  // this will be done in setTupletMeasureNumber()
  / * JMI
  tempoTuplet->setTempoTupletMeasureNumber (
    fTempoTupletMeasureNumber);

  // populate tempoTuplet's position in measure
  tempoTuplet->setTempoTupletPositionInMeasure (
    fTempoTupletPositionInMeasure);
    * /
}

void msrTempoTuplet::removeFirstNoteFromTempoTuplet (
  int            inputLineNumber,
  S_msrTempoNote tempoNote)
{
  if (gTraceOptions->fTraceTempos) {
    gLogIOstream <<
      "Removing first tempoNote '" <<
      tempoNote->asShortString () <<
      "' from tempoTuplet '" <<
      asString () <<
      "'" <<
      endl;
  }

  if (fTempoTupletElements.size ()) {
    S_msrElement
      firstTempoTupletElement =
        fTempoTupletElements.front ();

    for (
      list<S_msrElement>::iterator i=fTempoTupletElements.begin ();
      i!=fTempoTupletElements.end ();
      ++i) {
      if ((*i) == tempoNote) {
        // found note, erase it
        fTempoTupletElements.erase (i);
        
        // account for note duration
        fTempoTupletDisplayWholeNotes -= // JMI
          tempoNote->getTempoNoteWholeNotes ();  
        fTempoTupletDisplayWholeNotes.rationalise ();

        // don't update measure number nor position in measure: // JMI
        // they have not been set yet
  
        // return from function
        return;
      }
    } // for

    stringstream s;

    s <<
      "cannot remove tempoNote " <<
      tempoNote <<
      " from tempoTuplet " << asString () <<
      " since it has not been found";

    msrInternalError (
      gXml2lyOptions->fInputSourceName,
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  }
  
  else {
    stringstream s;

    s <<
      "cannot remove tempoNote " <<
      tempoNote <<
      " from empty tempoTuplet " <<
      " since it has not been found";

    msrInternalError (
      gXml2lyOptions->fInputSourceName,
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  }
}
*/

/* JMI
void msrTempoTuplet::applyDisplayFactorToTempoTupletMembers ()
{
  if (gTraceOptions->fTraceTempos) {
    gLogIOstream <<
      "% ==> applyDisplayFactorToTempoTupletMembers()" <<
      endl;

    gIndenter++;
    
    gLogIOstream <<
      "% fTempoTupletActualNotes = " <<
      fTempoTupletActualNotes <<
      ", fTempoTupletNormalNotes = " <<
      fTempoTupletNormalNotes <<
      endl <<
      endl;

    gIndenter--;
  }
}

void msrTempoTuplet::unapplySoundingFactorToTempoTupletMembers (
  int containingTempoTupletActualNotes,
  int containingTempoTupletNormalNotes)
{
  if (gTraceOptions->fTraceTempos) {
    gLogIOstream <<
      "unapplySoundingFactorToTempoTupletMembers()" <<
      endl;

    gIndenter++;
    
    gLogIOstream <<
      "% fTempoTupletActualNotes = " <<
      fTempoTupletActualNotes <<
      ", fTempoTupletNormalNotes = " <<
      fTempoTupletNormalNotes <<
      endl <<
      "% containingTempoTupletActualNotes = " <<
      containingTempoTupletActualNotes <<
      ", containingTempoTupletNormalNotes = " <<
      containingTempoTupletNormalNotes <<
      endl <<
      endl;

    gIndenter--;
  }

  fTempoTupletActualNotes /=
    containingTempoTupletActualNotes;
  fTempoTupletNormalNotes /=
    containingTempoTupletNormalNotes;
}
*/

void msrTempoTuplet::acceptIn (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrTempoTuplet::acceptIn()" <<
      endl;
  }
      
  if (visitor<S_msrTempoTuplet>*
    p =
      dynamic_cast<visitor<S_msrTempoTuplet>*> (v)) {
        S_msrTempoTuplet elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrTempoTuplet::visitStart()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrTempoTuplet::acceptOut (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrTempoTuplet::acceptOut()" <<
      endl;
  }

  if (visitor<S_msrTempoTuplet>*
    p =
      dynamic_cast<visitor<S_msrTempoTuplet>*> (v)) {
        S_msrTempoTuplet elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrTempoTuplet::visitEnd()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrTempoTuplet::browseData (basevisitor* v)
{
  for (
    list<S_msrElement>::const_iterator i = fTempoTupletElements.begin ();
    i != fTempoTupletElements.end ();
    i++ ) {
    // browse tempoTuplet element
    msrBrowser<msrElement> browser (v);
    browser.browse (*(*i));
  } // for
}

ostream& operator<< (ostream& os, const S_msrTempoTuplet& elt)
{
  elt->print (os);
  return os;
}

string msrTempoTuplet::asString () const
{
  stringstream s;

  s <<
    "TempoTuplet " <<
    fTempoTupletActualNotes << "/" << fTempoTupletNormalNotes <<
    " " << fTempoTupletDisplayWholeNotes << " display whole notes" <<
    ":";
  
  s << "[[";

  if (fTempoTupletElements.size ()) {
    list<S_msrElement>::const_iterator
      iBegin = fTempoTupletElements.begin (),
      iEnd   = fTempoTupletElements.end (),
      i      = iBegin;
    for ( ; ; ) {
      
      if (
        S_msrTempoNote note = dynamic_cast<msrTempoNote*>(&(*(*i)))
        ) {    
        s <<
          note->asShortString ();
      }
    
      else if (
        S_msrTempoTuplet tempoTuplet = dynamic_cast<msrTempoTuplet*>(&(*(*i)))
        ) {
        s <<
          tempoTuplet->asString ();
      }
      
      else {
        msrInternalError (
          gXml2lyOptions->fInputSourceName,
          fInputLineNumber,
          __FILE__, __LINE__,
          "tempoTuplet member should be a note, a chord or another tempoTuplet");
      }
  
      if (++i == iEnd) break;
      s << " ";
      
    } // for
  }

  s << "]]";
  
  return s.str ();
}

/* JMI
string msrTempoTuplet::asString () const
{
  stringstream s;

  s <<
    "TempoTuplet " <<
    fTempoTupletActualNotes << "/" << fTempoTupletNormalNotes <<
    " " << fTempoTupletSoundingWholeNotes << " sound whole notes" <<
    " @meas "<<
    fTempoTupletMeasureNumber <<
    ":";

  if (fTempoTupletPositionInMeasure.getNumerator () < 0)
    s << "?";
  else
    s << fTempoTupletPositionInMeasure;
  
  s << "[[";

  if (fTempoTupletElements.size ()) {
    list<S_msrElement>::const_iterator
      iBegin = fTempoTupletElements.begin (),
      iEnd   = fTempoTupletElements.end (),
      i      = iBegin;
    for ( ; ; ) {
      
      if (
        S_msrTempoNote note = dynamic_cast<msrTempoNote*>(&(*(*i)))
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
        S_msrTempoTuplet tempoTuplet = dynamic_cast<msrTempoTuplet*>(&(*(*i)))
        ) {
        s <<
          tempoTuplet->asString ();
      }
      
      else {
        msrInternalError (
          gXml2lyOptions->fInputSourceName,
          fInputLineNumber,
          __FILE__, __LINE__,
          "tempoTuplet member should be a note, a chord or another tempoTuplet");
      }
  
      if (++i == iEnd) break;
      s << " ";
    } // for
  }

  s << "]]";
  
  return s.str ();
}
*/

void msrTempoTuplet::print (ostream& os)
{
  os <<
    "TempoTuplet " <<
    fTempoTupletActualNotes << "/" << fTempoTupletNormalNotes <<
    ", " <<
    singularOrPlural (
      fTempoTupletElements.size (), "element", "elements") <<
    ", display whole notes: " <<
    fTempoTupletDisplayWholeNotes <<
    ", line " << fInputLineNumber <<
    endl;

  gIndenter++;
  
  const int fieldWidth = 30;

  os << left <<
    setw (fieldWidth) <<
    "TempoTupletBracketKind" << " : " <<
    tempoTupletBracketKindAsString (
      fTempoTupletBracketKind) <<
    endl <<
    setw (fieldWidth) <<
    "TempoTupletShowNumberKind" << " : " <<
    tempoTupletShowNumberKindAsString (
      fTempoTupletShowNumberKind) <<
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
  if (fTempoTupletPositionInMeasure.getNumerator () < 0)
    os << "???)";
  else
    os << fTempoTupletPositionInMeasure << ")";
  os <<
    endl;
    */

  os <<
    "TempoTupletsElements";
    
  if (fTempoTupletElements.size ()) {
    gIndenter++;

    list<S_msrElement>::const_iterator
      iBegin = fTempoTupletElements.begin (),
      iEnd   = fTempoTupletElements.end (),
      i      = iBegin;
    for ( ; ; ) {
      os << (*i);
      if (++i == iEnd) break;
      // no endl here
    } // for
    
    gIndenter--;
    }
  else {
    os << left <<
      " : " << "none" <<
      endl;
  }

  gIndenter--;
}

//______________________________________________________________________________
S_msrTempoRelationshipElements msrTempoRelationshipElements::create (
  int      inputLineNumber,
  msrTempoRelationshipElementsKind
           tempoRelationshipElementsKind)
{  
  msrTempoRelationshipElements * o =
    new msrTempoRelationshipElements (
      inputLineNumber,
      tempoRelationshipElementsKind);
  assert(o!=0);

  return o;
}

msrTempoRelationshipElements::msrTempoRelationshipElements (
  int      inputLineNumber,
  msrTempoRelationshipElementsKind
           tempoRelationshipElementsKind)
    : msrElement (inputLineNumber)
{
  fTempoRelationshipElementsKind = tempoRelationshipElementsKind;
}

msrTempoRelationshipElements::~msrTempoRelationshipElements ()
{}

void msrTempoRelationshipElements::addElementToTempoRelationshipElements (
  S_msrElement element)
{
  if (gTraceOptions->fTraceTempos){
    gLogIOstream <<
      "Adding element '" <<
      element->asString () <<
      "' to tempo relationship" <<
      endl;
  }
  
  fTempoRelationshipElementsList.push_back (element);
}

void msrTempoRelationshipElements::acceptIn (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrTempoRelationshipElements::acceptIn()" <<
      endl;
  }
      
  if (visitor<S_msrTempoRelationshipElements>*
    p =
      dynamic_cast<visitor<S_msrTempoRelationshipElements>*> (v)) {
        S_msrTempoRelationshipElements elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrTempoRelationshipElements::visitStart()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrTempoRelationshipElements::acceptOut (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrTempoRelationshipElements::acceptOut()" <<
      endl;
  }

  if (visitor<S_msrTempoRelationshipElements>*
    p =
      dynamic_cast<visitor<S_msrTempoRelationshipElements>*> (v)) {
        S_msrTempoRelationshipElements elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrTempoRelationshipElements::visitEnd()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrTempoRelationshipElements::browseData (basevisitor* v)
{
  // browse the elements list
  if (fTempoRelationshipElementsList.size ()) {
    for (
      list<S_msrElement>::const_iterator i =
        fTempoRelationshipElementsList.begin ();
      i != fTempoRelationshipElementsList.end ();
      i++) {
      // browse the element
      msrBrowser<msrElement> browser (v);
      browser.browse (*(*i));
    } // for
  }
}

string msrTempoRelationshipElements::asString () const
{
  stringstream s;

  s <<
    "TempoRelationshipElements" <<
    ", fTempoRelationshipElementsKind = " << fTempoRelationshipElementsKind;

  return s.str ();
}

ostream& operator<< (ostream& os, const S_msrTempoRelationshipElements& elt)
{
  elt->print (os);
  return os;
}

string msrTempoRelationshipElements::tempoRelationshipElementsKindAsString (
  msrTempoRelationshipElementsKind tempoRelationshipElementsKind)
{
  string result;
  
  switch (tempoRelationshipElementsKind) {
    case msrTempoRelationshipElements::kTempoRelationshipElementsLeft:
      result = "tempoRelationshipElementsLeft";
      break;
    case msrTempoRelationshipElements::kTempoRelationshipElementsRight:
      result = "tempoRelationshipElementsRight";
      break;
  } // switch

  return result;
}

void msrTempoRelationshipElements::print (ostream& os)
{  
  os <<
    "TempoRelationshipElements" <<
    ", line " << fInputLineNumber <<
    endl;
    
  gIndenter++;

  os <<
    "tempoRelationshipElementsKindAsString : " <<
    tempoRelationshipElementsKindAsString (
      fTempoRelationshipElementsKind) <<
    endl;

  const int fieldWidth = 26;
  
  os << left <<
    setw (fieldWidth) <<
    "tempoRelationshipElementsList";

    if (fTempoRelationshipElementsList.size ()) {
      gIndenter++;

      os <<
        endl;

      list<S_msrElement>::const_iterator
        iBegin = fTempoRelationshipElementsList.begin (),
        iEnd   = fTempoRelationshipElementsList.end (),
        i      = iBegin;
        
      for ( ; ; ) {
        os << (*i);
        if (++i == iEnd) break;
        // os << endl;
      } // for
  
      gIndenter--;
  }
  else {
    os <<
      " : " << "none ???" <<
      endl;
  }
  
  gIndenter--;
}

//______________________________________________________________________________
S_msrTempo msrTempo::create (
  int               inputLineNumber,
  S_msrWords        tempoWords,
  msrDottedDuration tempoBeatUnit,
  string            tempoPerMinute,
  msrTempoParenthesizedKind
                    tempoParenthesizedKind,
  msrPlacementKind  tempoPlacementKind)
{
  msrTempo* o =
    new msrTempo (
      inputLineNumber,
      tempoWords,
      tempoBeatUnit,
      tempoPerMinute,
      tempoParenthesizedKind,
      tempoPlacementKind);
  assert(o!=0);
  return o;
}

S_msrTempo msrTempo::create (
  int               inputLineNumber,
  S_msrWords        tempoWords,
  msrDottedDuration tempoBeatUnit,
  msrDottedDuration tempoEquivalentBeatUnit,
  msrTempoParenthesizedKind
                    tempoParenthesizedKind,
  msrPlacementKind  tempoPlacementKind)
{
  msrTempo* o =
    new msrTempo (
      inputLineNumber,
      tempoWords,
      tempoBeatUnit,
      tempoEquivalentBeatUnit,
      tempoParenthesizedKind,
      tempoPlacementKind);
  assert(o!=0);
  return o;
}

S_msrTempo msrTempo::create (
  int               inputLineNumber,
  S_msrWords        tempoWords,
  S_msrTempoRelationshipElements
                    tempoRelationLeftElements,
  S_msrTempoRelationshipElements
                    tempoRelationRightElements,
  msrTempoParenthesizedKind
                    tempoParenthesizedKind,
  msrPlacementKind  tempoPlacementKind)
{
  msrTempo* o =
    new msrTempo (
      inputLineNumber,
      tempoWords,
      tempoRelationLeftElements,
      tempoRelationRightElements,
      tempoParenthesizedKind,
      tempoPlacementKind);
  assert(o!=0);
  return o;
}

msrTempo::msrTempo (
  int               inputLineNumber,
  S_msrWords        tempoWords,
  msrDottedDuration tempoBeatUnit,
  string            tempoPerMinute,
  msrTempoParenthesizedKind
                    tempoParenthesizedKind,
  msrPlacementKind  tempoPlacementKind)
    : msrElement (inputLineNumber),
      fTempoBeatUnit (tempoBeatUnit),
      fTempoEquivalentBeatUnit ()
{
  fTempoKind = kTempoBeatUnitsPerMinute;
  
  fTempoWords = tempoWords;
  
  fTempoPerMinute = tempoPerMinute;

  fTempoParenthesizedKind = tempoParenthesizedKind;

  fTempoPlacementKind = tempoPlacementKind;
}

msrTempo::msrTempo (
  int               inputLineNumber,
  S_msrWords        tempoWords,
  msrDottedDuration tempoBeatUnit,
  msrDottedDuration tempoEquivalentBeatUnit,
  msrTempoParenthesizedKind
                    tempoParenthesizedKind,
  msrPlacementKind  tempoPlacementKind)
    : msrElement (inputLineNumber),
      fTempoBeatUnit (tempoBeatUnit),
      fTempoEquivalentBeatUnit (tempoEquivalentBeatUnit)
{
  fTempoKind = kTempoBeatUnitsEquivalence;
  
  fTempoWords = tempoWords;

  fTempoPerMinute = "";

  fTempoParenthesizedKind = tempoParenthesizedKind;

  fTempoPlacementKind = tempoPlacementKind;
}

msrTempo::msrTempo (
  int               inputLineNumber,
  S_msrWords        tempoWords,
  S_msrTempoRelationshipElements
                    tempoRelationLeftElements,
  S_msrTempoRelationshipElements
                    tempoRelationRightElements,
  msrTempoParenthesizedKind
                    tempoParenthesizedKind,
  msrPlacementKind  tempoPlacementKind)
    : msrElement (inputLineNumber)
{
  fTempoKind = kTempoNotesRelationShip;
  
  fTempoWords = tempoWords;

  fTempoPerMinute = "";

  fTempoRelationLeftElements =
    tempoRelationLeftElements;
  fTempoRelationRightElements =
    tempoRelationRightElements;

  fTempoParenthesizedKind = tempoParenthesizedKind;

  fTempoPlacementKind = tempoPlacementKind;
}

msrTempo::~msrTempo ()
{}

void msrTempo::acceptIn (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrTempo::acceptIn()" <<
      endl;
  }
      
  if (visitor<S_msrTempo>*
    p =
      dynamic_cast<visitor<S_msrTempo>*> (v)) {
        S_msrTempo elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrTempo::visitStart()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrTempo::acceptOut (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrTempo::acceptOut()" <<
      endl;
  }

  if (visitor<S_msrTempo>*
    p =
      dynamic_cast<visitor<S_msrTempo>*> (v)) {
        S_msrTempo elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrTempo::visitEnd()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}


void msrTempo::browseData (basevisitor* v)
{
  switch (fTempoKind) {
    case msrTempo::k_NoTempoKind:
      break;
      
    case msrTempo::kTempoBeatUnitsPerMinute:
      break;
      
    case msrTempo::kTempoBeatUnitsEquivalence:
      break;
      
    case msrTempo::kTempoNotesRelationShip:
      {
        // browse the left elements
        if (fTempoRelationLeftElements) {
          msrBrowser<msrTempoRelationshipElements> browser (v);
          browser.browse (*fTempoRelationLeftElements);
        }
        
        // browse the right elements
        if (fTempoRelationRightElements) {
          msrBrowser<msrTempoRelationshipElements> browser (v);
          browser.browse (*fTempoRelationRightElements);
        }
      }
      break;
  } // switch
}

string msrTempo::tempoKindAsString (
  msrTempoKind tempoKind)
{
  string result;
  
  switch (tempoKind) {
    case msrTempo::k_NoTempoKind:
      result = "k_NoTempoKind???";
      break;
    case msrTempo::kTempoBeatUnitsPerMinute:
      result = "tempoBeatUnitsPerMinute";
      break;
    case msrTempo::kTempoBeatUnitsEquivalence:
      result = "tempoBeatUnitsEquivalence";
      break;
    case msrTempo::kTempoNotesRelationShip:
      result = "tempoNotesRelationShip";
      break;
  } // switch

  return result;
}

string msrTempo::tempoParenthesizedAsString (
  msrTempoParenthesizedKind tempoParenthesizedKind)
{
  string result;
  
  switch (tempoParenthesizedKind) {
    case msrTempo::kTempoParenthesizedYes:
      result = "tempoParenthesizedYes";
      break;
    case msrTempo::kTempoParenthesizedNo:
      result = "tempoParenthesizedNo";
      break;
  } // switch

  return result;
}

string msrTempo::tempoRelationAsString (
  msrTempoRelationKind tempoRelationKind)
{
  string result;
  
  switch (tempoRelationKind) {
    case msrTempo::kTempoRelationEquals:
      result = "tempoRelationEquals";
      break;
    case msrTempo::k_NoTempoRelation:
      result = "k_NoTempoRelation???";
      break;
  } // switch

  return result;
}

string msrTempo::asString () const
{
  stringstream s;

  s <<
    "Tempo" <<
    ", tempoKind = \"" << tempoKindAsString (fTempoKind) << "\"" <<
    ", tempoWords = \"" << fTempoWords << "\"" <<
    ", " << fTempoBeatUnit << " = " << fTempoPerMinute <<
    ", fTempoParenthesizedKind = "  <<
    tempoParenthesizedAsString (fTempoParenthesizedKind);

  return s.str ();
}

ostream& operator<< (ostream& os, const S_msrTempo& elt)
{
  elt->print (os);
  return os;
}

void msrTempo::print (ostream& os)
{
  os <<
    "Tempo" <<
    ", line " << fInputLineNumber <<
    endl;

  gIndenter++;

  const int fieldWidth = 23;

  os << left <<
     setw (fieldWidth) <<
   "tempoKind" << " = " << tempoKindAsString (fTempoKind) <<
    endl;

  os << left <<
    "tempoWords";
    
  if (fTempoWords) {
    os <<
      ":" <<
      endl;
      
    gIndenter++;
    
    os << fTempoWords;
      
    gIndenter--;
  }
  else {
    os <<
      setw (fieldWidth) <<
      " = " << "none" <<
      endl;;
  }
    
  os << left <<
    setw (fieldWidth) <<
    "tempoBeatUnit:" <<
    endl;

  gIndenter++;

  os <<
    fTempoBeatUnit;

  gIndenter--;
  
  os << left <<
    setw (fieldWidth) <<
    "tempoPerMinute" << " = \"" << fTempoPerMinute << "\"" <<
    endl <<
    setw (fieldWidth) <<
    "tempoParenthesizedKind"  << " = " <<
    tempoParenthesizedAsString (fTempoParenthesizedKind) <<
    endl;

  os << left <<
    setw (fieldWidth) <<
    "tempoRelationLeftElements";
  if (fTempoRelationLeftElements) {
    os <<
      " : " <<
      endl;

    gIndenter++;

    os <<
      fTempoRelationLeftElements;

    gIndenter--;
  }
  else {
    os << " : " << "none" <<
    endl;
  }

  os << left <<
    setw (fieldWidth) <<
    "tempoRelationRightElements";
  if (fTempoRelationRightElements) {
    os <<
      ":" <<
      endl;

    gIndenter++;

    os <<
      fTempoRelationRightElements;

    gIndenter--;
  }
  else {
    os << " : " << "none" <<
    endl;
  }

  gIndenter--;
}

//______________________________________________________________________________
S_msrSyllable msrSyllable::create (
  int                   inputLineNumber,
  msrSyllableKind       syllableKind,
  msrSyllableExtendKind syllableExtendKind,
  rational              syllableWholeNotes,
  S_msrStanza           syllableStanzaUplink)
{
  msrSyllable* o =
    new msrSyllable (
      inputLineNumber,
      syllableKind,
      syllableExtendKind,
      syllableWholeNotes,
      syllableStanzaUplink);
  assert(o!=0);

  return o;
}

msrSyllable::msrSyllable (
  int                   inputLineNumber,
  msrSyllableKind       syllableKind,
  msrSyllableExtendKind syllableExtendKind,
  rational              syllableWholeNotes,
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
  
  fSyllableWholeNotes = syllableWholeNotes;

  fSyllableExtendKind = syllableExtendKind;
  
  // fSyllableNoteUplink will be set
  // by appendSyllableToNoteAndSetItsUplink() later

  if (gTraceOptions->fTraceLyrics) {
    gLogIOstream <<
      "Creating a syllable containing:" <<
      endl;

    gIndenter++;

    print (gLogIOstream);

    gIndenter--;
  }
}

msrSyllable::~msrSyllable ()
{}

S_msrSyllable msrSyllable::createSyllableNewbornClone (
  S_msrPart containingPart)
{
  if (gTraceOptions->fTraceLyrics) {
    gLogIOstream <<
      "Creating a newborn clone of syllable '" <<
      asString () <<
      "'" <<
      endl;
  }

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
  if (gTraceOptions->fTraceLyrics) {
    gLogIOstream <<
      "Creating a newborn clone of syllable '" <<
      asString () <<
      "'" <<
      endl;
  }

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
  if (gTraceOptions->fTraceLyrics) {
    gLogIOstream <<
      "Appending text \"" <<
      text <<
      "\" to the texts list of syllable '" <<
      asString () <<
      "'" <<
      endl;
  }

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
}

string msrSyllable::syllableWholeNotesAsMsrString () const
{
  return
    wholeNotesAsMsrString (
      fInputLineNumber,
      fSyllableWholeNotes);
}

void msrSyllable::acceptIn (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrSyllable::acceptIn()" <<
      endl;
  }
      
  if (visitor<S_msrSyllable>*
    p =
      dynamic_cast<visitor<S_msrSyllable>*> (v)) {
        S_msrSyllable elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrSyllable::visitStart()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrSyllable::acceptOut (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrSyllable::acceptOut()" <<
      endl;
  }

  if (visitor<S_msrSyllable>*
    p =
      dynamic_cast<visitor<S_msrSyllable>*> (v)) {
        S_msrSyllable elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrSyllable::visitEnd()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrSyllable::browseData (basevisitor* v)
{}

ostream& operator<< (ostream& os, const S_msrSyllable& elt)
{
  elt->print (os);
  return os;
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
      
    case msrSyllable::k_NoSyllable:
      result = "noSyllable ???";
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
    case msrSyllable::k_NoSyllableExtend:
      result = "noSyllableExtend";
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
        "measure end" <<
        " measure " << "fSyllableText ???";
      break;
      
    case msrSyllable::kSyllableLineBreak:
      // fSyllableText contains the measure number
      s << 
        "line break" <<
        " measure " << "fSyllableText ???";
      break;
      
    case msrSyllable::kSyllablePageBreak:
      s << 
        "page break" <<
        " measure " << "fSyllableText ???";
      break;
      
    case msrSyllable::k_NoSyllable:
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
    ", whole notes:" <<
    syllableWholeNotesAsMsrString () <<
    " (" << fSyllableWholeNotes << ")" <<
    ", line " << fInputLineNumber <<
    endl;

  gIndenter++;

  const int fieldWidth = 19;
  os << left <<
    setw (fieldWidth) <<
    "SyllableExtendKind" << " : " <<
    syllableExtendKindAsString (
      fSyllableExtendKind) <<
    endl <<
    setw (fieldWidth) <<
    "Texts list" << " : ";
    
    writeTextsList (
      fSyllableTextsList,
      os);

  os << left <<
    endl <<
    setw (fieldWidth) <<
    "Note uplink" << " : " <<
    syllableNoteUplinkAsString () <<      
    endl;

  switch (fSyllableKind) { // JMI
    case msrSyllable::kSyllableSingle:
    case msrSyllable::kSyllableBegin:
    case msrSyllable::kSyllableMiddle:
    case msrSyllable::kSyllableEnd:
    case msrSyllable::kSyllableSkip:
      break;
      
    case kSyllableMeasureEnd:
      // fSyllableText contains the measure number
      os << 
        "measure end" <<
        " measure " << "fSyllableText ???";
      break;
      
    case kSyllableLineBreak:
      // fSyllableText contains the measure number
      os << 
        "line break" <<
        " measure " << "fSyllableText ???";
      break;
      
    case kSyllablePageBreak:
      os << 
        "page break" <<
        " measure " << "fSyllableText ???";
      break;
      
    case k_NoSyllable:
      msrInternalError (
        gXml2lyOptions->fInputSourceName,
        fInputLineNumber,
        __FILE__, __LINE__,
        "syllable type has not been set");
      break;
  } // switch

  os << left <<
    setw (fieldWidth) <<
    "Stanza" << " : " <<
    fSyllableStanzaUplink->getStanzaName () <<
    endl;

  gIndenter--;
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
    fStanzaVoiceUplink->getVoiceName() +
    "_Stanza_" + 
      stringNumbersToEnglishWords (
        makeSingleWordFromString (
          fStanzaNumber));
 
  if (gTraceOptions->fTraceLyrics) {
    gLogIOstream <<
      "Initializing stanza " << getStanzaName () <<
      endl;
  }

  fStanzaTextPresent = false;
}

string msrStanza::getStanzaName () const
{
  return fStanzaName;
}

msrStanza::~msrStanza()
{}

S_msrStanza msrStanza::createStanzaNewbornClone (
  S_msrVoice containingVoice)
{
  if (gTraceOptions->fTraceLyrics) {
    gLogIOstream <<
      "Creating a newborn clone of stanza \"" <<
      getStanzaName () <<
      "\" in voice \"" <<
      containingVoice->getVoiceName () <<
      "\"" <<
      endl;
  }

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
  if (gTraceOptions->fTraceLyrics) {
    gLogIOstream <<
      "Creating a deep copy of stanza \"" <<
      getStanzaName () <<
      "\" in voice \"" <<
      containingVoice->getVoiceName () <<
      "\"" <<
      endl;
  }

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
  if (gTraceOptions->fTraceLyrics) {
    gLogIOstream <<
      "Appending syllable '" << syllable->asString () <<
      "' to stanza " << getStanzaName () <<
      endl;
  }
      
  // append the syllable to this stanza
  fSyllables.push_back (syllable);

  // does this stanza contain text?
  switch (syllable->getSyllableKind ()) {
    
    case msrSyllable::kSyllableSingle:
    case msrSyllable::kSyllableBegin:
    case msrSyllable::kSyllableMiddle:
    case msrSyllable::kSyllableEnd:
      // only now, in case addSyllableToStanza() is called
      // from LPSR for example
      fStanzaTextPresent = true;
      break;
      
    case msrSyllable::kSyllableSkip:
    case msrSyllable::kSyllableMeasureEnd:
    case msrSyllable::kSyllableLineBreak:
    case msrSyllable::kSyllablePageBreak:
      break;
      
    case msrSyllable::k_NoSyllable:
      msrInternalError (
        gXml2lyOptions->fInputSourceName,
        fInputLineNumber,
        __FILE__, __LINE__,
        "syllable type has not been set");
      break;
  } // switch
}

S_msrSyllable msrStanza::appendRestSyllableToStanza (
  int      inputLineNumber,
  rational wholeNotes)
{
  if (gTraceOptions->fTraceLyrics) {
    gLogIOstream <<
      "Appending 'Rest' syllable" <<
      " to stanza " << getStanzaName () <<
      ", whole notes = " << wholeNotes <<
      ", line " << inputLineNumber <<
      endl;
  }
  
  // create stanza rest syllable
  S_msrSyllable
    syllable =
      msrSyllable::create (
        inputLineNumber,
        msrSyllable::kSyllableSkip, // JMI ??? kSyllableRest,
        msrSyllable::k_NoSyllableExtend,
        wholeNotes,
        this);

  // append syllable to this stanza
  appendSyllableToStanza (syllable);

  // and return it
  return syllable;
}

S_msrSyllable msrStanza::appendSkipSyllableToStanza (
  int      inputLineNumber,
  rational wholeNotes)
{
  if (gTraceOptions->fTraceLyrics) {
    gLogIOstream <<
      "Appending 'Skip' syllable " <<
      " to stanza " << getStanzaName () <<
      ", whole notes = " << wholeNotes <<
      ", line " << inputLineNumber <<
      endl;
  }
  
  // create stanza skip syllable
  S_msrSyllable
    syllable =
      msrSyllable::create (
        inputLineNumber,
        msrSyllable::kSyllableSkip,
        msrSyllable::k_NoSyllableExtend,
        wholeNotes,
        this);

  // append syllable to this stanza
  appendSyllableToStanza (syllable);

  // and return it
  return syllable;
}

S_msrSyllable msrStanza::appendMeasureEndSyllableToStanza (
  int inputLineNumber)
{
  if (gTraceOptions->fTraceLyrics) {
    gLogIOstream <<
      "Appending 'Measure end' syllable " <<
      " to stanza " << getStanzaName () <<
      ", line " << inputLineNumber <<
      endl;
  }
  
  // create stanza skip syllable
  S_msrSyllable
    syllable =
      msrSyllable::create (
        inputLineNumber,
        msrSyllable::kSyllableMeasureEnd,
        msrSyllable::k_NoSyllableExtend,
        0, // wholeNotes
        this);

  // append syllable to this stanza
  appendSyllableToStanza (syllable);

  // and return it
  return syllable;
}

S_msrSyllable msrStanza::appendMelismaSyllableToStanza (
  int             inputLineNumber,
  msrSyllable::msrSyllableKind
                  syllableKind,
  rational        wholeNotes)
{
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
  
  // create stanza melisma syllable
  S_msrSyllable
    syllable =
      msrSyllable::create (
        inputLineNumber,
        syllableKind,
        msrSyllable::k_NoSyllableExtend,
        wholeNotes,
        this);

  // append syllable to this stanza
  appendSyllableToStanza (syllable);

  // and return it
  return syllable;
}

S_msrSyllable msrStanza::appendLineBreakSyllableToStanza (
  int inputLineNumber)
{
  if (gTraceOptions->fTraceLyrics) {
    gLogIOstream <<
      "Appending 'line break' syllable" <<
      " to stanza " << getStanzaName () <<
      ", line " << inputLineNumber <<
      endl;
  }
  
  // create stanza line break syllable
  S_msrSyllable
    syllable =
      msrSyllable::create (
        inputLineNumber,
        msrSyllable::kSyllableLineBreak,
        msrSyllable::k_NoSyllableExtend,
        0,
        this);

  // append syllable to this stanza
  appendSyllableToStanza (syllable);

  // and return it
  return syllable;
}

/* JMI
S_msrSyllable msrStanza::appendTiedSyllableToStanza (
  int      inputLineNumber,
  rational wholeNotes)
{
  if (gTraceOptions->fTraceLyrics) {
    gLogIOstream <<
      "Appending 'Tied' syllable" <<
      " to stanza " << getStanzaName () <<
      ", whole notes = " << wholeNotes <<
      ", line " << inputLineNumber <<
      endl;
  }
  
  // create stanza tied syllable
  S_msrSyllable
    syllable =
      msrSyllable::create (
        inputLineNumber,
        msrSyllable::kTiedSyllable,
        msrSyllable::k_NoSyllableExtend,
        wholeNotes,
        this);
        
  // append syllable to this stanza
  appendSyllableToStanza (syllable);

  // and return it
  return syllable;
}

S_msrSyllable msrStanza::appendSlurSyllableToStanza (
  int      inputLineNumber,
  rational wholeNotes)
{
  if (gTraceOptions->fTraceLyrics) {
    gLogIOstream <<
      "Appending 'Slur' syllable" <<
      " to stanza " << getStanzaName () <<
      ", whole notes = " << wholeNotes <<
      ", line " << inputLineNumber <<
      endl;
  }
  
  // create stanza slur syllable
  S_msrSyllable
    syllable =
      msrSyllable::create (
        inputLineNumber,
        msrSyllable::kSlurSyllable,
        msrSyllable::k_NoSyllableExtend,
        wholeNotes,
        this);
        
  // append syllable to this stanza
  appendSyllableToStanza (syllable);

  // and return it
  return syllable;
}

S_msrSyllable msrStanza::appendSlurBeyondEndSyllableToStanza (
  int      inputLineNumber,
  rational wholeNotes)
{
  if (gTraceOptions->fTraceLyrics) {
    gLogIOstream <<
      "Appending a 'SlurBeyondEnd' syllable" <<
      " to stanza " << getStanzaName () <<
      ", whole notes = " << wholeNotes <<
      ", line " << inputLineNumber <<
      endl;
  }
  
  // create stanza slur syllable
  S_msrSyllable
    syllable =
      msrSyllable::create (
        inputLineNumber,
        msrSyllable::kSlurBeyondEndSyllable,
        msrSyllable::k_NoSyllableExtend,
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
  if (gTraceOptions->fTraceLyrics) {
    gLogIOstream <<
      "Appending 'Ligature' syllable:" <<
      " to stanza " << getStanzaName () <<
      ", whole notes = " << wholeNotes <<
      ", line " << inputLineNumber <<
      endl;
  }
  
  // create stanza ligature syllable
  S_msrSyllable
    syllable =
      msrSyllable::create (
        inputLineNumber,
        msrSyllable::kLigatureSyllable,
        msrSyllable::k_NoSyllableExtend,
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
  if (gTraceOptions->fTraceLyrics) {
    gLogIOstream <<
      "Appending a 'LigatureBeyondEnd' syllable" <<
      " to stanza " << getStanzaName () <<
      ", whole notes = " << wholeNotes <<
      ", line " << inputLineNumber <<
      endl;
  }
  
  // create stanza ligature syllable
  S_msrSyllable
    syllable =
      msrSyllable::create (
        inputLineNumber,
        msrSyllable::kLigatureBeyondEndSyllable,
        msrSyllable::k_NoSyllableExtend,
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
  if (gTraceOptions->fTraceLyrics) {
    gLogIOstream <<
      "Appending a 'barcheck' syllable" <<
      " to stanza " << getStanzaName () <<
      ", line " << inputLineNumber <<
      endl;
  }

  // create stanza break syllable
  S_msrSyllable
    syllable =
      msrSyllable::create (
        inputLineNumber,
        msrSyllable::kBarcheckSyllable,
 // JMI       nextMeasureNumber,
        msrSyllable::k_NoSyllableExtend,
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
  if (gTraceOptions->fTraceLyrics) {
    gLogIOstream <<
      "Appending a 'bar number check' syllable" <<
      " to stanza " << getStanzaName () <<
      ", line " << inputLineNumber <<
      endl;
  }

  // create stanza break syllable        
  S_msrSyllable
    syllable =
      msrSyllable::create (
        inputLineNumber,
        msrSyllable::kBarNumberCheckSyllable,
 // JMI  nextMeasureNumber,
        msrSyllable::k_NoSyllableExtend,
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
  if (gTraceOptions->fTraceLyrics) {
    gLogIOstream <<
      "Appending a 'LineBreak' syllable" <<
      " to stanza " << getStanzaName () <<
      ", line " << inputLineNumber <<
      endl;
  }
  
  // create stanza break syllable
  S_msrSyllable
    syllable =
      msrSyllable::create (
        inputLineNumber,
        msrSyllable::kSyllableLineBreak,
 // JMI  nextMeasureNumber,
        msrSyllable::k_NoSyllableExtend,
        0,  // wholeNotes
        this);
        
  // append syllable to this stanza
  appendSyllableToStanza (syllable);

  // and return it
  return syllable;
}

S_msrSyllable msrStanza::appendPageBreakSyllableToStanza (
  int    inputLineNumber,
  string nextMeasureNumber)
{
  if (gTraceOptions->fTraceLyrics) {
    gLogIOstream <<
      "Appending a 'PageBreak' syllable" <<
      " to stanza " << getStanzaName () <<
      ", line " << inputLineNumber <<
      endl;
  }
  
  // create stanza break syllable
  S_msrSyllable
    syllable =
      msrSyllable::create (
        inputLineNumber,
        msrSyllable::kSyllablePageBreak,
 // JMI  nextMeasureNumber,
        msrSyllable::k_NoSyllableExtend,
        0,  // wholeNotes
        this);
        
  // append syllable to this stanza
  appendSyllableToStanza (syllable);

  // and return it
  return syllable;
}

void msrStanza::acceptIn (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrStanza::acceptIn()" <<
      endl;
  }
      
  if (visitor<S_msrStanza>*
    p =
      dynamic_cast<visitor<S_msrStanza>*> (v)) {
        S_msrStanza elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrStanza::visitStart()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrStanza::acceptOut (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrStanza::acceptOut()" <<
      endl;
  }

  if (visitor<S_msrStanza>*
    p =
      dynamic_cast<visitor<S_msrStanza>*> (v)) {
        S_msrStanza elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrStanza::visitEnd()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrStanza::browseData (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrStanza::browseData()" <<
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
      "% <== msrStanza::browseData()" <<
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

  if (gTraceOptions->fTraceHarmonies) {
    gLogIOstream <<
      "Creating harmony degree '" <<
      asString () <<
      "', line " << inputLineNumber <<
      endl;
  }
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

string msrHarmonyDegree::harmonyDegreeTypeKindAsString (
  msrHarmonyDegreeTypeKind harmonyDegreeTypeKind)
{
  string result;

  switch (harmonyDegreeTypeKind) {
    case msrHarmonyDegree::kHarmonyDegreeAddType:
      result = "HarmonyDegreeAdd";
      break;
    case msrHarmonyDegree::kHarmonyDegreeAlterType:
      result = "HarmonyDegreeAlter";
      break;
    case msrHarmonyDegree::kHarmonyDegreeSubtractType:
      result = "HarmonyDegreeSubtract";
      break;
  } // switch

  return result;
}

string msrHarmonyDegree::harmonyDegreeKindAsShortString () const
{
  string result;
  
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

void msrHarmonyDegree::acceptIn (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrHarmonyDegree::acceptIn()" <<
      endl;
  }
      
  if (visitor<S_msrHarmonyDegree>*
    p =
      dynamic_cast<visitor<S_msrHarmonyDegree>*> (v)) {
        S_msrHarmonyDegree elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrHarmonyDegree::visitStart()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrHarmonyDegree::acceptOut (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrHarmonyDegree::acceptOut()" <<
      endl;
  }

  if (visitor<S_msrHarmonyDegree>*
    p =
      dynamic_cast<visitor<S_msrHarmonyDegree>*> (v)) {
        S_msrHarmonyDegree elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrHarmonyDegree::visitEnd()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrHarmonyDegree::browseData (basevisitor* v)
{}

ostream& operator<< (ostream& os, const S_msrHarmonyDegree& elt)
{
  elt->print (os);
  return os;
}

void msrHarmonyDegree::print (ostream& os)
{  
  os <<
    asString () <<
    endl;
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

  if (gTraceOptions->fTraceHarmonies) {
    gLogIOstream <<
      "Creating harmony '" <<
      asString () <<
      "'" <<
      endl;
  }

  // handle harmony inversion if any
  /*
    Inversion is a number indicating which inversion is used:
    0 for root position, 1 for first inversion, etc.
  */
  if (fHarmonyInversion > 0) {
    // fetch the chord intervals
    S_msrChordIntervals
      chordIntervals =
        msrChordIntervals::create (
          inputLineNumber,
          fHarmonyKind);

    // fetch the bass chord item for the inversion
    S_msrChordItem
      bassChordItem =
        chordIntervals->
          bassChordItemForChordInversion (
            inputLineNumber,
            fHarmonyInversion);

    // fetch the inverted chord bass semitones pitch
    msrQuarterTonesPitchKind
      invertedChordBassQuarterTonesPitchKind =
        noteAtIntervalFromQuarterTonesPitch (
          inputLineNumber,
          bassChordItem->getChordItemIntervalKind (),
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
  if (gTraceOptions->fTraceHarmonies) {
    gLogIOstream <<
      "Creating a newborn clone of harmony '" <<
      msrHarmonyKindAsShortString (fHarmonyKind) <<
      "', line " << fInputLineNumber <<
      endl;
  }

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
  if (gTraceOptions->fTraceHarmonies) {
    gLogIOstream <<
      "Creating a deep copy of harmony '" <<
      msrHarmonyKindAsShortString (fHarmonyKind) <<
      "', line " << fInputLineNumber <<
      endl;
  }

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
      "% ==> msrHarmony::acceptIn()" <<
      endl;
  }
      
  if (visitor<S_msrHarmony>*
    p =
      dynamic_cast<visitor<S_msrHarmony>*> (v)) {
        S_msrHarmony elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrHarmony::visitStart()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrHarmony::acceptOut (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrHarmony::acceptOut()" <<
      endl;
  }

  if (visitor<S_msrHarmony>*
    p =
      dynamic_cast<visitor<S_msrHarmony>*> (v)) {
        S_msrHarmony elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrHarmony::visitEnd()" <<
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
S_msrFrameNote msrFrameNote::create (
  int              inputLineNumber,
  int              frameNoteStringNumber,
  int              frameNoteFretNumber,
  int              frameNoteFingering,
  msrBarreTypeKind frameNoteBarreTypeKind)
{
  msrFrameNote* o =
    new msrFrameNote (
      inputLineNumber,
      frameNoteStringNumber,
      frameNoteFretNumber,
      frameNoteFingering,
      frameNoteBarreTypeKind);
  assert(o!=0);

  return o;
}

msrFrameNote::msrFrameNote (
  int              inputLineNumber,
  int              frameNoteStringNumber,
  int              frameNoteFretNumber,
  int              frameNoteFingering,
  msrBarreTypeKind frameNoteBarreTypeKind)
    : msrElement (inputLineNumber)
{
  fFrameNoteStringNumber = frameNoteStringNumber;
  fFrameNoteFretNumber   = frameNoteFretNumber;

  fFrameNoteFingering = frameNoteFingering;
  
  fFrameNoteBarreTypeKind = frameNoteBarreTypeKind;

  if (gTraceOptions->fTraceHarmonies) {
    gLogIOstream <<
      "Creating frame note '" <<
      asString () <<
      "'" <<
      endl;
  }
}

msrFrameNote::~msrFrameNote ()
{}

string msrFrameNote::barreTypeKindAsString (
  msrBarreTypeKind barreTypeKind)
{
  string result;
  
  switch (barreTypeKind) {
    case msrFrameNote::k_NoBarreType:
      result = "k_NoBarreType";
      break;
    case msrFrameNote::kBarreTypeStart:
      result = "barreTypeStart";
      break;
    case msrFrameNote::kBarreTypeStop:
      result = "barreTypeStop";
      break;
  } // switch

  return result;
}

string msrFrameNote::asString () const
{
  stringstream s;

  s <<
    "FrameNote" <<
    ", frameNoteStringNumber: " << fFrameNoteStringNumber <<
    ", frameNoteFretNumber: " << fFrameNoteFretNumber <<
    ", frameNoteFingering: " << fFrameNoteFingering <<
    ", frameNoteBarreTypeKind: " <<
    barreTypeKindAsString (
      fFrameNoteBarreTypeKind) <<
    ", line: " << fInputLineNumber;

  return s.str ();
}

void msrFrameNote::acceptIn (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrFrameNote::acceptIn()" <<
      endl;
  }
      
  if (visitor<S_msrFrameNote>*
    p =
      dynamic_cast<visitor<S_msrFrameNote>*> (v)) {
        S_msrFrameNote elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrFrameNote::visitStart()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrFrameNote::acceptOut (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrFrameNote::acceptOut()" <<
      endl;
  }

  if (visitor<S_msrFrameNote>*
    p =
      dynamic_cast<visitor<S_msrFrameNote>*> (v)) {
        S_msrFrameNote elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrFrameNote::visitEnd()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrFrameNote::browseData (basevisitor* v)
{}

ostream& operator<< (ostream& os, const S_msrFrameNote& elt)
{
  elt->print (os);
  return os;
}

void msrFrameNote::print (ostream& os)
{  
  os <<
    "FrameNote" <<
    ", line: " << fInputLineNumber <<
    endl;

  gIndenter++;

  const int fieldWidth = 23;
  
  os <<
    setw (fieldWidth) <<
    "frameNoteStringNumber" << " : " << fFrameNoteStringNumber <<
    endl <<
    setw (fieldWidth) <<
    "frameNoteFretNumber" << " : " << fFrameNoteFretNumber <<
    endl <<
    setw (fieldWidth) <<
    "frameNoteFingering" << " : " << fFrameNoteFingering <<
    endl <<
    setw (fieldWidth) <<
    "frameNoteBarreTypeKind" << " : " << 
    barreTypeKindAsString (
      fFrameNoteBarreTypeKind) <<
    endl;

  gIndenter--;
}

//______________________________________________________________________________
S_msrFrame msrFrame::create (
  int inputLineNumber,
  int frameStringsNumber,
  int frameFretsNumber,
  int frameFirstFretNumber)
{
  msrFrame* o =
    new msrFrame (
      inputLineNumber,
      frameStringsNumber,
      frameFretsNumber,
      frameFirstFretNumber);
  assert(o!=0);

  return o;
}

msrFrame::msrFrame (
  int inputLineNumber,
  int frameStringsNumber,
  int frameFretsNumber,
  int frameFirstFretNumber)
    : msrElement (inputLineNumber)
{
  fFrameStringsNumber   = frameStringsNumber;
  fFrameFretsNumber     = frameFretsNumber;
  fFrameFirstFretNumber = frameFirstFretNumber;
 
  fFrameContainsFingerings = false;
  
  if (gTraceOptions->fTraceHarmonies) {
    gLogIOstream <<
      "Creating frame '" <<
      asString () <<
      "'" <<
      endl;
  }
}

msrFrame::~msrFrame ()
{}

void msrFrame::appendFrameNoteToFrame (
  S_msrFrameNote frameNote)
{
  int inputLineNumber =
    frameNote->getInputLineNumber ();
    
  fFrameFrameNotesList.push_back (
    frameNote);

  // handle nested barre start/stop
  switch (frameNote->getFrameNoteBarreTypeKind ()) {
    case msrFrameNote::k_NoBarreType:
      break;
      
    case msrFrameNote::kBarreTypeStart:
      fPendingBarreStartFrameNotes.push (
        frameNote);
      break;
      
    case msrFrameNote::kBarreTypeStop:
      {
        if (! fPendingBarreStartFrameNotes.size ()) {
          stringstream s;
      
          s <<
            "frame note with barre stop has no maching barre start" <<
            frameNote;
            
          msrMusicXMLError (
            gXml2lyOptions->fInputSourceName,
            inputLineNumber,
            __FILE__, __LINE__,
            s.str ());
        }
        
        else {
          S_msrFrameNote
            pendingBarreStartFrameNotesTop =
              fPendingBarreStartFrameNotes.top ();

          int
            barreStartFretNumber =
              pendingBarreStartFrameNotesTop->
                getFrameNoteFretNumber (),
            barreStopFretNumber =
              frameNote->
                getFrameNoteFretNumber ();

          if (barreStartFretNumber != barreStopFretNumber) {
            stringstream s;
        
            s <<
              "frame note with barre stop has is at fret" <<
              barreStopFretNumber <<
              "while the matching barre start is at fret" <<
              barreStartFretNumber;
              
            msrMusicXMLError (
              gXml2lyOptions->fInputSourceName,
              inputLineNumber,
              __FILE__, __LINE__,
              s.str ());
          }
  
          fFrameBarresList.push_back (
            msrBarre (
              pendingBarreStartFrameNotesTop->
                getFrameNoteStringNumber (),
              frameNote->
                getFrameNoteStringNumber (),
              barreStartFretNumber));
              
          fPendingBarreStartFrameNotes.pop ();
        }
      }
      break;
  } // switch

  // are there fingerings in this frame?
  if (frameNote->getFrameNoteFingering () != -1) {
    fFrameContainsFingerings = true;
  }
}

string msrFrame::asString () const
{
  stringstream s;

  s <<
    "Frame" <<
    ", frameStringsNumber: " << fFrameStringsNumber <<
    ", frameFretsNumber: " << fFrameFretsNumber <<
    ", frameFirstFretNumber: " << fFrameFirstFretNumber <<
    singularOrPlural (
      fFrameFrameNotesList.size (), "frame note", "frame notes") <<
    singularOrPlural (
      fFrameBarresList.size (), "barre", "barres") <<
    ", frameContainsFingerings: " <<
    booleanAsString (
      fFrameContainsFingerings);

  return s.str ();
}

void msrFrame::acceptIn (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrFrame::acceptIn()" <<
      endl;
  }
      
  if (visitor<S_msrFrame>*
    p =
      dynamic_cast<visitor<S_msrFrame>*> (v)) {
        S_msrFrame elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrFrame::visitStart()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrFrame::acceptOut (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrFrame::acceptOut()" <<
      endl;
  }

  if (visitor<S_msrFrame>*
    p =
      dynamic_cast<visitor<S_msrFrame>*> (v)) {
        S_msrFrame elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrFrame::visitEnd()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrFrame::browseData (basevisitor* v)
{
  // browse frame notes if any
  if (fFrameFrameNotesList.size ())
    for (
      list<S_msrFrameNote>::const_iterator i = fFrameFrameNotesList.begin ();
      i != fFrameFrameNotesList.end ();
      i++) {
      // browse the harmony degree
      msrBrowser<msrFrameNote> browser (v);
      browser.browse (*(*i));
    } // for
}

ostream& operator<< (ostream& os, const S_msrFrame& elt)
{
  elt->print (os);
  return os;
}

void msrFrame::print (ostream& os)
{  
  os <<
    "Frame" <<
     ", line " << fInputLineNumber <<
    endl;
    
  gIndenter++;

  const int fieldWidth = 15;

  os << left <<
    setw (fieldWidth) <<
    "frameStringsNumber" << " = " << fFrameStringsNumber <<
    endl <<
    setw (fieldWidth) <<
    "frameFretsNumber" << " = " << fFrameFretsNumber <<
    endl <<
    setw (fieldWidth) <<
    "frameFirstFretNumber" << " = " << fFrameFirstFretNumber <<
    endl <<
    setw (fieldWidth) <<
    "frameContainsFingerings" << " = " << fFrameContainsFingerings <<
    endl;

  // print frame notes if any
  os <<
    "Frame notes:";
  if (fFrameFrameNotesList.size ()) {
    os <<
      endl;

    gIndenter++;
    
    list<S_msrFrameNote>::const_iterator
      iBegin = fFrameFrameNotesList.begin (),
      iEnd   = fFrameFrameNotesList.end (),
      i      = iBegin;
      
    for ( ; ; ) {
      os << (*i);
      if (++i == iEnd) break;
      // no endl here;
    } // for
    
    gIndenter--;
  }
  else {
    os <<
      "none" <<
      endl;
  }

  // print the barres if any
  os <<
    "Frame barres:";
  if (fFrameBarresList.size ()) {
    os <<
      endl;

    gIndenter++;
    
    list<msrBarre>::const_iterator
      iBegin = fFrameBarresList.begin (),
      iEnd   = fFrameBarresList.end (),
      i      = iBegin;

    for ( ; ; ) {
      msrBarre barre = (*i);
      
      os <<
        "Barre" <<
        endl;

      gIndenter++;

      const int fieldWidth = 21;
      
      os << left <<
        setw (fieldWidth) <<
        "barreStartString" << " : " << barre.fBarreStartString <<
        endl <<
        setw (fieldWidth) <<
        "barreStopString" << " : " << barre.fBarreStopString <<
        endl <<
        setw (fieldWidth) <<
        "barreFretNumber" << " : " << barre.fBarreFretNumber <<
        endl;

      gIndenter--;
      
      if (++i == iEnd) break;
      // no endl here;
    } // for
    
    gIndenter--;
  }
  else {
    os <<
      "none" <<
      endl;
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
 
  if (gTraceOptions->fTraceFiguredBass) {
    gLogIOstream <<
      "Creating figure '" <<
      asString () <<
      "'" <<
      endl;
  }
}

msrFigure::~msrFigure()
{}

S_msrFigure msrFigure::createFigureNewbornClone (
  S_msrPart containingPart)
{
  if (gTraceOptions->fTraceFiguredBass) {
    gLogIOstream <<
      "Creating a newborn clone of figure '" <<
      asString () <<
      "'" <<
      endl;
  }

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
  if (gTraceOptions->fTraceFiguredBass) {
    gLogIOstream <<
      "Creating a deep copy of figure '" <<
      asString () <<
      "'" <<
      endl;
  }

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
      "% ==> msrFigure::acceptIn()" <<
      endl;
  }
      
  if (visitor<S_msrFigure>*
    p =
      dynamic_cast<visitor<S_msrFigure>*> (v)) {
        S_msrFigure elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrFigure::visitStart()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrFigure::acceptOut (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrFigure::acceptOut()" <<
      endl;
  }

  if (visitor<S_msrFigure>*
    p =
      dynamic_cast<visitor<S_msrFigure>*> (v)) {
        S_msrFigure elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrFigure::visitEnd()" <<
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
    
  if (gTraceOptions->fTraceFiguredBass) {
    gLogIOstream <<
      "Creating figuredBass '" <<
      asString () <<
      "'" <<
      endl;
  }
}

msrFiguredBass::~msrFiguredBass()
{}

S_msrFiguredBass msrFiguredBass::createFiguredBassNewbornClone (
  S_msrPart containingPart)
{
  if (gTraceOptions->fTraceFiguredBass) {
    gLogIOstream <<
      "Creating a newborn clone of figuredBass '" <<
      asString () <<
      "'" <<
      endl;
  }

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
  if (gTraceOptions->fTraceFiguredBass) {
    gLogIOstream <<
      "Creating a deep copy of figuredBass '" <<
      asString () <<
      "'" <<
      endl;
  }

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
  if (gTraceOptions->fTraceFiguredBass) {
    gLogIOstream <<
      "Appending figure'" << figure->asString () <<
      "' to figuredBass '" <<
      asString () <<
      "'" <<
      endl;
  }

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
      "% ==> msrFiguredBass::acceptIn()" <<
      endl;
  }
      
  if (visitor<S_msrFiguredBass>*
    p =
      dynamic_cast<visitor<S_msrFiguredBass>*> (v)) {
        S_msrFiguredBass elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrFiguredBass::visitStart()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrFiguredBass::acceptOut (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrFiguredBass::acceptOut()" <<
      endl;
  }

  if (visitor<S_msrFiguredBass>*
    p =
      dynamic_cast<visitor<S_msrFiguredBass>*> (v)) {
        S_msrFiguredBass elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrFiguredBass::visitEnd()" <<
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
S_msrSegno msrSegno::create (
  int                       inputLineNumber)
{
  msrSegno* o =
    new msrSegno (
      inputLineNumber);
  assert(o!=0);
  return o;
}

msrSegno::msrSegno (
  int                       inputLineNumber)
    : msrElement (inputLineNumber)
{}

msrSegno::~msrSegno()
{}

void msrSegno::acceptIn (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrSegno::acceptIn()" <<
      endl;
  }
      
  if (visitor<S_msrSegno>*
    p =
      dynamic_cast<visitor<S_msrSegno>*> (v)) {
        S_msrSegno elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrSegno::visitStart()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrSegno::acceptOut (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrSegno::acceptOut()" <<
      endl;
  }

  if (visitor<S_msrSegno>*
    p =
      dynamic_cast<visitor<S_msrSegno>*> (v)) {
        S_msrSegno elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrSegno::visitEnd()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrSegno::browseData (basevisitor* v)
{}

ostream& operator<< (ostream& os, const S_msrSegno& elt)
{
  elt->print (os);
  return os;
}

void msrSegno::print (ostream& os)
{
  os <<
    "Segno" <<
    ", line " << fInputLineNumber <<
    endl;
}

//______________________________________________________________________________
S_msrCoda msrCoda::create (
  int                       inputLineNumber)
{
  msrCoda* o =
    new msrCoda (
      inputLineNumber);
  assert(o!=0);
  return o;
}

msrCoda::msrCoda (
  int                       inputLineNumber)
    : msrElement (inputLineNumber)
{}

msrCoda::~msrCoda()
{}

void msrCoda::acceptIn (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrCoda::acceptIn()" <<
      endl;
  }
      
  if (visitor<S_msrCoda>*
    p =
      dynamic_cast<visitor<S_msrCoda>*> (v)) {
        S_msrCoda elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrCoda::visitStart()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrCoda::acceptOut (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrCoda::acceptOut()" <<
      endl;
  }

  if (visitor<S_msrCoda>*
    p =
      dynamic_cast<visitor<S_msrCoda>*> (v)) {
        S_msrCoda elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrCoda::visitEnd()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrCoda::browseData (basevisitor* v)
{}

ostream& operator<< (ostream& os, const S_msrCoda& elt)
{
  elt->print (os);
  return os;
}

void msrCoda::print (ostream& os)
{
  os <<
    "Coda" <<
    ", line " << fInputLineNumber <<
    endl;
}

//______________________________________________________________________________
S_msrEyeGlasses msrEyeGlasses::create (
  int inputLineNumber)
{
  msrEyeGlasses* o =
    new msrEyeGlasses (
      inputLineNumber);
  assert(o!=0);
  return o;
}

msrEyeGlasses::msrEyeGlasses (
  int inputLineNumber)
    : msrElement (inputLineNumber)
{}

msrEyeGlasses::~msrEyeGlasses ()
{}

void msrEyeGlasses::acceptIn (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrEyeGlasses::acceptIn()" <<
      endl;
  }
      
  if (visitor<S_msrEyeGlasses>*
    p =
      dynamic_cast<visitor<S_msrEyeGlasses>*> (v)) {
        S_msrEyeGlasses elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrEyeGlasses::visitStart()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrEyeGlasses::acceptOut (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrEyeGlasses::acceptOut()" <<
      endl;
  }

  if (visitor<S_msrEyeGlasses>*
    p =
      dynamic_cast<visitor<S_msrEyeGlasses>*> (v)) {
        S_msrEyeGlasses elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrEyeGlasses::visitEnd()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrEyeGlasses::browseData (basevisitor* v)
{}

ostream& operator<< (ostream& os, const S_msrEyeGlasses& elt)
{
  elt->print (os);
  return os;
}

void msrEyeGlasses::print (ostream& os)
{
  os <<
    "EyeGlasses" <<
    ", line " << fInputLineNumber <<
    endl;
}

//______________________________________________________________________________
S_msrPedal msrPedal::create (
  int              inputLineNumber,
  msrPedalTypeKind pedalTypeKind,
  msrPedalLineKind pedalLineKind,
  msrPedalSignKind pedalSignKind)
{
  msrPedal* o =
    new msrPedal (
      inputLineNumber,
      pedalTypeKind, pedalLineKind, pedalSignKind);
  assert(o!=0);
  return o;
}

msrPedal::msrPedal (
  int              inputLineNumber,
  msrPedalTypeKind pedalTypeKind,
  msrPedalLineKind pedalLineKind,
  msrPedalSignKind pedalSignKind)
    : msrElement (inputLineNumber)
{
  fPedalTypeKind = pedalTypeKind;
  fPedalLineKind = pedalLineKind;
  fPedalSignKind = pedalSignKind;
}

msrPedal::~msrPedal()
{}

string msrPedal::pedalTypeAsString ()
{
  string result;
  
  switch (fPedalTypeKind) {
    case kPedalStart:
      result = "start pedal";
      break;
    case kPedalContinue:
      result = "continue pedal";
      break;
    case kPedalChange:
      result = "change pedal";
      break;
    case kPedalStop:
      result = "stop pedal";
      break;
  } // switch

  return result;
}
      
string msrPedal::pedalLineAsString ()
{
  string result;
  
  switch (fPedalLineKind) {
    case msrPedal::kPedalLineYes:
      result = "pedalLineYes";
      break;
    case msrPedal::kPedalLineNo:
      result = "pedalLineNo";
      break;
  } // switch

  return result;
}

string msrPedal::pedalSignAsString ()
{
  string result;
  
  switch (fPedalSignKind) {
    case msrPedal::kPedalSignYes:
      result = "pedalSignYes";
      break;
    case msrPedal::kPedalSignNo:
      result = "pedalSignNo";
      break;
  } // switch

  return result;
}

void msrPedal::acceptIn (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrPedal::acceptIn()" <<
      endl;
  }
      
  if (visitor<S_msrPedal>*
    p =
      dynamic_cast<visitor<S_msrPedal>*> (v)) {
        S_msrPedal elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrPedal::visitStart()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrPedal::acceptOut (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrPedal::acceptOut()" <<
      endl;
  }

  if (visitor<S_msrPedal>*
    p =
      dynamic_cast<visitor<S_msrPedal>*> (v)) {
        S_msrPedal elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrPedal::visitEnd()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrPedal::browseData (basevisitor* v)
{}

ostream& operator<< (ostream& os, const S_msrPedal& elt)
{
  elt->print (os);
  return os;
}

void msrPedal::print (ostream& os)
{
  os <<
    "Pedal" <<
    ", line " << fInputLineNumber <<
    pedalTypeAsString () <<
    ", " <<
    pedalLineAsString () <<
    ", " <<
    pedalSignAsString () <<
    endl;
}

//______________________________________________________________________________
S_msrDamp msrDamp::create (
  int inputLineNumber)
{
  msrDamp* o =
    new msrDamp (
      inputLineNumber);
  assert(o!=0);
  return o;
}

msrDamp::msrDamp (
  int inputLineNumber)
    : msrElement (inputLineNumber)
{}

msrDamp::~msrDamp ()
{}

void msrDamp::acceptIn (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrDamp::acceptIn()" <<
      endl;
  }
      
  if (visitor<S_msrDamp>*
    p =
      dynamic_cast<visitor<S_msrDamp>*> (v)) {
        S_msrDamp elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrDamp::visitStart()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrDamp::acceptOut (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrDamp::acceptOut()" <<
      endl;
  }

  if (visitor<S_msrDamp>*
    p =
      dynamic_cast<visitor<S_msrDamp>*> (v)) {
        S_msrDamp elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrDamp::visitEnd()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrDamp::browseData (basevisitor* v)
{}

ostream& operator<< (ostream& os, const S_msrDamp& elt)
{
  elt->print (os);
  return os;
}

void msrDamp::print (ostream& os)
{
  os <<
    "Damp" <<
    ", line " << fInputLineNumber <<
    endl;
}

//______________________________________________________________________________
S_msrDampAll msrDampAll::create (
  int inputLineNumber)
{
  msrDampAll* o =
    new msrDampAll (
      inputLineNumber);
  assert(o!=0);
  return o;
}

msrDampAll::msrDampAll (
  int inputLineNumber)
    : msrElement (inputLineNumber)
{}

msrDampAll::~msrDampAll ()
{}

void msrDampAll::acceptIn (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrDampAll::acceptIn()" <<
      endl;
  }
      
  if (visitor<S_msrDampAll>*
    p =
      dynamic_cast<visitor<S_msrDampAll>*> (v)) {
        S_msrDampAll elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrDampAll::visitStart()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrDampAll::acceptOut (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrDampAll::acceptOut()" <<
      endl;
  }

  if (visitor<S_msrDampAll>*
    p =
      dynamic_cast<visitor<S_msrDampAll>*> (v)) {
        S_msrDampAll elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrDampAll::visitEnd()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrDampAll::browseData (basevisitor* v)
{}

ostream& operator<< (ostream& os, const S_msrDampAll& elt)
{
  elt->print (os);
  return os;
}

void msrDampAll::print (ostream& os)
{
  os <<
    "DampAll" <<
    ", line " << fInputLineNumber <<
    endl;
}

//______________________________________________________________________________
S_msrBarline msrBarline::create (
  int                           inputLineNumber,
  msrBarlineHasSegnoKind        barlineHasSegnoKind,
  msrBarlineHasCodaKind         barlineHasCodaKind,
  msrBarlineLocationKind        locationKind,
  msrBarlineStyleKind           styleKind,
  msrBarlineEndingTypeKind      endingTypeKind,
  string                        endingNumber,
  msrBarlineRepeatDirectionKind repeatDirectionKind,
  msrBarlineRepeatWingedKind    repeatWingedKind,
  int                           barlineTimes)
{
  msrBarline* o =
    new msrBarline (
      inputLineNumber,
      barlineHasSegnoKind, barlineHasCodaKind,
      locationKind, styleKind,
      endingTypeKind, endingNumber,
      repeatDirectionKind, repeatWingedKind,
      barlineTimes);
  assert(o!=0);
  return o;
}

msrBarline::msrBarline (
  int                           inputLineNumber,
  msrBarlineHasSegnoKind        barlineHasSegnoKind,
  msrBarlineHasCodaKind         barlineHasCodaKind,
  msrBarlineLocationKind        locationKind,
  msrBarlineStyleKind           styleKind,
  msrBarlineEndingTypeKind      endingTypeKind,
  string                        endingNumber,
  msrBarlineRepeatDirectionKind repeatDirectionKind,
  msrBarlineRepeatWingedKind    repeatWingedKind,
  int                           barlineTimes)
    : msrElement (inputLineNumber)
{
  fBarlineHasSegnoKind = barlineHasSegnoKind;
  fBarlineHasCodaKind  = barlineHasCodaKind;
  
  fLocationKind        = locationKind;
  fStyleKind           = styleKind;
  fEndingTypeKind      = endingTypeKind;
  fEndingNumber        = endingNumber;
  fRepeatDirectionKind = repeatDirectionKind;
  fRepeatWingedKind    = repeatWingedKind;

  fBarlineTimes = barlineTimes;
  
  // JMI gLogIOstream << "fEndingNumber = " << fEndingNumber << endl;
  
  // extract individual numbers from fEndingNumber
  // that may contain "1, 2"
  fEndingNumbersList =
    extractNumbersFromString (
      fEndingNumber, false); // 'true' to debug it
}

msrBarline::~msrBarline()
{}

void msrBarline::acceptIn (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrBarline::acceptIn()" <<
      endl;
  }
      
  if (visitor<S_msrBarline>*
    p =
      dynamic_cast<visitor<S_msrBarline>*> (v)) {
        S_msrBarline elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrBarline::visitStart()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrBarline::acceptOut (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrBarline::acceptOut()" <<
      endl;
  }

  if (visitor<S_msrBarline>*
    p =
      dynamic_cast<visitor<S_msrBarline>*> (v)) {
        S_msrBarline elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrBarline::visitEnd()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrBarline::browseData (basevisitor* v)
{}

ostream& operator<< (ostream& os, const S_msrBarline& elt)
{
  elt->print (os);
  return os;
}

string msrBarline::barlineLocationKindAsString (
  msrBarlineLocationKind barlineLocationKind)
{
  string result;
  
  switch (barlineLocationKind) {
    case k_NoBarlineLocation:
      result = "noBarlineLocation";
      break;
    case kBarlineLocationLeft:
      result = "barlineLocationLeft";
      break;
    case kBarlineLocationMiddle:
      result = "barlineLocationMiddle";
      break;
    case kBarlineLocationRight:
      result = "barlineLocationRight";
      break;
  } // switch

  return result;
}

string msrBarline::barlineCategoryKindAsString (
  msrBarlineCategoryKind barlineCategoryKind)
{
  string result;
  
  switch (barlineCategoryKind) {
    case kBarlineCategoryStandalone:
      result = "barlineCategoryStandalone";
      break;
    case kBarlineCategoryRepeatStart:
      result = "barlineCategoryRepeatStart";
      break;
    case kBarlineCategoryRepeatEnd:
      result = "barlineCategoryRepeatEnd";
      break;
    case kBarlineCategoryHookedEndingStart:
      result = "barlineCategoryHookedEndingStart";
      break;
    case kBarlineCategoryHookedEndingEnd:
      result = "kBarlineCategoryHookedEndingEnd";
      break;
    case kBarlineCategoryHooklessEndingStart:
      result = "barlineCategoryHooklessEndingStart";
      break;
    case kBarlineCategoryHooklessEndingEnd:
      result = "barlineCategoryHooklessEndingEnd";
      break;
  } // switch

  return result;
}

string msrBarline::barlineHasSegnoKindAsString (
  msrBarlineHasSegnoKind barlineHasSegnoKind)
{
  string result;

  switch (barlineHasSegnoKind) {
    case msrBarline::kBarlineHasSegnoYes:
      result = "barlineHasSegnoYes";
      break;
    case msrBarline::kBarlineHasSegnoNo:
      result = "barlineHasSegnoNo";
      break;
  } // switch

  return result;
}

string msrBarline::barlineHasCodaKindAsString (
  msrBarlineHasCodaKind barlineHasCodaKind)
{
  string result;

  switch (barlineHasCodaKind) {
    case msrBarline::kBarlineHasCodaYes:
      result = "barlineHasCodaYes";
      break;
    case msrBarline::kBarlineHasCodaNo:
      result = "barlineHasCodaNo";
      break;
  } // switch

  return result;
}

string msrBarline::barlineStyleKindAsString (
  msrBarlineStyleKind barlineStyleKind)
{
  string result;
  
  switch (barlineStyleKind) {
    case msrBarline::k_NoBarlineStyle:
      result = "NoBarlineStyle";
      break;
    case msrBarline::kBarlineStyleRegular:
      result = "barlineStyleRegular";
      break;
    case msrBarline::kBarlineStyleDotted:
      result = "barlineStyleDotted";
      break;
    case msrBarline::kBarlineStyleDashed:
      result = "barlineStyleDashed";
      break;
    case msrBarline::kBarlineStyleHeavy:
      result = "barlineStyleHeavy";
      break;
    case msrBarline::kBarlineStyleLightLight:
      result = "barlineStyleLightLight";
      break;
    case msrBarline::kBarlineStyleLightHeavy:
      result = "barlineStyleLightHeavy";
      break;
    case msrBarline::kBarlineStyleHeavyLight:
      result = "barlineStyleHeavyLight";
      break;
    case msrBarline::kBarlineStyleHeavyHeavy:
      result = "barlineStyleHeavyHeavy";
      break;
    case msrBarline::kBarlineStyleTick:
      result = "barlineStyleTick";
      break;
    case msrBarline::kBarlineStyleShort:
      result = "barlineStyleShort";
      break;
    case msrBarline::kBarlineStyleNone:
      result = "barlineStyleNone";
      break;
  } // switch

  return result;
}

string msrBarline::barlineEndingTypeKindAsString (
  msrBarlineEndingTypeKind barlineEndingTypeKind)
{
  string result;
  
  switch (barlineEndingTypeKind) {
    case msrBarline::k_NoBarlineEnding:
      result = "noBarlineEnding";
      break;
    case msrBarline::kBarlineEndingTypeStart:
      result = "barlineEndingTypeStart";
      break;
    case msrBarline::kBarlineEndingTypeStop:
      result = "barlineEndingTypeStop";
      break;
    case msrBarline::kBarlineEndingTypeDiscontinue:
      result = "barlineEndingTypeDiscontinue";
      break;
  } // switch

  return result;
}

string msrBarline::barlineRepeatDirectionKindAsString (
  msrBarlineRepeatDirectionKind barlineRepeatDirectionKind)
{
  string result;
  
  switch (barlineRepeatDirectionKind) {
    case msrBarline::k_NoBarlineRepeatDirection:
      result ="noBarlineRepeatDirection";
      break;
    case msrBarline::kBarlineRepeatDirectionForward:
      result ="barlineRepeatDirectionForward";
      break;
    case msrBarline::kBarlineRepeatDirectionBackward:
      result ="barlineRepeatDirectionBackward";
      break;
  } // switch

  return result;
}

string msrBarline::barlineRepeatWingedKindAsString (
  msrBarlineRepeatWingedKind barlineRepeatWingedKind)
{
  string result;
  
  switch (barlineRepeatWingedKind) {
    case msrBarline::k_NoBarlineRepeatWinged:
      result = "noBarlineRepeatWinged";
      break;
    case msrBarline::kBarlineRepeatWingedNone:
      result = "barlineRepeatWingedNone";
      break;
    case msrBarline::kBarlineRepeatWingedStraight:
      result = "barlineRepeatWingedStraight";
      break;
    case msrBarline::kBarlineRepeatWingedCurved:
      result = "barlineRepeatWingedCurved";
      break;
    case msrBarline::kBarlineRepeatWingedDoubleStraight:
      result = "barlineRepeatWingedDoubleStraight";
      break;
    case msrBarline::kBarlineRepeatWingedDoubleCurved:
      result = "barlineRepeatWingedDoubleCurved";
      break;
  } // switch

  return result;
}

string msrBarline::endingNumbersListAsString () const
{
  stringstream s;

  if (fEndingNumbersList.size ()) {
    list<int>::const_iterator
      iBegin = fEndingNumbersList.begin (),
      iEnd   = fEndingNumbersList.end (),
      i      = iBegin;
    for ( ; ; ) {
      s << (*i);
      if (++i == iEnd) break;
      s << " ";
    } // for
  }

  return s.str ();
}

string msrBarline::asString () const
{
  stringstream s;

  s <<
    "Barline " <<
    barlineCategoryKindAsString (fBarlineCategoryKind) <<
    ", " <<
    barlineHasSegnoKindAsString (
      fBarlineHasSegnoKind) <<
    ", " <<
    barlineHasCodaKindAsString (
      fBarlineHasCodaKind) <<
    
    ", " <<
    barlineLocationKindAsString (fLocationKind) <<
    ", " <<
    barlineStyleKindAsString (fStyleKind) <<

    ", " <<
    barlineEndingTypeKindAsString (fEndingTypeKind) <<
    ", " <<
    endingNumbersListAsString () <<

    ", " <<
    barlineRepeatDirectionKindAsString (fRepeatDirectionKind) <<
    ", " <<
    barlineRepeatWingedKindAsString (fRepeatWingedKind) <<
    
    ", " <<
    fBarlineTimes << " times" <<
    ", line " << fInputLineNumber;
    
  return s.str ();
}

void msrBarline::print (ostream& os)
{
  os <<
    "Barline, " <<
    barlineCategoryKindAsString (
      fBarlineCategoryKind) <<
    ", " <<
    barlineHasSegnoKindAsString (
      fBarlineHasSegnoKind) <<
    ", " <<
    barlineHasCodaKindAsString (
      fBarlineHasCodaKind) <<
    ", line " << fInputLineNumber <<
    endl;

  gIndenter++;

  const int fieldWidth = 15;

  os << left <<
    setw (fieldWidth) <<
    "location" << " : " <<
    barlineLocationKindAsString (fLocationKind) <<
    endl <<
    setw (fieldWidth) <<
    "style" << " : " <<
    barlineStyleKindAsString (fStyleKind) <<
    endl <<

    setw (fieldWidth) <<
    "endingType" << " : " <<
    barlineEndingTypeKindAsString (fEndingTypeKind) <<
    endl <<
    setw (fieldWidth) <<
    "ending number" << " : " <<
    endingNumbersListAsString () <<
    endl <<
 
    setw (fieldWidth) <<
    "repeatDirection" << " : " <<
    barlineRepeatDirectionKindAsString (fRepeatDirectionKind) <<
    endl <<
  
    setw (fieldWidth) <<
    "repeatWinged" << " : " <<
    barlineRepeatWingedKindAsString (fRepeatWingedKind) <<
    endl <<
  
    setw (fieldWidth) <<
    "barlineTimes" << " : " <<
    fBarlineTimes <<
    endl;
     
  gIndenter--;
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
  if (gTraceOptions->fTraceMeasures) {
    gLogIOstream <<
      "Initializing measure " << fMeasureNumber <<
      " in segment " <<
      fMeasureSegmentUplink->getSegmentAbsoluteNumber () <<
      " in voice \"" <<
      fMeasureSegmentUplink->
        getSegmentVoiceUplink ()->
          getVoiceName () <<
      "\"" <<
      ", line " << fInputLineNumber <<
      endl;
  }

  // measure kind
  fMeasureKind = kUnknownMeasureKind;

  // measure 'first in segment' kind
  fMeasureFirstInSegmentKind = kMeasureFirstInSegmentNo;

  // measure 'created after a repeat' kind
  fMeasureCreatedAfterARepeatKind = kMeasureCreatedAfterARepeatNo;

  // single-measure rest?
  fMeasureIsASingleMeasureRest = false;
  
  // initialize measure position
  setMeasureLength (
    fInputLineNumber,
    rational (0, 1)); // ready to receive the first note

  // measure doesn't contain music yet
  fMeasureContainsMusic = false;
}

msrMeasure::~msrMeasure()
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
  newbornClone->fFullMeasureLength =
    fFullMeasureLength;
    
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
  newbornClone->fMeasureCreatedAfterARepeatKind =
    fMeasureCreatedAfterARepeatKind;

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
  measureDeepCopy->fFullMeasureLength =
    fFullMeasureLength;
    
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
  measureDeepCopy->fMeasureCreatedAfterARepeatKind =
    fMeasureCreatedAfterARepeatKind;

  // elements

  int numberOfMeasureElements =
    fMeasureElementsList.size ();

  if (numberOfMeasureElements) {
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
  }

  // uplinks

  // fMeasureSegmentUplink JMI ???
  
  return measureDeepCopy;
}

void msrMeasure::setNextMeasureNumber (string nextMeasureNumber)
{
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

  fNextMeasureNumber = nextMeasureNumber;
}

string msrMeasure::fullMeasureLengthAsMSRString ()
{
  return
    wholeNotesAsMsrString (
      fInputLineNumber,
      fFullMeasureLength);
}

void msrMeasure::setMeasureLength (
  int      inputLineNumber,
  rational measureLength)
{
  // rationalise the measure length
  rational rationalisedMeasureLength = measureLength;
  rationalisedMeasureLength.rationalise ();
  
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

void msrMeasure::appendCommentToMeasure (S_msrComment comment)
{
  if (gTraceOptions->fTraceClefs || gTraceOptions->fTraceMeasures) {
    gLogIOstream <<
      "Appending comment '" << comment->asString () <<
      "' to measure " << fMeasureNumber <<
      ", in voice \"" <<
      fetchMeasureVoiceUplink ()->getVoiceName () <<
      "\"" <<
      endl;
  }
          
  // append it to the measure elements list
  fMeasureElementsList.push_back (comment);
}

void msrMeasure::appendClefToMeasure (S_msrClef clef)
{
  if (gTraceOptions->fTraceClefs || gTraceOptions->fTraceMeasures) {
    gLogIOstream <<
      "Appending clef '" << clef->asString () <<
      "' to measure " << fMeasureNumber <<
      ", in voice \"" <<
      fetchMeasureVoiceUplink ()->getVoiceName () <<
      "\"" <<
      endl;
  }
          
  // append it to the measure elements list
  fMeasureElementsList.push_back (clef);
}

void msrMeasure::appendKeyToMeasure (S_msrKey key)
{
  if (gTraceOptions->fTraceKeys || gTraceOptions->fTraceMeasures) {
    gLogIOstream <<
      "Appending key '" << key->asString () <<
      "' to measure " << fMeasureNumber <<
      ", in voice \"" <<
      fetchMeasureVoiceUplink ()->getVoiceName () <<
      "\"" <<
      endl;
  }
          
  // append it to the measure elements list
  fMeasureElementsList.push_back (key);
}

void msrMeasure::appendTimeToMeasure (S_msrTime time)
{
  // sanity check
  msrAssert(
    time != nullptr,
    "time is null");

  if (gTraceOptions->fTraceTimes || gTraceOptions->fTraceMeasures) {
    gLogIOstream <<
      "Appending time:" <<
      endl;

    gIndenter++;

    gLogIOstream <<
      time;

    gIndenter--;

    gLogIOstream <<
      "to measure '" << fMeasureNumber <<
      "' in voice \"" <<
      fMeasureSegmentUplink->
        getSegmentVoiceUplink ()->
          getVoiceName () <<
      "\"" <<
      endl;
  }
    
  // append time to the measure elements list
  fMeasureElementsList.push_back (time);

  // set the measure whole notes per full measure
  setFullMeasureLengthFromTime (
    time);
    
/* JMI
  if (time->getTimeSymbolKind () == msrTime::kTimeSymbolSenzaMisura) {
    
    // this measure is senza misura
    
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

    setMeasureKind (
      kSenzaMisuraMeasureKind);
    
    fFullMeasureLength = INT_MAX; // JMI
  }
  
  else {
    
    // this measure is con misura
    
    int partDivisionsPerQuarterNote =
      fetchMeasurePartUplink->
        getPartDivisionsPerQuarterNote ();
  
    rational
      wholeNotesPerMeasure =
        time->wholeNotesPerMeasure ();
        
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
    
    fFullMeasureLength =
      wholeNotesPerMeasure.getNumerator ()
        *
      partDivisionsPerQuarterNote * 4 // hence a whole note
        /
      wholeNotesPerMeasure.getDenominator ();
    
  
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
          fFullMeasureLength,
          "wholeNotes per full measure",
          "division per full measure") <<
        endl;
  }
  */
}

void msrMeasure::appendTimeToMeasureClone (S_msrTime time)
{
  // sanity check
  msrAssert(
    time != nullptr,
    "time is null");

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
    
  // append time to the measure elements list
  fMeasureElementsList.push_back (time);
}

void msrMeasure::setFullMeasureLengthFromTime (
  S_msrTime time)
{
  // sanity check
  msrAssert(
    time != nullptr,
    "time is null");

  if (
    gTraceOptions->fTraceDivisions
      ||
    gTraceOptions->fTraceTimes
      ||
    gTraceOptions->fTraceMeasures) {
    gLogIOstream <<
      "Setting measure full measure length from time" <<
      ", line " << fInputLineNumber <<
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
      endl;
  }

  if (time->getTimeSymbolKind () == msrTime::kTimeSymbolSenzaMisura) {
    
    // this measure is senza misura
    
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

    setMeasureKind (
      kSenzaMisuraMeasureKind);
    
    fFullMeasureLength =
      rational (INT_MAX, 1);
  }
  
  else {
    
    // this measure is con misura
    
    rational
      wholeNotesPerMeasure =
        time->
          wholeNotesPerMeasure ();
        
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

    // set full measure length
    fFullMeasureLength =
      wholeNotesPerMeasure;

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
        fFullMeasureLength <<
        " whole notes" <<
        endl;
    }
  }
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
  // the measure may have to be padded
  padUpToPartMeasureLengthHighTide (
    barline->getInputLineNumber ());
    
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

  // determine if the note occupies a full measure
  if (noteSoundingWholeNotes == fFullMeasureLength)
    note->
      setNoteOccupiesAFullMeasure ();
    
  // append the note to the measure elements list
// JMI  // only now to make it possible to remove it afterwards
  // if it happens to be the first note of a chord
  fMeasureElementsList.push_back (note);
  
  // register note as the last one in this measure
  fMeasureLastHandledNote = note;

  // this measure contains music
  fMeasureContainsMusic = true;
}

void msrMeasure::appendNoteToMeasureClone (S_msrNote note)
{
  int inputLineNumber =
    note->getInputLineNumber ();

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
  
    // determine if the note occupies a full measure
    if (noteSoundingWholeNotes == fFullMeasureLength)
      note->
        setNoteOccupiesAFullMeasure ();
      
    // append the note to the measure elements list
  // JMI  // only now to make it possible to remove it afterwards
    // if it happens to be the first note of a chord
    fMeasureElementsList.push_back (note);

    // register note as the last one in this measure
    fMeasureLastHandledNote = note;

  // this measure contains music
  fMeasureContainsMusic = true;
 // JMI }
}

void msrMeasure::appendDoubleTremoloToMeasure (
  S_msrDoubleTremolo doubleTremolo)
{
  int inputLineNumber =
    doubleTremolo->getInputLineNumber ();
  
  // populate measure uplink
  doubleTremolo->setDoubleTremoloMeasureUplink (this);

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
}

void msrMeasure::appendHarmonyToMeasure (S_msrHarmony harmony)
{
  int inputLineNumber =
    harmony->getInputLineNumber ();
    
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

    // does the rest occupy a full measure?
    if (missingDuration == fFullMeasureLength)
      paddingNote->
        setNoteOccupiesAFullMeasure ();
  
    // register rest's measure length
    paddingNote->
      setNotePositionInMeasure (
        fMeasureLength);
           
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

    // append the rest to the measure elements list
    // only now to make it possible to remove it afterwards
    // if it happens to be the first note of a chord
    appendNoteToMeasure (paddingNote);

    // this measure contains music
    fMeasureContainsMusic = true;

/*
    // account for rest duration in measure length
    setMeasureLength (
      inputLineNumber, fMeasureLength + missingDuration);
*/
  }
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
  if (gTraceOptions->fTraceChords || gTraceOptions->fTraceMeasures) {
    gLogIOstream <<
      "Removing note:" <<
      endl;

    gIndenter++;
    gLogIOstream <<
      note->asShortString ();
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
    
  // determine the measure kind
  if (fMeasureLength == fFullMeasureLength) {
    // full measure
    if (gTraceOptions->fTraceMeasures) {
      gLogIOstream <<
      "Measure '" << fMeasureNumber <<
      "' in voice \"" << voice->getVoiceName () <<
      "\", is full" <<
      ", line " << inputLineNumber <<
      endl;
    }

    setMeasureKind (
      kFullMeasureKind);
  }
  
  else if (fMeasureLength.getNumerator () == 0) { // JMI
    // empty measure
    if (gTraceOptions->fTraceMeasures) {
      gLogIOstream <<
      "Measure '" << fMeasureNumber <<
      "' in voice \"" << voice->getVoiceName () <<
      "\", is **empty**" <<
      ", line " << inputLineNumber <<
      endl;
    }

    setMeasureKind (
      kEmptyMeasureKind);
  }
  
  else if (fMeasureLength < fFullMeasureLength) {
    //  incomplete measure
    switch (fMeasureFirstInSegmentKind) {
      case msrMeasure::kMeasureFirstInSegmentYes:
        if (gTraceOptions->fTraceMeasures) {
          gLogIOstream <<
          "Measure '" << fMeasureNumber <<
          "' in voice \"" << voice->getVoiceName () <<
          "\", is an **upbeat**" <<
          ", line " << inputLineNumber <<
          endl;
        }
    
        setMeasureKind (
          kUpbeatMeasureKind);
        break;
        
      case msrMeasure::kMeasureFirstInSegmentNo:
        if (gTraceOptions->fTraceMeasures) {
          gLogIOstream <<
          "Measure '" << fMeasureNumber <<
          "' in voice \"" << voice->getVoiceName () <<
          "\", is **underfull**" <<
          ", line " << inputLineNumber <<
          endl;
        }
    
        setMeasureKind (
          kUnderfullMeasureKind);
        break;
    } // switch
  }

  else if (fMeasureLength > fFullMeasureLength) {
    // overfull measure
    if (gTraceOptions->fTraceMeasures) {
      gLogIOstream <<
      "Measure '" << fMeasureNumber <<
      "' in voice \"" << voice->getVoiceName () <<
      "\", is **overfull**" <<
      ", line " << inputLineNumber <<
      endl;
    }

    setMeasureKind (
      kOverfullMeasureKind);
  }
}

void msrMeasure::padUpToPartMeasureLengthHighTide (
  int inputLineNumber)
{
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

    // does the rest occupy a full measure?
    if (missingDuration == fFullMeasureLength)
      paddingNote->
        setNoteOccupiesAFullMeasure ();
  
    // register rest's position in measure
    paddingNote->
      setNotePositionInMeasure (fMeasureLength);
           
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

    // append the rest to the measure elements list
    // only now to make it possible to remove it afterwards
    // if it happens to be the first note of a chord
    appendNoteToMeasure (paddingNote);

    // this measure contains music
    fMeasureContainsMusic = true;
  }
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
    
  if (gTraceOptions->fTraceMeasures) {
    gLogIOstream <<
      "Finalizing measure '" << fMeasureNumber << "'" <<
      " in voice \"" << voice->getVoiceName () <<
      "\", line " << inputLineNumber <<
      endl;

    gIndenter++;

    const int fieldWidth = 26;
    
    gLogIOstream << left <<
      setw (fieldWidth) <<
      "measureLength" << " = " << fMeasureLength <<
      endl <<
      setw (fieldWidth) <<
      "fFullMeasureLength" << " = " << fFullMeasureLength <<
      endl <<
      setw (fieldWidth) <<
      "partMeasureLengthHighTide" << " = " <<
      partMeasureLengthHighTide <<
      endl;
        
    gIndenter--;
  }

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
      switch (fMeasureCreatedAfterARepeatKind) {
        case msrMeasure::kMeasureCreatedAfterARepeatYes:
          // such a measure should not be completed with a rest!
          break;
          
        case msrMeasure::kMeasureCreatedAfterARepeatNo:
          padUpToPartMeasureLengthHighTide (
            inputLineNumber);
          break;
      } // switch
  
      determineMeasureKind (
        inputLineNumber);
      break;
  } // switch
}

void msrMeasure::acceptIn (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrMeasure::acceptIn()" <<
      endl;
  }
      
  if (visitor<S_msrMeasure>*
    p =
      dynamic_cast<visitor<S_msrMeasure>*> (v)) {
        S_msrMeasure elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrMeasure::visitStart()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrMeasure::acceptOut (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrMeasure::acceptOut()" <<
      endl;
  }

  if (visitor<S_msrMeasure>*
    p =
      dynamic_cast<visitor<S_msrMeasure>*> (v)) {
        S_msrMeasure elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrMeasure::visitEnd()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrMeasure::browseData (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrMeasure::browseData()" <<
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
      "% <== msrMeasure::browseData()" <<
      endl;
  }
}

ostream& operator<< (ostream& os, const S_msrMeasure& elt)
{
  elt->print (os);
  return os;
}

string msrMeasure::measureKindAsString (
  msrMeasureKind measureKind)
{
  string result;

  switch (measureKind) {
    case msrMeasure::kUnknownMeasureKind:
      result = "**unkwnownKind**";
      break;
    case msrMeasure::kFullMeasureKind:
      result = "full";
      break;
    case msrMeasure::kUpbeatMeasureKind:
      result = "**upbeat**";
      break;
    case msrMeasure::kUnderfullMeasureKind:
      result = "**underfull**";
      break;
    case msrMeasure::kOverfullMeasureKind:
      result = "**overfull**";
      break;
    case msrMeasure::kSenzaMisuraMeasureKind:
      result = "**senzamisura**";
      break;
    case msrMeasure::kEmptyMeasureKind:
      result = "**empty**";
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
      result = "implicit";
      break;
    case msrMeasure::kMeasureImplicitNo:
      result = "not implicit";
      break;
  } // switch

  return result;
}

string msrMeasure::measureFirstInSegmentKindAsString (
    msrMeasureFirstInSegmentKind measureFirstInSegmentKind)
{
  string result;

  switch (measureFirstInSegmentKind) {
    case msrMeasure::kMeasureFirstInSegmentYes:
      result = "measureFirstInSegmentYes";
      break;
    case msrMeasure::kMeasureFirstInSegmentNo:
      result = "measureFirstInSegmentNo";
      break;
  } // switch

  return result;
}
      
string msrMeasure::measureCreatedAfterARepeatKindAsString (
  msrMeasureCreatedAfterARepeatKind measureCreatedAfterARepeatKind)
{
  string result;

  switch (measureCreatedAfterARepeatKind) {
    case msrMeasure::kMeasureCreatedAfterARepeatYes:
      result = "measureCreatedAfterARepeatYes";
      break;
    case msrMeasure::kMeasureCreatedAfterARepeatNo:
      result = "measureCreatedAfterARepeatNo";
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
    ", fullMeasureLengthAsMSRString: " <<
    fullMeasureLengthAsMSRString () <<
    ", " << fFullMeasureLength << " per full measure" <<
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
    "Segment uplink" << " : " <<
    fMeasureSegmentUplink->asShortString () <<
    endl <<
    
    setw (fieldWidth) <<
    "first in segment" << " : " <<
    msrMeasure::measureFirstInSegmentKindAsString (
      fMeasureFirstInSegmentKind) << 
    endl <<
    
    setw (fieldWidth) <<
    "measureContainsMusic" << " : " <<
    booleanAsString (
      fMeasureContainsMusic) << 
    endl <<
    
    setw (fieldWidth) <<
    "measureCreatedAfterARepeat:" << " : " <<
    msrMeasure::measureCreatedAfterARepeatKindAsString (
      fMeasureCreatedAfterARepeatKind) << 
    endl <<
    
    setw (fieldWidth) <<
    "measureIsASingleMeasureRest" << " : " <<
    booleanAsString (fMeasureIsASingleMeasureRest) <<
    endl <<
    
    setw (fieldWidth) <<
    "length" << " : " << fMeasureLength << " whole notes" <<
    endl <<
    
    setw (fieldWidth) <<
    "full measure length" << " : " <<
    fFullMeasureLength << " whole notes" <<
    endl <<
    
    setw (fieldWidth) <<
    "next measure number" << " : '" <<
    fNextMeasureNumber << "'" <<
    endl;
  gIndenter--;

  if (fMeasureElementsList.size ()) {
    os << endl;
    
    gIndenter++;
    
    list<S_msrElement>::const_iterator
      iBegin = fMeasureElementsList.begin (),
      iEnd   = fMeasureElementsList.end (),
      i      = iBegin;
    for ( ; ; ) {
      os << (*i);
        // JMI << endl;
      if (++i == iEnd) break;
    } // for
    
    gIndenter--;
  }
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

msrSegment::~msrSegment()
{}

void msrSegment::initializeSegment ()
{
  fSegmentAbsoluteNumber = ++gSegmentsCounter;
  
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
  if (gTraceOptions->fTraceSegments) {
    gLogIOstream <<
      "Creating a newborn clone of segment " <<
      asString () <<
      endl;
  }

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
  if (gTraceOptions->fTraceSegments) {
    gLogIOstream <<
      "Creating a deep copy of segment " <<
      asString () <<
      endl;
  }

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
     if (gTraceOptions->fTraceVoices) {
      gLogIOstream <<
        "There are " <<
        numberOfSegmentMeasures <<
        " measures in segment to be deep copied" <<
        endl;
    }
    
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
    if (gTraceOptions->fTraceSegments) {
      gLogIOstream <<
        "There are no measures in segment to be deep copied" <<
        endl;
    }
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

  S_msrMeasure
    newMeasure =
      msrMeasure::create (
        inputLineNumber,
        measureNumber,
        this);

  // get part current time
  S_msrTime
    partCurrentTime =
      fetchSegmentPartUplink ()->
        getPartCurrentTime ();

  // the measure can be created upon appendKeyTo..., // JMI
  // when the part time has not yet been set,
  // in which case the time will be set later
  if (partCurrentTime) {
  // set the measure length in whole notes per full measure
    newMeasure->
      setFullMeasureLengthFromTime (
        partCurrentTime);
  }

  // set 'first in segment' kind
  newMeasure->
    setMeasureFirstInSegmentKind (
      measureFirstInSegmentKind);
  
  // append it to the segment's measures list
  fSegmentMeasuresList.push_back (
    newMeasure);
  
  fMeasureNumberHasBeenSetInSegment = true;
}

void msrSegment::setNextMeasureNumberInSegment (
  int    inputLineNumber,
  string nextMeasureNumber)
{
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

  if (fSegmentMeasuresList.size ()) { // JMI ???
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
}

void msrSegment::finalizeCurrentMeasureInSegment (
  int    inputLineNumber)
{
  string
    currentMeasureNumber =
      fetchSegmentPartUplink ()->
        getPartCurrentMeasureNumber ();
      
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

  // don't finalize it it ain't been created
  if (fSegmentMeasuresList.size ()) {
    // finalize or remove segment's last measure
    S_msrMeasure
      lastMeasure =
        fSegmentMeasuresList.back ();
  
 // JMI   if (lastMeasure->getMeasureContainsMusic ()) {
      lastMeasure->
        finalizeMeasure (
          inputLineNumber);
 /*   }
    
    else {
      if (gTraceOptions->fTraceMeasures) {
        stringstream s;
    
        gLogIOstream <<
          "Cannot finalize current measure '" <<
          lastMeasure->getMeasureNumber () <<
          "' in segment '" <<
          asString () <<
          "' since it is empty, removing it";
      }
    
      fSegmentMeasuresList.pop_back ();
    }
  */
  
    if (gTraceOptions->fTraceMeasures || gTraceOptions->fTraceSegments) {
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
  }
}

void msrSegment::appendCommentToSegment (S_msrComment comment)
{
  if (gTraceOptions->fTraceClefs || gTraceOptions->fTraceSegments) {
    gLogIOstream <<
      "Appending comment '" << comment->asString () <<
      "' to segment " << asString () <<
      ", in voice \"" <<
      fSegmentVoiceUplink->getVoiceName () <<
      "\"" <<
      endl;
  }

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
      comment->getInputLineNumber (),
      __FILE__, __LINE__,
      s.str ());
  }
    
  // sanity check
  msrAssert (
    fSegmentMeasuresList.size () > 0,
    "fSegmentMeasuresList is empty");
    
  // register comment in segments's current measure
  fSegmentMeasuresList.back ()->
    appendCommentToMeasure (comment);
}

void msrSegment::appendClefToSegment (S_msrClef clef)
{
  if (gTraceOptions->fTraceClefs || gTraceOptions->fTraceSegments) {
    gLogIOstream <<
      "Appending clef '" << clef->asString () <<
      "' to segment " << asString () <<
      ", in voice \"" <<
      fSegmentVoiceUplink->getVoiceName () <<
      "\"" <<
      endl;
  }

      /* JMI
  fetchSegmentPartUplink ()->
    print (gLogIOstream);
*/

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
    
  // sanity check
  msrAssert (
    fSegmentMeasuresList.size () > 0,
    "fSegmentMeasuresList is empty");
    
  // register clef in segments's current measure
  fSegmentMeasuresList.back ()->
    appendClefToMeasure (clef);
}

void msrSegment::appendKeyToSegment (S_msrKey key)
{
  if (gTraceOptions->fTraceKeys || gTraceOptions->fTraceSegments) {
    gLogIOstream <<
      "Appending key '" << key->asString () <<
      "' to segment " << asString () <<
    ", in voice \"" <<
    fSegmentVoiceUplink->getVoiceName () <<
    "\"" <<
      endl;
  }
      
  // register key in segment
// JMI  fSegmentKey = key;

  // sanity check
  msrAssert (
    fSegmentMeasuresList.size () > 0,
    "fSegmentMeasuresList is empty");
    
  // register key in segments's current measure
  fSegmentMeasuresList.back ()->
    appendKeyToMeasure (key);
}
    
void msrSegment::appendTimeToSegment (S_msrTime time)
{
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
      
  // sanity check
  msrAssert (
    fSegmentMeasuresList.size () > 0,
    "fSegmentMeasuresList is empty");
    
  // append time to segments's current measure
  fSegmentMeasuresList.back ()->
    appendTimeToMeasure (time);
}

void msrSegment::appendTimeToSegmentClone (S_msrTime time)
{
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
      
  // sanity check
  msrAssert (
    fSegmentMeasuresList.size () > 0,
    "fSegmentMeasuresList is empty");
    
  // append time to segments's current measure
  fSegmentMeasuresList.back ()->
    appendTimeToMeasureClone (time);
}

void msrSegment::appendHarmonyToSegment (S_msrHarmony harmony)
{
  if (gTraceOptions->fTraceHarmonies || gTraceOptions->fTraceSegments) {
    gLogIOstream <<
      "Appending harmony " << harmony->asString () <<
      " to segment " << asString () <<
      "' in voice \"" <<
      fSegmentVoiceUplink->getVoiceName () <<
      "\"" <<
      endl;
  }
      
  // sanity check
  msrAssert (
    fSegmentMeasuresList.size () > 0,
    "fSegmentMeasuresList is empty");
    
  // append it to this segment
  fSegmentMeasuresList.back ()-> // JMI ???
    appendHarmonyToMeasure (harmony);
}

void msrSegment::appendHarmonyToSegmentClone (S_msrHarmony harmony)
{
  if (gTraceOptions->fTraceHarmonies || gTraceOptions->fTraceSegments) {
    gLogIOstream <<
      "Appending harmony " << harmony->asString () <<
      " to segment clone " << asString () <<
      "' in voice clone \"" <<
      fSegmentVoiceUplink->getVoiceName () <<
      "\"" <<
      endl;
  }
      
  // sanity check
  msrAssert (
    fSegmentMeasuresList.size () > 0,
    "fSegmentMeasuresList is empty");
    
  // append it to this segment
  fSegmentMeasuresList.back ()->
    appendHarmonyToMeasureClone (harmony);
}

void msrSegment::appendFiguredBassToSegment (
  S_msrFiguredBass figuredBass)
{
  if (gTraceOptions->fTraceFiguredBass || gTraceOptions->fTraceSegments) {
    gLogIOstream <<
      "Appending figured bass " << figuredBass->asString () <<
      " to segment " << asString () <<
      "' in voice \"" <<
      fSegmentVoiceUplink->getVoiceName () <<
      "\"" <<
      endl;
  }
      
  // sanity check
  msrAssert (
    fSegmentMeasuresList.size () > 0,
    "fSegmentMeasuresList is empty");
    
  // append it to this segment
  fSegmentMeasuresList.back ()-> // JMI ???
    appendFiguredBassToMeasure (figuredBass);
}

void msrSegment::appendFiguredBassToSegmentClone (
  S_msrFiguredBass figuredBass)
{
  if (gTraceOptions->fTraceFiguredBass || gTraceOptions->fTraceSegments) {
    gLogIOstream <<
      "Appending figured bass " << figuredBass->asString () <<
      " to segment clone " << asString () <<
      "' in voice clone \"" <<
      fSegmentVoiceUplink->getVoiceName () <<
      "\"" <<
      endl;
  }
      
  // sanity check
  msrAssert (
    fSegmentMeasuresList.size () > 0,
    "fSegmentMeasuresList is empty");
    
  // append it to this segment
  fSegmentMeasuresList.back ()->
    appendFiguredBassToMeasure (figuredBass);
}

void msrSegment::appendSegnoToSegment (S_msrSegno segno)
{
  if (gTraceOptions->fTraceHarmonies || gTraceOptions->fTraceSegments) {
    gLogIOstream <<
      "Appending segno " <<
      " to segment " << asString () <<
      "' in voice \"" <<
      fSegmentVoiceUplink->getVoiceName () <<
      "\"" <<
      endl;
  }
      
  // sanity check
  msrAssert (
    fSegmentMeasuresList.size () > 0,
    "fSegmentMeasuresList is empty");
    
  // append it to this segment
  fSegmentMeasuresList.back ()->
    appendSegnoToMeasure (segno);
}

void msrSegment::appendCodaToSegment (S_msrCoda coda)
{
  if (gTraceOptions->fTraceHarmonies || gTraceOptions->fTraceSegments) {
    gLogIOstream <<
      "Appending coda " <<
      " to segment " << asString () <<
      "' in voice \"" <<
      fSegmentVoiceUplink->getVoiceName () <<
      "\"" <<
      endl;
  }
      
  // sanity check
  msrAssert (
    fSegmentMeasuresList.size () > 0,
    "fSegmentMeasuresList is empty");
    
  // append it to this segment
  fSegmentMeasuresList.back ()->
    appendCodaToMeasure (coda);
}

void msrSegment::appendEyeGlassesToSegment (
  S_msrEyeGlasses eyeGlasses)
{
  if (gTraceOptions->fTraceHarmonies || gTraceOptions->fTraceSegments) {
    gLogIOstream <<
      "Appending eyeglasses " <<
      " to segment " << asString () <<
      "' in voice \"" <<
      fSegmentVoiceUplink->getVoiceName () <<
      "\"" <<
      endl;
  }
      
  // sanity check
  msrAssert (
    fSegmentMeasuresList.size () > 0,
    "fSegmentMeasuresList is empty");
    
  // append it to this segment
  fSegmentMeasuresList.back ()->
    appendEyeGlassesToMeasure (eyeGlasses);
}

void msrSegment::appendPedalToSegment (S_msrPedal pedal)
{
  if (gTraceOptions->fTraceHarmonies || gTraceOptions->fTraceSegments) {
    gLogIOstream <<
      "Appending pedal " <<
      " to segment " << asString () <<
      "' in voice \"" <<
      fSegmentVoiceUplink->getVoiceName () <<
      "\"" <<
      endl;
  }
      
  // sanity check
  msrAssert (
    fSegmentMeasuresList.size () > 0,
    "fSegmentMeasuresList is empty");
    
  // append it to this segment
  fSegmentMeasuresList.back ()->
    appendPedalToMeasure (pedal);
}

void msrSegment::appendDampToSegment (S_msrDamp damp)
{
  if (gTraceOptions->fTraceHarmonies || gTraceOptions->fTraceSegments) {
    gLogIOstream <<
      "Appending damp " <<
      " to segment " << asString () <<
      "' in voice \"" <<
      fSegmentVoiceUplink->getVoiceName () <<
      "\"" <<
      endl;
  }
      
  // sanity check
  msrAssert (
    fSegmentMeasuresList.size () > 0,
    "fSegmentMeasuresList is empty");
    
  // append it to this segment
  fSegmentMeasuresList.back ()->
    appendDampToMeasure (damp);
}

void msrSegment::appendDampAllToSegment (S_msrDampAll dampAll)
{
  if (gTraceOptions->fTraceHarmonies || gTraceOptions->fTraceSegments) {
    gLogIOstream <<
      "Appending damp all " <<
      " to segment " << asString () <<
      "' in voice \"" <<
      fSegmentVoiceUplink->getVoiceName () <<
      "\"" <<
      endl;
  }
      
  // sanity check
  msrAssert (
    fSegmentMeasuresList.size () > 0,
    "fSegmentMeasuresList is empty");
    
  // append it to this segment
  fSegmentMeasuresList.back ()->
    appendDampAllToMeasure (dampAll);
}

void msrSegment::appendTransposeToSegment (
  S_msrTranspose transpose)
{
  if (gTraceOptions->fTraceTranspositions || gTraceOptions->fTraceSegments) {
    gLogIOstream <<
      "Appending transpose " <<
      " to segment " << asString () <<
      "' in voice \"" <<
      fSegmentVoiceUplink->getVoiceName () <<
      "\"" <<
      endl;
  }
      
  // sanity check
  msrAssert (
    fSegmentMeasuresList.size () > 0,
    "fSegmentMeasuresList is empty");
    
  // append it to this segment
  fSegmentMeasuresList.back ()->
    appendTransposeToMeasure (transpose);
}

void msrSegment::appendPartNameDisplayToSegment (
  S_msrPartNameDisplay partNameDisplay)
{
  if (gTraceOptions->fTraceTranspositions || gTraceOptions->fTraceSegments) {
    gLogIOstream <<
      "Appending part name display " <<
      " to segment " << asString () <<
      "' in voice \"" <<
      fSegmentVoiceUplink->getVoiceName () <<
      "\"" <<
      endl;
  }
      
  // sanity check
  msrAssert (
    fSegmentMeasuresList.size () > 0,
    "fSegmentMeasuresList is empty");
    
  // append it to this segment
  fSegmentMeasuresList.back ()->
    appendPartNameDisplayToMeasure (partNameDisplay);
}

void msrSegment::appendPartAbbreviationDisplayToSegment (
  S_msrPartAbbreviationDisplay partAbbreviationDisplay)
{
  if (gTraceOptions->fTraceTranspositions || gTraceOptions->fTraceSegments) {
    gLogIOstream <<
      "Appending part abbreviation display " <<
      
      " to segment " << asString () <<
      "' in voice \"" <<
      fSegmentVoiceUplink->getVoiceName () <<
      "\"" <<
      endl;
  }
      
  // sanity check
  msrAssert (
    fSegmentMeasuresList.size () > 0,
    "fSegmentMeasuresList is empty");
    
  // append it to this segment
  fSegmentMeasuresList.back ()->
    appendPartAbbreviationDisplayToMeasure (partAbbreviationDisplay);
}

void msrSegment::appendStaffDetailsToSegment (
  S_msrStaffDetails staffDetails)
{
  if (gTraceOptions->fTraceHarmonies || gTraceOptions->fTraceSegments) {
    gLogIOstream <<
      "Appending staff details " <<
      " to segment " << asString () <<
      "' in voice \"" <<
      fSegmentVoiceUplink->getVoiceName () <<
      "\"" <<
      endl;
  }
      
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
}

void msrSegment::appendLineBreakToSegment (S_msrLineBreak lineBreak)
{
  if (gTraceOptions->fTraceHarmonies || gTraceOptions->fTraceSegments) {
    gLogIOstream <<
      "Appending break " <<
      " to segment " << asString () <<
      "' in voice \"" <<
      fSegmentVoiceUplink->getVoiceName () <<
      "\"" <<
      endl;
  }
      
  // sanity check
  msrAssert (
    fSegmentMeasuresList.size () > 0,
    "fSegmentMeasuresList is empty");
    
  // append it to this segment
  fSegmentMeasuresList.back ()->
    appendLineBreakToMeasure (lineBreak);
}

void msrSegment::appendPageBreakToSegment (S_msrPageBreak pageBreak)
{
  if (gTraceOptions->fTraceHarmonies || gTraceOptions->fTraceSegments) {
    gLogIOstream <<
      "Appending break " <<
      " to segment " << asString () <<
      "' in voice \"" <<
      fSegmentVoiceUplink->getVoiceName () <<
      "\"" <<
      endl;
  }
      
  // sanity check
  msrAssert (
    fSegmentMeasuresList.size () > 0,
    "fSegmentMeasuresList is empty");
    
  // append it to this segment
  fSegmentMeasuresList.back ()->
    appendPageBreakToMeasure (pageBreak);
}

void msrSegment::appendBarNumberCheckToSegment (
  S_msrBarNumberCheck barNumberCheck)
{
  if (gTraceOptions->fTraceHarmonies || gTraceOptions->fTraceSegments) {
    gLogIOstream <<
      "Appending bar number check " <<
      " to segment " << asString () <<
      "' in voice \"" <<
      fSegmentVoiceUplink->getVoiceName () <<
      "\"" <<
      endl;
  }
      
  // sanity check
  msrAssert (
    fSegmentMeasuresList.size () > 0,
    "fSegmentMeasuresList is empty");
    
  // append it to this segment
  fSegmentMeasuresList.back ()->
    appendBarNumberCheckToMeasure (barNumberCheck);
}

void msrSegment::appendTempoToSegment (
  S_msrTempo tempo)
{
  if (gTraceOptions->fTraceHarmonies || gTraceOptions->fTraceSegments) {
    gLogIOstream <<
      "Appending tempo " <<
      " to segment " << asString () <<
      "' in voice \"" <<
      fSegmentVoiceUplink->getVoiceName () <<
      "\"" <<
      endl;
  }
      
  // sanity check
  msrAssert (
    fSegmentMeasuresList.size () > 0,
    "fSegmentMeasuresList is empty");
    
  // append it to this segment
  fSegmentMeasuresList.back ()->
    appendTempoToMeasure (tempo);
}

void msrSegment::appendRehearsalToSegment (
  S_msrRehearsal rehearsal)
{
  if (gTraceOptions->fTraceHarmonies || gTraceOptions->fTraceSegments) {
    gLogIOstream <<
      "Appending rehearsal " <<
      " to segment " << asString () <<
      "' in voice \"" <<
      fSegmentVoiceUplink->getVoiceName () <<
      "\"" <<
      endl;
  }
      
  // sanity check
  msrAssert (
    fSegmentMeasuresList.size () > 0,
    "fSegmentMeasuresList is empty");
    
  // append it to this segment
  fSegmentMeasuresList.back ()->
    appendRehearsalToMeasure (rehearsal);
}

void msrSegment::appendOctaveShiftToSegment (
  S_msrOctaveShift octaveShift)
{
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
      
  // sanity check
  msrAssert (
    fSegmentMeasuresList.size () > 0,
    "fSegmentMeasuresList is empty");
    
  // append it to this segment
  fSegmentMeasuresList.back ()->
    appendOctaveShiftToMeasure (octaveShift);
}

void msrSegment::appendScordaturaToSegment (
  S_msrScordatura scordatura)
{
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
      
  // sanity check
  msrAssert (
    fSegmentMeasuresList.size () > 0,
    "fSegmentMeasuresList is empty");
    
  // append it to this segment
  fSegmentMeasuresList.back ()->
    appendScordaturaToMeasure (scordatura);
}

void msrSegment::appendAccordionRegistrationToSegment (
  S_msrAccordionRegistration
    accordionRegistration)
{
  if (gTraceOptions->fTraceBasic || gTraceOptions->fTraceSegments) {
    gLogIOstream <<
      "Appending accordion registration '" <<
      accordionRegistration->asString () <<
      "' to segment " << asString () <<
      "' in voice \"" <<
      fSegmentVoiceUplink->getVoiceName () <<
      "\"" <<
      endl;
  }
      
  // sanity check
  msrAssert (
    fSegmentMeasuresList.size () > 0,
    "fSegmentMeasuresList is empty");
    
  // append it to this segment
  fSegmentMeasuresList.back ()->
    appendAccordionRegistrationToMeasure (
      accordionRegistration);
}

void msrSegment::appendHarpPedalsTuningToSegment (
  S_msrHarpPedalsTuning
    harpPedalsTuning)
{
  if (gTraceOptions->fTraceBasic || gTraceOptions->fTraceSegments) {
    gLogIOstream <<
      "Appending staff pedals tuning '" <<
      harpPedalsTuning->asString () <<
      "' to segment " << asString () <<
      "' in voice \"" <<
      fSegmentVoiceUplink->getVoiceName () <<
      "\"" <<
      endl;
  }
      
  // sanity check
  msrAssert (
    fSegmentMeasuresList.size () > 0,
    "fSegmentMeasuresList is empty");
    
  // append it to this segment
  fSegmentMeasuresList.back ()->
    appendHarpPedalsTuningToMeasure (
      harpPedalsTuning);
}

void msrSegment::padUpToMeasureLengthInSegment (
  int      inputLineNumber,
  rational measureLength)
{
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

string msrSegment::segmentMeasureNumbersAsString () const
{
  stringstream s;

  return s.str ();
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
    
  if (gTraceOptions->fTraceMeasures || gTraceOptions->fTraceSegments) {
    gLogIOstream <<
      "Appending measure " << measureNumber <<
      " to segment " << asString ();

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
  if (gTraceOptions->fTraceMeasures || gTraceOptions->fTraceSegments) {
    gLogIOstream <<
      "Prepending barline '" << barline->asString () <<
      "' to segment '" << asString () << "'" <<
      "' in voice \"" <<
      fSegmentVoiceUplink->getVoiceName () <<
      "\"," <<
      endl;
  }

  // sanity check
  msrAssert (
    fSegmentMeasuresList.size () > 0,
    "fSegmentMeasuresList is empty");

  // prepend barline to this segment
  fSegmentMeasuresList.front ()->
    prependBarlineToMeasure (barline);
}

void msrSegment::appendBarlineToSegment (S_msrBarline barline)
{
  if (gTraceOptions->fTraceMeasures || gTraceOptions->fTraceSegments) {
    gLogIOstream <<
      "Appending barline '" << barline->asString () <<
      "' to segment '" << asString () << "'" <<
      "' in voice \"" <<
      fSegmentVoiceUplink->getVoiceName () <<
      "\"," <<
      endl;
  }

  if (! fSegmentMeasuresList.size ()) { // JMI
    if (gTraceOptions->fTraceBarlines || gTraceOptions->fTraceSegments) { // JMI
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
  }
  
  // sanity check
  msrAssert (
    fSegmentMeasuresList.size () > 0,
    "fSegmentMeasuresList is empty");

  // append barline to this segment
  fSegmentMeasuresList.back ()->
    appendBarlineToMeasure (barline);
}

void msrSegment::appendBarCheckToSegment (S_msrBarCheck barCheck)
{
  if (gTraceOptions->fTraceMeasures || gTraceOptions->fTraceSegments) {
    gLogIOstream <<
      "Appending bar check " << barCheck->asString () <<
      " to segment '" << asString () << "'" <<
      "' in voice \"" <<
      fSegmentVoiceUplink->getVoiceName () <<
      "\"," <<
      endl;
  }
      
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
  if (
    gTraceOptions->fTraceVoices
      ||
    gTraceOptions->fTraceStaves
      ||
    gTraceOptions->fTraceSegments) {
    gLogIOstream <<
      "Append voice staff change " <<
      voiceStaffChange->asString () <<
      " to segment " <<
      " \"" << asString () << "\"" <<
      endl;
  }
  
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
      "+++++++++++++++++ appendNoteToSegment, score:" <<
      endl <<
      fSegmentVoiceUplink->
        getVoiceStaffUplink ()->
          getStaffPartUplink ()->
            getPartPartGroupUplink ()->
              getPartGroupScoreUplink ()<<
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
}

void msrSegment::removeElementFromSegment (
  int          inputLineNumber,
  S_msrElement element)
{  
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
  if (gTraceOptions->fTraceMeasures || gTraceOptions->fTraceSegments) {
    gLogIOstream <<
      "Removing last measure from segment '" <<
      fSegmentAbsoluteNumber <<
      "'" <<
      endl;
  }

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

  fSegmentMeasuresList.pop_back ();
  
  return result;
}

void msrSegment::acceptIn (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrSegment::acceptIn()" <<
      endl;
  }
      
  if (visitor<S_msrSegment>*
    p =
      dynamic_cast<visitor<S_msrSegment>*> (v)) {
        S_msrSegment elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrSegment::visitStart()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrSegment::acceptOut (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrSegment::acceptOut()" <<
      endl;
  }

  if (visitor<S_msrSegment>*
    p =
      dynamic_cast<visitor<S_msrSegment>*> (v)) {
        S_msrSegment elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrSegment::visitEnd()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrSegment::browseData (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrSegment::browseData()" <<
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
      "% <== msrSegment::browseData()" <<
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
    setw (fieldWidth) << "Voice uplink" << " : " <<
    "\"" <<
    fSegmentVoiceUplink->getVoiceName () <<
    "\"" <<
    endl;

  if (! fSegmentMeasuresList.size ()) {
    os <<
      setw (fieldWidth) << "Measures" << " : " << "none" <<
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

msrRepeatCommonPart::~msrRepeatCommonPart()
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
      "% ==> msrRepeatCommonPart::acceptIn()" <<
      endl;
  }
      
  if (visitor<S_msrRepeatCommonPart>*
    p =
      dynamic_cast<visitor<S_msrRepeatCommonPart>*> (v)) {
        S_msrRepeatCommonPart elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrRepeatCommonPart::visitStart()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrRepeatCommonPart::acceptOut (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrRepeatCommonPart::acceptOut()" <<
      endl;
  }

  if (visitor<S_msrRepeatCommonPart>*
    p =
      dynamic_cast<visitor<S_msrRepeatCommonPart>*> (v)) {
        S_msrRepeatCommonPart elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrRepeatCommonPart::visitEnd()" <<
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
  if (gTraceOptions->fTraceVoices) {
    gLogIOstream <<
      "Appending element '" << elem <<
      "' to repeat common part '" << asString () << "'" <<
      endl;
  }

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

msrRepeatEnding::~msrRepeatEnding()
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
      "% ==> msrRepeatEnding::acceptIn()" <<
      endl;
  }
      
  if (visitor<S_msrRepeatEnding>*
    p =
      dynamic_cast<visitor<S_msrRepeatEnding>*> (v)) {
        S_msrRepeatEnding elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrRepeatEnding::visitStart()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrRepeatEnding::acceptOut (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrRepeatEnding::acceptOut()" <<
      endl;
  }

  if (visitor<S_msrRepeatEnding>*
    p =
      dynamic_cast<visitor<S_msrRepeatEnding>*> (v)) {
        S_msrRepeatEnding elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrRepeatEnding::visitEnd()" <<
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
    ", repeat uplink: '" <<
    fRepeatEndingRepeatUplink->
      asString () <<
    "', line " << fInputLineNumber <<
    ", number " << fRepeatEndingNumber <<
    ", internal number " << fRepeatEndingInternalNumber <<
    endl;

  return s.str ();
}

void msrRepeatEnding::print (ostream& os)
{
  os <<
    endl <<
    "RepeatEnding" <<
    ", " <<
    repeatEndingKindAsString (
      fRepeatEndingKind) <<
    ", line " << fInputLineNumber <<
    ", number " << fRepeatEndingNumber <<
    ", internal number " << fRepeatEndingInternalNumber <<
    endl;

  gIndenter++;
      
  os <<
    "Repeat uplink: '" <<
    fRepeatEndingRepeatUplink->
      asString () <<
    endl <<
    endl <<
    fRepeatEndingSegment;

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

msrRepeat::~msrRepeat()
{}

void msrRepeat::setRepeatCommonPart (
  S_msrRepeatCommonPart repeatCommonPart)
{
  // sanity check
  msrAssert(
    repeatCommonPart != nullptr,
    "repeatCommonPart is null");
    
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
      
  fRepeatCommonPart = repeatCommonPart;
}
/*
void msrRepeat::setRepeatCommonSegment (
  S_msrSegment repeatCommonSegment)
{
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
    
  if (gTraceOptions->fTraceRepeats) {
    gLogIOstream <<
      "Adding ending '" <<
      repeatEnding->asString () <<
      "' to repeat" <<
      endl;
  }
      
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
      "% ==> msrRepeat::acceptIn()" <<
      endl;
  }
      
  if (visitor<S_msrRepeat>*
    p =
      dynamic_cast<visitor<S_msrRepeat>*> (v)) {
        S_msrRepeat elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrRepeat::visitStart()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrRepeat::acceptOut (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrRepeat::acceptOut()" <<
      endl;
  }

  if (visitor<S_msrRepeat>*
    p =
      dynamic_cast<visitor<S_msrRepeat>*> (v)) {
        S_msrRepeat elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrRepeat::visitEnd()" <<
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

msrMeasuresRepeatPattern::~msrMeasuresRepeatPattern()
{}

void msrMeasuresRepeatPattern::setMeasuresRepeatPatternSegment (
    S_msrSegment measuresRepeatPatternSegment)
{
  if (gTraceOptions->fTraceRepeats) {
    gLogIOstream <<
      "Setting measure repeat pattern segment containing " <<
      singularOrPlural (
          measuresRepeatPatternMeasuresNumber (),
        "measure",
        "measures") <<
      endl;
  }
      
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
      "% ==> msrMeasuresRepeatPattern::acceptIn()" <<
      endl;
  }
      
  if (visitor<S_msrMeasuresRepeatPattern>*
    p =
      dynamic_cast<visitor<S_msrMeasuresRepeatPattern>*> (v)) {
        S_msrMeasuresRepeatPattern elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrMeasuresRepeatPattern::visitStart()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrMeasuresRepeatPattern::acceptOut (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrMeasuresRepeatPattern::acceptOut()" <<
      endl;
  }

  if (visitor<S_msrMeasuresRepeatPattern>*
    p =
      dynamic_cast<visitor<S_msrMeasuresRepeatPattern>*> (v)) {
        S_msrMeasuresRepeatPattern elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrMeasuresRepeatPattern::visitEnd()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrMeasuresRepeatPattern::browseData (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrMeasuresRepeatPattern::browseData()" <<
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

msrMeasuresRepeatReplicas::~msrMeasuresRepeatReplicas()
{}

void msrMeasuresRepeatReplicas::setMeasuresRepeatReplicasSegment (
    S_msrSegment measuresRepeatReplicasSegment)
{
  if (gTraceOptions->fTraceRepeats) {
    gLogIOstream <<
      "Setting measure repeat replicas segment containing " <<
      singularOrPlural (
          measuresRepeatReplicasMeasuresNumber (),
        "measure",
        "measures") <<
      endl;
  }
      
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
      "% ==> msrMeasuresRepeatReplicas::acceptIn()" <<
      endl;
  }
      
  if (visitor<S_msrMeasuresRepeatReplicas>*
    p =
      dynamic_cast<visitor<S_msrMeasuresRepeatReplicas>*> (v)) {
        S_msrMeasuresRepeatReplicas elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrMeasuresRepeatReplicas::visitStart()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrMeasuresRepeatReplicas::acceptOut (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrMeasuresRepeatReplicas::acceptOut()" <<
      endl;
  }

  if (visitor<S_msrMeasuresRepeatReplicas>*
    p =
      dynamic_cast<visitor<S_msrMeasuresRepeatReplicas>*> (v)) {
        S_msrMeasuresRepeatReplicas elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrMeasuresRepeatReplicas::visitEnd()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrMeasuresRepeatReplicas::browseData (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrMeasuresRepeatReplicas::browseData()" <<
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

msrMeasuresRepeat::~msrMeasuresRepeat()
{}

void msrMeasuresRepeat::setMeasuresRepeatPattern (
  S_msrMeasuresRepeatPattern measuresRepeatPattern)
{
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
      
  // sanity check
  msrAssert (
    measuresRepeatPattern != nullptr,
    "measuresRepeatPattern is null");

  fMeasuresRepeatPattern = measuresRepeatPattern;
}

void msrMeasuresRepeat::setMeasuresRepeatReplicas (
  S_msrMeasuresRepeatReplicas measuresRepeatReplicas)
{
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
      "% ==> msrMeasuresRepeat::acceptIn()" <<
      endl;
  }
      
  if (visitor<S_msrMeasuresRepeat>*
    p =
      dynamic_cast<visitor<S_msrMeasuresRepeat>*> (v)) {
        S_msrMeasuresRepeat elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrMeasuresRepeat::visitStart()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrMeasuresRepeat::acceptOut (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrMeasuresRepeat::acceptOut()" <<
      endl;
  }

  if (visitor<S_msrMeasuresRepeat>*
    p =
      dynamic_cast<visitor<S_msrMeasuresRepeat>*> (v)) {
        S_msrMeasuresRepeat elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrMeasuresRepeat::visitEnd()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrMeasuresRepeat::browseData (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrMeasuresRepeat::browseData()" <<
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
    if (gMsrOptions->fTraceMsrVisitors || gTraceOptions->fTraceRepeats) {
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

msrMultipleRestContents::~msrMultipleRestContents()
{}

S_msrMultipleRestContents msrMultipleRestContents::createMultipleRestContentsNewbornClone (
  S_msrVoice containingVoice)
{
  if (gTraceOptions->fTraceRepeats) {
    gLogIOstream <<
      "Creating a newborn clone of a multiple rest contents" <<
      endl;
  }
  
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
  if (gTraceOptions->fTraceRepeats) {
    gLogIOstream <<
      "Setting multiple rest contents segment containing " <<
      singularOrPlural (
          multipleRestContentsMeasuresNumber (),
        "measure",
        "measures") <<
      endl;
  }
      
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
      "% ==> msrMultipleRestContents::acceptIn()" <<
      endl;
  }
      
  if (visitor<S_msrMultipleRestContents>*
    p =
      dynamic_cast<visitor<S_msrMultipleRestContents>*> (v)) {
        S_msrMultipleRestContents elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrMultipleRestContents::visitStart()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrMultipleRestContents::acceptOut (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrMultipleRestContents::acceptOut()" <<
      endl;
  }

  if (visitor<S_msrMultipleRestContents>*
    p =
      dynamic_cast<visitor<S_msrMultipleRestContents>*> (v)) {
        S_msrMultipleRestContents elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrMultipleRestContents::visitEnd()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrMultipleRestContents::browseData (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrMultipleRestContents::browseData()" <<
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

msrMultipleRest::~msrMultipleRest()
{}

S_msrMultipleRest msrMultipleRest::createMultipleRestNewbornClone (
  S_msrVoice containingVoice)
{
  if (gTraceOptions->fTraceRepeats) {
    gLogIOstream <<
      "Creating a newborn clone of multiple rest " <<
      asString () <<
      endl;
  }
  
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
      
  // sanity check
  msrAssert (
    multipleRestContents != nullptr,
    "multipleRestContents is null");

  fMultipleRestContents = multipleRestContents;
}

void msrMultipleRest::setMultipleRestNextMeasureNumber (
  string measureNumber)
{
  if (gTraceOptions->fTraceRepeats) {
    gLogIOstream <<
      "Setting multiple rest next measure number to'" <<
      "' " <<
      measureNumber <<
      endl;
  }

  fMultipleRestNextMeasureNumber =
    measureNumber;
}

void msrMultipleRest::acceptIn (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrMultipleRest::acceptIn()" <<
      endl;
  }
      
  if (visitor<S_msrMultipleRest>*
    p =
      dynamic_cast<visitor<S_msrMultipleRest>*> (v)) {
        S_msrMultipleRest elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrMultipleRest::visitStart()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrMultipleRest::acceptOut (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrMultipleRest::acceptOut()" <<
      endl;
  }

  if (visitor<S_msrMultipleRest>*
    p =
      dynamic_cast<visitor<S_msrMultipleRest>*> (v)) {
        S_msrMultipleRest elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrMultipleRest::visitEnd()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrMultipleRest::browseData (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrMultipleRest::browseData()" <<
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
    "Multiple rest" <<
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

ostream& operator<< (ostream& os, const S_msrMultipleRest& elt)
{
  elt->print (os);
  return os;
}

void msrMultipleRest::print (ostream& os)
{
  os <<
    endl <<
    asString () <<
    endl;
  
  gIndenter++;

  const int fieldWidth = 24;

  // print the voice uplink
  os << left <<
    setw (fieldWidth) <<
    "MultipleRestVoiceUplink" << " : " <<
    "\"" <<
    fMultipleRestVoiceUplink->getVoiceName () <<
    "\"" <<
    endl;
    
  // print the rests contents
  if (! fMultipleRestContents) {
    os << left <<
      setw (fieldWidth) <<
      "Multiple rests contents" << " : " << "none" <<
      endl;
  }

  else {
    os <<
      fMultipleRestContents;
  }
      
  gIndenter--;
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

msrRepeatCoda::~msrRepeatCoda()
{}

S_msrRepeatCoda msrRepeatCoda::createRepeatCodaNewbornClone (
  S_msrRepeat containingRepeat)
{
  if (gTraceOptions->fTraceRepeats) {
    gLogIOstream <<
      "Creating a newborn clone of a " <<
      asString () <<
      endl;
  }
  
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
  if (gTraceOptions->fTraceRepeats) {
    gLogIOstream <<
      "Creating a newborn clone of a " <<
      asString () <<
      endl;
  }
  
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
      "% ==> msrRepeatCoda::acceptIn()" <<
      endl;
  }
      
  if (visitor<S_msrRepeatCoda>*
    p =
      dynamic_cast<visitor<S_msrRepeatCoda>*> (v)) {
        S_msrRepeatCoda elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrRepeatCoda::visitStart()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrRepeatCoda::acceptOut (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrRepeatCoda::acceptOut()" <<
      endl;
  }

  if (visitor<S_msrRepeatCoda>*
    p =
      dynamic_cast<visitor<S_msrRepeatCoda>*> (v)) {
        S_msrRepeatCoda elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrRepeatCoda::visitEnd()" <<
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
S_msrHarpPedalsTuning msrHarpPedalsTuning::create (
  int inputLineNumber)
{
  msrHarpPedalsTuning* o =
    new msrHarpPedalsTuning (
      inputLineNumber);
  assert(o!=0);
  return o;
}

msrHarpPedalsTuning::msrHarpPedalsTuning (
  int inputLineNumber)
    : msrElement (inputLineNumber)
{
}

msrHarpPedalsTuning::~msrHarpPedalsTuning()
{}

S_msrHarpPedalsTuning msrHarpPedalsTuning::createHarpPedalsTuningNewbornClone ()
{
  if (gTraceOptions->fTraceRepeats) {
    gLogIOstream <<
      "Creating a newborn clone of a " <<
      asString () <<
      endl;
  }
  
  S_msrHarpPedalsTuning
    newbornClone = 0; // JMI

  return newbornClone;
}

S_msrHarpPedalsTuning msrHarpPedalsTuning::createHarpPedalsTuningDeepCopy ()
{
  if (gTraceOptions->fTraceRepeats) {
    gLogIOstream <<
      "Creating a newborn clone of a " <<
      asString () <<
      endl;
  }
  
  S_msrHarpPedalsTuning
    harpPedalsTuningDeepCopy = 0; // JMI

  return harpPedalsTuningDeepCopy;
}

void msrHarpPedalsTuning::addPedalTuning (
  int                  inputLineNumber,
  msrDiatonicPitchKind diatonicPitchKind,
  msrAlterationKind    alterationKind)
{
  // is diatonicPitch in the part renaming map?
  map<msrDiatonicPitchKind, msrAlterationKind>::const_iterator
    it =
      fHarpPedalsAlterationKindsMap.find (diatonicPitchKind);
        
  if (it != fHarpPedalsAlterationKindsMap.end ()) {
    stringstream s;

    s <<
      "pedal tuning '" <<
      msrDiatonicPitchKindAsString (
        diatonicPitchKind) <<
      msrAlterationKindAsString (
        alterationKind) <<
      "' has already been specified";
      
    msrMusicXMLError (
      gXml2lyOptions->fInputSourceName,
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  }

  fHarpPedalsAlterationKindsMap [diatonicPitchKind] = alterationKind;
}

void msrHarpPedalsTuning::acceptIn (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrHarpPedalsTuning::acceptIn()" <<
      endl;
  }
      
  if (visitor<S_msrHarpPedalsTuning>*
    p =
      dynamic_cast<visitor<S_msrHarpPedalsTuning>*> (v)) {
        S_msrHarpPedalsTuning elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrHarpPedalsTuning::visitStart()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrHarpPedalsTuning::acceptOut (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrHarpPedalsTuning::acceptOut()" <<
      endl;
  }

  if (visitor<S_msrHarpPedalsTuning>*
    p =
      dynamic_cast<visitor<S_msrHarpPedalsTuning>*> (v)) {
        S_msrHarpPedalsTuning elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrHarpPedalsTuning::visitEnd()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrHarpPedalsTuning::browseData (basevisitor* v)
{}

ostream& operator<< (ostream& os, const S_msrHarpPedalsTuning& elt)
{
  elt->print (os);
  return os;
}

string msrHarpPedalsTuning::asString () const
{
  stringstream s;

  s <<
    "HarpPedalsTuning" <<
    ", line " << fInputLineNumber <<
    ", ";
    
  if (fHarpPedalsAlterationKindsMap.size ()) {
    gIndenter++;

    map<msrDiatonicPitchKind, msrAlterationKind>::const_iterator
      iBegin = fHarpPedalsAlterationKindsMap.begin (),
      iEnd   = fHarpPedalsAlterationKindsMap.end (),
      i      = iBegin;
      
    for ( ; ; ) {
      // print the pedal and its alteration
      s <<
        msrDiatonicPitchKindAsString (
          (*i).first) <<
        " " <<
        msrAlterationKindAsString (
          (*i).second);
      if (++i == iEnd) break;
      s << ", ";
    } // for

    gIndenter--;
  }

  else {
    s <<
      "empty";
  }
   
  return s.str ();
}

void msrHarpPedalsTuning::print (ostream& os)
{
  os <<
    "HarpPedalsTuning" <<
    ", line " << fInputLineNumber;
    
  if (fHarpPedalsAlterationKindsMap.size ()) {
    gIndenter++;

    os <<
      endl;
      
    map<msrDiatonicPitchKind, msrAlterationKind>::const_iterator
      iBegin = fHarpPedalsAlterationKindsMap.begin (),
      iEnd   = fHarpPedalsAlterationKindsMap.end (),
      i      = iBegin;
      
    for ( ; ; ) {
      // print the pedal and its alteration
      os <<
        msrDiatonicPitchKindAsString (
          (*i).first) <<
        " " <<
        msrAlterationKindAsString (
          (*i).second);
      if (++i == iEnd) break;
      os << ", ";
    } // for

    gIndenter--;
  }

  else {
    os <<
      " empty" <<
      endl;
  }

  os <<
    endl;
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
  if (gTraceOptions->fTraceVoices) {
    gLogIOstream <<
      "Setting the name of '" <<
      voiceKindAsString () <<
      "' voice \"" <<
      "\"" <<
      " from number: " << voiceNumber <<
      endl;
  }

  switch (fVoiceKind) {
    case msrVoice::kRegularVoice:
      fVoiceName =
        fVoiceStaffUplink->getStaffName() +
        "_Voice_" +
        int2EnglishWord (voiceNumber);
      break;
      
    case msrVoice::kHarmonyVoice:
      fVoiceName =
        fVoiceStaffUplink->getStaffName() +
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
  // in registerVoiceInRegularVoicesMap()
  fRegularVoiceStaffSequentialNumber = -1;

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
        fVoiceStaffUplink->getStaffName() +
        "_FIGURED_BASS_Voice";
      break;
  } // switch
  
  if (gTraceOptions->fTraceVoices) {
    gLogIOstream <<
      "Initializing voice \"" << fVoiceName <<
      "\" in staff \"" <<
      fVoiceStaffUplink->getStaffName () <<
      "\"" <<
      endl;
  }

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

  // should the initial last segment be created?
  switch (voiceCreateInitialLastSegmentKind) {
    case msrVoice::kCreateInitialLastSegmentYes:
      if (gTraceOptions->fTraceVoices || gTraceOptions->fTraceSegments) {
        gLogIOstream <<
          "Creating an initial voice last segment for voice \"" <<
          fVoiceName <<
          "\" in staff \"" <<
          fVoiceStaffUplink->getStaffName () <<
          "\"" <<
          endl;
      }
      
      createNewLastSegmentForVoice (
        fInputLineNumber);
      break;
      
    case msrVoice::kCreateInitialLastSegmentNo:
      if (gTraceOptions->fTraceVoices || gTraceOptions->fTraceSegments) {
        gLogIOstream <<
          "NO initial voice last segment is created for voice \"" <<
          fVoiceName <<
          "\" in staff \"" <<
          fVoiceStaffUplink->getStaffName () <<
          "\"" <<
          endl;
      }
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
}

void msrVoice::changeVoiceIdentity ( // after a deep copy
  int voiceNumber)
{
  if (gTraceOptions->fTraceVoices) {
    gLogIOstream <<
      "Changing identity of voice \"" <<
      getVoiceName () <<
      "\"" <<
      ", number: " << voiceNumber <<
      endl;
  }

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
  if (gTraceOptions->fTraceVoices) {
    gLogIOstream <<
      "Creating a newborn clone of voice \"" <<
      getVoiceName () <<
      "\"" <<
      endl;
  }

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

  // sanity check
  msrAssert(
    containingStaff != nullptr,
    "containingStaff is null");
    
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
    if (gTraceOptions->fTraceVoices) {
      gLogIOstream <<
        "There are " <<
        numberOfInitialElements <<
        " initial elements in voice \"" <<
        getVoiceName () <<
        "\"" <<
        endl;
    }

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
    if (gTraceOptions->fTraceVoices) {
      gLogIOstream <<
        "There are no initial elements in voice \"" <<
        getVoiceName () <<
        "\" to be deep copied" <<
        endl;
    }
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
    if (gTraceOptions->fTraceVoices) {
      gLogIOstream <<
        "There is no last segment in voice \"" <<
        getVoiceName () <<
        "\" to be deep copied" <<
        endl;
    }
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
      addStanzaToVoiceWithoutCatchUp (
        stanza->
          createStanzaDeepCopy (
            voiceDeepCopy));
  } // for

  // uplinks
  voiceDeepCopy->fVoiceStaffUplink =
    containingStaff;

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

  if (gTraceOptions->fTraceMeasures || gTraceOptions->fTraceVoices) {
    gLogIOstream <<
      "Creating measure number '" <<
      measureNumber <<
      "' and appending it to voice \"" << getVoiceName () << "\"" <<
      "', line " << inputLineNumber <<
      endl;
  }

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
}

void msrVoice::setNextMeasureNumberInVoice (
  int    inputLineNumber,
  string nextMeasureNumber)
{
  if (gTraceOptions->fTraceMeasures || gTraceOptions->fTraceVoices) {
    gLogIOstream <<
      "Setting next measure number to '" <<
      nextMeasureNumber <<
      ", in voice \"" << getVoiceName () << "\"" <<
      "', line " << inputLineNumber <<
      endl;
  }

  fVoiceLastSegment->
    setNextMeasureNumberInSegment (
      inputLineNumber,
      nextMeasureNumber);

  // is there a pending multiple rest in this voice?
  if (fVoiceMultipleRestWaitingForItsNextMeasureNumber) {      
    // yes
    if (gTraceOptions->fTraceRepeats) {
      gLogIOstream <<
        "There is a multiple rest waiting for its next measure number" <<
        ", fVoiceRemainingRestMeasures = " <<
        fVoiceRemainingRestMeasures <<
        "' in voice \"" <<
        fVoiceName << "\"" <<
        endl;
    }

    fVoiceRemainingRestMeasures--;

    // is this the last measure in the row?
    if (fVoiceRemainingRestMeasures == 0) {
      // yes, set waiting multiple rest's next measure number
      if (gTraceOptions->fTraceRepeats) {
        gLogIOstream <<
          "Setting multiple rest next measure number to '" <<
          nextMeasureNumber <<
          "' in voice \"" <<
          fVoiceName << "\"" <<
          endl;
      }

      fVoiceMultipleRestWaitingForItsNextMeasureNumber->
        setMultipleRestNextMeasureNumber (
          nextMeasureNumber);

      // forget about this waiting multiple rest
      fVoiceMultipleRestWaitingForItsNextMeasureNumber = nullptr;
    }
  }
}

void msrVoice::createNewLastSegmentForVoice (
  int inputLineNumber)
{
  // create the segment
  if (gTraceOptions->fTraceSegments || gTraceOptions->fTraceVoices) {
    gLogIOstream <<
      "Creating a new segment for voice \"" <<
      getVoiceName () << "\"" <<
      ", line " << inputLineNumber <<
      endl;
  }

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
  if (gTraceOptions->fTraceVoices) {
    gLogIOstream <<
      "Creating a new segment with first measure '" <<
      firstMeasure->getMeasureNumber () <<
      "' for voice \"" <<
      getVoiceName () << "\"" <<
      ", line " << inputLineNumber <<
      endl;
  }

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

void msrVoice::createNewLastSegmentAndANewMeasureAfterARepeat (
  int inputLineNumber,
  int fullMeasureLength)
{
/*
  This method is used for repeats
  It may lead to several measures having the same number,
  in case a bar line cuts them apart
  If this measure remains empty because the end of measure
  follows the barline, it will be removed upon finalizeMeasure()
*/

  // finalize the current measure
  finalizeCurrentMeasureInVoice (
    inputLineNumber);

  // create the segment
  if (gTraceOptions->fTraceVoices) {
    gLogIOstream <<
      "Creating a new segment containing a new measure '" <<
      fVoiceCurrentMeasureNumber <<
      "'for voice \"" <<
      getVoiceName () << "\"" <<
      ", line " << inputLineNumber <<
      endl;
  }

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
  newMeasure->setFullMeasureLength (
    fullMeasureLength);
    
  // set it as being created after a repeat
  newMeasure->
    setMeasureCreatedAfterARepeatKind (
      msrMeasure::kMeasureCreatedAfterARepeatYes);
    
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
  if (gTraceOptions->fTraceLyrics) {
    gLogIOstream <<
      "Adding stanza " << stanza->getStanzaName () <<
      " (" << stanzaNumber <<
      ") to voice \"" << getVoiceName () << "\"" <<
      endl;
  }

  // add the stanza to this voice
  fVoiceStanzasMap [stanzaNumber] = stanza;
}

void msrVoice::addStanzaToVoiceWithoutCatchUp (S_msrStanza stanza)
{
  // get stanza number
  string stanzaNumber =
    stanza->getStanzaNumber ();
    
  // register stanza in this voice
  if (gTraceOptions->fTraceLyrics) {
    gLogIOstream <<
      "Adding stanza " << stanza->getStanzaName () <<
      " (" << stanzaNumber <<
      ") to voice \"" << getVoiceName () << "\"" <<
      endl;
  }

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

void msrVoice::appendCommentToVoice (S_msrComment comment)
{
  if (gTraceOptions->fTraceClefs || gTraceOptions->fTraceVoices) {
    gLogIOstream <<
      "Appending comment '" << comment->asString () <<
      "' to voice \"" << getVoiceName () << "\"" <<
      endl;
  }

  // create the voice last segment and first measure if needed
  appendAFirstMeasureToVoiceIfNotYetDone (
    comment->getInputLineNumber ());

  // append clef to last segment
  fVoiceLastSegment->
    appendCommentToSegment (comment);
}

void msrVoice::appendClefToVoice (S_msrClef clef)
{
  if (gTraceOptions->fTraceClefs || gTraceOptions->fTraceVoices) {
    gLogIOstream <<
      "Appending clef '" << clef->asString () <<
      "' to voice \"" << getVoiceName () << "\"" <<
      endl;
  }

  // create the voice last segment and first measure if needed
  appendAFirstMeasureToVoiceIfNotYetDone (
    clef->getInputLineNumber ());

  // append clef to last segment
  fVoiceLastSegment->
    appendClefToSegment (clef);
}

void msrVoice::appendKeyToVoice (S_msrKey key)
{
  if (gTraceOptions->fTraceKeys || gTraceOptions->fTraceVoices) {
    gLogIOstream <<
      "Appending key '" << key->asString () <<
      "' to voice \"" << getVoiceName () << "\"" <<
      endl;
  }
  
  // create the voice last segment and first measure if needed
  appendAFirstMeasureToVoiceIfNotYetDone (
    key->getInputLineNumber ());

  // append key to last segment
  fVoiceLastSegment->
    appendKeyToSegment (key);
}

void msrVoice::appendTimeToVoice (S_msrTime time)
{
  if (gTraceOptions->fTraceTimes || gTraceOptions->fTraceVoices) {
    gLogIOstream <<
      "Appending time '" << time->asString () <<
      "' to voice \"" << getVoiceName () << "\"" <<
      endl;
  }
  
  // create the voice last segment and first measure if needed
  appendAFirstMeasureToVoiceIfNotYetDone (
    time->getInputLineNumber ());

  // append time to the last segment
  fVoiceLastSegment->
    appendTimeToSegment (time);
}

void msrVoice::appendTimeToVoiceClone (S_msrTime time)
{
  if (gTraceOptions->fTraceTimes || gTraceOptions->fTraceVoices) {
    gLogIOstream <<
      "Appending time '" << time->asString () <<
      "' to voice clone \"" << getVoiceName () << "\"" <<
      endl;
  }
  
  // create the voice last segment and first measure if needed
  appendAFirstMeasureToVoiceIfNotYetDone (
    time->getInputLineNumber ());

  // append time to the last segment
  fVoiceLastSegment->
    appendTimeToSegmentClone (time);
}

void msrVoice::appendHarmonyToVoice (S_msrHarmony harmony)
{
  if (gTraceOptions->fTraceHarmonies || gTraceOptions->fTraceVoices) {
    gLogIOstream <<
      "Appending harmony '" << harmony->asString () <<
      "' to voice \"" << getVoiceName () << "\"" <<
      endl;
  }

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
  if (gTraceOptions->fTraceHarmonies || gTraceOptions->fTraceVoices) {
    gLogIOstream <<
      "Appending harmony '" << harmony->asString () <<
      "' to voice clone \"" << getVoiceName () << "\"" <<
      endl;
  }
      
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
  if (gTraceOptions->fTraceFiguredBass || gTraceOptions->fTraceVoices) {
    gLogIOstream <<
      "Appending figured bass '" << figuredBass->asString () <<
      "' to voice \"" << getVoiceName () << "\"" <<
      endl;
  }

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
  if (gTraceOptions->fTraceFiguredBass || gTraceOptions->fTraceVoices) {
    gLogIOstream <<
      "Appending figured bass '" << figuredBass->asString () <<
      "' to voice clone \"" << getVoiceName () << "\"" <<
      endl;
  }
      
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
  if (gTraceOptions->fTraceVoices || gTraceOptions->fTraceMeasures) {
    gLogIOstream <<
      "Padding up to measure length '" << measureLength <<
      "' in voice \"" <<
      getVoiceName () <<
      "\", line " << inputLineNumber <<
      endl;
  }

  fVoiceLastSegment->
    padUpToMeasureLengthInSegment (
      inputLineNumber, measureLength);
}

void msrVoice::appendTransposeToVoice (
  S_msrTranspose transpose)
{
  if (gTraceOptions->fTraceTranspositions || gTraceOptions->fTraceVoices) {
    gLogIOstream <<
      "Appending transpose '" <<
      transpose->asString () <<
      "' to voice \"" << getVoiceName () << "\"" <<
      endl;
  }

  // create the voice last segment and first measure if needed
  appendAFirstMeasureToVoiceIfNotYetDone (
    transpose->getInputLineNumber ());

  fVoiceLastSegment->
    appendTransposeToSegment (transpose);
}

void msrVoice::appendPartNameDisplayToVoice (
  S_msrPartNameDisplay partNameDisplay)
{
  if (gTraceOptions->fTraceTranspositions || gTraceOptions->fTraceVoices) {
    gLogIOstream <<
      "Appending part name display '" <<
      partNameDisplay->asString () <<
      "' to voice \"" << getVoiceName () << "\"" <<
      endl;
  }

  // create the voice last segment and first measure if needed
  appendAFirstMeasureToVoiceIfNotYetDone (
    partNameDisplay->getInputLineNumber ());

  fVoiceLastSegment->
    appendPartNameDisplayToSegment (partNameDisplay);
}

void msrVoice::appendPartAbbreviationDisplayToVoice (
  S_msrPartAbbreviationDisplay partAbbreviationDisplay)
{
  if (gTraceOptions->fTraceTranspositions || gTraceOptions->fTraceVoices) {
    gLogIOstream <<
      "Appending part abbreviation display '" <<
      partAbbreviationDisplay->asString () <<
      "' to voice \"" << getVoiceName () << "\"" <<
      endl;
  }

  // create the voice last segment and first measure if needed
  appendAFirstMeasureToVoiceIfNotYetDone (
    partAbbreviationDisplay->getInputLineNumber ());

  fVoiceLastSegment->
    appendPartAbbreviationDisplayToSegment (partAbbreviationDisplay);
}

void msrVoice::appendStaffDetailsToVoice (
  S_msrStaffDetails staffDetails)
{
  if (gTraceOptions->fTraceStaves || gTraceOptions->fTraceVoices) {
    gLogIOstream <<
      "Appending staff details '" <<
      staffDetails->asShortString () <<
      "' to voice \"" << getVoiceName () << "\"" <<
      endl;
  }

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
  if (gTraceOptions->fTraceWords) {
    gLogIOstream <<
      "Appending words '" << words->asString () <<
      "' to voice " << getVoiceName () << endl;
  }

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
  if (gTraceOptions->fTraceBasic || gTraceOptions->fTraceVoices) {
    gLogIOstream <<
      "Appending accordion registration '" <<
      accordionRegistration->asString () <<
      "' to voice \"" << getVoiceName () << "\"" <<
      endl;
  }

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
  if (gTraceOptions->fTraceBasic || gTraceOptions->fTraceVoices) {
    gLogIOstream <<
      "Appending harp pedals tuning '" <<
      harpPedalsTuning->asString () <<
      "' to voice \"" << getVoiceName () << "\"" <<
      endl;
  }

  // create the voice last segment and first measure if needed
  appendAFirstMeasureToVoiceIfNotYetDone (
    harpPedalsTuning->getInputLineNumber ());

  fVoiceLastSegment->
    appendHarpPedalsTuningToSegment (
      harpPedalsTuning);
}

void msrVoice::appendRehearsalToVoice (S_msrRehearsal rehearsal)
{
  if (gTraceOptions->fTraceRepeats) {
    gLogIOstream <<
      "Appending rehearsal '" << rehearsal->getRehearsalText () <<
      "' to voice \"" << getVoiceName () << "\"" <<
      endl;
  }

  // create the voice last segment and first measure if needed
  appendAFirstMeasureToVoiceIfNotYetDone (
    rehearsal->getInputLineNumber ());

  fVoiceLastSegment->
    appendRehearsalToSegment (rehearsal);
}

void msrVoice::appendVoiceStaffChangeToVoice (
  S_msrVoiceStaffChange voiceStaffChange)
{
  if (gTraceOptions->fTraceVoices || gTraceOptions->fTraceStaves) {
    gLogIOstream <<
      "Append voice staff change " <<
      voiceStaffChange->asString () <<
      " to voice " <<
      " \"" << getVoiceName () << "\"" <<
      endl;
  }

  // create the voice last segment and first measure if needed
  appendAFirstMeasureToVoiceIfNotYetDone (
    voiceStaffChange->getInputLineNumber ());

  fVoiceLastSegment->
    appendVoiceStaffChangeToSegment (voiceStaffChange);
}

void msrVoice::appendNoteToVoice (S_msrNote note) {
  int inputLineNumber =
    note->getInputLineNumber ();
    
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
      // register actual note
      fVoiceActualNotesCounter++;
      fMusicHasBeenInsertedInVoice = true;
      break;
  } // switch
  
  // create the voice last segment and first measure if needed
  appendAFirstMeasureToVoiceIfNotYetDone (
    inputLineNumber);

  // append the note to the last segment
  fVoiceLastSegment->
    appendNoteToSegment (note);

  // register note as the last appended one into this voice
  fVoiceLastAppendedNote = note;

/* JMI
  // should a skip be added to the voice harmony voice if any?
  if (
    fHarmonyVoiceForRegularVoice
      &&
    ! note->getNoteHarmony ()) {
      // create skip with same duration as note
      S_msrNote
        skip =
          msrNote::createSkipNote (
            note->               getInputLineNumber (),
            note->               getNoteDisplayWholeNotes (), // would be 0/1 otherwise JMI
            note->               getNoteDisplayWholeNotes (),
            note->               getNoteDotsNumber (),
            fHarmonyVoiceForRegularVoice-> getRegularVoiceStaffSequentialNumber (), // JMI
            fHarmonyVoiceForRegularVoice-> getVoiceNumber ());

      // append it to the voice harmony voice
      fHarmonyVoiceForRegularVoice->
        appendNoteToVoice (skip);
  }
  */
}

void msrVoice::appendNoteToVoiceClone (S_msrNote note) {
  int inputLineNumber =
    note->getInputLineNumber ();
    
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
      // register actual note
      fVoiceActualNotesCounter++;
      fMusicHasBeenInsertedInVoice = true;
      break;
  } // switch
  
  // append the note to the last segment
  fVoiceLastSegment->
    appendNoteToSegmentClone (note);
}

void msrVoice::appendDoubleTremoloToVoice (
  S_msrDoubleTremolo doubleTremolo)

{
  if (gTraceOptions->fTraceTremolos) {
    gLogIOstream <<
      "Appending double tremolo '" <<
      doubleTremolo->asShortString () <<
      "' to voice \"" << getVoiceName () << "\"" <<
      endl;
  }

  // create the voice last segment and first measure if needed
  appendAFirstMeasureToVoiceIfNotYetDone (
    doubleTremolo->getInputLineNumber ());

  fVoiceLastSegment->
    appendDoubleTremoloToSegment (doubleTremolo);

  fMusicHasBeenInsertedInVoice = true;
}

void msrVoice::appendChordToVoice (S_msrChord chord)
{
  if (gTraceOptions->fTraceChords) {
    gLogIOstream <<
      "Appending chord '" << chord->asString () <<
      "' to voice \"" << getVoiceName () << "\"" <<
      endl;
  }

  // create the voice last segment and first measure if needed
  appendAFirstMeasureToVoiceIfNotYetDone (
    chord->getInputLineNumber ());

  fVoiceLastSegment->
    appendChordToSegment (chord);

  fMusicHasBeenInsertedInVoice = true;
}

void msrVoice::appendTupletToVoice (S_msrTuplet tuplet)
{
  if (gTraceOptions->fTraceTuplets) {
    gLogIOstream <<
      "Appending tuplet '" << tuplet->asString () <<
      "' to voice \"" << getVoiceName () << "\"" <<
      endl;
  }

  // create the voice last segment and first measure if needed
  appendAFirstMeasureToVoiceIfNotYetDone (
    tuplet->getInputLineNumber ());

  fVoiceLastSegment->
    appendTupletToSegment (tuplet);

  fMusicHasBeenInsertedInVoice = true;
}

void msrVoice::appendGraceNotesToVoice (S_msrGraceNotes graceNotes)
{
  if (gTraceOptions->fTraceGraceNotes) {
    gLogIOstream <<
      "Appending grace notes " << // JMI graceNotes <<
      " to voice \"" << getVoiceName () << "\"" <<
      endl;
  }

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
  
  if (gTraceOptions->fTraceGraceNotes) {
    gLogIOstream <<
      "Prepending grace notes '" <<
      graceNotes->asString () <<
      "' to voice \"" << getVoiceName () << "\"" <<
      endl;
  }

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
  if (gTraceOptions->fTraceGraceNotes) {
    gLogIOstream <<
      "Appending after grace notes " << // JMI AfterGraceNotes <<
      " to voice \"" << getVoiceName () << "\"" <<
      endl;
  }

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
  if (gTraceOptions->fTraceGraceNotes) {
    gLogIOstream <<
      "Prepending after grace notes " << // JMI AfterGraceNotes <<
      " to voice \"" << getVoiceName () << "\"" <<
      endl;
  }

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
  if (gTraceOptions->fTraceLyrics) {
    gLogIOstream <<
      "Appending syllable '" <<
      syllable->asString () <<
      "' to voice \"" << fVoiceName << "\"" <<
      endl;
  }

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
  if (gTraceOptions->fTraceMeasures) {
    gLogIOstream <<
      "Appending bar check '" << barCheck->asString () <<
      "' to voice \"" << getVoiceName () <<  "\"" <<
      endl;
  }

  // create the voice last segment and first measure if needed
  appendAFirstMeasureToVoiceIfNotYetDone (
    barCheck->getInputLineNumber ());

  fVoiceLastSegment->
    appendBarCheckToSegment (barCheck);
}

void msrVoice::appendBarNumberCheckToVoice (
  S_msrBarNumberCheck barNumberCheck)
{
  if (gTraceOptions->fTraceMeasures) {
    gLogIOstream <<
      "Appending barnumber check '" <<
      barNumberCheck->asString () <<
      "' to voice \"" << getVoiceName () <<  "\"" <<
      endl;
  }

  // create the voice last segment and first measure if needed
  appendAFirstMeasureToVoiceIfNotYetDone (
    barNumberCheck->getInputLineNumber ());

  fVoiceLastSegment->
    appendBarNumberCheckToSegment (barNumberCheck);
}

void msrVoice::appendLineBreakToVoice (S_msrLineBreak lineBreak)
{
  if (gTraceOptions->fTraceMeasures) {
    gLogIOstream <<
      "Appending line break '" << lineBreak->asString () <<
      "' to voice \"" << getVoiceName () << "\"" <<
      endl;
  }

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
  if (gTraceOptions->fTraceMeasures) {
    gLogIOstream <<
      "Appending page break '" << pageBreak->asString () <<
      "' to voice \"" << getVoiceName () << "\"" <<
      endl;
  }

  // create the voice last segment and first measure if needed
  appendAFirstMeasureToVoiceIfNotYetDone (
    pageBreak->getInputLineNumber ());

  fVoiceLastSegment->
    appendPageBreakToSegment (pageBreak);
}

void msrVoice::prependOtherElementToVoice (S_msrElement elem) {
  if (gTraceOptions->fTraceVoices) {
    gLogIOstream <<
      "Prepending other element '" << elem <<
      "' to voice \"" << getVoiceName () << "\"" <<
      endl;
  }

  // create the voice last segment and first measure if needed
  appendAFirstMeasureToVoiceIfNotYetDone (
    elem->getInputLineNumber ());

  fVoiceLastSegment->
    prependOtherElementToSegment (elem);
}

void msrVoice::appendOtherElementToVoice (S_msrElement elem) {
  if (gTraceOptions->fTraceVoices) {
    gLogIOstream <<
      "Appending other element '" << elem <<
      "' to voice \"" << getVoiceName () << "\"" <<
      endl;
  }

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
          int fullMeasureLength =
            fVoiceLastSegment->
              getSegmentMeasuresList ().back ()->
                getFullMeasureLength ();
                
          // finalize current measure in voice
          finalizeCurrentMeasureInVoice (
            inputLineNumber);
  
          if (gTraceOptions->fTraceRepeats || gTraceOptions->fTraceVoices) {
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
  
          // move current last segment to the list of initial elements
          if (gTraceOptions->fTraceRepeats) {
            gLogIOstream <<
              "Appending voice last segment to the initial elements in voice \"" <<
              getVoiceName () <<
              "\"" <<
              ", line " << inputLineNumber <<
              endl;
          }
                        
          fVoiceInitialElementsList.push_back (
            fVoiceLastSegment);
  
          // create a new last segment containing a new measure for the voice
          if (gTraceOptions->fTraceSegments || gTraceOptions->fTraceVoices) {
            gLogIOstream <<
              "Creating a new last segment containing a new measure for voice \"" <<
              fVoiceName << "\"" <<
              ", line " << inputLineNumber <<
              endl;
          }
  
          createNewLastSegmentAndANewMeasureAfterARepeat (
            inputLineNumber,
            fullMeasureLength);
  
          if (gTraceOptions->fTraceRepeats || gTraceOptions->fTraceVoices) {
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
          int fullMeasureLength =
            fVoiceLastSegment->
              getSegmentMeasuresList ().back ()->
                  getFullMeasureLength ();
                
          // finalize current measure in voice
          finalizeCurrentMeasureInVoice (
            inputLineNumber);
  
          if (gTraceOptions->fTraceRepeats || gTraceOptions->fTraceVoices) {
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
  
          // move current last segment to the list of initial elements
          if (gTraceOptions->fTraceRepeats) {
            gLogIOstream <<
              "Appending voice last segment to the initial elements in voice \"" <<
              getVoiceName () <<
              "\"" <<
              ", line " << inputLineNumber <<
              endl;
          }
                        
          fVoiceInitialElementsList.push_back (
            fVoiceLastSegment);
  
          // create a new last segment containing a new measure for the voice
          if (gTraceOptions->fTraceSegments || gTraceOptions->fTraceVoices) {
            gLogIOstream <<
              "Creating a new last segment containing a new measure for voice \"" <<
              fVoiceName << "\"" <<
              ", line " << inputLineNumber <<
              endl;
          }
  
          createNewLastSegmentAndANewMeasureAfterARepeat (
            inputLineNumber,
            fullMeasureLength);
  
          if (gTraceOptions->fTraceRepeats || gTraceOptions->fTraceVoices) {
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
        }
      }
      break;
  } // switch
}

void msrVoice::createRepeatUponItsEndAndAppendItToVoice (
  int inputLineNumber,
  int repeatTimes)
{
  switch (fVoiceKind) {
    case msrVoice::kRegularVoice:
    case msrVoice::kHarmonyVoice:
    case msrVoice::kFiguredBassVoice:
      {
        // fetch last measure's full measure length
        int fullMeasureLength =
          fVoiceLastSegment->
            getSegmentMeasuresList ().back ()->
                getFullMeasureLength ();
              
        // finalize current measure in voice
        finalizeCurrentMeasureInVoice (
          inputLineNumber);
          
        if (gTraceOptions->fTraceRepeats || gTraceOptions->fTraceVoices) {
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

        // create the repeat
        if (gTraceOptions->fTraceRepeats) {
          gLogIOstream <<
            "Creating and appending a repeat in voice \"" <<
            getVoiceName () <<
            "\"" <<
            ", line " << inputLineNumber <<
            endl;
        }
      
        S_msrRepeat
          newRepeat =
            msrRepeat::create (
              inputLineNumber,
              repeatTimes,
              this);

        // create a repeat common part from the new segment
        if (gTraceOptions->fTraceRepeats) {
          gLogIOstream <<
            "Creating a repeat common part in voice \"" <<
            getVoiceName () <<
            "\"" <<
            ", line " << inputLineNumber <<
            endl;
        }
      
        S_msrRepeatCommonPart
          repeatCommonPart =
            msrRepeatCommonPart::create (
              inputLineNumber,
              newRepeat);

        // is there another repeat to nest newRepeat into?
        if (fVoiceCurrentRepeat) {
          // yes
          // move the voice initial elements to the new repeat common part
          if (gTraceOptions->fTraceRepeats) {
            gLogIOstream <<
              "Moving the initial elements in voice \"" <<
              getVoiceName () <<
              "\" to the new repeat common part" <<
              ", line " << inputLineNumber <<
              endl;
          }
  
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
          if (gTraceOptions->fTraceRepeats) {
            gLogIOstream <<
              "Append the voice last segment to the repeat common part in voice \"" <<
              getVoiceName () <<
              "\"" <<
              ", line " << inputLineNumber <<
              endl;
          }
        
          repeatCommonPart->
            appendElementToRepeatCommonPart (
              fVoiceLastSegment);  
        }
        
        else {
          // no
          // append the voice last segment to the new repeat common part
          if (gTraceOptions->fTraceRepeats) {
            gLogIOstream <<
              "Appending the voice last segment in voice \"" <<
              getVoiceName () <<
              "\" to the new repeat common part" <<
              ", line " << inputLineNumber <<
              endl;
          }

          repeatCommonPart->
            appendElementToRepeatCommonPart (
            fVoiceLastSegment);
        }

        // set newRepeat's common part
        if (gTraceOptions->fTraceRepeats) {
          gLogIOstream <<
            "Setting repeat common part in voice \"" <<
            getVoiceName () <<
            "\"" <<
            ", line " << inputLineNumber <<
            endl;
        }

        newRepeat->
          setRepeatCommonPart (
            repeatCommonPart);

        // append newRepeat to the list of initial elements
        if (gTraceOptions->fTraceRepeats) {
          gLogIOstream <<
            "Appending new repeat to the initial elements in voice \"" <<
            getVoiceName () <<
            "\"" <<
            ", line " << inputLineNumber <<
            endl;
        }
                      
        fVoiceInitialElementsList.push_back (
          newRepeat);
      
        // set voice current repeat
        fVoiceCurrentRepeat =
          newRepeat;

        // create a new last segment containing a new measure for the voice
        if (gTraceOptions->fTraceSegments || gTraceOptions->fTraceVoices) {
          gLogIOstream <<
            "Creating a new last segment containing a new measure for voice \"" <<
            fVoiceName << "\"" <<
            ", line " << inputLineNumber <<
            endl;
        }

        createNewLastSegmentAndANewMeasureAfterARepeat (
          inputLineNumber,
          fullMeasureLength);

        if (gTraceOptions->fTraceRepeats || gTraceOptions->fTraceVoices) {
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
        int fullMeasureLength =
          fVoiceLastSegment->
            getSegmentMeasuresList ().back ()->
                getFullMeasureLength ();
              
        // finalize current measure in voice
        finalizeCurrentMeasureInVoice (
          inputLineNumber);
          */
          
        // create the repeat
        if (gTraceOptions->fTraceRepeats) {
          gLogIOstream <<
            "Creating and appending a repeat in voice \"" <<
            getVoiceName () <<
            "\"" <<
            ", line " << inputLineNumber <<
            endl;
        }
      
        fVoiceCurrentRepeat =
          msrRepeat::create (
            inputLineNumber,
            repeatTimes,
            this);

        // create a repeat common part
        if (gTraceOptions->fTraceRepeats) {
          gLogIOstream <<
            "Creating a repeat common part from current last segment in voice \"" <<
            getVoiceName () <<
            "\"" <<
            ", line " << inputLineNumber <<
            endl;
        }
      
        S_msrRepeatCommonPart
          repeatCommonPart =
            msrRepeatCommonPart::create (
              inputLineNumber,
              fVoiceCurrentRepeat);

        // move voice last segment into the repeat common part
        if (gTraceOptions->fTraceRepeats) {
          gLogIOstream <<
            "Append the voice last segment to the repeat common part in voice \"" <<
            getVoiceName () <<
            "\"" <<
            ", line " << inputLineNumber <<
            endl;
        }
      
        repeatCommonPart->
          appendElementToRepeatCommonPart (
            fVoiceLastSegment);

        // set fVoiceCurrentRepeat's common part
        if (gTraceOptions->fTraceRepeats) {
          gLogIOstream <<
            "Setting repeat common part in voice \"" <<
            getVoiceName () <<
            "\"" <<
            ", line " << inputLineNumber <<
            endl;
        }

        fVoiceCurrentRepeat->
          setRepeatCommonPart (
            repeatCommonPart);
          
        // append the repeat to the list of initial elements
        if (gTraceOptions->fTraceRepeats) {
          gLogIOstream <<
            "Appending repeat to the initial elements in voice \"" <<
            getVoiceName () <<
            "\"" <<
            ", line " << inputLineNumber <<
            endl;
        }
                      
        fVoiceInitialElementsList.push_back (
          fVoiceCurrentRepeat);
      
        // create a new last segment containing a new measure for the voice
        if (gTraceOptions->fTraceSegments || gTraceOptions->fTraceVoices) {
          gLogIOstream <<
            "Creating a new last segment with the first ending measure for voice \"" <<
            fVoiceName << "\"" <<
            ", line " << inputLineNumber <<
            endl;
        }

        createNewLastSegmentFromFirstMeasureForVoice (
          inputLineNumber,
          firstEndingMeasure);

        if (gTraceOptions->fTraceRepeats || gTraceOptions->fTraceVoices) {
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
        int fullMeasureLength =
          fVoiceLastSegment->
            getSegmentMeasuresList ().back ()->
                getFullMeasureLength ();
              
        // finalize current measure in voice
        finalizeCurrentMeasureInVoice (
          inputLineNumber);
          */
          
        // create the repeat
        if (gTraceOptions->fTraceRepeats) {
          gLogIOstream <<
            "Creating and appending a repeat in voice \"" <<
            getVoiceName () <<
            "\"" <<
            ", line " << inputLineNumber <<
            endl;
        }
      
        fVoiceCurrentRepeat =
          msrRepeat::create (
            inputLineNumber,
            repeatTimes,
            this);

        // create a repeat common part
        if (gTraceOptions->fTraceRepeats) {
          gLogIOstream <<
            "Creating a repeat common part from current last segment in voice \"" <<
            getVoiceName () <<
            "\"" <<
            ", line " << inputLineNumber <<
            endl;
        }
      
        S_msrRepeatCommonPart
          repeatCommonPart =
            msrRepeatCommonPart::create (
              inputLineNumber,
              fVoiceCurrentRepeat);

        // move voice initial elements into the repeat common part
        if (gTraceOptions->fTraceRepeats) {
          gLogIOstream <<
            "Move the voice initial elements to the repeat common part in voice \"" <<
            getVoiceName () <<
            "\"" <<
            ", line " << inputLineNumber <<
            endl;
        }

        for (
          list<S_msrElement>::const_iterator i = fVoiceInitialElementsList.begin ();
          i != fVoiceInitialElementsList.end ();
          i++) {
          repeatCommonPart->
            appendElementToRepeatCommonPart ((*i));
        } // for

        // empty the initial elements list
        fVoiceInitialElementsList.resize (0);
        
        // move voice last segment into the repeat common part
        if (gTraceOptions->fTraceRepeats) {
          gLogIOstream <<
            "Append the voice last segment to the repeat common part in voice \"" <<
            getVoiceName () <<
            "\"" <<
            ", line " << inputLineNumber <<
            endl;
        }
      
        repeatCommonPart->
          appendElementToRepeatCommonPart (
            fVoiceLastSegment);

        // set fVoiceCurrentRepeat's common part
        if (gTraceOptions->fTraceRepeats) {
          gLogIOstream <<
            "Setting repeat common part in voice \"" <<
            getVoiceName () <<
            "\"" <<
            ", line " << inputLineNumber <<
            endl;
        }

        fVoiceCurrentRepeat->
          setRepeatCommonPart (
            repeatCommonPart);
          
        // append the repeat to the list of initial elements
        if (gTraceOptions->fTraceRepeats) {
          gLogIOstream <<
            "Appending repeat to the initial elements in voice \"" <<
            getVoiceName () <<
            "\"" <<
            ", line " << inputLineNumber <<
            endl;
        }
                      
        fVoiceInitialElementsList.push_back (
          fVoiceCurrentRepeat);
      
        // create a new last segment containing a new measure for the voice
        if (gTraceOptions->fTraceSegments || gTraceOptions->fTraceVoices) {
          gLogIOstream <<
            "Creating a new last segment with the first ending measure for voice \"" <<
            fVoiceName << "\"" <<
            ", line " << inputLineNumber <<
            endl;
        }

        createNewLastSegmentFromFirstMeasureForVoice (
          inputLineNumber,
          firstEndingMeasure);

        if (gTraceOptions->fTraceRepeats || gTraceOptions->fTraceVoices) {
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
      
        // print current voice contents
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
            endl;
        }
            
        createNewLastSegmentFromFirstMeasureForVoice (
          inputLineNumber,
          firstReplicaMeasure);

        // print resulting voice contents
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

        // remove the next measure from the last segment's measure list
        voiceLastSegmentMeasureList.pop_back ();

        // create the measure repeat replicas contents
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
        
        fVoiceCurrentMeasuresRepeat->
          setMeasuresRepeatReplicas (
            measuresRepeatReplicas);

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

        fVoiceCurrentMeasuresRepeat->
          getMeasuresRepeatReplicas ()->
            setMeasuresRepeatReplicasSegment (
              fVoiceLastSegment);

        // append pending measure repeat to the list of initial elements
        fVoiceInitialElementsList.push_back (
          fVoiceCurrentMeasuresRepeat);

        // create a new last segment to collect the remainder of the voice,
        // containing the next, yet incomplete, measure
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
            endl;
        }
            
        createNewLastSegmentFromFirstMeasureForVoice (
          inputLineNumber,
          nextMeasureAfterMeasuresRepeat);

/* JMI
        // print resulting voice contents
        if (gTraceOptions->fTraceSegments || gTraceOptions->fTraceVoices) {
          gLogIOstream <<
            "==================> The current voice contents of voice \"" <<
            fVoiceName << "\" is:" <<
            endl;
        }

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
        int fullMeasureLength =
          fVoiceLastSegment->
            getSegmentMeasuresList ().back ()->
                getFullMeasureLength ();

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
        if (gTraceOptions->fTraceRepeats) {
          gLogIOstream <<
            "Creating and appending a measures repeat in voice \"" <<
            getVoiceName () <<
            "\"" <<
            ", line " << inputLineNumber <<
            endl;
        }
      
        fVoiceCurrentMeasuresRepeat =
          msrMeasuresRepeat::create (
            inputLineNumber,
            measuresRepeatMeasuresNumber,
            measuresRepeatSlashesNumber,
            this);

        // create a measures repeat pattern from current last segment
        if (gTraceOptions->fTraceRepeats) {
          gLogIOstream <<
            "Creating a measures repeat pattern from current last segment in voice \"" <<
            getVoiceName () <<
            "\"" <<
            ", line " << inputLineNumber <<
            endl;
        }
      
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
        if (gTraceOptions->fTraceRepeats) {
          gLogIOstream <<
            "Setting repeat common part in voice \"" <<
            getVoiceName () <<
            "\"" <<
            endl;
        }

        fVoiceCurrentMeasuresRepeat->
          setMeasuresRepeatPattern (
            measuresRepeatPattern);

        // create a new last segment containing a new measure for the voice
        if (gTraceOptions->fTraceSegments || gTraceOptions->fTraceVoices) {
          gLogIOstream <<
            "Creating a new last segment containing a new measure for voice \"" <<
            fVoiceName << "\"" <<
            endl;
        }

        createNewLastSegmentAndANewMeasureAfterARepeat (
          inputLineNumber,
          fullMeasureLength);

        // append the measures repeat to the list of initial elements
        if (gTraceOptions->fTraceRepeats) {
          gLogIOstream <<
            "Appending measures repeat to the list of initial elements in voice \"" <<
            getVoiceName () <<
            "\"" <<
            endl;
        }
                      
        fVoiceInitialElementsList.push_back (
          fVoiceCurrentMeasuresRepeat);
      
        // create a new last segment to collect the remainder of the voice,
        // containing the next, yet incomplete, measure
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
            endl;
        }
            
        createNewLastSegmentForVoice (
          inputLineNumber);

/* JMI
        // print resulting voice contents
        if (gTraceOptions->fTraceSegments || gTraceOptions->fTraceVoices) {
          gLogIOstream <<
            "==================> The current voice contents of voice \"" <<
            fVoiceName << "\" is:" <<
            endl;
        }

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
  if (gTraceOptions->fTraceRepeats) {
    gLogIOstream <<
      "Voice \"" <<
      getVoiceName () <<
      "\"" <<
      ", line " << inputLineNumber <<
      ", contains multiple rests" <<
      endl;
  }

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
            firstRestMeasure->getFullMeasureLength (),
            multipleRestMeasuresNumber,
            this);

         // remember fVoicePendingMultipleRest for later next measure number setting
        if (gTraceOptions->fTraceRepeats) {
          gLogIOstream <<
            "Registering multiple rest as waiting for its next measure number" <<
            ", multipleRestMeasuresNumber = " <<
            multipleRestMeasuresNumber <<
            " in voice \"" <<
            fVoiceName << "\"" <<
            endl;
        }
        
        fVoiceMultipleRestWaitingForItsNextMeasureNumber =
          fVoicePendingMultipleRest;

        fVoiceRemainingRestMeasures =
          multipleRestMeasuresNumber;

        if (gTraceOptions->fTraceRepeats) {
          gLogIOstream <<
            "Setting fVoiceRemainingRestMeasures to '" <<
            fVoiceRemainingRestMeasures <<
            "' in voice \"" <<
            fVoiceName << "\"" <<
            endl;
        }

        // create a new segment to collect the multiple rest measures,
        // containing the first, rest measure
        if (gTraceOptions->fTraceSegments || gTraceOptions->fTraceVoices) {
          gLogIOstream <<
            "Creating a new last segment containing the first rest measure in voice \"" <<
            fVoiceName << "\"" <<
            endl;
        }
            
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
        if (gTraceOptions->fTraceSegments || gTraceOptions->fTraceVoices) {
          gLogIOstream <<
            "The resulting voice contents of voice \"" <<
            fVoiceName << "\" is:" <<
            endl;
  
          gIndenter++;
          print (gLogIOstream);
          gIndenter--;
        }

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
        if (gTraceOptions->fTraceRepeats) {
          gLogIOstream <<
            "Appending the pending multiple rest to voice \"" <<
            getVoiceName () <<
            "\"" <<
            ", line " << inputLineNumber <<
            endl;
        }

/* JMI
        // print current voice contents
        if (gTraceOptions->fTraceSegments || gTraceOptions->fTraceVoices) {
          gLogIOstream <<
            "==================> The current voice contents of voice \"" <<
            fVoiceName << "\" is:" <<
            endl;

          gIndenter++;
          print (gLogIOstream);
          gIndenter--;
        }
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
        if (gTraceOptions->fTraceSegments || gTraceOptions->fTraceVoices) {
          gLogIOstream <<
            "Creating a measure pattern for voice \"" <<
            fVoiceName << "\" is:" <<
            endl;
        }

        S_msrMultipleRestContents
          multipleRestContents =
            msrMultipleRestContents::create (
              inputLineNumber,
              this);

        // set voice last segment as the multiple rest segment
        if (gTraceOptions->fTraceRepeats) {
          gLogIOstream <<
            "Setting current last segment as multiple rest segment in voice \"" <<
            getVoiceName () <<
            "\"" <<
            endl;
        }
      
        multipleRestContents->
          setMultipleRestContentsSegment (
            fVoiceLastSegment);

        // set multipleRestContents as the multiple rest contents
        if (gTraceOptions->fTraceRepeats) {
          gLogIOstream <<
            "Setting multiple rest contents in voice \"" <<
            getVoiceName () <<
            "\"" <<
            endl;
        }
      
        fVoicePendingMultipleRest->
          setMultipleRestContents (
            multipleRestContents);

        // append pending multiple rest to the list of initial elements
        fVoiceInitialElementsList.push_back (
          fVoicePendingMultipleRest);

/* JMI
        // print current voice contents
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
*/

        // create a new segment to collect the remainder of the voice,
        // containing the next, yet incomplete, measure
        if (gTraceOptions->fTraceSegments || gTraceOptions->fTraceVoices) {
          gLogIOstream <<
            "Creating a new last segment with the measure after the multiple rests for voice \"" <<
            fVoiceName << "\"" <<
            endl;
        }
            
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
        if (gTraceOptions->fTraceSegments || gTraceOptions->fTraceVoices) {
          gLogIOstream <<
            "The resulting voice contents of voice \"" <<
            fVoiceName << "\" is:" <<
            endl;

          gIndenter++;
          print (gLogIOstream);
          gIndenter--;
        }

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

/*
        // print current voice contents
        if (gTraceOptions->fTraceSegments || gTraceOptions->fTraceVoices) {
          gLogIOstream <<
            "==================> The current voice contents of voice \"" <<
            fVoiceName << "\" is:" <<
            endl;
        }

        gIndenter++;
        print (gLogIOstream);
        gIndenter--;
*/

        // append pending multiple rest to the list of initial elements
        fVoiceInitialElementsList.push_back (
          multipleRest);

        // print resulting voice contents
        if (gTraceOptions->fTraceSegments || gTraceOptions->fTraceVoices) {
          gLogIOstream <<
            "The resulting voice contents of voice \"" <<
            fVoiceName << "\" is:" <<
            endl;
        }

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
          int fullMeasureLength =
            fVoiceLastSegment->
              getSegmentMeasuresList ().back ()->
                  getFullMeasureLength ();
                
          // finalize current measure in voice
          finalizeCurrentMeasureInVoice (
            inputLineNumber);
  
          if (gTraceOptions->fTraceRepeats || gTraceOptions->fTraceVoices) {
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
  
          // move current last segment to the list of initial elements
          if (gTraceOptions->fTraceRepeats) {
            gLogIOstream <<
              "Appending voice last segment to the initial elements in voice \"" <<
              getVoiceName () <<
              "\"" <<
              ", line " << inputLineNumber <<
              endl;
          }
                        
          fVoiceInitialElementsList.push_back (
            fVoiceLastSegment);
  
          // create a new last segment containing a new measure for the voice
          if (gTraceOptions->fTraceSegments || gTraceOptions->fTraceVoices) {
            gLogIOstream <<
              "Creating a new last segment containing a new measure for voice \"" <<
              fVoiceName << "\"" <<
              ", line " << inputLineNumber <<
              endl;
          }
  
          createNewLastSegmentAndANewMeasureAfterARepeat (
            inputLineNumber,
            fullMeasureLength);
  
          if (gTraceOptions->fTraceRepeats || gTraceOptions->fTraceVoices) {
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
        if (gTraceOptions->fTraceRepeats) {
          gLogIOstream <<
            "Appending repeat clone to voice clone \"" <<
            getVoiceName () <<  "\"" <<
            endl;
        }
      
        // create a repeat common part from current last segment
        if (gTraceOptions->fTraceRepeats) {
          gLogIOstream <<
            "Creating a repeat clone common part from current last segment in voice \"" <<
            getVoiceName () <<
            "\"" <<
            ", line " << inputLineNumber <<
            endl;
        }
      
        S_msrRepeatCommonPart
          repeatCommonPart =
            msrRepeatCommonPart::create (
              inputLineNumber,
              repeatCLone);

        // move voice last segment into the repeat common part
        if (gTraceOptions->fTraceRepeats) {
          gLogIOstream <<
            "Append the segment to use to the repeat common part in voice \"" <<
            getVoiceName () <<
            "\"" <<
            ", line " << inputLineNumber <<
            endl;
        }
      
        repeatCommonPart->
          appendElementToRepeatCommonPart (
            repeatCommonPart);

        // set repeatCLone's common part
        if (gTraceOptions->fTraceRepeats) {
          gLogIOstream <<
            "Setting repeat clone common part in voice \"" <<
            getVoiceName () <<
            "\"" <<
            endl;
        }

        repeatCLone->
          setRepeatCommonPart (
            repeatCommonPart);

/* JMI
        // set current last segment as the repeat common segment
        if (gTraceOptions->fTraceRepeats) {
          gLogIOstream <<
            "Segging current last segment as repeat common segment in voice \"" <<
            getVoiceName () <<
            "\"" <<
            endl;
      
        repeatCLone->
          setRepeatCommonSegment (
            fVoiceLastSegment);
 */         
        // register repeat clone as the (new) current repeat
        if (gTraceOptions->fTraceRepeats) {
          gLogIOstream <<
            "Registering repeat clone as the new current repeat in voice \"" <<
            getVoiceName () <<
            "\"" <<
            endl;
        }
      
        fVoiceCurrentRepeat =
          repeatCLone;
      
        // append it to the list of initial elements
        fVoiceInitialElementsList.push_back (
          repeatCLone);
      
        // create a new segment for the voice
        if (gTraceOptions->fTraceSegments || gTraceOptions->fTraceVoices) {
          gLogIOstream <<
            "Creating a new last segment for voice BOF JMI \"" <<
            fVoiceName << "\"" <<
            endl;
        }
            
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
        if (gTraceOptions->fTraceRepeats) {
          gLogIOstream <<
            "Appending a repeat ending to voice \"" <<
            getVoiceName () <<  "\"" <<
            ", line " << inputLineNumber <<
            endl;
        }
      
        // fetch last measure's full measure length
        int fullMeasureLength =
          fVoiceLastSegment->
            getSegmentMeasuresList ().back ()->
                getFullMeasureLength ();

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
            
        fVoiceCurrentRepeat->
          addRepeatEnding (repeatEnding);
      
        // create a new last segment containing a new measure for the voice
        if (
          gTraceOptions->fTraceRepeats
            ||
          gTraceOptions->fTraceVoices
            ||
          gTraceOptions->fTraceSegments) {
          gLogIOstream <<
            "Creating a new last segment containing a new measure for voice \"" <<
            fVoiceName << "\"" <<
            endl;
        }
            
        createNewLastSegmentAndANewMeasureAfterARepeat (
          inputLineNumber,
          fullMeasureLength);
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
        if (gTraceOptions->fTraceRepeats) {
          gLogIOstream <<
            "Appending a measures repeat replica to voice \"" <<
            getVoiceName () <<  "\"" <<
            ", line " << inputLineNumber <<
            endl;
        }
      
        // fetch last measure's full measure length
        int fullMeasureLength =
          fVoiceLastSegment->
            getSegmentMeasuresList ().back ()->
              getFullMeasureLength ();

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
        if (gTraceOptions->fTraceRepeats) {
          gLogIOstream <<
            "Setting the measures repeat replica to current measures repeat BBB in voice \"" <<
            fVoiceName <<
            "\"" <<
            endl;
        }
            
        fVoiceCurrentMeasuresRepeat->
          setMeasuresRepeatReplicas (
            measuresRepeatReplicas);

        // create a new last segment containing a new measure for the voice
        if (gTraceOptions->fTraceSegments || gTraceOptions->fTraceVoices) {
          gLogIOstream <<
            "Creating a new last segment containing a new measure for voice \"" <<
            fVoiceName << "\"" <<
            endl;
        }
            
        createNewLastSegmentAndANewMeasureAfterARepeat (
          inputLineNumber,
          fullMeasureLength);
      }
      break;
  } // switch
}

void msrVoice:: appendRepeatEndingCloneToVoice ( // JMI
  S_msrRepeatEnding repeatEndingClone)
{
  switch (fVoiceKind) {
    case msrVoice::kRegularVoice:
    case msrVoice::kHarmonyVoice:
    case msrVoice::kFiguredBassVoice:
      {
        // add the repeat ending it to the voice current repeat
        if (gTraceOptions->fTraceRepeats) {
          gLogIOstream <<
            "Appending repeat ending clone to current repeat in voice \"" <<
            fVoiceName <<
            "\"" <<
            endl;
        }
            
        // sanity check
        msrAssert(
          fVoiceCurrentRepeat != nullptr,
          "fVoiceCurrentRepeat is null");
          
        fVoiceCurrentRepeat->
          addRepeatEnding (repeatEndingClone);
      
        // create a new segment for the voice
        if (gTraceOptions->fTraceSegments || gTraceOptions->fTraceVoices) {
          gLogIOstream <<
            "Creating a new last segment for voice \"" <<
            fVoiceName <<
            "\"" <<
            endl;
        }
            
        createNewLastSegmentForVoice (
          repeatEndingClone->getInputLineNumber ());
      }
      break;
  } // switch
}

void msrVoice::prependBarlineToVoice (S_msrBarline barline)
{
  if (gTraceOptions->fTraceRepeats) {
    gLogIOstream <<
      "Prepending barline '" <<
      barline->asString () <<
      "' to voice \"" << getVoiceName () << "\"" <<
      ":" <<
      endl;
  }
      
  // create the voice last segment and first measure if needed
  appendAFirstMeasureToVoiceIfNotYetDone (
    barline->getInputLineNumber ());

  fVoiceLastSegment->
    prependBarlineToSegment (barline);
}

void msrVoice::appendBarlineToVoice (S_msrBarline barline)
{
  if (gTraceOptions->fTraceRepeats) {
    gLogIOstream <<
      "Appending barline '" <<
      barline->asString () <<
      "' to voice \"" << getVoiceName () << "\"" <<
      ":" <<
      endl;
  }
      
  // create the voice last segment and first measure if needed
 // appendAFirstMeasureToVoiceIfNotYetDone (
 //JMI   barline->getInputLineNumber ());

  fVoiceLastSegment->
    appendBarlineToSegment (barline);
}

void msrVoice::appendSegnoToVoice (S_msrSegno segno)
{
  if (gTraceOptions->fTraceRepeats) {
    gLogIOstream <<
      "Appending a segno to voice \"" << getVoiceName () << "\"" <<
      endl;
  }

  // create the voice last segment and first measure if needed
  appendAFirstMeasureToVoiceIfNotYetDone (
    segno->getInputLineNumber ());

  fVoiceLastSegment->
    appendSegnoToSegment (segno);
}

void msrVoice::appendCodaToVoice (S_msrCoda coda)
{
  if (gTraceOptions->fTraceRepeats) {
    gLogIOstream <<
      "Appending a coda to voice \"" << getVoiceName () << "\"" <<
      ":" <<
      endl;
  }

  // create the voice last segment and first measure if needed
  appendAFirstMeasureToVoiceIfNotYetDone (
    coda->getInputLineNumber ());

  fVoiceLastSegment->
    appendCodaToSegment (coda);
}

void msrVoice::appendEyeGlassesToVoice (S_msrEyeGlasses eyeGlasses)
{
  if (gTraceOptions->fTraceNotes) {
    gLogIOstream <<
      "Appending a eyeGlasses to voice \"" << getVoiceName () << "\"" <<
      endl;
  }

  // create the voice last segment and first measure if needed
  appendAFirstMeasureToVoiceIfNotYetDone (
    eyeGlasses->getInputLineNumber ());

  fVoiceLastSegment->
    appendEyeGlassesToSegment (eyeGlasses);
}

void msrVoice::appendPedalToVoice (S_msrPedal pedal)
{
  if (gTraceOptions->fTraceNotes) {
    gLogIOstream <<
      "Appending a pedal to voice \"" << getVoiceName () << "\"" <<
      endl;
  }

  // create the voice last segment and first measure if needed
  appendAFirstMeasureToVoiceIfNotYetDone (
    pedal->getInputLineNumber ());

  fVoiceLastSegment->
    appendPedalToSegment (pedal);
}

void msrVoice::appendDampToVoice (S_msrDamp damp)
{
  if (gTraceOptions->fTraceNotes) {
    gLogIOstream <<
      "Appending a damp to voice \"" << getVoiceName () << "\"" <<
      endl;
  }

  // create the voice last segment and first measure if needed
  appendAFirstMeasureToVoiceIfNotYetDone (
    damp->getInputLineNumber ());

  fVoiceLastSegment->
    appendDampToSegment (damp);
}

void msrVoice::appendDampAllToVoice (S_msrDampAll dampAll)
{
  if (gTraceOptions->fTraceNotes) {
    gLogIOstream <<
      "Appending a damp all to voice \"" << getVoiceName () << "\"" <<
      endl;
  }

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
  if (gTraceOptions->fTraceVoices) {
    gLogIOstream <<
      "Removing last note" <<
      " from voice " << getVoiceName () <<
      endl;
  }

  return
    fVoiceLastSegment->
      removeLastElementFromSegment (inputLineNumber);
}
*/

void msrVoice::removeNoteFromVoice (
  int       inputLineNumber,
  S_msrNote note)
{
  if (gTraceOptions->fTraceChords) {
    gLogIOstream <<
      "Removing first chord note '" <<
      note->asShortString () <<
      "' from voice \"" << getVoiceName () << "\"" <<
      endl;
  }

  fVoiceLastSegment->
    removeNoteFromSegment (
      inputLineNumber,
      note);
}

void msrVoice::removeElementFromVoice (
  int          inputLineNumber,
  S_msrElement element)
{
  if (gTraceOptions->fTraceChords || gTraceOptions->fTraceGraceNotes) {
    gLogIOstream <<
      "Removing element '" <<
      element->asShortString () <<
      "' from voice \"" << getVoiceName () << "\"" <<
      endl;
  }

  fVoiceLastSegment->
    removeElementFromSegment (
      inputLineNumber,
      element);
}

S_msrMeasure msrVoice::removeLastMeasureFromVoice (
  int inputLineNumber)
{
  if (gTraceOptions->fTraceMeasures || gTraceOptions->fTraceVoices) {
    gLogIOstream <<
      "Removing last measure from voice \"" <<
      getVoiceName () <<
      "\"" <<
      endl;
  }

  // remove (new) last measure and return it
  return
    fVoiceLastSegment->
      removeLastMeasureFromSegment (
        inputLineNumber);
}

void msrVoice::finalizeCurrentMeasureInVoice (
  int inputLineNumber)
{
  if (gTraceOptions->fTraceMeasures || gTraceOptions->fTraceVoices) {
    gLogIOstream <<
      "Finalizing current measure in voice \"" <<
      getVoiceName () <<
      "\"" <<
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
}

void msrVoice::finalizeVoice ( // JMI ???
  int inputLineNumber)
{
  if (gTraceOptions->fTraceVoices) {
    gLogIOstream <<
      "Finalizing voice \"" <<
      getVoiceName () <<
      "\", line " << inputLineNumber <<
      endl;
  }
}

void msrVoice::acceptIn (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrVoice::acceptIn()" <<
      endl;
  }
      
  if (visitor<S_msrVoice>*
    p =
      dynamic_cast<visitor<S_msrVoice>*> (v)) {
        S_msrVoice elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrVoice::visitStart()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrVoice::acceptOut (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrVoice::acceptOut()" <<
      endl;
  }

  if (visitor<S_msrVoice>*
    p =
      dynamic_cast<visitor<S_msrVoice>*> (v)) {
        S_msrVoice elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrVoice::visitEnd()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrVoice::browseData (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrVoice::browseData()" <<
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
      "% <== msrVoice::browseData()" <<
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
    setw (fieldWidth) << "Staff uplink" << " : " <<
    fVoiceStaffUplink->getStaffName () <<
    endl <<
    setw (fieldWidth) << "VoiceNumber" << " : " <<
    voiceNumberAsString () <<
    endl <<
    setw (fieldWidth) << "RegularVoiceStaffSequentialNumber" << " : " <<
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
    setw (fieldWidth) << "MusicHasBeenInsertedInVoice" << " : " <<
    booleanAsString (fMusicHasBeenInsertedInVoice) <<
    endl <<
    setw (fieldWidth) << "VoiceContainsMultipleRests" << " : " <<
    booleanAsString (fVoiceContainsMultipleRests) <<
    endl <<
    setw (fieldWidth) << "fVoiceFirstSegment" << " : ";
    
  // print the voice first segment if any
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

  os <<
    endl;
  
  // print the voice initial elements
  int initialElementsNumber =
    fVoiceInitialElementsList.size ();
    
  os <<
    "Initial elements: ";
  if (initialElementsNumber)
    os << "(" << initialElementsNumber << ")";
  else
    os << "none";
  os <<
    endl;
    
  if (initialElementsNumber) {
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
S_msrPartNameDisplay msrPartNameDisplay::create (
  int    inputLineNumber,
  string partNameDisplayValue)
{
  msrPartNameDisplay* o =
    new msrPartNameDisplay (
      inputLineNumber, partNameDisplayValue);
  assert(o!=0);
  return o;
}

msrPartNameDisplay::msrPartNameDisplay (
  int    inputLineNumber,
  string partNameDisplayValue)
    : msrElement (inputLineNumber)
{
  fPartNameDisplayValue = partNameDisplayValue;
}

msrPartNameDisplay::~msrPartNameDisplay ()
{}

void msrPartNameDisplay::acceptIn (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrPartNameDisplay::acceptIn()" <<
      endl;
  }
      
  if (visitor<S_msrPartNameDisplay>*
    p =
      dynamic_cast<visitor<S_msrPartNameDisplay>*> (v)) {
        S_msrPartNameDisplay elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrPartNameDisplay::visitStart()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrPartNameDisplay::acceptOut (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrPartNameDisplay::acceptOut()" <<
      endl;
  }

  if (visitor<S_msrPartNameDisplay>*
    p =
      dynamic_cast<visitor<S_msrPartNameDisplay>*> (v)) {
        S_msrPartNameDisplay elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrPartNameDisplay::visitEnd()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrPartNameDisplay::browseData (basevisitor* v)
{}

string msrPartNameDisplay::asString () const
{
  stringstream s;

  s <<
    "PartNameDisplay" <<
    ", line " << fInputLineNumber <<
    ", partNameDisplayValue: " << fPartNameDisplayValue;
    
  return s.str ();
}

ostream& operator<< (ostream& os, const S_msrPartNameDisplay& elt)
{
  elt->print (os);
  return os;
}

void msrPartNameDisplay::print (ostream& os)
{
  os <<
    asString () <<
    endl;
}

//______________________________________________________________________________
S_msrPartAbbreviationDisplay msrPartAbbreviationDisplay::create (
  int    inputLineNumber,
  string partAbbreviationDisplayValue)
{
  msrPartAbbreviationDisplay* o =
    new msrPartAbbreviationDisplay (
      inputLineNumber, partAbbreviationDisplayValue);
  assert(o!=0);
  return o;
}

msrPartAbbreviationDisplay::msrPartAbbreviationDisplay (
  int    inputLineNumber,
  string partAbbreviationDisplayValue)
    : msrElement (inputLineNumber)
{
  fPartAbbreviationDisplayValue = partAbbreviationDisplayValue;
}

msrPartAbbreviationDisplay::~msrPartAbbreviationDisplay ()
{}

void msrPartAbbreviationDisplay::acceptIn (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrPartAbbreviationDisplay::acceptIn()" <<
      endl;
  }
      
  if (visitor<S_msrPartAbbreviationDisplay>*
    p =
      dynamic_cast<visitor<S_msrPartAbbreviationDisplay>*> (v)) {
        S_msrPartAbbreviationDisplay elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrPartAbbreviationDisplay::visitStart()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrPartAbbreviationDisplay::acceptOut (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrPartAbbreviationDisplay::acceptOut()" <<
      endl;
  }

  if (visitor<S_msrPartAbbreviationDisplay>*
    p =
      dynamic_cast<visitor<S_msrPartAbbreviationDisplay>*> (v)) {
        S_msrPartAbbreviationDisplay elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrPartAbbreviationDisplay::visitEnd()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrPartAbbreviationDisplay::browseData (basevisitor* v)
{}

string msrPartAbbreviationDisplay::asString () const
{
  stringstream s;

  s <<
    "PartAbbreviationDisplay" <<
    ", line " << fInputLineNumber <<
    ", " <<
    "partAbbreviationDisplayValue: " << fPartAbbreviationDisplayValue;
    
  return s.str ();
}

ostream& operator<< (ostream& os, const S_msrPartAbbreviationDisplay& elt)
{
  elt->print (os);
  return os;
}

void msrPartAbbreviationDisplay::print (ostream& os)
{
  os <<
    asString () <<
    endl;
}

//______________________________________________________________________________
S_msrStaffTuning msrStaffTuning::create (
  int                     inputLineNumber,
  int                     staffTuningLineNumber,
  msrQuarterTonesPitchKind quarterTonesPitchKind,
  int                     staffTuningOctave)
{
  msrStaffTuning* o =
    new msrStaffTuning (
      inputLineNumber,
      staffTuningLineNumber,
      quarterTonesPitchKind,
      staffTuningOctave);
  assert(o!=0);
  return o;
}

msrStaffTuning::msrStaffTuning (
  int                      inputLineNumber,
  int                      staffTuningLineNumber,
  msrQuarterTonesPitchKind quarterTonesPitchKind,
  int                      staffTuningOctave)
    : msrElement (inputLineNumber)
{
  fStaffTuningLineNumber            = staffTuningLineNumber;
  fStaffTuningQuarterTonesPitchKind = quarterTonesPitchKind;
  fStaffTuningOctave                = staffTuningOctave;
}

msrStaffTuning::~ msrStaffTuning ()
{}

S_msrStaffTuning msrStaffTuning::createStaffTuningNewbornClone ()
{
  if (gTraceOptions->fTraceStaffTuning) {
    gLogIOstream <<
      "Creating a newborn clone of staff tuning '" <<
      asString () <<
      "'" <<
      endl;
  }

 S_msrStaffTuning
    newbornClone =
      msrStaffTuning::create (
        fInputLineNumber,
        fStaffTuningLineNumber,
        fStaffTuningQuarterTonesPitchKind,
        fStaffTuningOctave);
  
  return newbornClone;
}

void msrStaffTuning::acceptIn (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrStaffTuning::acceptIn()" <<
      endl;
  }
      
  if (visitor<S_msrStaffTuning>*
    p =
      dynamic_cast<visitor<S_msrStaffTuning>*> (v)) {
        S_msrStaffTuning elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrStaffTuning::visitStart()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrStaffTuning::acceptOut (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrStaffTuning::acceptOut()" <<
      endl;
  }

  if (visitor<S_msrStaffTuning>*
    p =
      dynamic_cast<visitor<S_msrStaffTuning>*> (v)) {
        S_msrStaffTuning elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrStaffTuning::visitEnd()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrStaffTuning::browseData (basevisitor* v)
{}

ostream& operator<< (ostream& os, const S_msrStaffTuning& elt)
{
  elt->print (os);
  return os;
}

string msrStaffTuning::asString () const
{
  stringstream s;

  s <<
    "StaffTuning" <<
    ", line " << fStaffTuningLineNumber <<
    ", " <<
    msrQuarterTonesPitchKindAsString (
      gMsrOptions->fMsrQuarterTonesPitchesLanguageKind,
      fStaffTuningQuarterTonesPitchKind) <<
    ", octave " << fStaffTuningOctave;
    
  return s.str ();
}

void msrStaffTuning::print (ostream& os)
{
  os <<
    "StaffTuning" <<
    ", line " << fInputLineNumber <<
    ", " <<
    endl;

  gIndenter++;

  const int fieldWidth = 29;

  os << left <<  
    setw (fieldWidth) <<
    "StaffTuningLineNumber" << " = " <<
    fStaffTuningLineNumber <<
    endl <<
    setw (fieldWidth) <<
    "fStaffTuningQuarterTonesPitch" << " = " <<
    msrQuarterTonesPitchKindAsString (
      gMsrOptions->fMsrQuarterTonesPitchesLanguageKind,
      fStaffTuningQuarterTonesPitchKind) <<
    endl <<
    setw (fieldWidth) <<
    "StaffTuningOctave" << " = " <<
    fStaffTuningOctave <<
    endl;

  gIndenter--;
}

//______________________________________________________________________________
S_msrStaffDetails msrStaffDetails::create (
  int                 inputLineNumber,
  msrStaffTypeKind    staffTypeKind,
  msrShowFretsKind    showFretsKind,
  msrPrintObjectKind  printObjectKind,
  msrPrintSpacingKind printSpacingKind)
{
  msrStaffDetails* o =
    new msrStaffDetails (
      inputLineNumber,
      staffTypeKind,
      showFretsKind,
      printObjectKind,
      printSpacingKind);
  assert(o!=0);
  return o;
}

msrStaffDetails::msrStaffDetails (
  int                 inputLineNumber,
  msrStaffTypeKind    staffTypeKind,
  msrShowFretsKind    showFretsKind,
  msrPrintObjectKind  printObjectKind,
  msrPrintSpacingKind printSpacingKind)
    : msrElement (inputLineNumber)
{
  fStaffTypeKind    = staffTypeKind;
  
  fStaffLinesNumber = 5; // default value JMI ???

  fShowFretsKind    = showFretsKind;
  
  fPrintObjectKind  = printObjectKind;
  fPrintSpacingKind = printSpacingKind;
}

msrStaffDetails::~msrStaffDetails ()
{}

void msrStaffDetails::acceptIn (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrStaffDetails::acceptIn()" <<
      endl;
  }
      
  if (visitor<S_msrStaffDetails>*
    p =
      dynamic_cast<visitor<S_msrStaffDetails>*> (v)) {
        S_msrStaffDetails elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrStaffDetails::visitStart()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrStaffDetails::acceptOut (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrStaffDetails::acceptOut()" <<
      endl;
  }

  if (visitor<S_msrStaffDetails>*
    p =
      dynamic_cast<visitor<S_msrStaffDetails>*> (v)) {
        S_msrStaffDetails elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrStaffDetails::visitEnd()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrStaffDetails::browseData (basevisitor* v)
{
  if (fStaffTuningsList.size ()) {
    for (
      list<S_msrStaffTuning>::const_iterator i = fStaffTuningsList.begin ();
      i != fStaffTuningsList.end ();
      i++) {
        // browse the staff tuning
        msrBrowser<msrStaffTuning> browser (v);
        browser.browse (*(*i));
    } // for
  }
}

string msrStaffDetails::staffTypeKindAsString (
  msrStaffTypeKind staffTypeKind)
{
  string result;
  
  switch (staffTypeKind) {
    case msrStaffDetails::kRegularStaffType:
      result = "regular";
      break;
    case msrStaffDetails::kOssiaStaffType:
      result = "ossia";
      break;
    case msrStaffDetails::kCueStaffType:
      result = "cue";
      break;
    case msrStaffDetails::kEditorialStaffType:
      result = "editorial";
      break;
    case msrStaffDetails::kAlternateStaffType:
      result = "alternate";
      break;
  } // switch

  return result;
}

string msrStaffDetails::showFretsKindAsString (
  msrShowFretsKind showFretsKind)
{
  string result;
  
  switch (showFretsKind) {
    case msrStaffDetails::kShowFretsNumbers:
      result = "numbers";
      break;
    case msrStaffDetails::kShowFretsLetters:
      result = "letters";
      break;
  } // switch

  return result;
}
      
string msrStaffDetails::printObjectKindAsString (
  msrPrintObjectKind printObjectKind)
{
  string result;
  
  switch (printObjectKind) {
    case msrStaffDetails::kPrintObjectYes:
      result = "print object yes";
      break;
    case msrStaffDetails::kPrintObjectNo:
      result = "print object no";
      break;
  } // switch

  return result;
}
      
string msrStaffDetails::printSpacingKindAsString (
  msrPrintSpacingKind printSpacingKind)
{
  string result;
  
  switch (printSpacingKind) {
    case msrStaffDetails::kPrintSpacingYes:
      result = "print spacing yes";
      break;
    case msrStaffDetails::kPrintSpacingNo:
      result = "print spacing no";
      break;
  } // switch

  return result;
}
      
ostream& operator<< (ostream& os, const S_msrStaffDetails& elt)
{
  elt->print (os);
  return os;
}

string msrStaffDetails::asShortString ()
{
  stringstream s;

  s <<
    "StaffDetails" <<
    ", staffTypeKind" <<
    staffTypeKindAsString (fStaffTypeKind) <<
    ", line " << fInputLineNumber <<
    endl;

  // print the staff lines number
  s << ", staffLinesNumber: " << fStaffLinesNumber;

  // print the staff tunings if any
  s <<
    ", StaffTunings: " << fStaffTuningsList.size ();

  s <<
    ", showFretsKind = " <<
    showFretsKindAsString (fShowFretsKind) <<
    ", printObjectKind = " <<
    printObjectKindAsString (fPrintObjectKind) <<
    ", printSpacingKind = " <<
    printSpacingKindAsString (fPrintSpacingKind);

  return s.str ();
}

void msrStaffDetails::print (ostream& os)
{
  os <<
    "StaffDetails" <<
    ", line " << fInputLineNumber <<
    endl;

  gIndenter++;

  const int fieldWidth = 17;
  
  os << left <<
    setw (fieldWidth) <<
    "staffTypeKind" << " = " <<
    staffTypeKindAsString (fStaffTypeKind) <<
    endl <<
    setw (fieldWidth) <<
    "staffLinesNumber" << " = " << fStaffLinesNumber <<
    endl;

  // print the staff tunings if any
  os << "StaffTunings: ";
  if (fStaffTuningsList.size ()) {
    os <<
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
    os <<
      endl;
    
    gIndenter--;
  }
  else {
    os <<
      " : " << "none" <<
      endl;
  }

  os << left <<
    setw (fieldWidth) <<
    "showFretsKind" << " = " <<
    showFretsKindAsString (fShowFretsKind) <<
    endl <<
    setw (fieldWidth) <<
    "printObjectKind" << " = " <<
    printObjectKindAsString (fPrintObjectKind) <<
    endl <<
    setw (fieldWidth) <<
    "printSpacingKind" << " = " <<
    printSpacingKindAsString (fPrintSpacingKind) <<
    endl;

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

  if (gTraceOptions->fTraceStaves) {
    gLogIOstream <<
      "Initializing staff \"" << fStaffName <<
      "\" in part " <<
      fStaffPartUplink->getPartCombinedName () <<
      endl;
  }

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
      if (gTraceOptions->fTraceClefs || gTraceOptions->fTraceStaves) {
        gLogIOstream <<
          "Appending part clef '" << clef->asString () <<
          "' as initial clef to staff \"" <<
          getStaffName () <<
          "\" in part " <<
          fStaffPartUplink->getPartCombinedName () <<
          endl;
      }

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
      if (gTraceOptions->fTraceStaves || gTraceOptions->fTraceKeys) {
        gLogIOstream <<
          "Appending part key '" << key->asString () <<
          "' as initial key to staff \"" <<
          getStaffName () <<
          "\" in part " <<
          fStaffPartUplink->getPartCombinedName () <<
          endl;
      }

      appendKeyToStaff (key);
    }
      /* JMI
    else {
      if (gTraceOptions->fTraceStaves) {
        gLogIOstream <<
          "Appending default C major key " <<
          " to staff \"" <<
          getStaffName () <<
          "\" in part " <<
          fStaffPartUplink->getPartCombinedName () <<
          endl;
      }
          
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
      if (gTraceOptions->fTraceStaves || gTraceOptions->fTraceTimes) {
        gLogIOstream <<
          "Appending part time '" << time->asString () <<
          "' as initial time to staff \"" <<
          getStaffName () <<
          "\" in part " <<
          fStaffPartUplink->getPartCombinedName () <<
          endl;
      }

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
      if (gTraceOptions->fTraceStaves /* JMI || gTraceOptions->fTraceTransposes */) {
        gLogIOstream <<
          "Appending part transpose '" << transpose->asString () <<
          "' as initial transpose to staff \"" <<
          getStaffName () <<
          "\" in part " <<
          fStaffPartUplink->getPartCombinedName () <<
          endl;
      }
      
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
}

msrStaff::~msrStaff()
{}

S_msrStaff msrStaff::createStaffNewbornClone (
  S_msrPart containingPart)
{
  if (gTraceOptions->fTraceStaves) {
    gLogIOstream <<
      "Creating a newborn clone of staff \"" <<
      fStaffName <<
      "\"" <<
      endl;
  }
  
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
  if (gTraceOptions->fTraceMeasures || gTraceOptions->fTraceStaves) {
    gLogIOstream <<
      "Creating and appending measure '" <<
      measureNumber <<
      "', line " << inputLineNumber <<
      ", in staff \"" << getStaffName () << "\"" <<
      endl;
  }

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
      if (gTraceOptions->fTraceMeasures || gTraceOptions->fTraceStaves) {
        gLogIOstream <<
          "Propagating the creation and appending of measure '" <<
          measureNumber <<
          "', line " << inputLineNumber <<
          ", to voice \"" << voice->getVoiceName () << "\"" <<
          endl;
      }
  
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
    if (gTraceOptions->fTraceMeasures || gTraceOptions->fTraceStaves) {
      gLogIOstream <<
        "Propagating the creation and appending of measure '" <<
        measureNumber <<
        "', line " << inputLineNumber <<
        ", to voice \"" << voice->getVoiceName () << "\"" <<
        endl;
    }

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
  if (gTraceOptions->fTraceMeasures || gTraceOptions->fTraceStaves) {
    gLogIOstream <<
      "Setting next measure number to '" <<
      nextMeasureNumber <<
      ", in staff \"" << getStaffName () << "\"" <<
      "', line " << inputLineNumber <<
      endl;
  }

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
    
    if (gTraceOptions->fTraceMeasures || gTraceOptions->fTraceStaves) {
      gLogIOstream <<
        "Propagating the setting of next measure number '" <<
        nextMeasureNumber <<
        "', line " << inputLineNumber <<
        ", in voice \"" << voice->getVoiceName () << "\"" <<
        endl;
    }

    voice->
      setNextMeasureNumberInVoice (
        inputLineNumber,
        nextMeasureNumber);
  } // for
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
      break;
      
    case msrVoice::kHarmonyVoice:
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
      break;
      
    case msrVoice::kFiguredBassVoice:
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
      if (gTraceOptions->fTraceVoices) {
        gLogIOstream <<
          "Voice " << voiceNumber <<
          " in staff " << getStaffName () <<
          " gets staff regular voice number " <<
          fStaffRegularVoicesCounter <<
          endl;
      }
        
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
  if (gTraceOptions->fTraceVoices || gTraceOptions->fTraceStaves) {
    gLogIOstream <<
      "Registering voice number '" << voiceNumber <<
      "', named \"" << voice->getVoiceName () <<
      "\" in staff " << getStaffName () <<
      endl;
  }

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
      if (gTraceOptions->fTraceStaves || gTraceOptions->fTraceVoices) {
        gLogIOstream <<
          "Sorting the voices in staff \"" <<
          getStaffName () << "\"" <<
          ", line " << inputLineNumber <<
          endl;
      }

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
  if (gTraceOptions->fTraceVoices || gTraceOptions->fTraceStaves) {
    gLogIOstream <<
      "Registering regular voice number '" << voiceNumber <<
      "', named \"" << voice->getVoiceName () <<
      "\" in staff " << getStaffName () <<
      "'s regular voices map" <<
      endl;
  }

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
  if (gTraceOptions->fTraceVoices || gTraceOptions->fTraceStaves) {
    gLogIOstream <<
      "Registering voice number '" << voiceNumber <<
      "', named \"" << voice->getVoiceName () <<
      "\" in staff " << getStaffName () <<
      "'s all voices list" <<
      endl;
  }

  fStaffAllVoicesList.push_back (voice);
}

S_msrVoice msrStaff::fetchVoiceFromStaffByItsNumber (
  int inputLineNumber,
  int voiceNumber)
{
  S_msrVoice result; // JMI avoid repetivite messages!

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

  for (
    map<int, S_msrVoice>::const_iterator i =
      fStaffRegularVoicesMap.begin ();
    i != fStaffRegularVoicesMap.end ();
    i++) {
    if (
      (*i).second->getVoiceNumber ()
        ==
      voiceNumber) {
        if (gTraceOptions->fTraceVoices || gTraceOptions->fTraceStaves) {
          gLogIOstream <<
            "Voice " << voiceNumber <<
            " in staff \"" << getStaffName () << "\"" <<
            " has staff relative number " << (*i).first <<
            endl;
        }
        
        result = (*i).second;
        break;
    }
  } // for

  return result;
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
        if (gTraceOptions->fTraceVoices) {
          gLogIOstream <<
            "Registering regular voice '" << voiceNumber <<
            "' " << voice->getVoiceName () <<
            " with staff regular voice number " <<
            fStaffRegularVoicesCounter <<
            " in staff " << getStaffName () <<
            endl;
        }

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
  if (gTraceOptions->fTraceStaves || gTraceOptions->fTraceMeasures) {
    gLogIOstream <<
      "Padding up to measure length '" << measureLength <<
      "' in staff \"" <<
      getStaffName () <<
      "\", line " << inputLineNumber <<
      endl;
  }

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

void msrStaff::appendCommentToStaff (S_msrComment comment)
{
  if (gTraceOptions->fTraceClefs || gTraceOptions->fTraceStaves) {
    gLogIOstream <<
      "Appending comment '" << comment->asString () <<
      "' to staff \"" <<
      getStaffName () <<
      "\" in part " <<
      fStaffPartUplink->getPartCombinedName () <<
      endl;
  }
  
  // propagate clef to all voices
  for (
    map<int, S_msrVoice>::const_iterator i = fStaffAllVoicesMap.begin ();
    i != fStaffAllVoicesMap.end ();
    i++) {
    (*i).second-> // JMI msrAssert???
      appendCommentToVoice (comment);
  } // for
}

void msrStaff::appendClefToStaff (S_msrClef clef)
{
  if (gTraceOptions->fTraceClefs || gTraceOptions->fTraceStaves) {
    gLogIOstream <<
      "Appending clef '" << clef->asString () <<
      "' to staff \"" <<
      getStaffName () <<
      "\" in part " <<
      fStaffPartUplink->getPartCombinedName () <<
      endl;
  }

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
  }
}

void msrStaff::appendKeyToStaff (S_msrKey  key)
{
  if (gTraceOptions->fTraceKeys || gTraceOptions->fTraceStaves) {
    gLogIOstream <<
      "Appending key '" << key->asString () <<
      "' to staff \"" <<
      getStaffName () <<
      "\" in part " <<
      fStaffPartUplink->getPartCombinedName () <<
      endl;
  }
  
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
}

void msrStaff::appendTimeToStaff (S_msrTime time)
{
  if (gTraceOptions->fTraceTimes || gTraceOptions->fTraceStaves) {
    gLogIOstream <<
      "Appending time '" << time->asString () <<
      "' to staff \"" <<
      getStaffName () <<
      "\" in part " <<
      fStaffPartUplink->getPartCombinedName () <<
      endl;
  }
  
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
}    

void msrStaff::appendTimeToStaffClone (S_msrTime time)
{
  if (gTraceOptions->fTraceTimes || gTraceOptions->fTraceStaves) {
    gLogIOstream <<
      "Appending time '" << time->asString () <<
      "' to staff clone \"" <<
      getStaffName () <<
      "\" in part " <<
      fStaffPartUplink->getPartCombinedName () <<
      endl;
  }
  
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
}    

void msrStaff::prepareForRepeatInStaff (
  int inputLineNumber)
{
  if (gTraceOptions->fTraceRepeats) {
    gLogIOstream <<
      "Preparing for repeat in staff \"" <<
      getStaffName () <<
      "\" in part " <<
      fStaffPartUplink->getPartCombinedName () <<
      ", line " << inputLineNumber <<
      endl;
  }

  for (
    map<int, S_msrVoice>::const_iterator i = fStaffAllVoicesMap.begin ();
    i != fStaffAllVoicesMap.end ();
    i++) {
    (*i).second->
      prepareForRepeatInVoice (
        inputLineNumber);
  } // for
}

void msrStaff::nestContentsIntoNewRepeatInStaff (
  int inputLineNumber)
{
  if (gTraceOptions->fTraceRepeats) {
    gLogIOstream <<
      "Nesting contents into new repeat in staff \"" <<
      getStaffName () <<
      "\" in part " <<
      fStaffPartUplink->getPartCombinedName () <<
      endl;
  }

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
  int inputLineNumber,
  int repeatTimes)
{
  if (gTraceOptions->fTraceRepeats) {
    gLogIOstream <<
      "Creating and appending a repeat to staff \"" <<
      getStaffName () <<
      "\" in part " <<
      fStaffPartUplink->getPartCombinedName () <<
      endl;
  }

  for (
    map<int, S_msrVoice>::const_iterator i = fStaffAllVoicesMap.begin ();
    i != fStaffAllVoicesMap.end ();
    i++) {
    (*i).second->
      createRepeatUponItsEndAndAppendItToVoice (
        inputLineNumber,
        repeatTimes);
  } // for
}

void msrStaff::createRegularRepeatUponItsFirstEndingInStaff (
  int inputLineNumber,
  int repeatTimes)
{
  if (gTraceOptions->fTraceRepeats) {
    gLogIOstream <<
      "Creating and appending a repeat to staff \"" <<
      getStaffName () <<
      "\" in part " <<
      fStaffPartUplink->getPartCombinedName () <<
      endl;
  }

  for (
    map<int, S_msrVoice>::const_iterator i = fStaffAllVoicesMap.begin ();
    i != fStaffAllVoicesMap.end ();
    i++) {
    (*i).second->
      createRegularRepeatUponItsFirstEndingInVoice (
        inputLineNumber,
        repeatTimes);
  } // for
}

void msrStaff::createEnclosingRepeatUponItsFirstEndingInStaff (
  int inputLineNumber,
  int repeatTimes)
{
  if (gTraceOptions->fTraceRepeats) {
    gLogIOstream <<
      "Creating and appending a repeat to staff \"" <<
      getStaffName () <<
      "\" in part " <<
      fStaffPartUplink->getPartCombinedName () <<
      endl;
  }

  for (
    map<int, S_msrVoice>::const_iterator i = fStaffAllVoicesMap.begin ();
    i != fStaffAllVoicesMap.end ();
    i++) {
    (*i).second->
      createEnclosingRepeatUponItsFirstEndingInVoice (
        inputLineNumber,
        repeatTimes);
  } // for
}

void msrStaff::appendRepeatEndingToStaff (
  int       inputLineNumber,
  string    repeatEndingNumber, // may be "1, 2"
  msrRepeatEnding::msrRepeatEndingKind
            repeatEndingKind)
{
  if (gTraceOptions->fTraceRepeats) {
    gLogIOstream <<
      "Appending repeat ending to staff \"" <<
      getStaffName () <<
      "\" in part " <<
      fStaffPartUplink->getPartCombinedName () <<
      endl;
  }

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
}

void msrStaff::createMeasuresRepeatFromItsFirstMeasuresInStaff (
  int inputLineNumber,
  int measuresRepeatMeasuresNumber,
  int measuresRepeatSlashes)
{
  if (gTraceOptions->fTraceRepeats) {
    gLogIOstream <<
      "Creating a measure repeat from it's first measure in staff \"" <<
      getStaffName () <<
      "\" in part " <<
      fStaffPartUplink->getPartCombinedName () <<
      endl;
  }

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
  if (gTraceOptions->fTraceRepeats) {
    gLogIOstream <<
      "Appending the pending measure repeat to staff \"" <<
      getStaffName () <<
      "\" in part " <<
      fStaffPartUplink->getPartCombinedName () <<
      endl;
  }

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
  if (gTraceOptions->fTraceRepeats) {
    gLogIOstream <<
      "Appending the pending multiple rest to staff \"" <<
      getStaffName () <<
      "\" in part " <<
      fStaffPartUplink->getPartCombinedName () <<
      endl;
  }

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
  if (gTraceOptions->fTraceRepeats) {
    gLogIOstream <<
      "Appending multiple rest '" <<
      multipleRest->asString () <<
      "' to staff clone \"" <<
      getStaffName () <<
      "\"" <<
      endl;
  }

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
  if (gTraceOptions->fTraceRepeats) {
    gLogIOstream <<
      "Appending repeat clone to staff \"" <<
      getStaffName () <<
      "\" in part " <<
      fStaffPartUplink->getPartCombinedName () <<
      endl;
  }

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
  if (gTraceOptions->fTraceRepeats) {
    gLogIOstream <<
      "Appending repeat ending clone to staff \"" <<
      getStaffName () <<
      "\" in part " <<
      fStaffPartUplink->getPartCombinedName () <<
      endl;
  }

  for (
    map<int, S_msrVoice>::const_iterator i = fStaffAllVoicesMap.begin ();
    i != fStaffAllVoicesMap.end ();
    i++) {
    (*i).second-> // JMI msrAssert???
      appendRepeatEndingCloneToVoice (repeatEndingClone);
  } // for
}

void msrStaff::appendBarlineToStaff (S_msrBarline barline)
{
  if (gTraceOptions->fTraceMeasures) {
    gLogIOstream <<
      "Appending barline '" << barline->asString () <<
      "' to staff " <<
      getStaffName () <<
      "\" in part " <<
      fStaffPartUplink->getPartCombinedName () <<
      endl;
  }

  for (
    map<int, S_msrVoice>::const_iterator i = fStaffAllVoicesMap.begin ();
    i != fStaffAllVoicesMap.end ();
    i++) {
    (*i).second->
      appendBarlineToVoice (barline);
  } // for
}

void msrStaff::appendTransposeToStaff (
  S_msrTranspose transpose)
{
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

  // set staff transpose
  bool doAppendTransposeToStaff = true;
  
  if (! fStaffCurrentTranspose) {
    doAppendTransposeToStaff = true;
  }
  
  else {
    if (transpose->isEqualTo (fStaffCurrentTranspose)) {
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

  // set staff transpose
  bool doAppendPartNameDisplayToStaff = true;

  /* JMI ???
  if (! fStaffCurrentTranspose) {
    doAppendPartNameDisplayToStaff = true;
  }
  
  else {
    if (partNameDisplay->isEqualTo (fStaffCurrentTranspose)) {
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

  // set staff transpose
  bool doAppendPartAbbreviationDisplayToStaff = true;
  
/* JMI ???
  if (! fStaffCurrentTranspose) {
    doAppendPartAbbreviationDisplayToStaff = true;
  }
  
  else {
    if (partAbbreviationDisplay->isEqualTo (fStaffCurrentTranspose)) {
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
      
  if (gTraceOptions->fTraceMeasures || gTraceOptions->fTraceStaves) {
    gLogIOstream <<
      "Finalizing current measure in staff \"" <<
      getStaffName () <<
      "\", line " << inputLineNumber <<
      ", partMeasureLengthHighTide = " <<
      partMeasureLengthHighTide <<
      endl;
  }

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
  if (gTraceOptions->fTraceStaves) {
    gLogIOstream <<
      "Finalizing staff \"" <<
      getStaffName () << "\"" <<
      ", line " << inputLineNumber <<
      endl;
  }

  // finalize the voices
  if (gTraceOptions->fTraceStaves || gTraceOptions->fTraceVoices) {
    gLogIOstream <<
      "Finalizing the voices in staff \"" <<
      getStaffName () << "\"" <<
      ", line " << inputLineNumber <<
      endl;
  }

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
}

void msrStaff::acceptIn (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrStaff::acceptIn()" <<
      endl;
  }
      
  if (visitor<S_msrStaff>*
    p =
      dynamic_cast<visitor<S_msrStaff>*> (v)) {
        S_msrStaff elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrStaff::visitStart()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrStaff::acceptOut (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrStaff::acceptOut()" <<
      endl;
  }

  if (visitor<S_msrStaff>*
    p =
      dynamic_cast<visitor<S_msrStaff>*> (v)) {
        S_msrStaff elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrStaff::visitEnd()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrStaff::browseData (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrStaff::browseData()" <<
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
      "% <== msrStaff::browseData()" <<
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
    "Part uplink" << " : " <<
    fStaffPartUplink->getPartCombinedName () <<
    endl <<
    setw (fieldwidth) <<
    "StaffInstrumentName" << " : \"" <<
    fStaffInstrumentName <<
    "\"" <<
    endl <<
    setw (fieldwidth) <<
    "StaffInstrumentAbbreviation" << " : \"" <<
    fStaffInstrumentAbbreviation <<
    "\"" <<
    endl;

  // print the staff details if any
  os <<
    "Staff details: ";
  if (fStaffStaffDetails) {
    os << fStaffStaffDetails;
  }
  else
    os << "none";
  os << endl;

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
  S_msrStaff newStaff)
{
  msrVoiceStaffChange* o =
    new msrVoiceStaffChange (
      inputLineNumber, newStaff);
  assert(o!=0);
  return o;
}

msrVoiceStaffChange::msrVoiceStaffChange (
  int        inputLineNumber,
  S_msrStaff newStaff)
    : msrElement (inputLineNumber)
{
  fNewStaff = newStaff;
}

msrVoiceStaffChange::~msrVoiceStaffChange()
{}

S_msrVoiceStaffChange msrVoiceStaffChange::createStaffChangeNewbornClone ()
{
 if (gTraceOptions->fTraceStaffTuning) {
    gLogIOstream <<
      "Creating a newborn clone of staff change '" <<
      asString () <<
      "'" <<
      endl;
  }

 S_msrVoiceStaffChange
    newbornClone =
      msrVoiceStaffChange::create (
        fInputLineNumber,
        fNewStaff);
  
  return newbornClone;
}

void msrVoiceStaffChange::acceptIn (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrVoiceStaffChange::acceptIn()" <<
      endl;
  }
      
  if (visitor<S_msrVoiceStaffChange>*
    p =
      dynamic_cast<visitor<S_msrVoiceStaffChange>*> (v)) {
        S_msrVoiceStaffChange elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrVoiceStaffChange::visitStart()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrVoiceStaffChange::acceptOut (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrVoiceStaffChange::acceptOut()" <<
      endl;
  }

  if (visitor<S_msrVoiceStaffChange>*
    p =
      dynamic_cast<visitor<S_msrVoiceStaffChange>*> (v)) {
        S_msrVoiceStaffChange elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrVoiceStaffChange::visitEnd()" <<
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
    "newStaff: \"" << fNewStaff->getStaffName () << "\"";
    
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
  if (gTraceOptions->fTraceParts) {
    gLogIOstream <<
      "Creating part \"" << getPartCombinedName () << "\"" <<
      endl;
  }
  
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

msrPart::~msrPart()
{}

S_msrPart msrPart::createPartNewbornClone (S_msrPartGroup partGroupClone)
{
  if (gTraceOptions->fTraceParts) {
    gLogIOstream <<
      "Creating a newborn clone of part " <<
      getPartCombinedName () <<
      endl;
  }

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
  
    fPartFiguredBassStaff =
      addStaffToPartByItsNumber (
        inputLineNumber,
        msrStaff::kFiguredBassStaff,
        K_PART_FIGURED_BASS_STAFF_NUMBER);
      
    // create the part figured bass voice  
    if (gTraceOptions->fTraceHarmonies || gTraceOptions->fTraceVoices) {
      gLogIOstream <<
        "Creating figured bass voice " <<
        " number " << K_PART_FIGURED_BASS_VOICE_NUMBER <<
        " for part " <<
        getPartCombinedName () <<
        ", line " << inputLineNumber <<
        endl;
    }

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

void msrPart::setPartMeasureLengthHighTide (
  int      inputLineNumber,
  rational measureLength)
{
  if (gTraceOptions->fTraceDivisions || gTraceOptions->fTraceMeasures) {
    gLogIOstream <<
      "Setting measure length high tide for part \"" <<
      getPartCombinedName () <<
      "\" to " << measureLength <<
      ", line " << inputLineNumber <<
      endl;
  }

  fPartMeasureLengthHighTide = measureLength;
}

void msrPart::updatePartMeasureLengthHighTide (
  int      inputLineNumber,
  rational measureLength)
{
  if (measureLength > fPartMeasureLengthHighTide) {
    if (gTraceOptions->fTraceDivisions || gTraceOptions->fTraceMeasures) {
      gLogIOstream <<
        "Updating measure length high tide for part \"" <<
        getPartCombinedName () <<
        "\" to " << measureLength <<
        ", line " << inputLineNumber <<
        endl;
    }

    fPartMeasureLengthHighTide = measureLength;
  }
}

void msrPart::padUpToMeasureLengthInPart (
  int      inputLineNumber,
  rational measureLength)
{
  if (gTraceOptions->fTraceParts || gTraceOptions->fTraceMeasures) {
    gLogIOstream <<
      "Padding up to measure length '" << measureLength <<
      "' in part \"" <<
      getPartCombinedName () <<
      ", line " << inputLineNumber <<
      endl;
  }

  // print the registered staves to measure length  
  for (
    map<int, S_msrStaff>::const_iterator i = fPartStavesMap.begin ();
    i != fPartStavesMap.end ();
    i++) {
    (*i).second->
      padUpToMeasureLengthInStaff (
        inputLineNumber,
        measureLength);
  } // for
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

    if (gTraceOptions->fTraceParts) {
      gLogIOstream <<
        "Setting part name of " << getPartCombinedName () <<
        " to \"" << fPartMsrName << "\"" <<
         endl;
    }
  }
  else {
    // use the argument
    fPartMsrName = partMsrName;

    if (gTraceOptions->fTraceParts) {
      gLogIOstream <<
        "Keeping partID \"" << partMsrName <<
        "\" as part name  for " << getPartCombinedName () <<
      endl;
    }
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
  if (gTraceOptions->fTraceMeasures) {
    gLogIOstream <<
      "Creating and appending measure '" <<
      measureNumber <<
      "' to part " <<
      getPartCombinedName () <<
      "', line " << inputLineNumber <<
      endl;
  }

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
}

void msrPart::setNextMeasureNumberInPart (
  int    inputLineNumber,
  string nextMeasureNumber)
{
  if (gTraceOptions->fTraceMeasures) {
    gLogIOstream <<
      "Setting next measure number to '" <<
      nextMeasureNumber <<
      "' in part " <<
      getPartCombinedName () <<
      "', line " << inputLineNumber <<
      endl;
  }

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
}

/* JMI
void msrPart::complementPartVoicesUpToMeasure (
  int    inputLineNumber,
  string measureNumber)
{
  if (gTraceOptions->fTraceMeasures) {
    gLogIOstream <<
      "Complement part voices up to measure number " << measureNumber <<
      ", line " << inputLineNumber <<
      ", in part " << getPartCombinedName () <<
      endl;
  }

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
  if (gTraceOptions->fTraceStaves || gTraceOptions->fTraceParts) {
    gLogIOstream <<
      "Appending staff details\"" <<
      staffDetails->asShortString () <<
      "\" to part " << getPartCombinedName () <<
    endl;
  }

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

void msrPart::appendCommentToPart (S_msrComment comment)
{
  if (gTraceOptions->fTraceParts || gTraceOptions->fTraceClefs) {
    gLogIOstream <<
      "Appending comment '" <<
      comment->asString () <<
      "' to part " << getPartCombinedName () <<
    endl;
  }

  // append comment to registered staves
  for (
    map<int, S_msrStaff>::const_iterator i = fPartStavesMap.begin ();
    i != fPartStavesMap.end ();
    i++) {
    (*i).second->
      appendCommentToStaff (
        comment);
  } // for
}

void msrPart::appendClefToPart (S_msrClef clef)
{
  if (gTraceOptions->fTraceParts || gTraceOptions->fTraceClefs) {
    gLogIOstream <<
      "Appending clef '" <<
      clef->asString () <<
      "' to part " << getPartCombinedName () <<
    endl;
  }

  // append clef to registered staves
  for (
    map<int, S_msrStaff>::const_iterator i = fPartStavesMap.begin ();
    i != fPartStavesMap.end ();
    i++) {
    (*i).second->
      appendClefToStaff (
        clef);
  } // for
}

void msrPart::appendKeyToPart  (S_msrKey  key)
{
  if (gTraceOptions->fTraceKeys || gTraceOptions->fTraceParts) {
    gLogIOstream <<
      "Appending key '" <<
      key->asString () <<
      "' to part " << getPartCombinedName () <<
      endl;
  }
  
  // set part key
  fPartCurrentKey = key;

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
}

void msrPart::appendTimeToPart (S_msrTime time)
{
  if (gTraceOptions->fTraceTimes || gTraceOptions->fTraceParts) {
    gLogIOstream <<
      "Appending time '" <<
      time->asString () <<
      "' to part " << getPartCombinedName () <<
      endl;
  }
  
  // set part time
  fPartCurrentTime = time;

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
}

void msrPart::appendTimeToPartClone (S_msrTime time)
{
  if (gTraceOptions->fTraceTimes || gTraceOptions->fTraceParts) {
    gLogIOstream <<
      "Appending time '" <<
      time->asString () <<
      "' to part clone " << getPartCombinedName () <<
      endl;
  }
  
  // set part time
  fPartCurrentTime = time;

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
}

void msrPart::appendTransposeToPart (
  S_msrTranspose transpose)
{
  if (gTraceOptions->fTraceTranspositions || gTraceOptions->fTraceParts) {
    gLogIOstream <<
      "Appending transpose \"" <<
      transpose->asString () <<
      "\" to part " << getPartCombinedName () <<
    endl;
  }

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
  if (gTraceOptions->fTraceTranspositions || gTraceOptions->fTraceParts) {
    gLogIOstream <<
      "Appending part name display \"" <<
      partNameDisplay->asString () <<
      "\" to part " << getPartCombinedName () <<
    endl;
  }

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
  if (gTraceOptions->fTraceTranspositions || gTraceOptions->fTraceParts) {
    gLogIOstream <<
      "Appending part abbreviation display \"" <<
      partAbbreviationDisplay->asString () <<
      "\" to part " << getPartCombinedName () <<
    endl;
  }

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
  if (gTraceOptions->fTraceRepeats) {
    gLogIOstream <<
      "Preparing for repeat in part " <<
      getPartCombinedName () <<
      ", line " << inputLineNumber <<
      endl;
  }

  for (
    map<int, S_msrStaff>::const_iterator i = fPartStavesMap.begin ();
    i != fPartStavesMap.end ();
    i++) {
    (*i).second->
      prepareForRepeatInStaff (
        inputLineNumber);
  } // for
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
  int inputLineNumber,
  int repeatTimes)
{
  // create repeat and append it to registered staves
  for (
    map<int, S_msrStaff>::const_iterator i = fPartStavesMap.begin ();
    i != fPartStavesMap.end ();
    i++) {
    (*i).second->
      createRepeatUponItsEndAndAppendItToStaff (
        inputLineNumber,
        repeatTimes);
  } // for
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
  if (gTraceOptions->fTraceRepeats || gTraceOptions->fTraceParts) {
    gLogIOstream <<
      "Creating a multiple rest in part " <<
      getPartCombinedName () <<
      ", " <<
      singularOrPlural (
        multipleRestMeasuresNumber, "measure", "measures") <<
      endl;
  }

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
  if (gTraceOptions->fTraceRepeats || gTraceOptions->fTraceParts) {
    gLogIOstream <<
      "Appending the pending multiple rest to part " <<
      getPartCombinedName () <<
      endl;
  }

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
  if (gTraceOptions->fTraceStaves || gTraceOptions->fTraceParts) {
    gLogIOstream <<
      "Appending multiple rest '" <<
      multipleRest->asString () <<
      "' to part clone " <<
      getPartCombinedName () <<
      endl;
  }

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
      " already exists in part " << getPartCombinedName ();

    msrInternalError ( // JMI ???
      gXml2lyOptions->fInputSourceName,
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());
      
    return fPartStavesMap [staffNumber];
  }

  if (gTraceOptions->fTraceStaves || gTraceOptions->fTraceParts) {
    gLogIOstream <<
      "Adding " <<
      msrStaff::staffKindAsString (staffKind) <<
      " staff " << staffNumber <<
      " to part " << getPartCombinedName () <<
      endl;
  }
  
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
  if (gTraceOptions->fTraceStaves || gTraceOptions->fTraceParts) {
    gLogIOstream <<
      "Adding staff \"" << staff->getStaffName () <<
      "\" to part clone " << getPartCombinedName () <<
      endl;
  }

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
      if (gTraceOptions->fTraceHarmonies || gTraceOptions->fTraceParts) {
        gLogIOstream <<
          "Appending figured bass '" <<
          figuredBass->asString () <<
          "' to part " <<
          getPartCombinedName () <<
          ", line " << inputLineNumber <<
          endl;
      }
    
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
      if (gTraceOptions->fTraceFiguredBass || gTraceOptions->fTraceParts) {
        gLogIOstream <<
          "Appending figured bass '" <<
          figuredBass->asString () <<
          "' to part clone " <<
          getPartCombinedName () <<
          ", line " << inputLineNumber <<
          endl;
      }
    
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
  if (gTraceOptions->fTraceBasic || gTraceOptions->fTraceParts) {
    gLogIOstream <<
      "Appending scordatura '" <<
      scordatura->asString () <<
      "' to part " <<
      getPartCombinedName () <<
      endl;
  }

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
  if (gTraceOptions->fTraceBasic || gTraceOptions->fTraceParts) {
    gLogIOstream <<
      "Appending accordion registration '" <<
      accordionRegistration->asString () <<
      "' to part " <<
      getPartCombinedName () <<
      endl;
  }

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
  if (gTraceOptions->fTraceBasic || gTraceOptions->fTraceParts) {
    gLogIOstream <<
      "Appending harp pedals tuning '" <<
      harpPedalsTuning->asString () <<
      "' to part " <<
      getPartCombinedName () <<
      endl;
  }

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
        
  if (gTraceOptions->fTraceMeasures || gTraceOptions->fTraceParts) {
    gLogIOstream <<
      "appendSkipGraceNotesToVoicesClones() in " <<
      getPartCombinedName () <<
      ", graceNotesOriginVoiceMeasureLength = " <<
      graceNotesOriginVoiceMeasureLength <<
      ", line " << inputLineNumber <<
      endl;
  }

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
        
  if (gTraceOptions->fTraceMeasures || gTraceOptions->fTraceParts) {
    gLogIOstream <<
      "prependSkipGraceNotesToVoicesClones() in " <<
      getPartCombinedName () <<
      ", graceNotesOriginVoiceMeasureLength = " <<
      graceNotesOriginVoiceMeasureLength <<
      ", line " << inputLineNumber <<
      endl;
  }

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
  if (gTraceOptions->fTraceMeasures || gTraceOptions->fTraceParts) {
    gLogIOstream <<
      "Finalizing current measure in part " <<
      getPartCombinedName () <<
      ", line " << inputLineNumber <<
      endl;
  }
  
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

void msrPart::finalizePart (
  int inputLineNumber)
{
  if (gTraceOptions->fTraceParts) {
    gLogIOstream <<
      "Finalizing part " <<
      getPartCombinedName () <<
      ", line " << inputLineNumber <<
      endl;
  }

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
}

void msrPart::finalizePartClone (
  int inputLineNumber)
{
  if (gTraceOptions->fTraceParts) {
    gLogIOstream <<
      "Finalizing part clone " <<
      getPartCombinedName () <<
      ", line " << inputLineNumber <<
      endl;
  }

  // set score instrument names max lengthes if relevant
  setPartInstrumentNamesMaxLengthes ();
}

void msrPart::acceptIn (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrPart::acceptIn()" <<
      endl;
  }
      
  if (visitor<S_msrPart>*
    p =
      dynamic_cast<visitor<S_msrPart>*> (v)) {
        S_msrPart elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrPart::visitStart()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrPart::acceptOut (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrPart::acceptOut()" <<
      endl;
  }

  if (visitor<S_msrPart>*
    p =
      dynamic_cast<visitor<S_msrPart>*> (v)) {
        S_msrPart elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrPart::visitEnd()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrPart::browseData (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrPart::browseData()" <<
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
    "PartGroup uplink" << " : ";
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
      0,                                 // inputLineNumber
      partGroupNumber,
      partGroupAbsoluteNumber,
      partGroupName,
      partGroupNameDisplayText,
      partGroupAccidentalText,
      partGroupAbbreviation,
      msrPartGroup::k_NoPartGroupSymbol, // partGroupSymbolKind
      0,                                 // partGroupSymbolDefaultX,
      msrPartGroup::kPartGroupImplicitYes,
      partGroupBarlineKind,
      0,                                 // partGroupPartGroupUplink,
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

  if (gTraceOptions->fTracePartGroups) {
    gLogIOstream <<
      "--------------------------------------------" <<
      endl <<
      "Creating part group '" << fPartGroupNumber << "'" <<
      ", partGroupAbsoluteNumber = " << fPartGroupAbsoluteNumber <<
      ", line " << inputLineNumber <<
      endl;
  }
}

msrPartGroup::~msrPartGroup()
{}

S_msrPartGroup msrPartGroup::createPartGroupNewbornClone (
  S_msrPartGroup partGroupClone,
  S_msrScore     scoreClone)
{
  if (gTraceOptions->fTracePartGroups) {
    gLogIOstream <<
      "--------------------------------------------" <<
      endl <<
      "Creating a newborn clone part group " <<
      getPartGroupCombinedName () <<
      endl;
  }

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
  if (gTraceOptions->fTracePartGroups) {
    gLogIOstream <<
      "Adding part " <<
      part->getPartCombinedName () <<
      " to part group '" <<
      fPartGroupNumber <<
      "'" <<
      endl;
  }
  
  fPartGroupPartsMap [partID] = part;
  fPartGroupElements.push_back (part);

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
          getPartCombinedName() <<
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
  
  // return the part
  return part;
}

void msrPartGroup::appendPartToPartGroup (S_msrPart part)
{
  // register part in this part group
  if (gTraceOptions->fTracePartGroups) {
    gLogIOstream <<
      "Adding part " <<
      part->getPartCombinedName () <<
      " to part group " << fPartGroupNumber <<
      endl;
  }

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
  if (gTraceOptions->fTracePartGroups) {
    gLogIOstream <<
      "Removing part " <<
      partToBeRemoved->getPartCombinedName () <<
      " from part group " << fPartGroupNumber <<
      endl;
  }
  
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
}

void msrPartGroup::prependSubPartGroupToPartGroup (
  S_msrPartGroup partGroup)
{
  if (gTraceOptions->fTracePartGroups) {
    gLogIOstream <<
      "Prepending (sub-)part group " << partGroup->getPartGroupNumber () <<
      " to part group " << getPartGroupNumber ()  << endl;
  }

  // register it in this part group
  fPartGroupElements.push_front (partGroup);
}

void msrPartGroup::appendSubPartGroupToPartGroup (
  S_msrPartGroup partGroup)
{
  if (gTraceOptions->fTracePartGroups) {
    gLogIOstream <<
      "Appending (sub-)part group " << partGroup->getPartGroupNumber () <<
      " to part group " << getPartGroupNumber ()  << endl;
  }

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
      "% ==> msrPartGroup::acceptIn()" <<
      endl;
  }
      
  if (visitor<S_msrPartGroup>*
    p =
      dynamic_cast<visitor<S_msrPartGroup>*> (v)) {
        S_msrPartGroup elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrPartGroup::visitStart()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrPartGroup::acceptOut (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrPartGroup::acceptOut()" <<
      endl;
  }

  if (visitor<S_msrPartGroup>*
    p =
      dynamic_cast<visitor<S_msrPartGroup>*> (v)) {
        S_msrPartGroup elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrPartGroup::visitEnd()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrPartGroup::browseData (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrPartGroup::browseData()" <<
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
      "% <== msrPartGroup::browseData()" <<
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
    case msrPartGroup::k_NoPartGroupType:
      result = "none";
      break;
    case msrPartGroup::kStartPartGroupType:
      result = "start";
      break;
    case msrPartGroup::kStopPartGroupType:
      result = "stop";
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

string msrPartGroup::partGroupSymbolKindAsString (
  msrPartGroupSymbolKind partGroupSymbolKind)
{
  string result;
  
  switch (partGroupSymbolKind) {
    case msrPartGroup::k_NoPartGroupSymbol:
      result = "none";
      break;
    case msrPartGroup::kBracePartGroupSymbol:
      result = "brace";
      break;
    case msrPartGroup::kBracketPartGroupSymbol:
      result = "bracket";
      break;
    case msrPartGroup::kLinePartGroupSymbol:
      result = "line";
      break;
    case msrPartGroup::kSquarePartGroupSymbol:
      result = "square";
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
    "PartGroup uplink" << " : ";

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
    "PartGroupName" << " : \"" <<
    fPartGroupName <<
    "\"" <<
    endl;

   os << left <<
    setw (fieldWidth) <<
    "PartGroupPartGroupUplink" << " : ";
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
    "PartGroupNameDisplayText" << " : \"" <<
    fPartGroupNameDisplayText <<
    "\"" <<
    endl <<
    setw (fieldWidth) <<
    "PartGroupAccidentalText" << " : \"" <<
    fPartGroupAccidentalText <<
    "\"" <<
    endl <<
    setw (fieldWidth) <<
    "PartGroupAbbrevation" << " : \"" <<
    fPartGroupAbbreviation <<
    "\"" <<
    endl <<
    setw (fieldWidth) <<
    "PartGroupSymbolDefaultX" << " : " <<
    fPartGroupSymbolDefaultX <<
      endl <<
    setw (fieldWidth) <<
    "PartGroupSymbolKind" << " : " <<
    partGroupSymbolKindAsString (
      fPartGroupSymbolKind) <<
    endl;
    
  os << left <<
    setw (fieldWidth) <<
    "PartGroupImplicit" << " : " <<
    partGroupImplicitKindAsString (
      fPartGroupImplicitKind) <<
    endl;

  os << left <<
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
S_msrIdentification msrIdentification::create (
  int inputLineNumber)
{
  msrIdentification* o =
    new msrIdentification (
      inputLineNumber);
  assert(o!=0);
  return o;
}

msrIdentification::msrIdentification (
  int inputLineNumber)
    : msrElement (inputLineNumber)
{}

msrIdentification::~msrIdentification()
{}

void msrIdentification::setWorkNumber (
  int    inputLineNumber,
  string val)
  {
  fWorkNumber =
    msrVarValAssoc::create (
      inputLineNumber,
      msrVarValAssoc::kWorkNumber, val);
  }

void msrIdentification::setWorkTitle (
  int    inputLineNumber,
  string val)
  {
  fWorkTitle =
    msrVarValAssoc::create (
      inputLineNumber,
      msrVarValAssoc::kWorkTitle, val);
  }

void msrIdentification::setMovementNumber (
  int    inputLineNumber,
  string val)
  {
  fMovementNumber =
    msrVarValAssoc::create (
      inputLineNumber,
      msrVarValAssoc::kMovementNumber, val);
  }

void msrIdentification::setMovementTitle (
  int    inputLineNumber,
  string val)
{
  fMovementTitle =
    msrVarValAssoc::create (
      inputLineNumber,
      msrVarValAssoc::kMovementTitle, val);
}

void msrIdentification::setEncodingDate (
  int    inputLineNumber,
  string val)
{
  fEncodingDate =
    msrVarValAssoc::create (
      inputLineNumber,
      msrVarValAssoc::kEncodingDate, val);
}

void msrIdentification::setScoreInstrument (
  int    inputLineNumber,
  string val)
{
  fScoreInstrumentAssoc =
    msrVarValAssoc::create (
      inputLineNumber,
      msrVarValAssoc::kScoreInstrument, val);
}

void msrIdentification::setMiscellaneousField (
  int    inputLineNumber,
  string val)
{
  fEncodingDate =
    msrVarValAssoc::create (
      inputLineNumber,
      msrVarValAssoc::kMiscellaneousField, val);
}

void msrIdentification::addRights (
  int    inputLineNumber,
  string value)
{
  if (! fRights) {
    fRights =
      msrVarValsListAssoc::create (
        inputLineNumber,
        msrVarValsListAssoc::kRights);
  }
  
  fRights->
    addAssocVariableValue (value);
}

void msrIdentification::addComposer (
  int    inputLineNumber,
  string value)
{
  if (! fComposers) {
    fComposers =
      msrVarValsListAssoc::create (
        inputLineNumber,
        msrVarValsListAssoc::kComposer);
  }
  
  fComposers->
    addAssocVariableValue (value);
}

void msrIdentification::addArranger (
  int    inputLineNumber,
  string value)
{
  if (! fArrangers) {
    fArrangers =
      msrVarValsListAssoc::create (
        inputLineNumber,
        msrVarValsListAssoc::kArranger);
  }
  
  fArrangers->
    addAssocVariableValue (value);
}

void msrIdentification::addLyricist (
  int    inputLineNumber,
  string value)
{
  if (! fLyricists) {
    fLyricists =
      msrVarValsListAssoc::create (
        inputLineNumber,
        msrVarValsListAssoc::kLyricist);
  }
  
  fLyricists->
    addAssocVariableValue (value);
}

void msrIdentification::addPoet (
  int    inputLineNumber,
  string value)
{
  if (! fPoets) {
    fPoets =
      msrVarValsListAssoc::create (
        inputLineNumber,
        msrVarValsListAssoc::kPoet);
  }
  
  fPoets->
    addAssocVariableValue (value);
}

void msrIdentification::addTranslator (
  int    inputLineNumber,
  string value)
{
  if (! fTranslators) {
    fTranslators =
      msrVarValsListAssoc::create (
        inputLineNumber,
        msrVarValsListAssoc::kTranslator);
  }
  
  fTranslators->
    addAssocVariableValue (value);
}

void msrIdentification::addSoftware (
  int    inputLineNumber,
  string value)
{
  if (! fSoftwares) {
    fSoftwares =
      msrVarValsListAssoc::create (
        inputLineNumber,
        msrVarValsListAssoc::kSoftware);
  }
  
  fSoftwares->
    addAssocVariableValue (value);
}

void msrIdentification::acceptIn (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrIdentification::acceptIn()" <<
      endl;
  }
      
  if (visitor<S_msrIdentification>*
    p =
      dynamic_cast<visitor<S_msrIdentification>*> (v)) {
        S_msrIdentification elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrIdentification::visitStart()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrIdentification::acceptOut (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrIdentification::acceptOut()" <<
      endl;
  }

  if (visitor<S_msrIdentification>*
    p =
      dynamic_cast<visitor<S_msrIdentification>*> (v)) {
        S_msrIdentification elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrIdentification::visitEnd()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrIdentification::browseData (basevisitor* v)
{
  if (fWorkNumber) {
    // browse fWorkNumber
    msrBrowser<msrVarValAssoc> browser (v);
    browser.browse (*fWorkNumber);
  }

  if (fWorkTitle) {
    // browse fWorkTitle
    msrBrowser<msrVarValAssoc> browser (v);
    browser.browse (*fWorkTitle);
  }

  if (fMovementNumber) {
    // browse fMovementNumber
    msrBrowser<msrVarValAssoc> browser (v);
    browser.browse (*fMovementNumber);
  }

  if (fMovementTitle) {
    // browse fMovementTitle
    msrBrowser<msrVarValAssoc> browser (v);
    browser.browse (*fMovementTitle);
  }

  if (fComposers) {
    // browse fComposers
    msrBrowser<msrVarValsListAssoc> browser (v);
    browser.browse (*fComposers);
  }
    
  if (fArrangers) {
    // browse fArrangers
    msrBrowser<msrVarValsListAssoc> browser (v);
    browser.browse (*fArrangers);
  }
    
  if (fLyricists) {
    // browse fLyricists
    msrBrowser<msrVarValsListAssoc> browser (v);
    browser.browse (*fLyricists);
  }
    
  if (fPoets) {
    // browse fPoets
    msrBrowser<msrVarValsListAssoc> browser (v);
    browser.browse (*fPoets);
  }
    
  if (fTranslators) {
    // browse fTranslators
    msrBrowser<msrVarValsListAssoc> browser (v);
    browser.browse (*fTranslators);
  }
    
  if (fRights) {
    // browse fRights
    msrBrowser<msrVarValsListAssoc> browser (v);
    browser.browse (*fRights);
  }

  if (fSoftwares) {
    // browse fSoftwares
    msrBrowser<msrVarValsListAssoc> browser (v);
    browser.browse (*fSoftwares);
  }

  if (fEncodingDate) {
    // browse encoding date
    msrBrowser<msrVarValAssoc> browser (v);
    browser.browse (*fEncodingDate);
  }

  if (fScoreInstrumentAssoc) {
    // browse score instrument
    msrBrowser<msrVarValAssoc> browser (v);
    browser.browse (*fScoreInstrumentAssoc);
  }
}

ostream& operator<< (ostream& os, const S_msrIdentification& elt)
{
  elt->print (os);
  return os;
}

void msrIdentification::print (ostream& os)
{
  os <<
    "Identification" <<
    endl;

  bool emptyIdentification = true;

  gIndenter++;
  
  if (fWorkNumber) {
    os <<
      fWorkNumber <<
      endl;

    emptyIdentification = false;
  }
  
  if (fWorkTitle) {
    os <<
      fWorkTitle <<
      endl;

    emptyIdentification = false;
  }
    
  if (fMovementNumber) {
    os <<
      fMovementNumber <<
      endl;

    emptyIdentification = false;
  }
    
  if (fMovementTitle) {
    os <<
      fMovementTitle <<
      endl;

    emptyIdentification = false;
  }

  if (fRights) {
    os <<
      fRights <<
      endl;
      
    emptyIdentification = false;
  }
    
  if (fComposers) {
    os <<
      fComposers <<
      endl;
      
    emptyIdentification = false;
  }
    
  if (fArrangers) {
    os <<
      fArrangers <<
      endl;
      
    emptyIdentification = false;
  }
    
  if (fLyricists) {
    os <<
      fLyricists <<
      endl;
      
    emptyIdentification = false;
  }
    
  if (fPoets) {
    os <<
      fPoets <<
      endl;
      
    emptyIdentification = false;
  }
    
  if (fTranslators) {
    os <<
      fTranslators <<
      endl;
      
    emptyIdentification = false;
  }
    
  if (fSoftwares) {
    os <<
      fSoftwares <<
      endl;
    emptyIdentification = false;
  }
    
  if (fEncodingDate) {
    os <<
      fEncodingDate <<
      endl;
      
    emptyIdentification = false;
  }

  if (emptyIdentification)
    os <<
      " " << "nothing specified" <<
      endl <<
      endl;
  
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
  if (gTraceOptions->fTraceScore) {
    gLogIOstream <<
      "Creating a newborn clone of a score" <<
      endl;
  }

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
  if (gTraceOptions->fTraceCredits) {
    gLogIOstream <<
      "Appending credit '" <<
      credit->asString () <<
      "' to score" <<
      endl;
  }
  
  fCreditsList.push_back (credit);
}

S_msrPart msrScore::fetchPartFromScoreByItsPartID (
  int    inputLineNumber,
  string partID)
{
  S_msrPart result;
  
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
        (*i)->getPartGroupCombinedName() <<
        ", " <<
        (*i)->getPartGroupName () <<
        endl;
    } // for
  
    gIndenter--;

    if (gTraceOptions->fTracePartGroupsDetails) {
      gLogIOstream <<
        "<=- fetchPartFromScoreByItsPartID(" << partID << ")" <<
        endl <<
        endl;
    }
  }

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
      "% ==> msrScore::acceptIn()" <<
      endl;
  }
      
  if (visitor<S_msrScore>*
    p =
      dynamic_cast<visitor<S_msrScore>*> (v)) {
        S_msrScore elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrScore::visitStart()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrScore::acceptOut (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrScore::acceptOut()" <<
      endl;
  }

  if (visitor<S_msrScore>*
    p =
      dynamic_cast<visitor<S_msrScore>*> (v)) {
        S_msrScore elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrScore::visitEnd()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrScore::browseData (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrScore::browseData()" <<
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
      "% <== msrScore::browseData()" <<
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
  if (fPartGroupsList.size ()) {
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
  if (fPartGroupsList.size ()) {
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
  
  if (fPartGroupsList.size ()) {
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
S_msrMidi msrMidi::create (
  int    inputLineNumber,
  string midiTempoDuration,
  int    midiTempoPerSecond)
{
  msrMidi* o =
    new msrMidi (
      inputLineNumber,
      midiTempoDuration,
      midiTempoPerSecond);
  assert(o!=0);
  return o;
}

msrMidi::msrMidi (
  int    inputLineNumber,
  string midiTempoDuration,
  int    midiTempoPerSecond)
    : msrElement (inputLineNumber)
{
  fMidiTempoDuration = midiTempoDuration;
  fMidiTempoPerSecond = midiTempoPerSecond;
}

msrMidi::~msrMidi()
{}

void msrMidi::acceptIn (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrMidi::acceptIn()" <<
      endl;
  }
      
  if (visitor<S_msrMidi>*
    p =
      dynamic_cast<visitor<S_msrMidi>*> (v)) {
        S_msrMidi elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrMidi::visitStart()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrMidi::acceptOut (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrMidi::acceptOut()" <<
      endl;
  }

  if (visitor<S_msrMidi>*
    p =
      dynamic_cast<visitor<S_msrMidi>*> (v)) {
        S_msrMidi elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrMidi::visitEnd()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrMidi::browseData (basevisitor* v)
{}

ostream& operator<< (ostream& os, const S_msrMidi& elt)
{
  elt->print (os);
  return os;
}

void msrMidi::print (ostream& os)
{
  os << "Midi" << endl;

  gIndenter++;

  const int fieldWidth = 18;

  os << left <<
    setw (fieldWidth) <<
    "MidiTempoDuration" << " = " << fMidiTempoDuration <<
    endl <<
    setw (fieldWidth) <<
    "MidiTempoPerSecond" << " = " << fMidiTempoPerSecond <<
    endl <<
    endl;
      
  gIndenter--;
}    

//______________________________________________________________________________
void initializeMSR ()
{
  // languages handling
  // ------------------------------------------------------

  initializeQuarterTonesPitchesLanguageKinds ();
  
  // harmony handling
  // ------------------------------------------------------

  initializeChordIntervalsMap ();
  // JMI printChordIntervalsMap ();
}


}


/*

S_msrMeasuresRepeat msrMeasuresRepeat::createMeasuresRepeatNewbornClone (
  S_msrVoice containingVoice)
{
  if (gTraceOptions->fTraceRepeats) {
    gLogIOstream <<
      "Creating a newborn clone of a measure repeat" <<
      endl;
  }
  
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
  if (gTraceOptions->fTraceRepeats) {
    gLogIOstream <<
      "Creating a deep copy of a measure repeat" <<
      endl;
  }
  
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
  if (gTraceOptions->fTraceRepeats) {
    gLogIOstream <<
      "Creating a newborn clone of a measure repeat replicas" <<
      endl;
  }
  
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
  if (gTraceOptions->fTraceRepeats) {
    gLogIOstream <<
      "Creating a deep copy of a measure repeat replicas" <<
      endl;
  }
  
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
  if (gTraceOptions->fTraceRepeats) {
    gLogIOstream <<
      "Creating a newborn clone of a measure repeat pattern" <<
      endl;
  }
  
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
  if (gTraceOptions->fTraceRepeats) {
    gLogIOstream <<
      "Creating a newborn clone of a measure repeat pattern" <<
      endl;
  }
  
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
  if (gTraceOptions->fTraceRepeats) {
    gLogIOstream <<
      "Creating a newborn clone of a repeat" <<
      endl;
  }
  
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
  if (gTraceOptions->fTraceRepeats) {
    gLogIOstream <<
      "Creating a deep copy of a repeat" <<
      endl;
  }
  
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
  if (gTraceOptions->fTraceRepeats) {
    gLogIOstream <<
      "Creating a newborn clone of a " <<
      asString () <<
      endl;
  }
  
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
  if (gTraceOptions->fTraceRepeats) {
    gLogIOstream <<
      "Creating a newborn clone of a " <<
      asString () <<
      endl;
  }
  
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
  if (gTraceOptions->fTraceRepeats) {
    gLogIOstream <<
      "Creating a newborn clone of a " <<
      asString () <<
      endl;
  }
  
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
  if (gTraceOptions->fTraceRepeats) {
    gLogIOstream <<
      "Creating a newborn clone of a " <<
      asString () <<
      endl;
  }
  
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

