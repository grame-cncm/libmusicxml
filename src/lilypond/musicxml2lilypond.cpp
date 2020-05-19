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

#include "libmusicxml.h"
#include "xml.h"
#include "xmlfile.h"
#include "xmlreader.h"

#include "setTraceOahIfDesired.h"
#ifdef TRACE_OAH
  #include "traceOah.h"
#endif

#include "msr2LpsrOah.h"
#include "xml2lyOah.h"

#include "msr.h"

#include "musicXML2MxmlTreeInterface.h"
#include "mxmlTree2MsrSkeletonBuilderInterface.h"
#include "mxmlTree2MsrTranslatorInterface.h"
#include "msr2MxmltreeInterface.h"
#include "msr2LpsrInterface.h"
#include "lpsr2LilypondInterface.h"

using namespace std;

namespace MusicXML2
{

//_______________________________________________________________________________
static xmlErr xml2lilypond (SXMLFile& xmlfile, const optionsVector& options, ostream& out, const char* file)
{
	Sxmlelement st = xmlfile->elements();

	if (st) {
		if (st->getName() == "score-timewise") return kUnsupported;

/*
		xml2guidovisitor v(true, true, generateBars, partFilter);

		Sguidoelement gmn = v.convert(st);
*/

		if (file) {
			out << "(*\n  gmn code converted from '" << file << "'"
				<< "\n  using libmusicxml v." << musicxmllibVersionStr();
		}
		else out << "(*\n  gmn code converted using libmusicxml v." << musicxmllibVersionStr();
		out << "\n  and the embedded xml2guido converter v." << musicxml2guidoVersionStr()
			<< "\n*)" << endl;

//		out << gmn << endl;

		return kNoErr;
	}

	return kInvalidFile;
}

/*
  // create the options handler
  // ------------------------------------------------------

  S_xml2lyOahHandler
    handler =
      xml2lyOahHandler::create (
        argv [0],
        gOutputOstream);

  // analyze the command line options and arguments
  // ------------------------------------------------------

  vector<string>
    argumentsVector =
      handleOptionsAndArguments (
        handler,
        argc, argv,
        gOutputOstream);
  //      gLogOstream);

  string
    inputSourceName =
      gOahOah->fInputSourceName;

  string
    outputFileName =
      gXml2lyOah->fLilyPondOutputFileName;

  int
    outputFileNameSize =
      outputFileName.size ();

  // has quiet mode been requested?
  // ------------------------------------------------------

  if (gGeneralOah->fQuiet) {
    // disable all trace and display options
    handler->
      enforceOahHandlerQuietness ();
  }

  // welcome message
  // ------------------------------------------------------

#ifdef TRACE_OAH
  if (gTraceOah->fTracePasses) {
    gLogOstream <<
      "This is xml2ly " << currentVersionNumber () <<
      " from libmusicxml2 v" << musicxmllibVersionStr () <<
      endl;

    gLogOstream <<
      "Launching conversion of ";

    if (inputSourceName == "-")
      gLogOstream <<
        "standard input";
    else
      gLogOstream <<
        "\"" << inputSourceName << "\"";

    gLogOstream <<
      " to LilyPond" <<
      endl;

    gLogOstream <<
      "Time is " << gGeneralOah->fTranslationDateFull <<
      endl;

    gLogOstream <<
      "LilyPond code will be written to ";
    if (outputFileNameSize) {
      gLogOstream <<
        outputFileName;
    }
    else {
      gLogOstream <<
        "standard output";
    }
    gLogOstream <<
      endl <<
      endl;

    gLogOstream <<
      "The command line is:" <<
      endl;

    gIndenter++;

    gLogOstream <<
      handler->
        commandLineWithShortNamesAsString () <<
      endl;

    gIndenter--;
    gLogOstream <<
      "or:" <<
      endl;
    gIndenter++;

    gLogOstream <<
      handler->
        commandLineWithLongNamesAsString () <<
      endl <<
      endl;

    gIndenter--;
  }
#endif

  // acknoledge end of command line analysis
  // ------------------------------------------------------

#ifdef TRACE_OAH
  if (gTraceOah->fTracePasses) {
    gLogOstream <<
      "The command line options and arguments have been analyzed" <<
      endl;
  }
#endif

  // do the translation
  // ------------------------------------------------------

  convertMusicXMLToLilypond (
    inputSourceName,
    outputFileName);

  // display the input line numbers for which messages have been issued
  // ------------------------------------------------------

  displayWarningsAndErrorsInputLineNumbers ();

  // print timing information
  // ------------------------------------------------------

  if (gGeneralOah->fDisplayCPUusage)
    timing::gTiming.print (
      gLogOstream);

  // check indentation
  // ------------------------------------------------------
  if (gIndenter != 0) {
    gLogOstream <<
      "### gIndenter final value: "<< gIndenter.getIndent () << " ###" <<
      endl <<
      endl;

    // JMI abort ();
  }

  // over!
  // ------------------------------------------------------

  if (! true) { // JMI
    gLogOstream <<
      "### Conversion from LPSR to LilyPond code failed ###" <<
      endl <<
      endl;

    return 1;
  }
*/

//_______________________________________________________________________________
EXP xmlErr musicxmlfile2lilypond (const char *file, const optionsVector& options, ostream& out)
{
	xmlreader r;
	SXMLFile xmlfile;
	xmlfile = r.read(file);
	if (xmlfile) {
		return xml2lilypond (xmlfile, options, out, file);
	}

	return kInvalidFile;
}

//_______________________________________________________________________________
EXP xmlErr musicxmlfd2lilypond (FILE * fd, const optionsVector& options, ostream& out)
{
	xmlreader r;
	SXMLFile xmlfile;

	xmlfile = r.read(fd);

	if (xmlfile) {
		return xml2lilypond (xmlfile, options, out, 0);
	}

	return kInvalidFile;
}

//_______________________________________________________________________________
EXP xmlErr musicxmlstring2lilypond(const char *buffer, const optionsVector& options, std::ostream& out)
{
	xmlreader r;
	SXMLFile  xmlfile;

	xmlfile = r.readbuff (buffer);

	if (xmlfile) {
		return xml2lilypond (xmlfile, options, out, 0);
	}

	return kInvalidFile;
}

//_______________________________________________________________________________
EXP xmlErr convertMusicXMLToLilypond (
  string inputSourceName,
  string outputFileName,
  bool   loopBackToMusicXML)
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

    return kNoErr;
  }

  // display the score names if requested
  // ------------------------------------------------------

  if (gMsrOah->fDisplayMsrNames) {
    // display the score name
    displayMSRPopulatedScoreNames (
      gMsrOah,
      mScore,
      gLogOstream);

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
    gLogOstream <<
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

  convertLpsrScoreToLilypondCode (
    outputFileName,
    lpScore,
    "Pass 4");

  // create MusicXML back from the MSR if requested
  // ------------------------------------------------------
  if (loopBackToMusicXML) {
    convertMsrScoreToMusicXMLScore_Loop (
      mScore,
      regex_replace (
        outputFileName,
        regex (".ly"),
        "_LOOP.xml"));
  }

  return kNoErr;
}


}
