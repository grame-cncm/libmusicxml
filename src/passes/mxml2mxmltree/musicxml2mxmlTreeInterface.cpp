/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#include <string.h> // for strlen()
#include <fstream>      // ofstream, ofstream::open(), ofstream::close()
#include <regex>

#include <iomanip> // for setw()

#include "xml.h"
#include "xmlfile.h"
#include "xmlreader.h"

#include "messagesHandling.h"

#include "musicxml2mxmlTreeInterface.h"

#include "enableTracingIfDesired.h"
#ifdef TRACING_IS_ENABLED
  #include "traceOah.h"
#endif

#include "oahOah.h"
#include "generalOah.h"

#include "musicxmlOah.h"
#include "msrOah.h"

#include "oahAtomsCollection.h"


using namespace std;

namespace MusicXML2
{

//_______________________________________________________________________________
void displayXMLDeclaration (
  TXMLDecl* xmlDeclaration)
{
  string xmlVersion    = xmlDeclaration->getVersion ();
  string xmlEncoding   = xmlDeclaration->getEncoding ();
  int    xmlStandalone = xmlDeclaration->getStandalone ();

  const int fieldWidth = 14;

  gLogStream <<
    "XML Declaration:" <<
    endl;

  gIndenter++;

  gLogStream << left <<
    setw (fieldWidth) <<
    "xmlVersion" << " = \"" << xmlVersion << "\"" <<
    endl <<
    setw (fieldWidth) <<
    "xmlEncoding" << " = \"" << xmlEncoding << "\"" <<
    endl  <<
    setw (fieldWidth) <<
    "xmlStandalone" << " = \"" << xmlStandalone << "\"" <<
    endl << endl;

  gIndenter--;
}

//_______________________________________________________________________________
void displayMusicXMLDocumentType (
  TDocType* documentType)
{
  const int fieldWidth = 16;

  gLogStream <<
    "Document Type:" <<
    endl;

  gIndenter++;

  std::string xmlStartElement = documentType->getStartElement ();
  bool        xmlPublic       = documentType->getPublic ();
  std::string xmlPubLitteral  = documentType->getPubLitteral ();
  std::string xmlSysLitteral  = documentType->getSysLitteral ();

  gLogStream << left <<
    setw (fieldWidth) <<
    "xmlStartElement" << " = \"" << xmlStartElement << "\"" <<
    endl <<
    setw (fieldWidth) <<
    "xmlPublic" << " = \"" << xmlPublic << "\"" <<
    endl  <<
    setw (fieldWidth) <<
    "xmlPubLitteral" << " = \"" << xmlPubLitteral << "\"" <<
    endl  <<
    setw (fieldWidth) <<
    "xmlSysLitteral" << " = \"" << xmlSysLitteral << "\"" <<
    endl << endl;

  gIndenter--;
}

//_______________________________________________________________________________
string uncompressMXLFile (
  string   mxlFileName)
{
  string fileBaseName = baseName (mxlFileName);

  gLogStream <<
    "The compressed file name is '" <<
    mxlFileName <<
    "'" <<
    endl << endl;

  string uncompressedFileName;

#ifdef WIN32
  // JMI
#else
  {
    // build shell command to list the contents of the uncompress file
    stringstream s1;

    s1 <<
      "unzip -l " <<
      mxlFileName;

    string listContentsShellCommand = s1.str ();

    if (true) { // JMI
      gLogStream <<
        "Listing the contents of the compressed file '" <<
        mxlFileName <<
        "' with command:" <<
        endl;

      gIndenter++;

      gLogStream <<
        listContentsShellCommand <<
        endl << endl;

      gIndenter--;
    }

      // create a stream to receive the result of listContentsShellCommand
    FILE* inputStream =
      popen (
        listContentsShellCommand.c_str (),
        "r");

    if (inputStream == nullptr) {
      stringstream s;

      s <<
        "Cannot list the contents of compressed file '" <<
        mxlFileName <<
        "' with 'popen ()'";

      msrInternalError (
        gGlobalOahOahGroup->getInputSourceName (),
        0, // inputLineNumber
        __FILE__, __LINE__,
        s.str ());
    }

    else {
      string contentsList;

      // read the list from inputStream
      char tampon [1024];

      while (
        ! feof (inputStream)
          &&
        ! ferror (inputStream)
          &&
        fgets (tampon, sizeof (tampon), inputStream) != NULL
        ) {
        // append the contents of tampon to contentsList
        contentsList += tampon;
      } // while
      // terminate the string in tampon
      tampon [strlen (tampon) -1] = '\0';

      // close the stream
      if (pclose (inputStream) < 0) {
        msrInternalError (
          gGlobalOahOahGroup->getInputSourceName (),
          0, // inputLineNumber
          __FILE__, __LINE__,
          "Cannot close the input stream after 'popen ()'");
      }

      gLogStream <<
        "The contents of the compressed file '" <<
        mxlFileName <<
        "' is:" <<
        endl;

      gIndenter++;

      gLogStream <<
        contentsList <<
        endl;

      gIndenter--;

      // analyze the contents list
      list<string> linesList;

      istringstream inputStream (contentsList);
      string        currentLine;

      while (getline (inputStream, currentLine)) {

        if (inputStream.eof ()) break;

#ifdef TRACING_IS_ENABLED
        {
          gLogStream <<
            "*** currentLine:" <<
            endl;

          gIndenter++;

          gLogStream <<
            currentLine <<
            endl;

          gIndenter--;
        }
#endif

        /*
        user@lilydev: ~/libmusicxml-git/files/samples/musicxml > unzip -l UnofficialTestSuite/90a-Compressed-MusicXML.mxl
        Archive:  UnofficialTestSuite/90a-Compressed-MusicXML.mxl
          Length      Date    Time    Name
        ---------  ---------- -----   ----
                0  2007-11-14 16:04   META-INF/
              246  2007-11-14 16:02   META-INF/container.xml
             2494  2008-11-14 23:03   20a-Compressed-MusicXML.xml
            30903  2007-11-14 15:51   20a-Compressed-MusicXML.pdf
        ---------                     -------
            33643                     4 files
        */

        string regularExpression (
          "[[:space:]]*"
          ".*"            // length
          "[[:space:]]+"
          ".*"            // date
          "[[:space:]]+"
          ".*"            // time
          "[[:space:]]+"
          "(.*)"          // name
          );

        regex  e (regularExpression);
        smatch sm;

        regex_match (currentLine, sm, e);

        if (sm.size ()) {
#ifdef TRACING_IS_ENABLED
          if (gGlobalTraceOahGroup->getTracePasses ()) { // JMI ???
            gLogStream <<
              "There are " << sm.size () - 1 << " match(es) " <<
              "with regex '" << regularExpression <<
              "':" <<
              endl;

            for (unsigned i = 1; i < sm.size (); ++i) {
              gLogStream <<
                "[" << sm [i] << "] " <<
                endl;
            } // for

            gLogStream <<
              endl << endl;
          }
#endif

          string stringFromLine = sm [1];

          // has stringFromLine a ".xml" suffix?
          size_t
            posInString =
              stringFromLine.rfind (".xml");

          // JMI if (posInString == stringFromLine.size () - 4) {
          if (posInString != stringFromLine.npos) {  // JMI STRANGISSIMO!!!
    //      if (posInString != stringFromLine.npos && stringFromLine != "files") {  // JMI STRANGISSIMO!!!
            // yes, this is a MusicXML file

            // is this file part of META-INF?
            size_t
              posInString =
                stringFromLine.find ("META-INF");

            if (posInString == stringFromLine.npos) {
              // no, this is an actual MusicXML file

              if (uncompressedFileName.size ()) {
                stringstream s;

                s <<
                  "Compressed file '" << mxlFileName <<
                  "' contains multiple MusicMXL files" <<
                  ", found '" << uncompressedFileName <<
                  "' and then '" << stringFromLine << "'";

                msrInternalError (
                  gGlobalOahOahGroup->getInputSourceName (),
                  0, // inputLineNumber
                  __FILE__, __LINE__,
                  s.str ());
              }

              else {
                // we've got the uncompressed file name
                uncompressedFileName = stringFromLine;

                gLogStream <<
                  "The uncompressed file name is '" <<
                  uncompressedFileName <<
                  "'" <<
                  endl << endl;
              }
            }
          }
        }
      } // while
    }
  }

  {
    // build shell command to uncompress the file
    stringstream s2;

    s2 <<
      "unzip -u -d /tmp " <<
      mxlFileName;

    string uncompressShellCommand = s2.str ();

    if (true) { // JMI
      gLogStream <<
        "Uncompressing '" <<
        mxlFileName <<
        "' into '/tmp/" <<
        uncompressedFileName <<
        "' with command:" <<
        endl;

      gIndenter++;

      gLogStream <<
        uncompressShellCommand <<
        endl << endl;

      gIndenter--;
    }

    // create a stream to receive the result of uncompressShellCommand
    FILE* inputStream =
      popen (
        uncompressShellCommand.c_str (),
        "r");

    if (inputStream == nullptr) {
      stringstream s;

      s <<
        "Cannot uncompress the file '" <<
        mxlFileName <<
        "' with 'popen ()'";

      msrInternalError (
        gGlobalOahOahGroup->getInputSourceName (),
        0, // inputLineNumber
        __FILE__, __LINE__,
        s.str ());
    }
  }
#endif

  return uncompressedFileName;
}

//_______________________________________________________________________________
SXMLFile createXMLFileFromFile (
  const char * file,
  string       passNumber)
{
  SXMLFile result;

  // start the clock
  clock_t startClock = clock ();

	xmlreader r;
	SXMLFile xmlfile;

	result = r.read (file);

  // register time spent
  clock_t endClock = clock ();

  timing::gGlobalTiming.appendTimingItem (
    passNumber,
    "Build the MusicXML elements tree from a file",
    timingItem::kMandatory,
    startClock,
    endClock);

	return result;
}

//_______________________________________________________________________________
SXMLFile createXMLFileFromFd (
  FILE * fd,
  string passNumber)
{
  SXMLFile result;

  // start the clock
  clock_t startClock = clock ();

	xmlreader r;

	result = r.read (fd);

  // register time spent
  clock_t endClock = clock ();

  timing::gGlobalTiming.appendTimingItem (
    passNumber,
    "Build the MusicXML elements tree from a file descriptor",
    timingItem::kMandatory,
    startClock,
    endClock);

	return result;
}

//_______________________________________________________________________________
SXMLFile createXMLFileFromString (
  const char * buffer,
  string     passNumber)
{
  SXMLFile result;

  // start the clock
  clock_t startClock = clock ();

	xmlreader r;

  if (buffer [0] != '\0') { // superflous ??? JMI
  	xmlreader r;

	  result = r.readbuff (buffer);
  }

  // register time spent
  clock_t endClock = clock ();

  timing::gGlobalTiming.appendTimingItem (
    passNumber,
    "Build the MusicXML elements tree from a string",
    timingItem::kMandatory,
    startClock,
    endClock);

	return result;
}

//_______________________________________________________________________________
EXP Sxmlelement musicXMLFile2mxmlTree (
  const char*   fileName,
  S_mxmlTreeOahGroup mxmlOpts,
  string        passNumber)
{
  // start the clock
  clock_t startClock = clock ();

  string fileNameAsString = fileName;

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTracePasses ()) {
    string separator =
      "%--------------------------------------------------------------";

    gLogStream <<
      endl <<
      separator <<
      endl <<
      gTab <<
      passNumber << ": building the xmlelement tree from \"" <<
      fileNameAsString << "\"" <<
      endl <<
      separator <<
      endl << endl;
  }
#endif

  // has the input file name a ".mxl" suffix?
  size_t
    posInString =
      fileNameAsString.rfind (".mxl");

  if (posInString == fileNameAsString.size () - 4) {
 // JMI  if (posInString != fileNameAsString.npos) {
    // yes, this is a compressed file

    /* JMI OS dependent
    string uncompressedFileName =
      uncompressMXLFile (
        fileNameAsString);

    // the incompressed file in /tmp will be handled
    // instead of the compressed one
    fileName = uncompressedFileName.c_str ();
    */

    stringstream s;

    s <<
      "you should uncompress file " <<
      fileNameAsString <<
      " prior to running xml2ly";

    string message = s.str ();

    msrMusicXMLError (
      gGlobalOahOahGroup->getInputSourceName (),
      1, // inputLineNumber,
      __FILE__, __LINE__,
      message);

    gLogStream <<
      message <<
      endl;

    throw musicXMLFile2mxmlTreeException (message);
  }

  // read the input MusicXML data from the file
  xmlreader r;

  SXMLFile xmlFile = r.read (fileName);

  // has there been a problem?
  if (! xmlFile) {
    return Sxmlelement (0);
  }

#ifdef TRACING_IS_ENABLED
  if (gGlobalMusicxmlOahGroup->getTraceEncoding ()) {
    gLogStream <<
      endl <<
      "!!!!! xmlFile contents from file:" <<
      endl;

    xmlFile->print (gLogStream);

    gLogStream <<
      endl; // twice ??? JMI
  }
#endif

  // get the xmlDecl
  TXMLDecl * xmlDecl = xmlFile->getXMLDecl ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMusicxmlOahGroup->getTraceEncoding ()) {
    gLogStream <<
      endl <<
      "!!!!! xmlDecl contents from file:" <<
      endl << endl;
    xmlDecl->print (gLogStream);

    displayXMLDeclaration (
      xmlDecl);
  }
#endif

#ifdef TRACING_IS_ENABLED
  // get the docType
  TDocType * docType = xmlFile->getDocType ();

  if (gGlobalMusicxmlOahGroup->getTraceEncoding ()) {
    gLogStream <<
      endl <<
      "!!!!! docType from file:" <<
      endl << endl;
    docType->print (gLogStream);

    displayMusicXMLDocumentType (
      docType);
  }
#endif

  // get the encoding type
  string encoding = xmlDecl->getEncoding ();

  // build the xmlelement tree
  Sxmlelement mxmlTree;

  // should the encoding be converted to UTF-8?
  string desiredEncoding = "UTF-8";

  if (encoding == desiredEncoding) {
#ifdef TRACING_IS_ENABLED
    if (gGlobalTraceOahGroup->getTracePasses ()) {
      gLogStream <<
        "% MusicXML data uses \"" <<
        desiredEncoding <<
        "\" encoding" <<
        endl;
    }
#endif
  }

  else if (encoding.size () == 0) {
    stringstream s;

    s <<
      "MusicXML data in this file" <<
      " doesn't contain any encoding specification; assuming it is UTF-8";

    msrMusicXMLWarning (
      gGlobalOahOahGroup->getInputSourceName (),
      1, // inputLineNumber,
      s.str ());
  }

  else {
    stringstream s;

    s <<
      "you should convert this file to " <<
      desiredEncoding <<
      "\" encoding prior to running xml2ly" <<
      ", for example with iconv or using a text editor - handling it as is";

    msrMusicXMLWarning (
//    msrMusicXMLError (
      gGlobalOahOahGroup->getInputSourceName (),
      1, // inputLineNumber,
//      __FILE__, __LINE__,
      s.str ());
  }

  mxmlTree = xmlFile->elements ();

  // register time spent
  clock_t endClock = clock ();

  timing::gGlobalTiming.appendTimingItem (
    passNumber,
    "build xmlelement tree from file",
    timingItem::kMandatory,
    startClock,
    endClock);

  return mxmlTree;
}

//_______________________________________________________________________________
EXP Sxmlelement musicXMLFd2mxmlTree (
  FILE*         fd,
  S_mxmlTreeOahGroup mxmlOpts,
  string        passNumber)
{
  // start the clock
  clock_t startClock = clock ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTracePasses ()) {
    string separator =
      "%--------------------------------------------------------------";

    gLogStream <<
      endl <<
      separator <<
      endl <<
      gTab <<
      passNumber << ": building the xmlelement tree from standard input" <<
      endl <<
      separator <<
      endl;
  }
#endif

  // read the input MusicXML data
  xmlreader r;

  SXMLFile xmlFile = r.read (fd);

  // has there been a problem?
  if (! xmlFile) {
    return Sxmlelement (0);
  }

#ifdef TRACING_IS_ENABLED
  if (gGlobalMusicxmlOahGroup->getTraceEncoding ()) {
    gLogStream <<
      "!!!!! xmlFile contents from stream:" <<
      endl;
    xmlFile->print (gLogStream);
    gLogStream << endl;
  }
#endif

  // get the xmlDecl
  TXMLDecl *xmlDecl = xmlFile->getXMLDecl ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMusicxmlOahGroup->getTraceEncoding ()) {
    gLogStream <<
      endl <<
      "xmlDecl contents:" <<
      endl << endl;
    xmlDecl->print (gLogStream);

    displayXMLDeclaration (
      xmlDecl);
  }
#endif

#ifdef TRACING_IS_ENABLED
  // get the docType
  TDocType * docType = xmlFile->getDocType ();

  if (gGlobalMusicxmlOahGroup->getTraceEncoding ()) {
    gLogStream <<
      endl <<
      "!!!!! docType from stream:" <<
      endl << endl;
    docType->print (gLogStream);

    displayMusicXMLDocumentType (
      docType);
  }
#endif

  // get the encoding type
  string encoding = xmlDecl->getEncoding ();

  // should the encoding be converted to UTF-8?
  string desiredEncoding = "UTF-8";

#ifdef TRACING_IS_ENABLED
  if (gGlobalMusicxmlOahGroup->getTraceEncoding ()) {
    gLogStream <<
      "% MusicXML data uses \"" <<
      desiredEncoding <<
      "\" encoding" <<
      ", desired encoding is \"" << desiredEncoding << "\"" <<
      endl;
  }
#endif

  if (encoding != desiredEncoding) {
     stringstream s;

    s <<
      "you should convert this stream to " <<
      desiredEncoding <<
      "\" encoding prior to running xml2ly" <<
      ", for example with iconv or using a text editor - handling it as is";

    msrMusicXMLWarning (
      gGlobalOahOahGroup->getInputSourceName (),
      1, // inputLineNumber,
      s.str ());
  }

  // register time spent
  clock_t endClock = clock ();

  timing::gGlobalTiming.appendTimingItem (
    passNumber,
    ": build xmlelement tree from standard input",
    timingItem::kMandatory,
    startClock,
    endClock);

  // fetch mxmlTree
  Sxmlelement mxmlTree = xmlFile->elements ();

  return mxmlTree;
}

//_______________________________________________________________________________
EXP Sxmlelement musicXMLString2mxmlTree (
  const char*   buffer,
  S_mxmlTreeOahGroup mxmlOpts,
  string        passNumber)
{
  // start the clock
  clock_t startClock = clock ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTracePasses ()) {
    string separator =
      "%--------------------------------------------------------------";

    gLogStream <<
      endl <<
      separator <<
      endl <<
      gTab <<
      passNumber << "building the xmlelement tree from a buffer" <<
      endl <<
      separator <<
      endl;
  }
#endif

  xmlreader r;

  SXMLFile xmlFile = r.readbuff (buffer);

  // register time spent
  clock_t endClock = clock ();

  timing::gGlobalTiming.appendTimingItem (
    passNumber,
    ": build xmlelement tree from buffer",
    timingItem::kMandatory,
    startClock,
    endClock);

  // fetch mxmlTree
  Sxmlelement mxmlTree = xmlFile->elements();

  return mxmlTree;
}

//_______________________________________________________________________________
Sxmlelement convertMusicXMLToMxmlTree (
  string inputSourceName,
  string passNumber)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTracePasses ()) {
    string separator =
      "%--------------------------------------------------------------";

    gLogStream <<
      "convertMusicXMLToMxmlTree(): " <<
      "inputSourceName: \"" << inputSourceName << "\"" <<
      ", passNumber: \"" << passNumber << "\"" <<
      endl <<
      separator <<
      endl;
  }
#endif

  Sxmlelement mxmlTree;

  if (inputSourceName == "-") {
    // MusicXML data comes from standard input
    mxmlTree =
      musicXMLFd2mxmlTree (
        stdin,
        gGlobalMxmlTreeOahGroup,
        passNumber);
  }

  else {
    // input comes from a file

    // has the input file name a ".mxl" suffix?
    size_t
      posInString =
        inputSourceName.rfind (".mxl");

    if (posInString == inputSourceName.size () - 4) {
      stringstream s;

      s <<
        "compressed MusicXML files are not handled by xml2ly currently, quitting";

      msrMusicXMLError (
        inputSourceName,
        0, // inputLineNumber
        __FILE__, __LINE__,
        s.str ());
    }

    // OK, let's go ahead
    mxmlTree =
      musicXMLFile2mxmlTree (
        inputSourceName.c_str(),
        gGlobalMxmlTreeOahGroup,
        passNumber);
  }

  return mxmlTree;
}

/* JMI
//_______________________________________________________________________________
EXP S_msrScore mxmlTreeToMsrScoreSkeleton (
  Sxmlelement   mxmlTree,
  S_mxmlTreeOahGroup mxmlOpts,
  string        passNumber)
{

  // create the MSR skeleton from the mxmlTree
  // ------------------------------------------------------

  // start the clock
  clock_t startClock = clock ();

  S_msrScore mScore;

  try {
    mScore =
      convertMxmlTreeToMsrScoreSkeleton (
        mxmlTree,
        gGlobalMsrOahGroup,
        passNumber);
  }
  catch (mxmlTreeToMsrException& e) {
    return kInvalidFile;
  }
  catch (std::exception& e) {
    return kInvalidFile;
  }

  // register time spent
  clock_t endClock = clock ();

  timing::gGlobalTiming.appendTimingItem (
    passNumber,
    "create the MSR skeleton",
    timingItem::kMandatory,
    startClock,
    endClock);

  return mScore;
}
*/

} // namespace
