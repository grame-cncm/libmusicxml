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

msrGlobalVariables::CodeGenerationKind
  msrGlobalVariables::sCodeGenerationKind =
    msrGlobalVariables::kLilypondCode;

indenter msrElement::idtr;

//______________________________________________________________________________
S_msrElement msrElement::create(bool debug)
{
  msrElement * o = new msrElement(debug); assert(o!=0);
  return o; 
}

msrElement::msrElement(bool debug)
{
  fDebug = debug;
}
msrElement::~msrElement() {}

ostream& operator<< (ostream& os, const S_msrElement& elt)
{
  elt->print(os);
  return os;
}

void msrElement::print(ostream& os)
{
  // a global variable is needed so that msr::Element.print() 
  // can decide whether to print:
  //   - the MSR structure
  //   - MusicXML text
  //   - LilyPond source code
switch (msrGlobalVariables::getCodeGenerationKind()) {
    case msrGlobalVariables::kMSR:
      this->printMSR (os);
      break;
    case msrGlobalVariables::kMusicXML:
      this->printMusicXML (os);
      break;
    case msrGlobalVariables::kLilypondCode:
      this->printLilyPondCode (os);
      break;
  } // switch
}

void msrElement::printMusicXML(ostream& os)
{
  os << "<!-- msrElement??? -->" << endl;
}

void msrElement::printMSR(ostream& os)
{
  os << "Element???" << endl;
}

void msrElement::printLilyPondCode(ostream& os)
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
    "  " << left << setw(26) << "fMusicxmlStep = " << fMusicxmlStep <<  endl <<
    "  " << left << setw(26) << "fMusicxmlStepIsARest = " << fMusicxmlStepIsARest <<  endl <<
    "  " << left << setw(26) << "fMusicxmlAlteration = " << fMusicxmlAlteration <<  endl <<
    "  " << left << setw(26) << "fMusicxmlOctave = " << fMusicxmlOctave <<  endl <<
    "  " << left << setw(26) << "fMusicxmlDivisions = " << fMusicxmlDivisions <<  endl <<
    "  " << left << setw(26) << "fMusicxmlDuration = " << fMusicxmlDuration <<  endl <<
    "  " << left << setw(26) << "fDotsNumber = " << fDotsNumber <<  endl <<
    "  " << endl <<    
    "  " << left << setw(26) << "fNoteBelongsToAChord = " << fNoteBelongsToAChord <<  endl <<
    "  " << endl <<
    "  " << left << setw(26) << "fNoteBelongsToATuplet = " << fNoteBelongsToATuplet <<  endl <<
    "  " << left << setw(26) << "fTupletMemberNoteType = " << fTupletMemberNoteType <<  endl <<
    "  " << endl <<
    "  " << left << setw(26) << "fVoiceNumber = " << fVoiceNumber <<  endl;
};

void musicXMLBeatData::print (ostream& os)
{
  os <<
    "  " << left << setw(26) << "fBeatUnit = " << fBeatUnit <<  endl <<
    "  " << left << setw(26) << "fDots = " << fDots <<  endl;
};

//______________________________________________________________________________
S_msrAbsoluteOctave msrAbsoluteOctave::create (int musicxmlOctave)
{
  msrAbsoluteOctave * o = new msrAbsoluteOctave (musicxmlOctave);
  assert(o!=0); 
  return o;
}

msrAbsoluteOctave::msrAbsoluteOctave (int musicxmlOctave)
  : msrElement("")
{
  fMsrOctave = musicxmlOctave - 3;
  /*
  cout <<
    "msrAbsoluteOctave::msrAbsoluteOctave (), musicxmlOctave = " << musicxmlOctave << 
    ", fMsrOctave = " << fMsrOctave << endl;
    */
}
msrAbsoluteOctave::~msrAbsoluteOctave () {}

ostream& operator<< (ostream& os, const S_msrAbsoluteOctave& dur)
{
  dur->print(os);
  return os;
}

void msrAbsoluteOctave::printMusicXML (ostream& os)
{
  os << "<!-- msrAbsoluteOctave??? -->" << endl;
}

string msrAbsoluteOctave::absoluteOctaveAsLilypondString ()
{
  stringstream s;
  string  result;
  
  s << fMsrOctave;
  s >> result;
  return result;
}

void msrAbsoluteOctave::printMSR(ostream& os)
{
  this->printLilyPondCode (os);
}

void msrAbsoluteOctave::printLilyPondCode(ostream& os)
{
  /*
  enum NoteFigures = {
    k1024th, k512th, 256th, k128th, k64th, k32nd, k16th, 
    kEighth, kQuarter, kHalf, kWhole, kBreve, kLong, kMaxima};
*/

  os << absoluteOctaveAsLilypondString ();
}

//______________________________________________________________________________
S_msrDuration msrDuration::create (
  int     num,
  int    denom,
  int    dots,
  string tupletMemberType)
{
  msrDuration * o =
    new msrDuration (num, denom, dots, tupletMemberType);
  assert(o!=0); 
  return o;
}

msrDuration::msrDuration (
  int    num,
  int    denom,
  int    dots,
  string tupletMemberType)
    : msrElement("")
{
  fNum   = num;
  fDenom = denom;
  fDots  = dots;
  fTupletMemberNoteType = tupletMemberType;
  /*
  cout <<
    "msrDuration::msrDuration (), fNum = " << fNum << 
    ", fDenom = " << fDenom << ", fDots = " << fDots << endl;
  */
}
msrDuration::~msrDuration() {}

void msrDuration::scaleNumByFraction (int num, int denom)
{
  fNum *= num/denom;
}

ostream& operator<< (ostream& os, const S_msrDuration& dur)
{
  dur->print(os);
  return os;
}

void msrDuration::printMusicXML(ostream& os)
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
//  cout << "|"  << fNum << "|" << fDenom << "|" << fDots << "|" << endl;

  int noteDivisions         = fNum;
  int divisionsPerWholeNote = fDenom ;
  
  if (divisionsPerWholeNote == 0) {
    stringstream s;
    string  message;
    s << 
      endl << 
      "%--> msrDuration::printLilyPondCode, noteDivisions = " << noteDivisions <<
      ", divisionsPerWholeNote = " << divisionsPerWholeNote <<
      endl;
    s >> message;
    msrMusicXMLError(message);
  }

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
      string       message;
      s << 
        endl << 
        "--> unknown tuplet member type " << fTupletMemberNoteType <<
        endl;
      s >> message;
      msrMusicXMLError(message);
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
        string  message;
        s <<
          "*** ERROR, MusicXML note duration " << noteDivisions << "/" << 
          divisionsPerWholeNote << " is too large" << endl;
        s >> message;
        msrMusicXMLError(message);
        }
    } // switch
  }
  
  //cout << "--> fDots = " << fDots << endl;
  
  // print the dots if any 
  int n = fDots; 
  if (n > 0) {
    while (n-- > 0) {
      s << ".";  
    } // while
  }
  
  string  result;
  
  s >> result;
  return result;
}

void msrDuration::printMSR(ostream& os)
{
  os << durationAsMSRString () << flush;
}

void msrDuration::printLilyPondCode(ostream& os)
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
  ArticulationKind articulationKind)
{
  msrArticulation* o =
    new msrArticulation (articulationKind);
  assert (o!=0);
  return o;
}

msrArticulation::msrArticulation (
  ArticulationKind articulationKind)
    : msrElement("")
{
  fArticulationKind = articulationKind;
}

msrArticulation::~msrArticulation() {}


ostream& operator<< (ostream& os, const S_msrArticulation& elt)
{
  elt->print(os);
  return os;
}

void msrArticulation::printMusicXML(ostream& os)
{
  os << "<!-- msrTime??? -->" << endl;
}

void msrArticulation::printMSR(ostream& os)
{
  os << "Articulation" << " ";

  switch (fArticulationKind) {
    case kStaccato:
      os << "staccato";
      break;
    case kStaccatissimo:
      os << "staccatissimo";
      break;
  } // switch
  
  os << endl;
}

void msrArticulation::printLilyPondCode(ostream& os)
{
  switch (fArticulationKind) {
    case kStaccato:
      os << "-.";
      break;
    case kStaccatissimo:
      os << "-^";
      break;
  } // switch
}

//______________________________________________________________________________
S_msrDynamics msrDynamics::create(DynamicsKind dynamicsKind)
{
  msrDynamics* o = new msrDynamics(dynamicsKind); assert(o!=0);
  return o;
}

msrDynamics::msrDynamics(DynamicsKind dynamicsKind)
  : msrElement("")
{
  fDynamicsKind = dynamicsKind; 
}
msrDynamics::~msrDynamics() {}

ostream& operator<< (ostream& os, const S_msrDynamics& dyn)
{
  dyn->print(os);
  return os;
}

string msrDynamics::dynamicsKindAsLilypondString ()
{
  stringstream s;
  
  switch (fDynamicsKind) {
    case kF:
      s << "\\f";
      break;
    case kFF:
      s << "\\ff"; break;
    case kFFF:
      s << "\\fff"; break;
    case kFFFF:
      s << "\\ffff"; break;
    case kFFFFF:
      s << "\\fffff"; break;
    case kFFFFFF:
      s << "\\ffffff"; break;

    case kP:
      s << "\\p"; break;
    case kPP:
      s << "\\pp"; break;
    case kPPP:
      s << "\\ppp"; break;
    case kPPPP:
      s << "\\pppp"; break;
    case kPPPPP:
      s << "\\ppppp"; break;
    case kPPPPPP:
      s << "\\pppppp"; break;

     case kMF:
      s << "\\mf"; break;
    case kMP:
      s << "\\mp"; break;
    case kFP:
      s << "\\fp"; break;
    case kFZ:
      s << "\\fz"; break;
    case kRF:
      s << "\\rf"; break;
    case kSF:
      s << "\\sf"; break;

   case kRFZ:
      s << "\\rfz"; break;
    case kSFZ:
      s << "\\sfz"; break;
    case kSFP:
      s << "\\sfp"; break;
    case kSFPP:
      s << "\\sfpp"; break;
    case kSFFZ:
      s << "\\sffz"; break;

    default:
      {
      stringstream s;
      string  message;
      s << "Dynamics " << fDynamicsKind << " is unknown";
      s >> message;
      msrMusicXMLError(message);
      }
  } // switch
  
  string result;
  
  s >> result;
  return result;
}

void msrDynamics::printMusicXML(ostream& os)
{
  os << "<!-- msrDynamics??? -->" << endl;
}

void msrDynamics::printMSR(ostream& os)
{
  os <<
    "Dynamics" << " " << dynamicsKindAsLilypondString () << endl;
}

void msrDynamics::printLilyPondCode(ostream& os)
{
  os << dynamicsKindAsLilypondString ();
}

//______________________________________________________________________________
S_msrWedge msrWedge::create(WedgeKind wedgeKind)
{
  msrWedge* o = new msrWedge(wedgeKind); assert(o!=0);
  return o;
}

msrWedge::msrWedge(WedgeKind wedgeKind)
  : msrElement("")
{
  fWedgeKind=wedgeKind; 
}
msrWedge::~msrWedge() {}

ostream& operator<< (ostream& os, const S_msrWedge& wdg)
{
  wdg->print(os);
  return os;
}

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
  
  string result;
  
  s >> result;
  return result;
}

void msrWedge::printMusicXML(ostream& os)
{
  os << "<!-- msrWedge??? -->" << endl;
}

void msrWedge::printMSR(ostream& os)
{
  os << "Wedge" << " " << wedgeKindAsString () << endl;
}

void msrWedge::printLilyPondCode(ostream& os)
{
  os << wedgeKindAsString ();
}

//______________________________________________________________________________
S_msrSlur msrSlur::create(SlurKind slurKind)
{
  msrSlur* o = new msrSlur(slurKind); assert(o!=0);
  return o;
}

msrSlur::msrSlur(SlurKind slurKind)
  : msrElement("")
{
  fSlurKind=slurKind; 
}
msrSlur::~msrSlur() {}

ostream& operator<< (ostream& os, const S_msrSlur& wdg)
{
  wdg->print(os);
  return os;
}

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
  
  string result;
  
  s >> result;
  return result;
}

void msrSlur::printMusicXML(ostream& os)
{
  os << "<!-- msrSlur??? -->" << endl;
}

void msrSlur::printMSR(ostream& os)
{
  os << "Slur" << " " << slurKindAsString () << endl;
}

void msrSlur::printLilyPondCode(ostream& os)
{
  os << slurKindAsString ();
}

//______________________________________________________________________________
S_msrNote msrNote::createFromMusicXMLData (
  S_translationSettings& ts,
  musicXMLNoteData&      mxmldat,
  msrSlur::SlurKind      slurKind)
{  
  msrNote * o = new msrNote (ts, mxmldat, slurKind);
  assert(o!=0); 
  return o;
}

msrNote::msrNote (
  S_translationSettings& ts,
  musicXMLNoteData&      mxmldat,
  msrSlur::SlurKind      slurKind)
  :
    msrElement(""),
    fMusicXMLNoteData (mxmldat)
{
  fTranslationSettings = ts;

  fNoteSlurKind = slurKind;

//  if (true || fTranslationSettings->fDebug) {
  if (fTranslationSettings->fDebug) {
    cout << "==> fMusicXMLNoteData:" << endl;
    cout << fMusicXMLNoteData << endl;
  }
    
  // take rests into account
  if (fMusicXMLNoteData.fMusicxmlStepIsARest) {
    /*
    cout <<
      "--> REST, fMusicxmlDuration/fMusicxmlDivisions = " <<
      fMusicXMLNoteData.fMusicxmlDuration << 
     "/" <<
     fMusicXMLNoteData.fMusicxmlDivisions << endl;
    */
    fMusicXMLDiatonicPitch = msrNote::kRest;
  }

  if (
    fMusicXMLNoteData.fMusicxmlStep < 'A'
      ||
    fMusicXMLNoteData.fMusicxmlStep > 'G') {
    if (! fMusicXMLNoteData.fMusicxmlStepIsARest) {
      stringstream s;
      string       message;
      s <<
        "step value " << fMusicXMLNoteData.fMusicxmlStep <<
        " is not a letter from A to G";
      s >> message;
    //  msrMusicXMLError (message);
    msrMusicXMLWarning (message); // JMI
    }
  }

//  cout << "=== xmlPart2MsrVisitor::visitStart ( S_step& elt ) " << fCurrentMusicXMLStep << endl;
// JMI

  switch (fMusicXMLNoteData.fMusicxmlStep) {
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
  
  switch (fMusicXMLNoteData.fMusicxmlStep) {
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
  cout <<
    "--> fMusicXMLNoteData.fMusicxmlAlteration = " <<
    fMusicXMLNoteData.fMusicxmlAlteration <<  endl;
*/
  
  switch (fMusicXMLNoteData.fMusicxmlAlteration) {
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
      string       message;
      s <<
        "MusicXML alteration " << fMusicXMLNoteData.fMusicxmlAlteration <<
        " is not between -2 and +2";
      s >> message;
      cout << "message = |" << message << "|" << endl;
      
      msrAssert(
        fMusicXMLNoteData.fMusicxmlAlteration>=-2
          &&
        fMusicXMLNoteData.fMusicxmlAlteration<=+2,
        message);
      }
   } // switch

  fNoteMsrPitch = 
    computeNoteMsrPitch (noteQuatertonesFromA, mxmlAlteration);
  
  int divisionsPerWholeNote = fMusicXMLNoteData.fMusicxmlDivisions*4;
  
//  if (true || fTranslationSettings->fDebug)
  if (fTranslationSettings->fDebug)
    cerr << 
      "--> fMusicXMLNoteData.fMusicxmlDivisions = " <<
      fMusicXMLNoteData.fMusicxmlDivisions << ", " << 
      "divisionsPerWholeNote = " << divisionsPerWholeNote << endl;
    
  msrAssert(
    divisionsPerWholeNote > 0,
    "The MusicMXL divisions per quater note value should be positive");

  fNoteMsrDuration =
    msrDuration::create (
      fMusicXMLNoteData.fMusicxmlDuration,
      divisionsPerWholeNote,
      fMusicXMLNoteData.fDotsNumber,
      fMusicXMLNoteData.fTupletMemberNoteType);
//  cout << "fNoteMsrDuration = " << fNoteMsrDuration << endl;
    
  // diatonic note for relative code JMI
  msrNote::MusicXMLDiatonicPitch diatonicNote =
    msrNote::k_NoDiatonicPitch;
}

msrNote::~msrNote() {}

void msrNote::setNoteBelongsToAChord () {
  fMusicXMLNoteData.fNoteBelongsToAChord = true;
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

ostream& operator<< (ostream& os, const S_msrNote& elt)
{
  elt->print(os);
  return os;
}

string msrNote::notePitchAsLilypondString ()
{
  stringstream s;
  
  /*
  cout << "msrNote::notePitchAsLilypondString (), isRest = " <<
    fMusicXMLNoteData.fMusicxmlStepIsARest <<
    ", fMsrPitch = " << fMsrPitch << endl;
  */
  
  if (fMusicXMLNoteData.fMusicxmlStepIsARest)
    s << "r";

  else {
    //JMI assertMsr(fMsrPitch != k_NoMsrPitch, "fMsrPitch != k_NoMsrPitch");

    if (fMusicXMLNoteData.fMusicxmlStepIsUnpitched)
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

    s << fMusicXMLNoteData.fMusicxmlOctave;
  }

  string  result;
  
  s >> result;
  return result;
}

void msrNote::printMusicXML(ostream& os)
{
  os << "<!-- msrNote??? -->" << endl;
}

void msrNote::printMSR(ostream& os)
{
  /*
  cout <<
    "msrNote::printMSR(), fNoteBelongsToAChord = " << 
    fNoteBelongsToAChord << endl;
  */
  
  if (fMusicXMLNoteData.fNoteBelongsToAChord) {

    // do not print it, msrChord::printMSR() will do it
    os << notePitchAsLilypondString() << " (FOO) ";

  } else {

    if (fMusicXMLNoteData.fMusicxmlStepIsARest)
      os << "Rest";
    else
      os << "Note";
    os <<
      " " << notePitchAsLilypondString () <<
      ":" << fNoteMsrDuration;
    if (fMusicXMLNoteData.fNoteIsAGraceNote)
      os << " " << "grace";
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

void msrNote::printLilyPondCode(ostream& os)
{
  // print the note name
  os << notePitchAsLilypondString ();
  
  if (! fMusicXMLNoteData.fNoteBelongsToAChord) {
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
S_msrSequence msrSequence::create (ElementsSeparator elementsSeparator)
{
  msrSequence* o = new msrSequence (elementsSeparator); assert(o!=0);
  return o;
}

msrSequence::msrSequence (ElementsSeparator elementsSeparator)
  : msrElement("")
{
  fElementsSeparator=elementsSeparator;
}
msrSequence::~msrSequence() {}

void msrSequence::removeElementFromSequence (S_msrElement elem)
{
  for (
    msrElementList::iterator i = fSequenceElements.begin();
    i != fSequenceElements.end();
    i++) {
    if ((*i) == elem) {
      fSequenceElements.erase (i);
      break;
    }
  } // for
}

ostream& operator<< (ostream& os, const S_msrSequence& elt)
{
  elt->print(os);
  return os;
}

void msrSequence::printMSR(ostream& os)
{  
  os << "Sequence";
  
  if (fSequenceElements.size()) {
    os << endl;

    idtr++;
  
    list<S_msrElement>::const_iterator
      iBegin = fSequenceElements.begin(),
      iEnd   = fSequenceElements.end(),
      i      = iBegin;
    for ( ; ; ) {
      os << idtr << (*i);
      if (++i == iEnd) break;
      if (fElementsSeparator == kEndOfLine) os << endl;
    } // for
    
    idtr--;
  }
}

void msrSequence::printMusicXML(ostream& os)
{
  os << "<!-- msrSequence??? -->" << endl;
}

void msrSequence::printLilyPondCode(ostream& os)
{
  if (fSequenceElements.size()) {
    list<S_msrElement>::const_iterator
      iBegin = fSequenceElements.begin(),
      iEnd   = fSequenceElements.end(),
      i      = iBegin;
    for ( ; ; ) {
      os << idtr << (*i);
      if (++i == iEnd) break;
      if (fElementsSeparator == kEndOfLine) os << endl;
    } // for
  }
}

//______________________________________________________________________________
S_msrParallelMusic msrParallelMusic::create(ElementsSeparator elementsSeparator)
{
  msrParallelMusic* o = new msrParallelMusic(elementsSeparator); assert(o!=0);
  return o;
}

msrParallelMusic::msrParallelMusic(ElementsSeparator elementsSeparator)
  : msrElement("")
{
  fElementsSeparator=elementsSeparator;
}
msrParallelMusic::~msrParallelMusic() {}

ostream& operator<< (ostream& os, const S_msrParallelMusic& elt)
{
  elt->print(os);
  return os;
}

void msrParallelMusic::printMusicXML(ostream& os)
{
  os << "<!-- msrParallelMusic??? -->" << endl;
}

void msrParallelMusic::printMSR(ostream& os)
{
  os << "ParallelMusic" << endl;
  
  idtr++;
  
  int size = fParallelMusicElements.size();
  
  for (int i = 0; i < size; i++ ) {
    os << idtr << fParallelMusicElements[i];
  } // for
  
  idtr--;
}

void msrParallelMusic::printLilyPondCode(ostream& os)
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
S_msrChord msrChord::create (S_msrDuration chordDuration)
{
  msrChord* o = new msrChord(chordDuration); assert(o!=0);
  return o;
}

msrChord::msrChord (S_msrDuration chordDuration)
  : msrElement("")
{
  fChordDuration = chordDuration;
}
msrChord::~msrChord() {}

ostream& operator<< (ostream& os, const S_msrChord& chrd)
{
  chrd->print(os);
  return os;
}

void msrChord::printMusicXML(ostream& os)
{
  os << "<!-- msrChord??? -->" << endl;
}

void msrChord::printMSR(ostream& os)
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

void msrChord::printLilyPondCode(ostream& os)
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
S_msrBarLine msrBarLine::create (int nextBarNumber)
{
  msrBarLine* o = new msrBarLine (nextBarNumber); assert(o!=0);
  return o;
}

msrBarLine::msrBarLine (int nextBarNumber)
  : msrElement("")
{
  fNextBarNumber=nextBarNumber; 
}
msrBarLine::~msrBarLine() {}

ostream& operator<< (ostream& os, const S_msrBarLine& elt)
{
  elt->print(os);
  return os;
}

void msrBarLine::printMusicXML(ostream& os)
{
  os << "<!-- msrBarLine??? -->" << endl;
}

void msrBarLine::printMSR(ostream& os)
{
  os << "BarLine" << " " << fNextBarNumber << endl;
}

void msrBarLine::printLilyPondCode(ostream& os)
{
  os << "| % " << fNextBarNumber << endl;
}

//______________________________________________________________________________
S_msrComment msrComment::create(string contents, GapKind gapKind)
{
  msrComment* o = new msrComment(contents, gapKind); assert(o!=0);
  return o;
}

msrComment::msrComment(string contents, GapKind gapKind)
  : msrElement("")
{
  fContents=contents;
  fGapKind=gapKind;
}
msrComment::~msrComment() {}

ostream& operator<< (ostream& os, const S_msrComment& elt)
{
  elt->print(os);
  return os;
}

void msrComment::printMusicXML(ostream& os)
{
  os << "<!-- msrComment??? -->" << endl;
}

void msrComment::printMSR(ostream& os)
{
  os << "Comment" << endl;
  idtr++;
  os << idtr << "% " << fContents << endl;
  if (fGapKind == kGapAfterwards) os << idtr << endl;
  idtr--;
}

void msrComment::printLilyPondCode(ostream& os)
{
  os << "% " << fContents;
  if (fGapKind == kGapAfterwards) os << endl;
}

//______________________________________________________________________________
S_msrBreak msrBreak::create(int nextBarNumber)
{
  msrBreak* o = new msrBreak(nextBarNumber); assert(o!=0);
  return o;
}

msrBreak::msrBreak(int nextBarNumber)
  : msrElement("")
{
  fNextBarNumber=nextBarNumber; 
}
msrBreak::~msrBreak() {}

ostream& operator<< (ostream& os, const S_msrBreak& elt)
{
  elt->print(os);
  return os;
}

void msrBreak::printMusicXML(ostream& os)
{
  os << "<!-- msrBreak??? -->" << endl;
}

void msrBreak::printMSR(ostream& os)
{
  os <<
    "Break" << " " << fNextBarNumber << endl <<
    endl;
}

void msrBreak::printLilyPondCode(ostream& os)
{
  os <<
    "\\myBreak | % " << fNextBarNumber << endl <<
    endl;
}

//______________________________________________________________________________
S_msrBarNumberCheck msrBarNumberCheck::create(int nextBarNumber)
{
  msrBarNumberCheck* o = new msrBarNumberCheck(nextBarNumber); assert(o!=0);
  return o;
}

msrBarNumberCheck::msrBarNumberCheck(int nextBarNumber)
  : msrElement("")
{
  fNextBarNumber=nextBarNumber; 
}
msrBarNumberCheck::~msrBarNumberCheck() {}

ostream& operator<< (ostream& os, const S_msrBarNumberCheck& elt)
{
  elt->print(os);
  return os;
}

void msrBarNumberCheck::printMusicXML(ostream& os)
{
  os << "<!-- msrBarNumberCheck??? -->" << endl;
}

void msrBarNumberCheck::printMSR(ostream& os)
{
  os << "BarNumberCheck" << " " << fNextBarNumber << endl;
}

void msrBarNumberCheck::printLilyPondCode(ostream& os)
{
  os << "\\barNumberCheck #" << fNextBarNumber << endl;
}

//______________________________________________________________________________
S_msrTuplet msrTuplet::create()
{
  msrTuplet* o = new msrTuplet(); assert(o!=0);
  return o;
}

msrTuplet::msrTuplet()
  : msrElement("")
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

ostream& operator<< (ostream& os, const S_msrTuplet& elt)
{
  elt->print(os);
  return os;
}

void msrTuplet::printMusicXML(ostream& os)
{
  os << "<!-- msrTuplet??? -->" << endl;
}

void msrTuplet::printMSR(ostream& os)
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

void msrTuplet::printLilyPondCode(ostream& os)
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
S_msrBeam msrBeam::create(int number, BeamKind beamKind)
{
  msrBeam* o = new msrBeam(number, beamKind); assert(o!=0);
  return o;
}

msrBeam::msrBeam(int number, BeamKind beamKind)
  : msrElement("")
{
  fBeamNumber = number;
  fBeamKind   = beamKind; 
}
msrBeam::~msrBeam() {}

ostream& operator<< (ostream& os, const S_msrBeam& dyn)
{
  dyn->print(os);
  return os;
}

void msrBeam::printMusicXML(ostream& os)
{
  os << "<!-- msrBeam??? -->" << endl;
}

void msrBeam::printMSR(ostream& os)
{
  os << "Beam???" << endl;
}

void msrBeam::printLilyPondCode(ostream& os)
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
S_msrPaper msrPaper::create() {
  msrPaper* o = new msrPaper(); assert(o!=0);
  return o;
}

msrPaper::msrPaper()
  : msrElement("")
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

ostream& operator<< (ostream& os, const S_msrPaper& pap) {
  pap->print(os);
  return os;
}

void msrPaper::printMusicXML(ostream& os)
{
  os << "<!-- msrPaper??? -->" << endl;
}

void msrPaper::printMSR(ostream& os) {
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

void msrPaper::printLilyPondCode(ostream& os)
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
S_msrHeader msrHeader::create() {
  msrHeader* o = new msrHeader(); assert(o!=0);
  return o;
}

msrHeader::msrHeader() : msrElement("") {}
msrHeader::~msrHeader() {}

void msrHeader::setWorkNumber (string val)
  {
  fWorkNumber =
    msrLilypondVarValAssoc::create (
      "work-number", val,
      msrLilypondVarValAssoc::kEqualSign,
      msrLilypondVarValAssoc::kQuotesAroundValue,
      msrLilypondVarValAssoc::kUncommented);
  }
S_msrLilypondVarValAssoc msrHeader::getWorkNumber () const
 { return fWorkNumber; }

void msrHeader::setWorkTitle (string val)
  {
  fWorkTitle =
    msrLilypondVarValAssoc::create (
      "work-title", val,
      msrLilypondVarValAssoc::kEqualSign,
      msrLilypondVarValAssoc::kQuotesAroundValue,
      msrLilypondVarValAssoc::kUncommented);
  }
S_msrLilypondVarValAssoc msrHeader::getWorkTitle () const
  { return fWorkTitle; }

void msrHeader::setMovementNumber (string val)
  {
  fMovementNumber =
    msrLilypondVarValAssoc::create (
      "movement-number", val,
      msrLilypondVarValAssoc::kEqualSign,
      msrLilypondVarValAssoc::kQuotesAroundValue,
      msrLilypondVarValAssoc::kUncommented);
  }
S_msrLilypondVarValAssoc msrHeader::getMovementNumber () const
  { return fMovementNumber; }

void msrHeader::setMovementTitle (string val)
{
  fMovementTitle =
    msrLilypondVarValAssoc::create (
      "movement-title", val,
      msrLilypondVarValAssoc::kEqualSign,
      msrLilypondVarValAssoc::kQuotesAroundValue,
      msrLilypondVarValAssoc::kUncommented);
}
S_msrLilypondVarValAssoc msrHeader::getMovementTitle () const
  { return fMovementTitle; }

void msrHeader::addCreator (string type, string val)
{
  fCreators.push_back(
    msrLilypondVarValAssoc::create (
      type, val,
      msrLilypondVarValAssoc::kEqualSign,
      msrLilypondVarValAssoc::kQuotesAroundValue,
      msrLilypondVarValAssoc::kUncommented)
  );
}
vector<S_msrLilypondVarValAssoc> msrHeader::getCreators () const
  { return fCreators; };

void msrHeader::setRights (string val)
  {
  fRights =
    msrLilypondVarValAssoc::create (
      "rights", val,
      msrLilypondVarValAssoc::kEqualSign,
      msrLilypondVarValAssoc::kQuotesAroundValue,
      msrLilypondVarValAssoc::kUncommented);
  }
S_msrLilypondVarValAssoc msrHeader::getRights () const
  { return fRights; }

void msrHeader::addSoftware (string val)
{
  fSoftwares.push_back(
    msrLilypondVarValAssoc::create (
      "software", val,
      msrLilypondVarValAssoc::kEqualSign,
      msrLilypondVarValAssoc::kQuotesAroundValue,
      msrLilypondVarValAssoc::kUncommented)
  );
}
vector<S_msrLilypondVarValAssoc> msrHeader::getSoftwares () const
  { return fSoftwares; };

void msrHeader::setEncodingDate (string val)
{
  fEncodingDate =
    msrLilypondVarValAssoc::create (
      "encodingdate", val,
      msrLilypondVarValAssoc::kEqualSign,
      msrLilypondVarValAssoc::kQuotesAroundValue,
      msrLilypondVarValAssoc::kUncommented);
}
S_msrLilypondVarValAssoc msrHeader::getEncodingDate () const
  { return fEncodingDate; }

void msrHeader::setScoreInstrument (string val)
{
  fScoreInstrument =
    msrLilypondVarValAssoc::create (
      "score-instrument", val,
      msrLilypondVarValAssoc::kEqualSign,
      msrLilypondVarValAssoc::kQuotesAroundValue,
      msrLilypondVarValAssoc::kUncommented);
}
S_msrLilypondVarValAssoc msrHeader::getScoreInstrument () const
  { return fScoreInstrument; }


void msrHeader::printMusicXML(ostream& os)
{
  os << "<!-- msrHeader??? -->" << endl;
}

void msrHeader::printMSR(ostream& os)
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
    vector<S_msrLilypondVarValAssoc>::const_iterator i1;
    for (i1=fCreators.begin(); i1!=fCreators.end(); i1++) {
      os << idtr << (*i1);
    } // for
  }
    
  if (fRights) {
    os << idtr << fRights;
  }
    
  if (!fSoftwares.empty()) {
    vector<S_msrLilypondVarValAssoc>::const_iterator i2;
    for (i2=fSoftwares.begin(); i2!=fSoftwares.end(); i2++) {
      os << idtr << (*i2);
    } // for
  }
    
  if (fEncodingDate) {
    os << idtr << fEncodingDate;
  }
  
  idtr--;
}

void msrHeader::printLilyPondCode(ostream& os)
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
    vector<S_msrLilypondVarValAssoc>::const_iterator i1;
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
    vector<S_msrLilypondVarValAssoc>::const_iterator i2;
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
S_msrLilypondVarValAssoc msrLilypondVarValAssoc::create(
      string     variableName,
      string     value, 
      VarValSeparator varValSeparator,
      QuotesKind      quotesKind,
      CommentedKind   commentedKind,
      string     unit)
{
  msrLilypondVarValAssoc* o =
    new msrLilypondVarValAssoc(
    variableName, value, varValSeparator, 
    quotesKind, commentedKind, unit);
  assert(o!=0);
  return o;
}

msrLilypondVarValAssoc::msrLilypondVarValAssoc(
      string     variableName,
      string     value, 
      VarValSeparator varValSeparator,
      QuotesKind      quotesKind,
      CommentedKind   commentedKind,
      string     unit)
  : msrElement("")
{
  fVariableName=variableName;
  fVariableValue=value;
  fVarValSeparator=varValSeparator;
  fQuotesKind=quotesKind;
  fCommentedKind=commentedKind;
  fUnit = unit;
}

msrLilypondVarValAssoc::~msrLilypondVarValAssoc() {}

void msrLilypondVarValAssoc::changeAssoc (string value) {
  fVariableValue=value;
}

ostream& operator<< (ostream& os, const S_msrLilypondVarValAssoc& assoc) {
  assoc->print(os);
  return os;
}

void msrLilypondVarValAssoc::printMusicXML(ostream& os)
{
  os << "<!-- msrLilypondVarValAssoc??? -->" << endl;
}

void msrLilypondVarValAssoc::printMSR(ostream& os)
{
  os << "LilypondVarValAssoc" << endl;
  idtr++;
  os << idtr << fVariableName << endl;
  os << idtr << fVariableValue <<endl;
  idtr--;
}

void msrLilypondVarValAssoc::printLilyPondCode(ostream& os) {
  if (fCommentedKind == kCommented) os << "\%";
  os << fVariableName;
  if (fVarValSeparator == kEqualSign) os << " = ";
  else os << " ";
  if (fQuotesKind == kQuotesAroundValue) os << "\"";
  os << fVariableValue << fUnit;
  if (fQuotesKind == kQuotesAroundValue) os << "\"";
  os << endl;
}

//______________________________________________________________________________
S_msrSchemeVarValAssoc msrSchemeVarValAssoc::create(
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
  : msrElement("")
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

ostream& operator<< (ostream& os, const S_msrSchemeVarValAssoc& assoc)
{
  assoc->print(os);
  return os;
}

void msrSchemeVarValAssoc::printMusicXML(ostream& os)
{
  os << "<!-- msrSchemeVarValAssoc??? -->" << endl;
}

void msrSchemeVarValAssoc::printMSR(ostream& os)
{
  os << "SchemeVarValAssoc" << endl;
  idtr++;
  os << idtr << fVariableName << endl;
  os << idtr << fVariableValue <<endl;
  idtr--;
}

void msrSchemeVarValAssoc::printLilyPondCode(ostream& os)
{
  if (fCommentedKind == kCommented) os << "\%";
  os <<
    "#(" << 
    fVariableName << " " << fVariableValue << 
    ")" << endl;
}

//______________________________________________________________________________
S_msrLayout msrLayout::create() {
  msrLayout* o = new msrLayout(); assert(o!=0);
  return o;
}

msrLayout::msrLayout() : msrElement("") {}
msrLayout::~msrLayout() {}

ostream& operator<< (ostream& os, const S_msrLayout& lay)
{
  lay->print(os);
  return os;
}

void msrLayout::printMusicXML(ostream& os)
{
  os << "<!-- msrLayout??? -->" << endl;
}

void msrLayout::printMSR(ostream& os)
{
  os << "Layout" << endl;

  idtr++;

  int n1 = fMsrLilypondVarValAssocs.size();
  for (int i = 0; i < n1; i++ ) {
    os << idtr << fMsrLilypondVarValAssocs[i];
  } // for

    /* JMI
  int n2 = fMsrSchemeVarValAssocs.size();
  for (int i = 0; i < n2; i++ ) {
    os << idtr << fMsrSchemeVarValAssocs[i];
  } // for
  */
  
  idtr--;
}

void msrLayout::printLilyPondCode(ostream& os)
{  
  os << idtr << "\\layout {" << endl;

  idtr++;

  int n1 = fMsrLilypondVarValAssocs.size();
  for (int i = 0; i < n1; i++ ) {
    os << idtr << fMsrLilypondVarValAssocs[i];
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
  string sign,
  int    line,
  int    octaveChange)
{
  msrClef* o =
    new msrClef(sign, line, octaveChange); assert(o!=0);
  return o;
}

msrClef::msrClef(
  string sign,
  int    line,
  int    octaveChange)
  : msrElement("")
{
  fSign         = sign;
  fLine         = line;
  fOctaveChange = octaveChange;
}

msrClef::~msrClef() {}

ostream& operator<< (ostream& os, const S_msrClef& clef)
{
  clef->print(os);
  return os;
}

void msrClef::printMusicXML(ostream& os)
{
  os << "<!-- msrClef??? -->" << endl;
}

void msrClef::printMSR(ostream& os)
{
  os <<
    "Clef" << " \"" << fSign << "\"" <<
    " line " << fLine << ", " << fOctaveChange*8;
}

void msrClef::printLilyPondCode(ostream& os)
{
  stringstream s; 
  string       result;

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
  
  s >> result;
  
 // if (fLine != kStandardLine) 
    // s << fLine; // USER
//    s >> param;
    
  if (fOctaveChange == 1)
    result += "^8"; // USER
  else if (fOctaveChange == -1)
    result += "_8";

  os << "\\clef" << " \"" << result << "\"" << endl;
}

//______________________________________________________________________________
S_msrKey msrKey::create (
  int fifths, string mode, int cancel)
{
  msrKey* o = new msrKey (fifths, mode, cancel);
  assert (o!=0);
  return o;
}

msrKey::msrKey (
  int fifths, string mode, int cancel)
    : msrElement("")
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
      string  message;
      s << 
        "ERROR: unknown key sign \"" << fFifths << "\"" << endl;
      s >> message;
      msrMusicXMLError(message);
      }
  } // switch
  
  fTonic   = tonicNote;
  fKeyMode = keyMode; 
}

msrKey::~msrKey() {}

ostream& operator<< (ostream& os, const S_msrKey& key)
{
  key->print(os);
  return os;
}

void msrKey::printMusicXML(ostream& os)
{
  os << "<!-- msrKey??? -->" << endl;
}

void msrKey::printMSR(ostream& os)
{
  os << "Key " << fTonic << " ";
  if (fKeyMode == kMajor) os << "\\major";
  else os << "\\minor";
}

void msrKey::printLilyPondCode(ostream& os)
{
  os << "\\key " << fTonic << " ";
  if (fKeyMode == kMajor) os << "\\major";
  else os << "\\minor";
  os << endl;
}

//______________________________________________________________________________
S_msrTime msrTime::create (
  int numerator, int denominator, bool generateNumericalTime)
{
  msrTime* o =
    new msrTime (numerator, denominator, generateNumericalTime);
  assert (o!=0);
  return o;
}

msrTime::msrTime(
  int numerator, int denominator, bool generateNumericalTime)
  : msrElement("")
{
  fRational = rational (numerator, denominator);
// JMI  fNumerator=numerator; 
  //fDenominator=denominator;
  fGenerateNumericalTime = generateNumericalTime;
}
msrTime::~msrTime() {}


ostream& operator<< (ostream& os, const S_msrTime& elt)
{
  elt->print(os);
  return os;
}

void msrTime::printMusicXML(ostream& os)
{
  os << "<!-- msrTime??? -->" << endl;
}

void msrTime::printMSR(ostream& os)
{
  os <<
    "Time " << 
    fRational.getNumerator() << "/" << fRational.getDenominator();
}

void msrTime::printLilyPondCode(ostream& os)
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
S_msrMidi msrMidi::create()
{
  msrMidi* o = new msrMidi(); assert(o!=0);
  return o;
}

msrMidi::msrMidi()
  : msrElement("")
{
}
msrMidi::~msrMidi() {}

ostream& operator<< (ostream& os, const S_msrMidi& mid)
{
  mid->print(os);
  return os;
}

void msrMidi::printMusicXML(ostream& os)
{
  os << "<!-- msrMidi??? -->" << endl;
}

void msrMidi::printMSR(ostream& os)
{
  os << "Midi" << endl;

  idtr++;
  
  os << idtr << "% to be completed" << endl;
  
  idtr--;
}

void msrMidi::printLilyPondCode(ostream& os)
{  
  os << idtr << "\\midi {" << endl;
  
  idtr++;
  
  os << idtr << "% to be completed" << endl;
  
  idtr--;
  
  os << idtr << "}" << endl;
}

//______________________________________________________________________________
S_msrRepeat msrRepeat::create()
{
  msrRepeat* o = new msrRepeat(); assert(o!=0);
  return o;
}

msrRepeat::msrRepeat() : msrElement("")
{
  fActuallyUsed = false;
}
msrRepeat::~msrRepeat() {}

ostream& operator<< (ostream& os, const S_msrRepeat& rept)
{
  rept->print(os);
  return os;
}

void msrRepeat::printMusicXML(ostream& os)
{
  os << "<!-- msrRepeat??? -->" << endl;
}

void msrRepeat::printMSR(ostream& os)
{
  os << "Repeat" << endl;
  idtr++;
    os << idtr << fCommonPart;
    vector<S_msrSequence>::const_iterator i;
    for (i=fAlternateEndings.begin(); i!=fAlternateEndings.end(); i++) {
      os << idtr << (*i);
    } // for
  idtr--;
}

void msrRepeat::printLilyPondCode(ostream& os)
{
  os << "Repeat" << endl;
}

//______________________________________________________________________________
S_msrLyricsChunk msrLyricsChunk::create (
  LyricsChunkType chunkType,
  string          chunkText,
  S_msrDuration  duration)
{
  msrLyricsChunk* o =
    new msrLyricsChunk (chunkType, chunkText, duration);
  assert(o!=0);
  return o;
}

msrLyricsChunk::msrLyricsChunk (
  LyricsChunkType chunkType,
  string          chunkText,
  S_msrDuration  duration)
  : msrElement("")
{
  fLyricsChunkType = chunkType;
  fChunkText       = chunkText;
  fChunkDuration   = duration;
}
msrLyricsChunk::~msrLyricsChunk() {}

ostream& operator<< (ostream& os, const S_msrLyricsChunk& lyr)
{
  lyr->print(os);
  return os;
}

void msrLyricsChunk::printMusicXML(ostream& os)
{
  os << "<!-- msrLyricsChunk??? -->" << endl;
}

void msrLyricsChunk::printMSR(ostream& os)
{  
  os << "LyricsChunk" << " ";
  switch (fLyricsChunkType) {
    case kSingleChunk:
      os << "single" << " duration: " << fChunkDuration;
      if (fChunkText.size()) os << " " << "\"" << fChunkText << "\"";
      break;
    case kBeginChunk:
      os << "begin" << " duration: " << fChunkDuration;
      if (fChunkText.size()) os << " " << "\"" << fChunkText << "\"";
      break;
    case kMiddleChunk:
      os << "middle" << " duration: " << fChunkDuration;
      if (fChunkText.size()) os << " " << "\"" << fChunkText << "\"";
      break;
    case kEndChunk:
      os << "end" << " duration: " << fChunkDuration;
      if (fChunkText.size()) os << " " << "\"" << fChunkText << "\"";
      break;
      
    case kSkipChunk:
      os << "skip" << " duration: " << fChunkDuration;
      if (fChunkText.size()) os << " " << fChunkText;
      break;
      
    case kBreakChunk:
      os << "break" << " " << fChunkText << endl;
      break;
  } // switch
  os << endl;
}

void msrLyricsChunk::printLilyPondCode(ostream& os)
{  
  switch (fLyricsChunkType) {
    case kSingleChunk: os << fChunkText;           break;
    case kBeginChunk:  os << fChunkText;           break;
    case kMiddleChunk: os << " -- " << fChunkText; break;
    case kEndChunk:    os << " ++ " << fChunkText; break;
    case kSkipChunk:   os << "\\skip";             break;
    case kBreakChunk:
      os << "%{ " << fChunkText << " %}" << endl << idtr;
      break;
  } // switch
}

//______________________________________________________________________________
S_msrLyrics msrLyrics::create (
  S_translationSettings& ts,
  int                    lyricsNumber,
  S_msrVoice            lyricsVoice)
{
  msrLyrics* o = new msrLyrics (ts, lyricsNumber, lyricsVoice);
  assert(o!=0);
  return o;
}

msrLyrics::msrLyrics (
  S_translationSettings& ts,
  int                    lyricsNumber,
  S_msrVoice            lyricsVoice)
    : msrElement("")
{
  fTranslationSettings = ts;
  fLyricsNumber        = lyricsNumber;
  fLyricsVoice         = lyricsVoice; 
 
  fLyricsTextPresent = false;
}

string msrLyrics::getLyricsName () const
{
  // not stored in a field,
  // because the lyrics voice and staff may change name
  // when the part they belong to is re-used
  return
   fLyricsVoice->getVoiceName() +
    "_Lyrics_" +
    int2EnglishWord (fLyricsNumber);
}

msrLyrics::~msrLyrics() {}

void msrLyrics::addTextChunkToLyrics (
  string          syllabic,
  string          text,
  bool            elision,
  S_msrDuration  msrDuration)
{
  msrLyricsChunk::LyricsChunkType chunkType;
  
  if      (syllabic == "single")
    chunkType = msrLyricsChunk::kSingleChunk;
  else if (syllabic == "begin")
    chunkType = msrLyricsChunk::kBeginChunk;
  else if (syllabic == "middle")
    chunkType = msrLyricsChunk::kMiddleChunk;
  else if (syllabic == "end")
    chunkType = msrLyricsChunk::kEndChunk;
  else {
    stringstream s;
    string  result;
    s << "--> syllabic = " << syllabic << " is unknown";
    s >> result;
    msrMusicXMLError(result);
  }

  // create a lyrics text chunk
  if (fTranslationSettings->fDebug) {
    S_msrStaff staff = fLyricsVoice->getVoiceStaff();
    S_msrPart  part  = staff-> getStaffPart();
    
    cout << idtr <<
      "--> creating a lyrics " << fLyricsNumber <<
      " text chunk \"" << syllabic <<
      "\" in voice " << fLyricsVoice->getVoiceNumber() <<
      " of staff " << staff->getStaffNumber() <<
      " in part "  << part->getPartCombinedName() <<
      ", containing \"" << text <<
      "\", elision: " << elision <<
      ", duration: " << msrDuration << endl;
  }

  S_msrLyricsChunk
    chunk =
      msrLyricsChunk::create (
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
    case msrLyricsChunk::kBreakChunk:
      {
        // internalError (); JMI
      }
      break;
  } // switch

  fLyricsTextPresent = true;
}

void msrLyrics::addSkipChunkToLyrics (
  S_msrDuration  msrDuration)
{
  if (fTranslationSettings->fDebug) {
    S_msrStaff staff = fLyricsVoice->getVoiceStaff();
    S_msrPart  part  = staff-> getStaffPart();
    
    cout << idtr <<
      "--> creating a lyrics skip chunk, duration: " <<
      msrDuration <<
      " in voice " << fLyricsVoice->getVoiceNumber() <<
      " of staff " << staff->getStaffNumber() <<
      " in part "  << part->getPartCombinedName() << endl;
  }
  
  // create lyrics skip chunk
  S_msrLyricsChunk
    chunk =
      msrLyricsChunk::create (
        msrLyricsChunk::kSkipChunk, "", msrDuration);
        
  // add chunk to this lyrics
  fLyricsChunks.push_back (chunk);
}

void msrLyrics::addBreakChunkToLyrics (
  int nextMeasureNumber)
{
  if (fTranslationSettings->fDebug) {
    S_msrStaff staff = fLyricsVoice->getVoiceStaff();
    S_msrPart  part  = staff-> getStaffPart();
    
    cout << idtr <<
      "--> creating a lyrics break chunk" <<
      " in voice " << fLyricsVoice->getVoiceNumber() <<
      " of staff " << staff->getStaffNumber() <<
      " in part "  << part->getPartCombinedName() << endl;
  }

  // convert nextMeasureNumber to string
  stringstream s;
  string       str;
  s << nextMeasureNumber;
  s >> str;
  
  // create lyrics break chunk

  S_msrDuration
    nullMsrDuration =
      msrDuration::create (0, 0, 0, "");
        
  S_msrLyricsChunk
    chunk =
      msrLyricsChunk::create (
        msrLyricsChunk::kBreakChunk, str, nullMsrDuration);
        
  // add chunk to this lyrics
  fLyricsChunks.push_back (chunk);
}

ostream& operator<< (ostream& os, const S_msrLyrics& stan)
{
  stan->print(os);
  return os;
}

void msrLyrics::printMusicXML(ostream& os)
{
  os << "<!-- msrLyrics??? -->" << endl;
}

void msrLyrics::printMSR(ostream& os)
{  
  os << "Lyrics" << " " << getLyricsName ();
  if (! fLyricsTextPresent)
    os << " (No actual text)";
  os << endl;

  if (fLyricsTextPresent) {  
    idtr++;
    int n = fLyricsChunks.size();
    for (int i = 0; i < n; i++) {
      os << idtr << fLyricsChunks[i];
    } // for
    idtr--;
  }
}

void msrLyrics::printLilyPondCode(ostream& os)
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
  S_translationSettings& ts,
  int                    voiceNumber,
  S_msrStaff            voiceStaff)
{
  msrVoice* o = new msrVoice (ts, voiceNumber, voiceStaff);
  assert(o!=0);
  return o;
}

msrVoice::msrVoice (
  S_translationSettings& ts,
  int                    voiceNumber,
  S_msrStaff            voiceStaff)
    : msrElement("")
{
  fTranslationSettings = ts;

  fVoiceNumber = voiceNumber;
  fVoiceStaff  = voiceStaff;
  
  // create the implicit msrSequence element
  fVoiceSequence =
    msrSequence::create (msrSequence::kSpace);
  
  // add the implicit msrRepeat element
// JMI  fVoiceMsrRepeat = msrRepeat::create ();
//  fVoiceSequence->appendElementToSequence (fVoiceMsrRepeat);
  
  // add the implicit 4/4 time signature JMI
 // S_msrTime time = msrTime::create (4, 4, fGenerateNumericalTime);
 // S_msrElement t = time;
  //fVoiceSequence->appendElementToSequence (t);
}

msrVoice::~msrVoice() {}

string msrVoice::getVoiceName () const
{
  // not stored in a field,
  // because the voice staff may change name
  // when the part it belongs to is re-used
  return
    fVoiceStaff->getStaffName() +
    "_Voice_" +
    int2EnglishWord (fVoiceNumber);
}

S_msrLyrics msrVoice::addLyricsToVoice (
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
        fTranslationSettings,
        lyricsNumber,
        this);

  // register it in this staff
  fVoiceLyricsMap [lyricsNumber] = lyrics;

  // return it
  return lyrics;
}

S_msrLyrics msrVoice::voiceContainsLyrics (
  int lyricsNumber)
{
  S_msrLyrics result;
  
  if (fVoiceLyricsMap.count (lyricsNumber)) {
    result = fVoiceLyricsMap [lyricsNumber];
  }

  return result;
}

ostream& operator<< (ostream& os, const S_msrVoice& elt)
{
  elt->print(os);
  return os;
}

void msrVoice::printMusicXML(ostream& os)
{ 
  os << "<!-- msrVoice??? -->" << endl;
}

void msrVoice::printMSR(ostream& os)
{
  os << "Voice" << " " << getVoiceName () << endl << endl;

  idtr++;

  os << idtr << fVoiceSequence << endl;

  if (fVoiceLyricsMap.size()) {
    for (
      map<int, S_msrLyrics>::const_iterator i = fVoiceLyricsMap.begin();
      i != fVoiceLyricsMap.end();
      i++) {
      os << idtr << (*i).second;
    } // for
    os << endl;
  }
  
  idtr--;
}

void msrVoice::printLilyPondCode(ostream& os)
{
  os << getVoiceName () << " = ";
// JMI  if (! fVoiceAbsoluteCode) os << "\\relative ";
  os << "{" << endl;

  idtr++;
  os << fVoiceSequence;
  idtr--;

  os << idtr << "}" << endl;
}

//______________________________________________________________________________
S_msrStaff msrStaff::create (
  S_translationSettings& ts,
  int                    staffNumber,
  S_msrPart             staffPart)
{
  msrStaff* o = new msrStaff( ts, staffNumber, staffPart);
  assert(o!=0);
  return o;
}

msrStaff::msrStaff (
  S_translationSettings& ts,
  int                    staffNumber,
  S_msrPart             staffPart)
    : msrElement("")
{
  fTranslationSettings = ts;
  
  fStaffNumber = staffNumber;
  fStaffPart   = staffPart;
}

msrStaff::~msrStaff() {}

string msrStaff::getStaffName () const
  {
  // not stored in a field,
  // because the staff part may change name
  // when it is re-used
  return
    fStaffPart->getPartMSRName() +
    "_Staff_" +
    int2EnglishWord (fStaffNumber);
  }

S_msrVoice msrStaff::addVoiceToStaff (
  int voiceNumber)
{
  if (fStaffVoicesMap.count (voiceNumber)) {
    cerr << idtr <<
      "### Internal error: voice " << voiceNumber <<
      " already exists in this staff" << endl;

    return fStaffVoicesMap [voiceNumber];
  }

  // create the voice
  S_msrVoice
    voice =
      msrVoice::create (
        fTranslationSettings,
        voiceNumber,
        this);

  // register it in this staff
  if (fTranslationSettings->fTrace)
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

  return result;
}

void msrStaff::setStaffKey  (S_msrKey  key)
{
  if (fTranslationSettings->fTrace)
    cerr << idtr <<
      "Adding key '" << key <<
      "' to staff " << fStaffNumber <<
      " in part " << fStaffPart->getPartCombinedName () << endl;

  fStaffKey = key;
}

void msrStaff::setStaffTime (S_msrTime time)
{
  if (fTranslationSettings->fTrace)
    cerr << idtr <<
      "Adding time '" << time <<
      "' to staff " << fStaffNumber <<
      " in part " << fStaffPart->getPartCombinedName () << endl;

  fStaffTime = time;
}
      
void msrStaff::setStaffClef (S_msrClef clef)
{
  if (fTranslationSettings->fTrace)
    cerr << idtr <<
      "Adding clef '" << clef <<
      "' to staff " << fStaffNumber <<
      " in part " << fStaffPart->getPartCombinedName () << endl;

  fStaffClef = clef;
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
    os << idtr << fStaffClef << endl;
  else
    os << idtr << "NO_CLEF" << endl;

  if (fStaffKey)
    os << idtr << fStaffKey << endl;
  else
    os << idtr << "NO_KEY" << endl;

  if (fStaffTime)
    os << idtr << fStaffTime << endl;
  else
    os << idtr << "NO_TIME" << endl;

  os <<
    idtr << "StaffInstrumentName: \"" <<
    fStaffInstrumentName << "\"" << endl;
  
  for (
    map<int, S_msrVoice>::iterator i = fStaffVoicesMap.begin();
    i != fStaffVoicesMap.end();
    i++) {
    os << idtr << (*i).second;
  } // for

  idtr--;
}

void msrStaff::printLilyPondCode (ostream& os)
{
  os <<
    "Staff" << " " << getStaffName () << " " <<
    fStaffInstrumentName << endl;
 // if (! fTranslationSettings->fGenerateAbsoluteCode) os << "\\relative "; // JMI
  os << "{" << endl;

  idtr++;
  idtr--;

  os << idtr << "}" << endl;
}

//______________________________________________________________________________
S_msrPart msrPart::create (
    S_translationSettings& ts,
    string                 partMusicXMLName)
{
  msrPart* o = new msrPart( ts, partMusicXMLName);
  assert(o!=0);
  return o;
}

msrPart::msrPart (
  S_translationSettings& ts,
  string                 partMusicXMLName)
    : msrElement("")
{
  fTranslationSettings = ts;
  
  fPartMusicXMLName = partMusicXMLName;

  // coin the part MSR name
  fPartMSRName =
    "Part_"+stringNumbersToEnglishWords (fPartMusicXMLName);
    
  if (fTranslationSettings->fTrace)
    cerr << idtr <<
      "Creating part " << getPartCombinedName () << endl;
}

msrPart::~msrPart() {}

void msrPart::changePartMusicXMLName (
  string newPartMusicXMLName)
{
  string oldCombinedName = getPartCombinedName ();
  
  fPartMusicXMLName = newPartMusicXMLName;

  // coin the new part MSR name
  fPartMSRName =
    "Part_"+stringNumbersToEnglishWords (fPartMusicXMLName);
    
  if (fTranslationSettings->fTrace)
    cerr << idtr <<
      "Re-using part " << oldCombinedName <<
      " as " << getPartCombinedName () << endl;
}

ostream& operator<< (ostream& os, const S_msrPart& elt)
{
  elt->print(os);
  return os;
}

void msrPart::printMusicXML(ostream& os)
{
  os << "<!-- msrPart??? -->" << endl;
}

void msrPart::printMSR(ostream& os)
{
  os <<
    "Part" << " \"" << getPartCombinedName () << endl;
    
  idtr++;
  
  os <<
    idtr << "PartName          : \"" << fPartName << "\"" << endl <<
    idtr << "PartAbbrevation   : \"" << fPartAbbreviation << "\"" << endl;
  
  os << idtr <<
    "fPartMusicXMLDivisions: " << fPartMusicXMLDivisions << endl;

  os << idtr <<
    "PartInstrumentName: \"" << fPartInstrumentName << "\"" << endl;

  for (
    map<int, S_msrStaff>::iterator i = fPartStavesMap.begin();
    i != fPartStavesMap.end();
    i++) {
    os << idtr << (*i).second;
  } // for
  
  idtr--;
}

void msrPart::printLilyPondCode(ostream& os)
{
  os <<
    "Part" << " " << getPartCombinedName () << " " << endl <<
    "\"" << fPartInstrumentName << "\"" << endl;
  if (! fTranslationSettings->fGenerateAbsoluteCode) os << "\\relative "; // JMI
  os << "{" << endl;

  idtr++;
// JMI  os << fPartMsrSequence << endl;
  idtr--;

  os << idtr << "}" << endl;
}

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
  int staffNumber)
{
  if (fPartStavesMap.count (staffNumber)) {
    cerr << idtr <<
      "### Internal error: staffNumber " << staffNumber <<
      " already exists in part " << " " << getPartCombinedName () << endl;

    return fPartStavesMap [staffNumber];
  }

  if (fTranslationSettings->fTrace)
    cerr << idtr <<
      "Adding staff " << staffNumber <<
      " to part " << getPartCombinedName () << endl;
  
  // create the staff
  S_msrStaff
    staff =
      msrStaff::create (
        fTranslationSettings,
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

//______________________________________________________________________________
S_msrPartGroup msrPartGroup::create (
  S_translationSettings& ts,
  int                    partGroupNumber,
  PartGroupTypeKind      partPartGroupTypeKind,
  string                 partGroupName,
  string                 partGroupAbbreviation,
  PartGroupSymbolKind    partGroupSymbolKind,
  string                 partGroupSymbolDefaultX,
  bool                   partGroupBarline)
{
  msrPartGroup* o =
    new msrPartGroup (
      ts,
      partGroupNumber,
      partPartGroupTypeKind,
      partGroupName,
      partGroupAbbreviation,
      partGroupSymbolKind,
      partGroupSymbolDefaultX,
      partGroupBarline);
  assert(o!=0);
  return o;
}

msrPartGroup::msrPartGroup (
  S_translationSettings& ts,
  int                    partGroupNumber,
  PartGroupTypeKind      partPartGroupTypeKind,
  string                 partGroupName,
  string                 partGroupAbbreviation,
  PartGroupSymbolKind    partGroupSymbolKind,
  string                 partGroupSymbolDefaultX,
  bool                   partGroupBarline)
    : msrElement("")
{
  fTranslationSettings = ts;

  fPartGroupNumber = partGroupNumber;
  fPartPartGroupTypeKind = partPartGroupTypeKind;
        
  fPartGroupName = partGroupName;
  fPartGroupAbbreviation = partGroupAbbreviation;

  fPartGroupSymbolKind = partGroupSymbolKind;
  fPartGroupSymbolDefaultX = partGroupSymbolDefaultX;

  fPartGroupBarline = partGroupBarline;
  
  if (fTranslationSettings->fTrace)
    cerr <<
      "Creating part group " << fPartGroupNumber << endl;
}

msrPartGroup::~msrPartGroup() {}

S_msrPart msrPartGroup::addPartToPartGroup (
  string partMusicXMLName)
{
  if (fPartGroupPartsMap.count (partMusicXMLName)) {
    cerr << idtr <<
      "### Internal error: partMusicXMLName " << partMusicXMLName <<
      " already exists in this part group" << endl;

    return fPartGroupPartsMap [partMusicXMLName];
  }

  // create the part
  S_msrPart part;

  part =
    tryAndReUseInitialAnonymousPart (partMusicXMLName);

  if (part) {
    
    // the anonymous part is being re-used
    return part;

  } else {

    // create the part
    part =
      msrPart::create (
        fTranslationSettings, partMusicXMLName);

    if (fTranslationSettings->fTrace)
      cerr << idtr <<
        "Adding part " << part->getPartCombinedName () <<
        " to part group " << fPartGroupNumber << endl;
  
    // register it in this part group
    if (fTranslationSettings->fTrace) {
      cerr << idtr <<
        "Adding part " << part->getPartCombinedName () <<
        " to part group " << fPartGroupNumber << endl;
    }
    fPartGroupPartsMap [partMusicXMLName] = part;
    fPartGroupPartsList.push_back (part);
  }

  if (true || fTranslationSettings->fDebug) {
//  if (fTranslationSettings->fDebug) {
    cerr << idtr <<
      "==> After addPartToPartGroup, fPartGroupPartsMap contains:" << endl;
    idtr++;
    for (
        msrPartsMap::const_iterator i = fPartGroupPartsMap.begin();
        i != fPartGroupPartsMap.end();
        i++) {
      cerr << idtr <<
        "\"" << (*i).first << "\" ----> " <<
        (*i).second->getPartCombinedName() << endl;
    } // for
    idtr--;
    cerr << idtr << "<== addPartToPartGroup" << endl;
  }

  if (true || fTranslationSettings->fDebug) {
//  if (fTranslationSettings->fDebug) {
    cerr << idtr <<
      "==> After addPartToPartGroup, fPartGroupPartsList contains:" << endl;
    idtr++;
    for (
        msrPartsList::const_iterator i = fPartGroupPartsList.begin();
        i != fPartGroupPartsList.end();
        i++) {
      cerr << idtr <<
        (*i)->getPartCombinedName() << endl;
    } // for
    idtr--;
    cerr << idtr << "<== addPartToPartGroup" << endl;
  }

  // return the part
  return part;
}

S_msrPart msrPartGroup::fetchPartFromPartGroup (
  string partMusicXMLName)
{
  /*
  cerr << idtr << "==> fetchPartFromPartGroup, fPartGroupPartsMap contains:" << endl;
  for (
      msrPartsMap::const_iterator i = fPartGroupPartsMap.begin();
      i != fPartGroupPartsMap.end();
      i++) {
    cerr << idtr <<
      (*i).first << " ----> " <<
      (*i).second->getPartCombinedName() << endl;
  } // for
  cerr << idtr << "<== fetchPartFromPartGroup" << endl;
  */
  
  S_msrPart result;
  
  if (fPartGroupPartsMap.count (partMusicXMLName)) {
    result = fPartGroupPartsMap [partMusicXMLName];
  }

  return result;
}

S_msrPart msrPartGroup::tryAndReUseInitialAnonymousPart (
  string partMusicXMLName)
{
  if (fTranslationSettings->fDebug) {
    cerr << idtr <<
      "==> START tryAndReUseInitialAnonymousPart, fPartGroupPartsMap contains:" << endl;
    for (
        msrPartsMap::const_iterator i = fPartGroupPartsMap.begin();
        i != fPartGroupPartsMap.end();
        i++) {
      cerr << idtr <<
        "\"" << (*i).first << "\" ----> " <<
        (*i).second->getPartCombinedName() << endl;
    } // for
    cerr << idtr << "<== tryAndReUseInitialAnonymousPart" << endl;
  }

  S_msrPart result;

  if (fPartGroupPartsMap.size ()) {

    msrPartsMap::iterator i =
      fPartGroupPartsMap.find ("");
      
    if (i != fPartGroupPartsMap.end()) {
      // this is the first true part, re-use the one
      // created with an empty name initially
      S_msrPart partToBeReUsed = (*i).second;
      
      partToBeReUsed->
        changePartMusicXMLName (partMusicXMLName);

      fPartGroupPartsMap [partMusicXMLName] = partToBeReUsed;
      fPartGroupPartsMap.erase ("");
      
      result = partToBeReUsed;
    }
  }

  if (fTranslationSettings->fDebug) {
    cerr << idtr <<
      "==> END tryAndReUseInitialAnonymousPart, fPartGroupPartsMap contains:" << endl;
    for (
        msrPartsMap::const_iterator i = fPartGroupPartsMap.begin();
        i != fPartGroupPartsMap.end();
        i++) {
      cerr << idtr <<
        "\"" << (*i).first << "\" ----> " <<
        (*i).second->getPartCombinedName() << endl;
    } // for
    cerr << idtr << "<== tryAndReUseInitialAnonymousPart" << endl;
  }

  return result;
}

/*
void msrPartGroup::popPartGroupPartsStackTop ()
{
  if (fTranslationSettings->fTrace) {
    S_msrPart
      stackTopPart = fPartGroupPartsStack.top ();
      
    cerr << idtr <<
      "Popping part " << stackTopPart->getPartCombinedName () <<
      " from part group " << fPartGroupNumber << " stack" << endl;
  }
  
  fPartGroupPartsStack.pop ();
}
*/

ostream& operator<< (ostream& os, const S_msrPartGroup& elt)
{
  elt->print(os);
  return os;
}

void msrPartGroup::printMusicXML(ostream& os)
{
  os << "<!-- msrPartGroup??? -->" << endl;
}

void msrPartGroup::printMSR(ostream& os)
{
  os <<
    "PartGroup" << " " << fPartGroupNumber << endl;
    
  idtr++;

  os <<
    idtr << "PartGroupName        : \"" << fPartGroupName << "\"" << endl <<
    idtr << "PartGroupAbbrevation : \"" << fPartGroupAbbreviation << "\"" << endl <<
    idtr << "fPartGroupSymbolKind : \"" << fPartGroupSymbolKind << "\"" << endl <<
    idtr << "PartGroupBarline     : \"" << fPartGroupBarline << "\"" << endl << endl;

 // JMI   idtr << "PartGroupInstrumentName: \"" << fPartGroupInstrumentName << "\"" << endl;

  for (
    msrPartsMap::iterator i = fPartGroupPartsMap.begin();
    i != fPartGroupPartsMap.end();
    i++) {
    os << idtr << (*i).second;
  } // for
  
  idtr--;
}

void msrPartGroup::printLilyPondCode(ostream& os)
{
  os <<
    "PartGroup" << " " << fPartGroupNumber << endl;

  idtr++;
// JMI  os << fPartGroupMsrSequence << endl;
  idtr--;

  os << idtr << "}" << endl;
}

//______________________________________________________________________________
S_msrScore msrScore::create (
  S_translationSettings& ts)
{
  msrScore* o = new msrScore (ts);
  assert(o!=0);
  return o;
}

msrScore::msrScore (
  S_translationSettings& ts)
    : msrElement("")
{
  fTranslationSettings = ts;
}
msrScore::~msrScore() {}

S_msrPartGroup msrScore::addPartGroupToScore (
  int partGroupNumber)
{
  /* JMI
  if (fScorePartGroupsMap.count (partGroupNumber)) {
    cerr << idtr <<
      "### Internal error: part group " << partGroupNumber <<
      " already exists in this score" << endl;

    return fScorePartGroupsMap [partGroupNumber];
  }
*/

  // create the part group
  S_msrPartGroup
    partGroup =
      msrPartGroup::create (
        fTranslationSettings, partGroupNumber);

  // register it in this score
  fPartGroupsList.push_back (partGroup);

   return partGroup;
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

void msrScore::printMusicXML (ostream& os)
{
  os << "<!-- msrScore??? -->" << endl;
}

ostream& operator<< (ostream& os, const S_msrScore& elt)
{
  elt->print(os);
  return os;
}

void msrScore::printMSR (ostream& os)
{
  os << "Score" << endl;

  idtr++;
  
  for (
    msrPartGroupsList::iterator i = fPartGroupsList.begin();
    i != fPartGroupsList.end();
    i++) {
    os << idtr << (*i);
  } // for
  
  idtr--;
}

void msrScore::printLilyPondCode (ostream& os)
{
  printMSR (os);
}


}
