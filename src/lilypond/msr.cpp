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

musicXMLLocation  gCurrentMusicXMLLocation;

msrGlobalVariables::DisplayKind
  msrGlobalVariables::sDisplayKind =
    msrGlobalVariables::kLilypondCode;

indenter msrElement::idtr;

//______________________________________________________________________________
/* JMI
musicXMLLocation::musicXMLLocation (
  int         inputLineNumber,
  int         measureNumber,
  int         positionInMeasure)
{
  fInputLineNumber   = inputLineNumber;
  fMeasureNumber     = measureNumber;
  fPositionInMeasure = positionInMeasure;
}
*/
//______________________________________________________________________________
void msrMusicXMLWarning (int inputLineNumber, string message)
{
  cerr <<
    idtr <<
      "--> MusicXML WARNING, input line " <<
        inputLineNumber <<
      ", measure " <<
        gCurrentMusicXMLLocation.fMeasureNumber <<
      ":" <<
        gCurrentMusicXMLLocation.fPositionInMeasure <<

    endl <<
    
    idtr <<
      "      " << message << endl;
}

void msrMusicXMLError (int inputLineNumber, string message)
{
  cerr <<
    idtr <<
      endl <<
      "--> MusicXML ERROR, input line " <<
        inputLineNumber  <<
      ", measure " <<
        gCurrentMusicXMLLocation.fMeasureNumber <<
      ":" <<
        gCurrentMusicXMLLocation.fPositionInMeasure <<

    endl <<
      
    idtr <<
      "      " << message << endl <<
    endl;
    
  assert(false);
}

void msrInternalError (int inputLineNumber, string message)
{
  cerr <<
    endl <<

    idtr <<
      "--> MSR INTERNAL ERROR, input line " <<
        inputLineNumber  <<
      ", measure " <<
        gCurrentMusicXMLLocation.fMeasureNumber <<
      ":" <<
        gCurrentMusicXMLLocation.fPositionInMeasure << "/" ;
      
  if (gCurrentMusicXMLLocation.fPositionInMeasure > 0)
    cerr << gCurrentMusicXMLLocation.fPositionInMeasure;
  else
    cerr << "?";
    
  cerr << endl ;
    
  cerr <<
    idtr <<
      "      " << message <<
    endl << endl;
    
  assert(false);
}

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

MsrNoteNamesLanguage getMsrNoteNamesLanguage (std::string lang)
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
/* JMI

S_msrElement msrElement::create (
  S_msrOptions& msrOpts, 
  int                    inputLineNumber)
{
  msrElement * o =
    new msrElement (
      msrOpts, inputLineNumber);
  assert(o!=0);
  return o; 
}
*/

msrElement::msrElement (
  S_msrOptions& msrOpts, 
  int                    inputLineNumber)
{
  fMsrOptions = msrOpts;
  fInputLineNumber = inputLineNumber;  
}

msrElement::~msrElement() {}

/* JMI
string msrElementType (S_msrElement elem)
{
  if (dynamic_cast elem (elem)) {
  }
}
*/

void msrElement::acceptIn (basevisitor* v) {
  if (fMsrOptions->fDebug)
    cerr <<
      "==> msrElement::acceptIn()" << endl;
      
  if (visitor<S_msrElement>*
    p =
      dynamic_cast<visitor<S_msrElement>*> (v)) {
        S_msrElement elem = this;
        
        if (fMsrOptions->fDebug)
          cerr <<
            "==> Launching msrElement::visitStart()" << endl;
        p->visitStart (elem);
  }
}

void msrElement::acceptOut (basevisitor* v) {
  if (fMsrOptions->fDebug)
    cerr <<
      "==> msrElement::acceptOut()" << endl;

  if (visitor<S_msrElement>*
    p =
      dynamic_cast<visitor<S_msrElement>*> (v)) {
        S_msrElement elem = this;
      
        if (fMsrOptions->fDebug)
          cerr <<
            "==> Launching msrElement::visitEnd()" << endl;
        p->visitEnd (elem);
  }
}

ostream& operator<< (ostream& os, const S_msrElement& elt)
{
  elt->print(os);
  return os;
}

void msrElement::print (ostream& os)
{
  // a global variable is needed so that msr::Element.print() 
  // can decide whether to print:
  //   - the MSR structure
  //   - MusicXML text
  //   - LilyPond source code
  switch (msrGlobalVariables::getDisplayKind ()) {
    case msrGlobalVariables::kMSR:
      this->printMSR (os);
      break;
    case msrGlobalVariables::kMusicXML:
      this->printMusicXML (os);
      break;
    case msrGlobalVariables::kScoreSummary:
      this->printScoreSummary (os);
      break;
    case msrGlobalVariables::kLilypondCode:
      this->printLilyPondCode (os);
      break;
    case msrGlobalVariables::k_NoDisplay:
      msrInternalError (
        fInputLineNumber,
        "global variable 'display', "
        "used by msrElement::print(ostream& os), "
        "has not been set ");
  } // switch
}

void msrElement::printMusicXML (ostream& os)
{
  os << "<!-- msrElement??? -->" << endl;
}

void msrElement::printMSR (ostream& os)
{
  os << "Element???" << endl;
}

void msrElement::printScoreSummary (ostream& os)
{
  os << "Element???" << endl;
}

void msrElement::printLilyPondCode (ostream& os)
{
  os << "\%{ msrElement??? \%}" << endl;
}

//______________________________________________________________________________
ostream& operator<< (ostream& os, musicXMLNoteData& data)
{
  data.print(os);
  return os;
}

void musicXMLNoteData::print (ostream& os)
{
  os <<
    idtr << "  " << left << setw(26) <<
      "fMusicXMLStep = " <<
      fMusicXMLStep <<  endl <<
    idtr << "  " << left << setw(26) <<
      "fMusicXMLStepIsARest = " <<
      fMusicXMLStepIsARest <<  endl <<
    idtr << "  " << left << setw(26) <<
      "fMusicXMLAlteration = " <<
      fMusicXMLAlteration <<  endl <<
    idtr << "  " << left << setw(26) <<
      "fMusicXMLOctave = " <<
      fMusicXMLOctave <<  endl <<
    idtr << "  " << left << setw(26) <<
      "fMusicXMLDivisions = " <<
      fMusicXMLDivisions <<  endl <<
    idtr << "  " << left << setw(26) <<
      "fMusicXMLDuration = " <<
      fMusicXMLDuration <<  endl <<
    idtr << "  " << left << setw(26) <<
      "fMusicXMLDotsNumber = " <<
      fMusicXMLDotsNumber <<  endl <<
      
//    idtr << endl <<
    
    idtr << "  " << left << setw(26) <<
      "fMusicXMLNoteBelongsToAChord = " <<
      fMusicXMLNoteBelongsToAChord <<  endl <<
      
//    idtr << endl <<
    
    idtr << "  " << left << setw(26) <<
      "fMusicXMLNoteBelongsToATuplet = " <<
      fMusicXMLNoteBelongsToATuplet <<  endl <<
    idtr << "  " << left << setw(26) <<
      "fMusicXMLTupletMemberNoteType = " <<
      fMusicXMLTupletMemberNoteType <<  endl <<
      
//    idtr << endl <<
    
    idtr << "  " << left << setw(26) <<
      "fMusicXMLVoiceNumber = " <<
      fMusicXMLVoiceNumber <<  endl;
};

void musicXMLBeatData::print (ostream& os)
{
  os <<
    "  " << left << setw(26) << "fBeatUnit = " << fBeatUnit <<  endl <<
    "  " << left << setw(26) << "fDots = " << fDots <<  endl;
};

//______________________________________________________________________________
S_msrDuration msrDuration::create (
  S_msrOptions& msrOpts, 
  int                    inputLineNumber,
  int                    num,
  int                    denom,
  int                    dots,
  string                 tupletMemberType)
{
  msrDuration * o =
    new msrDuration (
      msrOpts, inputLineNumber,
      num, denom, dots, tupletMemberType);
  assert(o!=0); 
  return o;
}

msrDuration::msrDuration (
  S_msrOptions& msrOpts, 
  int                    inputLineNumber,
  int                    num,
  int                    denom,
  int                    dots,
  string                 tupletMemberType)
    : msrElement (msrOpts, inputLineNumber)
{
  fNum   = num;
  fDenom = denom;
  fDots  = dots;
  fTupletMemberNoteType = tupletMemberType;
  /*
  cerr <<
    "msrDuration::msrDuration (), fNum = " << fNum << 
    ", fDenom = " << fDenom << ", fDots = " << fDots << endl;
  */
  if (fDenom == 0) {
    stringstream s;
    s << 
      endl << 
      "duration " << fNum << "/" << fDenom <<
      " has 0 as denominator" <<
      endl;
    msrMusicXMLError (
      fInputLineNumber, s.str());
  }
}

msrDuration::~msrDuration() {}

void msrDuration::scaleNumByFraction (int num, int denom)
{
  fNum *= num/denom;
}

void msrDuration::acceptIn (basevisitor* v) {
  if (fMsrOptions->fDebug)
    cerr << idtr <<
      "==> msrDuration::acceptIn()" << endl;
      
  if (visitor<S_msrDuration>*
    p =
      dynamic_cast<visitor<S_msrDuration>*> (v)) {
        S_msrDuration elem = this;
        
        if (fMsrOptions->fDebug)
          cerr << idtr <<
            "==> Launching msrDuration::visitStart()" << endl;
        p->visitStart (elem);
  }
}

void msrDuration::acceptOut (basevisitor* v) {
  if (fMsrOptions->fDebug)
    cerr << idtr <<
      "==> msrDuration::acceptOut()" << endl;

  if (visitor<S_msrDuration>*
    p =
      dynamic_cast<visitor<S_msrDuration>*> (v)) {
        S_msrDuration elem = this;
      
        if (fMsrOptions->fDebug)
          cerr << idtr <<
            "==> Launching msrDuration::visitEnd()" << endl;
        p->visitEnd (elem);
  }
}


void msrDuration::browseData (basevisitor* v)
{}

ostream& operator<< (ostream& os, const S_msrDuration& dur)
{
  dur->print(os);
  return os;
}

void msrDuration::printMusicXML (ostream& os)
{
  os << "<!-- msrDuration??? -->" << endl;
}

rational msrDuration::durationAsRational ()
{
  return rational (fNum, fDenom); // TEMP JMI
}

string msrDuration::durationAsMSRString ()
{
  // divisions are per quater, Msr durations are in whole notes
//  cerr << "|"  << fNum << "|" << fDenom << "|" << fDots << "|" << endl;

  int noteDivisions         = fNum;
  int divisionsPerWholeNote = fDenom ;
  
  stringstream s;
  
  if (fTupletMemberNoteType.size()) {

    if      (fTupletMemberNoteType == "256th")   { s << "256"; }
    else if (fTupletMemberNoteType == "128th")   { s << "128"; } 
    else if (fTupletMemberNoteType == "64th")    { s << "64"; } 
    else if (fTupletMemberNoteType == "32nd")    { s << "32"; } 
    else if (fTupletMemberNoteType == "16th")    { s << "16"; } 
    else if (fTupletMemberNoteType == "eighth")  { s << "8"; } 
    else if (fTupletMemberNoteType == "quarter") { s << "4"; } 
    else if (fTupletMemberNoteType == "half")    { s << "2"; } 
    else if (fTupletMemberNoteType == "whole")   { s << "1"; } 
    else if (fTupletMemberNoteType == "breve")   { s << "breve"; } 
    else if (fTupletMemberNoteType == "long")    { s << "long"; }
    else
      {
      stringstream s;
      s << 
        endl << 
        "--> unknown tuplet member type " << fTupletMemberNoteType <<
        endl;
      msrMusicXMLError (
        fInputLineNumber, s.str());
      }
        
  } else {
    
    div_t divresult = div (noteDivisions, divisionsPerWholeNote);  
    int   div       = divresult.quot;
    //int   mod       = divresult.rem; not yet used JMI
        
    switch (div) {
      case 8:
      case 7:
      case 6:
      case 5:
        s << "\\maxima";
        break;
      case 4:
      case 3:
        s << "\\longa";
        break;
      case 2:
        s << "\\breve";
        break;
      case 1:
        s << "1";
        break;
      case 0:
        {
        // shorter than a whole note
        //s << "(shorter than a whole note) ";
        int weight = 2; // half note
        int n = noteDivisions*2;
  
        while (n < divisionsPerWholeNote) {
           weight *= 2;
           n *= 2;
        } // while
        s << weight;
        }
        break;
      default:
        {
        stringstream s;
        s <<
          "*** ERROR, MusicXML note duration " << noteDivisions << "/" << 
          divisionsPerWholeNote << " is too large" << endl;
        msrMusicXMLError (
          fInputLineNumber, s.str());
        }
    } // switch
  }
  
  //cerr << "--> fDots = " << fDots << endl;
  
  // print the dots if any 
  int n = fDots; 
  if (n > 0) {
    while (n-- > 0) {
      s << ".";  
    } // while
  }
    
  return s.str();
}

void msrDuration::printMSR (ostream& os)
{
  os << durationAsMSRString () << flush;
}

void msrDuration::printScoreSummary (ostream& os)
{}

void msrDuration::printLilyPondCode (ostream& os)
{
  /*
  enum NoteFigures = {
    k1024th, k512th, 256th, k128th, k64th, k32nd, k16th, 
    kEighth, kQuarter, kHalf, kWhole, kBreve, kLong, kMaxima};
*/

  os << durationAsMSRString ();
}

//______________________________________________________________________________
S_msrArticulation msrArticulation::create (
  S_msrOptions& msrOpts, 
  int                    inputLineNumber,
  ArticulationKind       articulationKind)
{
  msrArticulation* o =
    new msrArticulation (
      msrOpts, inputLineNumber, articulationKind);
  assert (o!=0);
  return o;
}

msrArticulation::msrArticulation (
  S_msrOptions& msrOpts, 
  int                    inputLineNumber,
  ArticulationKind       articulationKind)
    : msrElement (msrOpts, inputLineNumber)
{
  fArticulationKind = articulationKind;
}

msrArticulation::~msrArticulation() {}

void msrArticulation::acceptIn (basevisitor* v) {
  if (fMsrOptions->fDebug)
    cerr << idtr <<
      "==> msrArticulation::acceptIn()" << endl;
      
  if (visitor<S_msrArticulation>*
    p =
      dynamic_cast<visitor<S_msrArticulation>*> (v)) {
        S_msrArticulation elem = this;
        
        if (fMsrOptions->fDebug)
          cerr << idtr <<
            "==> Launching msrArticulation::visitStart()" << endl;
        p->visitStart (elem);
  }
}

void msrArticulation::acceptOut (basevisitor* v) {
  if (fMsrOptions->fDebug)
    cerr << idtr <<
      "==> msrArticulation::acceptOut()" << endl;

  if (visitor<S_msrArticulation>*
    p =
      dynamic_cast<visitor<S_msrArticulation>*> (v)) {
        S_msrArticulation elem = this;
      
        if (fMsrOptions->fDebug)
          cerr << idtr <<
            "==> Launching msrArticulation::visitEnd()" << endl;
        p->visitEnd (elem);
  }
}

void msrArticulation::browseData (basevisitor* v)
{}

ostream& operator<< (ostream& os, const S_msrArticulation& elt)
{
  elt->print(os);
  return os;
}

void msrArticulation::printMusicXML (ostream& os)
{
  os << "<!-- msrTime??? -->" << endl;
}

void msrArticulation::printMSR (ostream& os)
{
  os << "Articulation" << " ";

  switch (fArticulationKind) {
    case kStaccato:
      os << "staccato";
      break;
    case kStaccatissimo:
      os << "staccatissimo";
      break;
    case kFermata:
      os << "fermata";
      break;
  } // switch
  
  os << endl;
}

void msrArticulation::printScoreSummary (ostream& os)
{}

void msrArticulation::printLilyPondCode (ostream& os)
{
  switch (fArticulationKind) {
    case kStaccato:
      os << "-.";
      break;
    case kStaccatissimo:
      os << "-^";
      break;
    case kFermata:
      os << "\fermata";
      break;
  } // switch
}

//______________________________________________________________________________
S_msrDynamics msrDynamics::create (
  S_msrOptions& msrOpts, 
  int                    inputLineNumber,
  DynamicsKind           dynamicsKind)
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
  DynamicsKind           dynamicsKind)
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
        fInputLineNumber, s.str());
      }
  } // switch
  
  return s.str();
}

string msrDynamics::dynamicsKindAsLilypondString ()
{
  return "\\"+dynamicsKindAsString ();
}

void msrDynamics::acceptIn (basevisitor* v) {
  if (fMsrOptions->fDebug)
    cerr << idtr <<
      "==> msrDynamics::acceptIn()" << endl;
      
  if (visitor<S_msrDynamics>*
    p =
      dynamic_cast<visitor<S_msrDynamics>*> (v)) {
        S_msrDynamics elem = this;
        
        if (fMsrOptions->fDebug)
          cerr << idtr <<
            "==> Launching msrDynamics::visitStart()" << endl;
        p->visitStart (elem);
  }
}

void msrDynamics::acceptOut (basevisitor* v) {
  if (fMsrOptions->fDebug)
    cerr << idtr <<
      "==> msrDynamics::acceptOut()" << endl;

  if (visitor<S_msrDynamics>*
    p =
      dynamic_cast<visitor<S_msrDynamics>*> (v)) {
        S_msrDynamics elem = this;
      
        if (fMsrOptions->fDebug)
          cerr << idtr <<
            "==> Launching msrDynamics::visitEnd()" << endl;
        p->visitEnd (elem);
  }
}


void msrDynamics::browseData (basevisitor* v)
{}

ostream& operator<< (ostream& os, const S_msrDynamics& dyn)
{
  dyn->print(os);
  return os;
}

void msrDynamics::printMusicXML (ostream& os)
{
  os << "<!-- msrDynamics??? -->" << endl;
}

void msrDynamics::printMSR (ostream& os)
{
  os <<
    "Dynamics" << " " << dynamicsKindAsString () << endl;
}

void msrDynamics::printScoreSummary (ostream& os)
{}

void msrDynamics::printLilyPondCode (ostream& os)
{
  os << dynamicsKindAsLilypondString ();
}

//______________________________________________________________________________
S_msrWedge msrWedge::create (
  S_msrOptions& msrOpts, 
  int                    inputLineNumber,
  WedgeKind              wedgeKind)
{
  msrWedge* o =
    new msrWedge (
      msrOpts, inputLineNumber, wedgeKind);
  assert(o!=0);
  return o;
}

msrWedge::msrWedge (
  S_msrOptions& msrOpts, 
  int                    inputLineNumber,
  WedgeKind              wedgeKind)
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
  if (fMsrOptions->fDebug)
    cerr << idtr <<
      "==> msrWedge::acceptIn()" << endl;
      
  if (visitor<S_msrWedge>*
    p =
      dynamic_cast<visitor<S_msrWedge>*> (v)) {
        S_msrWedge elem = this;
        
        if (fMsrOptions->fDebug)
          cerr << idtr <<
            "==> Launching msrWedge::visitStart()" << endl;
        p->visitStart (elem);
  }
}

void msrWedge::acceptOut (basevisitor* v) {
  if (fMsrOptions->fDebug)
    cerr << idtr <<
      "==> msrWedge::acceptOut()" << endl;

  if (visitor<S_msrWedge>*
    p =
      dynamic_cast<visitor<S_msrWedge>*> (v)) {
        S_msrWedge elem = this;
      
        if (fMsrOptions->fDebug)
          cerr << idtr <<
            "==> Launching msrWedge::visitEnd()" << endl;
        p->visitEnd (elem);
  }
}


void msrWedge::browseData (basevisitor* v)
{}

ostream& operator<< (ostream& os, const S_msrWedge& wdg)
{
  wdg->print(os);
  return os;
}

void msrWedge::printMusicXML (ostream& os)
{
  os << "<!-- msrWedge??? -->" << endl;
}

void msrWedge::printMSR (ostream& os)
{
  os << "Wedge" << " " << wedgeKindAsString () << endl;
}

void msrWedge::printScoreSummary (ostream& os)
{}

void msrWedge::printLilyPondCode (ostream& os)
{
  os << wedgeKindAsString ();
}

//______________________________________________________________________________
S_msrSlur msrSlur::create (
  S_msrOptions& msrOpts, 
  int                    inputLineNumber,
  SlurKind               slurKind)
{
  msrSlur* o =
    new msrSlur (
      msrOpts, inputLineNumber, slurKind);
  assert(o!=0);
  return o;
}

msrSlur::msrSlur (
  S_msrOptions& msrOpts, 
  int                    inputLineNumber,
  SlurKind               slurKind)
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
      s << "crescendo";
      break;
    case msrSlur::kContinueSlur:
      s << "decrescendo";
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
  if (fMsrOptions->fDebug)
    cerr << idtr <<
      "==> msrSlur::acceptIn()" << endl;
      
  if (visitor<S_msrSlur>*
    p =
      dynamic_cast<visitor<S_msrSlur>*> (v)) {
        S_msrSlur elem = this;
        
        if (fMsrOptions->fDebug)
          cerr << idtr <<
            "==> Launching msrSlur::visitStart()" << endl;
        p->visitStart (elem);
  }
}

void msrSlur::acceptOut (basevisitor* v) {
  if (fMsrOptions->fDebug)
    cerr << idtr <<
      "==> msrSlur::acceptOut()" << endl;

  if (visitor<S_msrSlur>*
    p =
      dynamic_cast<visitor<S_msrSlur>*> (v)) {
        S_msrSlur elem = this;
      
        if (fMsrOptions->fDebug)
          cerr << idtr <<
            "==> Launching msrSlur::visitEnd()" << endl;
        p->visitEnd (elem);
  }
}


void msrSlur::browseData (basevisitor* v)
{}

ostream& operator<< (ostream& os, const S_msrSlur& wdg)
{
  wdg->print(os);
  return os;
}

void msrSlur::printMusicXML (ostream& os)
{
  os << "<!-- msrSlur??? -->" << endl;
}

void msrSlur::printMSR (ostream& os)
{
  os << "Slur" << " " << slurKindAsString () << endl;
}

void msrSlur::printScoreSummary (ostream& os)
{}

void msrSlur::printLilyPondCode (ostream& os)
{
  os << slurKindAsString ();
}

//______________________________________________________________________________
S_msrNote msrNote::createFromMusicXMLData (
  S_msrOptions& msrOpts,
  int                    inputLineNumber,
  musicXMLNoteData&      mxmldat,
  msrSlur::SlurKind      slurKind)
{  
  msrNote * o =
    new msrNote (
      msrOpts, inputLineNumber, mxmldat, slurKind);
  assert(o!=0); 
  return o;
}

msrNote::msrNote (
  S_msrOptions& msrOpts,
  int                    inputLineNumber,
  musicXMLNoteData&      mxmldat,
  msrSlur::SlurKind      slurKind)
  :
    msrElement (msrOpts, inputLineNumber),
    fMusicXMLNoteData (mxmldat)
{
  fMsrOptions = msrOpts;
  fInputLineNumber = inputLineNumber;

  fNoteSlurKind = slurKind;

//  if (true || fMsrOptions->fDebug) {
  if (fMsrOptions->fDebug) {
    cerr << idtr <<
      "==> fMusicXMLNoteData contains:" << endl;
    cerr <<
      fMusicXMLNoteData;
  }
    
  // take rests into account
  if (fMusicXMLNoteData.fMusicXMLStepIsARest) {
    /*
    cerr <<
      "--> REST, fMusicXMLDuration/fMusicXMLDivisions = " <<
      fMusicXMLNoteData.fMusicXMLDuration << 
     "/" <<
     fMusicXMLNoteData.fMusicXMLDivisions << endl;
    */
    fMusicXMLDiatonicPitch = msrNote::kRest;
  }

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
    msrMusicXMLWarning (fInputLineNumber, s.str());
    }
  }

//  cerr << "=== xmlPart2MsrVisitor::visitStart ( S_step& elt ) " << fCurrentMusicXMLStep << endl;
// JMI

  switch (fMusicXMLNoteData.fMusicXMLStep) {
    case 'A': fMusicXMLDiatonicPitch = msrNote::kA; break;
    case 'B': fMusicXMLDiatonicPitch = msrNote::kB; break;
    case 'C': fMusicXMLDiatonicPitch = msrNote::kC; break;
    case 'D': fMusicXMLDiatonicPitch = msrNote::kD; break;
    case 'E': fMusicXMLDiatonicPitch = msrNote::kE; break;
    case 'F': fMusicXMLDiatonicPitch = msrNote::kF; break;
    case 'G': fMusicXMLDiatonicPitch = msrNote::kG; break;
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
  
  // flat or sharp,possibly double?
  msrNote::MusicXMLAlteration mxmlAlteration;

/*
  cerr <<
    "--> fMusicXMLNoteData.fMusicXMLAlteration = " <<
    fMusicXMLNoteData.fMusicXMLAlteration <<  endl;
*/
  
  switch (fMusicXMLNoteData.fMusicXMLAlteration) {
    case -2:
      mxmlAlteration = msrNote::kDoubleFlat;
      noteQuatertonesFromA-=3;
      if (noteQuatertonesFromA < 0)
        noteQuatertonesFromA += 24; // it is below A
      break;
    case -1:
      mxmlAlteration = msrNote::kFlat;
      noteQuatertonesFromA-=2;
      if (noteQuatertonesFromA < 0)
        noteQuatertonesFromA += 24; // it is below A
      break;
    case 0:
      mxmlAlteration = msrNote::kNatural;
      break;
    case 1:
      mxmlAlteration = msrNote::kSharp;
      noteQuatertonesFromA+=2;
      break;
    case 2:
      mxmlAlteration = msrNote::kDoubleSharp;
      noteQuatertonesFromA+=3;
      break;
    default:
      {
      stringstream s;
      s <<
        "MusicXML alteration " << fMusicXMLNoteData.fMusicXMLAlteration <<
        " is not between -2 and +2";
      msrMusicXMLError (
        fInputLineNumber, s.str());
      
      msrAssert ( // JMI
        fMusicXMLNoteData.fMusicXMLAlteration>=-2
          &&
        fMusicXMLNoteData.fMusicXMLAlteration<=+2,
        s.str());
      }
   } // switch

  fNoteMsrPitch = 
    computeNoteMsrPitch (noteQuatertonesFromA, mxmlAlteration);
  
  int divisionsPerWholeNote = fMusicXMLNoteData.fMusicXMLDivisions*4;
  
//  if (true || fMsrOptions->fDebug)
  if (fMsrOptions->fDebug)
    cerr << idtr <<
      "--> fMusicXMLNoteData.fMusicXMLDivisions = " <<
      fMusicXMLNoteData.fMusicXMLDivisions << ", " << 
      "divisionsPerWholeNote = " << divisionsPerWholeNote << endl;
    
  msrAssert(
    divisionsPerWholeNote > 0,
    "The MusicMXL divisions per quater note value should be positive");

  fNoteMsrDuration =
    msrDuration::create (
      fMsrOptions,
      fInputLineNumber,
      fMusicXMLNoteData.fMusicXMLDuration,
      divisionsPerWholeNote,
      fMusicXMLNoteData.fMusicXMLDotsNumber,
      fMusicXMLNoteData.fMusicXMLTupletMemberNoteType);
//  cerr << "fNoteMsrDuration = " << fNoteMsrDuration << endl;
    
  // diatonic note for relative code JMI
//  msrNote::MusicXMLDiatonicPitch diatonicNote =
//    msrNote::k_NoDiatonicPitch;
}

msrNote::~msrNote() {}

bool msrNote::getNoteIsARest ()
{
  return fMusicXMLNoteData.fMusicXMLStepIsARest;
}

void msrNote::setNoteBelongsToAChord () {
  fMusicXMLNoteData.fMusicXMLNoteBelongsToAChord = true;
}

//______________________________________________________________________________

msrNote::MsrPitch msrNote::computeNoteMsrPitch (
  int                         noteQuatertonesFromA,
  msrNote::MusicXMLAlteration alteration)
{
  // computing the msr pitch
  /*
  Alter values of -2 and 2 can be used for double-flat and double-sharp.
  Decimal values can be used for microtones (e.g., 0.5 for a quarter-tone sharp),
  but not all programs may convert this into MIDI pitch-bend data.

  For rests, a rest element is used instead of the pitch element.
  The whole rest in 3/4 that begins the voice part is represented as:
    <note>
      <rest/>
      <duration>72</duration>
    </note>
  
  Quarter tones may be added; the following is a series of Cs with increasing pitches:
    \relative c'' { ceseh ces ceh c cih cis cisih }
  */
  msrNote::MsrPitch msrPitch = msrNote::k_NoMsrPitch;
  
  switch (noteQuatertonesFromA) {
    case 0:
      msrPitch = msrNote::k_a;
      break;
    case 1:
      msrPitch =
        alteration == msrNote::kDoubleSharp
          ? msrNote::k_aih
          : msrNote::k_beseh;
      break;
    case 2:
      msrPitch =
        alteration == msrNote::kSharp
          ? msrNote::k_ais
          : msrNote::k_bes;
      break;
    case 3:
      msrPitch =
        alteration == msrNote::kDoubleSharp
          ? msrNote::k_aisih
          : msrNote::k_beh;
      break;
    case 4:
      msrPitch = msrNote::k_b;
      break;
    case 5:
      msrPitch =
        alteration == msrNote::kDoubleSharp
          ? msrNote::k_bih
          : msrNote::k_ceseh;
      break;
    case 6:
      msrPitch = msrNote::k_c;
      break;
    case 7:
      msrPitch =
        alteration == msrNote::kDoubleSharp
          ? msrNote::k_cih
          : msrNote::k_deseh;
      break;
    case 8:
      msrPitch =
        alteration == msrNote::kSharp
          ? msrNote::k_cis
          : msrNote::k_des;
      break;
    case 9:
      msrPitch =
        alteration == msrNote::kDoubleSharp
          ? msrNote::k_cisih
          : msrNote::k_deh;
      break;
    case 10:
      msrPitch = msrNote::k_d;
      break;
    case 11:
      msrPitch =
        alteration == msrNote::kDoubleSharp
          ? msrNote::k_dih
          : msrNote::k_eeseh;
      break;
    case 12:
      msrPitch =
        alteration == msrNote::kSharp
          ? msrNote::k_dis
          : msrNote::k_ees;
      break;
    case 13:
      msrPitch =
        alteration == msrNote::kDoubleSharp
          ? msrNote::k_disih
          : msrNote::k_eeh;
      break;
    case 14:
      msrPitch = msrNote::k_e;
      break;
    case 15:
      msrPitch =
        alteration == msrNote::kDoubleSharp
          ? msrNote::k_eih
          : msrNote::k_feseh;
      break;
    case 16:
      msrPitch = msrNote::k_f;
      break;
    case 17:
      msrPitch =
        alteration == msrNote::kDoubleSharp
          ? msrNote::k_fih
          : msrNote::k_geseh;
      break;
    case 18:
      msrPitch =
        alteration == msrNote::kSharp
          ? msrNote::k_fis
          : msrNote::k_ges;
      break;
    case 19:
      msrPitch =
        alteration == msrNote::kDoubleSharp
          ? msrNote::k_fisih
          : msrNote::k_geh;
      break;
    case 20:
      msrPitch = msrNote::k_g;
      break;
    case 21:
      msrPitch =
        alteration == msrNote::kDoubleSharp
          ? msrNote::k_gih
          : msrNote::k_aeseh;
      break;
    case 22:
      msrPitch =
        alteration == msrNote::kSharp
          ? msrNote::k_gis
          : msrNote::k_aes;
      break;
    case 23:
      msrPitch =
        alteration == msrNote::kDoubleSharp
          ? msrNote::k_gisih
          : msrNote::k_aeh;
      break;
  } // switch
  
  return msrPitch;
}

void msrNote::addArticulation (S_msrArticulation art)
{
  fNoteArticulations.push_back(art);
}

void msrNote::addDynamics (S_msrDynamics dyn) {
  fNoteDynamics.push_back(dyn);
}
void msrNote::addWedge (S_msrWedge wdg) {
  fNoteWedges.push_back(wdg);
}

S_msrDynamics msrNote::removeFirstDynamics () {
  S_msrDynamics dyn = fNoteDynamics.front();
  fNoteDynamics.pop_front();
  return dyn;
}
S_msrWedge msrNote::removeFirstWedge () {
  S_msrWedge wdg = fNoteWedges.front();
  fNoteWedges.pop_front();
  return wdg;
}

string msrNote::notePitchAsLilypondString ()
{
  stringstream s;
  
  /*
  cerr << "msrNote::notePitchAsLilypondString (), isRest = " <<
    fMusicXMLNoteData.fMusicXMLStepIsARest <<
    ", fMsrPitch = " << fMsrPitch << endl;
  */
  
  if (fMusicXMLNoteData.fMusicXMLStepIsARest)
    s << "r";

  else {
    //JMI assertMsr(fMsrPitch != k_NoMsrPitch, "fMsrPitch != k_NoMsrPitch");

    if (fMusicXMLNoteData.fMusicXMLStepIsUnpitched)
      s << "unpitched ";
      
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

    s << fMusicXMLNoteData.fMusicXMLOctave;
  }
  
  return s.str();
}

void msrNote::acceptIn (basevisitor* v) {
  if (fMsrOptions->fDebug)
    cerr << idtr <<
      "==> msrNote::acceptIn()" << endl;
      
  if (visitor<S_msrNote>*
    p =
      dynamic_cast<visitor<S_msrNote>*> (v)) {
        S_msrNote elem = this;
        
        if (fMsrOptions->fDebug)
          cerr << idtr <<
            "==> Launching msrNote::visitStart()" << endl;
        p->visitStart (elem);
  }
}

void msrNote::acceptOut (basevisitor* v) {
  if (fMsrOptions->fDebug)
    cerr << idtr <<
      "==> msrNote::acceptOut()" << endl;

  if (visitor<S_msrNote>*
    p =
      dynamic_cast<visitor<S_msrNote>*> (v)) {
        S_msrNote elem = this;
      
        if (fMsrOptions->fDebug)
          cerr << idtr <<
            "==> Launching msrNote::visitEnd()" << endl;
        p->visitEnd (elem);
  }
}


void msrNote::browseData (basevisitor* v)
{}

ostream& operator<< (ostream& os, const S_msrNote& elt)
{
  elt->print(os);
  return os;
}

void msrNote::printMusicXML (ostream& os)
{
  os << "<!-- msrNote??? -->" << endl;
}

void msrNote::printMSR (ostream& os)
{
  /*
  cerr <<
    "msrNote::printMSR (), fNoteBelongsToAChord = " << 
    fNoteBelongsToAChord << endl;
  */
  
  if (fMusicXMLNoteData.fMusicXMLNoteBelongsToAChord) {

    // do not print it, msrChord::printMSR () will do it
    os << notePitchAsLilypondString() << " (FOO) ";

  } else {

    if (fMusicXMLNoteData.fMusicXMLStepIsARest)
      os << "Rest";
    else
      os << "Note";
    os <<
      " " << notePitchAsLilypondString () <<
      ":" << fNoteMsrDuration;
    if (fMusicXMLNoteData.fMusicXMLNoteIsAGraceNote)
      os << " " << "grace";
    if (fMusicXMLNoteData.fMusicXMLNoteIsTied)
      os << " " << "tied";
    os << endl;
    
    // print the alterations if any
    if (fNoteArticulations.size()) {
      idtr++;
      list<S_msrArticulation>::const_iterator i;
      for (i=fNoteArticulations.begin(); i!=fNoteArticulations.end(); i++) {
        os << idtr << (*i);
      } // for
      idtr--;
    }
    
    // print the dynamics if any
    if (fNoteDynamics.size()) {
      idtr++;
      list<S_msrDynamics>::const_iterator i1;
      for (i1=fNoteDynamics.begin(); i1!=fNoteDynamics.end(); i1++) {
        os << idtr << (*i1);
      } // for
      idtr--;
    }
  
    // print the wedges if any
    if (fNoteWedges.size()) {
      idtr++;
      list<S_msrWedge>::const_iterator i2;
      for (i2=fNoteWedges.begin(); i2!=fNoteWedges.end(); i2++) {
        os << idtr << (*i2);
      } // for
      idtr--;
    }

    // print the slur if any
    if (fNoteSlurKind != msrSlur::k_NoSlur) {
      idtr++;
      switch (fNoteSlurKind) {
        case msrSlur::kStartSlur:
          os << idtr << "Slur start";
          break;
        case msrSlur::kContinueSlur:
          os << idtr << "Slur continue";
          break;
        case msrSlur::kStopSlur:
          os << idtr << "Slur stop";
          break;
        default:
          os << idtr << "Slur" << fNoteSlurKind << "???";
      } // switch
      os << endl;
      idtr--;
    }
  }
}

void msrNote::printScoreSummary (ostream& os)
{}

void msrNote::printLilyPondCode (ostream& os)
{
  // print the note name
  os << notePitchAsLilypondString ();
  
  if (! fMusicXMLNoteData.fMusicXMLNoteBelongsToAChord) {
    // print the note duration
    os << fNoteMsrDuration;
    
    // print the dynamics if any
    if (fNoteDynamics.size()) {
      list<S_msrDynamics>::const_iterator
        i1Begin = fNoteDynamics.begin(),
        i1End   = fNoteDynamics.end(),
        i1      = i1Begin;
      os << " ";
      for ( ; ; ) {
        os << (*i1);
        if (++i1 == i1End) break;
        os << " ";
      } // for
    }
  
    // print the wedges if any
    if (fNoteWedges.size()) {
      list<S_msrWedge>::const_iterator
        i2Begin = fNoteWedges.begin(),
        i2End   = fNoteWedges.end(),
        i2      = i2Begin;
      os << " ";
      for ( ; ; ) {
        os << (*i2);
        if (++i2 == i2End) break;
        os << " ";
      } // for
    }
  }
}

/*

string msrNote::octaveRepresentation (char octave)
{
  stringstream s;
  if (octave > 0) {
    int n = octave;
    while (n > 0) {
    s << "'";
    n--;
    }
  } else if (octave < 0) {
    int n = octave;
    while (n < 0) {
      s << ",";
      n++;
    }  
  }
  return s.str();
}
  */

//______________________________________________________________________________
S_msrSequentialMusic msrSequentialMusic::create (
  S_msrOptions& msrOpts, 
  int                    inputLineNumber,
  ElementsSeparator      elementsSeparator)
{
  msrSequentialMusic* o =
    new msrSequentialMusic (
      msrOpts, inputLineNumber, elementsSeparator);
  assert(o!=0);
  return o;
}

msrSequentialMusic::msrSequentialMusic (
  S_msrOptions& msrOpts, 
  int                    inputLineNumber,
  ElementsSeparator      elementsSeparator)
    : msrElement (msrOpts, inputLineNumber)
{
  fElementsSeparator=elementsSeparator;
}
msrSequentialMusic::~msrSequentialMusic() {}

void msrSequentialMusic::removeElementFromSequentialMusic (
  S_msrElement elem)
{
  for (
    list<S_msrElement>::iterator i = fSequentialMusicElements.begin();
    i != fSequentialMusicElements.end();
    i++) {
    if ((*i) == elem) {
      fSequentialMusicElements.erase (i);
      break;
    }
  } // for
}

void msrSequentialMusic::acceptIn (basevisitor* v) {
  if (fMsrOptions->fDebug)
    cerr << idtr <<
      "==> msrSequentialMusic::acceptIn()" << endl;
      
  if (visitor<S_msrSequentialMusic>*
    p =
      dynamic_cast<visitor<S_msrSequentialMusic>*> (v)) {
        S_msrSequentialMusic elem = this;
        
        if (fMsrOptions->fDebug)
          cerr << idtr <<
            "==> Launching msrSequentialMusic::visitStart()" << endl;
        p->visitStart (elem);
  }
}

void msrSequentialMusic::acceptOut (basevisitor* v) {
  if (fMsrOptions->fDebug)
    cerr << idtr <<
      "==> msrSequentialMusic::acceptOut()" << endl;

  if (visitor<S_msrSequentialMusic>*
    p =
      dynamic_cast<visitor<S_msrSequentialMusic>*> (v)) {
        S_msrSequentialMusic elem = this;
      
        if (fMsrOptions->fDebug)
          cerr << idtr <<
            "==> Launching msrSequentialMusic::visitEnd()" << endl;
        p->visitEnd (elem);
  }
}

void msrSequentialMusic::browseData (basevisitor* v)
{
  if (fMsrOptions->fDebug)
    cerr << idtr <<
      "==> msrSequentialMusic::browseData()" << endl;
  
  idtr++;

  for (
    list<S_msrElement>::iterator i = fSequentialMusicElements.begin();
    i != fSequentialMusicElements.end();
    i++) {
    // create the element browser
    msrBrowser<msrElement> browser (v);
  
    // browse the element with the visitor
    browser.browse (*(*i));
  } // for

  idtr--;

  if (fMsrOptions->fDebug)
    cerr << idtr <<
      "<== msrSequentialMusic::browseData()" << endl;
}

ostream& operator<< (ostream& os, const S_msrSequentialMusic& elt)
{
  elt->print(os);
  return os;
}

void msrSequentialMusic::printMSR (ostream& os)
{  
  os << "SequentialMusic";
  
  if (! fSequentialMusicElements.size ())
    os << " (No actual notes)";
  os << endl;

  if (fSequentialMusicElements.size ()) {  
    idtr++;
  
    list<S_msrElement>::const_iterator
      iBegin = fSequentialMusicElements.begin(),
      iEnd   = fSequentialMusicElements.end(),
      i      = iBegin;
    for ( ; ; ) {
      os << idtr << (*i);
      if (++i == iEnd) break;
      if (fElementsSeparator == kEndOfLine) os << endl;
    } // for
    
    idtr--;
  }
}

void msrSequentialMusic::printScoreSummary (ostream& os)
{  
  int sequenceElementsSize = fSequentialMusicElements.size();

  os <<
    "SequentialMusic" <<
    " contains " << sequenceElementsSize;
  if (sequenceElementsSize == 1)
    os << " element";
  else
    os << " elements";
  
  if (! sequenceElementsSize)
    os << " (No actual notes)";
  os << endl;

/* JMI
  if (sequenceElementsSize) {  
    idtr++;
  
    list<S_msrElement>::const_iterator
      iBegin = fSequentialMusicElements.begin(),
      iEnd   = fSequentialMusicElements.end(),
      i      = iBegin;
    for ( ; ; ) {
      os << idtr << (*i);
      if (++i == iEnd) break;
      os << endl;
    } // for
    
    idtr--;
  }
*/
}

void msrSequentialMusic::printMusicXML (ostream& os)
{
  os << "<!-- msrSequentialMusic??? -->" << endl;
}

void msrSequentialMusic::printLilyPondCode (ostream& os)
{
  if (fSequentialMusicElements.size()) {
    list<S_msrElement>::const_iterator
      iBegin = fSequentialMusicElements.begin(),
      iEnd   = fSequentialMusicElements.end(),
      i      = iBegin;
    for ( ; ; ) {
      os << idtr << (*i);
      if (++i == iEnd) break;
      if (fElementsSeparator == kEndOfLine) os << endl;
    } // for
  }
}

//______________________________________________________________________________
S_msrParallelMusic msrParallelMusic::create (
  S_msrOptions& msrOpts, 
  int                    inputLineNumber,
  ElementsSeparator      elementsSeparator)
{
  msrParallelMusic* o =
    new msrParallelMusic (
      msrOpts, inputLineNumber, elementsSeparator);
  assert(o!=0);
  return o;
}

msrParallelMusic::msrParallelMusic (
  S_msrOptions& msrOpts, 
  int                    inputLineNumber,
  ElementsSeparator      elementsSeparator)
    : msrElement (msrOpts, inputLineNumber)
{
  fElementsSeparator=elementsSeparator;
}
msrParallelMusic::~msrParallelMusic() {}

void msrParallelMusic::acceptIn (basevisitor* v) {
  if (fMsrOptions->fDebug)
    cerr << idtr <<
      "==> msrParallelMusic::acceptIn()" << endl;
      
  if (visitor<S_msrParallelMusic>*
    p =
      dynamic_cast<visitor<S_msrParallelMusic>*> (v)) {
        S_msrParallelMusic elem = this;
        
        if (fMsrOptions->fDebug)
          cerr << idtr <<
            "==> Launching msrParallelMusic::visitStart()" << endl;
        p->visitStart (elem);
  }
}

void msrParallelMusic::acceptOut (basevisitor* v) {
  if (fMsrOptions->fDebug)
    cerr << idtr <<
      "==> msrParallelMusic::acceptOut()" << endl;

  if (visitor<S_msrParallelMusic>*
    p =
      dynamic_cast<visitor<S_msrParallelMusic>*> (v)) {
        S_msrParallelMusic elem = this;
      
        if (fMsrOptions->fDebug)
          cerr << idtr <<
            "==> Launching msrParallelMusic::visitEnd()" << endl;
        p->visitEnd (elem);
  }
}


void msrParallelMusic::browseData (basevisitor* v)
{}

ostream& operator<< (ostream& os, const S_msrParallelMusic& elt)
{
  elt->print(os);
  return os;
}

void msrParallelMusic::printMusicXML (ostream& os)
{
  os << "<!-- msrParallelMusic??? -->" << endl;
}

void msrParallelMusic::printMSR (ostream& os)
{
  os << "ParallelMusic" << endl;
  
  idtr++;
  
  int size = fParallelMusicElements.size();
  
  for (int i = 0; i < size; i++ ) {
    os << idtr << fParallelMusicElements[i];
  } // for
  
  idtr--;
}

void msrParallelMusic::printScoreSummary (ostream& os)
{}

void msrParallelMusic::printLilyPondCode (ostream& os)
{      
  os << idtr << "<<" << endl;
  
  idtr++;
  
  int size = fParallelMusicElements.size();
  
  for (int i = 0; i < size; i++ ) {
    os << idtr << fParallelMusicElements[i];
  } // for
  
  idtr--;
  
  os << idtr << ">>" << endl;
}

//______________________________________________________________________________
S_msrChord msrChord::create (
  S_msrOptions& msrOpts, 
  int                    inputLineNumber,
  S_msrDuration          chordDuration)
{
  msrChord* o =
    new msrChord (
      msrOpts, inputLineNumber, chordDuration);
  assert(o!=0);
  return o;
}

msrChord::msrChord (
  S_msrOptions& msrOpts, 
  int                    inputLineNumber,
  S_msrDuration          chordDuration)
    : msrElement (msrOpts, inputLineNumber)
{
  fChordDuration = chordDuration;
}
msrChord::~msrChord() {}

void msrChord::acceptIn (basevisitor* v) {
  if (fMsrOptions->fDebug)
    cerr << idtr <<
      "==> msrChord::acceptIn()" << endl;
      
  if (visitor<S_msrChord>*
    p =
      dynamic_cast<visitor<S_msrChord>*> (v)) {
        S_msrChord elem = this;
        
        if (fMsrOptions->fDebug)
          cerr << idtr <<
            "==> Launching msrChord::visitStart()" << endl;
        p->visitStart (elem);
  }
}

void msrChord::acceptOut (basevisitor* v) {
  if (fMsrOptions->fDebug)
    cerr << idtr <<
      "==> msrChord::acceptOut()" << endl;

  if (visitor<S_msrChord>*
    p =
      dynamic_cast<visitor<S_msrChord>*> (v)) {
        S_msrChord elem = this;
      
        if (fMsrOptions->fDebug)
          cerr << idtr <<
            "==> Launching msrChord::visitEnd()" << endl;
        p->visitEnd (elem);
  }
}


void msrChord::browseData (basevisitor* v)
{}

ostream& operator<< (ostream& os, const S_msrChord& chrd)
{
  chrd->print(os);
  return os;
}

void msrChord::printMusicXML (ostream& os)
{
  os << "<!-- msrChord??? -->" << endl;
}

void msrChord::printMSR (ostream& os)
{
  os << "Chord" << " " << "<";
  if (fChordNotes.size()) {
    vector<S_msrNote>::const_iterator
      iBegin = fChordNotes.begin(),
      iEnd   = fChordNotes.end(),
      i      = iBegin;
    for ( ; ; ) {
      os << (*i)->notePitchAsLilypondString();
      if (++i == iEnd) break;
      os << " ";
    } // for
  }
  os << ">";
  
  // print the chord duration
  os << fChordDuration << endl; 

  // print the articulations if any
  if (fChordArticulations.size()) {
    idtr++;
    list<S_msrArticulation>::const_iterator i;
    for (i=fChordArticulations.begin(); i!=fChordArticulations.end(); i++) {
      os << idtr << (*i);
    } // for
    idtr--;
  }

  // print the dynamics if any
  if (fChordDynamics.size()) {
    idtr++;
    list<S_msrDynamics>::const_iterator i;
    for (i=fChordDynamics.begin(); i!=fChordDynamics.end(); i++) {
      os << idtr << (*i);
    } // for
    idtr--;
  }

  // print the wedges if any
  if (fChordWedges.size()) {
    idtr++;
    list<S_msrWedge>::const_iterator i;
    for (i=fChordWedges.begin(); i!=fChordWedges.end(); i++) {
      os << idtr << (*i);
    } // for
    idtr--;
  }
}

void msrChord::printScoreSummary (ostream& os)
{}

void msrChord::printLilyPondCode (ostream& os)
{
  os << "<";
  if (fChordNotes.size()) {
    vector<S_msrNote>::const_iterator
      iBegin = fChordNotes.begin(),
      iEnd   = fChordNotes.end(),
      i      = iBegin;
    for ( ; ; ) {
      os << (*i);
      if (++i == iEnd) break;
      os << " ";
    } // for
  }
  os << ">";
  
  // print the chord duration
  os << fChordDuration;

  // print the dynamics if any
  if (fChordDynamics.size()) {
    list<S_msrDynamics>::const_iterator i1;
    for (i1=fChordDynamics.begin(); i1!=fChordDynamics.end(); i1++) {
      os << " " << (*i1);
    } // for
  }

  // print the wedges if any
  if (fChordWedges.size()) {
    list<S_msrWedge>::const_iterator i2;
    for (i2=fChordWedges.begin(); i2!=fChordWedges.end(); i2++) {
      os << " " << (*i2);
    } // for
  }
}

//______________________________________________________________________________
S_msrBarLine msrBarLine::create (
  S_msrOptions& msrOpts, 
  int                    inputLineNumber,
  int                    nextBarNumber)
{
  msrBarLine* o =
    new msrBarLine (
      msrOpts, inputLineNumber, nextBarNumber);
  assert(o!=0);
  return o;
}

msrBarLine::msrBarLine (
  S_msrOptions& msrOpts, 
  int                    inputLineNumber,
  int                    nextBarNumber)
    : msrElement (msrOpts, inputLineNumber)
{
  fNextBarNumber=nextBarNumber; 
}
msrBarLine::~msrBarLine() {}

void msrBarLine::acceptIn (basevisitor* v) {
  if (fMsrOptions->fDebug)
    cerr << idtr <<
      "==> msrBarLine::acceptIn()" << endl;
      
  if (visitor<S_msrBarLine>*
    p =
      dynamic_cast<visitor<S_msrBarLine>*> (v)) {
        S_msrBarLine elem = this;
        
        if (fMsrOptions->fDebug)
          cerr << idtr <<
            "==> Launching msrBarLine::visitStart()" << endl;
        p->visitStart (elem);
  }
}

void msrBarLine::acceptOut (basevisitor* v) {
  if (fMsrOptions->fDebug)
    cerr << idtr <<
      "==> msrBarLine::acceptOut()" << endl;

  if (visitor<S_msrBarLine>*
    p =
      dynamic_cast<visitor<S_msrBarLine>*> (v)) {
        S_msrBarLine elem = this;
      
        if (fMsrOptions->fDebug)
          cerr << idtr <<
            "==> Launching msrBarLine::visitEnd()" << endl;
        p->visitEnd (elem);
  }
}


void msrBarLine::browseData (basevisitor* v)
{}

ostream& operator<< (ostream& os, const S_msrBarLine& elt)
{
  elt->print(os);
  return os;
}

void msrBarLine::printMusicXML (ostream& os)
{
  os << "<!-- msrBarLine??? -->" << endl;
}

void msrBarLine::printMSR (ostream& os)
{
  os << "BarLine" << " " << fNextBarNumber << endl;
}

void msrBarLine::printScoreSummary (ostream& os)
{}

void msrBarLine::printLilyPondCode (ostream& os)
{
  os << "| % " << fNextBarNumber << endl;
}

//______________________________________________________________________________
S_msrComment msrComment::create (
  S_msrOptions& msrOpts, 
  int                    inputLineNumber,
  string                 contents,
  GapKind                gapKind)
{
  msrComment* o =
    new msrComment (
      msrOpts, inputLineNumber, contents, gapKind);
  assert(o!=0);
  return o;
}

msrComment::msrComment (
  S_msrOptions& msrOpts, 
  int                    inputLineNumber,
  string                 contents,
  GapKind                gapKind)
    : msrElement (msrOpts, inputLineNumber)
{
  fContents=contents;
  fGapKind=gapKind;
}
msrComment::~msrComment() {}

void msrComment::acceptIn (basevisitor* v) {
  if (fMsrOptions->fDebug)
    cerr << idtr <<
      "==> msrComment::acceptIn()" << endl;
      
  if (visitor<S_msrComment>*
    p =
      dynamic_cast<visitor<S_msrComment>*> (v)) {
        S_msrComment elem = this;
        
        if (fMsrOptions->fDebug)
          cerr << idtr <<
            "==> Launching msrComment::visitStart()" << endl;
        p->visitStart (elem);
  }
}

void msrComment::acceptOut (basevisitor* v) {
  if (fMsrOptions->fDebug)
    cerr << idtr <<
      "==> msrComment::acceptOut()" << endl;

  if (visitor<S_msrComment>*
    p =
      dynamic_cast<visitor<S_msrComment>*> (v)) {
        S_msrComment elem = this;
      
        if (fMsrOptions->fDebug)
          cerr << idtr <<
            "==> Launching msrComment::visitEnd()" << endl;
        p->visitEnd (elem);
  }
}


void msrComment::browseData (basevisitor* v)
{}

ostream& operator<< (ostream& os, const S_msrComment& elt)
{
  elt->print(os);
  return os;
}

void msrComment::printMusicXML (ostream& os)
{
  os << "<!-- msrComment??? -->" << endl;
}

void msrComment::printMSR (ostream& os)
{
  os << "Comment" << endl;
  idtr++;
  os << idtr << "% " << fContents << endl;
  if (fGapKind == kGapAfterwards) os << idtr << endl;
  idtr--;
}

void msrComment::printScoreSummary (ostream& os)
{}

void msrComment::printLilyPondCode (ostream& os)
{
  os << "% " << fContents;
  if (fGapKind == kGapAfterwards) os << endl;
}

//______________________________________________________________________________
S_msrBreak msrBreak::create (
  S_msrOptions& msrOpts, 
  int                    inputLineNumber,
  int                    nextBarNumber)
{
  msrBreak* o =
    new msrBreak (
      msrOpts, inputLineNumber, nextBarNumber);
  assert(o!=0);
  return o;
}

msrBreak::msrBreak (
  S_msrOptions& msrOpts, 
  int                    inputLineNumber,
  int                    nextBarNumber)
    : msrElement (msrOpts, inputLineNumber)
{
  fNextBarNumber=nextBarNumber; 
}
msrBreak::~msrBreak() {}

void msrBreak::acceptIn (basevisitor* v) {
  if (fMsrOptions->fDebug)
    cerr << idtr <<
      "==> msrBreak::acceptIn()" << endl;
      
  if (visitor<S_msrBreak>*
    p =
      dynamic_cast<visitor<S_msrBreak>*> (v)) {
        S_msrBreak elem = this;
        
        if (fMsrOptions->fDebug)
          cerr << idtr <<
            "==> Launching msrBreak::visitStart()" << endl;
        p->visitStart (elem);
  }
}

void msrBreak::acceptOut (basevisitor* v) {
  if (fMsrOptions->fDebug)
    cerr << idtr <<
      "==> msrBreak::acceptOut()" << endl;

  if (visitor<S_msrBreak>*
    p =
      dynamic_cast<visitor<S_msrBreak>*> (v)) {
        S_msrBreak elem = this;
      
        if (fMsrOptions->fDebug)
          cerr << idtr <<
            "==> Launching msrBreak::visitEnd()" << endl;
        p->visitEnd (elem);
  }
}


void msrBreak::browseData (basevisitor* v)
{}

ostream& operator<< (ostream& os, const S_msrBreak& elt)
{
  elt->print(os);
  return os;
}

void msrBreak::printMusicXML (ostream& os)
{
  os << "<!-- msrBreak??? -->" << endl;
}

void msrBreak::printMSR (ostream& os)
{
  os <<
    "Break" << " " << fNextBarNumber << endl <<
    endl;
}

void msrBreak::printScoreSummary (ostream& os)
{}

void msrBreak::printLilyPondCode (ostream& os)
{
  os <<
    "\\myBreak | % " << fNextBarNumber << endl <<
    endl;
}

//______________________________________________________________________________
S_msrBarNumberCheck msrBarNumberCheck::create (
  S_msrOptions& msrOpts, 
  int                    inputLineNumber,
  int                    nextBarNumber)
{
  msrBarNumberCheck* o =
    new msrBarNumberCheck (
      msrOpts, inputLineNumber, nextBarNumber);
  assert(o!=0);
  return o;
}

msrBarNumberCheck::msrBarNumberCheck (
  S_msrOptions& msrOpts, 
  int                    inputLineNumber,
  int                    nextBarNumber)
    : msrElement (msrOpts, inputLineNumber)
{
  fNextBarNumber=nextBarNumber; 
}
msrBarNumberCheck::~msrBarNumberCheck() {}

void msrBarNumberCheck::acceptIn (basevisitor* v) {
  if (fMsrOptions->fDebug)
    cerr << idtr <<
      "==> msrBarNumberCheck::acceptIn()" << endl;
      
  if (visitor<S_msrBarNumberCheck>*
    p =
      dynamic_cast<visitor<S_msrBarNumberCheck>*> (v)) {
        S_msrBarNumberCheck elem = this;
        
        if (fMsrOptions->fDebug)
          cerr << idtr <<
            "==> Launching msrBarNumberCheck::visitStart()" << endl;
        p->visitStart (elem);
  }
}

void msrBarNumberCheck::acceptOut (basevisitor* v) {
  if (fMsrOptions->fDebug)
    cerr << idtr <<
      "==> msrBarNumberCheck::acceptOut()" << endl;

  if (visitor<S_msrBarNumberCheck>*
    p =
      dynamic_cast<visitor<S_msrBarNumberCheck>*> (v)) {
        S_msrBarNumberCheck elem = this;
      
        if (fMsrOptions->fDebug)
          cerr << idtr <<
            "==> Launching msrBarNumberCheck::visitEnd()" << endl;
        p->visitEnd (elem);
  }
}


void msrBarNumberCheck::browseData (basevisitor* v)
{}

ostream& operator<< (ostream& os, const S_msrBarNumberCheck& elt)
{
  elt->print(os);
  return os;
}

void msrBarNumberCheck::printMusicXML (ostream& os)
{
  os << "<!-- msrBarNumberCheck??? -->" << endl;
}

void msrBarNumberCheck::printMSR (ostream& os)
{
  os << "BarNumberCheck" << " " << fNextBarNumber << endl;
}

void msrBarNumberCheck::printScoreSummary (ostream& os)
{}

void msrBarNumberCheck::printLilyPondCode (ostream& os)
{
  os << "\\barNumberCheck #" << fNextBarNumber << endl;
}

//______________________________________________________________________________
S_msrTuplet msrTuplet::create (
  S_msrOptions& msrOpts,
  int                    inputLineNumber)
{
  msrTuplet* o =
    new msrTuplet (
      msrOpts, inputLineNumber);
  assert(o!=0);
  return o;
}

msrTuplet::msrTuplet (
  S_msrOptions& msrOpts,
  int                    inputLineNumber)
    : msrElement (msrOpts, inputLineNumber)
{
  fTupletNumber = k_NoTuplet;
  
  fActualNotes = -1;
  fNormalNotes = -1;
}
msrTuplet::~msrTuplet() {}

void msrTuplet::updateTuplet (int number, int actualNotes, int normalNotes)
{
  fTupletNumber = number;
  
  fActualNotes = actualNotes;
  fNormalNotes = normalNotes;  
}

void msrTuplet::acceptIn (basevisitor* v) {
  if (fMsrOptions->fDebug)
    cerr << idtr <<
      "==> msrTuplet::acceptIn()" << endl;
      
  if (visitor<S_msrTuplet>*
    p =
      dynamic_cast<visitor<S_msrTuplet>*> (v)) {
        S_msrTuplet elem = this;
        
        if (fMsrOptions->fDebug)
          cerr << idtr <<
            "==> Launching msrTuplet::visitStart()" << endl;
        p->visitStart (elem);
  }
}

void msrTuplet::acceptOut (basevisitor* v) {
  if (fMsrOptions->fDebug)
    cerr << idtr <<
      "==> msrTuplet::acceptOut()" << endl;

  if (visitor<S_msrTuplet>*
    p =
      dynamic_cast<visitor<S_msrTuplet>*> (v)) {
        S_msrTuplet elem = this;
      
        if (fMsrOptions->fDebug)
          cerr << idtr <<
            "==> Launching msrTuplet::visitEnd()" << endl;
        p->visitEnd (elem);
  }
}


void msrTuplet::browseData (basevisitor* v)
{}

ostream& operator<< (ostream& os, const S_msrTuplet& elt)
{
  elt->print(os);
  return os;
}

void msrTuplet::printMusicXML (ostream& os)
{
  os << "<!-- msrTuplet??? -->" << endl;
}

void msrTuplet::printMSR (ostream& os)
{
  os <<
    "Tuplet " << fActualNotes << "/" << fNormalNotes << endl;
  idtr++;
  vector<S_msrElement>::const_iterator i;
  for (i=fTupletContents.begin(); i!=fTupletContents.end(); i++) {
    os << idtr << (*i);
  } // for
  idtr--;
}

void msrTuplet::printScoreSummary (ostream& os)
{}

void msrTuplet::printLilyPondCode (ostream& os)
{
  os << "\\tuplet " << fActualNotes << "/" << fNormalNotes << " { ";

  if (fTupletContents.size()) {
    idtr++;
    
    vector<S_msrElement>::const_iterator
      iBegin = fTupletContents.begin(),
      iEnd   = fTupletContents.end(),
      i      = iBegin;
      for ( ; ; ) {
        os << (*i);
        if (++i == iEnd) break;
        os << " ";
      } // for
    
    idtr--;
  }
  
  os << " }" << endl;
}

//______________________________________________________________________________
S_msrBeam msrBeam::create (
  S_msrOptions& msrOpts, 
  int                    inputLineNumber,
  int                    number,
  BeamKind               beamKind)
{
  msrBeam* o =
    new msrBeam (
      msrOpts, inputLineNumber, number, beamKind);
  assert(o!=0);
  return o;
}

msrBeam::msrBeam (
  S_msrOptions& msrOpts, 
  int                    inputLineNumber,
  int                    number,
  BeamKind               beamKind)
    : msrElement (msrOpts, inputLineNumber)
{
  fBeamNumber = number;
  fBeamKind   = beamKind; 
}
msrBeam::~msrBeam() {}

void msrBeam::acceptIn (basevisitor* v) {
  if (fMsrOptions->fDebug)
    cerr << idtr <<
      "==> msrBeam::acceptIn()" << endl;
      
  if (visitor<S_msrBeam>*
    p =
      dynamic_cast<visitor<S_msrBeam>*> (v)) {
        S_msrBeam elem = this;
        
        if (fMsrOptions->fDebug)
          cerr << idtr <<
            "==> Launching msrBeam::visitStart()" << endl;
        p->visitStart (elem);
  }
}

void msrBeam::acceptOut (basevisitor* v) {
  if (fMsrOptions->fDebug)
    cerr << idtr <<
      "==> msrBeam::acceptOut()" << endl;

  if (visitor<S_msrBeam>*
    p =
      dynamic_cast<visitor<S_msrBeam>*> (v)) {
        S_msrBeam elem = this;
      
        if (fMsrOptions->fDebug)
          cerr << idtr <<
            "==> Launching msrBeam::visitEnd()" << endl;
        p->visitEnd (elem);
  }
}


void msrBeam::browseData (basevisitor* v)
{}

ostream& operator<< (ostream& os, const S_msrBeam& dyn)
{
  dyn->print(os);
  return os;
}

void msrBeam::printMusicXML (ostream& os)
{
  os << "<!-- msrBeam??? -->" << endl;
}

void msrBeam::printMSR (ostream& os)
{
  os << "Beam???" << endl;
}

void msrBeam::printScoreSummary (ostream& os)
{}

void msrBeam::printLilyPondCode (ostream& os)
{
  switch (fBeamKind) {
    case kBeginBeam:
      os << "[ ";
      break;
    case kContinueBeam:
      break;
    case kEndBeam:
      os << "] ";
      break;
    default:
      os << "Beam " << fBeamKind << "???";
  } // switch
}

//______________________________________________________________________________
S_msrPaper msrPaper::create (
  S_msrOptions& msrOpts, 
  int                    inputLineNumber)
{
  msrPaper* o =
    new msrPaper (
      msrOpts, inputLineNumber);
  assert(o!=0);
  return o;
}

msrPaper::msrPaper (
  S_msrOptions& msrOpts, 
  int                    inputLineNumber)
    : msrElement (msrOpts, inputLineNumber)
{
  fPaperWidth = -1.0;
  fPaperHeight = -1.0;
  fTopMargin = -1.0;
  fBottomMargin = -1.0;
  fLeftMargin = -1.0;
  fRightMargin = -1.0;
    
  fBetweenSystemSpace = -1.0;
  fPageTopSpace = -1.0;
}
msrPaper::~msrPaper() {}

void msrPaper::acceptIn (basevisitor* v) {
  if (fMsrOptions->fDebug)
    cerr << idtr <<
      "==> msrPaper::acceptIn()" << endl;
      
  if (visitor<S_msrPaper>*
    p =
      dynamic_cast<visitor<S_msrPaper>*> (v)) {
        S_msrPaper elem = this;
        
        if (fMsrOptions->fDebug)
          cerr << idtr <<
            "==> Launching msrPaper::visitStart()" << endl;
        p->visitStart (elem);
  }
}

void msrPaper::acceptOut (basevisitor* v) {
  if (fMsrOptions->fDebug)
    cerr << idtr <<
      "==> msrPaper::acceptOut()" << endl;

  if (visitor<S_msrPaper>*
    p =
      dynamic_cast<visitor<S_msrPaper>*> (v)) {
        S_msrPaper elem = this;
      
        if (fMsrOptions->fDebug)
          cerr << idtr <<
            "==> Launching msrPaper::visitEnd()" << endl;
        p->visitEnd (elem);
  }
}


void msrPaper::browseData (basevisitor* v)
{}

ostream& operator<< (ostream& os, const S_msrPaper& pap) {
  pap->print(os);
  return os;
}

void msrPaper::printMusicXML (ostream& os)
{
  os << "<!-- msrPaper??? -->" << endl;
}

void msrPaper::printMSR (ostream& os) {
  os << "Paper" << endl;

  idtr++;
  
  if (fPaperWidth > 0) {
    os << 
      idtr << "paper-width = " << setprecision(4) << fPaperWidth << "\\cm" << endl;
  }
  if (fPaperHeight > 0) {
    os <<
      idtr << "paper-height = " << setprecision(4) << fPaperHeight << "\\cm" << endl;
  }
  if (fTopMargin > 0) {
    os <<
      idtr << "top-margin = " << setprecision(4) << fTopMargin << "\\cm" << endl;
  }
  if (fBottomMargin > 0) {
    os <<
      idtr << "bottom-margin = " << setprecision(4) << fBottomMargin << "\\cm" << endl;
  }
  if (fLeftMargin > 0) {
    os <<
      idtr << "left-margin = " << setprecision(4) << fLeftMargin << "\\cm" << endl;
  }

  if (fRightMargin > 0) {
    os << idtr << "right-margin = " << setprecision(4) << fRightMargin << "\\cm" << endl;
  }

/*
  if (fBetweenSystemSpace > 0) {
    os << idtr << "between-system-space = " << setprecision(4) << fBetweenSystemSpace << "\\cm" << endl;
  }

  if (fPageTopSpace > 0) {
    os << idtr << "page-top-space = " << setprecision(4) << fPageTopSpace << "\\cm" << endl;
  }
*/

  idtr--;
}

void msrPaper::printScoreSummary (ostream& os) {
  os << "Paper" << endl;

  idtr++;
  
  if (fPaperWidth > 0) {
    os << 
      idtr << "paper-width = " << setprecision(4) << fPaperWidth << "\\cm" << endl;
  }
  if (fPaperHeight > 0) {
    os <<
      idtr << "paper-height = " << setprecision(4) << fPaperHeight << "\\cm" << endl;
  }
  if (fTopMargin > 0) {
    os <<
      idtr << "top-margin = " << setprecision(4) << fTopMargin << "\\cm" << endl;
  }
  if (fBottomMargin > 0) {
    os <<
      idtr << "bottom-margin = " << setprecision(4) << fBottomMargin << "\\cm" << endl;
  }
  if (fLeftMargin > 0) {
    os <<
      idtr << "left-margin = " << setprecision(4) << fLeftMargin << "\\cm" << endl;
  }

  if (fRightMargin > 0) {
    os << idtr << "right-margin = " << setprecision(4) << fRightMargin << "\\cm" << endl;
  }

/*
  if (fBetweenSystemSpace > 0) {
    os << idtr << "between-system-space = " << setprecision(4) << fBetweenSystemSpace << "\\cm" << endl;
  }

  if (fPageTopSpace > 0) {
    os << idtr << "page-top-space = " << setprecision(4) << fPageTopSpace << "\\cm" << endl;
  }
*/

  idtr--;
}

void msrPaper::printLilyPondCode (ostream& os)
{  
  os << "\\paper {" << endl;

  idtr++;
  
  if (fPaperWidth > 0) {
    os << 
      idtr << "paper-width = " << setprecision(4) << fPaperWidth << "\\cm" << endl;
  }
  if (fPaperHeight > 0) {
    os <<
      idtr << "paper-height = " << setprecision(4) << fPaperHeight << "\\cm" << endl;
  }
  if (fTopMargin > 0) {
    os <<
      idtr << "top-margin = " << setprecision(4) << fTopMargin << "\\cm" << endl;
  }
  if (fBottomMargin > 0) {
    os <<
      idtr << "bottom-margin = " << setprecision(4) << fBottomMargin << "\\cm" << endl;
  }
  if (fLeftMargin > 0) {
    os <<
      idtr << "left-margin = " << setprecision(4) << fLeftMargin << "\\cm" << endl;
  }

  if (fRightMargin > 0) {
    os << idtr << "right-margin = " << setprecision(4) << fRightMargin << "\\cm" << endl;
  }

/*
  if (fBetweenSystemSpace > 0) {
    os << idtr << "between-system-space = " << setprecision(4) << fBetweenSystemSpace << "\\cm" << endl;
  }

  if (fPageTopSpace > 0) {
    os << idtr << "page-top-space = " << setprecision(4) << fPageTopSpace << "\\cm" << endl;
  }
*/

  idtr--;
  
  os << "}" << endl;
}

//______________________________________________________________________________
S_msrHeader msrHeader::create (
  S_msrOptions& msrOpts, 
  int                    inputLineNumber)
{
  msrHeader* o =
    new msrHeader (
      msrOpts, inputLineNumber);
  assert(o!=0);
  return o;
}

msrHeader::msrHeader (
  S_msrOptions& msrOpts, 
  int                    inputLineNumber)
    : msrElement (msrOpts, inputLineNumber)
{}
msrHeader::~msrHeader() {}

void msrHeader::setWorkNumber (
  int    inputLineNumber,
  string val)
  {
  fWorkNumber =
    msrVarValAssoc::create (
      fMsrOptions,
      inputLineNumber,
      "work-number", val,
      msrVarValAssoc::kEqualSign,
      msrVarValAssoc::kQuotesAroundValue,
      msrVarValAssoc::kUncommented);
  }

void msrHeader::setWorkTitle (
  int    inputLineNumber,
  string val)
  {
  fWorkTitle =
    msrVarValAssoc::create (
      fMsrOptions,
      inputLineNumber,
      "work-title", val,
      msrVarValAssoc::kEqualSign,
      msrVarValAssoc::kQuotesAroundValue,
      msrVarValAssoc::kUncommented);
  }

void msrHeader::setMovementNumber (
  int    inputLineNumber,
  string val)
  {
  fMovementNumber =
    msrVarValAssoc::create (
      fMsrOptions,
      inputLineNumber,
      "movement-number", val,
      msrVarValAssoc::kEqualSign,
      msrVarValAssoc::kQuotesAroundValue,
      msrVarValAssoc::kUncommented);
  }

void msrHeader::setMovementTitle (
  int    inputLineNumber,
  string val)
{
  fMovementTitle =
    msrVarValAssoc::create (
      fMsrOptions,
      inputLineNumber,
      "movement-title", val,
      msrVarValAssoc::kEqualSign,
      msrVarValAssoc::kQuotesAroundValue,
      msrVarValAssoc::kUncommented);
}

void msrHeader::addCreator (
  int    inputLineNumber,
  string type,
  string val)
{
  fCreators.push_back(
    msrVarValAssoc::create (
      fMsrOptions,
      inputLineNumber,
      type, val,
      msrVarValAssoc::kEqualSign,
      msrVarValAssoc::kQuotesAroundValue,
      msrVarValAssoc::kUncommented)
  );
}

void msrHeader::setRights (
  int    inputLineNumber,
  string val)
  {
  fRights =
    msrVarValAssoc::create (
      fMsrOptions,
      inputLineNumber,
      "rights", val,
      msrVarValAssoc::kEqualSign,
      msrVarValAssoc::kQuotesAroundValue,
      msrVarValAssoc::kUncommented);
  }

void msrHeader::addSoftware (
  int    inputLineNumber,
  string val)
{
  fSoftwares.push_back(
    msrVarValAssoc::create (
      fMsrOptions,
      inputLineNumber,
      "software", val,
      msrVarValAssoc::kEqualSign,
      msrVarValAssoc::kQuotesAroundValue,
      msrVarValAssoc::kUncommented)
  );
}

void msrHeader::setEncodingDate (
  int    inputLineNumber,
  string val)
{
  fEncodingDate =
    msrVarValAssoc::create (
      fMsrOptions,
      inputLineNumber,
      "encodingdate", val,
      msrVarValAssoc::kEqualSign,
      msrVarValAssoc::kQuotesAroundValue,
      msrVarValAssoc::kUncommented);
}

void msrHeader::setScoreInstrument (
  int    inputLineNumber,
  string val)
{
  fScoreInstrument =
    msrVarValAssoc::create (
      fMsrOptions,
      inputLineNumber,
      "score-instrument", val,
      msrVarValAssoc::kEqualSign,
      msrVarValAssoc::kQuotesAroundValue,
      msrVarValAssoc::kUncommented);
}

void msrHeader::acceptIn (basevisitor* v) {
  if (fMsrOptions->fDebug)
    cerr << idtr <<
      "==> msrHeader::acceptIn()" << endl;
      
  if (visitor<S_msrHeader>*
    p =
      dynamic_cast<visitor<S_msrHeader>*> (v)) {
        S_msrHeader elem = this;
        
        if (fMsrOptions->fDebug)
          cerr << idtr <<
            "==> Launching msrHeader::visitStart()" << endl;
        p->visitStart (elem);
  }
}

void msrHeader::acceptOut (basevisitor* v) {
  if (fMsrOptions->fDebug)
    cerr << idtr <<
      "==> msrHeader::acceptOut()" << endl;

  if (visitor<S_msrHeader>*
    p =
      dynamic_cast<visitor<S_msrHeader>*> (v)) {
        S_msrHeader elem = this;
      
        if (fMsrOptions->fDebug)
          cerr << idtr <<
            "==> Launching msrHeader::visitEnd()" << endl;
        p->visitEnd (elem);
  }
}


void msrHeader::browseData (basevisitor* v)
{}

ostream& operator<< (ostream& os, const S_msrHeader& elt)
{
  elt->print(os);
  return os;
}

void msrHeader::printMusicXML (ostream& os)
{
  os << "<!-- msrHeader??? -->" << endl;
}

void msrHeader::printMSR (ostream& os)
{
  os << "Header" << endl;

  idtr++;
  
  if (fWorkNumber) {
    os << idtr << fWorkNumber;
  }
  
  if (fWorkTitle) {
    os << idtr << fWorkTitle;
  }
    
  if (fMovementNumber) {
    os << idtr << fMovementNumber;
  }
    
  if (fMovementTitle) {
    os << idtr << fMovementTitle;
  }
    
  if (!fCreators.empty()) {
    vector<S_msrVarValAssoc>::const_iterator i1;
    for (i1=fCreators.begin(); i1!=fCreators.end(); i1++) {
      os << idtr << (*i1);
    } // for
  }
    
  if (fRights) {
    os << idtr << fRights;
  }
    
  if (!fSoftwares.empty()) {
    vector<S_msrVarValAssoc>::const_iterator i2;
    for (i2=fSoftwares.begin(); i2!=fSoftwares.end(); i2++) {
      os << idtr << (*i2);
    } // for
  }
    
  if (fEncodingDate) {
    os << idtr << fEncodingDate;
  }
  
  idtr--;
}

void msrHeader::printScoreSummary (ostream& os)
{
  os << "Header" << endl;

  idtr++;
  
  if (fWorkNumber) {
    os << idtr << fWorkNumber;
  }
  
  if (fWorkTitle) {
    os << idtr << fWorkTitle;
  }
    
  if (fMovementNumber) {
    os << idtr << fMovementNumber;
  }
    
  if (fMovementTitle) {
    os << idtr << fMovementTitle;
  }
    
  if (!fCreators.empty()) {
    vector<S_msrVarValAssoc>::const_iterator i1;
    for (i1=fCreators.begin(); i1!=fCreators.end(); i1++) {
      os << idtr << (*i1);
    } // for
  }
    
  if (fRights) {
    os << idtr << fRights;
  }
    
  if (!fSoftwares.empty()) {
    vector<S_msrVarValAssoc>::const_iterator i2;
    for (i2=fSoftwares.begin(); i2!=fSoftwares.end(); i2++) {
      os << idtr << (*i2);
    } // for
  }
    
  if (fEncodingDate) {
    os << idtr << fEncodingDate;
  }
  
  idtr--;
}

void msrHeader::printLilyPondCode (ostream& os)
{
  os << "\\header {" << endl;
  
  idtr++;
    
  if (fWorkNumber) {
    string source = fWorkNumber->getVariableValue();
    string dest;
    for_each( source.begin(), source.end(), stringQuoteEscaper(dest));
    os << idtr << 
      "%" << fWorkNumber->getVariableName() << " = \""  << 
      dest << "\"" << endl;
  }
  
  if (fWorkTitle) {
    string source = fWorkTitle->getVariableValue();
    string dest;
    for_each( source.begin(), source.end(), stringQuoteEscaper(dest));
    os << idtr << 
      "%" << fWorkTitle->getVariableName() << " = \""  << 
      dest << "\"" << endl;
    os << idtr << 
      "title" << " = \""  << 
      dest << "\"" << endl;
  }
    
  if (fMovementNumber) {
    string source = fMovementNumber->getVariableValue();
    string dest;
    for_each( source.begin(), source.end(), stringQuoteEscaper(dest));
    os << idtr << 
      "%" << fMovementNumber->getVariableName() << " = \""  << 
      dest << "\"" << endl;
  }
    
  if (fMovementTitle) {
    string source = fMovementTitle->getVariableValue();
    string dest;
    for_each( source.begin(), source.end(), stringQuoteEscaper(dest));
    os << idtr << 
      "%" << fMovementTitle->getVariableName() << " = \""  << 
      dest << "\"" << endl;
    os << idtr << 
      "subtitle" << " = \""  << 
      dest << "\"" << endl;
  }
    
  if (!fCreators.empty()) {
    vector<S_msrVarValAssoc>::const_iterator i1;
    for (i1=fCreators.begin(); i1!=fCreators.end(); i1++) {
      os << idtr << (*i1);
      if ((*i1)->getVariableName() == "composer")
      os << idtr << 
        "composer" << " = \""  << 
        (*i1)->getVariableValue() << "\"" << endl;
    } // for
  }
    
  if (fRights) {
    string source = fRights->getVariableValue();
    string dest;
    for_each( source.begin(), source.end(), stringQuoteEscaper(dest));
    os << idtr << 
      "%" << fRights->getVariableName() << " = \""  << 
      dest << "\"" << endl;
  }
    
  if (!fSoftwares.empty()) {
    vector<S_msrVarValAssoc>::const_iterator i2;
    for (i2=fSoftwares.begin(); i2!=fSoftwares.end(); i2++) {
      os << idtr << (*i2);
     } // for
  }
    
  if (fEncodingDate) {
    string source = fEncodingDate->getVariableValue();
    string dest;
    for_each( source.begin(), source.end(), stringQuoteEscaper(dest));
    os << idtr << 
      "%" << fEncodingDate->getVariableName() << " = \""  << 
      dest << "\"" << endl;
  }
  
  if (fScoreInstrument) {
    string source = fScoreInstrument->getVariableValue();
    string dest;
    for_each( source.begin(), source.end(), stringQuoteEscaper(dest));
    os << idtr << 
      "%" << fScoreInstrument->getVariableName() << " = \""  << 
      dest << "\"" << endl;
    os << idtr << 
      "instrument" << " = \""  << 
      dest << "\"" << endl;
  }
  
  os << "}" << endl;
  
  idtr--;
}

//______________________________________________________________________________

S_msrVarValAssoc msrVarValAssoc::create (
  S_msrOptions& msrOpts, 
  int                    inputLineNumber,
  string                 variableName,
  string                 value, 
  VarValSeparator        varValSeparator,
  QuotesKind             quotesKind,
  CommentedKind          commentedKind,
  string                 unit)
{
  msrVarValAssoc* o =
    new msrVarValAssoc(
      msrOpts, inputLineNumber,
      variableName, value, varValSeparator, 
      quotesKind, commentedKind, unit);
  assert(o!=0);
  return o;
}

msrVarValAssoc::msrVarValAssoc (
  S_msrOptions& msrOpts, 
  int                    inputLineNumber,
  string                 variableName,
  string                 value, 
  VarValSeparator        varValSeparator,
  QuotesKind             quotesKind,
  CommentedKind          commentedKind,
  string                 unit)
    : msrElement (msrOpts, inputLineNumber)
{
  fVariableName=variableName;
  fVariableValue=value;
  fVarValSeparator=varValSeparator;
  fQuotesKind=quotesKind;
  fCommentedKind=commentedKind;
  fUnit = unit;
}

msrVarValAssoc::~msrVarValAssoc() {}

void msrVarValAssoc::changeAssoc (string value) {
  fVariableValue=value;
}

void msrVarValAssoc::acceptIn (basevisitor* v) {
  if (fMsrOptions->fDebug)
    cerr << idtr <<
      "==> msrVarValAssoc::acceptIn()" << endl;
      
  if (visitor<S_msrVarValAssoc>*
    p =
      dynamic_cast<visitor<S_msrVarValAssoc>*> (v)) {
        S_msrVarValAssoc elem = this;
        
        if (fMsrOptions->fDebug)
          cerr << idtr <<
            "==> Launching msrVarValAssoc::visitStart()" << endl;
        p->visitStart (elem);
  }
}

void msrVarValAssoc::acceptOut (basevisitor* v) {
  if (fMsrOptions->fDebug)
    cerr << idtr <<
      "==> msrVarValAssoc::acceptOut()" << endl;

  if (visitor<S_msrVarValAssoc>*
    p =
      dynamic_cast<visitor<S_msrVarValAssoc>*> (v)) {
        S_msrVarValAssoc elem = this;
      
        if (fMsrOptions->fDebug)
          cerr << idtr <<
            "==> Launching msrVarValAssoc::visitEnd()" << endl;
        p->visitEnd (elem);
  }
}


void msrVarValAssoc::browseData (basevisitor* v)
{}

ostream& operator<< (ostream& os, const S_msrVarValAssoc& assoc) {
  assoc->print(os);
  return os;
}

void msrVarValAssoc::printMusicXML (ostream& os)
{
  os << "<!-- msrVarValAssoc??? -->" << endl;
}

void msrVarValAssoc::printMSR (ostream& os)
{
  os << "LilypondVarValAssoc" << endl;
  
  idtr++;
  
  os << idtr << fVariableName << endl;
  os << idtr << fVariableValue <<endl;
  
  idtr--;
}

void msrVarValAssoc::printScoreSummary (ostream& os)
{
  os << "LilypondVarValAssoc" << endl;
  
  idtr++;
  
  os << idtr << fVariableName << endl;
  os << idtr << fVariableValue <<endl;
  
  idtr--;
}

void msrVarValAssoc::printLilyPondCode (ostream& os) {
  if (fCommentedKind == kCommented) os << "\%";
  
  os << fVariableName;
  
  if (fVarValSeparator == kEqualSign) os << " = ";
  else os << " ";
  
  if (fQuotesKind == kQuotesAroundValue)
    os << "\"";
  os << fVariableValue << fUnit;
  if (fQuotesKind == kQuotesAroundValue)
    os << "\"";
  
  os << endl;
}

/*
//______________________________________________________________________________
S_msrSchemeVarValAssoc msrSchemeVarValAssoc::create (
      string     variableName,
      string     value, 
      CommentedKind   commentedKind )
{
  msrSchemeVarValAssoc* o = new msrSchemeVarValAssoc(
    variableName, value, commentedKind);
  assert(o!=0);
  return o;
}

msrSchemeVarValAssoc::msrSchemeVarValAssoc(
  string     variableName,
  string     value, 
  CommentedKind   commentedKind )
    : msrElement (msrOpts, inputLineNumber)
{
  fVariableName=variableName;
  fVariableValue=value;
  fCommentedKind=commentedKind;
}

msrSchemeVarValAssoc::~msrSchemeVarValAssoc() {}

void msrSchemeVarValAssoc::changeAssoc (string value)
{
  fVariableValue=value;
}

void msrSchemeVarValAssoc::acceptIn (basevisitor* v) {
  if (fMsrOptions->fDebug)
    cerr << idtr <<
      "==> msrSchemeVarValAssoc::acceptIn()" << endl;
      
  if (visitor<S_msrSchemeVarValAssoc>*
    p =
      dynamic_cast<visitor<S_msrSchemeVarValAssoc>*> (v)) {
        S_msrSchemeVarValAssoc elem = this;
        
        if (fMsrOptions->fDebug)
          cerr << idtr <<
            "==> Launching msrSchemeVarValAssoc::visitStart()" << endl;
        p->visitStart (elem);
  }
}

void msrSchemeVarValAssoc::acceptOut (basevisitor* v) {
  if (fMsrOptions->fDebug)
    cerr << idtr <<
      "==> msrSchemeVarValAssoc::acceptOut()" << endl;

  if (visitor<S_msrSchemeVarValAssoc>*
    p =
      dynamic_cast<visitor<S_msrSchemeVarValAssoc>*> (v)) {
        S_msrSchemeVarValAssoc elem = this;
      
        if (fMsrOptions->fDebug)
          cerr << idtr <<
            "==> Launching msrSchemeVarValAssoc::visitEnd()" << endl;
        p->visitEnd (elem);
  }
}


void msrSchemeVarValAssoc::browseData (basevisitor* v)
{}

ostream& operator<< (ostream& os, const S_msrSchemeVarValAssoc& assoc)
{
  assoc->print(os);
  return os;
}

void msrSchemeVarValAssoc::printMusicXML (ostream& os)
{
  os << "<!-- msrSchemeVarValAssoc??? -->" << endl;
}

void msrSchemeVarValAssoc::printMSR (ostream& os)
{
  os << "SchemeVarValAssoc" << endl;
  idtr++;
  os << idtr << fVariableName << endl;
  os << idtr << fVariableValue <<endl;
  idtr--;
}

void msrSchemeVarValAssoc::printScoreSummary (ostream& os)
{
  os << "SchemeVarValAssoc" << endl;
  idtr++;
  os << idtr << fVariableName << endl;
  os << idtr << fVariableValue <<endl;
  idtr--;
}

void msrSchemeVarValAssoc::printLilyPondCode (ostream& os)
{
  if (fCommentedKind == kCommented) os << "\%";
  os <<
    "#(" << 
    fVariableName << " " << fVariableValue << 
    ")" << endl;
}
*/
//______________________________________________________________________________
S_msrLayout msrLayout::create (
  S_msrOptions& msrOpts, 
  int                    inputLineNumber)
{
  msrLayout* o =
    new msrLayout (
      msrOpts, inputLineNumber);
  assert(o!=0);
  return o;
}

msrLayout::msrLayout (
  S_msrOptions& msrOpts, 
  int                    inputLineNumber)
    : msrElement (msrOpts, inputLineNumber)
{}
msrLayout::~msrLayout() {}

void msrLayout::acceptIn (basevisitor* v) {
  if (fMsrOptions->fDebug)
    cerr << idtr <<
      "==> msrLayout::acceptIn()" << endl;
      
  if (visitor<S_msrLayout>*
    p =
      dynamic_cast<visitor<S_msrLayout>*> (v)) {
        S_msrLayout elem = this;
        
        if (fMsrOptions->fDebug)
          cerr << idtr <<
            "==> Launching msrLayout::visitStart()" << endl;
        p->visitStart (elem);
  }
}

void msrLayout::acceptOut (basevisitor* v) {
  if (fMsrOptions->fDebug)
    cerr << idtr <<
      "==> msrLayout::acceptOut()" << endl;

  if (visitor<S_msrLayout>*
    p =
      dynamic_cast<visitor<S_msrLayout>*> (v)) {
        S_msrLayout elem = this;
      
        if (fMsrOptions->fDebug)
          cerr << idtr <<
            "==> Launching msrLayout::visitEnd()" << endl;
        p->visitEnd (elem);
  }
}


void msrLayout::browseData (basevisitor* v)
{}

ostream& operator<< (ostream& os, const S_msrLayout& lay)
{
  lay->print(os);
  return os;
}

void msrLayout::printMusicXML (ostream& os)
{
  os << "<!-- msrLayout??? -->" << endl;
}

void msrLayout::printMSR (ostream& os)
{
  os << "Layout" << endl;

  idtr++;

  int n1 = fmsrVarValAssocs.size();
  for (int i = 0; i < n1; i++ ) {
    os << idtr << fmsrVarValAssocs[i];
  } // for

    /* JMI
  int n2 = fMsrSchemeVarValAssocs.size();
  for (int i = 0; i < n2; i++ ) {
    os << idtr << fMsrSchemeVarValAssocs[i];
  } // for
  */
  
  idtr--;
}

void msrLayout::printScoreSummary (ostream& os)
{
  os << "Layout" << endl;

  idtr++;

  int n1 = fmsrVarValAssocs.size();
  for (int i = 0; i < n1; i++ ) {
    os << idtr << fmsrVarValAssocs[i];
  } // for

    /* JMI
  int n2 = fMsrSchemeVarValAssocs.size();
  for (int i = 0; i < n2; i++ ) {
    os << idtr << fMsrSchemeVarValAssocs[i];
  } // for
  */
  
  idtr--;
}

void msrLayout::printLilyPondCode (ostream& os)
{  
  os << idtr << "\\layout {" << endl;

  idtr++;

  int n1 = fmsrVarValAssocs.size();
  for (int i = 0; i < n1; i++ ) {
    os << idtr << fmsrVarValAssocs[i];
  } // for
  
    /* JMI
  int n2 = fMsrSchemeVarValAssocs.size();
  for (int i = 0; i < n2; i++ ) {
    os << idtr << fMsrSchemeVarValAssocs[i];
  } // for
  */
  
  idtr--;

  os << idtr << "}" << endl;
}

//______________________________________________________________________________
S_msrClef msrClef::create (
  S_msrOptions& msrOpts, 
  int                    inputLineNumber,
  string                 sign,
  int                    line,
  int                    octaveChange)
{
  msrClef* o =
    new msrClef (
      msrOpts, inputLineNumber, sign, line, octaveChange);
  assert(o!=0);
  return o;
}

msrClef::msrClef (
  S_msrOptions& msrOpts, 
  int                    inputLineNumber,
  string                 sign,
  int                    line,
  int                    octaveChange)
    : msrElement (msrOpts, inputLineNumber)
{
  fSign         = sign;
  fLine         = line;
  fOctaveChange = octaveChange;
}

msrClef::~msrClef() {}

void msrClef::acceptIn (basevisitor* v) {
  if (fMsrOptions->fDebug)
    cerr << idtr <<
      "==> msrClef::acceptIn()" << endl;
      
  if (visitor<S_msrClef>*
    p =
      dynamic_cast<visitor<S_msrClef>*> (v)) {
        S_msrClef elem = this;
        
        if (fMsrOptions->fDebug)
          cerr << idtr <<
            "==> Launching msrClef::visitStart()" << endl;
        p->visitStart (elem);
  }
}

void msrClef::acceptOut (basevisitor* v) {
  if (fMsrOptions->fDebug)
    cerr << idtr <<
      "==> msrClef::acceptOut()" << endl;

  if (visitor<S_msrClef>*
    p =
      dynamic_cast<visitor<S_msrClef>*> (v)) {
        S_msrClef elem = this;
      
        if (fMsrOptions->fDebug)
          cerr << idtr <<
            "==> Launching msrClef::visitEnd()" << endl;
        p->visitEnd (elem);
  }
}


void msrClef::browseData (basevisitor* v)
{}

ostream& operator<< (ostream& os, const S_msrClef& clef)
{
  clef->print(os);
  return os;
}

void msrClef::printMusicXML (ostream& os)
{
  os << "<!-- msrClef??? -->" << endl;
}

void msrClef::printMSR (ostream& os)
{
  os <<
    "Clef" << " \"" << fSign << "\"" <<
    " line " << fLine << ", " << fOctaveChange << "*8";
}

void msrClef::printScoreSummary (ostream& os)
{
  os <<
    "Clef" << " \"" << fSign << "\"" <<
    " line " << fLine << ", " << fOctaveChange << "*8";
}

void msrClef::printLilyPondCode (ostream& os)
{
  stringstream s; 

  // USER
//  checkStaff (staffnum);

  if ( fSign == "G") {
    if ( fLine == 2 )
      s << "treble"; 
    else { // unknown G clef line !!
      cerr << 
        "warning: unknown G clef line \"" << fLine << "\"" <<
        endl;
      return; 
      }
    }
  else if ( fSign == "F") {
    if ( fLine == 4 )
      s << "bass"; 
    else { // unknown F clef line !!
      cerr << 
        "warning: unknown F clef line \"" << fLine << "\"" <<
        endl;
      return; 
      }
    }
  else if ( fSign == "C") {
    if ( fLine == 4 )
      s << "tenor"; 
    else if ( fLine == 3 )
      s << "alto"; 
    else { // unknown C clef line !!
      cerr << 
        "warning: unknown C clef line \"" <<
        fLine << 
        "\"" <<
        endl;
      return; 
      }
    }
  else if ( fSign == "percussion") {
    s << "perc"; }
  else if ( fSign == "TAB") {
    s << "TAB"; }
  else if ( fSign == "none") {
    s << "none"; }
  else { // unknown clef sign !!
    cerr << 
      "warning: unknown clef sign \"" <<
       fSign << 
       "\"" <<
      endl;
    return; 
  }
    
 // if (fLine != kStandardLine) 
    // s << fLine; // USER
//    s >> param;
    
  if (fOctaveChange == 1)
    s << "^8";
  else if (fOctaveChange == -1)
    s << "_8";

  os << "\\clef" << " \"" << s.str() << "\"" << endl;
}

//______________________________________________________________________________
S_msrKey msrKey::create (
  S_msrOptions& msrOpts, 
  int                    inputLineNumber,
  int                    fifths,
  string                 mode,
  int                    cancel)
{
  msrKey* o =
    new msrKey (
      msrOpts, inputLineNumber, fifths, mode, cancel);
  assert (o!=0);
  return o;
}

msrKey::msrKey (
  S_msrOptions& msrOpts, 
  int                    inputLineNumber,
  int                    fifths,
  string                 mode,
  int                    cancel)
    : msrElement (msrOpts, inputLineNumber)
{
  fFifths = fifths;
  fMode   = mode;
  fCancel = cancel;

  string           tonicNote;
  msrKey::KeyMode keyMode;
  
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
        fInputLineNumber, s.str());
      }
  } // switch
  
  fTonic   = tonicNote;
  fKeyMode = keyMode; 
}

msrKey::~msrKey() {}

void msrKey::acceptIn (basevisitor* v) {
  if (fMsrOptions->fDebug)
    cerr << idtr <<
      "==> msrKey::acceptIn()" << endl;
      
  if (visitor<S_msrKey>*
    p =
      dynamic_cast<visitor<S_msrKey>*> (v)) {
        S_msrKey elem = this;
        
        if (fMsrOptions->fDebug)
          cerr << idtr <<
            "==> Launching msrKey::visitStart()" << endl;
        p->visitStart (elem);
  }
}

void msrKey::acceptOut (basevisitor* v) {
  if (fMsrOptions->fDebug)
    cerr << idtr <<
      "==> msrKey::acceptOut()" << endl;

  if (visitor<S_msrKey>*
    p =
      dynamic_cast<visitor<S_msrKey>*> (v)) {
        S_msrKey elem = this;
      
        if (fMsrOptions->fDebug)
          cerr << idtr <<
            "==> Launching msrKey::visitEnd()" << endl;
        p->visitEnd (elem);
  }
}


void msrKey::browseData (basevisitor* v)
{}

ostream& operator<< (ostream& os, const S_msrKey& key)
{
  key->print(os);
  return os;
}

void msrKey::printMusicXML (ostream& os)
{
  os << "<!-- msrKey??? -->" << endl;
}

void msrKey::printMSR (ostream& os)
{
  os << "Key " << fTonic << " ";
  if (fKeyMode == kMajor) os << "\\major";
  else os << "\\minor";
}

void msrKey::printScoreSummary (ostream& os)
{
  os << "Key " << fTonic << " ";
  if (fKeyMode == kMajor) os << "\\major";
  else os << "\\minor";
}

void msrKey::printLilyPondCode (ostream& os)
{
  os << "\\key " << fTonic << " ";
  if (fKeyMode == kMajor) os << "\\major";
  else os << "\\minor";
  os << endl;
}

//______________________________________________________________________________
S_msrTime msrTime::create (
  S_msrOptions& msrOpts, 
  int                    inputLineNumber,
  int                    numerator,
  int                    denominator)
{
  msrTime* o =
    new msrTime (
      msrOpts, inputLineNumber,
      numerator, denominator);
  assert (o!=0);
  return o;
}

msrTime::msrTime (
  S_msrOptions& msrOpts, 
  int                    inputLineNumber,
  int                    numerator,
  int                    denominator)
    : msrElement (msrOpts, inputLineNumber)
{
  fRational = rational (numerator, denominator);
// JMI  fNumerator=numerator; 
  //fDenominator=denominator;
}
msrTime::~msrTime() {}

void msrTime::acceptIn (basevisitor* v) {
  if (fMsrOptions->fDebug)
    cerr << idtr <<
      "==> msrTime::acceptIn()" << endl;
      
  if (visitor<S_msrTime>*
    p =
      dynamic_cast<visitor<S_msrTime>*> (v)) {
        S_msrTime elem = this;
        
        if (fMsrOptions->fDebug)
          cerr << idtr <<
            "==> Launching msrTime::visitStart()" << endl;
        p->visitStart (elem);
  }
}

void msrTime::acceptOut (basevisitor* v) {
  if (fMsrOptions->fDebug)
    cerr << idtr <<
      "==> msrTime::acceptOut()" << endl;

  if (visitor<S_msrTime>*
    p =
      dynamic_cast<visitor<S_msrTime>*> (v)) {
        S_msrTime elem = this;
      
        if (fMsrOptions->fDebug)
          cerr << idtr <<
            "==> Launching msrTime::visitEnd()" << endl;
        p->visitEnd (elem);
  }
}


void msrTime::browseData (basevisitor* v)
{}

ostream& operator<< (ostream& os, const S_msrTime& elt)
{
  elt->print(os);
  return os;
}

void msrTime::printMusicXML (ostream& os)
{
  os << "<!-- msrTime??? -->" << endl;
}

void msrTime::printMSR (ostream& os)
{
  os <<
    "Time " << 
    fRational.getNumerator() << "/" << fRational.getDenominator();
}

void msrTime::printScoreSummary (ostream& os)
{
  os <<
    "Time " << 
    fRational.getNumerator() << "/" << fRational.getDenominator();
}

void msrTime::printLilyPondCode (ostream& os)
{
//  os << fName << "\\time \"" << fNumerator << "/" << fDenominator << "\"" << endl;
  if (fGenerateNumericalTime)
    os << "\\numericTimeSignature ";
  os <<
    "\\time " <<
    fRational.getNumerator() << "/" << fRational.getDenominator() <<
    endl;
}

//______________________________________________________________________________
S_msrTempo msrTempo::create (
  S_msrOptions& msrOpts, 
  int                    inputLineNumber,
  int                    tempoUnit,
  int                    perMinute)
{
  msrTempo* o =
    new msrTempo (
      msrOpts, inputLineNumber, tempoUnit, perMinute);
  assert(o!=0);
  return o;
}

msrTempo::msrTempo (
  S_msrOptions& msrOpts, 
  int                    inputLineNumber,
  int                    tempoUnit,
  int                    perMinute)
    : msrElement (msrOpts, inputLineNumber)
{
  fTempoUnit = tempoUnit;
  fPerMinute = perMinute;
}
msrTempo::~msrTempo() {}

void msrTempo::acceptIn (basevisitor* v) {
  if (fMsrOptions->fDebug)
    cerr << idtr <<
      "==> msrTempo::acceptIn()" << endl;
      
  if (visitor<S_msrTempo>*
    p =
      dynamic_cast<visitor<S_msrTempo>*> (v)) {
        S_msrTempo elem = this;
        
        if (fMsrOptions->fDebug)
          cerr << idtr <<
            "==> Launching msrTempo::visitStart()" << endl;
        p->visitStart (elem);
  }
}

void msrTempo::acceptOut (basevisitor* v) {
  if (fMsrOptions->fDebug)
    cerr << idtr <<
      "==> msrTempo::acceptOut()" << endl;

  if (visitor<S_msrTempo>*
    p =
      dynamic_cast<visitor<S_msrTempo>*> (v)) {
        S_msrTempo elem = this;
      
        if (fMsrOptions->fDebug)
          cerr << idtr <<
            "==> Launching msrTempo::visitEnd()" << endl;
        p->visitEnd (elem);
  }
}


void msrTempo::browseData (basevisitor* v)
{}

ostream& operator<< (ostream& os, const S_msrTempo& nstf)
{
  nstf->print(os);
  return os;
}

void msrTempo::printMusicXML (ostream& os)
{
  os << "<!-- msrTempo??? -->" << endl;
}

void msrTempo::printMSR (ostream& os)
{
  os <<
    "Tempo" << " " <<
    fTempoUnit << " " << fPerMinute << endl;
}

void msrTempo::printScoreSummary (ostream& os)
{
  os <<
    "Tempo" << " " <<
    fTempoUnit << " " << fPerMinute << endl;
}

void msrTempo::printLilyPondCode (ostream& os)
{
  os <<
    "\\tempo" << " " <<
    fTempoUnit << " = " << fPerMinute << endl;
}

//______________________________________________________________________________
S_msrRepeat msrRepeat::create (
  S_msrOptions& msrOpts, 
  int                    inputLineNumber)
{
  msrRepeat* o =
    new msrRepeat (
      msrOpts, inputLineNumber);
  assert(o!=0);
  return o;
}

msrRepeat::msrRepeat (
  S_msrOptions& msrOpts, 
  int                    inputLineNumber)
    : msrElement (msrOpts, inputLineNumber)
{
  fActuallyUsed = false;
}
msrRepeat::~msrRepeat() {}

void msrRepeat::acceptIn (basevisitor* v) {
  if (fMsrOptions->fDebug)
    cerr << idtr <<
      "==> msrRepeat::acceptIn()" << endl;
      
  if (visitor<S_msrRepeat>*
    p =
      dynamic_cast<visitor<S_msrRepeat>*> (v)) {
        S_msrRepeat elem = this;
        
        if (fMsrOptions->fDebug)
          cerr << idtr <<
            "==> Launching msrRepeat::visitStart()" << endl;
        p->visitStart (elem);
  }
}

void msrRepeat::acceptOut (basevisitor* v) {
  if (fMsrOptions->fDebug)
    cerr << idtr <<
      "==> msrRepeat::acceptOut()" << endl;

  if (visitor<S_msrRepeat>*
    p =
      dynamic_cast<visitor<S_msrRepeat>*> (v)) {
        S_msrRepeat elem = this;
      
        if (fMsrOptions->fDebug)
          cerr << idtr <<
            "==> Launching msrRepeat::visitEnd()" << endl;
        p->visitEnd (elem);
  }
}


void msrRepeat::browseData (basevisitor* v)
{}

ostream& operator<< (ostream& os, const S_msrRepeat& rept)
{
  rept->print(os);
  return os;
}

void msrRepeat::printMusicXML (ostream& os)
{
  os << "<!-- msrRepeat??? -->" << endl;
}

void msrRepeat::printMSR (ostream& os)
{
  os << "Repeat" << endl;
  idtr++;
    os << idtr << fCommonPart;
    vector<S_msrSequentialMusic>::const_iterator i;
    for (i=fAlternateEndings.begin(); i!=fAlternateEndings.end(); i++) {
      os << idtr << (*i);
    } // for
  idtr--;
}

void msrRepeat::printScoreSummary (ostream& os)
{}

void msrRepeat::printLilyPondCode (ostream& os)
{
  os << "Repeat" << endl;
}

//______________________________________________________________________________
S_msrLyricsChunk msrLyricsChunk::create (
  S_msrOptions& msrOpts, 
  int                    inputLineNumber,
  LyricsChunkType        chunkType,
  string                 chunkText,
  S_msrDuration          msrDuration)
{
  msrLyricsChunk* o =
    new msrLyricsChunk (
      msrOpts, inputLineNumber,
      chunkType, chunkText, msrDuration);
  assert(o!=0);
  return o;
}

msrLyricsChunk::msrLyricsChunk (
  S_msrOptions& msrOpts, 
  int                    inputLineNumber,
  LyricsChunkType        chunkType,
  string                 chunkText,
  S_msrDuration          msrDuration)
    : msrElement (msrOpts, inputLineNumber)
{
  fLyricsChunkType = chunkType;
  fChunkText       = chunkText;
  fChunkDuration   = msrDuration;
}
msrLyricsChunk::~msrLyricsChunk() {}

void msrLyricsChunk::acceptIn (basevisitor* v) {
  if (fMsrOptions->fDebug)
    cerr << idtr <<
      "==> msrLyricsChunk::acceptIn()" << endl;
      
  if (visitor<S_msrLyricsChunk>*
    p =
      dynamic_cast<visitor<S_msrLyricsChunk>*> (v)) {
        S_msrLyricsChunk elem = this;
        
        if (fMsrOptions->fDebug)
          cerr << idtr <<
            "==> Launching msrLyricsChunk::visitStart()" << endl;
        p->visitStart (elem);
  }
}

void msrLyricsChunk::acceptOut (basevisitor* v) {
  if (fMsrOptions->fDebug)
    cerr << idtr <<
      "==> msrLyricsChunk::acceptOut()" << endl;

  if (visitor<S_msrLyricsChunk>*
    p =
      dynamic_cast<visitor<S_msrLyricsChunk>*> (v)) {
        S_msrLyricsChunk elem = this;
      
        if (fMsrOptions->fDebug)
          cerr << idtr <<
            "==> Launching msrLyricsChunk::visitEnd()" << endl;
        p->visitEnd (elem);
  }
}

void msrLyricsChunk::browseData (basevisitor* v)
{}

ostream& operator<< (ostream& os, const S_msrLyricsChunk& lyr)
{
  lyr->print(os);
  return os;
}

void msrLyricsChunk::printMusicXML (ostream& os)
{
  os << "<!-- msrLyricsChunk??? -->" << endl;
}

void msrLyricsChunk::printMSR (ostream& os)
{  
  os << "LyricsChunk" << " " << setw(6) << left;
  switch (fLyricsChunkType) {
    case kSingleChunk:
      os << "single" << ":" << fChunkDuration;
      if (fChunkText.size()) os << " " << "\"" << fChunkText << "\"";
      break;
    case kBeginChunk:
      os << "begin" << ":" << fChunkDuration;
      if (fChunkText.size()) os << " " << "\"" << fChunkText << "\"";
      break;
    case kMiddleChunk:
      os << "middle" << ":" << fChunkDuration;
      if (fChunkText.size()) os << " " << "\"" << fChunkText << "\"";
      break;
    case kEndChunk:
      os << "end" << ":" << fChunkDuration;
      if (fChunkText.size()) os << " " << "\"" << fChunkText << "\"";
      break;
      
    case kSkipChunk:
      os << "skip" << ":" << fChunkDuration;
      if (fChunkText.size()) os << " " << fChunkText;
      break;
      
    case kSlurChunk:
      os << "slur" << ":" << fChunkDuration;
      if (fChunkText.size()) os << " " << fChunkText;
      break;
    case kTiedChunk:
      os << "tied" << ":" << fChunkDuration;
      if (fChunkText.size()) os << " " << fChunkText;
      break;
      
    case kBreakChunk:
      os << "break" << " " << fChunkText << endl;
      break;
      
    case k_NoChunk:
      msrInternalError (
        fInputLineNumber,
        "lyrics chunk type has not been set");
      break;
  } // switch
  os << endl;
}

void msrLyricsChunk::printScoreSummary (ostream& os)
{}

void msrLyricsChunk::printLilyPondCode (ostream& os)
{  
  switch (fLyricsChunkType) {
    case kSingleChunk: os << fChunkText;           break;
    case kBeginChunk:  os << fChunkText;           break;
    case kMiddleChunk: os << " -- " << fChunkText; break;
    case kEndChunk:    os << " ++ " << fChunkText; break;
    case kSkipChunk:   os << "\\skip";             break;
    case kSlurChunk:                               break;
    case kTiedChunk:                               break;
    case kBreakChunk:
      os << "%{ " << fChunkText << " %}" << endl << idtr;
      break;
    case k_NoChunk:
      msrInternalError (
        fInputLineNumber,
        "lyrics chunk type has not been set");
      break;
 } // switch
}

//______________________________________________________________________________
S_msrLyrics msrLyrics::create (
  S_msrOptions& msrOpts, 
  int                    inputLineNumber,
  int                    lyricsNumber,
  S_msrVoice             lyricsVoice)
{
  msrLyrics* o =
    new msrLyrics (
      msrOpts, inputLineNumber, lyricsNumber, lyricsVoice);
  assert(o!=0);
  return o;
}

msrLyrics::msrLyrics (
  S_msrOptions& msrOpts, 
  int                    inputLineNumber,
  int                    lyricsNumber,
  S_msrVoice             lyricsVoice)
    : msrElement (msrOpts, inputLineNumber)
{
  fMsrOptions = msrOpts;
  fLyricsNumber        = lyricsNumber;
  fLyricsVoice         = lyricsVoice; 
 
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
      fLyricsNumber == 0
        ? "Master"
        : int2EnglishWord (fLyricsNumber);
        
  return
   fLyricsVoice->getVoiceName() +
    "_L_" +
    lyricsNameSuffix;
}

msrLyrics::~msrLyrics() {}

void msrLyrics::addTextChunkToLyrics (
  int                             inputLineNumber,
  string                          syllabic,
  msrLyricsChunk::LyricsChunkType chunkType,
  string                          text,
  bool                            elision,
  S_msrDuration                   msrDuration)
{
  // create a lyrics text chunk
//  if (true || fMsrOptions->fDebug) {
  if (fMsrOptions->fDebug) {
    S_msrStaff staff = fLyricsVoice->getVoiceStaff();
    S_msrPart  part  = staff-> getStaffPart();
    
    cerr << idtr <<
      "--> Adding text chunk " <<
      setw(8) << left << "\""+syllabic+"\"" <<
      " \"" << text << "\" :" << msrDuration << 
      " elision: " << elision << 
      " to " << getLyricsName () << endl;
  }

  S_msrLyricsChunk
    chunk =
      msrLyricsChunk::create (
        fMsrOptions,
        inputLineNumber,
        chunkType, text, msrDuration);
  
  switch (chunkType) {
    case msrLyricsChunk::kSingleChunk:
    case msrLyricsChunk::kBeginChunk:
      {  
      // add lyrics chunk to this lyrics
      fLyricsChunks.push_back (chunk);
      }
      break;

    case msrLyricsChunk::kMiddleChunk:
    case msrLyricsChunk::kEndChunk:
      // add chunk to this lyrics
      fLyricsChunks.push_back (chunk);
      break;
      
    case msrLyricsChunk::kSkipChunk:
    case msrLyricsChunk::kSlurChunk:
    case msrLyricsChunk::kTiedChunk:
    case msrLyricsChunk::kBreakChunk:
      {
        msrInternalError (
          fInputLineNumber,
          "a text chunk type can only be "
          "'single', 'begin', 'middle' or 'end'");
      }
      break;
    case msrLyricsChunk::k_NoChunk:
      msrInternalError (
        fInputLineNumber,
        "lyrics chunk type has not been set");
      break;
  } // switch

  fLyricsTextPresent = true;
}

void msrLyrics::addSkipChunkToLyrics (
  int            inputLineNumber,
  S_msrDuration  msrDuration)
{
//  if (true || fMsrOptions->fDebug) {
  if (fMsrOptions->fDebug) {
    S_msrStaff staff = fLyricsVoice->getVoiceStaff();
    S_msrPart  part  = staff-> getStaffPart();
    
    cerr << idtr <<
      "--> Adding skip chunk: " << msrDuration <<
      " to " << getLyricsName () << endl;
  }
  
  // create lyrics skip chunk
  S_msrLyricsChunk
    chunk =
      msrLyricsChunk::create (
        fMsrOptions,
        inputLineNumber,
        msrLyricsChunk::kSkipChunk, "", msrDuration);
        
  // add chunk to this lyrics
  fLyricsChunks.push_back (chunk);
}

void msrLyrics::addSlurChunkToLyrics (
  int            inputLineNumber,
  S_msrDuration  msrDuration)
{
//  if (true || fMsrOptions->fDebug) {
  if (fMsrOptions->fDebug) {
    S_msrStaff staff = fLyricsVoice->getVoiceStaff();
    S_msrPart  part  = staff-> getStaffPart();
    
    cerr << idtr <<
      "--> Adding slur chunk: " << msrDuration <<
      " to " << getLyricsName () << endl;
  }
  
  // create lyrics slur chunk
  S_msrLyricsChunk
    chunk =
      msrLyricsChunk::create (
        fMsrOptions,
        inputLineNumber,
        msrLyricsChunk::kSlurChunk, "", msrDuration);
        
  // add chunk to this lyrics
  fLyricsChunks.push_back (chunk);
}

void msrLyrics::addTiedChunkToLyrics (
  int            inputLineNumber,
  S_msrDuration  msrDuration)
{
//  if (true || fMsrOptions->fDebug) {
  if (fMsrOptions->fDebug) {
    S_msrStaff staff = fLyricsVoice->getVoiceStaff();
    S_msrPart  part  = staff-> getStaffPart();
    
    cerr << idtr <<
      "--> Adding tied chunk: " << msrDuration <<
      " to " << getLyricsName () << endl;
  }
  
  // create lyrics tied chunk
  S_msrLyricsChunk
    chunk =
      msrLyricsChunk::create (
        fMsrOptions,
        inputLineNumber,
        msrLyricsChunk::kTiedChunk, "", msrDuration);
        
  // add chunk to this lyrics
  fLyricsChunks.push_back (chunk);
}

void msrLyrics::addBreakChunkToLyrics (
  int inputLineNumber,
  int nextMeasureNumber)
{
//  if (true || fMsrOptions->fDebug) {
  if (fMsrOptions->fDebug) {
    S_msrStaff staff = fLyricsVoice->getVoiceStaff();
    S_msrPart  part  = staff-> getStaffPart();
    
    cerr << idtr <<
      "--> Adding break chunk" <<
      " to " << getLyricsName () << endl;
  }

  // convert nextMeasureNumber to string
  stringstream s;
  s << nextMeasureNumber;
  
  // create lyrics break chunk

  S_msrDuration
    nullMsrDuration =
      msrDuration::create (
        fMsrOptions,
        inputLineNumber,
        0, 1, 0, "");
        
  S_msrLyricsChunk
    chunk =
      msrLyricsChunk::create (
        fMsrOptions,
        inputLineNumber,
        msrLyricsChunk::kBreakChunk,
        s.str(),
        nullMsrDuration);
        
  // add chunk to this lyrics
  fLyricsChunks.push_back (chunk);
}

void msrLyrics::acceptIn (basevisitor* v) {
  if (fMsrOptions->fDebug)
    cerr << idtr <<
      "==> msrLyrics::acceptIn()" << endl;
      
  if (visitor<S_msrLyrics>*
    p =
      dynamic_cast<visitor<S_msrLyrics>*> (v)) {
        S_msrLyrics elem = this;
        
        if (fMsrOptions->fDebug)
          cerr << idtr <<
            "==> Launching msrLyrics::visitStart()" << endl;
        p->visitStart (elem);
  }
}

void msrLyrics::acceptOut (basevisitor* v) {
  if (fMsrOptions->fDebug)
    cerr << idtr <<
      "==> msrLyrics::acceptOut()" << endl;

  if (visitor<S_msrLyrics>*
    p =
      dynamic_cast<visitor<S_msrLyrics>*> (v)) {
        S_msrLyrics elem = this;
      
        if (fMsrOptions->fDebug)
          cerr << idtr <<
            "==> Launching msrLyrics::visitEnd()" << endl;
        p->visitEnd (elem);
  }
}

void msrLyrics::browseData (basevisitor* v)
{}

ostream& operator<< (ostream& os, const S_msrLyrics& stan)
{
  stan->print(os);
  return os;
}

void msrLyrics::printMusicXML (ostream& os)
{
  os << "<!-- msrLyrics??? -->" << endl;
}

void msrLyrics::printMSR (ostream& os)
{  
  os << "Lyrics" << " " << getLyricsName ();
  if (! fLyricsTextPresent)
    os << " (No actual text)";
  os << endl;

//  if (fLyricsTextPresent) {  JMI
    idtr++;

    int n = fLyricsChunks.size();
    for (int i = 0; i < n; i++) {
      os << idtr << fLyricsChunks[i];
    } // for
    os << endl;

    idtr--;
 // }
}

void msrLyrics::printScoreSummary (ostream& os)
{  
  int lyricsChunksSize = fLyricsChunks.size();

  os << "Lyrics" << " " << getLyricsName () <<
    " contains " << lyricsChunksSize;
  if (lyricsChunksSize == 1)
    os << " chunk";
  else
    os << " chunks";

  if (! fLyricsTextPresent)
    os << " (No actual text)";
    
  os << endl;
}

void msrLyrics::printLilyPondCode (ostream& os)
{
  os <<
    getLyricsName () << " = " << "\\lyricmode {" << endl;

  idtr++;

  if (fLyricsChunks.size()) {
    vector<S_msrLyricsChunk>::const_iterator
      iBegin = fLyricsChunks.begin(),
      iEnd   = fLyricsChunks.end(),
      i      = iBegin;
      
    for ( ; ; ) {
      os << idtr << (*i);
      if (++i == iEnd) break;
      os << " ";
    } // for
  }

  idtr--;

  os << endl << idtr << "}" << endl;
}

//______________________________________________________________________________ 
S_msrVoice msrVoice::create (
  S_msrOptions& msrOpts, 
  int                    inputLineNumber,
  int                    voiceNumber,
  int                    staffRelativeVoiceNumber,
  S_msrStaff             voiceStaff)
{
  msrVoice* o =
    new msrVoice (
      msrOpts, inputLineNumber,
      voiceNumber,
      staffRelativeVoiceNumber,
      voiceStaff);
  assert(o!=0);
  return o;
}

msrVoice::msrVoice (
  S_msrOptions& msrOpts, 
  int                    inputLineNumber,
  int                    voiceNumber,
  int                    staffRelativeVoiceNumber,
  S_msrStaff             voiceStaff)
    : msrElement (msrOpts, inputLineNumber)
{
  fMsrOptions = msrOpts;

  fVoiceNumber = voiceNumber;
  fStaffRelativeVoiceNumber = staffRelativeVoiceNumber;
  fVoiceStaff  = voiceStaff;

  if (fMsrOptions->fTrace)
    cerr << idtr <<
      "Creating voice " << getVoiceName () << endl;

  fVoiceContainsActualNotes = false;
  
  // create the implicit msrSequentialMusic element
  fVoiceSequentialMusic =
    msrSequentialMusic::create (
      msrOpts, inputLineNumber,
      msrSequentialMusic::kSpace);

  // add the implicit initial C major key
  S_msrKey
    key =
      msrKey::create (
        msrOpts, inputLineNumber,
        0, "major", 0);
  S_msrElement k = key;
  fVoiceSequentialMusic->appendElementToSequentialMusic (k);

  // add the implicit initial 4/4 time signature
  S_msrTime
    time =
      msrTime::create (
        msrOpts, inputLineNumber,
        4, 4);
  S_msrElement t = time;
  fVoiceSequentialMusic->appendElementToSequentialMusic (t);

  // add the master lyrics to this voice, to
  // collect skips along the way that are used as a 'prelude'
  // by actual lyrics that start at later points
  fVoiceMasterLyrics =
    addLyricsToVoice (inputLineNumber, 0);

  // add the implicit msrRepeat element
// JMI  fVoiceMsrRepeat = msrRepeat::create ();
//  fVoiceSequentialMusic->appendElementToSequentialMusic (fVoiceMsrRepeat);
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
    fVoiceStaff->getStaffName() +
    "_V_" +
    int2EnglishWord (voiceNumber);
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
        this);

  // register it in this voice
  if (fMsrOptions->fTrace)
    cerr << idtr <<
      "Adding lyrics " << lyricsNumber <<
      " " << lyrics->getLyricsName () <<
      " to voice " << getVoiceName () << endl;

  fVoiceLyricsMap [lyricsNumber] = lyrics;

  // catch up with fVoiceMasterLyrics if non null
  // in case the lyrics do not start upon the first voice note

  if (fVoiceMasterLyrics) { // JMI
    vector<S_msrLyricsChunk>
      masterChunks =
        fVoiceMasterLyrics->getLyricsChunks ();
  
    if (masterChunks.size()) {
      if (fMsrOptions->fTrace)
        cerr << idtr <<
          "Copying current contents of voice master lyrics to new lyrics" << endl;
      for (
        vector<S_msrLyricsChunk>::const_iterator i =
          masterChunks.begin();
        i != masterChunks.end();
        i++) {
        // add chunk to lyrics
        lyrics->addChunkToLyrics ((*i));
      } // for
    }
  }

  // return it
  return lyrics;
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

void msrVoice::appendNoteToVoice (S_msrNote note) {
  S_msrElement n = note;
  fVoiceSequentialMusic->appendElementToSequentialMusic (n);

  if (note->getNoteIsARest ())
    fVoiceContainsActualNotes = true;
    
  // add a skip chunk to the master lyrics
  S_msrDuration
    lyricsMsrDuration =
      note->getNoteMsrDuration ();

  fVoiceMasterLyrics->
    addSkipChunkToLyrics (
      note->getInputLineNumber (), lyricsMsrDuration);
}

void msrVoice::appendChordToVoice (S_msrChord chord) {
  S_msrElement c = chord;
  fVoiceSequentialMusic->appendElementToSequentialMusic (c);
}

void msrVoice::appendTupletToVoice (S_msrTuplet tuplet) {
  S_msrElement t = tuplet;
  fVoiceSequentialMusic->appendElementToSequentialMusic (t);
}

void msrVoice::appendElementToVoice (S_msrElement elem)
{
  fVoiceSequentialMusic->appendElementToSequentialMusic (elem);
}

void msrVoice::acceptIn (basevisitor* v) {
  if (fMsrOptions->fDebug)
    cerr << idtr <<
      "==> msrVoice::acceptIn()" << endl;
      
  if (visitor<S_msrVoice>*
    p =
      dynamic_cast<visitor<S_msrVoice>*> (v)) {
        S_msrVoice elem = this;
        
        if (fMsrOptions->fDebug)
          cerr << idtr <<
            "==> Launching msrVoice::visitStart()" << endl;
        p->visitStart (elem);
  }
}

void msrVoice::acceptOut (basevisitor* v) {
  if (fMsrOptions->fDebug)
    cerr << idtr <<
      "==> msrVoice::acceptOut()" << endl;

  if (visitor<S_msrVoice>*
    p =
      dynamic_cast<visitor<S_msrVoice>*> (v)) {
        S_msrVoice elem = this;
      
        if (fMsrOptions->fDebug)
          cerr << idtr <<
            "==> Launching msrVoice::visitEnd()" << endl;
        p->visitEnd (elem);
  }
}

void msrVoice::browseData (basevisitor* v)
{
  if (fMsrOptions->fDebug)
    cerr << idtr <<
      "==> msrVoice::browseData()" << endl;
  
  idtr++;

  // create the sequential music browser
  msrBrowser<msrSequentialMusic> browser (v);

  // browse the sequential music with the visitor
  browser.browse (*fVoiceSequentialMusic);

  // browse the voice lyrics
  for (
    map<int, S_msrLyrics>::iterator i = fVoiceLyricsMap.begin();
    i != fVoiceLyricsMap.end();
    i++) {
    // create the lyrics browser
    msrBrowser<msrLyrics> browser (v);
  
    // browse the lyrics with the visitor
    browser.browse (*((*i).second));
  } // for

  idtr--;

  if (fMsrOptions->fDebug)
    cerr << idtr <<
      "<== msrVoice::browseData()" << endl;
}

ostream& operator<< (ostream& os, const S_msrVoice& elt)
{
  elt->print(os);
  return os;
}

void msrVoice::printMusicXML (ostream& os)
{ 
  os << "<!-- msrVoice??? -->" << endl;
}

void msrVoice::printMSR (ostream& os)
{
  os << "Voice" << " " << getVoiceName () << endl;
  os << endl;

  idtr++;

  os << idtr << fVoiceSequentialMusic << endl;

  if (! fMsrOptions->fDontGenerateLyrics) {
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

void msrVoice::printScoreSummary (ostream& os)
{
  int voiceLyricsMapSize = fVoiceLyricsMap.size();

  os <<
    "Voice" << " " << getVoiceName () <<
    " has " << voiceLyricsMapSize;
  if (voiceLyricsMapSize == 1)
    os << " lyric";
  else
    os << " lyrics";
  os << endl;

  idtr++;

  os << idtr << fVoiceSequentialMusic;

  // don't show voice master lyrics in summary
  if (voiceLyricsMapSize > 1) {
    map<int, S_msrLyrics>::const_iterator
      iBegin = fVoiceLyricsMap.begin(),
      iEnd   = fVoiceLyricsMap.end(),
      i      = iBegin;
      
    for ( ; ; ) {
      if ((*i).first != 0)
        os << idtr << (*i).second;
      if (++i == iEnd) break;
      os << endl;
    } // for
  }
  
  idtr--;
}

void msrVoice::printLilyPondCode (ostream& os)
{
  os << getVoiceName () << " = ";
// JMI  if (! fVoiceAbsoluteCode) os << "\\relative ";
  os << "{" << endl;

  idtr++;
  os << fVoiceSequentialMusic;
  idtr--;

  os << idtr << "}" << endl;
}

//______________________________________________________________________________
int msrStaff::gMaxStaffVoices = 4;

S_msrStaff msrStaff::create (
  S_msrOptions& msrOpts, 
  int                    inputLineNumber,
  int                    staffNumber,
  S_msrPart              staffPart)
{
  msrStaff* o =
    new msrStaff (
      msrOpts, inputLineNumber, staffNumber, staffPart);
  assert(o!=0);
  return o;
}

msrStaff::msrStaff (
  S_msrOptions& msrOpts, 
  int                    inputLineNumber,
  int                    staffNumber,
  S_msrPart              staffPart)
    : msrElement (msrOpts, inputLineNumber)
{
  fMsrOptions = msrOpts;
  
  fStaffNumber = staffNumber;
  fStaffPart   = staffPart;

  fNextRelativeStaffVoiceNumber = 0;

  if (fMsrOptions->fTrace)
    cerr << idtr <<
      "Creating staff " << getStaffName () << endl;

  // create the implicit initial C major key
  fStaffKey =
    msrKey::create (
      msrOpts,
      inputLineNumber,
      0, "major", 0);

  // create the implicit initial 4/4 time signature
  fStaffTime =
    msrTime::create (
      msrOpts,
      inputLineNumber,
      4, 4);

  // add the maximum number of empty voices
  // those that turn out empty will be removed later
  /*
  for (int i = 1; i <= gMaxStaffVoices; i++) {
    S_msrVoice
      dummyVoice =
        addVoiceToStaff (i);
  } // for
  */
}

msrStaff::~msrStaff() {}

string msrStaff::getStaffName () const
  {
  // not stored in a field,
  // because the staff part may change name
  // when it is re-used
  return
    fStaffPart->getPartMSRName() +
    "_S_" +
    int2EnglishWord (fStaffNumber);
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
    msrMusicXMLWarning (999, s.str()); // JMI
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
      " in part " << fStaffPart->getPartCombinedName () << endl;
  
  fStaffVoicesMap [voiceNumber] = voice;

  // return it
  return voice;
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

void msrStaff::setStaffKey  (S_msrKey  key)
{
  if (fMsrOptions->fTrace)
    cerr << idtr <<
      "Adding key '" << key <<
      "' to staff " << fStaffNumber <<
      " in part " << fStaffPart->getPartCombinedName () << endl;

  fStaffKey = key;
}

void msrStaff::setStaffTime (S_msrTime time)
{
  if (fMsrOptions->fTrace)
    cerr << idtr <<
      "Adding time '" << time <<
      "' to staff " << fStaffNumber <<
      " in part " << fStaffPart->getPartCombinedName () << endl;

  fStaffTime = time;
}
      
void msrStaff::setStaffClef (S_msrClef clef)
{
  if (fMsrOptions->fTrace)
    cerr << idtr <<
      "Adding clef '" << clef <<
      "' to staff " << fStaffNumber <<
      " in part " << fStaffPart->getPartCombinedName () << endl;

  fStaffClef = clef;
}

void msrStaff::acceptIn (basevisitor* v) {
  if (fMsrOptions->fDebug)
    cerr << idtr <<
      "==> msrStaff::acceptIn()" << endl;
      
  if (visitor<S_msrStaff>*
    p =
      dynamic_cast<visitor<S_msrStaff>*> (v)) {
        S_msrStaff elem = this;
        
        if (fMsrOptions->fDebug)
          cerr << idtr <<
            "==> Launching msrStaff::visitStart()" << endl;
        p->visitStart (elem);
  }
}

void msrStaff::acceptOut (basevisitor* v) {
  if (fMsrOptions->fDebug)
    cerr << idtr <<
      "==> msrStaff::acceptOut()" << endl;

  if (visitor<S_msrStaff>*
    p =
      dynamic_cast<visitor<S_msrStaff>*> (v)) {
        S_msrStaff elem = this;
      
        if (fMsrOptions->fDebug)
          cerr << idtr <<
            "==> Launching msrStaff::visitEnd()" << endl;
        p->visitEnd (elem);
  }
}

void msrStaff::browseData (basevisitor* v)
{
  if (fMsrOptions->fDebug)
    cerr << idtr <<
      "==> msrStaff::browseData()" << endl;

  {
  // create the clef browser
  msrBrowser<msrClef> browser (v);

  // browse the voice with the visitor
  browser.browse (*fStaffClef);
  }

  {
  // create the key browser
  msrBrowser<msrKey> browser (v);

  // browse the voice with the visitor
  browser.browse (*fStaffKey);
  }

  {
  // create the time browser
  msrBrowser<msrTime> browser (v);

  // browse the voice with the visitor
  browser.browse (*fStaffTime);
  }
  
  idtr++;

  for (
    map<int, S_msrVoice>::iterator i = fStaffVoicesMap.begin();
    i != fStaffVoicesMap.end();
    i++) {
    // create the voice browser
    msrBrowser<msrVoice> browser (v);
  
    // browse the voice with the visitor
    browser.browse (*((*i).second));
  } // for

  idtr--;

  if (fMsrOptions->fDebug)
    cerr << idtr <<
      "<== msrStaff::browseData()" << endl;
}

ostream& operator<< (ostream& os, const S_msrStaff& elt)
{
  elt->print(os);
  return os;
}

void msrStaff::printMusicXML (ostream& os)
{
  os << "<!-- msrStaff??? -->" << endl;
}

void msrStaff::printMSR (ostream& os)
{
  os << "Staff" << " " << getStaffName () << endl;

  idtr++;

  if (fStaffClef)
    os << idtr << fStaffClef;
  else
    os << idtr << "NO_CLEF";
  os << endl;

  if (fStaffKey)
    os << idtr << fStaffKey;
  else
    os << idtr << "NO_KEY";
  os << endl;

  if (fStaffTime)
    os << idtr << fStaffTime;
  else
    os << idtr << "NO_TIME";
  os << endl;

  os <<
    idtr << "StaffInstrumentName: \"" <<
    fStaffInstrumentName << "\"" << endl;

  os << endl;
  
  for (
    map<int, S_msrVoice>::iterator i = fStaffVoicesMap.begin();
    i != fStaffVoicesMap.end();
    i++) {
    os << idtr << (*i).second;
  } // for

  idtr--;
}

void msrStaff::printScoreSummary (ostream& os)
{
  int staffVoicesMapSize = fStaffVoicesMap.size();
    
  os << "Staff" << " " << getStaffName () <<
    " contains " << staffVoicesMapSize;
  if (staffVoicesMapSize == 1)
    os << " voice";
  else
    os << " voices";
  os << endl;

  idtr++;
/*
  if (fStaffClef)
    os << idtr << fStaffClef;
  else
    os << idtr << "NO_CLEF" << endl;
*/
  if (fStaffKey)
    os << idtr << fStaffKey;
  else
    os << idtr << "NO_KEY";
  os << endl;
/*
  if (fStaffTime)
    os << idtr << fStaffTime;
  else
    os << idtr << "NO_TIME" << endl;
*/
  os <<
    idtr << "StaffInstrumentName: \"" <<
    fStaffInstrumentName << "\"" << endl;

  os << endl;
  
  map<int, S_msrVoice>::const_iterator
    iBegin = fStaffVoicesMap.begin(),
    iEnd   = fStaffVoicesMap.end(),
    i      = iBegin;
    
  for ( ; ; ) {
    os << idtr << (*i).second;
    if (++i == iEnd) break;
    os << endl;
  } // for

  idtr--;
}

void msrStaff::printLilyPondCode (ostream& os)
{
  os <<
    "Staff" << " " << getStaffName () << " " <<
    fStaffInstrumentName << endl;
 // if (! fMsrOptions->fGenerateAbsoluteCode) os << "\\relative "; // JMI
  os << "{" << endl;

  idtr++;
  idtr--;

  os << idtr << "}" << endl;
}

//______________________________________________________________________________ 
S_msrPart msrPart::create (
  S_msrOptions& msrOpts, 
  int                    inputLineNumber,
  string                 partMusicXMLName,
  S_msrPartgroup         partPartgroup)
{
  msrPart* o =
    new msrPart (
      msrOpts, inputLineNumber, partMusicXMLName, partPartgroup);
  assert(o!=0);
  return o;
}

msrPart::msrPart (
  S_msrOptions& msrOpts, 
  int                    inputLineNumber,
  string                 partMusicXMLName,
  S_msrPartgroup         partPartgroup)
    : msrElement (msrOpts, inputLineNumber)
{
  fMsrOptions = msrOpts;
  
  fPartMusicXMLName = partMusicXMLName;
  fPartPartgroup    = partPartgroup;

  // coin the part MSR name
  fPartMSRName =
    "P_"+stringNumbersToEnglishWords (fPartMusicXMLName);
    
  if (fMsrOptions->fTrace)
    cerr << idtr <<
      "Creating part " << getPartCombinedName () << endl;
}

msrPart::~msrPart() {}

void msrPart::setAllPartStavesKey   (S_msrKey  key)
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
          
void msrPart::setAllPartStavesClef (S_msrClef clef)
{
  for (
    map<int, S_msrStaff>::iterator i = fPartStavesMap.begin();
    i != fPartStavesMap.end();
    i++) {
    (*i).second->setStaffClef (clef);
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

  if (fMsrOptions->fTrace)
    cerr << idtr <<
      "Adding staff " << staffNumber <<
      " to part " << getPartCombinedName () << endl;
  
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
  if (fMsrOptions->fDebug)
    cerr << idtr <<
      "==> msrPart::acceptIn()" << endl;
      
  if (visitor<S_msrPart>*
    p =
      dynamic_cast<visitor<S_msrPart>*> (v)) {
        S_msrPart elem = this;
        
        if (fMsrOptions->fDebug)
          cerr << idtr <<
            "==> Launching msrPart::visitStart()" << endl;
        p->visitStart (elem);
  }
}

void msrPart::acceptOut (basevisitor* v) {
  if (fMsrOptions->fDebug)
    cerr << idtr <<
      "==> msrPart::acceptOut()" << endl;

  if (visitor<S_msrPart>*
    p =
      dynamic_cast<visitor<S_msrPart>*> (v)) {
        S_msrPart elem = this;
      
        if (fMsrOptions->fDebug)
          cerr << idtr <<
            "==> Launching msrPart::visitEnd()" << endl;
        p->visitEnd (elem);
  }
}

void msrPart::browseData (basevisitor* v)
{
  if (fMsrOptions->fDebug)
    cerr << idtr <<
      "==> msrPart::browseData()" << endl;
  
  idtr++;

  for (
    map<int, S_msrStaff>::iterator i = fPartStavesMap.begin();
    i != fPartStavesMap.end();
    i++) {
    // create the staff browser
    msrBrowser<msrStaff> browser (v);
  
    // browse the staff with the visitor
    browser.browse (*((*i).second));
  } // for

  idtr--;

  if (fMsrOptions->fDebug)
    cerr << idtr <<
      "<== msrPart::browseData()" << endl;
}

ostream& operator<< (ostream& os, const S_msrPart& elt)
{
  elt->print(os);
  return os;
}

void msrPart::printMusicXML (ostream& os)
{
  os << "<!-- msrPart??? -->" << endl;
}

void msrPart::printMSR (ostream& os)
{
  os <<
    "Part" << " \"" << getPartCombinedName () << endl;
    
  idtr++;
  
  os <<
    idtr << "PartName              : \"" << fPartName << "\"" << endl <<
    idtr << "PartAbbrevation       : \"" << fPartAbbreviation << "\"" << endl;
  
  os << idtr <<
    "PartMusicXMLDivisions : " << fPartMusicXMLDivisions << endl;

  os << idtr <<
    "PartInstrumentName    : \"" << fPartInstrumentName << "\"" << endl;

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

void msrPart::printScoreSummary (ostream& os)
{
  int partStavesMapSize = fPartStavesMap.size();
  
  os <<
    "Part " << getPartCombinedName () <<
    " contains " << partStavesMapSize;
  if (partStavesMapSize == 1)
    os << " staff";
  else
    os << " staves";
  os << endl;
    
  idtr++;
  
  os <<
    idtr << "PartName          : \"" << fPartName << "\"" << endl <<
    idtr << "PartAbbrevation   : \"" << fPartAbbreviation << "\"" << endl;
  
  os << idtr <<
    "fPartMusicXMLDivisions: " << fPartMusicXMLDivisions << endl;

  os << idtr <<
    "PartInstrumentName: \"" << fPartInstrumentName << "\"" << endl;

  if (partStavesMapSize) {
    os << endl;

    map<int, S_msrStaff>::const_iterator
      iBegin = fPartStavesMap.begin(),
      iEnd   = fPartStavesMap.end(),
      i      = iBegin;
      
    for ( ; ; ) {
      os << idtr << (*i).second;
      if (++i == iEnd) break;
      os << endl;
    } // for
  }

  idtr--;
}

void msrPart::printLilyPondCode (ostream& os)
{
  os <<
    "Part" << " " << getPartCombinedName () << " " << endl <<
    "\"" << fPartInstrumentName << "\"" << endl;
 // if (! fMsrOptions->fGenerateAbsoluteCode) os << "\\relative "; // JMI
  os << "{" << endl;

  idtr++;
// JMI  os << fPartMsrSequentialMusic << endl;
  idtr--;

  os << idtr << "}" << endl;
}

//______________________________________________________________________________
int msrPartgroup::gPartgroupsCounter = 0;

S_msrPartgroup msrPartgroup::create (
  S_msrOptions& msrOpts, 
  int                    inputLineNumber,
  int                    partGroupNumber,
  string                 partGroupName,
  string                 partGroupAbbreviation,
  PartgroupSymbolKind    partGroupSymbolKind,
  int                    partGroupSymbolDefaultX,
  bool                   partGroupBarline)
{
  msrPartgroup* o =
    new msrPartgroup (
      msrOpts, inputLineNumber,
      partGroupNumber,
      partGroupName,
      partGroupAbbreviation,
      partGroupSymbolKind,
      partGroupSymbolDefaultX,
      partGroupBarline);
  assert(o!=0);
  return o;
}

msrPartgroup::msrPartgroup (
  S_msrOptions& msrOpts, 
  int                    inputLineNumber,
  int                    partGroupNumber,
  string                 partGroupName,
  string                 partGroupAbbreviation,
  PartgroupSymbolKind    partGroupSymbolKind,
  int                    partGroupSymbolDefaultX,
  bool                   partGroupBarline)
    : msrElement (msrOpts, inputLineNumber)
{
  fPartgroupAbsoluteNumber = ++gPartgroupsCounter;
  
  fMsrOptions = msrOpts;

  fPartgroupNumber = partGroupNumber;
        
  fPartgroupName = partGroupName;
  fPartgroupAbbreviation = partGroupAbbreviation;

  fPartgroupSymbolKind = partGroupSymbolKind;
  fPartgroupSymbolDefaultX = partGroupSymbolDefaultX;

  fPartgroupBarline = partGroupBarline;
  
  if (fMsrOptions->fTrace)
    cerr << idtr <<
      "Creating part group " << fPartgroupNumber << endl;
}

msrPartgroup::~msrPartgroup() {}

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
  string partMusicXMLName)
{
  if (fPartgroupPartsMap.count (partMusicXMLName)) {
    cerr << idtr <<
      "### Internal error: partMusicXMLName " << partMusicXMLName <<
      " already exists in this part group" << endl;

    return fPartgroupPartsMap [partMusicXMLName];
  }

  // create the part
  S_msrPart
    part =
      msrPart::create (
        fMsrOptions,
        inputLineNumber,
        partMusicXMLName,
        this);

  // register it in this part group
  if (fMsrOptions->fTrace) {
    cerr << idtr <<
      "Adding part " <<
      part->getPartCombinedName () <<
      " to part group " << fPartgroupNumber << endl;
  }
  fPartgroupPartsMap [partMusicXMLName] = part;
  fPartgroupElements.push_back (part);

//  if (true || fMsrOptions->fDebug) {
  if (fMsrOptions->fDebug) {
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

//  if (true || fMsrOptions->fDebug) {
  if (fMsrOptions->fDebug) {
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

void msrPartgroup::prependSubPartgroupToPartgroup (
  S_msrPartgroup partGroup)
{
  if (fMsrOptions->fTrace)
    cerr << idtr <<
      "Adding (sub-)part group " << partGroup->getPartgroupNumber () <<
      " to part group " << getPartgroupNumber ()  << endl;

  // register it in this part group
  fPartgroupElements.push_front (partGroup);
}

S_msrPart msrPartgroup::fetchPartFromPartgroup (
  string partMusicXMLName)
{
  /*
  cerr << idtr << "==> fetchPartFromPartgroup, fPartgroupPartsMap contains:" << endl;
  for (
      map<string, S_msrPart>::const_iterator i = fPartgroupPartsMap.begin();
      i != fPartgroupPartsMap.end();
      i++) {
    cerr << idtr <<
      (*i).first << " ----> " <<
      (*i).second->getPartCombinedName() << endl;
  } // for
  cerr << idtr << "<== fetchPartFromPartgroup" << endl;
  */
  
  S_msrPart result;
  
  if (fPartgroupPartsMap.count (partMusicXMLName)) {
    result = fPartgroupPartsMap [partMusicXMLName];
  }

  return result;
}

void msrPartgroup::acceptIn (basevisitor* v) {
  if (fMsrOptions->fDebug)
    cerr << idtr <<
      "==> msrPartgroup::acceptIn()" << endl;
      
  if (visitor<S_msrPartgroup>*
    p =
      dynamic_cast<visitor<S_msrPartgroup>*> (v)) {
        S_msrPartgroup elem = this;
        
        if (fMsrOptions->fDebug)
          cerr << idtr <<
            "==> Launching msrPartgroup::visitStart()" << endl;
        p->visitStart (elem);
  }
}

void msrPartgroup::acceptOut (basevisitor* v) {
  if (fMsrOptions->fDebug)
    cerr << idtr <<
      "==> msrPartgroup::acceptOut()" << endl;

  if (visitor<S_msrPartgroup>*
    p =
      dynamic_cast<visitor<S_msrPartgroup>*> (v)) {
        S_msrPartgroup elem = this;
      
        if (fMsrOptions->fDebug)
          cerr << idtr <<
            "==> Launching msrPartgroup::visitEnd()" << endl;
        p->visitEnd (elem);
  }
}

void msrPartgroup::browseData (basevisitor* v)
{
  if (fMsrOptions->fDebug)
    cerr << idtr <<
      "==> msrPartgroup::browseData()" << endl;
  
  idtr++;

  for (
    list<S_msrElement>::iterator i = fPartgroupElements.begin();
    i != fPartgroupElements.end();
    i++) {
    // create the part element browser
    msrBrowser<msrElement> browser (v);
  
    // browse the part element with the visitor
    browser.browse (*(*i));
  } // for

  idtr--;

  if (fMsrOptions->fDebug)
    cerr << idtr <<
      "<== msrPartgroup::browseData()" << endl;
}

ostream& operator<< (ostream& os, const S_msrPartgroup& elt)
{
  elt->print(os);
  return os;
}

void msrPartgroup::printMusicXML (ostream& os)
{
  os << "<!-- msrPartgroup??? -->" << endl;
}

void msrPartgroup::printMSR (ostream& os)
{
  os <<
    "Partgroup" << " " << getPartgroupCombinedName () << endl;
    
  idtr++;

  os <<
    idtr << "PartgroupName            : \"" << fPartgroupName << "\"" << endl <<
    idtr << "PartgroupAbbrevation     : \"" << fPartgroupAbbreviation << "\"" << endl;
  os <<
    idtr << "fPartgroupSymbolDefaultX : " << fPartgroupSymbolDefaultX << endl;
  os <<
    idtr << "fPartgroupSymbolKind     : \"";
  switch (fPartgroupSymbolKind) {
    case kBracePartgroupSymbol:
      os << "brace";
      break;
    case kBracketPartgroupSymbol:
      os << "bracket";
      break;
    case kLinePartgroupSymbol:
      os << "line";
      break;
    case kSquarePartgroupSymbol:
      os << "square";
      break;
    case k_NoPartgroupSymbol:
      break;
  } // switch
  os << "\"" << endl;
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

void msrPartgroup::printScoreSummary (ostream& os)
{
  int partgroupElementsSize = fPartgroupElements.size();
  
  os <<
    "Partgroup" << " " << getPartgroupCombinedName () <<
    " contains " << partgroupElementsSize;
  if (partgroupElementsSize == 1)
    os << " part or sub part group";
  else
    os << " parts or sub part groups";
  os << endl;

  idtr++;

  os <<
    idtr << "PartgroupName            : \"" <<
      fPartgroupName << "\"" << endl <<
    idtr << "PartgroupAbbrevation     : \"" <<
      fPartgroupAbbreviation << "\"" << endl;
  os <<
    idtr << "fPartgroupSymbolDefaultX : " <<
      fPartgroupSymbolDefaultX << endl;
  os <<
    idtr << "fPartgroupSymbolKind     : \"";
  switch (fPartgroupSymbolKind) {
    case kBracePartgroupSymbol:
      os << "brace";
      break;
    case kBracketPartgroupSymbol:
      os << "bracket";
      break;
    case kLinePartgroupSymbol:
      os << "line";
      break;
    case kSquarePartgroupSymbol:
      os << "square";
      break;
    case k_NoPartgroupSymbol:
      break;
  } // switch
  os << "\"" << endl;
  os <<
    idtr << "PartgroupBarline         : ";
  if (fPartgroupBarline)
    os << "true";
  else
    os << "false";
  os << endl;

  if (partgroupElementsSize) {
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

void msrPartgroup::printLilyPondCode (ostream& os)
{
  os <<
    "Partgroup" << " " << fPartgroupNumber << endl;

  idtr++;
// JMI  os << fPartgroupMsrSequentialMusic << endl;
  idtr--;

  os << idtr << "}" << endl;
}

//______________________________________________________________________________
S_msrScore msrScore::create (
  S_msrOptions& msrOpts, 
  int                    inputLineNumber)
{
  msrScore* o =
    new msrScore (
      msrOpts, inputLineNumber);
  assert(o!=0);
  return o;
}

msrScore::msrScore (
  S_msrOptions& msrOpts, 
  int                    inputLineNumber)
    : msrElement (msrOpts, inputLineNumber)
{
  fMsrOptions = msrOpts;
}
msrScore::~msrScore() {}

void msrScore::addPartgroupToScore (S_msrPartgroup partGroup)
{
  /* JMI
  if (fScorePartgroupsMap.count (partGroupNumber)) {
    cerr << idtr <<
      "### Internal error: part group " << partGroupNumber <<
      " already exists in this score" << endl;

    return fScorePartgroupsMap [partGroupNumber];
  }
*/

  // register it in this score
  fPartgroupsList.push_back (partGroup);
}

/*
S_msrPartgroup msrScore::fetchScorePartgroup (
  int partGroupNumber)
{
  S_msrPartgroup result;
  
  if (fScorePartgroupsMap.count (partGroupNumber)) {
    result = fScorePartgroupsMap [partGroupNumber];
  }

  return result;
}
*/

void msrScore::acceptIn (basevisitor* v) {
  if (fMsrOptions->fDebug)
    cerr << idtr <<
      "==> msrScore::acceptIn()" << endl;
      
  if (visitor<S_msrScore>*
    p =
      dynamic_cast<visitor<S_msrScore>*> (v)) {
        S_msrScore elem = this;
        
        if (fMsrOptions->fDebug)
          cerr << idtr <<
            "==> Launching msrScore::visitStart()" << endl;
        p->visitStart (elem);
  }
}

void msrScore::acceptOut (basevisitor* v) {
  if (fMsrOptions->fDebug)
    cerr << idtr <<
      "==> msrScore::acceptOut()" << endl;

  if (visitor<S_msrScore>*
    p =
      dynamic_cast<visitor<S_msrScore>*> (v)) {
        S_msrScore elem = this;
      
        if (fMsrOptions->fDebug)
          cerr << idtr <<
            "==> Launching msrScore::visitEnd()" << endl;
        p->visitEnd (elem);
  }
}

void msrScore::browseData (basevisitor* v)
{
  if (fMsrOptions->fDebug)
    cerr << idtr <<
      "==> msrScore::browseData()" << endl;
  
  idtr++;
  
  for (
    list<S_msrPartgroup>::iterator i = fPartgroupsList.begin();
    i != fPartgroupsList.end();
    i++) {
    // create the part group browser
    msrBrowser<msrPartgroup> browser (v);
  
    // browse the part group with the visitor
    browser.browse (*(*i));
  } // for

  idtr--;

  if (fMsrOptions->fDebug)
    cerr << idtr <<
      "<== msrScore::browseData()" << endl;
  }

ostream& operator<< (ostream& os, const S_msrScore& elt)
{
  elt->print(os);
  return os;
}

void msrScore::printMusicXML (ostream& os)
{
  os << "<!-- msrScore??? -->" << endl;
}

void msrScore::printMSR (ostream& os)
{
  os << "Score" << endl;
  os << endl;

  idtr++;
  
  for (
    list<S_msrPartgroup>::iterator i = fPartgroupsList.begin();
    i != fPartgroupsList.end();
    i++) {
    os << idtr << (*i);
  } // for
  
  idtr--;
}

void msrScore::printScoreSummary (ostream& os)
{
  int partgroupsListSize = fPartgroupsList.size();
  
  os <<
    "Score" <<
    " contains " << partgroupsListSize;
  if (partgroupsListSize == 1)
    os << " part group";
  else
    os << " part groups";
  os << endl;

  idtr++;
  
  list<S_msrPartgroup>::const_iterator
    iBegin = fPartgroupsList.begin(),
    iEnd   = fPartgroupsList.end(),
    i      = iBegin;
    
  idtr++;
  for ( ; ; ) {
    os << idtr << (*i);
    if (++i == iEnd) break;
    os << endl;
  } // for
  
  idtr--;
}

void msrScore::printLilyPondCode (ostream& os)
{
  printMSR (os);
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
  if (fMsrOptions->fDebug)
    cerr << idtr <<
      "==> msrMidi::acceptIn()" << endl;
      
  if (visitor<S_msrMidi>*
    p =
      dynamic_cast<visitor<S_msrMidi>*> (v)) {
        S_msrMidi elem = this;
        
        if (fMsrOptions->fDebug)
          cerr << idtr <<
            "==> Launching msrMidi::visitStart()" << endl;
        p->visitStart (elem);
  }
}

void msrMidi::acceptOut (basevisitor* v) {
  if (fMsrOptions->fDebug)
    cerr << idtr <<
      "==> msrMidi::acceptOut()" << endl;

  if (visitor<S_msrMidi>*
    p =
      dynamic_cast<visitor<S_msrMidi>*> (v)) {
        S_msrMidi elem = this;
      
        if (fMsrOptions->fDebug)
          cerr << idtr <<
            "==> Launching msrMidi::visitEnd()" << endl;
        p->visitEnd (elem);
  }
}


void msrMidi::browseData (basevisitor* v)
{}

ostream& operator<< (ostream& os, const S_msrMidi& mid)
{
  mid->print(os);
  return os;
}

void msrMidi::printMusicXML (ostream& os)
{
  os << "<!-- msrMidi??? -->" << endl;
}

void msrMidi::printMSR (ostream& os)
{
  os << "Midi" << endl;

  idtr++;
  
  os << idtr << "% to be completed" << endl;
  
  idtr--;
}

void msrMidi::printScoreSummary (ostream& os)
{}

void msrMidi::printLilyPondCode (ostream& os)
{  
  os << idtr << "\\midi {" << endl;
  
  idtr++;
  
  os << idtr << "% to be completed" << endl;
  
  idtr--;
  
  os << idtr << "}" << endl;
}


}
