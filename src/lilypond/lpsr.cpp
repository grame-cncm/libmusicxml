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

#include "lpsr.h"

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
  //   - the LPSR structure
  //   - MusicXML text
  //   - LilyPond source code
switch (lpsrGlobalVariables::getCodeGenerationKind()) {
    case lpsrGlobalVariables::kLPSR:
      this->printLPSR (os);
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

void lpsrElement::printLPSR(ostream& os)
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
    "  " << left << setw(26) << "fTupletMemberType = " << fTupletMemberType <<  endl <<
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
  fLpsrOctave = musicxmlOctave - 3;
  /*
  cout <<
    "lpsrAbsoluteOctave::lpsrAbsoluteOctave (), musicxmlOctave = " << musicxmlOctave << 
    ", fLpsrOctave = " << fLpsrOctave << endl;
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
  
  s << fLpsrOctave;
  s >> result;
  return result;
}

void lpsrAbsoluteOctave::printLPSR(ostream& os)
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
  int num,
  int denom,
  int dots,
  string tupletMemberType)
{
  lpsrDuration * o =
    new lpsrDuration (num, denom, dots, tupletMemberType);
  assert(o!=0); 
  return o;
}

lpsrDuration::lpsrDuration (
    int num,
    int denom,
    int dots,
    string tupletMemberType)
  : lpsrElement("")
{
  fNum   = num;
  fDenom = denom;
  fDots  = dots;
  fTupletMemberType = tupletMemberType;
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
  // divisions are per quater, Lpsr durations are in whole notes
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

  if (fTupletMemberType.size()) {

    if      (fTupletMemberType == "256th")   { s << "256"; }
    else if (fTupletMemberType == "128th")   { s << "128"; } 
    else if (fTupletMemberType == "64th")    { s << "64"; } 
    else if (fTupletMemberType == "32nd")    { s << "32"; } 
    else if (fTupletMemberType == "16th")    { s << "16"; } 
    else if (fTupletMemberType == "eighth")  { s << "8"; } 
    else if (fTupletMemberType == "quarter") { s << "4"; } 
    else if (fTupletMemberType == "half")    { s << "2"; } 
    else if (fTupletMemberType == "whole")   { s << "1"; } 
    else if (fTupletMemberType == "breve")   { s << "breve"; } 
    else if (fTupletMemberType == "long")    { s << "long"; }
    else
      {
        stringstream s;
      string  message;
      s << 
        endl << 
        "--> unknown tuplet member type " << fTupletMemberType <<
        endl;
      s >> message;
      lpsrMusicXMLError(message);
      }
        
  } else {
    
    div_t divresult = div (noteDivisions, divisionsPerWholeNote);  
    int   div = divresult.quot;
    int   mod = divresult.rem;
        
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

void lpsrDuration::printLPSR(ostream& os)
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

void lpsrDynamics::printLPSR(ostream& os)
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

string lpsrWedge::wedgeKinsAsString ()
{
  stringstream s;
  
  switch (fWedgeKind) {
    case lpsrWedge::kCrescendoWedge:
      s << "\\<";
      break;
    case lpsrWedge::kDecrescendoWedge:
      s << "\\>";
      break;
    case lpsrWedge::kStopWedge:
      s << "\\!";
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

void lpsrWedge::printLPSR(ostream& os)
{
  os << "Wedge" << " " << wedgeKinsAsString () << endl;
}

void lpsrWedge::printLilyPondCode(ostream& os)
{
  os << wedgeKinsAsString ();
}

//______________________________________________________________________________
S_lpsrNote lpsrNote::createFromMusicXMLData (
  S_translationSettings& ts,
  musicXMLNoteData&      mxmldat)
{  
  lpsrNote * o = new lpsrNote (ts, mxmldat); assert(o!=0); 
  return o;
}

lpsrNote::lpsrNote (
  S_translationSettings& ts,
  musicXMLNoteData&      mxmldat)
  :
    lpsrElement(""),
    fMusicXMLNoteData (mxmldat)
{
  fTranslationSettings = ts;

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

//  cout << "=== xmlPart2LpsrVisitor::visitStart ( S_step& elt ) " << fCurrentMusicXMLStep << endl;
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
  
  stringstream s;
  string  message;
  s <<
    "MusicXML alteration " << fMusicXMLNoteData.fMusicxmlAlteration <<
    " is not between -2 and +2";
  s >> message;
  lpsrAssert(
    fMusicXMLNoteData.fMusicxmlAlteration>=-2
      &&
    fMusicXMLNoteData.fMusicxmlAlteration<=+2,
    message);
  
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
   } // switch

  fNoteLpsrPitch = 
    computeNoteLpsrPitch (noteQuatertonesFromA, mxmlAlteration);
  
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

  fNoteLpsrDuration =
    lpsrDuration::create (
      fMusicXMLNoteData.fMusicxmlDuration,
      divisionsPerWholeNote,
      fMusicXMLNoteData.fDotsNumber,
      fMusicXMLNoteData.fTupletMemberType);
//  cout << "fNoteLpsrDuration = " << fNoteLpsrDuration << endl;
    
  // diatonic note
  lpsrNote::MusicXMLDiatonicPitch diatonicNote =
    lpsrNote::k_NoDiatonicPitch;
}

lpsrNote::~lpsrNote() {}

void lpsrNote::setNoteBelongsToAChord () {
  fMusicXMLNoteData.fNoteBelongsToAChord = true;
}

//______________________________________________________________________________

lpsrNote::LpsrPitch lpsrNote::computeNoteLpsrPitch(
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
  lpsrNote::LpsrPitch lpsrPitch = lpsrNote::k_NoLpsrPitch;
  
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
    ", fLpsrPitch = " << fLpsrPitch << endl;
  */
  
  if (fMusicXMLNoteData.fMusicxmlStepIsARest)
    s << "r";

  else {
    //JMI assertLpsr(fLpsrPitch != k_NoLpsrPitch, "fLpsrPitch != k_NoLpsrPitch");
    switch (fNoteLpsrPitch) {
      
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
      
      default: s << "Note" << fNoteLpsrPitch << "???";
    } // switch
  }
  
  string  result;
  
  s >> result;
  return result;
}

void lpsrNote::printMusicXML(ostream& os)
{
  os << "<!-- lpsrNote??? -->" << endl;
}

void lpsrNote::printLPSR(ostream& os)
{
  /*
  cout <<
    "lpsrNote::printLPSR(), fNoteBelongsToAChord = " << 
    fNoteBelongsToAChord << endl;
  */
  
  if (fMusicXMLNoteData.fNoteBelongsToAChord) {
    os << notePitchAsLilypondString () << fNoteLpsrDuration;

  } else {
    os <<
      "Note" << " " << 
      notePitchAsLilypondString () << fNoteLpsrDuration << endl;
    
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
  }
}

void lpsrNote::printLilyPondCode(ostream& os)
{
  // print the note name
  os << notePitchAsLilypondString ();
  
  if (! fMusicXMLNoteData.fNoteBelongsToAChord) {
    // print the note duration
    os << fNoteLpsrDuration;
    
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
S_lpsrSequence lpsrSequence::create(ElementsSeparator elementsSeparator)
{
  lpsrSequence* o = new lpsrSequence(elementsSeparator); assert(o!=0);
  return o;
}

lpsrSequence::lpsrSequence(ElementsSeparator elementsSeparator)
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

void lpsrSequence::printLPSR(ostream& os)
{  
  os << "Sequence" << endl;
  
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

void lpsrParallelMusic::printLPSR(ostream& os)
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

void lpsrChord::addDynamics (S_lpsrDynamics dyn) {
  fChordDynamics.push_back(dyn);
}
void lpsrChord::addWedge (S_lpsrWedge wdg) {
  fChordWedges.push_back(wdg);
}

ostream& operator<< (ostream& os, const S_lpsrChord& chrd)
{
  chrd->print(os);
  return os;
}

void lpsrChord::printMusicXML(ostream& os)
{
  os << "<!-- lpsrChord??? -->" << endl;
}

void lpsrChord::printLPSR(ostream& os)
{
  os << "Chord" << " ";
  vector<S_lpsrNote>::const_iterator
    iBegin = fChordNotes.begin(),
    iEnd   = fChordNotes.end(),
    i      = iBegin;
  os << "<";
  for ( ; ; ) {
    os << (*i);
    if (++i == iEnd) break;
    os << " ";
  } // for
  os << ">";
  
  // print the chord duration
  os << fChordDuration << endl;

  // print the dynamics if any
  if (fChordDynamics.size()) {
    idtr++;
    list<S_lpsrDynamics>::const_iterator i1;
    for (i1=fChordDynamics.begin(); i1!=fChordDynamics.end(); i1++) {
      os << idtr << (*i1);
    } // for
    idtr--;
  }

  // print the wedges if any
  if (fChordWedges.size()) {
    idtr++;
    list<S_lpsrWedge>::const_iterator i2;
    for (i2=fChordWedges.begin(); i2!=fChordWedges.end(); i2++) {
      os << idtr << (*i2);
    } // for
    idtr--;
  }
}

void lpsrChord::printLilyPondCode(ostream& os)
{
  vector<S_lpsrNote>::const_iterator
    iBegin = fChordNotes.begin(),
    iEnd   = fChordNotes.end(),
    i      = iBegin;
  os << "<";
  for ( ; ; ) {
    os << (*i);
    if (++i == iEnd) break;
    os << " ";
  } // for
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
S_lpsrBarLine lpsrBarLine::create(int nextBarNumber)
{
  lpsrBarLine* o = new lpsrBarLine(nextBarNumber); assert(o!=0);
  return o;
}

lpsrBarLine::lpsrBarLine(int nextBarNumber)
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

void lpsrBarLine::printLPSR(ostream& os)
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

void lpsrComment::printLPSR(ostream& os)
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

void lpsrBreak::printLPSR(ostream& os)
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

void lpsrBarNumberCheck::printLPSR(ostream& os)
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

void lpsrTuplet::printLPSR(ostream& os)
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

void lpsrBeam::printLPSR(ostream& os)
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
S_lpsrLyricsChunk lpsrLyricsChunk::create (
  LyricsChunkType chunkType,
  string     chunkText)
{
  lpsrLyricsChunk* o =
    new lpsrLyricsChunk (chunkType, chunkText); assert(o!=0);
  return o;
}

lpsrLyricsChunk::lpsrLyricsChunk (
  LyricsChunkType chunkType,
  string     chunkText)
  : lpsrElement("")
{
  fLyricsChunkType = chunkType;
  fChunkText       = chunkText; 
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

void lpsrLyricsChunk::printLPSR(ostream& os)
{  
  os << "LyricsChunk" << " ";
  switch (fLyricsChunkType) {
    case kSingleChunk: os << "single"; break;
    case kBeginChunk:  os << "begin "; break;
    case kMiddleChunk: os << "middle"; break;
    case kEndChunk:    os << "end   "; break;
    case kSkipChunk:   os << "skip  "; break;
    case kBreakChunk:  os << "break "; break;
  } // switch
  if (fChunkText.size()) os << " " << fChunkText;
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
    case kBreakChunk:  os << endl << idtr;         break;
  } // switch
}

//______________________________________________________________________________
S_lpsrLyrics lpsrLyrics::create (
  string lyricsName,
  string voiceName)
{
  lpsrLyrics* o = new lpsrLyrics (lyricsName, voiceName); assert(o!=0);
  return o;
}

lpsrLyrics::lpsrLyrics (
  string lyricsName,
  string voiceName)
  : lpsrElement("")
{
  fLyricsName = lyricsName;
  fVoiceName  = voiceName; 
}
lpsrLyrics::~lpsrLyrics() {}

ostream& operator<< (ostream& os, const S_lpsrLyrics& stan)
{
  stan->print(os);
  return os;
}

void lpsrLyrics::printMusicXML(ostream& os)
{
  os << "<!-- lpsrLyrics??? -->" << endl;
}

void lpsrLyrics::printLPSR(ostream& os)
{  
  os << "Lyrics" << " " << fLyricsName << endl; //" " << fVoiceName << endl;
  idtr++;
  int n = fLyricsChunks.size();
  for (int i = 0; i < n; i++) {
    os << idtr << fLyricsChunks[i];
  } // for
  idtr--;
}

void lpsrLyrics::printLilyPondCode(ostream& os)
{
  os <<
    fLyricsName << " = " << "\\lyricmode {" << endl;

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
S_lpsrNewlyricsCommand lpsrNewlyricsCommand::create (
  string lyricsName,
  string voiceName)
{
  lpsrNewlyricsCommand* o =
    new lpsrNewlyricsCommand (lyricsName, voiceName);
  assert(o!=0);
  return o;
}

lpsrNewlyricsCommand::lpsrNewlyricsCommand (
  string lyricsName,
  string voiceName)
   : lpsrElement("")
{
  fLyricsName = lyricsName;
  fVoiceName  = voiceName;
}

lpsrNewlyricsCommand::~lpsrNewlyricsCommand() {}

ostream& operator<< (ostream& os, const S_lpsrNewlyricsCommand& nlc)
{
  nlc->print(os);
  return os;
}

void lpsrNewlyricsCommand::printMusicXML (ostream& os)
{
  os << "<!-- lpsrNewlyricsCommand??? -->" << endl;
}

void lpsrNewlyricsCommand::printLPSR (ostream& os)
{
  os <<
    "NewlyricsCommand" << " " <<
    fLyricsName << " " << fVoiceName << endl;
}

void lpsrNewlyricsCommand::printLilyPondCode (ostream& os)
{
   os <<
     idtr << "\\new Lyrics" << " " <<
    "\\lyricsto " << fVoiceName <<
    " " << "\\" << fLyricsName << endl;
}

/*
 *
 *                 \new Lyrics \lyricsto "PartPOneVoiceOne" \PartPOneVoiceOneLyricsOne
*/

//______________________________________________________________________________
S_lpsrVoice lpsrVoice::create(
  string name,
  bool   absoluteCode,
  bool   generateNumericalTime)
{
  lpsrVoice* o =
    new lpsrVoice (name, absoluteCode, generateNumericalTime);
  assert(o!=0);
  return o;
}

lpsrVoice::lpsrVoice(
  string name,
  bool   absoluteCode,
  bool   generateNumericalTime)
    : lpsrElement("")
{
  fVoiceName = name;
  fVoiceAbsoluteCode = absoluteCode;
  fGenerateNumericalTime = generateNumericalTime;
  
  // create the implicit lpsrSequence element
  fVoiceSequence = lpsrSequence::create (lpsrSequence::kSpace);
  
  // add the implicit lpsrRepeat element
// JMI  fVoiceLpsrRepeat = lpsrRepeat::create ();
//  fVoiceSequence->appendElementToSequence (fVoiceLpsrRepeat);
  
  // add the implicit 4/4 time signature
  S_lpsrTime time = lpsrTime::create (4, 4, fGenerateNumericalTime);
  S_lpsrElement t = time;
  fVoiceSequence->appendElementToSequence (t);

}
lpsrVoice::~lpsrVoice() {}

void lpsrVoice::printMusicXML(ostream& os)
{
  os << "<!-- lpsrVoice??? -->" << endl;
}

void lpsrVoice::printLPSR(ostream& os)
{
  os << "Voice" << " " << fVoiceName << endl;

  idtr++;
  os << idtr << fVoiceSequence;
  idtr--;
}

void lpsrVoice::printLilyPondCode(ostream& os)
{
  os << fVoiceName << " = ";
  if (! fVoiceAbsoluteCode) os << "\\relative ";
  os << "{" << endl;

  idtr++;
  os << fVoiceSequence << endl;
  idtr--;

  os << idtr << "}" << endl;
}

//______________________________________________________________________________
S_lpsrPart lpsrPart::create (
  string partID,
  string partName,
  string partInstrumentName,
  bool   absoluteCode,
  bool   generateNumericalTime)
{
  lpsrPart* o =
    new lpsrPart(
      partID,
      partName,
      partName,
      absoluteCode,
      generateNumericalTime);
  assert(o!=0);
  return o;
}

lpsrPart::lpsrPart (
  string partID,
  string partName,
  string partInstrumentName,
  bool   absoluteCode,
  bool   generateNumericalTime)
    : lpsrElement("")
{
  fPartID = partID;
  fPartName = partName;
  fPartInstrumentName = partInstrumentName;
  fPartAbsoluteCode = absoluteCode;
  fGenerateNumericalTime = generateNumericalTime;
}
lpsrPart::~lpsrPart() {}

void lpsrPart::printMusicXML(ostream& os)
{
  os << "<!-- lpsrPart??? -->" << endl;
}

void lpsrPart::printLPSR(ostream& os)
{
  os <<
    "Part" << " " << fPartName << " " <<
    fPartInstrumentName << endl;

  idtr++;
// JMI  os << idtr << fPartLpsrSequence;
  idtr--;
}

void lpsrPart::printLilyPondCode(ostream& os)
{
  os <<
    "Part" << " " << fPartName << " " <<
    fPartInstrumentName << endl;
  if (! fPartAbsoluteCode) os << "\\relative ";
  os << "{" << endl;

  idtr++;
// JMI  os << fPartLpsrSequence << endl;
  idtr--;

  os << idtr << "}" << endl;
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

void lpsrPaper::printLPSR(ostream& os) {
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

void lpsrHeader::printLPSR(ostream& os)
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

void lpsrLilypondVarValAssoc::printLPSR(ostream& os)
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

void lpsrSchemeVarValAssoc::printLPSR(ostream& os)
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

void lpsrLayout::printLPSR(ostream& os)
{
  os << "Layout" << endl;

  idtr++;

  int n1 = fLpsrLilypondVarValAssocs.size();
  for (int i = 0; i < n1; i++ ) {
    os << idtr << fLpsrLilypondVarValAssocs[i];
  } // for
    
  int n2 = fLpsrSchemeVarValAssocs.size();
  for (int i = 0; i < n2; i++ ) {
    os << idtr << fLpsrSchemeVarValAssocs[i];
  } // for
  
  idtr--;
}

void lpsrLayout::printLilyPondCode(ostream& os)
{  
  os << idtr << "\\layout {" << endl;

  idtr++;

  int n1 = fLpsrLilypondVarValAssocs.size();
  for (int i = 0; i < n1; i++ ) {
    os << idtr << fLpsrLilypondVarValAssocs[i];
  } // for
    
  int n2 = fLpsrSchemeVarValAssocs.size();
  for (int i = 0; i < n2; i++ ) {
    os << idtr << fLpsrSchemeVarValAssocs[i];
  } // for
  
  idtr--;

  os << idtr << "}" << endl;
}

//______________________________________________________________________________
S_lpsrTime lpsrTime::create(int numerator, int denominator, bool generateNumericalTime)
{
  lpsrTime* o = new lpsrTime(numerator, denominator, generateNumericalTime); assert(o!=0);
  return o;
}

lpsrTime::lpsrTime(int numerator, int denominator, bool generateNumericalTime)
  : lpsrElement("")
{
  fRational = rational(numerator, denominator);
// JMI  fNumerator=numerator; 
  //fDenominator=denominator;
  fGenerateNumericalTime = generateNumericalTime;
}
lpsrTime::~lpsrTime() {}

void lpsrTime::printMusicXML(ostream& os)
{
  os << "<!-- lpsrTime??? -->" << endl;
}

void lpsrTime::printLPSR(ostream& os)
{
  os <<
    "Time " << 
    fRational.getNumerator() << "/" << fRational.getDenominator() <<endl;
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
S_lpsrClef lpsrClef::create (
  string sign,
  int         line,
  int         octaveChange)
{
  lpsrClef* o =
    new lpsrClef(sign, line, octaveChange); assert(o!=0);
  return o;
}

lpsrClef::lpsrClef(
  string sign,
  int         line,
  int         octaveChange)
  : lpsrElement("")
{
  fSign         = sign;
  fLine         = line;
  fOctaveChange = octaveChange;
}
lpsrClef::~lpsrClef() {}

void lpsrClef::printMusicXML(ostream& os)
{
  os << "<!-- lpsrClef??? -->" << endl;
}

void lpsrClef::printLPSR(ostream& os)
{
  os <<
    "clef" << " \"" << fSign << "\"" <<
    " " << fLine << " " << fOctaveChange << endl;
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
S_lpsrKey lpsrKey::create(
  int fifths, string mode, int cancel)
{
  lpsrKey* o = new lpsrKey(fifths, mode, cancel);
  assert(o!=0);
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

void lpsrKey::printLPSR(ostream& os)
{
  os << "Key " << fTonic << " ";
  if (fKeyMode == kMajor) os << "\\major";
  else os << "\\minor";
  os << endl;
}

void lpsrKey::printLilyPondCode(ostream& os)
{
  os << "\\key " << fTonic << " ";
  if (fKeyMode == kMajor) os << "\\major";
  else os << "\\minor";
  os << endl;
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

void lpsrMidi::printLPSR(ostream& os)
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
S_lpsrScore lpsrScore::create()
{
  lpsrScore* o = new lpsrScore(); assert(o!=0);
  return o;
}

lpsrScore::lpsrScore()
  : lpsrElement("")
{
  // create the parallel music element
  fScoreParallelMusic = lpsrParallelMusic::create(lpsrParallelMusic::kEndOfLine);
  
  // create the layout element
  fScoreLayout = lpsrLayout::create();
  
  // create the midi element
  fScoreMidi = lpsrMidi::create();
}
lpsrScore::~lpsrScore() {}

ostream& operator<< (ostream& os, const S_lpsrScore& scr)
{
  scr->print(os);
  return os;
}

void lpsrScore::printMusicXML(ostream& os)
{
  os << "<!-- lpsrScore??? -->" << endl;
}

void lpsrScore::printLPSR(ostream& os)
{
  os << "Score" << endl;

  idtr++;

  os << idtr << fScoreParallelMusic;
  os << idtr << fScoreLayout;
  os << idtr << fScoreMidi;

  idtr--;
}

void lpsrScore::printLilyPondCode(ostream& os) {  
  os << "\\score {" << endl;

  idtr++;

  os << fScoreParallelMusic << endl;
  os << fScoreLayout << endl;
  os << fScoreMidi;

  idtr--;

  os << "}" << endl;
}

//______________________________________________________________________________
S_lpsrNewstaffCommand lpsrNewstaffCommand::create()
{
  lpsrNewstaffCommand* o = new lpsrNewstaffCommand(); assert(o!=0);
  return o;
}

lpsrNewstaffCommand::lpsrNewstaffCommand(): lpsrElement("") {}
lpsrNewstaffCommand::~lpsrNewstaffCommand() {}

ostream& operator<< (ostream& os, const S_lpsrNewstaffCommand& nstf)
{
  nstf->print(os);
  return os;
}

void lpsrNewstaffCommand::printMusicXML(ostream& os)
{
  os << "<!-- lpsrNewstaffCommand??? -->" << endl;
}

void lpsrNewstaffCommand::printLPSR(ostream& os)
{
  os << "NewstaffCommand" << endl;

  idtr++;
  
  int size = fNewStaffElements.size();

  for (int i = 0; i < size; i++ ) {
    os << idtr << fNewStaffElements[i];
  } // for
  
  idtr--;
}

void lpsrNewstaffCommand::printLilyPondCode(ostream& os)
{      
  os << "\\new Staff <<" << endl;
  
  idtr++;
  
  int size = fNewStaffElements.size();

  for (int i = 0; i < size; i++ ) {
    os << idtr << fNewStaffElements[i];
  } // for
  
  idtr--;
  
  os << idtr << ">>" << endl;  
}

//______________________________________________________________________________
S_lpsrVariableUseCommand lpsrVariableUseCommand::create(string variableName)
{
  lpsrVariableUseCommand* o =
    new lpsrVariableUseCommand(variableName);
  assert(o!=0);
  return o;
}

lpsrVariableUseCommand::lpsrVariableUseCommand(string variableName)
  : lpsrElement("")
{
  fVariableName = variableName;
}

lpsrVariableUseCommand::~lpsrVariableUseCommand() {}

ostream& operator<< (ostream& os, const S_lpsrVariableUseCommand& nstf)
{
  nstf->print(os);
  return os;
}

void lpsrVariableUseCommand::printMusicXML(ostream& os)
{
  os << "<!-- lpsrVariableUseCommand??? -->" << endl;
}

void lpsrVariableUseCommand::printLPSR(ostream& os)
{
  os << "VariableUseCommand" << endl;
  idtr++;
  os << idtr << fVariableName << endl;
  idtr--;
}

void lpsrVariableUseCommand::printLilyPondCode(ostream& os)
{
  os << "\\" << fVariableName << endl;
}

//______________________________________________________________________________
S_lpsrTempoCommand lpsrTempoCommand::create (
  int tempoUnit, int perMinute)
{
  lpsrTempoCommand* o =
    new lpsrTempoCommand(tempoUnit, perMinute); assert(o!=0);
  return o;
}

lpsrTempoCommand::lpsrTempoCommand(
    int tempoUnit, int perMinute)
  : lpsrElement("")
{
  fTempoUnit = tempoUnit;
  fPerMinute = perMinute;
}
lpsrTempoCommand::~lpsrTempoCommand() {}

ostream& operator<< (ostream& os, const S_lpsrTempoCommand& nstf)
{
  nstf->print(os);
  return os;
}

void lpsrTempoCommand::printMusicXML(ostream& os)
{
  os << "<!-- lpsrTempoCommand??? -->" << endl;
}

void lpsrTempoCommand::printLPSR(ostream& os)
{
  os <<
    "TempoCommand" << " " <<
    fTempoUnit << " " << fPerMinute << endl;
}

void lpsrTempoCommand::printLilyPondCode(ostream& os)
{
  os <<
    "\\tempo" << " " <<
    fTempoUnit << " = " << fPerMinute << endl;
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

void lpsrRepeat::printLPSR(ostream& os)
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
S_lpsrContext lpsrContext::create (
  ContextKind    contextKind,
  string    contextType,
  string    contextName)
{
  lpsrContext* o =
    new lpsrContext (contextKind, contextType, contextName);
  assert(o!=0);
  return o;
}

lpsrContext::lpsrContext (
  ContextKind    contextKind,
  string    contextType,
  string    contextName)
    : lpsrElement("")
{
  fContextKind = contextKind;
  fContextType = contextType;
  fContextName = contextName; 
}
lpsrContext::~lpsrContext() {}

void lpsrContext::printMusicXML(ostream& os)
{
  os << "<!-- context??? -->" << endl;
}

void lpsrContext::printLPSR(ostream& os)
{  
  os << "Context" << " ";
  switch (fContextKind) {
    case kExistingContext:
      os << "existing";
      break;
    case kNewContext:
      os << "new";
      break;
  } // switch
  os << " " << fContextType;
  if (fContextName.size())
    os << " " << fContextName;
  os << endl;
  
  idtr++;

  int size = fContextElements.size();

  for (int i = 0; i < size; i++ ) {
    os << idtr << fContextElements[i];
  } // for

  idtr--;
}

void lpsrContext::printLilyPondCode(ostream& os)
{  
  switch (fContextKind) {
    case kExistingContext:
      os << "\\context";
      break;
    case kNewContext:
      os << "\\new";
      break;
  } // switch
  os << " " << fContextType;
  if (fContextName.size())
    os << " = \"" << fContextName << "\"";
  os << " {" << endl;
  
  idtr++;
  
  int size = fContextElements.size();

  for (int i = 0; i < size; i++ ) {
    os << idtr << fContextElements[i];
  } // for
  
  idtr--;
  
  os << idtr << "}" << endl;
}


}
