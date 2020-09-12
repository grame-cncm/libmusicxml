/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#include <fstream>      // ofstream, ofstream::open(), ofstream::close()

#include "messagesHandling.h"

#include "setTraceOahIfDesired.h"
#ifdef TRACE_OAH
  #include "traceOah.h"
#endif

#include "generalOah.h"

#include "utilities.h"

#include "bsr2BrailleOah.h"

#include "bsr2BrailleTranslator.h"

#include "bsr2BrailleTranslatorInterface.h"


using namespace std;

namespace MusicXML2
{

//_______________________________________________________________________________
void generateBrailleCodeFromBsrScore (
  const S_bsrScore bScore,
  S_bsrOah         bsrOpts,
  ostream&         logOstream,
  ostream&         brailleCodeOutputStream,
  string           passNumber)
{
  // sanity check
  msgAssert (
    bScore != 0,
    "bScore is null");

  clock_t startClock = clock ();

  string separator =
    "%--------------------------------------------------------------";

#ifdef TRACE_OAH
  if (gGlobalTraceOah->getTracePasses ()) {
    logOstream <<
      endl <<
      separator <<
      endl <<
      gTab <<
      "Pass 4: writing the BSR as Braille code" <<
      endl <<
      separator <<
      endl;
  }
#endif

  // create an bsr2BrailleTranslator
  bsr2BrailleTranslator
    translator (
      bScore,
      bsrOpts,
      logOstream,
      brailleCodeOutputStream);

  // build the Braille score
  translator.generateBrailleCodeFromBsrScore ();

  clock_t endClock = clock ();

  // register time spent
  timing::gTiming.appendTimingItem (
    "Pass 4",
    "translate BSR to Braille",
    timingItem::kMandatory,
    startClock,
    endClock);
}

//_______________________________________________________________________________
void convertBsrScoreToBrailleText (
  string     outputFileName,
  S_bsrScore bScore,
  string     passNumber)
{
  int outputFileNameSize = outputFileName.size ();

  if (! gGlobalBsr2BrailleOah->fNoBrailleCode) {
    // open output file if need be
    // ------------------------------------------------------

    if (outputFileNameSize) {
#ifdef TRACE_OAH
      if (gGlobalTraceOah->getTracePasses ()) {
        gLogOstream <<
          "Opening file '" << outputFileName << "' for writing" <<
          endl;
      }
#endif

      ofstream
        brailleCodeFileOutputStream (
          outputFileName.c_str (),
          ofstream::out);

      if (! brailleCodeFileOutputStream.is_open ()) {
        stringstream s;

        s <<
          "Could not open braille music output file \"" <<
          outputFileName <<
          "\" for writing, exiting";

        string message = s.str ();

        gLogOstream <<
          message <<
          endl;

        throw bsrScoreToBrailleTextException (message);
      }

      // convert the BSR score to Braille music
      generateBrailleCodeFromBsrScore (
        bScore,
        gGlobalBsrOah,
        gLogOstream,
        brailleCodeFileOutputStream,
        passNumber);

#ifdef TRACE_OAH
      if (gGlobalTraceOah->getTracePasses ()) {
        gLogOstream <<
          endl <<
          "Closing file '" << outputFileName << "'" <<
          endl;
      }
#endif

      brailleCodeFileOutputStream.close ();
    }

    else {
#ifdef TRACE_OAH
      if (gGlobalTraceOah->getTracePasses ()) {
        gLogOstream <<
          endl <<
          "Braille music will be written to standard output" <<
          endl;
      }
#endif

      // create an indented output stream for the Braille music
      // to be written to cout
      indentedOstream
        brailleCodeCoutOutputStream (
          cout,
          gIndenter);

      // convert the BSR score to Braille music
      generateBrailleCodeFromBsrScore (
        bScore,
        gGlobalBsrOah,
        gLogOstream,
        brailleCodeCoutOutputStream,
        passNumber);
    }
  }

  if (gIndenter != 0) {
    if (! gGlobalGeneralOah->fQuiet) {
      stringstream s;

      s <<
        "gIndenter value after pass 4: "<<
        gIndenter.getIndent ();

      msrMusicXMLWarning (
        gGlobalOahOah->fInputSourceName,
        1, // JMI inputLineNumber,
        s.str ());
    }

    gIndenter.resetToZero ();
  }
}


}
