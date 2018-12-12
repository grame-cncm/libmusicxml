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

//_______________________________________________________________________________

S_bsrLineContents bsrLineContents::create (
  int                 inputLineNumber,
  bsrLineContentsKind lineContentsKind)
{
  bsrLineContents* o =
    new bsrLineContents (
      inputLineNumber,
      lineContentsKind);
  assert(o!=0);
  return o;
}

bsrLineContents::bsrLineContents (
  int                 inputLineNumber,
  bsrLineContentsKind lineContentsKind)
    : bsrElement (inputLineNumber)
{
  fLineContentsKind = lineContentsKind;
}

bsrLineContents::~bsrLineContents ()
{}

S_bsrLineContents bsrLineContents::createLineNewbornClone ()
{
#ifdef TRACE_OPTIONS
  if (gBsrOptions->fTraceLines) {
    gLogIOstream <<
      "Creating a newborn clone of line " <<
      asString () <<
      endl;
  }
#endif

  S_bsrLineContents
    newbornClone =
      bsrLineContents::create (
        fInputLineNumber,
        fLineContentsKind);
    
  return newbornClone;
}

/*
void bsrLineContents::setBsrLineUplink (
S_bsrLine bsrLineUplink)
{
  fBsrLineUplink = bsrLineUplink;
}

S_bsrLine bsrLineContents::getBsrLineUplink () const
{
  return fBsrLineUplink;
}
*/

void bsrLineContents::appendLineElementToLineContents (
  S_bsrLineElement lineElement)
{
  int spacesBefore = lineElement->getSpacesBefore ();

  if (spacesBefore > 0) {
    // append spaces to line elements list
    S_bsrSpaces
      spaces =
        bsrSpaces::create (
          fInputLineNumber, spacesBefore);
      
    fLineElementsList.push_back (spaces);
  }

  fLineElementsList.push_back (lineElement);
}

int bsrLineContents::fetchCellsNumber () const
{
  int result = 0;

  for (
    list<S_bsrLineElement>::const_iterator i = fLineElementsList.begin ();
    i != fLineElementsList.end ();
    i++ ) {
      S_bsrLineElement lineElement = (*i);
      
    result += lineElement->getSpacesBefore ();
    result += lineElement->fetchCellsNumber ();
  } // for

  return result;
}

void bsrLineContents::acceptIn (basevisitor* v)
{
  if (gBsrOptions->fTraceBsrVisitors) {
    gLogIOstream <<
      "% ==> bsrLineContents::acceptIn ()" <<
      endl;
  }
  
  if (visitor<S_bsrLineContents>*
    p =
      dynamic_cast<visitor<S_bsrLineContents>*> (v)) {
        S_bsrLineContents elem = this;
        
        if (gBsrOptions->fTraceBsrVisitors) {
          gLogIOstream <<
            "% ==> Launching bsrLineContents::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void bsrLineContents::acceptOut (basevisitor* v)
{
  if (gBsrOptions->fTraceBsrVisitors) {
    gLogIOstream <<
      "% ==> bsrLineContents::acceptOut ()" <<
      endl;
  }

  if (visitor<S_bsrLineContents>*
    p =
      dynamic_cast<visitor<S_bsrLineContents>*> (v)) {
        S_bsrLineContents elem = this;
      
        if (gBsrOptions->fTraceBsrVisitors) {
          gLogIOstream <<
            "% ==> Launching bsrLineContents::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void bsrLineContents::browseData (basevisitor* v)
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

string bsrLineContents::lineContentsKindAsString (
  bsrLineContentsKind lineContentsKind)
{
  string result;

  switch (lineContentsKind) {
    case bsrLineContents::kLineContentsRegular:
      result = "lineContentsRegular";
      break;
    case bsrLineContents::kLineContentsContinuation:
      result = "lineContentsContinuation";
      break;
  } // switch

  return result;
}

string bsrLineContents::asString () const
{
  // this is overriden all in actual elements
  return "??? bsrLineContents::asString () ???";
}

string bsrLineContents::asShortString () const
{
  // this can be overriden in actual elements
  return asString ();
}

void bsrLineContents::print (ostream& os)
{
  int lineElementsListSize = fLineElementsList.size ();
  
  os <<
    "LineContents" <<
//    ", lineContentsKind: " <<
    lineContentsKindAsString (fLineContentsKind) <<
    ", " <<
    singularOrPlural (
      lineElementsListSize, "lineElement", "lineElements") <<
    endl;
  
  gIndenter++;

  // print the line numbers
  const int fieldWidth = 17;
  
  os << left <<
    setw (fieldWidth) <<
    "cellsNumber" << " : " << fetchCellsNumber () <<
    endl;
  os <<
    endl;
  
  // print the line elements if any
  if (lineElementsListSize || gBsrOptions->fDisplayBsrDetails) {
    os <<
//      setw (fieldWidth) <<
      "LineElementsList" <<
      ", " <<
      singularOrPlural (
        lineElementsListSize, "lineElement", "lineElements");
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

ostream& operator<< (ostream& os, const S_bsrLineContents& elt)
{
  elt->print (os);
  return os;
}

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
    : bsrPageElement (inputLineNumber)
{
  fPrintLineNumber   = printLineNumber;
  fBrailleLineNumber = fPrintLineNumber; // will be set by BSR finalizer
  
  fCellsPerLine = cellsPerLine;

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

void bsrLine::appendLineElementToMeasure (S_bsrLineElement lineElement)
{
  S_bsrLineContents
    lineContentsToAppendTo;
    
  if (! fLineContentsList.size ()) {
    // first insertion in this line: create the first, regular line contents
    lineContentsToAppendTo =
      bsrLineContents::create (
        lineElement->getInputLineNumber (),
        bsrLineContents::kLineContentsRegular);

    // set lineElement's spacesAfter value if needed
    switch (lineContentsToAppendTo->getLineContentsKind ()) {
      case bsrLineContents::kLineContentsRegular:
        // leave it as 0
        break;
      case bsrLineContents::kLineContentsContinuation:
        lineElement->setSpacesBefore (2);
        break;
    } // switch

    // append it to the line contents list
    fLineContentsList.push_back (lineContentsToAppendTo);
  }
  else {
    lineContentsToAppendTo = fLineContentsList.back ();

    // set lineElement's spacesAfter value
    lineElement->setSpacesBefore (1);
  }


  lineContentsToAppendTo->
    appendLineElementToLineContents (lineElement);
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

  appendLineElementToMeasure (measure);
}

int bsrLine::fetchCellsNumber () const
{
  int result = 0;

  for (
    list<S_bsrLineContents>::const_iterator i = fLineContentsList.begin ();
    i != fLineContentsList.end ();
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
    list<S_bsrLineContents>::const_iterator i = fLineContentsList.begin ();
    i != fLineContentsList.end ();
    i++ ) {
    // browse the element
    bsrBrowser<bsrLineContents> browser (v);
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
    ", lineContents: " << fLineContentsList.size () <<
    ", cellsNumber" << " : " << fetchCellsNumber () <<
    ", line " << fInputLineNumber;
    
  return s.str ();
}

void bsrLine::print (ostream& os)
{
  int lineContentsListSize = fLineContentsList.size ();
  
  os <<
    "Line" <<
    ", " <<
    singularOrPlural (
      lineContentsListSize, "lineContent", "lineContents") <<
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
  
  // print the line contents if any
  if (lineContentsListSize || gBsrOptions->fDisplayBsrDetails) {
    os <<
//      setw (fieldWidth) <<
      "LineContentsList" <<
      ", " <<
      singularOrPlural (
        lineContentsListSize, "lineContent", "lineContents");
    if (lineContentsListSize) {
      os <<
        endl;
      gIndenter++;
  
      list<S_bsrLineContents>::const_iterator
        iBegin = fLineContentsList.begin (),
        iEnd   = fLineContentsList.end (),
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
