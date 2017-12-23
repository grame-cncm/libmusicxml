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
#include <iomanip>      // setw, setprecision, ...
#include <climits>      /* INT_MIN, INT_MAX */

#include "version.h"

#include "lpsr.h"

#include "lpsrOptions.h"
#include "lilypondOptions.h"

#include "xml2lyOptionsHandling.h"


using namespace std;

namespace MusicXML2 
{

//_______________________________________________________________________________
S_lpsrElement lpsrElement::create (
  int            inputLineNumber)
{
  lpsrElement* o =
    new lpsrElement (
      inputLineNumber);
  assert(o!=0);
  return o;
}

lpsrElement::lpsrElement (
  int            inputLineNumber)
    : msrElement (inputLineNumber)
{}

lpsrElement::~lpsrElement()
{}

void lpsrElement::acceptIn (basevisitor* v)
{
  if (gLpsrOptions->fTraceLpsrVisitors) {
    gLogIOstream <<
      "% ==> lpsrElement::acceptIn()" <<
      endl;
  }
  
  if (visitor<S_lpsrElement>*
    p =
      dynamic_cast<visitor<S_lpsrElement>*> (v)) {
        S_lpsrElement elem = this;
        
        if (gLpsrOptions->fTraceLpsrVisitors) {
          gLogIOstream <<
            "% ==> Launching lpsrElement::visitStart()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void lpsrElement::acceptOut (basevisitor* v)
{
  if (gLpsrOptions->fTraceLpsrVisitors) {
    gLogIOstream <<
      "% ==> lpsrElement::acceptOut()" <<
      endl;
  }

  if (visitor<S_lpsrElement>*
    p =
      dynamic_cast<visitor<S_lpsrElement>*> (v)) {
        S_lpsrElement elem = this;
      
        if (gLpsrOptions->fTraceLpsrVisitors) {
          gLogIOstream <<
            "% ==> Launching lpsrElement::visitEnd()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void lpsrElement::browseData (basevisitor* v)
{}

//______________________________________________________________________________
S_lpsrParallelMusicBLock lpsrParallelMusicBLock::create (
  int                       inputLineNumber,
  lpsrElementsSeparatorKind elementsSeparatorKind)
{
  lpsrParallelMusicBLock* o =
    new lpsrParallelMusicBLock (
      inputLineNumber,
      elementsSeparatorKind);
  assert(o!=0);
  return o;
}

lpsrParallelMusicBLock::lpsrParallelMusicBLock (
  int                       inputLineNumber,
  lpsrElementsSeparatorKind elementsSeparatorKind)
    : lpsrElement (inputLineNumber)
{
  fElementsSeparatorKind = elementsSeparatorKind;
}

lpsrParallelMusicBLock::~lpsrParallelMusicBLock ()
{}

void lpsrParallelMusicBLock::acceptIn (basevisitor* v)
{
  if (gLpsrOptions->fTraceLpsrVisitors) {
    gLogIOstream <<
      "% ==> lpsrParallelMusicBLock::acceptIn()" <<
      endl;
  }
      
  if (visitor<S_lpsrParallelMusicBLock>*
    p =
      dynamic_cast<visitor<S_lpsrParallelMusicBLock>*> (v)) {
        S_lpsrParallelMusicBLock elem = this;
        
        if (gLpsrOptions->fTraceLpsrVisitors) {
          gLogIOstream <<
            "% ==> Launching lpsrParallelMusicBLock::visitStart()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void lpsrParallelMusicBLock::acceptOut (basevisitor* v)
{
  if (gLpsrOptions->fTraceLpsrVisitors) {
    gLogIOstream <<
      "% ==> lpsrParallelMusicBLock::acceptOut()" <<
      endl;
  }

  if (visitor<S_lpsrParallelMusicBLock>*
    p =
      dynamic_cast<visitor<S_lpsrParallelMusicBLock>*> (v)) {
        S_lpsrParallelMusicBLock elem = this;
      
        if (gLpsrOptions->fTraceLpsrVisitors) {
          gLogIOstream <<
            "% ==> Launching lpsrParallelMusicBLock::visitEnd()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void lpsrParallelMusicBLock::browseData (basevisitor* v)
{
  if (gLpsrOptions->fTraceLpsrVisitors) {
    gLogIOstream <<
      "% ==> lpsrParallelMusicBLock::browseData()" <<
      endl;
  }
  
  for (
    list<S_lpsrPartGroupBlock>::const_iterator i =
      fParallelMusicBLockPartGroupBlocks.begin ();
    i != fParallelMusicBLockPartGroupBlocks.end ();
    i++) {
    // browse the element browser
    msrBrowser<msrElement> browser (v);
    browser.browse (*(*i));
  } // for

  if (gLpsrOptions->fTraceLpsrVisitors) {
    gLogIOstream <<
      "% <== lpsrParallelMusicBLock::browseData()" <<
      endl;
  }
}

ostream& operator<< (ostream& os, const S_lpsrParallelMusicBLock& elt)
{
  elt->print (os);
  return os;
}

void lpsrParallelMusicBLock::print (ostream& os)
{
  os <<
    "ParallelMusicBLock" <<
    ", " <<
    singularOrPlural (
      fParallelMusicBLockPartGroupBlocks.size (), "part group", "part groups") <<
    endl;
  
  gIndenter++;
  
  for (
    list<S_lpsrPartGroupBlock>::const_iterator i =
      fParallelMusicBLockPartGroupBlocks.begin ();
    i != fParallelMusicBLockPartGroupBlocks.end ();
    i++) {
    os << (*i);
  } // for

  gIndenter--;
}

//______________________________________________________________________________
S_lpsrComment lpsrComment::create (
  int                inputLineNumber,
  string             contents,
  lpsrCommentGapKind commentGapKind)
{
  lpsrComment* o = new
    lpsrComment (
      inputLineNumber, contents, commentGapKind);
  assert(o!=0);
  return o;
}

lpsrComment::lpsrComment (
  int                inputLineNumber,
  string             contents,
  lpsrCommentGapKind commentGapKind)
    : lpsrElement (inputLineNumber)
{
  fContents       = contents;
  fCommentGapKind = commentGapKind;
}

lpsrComment::~lpsrComment()
{}

void lpsrComment::acceptIn (basevisitor* v)
{
  if (gLpsrOptions->fTraceLpsrVisitors) {
    gLogIOstream <<
      "% ==> lpsrComment::acceptIn()" <<
      endl;
  }
      
  if (visitor<S_lpsrComment>*
    p =
      dynamic_cast<visitor<S_lpsrComment>*> (v)) {
        S_lpsrComment elem = this;
        
        if (gLpsrOptions->fTraceLpsrVisitors) {
          gLogIOstream <<
            "% ==> Launching lpsrComment::visitStart()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void lpsrComment::acceptOut (basevisitor* v)
{
  if (gLpsrOptions->fTraceLpsrVisitors) {
    gLogIOstream <<
      "% ==> lpsrComment::acceptOut()" <<
      endl;
  }

  if (visitor<S_lpsrComment>*
    p =
      dynamic_cast<visitor<S_lpsrComment>*> (v)) {
        S_lpsrComment elem = this;
      
        if (gLpsrOptions->fTraceLpsrVisitors) {
          gLogIOstream <<
            "% ==> Launching lpsrComment::visitEnd()" <<
            endl;
        }
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
  os <<
    "Comment" <<
    endl;
  
  gIndenter++;
  
  os <<
    "% " << fContents <<
    endl;

  if (fCommentGapKind == kGapAfterwards)
    os <<
      endl;
  
  gIndenter--;
}

//______________________________________________________________________________
S_lpsrBarNumberCheck lpsrBarNumberCheck::create (
  int                    inputLineNumber,
  int                    nextBarNumber)
{
  lpsrBarNumberCheck* o =
    new lpsrBarNumberCheck (
      inputLineNumber, nextBarNumber);
  assert(o!=0);
  return o;
}

lpsrBarNumberCheck::lpsrBarNumberCheck (
  int                    inputLineNumber,
  int                    nextBarNumber)
    : lpsrElement (inputLineNumber)
{
  fNextBarNumber=nextBarNumber; 
}

lpsrBarNumberCheck::~lpsrBarNumberCheck()
{}

void lpsrBarNumberCheck::acceptIn (basevisitor* v)
{
  if (gLpsrOptions->fTraceLpsrVisitors) {
    gLogIOstream <<
      "% ==> lpsrBarNumberCheck::acceptIn()" <<
      endl;
  }
      
  if (visitor<S_lpsrBarNumberCheck>*
    p =
      dynamic_cast<visitor<S_lpsrBarNumberCheck>*> (v)) {
        S_lpsrBarNumberCheck elem = this;
        
        if (gLpsrOptions->fTraceLpsrVisitors) {
          gLogIOstream <<
            "% ==> Launching lpsrBarNumberCheck::visitStart()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void lpsrBarNumberCheck::acceptOut (basevisitor* v)
{
  if (gLpsrOptions->fTraceLpsrVisitors) {
    gLogIOstream <<
      "% ==> lpsrBarNumberCheck::acceptOut()" <<
      endl;
  }

  if (visitor<S_lpsrBarNumberCheck>*
    p =
      dynamic_cast<visitor<S_lpsrBarNumberCheck>*> (v)) {
        S_lpsrBarNumberCheck elem = this;
      
        if (gLpsrOptions->fTraceLpsrVisitors) {
          gLogIOstream <<
            "% ==> Launching lpsrBarNumberCheck::visitEnd()" <<
            endl;
        }
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
  os <<
    "BarNumberCheck" << " " << fNextBarNumber <<
    endl;
}

//______________________________________________________________________________
S_lpsrUseVoiceCommand lpsrUseVoiceCommand::create (
  int            inputLineNumber,
  S_msrVoice     voice)
{
  lpsrUseVoiceCommand* o =
    new lpsrUseVoiceCommand (
      inputLineNumber,
      voice);
  assert(o!=0);
  return o;
}

lpsrUseVoiceCommand::lpsrUseVoiceCommand (
  int            inputLineNumber,
  S_msrVoice     voice)
    : lpsrElement (inputLineNumber)
{
  fVoice  = voice;
}

lpsrUseVoiceCommand::~lpsrUseVoiceCommand()
{}

void lpsrUseVoiceCommand::acceptIn (basevisitor* v)
{
  if (gLpsrOptions->fTraceLpsrVisitors) {
    gLogIOstream <<
      "% ==> lpsrUseVoiceCommand::acceptIn()" <<
      endl;
  }
      
  if (visitor<S_lpsrUseVoiceCommand>*
    p =
      dynamic_cast<visitor<S_lpsrUseVoiceCommand>*> (v)) {
        S_lpsrUseVoiceCommand elem = this;
        
        if (gLpsrOptions->fTraceLpsrVisitors) {
          gLogIOstream <<
            "% ==> Launching lpsrUseVoiceCommand::visitStart()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void lpsrUseVoiceCommand::acceptOut (basevisitor* v)
{
  if (gLpsrOptions->fTraceLpsrVisitors) {
    gLogIOstream <<
      "% ==> lpsrUseVoiceCommand::acceptOut()" <<
      endl;
  }

  if (visitor<S_lpsrUseVoiceCommand>*
    p =
      dynamic_cast<visitor<S_lpsrUseVoiceCommand>*> (v)) {
        S_lpsrUseVoiceCommand elem = this;
      
        if (gLpsrOptions->fTraceLpsrVisitors) {
          gLogIOstream <<
            "% ==> Launching lpsrUseVoiceCommand::visitEnd()" <<
            endl;
        }
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
    "UseVoiceCommand" << " \"" <<
    fVoice->getVoiceName () <<
    "\", " <<
    singularOrPlural (
      fVoice->getVoiceStanzasMap ().size (), "stanza", "stanzas") <<
    endl;
}

//______________________________________________________________________________
S_lpsrNewLyricsBlock lpsrNewLyricsBlock::create (
  int            inputLineNumber,
  S_msrStanza    stanza,
  S_msrVoice     voice)
{
  lpsrNewLyricsBlock* o =
    new lpsrNewLyricsBlock (
      inputLineNumber,
      stanza, voice);
  assert(o!=0);
  return o;
}

lpsrNewLyricsBlock::lpsrNewLyricsBlock (
  int            inputLineNumber,
  S_msrStanza    stanza,
  S_msrVoice     voice)
    : lpsrElement (inputLineNumber)
{
  fStanza = stanza;
  fVoice  = voice;
}

lpsrNewLyricsBlock::~lpsrNewLyricsBlock()
{}

void lpsrNewLyricsBlock::acceptIn (basevisitor* v)
{
  if (gLpsrOptions->fTraceLpsrVisitors) {
    gLogIOstream <<
      "% ==> lpsrNewLyricsBlock::acceptIn()" <<
      endl;
  }
      
  if (visitor<S_lpsrNewLyricsBlock>*
    p =
      dynamic_cast<visitor<S_lpsrNewLyricsBlock>*> (v)) {
        S_lpsrNewLyricsBlock elem = this;
        
        if (gLpsrOptions->fTraceLpsrVisitors) {
          gLogIOstream <<
            "% ==> Launching lpsrNewLyricsBlock::visitStart()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void lpsrNewLyricsBlock::acceptOut (basevisitor* v)
{
  if (gLpsrOptions->fTraceLpsrVisitors) {
    gLogIOstream <<
      "% ==> lpsrNewLyricsBlock::acceptOut()" <<
      endl;
  }

  if (visitor<S_lpsrNewLyricsBlock>*
    p =
      dynamic_cast<visitor<S_lpsrNewLyricsBlock>*> (v)) {
        S_lpsrNewLyricsBlock elem = this;
      
        if (gLpsrOptions->fTraceLpsrVisitors) {
          gLogIOstream <<
            "% ==> Launching lpsrNewLyricsBlock::visitEnd()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void lpsrNewLyricsBlock::browseData (basevisitor* v)
{}

ostream& operator<< (ostream& os, const S_lpsrNewLyricsBlock& nlc)
{
  nlc->print (os);
  return os;
}

void lpsrNewLyricsBlock::print (ostream& os)
{
  os <<
    "NewLyricsBlock" << " " <<
    fStanza->getStanzaName () << " " <<
    fVoice->getVoiceName () <<
    endl;
}

//______________________________________________________________________________
string const lpsrVarValAssoc::g_LilyPondVarValAssocNoUnit    = "";
string const lpsrVarValAssoc::g_LilyPondVarValAssocNoComment = "";

S_lpsrVarValAssoc lpsrVarValAssoc::create (
  int                     inputLineNumber,
  lpsrCommentedKind       commentedKind,
  lpsrBackSlashKind       backSlashKind,
  lpsrVarValAssocKind
                          lilyPondVarValAssocKind,
  lpsrVarValSeparatorKind varValSeparatorKind,
  lpsrQuotesKind          quotesKind,
  string                  value, 
  string                  unit,
  string                  comment,
  lpsrEndlKind            endlKind)
{
  lpsrVarValAssoc* o =
    new lpsrVarValAssoc(
      inputLineNumber,
      commentedKind,
      backSlashKind,
      lilyPondVarValAssocKind,
      varValSeparatorKind, 
      quotesKind,
      value,
      unit,
      comment,
      endlKind);
  assert(o!=0);
  return o;
}

lpsrVarValAssoc::lpsrVarValAssoc (
  int                     inputLineNumber,
  lpsrCommentedKind       commentedKind,
  lpsrBackSlashKind       backSlashKind,
  lpsrVarValAssocKind
                          lilyPondVarValAssocKind,
  lpsrVarValSeparatorKind varValSeparatorKind,
  lpsrQuotesKind          quotesKind,
  string                  value, 
  string                  unit,
  string                  comment,
  lpsrEndlKind            endlKind)
    : lpsrElement (inputLineNumber)
{
  fBackSlashKind       = backSlashKind;
  fLilyPondVarValAssocKind = lilyPondVarValAssocKind;
  fVarValSeparatorKind = varValSeparatorKind;
  fQuotesKind          = quotesKind;
  fVariableValue       = value;
  fCommentedKind       = commentedKind;
  fUnit                = unit;
  fComment             = comment;
  fEndlKind            = endlKind;
}

lpsrVarValAssoc::~lpsrVarValAssoc()
{}

string lpsrVarValAssoc::lilyPondVarValAssocKindAsString (
  lpsrVarValAssocKind lilyPondVarValAssocKind)
{
  string result;

  switch (lilyPondVarValAssocKind) {
    case lpsrVarValAssoc::kVersion:
      result = "version";
      break;
    case lpsrVarValAssoc::kWorkNumber:
      result = "workNumber";
      break;
    case lpsrVarValAssoc::kWorkTitle:
      result = "workTitle";
      break;
    case lpsrVarValAssoc::kMovementNumber:
      result = "movementNumber";
      break;
    case lpsrVarValAssoc::kMovementTitle:
      result = "movementTitle";
      break;
    case lpsrVarValAssoc::kEncodingDate:
      result = "encodingDate";
      break;
    case lpsrVarValAssoc::kScoreInstrument:
      result = "scoreInstrument";
      break;
    case lpsrVarValAssoc::kMiscellaneousField:
      result = "miscellaneousField";
      break;
    case lpsrVarValAssoc::kMyBreak:
      result = "myBreak";
      break;
    case lpsrVarValAssoc::kMyPageBreak:
      result = "myPageBreak";
      break;
    case lpsrVarValAssoc::kGlobal:
      result = "global";
      break;
  } // switch
  
  return result;
}

string lpsrVarValAssoc::commentedKindAsString (
  lpsrCommentedKind commentedKind)
{
  string result;

  switch (commentedKind) {
    case lpsrVarValAssoc::kCommented:
      result = "commented";
      break;
    case lpsrVarValAssoc::kUncommented:
      result = "uncommented";
      break;
  } // switch

  return result;
}

string lpsrVarValAssoc::backSlashKindAsString (
  lpsrBackSlashKind backSlashKind)
{
  string result;

  switch (backSlashKind) {
    case lpsrVarValAssoc::kWithBackSlash:
      result = "with backSlash";
      break;
    case lpsrVarValAssoc::kWithoutBackSlash:
      result = "without backSlash";
      break;
  } // switch

  return result;
}

string lpsrVarValAssoc::varValSeparatorKindAsString (
  lpsrVarValSeparatorKind varValSeparatorKind)
{
  string result;

  switch (varValSeparatorKind) {
    case lpsrVarValAssoc::kSpace:
      result = "space";
      break;
    case lpsrVarValAssoc::kEqualSign:
      result = "equal";
      break;
  } // switch

  return result;
}

string lpsrVarValAssoc::quotesKindAsString (
  lpsrQuotesKind quotesKind)
{
  string result;

  switch (quotesKind) {
    case lpsrVarValAssoc::kQuotesAroundValue:
      result = "quotes";
      break;
    case lpsrVarValAssoc::kNoQuotesAroundValue:
      result = "noQuotes";
      break;
  } // switch

  return result;
}

string lpsrVarValAssoc::endlKindAsString (
  lpsrEndlKind endlKind)
{
  string result;

  switch (endlKind) {
    case lpsrVarValAssoc::kWithEndl:
      result = "with end line";
      break;
    case lpsrVarValAssoc::kWithEndlTwice:
      result = "with end line twice";
      break;
    case lpsrVarValAssoc::kWithoutEndl:
      result = "without end line";
      break;
  } // switch

  return result;
}

void lpsrVarValAssoc::acceptIn (basevisitor* v)
{
  if (gLpsrOptions->fTraceLpsrVisitors) {
    gLogIOstream <<
      "% ==> lpsrVarValAssoc::acceptIn()" <<
      endl;
  }
      
  if (visitor<S_lpsrVarValAssoc>*
    p =
      dynamic_cast<visitor<S_lpsrVarValAssoc>*> (v)) {
        S_lpsrVarValAssoc elem = this;
        
        if (gLpsrOptions->fTraceLpsrVisitors) {
          gLogIOstream <<
            "% ==> Launching lpsrVarValAssoc::visitStart()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void lpsrVarValAssoc::acceptOut (basevisitor* v)
{
  if (gLpsrOptions->fTraceLpsrVisitors) {
    gLogIOstream <<
      "% ==> lpsrVarValAssoc::acceptOut()" <<
      endl;
  }

  if (visitor<S_lpsrVarValAssoc>*
    p =
      dynamic_cast<visitor<S_lpsrVarValAssoc>*> (v)) {
        S_lpsrVarValAssoc elem = this;
      
        if (gLpsrOptions->fTraceLpsrVisitors) {
          gLogIOstream <<
            "% ==> Launching lpsrVarValAssoc::visitEnd()" <<
            endl;
        }
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
  os <<
    "LPSR VarValAssoc" <<
    endl;
  
  gIndenter++;

  // escape quotes if any
  string variableValue;

  for_each (
    fVariableValue.begin (),
    fVariableValue.end (),
    stringQuoteEscaper (variableValue));

  const int fieldWidth = 16;

  os << left <<
    setw (fieldWidth) <<
    "assoc kind" << " : " <<
    lilyPondVarValAssocKindAsString () <<
    endl <<
  
    setw (fieldWidth) <<
    "assoc value" << " : \"" <<
    variableValue <<
    "\"" <<
    endl <<
  
    setw (fieldWidth) <<
   "commented kind" << " : " <<
    commentedKindAsString (
      fCommentedKind) <<
    endl <<

    setw (fieldWidth) <<
   "back slash kind" << " : " <<
    backSlashKindAsString (
      fBackSlashKind) <<
    endl <<

    setw (fieldWidth) <<
   "separator kind" << " : " <<
    varValSeparatorKindAsString (
      fVarValSeparatorKind) <<
    endl <<

    setw (fieldWidth) <<
   "quotes kind" << " : " <<
    quotesKindAsString (
      fQuotesKind) <<
    endl <<

    setw (fieldWidth) <<
   "unit" << " : ";
    
  if (fUnit.size ())
    os << "\"" << fUnit << "\"";
  else
    os << "none";
  os <<
    endl;
  
  os <<
    setw (fieldWidth) <<
   "comment" << " : ";

  if (fComment.size ())
    os << "\"" << fComment << "\"";
  else
    os << "none";
  os <<
    endl;

  os <<
    setw (fieldWidth) <<
   "endl kind" << " : " <<
    endlKindAsString (
      fEndlKind) <<
    endl;
  
  gIndenter--;
}

//______________________________________________________________________________
S_lpsrVarValsListAssoc lpsrVarValsListAssoc::create (
  int                      inputLineNumber,
  lpsrVarValsListAssocKind varValsListAssocKind)
{
  lpsrVarValsListAssoc* o =
    new lpsrVarValsListAssoc (
      inputLineNumber,
      varValsListAssocKind);
  assert(o!=0);
  return o;
}

lpsrVarValsListAssoc::lpsrVarValsListAssoc (
  int                      inputLineNumber,
  lpsrVarValsListAssocKind varValsListAssocKind)
    : lpsrElement (inputLineNumber)
{
  fVarValsListAssocKind = varValsListAssocKind;
}

lpsrVarValsListAssoc::~lpsrVarValsListAssoc ()
{}

string lpsrVarValsListAssoc::lilyPondVarValsListAssocValuesAsString (
  lpsrVarValsListAssocKind varValsListAssocKind)
{
  string result;

  switch (varValsListAssocKind) {
    case lpsrVarValsListAssoc::kRights:
      result = "rights";
      break;
    case lpsrVarValsListAssoc::kComposer:
      result = "composer";
      break;
    case lpsrVarValsListAssoc::kArranger:
      result = "arranger";
      break;
    case lpsrVarValsListAssoc::kPoet:
      result = "poet";
      break;
    case lpsrVarValsListAssoc::kLyricist:
      result = "lyricist";
      break;
    case lpsrVarValsListAssoc::kSoftware:
      result = "software";
      break;
  } // switch
  
  return result;
}

string lpsrVarValsListAssoc::lilyPondVarValsListAssocValuesAsString () const
{
  stringstream s;
  
  list<string>::const_iterator
    iBegin = fVariableValuesList.begin (),
    iEnd   = fVariableValuesList.end (),
    i      = iBegin;

  s << "[";
  for ( ; ; ) {
    s << "\"" << (*i) << "\"";
    if (++i == iEnd) break;
    s << ", ";
  } // for
  s << "]";
  
  return s.str ();
}

void lpsrVarValsListAssoc::acceptIn (basevisitor* v)
{
  if (gLpsrOptions->fTraceLpsrVisitors) {
    gLogIOstream <<
      "% ==> lpsrVarValsListAssoc::acceptIn()" <<
      endl;
  }
      
  if (visitor<S_lpsrVarValsListAssoc>*
    p =
      dynamic_cast<visitor<S_lpsrVarValsListAssoc>*> (v)) {
        S_lpsrVarValsListAssoc elem = this;
        
        if (gLpsrOptions->fTraceLpsrVisitors) {
          gLogIOstream <<
            "% ==> Launching lpsrVarValsListAssoc::visitStart()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void lpsrVarValsListAssoc::acceptOut (basevisitor* v)
{
  if (gLpsrOptions->fTraceLpsrVisitors) {
    gLogIOstream <<
      "% ==> lpsrVarValsListAssoc::acceptOut()" <<
      endl;
  }

  if (visitor<S_lpsrVarValsListAssoc>*
    p =
      dynamic_cast<visitor<S_lpsrVarValsListAssoc>*> (v)) {
        S_lpsrVarValsListAssoc elem = this;
      
        if (gLpsrOptions->fTraceLpsrVisitors) {
          gLogIOstream <<
            "% ==> Launching lpsrVarValsListAssoc::visitEnd()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void lpsrVarValsListAssoc::browseData (basevisitor* v)
{}

ostream& operator<< (ostream& os, const S_lpsrVarValsListAssoc& assoc) {
  assoc->print (os);
  return os;
}

void lpsrVarValsListAssoc::print (ostream& os)
{
  os <<
    "LPSR VarValsListAssoc" <<
    endl;
  
  gIndenter++;

  const int fieldWidth = 16;

  os << left <<
    setw (fieldWidth) <<
    "assoc kind" << " : " <<
    lilyPondVarValsListAssocKindAsString () <<
    endl <<
        
    "variable values" << " : " <<
    endl;

  gIndenter++;
  
  list<string>::const_iterator
    iBegin = fVariableValuesList.begin (),
    iEnd   = fVariableValuesList.end (),
    i      = iBegin;
    
  for ( ; ; ) {
    os << "\"" << (*i) << "\"";
    if (++i == iEnd) break;
    os << endl;
  } // for
  
  os <<
    endl;

  gIndenter--;
  
  gIndenter--;
}

//______________________________________________________________________________
string const lpsrSchemeVariable::g_SchemeVariableNoUnit    = "";
string const lpsrSchemeVariable::g_SchemeVariableNoComment = "";

S_lpsrSchemeVariable lpsrSchemeVariable::create (
  int               inputLineNumber,
  lpsrCommentedKind commentedKind,
  string            variableName,
  string            value, 
  string            comment,
  lpsrEndlKind      endlKind)
{
  lpsrSchemeVariable* o =
    new lpsrSchemeVariable (
      inputLineNumber,
      commentedKind, variableName, value, comment, endlKind);
  assert(o!=0);
  return o;
}

lpsrSchemeVariable::lpsrSchemeVariable (
  int               inputLineNumber,
  lpsrCommentedKind commentedKind,
  string            variableName,
  string            value, 
  string            comment,
  lpsrEndlKind      endlKind)
    : lpsrElement (inputLineNumber)
{
  fCommentedKind = commentedKind;

  fVariableName  = variableName;
  fVariableValue = value;

  fComment       = comment;
  
  fEndlKind      = endlKind;
}

lpsrSchemeVariable::~lpsrSchemeVariable()
{}

string lpsrSchemeVariable::commentedKindAsString (
  lpsrCommentedKind commentedKind)
{
  string result;

  switch (commentedKind) {
    case lpsrVarValAssoc::kCommented:
      result = "commented";
      break;
    case lpsrVarValAssoc::kUncommented:
      result = "uncommented";
      break;
  } // switch

  return result;
}

string lpsrSchemeVariable::endlKindAsString (
  lpsrEndlKind endlKind)
{
  string result;

  switch (endlKind) {
    case lpsrVarValAssoc::kWithEndl:
      result = "with end line";
      break;
    case lpsrVarValAssoc::kWithEndlTwice:
      result = "with end line twice";
      break;
    case lpsrVarValAssoc::kWithoutEndl:
      result = "without end line";
      break;
  } // switch

  return result;
}

void lpsrSchemeVariable::acceptIn (basevisitor* v)
{
  if (gLpsrOptions->fTraceLpsrVisitors) {
    gLogIOstream <<
      "% ==> lpsrSchemeVariable::acceptIn()" <<
      endl;
  }
      
  if (visitor<S_lpsrSchemeVariable>*
    p =
      dynamic_cast<visitor<S_lpsrSchemeVariable>*> (v)) {
        S_lpsrSchemeVariable elem = this;
        
        if (gLpsrOptions->fTraceLpsrVisitors) {
          gLogIOstream <<
            "% ==> Launching lpsrSchemeVariable::visitStart()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void lpsrSchemeVariable::acceptOut (basevisitor* v)
{
  if (gLpsrOptions->fTraceLpsrVisitors) {
    gLogIOstream <<
      "% ==> lpsrSchemeVariable::acceptOut()" <<
      endl;
  }

  if (visitor<S_lpsrSchemeVariable>*
    p =
      dynamic_cast<visitor<S_lpsrSchemeVariable>*> (v)) {
        S_lpsrSchemeVariable elem = this;
      
        if (gLpsrOptions->fTraceLpsrVisitors) {
          gLogIOstream <<
            "% ==> Launching lpsrSchemeVariable::visitEnd()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void lpsrSchemeVariable::browseData (basevisitor* v)
{}

ostream& operator<< (ostream& os, const S_lpsrSchemeVariable& assoc)
{
  assoc->print (os);
  return os;
}

void lpsrSchemeVariable::print (ostream& os)
{
  os <<
    "SchemeVariable" <<
    endl;

  gIndenter++;

  // escape quotes if any
  string variableName;
  string variableValue;

  for_each (
    fVariableName.begin (),
    fVariableName.end (),
    stringQuoteEscaper (variableName));
  for_each (
    fVariableValue.begin (),
    fVariableValue.end (),
    stringQuoteEscaper (variableValue));

  // print resulting strings
  const int fieldWidth = 15;

  os << left <<
    setw (fieldWidth) <<
    "variable name" <<
    " : \"" << variableName << "\"" <<
    endl <<
    setw (fieldWidth) <<
    "variable value" <<
    " : \"" << variableValue << "\"" <<
    endl <<

    setw (fieldWidth) <<
    "commented kind" << " : " <<
    commentedKindAsString (fCommentedKind) <<
    endl <<

  // backSlashKindAsString ??? JMI
  // varValSeparatorKindAsString ??? JMI
  // quotesKindAsString ??? JMI
  
    setw (fieldWidth) <<
    "endl kind" << " : " <<
    endlKindAsString (fEndlKind) <<
    endl;
  
  gIndenter--;
}

//______________________________________________________________________________
S_lpsrSchemeFunction lpsrSchemeFunction::create (
  int    inputLineNumber,
  string functionName,
  string functionDescription,
  string functionCode)
{
  lpsrSchemeFunction* o =
    new lpsrSchemeFunction (
      inputLineNumber,
      functionName, functionDescription, functionCode);
  assert(o!=0);
  return o;
}

lpsrSchemeFunction::lpsrSchemeFunction (
  int    inputLineNumber,
  string functionName,
  string functionDescription,
  string functionCode)
    : lpsrElement (inputLineNumber)
{
  fFunctionName        = functionName;
  fFunctionDescription = functionDescription;
  fFunctionCode        = functionCode;
  }

lpsrSchemeFunction::~lpsrSchemeFunction()
{}

void lpsrSchemeFunction::acceptIn (basevisitor* v)
{
  if (gLpsrOptions->fTraceLpsrVisitors) {
    gLogIOstream <<
      "% ==> lpsrSchemeFunction::acceptIn()" <<
      endl;
  }
      
  if (visitor<S_lpsrSchemeFunction>*
    p =
      dynamic_cast<visitor<S_lpsrSchemeFunction>*> (v)) {
        S_lpsrSchemeFunction elem = this;
        
        if (gLpsrOptions->fTraceLpsrVisitors) {
          gLogIOstream <<
            "% ==> Launching lpsrSchemeFunction::visitStart()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void lpsrSchemeFunction::acceptOut (basevisitor* v)
{
  if (gLpsrOptions->fTraceLpsrVisitors) {
    gLogIOstream <<
      "% ==> lpsrSchemeFunction::acceptOut()" <<
      endl;
  }

  if (visitor<S_lpsrSchemeFunction>*
    p =
      dynamic_cast<visitor<S_lpsrSchemeFunction>*> (v)) {
        S_lpsrSchemeFunction elem = this;
      
        if (gLpsrOptions->fTraceLpsrVisitors) {
          gLogIOstream <<
            "% ==> Launching lpsrSchemeFunction::visitEnd()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void lpsrSchemeFunction::browseData (basevisitor* v)
{}

ostream& operator<< (ostream& os, const S_lpsrSchemeFunction& assoc)
{
  assoc->print (os);
  return os;
}

void lpsrSchemeFunction::print (ostream& os)
{
  os <<
    "SchemeFunction" <<
    endl;

  gIndenter++;

  // print resulting strings
  os <<
    "function name : \"" << fFunctionName << "\"" <<
    endl <<
    "function description: \"" << fFunctionDescription << "\"" <<
    endl <<
    "function code: \"" << fFunctionCode << "\"" <<
    endl;

  gIndenter--;
}

//______________________________________________________________________________
S_lpsrNewStaffgroupBlock lpsrNewStaffgroupBlock::create (
  int            inputLineNumber)
{
  lpsrNewStaffgroupBlock* o =
    new lpsrNewStaffgroupBlock (
      inputLineNumber);
  assert(o!=0);
  return o;
}

lpsrNewStaffgroupBlock::lpsrNewStaffgroupBlock (
  int            inputLineNumber)
    : lpsrElement (inputLineNumber)
{}

lpsrNewStaffgroupBlock::~lpsrNewStaffgroupBlock()
{}

void lpsrNewStaffgroupBlock::acceptIn (basevisitor* v)
{
  if (gLpsrOptions->fTraceLpsrVisitors) {
    gLogIOstream <<
      "% ==> lpsrNewStaffgroupBlock::acceptIn()" <<
      endl;
  }
      
  if (visitor<S_lpsrNewStaffgroupBlock>*
    p =
      dynamic_cast<visitor<S_lpsrNewStaffgroupBlock>*> (v)) {
        S_lpsrNewStaffgroupBlock elem = this;
        
        if (gLpsrOptions->fTraceLpsrVisitors) {
          gLogIOstream <<
            "% ==> Launching lpsrNewStaffgroupBlock::visitStart()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void lpsrNewStaffgroupBlock::acceptOut (basevisitor* v)
{
  if (gLpsrOptions->fTraceLpsrVisitors) {
    gLogIOstream <<
      "% ==> lpsrNewStaffgroupBlock::acceptOut()" <<
      endl;
  }

  if (visitor<S_lpsrNewStaffgroupBlock>*
    p =
      dynamic_cast<visitor<S_lpsrNewStaffgroupBlock>*> (v)) {
        S_lpsrNewStaffgroupBlock elem = this;
      
        if (gLpsrOptions->fTraceLpsrVisitors) {
          gLogIOstream <<
            "% ==> Launching lpsrNewStaffgroupBlock::visitEnd()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void lpsrNewStaffgroupBlock::browseData (basevisitor* v)
{}

ostream& operator<< (ostream& os, const S_lpsrNewStaffgroupBlock& nstf)
{
  nstf->print (os);
  return os;
}

void lpsrNewStaffgroupBlock::print (ostream& os)
{
  os << "NewStaffgroupBlock" << endl;

  gIndenter++;
  
  int size = fNewStaffgroupElements.size ();

  for (int i = 0; i < size; i++ ) {
    os << fNewStaffgroupElements[i];
  } // for
  
  gIndenter--;
}

//______________________________________________________________________________
S_lpsrNewStaffTuningBlock lpsrNewStaffTuningBlock::create (
  int              inputLineNumber,
  S_msrStaffTuning staffTuning)
{
  lpsrNewStaffTuningBlock* o =
    new lpsrNewStaffTuningBlock (
      inputLineNumber, staffTuning);
  assert(o!=0);
  return o;
}

lpsrNewStaffTuningBlock::lpsrNewStaffTuningBlock (
  int              inputLineNumber,
  S_msrStaffTuning staffTuning)
    : lpsrElement (inputLineNumber)
{
  fStaffTuning = staffTuning;
}

lpsrNewStaffTuningBlock::~lpsrNewStaffTuningBlock()
{}

void lpsrNewStaffTuningBlock::acceptIn (basevisitor* v)
{
  if (gLpsrOptions->fTraceLpsrVisitors) {
    gLogIOstream <<
      "% ==> lpsrNewStaffTuningBlock::acceptIn()" <<
      endl;
  }
      
  if (visitor<S_lpsrNewStaffTuningBlock>*
    p =
      dynamic_cast<visitor<S_lpsrNewStaffTuningBlock>*> (v)) {
        S_lpsrNewStaffTuningBlock elem = this;
        
        if (gLpsrOptions->fTraceLpsrVisitors) {
          gLogIOstream <<
            "% ==> Launching lpsrNewStaffTuningBlock::visitStart()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void lpsrNewStaffTuningBlock::acceptOut (basevisitor* v)
{
  if (gLpsrOptions->fTraceLpsrVisitors) {
    gLogIOstream <<
      "% ==> lpsrNewStaffTuningBlock::acceptOut()" <<
      endl;
  }

  if (visitor<S_lpsrNewStaffTuningBlock>*
    p =
      dynamic_cast<visitor<S_lpsrNewStaffTuningBlock>*> (v)) {
        S_lpsrNewStaffTuningBlock elem = this;
      
        if (gLpsrOptions->fTraceLpsrVisitors) {
          gLogIOstream <<
            "% ==> Launching lpsrNewStaffTuningBlock::visitEnd()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void lpsrNewStaffTuningBlock::browseData (basevisitor* v)
{}

ostream& operator<< (ostream& os, const S_lpsrNewStaffTuningBlock& nstf)
{
  nstf->print (os);
  return os;
}

void lpsrNewStaffTuningBlock::print (ostream& os)
{
  os << "NewStaffTuningBlock" << endl;

  gIndenter++;
  
  os <<
    fStaffTuning;
  
  gIndenter--;
}

//______________________________________________________________________________
S_lpsrNewStaffBlock lpsrNewStaffBlock::create (
  int            inputLineNumber)
{
  lpsrNewStaffBlock* o =
    new lpsrNewStaffBlock (
      inputLineNumber);
  assert(o!=0);
  return o;
}

lpsrNewStaffBlock::lpsrNewStaffBlock (
  int            inputLineNumber)
    : lpsrElement (inputLineNumber)
{}

lpsrNewStaffBlock::~lpsrNewStaffBlock()
{}

void lpsrNewStaffBlock::acceptIn (basevisitor* v)
{
  if (gLpsrOptions->fTraceLpsrVisitors) {
    gLogIOstream <<
      "% ==> lpsrNewStaffBlock::acceptIn()" <<
      endl;
  }
      
  if (visitor<S_lpsrNewStaffBlock>*
    p =
      dynamic_cast<visitor<S_lpsrNewStaffBlock>*> (v)) {
        S_lpsrNewStaffBlock elem = this;
        
        if (gLpsrOptions->fTraceLpsrVisitors) {
          gLogIOstream <<
            "% ==> Launching lpsrNewStaffBlock::visitStart()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void lpsrNewStaffBlock::acceptOut (basevisitor* v)
{
  if (gLpsrOptions->fTraceLpsrVisitors) {
    gLogIOstream <<
      "% ==> lpsrNewStaffBlock::acceptOut()" <<
      endl;
  }

  if (visitor<S_lpsrNewStaffBlock>*
    p =
      dynamic_cast<visitor<S_lpsrNewStaffBlock>*> (v)) {
        S_lpsrNewStaffBlock elem = this;
      
        if (gLpsrOptions->fTraceLpsrVisitors) {
          gLogIOstream <<
            "% ==> Launching lpsrNewStaffBlock::visitEnd()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void lpsrNewStaffBlock::browseData (basevisitor* v)
{}

ostream& operator<< (ostream& os, const S_lpsrNewStaffBlock& nstf)
{
  nstf->print (os);
  return os;
}

void lpsrNewStaffBlock::print (ostream& os)
{
  os << "NewStaffBlock" << endl;

  gIndenter++;
  
  int size = fNewStaffElements.size ();

  for (int i = 0; i < size; i++ ) {
    os << fNewStaffElements[i];
  } // for
  
  gIndenter--;
}

//______________________________________________________________________________
S_lpsrVariableUseCommand lpsrVariableUseCommand::create (
  int            inputLineNumber,
  string         variableName)
{
  lpsrVariableUseCommand* o =
    new lpsrVariableUseCommand (
      inputLineNumber, variableName);
  assert(o!=0);
  return o;
}

lpsrVariableUseCommand::lpsrVariableUseCommand (
  int            inputLineNumber,
  string         variableName)
    : lpsrElement (inputLineNumber)
{
  fVariableName = variableName;
}

lpsrVariableUseCommand::~lpsrVariableUseCommand()
{}

void lpsrVariableUseCommand::acceptIn (basevisitor* v)
{
  if (gLpsrOptions->fTraceLpsrVisitors) {
    gLogIOstream <<
      "% ==> lpsrVariableUseCommand::acceptIn()" <<
      endl;
  }
      
  if (visitor<S_lpsrVariableUseCommand>*
    p =
      dynamic_cast<visitor<S_lpsrVariableUseCommand>*> (v)) {
        S_lpsrVariableUseCommand elem = this;
        
        if (gLpsrOptions->fTraceLpsrVisitors) {
          gLogIOstream <<
            "% ==> Launching lpsrVariableUseCommand::visitStart()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void lpsrVariableUseCommand::acceptOut (basevisitor* v)
{
  if (gLpsrOptions->fTraceLpsrVisitors) {
    gLogIOstream <<
      "% ==> lpsrVariableUseCommand::acceptOut()" <<
      endl;
  }

  if (visitor<S_lpsrVariableUseCommand>*
    p =
      dynamic_cast<visitor<S_lpsrVariableUseCommand>*> (v)) {
        S_lpsrVariableUseCommand elem = this;
      
        if (gLpsrOptions->fTraceLpsrVisitors) {
          gLogIOstream <<
            "% ==> Launching lpsrVariableUseCommand::visitEnd()" <<
            endl;
        }
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
  gIndenter++;
  os << fVariableName << endl;
  gIndenter--;
}

//______________________________________________________________________________
S_lpsrContext lpsrContext::create (
  int             inputLineNumber,
  lpsrContextExistingKind
                  contextExistingKind,
  lpsrContextTypeKind
                  contextTypeKind,
  string          contextName)
{
  lpsrContext* o =
    new lpsrContext (
      inputLineNumber,
      contextExistingKind,
      contextTypeKind,
      contextName);
  assert(o!=0);
  return o;
}

lpsrContext::lpsrContext (
  int             inputLineNumber,
  lpsrContextExistingKind
                  contextExistingKind,
  lpsrContextTypeKind
                  contextTypeKind,
  string          contextName)
    : lpsrElement (inputLineNumber)
{
  fContextExistingKind = contextExistingKind;
  fContextTypeKind = contextTypeKind;
  fContextName = contextName; 
}

lpsrContext::~lpsrContext()
{}

void lpsrContext::acceptIn (basevisitor* v)
{
  if (gLpsrOptions->fTraceLpsrVisitors) {
    gLogIOstream <<
      "% ==> lpsrContext::acceptIn()" <<
      endl;
  }
      
  if (visitor<S_lpsrContext>*
    p =
      dynamic_cast<visitor<S_lpsrContext>*> (v)) {
        S_lpsrContext elem = this;
        
        if (gLpsrOptions->fTraceLpsrVisitors) {
          gLogIOstream <<
            "% ==> Launching lpsrContext::visitStart()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void lpsrContext::acceptOut (basevisitor* v)
{
  if (gLpsrOptions->fTraceLpsrVisitors) {
    gLogIOstream <<
      "% ==> lpsrContext::acceptOut()" <<
      endl;
  }

  if (visitor<S_lpsrContext>*
    p =
      dynamic_cast<visitor<S_lpsrContext>*> (v)) {
        S_lpsrContext elem = this;
      
        if (gLpsrOptions->fTraceLpsrVisitors) {
          gLogIOstream <<
            "% ==> Launching lpsrContext::visitEnd()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void lpsrContext::browseData (basevisitor* v)
{}

string lpsrContext::contextTypeKindAsString (
  lpsrContextTypeKind contextTypeKind)
{
  string result;
  
  switch (contextTypeKind) {
    case kChordNames:
      result = "ChordNames";
      break;
    case kFiguredBass:
      result = "FiguredBass";
      break;
  } // switch

  return result;
}
  
string lpsrContext::contextExistingKindAsString (
  lpsrContextExistingKind contextExistingKind)
{
  string result;
  
  switch (contextExistingKind) {
    case kExistingContextYes:
      result = "existing: yes";
      break;
      
    case kExistingContextNo:
      result = "existing: no";
      break;
  } // switch

  return result;
}

void lpsrContext::print (ostream& os)
{  
  os <<
    "Context, \"" <<
    contextTypeKindAsString (
      fContextTypeKind) <<
    "\"" <<
    ", existing kind: " <<
    contextExistingKindAsString (
      fContextExistingKind);
    
  if (fContextName.size ())
    os << " " << fContextName;
  os <<
  endl;
  
  gIndenter++;

  int size = fContextElements.size ();

  for (int i = 0; i < size; i++ ) {
    os << fContextElements[i];
  } // for

  gIndenter--;
}

//______________________________________________________________________________
S_lpsrBarCommand lpsrBarCommand::create (
  int            inputLineNumber)
{
  lpsrBarCommand* o =
    new lpsrBarCommand (
      inputLineNumber);
  assert(o!=0);
  return o;
}

lpsrBarCommand::lpsrBarCommand (
  int            inputLineNumber)
    : lpsrElement (inputLineNumber)
{}

lpsrBarCommand::~lpsrBarCommand()
{}

void lpsrBarCommand::acceptIn (basevisitor* v)
{
  if (gLpsrOptions->fTraceLpsrVisitors) {
    gLogIOstream <<
      "% ==> lpsrBarCommand::acceptIn()" <<
      endl;
  }
      
  if (visitor<S_lpsrBarCommand>*
    p =
      dynamic_cast<visitor<S_lpsrBarCommand>*> (v)) {
        S_lpsrBarCommand elem = this;
        
        if (gLpsrOptions->fTraceLpsrVisitors) {
          gLogIOstream <<
            "% ==> Launching lpsrBarCommand::visitStart()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void lpsrBarCommand::acceptOut (basevisitor* v)
{
  if (gLpsrOptions->fTraceLpsrVisitors) {
    gLogIOstream <<
      "% ==> lpsrBarCommand::acceptOut()" <<
      endl;
  }

  if (visitor<S_lpsrBarCommand>*
    p =
      dynamic_cast<visitor<S_lpsrBarCommand>*> (v)) {
        S_lpsrBarCommand elem = this;
      
        if (gLpsrOptions->fTraceLpsrVisitors) {
          gLogIOstream <<
            "% ==> Launching lpsrBarCommand::visitEnd()" <<
            endl;
        }
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
  os <<
    "BarCommand" << " " << "\"|.\"" <<
    endl;
}

//______________________________________________________________________________
S_lpsrMelismaCommand lpsrMelismaCommand::create (
  int             inputLineNumber,
  lpsrMelismaKind melismaKind)
{
  lpsrMelismaCommand* o =
    new lpsrMelismaCommand (
      inputLineNumber, melismaKind);
  assert(o!=0);
  return o;
}

lpsrMelismaCommand::lpsrMelismaCommand (
  int             inputLineNumber,
  lpsrMelismaKind melismaKind)
    : lpsrElement (inputLineNumber)
{
  if (gGeneralOptions->fTraceLyrics) {
    gLogIOstream <<
      "--> creating lpsrMelismaCommand " <<
      melismaKindAsString (melismaKind) <<
      endl;
  }
  
  fMelismaKind = melismaKind;
}

lpsrMelismaCommand::~lpsrMelismaCommand()
{}

void lpsrMelismaCommand::acceptIn (basevisitor* v)
{
  if (gLpsrOptions->fTraceLpsrVisitors) {
    gLogIOstream <<
      "% ==> lpsrMelismaCommand::acceptIn()" <<
      endl;
  }
      
  if (visitor<S_lpsrMelismaCommand>*
    p =
      dynamic_cast<visitor<S_lpsrMelismaCommand>*> (v)) {
        S_lpsrMelismaCommand elem = this;
        
        if (gLpsrOptions->fTraceLpsrVisitors) {
          gLogIOstream <<
            "% ==> Launching lpsrMelismaCommand::visitStart()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void lpsrMelismaCommand::acceptOut (basevisitor* v)
{
  if (gLpsrOptions->fTraceLpsrVisitors) {
    gLogIOstream <<
      "% ==> lpsrMelismaCommand::acceptOut()" <<
      endl;
  }

  if (visitor<S_lpsrMelismaCommand>*
    p =
      dynamic_cast<visitor<S_lpsrMelismaCommand>*> (v)) {
        S_lpsrMelismaCommand elem = this;
      
        if (gLpsrOptions->fTraceLpsrVisitors) {
          gLogIOstream <<
            "% ==> Launching lpsrMelismaCommand::visitEnd()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void lpsrMelismaCommand::browseData (basevisitor* v)
{}

ostream& operator<< (ostream& os, const S_lpsrMelismaCommand& nstf)
{
  nstf->print (os);
  return os;
}

string lpsrMelismaCommand::melismaKindAsString (
  lpsrMelismaKind melismaKind)
{
  string result;

  switch (melismaKind) {
    case lpsrMelismaCommand::kMelismaStart:
      result = "Start";
      break;
    case lpsrMelismaCommand::kMelismaEnd:
      result = "End";
      break;
  } // switch

  return result;
}

void lpsrMelismaCommand::print (ostream& os)
{
  os <<
    "MelismaCommand" << " " <<
    melismaKindAsString (fMelismaKind) <<
    endl;
}

//______________________________________________________________________________
S_lpsrHeader lpsrHeader::create (
  int            inputLineNumber)
{
  lpsrHeader* o =
    new lpsrHeader (
      inputLineNumber);
  assert(o!=0);
  return o;
}

lpsrHeader::lpsrHeader (
  int            inputLineNumber)
    : lpsrElement (inputLineNumber)
{}

lpsrHeader::~lpsrHeader()
{}

void lpsrHeader::setWorkNumber (
  int    inputLineNumber,
  string val)
{
 fWorkNumber =
    lpsrVarValAssoc::create (
      inputLineNumber,
      lpsrVarValAssoc::kUncommented,
      lpsrVarValAssoc::kWithoutBackSlash,
      lpsrVarValAssoc::kWorkNumber,
      lpsrVarValAssoc::kEqualSign,
      lpsrVarValAssoc::kQuotesAroundValue,
      val,
      lpsrVarValAssoc::g_LilyPondVarValAssocNoUnit,
      lpsrVarValAssoc::g_LilyPondVarValAssocNoComment,
      lpsrVarValAssoc::kWithoutEndl);
}

void lpsrHeader::setWorkTitle (
  int    inputLineNumber,
  string val)
  {
  fWorkTitle =
    lpsrVarValAssoc::create (
      inputLineNumber,
      lpsrVarValAssoc::kUncommented,
      lpsrVarValAssoc::kWithoutBackSlash,
      lpsrVarValAssoc::kWorkTitle,
      lpsrVarValAssoc::kEqualSign,
      lpsrVarValAssoc::kQuotesAroundValue,
      val,
      lpsrVarValAssoc::g_LilyPondVarValAssocNoUnit,
      lpsrVarValAssoc::g_LilyPondVarValAssocNoComment,
      lpsrVarValAssoc::kWithoutEndl);
  }

void lpsrHeader::setMovementNumber (
  int    inputLineNumber,
  string val)
  {
  fMovementNumber =
    lpsrVarValAssoc::create (
      inputLineNumber,
      lpsrVarValAssoc::kUncommented,
      lpsrVarValAssoc::kWithoutBackSlash,
      lpsrVarValAssoc::kMovementNumber,
      lpsrVarValAssoc::kEqualSign,
      lpsrVarValAssoc::kQuotesAroundValue,
      val,
      lpsrVarValAssoc::g_LilyPondVarValAssocNoUnit,
      lpsrVarValAssoc::g_LilyPondVarValAssocNoComment,
      lpsrVarValAssoc::kWithoutEndl);
  }

void lpsrHeader::setMovementTitle (
  int    inputLineNumber,
  string val)
{
  fMovementTitle =
    lpsrVarValAssoc::create (
      inputLineNumber,
      lpsrVarValAssoc::kUncommented,
      lpsrVarValAssoc::kWithoutBackSlash,
      lpsrVarValAssoc::kMovementTitle,
      lpsrVarValAssoc::kEqualSign,
      lpsrVarValAssoc::kQuotesAroundValue,
      val,
      lpsrVarValAssoc::g_LilyPondVarValAssocNoUnit,
      lpsrVarValAssoc::g_LilyPondVarValAssocNoComment,
      lpsrVarValAssoc::kWithoutEndl);
}

void lpsrHeader::setEncodingDate (
  int    inputLineNumber,
  string val)
{
  fEncodingDate =
    lpsrVarValAssoc::create (
      inputLineNumber,
      lpsrVarValAssoc::kUncommented,
      lpsrVarValAssoc::kWithoutBackSlash,
      lpsrVarValAssoc::kEncodingDate,
      lpsrVarValAssoc::kEqualSign,
      lpsrVarValAssoc::kQuotesAroundValue,
      val,
      lpsrVarValAssoc::g_LilyPondVarValAssocNoUnit,
      lpsrVarValAssoc::g_LilyPondVarValAssocNoComment,
      lpsrVarValAssoc::kWithoutEndl);
}

void lpsrHeader::setScoreInstrument (
  int    inputLineNumber,
  string val)
{
  fScoreInstrument =
    lpsrVarValAssoc::create (
      inputLineNumber,
      lpsrVarValAssoc::kUncommented,
      lpsrVarValAssoc::kWithoutBackSlash,
      lpsrVarValAssoc::kScoreInstrument,
      lpsrVarValAssoc::kEqualSign,
      lpsrVarValAssoc::kQuotesAroundValue,
      val,
      lpsrVarValAssoc::g_LilyPondVarValAssocNoUnit,
      lpsrVarValAssoc::g_LilyPondVarValAssocNoComment,
      lpsrVarValAssoc::kWithoutEndl);
}

void lpsrHeader::setMiscellaneousField (
  int    inputLineNumber,
  string val)
{
  fMiscellaneousField =
    lpsrVarValAssoc::create (
      inputLineNumber,
      lpsrVarValAssoc::kUncommented,
      lpsrVarValAssoc::kWithoutBackSlash,
      lpsrVarValAssoc::kMiscellaneousField,
      lpsrVarValAssoc::kEqualSign,
      lpsrVarValAssoc::kQuotesAroundValue,
      val,
      lpsrVarValAssoc::g_LilyPondVarValAssocNoUnit,
      lpsrVarValAssoc::g_LilyPondVarValAssocNoComment,
      lpsrVarValAssoc::kWithoutEndl);
}

void lpsrHeader::addRights (
  int    inputLineNumber,
  string value)
{
  if (! fRights) {
    fRights =
      lpsrVarValsListAssoc::create (
        inputLineNumber,
        lpsrVarValsListAssoc::kRights);
  }
  
  fRights->
    addAssocVariableValue (value);
}

void lpsrHeader::addComposer (
  int    inputLineNumber,
  string value)
{
  if (! fComposers) {
    fComposers =
      lpsrVarValsListAssoc::create (
        inputLineNumber,
        lpsrVarValsListAssoc::kComposer);
  }
  
  fComposers->
    addAssocVariableValue (value);
}

void lpsrHeader::addArranger (
  int    inputLineNumber,
  string value)
{
  if (! fArrangers) {
    fArrangers =
      lpsrVarValsListAssoc::create (
        inputLineNumber,
        lpsrVarValsListAssoc::kArranger);
  }
  
  fArrangers->
    addAssocVariableValue (value);
}

void lpsrHeader::addLyricist (
  int    inputLineNumber,
  string value)
{
  if (! fLyricists) {
    fLyricists =
      lpsrVarValsListAssoc::create (
        inputLineNumber,
        lpsrVarValsListAssoc::kLyricist);
  }
  
  fLyricists->
    addAssocVariableValue (value);
}

void lpsrHeader::addPoet (
  int    inputLineNumber,
  string value)
{
  if (! fPoets) {
    fPoets =
      lpsrVarValsListAssoc::create (
        inputLineNumber,
        lpsrVarValsListAssoc::kPoet);
  }
  
  fPoets->
    addAssocVariableValue (value);
}

void lpsrHeader::addSoftware (
  int    inputLineNumber,
  string value)
{
  if (! fSoftwares) {
    fSoftwares =
      lpsrVarValsListAssoc::create (
        inputLineNumber,
        lpsrVarValsListAssoc::kSoftware);
  }
  
  fSoftwares->
    addAssocVariableValue (value);
}

int lpsrHeader::maxLilypondVariablesNamesLength ()
{
  int result = 0;

  if (fWorkNumber) {
    int length = fWorkNumber->lilyPondVarValAssocKindAsString ().size ();
    if (length > result) result = length;
  }
  
  if (fWorkTitle) {
    int length = fWorkTitle->lilyPondVarValAssocKindAsString ().size ();
    if (length > result) result = length;
  }
    
  if (fMovementNumber) {
    int length = fMovementNumber->lilyPondVarValAssocKindAsString ().size ();
    if (length > result) result = length;
  }
    
  if (fMovementTitle) {
    int length = fMovementTitle->lilyPondVarValAssocKindAsString ().size ();
    if (length > result) result = length;
  }
    
  if (fRights) {
    const list<string>&
      variableValuesList =
        fRights->getVariableValuesList ();
        
    list<string>::const_iterator i;
    for (i=variableValuesList.begin (); i!=variableValuesList.end (); i++) {
      int length = (*i).size ();
      if (length > result) result = length;
    } // for
  }
    
  if (fComposers) {
    const list<string>&
      variableValuesList =
        fComposers->getVariableValuesList ();
        
    list<string>::const_iterator i;
    for (i=variableValuesList.begin (); i!=variableValuesList.end (); i++) {
      int length = (*i).size ();
      if (length > result) result = length;
    } // for
  }
    
  if (fArrangers) {
    const list<string>&
      variableValuesList =
        fArrangers->getVariableValuesList ();
        
    list<string>::const_iterator i;
    for (i=variableValuesList.begin (); i!=variableValuesList.end (); i++) {
      int length = (*i).size ();
      if (length > result) result = length;
    } // for
  }
    
  if (fLyricists) {
    const list<string>&
      variableValuesList =
        fLyricists->getVariableValuesList ();
        
    list<string>::const_iterator i;
    for (i=variableValuesList.begin (); i!=variableValuesList.end (); i++) {
      int length = (*i).size ();
      if (length > result) result = length;
    } // for
  }
    
  if (fPoets) {
    const list<string>&
      variableValuesList =
        fPoets->getVariableValuesList ();
        
    list<string>::const_iterator i;
    for (i=variableValuesList.begin (); i!=variableValuesList.end (); i++) {
      int length = (*i).size ();
      if (length > result) result = length;
    } // for
  }
    
  if (fSoftwares) {
    const list<string>&
      variableValuesList =
        fSoftwares->getVariableValuesList ();
        
    list<string>::const_iterator i;
    for (i=variableValuesList.begin (); i!=variableValuesList.end (); i++) {
      int length = (*i).size ();
      if (length > result) result = length;
    } // for
  }
    
  if (fScoreInstrument) {
    int length = fScoreInstrument->lilyPondVarValAssocKindAsString ().size ();
    if (length > result) result = length;
  }
  
  if (fEncodingDate) {
    int length = fEncodingDate->lilyPondVarValAssocKindAsString ().size ();
    if (length > result) result = length;
  }
  
  if (fMiscellaneousField) {
    int length = fMiscellaneousField->lilyPondVarValAssocKindAsString ().size ();
    if (length > result) result = length;
  }
  
  return result;
}

void lpsrHeader::acceptIn (basevisitor* v)
{
  if (gLpsrOptions->fTraceLpsrVisitors) {
    gLogIOstream <<
      "% ==> lpsrHeader::acceptIn()" <<
      endl;
  }
      
  if (visitor<S_lpsrHeader>*
    p =
      dynamic_cast<visitor<S_lpsrHeader>*> (v)) {
        S_lpsrHeader elem = this;
        
        if (gLpsrOptions->fTraceLpsrVisitors) {
          gLogIOstream <<
            "% ==> Launching lpsrHeader::visitStart()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void lpsrHeader::acceptOut (basevisitor* v)
{
  if (gLpsrOptions->fTraceLpsrVisitors) {
    gLogIOstream <<
      "% ==> lpsrHeader::acceptOut()" <<
      endl;
  }

  if (visitor<S_lpsrHeader>*
    p =
      dynamic_cast<visitor<S_lpsrHeader>*> (v)) {
        S_lpsrHeader elem = this;
      
        if (gLpsrOptions->fTraceLpsrVisitors) {
          gLogIOstream <<
            "% ==> Launching lpsrHeader::visitEnd()" <<
            endl;
        }
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

  if (fRights) {
    // browse fRights
    msrBrowser<lpsrVarValsListAssoc> browser (v);
    browser.browse (*fRights);
  }

  if (fComposers) {
    // browse fComposers
    msrBrowser<lpsrVarValsListAssoc> browser (v);
    browser.browse (*fComposers);
  }
    
  if (fArrangers) {
    // browse fArrangers
    msrBrowser<lpsrVarValsListAssoc> browser (v);
    browser.browse (*fArrangers);
  }
    
  if (fLyricists) {
    // browse fLyricists
    msrBrowser<lpsrVarValsListAssoc> browser (v);
    browser.browse (*fLyricists);
  }
    
  if (fPoets) {
    // browse fPoets
    msrBrowser<lpsrVarValsListAssoc> browser (v);
    browser.browse (*fPoets);
  }
    
  if (fSoftwares) {
    // browse fSoftwares
    msrBrowser<lpsrVarValsListAssoc> browser (v);
    browser.browse (*fSoftwares);
  }

  if (fScoreInstrument) {
    // browse fScoreInstrument
    msrBrowser<lpsrVarValAssoc> browser (v);
    browser.browse (*fScoreInstrument);
  }

  if (fEncodingDate) {
    // browse fEncodingDate
    msrBrowser<lpsrVarValAssoc> browser (v);
    browser.browse (*fEncodingDate);
  }

  if (fMiscellaneousField) {
    // browse fMiscellaneousField
    msrBrowser<lpsrVarValAssoc> browser (v);
    browser.browse (*fMiscellaneousField);
  }
}

ostream& operator<< (ostream& os, const S_lpsrHeader& elt)
{
  elt->print (os);
  return os;
}

void lpsrHeader::print (ostream& os)
{
  os <<
    "Header" <<
    endl;

  bool emptyHeader = true;
  
  gIndenter++;

  const int fieldWidth = 16;
  
  if (fWorkNumber) {
    os << left <<
      setw (fieldWidth) <<
      fWorkNumber->lilyPondVarValAssocKindAsString () << " : " <<
      endl;
      
    gIndenter++;
    
    os <<
      fWorkNumber <<
      endl;
      
    gIndenter--;

    emptyHeader = false;
  }
  
  if (fWorkTitle) {
    os << left <<
      setw (fieldWidth) <<
      fWorkTitle->lilyPondVarValAssocKindAsString () << " : " <<
      endl;
      
    gIndenter++;
    
    os <<
      fWorkTitle <<
      endl;
      
    gIndenter--;

    emptyHeader = false;
  }
    
  if (fMovementNumber) {
    os << left <<
      setw (fieldWidth) <<
      fMovementNumber->lilyPondVarValAssocKindAsString () << " : " <<
      endl;
      
    gIndenter++;
    
    os <<
      fMovementNumber <<
      endl;
      
    gIndenter--;

    emptyHeader = false;
  }
    
  if (fMovementTitle) {
    os << left <<
      setw (fieldWidth) <<
      fMovementTitle->lilyPondVarValAssocKindAsString () << " : " <<
      endl;
      
    gIndenter++;
    
    os <<
      fMovementTitle <<
      endl;
      
    gIndenter--;

    emptyHeader = false;
  }
    
  if (fRights) {
    os << left <<
      setw (fieldWidth) <<
      fRights->lilyPondVarValsListAssocKindAsString () << " : " <<
      endl;
      
    gIndenter++;
    
    os <<
      fRights <<
      endl;
      
    gIndenter--;
        
    emptyHeader = false;
  }
    
  if (fComposers) {
    os << left <<
      setw (fieldWidth) <<
      fComposers->lilyPondVarValsListAssocKindAsString () << " : " <<
      endl;
      
    gIndenter++;
    
    os <<
      fComposers <<
      endl;
      
    gIndenter--;
        
    emptyHeader = false;
  }
    
  if (fArrangers) {
    os << left <<
      setw (fieldWidth) <<
      fArrangers->lilyPondVarValsListAssocKindAsString () << " : " <<
      endl;
      
    gIndenter++;
    
    os <<
      fArrangers <<
      endl;
      
    gIndenter--;
        
    emptyHeader = false;
  }
    
  if (fLyricists) {
    os << left <<
      setw (fieldWidth) <<
      fLyricists->lilyPondVarValsListAssocKindAsString () << " : " <<
      endl;
      
    gIndenter++;
    
    os <<
      fLyricists <<
      endl;
      
    gIndenter--;
        
    emptyHeader = false;
  }
    
  if (fPoets) {
    os << left <<
      setw (fieldWidth) <<
      fPoets->lilyPondVarValsListAssocKindAsString () << " : " <<
      endl;
      
    gIndenter++;
    
    os <<
      fPoets <<
      endl;
      
    gIndenter--;
        
    emptyHeader = false;
  }
    
  if (fSoftwares) {
    os << left <<
      setw (fieldWidth) <<
      fSoftwares->lilyPondVarValsListAssocKindAsString () << " : " <<
      endl;
      
    gIndenter++;
    
    os <<
      fSoftwares <<
      endl;
      
    gIndenter--;
        
    emptyHeader = false;
  }
    
  if (fScoreInstrument) {
    os << left <<
      setw (fieldWidth) <<
      fScoreInstrument->lilyPondVarValAssocKindAsString () << " : " <<
      endl;
      
    gIndenter++;
    
    os <<
      fScoreInstrument <<
      endl;
      
    gIndenter--;

    emptyHeader = false;
  }

  if (fEncodingDate) {
    os << left <<
      setw (fieldWidth) <<
      fEncodingDate->lilyPondVarValAssocKindAsString () << " : " <<
      endl;
      
    gIndenter++;
    
    os <<
      fEncodingDate <<
      endl;
      
    gIndenter--;

    emptyHeader = false;
  }

  if (fMiscellaneousField) {
    os << left <<
      setw (fieldWidth) <<
      fMiscellaneousField->lilyPondVarValAssocKindAsString () << " : " <<
      endl;
      
    gIndenter++;
    
    os <<
      fMiscellaneousField <<
      endl;
      
    gIndenter--;

    emptyHeader = false;
  }
  
  if (emptyHeader) {
    os <<
      " " << "nothing specified" <<
      endl; // JMI
  }
  
  gIndenter--;
}

//______________________________________________________________________________
S_lpsrPaper lpsrPaper::create (
  int           inputLineNumber)
{
  lpsrPaper* o =
    new lpsrPaper (
      inputLineNumber);
  assert(o!=0);
  return o;
}

lpsrPaper::lpsrPaper (
  int           inputLineNumber)
    : msrElement (inputLineNumber)
{
  fPaperWidth   = -1.0;
  fPaperWidth   = -1.0;
  
  fPaperHeight  = -1.0;
  fTopMargin    = -1.0;
  fBottomMargin = -1.0;
  fLeftMargin   = -1.0;
  fRightMargin  = -1.0;
    
  fIndent       = -1.0;
  fShortIndent  = -1.0;

  fBetweenSystemSpace = -1.0;
  fPageTopSpace = -1.0;
}

lpsrPaper::~lpsrPaper ()
{}

void lpsrPaper::setIndent (float val)
{
  if (gGeneralOptions->fTraceGeometry) {
    gLogIOstream <<
      "Setting paper indent to " << val <<
      endl;
  }

  fIndent = val;
}

void lpsrPaper::setShortIndent (float val)
{
  if (gGeneralOptions->fTraceGeometry) {
    gLogIOstream <<
      "Setting paper short indent to " << val <<
      endl;
  }

  fShortIndent = val;
}

void lpsrPaper::acceptIn (basevisitor* v)
{
  if (gLpsrOptions->fTraceLpsrVisitors) {
    gLogIOstream <<
      "% ==> lpsrPaper::acceptIn()" <<
      endl;
  }
      
  if (visitor<S_lpsrPaper>*
    p =
      dynamic_cast<visitor<S_lpsrPaper>*> (v)) {
        S_lpsrPaper elem = this;
        
        if (gLpsrOptions->fTraceLpsrVisitors) {
          gLogIOstream <<
            "% ==> Launching lpsrPaper::visitStart()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void lpsrPaper::acceptOut (basevisitor* v)
{
  if (gLpsrOptions->fTraceLpsrVisitors) {
    gLogIOstream <<
      "% ==> lpsrPaper::acceptOut()" <<
      endl;
  }

  if (visitor<S_lpsrPaper>*
    p =
      dynamic_cast<visitor<S_lpsrPaper>*> (v)) {
        S_lpsrPaper elem = this;
      
        if (gLpsrOptions->fTraceLpsrVisitors) {
          gLogIOstream <<
            "% ==> Launching lpsrPaper::visitEnd()" <<
            endl;
        }
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

  bool emptyPaper = true;

  gIndenter++;

  const int fieldWidth = 20;
  
  // page width, height and margins

  if (fPaperWidth > 0) {
    os << left <<
      setw (fieldWidth) <<
      "paper-width" << " = " <<
      setprecision (2) << fPaperWidth << "\\cm" <<
      endl;
      
    emptyPaper = false;
  }
  
  if (fPaperHeight > 0) {
    os << left <<
      setw (fieldWidth) <<
      "paper-height" << " = " <<
      setprecision (2) << fPaperHeight << "\\cm" <<
      endl;
      
    emptyPaper = false;
  }
  
  if (fTopMargin > 0) {
    os << left <<
      setw (fieldWidth) <<
      "top-margin" << " = " <<
      setprecision (2) << fTopMargin << "\\cm" <<
      endl;
      
    emptyPaper = false;
  }
  
  if (fBottomMargin > 0) {
    os << left <<
      setw (fieldWidth) <<
      "bottom-margin" << " = " <<
      setprecision (2) << fBottomMargin << "\\cm" <<
      endl;
      
    emptyPaper = false;
  }
  
  if (fLeftMargin > 0) {
    os << left <<
      setw (fieldWidth) <<
      "left-margin" << " = " <<
      setprecision (2) << fLeftMargin << "\\cm" <<
      endl;
      
    emptyPaper = false;
  }
  
  if (fRightMargin > 0) {
    os << left <<
      setw (fieldWidth) <<
      "right-margin" << " = " <<
      setprecision (2) << fRightMargin << "\\cm" <<
      endl;
      
    emptyPaper = false;
  }

  if (fIndent > 0) {
    os << left <<
      setw (fieldWidth) <<
      "indent" << " = " <<
      setprecision (2) << fIndent << "\\cm" <<
      endl;
      
    emptyPaper = false;
  }

  if (fShortIndent > 0) {
    os << left <<
      setw (fieldWidth) <<
      "short-indent" << " = " <<
      setprecision (2) << fShortIndent << "\\cm" <<
      endl;
      
    emptyPaper = false;
  }

  // spaces

  if (fBetweenSystemSpace > 0) {
    os << left <<
      setw (fieldWidth) <<
      "between-system-space" << " = " <<
      setprecision (2) << fBetweenSystemSpace << "\\cm" <<
      endl;
      
    emptyPaper = false;
  }

  if (fPageTopSpace > 0) {
    os << left <<
      setw (fieldWidth) <<
      "page-top-space" << " = " <<
      setprecision (2) << fPageTopSpace << "\\cm" <<
      endl;
      
    emptyPaper = false;
  }

  // headers and footers

  if (fOddHeaderMarkup.size ()) {
    os << left <<
      setw (fieldWidth) <<
      "oddHeaderMarkup" << " = " <<
      fOddHeaderMarkup <<
      endl;
      
    emptyPaper = false;
  }

  if (fEvenHeaderMarkup.size ()) {
    os << left <<
      setw (fieldWidth) <<
      "evenHeaderMarkup" << " = " <<
      fEvenHeaderMarkup <<
      endl;
      
    emptyPaper = false;
  }

  if (fOddFooterMarkup.size ()) {
    os << left <<
      setw (fieldWidth) <<
      "oddFooterMarkup" << " = " <<
      fOddFooterMarkup <<
      endl;
      
    emptyPaper = false;
  }

  if (fEvenFooterMarkup.size ()) {
    os << left <<
      setw (fieldWidth) <<
      "evenFooterMarkup" << " = " <<
      fEvenFooterMarkup <<
      endl;
      
    emptyPaper = false;
  }

  // otherwise
  
  if (emptyPaper) {
    os <<
      " " << "nothing specified" <<
      endl;
  }
  
  gIndenter--;
}

//______________________________________________________________________________
S_lpsrLayout lpsrLayout::create (
  int            inputLineNumber)
{
  lpsrLayout* o =
    new lpsrLayout (
      inputLineNumber);
  assert(o!=0);
  return o;
}

lpsrLayout::lpsrLayout (
  int            inputLineNumber)
    : lpsrElement (inputLineNumber)
{
  fStaffSize = 20; // LilyPond default // JMI
}

lpsrLayout::~lpsrLayout()
{}

void lpsrLayout::acceptIn (basevisitor* v)
{
  if (gLpsrOptions->fTraceLpsrVisitors) {
    gLogIOstream <<
      "% ==> lpsrLayout::acceptIn()" <<
      endl;
  }
      
  if (visitor<S_lpsrLayout>*
    p =
      dynamic_cast<visitor<S_lpsrLayout>*> (v)) {
        S_lpsrLayout elem = this;
        
        if (gLpsrOptions->fTraceLpsrVisitors) {
          gLogIOstream <<
            "% ==> Launching lpsrLayout::visitStart()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void lpsrLayout::acceptOut (basevisitor* v)
{
  if (gLpsrOptions->fTraceLpsrVisitors) {
    gLogIOstream <<
      "% ==> lpsrLayout::acceptOut()" <<
      endl;
  }

  if (visitor<S_lpsrLayout>*
    p =
      dynamic_cast<visitor<S_lpsrLayout>*> (v)) {
        S_lpsrLayout elem = this;
      
        if (gLpsrOptions->fTraceLpsrVisitors) {
          gLogIOstream <<
            "% ==> Launching lpsrLayout::visitEnd()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void lpsrLayout::browseData (basevisitor* v)
{

  int lilypondAssocs = flpsrVarValAssocs.size ();

  for (int i = 0; i < lilypondAssocs; i++ ) {
    // browse the variable/value association
    msrBrowser<lpsrVarValAssoc> browser (v);
    browser.browse (*flpsrVarValAssocs [i]);
  } // for
  
  int schemeAssocs = fLpsrSchemeVariables.size ();
  
  for (int i = 0; i < schemeAssocs; i++ ) {
    // browse the Scheme variable/value association
    msrBrowser<lpsrSchemeVariable> browser (v);
    browser.browse (*fLpsrSchemeVariables [i]);
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

  gIndenter++;

  os <<
    "StaffSize: " << fStaffSize <<
    endl;
    
  int lilypondAssocs = flpsrVarValAssocs.size ();
  
  for (int i = 0; i < lilypondAssocs; i++ ) {
    os << flpsrVarValAssocs [i];
  } // for

  int schemeAssocs = fLpsrSchemeVariables.size ();
  
  for (int i = 0; i < schemeAssocs; i++ ) {
    os << fLpsrSchemeVariables[i];
  } // for
  
  gIndenter--;
}

//______________________________________________________________________________
S_lpsrStaffBlock lpsrStaffBlock::create (
  S_msrStaff staff)
{
  lpsrStaffBlock* o = new lpsrStaffBlock (
    staff);
  assert(o!=0);
  return o;
}

lpsrStaffBlock::lpsrStaffBlock (
  S_msrStaff staff)
    : lpsrElement (0) // JMI
{
  // sanity check
  msrAssert (
    staff != nullptr,
    "staff is null");
    
  fStaff = staff;

  // set staff block instrument names
  fStaffBlockInstrumentName =
    fStaff->
      getStaffInstrumentName ();
  fStaffBlockShortInstrumentName =
    fStaff->
      getStaffInstrumentAbbreviation ();
}

lpsrStaffBlock::~lpsrStaffBlock()
{}

void lpsrStaffBlock::appendVoiceUseToStaffBlock (S_msrVoice voice)
{
  S_lpsrUseVoiceCommand
    useVoiceCommand =
      lpsrUseVoiceCommand::create (
        fInputLineNumber,
        voice);
  
  fStaffBlockElements.push_back (useVoiceCommand);
}

void lpsrStaffBlock::appendLyricsUseToStaffBlock (S_msrStanza stanza)
{
  S_lpsrNewLyricsBlock
    newLyricsCommand =
      lpsrNewLyricsBlock::create (
        fInputLineNumber,
        stanza,
        stanza->getStanzaVoiceUplink ());
  
  fStaffBlockElements.push_back (newLyricsCommand);
}

void lpsrStaffBlock::acceptIn (basevisitor* v)
{
  if (gLpsrOptions->fTraceLpsrVisitors) {
    gLogIOstream <<
      "% ==> lpsrStaffBlock::acceptIn()" <<
      endl;
  }
      
  if (visitor<S_lpsrStaffBlock>*
    p =
      dynamic_cast<visitor<S_lpsrStaffBlock>*> (v)) {
        S_lpsrStaffBlock elem = this;
        
        if (gLpsrOptions->fTraceLpsrVisitors) {
          gLogIOstream <<
            "% ==> Launching lpsrStaffBlock::visitStart()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void lpsrStaffBlock::acceptOut (basevisitor* v)
{
  if (gLpsrOptions->fTraceLpsrVisitors) {
    gLogIOstream <<
      "% ==> lpsrStaffBlock::acceptOut()" <<
      endl;
  }

  if (visitor<S_lpsrStaffBlock>*
    p =
      dynamic_cast<visitor<S_lpsrStaffBlock>*> (v)) {
        S_lpsrStaffBlock elem = this;
      
        if (gLpsrOptions->fTraceLpsrVisitors) {
          gLogIOstream <<
            "% ==> Launching lpsrStaffBlock::visitEnd()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void lpsrStaffBlock::browseData (basevisitor* v)
{
  if (gLpsrOptions->fTraceLpsrVisitors) {
    gLogIOstream <<
      "% ==> lpsrStaffBlock::browseData()" <<
      endl;
  }

  for (
    list<S_msrElement>::const_iterator i = fStaffBlockElements.begin ();
    i != fStaffBlockElements.end ();
    i++) {
    // browse the element
    msrBrowser<msrElement> browser (v);
    browser.browse (*(*i));
  } // for

  if (gLpsrOptions->fTraceLpsrVisitors) {
    gLogIOstream <<
      "% <== lpsrStaffBlock::browseData()" <<
      endl;
  }
}

ostream& operator<< (ostream& os, const S_lpsrStaffBlock& scr)
{
  scr->print (os);
  return os;
}

void lpsrStaffBlock::print (ostream& os)
{
  os <<
    "StaffBlock for staff \"" <<
    fStaff->getStaffName () <<
    "\" (" << fStaff->staffKindAsString () <<
    "), " <<
    singularOrPlural (
      fStaffBlockElements.size (), "element", "elements") <<
    endl;

  gIndenter++;

  const int fieldWidth = 31;
  
  os << left <<
    setw (fieldWidth) <<
    "(StaffBlockInstrumentName" << " = \"" << fStaffBlockInstrumentName <<
    "\")" <<
    endl <<
    setw (fieldWidth) <<
    "(StaffBlockShortInstrumentName" << " = \"" << fStaffBlockShortInstrumentName <<
    "\")" <<
    endl <<
    endl;

  if (fStaffBlockElements.size ()) {  
    list<S_msrElement>::const_iterator
      iBegin = fStaffBlockElements.begin (),
      iEnd   = fStaffBlockElements.end (),
      i      = iBegin;
    for ( ; ; ) {
      os << (*i);
      if (++i == iEnd) break;
 // JMI     os << endl;
    } // for
  }

  gIndenter--;
}

//______________________________________________________________________________
S_lpsrPartBlock lpsrPartBlock::create (
  S_msrPart part)
{
  lpsrPartBlock* o = new lpsrPartBlock (
    part);
  assert(o!=0);
  return o;
}

lpsrPartBlock::lpsrPartBlock (
  S_msrPart part)
    : lpsrElement (0) // JMI 
{
  // sanity check
  msrAssert (
    part != nullptr,
    "part is null");
    
  fPart = part;

  // set part block instrument names default values // JMI
  fPartBlockInstrumentName =
    fPart->
      getPartName ();
  fPartBlockShortInstrumentName =
    fPart->
      getPartAbbreviation();
}

lpsrPartBlock::~lpsrPartBlock()
{}

void lpsrPartBlock::acceptIn (basevisitor* v)
{
  if (gLpsrOptions->fTraceLpsrVisitors) {
    gLogIOstream <<
      "% ==> lpsrPartBlock::acceptIn()" <<
      endl;
  }
      
  if (visitor<S_lpsrPartBlock>*
    p =
      dynamic_cast<visitor<S_lpsrPartBlock>*> (v)) {
        S_lpsrPartBlock elem = this;
        
        if (gLpsrOptions->fTraceLpsrVisitors) {
          gLogIOstream <<
            "% ==> Launching lpsrPartBlock::visitStart()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void lpsrPartBlock::acceptOut (basevisitor* v)
{
  if (gLpsrOptions->fTraceLpsrVisitors) {
    gLogIOstream <<
      "% ==> lpsrPartBlock::acceptOut()" <<
      endl;
  }

  if (visitor<S_lpsrPartBlock>*
    p =
      dynamic_cast<visitor<S_lpsrPartBlock>*> (v)) {
        S_lpsrPartBlock elem = this;
      
        if (gLpsrOptions->fTraceLpsrVisitors) {
          gLogIOstream <<
            "% ==> Launching lpsrPartBlock::visitEnd()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void lpsrPartBlock::browseData (basevisitor* v)
{
  if (gLpsrOptions->fTraceLpsrVisitors) {
    gLogIOstream <<
      "% ==> lpsrPartBlock::browseData()" <<
      endl;
  }

  for (
    list<S_msrElement>::const_iterator i = fPartBlockElements.begin ();
    i != fPartBlockElements.end ();
    i++) {
    // browse the element
    msrBrowser<msrElement> browser (v);
    browser.browse (*(*i));
  } // for

  if (gLpsrOptions->fTraceLpsrVisitors) {
    gLogIOstream <<
      "% <== lpsrPartBlock::browseData()" <<
      endl;
  }
}

ostream& operator<< (ostream& os, const S_lpsrPartBlock& scr)
{
  scr->print (os);
  return os;
}

void lpsrPartBlock::print (ostream& os)
{
  os <<
    "PartBlock" << " " <<
    "for part " << fPart->getPartCombinedName () <<
    ", " <<
    singularOrPlural (
      fPartBlockElements.size (), "element", "elements") <<
    endl;

  gIndenter++;

  const int fieldWidth = 28;

  os << left <<
    setw (fieldWidth) << string ("(") + "PartName" << " = \"" <<
    fPart->getPartName () << "\")" <<
    endl <<
    setw (fieldWidth) << string ("(") + "PartAbbreviation" << " = \"" <<
    fPart->getPartAbbreviation () << "\")" <<
    endl <<

    setw (fieldWidth) << "PartBlockInstrumentName" << " = \"" <<
    fPartBlockInstrumentName <<
    "\"" <<
    endl <<
    setw (fieldWidth) << "PartBlockShortInstrumentName" << " = \"" <<
    fPartBlockShortInstrumentName <<
    "\"" <<
    endl;

  os << endl;

  if (fPartBlockElements.size ()) {  
    list<S_msrElement>::const_iterator
      iBegin = fPartBlockElements.begin (),
      iEnd   = fPartBlockElements.end (),
      i      = iBegin;
    for ( ; ; ) {
      os << (*i);
      if (++i == iEnd) break;
      os << endl;
    } // for
  }

  gIndenter--;
}

//______________________________________________________________________________
S_lpsrPartGroupBlock lpsrPartGroupBlock::create (
  S_msrPartGroup partGroup)
{
  lpsrPartGroupBlock* o = new lpsrPartGroupBlock (
    partGroup);
  assert(o!=0);
  return o;
}

lpsrPartGroupBlock::lpsrPartGroupBlock (
  S_msrPartGroup partGroup)
    : lpsrElement (0) // JMI
{
  fPartGroup = partGroup;
}

lpsrPartGroupBlock::~lpsrPartGroupBlock()
{}

void lpsrPartGroupBlock::acceptIn (basevisitor* v)
{
  if (gLpsrOptions->fTraceLpsrVisitors) {
    gLogIOstream <<
      "% ==> lpsrPartGroupBlock::acceptIn()" <<
      endl;
  }
      
  if (visitor<S_lpsrPartGroupBlock>*
    p =
      dynamic_cast<visitor<S_lpsrPartGroupBlock>*> (v)) {
        S_lpsrPartGroupBlock elem = this;
        
        if (gLpsrOptions->fTraceLpsrVisitors) {
          gLogIOstream <<
            "% ==> Launching lpsrPartGroupBlock::visitStart()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void lpsrPartGroupBlock::acceptOut (basevisitor* v)
{
  if (gLpsrOptions->fTraceLpsrVisitors) {
    gLogIOstream <<
      "% ==> lpsrPartGroupBlock::acceptOut()" <<
      endl;
  }

  if (visitor<S_lpsrPartGroupBlock>*
    p =
      dynamic_cast<visitor<S_lpsrPartGroupBlock>*> (v)) {
        S_lpsrPartGroupBlock elem = this;
      
        if (gLpsrOptions->fTraceLpsrVisitors) {
          gLogIOstream <<
            "% ==> Launching lpsrPartGroupBlock::visitEnd()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void lpsrPartGroupBlock::browseData (basevisitor* v)
{
  if (gLpsrOptions->fTraceLpsrVisitors) {
    gLogIOstream <<
      "% ==> lpsrPartGroupBlock::browseData()" <<
      endl;
  }

  for (
    list<S_msrElement>::const_iterator i = fPartGroupBlockElements.begin ();
    i != fPartGroupBlockElements.end ();
    i++) {
    // browse the element
    msrBrowser<msrElement> browser (v);
    browser.browse (*(*i));
  } // for

  if (gLpsrOptions->fTraceLpsrVisitors) {
    gLogIOstream <<
      "% <== lpsrPartGroupBlock::browseData()" <<
      endl;
  }
}

ostream& operator<< (ostream& os, const S_lpsrPartGroupBlock& scr)
{
  scr->print (os);
  return os;
}

void lpsrPartGroupBlock::print (ostream& os)
{
  os <<
    endl <<
    "PartGroupBlock" << " " <<
    "for partGroup \"" <<
    fPartGroup->getPartGroupCombinedName () <<
    "\", " << fPartGroup->partGroupSymbolKindAsString () <<
    ", " <<
    singularOrPlural (
      fPartGroupBlockElements.size (), "element", "elements") <<
    endl << endl;

  gIndenter++;

  if (fPartGroupBlockElements.size ()) {  
    list<S_msrElement>::const_iterator
      iBegin = fPartGroupBlockElements.begin (),
      iEnd   = fPartGroupBlockElements.end (),
      i      = iBegin;
    for ( ; ; ) {
      os << (*i);
      if (++i == iEnd) break;
      // no endl here ??? JMI
    } // for
  }

  gIndenter--;
}

//______________________________________________________________________________
S_lpsrScoreBlock lpsrScoreBlock::create (
  int            inputLineNumber)
{
  lpsrScoreBlock* o = new lpsrScoreBlock (
    inputLineNumber);
  assert(o!=0);
  return o;
}

lpsrScoreBlock::lpsrScoreBlock (
  int            inputLineNumber)
    : lpsrElement (inputLineNumber)
{
  // create the score command parallel music
  fScoreBlockParallelMusicBLock =
    lpsrParallelMusicBLock::create (
      inputLineNumber,
      lpsrParallelMusicBLock::kEndOfLine);
  
  // create the score command layout
  fScoreBlockLayout =
    lpsrLayout::create (
      inputLineNumber);
  
  // create the score command midi
  string midiTempoDuration  = gLilypondOptions->fMidiTempo.first;
  int    midiTempoPerSecond = gLilypondOptions->fMidiTempo.second;
  
  fScoreBlockMidi =
    msrMidi::create (
      inputLineNumber,
      midiTempoDuration,
      midiTempoPerSecond);
}

lpsrScoreBlock::~lpsrScoreBlock()
{}

void lpsrScoreBlock::appendPartGroupBlockToScoreBlock (
  S_lpsrPartGroupBlock partGroupBlock)
{
  if (gGeneralOptions->fTracePartGroups) {
    gLogIOstream <<
      "Appending part group block " <<
       partGroupBlock-> getPartGroup ()-> getPartGroupCombinedName() <<
       " to LPSR score" <<
       endl;
  }

  fScoreBlockParallelMusicBLock->
    appendPartGroupBlockToParallelMusicBLock (
      partGroupBlock);
    
//               fScoreBlockElements.push_back(partGroupBlock);
}

/* JMI
void lpsrScoreBlock::appendVoiceUseToParallelMusicBLock (
  S_lpsrUseVoiceCommand voiceUse)
{
  if (gGeneralOptions->fTraceVoices) {
    gLogIOstream <<
      "Appending the use of voice \"" <<
       voiceUse-> getVoice ()-> getVoiceName() <<
       "\" to LPSR score" <<
       endl;
  }

  fScoreBlockParallelMusicBLock->
    addElementToParallelMusicBLock (voiceUse);
    
//     JMI             fScoreBlockElements.push_back(voiceUse);
}
                  
void lpsrScoreBlock::appendLyricsUseToParallelMusicBLock (
  S_lpsrNewLyricsBlock lyricsUse)
{
  if (gGeneralOptions->fTraceLyrics) {
    gLogIOstream <<
      "Appending the use of stanza " <<
       lyricsUse-> getStanza ()-> getStanzaName() <<
       " to LPSR score" <<
       endl;
  }

  fScoreBlockParallelMusicBLock->
    addElementToParallelMusicBLock (lyricsUse);
}
*/

void lpsrScoreBlock::acceptIn (basevisitor* v)
{
  if (gLpsrOptions->fTraceLpsrVisitors) {
    gLogIOstream <<
      "% ==> lpsrScoreBlock::acceptIn()" <<
      endl;
  }
      
  if (visitor<S_lpsrScoreBlock>*
    p =
      dynamic_cast<visitor<S_lpsrScoreBlock>*> (v)) {
        S_lpsrScoreBlock elem = this;
        
        if (gLpsrOptions->fTraceLpsrVisitors) {
          gLogIOstream <<
            "% ==> Launching lpsrScoreBlock::visitStart()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void lpsrScoreBlock::acceptOut (basevisitor* v)
{
  if (gLpsrOptions->fTraceLpsrVisitors) {
    gLogIOstream <<
      "% ==> lpsrScoreBlock::acceptOut()" <<
      endl;
  }

  if (visitor<S_lpsrScoreBlock>*
    p =
      dynamic_cast<visitor<S_lpsrScoreBlock>*> (v)) {
        S_lpsrScoreBlock elem = this;
      
        if (gLpsrOptions->fTraceLpsrVisitors) {
          gLogIOstream <<
            "% ==> Launching lpsrScoreBlock::visitEnd()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void lpsrScoreBlock::browseData (basevisitor* v)
{
  if (gLpsrOptions->fTraceLpsrVisitors) {
    gLogIOstream <<
      "% ==> lpsrScoreBlock::browseData()" <<
      endl;
  }

  {
    // browse the score command parallel music
    msrBrowser<lpsrParallelMusicBLock> browser (v);    
    browser.browse (*fScoreBlockParallelMusicBLock);
  }

/* JMI
  for (
    vector<S_msrElement>::const_iterator i = fScoreBlockElements.begin ();
    i != fScoreBlockElements.end ();
    i++) {
    // browse the element
 //   msrBrowser<msrElement> browser (v);
 //   browser.browse (*(*i));
  } // for
*/
  {
    // browse the score command layout
    msrBrowser<lpsrLayout> browser (v);    
    browser.browse (*fScoreBlockLayout);
  }

  {
    // browse the score command midi
    msrBrowser<msrMidi> browser (v);    
    browser.browse (*fScoreBlockMidi);
  }

  if (gLpsrOptions->fTraceLpsrVisitors) {
    gLogIOstream <<
      "% <== lpsrScoreBlock::browseData()" <<
      endl;
  }
}

ostream& operator<< (ostream& os, const S_lpsrScoreBlock& scr)
{
  scr->print (os);
  return os;
}

void lpsrScoreBlock::print (ostream& os)
{
  os << "ScoreBlock" << endl << endl;

  gIndenter++;

  os <<
    fScoreBlockParallelMusicBLock <<
    endl;
    
  os <<
    fScoreBlockLayout <<
    endl;
  
  os <<
    fScoreBlockMidi <<
    endl;

  gIndenter--;
}

//______________________________________________________________________________
S_lpsrScore lpsrScore::create (
  int            inputLineNumber,
  S_msrScore     mScore)
{
  lpsrScore* o = new lpsrScore (
    inputLineNumber, mScore);
  assert(o!=0);
  return o;
}

lpsrScore::lpsrScore (
  int            inputLineNumber,
  S_msrScore     mScore)
    : lpsrElement (inputLineNumber)
{
  fMsrScore = mScore;

  // create the LilyPond version assoc
  fLilypondVersion =
    lpsrVarValAssoc::create (
      inputLineNumber,
      lpsrVarValAssoc::kUncommented,
      lpsrVarValAssoc::kWithBackSlash,
      lpsrVarValAssoc::kVersion,
      lpsrVarValAssoc::kSpace,
      lpsrVarValAssoc::kQuotesAroundValue,
      "2.19",
      lpsrVarValAssoc::g_LilyPondVarValAssocNoUnit,
      lpsrVarValAssoc::g_LilyPondVarValAssocNoComment,
      lpsrVarValAssoc::kWithEndl);

  // create the input source name comment
  {
    stringstream s;
  
    s <<
      "Generated by xml2ly " << currentVersionNumber () <<
      " from ";
  
    if (gXml2lyOptions->fInputSourceName == "-") {
      s << "standard input";
    }
    else {
      s << "\"" << gXml2lyOptions->fInputSourceName << "\"";
    }
  
    s <<
      endl <<
      "% on " << gXml2lyOptions->fTranslationDate <<
      endl;

    fInputSourceNameComment =
      lpsrComment::create (
        inputLineNumber,
        s.str (),
        lpsrComment::kNoGapAfterwards);
  }

  // create the translation date comment
  {
    stringstream s;
    
    s <<
      "% Translation command was:";
        
    fTranslationDateComment =
      lpsrComment::create (
        inputLineNumber,
        s.str (),
        lpsrComment::kNoGapAfterwards);
  }

  // do the command line long and short options differ?
  bool
    longAndShortOptionsDiffer =
      gGeneralOptions->fCommandLineWithShortOptions
        !=
      gGeneralOptions->fCommandLineWithLongOptions;

    
  // create the command line long options comment
  {
    stringstream s;
    
    s <<
      gTab <<
      gGeneralOptions->fCommandLineWithLongOptions;
  
    if (longAndShortOptionsDiffer)
      s <<
        endl <<
        "% or:";
    
    fCommandLineLongOptionsComment =
      lpsrComment::create (
        inputLineNumber,
        s.str (),
        longAndShortOptionsDiffer
          ? lpsrComment::kNoGapAfterwards
          : lpsrComment::kGapAfterwards);
  }

  if (longAndShortOptionsDiffer) {
    // create the command line short options comment
    stringstream s;
    
    s <<
      gTab <<
      gGeneralOptions->fCommandLineWithShortOptions;
    
    fCommandLineShortOptionsComment =
      lpsrComment::create (
        inputLineNumber,
        s.str (),
        lpsrComment::kGapAfterwards);
  }

  // create the global staff size assoc
  fGlobalStaffSizeAssoc =
    lpsrSchemeVariable::create (
      inputLineNumber,
      lpsrSchemeVariable::kUncommented,
      "set-global-staff-size",
      "20", // the LilyPond default
      "Comment or adapt next line as needed (default is 20)",
      lpsrSchemeVariable::kWithEndlTwice);

  // initialize Scheme functions informations
  fTongueSchemeFunctionIsNeeded              = false;
  fCustomShortBarLineSchemeFunctionIsNeeded  = false;
  fEditorialAccidentalSchemeFunctionIsNeeded = false;

  if (gLilypondOptions->fLilypondCompileDate) {
    // create the date and time functions
    addDateAndTimeSchemeFunctionsToScore ();
  }

  if (gLilypondOptions->fJianpu) {
    // create the Jianpu include command
  }
  
  // create the header
  fHeader =
    lpsrHeader::create (
      inputLineNumber);

  // create the paper
  fPaper =
    lpsrPaper::create (
      inputLineNumber);

  if (gLilypondOptions->fLilypondCompileDate) {
    // define headers and footers
    
    fPaper->
      setOddHeaderMarkup (
R"(\markup {
    \fill-line {
      \on-the-fly \not-first-page {
        \fromproperty #'page:page-number-string
        " "
        \fromproperty #'header:title
        " "
        \fromproperty #'header:subtitle
      }
    }
  }
)"
      );
      
    
    fPaper->
      setEvenHeaderMarkup (
R"(\markup {
    \fill-line {
      \on-the-fly \not-first-page {
        \fromproperty #'page:page-number-string
        " "
        \fromproperty #'header:title
        " "
        \fromproperty #'header:subtitle
      }
    }
  }
)"
      );

    stringstream s;

    s <<
R"(\markup {
    \tiny
    \column {
      \fill-line {
        #(string-append
)"
      <<
      "\"Music generated from MusicXML by xml2ly " <<
      currentVersionNumber () <<
      " and engraved by LilyPond \" (lilypond-version))" <<
R"(
      }
      \fill-line {
        "https://github.com/grame-cncm/libmusicxml/tree/lilypond - http://www.lilypond.org"
      }
      \fill-line { \italic { \modTimeAsString }}
    }
  }
)";
    
    fPaper->
      setOddFooterMarkup (
        s.str ());
  }
  
  // create the score layout
  fScoreLayout =
    lpsrLayout::create (
      inputLineNumber);

  // create the 'myBreakIsBreak' assoc
  {
    lpsrVarValAssoc::lpsrCommentedKind
      commentedKind =
        gLilypondOptions->fIgnoreLineBreaks
          ? lpsrVarValAssoc::kCommented
          : lpsrVarValAssoc::kUncommented;
  
    fMyBreakIsBreakAssoc =
      lpsrVarValAssoc::create (
        inputLineNumber,
        commentedKind,
        lpsrVarValAssoc::kWithoutBackSlash,
        lpsrVarValAssoc::kMyBreak,
        lpsrVarValAssoc::kEqualSign,
        lpsrVarValAssoc::kNoQuotesAroundValue,
        "{ \\break }",
        lpsrVarValAssoc::g_LilyPondVarValAssocNoUnit,
        "Pick your choice from the next two lines as needed",
        lpsrVarValAssoc::kWithoutEndl);
  }
  
  // create the 'myBreakIsEmpty' assoc
  {
    lpsrVarValAssoc::lpsrCommentedKind
      commentedKind =
        gLilypondOptions->fIgnoreLineBreaks
          ? lpsrVarValAssoc::kUncommented
          : lpsrVarValAssoc::kCommented;
  
    fMyBreakIsEmptyAssoc =
      lpsrVarValAssoc::create (
        inputLineNumber,
        commentedKind,
        lpsrVarValAssoc::kWithoutBackSlash,
        lpsrVarValAssoc::kMyBreak,
        lpsrVarValAssoc::kEqualSign,
        lpsrVarValAssoc::kNoQuotesAroundValue,
        "{ }",
        lpsrVarValAssoc::g_LilyPondVarValAssocNoUnit,
        lpsrVarValAssoc::g_LilyPondVarValAssocNoComment,
        lpsrVarValAssoc::kWithEndl);
  }

  // create the 'myPageBreakIsPageBreak' assoc
  {
    lpsrVarValAssoc::lpsrCommentedKind
      commentedKind =
        gLilypondOptions->fIgnoreLineBreaks
          ? lpsrVarValAssoc::kCommented
          : lpsrVarValAssoc::kUncommented;
  
    fMyPageBreakIsPageBreakAssoc =
      lpsrVarValAssoc::create (
        inputLineNumber,
        commentedKind,
        lpsrVarValAssoc::kWithoutBackSlash,
        lpsrVarValAssoc::kMyPageBreak,
        lpsrVarValAssoc::kEqualSign,
        lpsrVarValAssoc::kNoQuotesAroundValue,
        "{ \\pageBreak }",
        lpsrVarValAssoc::g_LilyPondVarValAssocNoUnit,
        "Pick your choice from the next two lines as needed",
        lpsrVarValAssoc::kWithoutEndl);
  }
  
  // create the 'myPageBreakIsEmpty' assoc
  {
    lpsrVarValAssoc::lpsrCommentedKind
      commentedKind =
        gLilypondOptions->fIgnoreLineBreaks
          ? lpsrVarValAssoc::kUncommented
          : lpsrVarValAssoc::kCommented;
  
    fMyPageBreakIsEmptyAssoc =
      lpsrVarValAssoc::create (
        inputLineNumber,
        commentedKind,
        lpsrVarValAssoc::kWithoutBackSlash,
        lpsrVarValAssoc::kMyPageBreak,
        lpsrVarValAssoc::kEqualSign,
        lpsrVarValAssoc::kNoQuotesAroundValue,
        "{ }",
        lpsrVarValAssoc::g_LilyPondVarValAssocNoUnit,
        lpsrVarValAssoc::g_LilyPondVarValAssocNoComment,
        lpsrVarValAssoc::kWithEndl);
  }

  if (gLilypondOptions->fGlobal) {
    // create the 'global' assoc
    fGlobalAssoc =
      lpsrVarValAssoc::create (
        inputLineNumber,
        lpsrVarValAssoc::kUncommented,
        lpsrVarValAssoc::kWithoutBackSlash,
        lpsrVarValAssoc::kGlobal,
        lpsrVarValAssoc::kEqualSign,
        lpsrVarValAssoc::kNoQuotesAroundValue,
        "{ }",
        lpsrVarValAssoc::g_LilyPondVarValAssocNoUnit,
        "Place whatever you need in the 'global' variable",
        lpsrVarValAssoc::kWithEndl);
  }
  
  // create the score command
  fScoreBlock =
    lpsrScoreBlock::create (
      inputLineNumber);
}

lpsrScore::~lpsrScore()
{}

void lpsrScore::setGlobalStaffSize (float size)
{
  stringstream s;
  
  s << size;
  
  fGlobalStaffSizeAssoc->
    setVariableValue (s.str());
}

void lpsrScore::setJianpuFileIncludeIsNeeded ()
{
  if (! fScmAndAccregSchemeModulesAreNeeded) {
    addJianpuFileIncludeToScore ();
    
    fJianpuFileIncludeIsNeeded = true;    
  }
}

void lpsrScore::setScmAndAccregSchemeModulesAreNeeded ()
{
  if (! fScmAndAccregSchemeModulesAreNeeded) {
    addAccordionRegistrationSchemeModulesToScore ();
    
    fScmAndAccregSchemeModulesAreNeeded = true;    
  }
}

void lpsrScore::setCustomShortBarLineSchemeFunctionIsNeeded ()
{
  if (! fCustomShortBarLineSchemeFunctionIsNeeded) {
    addCustomShortBarLineSchemeFunctionToScore ();
    
    fCustomShortBarLineSchemeFunctionIsNeeded = true;    
  }
}

void lpsrScore::setTongueSchemeFunctionIsNeeded ()
{
  if (! fTongueSchemeFunctionIsNeeded) {
    addTongueSchemeFunctionToScore ();
    
    fTongueSchemeFunctionIsNeeded = true;    
  }
}

void lpsrScore::setEditorialAccidentalSchemeFunctionIsNeeded ()
{
  if (! fEditorialAccidentalSchemeFunctionIsNeeded) {
    addEditorialAccidentalSchemeFunctionToScore ();
    
    fEditorialAccidentalSchemeFunctionIsNeeded = true;    
  }
}

void lpsrScore::setDynamicsSchemeFunctionIsNeeded ()
{
  if (! fDynamicsSchemeFunctionIsNeeded) {
    addDynamicsSchemeFunctionToScore ();
    
    fDynamicsSchemeFunctionIsNeeded = true;    
  }
}

void lpsrScore::setTupletsCurvedBracketsSchemeFunctionIsNeeded ()
{
  if (! fTupletsCurvedBracketsSchemeFunctionIsNeeded) {
    addTupletsCurvedBracketsSchemeFunctionToScore ();
    
    fTupletsCurvedBracketsSchemeFunctionIsNeeded = true;    
  }
}

void lpsrScore::setAfterSchemeFunctionIsNeeded ()
{
  if (! fAfterSchemeFunctionIsNeeded) {
    addAfterSchemeFunctionToScore ();
    
    fAfterSchemeFunctionIsNeeded = true;    
  }
}

void lpsrScore::addDateAndTimeSchemeFunctionsToScore ()
{
  string
    schemeFunctionName =
      "date & time",
      
    schemeFunctionDescription =
R"(
% A set of functions to obtain a source file's modification time.
)",

    schemeFunctionCode =
R"(
#(define comml           (object->string (command-line)))
#(define loc             (+ (string-rindex comml #\space ) 2))
#(define commllen        (- (string-length comml) 2))
#(define filen           (substring comml loc commllen))
#(define siz             (object->string (stat:size (stat filen))))
#(define ver             (object->string (lilypond-version)))
#(define dat             (strftime "%d/%m/%Y" (localtime (current-time))))
#(define tim             (strftime "%H:%M:%S" (localtime (current-time))))
#(define modTime         (stat:mtime (stat filen)))
#(define modTimeAsString (strftime "%d/%m/%Y - %H:%M:%S" (localtime modTime)))
)";

  if (gLpsrOptions->fTraceSchemeFunctions) {
    gLogIOstream <<
      "Creating Scheme functions for '" << schemeFunctionName << "'" <<
      endl;
  }

  // create the Scheme function
  S_lpsrSchemeFunction
    schemeFunction =
      lpsrSchemeFunction::create (
        1, // inputLineNumber, JMI ???
        schemeFunctionName,
        schemeFunctionDescription,
        schemeFunctionCode);

  // register it in the Scheme functions map
  fScoreSchemeFunctionsMap [schemeFunctionName] =
    schemeFunction;
}

void lpsrScore::addCustomShortBarLineSchemeFunctionToScore ()
{
  string
    schemeModulesName =
      "curstom short barline Scheme function",
      
    schemeModulesDescription =
R"(
% The function needed to produce curstom short barlines.
)",

    schemeModulesCode =
R"(
#(define ((make-custom-short-bar-line x y) grob extent)
   "Draw a short bar line."
   (let* ((short-staff (* 1/2 (ly:staff-symbol-staff-space grob)))
          (staff-line-thickness (ly:staff-symbol-line-thickness grob))
          (height (interval-end extent)))
     (bar-line::draw-filled-box
      (cons 0 (+ x staff-line-thickness))
      (cons (- height (* 7 short-staff) x) (- height short-staff x))
      staff-line-thickness
      extent
      grob)))

#(add-bar-glyph-print-procedure "/" (make-custom-short-bar-line 0.1 0.1))

#(define-bar-line "/" "/" #f #f)
)";


  if (gLpsrOptions->fTraceSchemeFunctions) {
    gLogIOstream <<
      "Including Jianpu definition file '" << schemeModulesName << "'" <<
      endl;
  }

  // create the Scheme function
  S_lpsrSchemeFunction
    schemeFunction =
      lpsrSchemeFunction::create (
        1, // inputLineNumber, JMI ???
        schemeModulesName,
        schemeModulesDescription,
        schemeModulesCode);

  // register it in the Scheme functions map
  fScoreSchemeFunctionsMap [schemeModulesName] =
    schemeFunction;
}

void lpsrScore::addJianpuFileIncludeToScore ()
{
  string
    schemeModulesName =
      "jianpu include file",
      
    schemeModulesDescription =
R"(
% The definitions needed to produce jianpu scores.
)",

    schemeModulesCode =
R"(
\include "jianpu10a.ly"
)";

  if (gLpsrOptions->fTraceSchemeFunctions) {
    gLogIOstream <<
      "Including Jianpu definition file '" << schemeModulesName << "'" <<
      endl;
  }

  // create the Scheme function
  S_lpsrSchemeFunction
    schemeFunction =
      lpsrSchemeFunction::create (
        1, // inputLineNumber, JMI ???
        schemeModulesName,
        schemeModulesDescription,
        schemeModulesCode);

  // register it in the Scheme functions map
  fScoreSchemeFunctionsMap [schemeModulesName] =
    schemeFunction;
}

void lpsrScore::addAccordionRegistrationSchemeModulesToScore ()
{
  string
    schemeModulesName =
      "scm & accreg",
      
    schemeModulesDescription =
R"(
% Two modules are to be used in the right order to use accordion registration.
)",

    schemeModulesCode =
R"(
#(use-modules (scm accreg))
)";

  if (gLpsrOptions->fTraceSchemeFunctions) {
    gLogIOstream <<
      "Using Scheme modules '" << schemeModulesName << "'" <<
      endl;
  }

  // create the Scheme function
  S_lpsrSchemeFunction
    schemeFunction =
      lpsrSchemeFunction::create (
        1, // inputLineNumber, JMI ???
        schemeModulesName,
        schemeModulesDescription,
        schemeModulesCode);

  // register it in the Scheme functions map
  fScoreSchemeFunctionsMap [schemeModulesName] =
    schemeFunction;
}

void lpsrScore::addTongueSchemeFunctionToScore ()
{
  string
    schemeFunctionName =
      "tongue",
      
    schemeFunctionDescription =
R"(
% Creates multiple tongue technicals, argument is a number.
% Example: 'c4 -\tongue #3' creates a triple tongue.
)",

    schemeFunctionCode =
R"(
tongue =
#(define-music-function (parser location dots) (integer?)
   (let ((script (make-music 'ArticulationEvent
                   'articulation-type "staccato")))
     (set! (ly:music-property script 'tweaks)
           (acons 'stencil
             (lambda (grob)
               (let ((stil (ly:script-interface::print grob)))
                 (let loop ((count (1- dots)) (new-stil stil))
                   (if (> count 0)
                       (loop (1- count)
                         (ly:stencil-combine-at-edge new-stil X RIGHT stil 0.2))
                       (ly:stencil-aligned-to new-stil X CENTER)))))
             (ly:music-property script 'tweaks)))
     script))
)";

  if (gLpsrOptions->fTraceSchemeFunctions) {
    gLogIOstream <<
      "Creating Scheme function '" << schemeFunctionName << "'" <<
      endl;
  }

  // create the Scheme function
  S_lpsrSchemeFunction
    schemeFunction =
      lpsrSchemeFunction::create (
        1, // inputLineNumber, JMI ???
        schemeFunctionName,
        schemeFunctionDescription,
        schemeFunctionCode);

  // register it in the Scheme functions map
  fScoreSchemeFunctionsMap [schemeFunctionName] =
    schemeFunction;
}

void lpsrScore::addEditorialAccidentalSchemeFunctionToScore ()
{
  string
    schemeFunctionName =
      "editorialAccidental",
      
    schemeFunctionDescription =
R"(
% Craetes editorial accidentals as LilyPond musica ficta.
% Example: '\editorialAccidental cis4'.
)",

    schemeFunctionCode =
R"(
editorialAccidental =
#(define-music-function
  (note)
  (ly:music?)
  #{
    \once\accidentalStyle forget
    \once\set suggestAccidentals = ##t
    #note
  #})
)";

  if (gLpsrOptions->fTraceSchemeFunctions) {
    gLogIOstream <<
      "Creating Scheme function '" << schemeFunctionName << "'" <<
      endl;
  }

  // create the Scheme function
  S_lpsrSchemeFunction
    schemeFunction =
      lpsrSchemeFunction::create (
        1, // inputLineNumber, JMI ???
        schemeFunctionName,
        schemeFunctionDescription,
        schemeFunctionCode);

  // register it in the Scheme functions map
  fScoreSchemeFunctionsMap [schemeFunctionName] =
    schemeFunction;
}

void lpsrScore::addDynamicsSchemeFunctionToScore ()
{
  string
    schemeFunctionName =
      "dynamics",
      
    schemeFunctionDescription =
R"(
% Creates variables define dynamics not native to LilyPond.
)",

    schemeFunctionCode =
R"(
rf = #(make-dynamic-script "rf")
sfpp = #(make-dynamic-script "sfpp")
sffz = #(make-dynamic-script "sffz")
ppppp = #(make-dynamic-script "ppppp")
pppppp = #(make-dynamic-script "pppppp")
fffff = #(make-dynamic-script "fffff")
ffffff = #(make-dynamic-script "ffffff")
)";

  if (gLpsrOptions->fTraceSchemeFunctions) {
    gLogIOstream <<
      "Creating Scheme function '" << schemeFunctionName << "'" <<
      endl;
  }

  // create the Scheme function
  S_lpsrSchemeFunction
    schemeFunction =
      lpsrSchemeFunction::create (
        1, // inputLineNumber, JMI ???
        schemeFunctionName,
        schemeFunctionDescription,
        schemeFunctionCode);

  // register it in the Scheme functions map
  fScoreSchemeFunctionsMap [schemeFunctionName] =
    schemeFunction;
}

void lpsrScore::addTupletsCurvedBracketsSchemeFunctionToScore ()
{
  string
    schemeFunctionName =
      "tupletsCurvedBrackets",
      
    schemeFunctionDescription =
R"(
% A function to draw curved tuplets brackets, not native to LilyPond.
% Thanks to Ben, mailto:soundsfromsound@gmail.com
)",

    schemeFunctionCode =
R"(
tupletsCurvedBrackets = {
  % Use slur-stencil
  \override TupletBracket.stencil = #ly:slur::print
  %% Use 'thickness from Slur
  \override TupletBracket.thickness = #1.2
  %% 'control-points need to be set
  \override TupletBracket.control-points =
  #(lambda (grob)
     (let* ((x-pos (ly:grob-property grob 'X-positions))
            (pos (ly:grob-property grob 'positions))
            (x-ln (interval-length x-pos))
            (dir (ly:grob-property grob 'direction))
            ;; read out the height of the TupletBracket, may be
            ;; negative!
            (height (- (cdr pos) (car pos)))
            ;; height-corr is introduced because sometimes the shape
            ;; of the slur needs to be adjusted.
            ;; It is used in the 2nd/3rd control-point.
            ;; The value of 0.3 is found by trial and error
            (height-corr (* 0.3 dir height))
            (edge-height (ly:grob-property grob 'edge-height
                           '(0.7 . 0.7)))
            (pad 1.0))
       (list
        ;; first cp
        (cons
         (+ (car x-pos) 0.5)
         (- (+ (* dir pad) (+ (car pos) (* -1 dir
                                          (car edge-height))))
           (if (= dir -1)
               (if (> height 3)
                   (/ dir 2.0)
                   0.0)
               (if (< height -3)
                   (/ dir 2.0)
                   0.0))))
        ;; second cp
        (cons
         (+ (car x-pos) (* x-ln 1/4))
         (+ (* dir pad) (+ (car pos) (* dir (+ 0.5 height-corr)))))
        ;; third cp
        (cons
         (+ (car x-pos) (* x-ln 3/4))
         (+ (* dir pad) (+ (cdr pos) (* dir (- 0.5 height-corr)))))
        ;; fourth cp
        (cons
         (- (cdr x-pos) 0.5)
         (+ (* dir pad) (+ (cdr pos) (* -1 dir (cdr edge-height)))))
        )))
  \override TupletBracket.staff-padding = #'()
  #(define (invert-direction x) (if (eq? UP
                                         (ly:tuplet-bracket::calc-direction x)) DOWN UP))
  % \override TupletBracket.direction = #invert-direction
}
)";

  if (gLpsrOptions->fTraceSchemeFunctions) {
    gLogIOstream <<
      "Creating Scheme function '" << schemeFunctionName << "'" <<
      endl;
  }

  // create the Scheme function
  S_lpsrSchemeFunction
    schemeFunction =
      lpsrSchemeFunction::create (
        1, // inputLineNumber, JMI ???
        schemeFunctionName,
        schemeFunctionDescription,
        schemeFunctionCode);

  // register it in the Scheme functions map
  fScoreSchemeFunctionsMap [schemeFunctionName] =
    schemeFunction;
}

void lpsrScore::addAfterSchemeFunctionToScore ()
{
  string
    schemeFunctionName =
      "after",
      
    schemeFunctionDescription =
R"(
% A function to create events after given music.
% Thanks to David Kastrup for the inspiration!
)",

    schemeFunctionCode =
R"(
after =
#(define-music-function (t e m) (ly:duration? ly:music? ly:music?)
   #{
     \context Bottom <<
       #m
       { \skip $t <> -\tweak extra-spacing-width #empty-interval $e }
     >>
   #})
)";

  if (gLpsrOptions->fTraceSchemeFunctions) {
    gLogIOstream <<
      "Creating Scheme function '" << schemeFunctionName << "'" <<
      endl;
  }

  // create the Scheme function
  S_lpsrSchemeFunction
    schemeFunction =
      lpsrSchemeFunction::create (
        1, // inputLineNumber, JMI ???
        schemeFunctionName,
        schemeFunctionDescription,
        schemeFunctionCode);

  // register it in the Scheme functions map
  fScoreSchemeFunctionsMap [schemeFunctionName] =
    schemeFunction;
}

/* JMI
void lpsrScore::appendVoiceUseToStoreCommand (S_msrVoice voice)
{
  S_lpsrUseVoiceCommand
    useVoiceCommand =
      lpsrUseVoiceCommand::create (
        fInputLineNumber,
        voice);
  
  fScoreBlock->
    appendVoiceUseToParallelMusicBLock (useVoiceCommand);
}

void lpsrScore::appendLyricsUseToStoreCommand (S_msrStanza stanza)
{
  S_lpsrNewLyricsBlock
    newLyricsCommand =
      lpsrNewLyricsBlock::create (
        fInputLineNumber,
        stanza,
        stanza->getStanzaVoiceUplink ());
  
  fScoreBlock->
    appendLyricsUseToParallelMusicBLock (newLyricsCommand);
}
*/

void lpsrScore::acceptIn (basevisitor* v)
{
  if (gLpsrOptions->fTraceLpsrVisitors) {
    gLogIOstream <<
      "% ==> lpsrScore::acceptIn()" <<
      endl;
  }
      
  if (visitor<S_lpsrScore>*
    p =
      dynamic_cast<visitor<S_lpsrScore>*> (v)) {
        S_lpsrScore elem = this;
        
        if (gLpsrOptions->fTraceLpsrVisitors) {
          gLogIOstream <<
            "% ==> Launching lpsrScore::visitStart()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void lpsrScore::acceptOut (basevisitor* v)
{
  if (gLpsrOptions->fTraceLpsrVisitors) {
    gLogIOstream <<
      "% ==> lpsrScore::acceptOut()" <<
      endl;
  }

  if (visitor<S_lpsrScore>*
    p =
      dynamic_cast<visitor<S_lpsrScore>*> (v)) {
        S_lpsrScore elem = this;
      
        if (gLpsrOptions->fTraceLpsrVisitors) {
          gLogIOstream <<
            "% ==> Launching lpsrScore::visitEnd()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void lpsrScore::browseData (basevisitor* v)
{
  if (gLpsrOptions->fTraceLpsrVisitors) {
    gLogIOstream <<
      "% ==> lpsrScore::browseData()" <<
      endl;
  }

  {
    // browse the score LilyPond version
    msrBrowser<lpsrVarValAssoc> browser (v);
    browser.browse (*fLilypondVersion);
  }

  {
    // browse the input source name comment
    msrBrowser<lpsrComment> browser (v);
    browser.browse (*fInputSourceNameComment);
  }

  {
    // browse the translation date comment
    msrBrowser<lpsrComment> browser (v);
    browser.browse (*fTranslationDateComment);
  }

  {
    // browse the command line long options comment
    msrBrowser<lpsrComment> browser (v);
    browser.browse (*fCommandLineLongOptionsComment);
  }

  if (fCommandLineShortOptionsComment) {
    // browse the command line short options comment
    msrBrowser<lpsrComment> browser (v);
    browser.browse (*fCommandLineShortOptionsComment);
  }

  {
    // browse the score global size
    msrBrowser<lpsrSchemeVariable> browser (v);
    browser.browse (*fGlobalStaffSizeAssoc);
  }

  {
    // browse the Scheme function map
    for (
      map<string, S_lpsrSchemeFunction>::const_iterator i =
        fScoreSchemeFunctionsMap.begin ();
      i != fScoreSchemeFunctionsMap.end ();
      i++) {
      // browse the Scheme function
      msrBrowser<lpsrSchemeFunction> browser (v);
      browser.browse (*(*i).second);
    } // for
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
    // browse the myBreakIsBreak assoc
    msrBrowser<lpsrVarValAssoc> browser (v);
    browser.browse (*fMyBreakIsBreakAssoc);
  }
  {
    // browse the myBreakIsEmpty assoc
    msrBrowser<lpsrVarValAssoc> browser (v);
    browser.browse (*fMyBreakIsEmptyAssoc);
  }

  {
    // browse the myPageBreakIsPageBreak assoc
    msrBrowser<lpsrVarValAssoc> browser (v);
    browser.browse (*fMyPageBreakIsPageBreakAssoc);
  }
  {
    // browse the myPageBreakIsEmpty assoc
    msrBrowser<lpsrVarValAssoc> browser (v);
    browser.browse (*fMyPageBreakIsEmptyAssoc);
  }

  if (fGlobalAssoc) {
    // browse the 'global' assoc
    msrBrowser<lpsrVarValAssoc> browser (v);
    browser.browse (*fGlobalAssoc);
  }

  {
    // browse the voices and stanzas list
    for (
      list<S_msrElement>::const_iterator i = fScoreElements.begin ();
      i != fScoreElements.end ();
      i++) {
      // browse the element
      msrBrowser<msrElement> browser (v);
      browser.browse (*(*i));
    } // for
  }

  {
    // browse the score command
    msrBrowser<lpsrScoreBlock> browser (v);    
    browser.browse (*fScoreBlock);
  }

  if (gLpsrOptions->fTraceLpsrVisitors) {
    gLogIOstream <<
      "% <== lpsrScore::browseData()" <<
      endl;
  }
}

ostream& operator<< (ostream& os, const S_lpsrScore& scr)
{
  scr->print (os);
  return os;
}

void lpsrScore::print (ostream& os)
{
  os <<
    "LPSR Score" <<
    endl <<
    endl;

  gIndenter++;

  // print the MSR structure (without the voices)
  fMsrScore->
    printSummary (os);
  os <<
    endl;

  // are some Scheme functions needed?
  const int fieldWidth = 42;
  
  os << left <<
    setw (fieldWidth) <<
    "TongueSchemeFunctionIsNeeded" << " : " <<
    booleanAsString (
      fTongueSchemeFunctionIsNeeded) <<
    endl <<
    setw (fieldWidth) <<
    "EditorialAccidentalSchemeFunctionIsNeeded" << " : " <<
    booleanAsString (
      fEditorialAccidentalSchemeFunctionIsNeeded) <<
    endl <<
    endl;

  // print LPSR basic information
  os <<
    fLilypondVersion <<
    endl <<
    
    fGlobalStaffSizeAssoc <<
    endl <<

    fHeader <<
    // no endl here

    fPaper <<
    endl <<

    fScoreLayout <<
    endl;

// myBreakAssoc,myPageBreakAssoc globalAssoc? JMI

  // print the voices
  if (fScoreElements.size ()) {  
    list<S_msrElement>::const_iterator
      iBegin = fScoreElements.begin (),
      iEnd   = fScoreElements.end (),
      i      = iBegin;
    for ( ; ; ) {
      os << (*i);
      if (++i == iEnd) break;
      os << endl;
    } // for

    os <<
      endl;
  }

  // print the score block
  os <<
    fScoreBlock;

  gIndenter--;
}

//______________________________________________________________________________
void initializeLPSR ()
{
  // LPSR accidental styles handling
  // ------------------------------------------------------

  initializeLpsrAccidentalStyleKindsMap ();

  // LPSR chords languages handling
  // ------------------------------------------------------

  initializeLpsrChordsLanguageKindsMap ();
}


}
