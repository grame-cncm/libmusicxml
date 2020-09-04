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

#include "generalOah.h"

#include "setTraceOahIfDesired.h"
#ifdef TRACE_OAH
  #include "traceOah.h"
#endif

#include "lpsr2LilypondOah.h"

#include "lpsr2LilypondTranslator.h"

#include "lpsr2LilypondInterface.h"


using namespace std;

namespace MusicXML2
{

//_______________________________________________________________________________
void generateLilypondCodeFromLpsrScore (
  const S_lpsrScore lpScore,
  S_msrOah          msrOpts,
  S_lpsrOah         lpsrOpts,
  ostream&  logOstream,
  ostream&  lilypondCodeOstream,
  string            passNumber)
{
  // sanity check
  msrAssert (
    lpScore != 0,
    "lpScore is null");

  clock_t startClock = clock ();

  string separator =
    "%--------------------------------------------------------------";

#ifdef TRACE_OAH
  if (gTraceOah->getTracePasses ()) {
    logOstream <<
      endl <<
      separator <<
      endl <<
      gTab <<
      passNumber << ": writing the LPSR as LilyPond code" <<
      endl <<
      separator <<
      endl;
  }
#endif

  // create an lpsr2LilypondTranslator
  lpsr2LilypondTranslator
    translator (
      lpScore,
      msrOpts,
      lpsrOpts,
      logOstream,
      lilypondCodeOstream);

  // build the LilyPond score
  translator.generateLilypondCodeFromLpsrScore ();

  clock_t endClock = clock ();

  // register time spent
  timing::gTiming.appendTimingItem (
    passNumber,
    "translate LPSR to LilyPond",
    timingItem::kMandatory,
    startClock,
    endClock);
}

//_______________________________________________________________________________
void convertLpsrScoreToLilypondCode (
  string      outputFileName,
  S_lpsrScore lpScore,
  string      passNumber)
{
  int outputFileNameSize = outputFileName.size ();

  if (gLpsr2LilypondOah->fNoLilypondCode) {
    gLogOstream <<
      "Option '-nolpc -no-lilypond-code' is set, no LilyPond code is created" <<
      endl <<
      endl;
  }
  else {
    // open output file if need be
    // ------------------------------------------------------

    ofstream outFileStream;

    if (outputFileNameSize) {
#ifdef TRACE_OAH
      if (gTraceOah->getTracePasses ()) {
        gLogOstream <<
          "Opening file '" << outputFileName << "' for writing" <<
          endl;
      }
#endif

      outFileStream.open (
        outputFileName.c_str(),
        ofstream::out);

      if (! outFileStream.is_open ()) {
        stringstream s;

        s <<
          "Could not open LilyPond output file \"" <<
          outputFileName <<
          "\" for writing, exiting";

        string message = s.str ();

        gLogOstream <<
          message <<
          endl;

        throw lpsrScoreToLilypondException (message);
      }

      // create an indented output stream for the LilyPond code
      // to be written to outFileStream
      indentedOstream
        lilypondCodeFileOutputStream (
          outFileStream,
          gIndenter);

      // convert the LPSR score to LilyPond code
      generateLilypondCodeFromLpsrScore (
        lpScore,
        gMsrOah,
        gLpsrOah,
        gLogOstream,
        lilypondCodeFileOutputStream,
        passNumber);
    }

    else {
#ifdef TRACE_OAH
      if (gTraceOah->getTracePasses ()) {
        gLogOstream <<
          endl <<
          "LilyPond code will be written to standard output" <<
          endl;
      }
#endif

      // create an indented output stream for the LilyPond code
      // to be written to cout
      indentedOstream
        brailleCodeCoutOutputStream (
          cout,
          gIndenter);

      // convert the LPSR score to LilyPond code
      generateLilypondCodeFromLpsrScore (
        lpScore,
        gMsrOah,
        gLpsrOah,
        gLogOstream,
        brailleCodeCoutOutputStream,
        passNumber);
    }

    if (outputFileNameSize) {
#ifdef TRACE_OAH
      if (gTraceOah->getTracePasses ()) {
        gLogOstream <<
          endl <<
          "Closing file '" << outputFileName << "'" <<
          endl;
      }
#endif

      outFileStream.close ();
    }
  }

  if (gIndenter != 0) {
    if (! gGeneralOah->fQuiet) {
      stringstream s;

      s <<
        "gIndenter value after pass 4: "<<
        gIndenter.getIndent ();

      msrMusicXMLWarning (
        gOahOah->fInputSourceName,
        1, // JMI inputLineNumber,
        s.str ());
    }

    gIndenter.resetToZero ();
  }
}


}
