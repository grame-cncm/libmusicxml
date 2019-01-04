/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#include "lpsrContexts.h"

#include "lpsrOptions.h"


using namespace std;

namespace MusicXML2 
{

//______________________________________________________________________________
S_lpsrContext lpsrContext::create (
  int                     inputLineNumber,
  lpsrContextExistingKind contextExistingKind,
  lpsrContextTypeKind     contextTypeKind,
  string                  contextName)
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
  int                     inputLineNumber,
  lpsrContextExistingKind contextExistingKind,
  lpsrContextTypeKind     contextTypeKind,
  string                  contextName)
    : lpsrElement (inputLineNumber)
{
  fContextExistingKind = contextExistingKind;
  fContextTypeKind = contextTypeKind;
  fContextName = contextName; 
}

lpsrContext::~lpsrContext ()
{}

void lpsrContext::acceptIn (basevisitor* v)
{
  if (gLpsrOptions->fTraceLpsrVisitors) {
    gLogIOstream <<
      "% ==> lpsrContext::acceptIn ()" <<
      endl;
  }
      
  if (visitor<S_lpsrContext>*
    p =
      dynamic_cast<visitor<S_lpsrContext>*> (v)) {
        S_lpsrContext elem = this;
        
        if (gLpsrOptions->fTraceLpsrVisitors) {
          gLogIOstream <<
            "% ==> Launching lpsrContext::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void lpsrContext::acceptOut (basevisitor* v)
{
  if (gLpsrOptions->fTraceLpsrVisitors) {
    gLogIOstream <<
      "% ==> lpsrContext::acceptOut ()" <<
      endl;
  }

  if (visitor<S_lpsrContext>*
    p =
      dynamic_cast<visitor<S_lpsrContext>*> (v)) {
        S_lpsrContext elem = this;
      
        if (gLpsrOptions->fTraceLpsrVisitors) {
          gLogIOstream <<
            "% ==> Launching lpsrContext::visitEnd ()" <<
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
    case lpsrContext::kChordNames:
      result = "ChordNames";
      break;
    case lpsrContext::kFiguredBass:
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
      result = "existingContextYes";
      break;
    case kExistingContextNo:
      result = "existingContextNo";
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
    
  if (fContextName.size ()) {
    os << " " << fContextName;
  }
  os <<
    endl;
  
  gIndenter++;

  os <<
    "ContextElements";

  if (fContextElementsList.size ()) {
    os <<
      endl;
      
    gIndenter++;
  
    list<S_msrElement>::const_iterator
      iBegin = fContextElementsList.begin (),
      iEnd   = fContextElementsList.end (),
      i      = iBegin;
    for ( ; ; ) {
      os << (*i);
      if (++i == iEnd) break;
 // JMI     os << endl;
    } // for
    
    gIndenter--;
  }
  else {
    os << "none" <<
    endl;
  }

  gIndenter--;
}

//______________________________________________________________________________
S_lpsrChordNamesContext lpsrChordNamesContext::create (
  int                     inputLineNumber,
  lpsrContextExistingKind contextExistingKind,
  string                  contextName,
  S_msrVoice              contextVoice)
{
  lpsrChordNamesContext* o =
    new lpsrChordNamesContext (
      inputLineNumber,
      contextExistingKind,
      contextName,
      contextVoice);
  assert(o!=0);
  return o;
}

lpsrChordNamesContext::lpsrChordNamesContext (
  int                     inputLineNumber,
  lpsrContextExistingKind contextExistingKind,
  string                  contextName,
  S_msrVoice              contextVoice)
    : lpsrContext (
      inputLineNumber,
      contextExistingKind,
      lpsrContext::kChordNames,
      contextName)
{
  fContextExistingKind = contextExistingKind;
    
  fContextName = contextName;

  fContextVoice = contextVoice;
}

lpsrChordNamesContext::~lpsrChordNamesContext ()
{}

void lpsrChordNamesContext::acceptIn (basevisitor* v)
{
  if (gLpsrOptions->fTraceLpsrVisitors) {
    gLogIOstream <<
      "% ==> lpsrChordNamesContext::acceptIn ()" <<
      endl;
  }
      
  if (visitor<S_lpsrChordNamesContext>*
    p =
      dynamic_cast<visitor<S_lpsrChordNamesContext>*> (v)) {
        S_lpsrChordNamesContext elem = this;
        
        if (gLpsrOptions->fTraceLpsrVisitors) {
          gLogIOstream <<
            "% ==> Launching lpsrChordNamesContext::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void lpsrChordNamesContext::acceptOut (basevisitor* v)
{
  if (gLpsrOptions->fTraceLpsrVisitors) {
    gLogIOstream <<
      "% ==> lpsrChordNamesContext::acceptOut ()" <<
      endl;
  }

  if (visitor<S_lpsrChordNamesContext>*
    p =
      dynamic_cast<visitor<S_lpsrChordNamesContext>*> (v)) {
        S_lpsrChordNamesContext elem = this;
      
        if (gLpsrOptions->fTraceLpsrVisitors) {
          gLogIOstream <<
            "% ==> Launching lpsrChordNamesContext::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void lpsrChordNamesContext::browseData (basevisitor* v)
{}

void lpsrChordNamesContext::print (ostream& os)
{  
  os <<
  /* JMI
    "ChordNamesContext, \"" <<
    contextTypeKindAsString (
      fContextTypeKind) <<
    "\"" <<
    */
    "ChordNamesContext" <<
    endl;
    
  gIndenter++;

  const int fieldWidth = 20;

  os << left <<
    setw (fieldWidth) <<
    "contextExistingKind" << " : " <<
    contextExistingKindAsString (
      fContextExistingKind) <<
    endl <<
    setw (fieldWidth) <<
    "contextName" << " : \"" << fContextName << "\"" <<
    endl <<
    setw (fieldWidth) <<
    "contextVoice" << " : \"" << fContextVoice->getVoiceName () << "\"" <<
    endl;

  os <<
    setw (fieldWidth) <<
    "ContextElements";

  if (fContextElementsList.size ()) {
    os <<
      endl;
      
    gIndenter++;
  
    list<S_msrElement>::const_iterator
      iBegin = fContextElementsList.begin (),
      iEnd   = fContextElementsList.end (),
      i      = iBegin;
    for ( ; ; ) {
      os << (*i);
      if (++i == iEnd) break;
 // JMI     os << endl;
    } // for
    
    gIndenter--;
  }
  else {
    os <<
      " : " << "none" <<
    endl;
  }
  
  gIndenter--;
}

ostream& operator<< (ostream& os, const S_lpsrChordNamesContext& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_lpsrFiguredBassContext lpsrFiguredBassContext::create (
  int                     inputLineNumber,
  lpsrContextExistingKind contextExistingKind,
  string                  contextName,
  S_msrStaff              contextStaff)
{
  lpsrFiguredBassContext* o =
    new lpsrFiguredBassContext (
      inputLineNumber,
      contextExistingKind,
      contextName,
      contextStaff);
  assert(o!=0);
  return o;
}

lpsrFiguredBassContext::lpsrFiguredBassContext (
  int                     inputLineNumber,
  lpsrContextExistingKind contextExistingKind,
  string                  contextName,
  S_msrStaff              contextStaff)
    : lpsrContext (
      inputLineNumber,
      contextExistingKind,
      lpsrContext::kFiguredBass,
      contextName)
{
  fContextExistingKind = contextExistingKind;
    
  fContextName = contextName;

  fContextStaff = contextStaff;
}

lpsrFiguredBassContext::~lpsrFiguredBassContext ()
{}

void lpsrFiguredBassContext::acceptIn (basevisitor* v)
{
  if (gLpsrOptions->fTraceLpsrVisitors) {
    gLogIOstream <<
      "% ==> lpsrFiguredBassContext::acceptIn ()" <<
      endl;
  }
      
  if (visitor<S_lpsrFiguredBassContext>*
    p =
      dynamic_cast<visitor<S_lpsrFiguredBassContext>*> (v)) {
        S_lpsrFiguredBassContext elem = this;
        
        if (gLpsrOptions->fTraceLpsrVisitors) {
          gLogIOstream <<
            "% ==> Launching lpsrFiguredBassContext::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void lpsrFiguredBassContext::acceptOut (basevisitor* v)
{
  if (gLpsrOptions->fTraceLpsrVisitors) {
    gLogIOstream <<
      "% ==> lpsrFiguredBassContext::acceptOut ()" <<
      endl;
  }

  if (visitor<S_lpsrFiguredBassContext>*
    p =
      dynamic_cast<visitor<S_lpsrFiguredBassContext>*> (v)) {
        S_lpsrFiguredBassContext elem = this;
      
        if (gLpsrOptions->fTraceLpsrVisitors) {
          gLogIOstream <<
            "% ==> Launching lpsrFiguredBassContext::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void lpsrFiguredBassContext::browseData (basevisitor* v)
{}

void lpsrFiguredBassContext::print (ostream& os)
{  
  os <<
  /* JMI
    "FiguredBassContext, \"" <<
    contextTypeKindAsString (
      fContextTypeKind) <<
    "\"" <<
    */
    "FiguredBassContext" <<
    endl;
    
  gIndenter++;

  const int fieldWidth = 20;

  os << left <<
    setw (fieldWidth) <<
    "contextExistingKind" << " : " <<
    contextExistingKindAsString (
      fContextExistingKind) <<
    endl <<
    setw (fieldWidth) <<
    "contextName" << " : \"" << fContextName << "\"" <<
    endl <<
    setw (fieldWidth) <<
    "contextStaff" << " : \"" << fContextStaff->getStaffName () << "\"" <<
    endl;

  os <<
    setw (fieldWidth) <<
    "ContextElements";

  if (fContextElementsList.size ()) {
    os <<
      endl;
      
    gIndenter++;
  
    list<S_msrElement>::const_iterator
      iBegin = fContextElementsList.begin (),
      iEnd   = fContextElementsList.end (),
      i      = iBegin;
    for ( ; ; ) {
      os << (*i);
      if (++i == iEnd) break;
 // JMI     os << endl;
    } // for
    
    gIndenter--;
  }
  else {
    os <<
      " : " << "none" <<
    endl;
  }
  
  gIndenter--;
}

ostream& operator<< (ostream& os, const S_lpsrFiguredBassContext& elt)
{
  elt->print (os);
  return os;
}


}
