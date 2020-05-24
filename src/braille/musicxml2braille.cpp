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

#include "xml.h"
#include "xmlfile.h"
#include "xmlreader.h"

#include "setTraceOahIfDesired.h"
#ifdef TRACE_OAH
  #include "traceOah.h"
#endif

#include "msrOah.h"
#include "msr2BsrOah.h"
#include "bsrOah.h"
#include "xml2brlOah.h"

#include "msr.h"

#include "musicXML2MxmlTreeInterface.h"
#include "mxmlTree2MsrSkeletonBuilderInterface.h"
#include "mxmlTree2MsrTranslatorInterface.h"
#include "msr2MxmltreeInterface.h"
#include "msr2BsrInterface.h"
#include "bsr2BsrFinalizerInterface.h"
#include "bsr2BrailleTranslatorInterface.h"

#include "musicxml2braille.h"


using namespace std;

namespace MusicXML2
{

//_______________________________________________________________________________
static xmlErr xml2braille (SXMLFile& xmlfile, const optionsVector& options, std::ostream& out, std::ostream& err, const char* file)
{
	Sxmlelement st = xmlfile->elements();

	if (st) {
		if (st->getName() == "score-timewise") return kUnsupported;

    // the fake executable name
    string fakeExecutableName = "xml2braille";

    // create the options handler
    // ------------------------------------------------------

    S_xml2brlOahHandler
      handler =
        xml2brlOahHandler::create (
          fakeExecutableName,
          out);

    // analyze the coptions vector
    // ------------------------------------------------------

    try {
      handler->
        hangleOptionsFromOptionsVector (
          fakeExecutableName,
          options);
    }
    catch (msrOahException& e) {
      return kInvalidOption;
    }
    catch (std::exception& e) {
      return kInvalidFile;
    }

    // has quiet mode been requested?
    // ------------------------------------------------------

    if (gGeneralOah->fQuiet) {
      // disable all trace and display options
      handler->
        enforceOahHandlerQuietness ();
    }

    // get the mxmlTree
    // ------------------------------------------------------

    Sxmlelement
      mxmlTree =
        xmlfile->elements ();
/* JMI
        convertMusicXMLToMxmlTree (
          inputSourceName,
          "Pass 1");
*/

    // create the MSR skeleton from the mxmlTree (pass 2a)
    // ------------------------------------------------------

    S_msrScore mScore;

    try {
      mScore =
        convertMxmlTreeToMsrScoreSkeleton (
          mxmlTree,
          "Pass 2a");
    }
    catch (mxmlTreeToMsrException& e) {
      return kInvalidFile;
    }
    catch (std::exception& e) {
      return kInvalidFile;
    }

    if (gMsr2LpsrOah->fExit2a) {
      err <<
        endl <<
        "Existing after pass 2a as requested" <<
        endl;

      return kNoErr;
    }

    // populate the MSR from MusicXML contents (pass 2b)
    // ------------------------------------------------------

    try {
      populateMsrSkeletonFromMxmlTree (
        mxmlTree,
        mScore,
        err,
        "Pass 2b");
    }
    catch (mxmlTreeToMsrException& e) {
      return kInvalidFile;
    }
    catch (std::exception& e) {
      return kInvalidFile;
    }

    if (gMsr2LpsrOah->fExit2b) {
      err <<
        endl <<
        "Existing after pass 2b as requested" <<
        endl;

      return kNoErr;
    }

    // display the MSR score summary if requested
    // ------------------------------------------------------

    if (gMsrOah->fDisplayMsr) {
      displayMsrScore_OptionalPass (
        mScore,
        gMsrOah);
    }

    // display the score summary if requested
    // ------------------------------------------------------

    if (gMsrOah->fDisplayMsrSummary) {
      // display the score summary
      displayMSRPopulatedScoreSummary (
        gMsrOah,
        mScore,
        gLogOstream);
    }

    // display the score names if requested
    // ------------------------------------------------------

    if (gMsrOah->fDisplayMsrNames) {
      // display the score name
      displayMSRPopulatedScoreNames (
        gMsrOah,
        mScore,
        gLogOstream);
    }

    // create the BSR from the MSR (pass 3a)
    // ------------------------------------------------------

    S_bsrScore firstBsrScore;

    try {
      firstBsrScore =
        convertMsrScoreToBsrScore (
          mScore,
          "Pass 3a");
    }
    catch (msrScoreToBsrScoreException& e) {
      return kInvalidFile;
    }
    catch (std::exception& e) {
      return kInvalidFile;
    }

    if (gMsr2BsrOah->fExit3a) {
      err <<
        endl <<
        "Existing after pass 3a as requested" <<
        endl;

      return kNoErr;
    }

    // display the first BSR score if requested
    // ------------------------------------------------------

    if (gBsrOah->fDisplayBsr) {
      displayBsrFirstScore_OptionalPass (
        firstBsrScore,
        gMsrOah,
        gBsrOah);
    }

    // create the finalized BSR from the first BSR (pass 3b)
    // ------------------------------------------------------

    S_bsrScore finalizedBsrScore;

    try {
      finalizedBsrScore =
        convertBsrScoreToFinalizedBsrScore (
          firstBsrScore,
          "Pass 3b");
    }
    catch (bsrScoreToFinalizedBsrScoreException& e) {
      return kInvalidFile;
    }
    catch (std::exception& e) {
      return kInvalidFile;
    }

    if (gMsr2BsrOah->fExit3b) {
      err <<
        endl <<
        "Existing after pass 3b as requested" <<
        endl;

      return kNoErr;
    }

    // display the finalized BSR score if requested
    // ------------------------------------------------------

    if (gBsrOah->fDisplayBsr) {
      displayFinalizedBsrScore_OptionalPass (
        finalizedBsrScore,
        gMsrOah,
        gBsrOah);
    }

    // generate Braille music text from the BSR (pass 4)
    // ------------------------------------------------------

    try {
      convertBsrScoreToBrailleText (
        file,
        finalizedBsrScore,
        "Pass 4");
    }
    catch (bsrScoreToBrailleTextException& e) {
      return kInvalidFile;
    }
    catch (std::exception& e) {
      return kInvalidFile;
    }

		return kNoErr;
	}

	return kInvalidFile;
}

//_______________________________________________________________________________
EXP xmlErr musicxmlfile2braille (const char *file, const optionsVector& options, ostream&                out, std::ostream& err)
{
	xmlreader r;
	SXMLFile xmlfile;

	xmlfile = r.read(file);

	if (xmlfile) {
		return xml2braille (xmlfile, options, out, err, file);
	}

	return kInvalidFile;
}

//_______________________________________________________________________________
EXP xmlErr musicxmlfd2braille (FILE *fd, const optionsVector& options, std::ostream& out, std::ostream& err)
{
	xmlreader r;
	SXMLFile xmlfile;

	xmlfile = r.read(fd);

	if (xmlfile) {
		return xml2braille (xmlfile, options, out, err, 0);
	}

	return kInvalidFile;
}

//_______________________________________________________________________________
EXP xmlErr musicxmlstring2braille (const char *buffer, const optionsVector& options, std::ostream&           out, std::ostream& err)
{
	xmlreader r;
	SXMLFile xmlfile;

	xmlfile = r.readbuff(buffer);

	if (xmlfile) {
		return xml2braille (xmlfile, options, out, err, 0);
	}

	return kInvalidFile;
}

//_______________________________________________________________________________
EXP xmlErr convertMusicXMLToBraille (
  std::string inputSourceName,
  std::string outputFileName)
{
  // create the mxmlTree from MusicXML contents (pass 1)
  // ------------------------------------------------------

  Sxmlelement
    mxmlTree =
      convertMusicXMLToMxmlTree (
        inputSourceName,
        "Pass 1");

  // create the MSR skeleton from the mxmlTree (pass 2a)
  // ------------------------------------------------------

  S_msrScore
    mScore =
      convertMxmlTreeToMsrScoreSkeleton (
        mxmlTree,
        "Pass 2a");

  if (gMsr2LpsrOah->fExit2a) {
    gLogOstream <<
      endl <<
      "Existing after pass 2a as requested" <<
      endl;

    return kNoErr;
  }

  // populate the MSR from MusicXML contents (pass 2b)
  // ------------------------------------------------------

  populateMsrSkeletonFromMxmlTree (
    mxmlTree,
    mScore,
    gLogOstream,
    "Pass 2b");

  if (gMsr2LpsrOah->fExit2b) {
    gLogOstream <<
      endl <<
      "Existing after pass 2b as requested" <<
      endl;

    return kNoErr;
  }

  // display the MSR score summary if requested
  // ------------------------------------------------------

  if (gMsrOah->fDisplayMsr) {
    displayMsrScore_OptionalPass (
      mScore,
      gMsrOah);
  }

  // display the score summary if requested
  // ------------------------------------------------------

  if (gMsrOah->fDisplayMsrSummary) {
    // display the score summary
    displayMSRPopulatedScoreSummary (
      gMsrOah,
      mScore,
      gLogOstream);
  }

  // display the score names if requested
  // ------------------------------------------------------

  if (gMsrOah->fDisplayMsrNames) {
    // display the score name
    displayMSRPopulatedScoreNames (
      gMsrOah,
      mScore,
      gLogOstream);
  }

  // create the BSR from the MSR (pass 3a)
  // ------------------------------------------------------

  S_bsrScore
    firstBsrScore =
      convertMsrScoreToBsrScore (
        mScore,
        "Pass 3a");

  if (gMsr2BsrOah->fExit3a) {
    gLogOstream <<
      endl <<
      "Existing after pass 3a as requested" <<
      endl;

    return kNoErr;
  }

  // display the first BSR score if requested
  // ------------------------------------------------------

  if (gBsrOah->fDisplayBsr) {
    displayBsrFirstScore_OptionalPass (
      firstBsrScore,
      gMsrOah,
      gBsrOah);
  }

  // create the finalized BSR from the first BSR (pass 3b)
  // ------------------------------------------------------

  S_bsrScore
    finalizedBsrScore =
      convertBsrScoreToFinalizedBsrScore (
        firstBsrScore,
        "Pass 3b");

  if (gMsr2BsrOah->fExit3b) {
    gLogOstream <<
      endl <<
      "Existing after pass 3b as requested" <<
      endl;

    return kNoErr;
  }

  // display the finalized BSR score if requested
  // ------------------------------------------------------

  if (gBsrOah->fDisplayBsr) {
    displayFinalizedBsrScore_OptionalPass (
      finalizedBsrScore,
      gMsrOah,
      gBsrOah);
  }

  // generate Braille music text from the BSR (pass 4)
  // ------------------------------------------------------

  convertBsrScoreToBrailleText (
    outputFileName,
    finalizedBsrScore,
    "Pass 4");

  return kNoErr;
}


}
