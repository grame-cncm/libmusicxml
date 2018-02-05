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
#include <iomanip>      // setw, setprecision, ...

#include "version.h"
#include "utilities.h"

#include "xml2lyOptionsHandling.h"

#include "traceOptions.h"
#include "generalOptions.h"
#include "musicXMLOptions.h"
#include "msrOptions.h"
#include "lpsrOptions.h"
#include "lilypondOptions.h"


using namespace std;

namespace MusicXML2 
{

#define TRACE_OPTIONS 0

//______________________________________________________________________________
void xml2lyOptionsVersionItem::printVersion (ostream& os) const
{  
  os <<
    endl <<
    "This is xml2ly" <<
    " version " << currentVersionNumber () <<
    endl <<
    endl;

  // print versions history
  printVersionsHistory (os);
}

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

  initializeXml2lyOptionsHandling (
    this);
    
  initializeTraceOptionsHandling (
    this);
    
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
      gXml2lyOptions->fInputSourceName =
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
    
  if (gXml2lyOptions->fInputSourceName != "-") {
    // determine potential output file name,
    // may be set differently by '--ofn, --outputFileName' option
    potentialOutputFileName =
      baseName (
        gXml2lyOptions->fInputSourceName);

    // set '.ly' suffix
    size_t
      posInString =
        potentialOutputFileName.rfind ('.');
      
    if (posInString != string::npos) {
      potentialOutputFileName.replace (
        posInString,
        potentialOutputFileName.size () - posInString,
        ".ly");
    }
  }

  // check auto output file option usage
  // ------------------------------------------------------

  if (gXml2lyOptions->fAutoOutputFile) {
    if (gXml2lyOptions->fOutputFileName.size ()) {
      stringstream s;
  
      s <<
        "options '--aofn, --autoOutputFileName' and '--ofn, --outputFileName'"  <<
        endl <<
        "cannot be chosen simultaneously";
        
      optionError (s.str ());

      exit (3);
    }
  
    else if (gXml2lyOptions->fInputSourceName == "-") {
      stringstream s;
  
      s <<
        "option '--aofn, --autoOutputFileName'"  <<
        endl <<
        "cannot be used when reading from standard input";
        
      optionError (s.str ());

      exit (4);
    }

    else {
      gXml2lyOptions->fOutputFileName =
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

//_______________________________________________________________________________
S_xml2lyOptions gXml2lyOptions;

S_xml2lyOptions xml2lyOptions::create (
  S_optionsHandler optionsHandler)
{
  xml2lyOptions* o = new xml2lyOptions (
    optionsHandler);
  assert(o!=0);

  return o;
}

xml2lyOptions::xml2lyOptions (
  S_optionsHandler optionsHandler)
  : optionsGroup (
    "xml2ly",
    "hx", "helpXml2ly",
R"(Options that are used by various components of the library
are grouped here.)",
    optionsHandler)
{
  // append this options group to the options handler
  // if relevant
  if (optionsHandler) {
    optionsHandler->
      appendOptionsGroup (this);
  }

  // initialize it
  initializeXml2lyOptions ();
}

xml2lyOptions::~xml2lyOptions ()
{}

void xml2lyOptions::initializeXml2lyOptions ()
{
  // register translation date
  // ------------------------------------------------------

  {
    time_t      translationRawtime;
    struct tm*  translationTimeinfo;
    char buffer [80];
  
    time (&translationRawtime);
    translationTimeinfo = localtime (&translationRawtime);
  
    strftime (buffer, 80, "%A %F @ %T %Z", translationTimeinfo);
    fTranslationDate = buffer;
  }


  // version
  // --------------------------------------

  {
    // variables  
    
    // options
  
    S_optionsSubGroup
      versionSubGroup =
        optionsSubGroup::create (
          "Version",
          "hxv", "helpXml2lyVersion",
R"()",
        optionsSubGroup::kAlwaysShowDescription,
        this);
    
    appendOptionsSubGroup (versionSubGroup);
  
    versionSubGroup->
      appendOptionsItem (
        optionsVersionItem::create (
          "v", "version",
R"(Display xml2ly's version number and history and exit.)"));
  }


  // about
  // --------------------------------------

  {
    // variables  
    
    // options
  
    S_optionsSubGroup
      aboutSubGroup =
        optionsSubGroup::create (
          "About",
          "hxa", "helpXml2lyAbout",
R"()",
        optionsSubGroup::kAlwaysShowDescription,
        this);
    
    appendOptionsSubGroup (aboutSubGroup);
  
    aboutSubGroup->
      appendOptionsItem (
        optionsAboutItem::create (
          "a", "about",
R"(Display information about xml2ly and exit.)"));
  }


  // contact
  // --------------------------------------

  {
    // variables  
    
    // options
  
    S_optionsSubGroup
      contactSubGroup =
        optionsSubGroup::create (
          "Contact",
          "hxc", "helpXml2lyContact",
R"()",
        optionsSubGroup::kAlwaysShowDescription,
        this);
    
    appendOptionsSubGroup (contactSubGroup);
  
    contactSubGroup->
      appendOptionsItem (
        optionsContactItem::create (
          "c", "contact",
R"(Display information about how to contacct xml2ly maintainers and exit.)"));
  }


  // output file
  // --------------------------------------

  {
    // variables  
  
    fAutoOutputFile = false;
    
    // options
  
    S_optionsSubGroup
      outputFileSubGroup =
        optionsSubGroup::create (
          "Output file",
          "hxof", "helpXml2lyOutputFile",
R"()",
        optionsSubGroup::kAlwaysShowDescription,
        this);
            
    appendOptionsSubGroup (outputFileSubGroup);
    
    outputFileSubGroup->
      appendOptionsItem (
        optionsStringItem::create (
          "o", "outputFileName",
R"(Write LilyPond code to file 'fileName' instead of standard output.)",
          "fileName",
          "outputFileName",
          fOutputFileName));  
  
    outputFileSubGroup->
      appendOptionsItem (
        optionsBooleanItem::create (
          "ao", "autoOutputFileName",
R"(This option can only be used when reading from a file.
Write LilyPond code to a file in the current working directory.
The file name is derived from that of the input file,
replacing any suffix after the the '.' by 'ly'
or adding '.ly' if none is present.)",
          "autoOutputFileName",
          fAutoOutputFile));
  }
}

void xml2lyOptions::printXml2lyOptionsValues (int fieldWidth)
{  
  gLogIOstream <<
    "The xml2ly options are:" <<
    endl;

  gIndenter++;

  // command line
  // --------------------------------------

  gLogIOstream << left <<

  gIndenter++;

  gLogIOstream << left <<
    setw (fieldWidth) << "input source name" << " : " <<
    fInputSourceName <<
    endl <<
      
    setw (fieldWidth) << "translation date" << " : " <<
    fTranslationDate <<
    endl;

  gIndenter--;

  // output file
  // --------------------------------------

  gLogIOstream << left <<
    setw (fieldWidth) << "Output file:" <<
    endl;

  gIndenter++;

  gLogIOstream << left <<        
    setw (fieldWidth) << "inputSourceName" << " : \"" <<
    fInputSourceName <<
    "\"" <<
    endl <<
    setw (fieldWidth) << "outputFileName" << " : \"" <<
    fOutputFileName <<
    "\"" <<
    endl;

  gIndenter--;
}

//______________________________________________________________________________
void initializeXml2lyOptionsHandling (
  S_optionsHandler optionsHandler)
{  
  // enlist versions information
  // ------------------------------------------------------

  enlistVersion (
    "Initial", "early 2016",
    "Start as a clone of xml2guido");
    
  enlistVersion (
    "alpha1", "November 2017",
    "First draft version");
  
  // create the options variables
  // ------------------------------------------------------
  
  gXml2lyOptions = xml2lyOptions::create (
    optionsHandler);
  assert(gXml2lyOptions != 0);
}


}
