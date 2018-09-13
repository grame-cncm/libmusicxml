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
#include <list>
#include <algorithm>
#include <iomanip>      // setw, setprecision, ...
#include <climits>      /* INT_MIN, INT_MAX */

#include "version.h"

#include "lpsr.h"

#ifdef TRACE_OPTIONS
  #include "traceOptions.h"
#endif

#include "lpsrOptions.h"
#include "lilypondOptions.h"

#include "xml2lyOptionsHandling.h"


using namespace std;

namespace MusicXML2 
{

/* JMI
//______________________________________________________________________________
S_lpsrMelismaCommand lpsrMelismaCommand::create (
  int             inputLineNumber,
  lpsrMelismaKind melismaKind)
{
  lpsrMelismaCommand* o =
    new lpsrMelismaCommand (
      inputLineNumber, melismaKind);
  assert(o!=0);
  return o;
}

lpsrMelismaCommand::lpsrMelismaCommand (
  int             inputLineNumber,
  lpsrMelismaKind melismaKind)
    : lpsrElement (inputLineNumber)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceLyrics) {
    gLogIOstream <<
      "--> creating lpsrMelismaCommand " <<
      melismaKindAsString (melismaKind) <<
      endl;
  }
#endif

  fMelismaKind = melismaKind;
}

lpsrMelismaCommand::~lpsrMelismaCommand ()
{}

void lpsrMelismaCommand::acceptIn (basevisitor* v)
{
  if (gLpsrOptions->fTraceLpsrVisitors) {
    gLogIOstream <<
      "% ==> lpsrMelismaCommand::acceptIn ()" <<
      endl;
  }
      
  if (visitor<S_lpsrMelismaCommand>*
    p =
      dynamic_cast<visitor<S_lpsrMelismaCommand>*> (v)) {
        S_lpsrMelismaCommand elem = this;
        
        if (gLpsrOptions->fTraceLpsrVisitors) {
          gLogIOstream <<
            "% ==> Launching lpsrMelismaCommand::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void lpsrMelismaCommand::acceptOut (basevisitor* v)
{
  if (gLpsrOptions->fTraceLpsrVisitors) {
    gLogIOstream <<
      "% ==> lpsrMelismaCommand::acceptOut ()" <<
      endl;
  }

  if (visitor<S_lpsrMelismaCommand>*
    p =
      dynamic_cast<visitor<S_lpsrMelismaCommand>*> (v)) {
        S_lpsrMelismaCommand elem = this;
      
        if (gLpsrOptions->fTraceLpsrVisitors) {
          gLogIOstream <<
            "% ==> Launching lpsrMelismaCommand::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void lpsrMelismaCommand::browseData (basevisitor* v)
{}

ostream& operator<< (ostream& os, const S_lpsrMelismaCommand& nstf)
{
  nstf->print (os);
  return os;
}

string lpsrMelismaCommand::melismaKindAsString (
  lpsrMelismaKind melismaKind)
{
  string result;

  switch (melismaKind) {
    case lpsrMelismaCommand::kMelismaStart:
      result = "Start";
      break;
    case lpsrMelismaCommand::kMelismaEnd:
      result = "End";
      break;
  } // switch

  return result;
}

void lpsrMelismaCommand::print (ostream& os)
{
  os <<
    "MelismaCommand" << " " <<
    melismaKindAsString (fMelismaKind) <<
    endl;
}
*/

//______________________________________________________________________________
void initializeLPSR ()
{
  initializeLPSRBasicTypes ();
}


}
