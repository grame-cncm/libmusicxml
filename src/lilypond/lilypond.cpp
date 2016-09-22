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
Slilypondparam lilypondparam::create(string value, bool quote) 
{ 
  lilypondparam * o = new lilypondparam(value, quote); assert(o!=0); 
  return o;
}
Slilypondparam lilypondparam::create(int value, bool quote) 
{
 lilypondparam * o = new lilypondparam(value, quote); assert(o!=0);
 return o; 
}

lilypondparam::lilypondparam(string value, bool quote) : 
    fValue(value), fQuote(quote) 
  {}
lilypondparam::lilypondparam(int value, bool quote) 
{
  set(value, quote);
}
lilypondparam::~lilypondparam () 
  {}

void lilypondparam::set (string value, bool quote)
{
  fValue = value;
  fQuote = quote;
}

void lilypondparam::set (int value, bool quote)
{
  stringstream s;
  s << value;
  s >> fValue;
  fQuote = quote;
}

//______________________________________________________________________________
void lilypondparam::print(ostream& os)
{
  if (fQuote) os << "\"";
  os << fValue;
  if (fQuote) os << "\"";
}
ostream& operator<< (ostream& os, const Slilypondparam& param)
{
  param->print(os);
  return os;
}

//______________________________________________________________________________
Slilypondelement lilypondelement::create(string name, string sep) 
{
  lilypondelement * o = new lilypondelement(name, sep); assert(o!=0);
  return o; 
}

lilypondelement::lilypondelement(string name, string sep) :
  fName(name), fSep(sep), fDebug(true)
  {}
lilypondelement::~lilypondelement() {}

void lilypondelement::dumpParams() {
  // print the optional parameters section
  if (!fParams.empty()) {
    cout << std::endl << "<========================" << std::endl;
    vector<Slilypondparam>::const_iterator param;
    for (param = fParams.begin(); param != fParams.end(); ) {
      cout << " ";
      if ((*param)->quote())
        cout << "\"" << (*param)->get() << "\"";
      else
        cout << (*param)->get();
      if (++param != fParams.end())
        cout << ", PARAMS ";
    } // for
    cout << "========================>" << std::endl;
  } // if
}

int lilypondelement::addParam (Slilypondparam& param) { 
//  bool doDebug = fDebug;
  bool doDebug = false;

 if (doDebug) cout << "addParam & : ";
  fParams.push_back(param);
  if (doDebug) cout << param << std::endl; //dumpParams();
  return fParams.size()-1;
}

int lilypondelement::addParam (Slilypondparam param) { 
//  bool doDebug = fDebug;
  bool doDebug = false;

  if (doDebug) cout << "addParam : ";
  fParams.push_back(param);
  if (doDebug) cout << param << std::endl; //dumpParams();
  return fParams.size()-1;
}

void lilypondelement::dumpElements() {
  /*
  bool isSeq    = dynamic_cast<const lilypondseq *>(this) != 0;
  bool isChord  = dynamic_cast<const lilypondchord *>(this) != 0;
  */
  
  // print the optional contained elements
  if (!fSubElements.empty()) {
    cout << std::endl << "<-------------------------" << std::endl;
    //cout << fStartList;
    vector<Slilypondelement>::const_iterator ielt;
    for (ielt = fSubElements.begin(); ielt != fSubElements.end(); ielt++) {
      /*
      bool prevNote = false;
      bool prevSeq  = false;

      Slilypondseq seq;
      seq.cast((lilypondelement *)(*ielt));
      Slilypondnote note;
      note.cast((lilypondelement *)(*ielt));

      if (isChord) {
        if (note) {
          // USER cout << (prevNote ? ", CHORD " : " ");
          cout << " ";
          prevNote = true;
        }
        else if (seq) {
          // USER cout << (prevSeq ? ", SEQ " : " ");
          cout << std::endl;
          prevSeq = true;
        }
      } // isChord
      //if (ielt = fSubElements.end()) 
      */
    cout << *ielt << " ";            
    } // for
  //cout << fEndList;
  cout << "------------------------->" << std::endl;
  } // if
  // if (isSeq) cout << std::endl;
}

int lilypondelement::addSubElement (Slilypondelement& elt) {
  bool doDebug = fDebug;
//  bool doDebug = false;

  if (doDebug) cout << "addSubElement : ";
  fSubElements.push_back(elt); 
  if (doDebug) cout << elt << std::endl; //dumpElements();
  return fSubElements.size()-1;
}

//______________________________________________________________________________
/*
 * The method that writes LilyPond code for the given element to the output stream
*/
void lilypondelement::print(ostream& os)
{
  // print the name
  os << fName;

  // print the optional parameters section
  if (!fParams.empty()) {
    /*
    vector<Slilypondparam>::const_iterator param;
    for (param = fParams.begin(); param != fParams.end(); ) {
      os << " ";
      if ((*param)->quote())
        os << "\"" << (*param)->get() << "\"";
      else
        os << (*param)->get();
      if (++param != fParams.end())
        os << ", PARAMS ";
    } // for
    os << "\n"; // USER
    */
  }

  bool isSeq    = dynamic_cast<const lilypondseq *>(this) != 0;
  bool isChord  = dynamic_cast<const lilypondchord *>(this) != 0;
  
  // print the optional contained elements
  if (!fSubElements.empty()) {
    /*
    os << fStartList;
    vector<Slilypondelement>::const_iterator ielt;
    for (ielt = fSubElements.begin(); ielt != fSubElements.end(); ielt++) {
      bool prevNote = false;
      bool prevSeq  = false;

      Slilypondseq seq;
      seq.cast((lilypondelement *)(*ielt));
      Slilypondnote note;
      note.cast((lilypondelement *)(*ielt));

      if (isChord) {
        if (note) {
          // USER os << (prevNote ? ", CHORD " : " ");
          os << " ";
          prevNote = true;
        }
        else if (seq) {
          // USER os << (prevSeq ? ", SEQ " : " ");
          os << std::endl;
          prevSeq = true;
        }
      } // isChord
    //if (ielt = fSubElements.end()) 
    os << *ielt << " ";            
    } // for
  os << fEndList;
  */
  } // if
  //if (isSeq) os << std::endl;
  
  /*
   *   if (fPendingWedges.size()) {
    while (fPendingWedges.size()) {
      cout << "% FOO ";
      Slilypondwedge wedg = fPendingWedges.front();
      if (wedg) {
        switch (wedg->getWedgeKind()) {
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
      } // while
      fPendingWedges.pop_front();
    }
    cout << endl;
  }
*/
}

//______________________________________________________________________________
ostream& operator<< (ostream& os, const Slilypondelement& elt)
{
  elt->print(os);
  return os;
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
  dur->print(os);
  return os;
}

void lilypondnoteduration::print(ostream& os)
{
  // divisions are per quater, Lilypond durations are in whole notes
  //os << "|"  << fLilypondnoteduration.fNum << "|" << fLilypondnoteduration.fDenom;

  int noteDivisions         = fNum;
  int divisionsPerWholeNote = fDenom ;
  
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
Slilypondnote lilypondnote::create()// JMI  Note note, int voice) 
{
  // JMI lilypondnotestatus status = lilypondnotestatus::get(voice);
  
  int voice=-17;
  
  lilypondnote * o = new lilypondnote ();// JMI note, voice);
 //   voice,"", status->fOctave, status->fDur, ""); 
  assert(o!=0); 
  
  return o;
}

lilypondnote::lilypondnote() : lilypondelement("")
{
  fDiatonicNote         = lilypondnote::kNoDiatonicNote;
  fAlteration           = lilypondnote::kNoAlteration;
  fOctave               = -1;
  fLilypondnoteduration = lilypondnoteduration::create(0, 0, 0);
  fDotsNumber           = 0;
  fVoice                = -1;
}
lilypondnote::~lilypondnote() {}

void lilypondnote::updateNote(
    bool                  currentStepIsRest,
    DiatonicNote          diatonicNote,
    Alteration            alteration,
    int                   octave,
    int                   dotsNumber,
    Slilypondnoteduration dur,
    LilypondNote          lilypondNote,
    int                   voice,
    bool                  noteBelongsToAChord)
{
  fCurrentStepIsRest = currentStepIsRest;
  fDiatonicNote = diatonicNote;
  fAlteration = alteration;
  fOctave = octave;
  fDotsNumber = dotsNumber;
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
  elt->print(os);
  return os;
}

void lilypondnote::print(ostream& os)
{
  /*
  os <<
    "note(fDiatonicNote = " << fDiatonicNote << ", " <<
    "fAlteration = " << fAlteration << ", " << ", " <<
    "fOctave = " << fOctave << ", " << ", " <<
    "fVoice = " << fVoice << ", " << ", " <<
    std::endl;
 */

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
Slilypondnote lilypondnote::create(
  unsigned short voice
  string name,
  char oct, 
  lilypondnoteduration& dur, string acc)
{
  lilypondnote * o = 
    new lilypondnote (voice, name, oct, dur, acc); assert(o!=0);
  return o;
}

lilypondnote::lilypondnote(
  unsigned short voice, 
  string name, 
  char octave, 
  lilypondnoteduration& dur, string acc) : 
    lilypondelement(""), fDuration(1,4) 
{
  set(voice, name, octave, dur, acc); 
}

void lilypondnote::set (
  unsigned short voice, 
  string name, 
  char octave, 
  lilypondnoteduration& dur, 
  string acc)
{
  lilypondnotestatus * status = lilypondnotestatus::get(voice);
  stringstream s;
  int dots = dur.fDots;
    
  fNote       = name;
  fAccidental = acc;
  fOctave     = octave;
  fDuration   = dur;
  
  s << 
  //"\%\{ octave = " << octave << "\%\} " <<
  name;
  // octave and accidentals are ignored in case of rests
  if (name[0] != 'r' && name[0] != 'R') {
    if (!acc.empty())
      s << acc;
    if (name != "empty" || octave != 0) { // JMI USER
      s << lilypondnote::octaveRepresentation(octave); // clean absolute/relative later
      / *
      if (!status) {
        // USER s << (int)octave;
        s << lilypondnote::octaveRepresentation(octave);
      } else if (status->fOctave != octave) {
        // USER s << (int)octave;
        s << lilypondnote::octaveRepresentation(octave);
        status->fOctave = octave;
      }
      * /
    } // name != "empty"
  } // non rest
  
  if (!status || (*status != dur)) {
    if (dur.fNum != 1) {
      s << "*" << (int)dur.fNum;
    }
  s << 
  //USER "/" << 
  (int)dur.fDenom;
  if (status) *status = dur;
  }
  
  while (dots-- > 0)
    s << ".";
  s >> fName;
}

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
lilypondnotestatus* lilypondnotestatus::fInstances[kMaxInstances] = { 0 };

lilypondnotestatus* lilypondnotestatus::get (unsigned short voice)
{ 
  if (voice < kMaxInstances) {
    if (!fInstances[voice])
      fInstances[voice] = new lilypondnotestatus; 
    return fInstances[voice];
  }
  return 0;
}

void lilypondnotestatus::resetalllilypondnotestatus ()
{ 
  for (int i=0; i<kMaxInstances; i++) {
    if (fInstances[i]) fInstances[i]->reset();
  }
}

void lilypondnotestatus::freeall ()
{ 
  for (int i=0; i<kMaxInstances; i++) {
    delete fInstances[i];
  fInstances[i] = 0;
  }
}

//______________________________________________________________________________
Slilypondseq lilypondseq::create(ElementsSeparator elementsSeparator)
{
  lilypondseq* o = new lilypondseq(elementsSeparator); assert(o!=0);
  return o;
}

lilypondseq::lilypondseq(ElementsSeparator elementsSeparator) : lilypondelement("")
{
  fElementsSeparator=elementsSeparator;
}
lilypondseq::~lilypondseq() {}

ostream& operator<< (ostream& os, const Slilypondseq& elt)
{
  elt->print(os);
  return os;
}

void lilypondseq::print(ostream& os)
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
  chrd->print(os);
  return os;
}

void lilypondchord::print(ostream& os)
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
Slilypondcmd lilypondcmd::create(string name, BackSlashPrefix backslashPrefix)
{
  lilypondcmd* o = new lilypondcmd(name, backslashPrefix); assert(o!=0);
  return o;
}

lilypondcmd::lilypondcmd(string name, BackSlashPrefix backslashPrefix) : 
    lilypondelement(
      backslashPrefix == kWithBackslash
        ? "\\"+name
        : name
        )
{
  fStartList=""; // "{";  // USER 
  fEndList=""; // "}"; // USER
}
lilypondcmd::~lilypondcmd() {}

void lilypondcmd::print(ostream& os)
{
  // print the cmd name
  os << fName << " ";
 // jMI lilypondelement::print(os);
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
  elt->print(os);
  return os;
}
void lilypondbarline::print(ostream& os)
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
  elt->print(os);
  return os;
}
void lilypondcomment::print(ostream& os)
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
  elt->print(os);
  return os;
}
void lilypondbreak::print(ostream& os)
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
  dyn->print(os);
  return os;
}

void lilypondbeam::print(ostream& os)
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
  dyn->print(os);
  return os;
}

void lilyponddynamics::print(ostream& os)
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
  wdg->print(os);
  return os;
}

void lilypondwedge::print(ostream& os)
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
  fName = name;
  fContents=contents; 
}
lilypondlyrics::~lilypondlyrics() {}

void lilypondlyrics::print(ostream& os)
{  
  hdl++;
  os << fName << " = \\lyricmode {" << hdl;
  hdl--;
  os << fContents << hdl;
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
  
  // create the implicit lilypondseq element
  fPartLilypondseq = lilypondseq::create(lilypondseq::kSpace);
  
  // add the default 4/4 time signature
  Slilypondtime time = lilypondtime::create(4, 4, fGenerateNumericalTime);
  Slilypondelement t = time;
  fPartLilypondseq->addElementToSequence (t);

}
lilypondpart::~lilypondpart() {}

void lilypondpart::print(ostream& os)
{
  /*
  lilypondelement cmd = lilypondcmd::create("set Staff.instrumentName ="); // USER
  stringstream s1, s2;
  string instr = header.fPartName->getValue();
  */
  
  hdl++;
  os << fPartName << " = ";
  if (!fPartAbsoluteCode) os << "\\relative ";
  os << "{" << hdl << fPartLilypondseq << hdl;
  hdl--;
  os << "}" << hdl;
  
  //lilypondelement::print(os);
}

  /*

  // print the optional parameters section
  if (!fParams.empty()) {
    vector<Slilypondparam>::const_iterator param;
    for (param = fParams.begin(); param != fParams.end(); ) {
      os << " ";
      if ((*param)->quote())
        os << "\"" << (*param)->get() << "\"";
      else
        os << (*param)->get();
      if (++param != fParams.end())
        os << ", PARAMS ";
    } // for
    os << "\n"; // USER
  }

  bool isSeq    = dynamic_cast<const lilypondseq *>(this) != 0;
  bool isChord  = dynamic_cast<const lilypondchord *>(this) != 0;
  
  // print the optional contained elements
  if (!fSubElements.empty()) {
    os << fStartList;
    vector<Slilypondelement>::const_iterator ielt;
    for (ielt = fSubElements.begin(); ielt != fSubElements.end(); ielt++) {
      bool prevNote = false;
      bool prevSeq  = false;

      Slilypondseq seq;
      seq.cast((lilypondelement *)(*ielt));
      Slilypondnote note;
      note.cast((lilypondelement *)(*ielt));

      if (isChord) {
        if (note) {
          // USER os << (prevNote ? ", CHORD " : " ");
          os << " ";
          prevNote = true;
        }
        else if (seq) {
          // USER os << (prevSeq ? ", SEQ " : " ");
          os << std::endl;
          prevSeq = true;
        }
      } // isChord
    //if (ielt = fSubElements.end()) 
    os << *ielt << " ";            
    } // for
  os << fEndList;
  } // if
  if (isSeq) os << std::endl;
  */

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

void lilypondpaper::print(ostream& os)
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

void lilypondheader::print(ostream& os)
{
  /*
    S_score_partwise        fScorePartwise; // may contain MusicXML version
    S_work_number           fWorkNumber;
    S_work_title            fWorkTitle;
    S_movement_number       fMovementNumber;
    S_movement_title        fMovementTitle;
    std::vector<S_creator>  fCreators;
    S_rights                fRights;
    std::vector<S_software> fSoftwares;
    S_encoding_date         fEncodingDate;
    S_score_instrument      fScoreInstrument;
*/
  
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

void lilypondvariablevalueassociation::print(ostream& os)
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

void lilypondschemevariablevalueassociation::print(ostream& os)
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

void lilypondglobalsettings::print(ostream& os)
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

void lilypondschemeglobalsettings::print(ostream& os)
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
{
}
lilypondlayout::~lilypondlayout() {}

void lilypondlayout::print(ostream& os)
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

void lilypondtime::print(ostream& os)
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

void lilypondclef::print(ostream& os)
{
  os << fName << "\\clef \"" << fClefName << "\"" << hdl;
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
  key->print(os);
  return os;
}
void lilypondkey::print(ostream& os)
{
  os << "\\key " << fTonicNote << " ";
  if (fKeyMode == kMajor) os << "\"major\"";
  else os << "\"minor\"";
  os << hdl;
}


}
