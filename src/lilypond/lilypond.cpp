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

#include "lilypond.h"

using namespace std;

namespace MusicXML2 
{

//______________________________________________________________________________
haendel lilypondelement::hdl;


//______________________________________________________________________________
Slilypondelement lilypondelement::create(bool debug) 
{
  lilypondelement * o = new lilypondelement(debug); assert(o!=0);
  return o; 
}

lilypondelement::lilypondelement(bool debug)
{
  fDebug = debug;
}
lilypondelement::~lilypondelement() {}

ostream& operator<< (ostream& os, const Slilypondelement& elt)
{
  elt->printLilyPondCode(os);
  return os;
}

void lilypondelement::printStructure(ostream& os)
{
  os << "\%{ lilypondelement??? \%}" << hdl;
}

void lilypondelement::printLilyPondCode(ostream& os)
{
  os << "\%{ lilypondelement??? \%}" << hdl;
}

//______________________________________________________________________________
Slilypondnoteduration lilypondnoteduration::create(int num, int denom, int dots) {
  lilypondnoteduration * o = new lilypondnoteduration (num, denom, dots);
  assert(o!=0); 
  return o;
}

lilypondnoteduration::lilypondnoteduration (int num, int denom, int dots)
  : lilypondelement("")
{
  fNum=num; fDenom=denom; fDots=dots; 
}
lilypondnoteduration::~lilypondnoteduration() {}

void lilypondnoteduration::sett (int num, int denom, int dots) {
  fNum=num; fDenom=denom; fDots=dots; 
}

ostream& operator<< (ostream& os, const Slilypondnoteduration& dur)
{
  dur->printLilyPondCode(os);
  return os;
}

void lilypondnoteduration::printStructure(ostream& os)
{
  os << "\%{ lilypondnoteduration??? \%}" << hdl;
}

void lilypondnoteduration::printLilyPondCode(ostream& os)
{
  // divisions are per quater, Lilypond durations are in whole notes
  //os << "|"  << fLilypondnoteduration.fNum << "|" << fLilypondnoteduration.fDenom;

  int noteDivisions         = fNum;
  int divisionsPerWholeNote = fDenom ;
  
  if (divisionsPerWholeNote == 0)
    {
    os << 
      std::endl << 
      "%--> lilypondnoteduration::printLilyPondCode, noteDivisions = " << noteDivisions <<
      ", divisionsPerWholeNote = " << divisionsPerWholeNote << std::endl;
    return;
  }
  
  div_t divresult = div (noteDivisions, divisionsPerWholeNote);  
  int   div = divresult.quot;
  int   mod = divresult.rem;
  
  switch (div) {
    case 8:
    case 7:
    case 6:
    case 5:
      os << "\\maxima";
      break;
    case 4:
    case 3:
      os << "\\longa";
      break;
    case 2:
      os << "\\breva";
      break;
    case 1:
      os << "1";
      break;
    case 0:
      {
      // shorter than a whole note
      //os << "(shorter than a whole note) ";
      int weight = 2; // half note
      int n = noteDivisions*2;

      while (n < divisionsPerWholeNote) {
         weight *= 2;
         n *= 2;
      } // while
      os << weight;
      }
      break;
    default:
      cerr <<
        "*** ERROR, MusicXML note duration " << noteDivisions << "/" << 
        divisionsPerWholeNote << " is too large" << std::endl;
  } // switch
  
  // print the dots if any  
  if (fDots > 0) {
    while (fDots-- > 0) {
      os << ".";  
    } // while
  }
}

//______________________________________________________________________________
Slilypondnote lilypondnote::create() 
{  
  lilypondnote * o = new lilypondnote (); assert(o!=0); 
  return o;
}

lilypondnote::lilypondnote() : lilypondelement("")
{
  fDiatonicNote         = lilypondnote::kNoDiatonicNote;
  fAlteration           = lilypondnote::kNoAlteration;
  fOctave               = -1;
//  fLilypondnoteduration = lilypondnoteduration::create(0, 0, 0);
  fVoice                = -1;
}
lilypondnote::~lilypondnote() {}

void lilypondnote::updateNote(
    bool                  currentStepIsRest,
    DiatonicNote          diatonicNote,
    Alteration            alteration,
    int                   octave,
    Slilypondnoteduration dur,
    LilypondNote          lilypondNote,
    int                   voice,
    bool                  noteBelongsToAChord)
{
  fCurrentStepIsRest = currentStepIsRest;
  fDiatonicNote = diatonicNote;
  fAlteration = alteration;
  fOctave = octave;
  fLilypondnoteduration = dur;
  fLilypondNote = lilypondNote;
  fVoice = voice;
  fNoteBelongsToAChord = noteBelongsToAChord;
}

void lilypondnote::setNoteBelongsToAChord () {
  fNoteBelongsToAChord = true;
}

void lilypondnote::addDynamics (Slilyponddynamics dyn) {
  fNoteDynamics.push_back(dyn);
}
void lilypondnote::addWedge (Slilypondwedge wdg) {
  fNoteWedges.push_back(wdg);
}

Slilyponddynamics lilypondnote::removeFirstDynamics () {
  Slilyponddynamics dyn = fNoteDynamics.front();
  fNoteDynamics.pop_front();
  return dyn;
}
Slilypondwedge lilypondnote::removeFirstWedge () {
  Slilypondwedge wdg = fNoteWedges.front();
  fNoteWedges.pop_front();
  return wdg;
}

ostream& operator<< (ostream& os, const Slilypondnote& elt)
{
  elt->printLilyPondCode(os);
  return os;
}

void lilypondnote::printStructure(ostream& os)
{
  os << "\%{ lilypondnote??? \%}" << hdl;
}

void lilypondnote::printLilyPondCode(ostream& os)
{
  if (fCurrentStepIsRest) os << "r";
  else {
    // print the note name
    //JMI assertLilypond(fLilypondNote != k_NoLilypondNote, "fLilypondNote != k_NoLilypondNote");
    switch (fLilypondNote) {
      
      case k_aeseh:
        os << "aeseh";
        break;
      case k_aes:
        os << "aes";
        break;
      case k_aeh:
        os << "aeh";
        break;
      case k_a:
        os << "a";
        break;
      case k_aih:
        os << "aih";
        break;
      case k_ais:
        os << "ais";
        break;
      case k_aisih:
        os << "aisih";
        break;
        
      case k_beseh:
        os << "beseh";
        break;
      case k_bes:
        os << "bes";
        break;
      case k_beh:
        os << "beh";
        break;
      case k_b:
        os << "b";
        break;
      case k_bih:
        os << "bih";
        break;
      case k_bis:
        os << "bis";
        break;
      case k_bisih:
        os << "bisih";
        break;
        
      case k_ceseh:
        os << "ceseh";
        break;
      case k_ces:
        os << "ces";
        break;
      case k_ceh:
        os << "ceh";
        break;
      case k_c:
        os << "c";
        break;
      case k_cih:
        os << "cih";
        break;
      case k_cis:
        os << "cis";
        break;
      case k_cisih:
        os << "cisih";
        break;
        
      case k_deseh:
        os << "deseh";
        break;
      case k_des:
        os << "des";
        break;
      case k_deh:
        os << "deh";
        break;
      case k_d:
        os << "d";
        break;
      case k_dih:
        os << "dih";
        break;
      case k_dis:
        os << "dis";
        break;
      case k_disih:
        os << "disih";
        break;
  
      case k_eeseh:
        os << "eeseh";
        break;
      case k_ees:
        os << "ees";
        break;
      case k_eeh:
        os << "eeh";
        break;
      case k_e:
        os << "e";
        break;
      case k_eih:
        os << "eih";
        break;
      case k_eis:
        os << "eis";
        break;
      case k_eisih:
        os << "eisih";
        break;
        
      case k_feseh:
        os << "feseh";
        break;
      case k_fes:
        os << "fes";
        break;
      case k_feh:
        os << "feh";
        break;
      case k_f:
        os << "f";
        break;
      case k_fih:
        os << "fih";
        break;
      case k_fis:
        os << "fis";
        break;
      case k_fisih:
        os << "fisih";
        break;
        
      case k_geseh:
        os << "geseh";
        break;
      case k_ges:
        os << "ges";
        break;
      case k_geh:
        os << "geh";
        break;
      case k_g:
        os << "g";
        break;
      case k_gih:
        os << "gih";
        break;
      case k_gis:
        os << "gis";
        break;
      case k_gisih:
        os << "gisih";
        break;
      default:
        os << "Note" << fLilypondNote << "???";
    } // switch
  }
  
  if (! fNoteBelongsToAChord) {
    // print the note duration
    os << fLilypondnoteduration;
    
    // print the dynamics if any
    std::list<Slilyponddynamics>::const_iterator i1;
    for (i1=fNoteDynamics.begin(); i1!=fNoteDynamics.end(); i1++) {
      os << " " << (*i1);
    } // for
  
    // print the wedges if any
    std::list<Slilypondwedge>::const_iterator i2;
    for (i2=fNoteWedges.begin(); i2!=fNoteWedges.end(); i2++) {
      os << " " << (*i2);
    } // for
  }
}

/*

std::string lilypondnote::octaveRepresentation (char octave)
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
Slilypondsequence lilypondsequence::create(ElementsSeparator elementsSeparator)
{
  lilypondsequence* o = new lilypondsequence(elementsSeparator); assert(o!=0);
  return o;
}

lilypondsequence::lilypondsequence(ElementsSeparator elementsSeparator) : lilypondelement("")
{
  fElementsSeparator=elementsSeparator;
}
lilypondsequence::~lilypondsequence() {}

ostream& operator<< (ostream& os, const Slilypondsequence& elt)
{
  elt->printLilyPondCode(os);
  return os;
}

void lilypondsequence::printStructure(ostream& os)
{
  os << "\%{ lilypondsequence??? \%}" << hdl;
}

void lilypondsequence::printLilyPondCode(ostream& os)
{
  vector<Slilypondelement>::const_iterator i;
  for (i=fSequenceElements.begin(); i!=fSequenceElements.end(); i++) {
    os << (*i);
    if (fElementsSeparator == kEndOfLine) os << hdl;
    // JMI
    else os << " ";
  } // for
}

//______________________________________________________________________________
Slilypondparallel lilypondparallel::create(ElementsSeparator elementsSeparator)
{
  lilypondparallel* o = new lilypondparallel(elementsSeparator); assert(o!=0);
  return o;
}

lilypondparallel::lilypondparallel(ElementsSeparator elementsSeparator) : lilypondelement("")
{
  fElementsSeparator=elementsSeparator;
}
lilypondparallel::~lilypondparallel() {}

ostream& operator<< (ostream& os, const Slilypondparallel& elt)
{
  elt->printLilyPondCode(os);
  return os;
}

void lilypondparallel::printStructure(ostream& os)
{
  os << "\%{ lilypondparallel??? \%}" << hdl;
}

void lilypondparallel::printLilyPondCode(ostream& os)
{
  hdl++;
  
  os << "<<" << hdl;
  
  if (fParallelElements.empty()) {
    cerr <<
      "%ERROR, lilypond parallel music is empty" << std::endl;
    cout <<
      "%ERROR, lilypond parallel music is empty" << std::endl;
  } else {
    vector<Slilypondelement>::const_iterator
    iBegin = fParallelElements.begin(),
    iEnd   = fParallelElements.end(),
    i      = iBegin;
    for ( ; ; ) {
      os << (*i);
      if (++i == iEnd) break;
      if (fElementsSeparator == kEndOfLine) os << hdl;
      else os << " ";
    } // for
  }
  
  hdl--;
  
  os << ">>" << hdl;
}

//______________________________________________________________________________
Slilypondchord lilypondchord::create(Slilypondnoteduration chordduration)
{
  lilypondchord* o = new lilypondchord(chordduration); assert(o!=0);
  return o;
}

lilypondchord::lilypondchord (Slilypondnoteduration chordduration)
  : lilypondelement("")
{
  fChordDuration = chordduration;
}
lilypondchord::~lilypondchord() {}

void lilypondchord::addDynamics (Slilyponddynamics dyn) {
  fChordDynamics.push_back(dyn);
}
void lilypondchord::addWedge (Slilypondwedge wdg) {
  fChordWedges.push_back(wdg);
}

ostream& operator<< (ostream& os, const Slilypondchord& chrd)
{
  chrd->printLilyPondCode(os);
  return os;
}

void lilypondchord::printStructure(ostream& os)
{
  os << "\%{ lilypondchord??? \%}" << hdl;
}

void lilypondchord::printLilyPondCode(ostream& os)
{
  std::vector<Slilypondnote>::const_iterator
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
  std::list<Slilyponddynamics>::const_iterator i1;
  for (i1=fChordDynamics.begin(); i1!=fChordDynamics.end(); i1++) {
    os << " " << (*i1);
  } // for

  // print the wedges if any
  std::list<Slilypondwedge>::const_iterator i2;
  for (i2=fChordWedges.begin(); i2!=fChordWedges.end(); i2++) {
    os << " " << (*i2);
  } // for
}

//______________________________________________________________________________
Slilypondbarline lilypondbarline::create(int nextBarNumber)
{
  lilypondbarline* o = new lilypondbarline(nextBarNumber); assert(o!=0);
  return o;
}

lilypondbarline::lilypondbarline(int nextBarNumber) : lilypondelement("")
{
  fNextBarNumber=nextBarNumber; 
}
lilypondbarline::~lilypondbarline() {}

ostream& operator<< (ostream& os, const Slilypondbarline& elt)
{
  elt->printLilyPondCode(os);
  return os;
}

void lilypondbarline::printStructure(ostream& os)
{
  os << "\%{ lilypondbarline??? \%}" << hdl;
}

void lilypondbarline::printLilyPondCode(ostream& os)
{
  hdl++;
  os << "| % ";
  hdl--;
  os << fNextBarNumber << hdl;
}

//______________________________________________________________________________
Slilypondcomment lilypondcomment::create(std::string contents, GapKind gapKind)
{
  lilypondcomment* o = new lilypondcomment(contents, gapKind); assert(o!=0);
  return o;
}

lilypondcomment::lilypondcomment(std::string contents, GapKind gapKind)
  : lilypondelement("")
{
  fContents=contents;
  fGapKind=gapKind;
}
lilypondcomment::~lilypondcomment() {}

ostream& operator<< (ostream& os, const Slilypondcomment& elt)
{
  elt->printLilyPondCode(os);
  return os;
}

void lilypondcomment::printStructure(ostream& os)
{
  os << "\%{ lilypondcomment??? \%}" << hdl;
}

void lilypondcomment::printLilyPondCode(ostream& os)
{
  hdl++;
  os << "% " << fContents;
  if (fGapKind == kGapAfterwards) os << hdl;
  hdl--;
}

//______________________________________________________________________________
Slilypondbreak lilypondbreak::create(int nextBarNumber)
{
  lilypondbreak* o = new lilypondbreak(nextBarNumber); assert(o!=0);
  return o;
}

lilypondbreak::lilypondbreak(int nextBarNumber) : lilypondelement("")
{
  fNextBarNumber=nextBarNumber; 
}
lilypondbreak::~lilypondbreak() {}

ostream& operator<< (ostream& os, const Slilypondbreak& elt)
{
  elt->printLilyPondCode(os);
  return os;
}

void lilypondbreak::printStructure(ostream& os)
{
  os << "\%{ lilypondbreak??? \%}" << hdl;
}

void lilypondbreak::printLilyPondCode(ostream& os)
{
  hdl++;
  os << "\\myBreak | % " << fNextBarNumber << hdl;
  hdl--;
  os << "\\barNumberCheck #" << fNextBarNumber << hdl;
}

//______________________________________________________________________________
Slilypondbeam lilypondbeam::create(int number, BeamKind beamKind)
{
  lilypondbeam* o = new lilypondbeam(number, beamKind); assert(o!=0);
  return o;
}

lilypondbeam::lilypondbeam(int number, BeamKind beamKind)
  : lilypondelement("")
{
  fBeamNumber = number;
  fBeamKind   = beamKind; 
}
lilypondbeam::~lilypondbeam() {}

ostream& operator<< (ostream& os, const Slilypondbeam& dyn)
{
  dyn->printLilyPondCode(os);
  return os;
}

void lilypondbeam::printStructure(ostream& os)
{
  os << "\%{ lilypondbeam??? \%}" << hdl;
}

void lilypondbeam::printLilyPondCode(ostream& os)
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
Slilyponddynamics lilyponddynamics::create(DynamicsKind dynamicsKind)
{
  lilyponddynamics* o = new lilyponddynamics(dynamicsKind); assert(o!=0);
  return o;
}

lilyponddynamics::lilyponddynamics(DynamicsKind dynamicsKind) : lilypondelement("")
{
  fDynamicsKind = dynamicsKind; 
}
lilyponddynamics::~lilyponddynamics() {}

ostream& operator<< (ostream& os, const Slilyponddynamics& dyn)
{
  dyn->printLilyPondCode(os);
  return os;
}

void lilyponddynamics::printStructure(ostream& os)
{
  os << "\%{ lilyponddynamics??? \%}" << hdl;
}

void lilyponddynamics::printLilyPondCode(ostream& os)
{
  switch (fDynamicsKind) {
    case kFDynamics:
      os << "\\f";
      break;
    case kPDynamics:
      os << "\\p";
      break;
    default:
      os << "Dynamics " << fDynamicsKind << "???";
  } // switch
}

//______________________________________________________________________________
Slilypondwedge lilypondwedge::create(WedgeKind wedgeKind)
{
  lilypondwedge* o = new lilypondwedge(wedgeKind); assert(o!=0);
  return o;
}

lilypondwedge::lilypondwedge(WedgeKind wedgeKind) : lilypondelement("")
{
  fWedgeKind=wedgeKind; 
}
lilypondwedge::~lilypondwedge() {}

ostream& operator<< (ostream& os, const Slilypondwedge& wdg)
{
  wdg->printLilyPondCode(os);
  return os;
}

void lilypondwedge::printStructure(ostream& os)
{
  os << "\%{ lilypondwedge??? \%}" << hdl;
}

void lilypondwedge::printLilyPondCode(ostream& os)
{
  switch (fWedgeKind) {
    case lilypondwedge::kCrescendoWedge:
      cout << "\\<";
      break;
    case lilypondwedge::kDecrescendoWedge:
      cout << "\\>";
      break;
    case lilypondwedge::kStopWedge:
      cout << "\\!";
      break;
  } // switch
}

//______________________________________________________________________________
Slilypondlyrics lilypondlyrics::create(std::string name, std::string contents)
{
  lilypondlyrics* o = new lilypondlyrics(name, contents); assert(o!=0);
  return o;
}

lilypondlyrics::lilypondlyrics(std::string name, std::string contents) : lilypondelement("")
{
  fLyricsName = name;
  fLyricsContents=contents; 
}
lilypondlyrics::~lilypondlyrics() {}

void lilypondlyrics::printStructure(ostream& os)
{  
  hdl++;
  os << "lilypondlyrics " << fLyricsName << hdl;
  hdl--;
  os << fLyricsContents << hdl;
}

void lilypondlyrics::printLilyPondCode(ostream& os)
{  
  hdl++;
  os << fLyricsName << " = \\lyricmode {" << hdl;
  hdl--;
  os << fLyricsContents << hdl;
  os << "}" << hdl;
}

//______________________________________________________________________________
Slilypondpart lilypondpart::create(std::string name, bool absoluteCode, bool generateNumericalTime)
{
  lilypondpart* o = new lilypondpart(name, absoluteCode, generateNumericalTime); assert(o!=0);
  return o;
}

lilypondpart::lilypondpart(std::string name, bool absoluteCode, bool generateNumericalTime)
  : lilypondelement("")
{
  fPartName = name;
  fPartAbsoluteCode = absoluteCode;
  fGenerateNumericalTime = generateNumericalTime;
  
  // create the implicit lilypondsequence element
  fPartLilypondsequence = lilypondsequence::create(lilypondsequence::kSpace);
  
  // add the default 4/4 time signature
  Slilypondtime time = lilypondtime::create(4, 4, fGenerateNumericalTime);
  Slilypondelement t = time;
  fPartLilypondsequence->addElementToSequence (t);

}
lilypondpart::~lilypondpart() {}

void lilypondpart::printStructure(ostream& os)
{
  os << "\%{ lilypondpart??? \%}" << hdl;
}

void lilypondpart::printLilyPondCode(ostream& os)
{
  /*
  lilypondelement cmd = lilypondcmd::create("set Staff.instrumentName ="); // USER
  stringstream s1, s2;
  string instr = header.fPartName->getValue();
  */
  
  hdl++;
  os << fPartName << " = ";
  if (!fPartAbsoluteCode) os << "\\relative ";
  os << "{" << hdl << fPartLilypondsequence << hdl;
  hdl--;
  os << "}" << hdl;
}

//______________________________________________________________________________
Slilypondpaper lilypondpaper::create()
{
  lilypondpaper* o = new lilypondpaper(); assert(o!=0);
  return o;
}

lilypondpaper::lilypondpaper() : lilypondelement("")
{
}
lilypondpaper::~lilypondpaper() {}

ostream& operator<< (ostream& os, const Slilypondpaper& pap)
{
  pap->printLilyPondCode(os);
  return os;
}

void lilypondpaper::printStructure(ostream& os)
{
  os << "\%{ lilypondpaper??? \%}" << hdl;
}

void lilypondpaper::printLilyPondCode(ostream& os)
{  
  hdl++;
  os << "\\paper {" << hdl;
  hdl--;
  os << "% to be completed" << hdl;
  os << "}" << hdl;
}

//______________________________________________________________________________
Slilypondheader lilypondheader::create()
{
  lilypondheader* o = new lilypondheader(); assert(o!=0);
  return o;
}

lilypondheader::lilypondheader() : lilypondelement("")
{
}
lilypondheader::~lilypondheader() {}

void lilypondheader::printStructure(ostream& os)
{
  os << "\%{ lilypondheader??? \%}" << hdl;
}

void lilypondheader::printLilyPondCode(ostream& os)
{
  hdl++;
  
  os << "\\header {" << hdl;
  
  if (fScorePartwise) {
    std::string source = fScorePartwise->getAttributeValue("version");
    std::string dest;
    std::for_each( source.begin(), source.end(), stringquoteescaper(dest));
    os << "%MusicXMl_version = \"" << dest << "\"" << hdl;
  }
  
  if (fWorkNumber) {
    std::string source = fWorkNumber->getValue();
    std::string dest;
    std::for_each( source.begin(), source.end(), stringquoteescaper(dest));
    os << "%work_number = \""  << dest << "\"" << hdl;
  }
  
  if (fWorkTitle) {
    std::string source = fWorkTitle->getValue();
    std::string dest;
    std::for_each( source.begin(), source.end(), stringquoteescaper(dest));
    os << "%work_title = \""  << dest << "\"" << hdl;
  }
    
  if (fMovementNumber) {
    os << "%movement_number = \""  << fMovementNumber->getValue() << "\"" << hdl;
  }
    
  if (fMovementTitle) {
    std::string source = fMovementTitle->getValue();
    std::string dest;
    std::for_each( source.begin(), source.end(), stringquoteescaper(dest));
    os << "%movement_title = \""  << dest << "\"" << hdl;
  }
    
  if (!fCreators.empty()) {
    vector<S_creator>::const_iterator i1;
    for (i1=fCreators.begin(); i1!=fCreators.end(); i1++) {
      string type = (*i1)->getAttributeValue("type");
      std::transform(type.begin(), type.end(), type.begin(), ::tolower);
      if (type == "composer")
        os << "" << type << " = \""  << (*i1)->getValue() << "\"" << hdl;
      else
        os << "%" << type << " = \""  << (*i1)->getValue() << "\"" << hdl;
    } // for
  }
    
  if (fRights) {
    std::string source = fRights->getValue();
    std::string dest;
    std::for_each( source.begin(), source.end(), stringquoteescaper(dest));
    os << "%rights = \""  << dest << "\"" << hdl;
  }
    
  if (!fSoftwares.empty()) {
    vector<S_software>::const_iterator i2;
    for (i2=fSoftwares.begin(); i2!=fSoftwares.end(); i2++) {
      std::string source = (*i2)->getValue();
      std::string dest;
      std::for_each( source.begin(), source.end(), stringquoteescaper(dest));
      os << "%software = \""  << dest << "\"" << hdl;
    } // for
  }
    
  hdl--;

  if (fEncodingDate) {
    std::string source = fEncodingDate->getValue();
    std::string dest;
    std::for_each( source.begin(), source.end(), stringquoteescaper(dest));
    os << "%encoding_date = \""  << dest << "\"" << hdl;
  }
  
  os << "}" << hdl; 
}

//______________________________________________________________________________
Slilypondvariablevalueassociation lilypondvariablevalueassociation::create(
      std::string     variableName,
      std::string     value, 
      VarValSeparator varValSeparator,
      QuotesKind      quotesKind,
      CommentedKind   commentedKind )
{
  lilypondvariablevalueassociation* o = new lilypondvariablevalueassociation(
    variableName, value, varValSeparator, quotesKind, commentedKind);
  assert(o!=0);
  return o;
}

lilypondvariablevalueassociation::lilypondvariablevalueassociation(
      std::string     variableName,
      std::string     value, 
      VarValSeparator varValSeparator,
      QuotesKind      quotesKind,
      CommentedKind   commentedKind )
  : lilypondelement("")
{
  fVariableName=variableName;
  fVariableValue=value;
  fVarValSeparator=varValSeparator;
  fQuotesKind=quotesKind;
  fCommentedKind=commentedKind;
}

lilypondvariablevalueassociation::~lilypondvariablevalueassociation() {}

void lilypondvariablevalueassociation::changeAssociation (std::string value)
{
  fVariableValue=value;
}

void lilypondvariablevalueassociation::printStructure(ostream& os)
{
  os << "\%{ lilypondvariablevalueassociation??? \%}" << hdl;
}

void lilypondvariablevalueassociation::printLilyPondCode(ostream& os)
{
  if (fCommentedKind == kCommented) os << "\%";
  os << fVariableName;
  if (fVarValSeparator == kEqualSign) os << " = ";
  else os << " ";
  if (fQuotesKind == kQuotesAroundValue) os << "\"";
  os << fVariableValue;
  if (fQuotesKind == kQuotesAroundValue) os << "\"";
  os << hdl;
}

//______________________________________________________________________________
Slilypondschemevariablevalueassociation lilypondschemevariablevalueassociation::create(
      std::string     variableName,
      std::string     value, 
      CommentedKind   commentedKind )
{
  lilypondschemevariablevalueassociation* o = new lilypondschemevariablevalueassociation(
    variableName, value, commentedKind);
  assert(o!=0);
  return o;
}

lilypondschemevariablevalueassociation::lilypondschemevariablevalueassociation(
      std::string     variableName,
      std::string     value, 
      CommentedKind   commentedKind )
  : lilypondelement("")
{
  fVariableName=variableName;
  fVariableValue=value;
  fCommentedKind=commentedKind;
}

lilypondschemevariablevalueassociation::~lilypondschemevariablevalueassociation() {}

void lilypondschemevariablevalueassociation::changeAssociation (std::string value)
{
  fVariableValue=value;
}

void lilypondschemevariablevalueassociation::printStructure(ostream& os)
{
  os << "\%{ lilypondschemevariablevalueassociation??? \%}" << hdl;
}

void lilypondschemevariablevalueassociation::printLilyPondCode(ostream& os)
{
  if (fCommentedKind == kCommented) os << "\%";
  os << "#(" << fVariableName << " " << fVariableValue << ")" << hdl;
}

//______________________________________________________________________________
Slilypondglobalsettings lilypondglobalsettings::create(VarValSeparator varValSeparator)
{
  lilypondglobalsettings* o = new lilypondglobalsettings(varValSeparator); assert(o!=0);
  return o;
}

lilypondglobalsettings::lilypondglobalsettings(VarValSeparator varValSeparator)
  : lilypondelement("")
{
  fVarValSeparator=varValSeparator;
}

lilypondglobalsettings::~lilypondglobalsettings() {}

void lilypondglobalsettings::setAssociation (std::string var, std::string val) {
  fAssociations[var]=val;
}
void lilypondglobalsettings::changeAssociation (std::string var, std::string val) {
  assert(fAssociations.count(var));
  fAssociations[var]=val;
}

void lilypondglobalsettings::printStructure(ostream& os)
{
  os << "\%{ lilypondglobalsettings??? \%}" << hdl;
}

void lilypondglobalsettings::printLilyPondCode(ostream& os)
{
  if (!fAssociations.empty()) {
    for (std::map<std::string, std::string> ::iterator 
          it=fAssociations.begin(); it!=fAssociations.end(); ++it) {
    
      string   var=it->first.c_str();
      string   val=it->second.c_str();
      
      os << var;
      if (fVarValSeparator == kEqualSign) os << " = ";
      else os << " ";
      os << "\"" << val << "\"" << hdl;
    } // for
  }
}

//______________________________________________________________________________
Slilypondschemeglobalsettings lilypondschemeglobalsettings::create()
{
  lilypondschemeglobalsettings* o = new lilypondschemeglobalsettings(); assert(o!=0);
  return o;
}

lilypondschemeglobalsettings::lilypondschemeglobalsettings()
  : lilypondelement("")
{}

lilypondschemeglobalsettings::~lilypondschemeglobalsettings() {}

void lilypondschemeglobalsettings::setAssociation (std::string var, std::string val) {
  fAssociations[var]=val;
}
void lilypondschemeglobalsettings::changeAssociation (std::string var, std::string val) {
  assert(fAssociations.count(var));
  fAssociations[var]=val;
}

void lilypondschemeglobalsettings::printStructure(ostream& os)
{
  os << "\%{ lilypondschemeglobalsettings??? \%}" << hdl;
}

void lilypondschemeglobalsettings::printLilyPondCode(ostream& os)
{
  if (!fAssociations.empty()) {
    for (std::map<std::string, std::string> ::iterator 
          it=fAssociations.begin(); it!=fAssociations.end(); ++it) {
    
      string   var=it->first.c_str();
      string   val=it->second.c_str();
      
      os << "#(" << var << " " << val << ")" << hdl;
    } // for
  }
}

//______________________________________________________________________________
Slilypondlayout lilypondlayout::create()
{
  lilypondlayout* o = new lilypondlayout(); assert(o!=0);
  return o;
}

lilypondlayout::lilypondlayout() : lilypondelement("")
{}
lilypondlayout::~lilypondlayout() {}

ostream& operator<< (ostream& os, const Slilypondlayout& lay)
{
  lay->printLilyPondCode(os);
  return os;
}

void lilypondlayout::printStructure(ostream& os)
{
  os << "\%{ lilypondlayout??? \%}" << hdl;
}

void lilypondlayout::printLilyPondCode(ostream& os)
{  
  hdl++;
  os << "\\layout {" << hdl;
  hdl--;
  os << "% to be completed" << hdl;
  os << "}" << hdl;
}

//______________________________________________________________________________
Slilypondtime lilypondtime::create(int numerator, int denominator, bool generateNumericalTime)
{
  lilypondtime* o = new lilypondtime(numerator, denominator, generateNumericalTime); assert(o!=0);
  return o;
}

lilypondtime::lilypondtime(int numerator, int denominator, bool generateNumericalTime)
  : lilypondelement("")
{
  fRational = rational(numerator, denominator);
// JMI  fNumerator=numerator; 
  //fDenominator=denominator;
  fGenerateNumericalTime = generateNumericalTime;
}
lilypondtime::~lilypondtime() {}

void lilypondtime::printStructure(ostream& os)
{
  os << "\%{ lilypondtime??? \%}" << hdl;
}

void lilypondtime::printLilyPondCode(ostream& os)
{
//  os << fName << "\\time \"" << fNumerator << "/" << fDenominator << "\"" << std::endl;
  if (fGenerateNumericalTime)
    os << "\\numericTimeSignature ";
  os << "\\time " << fRational.getNumerator() << "/" << fRational.getDenominator() << hdl;
}

//______________________________________________________________________________
Slilypondclef lilypondclef::create(std::string clefName)
{
  lilypondclef* o = new lilypondclef(clefName); assert(o!=0);
  return o;
}

lilypondclef::lilypondclef(std::string clefName) : lilypondelement("")
{
  fClefName=clefName;
}
lilypondclef::~lilypondclef() {}

void lilypondclef::printStructure(ostream& os)
{
  os << "\%{ lilypondclef??? \%}" << hdl;
}

void lilypondclef::printLilyPondCode(ostream& os)
{
  os << "\\clef \"" << fClefName << "\"" << hdl;
}

//______________________________________________________________________________
Slilypondkey lilypondkey::create(std::string tonicNote, KeyMode keyMode)
{
  lilypondkey* o = new lilypondkey(tonicNote, keyMode); assert(o!=0);
  return o;
}

lilypondkey::lilypondkey(std::string tonicNote, KeyMode keyMode) : lilypondelement("")
{
  fTonicNote=tonicNote;
  fKeyMode=keyMode; 
}
lilypondkey::~lilypondkey() {}

ostream& operator<< (ostream& os, const Slilypondkey& key)
{
  key->printLilyPondCode(os);
  return os;
}

void lilypondkey::printStructure(ostream& os)
{
  os << "\%{ lilypondkey??? \%}" << hdl;
}

void lilypondkey::printLilyPondCode(ostream& os)
{
  os << "\\key " << fTonicNote << " ";
  if (fKeyMode == kMajor) os << "\\major";
  else os << "\\minor";
  os << hdl;
}

//______________________________________________________________________________
Slilypondmidi lilypondmidi::create()
{
  lilypondmidi* o = new lilypondmidi(); assert(o!=0);
  return o;
}

lilypondmidi::lilypondmidi() : lilypondelement("")
{}
lilypondmidi::~lilypondmidi() {}

ostream& operator<< (ostream& os, const Slilypondmidi& mid)
{
  mid->printLilyPondCode(os);
  return os;
}

void lilypondmidi::printStructure(ostream& os)
{
  os << "\%{ lilypondmidi??? \%}" << hdl;
}

void lilypondmidi::printLilyPondCode(ostream& os)
{
  hdl++;
  
  os << "\\midi {" << hdl;
  
  os << "% to be completed" << hdl;

  hdl++;
  
  os << "}";
}

//______________________________________________________________________________
Slilypondscore lilypondscore::create()
{
  lilypondscore* o = new lilypondscore(); assert(o!=0);
  return o;
}

lilypondscore::lilypondscore() : lilypondelement("")
{
  // create the parallel music element
  fScoreParallelMusic = lilypondparallel::create(lilypondparallel::kEndOfLine);
  
  // create the layout element
  fScoreLayout = lilypondlayout::create();
  
  // create the midi element
  fScoreMidi = lilypondmidi::create();
}
lilypondscore::~lilypondscore() {}

ostream& operator<< (ostream& os, const Slilypondscore& scr)
{
  scr->printLilyPondCode(os);
  return os;
}

void lilypondscore::printStructure(ostream& os)
{
  os << "\%{ lilypondscore??? \%}" << hdl;
}

void lilypondscore::printLilyPondCode(ostream& os)
{
  hdl++;
  
  os << "\\score {" << hdl;
  os << fScoreParallelMusic << hdl;
  os << fScoreLayout << hdl;

  hdl--;

  os << fScoreMidi << hdl;
  os << "}" << hdl; 
}

//______________________________________________________________________________
Slilypondnewstaffcmd lilypondnewstaffcmd::create()
{
  lilypondnewstaffcmd* o = new lilypondnewstaffcmd(); assert(o!=0);
  return o;
}

lilypondnewstaffcmd::lilypondnewstaffcmd() : lilypondelement("")
{}
lilypondnewstaffcmd::~lilypondnewstaffcmd() {}

ostream& operator<< (ostream& os, const Slilypondnewstaffcmd& nstf)
{
  nstf->printLilyPondCode(os);
  return os;
}

void lilypondnewstaffcmd::printStructure(ostream& os)
{
  os << "\%{ lilypondnewstaffcmd??? \%}" << hdl;
}

void lilypondnewstaffcmd::printLilyPondCode(ostream& os)
{
  hdl++;
  
  os << "\\new Staff <<" << hdl;
  
  if (fNewStaffElements.empty()) {
    cerr <<
      "%ERROR, lilypond newstaff is empty" << std::endl;
    cout <<
      "%ERROR, lilypond newstaff is empty" << std::endl;
  } else {
    vector<Slilypondelement>::const_iterator
    iBegin = fNewStaffElements.begin(),
    iEnd   = fNewStaffElements.end(),
    i      = iBegin;
    for ( ; ; ) {
      os << (*i);
      if (++i == iEnd) break;
      os << hdl;
    } // for
  }
  
  os << hdl;
  
  hdl--;
  
  os << ">>" << hdl;
}

//______________________________________________________________________________
Slilypondnewlyricscmd lilypondnewlyricscmd::create()
{
  lilypondnewlyricscmd* o = new lilypondnewlyricscmd(); assert(o!=0);
  return o;
}

lilypondnewlyricscmd::lilypondnewlyricscmd() : lilypondelement("")
{}
lilypondnewlyricscmd::~lilypondnewlyricscmd() {}

ostream& operator<< (ostream& os, const Slilypondnewlyricscmd& nstf)
{
  nstf->printLilyPondCode(os);
  return os;
}

void lilypondnewlyricscmd::printStructure(ostream& os)
{
  os << "\%{ lilypondnewlyricscmd??? \%}" << hdl;
}

void lilypondnewlyricscmd::printLilyPondCode(ostream& os)
{
  hdl++;
  
  os << "\\new Staff <<" << hdl;
  
  if (fNewStaffElements.empty()) {
    cerr <<
      "%ERROR, lilypond newstaff is empty" << std::endl;
    cout <<
      "%ERROR, lilypond newstaff is empty" << std::endl;
  } else {
    vector<Slilypondelement>::const_iterator
    iBegin = fNewStaffElements.begin(),
    iEnd   = fNewStaffElements.end(),
    i      = iBegin;
    for ( ; ; ) {
      os << (*i);
      if (++i == iEnd) break;
      os << hdl;
    } // for
  }
  
  os << hdl;
  
  hdl--;
  
  os << ">>" << hdl;
}

//______________________________________________________________________________
Slilypondvariableusecmd lilypondvariableusecmd::create(std::string variableName)
{
  lilypondvariableusecmd* o = new lilypondvariableusecmd(variableName); assert(o!=0);
  return o;
}

lilypondvariableusecmd::lilypondvariableusecmd(std::string variableName)
  : lilypondelement("")
{ fVariableName = variableName; }
lilypondvariableusecmd::~lilypondvariableusecmd() {}

ostream& operator<< (ostream& os, const Slilypondvariableusecmd& nstf)
{
  nstf->printLilyPondCode(os);
  return os;
}

void lilypondvariableusecmd::printStructure(ostream& os)
{
  os << "lilypondvariableusecmd" << fVariableName << hdl;
}

void lilypondvariableusecmd::printLilyPondCode(ostream& os)
{
  hdl++;
  os << "\\" << fVariableName << hdl;
}


}
