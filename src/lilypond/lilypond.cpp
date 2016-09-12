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
#include <algorithm>

#include "lilypond.h"

using namespace std;

namespace MusicXML2 
{

//______________________________________________________________________________
Slilypondparam lilypondparam::create(string value, bool quote) 
{ 
  lilypondparam * o = new lilypondparam(value, quote); assert(o!=0); 
  return o;
}
Slilypondparam lilypondparam::create(long value, bool quote) 
{
 lilypondparam * o = new lilypondparam(value, quote); assert(o!=0);
 return o; 
}

lilypondparam::lilypondparam(string value, bool quote) : 
    fValue(value), fQuote(quote) 
  {}
lilypondparam::lilypondparam(long value, bool quote) 
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

void lilypondparam::set (long value, bool quote)
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

long lilypondelement::addParam (Slilypondparam& param) { 
//  bool doDebug = fDebug;
  bool doDebug = false;

 if (doDebug) cout << "addParam & : ";
  fParams.push_back(param);
  if (doDebug) cout << param << std::endl; //dumpParams();
  return fParams.size()-1;
}

long lilypondelement::addParam (Slilypondparam param) { 
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

long lilypondelement::addSubElement (Slilypondelement& elt) {
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
Slilypondnote lilypondnote::create(unsigned short voice) 
{
  lilypondnotestatus * status = lilypondnotestatus::get(voice);
  lilypondnote * o = new lilypondnote (
    voice,"", status->fOctave, status->fDur, ""); 
  assert(o!=0); 
  return o;
}
Slilypondnote lilypondnote::create(
  unsigned short voice, 
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
lilypondnote::~lilypondnote() {}

void lilypondnote::set (
  unsigned short voice, 
  string name, 
  char octave, 
  lilypondnoteduration& dur, 
  string acc)
{
  lilypondnotestatus * status = lilypondnotestatus::get(voice);
  stringstream s;
  long dots = dur.fDots;
    
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
      /*
      if (!status) {
        // USER s << (int)octave;
        s << lilypondnote::octaveRepresentation(octave);
      } else if (status->fOctave != octave) {
        // USER s << (int)octave;
        s << lilypondnote::octaveRepresentation(octave);
        status->fOctave = octave;
      }
      */
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

void lilypondnote::print(ostream& os)
{
  lilypondelement::print(os);
}

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

void lilypondnotestatus::resetall ()
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
Slilypondseq lilypondseq::create()
{
  lilypondseq* o = new lilypondseq(); assert(o!=0);
  return o;
}

lilypondseq::lilypondseq() : lilypondelement("")
{
  fStartList=""; // """{"; // USER
  fEndList=""; // " }"; 
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
    os << (*i) << std::endl;
  } // for
}

//______________________________________________________________________________
Slilypondchord lilypondchord::create()
{
  lilypondchord* o = new lilypondchord(); assert(o!=0);
  return o;
}

lilypondchord::lilypondchord () : lilypondelement("") 
{ 
  fStartList=""; // USER ???
  fEndList="";
}
lilypondchord::~lilypondchord() {}

void lilypondchord::print(ostream& os)
{
  lilypondelement::print(os);
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
  lilypondelement::print(os);
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

void lilypondwedge::print(ostream& os)
{
  lilypondelement::print(os);
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
  os <<
    fName << " = \\lyricmode {" << std::endl <<
    fContents << std::endl <<
    "}" << std::endl << std::endl;
}

//______________________________________________________________________________
Slilypondpart lilypondpart::create(std::string name, bool absoluteCode)
{
  lilypondpart* o = new lilypondpart(name, absoluteCode); assert(o!=0);
  return o;
}

lilypondpart::lilypondpart(std::string name, bool absoluteCode)
  : lilypondelement("")
{
  fPartName = name;
  fPartAbsoluteCode = absoluteCode;
  
  // create the implicit lilypondseq element
  fPartLilypondseq = lilypondseq::create();
}
lilypondpart::~lilypondpart() {}

void lilypondpart::print(ostream& os)
{
  /*
  lilypondelement cmd = lilypondcmd::create("set Staff.instrumentName ="); // USER
  stringstream s1, s2;
  string instr = header.fPartName->getValue();
  */
  
  os << fPartName << " = ";
  if (!fPartAbsoluteCode) os << "\\relative ";
  os <<
    "{" << std::endl <<
    "% instrumentName" << std::endl <<
    fPartLilypondseq << std::endl <<
    "}" << std::endl;
  
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
  os << "\\paper {" << std::endl;
  os << "}" << std::endl;
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
  os << "\\header {" << std::endl;
  
  if (fScorePartwise) {
    os << "  %MusicXMl_version = " << fScorePartwise->getAttributeValue("version") << std::endl;
  }
  
  if (fWorkNumber) {
    os << "  %work_number = " << fWorkNumber->getValue() << std::endl;
  }
  
  if (fWorkTitle) {
     /*
      string workTitle = fWorkTitle->getValue();
      unsigned int pos = workTitle.find ('"');
      while (pos != string::npos) {
        workTitle = workTitle.replace (pos, 1, "'");
        pos = workTitle.find ('"', pos);
      }
    */
   os << "  %work_title = " << fWorkTitle->getValue() << std::endl;
  }
    
  if (fMovementNumber) {
    os << "  %movement_number = " << fMovementNumber->getValue() << std::endl;
  }
    
  if (fMovementTitle) {
    /*
      string movementTitle = fMovementTitle->getValue();
      unsigned int pos = movementTitle.find ('"');
      while (pos != string::npos) {
        movementTitle = movementTitle.replace (pos, 1, "'");
        pos = movementTitle.find ('"', pos);
      }
  */
    os << "  %movement_title = " << fMovementTitle->getValue() << std::endl;
  }
    
  if (!fCreators.empty()) {
    vector<S_creator>::const_iterator i1;
    for (i1=fCreators.begin(); i1!=fCreators.end(); i1++) {
      string type = (*i1)->getAttributeValue("type");
      std::transform(type.begin(), type.end(), type.begin(), ::tolower);
      if (type == "composer")
        os << "  " << type << " = " << (*i1)->getValue() << std::endl;
      else
        os << "  %" << type << " = " << (*i1)->getValue() << std::endl;
    } // for
  }
    
  if (fRights) {
    os << "  %rights = " << fRights->getValue() << std::endl;
  }
    
  if (!fSoftwares.empty()) {
    vector<S_software>::const_iterator i2;
    for (i2=fSoftwares.begin(); i2!=fSoftwares.end(); i2++) {
     os << "  %software = " << (*i2)->getValue() << std::endl;
    } // for
  }
    
  if (fEncodingDate) {
    os << "  %encoding_date = " << fEncodingDate->getValue() << std::endl;
  }
  
  os << "}" << std::endl; 
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
  os << "\\layout {" << std::endl;
  os << "}" << std::endl;
}


}
