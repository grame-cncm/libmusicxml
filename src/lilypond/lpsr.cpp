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

lpsrNote::stringToLpsrNoteNamesLanguage const
  lpsrNote::sStringToLpsrNoteNamesLanguage;

std::map<lpsrNote::LpsrPitch, std::string> lpsrNote::sDutchLilypondPitches;

lpsrGlobalVariables::CodeGenerationKind
  lpsrGlobalVariables::sCodeGenerationKind =
    lpsrGlobalVariables::kLilypondCode;

indenter lpsrElement::idtr;

//______________________________________________________________________________
SlpsrElement lpsrElement::create(bool debug)
{
  lpsrElement * o = new lpsrElement(debug); assert(o!=0);
  return o; 
}

lpsrElement::lpsrElement(bool debug)
{
  fDebug = debug;
}
lpsrElement::~lpsrElement() {}

ostream& operator<< (ostream& os, const SlpsrElement& elt)
{
  elt->print(os);
  return os;
}

void lpsrElement::print(ostream& os)
{
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
SlpsrAbsoluteOctave lpsrAbsoluteOctave::create(int musicxmlOctave)
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
lpsrAbsoluteOctave::~lpsrAbsoluteOctave() {}

ostream& operator<< (ostream& os, const SlpsrAbsoluteOctave& dur)
{
  dur->print(os);
  return os;
}

void lpsrAbsoluteOctave::printMusicXML(ostream& os)
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
SlpsrDuration lpsrDuration::create(int num, int denom, int dots)
{
  lpsrDuration * o = new lpsrDuration (num, denom, dots);
  assert(o!=0); 
  return o;
}

lpsrDuration::lpsrDuration (int num, int denom, int dots)
  : lpsrElement("")
{
  fNum   = num;
  fDenom = denom;
  fDots  = dots; 
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

void lpsrDuration::sett (int num, int denom, int dots)
{
  fNum=num; fDenom=denom; fDots=dots; 
}

ostream& operator<< (ostream& os, const SlpsrDuration& dur)
{
  dur->print(os);
  return os;
}

void lpsrDuration::printMusicXML(ostream& os)
{
  os << "<!-- lpsrDuration??? -->" << std::endl;
}

void lpsrDuration::printLpsrStructure(ostream& os)
{
  this->printLilyPondCode (os);
}

std::string lpsrDuration::durationAsLilypondString ()
{
  // divisions are per quater, Lpsr durations are in whole notes
  //os << "|"  << fLpsrDuration.fNum << "|" << fLpsrDuration.fDenom;

  stringstream s;
  
  int noteDivisions         = fNum;
  int divisionsPerWholeNote = fDenom ;
  
  if (divisionsPerWholeNote == 0) {
    cerr << 
      std::endl << 
      "%--> lpsrDuration::printLilyPondCode, noteDivisions = " << noteDivisions <<
      ", divisionsPerWholeNote = " << divisionsPerWholeNote << std::endl;
  }
  
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
      cerr <<
        "*** ERROR, MusicXML note duration " << noteDivisions << "/" << 
        divisionsPerWholeNote << " is too large" << std::endl;
  } // switch
  
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
SlpsrDynamics lpsrDynamics::create(DynamicsKind dynamicsKind)
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

ostream& operator<< (ostream& os, const SlpsrDynamics& dyn)
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
      s << "\\ff";
      break;
    case kFFF:
      s << "\\fff";
      break;
    case kFFFF:
      s << "\\ffff";
      break;
    case kFFFFF:
      s << "\\fffff";
      break;
    case kFFFFFF:
      s << "\\ffffff";
      break;

    case kP:
      s << "\\p";
      break;
    case kPP:
      s << "\\pp";
      break;
    case kPPP:
      s << "\\ppp";
      break;
    case kPPPP:
      s << "\\pppp";
      break;
    case kPPPPP:
      s << "\\ppppp";
      break;
    case kPPPPPP:
      s << "\\pppppp";
      break;

     case kMF:
      s << "\\mf";
      break;
    case kMP:
      s << "\\mp";
      break;
    case kFP:
      s << "\\fp";
      break;
    case kFZ:
      s << "\\fz";
      break;
    case kRF:
      s << "\\rf";
      break;
    case kSF:
      s << "\\sf";
      break;

   case kRFZ:
      s << "\\rfz";
      break;
    case kSFZ:
      s << "\\sfz";
      break;
    case kSFP:
      s << "\\sfp";
      break;
    case kSFPP:
      s << "\\sfpp";
      break;
    case kSFFZ:
      s << "\\sffz";
      break;

    default:
      s << "Dynamics " << fDynamicsKind << "???";
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
SlpsrWedge lpsrWedge::create(WedgeKind wedgeKind)
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

ostream& operator<< (ostream& os, const SlpsrWedge& wdg)
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
SlpsrNote lpsrNote::createFromMusicXMLData (musicXMLNoteData& mxmldat)
{  
  lpsrNote * o = new lpsrNote (mxmldat); assert(o!=0); 
  return o;
}

lpsrNote::lpsrNote (musicXMLNoteData& mxmldat)
  : lpsrElement("")
{
  /*
  fDiatonicPitch        = lpsrNote::k_NoDiatonicPitch;
  fAlteration           = lpsrNote::k_NoAlteration;
  fOctave               = -1;
  // leave fLpsrDuration as it is, will be set on S_note
  fVoiceNumber          = -1;
*/

  sQuatertonesFromA['A']=0;
  sQuatertonesFromA['B']=4;
  sQuatertonesFromA['C']=6;
  sQuatertonesFromA['D']=10;
  sQuatertonesFromA['E']=14;
  sQuatertonesFromA['F']=16;
  sQuatertonesFromA['G']=20;
  
  fMusicXMLNoteData = mxmldat;
 
  // take rests into account
  if (fMusicXMLNoteData.fMusicxmlStep)
    fMusicXMLDiatonicPitch = lpsrNote::kRest;
 
  // how many quater tones from A?s
  int noteQuatertonesFromA =
    sQuatertonesFromA[fMusicXMLNoteData.fMusicxmlStep];
  
  // flat or sharp,possibly double?
  lpsrNote::MusicXMLAlteration mxmlAlteration;
  
  int alter = fMusicXMLNoteData.fMusicXMLAlteration;
  
  stringstream s;
  std::string  message;
  s << "MusicXML alteration " << alter << " is not between -2 and +2";
  s >> message;
  assertLpsr(alter>=-2 && alter<=+2, message);
  
  switch (alter) {
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

  fLpsrPitch = 
    computeNoteLpsrPitch (noteQuatertonesFromA, mxmlAlteration);
  
  int divisions             = fMusicXMLNoteData.fMusicxmlDivisions;
  int divisionsPerWholeNote = divisionsPerWholeNote*4;
  
  if (fTranslationSwitches.fDebug)
    std::cerr << 
    "divisions = " << divisions << ", " << 
    "divisionsPerWholeNote = " << divisionsPerWholeNote << std::endl;
    
  int durat = fMusicXMLNoteData.fMusicxmlDuration;

  if (divisionsPerWholeNote == 0) {
    std::cerr << 
      std::endl << 
      "%--> durat = " << durat <<
      ", durat = " << durat << std::endl;
    assertLpsr(false, "There cannot be 0 divisions per MusicXML note");
  }

  SlpsrDuration noteDuration =
    lpsrDuration::create (
      durat,
      divisionsPerWholeNote,
      fMusicXMLNoteData.fCurrentDotsNumber);
  //cout << "durat = " << durat << std::endl;
    
  // diatonic note
  lpsrNote::DiatonicPitch diatonicNote = lpsrNote::k_NoDiatonicPitch;

}

lpsrNote::~lpsrNote() {}

void lpsrNote::updateNote(
  bool              currentStepIsRest,
  DiatonicPitch     diatonicNote,
  Alteration        alteration,
  int               octave,
  SlpsrDuration     dur,
  LpsrPitch         lpsrPitch,
  int               voice,
  bool              noteBelongsToAChord)
{
  fCurrentStepIsRest = currentStepIsRest;
  fDiatonicPitch = diatonicNote;
  fAlteration = alteration;
  fOctave = octave;
  fLpsrDuration = dur;
  fLpsrPitch = lpsrPitch;
  fVoiceNumber = voice;
  fNoteBelongsToAChord = noteBelongsToAChord;
}

void lpsrNote::updateNoteDuration(int actualNotes, int normalNotes)
{
  fLpsrDuration->scaleNumByFraction(actualNotes, normalNotes);
}

void lpsrNote::setNoteBelongsToAChord () {
  fNoteBelongsToAChord = true;
}

void lpsrNote::addDynamics (SlpsrDynamics dyn) {
  fNoteDynamics.push_back(dyn);
}
void lpsrNote::addWedge (SlpsrWedge wdg) {
  fNoteWedges.push_back(wdg);
}

SlpsrDynamics lpsrNote::removeFirstDynamics () {
  SlpsrDynamics dyn = fNoteDynamics.front();
  fNoteDynamics.pop_front();
  return dyn;
}
SlpsrWedge lpsrNote::removeFirstWedge () {
  SlpsrWedge wdg = fNoteWedges.front();
  fNoteWedges.pop_front();
  return wdg;
}

ostream& operator<< (ostream& os, const SlpsrNote& elt)
{
  elt->print(os);
  return os;
}

std::string lpsrNote::notePitchAsLilypondString ()
{
  stringstream s;
  
  if (fCurrentStepIsRest)
    s << "r";
  else {
    //JMI assertLpsr(fLpsrPitch != k_NoLpsrPitch, "fLpsrPitch != k_NoLpsrPitch");
    switch (fLpsrPitch) {
      
      case k_aeseh:
        s << "aeseh";
        break;
      case k_aes:
        s << "aes";
        break;
      case k_aeh:
        s << "aeh";
        break;
      case k_a:
        s << "a";
        break;
      case k_aih:
        s << "aih";
        break;
      case k_ais:
        s << "ais";
        break;
      case k_aisih:
        s << "aisih";
        break;
        
      case k_beseh:
        s << "beseh";
        break;
      case k_bes:
        s << "bes";
        break;
      case k_beh:
        s << "beh";
        break;
      case k_b:
        s << "b";
        break;
      case k_bih:
        s << "bih";
        break;
      case k_bis:
        s << "bis";
        break;
      case k_bisih:
        s << "bisih";
        break;
        
      case k_ceseh:
        s << "ceseh";
        break;
      case k_ces:
        s << "ces";
        break;
      case k_ceh:
        s << "ceh";
        break;
      case k_c:
        s << "c";
        break;
      case k_cih:
        s << "cih";
        break;
      case k_cis:
        s << "cis";
        break;
      case k_cisih:
        s << "cisih";
        break;
        
      case k_deseh:
        s << "deseh";
        break;
      case k_des:
        s << "des";
        break;
      case k_deh:
        s << "deh";
        break;
      case k_d:
        s << "d";
        break;
      case k_dih:
        s << "dih";
        break;
      case k_dis:
        s << "dis";
        break;
      case k_disih:
        s << "disih";
        break;
  
      case k_eeseh:
        s << "eeseh";
        break;
      case k_ees:
        s << "ees";
        break;
      case k_eeh:
        s << "eeh";
        break;
      case k_e:
        s << "e";
        break;
      case k_eih:
        s << "eih";
        break;
      case k_eis:
        s << "eis";
        break;
      case k_eisih:
        s << "eisih";
        break;
        
      case k_feseh:
        s << "feseh";
        break;
      case k_fes:
        s << "fes";
        break;
      case k_feh:
        s << "feh";
        break;
      case k_f:
        s << "f";
        break;
      case k_fih:
        s << "fih";
        break;
      case k_fis:
        s << "fis";
        break;
      case k_fisih:
        s << "fisih";
        break;
        
      case k_geseh:
        s << "geseh";
        break;
      case k_ges:
        s << "ges";
        break;
      case k_geh:
        s << "geh";
        break;
      case k_g:
        s << "g";
        break;
      case k_gih:
        s << "gih";
        break;
      case k_gis:
        s << "gis";
        break;
      case k_gisih:
        s << "gisih";
        break;
      default:
        s << "Note" << fLpsrPitch << "???";
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
  if (fNoteBelongsToAChord) {
    os << notePitchAsLilypondString () << fLpsrDuration;

  } else {
    os <<
      "Note" << " " << 
      notePitchAsLilypondString () << fLpsrDuration << std::endl;
    
    // print the dynamics if any
    if (fNoteDynamics.size()) {
      idtr++;
      std::list<SlpsrDynamics>::const_iterator i1;
      for (i1=fNoteDynamics.begin(); i1!=fNoteDynamics.end(); i1++) {
        os << idtr << (*i1);
      } // for
      idtr--;
    }
  
    // print the wedges if any
    if (fNoteWedges.size()) {
      idtr++;
      std::list<SlpsrWedge>::const_iterator i2;
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
  
  if (! fNoteBelongsToAChord) {
    // print the note duration
    os << fLpsrDuration;
    
    // print the dynamics if any
    std::list<SlpsrDynamics>::const_iterator i1;
    for (i1=fNoteDynamics.begin(); i1!=fNoteDynamics.end(); i1++) {
      os << " " << (*i1);
    } // for
  
    // print the wedges if any
    std::list<SlpsrWedge>::const_iterator i2;
    for (i2=fNoteWedges.begin(); i2!=fNoteWedges.end(); i2++) {
      os << " " << (*i2);
    } // for
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
SlpsrSequence lpsrSequence::create(ElementsSeparator elementsSeparator)
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

ostream& operator<< (ostream& os, const SlpsrSequence& elt)
{
  elt->print(os);
  return os;
}

void lpsrSequence::printLpsrStructure(ostream& os)
{  
  os << "Sequence" << std::endl;
  
  idtr++;

  std::list<SlpsrElement>::const_iterator
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
  std::list<SlpsrElement>::const_iterator
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
SlpsrParallelMusic lpsrParallelMusic::create(ElementsSeparator elementsSeparator)
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

ostream& operator<< (ostream& os, const SlpsrParallelMusic& elt)
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
SlpsrChord lpsrChord::create(SlpsrDuration chordduration)
{
  lpsrChord* o = new lpsrChord(chordduration); assert(o!=0);
  return o;
}

lpsrChord::lpsrChord (SlpsrDuration chordduration)
  : lpsrElement("")
{
  fChordDuration = chordduration;
}
lpsrChord::~lpsrChord() {}

void lpsrChord::addDynamics (SlpsrDynamics dyn) {
  fChordDynamics.push_back(dyn);
}
void lpsrChord::addWedge (SlpsrWedge wdg) {
  fChordWedges.push_back(wdg);
}

ostream& operator<< (ostream& os, const SlpsrChord& chrd)
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
  std::vector<SlpsrNote>::const_iterator
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
    std::list<SlpsrDynamics>::const_iterator i1;
    for (i1=fChordDynamics.begin(); i1!=fChordDynamics.end(); i1++) {
      os << idtr << (*i1);
    } // for
    idtr--;
  }

  // print the wedges if any
  if (fChordWedges.size()) {
    idtr++;
    std::list<SlpsrWedge>::const_iterator i2;
    for (i2=fChordWedges.begin(); i2!=fChordWedges.end(); i2++) {
      os << idtr << (*i2);
    } // for
    idtr--;
  }
}

void lpsrChord::printLilyPondCode(ostream& os)
{
  std::vector<SlpsrNote>::const_iterator
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
    std::list<SlpsrDynamics>::const_iterator i1;
    for (i1=fChordDynamics.begin(); i1!=fChordDynamics.end(); i1++) {
      os << " " << (*i1);
    } // for
  }

  // print the wedges if any
  if (fChordWedges.size()) {
    std::list<SlpsrWedge>::const_iterator i2;
    for (i2=fChordWedges.begin(); i2!=fChordWedges.end(); i2++) {
      os << " " << (*i2);
    } // for
  }
}

//______________________________________________________________________________
SlpsrBarLine lpsrBarLine::create(int nextBarNumber)
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

ostream& operator<< (ostream& os, const SlpsrBarLine& elt)
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
SlpsrComment lpsrComment::create(std::string contents, GapKind gapKind)
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

ostream& operator<< (ostream& os, const SlpsrComment& elt)
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
SlpsrBreak lpsrBreak::create(int nextBarNumber)
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

ostream& operator<< (ostream& os, const SlpsrBreak& elt)
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
SlpsrBarNumberCheck lpsrBarNumberCheck::create(int nextBarNumber)
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

ostream& operator<< (ostream& os, const SlpsrBarNumberCheck& elt)
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
SlpsrTuplet lpsrTuplet::create()
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

ostream& operator<< (ostream& os, const SlpsrTuplet& elt)
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
  std::vector<SlpsrElement>::const_iterator i;
  for (i=fTupletContents.begin(); i!=fTupletContents.end(); i++) {
    os << idtr << (*i);
  } // for
  idtr--;
}

void lpsrTuplet::printLilyPondCode(ostream& os)
{
  os << "\\tuplet " << fActualNotes << "/" << fNormalNotes << " { ";

  idtr++;
  
  std::vector<SlpsrElement>::const_iterator
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
SlpsrBeam lpsrBeam::create(int number, BeamKind beamKind)
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

ostream& operator<< (ostream& os, const SlpsrBeam& dyn)
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
SlpsrLyrics lpsrLyrics::create(std::string name, std::string contents)
{
  lpsrLyrics* o = new lpsrLyrics(name, contents); assert(o!=0);
  return o;
}

lpsrLyrics::lpsrLyrics(std::string name, std::string contents)
  : lpsrElement("")
{
  fLyricsName = name;
  fLyricsContents=contents; 
}
lpsrLyrics::~lpsrLyrics() {}

void lpsrLyrics::printMusicXML(ostream& os)
{
  os << "<!-- lpsrLyrics??? -->" << std::endl;
}

void lpsrLyrics::printLpsrStructure(ostream& os)
{  
  os << "Lyrics" << std::endl;
  idtr++;
  os << idtr << fLyricsName << std::endl;
  os << idtr << fLyricsContents << std::endl;
  idtr--;
}

void lpsrLyrics::printLilyPondCode(ostream& os)
{  
  os << idtr << fLyricsName << " = \\lyricmode {" << std::endl;
  idtr++;
  os << idtr << fLyricsContents << std::endl;
  idtr--;
  os << idtr << "}" << std::endl;
}

//______________________________________________________________________________
SlpsrPart lpsrPart::create(
  std::string name, bool absoluteCode, bool generateNumericalTime)
{
  lpsrPart* o = new lpsrPart(name, absoluteCode, generateNumericalTime); assert(o!=0);
  return o;
}

lpsrPart::lpsrPart(std::string name, bool absoluteCode, bool generateNumericalTime)
  : lpsrElement("")
{
  fPartName = name;
  fPartAbsoluteCode = absoluteCode;
  fGenerateNumericalTime = generateNumericalTime;
  
  // create the implicit lpsrSequence element
  fPartLpsrSequence = lpsrSequence::create (lpsrSequence::kSpace);
  
  // add the implicit lpsrRepeat element
// JMI  fPartLpsrRepeat = lpsrRepeat::create ();
//  fPartLpsrSequence->appendElementToSequence (fPartLpsrRepeat);
  
  // add the default 4/4 time signature
  SlpsrTime time = lpsrTime::create (4, 4, fGenerateNumericalTime);
  SlpsrElement t = time;
  fPartLpsrSequence->appendElementToSequence (t);

}
lpsrPart::~lpsrPart() {}

void lpsrPart::printMusicXML(ostream& os)
{
  os << "<!-- lpsrPart??? -->" << std::endl;
}

void lpsrPart::printLpsrStructure(ostream& os)
{
  os << "Part" << std::endl;

  idtr++;

  os << idtr << fPartName << std::endl;
// JMI  if (! fPartAbsoluteCode)
  //  os << idtr << "\\relative " << std::endl;
  os << idtr << fPartLpsrSequence;

  idtr--;
}

void lpsrPart::printLilyPondCode(ostream& os)
{
  /*
  lpsrElement cmd = lpsrcmd::create("set Staff.instrumentName ="); // USER
  stringstream s1, s2;
  string instr = header.fPartName->getValue();
  */
  
  os << idtr << fPartName << " = ";
  if (! fPartAbsoluteCode) os << "\\relative ";
  os << "{" << std::endl;

  idtr++;

  os << fPartLpsrSequence << std::endl;

  idtr--;

  os << idtr << "}" << std::endl;
}

//______________________________________________________________________________
SlpsrPaper lpsrPaper::create() {
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

ostream& operator<< (ostream& os, const SlpsrPaper& pap) {
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
SlpsrHeader lpsrHeader::create() {
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
SlpsrLilypondVarValAssoc lpsrHeader::getWorkNumber () const
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
SlpsrLilypondVarValAssoc lpsrHeader::getWorkTitle () const
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
SlpsrLilypondVarValAssoc lpsrHeader::getMovementNumber () const
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
SlpsrLilypondVarValAssoc lpsrHeader::getMovementTitle () const
  { return fMovementTitle; }

void lpsrHeader::addCreator (std::string val)
{
  fCreators.push_back(
    lpsrLilypondVarValAssoc::create (
      "creator", val,
      lpsrLilypondVarValAssoc::kEqualSign,
      lpsrLilypondVarValAssoc::kQuotesAroundValue,
      lpsrLilypondVarValAssoc::kUncommented)
  );
}
std::vector<SlpsrLilypondVarValAssoc> lpsrHeader::getCreators () const
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
SlpsrLilypondVarValAssoc lpsrHeader::getRights () const
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
std::vector<SlpsrLilypondVarValAssoc> lpsrHeader::getSoftwares () const
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
SlpsrLilypondVarValAssoc lpsrHeader::getEncodingDate () const
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
SlpsrLilypondVarValAssoc lpsrHeader::getScoreInstrument () const
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
    std::vector<SlpsrLilypondVarValAssoc>::const_iterator i1;
    for (i1=fCreators.begin(); i1!=fCreators.end(); i1++) {
      os << idtr << (*i1);
    } // for
  }
    
  if (fRights) {
    os << idtr << fRights;
  }
    
  if (!fSoftwares.empty()) {
    std::vector<SlpsrLilypondVarValAssoc>::const_iterator i2;
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
  }
    
  if (!fCreators.empty()) {
    vector<SlpsrLilypondVarValAssoc>::const_iterator i1;
    for (i1=fCreators.begin(); i1!=fCreators.end(); i1++) {
      os << idtr << (*i1);
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
    vector<SlpsrLilypondVarValAssoc>::const_iterator i2;
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
  }
  
  os << "}" << std::endl;
  
  idtr--;
}

//______________________________________________________________________________
SlpsrLilypondVarValAssoc lpsrLilypondVarValAssoc::create(
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

ostream& operator<< (ostream& os, const SlpsrLilypondVarValAssoc& assoc) {
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
SlpsrSchemeVarValAssoc lpsrSchemeVarValAssoc::create(
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

ostream& operator<< (ostream& os, const SlpsrSchemeVarValAssoc& assoc)
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
SlpsrLayout lpsrLayout::create() {
  lpsrLayout* o = new lpsrLayout(); assert(o!=0);
  return o;
}

lpsrLayout::lpsrLayout() : lpsrElement("") {}
lpsrLayout::~lpsrLayout() {}

ostream& operator<< (ostream& os, const SlpsrLayout& lay)
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
SlpsrTime lpsrTime::create(int numerator, int denominator, bool generateNumericalTime)
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
SlpsrClef lpsrClef::create(std::string clefName)
{
  lpsrClef* o = new lpsrClef(clefName); assert(o!=0);
  return o;
}

lpsrClef::lpsrClef(std::string clefName)
  : lpsrElement("")
{
  fClefName=clefName;
}
lpsrClef::~lpsrClef() {}

void lpsrClef::printMusicXML(ostream& os)
{
  os << "<!-- lpsrClef??? -->" << std::endl;
}

void lpsrClef::printLpsrStructure(ostream& os)
{
  os << "clef" << " \"" << fClefName << "\"" << std::endl;
}

void lpsrClef::printLilyPondCode(ostream& os)
{
  os << "\\clef" << " \"" << fClefName << "\"" << std::endl;
}

//______________________________________________________________________________
SlpsrKey lpsrKey::create(std::string tonic, KeyMode keyMode)
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

ostream& operator<< (ostream& os, const SlpsrKey& key)
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
SlpsrMidi lpsrMidi::create()
{
  lpsrMidi* o = new lpsrMidi(); assert(o!=0);
  return o;
}

lpsrMidi::lpsrMidi()
  : lpsrElement("")
{
}
lpsrMidi::~lpsrMidi() {}

ostream& operator<< (ostream& os, const SlpsrMidi& mid)
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
SlpsrScore lpsrScore::create()
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

ostream& operator<< (ostream& os, const SlpsrScore& scr)
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
SlpsrNewstaffCommand lpsrNewstaffCommand::create()
{
  lpsrNewstaffCommand* o = new lpsrNewstaffCommand(); assert(o!=0);
  return o;
}

lpsrNewstaffCommand::lpsrNewstaffCommand(): lpsrElement("") {}
lpsrNewstaffCommand::~lpsrNewstaffCommand() {}

ostream& operator<< (ostream& os, const SlpsrNewstaffCommand& nstf)
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
SlpsrNewlyricsCommand lpsrNewlyricsCommand::create()
{
  lpsrNewlyricsCommand* o = new lpsrNewlyricsCommand(); assert(o!=0);
  return o;
}

lpsrNewlyricsCommand::lpsrNewlyricsCommand() : lpsrElement("") {}
lpsrNewlyricsCommand::~lpsrNewlyricsCommand() {}

ostream& operator<< (ostream& os, const SlpsrNewlyricsCommand& nstf)
{
  nstf->print(os);
  return os;
}

void lpsrNewlyricsCommand::printLpsrStructure(ostream& os)
{
  os << "NewlyricsCommand" << std::endl;

  idtr++;
  
  if (fNewStaffElements.empty()) {
    cerr <<
      "%ERROR, fNewStaffElements is empty" << std::endl;
    cout <<
      idtr << "%ERROR, fNewStaffElements is empty" << std::endl;
  } else {
    vector<SlpsrElement>::const_iterator
    iBegin = fNewStaffElements.begin(),
    iEnd   = fNewStaffElements.end(),
    i      = iBegin;
    for ( ; ; ) {
      os << idtr << (*i);
      if (++i == iEnd) break;
      os << std::endl;
    } // for
  }
    
  idtr--;
}

void lpsrNewlyricsCommand::printMusicXML(ostream& os)
{
  os << "<!-- lpsrNewlyricsCommand??? -->" << std::endl;
}

void lpsrNewlyricsCommand::printLilyPondCode(ostream& os)
{  
  os << "\\new Lyrics <<" << std::endl;
  
  idtr++;
  
  if (fNewStaffElements.empty()) {
    cerr <<
      "%ERROR, fNewStaffElements is empty" << std::endl;
    cout <<
      "%ERROR, fNewStaffElements is empty" << std::endl;
  } else {
    vector<SlpsrElement>::const_iterator
    iBegin = fNewStaffElements.begin(),
    iEnd   = fNewStaffElements.end(),
    i      = iBegin;
    for ( ; ; ) {
      os << idtr << (*i);
      if (++i == iEnd) break;
      os << std::endl;
    } // for
  }
    
  idtr--;
  
  os << ">>" << std::endl;
}

//______________________________________________________________________________
SlpsrVariableUseCommand lpsrVariableUseCommand::create(std::string variableName)
{
  lpsrVariableUseCommand* o = new lpsrVariableUseCommand(variableName); assert(o!=0);
  return o;
}

lpsrVariableUseCommand::lpsrVariableUseCommand(std::string variableName)
  : lpsrElement("")
{ fVariableName = variableName; }
lpsrVariableUseCommand::~lpsrVariableUseCommand() {}

ostream& operator<< (ostream& os, const SlpsrVariableUseCommand& nstf)
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
SlpsrRepeat lpsrRepeat::create()
{
  lpsrRepeat* o = new lpsrRepeat(); assert(o!=0);
  return o;
}

lpsrRepeat::lpsrRepeat() : lpsrElement("")
{
  fActuallyUsed = false;
}
lpsrRepeat::~lpsrRepeat() {}

ostream& operator<< (ostream& os, const SlpsrRepeat& rept)
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
    std::vector<SlpsrSequence>::const_iterator i;
    for (i=fAlternateEndings.begin(); i!=fAlternateEndings.end(); i++) {
      os << idtr << (*i);
    } // for
  idtr--;
}

void lpsrRepeat::printLilyPondCode(ostream& os)
{
  os << "Repeat" << std::endl;
}


}
