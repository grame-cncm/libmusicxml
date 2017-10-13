/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#include <climits>      /* INT_MIN, INT_MAX */
#include <sstream>
#include <iomanip>      // setw, set::precision, ...

#include "version.h"

#include "msrOptions.h"
#include "musicXMLOptions.h"

#include "msr.h"

using namespace std;


namespace MusicXML2 
{

// useful shortcut macros
#define idtr indenter::gIndenter
#define tab  indenter::gIndenter.getSpacer ()

//______________________________________________________________________________
msrElement::msrElement (
  int inputLineNumber)
{
  fInputLineNumber = inputLineNumber;  
}

msrElement::~msrElement()
{}

void msrElement::acceptIn (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors)
    gLogIos <<
      "% ==> msrElement::acceptIn()" <<
      endl;
      
  if (visitor<S_msrElement>*
    p =
      dynamic_cast<visitor<S_msrElement>*> (v)) {
        S_msrElement elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors)
          gLogIos <<
            "% ==> Launching msrElement::visitStart()" <<
             endl;
        p->visitStart (elem);
  }
}

void msrElement::acceptOut (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors)
    gLogIos <<
      "% ==> msrElement::acceptOut()" <<
      endl;

  if (visitor<S_msrElement>*
    p =
      dynamic_cast<visitor<S_msrElement>*> (v)) {
        S_msrElement elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors)
          gLogIos <<
            "% ==> Launching msrElement::visitEnd()" <<
            endl;
        p->visitEnd (elem);
  }
}

ostream& operator<< (ostream& os, const S_msrElement& elt)
{
  elt->print (os);
  return os;
}

void msrElement::print (ostream& os)
{
  os << "??? Element ???" << endl;
}

//______________________________________________________________________________
void msrPolyphony::print (ostream& os)
{
  const int fieldWidth = 9;

/* JMI
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
void msrBeatData::print (ostream& os)
{
  const int fieldWidth = 9;

  os << left <<
    setw (fieldWidth) <<
    "fBeatUnit" << " = " << fBeatUnit <<
    endl <<
    setw (fieldWidth) <<
    "fDots" << " = " << fDots <<
    endl;
};

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

msrOctaveShift::~msrOctaveShift()
{}

void msrOctaveShift::acceptIn (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors)
    gLogIos <<
      "% ==> msrOctaveShift::acceptIn()" <<
      endl;
      
  if (visitor<S_msrOctaveShift>*
    p =
      dynamic_cast<visitor<S_msrOctaveShift>*> (v)) {
        S_msrOctaveShift elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors)
          gLogIos <<
            "% ==> Launching msrOctaveShift::visitStart()" <<
             endl;
        p->visitStart (elem);
  }
}

void msrOctaveShift::acceptOut (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors)
    gLogIos <<
      "% ==> msrOctaveShift::acceptOut()" <<
      endl;

  if (visitor<S_msrOctaveShift>*
    p =
      dynamic_cast<visitor<S_msrOctaveShift>*> (v)) {
        S_msrOctaveShift elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors)
          gLogIos <<
            "% ==> Launching msrOctaveShift::visitEnd()" <<
            endl;
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
    case kOctaveShiftUp:
      result = "up";
      break;
    case kOctaveShiftDown:
      result = "down";
      break;
    case kOctaveShiftStop:
      result = "stop";
      break;
  } // switch

  return result;
}

void msrOctaveShift::print (ostream& os)
{
  idtr++;
  
  os <<
    "OctaveShift" <<
    ", kind: " << octaveShiftKindAsString () <<
    ", size: " << fOctaveShiftSize <<
    endl;

  idtr--;
}

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

void msrAccordionRegistration::acceptIn (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors)
    gLogIos <<
      "% ==> msrAccordionRegistration::acceptIn()" <<
      endl;
      
  if (visitor<S_msrAccordionRegistration>*
    p =
      dynamic_cast<visitor<S_msrAccordionRegistration>*> (v)) {
        S_msrAccordionRegistration elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors)
          gLogIos <<
            "% ==> Launching msrAccordionRegistration::visitStart()" <<
             endl;
        p->visitStart (elem);
  }
}

void msrAccordionRegistration::acceptOut (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors)
    gLogIos <<
      "% ==> msrAccordionRegistration::acceptOut()" <<
      endl;

  if (visitor<S_msrAccordionRegistration>*
    p =
      dynamic_cast<visitor<S_msrAccordionRegistration>*> (v)) {
        S_msrAccordionRegistration elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors)
          gLogIos <<
            "% ==> Launching msrAccordionRegistration::visitEnd()" <<
            endl;
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

string msrAccordionRegistration::accordionRegistrationAsString () const
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
  idtr++;
  
  os <<
    accordionRegistrationAsString () <<
    endl;

  idtr--;
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

void msrStem::acceptIn (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors)
    gLogIos <<
      "% ==> msrStem::acceptIn()" <<
      endl;
      
  if (visitor<S_msrStem>*
    p =
      dynamic_cast<visitor<S_msrStem>*> (v)) {
        S_msrStem elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors)
          gLogIos <<
            "% ==> Launching msrStem::visitStart()" <<
             endl;
        p->visitStart (elem);
  }
}

void msrStem::acceptOut (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors)
    gLogIos <<
      "% ==> msrStem::acceptOut()" <<
      endl;

  if (visitor<S_msrStem>*
    p =
      dynamic_cast<visitor<S_msrStem>*> (v)) {
        S_msrStem elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors)
          gLogIos <<
            "% ==> Launching msrStem::visitEnd()" <<
            endl;
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
  idtr++;
  
  os <<
    "Stem" " " <<
    stemKindAsString (fStemKind) <<
    ", line " << fInputLineNumber <<
    endl;

  idtr--;
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
}

msrBeam::~msrBeam()
{}

void msrBeam::acceptIn (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors)
    gLogIos <<
      "% ==> msrBeam::acceptIn()" <<
      endl;
      
  if (visitor<S_msrBeam>*
    p =
      dynamic_cast<visitor<S_msrBeam>*> (v)) {
        S_msrBeam elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors)
          gLogIos <<
            "% ==> Launching msrBeam::visitStart()" <<
             endl;
        p->visitStart (elem);
  }
}

void msrBeam::acceptOut (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors)
    gLogIos <<
      "% ==> msrBeam::acceptOut()" <<
      endl;

  if (visitor<S_msrBeam>*
    p =
      dynamic_cast<visitor<S_msrBeam>*> (v)) {
        S_msrBeam elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors)
          gLogIos <<
            "% ==> Launching msrBeam::visitEnd()" <<
            endl;
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

string msrBeam::beamAsString () const
{
  stringstream s;

  s <<
    "Beam" <<
    " number " << fBeamNumber <<
    ", line " << fInputLineNumber <<
    ", " << beamKindAsString (fBeamKind);

  return s.str ();
}

void msrBeam::print (ostream& os)
{  
  os <<
    beamAsString () <<
    endl;
}

//______________________________________________________________________________
S_msrArticulation msrArticulation::create (
  int                 inputLineNumber,
  msrArticulationKind articulationKind,
  msrPlacement        articulationPlacement)
{
  msrArticulation* o =
    new msrArticulation (
      inputLineNumber,
      articulationKind,
      articulationPlacement);
  assert (o!=0);
  return o;
}

msrArticulation::msrArticulation (
  int                 inputLineNumber,
  msrArticulationKind articulationKind,
  msrPlacement        articulationPlacement)
    : msrElement (inputLineNumber)
{
  fArticulationKind = articulationKind;

  fArticulationPlacement = articulationPlacement;
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

string msrArticulation::articulationPlacementAsString () const
{
  return
    msrPlacementAsString (
      fArticulationPlacement);
}

void msrArticulation::acceptIn (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors)
    gLogIos <<
      "% ==> msrArticulation::acceptIn()" <<
      endl;
      
  if (visitor<S_msrArticulation>*
    p =
      dynamic_cast<visitor<S_msrArticulation>*> (v)) {
        S_msrArticulation elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors)
          gLogIos <<
            "% ==> Launching msrArticulation::visitStart()" <<
             endl;
        p->visitStart (elem);
  }
}

void msrArticulation::acceptOut (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors)
    gLogIos <<
      "% ==> msrArticulation::acceptOut()" <<
      endl;

  if (visitor<S_msrArticulation>*
    p =
      dynamic_cast<visitor<S_msrArticulation>*> (v)) {
        S_msrArticulation elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors)
          gLogIos <<
            "% ==> Launching msrArticulation::visitEnd()" <<
            endl;
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
    articulationPlacementAsString () <<
    ", line " << fInputLineNumber <<
    endl;
}

//______________________________________________________________________________
S_msrArpeggiato msrArpeggiato::create (
  int          inputLineNumber,
  msrPlacement arpeggiatoPlacement,
  msrDirection arpeggiatoDirection,
  int          arpeggiatoNumber)
{
  msrArpeggiato* o =
    new msrArpeggiato (
      inputLineNumber,
      arpeggiatoPlacement,
      arpeggiatoDirection,
      arpeggiatoNumber);
  assert (o!=0);
  return o;
}

msrArpeggiato::msrArpeggiato (
  int          inputLineNumber,
  msrPlacement arpeggiatoPlacement,
  msrDirection arpeggiatoDirection,
  int          arpeggiatoNumber)
    : msrArticulation (
      inputLineNumber,
      kArpeggiato,
      arpeggiatoPlacement)
{  
  fArpeggiatoDirection = arpeggiatoDirection;
  
  fArpeggiatoNumber = arpeggiatoNumber;
}

msrArpeggiato::~msrArpeggiato()
{}

string msrArpeggiato::arpeggiatoDirectionAsString () const
{
  return
    msrDirectionAsString (fArpeggiatoDirection);
}

void msrArpeggiato::acceptIn (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors)
    gLogIos <<
      "% ==> msrArpeggiato::acceptIn()" <<
      endl;
      
  if (visitor<S_msrArpeggiato>*
    p =
      dynamic_cast<visitor<S_msrArpeggiato>*> (v)) {
        S_msrArpeggiato elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors)
          gLogIos <<
            "% ==> Launching msrArpeggiato::visitStart()" <<
             endl;
        p->visitStart (elem);
  }
}

void msrArpeggiato::acceptOut (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors)
    gLogIos <<
      "% ==> msrArpeggiato::acceptOut()" <<
      endl;

  if (visitor<S_msrArpeggiato>*
    p =
      dynamic_cast<visitor<S_msrArpeggiato>*> (v)) {
        S_msrArpeggiato elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors)
          gLogIos <<
            "% ==> Launching msrArpeggiato::visitEnd()" <<
            endl;
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
    articulationPlacementAsString () <<
    ", " <<
    arpeggiatoDirectionAsString () <<
    ", number: " <<
    fArpeggiatoNumber <<
    ", line " << fInputLineNumber <<
    endl;
}

//______________________________________________________________________________
S_msrNonArpeggiato msrNonArpeggiato::create (
  int                      inputLineNumber,
  msrPlacement             nonArpeggiatoPlacement,
  msrNonArpeggiatoTypeKind nonArpeggiatoTypeKind,
  int                      nonArpeggiatoNumber)
{
  msrNonArpeggiato* o =
    new msrNonArpeggiato (
      inputLineNumber,
      nonArpeggiatoPlacement,
      nonArpeggiatoTypeKind,
      nonArpeggiatoNumber);
  assert (o!=0);
  return o;
}

msrNonArpeggiato::msrNonArpeggiato (
  int                      inputLineNumber,
  msrPlacement             nonArpeggiatoPlacement,
  msrNonArpeggiatoTypeKind nonArpeggiatoTypeKind,
  int                      nonArpeggiatoNumber)
    : msrArticulation (
      inputLineNumber,
      kNonArpeggiato,
      nonArpeggiatoPlacement)
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
    case msrNonArpeggiato::k_NoNonArpeggiatoType:
      result = "nonArpeggiatoType: none";
      break;
    case msrNonArpeggiato::kNonArpeggiatoTypeTop:
      result = "nonArpeggiatoType: top";
      break;
    case msrNonArpeggiato::kNonArpeggiatoTypeBottom:
      result = "nonArpeggiatoType: bottom";
      break;
  } // switch

  return result;
}

string msrNonArpeggiato::nonArpeggiatoTypeKindAsString () const
{
  return
    nonArpeggiatoTypeKindAsString (fNonArpeggiatoTypeKind);
}

void msrNonArpeggiato::acceptIn (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors)
    gLogIos <<
      "% ==> msrNonArpeggiato::acceptIn()" <<
      endl;
      
  if (visitor<S_msrNonArpeggiato>*
    p =
      dynamic_cast<visitor<S_msrNonArpeggiato>*> (v)) {
        S_msrNonArpeggiato elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors)
          gLogIos <<
            "% ==> Launching msrNonArpeggiato::visitStart()" <<
             endl;
        p->visitStart (elem);
  }
}

void msrNonArpeggiato::acceptOut (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors)
    gLogIos <<
      "% ==> msrNonArpeggiato::acceptOut()" <<
      endl;

  if (visitor<S_msrNonArpeggiato>*
    p =
      dynamic_cast<visitor<S_msrNonArpeggiato>*> (v)) {
        S_msrNonArpeggiato elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors)
          gLogIos <<
            "% ==> Launching msrNonArpeggiato::visitEnd()" <<
            endl;
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
    articulationPlacementAsString () <<
    ", " <<
    nonArpeggiatoTypeKindAsString () <<
    ", number: " <<
    fNonArpeggiatoNumber <<
    ", line " << fInputLineNumber <<
    endl;
}

//______________________________________________________________________________
S_msrTechnical msrTechnical::create (
  int                       inputLineNumber,
  msrTechnicalKind          technicalKind,
  msrTechnicalPlacementKind technicalPlacementKind)
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
  int                       inputLineNumber,
  msrTechnicalKind          technicalKind,
  msrTechnicalPlacementKind technicalPlacementKind)
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
  string result;
  
  switch (fTechnicalPlacementKind) {
    case msrTechnical::k_NoTechnicalPlacement:
      result = "none";
      break;
    case msrTechnical::kTechnicalPlacementAbove:
      result = "above";
      break;
    case msrTechnical::kTechnicalPlacementBelow:
      result = "below";
      break;
  } // switch

  return result;
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

void msrTechnical::acceptIn (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors)
    gLogIos <<
      "% ==> msrTechnical::acceptIn()" <<
      endl;
      
  if (visitor<S_msrTechnical>*
    p =
      dynamic_cast<visitor<S_msrTechnical>*> (v)) {
        S_msrTechnical elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors)
          gLogIos <<
            "% ==> Launching msrTechnical::visitStart()" <<
             endl;
        p->visitStart (elem);
  }
}

void msrTechnical::acceptOut (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors)
    gLogIos <<
      "% ==> msrTechnical::acceptOut()" <<
      endl;

  if (visitor<S_msrTechnical>*
    p =
      dynamic_cast<visitor<S_msrTechnical>*> (v)) {
        S_msrTechnical elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors)
          gLogIos <<
            "% ==> Launching msrTechnical::visitEnd()" <<
            endl;
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

string msrTechnical::technicalAsString () const
{
  string result;
  
  switch (fTechnicalPlacementKind) {
    case msrTechnical::k_NoTechnicalPlacement:
      result = "none";
      break;
    case msrTechnical::kTechnicalPlacementAbove:
      result = "above";
      break;
    case msrTechnical::kTechnicalPlacementBelow:
      result = "below";
      break;
  } // switch

  return result;
}

void msrTechnical::print (ostream& os)
{
  os <<
    "Technical" " " <<
    technicalAsString () <<
    ", line " << fInputLineNumber <<
//    ", accidental mark" << " = " << technicalAccidentalMarkKindAsString () <<
    ", note uplink" << " = " <<
    fTechnicalNoteUplink->noteAsShortString () <<
    endl;
}

//______________________________________________________________________________
S_msrTechnicalWithInteger msrTechnicalWithInteger::create (
  int                         inputLineNumber,
  msrTechnicalWithIntegerKind technicalWithIntegerKind,
  int                         technicalWithIntegerValue,
  msrTechnicalWithIntegerPlacementKind
                              technicalWithIntegerPlacementKind)
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
  msrTechnicalWithIntegerPlacementKind
                              technicalWithIntegerPlacementKind)
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
      result = "Bend";
      break;
    case msrTechnicalWithInteger::kFingering:
      result = "Fingering";
      break;
    case msrTechnicalWithInteger::kFret:
      result = "Fret";
      break;
    case msrTechnicalWithInteger::kString:
      result = "String";
      break;
  } // switch

  return result;
}

string msrTechnicalWithInteger::technicalWithIntegerPlacementKindAsString () const
{
  string result;
  
  switch (fTechnicalWithIntegerPlacementKind) {
    case msrTechnicalWithInteger::k_NoTechnicalWithIntegerPlacement:
      result = "none";
      break;
    case msrTechnicalWithInteger::kTechnicalWithIntegerPlacementAbove:
      result = "above";
      break;
    case msrTechnicalWithInteger::kTechnicalWithIntegerPlacementBelow:
      result = "below";
      break;
  } // switch

  return result;
}

void msrTechnicalWithInteger::acceptIn (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors)
    gLogIos <<
      "% ==> msrTechnicalWithInteger::acceptIn()" <<
      endl;
      
  if (visitor<S_msrTechnicalWithInteger>*
    p =
      dynamic_cast<visitor<S_msrTechnicalWithInteger>*> (v)) {
        S_msrTechnicalWithInteger elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors)
          gLogIos <<
            "% ==> Launching msrTechnicalWithInteger::visitStart()" <<
             endl;
        p->visitStart (elem);
  }
}

void msrTechnicalWithInteger::acceptOut (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors)
    gLogIos <<
      "% ==> msrTechnicalWithInteger::acceptOut()" <<
      endl;

  if (visitor<S_msrTechnicalWithInteger>*
    p =
      dynamic_cast<visitor<S_msrTechnicalWithInteger>*> (v)) {
        S_msrTechnicalWithInteger elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors)
          gLogIos <<
            "% ==> Launching msrTechnicalWithInteger::visitEnd()" <<
            endl;
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

string msrTechnicalWithInteger::technicalWithIntegerAsString () const
{
  stringstream s;

  s <<
    technicalWithIntegerKindAsString () <<
    " " <<
    fTechnicalWithIntegerValue <<
    ", placement " <<
    technicalWithIntegerPlacementKindAsString ();

  return s.str ();
}

void msrTechnicalWithInteger::print (ostream& os)
{
  os <<
    "TechnicalWithInteger" " " <<
    technicalWithIntegerAsString () <<
    ", line " << fInputLineNumber <<
    ", note uplink" << " = " <<
    fTechnicalWithIntegerNoteUplink->noteAsShortString () <<
    endl;
}

//______________________________________________________________________________
S_msrTechnicalWithString msrTechnicalWithString::create (
  int                         inputLineNumber,
  msrTechnicalWithStringKind technicalWithStringKind,
  string                      technicalWithStringValue,
  msrTechnicalWithStringPlacementKind
                              technicalWithStringPlacementKind)
{
  msrTechnicalWithString* o =
    new msrTechnicalWithString (
      inputLineNumber,
      technicalWithStringKind,
      technicalWithStringValue,
      technicalWithStringPlacementKind);
  assert (o!=0);
  return o;
}

msrTechnicalWithString::msrTechnicalWithString (
  int                         inputLineNumber,
  msrTechnicalWithStringKind technicalWithStringKind,
  string                      technicalWithStringValue,
  msrTechnicalWithStringPlacementKind
                              technicalWithStringPlacementKind)
    : msrElement (inputLineNumber)
{
  fTechnicalWithStringKind = technicalWithStringKind;

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
    " " + fTechnicalWithStringValue;

  return result;
}

string msrTechnicalWithString::technicalWithStringPlacementKindAsString () const
{
  string result;
  
  switch (fTechnicalWithStringPlacementKind) {
    case msrTechnicalWithString::k_NoTechnicalWithStringPlacement:
      result = "none";
      break;
    case msrTechnicalWithString::kTechnicalWithStringPlacementAbove:
      result = "above";
      break;
    case msrTechnicalWithString::kTechnicalWithStringPlacementBelow:
      result = "below";
      break;
  } // switch

  return result;
}

void msrTechnicalWithString::acceptIn (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors)
    gLogIos <<
      "% ==> msrTechnicalWithString::acceptIn()" <<
      endl;
      
  if (visitor<S_msrTechnicalWithString>*
    p =
      dynamic_cast<visitor<S_msrTechnicalWithString>*> (v)) {
        S_msrTechnicalWithString elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors)
          gLogIos <<
            "% ==> Launching msrTechnicalWithString::visitStart()" <<
             endl;
        p->visitStart (elem);
  }
}

void msrTechnicalWithString::acceptOut (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors)
    gLogIos <<
      "% ==> msrTechnicalWithString::acceptOut()" <<
      endl;

  if (visitor<S_msrTechnicalWithString>*
    p =
      dynamic_cast<visitor<S_msrTechnicalWithString>*> (v)) {
        S_msrTechnicalWithString elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors)
          gLogIos <<
            "% ==> Launching msrTechnicalWithString::visitEnd()" <<
            endl;
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

string msrTechnicalWithString::technicalWithStringAsString () const
{
  stringstream s;

  s <<
    technicalWithStringKindAsString () <<
    " " <<
    fTechnicalWithStringValue <<
    ", placement " <<
    technicalWithStringPlacementKindAsString ();

  return s.str ();
}

void msrTechnicalWithString::print (ostream& os)
{
  os <<
    "TechnicalWithString" " " <<
    technicalWithStringAsString () <<
    ", line " << fInputLineNumber <<
    ", note uplink" << " = " <<
    fTechnicalWithStringNoteUplink->noteAsShortString () <<
    endl;
}

//______________________________________________________________________________
S_msrFermata msrFermata::create (
  int            inputLineNumber,
  msrFermataKind fermataKind,
  msrFermataType fermataType)
{
  msrFermata* o =
    new msrFermata (
      inputLineNumber,
      fermataKind,
      fermataType);
  assert (o!=0);
  return o;
}

msrFermata::msrFermata (
    int            inputLineNumber,
    msrFermataKind fermataKind,
    msrFermataType fermataType)
    : msrArticulation (
      inputLineNumber,
      msrArticulation::kFermata,
      k_NoPlacement) // temporary, JMI TEMP
{
  fFermataKind = fermataKind;
  fFermataType = fermataType;
}

msrFermata::~msrFermata()
{}

void msrFermata::acceptIn (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors)
    gLogIos <<
      "% ==> msrFermata::acceptIn()" <<
      endl;
      
  if (visitor<S_msrFermata>*
    p =
      dynamic_cast<visitor<S_msrFermata>*> (v)) {
        S_msrFermata elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors)
          gLogIos <<
            "% ==> Launching msrFermata::visitStart()" <<
             endl;
        p->visitStart (elem);
  }
}

void msrFermata::acceptOut (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors)
    gLogIos <<
      "% ==> msrFermata::acceptOut()" <<
      endl;

  if (visitor<S_msrFermata>*
    p =
      dynamic_cast<visitor<S_msrFermata>*> (v)) {
        S_msrFermata elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors)
          gLogIos <<
            "% ==> Launching msrFermata::visitEnd()" <<
            endl;
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
      
string msrFermata::fermataTypeAsString (
  msrFermataType fermataType)
{
  string result;
  
  switch (fermataType) {
    case msrFermata::k_NoFermataType:
      result = "fermata type: none";
      break;
    case msrFermata::kUprightFermataType:
      result = "fermata type: upright";
      break;
    case msrFermata::kInvertedFermataType:
      result = "fermata type: inverted";
      break;
  } // switch

  return result;
}
      
ostream& operator<< (ostream& os, const S_msrFermata& elt)
{
  elt->print (os);
  return os;
}

string msrFermata::fermataAsString () const
{
  stringstream s;

  s <<
    "Fermata" <<
    ", " <<
    fermataKindAsString (fFermataKind) <<
    ", " <<
    fermataTypeAsString (fFermataType) <<
    ", line " << fInputLineNumber;

  return s.str ();
}

void msrFermata::print (ostream& os)
{
  os <<
    fermataAsString () <<
    endl;
}

//______________________________________________________________________________
S_msrOrnament msrOrnament::create (
  int             inputLineNumber,
  msrOrnamentKind ornamentKind)
{
  msrOrnament* o =
    new msrOrnament (
      inputLineNumber, ornamentKind);
  assert (o!=0);
  return o;
}

msrOrnament::msrOrnament (
  int             inputLineNumber,
  msrOrnamentKind ornamentKind)
    : msrElement (inputLineNumber)
{
  fOrnamentKind = ornamentKind;

  fOrnamentPlacementKind = k_NoOrnamentPlacement;

  fOrnamentAccidentalMark = kNatural;
}

msrOrnament::~msrOrnament()
{}

string msrOrnament::ornamentKindAsString () const
{
  string result;
  
  switch (fOrnamentKind) {
    case msrOrnament::kTrillMark:
      result = "trill";
      break;
    case msrOrnament::kWavyLine:
      result = "wayvyLine";
      break;
    case msrOrnament::kTurn:
      result = "turn";
      break;
    case msrOrnament::kInvertedTurn:
      result = "invertedTurn";
      break;
    case msrOrnament::kDelayedTurn:
      result = "delayedTurn";
      break;
    case msrOrnament::kDelayedInvertedTurn:
      result = "delayedInvertedTurn";
      break;
    case msrOrnament::kVerticalTurn:
      result = "verticTurn";
      break;
    case msrOrnament::kMordent:
      result = "mordent";
      break;
    case msrOrnament::kInvertedMordent:
      result = "invertedMordent";
      break;
    case msrOrnament::kSchleifer:
      result = "schleifer";
      break;
    case msrOrnament::kShake:
      result = "shake";
      break;
    case msrOrnament::kAccidentalMark:
      result = "accidentalMark";
      break;
  } // switch

  return result;
}

string msrOrnament::ornamentPlacementKindAsString () const
{
  string result;
  
  switch (fOrnamentPlacementKind) {
    case msrOrnament::k_NoOrnamentPlacement:
      result = "none";
      break;
    case msrOrnament::kOrnamentPlacementAbove:
      result = "above";
      break;
    case msrOrnament::kOrnamentPlacementBelow:
      result = "below";
      break;
  } // switch

  return result;
}

string msrOrnament::ornamentAccidentalMarkAsString () const
{
  string result;
  
  switch (fOrnamentAccidentalMark) {
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
    case k_NoAlteration:
      result = "k_NoAlteration???";
      break;
  } // switch

  return result;
}

void msrOrnament::acceptIn (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors)
    gLogIos <<
      "% ==> msrOrnament::acceptIn()" <<
      endl;
      
  if (visitor<S_msrOrnament>*
    p =
      dynamic_cast<visitor<S_msrOrnament>*> (v)) {
        S_msrOrnament elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors)
          gLogIos <<
            "% ==> Launching msrOrnament::visitStart()" <<
             endl;
        p->visitStart (elem);
  }
}

void msrOrnament::acceptOut (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors)
    gLogIos <<
      "% ==> msrOrnament::acceptOut()" <<
      endl;

  if (visitor<S_msrOrnament>*
    p =
      dynamic_cast<visitor<S_msrOrnament>*> (v)) {
        S_msrOrnament elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors)
          gLogIos <<
            "% ==> Launching msrOrnament::visitEnd()" <<
            endl;
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
    "Ornament" " " <<
    ornamentKindAsString () <<
    ", line " << fInputLineNumber <<
    endl;

  idtr++;
  
  os <<
    ", placement" << " = " << ornamentPlacementKindAsString () <<
    ", accidental mark" << " = " << ornamentAccidentalMarkAsString () <<
    ", note uplink" << " = " << fOrnamentNoteUplink->noteAsShortString () <<
    endl;

  idtr--;
}

//______________________________________________________________________________
S_msrSingleTremolo msrSingleTremolo::create (
  int inputLineNumber,
  int singleTremoloMarksNumber,
  msrSingleTremoloPlacementKind
      singleTremoloPlacementKind)
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
  if (gGeneralOptions->fTraceNotes) {
    gLogIos <<
      "==> Creating a deep copy of single tremolo " <<
      singleTremoloAsString () <<
      " in note '" <<
      noteUplink->
        noteAsShortString () <<
      "'" <<
      endl;
  }

  // sanity check
  msrAssert(
    noteUplink != 0,
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
  int inputLineNumber,
  int singleTremoloMarksNumber,
  msrSingleTremoloPlacementKind
      singleTremoloPlacementKind)
    : msrElement (inputLineNumber)
{
  fSingleTremoloMarksNumber   = singleTremoloMarksNumber;
  fSingleTremoloPlacementKind = singleTremoloPlacementKind;
}

msrSingleTremolo::~msrSingleTremolo()
{}

string msrSingleTremolo::singleTremoloPlacementKindAsString (
  msrSingleTremoloPlacementKind singleTremoloPlacementKind)
{
  string result;
  
  switch (singleTremoloPlacementKind) {
    case msrSingleTremolo::k_NoSingleTremoloPlacement:
      result = "none";
      break;
    case msrSingleTremolo::kSingleTremoloPlacementAbove:
      result = "above";
      break;
    case msrSingleTremolo::kSingleTremoloPlacementBelow:
      result = "below";
      break;
  } // switch

  return result;
}

string msrSingleTremolo::singleTremoloPlacementKindAsString () const
{
  return
    singleTremoloPlacementKindAsString (
      fSingleTremoloPlacementKind);
}

void msrSingleTremolo::acceptIn (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors)
    gLogIos <<
      "% ==> msrSingleTremolo::acceptIn()" <<
      endl;
      
  if (visitor<S_msrSingleTremolo>*
    p =
      dynamic_cast<visitor<S_msrSingleTremolo>*> (v)) {
        S_msrSingleTremolo elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors)
          gLogIos <<
            "% ==> Launching msrSingleTremolo::visitStart()" <<
             endl;
        p->visitStart (elem);
  }
}

void msrSingleTremolo::acceptOut (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors)
    gLogIos <<
      "% ==> msrSingleTremolo::acceptOut()" <<
      endl;

  if (visitor<S_msrSingleTremolo>*
    p =
      dynamic_cast<visitor<S_msrSingleTremolo>*> (v)) {
        S_msrSingleTremolo elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors)
          gLogIos <<
            "% ==> Launching msrSingleTremolo::visitEnd()" <<
            endl;
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

string msrSingleTremolo::singleTremoloAsString () const
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
        noteAsShortStringWithRawWholeNotes ();

  return s.str ();
}

void msrSingleTremolo::print (ostream& os)
{
  os <<
    singleTremoloAsString () <<
    endl;
}

//______________________________________________________________________________
S_msrDoubleTremolo msrDoubleTremolo::create (
  int                  inputLineNumber,
  msrDoubleTremoloKind doubleTremoloKind,
  int                  doubleTremoloMarksNumber,
  msrDoubleTremoloPlacementKind
                       doubleTremoloPlacementKind,
  S_msrVoice           voiceUplink)
{
  msrDoubleTremolo* o =
    new msrDoubleTremolo (
      inputLineNumber,
      doubleTremoloKind,
      doubleTremoloMarksNumber,
      doubleTremoloPlacementKind,
      voiceUplink);
  assert (o!=0);
  return o;
}

msrDoubleTremolo::msrDoubleTremolo (
  int                  inputLineNumber,
  msrDoubleTremoloKind doubleTremoloKind,
  int                  doubleTremoloMarksNumber,
  msrDoubleTremoloPlacementKind
                       doubleTremoloPlacementKind,
  S_msrVoice           voiceUplink)
    : msrElement (inputLineNumber)
{
  // sanity check
  msrAssert (
    voiceUplink != 0,
    "voiceUplink is null");
    
  // set the double tremolo voice uplink
  fDoubleTremoloVoiceUplink = voiceUplink;
  
  fDoubleTremoloKind          = doubleTremoloKind;
  fDoubleTremoloMarksNumber   = doubleTremoloMarksNumber;
  fDoubleTremoloPlacementKind = doubleTremoloPlacementKind;
  
  fDoubleTremoloSoundingWholeNotes =
    rational (-1, 1); // will be set later

  fDoubleTremoloNumberOfRepeats = -1; // will be set later
}

/* JMI
int msrDoubleTremolo::getDoubleTremoloNumberOfRepeats () const
{
  // fDoubleTremoloNumberOfRepeats ??? JMI
  
  // fetch the current part's number of divisions per quarter element
  int partDivisionsPerQuarterNote =
    fDoubleTremoloVoiceUplink->
      fetchVoicePartUplink ()->
        getPartDivisionsPerQuarterNote ();

  // fetch the number of divisions per double tremolo element
  rational
    divisionsPerDoubleTremoloElement =
      partDivisionsPerQuarterNote
        *
      4 // quarter note
        /
      fCurrentDoubleTremoloElementsLpsrDuration;

  if (divisionsPerDoubleTremoloElement <= 0) {
    stringstream s;

    s <<
      "divisionsPerDoubleTremoloElement = " <<
      divisionsPerDoubleTremoloElement <<
      " while it should be positive" <<
      endl <<
      tab << "partDivisionsPerQuarterNote = " << 
      partDivisionsPerQuarterNote <<
      endl <<
      tab << "doubleTremoloSoundingWholeNotes = " <<
      fDoubleTremoloSoundingWholeNotes <<
      endl <<
      tab << "fCurrentDoubleTremoloElementsLpsrDuration = " <<
      fCurrentDoubleTremoloElementsLpsrDuration;
    
    msrInternalError (
      elt->getInputLineNumber (),
      s.str ());
  }
    
  // the number of repeats is the quotient of the number of sounding divisions
  // by the duration of the elements
  int numberOfRepeats =
    fDoubleTremoloSoundingWholeNotes
      /
    (2 * divisionsPerDoubleTremoloElement); // to account for both elements

  return numberOfRepeats;
}
* */

S_msrDoubleTremolo msrDoubleTremolo::createDoubleTremoloNewbornClone (
  S_msrVoice containingVoice)
{
  if (gGeneralOptions->fTraceTremolos)
    gLogIos <<
      "==> Creating a newborn clone of a double tremolo" <<
      endl;
  
  // sanity check
  msrAssert(
    containingVoice != 0,
    "containingVoice is null");
    
  S_msrDoubleTremolo
    newbornClone =
      msrDoubleTremolo::create (
        fInputLineNumber,
        fDoubleTremoloKind,
        fDoubleTremoloMarksNumber,
        fDoubleTremoloPlacementKind,
        containingVoice);

  newbornClone->fDoubleTremoloSoundingWholeNotes =
    fDoubleTremoloSoundingWholeNotes;
        
  return newbornClone;
}

msrDoubleTremolo::~msrDoubleTremolo()
{}

void msrDoubleTremolo::setDoubleTremoloNoteFirstElement (S_msrNote note)
{
  if (gGeneralOptions->fTraceTremolos) {
    gLogIos <<
      "Setting note " << note->noteAsShortString () <<
      " as first element of double tremolo " <<
      doubleTremoloAsShortString () <<
      endl;
  }

  // register note as first element of this double tremolo
  fDoubleTremoloFirstElement = note;

  // mark it as being a double tremolo first element
  note->
    setNoteIsFirstNoteInADoubleTremolo ();

  // fetch note displayed whole notes
  rational
    noteDisplayWholeNotes =
      note->
        getNoteDisplayWholeNotes ();

  // set double tremolo displayed whole notes to the note's displayed whole notes
  if (fDoubleTremoloSoundingWholeNotes.getNumerator () != 0) {
    if (noteDisplayWholeNotes != fDoubleTremoloSoundingWholeNotes) { // JMI
      stringstream s;

      s <<
        "attempt to set double tremolo sounding whole notes both to " <<
        fDoubleTremoloSoundingWholeNotes << " (existing)" <<
        " and " <<
        noteDisplayWholeNotes <<
        " on note first element:" << " (note)" <<
        endl;

      idtr++;

      s <<
        note <<
        endl;
      
      idtr--;
        
      msrInternalError (
        note->getInputLineNumber (),
        s.str ());
    }
  }
  
  else {
    fDoubleTremoloSoundingWholeNotes =
      noteDisplayWholeNotes;
  }
}

void msrDoubleTremolo::setDoubleTremoloChordFirstElement (S_msrChord chord)
{
  if (gGeneralOptions->fTraceTremolos || gGeneralOptions->fTraceChords) {
    gLogIos <<
      "Setting chord " << chord->chordAsString () <<
      " as first element of double tremolo " <<
      doubleTremoloAsShortString () <<
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
    
  // set double tremolo whole notes to the chords's displayed whole notes
  if (fDoubleTremoloSoundingWholeNotes.getNumerator () != 0) {
    if (chordDisplayWholeNotes != fDoubleTremoloSoundingWholeNotes) { // JMI
      stringstream s;

      s <<
        "attempt to set double tremolo sounding whole notes both to " <<
        fDoubleTremoloSoundingWholeNotes << " (existing)" <<
        " and " <<
        chordDisplayWholeNotes << " (chord)" <<
        " on chord first element:" <<
        endl;

      idtr++;

      s <<
        chord <<
        endl;
      
      idtr--;
                
      msrInternalError (
        chord->getInputLineNumber (),
        s.str ());
    }
  }
  
  else {
    fDoubleTremoloSoundingWholeNotes =
      chordDisplayWholeNotes;
  }
}

void msrDoubleTremolo::setDoubleTremoloNoteSecondElement (S_msrNote note)
{
  if (gGeneralOptions->fTraceTremolos) {
    gLogIos <<
      "Setting note " << note->noteAsShortString () <<
      " as second element of double tremolo " <<
      doubleTremoloAsShortString () <<
      endl;
  }

  // register note as second element of this double tremolo
  fDoubleTremoloSecondElement = note;

  // mark it as being a double tremolo second element
  note->
    setNoteIsSecondNoteInADoubleTremolo ();

  // fetch note displayed whole notes
  rational
    noteDisplayWholeNotes =
      note->
        getNoteDisplayWholeNotes ();

  // set double tremolo whole notes to the note's displayed whole notes
  if (fDoubleTremoloSoundingWholeNotes.getNumerator () != 0) {
    if (noteDisplayWholeNotes != fDoubleTremoloSoundingWholeNotes) { // JMI
      stringstream s;

      s <<
        "attempt to set double tremolo whole notes both to " <<
        fDoubleTremoloSoundingWholeNotes << " (existing)" <<
        " and " <<
        noteDisplayWholeNotes <<
        " on note second element:" << " (note)" <<
        endl;

      idtr++;

      s <<
        note <<
        endl;
      
      idtr--;
        
      msrInternalError (
        note->getInputLineNumber (),
        s.str ());
    }
  }
  
  else {
    fDoubleTremoloSoundingWholeNotes =
      noteDisplayWholeNotes;
  }
}

void msrDoubleTremolo::setDoubleTremoloChordSecondElement (S_msrChord chord)
{
  if (gGeneralOptions->fTraceTremolos || gGeneralOptions->fTraceChords) {
    gLogIos <<
      "Setting chord " << chord->chordAsString () <<
      " as second element of double tremolo " <<
      doubleTremoloAsShortString () <<
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

      idtr++;

      s <<
        chord <<
        endl;
      
      idtr--;
        
      msrInternalError (
        chord->getInputLineNumber (),
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

string msrDoubleTremolo::doubleTremoloPlacementKindAsString (
  msrDoubleTremoloPlacementKind doubleTremoloPlacementKind)
{
  string result;
  
  switch (doubleTremoloPlacementKind) {
    case msrDoubleTremolo::k_NoDoubleTremoloPlacement:
      result = "none";
      break;
    case msrDoubleTremolo::kDoubleTremoloPlacementAbove:
      result = "above";
      break;
    case msrDoubleTremolo::kDoubleTremoloPlacementBelow:
      result = "below";
      break;
  } // switch

  return result;
}

string msrDoubleTremolo::doubleTremoloPlacementKindAsString () const
{
  return
    doubleTremoloPlacementKindAsString (
      fDoubleTremoloPlacementKind);
}

void msrDoubleTremolo::acceptIn (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors)
    gLogIos <<
      "% ==> msrDoubleTremolo::acceptIn()" <<
      endl;
      
  if (visitor<S_msrDoubleTremolo>*
    p =
      dynamic_cast<visitor<S_msrDoubleTremolo>*> (v)) {
        S_msrDoubleTremolo elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors)
          gLogIos <<
            "% ==> Launching msrDoubleTremolo::visitStart()" <<
             endl;
        p->visitStart (elem);
  }
}

void msrDoubleTremolo::acceptOut (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors)
    gLogIos <<
      "% ==> msrDoubleTremolo::acceptOut()" <<
      endl;

  if (visitor<S_msrDoubleTremolo>*
    p =
      dynamic_cast<visitor<S_msrDoubleTremolo>*> (v)) {
        S_msrDoubleTremolo elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors)
          gLogIos <<
            "% ==> Launching msrDoubleTremolo::visitEnd()" <<
            endl;
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

string msrDoubleTremolo::doubleTremoloAsShortString () const
{
  stringstream s;
  
  s <<
    "DoubleTremolo"<<
 //   ", " << msrDoubleTremoloKindAsString (fDoubleTremoloKind) <<
    ", line " << fInputLineNumber <<
    ", " <<
    singularOrPlural (
      fDoubleTremoloMarksNumber, "mark", "marks") <<
    ", placement" << " = " << doubleTremoloPlacementKindAsString ();

  return s.str ();
}

string msrDoubleTremolo::doubleTremoloAsString () const
{
  stringstream s;
  
  s <<
    "DoubleTremolo" " " <<
//    ", " << msrDoubleTremoloKindAsString (fDoubleTremoloKind) <<
    ", line " << fInputLineNumber <<
    fDoubleTremoloMarksNumber << " marks" <<
    ", placement" << " = " << doubleTremoloPlacementKindAsString () <<
    ", " << fDoubleTremoloSoundingWholeNotes << " sound whole notes";

  if (fDoubleTremoloFirstElement) // it may not yet be set
    s <<
      ", first element " << " = ";
      
    switch (fDoubleTremoloKind) {
      case msrDoubleTremolo::kNotesDoubleTremolo:
        if (
          S_msrNote
            note =
              dynamic_cast<msrNote*>(&(*fDoubleTremoloFirstElement))
          ) {    
          note->noteAsShortString ();
        }
        else {
          msrInternalError (
            fInputLineNumber,
            "notes double tremolo first element should be a note");
        }
        break;
        
      case msrDoubleTremolo::kChordsDoubleTremolo:
        if (
          S_msrChord
            chord =
              dynamic_cast<msrChord*>(&(*fDoubleTremoloFirstElement))
          ) {
          chord->chordAsString ();
        }
        else {
          msrInternalError (
            fInputLineNumber,
            "chords double tremolo first element should be a chord");
        }
        break;
    } // switch
      
  if (fDoubleTremoloSecondElement) // it may not yet be set
    s <<
      ", second element " << " = ";
      
    switch (fDoubleTremoloKind) {
      case msrDoubleTremolo::kNotesDoubleTremolo:
        if (
          S_msrNote
            note =
              dynamic_cast<msrNote*>(&(*fDoubleTremoloSecondElement))
          ) {    
          note->noteAsShortString ();
        }
        else {
          msrInternalError (
            fInputLineNumber,
            "notes double tremolo second element should be a note");
        }
        break;
        
      case msrDoubleTremolo::kChordsDoubleTremolo:
        if (
          S_msrChord
            chord =
              dynamic_cast<msrChord*>(&(*fDoubleTremoloSecondElement))
          ) {
          chord->chordAsString ();
        }
        else {
          msrInternalError (
            fInputLineNumber,
            "chords double tremolo second element should be a chord");
        }
        break;
    } // switch

  return s.str ();
}

void msrDoubleTremolo::print (ostream& os)
{
  os <<
    "DoubleTremolo" <<
    ", " << msrDoubleTremoloKindAsString (fDoubleTremoloKind) <<
    ", line " << fInputLineNumber <<
    ", " <<
    singularOrPlural (
      fDoubleTremoloMarksNumber, "mark", "marks") <<
    ", placement: " << doubleTremoloPlacementKindAsString () <<
    ", " << fDoubleTremoloSoundingWholeNotes << " sound whole notes" <<
    endl;

  idtr++;
  
  os <<
    "First element:";
  if (fDoubleTremoloFirstElement) { // it may not yet be set
    os <<
      endl;
      
    idtr++;
    os <<
      fDoubleTremoloFirstElement;
    idtr--;
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

    idtr++;
    os <<
      fDoubleTremoloSecondElement;
    idtr--;
  }
  else {
    os <<
      " none" <<
      endl;
  }

  idtr--;
}

//______________________________________________________________________________
S_msrRehearsal msrRehearsal::create (
  int              inputLineNumber,
  msrRehearsalKind rehearsalKind,
  string           rehearsalText)
{
  msrRehearsal* o =
    new msrRehearsal (
      inputLineNumber, rehearsalKind, rehearsalText);
  assert (o!=0);
  return o;
}

msrRehearsal::msrRehearsal (
  int              inputLineNumber,
  msrRehearsalKind rehearsalKind,
  string           rehearsalText)
    : msrElement (inputLineNumber)
{
  fRehearsalKind = rehearsalKind;

  fRehearsalText = rehearsalText;
}

msrRehearsal::~msrRehearsal()
{}

void msrRehearsal::acceptIn (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors)
    gLogIos <<
      "% ==> msrRehearsal::acceptIn()" <<
      endl;
      
  if (visitor<S_msrRehearsal>*
    p =
      dynamic_cast<visitor<S_msrRehearsal>*> (v)) {
        S_msrRehearsal elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors)
          gLogIos <<
            "% ==> Launching msrRehearsal::visitStart()" <<
             endl;
        p->visitStart (elem);
  }
}

void msrRehearsal::acceptOut (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors)
    gLogIos <<
      "% ==> msrRehearsal::acceptOut()" <<
      endl;

  if (visitor<S_msrRehearsal>*
    p =
      dynamic_cast<visitor<S_msrRehearsal>*> (v)) {
        S_msrRehearsal elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors)
          gLogIos <<
            "% ==> Launching msrRehearsal::visitEnd()" <<
            endl;
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
    case kNone:
      result = "none";
      break;
    case kRectangle:
      result = "rectangle";
      break;
    case kOval:
      result = "oval";
      break;
    case kCircle:
      result = "circle";
      break;
    case kBracket:
      result = "bracket";
      break;
    case kTriangle:
      result = "triangle";
      break;
    case kDiamond:
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
    endl;
}

//______________________________________________________________________________
S_msrDynamics msrDynamics::create (
  int                      inputLineNumber,
  msrDynamicsKind          dynamicsKind,
  msrDynamicsPlacementKind dynamicsPlacementKind)
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
  int                      inputLineNumber,
  msrDynamicsKind          dynamicsKind,
  msrDynamicsPlacementKind dynamicsPlacementKind)
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

string msrDynamics::dynamicsPlacementKindAsString (
  msrDynamicsPlacementKind dynamicsPlacementKind)
{
  string result;

  switch (dynamicsPlacementKind) {
    case msrDynamics::k_NoDynamicsPlacement:
      result = "none";
      break;
    case msrDynamics::kDynamicsPlacementAbove:
      result = "above";
      break;
    case msrDynamics::kDynamicsPlacementBelow:
      result = "below";
      break;
  } // switch

  return result;
}

string msrDynamics::dynamicsPlacementKindAsString () const
{
  return
    dynamicsPlacementKindAsString (
      fDynamicsPlacementKind);
}

void msrDynamics::acceptIn (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors)
    gLogIos <<
      "% ==> msrDynamics::acceptIn()" <<
      endl;
      
  if (visitor<S_msrDynamics>*
    p =
      dynamic_cast<visitor<S_msrDynamics>*> (v)) {
        S_msrDynamics elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors)
          gLogIos <<
            "% ==> Launching msrDynamics::visitStart()" <<
             endl;
        p->visitStart (elem);
  }
}

void msrDynamics::acceptOut (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors)
    gLogIos <<
      "% ==> msrDynamics::acceptOut()" <<
      endl;

  if (visitor<S_msrDynamics>*
    p =
      dynamic_cast<visitor<S_msrDynamics>*> (v)) {
        S_msrDynamics elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors)
          gLogIos <<
            "% ==> Launching msrDynamics::visitEnd()" <<
            endl;
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

void msrOtherDynamics::acceptIn (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors)
    gLogIos <<
      "% ==> msrOtherDynamics::acceptIn()" <<
      endl;
      
  if (visitor<S_msrOtherDynamics>*
    p =
      dynamic_cast<visitor<S_msrOtherDynamics>*> (v)) {
        S_msrOtherDynamics elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors)
          gLogIos <<
            "% ==> Launching msrOtherDynamics::visitStart()" <<
             endl;
        p->visitStart (elem);
  }
}

void msrOtherDynamics::acceptOut (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors)
    gLogIos <<
      "% ==> msrOtherDynamics::acceptOut()" <<
      endl;

  if (visitor<S_msrOtherDynamics>*
    p =
      dynamic_cast<visitor<S_msrOtherDynamics>*> (v)) {
        S_msrOtherDynamics elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors)
          gLogIos <<
            "% ==> Launching msrOtherDynamics::visitEnd()" <<
            endl;
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

string msrOtherDynamics::otherDynamicsAsString ()
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
    otherDynamicsAsString () <<
    endl;
}

//______________________________________________________________________________
S_msrWedge msrWedge::create (
  int           inputLineNumber,
  msrWedgeKind  wedgeKind)
{
  msrWedge* o =
    new msrWedge (
      inputLineNumber, wedgeKind);
  assert(o!=0);
  return o;
}

msrWedge::msrWedge (
  int           inputLineNumber,
  msrWedgeKind  wedgeKind)
    : msrElement (inputLineNumber)
{
  fWedgeKind = wedgeKind; 
}

msrWedge::~msrWedge()
{}

string msrWedge::wedgeKindAsString ()
{
  stringstream s;
  
  switch (fWedgeKind) {
    case msrWedge::kCrescendoWedge:
      s << "crescendo";
      break;
    case msrWedge::kDecrescendoWedge:
      s << "decrescendo";
      break;
    case msrWedge::kStopWedge:
      s << "stop";
      break;
  } // switch
    
  return s.str ();
}

void msrWedge::acceptIn (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors)
    gLogIos <<
      "% ==> msrWedge::acceptIn()" <<
      endl;
      
  if (visitor<S_msrWedge>*
    p =
      dynamic_cast<visitor<S_msrWedge>*> (v)) {
        S_msrWedge elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors)
          gLogIos <<
            "% ==> Launching msrWedge::visitStart()" <<
             endl;
        p->visitStart (elem);
  }
}

void msrWedge::acceptOut (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors)
    gLogIos <<
      "% ==> msrWedge::acceptOut()" <<
      endl;

  if (visitor<S_msrWedge>*
    p =
      dynamic_cast<visitor<S_msrWedge>*> (v)) {
        S_msrWedge elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors)
          gLogIos <<
            "% ==> Launching msrWedge::visitEnd()" <<
            endl;
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
    "Wedge" << " " << wedgeKindAsString () <<
    ", line " << fInputLineNumber <<
    endl;
}

//______________________________________________________________________________
S_msrTie msrTie::create (
  int           inputLineNumber,
  msrTieKind    tieKind)
{
  msrTie* o =
    new msrTie (
      inputLineNumber, tieKind);
  assert(o!=0);
  return o;
}

msrTie::msrTie (
  int           inputLineNumber,
  msrTieKind    tieKind)
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
    case kStartTie:
      s << "start";
      break;
    case kContinueTie:
      s << "continue";
      break;
    case kStopTie:
      s << "stop";
      break;
    default:
      s << "Tie" << tieKind << "???";
  } // switch
    
  return s.str ();
}

void msrTie::acceptIn (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors)
    gLogIos <<
      "% ==> msrTie::acceptIn()" <<
      endl;
      
  if (visitor<S_msrTie>*
    p =
      dynamic_cast<visitor<S_msrTie>*> (v)) {
        S_msrTie elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors)
          gLogIos <<
            "% ==> Launching msrTie::visitStart()" <<
             endl;
        p->visitStart (elem);
  }
}

void msrTie::acceptOut (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors)
    gLogIos <<
      "% ==> msrTie::acceptOut()" <<
      endl;

  if (visitor<S_msrTie>*
    p =
      dynamic_cast<visitor<S_msrTie>*> (v)) {
        S_msrTie elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors)
          gLogIos <<
            "% ==> Launching msrTie::visitEnd()" <<
            endl;
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
    ", line " << fInputLineNumber;
}

//______________________________________________________________________________
S_msrSlur msrSlur::create (
  int           inputLineNumber,
  int           slurNumber,
  msrSlurKind   slurKind)
{
  msrSlur* o =
    new msrSlur (
      inputLineNumber, slurNumber, slurKind);
  assert(o!=0);
  return o;
}

msrSlur::msrSlur (
  int           inputLineNumber,
  int           slurNumber,
  msrSlurKind   slurKind)
    : msrElement (inputLineNumber)
{
  fSlurNumber = slurNumber;
  fSlurKind   = slurKind; 
}

msrSlur::~msrSlur()
{}

void msrSlur::acceptIn (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors)
    gLogIos <<
      "% ==> msrSlur::acceptIn()" <<
      endl;
      
  if (visitor<S_msrSlur>*
    p =
      dynamic_cast<visitor<S_msrSlur>*> (v)) {
        S_msrSlur elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors)
          gLogIos <<
            "% ==> Launching msrSlur::visitStart()" <<
             endl;
        p->visitStart (elem);
  }
}

void msrSlur::acceptOut (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors)
    gLogIos <<
      "% ==> msrSlur::acceptOut()" <<
      endl;

  if (visitor<S_msrSlur>*
    p =
      dynamic_cast<visitor<S_msrSlur>*> (v)) {
        S_msrSlur elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors)
          gLogIos <<
            "% ==> Launching msrSlur::visitEnd()" <<
            endl;
        p->visitEnd (elem);
  }
}


void msrSlur::browseData (basevisitor* v)
{}

string msrSlur::slurKindAsString (
  msrSlurKind slurKind)
{
  stringstream s;
  
  switch (slurKind) {
    case msrSlur::kStartSlur:
      s << "start";
      break;
    case msrSlur::kContinueSlur:
      s << "continue";
      break;
    case msrSlur::kStopSlur:
      s << "stop";
      break;
    default:
      s << "Slur" << slurKind << "???";
  } // switch
    
  return s.str ();
  
}
      
string msrSlur::slurKindAsString ()
{
  return slurKindAsString (fSlurKind);
}

string msrSlur::slurAsString ()
{
  stringstream s;

  s <<
   "Slur" " " << slurKindAsString () <<
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
    slurAsString () <<
     endl;
}

//______________________________________________________________________________
S_msrLigature msrLigature::create (
  int             inputLineNumber,
  int             ligatureNumber,
  msrLigatureKind ligatureKind)
{
  msrLigature* o =
    new msrLigature (
      inputLineNumber, ligatureNumber, ligatureKind);
  assert(o!=0);
  return o;
}

msrLigature::msrLigature (
  int             inputLineNumber,
  int             ligatureNumber,
  msrLigatureKind ligatureKind)
    : msrElement (inputLineNumber)
{
  fLigatureNumber = ligatureNumber;
  fLigatureKind   = ligatureKind; 
}

msrLigature::~msrLigature()
{}

string msrLigature::ligatureKindAsString (
  msrLigatureKind ligatureKind)
{
  stringstream s;
  
  switch (ligatureKind) {
    case msrLigature::kStartLigature:
      s << "start";
      break;
    case msrLigature::kContinueLigature:
      s << "continue";
      break;
    case msrLigature::kStopLigature:
      s << "stop";
      break;
    default:
      s << "Ligature" << ligatureKind << "???";
  } // switch
    
  return s.str ();
  
}
      
string msrLigature::ligatureKindAsString ()
{
  return ligatureKindAsString (fLigatureKind);
}

void msrLigature::acceptIn (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors)
    gLogIos <<
      "% ==> msrLigature::acceptIn()" <<
      endl;
      
  if (visitor<S_msrLigature>*
    p =
      dynamic_cast<visitor<S_msrLigature>*> (v)) {
        S_msrLigature elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors)
          gLogIos <<
            "% ==> Launching msrLigature::visitStart()" <<
             endl;
        p->visitStart (elem);
  }
}

void msrLigature::acceptOut (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors)
    gLogIos <<
      "% ==> msrLigature::acceptOut()" <<
      endl;

  if (visitor<S_msrLigature>*
    p =
      dynamic_cast<visitor<S_msrLigature>*> (v)) {
        S_msrLigature elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors)
          gLogIos <<
            "% ==> Launching msrLigature::visitEnd()" <<
            endl;
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
    ", line " << fInputLineNumber;
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
    graceNotesVoiceUplink != 0,
    "graceNotesVoiceUplink is null");

  fGraceNotesVoiceUplink =
    graceNotesVoiceUplink;    
    
  fGraceNotesIsSlashed = gracenoteIsSlashed;
}

msrGraceNotes::~msrGraceNotes()
{}

S_msrGraceNotes msrGraceNotes::createGraceNotesNewbornClone (
  S_msrVoice containingVoice)
{
  if (gGeneralOptions->fTraceGraceNotes) {
    gLogIos <<
      "==> Creating a newborn clone of grace notes" <<
      graceNotesAsShortString () <<
      endl;
  }
  
  // sanity check
  msrAssert(
    containingVoice != 0,
    "containingVoice is null");
    
  S_msrGraceNotes
    newbornClone =
      msrGraceNotes::create (
        fInputLineNumber,
        fGraceNotesIsSlashed,
        containingVoice);

  newbornClone->fGraceNotesIsSlashed =
    fGraceNotesIsSlashed;
    
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
  if (gGeneralOptions->fTraceGraceNotes) {
    gLogIos <<
      "==> Creating a skip clone of grace notes" <<
      graceNotesAsShortString () <<
      endl;
  }
  
  S_msrGraceNotes
    clone =
      msrGraceNotes::create (
        fInputLineNumber,
        fGraceNotesIsSlashed,
        containingVoice);

  clone->fGraceNotesIsSlashed =
    fGraceNotesIsSlashed;

  // populating the clone with skips
  for (
    list<S_msrNote>::const_iterator i=fGraceNotesNotesList.begin();
    i!=fGraceNotesNotesList.end();
    i++) {
    S_msrNote note = (*i);
    
    // create skip with same duration as note
    S_msrNote
      skip =
        msrNote::createSkipNote (
          note->            getInputLineNumber (),
          note->            getNoteDisplayWholeNotes (), // would be 0/1 otherwise JMI
          note->            getNoteDisplayWholeNotes (),
          note->            getNoteDotsNumber (),
          containingVoice-> getVoiceStaffRelativeNumber (), // JMI
          containingVoice-> getVoicePartRelativeID ());

    clone->
      appendNoteToGraceNotes (skip);
  } // for
    
  return clone;
}

void msrGraceNotes::appendNoteToGraceNotes (S_msrNote note)
{
  fGraceNotesNotesList.push_back (note);
}

void msrGraceNotes::acceptIn (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors)
    gLogIos <<
      "% ==> msrGraceNotes::acceptIn()" <<
      endl;
      
  if (visitor<S_msrGraceNotes>*
    p =
      dynamic_cast<visitor<S_msrGraceNotes>*> (v)) {
        S_msrGraceNotes elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors)
          gLogIos <<
            "% ==> Launching msrGraceNotes::visitStart()" <<
             endl;
        p->visitStart (elem);
  }
}

void msrGraceNotes::acceptOut (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors)
    gLogIos <<
      "% ==> msrGraceNotes::acceptOut()" <<
      endl;

  if (visitor<S_msrGraceNotes>*
    p =
      dynamic_cast<visitor<S_msrGraceNotes>*> (v)) {
        S_msrGraceNotes elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors)
          gLogIos <<
            "% ==> Launching msrGraceNotes::visitEnd()" <<
            endl;
        p->visitEnd (elem);
  }
}

void msrGraceNotes::browseData (basevisitor* v)
{
  list<S_msrNote>::const_iterator i;

  for (
    i=fGraceNotesNotesList.begin();
    i!=fGraceNotesNotesList.end();
    i++) {
    // browse the note
    msrBrowser<msrNote> browser (v);
    browser.browse (*(*i));
  } // for
}

ostream& operator<< (ostream& os, const S_msrGraceNotes& elt)
{
  elt->print (os);
  return os;
}

string msrGraceNotes::graceNotesAsShortString () const
{
  stringstream s;

  s <<
    "GraceNotes" " ";

  list<S_msrNote>::const_iterator
    iBegin = fGraceNotesNotesList.begin(),
    iEnd   = fGraceNotesNotesList.end(),
    i      = iBegin;
  for ( ; ; ) {
    s << (*i)->noteAsShortString ();
    if (++i == iEnd) break;
    s << " ";
  } // for

  return s.str ();
}

void msrGraceNotes::print (ostream& os)
{
  os <<
    "GraceNotes" <<
    ", line " << fInputLineNumber <<
    ", " <<
    singularOrPlural (
      fGraceNotesNotesList.size (), "note", "notes") <<
    ", slashed: " <<
    booleanAsString (fGraceNotesIsSlashed) <<
    endl;
  
  idtr++;
            
  list<S_msrNote>::const_iterator
    iBegin = fGraceNotesNotesList.begin(),
    iEnd   = fGraceNotesNotesList.end(),
    i      = iBegin;
    
  for ( ; ; ) {
    os << (*i);
    if (++i == iEnd) break;
    os << endl;
  } // for

  idtr--;
}

//______________________________________________________________________________
S_msrAfterGraceNotes msrAfterGraceNotes::create (
  int        inputLineNumber,
  S_msrNote  afterGraceNotesNote,
  bool       aftergracenoteIsSlashed,
  S_msrVoice afterGraceNotesVoiceUplink)
{
  msrAfterGraceNotes* o =
    new msrAfterGraceNotes (
      inputLineNumber,
      afterGraceNotesNote,
      aftergracenoteIsSlashed,
      afterGraceNotesVoiceUplink);
  assert(o!=0);

  return o;
}

msrAfterGraceNotes::msrAfterGraceNotes (
  int        inputLineNumber,
  S_msrNote  afterGraceNotesNote,
  bool       aftergracenoteIsSlashed,
  S_msrVoice afterGraceNotesVoiceUplink)
    : msrElement (inputLineNumber)
{
  // sanity check
  msrAssert(
    afterGraceNotesVoiceUplink != 0,
    "afterGraceNotesVoiceUplink is null");
  
  // set gracenote's voice uplink
  fAfterGraceNotesVoiceUplink =
    afterGraceNotesVoiceUplink;

  // sanity check
  msrAssert(
    afterGraceNotesVoiceUplink != 0,
    "afterGraceNotesVoiceUplink is null");

  // set gracenote's note uplink
  fAfterGraceNotesNote =
    afterGraceNotesNote;
    
  fAfterGraceNotesIsSlashed =
    aftergracenoteIsSlashed;
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
  if (gGeneralOptions->fTraceGraceNotes) {
    gLogIos <<
      "==> Creating a newborn clone of after grace notes" <<
      endl;
  }

  // sanity check
  msrAssert(
    noteClone != 0,
    "noteClone is null");
    
  // sanity check
  msrAssert(
    containingVoice != 0,
    "containingVoice is null");
    
  /* JMI ???
  // set after grace note's part uplink
  o->fAfterGraceNotesPartUplink =
    afterGraceNotesPartUplink;
    */
    
  S_msrAfterGraceNotes
    newbornClone =
      msrAfterGraceNotes::create (
        fInputLineNumber,
        noteClone,
        fAfterGraceNotesIsSlashed,
        containingVoice);
  
  newbornClone->fAfterGraceNotesIsSlashed =
    fAfterGraceNotesIsSlashed;
  
  return newbornClone;
}

void msrAfterGraceNotes::appendNoteToAfterGraceNotes (S_msrNote note)
{
  fAfterGraceNotesNotesList.push_back (note);
}

void msrAfterGraceNotes::acceptIn (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors)
    gLogIos <<
      "% ==> msrAfterGraceNotes::acceptIn()" <<
      endl;
      
  if (visitor<S_msrAfterGraceNotes>*
    p =
      dynamic_cast<visitor<S_msrAfterGraceNotes>*> (v)) {
        S_msrAfterGraceNotes elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors)
          gLogIos <<
            "% ==> Launching msrAfterGraceNotes::visitStart()" <<
             endl;
        p->visitStart (elem);
  }
}

void msrAfterGraceNotes::acceptOut (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors)
    gLogIos <<
      "% ==> msrAfterGraceNotes::acceptOut()" <<
      endl;

  if (visitor<S_msrAfterGraceNotes>*
    p =
      dynamic_cast<visitor<S_msrAfterGraceNotes>*> (v)) {
        S_msrAfterGraceNotes elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors)
          gLogIos <<
            "% ==> Launching msrAfterGraceNotes::visitEnd()" <<
            endl;
        p->visitEnd (elem);
  }
}

void msrAfterGraceNotes::browseData (basevisitor* v)
{
  list<S_msrNote>::const_iterator i;

  // browse the afterGraceNotes note
  msrBrowser<msrNote> browser (v);
  browser.browse (*fAfterGraceNotesNote);

  for (
    i=fAfterGraceNotesNotesList.begin();
    i!=fAfterGraceNotesNotesList.end();
    i++) {
    // browse the note
    msrBrowser<msrNote> browser (v);
    browser.browse (*(*i));
  } // for
}

ostream& operator<< (ostream& os, const S_msrAfterGraceNotes& elt)
{
  elt->print (os);
  return os;
}

string msrAfterGraceNotes::afterGraceNotesAsShortString () const
{
  stringstream s;

  s <<
    "AfterGraceNotes" " ";

  list<S_msrNote>::const_iterator
    iBegin = fAfterGraceNotesNotesList.begin(),
    iEnd   = fAfterGraceNotesNotesList.end(),
    i      = iBegin;
    
  for ( ; ; ) {
    s << (*i)->noteAsShortString ();
    if (++i == iEnd) break;
    s << " ";
  } // for

  return s.str ();
}

void msrAfterGraceNotes::print (ostream& os)
{
  os <<
    "AfterGraceNotes" <<
    ", line " << fInputLineNumber <<
    ", " <<
    singularOrPlural (
      fAfterGraceNotesNotesList.size (), "note", "notes") <<
    ", slashed: " <<
    booleanAsString (fAfterGraceNotesIsSlashed) <<
    endl;
  
  idtr++;

  // print the afterGraceNotes note
  os <<
    "Note:" <<
    endl;
  idtr++;
  os <<
    fAfterGraceNotesNote;
  idtr--;

  // print the afterGraceNotes contents
  os <<
    "Contents:" <<
    endl;

  idtr++;

  list<S_msrNote>::const_iterator
    iBegin = fAfterGraceNotesNotesList.begin(),
    iEnd   = fAfterGraceNotesNotesList.end(),
    i      = iBegin;
    
  for ( ; ; ) {
    os << (*i);
    if (++i == iEnd) break;
 // JMI   os << endl;
  } // for

  idtr--;
      
  idtr--;
}

//______________________________________________________________________________
S_msrNote msrNote::create (
  int                        inputLineNumber,
  
  msrNoteKind                noteKind,

  msrQuarterTonesPitch       noteQuarterTonesPitch,
  
  rational                   noteSoundingWholeNotes,
  rational                   noteDisplayWholeNotes,
  
  int                        noteDotsNumber,
  
  msrDuration                noteGraphicDuration,
  
  int                        noteOctave,
  
  msrQuarterTonesPitch       noteQuarterTonesDisplayPitch,
  int                        noteDisplayOctave,
  
  bool                       noteIsARest,
  bool                       noteIsUnpitched,

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
      
      noteQuarterTonesPitch,
      
      noteSoundingWholeNotes,
      noteDisplayWholeNotes,
      
      noteDotsNumber,
      
      noteGraphicDuration,
      
      noteOctave,

      noteQuarterTonesDisplayPitch,
      noteDisplayOctave,
      
      noteIsARest,
      noteIsUnpitched,
      
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

  msrQuarterTonesPitch       noteQuarterTonesPitch,
  
  rational                   noteSoundingWholeNotes,
  rational                   noteDisplayWholeNotes,
  
  int                        noteDotsNumber,
  
  msrDuration                noteGraphicDuration,
  
  int                        noteOctave,
  
  msrQuarterTonesPitch       noteQuarterTonesDisplayPitch,
  int                        noteDisplayOctave,
  
  bool                       noteIsARest,
  bool                       noteIsUnpitched,

  bool                       noteIsAGraceNote,

  msrNotePrintKind           notePrintKind,
  
  msrNoteHeadKind            noteHeadKind,
  msrNoteHeadFilledKind      noteHeadFilledKind,
  msrNoteHeadParenthesesKind noteHeadParenthesesKind)
  : msrElement (inputLineNumber)
{
  // basic note description
  fNoteKind = noteKind;

  fNoteQuarterTonesPitch  = noteQuarterTonesPitch;
  
  fNoteSoundingWholeNotes = noteSoundingWholeNotes;
  fNoteDisplayWholeNotes  = noteDisplayWholeNotes;
  
  fNoteDotsNumber         = noteDotsNumber;
  
  fNoteGraphicDuration    = noteGraphicDuration;
  
  fNoteOctave             = noteOctave;

  fNoteQuarterTonesDisplayPitch = noteQuarterTonesDisplayPitch;
  fNoteDisplayOctave            = noteDisplayOctave;

  fNoteIsARest     = noteIsARest;
  fNoteIsUnpitched = noteIsUnpitched;

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
    fNoteQuarterTonesPitch = fNoteQuarterTonesDisplayPitch;
    fNoteOctave = fNoteDisplayOctave;
  }

  // note accidentals
  // ------------------------------------------------------

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

  fNoteSyllableExtendKind =
    msrSyllable::k_NoSyllableExtend;
  
  if (gGeneralOptions->fTraceNotesDetails) {
    gLogIos <<
      endl <<
      "==> Initializing a note" <<
      ", kind: ";
    if (fNoteKind == k_NoNoteKind)
      gLogIos <<
        "not yet known";
    else
      gLogIos <<
        noteKindAsString (fNoteKind);
    gLogIos <<
      ", line " << fInputLineNumber << ":" <<
      endl;

    idtr++;
    
    const int fieldWidth = 30;
    
    gLogIos <<
      left <<
        setw (fieldWidth) <<
        "fNoteQuarterTonesPitch" << " = " <<
        msrQuarterTonesPitchAsString (
          gMsrOptions->fMsrQuarterTonesPitchesLanguage,
          fNoteQuarterTonesPitch) <<
        endl;

    gLogIos <<
      left <<
        setw (fieldWidth) <<
        "fNoteSoundingWholeNotes" << " = " <<
        fNoteSoundingWholeNotes <<
        endl;
      
    gLogIos <<
      left <<
        setw (fieldWidth) <<
        "fNoteDisplayWholeNotes" << " = " <<
        fNoteDisplayWholeNotes <<
        endl;
      
    gLogIos <<
      left <<
        setw (fieldWidth) <<
        "fNoteDotsNumber" << " = " <<
        fNoteDotsNumber <<
        endl <<

      left <<
        setw (fieldWidth) <<
        "fNoteGraphicDuration" << " = ";
      if (fNoteGraphicDuration != k_NoDuration)
        gLogIos <<
          msrDurationAsString (
            fNoteGraphicDuration);
      else
        gLogIos << "none";

    gLogIos <<
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
        "fVoiceNumber" << " = " <<
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
          
      left <<
        setw (fieldWidth) <<
        "fNoteSyllableExtendKind" << " = " <<
        msrSyllable::syllableExtendKindAsString (
          fNoteSyllableExtendKind) <<
        endl <<
      endl;
          
    idtr--;

/*
    gLogIos <<
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

  fNoteIsFirstNoteInADoubleTremolo  = false;
  fNoteIsSecondNoteInADoubleTremolo = false;
  
  fNoteHasATrill = false;
  fNoteIsFollowedByGraceNotes = false;
  
  fNoteHasADelayedOrnament = false;
}

msrNote::~msrNote()
{}

S_msrNote msrNote::createNoteNewbornClone (
  S_msrPart containingPart)
{
  if (gGeneralOptions->fTraceNotes) {
    gLogIos <<
      "==> Creating a newborn clone of note " <<
      noteAsString () <<
      " in part " <<
      containingPart->
        getPartCombinedName () << 
      endl;
  }

  // sanity check
  msrAssert(
    containingPart != 0,
    "containingPart is null");
    
  S_msrNote
    newbornClone =
      msrNote::create (
        fInputLineNumber,
        
        fNoteKind,
        
        fNoteQuarterTonesPitch,
        
        fNoteSoundingWholeNotes,
        fNoteDisplayWholeNotes,
        
        fNoteDotsNumber,
        
        fNoteGraphicDuration,
        
        fNoteOctave,
        
        fNoteQuarterTonesDisplayPitch,
        fNoteDisplayOctave,
        
        fNoteIsARest,
        fNoteIsUnpitched,
        
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

  newbornClone->
    fNoteSyllableExtendKind =
      fNoteSyllableExtendKind;

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

  newbornClone->fNoteIsFirstNoteInADoubleTremolo =
    fNoteIsFirstNoteInADoubleTremolo;
  newbornClone->fNoteIsSecondNoteInADoubleTremolo =
    fNoteIsSecondNoteInADoubleTremolo;

  newbornClone->fNoteHasATrill =
    fNoteHasATrill;
  newbornClone->fNoteIsFollowedByGraceNotes =
    fNoteIsFollowedByGraceNotes;

  newbornClone->fNoteHasADelayedOrnament =
    fNoteHasADelayedOrnament;

  // uplinks
  // ------------------------------------------------------

  /* JMI
    S_msrTuplet           fNoteTupletUplink;

    S_msrMeasure          fNoteMeasureUplink;
*/

  return newbornClone;
}

S_msrNote msrNote::createNoteDeepCopy (
  S_msrPart containingPart)
{
  if (gGeneralOptions->fTraceNotes) {
    gLogIos <<
      "==> Creating a deep copy of note " <<
      noteAsString () <<
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
    containingPart != 0,
    "containingPart is null");
    */
    
  S_msrNote
    noteDeepCopy =
      msrNote::create (
        fInputLineNumber,
        
        fNoteKind,
        
        fNoteQuarterTonesPitch,
        
        fNoteSoundingWholeNotes,
        fNoteDisplayWholeNotes,
        
        fNoteDotsNumber,
        
        fNoteGraphicDuration,
        
        fNoteOctave,
        
        fNoteQuarterTonesDisplayPitch,
        fNoteDisplayOctave,
        
        fNoteIsARest,
        fNoteIsUnpitched,
        
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

  {
    list<S_msrSyllable>::const_iterator i;
    for (i=fNoteSyllables.begin(); i!=fNoteSyllables.end(); i++) {
      // share this data
      noteDeepCopy->
        fNoteSyllables.push_back ((*i));
    } // for
  }
  
  noteDeepCopy->
    fNoteSyllableExtendKind =
      fNoteSyllableExtendKind;

  // stem
  // ------------------------------------------------------

  noteDeepCopy->fNoteStem = // JMI
    // share this data
    fNoteStem;

  // beams
  // ------------------------------------------------------

  {
    list<S_msrBeam>::const_iterator i;
    for (i=fNoteBeams.begin(); i!=fNoteBeams.end(); i++) {
      // share this data
      noteDeepCopy->
        fNoteBeams.push_back ((*i));
    } // for
  }
  
  // articulations
  // ------------------------------------------------------

  for (
    list<S_msrArticulation>::const_iterator i=
      fNoteArticulations.begin();
      i!=fNoteArticulations.end();
      i++) {
    // share this data
    noteDeepCopy->
      fNoteArticulations.push_back ((*i));
  } // for

  // technicals
  // ------------------------------------------------------

  {
    list<S_msrTechnical>::const_iterator i;
    for (
      i=fNoteTechnicals.begin();
      i!=fNoteTechnicals.end();
      i++) {
      // share this data
      noteDeepCopy->
        fNoteTechnicals.push_back ((*i));
    } // for
  }
  
  {
    list<S_msrTechnicalWithInteger>::const_iterator i;
    for (
      i=fNoteTechnicalWithIntegers.begin();
      i!=fNoteTechnicalWithIntegers.end();
      i++) {
      // share this data
      noteDeepCopy->
        fNoteTechnicalWithIntegers.push_back ((*i));
    } // for
  }
  
  {
    list<S_msrTechnicalWithString>::const_iterator i;
    for (
      i=fNoteTechnicalWithStrings.begin();
      i!=fNoteTechnicalWithStrings.end();
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
    for (i=fNoteOrnaments.begin(); i!=fNoteOrnaments.end(); i++) {
      // share this data
      noteDeepCopy->
        fNoteOrnaments.push_back ((*i));
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
    for (i=fNoteDynamics.begin(); i!=fNoteDynamics.end(); i++) {
      // share this data
      noteDeepCopy->
        fNoteDynamics.push_back ((*i));
    } // for
  }

  {
    list<S_msrOtherDynamics>::const_iterator i;
    for (i=fNoteOtherDynamics.begin(); i!=fNoteOtherDynamics.end(); i++) {
      // share this data
      noteDeepCopy->
        fNoteOtherDynamics.push_back ((*i));
    } // for
  }
  
  {
    list<S_msrWedge>::const_iterator i;
    for (i=fNoteWedges.begin(); i!=fNoteWedges.end(); i++) {
      // share this data
      noteDeepCopy->
        fNoteWedges.push_back ((*i));
    } // for
  }
  
  // words
  // ------------------------------------------------------

  {
    list<S_msrWords>::const_iterator i;
    for (i=fNoteWords.begin(); i!=fNoteWords.end(); i++) {
      // share this data
      noteDeepCopy->
        fNoteWords.push_back ((*i));
    } // for
  }
  
  // slurs
  // ------------------------------------------------------

  {
    list<S_msrSlur>::const_iterator i;
    for (i=fNoteSlurs.begin(); i!=fNoteSlurs.end(); i++) {
      // share this data
      noteDeepCopy->
        fNoteSlurs.push_back ((*i));
    } // for
  }
  
  // ligatures
  // ------------------------------------------------------

  { 
    list<S_msrLigature>::const_iterator i;
    for (i=fNoteLigatures.begin(); i!=fNoteLigatures.end(); i++) {
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
          containingPart);
  }

  // figured bass
  // ------------------------------------------------------

  if (fNoteFiguredBass) {
    noteDeepCopy->fNoteFiguredBass =
      fNoteFiguredBass->
        createFiguredBassDeepCopy (
          containingPart);
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

  noteDeepCopy->fNoteIsFirstNoteInADoubleTremolo =
    fNoteIsFirstNoteInADoubleTremolo;
  noteDeepCopy->fNoteIsSecondNoteInADoubleTremolo =
    fNoteIsSecondNoteInADoubleTremolo;

  noteDeepCopy->fNoteHasATrill =
    fNoteHasATrill;
  noteDeepCopy->fNoteIsFollowedByGraceNotes =
    fNoteIsFollowedByGraceNotes;

  noteDeepCopy->fNoteHasADelayedOrnament =
    fNoteHasADelayedOrnament;

  // uplinks
  // ------------------------------------------------------

  /* JMI
  
    S_msrTuplet           fNoteTupletUplink;

    S_msrMeasure          fNoteMeasureUplink;
*/

  return noteDeepCopy;
}

S_msrNote msrNote::createSkipNote (
  int       inputLineNumber,
  rational  soundingWholeNotes,
  rational  displayWholeNotes,
  int       dotsNumber,
  int       staffNumber,
  int       voicePartRelativeID)
{    
  msrNote * o =
    new msrNote (
      inputLineNumber,
      
      kSkipNote, // noteKind
      
      k_NoQuarterTonesPitch,
      
      soundingWholeNotes,
      displayWholeNotes,
      
      dotsNumber,
      
      k_NoDuration, // noteGraphicDuration
      
      K_NO_OCTAVE, // noteOctave,
      
      k_NoQuarterTonesPitch, // noteDisplayQuarterTonesPitch
      K_NO_OCTAVE, // noteDisplayOctave,
      
      false, // noteIsARest
      false, // noteIsUnpitched
      
      false, // noteIsAGraceNote

      kNotePrintYes, // JMI

      kNoteHeadNormal, // JMI
      kNoteHeadFilledYes, // JMI
      kNoteHeadParenthesesNo); // JMI
  assert(o!=0);
  
  return o;
}    

string msrNote::noteSoundingWholeNotesAsMsrString ()
{
  string result;

  if (fNoteSoundingWholeNotes.getNumerator () == 0)
    result = "(no sounding whole notes)";
  else
    result =
      wholeNotesAsMsrString (
        fInputLineNumber,
        fNoteSoundingWholeNotes);

  return result;
  }

string msrNote::noteDisplayWholeNotesAsMsrString ()
{
  string result;

  if (fNoteDisplayWholeNotes.getNumerator () == 0)
    result = "(no display whole notes)";
  else
    result =
      wholeNotesAsMsrString (
        fInputLineNumber,
        fNoteDisplayWholeNotes);

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
      
    case msrNote::kChordMemberNote:
      result = "chord member";
      break;
      
    case msrNote::kTupletMemberNote:
      result = "tuplet member";
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
      result = "editorial accidental: yes";
      break;
    case msrNote::kNoteEditorialAccidentalNo:
      result = "editorial accidental: no";
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
      result = "cautionary accidental: yes";
      break;
    case msrNote::kNoteCautionaryAccidentalNo:
      result = "cautionary accidental: no";
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
      result = "print: yes";
      break;
    case msrNote::kNotePrintNo:
      result = "print: no";
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
      result = "noteHead: Slash";
      break;
    case msrNote::kNoteHeadTriangle:
      result = "noteHead: triangle";
      break;
    case msrNote::kNoteHeadDiamond:
      result = "noteHead: diamond";
      break;
    case msrNote::kNoteHeadSquare:
      result = "noteHead: square";
      break;
    case msrNote::kNoteHeadCross:
      result = "noteHead: cross";
      break;
    case msrNote::kNoteHeadX:
      result = "noteHead: X";
      break;
    case msrNote::kNoteHeadCircleX:
      result = "noteHead: circleX";
      break;
    case msrNote::kNoteHeadInvertedTriangle:
      result = "noteHead: invertedTriangle";
      break;
    case msrNote::kNoteHeadArrowDown:
      result = "noteHead: arrowDown";
      break;
    case msrNote::kNoteHeadArrowUp:
      result = "noteHead: arrowUp";
      break;
    case msrNote::kNoteHeadSlashed:
      result = "noteHead: slashed";
      break;
    case msrNote::kNoteHeadBackSlashed:
      result = "noteHead: backSlashed";
      break;
    case msrNote::kNoteHeadNormal:
      result = "noteHead: normal";
      break;
    case msrNote::kNoteHeadCluster:
      result = "noteHead: cluster";
      break;
    case msrNote::kNoteHeadCircleDot:
      result = "noteHead: circleDot";
      break;
    case msrNote::kNoteHeadLeftTriangle:
      result = "noteHead: leftTriangle";
      break;
    case msrNote::kNoteHeadRectangle:
      result = "noteHead: rectangle";
      break;
    case msrNote::kNoteHeadNone:
      result = "noteHead: none";
      break;
    case msrNote::kNoteHeadDo:
      result = "noteHead: do";
      break;
    case msrNote::kNoteHeadRe:
      result = "noteHead: re";
      break;
    case msrNote::kNoteHeadMi:
      result = "noteHead: mi";
      break;
    case msrNote::kNoteHeadFa:
      result = "noteHead: fa";
      break;
    case msrNote::kNoteHeadFaUp:
      result = "noteHead: faUp";
      break;
    case msrNote::kNoteHeadSo:
      result = "noteHead: so";
      break;
    case msrNote::kNoteHeadLa:
      result = "noteHead: la";
      break;
    case msrNote::kNoteHeadTi:
      result = "noteHead: ii";
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
      result = "headFilled: yes";
      break;
    case msrNote::kNoteHeadFilledNo:
      result = "headFilled: no";
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
      result = "headParentheses: yes";
      break;
    case msrNote::kNoteHeadParenthesesNo:
      result = "headParentheses: no";
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

msrDiatonicPitch msrNote::noteDiatonicPitch (
  int inputLineNumber) const
{
  return
    msrDiatonicPitchFromQuarterTonesPitch (
      inputLineNumber,
      fNoteQuarterTonesPitch);
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
  if (gGeneralOptions->fTraceChords)
    gLogIos <<
      "Setting note '" <<
      noteAsShortStringWithRawWholeNotes () <<
      "' to belong to a chord"
      ", line " << fInputLineNumber <<
      endl;

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

  if (gGeneralOptions->fTraceTuplets || gGeneralOptions->fTraceNotes)
    gLogIos <<
      "Determining tuplet sounding from display whole notes" <<
      " for note '" <<
      noteAsShortString () <<
      ", factor is '" <<
      actualNotes << "/" << normalNotes <<
      "', line " << fInputLineNumber <<
      endl;

  // the display whole notes are known, compute the sounding ones
  fNoteSoundingWholeNotes =
    fNoteDisplayWholeNotes
      *
    normalNotes
      /
    actualNotes;

  fNoteSoundingWholeNotes.rationalise ();

  if (gGeneralOptions->fTraceTuplets || gGeneralOptions->fTraceNotes)
    gLogIos <<
      "The result is: '" <<
      noteAsShortString () <<
      endl;
}

void msrNote::addBeamToNote (S_msrBeam beam)
{
  fNoteBeams.push_back (beam);
}

void msrNote::addArticulationToNote (S_msrArticulation art)
{
  fNoteArticulations.push_back (art);
}

void msrNote::addTechnicalToNote (S_msrTechnical technical)
{
  if (gGeneralOptions->fTraceNotes || gGeneralOptions->fTraceTechnicals)
    gLogIos <<
      "Adding technical '" <<
      technical->technicalAsString () <<
      "' to note '" << noteAsShortString () <<
      "', line " << fInputLineNumber <<
      endl;

  // append the technical to the note technicals list
  fNoteTechnicals.push_back (technical);

  // set technical's note uplink
  technical->
    setTechnicalNoteUplink (this);
}

void msrNote::addTechnicalWithIntegerToNote (
  S_msrTechnicalWithInteger technicalWithInteger)
{
  if (gGeneralOptions->fTraceNotes || gGeneralOptions->fTraceTechnicals)
    gLogIos <<
      "Adding technical with integer '" <<
      technicalWithInteger->technicalWithIntegerAsString () <<
      "' to note '" << noteAsShortString () <<
      "', line " << fInputLineNumber <<
      endl;

  // append the technical with integer to the note technicals with integers list
  fNoteTechnicalWithIntegers.push_back (technicalWithInteger);

  // set technical's note uplink
  technicalWithInteger->
    setTechnicalWithIntegerNoteUplink (this);
}

void msrNote::addTechnicalWithStringToNote (
  S_msrTechnicalWithString technicalWithString)
{
  if (gGeneralOptions->fTraceNotes || gGeneralOptions->fTraceTechnicals)
    gLogIos <<
      "Adding technical with string'" <<
      technicalWithString->technicalWithStringAsString () <<
      "' to note '" << noteAsShortString () <<
      "', line " << fInputLineNumber <<
      endl;

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
    case msrOrnament::kTrillMark:
      fNoteHasATrill = true;
      break;

    case msrOrnament::kDelayedTurn:
    case msrOrnament::kDelayedInvertedTurn:
      fNoteHasADelayedOrnament = true;
      break;

    default:
      {}
  } // switch

  // set ornament's note uplink
  ornament->
    setOrnamentNoteUplink (this);
}

void msrNote::addSingleTremoloToNote (S_msrSingleTremolo trem)
{
  if (gGeneralOptions->fTraceTremolos)
    gLogIos <<
      "Adding singleTremolo " << trem->singleTremoloAsString () <<
      " to note " << noteAsShortString () <<
      ", line " << trem->getInputLineNumber () <<
      endl;

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
  if (gGeneralOptions->fTraceSlurs)
    gLogIos <<
      "Adding slur " << slur << " to note " << noteAsString ()
       << endl;

  if (fNoteSlurs.size ()) {
    if (
      fNoteSlurs.back ()->getSlurKind () == msrSlur::kStartSlur
        &&
      slur->getSlurKind () == msrSlur::kStopSlur
        &&
      fNoteSlurs.back ()->getSlurNumber () == slur->getSlurNumber ()
      ) {
      // it may happen that a given note has a 'slur start'
      // and a 'slur stop' in sequence, ignore both

      stringstream s;
      
      s <<
        "a 'slur start' is immediately followed by a 'slur stop'" <<
        endl <<
        "with the same number, ignoring both of them at line " <<
        slur->getInputLineNumber ();
        
      msrMusicXMLWarning (
        slur->getInputLineNumber (),
        s.str ());
        
      // rmeove 'slur start'
      fNoteSlurs.pop_back ();

      // don't register 'slur stop'
    }

    else
      fNoteSlurs.push_back (slur);
  }

  else
    fNoteSlurs.push_back (slur);
}

void msrNote::addLigatureToNote (S_msrLigature ligature)
{
  if (gGeneralOptions->fTraceLigatures)
    gLogIos <<
      "Adding ligature " << ligature << " to note " << noteAsString ()
       << endl;

  if (fNoteLigatures.size ()) {
    if (
      fNoteLigatures.back ()->getLigatureKind () == msrLigature::kStartLigature
        &&
      ligature->getLigatureKind () == msrLigature::kStopLigature
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
  if (gGeneralOptions->fTraceLyrics)
    gLogIos <<
      "Appending syllable '" <<
      syllable->syllableAsString () <<
      "' to note " << noteAsString () <<
      endl;

  fNoteSyllables.push_back (syllable);
}

void msrNote::setNoteHarmony (S_msrHarmony harmony)
{
  if (gGeneralOptions->fTraceNotes || gGeneralOptions->fTraceHarmonies)
    gLogIos <<
      "Setting note '" << noteAsShortString ()  << "'" <<
      " harmony to '" << harmony->harmonyAsString () << "'" <<
      endl;
      
  fNoteHarmony = harmony;
}

void msrNote::setNoteFiguredBass (S_msrFiguredBass figuredBass)
{
  if (gGeneralOptions->fTraceNotes || gGeneralOptions->fTraceHarmonies)
    gLogIos <<
      "Setting note '" << noteAsShortString ()  << "'" <<
      " figured bass to '" << figuredBass->figuredBassAsString () << "'" <<
      endl;
      
  fNoteFiguredBass = figuredBass;
}

void msrNote::acceptIn (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors)
    gLogIos <<
      "% ==> msrNote::acceptIn()" <<
      endl;
      
  if (visitor<S_msrNote>*
    p =
      dynamic_cast<visitor<S_msrNote>*> (v)) {
        S_msrNote elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors)
          gLogIos <<
            "% ==> Launching msrNote::visitStart()" <<
             endl;
        p->visitStart (elem);
  }
}

void msrNote::acceptOut (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors)
    gLogIos <<
      "% ==> msrNote::acceptOut()" <<
      endl;

  if (visitor<S_msrNote>*
    p =
      dynamic_cast<visitor<S_msrNote>*> (v)) {
        S_msrNote elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors)
          gLogIos <<
            "% ==> Launching msrNote::visitEnd()" <<
            endl;
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
  if (fNoteBeams.size()) {
    idtr++;
    list<S_msrBeam>::const_iterator i;
    for (i=fNoteBeams.begin(); i!=fNoteBeams.end(); i++) {
      // browse the beam
      msrBrowser<msrBeam> browser (v);
      browser.browse (*(*i));
    } // for
    idtr--;
  }
  
  // browse the articulations if any
  if (fNoteArticulations.size()) {
    idtr++;
    list<S_msrArticulation>::const_iterator i;
    for (i=fNoteArticulations.begin(); i!=fNoteArticulations.end(); i++) {
      // browse the articulation
      msrBrowser<msrArticulation> browser (v);
      browser.browse (*(*i));
    } // for
    idtr--;
  }
  
  // browse the technicals if any
  if (fNoteTechnicals.size()) {
    idtr++;
    list<S_msrTechnical>::const_iterator i;
    for (
      i=fNoteTechnicals.begin();
      i!=fNoteTechnicals.end();
      i++) {
      // browse the technical
      msrBrowser<msrTechnical> browser (v);
      browser.browse (*(*i));
    } // for
    idtr--;
  }
  
  // browse the technicals with integer if any
  if (fNoteTechnicalWithIntegers.size()) {
    idtr++;
    list<S_msrTechnicalWithInteger>::const_iterator i;
    for (
      i=fNoteTechnicalWithIntegers.begin();
      i!=fNoteTechnicalWithIntegers.end();
      i++) {
      // browse the technical
      msrBrowser<msrTechnicalWithInteger> browser (v);
      browser.browse (*(*i));
    } // for
    idtr--;
  }
  
  // browse the technicals with string if any
  if (fNoteTechnicalWithStrings.size()) {
    idtr++;
    list<S_msrTechnicalWithString>::const_iterator i;
    for (
      i=fNoteTechnicalWithStrings.begin();
      i!=fNoteTechnicalWithStrings.end();
      i++) {
      // browse the technical
      msrBrowser<msrTechnicalWithString> browser (v);
      browser.browse (*(*i));
    } // for
    idtr--;
  }
  
  // browse the ornaments if any
  if (fNoteOrnaments.size()) {
    idtr++;
    list<S_msrOrnament>::const_iterator i;
    for (i=fNoteOrnaments.begin(); i!=fNoteOrnaments.end(); i++) {
      // browse the ornament
      msrBrowser<msrOrnament> browser (v);
      browser.browse (*(*i));
    } // for
    idtr--;
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
  if (fNoteDynamics.size()) {
    idtr++;
    list<S_msrDynamics>::const_iterator i;
    for (i=fNoteDynamics.begin(); i!=fNoteDynamics.end(); i++) {
      // browse the dynamics
      msrBrowser<msrDynamics> browser (v);
      browser.browse (*(*i));
    } // for
    idtr--;
  }

  // browse the other dynamics if any
  if (fNoteOtherDynamics.size()) {
    idtr++;
    list<S_msrOtherDynamics>::const_iterator i;
    for (i=fNoteOtherDynamics.begin(); i!=fNoteOtherDynamics.end(); i++) {
      // browse the other dynamics
      msrBrowser<msrOtherDynamics> browser (v);
      browser.browse (*(*i));
    } // for
    idtr--;
  }

  // browse the words if any
  if (fNoteWords.size()) {
    idtr++;
    list<S_msrWords>::const_iterator i;
    for (i=fNoteWords.begin(); i!=fNoteWords.end(); i++) {
      // browse the words
      msrBrowser<msrWords> browser (v);
      browser.browse (*(*i));
    } // for
    idtr--;
  }
  
  // browse the slurs if any
  if (fNoteSlurs.size()) {
    idtr++;
    list<S_msrSlur>::const_iterator i;
    for (i=fNoteSlurs.begin(); i!=fNoteSlurs.end(); i++) {
      // browse the slur
      msrBrowser<msrSlur> browser (v);
      browser.browse (*(*i));
    } // for
    idtr--;
  }

  // browse the ligatures if any
  if (fNoteLigatures.size()) {
    idtr++;
    list<S_msrLigature>::const_iterator i;
    for (i=fNoteLigatures.begin(); i!=fNoteLigatures.end(); i++) {
      // browse the ligature
      msrBrowser<msrLigature> browser (v);
      browser.browse (*(*i));
    } // for
    idtr--;
  }

  // browse the wedges if any
  if (fNoteWedges.size()) {
    idtr++;
    list<S_msrWedge>::const_iterator i;
    for (i=fNoteWedges.begin(); i!=fNoteWedges.end(); i++) {
      // browse the wedge
      msrBrowser<msrWedge> browser (v);
      browser.browse (*(*i));
    } // for
    idtr--;
  }

  // browse the syllables if any
  if (fNoteSyllables.size()) {
    idtr++;
    list<S_msrSyllable>::const_iterator i;
    for (i=fNoteSyllables.begin(); i!=fNoteSyllables.end(); i++) {
      // browse the syllable
      msrBrowser<msrSyllable> browser (v);
      browser.browse (*(*i));
    } // for
    idtr--;
  }

  if (fNoteHarmony) {
    // browse the harmony
    msrBrowser<msrHarmony> browser (v);
    browser.browse (*fNoteHarmony);
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
  gLogIos << "msrNote::notePitchAsString (), isRest = " <<
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
      msrQuarterTonesPitchAsString (
        gMsrOptions->fMsrQuarterTonesPitchesLanguage,
        fNoteQuarterTonesPitch);
  }
  
  return s.str ();
}

string msrNote::noteDisplayPitchAsString () const
{
  stringstream s;
  
  /* JMI
  gLogIos << "msrNote::notePitchAsString (), isRest = " <<
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
      msrQuarterTonesPitchAsString (
        gMsrOptions->fMsrQuarterTonesPitchesLanguage,
        fNoteQuarterTonesDisplayPitch);  

 // JMI }
  
  return s.str ();
}

string msrNote::noteGraphicDurationAsMsrString () const
{
  string
    result =
      msrDurationAsString (
        fNoteGraphicDuration);
    
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

string msrNote::noteDiatonicPitchAsString (
  int inputLineNumber) const
{
  return
    msrDiatonicPitchAsString (
      gMsrOptions->fMsrQuarterTonesPitchesLanguage,
      noteDiatonicPitch (
        fInputLineNumber));
}

string msrNote::noteAsShortStringWithRawWholeNotes () const
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
        "[" << fNoteOctave << ", " << noteDisplayOctaveAsString () << "]" <<
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
        "[" << fNoteOctave << ", " << noteDisplayOctaveAsString () << "]" <<
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
        "[" << fNoteOctave << ", " << noteDisplayOctaveAsString () << "]" <<
        ":" <<
        " whole notes: " <<
        fNoteSoundingWholeNotes <<
        " sound, " <<
        fNoteDisplayWholeNotes <<
        " disp";
      break;
      
    case msrNote::kGraceNote:
      s <<
        notePitchAsString () <<
        noteGraphicDurationAsMsrString () <<
        "[" << fNoteOctave << ", " << noteDisplayOctaveAsString () << "]";
        
      for (int i = 0; i < fNoteDotsNumber; i++) {
        s << "."; // JMI
      } // for
      break;
      
    case msrNote::kChordMemberNote:
      s <<
        notePitchAsString () <<
        "[" << fNoteOctave << ", " << noteDisplayOctaveAsString () << "]" <<
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
        "[" << fNoteOctave << ", " << noteDisplayOctaveAsString () << "]";

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

string msrNote::noteAsShortString ()
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
        "[" << fNoteOctave << ", " << noteDisplayOctaveAsString () << "]";
      break;
      
    case msrNote::kDoubleTremoloMemberNote:
      s <<
        notePitchAsString () <<
        noteSoundingWholeNotesAsMsrString () <<
        "[" << fNoteOctave << ", " << noteDisplayOctaveAsString () << "]";
      break;
      
    case msrNote::kGraceNote:
      s <<
        notePitchAsString () <<
        noteGraphicDurationAsMsrString () <<
        "[" << fNoteOctave << ", " << noteDisplayOctaveAsString () << "]";
        
      for (int i = 0; i < fNoteDotsNumber; i++) {
        s << "."; // JMI
      } // for
      break;
      
    case msrNote::kChordMemberNote:
      s <<
        notePitchAsString () <<
        ", " <<
        noteSoundingWholeNotesAsMsrString () <<
        "[" << fNoteOctave << ", " << noteDisplayOctaveAsString () << "]";
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
        "[" << fNoteOctave << ", " << noteDisplayOctaveAsString () << "]";

      s <<
        ", line " << fInputLineNumber;
      break;
  } // switch

  return s.str ();
}

string msrNote::noteAsString ()
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

      if (fNoteDisplayOctave < 0)
        s <<
          "unpitched";
      else
        s <<
          "(" <<
          noteDisplayPitchAsString () <<
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
        "Double tremolo note" " "<<
        notePitchAsString () <<
        noteSoundingWholeNotesAsMsrString () <<
        " [octave" " " << fNoteOctave << ", " << noteDisplayOctaveAsString () << "]";
      break;
      
    case msrNote::kGraceNote:
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

      s <<
        ", line " << fInputLineNumber;
      break;
  } // switch

  if (fNoteOccupiesAFullMeasure)
    s <<
      ", full measure";

  if (fNoteHasATrill)
    s <<
      ", has a trill";
  
  if (fNoteIsFollowedByGraceNotes)
    s <<
      ", followed by grace notes";
  
/* JMI
  if (fNoteTie) {
    if (fNoteTie->getTieKind () != msrTie::k_NoTie ) {
      s <<
        ", " << fNoteTie->tieKindAsString ();
    }
  }
*/

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
    measureFullMeasureLength =
      fNoteMeasureUplink
        ? 
          fNoteMeasureUplink->
            getMeasureFullMeasureLength ()
        : rational (0, 1); // JMI
  
  // print the note itself and its positionInMeasure
  os <<
    noteAsString () <<
    ", line " << fInputLineNumber;

  {
    // print displayed whole notes
    os <<
      endl;
  
    idtr++;
  
    os <<
      idtr;
      
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
    if (measureFullMeasureLength.getNumerator () == 0) {
      os <<
        ", full measure length unknown, no time signature";
    }
    else {
      os <<
        ", " <<
        measureFullMeasureLength <<
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
 // JMI     msrInternalError (
      msrInternalWarning (
        fInputLineNumber,
        "note measure number is empty");
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
      
    idtr--;
  }

  {
    // note redundant information (for speed)

    stringstream s;
    
    idtr++;

    if (fNoteIsStemless)
      s <<
        "stemless" <<
        endl;
  
    if (fNoteIsFirstNoteInADoubleTremolo)
      s <<
        "first note in a double tremolo" <<
        endl;
    if (fNoteIsSecondNoteInADoubleTremolo)
      s <<
        "second note in a double tremolo" <<
        endl;
  
    if (fNoteHasATrill)
      s <<
        "has a trill" <<
        endl;
    if (fNoteIsFollowedByGraceNotes)
      s <<
        "is followed by graceNotes" <<
        endl;
  
    if (fNoteHasADelayedOrnament)
      s <<
        "has a delayed ornament" <<
        endl;

    string result = s.str ();

    if (result.size ()) {
      os <<
        result;
    }
        
    idtr--;
  }
  
  {
    // note MSR strings

    idtr++;
    
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
          const int fieldWidth = 31; // JMI

        }
        break;
        
      case msrNote::kGraceNote:
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
          const int fieldWidth = 46; // JMI

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
  
    idtr--;
  }

  // print the syllables associated to this note if any
  if (fNoteSyllables.size ()) {
    idtr++;

    os <<
      "Syllables:" <<
      endl;
      
    idtr++;
    
    list<S_msrSyllable>::const_iterator
      iBegin = fNoteSyllables.begin(),
      iEnd   = fNoteSyllables.end(),
      i      = iBegin;
    for ( ; ; ) {
      S_msrSyllable
        syllable = (*i);
        
      os <<
        syllable->syllableKindAsString () <<
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
            noteAsShortString ();
      if (++i == iEnd) break;
      os << endl;
    } // for

    idtr--;
    
    os <<
      endl;

    idtr--;
  }

/*
  // print the note's voice uplink JMI
  idtr++;
  os <<
    "NoteVoiceUplink" " = " <<
    fNoteMeasureUplink->fetchMeasureVoiceUplink () <<
    endl;
  idtr--;
*/

  // print the extend kind if relevant
  switch (fNoteSyllableExtendKind) {
    
    case msrSyllable::kStandaloneSyllableExtend:
    case msrSyllable::kStartSyllableExtend:
    case msrSyllable::kContinueSyllableExtend:
    case msrSyllable::kStopSyllableExtend:
      idtr++;
      os <<
        "NoteSyllableExtendKind" " = " <<
        msrSyllable::syllableExtendKindAsString (
          fNoteSyllableExtendKind) <<
        endl;
      idtr--;
      break;
      
    case msrSyllable::k_NoSyllableExtend:
      break;
  } // switch

  // print the octave shift if any
  if (fNoteOctaveShift) {
    idtr++;
    os <<
      fNoteOctaveShift;
    idtr--;
  }

  // print the stem if any
  if (fNoteStem) {
    idtr++;
    os <<
      fNoteStem;
    idtr--;
  }
    
  // print the beams if any
  if (fNoteBeams.size()) {
    idtr++;

    list<S_msrBeam>::const_iterator
      iBegin = fNoteBeams.begin(),
      iEnd   = fNoteBeams.end(),
      i      = iBegin;
    for ( ; ; ) {
      os << (*i);
      if (++i == iEnd) break;
      os << endl;
    } // for
        
    idtr--;
  }
  
  // print the articulations if any
  if (fNoteArticulations.size()) {
    idtr++;

    list<S_msrArticulation>::const_iterator
      iBegin = fNoteArticulations.begin(),
      iEnd   = fNoteArticulations.end(),
      i      = iBegin;
    for ( ; ; ) {
      os << (*i);
      if (++i == iEnd) break;
// JMI      os << endl;
    } // for
        
    idtr--;
  }
  
  // print the technicals if any
  if (fNoteTechnicals.size()) {
    idtr++;

    list<S_msrTechnical>::const_iterator
      iBegin = fNoteTechnicals.begin(),
      iEnd   = fNoteTechnicals.end(),
      i      = iBegin;
    for ( ; ; ) {
      os << (*i);
      if (++i == iEnd) break;
 // JMI     os << endl;
    } // for
        
    idtr--;
  }
  
  // print the technicals with integer if any
  if (fNoteTechnicalWithIntegers.size()) {
    idtr++;

    list<S_msrTechnicalWithInteger>::const_iterator
      iBegin = fNoteTechnicalWithIntegers.begin(),
      iEnd   = fNoteTechnicalWithIntegers.end(),
      i      = iBegin;
    for ( ; ; ) {
      os << (*i);
      if (++i == iEnd) break;
 // JMI     os << endl;
    } // for
        
    idtr--;
  }
  
  // print the technicals with string if any
  if (fNoteTechnicalWithStrings.size()) {
    idtr++;

    list<S_msrTechnicalWithString>::const_iterator
      iBegin = fNoteTechnicalWithStrings.begin(),
      iEnd   = fNoteTechnicalWithStrings.end(),
      i      = iBegin;
    for ( ; ; ) {
      os << (*i);
      if (++i == iEnd) break;
// JMI      os << endl;
    } // for
        
    idtr--;
  }
  
  // print the ornaments if any
  if (fNoteOrnaments.size()) {
    idtr++;

    list<S_msrOrnament>::const_iterator
      iBegin = fNoteOrnaments.begin(),
      iEnd   = fNoteOrnaments.end(),
      i      = iBegin;
    for ( ; ; ) {
      os << (*i);
      if (++i == iEnd) break;
 // JMI     os << endl;
    } // for
        
    idtr--;
  }
  
  // print the singleTremolo if any
  if (fNoteSingleTremolo) {
    idtr++;
    os << fNoteSingleTremolo;
    idtr--;
  }
  
  // print the tie if any
  if (fNoteTie) {
    idtr++;
    os <<
      fNoteTie;
    idtr--;
  }
    
  // print the dynamics if any
  if (fNoteDynamics.size()) {
    idtr++;
    
    list<S_msrDynamics>::const_iterator
      iBegin = fNoteDynamics.begin(),
      iEnd   = fNoteDynamics.end(),
      i      = iBegin;
    for ( ; ; ) {
      os << (*i);
      if (++i == iEnd) break;
 // JMI     os << endl;
    } // for
// JMI    os << endl;

    idtr--;
  }

  // print the other dynamics if any
  if (fNoteOtherDynamics.size()) {
    idtr++;
    
    list<S_msrOtherDynamics>::const_iterator
      iBegin = fNoteOtherDynamics.begin(),
      iEnd   = fNoteOtherDynamics.end(),
      i      = iBegin;
    for ( ; ; ) {
      os << (*i);
      if (++i == iEnd) break;
// JMI      os << endl;
    } // for
// JMI    os << endl;

    idtr--;
  }

  // print the words if any
  if (fNoteWords.size()) {
    idtr++;
    
    list<S_msrWords>::const_iterator
      iBegin = fNoteWords.begin(),
      iEnd   = fNoteWords.end(),
      i      = iBegin;
    for ( ; ; ) {
      os << (*i);
      if (++i == iEnd) break;
// JMI      os << endl;
    } // for
// JMI    os << endl;

    idtr--;
  }

  // print the slurs if any
  if (fNoteSlurs.size()) {
    idtr++;
    
    list<S_msrSlur>::const_iterator
      iBegin = fNoteSlurs.begin(),
      iEnd   = fNoteSlurs.end(),
      i      = iBegin;
    for ( ; ; ) {
      os << (*i);
      if (++i == iEnd) break;
 // JMI     os << endl;
    } // for
// JMI    os << endl;
    
    idtr--;
  }
  
  // print the ligatures if any
  if (fNoteLigatures.size()) {
    idtr++;
    
    list<S_msrLigature>::const_iterator
      iBegin = fNoteLigatures.begin(),
      iEnd   = fNoteLigatures.end(),
      i      = iBegin;
    for ( ; ; ) {
      os << (*i);
      if (++i == iEnd) break;
 // JMI     os << endl;
    } // for
// JMI    os << endl;
    
    idtr--;
  }
  
  // print the wedges if any
  if (fNoteWedges.size()) {
    idtr++;
    
    list<S_msrWedge>::const_iterator
      iBegin = fNoteWedges.begin(),
      iEnd   = fNoteWedges.end(),
      i      = iBegin;
    for ( ; ; ) {
      os << (*i);
      if (++i == iEnd) break;
 // JMI     os << endl;
    } // for
// JMI    os << endl;
    
    idtr--;
  }

  // print the harmony if any
  if (fNoteHarmony) {
    idtr++;
    os <<
      fNoteHarmony <<
      endl;
    idtr--;
  }

  // print the figured bass if any
  if (fNoteFiguredBass) {
    idtr++;
    os <<
      fNoteFiguredBass <<
      endl;
    idtr--;
  }
}

//______________________________________________________________________________
S_msrChord msrChord::create (
  int         inputLineNumber,
  rational    chordSoundingWholeNotes,
  rational    chordDisplayWholeNotes,
  msrDuration chordGraphicDuration)
{
  if (gGeneralOptions->fTraceChords) {
    gLogIos <<
      "==> Creating a chord" <<
      ", chordSoundingWholeNotes = " << chordSoundingWholeNotes <<
      ", chordDisplayWholeNotes = " << chordDisplayWholeNotes <<
      ", chordGraphicDuration = " <<
      msrDurationAsString (chordGraphicDuration) <<
      endl;
 }
   
  msrChord* o =
    new msrChord (
      inputLineNumber,
      chordSoundingWholeNotes, chordDisplayWholeNotes,
      chordGraphicDuration);
  assert(o!=0);

  return o;
}

msrChord::msrChord (
  int         inputLineNumber,
  rational    chordSoundingWholeNotes,
  rational    chordDisplayWholeNotes,
  msrDuration chordGraphicDuration)
    : msrElement (inputLineNumber)
{
  fChordSoundingWholeNotes = chordSoundingWholeNotes;
  fChordDisplayWholeNotes  = chordDisplayWholeNotes;
  
  fChordGraphicDuration = chordGraphicDuration;

  fChordIsFirstChordInADoubleTremolo  = false;
  fChordIsSecondChordInADoubleTremolo = false;
}

msrChord::~msrChord()
{}

S_msrChord msrChord::createChordNewbornClone (
  S_msrPart containingPart)
{
  if (gGeneralOptions->fTraceChords) {
    gLogIos <<
      "==> Creating a newborn clone of chord '" <<
      chordAsString () <<
      "'" <<
      endl;
  }

  // sanity check
  msrAssert(
    containingPart != 0,
    "containingPart is null");
    
  S_msrChord
    newbornClone =
      msrChord::create (
        fInputLineNumber,
        fChordSoundingWholeNotes,
        fChordDisplayWholeNotes,
        fChordGraphicDuration);
    
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
  if (gGeneralOptions->fTraceChords)
    gLogIos <<
      "Setting chord sounding whole notes to '" <<
      wholeNotes <<
      "' for chord '" <<
      chordAsString () <<
      "'" <<
      endl;

  fChordSoundingWholeNotes = wholeNotes;
}

void msrChord::setChordDisplayWholeNotes (
  rational wholeNotes)
{
  if (gGeneralOptions->fTraceChords)
    gLogIos <<
      "Setting chord displayed whole notes to '" <<
      wholeNotes <<
      "' for chord '" <<
      chordAsString () <<
      "'" <<
      endl;

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
  if (gGeneralOptions->fTraceChords)
    gLogIos <<
      "Adding first note '" <<
      note->noteAsString () <<
      "' to chord '" <<
      chordAsString () <<
      "'" <<
      endl;

  fChordNotes.push_back (note);
  
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
  if (gGeneralOptions->fTraceChords)
    gLogIos <<
      "Adding another note '" <<
      note->noteAsShortStringWithRawWholeNotes () <<
      "' to chord '" <<
      chordAsString () <<
      "'" <<
      endl;

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
    // JMI XXL
   // abort ();
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
    // JMI XXL
   // abort ();
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
    list<S_msrArticulation>::const_iterator i = fChordArticulations.begin();
    i!=fChordArticulations.end();
    i++) {
      if ((*i)->getArticulationKind () == articulationKind)
        return;
  } // for

  if (gGeneralOptions->fTraceTremolos || gGeneralOptions->fTraceChords) // JMI
    gLogIos <<
      "Adding articulation '" <<
      art->articulationKindAsString () <<
      "' to chord" <<
      endl;

  fChordArticulations.push_back (art);
}

void msrChord::addSingleTremoloToChord (S_msrSingleTremolo trem)
{
  if (gGeneralOptions->fTraceTremolos || gGeneralOptions->fTraceChords)
    gLogIos <<
      "Adding singleTremolo '" <<
      trem->singleTremoloAsString () <<
      "' to chord" <<
      endl;

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
      fChordTechnicals.begin();
    i!=fChordTechnicals.end();
    i++) {
      if ((*i)->getTechnicalKind () == technicalKind)
        return;
  } // for

  if (gGeneralOptions->fTraceChords)
    gLogIos <<
      "Adding technical '" <<
      tech->technicalAsString () <<
      "' to chord" <<
      endl;

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
      fChordTechnicalWithIntegers.begin();
    i!=fChordTechnicalWithIntegers.end();
    i++) {
      if ((*i)->getTechnicalWithIntegerKind () == technicalWithIntegerKind)
        return;
  } // for

  if (gGeneralOptions->fTraceChords)
    gLogIos <<
      "Adding technical with integer '" <<
      tech->technicalWithIntegerAsString () <<
      "' to chord" <<
      endl;

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
      fChordTechnicalWithStrings.begin();
    i!=fChordTechnicalWithStrings.end();
    i++) {
      if ((*i)->getTechnicalWithStringKind () == technicalWithStringKind)
        return;
  } // for

  if (gGeneralOptions->fTraceChords)
    gLogIos <<
      "Adding technical with integer '" <<
      tech->technicalWithStringAsString () <<
      "' to chord" <<
      endl;

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
    list<S_msrOrnament>::const_iterator i = fChordOrnaments.begin();
    i!=fChordOrnaments.end();
    i++) {
      if ((*i)->getOrnamentKind () == ornamentKind)
        return;
  } // for

  if (gGeneralOptions->fTraceChords)
    gLogIos <<
      "Adding ornament '" <<
      orn->ornamentKindAsString () <<
      "' to chord" <<
      endl;

  fChordOrnaments.push_back (orn);
}

void msrChord::acceptIn (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors)
    gLogIos <<
      "% ==> msrChord::acceptIn()" <<
      endl;
      
  if (visitor<S_msrChord>*
    p =
      dynamic_cast<visitor<S_msrChord>*> (v)) {
        S_msrChord elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors)
          gLogIos <<
            "% ==> Launching msrChord::visitStart()" <<
             endl;
        p->visitStart (elem);
  }
}

void msrChord::acceptOut (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors)
    gLogIos <<
      "% ==> msrChord::acceptOut()" <<
      endl;

  if (visitor<S_msrChord>*
    p =
      dynamic_cast<visitor<S_msrChord>*> (v)) {
        S_msrChord elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors)
          gLogIos <<
            "% ==> Launching msrChord::visitEnd()" <<
            endl;
        p->visitEnd (elem);
  }
}

void msrChord::browseData (basevisitor* v)
{
  for (
    vector<S_msrNote>::const_iterator i = fChordNotes.begin();
    i != fChordNotes.end();
    i++ ) {
    // browse chord member note
    msrBrowser<msrNote> browser (v);
    browser.browse (*(*i));
  } // for

  for (
    list<S_msrArticulation>::const_iterator i = fChordArticulations.begin();
    i != fChordArticulations.end();
    i++ ) {
    // browse the articulation
    msrBrowser<msrArticulation> browser (v);
    browser.browse (*(*i));
  } // for

  for (
    list<S_msrTechnical>::const_iterator i = fChordTechnicals.begin();
    i != fChordTechnicals.end();
    i++ ) {
    // browse the technical
    msrBrowser<msrTechnical> browser (v);
    browser.browse (*(*i));
  } // for

  for (
    list<S_msrOrnament>::const_iterator i = fChordOrnaments.begin();
    i != fChordOrnaments.end();
    i++ ) {
    // browse the ornament
    msrBrowser<msrOrnament> browser (v);
    browser.browse (*(*i));
  } // for

  for (
    list<S_msrDynamics>::const_iterator i = fChordDynamics.begin();
    i != fChordDynamics.end();
    i++ ) {
    // browse the dynamics
    msrBrowser<msrDynamics> browser (v);
    browser.browse (*(*i));
  } // for

  for (
    list<S_msrOtherDynamics>::const_iterator i = fChordOtherDynamics.begin();
    i != fChordOtherDynamics.end();
    i++ ) {
    // browse the other dynamics
    msrBrowser<msrOtherDynamics> browser (v);
    browser.browse (*(*i));
  } // for

  for (
    list<S_msrWords>::const_iterator i = fChordWords.begin();
    i != fChordWords.end();
    i++ ) {
    // browse the words
    msrBrowser<msrWords> browser (v);
    browser.browse (*(*i));
  } // for

  for (
    list<S_msrSlur>::const_iterator i = fChordSlurs.begin();
    i != fChordSlurs.end();
    i++ ) {
    // browse the slur
    msrBrowser<msrSlur> browser (v);
    browser.browse (*(*i));
  } // for
  
  for (
    list<S_msrLigature>::const_iterator i = fChordLigatures.begin();
    i != fChordLigatures.end();
    i++ ) {
    // browse the ligature
    msrBrowser<msrLigature> browser (v);
    browser.browse (*(*i));
  } // for
  
  for (
    list<S_msrWedge>::const_iterator i = fChordWedges.begin();
    i != fChordWedges.end();
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
  if (gGeneralOptions->fTraceTuplets || gGeneralOptions->fTraceChords)
    gLogIos <<
      "Applying tuplet member sounding factor '" <<
      actualNotes << "/" << normalNotes <<
      "' to the members of chord '" << chordAsStringwithRawDivisions () <<
      "', line " << fInputLineNumber <<
      endl;

  for (
    vector<S_msrNote>::const_iterator i = fChordNotes.begin();
    i != fChordNotes.end();
    ++i) {
    (*i)->
      applyTupletMemberSoundingFactorToNote (
        actualNotes, normalNotes);
  } // for
}
*/

string msrChord::chordAsStringwithRawDivisions () const
{
  stringstream s;

  s << "<";

  if (fChordNotes.size ()) {
    vector<S_msrNote>::const_iterator
      iBegin = fChordNotes.begin(),
      iEnd   = fChordNotes.end(),
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

        note->noteAsShortStringWithRawWholeNotes ();
        
      if (++i == iEnd) break;
      s << " ";
    } // for
  }

  s << ">";
  
  return s.str ();
}

string msrChord::chordAsString () const
{
  stringstream s;

  s << "<";

  if (fChordNotes.size ()) {
    vector<S_msrNote>::const_iterator
      iBegin = fChordNotes.begin(),
      iEnd   = fChordNotes.end(),
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
    measureFullMeasureLength =
      fChordMeasureUplink
        ? 
          fChordMeasureUplink->
            getMeasureFullMeasureLength ()
        : rational (0, 1); // JMI
    
  os <<
    "Chord:" <<
    chordSoundingWholeNotesAsMsrString () <<
    ", " <<
    singularOrPlural (
      fChordNotes.size (), "note", "notes") <<
    ", whole notes: " <<
    fChordSoundingWholeNotes << " sound whole notes" <<
    ", " <<
    fChordDisplayWholeNotes << " disp whole notes" <<
    ", meas "<<
    getChordMeasureNumber () <<
    ":" <<
    fChordPositionInMeasure <<
    "/" <<
    measureFullMeasureLength;

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
    
  idtr++;
  
  // print the member notes if any
  if (fChordNotes.size()) {
    vector<S_msrNote>::const_iterator i;
    for (i=fChordNotes.begin(); i!=fChordNotes.end(); i++) {
      os << (*i);
    } // for

/* JMI   vector<S_msrNote>::const_iterator
      iBegin = fChordNotes.begin(),
      iEnd   = fChordNotes.end(),
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
  if (fChordArticulations.size()) {
    list<S_msrArticulation>::const_iterator i;
    for (i=fChordArticulations.begin(); i!=fChordArticulations.end(); i++) {
      os << (*i);
    } // for
  }

  // print the technicals if any
  if (fChordTechnicals.size()) {
    list<S_msrTechnical>::const_iterator i;
    for (i=fChordTechnicals.begin(); i!=fChordTechnicals.end(); i++) {
      os << (*i);
    } // for
  }

  // print the ornaments if any
  if (fChordOrnaments.size()) {
    list<S_msrOrnament>::const_iterator i;
    for (i=fChordOrnaments.begin(); i!=fChordOrnaments.end(); i++) {
      os << (*i);
    } // for
  }

  // print the dynamics if any
  if (fChordDynamics.size()) {
    list<S_msrDynamics>::const_iterator i;
    for (i=fChordDynamics.begin(); i!=fChordDynamics.end(); i++) {
      os << (*i);
    } // for
  }

  // print the other dynamics if any
  if (fChordOtherDynamics.size()) {
    list<S_msrOtherDynamics>::const_iterator i;
    for (i=fChordOtherDynamics.begin(); i!=fChordOtherDynamics.end(); i++) {
      os << (*i);
    } // for
  }

  // print the beams if any
  if (fChordBeams.size()) {
    list<S_msrBeam>::const_iterator i;
    for (i=fChordBeams.begin(); i!=fChordBeams.end(); i++) {
      os << (*i);
    } // for
  }

  // print the words if any
  if (fChordWords.size()) {
    list<S_msrWords>::const_iterator i;
    for (i=fChordWords.begin(); i!=fChordWords.end(); i++) {
      os << (*i);
    } // for
  }

  // print the slurs if any
  if (fChordSlurs.size()) {
    list<S_msrSlur>::const_iterator i;
    for (i=fChordSlurs.begin(); i!=fChordSlurs.end(); i++) {
      os << (*i);
    } // for
  }

  // print the ligatures if any
  if (fChordLigatures.size()) {
    list<S_msrLigature>::const_iterator i;
    for (i=fChordLigatures.begin(); i!=fChordLigatures.end(); i++) {
      os << (*i);
    } // for
  }

  // print the wedges if any
  if (fChordWedges.size()) {
    list<S_msrWedge>::const_iterator i;
    for (i=fChordWedges.begin(); i!=fChordWedges.end(); i++) {
      os << (*i);
    } // for
  }

  // print the harmony if any
  if (fChordHarmony) {
    os <<
      "Chord harmony: " <<
      endl;
        
    idtr++;

    os <<
      fChordHarmony->harmonyAsString () <<
      endl;
      
    idtr--;
  }

  // print the figured bass if any
  if (fChordFiguredBass) {
    os <<
      "Chord fibured bass: " <<
      endl;
        
    idtr++;

    os <<
      fChordFiguredBass->figuredBassAsString () <<
      endl;
      
    idtr--;
  }

  idtr--;
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
 if (gGeneralOptions->fTraceDivisions) {
    gLogIos <<
      "==> Creating a newborn clone of divisions '" <<
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
  if (gGeneralOptions->fTraceDivisions) {
  gLogIos <<
    "==> Initializing divisions" <<
    ", divisionsPerQuarterNote = " << fDivisionsPerQuarterNote <<
    ", line " << fInputLineNumber <<
    endl;
  }
  
  // erase fDurationsToDivisions's contents
  fDurationsToDivisions.clear ();
  
  // positive powers of 2 of a quarter note
  int bigDivisions = fDivisionsPerQuarterNote;
  
  for (int i = kQuarter; i >= kMaxima; i--) {
    /*
    gLogIos <<
      msrDurationAsString (msrDuration (i)) <<
      " -> " <<
      bigDivisions <<
      endl;
    */
    
    fDurationsToDivisions.push_front (
      make_pair (msrDuration (i), bigDivisions));
      
    bigDivisions *= 2;
  } // for

  if (fDivisionsPerQuarterNote > 1) {
    // negative powers of 2 of a quarter note
    int
      smallDivisions =
        fDivisionsPerQuarterNote / 2;
    msrDuration
      currentDuration =
        kEighth;
    
    while (smallDivisions >= 1) {
      /*
      gLogIos <<
        msrDurationAsString (currentDuration) <<
        " % --> " <<
        smallDivisions <<
        endl;
      */

      fDurationsToDivisions.push_back (
        make_pair (currentDuration, smallDivisions));
        
      currentDuration = msrDuration (currentDuration + 1);
      smallDivisions /= 2;
    } // while
  }

  // print the durations divisions if needed
  if (gGeneralOptions->fTraceDivisions) {
    printDurationsDivisions (gLogIos);
  }
}

int msrDivisions::durationAsDivisions (
  int         inputLineNumber,
  msrDuration duration)
{
  for (
    list<pair<msrDuration, int> >::const_iterator i =
      fDurationsToDivisions.begin();
    i != fDurationsToDivisions.end ();
    i++) {
    if ((*i).first == duration)
      return (*i).second;
  } // for

  stringstream s;

  s <<
    "duration " << duration <<
    " cannot be converted to divisions with " <<
    fDivisionsPerQuarterNote << " divisions per quarter note" <<
    endl;

  printDurationsDivisions (s);
  
  msrInternalError (
    inputLineNumber,
    s.str ())
}

void msrDivisions::printDurationsDivisions (ostream& os)
{
  os <<
    "The mapping of durations to divisions with " <<
    fDivisionsPerQuarterNote << " divisions per quarter note" <<
    " is:" <<
    endl;

  idtr++;
  
  if (fDurationsToDivisions.size ()) {
    list<pair<msrDuration, int> >::const_iterator
      iBegin = fDurationsToDivisions.begin(),
      iEnd   = fDurationsToDivisions.end(),
      i      = iBegin;
          
    for ( ; ; ) {
      os <<
        setw (6) << left <<
        msrDurationAsString (msrDuration((*i).first)) <<
        ": " <<
        setw (4) << right <<
        (*i).second;

      if (++i == iEnd) break;
      
      os << endl;
    } // for

/* JMI

    for (
      list<pair<msrDuration, int> >::const_iterator i =
        fDurationsToDivisions.begin();
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

  idtr--;
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
  
  if (gGeneralOptions->fTraceDivisions) {
    gLogIos <<
     "--> divisionsAsMsrString ():" <<
      endl <<
      tab << setw (fieldWidth) <<
      "inputLineNumber" << " = " << inputLineNumber <<
      endl <<
      tab << setw (fieldWidth) <<
      "divisions" << " = " << divisions <<
      endl;
  }
    
  msrDuration baseDuration          = k1024th;
  int         baseDurationDivisions = -1;
  
  // search fDurationsToDivisions in longer to shortest order
  list<pair<msrDuration, int> >::const_iterator
    iBegin = fDurationsToDivisions.begin(),
    iEnd   = fDurationsToDivisions.end(),
    i      = iBegin;
    
  for ( ; ; ) {
    if (i == iEnd) {
      stringstream s;

      s <<
        "divisions " << divisions <<
        " could not be handled by divisionsAsMsrString () with:" <<
        endl;

      printDurationsDivisions (gLogIos);

      msrInternalError (
        inputLineNumber, s.str ());
      break;
    }

    if ((*i).second <= divisions) {
      // found base duration in list
      baseDuration          = (*i).first;
      baseDurationDivisions = (*i).second;

      result =
        msrDurationAsString (baseDuration);
      
      if (gGeneralOptions->fTraceDivisions) {
        gLogIos <<
            tab << setw (fieldWidth) <<
          "divisions" << " = " << divisions <<
          endl <<
          endl <<
            tab << setw (fieldWidth) <<
          "baseDurationDivisions" << " = " << baseDurationDivisions <<
          endl <<
            tab << setw (fieldWidth) <<
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

    if (gGeneralOptions->fTraceDivisions) {
      gLogIos <<
        tab << setw (fieldWidth) <<
        "divisions" << " = " << divisions <<
        endl <<
        tab << setw (fieldWidth) <<
      "baseDurationDivisions" << " = " << baseDurationDivisions <<
        endl <<
        tab << setw (fieldWidth) <<
      "nextDivisionsInList" << " = " << nextDivisionsInList <<
        endl <<
        tab << setw (fieldWidth) <<
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

      if (gGeneralOptions->fTraceDivisions) { // JMI
        gLogIos <<
          tab << setw (fieldWidth) <<
          "divisions" << " = " << divisions <<
          endl <<
          tab << setw (fieldWidth) <<
          "baseDurationDivisions" << " = " << baseDurationDivisions <<
          endl <<
          tab << setw (fieldWidth) <<
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
  
        if (gGeneralOptions->fTraceDivisions) {
          gLogIos <<
            tab << setw (fieldWidth) <<
            "divisions" << " = " << divisions <<
            endl <<
            tab << setw (fieldWidth) <<
            "baseDurationDivisions" << " = " << baseDurationDivisions <<
            endl <<
            tab << setw (fieldWidth) <<
            "nextDivisionsInList" << " = " << nextDivisionsInList <<
            endl <<
            tab << setw (fieldWidth) <<
            "remainingDivisions" << " = " << remainingDivisions <<
            endl <<
            tab << setw (fieldWidth) <<
            "dotsNumber" << " = " << dotsNumber <<
            endl <<
            endl;
        }
          
        if (dotsNumber > 5 )
          break; // JMI
      } // while
  
      if (gGeneralOptions->fTraceDivisions) {
        gLogIos <<
          tab << setw (fieldWidth) <<
          "divisions" << " = " << divisions <<
          endl <<
          tab << setw (fieldWidth) <<
          "baseDurationDivisions" << " = " << baseDurationDivisions <<
          endl <<
          tab << setw (fieldWidth) <<
          "nextDivisionsInList" << " = " << nextDivisionsInList <<
          endl <<
          tab << setw (fieldWidth) <<
          "remainingDivisions" << " = " << remainingDivisions <<
          endl <<
          tab << setw (fieldWidth) <<
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

  if (gGeneralOptions->fTraceDivisions) {
    gLogIos <<
      "<-- divisionsAsMsrString (): returns " << result <<
      endl <<
      endl;
  }
  
  return result;
}

string msrDivisions::wholeNotesAsMsrString (
  int      inputLineNumber,
  rational wholeNotes,
  int&     numberOfDotsNeeded)
{  
  string result;

  // the result is a base duration, followed by a suffix made of
  // either a sequence of dots or a multiplication factor

  const int fieldWidth = 22;
  
  if (gGeneralOptions->fTraceDivisions) {
    gLogIos << left <<
      endl <<
      "--> wholeNotesAsMsrString ():" <<
      endl <<
      tab << setw (fieldWidth) <<
      "inputLineNumber" << " = " << inputLineNumber <<
      endl <<
      tab << setw (fieldWidth) <<
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
  
  if (gGeneralOptions->fTraceDivisions) {
    gLogIos << left <<
      tab << "divisions" << " = " << divisions <<
      endl;
  }
    
  msrDuration baseDuration          = k1024th;
  int         baseDurationDivisions = -1;
  
  // search fDurationsToDivisions in longer to shortest order
  list<pair<msrDuration, int> >::const_iterator
    iBegin = fDurationsToDivisions.begin(),
    iEnd   = fDurationsToDivisions.end(),
    i      = iBegin;
    
  for ( ; ; ) {
    if (i == iEnd) {
      stringstream s;

      s <<
        "divisions " << divisions <<
        " could not be handled by wholeNotesAsMsrString () with:" <<
        endl;

      printDurationsDivisions (gLogIos);

      msrInternalError (
        inputLineNumber, s.str ());
      break;
    }

    if ((*i).second <= divisions) {
      // found base duration in list
      baseDuration          = (*i).first;
      baseDurationDivisions = (*i).second;

      result =
        msrDurationAsString (baseDuration);
      
      if (gGeneralOptions->fTraceDivisions) {
        gLogIos << left <<
          tab << setw (fieldWidth) <<
          "divisions" << " = " << divisions <<
          endl <<
          tab << setw (fieldWidth) <<
          "baseDuration" << " = " << msrDurationAsString (baseDuration) <<
          endl <<
          tab << setw (fieldWidth) <<
          "baseDurationDivisions" << " = " << baseDurationDivisions <<
          endl <<
          tab << setw (fieldWidth) <<
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

    if (gGeneralOptions->fTraceDivisions) {
      gLogIos << left <<
        tab << setw (fieldWidth) <<
        "divisions" << " = " << divisions <<
        endl <<
        tab << setw (fieldWidth) <<
        "baseDurationDivisions" << " = " << baseDurationDivisions <<
        endl <<
        tab << setw (fieldWidth) <<
        "nextDivisionsInList" << " = " << nextDivisionsInList <<
        endl <<
        tab << setw (fieldWidth) <<
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

      if (gGeneralOptions->fTraceDivisions) {
        gLogIos << left <<
          tab << setw (fieldWidth) <<
          "divisions" << " = " << divisions <<
          endl <<
          tab << setw (fieldWidth) <<
          "baseDurationDivisions" << " = " << baseDurationDivisions <<
          endl <<
          tab << setw (fieldWidth) <<
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
  
        if (gGeneralOptions->fTraceDivisions) {
          gLogIos << left <<
            tab << setw (fieldWidth) <<
            "divisions" << " = " << divisions <<
            endl <<
            tab << setw (fieldWidth) <<
            "baseDurationDivisions" << " = " << baseDurationDivisions <<
            endl <<
            tab << setw (fieldWidth) <<
            "nextDivisionsInList" << " = " << nextDivisionsInList <<
            endl <<
            tab << setw (fieldWidth) <<
            "remainingDivisions" << " = " << remainingDivisions <<
            endl <<
            tab << setw (fieldWidth) <<
            "dotsNumber" << " = " << dotsNumber <<
            endl <<
            endl;
        }
          
        if (dotsNumber > 5 )
          break; // JMI
      } // while
  
      if (gGeneralOptions->fTraceDivisions) {
        gLogIos << left <<
          tab << setw (fieldWidth) <<
          "divisions" << " = " << divisions <<
          endl <<
          tab << setw (fieldWidth) <<
          "baseDurationDivisions" << " = " << baseDurationDivisions <<
          endl <<
          tab << setw (fieldWidth) <<
          "nextDivisionsInList" << " = " << nextDivisionsInList <<
          endl <<
          tab << setw (fieldWidth) <<
          "remainingDivisions" << " = " << remainingDivisions <<
          endl <<
          tab << setw (fieldWidth) <<
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

  if (gGeneralOptions->fTraceDivisions) {
    gLogIos <<
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

void msrDivisions::acceptIn (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors)
    gLogIos <<
      "% ==> msrDivisions::acceptIn()" <<
      endl;
      
  if (visitor<S_msrDivisions>*
    p =
      dynamic_cast<visitor<S_msrDivisions>*> (v)) {
        S_msrDivisions elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors)
          gLogIos <<
            "% ==> Launching msrDivisions::visitStart()" <<
             endl;
        p->visitStart (elem);
  }
}

void msrDivisions::acceptOut (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors)
    gLogIos <<
      "% ==> msrDivisions::acceptOut()" <<
      endl;

  if (visitor<S_msrDivisions>*
    p =
      dynamic_cast<visitor<S_msrDivisions>*> (v)) {
        S_msrDivisions elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors)
          gLogIos <<
            "% ==> Launching msrDivisions::visitEnd()" <<
            endl;
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
  if (gGeneralOptions->fTraceMeasures) {
    gLogIos <<
      "==> Creating a bar check without next bar number" <<
      endl;
  }
}

msrBarCheck::msrBarCheck (
  int    inputLineNumber,
  string nextBarNumber)
    : msrElement (inputLineNumber)
{
  fNextBarNumber = nextBarNumber; 

  if (gGeneralOptions->fTraceMeasures) {
    gLogIos <<
      "==> Creating a bar check with next bar number '" <<
      fNextBarNumber <<
      "'" <<
      endl;
  }
}

msrBarCheck::~msrBarCheck()
{}

void msrBarCheck::setNextBarNumber (string number)
{
  if (gGeneralOptions->fTraceMeasures)
    gLogIos <<
      "Setting bar check next bar number to '" <<
      number <<
      "'" <<
      endl;

  fNextBarNumber = number;
}

string msrBarCheck::barCheckAsString () const
{
  stringstream s;

  s <<
    "BarCheck" <<
    ", next bar number = \"" << fNextBarNumber << "\"" <<
    ", line " << fInputLineNumber;

  return s.str ();
}

void msrBarCheck::acceptIn (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors)
    gLogIos <<
      "% ==> msrBarCheck::acceptIn()" <<
      endl;
      
  if (visitor<S_msrBarCheck>*
    p =
      dynamic_cast<visitor<S_msrBarCheck>*> (v)) {
        S_msrBarCheck elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors)
          gLogIos <<
            "% ==> Launching msrBarCheck::visitStart()" <<
             endl;
        p->visitStart (elem);
  }
}

void msrBarCheck::acceptOut (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors)
    gLogIos <<
      "% ==> msrBarCheck::acceptOut()" <<
      endl;

  if (visitor<S_msrBarCheck>*
    p =
      dynamic_cast<visitor<S_msrBarCheck>*> (v)) {
        S_msrBarCheck elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors)
          gLogIos <<
            "% ==> Launching msrBarCheck::visitEnd()" <<
            endl;
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
    barCheckAsString () <<
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

string msrBarNumberCheck::barNumberCheckAsString () const
{
  stringstream s;

  s <<
    "BarNumberCheck" <<
    ", next bar number = \"" << fNextBarNumber << "\"";

  return s.str ();
}

void msrBarNumberCheck::acceptIn (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors)
    gLogIos <<
      "% ==> msrBarNumberCheck::acceptIn()" <<
      endl;
      
  if (visitor<S_msrBarNumberCheck>*
    p =
      dynamic_cast<visitor<S_msrBarNumberCheck>*> (v)) {
        S_msrBarNumberCheck elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors)
          gLogIos <<
            "% ==> Launching msrBarNumberCheck::visitStart()" <<
             endl;
        p->visitStart (elem);
  }
}

void msrBarNumberCheck::acceptOut (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors)
    gLogIos <<
      "% ==> msrBarNumberCheck::acceptOut()" <<
      endl;

  if (visitor<S_msrBarNumberCheck>*
    p =
      dynamic_cast<visitor<S_msrBarNumberCheck>*> (v)) {
        S_msrBarNumberCheck elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors)
          gLogIos <<
            "% ==> Launching msrBarNumberCheck::visitEnd()" <<
            endl;
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
    barNumberCheckAsString ();
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

  if (gGeneralOptions->fTraceMeasures)
    gLogIos <<
      "==> Creating a break before measure " << fNextBarNumber <<
      endl;
}

msrLineBreak::~msrLineBreak()
{}

string msrLineBreak::lineBreakAsString () const
{
  stringstream s;

  s <<
    "Line break" <<
    ", next bar number = \"" << fNextBarNumber << "\"";

  return s.str ();
}

void msrLineBreak::acceptIn (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors)
    gLogIos <<
      "% ==> msrLineBreak::acceptIn()" <<
      endl;
      
  if (visitor<S_msrLineBreak>*
    p =
      dynamic_cast<visitor<S_msrLineBreak>*> (v)) {
        S_msrLineBreak elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors)
          gLogIos <<
            "% ==> Launching msrLineBreak::visitStart()" <<
             endl;
        p->visitStart (elem);
  }
}

void msrLineBreak::acceptOut (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors)
    gLogIos <<
      "% ==> msrLineBreak::acceptOut()" <<
      endl;

  if (visitor<S_msrLineBreak>*
    p =
      dynamic_cast<visitor<S_msrLineBreak>*> (v)) {
        S_msrLineBreak elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors)
          gLogIos <<
            "% ==> Launching msrLineBreak::visitEnd()" <<
            endl;
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
    lineBreakAsString () <<
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
  if (gGeneralOptions->fTraceMeasures)
    gLogIos <<
      "==> Creating a page break" <<
      endl;
}

msrPageBreak::~msrPageBreak()
{}

string msrPageBreak::pageBreakAsString () const
{
  stringstream s;

  s <<
    "Page break";

  return s.str ();
}

void msrPageBreak::acceptIn (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors)
    gLogIos <<
      "% ==> msrPageBreak::acceptIn()" <<
      endl;
      
  if (visitor<S_msrPageBreak>*
    p =
      dynamic_cast<visitor<S_msrPageBreak>*> (v)) {
        S_msrPageBreak elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors)
          gLogIos <<
            "% ==> Launching msrPageBreak::visitStart()" <<
             endl;
        p->visitStart (elem);
  }
}

void msrPageBreak::acceptOut (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors)
    gLogIos <<
      "% ==> msrPageBreak::acceptOut()" <<
      endl;

  if (visitor<S_msrPageBreak>*
    p =
      dynamic_cast<visitor<S_msrPageBreak>*> (v)) {
        S_msrPageBreak elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors)
          gLogIos <<
            "% ==> Launching msrPageBreak::visitEnd()" <<
            endl;
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
    pageBreakAsString () <<
    endl;
}

//______________________________________________________________________________
S_msrTuplet msrTuplet::create (
  int      inputLineNumber,
  int      number,
  int      actualNotes,
  int      normalNotes,
  rational notePositionInMeasure)
{
  msrTuplet* o =
    new msrTuplet (
      inputLineNumber,
      number, actualNotes, normalNotes,
      notePositionInMeasure);
  assert(o!=0);
  return o;
}

msrTuplet::msrTuplet (
  int      inputLineNumber,
  int      number,
  int      actualNotes,
  int      normalNotes,
  rational notePositionInMeasure)
    : msrElement (inputLineNumber)
{  
  fTupletNumber = number;
  
  fTupletActualNotes = actualNotes;
  fTupletNormalNotes = normalNotes;
  
  fTupletSoundingWholeNotes = rational (0, 1);
  fTupletDisplayWholeNotes  = rational (0, 1);

  fTupletPositionInMeasure = notePositionInMeasure;
}

msrTuplet::~msrTuplet()
{}

S_msrTuplet msrTuplet::createTupletNewbornClone ()
{
  if (gGeneralOptions->fTraceTuplets) {
    gLogIos <<
      "==> Creating a newborn clone of tuplet '" <<
      tupletAsShortString () <<
      "'" <<
      endl;
  }

  S_msrTuplet
    newbornClone =
      msrTuplet::create (
        fInputLineNumber,
        fTupletNumber,
        fTupletActualNotes,
        fTupletNormalNotes,
        fTupletPositionInMeasure);

  newbornClone->fTupletSoundingWholeNotes =
    fTupletSoundingWholeNotes;

  newbornClone->fTupletDisplayWholeNotes =
    fTupletDisplayWholeNotes;

  newbornClone->fTupletMeasureNumber =
    fTupletMeasureNumber;

  return newbornClone;
}

void msrTuplet::addNoteToTuplet (S_msrNote note)
{
  if (gGeneralOptions->fTraceTuplets)
    gLogIos <<
      "Adding note '" <<
      note->noteAsShortStringWithRawWholeNotes () <<
      // the information is missing to display it the normal way
      "' to tuplet '" <<
      tupletAsShortString () <<
      "'" <<
      endl;

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
  if (gGeneralOptions->fTraceChords || gGeneralOptions->fTraceTuplets)
    gLogIos <<
      "Adding chord '" <<
      chord->chordAsString () <<
      "' to tuplet '" <<
      tupletAsString () <<
      "'" <<
      endl;

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
  if (gGeneralOptions->fTraceTuplets)
    gLogIos <<
      "Adding tuplet '" <<
      tuplet->tupletAsString () <<
      "' to tuplet '" <<
      tupletAsString () <<
      "'" <<
      endl;

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
  if (gGeneralOptions->fTraceTuplets)
    gLogIos <<
      "Adding tuplet '" <<
      tuplet->tupletAsString () <<
      "' to tuplet '" <<
      tupletAsString () <<
      "'" <<
      endl;

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
  if (gGeneralOptions->fTraceTuplets)
    gLogIos <<
      "Removing first note '" <<
      note->noteAsShortStringWithRawWholeNotes () <<
      "' from tuplet '" <<
      tupletAsString () <<
      "'" <<
      endl;

  if (fTupletElements.size ()) {
    S_msrElement
      firstTupletElement =
        fTupletElements.front ();

    for (
      list<S_msrElement>::const_iterator i=fTupletElements.begin();
      i!=fTupletElements.end();
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
    
    msrInternalError (
      inputLineNumber,
      "cannot remove note " <<
      note <<
      " from tuplet " << tupletAsString () <<
      "' in voice \"" <<
      fTupletMeasureUplink->
        getMeasureSegmentUplink ()->
          getSegmentVoiceUplink ()->
            getVoiceName () <<
      "\"," <<
      " since it has not been found");
  }
  
  else {
    msrInternalError (
      inputLineNumber,
      "cannot remove note " <<
      note <<
      " from empty tuplet " <<
      "' in voice \"" <<
      fTupletMeasureUplink->
        getMeasureSegmentUplink ()->
          getSegmentVoiceUplink ()->
            getVoiceName () <<
      "\"," <<
      " since it has not been found");
  }
}

void msrTuplet::setTupletMeasureNumber (string measureNumber) // JMI
{
  fTupletMeasureNumber = measureNumber;

  // propagate measure number to the tuplets elements
  for (
    list<S_msrElement>::const_iterator i = fTupletElements.begin();
    i != fTupletElements.end();
    i++ ) {
    // set tuplet element measure number

  //  SMARTP<msrNote>* note = dynamic_cast<SMARTP<msrNote>*>(&(*tupletMember));

  // BON   SMARTP<msrNote> note = dynamic_cast<msrNote*>(&(*tupletMember));

    if (
      S_msrNote note = dynamic_cast<msrNote*>(&(**i))
      ) {    
      note->setNoteMeasureNumber (measureNumber);
    }
  
    else if (
      S_msrChord chord = dynamic_cast<msrChord*>(&(**i))
      ) {
      chord->setChordMeasureNumber (measureNumber);
    }
    
    else if (
      S_msrTuplet tuplet = dynamic_cast<msrTuplet*>(&(**i))
      ) {
      tuplet->setTupletMeasureNumber (measureNumber);
    }
    
    else {
      msrInternalError (
        fInputLineNumber,
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
    list<S_msrElement>::const_iterator i = fTupletElements.begin();
    i != fTupletElements.end();
    i++ ) {
    // set tuplet element position in measure
    
    if (
      S_msrNote note = dynamic_cast<msrNote*>(&(**i))
      ) {    
      note->
        setNotePositionInMeasure (currentPosition);
        
      currentPosition +=
        note->
          getNoteSoundingWholeNotes ();
    }
  
    else if (
      S_msrChord chord = dynamic_cast<msrChord*>(&(**i))
      ) {
      chord->
        setChordPositionInMeasure (currentPosition);
        
      currentPosition +=
        chord->
          getChordSoundingWholeNotes ();
    }
    
    else if (
      S_msrTuplet tuplet = dynamic_cast<msrTuplet*>(&(**i))
      ) {
      currentPosition =
        tuplet->
          setTupletPositionInMeasure (currentPosition);
    }
    
    else {
      msrInternalError (
        fInputLineNumber,
        "tuplet member should be a note, a chord or another tuplet");
    }

  } // for

  return currentPosition;
}

/* JMI
void msrTuplet::applyDisplayFactorToTupletMembers ()
{
  if (gGeneralOptions->fTraceTuplets) {
    gLogIos <<
      "% ==> applyDisplayFactorToTupletMembers()" <<
      endl;

    idtr++;
    
    gLogIos <<
      "% fTupletActualNotes = " <<
      fTupletActualNotes <<
      ", fTupletNormalNotes = " <<
      fTupletNormalNotes <<
      endl <<
      endl;

    idtr--;
  }

  for (
    list<S_msrElement>::const_iterator i = fTupletElements.begin();
    i != fTupletElements.end();
    i++ ) {
    // apply sounding factor to tuplet element
    
    if (
      S_msrNote note = dynamic_cast<msrNote*>(&(**i))
      ) {    
      note->
        applyTupletMemberSoundingFactorToNote (
          fTupletActualNotes, fTupletNormalNotes);
     }
  
    else if (
      S_msrChord chord = dynamic_cast<msrChord*>(&(**i))
      ) {
      chord->
        applyTupletMemberDisplayFactorToChordMembers (
          fTupletActualNotes, fTupletNormalNotes);
    }
    
    else if (
      S_msrTuplet tuplet = dynamic_cast<msrTuplet*>(&(**i))
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
  if (gGeneralOptions->fTraceTuplets) {
    gLogIos <<
      "unapplySoundingFactorToTupletMembers()" <<
      endl;

    idtr++;
    
    gLogIos <<
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

    idtr--;
  }

  fTupletActualNotes /=
    containingTupletActualNotes;
  fTupletNormalNotes /=
    containingTupletNormalNotes;
}

void msrTuplet::acceptIn (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors)
    gLogIos <<
      "% ==> msrTuplet::acceptIn()" <<
      endl;
      
  if (visitor<S_msrTuplet>*
    p =
      dynamic_cast<visitor<S_msrTuplet>*> (v)) {
        S_msrTuplet elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors)
          gLogIos <<
            "% ==> Launching msrTuplet::visitStart()" <<
             endl;
        p->visitStart (elem);
  }
}

void msrTuplet::acceptOut (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors)
    gLogIos <<
      "% ==> msrTuplet::acceptOut()" <<
      endl;

  if (visitor<S_msrTuplet>*
    p =
      dynamic_cast<visitor<S_msrTuplet>*> (v)) {
        S_msrTuplet elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors)
          gLogIos <<
            "% ==> Launching msrTuplet::visitEnd()" <<
            endl;
        p->visitEnd (elem);
  }
}

void msrTuplet::browseData (basevisitor* v)
{
  for (
    list<S_msrElement>::const_iterator i = fTupletElements.begin();
    i != fTupletElements.end();
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

string msrTuplet::tupletAsShortString () const
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
      iBegin = fTupletElements.begin(),
      iEnd   = fTupletElements.end(),
      i      = iBegin;
    for ( ; ; ) {
      
      if (
        S_msrNote note = dynamic_cast<msrNote*>(&(**i))
        ) {    
        s <<
          note->noteAsShortStringWithRawWholeNotes ();
      }
    
      else if (
        S_msrChord chord = dynamic_cast<msrChord*>(&(**i))
        ) {
        s <<
          chord->chordAsString ();
      }
      
      else if (
        S_msrTuplet tuplet = dynamic_cast<msrTuplet*>(&(**i))
        ) {
        s <<
          tuplet->tupletAsString ();
      }
      
      else {
        msrInternalError (
          fInputLineNumber,
          "tuplet member should be a note, a chord or another tuplet");
      }
  
      if (++i == iEnd) break;
      s << " ";
      
    } // for
  }

  s << "]]";
  
  return s.str ();
}

string msrTuplet::tupletAsString () const
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
      iBegin = fTupletElements.begin(),
      iEnd   = fTupletElements.end(),
      i      = iBegin;
    for ( ; ; ) {

      s << idtr;
      
      if (
        S_msrNote note = dynamic_cast<msrNote*>(&(**i))
        ) {    
        s <<
          note->noteAsShortStringWithRawWholeNotes ();
      }
    
      else if (
        S_msrChord chord = dynamic_cast<msrChord*>(&(**i))
        ) {
        s <<
          chord->chordAsString ();
      }
      
      else if (
        S_msrTuplet tuplet = dynamic_cast<msrTuplet*>(&(**i))
        ) {
        s <<
          tuplet->tupletAsString ();
      }
      
      else {
        msrInternalError (
          fInputLineNumber,
          "tuplet member should be a note, a chord or another tuplet");
      }
  
      if (++i == iEnd) break;
      s << " ";
    } // for
  }

  s << "]]";
  
  return s.str ();
}

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
    ":";
    
  if (fTupletPositionInMeasure.getNumerator () < 0)
    os << "?";
  else
    os << fTupletPositionInMeasure;
  os <<
    endl;
  
  if (fTupletElements.size ()) {
    idtr++;

    list<S_msrElement>::const_iterator
      iBegin = fTupletElements.begin(),
      iEnd   = fTupletElements.end(),
      i      = iBegin;
    for ( ; ; ) {
      os << (*i);
      if (++i == iEnd) break;
  // JMI    os << endl;
    } // for
    
    idtr--;
    
  // JMI  os << endl;
  }
}

//______________________________________________________________________________
S_msrPageGeometry msrPageGeometry::create (
  int           inputLineNumber)
{
  msrPageGeometry* o =
    new msrPageGeometry (
      inputLineNumber);
  assert(o!=0);
  return o;
}

msrPageGeometry::msrPageGeometry (
  int           inputLineNumber)
    : msrElement (inputLineNumber)
{
  fPaperWidth   = -1.0;
  fPaperHeight  = -1.0;
  fTopMargin    = -1.0;
  fBottomMargin = -1.0;
  fLeftMargin   = -1.0;
  fRightMargin  = -1.0;
    
  fBetweenSystemSpace = -1.0;
  fPageTopSpace       = -1.0;

  fMillimeters = -1;
  fTenths      = -1;
}

msrPageGeometry::~msrPageGeometry()
{}

float msrPageGeometry::globalStaffSize () const
{
  if (fMillimeters > 0)
    return fMillimeters * 72.27 / 25.4;
  else
    return 20.0; // LilyPond default
}

void msrPageGeometry::acceptIn (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors)
    gLogIos <<
      "% ==> msrPageGeometry::acceptIn()" <<
      endl;
      
  if (visitor<S_msrPageGeometry>*
    p =
      dynamic_cast<visitor<S_msrPageGeometry>*> (v)) {
        S_msrPageGeometry elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors)
          gLogIos <<
            "% ==> Launching msrPageGeometry::visitStart()" <<
             endl;
        p->visitStart (elem);
  }
}

void msrPageGeometry::acceptOut (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors)
    gLogIos <<
      "% ==> msrPageGeometry::acceptOut()" <<
      endl;

  if (visitor<S_msrPageGeometry>*
    p =
      dynamic_cast<visitor<S_msrPageGeometry>*> (v)) {
        S_msrPageGeometry elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors)
          gLogIos <<
            "% ==> Launching msrPageGeometry::visitEnd()" <<
            endl;
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
  
  idtr++;
  
    const int fieldWidth = 12;

  if (fPaperWidth > 0) {
    os << left <<
      setw (fieldWidth) <<
      "PaperWidth" << " = " <<
      setprecision(4) << fPaperWidth << " cm" <<
      endl;
    emptyGeometry = false;
  }
  
  if (fPaperHeight > 0) {
    os << left <<
      setw (fieldWidth) <<
      "PaperHeight" << " = " <<
      setprecision(4) << fPaperHeight << " cm" <<
      endl;
    emptyGeometry = false;
  }
  
  if (fTopMargin > 0) {
    os << left <<
      setw (fieldWidth) <<
      "TopMargin" << " = " <<
      setprecision(4) << fTopMargin << " cm" <<
      endl;
    emptyGeometry = false;
  }
  
  if (fBottomMargin > 0) {
    os << left <<
      setw (fieldWidth) <<
      "BottomMargin" << " = " <<
      setprecision(4) << fBottomMargin << " cm" <<
      endl;
    emptyGeometry = false;
  }
  
  if (fLeftMargin > 0) {
    os << left <<
      setw (fieldWidth) <<
      "LeftMargin" << " = " <<
      setprecision(4) << fLeftMargin << " cm" <<
      endl;
    emptyGeometry = false;
  }
  
  if (fRightMargin > 0) {
    os << left <<
      setw (fieldWidth) <<
      "RightMargin" << " = " <<
      setprecision(4) << fRightMargin << " cm" <<
      endl;
    emptyGeometry = false;
  }


  if (fMillimeters > 0) {
    os << left <<
      setw (fieldWidth) <<
      "Millimeters" << " = " <<
      setprecision(4) << fMillimeters <<
      endl;
    emptyGeometry = false;
  }

  if (fTenths > 0) {
    os << left <<
      setw (fieldWidth) <<
      "Tenths" << " = " <<
      setprecision(4) << fTenths <<
      endl;
    emptyGeometry = false;
  }

  if (emptyGeometry)
    os <<
      " " << "nothing specified" <<
      endl;
  
  os <<
    endl;

  idtr--;
}

/*
  if (fBetweenSystemSpace > 0) {
    os <<
      "between-system-space = " <<
      setprecision(4) << fBetweenSystemSpace << "\\cm" << endl;
  }

  if (fPageTopSpace > 0) {
    os <<
      "page-top-space = " <<
      setprecision(4) << fPageTopSpace << "\\cm" << endl;
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

void msrCreditWords::acceptIn (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors)
    gLogIos <<
      "% ==> msrCreditWords::acceptIn()" <<
      endl;
      
  if (visitor<S_msrCreditWords>*
    p =
      dynamic_cast<visitor<S_msrCreditWords>*> (v)) {
        S_msrCreditWords elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors)
          gLogIos <<
            "% ==> Launching msrCreditWords::visitStart()" <<
             endl;
        p->visitStart (elem);
  }
}

void msrCreditWords::acceptOut (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors)
    gLogIos <<
      "% ==> msrCreditWords::acceptOut()" <<
      endl;

  if (visitor<S_msrCreditWords>*
    p =
      dynamic_cast<visitor<S_msrCreditWords>*> (v)) {
        S_msrCreditWords elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors)
          gLogIos <<
            "% ==> Launching msrCreditWords::visitEnd()" <<
            endl;
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

  idtr++;

  const int fieldWidth = 26;
  
  os <<
    setw (fieldWidth) <<
    "fCreditWordsContents" << " = \"" <<
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

  idtr--;
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

void msrCredit::acceptIn (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors)
    gLogIos <<
      "% ==> msrCredit::acceptIn()" <<
      endl;
      
  if (visitor<S_msrCredit>*
    p =
      dynamic_cast<visitor<S_msrCredit>*> (v)) {
        S_msrCredit elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors)
          gLogIos <<
            "% ==> Launching msrCredit::visitStart()" <<
             endl;
        p->visitStart (elem);
  }
}

void msrCredit::acceptOut (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors)
    gLogIos <<
      "% ==> msrCredit::acceptOut()" <<
      endl;

  if (visitor<S_msrCredit>*
    p =
      dynamic_cast<visitor<S_msrCredit>*> (v)) {
        S_msrCredit elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors)
          gLogIos <<
            "% ==> Launching msrCredit::visitEnd()" <<
            endl;
        p->visitEnd (elem);
  }
}

void msrCredit::browseData (basevisitor* v)
{
  int n1 = fCreditWordsList.size();
  
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

string msrCredit::creditAsString () const
{
  stringstream s;

  s <<
    "Credit" <<
    ", fCreditPageNumber" << " = " << fCreditPageNumber << ", ";

  if (fCreditWordsList.size ()) {
    s << "[";
    
    vector<S_msrCreditWords>::const_iterator
      iBegin = fCreditWordsList.begin(),
      iEnd   = fCreditWordsList.end(),
      i      = iBegin;
  
    for ( ; ; ) {
      s << (*i)->getCreditWordsContents ();
      if (++i == iEnd) break;
      s << " ";
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

  idtr++;
  
  os <<
    "fCreditPageNumber" << " = " << fCreditPageNumber <<
    endl <<
    endl;

  vector<S_msrCreditWords>::const_iterator
    iBegin = fCreditWordsList.begin(),
    iEnd   = fCreditWordsList.end(),
    i      = iBegin;

  for ( ; ; ) {
    os << (*i);
    if (++i == iEnd) break;
// JMI    os << endl;
  } // for
  
  idtr--;
}

//______________________________________________________________________________

S_msrVarValAssoc msrVarValAssoc::create (
  int           inputLineNumber,
  string        variableName,
  string        value)
{
  msrVarValAssoc* o =
    new msrVarValAssoc(
      inputLineNumber,
      variableName, value);
  assert(o!=0);
  return o;
}

msrVarValAssoc::msrVarValAssoc (
  int           inputLineNumber,
  string        variableName,
  string        value)
    : msrElement (inputLineNumber)
{
  fVariableName    = variableName;
  fVariableValue   = value;
}

msrVarValAssoc::~msrVarValAssoc()
{}

void msrVarValAssoc::acceptIn (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors)
    gLogIos <<
      "% ==> msrVarValAssoc::acceptIn()" <<
      endl;
      
  if (visitor<S_msrVarValAssoc>*
    p =
      dynamic_cast<visitor<S_msrVarValAssoc>*> (v)) {
        S_msrVarValAssoc elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors)
          gLogIos <<
            "% ==> Launching msrVarValAssoc::visitStart()" <<
             endl;
        p->visitStart (elem);
  }
}

void msrVarValAssoc::acceptOut (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors)
    gLogIos <<
      "% ==> msrVarValAssoc::acceptOut()" <<
      endl;

  if (visitor<S_msrVarValAssoc>*
    p =
      dynamic_cast<visitor<S_msrVarValAssoc>*> (v)) {
        S_msrVarValAssoc elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors)
          gLogIos <<
            "% ==> Launching msrVarValAssoc::visitEnd()" <<
            endl;
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
  
  idtr++;

  // escape quotes if any
  string variableName;

  for_each (
    fVariableName.begin(),
    fVariableName.end(),
    stringQuoteEscaper (variableName));
    
  string variableValue;

  for_each (
    fVariableValue.begin(),
    fVariableValue.end(),
    stringQuoteEscaper (variableValue));

  // print resulting strings
  os <<
    "variable name : " <<
    "\"" << variableName << "\"" <<
    endl <<
    "variable value: " <<
    "\"" << variableValue << "\"" <<
    endl <<
    endl;
  
  idtr--;
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

void msrLayout::acceptIn (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors)
    gLogIos <<
      "% ==> msrLayout::acceptIn()" <<
      endl;
      
  if (visitor<S_msrLayout>*
    p =
      dynamic_cast<visitor<S_msrLayout>*> (v)) {
        S_msrLayout elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors)
          gLogIos <<
            "% ==> Launching msrLayout::visitStart()" <<
             endl;
        p->visitStart (elem);
  }
}

void msrLayout::acceptOut (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors)
    gLogIos <<
      "% ==> msrLayout::acceptOut()" <<
      endl;

  if (visitor<S_msrLayout>*
    p =
      dynamic_cast<visitor<S_msrLayout>*> (v)) {
        S_msrLayout elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors)
          gLogIos <<
            "% ==> Launching msrLayout::visitEnd()" <<
            endl;
        p->visitEnd (elem);
  }
}

void msrLayout::browseData (basevisitor* v)
{
  int n1 = fVarValAssocs.size();
  
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

  idtr++;

  int n1 = fVarValAssocs.size();
  
  for (int i = 0; i < n1; i++ ) {
    os << fVarValAssocs [i];
  } // for

    /* JMI
  int n2 = fMsrSchemeVarValAssocs.size();
  for (int i = 0; i < n2; i++ ) {
    os << fMsrSchemeVarValAssocs[i];
  } // for
  */
  
  idtr--;
}

//______________________________________________________________________________
S_msrClef msrClef::create (
  int          inputLineNumber,
  msrClefKind  clefKind)
{
  msrClef* o =
    new msrClef (
      inputLineNumber, clefKind);
  assert(o!=0);
  return o;
}

msrClef::msrClef (
  int          inputLineNumber,
  msrClefKind  clefKind)
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

bool msrClef::isEqualTo (S_msrClef otherClef) const // JMI
{
  if (fClefKind != otherClef->fClefKind)
    return false;
    
  switch (fClefKind) {
    case msrClef::kTablature4Clef:
    case msrClef::kTablature5Clef:
    case msrClef::kTablature6Clef:
    case msrClef::kTablature7Clef:
      return false;
      break;
    default:
      return false;
  } // switch
}

void msrClef::acceptIn (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors)
    gLogIos <<
      "% ==> msrClef::acceptIn()" <<
      endl;
      
  if (visitor<S_msrClef>*
    p =
      dynamic_cast<visitor<S_msrClef>*> (v)) {
        S_msrClef elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors)
          gLogIos <<
            "% ==> Launching msrClef::visitStart()" <<
             endl;
        p->visitStart (elem);
  }
}

void msrClef::acceptOut (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors)
    gLogIos <<
      "% ==> msrClef::acceptOut()" <<
      endl;

  if (visitor<S_msrClef>*
    p =
      dynamic_cast<visitor<S_msrClef>*> (v)) {
        S_msrClef elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors)
          gLogIos <<
            "% ==> Launching msrClef::visitEnd()" <<
            endl;
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

string msrClef::clefAsString () const
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
  } // switch

  s <<
    "\", line " << fInputLineNumber;

  return s.str ();
}

void msrClef::print (ostream& os)
{
  os <<
    clefAsString () <<
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
  if (gGeneralOptions->fTraceKeys) {
    gLogIos <<
      "==> Creating Humdrum/Scot key item" <<
      ", line = " << inputLineNumber <<
      endl;
  }

  fKeyDiatonicPitch = k_NoDiatonicPitch;
  fKeyAlteration    = k_NoAlteration;
  fKeyOctave        = -1; // actual MusicXML octaves are non-negative
}

msrHumdrumScotKeyItem::~msrHumdrumScotKeyItem()
{}

void msrHumdrumScotKeyItem::setKeyItemDiatonicPitch (
  msrDiatonicPitch diatonicPitch)
{
  if (gGeneralOptions->fTraceKeys) {
    gLogIos <<
      "Setting Humdrum/Scot key item diatonic pitch to '" <<
      msrDiatonicPitchAsString (diatonicPitch) <<
      "'" <<
      endl;
  }
  
  fKeyDiatonicPitch = diatonicPitch;
}

void msrHumdrumScotKeyItem::setKeyItemAlteration (
  msrAlteration alteration)
{
  if (gGeneralOptions->fTraceKeys) {
    gLogIos <<
      "Setting Humdrum/Scot key item alteration to '" <<
      msrAlterationAsString (alteration) <<
      "'" <<
      endl;
  }
  
  fKeyAlteration = alteration;
}

void msrHumdrumScotKeyItem::setKeyItemOctave (int keyOctave)
{
  if (gGeneralOptions->fTraceKeys) {
    gLogIos <<
      "Setting Humdrum/Scot key item octave to '" <<
      keyOctave <<
      "'" <<
      endl;
  }
  
  fKeyOctave = keyOctave;
}

void msrHumdrumScotKeyItem::acceptIn (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors)
    gLogIos <<
      "% ==> msrHumdrumScotKeyItem::acceptIn()" <<
      endl;
      
  if (visitor<S_msrHumdrumScotKeyItem>*
    p =
      dynamic_cast<visitor<S_msrHumdrumScotKeyItem>*> (v)) {
        S_msrHumdrumScotKeyItem elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors)
          gLogIos <<
            "% ==> Launching msrHumdrumScotKeyItem::visitStart()" <<
             endl;
        p->visitStart (elem);
  }
}

void msrHumdrumScotKeyItem::acceptOut (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors)
    gLogIos <<
      "% ==> msrHumdrumScotKeyItem::acceptOut()" <<
      endl;

  if (visitor<S_msrHumdrumScotKeyItem>*
    p =
      dynamic_cast<visitor<S_msrHumdrumScotKeyItem>*> (v)) {
        S_msrHumdrumScotKeyItem elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors)
          gLogIos <<
            "% ==> Launching msrHumdrumScotKeyItem::visitEnd()" <<
            endl;
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

string msrHumdrumScotKeyItem::humdrumScotKeyItemAsString () const
{
  stringstream s;

  s <<
    "HumdrumScotKeyItem" <<
    ", KeyDiatonicPitch" << ": " <<
    msrDiatonicPitchAsString (fKeyDiatonicPitch) <<
    ", KeyAlteration" << ": " <<
    msrAlterationAsString (fKeyAlteration) <<
    ", KeyOctave" << ": " << fKeyOctave <<
    ", line " << fInputLineNumber;
     
  return s.str ();
}

void msrHumdrumScotKeyItem::print (ostream& os)
{
  os <<
    humdrumScotKeyItemAsString () <<
    endl;
}

//______________________________________________________________________________
S_msrKey msrKey::createTraditional (
  int                  inputLineNumber,
  msrQuarterTonesPitch keyTonicQuarterTonesPitch,
  msrKeyModeKind       keyModeKind,
  int                  keyCancel)
{
  msrKey* o =
    new msrKey (
      inputLineNumber,
      keyTonicQuarterTonesPitch, keyModeKind,
      keyCancel);
  assert (o!=0);

  return o;
}

S_msrKey msrKey::createHumdrumScot (
  int                  inputLineNumber)
{
  msrKey* o =
    new msrKey (
      inputLineNumber);
  assert (o!=0);

  return o;
}

msrKey::msrKey ( // for traditional keys
  int                  inputLineNumber,
  msrQuarterTonesPitch keyTonicQuarterTonesPitch,
  msrKeyModeKind       keyModeKind,
  int                  keyCancel)
    : msrElement (inputLineNumber)
{
  // this is a traditional key
  fKeyKind = kTraditionalKind;
  
  fKeyTonicQuarterTonesPitch = keyTonicQuarterTonesPitch;
  fKeyModeKind               = keyModeKind;
  
  fKeyCancel     = keyCancel;

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

bool msrKey::isEqualTo (S_msrKey otherKey) const // JMI
{
  if (fKeyKind != otherKey->fKeyKind)
    return false;
    
  switch (fKeyKind) {
    case msrKey::kTraditionalKind:
    case msrKey::kHumdrumScotKind:
      return false;
      break;
    default:
      return false;
  } // switch
}

void msrKey::appendHumdrumScotKeyItem (
  S_msrHumdrumScotKeyItem item)
{
  if (gGeneralOptions->fTraceKeys) {
    gLogIos <<
      "Append item '" <<
      item->humdrumScotKeyItemAsString () <<
      "' to key '" <<
      "'" <<
      endl;
    }

  // have key items octaves been specified?
  if (item->getKeyItemOctave () >= 0)
    fKeyItemsOctavesAreSpecified = true;;

  // append the item to the vector
  fHumdrumScotKeyItemsVector.insert (
    fHumdrumScotKeyItemsVector.end(), item);
}

void msrKey::acceptIn (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors)
    gLogIos <<
      "% ==> msrKey::acceptIn()" <<
      endl;
      
  if (visitor<S_msrKey>*
    p =
      dynamic_cast<visitor<S_msrKey>*> (v)) {
        S_msrKey elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors)
          gLogIos <<
            "% ==> Launching msrKey::visitStart()" <<
             endl;
        p->visitStart (elem);
  }
}

void msrKey::acceptOut (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors)
    gLogIos <<
      "% ==> msrKey::acceptOut()" <<
      endl;

  if (visitor<S_msrKey>*
    p =
      dynamic_cast<visitor<S_msrKey>*> (v)) {
        S_msrKey elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors)
          gLogIos <<
            "% ==> Launching msrKey::visitEnd()" <<
            endl;
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

string msrKey::keyAsString () const
{
  stringstream s;

  s <<
    "Key" <<
    ", " << keyKindAsString (fKeyKind) <<
    ", ";

  switch (fKeyKind) {
    case msrKey::kTraditionalKind:
      s <<
        msrQuarterTonesPitchAsString (
          gMsrOptions->fMsrQuarterTonesPitchesLanguage,
          fKeyTonicQuarterTonesPitch) <<
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
        msrQuarterTonesPitchAsString (
          gMsrOptions->fMsrQuarterTonesPitchesLanguage,
          fKeyTonicQuarterTonesPitch) <<
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
          
        idtr++;
        
        vector<S_msrHumdrumScotKeyItem>::const_iterator
          iBegin = fHumdrumScotKeyItemsVector.begin(),
          iEnd   = fHumdrumScotKeyItemsVector.end(),
          i      = iBegin;
          
        for ( ; ; ) {
          os << (*i);
          if (++i == iEnd) break;
    // JMI     os << endl;
        } // for
        
        idtr--;
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
  
  if (gGeneralOptions->fTraceTimes) {
    gLogIos <<
      "==> Creating time item" <<
      ", line = " << inputLineNumber <<
      endl;
  }
}

msrTimeItem::~msrTimeItem()
{}

void msrTimeItem::appendBeatsNumber (int beatsNumber)
{
  if (gGeneralOptions->fTraceTimes) {
    gLogIos <<
      "Append beat number '" <<
      beatsNumber <<
      "' to time '" << // JMI
      "'" <<
      endl;
    }

  fTimeBeatsNumbersVector.insert (
    fTimeBeatsNumbersVector.end(),
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

void msrTimeItem::acceptIn (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors)
    gLogIos <<
      "% ==> msrTimeItem::acceptIn()" <<
      endl;
      
  if (visitor<S_msrTimeItem>*
    p =
      dynamic_cast<visitor<S_msrTimeItem>*> (v)) {
        S_msrTimeItem elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors)
          gLogIos <<
            "% ==> Launching msrTimeItem::visitStart()" <<
             endl;
        p->visitStart (elem);
  }
}

void msrTimeItem::acceptOut (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors)
    gLogIos <<
      "% ==> msrTimeItem::acceptOut()" <<
      endl;

  if (visitor<S_msrTimeItem>*
    p =
      dynamic_cast<visitor<S_msrTimeItem>*> (v)) {
        S_msrTimeItem elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors)
          gLogIos <<
            "% ==> Launching msrTimeItem::visitEnd()" <<
            endl;
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

string msrTimeItem::timeItemAsString () const
{
  stringstream s;

  s <<
    "TimeItem" <<
    " beats numbers: ";

  int vectorSize =
    fTimeBeatsNumbersVector.size ();

  if (vectorSize) {
    for (int i = 0; i < vectorSize; i++) {
      s <<
        fTimeBeatsNumbersVector [i];

      if (i != vectorSize - 1)
        s <<
          " ";
      } // for
  }
  
  else {
    msrInternalError (
      fInputLineNumber,
      "time item beats numbers vector is empty");
  }

  s <<
    ", beat value: " << fTimeBeatValue <<
    ", line " << fInputLineNumber;
     
  return s.str ();
}

void msrTimeItem::print (ostream& os)
{
  os <<
    timeItemAsString () <<
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

bool msrTime::isEqualTo (S_msrTime otherTime) const // JMI
{
  bool result = false;
  
  if (fTimeSymbolKind != otherTime->fTimeSymbolKind)
    result = false;
    
    /* JMI
  switch (fTimeSymbolKind) {
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
      return false;
      break;
    default:
      return false;
  } // switch
  */

  return result;
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
  if (gGeneralOptions->fTraceTimes) {
    gLogIos <<
      "Append item '" <<
      timeItem->timeItemAsString () <<
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
    fTimeItemsVector.end(), timeItem);
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
    gLogIos <<
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
      gLogIos <<
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
      fInputLineNumber,
      "time items vector is empty");
  }

  // rationalize result
  result.rationalise ();

  // return it
  return result;
}

void msrTime::acceptIn (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors)
    gLogIos <<
      "% ==> msrTime::acceptIn()" <<
      endl;
      
  if (visitor<S_msrTime>*
    p =
      dynamic_cast<visitor<S_msrTime>*> (v)) {
        S_msrTime elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors)
          gLogIos <<
            "% ==> Launching msrTime::visitStart()" <<
             endl;
        p->visitStart (elem);
  }
}

void msrTime::acceptOut (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors)
    gLogIos <<
      "% ==> msrTime::acceptOut()" <<
      endl;

  if (visitor<S_msrTime>*
    p =
      dynamic_cast<visitor<S_msrTime>*> (v)) {
        S_msrTime elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors)
          gLogIos <<
            "% ==> Launching msrTime::visitEnd()" <<
            endl;
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

string msrTime::timeAsShortString () const
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

  return timeAsString ();
}

string msrTime::timeAsString () const
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
      iBegin = fTimeItemsVector.begin(),
      iEnd   = fTimeItemsVector.end(),
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
        fInputLineNumber,
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
      
    idtr++;
    
    vector<S_msrTimeItem>::const_iterator
      iBegin = fTimeItemsVector.begin(),
      iEnd   = fTimeItemsVector.end(),
      i      = iBegin;
      
    for ( ; ; ) {
      os << (*i);
      if (++i == iEnd) break;
 // JMI     os << endl;
    } // for
    
    idtr--;
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

  if (gGeneralOptions->fTraceTranspositions) {
    gLogIos <<
      "==> Creating transpose '" <<
      transposeAsString () <<
      "'" <<
      endl;
  }
}

msrTranspose::~msrTranspose()
{}

bool msrTranspose::isEqualTo (S_msrTranspose otherTranspose) const
{
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

void msrTranspose::acceptIn (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors)
    gLogIos <<
      "% ==> msrTranspose::acceptIn()" <<
      endl;
      
  if (visitor<S_msrTranspose>*
    p =
      dynamic_cast<visitor<S_msrTranspose>*> (v)) {
        S_msrTranspose elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors)
          gLogIos <<
            "% ==> Launching msrTranspose::visitStart()" <<
             endl;
        p->visitStart (elem);
  }
}

void msrTranspose::acceptOut (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors)
    gLogIos <<
      "% ==> msrTranspose::acceptOut()" <<
      endl;

  if (visitor<S_msrTranspose>*
    p =
      dynamic_cast<visitor<S_msrTranspose>*> (v)) {
        S_msrTranspose elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors)
          gLogIos <<
            "% ==> Launching msrTranspose::visitEnd()" <<
            endl;
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

string msrTranspose::transposeAsString () const
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

  idtr++;
  
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
    
  idtr--;
}

//______________________________________________________________________________
S_msrWords msrWords::create (
  int                      inputLineNumber,
  msrWordsPlacementKind    wordsPlacementKind,
  string                   wordsContents,
  msrJustifyKind           wordsJustifyKind,
  msrVerticalAlignmentKind wordsVerticalAlignmentKind,
  msrFontStyle             wordsFontStyle,
  msrFontSize              wordsFontSize,
  msrFontWeight            wordsFontWeight,
  msrWordsXMLLangKind      wordsXMLLangKind)
{
  msrWords* o =
    new msrWords (
      inputLineNumber,
      wordsPlacementKind,
      wordsContents,
      wordsJustifyKind,
      wordsVerticalAlignmentKind,
      wordsFontStyle,
      wordsFontSize,
      wordsFontWeight,
      wordsXMLLangKind);
  assert(o!=0);
  return o;
}

msrWords::msrWords (
  int                      inputLineNumber,
  msrWordsPlacementKind    wordsPlacementKind,
  string                   wordsContents,
  msrJustifyKind           wordsJustifyKind,
  msrVerticalAlignmentKind wordsVerticalAlignmentKind,
  msrFontStyle             wordsFontStyle,
  msrFontSize              wordsFontSize,
  msrFontWeight            wordsFontWeight,
  msrWordsXMLLangKind      wordsXMLLangKind)
    : msrElement (inputLineNumber)
{
  fWordsPlacementKind  = wordsPlacementKind;
  
  fWordsContents       = wordsContents;
  
  fWordsJustifyKind    = wordsJustifyKind;
  fWordsVerticalAlignmentKind = wordsVerticalAlignmentKind;

  fWordsFontStyle      = wordsFontStyle;
  fWordsFontSize       = wordsFontSize;
  fWordsFontWeight     = wordsFontWeight;
  
  fWordsXMLLangKind    = wordsXMLLangKind;
}

msrWords::~msrWords()
{}

void msrWords::acceptIn (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors)
    gLogIos <<
      "% ==> msrWords::acceptIn()" <<
      endl;
      
  if (visitor<S_msrWords>*
    p =
      dynamic_cast<visitor<S_msrWords>*> (v)) {
        S_msrWords elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors)
          gLogIos <<
            "% ==> Launching msrWords::visitStart()" <<
             endl;
        p->visitStart (elem);
  }
}

void msrWords::acceptOut (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors)
    gLogIos <<
      "% ==> msrWords::acceptOut()" <<
      endl;

  if (visitor<S_msrWords>*
    p =
      dynamic_cast<visitor<S_msrWords>*> (v)) {
        S_msrWords elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors)
          gLogIos <<
            "% ==> Launching msrWords::visitEnd()" <<
            endl;
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

string msrWords::wordsPlacementKindAsString (
  msrWordsPlacementKind wordsPlacementKind)
{
  string result;

  switch (wordsPlacementKind) {
    case msrWords::kWordsPlacementAbove:
      result = "above";
      break;
    case msrWords::kWordsPlacementBelow:
      result = "below";
      break;
  } // switch

  return result;
}

string msrWords::wordsJustifyKindAsString () const
{
  return msrJustifyKindAsString (fWordsJustifyKind);
}

string msrWords::wordsVerticalAlignmentKindAsString () const
{
  return msrVerticalAlignmentKindAsString (fWordsVerticalAlignmentKind);
}

string msrWords::wordsFontStyleAsString () const
{
  return msrFontStyleAsString (fWordsFontStyle);
}

string msrWords::wordsFontSizeAsString () const
{
  return msrFontSizeAsString (fWordsFontSize);
}

string msrWords::wordsFontWeightAsString () const
{
  return msrFontWeightAsString (fWordsFontWeight);
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

string msrWords::wordsAsString () const
{
  stringstream s;

  s <<
    "Words" << " " <<
    fWordsContents << ", placement = " <<
    wordsPlacementKindAsString (fWordsPlacementKind);

  return s.str ();
}

void msrWords::print (ostream& os)
{
// JMI  os << wordsAsString () << endl;

  os <<
    "Words" " \"" << fWordsContents << "\"" <<
    endl;

  idtr++;

  const int fieldWidth = 16;

  os << left <<
    setw (fieldWidth) <<
    "Placement" << " = " <<
    wordsPlacementKindAsString (fWordsPlacementKind) <<
    endl <<
    setw (fieldWidth) <<
    "WordsJustifyKind" << " = " <<
    msrJustifyKindAsString (fWordsJustifyKind) <<
    endl <<
    setw (fieldWidth) <<
    "WordsVerticalAlignmentKind" << " = " <<
    msrVerticalAlignmentKindAsString (fWordsVerticalAlignmentKind) <<
    endl <<
    setw (fieldWidth) <<
    "WordsFontStyle" << " = " <<
    msrFontStyleAsString (fWordsFontStyle) <<
    endl <<
    setw (fieldWidth) <<
    "WordsFontSize" << " = " <<
    msrFontSizeAsString (fWordsFontSize) <<
    endl <<
    setw (fieldWidth) <<
    "WordsFontWeight" << " = " <<
    msrFontWeightAsString (fWordsFontWeight) <<
    endl <<
    setw (fieldWidth) <<
    "WordsFontXMLLang" << " = " <<
    msrWordsXMLLangKindAsString (fWordsXMLLangKind) <<
    endl;
  
  idtr--;
}

//______________________________________________________________________________
S_msrTempo msrTempo::create (
  int           inputLineNumber,
  int           tempoUnit,
  int           perMinute)
{
  msrTempo* o =
    new msrTempo (
      inputLineNumber, tempoUnit, perMinute);
  assert(o!=0);
  return o;
}

msrTempo::msrTempo (
  int           inputLineNumber,
  int           tempoUnit,
  int           perMinute)
    : msrElement (inputLineNumber)
{
  fTempoIndication = "";
  
  fTempoUnit = tempoUnit;
  fPerMinute = perMinute;
}

msrTempo::~msrTempo()
{}

void msrTempo::setTempoIndication (string indication)
{
  if (gMsrOptions->fTraceMsrVisitors)
    gLogIos <<
      "Setting indication of tempo " <<
      fTempoUnit << " = " << fPerMinute <<
      " to \"" << indication << "\"" <<
      endl;
      
  fTempoIndication = indication;
}

void msrTempo::acceptIn (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors)
    gLogIos <<
      "% ==> msrTempo::acceptIn()" <<
      endl;
      
  if (visitor<S_msrTempo>*
    p =
      dynamic_cast<visitor<S_msrTempo>*> (v)) {
        S_msrTempo elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors)
          gLogIos <<
            "% ==> Launching msrTempo::visitStart()" <<
             endl;
        p->visitStart (elem);
  }
}

void msrTempo::acceptOut (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors)
    gLogIos <<
      "% ==> msrTempo::acceptOut()" <<
      endl;

  if (visitor<S_msrTempo>*
    p =
      dynamic_cast<visitor<S_msrTempo>*> (v)) {
        S_msrTempo elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors)
          gLogIos <<
            "% ==> Launching msrTempo::visitEnd()" <<
            endl;
        p->visitEnd (elem);
  }
}


void msrTempo::browseData (basevisitor* v)
{}

ostream& operator<< (ostream& os, const S_msrTempo& elt)
{
  elt->print (os);
  return os;
}

string msrTempo::tempoAsString () const
{
  stringstream s;

  s <<
    "Tempo" << " " <<
    ", indication = \"" << fTempoIndication << "\"" <<
    ", " << fTempoUnit << " = " << fPerMinute;

  return s.str ();
}

void msrTempo::print (ostream& os)
{
  os << tempoAsString () << endl;
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
      syllableKind, syllableExtendKind,
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
    syllableStanzaUplink != 0,
    "syllableStanzaUplink is null");
    
  // set syllable's stanza uplink
  fSyllableStanzaUplink =
    syllableStanzaUplink;
    
  fSyllableKind = syllableKind;
  
  fSyllableWholeNotes = syllableWholeNotes;

  fSyllableExtendKind = syllableExtendKind;
  
  // fSyllableNoteUplink will be set
  // by setSyllableNoteUplink() later
}

msrSyllable::~msrSyllable()
{}

S_msrSyllable msrSyllable::createSyllableNewbornClone (
  S_msrPart containingPart)
{
  if (gGeneralOptions->fTraceLyrics) {
    gLogIos <<
      "==> Creating a newborn clone of syllable '" <<
      syllableAsString () <<
      "'" <<
      endl;
  }

  // sanity check
  msrAssert(
    containingPart != 0,
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
    list<string>::const_iterator i = fSyllableTextsList.begin();
    i!=fSyllableTextsList.end();
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
  if (gGeneralOptions->fTraceLyrics) {
    gLogIos <<
      "==> Creating a newborn clone of syllable '" <<
      syllableAsString () <<
      "'" <<
      endl;
  }

  // sanity check
  msrAssert(
    containingPart != 0,
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
    list<string>::const_iterator i = fSyllableTextsList.begin();
    i!=fSyllableTextsList.end();
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
      iBegin = textsList.begin(),
      iEnd   = textsList.end(),
      i      = iBegin;
      
    for ( ; ; ) {
      os << quoteStringIfNonAlpha (*i);
      if (++i == iEnd) break;
      os << ", ";
    } // for
  }

  os << "]";
} 

void msrSyllable::appendLyricTextToSyllable (string text)
{
  if (gGeneralOptions->fTraceLyrics) {
    gLogIos <<
      "Appending text '" <<
      text <<
      "' to the texts list of syllable '" <<
      syllableAsString () <<
      "'" <<
      endl;
  }

  fSyllableTextsList.push_back (
    text);
}

void msrSyllable::setSyllableNoteUplink (S_msrNote note)
{
  fSyllableNoteUplink = note;

  // register syllable in note if its text list is not empty
  if (fSyllableTextsList.size ())
     note->
      appendSyllableToNote (this); // JMI
  
  if (gGeneralOptions->fTraceLyrics) {
    gLogIos <<
      "Setting syllable note uplink for:" <<
      endl;

    idtr++;
    
    gLogIos <<
      syllableAsString () <<
    // JMI    "to '" << note->noteAsString () <<
      ", line " << note->getInputLineNumber () <<
      endl;

    idtr--;
      
/*
    if (note) { // JMI
      gLogIos <<
        note->noteAsString () <<
        ", line " << note->getInputLineNumber ();
    }
    else {
      gLogIos << "none";
    }
    
    gLogIos <<
      endl;
      */
  }
}

string msrSyllable::syllableWholeNotesAsMsrString ()
{
  return
    wholeNotesAsMsrString (
      fInputLineNumber,
      fSyllableWholeNotes);
}

void msrSyllable::acceptIn (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors)
    gLogIos <<
      "% ==> msrSyllable::acceptIn()" <<
      endl;
      
  if (visitor<S_msrSyllable>*
    p =
      dynamic_cast<visitor<S_msrSyllable>*> (v)) {
        S_msrSyllable elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors)
          gLogIos <<
            "% ==> Launching msrSyllable::visitStart()" <<
             endl;
        p->visitStart (elem);
  }
}

void msrSyllable::acceptOut (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors)
    gLogIos <<
      "% ==> msrSyllable::acceptOut()" <<
      endl;

  if (visitor<S_msrSyllable>*
    p =
      dynamic_cast<visitor<S_msrSyllable>*> (v)) {
        S_msrSyllable elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors)
          gLogIos <<
            "% ==> Launching msrSyllable::visitEnd()" <<
            endl;
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
    case msrSyllable::kSingleSyllable:
      result = "single";
      break;
    case msrSyllable::kBeginSyllable:
      result = "begin";
      break;
    case msrSyllable::kMiddleSyllable:
      result = "middle";
      break;
    case msrSyllable::kEndSyllable:
      result = "end";
      break;
      
    case msrSyllable::kRestSyllable:
      result = "rest";
      break;
      
    case msrSyllable::kSkipSyllable:
      result = "skip";
      break;
      
    case msrSyllable::kMelismaFirstSyllable:
      result = "melisma first";
      break;
      
    case msrSyllable::kMelismaOtherSyllable:
      result = "melisma";
      break;
      
    case msrSyllable::kSlurSyllable:
      result = "slur";
      break;
    case msrSyllable::kSlurBeyondEndSyllable:
      result = "slur beyond end";
      break;
      
    case msrSyllable::kLigatureSyllable:
      result = "ligature";
      break;
    case msrSyllable::kLigatureBeyondEndSyllable:
      result = "ligature beyond end";
      break;
      
    case msrSyllable::kTiedSyllable:
      result = "tied";
      break;
      
    case msrSyllable::kBarcheckSyllable:
      result = "bar check";
      break;
      
    case msrSyllable::kBarNumberCheckSyllable:
      result = "barnumber check";
      break;
      
    case msrSyllable::kLineBreakSyllable:
      result = "line break";
      break;
      
    case msrSyllable::kPageBreakSyllable:
      result = "page break";
      break;
      
    case msrSyllable::k_NoSyllable:
      result = "k_NoSyllable ???";
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
    case msrSyllable::kStandaloneSyllableExtend:
      result = "standalone";
      break;
    case msrSyllable::kStartSyllableExtend:
      result = "start";
      break;
    case msrSyllable::kContinueSyllableExtend:
      result = "continue";
      break;
    case msrSyllable::kStopSyllableExtend:
      result = "stop";
      break;
    case msrSyllable::k_NoSyllableExtend:
      result = "none";
      break;
  } // switch

  return result;
}

string msrSyllable::syllableNoteUplinkAsString () const
{
  string result;

  if (fSyllableNoteUplink)
    result =
      "% ==> "
        +
      fSyllableNoteUplink->noteAsString ();
  else
    result =
      "no syllable note uplink";

  return result;
}

string msrSyllable::syllableAsString ()
{
  stringstream s;
  
  switch (fSyllableKind) {
    case kSingleSyllable:
      s <<
        "single" <<
        ", texts list: ";

      writeTextsList (
        fSyllableTextsList,
        s);

      s <<
        ":" << syllableWholeNotesAsMsrString () <<
        " (" << fSyllableWholeNotes << ")" <<
        ", line " << fInputLineNumber <<
        ", " <<
        syllableNoteUplinkAsString () <<
        ", NoteSyllableExtendKind: " <<
        syllableExtendKindAsString (fSyllableExtendKind);
      break;
      
    case kBeginSyllable:
      s << 
        "begin" <<
        ", texts list: ";

      writeTextsList (
        fSyllableTextsList,
        s);

      s <<
         ":" << syllableWholeNotesAsMsrString () <<
        " (" << fSyllableWholeNotes << ")" <<
        ", line " << fInputLineNumber <<
        ", " <<
        syllableNoteUplinkAsString () <<
        ", NoteSyllableExtendKind: " <<
        syllableExtendKindAsString (fSyllableExtendKind);
      break;
      
    case kMiddleSyllable:
      s << 
        "middle" <<
        ", texts list: ";

      writeTextsList (
        fSyllableTextsList,
        s);

      s <<
        ":" << syllableWholeNotesAsMsrString () <<
        " (" << fSyllableWholeNotes << ")" <<
        ", line " << fInputLineNumber <<
        ", " <<
        syllableNoteUplinkAsString () <<
        ", NoteSyllableExtendKind: " <<
        syllableExtendKindAsString (fSyllableExtendKind);
      break;
      
    case kEndSyllable:
      s << 
        "end" <<
        ", texts list: ";

      writeTextsList (
        fSyllableTextsList,
        s);

      s <<
        ":" << syllableWholeNotesAsMsrString () <<
        " (" << fSyllableWholeNotes << ")" <<
        ", line " << fInputLineNumber <<
        ", " <<
        syllableNoteUplinkAsString () <<
        ", NoteSyllableExtendKind: " <<
        syllableExtendKindAsString (fSyllableExtendKind);
      break;
      
    case kRestSyllable:
      s <<
        "rest" <<
        ", texts list: ";

      writeTextsList (
        fSyllableTextsList,
        s);

      s <<
        ":" << syllableWholeNotesAsMsrString () <<
        " (" << fSyllableWholeNotes << ")" <<
        ", line " << fInputLineNumber <<
        ", " <<
        syllableNoteUplinkAsString () <<
        ", NoteSyllableExtendKind: " <<
        syllableExtendKindAsString (fSyllableExtendKind);
      break;
      
    case kSkipSyllable:
      s << 
        "skip" << ":" << syllableWholeNotesAsMsrString () <<
        " (" << fSyllableWholeNotes << ")" <<
        ", line " << fInputLineNumber;
      break;
      
    case kMelismaFirstSyllable:
      s << 
        "melisma first" << ":" << syllableWholeNotesAsMsrString () <<
        " (" << fSyllableWholeNotes << ")" <<
        ", line " << fInputLineNumber;
      break;
      
    case kMelismaOtherSyllable:
      s << 
        "melisma other" << ":" << syllableWholeNotesAsMsrString () <<
        " (" << fSyllableWholeNotes << ")" <<
        ", line " << fInputLineNumber;
      break;
      
    case kSlurSyllable:
      s << 
        "slur" << ":" << syllableWholeNotesAsMsrString () <<
        " (" << fSyllableWholeNotes << ")" <<
        ", line " << fInputLineNumber <<
        ", " <<
        syllableNoteUplinkAsString ();
      break;
      
    case kSlurBeyondEndSyllable:
      s << 
        "slur beyond end" << ":" <<
        syllableWholeNotesAsMsrString () <<
        ", line " << fInputLineNumber <<
        ", " <<
        syllableNoteUplinkAsString ();
      break;
      
    case kLigatureSyllable:
      s << 
        "ligature" << ":" <<
        syllableWholeNotesAsMsrString () <<
        " (" << fSyllableWholeNotes << ")" <<
        ", line " << fInputLineNumber <<
        ", " <<
        syllableNoteUplinkAsString ();
      break;
      
    case kLigatureBeyondEndSyllable:
      s << 
        "ligature beyond end" << ":" <<
        syllableWholeNotesAsMsrString () <<
        ", line " << fInputLineNumber <<
        ", " <<
        syllableNoteUplinkAsString ();
      break;
      
    case kTiedSyllable:
      s << 
        "tied" << ":" <<
        syllableWholeNotesAsMsrString () <<
        " (" << fSyllableWholeNotes << ")" <<
        ", line " << fInputLineNumber <<
        ", " <<
        syllableNoteUplinkAsString () <<
        ", " << "\"";

      writeTextsList (
        fSyllableTextsList,
        s);

      s <<
        "\"";
      break;
      
    case kBarcheckSyllable:
      // fSyllableText contains the measure number JMI ???
      s << 
        "bar check" <<
        " measure " << "fSyllableText ???";
      break;
      
    case kBarNumberCheckSyllable:
      // fSyllableText contains the measure number
      s << 
        "bar number check" <<
        " measure " << "fSyllableText ???";
      break;
      
    case kLineBreakSyllable:
      // fSyllableText contains the measure number
      s << 
        "line break" <<
        " measure " << "fSyllableText ???";
      break;
      
    case kPageBreakSyllable:
      s << 
        "page break" <<
        " measure " << "fSyllableText ???";
      break;
      
    case k_NoSyllable:
      msrInternalError (
        fInputLineNumber,
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
    "Syllable" " " <<
    syllableAsString () <<
    endl;
}

//______________________________________________________________________________
S_msrStanza msrStanza::create (
  int           inputLineNumber,
  string        stanzaNumber,
  msrStanzaKind stanzaKind,
  S_msrVoice    stanzaVoiceUplink)
{
  msrStanza* o =
    new msrStanza (
      inputLineNumber,
      stanzaNumber,
      stanzaKind,
      stanzaVoiceUplink);
  assert(o!=0);

  return o;
}

msrStanza::msrStanza (
  int           inputLineNumber,
  string        stanzaNumber,
  msrStanzaKind stanzaKind,
  S_msrVoice    stanzaVoiceUplink)
    : msrElement (inputLineNumber)
{
  // set stanza number and kind
  fStanzaNumber = stanzaNumber;
  fStanzaKind   = stanzaKind;

  // sanity check
  msrAssert(
    stanzaVoiceUplink != 0,
    "stanzaVoiceUplink is null");

  // set stanza's voice uplink
  fStanzaVoiceUplink =
    stanzaVoiceUplink;
  
  // do other initializations
  initializeStanza ();
}

void msrStanza::initializeStanza ()
{
  switch (fStanzaKind) {
    case kRegularStanza:
      fStanzaName =
        fStanzaVoiceUplink->getVoiceName() +
        "_Stanza_" + 
          stringNumbersToEnglishWords (
            makeSingleWordFromString (
              fStanzaNumber));
      break;
      
    case kMuteStanza:
      fStanzaName =
        fStanzaVoiceUplink->getVoiceName() +
        "_MUTE_Stanza";
      break;
  } // switch
 
  if (gGeneralOptions->fTraceLyrics)
    gLogIos <<
      "==> Initializing stanza " << getStanzaName () <<
      endl;

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
  if (gGeneralOptions->fTraceLyrics) {
    gLogIos <<
      "==> Creating a newborn clone of stanza \"" <<
      getStanzaName () <<
      "\" in voice \"" <<
      containingVoice->getVoiceName () <<
      "\"" <<
      endl;
  }

  // sanity check
  msrAssert(
    containingVoice != 0,
    "containingVoice is null");
    
  S_msrStanza
    newbornClone =
      msrStanza::create (
        fInputLineNumber,
        fStanzaNumber,
        fStanzaKind,
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
  if (gGeneralOptions->fTraceLyrics) {
    gLogIos <<
      "==> Creating a deep copy of stanza \"" <<
      getStanzaName () <<
      "\" in voice \"" <<
      containingVoice->getVoiceName () <<
      "\"" <<
      endl;
  }

  // sanity check
  msrAssert(
    containingVoice != 0,
    "containingVoice is null");
    
  S_msrStanza
    stanzaDeepCopy =
      msrStanza::create (
        fInputLineNumber,
        fStanzaNumber,
        fStanzaKind,
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
  if (gGeneralOptions->fTraceLyrics)
    gLogIos <<
      "Appending syllable '" << syllable->syllableAsString () <<
      "' to stanza " << getStanzaName () <<
      endl;
      
  // append the syllable to this stanza
  fSyllables.push_back (syllable);

  // does this stanza contain text?
  switch (syllable->getSyllableKind ()) {
    
    case msrSyllable::kSingleSyllable:
    case msrSyllable::kBeginSyllable:
    case msrSyllable::kMiddleSyllable:
    case msrSyllable::kEndSyllable:
    case msrSyllable::kRestSyllable:
      // only now, in case addSyllableToStanza() is called
      // from LPSR for example
      fStanzaTextPresent = true;
      break;
      
    case msrSyllable::kSkipSyllable:
    case msrSyllable::kMelismaFirstSyllable:
    case msrSyllable::kMelismaOtherSyllable:
    case msrSyllable::kSlurSyllable:
    case msrSyllable::kSlurBeyondEndSyllable:
    case msrSyllable::kLigatureSyllable:
    case msrSyllable::kLigatureBeyondEndSyllable:
    case msrSyllable::kTiedSyllable:
    case msrSyllable::kBarcheckSyllable:
    case msrSyllable::kBarNumberCheckSyllable:
    case msrSyllable::kLineBreakSyllable:
    case msrSyllable::kPageBreakSyllable:
      break;
      
    case msrSyllable::k_NoSyllable:
      msrInternalError (
        fInputLineNumber,
        "syllable type has not been set");
      break;
  } // switch
}

S_msrSyllable msrStanza::appendRestSyllableToStanza (
  int      inputLineNumber,
  rational wholeNotes)
{
  if (gGeneralOptions->fTraceLyrics) {
    gLogIos <<
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
        msrSyllable::kRestSyllable,
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
  if (gGeneralOptions->fTraceLyrics) {
    gLogIos <<
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
        msrSyllable::kSkipSyllable,
        msrSyllable::k_NoSyllableExtend,
        wholeNotes,
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
  if (gGeneralOptions->fTraceLyrics) {
    gLogIos <<
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

S_msrSyllable msrStanza::appendTiedSyllableToStanza (
  int      inputLineNumber,
  rational wholeNotes)
{
  if (gGeneralOptions->fTraceLyrics) {
    gLogIos <<
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
  if (gGeneralOptions->fTraceLyrics) {
    gLogIos <<
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
  if (gGeneralOptions->fTraceLyrics) {
    gLogIos <<
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
  if (gGeneralOptions->fTraceLyrics) {
    gLogIos <<
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
  if (gGeneralOptions->fTraceLyrics) {
    gLogIos <<
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
  if (gGeneralOptions->fTraceLyrics) {
    gLogIos <<
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
  if (gGeneralOptions->fTraceLyrics) {
    gLogIos <<
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

S_msrSyllable msrStanza::appendLineLineBreakSyllableToStanza (
  int    inputLineNumber,
  string nextMeasureNumber)
{
  if (gGeneralOptions->fTraceLyrics) {
    gLogIos <<
      "Appending a 'Break' syllable" <<
      " to stanza " << getStanzaName () <<
      ", line " << inputLineNumber <<
      endl;
  }
  
  // create stanza break syllable
  S_msrSyllable
    syllable =
      msrSyllable::create (
        inputLineNumber,
        msrSyllable::kLineBreakSyllable,
 // JMI  nextMeasureNumber,
        msrSyllable::k_NoSyllableExtend,
        0,  // wholeNotes
        this);
        
  // append syllable to this stanza
  appendSyllableToStanza (syllable);

  // and return it
  return syllable;
}

void msrStanza::acceptIn (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors)
    gLogIos <<
      "% ==> msrStanza::acceptIn()" <<
      endl;
      
  if (visitor<S_msrStanza>*
    p =
      dynamic_cast<visitor<S_msrStanza>*> (v)) {
        S_msrStanza elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors)
          gLogIos <<
            "% ==> Launching msrStanza::visitStart()" <<
             endl;
        p->visitStart (elem);
  }
}

void msrStanza::acceptOut (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors)
    gLogIos <<
      "% ==> msrStanza::acceptOut()" <<
      endl;

  if (visitor<S_msrStanza>*
    p =
      dynamic_cast<visitor<S_msrStanza>*> (v)) {
        S_msrStanza elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors)
          gLogIos <<
            "% ==> Launching msrStanza::visitEnd()" <<
            endl;
        p->visitEnd (elem);
  }
}

void msrStanza::browseData (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors)
    gLogIos <<
      "% ==> msrStanza::browseData()" <<
      endl;

  idtr++;
  
  // browse the syllables
  int n = fSyllables.size ();
  for (int i = 0; i < n; i++) {
    // browse the syllable
    msrBrowser<msrSyllable> browser (v);
    browser.browse (*fSyllables [i]);
  } // for
  gLogIos <<
    endl;

  idtr--;

  if (gMsrOptions->fTraceMsrVisitors)
    gLogIos <<
      "% <== msrStanza::browseData()" <<
      endl;
}

ostream& operator<< (ostream& os, const S_msrStanza& elt)
{
  elt->print (os);
  return os;
}

string msrStanza::stanzaKindAsString (
  msrStanzaKind stanzaKind)
{
 string result;
  
  switch (stanzaKind) {
    case msrStanza::kRegularStanza:
      result = "regular";
      break;
      
    case msrStanza::kMuteStanza:
      result = "mute";
      break;
  } // switch

  return result;
}

void msrStanza::print (ostream& os)
{
  // getch stanza kind with an initial uppper case letter
  string stanzaKind =
    stanzaKindAsString (fStanzaKind);

  stanzaKind [0] = toupper (stanzaKind [0]);
  
  os <<
    stanzaKind << " " "stanza" " " << getStanzaName () <<
    " (" << fSyllables.size () << " syllables)";
    
  if (! fStanzaTextPresent)
    os << " (No actual text)";

  os << endl;

//  if (fStanzaTextPresent) {  JMI
    idtr++;

    int n = fSyllables.size();
    for (int i = 0; i < n; i++) {
      os << fSyllables [i];
    } // for
    os << endl;

    idtr--;
 // }
}

//______________________________________________________________________________
S_msrHarmonyDegree msrHarmonyDegree::create (
  int                      inputLineNumber,
  int                      harmonyDegreeValue,
  msrAlteration            harmonyDegreeAlteration,
  msrHarmonyDegreeTypeKind harmonyDegreeTypeKind)
{
  msrHarmonyDegree* o =
    new msrHarmonyDegree (
      inputLineNumber,
      harmonyDegreeValue,
      harmonyDegreeAlteration,
      harmonyDegreeTypeKind);
  assert(o!=0);

  return o;
}

msrHarmonyDegree::msrHarmonyDegree (
  int                      inputLineNumber,
  int                      harmonyDegreeValue,
  msrAlteration            harmonyDegreeAlteration,
  msrHarmonyDegreeTypeKind harmonyDegreeTypeKind)
    : msrElement (inputLineNumber)
{
  fHarmonyDegreeValue      = harmonyDegreeValue;
  fHarmonyDegreeAlteration = harmonyDegreeAlteration;
  fHarmonyDegreeTypeKind   = harmonyDegreeTypeKind;

  if (gGeneralOptions->fTraceHarmonies) {
    gLogIos <<
      "==> Creating harmony degree '" <<
      harmonyDegreeAsString () <<
      "'" <<
      endl;
  }
}

msrHarmonyDegree::~msrHarmonyDegree()
{}

void msrHarmonyDegree::setHarmonyDegreeHarmonyUplink (
  S_msrHarmony harmonyUplink)
{
  // sanity check
  msrAssert(
    harmonyUplink != 0,
     "harmonyUplink is null");
     
  fHarmonyDegreeHarmonyUplink =
    harmonyUplink;
}

/* JMI
S_msrHarmonyDegree msrHarmonyDegree::createHarmonyNewbornClone (
  S_msrPart containingPart)
{
  if (gGeneralOptions->fTraceHarmonies) {
    gLogIos <<
      "==> Creating a newborn clone of harmony degree '" <<
      harmonyKindAsShortString () <<
      "'" <<
      endl;
  }

  // sanity check
  msrAssert(
    containingPart != 0,
    "containingPart is null");
    
  S_msrHarmonyDegree
    newbornClone =
      msrHarmonyDegree::create (
        fInputLineNumber,
        fHarmonyDegreeValue,
        fHarmonyDegreeAlteration,
        fHarmonyDegreeTypeKind);
        
  return newbornClone;
}

S_msrHarmonyDegree msrHarmonyDegree::createHarmonyDeepCopy (
  S_msrPart containingPart)
{
  if (gGeneralOptions->fTraceHarmonies) {
    gLogIos <<
      "==> Creating a deep copy of harmony degree '" <<
      harmonyKindAsShortString () <<
      "'" <<
      endl;
  }

  // sanity check
  msrAssert(
    containingPart != 0,
    "containingPart is null");
    
  S_msrHarmonyDegree
    harmonyDeepCopy =
      msrHarmonyDegree::create (
        fInputLineNumber,
        fHarmonyDegreeValue,
        fHarmonyDegreeAlteration,
        fHarmonyDegreeTypeKind);
        
  return harmonyDeepCopy;
}
*/

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

string msrHarmonyDegree::harmonyDegreeTypeAsString (
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

string msrHarmonyDegree::harmonyDegreeAsString () const
{
  stringstream s;

  s <<
    "HarmonyDegree" <<
    ", type: " << harmonyDegreeKindAsShortString () <<
    ", value: " << fHarmonyDegreeValue <<
    ", alteration: " <<
    msrAlterationAsString (
      fHarmonyDegreeAlteration) <<
    ", line: " << fInputLineNumber;

  return s.str ();
}

void msrHarmonyDegree::acceptIn (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors)
    gLogIos <<
      "% ==> msrHarmonyDegree::acceptIn()" <<
      endl;
      
  if (visitor<S_msrHarmonyDegree>*
    p =
      dynamic_cast<visitor<S_msrHarmonyDegree>*> (v)) {
        S_msrHarmonyDegree elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors)
          gLogIos <<
            "% ==> Launching msrHarmonyDegree::visitStart()" <<
             endl;
        p->visitStart (elem);
  }
}

void msrHarmonyDegree::acceptOut (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors)
    gLogIos <<
      "% ==> msrHarmonyDegree::acceptOut()" <<
      endl;

  if (visitor<S_msrHarmonyDegree>*
    p =
      dynamic_cast<visitor<S_msrHarmonyDegree>*> (v)) {
        S_msrHarmonyDegree elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors)
          gLogIos <<
            "% ==> Launching msrHarmonyDegree::visitEnd()" <<
            endl;
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
    harmonyDegreeAsString () <<
    endl;
}

//______________________________________________________________________________
S_msrHarmony msrHarmony::create (
  int                  inputLineNumber,
  S_msrPart            harmonyPart,
  msrQuarterTonesPitch harmonyRootQuarterTonesPitch,
  msrHarmonyKind       harmonyKind,
  string               harmonyKindText,
  int                  harmonyInversion,
  msrQuarterTonesPitch harmonyBassQuarterTonesPitch,
  rational             harmonySoundingWholeNotes)
{
  msrHarmony* o =
    new msrHarmony (
      inputLineNumber,
      harmonyPart,
      harmonyRootQuarterTonesPitch,
      harmonyKind, harmonyKindText,
      harmonyInversion,
      harmonyBassQuarterTonesPitch,
      harmonySoundingWholeNotes);
  assert(o!=0);

  return o;
}

msrHarmony::msrHarmony (
  int                  inputLineNumber,
  S_msrPart            harmonyPartUplink,
  msrQuarterTonesPitch harmonyRootQuarterTonesPitch,
  msrHarmonyKind       harmonyKind,
  string               harmonyKindText,
  int                  harmonyInversion,
  msrQuarterTonesPitch harmonyBassQuarterTonesPitch,
  rational             harmonySoundingWholeNotes)
    : msrElement (inputLineNumber)
{
  // sanity check
  msrAssert(
    harmonyPartUplink != 0,
     "harmonyPartUplink is null");
     
  // set harmony's part
  fHarmonyPartUplink =
    harmonyPartUplink;
    
  fHarmonyRootQuarterTonesPitch =
    harmonyRootQuarterTonesPitch;
 
  fHarmonyKind     = harmonyKind;
  fHarmonyKindText = harmonyKindText;
 
  fHarmonyInversion = harmonyInversion;

  fHarmonyBassQuarterTonesPitch =
    harmonyBassQuarterTonesPitch;
 
  fHarmonySoundingWholeNotes =
    harmonySoundingWholeNotes;

  if (gGeneralOptions->fTraceHarmonies) {
    gLogIos <<
      "==> Creating harmony '" <<
      harmonyAsString () <<
      "'" <<
      endl;
  }
}

msrHarmony::~msrHarmony()
{}

S_msrHarmony msrHarmony::createHarmonyNewbornClone (
  S_msrPart containingPart)
{
  if (gGeneralOptions->fTraceHarmonies) {
    gLogIos <<
      "==> Creating a newborn clone of harmony '" <<
      harmonyKindAsShortString (fHarmonyKind) <<
      "'" <<
      endl;
  }

  // sanity check
  msrAssert(
    containingPart != 0,
    "containingPart is null");
    
  S_msrHarmony
    newbornClone =
      msrHarmony::create (
        fInputLineNumber,
        containingPart,
        fHarmonyRootQuarterTonesPitch,
        fHarmonyKind, fHarmonyKindText,
        fHarmonyInversion,
        fHarmonyBassQuarterTonesPitch,
        fHarmonySoundingWholeNotes);
        
  return newbornClone;
}

S_msrHarmony msrHarmony::createHarmonyDeepCopy (
  S_msrPart containingPart)
{
  if (gGeneralOptions->fTraceHarmonies) {
    gLogIos <<
      "==> Creating a deep copy of harmony '" <<
      harmonyKindAsShortString (fHarmonyKind) <<
      "'" <<
      endl;
  }

  // sanity check
  msrAssert(
    containingPart != 0,
    "containingPart is null");
    
  S_msrHarmony
    harmonyDeepCopy =
      msrHarmony::create (
        fInputLineNumber,
        containingPart,
        fHarmonyRootQuarterTonesPitch,
        fHarmonyKind, fHarmonyKindText,
        fHarmonyInversion,
        fHarmonyBassQuarterTonesPitch,
        fHarmonySoundingWholeNotes);
        
  return harmonyDeepCopy;
}

string msrHarmony::harmonyAsString () const
{
  stringstream s;

  s <<
    "Harmony" <<
    ":" <<
    msrQuarterTonesPitchAsString (
      gMsrOptions->fMsrQuarterTonesPitchesLanguage,
      fHarmonyRootQuarterTonesPitch) <<          
    harmonyKindAsShortString (fHarmonyKind) <<
    " | " <<
    wholeNotesAsMsrString (
      fInputLineNumber,
      fHarmonySoundingWholeNotes);

  if (fHarmonyKindText.size ())
    s <<
      " (" <<fHarmonyKindText << ") ";

  s << "inversion: ";
  if (fHarmonyInversion == K_HARMONY_NO_INVERSION)
    s << "none";
  else
    s << fHarmonyInversion;
    
  if (fHarmonyBassQuarterTonesPitch != k_NoQuarterTonesPitch)
    s <<
      "/" <<
    msrQuarterTonesPitchAsString (
      gMsrOptions->fMsrQuarterTonesPitchesLanguage,
      fHarmonyBassQuarterTonesPitch);    

  if (fHarmonyDegreesList.size ()) {
    list<S_msrHarmonyDegree>::const_iterator
      iBegin = fHarmonyDegreesList.begin(),
      iEnd   = fHarmonyDegreesList.end(),
      i      = iBegin;
      
    for ( ; ; ) {
      s << (*i);
      if (++i == iEnd) break;
      s << " ";
    } // for
  }

  return s.str ();
}

void msrHarmony::acceptIn (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors)
    gLogIos <<
      "% ==> msrHarmony::acceptIn()" <<
      endl;
      
  if (visitor<S_msrHarmony>*
    p =
      dynamic_cast<visitor<S_msrHarmony>*> (v)) {
        S_msrHarmony elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors)
          gLogIos <<
            "% ==> Launching msrHarmony::visitStart()" <<
             endl;
        p->visitStart (elem);
  }
}

void msrHarmony::acceptOut (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors)
    gLogIos <<
      "% ==> msrHarmony::acceptOut()" <<
      endl;

  if (visitor<S_msrHarmony>*
    p =
      dynamic_cast<visitor<S_msrHarmony>*> (v)) {
        S_msrHarmony elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors)
          gLogIos <<
            "% ==> Launching msrHarmony::visitEnd()" <<
            endl;
        p->visitEnd (elem);
  }
}

void msrHarmony::browseData (basevisitor* v)
{
  // browse harmony degrees if any
  if (fHarmonyDegreesList.size ())
    for (
      list<S_msrHarmonyDegree>::const_iterator i = fHarmonyDegreesList.begin();
      i != fHarmonyDegreesList.end();
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
    
  idtr++;

  const int fieldWidth = 15;

  os << left <<
    setw (fieldWidth) <<
    "HarmonyRoot" << " = " <<
    msrQuarterTonesPitchAsString (
      gMsrOptions->fMsrQuarterTonesPitchesLanguage,
      fHarmonyRootQuarterTonesPitch) <<
    endl <<
    setw (fieldWidth) <<
    "HarmonyKind" << " = " <<
    harmonyKindAsString (fHarmonyKind) <<
    endl <<
    setw (fieldWidth) <<
    "HarmonyKindText" << " = \"" <<
    fHarmonyKindText <<
    "\"" <<
    endl <<
    setw (fieldWidth) <<
    "HarmonyBass" << " = " <<
    msrQuarterTonesPitchAsString (
      gMsrOptions->fMsrQuarterTonesPitchesLanguage,
      fHarmonyBassQuarterTonesPitch) <<
    endl;

  os <<
    "inversion: ";
  if (fHarmonyInversion == K_HARMONY_NO_INVERSION)
    os << "none";
  else
    os << fHarmonyInversion;
  os << endl;

  // print harmony degrees if any
  if (fHarmonyDegreesList.size ()) {
    os <<
      "Harmony degrees:" <<
      endl;

    idtr++;
    
    list<S_msrHarmonyDegree>::const_iterator
      iBegin = fHarmonyDegreesList.begin(),
      iEnd   = fHarmonyDegreesList.end(),
      i      = iBegin;
      
    for ( ; ; ) {
      os <<
        (*i)->harmonyDegreeAsString ();
      if (++i == iEnd) break;
      os << endl;
    } // for
    os << endl;
    
    idtr--;
  }

  idtr--;
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
    figurePartUplink != 0,
    "figurePartUplink is null");
     
  // set figured's part uplink
  fFigurePartUplink =
    figurePartUplink;
 
  fFigurePrefixKind = figurePrefixKind;
  fFigureNumber     = figureNumber;
  fFigureSuffixKind = figureSuffixKind;
 
  if (gGeneralOptions->fTraceFiguredBass) {
    gLogIos <<
      "==> Creating figure '" <<
      figureAsString () <<
      "'" <<
      endl;
  }
}

msrFigure::~msrFigure()
{}

S_msrFigure msrFigure::createFigureNewbornClone (
  S_msrPart containingPart)
{
  if (gGeneralOptions->fTraceFiguredBass) {
    gLogIos <<
      "==> Creating a newborn clone of figure '" <<
      figureAsString () <<
      "'" <<
      endl;
  }

  // sanity check
  msrAssert(
    containingPart != 0,
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
  if (gGeneralOptions->fTraceFiguredBass) {
    gLogIos <<
      "==> Creating a deep copy of figure '" <<
      figureAsString () <<
      "'" <<
      endl;
  }

  // sanity check
  msrAssert(
    containingPart != 0,
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

string msrFigure::figureAsString () const
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

void msrFigure::acceptIn (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors)
    gLogIos <<
      "% ==> msrFigure::acceptIn()" <<
      endl;
      
  if (visitor<S_msrFigure>*
    p =
      dynamic_cast<visitor<S_msrFigure>*> (v)) {
        S_msrFigure elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors)
          gLogIos <<
            "% ==> Launching msrFigure::visitStart()" <<
             endl;
        p->visitStart (elem);
  }
}

void msrFigure::acceptOut (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors)
    gLogIos <<
      "% ==> msrFigure::acceptOut()" <<
      endl;

  if (visitor<S_msrFigure>*
    p =
      dynamic_cast<visitor<S_msrFigure>*> (v)) {
        S_msrFigure elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors)
          gLogIos <<
            "% ==> Launching msrFigure::visitEnd()" <<
            endl;
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
    figuredBassPartUplink != 0,
    "figuredBassPartUplink is null");
     
  // set figuredBass's part uplink
  fFiguredBassPartUplink =
    figuredBassPartUplink;

  fFiguredBassSoundingWholeNotes =
    figuredBassSoundingWholeNotes;

  fFiguredBassParenthesesKind =
    figuredBassParenthesesKind;
    
  if (gGeneralOptions->fTraceFiguredBass) {
    gLogIos <<
      "==> Creating figuredBass '" <<
      figuredBassAsString () <<
      "'" <<
      endl;
  }
}

msrFiguredBass::~msrFiguredBass()
{}

S_msrFiguredBass msrFiguredBass::createFiguredBassNewbornClone (
  S_msrPart containingPart)
{
  if (gGeneralOptions->fTraceFiguredBass) {
    gLogIos <<
      "==> Creating a newborn clone of figuredBass '" <<
      figuredBassAsString () <<
      "'" <<
      endl;
  }

  // sanity check
  msrAssert(
    containingPart != 0,
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
  if (gGeneralOptions->fTraceFiguredBass) {
    gLogIos <<
      "==> Creating a deep copy of figuredBass '" <<
      figuredBassAsString () <<
      "'" <<
      endl;
  }

  // sanity check
  msrAssert(
    containingPart != 0,
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
  if (gGeneralOptions->fTraceFiguredBass) {
    gLogIos <<
      "Appending figure'" << figure->figureAsString () <<
      "' to figuredBass '" <<
      figuredBassAsString () <<
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
      result = "parentheses: yes";
      break;
    case msrFiguredBass::kFiguredBassParenthesesNo:
      result = "parentheses: no";
      break;
  } // switch

  return result;
}

string msrFiguredBass::figuredBassAsString () const
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
      iBegin = fFiguredBassFiguresList.begin(),
      iEnd   = fFiguredBassFiguresList.end(),
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

void msrFiguredBass::acceptIn (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors)
    gLogIos <<
      "% ==> msrFiguredBass::acceptIn()" <<
      endl;
      
  if (visitor<S_msrFiguredBass>*
    p =
      dynamic_cast<visitor<S_msrFiguredBass>*> (v)) {
        S_msrFiguredBass elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors)
          gLogIos <<
            "% ==> Launching msrFiguredBass::visitStart()" <<
             endl;
        p->visitStart (elem);
  }
}

void msrFiguredBass::acceptOut (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors)
    gLogIos <<
      "% ==> msrFiguredBass::acceptOut()" <<
      endl;

  if (visitor<S_msrFiguredBass>*
    p =
      dynamic_cast<visitor<S_msrFiguredBass>*> (v)) {
        S_msrFiguredBass elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors)
          gLogIos <<
            "% ==> Launching msrFiguredBass::visitEnd()" <<
            endl;
        p->visitEnd (elem);
  }
}

void msrFiguredBass::browseData (basevisitor* v)
{
  for (
    list<S_msrFigure>::const_iterator i = fFiguredBassFiguresList.begin();
    i != fFiguredBassFiguresList.end();
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
    idtr++;

    list<S_msrFigure>::const_iterator
      iBegin = fFiguredBassFiguresList.begin(),
      iEnd   = fFiguredBassFiguresList.end(),
      i      = iBegin;
      
    for ( ; ; ) {
      os << (*i);
      if (++i == iEnd) break;
 // JMI     os << endl;
    } // for

    idtr--;
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

void msrSegno::acceptIn (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors)
    gLogIos <<
      "% ==> msrSegno::acceptIn()" <<
      endl;
      
  if (visitor<S_msrSegno>*
    p =
      dynamic_cast<visitor<S_msrSegno>*> (v)) {
        S_msrSegno elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors)
          gLogIos <<
            "% ==> Launching msrSegno::visitStart()" <<
             endl;
        p->visitStart (elem);
  }
}

void msrSegno::acceptOut (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors)
    gLogIos <<
      "% ==> msrSegno::acceptOut()" <<
      endl;

  if (visitor<S_msrSegno>*
    p =
      dynamic_cast<visitor<S_msrSegno>*> (v)) {
        S_msrSegno elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors)
          gLogIos <<
            "% ==> Launching msrSegno::visitEnd()" <<
            endl;
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

void msrCoda::acceptIn (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors)
    gLogIos <<
      "% ==> msrCoda::acceptIn()" <<
      endl;
      
  if (visitor<S_msrCoda>*
    p =
      dynamic_cast<visitor<S_msrCoda>*> (v)) {
        S_msrCoda elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors)
          gLogIos <<
            "% ==> Launching msrCoda::visitStart()" <<
             endl;
        p->visitStart (elem);
  }
}

void msrCoda::acceptOut (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors)
    gLogIos <<
      "% ==> msrCoda::acceptOut()" <<
      endl;

  if (visitor<S_msrCoda>*
    p =
      dynamic_cast<visitor<S_msrCoda>*> (v)) {
        S_msrCoda elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors)
          gLogIos <<
            "% ==> Launching msrCoda::visitEnd()" <<
            endl;
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

msrEyeGlasses::~msrEyeGlasses()
{}

void msrEyeGlasses::acceptIn (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors)
    gLogIos <<
      "% ==> msrEyeGlasses::acceptIn()" <<
      endl;
      
  if (visitor<S_msrEyeGlasses>*
    p =
      dynamic_cast<visitor<S_msrEyeGlasses>*> (v)) {
        S_msrEyeGlasses elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors)
          gLogIos <<
            "% ==> Launching msrEyeGlasses::visitStart()" <<
             endl;
        p->visitStart (elem);
  }
}

void msrEyeGlasses::acceptOut (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors)
    gLogIos <<
      "% ==> msrEyeGlasses::acceptOut()" <<
      endl;

  if (visitor<S_msrEyeGlasses>*
    p =
      dynamic_cast<visitor<S_msrEyeGlasses>*> (v)) {
        S_msrEyeGlasses elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors)
          gLogIos <<
            "% ==> Launching msrEyeGlasses::visitEnd()" <<
            endl;
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
  int          inputLineNumber,
  msrPedalType pedalType,
  msrPedalLine pedalLine,
  msrPedalSign pedalSign)
{
  msrPedal* o =
    new msrPedal (
      inputLineNumber,
      pedalType, pedalLine, pedalSign);
  assert(o!=0);
  return o;
}

msrPedal::msrPedal (
  int          inputLineNumber,
  msrPedalType pedalType,
  msrPedalLine pedalLine,
  msrPedalSign pedalSign)
    : msrElement (inputLineNumber)
{
  fPedalType = pedalType;
  fPedalLine = pedalLine;
  fPedalSign = pedalSign;
}

msrPedal::~msrPedal()
{}

string msrPedal::pedalTypeAsString ()
{
  string result;
  
  switch (fPedalType) {
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
  
  switch (fPedalLine) {
    case msrPedal::kPedalLineYes:
      result = "pedal line: yes";
      break;
    case msrPedal::kPedalLineNo:
      result = "pedal line: no";
      break;
  } // switch

  return result;
}

string msrPedal::pedalSignAsString ()
{
  string result;
  
  switch (fPedalSign) {
    case msrPedal::kPedalSignYes:
      result = "pedal sign: yes";
      break;
    case msrPedal::kPedalSignNo:
      result = "pedal sign: no";
      break;
  } // switch

  return result;
}

void msrPedal::acceptIn (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors)
    gLogIos <<
      "% ==> msrPedal::acceptIn()" <<
      endl;
      
  if (visitor<S_msrPedal>*
    p =
      dynamic_cast<visitor<S_msrPedal>*> (v)) {
        S_msrPedal elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors)
          gLogIos <<
            "% ==> Launching msrPedal::visitStart()" <<
             endl;
        p->visitStart (elem);
  }
}

void msrPedal::acceptOut (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors)
    gLogIos <<
      "% ==> msrPedal::acceptOut()" <<
      endl;

  if (visitor<S_msrPedal>*
    p =
      dynamic_cast<visitor<S_msrPedal>*> (v)) {
        S_msrPedal elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors)
          gLogIos <<
            "% ==> Launching msrPedal::visitEnd()" <<
            endl;
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
S_msrBarline msrBarline::create (
  int                       inputLineNumber,
  bool                      barlineHasSegno,
  bool                      barlineHasCoda,
  msrBarlineLocation        location,
  msrBarlineStyle           style,
  msrBarlineEndingType      endingType,
  string                    endingNumber,
  msrBarlineRepeatDirection repeatDirection,
  msrBarlineRepeatWinged    repeatWinged)
{
  msrBarline* o =
    new msrBarline (
      inputLineNumber,
      barlineHasSegno, barlineHasCoda,
      location, style,
      endingType, endingNumber,
      repeatDirection, repeatWinged);
  assert(o!=0);
  return o;
}

msrBarline::msrBarline (
  int                       inputLineNumber,
  bool                      barlineHasSegno,
  bool                      barlineHasCoda,
  msrBarlineLocation        location,
  msrBarlineStyle           style,
  msrBarlineEndingType      endingType,
  string                    endingNumber,
  msrBarlineRepeatDirection repeatDirection,
  msrBarlineRepeatWinged    repeatWinged)
    : msrElement (inputLineNumber)
{
  fBarlineHasSegno = barlineHasSegno;
  fBarlineHasCoda  = barlineHasCoda;
  
  fLocation        = location;
  fStyle           = style;
  fEndingType      = endingType;
  fEndingNumber    = endingNumber;
  fRepeatDirection = repeatDirection;
  fRepeatWinged    = repeatWinged;
  
  // JMI gLogIos << "fEndingNumber = " << fEndingNumber << endl;
  
  // extract individual numbers from fEndingNumber
  // that may contain "1, 2"
  fEndingNumbersList =
    extractNumbersFromString (
      fEndingNumber, false); // 'true' to debug it
}

msrBarline::~msrBarline()
{}

void msrBarline::acceptIn (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors)
    gLogIos <<
      "% ==> msrBarline::acceptIn()" <<
      endl;
      
  if (visitor<S_msrBarline>*
    p =
      dynamic_cast<visitor<S_msrBarline>*> (v)) {
        S_msrBarline elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors)
          gLogIos <<
            "% ==> Launching msrBarline::visitStart()" <<
             endl;
        p->visitStart (elem);
  }
}

void msrBarline::acceptOut (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors)
    gLogIos <<
      "% ==> msrBarline::acceptOut()" <<
      endl;

  if (visitor<S_msrBarline>*
    p =
      dynamic_cast<visitor<S_msrBarline>*> (v)) {
        S_msrBarline elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors)
          gLogIos <<
            "% ==> Launching msrBarline::visitEnd()" <<
            endl;
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

string msrBarline::barlineLocationAsString (
  msrBarlineLocation barlineLocation)
{
  string result;
  
  switch (barlineLocation) {
    case k_NoLocation:
      result = "none";
      break;
    case kLeftLocation:
      result = "Left";
      break;
    case kMiddleLocation:
      result = "Middle";
      break;
    case kRightLocation:
      result = "Right";
      break;
  } // switch

  return result;
}

string msrBarline::barlineCategoryAsString (
  msrBarlineCategory barlineCategory)
{
  string result;
  
  switch (barlineCategory) {
    case kStandaloneBarline:
      result = "standalone";
      break;
    case kRepeatStartBarline:
      result = "repeat start";
      break;
    case kRepeatEndBarline:
      result = "repeat end";
      break;
    case kHookedEndingStartBarline:
      result = "hooked ending start";
      break;
    case kHookedEndingEndBarline:
      result = "hooked ending end";
      break;
    case kHooklessEndingStartBarline:
      result = "hookless ending start";
      break;
    case kHooklessEndingEndBarline:
      result = "hookless ending end";
      break;
  } // switch

  return result;
}

string msrBarline::barlineStyleAsString (
  msrBarlineStyle barlineStyle)
{
  string result;
  
  switch (barlineStyle) {
    case k_NoStyle:
      result = "none";
      break;
    case kRegularStyle:
      result = "Regular";
      break;
    case kDottedStyle:
      result = "Dotted";
      break;
    case kDashedStyle:
      result = "Dashed";
      break;
    case kHeavyStyle:
      result = "Heavy";
      break;
    case kLightLightStyle:
      result = "LightLight";
      break;
    case kLightHeavyStyle:
      result = "LightHeavy";
      break;
    case kHeavyLightStyle:
      result = "HeavyLight";
      break;
    case kHeavyHeavyStyle:
      result = "HeavyHeavy";
      break;
    case kTickStyle:
      result = "Tick";
      break;
    case kShortStyle:
      result = "Short";
      break;
    case kNoneStyle:
      result = "None";
      break;
  } // switch

  return result;
}

string msrBarline::barlineEndingTypeAsString (
  msrBarlineEndingType barlineEndingType)
{
  string result;
  
  switch (barlineEndingType) {
    case k_NoEndingType:
      result = "none";
      break;
    case kStartEndingType:
      result = "Start";
      break;
    case kStopEndingType:
      result = "Stop";
      break;
    case kDiscontinueEndingType:
      result = "Discontinue";
      break;
  } // switch

  return result;
}

string msrBarline::barlineRepeatDirectionAsString (
  msrBarlineRepeatDirection barlineRepeatDirection)
{
  string result;
  
  switch (barlineRepeatDirection) {
    case k_NoRepeatDirection:
      result ="none";
      break;
    case kForwardRepeatDirection:
      result ="Forward";
      break;
    case kBackwardRepeatDirection:
      result ="Backward";
      break;
  } // switch

  return result;
}

string msrBarline::barlineRepeatWingedAsString (
  msrBarlineRepeatWinged barlineRepeatWinged)
{
  string result;
  
  switch (barlineRepeatWinged) {
    case k_NoRepeatWinged:
      result = "none"; // ??? JMI
      break;
    case kNoneRepeatWinged:
      result = "none";
      break;
    case kStraightRepeatWinged:
      result = "Straight";
      break;
    case kCurvedRepeatWinged:
      result = "Curved";
      break;
    case kDoubleStraightRepeatWinged:
      result = "DoubleStraight";
      break;
    case kDoubleCurvedRepeatWinged:
      result = "DoubleCurved";
      break;
  } // switch

  return result;
}

string msrBarline::barlineAsString () const
{
  stringstream s;

  s <<
    "Barline " << barlineCategoryAsString (fBarlineCategory) <<
    ", line " << fInputLineNumber <<
    ", EndingType" << " : " <<
    barlineEndingTypeAsString (fEndingType) <<
    ", RepeatDirection" << " : " <<
    barlineRepeatDirectionAsString (fRepeatDirection);
    
  return s.str ();
}

void msrBarline::print (ostream& os)
{
  os <<
    "Barline, " << barlineCategoryAsString (fBarlineCategory) <<
    ", line " << fInputLineNumber;

  if (fBarlineHasSegno)
    os << ", has segno";
    
  if (fBarlineHasCoda)
    os << ", has coda";

  os <<
    endl;

  idtr++;

  const int fieldWidth = 15;

  os << left <<
    setw (fieldWidth) <<
    "Location" << " : " <<
    barlineLocationAsString (fLocation) <<
    endl <<
    setw (fieldWidth) <<
    "Style" << " : " <<
    barlineStyleAsString (fStyle) <<
    endl <<
    setw (fieldWidth) <<
    "EndingType" << " : " <<
    barlineEndingTypeAsString (fEndingType) <<
    endl;
  
  os <<
    setw (fieldWidth) <<
    "Ending number" << " : ";
      
  list<int>::const_iterator i;
  for (i=fEndingNumbersList.begin(); i!=fEndingNumbersList.end(); i++) {
    os << (*i) << " ";
  } // for
  
  os <<
    endl;
 
  os <<
    setw (fieldWidth) <<
     "RepeatDirection" << " : " <<
    barlineRepeatDirectionAsString (fRepeatDirection) <<
    endl;
  
  os <<
    setw (fieldWidth) <<
    "RepeatWinged" << " : " <<
    barlineRepeatWingedAsString (fRepeatWinged) <<
    endl;
     
  idtr--;
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
    measureSegmentUplink != 0,
    "measureSegmentUplink is null");

  // set measure's segment uplink
  fMeasureSegmentUplink =
    measureSegmentUplink;

  // set measure number
  fMeasureNumber = measureNumber;
  
  // do other initializations
  initializeMeasure ();
}

void msrMeasure::initializeMeasure ()
{
  if (gGeneralOptions->fTraceMeasures)
    gLogIos <<
      "==> Initializing measure " << fMeasureNumber <<
      " in segment " <<
      fMeasureSegmentUplink->getSegmentAbsoluteNumber () <<
      " in voice \"" <<
      fMeasureSegmentUplink->
        getSegmentVoiceUplink ()->
          getVoiceName () <<
      "\"" <<
      ", line " << fInputLineNumber <<
      endl;

  fMeasureKind = kUnknownMeasureKind;

/* JMI
  static int count = 0;

  count++;

  if (count == 2)
    abort();
*/

  // initialize measure position
  setMeasureLength (
    fInputLineNumber,
    rational (0, 1)); // ready to receive the first note
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
  if (gGeneralOptions->fTraceMeasures)
    gLogIos <<
      "==> Creating a newborn clone of measure '" <<
      fMeasureNumber <<
      "'" <<
      " in segment " <<
      containingSegment->segmentAsString () <<
      " in voice \"" <<
        containingSegment->
          getSegmentVoiceUplink ()->
            getVoiceName () <<
      "\"" <<
      endl;
  
  // sanity check
  msrAssert(
    containingSegment != 0,
    "containingSegment is null");

  // create newborn clone
  S_msrMeasure
    newbornClone =
      msrMeasure::create (
        fInputLineNumber,
        fMeasureNumber,
        containingSegment);

  // lengthes
  newbornClone->fMeasureFullMeasureLength =
    fMeasureFullMeasureLength;
    
  // don't take fMeasureLength over,
  // it will be computed on the fly
  // while appending notes to the measure newborn clone

  // measure kind
  newbornClone->fMeasureKind =
    fMeasureKind;

  // measure 'first in segment' kind
  newbornClone->fMeasureFirstInSegmentKind =
    fMeasureFirstInSegmentKind;

  // chords handling

  // elements

  // uplinks

  return newbornClone;
}

S_msrMeasure msrMeasure::createMeasureDeepCopy (
  S_msrSegment containingSegment)
{
  if (gGeneralOptions->fTraceMeasures)
    gLogIos <<
      "==> Creating a deep copy of measure '" <<
      fMeasureNumber <<
      "'" <<
      " in segment " <<
      containingSegment->segmentAsString () <<
      " in voice \"" <<
        containingSegment->
          getSegmentVoiceUplink ()->
            getVoiceName () <<
      "\"" <<
      endl;
  
  // sanity check
  msrAssert(
    containingSegment != 0,
    "containingSegment is null");

  // create deep copy
  S_msrMeasure
    measureDeepCopy =
      msrMeasure::create (
        fInputLineNumber,
        fMeasureNumber,
        containingSegment);

  // lengthes
  measureDeepCopy->fMeasureFullMeasureLength =
    fMeasureFullMeasureLength;
    
  measureDeepCopy->fMeasureLength =
    fMeasureLength;
    
  // measure kind
  measureDeepCopy->fMeasureKind =
    fMeasureKind;

  // measure 'first in segment' kind
  measureDeepCopy->fMeasureFirstInSegmentKind =
    fMeasureFirstInSegmentKind;

  // elements

  int numberOfMeasureElements =
    fMeasureElementsList.size ();

  if (numberOfMeasureElements) {
     if (gGeneralOptions->fTraceMeasures) {
      gLogIos <<
        singularOrPluralWithoutNumber (
          numberOfMeasureElements, "There is", "There are") <<
        " " <<
        singularOrPlural (
          numberOfMeasureElements, "element", "elements") <<
        " to be deep copied" <<
        " in measure " <<
        fMeasureNumber <<
        " in segment " <<
        containingSegment->segmentAsString () <<
        " in voice \"" <<
          containingSegment->
            getSegmentVoiceUplink ()->
              getVoiceName () <<
        "\"" <<
        endl;
    }
    
    for (
      list<S_msrElement>::const_iterator i = fMeasureElementsList.begin();
      i != fMeasureElementsList.end();
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
            fetchMeasurePartUplink ()); // JMI

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
        if (gGeneralOptions->fTraceTimes || gGeneralOptions->fTraceMeasures) {
          gLogIos <<
            "Sharing time '" <<
            time->timeAsShortString () <<
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
    if (gGeneralOptions->fTraceMeasures) {
      gLogIos <<
        "There are no elements in measure to be deep copied" <<
        " in segment " <<
        containingSegment->segmentAsString () <<
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

string msrMeasure::measureFullMeasureLengthAsMSRString ()
{
  return
    wholeNotesAsMsrString (
      fInputLineNumber,
      fMeasureFullMeasureLength);
}

void msrMeasure::setMeasureLength (
  int      inputLineNumber,
  rational measureLength)
{
  // rationalise the measure length
  rational rationalisedMeasureLength = measureLength;
  rationalisedMeasureLength.rationalise ();
  
  if (gGeneralOptions->fTraceMeasures || gGeneralOptions->fTraceDivisions) {
    gLogIos <<
      "Setting measure " << fMeasureNumber <<
      " measure length to '"  << rationalisedMeasureLength << "'";

    if (
      rationalisedMeasureLength.getDenominator ()
        !=
      measureLength.getDenominator ()) {
      gLogIos <<
        " (was '" << measureLength << "')";
    }

    gLogIos <<
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

void msrMeasure::appendClefToMeasure (S_msrClef clef)
{
  if (gGeneralOptions->fTraceClefs || gGeneralOptions->fTraceMeasures) {
    gLogIos <<
      "Appending clef '" << clef->clefAsString () <<
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
  if (gGeneralOptions->fTraceKeys || gGeneralOptions->fTraceMeasures) {
    gLogIos <<
      "Appending key '" << key->keyAsString () <<
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
    time != 0, "time is null");

  if (gGeneralOptions->fTraceTimes || gGeneralOptions->fTraceMeasures) {
    gLogIos <<
      "Appending time:" <<
      endl;

    idtr++;

    gLogIos <<
      time;

    idtr--;

    gLogIos <<
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
  setMeasureFullMeasureLengthFromTime (
    time);
    
/* JMI
  if (time->getTimeSymbolKind () == msrTime::kTimeSymbolSenzaMisura) {
    
    // this measure is senza misura
    
    if (gGeneralOptions->fTraceMeasures) {
      gLogIos <<
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
    
    fMeasureFullMeasureLength = INT_MAX; // JMI
  }
  
  else {
    
    // this measure is con misura
    
    int partDivisionsPerQuarterNote =
      fetchMeasurePartUplink->
        getPartDivisionsPerQuarterNote ();
  
    rational
      wholeNotesPerMeasure =
        time->wholeNotesPerMeasure ();
        
    if (gGeneralOptions->fTraceTimes) {
      gLogIos <<
        time;
  
      gLogIos <<
        "has " <<
        wholeNotesPerMeasure.getNumerator () <<
        "/" <<
        wholeNotesPerMeasure.getDenominator () <<
        " whole note(s) per measure" <<
        ", line " << fInputLineNumber <<
        endl;
    }
    
    fMeasureFullMeasureLength =
      wholeNotesPerMeasure.getNumerator ()
        *
      partDivisionsPerQuarterNote * 4 // hence a whole note
        /
      wholeNotesPerMeasure.getDenominator ();
    
  
    if (gGeneralOptions->fTraceMeasures)
      gLogIos <<
        "Measure '" << fMeasureNumber <<
        "' in voice \"" <<
        fMeasureSegmentUplink->
          getSegmentVoiceUplink ()->
            getVoiceName () <<
        "\"" <<
        " has " <<
        singularOrPlural (
          fMeasureFullMeasureLength,
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
    time != 0, "time is null");

  if (gGeneralOptions->fTraceTimes || gGeneralOptions->fTraceMeasures) {
    gLogIos <<
      "Appending time:" <<
      endl;

    idtr++;

    gLogIos <<
      time;

    idtr--;

    gLogIos <<
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

void msrMeasure::setMeasureFullMeasureLengthFromTime (
  S_msrTime time)
{
  // sanity check
  msrAssert(
    time != 0,
    "time is null");

  if (
    gGeneralOptions->fTraceDivisions
      ||
    gGeneralOptions->fTraceTimes
      ||
    gGeneralOptions->fTraceMeasures) {
    gLogIos <<
      "Setting measure full measure length from time" <<
      ", line " << fInputLineNumber <<
      endl;

    idtr++;

    gLogIos <<
      time;

    idtr--;

    gLogIos <<
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
      gGeneralOptions->fTraceDivisions
        ||
      gGeneralOptions->fTraceTimes
        ||
      gGeneralOptions->fTraceMeasures) {
      gLogIos <<
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
    
    fMeasureFullMeasureLength =
      rational (INT_MAX, 1);
  }
  
  else {
    
    // this measure is con misura
    
    rational
      wholeNotesPerMeasure =
        time->
          wholeNotesPerMeasure ();
        
    if (
      gGeneralOptions->fTraceDivisions
        ||
      gGeneralOptions->fTraceTimes
        ||
      gGeneralOptions->fTraceMeasures) {
      gLogIos <<
        "There are " <<
        wholeNotesPerMeasure <<
        " whole note(s) per measure in time:" <<
        endl;

      idtr++;
      
      gLogIos <<
        time;
                  
      idtr--;

      gLogIos <<
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
    fMeasureFullMeasureLength =
      wholeNotesPerMeasure;

    if (
      gGeneralOptions->fTraceDivisions
        ||
      gGeneralOptions->fTraceTimes
        ||
      gGeneralOptions->fTraceMeasures) {
      gLogIos <<
        "Measure '" << fMeasureNumber <<
        "' in voice \"" <<
        fMeasureSegmentUplink->
          getSegmentVoiceUplink ()->
            getVoiceName () <<
        "\"" <<
        " has full measure length " <<
        fMeasureFullMeasureLength <<
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

void msrMeasure::appendBarlineToMeasure (S_msrBarline barline)
{    
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

  if (gGeneralOptions->fTraceNotes || gGeneralOptions->fTraceMeasures)
    gLogIos <<
      "Appending note '" << note->noteAsShortString () <<
      "' to measure '" << fMeasureNumber <<
      "' in voice \"" <<
      fMeasureSegmentUplink->
        getSegmentVoiceUplink ()->
          getVoiceName () <<
      "\"" <<
      endl;

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
  if (noteSoundingWholeNotes == fMeasureFullMeasureLength)
    note->
      setNoteOccupiesAFullMeasure ();
    
  // append the note to the measure elements list
// JMI  // only now to make it possible to remove it afterwards
  // if it happens to be the first note of a chord
  fMeasureElementsList.push_back (note);

  // fetch part harmony voice
  S_msrVoice
    partHarmonyVoice =
      fetchMeasurePartUplink ()->
        getPartHarmonyVoice ();

  // fetch part harmonies supplier voice
  S_msrVoice
    partHarmoniesSupplierVoice =
      fetchMeasurePartUplink ()->
        getPartHarmoniesSupplierVoice ();

  // fetch note harmony
  S_msrHarmony
    noteHarmony =
      note->getNoteHarmony ();
      
  // don't handle the note harmony here,
  // this has been done after harmony::create ()
  if (! noteHarmony) {
    if (partHarmoniesSupplierVoice) {
      if (gGeneralOptions->fTraceNotes || gGeneralOptions->fTraceMeasures)
        gLogIos <<
          "measureVoiceUplink = \"" <<
          fetchMeasureVoiceUplink ()->getVoiceName () <<
          "\"" <<
          endl <<
          "partHarmoniesSupplierVoice = \"" <<
          partHarmoniesSupplierVoice->getVoiceName () <<
          "\"" <<
          endl;

/* JMI
      // bring harmony voice to the same measure length
      partHarmonyVoice->
        bringVoiceToMeasureLength (
          inputLineNumber,
          noteMeasureLength);
*/

      // is measure voice uplink the part harmonies suppplier voice?
      if (
        fetchMeasureVoiceUplink ()
          ==
        partHarmoniesSupplierVoice) {
        // yes, create a skip note of the same duration as the note
        S_msrNote
          skipNote =
            msrNote::createSkipNote (
              inputLineNumber,
              noteSoundingWholeNotes,
              noteSoundingWholeNotes,
              note->getNoteDotsNumber (),
              partHarmonyVoice->
                getVoiceStaffUplink ()->
                  getStaffNumber (),
              partHarmonyVoice->
                getVoicePartRelativeID ());
  
        // append the skip to the part harmony voice
        if (gGeneralOptions->fTraceHarmonies || gGeneralOptions->fTraceMeasures)
          gLogIos <<
            "Appending skip '" << skipNote->noteAsShortString () <<
            "' to measure '" << fMeasureNumber <<
            "' in harmony voice \"" <<
            partHarmonyVoice->getVoiceName () <<
            "\"" <<
            endl;

        // sanity check
        msrAssert (
          fMeasureElementsList.size () > 0,
          "fMeasureElementsList is empty"); // JMI
          
        partHarmonyVoice->
          appendNoteToVoice (skipNote);
      }
    }
  }

  // register note as the last one in this measure
  fMeasureLastHandledNote = note;
}

void msrMeasure::appendNoteToMeasureClone (S_msrNote note)
{
  int inputLineNumber =
    note->getInputLineNumber ();

  if (gGeneralOptions->fTraceNotes || gGeneralOptions->fTraceMeasures)
    gLogIos <<
      "Appending note '" << note->noteAsShortString () <<
      "' to measure clone '" << fMeasureNumber <<
      "' in voice clone \"" <<
      fMeasureSegmentUplink->
        getSegmentVoiceUplink ()->
          getVoiceName () <<
      "\"" <<
      endl;

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
    note->setNoteMeasureUplink (this);

    // register note measure number
    note->setNoteMeasureNumber (fMeasureNumber);
    
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
    if (noteSoundingWholeNotes == fMeasureFullMeasureLength)
      note->
        setNoteOccupiesAFullMeasure ();
      
    // append the note to the measure elements list
  // JMI  // only now to make it possible to remove it afterwards
    // if it happens to be the first note of a chord
    fMeasureElementsList.push_back (note);

    // fetch part harmony voice
    S_msrVoice
      partHarmonyVoice =
        fetchMeasurePartUplink ()->
          getPartHarmonyVoice ();

    // fetch note harmony
    S_msrHarmony
      noteHarmony =
        note->getNoteHarmony ();


   /*     
    if (noteHarmony) {
      // append the harmony to the harmony voice
      if (gGeneralOptions->fTraceNotes || gGeneralOptions->fTraceMeasures)
        gLogIos <<
          "Appending harmony '" << noteHarmony->harmonyAsString () <<
          "' to measure '" << fMeasureNumber <<
          "' in harmony voice \"" <<
          partHarmonyVoice->getVoiceName () <<
          "\"" <<
          endl;

      partHarmonyVoice->
        appendHarmonyToVoice (
          noteHarmony);
    }

    else {
      // is fetchMeasureVoiceUplink the part harmonies suppplier voice?
      if (
        fetchMeasureVoiceUplink
          ==
        fMeasureDirectPartUplink->getPartHarmoniesSupplierVoice ()) {
        // yes, create a skip note of the same duration as the note
        S_msrNote
          skipNote =
            msrNote::createSkipNote (
              inputLineNumber,
              noteSoundingWholeNotes,
              noteSoundingWholeNotes,
              note->getNoteDotsNumber (),
              partHarmonyVoice->
                getVoiceStaffUplink ()->
                  getStaffNumber (),
              partHarmonyVoice->
                getVoicePartRelativeID ());
  
        // append the skip to the part harmony voice
        if (gGeneralOptions->fTraceNotes || gGeneralOptions->fTraceMeasures)
          gLogIos <<
            "Appending skip '" << skipNote->noteAsShortString () <<
            "' to measure '" << fMeasureNumber <<
            "' in harmony voice \"" <<
            partHarmonyVoice->getVoiceName () <<
            "\"" <<
            endl;

        partHarmonyVoice->
          appendNoteToVoice (skipNote);
      }
    }
*/

    // register note as the last one in this measure
    fMeasureLastHandledNote = note;
 // JMI }
}

void msrMeasure::appendDoubleTremoloToMeasure (
  S_msrDoubleTremolo doubleTremolo)
{
  int inputLineNumber =
    doubleTremolo->getInputLineNumber ();
  
  // populate measure uplink
  doubleTremolo->setDoubleTremoloMeasureUplink (this);

  if (gGeneralOptions->fTraceTremolos || gGeneralOptions->fTraceMeasures)
    gLogIos <<
      "Appending double tremolo '" <<
      doubleTremolo->doubleTremoloAsShortString () <<
      "' to measure '" << fMeasureNumber <<
      "' in voice \"" <<
      fMeasureSegmentUplink->
        getSegmentVoiceUplink ()->
          getVoiceName () <<
      "\"" <<
      endl;

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

/* JMI
  // bring harmony voice to the same measure length
  fetchMeasurePartUplink->
    getPartHarmonyVoice ()->
      bringVoiceToMeasureLength (
        inputLineNumber,
        fMeasureLength);
        */
}

void msrMeasure::appendMeasureRepeatToMeasure (
  S_msrMeasureRepeat measureRepeat)
{
  int inputLineNumber =
    measureRepeat->getInputLineNumber ();
    
/* JMI  
  // populate measure uplink
  measureRepeat->setMeasureRepeatMeasureUplink (this);
*/

  if (gGeneralOptions->fTraceRepeats || gGeneralOptions->fTraceMeasures)
    gLogIos <<
      "Appending multiple rest '" <<
      measureRepeat->measureRepeatAsString () <<
      "' to measure '" << fMeasureNumber <<
      "' in voice \"" <<
      fMeasureSegmentUplink->
        getSegmentVoiceUplink ()->
          getVoiceName () <<
      "\"" <<
      endl;
/* JMI
  // register measureRepeat measure number
  measureRepeat->
    setmeasureRepeatMeasureNumber (fMeasureNumber);
  
  // register measureRepeat measure position
  measureRepeat->
    setmeasureRepeatPositionInMeasure (
      fMeasureLength);

  // copy measure number to first note, that was created beforehand
  measureRepeat->
    setmeasureRepeatMeasureNumber (
      fMeasureNumber);
  
  // copy measure position to first note, that was created beforehand
  measureRepeat->
    setmeasureRepeatPositionInMeasure (
      fMeasureLength);

  // fetch measureRepeat sounding whole notes
  int measureRepeatSoundingWholeNotes =
    measureRepeat->getmeasureRepeatSoundingWholeNotes ();
    
  // account for measureRepeat duration in measure length
  setMeasureLength (
    inputLineNumber,
    fMeasureLength + measureRepeatSoundingWholeNotes);

  // update part measure length high tide if need be
  fMeasureDirectPartUplink->
    updatePartMeasureLengthHighTide (
      inputLineNumber,
      fMeasureLength);

  // determine if the measureRepeat occupies a full measure
// XXL  JMI  if (measureRepeatSoundingWholeNotes == fMeasureDivisionsPerWholeMeasure)
    // measureRepeat->setmeasureRepeatOccupiesAFullMeasure ();

  // append the measureRepeat to the measure elements list
  fMeasureElementsList.push_back (measureRepeat);
*/

/* JMI
  // bring harmony voice to the same measure length
  fMeasureDirectPartUplink->
    getPartHarmonyVoice ()->
      bringVoiceToMeasureLength (
        inputLineNumber,
        fMeasureLength);
        */
}

void msrMeasure::appendMultipleRestToMeasure (
  S_msrMultipleRest multipleRest)
{
  int inputLineNumber =
    multipleRest->getInputLineNumber ();
    
  /* JMI  
    // populate measure uplink
    multipleRest->setMultipleRestMeasureUplink (this);
*/

  if (gGeneralOptions->fTraceRepeats || gGeneralOptions->fTraceMeasures)
    gLogIos <<
      "Appending multiple rest '" <<
      multipleRest->multipleRestAsString () <<
      "' to measure '" << fMeasureNumber <<
      "' in voice \"" <<
      fMeasureSegmentUplink->
        getSegmentVoiceUplink ()->
          getVoiceName () <<
      "\"" <<
      endl;
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

  // determine if the multipleRest occupies a full measure
// XXL  JMI  if (multipleRestSoundingWholeNotes == fMeasureDivisionsPerWholeMeasure)
    // multipleRest->setmultipleRestOccupiesAFullMeasure ();

  // append the multipleRest to the measure elements list
  fMeasureElementsList.push_back (multipleRest);
*/

/* JMI
  // bring harmony voice to the same measure length
  fMeasureDirectPartUplink->
    getPartHarmonyVoice ()->
      bringVoiceToMeasureLength (
        inputLineNumber,
        fMeasureLength);
        */
}

void msrMeasure::appendChordToMeasure (S_msrChord chord) // JMI XXL
{
  int inputLineNumber =
    chord->getInputLineNumber ();
    
  // populate measure uplink
  chord->setChordMeasureUplink (this);

  if (gGeneralOptions->fTraceChords || gGeneralOptions->fTraceMeasures)
    gLogIos <<
      "Appending chord '" << chord->chordAsString () <<
      "' to measure '" << fMeasureNumber <<
      "' in voice \"" <<
      fMeasureSegmentUplink->
        getSegmentVoiceUplink ()->
          getVoiceName () <<
      "\"" <<
      endl;

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

/* JMI
  // bring harmony voice to the new measure length
  fetchMeasurePartUplink->
    getPartHarmonyVoice ()->
      bringVoiceToMeasureLength (
        inputLineNumber,
        fMeasureLength);
        */
}

void msrMeasure::appendTupletToMeasure (S_msrTuplet tuplet)
{
  int inputLineNumber =
    tuplet->getInputLineNumber ();
    
  if (gGeneralOptions->fTraceTuplets || gGeneralOptions->fTraceMeasures)
    gLogIos <<
      "Appending tuplet '" << tuplet->tupletAsString () <<
      "' to measure '" << fMeasureNumber <<
      "' in voice \"" <<
      fMeasureSegmentUplink->
        getSegmentVoiceUplink ()->
          getVoiceName () <<
      "\"" <<
      endl;

  // populate measure uplink
  tuplet->setTupletMeasureUplink (this);

  // register tuplet measure number
  tuplet->
    setTupletMeasureNumber (fMeasureNumber);
  
  // register tuplet measure position in measure
  rational
    dummy = // JMI
      tuplet->
        setTupletPositionInMeasure (
          fMeasureLength);

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
// XXL    if (tupletSoundingWholeNotes == fMeasureDivisionsPerWholeMeasure)
    // tuplet->setTupletOccupiesAFullMeasure ();

  // append the tuplet to the measure elements list
  fMeasureElementsList.push_back (tuplet);

/* JMI
  // bring harmony voice to the new measure length
  fetchMeasurePartUplink->
    getPartHarmonyVoice ()->
      bringVoiceToMeasureLength (
        inputLineNumber,
        fMeasureLength);
        */
}

void msrMeasure::appendHarmonyToMeasure (S_msrHarmony harmony)
{
  int inputLineNumber =
    harmony->getInputLineNumber ();
    
  if (gGeneralOptions->fTraceHarmonies || gGeneralOptions->fTraceMeasures)
    gLogIos <<
      "Appending harmony '" << harmony->harmonyAsString () <<
      "' to measure '" << fMeasureNumber <<
      "' in voice \"" <<
      fMeasureSegmentUplink->
        getSegmentVoiceUplink ()->
          getVoiceName () <<
      "\"" <<
      ", measureLength = " << fMeasureLength <<
      endl;

  // populate measure uplink
// JMI   harmony->setHarmonyMeasureUplink (this);

  // register harmony measure number
//   harmony->
// JMI     setHarmonyMeasureNumber (fMeasureNumber);
  
  // register harmony measure position in measure
//  int dummy = // JMI
 //   harmony->
  //    setHarmonyPositionInMeasure (fMeasureLength);


/* JMI
  // fetch voice
  S_msrVoice
    voice =
      fMeasureSegmentUplink->
        getSegmentVoiceUplink ();

  // register voice as part harmonies supplied
  // this will abort if another voice is already supplying harmonies
  fetchMeasurePartUplink->
    setPartHarmoniesSupplierVoice (
      voice);

      */

      
  // fetch harmony sounding whole notes
  rational
    harmonySoundingWholeNotes =
      harmony->
        getHarmonySoundingWholeNotes ();
    
//* JMI FOO
/*
  // append a skip syllable of the same duration to the part harmony voice
  S_msrNote
    skip =
        msrNote::createSkipNote (
            inputLineNumber,
            harmonySoundingWholeNotes,
            harmonySoundingWholeNotes,
            voice->
              getVoiceStaffUplink ()->getStaffNumber (),
            voice->
              getVoicePartRelativeID ());
  
    fetchMeasurePartUplink->
      getPartHarmonyVoice ()->
        appendNoteToVoice (skip);
  */

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
}

void msrMeasure::appendHarmonyToMeasureClone (S_msrHarmony harmony)
{
  int inputLineNumber =
    harmony->getInputLineNumber ();
    
  // regular insertion in current measure
  
  if (gGeneralOptions->fTraceHarmonies || gGeneralOptions->fTraceMeasures)
    gLogIos <<
      "Appending harmony '" << harmony->harmonyAsString () <<
      "' to measure clone '" << fMeasureNumber <<
      "' in voice clone \"" <<
      fMeasureSegmentUplink->
        getSegmentVoiceUplink ()->
          getVoiceName () <<
      "\"" <<
      ", measureLength = " << fMeasureLength <<
      endl;
      
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
}

void msrMeasure::appendFiguredBassToMeasure (
  S_msrFiguredBass figuredBass)
{
  int inputLineNumber =
    figuredBass->getInputLineNumber ();
    
  if (gGeneralOptions->fTraceHarmonies || gGeneralOptions->fTraceMeasures)
    gLogIos <<
      "Appending figured bass '" << figuredBass->figuredBassAsString () <<
      "' to measure '" << fMeasureNumber <<
      "' in voice \"" <<
      fMeasureSegmentUplink->
        getSegmentVoiceUplink ()->
          getVoiceName () <<
      "\"" <<
      ", measureLength = " << fMeasureLength <<
      endl;

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
}

void msrMeasure::appendFiguredBassToMeasureClone (
  S_msrFiguredBass figuredBass)
{
  int inputLineNumber =
    figuredBass->getInputLineNumber ();
    
  // regular insertion in current measure
  
  if (gGeneralOptions->fTraceHarmonies || gGeneralOptions->fTraceMeasures)
    gLogIos <<
      "Appending figured bass '" << figuredBass->figuredBassAsString () <<
      "' to measure clone '" << fMeasureNumber <<
      "' in voice clone \"" <<
      fMeasureSegmentUplink->
        getSegmentVoiceUplink ()->
          getVoiceName () <<
      "\"" <<
      ", measureLength = " << fMeasureLength <<
      endl;
      
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
}

void msrMeasure::bringMeasureToMeasureLength (
  int      inputLineNumber,
  rational measureLength)
{
  if (gGeneralOptions->fTraceMeasures)
    gLogIos <<
      "Bringing measure length for measure '" <<
      fMeasureNumber <<
      "' in segment " <<
      fMeasureSegmentUplink->getSegmentAbsoluteNumber () << 
      " from " << fMeasureLength <<
      " to " << measureLength <<
      ", line " << inputLineNumber <<
      endl;

  if (fMeasureLength < measureLength) {
    // appending a skip to this measure to reach measureLength
    rational
      skipDuration =
        measureLength - fMeasureLength;
    
    // fetch the voice
    S_msrVoice
      voice =
        fMeasureSegmentUplink->
          getSegmentVoiceUplink ();
      
    // create the skip
    S_msrNote
      skip =
        msrNote::createSkipNote (
          inputLineNumber,
   // JMI       37, // JMI
          skipDuration,
          skipDuration,
          0, // dots number JMI ???
          voice->
            getVoiceStaffUplink ()->getStaffNumber (),
          voice->
            getVoicePartRelativeID ());

    // does the skip occupy a full measure?
    if (skipDuration == fMeasureFullMeasureLength)
      skip->
        setNoteOccupiesAFullMeasure ();
  
    // register skip's measure length
    skip->
      setNotePositionInMeasure (
        fMeasureLength);
           
    if (gGeneralOptions->fTraceMeasures || gGeneralOptions->fTraceDivisions)
      gLogIos <<
       "Appending " << skip->noteAsString () <<
       " (" << skipDuration <<
       " whole notes) to bring voice \"" << voice->getVoiceName () <<
       "\" measure '" << fMeasureNumber << "'" <<
       " from length " << fMeasureLength <<
       " to length '" << measureLength << "'" <<
        ", skipDuration = " << skipDuration <<
       endl;

    // append the skip to the measure elements list
    // only now to make it possible to remove it afterwards
    // if it happens to be the first note of a chord
    appendNoteToMeasure (skip);

/*
    // account for skip duration in measure length
    setMeasureLength (
      inputLineNumber, fMeasureLength + skipDuration);
*/
  }
}

void msrMeasure::appendGraceNotesToMeasure (
  S_msrGraceNotes graceNotes)
{
  fMeasureElementsList.push_back (graceNotes);
}
  
void msrMeasure::prependGraceNotesToMeasure (
  S_msrGraceNotes graceNotes)
{
  // in order to work around LilyPond issue 34,
  // we need to insert the skip grace notes
  // after clef, key and time signature if any

  for (
    list<S_msrElement>::const_iterator i = fMeasureElementsList.begin();
    i != fMeasureElementsList.end();
    i++ ) {

    if (
      S_msrClef clef = dynamic_cast<msrClef*>(&(**i))
      ) {
    }
  
    else if (
      S_msrKey key = dynamic_cast<msrKey*>(&(**i))
      ) {
    }
    
    else if (
      S_msrTime time = dynamic_cast<msrTime*>(&(**i))
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
}
  
void msrMeasure::appendAfterGraceNotesToMeasure (
  S_msrAfterGraceNotes afterGraceNotes)
{
  fMeasureElementsList.push_back (afterGraceNotes);
}
  
void msrMeasure::prependAfterGraceNotesToMeasure (
  S_msrAfterGraceNotes afterGraceNotes)
{
  // in order to work around LilyPond issue 34,
  // we need to insert the skip after grace notes
  // after clef, key and time signature if any

  for (
    list<S_msrElement>::const_iterator i = fMeasureElementsList.begin();
    i != fMeasureElementsList.end();
    i++ ) {

    if (
      S_msrClef clef = dynamic_cast<msrClef*>(&(**i))
      ) {
    }
  
    else if (
      S_msrKey key = dynamic_cast<msrKey*>(&(**i))
      ) {
    }
    
    else if (
      S_msrTime time = dynamic_cast<msrTime*>(&(**i))
      ) {
    }
    
    else {
       // insert afterGraceNotes before (*i) in the list
      fMeasureElementsList.insert (
        i, afterGraceNotes);

      break;
    }
  } // for
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
}

void msrMeasure::appendOtherElementToMeasure  (S_msrElement elem)
{
  fMeasureElementsList.push_back (elem);
}

void msrMeasure::removeNoteFromMeasure (
  int       inputLineNumber,
  S_msrNote note)
{  
  if (gGeneralOptions->fTraceChords || gGeneralOptions->fTraceMeasures) {
    gLogIos <<
      "Removing note:" <<
      endl;

    idtr++;
    gLogIos <<
      note->noteAsShortString ();
    idtr--;
    
    gLogIos <<
      endl <<
      " from measure '" << fMeasureNumber <<
      "' in voice \"" <<
      fMeasureSegmentUplink->
        getSegmentVoiceUplink ()->
          getVoiceName () <<
      "\"," <<
      endl;

    idtr++;
    gLogIos <<
      "fMeasureLastHandledNote:" <<
      endl <<
      fMeasureLastHandledNote <<
      endl;
    idtr--;
  }

  for (
    list<S_msrElement>::const_iterator i=fMeasureElementsList.begin();
    i!=fMeasureElementsList.end();
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
  
  msrInternalError (
    inputLineNumber,
    "cannot remove note " <<
    note <<
    " from measure " << fMeasureNumber <<
    "' in voice \"" <<
    fMeasureSegmentUplink->
      getSegmentVoiceUplink ()->
        getVoiceName () <<
    "\"," <<
    " since it has not been found");
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
    gLogIos <<
      "% --> removing last element:" <<
      endl;

    idtr++;
    gLogIos <<
      measureLastElement;
    idtr--;
    
    gLogIos <<
      endl <<
      " from measure '" <<
      fMeasureNumber <<
      "' in voice \"" <<
      fMeasureSegmentUplink->
        getSegmentVoiceUplink ()->
          getVoiceName () <<
      "\"," <<
      endl;

    idtr++;
    gLogIos <<
      "fMeasureLastHandledNote:" <<
      endl <<
      fMeasureLastHandledNote <<
      endl;
    idtr--;
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
    
  if (gGeneralOptions->fTraceMeasures) {
    gLogIos <<
      "Finalizing measure " << fMeasureNumber <<
      " in voice \"" << voice->getVoiceName () <<
      "\", line " << inputLineNumber <<
      endl;

    idtr++;

    const int fieldWidth = 26;
    
    gLogIos << left <<
      setw (fieldWidth) <<
      "measureLength" << " = " << fMeasureLength <<
      endl <<
      setw (fieldWidth) <<
      "partMeasureLengthHighTide" << " = " <<
      partMeasureLengthHighTide <<
      endl;
        
    idtr--;
  }

  if (fMeasureKind != msrMeasure::kSenzaMisuraMeasureKind) {
    
    if (fMeasureLength < partMeasureLengthHighTide) {
      // appending a skip to this measure to reach partMeasureLengthHighTide
      rational
        skipDuration =
          partMeasureLengthHighTide - fMeasureLength;
  
      /* JMI
        partMeasureLengthHighTide > fMeasureFullMeasureLength // + 1 // JMI ???
          ? partMeasureLengthHighTide - fMeasureLength
          : fMeasureFullMeasureLength - fMeasureLength;
          */
      
      // create the skip
      S_msrNote
        skip =
          msrNote::createSkipNote (
            inputLineNumber,
   // JMI         49, // JMI
            skipDuration,
            skipDuration,
            0, // dots number JMI ???
            voice->
              getVoiceStaffUplink ()->getStaffNumber (),
            voice->
              getVoicePartRelativeID ());
  
      // does the skip occupy a full measure?
      if (skipDuration == fMeasureFullMeasureLength)
        skip->
          setNoteOccupiesAFullMeasure ();
    
      // register skip's position in measure
      skip->
        setNotePositionInMeasure (fMeasureLength);
             
      if (gGeneralOptions->fTraceMeasures)
        gLogIos <<
         "Appending '" << skip->noteAsString () <<
         " (" << skipDuration << " whole notes)'" <<
         " to finalize \"" << voice->getVoiceName () <<
         "\" measure: @" << fMeasureNumber << ":" << fMeasureLength <<
         " % --> @" << fMeasureNumber << // JMI
         ":" << partMeasureLengthHighTide <<
          ", skipDuration = " << skipDuration <<
         endl;
  
      // append the skip to the measure elements list
      // only now to make it possible to remove it afterwards
      // if it happens to be the first note of a chord
      appendNoteToMeasure (skip);
    }
  
    // determine the measure kind
    if (fMeasureLength == fMeasureFullMeasureLength) {
      // full measure
      if (gGeneralOptions->fTraceMeasures) {
        gLogIos <<
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
      if (gGeneralOptions->fTraceMeasures) {
        gLogIos <<
        "Measure '" << fMeasureNumber <<
        "' in voice \"" << voice->getVoiceName () <<
        "\", is **empty**" <<
        ", line " << inputLineNumber <<
        endl;
      }
  
      setMeasureKind (
        kEmptyMeasureKind);
    }
    
    else if (fMeasureLength < fMeasureFullMeasureLength) {
      //  incomplete measure
      switch (fMeasureFirstInSegmentKind) {
        case msrMeasure::kMeasureFirstInSegmentYes:
          if (gGeneralOptions->fTraceMeasures) {
            gLogIos <<
            "Measure '" << fMeasureNumber <<
            "' in voice \"" << voice->getVoiceName () <<
            "\", is **incomplete left**" <<
            ", line " << inputLineNumber <<
            endl;
          }
      
          setMeasureKind (
            kUpbeatMeasureKind);
          break;
          
        case msrMeasure::kMeasureFirstInSegmentNo:
          if (gGeneralOptions->fTraceMeasures) {
            gLogIos <<
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
  
    else if (fMeasureLength > fMeasureFullMeasureLength) {
      // overfull measure
      if (gGeneralOptions->fTraceMeasures) {
        gLogIos <<
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

  else {
    // leave measure as it it
  }
}

void msrMeasure::acceptIn (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors)
    gLogIos <<
      "% ==> msrMeasure::acceptIn()" <<
      endl;
      
  if (visitor<S_msrMeasure>*
    p =
      dynamic_cast<visitor<S_msrMeasure>*> (v)) {
        S_msrMeasure elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors)
          gLogIos <<
            "% ==> Launching msrMeasure::visitStart()" <<
             endl;
        p->visitStart (elem);
  }
}

void msrMeasure::acceptOut (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors)
    gLogIos <<
      "% ==> msrMeasure::acceptOut()" <<
      endl;

  if (visitor<S_msrMeasure>*
    p =
      dynamic_cast<visitor<S_msrMeasure>*> (v)) {
        S_msrMeasure elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors)
          gLogIos <<
            "% ==> Launching msrMeasure::visitEnd()" <<
            endl;
        p->visitEnd (elem);
  }
}

void msrMeasure::browseData (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors)
    gLogIos <<
      "% ==> msrMeasure::browseData()" <<
      endl;

  for (
    list<S_msrElement>::const_iterator i = fMeasureElementsList.begin();
    i != fMeasureElementsList.end();
    i++) {
    // browse the element
    msrBrowser<msrElement> browser (v);
    browser.browse (*(*i));
  } // for

  if (gMsrOptions->fTraceMsrVisitors)
    gLogIos <<
      "% <== msrMeasure::browseData()" <<
      endl;
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
      result = "**unkwnown kind**";
      break;
    case msrMeasure::kFullMeasureKind:
      result = "full";
      break;
    case msrMeasure::kUpbeatMeasureKind:
      result = "**upbeat**";
      break;
    case msrMeasure::kUnderfullMeasureKind:
      result = "**under full**";
      break;
    case msrMeasure::kOverfullMeasureKind:
      result = "**over full**";
      break;
    case msrMeasure::kSenzaMisuraMeasureKind:
      result = "**senza misura**";
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
    msrMeasureFirstInSegmentKind measureFirstInSegmentKind
)
{
  string result;

  switch (measureFirstInSegmentKind) {
    case msrMeasure::kMeasureFirstInSegmentYes:
      result = "first in segment";
      break;
    case msrMeasure::kMeasureFirstInSegmentNo:
      result = "not first in segment";
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
    "Measure " << fMeasureNumber <<
    ", " << measureKindAsString () <<
/* JMI
    ", measureLengthAsMSRString: " <<
    measureLengthAsMSRString () <<
    ", measureFullMeasureLengthAsMSRString: " <<
    measureFullMeasureLengthAsMSRString () <<
    ", " << fMeasureFullMeasureLength << " per full measure" <<
    */
    ", " <<
    msrMeasure::measureFirstInSegmentKindAsString (
      fMeasureFirstInSegmentKind) << 
    ", " <<
    singularOrPlural (
      fMeasureElementsList.size (), "element", "elements") <<
    ", line " << fInputLineNumber <<
    endl;

  idtr++;
  os <<
    "MeasureSegmentUplink: " <<
    fMeasureSegmentUplink->segmentAsShortString () <<
    endl <<
    "Length: " << fMeasureLength << " whole notes" <<
    endl <<
    "Full measure length: " << fMeasureFullMeasureLength << " whole notes" <<
    endl;
  idtr--;

  if (fMeasureElementsList.size ()) {
    os << endl;
    
    idtr++;
    
    list<S_msrElement>::const_iterator
      iBegin = fMeasureElementsList.begin(),
      iEnd   = fMeasureElementsList.end(),
      i      = iBegin;
    for ( ; ; ) {
      os << (*i);
        // JMI << endl;
      if (++i == iEnd) break;
  //    os << idtr;
    } // for
    
    idtr--;
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
    segmentVoicekUplink != 0,
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
  
  if (gGeneralOptions->fTraceSegments) {
    gLogIos <<
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
  if (gGeneralOptions->fTraceSegments) {
    gLogIos <<
      "==> Creating a newborn clone of segment " <<
      segmentAsString () <<
      endl;
  }

  // sanity check
  msrAssert(
    containingVoice != 0,
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
  if (gGeneralOptions->fTraceSegments) {
    gLogIos <<
      "==> Creating a deep copy of segment " <<
      segmentAsString () <<
      endl;
  }

  // sanity check
  msrAssert(
    containingVoice != 0,
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
     if (gGeneralOptions->fTraceVoices) {
      gLogIos <<
        "There are " <<
        numberOfSegmentMeasures <<
        " measures in segment to be deep copied" <<
        endl;
    }
    
    for (
      list<S_msrMeasure>::const_iterator i = fSegmentMeasuresList.begin();
      i != fSegmentMeasuresList.end();
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
    if (gGeneralOptions->fTraceSegments) {
      gLogIos <<
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
  string measureNumber)
{
  if (gGeneralOptions->fTraceMeasures || gGeneralOptions->fTraceSegments)
    gLogIos <<
      "==> Creating and appending a measure '" << measureNumber <<
      "', to segment '" << segmentAsString () <<
      "' in voice \"" <<
      fSegmentVoiceUplink->getVoiceName () <<
      "\"" <<
      "', line " << inputLineNumber <<
      endl;

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

/* JMI
  // fetch segment last measure
  S_msrMeasure
    lastMeasure =
      fSegmentMeasuresList.back ();

  // finalize last measure
  lastMeasure->
    finalizeMeasure (
      inputLineNumber,
      measureFirstInSegmentKind);
      */
      
  // create a measure
  if (gGeneralOptions->fTraceMeasures || gGeneralOptions->fTraceSegments)
    gLogIos <<
      "==> Creating measure '" << measureNumber <<
      "' in segment " << segmentAsString () <<
      ", in voice \"" <<
      fSegmentVoiceUplink->getVoiceName () <<
      "\"" <<
      ", line " << inputLineNumber <<
      ", measure is " <<
      msrMeasure::measureFirstInSegmentKindAsString (
        measureFirstInSegmentKind) <<
      endl;

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
      setMeasureFullMeasureLengthFromTime (
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

void msrSegment::finalizeCurrentMeasureInSegment (
  int    inputLineNumber)
{
  string
    currentMeasureNumber =
      fetchSegmentPartUplink ()->
        getPartCurrentMeasureNumber ();
      
  if (gGeneralOptions->fTraceMeasures || gGeneralOptions->fTraceSegments) {
    gLogIos <<
      "Finalizing current measure '" <<
      currentMeasureNumber <<
      "'in segment " <<
      segmentAsString () <<
      ", in voice \"" <<
      fSegmentVoiceUplink->getVoiceName () <<
      "\"" <<
      ", line " << inputLineNumber <<
      endl;
  }

  // should a measure be appended to the segment
  // to match measureNumber?
  bool doCreateAndAppendAMeasure = false;
  
  if (! fSegmentMeasuresList.size ()) {
    doCreateAndAppendAMeasure = true;
  }
  else if (
    fSegmentMeasuresList.back ()->getMeasureNumber ()
        !=
     currentMeasureNumber) {
    doCreateAndAppendAMeasure = true;
  }
  
  if (doCreateAndAppendAMeasure)
    createMeasureAndAppendItToSegment (
      inputLineNumber,
      currentMeasureNumber);
  
  // finalize segment's last measure
  fSegmentMeasuresList.back ()->
    finalizeMeasure (
      inputLineNumber);
        
    /* JMI
  else {
      
    stringstream s;

    s <<
      "cannot finalize current measure in segment " <<
      segmentAsString () <<
      " since it doesn't contain any measure";

    msrInternalError (
      inputLineNumber, s.str ());
  }
      */

  if (gGeneralOptions->fTraceMeasures || gGeneralOptions->fTraceSegments) { // JMI
    gLogIos <<
      "Current measure '" <<
      currentMeasureNumber <<
      "'in segment " <<
      segmentAsString () <<
      ", in voice \"" <<
      fSegmentVoiceUplink->getVoiceName () <<
      "\"" <<
      ", line " << inputLineNumber <<
      " contains:" <<
      endl <<
      fSegmentMeasuresList.back ();
  }
}

void msrSegment::appendClefToSegment (S_msrClef clef)
{
  if (gGeneralOptions->fTraceClefs || gGeneralOptions->fTraceSegments) {
    gLogIos <<
      "Appending clef '" << clef->clefAsString () <<
      "' to segment " << segmentAsString () <<
      ", in voice \"" <<
      fSegmentVoiceUplink->getVoiceName () <<
      "\"" <<
      endl;
  }

      /* JMI
  gLogIos << idtr; // JMI
  fetchSegmentPartUplink ()->
    print (gLogIos);
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

    gLogIos <<
      "SegmentVoiceUplink:" <<
      endl;
    idtr++;
    gLogIos <<
      fSegmentVoiceUplink <<
      endl;
    idtr--;
    
    msrInternalError (
      clef->getInputLineNumber (),
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
  if (gGeneralOptions->fTraceKeys || gGeneralOptions->fTraceSegments) {
    gLogIos <<
      "Appending key '" << key->keyAsString () <<
      "' to segment " << segmentAsString () <<
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
  if (gGeneralOptions->fTraceTimes || gGeneralOptions->fTraceSegments) {
    gLogIos <<
      "Appending time:" <<
      endl;

    idtr++;

    gLogIos <<
      time;

    idtr--;

    gLogIos <<
      "to segment " << segmentAsString () <<
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
  if (gGeneralOptions->fTraceTimes || gGeneralOptions->fTraceSegments) {
    gLogIos <<
      "Appending time:" <<
      endl;

    idtr++;

    gLogIos <<
      time;

    idtr--;

    gLogIos <<
      "to segment clone " << segmentAsString () <<
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
  if (gGeneralOptions->fTraceHarmonies || gGeneralOptions->fTraceSegments)
    gLogIos <<
      "Appending harmony " << harmony->harmonyAsString () <<
      " to segment " << segmentAsString () <<
      "' in voice \"" <<
      fSegmentVoiceUplink->getVoiceName () <<
      "\"" <<
      endl;
      
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
  if (gGeneralOptions->fTraceHarmonies || gGeneralOptions->fTraceSegments)
    gLogIos <<
      "Appending harmony " << harmony->harmonyAsString () <<
      " to segment clone " << segmentAsString () <<
      "' in voice clone \"" <<
      fSegmentVoiceUplink->getVoiceName () <<
      "\"" <<
      endl;
      
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
  if (gGeneralOptions->fTraceFiguredBass || gGeneralOptions->fTraceSegments)
    gLogIos <<
      "Appending figured bass " << figuredBass->figuredBassAsString () <<
      " to segment " << segmentAsString () <<
      "' in voice \"" <<
      fSegmentVoiceUplink->getVoiceName () <<
      "\"" <<
      endl;
      
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
  if (gGeneralOptions->fTraceFiguredBass || gGeneralOptions->fTraceSegments)
    gLogIos <<
      "Appending figured bass " << figuredBass->figuredBassAsString () <<
      " to segment clone " << segmentAsString () <<
      "' in voice clone \"" <<
      fSegmentVoiceUplink->getVoiceName () <<
      "\"" <<
      endl;
      
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
  if (gGeneralOptions->fTraceHarmonies || gGeneralOptions->fTraceSegments)
    gLogIos <<
      "Appending segno " <<
      " to segment " << segmentAsString () <<
      "' in voice \"" <<
      fSegmentVoiceUplink->getVoiceName () <<
      "\"" <<
      endl;
      
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
  if (gGeneralOptions->fTraceHarmonies || gGeneralOptions->fTraceSegments)
    gLogIos <<
      "Appending coda " <<
      " to segment " << segmentAsString () <<
      "' in voice \"" <<
      fSegmentVoiceUplink->getVoiceName () <<
      "\"" <<
      endl;
      
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
  if (gGeneralOptions->fTraceHarmonies || gGeneralOptions->fTraceSegments)
    gLogIos <<
      "Appending eyeglasses " <<
      " to segment " << segmentAsString () <<
      "' in voice \"" <<
      fSegmentVoiceUplink->getVoiceName () <<
      "\"" <<
      endl;
      
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
  if (gGeneralOptions->fTraceHarmonies || gGeneralOptions->fTraceSegments)
    gLogIos <<
      "Appending pedal " <<
      " to segment " << segmentAsString () <<
      "' in voice \"" <<
      fSegmentVoiceUplink->getVoiceName () <<
      "\"" <<
      endl;
      
  // sanity check
  msrAssert (
    fSegmentMeasuresList.size () > 0,
    "fSegmentMeasuresList is empty");
    
  // append it to this segment
  fSegmentMeasuresList.back ()->
    appendPedalToMeasure (pedal);
}

void msrSegment::appendTransposeToSegment (
  S_msrTranspose transpose)
{
  if (gGeneralOptions->fTraceTranspositions || gGeneralOptions->fTraceSegments) {
    gLogIos <<
      "Appending transpose " <<
      " to segment " << segmentAsString () <<
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

void msrSegment::appendStaffDetailsToSegment (
  S_msrStaffDetails staffDetails)
{
  if (gGeneralOptions->fTraceHarmonies || gGeneralOptions->fTraceSegments)
    gLogIos <<
      "Appending staff details " <<
      " to segment " << segmentAsString () <<
      "' in voice \"" <<
      fSegmentVoiceUplink->getVoiceName () <<
      "\"" <<
      endl;
      
  // sanity check
  msrAssert (
    fSegmentMeasuresList.size () > 0,
    "fSegmentMeasuresList is empty");
    
  // append it to this segment
  fSegmentMeasuresList.back ()->
    appendStaffDetailsToMeasure (staffDetails);
}

void msrSegment::appendLineBreakToSegment (S_msrLineBreak lineBreak)
{
  if (gGeneralOptions->fTraceHarmonies || gGeneralOptions->fTraceSegments)
    gLogIos <<
      "Appending break " <<
      " to segment " << segmentAsString () <<
      "' in voice \"" <<
      fSegmentVoiceUplink->getVoiceName () <<
      "\"" <<
      endl;
      
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
  if (gGeneralOptions->fTraceHarmonies || gGeneralOptions->fTraceSegments)
    gLogIos <<
      "Appending break " <<
      " to segment " << segmentAsString () <<
      "' in voice \"" <<
      fSegmentVoiceUplink->getVoiceName () <<
      "\"" <<
      endl;
      
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
  if (gGeneralOptions->fTraceHarmonies || gGeneralOptions->fTraceSegments)
    gLogIos <<
      "Appending bar number check " <<
      " to segment " << segmentAsString () <<
      "' in voice \"" <<
      fSegmentVoiceUplink->getVoiceName () <<
      "\"" <<
      endl;
      
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
  if (gGeneralOptions->fTraceHarmonies || gGeneralOptions->fTraceSegments)
    gLogIos <<
      "Appending tempo " <<
      " to segment " << segmentAsString () <<
      "' in voice \"" <<
      fSegmentVoiceUplink->getVoiceName () <<
      "\"" <<
      endl;
      
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
  if (gGeneralOptions->fTraceHarmonies || gGeneralOptions->fTraceSegments)
    gLogIos <<
      "Appending rehearsal " <<
      " to segment " << segmentAsString () <<
      "' in voice \"" <<
      fSegmentVoiceUplink->getVoiceName () <<
      "\"" <<
      endl;
      
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
  if (gGeneralOptions->fTraceHarmonies || gGeneralOptions->fTraceSegments)
    gLogIos <<
      "Appending octave shift '" <<
      octaveShift->octaveShiftKindAsString () <<
      "' to segment " << segmentAsString () <<
      "' in voice \"" <<
      fSegmentVoiceUplink->getVoiceName () <<
      "\"" <<
      endl;
      
  // sanity check
  msrAssert (
    fSegmentMeasuresList.size () > 0,
    "fSegmentMeasuresList is empty");
    
  // append it to this segment
  fSegmentMeasuresList.back ()->
    appendOctaveShiftToMeasure (octaveShift);
}

void msrSegment::appendAccordionRegistrationToSegment (
  S_msrAccordionRegistration
    accordionRegistration)
{
  if (gGeneralOptions->fTraceGeneral || gGeneralOptions->fTraceSegments)
    gLogIos <<
      "Appending accordion registration '" <<
      accordionRegistration->accordionRegistrationAsString () <<
      "' to segment " << segmentAsString () <<
      "' in voice \"" <<
      fSegmentVoiceUplink->getVoiceName () <<
      "\"" <<
      endl;
      
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
  if (gGeneralOptions->fTraceGeneral || gGeneralOptions->fTraceSegments)
    gLogIos <<
      "Appending staff pedals tuning '" <<
      harpPedalsTuning->harpPedalsTuningAsString () <<
      "' to segment " << segmentAsString () <<
      "' in voice \"" <<
      fSegmentVoiceUplink->getVoiceName () <<
      "\"" <<
      endl;
      
  // sanity check
  msrAssert (
    fSegmentMeasuresList.size () > 0,
    "fSegmentMeasuresList is empty");
    
  // append it to this segment
  fSegmentMeasuresList.back ()->
    appendHarpPedalsTuningToMeasure (
      harpPedalsTuning);
}

void msrSegment::bringSegmentToMeasureLength (
  int      inputLineNumber,
  rational measureLength)
{
  if (gGeneralOptions->fTraceSegments || gGeneralOptions->fTraceMeasures)
    gLogIos <<
      "Bringing measure length for segment '" <<
      fSegmentAbsoluteNumber <<
      "' in voice \"" <<
      fSegmentVoiceUplink->getVoiceName () <<
      "\"," <<
      "' to " << measureLength <<
      ", line " << inputLineNumber <<
      endl;

  if (false && fSegmentMeasuresList.size () == 0) {
    stringstream s;

    s <<
      "SegmentMeasuresList is empty"  <<
      " in segment '" <<
      fSegmentAbsoluteNumber <<
      "' in voice \"" <<
      fSegmentVoiceUplink->getVoiceName () <<
      "\"";

    gLogIos <<
      "SegmentVoiceUplink:" <<
      endl;
    idtr++;
    gLogIos <<
      fSegmentVoiceUplink <<
      endl;
    idtr--;
    
    gLogIos <<
      "Part:" <<
      endl;
    idtr++;
    gLogIos <<
      fSegmentVoiceUplink->fetchVoicePartUplink () <<
      endl;
    idtr--;
    
    msrInternalError (
      inputLineNumber,
      s.str ());
  }

  if (fSegmentMeasuresList.size ()) { // JMI BOFBOF
    // bring last measure to this length
    fSegmentMeasuresList.back ()->
      bringMeasureToMeasureLength (
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
    
  if (gGeneralOptions->fTraceMeasures || gGeneralOptions->fTraceSegments) {
    gLogIos <<
      "Appending measure " << measureNumber <<
      " to segment " << segmentAsString ();

    if (fSegmentMeasuresList.size () == 0)
      gLogIos <<
        ", as first measure";
    else
      gLogIos <<
      ", after measure number '" << currentMeasureNumber << "'";

    gLogIos <<
      "' in voice \"" <<
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
    
  if (gGeneralOptions->fTraceMeasures || gGeneralOptions->fTraceSegments) {
    gLogIos <<
      "Prepending measure " << measureNumber <<
      " to segment " << segmentAsString ();

    if (fSegmentMeasuresList.size () == 0)
      gLogIos <<
        ", as first measure";
/* JMI
    else
      gLogIos <<
      ", after measure number '" << currentMeasureNumber << "'";
*/

    gLogIos <<
      "' in voice \"" <<
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
      inputLineNumber,
      s.str ());
  }

  else { // JMI TEMP
    // append measure to the segment
    fSegmentMeasuresList.push_back (measure);
  }
}

void msrSegment::appendMeasureToSegmentIfNotYetDone ( // JMI
  int    inputLineNumber,
  string measureNumber)
{
  /* JMI BOF
  if (! fSegmentMeasuresList.size ()) {
    // create a new measure
    if (gGeneralOptions->fTraceMeasures || gGeneralOptions->fTraceSegments)
      gLogIos <<
        "Appending a new measure '" << measureNumber <<
        "' to segment '" << segmentAsString () << "'" <<
        "' in voice \"" <<
        fSegmentVoiceUplink->getVoiceName () <<
        "\"," <<
        ", line " << inputLineNumber <<
        endl;

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
  if (gGeneralOptions->fTraceMeasures || gGeneralOptions->fTraceSegments)
    gLogIos <<
      "Prepending barline '" << barline->barlineAsString () <<
      "' to segment '" << segmentAsString () << "'" <<
      "' in voice \"" <<
      fSegmentVoiceUplink->getVoiceName () <<
      "\"," <<
      endl;

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
  if (gGeneralOptions->fTraceMeasures || gGeneralOptions->fTraceSegments)
    gLogIos <<
      "Appending barline '" << barline->barlineAsString () <<
      "' to segment '" << segmentAsString () << "'" <<
      "' in voice \"" <<
      fSegmentVoiceUplink->getVoiceName () <<
      "\"," <<
      endl;

  if (! fSegmentMeasuresList.size ()) {// JMI
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
  if (gGeneralOptions->fTraceMeasures || gGeneralOptions->fTraceSegments)
    gLogIos <<
      "Appending bar check " << barCheck->barCheckAsString () <<
      " to segment '" << segmentAsString () << "'" <<
      "' in voice \"" <<
      fSegmentVoiceUplink->getVoiceName () <<
      "\"," <<
      endl;
      
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
    gGeneralOptions->fTraceVoices
      ||
    gGeneralOptions->fTraceStaves
      ||
    gGeneralOptions->fTraceSegments) {
    gLogIos <<
      "Append voice staff change " <<
      voiceStaffChange->getNewStaff () <<
      " to segment " <<
      " \"" << segmentAsString () << "\"" <<
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

void msrSegment::appendDoubleTremoloToSegment ( // XXL
  S_msrDoubleTremolo doubleTremolo)
{
  if (gGeneralOptions->fTraceTremolos || gGeneralOptions->fTraceSegments)
    gLogIos <<
      "Appending double tremolo " <<
      doubleTremolo->doubleTremoloAsShortString () <<
      " to segment '" << segmentAsString () << "'" <<
      "' in voice \"" <<
      fSegmentVoiceUplink->getVoiceName () <<
      "\"," <<
      endl;
      
  // sanity check
  msrAssert (
    fSegmentMeasuresList.size () > 0,
    "fSegmentMeasuresList is empty");
    
  fSegmentMeasuresList.back ()->
    appendDoubleTremoloToMeasure (doubleTremolo);
}

void msrSegment::appendMeasureRepeatToSegment (
  S_msrMeasureRepeat measureRepeat)
{
  if (gGeneralOptions->fTraceRepeats || gGeneralOptions->fTraceSegments)
    gLogIos <<
      "Appending measure repeat '" <<
      measureRepeat->measureRepeatAsString () <<
      "' to segment '" << segmentAsString () << "'" <<
      "' in voice \"" <<
      fSegmentVoiceUplink->getVoiceName () <<
      "\"," <<
      endl;
      
  // sanity check
  msrAssert (
    fSegmentMeasuresList.size () > 0,
    "fSegmentMeasuresList is empty");
    
  fSegmentMeasuresList.back ()->
    appendMeasureRepeatToMeasure (measureRepeat);
}

void msrSegment::appendMultipleRestToSegment (
  S_msrMultipleRest multipleRest)
{
  if (gGeneralOptions->fTraceRepeats || gGeneralOptions->fTraceSegments)
    gLogIos <<
      "Appending multiple rest '" <<
      multipleRest->multipleRestAsString () <<
      "' to segment '" << segmentAsString () << "'" <<
      "' in voice \"" <<
      fSegmentVoiceUplink->getVoiceName () <<
      "\"," <<
      endl;
      
  // sanity check
  msrAssert (
    fSegmentMeasuresList.size () > 0,
    "fSegmentMeasuresList is empty");
    
  fSegmentMeasuresList.back ()->
    appendMultipleRestToMeasure (multipleRest);
}

void msrSegment::appendChordToSegment (S_msrChord chord) // XXL
{
  // sanity check
  msrAssert (
    fSegmentMeasuresList.size () > 0,
    "fSegmentMeasuresList is empty");
    
  fSegmentMeasuresList.back ()->
    appendChordToMeasure (chord);
}

void msrSegment::appendTupletToSegment (S_msrTuplet tuplet) // XXL
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

/* JMI
void msrSegment::removeElementFromSegment (
  S_msrElement elem)
{
  for (
    list<S_msrElement>::const_iterator i = fSegmentMeasuresList.begin();
    i != fSegmentMeasuresList.end();
    i++) {
    if ((*i) == elem) {
      fSegmentMeasuresList.erase (i);
      break;
    }
  } // for
}
*/

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
      segmentAsString () <<
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
    msrInternalError (
      inputLineNumber,
      "cannot remove note from segment " <<
      segmentAsString () <<
      "' in voice \"" <<
      fSegmentVoiceUplink->getVoiceName () <<
      "\"," <<
      " since it is empty");
  }
}

S_msrMeasure msrSegment::removeLastMeasureFromSegment (
  int inputLineNumber)
{
  if (gGeneralOptions->fTraceMeasures || gGeneralOptions->fTraceSegments) {
    gLogIos <<
      "Removing last measure from segment '" <<
      fSegmentAbsoluteNumber <<
      "'" <<
      endl;
  }

  if (! fSegmentMeasuresList.size ()) {
    stringstream s;

    s <<
      "cannot remove last measure from from segment '" <<
      fSegmentAbsoluteNumber <<
      "' in voice \"" <<
      fSegmentVoiceUplink->getVoiceName () <<
      "\"," <<
      "' since it is empty";

    msrInternalError (
      inputLineNumber, s.str ());
  }

  S_msrMeasure
    result =
      fSegmentMeasuresList.back ();

  if (gGeneralOptions->fTraceMeasures || gGeneralOptions->fTraceSegments) {
    gLogIos <<
      "This measure contains:";

    idtr++;

    gLogIos <<
      result;

    idtr--;
  }

  fSegmentMeasuresList.pop_back ();
  
  return result;
}

void msrSegment::acceptIn (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors)
    gLogIos <<
      "% ==> msrSegment::acceptIn()" <<
      endl;
      
  if (visitor<S_msrSegment>*
    p =
      dynamic_cast<visitor<S_msrSegment>*> (v)) {
        S_msrSegment elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors)
          gLogIos <<
            "% ==> Launching msrSegment::visitStart()" <<
             endl;
        p->visitStart (elem);
  }
}

void msrSegment::acceptOut (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors)
    gLogIos <<
      "% ==> msrSegment::acceptOut()" <<
      endl;

  if (visitor<S_msrSegment>*
    p =
      dynamic_cast<visitor<S_msrSegment>*> (v)) {
        S_msrSegment elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors)
          gLogIos <<
            "% ==> Launching msrSegment::visitEnd()" <<
            endl;
        p->visitEnd (elem);
  }
}

void msrSegment::browseData (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors)
    gLogIos <<
      "% ==> msrSegment::browseData()" <<
      endl;

/* JMI
  gLogIos <<
    tab <<
    "==========>>> " <<
    fSegmentMeasuresList.size () << " measures" << endl;
    */
    
  for (
    list<S_msrMeasure>::const_iterator i = fSegmentMeasuresList.begin();
    i != fSegmentMeasuresList.end();
    i++) {
    // browse the element
    msrBrowser<msrMeasure> browser (v);
    browser.browse (*(*i));
  } // for

  if (gMsrOptions->fTraceMsrVisitors)
    gLogIos <<
      "% <== msrSegment::browseData()" <<
      endl;
}

string msrSegment::segmentAsString ()
{
  stringstream s;

  s <<
    "Segment " <<
    fSegmentAbsoluteNumber <<
    " in voice \"" <<
    fSegmentVoiceUplink->getVoiceName () <<
    "\"";
  
  if (! fSegmentMeasuresList.size ())
    s <<
      " (0 measures)";
  else
    s <<
      " (" <<
      singularOrPlural (
        fSegmentMeasuresList.size (), "measure", " measures") <<
      ")";

  return s.str ();
}

string msrSegment::segmentAsShortString ()
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
      fSegmentMeasuresList.size(), "measure", "measures") <<
    endl;

  idtr++;

  const int fieldWidth = 20;
  
  os <<
    setw (fieldWidth) << "SegmentVoiceUplink" << " : " <<
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
      iBegin = fSegmentMeasuresList.begin(),
      iEnd   = fSegmentMeasuresList.end(),
      i      = iBegin;
      
    for ( ; ; ) {
      os << (*i);
      if (++i == iEnd) break;
 // JMI     os << endl;
    } // for
  }
    
  idtr--;
}

//______________________________________________________________________________
S_msrRepeatCommonPart msrRepeatCommonPart::create (
  int                 inputLineNumber,
  S_msrSegment        repeatCommonPartSegment,
  S_msrRepeat         repeatUplink)
{
  msrRepeatCommonPart* o =
    new msrRepeatCommonPart (
      inputLineNumber,
      repeatCommonPartSegment,
      repeatUplink);
  assert(o!=0);
  return o;
}

msrRepeatCommonPart::msrRepeatCommonPart (
  int                 inputLineNumber,
  S_msrSegment        repeatCommonPartSegment,
  S_msrRepeat         repeatUplink)
    : msrElement (inputLineNumber)
{
  // sanity check
  msrAssert (
    repeatCommonPartSegment != 0,
    "repeatCommonPartSegment is null");
    
  fRepeatCommonPartSegment = repeatCommonPartSegment;
  
  // sanity check
  msrAssert (
    repeatUplink != 0,
    "repeatUplink is null");
    
  fRepeatCommonPartRepeatUplink = repeatUplink;
}

msrRepeatCommonPart::~msrRepeatCommonPart()
{}

S_msrRepeatCommonPart msrRepeatCommonPart::createRepeatCommonPartNewbornClone (
  S_msrRepeat containingRepeat)
{
  if (gGeneralOptions->fTraceRepeats)
    gLogIos <<
      "==> Creating a newborn clone of a " <<
      repeatCommonPartAsString () <<
      endl;
  
  // sanity check
  msrAssert(
    containingRepeat != 0,
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
  if (gGeneralOptions->fTraceRepeats)
    gLogIos <<
      "==> Creating a newborn clone of a " <<
      repeatCommonPartAsString () <<
      endl;
  
  // sanity check
  msrAssert(
    containingRepeat != 0,
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

/* JMI
void msrRepeatCommonPart::appendElementToRepeatCommonPart (
  S_msrElement elem) // JMI ???
{
  fRepeatCommonPartSegment->
    appendOtherElementToSegment (elem);
}
*/

void msrRepeatCommonPart::acceptIn (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors)
    gLogIos <<
      "% ==> msrRepeatCommonPart::acceptIn()" <<
      endl;
      
  if (visitor<S_msrRepeatCommonPart>*
    p =
      dynamic_cast<visitor<S_msrRepeatCommonPart>*> (v)) {
        S_msrRepeatCommonPart elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors)
          gLogIos <<
            "% ==> Launching msrRepeatCommonPart::visitStart()" <<
             endl;
        p->visitStart (elem);
  }
}

void msrRepeatCommonPart::acceptOut (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors)
    gLogIos <<
      "% ==> msrRepeatCommonPart::acceptOut()" <<
      endl;

  if (visitor<S_msrRepeatCommonPart>*
    p =
      dynamic_cast<visitor<S_msrRepeatCommonPart>*> (v)) {
        S_msrRepeatCommonPart elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors)
          gLogIos <<
            "% ==> Launching msrRepeatCommonPart::visitEnd()" <<
            endl;
        p->visitEnd (elem);
  }
}

void msrRepeatCommonPart::browseData (basevisitor* v)
{
  // browse the segment
  msrBrowser<msrSegment> browser (v);
  browser.browse (*fRepeatCommonPartSegment);
}

ostream& operator<< (ostream& os, const S_msrRepeatCommonPart& elt)
{
  elt->print (os);
  return os;
}

string msrRepeatCommonPart::repeatCommonPartAsString () const
{
  stringstream s;

  s <<
    "RepeatCommonPart" <<
    ", line " << fInputLineNumber <<
    endl;

  return s.str ();
}

void msrRepeatCommonPart::print (ostream& os)
{
  os <<
    endl <<
    repeatCommonPartAsString () <<
    endl;
 
  idtr++;

  os <<
    fRepeatCommonPartSegment;

  idtr--;
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

S_msrRepeatEnding msrRepeatEnding::createRepeatEndingNewbornClone (
  S_msrRepeat containingRepeat)
{
  if (gGeneralOptions->fTraceRepeats)
    gLogIos <<
      "==> Creating a newborn clone of a " <<
      repeatEndingAsString () <<
      endl;
  
  // sanity check
  msrAssert(
    containingRepeat != 0,
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
  if (gGeneralOptions->fTraceRepeats)
    gLogIos <<
      "==> Creating a newborn clone of a " <<
      repeatEndingAsString () <<
      endl;
  
  // sanity check
  msrAssert(
    containingRepeat != 0,
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

/* JMI
void msrRepeatEnding::appendElementToRepeatEnding (
  S_msrElement elem) // JMI ???
{
  fRepeatEndingSegment->
    appendOtherElementToSegment (elem);
}
*/

void msrRepeatEnding::acceptIn (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors)
    gLogIos <<
      "% ==> msrRepeatEnding::acceptIn()" <<
      endl;
      
  if (visitor<S_msrRepeatEnding>*
    p =
      dynamic_cast<visitor<S_msrRepeatEnding>*> (v)) {
        S_msrRepeatEnding elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors)
          gLogIos <<
            "% ==> Launching msrRepeatEnding::visitStart()" <<
             endl;
        p->visitStart (elem);
  }
}

void msrRepeatEnding::acceptOut (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors)
    gLogIos <<
      "% ==> msrRepeatEnding::acceptOut()" <<
      endl;

  if (visitor<S_msrRepeatEnding>*
    p =
      dynamic_cast<visitor<S_msrRepeatEnding>*> (v)) {
        S_msrRepeatEnding elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors)
          gLogIos <<
            "% ==> Launching msrRepeatEnding::visitEnd()" <<
            endl;
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

string msrRepeatEnding::repeatEndingAsString () const
{
  stringstream s;

  s <<
    "RepeatEnding, " <<
    repeatEndingKindAsString (
      fRepeatEndingKind) <<
    ", line " << fInputLineNumber <<
    ", number " << fRepeatEndingNumber <<
    ", internal number " << fRepeatEndingInternalNumber <<
    endl;

  return s.str ();
}

void msrRepeatEnding::print (ostream& os)
{
  os <<
    endl <<
    repeatEndingAsString () <<
    endl;
 
  idtr++;

  os <<
    fRepeatEndingSegment;

  idtr--;
}

//______________________________________________________________________________
S_msrRepeat msrRepeat::create (
  int        inputLineNumber,
  S_msrVoice voiceUplink)
{
  msrRepeat* o =
    new msrRepeat (
      inputLineNumber, voiceUplink);
  assert(o!=0);
  return o;
}

msrRepeat::msrRepeat (
  int        inputLineNumber,
  S_msrVoice voiceUplink)
    : msrElement (inputLineNumber)
{
  fRepeatEndingsInternalCounter = 0;
  
  // sanity check
  msrAssert(
    voiceUplink != 0,
    "voiceUplink is null");
    
  fRepeatVoiceUplink = voiceUplink;
}

msrRepeat::~msrRepeat()
{}

S_msrRepeat msrRepeat::createRepeatNewbornClone (
  S_msrVoice containingVoice)
{
  if (gGeneralOptions->fTraceRepeats)
    gLogIos <<
      "==> Creating a newborn clone of a repeat" <<
      endl;
  
  // sanity check
  msrAssert(
    containingVoice != 0,
    "containingVoice is null");
    
  S_msrRepeat
    newbornClone =
      msrRepeat::create (
        fInputLineNumber,
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
  if (gGeneralOptions->fTraceRepeats)
    gLogIos <<
      "==> Creating a deep copy of a repeat" <<
      endl;
  
  // sanity check
  msrAssert(
    containingVoice != 0,
    "containingVoice is null");
    
  S_msrRepeat
    repeatDeepCopy =
      msrRepeat::create (
        fInputLineNumber,
        containingVoice);
/*
  // common segment
  repeatDeepCopy->fRepeatCommonSegment =
    fRepeatCommonSegment->
      createSegmentDeepCopy (
        containingVoice);
  */      
  // common part
  repeatDeepCopy->fRepeatCommonPart =
    fRepeatCommonPart->
      createRepeatCommonPartDeepCopy (
        repeatDeepCopy);
        
  // repeat endings
  for (
    vector<S_msrRepeatEnding>::const_iterator i = fRepeatEndings.begin();
    i != fRepeatEndings.end();
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

void msrRepeat::setRepeatCommonPart (
  S_msrRepeatCommonPart repeatCommonPart)
{
  // sanity check
  msrAssert(
    repeatCommonPart != 0,
    "repeatCommonPart is null");
    
  if (gGeneralOptions->fTraceRepeats)
    gLogIos <<
      "Setting repeat common part with segment containing " <<
      singularOrPlural (
        repeatCommonPart->
          getRepeatCommonPartSegment ()->
            getSegmentMeasuresList ().size (),
        "measure",
        "measures") <<
      endl;
      
  fRepeatCommonPart = repeatCommonPart;
}
/*
void msrRepeat::setRepeatCommonSegment (
  S_msrSegment repeatCommonSegment)
{
  if (gGeneralOptions->fTraceRepeats)
    gLogIos <<
      "Setting repeat common segment containing " <<
      singularOrPlural (
        repeatCommonSegment->
          getSegmentMeasuresList ().size (),
        "measure",
        "measures") <<
      endl;
      
  // sanity check
  msrAssert(
    repeatCommonSegment != 0,
    "repeatCommonSegment is null");
    
  fRepeatCommonSegment = repeatCommonSegment;
}
*/

void msrRepeat::addRepeatEnding (
  S_msrRepeatEnding repeatEnding)
{
  // sanity check
  msrAssert(
    repeatEnding != 0,
    "repeatEnding is null");
    
  if (gGeneralOptions->fTraceRepeats)
    gLogIos <<
      "Adding ending '" <<
      repeatEnding->repeatEndingAsString () <<
      "' to repeat" <<
      endl;
      
  fRepeatEndings.push_back (repeatEnding);

  // set repeat ending internal number
  repeatEnding->
    setRepeatEndingInternalNumber (
      ++ fRepeatEndingsInternalCounter);
}

void msrRepeat::acceptIn (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors)
    gLogIos <<
      "% ==> msrRepeat::acceptIn()" <<
      endl;
      
  if (visitor<S_msrRepeat>*
    p =
      dynamic_cast<visitor<S_msrRepeat>*> (v)) {
        S_msrRepeat elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors)
          gLogIos <<
            "% ==> Launching msrRepeat::visitStart()" <<
             endl;
        p->visitStart (elem);
  }
}

void msrRepeat::acceptOut (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors)
    gLogIos <<
      "% ==> msrRepeat::acceptOut()" <<
      endl;

  if (visitor<S_msrRepeat>*
    p =
      dynamic_cast<visitor<S_msrRepeat>*> (v)) {
        S_msrRepeat elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors)
          gLogIos <<
            "% ==> Launching msrRepeat::visitEnd()" <<
            endl;
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
    vector<S_msrRepeatEnding>::const_iterator i = fRepeatEndings.begin();
    i != fRepeatEndings.end();
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

void msrRepeat::print (ostream& os)
{
  os <<
    endl <<
    "Repeat" <<
    ", line " << fInputLineNumber <<
    " (" <<
    singularOrPlural (
      fRepeatEndings.size(), "repeat ending", "repeat endings") <<
    ")" <<
    endl;
  
  idtr++;
  
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
    idtr++;
    
    os <<
      fRepeatCommonSegment <<
      endl;

    idtr--;
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
    idtr++;
    
    vector<S_msrRepeatEnding>::const_iterator
      iBegin = fRepeatEndings.begin(),
      iEnd   = fRepeatEndings.end(),
      i      = iBegin;
      
    for ( ; ; ) {
      // print the repeat ending
      os << (*i);
      if (++i == iEnd) break;
  // JMI    os << endl;
    } // for

    idtr--;
  }

 // JMI os << endl;
      
  idtr--;
}

//______________________________________________________________________________
S_msrMeasureRepeatPattern msrMeasureRepeatPattern::create (
  int        inputLineNumber,
  S_msrVoice voiceUplink)
{
  msrMeasureRepeatPattern* o =
    new msrMeasureRepeatPattern (
      inputLineNumber,
      voiceUplink);
  assert(o!=0);
  return o;
}

msrMeasureRepeatPattern::msrMeasureRepeatPattern (
  int        inputLineNumber,
  S_msrVoice voiceUplink)
    : msrElement (inputLineNumber)
{
  fMeasureRepeatPatternVoiceUplink = voiceUplink;
}

msrMeasureRepeatPattern::~msrMeasureRepeatPattern()
{}

S_msrMeasureRepeatPattern msrMeasureRepeatPattern::createMeasureRepeatPatternNewbornClone (
  S_msrVoice containingVoice)
{
  if (gGeneralOptions->fTraceRepeats)
    gLogIos <<
      "==> Creating a newborn clone of a measure repeat pattern" <<
      endl;
  
  // sanity check
  msrAssert(
    containingVoice != 0,
    "containingVoice is null");
    
  S_msrMeasureRepeatPattern
    newbornClone =
      msrMeasureRepeatPattern::create (
        fInputLineNumber,
        containingVoice);

  // segment

  // uplinks
  newbornClone->fMeasureRepeatPatternVoiceUplink =
    containingVoice;
    
  return newbornClone;
}

S_msrMeasureRepeatPattern msrMeasureRepeatPattern::createMeasureRepeatPatternDeepCopy (
  S_msrVoice containingVoice)
{
  if (gGeneralOptions->fTraceRepeats)
    gLogIos <<
      "==> Creating a newborn clone of a measure repeat pattern" <<
      endl;
  
  // sanity check
  msrAssert(
    containingVoice != 0,
    "containingVoice is null");
    
  S_msrMeasureRepeatPattern
    measureRepeatPatternDeepCopy =
      msrMeasureRepeatPattern::create (
        fInputLineNumber,
        containingVoice);

  // segment
  // fMeasureRepeatPatternSegment JMI ???
  
  // uplinks
  measureRepeatPatternDeepCopy->fMeasureRepeatPatternVoiceUplink =
    containingVoice;
    
  return measureRepeatPatternDeepCopy;
}

void msrMeasureRepeatPattern::setMeasureRepeatPatternSegment (
    S_msrSegment measureRepeatPatternSegment)
{
  if (gGeneralOptions->fTraceRepeats)
    gLogIos <<
      "Setting measure repeat pattern segment containing " <<
      singularOrPlural (
          measureRepeatPatternMeasuresNumber (),
        "measure",
        "measures") <<
      endl;
      
  // sanity check
  msrAssert (
    measureRepeatPatternSegment != 0,
    "measureRepeatPatternSegment is null");

  fMeasureRepeatPatternSegment =
    measureRepeatPatternSegment;
}

int msrMeasureRepeatPattern::measureRepeatPatternMeasuresNumber () const
{
  int result;

  if (fMeasureRepeatPatternSegment)
    result =
      fMeasureRepeatPatternSegment->
      getSegmentMeasuresList ().size ();
  else
    result = 0;

  return result;
}

void msrMeasureRepeatPattern::acceptIn (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors)
    gLogIos <<
      "% ==> msrMeasureRepeatPattern::acceptIn()" <<
      endl;
      
  if (visitor<S_msrMeasureRepeatPattern>*
    p =
      dynamic_cast<visitor<S_msrMeasureRepeatPattern>*> (v)) {
        S_msrMeasureRepeatPattern elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors)
          gLogIos <<
            "% ==> Launching msrMeasureRepeatPattern::visitStart()" <<
             endl;
        p->visitStart (elem);
  }
}

void msrMeasureRepeatPattern::acceptOut (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors)
    gLogIos <<
      "% ==> msrMeasureRepeatPattern::acceptOut()" <<
      endl;

  if (visitor<S_msrMeasureRepeatPattern>*
    p =
      dynamic_cast<visitor<S_msrMeasureRepeatPattern>*> (v)) {
        S_msrMeasureRepeatPattern elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors)
          gLogIos <<
            "% ==> Launching msrMeasureRepeatPattern::visitEnd()" <<
            endl;
        p->visitEnd (elem);
  }
}

void msrMeasureRepeatPattern::browseData (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors)
    gLogIos <<
      "% ==> msrMeasureRepeatPattern::browseData()" <<
      endl;

  if (fMeasureRepeatPatternSegment) {
    // browse the pattern segment
    msrBrowser<msrSegment> browser (v);
    browser.browse (*fMeasureRepeatPatternSegment);
  }
}

ostream& operator<< (ostream& os, const S_msrMeasureRepeatPattern& elt)
{
  elt->print (os);
  return os;
}

string msrMeasureRepeatPattern::measureRepeatPatternAsString () const
{
  stringstream s;

  s <<

    "MeasureRepeatPattern" <<
    ", line " << fInputLineNumber <<
    " (" <<
    singularOrPlural (
      measureRepeatPatternMeasuresNumber (),
      "repeated measure",
      "repeated measures") <<
    ")"; 

  return s.str ();
}

void msrMeasureRepeatPattern::print (ostream& os)
{
  os <<
    endl <<
    measureRepeatPatternAsString () <<
    "MeasureRepeatPattern" <<
    endl <<
    endl;
  
  idtr++;
  
  // print the pattern segment
  os <<
    "Pattern segment:";

  if (! fMeasureRepeatPatternSegment) {
    os <<
      " none" <<
      endl;
  }
  else {
    os <<
      endl;
      
    idtr++;
    
    os <<
      fMeasureRepeatPatternSegment;

    idtr--;
  }
      
  idtr--;
}

//______________________________________________________________________________
S_msrMeasureRepeatReplicas msrMeasureRepeatReplicas::create (
  int          inputLineNumber,
  S_msrVoice   voiceUplink)
{
  msrMeasureRepeatReplicas* o =
    new msrMeasureRepeatReplicas (
      inputLineNumber,
      voiceUplink);
  assert(o!=0);
  return o;
}

msrMeasureRepeatReplicas::msrMeasureRepeatReplicas (
  int        inputLineNumber,
  S_msrVoice voiceUplink)
    : msrElement (inputLineNumber)
{
  fMeasureRepeatReplicasVoiceUplink = voiceUplink;
}

msrMeasureRepeatReplicas::~msrMeasureRepeatReplicas()
{}

S_msrMeasureRepeatReplicas msrMeasureRepeatReplicas::createMeasureRepeatReplicasNewbornClone (
  S_msrVoice containingVoice)
{
  if (gGeneralOptions->fTraceRepeats)
    gLogIos <<
      "==> Creating a newborn clone of a measure repeat replicas" <<
      endl;
  
  // sanity check
  msrAssert(
    containingVoice != 0,
    "containingVoice is null");
    
  S_msrMeasureRepeatReplicas
    newbornClone =
      msrMeasureRepeatReplicas::create (
        fInputLineNumber,
        containingVoice);

  // segment

  // uplinks
  newbornClone->fMeasureRepeatReplicasVoiceUplink =
    containingVoice;
    
  return newbornClone;
}

S_msrMeasureRepeatReplicas msrMeasureRepeatReplicas::createMeasureRepeatReplicasDeepCopy (
  S_msrVoice containingVoice)
{
  if (gGeneralOptions->fTraceRepeats)
    gLogIos <<
      "==> Creating a deep copy of a measure repeat replicas" <<
      endl;
  
  // sanity check
  msrAssert(
    containingVoice != 0,
    "containingVoice is null");
    
  S_msrMeasureRepeatReplicas
    measureRepeatReplicasDeepCopy =
      msrMeasureRepeatReplicas::create (
        fInputLineNumber,
        containingVoice);

  // segment
  measureRepeatReplicasDeepCopy->fMeasureRepeatReplicasSegment =
    fMeasureRepeatReplicasSegment->
      createSegmentDeepCopy (
        containingVoice);

  // uplinks
  measureRepeatReplicasDeepCopy->fMeasureRepeatReplicasVoiceUplink =
    containingVoice;
    
  return measureRepeatReplicasDeepCopy;
}

void msrMeasureRepeatReplicas::setMeasureRepeatReplicasSegment (
    S_msrSegment measureRepeatReplicasSegment)
{
  if (gGeneralOptions->fTraceRepeats)
    gLogIos <<
      "Setting measure repeat replicas segment containing " <<
      singularOrPlural (
          measureRepeatReplicasMeasuresNumber (),
        "measure",
        "measures") <<
      endl;
      
  // sanity check
  msrAssert (
    measureRepeatReplicasSegment != 0,
    "measureRepeatReplicasSegment is null");

  fMeasureRepeatReplicasSegment =
    measureRepeatReplicasSegment;
}

int msrMeasureRepeatReplicas::measureRepeatReplicasMeasuresNumber () const
{
  int result;

  if (fMeasureRepeatReplicasSegment)
    result =
      fMeasureRepeatReplicasSegment->
      getSegmentMeasuresList ().size ();
  else
    result = 0;

  return result;
}

void msrMeasureRepeatReplicas::acceptIn (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors)
    gLogIos <<
      "% ==> msrMeasureRepeatReplicas::acceptIn()" <<
      endl;
      
  if (visitor<S_msrMeasureRepeatReplicas>*
    p =
      dynamic_cast<visitor<S_msrMeasureRepeatReplicas>*> (v)) {
        S_msrMeasureRepeatReplicas elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors)
          gLogIos <<
            "% ==> Launching msrMeasureRepeatReplicas::visitStart()" <<
             endl;
        p->visitStart (elem);
  }
}

void msrMeasureRepeatReplicas::acceptOut (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors)
    gLogIos <<
      "% ==> msrMeasureRepeatReplicas::acceptOut()" <<
      endl;

  if (visitor<S_msrMeasureRepeatReplicas>*
    p =
      dynamic_cast<visitor<S_msrMeasureRepeatReplicas>*> (v)) {
        S_msrMeasureRepeatReplicas elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors)
          gLogIos <<
            "% ==> Launching msrMeasureRepeatReplicas::visitEnd()" <<
            endl;
        p->visitEnd (elem);
  }
}

void msrMeasureRepeatReplicas::browseData (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors)
    gLogIos <<
      "% ==> msrMeasureRepeatReplicas::browseData()" <<
      endl;

  if (fMeasureRepeatReplicasSegment) {
  // browse the replicas segment
    msrBrowser<msrSegment> browser (v);
    browser.browse (*fMeasureRepeatReplicasSegment);
  }
}

ostream& operator<< (ostream& os, const S_msrMeasureRepeatReplicas& elt)
{
  elt->print (os);
  return os;
}

string msrMeasureRepeatReplicas::measureRepeatReplicasAsString () const
{
  stringstream s;

  s <<

    "MeasureRepeatReplicas" <<
    ", line " << fInputLineNumber <<
    " (" <<
    singularOrPlural (
      measureRepeatReplicasMeasuresNumber (),
      "replicas measure",
      "replicas measures") <<
    ")"; 

  return s.str ();
}

void msrMeasureRepeatReplicas::print (ostream& os)
{
  os <<
    endl <<
    measureRepeatReplicasAsString () <<
    endl <<
    endl;
  
  idtr++;
  
  // print the replicas segment
  os <<
    "Replicas segment:";

  if (! fMeasureRepeatReplicasSegment) {
    os <<
      " none" <<
      endl;
  }
  else {
    os <<
      endl;
      
    idtr++;
    
    os <<
      fMeasureRepeatReplicasSegment;

    idtr--;
  }

  idtr--;
}

//______________________________________________________________________________
S_msrMeasureRepeat msrMeasureRepeat::create (
  int        inputLineNumber,
  int        measureRepeatMeasuresNumber,
  int        measureRepeatSlashesNumber,
  S_msrVoice voiceUplink)
{
  msrMeasureRepeat* o =
    new msrMeasureRepeat (
      inputLineNumber,
      measureRepeatMeasuresNumber, measureRepeatSlashesNumber,
      voiceUplink);
  assert(o!=0);
  return o;
}

msrMeasureRepeat::msrMeasureRepeat (
  int        inputLineNumber,
  int        measureRepeatMeasuresNumber,
  int        measureRepeatSlashesNumber,
  S_msrVoice voiceUplink)
    : msrElement (inputLineNumber)
{
  fMeasureRepeatMeasuresNumber = measureRepeatMeasuresNumber;
  fMeasureRepeatSlashesNumber  = measureRepeatSlashesNumber;

  fMeasureRepeatVoiceUplink = voiceUplink;
}

msrMeasureRepeat::~msrMeasureRepeat()
{}

S_msrMeasureRepeat msrMeasureRepeat::createMeasureRepeatNewbornClone (
  S_msrVoice containingVoice)
{
  if (gGeneralOptions->fTraceRepeats)
    gLogIos <<
      "==> Creating a newborn clone of a measure repeat" <<
      endl;
  
  // sanity check
  msrAssert(
    containingVoice != 0,
    "containingVoice is null");
    
  S_msrMeasureRepeat
    newbornClone =
      msrMeasureRepeat::create (
        fInputLineNumber,
        fMeasureRepeatMeasuresNumber,
        fMeasureRepeatSlashesNumber,
        containingVoice);

  // numbers
  
  // repeat pattern

  // repeat replicas

  // uplinks

  return newbornClone;
}

S_msrMeasureRepeat msrMeasureRepeat::createMeasureRepeatDeepCopy (
  S_msrVoice containingVoice)
{
  if (gGeneralOptions->fTraceRepeats)
    gLogIos <<
      "==> Creating a deep copy of a measure repeat" <<
      endl;
  
  // sanity check
  msrAssert(
    containingVoice != 0,
    "containingVoice is null");
    
  S_msrMeasureRepeat
    measureRepeatDeepCopy =
      msrMeasureRepeat::create (
        fInputLineNumber,
        fMeasureRepeatMeasuresNumber,
        fMeasureRepeatSlashesNumber,
        containingVoice);

  // numbers

  // repeat pattern
  measureRepeatDeepCopy->fMeasureRepeatPattern =
    fMeasureRepeatPattern->
      createMeasureRepeatPatternDeepCopy (
        containingVoice);
        
  // repeat replicas

   measureRepeatDeepCopy->fMeasureRepeatReplicas =
    fMeasureRepeatReplicas->
      createMeasureRepeatReplicasDeepCopy (
        containingVoice);
        
  // uplinks
  measureRepeatDeepCopy->fMeasureRepeatVoiceUplink =
    containingVoice;
    
  return measureRepeatDeepCopy;
}

void msrMeasureRepeat::setMeasureRepeatPattern (
  S_msrMeasureRepeatPattern measureRepeatPattern)
{
  if (gGeneralOptions->fTraceRepeats)
    gLogIos <<
      "Setting measure repeat pattern containing " <<
      singularOrPlural (
        measureRepeatPattern->
          measureRepeatPatternMeasuresNumber (),
        "measure",
        "measures") <<
      endl;
      
  // sanity check
  msrAssert (
    measureRepeatPattern != 0,
    "measureRepeatPattern is null");

  fMeasureRepeatPattern = measureRepeatPattern;
}

void msrMeasureRepeat::setMeasureRepeatReplicas (
  S_msrMeasureRepeatReplicas measureRepeatReplicas)
{
  if (gGeneralOptions->fTraceRepeats)
    gLogIos <<
      "Setting measure repeat replicas containing " <<
      singularOrPlural (
        measureRepeatReplicas->
          measureRepeatReplicasMeasuresNumber (),
        "measure",
        "measures") <<
      endl;
      
  // sanity check
  msrAssert (
    measureRepeatReplicas != 0,
    "measureRepeatReplicas is null");

  fMeasureRepeatReplicas = measureRepeatReplicas;
}

int msrMeasureRepeat::measureRepeatReplicasNumber () const
{
  // compute replicas number
  return
    measureRepeatReplicasMeasuresNumber ()
      /
    measureRepeatPatternMeasuresNumber ();    
}

void msrMeasureRepeat::acceptIn (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors)
    gLogIos <<
      "% ==> msrMeasureRepeat::acceptIn()" <<
      endl;
      
  if (visitor<S_msrMeasureRepeat>*
    p =
      dynamic_cast<visitor<S_msrMeasureRepeat>*> (v)) {
        S_msrMeasureRepeat elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors)
          gLogIos <<
            "% ==> Launching msrMeasureRepeat::visitStart()" <<
             endl;
        p->visitStart (elem);
  }
}

void msrMeasureRepeat::acceptOut (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors)
    gLogIos <<
      "% ==> msrMeasureRepeat::acceptOut()" <<
      endl;

  if (visitor<S_msrMeasureRepeat>*
    p =
      dynamic_cast<visitor<S_msrMeasureRepeat>*> (v)) {
        S_msrMeasureRepeat elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors)
          gLogIos <<
            "% ==> Launching msrMeasureRepeat::visitEnd()" <<
            endl;
        p->visitEnd (elem);
  }
}

void msrMeasureRepeat::browseData (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors)
    gLogIos <<
      "% ==> msrMeasureRepeat::browseData()" <<
      endl;

  if (fMeasureRepeatPattern) {
  // browse the measdure repeat pattern
    msrBrowser<msrMeasureRepeatPattern> browser (v);
    browser.browse (*fMeasureRepeatPattern);
  }

  // fetch the score
  S_msrScore
    score =
      fMeasureRepeatVoiceUplink->
        fetchVoicePartUplink ()->
          getPartPartGroupUplink ()->
            getPartGroupScoreUplink ();
              
  bool inhibitMeasureRepeatReplicasBrowsing =
    score->getInhibitMeasureRepeatReplicasBrowsing ();

  if (inhibitMeasureRepeatReplicasBrowsing) {
    if (gMsrOptions->fTraceMsrVisitors || gGeneralOptions->fTraceRepeats)
      gLogIos <<
        "% ==> visiting measure repeat replicas is inhibited" <<
        endl;
  }

  if (fMeasureRepeatReplicas) {
    if (! inhibitMeasureRepeatReplicasBrowsing) {
      // browse the measure repeat replicas
      msrBrowser<msrMeasureRepeatReplicas> browser (v);
      browser.browse (*fMeasureRepeatReplicas);
    }
  }
}

ostream& operator<< (ostream& os, const S_msrMeasureRepeat& elt)
{
  elt->print (os);
  return os;
}

string msrMeasureRepeat::measureRepeatAsString () const
{
  stringstream s;

  s <<

    "MeasureRepeat" <<
    ", line " << fInputLineNumber <<
    " (" <<
    singularOrPlural (
      measureRepeatPatternMeasuresNumber (),
      "repeated measure",
      "repeated measures") <<
    ", " <<
    singularOrPlural (
      measureRepeatReplicasMeasuresNumber (),
      "replicas measure",
      "replicas measures") <<
    ", " <<
    measureRepeatReplicasNumber () << " replicas" <<
    ")"; 

  return s.str ();
}

void msrMeasureRepeat::print (ostream& os)
{
  os <<
    endl <<
    "MeasureRepeat" <<
    ", line " << fInputLineNumber <<
    " (" <<
    singularOrPlural (
      measureRepeatPatternMeasuresNumber (),
      "repeated measure",
      "repeated measures") <<
    ", " <<
    singularOrPlural (
      measureRepeatReplicasMeasuresNumber (),
      "replicas measure",
      "replicas measures") <<
    ", " <<
    measureRepeatReplicasNumber () << " replicas" <<
    ")" <<
    endl <<
    endl;
  
  idtr++;
  
  // print the measure repeat pattern
  if (! fMeasureRepeatPattern) {
    os <<
      "Measure repeat pattern: none" <<
      endl;
  }

  else {
    os <<
      fMeasureRepeatPattern;
  }

  os << endl;
  
  // print the measure repeat replicas
  if (! fMeasureRepeatReplicas) {
    os <<
      "Measure repeat replicas: none" <<
      endl;
  }

  else {
    os <<
      fMeasureRepeatReplicas;
  }
      
  idtr--;
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
  if (gGeneralOptions->fTraceRepeats)
    gLogIos <<
      "==> Creating a newborn clone of a multiple rest contents" <<
      endl;
  
  // sanity check
  msrAssert(
    containingVoice != 0,
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
  if (gGeneralOptions->fTraceRepeats)
    gLogIos <<
      "Setting multiple rest contents segment containing " <<
      singularOrPlural (
          multipleRestContentsMeasuresNumber (),
        "measure",
        "measures") <<
      endl;
      
  // sanity check
  msrAssert (
    multipleRestContentsSegment != 0,
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

void msrMultipleRestContents::acceptIn (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors)
    gLogIos <<
      "% ==> msrMultipleRestContents::acceptIn()" <<
      endl;
      
  if (visitor<S_msrMultipleRestContents>*
    p =
      dynamic_cast<visitor<S_msrMultipleRestContents>*> (v)) {
        S_msrMultipleRestContents elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors)
          gLogIos <<
            "% ==> Launching msrMultipleRestContents::visitStart()" <<
             endl;
        p->visitStart (elem);
  }
}

void msrMultipleRestContents::acceptOut (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors)
    gLogIos <<
      "% ==> msrMultipleRestContents::acceptOut()" <<
      endl;

  if (visitor<S_msrMultipleRestContents>*
    p =
      dynamic_cast<visitor<S_msrMultipleRestContents>*> (v)) {
        S_msrMultipleRestContents elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors)
          gLogIos <<
            "% ==> Launching msrMultipleRestContents::visitEnd()" <<
            endl;
        p->visitEnd (elem);
  }
}

void msrMultipleRestContents::browseData (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors)
    gLogIos <<
      "% ==> msrMultipleRestContents::browseData()" <<
      endl;

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

string msrMultipleRestContents::multipleRestContentsAsString () const
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
    multipleRestContentsAsString () <<
    "MultipleRestContents" <<
    endl <<
    endl;
  
  idtr++;
  
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
      
    idtr++;
    
    os <<
      fMultipleRestContentsSegment;

    idtr--;
  }
      
  idtr--;
}

//______________________________________________________________________________
S_msrMultipleRest msrMultipleRest::create (
  int        inputLineNumber,
  int        multipleRestMeasuresNumber,
  S_msrVoice voiceUplink)
{
  msrMultipleRest* o =
    new msrMultipleRest (
      inputLineNumber,
      multipleRestMeasuresNumber,
      voiceUplink);
  assert(o!=0);
  return o;
}

msrMultipleRest::msrMultipleRest (
  int        inputLineNumber,
  int        multipleRestMeasuresNumber,
  S_msrVoice voiceUplink)
    : msrElement (inputLineNumber)
{
  fMultipleRestMeasuresNumber = multipleRestMeasuresNumber;

  fMultipleRestVoiceUplink = voiceUplink;
}

msrMultipleRest::~msrMultipleRest()
{}

S_msrMultipleRest msrMultipleRest::createMultipleRestNewbornClone (
  S_msrVoice containingVoice)
{
  if (gGeneralOptions->fTraceRepeats)
    gLogIos <<
      "==> Creating a newborn clone of multiple rest " <<
      multipleRestAsString () <<
      endl;
  
  // sanity check
  msrAssert(
    containingVoice != 0,
    "containingVoice is null");
    
  S_msrMultipleRest
    newbornClone =
      msrMultipleRest::create (
        fInputLineNumber,
        fMultipleRestMeasuresNumber,
        containingVoice);

  return newbornClone;
}

void msrMultipleRest::setMultipleRestContents (
  S_msrMultipleRestContents multipleRestContents)
{
  if (gGeneralOptions->fTraceRepeats)
    gLogIos <<
      "Setting multiple rest segment containing " <<
      singularOrPlural (
        multipleRestContents->
          multipleRestContentsMeasuresNumber (),
        "measure",
        "measures") <<
      endl;
      
  // sanity check
  msrAssert (
    multipleRestContents != 0,
    "multipleRestContents is null");

  fMultipleRestContents = multipleRestContents;
}

void msrMultipleRest::acceptIn (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors)
    gLogIos <<
      "% ==> msrMultipleRest::acceptIn()" <<
      endl;
      
  if (visitor<S_msrMultipleRest>*
    p =
      dynamic_cast<visitor<S_msrMultipleRest>*> (v)) {
        S_msrMultipleRest elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors)
          gLogIos <<
            "% ==> Launching msrMultipleRest::visitStart()" <<
             endl;
        p->visitStart (elem);
  }
}

void msrMultipleRest::acceptOut (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors)
    gLogIos <<
      "% ==> msrMultipleRest::acceptOut()" <<
      endl;

  if (visitor<S_msrMultipleRest>*
    p =
      dynamic_cast<visitor<S_msrMultipleRest>*> (v)) {
        S_msrMultipleRest elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors)
          gLogIos <<
            "% ==> Launching msrMultipleRest::visitEnd()" <<
            endl;
        p->visitEnd (elem);
  }
}

void msrMultipleRest::browseData (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors)
    gLogIos <<
      "% ==> msrMultipleRest::browseData()" <<
      endl;

  // fetch the score
  S_msrScore
    score =
      fMultipleRestVoiceUplink->
        fetchVoicePartUplink ()->
          getPartPartGroupUplink ()->
            getPartGroupScoreUplink ();

  bool inhibitMultipleRestMeasuresBrowsing =
    score->getInhibitMultipleRestMeasuresBrowsing ();

  if (inhibitMultipleRestMeasuresBrowsing) {
    if (gMsrOptions->fTraceMsrVisitors || gGeneralOptions->fTraceRepeats)
      gLogIos <<
        "% ==> visiting multiple rest measures is inhibited" <<
        endl;
  }

  if (fMultipleRestContents) {
    if (! inhibitMultipleRestMeasuresBrowsing) {
      // browse the multiple rest contents
      msrBrowser<msrMultipleRestContents> browser (v);
      browser.browse (*fMultipleRestContents);
    }
  }
}

string msrMultipleRest::multipleRestAsString () const
{
  stringstream s;

  s <<
    "Multiple rest" <<
    ", line " << fInputLineNumber <<
    ", " <<
    singularOrPlural (
      fMultipleRestMeasuresNumber,
        "rest measure",
        "rest measures");
    
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
    multipleRestAsString () <<
    endl;
  
  idtr++;
  
  // print the rests contents
  if (! fMultipleRestContents) {
    os <<
      "Multiple rests contents: none" <<
      endl;
  }

  else {
    os <<
      fMultipleRestContents;
  }
      
  idtr--;
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
    repeatCodaSegment != 0,
    "repeatCodaSegment is null");
    
  fRepeatCodaSegment = repeatCodaSegment;
  
  // sanity check
  msrAssert (
    repeatUplink != 0,
    "repeatUplink is null");
    
  fRepeatCodaRepeatUplink = repeatUplink;
}

msrRepeatCoda::~msrRepeatCoda()
{}

S_msrRepeatCoda msrRepeatCoda::createRepeatCodaNewbornClone (
  S_msrRepeat containingRepeat)
{
  if (gGeneralOptions->fTraceRepeats)
    gLogIos <<
      "==> Creating a newborn clone of a " <<
      repeatCodaAsString () <<
      endl;
  
  // sanity check
  msrAssert(
    containingRepeat != 0,
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
  if (gGeneralOptions->fTraceRepeats)
    gLogIos <<
      "==> Creating a newborn clone of a " <<
      repeatCodaAsString () <<
      endl;
  
  // sanity check
  msrAssert(
    containingRepeat != 0,
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

void msrRepeatCoda::acceptIn (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors)
    gLogIos <<
      "% ==> msrRepeatCoda::acceptIn()" <<
      endl;
      
  if (visitor<S_msrRepeatCoda>*
    p =
      dynamic_cast<visitor<S_msrRepeatCoda>*> (v)) {
        S_msrRepeatCoda elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors)
          gLogIos <<
            "% ==> Launching msrRepeatCoda::visitStart()" <<
             endl;
        p->visitStart (elem);
  }
}

void msrRepeatCoda::acceptOut (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors)
    gLogIos <<
      "% ==> msrRepeatCoda::acceptOut()" <<
      endl;

  if (visitor<S_msrRepeatCoda>*
    p =
      dynamic_cast<visitor<S_msrRepeatCoda>*> (v)) {
        S_msrRepeatCoda elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors)
          gLogIos <<
            "% ==> Launching msrRepeatCoda::visitEnd()" <<
            endl;
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

string msrRepeatCoda::repeatCodaAsString () const
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
    repeatCodaAsString () <<
    endl;
 
  idtr++;

  os <<
    fRepeatCodaSegment;

  idtr--;
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
  if (gGeneralOptions->fTraceRepeats)
    gLogIos <<
      "==> Creating a newborn clone of a " <<
      harpPedalsTuningAsString () <<
      endl;
  
  S_msrHarpPedalsTuning
    newbornClone = 0; // JMI

  return newbornClone;
}

S_msrHarpPedalsTuning msrHarpPedalsTuning::createHarpPedalsTuningDeepCopy ()
{
  if (gGeneralOptions->fTraceRepeats)
    gLogIos <<
      "==> Creating a newborn clone of a " <<
      harpPedalsTuningAsString () <<
      endl;
  
  S_msrHarpPedalsTuning
    harpPedalsTuningDeepCopy = 0; // JMI

  return harpPedalsTuningDeepCopy;
}

void msrHarpPedalsTuning::addPedalTuning (
  int              intputLineNumber,
  msrDiatonicPitch diatonicPitch,
  msrAlteration    alteration)
{
  // is diatonicPitch in the part renaming map?
  map<msrDiatonicPitch, msrAlteration>::const_iterator
    it =
      fHarpPedalsAlterationsMap.find (diatonicPitch);
        
  if (it != fHarpPedalsAlterationsMap.end ()) {
    stringstream s;

    s <<
      "pedal tuning '" <<
      msrDiatonicPitchAsString (
        diatonicPitch) <<
      msrAlterationAsString (
        alteration);
      "' has already been specified";
      
    msrMusicXMLError (
      intputLineNumber,
      s.str ());
  }

  fHarpPedalsAlterationsMap [diatonicPitch] = alteration;
}

void msrHarpPedalsTuning::acceptIn (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors)
    gLogIos <<
      "% ==> msrHarpPedalsTuning::acceptIn()" <<
      endl;
      
  if (visitor<S_msrHarpPedalsTuning>*
    p =
      dynamic_cast<visitor<S_msrHarpPedalsTuning>*> (v)) {
        S_msrHarpPedalsTuning elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors)
          gLogIos <<
            "% ==> Launching msrHarpPedalsTuning::visitStart()" <<
             endl;
        p->visitStart (elem);
  }
}

void msrHarpPedalsTuning::acceptOut (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors)
    gLogIos <<
      "% ==> msrHarpPedalsTuning::acceptOut()" <<
      endl;

  if (visitor<S_msrHarpPedalsTuning>*
    p =
      dynamic_cast<visitor<S_msrHarpPedalsTuning>*> (v)) {
        S_msrHarpPedalsTuning elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors)
          gLogIos <<
            "% ==> Launching msrHarpPedalsTuning::visitEnd()" <<
            endl;
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

string msrHarpPedalsTuning::harpPedalsTuningAsString () const
{
  stringstream s;

  s <<
    "HarpPedalsTuning" <<
    ", line " << fInputLineNumber <<
    ", ";
    
  if (fHarpPedalsAlterationsMap.size ()) {
    idtr++;

    map<msrDiatonicPitch, msrAlteration>::const_iterator
      iBegin = fHarpPedalsAlterationsMap.begin(),
      iEnd   = fHarpPedalsAlterationsMap.end(),
      i      = iBegin;
      
    for ( ; ; ) {
      // print the pedal and its alteration
      s <<
        msrDiatonicPitchAsString (
          (*i).first) <<
        " " <<
        msrAlterationAsString (
          (*i).second);
      if (++i == iEnd) break;
      s << ", ";
    } // for

    idtr--;
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
    
  if (fHarpPedalsAlterationsMap.size ()) {
    idtr++;

    os <<
      endl <<
      idtr;
      
    map<msrDiatonicPitch, msrAlteration>::const_iterator
      iBegin = fHarpPedalsAlterationsMap.begin(),
      iEnd   = fHarpPedalsAlterationsMap.end(),
      i      = iBegin;
      
    for ( ; ; ) {
      // print the pedal and its alteration
      os <<
        msrDiatonicPitchAsString (
          (*i).first) <<
        " " <<
        msrAlterationAsString (
          (*i).second);
      if (++i == iEnd) break;
      os << ", ";
    } // for

    idtr--;
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
int msrVoice::gVoicesCounter = 0;

S_msrVoice msrVoice::create (
  int          inputLineNumber,
  msrVoiceKind voiceKind,
  int          voicePartRelativeID,
  msrVoiceCreateInitialLastSegment
               voiceCreateInitialLastSegment,
  S_msrStaff   voiceStaffUplink)
{
  msrVoice* o =
    new msrVoice (
      inputLineNumber,
      voiceKind,
      voicePartRelativeID,
      voiceCreateInitialLastSegment,
      voiceStaffUplink);
  assert(o!=0);

  return o;
}

msrVoice::msrVoice (
  int          inputLineNumber,
  msrVoiceKind voiceKind,
  int          voicePartRelativeID,
  msrVoiceCreateInitialLastSegment
               voiceCreateInitialLastSegment,
  S_msrStaff   voiceStaffUplink)
    : msrElement (inputLineNumber)
{
  fVoiceAbsoluteNumber = ++gVoicesCounter;
  
  // sanity check
  msrAssert(
    voiceStaffUplink != 0,
    "voiceStaffUplink is null");

  // set voice staff uplink
  fVoiceStaffUplink = voiceStaffUplink;

  // set voice kind
  fVoiceKind = voiceKind;

  // set voice part-relative ID
  fVoicePartRelativeID = voicePartRelativeID;
  
  // do other initializations
  initializeVoice (
    voiceCreateInitialLastSegment);
}

msrVoice::~msrVoice()
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
  if (gGeneralOptions->fTraceVoices) {
    gLogIos <<
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
      
    case msrVoice::kMasterVoice:
    case msrVoice::kHarmonyVoice:
    case msrVoice::kFiguredBassVoice:
    case msrVoice::kSilentVoice:
      {
        stringstream s;

        s <<
          "A '" <<
          getVoiceName () <<
          "' voice cannot get its name from its number";

        msrInternalError (
          inputLineNumber,
          s.str ());
      }
      break;
  } // switch
  
  /* JMI
    CAUTION:
      the following makes the generated LilyPond code not compilable,
      but may be useful to disambiguate the voice and voice clones names

  fVoiceName += // JMI
    "[" + to_string (fVoiceAbsoluteNumber) + "]";
  */
}

void msrVoice::initializeVoice (
  msrVoiceCreateInitialLastSegment
    voiceCreateInitialLastSegment)
{
  fVoiceStaffRelativeNumber = fVoicePartRelativeID;
    // may be changed afterwards JMI ???

  // compute voice number
  int voiceNumber =
    gMsrOptions->
      fCreateVoicesStaffRelativeNumbers // JMI use it
        ? fVoiceStaffRelativeNumber
        : fVoicePartRelativeID;
  
  // set voice name
  switch (fVoiceKind) {
    case msrVoice::kMasterVoice:
      fVoiceName =
        fVoiceStaffUplink->getStaffName() +
        "_MASTER_Voice";
      break;
      
    case msrVoice::kRegularVoice:
      setVoiceNameFromNumber (
        fInputLineNumber,
        voiceNumber);
      break;
      
    case msrVoice::kHarmonyVoice:
      fVoiceName =
        fVoiceStaffUplink->getStaffName() +
        "_HARMONY_Voice";
      break;
      
    case msrVoice::kFiguredBassVoice:
      fVoiceName =
        fVoiceStaffUplink->getStaffName() +
        "_FIGURED_BASS_Voice";
      break;
      
    case msrVoice::kSilentVoice:
      fVoiceName =
        fVoiceStaffUplink->getStaffName() +
        "_SILENT_Voice";
      break;
  } // switch
  
  if (gGeneralOptions->fTraceVoices) {
    gLogIos <<
      "==> Initializing voice \"" << fVoiceName <<
      "\" in staff \"" <<
      fVoiceStaffUplink->getStaffName () <<
      "\"" <<
      endl;
  }

  // check voice part-relative ID
  switch (fVoiceKind) {
    case msrVoice::kMasterVoice:
      if (fVoicePartRelativeID != K_PART_MASTER_VOICE_NUMBER) {
        stringstream s;
    
        s <<
          "master voice number " << fVoicePartRelativeID <<
          " is not equal to " << K_PART_MASTER_VOICE_NUMBER;
          
        msrInternalError (
          fInputLineNumber, s.str ());
      }
      break;
      
    case msrVoice::kRegularVoice:
      // the voice part-relative ID should not be negative
      // (K_SILENT_VOICE_NUMBER is used for the staves silent voices)
      if (fVoicePartRelativeID < 0) {
        stringstream s;
    
        s <<
          "regular voice number " << fVoicePartRelativeID <<
          " is not in the 0..4 range";
          
        msrMusicXMLError (
          fInputLineNumber, s.str ());
      }
      break;
      
    case msrVoice::kHarmonyVoice:
      if (fVoicePartRelativeID != K_PART_HARMONY_VOICE_NUMBER) {
        stringstream s;
    
        s <<
          "harmony voice number " << fVoicePartRelativeID <<
          " is not equal to " << K_PART_HARMONY_VOICE_NUMBER;
          
        msrInternalError (
          fInputLineNumber, s.str ());
      }
      break;
      
    case msrVoice::kFiguredBassVoice:
      if (fVoicePartRelativeID != K_PART_FIGURED_BASS_VOICE_NUMBER) {
        stringstream s;
    
        s <<
          "harmony voice number " << fVoicePartRelativeID <<
          " is not equal to " << K_PART_FIGURED_BASS_VOICE_NUMBER;
          
        msrInternalError (
          fInputLineNumber, s.str ());
      }
      break;
      
    case msrVoice::kSilentVoice:
      if (fVoicePartRelativeID != K_SILENT_VOICE_NUMBER) {
        stringstream s;
    
        s <<
          "silent voice number " << fVoicePartRelativeID <<
          " is not equal to " << K_SILENT_VOICE_NUMBER;
          
        msrInternalError (
          fInputLineNumber, s.str ());
      }
      break;
  } // switch

  // should the initial last segment be created?
  switch (voiceCreateInitialLastSegment) {
    case msrVoice::kCreateInitialLastSegmentYes:
      if (gGeneralOptions->fTraceVoices) {
        gLogIos <<
          "==> Creating and initial voice last segment for voice \"" <<
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

  // get the initial staff details from the staff if any
  S_msrStaffDetails
    staffStaffDetails =
      fVoiceStaffUplink->
        getCurrentStaffStaffDetails ();

  if (staffStaffDetails) {
    // append it to the voice
    appendStaffDetailsToVoice (staffStaffDetails);
  }
    
  // add the mute stanza for this voice,
  // to collect skips along the way that are used as a 'prelude'
  // by actual stanza that start at later points
  fVoiceMuteStanza =
    msrStanza::create (
      fInputLineNumber,
      K_MUTE_STANZA_NUMBER,    // this stanza number is unused anyway
      msrStanza::kMuteStanza,
      this);
}

void msrVoice::changeVoiceIdentity ( // after a deep copy
  int voicePartRelativeID)
{
  if (gGeneralOptions->fTraceVoices) {
    gLogIos <<
      "Changing identity of voice \"" <<
      getVoiceName () <<
      "\"" <<
      ", part-relative voice ID: " << voicePartRelativeID <<
      endl;
  }

  // make it a regular voice
  setVoiceKind (
    msrVoice::kRegularVoice);

  // set its voice part-relative ID
  setVoicePartRelativeID (
    voicePartRelativeID);

  // set its name
  setVoiceNameFromNumber (
    fInputLineNumber,
    voicePartRelativeID);
}
    
S_msrVoice msrVoice::createVoiceNewbornClone (
  S_msrStaff staffClone)
{
  if (gGeneralOptions->fTraceVoices) {
    gLogIos <<
      "==> Creating a newborn clone of voice \"" <<
      getVoiceName () <<
      "\"" <<
      endl;
  }

  // sanity check
  msrAssert(
    staffClone != 0,
    "staffClone is null");
    
  S_msrVoice
    newbornClone =
      msrVoice::create (
        fInputLineNumber,
        fVoiceKind,
        fVoicePartRelativeID,
        msrVoice::kCreateInitialLastSegmentNo,
          // will be created upon a later segment visit
        staffClone);

  // voice numbers
  newbornClone->fVoiceAbsoluteNumber =
    ++gVoicesCounter;

  newbornClone->fVoiceStaffRelativeNumber =
    fVoiceStaffRelativeNumber;

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

  // initial repeats and segments

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
  int          voicePartRelativeID,
  S_msrStaff   containingStaff)
{
  if (gGeneralOptions->fTraceVoices) {
    gLogIos <<
      "==> Creating a deep copy of voice \"" <<
      getVoiceName () <<
      "\"" <<
      ", to be placed in containing staff \"" <<
      containingStaff->getStaffName () << "\"" <<
      ", line " << inputLineNumber <<
      endl;
  }

  // sanity check
  msrAssert(
    containingStaff != 0,
    "containingStaff is null");
    
  if (gGeneralOptions->fTraceVoicesDetails) {
    gLogIos <<
      "****" <<
      " BEFORE voiceDeepCopy = " <<
      endl;
  
    print (gLogIos);
    
    gLogIos <<
      endl <<
      "****" <<
      endl;
  }
    
  S_msrVoice
    voiceDeepCopy =
      msrVoice::create (
        fInputLineNumber,
        voiceKind,
        voicePartRelativeID,
        msrVoice::kCreateInitialLastSegmentNo,
          // the voice initial last segment
          // will be created by deep cloning below
        containingStaff);

  // voice numbers
  voiceDeepCopy->fVoiceAbsoluteNumber =
    ++gVoicesCounter;;

  // voice name
  if (false) // JMI
  voiceDeepCopy->fVoiceName =
    fVoiceName;

/* JMI
  // set voice name // JMI
  voiceDeepCopy->
    setVoiceNameFromNumber (
      fInputLineNumber,
      voicePartRelativeID);
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

  // initial repeats and segments
  int numberOfInitialRepeatsAndSegments =
    fVoiceInitialRepeatsAndSegments.size ();
    
  if (numberOfInitialRepeatsAndSegments) {
     if (gGeneralOptions->fTraceVoices) {
      gLogIos <<
        "There are " <<
        numberOfInitialRepeatsAndSegments <<
        " initial repeats and segments in voice \"" <<
        getVoiceName () <<
        "\"" <<
        endl;
    }

   list<S_msrElement>::const_iterator
      iBegin = fVoiceInitialRepeatsAndSegments.begin(),
      iEnd   = fVoiceInitialRepeatsAndSegments.end(),
      i      = iBegin;
      
    for ( ; ; ) {
      // handle the deep copy
      S_msrElement
        elementDeepCopy;
        
      if (
        S_msrRepeat repeat = dynamic_cast<msrRepeat*>(&(**i))
        ) {    
        // create the repeat deep copy
        elementDeepCopy =
          repeat->createRepeatDeepCopy (
            voiceDeepCopy);
      }
    
      else if (
        // create the segment deep copy
        S_msrSegment segment = dynamic_cast<msrSegment*>(&(**i))
        ) {
        elementDeepCopy =
          segment->createSegmentDeepCopy (
            voiceDeepCopy);
      }
      
      else {
        msrInternalError (
          fInputLineNumber,
          "voice  \"" <<
          getVoiceName () <<
          "\" initial repeats and segments element should be a repeat or a segment");
      }
  
      // append the element deep copy to the voice deep copy
      voiceDeepCopy->
        fVoiceInitialRepeatsAndSegments.push_back (
          elementDeepCopy);
        
      if (++i == iEnd) break;
    } // for
  }

  else {    
    if (gGeneralOptions->fTraceVoices) {
      gLogIos <<
        "There are no initial repeats and segments in voice \"" <<
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
  }
  else {    
    if (gGeneralOptions->fTraceVoices) {
      gLogIos <<
        "There is no last segment in voice \"" <<
        getVoiceName () <<
        "\" to be deep copied" <<
        endl;
    }
  }

  // first segment
  //fVoiceFirstSegment ??? JMI
  
  // current repeat
  // fVoiceCurrentRepeat ??? JMI
  
  // multple rests
  // fVoicePendingMultipleRest ??? JMI
  voiceDeepCopy->fVoiceContainsMultipleRests =
    fVoiceContainsMultipleRests;

  // stanzas
  voiceDeepCopy->fVoiceMuteStanza =
    fVoiceMuteStanza->
      createStanzaDeepCopy (
        voiceDeepCopy);

  for (
    map<string, S_msrStanza>::const_iterator i = fVoiceStanzasMap.begin();
    i != fVoiceStanzasMap.end();
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

  if (gGeneralOptions->fTraceVoicesDetails) {
    gLogIos <<
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
{ // JMI
}

/* JMI
string msrVoice::getVoiceName () const
{
  return fVoiceName;

  int voiceNumber =
    gMsrOptions-> fCreateVoicesStaffRelativeNumbers // JMI use
      ? fVoiceStaffRelativeNumber
      : fVoicePartRelativeID;

  string suffix =
    fVoiceStaffRelativeNumber == 0
      ? "SILENT"
      : int2EnglishWord (voiceNumber);
      
  return
    fVoiceStaffUplink->getStaffName() +
    "_Voice_" +
    suffix;
}
*/

void msrVoice::createMeasureAndAppendItToVoice (
  int    inputLineNumber,
  string measureNumber)
{
  fVoiceCurrentMeasureNumber = measureNumber;

  if (! fVoiceLastSegment) {
    // create the voice last segment if not yet done // JMI
    createNewLastSegmentForVoice (
      inputLineNumber);
  }
  
  // append new measure with given number
  fVoiceLastSegment->
    createMeasureAndAppendItToSegment (
      inputLineNumber,
      measureNumber);
}

void msrVoice::createNewLastSegmentForVoice (
  int inputLineNumber)
{
  // create the segment
  if (gGeneralOptions->fTraceSegments || gGeneralOptions->fTraceVoices) {
    gLogIos <<
      "==> Creating a new segment for voice \"" <<
      getVoiceName () << "\"" <<
      ", line " << inputLineNumber <<
      endl;
  }

  fVoiceLastSegment =
    msrSegment::create (
      inputLineNumber,
      this);
}

void msrVoice::createNewLastSegmentFromFirstMeasureForVoice (
  int          inputLineNumber,
  S_msrMeasure firstMeasure)
{
  // create the segment
  if (gGeneralOptions->fTraceVoices) {
    gLogIos <<
      "==> Creating a new segment with first measure '" <<
      firstMeasure->getMeasureNumber () <<
      "'for voice \"" <<
      getVoiceName () << "\"" <<
      ", line " << inputLineNumber <<
      endl;
  }

  // create the last segment
  fVoiceLastSegment =
    msrSegment::create (
      inputLineNumber,
      this);

  // append firstMeasure to it
  fVoiceLastSegment->
    appendMeasureToSegment (firstMeasure);
}

void msrVoice::createNewLastSegmentAndANewMeasureForVoice (
  int    inputLineNumber)
{
/*
  This method is used for repeats
  It may lead to several measures having the same number,
  in case a bar line cuts them apart
  If this measure remains empty because the end of measure
  follows the barline, it will be removed upon finalizeMeasure()
*/

  // create the segment
  if (gGeneralOptions->fTraceVoices) {
    gLogIos <<
      "==> Creating a new segment containing a new measure '" <<
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

  // create the new measure with number newMeasureMeasureNumber
  S_msrMeasure
    newMeasure =
      msrMeasure::create (
        inputLineNumber,
        fVoiceCurrentMeasureNumber,
        fVoiceLastSegment);
  
  // append new measure to new voice last segment
  fVoiceLastSegment->
    appendMeasureToSegment (newMeasure);
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
      inputLineNumber,
      s.str ());
  }

  // create the stanza
  S_msrStanza
    stanza =
      msrStanza::create (
        inputLineNumber,
        stanzaNumber,
        msrStanza::kRegularStanza,
        this);

  // add the stanza to this voice
  addStanzaToVoiceWithCatchUp (stanza);

  // return it
  return stanza;
}

void msrVoice::addStanzaToVoiceWithoutCatchUp (S_msrStanza stanza)
{
  // get stanza number
  string stanzaNumber =
    stanza->getStanzaNumber ();
    
  // register stanza in this voice
  if (gGeneralOptions->fTraceLyrics) {
    gLogIos <<
      "Adding stanza " << stanza->getStanzaName () <<
      " (" << stanzaNumber <<
      ") to voice \"" << getVoiceName () << "\"" <<
      endl;
  }

  // add the stanza to this voice
  fVoiceStanzasMap [stanzaNumber] = stanza;
}

void msrVoice::catchUpWithVoiceMuteStanza (S_msrStanza stanza)
{
  vector<S_msrSyllable>
    muteSyllables =
      fVoiceMuteStanza->
        getSyllables ();

  if (muteSyllables.size()) {
    if (gGeneralOptions->fTraceLyrics)
      gLogIos <<
        "Copying current contents of voice mute stanza to " <<
        stanza->getStanzaName () <<
        endl;
        
    for (
      vector<S_msrSyllable>::const_iterator i =
        muteSyllables.begin();
      i != muteSyllables.end();
      i++) {
      // add syllable to stanza
      stanza->appendSyllableToStanza ((*i));
    } // for
  }
}

void msrVoice::addStanzaToVoiceWithCatchUp (S_msrStanza stanza)
{
  // get stanza number
  string stanzaNumber =
    stanza->getStanzaNumber ();
    
  // register stanza in this voice
  if (gGeneralOptions->fTraceLyrics) {
    gLogIos <<
      "Adding stanza " << stanza->getStanzaName () <<
      " (" << stanzaNumber <<
      ") to voice \"" << getVoiceName () << "\"" <<
      endl;
  }

  fVoiceStanzasMap [stanzaNumber] = stanza;

  // catch up with fVoiceMuteStanza
  // in case the stanza does not start upon the first voice note
  catchUpWithVoiceMuteStanza (stanza);
}

S_msrStanza msrVoice::createStanzaInVoiceIfNotYetDone (
  int    inputLineNumber,
  string stanzaNumber)
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
    if (gGeneralOptions->fTraceVoices || gGeneralOptions->fTraceLyrics)
      gLogIos <<
        "==> Creating stanza " << stanzaNumber <<
        " in voice \"" << getVoiceName () << "\"" <<
        ", line " << inputLineNumber <<
        ", fVoiceStanzasMap.size() = " << fVoiceStanzasMap.size () <<
        endl;

    stanza =
      addStanzaToVoiceByItsNumber (
        inputLineNumber, stanzaNumber);
  }

  return stanza;
}

void msrVoice::appendClefToVoice (S_msrClef clef)
{
  if (gGeneralOptions->fTraceClefs || gGeneralOptions->fTraceVoices) {
    gLogIos <<
      "Appending clef '" << clef->clefAsString () <<
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
  if (gGeneralOptions->fTraceKeys || gGeneralOptions->fTraceVoices) {
    gLogIos <<
      "Appending key '" << key->keyAsString () <<
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
  if (gGeneralOptions->fTraceTimes || gGeneralOptions->fTraceVoices) {
    gLogIos <<
      "Appending time '" << time->timeAsString () <<
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
  if (gGeneralOptions->fTraceTimes || gGeneralOptions->fTraceVoices) {
    gLogIos <<
      "Appending time '" << time->timeAsString () <<
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
  if (gGeneralOptions->fTraceHarmonies || gGeneralOptions->fTraceVoices)
    gLogIos <<
      "Appending harmony '" << harmony->harmonyAsString () <<
      "' to voice \"" << getVoiceName () << "\"" <<
      endl;
      
  switch (fVoiceKind) {
    case msrVoice::kHarmonyVoice:
      // create the voice last segment and first measure if needed
      appendAFirstMeasureToVoiceIfNotYetDone (
        harmony->getInputLineNumber ());

      fVoiceLastSegment->
        appendHarmonyToSegment (harmony);
    
      // register harmony
      fVoiceActualHarmoniesCounter++;
      fMusicHasBeenInsertedInVoice = true;
      break;
      
    case msrVoice::kMasterVoice:
    case msrVoice::kRegularVoice:
    case msrVoice::kFiguredBassVoice:
    case msrVoice::kSilentVoice:
      {
        stringstream s;

        s <<
          "cannot append a harmony to " <<
          voiceKindAsString () <<
          " voice \"" <<
          getVoiceName () <<
          "\"";

        msrInternalError (
          harmony->getInputLineNumber (),
          s.str ());
      }
      break;
  } // switch
}

void msrVoice::appendHarmonyToVoiceClone (S_msrHarmony harmony)
{
  if (gGeneralOptions->fTraceHarmonies || gGeneralOptions->fTraceVoices)
    gLogIos <<
      "Appending harmony '" << harmony->harmonyAsString () <<
      "' to voice clone \"" << getVoiceName () << "\"" <<
      endl;
      
  switch (fVoiceKind) {
    case msrVoice::kHarmonyVoice:
   // /* JMI DON'T
      // create the voice last segment and first measure if needed
      appendAFirstMeasureToVoiceIfNotYetDone (
        harmony->getInputLineNumber ());
//*/

      fVoiceLastSegment->
        appendHarmonyToSegmentClone (harmony);
    
      // register harmony
      fVoiceActualHarmoniesCounter++;
      fMusicHasBeenInsertedInVoice = true;
      break;
      
    case msrVoice::kMasterVoice:
    case msrVoice::kRegularVoice:
    case msrVoice::kFiguredBassVoice:
    case msrVoice::kSilentVoice:
      {
        stringstream s;

        s <<
          "cannot append a harmony to " <<
          voiceKindAsString () <<
          " voice clone \"" <<
          getVoiceName () <<
          "\"";

        msrInternalError (
          harmony->getInputLineNumber (),
          s.str ());
      }
      break;
  } // switch
}

void msrVoice::appendFiguredBassToVoice (
  S_msrFiguredBass figuredBass)
{
  if (gGeneralOptions->fTraceFiguredBass || gGeneralOptions->fTraceVoices)
    gLogIos <<
      "Appending figured bass '" << figuredBass->figuredBassAsString () <<
      "' to voice \"" << getVoiceName () << "\"" <<
      endl;
      
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

    case msrVoice::kMasterVoice:
    case msrVoice::kRegularVoice:
    case msrVoice::kHarmonyVoice:
    case msrVoice::kSilentVoice:
      {
        stringstream s;

        s <<
          "cannot append a figured bass to " <<
          voiceKindAsString () <<
          " voice \"" <<
          getVoiceName () <<
          "\"";

        msrInternalError (
          figuredBass->getInputLineNumber (),
          s.str ());
      }
      break;
  } // switch
}

void msrVoice::appendFiguredBassToVoiceClone (S_msrFiguredBass figuredBass)
{
  if (gGeneralOptions->fTraceFiguredBass || gGeneralOptions->fTraceVoices)
    gLogIos <<
      "Appending figured bass '" << figuredBass->figuredBassAsString () <<
      "' to voice clone \"" << getVoiceName () << "\"" <<
      endl;
      
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
      
    case msrVoice::kMasterVoice:
    case msrVoice::kRegularVoice:
    case msrVoice::kHarmonyVoice:
    case msrVoice::kSilentVoice:
      {
        stringstream s;

        s <<
          "cannot append a figured bass to " <<
          voiceKindAsString () <<
          " voice clone \"" <<
          getVoiceName () <<
          "\"";

        msrInternalError (
          figuredBass->getInputLineNumber (),
          s.str ());
      }
      break;
  } // switch
}

void msrVoice::fillVoiceWithSkipsUpToMeasure (
  int    inputLineNumber,
  string measureNumber)
{
  if (gGeneralOptions->fTraceVoices || gGeneralOptions->fTraceDivisions)
    gLogIos <<
      "Filling voice \"" <<
      getVoiceName () <<
      "\" with skips up to measure '" <<
      measureNumber <<
      "', line " << inputLineNumber <<
      endl;

  // get staff silent voice
  S_msrVoice
    staffSilentVoice =
      fVoiceStaffUplink->
        getStaffSilentVoice ();

// JMI
}

void msrVoice::bringVoiceToMeasureLength (
  int      inputLineNumber,
  rational measureLength)
{
  if (gGeneralOptions->fTraceVoices || gGeneralOptions->fTraceMeasures) {
    gLogIos <<
      "Bringing measure length for voice \"" <<
      getVoiceName () <<
      "\" to " << measureLength <<
      ", line " << inputLineNumber <<
      endl;
  }

  fVoiceLastSegment->
    bringSegmentToMeasureLength (
      inputLineNumber, measureLength);
}

void msrVoice::appendTransposeToVoice (S_msrTranspose transpose)
{
  if (gGeneralOptions->fTraceTranspositions || gGeneralOptions->fTraceVoices) {
    gLogIos <<
      "Appending transpose '" <<
      transpose->transposeAsString () <<
      "' to voice \"" << getVoiceName () << "\"" <<
      endl;
  }

  // create the voice last segment and first measure if needed
  appendAFirstMeasureToVoiceIfNotYetDone (
    transpose->getInputLineNumber ());

  fVoiceLastSegment->
    appendTransposeToSegment (transpose);
}

void msrVoice::appendStaffDetailsToVoice (
  S_msrStaffDetails staffDetails)
{
  if (gGeneralOptions->fTraceStaves || gGeneralOptions->fTraceVoices)
    gLogIos <<
      "Appending staff details '" <<
      staffDetails->staffDetailsAsShortString () <<
      "' to voice \"" << getVoiceName () << "\"" <<
      endl;

/* JMI
  // register staff details in voice
  fCurrentVoicefStaffDetails = staffDetails;
  */
  
 // create the voice last segment and first measure if needed
  appendAFirstMeasureToVoiceIfNotYetDone (
    staffDetails->getInputLineNumber ());

  fVoiceLastSegment->
    appendStaffDetailsToSegment (staffDetails);
}

/* JMI
void msrVoice::appendWordsToVoice (S_msrWords words)
{
  if (gGeneralOptions->fTraceWords)
    gLogIos <<
      "Appending words '" << words->wordsAsString () <<
      "' to voice " << getVoiceName () << endl;

  S_msrElement w = words;
  fVoiceLastSegment->
    appendWordsToSegment (w);
}
*/

void msrVoice::appendTempoToVoice (S_msrTempo tempo)
{
  if (gMsrOptions->fTraceMsr)
    gLogIos <<
      "Appending tempo '" << tempo->tempoAsString () <<
      "' to voice \"" << getVoiceName () << "\"" <<
      endl;

  // create the voice last segment and first measure if needed
  appendAFirstMeasureToVoiceIfNotYetDone (
    tempo->getInputLineNumber ());

  fVoiceLastSegment->
    appendTempoToSegment (tempo);
}

void msrVoice::appendOctaveShiftToVoice (S_msrOctaveShift octaveShift)
{
  if (gMsrOptions->fTraceMsr)
    gLogIos <<
      "Appending octave shift '" <<
      octaveShift->octaveShiftKindAsString () <<
      "', size: " << octaveShift->getOctaveShiftSize () <<
      "' to voice \"" << getVoiceName () << "\"" <<
      endl;

  // create the voice last segment and first measure if needed
  appendAFirstMeasureToVoiceIfNotYetDone (
    octaveShift->getInputLineNumber ());

  fVoiceLastSegment->
    appendOctaveShiftToSegment (octaveShift);
}

void msrVoice::appendAccordionRegistrationToVoice (
  S_msrAccordionRegistration
    accordionRegistration)
{
  if (gGeneralOptions->fTraceGeneral || gGeneralOptions->fTraceVoices)
    gLogIos <<
      "Appending accordion registration '" <<
      accordionRegistration->accordionRegistrationAsString () <<
      "' to voice \"" << getVoiceName () << "\"" <<
      endl;

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
  if (gGeneralOptions->fTraceGeneral || gGeneralOptions->fTraceVoices)
    gLogIos <<
      "Appending harp pedals tuning '" <<
      harpPedalsTuning->harpPedalsTuningAsString () <<
      "' to voice \"" << getVoiceName () << "\"" <<
      endl;

  // create the voice last segment and first measure if needed
  appendAFirstMeasureToVoiceIfNotYetDone (
    harpPedalsTuning->getInputLineNumber ());

  fVoiceLastSegment->
    appendHarpPedalsTuningToSegment (
      harpPedalsTuning);
}

void msrVoice::appendRehearsalToVoice (S_msrRehearsal rehearsal)
{
  if (gGeneralOptions->fTraceRepeats)
    gLogIos <<
      "Appending rehearsal '" << rehearsal->getRehearsalText () <<
      "' to voice \"" << getVoiceName () << "\"" <<
      endl;

  // create the voice last segment and first measure if needed
  appendAFirstMeasureToVoiceIfNotYetDone (
    rehearsal->getInputLineNumber ());

  fVoiceLastSegment->
    appendRehearsalToSegment (rehearsal);
}

void msrVoice::appendVoiceStaffChangeToVoice (
  S_msrVoiceStaffChange voiceStaffChange)
{
  if (gGeneralOptions->fTraceVoices || gGeneralOptions->fTraceStaves)
    gLogIos <<
      "Append voice staff change " <<
      voiceStaffChange->voiceStaffChangeAsString () <<
      " to voice " <<
      " \"" << getVoiceName () << "\"" <<
      endl;

  // create the voice last segment and first measure if needed
  appendAFirstMeasureToVoiceIfNotYetDone (
    voiceStaffChange->getInputLineNumber ());

  fVoiceLastSegment->
    appendVoiceStaffChangeToSegment (voiceStaffChange);
}

void msrVoice::appendNoteToVoice (S_msrNote note) {
  if (gGeneralOptions->fTraceNotes) {
    gLogIos <<
      "Appending note:" <<
      endl;

    idtr++;
    
    gLogIos <<
      note <<
        "to voice \"" << getVoiceName () << "\"" <<
        endl;

    idtr--;
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
    note->getInputLineNumber ());

  // append the note to the last segment
  fVoiceLastSegment->
    appendNoteToSegment (note);
  
  // add a skip syllable of the same duration to the mute stanza
  rational
    noteSoundingWholeNotes =
      note->
        getNoteSoundingWholeNotes ();

  if (note->getNoteIsARest ())
    fVoiceMuteStanza->
      appendRestSyllableToStanza (
        note->getInputLineNumber (),
        noteSoundingWholeNotes);
  else
    fVoiceMuteStanza->
      appendSkipSyllableToStanza (
        note->getInputLineNumber (),
        noteSoundingWholeNotes);
}

void msrVoice::appendNoteToVoiceClone (S_msrNote note) {
  if (gGeneralOptions->fTraceNotes) {
    gLogIos <<
      "Appending note:" <<
      endl;

    idtr++;
    
    gLogIos <<
      note <<
        "to voice clone \"" << getVoiceName () << "\"" <<
        endl;

    idtr--;
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
  
  // add a skip syllable of the same duration to the mute stanza
  rational
    noteSoundingWholeNotes =
      note->
        getNoteSoundingWholeNotes ();

  if (note->getNoteIsARest ())
    fVoiceMuteStanza->
      appendRestSyllableToStanza (
        note->getInputLineNumber (),
        noteSoundingWholeNotes);
  else
    fVoiceMuteStanza->
      appendSkipSyllableToStanza (
        note->getInputLineNumber (),
        noteSoundingWholeNotes);
}

void msrVoice::appendDoubleTremoloToVoice (
  S_msrDoubleTremolo doubleTremolo)

{
  if (gGeneralOptions->fTraceTremolos) {
    gLogIos <<
      "Appending double tremolo '" <<
      doubleTremolo->doubleTremoloAsShortString () <<
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
  if (gGeneralOptions->fTraceChords) {
    gLogIos <<
      "Appending chord '" << chord->chordAsString () <<
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
  if (gGeneralOptions->fTraceTuplets) {
    gLogIos <<
      "Appending tuplet '" << tuplet->tupletAsString () <<
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

void msrVoice::appendOtherElementToVoice (S_msrElement elem) {
  if (gGeneralOptions->fTraceVoices) {
    gLogIos <<
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

void msrVoice::appendGraceNotesToVoice (S_msrGraceNotes graceNotes)
{
  if (gGeneralOptions->fTraceGraceNotes) {
    gLogIos <<
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
  if (gGeneralOptions->fTraceGraceNotes) {
    gLogIos <<
      "Prepending grace notes " << // JMI graceNotes <<
      " to voice \"" << getVoiceName () << "\"" <<
      endl;
  }

  // create the voice last segment and first measure if needed
  appendAFirstMeasureToVoiceIfNotYetDone (
    graceNotes->getInputLineNumber ());

  fVoiceFirstSegment->
    prependGraceNotesToSegment (graceNotes);

  fMusicHasBeenInsertedInVoice = true;
}

void msrVoice::appendAfterGraceNotesToVoice (
  S_msrAfterGraceNotes afterGraceNotes)
{
  if (gGeneralOptions->fTraceGraceNotes) {
    gLogIos <<
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
  if (gGeneralOptions->fTraceGraceNotes) {
    gLogIos <<
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
  S_msrSyllable syllable)
{
  // append syllable to this voice
  if (gGeneralOptions->fTraceLyrics) {
    gLogIos <<
      "Appending syllable '" <<
      syllable->syllableAsString () <<
      "' to voice \"" << fVoiceName << "\"" <<
      endl;
  }

  // fetch stanzaNumber in this voice
  S_msrStanza
    stanza =
      createStanzaInVoiceIfNotYetDone (
        inputLineNumber, stanzaNumber);

  // add the syllable to the stanza
  stanza->
    appendSyllableToStanza (
      syllable);
}

void msrVoice::appendBarCheckToVoice (S_msrBarCheck barCheck)
{
  if (gGeneralOptions->fTraceMeasures)
    gLogIos <<
      "Appending bar check '" << barCheck->barCheckAsString () <<
      "' to voice \"" << getVoiceName () <<  "\"" <<
      endl;

  // create the voice last segment and first measure if needed
  appendAFirstMeasureToVoiceIfNotYetDone (
    barCheck->getInputLineNumber ());

  fVoiceLastSegment->
    appendBarCheckToSegment (barCheck);

  // add bar check syllable to the voice mute stanza
  fVoiceMuteStanza->
    appendBarcheckSyllableToStanza (
      barCheck->getInputLineNumber (),  // [passer barCheck directement? JMI
      barCheck->getNextBarNumber ());
}

void msrVoice::appendBarNumberCheckToVoice (
  S_msrBarNumberCheck barNumberCheck)
{
  if (gGeneralOptions->fTraceMeasures)
    gLogIos <<
      "Appending barnumber check '" <<
      barNumberCheck->barNumberCheckAsString () <<
      "' to voice \"" << getVoiceName () <<  "\"" <<
      endl;

  // create the voice last segment and first measure if needed
  appendAFirstMeasureToVoiceIfNotYetDone (
    barNumberCheck->getInputLineNumber ());

  fVoiceLastSegment->
    appendBarNumberCheckToSegment (barNumberCheck);

  // add barnumber check syllable to the voice mute stanza
  fVoiceMuteStanza->
    appendBarNumberCheckSyllableToStanza (
      barNumberCheck->getInputLineNumber (),  // [passer barNumberCheck directement? JMI
      barNumberCheck->getNextBarNumber ());
}

void msrVoice::appendLineBreakToVoice (S_msrLineBreak lineBreak)
{
  if (gGeneralOptions->fTraceMeasures)
    gLogIos <<
      "Appending line break '" << lineBreak->lineBreakAsString () <<
      "' to voice \"" << getVoiceName () << "\"" <<
      endl;

  // create the voice last segment and first measure if needed
  appendAFirstMeasureToVoiceIfNotYetDone (
    lineBreak->getInputLineNumber ());

  fVoiceLastSegment->
    appendLineBreakToSegment (lineBreak);

  // add break syllable to the voice mute stanza
  fVoiceMuteStanza->
    appendLineLineBreakSyllableToStanza (
      lineBreak->getInputLineNumber (),
      lineBreak->getNextBarNumber ());
}

void msrVoice::appendPageBreakToVoice (S_msrPageBreak pageBreak)
{
  if (gGeneralOptions->fTraceMeasures)
    gLogIos <<
      "Appending page break '" << pageBreak->pageBreakAsString () <<
      "' to voice \"" << getVoiceName () << "\"" <<
      endl;

  // create the voice last segment and first measure if needed
  appendAFirstMeasureToVoiceIfNotYetDone (
    pageBreak->getInputLineNumber ());

  fVoiceLastSegment->
    appendPageBreakToSegment (pageBreak);
}

void msrVoice::createRepeatAndAppendItToVoice (int inputLineNumber)
{
  switch (fVoiceKind) {
    case msrVoice::kMasterVoice:
    case msrVoice::kRegularVoice:
    case msrVoice::kHarmonyVoice:
    case msrVoice::kFiguredBassVoice:
    case msrVoice::kSilentVoice:
      {
        // finalize current measure in voice
        finalizeCurrentMeasureInVoice (
          inputLineNumber);
          
        // create a repeat
        if (gGeneralOptions->fTraceRepeats)
          gLogIos <<
            "==> Creating and appending a repeat in voice \"" <<
            getVoiceName () <<
            "\"" <<
            ", line " << inputLineNumber <<
            endl;
      
        S_msrRepeat
          repeat =
            msrRepeat::create (
              inputLineNumber,
              this);

        // create a repeat common part from current last segment
        if (gGeneralOptions->fTraceRepeats)
          gLogIos <<
            "==> Creating a repeat common part from current last segment in voice \"" <<
            getVoiceName () <<
            "\"" <<
            ", line " << inputLineNumber <<
            endl;
      
        S_msrRepeatCommonPart
          repeatCommonPart =
            msrRepeatCommonPart::create (
              inputLineNumber,
              fVoiceLastSegment,
              repeat);

        // set current last segment as the repeat common segment
        if (gGeneralOptions->fTraceRepeats)
          gLogIos <<
            "Setting repeat common part in voice \"" <<
            getVoiceName () <<
            "\"" <<
            endl;
      /*
        repeat->
          setRepeatCommonSegment (
            fVoiceLastSegment);
            */
        repeat->
          setRepeatCommonPart (
            repeatCommonPart);
          
        // register repeat as the (new) current repeat
        if (gGeneralOptions->fTraceRepeats)
          gLogIos <<
            "Registering repeat as the new current repeat in voice \"" <<
            getVoiceName () <<
            "\"" <<
            endl;
                
        fVoiceCurrentRepeat =
          repeat;
      
        // append it to the list of repeats and segments
        fVoiceInitialRepeatsAndSegments.push_back (
          repeat);
      
        // create a new last segment containing a new measure for the voice
        if (gGeneralOptions->fTraceSegments || gGeneralOptions->fTraceVoices)
          gLogIos <<
            "==> Creating a new last segment containing a new measure for voice \"" <<
            fVoiceName << "\"" <<
            endl;
            
        createNewLastSegmentAndANewMeasureForVoice (
          inputLineNumber);
      }
      break;
  } // switch
}

void msrVoice::createMeasureRepeatFromItsFirstMeasureInVoice (
  int inputLineNumber,
  int measureRepeatMeasuresNumber,
  int measureRepeatSlashes)
{
  switch (fVoiceKind) {
    case msrVoice::kMasterVoice:
    case msrVoice::kRegularVoice:
    case msrVoice::kHarmonyVoice:
    case msrVoice::kFiguredBassVoice:
    case msrVoice::kSilentVoice:
      {
        // create a measure repeat
        if (gGeneralOptions->fTraceRepeats) {
          gLogIos <<
            "==> Creating a '" <<
            measureRepeatMeasuresNumber <<
            "' measure, '" <<
            measureRepeatSlashes <<
            "' slashes repeat from it's first measure in voice \"" <<
            getVoiceName () <<
            "\"" <<
            ", line " << inputLineNumber <<
            endl;
        }
      
        // print current voice contents
        if (gGeneralOptions->fTraceRepeats || gGeneralOptions->fTraceVoices)
          gLogIos <<
            "The current voice contents of voice \"" <<
            fVoiceName << "\" is:" <<
            endl;

        idtr++;
        gLogIos <<
          idtr;
        print (gLogIos);
        idtr--;

        // this occurs after a measure has just been created,
        // hence the repeated measure/measures is/are the
        // measureRepeatMeasuresNumber measures preceding the last one

        int
          lastSegmentMeasuresNumber =
            fVoiceLastSegment->
              getSegmentMeasuresList ().size (),
          availableMeasuresNumber =
            lastSegmentMeasuresNumber - 1;
            
        if (
          availableMeasuresNumber < measureRepeatMeasuresNumber) {
          stringstream s;

          s <<
            "attempting to create a measure repeat with " <<
            measureRepeatMeasuresNumber <<
            " measures while current last segment has only " <<
            availableMeasuresNumber <<
            " available";

          msrInternalError (
            inputLineNumber, s.str ());
        }

        // grab the just created last measure from the voice,
        // (i.e. the one containing:
        //   <measure-repeat ... type="start">2</measure-repeat>)
        // which is the first replica measure
        S_msrMeasure
          firstReplicaMeasure =
            removeLastMeasureFromVoice (
              inputLineNumber);

        // grab the (new current) last measure from the voice,
        // which is the repeated measure
        S_msrMeasure
          repeatedMeasure =
            removeLastMeasureFromVoice (
              inputLineNumber);

        // create the measure repeat repeated segment
        S_msrSegment
          repeatedSegment =
            msrSegment::create (
              inputLineNumber,
              this);

        // remove the repeated measure(s) for the last segment
        // and preppend  them to the repeated segment
        for (int i = 0; i< measureRepeatMeasuresNumber; i++) {
          S_msrMeasure
            lastMeasure =
              removeLastMeasureFromVoice (
                inputLineNumber);

          repeatedSegment->
            prependMeasureToSegment (
              lastMeasure);
        }
            
        // create the measure pattern
        if (gGeneralOptions->fTraceSegments || gGeneralOptions->fTraceVoices)
          gLogIos <<
            "==> Creating a measure pattern for voice \"" <<
            fVoiceName << "\" is:" <<
            endl;

        S_msrMeasureRepeatPattern
          measureRepeatPattern =
            msrMeasureRepeatPattern::create (
              inputLineNumber,
              this);

        // set the repeated segment as the measure repeat pattern segment
        measureRepeatPattern->
          setMeasureRepeatPatternSegment (
            repeatedSegment);
            
        // create the measure repeat
        if (fVoicePendingMeasureRepeat) {
          stringstream s;

          s <<
            "attempting to create a measure repeat while another one is pending";

          msrInternalError (
            inputLineNumber, s.str ());
        }
        
        fVoicePendingMeasureRepeat =
          msrMeasureRepeat::create (
            inputLineNumber,
            measureRepeatMeasuresNumber,
            measureRepeatSlashes,
            this);

        // set the measure repeat pattern
        fVoicePendingMeasureRepeat->
          setMeasureRepeatPattern (
            measureRepeatPattern);
        
        // create a new last segment to collect the measure repeat replicas,
        // containing the first, yet incomplete, replica
        if (gGeneralOptions->fTraceSegments || gGeneralOptions->fTraceVoices)
          gLogIos <<
            "==> Creating a new last segment with the first replica for voice \"" <<
            fVoiceName << "\"" <<
            endl;
            
        createNewLastSegmentFromFirstMeasureForVoice (
          inputLineNumber,
          firstReplicaMeasure);

        // print resulting voice contents
        if (gGeneralOptions->fTraceRepeats || gGeneralOptions->fTraceVoices)
          gLogIos <<
            "The resulting voice contents of voice \"" <<
            fVoiceName << "\" is:" <<
            endl;

        idtr++;
        gLogIos <<
          idtr;
        print (gLogIos);
        idtr--;

        // keep the measure repeat pending
      }
      break;
  } // switch
}

void msrVoice::appendPendingMeasureRepeatToVoice (
  int inputLineNumber)
{
  switch (fVoiceKind) {
    case msrVoice::kMasterVoice:
    case msrVoice::kRegularVoice:
    case msrVoice::kHarmonyVoice:
    case msrVoice::kFiguredBassVoice:
    case msrVoice::kSilentVoice:
      {
        /* JMI
        // print current voice contents
        if (gGeneralOptions->fTraceSegments || gGeneralOptions->fTraceVoices)
          gLogIos <<
            "==================> The current voice contents of voice \"" <<
            fVoiceName << "\" is:" <<
            endl <<
            idtr;

        idtr++;
        print (gLogIos);
        idtr--;
*/

        // does the pending measure repeat exist?
        if (! fVoicePendingMeasureRepeat) {
          stringstream s;

          s <<
            "attempting to append a pending measure repeat which doesn't exist";

          msrInternalError (
            inputLineNumber, s.str ());
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
            inputLineNumber, s.str ());
        }

        S_msrMeasure
          nextMeasureAfterMeasureRepeat =
            voiceLastSegmentMeasureList.back ();

            /* JMI
        gLogIos <<
          endl <<
          "==========> nextMeasureAfterMeasureRepeat:" <<
          endl;
          
        nextMeasureAfterMeasureRepeat->print (gLogIos);
          
        gLogIos <<
          endl;
*/

        // remove the next measure from the last segment's measure list
        voiceLastSegmentMeasureList.pop_back ();

         // create the measure repeat replicas contents
        if (gGeneralOptions->fTraceSegments || gGeneralOptions->fTraceVoices)
          gLogIos <<
            "==> Creating a measure repeat replicas contents for voice \"" <<
            fVoiceName << "\" is:" <<
            endl;

        S_msrMeasureRepeatReplicas
          measureRepeatReplicas =
            msrMeasureRepeatReplicas::create (
              inputLineNumber,
              this);

        // set the voice last segment as the measure repeat replicas segment
        measureRepeatReplicas->
          setMeasureRepeatReplicasSegment (
            fVoiceLastSegment);

        // set the measure repeat replicas contents
        fVoicePendingMeasureRepeat->
          setMeasureRepeatReplicas (
            measureRepeatReplicas);
        

/* JMI BOF
       // set last segment as the measure repeat replicas segment
        if (gGeneralOptions->fTraceRepeats)
          gLogIos <<
            "Setting current last segment as measure repeat replicas segment in voice \"" <<
            getVoiceName () <<
            "\"" <<
            endl;
      
        fVoicePendingMeasureRepeat->
          setMeasureRepeatReplicas (
            fVoiceLastSegment);

        // append pending measure repeat to the list of repeats and segments
        fVoiceInitialRepeatsAndSegments.push_back (
          fVoicePendingMeasureRepeat);

        // set multipleRestContents as the multiple rest contents
        if (gGeneralOptions->fTraceRepeats)
          gLogIos <<
            "Setting current last segment as multiple rest segment in voice \"" <<
            getVoiceName () <<
            "\"" <<
            endl;
      
        fVoicePendingMultipleRest->
          setMultipleRestContents (
            multipleRestContents);
*/





/* JMI
        // print current voice contents
        if (gGeneralOptions->fTraceSegments || gGeneralOptions->fTraceVoices)
          gLogIos <<
            "==================> The current voice contents of voice \"" <<
            fVoiceName << "\" is:" <<
            endl <<
            idtr;

        idtr++;
        print (gLogIos);
        idtr--;
        */

        // create a new last segment to collect the remainder of the voice,
        // containing the next, yet incomplete, measure
        if (gGeneralOptions->fTraceSegments || gGeneralOptions->fTraceVoices)
          gLogIos <<
            "==> Creating a new last segment with the measure repeat next measure for voice \"" <<
            fVoiceName << "\"" <<
            endl;
            
        createNewLastSegmentFromFirstMeasureForVoice (
          inputLineNumber,
          nextMeasureAfterMeasureRepeat);

        // print resulting voice contents
        if (gGeneralOptions->fTraceSegments || gGeneralOptions->fTraceVoices)
          gLogIos <<
            "The resulting voice contents of voice \"" <<
            fVoiceName << "\" is:" <<
            endl;

        idtr++;
        print (gLogIos);
        idtr--;

        // forget about this pending measure repeat
        fVoicePendingMeasureRepeat = 0;
      }
      break;
  } // switch
}

void msrVoice::setVoiceContainsMultipleRests (
  int inputLineNumber)
{
  if (gGeneralOptions->fTraceRepeats) {
    gLogIos <<
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
    case msrVoice::kMasterVoice:
    case msrVoice::kRegularVoice:
    case msrVoice::kHarmonyVoice:
    case msrVoice::kFiguredBassVoice:
    case msrVoice::kSilentVoice:
      {
        // create a multiple rest
        if (gGeneralOptions->fTraceRepeats) {
          gLogIos <<
            "==> Creating a multiple rest in voice \"" <<
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

/* JMI
        // create the multiple rest rests segment
        S_msrSegment
          restsSegment =
            msrSegment::create (
              inputLineNumber,
              this);
*/

        // create the multiple rest
        if (fVoicePendingMultipleRest) {
          stringstream s;

          s <<
            "attempting to create a multiple rest while another one is pending";

          msrInternalError (
            inputLineNumber, s.str ());
        }
        
        fVoicePendingMultipleRest =
          msrMultipleRest::create (
            inputLineNumber,
            multipleRestMeasuresNumber,
            this);

        // create a new segment to collect the multiple rest measures,
        // containing the first, rest measure
        if (gGeneralOptions->fTraceSegments || gGeneralOptions->fTraceVoices)
          gLogIos <<
            "==> Creating a new last segment containing the first rest measure in voice \"" <<
            fVoiceName << "\"" <<
            endl;
            
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
        if (gGeneralOptions->fTraceSegments || gGeneralOptions->fTraceVoices)
          gLogIos <<
            "The resulting voice contents of voice \"" <<
            fVoiceName << "\" is:" <<
            endl;

        idtr++;
        print (gLogIos);
        idtr--;

        // keep the multiple rest pending
      }
      break;
  } // switch
}

void msrVoice::appendPendingMultipleRestToVoice (
  int inputLineNumber)
{
  switch (fVoiceKind) {
    case msrVoice::kMasterVoice:
    case msrVoice::kRegularVoice:
    case msrVoice::kHarmonyVoice:
    case msrVoice::kFiguredBassVoice:
    case msrVoice::kSilentVoice:
      {
        if (gGeneralOptions->fTraceRepeats) {
          gLogIos <<
            "Appending the pending multiple rest to voice \"" <<
            getVoiceName () <<
            "\"" <<
            ", line " << inputLineNumber <<
            endl;
        }

/*
        // print current voice contents
        if (gGeneralOptions->fTraceSegments || gGeneralOptions->fTraceVoices)
          gLogIos <<
            "==================> The current voice contents of voice \"" <<
            fVoiceName << "\" is:" <<
            endl <<
            idtr;

        idtr++;
        print (gLogIos);
        idtr--;
*/

        // does the pending multiple rest exist?
        if (! fVoicePendingMultipleRest) {
          stringstream s;

          s <<
            "attempting to append a pending multiple rest which doesn't exist";

          msrInternalError (
            inputLineNumber, s.str ());
        }

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
            inputLineNumber, s.str ());
        }

        S_msrMeasure
          nextMeasureAfterMultipleRest =
            voiceLastSegmentMeasureList.back ();

        /* JMI
        gLogIos <<
          endl <<
          "==========> nextMeasureAfterMultipleRest:" <<
          endl;

          
        nextMeasureAfterMultipleRest->
          print (gLogIos);
          
        gLogIos <<
          endl;
          */
          
        // remove the next measure from the last segment's measure list
        voiceLastSegmentMeasureList.pop_back ();

        // create the multiple rest contents
        if (gGeneralOptions->fTraceSegments || gGeneralOptions->fTraceVoices)
          gLogIos <<
            "==> Creating a measure pattern for voice \"" <<
            fVoiceName << "\" is:" <<
            endl;

        S_msrMultipleRestContents
          multipleRestContents =
            msrMultipleRestContents::create (
              inputLineNumber,
              this);

        // set voice last segment as the multiple rest segment
        if (gGeneralOptions->fTraceRepeats)
          gLogIos <<
            "Setting current last segment as multiple rest segment in voice \"" <<
            getVoiceName () <<
            "\"" <<
            endl;
      
        multipleRestContents->
          setMultipleRestContentsSegment (
            fVoiceLastSegment);

        // set multipleRestContents as the multiple rest contents
        if (gGeneralOptions->fTraceRepeats)
          gLogIos <<
            "Setting current last segment as multiple rest segment in voice \"" <<
            getVoiceName () <<
            "\"" <<
            endl;
      
        fVoicePendingMultipleRest->
          setMultipleRestContents (
            multipleRestContents);

        // append pending multiple rest to the list of repeats and segments
        fVoiceInitialRepeatsAndSegments.push_back (
          fVoicePendingMultipleRest);

/* JMI
        // print current voice contents
        if (gGeneralOptions->fTraceSegments || gGeneralOptions->fTraceVoices)
          gLogIos <<
            "==================> The current voice contents of voice \"" <<
            fVoiceName << "\" is:" <<
            endl <<
            idtr;

        idtr++;
        print (gLogIos);
        idtr--;
*/

        // create a new segment to collect the remainder of the voice,
        // containing the next, yet incomplete, measure
        if (gGeneralOptions->fTraceSegments || gGeneralOptions->fTraceVoices)
          gLogIos <<
            "==> Creating a new last segment with the measure after the multiple rests for voice \"" <<
            fVoiceName << "\"" <<
            endl;
            
        createNewLastSegmentFromFirstMeasureForVoice (
          inputLineNumber,
          nextMeasureAfterMultipleRest);

/* JMI
        // append the next measure after the multiple rest
        // to the new last segment
        fVoiceLastSegment->
          appendMeasureToSegment (
            nextMeasureAfterMultipleRest);
*/
        // print resulting voice contents
        if (gGeneralOptions->fTraceSegments || gGeneralOptions->fTraceVoices)
          gLogIos <<
            "The resulting voice contents of voice \"" <<
            fVoiceName << "\" is:" <<
            endl;

        idtr++;
        print (gLogIos);
        idtr--;

        // forget about this pending multiple rest
        fVoicePendingMultipleRest = 0;
      }
      break;
  } // switch
}

void msrVoice::appendMultipleRestCloneToVoice (
  int               inputLineNumber,
  S_msrMultipleRest multipleRest)
{
  switch (fVoiceKind) {
    case msrVoice::kMasterVoice:
    case msrVoice::kRegularVoice:
    case msrVoice::kHarmonyVoice:
    case msrVoice::kFiguredBassVoice:
    case msrVoice::kSilentVoice:
      {
        if (gGeneralOptions->fTraceRepeats) {
          gLogIos <<
            "Appending multiple rest '" <<
            multipleRest->multipleRestAsString () <<
            " to voice clone \"" <<
            getVoiceName () <<
            "\"" <<
            ", line " << inputLineNumber <<
            endl;
        }

/*
        // print current voice contents
        if (gGeneralOptions->fTraceSegments || gGeneralOptions->fTraceVoices)
          gLogIos <<
            "==================> The current voice contents of voice \"" <<
            fVoiceName << "\" is:" <<
            endl <<
            idtr;

        idtr++;
        print (gLogIos);
        idtr--;
*/

        // append pending multiple rest to the list of repeats and segments
        fVoiceInitialRepeatsAndSegments.push_back (
          multipleRest);

        // print resulting voice contents
        if (gGeneralOptions->fTraceSegments || gGeneralOptions->fTraceVoices)
          gLogIos <<
            "The resulting voice contents of voice \"" <<
            fVoiceName << "\" is:" <<
            endl;

        idtr++;
        print (gLogIos);
        idtr--;
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
    repeatCLone != 0,
    "repeatCLone is null");
      
  switch (fVoiceKind) {
    case msrVoice::kMasterVoice:
    case msrVoice::kRegularVoice:
    case msrVoice::kHarmonyVoice:
    case msrVoice::kFiguredBassVoice:
    case msrVoice::kSilentVoice:
      {
        if (gGeneralOptions->fTraceRepeats)
          gLogIos <<
            "Appending repeat clone to voice clone \"" <<
            getVoiceName () <<  "\"" <<
            endl;
      
        // create a repeat common part from current last segment
        if (gGeneralOptions->fTraceRepeats)
          gLogIos <<
            "==> Creating a repeat clone common part from current last segment in voice \"" <<
            getVoiceName () <<
            "\"" <<
            ", line " << inputLineNumber <<
            endl;
      
        S_msrRepeatCommonPart
          repeatCommonPart =
            msrRepeatCommonPart::create (
              inputLineNumber,
              fVoiceLastSegment,
              repeatCLone);

        // set current last segment as the repeat common segment
        if (gGeneralOptions->fTraceRepeats)
          gLogIos <<
            "Setting repeat common part in voice \"" <<
            getVoiceName () <<
            "\"" <<
            endl;
      /*
        repeat->
          setRepeatCommonSegment (
            fVoiceLastSegment);
            */
        repeatCLone->
          setRepeatCommonPart (
            repeatCommonPart);

/*
        // set current last segment as the repeat common segment
        if (gGeneralOptions->fTraceRepeats)
          gLogIos <<
            "Segging current last segment as repeat common segment in voice \"" <<
            getVoiceName () <<
            "\"" <<
            endl;
      
        repeatCLone->
          setRepeatCommonSegment (
            fVoiceLastSegment);
 */         
        // register repeat clone as the (new) current repeat
        if (gGeneralOptions->fTraceRepeats)
          gLogIos <<
            "Registering repeat clone as the new current repeat in voice \"" <<
            getVoiceName () <<
            "\"" <<
            endl;
      
        fVoiceCurrentRepeat =
          repeatCLone;
      
        // append it to the list of repeats and segments
        fVoiceInitialRepeatsAndSegments.push_back (
          repeatCLone);
      
        // create a new segment for the voice
        if (gGeneralOptions->fTraceSegments || gGeneralOptions->fTraceVoices)
          gLogIos <<
            "==> Creating a new last segment for voice BOF JMI \"" <<
            fVoiceName << "\"" <<
            endl;
            
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
    case msrVoice::kMasterVoice:
    case msrVoice::kRegularVoice:
    case msrVoice::kHarmonyVoice:
    case msrVoice::kFiguredBassVoice:
    case msrVoice::kSilentVoice:
      {
        if (gGeneralOptions->fTraceRepeats)
          gLogIos <<
            "Appending a repeat ending to voice \"" <<
            getVoiceName () <<  "\"" <<
            ", line " << inputLineNumber <<
            endl;
      
        S_msrRepeatEnding
          repeatEnding =
            msrRepeatEnding::create (
              inputLineNumber,
              repeatEndingNumber,
              repeatEndingKind,
              fVoiceLastSegment,
              fVoiceCurrentRepeat);
      
        // add the repeat ending it to the voice current repeat
        if (gGeneralOptions->fTraceRepeats)
          gLogIos <<
            "Appending repeat ending to current repeat in voice \"" <<
            fVoiceName <<
            "\"" <<
            endl;
            
        fVoiceCurrentRepeat->
          addRepeatEnding (repeatEnding);
      
      
        // create a new last segment containing a new measure for the voice
        if (gGeneralOptions->fTraceSegments || gGeneralOptions->fTraceVoices)
          gLogIos <<
            "==> Creating a new last segment containing a new measure for voice \"" <<
            fVoiceName << "\"" <<
            endl;
            
        createNewLastSegmentAndANewMeasureForVoice (
          inputLineNumber);
      }
      break;
  } // switch
}

void msrVoice:: appendRepeatEndingCloneToVoice ( // JMI
  S_msrRepeatEnding repeatEndingClone)
{
  switch (fVoiceKind) {
    case msrVoice::kMasterVoice:
    case msrVoice::kRegularVoice:
    case msrVoice::kHarmonyVoice:
    case msrVoice::kFiguredBassVoice:
    case msrVoice::kSilentVoice:
      {
        // add the repeat ending it to the voice current repeat
        if (gGeneralOptions->fTraceRepeats)
          gLogIos <<
            "Appending repeat ending clone to current repeat in voice \"" <<
            fVoiceName <<
            "\"" <<
            endl;
            
        // sanity check
        msrAssert(
          fVoiceCurrentRepeat != 0,
          "fVoiceCurrentRepeat is null");
          
        fVoiceCurrentRepeat->
          addRepeatEnding (repeatEndingClone);
      
        // create a new segment for the voice
        if (gGeneralOptions->fTraceSegments || gGeneralOptions->fTraceVoices)
          gLogIos <<
            "==> Creating a new last segment for voice \"" <<
            fVoiceName <<
            "\"" <<
            endl;
            
        createNewLastSegmentForVoice (
          repeatEndingClone->getInputLineNumber ());
      }
      break;
  } // switch
}

void msrVoice::prependBarlineToVoice (S_msrBarline barline)
{
  if (gGeneralOptions->fTraceRepeats)
    gLogIos <<
      "Prepending barline '" <<
      barline->barlineAsString () <<
      "' to voice \"" << getVoiceName () << "\"" <<
      ":" << endl;
      
  // create the voice last segment and first measure if needed
  appendAFirstMeasureToVoiceIfNotYetDone (
    barline->getInputLineNumber ());

  fVoiceLastSegment->
    prependBarlineToSegment (barline);
}

void msrVoice::appendBarlineToVoice (S_msrBarline barline)
{
  if (gGeneralOptions->fTraceRepeats)
    gLogIos <<
      "Appending barline '" <<
      barline->barlineAsString () <<
      "' to voice \"" << getVoiceName () << "\"" <<
      ":" << endl;
      
  // create the voice last segment and first measure if needed
 // appendAFirstMeasureToVoiceIfNotYetDone (
 //JMI   barline->getInputLineNumber ());

  fVoiceLastSegment->
    appendBarlineToSegment (barline);
}

void msrVoice::appendSegnoToVoice (S_msrSegno segno)
{
  if (gGeneralOptions->fTraceRepeats)
    gLogIos <<
      "Appending a segno to voice \"" << getVoiceName () << "\"" <<
      endl;

  // create the voice last segment and first measure if needed
  appendAFirstMeasureToVoiceIfNotYetDone (
    segno->getInputLineNumber ());

  fVoiceLastSegment->
    appendSegnoToSegment (segno);
}

void msrVoice::appendCodaToVoice (S_msrCoda coda)
{
  if (gGeneralOptions->fTraceRepeats)
    gLogIos <<
      "Appending a coda to voice \"" << getVoiceName () << "\"" <<
      ":" << endl;

  // create the voice last segment and first measure if needed
  appendAFirstMeasureToVoiceIfNotYetDone (
    coda->getInputLineNumber ());

  fVoiceLastSegment->
    appendCodaToSegment (coda);
}

void msrVoice::appendEyeGlassesToVoice (S_msrEyeGlasses eyeGlasses)
{
  if (gGeneralOptions->fTraceNotes)
    gLogIos <<
      "Appending a eyeGlasses to voice \"" << getVoiceName () << "\"" <<
      endl;

  // create the voice last segment and first measure if needed
  appendAFirstMeasureToVoiceIfNotYetDone (
    eyeGlasses->getInputLineNumber ());

  fVoiceLastSegment->
    appendEyeGlassesToSegment (eyeGlasses);
}

void msrVoice::appendPedalToVoice (S_msrPedal pedal)
{
  if (gGeneralOptions->fTraceNotes)
    gLogIos <<
      "Appending a pedal to voice \"" << getVoiceName () << "\"" <<
      endl;

  // create the voice last segment and first measure if needed
  appendAFirstMeasureToVoiceIfNotYetDone (
    pedal->getInputLineNumber ());

  fVoiceLastSegment->
    appendPedalToSegment (pedal);
}

/*
S_msrElement msrVoice::removeLastElementFromVoice (  // JMI
  int inputLineNumber)
{
  if (gGeneralOptions->fTraceVoices)
    gLogIos <<
      "Removing last note" <<
      " from voice " << getVoiceName () <<
      endl;

  return
    fVoiceLastSegment->
      removeLastElementFromSegment (inputLineNumber);
}
*/

void msrVoice::removeNoteFromVoice (
  int       inputLineNumber,
  S_msrNote note)
{
  if (gGeneralOptions->fTraceChords) {
    gLogIos <<
      "Removing first chord note '" <<
      note->noteAsShortString () <<
      "' from voice \"" << getVoiceName () << "\"" <<
      endl;
  }

  fVoiceLastSegment->
    removeNoteFromSegment (
      inputLineNumber,
      note);
}

S_msrMeasure msrVoice::removeLastMeasureFromVoice (
  int inputLineNumber)
{
  if (gGeneralOptions->fTraceMeasures || gGeneralOptions->fTraceVoices)
    gLogIos <<
      "Removing last measure from voice \"" <<
      getVoiceName () <<
      "\"" <<
      endl;

  // remove (new) last measure and return it
  return
    fVoiceLastSegment->
      removeLastMeasureFromSegment (
        inputLineNumber);
}

void msrVoice::finalizeCurrentMeasureInVoice (
  int    inputLineNumber)
{
  if (gGeneralOptions->fTraceMeasures || gGeneralOptions->fTraceVoices) {
    gLogIos <<
      "Finalizing current measure in voice \"" <<
      getVoiceName () <<
      "\"" <<
      endl;
      
    idtr++;

    const int fieldWidth = 26;
    
    gLogIos << left <<
      setw (fieldWidth) <<
      "partMeasureLengthHighTide" << " = " <<
      fetchVoicePartUplink ()->
        getPartMeasureLengthHighTide () <<
      endl;
        
    idtr--;
  }

  // make sure the voice's last segment exists
  appendAFirstMeasureToVoiceIfNotYetDone (
    inputLineNumber);

  // finalize last segment' current measure
  fVoiceLastSegment->
    finalizeCurrentMeasureInSegment (
      inputLineNumber);
}

msrVoice::msrVoiceFinalizationStatus msrVoice::finalizeVoice (
  int inputLineNumber)
{
  msrVoice::msrVoiceFinalizationStatus
    result =
      msrVoice::kKeepVoice;
  
  if (gGeneralOptions->fTraceVoices) {
    gLogIos <<
      "Finalizing voice \"" <<
      getVoiceName () <<
      "\", line " << inputLineNumber <<
      endl;
  }

  // should the voice be erased?
  switch (fVoiceKind) {
    case msrVoice::kMasterVoice:
      if (! getMusicHasBeenInsertedInVoice ()) {
        if (gGeneralOptions->fTraceStaves || gGeneralOptions->fTraceVoices) {
          gLogIos <<
            "Erasing empty master voice \"" << // JMI
            getVoiceName () <<
            "\" in staff " <<
            fVoiceStaffUplink->getStaffName () <<
            ", line " << inputLineNumber <<
            endl;
        }

        result = msrVoice::kEraseVoice;
      }
      break;

    case msrVoice::kRegularVoice:
      if (! getMusicHasBeenInsertedInVoice ()) {
        if (gGeneralOptions->fTraceStaves || gGeneralOptions->fTraceVoices) {
          gLogIos <<
            "Erasing empty regular voice \"" <<
            getVoiceName () <<
            "\" in staff " <<
            fVoiceStaffUplink->getStaffName () <<
            ", line " << inputLineNumber <<
            endl;
        }

        result = msrVoice::kEraseVoice;
      }
      break;

   case msrVoice::kHarmonyVoice:
      if (
        !
          (
          getMusicHasBeenInsertedInVoice ()
             ||
          gMsrOptions->fKeepEmptyHarmonyVoices
          )
        ) {
        if (gGeneralOptions->fTraceStaves || gGeneralOptions->fTraceVoices) {
          gLogIos <<
            "Erasing empty harmony voice \"" <<
            getVoiceName () <<
            "\" in staff " <<
            fVoiceStaffUplink->getStaffName () <<
            ", line " << inputLineNumber <<
            endl;
        }
            
        result = msrVoice::kEraseVoice;
      }
      break;
              
   case msrVoice::kFiguredBassVoice:
      if (
        !
          (
          getMusicHasBeenInsertedInVoice ()
             ||
          gMsrOptions->fKeepEmptyHarmonyVoices
          )
        ) {
        if (gGeneralOptions->fTraceStaves || gGeneralOptions->fTraceVoices) {
          gLogIos <<
            "Erasing empty figured bass voice \"" <<
            getVoiceName () <<
            "\" in staff " <<
            fVoiceStaffUplink->getStaffName () <<
            ", line " << inputLineNumber <<
            endl;
        }
            
        result = msrVoice::kEraseVoice;
      }
      break;
              
    case msrVoice::kSilentVoice:
      if (
        !
          (
            gMsrOptions->fKeepSilentVoices
              ||
            gMsrOptions->fShowSilentVoices
          )
        ) {
        if (gGeneralOptions->fTraceStaves || gGeneralOptions->fTraceVoices) {
          gLogIos <<
            "Erasing silent voice \"" <<
            getVoiceName () <<
            "\" in staff " <<
            fVoiceStaffUplink->getStaffName () <<
            ", line " << inputLineNumber <<
            endl;
        }

        result = msrVoice::kEraseVoice;
      }
      break;
  } // switch

  if (! gMsrOptions->fKeepMuteStanzas) { // JMI
// JMI    delete (fVoiceMuteStanza);
 // JMI   fVoiceMuteStanza = 0;
  }

  return result;
}

void msrVoice::acceptIn (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors)
    gLogIos <<
      "% ==> msrVoice::acceptIn()" <<
      endl;
      
  if (visitor<S_msrVoice>*
    p =
      dynamic_cast<visitor<S_msrVoice>*> (v)) {
        S_msrVoice elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors)
          gLogIos <<
            "% ==> Launching msrVoice::visitStart()" <<
             endl;
        p->visitStart (elem);
  }
}

void msrVoice::acceptOut (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors)
    gLogIos <<
      "% ==> msrVoice::acceptOut()" <<
      endl;

  if (visitor<S_msrVoice>*
    p =
      dynamic_cast<visitor<S_msrVoice>*> (v)) {
        S_msrVoice elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors)
          gLogIos <<
            "% ==> Launching msrVoice::visitEnd()" <<
            endl;
        p->visitEnd (elem);
  }
}

void msrVoice::browseData (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors)
    gLogIos <<
      "% ==> msrVoice::browseData()" <<
      endl;

  // browse the voice initial repeats and segments
  if (fVoiceInitialRepeatsAndSegments.size ()) {
    for (
      list<S_msrElement>::const_iterator i = fVoiceInitialRepeatsAndSegments.begin();
      i != fVoiceInitialRepeatsAndSegments.end();
      i++) {
      // browse the element
      msrBrowser<msrElement> browser (v);
      browser.browse (*(*i));
    } // for
  }

  // browse the segment
  msrBrowser<msrSegment> browser (v);
  browser.browse (*fVoiceLastSegment);

  // browse the voice stanzas
  if (fVoiceStanzasMap.size ()) {
    for (
      map<string, S_msrStanza>::const_iterator i = fVoiceStanzasMap.begin();
      i != fVoiceStanzasMap.end();
      i++) {
      S_msrStanza stanza = (*i).second;

      if (stanza->getStanzaTextPresent ()) {
        // browse the stanza
        msrBrowser<msrStanza> browser (v);
        browser.browse (*(stanza));
      }
    } // for
  }

  if (gMsrOptions->fTraceMsrVisitors)
    gLogIos <<
      "% <== msrVoice::browseData()" <<
      endl;
}

string msrVoice::voiceKindAsString (
  msrVoiceKind voiceKind)
{
  string result;
  
  switch (voiceKind) {
    case msrVoice::kMasterVoice:
      result = "master";
      break;
    case msrVoice::kRegularVoice:
      result = "regular";
      break;
    case msrVoice::kHarmonyVoice:
      result = "harmony";
      break;
    case msrVoice::kFiguredBassVoice:
      result = "figured bass";
      break;
    case msrVoice::kSilentVoice:
      result = "silent";
      break;
  } // switch

  return result;
}

string msrVoice::voicePartRelativeIDAsString () const
{
  string result;
  
  switch (fVoicePartRelativeID) {
    case K_PART_MASTER_VOICE_NUMBER:
      result = "K_PART_MASTER_VOICE_NUMBER";
      break;
    case K_PART_HARMONY_VOICE_NUMBER:
      result = "K_PART_HARMONY_VOICE_NUMBER";
      break;
    case K_SILENT_VOICE_NUMBER:
      result = "K_SILENT_VOICE_NUMBER";
      break;
    default:
      result = to_string (fVoicePartRelativeID);
  } // switch
  
  return result;
}

string msrVoice::voiceStaffRelativeNumberAsString () const
{
  string result;
  
  switch (fVoiceStaffRelativeNumber) {
    case K_PART_MASTER_VOICE_NUMBER:
      result = "K_PART_MASTER_VOICE_NUMBER";
      break;
    case K_PART_HARMONY_VOICE_NUMBER:
      result = "K_PART_HARMONY_VOICE_NUMBER";
      break;
    case K_SILENT_VOICE_NUMBER:
      result = "K_SILENT_VOICE_NUMBER";
      break;
    default:
      result = to_string (fVoiceStaffRelativeNumber);
  } // switch
  
  return result;
}
         
string msrVoice::voiceFinalizationStatusAsString (
  msrVoiceFinalizationStatus voiceFinalizationStatus)
{
  string result;
  
  switch (voiceFinalizationStatus) {
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
    endl;

  idtr++;

  os <<
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
      fVoiceStanzasMap.size(), "stanza", "stanzas") <<
    ")" <<
    endl;

  const int fieldWidth = 28;

  os <<
    setw (fieldWidth) << "VoiceStaffUplink" << " : " <<
    fVoiceStaffUplink->getStaffName () <<
    endl <<
    setw (fieldWidth) << "VoiceAbsoluteNumber" << " : " <<
    fVoiceAbsoluteNumber <<
    endl <<
    setw (fieldWidth) << "VoicePartRelativeID" << " : " <<
    voicePartRelativeIDAsString () <<
    endl <<
    setw (fieldWidth) << "VoiceStaffRelativeNumber" << " : " <<
    voiceStaffRelativeNumberAsString () <<
    endl <<
    setw (fieldWidth) << "MusicHasBeenInsertedInVoice" << " : " <<
    booleanAsString (fMusicHasBeenInsertedInVoice) <<
    endl <<
    setw (fieldWidth) << "VoiceContainsMultipleRests" << " : " <<
    booleanAsString (fVoiceContainsMultipleRests) <<
    endl;

  os << endl;
  
  // print the voice initial repeats and segments
  int initialRepeatsAndSegmentsNumber =
    fVoiceInitialRepeatsAndSegments.size ();
    
  os <<
    "Initial repeats and segments: ";
  if (initialRepeatsAndSegmentsNumber)
    os << "(" << initialRepeatsAndSegmentsNumber << ")";
  else
    os << "none";
  os <<
    endl;
    
  if (initialRepeatsAndSegmentsNumber) {
    idtr++;

    list<S_msrElement>::const_iterator
      iBegin = fVoiceInitialRepeatsAndSegments.begin(),
      iEnd   = fVoiceInitialRepeatsAndSegments.end(),
      i      = iBegin;
      
    for ( ; ; ) {
      // print the element
      os << (*i);
      if (++i == iEnd) break;
      os << endl;
    } // for

    idtr--;
  }
  os << endl;

  /* JMI
  // sanity check
  msrAssert (
    fVoiceLastSegment != 0,
    "fVoiceLastSegment is null");
    */

  // print the last segment
  if (fVoiceLastSegment) {
    os <<
      "Voice last segment:" <<
      endl;
      
    idtr++;
    os <<
      fVoiceLastSegment <<
      endl;
    idtr--;
  }
  else {
    os <<
      "*** Last segment is null ***" << // JMI
      endl;
  }
  
  if (gGeneralOptions->fTraceLyrics) {
    // print the mute stanza
    os <<
      fVoiceMuteStanza <<
      endl;    
  }
  
  if (gMsrOptions->fShowMsrStanzas) {
    // print the voice mute stanza
    os <<
      fVoiceMuteStanza;
    
    // print the stanzas
    if (fVoiceStanzasMap.size()) {
      map<string, S_msrStanza>::const_iterator
        iBegin = fVoiceStanzasMap.begin(),
        iEnd   = fVoiceStanzasMap.end(),
        i      = iBegin;
        
      for ( ; ; ) {
        os << (*i).second;
        if (++i == iEnd) break;
        os << endl;
      } // for
    }
  }
  
  idtr--;
}

//______________________________________________________________________________
S_msrStaffLinesNumber msrStaffLinesNumber::create (
  int inputLineNumber,
  int linesNumber)
{
  msrStaffLinesNumber* o =
    new msrStaffLinesNumber (
      inputLineNumber, linesNumber);
  assert(o!=0);
  return o;
}

msrStaffLinesNumber::msrStaffLinesNumber (
  int inputLineNumber,
  int linesNumber)
    : msrElement (inputLineNumber)
{
  fLinesNumber = linesNumber;
}

msrStaffLinesNumber::~msrStaffLinesNumber()
{}

S_msrStaffLinesNumber msrStaffLinesNumber::createStaffLinesNumberNewbornClone ()
{
 if (gGeneralOptions->fTraceStaffTuning) {
    gLogIos <<
      "==> Creating a newborn clone of staff lines number '" <<
      staffLinesNumberAsString () <<
      "'" <<
      endl;
  }

 S_msrStaffLinesNumber
    newbornClone =
      msrStaffLinesNumber::create (
        fInputLineNumber,
        fLinesNumber);
  
  return newbornClone;
}

void msrStaffLinesNumber::acceptIn (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors)
    gLogIos <<
      "% ==> msrStaffLinesNumber::acceptIn()" <<
      endl;
      
  if (visitor<S_msrStaffLinesNumber>*
    p =
      dynamic_cast<visitor<S_msrStaffLinesNumber>*> (v)) {
        S_msrStaffLinesNumber elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors)
          gLogIos <<
            "% ==> Launching msrStaffLinesNumber::visitStart()" <<
             endl;
        p->visitStart (elem);
  }
}

void msrStaffLinesNumber::acceptOut (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors)
    gLogIos <<
      "% ==> msrStaffLinesNumber::acceptOut()" <<
      endl;

  if (visitor<S_msrStaffLinesNumber>*
    p =
      dynamic_cast<visitor<S_msrStaffLinesNumber>*> (v)) {
        S_msrStaffLinesNumber elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors)
          gLogIos <<
            "% ==> Launching msrStaffLinesNumber::visitEnd()" <<
            endl;
        p->visitEnd (elem);
  }
}

void msrStaffLinesNumber::browseData (basevisitor* v)
{}

string msrStaffLinesNumber::staffLinesNumberAsString () const
{
  stringstream s;

  s <<
    "StaffLinesNumber" <<
    ", line " << fInputLineNumber <<
    ", " <<
    "linesNumber: " << fLinesNumber;
    
  return s.str ();
}

ostream& operator<< (ostream& os, const S_msrStaffLinesNumber& elt)
{
  elt->print (os);
  return os;
}

void msrStaffLinesNumber::print (ostream& os)
{
  os <<
    staffLinesNumberAsString () <<
    endl;
}

//______________________________________________________________________________
S_msrStaffTuning msrStaffTuning::create (
  int                  inputLineNumber,
  int                  staffTuningLineNumber,
  msrQuarterTonesPitch quarterTonesPitch,
  int                  staffTuningOctave)
{
  msrStaffTuning* o =
    new msrStaffTuning (
      inputLineNumber,
      staffTuningLineNumber,
      quarterTonesPitch,
      staffTuningOctave);
  assert(o!=0);
  return o;
}

msrStaffTuning::msrStaffTuning (
  int                  inputLineNumber,
  int                  staffTuningLineNumber,
  msrQuarterTonesPitch quarterTonesPitch,
  int                  staffTuningOctave)
    : msrElement (inputLineNumber)
{
  fStaffTuningLineNumber        = staffTuningLineNumber;
  fStaffTuningQuarterTonesPitch = quarterTonesPitch;
  fStaffTuningOctave            = staffTuningOctave;
}

msrStaffTuning::~ msrStaffTuning ()
{}

S_msrStaffTuning msrStaffTuning::createStaffTuningNewbornClone ()
{
  if (gGeneralOptions->fTraceStaffTuning) {
    gLogIos <<
      "==> Creating a newborn clone of staff tuning '" <<
      staffTuningAsString () <<
      "'" <<
      endl;
  }

 S_msrStaffTuning
    newbornClone =
      msrStaffTuning::create (
        fInputLineNumber,
        fStaffTuningLineNumber,
        fStaffTuningQuarterTonesPitch,
        fStaffTuningOctave);
  
  return newbornClone;
}

void msrStaffTuning::acceptIn (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIos <<
      "% ==> msrStaffTuning::acceptIn()" <<
      endl;
  }
      
  if (visitor<S_msrStaffTuning>*
    p =
      dynamic_cast<visitor<S_msrStaffTuning>*> (v)) {
        S_msrStaffTuning elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors)
          gLogIos <<
            "% ==> Launching msrStaffTuning::visitStart()" <<
             endl;
        p->visitStart (elem);
  }
}

void msrStaffTuning::acceptOut (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors)
    gLogIos <<
      "% ==> msrStaffTuning::acceptOut()" <<
      endl;

  if (visitor<S_msrStaffTuning>*
    p =
      dynamic_cast<visitor<S_msrStaffTuning>*> (v)) {
        S_msrStaffTuning elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors)
          gLogIos <<
            "% ==> Launching msrStaffTuning::visitEnd()" <<
            endl;
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

string msrStaffTuning::staffTuningAsString () const
{
  stringstream s;

  s <<
    "StaffTuning" <<
    ", line " << fStaffTuningLineNumber <<
    ", " <<
    msrQuarterTonesPitchAsString (
      gMsrOptions->fMsrQuarterTonesPitchesLanguage,
      fStaffTuningQuarterTonesPitch) <<
    ", octave " << fStaffTuningOctave;
    
  return s.str ();
}

void msrStaffTuning::print (ostream& os)
{
  os <<
    "StaffTuning" <<
    ", line " << fStaffTuningLineNumber <<
    ", " <<
    endl;

  idtr++;

  const int fieldWidth = 29;

  os << left <<  
    setw (fieldWidth) <<
    "StaffTuningLineNumber" << " = " <<
    fStaffTuningLineNumber <<
    endl <<
    setw (fieldWidth) <<
    "fStaffTuningQuarterTonesPitch" << " = " <<
    msrQuarterTonesPitchAsString (
      gMsrOptions->fMsrQuarterTonesPitchesLanguage,
      fStaffTuningQuarterTonesPitch) <<
    endl <<
    setw (fieldWidth) <<
    "StaffTuningOctave" << " = " <<
    fStaffTuningOctave <<
    endl;

  idtr--;
}

//______________________________________________________________________________
S_msrStaffDetails msrStaffDetails::create (
  int                   inputLineNumber,
  msrStaffTypeKind      staffTypeKind,
  S_msrStaffLinesNumber staffLinesNumber,
  S_msrStaffTuning      staffTuning,
  msrShowFretsKind      showFretsKind,
  msrPrintObjectKind    printObjectKind,
  msrPrintSpacingKind   printSpacingKind)
{
  msrStaffDetails* o =
    new msrStaffDetails (
      inputLineNumber,
      staffTypeKind,
      staffLinesNumber,
      staffTuning,
      showFretsKind,
      printObjectKind,
      printSpacingKind);
  assert(o!=0);
  return o;
}

msrStaffDetails::msrStaffDetails (
  int                   inputLineNumber,
  msrStaffTypeKind      staffTypeKind,
  S_msrStaffLinesNumber staffLinesNumber,
  S_msrStaffTuning      staffTuning,
  msrShowFretsKind      showFretsKind,
  msrPrintObjectKind    printObjectKind,
  msrPrintSpacingKind   printSpacingKind)
    : msrElement (inputLineNumber)
{
  fStaffTypeKind    = staffTypeKind;
  fStaffLinesNumber = staffLinesNumber;
  fStaffTuning      = staffTuning;
  fShowFretsKind    = showFretsKind;
  fPrintObjectKind  = printObjectKind;
  fPrintSpacingKind = printSpacingKind;
}

msrStaffDetails::~msrStaffDetails()
{}

S_msrStaffDetails msrStaffDetails::createStaffDetailsNewbornClone (
  S_msrStaffLinesNumber staffLinesNumberClone,
  S_msrStaffTuning      staffTuningClone)
{
  if (gGeneralOptions->fTraceStaves) {
    gLogIos <<
      "==> Creating a newborn clone of staff details \"" <<
      staffDetailsAsShortString () <<
      "\"" <<
      endl;
  }
      
  S_msrStaffDetails
    newbornClone =
      msrStaffDetails::create (
        fInputLineNumber,
        fStaffTypeKind,
        staffLinesNumberClone,
        staffTuningClone,
        fShowFretsKind,
        fPrintObjectKind,
        fPrintSpacingKind);

  return newbornClone;
}

void msrStaffDetails::acceptIn (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors)
    gLogIos <<
      "% ==> msrStaffDetails::acceptIn()" <<
      endl;
      
  if (visitor<S_msrStaffDetails>*
    p =
      dynamic_cast<visitor<S_msrStaffDetails>*> (v)) {
        S_msrStaffDetails elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors)
          gLogIos <<
            "% ==> Launching msrStaffDetails::visitStart()" <<
             endl;
        p->visitStart (elem);
  }
}

void msrStaffDetails::acceptOut (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors)
    gLogIos <<
      "% ==> msrStaffDetails::acceptOut()" <<
      endl;

  if (visitor<S_msrStaffDetails>*
    p =
      dynamic_cast<visitor<S_msrStaffDetails>*> (v)) {
        S_msrStaffDetails elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors)
          gLogIos <<
            "% ==> Launching msrStaffDetails::visitEnd()" <<
            endl;
        p->visitEnd (elem);
  }
}

void msrStaffDetails::browseData (basevisitor* v)
{
  if (fStaffLinesNumber) {
    // browse the staff lines number
    msrBrowser<msrStaffLinesNumber> browser (v);
    browser.browse (*fStaffLinesNumber);
  }

  if (fStaffTuning) {
    // browse the staff tuning
    msrBrowser<msrStaffTuning> browser (v);
    browser.browse (*fStaffTuning);
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
      
string msrStaffDetails::printObjectKindKindAsString (
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
      
string msrStaffDetails::printSpacingKindKindAsString (
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

string msrStaffDetails::staffDetailsAsShortString ()
{
  stringstream s;

  s <<
    "StaffDetails" <<
    ", line " << fInputLineNumber;

  // print the staff lines number if any
  s << ", StaffLinesNumber: ";
  if (fStaffLinesNumber)
    s <<
      fStaffLinesNumber->getLinesNumber ();
  else
    s <<
      "none";

  // print the staff tuning if any
  s << ", StaffTuning: ";
  if (fStaffTuning)
    s <<
      "present";
  else
    s <<
      "none";

  return s.str ();
}

void msrStaffDetails::print (ostream& os)
{
  os <<
    "StaffDetails:" <<
    endl;

  idtr++;

  // print the staff lines number if any
  if (fStaffLinesNumber)
    os <<
      fStaffLinesNumber;
  else
    os <<
      "StaffLinesNumber: none" <<
      endl;

  // print the staff tuning if any
  if (fStaffTuning)
    os <<
      fStaffTuning;
  else
    os <<
      "StaffTuning: none" <<
      endl;

  idtr--;
}

//______________________________________________________________________________
int msrStaff::gMaxStaffVoices = 4;

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
    staffPartUplink != 0,
    "staffPartUplink is null");

  // set staff part uplink
  fStaffPartUplink =
    staffPartUplink;

  // set staff kind and number
  fStaffKind = staffKind;
  fStaffNumber = staffNumber;

  // do other initializations
  initializeStaff ();
}

void msrStaff::initializeStaff ()
{
  fStaffRegisteredVoicesCounter = 0;

  // set staff name
  switch (fStaffKind) {
    case msrStaff::kMasterStaff:
      fStaffName =
        fStaffPartUplink->getPartMsrName () +
        "_MASTER_Staff";
      break;
      
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
      
    case msrStaff::kPercussionStaff:
        fStaffPartUplink->getPartMsrName () +
        "_Percussion_" +
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
  } // switch

  if (gGeneralOptions->fTraceStaves)
    gLogIos <<
      "==> Initializing staff \"" << fStaffName <<
      "\" in part " <<
      fStaffPartUplink->getPartCombinedName () <<
      endl;

  // check the staff number
  switch (fStaffKind) {
    case msrStaff::kMasterStaff:
      // DON'T create any staff silent voice
      break;
      
    case msrStaff::kRegularStaff:
      // the staff number should not be negative
      // (K_SILENT_VOICE_NUMBER is used for the hidden staff
      // containing the staves silent voices)
      if (fStaffNumber < 0) {
        stringstream s;
    
        s <<
          "regular staff number " << fStaffNumber <<
          " is not positive";
          
        msrAssert (false, s.str ());
      }

      // create the staff silent voice
      createStaffSilentVoice (
        fInputLineNumber);
      break;
      
    case msrStaff::kTablatureStaff:
      // create the staff silent voice
      createStaffSilentVoice (
        fInputLineNumber);
      break;
      
    case msrStaff::kPercussionStaff:
      // create the staff silent voice
      createStaffSilentVoice (
        fInputLineNumber);
      break;
      
    case msrStaff::kHarmonyStaff:
      if (fStaffNumber != K_PART_HARMONY_STAFF_NUMBER) {
        stringstream s;
    
        s <<
          "harmony staff number " << fStaffNumber <<
          " is not equal to " << K_PART_HARMONY_STAFF_NUMBER;
          
        msrInternalError (
          fInputLineNumber, s.str ());
      }
      
    // JMI ???  // dont't create any staff silent voice for a harmony staff
      // create the staff silent voice
      createStaffSilentVoice (
        fInputLineNumber);
      break;
      
    case msrStaff::kFiguredBassStaff:
      if (fStaffNumber != K_PART_FIGURED_BASS_STAFF_NUMBER) {
        stringstream s;
    
        s <<
          "figured bass staff number " << fStaffNumber <<
          " is not equal to " << K_PART_FIGURED_BASS_STAFF_NUMBER;
          
        msrInternalError (
          fInputLineNumber, s.str ());
      }
      
    // JMI ???  // dont't create any staff silent voice for a harmony staff
      // create the staff silent voice
      createStaffSilentVoice (
        fInputLineNumber);
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
      if (gGeneralOptions->fTraceClefs || gGeneralOptions->fTraceStaves) {
        gLogIos <<
          "Appending part clef '" << clef->clefAsString () <<
          "' as initial clef to staff \"" <<
          getStaffName () <<
          "\" in part " <<
          fStaffPartUplink->getPartCombinedName () <<
          endl;
      }

      appendClefToStaff (clef);
    }
    /* JMI
    else {
      if (gGeneralOptions->fTraceStaves)
        gLogIos <<
          "Appending default treble clef " <<
          " to staff \"" <<
          getStaffName () <<
          "\" in part " <<
          fStaffPartUplink->getPartCombinedName () <<
          endl;

      // create the implicit initial G line 2 clef
      appendClefToStaff (
        msrClef::create (
          fInputLineNumber,
          msrClef::kTrebleClef));
    }
    * */
  }
    
  // get the initial key from the part if any
  {
    //* JMI
    S_msrKey
      key =
        fStaffPartUplink->
          getPartCurrentKey ();
  
    if (key) {
      if (gGeneralOptions->fTraceStaves || gGeneralOptions->fTraceKeys) {
        gLogIos <<
          "Appending part key '" << key->keyAsString () <<
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
      if (gGeneralOptions->fTraceStaves)
        gLogIos <<
          "Appending default C major key " <<
          " to staff \"" <<
          getStaffName () <<
          "\" in part " <<
          fStaffPartUplink->getPartCombinedName () <<
          endl;
          
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

  // DON'T get the staff initial time from the part,
  // it has already been copied during the deep copy of the part master staff
  
  // get the initial time from the part if any
  {
    /* JMI
    S_msrTime
      time =
        fStaffPartUplink->
          getPartCurrentTime ();

    if (time) {
      if (gGeneralOptions->fTraceStaves || gGeneralOptions->fTraceTimes) {
        gLogIos <<
          "Appending part time '" << time->timeAsString () <<
          "' as initial time to staff \"" <<
          getStaffName () <<
          "\" in part " <<
          fStaffPartUplink->getPartCombinedName () <<
          endl;
      }

      appendTimeToStaff (time);
    }
    */
    
    /* JMI
    else {
      // time is crucially needed for measures management,
      // we cannot stay without any

      if (gGeneralOptions->fTraceStaves || gGeneralOptions->fTraceTimes)
        gLogIos <<
          "Appending default 4/4 time " <<
          " to staff \"" <<
          getStaffName () <<
          "\" in part " <<
          fStaffPartUplink->getPartCombinedName () <<
          endl;
          
      // append the implicit initial 4/4 time signature
      appendTimeToStaff (
        msrTime::createFourQuartersTime (
          fInputLineNumber));
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
      if (gGeneralOptions->fTraceStaves /* JMI || gGeneralOptions->fTraceTransposes */) {
        gLogIos <<
          "Appending part transpose '" << transpose->transposeAsString () <<
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

void msrStaff::createStaffSilentVoice (
  int inputLineNumber)
{
  // create the staff silent voice  
  if (gGeneralOptions->fTraceStaves || gGeneralOptions->fTraceVoices) {
    gLogIos <<
      "==> Creating the silent voice for staff \"" <<
      getStaffName () <<
      "\", line " << inputLineNumber <<
      endl;
  }

/* JMI
  gLogIos <<
    endl <<
    "*****************" <<
    endl <<
    "msrStaff::createStaffSilentVoice, this =" <<
    endl <<
    idtr;
  print (gLogIos);
  gLogIos <<
    "*****************" <<
    endl <<
    endl;
    
  gLogIos <<
    endl <<
    "*****************" <<
    endl <<
    "fStaffPartUplink->getPartHarmonyVoice () = " <<
    endl <<
    fStaffPartUplink->
      getPartHarmonyVoice () <<
    "*****************" <<
    endl <<
    endl;
*/

  // create a deep copy of the part master voice
  fStaffSilentVoice =
    fStaffPartUplink->
      getPartMasterVoice ()->
        createVoiceDeepCopy (
          inputLineNumber,
          msrVoice::kSilentVoice,
          K_SILENT_VOICE_NUMBER,
          this);

if (true) // JMI
  registerVoiceInStaff (
    inputLineNumber,
    fStaffSilentVoice );
else
  // register is by its relative number
  fStaffAllVoicesMap [fStaffRegisteredVoicesCounter] =
    fStaffSilentVoice;

/* JMI
  // register it by its part-relative ID
  fStaffVoiceRelativeNumberToVoiceMap [voice->getVoicePartRelativeID ()] =
    fStaffSilentVoice;
    */

/* JMI
  gLogIos <<
    endl <<
    "**********" <<
    endl <<
    idtr;
  print (gLogIos);
  gLogIos <<
    endl <<
    "**********" <<
    endl <<
    endl;
    */
}

S_msrStaff msrStaff::createStaffNewbornClone (
  S_msrPart containingPart)
{
  if (gGeneralOptions->fTraceStaves) {
    gLogIos <<
      "==> Creating a newborn clone of staff \"" <<
      fStaffName <<
      "\"" <<
      endl;
  }
  
  // sanity check
  msrAssert(
    containingPart != 0,
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

string msrStaff::staffNumberAsString () const
{
  string result;
  
  switch (fStaffNumber) {
    case K_PART_MASTER_STAFF_NUMBER:
      result = "K_PART_MASTER_STAFF_NUMBER";
      break;
    case K_PART_HARMONY_STAFF_NUMBER:
      result = "K_PART_HARMONY_STAFF_NUMBER";
      break;
    case K_PART_FIGURED_BASS_STAFF_NUMBER:
      result = "K_PART_FIGURED_BASS_STAFF_NUMBER";
      break;
    default:
      result = to_string (fStaffNumber);
  } // switch
  
  return result;
}

const int msrStaff::getStaffNumberOfMusicVoices () const
{
  int result = 0;

  for (
    map<int, S_msrVoice>::const_iterator i =
      fStaffVoiceRelativeNumberToVoiceMap.begin();
    i != fStaffVoiceRelativeNumberToVoiceMap.end();
    i++) {
      S_msrVoice
        voice =
          (*i).second;

      switch (voice->getVoiceKind ()) {
        case msrVoice::kMasterVoice:
          if (voice->getMusicHasBeenInsertedInVoice ())
            result++;
          break;
          
        case msrVoice::kRegularVoice:
          if (voice->getMusicHasBeenInsertedInVoice ())
            result++;
          break;
          
        case msrVoice::kHarmonyVoice: // JMI
          break;
          
        case msrVoice::kFiguredBassVoice: // JMI
          break;
          
        case msrVoice::kSilentVoice: // JMI
          break;
      } // switch
      
  } // for

  return result;
}

void msrStaff::createMeasureAndAppendItToStaff (
  int    inputLineNumber,
  string measureNumber)
{
  if (gGeneralOptions->fTraceMeasures || gGeneralOptions->fTraceStaves) {
    gLogIos <<
      "==> Creating and appending measure '" <<
      measureNumber <<
      "', line " << inputLineNumber <<
      ", in staff \"" << getStaffName () << "\"" <<
      endl;
  }

  // propagate it to all staves
  for (
    map<int, S_msrVoice>::const_iterator i = fStaffAllVoicesMap.begin();
    i != fStaffAllVoicesMap.end();
    i++) {
    S_msrVoice voice = (*i).second;

    // sanity check
    msrAssert (
      voice != 0,
      "voice is null");
    
    if (gGeneralOptions->fTraceMeasures || gGeneralOptions->fTraceStaves) {
      gLogIos <<
        "==> Propagating the creation and appending of measure '" <<
        measureNumber <<
        "', line " << inputLineNumber <<
        ", to voice \"" << voice->getVoiceName () << "\"" <<
        endl;
    }

    voice->
      createMeasureAndAppendItToVoice (
        inputLineNumber,
        measureNumber);
  } // for
}

S_msrVoice msrStaff::createVoiceInStaffByItsPartRelativeID (
  int          inputLineNumber,
  msrVoice::msrVoiceKind
               voiceKind,
  int          voicePartRelativeID,
  string       currentMeasureNumber)
{
  // take this new voice into account
  fStaffRegisteredVoicesCounter++;

  if (gGeneralOptions->fTraceStaves || gGeneralOptions->fTraceVoices) {
    gLogIos <<
      "==> Creating voice with part-relative ID '" <<
      voicePartRelativeID <<
      "' as relative voice '" <<
      fStaffRegisteredVoicesCounter <<
      "' of staff \"" << getStaffName () <<
      "\", line " << inputLineNumber <<
      "\", current measure number: " <<
      currentMeasureNumber <<
 // JMI     " in part " << fStaffPartUplink->getPartCombinedName () <<
      endl;
  }

  // are there too many voices in this staff? 
  if (fStaffRegisteredVoicesCounter > msrStaff::gMaxStaffVoices) {
    stringstream s;
    
    s <<
      "staff \"" << getStaffName () <<
      "\" is already filled up with " <<
      msrStaff::gMaxStaffVoices << " voices" <<
      endl <<
      "the voice with part-relative ID " <<
      voicePartRelativeID <<
      " overflows it" <<
      endl <<
      ", fStaffRegisteredVoicesCounter = " <<
      fStaffRegisteredVoicesCounter <<
      ", msrStaff::gMaxStaffVoices = " <<
      msrStaff::gMaxStaffVoices <<
      endl;
      
    msrMusicXMLError (
// JMI    msrMusicXMLWarning ( JMI
      inputLineNumber,
      s.str ());
  }

  // create the voice as a deep copy of the silent voice
  S_msrVoice
    voice =
      fStaffSilentVoice->
        createVoiceDeepCopy (
          inputLineNumber,
          voiceKind,
          voicePartRelativeID,
          this);

  // change its identity
  voice->
    changeVoiceIdentity (
      voicePartRelativeID);

  // get the part current time JMI???
  S_msrTime
    time =
      fStaffPartUplink->
        getPartCurrentTime ();

  if (! time) {
    // create default 4/4 time
    time =
      msrTime::createFourQuartersTime (
        inputLineNumber);

    // register it in the part for future use
    fStaffPartUplink->
      setPartCurrentTime (time);
  }
      
  // append the time to this staff
  appendTimeToStaff (time);

  // set fMeasureFullMeasureLength accordingly JMI
 // setMeasureFullMeasureLengthFromTime (
 //   partCurrentTime);
          
  // register the voice by its relative number
  if (gGeneralOptions->fTraceVoices)
    gLogIos <<
      "Voice " << voicePartRelativeID <<
      " in staff " << getStaffName () <<
      " gets staff relative number " <<
      fStaffRegisteredVoicesCounter <<
      endl;
    
  fStaffVoiceRelativeNumberToVoiceMap [fStaffRegisteredVoicesCounter] =
    voice;

  // register is by its part-relative ID
  fStaffAllVoicesMap [fStaffRegisteredVoicesCounter] =
    voice;

  // add initial measures with skip notes up to currentMeasureNumber
  // in case this voice does not start at the beginning of the part
  
  return voice;
}

S_msrVoice msrStaff::fetchVoiceFromStaffByItsPartRelativeID (
  int inputLineNumber,
  int voicePartRelativeID)
{
  S_msrVoice result; // JMI avoid repetivite messages!

  if (gGeneralOptions->fTraceVoices && gGeneralOptions->fTraceStaves)
    gLogIos <<
      "Fetching part-relative voice ID " <<
      voicePartRelativeID <<
     " in staff \"" << getStaffName () <<
      "\", line " << inputLineNumber <<
      " in part " <<
      fStaffPartUplink->getPartCombinedName () <<
      endl;

  for (
    map<int, S_msrVoice>::const_iterator i =
      fStaffVoiceRelativeNumberToVoiceMap.begin();
    i != fStaffVoiceRelativeNumberToVoiceMap.end();
    i++) {
    if (
      (*i).second->getVoicePartRelativeID ()
        ==
      voicePartRelativeID) {
      if (gGeneralOptions->fTraceVoices) {
        gLogIos <<
          "Voice " << voicePartRelativeID <<
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
  int inputLineNumber, S_msrVoice voice)
{
  // sanity check
  msrAssert (
    voice != 0,
    "voice is null");
    
  // take this new voice into account
  fStaffRegisteredVoicesCounter++;

  // are there too many voices in this staff? 
  if (fStaffRegisteredVoicesCounter > msrStaff::gMaxStaffVoices) {
    stringstream s;
    
    s <<
      "staff \"" << getStaffName () <<
      "\" is already filled up with " <<
      msrStaff::gMaxStaffVoices << " voices," <<
      endl <<
      "the voice named \"" << voice->getVoiceName () << "\" overflows it" <<
      endl <<
      ", fStaffRegisteredVoicesCounter = " << fStaffRegisteredVoicesCounter <<
      ", msrStaff::gMaxStaffVoices = " << msrStaff::gMaxStaffVoices <<
      endl;
      
    msrMusicXMLError (
// JMI    msrMusicXMLWarning ( JMI
      inputLineNumber,
      s.str ());
  }

  // register voice in this staff
  if (gGeneralOptions->fTraceStaves || gGeneralOptions->fTraceVoices)
    gLogIos <<
      "Registering voice \"" << voice->getVoiceName () <<
      "\" as relative voice " << fStaffRegisteredVoicesCounter <<
      " of staff \"" << getStaffName () <<
      "\", line " << inputLineNumber <<
// JMI       " in part " << fStaffPartUplink->getPartCombinedName () <<
      endl;

  // register is by its relative number
  fStaffAllVoicesMap [fStaffRegisteredVoicesCounter] =
    voice;

  // register it by its part-relative ID
  fStaffVoiceRelativeNumberToVoiceMap [voice->getVoicePartRelativeID ()] =
    voice;
}

void msrStaff::bringStaffToMeasureLength (
  int      inputLineNumber,
  rational measureLength)
{
  if (gGeneralOptions->fTraceStaves || gGeneralOptions->fTraceMeasures) {
    gLogIos <<
      "Bringing measure length for staff \"" <<
      getStaffName () <<
      "\" to " << measureLength <<
      ", line " << inputLineNumber <<
      endl;
  }

  for (
    map<int, S_msrVoice>::const_iterator i = fStaffAllVoicesMap.begin();
    i != fStaffAllVoicesMap.end();
    i++) {
    (*i).second-> // JMI msrAssert???
      bringVoiceToMeasureLength (
        inputLineNumber,
        measureLength);
  } // for
}

void msrStaff::appendClefToStaff (S_msrClef clef)
{
  if (gGeneralOptions->fTraceClefs || gGeneralOptions->fTraceStaves) {
    gLogIos <<
      "Appending clef '" << clef->clefAsString () <<
      "' to staff \"" <<
      getStaffName () <<
      "\" in part " <<
      fStaffPartUplink->getPartCombinedName () <<
      endl;
  }

  // set staff clef
  fStaffCurrentClef = clef;

  // is this a tablature or percussion staff?
  switch (fStaffKind) {
    case msrStaff::kMasterStaff: // JMI
      break;
      
    case msrStaff::kRegularStaff: // JMI
    /* JMI
      if (clef->clefIsATablatureClef ())
        fStaffKind = kTablatureStaff;
      else if (clef->clefIsAPercussionClef ())
        fStaffKind = kPercussionStaff;
        */
      break;
      
    case msrStaff::kTablatureStaff:
      break;
      
    case msrStaff::kPercussionStaff:
      break;
      
    case msrStaff::kHarmonyStaff:
      break;
      
    case msrStaff::kFiguredBassStaff:
      break;
  } // switch
  
  // propagate clef to all voices
  for (
    map<int, S_msrVoice>::const_iterator i = fStaffAllVoicesMap.begin();
    i != fStaffAllVoicesMap.end();
    i++) {
    (*i).second-> // JMI msrAssert???
      appendClefToVoice (clef);
  } // for
}

void msrStaff::appendKeyToStaff (S_msrKey  key)
{
  if (gGeneralOptions->fTraceKeys || gGeneralOptions->fTraceStaves) {
    gLogIos <<
      "Appending key '" << key->keyAsString () <<
      "' to staff \"" <<
      getStaffName () <<
      "\" in part " <<
      fStaffPartUplink->getPartCombinedName () <<
      endl;
  }
  
  // set staff key
  fStaffCurrentKey = key;


  bool doAppendKeyToStaff;
  
  if (! fStaffCurrentKey) {
    doAppendKeyToStaff = true;
  }
  
  else {
    if (key->isEqualTo (fStaffCurrentKey)) {
      if (gGeneralOptions->fTraceTranspositions || gGeneralOptions->fTraceStaves) {
        gLogIos <<
          "Key '" <<
          key->keyAsString () <<
          "' ignored because it is already present in staff " <<
          getStaffName () <<
          "\" in part " <<
          fStaffPartUplink->getPartCombinedName () <<
          endl;
      }

      doAppendKeyToStaff = false;
    }
  }
  
  if (doAppendKeyToStaff) {
    // register key as current staff key
    fStaffCurrentKey = key;
  
    // propagate it to all voices
    for (
      map<int, S_msrVoice>::const_iterator i = fStaffAllVoicesMap.begin();
      i != fStaffAllVoicesMap.end();
      i++) {
      (*i).second-> // JMI msrAssert???
        appendKeyToVoice (key);
    } // for
  }
}

void msrStaff::appendTimeToStaff (S_msrTime time)
{
  if (gGeneralOptions->fTraceTimes || gGeneralOptions->fTraceStaves) {
    gLogIos <<
      "Appending time '" << time->timeAsString () <<
      "' to staff \"" <<
      getStaffName () <<
      "\" in part " <<
      fStaffPartUplink->getPartCombinedName () <<
      endl;
  }
  
  // set staff time
  if (time != fStaffCurrentTime) { // JMI
    fStaffCurrentTime = time;

    // propagate it to all voices
    for (
      map<int, S_msrVoice>::const_iterator i = fStaffAllVoicesMap.begin();
      i != fStaffAllVoicesMap.end();
      i++) {
      (*i).second-> // JMI msrAssert???
        appendTimeToVoice (time);
    } // for
  }
}    

void msrStaff::appendTimeToStaffClone (S_msrTime time)
{
  if (gGeneralOptions->fTraceTimes || gGeneralOptions->fTraceStaves) {
    gLogIos <<
      "Appending time '" << time->timeAsString () <<
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
    map<int, S_msrVoice>::const_iterator i = fStaffAllVoicesMap.begin();
    i != fStaffAllVoicesMap.end();
    i++) {
    (*i).second-> // JMI msrAssert???
      appendTimeToVoiceClone (time);
  } // for
}    

void msrStaff::createRepeatAndAppendItToStaff (int inputLineNumber)
{
  if (gGeneralOptions->fTraceRepeats)
    gLogIos <<
      "==> Creating and appending a repeat to staff \"" <<
      getStaffName () <<
      "\" in part " <<
      fStaffPartUplink->getPartCombinedName () <<
      endl;

  for (
    map<int, S_msrVoice>::const_iterator i = fStaffAllVoicesMap.begin();
    i != fStaffAllVoicesMap.end();
    i++) {
    (*i).second->
      createRepeatAndAppendItToVoice (inputLineNumber);
  } // for
}

void msrStaff::appendRepeatEndingToStaff (
  int       inputLineNumber,
  string    repeatEndingNumber, // may be "1, 2"
  msrRepeatEnding::msrRepeatEndingKind
            repeatEndingKind)
{
  if (gGeneralOptions->fTraceRepeats)
    gLogIos <<
      "Appending repeat ending to staff \"" <<
      getStaffName () <<
      "\" in part " <<
      fStaffPartUplink->getPartCombinedName () <<
      endl;

  for (
    map<int, S_msrVoice>::const_iterator i = fStaffAllVoicesMap.begin();
    i != fStaffAllVoicesMap.end();
    i++) {
    (*i).second->
      appendRepeatEndingToVoice (
        inputLineNumber,
        repeatEndingNumber,
        repeatEndingKind);
  } // for
}

void msrStaff::createMeasureRepeatFromItsFirstMeasureInStaff (
  int inputLineNumber,
  int measureRepeatMeasuresNumber,
  int measureRepeatSlashes)
{
  if (gGeneralOptions->fTraceRepeats)
    gLogIos <<
      "==> Creating a measure repeat from it's first measure in staff \"" <<
      getStaffName () <<
      "\" in part " <<
      fStaffPartUplink->getPartCombinedName () <<
      endl;

  for (
    map<int, S_msrVoice>::const_iterator i = fStaffAllVoicesMap.begin();
    i != fStaffAllVoicesMap.end();
    i++) {
    (*i).second->
      createMeasureRepeatFromItsFirstMeasureInVoice (
        inputLineNumber,
        measureRepeatMeasuresNumber,
        measureRepeatSlashes);
  } // for
}

void msrStaff::appendPendingMeasureRepeatToStaff (
  int inputLineNumber)
{
  if (gGeneralOptions->fTraceRepeats)
    gLogIos <<
      "Appending the pending measure repeat to staff \"" <<
      getStaffName () <<
      "\" in part " <<
      fStaffPartUplink->getPartCombinedName () <<
      endl;

  for (
    map<int, S_msrVoice>::const_iterator i = fStaffAllVoicesMap.begin();
    i != fStaffAllVoicesMap.end();
    i++) {
    (*i).second->
      appendPendingMeasureRepeatToVoice (
        inputLineNumber);
  } // for
}

void msrStaff::createMultipleRestInStaff (
  int inputLineNumber,
  int multipleRestMeasuresNumber)
{
  if (gGeneralOptions->fTraceRepeats)
    gLogIos <<
      "==> Creating a multiple rest in staff \"" <<
      getStaffName () <<
      "\" in part " <<
      fStaffPartUplink->getPartCombinedName () <<
      ", " <<
      singularOrPlural (
        multipleRestMeasuresNumber, "measure", "measures") <<
      endl;

  for (
    map<int, S_msrVoice>::const_iterator i = fStaffAllVoicesMap.begin();
    i != fStaffAllVoicesMap.end();
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
  if (gGeneralOptions->fTraceRepeats)
    gLogIos <<
      "Appending the pending multiple rest to staff \"" <<
      getStaffName () <<
      "\" in part " <<
      fStaffPartUplink->getPartCombinedName () <<
      endl;

  for (
    map<int, S_msrVoice>::const_iterator i = fStaffAllVoicesMap.begin();
    i != fStaffAllVoicesMap.end();
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
  if (gGeneralOptions->fTraceRepeats)
    gLogIos <<
      "Appending multiple rest '" <<
      multipleRest->multipleRestAsString () <<
      "' to staff clone \"" <<
      getStaffName () <<
      "\"" <<
      endl;

  for (
    map<int, S_msrVoice>::const_iterator i = fStaffAllVoicesMap.begin();
    i != fStaffAllVoicesMap.end();
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
  if (gGeneralOptions->fTraceRepeats)
    gLogIos <<
      "Appending repeat clone to staff \"" <<
      getStaffName () <<
      "\" in part " <<
      fStaffPartUplink->getPartCombinedName () <<
      endl;

  for (
    map<int, S_msrVoice>::const_iterator i = fStaffAllVoicesMap.begin();
    i != fStaffAllVoicesMap.end();
    i++) {
    (*i).second-> // JMI msrAssert???
      appendRepeatCloneToVoice (
        inputLineNumber, repeatCLone);
  } // for
}

void msrStaff::appendRepeatEndingCloneToStaff (
  S_msrRepeatEnding repeatEndingClone)
{
  if (gGeneralOptions->fTraceRepeats)
    gLogIos <<
      "Appending repeat ending clone to staff \"" <<
      getStaffName () <<
      "\" in part " <<
      fStaffPartUplink->getPartCombinedName () <<
      endl;

  for (
    map<int, S_msrVoice>::const_iterator i = fStaffAllVoicesMap.begin();
    i != fStaffAllVoicesMap.end();
    i++) {
    (*i).second-> // JMI msrAssert???
      appendRepeatEndingCloneToVoice (repeatEndingClone);
  } // for
}

void msrStaff::appendBarlineToStaff (S_msrBarline barline)
{
  if (gGeneralOptions->fTraceMeasures)
    gLogIos <<
      "Appending barline '" << barline->barlineAsString () <<
      "' to staff " <<
      getStaffName () <<
      "\" in part " <<
      fStaffPartUplink->getPartCombinedName () <<
      endl;

  for (
    map<int, S_msrVoice>::const_iterator i = fStaffAllVoicesMap.begin();
    i != fStaffAllVoicesMap.end();
    i++) {
    (*i).second->
      appendBarlineToVoice (barline);
  } // for
}

void msrStaff::appendTransposeToStaff (S_msrTranspose transpose)
{
  if (gGeneralOptions->fTraceTranspositions || gGeneralOptions->fTraceStaves) {
    gLogIos <<
      "Setting transpose '" <<
      transpose->transposeAsString () <<
      "' in staff " <<
      getStaffName () <<
      "\" in part " <<
      fStaffPartUplink->getPartCombinedName () <<
      endl;
  }

  // set staff transpose
  bool doAppendTransposeToStaff;
  
  if (! fStaffCurrentTranspose) {
    doAppendTransposeToStaff = true;
  }
  
  else {
    if (transpose->isEqualTo (fStaffCurrentTranspose)) {
      if (gGeneralOptions->fTraceTranspositions || gGeneralOptions->fTraceStaves) {
        gLogIos <<
          "Transpose '" <<
          transpose->transposeAsString () <<
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

void msrStaff::appendStaffDetailsToStaff (
  S_msrStaffDetails staffDetails)
{
  if (gGeneralOptions->fTraceStaves)
    gLogIos <<
      "Appending staff details '" <<
      staffDetails->staffDetailsAsShortString () <<
      "' to staff \"" <<
      getStaffName () <<
      "\" in part " <<
      fStaffPartUplink->getPartCombinedName () <<
      endl;

  // register staff details in staff
  fCurrentStaffStaffDetails = staffDetails;
  
  for (
    map<int, S_msrVoice>::const_iterator i = fStaffAllVoicesMap.begin();
    i != fStaffAllVoicesMap.end();
    i++) {
    (*i).second->
      appendStaffDetailsToVoice (staffDetails);
  } // for
}

void msrStaff::appendTransposeToAllStaffVoices (
  S_msrTranspose transpose)
{
  for (
    map<int, S_msrVoice>::const_iterator i = fStaffAllVoicesMap.begin();
    i != fStaffAllVoicesMap.end();
    i++) {
    (*i).second->
      appendTransposeToVoice (transpose);
  } // for
}

void msrStaff::appendAccordionRegistrationToStaff (
  S_msrAccordionRegistration
    accordionRegistration)
{
  for (
    map<int, S_msrVoice>::const_iterator i = fStaffAllVoicesMap.begin();
    i != fStaffAllVoicesMap.end();
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
    map<int, S_msrVoice>::const_iterator i = fStaffAllVoicesMap.begin();
    i != fStaffAllVoicesMap.end();
    i++) {
    (*i).second->
      appendHarpPedalsTuningToVoice (
        harpPedalsTuning);
  } // for
}

void msrStaff::finalizeCurrentMeasureInStaff (
  int inputLineNumber)
{
  // first take care of the staff's silent voice
  switch (fStaffKind) {
    case msrStaff::kMasterStaff:
      // no silent voice here
      break;
      
    case msrStaff::kRegularStaff:
    case msrStaff::kTablatureStaff:
    case msrStaff::kPercussionStaff:
    case msrStaff::kHarmonyStaff:
    case msrStaff::kFiguredBassStaff:
      {
        rational
          partMeasureLengthHighTide =
            fStaffPartUplink->
              getPartMeasureLengthHighTide ();
            
        if (gGeneralOptions->fTraceMeasures || gGeneralOptions->fTraceStaves) {
          gLogIos <<
            "Finalizing current measure in staff \"" <<
            getStaffName () <<
            "\", line " << inputLineNumber <<
            ", partMeasureLengthHighTide = " <<
            partMeasureLengthHighTide <<
            endl;
        }

        // first bring the staff's silent voice
        // to the part's measure lenth high tide
        fStaffSilentVoice->
          bringVoiceToMeasureLength (
            inputLineNumber,
            partMeasureLengthHighTide);
      
        // then finalize the staff silent voice,
        // for use by the other voices
        fStaffSilentVoice->
          finalizeCurrentMeasureInVoice ( // JMI deja???
            inputLineNumber);
      }
      break;

      /* JMI
    case msrStaff::kHarmonyStaff:
      // no silent voice here
      break;
      */
  } // switch

  // then finalize all the registered voices
  for (
    map<int, S_msrVoice>::const_iterator i = fStaffAllVoicesMap.begin();
    i != fStaffAllVoicesMap.end();
    i++) {
    S_msrVoice
      voice =
        (*i).second;

    switch (voice->getVoiceKind ()) {
      case msrVoice::kMasterVoice:
      case msrVoice::kRegularVoice:
      case msrVoice::kHarmonyVoice:
      case msrVoice::kFiguredBassVoice:
        voice->
          finalizeCurrentMeasureInVoice (
            inputLineNumber);
        break;
        
      case msrVoice::kSilentVoice:
        // it has already been finalized
        break;
    } // switch
  } // for
}

void msrStaff::finalizeStaff (int inputLineNumber)
{  
  if (gGeneralOptions->fTraceStaves) {
    gLogIos <<
      "Finalizing staff \"" <<
      getStaffName () << "\"" <<
      ", line " << inputLineNumber <<
      endl;
  }

  for (
    map<int, S_msrVoice>::const_iterator i = fStaffAllVoicesMap.begin();
    i != fStaffAllVoicesMap.end();
    i++) {
    S_msrVoice
      voice = (*i).second;

    msrVoice::msrVoiceFinalizationStatus
      voiceFinalizationStatus =
        voice->
          finalizeVoice (inputLineNumber);

    switch (voiceFinalizationStatus) {
      case msrVoice::kKeepVoice:
        if (gGeneralOptions->fTraceVoices || gGeneralOptions->fTraceStaves) {
          gLogIos <<
            "Keeping voice \"" <<
            voice->getVoiceName () << "\"" <<
            " if staff \"" <<getStaffName () << "\"" <<
            ", line " << inputLineNumber <<
            endl;
        }

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
        break;
        
      case msrVoice::kEraseVoice:
        if (gGeneralOptions->fTraceVoices || gGeneralOptions->fTraceStaves) {
          gLogIos <<
            "Erasing voice \"" <<
            voice->getVoiceName () << "\"" <<
            " if staff \"" <<getStaffName () << "\"" <<
            ", line " << inputLineNumber <<
            endl;
        }
      
        fStaffAllVoicesMap.erase (i);
        break;
    } // switch
  } // for
}

void msrStaff::acceptIn (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors)
    gLogIos <<
      "% ==> msrStaff::acceptIn()" <<
      endl;
      
  if (visitor<S_msrStaff>*
    p =
      dynamic_cast<visitor<S_msrStaff>*> (v)) {
        S_msrStaff elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors)
          gLogIos <<
            "% ==> Launching msrStaff::visitStart()" <<
             endl;
        p->visitStart (elem);
  }
}

void msrStaff::acceptOut (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors)
    gLogIos <<
      "% ==> msrStaff::acceptOut()" <<
      endl;

  if (visitor<S_msrStaff>*
    p =
      dynamic_cast<visitor<S_msrStaff>*> (v)) {
        S_msrStaff elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors)
          gLogIos <<
            "% ==> Launching msrStaff::visitEnd()" <<
            endl;
        p->visitEnd (elem);
  }
}

void msrStaff::browseData (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors)
    gLogIos <<
      "% ==> msrStaff::browseData()" <<
      endl;

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
      list<S_msrStaffTuning>::const_iterator i = fStaffTuningsList.begin();
      i != fStaffTuningsList.end();
      i++) {
      // browse the voice
      msrBrowser<msrStaffTuning> browser (v);
      browser.browse (*(*i));
    } // for
    idtr--;
  }
*/

  if (fStaffAllVoicesMap.size ()) {
    for (
      map<int, S_msrVoice>::const_iterator i = fStaffAllVoicesMap.begin();
      i != fStaffAllVoicesMap.end();
      i++) {
        msrBrowser<msrVoice> browser (v);
        browser.browse (*((*i).second));
    } // for
  }

  if (gMsrOptions->fTraceMsrVisitors)
    gLogIos <<
      "% <== msrStaff::browseData()" <<
      endl;
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
    case msrStaff::kMasterStaff:
      result = "master";
      break;
    case msrStaff::kRegularStaff:
      result = "regular";
      break;
    case msrStaff::kTablatureStaff:
      result = "tablature";
      break;
    case msrStaff::kPercussionStaff:
      result = "percussion";
      break;
    case msrStaff::kHarmonyStaff:
      result = "harmony";
      break;
    case msrStaff::kFiguredBassStaff:
      result = "figured bass";
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
    ", (" <<
    singularOrPlural (
      fStaffVoiceRelativeNumberToVoiceMap.size(), "voice", "voices") <<
    ")" <<
    endl;

  idtr++;

  const int fieldwidth = 28;
  
  os <<
    setw (fieldwidth) << "StaffPartUplink" << " : " <<
    fStaffPartUplink->getPartCombinedName () <<
    endl <<
    setw (fieldwidth) << "StaffNumber" << " : " <<
    staffNumberAsString () <<
    endl <<
    setw (fieldwidth) << "StaffInstrumentName" << " : \"" <<
    fStaffInstrumentName <<
    "\"" <<
    endl <<
    setw (fieldwidth) << "StaffInstrumentAbbreviation" << " : \"" <<
    fStaffInstrumentAbbreviation <<
    "\"" <<
    endl;

/* JMI
  os <<
    "StaffInstrumentName: \"" <<
    fStaffInstrumentName << "\"" << endl;
*/

/*
  // print the staff staff datails if any
  os <<
    "Staff details: ";
  if (fCurrentStaffStaffDetails) {
    os << fCurrentStaffStaffDetails;
  }
  else
    os << "none";
  os << endl;
*/

  os << endl;

  /* JMI
  if (fStaffTuningsList.size ()) {
    os <<
      "Staff tunings:" <<
      endl;
      
    list<S_msrStaffTuning>::const_iterator
      iBegin = fStaffTuningsList.begin(),
      iEnd   = fStaffTuningsList.end(),
      i      = iBegin;
      
    idtr++;
    for ( ; ; ) {
      os <<
        (*i)->staffTuningAsString ();
      if (++i == iEnd) break;
      os << endl;
    } // for
    os << endl;
    idtr--;
  }

*/

  // print the registered voices
// JMI ???  if (fStaffVoiceRelativeNumberToVoiceMap.size ()) {
  if (fStaffAllVoicesMap.size ()) {
    map<int, S_msrVoice>::const_iterator
      iBegin = fStaffAllVoicesMap.begin(),
      iEnd   = fStaffAllVoicesMap.end(),
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

      switch (voice->getVoiceKind ()) {
        case msrVoice::kMasterVoice:
          os <<
            voice;
          break;

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
          
        case msrVoice::kSilentVoice:
          if (gMsrOptions->fShowSilentVoices)
            os <<
              voice;
          break;
      } // switch

      if (++i == iEnd) break;

      switch (voice->getVoiceKind ()) {
        case msrVoice::kMasterVoice:
          os << endl;
          break;

        case msrVoice::kRegularVoice:
          os << endl;
          break;

        case msrVoice::kHarmonyVoice:
          if (
            gMsrOptions->fShowHarmonyVoices
              ||
            voice->getMusicHasBeenInsertedInVoice ())
            os <<
              endl;
          break;
          
        case msrVoice::kFiguredBassVoice:
          if (
            gMsrOptions->fShowFiguredBassVoices
              ||
            voice->getMusicHasBeenInsertedInVoice ())
            os <<
              endl;
          break;
          
        case msrVoice::kSilentVoice:
          if (gMsrOptions->fShowSilentVoices)
            os <<
              endl;
          break;
      } // switch

    } // for
  }

  idtr--;
}

void msrStaff::printStructure (ostream& os)
{
  os <<
    "Staff" " " << getStaffName () <<
    ", " << staffKindAsString () <<
    " (" <<
    singularOrPlural (
      fStaffVoiceRelativeNumberToVoiceMap.size(), "voice", "voices") <<
    ")" <<
    endl;

  idtr++;

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
      iBegin = fStaffTuningsList.begin(),
      iEnd   = fStaffTuningsList.end(),
      i      = iBegin;
      
    idtr++;
    for ( ; ; ) {
      os << (*i)->staffTuningAsString ();
      if (++i == iEnd) break;
      os << endl;
    } // for
    os << endl;
    idtr--;
  }
*/

  // print the registered voices names
  if (fStaffVoiceRelativeNumberToVoiceMap.size ()) {
    os <<
      "Voices:" <<
      endl;
  
    idtr++;
    
    map<int, S_msrVoice>::const_iterator
      iBegin = fStaffVoiceRelativeNumberToVoiceMap.begin(),
      iEnd   = fStaffVoiceRelativeNumberToVoiceMap.end(),
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
            voice->getVoiceStanzasMap ().size(),
            "stanza",
            "stanzas") <<
          ")";
      if (++i == iEnd) break;
      os << endl;
    } // for

    idtr --;
  }

  idtr--;

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
 if (gGeneralOptions->fTraceStaffTuning) {
    gLogIos <<
      "==> Creating a newborn clone of staff change '" <<
      voiceStaffChangeAsString () <<
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

void msrVoiceStaffChange::acceptIn (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors)
    gLogIos <<
      "% ==> msrVoiceStaffChange::acceptIn()" <<
      endl;
      
  if (visitor<S_msrVoiceStaffChange>*
    p =
      dynamic_cast<visitor<S_msrVoiceStaffChange>*> (v)) {
        S_msrVoiceStaffChange elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors)
          gLogIos <<
            "% ==> Launching msrVoiceStaffChange::visitStart()" <<
             endl;
        p->visitStart (elem);
  }
}

void msrVoiceStaffChange::acceptOut (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors)
    gLogIos <<
      "% ==> msrVoiceStaffChange::acceptOut()" <<
      endl;

  if (visitor<S_msrVoiceStaffChange>*
    p =
      dynamic_cast<visitor<S_msrVoiceStaffChange>*> (v)) {
        S_msrVoiceStaffChange elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors)
          gLogIos <<
            "% ==> Launching msrVoiceStaffChange::visitEnd()" <<
            endl;
        p->visitEnd (elem);
  }
}

void msrVoiceStaffChange::browseData (basevisitor* v)
{}

string msrVoiceStaffChange::voiceStaffChangeAsString () const
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
    voiceStaffChangeAsString () <<
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
  // replace spaces in part ID to set fPartID
  for_each (
    partID.begin(),
    partID.end(),
    stringSpaceReplacer (fPartID, '_'));

  // sanity check
  msrAssert(
    partPartGroupUplink != 0,
    "partPartGroupUplink is null");
    
  // set part's part group uplink
  fPartPartGroupUplink = partPartGroupUplink;

  // do other initializations
  initializePart ();
}

void msrPart::initializePart ()
{
  if (gGeneralOptions->fTraceParts)
    gLogIos <<
      "==> Initializing part \"" << getPartCombinedName () <<
      endl;

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
  
  if (gGeneralOptions->fTraceParts)
    gLogIos <<
      "==> Creating part " << getPartCombinedName () << endl;
  
  setPartMeasureLengthHighTide (
    fInputLineNumber,
    rational (0, 1));

  // create the part master staff and voice
  createPartMasterStaffAndVoice (0); // input line number // JMI
    
/* JMI
  // set part current time to the default 4/4 time signature
  fPartCurrenltTime =
    msrTime::createFourQuartersTime (
      fInputLineNumber);
*/
}

msrPart::~msrPart()
{}

S_msrPart msrPart::createPartNewbornClone (S_msrPartGroup partGroupClone)
{
  if (gGeneralOptions->fTraceParts) {
    gLogIos <<
      "==> Creating a newborn clone of part " <<
      getPartCombinedName () <<
      endl;
  }

  // sanity check
  msrAssert(
    partGroupClone != 0,
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
  newbornClone->fPartAbbreviation =
    fPartAbbreviation;
  
  newbornClone->fPartInstrumentName =
    fPartInstrumentName;
  newbornClone->fPartInstrumentAbbreviation =
    fPartInstrumentAbbreviation;

  return newbornClone;
}


void msrPart::createPartMasterStaffAndVoice (
  int inputLineNumber)
{
  if (fPartMasterStaff) {
    stringstream s;

    s <<
      "Part \"" <<
      getPartCombinedName () <<
      "\" already has a master staff";

    msrInternalError (
      inputLineNumber,
      s.str ());
  }
    
  // create the part master staff
  if (
    gGeneralOptions->fTraceParts
      ||
    gGeneralOptions->fTraceStaves
      ||
    gGeneralOptions->fTraceVoices) {
    gLogIos <<
      "==> Creating the master staff" <<
      " with number " << K_PART_MASTER_STAFF_NUMBER <<
      " for part " <<
      getPartCombinedName () <<
      ", line " << inputLineNumber <<
      endl;
  }

  fPartMasterStaff =
    addStaffToPartByItsNumber (
      inputLineNumber,
      msrStaff::kMasterStaff,
      K_PART_MASTER_STAFF_NUMBER);
    
  // create the part master voice  
  if (
    gGeneralOptions->fTraceParts
      ||
    gGeneralOptions->fTraceStaves
      ||
    gGeneralOptions->fTraceVoices) {
    gLogIos <<
      "==> Creating the master voice " <<
      " with number " << K_PART_MASTER_VOICE_NUMBER <<
      " for part " <<
      getPartCombinedName () <<
      ", line " << inputLineNumber <<
      endl;
  }

  fPartMasterVoice =
    msrVoice::create (
      inputLineNumber,
      msrVoice::kMasterVoice,
      K_PART_MASTER_VOICE_NUMBER,
      msrVoice::kCreateInitialLastSegmentYes,
      fPartMasterStaff);

  // register it in master staff
  fPartMasterStaff->
    registerVoiceInStaff (
      inputLineNumber,
      fPartMasterVoice );

  if (fPartCurrentTime) { // JMI
    // append part current time to master voice
    fPartMasterVoice->
      appendTimeToVoice (
        fPartCurrentTime);
  }

  if (false && gGeneralOptions->fTraceParts) { // JMI
    gLogIos <<
      endl <<
      "***********" <<
      endl <<
      endl <<
      idtr;
    print (gLogIos);
    gLogIos <<
      "***********" <<
      endl <<
      endl;
  }
}

void msrPart::setPartName (string partName)
{
  fPartName = partName;

  // set part instrument name value by default it not yet set
  if (fPartInstrumentName.size () == 0)
    fPartInstrumentName = fPartName;
}

void msrPart::setPartAbbreviation (
  string partAbbreviation)
{
  fPartAbbreviation = partAbbreviation;

  // set part instrument abbreviation value by default it not yet set
  if (fPartInstrumentAbbreviation.size () == 0)
    fPartInstrumentAbbreviation = fPartName;
}

/* JMI
void msrPart::setPartInstrumentName (
  string partInstrumentName)
{
  fPartInstrumentName = partInstrumentName; }

void msrPart::setPartInstrumentAbbreviation (
  string partInstrumentAbbreviation)
    {
      fPartInstrumentAbbreviation =
        partInstrumentAbbreviation;
    }
*/

void msrPart::createPartHarmonyStaffAndVoiceIfNotYetDone (
  int inputLineNumber)
{
  if (! fPartHarmonyStaff) {    
    // create the part harmony staff
    if (
      gGeneralOptions->fTraceParts
        ||
      gGeneralOptions->fTraceHarmonies
        ||
      gGeneralOptions->fTraceStaves
        ||
      gGeneralOptions->fTraceVoices) {
      gLogIos <<
        "==> Creating the harmony staff" <<
        " with number " << K_PART_HARMONY_STAFF_NUMBER <<
        " for part " <<
        getPartCombinedName () <<
        ", line " << inputLineNumber <<
        endl;
    }
  
    fPartHarmonyStaff =
      addStaffToPartByItsNumber (
        inputLineNumber,
        msrStaff::kHarmonyStaff,
        K_PART_HARMONY_STAFF_NUMBER);
      
    // create the part harmony voice  
    if (gGeneralOptions->fTraceHarmonies || gGeneralOptions->fTraceVoices)
      gLogIos <<
        "==> Creating the harmony voice " <<
        " with number " << K_PART_HARMONY_VOICE_NUMBER <<
        " for part " <<
        getPartCombinedName () <<
        ", line " << inputLineNumber <<
        endl;

    // create a deep copy of the part master voice
    fPartHarmonyVoice =
      fPartMasterVoice->
        createVoiceDeepCopy (
          inputLineNumber,
          msrVoice::kHarmonyVoice,
          K_PART_HARMONY_VOICE_NUMBER,
          fPartHarmonyStaff);
  
    // register it in harmony staff
    fPartHarmonyStaff->
      registerVoiceInStaff (
        inputLineNumber,
        fPartHarmonyVoice );

    if (fPartCurrentTime) {
      // append part current time to harmony voice
      fPartHarmonyVoice->
        appendTimeToVoice (
          fPartCurrentTime);
    }
  }

/* JMI
  gLogIos <<
    endl <<
    "***********" <<
    endl <<
    endl <<
    idtr;
  print (gLogIos);
  gLogIos <<
    "***********" <<
    endl <<
    endl;
  */  
}

void msrPart::createPartFiguredStaffAndVoiceIfNotYetDone (
  int inputLineNumber)
{
  if (! fPartFiguredBassStaff) {    
    // create the part figured bass staff
    if (
      gGeneralOptions->fTraceParts
        ||
      gGeneralOptions->fTraceFiguredBass
        ||
      gGeneralOptions->fTraceStaves
        ||
      gGeneralOptions->fTraceVoices) {
      gLogIos <<
        "==> Creating the figured bass staff" <<
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
    if (gGeneralOptions->fTraceHarmonies || gGeneralOptions->fTraceVoices)
      gLogIos <<
        "==> Creating the figured bass voice " <<
        " with number " << K_PART_FIGURED_BASS_VOICE_NUMBER <<
        " for part " <<
        getPartCombinedName () <<
        ", line " << inputLineNumber <<
        endl;

    // create a deep copy of the part master voice
    fPartFiguredBassVoice =
      fPartMasterVoice->
        createVoiceDeepCopy (
          inputLineNumber,
          msrVoice::kFiguredBassVoice,
          K_PART_FIGURED_BASS_VOICE_NUMBER,
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
  gLogIos <<
    endl <<
    "***********" <<
    endl <<
    endl <<
    idtr;
  print (gLogIos);
  gLogIos <<
    "***********" <<
    endl <<
    endl;
  */  
}

void msrPart::setPartMeasureLengthHighTide (
  int      inputLineNumber,
  rational measureLength)
{
  if (gGeneralOptions->fTraceDivisions || gGeneralOptions->fTraceMeasures)
    gLogIos <<
      "Setting measure length high tide for part \"" <<
      getPartCombinedName () <<
      "\" to " << measureLength <<
      ", line " << inputLineNumber <<
      endl;

  fPartMeasureLengthHighTide = measureLength;
}

void msrPart::updatePartMeasureLengthHighTide (
  int      inputLineNumber,
  rational measureLength)
{
  if (measureLength > fPartMeasureLengthHighTide) {
    if (gGeneralOptions->fTraceDivisions || gGeneralOptions->fTraceMeasures)
        gLogIos <<
          "Updating measure length high tide for part \"" <<
          getPartCombinedName () <<
          "\" to " << measureLength <<
          ", line " << inputLineNumber <<
          endl;

    fPartMeasureLengthHighTide = measureLength;
  }
}

void msrPart::bringPartToMeasureLength (
  int      inputLineNumber,
  rational measureLength)
{
  if (gGeneralOptions->fTraceParts || gGeneralOptions->fTraceMeasures) {
    gLogIos <<
      "Bringing measure length for part \"" <<
      getPartCombinedName () <<
      "\" to " << measureLength <<
      ", line " << inputLineNumber <<
      endl;
  }

  // print the master staff to measure length specifically
  fPartMasterStaff->
    bringStaffToMeasureLength (
      inputLineNumber,
      measureLength);  

  // print the registered staves to measure length  
  for (
    map<int, S_msrStaff>::const_iterator i = fPartStavesMap.begin();
    i != fPartStavesMap.end();
    i++) {
    (*i).second->
      bringStaffToMeasureLength (
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

    if (gGeneralOptions->fTraceParts)
      gLogIos <<
        "Setting part name of " << getPartCombinedName () <<
        " to \"" << fPartMsrName << "\"" <<
         endl;
  }
  else {
    // use the argument
    fPartMsrName = partMsrName;

    if (gGeneralOptions->fTraceParts)
      gLogIos <<
        "Keeping partID \"" << partMsrName <<
        "\" as part name  for " << getPartCombinedName () <<
      endl;
  }
}

string msrPart::getPartCombinedName () const
{
  stringstream s;
  
  s <<
    "\"" << fPartMsrName << "\"" <<
    " (" << fPartID;

  if (fPartName.size ())
    s <<
      ", \"" << fPartName << "\"";

  s <<
    ")";

  return s.str ();
}

void msrPart::createMeasureAndAppendItToPart (
  int    inputLineNumber,
  string measureNumber)
{
  if (gGeneralOptions->fTraceMeasures)
    gLogIos <<
      "==> Creating and appending measure '" <<
      measureNumber <<
      "'to part " <<
      getPartCombinedName () <<
      "', line " << inputLineNumber <<
      endl;

  // set part current measure number
  fPartCurrentMeasureNumber = measureNumber;

  // create and append measure to the master staff
  fPartMasterStaff->
    createMeasureAndAppendItToStaff (
      inputLineNumber,
      measureNumber);

  // create and append measure to registered staves
  for (
    map<int, S_msrStaff>::const_iterator i = fPartStavesMap.begin();
    i != fPartStavesMap.end();
    i++) {
    S_msrStaff
      staff = (*i).second;
      
    staff->
      createMeasureAndAppendItToStaff (
        inputLineNumber,
        measureNumber);
  } // for
}

/* JMI
void msrPart::complementPartVoicesUpToMeasure (
  int    inputLineNumber,
  string measureNumber)
{
  if (gGeneralOptions->fTraceMeasures)
    gLogIos <<
      "Complement part voices up to measure number " << measureNumber <<
      ", line " << inputLineNumber <<
      ", in part " << getPartCombinedName () <<
      endl;

  // propagate to all staves
  for (
    map<int, S_msrStaff>::const_iterator i = fPartStavesMap.begin();
    i != fPartStavesMap.end();
    i++) {
    S_msrStaff
      staff = (*i).second;
      / * JMI
    staff->
      createMeasureAndAppendItToStaff (
        inputLineNumber, measureNumber);
  } // for
}
*/

void msrPart::appendStaffDetailsToPart (
  S_msrStaffDetails staffDetails)
{
  if (gGeneralOptions->fTraceStaves || gGeneralOptions->fTraceParts)
    gLogIos <<
      "Appending staff details\"" <<
      staffDetails->staffDetailsAsShortString () <<
      "\" to part " << getPartCombinedName () <<
    endl;

  // register staff details in part
  fCurrentPartStaffDetails = staffDetails;
  
  // append staff details to the master staff specifically
  fPartMasterStaff->
    appendStaffDetailsToStaff (
      staffDetails);
  
  // append staff details to registered staves
  for (
    map<int, S_msrStaff>::const_iterator i = fPartStavesMap.begin();
    i != fPartStavesMap.end();
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
  if (gGeneralOptions->fTraceParts || gGeneralOptions->fTraceClefs) {
    gLogIos <<
      "Appending clef '" <<
      clef->clefAsString () <<
      "' to part " << getPartCombinedName () <<
    endl;
  }

  // set part clef
  fPartCurrentClef = clef;
  
  // append clef to registered staves
  for (
    map<int, S_msrStaff>::const_iterator i = fPartStavesMap.begin();
    i != fPartStavesMap.end();
    i++) {
    (*i).second->
      appendClefToStaff (
        clef);
  } // for
}

void msrPart::appendKeyToPart  (S_msrKey  key)
{
  if (gGeneralOptions->fTraceKeys || gGeneralOptions->fTraceParts) {
    gLogIos <<
      "Appending key '" <<
      key->keyAsString () <<
      "' to part " << getPartCombinedName () <<
    endl;
  }
  
  // set part key
  fPartCurrentKey = key;

  // append key to registered staves
  for (
    map<int, S_msrStaff>::const_iterator i = fPartStavesMap.begin();
    i != fPartStavesMap.end();
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
  if (gGeneralOptions->fTraceTimes || gGeneralOptions->fTraceParts) {
    gLogIos <<
      "Appending time '" <<
      time->timeAsString () <<
      "' to part " << getPartCombinedName () <<
    endl;
  }
  
  // set part time
  fPartCurrentTime = time;

  // append time to the master staff specifically,
  // this is crucial to the score structure
  fPartMasterStaff->
    appendTimeToStaff (
      time);
  
  // append time to registered staves
  for (
    map<int, S_msrStaff>::const_iterator i = fPartStavesMap.begin();
    i != fPartStavesMap.end();
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
  if (gGeneralOptions->fTraceTimes || gGeneralOptions->fTraceParts) {
    gLogIos <<
      "Appending time '" <<
      time->timeAsString () <<
      "' to part clone " << getPartCombinedName () <<
    endl;
  }
  
  // set part time
  fPartCurrentTime = time;

  // propagate it to all staves
  for (
    map<int, S_msrStaff>::const_iterator i = fPartStavesMap.begin();
    i != fPartStavesMap.end();
    i++) {
    S_msrStaff
      staff = (*i).second;

    staff->
      appendTimeToStaffClone (time);
  } // for
}

void msrPart::appendTransposeToPart (S_msrTranspose transpose)
{
  if (gGeneralOptions->fTraceTranspositions || gGeneralOptions->fTraceParts)
    gLogIos <<
      "Appending transpose \"" <<
      transpose->transposeAsString () <<
      "\" to part " << getPartCombinedName () <<
    endl;

  // set part current transpose
  fPartCurrentTranspose = transpose;

  // propagate it to all staves
  for (
    map<int, S_msrStaff>::const_iterator i = fPartStavesMap.begin();
    i != fPartStavesMap.end();
    i++) {
    S_msrStaff
      staff = (*i).second;

    staff->
      appendTransposeToStaff (transpose);
  } // for
}


void msrPart::createRepeatAndAppendItToPart (int inputLineNumber)
{
  // create repeat and append it to master staff specifically
  fPartMasterStaff->
    createRepeatAndAppendItToStaff (inputLineNumber);  
  
  // create repeat and append it to registered staves
  for (
    map<int, S_msrStaff>::const_iterator i = fPartStavesMap.begin();
    i != fPartStavesMap.end();
    i++) {
    (*i).second->
      createRepeatAndAppendItToStaff (inputLineNumber);
  } // for
}

void msrPart::appendRepeatEndingToPart (
  int       inputLineNumber,
  string    repeatEndingNumber, // may be "1, 2"
  msrRepeatEnding::msrRepeatEndingKind
            repeatEndingKind)
{
  // append repeat ending to master staff specifically
  fPartMasterStaff->
    appendRepeatEndingToStaff (
      inputLineNumber,
      repeatEndingNumber,
      repeatEndingKind);
  
  // append repeat ending to registered staves
  for (
    map<int, S_msrStaff>::const_iterator i = fPartStavesMap.begin();
    i != fPartStavesMap.end();
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
    map<int, S_msrStaff>::const_iterator i = fPartStavesMap.begin();
    i != fPartStavesMap.end();
    i++) {
    S_msrStaff
      staff =
        (*i).second;

    switch (staff->getStaffKind ()) {
      case msrStaff::kMasterStaff:
        staff->appendRepeatCloneToStaff (
          inputLineNumber, repeatCLone);
        break;
        
      case msrStaff::kRegularStaff:
        staff->appendRepeatCloneToStaff (
          inputLineNumber, repeatCLone);
        break;
        
      case msrStaff::kTablatureStaff:
        break;
        
      case msrStaff::kPercussionStaff:
        break;
        
      case msrStaff::kHarmonyStaff:
        break;
        
      case msrStaff::kFiguredBassStaff:
        break;
    } // switch
  } // for
}

void msrPart::appendRepeatEndingCloneToPart (
  S_msrRepeatEnding repeatEndingCLone)
{
  for (
    map<int, S_msrStaff>::const_iterator i = fPartStavesMap.begin();
    i != fPartStavesMap.end();
    i++) {
    (*i).second->
      appendRepeatEndingCloneToStaff (repeatEndingCLone);
  } // for
}

void msrPart::createMeasureRepeatFromItsFirstMeasureInPart (
  int inputLineNumber,
  int measureRepeatMeasuresNumber,
  int measureRepeatSlashes)
{
  // create measure repeat from its first measure in master staff specifically
  fPartMasterStaff->
    createMeasureRepeatFromItsFirstMeasureInStaff (
      inputLineNumber,
      measureRepeatMeasuresNumber,
      measureRepeatSlashes);
  
  // create measure repeat from its first measure in registered staves
  for (
    map<int, S_msrStaff>::const_iterator i = fPartStavesMap.begin();
    i != fPartStavesMap.end();
    i++) {
    (*i).second->
      createMeasureRepeatFromItsFirstMeasureInStaff (
        inputLineNumber,
        measureRepeatMeasuresNumber,
        measureRepeatSlashes);
  } // for
}

void msrPart::appendPendingMeasureRepeatToPart (
  int inputLineNumber)
{
  // append pending measure repeat to master staff specifically
  fPartMasterStaff->
    appendPendingMeasureRepeatToStaff (
      inputLineNumber);
  
  // append pending measure repeat to registered staves
  for (
    map<int, S_msrStaff>::const_iterator i = fPartStavesMap.begin();
    i != fPartStavesMap.end();
    i++) {
    (*i).second->
      appendPendingMeasureRepeatToStaff (
        inputLineNumber);
  } // for
}

void msrPart::createMultipleRestInPart (
  int inputLineNumber,
  int multipleRestMeasuresNumber)
{
  if (gGeneralOptions->fTraceRepeats || gGeneralOptions->fTraceParts) {
    gLogIos <<
      "==> Creating a multiple rest in part " <<
      getPartCombinedName () <<
      ", " <<
      singularOrPlural (
        multipleRestMeasuresNumber, "measure", "measures") <<
      endl;
  }

  // create multiple rest in master staff specifically
  fPartMasterStaff->
    createMultipleRestInStaff (
      inputLineNumber,
      multipleRestMeasuresNumber);
  
  // create multiple rest in registered staves
  for (
    map<int, S_msrStaff>::const_iterator i = fPartStavesMap.begin();
    i != fPartStavesMap.end();
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
  if (gGeneralOptions->fTraceRepeats || gGeneralOptions->fTraceParts) {
    gLogIos <<
      "Appending the pending multiple rest to part " <<
      getPartCombinedName () <<
      endl;
  }

  // append pending multiple rest to master staff specifically
  fPartMasterStaff->
    appendPendingMultipleRestToStaff (
      inputLineNumber);
  
  // append pending multiple rest to registered staves
  for (
    map<int, S_msrStaff>::const_iterator i = fPartStavesMap.begin();
    i != fPartStavesMap.end();
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
  if (gGeneralOptions->fTraceStaves || gGeneralOptions->fTraceParts) {
    gLogIos <<
      "Appending multiple rest '" <<
      multipleRest->multipleRestAsString () <<
      "' to part clone " <<
      getPartCombinedName () <<
      endl;
  }

  for (
    map<int, S_msrStaff>::const_iterator i = fPartStavesMap.begin();
    i != fPartStavesMap.end();
    i++) {
    (*i).second->
      appendMultipleRestCloneToStaff (
        inputLineNumber,
        multipleRest);
  } // for
}

void msrPart::appendBarlineToPart (S_msrBarline barline)
{
  // append barline to master staff specifically
  fPartMasterStaff->
    appendBarlineToStaff (barline);
  
  // append barline to registered staves
  for (
    map<int, S_msrStaff>::const_iterator i = fPartStavesMap.begin();
    i != fPartStavesMap.end();
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

    msrInternalError ( // JMI
      inputLineNumber, s.str ());
      
    return fPartStavesMap [staffNumber];
  }

  if (gGeneralOptions->fTraceStaves || gGeneralOptions->fTraceParts) {
    gLogIos <<
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
    case msrStaff::kMasterStaff:
      // DON'T register it in part staves map,
      // it will be handled specifically
      break;
      
    case msrStaff::kRegularStaff:
    case msrStaff::kTablatureStaff:
    case msrStaff::kPercussionStaff:
    case msrStaff::kHarmonyStaff:
    case msrStaff::kFiguredBassStaff:
      fPartStavesMap [staffNumber] = staff;
      break;
  } // switch
  
  // initialize staff current clef and key if relevant // JMI
  switch (staffKind) {
    case msrStaff::kMasterStaff:
      // DON'T initialize staff and key in par master stave,
      break;
      
    case msrStaff::kRegularStaff:
    case msrStaff::kTablatureStaff:
    case msrStaff::kPercussionStaff:
    case msrStaff::kHarmonyStaff:
    case msrStaff::kFiguredBassStaff:
      staff->setStaffCurrentClef (fPartCurrentClef);
      staff->setStaffCurrentKey (fPartCurrentKey);
      break;
  } // switch
  
  // return staff
  return staff;
}

void msrPart::addStaffToPartCloneByItsNumber (S_msrStaff staff)
{
  if (gGeneralOptions->fTraceStaves || gGeneralOptions->fTraceParts) {
    gLogIos <<
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

void msrPart::setPartHarmoniesSupplierVoice (
  int        inputLineNumber,
  S_msrVoice partHarmoniesSupplierVoice)
{
  switch (partHarmoniesSupplierVoice->getVoiceKind ()) {
    case msrVoice::kRegularVoice:
      if (! fPartHarmoniesSupplierVoice) {
        // first harmonies supplier voice met in this part,
        // set part harmonies supplier voice accordingly
        if (gGeneralOptions->fTraceHarmonies || gGeneralOptions->fTraceParts)
          gLogIos <<
            "Setting voice \"" <<
            partHarmoniesSupplierVoice->
              getVoiceName () <<
            "\" as harmonies supplier for part " <<
            getPartCombinedName () <<
            ", line " << inputLineNumber <<
            endl;
    
        fPartHarmoniesSupplierVoice =
          partHarmoniesSupplierVoice;
      }
    
      else if (
        fPartHarmoniesSupplierVoice != partHarmoniesSupplierVoice) {
        stringstream s;
    
        s <<
          "harmonies are supplied both by:" <<
          endl <<
          msrVoice::voiceKindAsString (
            fPartHarmoniesSupplierVoice->getVoiceKind ()) <<
          " \"" << fPartHarmoniesSupplierVoice->getVoiceName () << "\"" <<
          endl <<
          "and:" <<
          endl <<
          msrVoice::voiceKindAsString (
            partHarmoniesSupplierVoice->getVoiceKind ()) <<
          " \"" << partHarmoniesSupplierVoice->getVoiceName () << "\"";
    
        msrMusicXMLError (
          inputLineNumber,
          s.str ());
      }
      break;
      
    case msrVoice::kMasterVoice:
    case msrVoice::kHarmonyVoice:
    case msrVoice::kFiguredBassVoice:
    case msrVoice::kSilentVoice:
      {
        stringstream s;
    
        s <<
          "harmonies cannot by supplied by " <<
          msrVoice::voiceKindAsString (
            partHarmoniesSupplierVoice->getVoiceKind ()) <<
          " voice \"" <<
           partHarmoniesSupplierVoice->getVoiceName () <<
           "\"";
    
        msrMusicXMLError (
          inputLineNumber,
          s.str ());
      }
      break;
  } // switch
}
                              
void msrPart::appendHarmonyToPart (
  S_msrVoice   harmoniesSupplierVoice,
  S_msrHarmony harmony)
{
  int inputLineNumber =
    harmony->getInputLineNumber ();

  switch (harmoniesSupplierVoice->getVoiceKind ()) {
    case msrVoice::kRegularVoice:
      // create the harmony staff and voice if not yet done
      createPartHarmonyStaffAndVoiceIfNotYetDone (
        inputLineNumber);
      
      // register this voice as the part harmonies supplier voice
      setPartHarmoniesSupplierVoice (
        inputLineNumber,
        harmoniesSupplierVoice);
    
      // append the harmony to the part harmony voice
      if (gGeneralOptions->fTraceHarmonies || gGeneralOptions->fTraceParts)
        gLogIos <<
          "Appending harmony '" <<
          harmony->harmonyAsString () <<
          "' to part " <<
          getPartCombinedName () <<
          ", line " << inputLineNumber <<
          endl;
    
      fPartHarmonyVoice->
        appendHarmonyToVoice (harmony);
      break;
      
    case msrVoice::kMasterVoice:
    case msrVoice::kHarmonyVoice:
    case msrVoice::kFiguredBassVoice:
    case msrVoice::kSilentVoice:
      {
        stringstream s;
    
        s <<
          "harmonies cannot by supplied to part by " <<
          msrVoice::voiceKindAsString (
            harmoniesSupplierVoice->getVoiceKind ()) <<
          " voice \" " <<
          harmoniesSupplierVoice->getVoiceName () <<
          "\"";
    
        msrInternalError (
          inputLineNumber,
          s.str ());
      }
      break;
  } // switch
}

void msrPart::appendHarmonyToPartClone (
  S_msrVoice   harmoniesSupplierVoice,
  S_msrHarmony harmony)
{
  int inputLineNumber =
    harmony->getInputLineNumber ();

  switch (harmoniesSupplierVoice->getVoiceKind ()) {
    case msrVoice::kHarmonyVoice:
      // create the harmony staff and voice if not yet done
      createPartHarmonyStaffAndVoiceIfNotYetDone (
        inputLineNumber);

      /* JMI NON
      // register this voice as the part harmonies supplier voice
      setPartHarmoniesSupplierVoice (
        inputLineNumber,
        harmoniesSupplierVoice);
        */
    
      // append the harmony to the part harmony voice
      if (gGeneralOptions->fTraceHarmonies || gGeneralOptions->fTraceParts)
        gLogIos <<
          "Appending harmony '" <<
          harmony->harmonyAsString () <<
          "' to part clone " <<
          getPartCombinedName () <<
          ", line " << inputLineNumber <<
          endl;
    
      fPartHarmonyVoice->
        appendHarmonyToVoiceClone (harmony);
      break;
      
    case msrVoice::kMasterVoice:
    case msrVoice::kRegularVoice:
    case msrVoice::kFiguredBassVoice:
    case msrVoice::kSilentVoice:
      {
        stringstream s;
    
        s <<
          "harmonies cannot by supplied to part clone by " <<
          msrVoice::voiceKindAsString (
            harmoniesSupplierVoice->getVoiceKind ()) <<
          " voice \" " <<
          harmoniesSupplierVoice->getVoiceName () <<
          "\"";
    
        msrInternalError (
          inputLineNumber,
          s.str ());
      }
      break;
  } // switch
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
      
      // register this voice as the part figured bass supplier voice
      setPartHarmoniesSupplierVoice (
        inputLineNumber,
        figuredBassSupplierVoice);
    
      // append the figured bass to the part figured bass voice
      if (gGeneralOptions->fTraceHarmonies || gGeneralOptions->fTraceParts)
        gLogIos <<
          "Appending figured bass '" <<
          figuredBass->figuredBassAsString () <<
          "' to part " <<
          getPartCombinedName () <<
          ", line " << inputLineNumber <<
          endl;
    
      fPartFiguredBassVoice->
        appendFiguredBassToVoice (figuredBass);
      break;
      
    case msrVoice::kMasterVoice:
    case msrVoice::kHarmonyVoice:
    case msrVoice::kFiguredBassVoice:
    case msrVoice::kSilentVoice:
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
          inputLineNumber,
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
      if (gGeneralOptions->fTraceFiguredBass || gGeneralOptions->fTraceParts)
        gLogIos <<
          "Appending figured bass '" <<
          figuredBass->figuredBassAsString () <<
          "' to part clone " <<
          getPartCombinedName () <<
          ", line " << inputLineNumber <<
          endl;
    
      fPartFiguredBassVoice->
        appendFiguredBassToVoiceClone (figuredBass);
      break;
      
    case msrVoice::kMasterVoice:
    case msrVoice::kRegularVoice:
    case msrVoice::kHarmonyVoice:
    case msrVoice::kSilentVoice:
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
          inputLineNumber,
          s.str ());
      }
      break;
  } // switch
}

void msrPart::appendAccordionRegistrationToPart (
  S_msrAccordionRegistration
    accordionRegistration)
{
  if (gGeneralOptions->fTraceGeneral || gGeneralOptions->fTraceParts)
    gLogIos <<
      "Appending accordion registration '" <<
      accordionRegistration->accordionRegistrationAsString () <<
      "' to part " <<
      getPartCombinedName () <<
      endl;

  for (
    map<int, S_msrStaff>::const_iterator i = fPartStavesMap.begin();
    i != fPartStavesMap.end();
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
  if (gGeneralOptions->fTraceGeneral || gGeneralOptions->fTraceParts)
    gLogIos <<
      "Appending harp pedals tuning '" <<
      harpPedalsTuning->harpPedalsTuningAsString () <<
      "' to part " <<
      getPartCombinedName () <<
      endl;

  for (
    map<int, S_msrStaff>::const_iterator i = fPartStavesMap.begin();
    i != fPartStavesMap.end();
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
    gGeneralOptions->fTraceParts
      ||
    gGeneralOptions->fTraceDivisions
      ||
    gGeneralOptions->fTraceMeasures) {
    gLogIos <<
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
  bringPartToMeasureLength (
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
    gGeneralOptions->fTraceParts
      ||
    gGeneralOptions->fTraceDivisions
      ||
    gGeneralOptions->fTraceMeasures) {
    gLogIos <<
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
  bringPartToMeasureLength (
    inputLineNumber,
    measurePosition);
    */
}

void msrPart::appendSkipGraceNotesToVoicesClones (
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
        
  for (
    map<int, S_msrStaff>::const_iterator i=fPartStavesMap.begin();
    i!=fPartStavesMap.end();
    i++) {

    map<int, S_msrVoice>
      staffAllVoicesMap =
        (*i).second->
          getStaffAllVoicesMap ();
          
    for (
      map<int, S_msrVoice>::const_iterator j=staffAllVoicesMap.begin();
      j!=staffAllVoicesMap.end();
      j++) {

      S_msrVoice voice = (*j).second;
      
      if (voice != graceNotesOriginVoice) {
        voice->
          appendAFirstMeasureToVoiceIfNotYetDone ( // JMI
            inputLineNumber);
            
        // bring voice to the same measure length as graceNotesOriginVoice
        voice->
          bringVoiceToMeasureLength (
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

void msrPart::finalizeCurrentMeasureInPart (
  int    inputLineNumber)
{
  if (gGeneralOptions->fTraceMeasures || gGeneralOptions->fTraceParts) {
    gLogIos <<
      "Finalizing current measure in part " <<
      getPartCombinedName () <<
      ", line " << inputLineNumber <<
      endl;
  }

  // finalize current measure in master staff specifically
  fPartMasterStaff->
    finalizeCurrentMeasureInStaff (
      inputLineNumber);
  
  // finalize current measure in registered staves
  for (
    map<int, S_msrStaff>::const_iterator i = fPartStavesMap.begin();
    i != fPartStavesMap.end();
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

void msrPart::finalizePart (int inputLineNumber)
{
  if (gGeneralOptions->fTraceParts) {
    gLogIos <<
      "Finalizing part " <<
      getPartCombinedName () <<
      ", line " << inputLineNumber <<
      endl;
  }

  // finalize master staff specifically
  fPartMasterStaff->
    finalizeStaff (
      inputLineNumber);

  // finalize registered staves
  for (
    map<int, S_msrStaff>::const_iterator i = fPartStavesMap.begin();
    i != fPartStavesMap.end();
    i++) {
    (*i).second->
      finalizeStaff (
        inputLineNumber);
  } // for
}

void msrPart::acceptIn (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors)
    gLogIos <<
      "% ==> msrPart::acceptIn()" <<
      endl;
      
  if (visitor<S_msrPart>*
    p =
      dynamic_cast<visitor<S_msrPart>*> (v)) {
        S_msrPart elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors)
          gLogIos <<
            "% ==> Launching msrPart::visitStart()" <<
             endl;
        p->visitStart (elem);
  }
}

void msrPart::acceptOut (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors)
    gLogIos <<
      "% ==> msrPart::acceptOut()" <<
      endl;

  if (visitor<S_msrPart>*
    p =
      dynamic_cast<visitor<S_msrPart>*> (v)) {
        S_msrPart elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors)
          gLogIos <<
            "% ==> Launching msrPart::visitEnd()" <<
            endl;
        p->visitEnd (elem);
  }
}

void msrPart::browseData (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors)
    gLogIos <<
      "% ==> msrPart::browseData()" <<
      endl;

  // don't browse the harmony staff specifically,
  // it's part of fPartStavesMap
  /* JMI
  msrBrowser<msrStaff> browser (v);
  browser.browse (*fPartHarmonyStaff);
*/

  // browse all non figured bass staves
  for (
    map<int, S_msrStaff>::const_iterator i = fPartStavesMap.begin();
    i != fPartStavesMap.end();
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
      fPartStavesMap.size(), "staff", "staves") <<
    ")" <<
    endl;
    
  idtr++;
  
  const int fieldWidth = 19;

  os << left <<
    setw (fieldWidth) <<
    "PartPartGroupUplink" << ": \"" <<
    fPartPartGroupUplink->getPartGroupName () << "\"" <<
    endl <<
    setw (fieldWidth) <<
    "PartMsrName" << ": \"" <<
    fPartMsrName << "\"" <<
    endl <<
    setw (fieldWidth) <<
    "PartName" << ": \"" <<
    fPartName << "\"" <<
    endl <<
    setw (fieldWidth) <<
    "PartAbbrevation" << ": \"" <<
    fPartAbbreviation << "\"" <<
    endl <<
    setw (fieldWidth) <<
     "PartInstrumentName" << ": \"" <<
    fPartInstrumentName << "\"" <<
    endl <<
    endl <<
    setw (fieldWidth) <<
     "PartInstrumentAbbreviation" << ": \"" <<
    fPartInstrumentAbbreviation << "\"" <<
    endl <<
    endl;

/* JMI not specifically
  // print the harmony staff
  if (fPartHarmonyStaff) {
    os <<
      endl <<
      "Harmony staff:" <<
      endl;
            
    idtr++;
    os << fPartHarmonyStaff;
    idtr--;
  }
*/

  // print the master staff specifically
  os <<
    "Part master staff:" <<
    endl <<
    endl;
  idtr++;
  os <<
    fPartMasterStaff;
  idtr--;

  // print the registered staves
  if (fPartStavesMap.size()) {
    os <<
      "Other staves:" <<
      endl <<
      endl;

    idtr++;
    
    for (
      map<int, S_msrStaff>::const_iterator i = fPartStavesMap.begin();
      i != fPartStavesMap.end();
      i++) {
      S_msrStaff
        staff =
          (*i).second;
          
      msrStaff::msrStaffKind
        staffKind =
          staff->getStaffKind ();

      switch (staffKind) { // JMI
        case msrStaff::kMasterStaff:
          os <<
            staff;
          break;
          
        case msrStaff::kRegularStaff:
          os <<
            staff;
          break;
          
        case msrStaff::kTablatureStaff:
          os <<
            staff;
          break;
          
        case msrStaff::kPercussionStaff:
          os <<
            staff;
          break;
          
        case msrStaff::kHarmonyStaff:
    // JMI      if (gMsrOptions->fShowHarmonyVoices)
          os <<
            staff;
          break;
          
        case msrStaff::kFiguredBassStaff:
    // JMI      if (gMsrOptions->fShowHarmonyVoices)
          os <<
            staff;
          break;
      } // switch

      if (i != fPartStavesMap.end())
        switch (staffKind) {
          case msrStaff::kMasterStaff:
            os <<
              endl;
            break;
            
          case msrStaff::kRegularStaff:
            os <<
              endl;
            break;
            
          case msrStaff::kTablatureStaff:
            os <<
              endl;
            break;
            
          case msrStaff::kPercussionStaff:
            os <<
              endl;
            break;
            
          case msrStaff::kHarmonyStaff:
   // JMI         if (gMsrOptions->fShowHarmonyVoices)
              os <<
                endl;
            break;
            
          case msrStaff::kFiguredBassStaff:
   // JMI         if (gMsrOptions->fShowHarmonyVoices)
              os <<
                endl;
            break;
        } // switch
    } // for

    idtr--;
  }

  idtr--;
}

void msrPart::printStructure (ostream& os)
{
  os <<
    "Part" << " " << getPartCombinedName () <<
    " (" <<
    singularOrPlural (
      fPartStavesMap.size(), "staff", "staves") <<
    ", length high tide " << fPartMeasureLengthHighTide <<
    ")" <<
    endl;
    
  idtr++;
  
  const int fieldWidth = 27;

  os << left <<
    setw (fieldWidth) <<
    "PartMsrName" << ": \"" <<
    fPartMsrName << "\"" <<
    endl <<
    setw (fieldWidth) <<
    "PartName" << ": \"" <<
    fPartName << "\"" <<
    endl <<
    setw (fieldWidth) <<
    "PartAbbrevation" << ": \"" <<
    fPartAbbreviation << "\"" <<
    endl <<
    setw (fieldWidth) <<
    "PartInstrumentName" << ": \"" <<
    fPartInstrumentName << "\"" <<
    endl <<
    setw (fieldWidth) <<
     "PartInstrumentAbbreviation" << ": \"" <<
    fPartInstrumentAbbreviation << "\"" <<
    endl <<
    setw (fieldWidth) <<
     "PartInstrumentAbbreviation" << ": \"" <<
    fPartInstrumentAbbreviation << "\"" <<
    endl <<
    endl;

  // print the master staff specifically
  if (fPartMasterStaff) {
    os <<
      "Master staff" <<
      endl;
            
    idtr++;
    os <<
      fPartMasterStaff;
    idtr--;
  }

  // print the harmony staff // JMI specifically?
  if (fPartHarmonyStaff) {
    os <<
      "Harmony staff" <<
      endl;
            
    idtr++;
    os <<
      fPartHarmonyStaff;
    idtr--;
  }

  // print the staves
  if (fPartStavesMap.size()) {
    os << 
      "Other staves" <<
      endl;
              
    idtr++;
  
    map<int, S_msrStaff>::const_iterator
      iBegin = fPartStavesMap.begin(),
      iEnd   = fPartStavesMap.end(),
      i      = iBegin;
          
    for ( ; ; ) {
      os <<
        idtr;
      (*i).second->printStructure (os);
      if (++i == iEnd) break;
      os << endl;
    } // for

    idtr--;
  }

  idtr--;
}

//______________________________________________________________________________
int msrPartGroup::gPartGroupsCounter = 0;

S_msrPartGroup msrPartGroup::create (
  int                    inputLineNumber,
  int                    partGroupNumber,
  string                 partGroupName,
  string                 partGroupDisplayText,
  string                 partGroupAccidentalText,
  string                 partGroupAbbreviation,
  msrPartGroupSymbolKind partGroupSymbolKind,
  int                    partGroupSymbolDefaultX,
  bool                   partGroupBarline,
  S_msrPartGroup         partGroupPartGroupUplink,
  S_msrScore             partGroupScoreUplink)
{
  msrPartGroup* o =
    new msrPartGroup (
      inputLineNumber,
      partGroupNumber,
      partGroupName,
      partGroupDisplayText,
      partGroupAccidentalText,
      partGroupAbbreviation,
      partGroupSymbolKind,
      partGroupSymbolDefaultX,
      partGroupBarline,
      partGroupPartGroupUplink,
      partGroupScoreUplink);
  assert(o!=0);
  return o;
}

msrPartGroup::msrPartGroup (
  int                    inputLineNumber,
  int                    partGroupNumber,
  string                 partGroupName,
  string                 partGroupDisplayText,
  string                 partGroupAccidentalText,
  string                 partGroupAbbreviation,
  msrPartGroupSymbolKind partGroupSymbolKind,
  int                    partGroupSymbolDefaultX,
  bool                   partGroupBarline,
  S_msrPartGroup         partGroupPartGroupUplink,
  S_msrScore             partGroupScoreUplink)
    : msrElement (inputLineNumber)
{
  // generate a new partGroup absolute number
  fPartGroupAbsoluteNumber = ++gPartGroupsCounter;
  
  fPartGroupNumber = partGroupNumber;
        
  fPartGroupName = partGroupName;

  fPartGroupDisplayText    = partGroupDisplayText;
  fPartGroupAccidentalText = partGroupAccidentalText;
  
  fPartGroupAbbreviation = partGroupAbbreviation;

  fPartGroupSymbolKind     = partGroupSymbolKind;
  fPartGroupSymbolDefaultX = partGroupSymbolDefaultX;

  fPartGroupBarline = partGroupBarline;

  fPartGroupPartGroupUplink = partGroupPartGroupUplink;

  fPartGroupScoreUplink = partGroupScoreUplink;
  
  if (gGeneralOptions->fTracePartGroups)
    gLogIos <<
      "--------------------------------------------" <<
      endl <<
      "==> Creating part group " << fPartGroupNumber <<
      endl;
}

msrPartGroup::~msrPartGroup()
{}

S_msrPartGroup msrPartGroup::createPartGroupNewbornClone (
  S_msrPartGroup partGroupClone,
  S_msrScore     scoreClone)
{
  if (gGeneralOptions->fTracePartGroups)
    gLogIos <<
      "--------------------------------------------" <<
      endl <<
      "==> Creating a newborn clone part group " <<
      getPartGroupCombinedName () <<
      endl;

  // don't check against 0, since the partGroup stack JMI
  // that it comes from may be empty
/* JMI
  // sanity check
  msrAssert(
    partGroupClone != 0,
    "partGroupClone is null");
    */
    
  S_msrPartGroup
    newbornClone =
      msrPartGroup::create (
        fInputLineNumber,
        fPartGroupNumber,
        fPartGroupName,
        fPartGroupDisplayText,
        fPartGroupAccidentalText,
        fPartGroupAbbreviation,
        fPartGroupSymbolKind,
        fPartGroupSymbolDefaultX,
        fPartGroupBarline,
        partGroupClone,
        scoreClone);

  // avoid part group newborn clone to keep its (new) absolute number
  newbornClone->fPartGroupAbsoluteNumber =
    fPartGroupAbsoluteNumber;
  
  newbornClone->fPartGroupInstrumentName =
    fPartGroupInstrumentName;
  
  return newbornClone;
}

string msrPartGroup::getPartGroupCombinedName () const
{
  stringstream s;

  s <<
    "PG_" << fPartGroupAbsoluteNumber <<
    " (" << fPartGroupNumber << ")";

  return s.str ();
}

S_msrPart msrPartGroup::addPartToPartGroupByItsID (
  int    inputLineNumber,
  string partID)
{
  if (fPartGroupPartsMap.count (partID)) {
    stringstream s;

    s <<
      "partID \"" << partID <<
      "\" already exists in part group " <<
      getPartGroupCombinedName ();

    msrMusicXMLWarning ( // JMI
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
  if (gGeneralOptions->fTracePartGroups) {
    gLogIos <<
      "Adding part " <<
      part->getPartCombinedName () <<
      " to part group " << fPartGroupNumber <<
      endl;
  }
  
  fPartGroupPartsMap [partID] = part;
  fPartGroupElements.push_back (part);

  if (gGeneralOptions->fTracePartGroups) {
    gLogIos <<
      endl <<
      "After addPartToPartGroupByItsID, fPartGroupPartsMap contains:" <<
      endl;
        
    idtr++;
    
    for (
        map<string, S_msrPart>::const_iterator i = fPartGroupPartsMap.begin();
        i != fPartGroupPartsMap.end();
        i++) {
      gLogIos <<
        "\"" << (*i).first << "\" --% --> " <<
        (*i).second->getPartCombinedName() <<
        endl;
    } // for
    
    idtr--;
    
    gLogIos <<
 // JMI     "% ==> addPartToPartGroup" <<
      endl << endl <<

    "After addPartToPartGroupByItsID, fPartGroupPartsList contains:" <<
    endl;
      
    if (fPartGroupElements.size()) {
      list<S_msrElement>::const_iterator
        iBegin = fPartGroupElements.begin(),
        iEnd   = fPartGroupElements.end(),
        i      = iBegin;
        
      idtr++;
      for ( ; ; ) {
        gLogIos << (*i);
        if (++i == iEnd) break;
        gLogIos << endl;
      } // for
      idtr--;
    }
    
    gLogIos <<
  // JMI    "% ==> addPartToPartGroupByItsID" <<
      endl << endl;
  }
  
  // return the part
  return part;
}

void msrPartGroup::addPartToPartGroup (S_msrPart part)
{
  // register part in this part group
  if (gGeneralOptions->fTracePartGroups) {
    gLogIos <<
      "Adding part " <<
      part->getPartCombinedName () <<
      " to part group " << fPartGroupNumber <<
      endl;
  }
  
  fPartGroupPartsMap [part->getPartID ()] = part;
  fPartGroupElements.push_back (part);
}

void msrPartGroup::prependSubPartGroupToPartGroup (
  S_msrPartGroup partGroup)
{
  if (gGeneralOptions->fTracePartGroups)
    gLogIos <<
      "Prepending (sub-)part group " << partGroup->getPartGroupNumber () <<
      " to part group " << getPartGroupNumber ()  << endl;

  // register it in this part group
  fPartGroupElements.push_front (partGroup);
}

void msrPartGroup::appendSubPartGroupToPartGroup (
  S_msrPartGroup partGroup)
{
  if (gGeneralOptions->fTracePartGroups)
    gLogIos <<
      "Appending (sub-)part group " << partGroup->getPartGroupNumber () <<
      " to part group " << getPartGroupNumber ()  << endl;

  // register it in this part group
  fPartGroupElements.push_back (partGroup);
}

S_msrPart msrPartGroup::fetchPartFromPartGroup (
  string partID)
{
  if (gGeneralOptions->fTracePartGroups) {
    gLogIos <<
      "fetchPartFromPartGroup(), fPartGroupPartsMap contains:" << endl;
    for (
        map<string, S_msrPart>::const_iterator i = fPartGroupPartsMap.begin();
        i != fPartGroupPartsMap.end();
        i++) {
      gLogIos <<
        (*i).first << " --% --> " <<
        (*i).second->getPartCombinedName() <<
        endl;
    } // for
    gLogIos <<
  // JMI    "% ==> fetchPartFromPartGroup" <<
      endl;
  }
  
  S_msrPart result;
  
  if (fPartGroupPartsMap.count (partID)) {
    result = fPartGroupPartsMap [partID];
  }

  return result;
}

void msrPartGroup::acceptIn (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors)
    gLogIos <<
      "% ==> msrPartGroup::acceptIn()" <<
      endl;
      
  if (visitor<S_msrPartGroup>*
    p =
      dynamic_cast<visitor<S_msrPartGroup>*> (v)) {
        S_msrPartGroup elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors)
          gLogIos <<
            "% ==> Launching msrPartGroup::visitStart()" <<
             endl;
        p->visitStart (elem);
  }
}

void msrPartGroup::acceptOut (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors)
    gLogIos <<
      "% ==> msrPartGroup::acceptOut()" <<
      endl;

  if (visitor<S_msrPartGroup>*
    p =
      dynamic_cast<visitor<S_msrPartGroup>*> (v)) {
        S_msrPartGroup elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors)
          gLogIos <<
            "% ==> Launching msrPartGroup::visitEnd()" <<
            endl;
        p->visitEnd (elem);
  }
}

void msrPartGroup::browseData (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors)
    gLogIos <<
      "% ==> msrPartGroup::browseData()" <<
      endl;
  
  for (
    list<S_msrElement>::const_iterator i = fPartGroupElements.begin();
    i != fPartGroupElements.end();
    i++) {
    // browse the part element
    msrBrowser<msrElement> browser (v);
    browser.browse (*(*i));
  } // for

  if (gMsrOptions->fTraceMsrVisitors)
    gLogIos <<
      "% <== msrPartGroup::browseData()" <<
      endl;
}

ostream& operator<< (ostream& os, const S_msrPartGroup& elt)
{
  elt->print (os);
  return os;
}

string msrPartGroup::partGroupSymbolKindAsString (
  msrPartGroupSymbolKind partGroupSymbolKind)
{
  string result;
  
  switch (partGroupSymbolKind) {
    case k_NoPartGroupSymbol:
      result = "none";
      break;
    case kBracePartGroupSymbol:
      result = "brace";
      break;
    case kBracketPartGroupSymbol:
      result = "bracket";
      break;
    case kLinePartGroupSymbol:
      result = "line";
      break;
    case kSquarePartGroupSymbol:
      result = "square";
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
      fPartGroupPartsMap.size(), "part", "parts") <<
    ")" <<
    endl;
    
  idtr++;

  const int fieldWidth = 23;
  
  os << left <<
    setw (fieldWidth) << "PartGroupName" << " : \"" <<
    fPartGroupName <<
    "\"" <<
    endl <<
    setw (fieldWidth) << "PartGroupDisplayText" << " : \"" <<
    fPartGroupDisplayText <<
    "\"" <<
    endl <<
    setw (fieldWidth) << "PartGroupAccidentalText" << " : \"" <<
    fPartGroupAccidentalText <<
    "\"" <<
    endl <<
    setw (fieldWidth) << "PartGroupAbbrevation" << " : \"" <<
    fPartGroupAbbreviation <<
    "\"" <<
    endl <<
    setw (fieldWidth) << "PartGroupSymbolDefaultX" << " : " <<
    fPartGroupSymbolDefaultX <<
      endl <<
    setw (fieldWidth) << "PartGroupSymbolKind" << " : " <<
    partGroupSymbolKindAsString (fPartGroupSymbolKind) <<
    endl;
    
  os << left <<
    setw (fieldWidth) << "PartGroupBarline" << " : ";
  if (fPartGroupBarline)
    os << "true";
  else
    os << "false";
  os << endl;

  os << left <<
    setw (fieldWidth) << "PartGroupPartGroupUplink" << " : ";
  if (fPartGroupPartGroupUplink)
    os <<
      "\"" <<
      fPartGroupPartGroupUplink->
        getPartGroupName () <<
      "\"";
  else
    os << "none";
  os << endl;

  if (fPartGroupElements.size()) {
    os << endl;
    list<S_msrElement>::const_iterator
      iBegin = fPartGroupElements.begin(),
      iEnd   = fPartGroupElements.end(),
      i      = iBegin;
      
    for ( ; ; ) {
      os << (*i);
      if (++i == iEnd) break;
      os << endl;
    } // for
  }
  
  idtr--;
}

void msrPartGroup::printStructure (ostream& os)
{
  os <<
    "PartGroup" " \"" << getPartGroupCombinedName () <<
    "\" (" <<
    singularOrPlural (
      fPartGroupPartsMap.size(), "part", "parts") <<
    ")" <<
    endl;
    
  idtr++;

  const int fieldWidth = 24;
  
  os << left <<
    setw (fieldWidth) << "PartGroupName" << " : \"" <<
    fPartGroupName <<
    "\"" <<
    endl <<
    setw (fieldWidth) << "PartGroupAbbrevation" << " : \"" <<
    fPartGroupAbbreviation <<
    "\"" <<
    endl <<
    setw (fieldWidth) << "fPartGroupSymbolDefaultX" << " : " <<
    fPartGroupSymbolDefaultX <<
      endl <<
    setw (fieldWidth) << "fPartGroupSymbolKind" << " : \"" <<
    partGroupSymbolKindAsString (fPartGroupSymbolKind) <<
    "\"" <<
    endl;
    
  os <<
    "PartGroupBarline         : ";
  if (fPartGroupBarline)
    os << "true";
  else
    os << "false";
  os << endl;

  if (fPartGroupElements.size()) {
    os << endl;
    list<S_msrElement>::const_iterator
      iBegin = fPartGroupElements.begin(),
      iEnd   = fPartGroupElements.end(),
      i      = iBegin;
      
    idtr++;
    for ( ; ; ) {
      os << idtr;
      (*i)->printStructure (os);
      if (++i == iEnd) break;
      os << endl;
    } // for
    idtr--;
  }
  
  idtr--;
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
      "work-number", val);
  }

void msrIdentification::setWorkTitle (
  int    inputLineNumber,
  string val)
  {
  fWorkTitle =
    msrVarValAssoc::create (
      inputLineNumber,
      "work-title", val);
  }

void msrIdentification::setMovementNumber (
  int    inputLineNumber,
  string val)
  {
  fMovementNumber =
    msrVarValAssoc::create (
      inputLineNumber,
      "movement-number", val);
  }

void msrIdentification::setMovementTitle (
  int    inputLineNumber,
  string val)
{
  fMovementTitle =
    msrVarValAssoc::create (
      inputLineNumber,
      "movement-title", val);
}

void msrIdentification::addComposer (
  int    inputLineNumber,
  string type,
  string val)
{
  fComposers.push_back(
    msrVarValAssoc::create (
      inputLineNumber,
      type, val)
  );
}

void msrIdentification::addArranger (
  int    inputLineNumber,
  string type,
  string val)
{
  fArrangers.push_back(
    msrVarValAssoc::create (
      inputLineNumber,
      type, val)
  );
}

void msrIdentification::addPoet (
  int    inputLineNumber,
  string type,
  string val)
{
  fPoets.push_back(
    msrVarValAssoc::create (
      inputLineNumber,
      type, val)
  );
}

void msrIdentification::addLyricist (
  int    inputLineNumber,
  string type,
  string val)
{
  fLyricists.push_back(
    msrVarValAssoc::create (
      inputLineNumber,
      type, val)
  );
}

void msrIdentification::setRights (
  int    inputLineNumber,
  string val)
  {
  fRights =
    msrVarValAssoc::create (
      inputLineNumber,
      "rights", val);
  }

void msrIdentification::addSoftware (
  int    inputLineNumber,
  string val)
{
  fSoftwares.push_back(
    msrVarValAssoc::create (
      inputLineNumber,
      "software", val)
  );
}

void msrIdentification::setEncodingDate (
  int    inputLineNumber,
  string val)
{
  fEncodingDate =
    msrVarValAssoc::create (
      inputLineNumber,
      "encoding-date", val);
}

void msrIdentification::setScoreInstrumentAssoc (
  int    inputLineNumber,
  string val)
{
  fScoreInstrumentAssoc =
    msrVarValAssoc::create (
      inputLineNumber,
      "score-instrument", val);
}

void msrIdentification::acceptIn (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors)
    gLogIos <<
      "% ==> msrIdentification::acceptIn()" <<
      endl;
      
  if (visitor<S_msrIdentification>*
    p =
      dynamic_cast<visitor<S_msrIdentification>*> (v)) {
        S_msrIdentification elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors)
          gLogIos <<
            "% ==> Launching msrIdentification::visitStart()" <<
             endl;
        p->visitStart (elem);
  }
}

void msrIdentification::acceptOut (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors)
    gLogIos <<
      "% ==> msrIdentification::acceptOut()" <<
      endl;

  if (visitor<S_msrIdentification>*
    p =
      dynamic_cast<visitor<S_msrIdentification>*> (v)) {
        S_msrIdentification elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors)
          gLogIos <<
            "% ==> Launching msrIdentification::visitEnd()" <<
            endl;
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
    // fMovementTitle fMovementTitle
    msrBrowser<msrVarValAssoc> browser (v);
    browser.browse (*fMovementTitle);
  }

  if (!fComposers.empty()) {
    vector<S_msrVarValAssoc>::const_iterator i;
    for (i=fComposers.begin(); i!=fComposers.end(); i++) {
      // browse creator
      msrBrowser<msrVarValAssoc> browser (v);
      browser.browse (*(*i));
    } // for
  }
    
  if (!fArrangers.empty()) {
    vector<S_msrVarValAssoc>::const_iterator i;
    for (i=fArrangers.begin(); i!=fArrangers.end(); i++) {
      // browse creator
      msrBrowser<msrVarValAssoc> browser (v);
      browser.browse (*(*i));
    } // for
  }
    
  if (!fPoets.empty()) {
    vector<S_msrVarValAssoc>::const_iterator i;
    for (i=fPoets.begin(); i!=fPoets.end(); i++) {
      // browse creator
      msrBrowser<msrVarValAssoc> browser (v);
      browser.browse (*(*i));
    } // for
  }
    
  if (!fLyricists.empty()) {
    vector<S_msrVarValAssoc>::const_iterator i;
    for (i=fLyricists.begin(); i!=fLyricists.end(); i++) {
      // browse creator
      msrBrowser<msrVarValAssoc> browser (v);
      browser.browse (*(*i));
    } // for
  }
    
  if (fRights) {
    // browse rights
    msrBrowser<msrVarValAssoc> browser (v);
    browser.browse (*fRights);
  }

  if (!fSoftwares.empty()) {
    vector<S_msrVarValAssoc>::const_iterator i;
    for (i=fSoftwares.begin(); i!=fSoftwares.end(); i++) {
      // browse software
      msrBrowser<msrVarValAssoc> browser (v);
      browser.browse (*(*i));
    } // for
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

  idtr++;
  
  if (fWorkNumber) {
    os << fWorkNumber;
    emptyIdentification = false;
  }
  
  if (fWorkTitle) {
    os << fWorkTitle;
    emptyIdentification = false;
  }
    
  if (fMovementNumber) {
    os << fMovementNumber;
    emptyIdentification = false;
  }
    
  if (fMovementTitle) {
    os << fMovementTitle;
    emptyIdentification = false;
  }

  if (! fComposers.empty()) {
    vector<S_msrVarValAssoc>::const_iterator
      iBegin = fComposers.begin(),
      iEnd   = fComposers.end(),
      i      = iBegin;
    for ( ; ; ) {
      os << (*i);
      if (++i == iEnd) break;
      os << endl;
    } // for

    emptyIdentification = false;
  }
    
  if (! fArrangers.empty()) {
    vector<S_msrVarValAssoc>::const_iterator
      iBegin = fArrangers.begin(),
      iEnd   = fArrangers.end(),
      i      = iBegin;
    for ( ; ; ) {
      os << (*i);
      if (++i == iEnd) break;
      os << endl;
    } // for

    emptyIdentification = false;
  }
    
  if (! fPoets.empty()) {
    vector<S_msrVarValAssoc>::const_iterator
      iBegin = fPoets.begin(),
      iEnd   = fPoets.end(),
      i      = iBegin;
    for ( ; ; ) {
      os << (*i);
      if (++i == iEnd) break;
      os << endl;
    } // for

    emptyIdentification = false;
  }
    
  if (! fLyricists.empty()) {
    vector<S_msrVarValAssoc>::const_iterator
      iBegin = fLyricists.begin(),
      iEnd   = fLyricists.end(),
      i      = iBegin;
    for ( ; ; ) {
      os << (*i);
      if (++i == iEnd) break;
      os << endl;
    } // for

    emptyIdentification = false;
  }
    
  if (fRights) {
    os << fRights;
    emptyIdentification = false;
  }
    
  if (!fSoftwares.empty()) {
    vector<S_msrVarValAssoc>::const_iterator
      iBegin = fSoftwares.begin(),
      iEnd   = fSoftwares.end(),
      i      = iBegin;
    for ( ; ; ) {
      os << (*i);
      if (++i == iEnd) break;
      os << endl;
    } // for

    emptyIdentification = false;
  }
    
  if (fEncodingDate) {
    os << fEncodingDate;
    emptyIdentification = false;
  }

  if (emptyIdentification)
    os <<
      " " << "nothing specified" << endl;

// JMI  os << endl;
  
  idtr--;
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

  // measure repeats replicas should be browsed by default
  fInhibitMeasureRepeatReplicasBrowsing = false;

  // multiple rest measures should be browsed by default
  fInhibitMultipleRestMeasuresBrowsing = false;
}

msrScore::~msrScore()
{}

S_msrScore msrScore::createScoreNewbornClone ()
{
  if (gGeneralOptions->fTraceScore) {
    gLogIos <<
      "==> Creating a newborn clone of a score" <<
      endl;
  }

  S_msrScore
    newbornClone =
      msrScore::create (
        fInputLineNumber);

/* JMI
  for (
    list<S_msrPartGroup>::const_iterator i = fCreditsList.begin();
    i != fCreditsList.end();
    i++) {
    // append the credit to the clone
    appendCreditToScore (*i);
  } // for
*/

  newbornClone->fInhibitMeasureRepeatReplicasBrowsing =
    fInhibitMeasureRepeatReplicasBrowsing;
    
  newbornClone->fInhibitMultipleRestMeasuresBrowsing =
    fInhibitMultipleRestMeasuresBrowsing;
    
  return newbornClone;
}

void msrScore::addPartGroupToScore (S_msrPartGroup partGroup)
{
  /* JMI
  if (fScorePartGroupsMap.count (partGroupNumber)) {
    stringstream s;
    
    s <<      
      "part group " << partGroupNumber <<
      " already exists in this score";

    msrInternalError ( // JMI
      inputLineNumber, s.str ());

    return fScorePartGroupsMap [partGroupNumber];
  }
*/

  // register it in this score
  fPartGroupsList.push_back (partGroup);
}

void msrScore::appendCreditToScore (S_msrCredit credit)
{
  if (gGeneralOptions->fTraceCredits) {
    gLogIos <<
      "Appending credit '" <<
      credit->creditAsString () <<
      "' to score" <<
      endl;
  }
  
  fCreditsList.push_back (credit);
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

void msrScore::acceptIn (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors)
    gLogIos <<
      "% ==> msrScore::acceptIn()" <<
      endl;
      
  if (visitor<S_msrScore>*
    p =
      dynamic_cast<visitor<S_msrScore>*> (v)) {
        S_msrScore elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors)
          gLogIos <<
            "% ==> Launching msrScore::visitStart()" <<
             endl;
        p->visitStart (elem);
  }
}

void msrScore::acceptOut (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors)
    gLogIos <<
      "% ==> msrScore::acceptOut()" <<
      endl;

  if (visitor<S_msrScore>*
    p =
      dynamic_cast<visitor<S_msrScore>*> (v)) {
        S_msrScore elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors)
          gLogIos <<
            "% ==> Launching msrScore::visitEnd()" <<
            endl;
        p->visitEnd (elem);
  }
}

void msrScore::browseData (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors)
    gLogIos <<
      "% ==> msrScore::browseData()" <<
      endl;
    
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
    list<S_msrCredit>::const_iterator i = fCreditsList.begin();
    i != fCreditsList.end();
    i++) {
    // browse the part credit
    msrBrowser<msrCredit> browser (v);
    browser.browse (*(*i));
  } // for

  for (
    list<S_msrPartGroup>::const_iterator i = fPartGroupsList.begin();
    i != fPartGroupsList.end();
    i++) {
    // browse the part group
    msrBrowser<msrPartGroup> browser (v);
    browser.browse (*(*i));
  } // for

  if (gMsrOptions->fTraceMsrVisitors)
    gLogIos <<
      "% <== msrScore::browseData()" <<
      endl;
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
    " (" <<
    singularOrPlural (
      fPartGroupsList.size(),
      "part group",
      "part groups") <<
    ")" <<
    endl << endl;

  idtr++;
  
  // print the identification if any
  if (fIdentification) {
    os <<
      fIdentification;
  }
  
  // print the geometry if any
  if (fPageGeometry) {
    os <<
      fPageGeometry;
  }

  // print the credits if any
  if (fCreditsList.size ()) {
    list<S_msrCredit>::const_iterator
      iBegin = fCreditsList.begin(),
      iEnd   = fCreditsList.end(),
      i      = iBegin;
    for ( ; ; ) {
      os << (*i);
      if (++i == iEnd) break;
      os << endl;
    } // for
  }

  // print the part groups
  list<S_msrPartGroup>::const_iterator
    iBegin = fPartGroupsList.begin(),
    iEnd   = fPartGroupsList.end(),
    i      = iBegin;
  for ( ; ; ) {
    os << (*i);
    if (++i == iEnd) break;
    os << endl;
  } // for
  
  idtr--;
}

void msrScore::printStructure (ostream& os)
{
  os <<
    "MSR component" <<
    " (" <<
    singularOrPlural (
      fPartGroupsList.size(),
      "part group",
      "part groups") <<
    ")" <<
    ", parts and staves not shown" <<
    endl << endl;

  idtr++;
  
  if (fIdentification) {
    os << fIdentification;
  }
  
  if (fPageGeometry) {
    os << fPageGeometry;
  }
  
  // print the credits if any
  if (fCreditsList.size ()) {
    list<S_msrCredit>::const_iterator
      iBegin = fCreditsList.begin(),
      iEnd   = fCreditsList.end(),
      i      = iBegin;
    for ( ; ; ) {
      os << (*i);
      if (++i == iEnd) break;
      os << endl;
    } // for
  }
  
  for (
    list<S_msrPartGroup>::const_iterator i = fPartGroupsList.begin();
    i != fPartGroupsList.end();
    i++) {
    os << idtr;
    (*i)->
      printStructure (os);
  } // for
  
  idtr--;
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

void msrMidi::acceptIn (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors)
    gLogIos <<
      "% ==> msrMidi::acceptIn()" <<
      endl;
      
  if (visitor<S_msrMidi>*
    p =
      dynamic_cast<visitor<S_msrMidi>*> (v)) {
        S_msrMidi elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors)
          gLogIos <<
            "% ==> Launching msrMidi::visitStart()" <<
             endl;
        p->visitStart (elem);
  }
}

void msrMidi::acceptOut (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors)
    gLogIos <<
      "% ==> msrMidi::acceptOut()" <<
      endl;

  if (visitor<S_msrMidi>*
    p =
      dynamic_cast<visitor<S_msrMidi>*> (v)) {
        S_msrMidi elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors)
          gLogIos <<
            "% ==> Launching msrMidi::visitEnd()" <<
            endl;
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

  idtr++;

  const int fieldWidth = 18;

  os << left <<
    setw (fieldWidth) <<
    "MidiTempoDuration" << " = " << fMidiTempoDuration <<
    endl <<
    setw (fieldWidth) <<
    "MidiTempoPerSecond" << " = " << fMidiTempoPerSecond <<
    endl <<
    endl;
      
  idtr--;
}    


/* JMI
void msrStaff::appendDivisionsToStaff (
  S_msrDivisions divisions)
{
  if (gGeneralOptions->fTraceDivisions || gGeneralOptions->fTraceStaves)
    gLogIos <<
      "Appending divisions '" <<
      divisions->divisionsAsString () <<
      "' to staff \"" <<
      getStaffName () <<
      "\" in part " <<
      fStaffPartUplink->getPartCombinedName () <<
      endl;

  // propagate it to all voices
  for (
    map<int, S_msrVoice>::const_iterator i = fStaffAllVoicesMap.begin();
    i != fStaffAllVoicesMap.end();
    i++) {
    (*i).second->
      appendDivisionsToVoice (divisions);
  } // for
}

void msrVoice::appendDivisionsToVoice (
  S_msrDivisions divisions)
{
  if (gGeneralOptions->fTraceDivisions || gGeneralOptions->fTraceVoices)
    gLogIos <<
      "Appending divisions '" <<
      divisions->divisionsAsString () <<
      "' to voice \"" << getVoiceName () << "\"" <<
      endl;

  // create the voice last segment and first measure if needed
  appendAFirstMeasureToVoiceIfNotYetDone (
    divisions->getInputLineNumber ());

  // append divisions to last segment
  fVoiceLastSegment->
    appendDivisionsToSegment (divisions);
}

void msrSegment::appendDivisionsToSegment (
  S_msrDivisions divisions)
{
  if (gGeneralOptions->fTraceDivisions || gGeneralOptions->fTraceSegments)
    gLogIos <<
      "Appending divisions '" <<
      divisions->divisionsAsString () <<
      " to segment " << segmentAsString () <<
      endl;
      
  // register divisions in segments's current measure
  fSegmentMeasuresList.back ()->
    appendDivisionsToMeasure (divisions);
}
    
void msrMeasure::appendDivisionsToMeasure (
  S_msrDivisions divisions)
{
  if (gGeneralOptions->fTraceDivisions || gGeneralOptions->fTraceSegments)
    gLogIos <<
      "Appending divisions '" <<
      divisions->divisionsAsString () <<
      " to measure '" << fMeasureNumber << "'" <<
      endl;
      
  // append it to the measure elements list
  fMeasureElementsList.push_back (divisions);
}
*/

//______________________________________________________________________________
void initializeMSR ()
{
  // enlist versions information
  // ------------------------------------------------------

  enlistVersion (
    "0.0.0", "somewhere in 2016",
    "Start as a clone of xml2guido");
    
  enlistVersion (
    "0.1.0", "01-OCT-2017",
    "First draft version");
  
  // languages handling
  // ------------------------------------------------------

  initializeQuarterTonesPitchesLanguages ();
  
  // options handling
  // ------------------------------------------------------

  initializeGeneralOptions ();
  initializeMusicXMLOptions ();

  // create the options variables
  // ------------------------------------------------------

  // MSR options
  
  gMsrOptionsUserChoices = msrOptions::create ();
  assert(gMsrOptionsUserChoices != 0);

  gMsrOptions =
    gMsrOptionsUserChoices;

  // prepare for measure detailed trace

  gMsrOptionsWithDetailedTrace =
    gMsrOptions->
      createCloneWithDetailedTrace ();

  // harmony handling
  // ------------------------------------------------------

  initializeChordIntervalsMap ();
  // JMI printChordIntervalsMap ();
}


}
