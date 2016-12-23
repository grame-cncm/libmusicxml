/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#include <iostream>
#include <list>
#include <algorithm>
#include <iomanip>      // setw, set::precision, ...

#include "msr.h"

using namespace std;

namespace MusicXML2 
{

//______________________________________________________________________________
// global variables


//_______________________________________________________________________________
void initializeStringToMsrNoteNamesLanguage ()
{
  gMsrNoteNamesLanguageMap["dutch"] =     kNederlands;
  gMsrNoteNamesLanguageMap["catalan"] =   kCatalan;
  gMsrNoteNamesLanguageMap["deutsch"] =   kDeutsch;
  gMsrNoteNamesLanguageMap["english"] =   kEnglish;
  gMsrNoteNamesLanguageMap["espanol"] =   kEspanol;
  gMsrNoteNamesLanguageMap["italiano"] =  kItaliano;
  gMsrNoteNamesLanguageMap["francais"] =  kFrancais;
  gMsrNoteNamesLanguageMap["norsk"] =     kNorsk;
  gMsrNoteNamesLanguageMap["portugues"] = kPortugues;
  gMsrNoteNamesLanguageMap["suomi"] =     kSuomi;
  gMsrNoteNamesLanguageMap["svenska"] =   kSvenska;
  gMsrNoteNamesLanguageMap["vlaams"] =    kVlaams;
}

msrNoteNamesLanguage getMsrNoteNamesLanguage (string lang)
{
  return gMsrNoteNamesLanguageMap[lang];
}

//_______________________________________________________________________________
S_msrOptions msrOptions::create()
{
  msrOptions* o = new msrOptions();
  assert(o!=0);
  return o;
}

msrOptions::msrOptions() {}

msrOptions::~msrOptions() {}

//______________________________________________________________________________
msrElement::msrElement (
  S_msrOptions& msrOpts, 
  int           inputLineNumber)
{
  fMsrOptions      = msrOpts;
  fInputLineNumber = inputLineNumber;  
}

msrElement::~msrElement() {}

void msrElement::acceptIn (basevisitor* v) {
  if (fMsrOptions->fDebugDebug)
    cerr <<
      "==> msrElement::acceptIn()" << endl;
      
  if (visitor<S_msrElement>*
    p =
      dynamic_cast<visitor<S_msrElement>*> (v)) {
        S_msrElement elem = this;
        
        if (fMsrOptions->fDebugDebug)
          cerr <<
            "==> Launching msrElement::visitStart()" << endl;
        p->visitStart (elem);
  }
}

void msrElement::acceptOut (basevisitor* v) {
  if (fMsrOptions->fDebugDebug)
    cerr <<
      "==> msrElement::acceptOut()" << endl;

  if (visitor<S_msrElement>*
    p =
      dynamic_cast<visitor<S_msrElement>*> (v)) {
        S_msrElement elem = this;
      
        if (fMsrOptions->fDebugDebug)
          cerr <<
            "==> Launching msrElement::visitEnd()" << endl;
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
ostream& operator<< (ostream& os, msrMusicXMLNoteData& musicXMLNoteData)
{
  musicXMLNoteData.print (os);
  return os;
}

msrMusicXMLNoteData::msrMusicXMLNoteData ()
{
  fMusicXMLStep = '?';
  fMusicXMLStepIsARest = false;;
  fMusicXMLStepIsUnpitched = false;;
  
  fMusicXMLAlteration = kNatural;
  
  fMusicXMLOctave = -1;

  // MusicXML durations are in divisions per quarter note.
  // LilyPond durations are in whole notes,
  // hence the "* 4" multiplications
  
  // the note duration when played
  fMusicXMLDivisions = -17;

  // tuplets member notes need another value for display
  fMusicXMLDisplayDivisions = 19;

  fMusicXMLDotsNumber = -1;
  
  fMusicXMLNoteIsAGraceNote = false;;
  
  fMusicXMLNoteBelongsToAChord = false;;
  
  fMusicXMLNoteBelongsToATuplet = false;;
  fMusicXMLType = "";

  fMusicXMLTieKind = k_NoTie;

  fMusicXMLVoiceNumber = -1;
}

string msrMusicXMLNoteData::musicXMLTieKindAsString () const
{
  string result;
  
  switch (fMusicXMLTieKind) {
    case kStartTie:
      result = "start tie";
      break;
    case kContinueTie:
      result = "continue tie";
      break;
    case kStopTie:
      result = "stop tie";
      break;
    case k_NoTie:
      result = "NO_TIE";
      break;
  } // switch

  return result;
}

void msrMusicXMLNoteData::print (ostream& os)
{
  os <<
    idtr << "  " << left <<
      setw(29) <<
      "fMusicXMLStep" << " = " << fMusicXMLStep <<  endl <<
    idtr << "  " << left <<
      setw(29) << "fMusicXMLStepIsARest" << " = " <<
      fMusicXMLStepIsARest <<  endl <<
    idtr << "  " << left <<
      setw(29) << "fMusicXMLAlteration" << " = " <<
      fMusicXMLAlteration <<  endl <<
    idtr << "  " << left <<
      setw(29) << "fMusicXMLOctave" << " = " <<
      fMusicXMLOctave <<  endl <<
    idtr << "  " << left <<
      setw(29) << "fMusicXMLDivisions" << " = " <<
      fMusicXMLDivisions <<  endl <<
    idtr << "  " << left <<
      setw(29) << "fMusicXMLDisplayDivisions" << " = " <<
      fMusicXMLDisplayDivisions <<  endl <<
    idtr << "  " << left <<
      setw(29) << "fMusicXMLDotsNumber" << " = " <<
      fMusicXMLDotsNumber <<  endl <<
      
//    idtr << endl <<
    
    idtr << "  " << left <<
      setw(29) << "fMusicXMLNoteBelongsToAChord" << " = " <<
      fMusicXMLNoteBelongsToAChord <<  endl <<
      
//    idtr << endl <<
    
    idtr << "  " << left <<
      setw(29) << "fMusicXMLNoteBelongsToATuplet" << " = " <<
      fMusicXMLNoteBelongsToATuplet <<  endl <<
    idtr << "  " << left <<
      setw(29) << "fMusicXMLType" << " = " <<
      fMusicXMLType <<  endl <<
      
//    idtr << endl <<
    
    idtr << "  " << left <<
      setw(29) << "fMusicXMLVoiceNumber" << " = " <<
      fMusicXMLVoiceNumber <<  endl;
};


//______________________________________________________________________________
void musicXMLBeatData::print (ostream& os)
{
  os <<
    "  " << left << setw(26) << "fBeatUnit = " << fBeatUnit <<  endl <<
    "  " << left << setw(26) << "fDots = " << fDots <<  endl;
};

//______________________________________________________________________________
S_msrOctaveShift msrOctaveShift::create (
  S_msrOptions&      msrOpts, 
  int                inputLineNumber,
  msrOctaveShiftKind octaveShiftKind,
  int                octaveShiftSize)
{
  msrOctaveShift* o =
    new msrOctaveShift (
      msrOpts, inputLineNumber, octaveShiftKind, octaveShiftSize);
  assert(o!=0);
  return o;
}

msrOctaveShift::msrOctaveShift (
  S_msrOptions&      msrOpts, 
  int                inputLineNumber,
  msrOctaveShiftKind octaveShiftKind,
  int                octaveShiftSize)
    : msrElement (msrOpts, inputLineNumber)
{
  fOctaveShiftKind = octaveShiftKind;

  fOctaveShiftSize = octaveShiftSize;
}

msrOctaveShift::~msrOctaveShift() {}

void msrOctaveShift::acceptIn (basevisitor* v) {
  if (fMsrOptions->fDebugDebug)
    cerr << idtr <<
      "==> msrOctaveShift::acceptIn()" << endl;
      
  if (visitor<S_msrOctaveShift>*
    p =
      dynamic_cast<visitor<S_msrOctaveShift>*> (v)) {
        S_msrOctaveShift elem = this;
        
        if (fMsrOptions->fDebugDebug)
          cerr << idtr <<
            "==> Launching msrOctaveShift::visitStart()" << endl;
        p->visitStart (elem);
  }
}

void msrOctaveShift::acceptOut (basevisitor* v) {
  if (fMsrOptions->fDebugDebug)
    cerr << idtr <<
      "==> msrOctaveShift::acceptOut()" << endl;

  if (visitor<S_msrOctaveShift>*
    p =
      dynamic_cast<visitor<S_msrOctaveShift>*> (v)) {
        S_msrOctaveShift elem = this;
      
        if (fMsrOptions->fDebugDebug)
          cerr << idtr <<
            "==> Launching msrOctaveShift::visitEnd()" << endl;
        p->visitEnd (elem);
  }
}

void msrOctaveShift::browseData (basevisitor* v)
{}

ostream& operator<< (ostream& os, const S_msrOctaveShift& beam)
{
  beam->print (os);
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
S_msrStem msrStem::create (
  S_msrOptions& msrOpts, 
  int           inputLineNumber,
  msrStemKind   stemKind)
{
  msrStem* o =
    new msrStem (
      msrOpts, inputLineNumber, stemKind);
  assert(o!=0);
  return o;
}

msrStem::msrStem (
  S_msrOptions& msrOpts, 
  int           inputLineNumber,
  msrStemKind   stemKind)
    : msrElement (msrOpts, inputLineNumber)
{
  fStemKind   = stemKind; 
}

msrStem::~msrStem() {}

void msrStem::acceptIn (basevisitor* v) {
  if (fMsrOptions->fDebugDebug)
    cerr << idtr <<
      "==> msrStem::acceptIn()" << endl;
      
  if (visitor<S_msrStem>*
    p =
      dynamic_cast<visitor<S_msrStem>*> (v)) {
        S_msrStem elem = this;
        
        if (fMsrOptions->fDebugDebug)
          cerr << idtr <<
            "==> Launching msrStem::visitStart()" << endl;
        p->visitStart (elem);
  }
}

void msrStem::acceptOut (basevisitor* v) {
  if (fMsrOptions->fDebugDebug)
    cerr << idtr <<
      "==> msrStem::acceptOut()" << endl;

  if (visitor<S_msrStem>*
    p =
      dynamic_cast<visitor<S_msrStem>*> (v)) {
        S_msrStem elem = this;
      
        if (fMsrOptions->fDebugDebug)
          cerr << idtr <<
            "==> Launching msrStem::visitEnd()" << endl;
        p->visitEnd (elem);
  }
}

void msrStem::browseData (basevisitor* v)
{}

ostream& operator<< (ostream& os, const S_msrStem& beam)
{
  beam->print (os);
  return os;
}

void msrStem::print (ostream& os)
{
  idtr++;
  
  os <<
    "Stem" <<
    ", kind: ";

  switch (fStemKind) {
    case k_NoStem:
      os << "none";
      break;
    case kStemUp:
      os << "up";
      break;
    case kStemDown:
      os << "down";
      break;
  } // switch
  os << endl;

  idtr--;
}

//______________________________________________________________________________
S_msrBeam msrBeam::create (
  S_msrOptions& msrOpts, 
  int           inputLineNumber,
  int           number,
  msrBeamKind   beamKind)
{
  msrBeam* o =
    new msrBeam (
      msrOpts, inputLineNumber, number, beamKind);
  assert(o!=0);
  return o;
}

msrBeam::msrBeam (
  S_msrOptions& msrOpts, 
  int           inputLineNumber,
  int           number,
  msrBeamKind   beamKind)
    : msrElement (msrOpts, inputLineNumber)
{
  fBeamNumber = number;
  fBeamKind   = beamKind; 
}

msrBeam::~msrBeam() {}

void msrBeam::acceptIn (basevisitor* v) {
  if (fMsrOptions->fDebugDebug)
    cerr << idtr <<
      "==> msrBeam::acceptIn()" << endl;
      
  if (visitor<S_msrBeam>*
    p =
      dynamic_cast<visitor<S_msrBeam>*> (v)) {
        S_msrBeam elem = this;
        
        if (fMsrOptions->fDebugDebug)
          cerr << idtr <<
            "==> Launching msrBeam::visitStart()" << endl;
        p->visitStart (elem);
  }
}

void msrBeam::acceptOut (basevisitor* v) {
  if (fMsrOptions->fDebugDebug)
    cerr << idtr <<
      "==> msrBeam::acceptOut()" << endl;

  if (visitor<S_msrBeam>*
    p =
      dynamic_cast<visitor<S_msrBeam>*> (v)) {
        S_msrBeam elem = this;
      
        if (fMsrOptions->fDebugDebug)
          cerr << idtr <<
            "==> Launching msrBeam::visitEnd()" << endl;
        p->visitEnd (elem);
  }
}

void msrBeam::browseData (basevisitor* v)
{}

ostream& operator<< (ostream& os, const S_msrBeam& beam)
{
  beam->print (os);
  return os;
}

void msrBeam::print (ostream& os)
{
  idtr++;
  
  os <<
    "Beam" <<
    " number " << fBeamNumber <<
    ", kind: ";

  switch (fBeamKind) {
    case kBeginBeam:
      os << "begin";
      break;
    case kContinueBeam:
      os << "continue";
      break;
    case kEndBeam:
      os << "end";
      break;
    case kForwardHookBeam:
      os << "forward";
      break;
    case kBackwardHookBeam:
      os << "backward";
      break;
    case k_NoBeam:
      os << "### none ###";
      break;
  } // switch
  os << endl;

  idtr--;
}

//______________________________________________________________________________
S_msrArticulation msrArticulation::create (
  S_msrOptions&       msrOpts, 
  int                 inputLineNumber,
  msrArticulationKind articulationKind)
{
  msrArticulation* o =
    new msrArticulation (
      msrOpts, inputLineNumber, articulationKind);
  assert (o!=0);
  return o;
}

msrArticulation::msrArticulation (
  S_msrOptions&       msrOpts, 
  int                 inputLineNumber,
  msrArticulationKind articulationKind)
    : msrElement (msrOpts, inputLineNumber)
{
  fArticulationKind = articulationKind;
}

msrArticulation::~msrArticulation() {}

string msrArticulation::articulationKindAsString () const
{
  string result;
  
  switch (fArticulationKind) {
    case kStaccato:
      result = "staccato";
      break;
    case kStaccatissimo:
      result = "staccatissimo";
      break;
    case kFermata:
      result = "fermata";
      break;
    case kTrill:
      result = "trill";
      break;
    case kArpeggiato:
      result = "arpeggiato";
      break;
  } // switch

  return result;
}

void msrArticulation::acceptIn (basevisitor* v) {
  if (fMsrOptions->fDebugDebug)
    cerr << idtr <<
      "==> msrArticulation::acceptIn()" << endl;
      
  if (visitor<S_msrArticulation>*
    p =
      dynamic_cast<visitor<S_msrArticulation>*> (v)) {
        S_msrArticulation elem = this;
        
        if (fMsrOptions->fDebugDebug)
          cerr << idtr <<
            "==> Launching msrArticulation::visitStart()" << endl;
        p->visitStart (elem);
  }
}

void msrArticulation::acceptOut (basevisitor* v) {
  if (fMsrOptions->fDebugDebug)
    cerr << idtr <<
      "==> msrArticulation::acceptOut()" << endl;

  if (visitor<S_msrArticulation>*
    p =
      dynamic_cast<visitor<S_msrArticulation>*> (v)) {
        S_msrArticulation elem = this;
      
        if (fMsrOptions->fDebugDebug)
          cerr << idtr <<
            "==> Launching msrArticulation::visitEnd()" << endl;
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
    "Articulation" << " " <<
    articulationKindAsString () <<
    endl;
}

//______________________________________________________________________________
S_msrRehearsal msrRehearsal::create (
  S_msrOptions&    msrOpts, 
  int              inputLineNumber,
  msrRehearsalKind rehearsalKind,
  string           rehearsalText)
{
  msrRehearsal* o =
    new msrRehearsal (
      msrOpts, inputLineNumber, rehearsalKind, rehearsalText);
  assert (o!=0);
  return o;
}

msrRehearsal::msrRehearsal (
  S_msrOptions&    msrOpts, 
  int              inputLineNumber,
  msrRehearsalKind rehearsalKind,
  string           rehearsalText)
    : msrElement (msrOpts, inputLineNumber)
{
  fRehearsalKind = rehearsalKind;

  fRehearsalText = rehearsalText;
}

msrRehearsal::~msrRehearsal() {}

void msrRehearsal::acceptIn (basevisitor* v) {
  if (fMsrOptions->fDebugDebug)
    cerr << idtr <<
      "==> msrRehearsal::acceptIn()" << endl;
      
  if (visitor<S_msrRehearsal>*
    p =
      dynamic_cast<visitor<S_msrRehearsal>*> (v)) {
        S_msrRehearsal elem = this;
        
        if (fMsrOptions->fDebugDebug)
          cerr << idtr <<
            "==> Launching msrRehearsal::visitStart()" << endl;
        p->visitStart (elem);
  }
}

void msrRehearsal::acceptOut (basevisitor* v) {
  if (fMsrOptions->fDebugDebug)
    cerr << idtr <<
      "==> msrRehearsal::acceptOut()" << endl;

  if (visitor<S_msrRehearsal>*
    p =
      dynamic_cast<visitor<S_msrRehearsal>*> (v)) {
        S_msrRehearsal elem = this;
      
        if (fMsrOptions->fDebugDebug)
          cerr << idtr <<
            "==> Launching msrRehearsal::visitEnd()" << endl;
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

void msrRehearsal::print (ostream& os)
{
  os <<
    "Rehearsal" << " " << fRehearsalText <<
    " kind: ";

  switch (fRehearsalKind) {
    case kNone:
      os << "none";
      break;
    case kRectangle:
      os << "rectangle";
      break;
    case kOval:
      os << "oval";
      break;
    case kCircle:
      os << "circle";
      break;
    case kBracket:
      os << "bracket";
      break;
    case kTriangle:
      os << "triangle";
      break;
    case kDiamond:
      os << "diamond";
      break;
  } // switch
  
  os << endl;
}

//______________________________________________________________________________
S_msrDynamics msrDynamics::create (
  S_msrOptions& msrOpts, 
  int                    inputLineNumber,
  msrDynamicsKind           dynamicsKind)
{
  msrDynamics* o =
    new msrDynamics (
      msrOpts, inputLineNumber, dynamicsKind);
    assert(o!=0);
  return o;
}

msrDynamics::msrDynamics (
  S_msrOptions& msrOpts, 
  int                    inputLineNumber,
  msrDynamicsKind           dynamicsKind)
    : msrElement (msrOpts, inputLineNumber)
{
  fDynamicsKind = dynamicsKind; 
}
msrDynamics::~msrDynamics() {}

string msrDynamics::dynamicsKindAsString ()
{
  stringstream s;
  
  switch (fDynamicsKind) {
    case kF:
      s << "f";
      break;
    case kFF:
      s << "ff"; break;
    case kFFF:
      s << "fff"; break;
    case kFFFF:
      s << "ffff"; break;
    case kFFFFF:
      s << "fffff"; break;
    case kFFFFFF:
      s << "ffffff"; break;

    case kP:
      s << "p"; break;
    case kPP:
      s << "pp"; break;
    case kPPP:
      s << "ppp"; break;
    case kPPPP:
      s << "pppp"; break;
    case kPPPPP:
      s << "ppppp"; break;
    case kPPPPPP:
      s << "pppppp"; break;

    case kMF:
      s << "mf"; break;
    case kMP:
      s << "mp"; break;
    case kFP:
      s << "fp"; break;
    case kFZ:
      s << "fz"; break;
    case kRF:
      s << "rf"; break;
    case kSF:
      s << "sf"; break;

    case kRFZ:
      s << "rfz"; break;
    case kSFZ:
      s << "sfz"; break;
    case kSFP:
      s << "sfp"; break;
    case kSFPP:
      s << "sfpp"; break;
    case kSFFZ:
      s << "sffz"; break;

    default:
      {
      stringstream s;
      s << "Dynamics " << fDynamicsKind << " is unknown";
      msrMusicXMLError (
        fMsrOptions->fInputSourceName,
        fInputLineNumber,
        s.str());
      }
  } // switch
  
  return s.str();
}

string msrDynamics::dynamicsKindAsLilypondString ()
{
  return "\\"+dynamicsKindAsString ();
}

void msrDynamics::acceptIn (basevisitor* v) {
  if (fMsrOptions->fDebugDebug)
    cerr << idtr <<
      "==> msrDynamics::acceptIn()" << endl;
      
  if (visitor<S_msrDynamics>*
    p =
      dynamic_cast<visitor<S_msrDynamics>*> (v)) {
        S_msrDynamics elem = this;
        
        if (fMsrOptions->fDebugDebug)
          cerr << idtr <<
            "==> Launching msrDynamics::visitStart()" << endl;
        p->visitStart (elem);
  }
}

void msrDynamics::acceptOut (basevisitor* v) {
  if (fMsrOptions->fDebugDebug)
    cerr << idtr <<
      "==> msrDynamics::acceptOut()" << endl;

  if (visitor<S_msrDynamics>*
    p =
      dynamic_cast<visitor<S_msrDynamics>*> (v)) {
        S_msrDynamics elem = this;
      
        if (fMsrOptions->fDebugDebug)
          cerr << idtr <<
            "==> Launching msrDynamics::visitEnd()" << endl;
        p->visitEnd (elem);
  }
}


void msrDynamics::browseData (basevisitor* v)
{}

ostream& operator<< (ostream& os, const S_msrDynamics& dyn)
{
  dyn->print (os);
  return os;
}

void msrDynamics::print (ostream& os)
{
  os <<
    "Dynamics" << " " << dynamicsKindAsString () <<
    endl;
}

//______________________________________________________________________________
S_msrWedge msrWedge::create (
  S_msrOptions& msrOpts, 
  int           inputLineNumber,
  msrWedgeKind  wedgeKind)
{
  msrWedge* o =
    new msrWedge (
      msrOpts, inputLineNumber, wedgeKind);
  assert(o!=0);
  return o;
}

msrWedge::msrWedge (
  S_msrOptions& msrOpts, 
  int           inputLineNumber,
  msrWedgeKind  wedgeKind)
    : msrElement (msrOpts, inputLineNumber)
{
  fWedgeKind = wedgeKind; 
}
msrWedge::~msrWedge() {}

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
    default:
      s << "Wedge" << fWedgeKind << "???";
  } // switch
    
  return s.str();
}

void msrWedge::acceptIn (basevisitor* v) {
  if (fMsrOptions->fDebugDebug)
    cerr << idtr <<
      "==> msrWedge::acceptIn()" << endl;
      
  if (visitor<S_msrWedge>*
    p =
      dynamic_cast<visitor<S_msrWedge>*> (v)) {
        S_msrWedge elem = this;
        
        if (fMsrOptions->fDebugDebug)
          cerr << idtr <<
            "==> Launching msrWedge::visitStart()" << endl;
        p->visitStart (elem);
  }
}

void msrWedge::acceptOut (basevisitor* v) {
  if (fMsrOptions->fDebugDebug)
    cerr << idtr <<
      "==> msrWedge::acceptOut()" << endl;

  if (visitor<S_msrWedge>*
    p =
      dynamic_cast<visitor<S_msrWedge>*> (v)) {
        S_msrWedge elem = this;
      
        if (fMsrOptions->fDebugDebug)
          cerr << idtr <<
            "==> Launching msrWedge::visitEnd()" << endl;
        p->visitEnd (elem);
  }
}

void msrWedge::browseData (basevisitor* v)
{}

ostream& operator<< (ostream& os, const S_msrWedge& wdg)
{
  wdg->print (os);
  return os;
}

void msrWedge::print (ostream& os)
{
  os <<
    "Wedge" << " " << wedgeKindAsString () <<
    endl;
}

//______________________________________________________________________________
S_msrSlur msrSlur::create (
  S_msrOptions& msrOpts, 
  int           inputLineNumber,
  msrSlurKind   slurKind)
{
  msrSlur* o =
    new msrSlur (
      msrOpts, inputLineNumber, slurKind);
  assert(o!=0);
  return o;
}

msrSlur::msrSlur (
  S_msrOptions& msrOpts, 
  int           inputLineNumber,
  msrSlurKind   slurKind)
    : msrElement (msrOpts, inputLineNumber)
{
  fSlurKind = slurKind; 
}
msrSlur::~msrSlur() {}

string msrSlur::slurKindAsString ()
{
  stringstream s;
  
  switch (fSlurKind) {
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
      s << "Slur" << fSlurKind << "???";
  } // switch
    
  return s.str();
}

void msrSlur::acceptIn (basevisitor* v) {
  if (fMsrOptions->fDebugDebug)
    cerr << idtr <<
      "==> msrSlur::acceptIn()" << endl;
      
  if (visitor<S_msrSlur>*
    p =
      dynamic_cast<visitor<S_msrSlur>*> (v)) {
        S_msrSlur elem = this;
        
        if (fMsrOptions->fDebugDebug)
          cerr << idtr <<
            "==> Launching msrSlur::visitStart()" << endl;
        p->visitStart (elem);
  }
}

void msrSlur::acceptOut (basevisitor* v) {
  if (fMsrOptions->fDebugDebug)
    cerr << idtr <<
      "==> msrSlur::acceptOut()" << endl;

  if (visitor<S_msrSlur>*
    p =
      dynamic_cast<visitor<S_msrSlur>*> (v)) {
        S_msrSlur elem = this;
      
        if (fMsrOptions->fDebugDebug)
          cerr << idtr <<
            "==> Launching msrSlur::visitEnd()" << endl;
        p->visitEnd (elem);
  }
}


void msrSlur::browseData (basevisitor* v)
{}

ostream& operator<< (ostream& os, const S_msrSlur& wdg)
{
  wdg->print (os);
  return os;
}

void msrSlur::print (ostream& os)
{
  os << "Slur" << " " << slurKindAsString ();
}

//______________________________________________________________________________
S_msrGraceexpression msrGraceexpression::create (
  S_msrOptions&   msrOpts, 
  int             inputLineNumber,
  bool            slashed)
{
  msrGraceexpression* o =
    new msrGraceexpression (
      msrOpts, inputLineNumber, slashed);
  assert(o!=0);
  return o;
}

msrGraceexpression::msrGraceexpression (
  S_msrOptions&   msrOpts, 
  int             inputLineNumber,
  bool            slashed)
    : msrElement (msrOpts, inputLineNumber)
{
  fGraceexpressionIsSlashed = slashed;
  
  fGraceexpressionVoicechunk =
    msrVoicechunk::create (
      fMsrOptions, fInputLineNumber);
}

msrGraceexpression::~msrGraceexpression() {}

S_msrGraceexpression msrGraceexpression::createEmptyClone ()
{
  if (fMsrOptions->fForceDebug || fMsrOptions->fDebug)
    cerr << idtr <<
      "--> Creating an empty clone of a grace expression" << endl;
  
  S_msrGraceexpression
    clone =
      msrGraceexpression::create (
        fMsrOptions,
        fInputLineNumber,
        fGraceexpressionIsSlashed);
  
  return clone;
}

void msrGraceexpression::acceptIn (basevisitor* v) {
  if (fMsrOptions->fDebugDebug)
    cerr << idtr <<
      "==> msrGraceexpression::acceptIn()" << endl;
      
  if (visitor<S_msrGraceexpression>*
    p =
      dynamic_cast<visitor<S_msrGraceexpression>*> (v)) {
        S_msrGraceexpression elem = this;
        
        if (fMsrOptions->fDebugDebug)
          cerr << idtr <<
            "==> Launching msrGraceexpression::visitStart()" << endl;
        p->visitStart (elem);
  }
}

void msrGraceexpression::acceptOut (basevisitor* v) {
  if (fMsrOptions->fDebugDebug)
    cerr << idtr <<
      "==> msrGraceexpression::acceptOut()" << endl;

  if (visitor<S_msrGraceexpression>*
    p =
      dynamic_cast<visitor<S_msrGraceexpression>*> (v)) {
        S_msrGraceexpression elem = this;
      
        if (fMsrOptions->fDebugDebug)
          cerr << idtr <<
            "==> Launching msrGraceexpression::visitEnd()" << endl;
        p->visitEnd (elem);
  }
}

void msrGraceexpression::browseData (basevisitor* v)
{
  // browse the voicechunk
  msrBrowser<msrVoicechunk> browser (v);
  browser.browse (*fGraceexpressionVoicechunk);
}

ostream& operator<< (ostream& os, const S_msrGraceexpression& graceexpression)
{
  graceexpression->print (os);
  return os;
}

void msrGraceexpression::print (ostream& os)
{
  os <<
    "Graceexpression" <<
    ", input line: " << fInputLineNumber <<
    ", slashed: " <<
    string (
      fGraceexpressionIsSlashed
        ? "yes"
        : "no") <<
    endl;
  
  idtr++;
  
  os << fGraceexpressionVoicechunk;
      
  idtr--;
}

//______________________________________________________________________________
S_msrNote msrNote::createFromMusicXMLData (
  S_msrOptions&        msrOpts,
  int                  inputLineNumber,
  msrMusicXMLNoteData& musicXMLNoteData,
  msrSlur::msrSlurKind slurKind)
{  
  msrNote * o =
    new msrNote (
      msrOpts, inputLineNumber, musicXMLNoteData, slurKind);
  assert(o!=0); 
  return o;
}

S_msrNote msrNote::createRest (
  S_msrOptions& msrOpts,
  int           inputLineNumber,
  int           divisions,
  int           voiceNumber)
{
  msrMusicXMLNoteData musicXMLNoteData;

  musicXMLNoteData.fMusicXMLStep = 'r';
  musicXMLNoteData.fMusicXMLStepIsARest = true;
  musicXMLNoteData.fMusicXMLStepIsUnpitched = false;

  musicXMLNoteData.fMusicXMLAlter = 0.0;
  musicXMLNoteData.fMusicXMLAlteration = msrMusicXMLNoteData::kNatural;
  musicXMLNoteData.fMusicXMLOctave = 0;
  
  musicXMLNoteData.fMusicXMLDivisions = divisions;

  musicXMLNoteData.fMusicXMLDotsNumber = 0;
  
  musicXMLNoteData.fMusicXMLNoteIsAGraceNote = false;
  
  musicXMLNoteData.fMusicXMLNoteBelongsToAChord = false;
  
  musicXMLNoteData.fMusicXMLNoteBelongsToATuplet = false;
  
  musicXMLNoteData.fMusicXMLTieKind =
    msrMusicXMLNoteData::k_NoTie;
  
  musicXMLNoteData.fMusicXMLVoiceNumber = voiceNumber;

  msrNote * o =
    new msrNote (
      msrOpts, inputLineNumber, musicXMLNoteData, msrSlur::k_NoSlur);
  assert(o!=0); 
  return o;
}    

S_msrNote msrNote::createBareNoteClone ()
{
  S_msrNote
    clone =
      msrNote::createFromMusicXMLData (
        fMsrOptions,
        fInputLineNumber,
        fMusicXMLNoteData,
        fNoteSlurKind);

  clone->fNoteKind = fNoteKind;
  
  clone->fNoteOctaveShift = fNoteOctaveShift;
  
  clone->fNoteStem = fNoteStem;  
  clone->fNoteBeam = fNoteBeam;  

  clone->fDivisionsPerWholeNote = fDivisionsPerWholeNote;
  
  clone->fNoteMeasureLocation = fNoteMeasureLocation;  

  return clone;
}

msrNote::msrNote (
  S_msrOptions&        msrOpts,
  int                  inputLineNumber,
  msrMusicXMLNoteData& musicXMLNoteData,
  msrSlur::msrSlurKind slurKind)
  :
    msrElement (msrOpts, inputLineNumber),
    fMusicXMLNoteData (musicXMLNoteData)
{
  fNoteSlurKind = slurKind;

//  if (fMsrOptions->fForceDebug || fMsrOptions->fDebugDebug) {
  if (fMsrOptions->fDebugDebug) {
    cerr << idtr <<
      "==> fMusicXMLNoteData contains:" << endl;
    cerr <<
      fMusicXMLNoteData;
  }

  fNoteKind = k_NoNoteKind;

  if (
    fMusicXMLNoteData.fMusicXMLStep < 'A'
      ||
    fMusicXMLNoteData.fMusicXMLStep > 'G') {
    if (! fMusicXMLNoteData.fMusicXMLStepIsARest) {
      stringstream s;
      
      s <<
        "step value " << fMusicXMLNoteData.fMusicXMLStep <<
        " is not a letter from A to G";
        
    //  msrMusicXMLError (s.str()); JMI
    msrMusicXMLWarning (
      fMsrOptions->fInputSourceName,
      fInputLineNumber,
      s.str());
    }
  }

//  cerr << "=== xmlPart2MsrVisitor::visitStart ( S_step& elt ) " << fCurrentMusicXMLStep << endl;
// JMI

  switch (fMusicXMLNoteData.fMusicXMLStep) {
    case 'A': fMusicXMLNoteData.fDiatonicPitch = msrMusicXMLNoteData::kA; break;
    case 'B': fMusicXMLNoteData.fDiatonicPitch = msrMusicXMLNoteData::kB; break;
    case 'C': fMusicXMLNoteData.fDiatonicPitch = msrMusicXMLNoteData::kC; break;
    case 'D': fMusicXMLNoteData.fDiatonicPitch = msrMusicXMLNoteData::kD; break;
    case 'E': fMusicXMLNoteData.fDiatonicPitch = msrMusicXMLNoteData::kE; break;
    case 'F': fMusicXMLNoteData.fDiatonicPitch = msrMusicXMLNoteData::kF; break;
    case 'G': fMusicXMLNoteData.fDiatonicPitch = msrMusicXMLNoteData::kG; break;
    default: {}
  } // switch

  // how many quater tones from A?s
  int noteQuatertonesFromA;
  
  switch (fMusicXMLNoteData.fMusicXMLStep) {
    case 'A': noteQuatertonesFromA =  0; break;
    case 'B': noteQuatertonesFromA =  4; break;
    case 'C': noteQuatertonesFromA =  6; break;
    case 'D': noteQuatertonesFromA = 10; break;
    case 'E': noteQuatertonesFromA = 14; break;
    case 'F': noteQuatertonesFromA = 16; break;
    case 'G': noteQuatertonesFromA = 20; break;
    default: {}    
  } // switch

  // flat or sharp,possibly semi- or sesqui-?
  msrMusicXMLNoteData::msrMusicXMLAlterationKind
    musicXMLAlterationKind; // JMI

//   if (fMsrOptions->fForceDebug || fMsrOptions->fDebugDebug) {
  if (fMsrOptions->fDebugDebug) {
    cerr <<
      "--> fMusicXMLNoteData.fMusicXMLAlter = " <<
      fMusicXMLNoteData.fMusicXMLAlter <<
      endl;
  }

/*
  The alter element represents chromatic alteration
  in number of semitones (e.g., -1 for flat, 1 for sharp).
  Decimal values like 0.5 (quarter tone sharp) are used for microtones.

  The following table lists note names for quarter-tone accidentals
  in various languages; here the pre- fixes semi- and sesqui-
  respectively mean ‘half’ and ‘one and a half’.
  
  Languages that do not appear in this table do not provide special note names yet.
  Language
              semi-sharp semi-flat sesqui-sharp sesqui-flat
                 +0.5      -0.5        +1.5       -1.5
    nederlands   -ih       -eh        -isih       -eseh

  We use dutch pitches names for the enumeration below.
  The following is a series of Cs with increasing pitches:
    \relative c'' { ceseh ces ceh c cih cis cisih }
*/

  if      (fMusicXMLNoteData.fMusicXMLAlter == 0 ) {
    fMusicXMLNoteData.fMusicXMLAlteration = msrMusicXMLNoteData::kNatural;
  }
  
  else if (fMusicXMLNoteData.fMusicXMLAlter == -1 ) {
    fMusicXMLNoteData.fMusicXMLAlteration = msrMusicXMLNoteData::kFlat;
    noteQuatertonesFromA -= 2;
    if (noteQuatertonesFromA < 0)
      noteQuatertonesFromA += 24; // it is below A
  }
  
  else if (fMusicXMLNoteData.fMusicXMLAlter == 1 ) {
    fMusicXMLNoteData.fMusicXMLAlteration = msrMusicXMLNoteData::kSharp;
    noteQuatertonesFromA += 2;
  }
  
  else if (fMusicXMLNoteData.fMusicXMLAlter == -0.5 ) {
    fMusicXMLNoteData.fMusicXMLAlteration = msrMusicXMLNoteData::kSemiFlat;
    noteQuatertonesFromA -= 1;
    if (noteQuatertonesFromA < 0)
      noteQuatertonesFromA += 24; // it is below A
  }
  
  else if (fMusicXMLNoteData.fMusicXMLAlter == +0.5 ) {
    fMusicXMLNoteData.fMusicXMLAlteration = msrMusicXMLNoteData::kSemiSharp;
    noteQuatertonesFromA += 1;
  }
  
  else if (fMusicXMLNoteData.fMusicXMLAlter == -1.5 ) {
    fMusicXMLNoteData.fMusicXMLAlteration = msrMusicXMLNoteData::kSesquiFlat;
    noteQuatertonesFromA -= 3;
    if (noteQuatertonesFromA < 0)
      noteQuatertonesFromA += 24; // it is below A
  }
  
  else if (fMusicXMLNoteData.fMusicXMLAlter == +1.5 ) {
    fMusicXMLNoteData.fMusicXMLAlteration = msrMusicXMLNoteData::kSesquiSharp;
    noteQuatertonesFromA += 3;
  }
  
  else {
    stringstream s;
    
    s <<
      "MusicXML alter " << fMusicXMLNoteData.fMusicXMLAlter <<
      " should be -1.5, -1, -0.5, 0, +0.5, +1 or +1.5";
      
    msrMusicXMLError (
      fMsrOptions->fInputSourceName,
      fInputLineNumber,
      s.str());
  }

/* JMI
  if (fMsrOptions->fDebugDebug)
    cerr <<
      "--> noteQuatertonesFromA = " << noteQuatertonesFromA <<
      endl;
*/

//  fNoteIsChordFirstNote = false;
}

msrNote::~msrNote()
{}

void msrNote::setNoteBelongsToAChord () {
  if (fMsrOptions->fDebug)
    cerr << idtr <<
      "--> note " << this <<
      " is set to belong to a chord" << endl;

  fMusicXMLNoteData.fMusicXMLNoteBelongsToAChord = true;
  fNoteKind = msrNote::kChordMemberNote;
}

void msrNote::applyTupletMemberDisplayFactor (
  int actualNotes, int normalNotes)
{
  if (fMsrOptions->fDebug)
    cerr << idtr <<
      "--> applying tuplet display factor " <<
      actualNotes << "/" << normalNotes <<
      " to note " << this <<
      endl;

  fMusicXMLNoteData.fMusicXMLDisplayDivisions =
    fMusicXMLNoteData.fMusicXMLDisplayDivisions
      *
    actualNotes
      /
    normalNotes;
}

void msrNote::addArticulationToNote (S_msrArticulation art)
{
  fNoteArticulations.push_back (art);
}

void msrNote::addDynamicsToNote (S_msrDynamics dynamics)
{
  fNoteDynamics.push_back (dynamics);
}

void msrNote::addWordsToNote (S_msrWords words)
{
  fNoteWords.push_back (words);
}

void msrNote::addWedgeToNote (S_msrWedge wedge)
{
  fNoteWedges.push_back (wedge);
}

S_msrDynamics msrNote::removeFirstDynamics ()
{
  S_msrDynamics dyn = fNoteDynamics.front ();
  fNoteDynamics.pop_front ();
  return dyn;
}

S_msrWedge msrNote::removeFirstWedge ()
{
  S_msrWedge wdg = fNoteWedges.front ();
  fNoteWedges.pop_front ();
  return wdg;
}

void msrNote::acceptIn (basevisitor* v)
{
  if (fMsrOptions->fDebugDebug)
    cerr << idtr <<
      "==> msrNote::acceptIn()" << endl;
      
  if (visitor<S_msrNote>*
    p =
      dynamic_cast<visitor<S_msrNote>*> (v)) {
        S_msrNote elem = this;
        
        if (fMsrOptions->fDebugDebug)
          cerr << idtr <<
            "==> Launching msrNote::visitStart()" << endl;
        p->visitStart (elem);
  }
}

void msrNote::acceptOut (basevisitor* v)
{
  if (fMsrOptions->fDebugDebug)
    cerr << idtr <<
      "==> msrNote::acceptOut()" << endl;

  if (visitor<S_msrNote>*
    p =
      dynamic_cast<visitor<S_msrNote>*> (v)) {
        S_msrNote elem = this;
      
        if (fMsrOptions->fDebugDebug)
          cerr << idtr <<
            "==> Launching msrNote::visitEnd()" << endl;
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

  if (fNoteBeam) {
    // browse the beam
    msrBrowser<msrBeam> browser (v);
    browser.browse (*fNoteBeam);
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

  // browse the words if any
  if (fNoteWords.size()) {
    idtr++;
    list<S_msrWords>::const_iterator i;
    for (i=fNoteWords.begin(); i!=fNoteWords.end(); i++) {
      // browse the articulation
      msrBrowser<msrWords> browser (v);
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

/* JMI
  // browse the slur if any
  if (fNoteSlurKind != msrSlur::k_NoSlur) {
    // browse the slur
    msrBrowser<msrDynamics> browser (v);
    browser.browse (*(*i));
  }
  */
}

string msrNote::notePitchAsString () const
{
  stringstream s;
  
  /*
  cerr << "msrNote::notePitchAsString (), isRest = " <<
    fMusicXMLNoteData.fMusicXMLStepIsARest <<
    ", fMsrPitch = " << fMsrPitch << endl;
  */
  
  if (fMusicXMLNoteData.fMusicXMLStepIsARest)

    s << "r";

  else if (fMusicXMLNoteData.fMusicXMLStepIsUnpitched)

    s << "unpitched ";

  else {

    switch (fMusicXMLNoteData.fDiatonicPitch) {
      case msrMusicXMLNoteData::kA: s << "a"; break;
      case msrMusicXMLNoteData::kB: s << "b"; break;
      case msrMusicXMLNoteData::kC: s << "c"; break;
      case msrMusicXMLNoteData::kD: s << "d"; break;
      case msrMusicXMLNoteData::kE: s << "e"; break;
      case msrMusicXMLNoteData::kF: s << "f"; break;
      case msrMusicXMLNoteData::kG: s << "g"; break;
      default: s << "?";
    } // switch

    /*
    The alter element represents chromatic alteration
    in number of semitones (e.g., -1 for flat, 1 for sharp).
    Decimal values like 0.5 (quarter tone sharp) are used for microtones.
  
    The following table lists note names for quarter-tone accidentals
    in various languages; here the pre- fixes semi- and sesqui-
    respectively mean ‘half’ and ‘one and a half’.
    
    Languages that do not appear in this table do not provide special note names yet.
    Language
                semi-sharp semi-flat sesqui-sharp sesqui-flat
                   +0.5      -0.5        +1.5       -1.5
      nederlands   -ih       -eh        -isih       -eseh
  
    We use dutch pitches names for the enumeration below.
    The following is a series of Cs with increasing pitches:
      \relative c'' { ceseh ces ceh c cih cis cisih }
  */

    switch (fMusicXMLNoteData.fMusicXMLAlteration) {

      case msrMusicXMLNoteData::kSesquiFlat:
        s << "eseh";
        break;
        
      case msrMusicXMLNoteData::kFlat:
        s << "es";
        break;
        
      case msrMusicXMLNoteData::kSemiFlat:
        s << "eh";
        break;
        
      case msrMusicXMLNoteData::kNatural:
        break;
        
      case msrMusicXMLNoteData::kSemiSharp:
        s << "ih";
        break;
        
      case msrMusicXMLNoteData::kSharp:
        s << "is";
        break;
        
      case msrMusicXMLNoteData::kSesquiSharp:
        s << "isih";
        break;      
    } // switch  
  }
  
  return s.str();
}

string msrNote::noteDivisionsAsMSRString () const
{
  string result;
  int    computedNumberOfDots;
  string errorMessage;

  result =
    divisionsAsMSRDuration (
      fMusicXMLNoteData.fMusicXMLDisplayDivisions,
      fDivisionsPerWholeNote,
      fMusicXMLNoteData.fMusicXMLDotsNumber,
      computedNumberOfDots,
      errorMessage,
      false); // 'true' to debug it

  if (errorMessage.size ())
    msrMusicXMLError (
      fMsrOptions->fInputSourceName,
      fInputLineNumber,
      errorMessage);

  return result;
}

string msrNote::noteTypeAsMSRString () const
{
  string result;
  string errorMessage;

  result =
    noteTypeAsMSRDuration (
      fMusicXMLNoteData.fMusicXMLType,
      errorMessage);

  if (errorMessage.size ())
    msrMusicXMLError (
      fMsrOptions->fInputSourceName,
      fInputLineNumber,
      errorMessage);

  return result;
}

string msrNote::noteDiatonicPitchAsString () const
{
  // fMusicXMLNoteData.fMusicXMLStep is a char
  switch (fMusicXMLNoteData.fDiatonicPitch) {
    case msrMusicXMLNoteData::kA: return "A"; break;
    case msrMusicXMLNoteData::kB: return "B"; break;
    case msrMusicXMLNoteData::kC: return "C"; break;
    case msrMusicXMLNoteData::kD: return "D"; break;
    case msrMusicXMLNoteData::kE: return "E"; break;
    case msrMusicXMLNoteData::kF: return "F"; break;
    case msrMusicXMLNoteData::kG: return "G"; break;
    default: return "?";
  } // switch
}

string msrNote::noteAsString () const
{
  stringstream s;

  switch (fNoteKind) {
    case msrNote::k_NoNoteKind:
      break;
      
    case msrNote::kStandaloneNote:
      s <<
        "Standalone note" <<
        " " <<
        notePitchAsString () <<
        "[" << fMusicXMLNoteData.fMusicXMLOctave << "]" <<
        ":" <<
        noteDivisionsAsMSRString ();
      break;
      
    case msrNote::kGraceNote:
      s <<
        "Grace note" <<
        " " <<
        notePitchAsString () <<
        "[" << fMusicXMLNoteData.fMusicXMLOctave << "]" <<
        ":" <<
        noteTypeAsMSRString ();
      for (int i = 0; i < fMusicXMLNoteData.fMusicXMLDotsNumber; i++) {
        s << ".";
      } // for
      break;
      
    case msrNote::kRestNote:
      s <<
        "Rest" <<
        ":" <<
        noteDivisionsAsMSRString ();
      break;
      
    case msrNote::kChordMemberNote:
      s <<
        "Chord member note" <<
        " " <<
        notePitchAsString () <<
        "[" << fMusicXMLNoteData.fMusicXMLOctave << "]" <<
        ":" <<
        noteDivisionsAsMSRString ();
      break;
      
    case msrNote::kTupletMemberNote:
      s <<
        "Tuplet member note" <<
        " " <<
        notePitchAsString ();

      if (! fMusicXMLNoteData.fMusicXMLStepIsARest)
        s <<
          "[" << fMusicXMLNoteData.fMusicXMLOctave << "]" <<
          ":" <<
          noteDivisionsAsMSRString ();
      break;
  } // switch
     
  if (
    fMusicXMLNoteData.fMusicXMLTieKind
      !=
    msrMusicXMLNoteData::k_NoTie ) {
      s <<
        ", " << fMusicXMLNoteData.musicXMLTieKindAsString ();
  }

  return s.str();
}

ostream& operator<< (ostream& os, const S_msrNote& elt)
{
//  if (elt) // JMI JMI
  elt->print (os);
  return os;
}

void msrNote::print (ostream& os)
{
  rational
    position (
      fNoteMeasureLocation.fPositionInMeasure,
      fDivisionsPerWholeNote);

  position.rationalise ();
  
  // print the note itself and its position
  os <<
    noteAsString () <<
    ", input line " << fInputLineNumber << ", " <<
    "(" <<
    fMusicXMLNoteData.fMusicXMLDivisions <<
    "_" <<
    fMusicXMLNoteData.fMusicXMLDisplayDivisions <<
    "/" <<
    fDivisionsPerWholeNote <<
    ") @"<<
    fNoteMeasureLocation.fMeasureNumber <<
    ":" <<
    position.getNumerator() <<
    "/" <<
    position.getDenominator() <<
    " (" <<
    fNoteMeasureLocation.fPositionInMeasure <<
    "/" <<
    fDivisionsPerWholeNote <<
    ")" <<
    /*
    " first = " <<
    string (
      fNoteIsChordFirstNote
        ? "true"
        : "false" ) <<
        */
    endl;

  // print the octave shift if any
  if (fNoteOctaveShift) {
    idtr++;
    os <<
      idtr << fNoteOctaveShift;
    idtr--;
  }

  // print the stem if any
  if (fNoteStem) {
    idtr++;
    os <<
      idtr << fNoteStem;
    idtr--;
  }

  // print the beam if any
  if (fNoteBeam) {
    idtr++;
    os <<
      idtr << fNoteBeam;
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
      os << idtr << (*i) << endl;
      if (++i == iEnd) break;
      os << endl;
    } // for
        
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
      os << idtr << (*i);
      if (++i == iEnd) break;
      os << endl;
    } // for

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
      os << idtr << (*i);
      if (++i == iEnd) break;
      os << endl;
    } // for

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
      os << idtr << (*i);
      if (++i == iEnd) break;
      os << endl;
    } // for
    
    idtr--;
  }

  // print the slur if any
  if (fNoteSlurKind != msrSlur::k_NoSlur) { // JMI
    idtr++;

    os <<
      idtr << "Slur, kind: ";
    switch (fNoteSlurKind) {
      case msrSlur::kStartSlur:
        os << "start";
        break;
      case msrSlur::kContinueSlur:
        os << "continue";
        break;
      case msrSlur::kStopSlur:
        os << "stop";
        break;
      default:
        os << fNoteSlurKind << "???";
    } // switch
    os << endl;
    
    idtr--;
  }
}

//______________________________________________________________________________
S_msrChord msrChord::create (
  S_msrOptions& msrOpts, 
  int           inputLineNumber,
  int           chordDivisions)
{
  msrChord* o =
    new msrChord (
      msrOpts, inputLineNumber, chordDivisions);
  assert(o!=0);
  return o;
}

msrChord::msrChord (
  S_msrOptions& msrOpts, 
  int           inputLineNumber,
  int           chordDivisions)
    : msrElement (msrOpts, inputLineNumber)
{
  fChordDivisions = chordDivisions;

  fChordTieKind = msrMusicXMLNoteData::k_NoTie;
}

msrChord::~msrChord() {}

S_msrChord msrChord::createEmptyChordClone ()
{
  S_msrChord
    clone =
      msrChord::create (
        fMsrOptions,
        fInputLineNumber,
        fChordDivisions);

  clone->
    fDivisionsPerWholeNote = fDivisionsPerWholeNote;

  // use setChordMeasureLocation() for its side effects
  // instead of a direct assignment to fChordMeasureLocation
  clone->
    setChordMeasureLocation (fChordMeasureLocation);
    
  clone->fChordTieKind = fChordTieKind;

/*
  // get the articulations if any
  if (fChordArticulations.size()) {
    list<S_msrArticulation>::const_iterator i;
    for (i=fChordArticulations.begin(); i!=fChordArticulations.end(); i++) {
      clone->addArticulationToChord ((*i));
    } // for
  }
*/

  return clone;
}
    
void msrChord::addNoteToChord (S_msrNote note)
{
  /*
  if (! fChordNotes.size ())
    note->setNoteIsChordFirstNote (true);
*/
  fChordNotes.push_back (note);
  
  note->setNoteBelongsToAChord ();
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

//  if (fMsrOptions->fDebug)
    cerr << idtr <<
      "--> adding articulation '" <<
      art->articulationKindAsString () <<
      "' to chord" <<
      endl;

  fChordArticulations.push_back (art);
}

void msrChord::acceptIn (basevisitor* v) {
  if (fMsrOptions->fDebugDebug)
    cerr << idtr <<
      "==> msrChord::acceptIn()" << endl;
      
  if (visitor<S_msrChord>*
    p =
      dynamic_cast<visitor<S_msrChord>*> (v)) {
        S_msrChord elem = this;
        
        if (fMsrOptions->fDebugDebug)
          cerr << idtr <<
            "==> Launching msrChord::visitStart()" << endl;
        p->visitStart (elem);
  }
}

void msrChord::acceptOut (basevisitor* v) {
  if (fMsrOptions->fDebugDebug)
    cerr << idtr <<
      "==> msrChord::acceptOut()" << endl;

  if (visitor<S_msrChord>*
    p =
      dynamic_cast<visitor<S_msrChord>*> (v)) {
        S_msrChord elem = this;
      
        if (fMsrOptions->fDebugDebug)
          cerr << idtr <<
            "==> Launching msrChord::visitEnd()" << endl;
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
    list<S_msrDynamics>::const_iterator i = fChordDynamics.begin();
    i != fChordDynamics.end();
    i++ ) {
    // browse the dynamics
    msrBrowser<msrDynamics> browser (v);
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
    list<S_msrWedge>::const_iterator i = fChordWedges.begin();
    i != fChordWedges.end();
    i++ ) {
    // browse the wedge
    msrBrowser<msrWedge> browser (v);
    browser.browse (*(*i));
  } // for
}

ostream& operator<< (ostream& os, const S_msrChord& chrd)
{
  chrd->print (os);
  return os;
}

string msrChord::chordDivisionsAsMSRString () const
{
  string result;
  int    computedNumberOfDots;
  string errorMessage;

  int inputSourceSuppliedNumberOfDots =
    fChordNotes [0]-> 
      getNoteMusicXMLDotsNumber (); // any chord member note is fine
    
  result =
    divisionsAsMSRDuration (
      fChordDivisions,
      fDivisionsPerWholeNote,
      inputSourceSuppliedNumberOfDots,
      computedNumberOfDots,
      errorMessage,
      false); // 'true' to debug it

  if (errorMessage.size ())
    msrMusicXMLError (
      fMsrOptions->fInputSourceName,
      fInputLineNumber,
      errorMessage);

  return result;
}

void msrChord::print (ostream& os)
{
  os <<
    "Chord" <<
    " (" <<
    chordDivisionsAsMSRString () <<
    "/" <<
    fDivisionsPerWholeNote <<
    ") @"<<
    fChordMeasureLocation.fMeasureNumber <<
    ":" <<
    fChordMeasureLocation.fPositionInMeasure <<
    "/" <<
    fDivisionsPerWholeNote <<
    endl;

  idtr++;
  
  // print the member notes if any
  if (fChordNotes.size()) {
    vector<S_msrNote>::const_iterator
      iBegin = fChordNotes.begin(),
      iEnd   = fChordNotes.end(),
      i      = iBegin;
    for ( ; ; ) {
   //   os << (*i)->notePitchAsString (); JMI
      os << idtr << (*i);
      if (++i == iEnd) break;
      os << endl;
    } // for
  }
  
  // print the articulations if any
  if (fChordArticulations.size()) {
    list<S_msrArticulation>::const_iterator i;
    for (i=fChordArticulations.begin(); i!=fChordArticulations.end(); i++) {
      os << idtr << (*i);
    } // for
  }

  // print the dynamics if any
  if (fChordDynamics.size()) {
    list<S_msrDynamics>::const_iterator i;
    for (i=fChordDynamics.begin(); i!=fChordDynamics.end(); i++) {
      os << idtr << (*i);
    } // for
  }

  // print the words if any
  if (fChordWords.size()) {
    list<S_msrWords>::const_iterator i;
    for (i=fChordWords.begin(); i!=fChordWords.end(); i++) {
      os << idtr << (*i);
    } // for
  }

  // print the wedges if any
  if (fChordWedges.size()) {
    list<S_msrWedge>::const_iterator i;
    for (i=fChordWedges.begin(); i!=fChordWedges.end(); i++) {
      os << idtr << (*i);
    } // for
  }

  idtr--;
}

//______________________________________________________________________________
S_msrComment msrComment::create (
  S_msrOptions& msrOpts, 
  int           inputLineNumber,
  string        contents,
  msrGapKind    gapKind)
{
  msrComment* o =
    new msrComment (
      msrOpts, inputLineNumber, contents, gapKind);
  assert(o!=0);
  return o;
}

msrComment::msrComment (
  S_msrOptions& msrOpts, 
  int           inputLineNumber,
  string        contents,
  msrGapKind    gapKind)
    : msrElement (msrOpts, inputLineNumber)
{
  fContents = contents;
  fGapKind  = gapKind;
}
msrComment::~msrComment() {}

void msrComment::acceptIn (basevisitor* v) {
  if (fMsrOptions->fDebugDebug)
    cerr << idtr <<
      "==> msrComment::acceptIn()" << endl;
      
  if (visitor<S_msrComment>*
    p =
      dynamic_cast<visitor<S_msrComment>*> (v)) {
        S_msrComment elem = this;
        
        if (fMsrOptions->fDebugDebug)
          cerr << idtr <<
            "==> Launching msrComment::visitStart()" << endl;
        p->visitStart (elem);
  }
}

void msrComment::acceptOut (basevisitor* v) {
  if (fMsrOptions->fDebugDebug)
    cerr << idtr <<
      "==> msrComment::acceptOut()" << endl;

  if (visitor<S_msrComment>*
    p =
      dynamic_cast<visitor<S_msrComment>*> (v)) {
        S_msrComment elem = this;
      
        if (fMsrOptions->fDebugDebug)
          cerr << idtr <<
            "==> Launching msrComment::visitEnd()" << endl;
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

void msrComment::print (ostream& os)
{
  os << "Comment" << endl;
  idtr++;
  os << idtr << "% " << fContents << endl;
  if (fGapKind == kGapAfterwards) os << idtr << endl;
  idtr--;
}

//______________________________________________________________________________
S_msrBreak msrBreak::create (
  S_msrOptions& msrOpts, 
  int           inputLineNumber,
  int           nextBarNumber)
{
  msrBreak* o =
    new msrBreak (
      msrOpts, inputLineNumber, nextBarNumber);
  assert(o!=0);
  return o;
}

msrBreak::msrBreak (
  S_msrOptions& msrOpts, 
  int           inputLineNumber,
  int           nextBarNumber)
    : msrElement (msrOpts, inputLineNumber)
{
  fNextBarNumber = nextBarNumber; 

  if (fMsrOptions->fTrace)
    cerr << idtr <<
      "Creating a break before measure " << fNextBarNumber <<
      endl;
}

msrBreak::~msrBreak() {}

void msrBreak::acceptIn (basevisitor* v) {
  if (fMsrOptions->fDebugDebug)
    cerr << idtr <<
      "==> msrBreak::acceptIn()" << endl;
      
  if (visitor<S_msrBreak>*
    p =
      dynamic_cast<visitor<S_msrBreak>*> (v)) {
        S_msrBreak elem = this;
        
        if (fMsrOptions->fDebugDebug)
          cerr << idtr <<
            "==> Launching msrBreak::visitStart()" << endl;
        p->visitStart (elem);
  }
}

void msrBreak::acceptOut (basevisitor* v) {
  if (fMsrOptions->fDebugDebug)
    cerr << idtr <<
      "==> msrBreak::acceptOut()" << endl;

  if (visitor<S_msrBreak>*
    p =
      dynamic_cast<visitor<S_msrBreak>*> (v)) {
        S_msrBreak elem = this;
      
        if (fMsrOptions->fDebugDebug)
          cerr << idtr <<
            "==> Launching msrBreak::visitEnd()" << endl;
        p->visitEnd (elem);
  }
}

void msrBreak::browseData (basevisitor* v)
{}

ostream& operator<< (ostream& os, const S_msrBreak& elt)
{
  elt->print (os);
  return os;
}

void msrBreak::print (ostream& os)
{
  os <<
    "Break" << ", next bar number = " << fNextBarNumber << endl <<
    endl;
}

//______________________________________________________________________________
S_msrBarCheck msrBarCheck::create (
  S_msrOptions& msrOpts, 
  int           inputLineNumber,
  int           nextBarNumber)
{
  msrBarCheck* o =
    new msrBarCheck (
      msrOpts, inputLineNumber, nextBarNumber);
  assert(o!=0);
  return o;
}

msrBarCheck::msrBarCheck (
  S_msrOptions& msrOpts, 
  int           inputLineNumber,
  int           nextBarNumber)
    : msrElement (msrOpts, inputLineNumber)
{
  fNextBarNumber = nextBarNumber; 

  if (fMsrOptions->fTrace)
    cerr << idtr <<
      "Creating a bar check before measure " << fNextBarNumber <<
      endl;
}
msrBarCheck::~msrBarCheck() {}

void msrBarCheck::acceptIn (basevisitor* v) {
  if (fMsrOptions->fDebugDebug)
    cerr << idtr <<
      "==> msrBarCheck::acceptIn()" << endl;
      
  if (visitor<S_msrBarCheck>*
    p =
      dynamic_cast<visitor<S_msrBarCheck>*> (v)) {
        S_msrBarCheck elem = this;
        
        if (fMsrOptions->fDebugDebug)
          cerr << idtr <<
            "==> Launching msrBarCheck::visitStart()" << endl;
        p->visitStart (elem);
  }
}

void msrBarCheck::acceptOut (basevisitor* v) {
  if (fMsrOptions->fDebugDebug)
    cerr << idtr <<
      "==> msrBarCheck::acceptOut()" << endl;

  if (visitor<S_msrBarCheck>*
    p =
      dynamic_cast<visitor<S_msrBarCheck>*> (v)) {
        S_msrBarCheck elem = this;
      
        if (fMsrOptions->fDebugDebug)
          cerr << idtr <<
            "==> Launching msrBarCheck::visitEnd()" << endl;
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
    "BarCheck" << ", next bar number = " << fNextBarNumber <<
    endl;
}

//______________________________________________________________________________
S_msrBarnumberCheck msrBarnumberCheck::create (
  S_msrOptions& msrOpts, 
  int           inputLineNumber,
  int           nextBarNumber)
{
  msrBarnumberCheck* o =
    new msrBarnumberCheck (
      msrOpts, inputLineNumber, nextBarNumber);
  assert(o!=0);
  return o;
}

msrBarnumberCheck::msrBarnumberCheck (
  S_msrOptions& msrOpts, 
  int           inputLineNumber,
  int           nextBarNumber)
    : msrElement (msrOpts, inputLineNumber)
{
  fNextBarNumber=nextBarNumber; 
}
msrBarnumberCheck::~msrBarnumberCheck() {}

void msrBarnumberCheck::acceptIn (basevisitor* v) {
  if (fMsrOptions->fDebugDebug)
    cerr << idtr <<
      "==> msrBarnumberCheck::acceptIn()" << endl;
      
  if (visitor<S_msrBarnumberCheck>*
    p =
      dynamic_cast<visitor<S_msrBarnumberCheck>*> (v)) {
        S_msrBarnumberCheck elem = this;
        
        if (fMsrOptions->fDebugDebug)
          cerr << idtr <<
            "==> Launching msrBarnumberCheck::visitStart()" << endl;
        p->visitStart (elem);
  }
}

void msrBarnumberCheck::acceptOut (basevisitor* v) {
  if (fMsrOptions->fDebugDebug)
    cerr << idtr <<
      "==> msrBarnumberCheck::acceptOut()" << endl;

  if (visitor<S_msrBarnumberCheck>*
    p =
      dynamic_cast<visitor<S_msrBarnumberCheck>*> (v)) {
        S_msrBarnumberCheck elem = this;
      
        if (fMsrOptions->fDebugDebug)
          cerr << idtr <<
            "==> Launching msrBarnumberCheck::visitEnd()" << endl;
        p->visitEnd (elem);
  }
}

void msrBarnumberCheck::browseData (basevisitor* v)
{}

ostream& operator<< (ostream& os, const S_msrBarnumberCheck& elt)
{
  elt->print (os);
  return os;
}

void msrBarnumberCheck::print (ostream& os)
{
  os <<
    "BarnumberCheck" << ", next bar number = " << fNextBarNumber <<
    endl << endl;
}

//______________________________________________________________________________
S_msrTuplet msrTuplet::create (
  S_msrOptions& msrOpts, 
  int           inputLineNumber,
  int           number,
  int           actualNotes,
  int           normalNotes,
  S_msrNote     firstNote)
{
  msrTuplet* o =
    new msrTuplet (
      msrOpts, inputLineNumber,
      number, actualNotes, normalNotes, firstNote);
  assert(o!=0);
  return o;
}

msrTuplet::msrTuplet (
  S_msrOptions& msrOpts, 
  int           inputLineNumber,
  int           number,
  int           actualNotes,
  int           normalNotes,
  S_msrNote     firstNote)
    : msrElement (msrOpts, inputLineNumber)
{  
  fTupletNumber = number;
  
  fActualNotes = actualNotes;
  fNormalNotes = normalNotes;

/*
  fTupletDivisions =
    firstNote->getNoteMusicXMLDivisions ();
  fTupletDisplayDivisions =
    firstNote->getNoteDisplayDivisions ();

  // measure location is that of the first note
  fTupletMeasureLocation =
    firstNote->getNoteMeasureLocation ();
    */
}

msrTuplet::~msrTuplet() {}

S_msrTuplet msrTuplet::createEmptyClone ()
{
  S_msrTuplet
    clone =
      msrTuplet::create (
        fMsrOptions,
        fInputLineNumber,
        fTupletNumber,
        fActualNotes,
        fNormalNotes,
 // JMI       fTupletElements [0]); // any element would be fine
        0); // any element would be fine
  
  return clone;
}

void msrTuplet::acceptIn (basevisitor* v) {
  if (fMsrOptions->fDebugDebug)
    cerr << idtr <<
      "==> msrTuplet::acceptIn()" << endl;
      
  if (visitor<S_msrTuplet>*
    p =
      dynamic_cast<visitor<S_msrTuplet>*> (v)) {
        S_msrTuplet elem = this;
        
        if (fMsrOptions->fDebugDebug)
          cerr << idtr <<
            "==> Launching msrTuplet::visitStart()" << endl;
        p->visitStart (elem);
  }
}

void msrTuplet::acceptOut (basevisitor* v) {
  if (fMsrOptions->fDebugDebug)
    cerr << idtr <<
      "==> msrTuplet::acceptOut()" << endl;

  if (visitor<S_msrTuplet>*
    p =
      dynamic_cast<visitor<S_msrTuplet>*> (v)) {
        S_msrTuplet elem = this;
      
        if (fMsrOptions->fDebugDebug)
          cerr << idtr <<
            "==> Launching msrTuplet::visitEnd()" << endl;
        p->visitEnd (elem);
  }
}


void msrTuplet::browseData (basevisitor* v)
{
 for (
    vector<S_msrElement>::const_iterator i = fTupletElements.begin();
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

void msrTuplet::print (ostream& os)
{
  os <<
    "Tuplet " << fActualNotes << "/" << fNormalNotes <<
    " (" << fTupletDivisions <<
    "/" <<
    fDivisionsPerWholeNote <<
    ") @"<<
    fTupletMeasureLocation.fMeasureNumber <<
    ":" <<
    fTupletMeasureLocation.fPositionInMeasure <<
    "/" <<
    fDivisionsPerWholeNote <<
    endl;
    
  idtr++;

  vector<S_msrElement>::const_iterator i;
  for (i=fTupletElements.begin(); i!=fTupletElements.end(); i++) {
    os << idtr << (*i);
  } // for
  
  idtr--;
}

//______________________________________________________________________________
S_msrPageGeometry msrPageGeometry::create (
  S_msrOptions& msrOpts, 
  int           inputLineNumber)
{
  msrPageGeometry* o =
    new msrPageGeometry (
      msrOpts, inputLineNumber);
  assert(o!=0);
  return o;
}

msrPageGeometry::msrPageGeometry (
  S_msrOptions& msrOpts, 
  int           inputLineNumber)
    : msrElement (msrOpts, inputLineNumber)
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

msrPageGeometry::~msrPageGeometry() {}

float msrPageGeometry::globalStaffSize () const
{
  if (fMillimeters > 0)
    return fMillimeters * 72.27 / 25.4;
  else
    return 20.0; // LilyPond default
}

void msrPageGeometry::acceptIn (basevisitor* v) {
  if (fMsrOptions->fDebugDebug)
    cerr << idtr <<
      "==> msrPageGeometry::acceptIn()" << endl;
      
  if (visitor<S_msrPageGeometry>*
    p =
      dynamic_cast<visitor<S_msrPageGeometry>*> (v)) {
        S_msrPageGeometry elem = this;
        
        if (fMsrOptions->fDebugDebug)
          cerr << idtr <<
            "==> Launching msrPageGeometry::visitStart()" << endl;
        p->visitStart (elem);
  }
}

void msrPageGeometry::acceptOut (basevisitor* v) {
  if (fMsrOptions->fDebugDebug)
    cerr << idtr <<
      "==> msrPageGeometry::acceptOut()" << endl;

  if (visitor<S_msrPageGeometry>*
    p =
      dynamic_cast<visitor<S_msrPageGeometry>*> (v)) {
        S_msrPageGeometry elem = this;
      
        if (fMsrOptions->fDebugDebug)
          cerr << idtr <<
            "==> Launching msrPageGeometry::visitEnd()" << endl;
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

ostream& operator<< (ostream& os, const S_msrPageGeometry& pap) {
  pap->print (os);
  return os;
}

void msrPageGeometry::print (ostream& os) {
  os << "PageGeometry" << endl;

  bool emptyGeometry = true;
  
  idtr++;
  
  if (fPaperWidth > 0) {
    os <<
      idtr << setw(12) << left << "PaperWidth" << " = " <<
      setprecision(4) << fPaperWidth << " cm" << endl;
    emptyGeometry = false;
  }
  if (fPaperHeight > 0) {
    os <<
      idtr << setw(12) << left << "PaperHeight" << " = " <<
      setprecision(4) << fPaperHeight << " cm" << endl;
    emptyGeometry = false;
  }
  
  if (fTopMargin > 0) {
    os <<
      idtr << setw(12) << left << "TopMargin" << " = " <<
      setprecision(4) << fTopMargin << " cm" << endl;
    emptyGeometry = false;
  }
  if (fBottomMargin > 0) {
    os <<
      idtr << setw(12) << left << "BottomMargin" << " = " <<
      setprecision(4) << fBottomMargin << " cm" << endl;
    emptyGeometry = false;
  }
  if (fLeftMargin > 0) {
    os <<
      idtr << setw(12) << left << "LeftMargin" << " = " <<
      setprecision(4) << fLeftMargin << " cm" << endl;
    emptyGeometry = false;
  }
  if (fRightMargin > 0) {
    os <<
      idtr << setw(12) << left << "RightMargin" << " = " <<
      setprecision(4) << fRightMargin << " cm" << endl;
    emptyGeometry = false;
  }

  if (fMillimeters > 0) {
    os <<
      idtr << setw(12) << left << "Millimeters" << " = " <<
      setprecision(4) << fMillimeters << endl;
    emptyGeometry = false;
  }

  if (fTenths > 0) {
    os <<
      idtr << setw(12) << left << "Tenths" << " = " <<
      setprecision(4) << fTenths << endl;
    emptyGeometry = false;
  }

  if (emptyGeometry)
    os << idtr <<
      " " << "nothing specified" << endl;
  
  os << endl;

  idtr--;
}

/*
  if (fBetweenSystemSpace > 0) {
    os << idtr <<
    * "between-system-space = " <<
    * setprecision(4) << fBetweenSystemSpace << "\\cm" << endl;
  }

  if (fPageTopSpace > 0) {
    os << idtr <<
    * "page-top-space = " <<
    * setprecision(4) << fPageTopSpace << "\\cm" << endl;
  }
*/


//______________________________________________________________________________

S_msrVarValAssoc msrVarValAssoc::create (
  S_msrOptions& msrOpts, 
  int           inputLineNumber,
  string        variableName,
  string        value)
{
  msrVarValAssoc* o =
    new msrVarValAssoc(
      msrOpts, inputLineNumber,
      variableName, value);
  assert(o!=0);
  return o;
}

msrVarValAssoc::msrVarValAssoc (
  S_msrOptions& msrOpts, 
  int           inputLineNumber,
  string        variableName,
  string        value)
    : msrElement (msrOpts, inputLineNumber)
{
  fVariableName    = variableName;
  fVariableValue   = value;
}

msrVarValAssoc::~msrVarValAssoc() {}

void msrVarValAssoc::acceptIn (basevisitor* v) {
  if (fMsrOptions->fDebugDebug)
    cerr << idtr <<
      "==> msrVarValAssoc::acceptIn()" << endl;
      
  if (visitor<S_msrVarValAssoc>*
    p =
      dynamic_cast<visitor<S_msrVarValAssoc>*> (v)) {
        S_msrVarValAssoc elem = this;
        
        if (fMsrOptions->fDebugDebug)
          cerr << idtr <<
            "==> Launching msrVarValAssoc::visitStart()" << endl;
        p->visitStart (elem);
  }
}

void msrVarValAssoc::acceptOut (basevisitor* v) {
  if (fMsrOptions->fDebugDebug)
    cerr << idtr <<
      "==> msrVarValAssoc::acceptOut()" << endl;

  if (visitor<S_msrVarValAssoc>*
    p =
      dynamic_cast<visitor<S_msrVarValAssoc>*> (v)) {
        S_msrVarValAssoc elem = this;
      
        if (fMsrOptions->fDebugDebug)
          cerr << idtr <<
            "==> Launching msrVarValAssoc::visitEnd()" << endl;
        p->visitEnd (elem);
  }
}


void msrVarValAssoc::browseData (basevisitor* v)
{}

ostream& operator<< (ostream& os, const S_msrVarValAssoc& assoc) {
  assoc->print (os);
  return os;
}

void msrVarValAssoc::print (ostream& os)
{
  os << "MSR VarValAssoc" << endl;
  
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
    idtr << "variable name : " <<
    "\"" << variableName << "\"" << endl <<
    idtr << "variable value: " <<
    "\"" << variableValue << "\"" << endl <<
    endl;
  
  idtr--;
}

//______________________________________________________________________________
S_msrLayout msrLayout::create (
  S_msrOptions& msrOpts, 
  int           inputLineNumber)
{
  msrLayout* o =
    new msrLayout (
      msrOpts, inputLineNumber);
  assert(o!=0);
  return o;
}

msrLayout::msrLayout (
  S_msrOptions& msrOpts, 
  int           inputLineNumber)
    : msrElement (msrOpts, inputLineNumber)
{}
msrLayout::~msrLayout() {}

void msrLayout::acceptIn (basevisitor* v) {
  if (fMsrOptions->fDebugDebug)
    cerr << idtr <<
      "==> msrLayout::acceptIn()" << endl;
      
  if (visitor<S_msrLayout>*
    p =
      dynamic_cast<visitor<S_msrLayout>*> (v)) {
        S_msrLayout elem = this;
        
        if (fMsrOptions->fDebugDebug)
          cerr << idtr <<
            "==> Launching msrLayout::visitStart()" << endl;
        p->visitStart (elem);
  }
}

void msrLayout::acceptOut (basevisitor* v) {
  if (fMsrOptions->fDebugDebug)
    cerr << idtr <<
      "==> msrLayout::acceptOut()" << endl;

  if (visitor<S_msrLayout>*
    p =
      dynamic_cast<visitor<S_msrLayout>*> (v)) {
        S_msrLayout elem = this;
      
        if (fMsrOptions->fDebugDebug)
          cerr << idtr <<
            "==> Launching msrLayout::visitEnd()" << endl;
        p->visitEnd (elem);
  }
}


void msrLayout::browseData (basevisitor* v)
{
  int n1 = fVarValAssocs.size();
  
  for (int i = 0; i < n1; i++ ) {
    // browse the lyrics
    msrBrowser<msrVarValAssoc> browser (v);
    browser.browse (*fVarValAssocs [i]);
  } // for
}

ostream& operator<< (ostream& os, const S_msrLayout& lay)
{
  lay->print (os);
  return os;
}

void msrLayout::print (ostream& os)
{
  os << "Layout" << endl;

  idtr++;

  int n1 = fVarValAssocs.size();
  
  for (int i = 0; i < n1; i++ ) {
    os << idtr << fVarValAssocs [i];
  } // for

    /* JMI
  int n2 = fMsrSchemeVarValAssocs.size();
  for (int i = 0; i < n2; i++ ) {
    os << idtr << fMsrSchemeVarValAssocs[i];
  } // for
  */
  
  idtr--;
}

//______________________________________________________________________________
S_msrClef msrClef::create (
  S_msrOptions& msrOpts, 
  int          inputLineNumber,
  string       sign,
  int          line,
  int          octaveChange)
{
  msrClef* o =
    new msrClef (
      msrOpts, inputLineNumber, sign, line, octaveChange);
  assert(o!=0);
  return o;
}

msrClef::msrClef (
  S_msrOptions& msrOpts, 
  int          inputLineNumber,
  string       sign,
  int          line,
  int          octaveChange)
    : msrElement (msrOpts, inputLineNumber)
{
  fSign         = sign;
  fLine         = line;
  fOctaveChange = octaveChange;
}

msrClef::~msrClef() {}

void msrClef::acceptIn (basevisitor* v) {
  if (fMsrOptions->fDebugDebug)
    cerr << idtr <<
      "==> msrClef::acceptIn()" << endl;
      
  if (visitor<S_msrClef>*
    p =
      dynamic_cast<visitor<S_msrClef>*> (v)) {
        S_msrClef elem = this;
        
        if (fMsrOptions->fDebugDebug)
          cerr << idtr <<
            "==> Launching msrClef::visitStart()" << endl;
        p->visitStart (elem);
  }
}

void msrClef::acceptOut (basevisitor* v) {
  if (fMsrOptions->fDebugDebug)
    cerr << idtr <<
      "==> msrClef::acceptOut()" << endl;

  if (visitor<S_msrClef>*
    p =
      dynamic_cast<visitor<S_msrClef>*> (v)) {
        S_msrClef elem = this;
      
        if (fMsrOptions->fDebugDebug)
          cerr << idtr <<
            "==> Launching msrClef::visitEnd()" << endl;
        p->visitEnd (elem);
  }
}

void msrClef::browseData (basevisitor* v)
{}

ostream& operator<< (ostream& os, const S_msrClef& clef)
{
  clef->print (os);
  return os;
}

string msrClef::clefAsString () const
{
  stringstream s;

  s <<
    "Clef" << " \"" << fSign << "\"" <<
    " line " << fLine;

  if (fOctaveChange != 0) {
    s << ", octaveChange:";
    
    switch (fOctaveChange) {
      case 1:
        s << "8";
        break;
      case -1:
        s << "-8";
        break;
      case 2:
        s << "15";
        break;
      case -2:
        s << "-15";
        break;
      default:
        s << "octaveChange" << fOctaveChange << "???";
    } // switch
  }

  return s.str();
}

void msrClef::print (ostream& os)
{
  os << clefAsString () << endl;
}

//______________________________________________________________________________
S_msrKey msrKey::create (
  S_msrOptions& msrOpts, 
  int           inputLineNumber,
  int           fifths,
  string        mode,
  int           cancel)
{
  msrKey* o =
    new msrKey (
      msrOpts, inputLineNumber, fifths, mode, cancel);
  assert (o!=0);
  return o;
}

msrKey::msrKey (
  S_msrOptions& msrOpts, 
  int           inputLineNumber,
  int           fifths,
  string        mode,
  int           cancel)
    : msrElement (msrOpts, inputLineNumber)
{
  fFifths = fifths;
  fMode   = mode;
  fCancel = cancel;

  string           tonicNote;
  msrKey::msrKeyMode keyMode;
  
  switch (fFifths) {
    case 0:
      tonicNote = "c";
      keyMode = msrKey::kMajor;
      break;
    case 1:
      tonicNote = "g";
      keyMode = msrKey::kMajor;
      break;
    case 2:
      tonicNote = "d";
      keyMode = msrKey::kMajor;
      break;
    case 3:
      tonicNote = "a";
      keyMode = msrKey::kMajor;
      break;
    case 4:
      tonicNote = "e";
      keyMode = msrKey::kMajor;
      break;
    case 5:
      tonicNote = "b";
      keyMode = msrKey::kMajor;
      break;
    case 6:
       tonicNote = "fis";
      keyMode = msrKey::kMajor;
      break;
    case 7:
      tonicNote = "cis";
      keyMode = msrKey::kMajor;
      break;
    case -1:
      tonicNote = "f";
      keyMode = msrKey::kMajor;
      break;
    case -2:
      tonicNote = "bes";
      keyMode = msrKey::kMajor;
      break;
    case -3:
      tonicNote = "ees";
      keyMode = msrKey::kMajor;
      break;
    case -4:
      tonicNote = "aes";
      keyMode = msrKey::kMajor;
      break;
    case -5:
      tonicNote = "des";
      keyMode = msrKey::kMajor;
      break;
    case -6:
      tonicNote = "ges";
      keyMode = msrKey::kMajor;
      break;
    case -7:
      tonicNote = "ces";
      keyMode = msrKey::kMajor;
      break;
    default: // unknown key sign !!
      {
      stringstream s;
      s << 
        "ERROR: unknown key sign \"" << fFifths << "\"" << endl;
      msrMusicXMLError (
        fMsrOptions->fInputSourceName,
        fInputLineNumber,
        s.str());
      }
  } // switch
  
  fTonic   = tonicNote;
  fKeyMode = keyMode; 
}

msrKey::~msrKey() {}

void msrKey::acceptIn (basevisitor* v) {
  if (fMsrOptions->fDebugDebug)
    cerr << idtr <<
      "==> msrKey::acceptIn()" << endl;
      
  if (visitor<S_msrKey>*
    p =
      dynamic_cast<visitor<S_msrKey>*> (v)) {
        S_msrKey elem = this;
        
        if (fMsrOptions->fDebugDebug)
          cerr << idtr <<
            "==> Launching msrKey::visitStart()" << endl;
        p->visitStart (elem);
  }
}

void msrKey::acceptOut (basevisitor* v) {
  if (fMsrOptions->fDebugDebug)
    cerr << idtr <<
      "==> msrKey::acceptOut()" << endl;

  if (visitor<S_msrKey>*
    p =
      dynamic_cast<visitor<S_msrKey>*> (v)) {
        S_msrKey elem = this;
      
        if (fMsrOptions->fDebugDebug)
          cerr << idtr <<
            "==> Launching msrKey::visitEnd()" << endl;
        p->visitEnd (elem);
  }
}


void msrKey::browseData (basevisitor* v)
{}

ostream& operator<< (ostream& os, const S_msrKey& key)
{
  key->print (os);
  return os;
}

string msrKey::keyAsString () const
{
  stringstream s;

  s << "Key " << fTonic << " ";
  if (fKeyMode == kMajor)
    s << "\\major";
  else
    s << "\\minor";

  return s.str();
}

void msrKey::print (ostream& os)
{
  os << keyAsString () << endl;
}

//______________________________________________________________________________
S_msrTime msrTime::create (
  S_msrOptions& msrOpts, 
  int           inputLineNumber,
  int           beatsNumber,
  int           beatsValue)
{
  msrTime* o =
    new msrTime (
      msrOpts, inputLineNumber,
      beatsNumber, beatsValue);
  assert (o!=0);
  return o;
}

msrTime::msrTime (
  S_msrOptions& msrOpts, 
  int           inputLineNumber,
  int           beatsNumber,
  int           beatsValue)
    : msrElement (msrOpts, inputLineNumber)
{
  fBeatsNumber = beatsNumber;
  fBeatsValue  = beatsValue;
}

msrTime::~msrTime() {}

void msrTime::acceptIn (basevisitor* v) {
  if (fMsrOptions->fDebugDebug)
    cerr << idtr <<
      "==> msrTime::acceptIn()" << endl;
      
  if (visitor<S_msrTime>*
    p =
      dynamic_cast<visitor<S_msrTime>*> (v)) {
        S_msrTime elem = this;
        
        if (fMsrOptions->fDebugDebug)
          cerr << idtr <<
            "==> Launching msrTime::visitStart()" << endl;
        p->visitStart (elem);
  }
}

void msrTime::acceptOut (basevisitor* v) {
  if (fMsrOptions->fDebugDebug)
    cerr << idtr <<
      "==> msrTime::acceptOut()" << endl;

  if (visitor<S_msrTime>*
    p =
      dynamic_cast<visitor<S_msrTime>*> (v)) {
        S_msrTime elem = this;
      
        if (fMsrOptions->fDebugDebug)
          cerr << idtr <<
            "==> Launching msrTime::visitEnd()" << endl;
        p->visitEnd (elem);
  }
}

void msrTime::browseData (basevisitor* v)
{}

ostream& operator<< (ostream& os, const S_msrTime& elt)
{
  elt->print (os);
  return os;
}

string msrTime::timeAsString () const
{
  stringstream s;

  s <<
    "Time " << 
    fBeatsNumber << "/" << fBeatsValue;

  return s.str();
}

void msrTime::print (ostream& os)
{
  os << timeAsString () << endl;
}

//______________________________________________________________________________
S_msrTranspose msrTranspose::create (
  S_msrOptions& msrOpts, 
  int           inputLineNumber,
  int           transposeDiatonic,
  int           transposeChromatic)
{
  msrTranspose* o =
    new msrTranspose (
      msrOpts, inputLineNumber,
      transposeDiatonic, transposeChromatic);
  assert (o!=0);
  return o;
}

msrTranspose::msrTranspose (
  S_msrOptions& msrOpts, 
  int           inputLineNumber,
  int           transposeDiatonic,
  int           transposeChromatic)
    : msrElement (msrOpts, inputLineNumber)
{
  fTransposeDiatonic  = transposeDiatonic;
  fTransposeChromatic = transposeChromatic;
}

msrTranspose::~msrTranspose() {}

void msrTranspose::acceptIn (basevisitor* v) {
  if (fMsrOptions->fDebugDebug)
    cerr << idtr <<
      "==> msrTranspose::acceptIn()" << endl;
      
  if (visitor<S_msrTranspose>*
    p =
      dynamic_cast<visitor<S_msrTranspose>*> (v)) {
        S_msrTranspose elem = this;
        
        if (fMsrOptions->fDebugDebug)
          cerr << idtr <<
            "==> Launching msrTranspose::visitStart()" << endl;
        p->visitStart (elem);
  }
}

void msrTranspose::acceptOut (basevisitor* v) {
  if (fMsrOptions->fDebugDebug)
    cerr << idtr <<
      "==> msrTranspose::acceptOut()" << endl;

  if (visitor<S_msrTranspose>*
    p =
      dynamic_cast<visitor<S_msrTranspose>*> (v)) {
        S_msrTranspose elem = this;
      
        if (fMsrOptions->fDebugDebug)
          cerr << idtr <<
            "==> Launching msrTranspose::visitEnd()" << endl;
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
    "Transpose " << 
    "diatonic = " << fTransposeDiatonic <<
    ", chromatic= " << fTransposeChromatic;

  return s.str();
}

void msrTranspose::print (ostream& os)
{
  os << transposeAsString () << endl;
}

//______________________________________________________________________________
S_msrWords msrWords::create (
  S_msrOptions&         msrOpts, 
  int                   inputLineNumber,
  msrWordsPlacementKind wordsPlacementKind,
  string                wordsContents,
  string                wordsFontStyle,
  string                wordsFontSize,
  string                wordsFontWeight,
  string                wordsFontXMLLang)
{
  msrWords* o =
    new msrWords (
      msrOpts, inputLineNumber,
      wordsPlacementKind,
      wordsContents,
      wordsFontStyle,
      wordsFontSize,
      wordsFontWeight,
      wordsFontXMLLang);
  assert(o!=0);
  return o;
}

msrWords::msrWords (
  S_msrOptions&         msrOpts, 
  int                   inputLineNumber,
  msrWordsPlacementKind wordsPlacementKind,
  string                wordsContents,
  string                wordsFontStyle,
  string                wordsFontSize,
  string                wordsFontWeight,
  string                wordsFontXMLLang)
    : msrElement (msrOpts, inputLineNumber)
{
  fWordsPlacementKind = wordsPlacementKind;
  
  fWordsContents      = wordsContents;

  fWordsFontStyle     = wordsFontStyle;
  fWordsFontSize      = wordsFontSize;
  fWordsFontWeight    = wordsFontWeight;
  fWordsFontXMLLang   = wordsFontXMLLang;
}
msrWords::~msrWords() {}

void msrWords::acceptIn (basevisitor* v) {
  if (fMsrOptions->fDebugDebug)
    cerr << idtr <<
      "==> msrWords::acceptIn()" << endl;
      
  if (visitor<S_msrWords>*
    p =
      dynamic_cast<visitor<S_msrWords>*> (v)) {
        S_msrWords elem = this;
        
        if (fMsrOptions->fDebugDebug)
          cerr << idtr <<
            "==> Launching msrWords::visitStart()" << endl;
        p->visitStart (elem);
  }
}

void msrWords::acceptOut (basevisitor* v) {
  if (fMsrOptions->fDebugDebug)
    cerr << idtr <<
      "==> msrWords::acceptOut()" << endl;

  if (visitor<S_msrWords>*
    p =
      dynamic_cast<visitor<S_msrWords>*> (v)) {
        S_msrWords elem = this;
      
        if (fMsrOptions->fDebugDebug)
          cerr << idtr <<
            "==> Launching msrWords::visitEnd()" << endl;
        p->visitEnd (elem);
  }
}


void msrWords::browseData (basevisitor* v)
{}

ostream& operator<< (ostream& os, const S_msrWords& nstf)
{
  nstf->print (os);
  return os;
}

string msrWords::wordsAsString () const
{
  stringstream s;

  s <<
    "Words" << " " <<
    fWordsContents << ", placement = ";

  switch (fWordsPlacementKind) {
    case kAbove:
      s << "above";
      break;
    case kBelow:
      s << "below";
      break;
  } // switch

  return s.str();
}

void msrWords::print (ostream& os)
{
// JMI  os << wordsAsString () << endl;

  os <<
    "Words" " \"" << fWordsContents << "\"" <<
    endl;

  idtr ++;

  os <<
    idtr <<
      setw(16) << "Placement" << " = ";

  switch (fWordsPlacementKind) {
    case kAbove:
      os << "above";
      break;
    case kBelow:
      os << "below";
      break;
  } // switch
  os << endl;

  os << left <<
    idtr <<
      setw(16) << "WordsFontStyle" << " = " << fWordsFontStyle <<
      endl <<
    idtr <<
      setw(16) << "WordsFontSize" << " = " << fWordsFontSize <<
      endl <<
    idtr <<
      setw(16) << "WordsFontWeight" << " = " << fWordsFontWeight <<
      endl <<
    idtr <<
      setw(16) << "WordsFontXMLLang" << " = " << fWordsFontXMLLang <<
      endl;
  
  idtr--;
}

//______________________________________________________________________________
S_msrTempo msrTempo::create (
  S_msrOptions& msrOpts, 
  int           inputLineNumber,
  int           tempoUnit,
  int           perMinute)
{
  msrTempo* o =
    new msrTempo (
      msrOpts, inputLineNumber, tempoUnit, perMinute);
  assert(o!=0);
  return o;
}

msrTempo::msrTempo (
  S_msrOptions& msrOpts, 
  int           inputLineNumber,
  int           tempoUnit,
  int           perMinute)
    : msrElement (msrOpts, inputLineNumber)
{
  fTempoIndication = "";
  
  fTempoUnit = tempoUnit;
  fPerMinute = perMinute;
}
msrTempo::~msrTempo() {}

void msrTempo::setTempoIndication (string indication)
{
  if (fMsrOptions->fDebugDebug)
    cerr << idtr <<
      "Setting indication of tempo " <<
      fTempoUnit << " = " << fPerMinute <<
      " to \"" << indication << "\"" <<
      endl;
      
  fTempoIndication = indication;
}

void msrTempo::acceptIn (basevisitor* v) {
  if (fMsrOptions->fDebugDebug)
    cerr << idtr <<
      "==> msrTempo::acceptIn()" << endl;
      
  if (visitor<S_msrTempo>*
    p =
      dynamic_cast<visitor<S_msrTempo>*> (v)) {
        S_msrTempo elem = this;
        
        if (fMsrOptions->fDebugDebug)
          cerr << idtr <<
            "==> Launching msrTempo::visitStart()" << endl;
        p->visitStart (elem);
  }
}

void msrTempo::acceptOut (basevisitor* v) {
  if (fMsrOptions->fDebugDebug)
    cerr << idtr <<
      "==> msrTempo::acceptOut()" << endl;

  if (visitor<S_msrTempo>*
    p =
      dynamic_cast<visitor<S_msrTempo>*> (v)) {
        S_msrTempo elem = this;
      
        if (fMsrOptions->fDebugDebug)
          cerr << idtr <<
            "==> Launching msrTempo::visitEnd()" << endl;
        p->visitEnd (elem);
  }
}


void msrTempo::browseData (basevisitor* v)
{}

ostream& operator<< (ostream& os, const S_msrTempo& nstf)
{
  nstf->print (os);
  return os;
}

string msrTempo::tempoAsString () const
{
  stringstream s;

  s <<
    "Tempo" << " " <<
    ", indication = \"" << fTempoIndication << "\"" <<
    ", " << fTempoUnit << " = " << fPerMinute;

  return s.str();
}

void msrTempo::print (ostream& os)
{
  os << tempoAsString () << endl;
}

//______________________________________________________________________________
S_msrLyricschunk msrLyricschunk::create (
  S_msrOptions&      msrOpts, 
  int                inputLineNumber,
  msrLyricschunkKind lyricschunkKind,
  string             chunkText,
  int                divisions,
  S_msrNote          lyricschunkNote,
  S_msrLyrics        lyricschunkLyricsUplink)
{
  msrLyricschunk* o =
    new msrLyricschunk (
      msrOpts, inputLineNumber,
      lyricschunkKind, chunkText, divisions,
      lyricschunkNote, lyricschunkLyricsUplink);
  assert(o!=0);
  return o;
}

msrLyricschunk::msrLyricschunk (
  S_msrOptions&      msrOpts, 
  int                inputLineNumber,
  msrLyricschunkKind lyricschunkKind,
  string             chunkText,
  int                divisions,
  S_msrNote          lyricschunkNote,
  S_msrLyrics        lyricschunkLyricsUplink)
    : msrElement (msrOpts, inputLineNumber)
{
  fLyricschunkKind = lyricschunkKind;
  fChunkText       = chunkText;
  fChunkDivisions  = divisions;

  fLyricschunkNote = lyricschunkNote;
  fLyricschunkLyricsUplink = lyricschunkLyricsUplink;
}

msrLyricschunk::~msrLyricschunk() {}

S_msrLyricschunk msrLyricschunk::createEmptyClone ()
{
  S_msrLyricschunk
    clone =
      msrLyricschunk::create (
        fMsrOptions,
        fInputLineNumber,
        fLyricschunkKind,
        fChunkText,
        fChunkDivisions,
        fLyricschunkNote,
        fLyricschunkLyricsUplink);
  
  return clone;
}

void msrLyricschunk::acceptIn (basevisitor* v) {
  if (fMsrOptions->fDebugDebug)
    cerr << idtr <<
      "==> msrLyricschunk::acceptIn()" << endl;
      
  if (visitor<S_msrLyricschunk>*
    p =
      dynamic_cast<visitor<S_msrLyricschunk>*> (v)) {
        S_msrLyricschunk elem = this;
        
        if (fMsrOptions->fDebugDebug)
          cerr << idtr <<
            "==> Launching msrLyricschunk::visitStart()" << endl;
        p->visitStart (elem);
  }
}

void msrLyricschunk::acceptOut (basevisitor* v) {
  if (fMsrOptions->fDebugDebug)
    cerr << idtr <<
      "==> msrLyricschunk::acceptOut()" << endl;

  if (visitor<S_msrLyricschunk>*
    p =
      dynamic_cast<visitor<S_msrLyricschunk>*> (v)) {
        S_msrLyricschunk elem = this;
      
        if (fMsrOptions->fDebugDebug)
          cerr << idtr <<
            "==> Launching msrLyricschunk::visitEnd()" << endl;
        p->visitEnd (elem);
  }
}

void msrLyricschunk::browseData (basevisitor* v)
{}

ostream& operator<< (ostream& os, const S_msrLyricschunk& lyr)
{
  lyr->print (os);
  return os;
}

string msrLyricschunk::lyricschunkKindAsString ()
{
  string result;
    
  switch (fLyricschunkKind) {
    case msrLyricschunk::kSingleChunk:
      result = "single";
      break;
    case msrLyricschunk::kBeginChunk:
      result = "begin";
      break;
    case msrLyricschunk::kMiddleChunk:
      result = "middle";
      break;
    case msrLyricschunk::kEndChunk:
      result = "end";
      break;
      
    case msrLyricschunk::kSkipChunk:
      result = "skip";
      break;
      
    case msrLyricschunk::kSlurChunk:
      result = "slur";
      break;
    case msrLyricschunk::kSlurBeyondEndChunk:
      result = "slur beyond end";
      break;
      
    case msrLyricschunk::kTiedChunk:
      result = "tied";
      break;
      
    case msrLyricschunk::kBarcheckChunk:
      result = "barcheck";
      break;
      
    case msrLyricschunk::kBreakChunk:
      result = "break";
      break;
      
    case msrLyricschunk::k_NoChunk:
      msrInternalError (
        fMsrOptions->fInputSourceName,
        fInputLineNumber,
        "lyrics chunk type has not been set");
      break;
  } // switch

  return result;
}

string msrLyricschunk::lyricschunkAsString ()
{
  stringstream s;
  
  switch (fLyricschunkKind) {
    case kSingleChunk:
      s << "single" << ":" << fChunkDivisions;
      if (fChunkText.size())
        s << " " << "\"" << fChunkText << "\"";
      s <<
        ", line " << fInputLineNumber <<
        ", " << fLyricschunkNote->notePitchAsString () <<
        ":" << fLyricschunkNote->noteDivisionsAsMSRString () <<
        endl;
      break;
      
    case kBeginChunk:
      s << "begin" << ":" << fChunkDivisions;
      if (fChunkText.size())
        s << " " << "\"" << fChunkText << "\"";
      s <<
        ", line " << fInputLineNumber <<
        ", " << fLyricschunkNote->notePitchAsString () <<
        ":" << fLyricschunkNote->noteDivisionsAsMSRString () <<
        endl;
      break;
      
    case kMiddleChunk:
      s << "middle" << ":" << fChunkDivisions;
      if (fChunkText.size())
        s << " " << "\"" << fChunkText << "\"";
      s <<
        ", line " << fInputLineNumber <<
        ", " << fLyricschunkNote->notePitchAsString () <<
        ":" << fLyricschunkNote->noteDivisionsAsMSRString () <<
        endl;
      break;
      
    case kEndChunk:
      s << "end" << ":" << fChunkDivisions;
      if (fChunkText.size())
        s << " " << "\"" << fChunkText << "\"";
      s <<
        ", line " << fInputLineNumber <<
        ", " << fLyricschunkNote->notePitchAsString () <<
        ":" << fLyricschunkNote->noteDivisionsAsMSRString () <<
        endl;
      break;
      
    case kSkipChunk:
      s << "skip" << ":" << fChunkDivisions;
      if (fChunkText.size())
        s << " " << fChunkText;
      s <<
        ", line " << fInputLineNumber <<
        " (" <<
        fLyricschunkNote->notePitchAsString () <<
        ":" <<
        fLyricschunkNote->noteDivisionsAsMSRString () <<
        ")" <<
        endl;
      break;
      
    case kSlurChunk:
      s << "slur" << ":" << fChunkDivisions;
      if (fChunkText.size())
        s << " " << fChunkText;
      s <<
        ", line " << fInputLineNumber <<
        ", " << fLyricschunkNote->notePitchAsString () <<
        ":" << fLyricschunkNote->noteDivisionsAsMSRString () <<
        endl;
      break;
      
    case kSlurBeyondEndChunk:
      s << "slur beyond end" << ":" << fChunkDivisions;
      if (fChunkText.size())
        s << " " << fChunkText;
      s <<
        ", line " << fInputLineNumber <<
        ", " << fLyricschunkNote->notePitchAsString () <<
        ":" << fLyricschunkNote->noteDivisionsAsMSRString () <<
        endl;
      break;
      
    case kTiedChunk:
      s << "tied" << ":" << fChunkDivisions;
      if (fChunkText.size())
        s << " " << fChunkText;
      s <<
        ", line " << fInputLineNumber <<
        ", " << fLyricschunkNote->notePitchAsString () <<
        ":" << fLyricschunkNote->noteDivisionsAsMSRString () <<
        endl;
      break;
      
    case kBarcheckChunk:
      // fChunkText contains the measure number
      s << "barCheck" << " measure " << fChunkText << endl;
      break;
      
    case kBreakChunk:
      // fChunkText contains the measure number
      s << "break" << " measure " << fChunkText << endl << endl;
      break;
      
    case k_NoChunk:
      msrInternalError (
        fMsrOptions->fInputSourceName,
        fInputLineNumber,
        "lyrics chunk type has not been set");
      break;
  } // switch

  return s.str();
}


void msrLyricschunk::print (ostream& os)
{  
  os <<
    "Lyricschunk" << " " << setw(15) << lyricschunkAsString ();
}

//______________________________________________________________________________
S_msrLyrics msrLyrics::create (
  S_msrOptions&         msrOpts, 
  int                   inputLineNumber,
  int                   lyricsNumber,
  msrLyricsMasterStatus lyricsMasterStatus,
  S_msrVoice            lyricsVoiceUplink)
{
  msrLyrics* o =
    new msrLyrics (
      msrOpts, inputLineNumber,
      lyricsNumber,
      lyricsMasterStatus,
      lyricsVoiceUplink);
  assert(o!=0);
  return o;
}

msrLyrics::msrLyrics (
  S_msrOptions&         msrOpts, 
  int                   inputLineNumber,
  int                   lyricsNumber,
  msrLyricsMasterStatus lyricsMasterStatus,
  S_msrVoice            lyricsVoiceUplink)
    : msrElement (msrOpts, inputLineNumber)
{
  fLyricsNumber       = lyricsNumber;
  fLyricsMasterStatus = lyricsMasterStatus;
 
  fLyricsVoiceUplink  = lyricsVoiceUplink;
  
  if (fMsrOptions->fTrace)
    cerr << idtr <<
      "Creating lyrics " << getLyricsName () << endl;

  fLyricsTextPresent = false;
}

string msrLyrics::getLyricsName () const
{
  // not stored in a field,
  // because the lyrics voice and staff may change name
  // when the part they belong to is re-used

  string
    lyricsNameSuffix =
      fLyricsMasterStatus == kMasterLyrics
        ? "Master"
        : int2EnglishWord (fLyricsNumber);
        
  return
   fLyricsVoiceUplink->getVoiceName() +
    "_L_" +
    lyricsNameSuffix;
}

msrLyrics::~msrLyrics() {}

S_msrLyrics msrLyrics::createEmptyClone (S_msrVoice clonedVoice)
{
  S_msrLyrics
    clone =
      msrLyrics::create (
        fMsrOptions,
        fInputLineNumber,
        fLyricsNumber,
        fLyricsMasterStatus,
        clonedVoice);
  
  return clone;
}

void msrLyrics::addTextChunkToLyrics (
  int       inputLineNumber,
  string    syllabic,
  msrLyricschunk::msrLyricschunkKind
            lyricschunkKind,
  string    text,
  bool      elision,
  int       divisions,
  S_msrNote note)
{
  // create a lyrics text chunk
  if (fMsrOptions->fForceDebug || fMsrOptions->fDebug) {
    S_msrStaff
      staff =
        fLyricsVoiceUplink->getVoiceStaffUplink ();
    S_msrPart
      part =
        staff-> getStaffPartUplink ();
    
    cerr << idtr <<
      "--> Adding text lyrics chunk"
      ", line " << inputLineNumber <<
      ", divisions = " << divisions << 
      ", syllabic = \"" << syllabic << "\"" <<
      ", text = \"" << text << "\"";

/*
    string lyricschunkKindAsString =
      lyricschunkAsString ();
  */
    cerr <<
 //     ", type = \"" << lyricschunkKindAsString << "\"" <<
      ", elision: " << elision << 
      " to lyrics " << getLyricsName () << endl;
  }

  // create text lyrics chunk
  S_msrLyricschunk
    chunk =
      msrLyricschunk::create (
        fMsrOptions,
        inputLineNumber,
        lyricschunkKind, text, divisions,
        note,
        this);

  // add chunk to this lyrics
  fLyricschunks.push_back (chunk);

  fLyricsTextPresent = true;
}

void msrLyrics::addSkipChunkToLyrics (
  int       inputLineNumber,
  int       divisions,
  S_msrNote note)
{
  if (fMsrOptions->fForceDebug || fMsrOptions->fDebug) {
    S_msrStaff
      staff =
        fLyricsVoiceUplink->getVoiceStaffUplink ();
    S_msrPart
      part =
        staff-> getStaffPartUplink ();
    
    cerr << idtr <<
      "--> Adding 'Skip' lyrics chunk:" << divisions <<
      " to lyrics " << getLyricsName () << endl;
  }
  
  // create lyrics skip chunk
  S_msrLyricschunk
    lyricschunk =
      msrLyricschunk::create (
        fMsrOptions,
        inputLineNumber,
        msrLyricschunk::kSkipChunk, "", divisions,
        note,
        this);
  
  // add chunk to this lyrics
  fLyricschunks.push_back (lyricschunk);

/*
  if (fMsrOptions->fForceDebug || fMsrOptions->fDebug) {
    S_msrStaff staff = fLyricsVoiceUplink->getVoiceStaffUplink ();
    S_msrPart  part  = staff-> getStaffPartUplink ();
    
    cerr << idtr <<
      "--> Adding 'Skip' lyrics chunk:" << divisions <<
      " to " << getLyricsName () << endl;

    string result;
    int    computedNumberOfDots; // value not used
    string errorMessage;
  
    int divisionsPerWholeNote =
      fLyricsVoiceUplink->getDivisionsPerWholeNote ();
  
    string
      divisionsAsString =
        divisionsAsMSRDuration (
          divisions,
          divisionsPerWholeNote,
          computedNumberOfDots,
          errorMessage,
          false); // 'true' to debug it;
    
    if (errorMessage.size ())
      msrMusicXMLError (
        fMsrOptions->fInputSourceName,
        fInputLineNumber,
        errorMessage);

    cerr << idtr <<
      "--> Adding 'Skip' lyrics chunk:" << divisions <<
      "/" << divisionsPerWholeNote << "" <<
      "(" << divisionsAsString << ")" <<
      " to lyrics " << getLyricsName () << endl;
  }
  */
}

void msrLyrics::addTiedChunkToLyrics (
  int       inputLineNumber,
  int       divisions,
  S_msrNote note)
{
  if (fMsrOptions->fForceDebug || fMsrOptions->fDebug) {
    S_msrStaff
      staff =
        fLyricsVoiceUplink->getVoiceStaffUplink ();
    S_msrPart
      part =
        staff-> getStaffPartUplink ();
    
    cerr << idtr <<
      "--> Adding 'Tied' lyrics chunk: " << divisions <<
      " to lyrics " << getLyricsName () << endl;
  }
  
  // create lyrics tied chunk
  S_msrLyricschunk
    chunk =
      msrLyricschunk::create (
        fMsrOptions,
        inputLineNumber,
        msrLyricschunk::kTiedChunk, "", divisions,
        note,
        this);
        
  // add chunk to this lyrics
  fLyricschunks.push_back (chunk);
}

void msrLyrics::addSlurChunkToLyrics (
  int       inputLineNumber,
  int       divisions,
  S_msrNote note)
{
  if (fMsrOptions->fForceDebug || fMsrOptions->fDebug) {
    S_msrStaff
      staff =
        fLyricsVoiceUplink->getVoiceStaffUplink ();
    S_msrPart
      part =
        staff-> getStaffPartUplink ();
    
    cerr << idtr <<
      "--> Adding 'Slur' lyrics chunk:" << divisions <<
      " to lyrics " << getLyricsName () << endl;
  }
  
  // create lyrics slur chunk
  S_msrLyricschunk
    chunk =
      msrLyricschunk::create (
        fMsrOptions,
        inputLineNumber,
        msrLyricschunk::kSlurChunk, "", divisions,
        note,
        this);
        
  // add chunk to this lyrics
  fLyricschunks.push_back (chunk);
}

void msrLyrics::addSlurBeyondEndChunkToLyrics (
  int       inputLineNumber,
  int       divisions,
  S_msrNote note)
{
  if (fMsrOptions->fForceDebug || fMsrOptions->fDebug) {
    S_msrStaff
      staff =
        fLyricsVoiceUplink->getVoiceStaffUplink ();
    S_msrPart
      part =
        staff-> getStaffPartUplink ();
    
    cerr << idtr <<
      "--> Adding a 'SlurBeyondEnd' lyrics chunk: " << divisions <<
      " to lyrics " << getLyricsName () << endl;
  }
  
  // create lyrics slur chunk
  S_msrLyricschunk
    chunk =
      msrLyricschunk::create (
        fMsrOptions,
        inputLineNumber,
        msrLyricschunk::kSlurBeyondEndChunk, "", divisions,
        note,
        this);
        
  // add chunk to this lyrics
  fLyricschunks.push_back (chunk);
}

void msrLyrics::addBarcheckChunkToLyrics (
  int inputLineNumber,
  int nextMeasureNumber)
{
  if (fMsrOptions->fForceDebug || fMsrOptions->fDebug) {
    S_msrStaff
      staff =
        fLyricsVoiceUplink->getVoiceStaffUplink ();
    S_msrPart
      part =
        staff-> getStaffPartUplink ();
    
    cerr << idtr <<
      "--> Adding a 'barcheck' lyrics chunk" <<
      " to lyrics " << getLyricsName () << endl;
  }

  // convert nextMeasureNumber to string
  stringstream s;
  s << nextMeasureNumber;
  
  // create lyrics break chunk
  /*
  S_msrDuration
    nullMsrDuration =
      msrDuration::create (
        fMsrOptions,
        inputLineNumber,
        0, 1, 0, "");
    */    
  S_msrLyricschunk
    chunk =
      msrLyricschunk::create (
        fMsrOptions,
        inputLineNumber,
        msrLyricschunk::kBarcheckChunk,
        s.str(),
        0,
        0,
        this);
        
  // add chunk to this lyrics
  fLyricschunks.push_back (chunk);
}

void msrLyrics::addBreakChunkToLyrics (
  int inputLineNumber,
  int nextMeasureNumber)
{
  if (fMsrOptions->fForceDebug || fMsrOptions->fDebug) {
    S_msrStaff
      staff =
        fLyricsVoiceUplink->getVoiceStaffUplink ();
    S_msrPart
      part =
        staff-> getStaffPartUplink ();
    
    cerr << idtr <<
      "--> Adding a 'Break' lyrics chunk" <<
      " to lyrics " << getLyricsName () << endl;
  }

  // convert nextMeasureNumber to string
  stringstream s;
  s << nextMeasureNumber;
  
  // create lyrics break chunk
  S_msrLyricschunk
    chunk =
      msrLyricschunk::create (
        fMsrOptions,
        inputLineNumber,
        msrLyricschunk::kBreakChunk,
        s.str(),
        0,
        0,
        this);
        
  // add chunk to this lyrics
  fLyricschunks.push_back (chunk);
}

void msrLyrics::addChunkToLyrics (S_msrLyricschunk chunk)
{
  if (fMsrOptions->fDebugDebug)
    cerr << idtr <<
      "--> Adding chunk to lyrics" << getLyricsName () << endl;
      
  fLyricschunks.push_back (chunk);
}

void msrLyrics::acceptIn (basevisitor* v) {
  if (fMsrOptions->fDebugDebug)
    cerr << idtr <<
      "==> msrLyrics::acceptIn()" << endl;
      
  if (visitor<S_msrLyrics>*
    p =
      dynamic_cast<visitor<S_msrLyrics>*> (v)) {
        S_msrLyrics elem = this;
        
        if (fMsrOptions->fDebugDebug)
          cerr << idtr <<
            "==> Launching msrLyrics::visitStart()" << endl;
        p->visitStart (elem);
  }
}

void msrLyrics::acceptOut (basevisitor* v) {
  if (fMsrOptions->fDebugDebug)
    cerr << idtr <<
      "==> msrLyrics::acceptOut()" << endl;

  if (visitor<S_msrLyrics>*
    p =
      dynamic_cast<visitor<S_msrLyrics>*> (v)) {
        S_msrLyrics elem = this;
      
        if (fMsrOptions->fDebugDebug)
          cerr << idtr <<
            "==> Launching msrLyrics::visitEnd()" << endl;
        p->visitEnd (elem);
  }
}

void msrLyrics::browseData (basevisitor* v)
{
  if (fMsrOptions->fDebugDebug)
    cerr << idtr <<
      "==> msrLyrics::browseData()" << endl;

  idtr++;
  
  // browse the lyrics chunks
  int n = fLyricschunks.size ();
  for (int i = 0; i < n; i++) {
    // browse the lyrics
    msrBrowser<msrLyricschunk> browser (v);
    browser.browse (*fLyricschunks [i]);
  } // for
  cerr << endl;

  idtr--;

  if (fMsrOptions->fDebug)
    cerr << idtr <<
      "<== msrLyrics::browseData()" << endl;
}

ostream& operator<< (ostream& os, const S_msrLyrics& stan)
{
  stan->print (os);
  return os;
}

void msrLyrics::print (ostream& os)
{  
  os <<
    "Lyrics" << " " << getLyricsName () <<
    " (" << fLyricschunks.size () << " lyrics chunks)";
    
  if (! fLyricsTextPresent)
    os << " (No actual text)";

  os << endl;

//  if (fLyricsTextPresent) {  JMI
    idtr++;

    int n = fLyricschunks.size();
    for (int i = 0; i < n; i++) {
      os << idtr << fLyricschunks [i];
    } // for
    os << endl;

    idtr--;
 // }
}

//______________________________________________________________________________
S_msrBarline msrBarline::create (
  S_msrOptions&             msrOpts, 
  int                       inputLineNumber,
  bool                      barlineHasSegno,
  bool                      barlineHasCoda,
  msrBarlineLocation        location,
  msrBarlineStyle           style,
  msrBarlineEndingType      endingType,
  string                    endingMusicXMLNumber,
  msrBarlineRepeatDirection repeatDirection,
  msrBarlineRepeatWinged    repeatWinged)
{
  msrBarline* o =
    new msrBarline (
      msrOpts, inputLineNumber,
      barlineHasSegno, barlineHasCoda,
      location, style,
      endingType, endingMusicXMLNumber,
      repeatDirection, repeatWinged);
  assert(o!=0);
  return o;
}

msrBarline::msrBarline (
  S_msrOptions&             msrOpts, 
  int                       inputLineNumber,
  bool                      barlineHasSegno,
  bool                      barlineHasCoda,
  msrBarlineLocation        location,
  msrBarlineStyle           style,
  msrBarlineEndingType      endingType,
  string                    endingMusicXMLNumber,
  msrBarlineRepeatDirection repeatDirection,
  msrBarlineRepeatWinged    repeatWinged)
    : msrElement (msrOpts, inputLineNumber)
{
  fBarlineHasSegno = barlineHasSegno;
  fBarlineHasCoda  = barlineHasCoda;
  
  fLocation        = location;
  fStyle           = style;
  fEndingType      = endingType;
  fEndingNumber    = endingMusicXMLNumber;
  fRepeatDirection = repeatDirection;
  fRepeatWinged    = repeatWinged;
  
  // JMI cerr << "fEndingNumber = " << fEndingNumber << endl;
  
  // extract individual numbers from fEndingNumber
  // that may contain "1, 2"
  fEndingNumbersList =
    extractNumbersFromString (
      fEndingNumber, fMsrOptions->fDebugDebug);
}

msrBarline::~msrBarline() {}

void msrBarline::acceptIn (basevisitor* v) {
  if (fMsrOptions->fDebugDebug)
    cerr << idtr <<
      "==> msrBarline::acceptIn()" << endl;
      
  if (visitor<S_msrBarline>*
    p =
      dynamic_cast<visitor<S_msrBarline>*> (v)) {
        S_msrBarline elem = this;
        
        if (fMsrOptions->fDebugDebug)
          cerr << idtr <<
            "==> Launching msrBarline::visitStart()" << endl;
        p->visitStart (elem);
  }
}

void msrBarline::acceptOut (basevisitor* v) {
  if (fMsrOptions->fDebugDebug)
    cerr << idtr <<
      "==> msrBarline::acceptOut()" << endl;

  if (visitor<S_msrBarline>*
    p =
      dynamic_cast<visitor<S_msrBarline>*> (v)) {
        S_msrBarline elem = this;
      
        if (fMsrOptions->fDebugDebug)
          cerr << idtr <<
            "==> Launching msrBarline::visitEnd()" << endl;
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

void msrBarline::print (ostream& os)
{
  os <<
    "Barline" <<
    ", input line: " << fInputLineNumber << ", ";

  if (fBarlineHasSegno)
    os << "has segno, ";
  if (fBarlineHasCoda)
      os << "has coda, ";

  switch (fBarlineCategory) {
    case kStandaloneBar:
      os << "standalone";
      break;
    case kRepeatStart:
      os << "repeat start";
      break;
    case kRepeatEnd:
      os << "repeat end";
      break;
    case kEndingStart:
      os << "ending start";
      break;
    case kHookedEndingEnd:
      os << "hooked ending end";
      break;
    case kHooklessEndingEnd:
      os << "hookless ending end";
      break;
  } // switch
  os << endl;

  idtr++;

  os <<
    idtr << left << setw(15) << "Location" << " : ";
  switch (fLocation) {
    case k_NoLocation:
      os << "none";
      break;
    case kLeft:
      os << "Left";
      break;
    case kMiddle:
      os << "Middle";
      break;
    case kRight:
      os << "Right";
      break;
  } // switch
  os << endl;
  
  os <<
    idtr << setw(15) << "Style" << " : ";
  switch (fStyle) {
    case k_NoStyle:
      os << "none";
      break;
    case kRegular:
      os << "Regular";
      break;
    case kDotted:
      os << "Dotted";
      break;
    case kDashed:
      os << "Dashed";
      break;
    case kHeavy:
      os << "Heavy";
      break;
    case kLightLight:
      os << "LightLight";
      break;
    case kLightHeavy:
      os << "LightHeavy";
      break;
    case kHeavyLight:
      os << "HeavyLight";
      break;
    case kHeavyHeavy:
      os << "HeavyHeavy";
      break;
    case kTick:
      os << "Tick";
      break;
    case kShort:
      os << "Short";
      break;
  } // switch
  os << endl;
  
  os <<
    idtr << setw(15) << "EndingType" << " : ";
  switch (fEndingType) {
    case k_NoEndingType:
      os << "none";
      break;
    case kStart:
      os << "Start";
      break;
    case kStop:
      os << "Stop";
      break;
    case kDiscontinue:
      os << "Discontinue";
      break;
  } // switch
  os << endl;
  
  os <<
    idtr << setw(15) << "Ending number" << " : ";
  list<int>::const_iterator i;
  for (i=fEndingNumbersList.begin(); i!=fEndingNumbersList.end(); i++) {
    os << (*i) << " ";
  } // for
  os << endl;
 
  os <<
    idtr << setw(15) << "RepeatDirection" << " : ";
  switch (fRepeatDirection) {
    case k_NoRepeatDirection:
      os << "none";
      break;
    case kForward:
      os << "Forward";
      break;
    case kBackward:
      os << "Backward";
      break;
  } // switch
  os << endl;
  
  os <<
    idtr << setw(15) << "RepeatWinged" << " : ";
  switch (fRepeatWinged) {
    case k_NoRepeatWinged:
      os << "none";
      break;
    case kStraight:
      os << "Straight";
      break;
    case kCurved:
      os << "Curved";
      break;
    case kDoubleStraight:
      os << "DoubleStraight";
      break;
    case kDoubleCurved:
      os << "DoubleCurved";
      break;
  } // switch
  os << endl;
     
  idtr--;
}

//______________________________________________________________________________
S_msrVoicechunk msrVoicechunk::create (
  S_msrOptions&        msrOpts, 
  int                  inputLineNumber)
{
  msrVoicechunk* o =
    new msrVoicechunk (
      msrOpts, inputLineNumber);
  assert(o!=0);
  return o;
}

msrVoicechunk::msrVoicechunk (
  S_msrOptions&        msrOpts, 
  int                  inputLineNumber)
    : msrElement (msrOpts, inputLineNumber)
{}

msrVoicechunk::~msrVoicechunk() {}

S_msrVoicechunk msrVoicechunk::createEmptyClone ()
{
  S_msrVoicechunk
    clone =
      msrVoicechunk::create (
        fMsrOptions, fInputLineNumber);
  
  return clone;
}

/* JMI
void msrVoicechunk::removeElementFromVoicechunk (
  S_msrElement elem)
{
  for (
    list<S_msrElement>::iterator i = fVoicechunkElementsList.begin();
    i != fVoicechunkElementsList.end();
    i++) {
    if ((*i) == elem) {
      fVoicechunkElementsList.erase (i);
      break;
    }
  } // for
}
*/
void msrVoicechunk::acceptIn (basevisitor* v) {
  if (fMsrOptions->fDebugDebug)
    cerr << idtr <<
      "==> msrVoicechunk::acceptIn()" << endl;
      
  if (visitor<S_msrVoicechunk>*
    p =
      dynamic_cast<visitor<S_msrVoicechunk>*> (v)) {
        S_msrVoicechunk elem = this;
        
        if (fMsrOptions->fDebugDebug)
          cerr << idtr <<
            "==> Launching msrVoicechunk::visitStart()" << endl;
        p->visitStart (elem);
  }
}

void msrVoicechunk::acceptOut (basevisitor* v) {
  if (fMsrOptions->fDebugDebug)
    cerr << idtr <<
      "==> msrVoicechunk::acceptOut()" << endl;

  if (visitor<S_msrVoicechunk>*
    p =
      dynamic_cast<visitor<S_msrVoicechunk>*> (v)) {
        S_msrVoicechunk elem = this;
      
        if (fMsrOptions->fDebugDebug)
          cerr << idtr <<
            "==> Launching msrVoicechunk::visitEnd()" << endl;
        p->visitEnd (elem);
  }
}

void msrVoicechunk::browseData (basevisitor* v)
{
  if (fMsrOptions->fDebugDebug)
    cerr << idtr <<
      "==> msrVoicechunk::browseData()" << endl;

  for (
    list<S_msrElement>::iterator i = fVoicechunkElementsList.begin();
    i != fVoicechunkElementsList.end();
    i++) {
    // browse the element
    msrBrowser<msrElement> browser (v);
    browser.browse (*(*i));
  } // for

  if (fMsrOptions->fDebugDebug)
    cerr << idtr <<
      "<== msrVoicechunk::browseData()" << endl;
}

string msrVoicechunk::voicechunkAsString ()
{
  stringstream s;

  s << "Voicechunk" ;
  if (! fVoicechunkElementsList.size ())
    s << "(No actual notes)";
  else
    s << "(" << fVoicechunkElementsList.size () << " elements)";

  return s.str();
}

ostream& operator<< (ostream& os, const S_msrVoicechunk& elt)
{
  elt->print (os);
  return os;
}

void msrVoicechunk::print (ostream& os)
{  
  os << idtr <<
    "Voicechunk" <<
    " (" << fVoicechunkElementsList.size() << " elements)" <<
    endl;

  idtr++;
    
  os <<
    idtr << "Elements";

  idtr++;
  
  if (! fVoicechunkElementsList.size ())
    os << " none";
  else {
    os << endl;
    
    list<S_msrElement>::const_iterator
      iBegin = fVoicechunkElementsList.begin(),
      iEnd   = fVoicechunkElementsList.end(),
      i      = iBegin;
    for ( ; ; ) {
      os << idtr << (*i);
      if (++i == iEnd) break;
  // JMI    os << endl;
    } // for
  }
  os << endl;
    
  idtr--;

  idtr--;
}

//______________________________________________________________________________
S_msrRepeatending msrRepeatending::create (
  S_msrOptions&       msrOpts, 
  int                 inputLineNumber,
  string              repeatendingMusicXMLNumber, // may be "1, 2"
  msrRepeatendingKind repeatendingKind,
  S_msrVoicechunk     voicechunk,
  S_msrRepeat         repeatUplink)
{
  msrRepeatending* o =
    new msrRepeatending (
      msrOpts, inputLineNumber,
      repeatendingMusicXMLNumber,
      repeatendingKind,
      voicechunk,
      repeatUplink);
  assert(o!=0);
  return o;
}

msrRepeatending::msrRepeatending (
  S_msrOptions&       msrOpts, 
  int                 inputLineNumber,
  string              repeatendingMusicXMLNumber, // may be "1, 2"
  msrRepeatendingKind repeatendingKind,
  S_msrVoicechunk     voicechunk,
  S_msrRepeat         repeatUplink)
    : msrElement (msrOpts, inputLineNumber)
{
  fRepeatendingMusicXMLNumber = repeatendingMusicXMLNumber;
  
  fRepeatendingNumber = 0;
    // will be set by msrRepeat::addRepeatending ()
  
  fRepeatendingKind = repeatendingKind;
  
  fRepeatendingVoicechunk   = voicechunk;
  fRepeatendingRepeatUplink = repeatUplink;
}

msrRepeatending::~msrRepeatending() {}

S_msrRepeatending msrRepeatending::createEmptyClone (
  S_msrRepeat clonedRepeat)
{
//  if (fMsrOptions->fDebug)
    cerr << idtr <<
      "--> Creating an empty clone of a repeat ending" << endl;
  
  S_msrRepeatending
    clone =
      msrRepeatending::create (
        fMsrOptions,
        fInputLineNumber,
        fRepeatendingMusicXMLNumber,
        fRepeatendingKind,
        clonedRepeat->getRepeatCommonPart (),
        clonedRepeat);
  
  return clone;
}

void msrRepeatending::acceptIn (basevisitor* v) {
  if (fMsrOptions->fDebugDebug)
    cerr << idtr <<
      "==> msrRepeatending::acceptIn()" << endl;
      
  if (visitor<S_msrRepeatending>*
    p =
      dynamic_cast<visitor<S_msrRepeatending>*> (v)) {
        S_msrRepeatending elem = this;
        
        if (fMsrOptions->fDebugDebug)
          cerr << idtr <<
            "==> Launching msrRepeatending::visitStart()" << endl;
        p->visitStart (elem);
  }
}

void msrRepeatending::acceptOut (basevisitor* v) {
  if (fMsrOptions->fDebugDebug)
    cerr << idtr <<
      "==> msrRepeatending::acceptOut()" << endl;

  if (visitor<S_msrRepeatending>*
    p =
      dynamic_cast<visitor<S_msrRepeatending>*> (v)) {
        S_msrRepeatending elem = this;
      
        if (fMsrOptions->fDebugDebug)
          cerr << idtr <<
            "==> Launching msrRepeatending::visitEnd()" << endl;
        p->visitEnd (elem);
  }
}

void msrRepeatending::browseData (basevisitor* v)
{
  // browse the voice chunk
  msrBrowser<msrVoicechunk> browser (v);
  browser.browse (*fRepeatendingVoicechunk);
}

ostream& operator<< (ostream& os, const S_msrRepeatending& rept)
{
  rept->print (os);
  return os;
}

void msrRepeatending::print (ostream& os)
{
  os <<
    "Repeatending" " number " << fRepeatendingNumber << ", ";

  switch (fRepeatendingKind) {
    case kHookedEnding:
      os << "hooked";
      break;
    case kHooklessEnding:
      os << "hookless";
      break;
  } // switch
  os << endl;
  
  idtr++;

  os << fRepeatendingVoicechunk;

  idtr--;
}

//______________________________________________________________________________
S_msrRepeat msrRepeat::create (
  S_msrOptions&   msrOpts, 
  int             inputLineNumber,
  S_msrVoicechunk commonPart,
  S_msrVoice      voiceUplink)
{
  msrRepeat* o =
    new msrRepeat (
      msrOpts, inputLineNumber, commonPart, voiceUplink);
  assert(o!=0);
  return o;
}

msrRepeat::msrRepeat (
  S_msrOptions&   msrOpts, 
  int             inputLineNumber,
  S_msrVoicechunk commonPart,
  S_msrVoice      voiceUplink)
    : msrElement (msrOpts, inputLineNumber)
{
  fRepeatCommonPart     = commonPart;
  fRepeatEndingsCounter = 0;
  fRepeatVoiceUplink    = voiceUplink;
}

msrRepeat::~msrRepeat() {}

S_msrRepeat msrRepeat::createEmptyClone (S_msrVoice clonedVoice)
{
  S_msrVoicechunk
    voicechunk =
      msrVoicechunk::create (
        fMsrOptions, fInputLineNumber);

//  if (fMsrOptions->fDebug)
    cerr << idtr <<
      "--> Creating an empty clone of a repeat" << endl;
  
  S_msrRepeat
    clone =
      msrRepeat::create (
        fMsrOptions,
        fInputLineNumber,
        voicechunk,
        clonedVoice);
  
  return clone;
}

void msrRepeat::setRepeatCommonPart (
  S_msrVoicechunk repeatCommonPart)
{
  if (fMsrOptions->fTrace)
    cerr << idtr <<
      "Setting repeat common part containing " <<
      repeatCommonPart->getVoicechunkElements ().size () <<
      " elements" <<
      endl;
      
  fRepeatCommonPart = repeatCommonPart;
}

void msrRepeat::addRepeatending (S_msrRepeatending repeatending)
{
  if (fMsrOptions->fTrace)
    cerr << idtr <<
      "Adding an ending to repeat" << endl;
      
  fRepeatEndings.push_back (repeatending);

  repeatending->setRepeatendingNumber (
    ++ fRepeatEndingsCounter);
}

void msrRepeat::acceptIn (basevisitor* v) {
  if (fMsrOptions->fDebugDebug)
    cerr << idtr <<
      "==> msrRepeat::acceptIn()" << endl;
      
  if (visitor<S_msrRepeat>*
    p =
      dynamic_cast<visitor<S_msrRepeat>*> (v)) {
        S_msrRepeat elem = this;
        
        if (fMsrOptions->fDebugDebug)
          cerr << idtr <<
            "==> Launching msrRepeat::visitStart()" << endl;
        p->visitStart (elem);
  }
}

void msrRepeat::acceptOut (basevisitor* v) {
  if (fMsrOptions->fDebugDebug)
    cerr << idtr <<
      "==> msrRepeat::acceptOut()" << endl;

  if (visitor<S_msrRepeat>*
    p =
      dynamic_cast<visitor<S_msrRepeat>*> (v)) {
        S_msrRepeat elem = this;
      
        if (fMsrOptions->fDebugDebug)
          cerr << idtr <<
            "==> Launching msrRepeat::visitEnd()" << endl;
        p->visitEnd (elem);
  }
}

void msrRepeat::browseData (basevisitor* v)
{
  // browse the common part
  msrBrowser<msrVoicechunk> browser (v);
  browser.browse (*fRepeatCommonPart);
  
  // browse the alternatives
  for (
    vector<S_msrRepeatending>::iterator i = fRepeatEndings.begin();
    i != fRepeatEndings.end();
    i++) {
    // browse the alternative
    msrBrowser<msrRepeatending> browser (v);
    browser.browse (*(*i));
  } // for
}

ostream& operator<< (ostream& os, const S_msrRepeat& rept)
{
  rept->print (os);
  return os;
}

void msrRepeat::print (ostream& os)
{
  os <<
    endl <<
    idtr << "Repeat" <<
    ", input line: " << fInputLineNumber <<
    " (" << fRepeatEndings.size() << " repeat endings)" <<
    endl;
  
  idtr++;
  
  os << fRepeatCommonPart;
  
  vector<S_msrRepeatending>::const_iterator i;
  for (i=fRepeatEndings.begin(); i!=fRepeatEndings.end(); i++) {
    os << idtr << (*i);
  } // for
    
  idtr--;
}

//______________________________________________________________________________
S_msrUpbeat msrUpbeat::create (
  S_msrOptions&   msrOpts, 
  int             inputLineNumber,
  int             divisions,
  S_msrVoice      voiceUplink)
{
  msrUpbeat* o =
    new msrUpbeat (
      msrOpts, inputLineNumber, divisions, voiceUplink);
  assert(o!=0);
  return o;
}

msrUpbeat::msrUpbeat (
  S_msrOptions&   msrOpts, 
  int             inputLineNumber,
  int             divisions,
  S_msrVoice      voiceUplink)
    : msrElement (msrOpts, inputLineNumber)
{
  fUpbeatDivisions   = divisions;
  fUpbeatVoiceUplink = voiceUplink;
}

msrUpbeat::~msrUpbeat() {}

S_msrUpbeat msrUpbeat::createEmptyClone (S_msrVoice clonedVoice)
{
//  if (fMsrOptions->fDebug)
    cerr << idtr <<
      "--> Creating an empty clone of a Upbeat" << endl;
  
  S_msrUpbeat
    clone =
      msrUpbeat::create (
        fMsrOptions,
        fInputLineNumber,
        fUpbeatDivisions,
        clonedVoice);
  
  return clone;
}

void msrUpbeat::acceptIn (basevisitor* v) {
  if (fMsrOptions->fDebugDebug)
    cerr << idtr <<
      "==> msrUpbeat::acceptIn()" << endl;
      
  if (visitor<S_msrUpbeat>*
    p =
      dynamic_cast<visitor<S_msrUpbeat>*> (v)) {
        S_msrUpbeat elem = this;
        
        if (fMsrOptions->fDebugDebug)
          cerr << idtr <<
            "==> Launching msrUpbeat::visitStart()" << endl;
        p->visitStart (elem);
  }
}

void msrUpbeat::acceptOut (basevisitor* v) {
  if (fMsrOptions->fDebugDebug)
    cerr << idtr <<
      "==> msrUpbeat::acceptOut()" << endl;

  if (visitor<S_msrUpbeat>*
    p =
      dynamic_cast<visitor<S_msrUpbeat>*> (v)) {
        S_msrUpbeat elem = this;
      
        if (fMsrOptions->fDebugDebug)
          cerr << idtr <<
            "==> Launching msrUpbeat::visitEnd()" << endl;
        p->visitEnd (elem);
  }
}

void msrUpbeat::browseData (basevisitor* v)
{}

ostream& operator<< (ostream& os, const S_msrUpbeat& rept)
{
  rept->print (os);
  return os;
}

string msrUpbeat::getUpbeatDivisionsAsString () const
{
  string result;
  int    computedNumberOfDots; // value not used
  string errorMessage;

  int divisionsPerWholeNote =
    fUpbeatVoiceUplink->
      getDivisionsPerWholeNote ();
  
  if (fMsrOptions->fDebug)
    cerr <<
      endl <<
      idtr <<
        "% --> fUpbeatDivisions = " << fUpbeatDivisions <<
        ", divisionsPerWholeNote = " << divisionsPerWholeNote <<
      endl;

  result =
    divisionsAsMSRDuration (
      fUpbeatDivisions,
      divisionsPerWholeNote,
      fUpbeatDivisions, // JMI
      computedNumberOfDots,
      errorMessage,
      false); // 'true' to debug it;

  if (errorMessage.size ())
    msrMusicXMLError (
      fMsrOptions->fInputSourceName,
      fInputLineNumber,
      errorMessage);

  return result;
}

void msrUpbeat::print (ostream& os)
{
  os <<
    endl <<
    idtr << "Upbeat" <<
      ", input line: " << fInputLineNumber <<
      ", voice " << fUpbeatVoiceUplink->getVoiceName () << ", " <<
      fUpbeatDivisions << " divisions" <<
      " (" << getUpbeatDivisionsAsString () << ")" <<
    endl;
}

//______________________________________________________________________________ 
S_msrVoice msrVoice::create (
  S_msrOptions& msrOpts, 
  int           inputLineNumber,
  int           voiceNumber,
  int           staffRelativeVoiceNumber,
  S_msrStaff    voiceStaffUplink)
{
  msrVoice* o =
    new msrVoice (
      msrOpts, inputLineNumber,
      voiceNumber,
      staffRelativeVoiceNumber,
      voiceStaffUplink);
  assert(o!=0);
  return o;
}

S_msrVoice msrVoice::createEmptyClone (S_msrStaff clonedStaff)
{
  S_msrVoice
    clone =
      msrVoice::create (
        fMsrOptions,
        fInputLineNumber,
        fVoiceNumber,
        fStaffRelativeVoiceNumber,
        clonedStaff);

  // populate the voice measure location
  clone->fDivisionsPerWholeNote =
    fDivisionsPerWholeNote;
  clone->fVoiceMeasureLocation.fMeasureNumber =
    fVoiceMeasureLocation.fMeasureNumber;
  clone->fVoiceMeasureLocation.fPositionInMeasure =
    fVoiceMeasureLocation.fPositionInMeasure;

  // create the voice chunk
  if (fMsrOptions->fTrace)
    cerr << idtr <<
      "Creating the initial voice chunk for voice " <<
      clone->getVoiceName () << endl;
      
  clone->fVoicechunk =
    msrVoicechunk::create (
      clone->fMsrOptions, clone->fInputLineNumber);
  
  return clone;
}

msrVoice::msrVoice (
  S_msrOptions& msrOpts, 
  int           inputLineNumber,
  int           voiceNumber,
  int           staffRelativeVoiceNumber,
  S_msrStaff    voiceStaffUplink)
    : msrElement (msrOpts, inputLineNumber)
{
  fVoiceNumber = voiceNumber;
  fStaffRelativeVoiceNumber = staffRelativeVoiceNumber;
  fVoiceStaffUplink = voiceStaffUplink;

  if (fMsrOptions->fTrace)
    cerr << idtr <<
      "Creating voice \"" << getVoiceName () <<
      "\" in staff \"" << fVoiceStaffUplink->getStaffName () << "\"" <<
      endl;

  // the voice number should be in the 1..4 range  
  if (voiceNumber < 1 || voiceNumber > 4) {
    stringstream s;

    s <<
      "voice number " << voiceNumber <<
      " is not in the 1..4 range";
      
    msrAssert (false, s.str());
  }
  
  fDivisionsPerWholeNote =
    fVoiceStaffUplink->
      getDivisionsPerWholeNote ();
    
  // there may be an anacrusis
  fVoiceMeasureLocation.fMeasureNumber = 0;

  fVoiceMeasureLocation.fPositionInMeasure = 1;

  fMeasureZeroHasBeenMetInVoice   = false;
  fMeasureNumberHasBeenSetInVoice = false;
  fMusicHasBeenInsertedInVoice    = false;
  
  fVoiceContainsActualNotes = false;

  // get measure location from staff uplink
  fVoiceMeasureLocation =
    fVoiceStaffUplink->
      getStaffMeasureLocation ();
  
  // create the voice chunk
  if (fMsrOptions->fTrace)
    cerr << idtr <<
      "Creating the initial voice chunk for voice " <<
      getVoiceName () << endl;
      
  fVoicechunk =
    msrVoicechunk::create (
      fMsrOptions, inputLineNumber);

  // get the initial clef from the staff if any
  {
  S_msrClef
    clef =
      fVoiceStaffUplink->getStaffClef ();
      /*
  if (! clef)
    // it doesn't exist yet, create default G clef
    clef =
      msrClef::create (
        msrOpts,
        inputLineNumber,
        "G", 2, 0);
        */
  if (clef) {
    // append it to the voice chunk
    S_msrElement c = clef;
    fVoicechunk->
      appendElementToVoicechunk (c);
    }
  }
    
  // get the initial key from the staff if any
  {
  S_msrKey
    key =
      fVoiceStaffUplink->getStaffKey ();
  /*    JMI
  if (! key)
    // it doesn't exist yet, create default C major key
    key =
      msrKey::create (
        msrOpts,
        inputLineNumber,
        0, "major", 0);
        */
    if (key) {
    // append it to the voice chunk
    S_msrElement k = key;
    fVoicechunk->
      appendElementToVoicechunk (k);
    }
  }
  
  // get the initial time from the staff if any
  {
    S_msrTime
      time =
        fVoiceStaffUplink->getStaffTime ();
        /*
    if (! time)
      // it doesn't exist yet, create default 4/4 time
      time =
        msrTime::create (
          msrOpts,
          inputLineNumber,
          4, 4);
  */
    if (time) {
      // append it to the voice chunk
      S_msrElement t = time;
      fVoicechunk->
        appendElementToVoicechunk (t);
    }
  }
  
  // get the initial transpose from the staff if any
  {
    S_msrTranspose
      transpose =
        fVoiceStaffUplink->getStaffTranspose ();
        
    if (transpose) {
      // append it to the voice chunk
      S_msrElement t = transpose;
      fVoicechunk->
        appendElementToVoicechunk (t);
    }
  }
  
  // add the master lyrics to this voice, to
  // collect skips along the way that are used as a 'prelude'
  // by actual lyrics that start at later points
  fVoiceMasterLyrics =
    msrLyrics::create (
      fMsrOptions,
      inputLineNumber,
      -1,    // this lyrics number is unused anyway
      msrLyrics::kMasterLyrics,
      this);
}

msrVoice::~msrVoice() {}

string msrVoice::getVoiceName () const
{
  // not stored in a field,
  // because the voice staff may change name
  // when the part it belongs to is re-used

  int voiceNumber =
    fMsrOptions-> fCreateStaffRelativeVoiceNumbers
      ? fStaffRelativeVoiceNumber
      : fVoiceNumber;
    
  return
    fVoiceStaffUplink->getStaffName() +
    "_V_" +
    int2EnglishWord (voiceNumber);
}

void msrVoice::handleForward (int duration)
{} // JMI ???

void msrVoice::setMeasureNumber (
  int inputLineNumber, int measureNumber)
{
  enum voiceAnacrusisKind {
      k_NoAnacrusis, kExplicitAnacrusis, kImplicitAnacrusis };

  voiceAnacrusisKind anacrusisKind = k_NoAnacrusis;
/*
  if (fMsrOptions->fDebug)
    cerr <<
      "--> setMeasureNumber, " << endl <<
      "    measureNumber = " << measureNumber << endl <<
      "    fVoiceMeasureLocation.fMeasureNumber = " <<
           fVoiceMeasureLocation.fMeasureNumber << endl <<
      "    fMeasureNumberHasBeenSetInVoice = " <<
      fMeasureNumberHasBeenSetSetInVoice << endl <<
      "    fMusicHasBeenInserted = " << fMusicHasBeenInserted << endl <<
      endl;
*/
  int
    beatsNumber =
      fVoiceStaffUplink->
        getStaffTime ()->
          getBeatsNumber (),
          
    beatsValue =
      fVoiceStaffUplink->
        getStaffTime ()->
          getBeatsValue (),

    divisionsPerWholeNote =
      fDivisionsPerWholeNote,
      
    divisionsPerMeasure =
      divisionsPerWholeNote * beatsNumber / beatsValue,
          
    positionInMeasure =
      getVoicePositionInMeasure ();

//    if (gMsrOptions->fForceDebug || fMsrOptions->fDebug)
    cerr <<
      endl <<
      idtr << left <<
      "=== measure " << measureNumber <<
      ", voice = " << fVoiceNumber <<
      ", voice name = " << getVoiceName () <<
      ", positionInMeasure = " << positionInMeasure <<
      endl <<
      idtr <<
        setw(36) << "fVoiceMeasureLocation.fMeasureNumber" << " = " <<
        fVoiceMeasureLocation.fMeasureNumber <<
        endl <<
      idtr <<
        setw(36) << "inputLineNumber" << " = " <<
        inputLineNumber <<
        endl <<
      idtr <<
        setw(36) << "fMeasureZeroHasBeenMetInVoice" << " = " <<
        fMeasureZeroHasBeenMetInVoice <<
        endl <<
      idtr <<
        setw(36) << "beatsNumber" << " = " <<
        beatsNumber <<
        endl <<
      idtr <<
        setw(36) << "beatsValue" << " = " <<
        beatsValue <<
        endl <<
      idtr <<
        setw(36) << "divisionsPerWholeNote" << " = " <<
        divisionsPerWholeNote <<
        endl <<
      idtr <<
        setw(36) << "divisionsPerMeasure" << " = " <<
        divisionsPerMeasure <<
        endl <<
      endl;

  if (measureNumber == 0) {  
    fMeasureZeroHasBeenMetInVoice = true;
  }
  
  else if (measureNumber == 1) {
    if (
      positionInMeasure > 1 // there may be initial measures without music
        &&
      positionInMeasure <= divisionsPerMeasure) { // positions start at 1
        
      if (fMeasureZeroHasBeenMetInVoice) {
        int upbeatDivisions = positionInMeasure;
  
  //    if (gMsrOptions->fForceDebug || fMsrOptions->fDebug)
        cerr <<
          "====== upbeat found, upbeatDivisions = " <<
          upbeatDivisions << "======" <<
          endl;
      }
    
      else {
      }
  
    }

    else {
    }
  }
  


  if (
    positionInMeasure > 1 // there may be initial measures without music...
      &&
    positionInMeasure <= divisionsPerMeasure) {
 //   anacrusisKind = kExplicitAnacrusis;
  }
    
/*
  if (measureNumber == 2)
    cerr <<
      "====== measureNumber == 2, positionInMeasure = " <<
      getVoicePositionInMeasure () <<
      endl;

  if (
    measureNumber == 0
      &&
    ! fMeasureNumberHasBeenSet) {
    anacrusisKind = kExplicitAnacrusis;
  }
  / *
  else if (
//    getVoicePositionInMeasure () == 13 JMI
 //     &&
    measureNumber == 2
      &&
    ) {
    anacrusisKind = kImplicitAnacrusis;
  }
*/

  int    anacrusisDivisions;
  string anacrusisDivisionsAsString;
  
  if (anacrusisKind != k_NoAnacrusis) {
    int    computedNumberOfDots; // value not used
    string errorMessage;
    
    anacrusisDivisions = getVoicePositionInMeasure () - 1 ;
    anacrusisDivisionsAsString =
      divisionsAsMSRDuration (
        anacrusisDivisions,
        fDivisionsPerWholeNote,
        anacrusisDivisions, // JMI
        computedNumberOfDots,
        errorMessage,
        false); // 'true' to debug it
    
      if (errorMessage.size ())
        msrMusicXMLError (
          fMsrOptions->fInputSourceName,
          fInputLineNumber,
          errorMessage);

      if (fMsrOptions->fTrace) {
        cerr << idtr <<
          "Voice  " << getVoiceName () << " has an ";

/*
      if (anacrusisKind == kExplicitAnacrusis)
        cerr << "explicit";
      else
        cerr << "implicit";
*/

      cerr <<
        " anacrusis of " <<
        anacrusisDivisions <<
        " divisions, " <<
        "(" << anacrusisDivisionsAsString << ")" <<
        endl;
    }

    // create the anacrusis
    fVoiceAnacrusis =
      msrUpbeat::create (
        fMsrOptions,
        fInputLineNumber,
        anacrusisDivisions,
        this);
  }

  fVoiceMeasureLocation.fMeasureNumber =
    measureNumber;

  fMeasureNumberHasBeenSetInVoice = true;
}

void msrVoice::setNewVoicechunkForVoice (
  int inputLineNumber)
{
  // create the voice chunk
  if (fMsrOptions->fTrace)
    cerr << idtr <<
      "Creating a new voice chunk for voice " <<
      getVoiceName () << endl;
      
  fVoicechunk =
    msrVoicechunk::create (
      fMsrOptions, inputLineNumber);
}

S_msrLyrics msrVoice::addLyricsToVoice (
  int inputLineNumber,
  int lyricsNumber)
{
  if (fVoiceLyricsMap.count (lyricsNumber)) {
    cerr << idtr <<
      "### Internal error: lyrics " << lyricsNumber <<
      " already exists in this voice" << endl;

    return fVoiceLyricsMap [lyricsNumber];
  }

  // create the lyrics
  S_msrLyrics
    lyrics =
      msrLyrics::create (
        fMsrOptions,
        inputLineNumber,
        lyricsNumber,
        msrLyrics::kRegularLyrics,
        this);

  // register it in this voice
  if (fMsrOptions->fTrace)
    cerr << idtr <<
      "Adding lyrics " << lyrics->getLyricsName () <<
      " (" << lyricsNumber <<
      ") to voice " << getVoiceName () << endl;

  fVoiceLyricsMap [lyricsNumber] = lyrics;

  // catch up with fVoiceMasterLyrics
  // in case the lyrics does not start upon the first voice note
  vector<S_msrLyricschunk>
    masterChunks =
      fVoiceMasterLyrics->getLyricschunks ();

  if (masterChunks.size()) {
    if (fMsrOptions->fTrace)
      cerr << idtr <<
        "Copying current contents of voice master lyrics to new lyrics" << endl;
    for (
      vector<S_msrLyricschunk>::const_iterator i =
        masterChunks.begin();
      i != masterChunks.end();
      i++) {
      // add chunk to lyrics
      lyrics->addChunkToLyrics ((*i));
    } // for
  }

  // return it
  return lyrics;
}

void msrVoice::addLyricsToVoice (S_msrLyrics lyrics)
{
  // register it in this voice
  if (fMsrOptions->fTrace)
    cerr << idtr <<
      "Adding lyrics " << lyrics->getLyricsName () <<
      " (" << lyrics->getLyricsNumber () <<
      ") to voice " << getVoiceName () << endl;

  fVoiceLyricsMap [lyrics->getLyricsNumber ()] = lyrics;
}

S_msrLyrics msrVoice::fetchLyricsFromVoice (
  int lyricsNumber)
{
  S_msrLyrics result;
  
  if (fVoiceLyricsMap.count (lyricsNumber)) {
    result = fVoiceLyricsMap [lyricsNumber];
  }

  return result;
}

void msrVoice::appendClefToVoice (S_msrClef clef)
{
  if (fMsrOptions->fTrace)
    cerr << idtr <<
      "Appending clef '" << clef->clefAsString () <<
      "' to voice " << getVoiceName () << endl;

  S_msrElement c = clef;
  fVoicechunk->
    appendElementToVoicechunk (c);
}

void msrVoice::appendKeyToVoice (S_msrKey key)
{
  if (fMsrOptions->fTrace)
    cerr << idtr <<
      "Appending key '" << key->keyAsString () <<
      "' to voice " << getVoiceName () << endl;

  S_msrElement k = key;
  fVoicechunk->
    appendElementToVoicechunk (k);
}

void msrVoice::appendTimeToVoice (S_msrTime time)
{
  if (fMsrOptions->fTrace)
    cerr << idtr <<
      "Appending time '" << time->timeAsString () <<
      "' to voice " << getVoiceName () << endl;

  S_msrElement t = time;
  fVoicechunk->
    appendElementToVoicechunk (t);
}

void msrVoice::appendTransposeToVoice (S_msrTranspose transpose)
{
  if (fMsrOptions->fTrace)
    cerr << idtr <<
      "Appending transpose '" << transpose->transposeAsString () <<
      "' to voice " << getVoiceName () << endl;

  S_msrElement t = transpose;
  fVoicechunk->
    appendElementToVoicechunk (t);
}

/* JMI
void msrVoice::appendWordsToVoice (S_msrWords words)
{
  if (fMsrOptions->fTrace)
    cerr << idtr <<
      "Appending words '" << words->wordsAsString () <<
      "' to voice " << getVoiceName () << endl;

  S_msrElement w = words;
  fVoicechunk->
    appendElementToVoicechunk (w);
}
*/

void msrVoice::appendTempoToVoice (S_msrTempo tempo)
{
  if (fMsrOptions->fTrace)
    cerr << idtr <<
      "Appending tempo '" << tempo->tempoAsString () <<
      "' to voice " << getVoiceName () << endl;

  S_msrElement t = tempo;
  fVoicechunk->
    appendElementToVoicechunk (t);
}

void msrVoice::appendOctaveShiftToVoice (S_msrOctaveShift octaveShift)
{
  if (fMsrOptions->fTrace)
    cerr << idtr <<
      "Appending octave shift '" <<
      octaveShift->octaveShiftKindAsString () <<
      "', size: " << octaveShift->getOctaveShiftSize () <<
      " to voice " << getVoiceName () << endl;

  S_msrElement o = octaveShift;
  fVoicechunk->
    appendElementToVoicechunk (o);
}

void msrVoice::appendRehearsalToVoice (S_msrRehearsal rehearsal)
{
  if (fMsrOptions->fTrace)
    cerr << idtr <<
      "Appending rehearsal '" << rehearsal->getRehearsalText () <<
      "' to voice " << getVoiceName () << endl;

  S_msrElement r = rehearsal;
  fVoicechunk->
    appendElementToVoicechunk (r);
}

void msrVoice::appendNoteToVoice (S_msrNote note) {
  if (fMsrOptions->fDebugDebug)
    cerr << idtr <<
      "Appending note '" << note <<
      "' to voice " << getVoiceName () << endl;

  // append the note to the voice chunk
  S_msrElement n = note;
  fVoicechunk->
    appendElementToVoicechunk (n);

  if (note->getNoteKind () != msrNote::kRestNote)
    fVoiceContainsActualNotes = true;
    
  // add a skip chunk of the same duration to the master lyrics
  int
    lyricsDivisions =
      note->getNoteMusicXMLDivisions ();

  fVoiceMasterLyrics->
    addSkipChunkToLyrics (
      note->getInputLineNumber (),
      lyricsDivisions,
      note); // JMI

  fMusicHasBeenInsertedInVoice = true;
}

void msrVoice::appendChordToVoice (S_msrChord chord)
{
  if (fMsrOptions->fDebugDebug)
    cerr << idtr <<
      "Appending chord '" << chord <<
      "' to voice " << getVoiceName () << endl;

  S_msrElement c = chord;
  fVoicechunk->
    appendElementToVoicechunk (c);

  fMusicHasBeenInsertedInVoice = true;
}

void msrVoice::appendTupletToVoice (S_msrTuplet tuplet) {
  if (fMsrOptions->fDebugDebug)
    cerr << idtr <<
      "Appending tuplet '" << tuplet <<
      "' to voice " << getVoiceName () << endl;

  S_msrElement t = tuplet;
  fVoicechunk->
    appendElementToVoicechunk (t);

  fMusicHasBeenInsertedInVoice = true;
}

void msrVoice::appendGraceexpressionToVoice (
  S_msrGraceexpression graceexpression)
{
  if (fMsrOptions->fForceDebug || fMsrOptions->fDebugDebug)
    cerr << idtr <<
      "Appending grace expression " << graceexpression <<
      " to voice " << getVoiceName () << endl;

  S_msrElement g = graceexpression;
  fVoicechunk->
    appendElementToVoicechunk (g);

  fMusicHasBeenInsertedInVoice = true;
}

void msrVoice::addTextLyricschunkToVoice (
  int       lyricsNumber,
  string    syllabic,
  msrLyricschunk::msrLyricschunkKind
            lyricschunkKind,
  string    text,
  bool      elision,
  int       divisions,
  S_msrNote newNote)
{
  int inputLineNumber =
    newNote->getInputLineNumber ();
    
  // create a lyrics text chunk
  if (fMsrOptions->fForceDebug || fMsrOptions->fDebug) {
//    S_msrStaff staff = fLyricsVoiceUplink->getVoiceStaffUplink (); JMI
//    S_msrPart  part  = staff-> getStaffPartUplink ();
    
    cerr << idtr <<
      "--> Adding text lyrics chunk"
      ", line " << inputLineNumber <<
      ", divisions = " << divisions << 
      ", syllabic = \"" << syllabic << "\"" <<
      ", text = \"" << text << "\"";

/*
    string lyricschunkKindAsString =
      lyricschunkAsString ();
  */
    cerr <<
//      ", type = \"" << lyricschunkKindAsString << "\"" <<
      ", elision: " << elision <<
      " to lyrics " << lyricsNumber <<
      " in voice " << getVoiceName () << endl;
  }

  // is lyrics fCurrentLyricsNumber present in this voice?
  S_msrLyrics
    lyrics =
      fetchLyricsFromVoice (lyricsNumber);

  if (! lyrics)
    // no, add it to the voice
    lyrics =
      addLyricsToVoice (
        newNote->getInputLineNumber (), lyricsNumber);

  S_msrLyricschunk
    lyricshunk =
      msrLyricschunk::create (
        fMsrOptions,
        inputLineNumber,
        lyricschunkKind,
        text,
        divisions,
        newNote,
        lyrics);

  // add it to the lyrics
  lyrics->
    addChunkToLyrics (lyricshunk);

  // this lyrics does contain text
  lyrics->
    setLyricsTextPresent ();
}

void msrVoice::addSkipLyricschunkToVoice (
  int       lyricsNumber,
  int       divisions,
  S_msrNote newNote)
{
  int inputLineNumber =
    newNote->getInputLineNumber ();
    
  // create a lyrics text chunk
  if (fMsrOptions->fForceDebug || fMsrOptions->fDebug) {
//    S_msrStaff staff = fLyricsVoiceUplink->getVoiceStaffUplink (); JMI
//    S_msrPart  part  = staff-> getStaffPartUplink ();
    
    cerr << idtr <<
      "--> Adding skip lyrics chunk"
      ", line " << inputLineNumber <<
      ", divisions = " << divisions;

/*
    string lyricschunkKindAsString =
      lyricschunkAsString ();
  */
    cerr <<
//      ", type = \"" << lyricschunkKindAsString << "\"" <<
//      ", elision: " << elision <<
//      " in lyrics " << lyricsNumber <<
      " in voice " << getVoiceName () << endl;
  }

  // is lyrics fCurrentLyricsNumber present in this voice?
  S_msrLyrics
    lyrics =
      fetchLyricsFromVoice (lyricsNumber);

  if (! lyrics)
    // no, add it to the voice
    lyrics =
      addLyricsToVoice (
        newNote->getInputLineNumber (), lyricsNumber);
  
  // create lyrics slur chunk
  S_msrLyricschunk
    lyricshunk =
      msrLyricschunk::create (
        fMsrOptions,
        inputLineNumber,
        msrLyricschunk::kSlurBeyondEndChunk, "", divisions,
        newNote,
        lyrics);
        
  // add it to the lyrics
  lyrics->
    addChunkToLyrics (lyricshunk);
}

void msrVoice::addTiedLyricschunkToVoice (
  int       lyricsNumber,
  int       divisions,
  S_msrNote newNote)
{
  int inputLineNumber =
    newNote->getInputLineNumber ();
    
  // create a lyrics text chunk
  if (fMsrOptions->fForceDebug || fMsrOptions->fDebug) {
//    S_msrStaff staff = fLyricsVoiceUplink->getVoiceStaffUplink (); JMI
//    S_msrPart  part  = staff-> getStaffPartUplink ();
    
    cerr << idtr <<
      "--> Adding skip lyrics chunk"
      ", line " << inputLineNumber <<
      ", divisions = " << divisions <<
      endl;

/*
    string lyricschunkKindAsString =
      lyricschunkAsString ();
  */
    cerr <<
//      ", type = \"" << lyricschunkKindAsString << "\"" <<
//      ", elision: " << elision <<
//      " in lyrics " << lyricsNumber <<
      " in voice " << getVoiceName () << endl;
  }

  // is lyrics fCurrentLyricsNumber present in this voice?
  S_msrLyrics
    lyrics =
      fetchLyricsFromVoice (lyricsNumber);

  if (! lyrics)
    // no, add it to the voice
    lyrics =
      addLyricsToVoice (
        newNote->getInputLineNumber (), lyricsNumber);
  
  // create lyrics slur chunk
  S_msrLyricschunk
    lyricschunk =
      msrLyricschunk::create (
        fMsrOptions,
        inputLineNumber,
        msrLyricschunk::kSlurBeyondEndChunk, "", divisions,
        newNote,
        lyrics);
        
  // add it to the lyrics
  lyrics->
    addChunkToLyrics (lyricschunk);
}

void msrVoice::addSlurLyricschunkToVoice (
  int       lyricsNumber,
  int       divisions,
  S_msrNote newNote)
{
  int inputLineNumber =
    newNote->getInputLineNumber ();
    
  // create a lyrics text chunk
  if (fMsrOptions->fForceDebug || fMsrOptions->fDebug) {
//    S_msrStaff staff = fLyricsVoiceUplink->getVoiceStaffUplink (); JMI
//    S_msrPart  part  = staff-> getStaffPartUplink ();
    
    cerr << idtr <<
      "--> Adding 'Slur' lyrics chunk"
      ", line " << inputLineNumber <<
      ", divisions = " << divisions <<
/*
      endl;

    string lyricschunkKindAsString =
      lyricschunkAsString ();
    cerr <<
  */

//      ", type = \"" << lyricschunkKindAsString << "\"" <<
//      ", elision: " << elision <<
      " in lyrics " << lyricsNumber <<
      " in voice " << getVoiceName () << endl;
  }

  // is lyrics fCurrentLyricsNumber present in this voice?
  S_msrLyrics
    lyrics =
      fetchLyricsFromVoice (lyricsNumber);

  if (! lyrics)
    // no, add it to the voice
    lyrics =
      addLyricsToVoice (
        newNote->getInputLineNumber (), lyricsNumber);
  
  // create lyrics slur chunk
  S_msrLyricschunk
    lyricshunk =
      msrLyricschunk::create (
        fMsrOptions,
        inputLineNumber,
        msrLyricschunk::kSlurBeyondEndChunk, "", divisions,
        newNote,
        lyrics);
        
  // add it to the lyrics
  lyrics->
    addChunkToLyrics (lyricshunk);
}

void msrVoice::addSlurBeyondEndLyricschunkToVoice (
  int       lyricsNumber,
  int       divisions,
  S_msrNote newNote)
{
  int inputLineNumber =
    newNote->getInputLineNumber ();
    
  // create a lyrics text chunk
  if (fMsrOptions->fForceDebug || fMsrOptions->fDebug) {
//    S_msrStaff staff = fLyricsVoiceUplink->getVoiceStaffUplink (); // JMI
//    S_msrPart  part  = staff-> getStaffPartUplink ();
    
    cerr << idtr <<
      "--> Adding 'SlurBeyondEnd' lyrics chunk"
      ", line " << inputLineNumber <<
      ", divisions = " << divisions <<
      endl;

/*
    string lyricschunkKindAsString =
      lyricschunkAsString ();
  */
    cerr <<
//      ", type = \"" << lyricschunkKindAsString << "\"" <<
//      ", elision: " << elision <<
//      " in lyrics " << lyricsNumber <<
      " in voice " << getVoiceName () << endl;
  }

  // is lyrics fCurrentLyricsNumber present in this voice?
  S_msrLyrics
    lyrics =
      fetchLyricsFromVoice (lyricsNumber);

  if (! lyrics)
    // no, add it to the voice
    lyrics =
      addLyricsToVoice (
        newNote->getInputLineNumber (), lyricsNumber);
  
  // create lyrics slur chunk
  S_msrLyricschunk
    lyricshunk =
      msrLyricschunk::create (
        fMsrOptions,
        inputLineNumber,
        msrLyricschunk::kSlurBeyondEndChunk, "", divisions,
        newNote,
        lyrics);
        
  // add it to the lyrics
  lyrics->
    addChunkToLyrics (lyricshunk);
}

void msrVoice::appendBarCheckToVoice (S_msrBarCheck barCheck)
{
  if (fMsrOptions->fDebugDebug)
    cerr << idtr <<
      "Appending bar check '" << barCheck <<
      "' to voice " << getVoiceName () << endl;

  fVoicechunk->
    appendElementToVoicechunk (barCheck);

  // add bar check chunk to the voice master lyrics
  fVoiceMasterLyrics->
    addBarcheckChunkToLyrics (
      barCheck->getInputLineNumber (),
      fVoiceMeasureLocation.fMeasureNumber);
}

void msrVoice::appendBarnumberCheckToVoice (S_msrBarnumberCheck bnc)
{
  if (fMsrOptions->fDebugDebug)
    cerr << idtr <<
      "Appending barnumber check '" << bnc <<
      "' to voice " << getVoiceName () << endl;

  fVoicechunk->
    appendElementToVoicechunk (bnc);

/*
  // add barnumber check chunk to the voice master lyrics
  fVoiceMasterLyrics->
    addBarnumberCheckChunkToLyrics (
      bnc->getInputLineNumber (),
      fVoiceMeasureLocation.fMeasureNumber);
*/
}

void msrVoice::appendBreakToVoice (S_msrBreak break_)
{
  if (fMsrOptions->fDebugDebug)
    cerr << idtr <<
      "Appending break '" << break_ <<
      "' to voice " << getVoiceName () << endl;

  fVoicechunk->
    appendElementToVoicechunk (break_);

  // add break chunk to the voice master lyrics
  fVoiceMasterLyrics->
    addBreakChunkToLyrics (
      break_->getInputLineNumber (),
      fVoiceMeasureLocation.fMeasureNumber);
}

/*
void msrVoice::addSlurLyricschunkToVoice (
  int       lyricsNumber,
  int       divisions,
  S_msrNote newNote)
{
  int inputLineNumber =
    newNote->getInputLineNumber ();
    
  // create a lyrics text chunk
  if (fMsrOptions->fForceDebug || fMsrOptions->fDebug) {
//    S_msrStaff staff = fLyricsVoiceUplink->getVoiceStaffUplink ();
//    S_msrPart  part  = staff-> getStaffPartUplink ();
    
    cerr << idtr <<
      "--> Adding skip lyrics chunk"
      ", line " << inputLineNumber <<
      ", divisions = " << divisions <<
      endl;

/ *
    string lyricschunkKindAsString =
      lyricschunkAsString ();
  * /
    cerr <<
//      ", type = \"" << lyricschunkKindAsString << "\"" <<
//      ", elision: " << elision <<
//      " in lyrics " << lyricsNumber <<
      " in voice " << getVoiceName () << endl;
  }

  // is lyrics fCurrentLyricsNumber present in this voice?
  S_msrLyrics
    lyrics =
      fetchLyricsFromVoice (lyricsNumber);

  if (! lyrics)
    // no, add it to the voice
    lyrics =
      addLyricsToVoice (
        newNote->getInputLineNumber (), lyricsNumber);
  
  // create lyrics slur chunk
  S_msrLyricschunk
    lyricshunk =
      msrLyricschunk::create (
        fMsrOptions,
        inputLineNumber,
        msrLyricschunk::kSlurBeyondEndChunk, "", divisions,
        newNote,
        lyrics);
        
  // add it to the lyrics
  lyrics->
    addChunkToLyrics (lyricshunk);
}
*/

void msrVoice::appendRepeatToVoice (S_msrRepeat repeat) {
  if (fMsrOptions->fTrace)
    cerr << idtr <<
      "Appending repeat to voice " << getVoiceName () << endl;

  S_msrElement r = repeat;
  fVoicechunk->
    appendElementToVoicechunk (r);
}

void msrVoice::prependBarlineToVoice (S_msrBarline barline) {
  if (fMsrOptions->fTrace)
    cerr << idtr <<
      "Prepending a barline to voice " << getVoiceName () <<
      ":" << endl;
    idtr++;
    cerr << idtr << barline;
    idtr--;

  S_msrElement b = barline;
  fVoicechunk->
    prependElementToVoicechunk (b);
}

void msrVoice::appendBarlineToVoice (S_msrBarline barline) {
  if (fMsrOptions->fDebugDebug)
  if (fMsrOptions->fTrace)
    cerr << idtr <<
      "Appending a barline to voice " << getVoiceName () <<
      ":" << endl;
    idtr++;
    cerr << idtr << barline;
    idtr--;

  S_msrElement b = barline;
  fVoicechunk->
    appendElementToVoicechunk (b);
}

/* JMI
void msrVoice::appendElementToVoice (S_msrElement elem)
{
  if (fMsrOptions->fDebugDebug)
    cerr << idtr <<
      "Appending element '" << elem <<
      "' to voice " << getVoiceName () << endl;

  fVoicechunk->
    appendElementToVoicechunk (elem);
}
*/

void msrVoice::removeLastElementFromVoice ()
{
  if (fMsrOptions->fDebugDebug)
    cerr << idtr <<
      "Removing last element " <<
      " from voice " << getVoiceName () << endl;

  fVoicechunk->
    removeLastElementFromVoicechunk ();
}

void msrVoice::acceptIn (basevisitor* v) {
  if (fMsrOptions->fDebugDebug)
    cerr << idtr <<
      "==> msrVoice::acceptIn()" << endl;
      
  if (visitor<S_msrVoice>*
    p =
      dynamic_cast<visitor<S_msrVoice>*> (v)) {
        S_msrVoice elem = this;
        
        if (fMsrOptions->fDebugDebug)
          cerr << idtr <<
            "==> Launching msrVoice::visitStart()" << endl;
        p->visitStart (elem);
  }
}

void msrVoice::acceptOut (basevisitor* v) {
  if (fMsrOptions->fDebugDebug)
    cerr << idtr <<
      "==> msrVoice::acceptOut()" << endl;

  if (visitor<S_msrVoice>*
    p =
      dynamic_cast<visitor<S_msrVoice>*> (v)) {
        S_msrVoice elem = this;
      
        if (fMsrOptions->fDebugDebug)
          cerr << idtr <<
            "==> Launching msrVoice::visitEnd()" << endl;
        p->visitEnd (elem);
  }
}

void msrVoice::browseData (basevisitor* v)
{
  if (fMsrOptions->fDebugDebug)
    cerr << idtr <<
      "==> msrVoice::browseData()" << endl;

  if (fVoiceAnacrusis) {
  // browse the voice anacrusis
  msrBrowser<msrUpbeat> browser (v);
  browser.browse (*fVoiceAnacrusis);
  }
  
  // browse the voice chunk
  msrBrowser<msrVoicechunk> browser (v);
  browser.browse (*fVoicechunk);

  // browse the voice lyrics
  for (
    map<int, S_msrLyrics>::iterator i = fVoiceLyricsMap.begin();
    i != fVoiceLyricsMap.end();
    i++) {
    // browse the lyrics
    msrBrowser<msrLyrics> browser (v);
    browser.browse (*((*i).second));
  } // for

  if (fMsrOptions->fDebug)
    cerr << idtr <<
      "<== msrVoice::browseData()" << endl;
}

ostream& operator<< (ostream& os, const S_msrVoice& elt)
{
  elt->print (os);
  return os;
}

void msrVoice::print (ostream& os)
{
  os <<
    "Voice" << " " << getVoiceName () <<
    " (" << fVoiceLyricsMap.size() << " lyrics)" <<
    endl;

  idtr++;

  // print the anacrusis if any
  os << idtr;
  if (fVoiceAnacrusis)
    os << fVoiceAnacrusis;
  else
    os << "none" << endl;
  os << endl;

  // print the voice chunk
  os << fVoicechunk << endl;
  
  if (fMsrOptions->fForceDebug || fMsrOptions->fDebug) {
    // print the master lyrics
    os << idtr <<
      fVoiceMasterLyrics <<
      endl;    
  }
  
  if (! fMsrOptions->fDontDisplayMSRLyrics) {
    // print the lyrics
    if (fVoiceLyricsMap.size()) {
      map<int, S_msrLyrics>::const_iterator
        iBegin = fVoiceLyricsMap.begin(),
        iEnd   = fVoiceLyricsMap.end(),
        i      = iBegin;
        
      for ( ; ; ) {
        os << idtr << (*i).second;
        if (++i == iEnd) break;
        os << endl;
      } // for
    }
  }
  
  idtr--;
}

//______________________________________________________________________________
int msrStaff::gMaxStaffVoices = 4;

S_msrStaff msrStaff::create (
  S_msrOptions& msrOpts, 
  int           inputLineNumber,
  int           staffNumber,
  S_msrPart     staffPartUplink)
{
  msrStaff* o =
    new msrStaff (
      msrOpts, inputLineNumber, staffNumber, staffPartUplink);
  assert(o!=0);
  return o;
}

msrStaff::msrStaff (
  S_msrOptions& msrOpts, 
  int           inputLineNumber,
  int           staffNumber,
  S_msrPart     staffPartUplink)
    : msrElement (msrOpts, inputLineNumber)
{  
  fStaffNumber = staffNumber;
  fStaffPartUplink   = staffPartUplink;

  fNextRelativeStaffVoiceNumber = 0;

  if (fMsrOptions->fTrace)
    cerr << idtr <<
      "Creating staff \"" << getStaffName () <<
      "\" in part \"" << fStaffPartUplink->getPartCombinedName () << "\"" <<
      endl;

  // the staff number should be positive 
  if (staffNumber <= 0) {
    stringstream s;

    s <<
      "staff number " << staffNumber <<
      " is not positive";
      
    msrAssert (false, s.str());
  }

  fDivisionsPerWholeNote =
    fStaffPartUplink->
      getDivisionsPerWholeNote ();

  // get measure location from part uplink
  fStaffMeasureLocation =
    fStaffPartUplink->
      getPartMeasureLocation ();
  
  // get the initial clef from the staff if any
  {
    S_msrClef
      clef =
        fStaffPartUplink->getPartClef ();
  
    if (clef) {
      fStaffClef = clef;
      appendClefToAllStaffVoices (clef);
    }
    else {
      // create the implicit initial G line 2 clef
      fStaffClef =
        msrClef::create (
          msrOpts,
          inputLineNumber,
          "G", 2, 0);
    }
  }
    
  // get the initial key from the staff if any
  {
    S_msrKey
      key =
        fStaffPartUplink->getPartKey ();
  
    if (key) {
      fStaffKey = key;
      appendKeyToAllStaffVoices (key);
    }
    else {
      // create the implicit initial C major key
      fStaffKey =
        msrKey::create (
          msrOpts,
          inputLineNumber,
          0, "major", 0);
    }
  }
  
  // get the initial time from the staff if any
  {
    S_msrTime
      time =
        fStaffPartUplink->getPartTime ();

    if (time) {
      fStaffTime = time;
      appendTimeToAllStaffVoices (time);
    }
    else {
      // create the implicit initial 4/4 time signature
      fStaffTime =
        msrTime::create (
          msrOpts,
          inputLineNumber,
          4, 4);
    }
  }
  
  // get the initial transpose from the staff if any
  {
    S_msrTranspose
      transpose =
        fStaffPartUplink->getPartTranspose ();
        
    if (transpose) {
      fStaffTranspose = transpose;
      appendTransposeToAllStaffVoices (transpose);
    }
  }
}

msrStaff::~msrStaff() {}

S_msrStaff msrStaff::createEmptyClone (S_msrPart clonedPart)
{
  S_msrStaff
    clone =
      msrStaff::create (
        fMsrOptions,
        fInputLineNumber,
        fStaffNumber,
        clonedPart);
  
  return clone;
}

string msrStaff::getStaffName () const
  {
  // not stored in a field,
  // because the staff part may change name
  // when it is re-used
  return
    fStaffPartUplink->getPartMSRName () +
    "_S_" +
    int2EnglishWord (fStaffNumber);
  }

void msrStaff::setAllStaffVoicesDivisionsPerWholeNote (int divisions)
{
  for (
    map<int, S_msrVoice>::iterator i = fStaffVoicesMap.begin();
    i != fStaffVoicesMap.end();
    i++) {
    (*i).second->setDivisionsPerWholeNote (divisions);
  } // for
}

S_msrVoice msrStaff::addVoiceToStaff (
  int inputLineNumber,
  int voiceNumber)
{
  if (fStaffVoicesMap.count (voiceNumber)) {
    cerr << idtr <<
      "### Internal error: voice " << voiceNumber <<
      " already exists in this staff" << endl;

    return fStaffVoicesMap [voiceNumber];
  }

  // take this new voice into account
  fNextRelativeStaffVoiceNumber++;
  
  if (fNextRelativeStaffVoiceNumber > msrStaff::gMaxStaffVoices) {
    stringstream s;    
    s <<
      "staff " << getStaffName () <<
      " is already filled up with" << msrStaff::gMaxStaffVoices <<
      " voices, voice " << voiceNumber << " overflows it" << endl;
// JMI    msrMusicXMLError (s.str());
    msrMusicXMLWarning (
      fMsrOptions->fInputSourceName,
      999,
      s.str()); // JMI
  }

  // create the voice
  S_msrVoice
    voice =
      msrVoice::create (
        fMsrOptions,
        inputLineNumber,
        voiceNumber,
        fNextRelativeStaffVoiceNumber,
        this);

  // register it in this staff
  if (fMsrOptions->fTrace)
    cerr << idtr <<
      "Adding voice " << voiceNumber <<
      " " << voice->getVoiceName () <<
      " to staff " << fStaffNumber <<
      " in part " << fStaffPartUplink->getPartCombinedName () << endl;
  
  fStaffVoicesMap [voiceNumber] = voice;

  // return it
  return voice;
}

void msrStaff::addVoiceToStaff (S_msrVoice voice)
{
  // register voic in this staff
  if (fMsrOptions->fTrace)
    cerr << idtr <<
      "Adding voice " << voice->getVoiceNumber () <<
      " " << voice->getVoiceName () <<
      " to staff " << fStaffNumber <<
      " in part " << fStaffPartUplink->getPartCombinedName () << endl;
  
  fStaffVoicesMap [voice->getVoiceNumber ()] = voice;
}

S_msrVoice msrStaff::fetchVoiceFromStaff (
  int voiceNumber)
{
  S_msrVoice result;
  
  if (fStaffVoicesMap.count (voiceNumber)) {
    result = fStaffVoicesMap [voiceNumber];
  }
  /* else {
    stringstream s;
    s <<
      "staff " << getStaffName () <<
      " has no voice number " << voiceNumber << endl;
 //   msrMusicXMLError (s.str()); JMI
    msrMusicXMLWarning (s.str());
  }
  */

  return result;
}

void msrStaff::setStaffClef (S_msrClef clef)
{
  if (fMsrOptions->fTrace)
    cerr << idtr <<
      "Setting clef '" << clef->clefAsString () <<
      "' in staff " << fStaffNumber <<
      " in part \"" << fStaffPartUplink->getPartCombinedName () << "\"" <<
      endl;

  // set staff clef
  fStaffClef = clef;

  // propagate it to all voices
  appendClefToAllStaffVoices (clef);
}

void msrStaff::setStaffKey  (S_msrKey  key)
{
  if (fMsrOptions->fTrace)
    cerr << idtr <<
      "Setting key '" << key->keyAsString () <<
      "' in staff " << fStaffNumber <<
      " in part \"" << fStaffPartUplink->getPartCombinedName () << "\"" <<
      endl;

  // set staff key
  fStaffKey = key;

  // propagate it to all voices
  appendKeyToAllStaffVoices (key);
}

void msrStaff::setStaffTime (S_msrTime time)
{
  if (fMsrOptions->fTrace)
    cerr << idtr <<
      "Setting time '" << time->timeAsString () <<
      "' in staff " << fStaffNumber <<
      " in part \"" << fStaffPartUplink->getPartCombinedName () << "\"" <<
      endl;

  // set staff time
  fStaffTime = time;

  // propagate it to all voices
  appendTimeToAllStaffVoices (time);
}

void msrStaff::setStaffTranspose (S_msrTranspose transpose)
{
  if (fMsrOptions->fTrace)
    cerr << idtr <<
      "Setting transpose '" << transpose->transposeAsString () <<
      "' in staff " << fStaffNumber <<
      " in part \"" << fStaffPartUplink->getPartCombinedName () << "\"" <<
      endl;

  // set staff transpose
  fStaffTranspose = transpose;

  // propagate it to all voices
  appendTransposeToAllStaffVoices (transpose);
}

void msrStaff::appendClefToAllStaffVoices (S_msrClef clef)
{
  for (
    map<int, S_msrVoice>::iterator i = fStaffVoicesMap.begin();
    i != fStaffVoicesMap.end();
    i++) {
    (*i).second->appendClefToVoice (clef);
  } // for
}

void msrStaff::appendKeyToAllStaffVoices (S_msrKey  key)
{
  for (
    map<int, S_msrVoice>::iterator i = fStaffVoicesMap.begin();
    i != fStaffVoicesMap.end();
    i++) {
    (*i).second->appendKeyToVoice (key);
  } // for
}

void msrStaff::appendTimeToAllStaffVoices (S_msrTime time)
{
  for (
    map<int, S_msrVoice>::iterator i = fStaffVoicesMap.begin();
    i != fStaffVoicesMap.end();
    i++) {
    (*i).second->appendTimeToVoice (time);
  } // for
}

void msrStaff::appendTransposeToAllStaffVoices (S_msrTranspose transpose)
{
  for (
    map<int, S_msrVoice>::iterator i = fStaffVoicesMap.begin();
    i != fStaffVoicesMap.end();
    i++) {
    (*i).second->appendTransposeToVoice (transpose);
  } // for
}
 
void msrStaff::acceptIn (basevisitor* v) {
  if (fMsrOptions->fDebugDebug)
    cerr << idtr <<
      "==> msrStaff::acceptIn()" << endl;
      
  if (visitor<S_msrStaff>*
    p =
      dynamic_cast<visitor<S_msrStaff>*> (v)) {
        S_msrStaff elem = this;
        
        if (fMsrOptions->fDebugDebug)
          cerr << idtr <<
            "==> Launching msrStaff::visitStart()" << endl;
        p->visitStart (elem);
  }
}

void msrStaff::acceptOut (basevisitor* v) {
  if (fMsrOptions->fDebugDebug)
    cerr << idtr <<
      "==> msrStaff::acceptOut()" << endl;

  if (visitor<S_msrStaff>*
    p =
      dynamic_cast<visitor<S_msrStaff>*> (v)) {
        S_msrStaff elem = this;
      
        if (fMsrOptions->fDebugDebug)
          cerr << idtr <<
            "==> Launching msrStaff::visitEnd()" << endl;
        p->visitEnd (elem);
  }
}

void msrStaff::browseData (basevisitor* v)
{
  if (fMsrOptions->fDebugDebug)
    cerr << idtr <<
      "==> msrStaff::browseData()" << endl;

  /*
    fStaffClef, fStaffKey and fStaffTime are used to populate
    newly created voices, not to create music proper:
    they're thus not browsed
  */

  if (fStaffVoicesMap.size ()) {
    for (
      map<int, S_msrVoice>::iterator i = fStaffVoicesMap.begin();
      i != fStaffVoicesMap.end();
      i++) {
      // browse the voice
      msrBrowser<msrVoice> browser (v);
      browser.browse (*((*i).second));
    } // for
  }

  if (fMsrOptions->fDebug)
    cerr << idtr <<
      "<== msrStaff::browseData()" << endl;
}

ostream& operator<< (ostream& os, const S_msrStaff& elt)
{
  elt->print (os);
  return os;
}

void msrStaff::print (ostream& os)
{
  os <<
    "Staff" << " " << getStaffName () <<
    " (" << fStaffVoicesMap.size() << " voices)" <<
    endl;

  idtr++;

  if (fStaffClef)
    os << idtr << fStaffClef;
  else
    os << idtr << "NO_CLEF" << endl;

  if (fStaffKey)
    os << idtr << fStaffKey;
  else
    os << idtr << "NO_KEY" << endl;

  if (fStaffTime)
    os << idtr << fStaffTime;
  else
    os << idtr << "NO_TIME" << endl;

  os <<
    idtr << "StaffInstrumentName: \"" <<
    fStaffInstrumentName << "\"" << endl;

  os << endl;

  if (fStaffVoicesMap.size ()) {
    for (
      map<int, S_msrVoice>::iterator i = fStaffVoicesMap.begin();
      i != fStaffVoicesMap.end();
      i++) {
      os << idtr << (*i).second;
    } // for
  }

  idtr--;
}

//______________________________________________________________________________ 
S_msrPart msrPart::create (
  S_msrOptions&  msrOpts, 
  int            inputLineNumber,
  string         partMusicXMLID,
  S_msrPartgroup partPartgroupUplink)
{
  msrPart* o =
    new msrPart (
      msrOpts, inputLineNumber, partMusicXMLID, partPartgroupUplink);
  assert(o!=0);
  return o;
}

msrPart::msrPart (
  S_msrOptions&  msrOpts, 
  int            inputLineNumber,
  string         partMusicXMLID,
  S_msrPartgroup partPartgroupUplink)
    : msrElement (msrOpts, inputLineNumber)
{
  // replace spaces in part ID
  for_each (
    partMusicXMLID.begin(),
    partMusicXMLID.end(),
    stringSpaceReplacer (fPartMusicXMLID, '_'));

 // JMI fPartMusicXMLID      = partMusicXMLID;
 
  fPartPartgroupUplink = partPartgroupUplink;

  // is this part name in the part renaming map?
  map<string, string>::iterator
    it =
      msrOpts->fPartsRenaming.find (fPartMusicXMLID);
        
  if (it != msrOpts->fPartsRenaming.end ()) {
    // yes, rename the part accordinglingly
    fPartMSRName = (*it).second;
  }
  else {
    // coin the name from the argument
    fPartMSRName =
      "P_"+stringNumbersToEnglishWords (fPartMusicXMLID);
  }
    
  if (fMsrOptions->fTrace)
    cerr << idtr <<
      "Creating part " << getPartCombinedName () << endl;

  fDivisionsPerWholeNote = 0;
}

msrPart::~msrPart() {}

S_msrPart msrPart::createEmptyPartClone (S_msrPartgroup clonedPartgroup)
{
  S_msrPart
    clone =
      msrPart::create (
        fMsrOptions,
        fInputLineNumber,
        fPartMusicXMLID,
        clonedPartgroup);

  clone->fPartName           = fPartName;
  clone->fPartAbbreviation   = fPartAbbreviation;
  clone->fPartInstrumentName = fPartInstrumentName;
  
  return clone;
}

void msrPart::setPartMSRName (string partMSRName)
{
  // is this part name in the part renaming map?
  map<string, string>::iterator
    it =
      fMsrOptions->fPartsRenaming.find (fPartMSRName);
        
  if (it != fMsrOptions->fPartsRenaming.end ()) {
    // yes, rename the part accordinglingly
    fPartMSRName = (*it).second;

    if (fMsrOptions->fTrace)
      cerr << idtr <<
        "Setting part name of \"" << getPartCombinedName () <<
        "\" to \"" << fPartMSRName << "\"" <<
         endl;
  }
  else {
    // use the argument
    fPartMSRName = partMSRName;

    if (fMsrOptions->fTrace)
      cerr << idtr <<
        "Keeping partID \"" << partMSRName <<
        "\" as part name  for \"" <<
        getPartCombinedName () <<
        "\"" <<
      endl;
  }
}

string msrPart::getPartCombinedName () const
{
  return
    "\"" + fPartMSRName + "\"" +
    " (" + fPartMusicXMLID + ")";
}

void msrPart::setPartMeasureLocation (
  const msrMeasureLocation& measureLocation)
{
  // set part measure location
  fPartMeasureLocation = measureLocation;

  // propagate it to all staves
  setAllPartStavesMeasureLocation (measureLocation);  
}

void msrPart::setPartClef (S_msrClef clef)
{
//  if (ffMsrOptions->fTrace)
    cerr << idtr <<
      "Setting part clef \"" << clef->clefAsString () <<
      "\" in part \"" <<
      getPartCombinedName () <<
      "\"" <<
    endl;

  // set part clef
  fPartClef = clef;

  // propagate it to all staves
  setAllPartStavesClef (clef);
}

void msrPart::setPartKey  (S_msrKey  key)
{
//  if (ffMsrOptions->fTrace)
    cerr << idtr <<
      "Setting part key \"" << key->keyAsString () <<
      "\" in part \"" <<
      getPartCombinedName () <<
      "\"" <<
    endl;

  // set part key
  fPartKey = key;

  // propagate it to all staves
  setAllPartStavesKey (key);
}

void msrPart::setPartTime (S_msrTime time)
{
//  if (ffMsrOptions->fTrace)
    cerr << idtr <<
      "Setting part time \"" << time->timeAsString () <<
      "\" in part \"" <<
      getPartCombinedName () <<
      "\"" <<
    endl;

  // set part time
  fPartTime = time;

  // propagate it to all staves
  setAllPartStavesTime (time);
}

void msrPart::setPartTranspose (S_msrTranspose transpose)
{
//  if (ffMsrOptions->fTrace)
    cerr << idtr <<
      "Setting part transpose \"" << transpose->transposeAsString () <<
      "\" in part \"" <<
      getPartCombinedName () <<
      "\"" <<
    endl;

  // set part transpose
  fPartTranspose = transpose;

  // propagate it to all staves
  setAllPartStavesTranspose (transpose);
}

void msrPart::setAllPartStavesDivisionsPerWholeNote (int divisions)
{
  for (
    map<int, S_msrStaff>::iterator i = fPartStavesMap.begin();
    i != fPartStavesMap.end();
    i++) {
    (*i).second->setDivisionsPerWholeNote (divisions);
  } // for
}

void msrPart::setAllPartStavesMeasureLocation (
  const msrMeasureLocation& measureLocation)
{
  for (
    map<int, S_msrStaff>::iterator i = fPartStavesMap.begin();
    i != fPartStavesMap.end();
    i++) {
    (*i).second->setStaffMeasureLocation (measureLocation);
  } // for
}

void msrPart::setAllPartStavesClef (S_msrClef clef)
{
  for (
    map<int, S_msrStaff>::iterator i = fPartStavesMap.begin();
    i != fPartStavesMap.end();
    i++) {
    (*i).second->setStaffClef (clef);
  } // for
}

void msrPart::setAllPartStavesKey (S_msrKey  key)
{
  for (
    map<int, S_msrStaff>::iterator i = fPartStavesMap.begin();
    i != fPartStavesMap.end();
    i++) {
    (*i).second->setStaffKey (key);
  } // for
}
          
void msrPart::setAllPartStavesTime  (S_msrTime time)
{
  for (
    map<int, S_msrStaff>::iterator i = fPartStavesMap.begin();
    i != fPartStavesMap.end();
    i++) {
    (*i).second->setStaffTime (time);
  } // for
}
          
void msrPart::setAllPartStavesTranspose (S_msrTranspose transpose)
{
  for (
    map<int, S_msrStaff>::iterator i = fPartStavesMap.begin();
    i != fPartStavesMap.end();
    i++) {
    (*i).second->setStaffTranspose (transpose);
  } // for
}

S_msrStaff msrPart::addStaffToPart (
  int inputLineNumber,
  int staffNumber)
{
  if (fPartStavesMap.count (staffNumber)) {
    cerr << idtr <<
      "### Internal error: staffNumber " << staffNumber <<
      " already exists in part " << " " << getPartCombinedName () << endl;

    return fPartStavesMap [staffNumber];
  }

// JMI  if (fMsrOptions->fForceDebug || fMsrOptions->fTrace)
    cerr << idtr <<
      "Adding staff " << staffNumber <<
      " to part " << getPartCombinedName () << "\"" <<
      endl;
  
  // create the staff
  S_msrStaff
    staff =
      msrStaff::create (
        fMsrOptions,
        inputLineNumber,
        staffNumber,
        this);

  // register it in this part
  fPartStavesMap [staffNumber] = staff;

  // return it
  return staff;
}

void msrPart::addStaffToPart (S_msrStaff staff)
{
  if (fMsrOptions->fTrace)
    cerr << idtr <<
      "Adding staff " << staff->getStaffNumber () <<
      " to part " << getPartCombinedName () << endl;

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

void msrPart::acceptIn (basevisitor* v) {
  if (fMsrOptions->fDebugDebug)
    cerr << idtr <<
      "==> msrPart::acceptIn()" << endl;
      
  if (visitor<S_msrPart>*
    p =
      dynamic_cast<visitor<S_msrPart>*> (v)) {
        S_msrPart elem = this;
        
        if (fMsrOptions->fDebugDebug)
          cerr << idtr <<
            "==> Launching msrPart::visitStart()" << endl;
        p->visitStart (elem);
  }
}

void msrPart::acceptOut (basevisitor* v) {
  if (fMsrOptions->fDebugDebug)
    cerr << idtr <<
      "==> msrPart::acceptOut()" << endl;

  if (visitor<S_msrPart>*
    p =
      dynamic_cast<visitor<S_msrPart>*> (v)) {
        S_msrPart elem = this;
      
        if (fMsrOptions->fDebugDebug)
          cerr << idtr <<
            "==> Launching msrPart::visitEnd()" << endl;
        p->visitEnd (elem);
  }
}

void msrPart::browseData (basevisitor* v)
{
  if (fMsrOptions->fDebugDebug)
    cerr << idtr <<
      "==> msrPart::browseData()" << endl;
  
  for (
    map<int, S_msrStaff>::iterator i = fPartStavesMap.begin();
    i != fPartStavesMap.end();
    i++) {
    // browse the staff
    msrBrowser<msrStaff> browser (v);
    browser.browse (*((*i).second));
  } // for

  if (fMsrOptions->fDebug)
    cerr << idtr <<
      "<== msrPart::browseData()" << endl;
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
    " (" << fPartStavesMap.size() << " staves)" <<
    endl;
    
  idtr++;
  
  os << left <<
    idtr <<
      setw(22) << "DivisionsPerWholeNote" << ": " <<
      fDivisionsPerWholeNote << endl <<
    idtr <<
      setw(22) << "PartMSRName" << ": \"" <<
      fPartMSRName << "\"" << endl <<
    idtr <<
      setw(22) << "PartName" << ": \"" <<
      fPartName << "\"" << endl <<
    idtr <<
      setw(22) << "PartAbbrevation" << ": \"" <<
      fPartAbbreviation << "\"" << endl <<
    idtr <<
      setw(22) << "PartInstrumentName" << ": \"" <<
      fPartInstrumentName << "\"" << endl;

  if (fPartStavesMap.size()) {
    os << endl;
    for (
      map<int, S_msrStaff>::iterator i = fPartStavesMap.begin();
      i != fPartStavesMap.end();
      i++) {
      os << idtr << (*i).second;
    } // for
  }

  idtr--;
}

//______________________________________________________________________________
int msrPartgroup::gPartgroupsCounter = 0;

S_msrPartgroup msrPartgroup::create (
  S_msrOptions&          msrOpts, 
  int                    inputLineNumber,
  int                    partgroupNumber,
  string                 partgroupName,
  string                 partgroupAbbreviation,
  msrPartgroupSymbolKind partgroupSymbolKind,
  int                    partgroupSymbolDefaultX,
  bool                   partgroupBarline,
  S_msrPartgroup         partgroupPartgroupUplink)
{
  msrPartgroup* o =
    new msrPartgroup (
      msrOpts, inputLineNumber,
      partgroupNumber,
      partgroupName,
      partgroupAbbreviation,
      partgroupSymbolKind,
      partgroupSymbolDefaultX,
      partgroupBarline,
      partgroupPartgroupUplink);
  assert(o!=0);
  return o;
}

msrPartgroup::msrPartgroup (
  S_msrOptions&          msrOpts, 
  int                    inputLineNumber,
  int                    partgroupNumber,
  string                 partgroupName,
  string                 partgroupAbbreviation,
  msrPartgroupSymbolKind partgroupSymbolKind,
  int                    partgroupSymbolDefaultX,
  bool                   partgroupBarline,
  S_msrPartgroup         partgroupPartgroupUplink)
    : msrElement (msrOpts, inputLineNumber)
{
  fPartgroupAbsoluteNumber = ++gPartgroupsCounter;
  
  fPartgroupNumber = partgroupNumber;
        
  fPartgroupName = partgroupName;
  fPartgroupAbbreviation = partgroupAbbreviation;

  fPartgroupSymbolKind = partgroupSymbolKind;
  fPartgroupSymbolDefaultX = partgroupSymbolDefaultX;

  fPartgroupBarline = partgroupBarline;

  fPartgroupPartgroupUplink = partgroupPartgroupUplink;
  
  if (fMsrOptions->fTrace)
    cerr <<
      idtr <<
      "--------------------------------------------" <<
      endl <<
      idtr <<
      "Creating part group " << fPartgroupNumber <<
      endl;
}

msrPartgroup::~msrPartgroup() {}

S_msrPartgroup msrPartgroup::createEmptyPartgroupClone (
  S_msrPartgroup clonedPartgroup)
{

// JMI  cerr <<

  S_msrPartgroup
    clone =
      msrPartgroup::create (
        fMsrOptions,
        fInputLineNumber,
        fPartgroupNumber,
        fPartgroupName,
        fPartgroupAbbreviation,
        fPartgroupSymbolKind,
        fPartgroupSymbolDefaultX,
        fPartgroupBarline,
        clonedPartgroup);

  // avoid part group clone to keep its (new) absolute number
  clone->fPartgroupAbsoluteNumber =
    fPartgroupAbsoluteNumber;
  
  return clone;
}

string msrPartgroup::getPartgroupCombinedName () const
{
  stringstream s;

  s <<
    "PG_" << fPartgroupAbsoluteNumber <<
    " (" << fPartgroupNumber << ")";

  return s.str();
}

S_msrPart msrPartgroup::addPartToPartgroup (
  int    inputLineNumber,
  string partMusicXMLID)
{
  if (fPartgroupPartsMap.count (partMusicXMLID)) {
    cerr << idtr <<
      "### Internal error: partMusicXMLID " << partMusicXMLID <<
      " already exists in this part group" << endl;

    return fPartgroupPartsMap [partMusicXMLID];
  }

  // create the part
  S_msrPart
    part =
      msrPart::create (
        fMsrOptions,
        inputLineNumber,
        partMusicXMLID,
        this);

  // register it in this part group
  if (fMsrOptions->fTrace) {
    cerr << idtr <<
      "Adding part " <<
      part->getPartCombinedName () <<
      " to part group " << fPartgroupNumber << endl;
  }
  
  fPartgroupPartsMap [partMusicXMLID] = part;
  fPartgroupElements.push_back (part);

  if (fMsrOptions->fForceDebug || fMsrOptions->fDebugDebug) {
    cerr << idtr <<
      "==> After addPartToPartgroup, fPartgroupPartsMap contains:" << endl;
    idtr++;
    for (
        map<string, S_msrPart>::const_iterator i = fPartgroupPartsMap.begin();
        i != fPartgroupPartsMap.end();
        i++) {
      cerr << idtr <<
        "\"" << (*i).first << "\" ----> " <<
        (*i).second->getPartCombinedName() << endl;
    } // for
    idtr--;
    cerr << idtr << "<== addPartToPartgroup" << endl;
  }

  if (fMsrOptions->fForceDebug || fMsrOptions->fDebugDebug) {
    cerr << idtr <<
      "==> After addPartToPartgroup, fPartgroupPartsList contains:" << endl;
    if (fPartgroupElements.size()) {
      list<S_msrElement>::const_iterator
        iBegin = fPartgroupElements.begin(),
        iEnd   = fPartgroupElements.end(),
        i      = iBegin;
        
      idtr++;
      for ( ; ; ) {
        cerr << idtr << (*i);
        if (++i == iEnd) break;
        cerr << endl;
      } // for
      idtr--;
    }
    cerr << idtr << "<== addPartToPartgroup" << endl;
  }
  
  // return the part
  return part;
} // addPartToPartgroup

void msrPartgroup::addPartToPartgroup (S_msrPart part)
{
  // register part in this part group
  if (fMsrOptions->fTrace) {
    cerr << idtr <<
      "Adding part " <<
      part->getPartCombinedName () <<
      " to part group " << fPartgroupNumber << endl;
  }
  
  fPartgroupPartsMap [part->getPartMusicXMLID ()] = part;
  fPartgroupElements.push_back (part);
}

void msrPartgroup::prependSubPartgroupToPartgroup (
  S_msrPartgroup partgroup)
{
  if (fMsrOptions->fTrace)
    cerr << idtr <<
      "Adding (sub-)part group " << partgroup->getPartgroupNumber () <<
      " to part group " << getPartgroupNumber ()  << endl;

  // register it in this part group
  fPartgroupElements.push_front (partgroup);
}

S_msrPart msrPartgroup::fetchPartFromPartgroup (
  string partMusicXMLID)
{
  if (fMsrOptions->fDebugDebug) {
    cerr << idtr <<
      "==> fetchPartFromPartgroup, fPartgroupPartsMap contains:" << endl;
    for (
        map<string, S_msrPart>::const_iterator i = fPartgroupPartsMap.begin();
        i != fPartgroupPartsMap.end();
        i++) {
      cerr << idtr <<
        (*i).first << " ----> " <<
        (*i).second->getPartCombinedName() << endl;
    } // for
    cerr << idtr <<
      "<== fetchPartFromPartgroup" << endl;
  }
  
  S_msrPart result;
  
  if (fPartgroupPartsMap.count (partMusicXMLID)) {
    result = fPartgroupPartsMap [partMusicXMLID];
  }

  return result;
}

void msrPartgroup::acceptIn (basevisitor* v) {
  if (fMsrOptions->fDebugDebug)
    cerr << idtr <<
      "==> msrPartgroup::acceptIn()" << endl;
      
  if (visitor<S_msrPartgroup>*
    p =
      dynamic_cast<visitor<S_msrPartgroup>*> (v)) {
        S_msrPartgroup elem = this;
        
        if (fMsrOptions->fDebugDebug)
          cerr << idtr <<
            "==> Launching msrPartgroup::visitStart()" << endl;
        p->visitStart (elem);
  }
}

void msrPartgroup::acceptOut (basevisitor* v) {
  if (fMsrOptions->fDebugDebug)
    cerr << idtr <<
      "==> msrPartgroup::acceptOut()" << endl;

  if (visitor<S_msrPartgroup>*
    p =
      dynamic_cast<visitor<S_msrPartgroup>*> (v)) {
        S_msrPartgroup elem = this;
      
        if (fMsrOptions->fDebugDebug)
          cerr << idtr <<
            "==> Launching msrPartgroup::visitEnd()" << endl;
        p->visitEnd (elem);
  }
}

void msrPartgroup::browseData (basevisitor* v)
{
  if (fMsrOptions->fDebugDebug)
    cerr << idtr <<
      "==> msrPartgroup::browseData()" << endl;
  
  for (
    list<S_msrElement>::iterator i = fPartgroupElements.begin();
    i != fPartgroupElements.end();
    i++) {
    // browse the part element
    msrBrowser<msrElement> browser (v);
    browser.browse (*(*i));
  } // for

  if (fMsrOptions->fDebugDebug)
    cerr << idtr <<
      "<== msrPartgroup::browseData()" << endl;
}

ostream& operator<< (ostream& os, const S_msrPartgroup& elt)
{
  elt->print (os);
  return os;
}

string msrPartgroup::pargroupSymbolKindAsString (
  msrPartgroupSymbolKind partgroupSymbolKind)
{
  string result;
  
  switch (partgroupSymbolKind) {
    case k_NoPartgroupSymbol:
      result = "none";
      break;
    case kBracePartgroupSymbol:
      result = "brace";
      break;
    case kBracketPartgroupSymbol:
      result = "bracket";
      break;
    case kLinePartgroupSymbol:
      result = "line";
      break;
    case kSquarePartgroupSymbol:
      result = "square";
      break;
  } // switch

  return result;
}

void msrPartgroup::print (ostream& os)
{
  os <<
    "Partgroup" << " " << getPartgroupCombinedName () <<
    " (" << fPartgroupPartsMap.size() << " parts)" <<
    endl;
    
  idtr++;

  os << left <<
    idtr <<
      setw(24) << "PartgroupName" << " : \"" <<
      fPartgroupName <<
      "\"" <<
      endl <<
    idtr <<
      setw(24) << "PartgroupAbbrevation" << " : \"" <<
      fPartgroupAbbreviation <<
      "\"" <<
      endl <<
    idtr <<
      setw(24) << "fPartgroupSymbolDefaultX" << " : " <<
      fPartgroupSymbolDefaultX <<
        endl <<
    idtr <<
      setw(24) << "fPartgroupSymbolKind" << " : \"" <<
      pargroupSymbolKindAsString (fPartgroupSymbolKind) <<
      "\"" <<
      endl;
    
  os <<
    idtr << "PartgroupBarline         : ";
  if (fPartgroupBarline)
    os << "true";
  else
    os << "false";
  os << endl;

  if (fPartgroupElements.size()) {
    os << endl;
    list<S_msrElement>::const_iterator
      iBegin = fPartgroupElements.begin(),
      iEnd   = fPartgroupElements.end(),
      i      = iBegin;
      
    idtr++;
    for ( ; ; ) {
      os << idtr << (*i);
      if (++i == iEnd) break;
      os << endl;
    } // for
    idtr--;
  }
  
  idtr--;
}

//______________________________________________________________________________
S_msrIdentification msrIdentification::create (
  S_msrOptions& msrOpts, 
  int           inputLineNumber)
{
  msrIdentification* o =
    new msrIdentification (
      msrOpts, inputLineNumber);
  assert(o!=0);
  return o;
}

msrIdentification::msrIdentification (
  S_msrOptions& msrOpts, 
  int           inputLineNumber)
    : msrElement (msrOpts, inputLineNumber)
{}

msrIdentification::~msrIdentification() {}

void msrIdentification::setWorkNumber (
  int    inputLineNumber,
  string val)
  {
  fWorkNumber =
    msrVarValAssoc::create (
      fMsrOptions,
      inputLineNumber,
      "work-number", val);
  }

void msrIdentification::setWorkTitle (
  int    inputLineNumber,
  string val)
  {
  fWorkTitle =
    msrVarValAssoc::create (
      fMsrOptions,
      inputLineNumber,
      "work-title", val);
  }

void msrIdentification::setMovementNumber (
  int    inputLineNumber,
  string val)
  {
  fMovementNumber =
    msrVarValAssoc::create (
      fMsrOptions,
      inputLineNumber,
      "movement-number", val);
  }

void msrIdentification::setMovementTitle (
  int    inputLineNumber,
  string val)
{
  fMovementTitle =
    msrVarValAssoc::create (
      fMsrOptions,
      inputLineNumber,
      "movement-title", val);
}

void msrIdentification::addCreator (
  int    inputLineNumber,
  string type,
  string val)
{
  fCreators.push_back(
    msrVarValAssoc::create (
      fMsrOptions,
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
      fMsrOptions,
      inputLineNumber,
      "rights", val);
  }

void msrIdentification::addSoftware (
  int    inputLineNumber,
  string val)
{
  fSoftwares.push_back(
    msrVarValAssoc::create (
      fMsrOptions,
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
      fMsrOptions,
      inputLineNumber,
      "encoding-date", val);
}

void msrIdentification::setScoreInstrumentAssoc (
  int    inputLineNumber,
  string val)
{
  fScoreInstrumentAssoc =
    msrVarValAssoc::create (
      fMsrOptions,
      inputLineNumber,
      "score-instrument", val);
}

void msrIdentification::acceptIn (basevisitor* v) {
  if (fMsrOptions->fDebugDebug)
    cerr << idtr <<
      "==> msrIdentification::acceptIn()" << endl;
      
  if (visitor<S_msrIdentification>*
    p =
      dynamic_cast<visitor<S_msrIdentification>*> (v)) {
        S_msrIdentification elem = this;
        
        if (fMsrOptions->fDebugDebug)
          cerr << idtr <<
            "==> Launching msrIdentification::visitStart()" << endl;
        p->visitStart (elem);
  }
}

void msrIdentification::acceptOut (basevisitor* v) {
  if (fMsrOptions->fDebugDebug)
    cerr << idtr <<
      "==> msrIdentification::acceptOut()" << endl;

  if (visitor<S_msrIdentification>*
    p =
      dynamic_cast<visitor<S_msrIdentification>*> (v)) {
        S_msrIdentification elem = this;
      
        if (fMsrOptions->fDebugDebug)
          cerr << idtr <<
            "==> Launching msrIdentification::visitEnd()" << endl;
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

  if (!fCreators.empty()) {
    vector<S_msrVarValAssoc>::const_iterator i;
    for (i=fCreators.begin(); i!=fCreators.end(); i++) {
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
  os << "Identification" << endl;

  bool emptyIdentification = true;

  idtr++;
  
  if (fWorkNumber) {
    os << idtr << fWorkNumber;
    emptyIdentification = false;
  }
  
  if (fWorkTitle) {
    os << idtr << fWorkTitle;
    emptyIdentification = false;
  }
    
  if (fMovementNumber) {
    os << idtr << fMovementNumber;
    emptyIdentification = false;
  }
    
  if (fMovementTitle) {
    os << idtr << fMovementTitle;
    emptyIdentification = false;
  }
    // JMI for   ; ; 
  if (!fCreators.empty()) {
    vector<S_msrVarValAssoc>::const_iterator i1;
    for (i1=fCreators.begin(); i1!=fCreators.end(); i1++) {
      os << idtr << (*i1);
    } // for
    emptyIdentification = false;
  }
    
  if (fRights) {
    os << idtr << fRights;
    emptyIdentification = false;
  }
    
  if (!fSoftwares.empty()) {
    vector<S_msrVarValAssoc>::const_iterator i2;
    for (i2=fSoftwares.begin(); i2!=fSoftwares.end(); i2++) {
      os << idtr << (*i2);
    } // for
    emptyIdentification = false;
  }
    
  if (fEncodingDate) {
    os << idtr << fEncodingDate;
    emptyIdentification = false;
  }

  if (emptyIdentification)
    os << idtr <<
      " " << "nothing specified" << endl;

  os << endl;
  
  idtr--;
}

//______________________________________________________________________________
S_msrScore msrScore::create (
  S_msrOptions& msrOpts, 
  int           inputLineNumber)
{
  msrScore* o =
    new msrScore (
      msrOpts, inputLineNumber);
  assert(o!=0);
  return o;
}

msrScore::msrScore (
  S_msrOptions& msrOpts, 
  int           inputLineNumber)
    : msrElement (msrOpts, inputLineNumber)
{
  fIdentification =
    msrIdentification::create (
      msrOpts, inputLineNumber);
      
  fPageGeometry =
    msrPageGeometry::create (
      msrOpts, inputLineNumber);
}

msrScore::~msrScore() {}

S_msrScore msrScore::createEmptyClone ()
{
  S_msrScore
    clone =
      msrScore::create (
        fMsrOptions,
        fInputLineNumber);

  return clone;
}

void msrScore::addPartgroupToScore (S_msrPartgroup partgroup)
{
  /* JMI
  if (fScorePartgroupsMap.count (partgroupNumber)) {
    cerr << idtr <<
      "### Internal error: part group " << partgroupNumber <<
      " already exists in this score" << endl;

    return fScorePartgroupsMap [partgroupNumber];
  }
*/

  // register it in this score
  fPartgroupsList.push_back (partgroup);
}

/*
S_msrPartgroup msrScore::fetchScorePartgroup (
  int partgroupNumber)
{
  S_msrPartgroup result;
  
  if (fScorePartgroupsMap.count (partgroupNumber)) {
    result = fScorePartgroupsMap [partgroupNumber];
  }

  return result;
}
*/

void msrScore::acceptIn (basevisitor* v) {
  if (fMsrOptions->fDebugDebug)
    cerr << idtr <<
      "==> msrScore::acceptIn()" << endl;
      
  if (visitor<S_msrScore>*
    p =
      dynamic_cast<visitor<S_msrScore>*> (v)) {
        S_msrScore elem = this;
        
        if (fMsrOptions->fDebugDebug)
          cerr << idtr <<
            "==> Launching msrScore::visitStart()" << endl;
        p->visitStart (elem);
  }
}

void msrScore::acceptOut (basevisitor* v) {
  if (fMsrOptions->fDebugDebug)
    cerr << idtr <<
      "==> msrScore::acceptOut()" << endl;

  if (visitor<S_msrScore>*
    p =
      dynamic_cast<visitor<S_msrScore>*> (v)) {
        S_msrScore elem = this;
      
        if (fMsrOptions->fDebugDebug)
          cerr << idtr <<
            "==> Launching msrScore::visitEnd()" << endl;
        p->visitEnd (elem);
  }
}

void msrScore::browseData (basevisitor* v)
{
  if (fMsrOptions->fDebugDebug)
    cerr << idtr <<
      "==> msrScore::browseData()" << endl;
    
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
    list<S_msrPartgroup>::iterator i = fPartgroupsList.begin();
    i != fPartgroupsList.end();
    i++) {
    // browse the part group
    msrBrowser<msrPartgroup> browser (v);
    browser.browse (*(*i));
  } // for

  if (fMsrOptions->fDebugDebug)
    cerr << idtr <<
      "<== msrScore::browseData()" << endl;
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
    " (" << fPartgroupsList.size() << " part groups)" <<
    endl << endl;

  idtr++;
  
  if (fIdentification) {
    os << idtr << fIdentification;
  }
  
  if (fPageGeometry) {
    os << idtr << fPageGeometry;
  }
  
  for (
    list<S_msrPartgroup>::iterator i = fPartgroupsList.begin();
    i != fPartgroupsList.end();
    i++) {
    os << idtr << (*i);
  } // for
  
  idtr--;
}

//______________________________________________________________________________
S_msrMidi msrMidi::create (
  S_msrOptions& msrOpts, 
  int                    inputLineNumber)
{
  msrMidi* o =
    new msrMidi (
      msrOpts, inputLineNumber);
  assert(o!=0);
  return o;
}

msrMidi::msrMidi (
  S_msrOptions& msrOpts, 
  int                    inputLineNumber)
    : msrElement (msrOpts, inputLineNumber)
{
}
msrMidi::~msrMidi() {}

void msrMidi::acceptIn (basevisitor* v) {
  if (fMsrOptions->fDebugDebug)
    cerr << idtr <<
      "==> msrMidi::acceptIn()" << endl;
      
  if (visitor<S_msrMidi>*
    p =
      dynamic_cast<visitor<S_msrMidi>*> (v)) {
        S_msrMidi elem = this;
        
        if (fMsrOptions->fDebugDebug)
          cerr << idtr <<
            "==> Launching msrMidi::visitStart()" << endl;
        p->visitStart (elem);
  }
}

void msrMidi::acceptOut (basevisitor* v) {
  if (fMsrOptions->fDebugDebug)
    cerr << idtr <<
      "==> msrMidi::acceptOut()" << endl;

  if (visitor<S_msrMidi>*
    p =
      dynamic_cast<visitor<S_msrMidi>*> (v)) {
        S_msrMidi elem = this;
      
        if (fMsrOptions->fDebugDebug)
          cerr << idtr <<
            "==> Launching msrMidi::visitEnd()" << endl;
        p->visitEnd (elem);
  }
}


void msrMidi::browseData (basevisitor* v)
{}

ostream& operator<< (ostream& os, const S_msrMidi& mid)
{
  mid->print (os);
  return os;
}

void msrMidi::print (ostream& os)
{
  os << "Midi" << endl;

  idtr++;
  
  os << idtr << "% to be completed" << endl;
  
  idtr--;
}


}

/*
  switch (lyricschunkKind) {
    case msrLyricschunk::kSingleChunk:
    case msrLyricschunk::kBeginChunk:
      {  
      // add lyrics chunk to this lyrics
      fLyricschunks.push_back (chunk);
      }
      break;

    case msrLyricschunk::kMiddleChunk:
    case msrLyricschunk::kEndChunk:
      // add chunk to this lyrics
      fLyricschunks.push_back (chunk);
      break;
      
    case msrLyricschunk::kSkipChunk:
    case msrLyricschunk::kSlurChunk:
    case msrLyricschunk::kSlurBeyondEndChunk:
    case msrLyricschunk::kTiedChunk:
    case msrLyricschunk::kBarcheckChunk:
    case msrLyricschunk::kBreakChunk:
      {
        msrInternalError (
          fMsrOptions->fInputSourceName,
          fInputLineNumber,
          "a text chunk type can only be "
          "'single', 'begin', 'middle' or 'end'");
      }
      break;
      
    case msrLyricschunk::k_NoChunk:
      msrInternalError (
        fMsrOptions->fInputSourceName,
        fInputLineNumber,
        "lyrics chunk type has not been set");
      break;
  } // switch


*/


/*
  switch (lyricschunkKind) {
    case msrLyricschunk::kSingleChunk:
    case msrLyricschunk::kBeginChunk:
      {  
      // add lyrics chunk to this lyrics
      fLyricschunks.push_back (lyricshunk);
      }
      break;

    case msrLyricschunk::kMiddleChunk:
    case msrLyricschunk::kEndChunk:
      // add chunk to this lyrics
      fLyricschunks.push_back (lyricshunk);
      break;
      
    case msrLyricschunk::kSkipChunk:
    case msrLyricschunk::kSlurChunk:
    case msrLyricschunk::kSlurBeyondEndChunk:
    case msrLyricschunk::kTiedChunk:
    case msrLyricschunk::kBarcheckChunk:
    case msrLyricschunk::kBreakChunk:
      {
        msrInternalError (
          fMsrOptions->fInputSourceName,
          fInputLineNumber,
          "a text chunk type can only be "
          "'single', 'begin', 'middle' or 'end'");
      }
      break;
      
    case msrLyricschunk::k_NoChunk:
      msrInternalError (
        fMsrOptions->fInputSourceName,
        fInputLineNumber,
        "lyrics chunk type has not been set");
      break;
  } // switch

  fLyricsTextPresent = true;
  */

  /*
    switch (fNoteMsrPitch) {
      
      case k_aeseh: s << "aeseh"; break;
      case k_aes:   s << "aes"; break;
      case k_aeh:   s << "aeh"; break;
      case k_a:     s << "a"; break;
      case k_aih:   s << "aih"; break;
      case k_ais:   s << "ais"; break;
      case k_aisih: s << "aisih"; break;
        
      case k_beseh: s << "beseh"; break;
      case k_bes:   s << "bes"; break;
      case k_beh:   s << "beh"; break;
      case k_b:     s << "b"; break;
      case k_bih:   s << "bih"; break;
      case k_bis:   s << "bis"; break;
      case k_bisih: s << "bisih"; break;
        
      case k_ceseh: s << "ceseh"; break;
      case k_ces:   s << "ces"; break;
      case k_ceh:   s << "ceh"; break;
      case k_c:     s << "c"; break;
      case k_cih:   s << "cih"; break;
      case k_cis:   s << "cis"; break;
      case k_cisih: s << "cisih"; break;
        
      case k_deseh: s << "deseh"; break;
      case k_des:   s << "des"; break;
      case k_deh:   s << "deh"; break;
      case k_d:     s << "d"; break;
      case k_dih:   s << "dih"; break;
      case k_dis:   s << "dis"; break;
      case k_disih: s << "disih"; break;
  
      case k_eeseh: s << "eeseh"; break;
      case k_ees:   s << "ees"; break;
      case k_eeh:   s << "eeh"; break;
      case k_e:     s << "e"; break;
      case k_eih:   s << "eih"; break;
      case k_eis:   s << "eis"; break;
      case k_eisih: s << "eisih"; break;
        
      case k_feseh: s << "feseh"; break;
      case k_fes:   s << "fes"; break;
      case k_feh:   s << "feh"; break;
      case k_f:     s << "f"; break;
      case k_fih:   s << "fih"; break;
      case k_fis:   s << "fis"; break;
      case k_fisih: s << "fisih"; break;
        
      case k_geseh: s << "geseh"; break;
      case k_ges:   s << "ges"; break;
      case k_geh:   s << "geh"; break;
      case k_g:     s << "g"; break;
      case k_gih:   s << "gih"; break;
      case k_gis:   s << "gis"; break;
      case k_gisih: s << "gisih"; break;
      
      default: s << "Note" << fNoteMsrPitch << "???";
    } // switch
    */
