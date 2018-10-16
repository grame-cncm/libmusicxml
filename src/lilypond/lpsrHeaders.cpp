/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#include <iomanip>      // setw, setprecision, ...

#include "lpsrHeaders.h"

#include "setTraceOptionsIfDesired.h"
#ifdef TRACE_OPTIONS
  #include "traceOptions.h"
#endif

#include "lpsrOptions.h"


using namespace std;

namespace MusicXML2 
{

//______________________________________________________________________________
S_lpsrHeader lpsrHeader::create (
  int inputLineNumber)
{
  lpsrHeader* o =
    new lpsrHeader (
      inputLineNumber);
  assert(o!=0);
  return o;
}

lpsrHeader::lpsrHeader (
  int inputLineNumber)
    : lpsrElement (inputLineNumber)
{
  // grab values from the LilyPond options if any
  if (gLilypondOptions->fDedication.size ()) {
    setLilypondDedication (
      inputLineNumber,
      gLilypondOptions->fDedication);
  }
  
  if (gLilypondOptions->fPiece.size ()) {
    setLilypondPiece (
      inputLineNumber,
      gLilypondOptions->fPiece);
  }
  
  if (gLilypondOptions->fOpus.size ()) {
    setLilypondOpus (
      inputLineNumber,
      gLilypondOptions->fOpus);
  }
  
  if (gLilypondOptions->fTitle.size ()) {
    setLilypondTitle (
      inputLineNumber,
      gLilypondOptions->fTitle);
  }
  if (gLilypondOptions->fSubTitle.size ()) {
    setLilypondSubTitle (
      inputLineNumber,
      gLilypondOptions->fSubTitle);
  }
  if (gLilypondOptions->fSubSubTitle.size ()) {
    setLilypondSubSubTitle (
      inputLineNumber,
      gLilypondOptions->fSubSubTitle);
  }
  
  if (gLilypondOptions->fInstrument.size ()) {
    setLilypondInstrument (
      inputLineNumber,
      gLilypondOptions->fInstrument);
  }
  if (gLilypondOptions->fMeter.size ()) {
    setLilypondMeter (
      inputLineNumber,
      gLilypondOptions->fMeter);
  }
  
  if (gLilypondOptions->fCopyright.size ()) {
    setLilypondCopyright (
      inputLineNumber,
      gLilypondOptions->fCopyright);
  }
  if (gLilypondOptions->fTagline.size ()) {
    setLilypondTagline (
      inputLineNumber,
      gLilypondOptions->fTagline);
  }
}

lpsrHeader::~lpsrHeader ()
{}

void lpsrHeader::setLilypondDedication (
  int    inputLineNumber,
  string val)
{
  fLilypondDedication =
    lpsrVarValAssoc::create (
      inputLineNumber,
      lpsrVarValAssoc::kUncommented,
      lpsrVarValAssoc::kWithoutBackSlash,
      lpsrVarValAssoc::kLilypondDedication,
      lpsrVarValAssoc::kEqualSign,
      lpsrVarValAssoc::kQuotesAroundValue,
      val,
      lpsrVarValAssoc::g_LilyPondVarValAssocNoUnit,
      lpsrVarValAssoc::g_LilyPondVarValAssocNoComment,
      lpsrVarValAssoc::kWithoutEndl);
}

void lpsrHeader::setLilypondPiece (
  int    inputLineNumber,
  string val)
{
  fLilypondPiece =
    lpsrVarValAssoc::create (
      inputLineNumber,
      lpsrVarValAssoc::kUncommented,
      lpsrVarValAssoc::kWithoutBackSlash,
      lpsrVarValAssoc::kLilypondPiece,
      lpsrVarValAssoc::kEqualSign,
      lpsrVarValAssoc::kQuotesAroundValue,
      val,
      lpsrVarValAssoc::g_LilyPondVarValAssocNoUnit,
      lpsrVarValAssoc::g_LilyPondVarValAssocNoComment,
      lpsrVarValAssoc::kWithoutEndl);
}

void lpsrHeader::setLilypondOpus (
  int    inputLineNumber,
  string val)
{
  fLilypondOpus =
    lpsrVarValAssoc::create (
      inputLineNumber,
      lpsrVarValAssoc::kUncommented,
      lpsrVarValAssoc::kWithoutBackSlash,
      lpsrVarValAssoc::kLilypondOpus,
      lpsrVarValAssoc::kEqualSign,
      lpsrVarValAssoc::kQuotesAroundValue,
      val,
      lpsrVarValAssoc::g_LilyPondVarValAssocNoUnit,
      lpsrVarValAssoc::g_LilyPondVarValAssocNoComment,
      lpsrVarValAssoc::kWithoutEndl);
}

void lpsrHeader::setLilypondTitle (
  int    inputLineNumber,
  string val)
{
  fLilypondTitle =
    lpsrVarValAssoc::create (
      inputLineNumber,
      lpsrVarValAssoc::kUncommented,
      lpsrVarValAssoc::kWithoutBackSlash,
      lpsrVarValAssoc::kLilypondTitle,
      lpsrVarValAssoc::kEqualSign,
      lpsrVarValAssoc::kQuotesAroundValue,
      val,
      lpsrVarValAssoc::g_LilyPondVarValAssocNoUnit,
      lpsrVarValAssoc::g_LilyPondVarValAssocNoComment,
      lpsrVarValAssoc::kWithoutEndl);
}

void lpsrHeader::setLilypondSubTitle (
  int    inputLineNumber,
  string val)
{
  fLilypondSubTitle =
    lpsrVarValAssoc::create (
      inputLineNumber,
      lpsrVarValAssoc::kUncommented,
      lpsrVarValAssoc::kWithoutBackSlash,
      lpsrVarValAssoc::kLilypondSubTitle,
      lpsrVarValAssoc::kEqualSign,
      lpsrVarValAssoc::kQuotesAroundValue,
      val,
      lpsrVarValAssoc::g_LilyPondVarValAssocNoUnit,
      lpsrVarValAssoc::g_LilyPondVarValAssocNoComment,
      lpsrVarValAssoc::kWithoutEndl);
}

void lpsrHeader::setLilypondSubSubTitle (
  int    inputLineNumber,
  string val)
{
  fLilypondSubSubTitle =
    lpsrVarValAssoc::create (
      inputLineNumber,
      lpsrVarValAssoc::kUncommented,
      lpsrVarValAssoc::kWithoutBackSlash,
      lpsrVarValAssoc::kLilypondSubSubTitle,
      lpsrVarValAssoc::kEqualSign,
      lpsrVarValAssoc::kQuotesAroundValue,
      val,
      lpsrVarValAssoc::g_LilyPondVarValAssocNoUnit,
      lpsrVarValAssoc::g_LilyPondVarValAssocNoComment,
      lpsrVarValAssoc::kWithoutEndl);
}

void lpsrHeader::setLilypondInstrument (
  int    inputLineNumber,
  string val)
{
  fLilypondInstrument =
    lpsrVarValAssoc::create (
      inputLineNumber,
      lpsrVarValAssoc::kUncommented,
      lpsrVarValAssoc::kWithoutBackSlash,
      lpsrVarValAssoc::kLilypondInstrument,
      lpsrVarValAssoc::kEqualSign,
      lpsrVarValAssoc::kQuotesAroundValue,
      val,
      lpsrVarValAssoc::g_LilyPondVarValAssocNoUnit,
      lpsrVarValAssoc::g_LilyPondVarValAssocNoComment,
      lpsrVarValAssoc::kWithoutEndl);
}

void lpsrHeader::setLilypondMeter (
  int    inputLineNumber,
  string val)
{
  fLilypondMeter =
    lpsrVarValAssoc::create (
      inputLineNumber,
      lpsrVarValAssoc::kUncommented,
      lpsrVarValAssoc::kWithoutBackSlash,
      lpsrVarValAssoc::kLilypondMeter,
      lpsrVarValAssoc::kEqualSign,
      lpsrVarValAssoc::kQuotesAroundValue,
      val,
      lpsrVarValAssoc::g_LilyPondVarValAssocNoUnit,
      lpsrVarValAssoc::g_LilyPondVarValAssocNoComment,
      lpsrVarValAssoc::kWithoutEndl);
}

void lpsrHeader::setLilypondCopyright (
  int    inputLineNumber,
  string val)
{
  fLilypondCopyright =
    lpsrVarValAssoc::create (
      inputLineNumber,
      lpsrVarValAssoc::kUncommented,
      lpsrVarValAssoc::kWithoutBackSlash,
      lpsrVarValAssoc::kLilypondCopyright,
      lpsrVarValAssoc::kEqualSign,
      lpsrVarValAssoc::kQuotesAroundValue,
      val,
      lpsrVarValAssoc::g_LilyPondVarValAssocNoUnit,
      lpsrVarValAssoc::g_LilyPondVarValAssocNoComment,
      lpsrVarValAssoc::kWithoutEndl);
}

void lpsrHeader::setLilypondTagline (
  int    inputLineNumber,
  string val)
{
  fLilypondTagline =
    lpsrVarValAssoc::create (
      inputLineNumber,
      lpsrVarValAssoc::kUncommented,
      lpsrVarValAssoc::kWithoutBackSlash,
      lpsrVarValAssoc::kLilypondTagline,
      lpsrVarValAssoc::kEqualSign,
      lpsrVarValAssoc::kQuotesAroundValue,
      val,
      lpsrVarValAssoc::g_LilyPondVarValAssocNoUnit,
      lpsrVarValAssoc::g_LilyPondVarValAssocNoComment,
      lpsrVarValAssoc::kWithoutEndl);
}

void lpsrHeader::setWorkNumber (
  int    inputLineNumber,
  string val)
{
  fWorkNumber =
    lpsrVarValAssoc::create (
      inputLineNumber,
      lpsrVarValAssoc::kUncommented,
      lpsrVarValAssoc::kWithoutBackSlash,
      lpsrVarValAssoc::kMusicXMLWorkNumber,
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
      lpsrVarValAssoc::kMusicXMLWorkTitle,
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
      lpsrVarValAssoc::kMusicXMLMovementNumber,
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
      lpsrVarValAssoc::kMusicXMLMovementTitle,
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
      lpsrVarValAssoc::kMusicXMLEncodingDate,
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
      lpsrVarValAssoc::kMusicXMLScoreInstrument,
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
      lpsrVarValAssoc::kMusicXMLMiscellaneousField,
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
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceVarValAssocs) {
    gLogIOstream <<
      "Adding right \"" <<
      value <<
      "\" to lpsrHeader" <<
      ", line " << fInputLineNumber <<
      endl;
  }
#endif

  if (! fRights) {
    fRights =
      lpsrVarValsListAssoc::create (
        inputLineNumber,
        lpsrVarValsListAssoc::kMusicXMLRights);
  }
  
  fRights->
    addAssocVariableValue (value);
}

void lpsrHeader::addComposer (
  int    inputLineNumber,
  string value)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceVarValAssocs) {
    gLogIOstream <<
      "Adding composer \"" <<
      value <<
      "\" to lpsrHeader" <<
      ", line " << fInputLineNumber <<
      endl;
  }
#endif

  if (! fComposers) {
    fComposers =
      lpsrVarValsListAssoc::create (
        inputLineNumber,
        lpsrVarValsListAssoc::kMusicXMLComposer);
  }
  
  fComposers->
    addAssocVariableValue (value);
}

void lpsrHeader::addArranger (
  int    inputLineNumber,
  string value)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceVarValAssocs) {
    gLogIOstream <<
      "Adding arranger \"" <<
      value <<
      "\" to lpsrHeader" <<
      ", line " << fInputLineNumber <<
      endl;
  }
#endif

  if (! fArrangers) {
    fArrangers =
      lpsrVarValsListAssoc::create (
        inputLineNumber,
        lpsrVarValsListAssoc::kMusicXMLArranger);
  }
  
  fArrangers->
    addAssocVariableValue (value);
}

void lpsrHeader::addLyricist (
  int    inputLineNumber,
  string value)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceVarValAssocs) {
    gLogIOstream <<
      "Adding lyricist \"" <<
      value <<
      "\" to lpsrHeader" <<
      ", line " << fInputLineNumber <<
      endl;
  }
#endif

  if (! fLyricists) {
    fLyricists =
      lpsrVarValsListAssoc::create (
        inputLineNumber,
        lpsrVarValsListAssoc::kMusicXMLLyricist);
  }
  
  fLyricists->
    addAssocVariableValue (value);
}

void lpsrHeader::addPoet (
  int    inputLineNumber,
  string value)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceVarValAssocs) {
    gLogIOstream <<
      "Adding poet \"" <<
      value <<
      "\" to lpsrHeader" <<
      ", line " << fInputLineNumber <<
      endl;
  }
#endif

  if (! fPoets) {
    fPoets =
      lpsrVarValsListAssoc::create (
        inputLineNumber,
        lpsrVarValsListAssoc::kMusicXMLPoet);
  }
  
  fPoets->
    addAssocVariableValue (value);
}

void lpsrHeader::addTranslator (
  int    inputLineNumber,
  string value)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceVarValAssocs) {
    gLogIOstream <<
      "Adding translator \"" <<
      value <<
      "\" to lpsrHeader" <<
      ", line " << fInputLineNumber <<
      endl;
  }
#endif

  if (! fTranslators) {
    fTranslators =
      lpsrVarValsListAssoc::create (
        inputLineNumber,
        lpsrVarValsListAssoc::kMusicXMLTranslator);
  }
  
  fTranslators->
    addAssocVariableValue (value);
}

void lpsrHeader::addSoftware (
  int    inputLineNumber,
  string value)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceVarValAssocs) {
    gLogIOstream <<
      "Adding software \"" <<
      value <<
      "\" to lpsrHeader" <<
      ", line " << fInputLineNumber <<
      endl;
  }
#endif

  if (! fSoftwares) {
    fSoftwares =
      lpsrVarValsListAssoc::create (
        inputLineNumber,
        lpsrVarValsListAssoc::kMusicXMLSoftware);
  }
  
  fSoftwares->
    addAssocVariableValue (value);
}

int lpsrHeader::maxLilypondVariablesNamesLength ()
{
  int result = 0;

  // MusicXML informations
  
  if (fWorkNumber) {
    int length =
      fWorkNumber->
        lilyPondVarValAssocKindAsString ().size ();
    if (length > result) {
      result = length;
    }
  }
  
  if (fWorkTitle) {
    int length =
      fWorkTitle->
        lilyPondVarValAssocKindAsString ().size ();
    if (length > result) {
      result = length;
    }
  }
    
  if (fMovementNumber) {
    int length =
      fMovementNumber->
        lilyPondVarValAssocKindAsString ().size ();
    if (length > result) {
      result = length;
    }
  }
    
  if (fMovementTitle) {
    int length =
      fMovementTitle->
        lilyPondVarValAssocKindAsString ().size ();
    if (length > result) {
      result = length;
    }
  }
    
  if (fRights) {
    const list<string>&
      variableValuesList =
        fRights->getVariableValuesList ();
        
    list<string>::const_iterator i;
    for (i=variableValuesList.begin (); i!=variableValuesList.end (); i++) {
      int length = (*i).size ();
      if (length > result) {
        result = length;
      }
    } // for
  }
    
  if (fComposers) {
    const list<string>&
      variableValuesList =
        fComposers->getVariableValuesList ();
        
    list<string>::const_iterator i;
    for (i=variableValuesList.begin (); i!=variableValuesList.end (); i++) {
      int length = (*i).size ();
      if (length > result){
        result = length;
      }
    } // for
  }
    
  if (fArrangers) {
    const list<string>&
      variableValuesList =
        fArrangers->getVariableValuesList ();
        
    list<string>::const_iterator i;
    for (i=variableValuesList.begin (); i!=variableValuesList.end (); i++) {
      int length = (*i).size ();
      if (length > result) {
        result = length;
      }
    } // for
  }
    
  if (fLyricists) {
    const list<string>&
      variableValuesList =
        fLyricists->getVariableValuesList ();
        
    list<string>::const_iterator i;
    for (i=variableValuesList.begin (); i!=variableValuesList.end (); i++) {
      int length = (*i).size ();
      if (length > result) {
        result = length;
      }
    } // for
  }
    
  if (fPoets) {
    const list<string>&
      variableValuesList =
        fPoets->getVariableValuesList ();
        
    list<string>::const_iterator i;
    for (i=variableValuesList.begin (); i!=variableValuesList.end (); i++) {
      int length = (*i).size ();
      if (length > result) {
        result = length;
      }
    } // for
  }
    
  if (fTranslators) {
    const list<string>&
      variableValuesList =
        fTranslators->getVariableValuesList ();
        
    list<string>::const_iterator i;
    for (i=variableValuesList.begin (); i!=variableValuesList.end (); i++) {
      int length = (*i).size ();
      if (length > result) {
        result = length;
      }
    } // for
  }
    
  if (fSoftwares) {
    const list<string>&
      variableValuesList =
        fSoftwares->getVariableValuesList ();
        
    list<string>::const_iterator i;
    for (i=variableValuesList.begin (); i!=variableValuesList.end (); i++) {
      int length = (*i).size ();
      if (length > result) {
        result = length;
      }
    } // for
  }
    
  if (fScoreInstrument) {
    int length = fScoreInstrument->lilyPondVarValAssocKindAsString ().size ();
    if (length > result) {
      result = length;
    }
  }
  
  if (fEncodingDate) {
    int length = fEncodingDate->lilyPondVarValAssocKindAsString ().size ();
    if (length > result) {
      result = length;
    }
  }
  
  if (fMiscellaneousField) {
    int length = fMiscellaneousField->lilyPondVarValAssocKindAsString ().size ();
    if (length > result) {
      result = length;
    }
  }
  
  // Lilypond informations

  if (fLilypondDedication) {
    int length =
      fLilypondDedication->
        lilyPondVarValAssocKindAsString ().size ();
    if (length > result) {
      result = length;
    }
  }

  if (fLilypondPiece) {
    int length =
      fLilypondPiece->
        lilyPondVarValAssocKindAsString ().size ();
    if (length > result) {
      result = length;
    }
  }

  if (fLilypondOpus) {
    int length =
      fLilypondOpus->
        lilyPondVarValAssocKindAsString ().size ();
    if (length > result) {
      result = length;
    }
  }

  if (fLilypondTitle) {
    int length =
      fLilypondTitle->
        lilyPondVarValAssocKindAsString ().size ();
    if (length > result) {
      result = length;
    }
  }

  if (fLilypondSubTitle) {
    int length =
      fLilypondSubTitle->
        lilyPondVarValAssocKindAsString ().size ();
    if (length > result) {
      result = length;
    }
  }

  if (fLilypondSubSubTitle) {
    int length =
      fLilypondSubSubTitle->
        lilyPondVarValAssocKindAsString ().size ();
    if (length > result) {
      result = length;
    }
  }

  if (fLilypondInstrument) {
    int length =
      fLilypondInstrument->
        lilyPondVarValAssocKindAsString ().size ();
    if (length > result) {
      result = length;
    }
  }

  if (fLilypondMeter) {
    int length =
      fLilypondMeter->
        lilyPondVarValAssocKindAsString ().size ();
    if (length > result) {
      result = length;
    }
  }

  if (fLilypondCopyright) {
    int length =
      fLilypondCopyright->
        lilyPondVarValAssocKindAsString ().size ();
    if (length > result) {
      result = length;
    }
  }

  if (fLilypondTagline) {
    int length =
      fLilypondTagline->
        lilyPondVarValAssocKindAsString ().size ();
    if (length > result) {
      result = length;
    }
  }

  return result;
}

void lpsrHeader::acceptIn (basevisitor* v)
{
  if (gLpsrOptions->fTraceLpsrVisitors) {
    gLogIOstream <<
      "% ==> lpsrHeader::acceptIn ()" <<
      endl;
  }
      
  if (visitor<S_lpsrHeader>*
    p =
      dynamic_cast<visitor<S_lpsrHeader>*> (v)) {
        S_lpsrHeader elem = this;
        
        if (gLpsrOptions->fTraceLpsrVisitors) {
          gLogIOstream <<
            "% ==> Launching lpsrHeader::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void lpsrHeader::acceptOut (basevisitor* v)
{
  if (gLpsrOptions->fTraceLpsrVisitors) {
    gLogIOstream <<
      "% ==> lpsrHeader::acceptOut ()" <<
      endl;
  }

  if (visitor<S_lpsrHeader>*
    p =
      dynamic_cast<visitor<S_lpsrHeader>*> (v)) {
        S_lpsrHeader elem = this;
      
        if (gLpsrOptions->fTraceLpsrVisitors) {
          gLogIOstream <<
            "% ==> Launching lpsrHeader::visitEnd ()" <<
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
    
  if (fTranslators) {
    // browse fTranslators
    msrBrowser<lpsrVarValsListAssoc> browser (v);
    browser.browse (*fTranslators);
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

void lpsrHeader::print (ostream& os)
{
  os <<
    "Header" <<
    endl;

  bool emptyHeader = true;
  
  gIndenter++;

  const int fieldWidth = 16;
  
  // LilyPond informations

  // MusicXML informations

  if (fLilypondTitle) {
    os << left <<
      setw (fieldWidth) <<
      fLilypondTitle->lilyPondVarValAssocKindAsString () << " : " <<
      endl;
      
    gIndenter++;
    
    os <<
      fLilypondTitle <<
      endl;
      
    gIndenter--;

    emptyHeader = false;
  }
    
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
    
  if (fTranslators) {
    os << left <<
      setw (fieldWidth) <<
      fTranslators->lilyPondVarValsListAssocKindAsString () << " : " <<
      endl;
      
    gIndenter++;
    
    os <<
      fTranslators <<
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
      endl <<
      endl; // JMI
  }
  
  gIndenter--;
}

ostream& operator<< (ostream& os, const S_lpsrHeader& elt)
{
  elt->print (os);
  return os;
}


}
