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

/*
#include "xml.h"
#include "xmlfile.h"
#include "xmlreader.h"
*/
#include "versions.h"

#include "lpsr2LilypondInterface.h"

#include "lpsr2LilypondTranslator.h"

using namespace std;

namespace MusicXML2 
{

// useful shortcut macros
#define idtr indenter::gIndenter
#define tab  indenter::gIndenter.getSpacer ()

//_______________________________________________________________________________
/*
 * The method that converts the file contents to LilyPond code
 * and  writes the result to the output stream
*/
void lpsr2Lilypond (
  const S_lpsrScore lpScore,
  S_msrOptions&     msrOpts,
  S_lpsrOptions&    lpsrOpts,
  ostream&          os) 
{  
  // generate LilyPond code from LPSR score
  generateLilypondCodeFromLpsrScore (
    lpScore, msrOpts, lpsrOpts, os);
}

//_______________________________________________________________________________
void generateLilypondCodeFromLpsrScore (
  const S_lpsrScore lpScore,
  S_msrOptions&     msrOpts,
  S_lpsrOptions&    lpsrOpts,
  ostream&          os)
{
  clock_t startClock = clock();

  string separator =
    "%--------------------------------------------------------------";

  if (gGeneralOptions->fTraceGeneral) {
    cerr <<
      endl <<
      idtr <<
        separator <<
        endl <<
      idtr <<
        "Pass 4: writing the LPSR as LilyPond code" <<
        endl <<
      idtr <<
        separator <<
        endl;
  }
  
  // create an lpsr2LilypondTranslator
  lpsr2LilypondTranslator translator (msrOpts, lpsrOpts, os, lpScore);
  
  // build the LPSR score    
  translator.generateLilypondCodeFromLpsrScore ();
  
  if (gGeneralOptions->fTraceGeneral)
    os << separator << endl;

  clock_t endClock = clock();

  // register time spent
  timing::gTiming.appendTimingItem (
    "Pass 4: translate LPSR to LilyPond",
    timingItem::kMandatory,
    startClock,
    endClock);
}


}
