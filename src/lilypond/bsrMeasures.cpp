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

#include "generalOptions.h"
#include "bsrOptions.h"
#include "brailleOptions.h"


using namespace std;

namespace MusicXML2 
{

//______________________________________________________________________________
S_bsrMeasure bsrMeasure::create (
  int    inputLineNumber,
  string printMeasureNumber)
{
  bsrMeasure* o =
    new bsrMeasure (
      inputLineNumber, printMeasureNumber);
  assert(o!=0);
  return o;
}

bsrMeasure::bsrMeasure (
  int    inputLineNumber,
  string printMeasureNumber)
    : bsrLineElement (inputLineNumber)
{
  fPrintMeasureNumber = printMeasureNumber;

  // initially, fBrailleMeasureNumber is the same as fPrintMeasureNumber
  fBrailleMeasureNumber = fPrintMeasureNumber;

  fMeasureCellsList =
    bsrCellsList::create (fInputLineNumber);

#ifdef TRACE_OPTIONS
  if (gGeneralOptions->fTraceMeasures) {
    gLogIOstream <<
      "Creating bsrMeasure '" <<
      asString () <<
      "', line " <<
      fInputLineNumber <<
      endl;
  }
#endif
}

bsrMeasure::~bsrMeasure ()
{}

S_bsrMeasure bsrMeasure::createMeasureNewbornClone ()
{
#ifdef TRACE_OPTIONS
  if (gGeneralOptions->fTraceMeasures) {
    gLogIOstream <<
      "Creating a newborn clone of measure " <<
      asString () <<
      endl;
  }
#endif

  S_bsrMeasure
    newbornClone =
      bsrMeasure::create (
        fInputLineNumber,
        fPrintMeasureNumber);

  // braille measure number
  newbornClone->fBrailleMeasureNumber =
    fBrailleMeasureNumber;
    
  return newbornClone;
}

void  bsrMeasure::appendElementToMeasure (S_bsrElement elem)
{
  fMeasureElementsList.push_back (elem);
}

void bsrMeasure::appendBarlineToMeasure (S_bsrBarline barline)
{
#ifdef TRACE_OPTIONS
  if (gGeneralOptions->fTraceBarlines || gGeneralOptions->fTraceMeasures) {
    gLogIOstream <<
      "Appending barline '" <<
      barline->asShortString () <<
      "' to measure '" <<
      asString () <<
      "'" <<
      endl;
    }
#endif

  appendElementToMeasure (barline);
}

void bsrMeasure::appendNumberToMeasure (S_bsrNumber number) // JMI ???
{
#ifdef TRACE_OPTIONS
  if (gBsrOptions->fTraceNumbers || gGeneralOptions->fTraceMeasures) {
    gLogIOstream <<
      "Appending number '" <<
      number->asShortString () <<
      "' to measure '" <<
      asString () <<
      "'" <<
      endl;
    }
#endif

  appendElementToMeasure (number);
}

void bsrMeasure::appendClefToMeasure (S_bsrClef clef)
{
#ifdef TRACE_OPTIONS
  if (gGeneralOptions->fTraceClefs || gGeneralOptions->fTraceMeasures) {
    gLogIOstream <<
      "Appending clef '" <<
      clef->asShortString () <<
      "' to measure '" <<
      asString () <<
      "'" <<
      endl;
    }
#endif

  appendElementToMeasure (clef);
}

void bsrMeasure::appendKeyToMeasure (S_bsrKey key)
{
#ifdef TRACE_OPTIONS
  if (gGeneralOptions->fTraceKeys || gGeneralOptions->fTraceMeasures) {
    gLogIOstream <<
      "Appending key '" <<
      key->asShortString () <<
      "' to measure '" <<
      asString () <<
      "'" <<
      endl;
    }
#endif

  appendElementToMeasure (key);
}

void bsrMeasure::appendTimeToMeasure (S_bsrTime time)
{
#ifdef TRACE_OPTIONS
  if (gGeneralOptions->fTraceTimes || gGeneralOptions->fTraceMeasures) {
    gLogIOstream <<
      "Appending time '" <<
      time->asShortString () <<
      "' to measure '" <<
      asString () <<
      "'" <<
      endl;
    }
#endif

  appendElementToMeasure (time);
}

void bsrMeasure::appendTempoToMeasure (S_bsrTempo tempo)
{
#ifdef TRACE_OPTIONS
  if (gGeneralOptions->fTraceTempos || gGeneralOptions->fTraceMeasures) {
    gLogIOstream <<
      "Appending tempo '" <<
      tempo->asShortString () <<
      "' to measure '" <<
      asString () <<
      "'" <<
      endl;
    }
#endif

  appendElementToMeasure (tempo);
}

void bsrMeasure::appendNoteToMeasure (S_bsrNote note)
{
#ifdef TRACE_OPTIONS
  if (gGeneralOptions->fTraceNotes || gGeneralOptions->fTraceMeasures) {
    gLogIOstream <<
      "Appending note '" <<
      note->asShortString () <<
      "' to measure '" <<
      asString () <<
      "'" <<
      endl;
    }
#endif

  appendElementToMeasure (note);
}

void bsrMeasure::appendDynamicsToMeasure (S_bsrDynamics dynamics)
{
#ifdef TRACE_OPTIONS
  if (gGeneralOptions->fTraceDynamics || gGeneralOptions->fTraceMeasures) {
    gLogIOstream <<
      "Appending dynamics '" <<
      dynamics->asShortString () <<
      "' to measure '" <<
      asString () <<
      "'" <<
      endl;
    }
#endif

  appendElementToMeasure (dynamics);
}

S_bsrCellsList bsrMeasure::asCellsList () const
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

int bsrMeasure::fetchCellsNumber () const
{
  return asCellsList ()->fetchCellsNumber ();
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

string bsrMeasure::asString () const
{
  stringstream s;

  s <<
    "Spaces" <<
    ", spacesBefore: " << fSpacesBefore <<
    ", spacesAfter: " << fSpacesAfter <<
    ", printMeasureNumber: " << fPrintMeasureNumber <<
    ", printMeasureNumber: " << fPrintMeasureNumber <<
    ", brailleMeasureNumber: " << fBrailleMeasureNumber <<
    ", measureElementsList.size (): " << fMeasureElementsList.size () <<
    ", line " << fInputLineNumber;

  return s.str ();
}

void bsrMeasure::print (ostream& os)
{
  os <<
    "Measure" <<
    ", printMeasureNumber " << fPrintMeasureNumber <<
    ", line " << fInputLineNumber <<
    endl;
    
  gIndenter++;

  const int fieldWidth = 21;

  // print spaces needs
  os << left <<
    setw (fieldWidth) <<
    "spacesBefore" << " : " <<
    fSpacesBefore <<
    endl <<
    setw (fieldWidth) <<
    "spacesAfter" << " : " <<
    fSpacesAfter <<
    endl;
  
  // print the measure numbers
  os << left <<
    setw (fieldWidth) <<
    "printMeasureNumber" << " : " << fPrintMeasureNumber <<
    endl <<
    setw (fieldWidth) <<
    "brailleMeasureNumber" << " : " << fBrailleMeasureNumber <<
    endl;

  // print the number of cells
  os << left <<
    setw (fieldWidth) <<
    "cellsNumber" << " : " << fetchCellsNumber () <<
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
