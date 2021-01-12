/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifdef MSVC
# pragma warning (disable : 4786)
#endif

#include <iostream>
#include <fstream>      // ofstream, ofstream::open(), ofstream::close()

#include "xml.h"
#include "xmlfile.h"
#include "xmlreader.h"

#include "enableTracingIfDesired.h"
#ifdef TRACING_IS_ENABLED
  #include "traceOah.h"
#endif

#include "msrOah.h"
#include "msr2lpsrOah.h"
#include "lpsrOah.h"

#include "msr2lilypondInsiderHandler.h"
#include "msr2lilypondRegularHandler.h"

#include "msr2lpsrInterface.h"
#include "lpsr2lilypondInterface.h"

#include "msr2lilypond.h"

using namespace std;

namespace MusicXML2
{
/*
  ENFORCE_TRACE_OAH can be used to issue trace messages
  before gGlobalOahOahGroup->fTrace has been initialized
*/
//#define ENFORCE_TRACE_OAH

//_______________________________________________________________________________
EXP xmlErr msrScore2lilypondWithHandler (
  S_msrScore   theMsrScore,
  string       passNumber1,
  string       passDescription1,
  string       passNumber2,
  string       passDescription2,
  ostream&     out,
  ostream&     err,
  S_oahHandler handler)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceOah ()) {
    gLogStream <<
      "Translating an MSR score to LilyPond in \"" <<
      handler->getHandlerHeader () <<
      "\"" <<
      endl;
  }
#endif

  // has quiet mode been requested?
  // ------------------------------------------------------

  if (gGlobalGeneralOahGroup->getQuiet ()) {
    // disable all trace and display options
    handler->
      enforceHandlerQuietness ();
  }

  // the LPSR score
  S_lpsrScore theLpsrScore;

  {
    // convert the MSR into an LPSR
    // ------------------------------------------------------

    try {
      theLpsrScore =
        convertMsrScoreToLpsrScore (
          theMsrScore,
          gGlobalMsrOahGroup,
          gGlobalLpsrOahGroup,
          passNumber1,
          passDescription1);
    }
    catch (msrScoreToLpsrScoreException& e) {
      displayException (e, gOutputStream);
      return kInvalidFile;
    }
    catch (std::exception& e) {
      displayException (e, gOutputStream);
      return kInvalidFile;
    }

    // display the LPSR score if requested
    // ------------------------------------------------------

    if (gGlobalLpsrOahGroup->getDisplayLpsr ()) {
      displayLpsrScore_OptionalPass (
        theLpsrScore,
        gGlobalMsrOahGroup,
        gGlobalLpsrOahGroup);
    }

    if (gGlobalLpsrOahGroup->getDisplayLpsrShort ()) {
      displayLpsrScoreShort_OptionalPass (
        theLpsrScore,
        gGlobalMsrOahGroup,
        gGlobalLpsrOahGroup);
    }

    // should we return now?
    // ------------------------------------------------------

    if (gGlobalLpsrOahGroup->getQuitAfterPass3 ()) {
      err <<
        endl <<
        "Quitting after pass 3 as requested" <<
        endl;

      return kNoErr;
    }
  }

  {
    // convert the LPSR into LilyPond code
    // ------------------------------------------------------

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
          passNumber2,
          passDescription2,
          lilypondStandardOutputStream);
      }
      catch (lpsrScoreToLilypondException& e) {
        displayException (e, gOutputStream);
        return kInvalidFile;
      }
      catch (std::exception& e) {
        displayException (e, gOutputStream);
        return kInvalidFile;
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
          passNumber2,
          passDescription2,
          lilypondFileOutputStream);
      }
      catch (lpsrScoreToLilypondException& e) {
        displayException (e, gOutputStream);
        return kInvalidFile;
      }
      catch (std::exception& e) {
        displayException (e, gOutputStream);
        return kInvalidFile;
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

	return kNoErr;
}


}
