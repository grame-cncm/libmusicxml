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
#include <iomanip>      // setw, setprecision, ...

#include "msrMidi.h"

#include "msrOptions.h"


using namespace std;

namespace MusicXML2 {

//______________________________________________________________________________
S_msrMidi msrMidi::create (
  int    inputLineNumber,
  string midiTempoDuration,
  int    midiTempoPerSecond)
{
  msrMidi* o =
    new msrMidi (
      inputLineNumber,
      midiTempoDuration,
      midiTempoPerSecond);
  assert(o!=0);
  return o;
}

msrMidi::msrMidi (
  int    inputLineNumber,
  string midiTempoDuration,
  int    midiTempoPerSecond)
    : msrElement (inputLineNumber)
{
  fMidiTempoDuration = midiTempoDuration;
  fMidiTempoPerSecond = midiTempoPerSecond;
}

msrMidi::~msrMidi ()
{}

void msrMidi::acceptIn (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrMidi::acceptIn ()" <<
      endl;
  }
      
  if (visitor<S_msrMidi>*
    p =
      dynamic_cast<visitor<S_msrMidi>*> (v)) {
        S_msrMidi elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrMidi::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrMidi::acceptOut (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrMidi::acceptOut ()" <<
      endl;
  }

  if (visitor<S_msrMidi>*
    p =
      dynamic_cast<visitor<S_msrMidi>*> (v)) {
        S_msrMidi elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrMidi::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrMidi::browseData (basevisitor* v)
{}

ostream& operator<< (ostream& os, const S_msrMidi& elt)
{
  elt->print (os);
  return os;
}

void msrMidi::print (ostream& os)
{
  os << "Midi" << endl;

  gIndenter++;

  const int fieldWidth = 18;

  os << left <<
    setw (fieldWidth) <<
    "MidiTempoDuration" << " = " << fMidiTempoDuration <<
    endl <<
    setw (fieldWidth) <<
    "MidiTempoPerSecond" << " = " << fMidiTempoPerSecond <<
    endl <<
    endl;
      
  gIndenter--;
}    


}
