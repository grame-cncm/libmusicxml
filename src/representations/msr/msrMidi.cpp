/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#include <iostream>
#include <sstream>
#include <iomanip>      // setw, setprecision, ...

#include "utilities.h"

#include "msrMidi.h"

#include "oahOah.h"

#include "msrOah.h"


using namespace std;

namespace MusicXML2
{

//______________________________________________________________________________
S_msrMidiTempo msrMidiTempo::create (
  int    inputLineNumber,
  string midiTempoDuration,
  int    midiTempoPerSecond)
{
  msrMidiTempo* o =
    new msrMidiTempo (
      inputLineNumber,
      midiTempoDuration,
      midiTempoPerSecond);
  assert (o!=0);
  return o;
}

msrMidiTempo::msrMidiTempo (
  int    inputLineNumber,
  string midiTempoDuration,
  int    midiTempoPerSecond)
    : msrElement (inputLineNumber)
{
  fMidiTempoDuration = midiTempoDuration;
  fMidiTempoPerSecond = midiTempoPerSecond;
}

msrMidiTempo::msrMidiTempo ()
    : msrElement (0)
{
  fMidiTempoDuration = "4";
  fMidiTempoPerSecond = 90;
}

msrMidiTempo::~msrMidiTempo ()
{}

S_msrMidiTempo msrMidiTempo::createMsrMidiTempoNewbornClone ()
{
  S_msrMidiTempo
    newbornClone =
      msrMidiTempo::create (
        fInputLineNumber,
        fMidiTempoDuration,
        fMidiTempoPerSecond);

  return newbornClone;
}

void msrMidiTempo::acceptIn (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrMidiTempo::acceptIn ()" <<
      endl;
  }

  if (visitor<S_msrMidiTempo>*
    p =
      dynamic_cast<visitor<S_msrMidiTempo>*> (v)) {
        S_msrMidiTempo elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrMidiTempo::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrMidiTempo::acceptOut (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrMidiTempo::acceptOut ()" <<
      endl;
  }

  if (visitor<S_msrMidiTempo>*
    p =
      dynamic_cast<visitor<S_msrMidiTempo>*> (v)) {
        S_msrMidiTempo elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrMidiTempo::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrMidiTempo::browseData (basevisitor* v)
{}

string msrMidiTempo::asString () const
{
  stringstream s;

  s <<
    "MidiTempo" <<
    ", midiTempoDuration = \"" << fMidiTempoDuration << "\"" <<
    ", midiTempoPerSecond = " << fMidiTempoPerSecond <<
    ", line " << fInputLineNumber;

  return s.str ();
}

void msrMidiTempo::print (ostream& os) const
{
  os << "MidiTempo" << endl;

  gIndenter++;

  const int fieldWidth = 18;

  os << left <<
    setw (fieldWidth) <<
    "midiTempoDuration" << " = " << fMidiTempoDuration <<
    endl <<
    setw (fieldWidth) <<
    "midiTempoPerSecond" << " = " << fMidiTempoPerSecond <<
    endl << endl;

  gIndenter--;
}

ostream& operator<< (ostream& os, const S_msrMidiTempo& elt)
{
  elt->print (os);
  return os;
}


}
