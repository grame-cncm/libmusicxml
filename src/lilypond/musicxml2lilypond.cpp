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

//#include "oahBasicTypes.h"
#include "msrOah.h"
#include "msr2LpsrOah.h"
#include "lpsrOah.h"

#include "xml2lyInsiderOahHandler.h"
#include "xml2lyRegularOahHandler.h"

#include "msr.h"

#include "musicXML2MxmlTreeInterface.h"
#include "mxmlTree2MsrSkeletonBuilderInterface.h"
#include "mxmlTree2MsrTranslatorInterface.h"
#include "msr2MxmlTreeInterface.h"
#include "msr2LpsrInterface.h"
#include "lpsr2LilypondInterface.h"

#include "musicxml2lilypond.h"
#include "musicxml2musicxml.h"


using namespace std;

namespace MusicXML2
{

//_______________________________________________________________________________
static xmlErr xml2lilypond (SXMLFile& xmlfile, bool insiderOptions, const optionsVector& options, std::ostream& out, std::ostream& err, const char* file)
{
	Sxmlelement st;

	if (xmlfile) {
	  st = xmlfile->elements();

    if (st) {
      if (st->getName() == "score-timewise") return kUnsupported;
    }
  }

  // the fake executable name
  // ------------------------------------------------------

  string fakeExecutableName = "xml2ly";

  // the indented output streams
  // ------------------------------------------------------

  indentedOstream outIndentedOstream (out, indenter::gIndenter);
  indentedOstream errIndentedOstream (err, indenter::gIndenter);

  // the OAH handler
  // ------------------------------------------------------

  S_oahHandler handler;

  if (insiderOptions) {
    // create an insider xml2ly OAH handler
    handler =
      xml2lyInsiderOahHandler::create (
        fakeExecutableName,
        "xml2ly with insider options",
        outIndentedOstream);
  }
  else {
    // create a regular xml2ly OAH handler
    handler =
      xml2lyRegularOahHandler::create (
        fakeExecutableName,
        "xml2ly with regular options",
        outIndentedOstream);
  }

  // analyze the options vector
  // ------------------------------------------------------

  try {
    oahHandler::oahHelpOptionsHaveBeenUsedKind
      helpOptionsHaveBeenUsedKind =
        handler->
          applyOptionsFromOptionsVector (
            fakeExecutableName,
            options);

    switch (helpOptionsHaveBeenUsedKind) {
      case oahHandler::kHelpOptionsHaveBeenUsedYes:
        return kNoErr;
        break;
      case oahHandler::kHelpOptionsHaveBeenUsedNo:
        // let's go ahead!
        break;
    } // switch
  }
  catch (msrOahException& e) {
    return kInvalidOption;
  }
  catch (std::exception& e) {
    return kInvalidFile;
  }

	if (xmlfile) {
    // has quiet mode been requested?
    // ------------------------------------------------------

    if (gGlobalGeneralOahGroup->fQuiet) {
      // disable all trace and display options
      handler->
        enforceHandlerQuietness ();
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

    // should we return now?
    // ------------------------------------------------------

    if (gGlobalXml2lyInsiderOahGroup->fExit2a) {
      errIndentedOstream <<
        endl <<
        "Exiting after pass 2a as requested" <<
        endl;

      return kNoErr;
    }

    // populate the MSR from MusicXML contents (pass 2b)
    // ------------------------------------------------------

    try {
      populateMsrSkeletonFromMxmlTree (
        mxmlTree,
        mScore,
        errIndentedOstream,
        "Pass 2b");
    }
    catch (mxmlTreeToMsrException& e) {
      return kInvalidFile;
    }
    catch (std::exception& e) {
      return kInvalidFile;
    }

    // display the MSR score if requested
    // ------------------------------------------------------

    if (gGlobalMsrOah->fDisplayMsr) {
      displayMsrScore_OptionalPass (
        mScore,
        gGlobalMsrOah);
    }

    if (gGlobalMsrOah->fDisplayMsrShort) {
      displayMsrScoreShort_OptionalPass (
        mScore,
        gGlobalMsrOah);
    }

    // display the MSR score summary if requested
    // ------------------------------------------------------

    if (gGlobalMsrOah->fDisplayMsrSummary) {
      // display the score summary
      displayMsrPopulatedScoreSummary (
        gGlobalMsrOah,
        mScore,
        errIndentedOstream);

      return kNoErr;
    }

    // display the score names if requested
    // ------------------------------------------------------

    if (gGlobalMsrOah->fDisplayMsrNames) {
      // display the score name
      displayMsrPopulatedScoreNames (
        gGlobalMsrOah,
        mScore,
        errIndentedOstream);

      return kNoErr;
    }

    // should we return now?
    // ------------------------------------------------------

    if (gGlobalXml2lyInsiderOahGroup->fExit2b) {
      errIndentedOstream <<
        endl <<
        "Exiting after pass 2b as requested" <<
        endl;

      return kNoErr;
    }

    // create the LPSR from the MSR (pass 3)
    // ------------------------------------------------------

    S_lpsrScore lpScore;

    try {
      lpScore =
        convertMsrScoreToLpsrScore (
          mScore,
          "Pass 3");
    }
    catch (msrScoreToLpsrScoreException& e) {
      return kInvalidFile;
    }
    catch (std::exception& e) {
      return kInvalidFile;
    }

    // display the LPSR score if requested
    // ------------------------------------------------------

    if (gGlobalLpsrOah->fDisplayLpsr) {
      displayLpsrScore_OptionalPass (
        lpScore,
        gGlobalMsrOah,
        gGlobalLpsrOah);
    }

    if (gGlobalLpsrOah->fDisplayLpsrShort) {
      displayLpsrScoreShort_OptionalPass (
        lpScore,
        gGlobalMsrOah,
        gGlobalLpsrOah);
    }

    // should we return now?
    // ------------------------------------------------------

    if (gGlobalLpsrOah->fExit3) {
      errIndentedOstream <<
        endl <<
        "Exiting after pass 3 as requested" <<
        endl;

      return kNoErr;
    }

    // generate LilyPond code from the LPSR (pass 4)
    // ------------------------------------------------------

    try {
      generateLilypondCodeFromLpsrScore (
        lpScore,
        gGlobalMsrOah,
        gGlobalLpsrOah,
        errIndentedOstream,
        outIndentedOstream,
        "Pass 4");
    }
    catch (lpsrScoreToLilypondException& e) {
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
EXP xmlErr musicxmlfile2lilypond (const char *file, bool insiderOptions, const optionsVector& options, std::ostream& out, std::ostream& err)
{
	xmlreader r;
	SXMLFile xmlfile;

	xmlfile = r.read(file);

	if (xmlfile) {
		return xml2lilypond (xmlfile, insiderOptions, options, out, err, file);
	}

	return kInvalidFile;
}

//_______________________________________________________________________________
EXP xmlErr musicxmlfd2lilypond (FILE * fd, bool insiderOptions, const optionsVector& options, std::ostream& out, std::ostream& err)
{
	xmlreader r;
	SXMLFile xmlfile;

	xmlfile = r.read(fd);

	if (xmlfile) {
		return xml2lilypond (xmlfile, insiderOptions, options, out, err, 0);
	}

	return kInvalidFile;
}

//_______________________________________________________________________________
EXP xmlErr musicxmlstring2lilypond (const char *buffer, bool insiderOptions, const optionsVector& options, std::ostream& out, std::ostream& err)
{
	SXMLFile  xmlfile;

  if (buffer [0] != '\0') {
  	xmlreader r;

	  xmlfile = r.readbuff (buffer);
  }

	// call xml2lilypond() even if xmlfile is null,
	// to handle the help options if any
  return xml2lilypond (xmlfile, insiderOptions, options, out, err, 0);

	return kInvalidFile;
}

//_______________________________________________________________________________
EXP xmlErr convertMusicXMLToLilypond (
  string inputSourceName,
  string outputFileName,
  bool   loopBackToMusicXML) // loopBackToMusicXML is used by 'xml2ly -loop'
{
  int saveIndent = 0;

  // create the mxmlTree from MusicXML contents (pass 1)
  // ------------------------------------------------------

  saveIndent = gIndenter.getIndent ();

  Sxmlelement
    mxmlTree =
      convertMusicXMLToMxmlTree (
        inputSourceName,
        "Pass 1");

  if (gIndenter != saveIndent) {
    gLogOstream <<
      "### gIndenter final value has changed after convertMxmlTreeToMsrScoreSkeleton(): "<<
      gIndenter.getIndent () <<
      " ###" <<
      endl << endl;
  }

  // create the MSR skeleton from the mxmlTree (pass 2a)
  // ------------------------------------------------------

  saveIndent = gIndenter.getIndent ();

  S_msrScore
    mScore =
      convertMxmlTreeToMsrScoreSkeleton (
        mxmlTree,
        "Pass 2a");

  if (gIndenter != saveIndent) {
    gLogOstream <<
      "### gIndenter final value has changed after convertMxmlTreeToMsrScoreSkeleton(): "<<
      gIndenter.getIndent () <<
      " ###" <<
      endl << endl;
  }

  // should we return now?
  // ------------------------------------------------------

  if (gGlobalXml2lyInsiderOahGroup->fExit2a) {
    gLogOstream <<
      endl <<
      "Exiting after pass 2a as requested" <<
      endl;

    return kNoErr;
  }

  // populate the MSR from MusicXML contents (pass 2b)
  // ------------------------------------------------------

  saveIndent = gIndenter.getIndent ();

  try {
    populateMsrSkeletonFromMxmlTree (
      mxmlTree,
      mScore,
      gLogOstream,
      "Pass 2b");
  }
  catch (mxmlTreeToMsrException& e) {
    return kInvalidFile;
  }
  catch (std::exception& e) {
    return kInvalidFile;
  }

  if (gIndenter != saveIndent) {
    gLogOstream <<
      "### gIndenter final value has changed after populateMsrSkeletonFromMxmlTree(): "<<
      gIndenter.getIndent () <<
      " ###" <<
      endl << endl;
  }

  // should we return now?
  // ------------------------------------------------------

  if (gGlobalXml2lyInsiderOahGroup->fExit2b) {
    gLogOstream <<
      endl <<
      "Exiting after pass 2b as requested" <<
      endl;

    return kNoErr;
  }

  // display the MSR score if requested
  // ------------------------------------------------------

  if (gGlobalMsrOah->fDisplayMsr) {
    displayMsrScore_OptionalPass (
      mScore,
      gGlobalMsrOah);
  }

  if (gGlobalMsrOah->fDisplayMsrShort) {
    displayMsrScoreShort_OptionalPass (
      mScore,
      gGlobalMsrOah);
  }

  // display the populated MSR score summary if requested
  // ------------------------------------------------------

  if (gGlobalMsrOah->fDisplayMsrSummary) {
    // display the score summary
    displayMsrPopulatedScoreSummary (
      gGlobalMsrOah,
      mScore,
      gLogOstream);

    return kNoErr;
  }

  // display the populated MSR score names if requested
  // ------------------------------------------------------

  if (gGlobalMsrOah->fDisplayMsrNames) {
    // display the score name
    displayMsrPopulatedScoreNames (
      gGlobalMsrOah,
      mScore,
      gLogOstream);

    return kNoErr;
  }

  // create the LPSR from the MSR (pass 3)
  // ------------------------------------------------------

  saveIndent = gIndenter.getIndent ();

  S_lpsrScore lpScore;

  try {
    lpScore =
      convertMsrScoreToLpsrScore (
        mScore,
        "Pass 3");
  }
  catch (msrScoreToLpsrScoreException& e) {
    return kInvalidFile;
  }
  catch (std::exception& e) {
    return kInvalidFile;
  }

  if (gIndenter != saveIndent) {
    gLogOstream <<
      "### gIndenter final value has changed after convertMsrScoreToLpsrScore(): "<<
      gIndenter.getIndent () <<
      " ###" <<
      endl << endl;
  }

  // should we return now?
  // ------------------------------------------------------

  if (gGlobalLpsrOah->fExit3) {
    gLogOstream <<
      endl <<
      "Exiting after pass 3 as requested" <<
      endl;

    return kNoErr;
  }

  // display the LPSR score if requested
  // ------------------------------------------------------

  if (gGlobalLpsrOah->fDisplayLpsr) {
    displayLpsrScore_OptionalPass (
      lpScore,
      gGlobalMsrOah,
      gGlobalLpsrOah);
  }

  if (gGlobalLpsrOah->fDisplayLpsrShort) {
    displayLpsrScoreShort_OptionalPass (
      lpScore,
      gGlobalMsrOah,
      gGlobalLpsrOah);
    }

  // generate LilyPond code from the LPSR (pass 4)
  // ------------------------------------------------------

  saveIndent = gIndenter.getIndent ();

  convertLpsrScoreToLilypondCode (
    outputFileName,
    lpScore,
    "Pass 4");

  if (gIndenter != saveIndent) {
    gLogOstream <<
      "### gIndenter final value has changed after convertLpsrScoreToLilypondCode(): "<< gIndenter.getIndent () << " ###" <<
      endl << endl;
  }

  // create MusicXML back from the MSR if requested
  // ------------------------------------------------------
  if (loopBackToMusicXML) {
    saveIndent = gIndenter.getIndent ();

    convertMsrScoreToMusicXMLScore (
      mScore,
      regex_replace (
        outputFileName,
        regex (".ly"),
        "_LOOP.xml"),
      "Pass 5",
      timingItem::kOptional);

    if (gIndenter != saveIndent) {
      gLogOstream <<
        "### gIndenter final value has changed after convertMusicXMLToLilypond(): "<<
        gIndenter.getIndent () <<
        " ###" <<
        endl <<
        endl;
    }
  }

  return kNoErr;
}


}
