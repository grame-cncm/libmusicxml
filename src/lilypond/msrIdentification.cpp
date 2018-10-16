/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifdef VC6
# pragma warning (disable : 4786)
#endif

#include <iostream>
#include <sstream>

#include "msrIdentification.h"

#include "utilities.h"

#include "setTraceOptionsIfDesired.h"
#ifdef TRACE_OPTIONS
  #include "traceOptions.h"
#endif

#include "msrOptions.h"


using namespace std;

namespace MusicXML2 {

//______________________________________________________________________________
S_msrIdentification msrIdentification::create (
  int inputLineNumber)
{
  msrIdentification* o =
    new msrIdentification (
      inputLineNumber);
  assert(o!=0);
  return o;
}

msrIdentification::msrIdentification (
  int inputLineNumber)
    : msrElement (inputLineNumber)
{}

msrIdentification::~msrIdentification ()
{}

void msrIdentification::setWorkNumber (
  int    inputLineNumber,
  string val)
  {
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceVarValAssocs) {
    gLogIOstream <<
      "Setting workNumber to \"" << val << "\"" <<
      endl;
  }
#endif

  fWorkNumber =
    msrVarValAssoc::create (
      inputLineNumber,
      msrVarValAssoc::kWorkNumber, val);
  }

void msrIdentification::setWorkTitle (
  int    inputLineNumber,
  string val)
  {
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceVarValAssocs) {
    gLogIOstream <<
      "Setting workTitle to \"" << val << "\"" <<
      endl;
  }
#endif

  fWorkTitle =
    msrVarValAssoc::create (
      inputLineNumber,
      msrVarValAssoc::kWorkTitle, val);
  }

void msrIdentification::setMovementNumber (
  int    inputLineNumber,
  string val)
  {
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceVarValAssocs) {
    gLogIOstream <<
      "Setting movementNumber to \"" << val << "\"" <<
      endl;
  }
#endif

  fMovementNumber =
    msrVarValAssoc::create (
      inputLineNumber,
      msrVarValAssoc::kMovementNumber, val);
  }

void msrIdentification::setMovementTitle (
  int    inputLineNumber,
  string val)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceVarValAssocs) {
    gLogIOstream <<
      "Setting movementTitle to \"" << val << "\"" <<
      endl;
  }
#endif

  fMovementTitle =
    msrVarValAssoc::create (
      inputLineNumber,
      msrVarValAssoc::kMovementTitle, val);
}

void msrIdentification::setEncodingDate (
  int    inputLineNumber,
  string val)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceVarValAssocs) {
    gLogIOstream <<
      "Setting encodingDate to \"" << val << "\"" <<
      endl;
  }
#endif

  fEncodingDate =
    msrVarValAssoc::create (
      inputLineNumber,
      msrVarValAssoc::kEncodingDate, val);
}

void msrIdentification::setScoreInstrument (
  int    inputLineNumber,
  string val)
{
 #ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceVarValAssocs) {
    gLogIOstream <<
      "Setting scoreInstrument to \"" << val << "\"" <<
      endl;
  }
#endif

 fScoreInstrumentAssoc =
    msrVarValAssoc::create (
      inputLineNumber,
      msrVarValAssoc::kScoreInstrument, val);
}

void msrIdentification::setMiscellaneousField (
  int    inputLineNumber,
  string val)
{
 #ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceVarValAssocs) {
    gLogIOstream <<
      "Setting workTitle to \"" << val << "\"" <<
      endl;
  }
#endif

 fEncodingDate =
    msrVarValAssoc::create (
      inputLineNumber,
      msrVarValAssoc::kMiscellaneousField, val);
}

void msrIdentification::addRights (
  int    inputLineNumber,
  string value)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceVarValAssocs) {
    gLogIOstream <<
      "Adding right \"" << value << "\"" <<
      endl;
  }
#endif

  if (! fRights) {
    fRights =
      msrVarValsListAssoc::create (
        inputLineNumber,
        msrVarValsListAssoc::kRights);
  }
  
  fRights->
    addAssocVariableValue (value);
}

void msrIdentification::addComposer (
  int    inputLineNumber,
  string value)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceVarValAssocs) {
    gLogIOstream <<
      "Adding composer \"" << value << "\"" <<
      endl;
  }
#endif

  if (! fComposers) {
    fComposers =
      msrVarValsListAssoc::create (
        inputLineNumber,
        msrVarValsListAssoc::kComposer);
  }
  
  fComposers->
    addAssocVariableValue (value);
}

void msrIdentification::addArranger (
  int    inputLineNumber,
  string value)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceVarValAssocs) {
    gLogIOstream <<
      "Adding arranger \"" << value << "\"" <<
      endl;
  }
#endif

  if (! fArrangers) {
    fArrangers =
      msrVarValsListAssoc::create (
        inputLineNumber,
        msrVarValsListAssoc::kArranger);
  }
  
  fArrangers->
    addAssocVariableValue (value);
}

void msrIdentification::addLyricist (
  int    inputLineNumber,
  string value)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceVarValAssocs) {
    gLogIOstream <<
      "Adding lyricist \"" << value << "\"" <<
      endl;
  }
#endif

  if (! fLyricists) {
    fLyricists =
      msrVarValsListAssoc::create (
        inputLineNumber,
        msrVarValsListAssoc::kLyricist);
  }
  
  fLyricists->
    addAssocVariableValue (value);
}

void msrIdentification::addPoet (
  int    inputLineNumber,
  string value)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceVarValAssocs) {
    gLogIOstream <<
      "Adding poet \"" << value << "\"" <<
      endl;
  }
#endif

  if (! fPoets) {
    fPoets =
      msrVarValsListAssoc::create (
        inputLineNumber,
        msrVarValsListAssoc::kPoet);
  }
  
  fPoets->
    addAssocVariableValue (value);
}

void msrIdentification::addTranslator (
  int    inputLineNumber,
  string value)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceVarValAssocs) {
    gLogIOstream <<
      "Adding translator \"" << value << "\"" <<
      endl;
  }
#endif

  if (! fTranslators) {
    fTranslators =
      msrVarValsListAssoc::create (
        inputLineNumber,
        msrVarValsListAssoc::kTranslator);
  }
  
  fTranslators->
    addAssocVariableValue (value);
}

void msrIdentification::addSoftware (
  int    inputLineNumber,
  string value)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceVarValAssocs) {
    gLogIOstream <<
      "Adding software \"" << value << "\"" <<
      endl;
  }
#endif

  if (! fSoftwares) {
    fSoftwares =
      msrVarValsListAssoc::create (
        inputLineNumber,
        msrVarValsListAssoc::kSoftware);
  }
  
  fSoftwares->
    addAssocVariableValue (value);
}

void msrIdentification::acceptIn (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrIdentification::acceptIn ()" <<
      endl;
  }
      
  if (visitor<S_msrIdentification>*
    p =
      dynamic_cast<visitor<S_msrIdentification>*> (v)) {
        S_msrIdentification elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrIdentification::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrIdentification::acceptOut (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrIdentification::acceptOut ()" <<
      endl;
  }

  if (visitor<S_msrIdentification>*
    p =
      dynamic_cast<visitor<S_msrIdentification>*> (v)) {
        S_msrIdentification elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrIdentification::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrIdentification::browseData (basevisitor* v)
{
  if (fWorkNumber) {
    // browse fWorkNumber
    msrBrowser<msrVarValAssoc> browser (v);
    browser.browse (*fWorkNumber);
  }

  if (fWorkTitle) {
    // browse fWorkTitle
    msrBrowser<msrVarValAssoc> browser (v);
    browser.browse (*fWorkTitle);
  }

  if (fMovementNumber) {
    // browse fMovementNumber
    msrBrowser<msrVarValAssoc> browser (v);
    browser.browse (*fMovementNumber);
  }

  if (fMovementTitle) {
    // browse fMovementTitle
    msrBrowser<msrVarValAssoc> browser (v);
    browser.browse (*fMovementTitle);
  }

  if (fComposers) {
    // browse fComposers
    msrBrowser<msrVarValsListAssoc> browser (v);
    browser.browse (*fComposers);
  }
    
  if (fArrangers) {
    // browse fArrangers
    msrBrowser<msrVarValsListAssoc> browser (v);
    browser.browse (*fArrangers);
  }
    
  if (fLyricists) {
    // browse fLyricists
    msrBrowser<msrVarValsListAssoc> browser (v);
    browser.browse (*fLyricists);
  }
    
  if (fPoets) {
    // browse fPoets
    msrBrowser<msrVarValsListAssoc> browser (v);
    browser.browse (*fPoets);
  }
    
  if (fTranslators) {
    // browse fTranslators
    msrBrowser<msrVarValsListAssoc> browser (v);
    browser.browse (*fTranslators);
  }
    
  if (fRights) {
    // browse fRights
    msrBrowser<msrVarValsListAssoc> browser (v);
    browser.browse (*fRights);
  }

  if (fSoftwares) {
    // browse fSoftwares
    msrBrowser<msrVarValsListAssoc> browser (v);
    browser.browse (*fSoftwares);
  }

  if (fEncodingDate) {
    // browse encoding date
    msrBrowser<msrVarValAssoc> browser (v);
    browser.browse (*fEncodingDate);
  }

  if (fScoreInstrumentAssoc) {
    // browse score instrument
    msrBrowser<msrVarValAssoc> browser (v);
    browser.browse (*fScoreInstrumentAssoc);
  }
}

ostream& operator<< (ostream& os, const S_msrIdentification& elt)
{
  elt->print (os);
  return os;
}

void msrIdentification::print (ostream& os)
{
  os <<
    "Identification" <<
    endl;

  bool emptyIdentification = true;

  gIndenter++;
  
  if (fWorkNumber) {
    os <<
      fWorkNumber <<
      endl;

    emptyIdentification = false;
  }
  
  if (fWorkTitle) {
    os <<
      fWorkTitle <<
      endl;

    emptyIdentification = false;
  }
    
  if (fMovementNumber) {
    os <<
      fMovementNumber <<
      endl;

    emptyIdentification = false;
  }
    
  if (fMovementTitle) {
    os <<
      fMovementTitle <<
      endl;

    emptyIdentification = false;
  }

  if (fRights) {
    os <<
      fRights <<
      endl;
      
    emptyIdentification = false;
  }
    
  if (fComposers) {
    os <<
      fComposers <<
      endl;
      
    emptyIdentification = false;
  }
    
  if (fArrangers) {
    os <<
      fArrangers <<
      endl;
      
    emptyIdentification = false;
  }
    
  if (fLyricists) {
    os <<
      fLyricists <<
      endl;
      
    emptyIdentification = false;
  }
    
  if (fPoets) {
    os <<
      fPoets <<
      endl;
      
    emptyIdentification = false;
  }
    
  if (fTranslators) {
    os <<
      fTranslators <<
      endl;
      
    emptyIdentification = false;
  }
    
  if (fSoftwares) {
    os <<
      fSoftwares <<
      endl;
    emptyIdentification = false;
  }
    
  if (fEncodingDate) {
    os <<
      fEncodingDate <<
      endl;
      
    emptyIdentification = false;
  }

  if (emptyIdentification) {
    os <<
      " " << "nothing specified" <<
      endl <<
      endl;
  }
  
  gIndenter--;
}


}
