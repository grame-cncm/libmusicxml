/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifdef VC6
# pragma warning (disable : 4786)
#endif

#include <string.h> // for strlen()
#include <regex>

#include <iomanip> // for setw()

#ifdef WIN32
  // JMI
#else
  #include <unistd.h> // for pipe(), fork(), ...
  #include <stdio.h> // for popen()
#endif

#include "xml.h"
#include "xmlfile.h"
#include "xmlreader.h"

#include "setTraceOptionsIfDesired.h"
#ifdef TRACE_OPTIONS
  #include "traceOptions.h"
#endif


#include "generalOptions.h"
#include "musicXMLOptions.h"

#include "messagesHandling.h"

#include "xml2lyOptionsHandling.h"

#include "musicXML2mxmlTreeInterface.h"


using namespace std;

namespace MusicXML2
{

//_______________________________________________________________________________
void displayXMLDeclaration (
  TXMLDecl*        xmlDeclaration,
  indentedOstream& logIOstream)
{
  string xmlVersion    = xmlDeclaration->getVersion ();
  string xmlEncoding   = xmlDeclaration->getEncoding ();
  int    xmlStandalone = xmlDeclaration->getStandalone ();

  const int fieldWidth = 14;
  
  logIOstream <<
    "XML Declaration:" <<
    endl;

  gIndenter++;
  
  logIOstream << left <<
    setw (fieldWidth) <<
    "xmlVersion" << " = \"" << xmlVersion << "\"" <<
    endl <<
    setw (fieldWidth) <<
    "xmlEncoding" << " = \"" << xmlEncoding << "\"" <<
    endl  <<
    setw (fieldWidth) <<
    "xmlStandalone" << " = \"" << xmlStandalone << "\"" <<
    endl <<
    endl;

  gIndenter--;
}

//_______________________________________________________________________________
void displayDocumentType (
  TDocType*        documentType,
  indentedOstream& logIOstream)
{
  const int fieldWidth = 16;

  logIOstream <<
    "Document Type:" <<
    endl;

  gIndenter++;

  std::string xmlStartElement = documentType->getStartElement ();
  bool        xmlPublic       = documentType->getPublic ();
  std::string xmlPubLitteral  = documentType->getPubLitteral ();
  std::string xmlSysLitteral  = documentType->getSysLitteral ();

  logIOstream << left <<
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
    endl <<
    endl;
    
  gIndenter--;
}

//_______________________________________________________________________________
string uncompressMXLFile (
  string           mxlFileName,
  indentedOstream& logIOstream)
{
  string fileBaseName = baseName (mxlFileName);

  logIOstream <<
    "The compressed file name is '" <<
    mxlFileName <<
    "'" <<
    endl <<
    endl;
    
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
              
    if (true) {
      logIOstream <<
        "Listing the contents of the compressed file '" <<
        mxlFileName <<
        "' with command:" <<
        endl;
  
      gIndenter++;
      
      logIOstream <<
        listContentsShellCommand <<
        endl <<
        endl;
      
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
        gXml2lyOptions->fInputSourceName,
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
          gXml2lyOptions->fInputSourceName,
          0, // inputLineNumber
          __FILE__, __LINE__,
          "Cannot close the input stream after 'popen ()'");
      }

      logIOstream <<
        "The contents of the compressed file '" <<
        mxlFileName <<
        "' is:" <<
        endl;
  
      gIndenter++;
      
      logIOstream <<
        contentsList <<
        endl;
      
      gIndenter--;

      // analyze the contents list      
      list<string> linesList;
    
      istringstream inputStream (contentsList);
      string        currentLine;
      
      while (getline (inputStream, currentLine)) {

        if (inputStream.eof ()) break;
              
#ifdef TRACE_OPTIONS
        {
          logIOstream <<
            "*** currentLine:" <<
            endl;

          gIndenter++;
          
          logIOstream <<
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
          ".*"   // length
          "[[:space:]]+"
          ".*"   // date
          "[[:space:]]+"
          ".*"   // time
          "[[:space:]]+"
          "(.*)" // name
          );
        
        regex  e (regularExpression);
        smatch sm;
  
        regex_match (currentLine, sm, e);
  
        if (sm.size ()) {
#ifdef TRACE_OPTIONS
          if (gTraceOptions->fTracePasses) { // JMI ???
            logIOstream <<
              "There are " << sm.size () - 1 << " match(es) " <<
              "with regex '" << regularExpression <<
              "':" <<
              endl;
  
            for (unsigned i = 1; i < sm.size (); ++i) {
              logIOstream <<
                "[" << sm [i] << "] " <<
                endl;
            } // for
          
            logIOstream <<
              endl <<
              endl;
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
                  gXml2lyOptions->fInputSourceName,
                  0, // inputLineNumber
                  __FILE__, __LINE__,
                  s.str ());
              }
              
              else {
                // we've got the uncompressed file name
                uncompressedFileName = stringFromLine;
          
                logIOstream <<
                  "The uncompressed file name is '" <<
                  uncompressedFileName <<
                  "'" <<
                  endl <<
                  endl;
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
              
    if (true) {
      logIOstream <<
        "Uncompressing '" <<
        mxlFileName <<
        "' into '/tmp/" <<
        uncompressedFileName <<
        "' with command:" <<
        endl;
  
      gIndenter++;
      
      logIOstream <<
        uncompressShellCommand <<
        endl <<
        endl;
      
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
        gXml2lyOptions->fInputSourceName,
        0, // inputLineNumber
        __FILE__, __LINE__,
        s.str ());
    }
  }
#endif

  return uncompressedFileName;
}

//_______________________________________________________________________________
EXP Sxmlelement musicXMLFile2mxmlTree (
  const char*       fileName,
  S_musicXMLOptions mxmlOpts,
  indentedOstream&  logIOstream) 
{
  clock_t startClock = clock ();

  string fileNameAsString = fileName;
  
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTracePasses) {
    string separator =
      "%--------------------------------------------------------------";
    
    logIOstream <<
      endl <<
      separator <<
      endl <<
      gTab <<
      "Pass 1: building the xmlelement tree from \"" << fileNameAsString << "\"" <<
      endl <<
      separator <<
      endl <<
      endl;
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
        fileNameAsString,
        logIOstream);

    // the incompressed file in /tmp will be handled
    // instead of the compressed one 
    fileName = uncompressedFileName.c_str ();
    */

    stringstream s;
    
    s <<
      "you should uncompress this file prior to running xml2ly";
          
    msrMusicXMLError (
      gXml2lyOptions->fInputSourceName,
      1, // inputLineNumber,
      __FILE__, __LINE__,
      s.str ());

    exit (333);
  }

  // read the input MusicXML data from the file
  xmlreader r;
  
  SXMLFile xmlFile = r.read (fileName);

  // has there been a problem?
  if (! xmlFile) {
    return Sxmlelement (0);
  }

#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceEncoding) {
    logIOstream <<
      endl <<
      "!!!!! xmlFile contents from file:" <<
      endl <<
      endl;
      
    xmlFile->print (logIOstream);

    logIOstream <<
      endl <<
      endl;
  }
#endif
  
  // get the xmlDecl
  TXMLDecl * xmlDecl = xmlFile->getXMLDecl ();
  
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceEncoding) {
    logIOstream <<
      endl <<
      "!!!!! xmlDecl contents from file:" <<
      endl <<
      endl;
    xmlDecl->print (logIOstream);

    displayXMLDeclaration (
      xmlDecl,
      logIOstream);
  }
#endif
  
#ifdef TRACE_OPTIONS
  // get the docType
  TDocType * docType = xmlFile->getDocType ();
  
  if (gTraceOptions->fTraceEncoding) {
    logIOstream <<
      endl <<
      "!!!!! docType from file:" <<
      endl <<
      endl;
    docType->print (logIOstream);

    displayDocumentType (
      docType,
      logIOstream);
  }
#endif

  // get the encoding type
  string encoding = xmlDecl->getEncoding ();

  // build the xmlelement tree
  Sxmlelement mxmlTree;
    
  // should the encoding be converted to UTF-8?
  string desiredEncoding = "UTF-8";
  
  if (encoding == desiredEncoding) {
#ifdef TRACE_OPTIONS
    if (gTraceOptions->fTracePasses) {
      logIOstream <<
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
      gXml2lyOptions->fInputSourceName,
      1, // inputLineNumber,
      s.str ());
  }
  
  else {
    stringstream s;
    
    s <<
      "you should convert this file to " <<
      desiredEncoding <<
      "\" encoding prior to running xml2ly" <<
      ", for example with iconv; handling it as is";
          
    msrMusicXMLWarning (
      gXml2lyOptions->fInputSourceName,
      1, // inputLineNumber,
      s.str ());
  }
 
  mxmlTree = xmlFile->elements ();

  clock_t endClock = clock ();

  // register time spent
  timing::gTiming.appendTimingItem (
    "Pass 1",
    "build xmlelement tree from file",
    timingItem::kMandatory,
    startClock,
    endClock);
  
  return mxmlTree;
}

//_______________________________________________________________________________
EXP Sxmlelement musicXMLFd2mxmlTree (
  FILE*             fd,
  S_musicXMLOptions mxmlOpts,
  indentedOstream&  logIOstream) 
{
  clock_t startClock = clock ();
  
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTracePasses) {
    string separator =
      "%--------------------------------------------------------------";
    
    logIOstream <<
      endl <<
      separator <<
      endl <<
      gTab <<
      "Pass 1: building the xmlelement tree from standard input" <<
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

#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceEncoding) {
    logIOstream <<
      "!!!!! xmlFile contents from stream:" <<
      endl;
    xmlFile->print (logIOstream);
    logIOstream <<
      endl;
  }
#endif
  
  // get the xmlDecl
  TXMLDecl *xmlDecl = xmlFile->getXMLDecl ();
  
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceEncoding) {
    logIOstream <<
      endl <<
      "xmlDecl contents:" <<
      endl <<
      endl;
    xmlDecl->print (logIOstream);

    displayXMLDeclaration (
      xmlDecl,
      logIOstream);
  }
#endif

#ifdef TRACE_OPTIONS
  // get the docType
  TDocType * docType = xmlFile->getDocType ();
  
  if (gTraceOptions->fTraceEncoding) {
    logIOstream <<
      endl <<
      "!!!!! docType from stream:" <<
      endl <<
      endl;
    docType->print (logIOstream);

    displayDocumentType (
      docType,
      logIOstream);
  }
#endif
  
  // get the encoding type
  string encoding = xmlDecl->getEncoding ();

  // should the encoding be converted to UTF-8?
  string desiredEncoding = "UTF-8";
  
  logIOstream <<
    "% MusicXML data uses \"" <<
    desiredEncoding <<
    "\" encoding" <<
    ", desired encoding is \"" << desiredEncoding << "\"" <<
    endl;

  if (encoding != desiredEncoding) {    
     stringstream s;
    
    s <<
      "you should convert this stream to " <<
      desiredEncoding <<
      "\" encoding prior to running xml2ly" <<
      ", for example with iconv; exiting";
          
    msrMusicXMLError (
      gXml2lyOptions->fInputSourceName,
      1, // inputLineNumber,
      __FILE__, __LINE__,
      s.str ());

    exit (444);
  }

  clock_t endClock = clock ();

  // register time spent
  timing::gTiming.appendTimingItem (
    "Pass 1",
    "build xmlelement tree from standard input",
    timingItem::kMandatory,
    startClock,
    endClock);

  // fetch mxmlTree
  Sxmlelement mxmlTree = xmlFile->elements ();

  return mxmlTree;
}

//_______________________________________________________________________________
EXP Sxmlelement musicXMLString2mxmlTree (
  const char*       buffer,
  S_musicXMLOptions mxmlOpts,
  indentedOstream&  logIOstream) 
{
  clock_t startClock = clock ();

#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTracePasses) {
    string separator =
      "%--------------------------------------------------------------";
    
    logIOstream <<
      endl <<
      separator <<
      endl <<
      gTab <<
      "Pass 1: building the xmlelement tree from a buffer" <<
      endl <<
      separator <<
      endl;
  }
#endif
  
  xmlreader r;
  
  SXMLFile xmlFile = r.readbuff (buffer);

  clock_t endClock = clock ();

  // register time spent
  timing::gTiming.appendTimingItem (
    "Pass 1",
    "build xmlelement tree from buffer",
    timingItem::kMandatory,
    startClock,
    endClock);
  
  // fetch mxmlTree
  Sxmlelement mxmlTree = xmlFile->elements();

  return mxmlTree;
}


} // namespace
