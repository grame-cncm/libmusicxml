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

#include <regex>

#include "xml.h"
#include "xmlfile.h"
#include "xmlreader.h"
#include "libmusicxml.h"

#include "utilities.h"

#include "setTraceOahIfDesired.h"
#ifdef TRACE_OAH
  #include "traceOah.h"
#endif

#include "msrOah.h"
#include "msr2LpsrOah.h"
#include "musicxmlOah.h"
#include "xml2xmlOah.h"

#include "msr.h"

#include "musicXML2MxmlTreeInterface.h"
#include "mxmlTree2MsrSkeletonBuilderInterface.h"
#include "mxmlTree2MsrTranslatorInterface.h"
#include "msr2MxmltreeInterface.h"

#include "mxmlTree.h"

#include "musicxml2musicxml.h"


using namespace std;

namespace MusicXML2
{

//_______________________________________________________________________________
static xmlErr xml2musicxml (SXMLFile& xmlfile, const optionsVector& options, std::ostream& out, std::ostream& err, const char* file)
{
	Sxmlelement st = xmlfile->elements();

	if (st) {
		if (st->getName() == "score-timewise") return kUnsupported;

    // the fake executable name
    string fakeExecutableName = "xml2musicxml";

    // create the options handler
    // ------------------------------------------------------

    S_xml2xmlOahHandler
      handler =
        xml2xmlOahHandler::create (
          fakeExecutableName,
          out);

    // analyze the coptions vector
    // ------------------------------------------------------

    try {
      oahHandler::oahHelpOptionsHaveBeenUsedKind
        helpOptionsHaveBeenUsedKind =
          handler->
            hangleOptionsFromOptionsVector (
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


    if (gXml2xmlOah->fExit2a) {
      gLogOstream <<
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

    if (gXml2xmlOah->fExit2b) {
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

    // create MusicXML back from the MSR
    // ------------------------------------------------------
    try {
      convertMsrScoreToMusicXMLScore (
        mScore,
        regex_replace (
          file,
          regex (".ly"),
          "_LOOP.xml"),
          "Pass 3");
    }
    catch (msrScoreToMusicXMLScoreException& e) {
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
EXP xmlErr musicxmlfile2musicxml (const char *file, const optionsVector& options, std::ostream& out, std::ostream& err)
{
	xmlreader r;
	SXMLFile xmlfile;

	xmlfile = r.read(file);

	if (xmlfile) {
		return xml2musicxml(xmlfile, options, out, err, file);
	}

	return kInvalidFile;
}

//_______________________________________________________________________________
EXP xmlErr musicxmlfd2musicxml (FILE * fd, const optionsVector& options, std::ostream& out, std::ostream& err)
{
	xmlreader r;
	SXMLFile xmlfile;

	xmlfile = r.read(fd);

	if (xmlfile) {
		return xml2musicxml(xmlfile, options, out, err, 0);
	}

	return kInvalidFile;
}

//_______________________________________________________________________________
EXP xmlErr musicxmlstring2musicxml (const char * buffer, const optionsVector& options, std::ostream& out, std::ostream& err)
{
	xmlreader r;
	SXMLFile xmlfile;

	xmlfile = r.readbuff(buffer);

	if (xmlfile) {
		return xml2musicxml(xmlfile, options, out, err, 0);
	}

	return kInvalidFile;
}

//_______________________________________________________________________________
EXP xmlErr convertMsrScoreToMusicXMLScore (
  S_msrScore mScore,
  string     outputFileName,
  string     passNumber)
{
  // open output file if need be
  // ------------------------------------------------------

  ofstream outFileStream;

#ifdef TRACE_OAH
  if (gTraceOah->fTracePasses) {
    gLogOstream <<
      "Opening file '" << outputFileName << "' for writing" <<
      endl;
  }
#endif

  outFileStream.open (
    outputFileName.c_str(),
    ofstream::out);

  if (! outFileStream.is_open ()) {
    gLogOstream <<
      "Could not open MusicXML output file \"" <<
      outputFileName <<
      "\" for writing, exiting" <<
      endl;

    return kInvalidFile;
  }

  // create an indented output stream for the LilyPond code
  // to be written to outFileStream
  indentedOstream
    musicXMLCodeFileOutputStream (
      outFileStream,
      gIndenter);

  // convert the MSR score to an mxmltree
  Sxmlelement
    mxmltree =
      buildMxmltreeFromMsrScore (
        mScore,
        gMsrOah,
        gLogOstream);

  // create the MusicXML data
	SXMLFile xmlFile = createXMLFile ();
/* JMI
	SXMLFile xmlFile = TXMLFile::create ();

  TXMLDecl * xmlDecl = new TXMLDecl ("1.0", "UTF-8", TXMLDecl::kNo);
	xmlFile->set (xmlDecl);

	TDocType * docType = new TDocType ("score-partwise");
	xmlFile->set (docType);
*/

	// insert the mxmlTree into it
  xmlFile->set (mxmltree);

	// write the MusicXML data to the output file stream
	xmlFile->print (outFileStream);
	outFileStream << endl;

#ifdef TRACE_OAH
  if (gTraceOah->fTracePasses) {
    gLogOstream <<
      endl <<
      "Closing file '" << outputFileName << "'" <<
      endl;
  }
#endif

  outFileStream.close ();

  return kNoErr;
}

//_______________________________________________________________________________
EXP xmlErr convertMusicXMLBackToMusicXML (
  string inputSourceName,
  string outputFileName)
{
  // sanity check
  // ------------------------------------------------------

  if (inputSourceName == outputFileName) {
    gLogOstream <<
      "\"" << inputSourceName << "\" is both the input and output file name" <<
      endl;

    return kInvalidOption;
  }

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

  if (gXml2xmlOah->fExit2a) {
    gLogOstream <<
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
        gLogOstream,
        "Pass 2b");
    }
    catch (mxmlTreeToMsrException& e) {
      return kInvalidFile;
    }
    catch (std::exception& e) {
      return kInvalidFile;
    }

  if (gXml2xmlOah->fExit2b) {
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

  // create MusicXML back from the MSR
  // ------------------------------------------------------

  xmlErr err =
    convertMsrScoreToMusicXMLScore (
      mScore,
      regex_replace (
        outputFileName,
        regex (".ly"),
        "_LOOP.xml"),
        "Pass 3");

  if (err != kNoErr) {
    return err;
  }

  return kNoErr;
}


}
