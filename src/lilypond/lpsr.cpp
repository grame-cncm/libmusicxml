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


//_______________________________________________________________________________
S_lpsrOptions lpsrOptions::create()
{
  lpsrOptions* o = new lpsrOptions();
  assert(o!=0);
  return o;
}

lpsrOptions::lpsrOptions() {}
lpsrOptions::~lpsrOptions() {}

//_______________________________________________________________________________
S_lpsrElement lpsrElement::create (
  S_msrOptions   msrOpts,
  S_lpsrOptions& lpsrOpts, 
  int            inputLineNumber)
{
  lpsrElement* o =
    new lpsrElement (
      msrOpts, lpsrOpts, inputLineNumber);
  assert(o!=0);
  return o;
}

lpsrElement::lpsrElement (
  S_msrOptions   msrOpts,
  S_lpsrOptions& lpsrOpts, 
  int            inputLineNumber)
    : msrElement (msrOpts, inputLineNumber)
{
  fLpsrOptions = lpsrOpts;
}

lpsrElement::~lpsrElement() {}

void lpsrElement::acceptIn (basevisitor* v) {
  if (fMsrOptions->fDebugDebug)
    cerr << idtr <<
      "==> lpsrElement::acceptIn()" << endl;
      
  if (visitor<S_lpsrElement>*
    p =
      dynamic_cast<visitor<S_lpsrElement>*> (v)) {
        S_lpsrElement elem = this;
        
        if (fMsrOptions->fDebug)
          cerr << idtr <<
            "==> Launching lpsrElement::visitStart()" << endl;
        p->visitStart (elem);
  }
}

void lpsrElement::acceptOut (basevisitor* v) {
  if (fMsrOptions->fDebugDebug)
    cerr << idtr <<
      "==> lpsrElement::acceptOut()" << endl;

  if (visitor<S_lpsrElement>*
    p =
      dynamic_cast<visitor<S_lpsrElement>*> (v)) {
        S_lpsrElement elem = this;
      
        if (fMsrOptions->fDebug)
          cerr << idtr <<
            "==> Launching lpsrElement::visitEnd()" << endl;
        p->visitEnd (elem);
  }
}

void lpsrElement::browseData (basevisitor* v)
{}

//______________________________________________________________________________
/* JMI
S_msrAbsoluteOctave msrAbsoluteOctave::create (
  S_msrOptions&     msrOpts, 
  S_lpsrOptions&    lpsrOpts, 
  int                    inputLineNumber,
  int                    musicxmlOctave)
{
  msrAbsoluteOctave * o =
    new msrAbsoluteOctave (
      msrOpts, lpsrOpts, inputLineNumber, musicxmlOctave);
  assert(o!=0); 
  return o;
}

msrAbsoluteOctave::msrAbsoluteOctave (
  S_msrOptions&     msrOpts, 
  S_lpsrOptions&    lpsrOpts, 
  int                    inputLineNumber,
  int                    musicxmlOctave)
    : msrElement (lpsrOpts, inputLineNumber)
{
  fMsrOctave = musicxmlOctave - 3;
  / *
  cerr <<
    "msrAbsoluteOctave::msrAbsoluteOctave (), musicxmlOctave = " << musicxmlOctave << 
    ", fMsrOctave = " << fMsrOctave << endl;
    * /
}
msrAbsoluteOctave::~msrAbsoluteOctave () {}


void msrAbsoluteOctave::acceptIn (basevisitor* v) {
  if (fTranslationSettings->fDebugDebug)
    cerr << idtr <<
      "==> msrAbsoluteOctave::acceptIn()" << endl;
      
  if (visitor<S_msrAbsoluteOctave>*
    p =
      dynamic_cast<visitor<S_msrAbsoluteOctave>*> (v)) {
        S_msrAbsoluteOctave elem = this;
        
        if (fTranslationSettings->fDebug)
          cerr << idtr <<
            "==> Launching msrAbsoluteOctave::visitStart()" << endl;
        p->visitStart (elem);
  }
}

void msrAbsoluteOctave::acceptOut (basevisitor* v) {
  if (fTranslationSettings->fDebugDebug)
    cerr << idtr <<
      "==> msrAbsoluteOctave::acceptOut()" << endl;

  if (visitor<S_msrAbsoluteOctave>*
    p =
      dynamic_cast<visitor<S_msrAbsoluteOctave>*> (v)) {
        S_msrAbsoluteOctave elem = this;
      
        if (fTranslationSettings->fDebug)
          cerr << idtr <<
            "==> Launching msrAbsoluteOctave::visitEnd()" << endl;
        p->visitEnd (elem);
  }
}

void msrAbsoluteOctave::browseData (basevisitor* v)
{}

ostream& operator<< (ostream& os, const S_msrAbsoluteOctave& dur)
{
  dur->print (os);
  return os;
}

string msrAbsoluteOctave::absoluteOctaveAsLilypondString ()
{
  stringstream s;  
  s << fMsrOctave;
  return s.str();
}

void msrAbsoluteOctave::print (ostream& os)
{
  this->printLilyPondCode (os);
}

void msrAbsoluteOctave::printLilyPondCode (ostream& os)
{
  / *
  enum NoteFigures = {
    k1024th, k512th, 256th, k128th, k64th, k32nd, k16th, 
    kEighth, kQuarter, kHalf, kWhole, kBreve, kLong, kMaxima};
* /

  os << absoluteOctaveAsLilypondString ();
}
*/

//______________________________________________________________________________
/*
S_lpsrSequence lpsrSequence::create (
  S_msrOptions&     msrOpts, 
  S_lpsrOptions&    lpsrOpts, 
  int                    inputLineNumber,
  lpsrElementsSeparator      elementsSeparator)
{
  lpsrSequence* o =
    new lpsrSequence (
      msrOpts, lpsrOpts, elementsSeparator);
  assert(o!=0);
  return o;
}

lpsrSequence::lpsrSequence (lpsrElementsSeparator elementsSeparator)
    : msrElement (lpsrOpts, inputLineNumber)
{
  fElementsSeparator=elementsSeparator;
}
lpsrSequence::~lpsrSequence() {}

void lpsrSequence::acceptIn (basevisitor* v) {
  if (fMsrOptions->fDebugDebug)
    cerr << idtr <<
      "==> lpsrSequence::acceptIn()" << endl;
      
  if (visitor<S_lpsrSequence>*
    p =
      dynamic_cast<visitor<S_lpsrSequence>*> (v)) {
        S_lpsrSequence elem = this;
        
        if (fMsrOptions->fDebug)
          cerr << idtr <<
            "==> Launching lpsrSequence::visitStart()" << endl;
        p->visitStart (elem);
  }
}

void lpsrSequence::acceptOut (basevisitor* v) {
  if (fMsrOptions->fDebugDebug)
    cerr << idtr <<
      "==> lpsrSequence::acceptOut()" << endl;

  if (visitor<S_lpsrSequence>*
    p =
      dynamic_cast<visitor<S_lpsrSequence>*> (v)) {
        S_lpsrSequence elem = this;
      
        if (fMsrOptions->fDebug)
          cerr << idtr <<
            "==> Launching lpsrSequence::visitEnd()" << endl;
        p->visitEnd (elem);
  }
}

void lpsrSequence::browseData (basevisitor* v)
{}

ostream& operator<< (ostream& os, const S_lpsrSequence& elt)
{
  elt->print (os);
  return os;
}

void lpsrSequence::print (ostream& os)
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

*/

//______________________________________________________________________________
S_lpsrParallelMusic lpsrParallelMusic::create (
  S_msrOptions          msrOpts,
  S_lpsrOptions&        lpsrOpts, 
  int                   inputLineNumber,
  lpsrElementsSeparator elementsSeparator)
{
  lpsrParallelMusic* o =
    new lpsrParallelMusic (
      msrOpts, lpsrOpts, inputLineNumber, elementsSeparator);
  assert(o!=0);
  return o;
}

lpsrParallelMusic::lpsrParallelMusic (
  S_msrOptions          msrOpts,
  S_lpsrOptions&        lpsrOpts, 
  int                   inputLineNumber,
  lpsrElementsSeparator elementsSeparator)
    : lpsrElement (msrOpts, lpsrOpts, inputLineNumber)
{
  fElementsSeparator=elementsSeparator;
}

lpsrParallelMusic::~lpsrParallelMusic() {}

void lpsrParallelMusic::acceptIn (basevisitor* v) {
  if (fMsrOptions->fDebugDebug)
    cerr << idtr <<
      "==> lpsrParallelMusic::acceptIn()" << endl;
      
  if (visitor<S_lpsrParallelMusic>*
    p =
      dynamic_cast<visitor<S_lpsrParallelMusic>*> (v)) {
        S_lpsrParallelMusic elem = this;
        
        if (fMsrOptions->fDebug)
          cerr << idtr <<
            "==> Launching lpsrParallelMusic::visitStart()" << endl;
        p->visitStart (elem);
  }
}

void lpsrParallelMusic::acceptOut (basevisitor* v) {
  if (fMsrOptions->fDebugDebug)
    cerr << idtr <<
      "==> lpsrParallelMusic::acceptOut()" << endl;

  if (visitor<S_lpsrParallelMusic>*
    p =
      dynamic_cast<visitor<S_lpsrParallelMusic>*> (v)) {
        S_lpsrParallelMusic elem = this;
      
        if (fMsrOptions->fDebug)
          cerr << idtr <<
            "==> Launching lpsrParallelMusic::visitEnd()" << endl;
        p->visitEnd (elem);
  }
}

void lpsrParallelMusic::browseData (basevisitor* v)
{
  if (fMsrOptions->fDebug)
    cerr << idtr <<
      "==> lpsrParallelMusic::browseData()" << endl;
  
  for (
    vector<S_msrElement>::iterator i = fParallelMusicElements.begin();
    i != fParallelMusicElements.end();
    i++) {
    // create the element browser
    msrBrowser<msrElement> browser (v);
  
    // browse the element with the visitor
    browser.browse (*(*i));
  } // for

  if (fMsrOptions->fDebug)
    cerr << idtr <<
      "<== lpsrParallelMusic::browseData()" << endl;
}

ostream& operator<< (ostream& os, const S_lpsrParallelMusic& elt)
{
  elt->print (os);
  return os;
}

void lpsrParallelMusic::print (ostream& os)
{
  os << "ParallelMusic" << endl;
  
  idtr++;
  
  int size = fParallelMusicElements.size();
  
  for (int i = 0; i < size; i++ ) {
    os << idtr << fParallelMusicElements[i];
  } // for
  
  idtr--;
}

//______________________________________________________________________________
/*
S_lpsrBarLine lpsrBarLine::create (
  S_msrOptions&     msrOpts, 
  S_lpsrOptions&    lpsrOpts, 
  int                    inputLineNumber,
  int                    nextBarNumber)
{
  lpsrBarLine* o = new lpsrBarLine (nextBarNumber); assert(o!=0);
  return o;
}

lpsrBarLine::lpsrBarLine (int nextBarNumber)
    : lpsrElement (msrOpts, lpsrOpts, lpsrOpts, inputLineNumber)
{
  fNextBarNumber=nextBarNumber; 
}
lpsrBarLine::~lpsrBarLine() {}

ostream& operator<< (ostream& os, const S_lpsrBarLine& elt)
{
  elt->print (os);
  return os;
}

void lpsrBarLine::print (ostream& os)
{
  os << "BarLine" << " " << fNextBarNumber << endl;
}

*/

//______________________________________________________________________________
S_lpsrComment lpsrComment::create (
  S_msrOptions&     msrOpts, 
  S_lpsrOptions&    lpsrOpts, 
  int                    inputLineNumber,
  string                 contents,
  lpsrGapKind                gapKind)
{
  lpsrComment* o = new
    lpsrComment (
      msrOpts, lpsrOpts, inputLineNumber, contents, gapKind);
  assert(o!=0);
  return o;
}

lpsrComment::lpsrComment (
  S_msrOptions&     msrOpts, 
  S_lpsrOptions&    lpsrOpts, 
  int                    inputLineNumber,
  string                 contents,
  lpsrGapKind                gapKind)
    : lpsrElement (msrOpts, lpsrOpts, inputLineNumber)
{
  fContents=contents;
  fGapKind=gapKind;
}
lpsrComment::~lpsrComment() {}

void lpsrComment::acceptIn (basevisitor* v) {
  if (fMsrOptions->fDebugDebug)
    cerr << idtr <<
      "==> lpsrComment::acceptIn()" << endl;
      
  if (visitor<S_lpsrComment>*
    p =
      dynamic_cast<visitor<S_lpsrComment>*> (v)) {
        S_lpsrComment elem = this;
        
        if (fMsrOptions->fDebug)
          cerr << idtr <<
            "==> Launching lpsrComment::visitStart()" << endl;
        p->visitStart (elem);
  }
}

void lpsrComment::acceptOut (basevisitor* v) {
  if (fMsrOptions->fDebugDebug)
    cerr << idtr <<
      "==> lpsrComment::acceptOut()" << endl;

  if (visitor<S_lpsrComment>*
    p =
      dynamic_cast<visitor<S_lpsrComment>*> (v)) {
        S_lpsrComment elem = this;
      
        if (fMsrOptions->fDebug)
          cerr << idtr <<
            "==> Launching lpsrComment::visitEnd()" << endl;
        p->visitEnd (elem);
  }
}

void lpsrComment::browseData (basevisitor* v)
{}

ostream& operator<< (ostream& os, const S_lpsrComment& elt)
{
  elt->print (os);
  return os;
}

void lpsrComment::print (ostream& os)
{
  os << "Comment" << endl;
  idtr++;
  os << idtr << "% " << fContents << endl;
  if (fGapKind == kGapAfterwards) os << idtr << endl;
  idtr--;
}

//______________________________________________________________________________
S_lpsrBarNumberCheck lpsrBarNumberCheck::create (
  S_msrOptions&     msrOpts, 
  S_lpsrOptions&    lpsrOpts, 
  int                    inputLineNumber,
  int                    nextBarNumber)
{
  lpsrBarNumberCheck* o =
    new lpsrBarNumberCheck (
      msrOpts, lpsrOpts, inputLineNumber, nextBarNumber);
  assert(o!=0);
  return o;
}

lpsrBarNumberCheck::lpsrBarNumberCheck (
  S_msrOptions&     msrOpts, 
  S_lpsrOptions&    lpsrOpts, 
  int                    inputLineNumber,
  int                    nextBarNumber)
    : lpsrElement (msrOpts, lpsrOpts, inputLineNumber)
{
  fNextBarNumber=nextBarNumber; 
}
lpsrBarNumberCheck::~lpsrBarNumberCheck() {}

void lpsrBarNumberCheck::acceptIn (basevisitor* v) {
  if (fMsrOptions->fDebugDebug)
    cerr << idtr <<
      "==> lpsrBarNumberCheck::acceptIn()" << endl;
      
  if (visitor<S_lpsrBarNumberCheck>*
    p =
      dynamic_cast<visitor<S_lpsrBarNumberCheck>*> (v)) {
        S_lpsrBarNumberCheck elem = this;
        
        if (fMsrOptions->fDebug)
          cerr << idtr <<
            "==> Launching lpsrBarNumberCheck::visitStart()" << endl;
        p->visitStart (elem);
  }
}

void lpsrBarNumberCheck::acceptOut (basevisitor* v) {
  if (fMsrOptions->fDebugDebug)
    cerr << idtr <<
      "==> lpsrBarNumberCheck::acceptOut()" << endl;

  if (visitor<S_lpsrBarNumberCheck>*
    p =
      dynamic_cast<visitor<S_lpsrBarNumberCheck>*> (v)) {
        S_lpsrBarNumberCheck elem = this;
      
        if (fMsrOptions->fDebug)
          cerr << idtr <<
            "==> Launching lpsrBarNumberCheck::visitEnd()" << endl;
        p->visitEnd (elem);
  }
}

void lpsrBarNumberCheck::browseData (basevisitor* v)
{}

ostream& operator<< (ostream& os, const S_lpsrBarNumberCheck& elt)
{
  elt->print (os);
  return os;
}

void lpsrBarNumberCheck::print (ostream& os)
{
  os << "BarNumberCheck" << " " << fNextBarNumber << endl;
}

//______________________________________________________________________________
S_lpsrUseVoiceCommand lpsrUseVoiceCommand::create (
  S_msrOptions&  msrOpts, 
  S_lpsrOptions& lpsrOpts, 
  int            inputLineNumber,
  S_msrVoice     voice)
{
  lpsrUseVoiceCommand* o =
    new lpsrUseVoiceCommand (
      msrOpts, lpsrOpts, inputLineNumber,
      voice);
  assert(o!=0);
  return o;
}

lpsrUseVoiceCommand::lpsrUseVoiceCommand (
  S_msrOptions&  msrOpts, 
  S_lpsrOptions& lpsrOpts, 
  int            inputLineNumber,
  S_msrVoice     voice)
    : lpsrElement (msrOpts, lpsrOpts, inputLineNumber)
{
  fVoice  = voice;
}

lpsrUseVoiceCommand::~lpsrUseVoiceCommand() {}

void lpsrUseVoiceCommand::acceptIn (basevisitor* v) {
  if (fMsrOptions->fDebugDebug)
    cerr << idtr <<
      "==> lpsrUseVoiceCommand::acceptIn()" << endl;
      
  if (visitor<S_lpsrUseVoiceCommand>*
    p =
      dynamic_cast<visitor<S_lpsrUseVoiceCommand>*> (v)) {
        S_lpsrUseVoiceCommand elem = this;
        
        if (fMsrOptions->fDebug)
          cerr << idtr <<
            "==> Launching lpsrUseVoiceCommand::visitStart()" << endl;
        p->visitStart (elem);
  }
}

void lpsrUseVoiceCommand::acceptOut (basevisitor* v) {
  if (fMsrOptions->fDebugDebug)
    cerr << idtr <<
      "==> lpsrUseVoiceCommand::acceptOut()" << endl;

  if (visitor<S_lpsrUseVoiceCommand>*
    p =
      dynamic_cast<visitor<S_lpsrUseVoiceCommand>*> (v)) {
        S_lpsrUseVoiceCommand elem = this;
      
        if (fMsrOptions->fDebug)
          cerr << idtr <<
            "==> Launching lpsrUseVoiceCommand::visitEnd()" << endl;
        p->visitEnd (elem);
  }
}

void lpsrUseVoiceCommand::browseData (basevisitor* v)
{}

ostream& operator<< (ostream& os, const S_lpsrUseVoiceCommand& nlc)
{
  nlc->print (os);
  return os;
}

void lpsrUseVoiceCommand::print (ostream& os)
{
  os <<
    "UseVoiceCommand" << " " <<
    fVoice->getVoiceName () << endl;
}

//______________________________________________________________________________
S_lpsrNewlyricsCommand lpsrNewlyricsCommand::create (
  S_msrOptions&  msrOpts, 
  S_lpsrOptions& lpsrOpts, 
  int            inputLineNumber,
  S_msrLyrics    lyrics,
  S_msrVoice     voice)
{
  lpsrNewlyricsCommand* o =
    new lpsrNewlyricsCommand (
      msrOpts, lpsrOpts, inputLineNumber,
      lyrics, voice);
  assert(o!=0);
  return o;
}

lpsrNewlyricsCommand::lpsrNewlyricsCommand (
  S_msrOptions&  msrOpts, 
  S_lpsrOptions& lpsrOpts, 
  int            inputLineNumber,
  S_msrLyrics    lyrics,
  S_msrVoice     voice)
    : lpsrElement (msrOpts, lpsrOpts, inputLineNumber)
{
  fLyrics = lyrics;
  fVoice  = voice;
}

lpsrNewlyricsCommand::~lpsrNewlyricsCommand() {}

void lpsrNewlyricsCommand::acceptIn (basevisitor* v) {
  if (fMsrOptions->fDebugDebug)
    cerr << idtr <<
      "==> lpsrNewlyricsCommand::acceptIn()" << endl;
      
  if (visitor<S_lpsrNewlyricsCommand>*
    p =
      dynamic_cast<visitor<S_lpsrNewlyricsCommand>*> (v)) {
        S_lpsrNewlyricsCommand elem = this;
        
        if (fMsrOptions->fDebug)
          cerr << idtr <<
            "==> Launching lpsrNewlyricsCommand::visitStart()" << endl;
        p->visitStart (elem);
  }
}

void lpsrNewlyricsCommand::acceptOut (basevisitor* v) {
  if (fMsrOptions->fDebugDebug)
    cerr << idtr <<
      "==> lpsrNewlyricsCommand::acceptOut()" << endl;

  if (visitor<S_lpsrNewlyricsCommand>*
    p =
      dynamic_cast<visitor<S_lpsrNewlyricsCommand>*> (v)) {
        S_lpsrNewlyricsCommand elem = this;
      
        if (fMsrOptions->fDebug)
          cerr << idtr <<
            "==> Launching lpsrNewlyricsCommand::visitEnd()" << endl;
        p->visitEnd (elem);
  }
}

void lpsrNewlyricsCommand::browseData (basevisitor* v)
{}

ostream& operator<< (ostream& os, const S_lpsrNewlyricsCommand& nlc)
{
  nlc->print (os);
  return os;
}

void lpsrNewlyricsCommand::print (ostream& os)
{
  os <<
    "NewlyricsCommand" << " " <<
    fLyrics->getLyricsName () << " " <<
    fVoice->getVoiceName () << endl;
}

//______________________________________________________________________________
S_lpsrVarValAssoc lpsrVarValAssoc::create (
  S_msrOptions&       msrOpts, 
  S_lpsrOptions&      lpsrOpts, 
  int                 inputLineNumber,
  lpsrCommentedKind   commentedKind,
  lpsrBackslashKind   backslashKind,
  string              variableName,
  lpsrVarValSeparator varValSeparator,
  lpsrQuotesKind      quotesKind,
  string              value, 
  string              unit,
  lpsrEndlKind        endlKind)
{
  lpsrVarValAssoc* o =
    new lpsrVarValAssoc(
      msrOpts, lpsrOpts, inputLineNumber,
      commentedKind,
      backslashKind,
      variableName,
      varValSeparator, 
      quotesKind,
      value,
      unit,
      endlKind);
  assert(o!=0);
  return o;
}

lpsrVarValAssoc::lpsrVarValAssoc (
  S_msrOptions&       msrOpts, 
  S_lpsrOptions&      lpsrOpts, 
  int                 inputLineNumber,
  lpsrCommentedKind   commentedKind,
  lpsrBackslashKind   backslashKind,
  string              variableName,
  lpsrVarValSeparator varValSeparator,
  lpsrQuotesKind      quotesKind,
  string              value, 
  string              unit,
  lpsrEndlKind        endlKind)
    : lpsrElement (msrOpts, lpsrOpts, inputLineNumber)
{
  fBackslashKind   = backslashKind;
  fVariableName    = variableName;
  fVarValSeparator = varValSeparator;
  fQuotesKind      = quotesKind;
  fVariableValue   = value;
  fCommentedKind   = commentedKind;
  fUnit            = unit;
  fEndlKind        = endlKind;
}

lpsrVarValAssoc::~lpsrVarValAssoc() {}

string const lpsrVarValAssoc::g_NoUnit = "";

void lpsrVarValAssoc::changeAssoc (string value) {
  fVariableValue=value;
}

void lpsrVarValAssoc::acceptIn (basevisitor* v) {
  if (fMsrOptions->fDebugDebug)
    cerr << idtr <<
      "==> lpsrVarValAssoc::acceptIn()" << endl;
      
  if (visitor<S_lpsrVarValAssoc>*
    p =
      dynamic_cast<visitor<S_lpsrVarValAssoc>*> (v)) {
        S_lpsrVarValAssoc elem = this;
        
        if (fMsrOptions->fDebug)
          cerr << idtr <<
            "==> Launching lpsrVarValAssoc::visitStart()" << endl;
        p->visitStart (elem);
  }
}

void lpsrVarValAssoc::acceptOut (basevisitor* v) {
  if (fMsrOptions->fDebugDebug)
    cerr << idtr <<
      "==> lpsrVarValAssoc::acceptOut()" << endl;

  if (visitor<S_lpsrVarValAssoc>*
    p =
      dynamic_cast<visitor<S_lpsrVarValAssoc>*> (v)) {
        S_lpsrVarValAssoc elem = this;
      
        if (fMsrOptions->fDebug)
          cerr << idtr <<
            "==> Launching lpsrVarValAssoc::visitEnd()" << endl;
        p->visitEnd (elem);
  }
}

void lpsrVarValAssoc::browseData (basevisitor* v)
{}

ostream& operator<< (ostream& os, const S_lpsrVarValAssoc& assoc) {
  assoc->print (os);
  return os;
}

void lpsrVarValAssoc::print (ostream& os)
{
  os << "LpsrVarValAssoc" << endl;
  
  idtr++;

  os << idtr;
  switch (fCommentedKind) {
    case kCommented:
      os << "commented";
      break;
    case kUncommented:
      os << "uncommented";
      break;
  } // switch
  os << endl;

  os << idtr;
  switch (fBackslashKind) {
    case kWithBackslash:
      os << "with backslash";
      break;
    case kWithoutBackslash:
      os << "without backslash";
      break;
  } // switch
  os << " before variable name";
  os << endl;
  
  os <<
    idtr << "variable name: \"" << fVariableName << "\"" <<
    endl;
  
  os << idtr;
  switch (fVarValSeparator) {
    case kSpace:
      os << "space";
      break;
    case kEqualSign:
      os << "equal";
      break;
  } // switch
  os << " separator";
  os << idtr << endl;

  os << idtr;
  switch (fQuotesKind) {
    case kQuotesAroundValue:
      os << "quotes";
      break;
    case kNoQuotesAroundValue:
      os << "noQuotes";
      break;
  } // switch
  os << " around value";
  os << idtr << endl;

  os <<
    idtr << "variable value: \"" << fVariableValue << "\"" <<
    endl;

  os <<
    idtr << "unit: ";
  if (fUnit.size())
    os << "\"" << fUnit << "\"";
  else
    os << "none";
  os << endl;

  os << idtr;
  switch (fEndlKind) {
    case kWithEndl:
      os << "with";
      break;
    case kWithoutEndl:
      os << "without";
      break;
  } // switch
  os << " end line afterwards";
  os << endl;
  
  os << idtr << endl;

  idtr--;
}

//______________________________________________________________________________
S_lpsrSchemeVarValAssoc lpsrSchemeVarValAssoc::create (
  S_msrOptions&     msrOpts, 
  S_lpsrOptions&    lpsrOpts, 
  int               inputLineNumber,
  string            variableName,
  string            value, 
  lpsrCommentedKind commentedKind,
  lpsrEndlKind      endlKind)
{
  lpsrSchemeVarValAssoc* o =
    new lpsrSchemeVarValAssoc (
      msrOpts, lpsrOpts, inputLineNumber,
      variableName, value, commentedKind, endlKind);
  assert(o!=0);
  return o;
}

lpsrSchemeVarValAssoc::lpsrSchemeVarValAssoc (
  S_msrOptions&     msrOpts, 
  S_lpsrOptions&    lpsrOpts, 
  int               inputLineNumber,
  string            variableName,
  string            value, 
  lpsrCommentedKind commentedKind,
  lpsrEndlKind      endlKind)
    : lpsrElement (msrOpts, lpsrOpts, inputLineNumber)
{
  fCommentedKind = commentedKind;

  fVariableName  = variableName;
  fVariableValue = value;

  fEndlKind      = endlKind;
}

lpsrSchemeVarValAssoc::~lpsrSchemeVarValAssoc() {}

void lpsrSchemeVarValAssoc::acceptIn (basevisitor* v) {
  if (fMsrOptions->fDebugDebug)
    cerr << idtr <<
      "==> lpsrSchemeVarValAssoc::acceptIn()" << endl;
      
  if (visitor<S_lpsrSchemeVarValAssoc>*
    p =
      dynamic_cast<visitor<S_lpsrSchemeVarValAssoc>*> (v)) {
        S_lpsrSchemeVarValAssoc elem = this;
        
        if (fMsrOptions->fDebug)
          cerr << idtr <<
            "==> Launching lpsrSchemeVarValAssoc::visitStart()" << endl;
        p->visitStart (elem);
  }
}

void lpsrSchemeVarValAssoc::acceptOut (basevisitor* v) {
  if (fMsrOptions->fDebugDebug)
    cerr << idtr <<
      "==> lpsrSchemeVarValAssoc::acceptOut()" << endl;

  if (visitor<S_lpsrSchemeVarValAssoc>*
    p =
      dynamic_cast<visitor<S_lpsrSchemeVarValAssoc>*> (v)) {
        S_lpsrSchemeVarValAssoc elem = this;
      
        if (fMsrOptions->fDebug)
          cerr << idtr <<
            "==> Launching lpsrSchemeVarValAssoc::visitEnd()" << endl;
        p->visitEnd (elem);
  }
}

void lpsrSchemeVarValAssoc::browseData (basevisitor* v)
{}

ostream& operator<< (ostream& os, const S_lpsrSchemeVarValAssoc& assoc)
{
  assoc->print (os);
  return os;
}

void lpsrSchemeVarValAssoc::print (ostream& os)
{
  os << "SchemeVarValAssoc" << endl;

  idtr++;

  os << idtr;
  switch (fCommentedKind) {
    case kCommented:
      os << "commented";
      break;
    case kUncommented:
      os << "uncommented";
      break;
  } // switch
  os << endl;

  os <<
    idtr << "variable name : \"" << fVariableName << "\"" <<
    endl;
  
  os <<
    idtr << "variable value: \"" << fVariableValue << "\"" <<
    endl;

  os << idtr;
  switch (fEndlKind) {
    case kWithEndl:
      os << "with";
      break;
    case kWithoutEndl:
      os << "without";
      break;
  } // switch
  os << " end line afterwards";
  os << endl;
  
  os << idtr << endl;

  idtr--;
}

//______________________________________________________________________________
S_lpsrNewStaffgroupCommand lpsrNewStaffgroupCommand::create (
  S_msrOptions&  msrOpts, 
  S_lpsrOptions& lpsrOpts, 
  int            inputLineNumber)
{
  lpsrNewStaffgroupCommand* o =
    new lpsrNewStaffgroupCommand (
      msrOpts, lpsrOpts, inputLineNumber);
  assert(o!=0);
  return o;
}

lpsrNewStaffgroupCommand::lpsrNewStaffgroupCommand (
  S_msrOptions&  msrOpts, 
  S_lpsrOptions& lpsrOpts, 
  int            inputLineNumber)
    : lpsrElement (msrOpts, lpsrOpts, inputLineNumber)
{}

lpsrNewStaffgroupCommand::~lpsrNewStaffgroupCommand() {}

void lpsrNewStaffgroupCommand::acceptIn (basevisitor* v) {
  if (fMsrOptions->fDebugDebug)
    cerr << idtr <<
      "==> lpsrNewStaffgroupCommand::acceptIn()" << endl;
      
  if (visitor<S_lpsrNewStaffgroupCommand>*
    p =
      dynamic_cast<visitor<S_lpsrNewStaffgroupCommand>*> (v)) {
        S_lpsrNewStaffgroupCommand elem = this;
        
        if (fMsrOptions->fDebug)
          cerr << idtr <<
            "==> Launching lpsrNewStaffgroupCommand::visitStart()" << endl;
        p->visitStart (elem);
  }
}

void lpsrNewStaffgroupCommand::acceptOut (basevisitor* v) {
  if (fMsrOptions->fDebugDebug)
    cerr << idtr <<
      "==> lpsrNewStaffgroupCommand::acceptOut()" << endl;

  if (visitor<S_lpsrNewStaffgroupCommand>*
    p =
      dynamic_cast<visitor<S_lpsrNewStaffgroupCommand>*> (v)) {
        S_lpsrNewStaffgroupCommand elem = this;
      
        if (fMsrOptions->fDebug)
          cerr << idtr <<
            "==> Launching lpsrNewStaffgroupCommand::visitEnd()" << endl;
        p->visitEnd (elem);
  }
}

void lpsrNewStaffgroupCommand::browseData (basevisitor* v)
{}

ostream& operator<< (ostream& os, const S_lpsrNewStaffgroupCommand& nstf)
{
  nstf->print (os);
  return os;
}

void lpsrNewStaffgroupCommand::print (ostream& os)
{
  os << "NewStaffgroupCommand" << endl;

  idtr++;
  
  int size = fNewStaffgroupElements.size();

  for (int i = 0; i < size; i++ ) {
    os << idtr << fNewStaffgroupElements[i];
  } // for
  
  idtr--;
}

//______________________________________________________________________________
S_lpsrNewStaffCommand lpsrNewStaffCommand::create (
  S_msrOptions&  msrOpts, 
  S_lpsrOptions& lpsrOpts, 
  int            inputLineNumber)
{
  lpsrNewStaffCommand* o =
    new lpsrNewStaffCommand (
      msrOpts, lpsrOpts, inputLineNumber);
  assert(o!=0);
  return o;
}

lpsrNewStaffCommand::lpsrNewStaffCommand (
  S_msrOptions&  msrOpts, 
  S_lpsrOptions& lpsrOpts, 
  int            inputLineNumber)
    : lpsrElement (msrOpts, lpsrOpts, inputLineNumber)
{}

lpsrNewStaffCommand::~lpsrNewStaffCommand() {}

void lpsrNewStaffCommand::acceptIn (basevisitor* v) {
  if (fMsrOptions->fDebugDebug)
    cerr << idtr <<
      "==> lpsrNewStaffCommand::acceptIn()" << endl;
      
  if (visitor<S_lpsrNewStaffCommand>*
    p =
      dynamic_cast<visitor<S_lpsrNewStaffCommand>*> (v)) {
        S_lpsrNewStaffCommand elem = this;
        
        if (fMsrOptions->fDebug)
          cerr << idtr <<
            "==> Launching lpsrNewStaffCommand::visitStart()" << endl;
        p->visitStart (elem);
  }
}

void lpsrNewStaffCommand::acceptOut (basevisitor* v) {
  if (fMsrOptions->fDebugDebug)
    cerr << idtr <<
      "==> lpsrNewStaffCommand::acceptOut()" << endl;

  if (visitor<S_lpsrNewStaffCommand>*
    p =
      dynamic_cast<visitor<S_lpsrNewStaffCommand>*> (v)) {
        S_lpsrNewStaffCommand elem = this;
      
        if (fMsrOptions->fDebug)
          cerr << idtr <<
            "==> Launching lpsrNewStaffCommand::visitEnd()" << endl;
        p->visitEnd (elem);
  }
}

void lpsrNewStaffCommand::browseData (basevisitor* v)
{}

ostream& operator<< (ostream& os, const S_lpsrNewStaffCommand& nstf)
{
  nstf->print (os);
  return os;
}

void lpsrNewStaffCommand::print (ostream& os)
{
  os << "NewStaffCommand" << endl;

  idtr++;
  
  int size = fNewStaffElements.size();

  for (int i = 0; i < size; i++ ) {
    os << idtr << fNewStaffElements[i];
  } // for
  
  idtr--;
}

//______________________________________________________________________________
S_lpsrVariableUseCommand lpsrVariableUseCommand::create (
  S_msrOptions&  msrOpts, 
  S_lpsrOptions& lpsrOpts, 
  int            inputLineNumber,
  string         variableName)
{
  lpsrVariableUseCommand* o =
    new lpsrVariableUseCommand (
      msrOpts, lpsrOpts, inputLineNumber, variableName);
  assert(o!=0);
  return o;
}

lpsrVariableUseCommand::lpsrVariableUseCommand (
  S_msrOptions&  msrOpts, 
  S_lpsrOptions& lpsrOpts, 
  int            inputLineNumber,
  string         variableName)
    : lpsrElement (msrOpts, lpsrOpts, inputLineNumber)
{
  fVariableName = variableName;
}

lpsrVariableUseCommand::~lpsrVariableUseCommand() {}

void lpsrVariableUseCommand::acceptIn (basevisitor* v) {
  if (fMsrOptions->fDebugDebug)
    cerr << idtr <<
      "==> lpsrVariableUseCommand::acceptIn()" << endl;
      
  if (visitor<S_lpsrVariableUseCommand>*
    p =
      dynamic_cast<visitor<S_lpsrVariableUseCommand>*> (v)) {
        S_lpsrVariableUseCommand elem = this;
        
        if (fMsrOptions->fDebug)
          cerr << idtr <<
            "==> Launching lpsrVariableUseCommand::visitStart()" << endl;
        p->visitStart (elem);
  }
}

void lpsrVariableUseCommand::acceptOut (basevisitor* v) {
  if (fMsrOptions->fDebugDebug)
    cerr << idtr <<
      "==> lpsrVariableUseCommand::acceptOut()" << endl;

  if (visitor<S_lpsrVariableUseCommand>*
    p =
      dynamic_cast<visitor<S_lpsrVariableUseCommand>*> (v)) {
        S_lpsrVariableUseCommand elem = this;
      
        if (fMsrOptions->fDebug)
          cerr << idtr <<
            "==> Launching lpsrVariableUseCommand::visitEnd()" << endl;
        p->visitEnd (elem);
  }
}

void lpsrVariableUseCommand::browseData (basevisitor* v)
{}

ostream& operator<< (ostream& os, const S_lpsrVariableUseCommand& nstf)
{
  nstf->print (os);
  return os;
}

void lpsrVariableUseCommand::print (ostream& os)
{
  os << "VariableUseCommand" << endl;
  idtr++;
  os << idtr << fVariableName << endl;
  idtr--;
}

/*
void lpsrVariableUseCommand::printLilyPondCode (ostream& os)
{
  os << "\\" << fVariableName << endl;
}
*/

//______________________________________________________________________________
S_lpsrContext lpsrContext::create (
    S_msrOptions&   msrOpts, 
    S_lpsrOptions&  lpsrOpts, 
    int             inputLineNumber,
    lpsrContextKind contextKind,
    string          contextType,
    string          contextName)
{
  lpsrContext* o =
    new lpsrContext (
      msrOpts, lpsrOpts, inputLineNumber,
      contextKind, contextType, contextName);
  assert(o!=0);
  return o;
}

lpsrContext::lpsrContext (
  S_msrOptions&   msrOpts, 
  S_lpsrOptions&  lpsrOpts, 
  int             inputLineNumber,
  lpsrContextKind contextKind,
  string          contextType,
  string          contextName)
    : lpsrElement (msrOpts, lpsrOpts, inputLineNumber)
{
  fContextKind = contextKind;
  fContextType = contextType;
  fContextName = contextName; 
}
lpsrContext::~lpsrContext() {}

void lpsrContext::acceptIn (basevisitor* v) {
  if (fMsrOptions->fDebugDebug)
    cerr << idtr <<
      "==> lpsrContext::acceptIn()" << endl;
      
  if (visitor<S_lpsrContext>*
    p =
      dynamic_cast<visitor<S_lpsrContext>*> (v)) {
        S_lpsrContext elem = this;
        
        if (fMsrOptions->fDebug)
          cerr << idtr <<
            "==> Launching lpsrContext::visitStart()" << endl;
        p->visitStart (elem);
  }
}

void lpsrContext::acceptOut (basevisitor* v) {
  if (fMsrOptions->fDebugDebug)
    cerr << idtr <<
      "==> lpsrContext::acceptOut()" << endl;

  if (visitor<S_lpsrContext>*
    p =
      dynamic_cast<visitor<S_lpsrContext>*> (v)) {
        S_lpsrContext elem = this;
      
        if (fMsrOptions->fDebug)
          cerr << idtr <<
            "==> Launching lpsrContext::visitEnd()" << endl;
        p->visitEnd (elem);
  }
}

void lpsrContext::browseData (basevisitor* v)
{}

void lpsrContext::print (ostream& os)
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

/*
void lpsrContext::printLilyPondCode (ostream& os)
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
*/

//______________________________________________________________________________
S_lpsrBarCommand lpsrBarCommand::create (
  S_msrOptions&  msrOpts, 
  S_lpsrOptions& lpsrOpts, 
  int            inputLineNumber)
{
  lpsrBarCommand* o =
    new lpsrBarCommand (
      msrOpts, lpsrOpts, inputLineNumber);
  assert(o!=0);
  return o;
}

lpsrBarCommand::lpsrBarCommand (
  S_msrOptions&  msrOpts, 
  S_lpsrOptions& lpsrOpts, 
  int            inputLineNumber)
    : lpsrElement (msrOpts, lpsrOpts, inputLineNumber)
{}

lpsrBarCommand::~lpsrBarCommand() {}

void lpsrBarCommand::acceptIn (basevisitor* v) {
  if (fMsrOptions->fDebugDebug)
    cerr << idtr <<
      "==> lpsrBarCommand::acceptIn()" << endl;
      
  if (visitor<S_lpsrBarCommand>*
    p =
      dynamic_cast<visitor<S_lpsrBarCommand>*> (v)) {
        S_lpsrBarCommand elem = this;
        
        if (fMsrOptions->fDebug)
          cerr << idtr <<
            "==> Launching lpsrBarCommand::visitStart()" << endl;
        p->visitStart (elem);
  }
}

void lpsrBarCommand::acceptOut (basevisitor* v) {
  if (fMsrOptions->fDebugDebug)
    cerr << idtr <<
      "==> lpsrBarCommand::acceptOut()" << endl;

  if (visitor<S_lpsrBarCommand>*
    p =
      dynamic_cast<visitor<S_lpsrBarCommand>*> (v)) {
        S_lpsrBarCommand elem = this;
      
        if (fMsrOptions->fDebug)
          cerr << idtr <<
            "==> Launching lpsrBarCommand::visitEnd()" << endl;
        p->visitEnd (elem);
  }
}

void lpsrBarCommand::browseData (basevisitor* v)
{}

ostream& operator<< (ostream& os, const S_lpsrBarCommand& nstf)
{
  nstf->print (os);
  return os;
}

void lpsrBarCommand::print (ostream& os)
{
  os << "BarCommand" << " " << "\"|.\"" << endl;
}

/*
void lpsrBarCommand::printLilyPondCode (ostream& os)
{      
  os << "\\bar" << " " << "\"|.\"" << endl;
}
*/

//______________________________________________________________________________
S_lpsrHeader lpsrHeader::create (
  S_msrOptions&  msrOpts, 
  S_lpsrOptions& lpsrOpts, 
  int            inputLineNumber)
{
  lpsrHeader* o =
    new lpsrHeader (
      msrOpts, lpsrOpts, inputLineNumber);
  assert(o!=0);
  return o;
}

lpsrHeader::lpsrHeader (
  S_msrOptions&  msrOpts, 
  S_lpsrOptions& lpsrOpts, 
  int            inputLineNumber)
    : lpsrElement (msrOpts, lpsrOpts, inputLineNumber)
{}

lpsrHeader::~lpsrHeader() {}

void lpsrHeader::setWorkNumber (
  int    inputLineNumber,
  string val)
  {
  fWorkNumber =
    lpsrVarValAssoc::create (
      fMsrOptions, fLpsrOptions, inputLineNumber,
      lpsrVarValAssoc::kUncommented,
      lpsrVarValAssoc::kWithoutBackslash,
      "work-number",
      lpsrVarValAssoc::kEqualSign,
      lpsrVarValAssoc::kQuotesAroundValue,
      val,
      lpsrVarValAssoc::g_NoUnit,
      lpsrVarValAssoc::kWithoutEndl);
  }

void lpsrHeader::setWorkTitle (
  int    inputLineNumber,
  string val)
  {
  fWorkTitle =
    lpsrVarValAssoc::create (
      fMsrOptions, fLpsrOptions, inputLineNumber,
      lpsrVarValAssoc::kUncommented,
      lpsrVarValAssoc::kWithoutBackslash,
      "work-title",
      lpsrVarValAssoc::kEqualSign,
      lpsrVarValAssoc::kQuotesAroundValue,
      val,
      lpsrVarValAssoc::g_NoUnit,
      lpsrVarValAssoc::kWithoutEndl);
  }

void lpsrHeader::setMovementNumber (
  int    inputLineNumber,
  string val)
  {
  fMovementNumber =
    lpsrVarValAssoc::create (
      fMsrOptions, fLpsrOptions, inputLineNumber,
      lpsrVarValAssoc::kUncommented,
      lpsrVarValAssoc::kWithoutBackslash,
      "movement-number",
      lpsrVarValAssoc::kEqualSign,
      lpsrVarValAssoc::kQuotesAroundValue,
      val,
      lpsrVarValAssoc::g_NoUnit,
      lpsrVarValAssoc::kWithoutEndl);
  }

void lpsrHeader::setMovementTitle (
  int    inputLineNumber,
  string val)
{
  fMovementTitle =
    lpsrVarValAssoc::create (
      fMsrOptions, fLpsrOptions, inputLineNumber,
      lpsrVarValAssoc::kUncommented,
      lpsrVarValAssoc::kWithoutBackslash,
      "movement-title",
      lpsrVarValAssoc::kEqualSign,
      lpsrVarValAssoc::kQuotesAroundValue,
      val,
      lpsrVarValAssoc::g_NoUnit,
      lpsrVarValAssoc::kWithoutEndl);
}

void lpsrHeader::addCreator (
  int    inputLineNumber,
  string type,
  string val)
{
  fCreators.push_back(
    lpsrVarValAssoc::create (
      fMsrOptions, fLpsrOptions, inputLineNumber,
      lpsrVarValAssoc::kUncommented,
      lpsrVarValAssoc::kWithoutBackslash,
      type,
      lpsrVarValAssoc::kEqualSign,
      lpsrVarValAssoc::kQuotesAroundValue,
      val,
      lpsrVarValAssoc::g_NoUnit,
      lpsrVarValAssoc::kWithoutEndl)
  );
}

void lpsrHeader::setRights (
  int    inputLineNumber,
  string val)
  {
  fRights =
    lpsrVarValAssoc::create (
      fMsrOptions, fLpsrOptions, inputLineNumber,
      lpsrVarValAssoc::kUncommented,
      lpsrVarValAssoc::kWithoutBackslash,
      "rights",
      lpsrVarValAssoc::kEqualSign,
      lpsrVarValAssoc::kQuotesAroundValue,
      val,
      lpsrVarValAssoc::g_NoUnit,
      lpsrVarValAssoc::kWithoutEndl);
  }

void lpsrHeader::addSoftware (
  int    inputLineNumber,
  string val)
{
  fSoftwares.push_back(
    lpsrVarValAssoc::create (
      fMsrOptions, fLpsrOptions, inputLineNumber,
      lpsrVarValAssoc::kUncommented,
      lpsrVarValAssoc::kWithoutBackslash,
      "software",
      lpsrVarValAssoc::kEqualSign,
      lpsrVarValAssoc::kQuotesAroundValue,
      val,
      lpsrVarValAssoc::g_NoUnit,
      lpsrVarValAssoc::kWithoutEndl)
  );
}

void lpsrHeader::setEncodingDate (
  int    inputLineNumber,
  string val)
{
  fEncodingDate =
    lpsrVarValAssoc::create (
      fMsrOptions, fLpsrOptions, inputLineNumber,
      lpsrVarValAssoc::kUncommented,
      lpsrVarValAssoc::kWithoutBackslash,
      "encoding-date",
      lpsrVarValAssoc::kEqualSign,
      lpsrVarValAssoc::kQuotesAroundValue,
      val,
      lpsrVarValAssoc::g_NoUnit,
      lpsrVarValAssoc::kWithoutEndl);
}

void lpsrHeader::setScoreInstrument (
  int    inputLineNumber,
  string val)
{
  fScoreInstrument =
    lpsrVarValAssoc::create (
      fMsrOptions, fLpsrOptions, inputLineNumber,
      lpsrVarValAssoc::kUncommented,
      lpsrVarValAssoc::kWithoutBackslash,
      "score-instrument",
      lpsrVarValAssoc::kEqualSign,
      lpsrVarValAssoc::kQuotesAroundValue,
      val,
      lpsrVarValAssoc::g_NoUnit,
      lpsrVarValAssoc::kWithoutEndl);
}

void lpsrHeader::acceptIn (basevisitor* v) {
  if (fMsrOptions->fDebugDebug)
    cerr << idtr <<
      "==> lpsrHeader::acceptIn()" << endl;
      
  if (visitor<S_lpsrHeader>*
    p =
      dynamic_cast<visitor<S_lpsrHeader>*> (v)) {
        S_lpsrHeader elem = this;
        
        if (fMsrOptions->fDebug)
          cerr << idtr <<
            "==> Launching lpsrHeader::visitStart()" << endl;
        p->visitStart (elem);
  }
}

void lpsrHeader::acceptOut (basevisitor* v) {
  if (fMsrOptions->fDebugDebug)
    cerr << idtr <<
      "==> lpsrHeader::acceptOut()" << endl;

  if (visitor<S_lpsrHeader>*
    p =
      dynamic_cast<visitor<S_lpsrHeader>*> (v)) {
        S_lpsrHeader elem = this;
      
        if (fMsrOptions->fDebug)
          cerr << idtr <<
            "==> Launching lpsrHeader::visitEnd()" << endl;
        p->visitEnd (elem);
  }
}

void lpsrHeader::browseData (basevisitor* v)
{
  if (fWorkNumber) {
    // browse fWorkNumber
    msrBrowser<lpsrVarValAssoc> browser (v);
    browser.browse (*fWorkNumber);
  }

  if (fWorkTitle) {
    // browse fWorkTitle
    msrBrowser<lpsrVarValAssoc> browser (v);
    browser.browse (*fWorkTitle);
  }

  if (fMovementNumber) {
    // browse fMovementNumber
    msrBrowser<lpsrVarValAssoc> browser (v);
    browser.browse (*fMovementNumber);
  }

  if (fMovementTitle) {
    // browse fMovementTitle
    msrBrowser<lpsrVarValAssoc> browser (v);
    browser.browse (*fMovementTitle);
  }

  if (! fCreators.empty()) {
    vector<S_lpsrVarValAssoc>::const_iterator i;
    for (i=fCreators.begin(); i!=fCreators.end(); i++) {
      // browse creator
      msrBrowser<lpsrVarValAssoc> browser (v);
      browser.browse (*(*i));
    } // for
  }
    
  if (fRights) {
    // browse fRights
    msrBrowser<lpsrVarValAssoc> browser (v);
    browser.browse (*fRights);
  }

  if (! fSoftwares.empty()) {
    vector<S_lpsrVarValAssoc>::const_iterator i;
    for (i=fSoftwares.begin(); i!=fSoftwares.end(); i++) {
      // browse software
      msrBrowser<lpsrVarValAssoc> browser (v);
      browser.browse (*(*i));
    } // for
  }

  if (fEncodingDate) {
    // browse fEncodingDate
    msrBrowser<lpsrVarValAssoc> browser (v);
    browser.browse (*fEncodingDate);
  }

  if (fScoreInstrument) {
    // browse fScoreInstrument
    msrBrowser<lpsrVarValAssoc> browser (v);
    browser.browse (*fScoreInstrument);
  }
}

ostream& operator<< (ostream& os, const S_lpsrHeader& elt)
{
  elt->print (os);
  return os;
}

void lpsrHeader::print (ostream& os)
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
    
  if (! fCreators.empty()) {
    vector<S_lpsrVarValAssoc>::const_iterator i1;
    for (i1=fCreators.begin(); i1!=fCreators.end(); i1++) {
      os << idtr << (*i1);
    } // for
  }
    
  if (fRights) {
    os << idtr << fRights;
  }
    
  if (! fSoftwares.empty()) {
    vector<S_lpsrVarValAssoc>::const_iterator i2;
    for (i2=fSoftwares.begin(); i2!=fSoftwares.end(); i2++) {
      os << idtr << (*i2);
    } // for
  }
    
  if (fEncodingDate) {
    os << idtr << fEncodingDate;
  }
  
  idtr--;
}

//______________________________________________________________________________
S_lpsrPaper lpsrPaper::create (
  S_msrOptions& msrOpts, 
  int           inputLineNumber)
{
  lpsrPaper* o =
    new lpsrPaper (
      msrOpts, inputLineNumber);
  assert(o!=0);
  return o;
}

lpsrPaper::lpsrPaper (
  S_msrOptions& msrOpts, 
  int           inputLineNumber)
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

lpsrPaper::~lpsrPaper() {}

void lpsrPaper::acceptIn (basevisitor* v) {
  if (fMsrOptions->fDebugDebug)
    cerr << idtr <<
      "==> lpsrPaper::acceptIn()" << endl;
      
  if (visitor<S_lpsrPaper>*
    p =
      dynamic_cast<visitor<S_lpsrPaper>*> (v)) {
        S_lpsrPaper elem = this;
        
        if (fMsrOptions->fDebug)
          cerr << idtr <<
            "==> Launching lpsrPaper::visitStart()" << endl;
        p->visitStart (elem);
  }
}

void lpsrPaper::acceptOut (basevisitor* v) {
  if (fMsrOptions->fDebugDebug)
    cerr << idtr <<
      "==> lpsrPaper::acceptOut()" << endl;

  if (visitor<S_lpsrPaper>*
    p =
      dynamic_cast<visitor<S_lpsrPaper>*> (v)) {
        S_lpsrPaper elem = this;
      
        if (fMsrOptions->fDebug)
          cerr << idtr <<
            "==> Launching lpsrPaper::visitEnd()" << endl;
        p->visitEnd (elem);
  }
}

void lpsrPaper::browseData (basevisitor* v)
{}

ostream& operator<< (ostream& os, const S_lpsrPaper& pap) {
  pap->print (os);
  return os;
}

void lpsrPaper::print (ostream& os) {
  os << "Paper" << endl;

  idtr++;
  
  if (fPaperWidth > 0) {
    os << idtr <<
      setw(13) << left << "paper-width" << " = " <<
      setprecision(4) << fPaperWidth << "\\cm" << endl;
  }
  if (fPaperHeight > 0) {
    os << idtr << 
      setw(13) << left << "paper-height" << " = " <<
      setprecision(4) << fPaperHeight << "\\cm" << endl;
  }
  
  if (fTopMargin > 0) {
    os << idtr << 
      setw(13) << left << "top-margin" << " = " <<
      setprecision(4) << fTopMargin << "\\cm" << endl;
  }
  if (fBottomMargin > 0) {
    os << idtr << 
      setw(13) << left << "bottom-margin" << " = " <<
      setprecision(4) << fBottomMargin << "\\cm" << endl;
  }
  if (fLeftMargin > 0) {
    os << idtr << 
      setw(13) << left << "left-margin" << " = " <<
      setprecision(4) << fLeftMargin << "\\cm" << endl;
  }
  if (fRightMargin > 0) {
    os << idtr << 
      setw(13) << left << "right-margin" << " = " <<
    setprecision(4) << fRightMargin << "\\cm" << endl;
  }

/*
 * 
  if (fBetweenSystemSpace > 0) {
    os << idtr << "between-system-space = " << setprecision(4) << fBetweenSystemSpace << "\\cm" << endl;
  }

  if (fPageTopSpace > 0) {
    os << idtr << "page-top-space = " << setprecision(4) << fPageTopSpace << "\\cm" << endl;
  }
*/

  idtr--;
}

//______________________________________________________________________________
S_lpsrLayout lpsrLayout::create (
  S_msrOptions&  msrOpts, 
  S_lpsrOptions& lpsrOpts, 
  int            inputLineNumber)
{
  lpsrLayout* o =
    new lpsrLayout (
      msrOpts, lpsrOpts,inputLineNumber);
  assert(o!=0);
  return o;
}

lpsrLayout::lpsrLayout (
  S_msrOptions&  msrOpts, 
  S_lpsrOptions& lpsrOpts, 
  int            inputLineNumber)
    : lpsrElement (msrOpts, lpsrOpts, inputLineNumber)
{
  fMillimeters = -1;
  fTenths      = -1;
}

lpsrLayout::~lpsrLayout() {}

void lpsrLayout::acceptIn (basevisitor* v) {
  if (fMsrOptions->fDebugDebug)
    cerr << idtr <<
      "==> lpsrLayout::acceptIn()" << endl;
      
  if (visitor<S_lpsrLayout>*
    p =
      dynamic_cast<visitor<S_lpsrLayout>*> (v)) {
        S_lpsrLayout elem = this;
        
        if (fMsrOptions->fDebug)
          cerr << idtr <<
            "==> Launching lpsrLayout::visitStart()" << endl;
        p->visitStart (elem);
  }
}

void lpsrLayout::acceptOut (basevisitor* v) {
  if (fMsrOptions->fDebugDebug)
    cerr << idtr <<
      "==> lpsrLayout::acceptOut()" << endl;

  if (visitor<S_lpsrLayout>*
    p =
      dynamic_cast<visitor<S_lpsrLayout>*> (v)) {
        S_lpsrLayout elem = this;
      
        if (fMsrOptions->fDebug)
          cerr << idtr <<
            "==> Launching lpsrLayout::visitEnd()" << endl;
        p->visitEnd (elem);
  }
}

void lpsrLayout::browseData (basevisitor* v)
{
  int n1 = fLpsrVarValAssocs.size();
  
  for (int i = 0; i < n1; i++ ) {
    // browse the variable/value association
    msrBrowser<lpsrVarValAssoc> browser (v);
    browser.browse (*fLpsrVarValAssocs [i]);
  } // for
}

ostream& operator<< (ostream& os, const S_lpsrLayout& lay)
{
  lay->print (os);
  return os;
}

void lpsrLayout::print (ostream& os)
{
  os << "Layout" << endl;

  idtr++;

  if (fMillimeters > 0) {
    os << idtr <<
      setw(12) << left << "Millimeters" << " = " <<
      setprecision(4) << fMillimeters << endl;
  }

  if (fTenths > 0) {
    os << idtr <<
      setw(12) << left << "Tenths" << " = " <<
      setprecision(4) << fTenths << endl;
  }

  int n1 = fLpsrVarValAssocs.size();
  
  for (int i = 0; i < n1; i++ ) {
    os << idtr << fLpsrVarValAssocs [i];
  } // for

  int n2 = fLpsrSchemeVarValAssocs.size();
  for (int i = 0; i < n2; i++ ) {
    os << idtr << fLpsrSchemeVarValAssocs[i];
  } // for
  
  idtr--;
}

//______________________________________________________________________________
S_lpsrScoreCommand lpsrScoreCommand::create (
  S_msrOptions&  msrOpts, 
  S_lpsrOptions& lpsrOpts, 
  int            inputLineNumber)
{
  lpsrScoreCommand* o = new lpsrScoreCommand (
    msrOpts, lpsrOpts, inputLineNumber);
  assert(o!=0);
  return o;
}

lpsrScoreCommand::lpsrScoreCommand (
  S_msrOptions&  msrOpts, 
  S_lpsrOptions& lpsrOpts, 
  int            inputLineNumber)
    : lpsrElement (msrOpts, lpsrOpts, inputLineNumber)
{
  // create the score command parallel music
  fScoreCommandParallelMusic =
    lpsrParallelMusic::create (
      msrOpts, lpsrOpts, inputLineNumber,
      lpsrParallelMusic::kEndOfLine);
  
  // create the score command layout
  fScoreCommandLayout =
    lpsrLayout::create (
      msrOpts, lpsrOpts, inputLineNumber);
  
  // create the score command midi
  fScoreCommandMidi =
    msrMidi::create (msrOpts, inputLineNumber);
}

lpsrScoreCommand::~lpsrScoreCommand() {}

void lpsrScoreCommand::acceptIn (basevisitor* v) {
  if (fMsrOptions->fDebugDebug)
    cerr << idtr <<
      "==> lpsrScoreCommand::acceptIn()" << endl;
      
  if (visitor<S_lpsrScoreCommand>*
    p =
      dynamic_cast<visitor<S_lpsrScoreCommand>*> (v)) {
        S_lpsrScoreCommand elem = this;
        
        if (fMsrOptions->fDebug)
          cerr << idtr <<
            "==> Launching lpsrScoreCommand::visitStart()" << endl;
        p->visitStart (elem);
  }
}

void lpsrScoreCommand::acceptOut (basevisitor* v) {
  if (fMsrOptions->fDebugDebug)
    cerr << idtr <<
      "==> lpsrScoreCommand::acceptOut()" << endl;

  if (visitor<S_lpsrScoreCommand>*
    p =
      dynamic_cast<visitor<S_lpsrScoreCommand>*> (v)) {
        S_lpsrScoreCommand elem = this;
      
        if (fMsrOptions->fDebug)
          cerr << idtr <<
            "==> Launching lpsrScoreCommand::visitEnd()" << endl;
        p->visitEnd (elem);
  }
}

void lpsrScoreCommand::browseData (basevisitor* v)
{
  if (fMsrOptions->fDebug)
    cerr << idtr <<
      "==> lpsrScoreCommand::browseData()" << endl;

  {
    // browse the score command parallel music
    msrBrowser<lpsrParallelMusic> browser (v);    
    browser.browse (*fScoreCommandParallelMusic);
  }

  {
    // browse the score command layout
    msrBrowser<lpsrLayout> browser (v);    
    browser.browse (*fScoreCommandLayout);
  }

  {
    // browse the score command midi
    msrBrowser<msrMidi> browser (v);    
    browser.browse (*fScoreCommandMidi);
  }

  if (fMsrOptions->fDebug)
    cerr << idtr <<
      "<== lpsrScoreCommand::browseData()" << endl;
}

ostream& operator<< (ostream& os, const S_lpsrScoreCommand& scr)
{
  scr->print (os);
  return os;
}

void lpsrScoreCommand::print (ostream& os)
{
  os << "ScoreCommand" << endl << endl;

  idtr++;

  os << idtr << fScoreCommandParallelMusic << endl;
  os << idtr << fScoreCommandLayout << endl;
  os << idtr << fScoreCommandMidi << endl;

  idtr--;
}

//______________________________________________________________________________
S_lpsrScore lpsrScore::create (
  S_msrOptions&  msrOpts, 
  S_lpsrOptions& lpsrOpts, 
  int            inputLineNumber,
  S_msrScore     mScore)
{
  lpsrScore* o = new lpsrScore (
    msrOpts, lpsrOpts, inputLineNumber, mScore);
  assert(o!=0);
  return o;
}

lpsrScore::lpsrScore (
  S_msrOptions&  msrOpts, 
  S_lpsrOptions& lpsrOpts, 
  int            inputLineNumber,
  S_msrScore     mScore)
    : lpsrElement (msrOpts, lpsrOpts, inputLineNumber)
{
  fMsrScore = mScore;

  // create the LilyPond version assoc
  fLilyPondVersion =
    lpsrVarValAssoc::create (
      msrOpts, lpsrOpts, inputLineNumber,
      lpsrVarValAssoc::kUncommented,
      lpsrVarValAssoc::kWithBackslash,
      "version",
      lpsrVarValAssoc::kSpace,
      lpsrVarValAssoc::kQuotesAroundValue,
      "2.19",
      lpsrVarValAssoc::g_NoUnit,
      lpsrVarValAssoc::kWithEndl);

  // create the global staff size assoc
  fGlobalStaffSize =
    lpsrSchemeVarValAssoc::create (
      fMsrOptions,
      fLpsrOptions,
      inputLineNumber,
      "set-global-staff-size",
      "-1",
      lpsrSchemeVarValAssoc::kCommented,
      lpsrSchemeVarValAssoc::kWithEndl);

  // create the header
  fHeader =
    lpsrHeader::create (
      msrOpts, lpsrOpts, inputLineNumber);
  
  // create the paper
  fPaper =
    lpsrPaper::create (
      msrOpts, inputLineNumber);

  // create the score layout
  fScoreLayout =
    lpsrLayout::create (
      msrOpts, lpsrOpts, inputLineNumber);

  // create the score command
  fScoreCommand =
    lpsrScoreCommand::create (
      msrOpts, lpsrOpts, inputLineNumber);
}

lpsrScore::~lpsrScore() {}

void lpsrScore::appendVoiceUseToStoreCommand (S_msrVoice voice)
{
  S_lpsrUseVoiceCommand
    useVoiceCommand =
      lpsrUseVoiceCommand::create (
        fMsrOptions, 
        fLpsrOptions, 
        fInputLineNumber,
        voice);
  
  fScoreCommand->
    appendVoiceUseToParallelMusic (useVoiceCommand);
}

void lpsrScore::appendLyricsUseToStoreCommand (S_msrLyrics lyrics)
{
  S_lpsrNewlyricsCommand
    newLyricsCommand =
      lpsrNewlyricsCommand::create (
        fMsrOptions, 
        fLpsrOptions, 
        fInputLineNumber,
        lyrics,
        lyrics->getLyricsVoice ());
  
  fScoreCommand->
    appendLyricsUseToParallelMusic (newLyricsCommand);
}

void lpsrScore::acceptIn (basevisitor* v) {
  if (fMsrOptions->fDebugDebug)
    cerr << idtr <<
      "==> lpsrScore::acceptIn()" << endl;
      
  if (visitor<S_lpsrScore>*
    p =
      dynamic_cast<visitor<S_lpsrScore>*> (v)) {
        S_lpsrScore elem = this;
        
        if (fMsrOptions->fDebug)
          cerr << idtr <<
            "==> Launching lpsrScore::visitStart()" << endl;
        p->visitStart (elem);
  }
}

void lpsrScore::acceptOut (basevisitor* v) {
  if (fMsrOptions->fDebugDebug)
    cerr << idtr <<
      "==> lpsrScore::acceptOut()" << endl;

  if (visitor<S_lpsrScore>*
    p =
      dynamic_cast<visitor<S_lpsrScore>*> (v)) {
        S_lpsrScore elem = this;
      
        if (fMsrOptions->fDebug)
          cerr << idtr <<
            "==> Launching lpsrScore::visitEnd()" << endl;
        p->visitEnd (elem);
  }
}

void lpsrScore::browseData (basevisitor* v)
{
  if (fMsrOptions->fDebug)
    cerr << idtr <<
      "==> lpsrScore::browseData()" << endl;

  {
    // browse the score LilyPond version
    msrBrowser<lpsrVarValAssoc> browser (v);
    browser.browse (*fLilyPondVersion);
  }
  
  {
    // browse the score global size
    msrBrowser<lpsrSchemeVarValAssoc> browser (v);
    browser.browse (*fGlobalStaffSize);
  }

  {
    // browse the score header
    msrBrowser<lpsrHeader> browser (v);
    browser.browse (*fHeader);
  }

  {
    // browse the score paper
    msrBrowser<lpsrPaper> browser (v);
    browser.browse (*fPaper);
  }

  {
    // browse the score layout
    msrBrowser<lpsrLayout> browser (v);
    browser.browse (*fScoreLayout);
  }

  {
    // browse the voices and lyrics list
    for (
      list<S_msrElement>::iterator i = fScoreElements.begin();
      i != fScoreElements.end();
      i++) {
      // create the element browser
      msrBrowser<msrElement> browser (v);
    
      // browse the element with the visitor
      browser.browse (*(*i));
    } // for
  }

  {
    // browse the score command
    msrBrowser<lpsrScoreCommand> browser (v);    
    browser.browse (*fScoreCommand);
  }

  if (fMsrOptions->fDebug)
    cerr << idtr <<
      "<== lpsrScore::browseData()" << endl;
}

ostream& operator<< (ostream& os, const S_lpsrScore& scr)
{
  scr->print (os);
  return os;
}

void lpsrScore::print (ostream& os)
{
  os << "LPSR Score" << endl << endl;

  idtr++;

  if (fMsrOptions->fDebugDebug)
    os << idtr << fMsrScore;

  os << idtr << fLilyPondVersion << endl;
  os << idtr << fGlobalStaffSize << endl;
  os << idtr << fHeader << endl;
  os << idtr << fPaper << endl;
  os << idtr << fScoreLayout << endl;
  
  if (fScoreElements.size()) {  
    list<S_msrElement>::const_iterator
      iBegin = fScoreElements.begin(),
      iEnd   = fScoreElements.end(),
      i      = iBegin;
    for ( ; ; ) {
      os << idtr << (*i);
      if (++i == iEnd) break;
      os << endl;
    } // for
  }

  os << idtr << fScoreCommand << endl;

  idtr--;
}


}
