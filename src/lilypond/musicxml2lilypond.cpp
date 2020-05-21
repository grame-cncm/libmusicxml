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
#include <regex>

#include "xml.h"
#include "xmlfile.h"
#include "xmlreader.h"
#include "version.h"

#include "setTraceOahIfDesired.h"
#ifdef TRACE_OAH
  #include "traceOah.h"
#endif

#include "msrOah.h"
#include "msr2LpsrOah.h"
#include "lpsrOah.h"
#include "xml2lyOah.h"

#include "msr.h"

#include "musicXML2MxmlTreeInterface.h"
#include "mxmlTree2MsrSkeletonBuilderInterface.h"
#include "mxmlTree2MsrTranslatorInterface.h"
#include "msr2MxmltreeInterface.h"
#include "msr2LpsrInterface.h"
#include "lpsr2LilypondInterface.h"

#include "musicxml2lilypond.h"
#include "musicxml2musicxml.h"


using namespace std;

namespace MusicXML2
{

//_______________________________________________________________________________
static xmlErr xml2lilypond (SXMLFile& xmlfile, const optionsVector& options, std::ostream& out, std::ostream& err, const char* file)
{
	Sxmlelement st = xmlfile->elements();

	if (st) {
		if (st->getName() == "score-timewise") return kUnsupported;

    // the fake executable name
    string fakeExecutableName = "xml2lilypond";

    // create the options handler
    // ------------------------------------------------------

    S_xml2lyOahHandler
      handler =
        xml2lyOahHandler::create (
          fakeExecutableName,
          out);

    // analyze the coptions vector
    // ------------------------------------------------------

    vector<string>
      argumentsVector =
        handler->
          hangleOptionsFromOptionsVector (
            fakeExecutableName,
            options);

    // has quiet mode been requested?
    // ------------------------------------------------------

    if (gGeneralOah->fQuiet) {
      // disable all trace and display options
      handler->
        enforceOahHandlerQuietness ();
    }

    // do the translation
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

    S_msrScore
      mScore =
        convertMxmlTreeToMsrScoreSkeleton (
          mxmlTree,
          "Pass 2a");

    if (gMsr2LpsrOah->fExit2a) {
      err <<
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
      err,
      "Pass 2b");

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
        err);

      return kNoErr;
    }

    // display the score names if requested
    // ------------------------------------------------------

    if (gMsrOah->fDisplayMsrNames) {
      // display the score name
      displayMSRPopulatedScoreNames (
        gMsrOah,
        mScore,
        err);

      return kNoErr;
    }

    // create the LPSR from the MSR (pass 3)
    // ------------------------------------------------------

    S_lpsrScore
      lpScore =
        convertMsrScoreToLpsrScore (
          mScore,
          "Pass 3");

    if (gLpsrOah->fExit3) {
      err <<
        endl <<
        "Existing after pass 3 as requested" <<
        endl;

      return kNoErr;
    }

    // display the LPSR score if requested
    // ------------------------------------------------------

    if (gLpsrOah->fDisplayLpsr) {
      displayLpsrScore_OptionalPass (
        lpScore,
        gMsrOah,
        gLpsrOah);
    }

    // generate LilyPond code from the LPSR (pass 4)
    // ------------------------------------------------------

    generateLilypondCodeFromLpsrScore (
      lpScore,
      gMsrOah,
      gLpsrOah,
      err,
      out,
      "Pass 4");

    // over!
    // ------------------------------------------------------

    if (! true) { // JMI
      err <<
        "### Conversion from LPSR to LilyPond code failed ###" <<
        endl <<
        endl;

      return kInvalidFile;
    }

		return kNoErr;
	}

	return kInvalidFile;
}

//_______________________________________________________________________________
EXP xmlErr musicxmlfile2lilypond (const char *file, const optionsVector& options, std::ostream& out, std::ostream& err)
{
	xmlreader r;
	SXMLFile xmlfile;

	xmlfile = r.read(file);

	if (xmlfile) {
		return xml2lilypond (xmlfile, options, out, err, file);
	}

	return kInvalidFile;
}

//_______________________________________________________________________________
EXP xmlErr musicxmlfd2lilypond (FILE * fd, const optionsVector& options, std::ostream& out, std::ostream& err)
{
	xmlreader r;
	SXMLFile xmlfile;

	xmlfile = r.read(fd);

	if (xmlfile) {
		return xml2lilypond (xmlfile, options, out, err, 0);
	}

	return kInvalidFile;
}

//_______________________________________________________________________________
EXP xmlErr musicxmlstring2lilypond(const char *buffer, const optionsVector& options, std::ostream& out, std::ostream& err)
{
	xmlreader r;
	SXMLFile  xmlfile;

	xmlfile = r.readbuff (buffer);

	if (xmlfile) {
		return xml2lilypond (xmlfile, options, out, err, 0);
	}

	return kInvalidFile;
}

//_______________________________________________________________________________
EXP void convertMusicXMLToLilypond (
  string inputSourceName,
  string outputFileName,
  bool   loopBackToMusicXML) // loopBackToMusicXML is used by 'xml2ly -loop'
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

    return;
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

    return;
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

    return;
  }

  // display the score names if requested
  // ------------------------------------------------------

  if (gMsrOah->fDisplayMsrNames) {
    // display the score name
    displayMSRPopulatedScoreNames (
      gMsrOah,
      mScore,
      gLogOstream);

    return;
  }

  // create the LPSR from the MSR (pass 3)
  // ------------------------------------------------------

  S_lpsrScore
    lpScore =
      convertMsrScoreToLpsrScore (
        mScore,
        "Pass 3");

  if (gLpsrOah->fExit3) {
    gLogOstream <<
      endl <<
      "Existing after pass 3 as requested" <<
      endl;

    return;
  }

  // display the LPSR score if requested
  // ------------------------------------------------------

  if (gLpsrOah->fDisplayLpsr) {
    displayLpsrScore_OptionalPass (
      lpScore,
      gMsrOah,
      gLpsrOah);
  }

  // generate LilyPond code from the LPSR (pass 4)
  // ------------------------------------------------------

  convertLpsrScoreToLilypondCode (
    outputFileName,
    lpScore,
    "Pass 4");

  // create MusicXML back from the MSR if requested
  // ------------------------------------------------------
  if (loopBackToMusicXML) {
    convertMsrScoreToMusicXMLScore (
      mScore,
      regex_replace (
        outputFileName,
        regex (".ly"),
        "_LOOP.xml"),
      "Pass 5");
  }
}


}
