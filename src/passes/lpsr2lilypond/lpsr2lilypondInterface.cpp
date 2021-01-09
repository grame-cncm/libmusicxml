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

#include "enableTracingIfDesired.h"
#ifdef TRACING_IS_ENABLED
  #include "traceOah.h"
#endif

#include "lpsr2lilypondOah.h"

#include "lpsr2lilypondTranslator.h"

#include "lpsr2lilypondInterface.h"


using namespace std;

namespace MusicXML2
{

//_______________________________________________________________________________
void convertLpsrScoreToLilypondCode (
  const S_lpsrScore theLpsrScore,
  S_msrOahGroup     msrOpts,
  S_lpsrOahGroup    lpsrOpts,
  string            passNumber,
  string            passDescription,
  ostream&          lilypondCodeStream)
{
  // sanity check
  msgAssert (
    theLpsrScore != nullptr,
    "theLpsrScore is null");

  // start the clock
  clock_t startClock = clock ();

  string separator =
    "%--------------------------------------------------------------";

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTracePasses ()) {
    gLogStream <<
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

  // create an lpsr2lilypondTranslator
  lpsr2lilypondTranslator
    translator (
      theLpsrScore,
      msrOpts,
      lpsrOpts,
      lilypondCodeStream);

  // generate the LilyPond code
  translator.generateLilypondCodeFromLpsrScore ();

  // register time spent
  clock_t endClock = clock ();

  timing::gGlobalTiming.appendTimingItem (
    passNumber,
    passDescription,
    timingItem::kMandatory,
    startClock,
    endClock);

  // check indentation
  if (gIndenter != 0) {
    gLogStream <<
      "### convertLpsrScoreToLilypondCode gIndenter final value: " <<
      gIndenter.getIndent () <<
      " ###" <<
      endl;

    gIndenter.resetToZero ();
  }
}

EXP void convertLpsrScoreToLilypondCodeWithHandler (
  const S_lpsrScore theLpsrScore,
  S_msrOahGroup     msrOpts,
  S_lpsrOahGroup    lpsrOpts,
  string            passNumber,
  string            passDescription,
  S_oahHandler      handler,
  ostream&          out,
  ostream&          err)
{
  string
    outputFileName =
      handler->
        fetchOutputFileNameFromTheOptions ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceOah ()) {
    err <<
      "xmlFile2lilypond() outputFileName = \"" <<
      outputFileName <<
      "\"" <<
      endl;
  }
#endif

  if (! outputFileName.size ()) {
#ifdef TRACING_IS_ENABLED
    if (gGlobalTraceOahGroup->getTraceOah ()) {
      err <<
        "xmlFile2lilypond() output goes to standard output" <<
        endl;
    }
#endif

    // create an indented output stream for the LilyPond code
    // to be written to outputFileStream
    indentedOstream
      lilypondStandardOutputStream (
        out,
        gIndenter);

    // convert the LPSR score to LilyPond code
    try {
      convertLpsrScoreToLilypondCode (
        theLpsrScore,
        gGlobalMsrOahGroup,
        gGlobalLpsrOahGroup,
        passNumber,
        passDescription,
        lilypondStandardOutputStream);
    }
    catch (lpsrScoreToLilypondException& e) {
      displayException (e, gOutputStream);
      return;
    }
    catch (std::exception& e) {
      displayException (e, gOutputStream);
      return;
    }
  }

  else {
#ifdef TRACING_IS_ENABLED
    if (gGlobalTraceOahGroup->getTraceOah ()) {
      err <<
        "xmlFile2lilypond() output goes to file \"" <<
        outputFileName <<
        "\"" <<
        endl;
    }
#endif

    // open output file
#ifdef TRACING_IS_ENABLED
    if (gGlobalTraceOahGroup->getTracePasses ()) {
      err <<
        "Opening file '" << outputFileName << "' for writing" <<
        endl;
    }
#endif

    ofstream
      outputFileStream (
        outputFileName.c_str (),
        ofstream::out);

    if (! outputFileStream.is_open ()) {
      stringstream s;

      s <<
        "Could not open LilyPond output file \"" <<
        outputFileName <<
        "\" for writing, quitting";

      string message = s.str ();

      err <<
        message <<
        endl;

      throw lpsrScoreToLilypondException (message);
    }

    // create an indented output stream for the LilyPond code
    // to be written to outputFileStream
    indentedOstream
      lilypondFileOutputStream (
        outputFileStream,
        gIndenter);

    // convert the LPSR score to LilyPond code
    try {
      convertLpsrScoreToLilypondCode (
        theLpsrScore,
        gGlobalMsrOahGroup,
        gGlobalLpsrOahGroup,
        passNumber,
        passDescription,
        lilypondFileOutputStream);
    }
    catch (lpsrScoreToLilypondException& e) {
      displayException (e, gOutputStream);
      return;
    }
    catch (std::exception& e) {
      displayException (e, gOutputStream);
      return;
    }

    // close output file
#ifdef TRACE_OAH
    if (gTraceOah->fTracePasses) {
      gLogOstream <<
        endl <<
        "Closing file \"" << outputFileName << "\"" <<
        endl;
    }
#endif

    outputFileStream.close ();
  }
}


}
