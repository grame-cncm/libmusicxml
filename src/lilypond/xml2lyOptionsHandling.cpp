/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#include <sstream>

#include <climits>      /* INT_MIN, INT_MAX */
#include <iomanip>      // setw, set::precision, ...

#include "xml2lyOptionsHandling.h"

#include "generalOptions.h"
#include "mxmlOptions.h"
#include "msrOptions.h"
#include "lpsrOptions.h"
#include "lilypondOptions.h"

#include "utilities.h"


using namespace std;

namespace MusicXML2 
{

#define TRACE_OPTIONS 0

//______________________________________________________________________________
S_xml2lyOptionsHandler xml2lyOptionsHandler::create (
  indentedOstream& ios)
{
  xml2lyOptionsHandler* o = new
    xml2lyOptionsHandler (
      ios);
  assert(o!=0);
  return o;
}

xml2lyOptionsHandler::xml2lyOptionsHandler (
  indentedOstream& ios)
  : optionsHandler (
    "Available options",
    "Options values",
    "h", "help",
    "hs", "helpSummary",
R"(
                      Welcome to xml2ly, 
              the MusicXML to LilyPond translator
          delivered as part of the libmusicxml2 library.
      https://github.com/grame-cncm/libmusicxml/tree/lilypond

Usage:

    xml2ly [options] [MusicXMLFile|-] [options] 
)",
R"(
Options '-h, -help' print the full help,
while '-hs, -helpSummary' only print a help summary.)",
    ios
    )
{
  initializeOptionsHandler ();
}

xml2lyOptionsHandler::~xml2lyOptionsHandler ()
{}

void xml2lyOptionsHandler::initializeOptionsHandler ()
{
  // initialize options handling
  // ------------------------------------------------------

  initializeGeneralOptionsHandling (
    this);
    
  initializeMusicXMLOptionsHandling (
    this);
    
  initializeMsrOptionsHandling (
    this);
    
  initializeLpsrOptionsHandling (
    this);
    
  initializeLilypondOptionsHandling (
    this);

    /* JMI
  // append the various options groups
  // ------------------------------------------------------
  
  appendOptionsGroup (
    gGeneralOptions);
    
 // appendOptionsGroup (
//    gMusicXMLOptions);
    
  appendOptionsGroup (
    gMsrOptions);
    
  appendOptionsGroup (
    gLpsrOptions);
    
  appendOptionsGroup (
    gLilypondOptions);
*/

  if (TRACE_OPTIONS) {
    // print the options handler initial state
    fOptionsHandlerLogIOstream <<
      "xml2lyOptionsHandler has been initialized as:" <<
      endl;
  
    gIndenter++;
  
    print (
      fOptionsHandlerLogIOstream);
    fOptionsHandlerLogIOstream <<
      endl <<
      endl;
    
    gIndenter--;
  }

  // register options handler in itself,
  // so that the 'global' help options can be handled
  this->
    registerOptionsHandlerInItself ();

  if (TRACE_OPTIONS) {
    fOptionsHandlerLogIOstream <<
      "xml2lyOptionsHandler help:" <<
      endl;

    this->
      printHelp (
        fOptionsHandlerLogIOstream);
  }
}

void xml2lyOptionsHandler::checkOptionsAndArguments ()
{
  unsigned int argumentsNumber =
    fArgumentsVector.size ();

  if (TRACE_OPTIONS) {    
    if (argumentsNumber > 0) {
      fOptionsHandlerLogIOstream <<
        singularOrPluralWithoutNumber (
          argumentsNumber, "There is", "There are") <<
        " " <<
        argumentsNumber <<
        " " <<
        singularOrPluralWithoutNumber (
          argumentsNumber, "argument", "arguments") <<
        ":" <<
        endl;

      gIndenter++;
      
      for (unsigned int i = 0; i < argumentsNumber; i++) {
        fOptionsHandlerLogIOstream <<
          i << " : " << fArgumentsVector [i] <<
            endl;
      } // for

      fOptionsHandlerLogIOstream <<
        endl;

      gIndenter--;
    }
    else {
      fOptionsHandlerLogIOstream <<
        "There are no arguments" <<
        endl;
    }
  }

  // input source name
  // ------------------------------------------------------

  switch (argumentsNumber) {
    case 1 :
      // register intput file name
      gGeneralOptions->fInputSourceName =
        fArgumentsVector [0];
      break;

    default:
      fOptionsHandlerLogIOstream <<
        endl <<
        "Input file name or '-' for standard input expected" <<
        endl <<
        endl;

      printHelpSummary (
        fOptionsHandlerLogIOstream);

      exit (1);
      break;
  } //  switch

  // build potential output file name
  // ------------------------------------------------------

  string potentialOutputFileName;
    
  if (gGeneralOptions->fInputSourceName != "-") {
    // determine potential output file name,
    // may be set differently by '--ofn, --outputFileName' option
    potentialOutputFileName =
      baseName (
        gGeneralOptions->fInputSourceName);

    // set '.ly' suffix
    size_t
      posInString =
        potentialOutputFileName.rfind ('.');
      
    if (posInString != string::npos)
      potentialOutputFileName.replace (
        posInString,
        potentialOutputFileName.size () - posInString,
        ".ly");
  }

  // check auto output file option usage
  // ------------------------------------------------------

  if (gGeneralOptions->fAutoOutputFile) {
    if (gGeneralOptions->fOutputFileName.size ()) {
      stringstream s;
  
      s <<
        "options '--aofn, --autoOutputFileName' and '--ofn, --outputFileName'"  <<
        endl <<
        "cannot be chosen simultaneously";
        
      optionError (s.str ());

      exit (3);
    }
  
    else if (gGeneralOptions->fInputSourceName == "-") {
      stringstream s;
  
      s <<
        "option '--aofn, --autoOutputFileName'"  <<
        endl <<
        "cannot be used when reading from standard input";
        
      optionError (s.str ());

      exit (4);
    }

    else {
      gGeneralOptions->fOutputFileName =
        potentialOutputFileName;
    }
  }

  // register command line informations in gGeneralOptions
  // ------------------------------------------------------

  gGeneralOptions->fProgramName =
    fProgramName;

  gGeneralOptions->fCommandLineWithShortOptions =
    fCommandLineWithShortOptions;
  gGeneralOptions->fCommandLineWithLongOptions =
    fCommandLineWithLongOptions;
}

void xml2lyOptionsHandler::print (ostream& os) const
{
  const int fieldWidth = 27;
  
  os <<
    "xml2lyOptionsHandler:" <<
    endl;

  gIndenter++;

  os << left <<
    setw (fieldWidth) <<
    "fOptionsElementShortName" << " : " << fOptionsElementShortName <<
    endl <<
    setw (fieldWidth) <<
    "fOptionsElementLongName" << " : " << fOptionsElementLongName <<
    endl <<
    setw (fieldWidth) <<
    "fOptionsElementDescription" << " : " << fOptionsElementDescription <<
    endl <<
    endl;

  os <<
    "Options groups (" <<
    singularOrPlural (
      fOptionsHandlerOptionsGroupsList.size (), "element",  "elements") <<
    "):" <<
    endl;

  if (fOptionsHandlerOptionsGroupsList.size ()) {
    os << endl;
    
    gIndenter++;

    list<S_optionsGroup>::const_iterator
      iBegin = fOptionsHandlerOptionsGroupsList.begin (),
      iEnd   = fOptionsHandlerOptionsGroupsList.end (),
      i      = iBegin;
    for ( ; ; ) {
      // print the element
      os << (*i);
      if (++i == iEnd) break;
      os << endl;
    } // for

    gIndenter--;
  }

  gIndenter--;
  
  os << endl;
}

ostream& operator<< (ostream& os, const S_xml2lyOptionsHandler& elt)
{
  elt->print (os);
  return os;
}


}
