/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#include <climits>      // INT_MIN, INT_MAX

#include "msrVoiceElements.h"

#include "msrOah.h"


using namespace std;

namespace MusicXML2
{
//______________________________________________________________________________
msrVoiceElement::msrVoiceElement (
  int inputLineNumber)
    : msrElement (inputLineNumber)
{
  /* JMI
  fVoiceNumber = "???";
  fPositionInVoice = rational (INT_MIN, 1);
  */
}

msrVoiceElement::~msrVoiceElement ()
{}

/* JMI
void msrVoiceElement::acceptIn (basevisitor* v)
{
  if (gMsrOah->fTraceMsrVisitors) {
    gLogOstream <<
      "% ==> msrVoiceElement::acceptIn ()" <<
      endl;
  }

  if (visitor<S_msrVoiceElement>*
    p =
      dynamic_cast<visitor<S_msrVoiceElement>*> (v)) {
        S_msrVoiceElement elem = this;

        if (gMsrOah->fTraceMsrVisitors) {
          gLogOstream <<
            "% ==> Launching msrVoiceElement::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrVoiceElement::acceptOut (basevisitor* v)
{
  if (gMsrOah->fTraceMsrVisitors) {
    gLogOstream <<
      "% ==> msrVoiceElement::acceptOut ()" <<
      endl;
  }

  if (visitor<S_msrVoiceElement>*
    p =
      dynamic_cast<visitor<S_msrVoiceElement>*> (v)) {
        S_msrVoiceElement elem = this;

        if (gMsrOah->fTraceMsrVisitors) {
          gLogOstream <<
            "% ==> Launching msrVoiceElement::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

string msrVoiceElement::asString () const
{
  // this is overriden all in actual elements
  return "??? msrVoiceElement::asString () ???";
}

string msrVoiceElement::asShortString () const
{
  // this can be overriden in actual elements
  return asString () const;
}

void msrVoiceElement::print (ostream& os) const
{
  os << asString () << endl;
}
*/

ostream& operator<< (ostream& os, const S_msrVoiceElement& elt)
{
  elt->print (os);
  return os;
}

}
