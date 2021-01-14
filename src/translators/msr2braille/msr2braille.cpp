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
#include "msr2bsrOah.h"
#include "bsrOah.h"

#include "msr2brailleInsiderHandler.h"
#include "msr2brailleRegularHandler.h"

#include "msr2mxmlTreeInterface.h"
#include "mxmlTree2msrSkeletonBuilderInterface.h"
#include "mxmlTree2msrTranslatorInterface.h"
#include "msr2mxmlTreeInterface.h"
#include "msr2bsrInterface.h"
#include "bsr2bsrFinalizerInterface.h"
#include "bsr2brailleTranslatorInterface.h"

#include "msr2braille.h"


using namespace std;

namespace MusicXML2
{
/*
  ENFORCE_TRACE_OAH can be used to issue trace messages
  before gGlobalOahOahGroup->fTrace has been initialized
*/

//#define ENFORCE_TRACE_OAH

//_______________________________________________________________________________
EXP xmlErr msrScore2brailleWithHandler (
  S_msrScore   theMsrScore,
  string       passNumber1a,
  string       passDescription1a,
  string       passNumber1b,
  string       passDescription1b,
  string       passNumber2,
  string       passDescription2,
  ostream&     out,
  ostream&     err,
  S_oahHandler handler)
{
  // has quiet mode been requested?
  // ------------------------------------------------------

  if (gGlobalGeneralOahGroup->getQuiet ()) {
    // disable all trace and display options
    handler->
      enforceHandlerQuietness ();
  }

  // the first BSR score
  S_bsrScore firstBsrScore;

  {
    // create the first BSR from the MSR
    // ------------------------------------------------------

    try {
      firstBsrScore =
        convertMsrScoreToBsrScore (
          theMsrScore,
          gGlobalMsrOahGroup,
          gGlobalBsrOahGroup,
          passNumber1a,
          passDescription1a);
    }
    catch (msrScoreToBsrScoreException& e) {
      displayException (e, gOutputStream);
      return kInvalidFile;
    }
    catch (std::exception& e) {
      displayException (e, gOutputStream);
      return kInvalidFile;
    }

    // display the first BSR score if requested
    // ------------------------------------------------------

    if (gGlobalBsrOahGroup->getDisplayBsr ()) {
      displayBsrFirstScore_OptionalPass (
        firstBsrScore,
        gGlobalMsrOahGroup,
        gGlobalBsrOahGroup);
    }

    if (gGlobalBsrOahGroup->getDisplayBsrShort ()) {
      displayBsrFirstScoreShort_OptionalPass (
        firstBsrScore,
        gGlobalMsrOahGroup,
        gGlobalBsrOahGroup);
    }

    // should we return now?
    // ------------------------------------------------------

    if (gGlobalMsr2bsrOahGroup->getQuitAfterPass3a ()) {
      err <<
        endl <<
        "Quitting after pass 3a as requested" <<
        endl;

      return kNoErr;
    }
  }

  // the finalized BSR score
  S_bsrScore finalizedBsrScore;

  {
    // create the finalized BSR from the first BSR
    // ------------------------------------------------------

    try {
      finalizedBsrScore =
        convertBsrFirstScoreToFinalizedBsrScore (
          firstBsrScore,
          gGlobalBsrOahGroup,
          passNumber1b,
          passDescription1b);
    }
    catch (bsrScoreToFinalizedBsrScoreException& e) {
      displayException (e, gOutputStream);
      return kInvalidFile;
    }
    catch (std::exception& e) {
      displayException (e, gOutputStream);
      return kInvalidFile;
    }

    // display the finalized BSR score if requested
    // ------------------------------------------------------

    if (gGlobalBsrOahGroup->getDisplayBsr ()) {
      displayFinalizedBsrScore_OptionalPass (
        finalizedBsrScore,
        gGlobalMsrOahGroup,
        gGlobalBsrOahGroup);
    }

    if (gGlobalBsrOahGroup->getDisplayBsrShort ()) {
      displayFinalizedBsrScoreShort_OptionalPass (
        finalizedBsrScore,
        gGlobalMsrOahGroup,
        gGlobalBsrOahGroup);
    }

    // should we return now?
    // ------------------------------------------------------

    if (gGlobalMsr2bsrOahGroup->getQuitAfterPass3b ()) {
      err <<
        endl <<
        "Quitting after pass 3b as requested" <<
        endl;

      return kNoErr;
    }
  }

  {
    // convert the BSR into Braille music
    // ------------------------------------------------------

    string
      outputFileName =
        handler->
          fetchOutputFileNameFromTheOptions ();

#ifdef TRACING_IS_ENABLED
      if (gGlobalTraceOahGroup->getTraceOah ()) {
        err <<
          "xmlFile2braille() outputFileName = \"" <<
          outputFileName <<
          "\"" <<
          endl;
      }
#endif

    if (! outputFileName.size ()) {
#ifdef TRACING_IS_ENABLED
      if (gGlobalTraceOahGroup->getTraceOah ()) {
        err <<
          "xmlFile2braille() output goes to standard output" <<
          endl;
      }
#endif

      // convert the BSR score to braille text
      try {
        convertBsrScoreToBrailleText (
          finalizedBsrScore,
          gGlobalBsrOahGroup,
          passNumber2,
          passDescription2,
          out);
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
          "xmlFile2braille() output goes to file \"" <<
          outputFileName <<
          "\"" <<
          endl;
      }
#endif

      // open output file
#ifdef TRACING_IS_ENABLED
      if (gGlobalTraceOahGroup->getTracePasses ()) {
        err <<
          "Opening file \"" << outputFileName << "\" for writing" <<
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
          "Could not open Braille music output file \"" <<
          outputFileName <<
          "\" for writing, quitting";

        string message = s.str ();

        err <<
          message <<
          endl;

        throw bsrScoreToBrailleTextException (message);
      }

      // convert the finalized BSR score to braille text
      try {
        convertBsrScoreToBrailleText (
          finalizedBsrScore,
          gGlobalBsrOahGroup,
          passNumber2,
          passDescription2,
          brailleCodeFileOutputStream);
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
        gLogStream <<
          endl <<
          "Closing file \"" << outputFileName << "\"" <<
          endl;
      }
#endif

      brailleCodeFileOutputStream.close ();
    }
  }

	return kNoErr;
}


}
