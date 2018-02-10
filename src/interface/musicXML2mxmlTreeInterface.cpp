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

#include <stdio.h> // for popen()
#include <string.h> // for strlen()
#include <unistd.h> // for pipe(), fork(), ...
#include <regex>

#include <iomanip> // for setw()

#include "xml.h"
#include "xmlfile.h"
#include "xmlreader.h"

#include "generalOptions.h"
#include "traceOptions.h"
#include "musicXMLOptions.h"

#include "messagesHandling.h"

#include "xml2lyOptionsHandling.h"

#include "musicXML2mxmlTreeInterface.h"


using namespace std;

namespace MusicXML2
{

#define TRACE_OPTIONS 0

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
              
        if (TRACE_OPTIONS) {
          logIOstream <<
            "currentLine:" <<
            endl;

          gIndenter++;
          
          logIOstream <<
            currentLine <<
            endl;
            
          gIndenter--;
        }

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
          if (TRACE_OPTIONS) {
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

  return uncompressedFileName;
}

//_______________________________________________________________________________
FILE* convertFileDataEncoding (
  string           fileName,
  string           currentEncoding,
  string           desiredEncoding,
  indentedOstream& logIOstream)
{
  // build shell command
  stringstream s;

  s <<
    "sed 's/" <<
    currentEncoding <<
    "/" <<
    desiredEncoding <<
    "/' " <<
    fileName <<
    " | " <<
    "iconv" <<
    " -f " << currentEncoding <<
    " -t " << desiredEncoding <<
    " -";

  if (true) {
    s <<
    " | tee ConvertedFileData_" <<
      baseName (fileName) << "_" <<
      desiredEncoding << ".xml";
  }

  string shellCommand = s.str ();
            
  if (true) {
    logIOstream <<
      "Converting file MusicXML data from \"" <<
      currentEncoding <<
      "\" to " <<
      desiredEncoding <<
      "\" with command:" <<
      endl;

    gIndenter++;
    
    logIOstream <<
      shellCommand <<
      endl <<
      endl;
    
    gIndenter--;
  }

  // create a stream to receive the result of shellCommand
  FILE* inputStream =
    popen (
      shellCommand.c_str (),
      "r");

  if (inputStream == nullptr) {
    msrInternalError (
      gXml2lyOptions->fInputSourceName,
      0, // inputLineNumber
      __FILE__, __LINE__,
      "Cannot read the input stream with 'popen ()'");
  }

  return inputStream;
}

//_______________________________________________________________________________
SXMLFile convertStreamDataEncoding (
  SXMLFile         xmlFile,
  string           currentEncoding,
  string           desiredEncoding,
  indentedOstream& logIOstream)
{
  SXMLFile result = nullptr;
  
  // build shell command
  stringstream s;

  s <<
    "iconv" <<
    " -f " << currentEncoding <<
    " -t " << desiredEncoding <<
    " -" <<
    " -";

  if (true) {
    s <<
    " | tee StreamData_" << desiredEncoding << ".xml";
  }

  string shellCommand = s.str ();
            
  if (true) {
    logIOstream <<
      "Converting stream MusicXML data from \"" <<
      currentEncoding <<
      "\" to \"" <<
      desiredEncoding <<
      "\" with command:" <<
      endl;

    gIndenter++;
    
    logIOstream <<
      shellCommand <<
      endl <<
      endl;
    
    gIndenter--;
  }

  /*
  setup 2 pipes for the communication between parent and child,
  see
  https://jineshkj.wordpress.com/2006/12/22/how-to-capture-stdin-stdout-and-stderr-of-child-program/
  */
  
  // in a pipe, pipe[0] is for read and  pipe[1] is for write
  #define READ_FD  0
  #define WRITE_FD 1

  // create the 2 pipes
  int parentToChildFds   [2];
  int childFromParentFds [2];

  pipe (parentToChildFds); // where the parent is going to write to
  if (true) {
    logIOstream <<
      "pipe parentToChildFds contains:" <<
      gTab <<
      "read: " << parentToChildFds [READ_FD] <<
      gTab <<
      "write: " << parentToChildFds [WRITE_FD] <<
      endl;
  }

  pipe (childFromParentFds);  // where the child is going to read from
  if (true) {
    logIOstream <<
      "pipe childFromParentFds contains:" <<
      gTab <<
      "read: " << childFromParentFds [READ_FD] <<
      gTab <<
      "write: " << childFromParentFds [WRITE_FD] <<
      endl;
  }
  
  pid_t   idProcess;
  
  switch (idProcess = fork ())
    {
    case -1:
      if (! (gGeneralOptions->fQuiet && gGeneralOptions->fIgnoreErrors)) {  
        logIOstream <<
          endl <<
          "'fork ()' failed" <<
          endl <<
          /* JMI
          inputSourceName << ":" << inputLineNumber << ": " <<message <<
          endl <<
          baseName (sourceCodeFileName) << ":" << sourceCodeLineNumber <<
          */
          endl;
      }
      
      abort ();
      break;

    case 0:
      {
        // child process only
        // ------------------

        // close the descriptors not used by the child
        close (STDIN_FILENO);
        close (STDOUT_FILENO);

        close (parentToChildFds   [WRITE_FD]);
        close (childFromParentFds [READ_FD]);

        // setting child input descriptor
        if (true) {
          logIOstream <<
            "Child will read from parentToChildFds [READ_FD], i.e. " <<
            parentToChildFds [READ_FD] <<
            endl;
        }
        dup2 (parentToChildFds [READ_FD], STDIN_FILENO);
        
        // setting child output descriptor
        if (true) {
          logIOstream <<
            "Child will write to childFromParentFds [WRITE_FD]. i.e. " <<
            childFromParentFds [WRITE_FD] <<
            endl;
        }
        dup2 (childFromParentFds [WRITE_FD], STDOUT_FILENO);

        // write to stdout
        system ("iconv -f ISO-8859-1 -t UTF-8 - | tee ConvertedData.xml");

        // close the descriptors after use by the child
        close (parentToChildFds [READ_FD]); 
        close (childFromParentFds [WRITE_FD]);

        exit (0);      
        
      }
      break;

    default:
      {
        // parent process only
        // -------------------

        // close the descriptors used by the child
        close (parentToChildFds   [READ_FD]); 
        close (childFromParentFds [WRITE_FD]);

        // create a stream buffer to receive output
        OFdnStreambuf outputStreamBuffer (
          parentToChildFds [WRITE_FD], 1024);

        // create the output stream to write to
        ostream outputStream (& outputStreamBuffer);

        if (true) {
          // write the xmlFile representation to log stream
          logIOstream <<
            endl <<
            "<!-- ******************************* -->" <<
            endl <<
            "<!-- The xmlFile contains: -->" <<
            endl;

          xmlFile->print (logIOstream);

          logIOstream <<
            endl <<
            "<!-- ******************************* -->" <<
            endl <<
            endl;
        }

        // write the xmlFile representation to output stream
        xmlFile->print (outputStream);

        // close the needed descriptor after use
        close (parentToChildFds [WRITE_FD]);

        // open the input stream descriptor for reading
        FILE* inputStream =
          fdopen (
            childFromParentFds [READ_FD], "r");
    
        // read the converted data
        if (true) {
          logIOstream <<
            "Reading the converted data from descriptor " <<
            "childFromParentFds [READ_FD], i.e. " <<
            childFromParentFds [READ_FD] <<
            endl;
        }
        
        xmlreader r;

        SXMLFile result = r.read (inputStream);

        // close the needed descriptor after use
        close (childFromParentFds [READ_FD]);
      }
      break;
    } // switch

  // both parent and child processses
  return result;
}

//_______________________________________________________________________________
EXP Sxmlelement musicXMLFile2mxmlTree (
  const char*       fileName,
  S_musicXMLOptions mxmlOpts,
  indentedOstream&  logIOstream) 
{
  clock_t startClock = clock ();

  string fileNameAsString = fileName;
  
  if (gTraceOptions->fTraceBasic) {
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

  // has the input file name a ".mxl" suffix?    
  size_t
    posInString =
      fileNameAsString.rfind (".mxl");
        
  if (posInString == fileNameAsString.size () - 4) {
 // JMI  if (posInString != fileNameAsString.npos) {
    // yes, this is a compressed file

    string uncompressedFileName =
      uncompressMXLFile (
        fileNameAsString,
        logIOstream);

    // the incompressed file in /tmp will be handled
    // instead of the compressed one 
    fileName = uncompressedFileName.c_str ();
  }

  // read the input MusicXML data from the file
  xmlreader r;
  
  SXMLFile xmlFile = r.read (fileName);

  // has there been a problem?
  if (! xmlFile) {
    return Sxmlelement (0);
  }

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
  
  // get the xmlDecl
  TXMLDecl * xmlDecl = xmlFile->getXMLDecl ();
  
  if (gTraceOptions->fTraceEncoding) {
    logIOstream <<
      endl <<
      "!!!!! xmlDecl contents from file:" <<
      endl <<
      endl;
    xmlDecl->print (logIOstream);
  }

  if (gTraceOptions->fTraceEncoding) {
    displayXMLDeclaration (
      xmlDecl,
      logIOstream);
  }
  
  // get the docType
  TDocType * docType = xmlFile->getDocType ();
  
  if (gTraceOptions->fTraceEncoding) {
    logIOstream <<
      endl <<
      "!!!!! docType from file:" <<
      endl <<
      endl;
    docType->print (logIOstream);
  }

  if (gTraceOptions->fTraceEncoding) {
    displayDocumentType (
      docType,
      logIOstream);
  }

  // get the encoding type
  string encoding = xmlDecl->getEncoding ();

  // build the xmlelement tree
  Sxmlelement mxmlTree;
    
  // should the encoding be converted to UTF-8?
  string desiredEncoding = "UTF-8";
  
  if (encoding == desiredEncoding) {
    logIOstream <<
      "MusicXML data uses \"" <<
      desiredEncoding <<
      "\" encoding" <<
      endl;

    mxmlTree = xmlFile->elements ();
  }

  else {
    // convert file data
    FILE* inputStream =
      convertFileDataEncoding (
        fileName,
        encoding,
        desiredEncoding,
        logIOstream);

    // build xmlement tree from inputStream
    mxmlTree =
      musicXMLFd2mxmlTree (
        inputStream,
        mxmlOpts,
        logIOstream);
  
    // register encoding as the desired one after re-reading the file
    xmlDecl->setEncoding (desiredEncoding);
  }
 
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
  
  if (gTraceOptions->fTraceBasic) {
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

  // read the input MusicXML data
  xmlreader r;
  
  SXMLFile xmlFile = r.read (fd);

  // has there been a problem?
  if (! xmlFile) {
    return Sxmlelement (0);
  }

  if (gTraceOptions->fTraceEncoding) {
    logIOstream <<
      "!!!!! xmlFile contents from stream:" <<
      endl;
    xmlFile->print (logIOstream);
    logIOstream <<
      endl;
  }
  
  // get the xmlDecl
  TXMLDecl *xmlDecl = xmlFile->getXMLDecl ();
  
  if (gTraceOptions->fTraceEncoding) {
    logIOstream <<
      endl <<
      "xmlDecl contents:" <<
      endl <<
      endl;
    xmlDecl->print (logIOstream);
  }

  if (gTraceOptions->fTraceEncoding) {
    displayXMLDeclaration (
      xmlDecl,
      logIOstream);
  }

  // get the docType
  TDocType * docType = xmlFile->getDocType ();
  
  if (gTraceOptions->fTraceEncoding) {
    logIOstream <<
      endl <<
      "!!!!! docType from stream:" <<
      endl <<
      endl;
    docType->print (logIOstream);
  }

  if (gTraceOptions->fTraceEncoding) {
    displayDocumentType (
      docType,
      logIOstream);
  }
  
  // get the encoding type
  string encoding = xmlDecl->getEncoding ();

  // should the encoding be converted to UTF-8?
  string desiredEncoding = "UTF-8";
  
  if (encoding == desiredEncoding) {
    logIOstream <<
      "MusicXML data uses \"" <<
      desiredEncoding <<
      "\" encoding" <<
      ", desired encoding is \"" << desiredEncoding << "\"" <<
      endl;
  }

  else {
    // register encoding as the desired one prior to printing
    xmlDecl->setEncoding (desiredEncoding);

    // convert the stream data to desiredEncoding
    xmlFile =
      convertStreamDataEncoding (
        xmlFile,
        encoding,
        desiredEncoding,
        logIOstream);
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

  if (gTraceOptions->fTraceBasic) {
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
