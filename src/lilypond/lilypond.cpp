/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#include "lilypond.h"

using namespace std;

namespace MusicXML2 
{

//______________________________________________________________________________
std::string int2EnglishWord (int n) {
  if (n < 1) return "<1";
  if (n > 99) return ">99";
  
  std::stringstream s;

  switch (n) {
    case 1:
      s << "One";
      break;
    case 2:
      s << "Two";
      break;
    case 3:
      s << "Three";
      break;
    case 4:
      s << "Four";
      break;
    case 5:
      s << "Five";
      break;
    case 6:
      s << "Six";
      break;
    case 7:
      s << "Seven";
      break;
    case 8:
      s << "Eight";
      break;
    case 9:
      s << "Nine";
      break;
   case 10:
      s << "Ten";
      break;
    case 11:
      s << "Eleven";
      break;
    case 12:
      s << "Twelve";
      break;
    case 13:
      s << "Thirteen";
      break;
    case 14:
      s << "Fourteen";
      break;
    case 15:
      s << "Fifteen";
      break;
    case 16:
      s << "Sixteen";
      break;
    case 17:
      s << "Seventeen";
      break;
    case 18:
      s << "Eighteen";
      break;
    case 19:
      s << "Nineteen";
      break;
      
    default: {
      // n >= 20
      int digit1 = n / 10;
      int digit2 = n % 10;
      
      switch (digit1) {
        case 2:
          s << "Twenty";
          break;
        case 3:
          s << "Thirty";
          break;
        case 4:
          s << "Fourty";
          break;
        case 5:
          s << "Fifty";
          break;
        case 6:
          s << "Sixty";
          break;
        case 7:
          s << "Seventy";
          break;
        case 8:
          s << "Eighty";
          break;
        case 9:
          s << "Ninety";
          break;
      } // switch
      s << int2EnglishWord(digit2);
    } // default
  } // switch

  std::string result;
  
  s >> result;
  return result;
}

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
Slilypondelement lilypondelement::create(string name, string sep) 
{
  lilypondelement * o = new lilypondelement(name, sep); assert(o!=0);
  return o; 
}

lilypondelement::lilypondelement(string name, string sep) : fName(name), fSep(sep)
  {}
lilypondelement::~lilypondelement() {}

long lilypondelement::add (Slilypondelement& elt) { 
  fElements.push_back(elt); 
  return fElements.size()-1;
}
long lilypondelement::add (Slilypondparam& param) { 
  fParams.push_back(param); 
  return fParams.size()-1;
}
long lilypondelement::add (Slilypondparam param) { 
  fParams.push_back(param); 
  return fParams.size()-1;
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
  if (!fElements.empty()) {
    os << fStartList;
    vector<Slilypondelement>::const_iterator ielt;
    for (ielt = fElements.begin(); ielt != fElements.end(); ielt++) {
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
    //if (ielt = fElements.end()) 
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

//______________________________________________________________________________
Slilypondwedge lilypondwedge::create(WedgeKind kind)
{
  lilypondwedge* o = new lilypondwedge(kind); assert(o!=0);
  return o;
}

lilypondwedge::lilypondwedge(WedgeKind kind) : lilypondelement("")
{
  fWedgeKind=kind; 
}
lilypondwedge::~lilypondwedge() {}


}
