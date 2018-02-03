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

  if (false) { // JMI
    gLogIOstream <<
      "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" <<
      endl;
      
    xmlFile->print (gLogIOstream);

    gLogIOstream <<
      endl;
  }
  
  // get the xmlDecl
  TXMLDecl * xmlDecl = xmlFile->getXMLDecl ();
  
  if (false) { // JMI
    gLogIOstream <<
      "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" <<
      "getXMLDecl =" <<
      endl;
    xmlDecl->print (gLogIOstream);
  }
  displayXMLDeclaration (xmlDecl);
  
  // get the docType
  TDocType * docType = xmlFile->getDocType ();
  
  if (false) { // JMI
    gLogIOstream <<
      "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" <<
      "getDocType =" <<
      endl;
    docType->print (gLogIOstream);
  }
  displayDocumentType (docType);

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
    gLogIOstream <<
      "Converting MusicXML data from \"" <<
      encoding <<
      "\" to " <<
      desiredEncoding <<
      "\"" <<
      endl;

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
      " -";

    string shellCommand = s.str ();
              
    if (true) {
      gLogIOstream <<
        "Executing command:" <<
        endl;

      gIndenter++;
      
      gLogIOstream <<
        shellCommand <<
        endl;
      
      gIndenter--;

      gLogIOstream <<
        "with 'popen ()" <<
        endl <<
        endl;
    }

    FILE *inputStream =
      popen (shellCommand.c_str (), "r");

    // create a stream to receive the result of shellCommand
    if (inputStream == nullptr) {
      msrInternalError (
        gXml2lyOptions->fInputSourceName,
        0, // inputLineNumber
        __FILE__, __LINE__,
        "Cannot read the input stream with 'popen ()'");
    }

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

  xmlreader r;
  
  SXMLFile xmlFile = r.read (fd);
  if (! xmlFile)
    return Sxmlelement (0);

  if (false) { // JMI
    gLogIOstream <<
      "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" <<
      endl;
    xmlFile->print (gLogIOstream);
    gLogIOstream <<
      endl;
  }
  
  // get the xmlDecl
  TXMLDecl *xmlDecl = xmlFile->getXMLDecl ();
  
  if (false) { // JMI
    gLogIOstream <<
      "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" <<
      "getXMLDecl =" <<
      endl;
    xmlDecl->print (gLogIOstream);
  }
  displayXMLDeclaration (xmlDecl);

  // get the docType
  TDocType * docType = xmlFile->getDocType ();
  
  if (false) { // JMI
    gLogIOstream <<
      "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" <<
      "getDocType =" <<
      endl;
    docType->print (gLogIOstream);
  }
  displayDocumentType (docType);
  
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
    gLogIOstream <<
      "Converting MusicXML data from \"" <<
      encoding <<
      "\" to \"" <<
      desiredEncoding <<
      "\"" <<
      endl;    

    /* see
    https://jineshkj.wordpress.com/2006/12/22/how-to-capture-stdin-stdout-and-stderr-of-child-program/
    */
    
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
        "Executing command:" <<
        endl;

      gIndenter++;
      
      gLogIOstream <<
        shellCommand <<
        endl;
      
      gIndenter--;

      gLogIOstream <<
        "with 'popen ()" <<
        endl <<
        endl;
    }

    int outfd [2];
    int infd  [2];
    
    pipe (outfd); // where the parent is going to write to
    pipe (infd);  // from where parent is going to read
    
    pid_t   idProcess;
    
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
          close(STDOUT_FILENO);
          close(STDIN_FILENO);
          
          dup2 (outfd [0], STDIN_FILENO);
          dup2 (infd  [1], STDOUT_FILENO);
          
          close (outfd [0]); // not required for the child
          close (outfd [1]);
          close (infd  [0]);
          close (infd  [1]);
          
          system ("iconv -f ISO-8859-1 -t UTF-8 -");
        }
        break;
  
      default:
        {
          // parent process only
          char input [100];
          
          close (outfd [0]); // these are being used by the child
          close (infd  [1]);
          
          write (outfd [1], "2^32\n", 5); // write to child’s stdin
          
          input [read (infd [0], input, 100)] = 0; // read from child’s stdout
          
          printf ("%s", input);
          
          close (outfd [1]);
          close (infd  [0]);
        }
        break;
      } // switch
  
  
    // both parent and child processses

      /*
    // read from the stream
    int numeroDeLigne = 0;
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
*/
  }


  clock_t endClock = clock ();

  // register time spent
  timing::gTiming.appendTimingItem (
    "Pass 1",
    "build xmlelement tree from stdin",
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
