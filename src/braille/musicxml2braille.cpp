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
#include "xml2brlInsiderOahHandler.h"

#include "msr.h"

#include "musicXML2MxmlTreeInterface.h"
#include "mxmlTree2MsrSkeletonBuilderInterface.h"
#include "mxmlTree2MsrTranslatorInterface.h"
#include "msr2MxmlTreeInterface.h"
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
	Sxmlelement st;

	if (xmlfile) {
	  st = xmlfile->elements();

    if (st) {
      if (st->getName() == "score-timewise") return kUnsupported;
    }
  }

  // the fake executable name
  // ------------------------------------------------------

  string fakeExecutableName = "xml2brl";

  // create the options handler
  // ------------------------------------------------------

  S_xml2brlInsiderOahHandler
    handler =
      xml2brlInsiderOahHandler::create (
        fakeExecutableName,
        "xml2brl",
        out);

  // analyze the coptions vector
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

    if (gGlobalXml2brlOahGroup->fExit2a) {
      err <<
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
        err,
        "Pass 2b");
    }
    catch (mxmlTreeToMsrException& e) {
      return kInvalidFile;
    }
    catch (std::exception& e) {
      return kInvalidFile;
    }

    if (gGlobalXml2brlOahGroup->fExit2b) {
      err <<
        endl <<
        "Exiting after pass 2b as requested" <<
        endl;

      return kNoErr;
    }

    // display the MSR score summary if requested
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

    // display the score summary if requested
    // ------------------------------------------------------

    if (gGlobalMsrOah->fDisplayMsrSummary) {
      // display the score summary
      displayMSRPopulatedScoreSummary (
        gGlobalMsrOah,
        mScore,
        gLogOstream);
    }

    // display the score names if requested
    // ------------------------------------------------------

    if (gGlobalMsrOah->fDisplayMsrNames) {
      // display the score name
      displayMSRPopulatedScoreNames (
        gGlobalMsrOah,
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

    // should we return now?
    // ------------------------------------------------------

    if (gGlobalMsr2BsrOah->fExit3a) {
      err <<
        endl <<
        "Exiting after pass 3a as requested" <<
        endl;

      return kNoErr;
    }

    // display the first BSR score if requested
    // ------------------------------------------------------

    if (gGlobalBsrOah->fDisplayBsr) {
      displayBsrFirstScore_OptionalPass (
        firstBsrScore,
        gGlobalMsrOah,
        gGlobalBsrOah);
    }

    if (gGlobalBsrOah->fDisplayBsrShort) {
      displayBsrFirstScoreShort_OptionalPass (
        firstBsrScore,
        gGlobalMsrOah,
        gGlobalBsrOah);
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

    // should we return now?
    // ------------------------------------------------------

    if (gGlobalMsr2BsrOah->fExit3b) {
      err <<
        endl <<
        "Exiting after pass 3b as requested" <<
        endl;

      return kNoErr;
    }

    // display the finalized BSR score if requested
    // ------------------------------------------------------

    if (gGlobalBsrOah->fDisplayBsr) {
      displayFinalizedBsrScore_OptionalPass (
        finalizedBsrScore,
        gGlobalMsrOah,
        gGlobalBsrOah);
    }

    if (gGlobalBsrOah->fDisplayBsrShort) {
      displayFinalizedBsrScoreShort_OptionalPass (
        finalizedBsrScore,
        gGlobalMsrOah,
        gGlobalBsrOah);
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
	SXMLFile  xmlfile;

  if (buffer [0] != '\0') {
  	xmlreader r;

	  xmlfile = r.readbuff (buffer);
  }

	// call xml2lilypond() even if xmlfile is null,
	// to handle the help options if any
  return xml2braille (xmlfile, options, out, err, 0);

	return kInvalidFile;
}

//_______________________________________________________________________________
EXP xmlErr convertMusicXMLToBraille (
  std::string inputSourceName,
  std::string outputFileName)
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
      "### gIndenter final value has changed after convertMusicXMLToMxmlTree(): "<< gIndenter.getIndent () << " ###" <<
      endl <<
      endl;
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
      "### gIndenter final value has changed after convertMxmlTreeToMsrScoreSkeleton(): "<< gIndenter.getIndent () << " ###" <<
      endl <<
      endl;
  }

  // should we return now?
  // ------------------------------------------------------

  if (gGlobalXml2brlOahGroup->fExit2a) {
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
      "### gIndenter final value has changed after populateMsrSkeletonFromMxmlTree(): "<< gIndenter.getIndent () << " ###" <<
      endl <<
      endl;
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
    displayMSRPopulatedScoreSummary (
      gGlobalMsrOah,
      mScore,
      gLogOstream);
  }

  // display the populated MSR score names if requested
  // ------------------------------------------------------

  if (gGlobalMsrOah->fDisplayMsrNames) {
    // display the score name
    displayMSRPopulatedScoreNames (
      gGlobalMsrOah,
      mScore,
      gLogOstream);
  }

  // should we return now?
  // ------------------------------------------------------

  if (gGlobalXml2brlOahGroup->fExit2b) {
    gLogOstream <<
      endl <<
      "Exiting after pass 2b as requested" <<
      endl;

    return kNoErr;
  }

  // create the BSR from the MSR (pass 3a)
  // ------------------------------------------------------

  saveIndent = gIndenter.getIndent ();

  S_bsrScore
    firstBsrScore =
      convertMsrScoreToBsrScore (
        mScore,
        "Pass 3a");

  if (gIndenter != saveIndent) {
    gLogOstream <<
      "### gIndenter final value has changed after convertMsrScoreToBsrScore(): "<< gIndenter.getIndent () << " ###" <<
      endl <<
      endl;
  }

  // should we return now?
  // ------------------------------------------------------

  if (gGlobalMsr2BsrOah->fExit3a) {
    gLogOstream <<
      endl <<
      "Exiting after pass 3a as requested" <<
      endl;

    return kNoErr;
  }

  // display the first BSR score if requested
  // ------------------------------------------------------

  if (gGlobalBsrOah->fDisplayBsr) {
    displayBsrFirstScore_OptionalPass (
      firstBsrScore,
      gGlobalMsrOah,
      gGlobalBsrOah);
  }

  if (gGlobalBsrOah->fDisplayBsrShort) {
    displayBsrFirstScoreShort_OptionalPass (
      firstBsrScore,
      gGlobalMsrOah,
      gGlobalBsrOah);
  }

  // create the finalized BSR from the first BSR (pass 3b)
  // ------------------------------------------------------

  saveIndent = gIndenter.getIndent ();

  S_bsrScore
    finalizedBsrScore =
      convertBsrScoreToFinalizedBsrScore (
        firstBsrScore,
        "Pass 3b");

  if (gIndenter != saveIndent) {
    gLogOstream <<
      "### gIndenter final value has changed after convertBsrScoreToFinalizedBsrScore(): "<< gIndenter.getIndent () << " ###" <<
      endl <<
      endl;
  }

  // should we return now?
  // ------------------------------------------------------

  if (gGlobalMsr2BsrOah->fExit3b) {
    gLogOstream <<
      endl <<
      "Exiting after pass 3b as requested" <<
      endl;

    return kNoErr;
  }

  // display the finalized BSR score if requested
  // ------------------------------------------------------

  if (gGlobalBsrOah->fDisplayBsr) {
    displayFinalizedBsrScore_OptionalPass (
      finalizedBsrScore,
      gGlobalMsrOah,
      gGlobalBsrOah);
  }

  if (gGlobalBsrOah->fDisplayBsrShort) {
    displayFinalizedBsrScoreShort_OptionalPass (
      finalizedBsrScore,
      gGlobalMsrOah,
      gGlobalBsrOah);
  }

  // generate Braille music text from the BSR (pass 4)
  // ------------------------------------------------------

  saveIndent = gIndenter.getIndent ();

  convertBsrScoreToBrailleText (
    outputFileName,
    finalizedBsrScore,
    "Pass 4");

   if (gIndenter != saveIndent) {
    gLogOstream <<
      "### gIndenter final value has changed after convertBsrScoreToBrailleText(): "<< gIndenter.getIndent () << " ###" <<
      endl <<
      endl;
  }

 return kNoErr;
}


}
