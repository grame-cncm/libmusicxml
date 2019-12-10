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

#include "bsrMutualDependencies.h"

#include "oahOah.h"
#include "generalOah.h"

#include "setTraceOahIfDesired.h"
#ifdef TRACE_OAH
  #include "traceOah.h"
#endif

#include "bsrOah.h"
#include "brailleOah.h"


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
#ifdef TRACE_OAH
  if (gBsrOah->fTraceLines) {
    gLogOstream <<
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
void bsrLineContents::setBsrLineUpLink (
S_bsrLine bsrLineUpLink)
{
  fBsrLineUpLink = bsrLineUpLink;
}

S_bsrLine bsrLineContents::getBsrLineUpLink () const
{
  return fBsrLineUpLink;
}
*/

void bsrLineContents::appendLineElementToLineContents (
  S_bsrLineElement lineElement)
{
  fLineContentsElementsList.push_back (lineElement);
}

void bsrLineContents::insertLineElementBeforeLastElementOfLineContents (
  S_bsrLineElement lineElement)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceTimes || gTraceOah->fTraceMeasures) {
    gLogOstream <<
      "Inserting line element '" <<
      lineElement->asShortString () <<
      "' before the last element of line contents '" <<
      asString () <<
      "'" <<
      endl;
    }
#endif

  int
    lineContentsElementsListSize =
      fLineContentsElementsList.size ();

  if (lineContentsElementsListSize > 0) {
    list<S_bsrLineElement>::iterator it = fLineContentsElementsList.begin();

    std::advance (it, lineContentsElementsListSize - 1);

    fLineContentsElementsList.insert (it, lineElement);
  }
  else {
    stringstream s;

    s <<
      "line contents elementslist is empty, cannot insert '" <<
      lineElement->asShortString () <<
      "' before its last element";

    bsrInternalError (
      gOahOah->fInputSourceName,
      lineElement->getInputLineNumber (),
      __FILE__, __LINE__,
      s.str ());
  }
}

int bsrLineContents::fetchCellsNumber () const
{
  int result = 0;

  for (
    list<S_bsrLineElement>::const_iterator i = fLineContentsElementsList.begin ();
    i != fLineContentsElementsList.end ();
    i++
  ) {
    S_bsrLineElement lineElement = (*i);

    result += lineElement->fetchCellsNumber ();
  } // for

  return result;
}

void bsrLineContents::acceptIn (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gBsrOah->fTraceBsrVisitors) {
    gLogOstream <<
      "% ==> bsrLineContents::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_bsrLineContents>*
    p =
      dynamic_cast<visitor<S_bsrLineContents>*> (v)) {
        S_bsrLineContents elem = this;

#ifdef TRACE_OAH
        if (gBsrOah->fTraceBsrVisitors) {
          gLogOstream <<
            "% ==> Launching bsrLineContents::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void bsrLineContents::acceptOut (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gBsrOah->fTraceBsrVisitors) {
    gLogOstream <<
      "% ==> bsrLineContents::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_bsrLineContents>*
    p =
      dynamic_cast<visitor<S_bsrLineContents>*> (v)) {
        S_bsrLineContents elem = this;

#ifdef TRACE_OAH
        if (gBsrOah->fTraceBsrVisitors) {
          gLogOstream <<
            "% ==> Launching bsrLineContents::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void bsrLineContents::browseData (basevisitor* v)
{
  for (
    list<S_bsrLineElement>::const_iterator i = fLineContentsElementsList.begin ();
    i != fLineContentsElementsList.end ();
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

string bsrLineContents::asShortString () const
{
  /* JMI
  // this is overriden all in actual elements
  return "??? bsrLineContents::asString () ???";
  */

  stringstream s;

  s <<
   "LineContents" <<
    ", lineContentsKind: " <<
    lineContentsKindAsString (fLineContentsKind) <<
    ", " <<
    singularOrPlural (
      fLineContentsElementsList.size (), "lineElement", "lineElements");

  return s.str ();
}

string bsrLineContents::asString () const
{
  // this can be overriden in actual elements
  return asShortString ();
}

void bsrLineContents::print (ostream& os) const
{
  int lineElementsListSize = fLineContentsElementsList.size ();

  os <<
    "LineContents" <<
    ", lineContentsKind: " <<
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
  os << endl;

  // print the line elements if any
  if (lineElementsListSize || gBsrOah->fDisplayBsrDetails) {
    os <<
//      setw (fieldWidth) <<
      "LineElementsList" <<
      ", " <<
      singularOrPlural (
        lineElementsListSize, "lineElement", "lineElements");
    if (lineElementsListSize) {
      os << endl;
      gIndenter++;

      list<S_bsrLineElement>::const_iterator
        iBegin = fLineContentsElementsList.begin (),
        iEnd   = fLineContentsElementsList.end (),
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

  fLineNumberCellsList = buildLineNumberCellsList ();

  fASpaceIsNeededInLine = true;

#ifdef TRACE_OAH
  if (gBsrOah->fTraceLines) {
    gLogOstream <<
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
#ifdef TRACE_OAH
  if (gBsrOah->fTraceLines) {
    gLogOstream <<
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

void bsrLine::appendLineElementToLine (S_bsrLineElement lineElement)
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
    // JMI    lineElement->setSpacesBefore (2);
        break;
    } // switch

    // append it to the line contents list
    fLineContentsList.push_back (lineContentsToAppendTo);
  }
  else {
    lineContentsToAppendTo = fLineContentsList.back ();

    // set lineElement's spacesAfter value
 // JMI   lineElement->setSpacesBefore (1);
  }

  if (fASpaceIsNeededInLine) {
    // append a space to the line elements list // JMI appendSpacesToLine ???
    lineContentsToAppendTo->
      appendLineElementToLineContents (
        bsrSpaces::create (
          fInputLineNumber, 1));

    fASpaceIsNeededInLine = false;
  }

  lineContentsToAppendTo->
    appendLineElementToLineContents (lineElement);
}

void bsrLine::insertElementBeforeLastElementOfLine (
  S_bsrLineElement lineElement)
{
  S_bsrLineContents
    lineContentsToAppendTo;

  if (! fLineContentsList.size ()) {
    // first insertion in this line: create the first, regular line contents
    lineContentsToAppendTo =
      bsrLineContents::create (
        lineElement->getInputLineNumber (),
        bsrLineContents::kLineContentsRegular);

    // append it to the line contents list
    fLineContentsList.push_back (lineContentsToAppendTo);
  }
  else {
    lineContentsToAppendTo = fLineContentsList.back ();
  }

  lineContentsToAppendTo->
    insertLineElementBeforeLastElementOfLineContents (lineElement);
}

void bsrLine::appendSpacesToLine (S_bsrSpaces spaces)
{
#ifdef TRACE_OAH
  if (gBsrOah->fTraceSpaces || gBsrOah->fTraceLines) {
    gLogOstream <<
      "Appending spaces '" <<
      spaces->asShortString () <<
      "' to line '" <<
      asString () <<
      "'" <<
      endl;
    }
#endif

  appendLineElementToLine (spaces);
}

void bsrLine::appendKeyToLine (S_bsrKey key)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceKeys || gTraceOah->fTraceMeasures) {
    gLogOstream <<
      "Appending key '" <<
      key->asShortString () <<
      "' to line '" <<
      asString () <<
      "'" <<
      endl;
    }
#endif

  appendLineElementToLine (key);
  fASpaceIsNeededInLine = true;
}

void bsrLine::appendTimeToLine (S_bsrTime time)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceTimes || gTraceOah->fTraceMeasures) {
    gLogOstream <<
      "Appending time '" <<
      time->asShortString () <<
      "' to line '" <<
      asString () <<
      "'" <<
      endl;
    }
#endif

  appendLineElementToLine (time);
  fASpaceIsNeededInLine = true;
}

void bsrLine::insertTimeBeforeLastElementOfLine (S_bsrTime time)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceTimes || gTraceOah->fTraceMeasures) {
    gLogOstream <<
      "Inserting time '" <<
      time->asShortString () <<
      "' before the last element of line '" <<
      asString () <<
      "'" <<
      endl;
    }
#endif

  insertElementBeforeLastElementOfLine (time);
  fASpaceIsNeededInLine = true;
}

void bsrLine::appendTempoToLine (S_bsrTempo tempo)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceTempos || gTraceOah->fTraceMeasures) {
    gLogOstream <<
      "Appending tempo '" <<
      tempo->asShortString () <<
      "' to line '" <<
      asString () <<
      "'" <<
      endl;
    }
#endif

  appendLineElementToLine (tempo);
  fASpaceIsNeededInLine = true;
}

void bsrLine::appendMeasureToLine (S_bsrMeasure measure)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceMeasures || gBsrOah->fTraceLines) {
    gLogOstream <<
      "Appending line '" <<
      measure->asShortString () <<
      "' to line '" <<
      asString () <<
      "'" <<
      endl;
    }
#endif

  appendLineElementToLine (measure);
  fASpaceIsNeededInLine = true;
}

void bsrLine::appendLineElementToLastMeasureOfLine (
  S_bsrLineElement lineElement)
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
    // JMI    lineElement->setSpacesBefore (2);
        break;
    } // switch

    // append it to the line contents list
    fLineContentsList.push_back (lineContentsToAppendTo);
  }
  else {
    lineContentsToAppendTo = fLineContentsList.back ();

    // set lineElement's spacesAfter value
 // JMI   lineElement->setSpacesBefore (1);
  }

  if (fASpaceIsNeededInLine) {
    // append a space to the line elements list // JMI appendSpacesToLine ???
    lineContentsToAppendTo->
      appendLineElementToLineContents (
        bsrSpaces::create (
          fInputLineNumber, 1));

    fASpaceIsNeededInLine = false;
  }

  lineContentsToAppendTo->
    appendLineElementToLineContents (lineElement);
}

void bsrLine::appendNoteToLine (S_bsrNote note)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceNotes || gTraceOah->fTraceMeasures) {
    gLogOstream <<
      "Appending note '" <<
      note->asShortString () <<
      "' to line '" <<
      asString () <<
      "'" <<
      endl;
    }
#endif

  appendLineElementToLastMeasureOfLine (note); // last measure? JMI
}

S_bsrCellsList bsrLine::buildLineNumberCellsList () const
{
  S_bsrCellsList
    result =
      bsrCellsList::create (fInputLineNumber);

  if (! gBsrOah->fNoLineNumbers) {
    // create the print line number
    S_bsrNumber
      printLineNumber =
        bsrNumber::create (
          fInputLineNumber,
          fPrintLineNumber,
          bsrNumber::kNumberSignIsNeededYes);

    // append it to result
    result->appendCellsListToCellsList (
      printLineNumber->fetchCellsList ());

    if (fBrailleLineNumber != fPrintLineNumber) { // JMI
      // create the braille line number
      S_bsrNumber
        brailleLineNumber =
          bsrNumber::create (
            fInputLineNumber,
            fBrailleLineNumber,
            bsrNumber::kNumberSignIsNeededYes);

      // append it to result
      result->appendCellsListToCellsList (
        brailleLineNumber->fetchCellsList ());
    }
  }

  return result;
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
#ifdef TRACE_OAH
  if (gBsrOah->fTraceBsrVisitors) {
    gLogOstream <<
      "% ==> bsrLine::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_bsrLine>*
    p =
      dynamic_cast<visitor<S_bsrLine>*> (v)) {
        S_bsrLine elem = this;

#ifdef TRACE_OAH
        if (gBsrOah->fTraceBsrVisitors) {
          gLogOstream <<
            "% ==> Launching bsrLine::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void bsrLine::acceptOut (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gBsrOah->fTraceBsrVisitors) {
    gLogOstream <<
      "% ==> bsrLine::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_bsrLine>*
    p =
      dynamic_cast<visitor<S_bsrLine>*> (v)) {
        S_bsrLine elem = this;

#ifdef TRACE_OAH
        if (gBsrOah->fTraceBsrVisitors) {
          gLogOstream <<
            "% ==> Launching bsrLine::visitEnd ()" <<
            endl;
        }
#endif
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

void bsrLine::print (ostream& os) const
{
  int lineContentsListSize = fLineContentsList.size ();

  os <<
    "Line" <<
    ", printLineNumber: " << fPrintLineNumber <<
    ", " <<
    singularOrPlural (
      lineContentsListSize, "lineContent", "lineContents") <<
    endl;

  gIndenter++;

  // print the line numbers
  const int fieldWidth = 20;

  os << left <<
  /* JMI
    setw (fieldWidth) <<
    "printLineNumber" << " : " << fPrintLineNumber <<
    endl <<
    */
    setw (fieldWidth) <<
    "brailleLineNumber" << " : " << fBrailleLineNumber <<
    endl <<
    setw (fieldWidth) <<
    "cellsPerLine" << " : " << fCellsPerLine <<
    endl <<
    setw (fieldWidth) <<
    "lineNumberCellsList" << " : " << buildLineNumberCellsList () <<
    endl <<
    setw (fieldWidth) <<
    "cellsNumber" << " : " << fetchCellsNumber () <<
    endl;
  os << endl;

  // print the line contents if any
  if (lineContentsListSize || gBsrOah->fDisplayBsrDetails) {
    os <<
//      setw (fieldWidth) <<
      "LineContentsList" <<
      ", " <<
      singularOrPlural (
        lineContentsListSize, "lineContent", "lineContents");
    if (lineContentsListSize) {
      os << endl;
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
