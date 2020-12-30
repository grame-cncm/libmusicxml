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

#include "utilities.h"
#include "messagesHandling.h"

#include "bsrLineContents.h"

/* the following due to:
/Users/menu/libmusicxml-git/src/lib/smartpointer.h:75:40: error: member access into incomplete type 'MusicXML2::bsrLine'
/Users/menu/libmusicxml-git/src/representations/bsr/bsrLineContents.cpp:51:18: note: in instantiation of member function 'MusicXML2::SMARTP<MusicXML2::bsrLine>::~SMARTP' requested here
1 error generated.
*/
#include "bsrLineContents.h"

#include "enableTracingIfDesired.h"
#ifdef TRACING_IS_ENABLED
  #include "traceOah.h"
#endif

#include "oahOah.h"
#include "generalOah.h"

#include "bsrOah.h"
#include "msr2bsrOah.h"
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
  assert (o!=0);
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
#ifdef TRACING_IS_ENABLED
  if (gGlobalBsrOahGroup->getTraceLines ()) {
    gLogStream <<
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
  S_bsrLineContentsElement lineElement)
{
  fLineContentsLineElementsList.push_back (lineElement);
}

void bsrLineContents::insertLineElementBeforeLastElementOfLineContents (
  S_bsrLineContentsElement lineElement)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceTimes () || gGlobalTraceOahGroup->getTraceMeasures ()) {
    gLogStream <<
      "Inserting line element '" <<
      lineElement->asShortString () <<
      "' before the last element of line contents '" <<
      asString () <<
      "'" <<
      endl;
    }
#endif

  unsigned int
    lineContentsElementsListSize =
      fLineContentsLineElementsList.size ();

  if (lineContentsElementsListSize > 0) {
    list<S_bsrLineContentsElement>::iterator it =
      fLineContentsLineElementsList.begin();

    std::advance (it, lineContentsElementsListSize - 1);

    fLineContentsLineElementsList.insert (
      it, lineElement);
  }
  else {
    stringstream s;

    s <<
      "line contents elementslist is empty, cannot insert '" <<
      lineElement->asShortString () <<
      "' before its last element";

    bsrInternalError (
      gGlobalOahOahGroup->getInputSourceName (),
      lineElement->getInputLineNumber (),
      __FILE__, __LINE__,
      s.str ());
  }
}

S_bsrCellsList bsrLineContents::buildLineContentsElementsList () const
{
  S_bsrCellsList
    result =
      bsrCellsList::create (fInputLineNumber);

  for (
    list<S_bsrLineContentsElement>::const_iterator i =
      fLineContentsLineElementsList.begin ();
    i != fLineContentsLineElementsList.end ();
    i++
  ) {
    S_bsrLineContentsElement lineElement = (*i);

    result->
      appendCellsListToCellsList (
        lineElement->
          fetchCellsList ());
  } // for

  return result;
}

int bsrLineContents::fetchCellsNumber () const
{
  int result = 0;

  for (
    list<S_bsrLineContentsElement>::const_iterator i =
      fLineContentsLineElementsList.begin ();
    i != fLineContentsLineElementsList.end ();
    i++
  ) {
    S_bsrLineContentsElement lineElement = (*i);

    result += lineElement->fetchCellsNumber ();
  } // for

  return result;
}

void bsrLineContents::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalBsrOahGroup->getTraceBsrVisitors ()) {
    gLogStream <<
      "% ==> bsrLineContents::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_bsrLineContents>*
    p =
      dynamic_cast<visitor<S_bsrLineContents>*> (v)) {
        S_bsrLineContents elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalBsrOahGroup->getTraceBsrVisitors ()) {
          gLogStream <<
            "% ==> Launching bsrLineContents::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void bsrLineContents::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalBsrOahGroup->getTraceBsrVisitors ()) {
    gLogStream <<
      "% ==> bsrLineContents::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_bsrLineContents>*
    p =
      dynamic_cast<visitor<S_bsrLineContents>*> (v)) {
        S_bsrLineContents elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalBsrOahGroup->getTraceBsrVisitors ()) {
          gLogStream <<
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
    list<S_bsrLineContentsElement>::const_iterator i =
      fLineContentsLineElementsList.begin ();
    i != fLineContentsLineElementsList.end ();
    i++ ) {
    // browse the element
    bsrBrowser<bsrLineContentsElement> browser (v);
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
      fLineContentsLineElementsList.size (), "lineElement", "lineElements");

  return s.str ();
}

string bsrLineContents::asString () const
{
  // this can be overriden in actual elements
  return asShortString ();
}

void bsrLineContents::print (ostream& os) const
{
  unsigned int lineElementsListSize =
    fLineContentsLineElementsList.size ();

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
  const unsigned int fieldWidth = 17;

  os << left <<
    setw (fieldWidth) <<
    "cellsNumber" << " : " << fetchCellsNumber () <<
    endl;
  os << endl;

  // print the line elements if any
  if (lineElementsListSize || gGlobalBsrOahGroup->getDisplayBsrDetails ()) {
    os <<
//      setw (fieldWidth) <<
      "LineContentsLineElementsList" <<
      ", " <<
      singularOrPlural (
        lineElementsListSize, "lineElement", "lineElements");
    if (lineElementsListSize) {
      os << endl;
      gIndenter++;

      list<S_bsrLineContentsElement>::const_iterator
        iBegin = fLineContentsLineElementsList.begin (),
        iEnd   = fLineContentsLineElementsList.end (),
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

string bsrLineContents::asDebugString () const
{
  stringstream s;

  unsigned int lineElementsListSize =
    fLineContentsLineElementsList.size ();

  // collect the line elements debug strings if any
  if (lineElementsListSize) {
    list<S_bsrLineContentsElement>::const_iterator
      iBegin = fLineContentsLineElementsList.begin (),
      iEnd   = fLineContentsLineElementsList.end (),
      i      = iBegin;
    for ( ; ; ) {
      s << (*i)->asDebugString ();
      if (++i == iEnd) break;
 // JMI     s << " ";
    } // for
  }

  return s.str ();
}

ostream& operator<< (ostream& os, const S_bsrLineContents& elt)
{
  elt->print (os);
  return os;
}


}
