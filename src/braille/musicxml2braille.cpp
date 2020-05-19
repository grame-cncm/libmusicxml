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
#include "libmusicxml.h"
#include "xml.h"
#include "xmlfile.h"
#include "xmlreader.h"

#include "setTraceOahIfDesired.h"
#ifdef TRACE_OAH
  #include "traceOah.h"
#endif

#include "msr2LpsrOah.h"
#include "msr2BsrOah.h"

#include "msr.h"
#include "bsr.h"

#include "musicXML2MxmlTreeInterface.h"
#include "mxmlTree2MsrSkeletonBuilderInterface.h"
#include "mxmlTree2MsrTranslatorInterface.h"
#include "msr2MxmltreeInterface.h"
#include "msr2BsrInterface.h"
#include "bsr2BsrFinalizerInterface.h"
#include "bsr2BrailleTranslatorInterface.h"


using namespace std;

namespace MusicXML2
{

//_______________________________________________________________________________
static xmlErr xml2braille (SXMLFile& xmlfile, const optionsVector& options, ostream& out, const char* file)
{
/*
	Sxmlelement st = xmlfile->elements();
	if (st) {
		if (st->getName() == "score-timewise") return kUnsupported;

		xml2braillevisitor v(true, true, generateBars, partFilter);
		Sbrailleelement gmn = v.convert(st);
		if (file) {
			out << "(*\n  gmn code converted from '" << file << "'"
				<< "\n  using libmusicxml v." << musicxmllibVersionStr();
		}
		else out << "(*\n  gmn code converted using libmusicxml v." << musicxmllibVersionStr();
		out << "\n  and the embedded xml2braille converter v." << musicxml2brailleVersionStr()
			<< "\n*)" << endl;
		out << gmn << endl;
		return kNoErr;
	}
	*/
	return kInvalidFile;
}

//_______________________________________________________________________________
EXP xmlErr musicxmlfile2braille (const char *file, const optionsVector& options, ostream&                out)
{
/*
	xmlreader r;
	SXMLFile xmlfile;
	xmlfile = r.read(file);
	if (xmlfile) {
		return xml2braille (xmlfile, generateBars, 0, out, file);
	}
	*/
	return kInvalidFile;
}

//_______________________________________________________________________________
EXP xmlErr musicxmlfd2braille (FILE *fd, const optionsVector& options, ostream& out)
{
/*
	xmlreader r;
	SXMLFile xmlfile;
	xmlfile = r.read(fd);
	if (xmlfile) {
		return xml2braille (xmlfile, generateBars, 0, out, 0);
	}
	*/
	return kInvalidFile;
}

//_______________________________________________________________________________
EXP xmlErr musicxmlstring2braille (const char *buffer, const optionsVector& options, std::ostream&           out)
{
/*
	xmlreader r;
	SXMLFile xmlfile;
	xmlfile = r.readbuff(buffer);
	if (xmlfile) {
		return xml2braille (xmlfile, generateBars, partFilter, out, 0);
	}
	*/
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
