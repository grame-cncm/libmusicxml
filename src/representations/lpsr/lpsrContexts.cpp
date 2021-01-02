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

#include "oahOah.h"

#include "lpsrOah.h"


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
  assert (o!=0);
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
#ifdef TRACING_IS_ENABLED
  if (gGlobalLpsrOahGroup->getTraceLpsrVisitors ()) {
    gLogStream <<
      "% ==> lpsrContext::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_lpsrContext>*
    p =
      dynamic_cast<visitor<S_lpsrContext>*> (v)) {
        S_lpsrContext elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalLpsrOahGroup->getTraceLpsrVisitors ()) {
          gLogStream <<
            "% ==> Launching lpsrContext::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void lpsrContext::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalLpsrOahGroup->getTraceLpsrVisitors ()) {
    gLogStream <<
      "% ==> lpsrContext::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_lpsrContext>*
    p =
      dynamic_cast<visitor<S_lpsrContext>*> (v)) {
        S_lpsrContext elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalLpsrOahGroup->getTraceLpsrVisitors ()) {
          gLogStream <<
            "% ==> Launching lpsrContext::visitEnd ()" <<
            endl;
        }
#endif
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

void lpsrContext::print (ostream& os) const
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
  os << endl;

  ++gIndenter;

  os <<
    "ContextElements";

  if (fContextElementsList.size ()) {
    os << endl;

    ++gIndenter;

    list<S_msrElement>::const_iterator
      iBegin = fContextElementsList.begin (),
      iEnd   = fContextElementsList.end (),
      i      = iBegin;
    for ( ; ; ) {
      os << (*i);
      if (++i == iEnd) break;
 // JMI     os << endl;
    } // for

    --gIndenter;
  }
  else {
    os << "none" <<
    endl;
  }

  --gIndenter;
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
  assert (o!=0);
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
#ifdef TRACING_IS_ENABLED
  if (gGlobalLpsrOahGroup->getTraceLpsrVisitors ()) {
    gLogStream <<
      "% ==> lpsrChordNamesContext::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_lpsrChordNamesContext>*
    p =
      dynamic_cast<visitor<S_lpsrChordNamesContext>*> (v)) {
        S_lpsrChordNamesContext elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalLpsrOahGroup->getTraceLpsrVisitors ()) {
          gLogStream <<
            "% ==> Launching lpsrChordNamesContext::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void lpsrChordNamesContext::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalLpsrOahGroup->getTraceLpsrVisitors ()) {
    gLogStream <<
      "% ==> lpsrChordNamesContext::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_lpsrChordNamesContext>*
    p =
      dynamic_cast<visitor<S_lpsrChordNamesContext>*> (v)) {
        S_lpsrChordNamesContext elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalLpsrOahGroup->getTraceLpsrVisitors ()) {
          gLogStream <<
            "% ==> Launching lpsrChordNamesContext::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void lpsrChordNamesContext::browseData (basevisitor* v)
{}

void lpsrChordNamesContext::print (ostream& os) const
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

  ++gIndenter;

  const unsigned int fieldWidth = 20;

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
    os << endl;

    ++gIndenter;

    list<S_msrElement>::const_iterator
      iBegin = fContextElementsList.begin (),
      iEnd   = fContextElementsList.end (),
      i      = iBegin;
    for ( ; ; ) {
      os << (*i);
      if (++i == iEnd) break;
 // JMI     os << endl;
    } // for

    --gIndenter;
  }
  else {
    os <<
      " : " << "none" <<
    endl;
  }

  --gIndenter;
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
  assert (o!=0);
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
#ifdef TRACING_IS_ENABLED
  if (gGlobalLpsrOahGroup->getTraceLpsrVisitors ()) {
    gLogStream <<
      "% ==> lpsrFiguredBassContext::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_lpsrFiguredBassContext>*
    p =
      dynamic_cast<visitor<S_lpsrFiguredBassContext>*> (v)) {
        S_lpsrFiguredBassContext elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalLpsrOahGroup->getTraceLpsrVisitors ()) {
          gLogStream <<
            "% ==> Launching lpsrFiguredBassContext::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void lpsrFiguredBassContext::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalLpsrOahGroup->getTraceLpsrVisitors ()) {
    gLogStream <<
      "% ==> lpsrFiguredBassContext::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_lpsrFiguredBassContext>*
    p =
      dynamic_cast<visitor<S_lpsrFiguredBassContext>*> (v)) {
        S_lpsrFiguredBassContext elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalLpsrOahGroup->getTraceLpsrVisitors ()) {
          gLogStream <<
            "% ==> Launching lpsrFiguredBassContext::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void lpsrFiguredBassContext::browseData (basevisitor* v)
{}

void lpsrFiguredBassContext::print (ostream& os) const
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

  ++gIndenter;

  const unsigned int fieldWidth = 20;

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
    os << endl;

    ++gIndenter;

    list<S_msrElement>::const_iterator
      iBegin = fContextElementsList.begin (),
      iEnd   = fContextElementsList.end (),
      i      = iBegin;
    for ( ; ; ) {
      os << (*i);
      if (++i == iEnd) break;
 // JMI     os << endl;
    } // for

    --gIndenter;
  }
  else {
    os <<
      " : " << "none" <<
    endl;
  }

  --gIndenter;
}

ostream& operator<< (ostream& os, const S_lpsrFiguredBassContext& elt)
{
  elt->print (os);
  return os;
}


}
