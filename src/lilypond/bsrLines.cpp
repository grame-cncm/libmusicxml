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

#include "bsrMutuallyDependent.h"

#include "generalOptions.h"
#include "bsrOptions.h"
#include "brailleOptions.h"


using namespace std;

namespace MusicXML2 
{

//______________________________________________________________________________
S_bsrLine bsrLine::create (
  int inputLineNumber,
  int printLineNumber,
  int cellsPerLine)
{
  bsrLine* o =
    new bsrLine (
      inputLineNumber, printLineNumber, cellsPerLine);
  assert(o!=0);
  return o;
}

bsrLine::bsrLine (
  int inputLineNumber,
  int printLineNumber,
  int cellsPerLine)
    : bsrElement (inputLineNumber)
{
  fPrintLineNumber   = printLineNumber;
  fBrailleLineNumber = fPrintLineNumber; // will be set by BSR finalizer
  
  fCellsPerLine = cellsPerLine;

  // prevent spaces to be inserted at the beginning of the line
  fLastAppendedLineElementIsASpaces = true;

#ifdef TRACE_OPTIONS
  if (gBsrOptions->fTraceLines) {
    gLogIOstream <<
      "Creating bsrLine '" <<
      asString () <<
      "', line " <<
      fInputLineNumber <<
      endl;
  }
#endif
}

bsrLine::~bsrLine ()
{}

S_bsrLine bsrLine::createLineNewbornClone ()
{
#ifdef TRACE_OPTIONS
  if (gBsrOptions->fTraceLines) {
    gLogIOstream <<
      "Creating a newborn clone of line " <<
      asString () <<
      endl;
  }
#endif

  S_bsrLine
    newbornClone =
      bsrLine::create (
        fInputLineNumber,
        fPrintLineNumber,
        fCellsPerLine);

  // braille line number
  newbornClone->fBrailleLineNumber =
    fBrailleLineNumber;
    
  return newbornClone;
}

void  bsrLine::appendLineElementToMeasure (S_bsrLineElement lineElement)
{
  fLineElementsList.push_back (lineElement);
}

void bsrLine::appendSpacesToLine (S_bsrSpaces spaces)
{
#ifdef TRACE_OPTIONS
  if (gBsrOptions->fTraceSpaces || gBsrOptions->fTraceLines) {
    gLogIOstream <<
      "Appending spaces '" <<
      spaces->asShortString () <<
      "' to line '" <<
      asString () <<
      "'" <<
      endl;
    }
#endif

  appendLineElementToMeasure (spaces);
  fLastAppendedLineElementIsASpaces = true;
}

void bsrLine::appendMeasureToLine (S_bsrMeasure measure)
{
#ifdef TRACE_OPTIONS
  if (gGeneralOptions->fTraceMeasures || gBsrOptions->fTraceLines) {
    gLogIOstream <<
      "Appending measure '" <<
      measure->asShortString () <<
      "' to line '" <<
      asString () <<
      "'" <<
      endl;
    }
#endif

  if (! fLastAppendedLineElementIsASpaces) {
    // create a 1-spaces
    S_bsrSpaces
      spaces =
        bsrSpaces::create (
          fInputLineNumber, 1);
  
    // append it to this line
    this->
      appendSpacesToLine (spaces);
  }
  
  appendLineElementToMeasure (measure);
  fLastAppendedLineElementIsASpaces = false;
}

int bsrLine::fetchCellsNumber () const
{
  int result = 0;

  for (
    list<S_bsrLineElement>::const_iterator i = fLineElementsList.begin ();
    i != fLineElementsList.end ();
    i++ ) {
    result += (*i)->fetchCellsNumber ();
  } // for

  return result;
}

void bsrLine::acceptIn (basevisitor* v)
{
  if (gBsrOptions->fTraceBsrVisitors) {
    gLogIOstream <<
      "% ==> bsrLine::acceptIn ()" <<
      endl;
  }
      
  if (visitor<S_bsrLine>*
    p =
      dynamic_cast<visitor<S_bsrLine>*> (v)) {
        S_bsrLine elem = this;
        
        if (gBsrOptions->fTraceBsrVisitors) {
          gLogIOstream <<
            "% ==> Launching bsrLine::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void bsrLine::acceptOut (basevisitor* v)
{
  if (gBsrOptions->fTraceBsrVisitors) {
    gLogIOstream <<
      "% ==> bsrLine::acceptOut ()" <<
      endl;
  }

  if (visitor<S_bsrLine>*
    p =
      dynamic_cast<visitor<S_bsrLine>*> (v)) {
        S_bsrLine elem = this;
      
        if (gBsrOptions->fTraceBsrVisitors) {
          gLogIOstream <<
            "% ==> Launching bsrLine::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void bsrLine::browseData (basevisitor* v)
{
  for (
    list<S_bsrLineElement>::const_iterator i = fLineElementsList.begin ();
    i != fLineElementsList.end ();
    i++ ) {
    // browse the element
    bsrBrowser<bsrLineElement> browser (v);
    browser.browse (*(*i));
  } // for
}

string bsrLine::asString () const
{
  stringstream s;

  s <<
    "Line" <<
    ", printLineNumber" << " : " << fPrintLineNumber <<
    ", brailleLineNumber" << " : " << fBrailleLineNumber <<
    ", cellsPerLine" << " : " << fCellsPerLine <<
    ", elements: " << fLineElementsList.size () <<
    ", cellsNumber" << " : " << fetchCellsNumber () <<
    ", line " << fInputLineNumber;
    
  return s.str ();
}

void bsrLine::print (ostream& os)
{
  os <<
    "Line" <<
    endl;
  
  gIndenter++;

  // print the line numbers
  const int fieldWidth = 18;
  
  os << left <<
    setw (fieldWidth) <<
    "printLineNumber" << " : " << fPrintLineNumber <<
    endl <<
    setw (fieldWidth) <<
    "brailleLineNumber" << " : " << fBrailleLineNumber <<
    endl <<
    setw (fieldWidth) <<
    "cellsPerLine" << " : " << fCellsPerLine <<
    endl <<
    setw (fieldWidth) <<
    "cellsNumber" << " : " << fetchCellsNumber () <<
    endl;
  os <<
    endl;
  
  // print the line elements if any
  int lineElementsListSize = fLineElementsList.size ();
  
  if (lineElementsListSize || gBsrOptions->fDisplayBsrDetails) {
    os <<
      setw (fieldWidth) <<
      "LineElementsList";
    if (lineElementsListSize) {
      os <<
        endl;
      gIndenter++;
  
      list<S_bsrLineElement>::const_iterator
        iBegin = fLineElementsList.begin (),
        iEnd   = fLineElementsList.end (),
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

ostream& operator<< (ostream& os, const S_bsrLine& elt)
{
  elt->print (os);
  return os;
}


}
