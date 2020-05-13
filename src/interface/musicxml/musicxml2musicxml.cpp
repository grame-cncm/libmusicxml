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

#include "libmusicxml.h"

#include "msr.h"


using namespace std;

namespace MusicXML2
{

//_______________________________________________________________________________
static xmlErr xml2xml(SXMLFile& xmlfile, const optionsVector& options, ostream& out, const char* file)
{
/*
	Sxmlelement st = xmlfile->elements();
	if (st) {
		if (st->getName() == "score-timewise") return kUnsupported;

		xml2guidovisitor v(true, true, optionsVector, partFilter);
		Sguidoelement gmn = v.convert(st);
		if (file) {
			out << "(*\n  gmn code converted from '" << file << "'"
				<< "\n  using libmusicxml v." << musicxmllibVersionStr();
		}
		else out << "(*\n  gmn code converted using libmusicxml v." << musicxmllibVersionStr();
		out << "\n  and the embedded xml2guido converter v." << musicxml2guidoVersionStr()
			<< "\n*)" << endl;
		out << gmn << endl;
		return kNoErr;
	}
	*/
	return kInvalidFile;
}

//_______________________________________________________________________________
EXP xmlErr musicxmlfile2musicxml(const char *file, const optionsVector& options, ostream& out)
{
/*
	xmlreader r;
	SXMLFile xmlfile;
	xmlfile = r.read(file);
	if (xmlfile) {
		return xml2musicxml(xmlfile, optionsVector, 0, out, file);
	}
	*/
	return kInvalidFile;
}

//_______________________________________________________________________________
EXP xmlErr musicxmlfd2musicxml(FILE * fd, const optionsVector& options, ostream& out)
{
/*
	xmlreader r;
	SXMLFile xmlfile;
	xmlfile = r.read(fd);
	if (xmlfile) {
		return xml2musicxml(xmlfile, optionsVector, 0, out, 0);
	}
	*/
	return kInvalidFile;
}

//_______________________________________________________________________________
EXP xmlErr musicxmlstring2musicxml(const char * buffer, const optionsVector& options, ostream& out)
{
/*
	xmlreader r;
	SXMLFile xmlfile;
	xmlfile = r.readbuff(buffer);
	if (xmlfile) {
		return xml2musicxml(xmlfile, optionsVector, partFilter, out, 0);
	}
	*/
	return kInvalidFile;
}

//_______________________________________________________________________________
void convertMsrScoreToMusicXMLScore (
  S_msrScore mScore,
  string     outputFileName,
  string     passNumber)
{
/*
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

    exit (9);
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
	/*
	SXMLFile xmlFile = TXMLFile::create ();

  TXMLDecl * xmlDecl = new TXMLDecl ("1.0", "UTF-8", TXMLDecl::kNo);
	xmlFile->set (xmlDecl);

	TDocType * docType = new TDocType ("score-partwise");
	xmlFile->set (docType);
* /

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
  */
}

//_______________________________________________________________________________
EXP xmlErr convertMusicXMLBackToMusicXML (
  string inputSourceName,
  string outputFileName)
{
/*
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

  // create MusicXML back from the MSR
  // ------------------------------------------------------
  convertMsrScoreToMusicXMLScore (
    mScore,
    regex_replace (
      outputFileName,
      regex (".ly"),
      "_LOOP.xml"),
      "Pass 3");
*/

  return kNoErr;
}


}
