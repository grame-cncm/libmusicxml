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

#include "bsrMeasures.h"

#include "setTraceOptionsIfDesired.h"
#ifdef TRACE_OPTIONS
  #include "traceOptions.h"
  #include "bsrTraceOptions.h"
#endif

#include "bsrOptions.h"
#include "brailleOptions.h"


using namespace std;

namespace MusicXML2 
{

//______________________________________________________________________________
S_bsrMeasure bsrMeasure::create (
  int inputLineNumber)
{
  bsrMeasure* o =
    new bsrMeasure (
      inputLineNumber);
  assert(o!=0);
  return o;
}

bsrMeasure::bsrMeasure (
  int inputLineNumber)
    : bsrLineElement (inputLineNumber)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceMeasures) {
    gLogIOstream <<
      "Creating bsrMeasure '" <<
      asString () <<
      "', line " <<
      fInputLineNumber <<
      endl;
  }
#endif

  fMeasureCellsList =
    bsrCellsList::create (fInputLineNumber);
}

bsrMeasure::~bsrMeasure ()
{}

S_bsrMeasure bsrMeasure::createMeasureNewbornClone ()
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceMeasures) {
    gLogIOstream <<
      "Creating a newborn clone of measure " <<
      asString () <<
      endl;
  }
#endif

  S_bsrMeasure
    newbornClone =
      bsrMeasure::create (
        fInputLineNumber);

  // measure number
  newbornClone->fPrintMeasureNumber =
    fPrintMeasureNumber;
  newbornClone->fBrailleMeasureNumber =
    fBrailleMeasureNumber;
    
  return newbornClone;
}

S_bsrCellsList bsrMeasure::asCellsList ()
{
  S_bsrCellsList
    result =
      bsrCellsList::create (fInputLineNumber);

  for (
    list<S_bsrElement>::const_iterator i = fMeasureElementsList.begin ();
    i != fMeasureElementsList.end ();
    i++ ) {
    // append the braille for the element
    /* JMI
    fMeasureCellsList->
      appendCellsListToCellsList (
        (*i)->asCellsList ());
        */
  } // for

  return result;
}

void bsrMeasure::acceptIn (basevisitor* v)
{
  if (gBsrOptions->fTraceBsrVisitors) {
    gLogIOstream <<
      "% ==> bsrMeasure::acceptIn ()" <<
      endl;
  }
      
  if (visitor<S_bsrMeasure>*
    p =
      dynamic_cast<visitor<S_bsrMeasure>*> (v)) {
        S_bsrMeasure elem = this;
        
        if (gBsrOptions->fTraceBsrVisitors) {
          gLogIOstream <<
            "% ==> Launching bsrMeasure::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void bsrMeasure::acceptOut (basevisitor* v)
{
  if (gBsrOptions->fTraceBsrVisitors) {
    gLogIOstream <<
      "% ==> bsrMeasure::acceptOut ()" <<
      endl;
  }

  if (visitor<S_bsrMeasure>*
    p =
      dynamic_cast<visitor<S_bsrMeasure>*> (v)) {
        S_bsrMeasure elem = this;
      
        if (gBsrOptions->fTraceBsrVisitors) {
          gLogIOstream <<
            "% ==> Launching bsrMeasure::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void bsrMeasure::browseData (basevisitor* v)
{
  for (
    list<S_bsrElement>::const_iterator i = fMeasureElementsList.begin ();
    i != fMeasureElementsList.end ();
    i++ ) {
    // browse the element
    bsrBrowser<bsrElement> browser (v);
    browser.browse (*(*i));
  } // for
}

void bsrMeasure::print (ostream& os)
{
  os <<
    "Measure" <<
    ", line " << fInputLineNumber <<
    endl;
    
  gIndenter++;

  const int fieldWidth = 21;

  // print spaces needs
  os << left <<
    setw (fieldWidth) <<
    "aSpaceIsNeededBefore" << " : " <<
    booleanAsString (fASpaceIsNeededBefore) <<
    endl <<
    setw (fieldWidth) <<
    "aSpaceIsNeededAfter" << " : " <<
    booleanAsString (fASpaceIsNeededAfter) <<
    endl;
  
  // print the measure numbers
  os << left <<
    setw (fieldWidth) <<
    "printMeasureNumber" << " : " << fPrintMeasureNumber <<
    endl <<
    setw (fieldWidth) <<
    "brailleMeasureNumber" << " : " << fBrailleMeasureNumber <<
    endl;

  os <<
    endl;
  
  // print the measure elements if any
  int measureElementsListSize = fMeasureElementsList.size ();
  
  if (measureElementsListSize || gBsrOptions->fDisplayBsrDetails) {
    os <<
      setw (fieldWidth) <<
      "MeasureElementsList";
    if (measureElementsListSize) {
      os <<
        endl;
      gIndenter++;
  
      list<S_bsrElement>::const_iterator
        iBegin = fMeasureElementsList.begin (),
        iEnd   = fMeasureElementsList.end (),
        i      = iBegin;
      for ( ; ; ) {
        os << (*i);
        if (++i == iEnd) break;
        os << endl;
      } // for
          
      gIndenter--;
    }
    else {
      os <<
        " : " << "none" <<
      endl;
    }
  }

  gIndenter--;
}

ostream& operator<< (ostream& os, const S_bsrMeasure& elt)
{
  elt->print (os);
  return os;
}


}
