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

#include "xml2lilypondOptionsHandling.h"

#include "generalOptions.h"
#include "musicXMLOptions.h"
#include "msrOptions.h"
#include "lpsrOptions.h"
#include "lilypondOptions.h"

#include "utilities.h"


using namespace std;

namespace MusicXML2 
{

// useful shortcut macros
#define idtr indenter::gIndenter
#define tab  indenter::gIndenter.getSpacer ()

#define TRACE_OPTIONS 0

//______________________________________________________________________________
S_xml2lilypondOptionsHandler xml2lilypondOptionsHandler::create (
  indentedOutputStream& ios)
{
  xml2lilypondOptionsHandler* o = new
    xml2lilypondOptionsHandler (
      ios);
  assert(o!=0);
  return o;
}

xml2lilypondOptionsHandler::xml2lilypondOptionsHandler (
  indentedOutputStream& ios)
  : msrOptionsHandler (
    "Available options",
    "Options values",
    "h", "help",
    "hs", "helpSummary",
R"(
                Welcome to xml2lilypond v0.1.0, 
              the MusicXML to LilyPond translator
          delivered as part of the libmusicxml2 library.
      https://github.com/grame-cncm/libmusicxml/tree/lilypond

Usage:

    xml2lilypond [options] [MusicXMLFile|-] [options] 

What it does:

    This multi-pass translator basically performs 4 passes:
        Pass 1: reads the contents of MusicXMLFile or stdin ('-')
                and converts it to a MusicXML tree;
        Pass 2: converts that tree to a Music Score Representation (MSR);
        Pass 3: converts the MSR to a LilyPond Score Representation (LPSR);
        Pass 4: converts the LPSR to LilyPond source code 
                and writes it to standard output.

    Other passes are performed according to the options, such as
    printing views of the internal data or printing a summary of the score.

    '-' as an argument represents standard input.

    A number of options exist to fine tune the generated LilyPond code
    and limit the need for manually editing the latter.
    They are organized in a group-subgroup-item hierarchy,
    and help can be obtained for groups or subgroups at will.

    Most options have a short and a long name for commodity.
    
    A subgroup displayed with '***' has its description printed
    only when the corresponding item short or long names are used.
    
    Both '-' and '--' can be used to introduce options in the command line,
    even though the help shows only '-'.
    
    Command line options and arguments can be placed in any order,
    provided item values immediately follow the corresponding items.
    
    The activity log and warning/error messages go to standard error.
)",
R"(
Options '-h, -help' print the full help,
while '-hs, -helpSummary' only print a help summary.)",
    ios
    )
{
  initializeOptionsHandler ();
}

xml2lilypondOptionsHandler::~xml2lilypondOptionsHandler()
{}

void xml2lilypondOptionsHandler::initializeOptionsHandler ()
{
  // append the various options groups
  appendOptionsGroup (
    gGeneralOptions);
    
  appendOptionsGroup (
    gMusicXMLOptions);
    
  appendOptionsGroup (
    gMsrOptions);
    
  appendOptionsGroup (
    gLpsrOptions);
    
  appendOptionsGroup (
    gLilypondOptions);

  if (TRACE_OPTIONS) {
    // print the options handler initial state
    cerr << idtr <<
      "xml2lilypondOptionsHandler has been initialized as:" <<
      endl;
  
    idtr++;
  
    print (cerr);
    cerr <<
      endl <<
      endl;
    
    idtr--;
  }

  // register options handler in itself,
  // so that the 'global' help options can be handled
  this->
    registerOptionsHandlerInItself ();

  if (TRACE_OPTIONS) {
    cerr << idtr <<
      "xml2lilypondOptionsHandler help:" <<
      endl;

    this->
      printHelp (cerr);
  }
}

void xml2lilypondOptionsHandler::checkOptionsAndArguments ()
{
  unsigned int argumentsNumber =
    fArgumentsVector.size ();

  if (TRACE_OPTIONS) {    
    if (argumentsNumber > 0) {
      cerr << idtr <<
        singularOrPluralWithoutNumber (
          argumentsNumber, "There is", "There are") <<
        " " <<
        argumentsNumber <<
        " " <<
        singularOrPluralWithoutNumber (
          argumentsNumber, "argument", "arguments") <<
        ":" <<
        endl;

      idtr++;
      
      for (unsigned int i = 0; i < argumentsNumber; i++) {
        cerr << idtr <<
          i << " : " << fArgumentsVector [i] <<
            endl;
      } // for

      cerr <<
        endl;

      idtr--;
    }
    else {
      cerr << idtr <<
        "There are no arguments" <<
        endl;
    }
  }

  // input source name
  // ------------------------------------------------------

  switch (argumentsNumber) {
    case 1 :
      // register intput file name
      fInputSourceName =
        fArgumentsVector [0];
      break;

    default:
      cerr <<
        endl <<
        "Input file name or '-' for standard input expected" <<
        endl <<
        endl;

      printHelpSummary (cerr);

      exit (1);
      break;
  } //  switch

  // build potential output file name
  // ------------------------------------------------------

  if (fInputSourceName == "-") {
    // keep fOutputFileName empty for now
  }
  else {
    // determine potential output file name,
    // may be set differently by '--of, --outputFile' option
    string
      inputFileBasename =
        baseName (fInputSourceName);
    
    fOutputFileName =
      inputFileBasename;
    
    size_t
      posInString =
        fOutputFileName.rfind ('.');
      
    if (posInString != string::npos)
      fOutputFileName.replace (
        posInString, fOutputFileName.size () - posInString, ".ly");
  }

  // check auto output file option usage
  // ------------------------------------------------------

  int outputFileNameSize =
    gGeneralOptions->fOutputFileName.size ();

  if (gGeneralOptions->fAutoOutputFile) {
    if (outputFileNameSize) {
      stringstream s;
  
      s <<
        "options '--aof, --autoOutputFile' and '--of, --outputFile'"  <<
        endl <<
        "cannot be used simultaneously";
        
      optionError (s.str ());
    }
  
    else if (fInputSourceName == "-") {
      stringstream s;
  
      s <<
        "option '--aof, --autoOutputFile'"  <<
        endl <<
        "cannot be used when reading from standard input";
        
      optionError (s.str ());
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

  gGeneralOptions->fInputSourceName =
    fInputSourceName;
  gGeneralOptions->fOutputFileName =
    fOutputFileName;
}

void xml2lilypondOptionsHandler::print (ostream& os) const
{
  const int fieldWidth = 27;
  
  os << idtr <<
    "xml2lilypondOptionsHandler:" <<
    endl;

  idtr++;

  os <<
    idtr << left <<
      setw (fieldWidth) <<
      "fOptionsElementShortName" << " : " << fOptionsElementShortName <<
      endl <<
    idtr <<
      setw (fieldWidth) <<
      "fOptionsElementLongName" << " : " << fOptionsElementLongName <<
      endl <<
    idtr <<
      setw (fieldWidth) <<
      "fOptionsElementDescription" << " : " << fOptionsElementDescription <<
      endl <<
    endl;

  os << idtr <<
    "Options groups (" <<
    singularOrPlural (
      fOptionsHandlerOptionsGroupsList.size (), "element",  "elements") <<
    "):" <<
    endl;

  if (fOptionsHandlerOptionsGroupsList.size ()) {
    os << endl;
    
    idtr++;

    list<S_msrOptionsGroup>::const_iterator
      iBegin = fOptionsHandlerOptionsGroupsList.begin(),
      iEnd   = fOptionsHandlerOptionsGroupsList.end(),
      i      = iBegin;
    for ( ; ; ) {
      // print the element
      os << (*i);
      if (++i == iEnd) break;
      cerr << endl;
    } // for

    idtr--;
  }

  idtr--;
  
  os << endl;
}

ostream& operator<< (ostream& os, const S_xml2lilypondOptionsHandler& elt)
{
  elt->print (os);
  return os;
}


}
