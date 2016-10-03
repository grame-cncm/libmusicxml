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
#include <iomanip>      // std::setw, set::precision, ...

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
    case lpsrGlobalVariables::kLpsrStructure:
      this->printLpsrStructure (os);
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
  os << "<!-- lpsrElement??? -->" << std::endl;
}

void lpsrElement::printLpsrStructure(ostream& os)
{
  os << "Element???" << std::endl;
}

void lpsrElement::printLilyPondCode(ostream& os)
{
  os << "\%{ lpsrElement??? \%}" << std::endl;
}

//______________________________________________________________________________
ostream& operator<< (ostream& os, musicXMLNoteData& data)
{
  data.print(os);
  return os;
}

void musicXMLNoteData::print (std::ostream& os)
{
  os <<
    "  " << std::left << setw(26) << "fMusicxmlStep = " << fMusicxmlStep <<  std::endl <<
    "  " << std::left << setw(26) << "fMusicxmlStepIsARest = " << fMusicxmlStepIsARest <<  std::endl <<
    "  " << std::left << setw(26) << "fMusicxmlAlteration = " << fMusicxmlAlteration <<  std::endl <<
    "  " << std::left << setw(26) << "fMusicxmlOctave = " << fMusicxmlOctave <<  std::endl <<
    "  " << std::left << setw(26) << "fMusicxmlDivisions = " << fMusicxmlDivisions <<  std::endl <<
    "  " << std::left << setw(26) << "fMusicxmlDuration = " << fMusicxmlDuration <<  std::endl <<
    "  " << std::left << setw(26) << "fDotsNumber = " << fDotsNumber <<  std::endl <<
    "  " << std::endl <<    
    "  " << std::left << setw(26) << "fNoteBelongsToAChord = " << fNoteBelongsToAChord <<  std::endl <<
    "  " << std::endl <<
    "  " << std::left << setw(26) << "fNoteBelongsToATuplet = " << fNoteBelongsToATuplet <<  std::endl <<
    "  " << std::left << setw(26) << "fTupletMemberType = " << fTupletMemberType <<  std::endl <<
    "  " << std::endl <<
    "  " << std::left << setw(26) << "fVoiceNumber = " << fVoiceNumber <<  std::endl;
};

void musicXMLBeatData::print (std::ostream& os)
{
  os <<
    "  " << std::left << setw(26) << "fBeatUnit = " << fBeatUnit <<  std::endl <<
    "  " << std::left << setw(26) << "fDots = " << fDots <<  std::endl;
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
    ", fLpsrOctave = " << fLpsrOctave << std::endl;
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
  os << "<!-- lpsrAbsoluteOctave??? -->" << std::endl;
}

std::string lpsrAbsoluteOctave::absoluteOctaveAsLilypondString ()
{
  stringstream s;
  std::string  result;
  
  s << fLpsrOctave;
  s >> result;
  return result;
}

void lpsrAbsoluteOctave::printLpsrStructure(ostream& os)
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
  std::string tupletMemberType)
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
    std::string tupletMemberType)
  : lpsrElement("")
{
  fNum   = num;
  fDenom = denom;
  fDots  = dots;
  fTupletMemberType = tupletMemberType;
  /*
  cout <<
    "lpsrDuration::lpsrDuration (), fNum = " << fNum << 
    ", fDenom = " << fDenom << ", fDots = " << fDots << std::endl;
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
  os << "<!-- lpsrDuration??? -->" << std::endl;
}

std::string lpsrDuration::durationAsLilypondString ()
{
  // divisions are per quater, Lpsr durations are in whole notes
//  cout << "|"  << fNum << "|" << fDenom << "|" << fDots << "|" << std::endl;

  int noteDivisions         = fNum;
  int divisionsPerWholeNote = fDenom ;
  
  if (divisionsPerWholeNote == 0) {
    stringstream s;
    std::string  message;
    s << 
      std::endl << 
      "%--> lpsrDuration::printLilyPondCode, noteDivisions = " << noteDivisions <<
      ", divisionsPerWholeNote = " << divisionsPerWholeNote <<
      std::endl;
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
      std::string  message;
      s << 
        std::endl << 
        "--> unknown tuplet member type " << fTupletMemberType <<
        std::endl;
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
        std::string  message;
        s <<
          "*** ERROR, MusicXML note duration " << noteDivisions << "/" << 
          divisionsPerWholeNote << " is too large" << std::endl;
        s >> message;
        lpsrMusicXMLError(message);
        }
    } // switch
  }
  
  //cout << "--> fDots = " << fDots << std::endl;
  
  // print the dots if any 
  int n = fDots; 
  if (n > 0) {
    while (n-- > 0) {
      s << ".";  
    } // while
  }
  
  std::string  result;
  
  s >> result;
  return result;
}

void lpsrDuration::printLpsrStructure(ostream& os)
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

std::string lpsrDynamics::dynamicsKindAsLilypondString ()
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
      std::string  message;
      s << "Dynamics " << fDynamicsKind << " is unknown";
      s >> message;
      lpsrMusicXMLError(message);
      }
  } // switch
  
  std::string result;
  
  s >> result;
  return result;
}

void lpsrDynamics::printMusicXML(ostream& os)
{
  os << "<!-- lpsrDynamics??? -->" << std::endl;
}

void lpsrDynamics::printLpsrStructure(ostream& os)
{
  os <<
    "Dynamics" << " " << dynamicsKindAsLilypondString () << std::endl;
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

std::string lpsrWedge::wedgeKinsAsString ()
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
  
  std::string result;
  
  s >> result;
  return result;
}

void lpsrWedge::printMusicXML(ostream& os)
{
  os << "<!-- lpsrWedge??? -->" << std::endl;
}

void lpsrWedge::printLpsrStructure(ostream& os)
{
  os << "Wedge" << " " << wedgeKinsAsString () << std::endl;
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
    cout << "==> fMusicXMLNoteData:" << std::endl;
    cout << fMusicXMLNoteData << std::endl;
  }
    
  // take rests into account
  if (fMusicXMLNoteData.fMusicxmlStep)
    fMusicXMLDiatonicPitch = lpsrNote::kRest;

  if (fMusicXMLNoteData.fMusicxmlStep <'A' || fMusicXMLNoteData.fMusicxmlStep > 'G') {
    stringstream s;
    std::string  message;
    s << "step value " << fMusicXMLNoteData.fMusicxmlStep << " is not a letter from A to G";
    s >> message;
    lpsrMusicXMLError (message);
  }

//  cout << "=== xmlPart2LpsrVisitor::visitStart ( S_step& elt ) " << fCurrentMusicXMLStep << std::endl;
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
    fMusicXMLNoteData.fMusicxmlAlteration <<  std::endl;
*/
  
  stringstream s;
  std::string  message;
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
    std::cerr << 
      "--> fMusicXMLNoteData.fMusicxmlDivisions = " <<
      fMusicXMLNoteData.fMusicxmlDivisions << ", " << 
      "divisionsPerWholeNote = " << divisionsPerWholeNote << std::endl;
    
  lpsrAssert(
    divisionsPerWholeNote > 0,
    "The MusicMXL divisions per quater note value should be positive");

  fNoteLpsrDuration =
    lpsrDuration::create (
      fMusicXMLNoteData.fMusicxmlDuration,
      divisionsPerWholeNote,
      fMusicXMLNoteData.fDotsNumber,
      fMusicXMLNoteData.fTupletMemberType);
//  cout << "fNoteLpsrDuration = " << fNoteLpsrDuration << std::endl;
    
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

std::string lpsrNote::notePitchAsLilypondString ()
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
  
  std::string  result;
  
  s >> result;
  return result;
}

void lpsrNote::printMusicXML(ostream& os)
{
  os << "<!-- lpsrNote??? -->" << std::endl;
}

void lpsrNote::printLpsrStructure(ostream& os)
{
  /*
  cout <<
    "lpsrNote::printLpsrStructure(), fNoteBelongsToAChord = " << 
    fNoteBelongsToAChord << endl;
  */
  
  if (fMusicXMLNoteData.fNoteBelongsToAChord) {
    os << notePitchAsLilypondString () << fNoteLpsrDuration;

  } else {
    os <<
      "Note" << " " << 
      notePitchAsLilypondString () << fNoteLpsrDuration << std::endl;
    
    // print the dynamics if any
    if (fNoteDynamics.size()) {
      idtr++;
      std::list<S_lpsrDynamics>::const_iterator i1;
      for (i1=fNoteDynamics.begin(); i1!=fNoteDynamics.end(); i1++) {
        os << idtr << (*i1);
      } // for
      idtr--;
    }
  
    // print the wedges if any
    if (fNoteWedges.size()) {
      idtr++;
      std::list<S_lpsrWedge>::const_iterator i2;
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
      std::list<S_lpsrDynamics>::const_iterator
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
      std::list<S_lpsrWedge>::const_iterator
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

std::string lpsrNote::octaveRepresentation (char octave)
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

void lpsrSequence::printLpsrStructure(ostream& os)
{  
  os << "Sequence" << std::endl;
  
  idtr++;

  std::list<S_lpsrElement>::const_iterator
    iBegin = fSequenceElements.begin(),
    iEnd   = fSequenceElements.end(),
    i      = iBegin;
  for ( ; ; ) {
    os << idtr << (*i);
    if (++i == iEnd) break;
    if (fElementsSeparator == kEndOfLine) os << std::endl;
  } // for
  
  idtr--;
}

void lpsrSequence::printMusicXML(ostream& os)
{
  os << "<!-- lpsrSequence??? -->" << std::endl;
}

void lpsrSequence::printLilyPondCode(ostream& os)
{
  std::list<S_lpsrElement>::const_iterator
    iBegin = fSequenceElements.begin(),
    iEnd   = fSequenceElements.end(),
    i      = iBegin;
  for ( ; ; ) {
    os << idtr << (*i);
    if (++i == iEnd) break;
    if (fElementsSeparator == kEndOfLine) os << std::endl;
  } // for
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
  os << "<!-- lpsrParallelMusic??? -->" << std::endl;
}

void lpsrParallelMusic::printLpsrStructure(ostream& os)
{
  os << "ParallelMusic" << std::endl;
  
  idtr++;
  
  int size = fParallelMusicElements.size();
  
  for (int i = 0; i < size; i++ ) {
    os << idtr << fParallelMusicElements[i];
  } // for
  
  idtr--;
}

void lpsrParallelMusic::printLilyPondCode(ostream& os)
{      
  os << idtr << "<<" << std::endl;
  
  idtr++;
  
  int size = fParallelMusicElements.size();
  
  for (int i = 0; i < size; i++ ) {
    os << idtr << fParallelMusicElements[i];
  } // for
  
  idtr--;
  
  os << idtr << ">>" << std::endl;
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
  os << "<!-- lpsrChord??? -->" << std::endl;
}

void lpsrChord::printLpsrStructure(ostream& os)
{
  os << "Chord" << " ";
  std::vector<S_lpsrNote>::const_iterator
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
  os << fChordDuration << std::endl;

  // print the dynamics if any
  if (fChordDynamics.size()) {
    idtr++;
    std::list<S_lpsrDynamics>::const_iterator i1;
    for (i1=fChordDynamics.begin(); i1!=fChordDynamics.end(); i1++) {
      os << idtr << (*i1);
    } // for
    idtr--;
  }

  // print the wedges if any
  if (fChordWedges.size()) {
    idtr++;
    std::list<S_lpsrWedge>::const_iterator i2;
    for (i2=fChordWedges.begin(); i2!=fChordWedges.end(); i2++) {
      os << idtr << (*i2);
    } // for
    idtr--;
  }
}

void lpsrChord::printLilyPondCode(ostream& os)
{
  std::vector<S_lpsrNote>::const_iterator
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
    std::list<S_lpsrDynamics>::const_iterator i1;
    for (i1=fChordDynamics.begin(); i1!=fChordDynamics.end(); i1++) {
      os << " " << (*i1);
    } // for
  }

  // print the wedges if any
  if (fChordWedges.size()) {
    std::list<S_lpsrWedge>::const_iterator i2;
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
  os << "<!-- lpsrBarLine??? -->" << std::endl;
}

void lpsrBarLine::printLpsrStructure(ostream& os)
{
  os << "BarLine" << " " << fNextBarNumber << std::endl;
}

void lpsrBarLine::printLilyPondCode(ostream& os)
{
  os << "| % " << fNextBarNumber << std::endl;
}

//______________________________________________________________________________
S_lpsrComment lpsrComment::create(std::string contents, GapKind gapKind)
{
  lpsrComment* o = new lpsrComment(contents, gapKind); assert(o!=0);
  return o;
}

lpsrComment::lpsrComment(std::string contents, GapKind gapKind)
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
  os << "<!-- lpsrComment??? -->" << std::endl;
}

void lpsrComment::printLpsrStructure(ostream& os)
{
  os << "Comment" << std::endl;
  idtr++;
  os << idtr << "% " << fContents << std::endl;
  if (fGapKind == kGapAfterwards) os << idtr << std::endl;
  idtr--;
}

void lpsrComment::printLilyPondCode(ostream& os)
{
  os << "% " << fContents;
  if (fGapKind == kGapAfterwards) os << std::endl;
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
  os << "<!-- lpsrBreak??? -->" << std::endl;
}

void lpsrBreak::printLpsrStructure(ostream& os)
{
  os <<
    "Break" << " " << fNextBarNumber << std::endl <<
    std::endl;
}

void lpsrBreak::printLilyPondCode(ostream& os)
{
  os <<
    "\\myBreak | % " << fNextBarNumber << std::endl <<
    std::endl;
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
  os << "<!-- lpsrBarNumberCheck??? -->" << std::endl;
}

void lpsrBarNumberCheck::printLpsrStructure(ostream& os)
{
  os << "BarNumberCheck" << " " << fNextBarNumber << std::endl;
}

void lpsrBarNumberCheck::printLilyPondCode(ostream& os)
{
  os << "\\barNumberCheck #" << fNextBarNumber << std::endl;
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
  os << "<!-- lpsrTuplet??? -->" << std::endl;
}

void lpsrTuplet::printLpsrStructure(ostream& os)
{
  os <<
    "Tuplet " << fActualNotes << "/" << fNormalNotes << std::endl;
  idtr++;
  std::vector<S_lpsrElement>::const_iterator i;
  for (i=fTupletContents.begin(); i!=fTupletContents.end(); i++) {
    os << idtr << (*i);
  } // for
  idtr--;
}

void lpsrTuplet::printLilyPondCode(ostream& os)
{
  os << "\\tuplet " << fActualNotes << "/" << fNormalNotes << " { ";

  idtr++;
  
  std::vector<S_lpsrElement>::const_iterator
    iBegin = fTupletContents.begin(),
    iEnd   = fTupletContents.end(),
    i      = iBegin;
    for ( ; ; ) {
      os << (*i);
      if (++i == iEnd) break;
      os << " ";
    } // for
  
  idtr--;
  
  os << " }" << std::endl;
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
  os << "<!-- lpsrBeam??? -->" << std::endl;
}

void lpsrBeam::printLpsrStructure(ostream& os)
{
  os << "Beam???" << std::endl;
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
/*
std::map<std::string, xmlPartSummaryVisitor::stanzaContents> & xmlPartSummaryVisitor::getStanzas() { 
  return fStanzas;
}

void xmlPartSummaryVisitor::clearStanzas () {
  fStanzas.clear();
}
*/

/*
std::string xmlPartSummaryVisitor::stanzaAsString (std::string separator) const {
//  if (fTranslationSettings->fTrace) cerr << "Extracting part \"" << partid << "\" lyrics information" << endl;
//  std::map<std::string, std::list<std::list<std::string> > > stanzas = ps.getStanzas();


  std::map<std::string, stanzaContents> ::const_iterator
    it1 = fStanzas.find(name);
  
  if (it1 != fStanzas.end()) {
    stringstream s;
    string       lyricsName =
                  "_Lyrics" + int2EnglishWord (atoi (it1->first.c_str()));

    s << lyricsName << " = \\lyricmode { " << std::endl;

    for (stanzaContents::const_iterator 
        it2=it1->second.begin(); it2!=it1->second.end(); ++it2) { 
      std::list<std::string> ::const_iterator 
        it2Begin = it2->begin(),
        it2End   = it2->end(),
        it3      = it2Begin;

      for ( ; ; ) {
        s << *it3;
        if (++it3 == it2End) break;
        s << separator;
      } // for
      cout << " ";
    } // for
      
    s << std::endl << "}" << std::endl << std::endl;
    } else {
    result = "Can't find stanza \""+name+"\"";
  } // if
}
  */

/*
class lpsrStanzaChunk : public lpsrElement {
  public:

    // we want to end the line in the LilyPond code at a break
    typedef enum { kWordChunk, kSkipChunk, kBreakChunk } stanzaChunkType;

    static SMARTP<lpsrStanzaChunk> create (stanzaChunkType type);
     
  protected:

   lpsrStanza (stanzaChunkType type);
    virtual ~lpsrStanza();

  private:
  
    stanzaChunkType fStanzaChunkType;
    std::string     fChunkText;
}
*/


//______________________________________________________________________________
S_lpsrStanzaChunk lpsrStanzaChunk::create (
  stanzaChunkType chunkType,
  std::string     chunkText)
{
  lpsrStanzaChunk* o =
    new lpsrStanzaChunk (chunkType, chunkText); assert(o!=0);
  return o;
}

lpsrStanzaChunk::lpsrStanzaChunk (
  stanzaChunkType chunkType,
  std::string     chunkText)
  : lpsrElement("")
{
  fStanzaChunkType = chunkType;
  fChunkText       = chunkText; 
}
lpsrStanzaChunk::~lpsrStanzaChunk() {}

ostream& operator<< (ostream& os, const S_lpsrStanzaChunk& lyr)
{
  lyr->print(os);
  return os;
}

void lpsrStanzaChunk::printMusicXML(ostream& os)
{
  os << "<!-- lpsrStanzaChunk??? -->" << std::endl;
}

void lpsrStanzaChunk::printLpsrStructure(ostream& os)
{  
  os << "StanzaChunk" << " ";
  switch (fStanzaChunkType) {
    case kWordChunk:  os << "Word"; break;
    case kSkipChunk:  os << "Skip"; break;
    case kBreakChunk: os << "Break"; break;
  } // switch
  if (fChunkText.size()) os << " " << fChunkText;
  os << std::endl;
}

void lpsrStanzaChunk::printLilyPondCode(ostream& os)
{  
  switch (fStanzaChunkType) {
    case kWordChunk:  os << fChunkText; break;
    case kSkipChunk:  os << "\\skip"; break;
    case kBreakChunk: os << "\\myBreak"; break;
  } // switch
}

//______________________________________________________________________________
S_lpsrStanza lpsrStanza::create (
  std::string lyricsName,
  std::string voiceName)
{
  lpsrStanza* o = new lpsrStanza (lyricsName, voiceName); assert(o!=0);
  return o;
}

lpsrStanza::lpsrStanza (
  std::string lyricsName,
  std::string voiceName)
  : lpsrElement("")
{
  fLyricsName = lyricsName;
  fVoiceName  = voiceName; 
}
lpsrStanza::~lpsrStanza() {}

ostream& operator<< (ostream& os, const S_lpsrStanza& lyr)
{
  lyr->print(os);
  return os;
}

void lpsrStanza::printMusicXML(ostream& os)
{
  os << "<!-- lpsrStanza??? -->" << std::endl;
}

void lpsrStanza::printLpsrStructure(ostream& os)
{  
  os << "Stanza" << " " << fLyricsName << "fVoiceName" << std::endl;
  idtr++;
  int n = fStanzaChunks.size();
  for (int i = 0; i < n; i++) {
    os << idtr << fStanzaChunks[i] << std::endl;
  } // for
  idtr--;
}

void lpsrStanza::printLilyPondCode(ostream& os)
{  
  std::vector<S_lpsrStanzaChunk>::const_iterator
    iBegin = fStanzaChunks.begin(),
    iEnd   = fStanzaChunks.end(),
    i      = iBegin;
    
  for ( ; ; ) {
    os << (*i);
    if (++i == iEnd) break;
    os << "--";
  } // for
}

//______________________________________________________________________________
S_lpsrLyrics lpsrLyrics::create (
  std::string lyricsName,
  std::string voiceName)
{
  lpsrLyrics* o =
    new lpsrLyrics (lyricsName, voiceName);
  assert(o!=0);
  return o;
}

lpsrLyrics::lpsrLyrics(
  std::string lyricsName,
  std::string voiceName)
  : lpsrElement("")
{
  fLyricsName = lyricsName;
  fVoiceName  = voiceName;
}
lpsrLyrics::~lpsrLyrics() {}

ostream& operator<< (ostream& os, const S_lpsrLyrics& lyr)
{
  lyr->print(os);
  return os;
}

void lpsrLyrics::printMusicXML(ostream& os)
{
  os << "<!-- lpsrLyrics??? -->" << std::endl;
}

void lpsrLyrics::printLpsrStructure(ostream& os)
{  
  os <<
    "Lyrics" << " " << fLyricsName << " " << fVoiceName << std::endl;
  idtr++;
  int n = fLyricsStanzas.size();
  for (int i = 0; i < n; i++) {
    os << idtr << fLyricsStanzas[i] << std::endl;
  } // for
  idtr--;
}

void lpsrLyrics::printLilyPondCode(ostream& os)
{  
  os <<
    idtr << "\\new Lyrics" << " " <<
    "\\lyricsto " << fVoiceName <<
    " = \\lyricmode {" << fLyricsName << std::endl;

  std::vector<S_lpsrStanza>::const_iterator
    iBegin = fLyricsStanzas.begin(),
    iEnd   = fLyricsStanzas.end(),
    i      = iBegin;
    
  for ( ; ; ) {
    os << (*i);
    if (++i == iEnd) break;
    os << "--";
  } // for
}

/*
void lpsrLyrics::printLilyPondCode(ostream& os)
{      
  os << "\\new Staff <<" << std::endl;
  
  idtr++;
  
  int size = fNewStaffElements.size();

  for (int i = 0; i < size; i++ ) {
    os << idtr << fNewStaffElements[i];
  } // for
  
  idtr--;
  
  os << idtr << ">>" << std::endl;  
}
*/

//______________________________________________________________________________
S_lpsrVoiceBOF lpsrVoiceBOF::create (std::string voiceName)
{
  lpsrVoiceBOF* o = new lpsrVoiceBOF (voiceName); assert(o!=0);
  return o;
}

lpsrVoiceBOF::lpsrVoiceBOF (std::string voiceName)
   : lpsrElement("")
{
  fVoiceName = voiceName;
}

lpsrVoiceBOF::~lpsrVoiceBOF() {}

ostream& operator<< (ostream& os, const S_lpsrVoiceBOF& vce)
{
  vce->print(os);
  return os;
}

void lpsrVoiceBOF::printMusicXML(ostream& os)
{
  os << "<!-- lpsrVoiceBOF??? -->" << std::endl;
}

void lpsrVoiceBOF::printLpsrStructure(ostream& os)
{
  os << "Voice" << " " << fVoiceName << std::endl;
  
  idtr++;
  
  int size = fVoiceLyrics.size ();
  for (int i = 0; i < size; i++) {
    os << idtr << fVoiceLyrics[i];
  } // for
  
  idtr--;  
}

void lpsrVoiceBOF::printLilyPondCode(ostream& os)
{  
  os <<
    "\\new Voice" << " = " <<
    " \"" << fVoiceName << "\"" << " {" << std::endl;

  idtr++;
  
  os << idtr << "\\" << fVoiceName << std::endl;
  
  idtr--;
  
  os << idtr << "}" << std::endl;
  //                  \new Voice = "PartPOneVoiceOne" { \voiceOne \PartPOneVoiceOne }

}

//______________________________________________________________________________
S_lpsrVoice lpsrVoice::create(
  std::string name,
  bool absoluteCode,
  bool generateNumericalTime)
{
  lpsrVoice* o =
    new lpsrVoice (name, absoluteCode, generateNumericalTime);
  assert(o!=0);
  return o;
}

lpsrVoice::lpsrVoice(
  std::string name,
  bool absoluteCode,
  bool generateNumericalTime)
    : lpsrElement("")
{
  fVoiceName = name;
  fVoiceAbsoluteCode = absoluteCode;
  fGenerateNumericalTime = generateNumericalTime;
  
  // create the implicit lpsrSequence element
  fVoiceLpsrSequence = lpsrSequence::create (lpsrSequence::kSpace);
  
  // add the implicit lpsrRepeat element
// JMI  fVoiceLpsrRepeat = lpsrRepeat::create ();
//  fVoiceLpsrSequence->appendElementToSequence (fVoiceLpsrRepeat);
  
  // add the implicit 4/4 time signature
  S_lpsrTime time = lpsrTime::create (4, 4, fGenerateNumericalTime);
  S_lpsrElement t = time;
  fVoiceLpsrSequence->appendElementToSequence (t);

}
lpsrVoice::~lpsrVoice() {}

void lpsrVoice::printMusicXML(ostream& os)
{
  os << "<!-- lpsrVoice??? -->" << std::endl;
}

void lpsrVoice::printLpsrStructure(ostream& os)
{
  os << "Voice" << " " << fVoiceName << std::endl;

  idtr++;
  os << idtr << fVoiceLpsrSequence;
  idtr--;
}

void lpsrVoice::printLilyPondCode(ostream& os)
{
  os << fVoiceName << " = ";
  if (! fVoiceAbsoluteCode) os << "\\relative ";
  os << "{" << std::endl;

  idtr++;
  os << fVoiceLpsrSequence << std::endl;
  idtr--;

  os << idtr << "}" << std::endl;
}

//______________________________________________________________________________
S_lpsrPart lpsrPart::create (
  std::string name,
  std::string partInstrumentName,
  bool        absoluteCode,
  bool        generateNumericalTime)
{
  lpsrPart* o =
    new lpsrPart(
      name,
      partInstrumentName,
      absoluteCode,
      generateNumericalTime);
  assert(o!=0);
  return o;
}

lpsrPart::lpsrPart (
  std::string name,
  std::string partInstrumentName,
  bool        absoluteCode,
  bool        generateNumericalTime)
    : lpsrElement("")
{
  fPartName = name;
  fPartInstrumentName = partInstrumentName;
  fPartAbsoluteCode = absoluteCode;
  fGenerateNumericalTime = generateNumericalTime;
}
lpsrPart::~lpsrPart() {}

void lpsrPart::printMusicXML(ostream& os)
{
  os << "<!-- lpsrPart??? -->" << std::endl;
}

void lpsrPart::printLpsrStructure(ostream& os)
{
  os <<
    "Part" << " " << fPartName << " " <<
    fPartInstrumentName << std::endl;

  idtr++;
// JMI  os << idtr << fPartLpsrSequence;
  idtr--;
}

void lpsrPart::printLilyPondCode(ostream& os)
{
  os <<
    "Part" << " " << fPartName << " " <<
    fPartInstrumentName << std::endl;
  if (! fPartAbsoluteCode) os << "\\relative ";
  os << "{" << std::endl;

  idtr++;
// JMI  os << fPartLpsrSequence << std::endl;
  idtr--;

  os << idtr << "}" << std::endl;
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
  os << "<!-- lpsrPaper??? -->" << std::endl;
}

void lpsrPaper::printLpsrStructure(ostream& os) {
  os << "Paper" << std::endl;

  idtr++;
  
  if (fPaperWidth > 0) {
    os << 
      idtr << "paper-width = " << std::setprecision(4) << fPaperWidth << "\\cm" << std::endl;
  }
  if (fPaperHeight > 0) {
    os <<
      idtr << "paper-height = " << std::setprecision(4) << fPaperHeight << "\\cm" << std::endl;
  }
  if (fTopMargin > 0) {
    os <<
      idtr << "top-margin = " << std::setprecision(4) << fTopMargin << "\\cm" << std::endl;
  }
  if (fBottomMargin > 0) {
    os <<
      idtr << "bottom-margin = " << std::setprecision(4) << fBottomMargin << "\\cm" << std::endl;
  }
  if (fLeftMargin > 0) {
    os <<
      idtr << "left-margin = " << std::setprecision(4) << fLeftMargin << "\\cm" << std::endl;
  }

  if (fRightMargin > 0) {
    os << idtr << "right-margin = " << std::setprecision(4) << fRightMargin << "\\cm" << std::endl;
  }

/*
  if (fBetweenSystemSpace > 0) {
    os << idtr << "between-system-space = " << std::setprecision(4) << fBetweenSystemSpace << "\\cm" << std::endl;
  }

  if (fPageTopSpace > 0) {
    os << idtr << "page-top-space = " << std::setprecision(4) << fPageTopSpace << "\\cm" << std::endl;
  }
*/

  idtr--;
}

void lpsrPaper::printLilyPondCode(ostream& os)
{  
  os << "\\paper {" << std::endl;

  idtr++;
  
  if (fPaperWidth > 0) {
    os << 
      idtr << "paper-width = " << std::setprecision(4) << fPaperWidth << "\\cm" << std::endl;
  }
  if (fPaperHeight > 0) {
    os <<
      idtr << "paper-height = " << std::setprecision(4) << fPaperHeight << "\\cm" << std::endl;
  }
  if (fTopMargin > 0) {
    os <<
      idtr << "top-margin = " << std::setprecision(4) << fTopMargin << "\\cm" << std::endl;
  }
  if (fBottomMargin > 0) {
    os <<
      idtr << "bottom-margin = " << std::setprecision(4) << fBottomMargin << "\\cm" << std::endl;
  }
  if (fLeftMargin > 0) {
    os <<
      idtr << "left-margin = " << std::setprecision(4) << fLeftMargin << "\\cm" << std::endl;
  }

  if (fRightMargin > 0) {
    os << idtr << "right-margin = " << std::setprecision(4) << fRightMargin << "\\cm" << std::endl;
  }

/*
  if (fBetweenSystemSpace > 0) {
    os << idtr << "between-system-space = " << std::setprecision(4) << fBetweenSystemSpace << "\\cm" << std::endl;
  }

  if (fPageTopSpace > 0) {
    os << idtr << "page-top-space = " << std::setprecision(4) << fPageTopSpace << "\\cm" << std::endl;
  }
*/

  idtr--;
  
  os << "}" << std::endl;
}

//______________________________________________________________________________
S_lpsrHeader lpsrHeader::create() {
  lpsrHeader* o = new lpsrHeader(); assert(o!=0);
  return o;
}

lpsrHeader::lpsrHeader() : lpsrElement("") {}
lpsrHeader::~lpsrHeader() {}

void lpsrHeader::setWorkNumber (std::string val)
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

void lpsrHeader::setWorkTitle (std::string val)
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

void lpsrHeader::setMovementNumber (std::string val)
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

void lpsrHeader::setMovementTitle (std::string val)
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

void lpsrHeader::addCreator (std::string type, std::string val)
{
  fCreators.push_back(
    lpsrLilypondVarValAssoc::create (
      type, val,
      lpsrLilypondVarValAssoc::kEqualSign,
      lpsrLilypondVarValAssoc::kQuotesAroundValue,
      lpsrLilypondVarValAssoc::kUncommented)
  );
}
std::vector<S_lpsrLilypondVarValAssoc> lpsrHeader::getCreators () const
  { return fCreators; };

void lpsrHeader::setRights (std::string val)
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

void lpsrHeader::addSoftware (std::string val)
{
  fSoftwares.push_back(
    lpsrLilypondVarValAssoc::create (
      "software", val,
      lpsrLilypondVarValAssoc::kEqualSign,
      lpsrLilypondVarValAssoc::kQuotesAroundValue,
      lpsrLilypondVarValAssoc::kUncommented)
  );
}
std::vector<S_lpsrLilypondVarValAssoc> lpsrHeader::getSoftwares () const
  { return fSoftwares; };

void lpsrHeader::setEncodingDate (std::string val)
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

void lpsrHeader::setScoreInstrument (std::string val)
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
  os << "<!-- lpsrHeader??? -->" << std::endl;
}

void lpsrHeader::printLpsrStructure(ostream& os)
{
  os << "Header" << std::endl;

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
    std::vector<S_lpsrLilypondVarValAssoc>::const_iterator i1;
    for (i1=fCreators.begin(); i1!=fCreators.end(); i1++) {
      os << idtr << (*i1);
    } // for
  }
    
  if (fRights) {
    os << idtr << fRights;
  }
    
  if (!fSoftwares.empty()) {
    std::vector<S_lpsrLilypondVarValAssoc>::const_iterator i2;
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
  os << "\\header {" << std::endl;
  
  idtr++;
    
  if (fWorkNumber) {
    std::string source = fWorkNumber->getVariableValue();
    std::string dest;
    std::for_each( source.begin(), source.end(), stringQuoteEscaper(dest));
    os << idtr << 
      "%" << fWorkNumber->getVariableName() << " = \""  << 
      dest << "\"" << std::endl;
  }
  
  if (fWorkTitle) {
    std::string source = fWorkTitle->getVariableValue();
    std::string dest;
    std::for_each( source.begin(), source.end(), stringQuoteEscaper(dest));
    os << idtr << 
      "%" << fWorkTitle->getVariableName() << " = \""  << 
      dest << "\"" << std::endl;
    os << idtr << 
      "title" << " = \""  << 
      dest << "\"" << std::endl;
  }
    
  if (fMovementNumber) {
    std::string source = fMovementNumber->getVariableValue();
    std::string dest;
    std::for_each( source.begin(), source.end(), stringQuoteEscaper(dest));
    os << idtr << 
      "%" << fMovementNumber->getVariableName() << " = \""  << 
      dest << "\"" << std::endl;
  }
    
  if (fMovementTitle) {
    std::string source = fMovementTitle->getVariableValue();
    std::string dest;
    std::for_each( source.begin(), source.end(), stringQuoteEscaper(dest));
    os << idtr << 
      "%" << fMovementTitle->getVariableName() << " = \""  << 
      dest << "\"" << std::endl;
    os << idtr << 
      "subtitle" << " = \""  << 
      dest << "\"" << std::endl;
  }
    
  if (!fCreators.empty()) {
    vector<S_lpsrLilypondVarValAssoc>::const_iterator i1;
    for (i1=fCreators.begin(); i1!=fCreators.end(); i1++) {
      os << idtr << (*i1);
      if ((*i1)->getVariableName() == "composer")
      os << idtr << 
        "composer" << " = \""  << 
        (*i1)->getVariableValue() << "\"" << std::endl;
    } // for
  }
    
  if (fRights) {
    std::string source = fRights->getVariableValue();
    std::string dest;
    std::for_each( source.begin(), source.end(), stringQuoteEscaper(dest));
    os << idtr << 
      "%" << fRights->getVariableName() << " = \""  << 
      dest << "\"" << std::endl;
  }
    
  if (!fSoftwares.empty()) {
    vector<S_lpsrLilypondVarValAssoc>::const_iterator i2;
    for (i2=fSoftwares.begin(); i2!=fSoftwares.end(); i2++) {
      os << idtr << (*i2);
     } // for
  }
    
  if (fEncodingDate) {
    std::string source = fEncodingDate->getVariableValue();
    std::string dest;
    std::for_each( source.begin(), source.end(), stringQuoteEscaper(dest));
    os << idtr << 
      "%" << fEncodingDate->getVariableName() << " = \""  << 
      dest << "\"" << std::endl;
  }
  
  if (fScoreInstrument) {
    std::string source = fScoreInstrument->getVariableValue();
    std::string dest;
    std::for_each( source.begin(), source.end(), stringQuoteEscaper(dest));
    os << idtr << 
      "%" << fScoreInstrument->getVariableName() << " = \""  << 
      dest << "\"" << std::endl;
    os << idtr << 
      "instrument" << " = \""  << 
      dest << "\"" << std::endl;
  }
  
  os << "}" << std::endl;
  
  idtr--;
}

//______________________________________________________________________________
S_lpsrLilypondVarValAssoc lpsrLilypondVarValAssoc::create(
      std::string     variableName,
      std::string     value, 
      VarValSeparator varValSeparator,
      QuotesKind      quotesKind,
      CommentedKind   commentedKind,
      std::string     unit)
{
  lpsrLilypondVarValAssoc* o =
    new lpsrLilypondVarValAssoc(
    variableName, value, varValSeparator, 
    quotesKind, commentedKind, unit);
  assert(o!=0);
  return o;
}

lpsrLilypondVarValAssoc::lpsrLilypondVarValAssoc(
      std::string     variableName,
      std::string     value, 
      VarValSeparator varValSeparator,
      QuotesKind      quotesKind,
      CommentedKind   commentedKind,
      std::string     unit)
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

void lpsrLilypondVarValAssoc::changeAssoc (std::string value) {
  fVariableValue=value;
}

ostream& operator<< (ostream& os, const S_lpsrLilypondVarValAssoc& assoc) {
  assoc->print(os);
  return os;
}

void lpsrLilypondVarValAssoc::printMusicXML(ostream& os)
{
  os << "<!-- lpsrLilypondVarValAssoc??? -->" << std::endl;
}

void lpsrLilypondVarValAssoc::printLpsrStructure(ostream& os)
{
  os << "LilypondVarValAssoc" << std::endl;
  idtr++;
  os << idtr << fVariableName << std::endl;
  os << idtr << fVariableValue <<std::endl;
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
  os << std::endl;
}

//______________________________________________________________________________
S_lpsrSchemeVarValAssoc lpsrSchemeVarValAssoc::create(
      std::string     variableName,
      std::string     value, 
      CommentedKind   commentedKind )
{
  lpsrSchemeVarValAssoc* o = new lpsrSchemeVarValAssoc(
    variableName, value, commentedKind);
  assert(o!=0);
  return o;
}

lpsrSchemeVarValAssoc::lpsrSchemeVarValAssoc(
      std::string     variableName,
      std::string     value, 
      CommentedKind   commentedKind )
  : lpsrElement("")
{
  fVariableName=variableName;
  fVariableValue=value;
  fCommentedKind=commentedKind;
}

lpsrSchemeVarValAssoc::~lpsrSchemeVarValAssoc() {}

void lpsrSchemeVarValAssoc::changeAssoc (std::string value)
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
  os << "<!-- lpsrSchemeVarValAssoc??? -->" << std::endl;
}

void lpsrSchemeVarValAssoc::printLpsrStructure(ostream& os)
{
  os << "SchemeVarValAssoc" << std::endl;
  idtr++;
  os << idtr << fVariableName << std::endl;
  os << idtr << fVariableValue <<std::endl;
  idtr--;
}

void lpsrSchemeVarValAssoc::printLilyPondCode(ostream& os)
{
  if (fCommentedKind == kCommented) os << "\%";
  os <<
    "#(" << 
    fVariableName << " " << fVariableValue << 
    ")" << std::endl;
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
  os << "<!-- lpsrLayout??? -->" << std::endl;
}

void lpsrLayout::printLpsrStructure(ostream& os)
{
  os << "Layout" << std::endl;

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
  os << idtr << "\\layout {" << std::endl;

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

  os << idtr << "}" << std::endl;
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
  os << "<!-- lpsrTime??? -->" << std::endl;
}

void lpsrTime::printLpsrStructure(ostream& os)
{
  os <<
    "Time " << 
    fRational.getNumerator() << "/" << fRational.getDenominator() <<std::endl;
}

void lpsrTime::printLilyPondCode(ostream& os)
{
//  os << fName << "\\time \"" << fNumerator << "/" << fDenominator << "\"" << std::endl;
  if (fGenerateNumericalTime)
    os << "\\numericTimeSignature ";
  os <<
    "\\time " <<
    fRational.getNumerator() << "/" << fRational.getDenominator() <<
    std::endl;
}

//______________________________________________________________________________
S_lpsrClef lpsrClef::create (
  std::string sign,
  int         line,
  int         octaveChange)
{
  lpsrClef* o =
    new lpsrClef(sign, line, octaveChange); assert(o!=0);
  return o;
}

lpsrClef::lpsrClef(
  std::string sign,
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
  os << "<!-- lpsrClef??? -->" << std::endl;
}

void lpsrClef::printLpsrStructure(ostream& os)
{
  os <<
    "clef" << " \"" << fSign << "\"" <<
    " " << fLine << " " << fOctaveChange << std::endl;
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

  os << "\\clef" << " \"" << result << "\"" << std::endl;
}

//______________________________________________________________________________
S_lpsrKey lpsrKey::create(std::string tonic, KeyMode keyMode)
{
  lpsrKey* o = new lpsrKey(tonic, keyMode); assert(o!=0);
  return o;
}

lpsrKey::lpsrKey(std::string tonic, KeyMode keyMode)
  : lpsrElement("")
{
  fTonic=tonic;
  fKeyMode=keyMode; 
}
lpsrKey::~lpsrKey() {}

ostream& operator<< (ostream& os, const S_lpsrKey& key)
{
  key->print(os);
  return os;
}

void lpsrKey::printMusicXML(ostream& os)
{
  os << "<!-- lpsrKey??? -->" << std::endl;
}

void lpsrKey::printLpsrStructure(ostream& os)
{
  os << "Key " << fTonic << " ";
  if (fKeyMode == kMajor) os << "\\major";
  else os << "\\minor";
  os << std::endl;
}

void lpsrKey::printLilyPondCode(ostream& os)
{
  os << "\\key " << fTonic << " ";
  if (fKeyMode == kMajor) os << "\\major";
  else os << "\\minor";
  os << std::endl;
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
  os << "<!-- lpsrMidi??? -->" << std::endl;
}

void lpsrMidi::printLpsrStructure(ostream& os)
{
  os << "Midi" << std::endl;

  idtr++;
  
  os << idtr << "% to be completed" << std::endl;
  
  idtr--;
}

void lpsrMidi::printLilyPondCode(ostream& os)
{  
  os << idtr << "\\midi {" << std::endl;
  
  idtr++;
  
  os << idtr << "% to be completed" << std::endl;
  
  idtr--;
  
  os << idtr << "}" << std::endl;
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
  os << "<!-- lpsrScore??? -->" << std::endl;
}

void lpsrScore::printLpsrStructure(ostream& os)
{
  os << "Score" << std::endl;

  idtr++;

  os << idtr << fScoreParallelMusic;
  os << idtr << fScoreLayout;
  os << idtr << fScoreMidi;

  idtr--;
}

void lpsrScore::printLilyPondCode(ostream& os) {  
  os << "\\score {" << std::endl;

  idtr++;

  os << fScoreParallelMusic << std::endl;
  os << fScoreLayout << std::endl;
  os << fScoreMidi;

  idtr--;

  os << "}" << std::endl;
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
  os << "<!-- lpsrNewstaffCommand??? -->" << std::endl;
}

void lpsrNewstaffCommand::printLpsrStructure(ostream& os)
{
  os << "NewstaffCommand" << std::endl;

  idtr++;
  
  int size = fNewStaffElements.size();

  for (int i = 0; i < size; i++ ) {
    os << idtr << fNewStaffElements[i];
  } // for
  
  idtr--;
}

void lpsrNewstaffCommand::printLilyPondCode(ostream& os)
{      
  os << "\\new Staff <<" << std::endl;
  
  idtr++;
  
  int size = fNewStaffElements.size();

  for (int i = 0; i < size; i++ ) {
    os << idtr << fNewStaffElements[i];
  } // for
  
  idtr--;
  
  os << idtr << ">>" << std::endl;  
}

//______________________________________________________________________________
S_lpsrNewlyricsCommand lpsrNewlyricsCommand::create (
        std::string lyricsName,
        std::string partName)
{
  lpsrNewlyricsCommand* o =
    new lpsrNewlyricsCommand (lyricsName, partName);
  assert(o!=0);
  return o;
}

lpsrNewlyricsCommand::lpsrNewlyricsCommand (
  std::string lyricsName,
  std::string partName)
   : lpsrElement("")
{
  fLyricsName = lyricsName;
  fPartName   = partName;
}

lpsrNewlyricsCommand::~lpsrNewlyricsCommand() {}

ostream& operator<< (ostream& os, const S_lpsrNewlyricsCommand& nlc)
{
  nlc->print(os);
  return os;
}

void lpsrNewlyricsCommand::printMusicXML (ostream& os)
{
  os << "<!-- lpsrNewlyricsCommand??? -->" << std::endl;
}

void lpsrNewlyricsCommand::printLpsrStructure (ostream& os)
{
  os <<
    "NewlyricsCommand" << " " <<
    fLyricsName << " " << fPartName << std::endl;
}

void lpsrNewlyricsCommand::printLilyPondCode (ostream& os)
{
  os <<
    "NewlyricsCommand" << " " <<
    fLyricsName << " " << fPartName << std::endl;
}

//______________________________________________________________________________
S_lpsrVariableUseCommand lpsrVariableUseCommand::create(std::string variableName)
{
  lpsrVariableUseCommand* o =
    new lpsrVariableUseCommand(variableName);
  assert(o!=0);
  return o;
}

lpsrVariableUseCommand::lpsrVariableUseCommand(std::string variableName)
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
  os << "<!-- lpsrVariableUseCommand??? -->" << std::endl;
}

void lpsrVariableUseCommand::printLpsrStructure(ostream& os)
{
  os << "VariableUseCommand" << std::endl;
  idtr++;
  os << idtr << fVariableName << std::endl;
  idtr--;
}

void lpsrVariableUseCommand::printLilyPondCode(ostream& os)
{
  os << "\\" << fVariableName << std::endl;
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
  os << "<!-- lpsrTempoCommand??? -->" << std::endl;
}

void lpsrTempoCommand::printLpsrStructure(ostream& os)
{
  os <<
    "TempoCommand" << " " <<
    fTempoUnit << " " << fPerMinute << std::endl;
}

void lpsrTempoCommand::printLilyPondCode(ostream& os)
{
  os <<
    "\\tempo" << " " <<
    fTempoUnit << " = " << fPerMinute << std::endl;
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
  os << "<!-- lpsrRepeat??? -->" << std::endl;
}

void lpsrRepeat::printLpsrStructure(ostream& os)
{
  os << "Repeat" << std::endl;
  idtr++;
    os << idtr << fCommonPart;
    std::vector<S_lpsrSequence>::const_iterator i;
    for (i=fAlternateEndings.begin(); i!=fAlternateEndings.end(); i++) {
      os << idtr << (*i);
    } // for
  idtr--;
}

void lpsrRepeat::printLilyPondCode(ostream& os)
{
  os << "Repeat" << std::endl;
}

//______________________________________________________________________________
S_lpsrContext lpsrContext::create (
  ContextKind    contextKind,
  std::string    contextType,
  std::string    contextName)
{
  lpsrContext* o =
    new lpsrContext (contextKind, contextType, contextName);
  assert(o!=0);
  return o;
}

lpsrContext::lpsrContext (
  ContextKind    contextKind,
  std::string    contextType,
  std::string    contextName)
    : lpsrElement("")
{
  fContextKind = contextKind;
  fContextType = contextType;
  fContextName = contextName; 
}
lpsrContext::~lpsrContext() {}

void lpsrContext::printMusicXML(ostream& os)
{
  os << "<!-- context??? -->" << std::endl;
}

void lpsrContext::printLpsrStructure(ostream& os)
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
  os << std::endl;
  
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
  os << " {" << std::endl;
  
  idtr++;
  
  int size = fContextElements.size();

  for (int i = 0; i < size; i++ ) {
    os << idtr << fContextElements[i];
  } // for
  
  idtr--;
  
  os << idtr << "}" << std::endl;
}


}
