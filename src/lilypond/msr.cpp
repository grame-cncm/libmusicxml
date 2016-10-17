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

lpsrGlobalVariables::CodeGenerationKind
  lpsrGlobalVariables::sCodeGenerationKind =
    lpsrGlobalVariables::kLilypondCode;

indenter lpsrElement::idtr;

//______________________________________________________________________________
S_lpsrElement lpsrElement::create(bool debug)
{
  lpsrElement * o = new lpsrElement(debug); assert(o!=0);
  return o; 
}

lpsrElement::lpsrElement(bool debug)
{
  fDebug = debug;
}
lpsrElement::~lpsrElement() {}

ostream& operator<< (ostream& os, const S_lpsrElement& elt)
{
  elt->print(os);
  return os;
}

void lpsrElement::print(ostream& os)
{
  // a global variable is needed so that lpsr::Element.print() 
  // can decide whether to print:
  //   - the MSR structure
  //   - MusicXML text
  //   - LilyPond source code
switch (lpsrGlobalVariables::getCodeGenerationKind()) {
    case lpsrGlobalVariables::kMSR:
      this->printMSR (os);
      break;
    case lpsrGlobalVariables::kMusicXML:
      this->printMusicXML (os);
      break;
    case lpsrGlobalVariables::kLilypondCode:
      this->printLilyPondCode (os);
      break;
  } // switch
}

void lpsrElement::printMusicXML(ostream& os)
{
  os << "<!-- lpsrElement??? -->" << endl;
}

void lpsrElement::printMSR(ostream& os)
{
  os << "Element???" << endl;
}

void lpsrElement::printLilyPondCode(ostream& os)
{
  os << "\%{ lpsrElement??? \%}" << endl;
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
S_lpsrAbsoluteOctave lpsrAbsoluteOctave::create (int musicxmlOctave)
{
  lpsrAbsoluteOctave * o = new lpsrAbsoluteOctave (musicxmlOctave);
  assert(o!=0); 
  return o;
}

lpsrAbsoluteOctave::lpsrAbsoluteOctave (int musicxmlOctave)
  : lpsrElement("")
{
  fMsrOctave = musicxmlOctave - 3;
  /*
  cout <<
    "lpsrAbsoluteOctave::lpsrAbsoluteOctave (), musicxmlOctave = " << musicxmlOctave << 
    ", fMsrOctave = " << fMsrOctave << endl;
    */
}
lpsrAbsoluteOctave::~lpsrAbsoluteOctave () {}

ostream& operator<< (ostream& os, const S_lpsrAbsoluteOctave& dur)
{
  dur->print(os);
  return os;
}

void lpsrAbsoluteOctave::printMusicXML (ostream& os)
{
  os << "<!-- lpsrAbsoluteOctave??? -->" << endl;
}

string lpsrAbsoluteOctave::absoluteOctaveAsLilypondString ()
{
  stringstream s;
  string  result;
  
  s << fMsrOctave;
  s >> result;
  return result;
}

void lpsrAbsoluteOctave::printMSR(ostream& os)
{
  this->printLilyPondCode (os);
}

void lpsrAbsoluteOctave::printLilyPondCode(ostream& os)
{
  /*
  enum NoteFigures = {
    k1024th, k512th, 256th, k128th, k64th, k32nd, k16th, 
    kEighth, kQuarter, kHalf, kWhole, kBreve, kLong, kMaxima};
*/

  os << absoluteOctaveAsLilypondString ();
}

//______________________________________________________________________________
S_lpsrDuration lpsrDuration::create (
  int     num,
  int    denom,
  int    dots,
  string tupletMemberType)
{
  lpsrDuration * o =
    new lpsrDuration (num, denom, dots, tupletMemberType);
  assert(o!=0); 
  return o;
}

lpsrDuration::lpsrDuration (
  int    num,
  int    denom,
  int    dots,
  string tupletMemberType)
    : lpsrElement("")
{
  fNum   = num;
  fDenom = denom;
  fDots  = dots;
  fTupletMemberNoteType = tupletMemberType;
  /*
  cout <<
    "lpsrDuration::lpsrDuration (), fNum = " << fNum << 
    ", fDenom = " << fDenom << ", fDots = " << fDots << endl;
  */
}
lpsrDuration::~lpsrDuration() {}

void lpsrDuration::scaleNumByFraction (int num, int denom)
{
  fNum *= num/denom;
}

ostream& operator<< (ostream& os, const S_lpsrDuration& dur)
{
  dur->print(os);
  return os;
}

void lpsrDuration::printMusicXML(ostream& os)
{
  os << "<!-- lpsrDuration??? -->" << endl;
}

string lpsrDuration::durationAsLilypondString ()
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
      "%--> lpsrDuration::printLilyPondCode, noteDivisions = " << noteDivisions <<
      ", divisionsPerWholeNote = " << divisionsPerWholeNote <<
      endl;
    s >> message;
    lpsrMusicXMLError(message);
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
      string  message;
      s << 
        endl << 
        "--> unknown tuplet member type " << fTupletMemberNoteType <<
        endl;
      s >> message;
      lpsrMusicXMLError(message);
      }
        
  } else {
    
    div_t divresult = div (noteDivisions, divisionsPerWholeNote);  
    int   div = divresult.quot;
//    int   mod = divresult.rem; not yet used JMI
        
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
        lpsrMusicXMLError(message);
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

void lpsrDuration::printMSR(ostream& os)
{
  os << durationAsLilypondString () << flush;
}

void lpsrDuration::printLilyPondCode(ostream& os)
{
  /*
  enum NoteFigures = {
    k1024th, k512th, 256th, k128th, k64th, k32nd, k16th, 
    kEighth, kQuarter, kHalf, kWhole, kBreve, kLong, kMaxima};
*/

  os << durationAsLilypondString ();
}

//______________________________________________________________________________
S_lpsrArticulation lpsrArticulation::create (
  ArticulationKind articulationKind)
{
  lpsrArticulation* o =
    new lpsrArticulation (articulationKind);
  assert (o!=0);
  return o;
}

lpsrArticulation::lpsrArticulation (
  ArticulationKind articulationKind)
    : lpsrElement("")
{
  fArticulationKind = articulationKind;
}

lpsrArticulation::~lpsrArticulation() {}


ostream& operator<< (ostream& os, const S_lpsrArticulation& elt)
{
  elt->print(os);
  return os;
}

void lpsrArticulation::printMusicXML(ostream& os)
{
  os << "<!-- lpsrTime??? -->" << endl;
}

void lpsrArticulation::printMSR(ostream& os)
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

void lpsrArticulation::printLilyPondCode(ostream& os)
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
S_lpsrDynamics lpsrDynamics::create(DynamicsKind dynamicsKind)
{
  lpsrDynamics* o = new lpsrDynamics(dynamicsKind); assert(o!=0);
  return o;
}

lpsrDynamics::lpsrDynamics(DynamicsKind dynamicsKind)
  : lpsrElement("")
{
  fDynamicsKind = dynamicsKind; 
}
lpsrDynamics::~lpsrDynamics() {}

ostream& operator<< (ostream& os, const S_lpsrDynamics& dyn)
{
  dyn->print(os);
  return os;
}

string lpsrDynamics::dynamicsKindAsLilypondString ()
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
      lpsrMusicXMLError(message);
      }
  } // switch
  
  string result;
  
  s >> result;
  return result;
}

void lpsrDynamics::printMusicXML(ostream& os)
{
  os << "<!-- lpsrDynamics??? -->" << endl;
}

void lpsrDynamics::printMSR(ostream& os)
{
  os <<
    "Dynamics" << " " << dynamicsKindAsLilypondString () << endl;
}

void lpsrDynamics::printLilyPondCode(ostream& os)
{
  os << dynamicsKindAsLilypondString ();
}

//______________________________________________________________________________
S_lpsrWedge lpsrWedge::create(WedgeKind wedgeKind)
{
  lpsrWedge* o = new lpsrWedge(wedgeKind); assert(o!=0);
  return o;
}

lpsrWedge::lpsrWedge(WedgeKind wedgeKind)
  : lpsrElement("")
{
  fWedgeKind=wedgeKind; 
}
lpsrWedge::~lpsrWedge() {}

ostream& operator<< (ostream& os, const S_lpsrWedge& wdg)
{
  wdg->print(os);
  return os;
}

string lpsrWedge::wedgeKindAsString ()
{
  stringstream s;
  
  switch (fWedgeKind) {
    case lpsrWedge::kCrescendoWedge:
      s << "crescendo";
      break;
    case lpsrWedge::kDecrescendoWedge:
      s << "decrescendo";
      break;
    case lpsrWedge::kStopWedge:
      s << "stop";
      break;
    default:
      s << "Wedge" << fWedgeKind << "???";
  } // switch
  
  string result;
  
  s >> result;
  return result;
}

void lpsrWedge::printMusicXML(ostream& os)
{
  os << "<!-- lpsrWedge??? -->" << endl;
}

void lpsrWedge::printMSR(ostream& os)
{
  os << "Wedge" << " " << wedgeKindAsString () << endl;
}

void lpsrWedge::printLilyPondCode(ostream& os)
{
  os << wedgeKindAsString ();
}

//______________________________________________________________________________
S_lpsrSlur lpsrSlur::create(SlurKind slurKind)
{
  lpsrSlur* o = new lpsrSlur(slurKind); assert(o!=0);
  return o;
}

lpsrSlur::lpsrSlur(SlurKind slurKind)
  : lpsrElement("")
{
  fSlurKind=slurKind; 
}
lpsrSlur::~lpsrSlur() {}

ostream& operator<< (ostream& os, const S_lpsrSlur& wdg)
{
  wdg->print(os);
  return os;
}

string lpsrSlur::slurKindAsString ()
{
  stringstream s;
  
  switch (fSlurKind) {
    case lpsrSlur::kStartSlur:
      s << "crescendo";
      break;
    case lpsrSlur::kContinueSlur:
      s << "decrescendo";
      break;
    case lpsrSlur::kStopSlur:
      s << "stop";
      break;
    default:
      s << "Slur" << fSlurKind << "???";
  } // switch
  
  string result;
  
  s >> result;
  return result;
}

void lpsrSlur::printMusicXML(ostream& os)
{
  os << "<!-- lpsrSlur??? -->" << endl;
}

void lpsrSlur::printMSR(ostream& os)
{
  os << "Slur" << " " << slurKindAsString () << endl;
}

void lpsrSlur::printLilyPondCode(ostream& os)
{
  os << slurKindAsString ();
}

//______________________________________________________________________________
S_lpsrNote lpsrNote::createFromMusicXMLData (
  S_translationSettings& ts,
  musicXMLNoteData&      mxmldat,
  lpsrSlur::SlurKind     slurKind)
{  
  lpsrNote * o = new lpsrNote (ts, mxmldat, slurKind);
  assert(o!=0); 
  return o;
}

lpsrNote::lpsrNote (
  S_translationSettings& ts,
  musicXMLNoteData&      mxmldat,
  lpsrSlur::SlurKind     slurKind)
  :
    lpsrElement(""),
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
  if (fMusicXMLNoteData.fMusicxmlStep)
    fMusicXMLDiatonicPitch = lpsrNote::kRest;

  if (fMusicXMLNoteData.fMusicxmlStep < 'A' || fMusicXMLNoteData.fMusicxmlStep > 'G') {
    if (! fMusicXMLNoteData.fMusicxmlStepIsARest) {
      stringstream s;
      string  message;
      s << "step value " << fMusicXMLNoteData.fMusicxmlStep << " is not a letter from A to G";
      s >> message;
    //  lpsrMusicXMLError (message);
    lpsrMusicXMLWarning (message); // JMI
    }
  }

//  cout << "=== xmlPart2MsrVisitor::visitStart ( S_step& elt ) " << fCurrentMusicXMLStep << endl;
// JMI

  switch (fMusicXMLNoteData.fMusicxmlStep) {
    case 'A': fMusicXMLDiatonicPitch = lpsrNote::kA; break;
    case 'B': fMusicXMLDiatonicPitch = lpsrNote::kB; break;
    case 'C': fMusicXMLDiatonicPitch = lpsrNote::kC; break;
    case 'D': fMusicXMLDiatonicPitch = lpsrNote::kD; break;
    case 'E': fMusicXMLDiatonicPitch = lpsrNote::kE; break;
    case 'F': fMusicXMLDiatonicPitch = lpsrNote::kF; break;
    case 'G': fMusicXMLDiatonicPitch = lpsrNote::kG; break;
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
  lpsrNote::MusicXMLAlteration mxmlAlteration;

/*
  cout <<
    "--> fMusicXMLNoteData.fMusicxmlAlteration = " <<
    fMusicXMLNoteData.fMusicxmlAlteration <<  endl;
*/
  
  switch (fMusicXMLNoteData.fMusicxmlAlteration) {
    case -2:
      mxmlAlteration = lpsrNote::kDoubleFlat;
      noteQuatertonesFromA-=3;
      if (noteQuatertonesFromA < 0)
        noteQuatertonesFromA += 24; // it is below A
      break;
    case -1:
      mxmlAlteration = lpsrNote::kFlat;
      noteQuatertonesFromA-=2;
      if (noteQuatertonesFromA < 0)
        noteQuatertonesFromA += 24; // it is below A
      break;
    case 0:
      mxmlAlteration = lpsrNote::kNatural;
      break;
    case 1:
      mxmlAlteration = lpsrNote::kSharp;
      noteQuatertonesFromA+=2;
      break;
    case 2:
      mxmlAlteration = lpsrNote::kDoubleSharp;
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
      
      lpsrAssert(
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
    
  lpsrAssert(
    divisionsPerWholeNote > 0,
    "The MusicMXL divisions per quater note value should be positive");

  fNoteMsrDuration =
    lpsrDuration::create (
      fMusicXMLNoteData.fMusicxmlDuration,
      divisionsPerWholeNote,
      fMusicXMLNoteData.fDotsNumber,
      fMusicXMLNoteData.fTupletMemberNoteType);
//  cout << "fNoteMsrDuration = " << fNoteMsrDuration << endl;
    
  // diatonic note JMI forelative code
  lpsrNote::MusicXMLDiatonicPitch diatonicNote =
    lpsrNote::k_NoDiatonicPitch;
}

lpsrNote::~lpsrNote() {}

void lpsrNote::setNoteBelongsToAChord () {
  fMusicXMLNoteData.fNoteBelongsToAChord = true;
}

//______________________________________________________________________________

lpsrNote::MsrPitch lpsrNote::computeNoteMsrPitch (
  int                          noteQuatertonesFromA,
  lpsrNote::MusicXMLAlteration alteration)
{
  // computing the lpsr pitch
  /*
  Alter values of -2 and 2 can be used for double-flat and double-sharp. Decimal values can be used for microtones (e.g., 0.5 for a quarter-tone sharp), but not all programs may convert this into MIDI pitch-bend data.

  For rests, a rest element is used instead of the pitch element. The whole rest in 3/4 that begins the voice part is represented as:
    <note>
      <rest/>
      <duration>72</duration>
    </note>
  
  Quarter tones may be added; the following is a series of Cs with increasing pitches:
    \relative c'' { ceseh ces ceh c cih cis cisih }
   */
  lpsrNote::MsrPitch lpsrPitch = lpsrNote::k_NoMsrPitch;
  
  switch (noteQuatertonesFromA) {
    case 0:
      lpsrPitch = lpsrNote::k_a;
      break;
    case 1:
      lpsrPitch =
        alteration == lpsrNote::kDoubleSharp
          ? lpsrNote::k_aih
          : lpsrNote::k_beseh;
      break;
    case 2:
      lpsrPitch =
        alteration == lpsrNote::kSharp
          ? lpsrNote::k_ais
          : lpsrNote::k_bes;
      break;
    case 3:
      lpsrPitch =
        alteration == lpsrNote::kDoubleSharp
          ? lpsrNote::k_aisih
          : lpsrNote::k_beh;
      break;
    case 4:
      lpsrPitch = lpsrNote::k_b;
      break;
    case 5:
      lpsrPitch =
        alteration == lpsrNote::kDoubleSharp
          ? lpsrNote::k_bih
          : lpsrNote::k_ceseh;
      break;
    case 6:
      lpsrPitch = lpsrNote::k_c;
      break;
    case 7:
      lpsrPitch =
        alteration == lpsrNote::kDoubleSharp
          ? lpsrNote::k_cih
          : lpsrNote::k_deseh;
      break;
    case 8:
      lpsrPitch =
        alteration == lpsrNote::kSharp
          ? lpsrNote::k_cis
          : lpsrNote::k_des;
      break;
    case 9:
      lpsrPitch =
        alteration == lpsrNote::kDoubleSharp
          ? lpsrNote::k_cisih
          : lpsrNote::k_deh;
      break;
    case 10:
      lpsrPitch = lpsrNote::k_d;
      break;
    case 11:
      lpsrPitch =
        alteration == lpsrNote::kDoubleSharp
          ? lpsrNote::k_dih
          : lpsrNote::k_eeseh;
      break;
    case 12:
      lpsrPitch =
        alteration == lpsrNote::kSharp
          ? lpsrNote::k_dis
          : lpsrNote::k_ees;
      break;
    case 13:
      lpsrPitch =
        alteration == lpsrNote::kDoubleSharp
          ? lpsrNote::k_disih
          : lpsrNote::k_eeh;
      break;
    case 14:
      lpsrPitch = lpsrNote::k_e;
      break;
    case 15:
      lpsrPitch =
        alteration == lpsrNote::kDoubleSharp
          ? lpsrNote::k_eih
          : lpsrNote::k_feseh;
      break;
    case 16:
      lpsrPitch = lpsrNote::k_f;
      break;
    case 17:
      lpsrPitch =
        alteration == lpsrNote::kDoubleSharp
          ? lpsrNote::k_fih
          : lpsrNote::k_geseh;
      break;
    case 18:
      lpsrPitch =
        alteration == lpsrNote::kSharp
          ? lpsrNote::k_fis
          : lpsrNote::k_ges;
      break;
    case 19:
      lpsrPitch =
        alteration == lpsrNote::kDoubleSharp
          ? lpsrNote::k_fisih
          : lpsrNote::k_geh;
      break;
    case 20:
      lpsrPitch = lpsrNote::k_g;
      break;
    case 21:
      lpsrPitch =
        alteration == lpsrNote::kDoubleSharp
          ? lpsrNote::k_gih
          : lpsrNote::k_aeseh;
      break;
    case 22:
      lpsrPitch =
        alteration == lpsrNote::kSharp
          ? lpsrNote::k_gis
          : lpsrNote::k_aes;
      break;
    case 23:
      lpsrPitch =
        alteration == lpsrNote::kDoubleSharp
          ? lpsrNote::k_gisih
          : lpsrNote::k_aeh;
      break;
  } // switch
  
  return lpsrPitch;
}

void lpsrNote::addArticulation (S_lpsrArticulation art)
{
  fNoteArticulations.push_back(art);
}

void lpsrNote::addDynamics (S_lpsrDynamics dyn) {
  fNoteDynamics.push_back(dyn);
}
void lpsrNote::addWedge (S_lpsrWedge wdg) {
  fNoteWedges.push_back(wdg);
}

S_lpsrDynamics lpsrNote::removeFirstDynamics () {
  S_lpsrDynamics dyn = fNoteDynamics.front();
  fNoteDynamics.pop_front();
  return dyn;
}
S_lpsrWedge lpsrNote::removeFirstWedge () {
  S_lpsrWedge wdg = fNoteWedges.front();
  fNoteWedges.pop_front();
  return wdg;
}

ostream& operator<< (ostream& os, const S_lpsrNote& elt)
{
  elt->print(os);
  return os;
}

string lpsrNote::notePitchAsLilypondString ()
{
  stringstream s;
  
  /*
  cout << "lpsrNote::notePitchAsLilypondString (), isRest = " <<
    fMusicXMLNoteData.fMusicxmlStepIsARest <<
    ", fMsrPitch = " << fMsrPitch << endl;
  */
  
  if (fMusicXMLNoteData.fMusicxmlStepIsARest)
    s << "r";

  else {
    //JMI assertMsr(fMsrPitch != k_NoMsrPitch, "fMsrPitch != k_NoMsrPitch");
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

void lpsrNote::printMusicXML(ostream& os)
{
  os << "<!-- lpsrNote??? -->" << endl;
}

void lpsrNote::printMSR(ostream& os)
{
  /*
  cout <<
    "lpsrNote::printMSR(), fNoteBelongsToAChord = " << 
    fNoteBelongsToAChord << endl;
  */
  
  if (fMusicXMLNoteData.fNoteBelongsToAChord) {
    
    os <<
      notePitchAsLilypondString () <<
      ":" << fNoteMsrDuration;

  } else {
    
    os <<
      "Note" << " " <<
      notePitchAsLilypondString () <<
      ":" << fNoteMsrDuration;
    if (fMusicXMLNoteData.fNoteIsAGraceNote)
      os << " " << "grace";
    os << endl;
    
    // print the alterations if any
    if (fNoteArticulations.size()) {
      idtr++;
      list<S_lpsrArticulation>::const_iterator i;
      for (i=fNoteArticulations.begin(); i!=fNoteArticulations.end(); i++) {
        os << idtr << (*i);
      } // for
      idtr--;
    }
    
    // print the dynamics if any
    if (fNoteDynamics.size()) {
      idtr++;
      list<S_lpsrDynamics>::const_iterator i1;
      for (i1=fNoteDynamics.begin(); i1!=fNoteDynamics.end(); i1++) {
        os << idtr << (*i1);
      } // for
      idtr--;
    }
  
    // print the wedges if any
    if (fNoteWedges.size()) {
      idtr++;
      list<S_lpsrWedge>::const_iterator i2;
      for (i2=fNoteWedges.begin(); i2!=fNoteWedges.end(); i2++) {
        os << idtr << (*i2);
      } // for
      idtr--;
    }

    // print the slur if any
    if (fNoteSlurKind != lpsrSlur::k_NoSlur) {
      idtr++;
      switch (fNoteSlurKind) {
        case lpsrSlur::kStartSlur:
          os << idtr << "Slur start";
          break;
        case lpsrSlur::kContinueSlur:
          os << idtr << "Slur continue";
          break;
        case lpsrSlur::kStopSlur:
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

void lpsrNote::printLilyPondCode(ostream& os)
{
  // print the note name
  os << notePitchAsLilypondString ();
  
  if (! fMusicXMLNoteData.fNoteBelongsToAChord) {
    // print the note duration
    os << fNoteMsrDuration;
    
    // print the dynamics if any
    if (fNoteDynamics.size()) {
      list<S_lpsrDynamics>::const_iterator
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
      list<S_lpsrWedge>::const_iterator
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

string lpsrNote::octaveRepresentation (char octave)
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
S_lpsrSequence lpsrSequence::create (ElementsSeparator elementsSeparator)
{
  lpsrSequence* o = new lpsrSequence (elementsSeparator); assert(o!=0);
  return o;
}

lpsrSequence::lpsrSequence (ElementsSeparator elementsSeparator)
  : lpsrElement("")
{
  fElementsSeparator=elementsSeparator;
}
lpsrSequence::~lpsrSequence() {}

ostream& operator<< (ostream& os, const S_lpsrSequence& elt)
{
  elt->print(os);
  return os;
}

void lpsrSequence::printMSR(ostream& os)
{  
  os << "Sequence";
  
  if (fSequenceElements.size()) {
    os << endl;

    idtr++;
  
    list<S_lpsrElement>::const_iterator
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

void lpsrSequence::printMusicXML(ostream& os)
{
  os << "<!-- lpsrSequence??? -->" << endl;
}

void lpsrSequence::printLilyPondCode(ostream& os)
{
  if (fSequenceElements.size()) {
    list<S_lpsrElement>::const_iterator
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
S_lpsrParallelMusic lpsrParallelMusic::create(ElementsSeparator elementsSeparator)
{
  lpsrParallelMusic* o = new lpsrParallelMusic(elementsSeparator); assert(o!=0);
  return o;
}

lpsrParallelMusic::lpsrParallelMusic(ElementsSeparator elementsSeparator)
  : lpsrElement("")
{
  fElementsSeparator=elementsSeparator;
}
lpsrParallelMusic::~lpsrParallelMusic() {}

ostream& operator<< (ostream& os, const S_lpsrParallelMusic& elt)
{
  elt->print(os);
  return os;
}

void lpsrParallelMusic::printMusicXML(ostream& os)
{
  os << "<!-- lpsrParallelMusic??? -->" << endl;
}

void lpsrParallelMusic::printMSR(ostream& os)
{
  os << "ParallelMusic" << endl;
  
  idtr++;
  
  int size = fParallelMusicElements.size();
  
  for (int i = 0; i < size; i++ ) {
    os << idtr << fParallelMusicElements[i];
  } // for
  
  idtr--;
}

void lpsrParallelMusic::printLilyPondCode(ostream& os)
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
S_lpsrChord lpsrChord::create (S_lpsrDuration chordDuration)
{
  lpsrChord* o = new lpsrChord(chordDuration); assert(o!=0);
  return o;
}

lpsrChord::lpsrChord (S_lpsrDuration chordDuration)
  : lpsrElement("")
{
  fChordDuration = chordDuration;
}
lpsrChord::~lpsrChord() {}

ostream& operator<< (ostream& os, const S_lpsrChord& chrd)
{
  chrd->print(os);
  return os;
}

void lpsrChord::printMusicXML(ostream& os)
{
  os << "<!-- lpsrChord??? -->" << endl;
}

void lpsrChord::printMSR(ostream& os)
{
  os << "Chord" << " " << "<";
  if (fChordNotes.size()) {
    vector<S_lpsrNote>::const_iterator
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
  os << fChordDuration << endl; 

  // print the articulations if any
  if (fChordArticulations.size()) {
    idtr++;
    list<S_lpsrArticulation>::const_iterator i;
    for (i=fChordArticulations.begin(); i!=fChordArticulations.end(); i++) {
      os << idtr << (*i);
    } // for
    idtr--;
  }

  // print the dynamics if any
  if (fChordDynamics.size()) {
    idtr++;
    list<S_lpsrDynamics>::const_iterator i;
    for (i=fChordDynamics.begin(); i!=fChordDynamics.end(); i++) {
      os << idtr << (*i);
    } // for
    idtr--;
  }

  // print the wedges if any
  if (fChordWedges.size()) {
    idtr++;
    list<S_lpsrWedge>::const_iterator i;
    for (i=fChordWedges.begin(); i!=fChordWedges.end(); i++) {
      os << idtr << (*i);
    } // for
    idtr--;
  }
}

void lpsrChord::printLilyPondCode(ostream& os)
{
  os << "<";
  if (fChordNotes.size()) {
    vector<S_lpsrNote>::const_iterator
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
    list<S_lpsrDynamics>::const_iterator i1;
    for (i1=fChordDynamics.begin(); i1!=fChordDynamics.end(); i1++) {
      os << " " << (*i1);
    } // for
  }

  // print the wedges if any
  if (fChordWedges.size()) {
    list<S_lpsrWedge>::const_iterator i2;
    for (i2=fChordWedges.begin(); i2!=fChordWedges.end(); i2++) {
      os << " " << (*i2);
    } // for
  }
}

//______________________________________________________________________________
S_lpsrBarLine lpsrBarLine::create (int nextBarNumber)
{
  lpsrBarLine* o = new lpsrBarLine (nextBarNumber); assert(o!=0);
  return o;
}

lpsrBarLine::lpsrBarLine (int nextBarNumber)
  : lpsrElement("")
{
  fNextBarNumber=nextBarNumber; 
}
lpsrBarLine::~lpsrBarLine() {}

ostream& operator<< (ostream& os, const S_lpsrBarLine& elt)
{
  elt->print(os);
  return os;
}

void lpsrBarLine::printMusicXML(ostream& os)
{
  os << "<!-- lpsrBarLine??? -->" << endl;
}

void lpsrBarLine::printMSR(ostream& os)
{
  os << "BarLine" << " " << fNextBarNumber << endl;
}

void lpsrBarLine::printLilyPondCode(ostream& os)
{
  os << "| % " << fNextBarNumber << endl;
}

//______________________________________________________________________________
S_lpsrComment lpsrComment::create(string contents, GapKind gapKind)
{
  lpsrComment* o = new lpsrComment(contents, gapKind); assert(o!=0);
  return o;
}

lpsrComment::lpsrComment(string contents, GapKind gapKind)
  : lpsrElement("")
{
  fContents=contents;
  fGapKind=gapKind;
}
lpsrComment::~lpsrComment() {}

ostream& operator<< (ostream& os, const S_lpsrComment& elt)
{
  elt->print(os);
  return os;
}

void lpsrComment::printMusicXML(ostream& os)
{
  os << "<!-- lpsrComment??? -->" << endl;
}

void lpsrComment::printMSR(ostream& os)
{
  os << "Comment" << endl;
  idtr++;
  os << idtr << "% " << fContents << endl;
  if (fGapKind == kGapAfterwards) os << idtr << endl;
  idtr--;
}

void lpsrComment::printLilyPondCode(ostream& os)
{
  os << "% " << fContents;
  if (fGapKind == kGapAfterwards) os << endl;
}

//______________________________________________________________________________
S_lpsrBreak lpsrBreak::create(int nextBarNumber)
{
  lpsrBreak* o = new lpsrBreak(nextBarNumber); assert(o!=0);
  return o;
}

lpsrBreak::lpsrBreak(int nextBarNumber)
  : lpsrElement("")
{
  fNextBarNumber=nextBarNumber; 
}
lpsrBreak::~lpsrBreak() {}

ostream& operator<< (ostream& os, const S_lpsrBreak& elt)
{
  elt->print(os);
  return os;
}

void lpsrBreak::printMusicXML(ostream& os)
{
  os << "<!-- lpsrBreak??? -->" << endl;
}

void lpsrBreak::printMSR(ostream& os)
{
  os <<
    "Break" << " " << fNextBarNumber << endl <<
    endl;
}

void lpsrBreak::printLilyPondCode(ostream& os)
{
  os <<
    "\\myBreak | % " << fNextBarNumber << endl <<
    endl;
}

//______________________________________________________________________________
S_lpsrBarNumberCheck lpsrBarNumberCheck::create(int nextBarNumber)
{
  lpsrBarNumberCheck* o = new lpsrBarNumberCheck(nextBarNumber); assert(o!=0);
  return o;
}

lpsrBarNumberCheck::lpsrBarNumberCheck(int nextBarNumber)
  : lpsrElement("")
{
  fNextBarNumber=nextBarNumber; 
}
lpsrBarNumberCheck::~lpsrBarNumberCheck() {}

ostream& operator<< (ostream& os, const S_lpsrBarNumberCheck& elt)
{
  elt->print(os);
  return os;
}

void lpsrBarNumberCheck::printMusicXML(ostream& os)
{
  os << "<!-- lpsrBarNumberCheck??? -->" << endl;
}

void lpsrBarNumberCheck::printMSR(ostream& os)
{
  os << "BarNumberCheck" << " " << fNextBarNumber << endl;
}

void lpsrBarNumberCheck::printLilyPondCode(ostream& os)
{
  os << "\\barNumberCheck #" << fNextBarNumber << endl;
}

//______________________________________________________________________________
S_lpsrTuplet lpsrTuplet::create()
{
  lpsrTuplet* o = new lpsrTuplet(); assert(o!=0);
  return o;
}

lpsrTuplet::lpsrTuplet()
  : lpsrElement("")
{
  fTupletNumber = k_NoTuplet;
  
  fActualNotes = -1;
  fNormalNotes = -1;
}
lpsrTuplet::~lpsrTuplet() {}

void lpsrTuplet::updateTuplet (int number, int actualNotes, int normalNotes)
{
  fTupletNumber = number;
  
  fActualNotes = actualNotes;
  fNormalNotes = normalNotes;  
}

ostream& operator<< (ostream& os, const S_lpsrTuplet& elt)
{
  elt->print(os);
  return os;
}

void lpsrTuplet::printMusicXML(ostream& os)
{
  os << "<!-- lpsrTuplet??? -->" << endl;
}

void lpsrTuplet::printMSR(ostream& os)
{
  os <<
    "Tuplet " << fActualNotes << "/" << fNormalNotes << endl;
  idtr++;
  vector<S_lpsrElement>::const_iterator i;
  for (i=fTupletContents.begin(); i!=fTupletContents.end(); i++) {
    os << idtr << (*i);
  } // for
  idtr--;
}

void lpsrTuplet::printLilyPondCode(ostream& os)
{
  os << "\\tuplet " << fActualNotes << "/" << fNormalNotes << " { ";

  if (fTupletContents.size()) {
    idtr++;
    
    vector<S_lpsrElement>::const_iterator
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
S_lpsrBeam lpsrBeam::create(int number, BeamKind beamKind)
{
  lpsrBeam* o = new lpsrBeam(number, beamKind); assert(o!=0);
  return o;
}

lpsrBeam::lpsrBeam(int number, BeamKind beamKind)
  : lpsrElement("")
{
  fBeamNumber = number;
  fBeamKind   = beamKind; 
}
lpsrBeam::~lpsrBeam() {}

ostream& operator<< (ostream& os, const S_lpsrBeam& dyn)
{
  dyn->print(os);
  return os;
}

void lpsrBeam::printMusicXML(ostream& os)
{
  os << "<!-- lpsrBeam??? -->" << endl;
}

void lpsrBeam::printMSR(ostream& os)
{
  os << "Beam???" << endl;
}

void lpsrBeam::printLilyPondCode(ostream& os)
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
S_lpsrPaper lpsrPaper::create() {
  lpsrPaper* o = new lpsrPaper(); assert(o!=0);
  return o;
}

lpsrPaper::lpsrPaper()
  : lpsrElement("")
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
lpsrPaper::~lpsrPaper() {}

ostream& operator<< (ostream& os, const S_lpsrPaper& pap) {
  pap->print(os);
  return os;
}

void lpsrPaper::printMusicXML(ostream& os)
{
  os << "<!-- lpsrPaper??? -->" << endl;
}

void lpsrPaper::printMSR(ostream& os) {
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

void lpsrPaper::printLilyPondCode(ostream& os)
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
S_lpsrHeader lpsrHeader::create() {
  lpsrHeader* o = new lpsrHeader(); assert(o!=0);
  return o;
}

lpsrHeader::lpsrHeader() : lpsrElement("") {}
lpsrHeader::~lpsrHeader() {}

void lpsrHeader::setWorkNumber (string val)
  {
  fWorkNumber =
    lpsrLilypondVarValAssoc::create (
      "work-number", val,
      lpsrLilypondVarValAssoc::kEqualSign,
      lpsrLilypondVarValAssoc::kQuotesAroundValue,
      lpsrLilypondVarValAssoc::kUncommented);
  }
S_lpsrLilypondVarValAssoc lpsrHeader::getWorkNumber () const
 { return fWorkNumber; }

void lpsrHeader::setWorkTitle (string val)
  {
  fWorkTitle =
    lpsrLilypondVarValAssoc::create (
      "work-title", val,
      lpsrLilypondVarValAssoc::kEqualSign,
      lpsrLilypondVarValAssoc::kQuotesAroundValue,
      lpsrLilypondVarValAssoc::kUncommented);
  }
S_lpsrLilypondVarValAssoc lpsrHeader::getWorkTitle () const
  { return fWorkTitle; }

void lpsrHeader::setMovementNumber (string val)
  {
  fMovementNumber =
    lpsrLilypondVarValAssoc::create (
      "movement-number", val,
      lpsrLilypondVarValAssoc::kEqualSign,
      lpsrLilypondVarValAssoc::kQuotesAroundValue,
      lpsrLilypondVarValAssoc::kUncommented);
  }
S_lpsrLilypondVarValAssoc lpsrHeader::getMovementNumber () const
  { return fMovementNumber; }

void lpsrHeader::setMovementTitle (string val)
{
  fMovementTitle =
    lpsrLilypondVarValAssoc::create (
      "movement-title", val,
      lpsrLilypondVarValAssoc::kEqualSign,
      lpsrLilypondVarValAssoc::kQuotesAroundValue,
      lpsrLilypondVarValAssoc::kUncommented);
}
S_lpsrLilypondVarValAssoc lpsrHeader::getMovementTitle () const
  { return fMovementTitle; }

void lpsrHeader::addCreator (string type, string val)
{
  fCreators.push_back(
    lpsrLilypondVarValAssoc::create (
      type, val,
      lpsrLilypondVarValAssoc::kEqualSign,
      lpsrLilypondVarValAssoc::kQuotesAroundValue,
      lpsrLilypondVarValAssoc::kUncommented)
  );
}
vector<S_lpsrLilypondVarValAssoc> lpsrHeader::getCreators () const
  { return fCreators; };

void lpsrHeader::setRights (string val)
  {
  fRights =
    lpsrLilypondVarValAssoc::create (
      "rights", val,
      lpsrLilypondVarValAssoc::kEqualSign,
      lpsrLilypondVarValAssoc::kQuotesAroundValue,
      lpsrLilypondVarValAssoc::kUncommented);
  }
S_lpsrLilypondVarValAssoc lpsrHeader::getRights () const
  { return fRights; }

void lpsrHeader::addSoftware (string val)
{
  fSoftwares.push_back(
    lpsrLilypondVarValAssoc::create (
      "software", val,
      lpsrLilypondVarValAssoc::kEqualSign,
      lpsrLilypondVarValAssoc::kQuotesAroundValue,
      lpsrLilypondVarValAssoc::kUncommented)
  );
}
vector<S_lpsrLilypondVarValAssoc> lpsrHeader::getSoftwares () const
  { return fSoftwares; };

void lpsrHeader::setEncodingDate (string val)
{
  fEncodingDate =
    lpsrLilypondVarValAssoc::create (
      "encodingdate", val,
      lpsrLilypondVarValAssoc::kEqualSign,
      lpsrLilypondVarValAssoc::kQuotesAroundValue,
      lpsrLilypondVarValAssoc::kUncommented);
}
S_lpsrLilypondVarValAssoc lpsrHeader::getEncodingDate () const
  { return fEncodingDate; }

void lpsrHeader::setScoreInstrument (string val)
{
  fScoreInstrument =
    lpsrLilypondVarValAssoc::create (
      "score-instrument", val,
      lpsrLilypondVarValAssoc::kEqualSign,
      lpsrLilypondVarValAssoc::kQuotesAroundValue,
      lpsrLilypondVarValAssoc::kUncommented);
}
S_lpsrLilypondVarValAssoc lpsrHeader::getScoreInstrument () const
  { return fScoreInstrument; }


void lpsrHeader::printMusicXML(ostream& os)
{
  os << "<!-- lpsrHeader??? -->" << endl;
}

void lpsrHeader::printMSR(ostream& os)
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
    vector<S_lpsrLilypondVarValAssoc>::const_iterator i1;
    for (i1=fCreators.begin(); i1!=fCreators.end(); i1++) {
      os << idtr << (*i1);
    } // for
  }
    
  if (fRights) {
    os << idtr << fRights;
  }
    
  if (!fSoftwares.empty()) {
    vector<S_lpsrLilypondVarValAssoc>::const_iterator i2;
    for (i2=fSoftwares.begin(); i2!=fSoftwares.end(); i2++) {
      os << idtr << (*i2);
    } // for
  }
    
  if (fEncodingDate) {
    os << idtr << fEncodingDate;
  }
  
  idtr--;
}

void lpsrHeader::printLilyPondCode(ostream& os)
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
    vector<S_lpsrLilypondVarValAssoc>::const_iterator i1;
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
    vector<S_lpsrLilypondVarValAssoc>::const_iterator i2;
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
S_lpsrLilypondVarValAssoc lpsrLilypondVarValAssoc::create(
      string     variableName,
      string     value, 
      VarValSeparator varValSeparator,
      QuotesKind      quotesKind,
      CommentedKind   commentedKind,
      string     unit)
{
  lpsrLilypondVarValAssoc* o =
    new lpsrLilypondVarValAssoc(
    variableName, value, varValSeparator, 
    quotesKind, commentedKind, unit);
  assert(o!=0);
  return o;
}

lpsrLilypondVarValAssoc::lpsrLilypondVarValAssoc(
      string     variableName,
      string     value, 
      VarValSeparator varValSeparator,
      QuotesKind      quotesKind,
      CommentedKind   commentedKind,
      string     unit)
  : lpsrElement("")
{
  fVariableName=variableName;
  fVariableValue=value;
  fVarValSeparator=varValSeparator;
  fQuotesKind=quotesKind;
  fCommentedKind=commentedKind;
  fUnit = unit;
}

lpsrLilypondVarValAssoc::~lpsrLilypondVarValAssoc() {}

void lpsrLilypondVarValAssoc::changeAssoc (string value) {
  fVariableValue=value;
}

ostream& operator<< (ostream& os, const S_lpsrLilypondVarValAssoc& assoc) {
  assoc->print(os);
  return os;
}

void lpsrLilypondVarValAssoc::printMusicXML(ostream& os)
{
  os << "<!-- lpsrLilypondVarValAssoc??? -->" << endl;
}

void lpsrLilypondVarValAssoc::printMSR(ostream& os)
{
  os << "LilypondVarValAssoc" << endl;
  idtr++;
  os << idtr << fVariableName << endl;
  os << idtr << fVariableValue <<endl;
  idtr--;
}

void lpsrLilypondVarValAssoc::printLilyPondCode(ostream& os) {
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
S_lpsrSchemeVarValAssoc lpsrSchemeVarValAssoc::create(
      string     variableName,
      string     value, 
      CommentedKind   commentedKind )
{
  lpsrSchemeVarValAssoc* o = new lpsrSchemeVarValAssoc(
    variableName, value, commentedKind);
  assert(o!=0);
  return o;
}

lpsrSchemeVarValAssoc::lpsrSchemeVarValAssoc(
      string     variableName,
      string     value, 
      CommentedKind   commentedKind )
  : lpsrElement("")
{
  fVariableName=variableName;
  fVariableValue=value;
  fCommentedKind=commentedKind;
}

lpsrSchemeVarValAssoc::~lpsrSchemeVarValAssoc() {}

void lpsrSchemeVarValAssoc::changeAssoc (string value)
{
  fVariableValue=value;
}

ostream& operator<< (ostream& os, const S_lpsrSchemeVarValAssoc& assoc)
{
  assoc->print(os);
  return os;
}

void lpsrSchemeVarValAssoc::printMusicXML(ostream& os)
{
  os << "<!-- lpsrSchemeVarValAssoc??? -->" << endl;
}

void lpsrSchemeVarValAssoc::printMSR(ostream& os)
{
  os << "SchemeVarValAssoc" << endl;
  idtr++;
  os << idtr << fVariableName << endl;
  os << idtr << fVariableValue <<endl;
  idtr--;
}

void lpsrSchemeVarValAssoc::printLilyPondCode(ostream& os)
{
  if (fCommentedKind == kCommented) os << "\%";
  os <<
    "#(" << 
    fVariableName << " " << fVariableValue << 
    ")" << endl;
}

//______________________________________________________________________________
S_lpsrLayout lpsrLayout::create() {
  lpsrLayout* o = new lpsrLayout(); assert(o!=0);
  return o;
}

lpsrLayout::lpsrLayout() : lpsrElement("") {}
lpsrLayout::~lpsrLayout() {}

ostream& operator<< (ostream& os, const S_lpsrLayout& lay)
{
  lay->print(os);
  return os;
}

void lpsrLayout::printMusicXML(ostream& os)
{
  os << "<!-- lpsrLayout??? -->" << endl;
}

void lpsrLayout::printMSR(ostream& os)
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

void lpsrLayout::printLilyPondCode(ostream& os)
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
S_lpsrClef lpsrClef::create (
  string sign,
  int    line,
  int    octaveChange)
{
  lpsrClef* o =
    new lpsrClef(sign, line, octaveChange); assert(o!=0);
  return o;
}

lpsrClef::lpsrClef(
  string sign,
  int    line,
  int    octaveChange)
  : lpsrElement("")
{
  fSign         = sign;
  fLine         = line;
  fOctaveChange = octaveChange;
}

lpsrClef::~lpsrClef() {}

ostream& operator<< (ostream& os, const S_lpsrClef& clef)
{
  clef->print(os);
  return os;
}

void lpsrClef::printMusicXML(ostream& os)
{
  os << "<!-- lpsrClef??? -->" << endl;
}

void lpsrClef::printMSR(ostream& os)
{
  os <<
    "Clef" << " \"" << fSign << "\"" <<
    " line: " << fLine << ", octaveChange: " << fOctaveChange;
}

void lpsrClef::printLilyPondCode(ostream& os)
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
S_lpsrKey lpsrKey::create (
  int fifths, string mode, int cancel)
{
  lpsrKey* o = new lpsrKey (fifths, mode, cancel);
  assert (o!=0);
  return o;
}

lpsrKey::lpsrKey (
  int fifths, string mode, int cancel)
    : lpsrElement("")
{
  fFifths = fifths;
  fMode   = mode;
  fCancel = cancel;

  string           tonicNote;
  lpsrKey::KeyMode keyMode;
  
  switch (fFifths) {
    case 0:
      tonicNote = "c";
      keyMode = lpsrKey::kMajor;
      break;
    case 1:
      tonicNote = "g";
      keyMode = lpsrKey::kMajor;
      break;
    case 2:
      tonicNote = "d";
      keyMode = lpsrKey::kMajor;
      break;
    case 3:
      tonicNote = "a";
      keyMode = lpsrKey::kMajor;
      break;
    case 4:
      tonicNote = "e";
      keyMode = lpsrKey::kMajor;
      break;
    case 5:
      tonicNote = "b";
      keyMode = lpsrKey::kMajor;
      break;
    case 6:
       tonicNote = "fis";
      keyMode = lpsrKey::kMajor;
      break;
    case 7:
      tonicNote = "cis";
      keyMode = lpsrKey::kMajor;
      break;
    case -1:
      tonicNote = "f";
      keyMode = lpsrKey::kMajor;
      break;
    case -2:
      tonicNote = "bes";
      keyMode = lpsrKey::kMajor;
      break;
    case -3:
      tonicNote = "ees";
      keyMode = lpsrKey::kMajor;
      break;
    case -4:
      tonicNote = "aes";
      keyMode = lpsrKey::kMajor;
      break;
    case -5:
      tonicNote = "des";
      keyMode = lpsrKey::kMajor;
      break;
    case -6:
      tonicNote = "ges";
      keyMode = lpsrKey::kMajor;
      break;
    case -7:
      tonicNote = "ces";
      keyMode = lpsrKey::kMajor;
      break;
    default: // unknown key sign !!
      {
      stringstream s;
      string  message;
      s << 
        "ERROR: unknown key sign \"" << fFifths << "\"" << endl;
      s >> message;
      lpsrMusicXMLError(message);
      }
  } // switch
  
  fTonic   = tonicNote;
  fKeyMode = keyMode; 
}

lpsrKey::~lpsrKey() {}

ostream& operator<< (ostream& os, const S_lpsrKey& key)
{
  key->print(os);
  return os;
}

void lpsrKey::printMusicXML(ostream& os)
{
  os << "<!-- lpsrKey??? -->" << endl;
}

void lpsrKey::printMSR(ostream& os)
{
  os << "Key " << fTonic << " ";
  if (fKeyMode == kMajor) os << "\\major";
  else os << "\\minor";
}

void lpsrKey::printLilyPondCode(ostream& os)
{
  os << "\\key " << fTonic << " ";
  if (fKeyMode == kMajor) os << "\\major";
  else os << "\\minor";
  os << endl;
}

//______________________________________________________________________________
S_lpsrTime lpsrTime::create (
  int numerator, int denominator, bool generateNumericalTime)
{
  lpsrTime* o =
    new lpsrTime (numerator, denominator, generateNumericalTime);
  assert (o!=0);
  return o;
}

lpsrTime::lpsrTime(
  int numerator, int denominator, bool generateNumericalTime)
  : lpsrElement("")
{
  fRational = rational (numerator, denominator);
// JMI  fNumerator=numerator; 
  //fDenominator=denominator;
  fGenerateNumericalTime = generateNumericalTime;
}
lpsrTime::~lpsrTime() {}


ostream& operator<< (ostream& os, const S_lpsrTime& elt)
{
  elt->print(os);
  return os;
}

void lpsrTime::printMusicXML(ostream& os)
{
  os << "<!-- lpsrTime??? -->" << endl;
}

void lpsrTime::printMSR(ostream& os)
{
  os <<
    "Time " << 
    fRational.getNumerator() << "/" << fRational.getDenominator();
}

void lpsrTime::printLilyPondCode(ostream& os)
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
S_lpsrMidi lpsrMidi::create()
{
  lpsrMidi* o = new lpsrMidi(); assert(o!=0);
  return o;
}

lpsrMidi::lpsrMidi()
  : lpsrElement("")
{
}
lpsrMidi::~lpsrMidi() {}

ostream& operator<< (ostream& os, const S_lpsrMidi& mid)
{
  mid->print(os);
  return os;
}

void lpsrMidi::printMusicXML(ostream& os)
{
  os << "<!-- lpsrMidi??? -->" << endl;
}

void lpsrMidi::printMSR(ostream& os)
{
  os << "Midi" << endl;

  idtr++;
  
  os << idtr << "% to be completed" << endl;
  
  idtr--;
}

void lpsrMidi::printLilyPondCode(ostream& os)
{  
  os << idtr << "\\midi {" << endl;
  
  idtr++;
  
  os << idtr << "% to be completed" << endl;
  
  idtr--;
  
  os << idtr << "}" << endl;
}

//______________________________________________________________________________
S_lpsrRepeat lpsrRepeat::create()
{
  lpsrRepeat* o = new lpsrRepeat(); assert(o!=0);
  return o;
}

lpsrRepeat::lpsrRepeat() : lpsrElement("")
{
  fActuallyUsed = false;
}
lpsrRepeat::~lpsrRepeat() {}

ostream& operator<< (ostream& os, const S_lpsrRepeat& rept)
{
  rept->print(os);
  return os;
}

void lpsrRepeat::printMusicXML(ostream& os)
{
  os << "<!-- lpsrRepeat??? -->" << endl;
}

void lpsrRepeat::printMSR(ostream& os)
{
  os << "Repeat" << endl;
  idtr++;
    os << idtr << fCommonPart;
    vector<S_lpsrSequence>::const_iterator i;
    for (i=fAlternateEndings.begin(); i!=fAlternateEndings.end(); i++) {
      os << idtr << (*i);
    } // for
  idtr--;
}

void lpsrRepeat::printLilyPondCode(ostream& os)
{
  os << "Repeat" << endl;
}

//______________________________________________________________________________
S_lpsrLyricsChunk lpsrLyricsChunk::create (
  LyricsChunkType chunkType,
  string          chunkText,
  S_lpsrDuration  duration)
{
  lpsrLyricsChunk* o =
    new lpsrLyricsChunk (chunkType, chunkText, duration);
  assert(o!=0);
  return o;
}

lpsrLyricsChunk::lpsrLyricsChunk (
  LyricsChunkType chunkType,
  string          chunkText,
  S_lpsrDuration  duration)
  : lpsrElement("")
{
  fLyricsChunkType = chunkType;
  fChunkText       = chunkText;
  fChunkDuration   = duration;
}
lpsrLyricsChunk::~lpsrLyricsChunk() {}

ostream& operator<< (ostream& os, const S_lpsrLyricsChunk& lyr)
{
  lyr->print(os);
  return os;
}

void lpsrLyricsChunk::printMusicXML(ostream& os)
{
  os << "<!-- lpsrLyricsChunk??? -->" << endl;
}

void lpsrLyricsChunk::printMSR(ostream& os)
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

void lpsrLyricsChunk::printLilyPondCode(ostream& os)
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
S_lpsrLyrics lpsrLyrics::create (
  S_translationSettings& ts,
  int                    lyricsNumber,
  S_lpsrVoice            lyricsVoice)
{
  lpsrLyrics* o = new lpsrLyrics (ts, lyricsNumber, lyricsVoice);
  assert(o!=0);
  return o;
}

lpsrLyrics::lpsrLyrics (
  S_translationSettings& ts,
  int                    lyricsNumber,
  S_lpsrVoice            lyricsVoice)
    : lpsrElement("")
{
  fTranslationSettings = ts;
  fLyricsNumber        = lyricsNumber;
  fLyricsVoice         = lyricsVoice; 
 
  fLyricsTextPresent = false;
}

string lpsrLyrics::getLyricsName () const
{
  // not stored in a field,
  // because the lyrics voice and staff may change name
  // when the part they belong to is re-used
  return
   fLyricsVoice->getVoiceName() +
    "_Lyrics_" +
    int2EnglishWord (fLyricsNumber);
}

lpsrLyrics::~lpsrLyrics() {}

void lpsrLyrics::addTextChunkToLyrics (
  string          syllabic,
  string          text,
  bool            elision,
  S_lpsrDuration  lpsrDuration)
{
  lpsrLyricsChunk::LyricsChunkType chunkType;
  
  if      (syllabic == "single")
    chunkType = lpsrLyricsChunk::kSingleChunk;
  else if (syllabic == "begin")
    chunkType = lpsrLyricsChunk::kBeginChunk;
  else if (syllabic == "middle")
    chunkType = lpsrLyricsChunk::kMiddleChunk;
  else if (syllabic == "end")
    chunkType = lpsrLyricsChunk::kEndChunk;
  else {
    stringstream s;
    string  result;
    s << "--> syllabic = " << syllabic << " is unknown";
    s >> result;
    lpsrMusicXMLError(result);
  }

  // create a lyrics text chunk
  if (fTranslationSettings->fDebug) {
    S_lpsrStaff staff = fLyricsVoice->getVoiceStaff();
    S_lpsrPart  part  = staff-> getStaffPart();
    
    cout << idtr <<
      "--> creating a lyrics " << fLyricsNumber <<
      " text chunk \"" << syllabic <<
      "\" in voice " << fLyricsVoice->getVoiceNumber() <<
      " of staff " << staff->getStaffNumber() <<
      " in part "  << part->getPartCombinedName() <<
      ", containing \"" << text <<
      "\", elision: " << elision <<
      ", duration: " << lpsrDuration << endl;
  }

  S_lpsrLyricsChunk
    chunk =
      lpsrLyricsChunk::create (
        chunkType, text, lpsrDuration);
  
  switch (chunkType) {
    case lpsrLyricsChunk::kSingleChunk:
    case lpsrLyricsChunk::kBeginChunk:
      {  
      // add lyrics chunk to this lyrics
      fLyricsChunks.push_back (chunk);
      }
      break;

    case lpsrLyricsChunk::kMiddleChunk:
    case lpsrLyricsChunk::kEndChunk:
      // add chunk to this lyrics
      fLyricsChunks.push_back (chunk);
      break;
      
    case lpsrLyricsChunk::kSkipChunk:
    case lpsrLyricsChunk::kBreakChunk:
      {
        // internalError (); JMI
      }
      break;
  } // switch

  fLyricsTextPresent = true;
}

void lpsrLyrics::addSkipChunkToLyrics (
  S_lpsrDuration  lpsrDuration)
{
  if (fTranslationSettings->fDebug) {
    S_lpsrStaff staff = fLyricsVoice->getVoiceStaff();
    S_lpsrPart  part  = staff-> getStaffPart();
    
    cout << idtr <<
      "--> creating a lyrics skip chunk, duration: " <<
      lpsrDuration <<
      " in voice " << fLyricsVoice->getVoiceNumber() <<
      " of staff " << staff->getStaffNumber() <<
      " in part "  << part->getPartCombinedName() << endl;
  }
  
  // create lyrics skip chunk
  S_lpsrLyricsChunk
    chunk =
      lpsrLyricsChunk::create (
        lpsrLyricsChunk::kSkipChunk, "", lpsrDuration);
        
  // add chunk to this lyrics
  fLyricsChunks.push_back (chunk);
}

void lpsrLyrics::addBreakChunkToLyrics (
  int nextMeasureNumber)
{
  if (fTranslationSettings->fDebug) {
    S_lpsrStaff staff = fLyricsVoice->getVoiceStaff();
    S_lpsrPart  part  = staff-> getStaffPart();
    
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

  S_lpsrDuration
    nullMsrDuration =
      lpsrDuration::create (0, 0, 0, "");
        
  S_lpsrLyricsChunk
    chunk =
      lpsrLyricsChunk::create (
        lpsrLyricsChunk::kBreakChunk, str, nullMsrDuration);
        
  // add chunk to this lyrics
  fLyricsChunks.push_back (chunk);
}

ostream& operator<< (ostream& os, const S_lpsrLyrics& stan)
{
  stan->print(os);
  return os;
}

void lpsrLyrics::printMusicXML(ostream& os)
{
  os << "<!-- lpsrLyrics??? -->" << endl;
}

void lpsrLyrics::printMSR(ostream& os)
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

void lpsrLyrics::printLilyPondCode(ostream& os)
{
  os <<
    getLyricsName () << " = " << "\\lyricmode {" << endl;

  idtr++;

  if (fLyricsChunks.size()) {
    vector<S_lpsrLyricsChunk>::const_iterator
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
S_lpsrVoice lpsrVoice::create (
  S_translationSettings& ts,
  int                    voiceNumber,
  S_lpsrStaff            voiceStaff)
{
  lpsrVoice* o = new lpsrVoice (ts, voiceNumber, voiceStaff);
  assert(o!=0);
  return o;
}

lpsrVoice::lpsrVoice (
  S_translationSettings& ts,
  int                    voiceNumber,
  S_lpsrStaff            voiceStaff)
    : lpsrElement("")
{
  fTranslationSettings = ts;

  fVoiceNumber = voiceNumber;
  fVoiceStaff  = voiceStaff;
  
  // create the implicit lpsrSequence element
  fVoiceSequence =
    lpsrSequence::create (lpsrSequence::kSpace);
  
  // add the implicit lpsrRepeat element
// JMI  fVoiceMsrRepeat = lpsrRepeat::create ();
//  fVoiceSequence->appendElementToSequence (fVoiceMsrRepeat);
  
  // add the implicit 4/4 time signature JMI
 // S_lpsrTime time = lpsrTime::create (4, 4, fGenerateNumericalTime);
 // S_lpsrElement t = time;
  //fVoiceSequence->appendElementToSequence (t);
}

lpsrVoice::~lpsrVoice() {}

string lpsrVoice::getVoiceName () const
{
  // not stored in a field,
  // because the voice staff may change name
  // when the part it belongs to is re-used
  return
    fVoiceStaff->getStaffName() +
    "_Voice_" +
    int2EnglishWord (fVoiceNumber);
}

S_lpsrLyrics lpsrVoice::addLyricsToVoice (
  int lyricsNumber)
{
  if (fVoiceLyricsMap.count (lyricsNumber)) {
    cerr << idtr <<
      "### Internal error: lyrics " << lyricsNumber <<
      " already exists in this voice" << endl;

    return fVoiceLyricsMap [lyricsNumber];
  }

  // create the lyrics
  S_lpsrLyrics
    lyrics =
      lpsrLyrics::create (
        fTranslationSettings,
        lyricsNumber,
        this);

  // register it in this staff
  fVoiceLyricsMap [lyricsNumber] = lyrics;

  // return it
  return lyrics;
}

S_lpsrLyrics lpsrVoice::voiceContainsLyrics (
  int lyricsNumber)
{
  S_lpsrLyrics result;
  
  if (fVoiceLyricsMap.count (lyricsNumber)) {
    result = fVoiceLyricsMap [lyricsNumber];
  }

  return result;
}

ostream& operator<< (ostream& os, const S_lpsrVoice& elt)
{
  elt->print(os);
  return os;
}

void lpsrVoice::printMusicXML(ostream& os)
{ 
  os << "<!-- lpsrVoice??? -->" << endl;
}

void lpsrVoice::printMSR(ostream& os)
{
  os << "Voice" << " " << getVoiceName () << endl << endl;

  idtr++;

  os << idtr << fVoiceSequence << endl;

  if (fVoiceLyricsMap.size()) {
    for (
      map<int, S_lpsrLyrics>::const_iterator i = fVoiceLyricsMap.begin();
      i != fVoiceLyricsMap.end();
      i++) {
      os << idtr << (*i).second;
    } // for
    os << endl;
  }
  
  idtr--;
}

void lpsrVoice::printLilyPondCode(ostream& os)
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
S_lpsrStaff lpsrStaff::create (
  S_translationSettings& ts,
  int                    staffNumber,
  S_lpsrPart             staffPart)
{
  lpsrStaff* o = new lpsrStaff( ts, staffNumber, staffPart);
  assert(o!=0);
  return o;
}

lpsrStaff::lpsrStaff (
  S_translationSettings& ts,
  int                    staffNumber,
  S_lpsrPart             staffPart)
    : lpsrElement("")
{
  fTranslationSettings = ts;
  
  fStaffNumber = staffNumber;
  fStaffPart   = staffPart;
}

lpsrStaff::~lpsrStaff() {}

string lpsrStaff::getStaffName () const
  {
  // not stored in a field,
  // because the staff part may change name
  // when it is re-used
  return
    fStaffPart->getPartMSRName() +
    "_Staff_" +
    int2EnglishWord (fStaffNumber);
  }

S_lpsrVoice lpsrStaff::addVoiceToStaff (
  int voiceNumber)
{
  if (fStaffVoicesMap.count (voiceNumber)) {
    cerr << idtr <<
      "### Internal error: voice " << voiceNumber <<
      " already exists in this staff" << endl;

    return fStaffVoicesMap [voiceNumber];
  }

  if (fTranslationSettings->fTrace)
    cerr << idtr <<
      "Adding voice " << voiceNumber <<
      " to staff " << fStaffNumber <<
      " in part " << fStaffPart->getPartCombinedName () << endl;
  
  // create the voice
  S_lpsrVoice
    voice =
      lpsrVoice::create (
        fTranslationSettings,
        voiceNumber,
        this);

  // register it in this staff
  fStaffVoicesMap [voiceNumber] = voice;

  // return it
  return voice;
}
              
S_lpsrVoice lpsrStaff::staffContainsVoice (
  int voiceNumber)
{
  S_lpsrVoice result;
  
  if (fStaffVoicesMap.count (voiceNumber)) {
    result = fStaffVoicesMap [voiceNumber];
  }

  return result;
}

ostream& operator<< (ostream& os, const S_lpsrStaff& elt)
{
  elt->print(os);
  return os;
}

void lpsrStaff::printMusicXML (ostream& os)
{
  os << "<!-- lpsrStaff??? -->" << endl;
}

void lpsrStaff::printMSR (ostream& os)
{
  os << "Staff" << " " << getStaffName () << endl;

  idtr++;

  os <<
    idtr << "StaffInstrumentName: \"" <<
    fStaffInstrumentName << "\"" << endl;
  
  for (
    map<int, S_lpsrVoice>::iterator i = fStaffVoicesMap.begin();
    i != fStaffVoicesMap.end();
    i++) {
    os << idtr << (*i).second;
  } // for

  idtr--;
}

void lpsrStaff::printLilyPondCode (ostream& os)
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
S_lpsrPart lpsrPart::create (
    S_translationSettings& ts,
    string                 partMusicXMLName)
{
  lpsrPart* o = new lpsrPart( ts, partMusicXMLName);
  assert(o!=0);
  return o;
}

lpsrPart::lpsrPart (
  S_translationSettings& ts,
  string                 partMusicXMLName)
    : lpsrElement("")
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

lpsrPart::~lpsrPart() {}

void lpsrPart::changePartMusicXMLName (
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

ostream& operator<< (ostream& os, const S_lpsrPart& elt)
{
  elt->print(os);
  return os;
}

void lpsrPart::printMusicXML(ostream& os)
{
  os << "<!-- lpsrPart??? -->" << endl;
}

void lpsrPart::printMSR(ostream& os)
{
  os <<
    "Part" << " \"" << getPartCombinedName () << endl;
    
  idtr++;
  
  os <<
    idtr << "PartName          : \"" << fPartName << "\"" << endl <<
    idtr << "PartAbbrevation   : \"" << fPartAbbreviation << "\"" << endl;
  
  os << idtr <<
    "fPartMusicXMLDivisions: " << fPartMusicXMLDivisions << endl;

  if (fPartClef)
    os << idtr << fPartClef << endl;
  else
    os << idtr << "NO_CLEF" << endl;

  if (fPartKey)
    os << idtr << fPartKey << endl;
  else
    os << idtr << "NO_KEY" << endl;

  if (fPartTime)
    os << idtr << fPartTime << endl;
  else
    os << idtr << "NO_TIME" << endl;

  os << idtr <<
    "PartInstrumentName: \"" << fPartInstrumentName << "\"" << endl;

  for (
    map<int, S_lpsrStaff>::iterator i = fPartStavesMap.begin();
    i != fPartStavesMap.end();
    i++) {
    os << idtr << (*i).second;
  } // for
  
  idtr--;
}

void lpsrPart::printLilyPondCode(ostream& os)
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

S_lpsrStaff lpsrPart::addStaffToPart (
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
  S_lpsrStaff
    staff =
      lpsrStaff::create (
        fTranslationSettings,
        staffNumber,
        this);

  // register it in this part
  fPartStavesMap [staffNumber] = staff;

  // return it
  return staff;
}

S_lpsrStaff lpsrPart::partContainsStaff (
  int staffNumber)
{
  S_lpsrStaff result;
  
  if (fPartStavesMap.count (staffNumber)) {
    result = fPartStavesMap [staffNumber];
  }

  return result;
}

//______________________________________________________________________________
S_lpsrPartGroup lpsrPartGroup::create (
  S_translationSettings& ts,
  int                    partGroupNumber)
{
  lpsrPartGroup* o = new lpsrPartGroup (ts, partGroupNumber);
  assert(o!=0);
  return o;
}

lpsrPartGroup::lpsrPartGroup (
  S_translationSettings& ts,
  int                    partGroupNumber)
    : lpsrElement("")
{
  fTranslationSettings = ts;

  fPartGroupNumber = partGroupNumber;
  
  if (fTranslationSettings->fTrace)
    cerr << idtr <<
      "Creating part group " << fPartGroupNumber << endl;
}

lpsrPartGroup::~lpsrPartGroup() {}

S_lpsrPart lpsrPartGroup::addPartToPartGroup (
  string partMusicXMLName)
{
  if (fPartGroupPartsMap.count (partMusicXMLName)) {
    cerr << idtr <<
      "### Internal error: partMusicXMLName " << partMusicXMLName <<
      " already exists in this part group" << endl;

    return fPartGroupPartsMap [partMusicXMLName];
  }

  // create the part
  S_lpsrPart part;

//  part =
  // JMI  tryAndReUseInitialAnonymousPart (partMusicXMLName);

  if (part) {
    
    // the anonymous part is being re-used
    return part;

  } else {
    
    part =
      lpsrPart::create (
        fTranslationSettings, partMusicXMLName);

    if (fTranslationSettings->fTrace)
      cerr << idtr <<
        "Adding part " << part->getPartCombinedName () <<
        " to part group " << fPartGroupNumber << endl;
  
    // register it in this part group
    fPartGroupPartsMap [partMusicXMLName] = part;
  
    // push it on top the this part group's stack
    if (fTranslationSettings->fTrace) {
      cerr << idtr <<
        "Pushing part " << part->getPartCombinedName () <<
        " onto part group " << fPartGroupNumber << " stack" << endl;
    }

    // register it in this part group
    fPartGroupPartsStack.push (part);
  }

  if (fTranslationSettings->fTrace) {
    cerr << idtr << "==> After addPartToPartGroup, fPartGroupPartsMap contains:" << endl;
    for (
        lpsrPartsMap::const_iterator i = fPartGroupPartsMap.begin();
        i != fPartGroupPartsMap.end();
        i++) {
      cerr << idtr <<
        "\"" << (*i).first << "\" ----> " <<
        (*i).second->getPartCombinedName() << endl;
    } // for
    cerr << idtr << "<== partGroupContainsPart" << endl;
  }

  // return it
  return part;
}

S_lpsrPart lpsrPartGroup::partGroupContainsPart (
  string partMusicXMLName)
{
  /*
  cerr << idtr << "==> partGroupContainsPart, fPartGroupPartsMap contains:" << endl;
  for (
      lpsrPartsMap::const_iterator i = fPartGroupPartsMap.begin();
      i != fPartGroupPartsMap.end();
      i++) {
    cerr << idtr <<
      (*i).first << " ----> " <<
      (*i).second->getPartCombinedName() << endl;
  } // for
  cerr << idtr << "<== partGroupContainsPart" << endl;
  */
  
  S_lpsrPart result;
  
  if (fPartGroupPartsMap.count (partMusicXMLName)) {
    result = fPartGroupPartsMap [partMusicXMLName];
  }

  return result;
}

S_lpsrPart lpsrPartGroup::tryAndReUseInitialAnonymousPart (
  string partMusicXMLName)
{
  /*
  cerr << idtr << "==> tryAndReUseInitialAnonymousPart, fPartGroupPartsMap contains:" << endl;
  for (
      lpsrPartsMap::const_iterator i = fPartGroupPartsMap.begin();
      i != fPartGroupPartsMap.end();
      i++) {
    cerr << idtr <<
      (*i).first << " ----> " <<
      (*i).second->getPartCombinedName() << endl;
  } // for
  cerr << idtr << "<== tryAndReUseInitialAnonymousPart" << endl;
  */

  S_lpsrPart result;

  if (fPartGroupPartsStack.size ()) {
 //   S_lpsrPart stackTopPart = fPartGroupPartsStack.top ();

    lpsrPartsMap::iterator i =
      fPartGroupPartsMap.find (partMusicXMLName);
      
//    if (! stackTopPart->getPartMusicXMLName().size()) {
    if (i != fPartGroupPartsMap.end()) {
      // this is the first true part, re-use the one
      // created with an empty name initially
      (*i).second->
        changePartMusicXMLName (partMusicXMLName);
//      result = stackTopPart;
      result = (*i).second;
    }
  }

/*
  cerr << idtr << "==> tryAndReUseInitialAnonymousPart, fPartGroupPartsMap contains:" << endl;
  for (
      lpsrPartsMap::const_iterator i = fPartGroupPartsMap.begin();
      i != fPartGroupPartsMap.end();
      i++) {
    cerr << idtr <<
      (*i).first << " ----> " <<
      (*i).second->getPartCombinedName() << endl;
  } // for
  cerr << idtr << "<== tryAndReUseInitialAnonymousPart" << endl;
*/

  return result;
}

void lpsrPartGroup::popPartGroupPartsStackTop ()
{
  if (fTranslationSettings->fTrace) {
    S_lpsrPart
      stackTopPart = fPartGroupPartsStack.top ();
      
    cerr << idtr <<
      "Popping part " << stackTopPart->getPartCombinedName () <<
      " from part group " << fPartGroupNumber << " stack" << endl;
  }
  
  fPartGroupPartsStack.pop ();
}

ostream& operator<< (ostream& os, const S_lpsrPartGroup& elt)
{
  elt->print(os);
  return os;
}

void lpsrPartGroup::printMusicXML(ostream& os)
{
  os << "<!-- lpsrPartGroup??? -->" << endl;
}

void lpsrPartGroup::printMSR(ostream& os)
{
  os <<
    "PartGroup" << " " << fPartGroupNumber << endl;
    
  idtr++;

  os <<
    idtr << "PartGroupName        : \"" << fPartGroupName << "\"" << endl <<
    idtr << "PartGroupAbbrevation : \"" << fPartGroupAbbreviation << "\"" << endl <<
    idtr << "PartGroupSymbol      : \"" << fPartGroupSymbol << "\"" << endl <<
    idtr << "PartGroupBarline     : \"" << fPartGroupBarline << "\"" << endl;

 // JMI   idtr << "PartGroupInstrumentName: \"" << fPartGroupInstrumentName << "\"" << endl;

  for (
    lpsrPartsMap::iterator i = fPartGroupPartsMap.begin();
    i != fPartGroupPartsMap.end();
    i++) {
    os << idtr << (*i).second;
  } // for
  
  idtr--;
}

void lpsrPartGroup::printLilyPondCode(ostream& os)
{
  os <<
    "PartGroup" << " " << fPartGroupNumber << endl;

  idtr++;
// JMI  os << fPartGroupMsrSequence << endl;
  idtr--;

  os << idtr << "}" << endl;
}

//______________________________________________________________________________
S_lpsrScore lpsrScore::create (
  S_translationSettings& ts)
{
  lpsrScore* o = new lpsrScore (ts);
  assert(o!=0);
  return o;
}

lpsrScore::lpsrScore (
  S_translationSettings& ts)
    : lpsrElement("")
{
  fTranslationSettings = ts;
}
lpsrScore::~lpsrScore() {}

S_lpsrPartGroup lpsrScore::addPartGroupToScore (
  int partGroupNumber)
{
  if (fScorePartGroupsMap.count (partGroupNumber)) {
    cerr << idtr <<
      "### Internal error: part group " << partGroupNumber <<
      " already exists in this score" << endl;

    return fScorePartGroupsMap [partGroupNumber];
  }

  // create the part group
  S_lpsrPartGroup
    partGroup =
      lpsrPartGroup::create (
        fTranslationSettings, partGroupNumber);

  // register it in this score
  fScorePartGroupsMap [partGroupNumber] = partGroup;

   return partGroup;
}
  
S_lpsrPartGroup lpsrScore::ScoreContainsPartGroup (
  int partGroupNumber)
{
  S_lpsrPartGroup result;
  
  if (fScorePartGroupsMap.count (partGroupNumber)) {
    result = fScorePartGroupsMap [partGroupNumber];
  }

  return result;
}

void lpsrScore::printMusicXML (ostream& os)
{
  os << "<!-- lpsrScore??? -->" << endl;
}

ostream& operator<< (ostream& os, const S_lpsrScore& elt)
{
  elt->print(os);
  return os;
}

void lpsrScore::printMSR (ostream& os)
{
  os << "Score" << endl;

  idtr++;
  
  for (
    lpsrPartGroupsMap::iterator i = fScorePartGroupsMap.begin();
    i != fScorePartGroupsMap.end();
    i++) {
    os << idtr << (*i).second;
  } // for
  
  idtr--;
}

void lpsrScore::printLilyPondCode (ostream& os)
{
  printMSR (os);
}


}
