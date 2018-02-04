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

//_______________________________________________________________________________
void displayXMLDeclaration (
  TXMLDecl *xmlDeclaration)
{
  string xmlVersion    = xmlDeclaration->getVersion ();
  string xmlEncoding   = xmlDeclaration->getEncoding ();
  int    xmlStandalone = xmlDeclaration->getStandalone ();

  const int fieldWidth = 14;
  
  gLogIOstream <<
    "XML Declaration:" <<
    endl;

  gIndenter++;
  
  gLogIOstream << left <<
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
  TDocType *documentType)
{
  const int fieldWidth = 16;

  gLogIOstream <<
    "Document Type:" <<
    endl;

  gIndenter++;

  std::string xmlStartElement = documentType->getStartElement ();
  bool        xmlPublic       = documentType->getPublic ();
  std::string xmlPubLitteral  = documentType->getPubLitteral ();
  std::string xmlSysLitteral  = documentType->getSysLitteral ();

  gLogIOstream << left <<
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
EXP Sxmlelement musicXMLFile2mxmlTree (
  const char*       fileName,
  S_musicXMLOptions mxmlOpts,
  indentedOstream&  logIOstream) 
{
  clock_t startClock = clock ();

  if (gTraceOptions->fTraceBasic) {
    string separator =
      "%--------------------------------------------------------------";
    
    gLogIOstream <<
      endl <<
      separator <<
      endl <<
      gTab <<
      "Pass 1: building the xmlelement tree from \"" << fileName << "\"" <<
      endl <<
      separator <<
      endl <<
      endl;
  }

  // read the input MusicXML data
  xmlreader r;
  
  SXMLFile xmlFile = r.read (fileName);

  // has there been a problem?
  if (! xmlFile) {
    return Sxmlelement (0);
  }

  if (gTraceOptions->fTraceEncoding) {
    gLogIOstream <<
      endl <<
      "!!!!! xmlFile contents from file:" <<
      endl <<
      endl;
      
    xmlFile->print (gLogIOstream);

    gLogIOstream <<
      endl <<
      endl;
  }
  
  // get the xmlDecl
  TXMLDecl * xmlDecl = xmlFile->getXMLDecl ();
  
  if (gTraceOptions->fTraceEncoding) {
    gLogIOstream <<
      endl <<
      "!!!!! xmlDecl contents from file:" <<
      endl <<
      endl;
    xmlDecl->print (gLogIOstream);
  }

  if (gTraceOptions->fTraceEncoding) {
    displayXMLDeclaration (xmlDecl);
  }
  
  // get the docType
  TDocType * docType = xmlFile->getDocType ();
  
  if (gTraceOptions->fTraceEncoding) {
    gLogIOstream <<
      endl <<
      "!!!!! docType from file:" <<
      endl <<
      endl;
    docType->print (gLogIOstream);
  }

  if (gTraceOptions->fTraceEncoding) {
    displayDocumentType (docType);
  }

  // get the encoding type
  string encoding = xmlDecl->getEncoding ();

  // build the xmlelement tree
  Sxmlelement mxmlTree;
    
  // should the encoding be converted to UTF-8?
  string desiredEncoding = "UTF-8";
  
  if (encoding == desiredEncoding) {
    gLogIOstream <<
      "MusicXML data uses \"" <<
      desiredEncoding <<
      "\" encoding" <<
      endl;

    mxmlTree = xmlFile->elements ();
  }

  else {
    // build shell command
    stringstream s;

    s <<
      "sed 's/" <<
      encoding <<
      "/" <<
      desiredEncoding <<
      "/' " <<
      fileName <<
      " | " <<
      "iconv" <<
      " -f " << encoding <<
      " -t " << desiredEncoding <<
      " -" ;

    string shellCommand = s.str ();
              
    if (true) {
      gLogIOstream <<
        "Converting file MusicXML data from \"" <<
        encoding <<
        "\" to " <<
        desiredEncoding <<
        "\" with command:" <<
        endl;

      gIndenter++;
      
      gLogIOstream <<
        shellCommand <<
        endl;
      
      gIndenter--;
    }

    // create a stream to receive the result of shellCommand
    FILE *inputStream =
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
    gLogIOstream <<
      "!!!!! xmlFile contents from stream:" <<
      endl;
    xmlFile->print (gLogIOstream);
    gLogIOstream <<
      endl;
  }
  
  // get the xmlDecl
  TXMLDecl *xmlDecl = xmlFile->getXMLDecl ();
  
  if (gTraceOptions->fTraceEncoding) {
    gLogIOstream <<
      endl <<
      "xmlDecl contents:" <<
      endl <<
      endl;
    xmlDecl->print (gLogIOstream);
  }

  if (gTraceOptions->fTraceEncoding) {
    displayXMLDeclaration (xmlDecl);
  }

  // get the docType
  TDocType * docType = xmlFile->getDocType ();
  
  if (gTraceOptions->fTraceEncoding) {
    gLogIOstream <<
      endl <<
      "!!!!! docType from stream:" <<
      endl <<
      endl;
    docType->print (gLogIOstream);
  }

  if (gTraceOptions->fTraceEncoding) {
    displayDocumentType (docType);
  }
  
  // get the encoding type
  string encoding = xmlDecl->getEncoding ();

  // should the encoding be converted to UTF-8?
  string desiredEncoding = "UTF-8";
  
  if (encoding == desiredEncoding) {
    gLogIOstream <<
      "MusicXML data uses \"" <<
      desiredEncoding <<
      "\" encoding" <<
      endl;
  }

  else {
    // register encoding as the desired one prior to printing
    xmlDecl->setEncoding (desiredEncoding);

    // build shell command
    stringstream s;

    s <<
      "iconv" <<
      " -f " << encoding <<
      " -t " << desiredEncoding <<
      " -";

    string shellCommand = s.str ();
              
    if (true) {
      gLogIOstream <<
        "Converting stream MusicXML data from \"" <<
        encoding <<
        "\" to \"" <<
        desiredEncoding <<
        "\" with command:" <<
        endl;

      gIndenter++;
      
      gLogIOstream <<
        shellCommand <<
        endl;
      
      gIndenter--;
    }

    /*
    setup 2 pipes for the communication between parent and child,
    see
    https://jineshkj.wordpress.com/2006/12/22/how-to-capture-stdin-stdout-and-stderr-of-child-program/
    */
    
    int outfd [2];
    pipe (outfd); // where the parent is going to write to

    int infd  [2];
    pipe (infd);  // where the child is going to read from
    
    pid_t   idProcess;
    
    // in a pipe, pipe[0] is for read and  pipe[1] is for write
    #define READ_FD  0
    #define WRITE_FD 1

    switch (idProcess = fork ())
      {
      case -1:
        if (! (gGeneralOptions->fQuiet && gGeneralOptions->fIgnoreErrors)) {  
          gLogIOstream <<
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

          // these descriptors are not used by the child
          close(STDOUT_FILENO);
          close(STDIN_FILENO);
          
          dup2 (outfd [READ_FD], STDIN_FILENO);
          dup2 (infd  [WRITE_FD], STDOUT_FILENO);

          // descriptors not required for the child
          close (outfd [READ_FD]); 
          close (outfd [WRITE_FD]);
          close (infd  [READ_FD]);
          close (infd  [WRITE_FD]);

          // write to stdout
          system ("iconv -f ISO-8859-1 -t UTF-8 -");
        }
        break;
  
      default:
        {
          // parent process only

          // these descriptors are not used by the parent
          close (outfd [READ_FD]); 
          close (infd  [WRITE_FD]);

          // write to child’s stdin
          write (outfd [WRITE_FD], "2^32\n", 5);
          
          // read from child’s stdout
          FILE *inputStream =
            fdopen (
              infd [READ_FD],
              "r");

          char tampon [1024];
          
          while (
            ! feof (inputStream)
              &&
            ! ferror (inputStream)
              &&
            fgets (tampon, sizeof (tampon), inputStream) != NULL
            ) {
            fputs (tampon, stdout);
          } // while
          tampon [strlen (tampon) -1] = '\0'; // removing trailing end of line
        
          // close the stream
          if (pclose (inputStream) < 0) {
            msrInternalError (
              gXml2lyOptions->fInputSourceName,
              0, // inputLineNumber
              __FILE__, __LINE__,
              "Cannot close the input stream after 'popen ()'");
          }

          // close the descriptors after use
          close (outfd [WRITE_FD]);
          close (infd  [READ_FD]);
        }
        break;
      } // switch
  
    // both parent and child processses
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
