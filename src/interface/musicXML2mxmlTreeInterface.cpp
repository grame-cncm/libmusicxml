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

#include "traceOptions.h"
#include "musicXMLOptions.h"

#include "messagesHandling.h"

#include "xml2lyOptionsHandling.h"

#include "musicXML2mxmlTreeInterface.h"


using namespace std;

namespace MusicXML2
{

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

  xmlreader r;
  
  SXMLFile xmlFile = r.read (fileName);
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
  TXMLDecl * xmlDecl = xmlFile->getXMLDecl ();
  
  if (false) { // JMI
    gLogIOstream <<
      "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" <<
      "getXMLDecl =" <<
      endl;
    xmlDecl->print (gLogIOstream);
  }
  
  string fXMLVersion = xmlDecl->getVersion ();
  string fXMLEncoding = xmlDecl->getEncoding ();
  int     fXMLStandalone = xmlDecl->getStandalone ();

  const int fieldWidth = 17;
  
  gLogIOstream <<
    "XML Declaration:" <<
    endl;

  gIndenter++;
  
  gLogIOstream << left <<
    setw (fieldWidth) <<
    "fXMLVersion" << " = \"" << fXMLVersion << "\"" <<
    endl <<
    setw (fieldWidth) <<
    "fXMLEncoding" << " = \"" << fXMLEncoding << "\"" <<
    endl  <<
    setw (fieldWidth) <<
    "fXMLStandalone" << " = \"" << fXMLStandalone << "\"" <<
    endl <<
    endl;

  gIndenter--;

  // get the docType
  TDocType * docType = xmlFile->getDocType ();
  
  if (false) { // JMI
    gLogIOstream <<
      "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" <<
      "getDocType =" <<
      endl;
    docType->print (gLogIOstream);
  }

  gLogIOstream <<
    "Document Type:" <<
    endl;

  gIndenter++;
  
  std::string fXMLStartElement = docType->getStartElement ();
  bool    fXMLPublic = docType->getPublic ();
  std::string fXMLPubLitteral = docType->getPubLitteral ();
  std::string fXMLSysLitteral = docType->getSysLitteral ();

  gLogIOstream << left <<
    setw (fieldWidth) <<
    "fXMLStartElement" << " = \"" << fXMLStartElement << "\"" <<
    endl <<
    setw (fieldWidth) <<
    "fXMLPublic" << " = \"" << fXMLPublic << "\"" <<
    endl  <<
    setw (fieldWidth) <<
    "fXMLPubLitteral" << " = \"" << fXMLPubLitteral << "\"" <<
    endl  <<
    setw (fieldWidth) <<
    "fXMLSysLitteral" << " = \"" << fXMLSysLitteral << "\"" <<
    endl <<
    endl;
    
  gIndenter--;

  Sxmlelement mxmlTree;
    
  // should the encoding be converted to UTF-8?
  string desiredEncoding = "UTF-8";
  
  if (fXMLEncoding != desiredEncoding) {
    gLogIOstream <<
      "Converting MusicXML data from \"" <<
      fXMLEncoding <<
      "\" to " <<
      desiredEncoding <<
      "\"" <<
      endl;

    // build shell command
    stringstream s;

    s <<
      "sed 's/" <<
      fXMLEncoding <<
      "/" <<
      desiredEncoding <<
      "/' " <<
      fileName <<
      " | " <<
      "iconv" <<
      " -f " << fXMLEncoding <<
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

    FILE    *flotDeLecture;
 // JMI   char    tampon [1024];

    // create a stream to receive the result of shellCommand
    if ((flotDeLecture = popen (shellCommand.c_str (), "r")) < 0) {
      msrInternalError (
        gXml2lyOptions->fInputSourceName,
        0, // inputLineNumber
        __FILE__, __LINE__,
        "Cannot read the input stream with 'popen ()'");
    }

    mxmlTree =
      musicXMLFd2mxmlTree (
        flotDeLecture,
        mxmlOpts,
        logIOstream);

      /*
    // read from the stream
    int numeroDeLigne = 0;
    while (
      ! feof (flotDeLecture)
        &&
      ! ferror (flotDeLecture)
        &&
      fgets (tampon, sizeof (tampon), flotDeLecture) != NULL
      ) {
      fputs (tampon, stdout);
    } // while
    tampon [strlen (tampon) -1] = '\0'; // removing trailing end of line
  
    // close the stream
    if (pclose (flotDeLecture) < 0) {
      msrInternalError (
        gXml2lyOptions->fInputSourceName,
        0, // inputLineNumber
        __FILE__, __LINE__,
        "Cannot close the input stream after 'popen ()'");
    }
*/

  }

  else {
    mxmlTree = xmlFile->elements ();
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
  TXMLDecl * xmlDecl = xmlFile->getXMLDecl ();
  
  if (false) { // JMI
    gLogIOstream <<
      "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" <<
      "getXMLDecl =" <<
      endl;
    xmlDecl->print (gLogIOstream);
  }
  
  string fXMLVersion = xmlDecl->getVersion ();
  string fXMLEncoding = xmlDecl->getEncoding ();
  int     fXMLStandalone = xmlDecl->getStandalone ();

  const int fieldWidth = 17;
  
  gLogIOstream <<
    "XML Declaration:" <<
    endl;

  gIndenter++;
  
  gLogIOstream << left <<
    setw (fieldWidth) <<
    "fXMLVersion" << " = \"" << fXMLVersion << "\"" <<
    endl <<
    setw (fieldWidth) <<
    "fXMLEncoding" << " = \"" << fXMLEncoding << "\"" <<
    endl  <<
    setw (fieldWidth) <<
    "fXMLStandalone" << " = \"" << fXMLStandalone << "\"" <<
    endl <<
    endl;

  gIndenter--;

  // get the docType
  TDocType * docType = xmlFile->getDocType ();
  
  if (false) { // JMI
    gLogIOstream <<
      "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" <<
      "getDocType =" <<
      endl;
    docType->print (gLogIOstream);
  }

  gLogIOstream <<
    "Document Type:" <<
    endl;

  gIndenter++;
  
  std::string fXMLStartElement = docType->getStartElement ();
  bool    fXMLPublic = docType->getPublic ();
  std::string fXMLPubLitteral = docType->getPubLitteral ();
  std::string fXMLSysLitteral = docType->getSysLitteral ();

  gLogIOstream << left <<
    setw (fieldWidth) <<
    "fXMLStartElement" << " = \"" << fXMLStartElement << "\"" <<
    endl <<
    setw (fieldWidth) <<
    "fXMLPublic" << " = \"" << fXMLPublic << "\"" <<
    endl  <<
    setw (fieldWidth) <<
    "fXMLPubLitteral" << " = \"" << fXMLPubLitteral << "\"" <<
    endl  <<
    setw (fieldWidth) <<
    "fXMLSysLitteral" << " = \"" << fXMLSysLitteral << "\"" <<
    endl <<
    endl;
    
  gIndenter--;

  // should the encoding be converted to UTF-8?
  string desiredEncoding = "UTF-8";
  
  if (fXMLEncoding != desiredEncoding) {
    gLogIOstream <<
      "Converting MusicXML data from \"" <<
      fXMLEncoding <<
      "\" to \"" <<
      desiredEncoding <<
      "\"" <<
      endl;    

    /* see
    https://jineshkj.wordpress.com/2006/12/22/how-to-capture-stdin-stdout-and-stderr-of-child-program/
    */
    
    // set the desired encoding
    xmlDecl->setEncoding (desiredEncoding);

    // build shell command
    stringstream s;

    s <<
      "iconv" <<
      " -f " << fXMLEncoding <<
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
    
    pipe (outfd); // Where the parent is going to write to
    pipe (infd); // From where parent is going to read
    
    if (! fork ()) {
      close(STDOUT_FILENO);
      close(STDIN_FILENO);
      
      dup2 (outfd [0], STDIN_FILENO);
      dup2 (infd  [1], STDOUT_FILENO);
      
      close (outfd [0]); // Not required for the child
      close (outfd [1]);
      close (infd  [0]);
      close (infd  [1]);
      
      system ("iconv -f ISO-8859-1 -t UTF-8 -");
    }
    
    else {
      char input [100];
      
      close (outfd [0]); // These are being used by the child
      close (infd  [1]);
      
      write (outfd [1], "2^32\n", 5); // Write to child’s stdin
      
      input [read (infd [0], input, 100)] = 0; // Read from child’s stdout
      
      printf ("%s", input);
      
      close (outfd [1]);
      close (infd  [0]);
    }
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
