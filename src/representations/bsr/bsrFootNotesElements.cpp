/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#include <sstream>
#include <iomanip> // for 'setw()'

#include "bsrFootNotesElements.h"

#include "utilities.h"

#include "oahOah.h"
#include "bsrOah.h"
#include "bsr2brailleOah.h"


using namespace std;

namespace MusicXML2
{

//______________________________________________________________________________
S_bsrFootNotesElement bsrFootNotesElement::create (
  int    inputLineNumber,
  string footNoteText)
{
  bsrFootNotesElement* o =
    new bsrFootNotesElement (
      inputLineNumber, footNoteText);
  assert (o!=0);
  return o;
}

bsrFootNotesElement::bsrFootNotesElement (
  int    inputLineNumber,
  string footNoteText)
    : bsrElement (inputLineNumber)
{
  fFootNoteText = footNoteText;

  fFootNotesElementCellsList =
    bsrCellsList::create (inputLineNumber);
}

bsrFootNotesElement::~bsrFootNotesElement ()
{}

void bsrFootNotesElement::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalBsrOahGroup->getTraceBsrVisitors ()) {
    gLogStream <<
      "% ==> bsrFootNotesElement::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_bsrFootNotesElement>*
    p =
      dynamic_cast<visitor<S_bsrFootNotesElement>*> (v)) {
        S_bsrFootNotesElement elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalBsrOahGroup->getTraceBsrVisitors ()) {
          gLogStream <<
            "% ==> Launching bsrFootNotesElement::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void bsrFootNotesElement::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalBsrOahGroup->getTraceBsrVisitors ()) {
    gLogStream <<
      "% ==> bsrFootNotesElement::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_bsrFootNotesElement>*
    p =
      dynamic_cast<visitor<S_bsrFootNotesElement>*> (v)) {
        S_bsrFootNotesElement elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalBsrOahGroup->getTraceBsrVisitors ()) {
          gLogStream <<
            "% ==> Launching bsrFootNotesElement::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void bsrFootNotesElement::browseData (basevisitor* v)
{}

string bsrFootNotesElement::asString () const
{
  stringstream s;

  s <<
    "FootNotesElement" <<
    // JMI ", fFootNoteText: " <<
    ", \"" << fFootNoteText << "\"" <<
    ", line " << fInputLineNumber;

  return s.str ();
}

void bsrFootNotesElement::print (ostream& os) const
{
  os <<
    "FootNotesElement" <<
    ", line " << fInputLineNumber <<
    endl;

  gIndenter++;

  os <<
    "footNoteText " << " : \"" << fFootNoteText << "\"" <<
    endl;

  gIndenter--;
}

ostream& operator<< (ostream& os, const S_bsrFootNotesElement& elt)
{
  elt->print (os);
  return os;
}


}
