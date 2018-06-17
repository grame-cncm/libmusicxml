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

#include "msrStreamsExampleOptionsHandling.h"

#include "traceOptions.h"
#include "generalOptions.h"
#include "musicXMLOptions.h"
#include "msrOptions.h"
#include "lpsrOptions.h"
#include "lilypondOptions.h"
#include "extraOptions.h"


using namespace std;

namespace MusicXML2 
{

//______________________________________________________________________________
S_msrStreamsExampleOptionsVersionItem msrStreamsExampleOptionsVersionItem::create (
  string optionsItemShortName,
  string optionsItemLongName,
  string optionsItemDescription)
{
  msrStreamsExampleOptionsVersionItem* o = new
    msrStreamsExampleOptionsVersionItem (
      optionsItemShortName,
      optionsItemLongName,
      optionsItemDescription);
  assert(o!=0);
  return o;
}

msrStreamsExampleOptionsVersionItem::msrStreamsExampleOptionsVersionItem (
  string optionsItemShortName,
  string optionsItemLongName,
  string optionsItemDescription)
  : optionsItem (
      optionsItemShortName,
      optionsItemLongName,
      optionsItemDescription)
{}

msrStreamsExampleOptionsVersionItem::~msrStreamsExampleOptionsVersionItem()
{}

void msrStreamsExampleOptionsVersionItem::print (ostream& os) const
{
  const int fieldWidth = K_FIELD_WIDTH;
  
  os <<
    "msrStreamsExampleOptionsVersionItem:" <<
    endl;

  gIndenter++;

  optionsElement::printElementEssentials (
    os, fieldWidth);

  gIndenter++;
  os <<
    gIndenter.indentMultiLineString (
      fOptionsElementDescription) <<
    endl;
  gIndenter--;

  gIndenter--;
}

void msrStreamsExampleOptionsVersionItem::printVersion (ostream& os) const
{  
  os <<
    endl <<
    "This is msrStreamsExample" <<
    " version " << currentVersionNumber () <<
    endl <<
    endl;

  // print versions history
  printVersionsHistory (os);
}

void msrStreamsExampleOptionsVersionItem::printOptionsValues (
  ostream& os,
  int      valueFieldWidth) const
{
  // nothing to print here
}
                            
ostream& operator<< (ostream& os, const S_msrStreamsExampleOptionsVersionItem& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_msrStreamsExampleOptionsAboutItem msrStreamsExampleOptionsAboutItem::create (
  string optionsItemShortName,
  string optionsItemLongName,
  string optionsItemDescription)
{
  msrStreamsExampleOptionsAboutItem* o = new
    msrStreamsExampleOptionsAboutItem (
      optionsItemShortName,
      optionsItemLongName,
      optionsItemDescription);
  assert(o!=0);
  return o;
}

msrStreamsExampleOptionsAboutItem::msrStreamsExampleOptionsAboutItem (
  string optionsItemShortName,
  string optionsItemLongName,
  string optionsItemDescription)
  : optionsItem (
      optionsItemShortName,
      optionsItemLongName,
      optionsItemDescription)
{}

msrStreamsExampleOptionsAboutItem::~msrStreamsExampleOptionsAboutItem ()
{}

void msrStreamsExampleOptionsAboutItem::print (ostream& os) const
{
  const int fieldWidth = K_FIELD_WIDTH;
  
  os <<
    "msrStreamsExampleOptionsAboutItem:" <<
    endl;

  gIndenter++;

  optionsElement::printElementEssentials (
    os, fieldWidth);

  gIndenter++;
  os <<
    gIndenter.indentMultiLineString (
      fOptionsElementDescription) <<
    endl;
  gIndenter--;

  gIndenter--;
}

void msrStreamsExampleOptionsAboutItem::printAbout (ostream& os) const
{  
  os <<
    endl <<
R"(What msrStreamsExample does:

    This multi-pass translator basically performs 5 passes:
        Pass 1:  reads the contents of MusicXMLFile or stdin ('-')
                 and converts it to a MusicXML tree;
        Pass 2a: converts that MusicXML tree into to
                 a Music Score Representation (MSR) skeleton;
        Pass 2b: converts that tree and the skeleton into a
                 Music Score Representation (MSR);
        Pass 3:  converts the MSR into a
                 LilyPond Score Representation (LPSR);
        Pass 4:  converts the LPSR to LilyPond source code 
                 and writes it to standard output.

    Other passes are performed according to the options, such as
    printing views of the internal data or printing a summary of the score.

    The activity log and warning/error messages go to standard error.)" <<
    endl <<
    endl;
}

void msrStreamsExampleOptionsAboutItem::printOptionsValues (
  ostream& os,
  int      valueFieldWidth) const
{
  // nothing to print here
}
                            
ostream& operator<< (ostream& os, const S_msrStreamsExampleOptionsAboutItem& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_msrStreamsExampleOptionsContactItem msrStreamsExampleOptionsContactItem::create (
  string optionsItemShortName,
  string optionsItemLongName,
  string optionsItemDescription)
{
  msrStreamsExampleOptionsContactItem* o = new
    msrStreamsExampleOptionsContactItem (
      optionsItemShortName,
      optionsItemLongName,
      optionsItemDescription);
  assert(o!=0);
  return o;
}

msrStreamsExampleOptionsContactItem::msrStreamsExampleOptionsContactItem (
  string optionsItemShortName,
  string optionsItemLongName,
  string optionsItemDescription)
  : optionsItem (
      optionsItemShortName,
      optionsItemLongName,
      optionsItemDescription)
{}

msrStreamsExampleOptionsContactItem::~msrStreamsExampleOptionsContactItem ()
{}

void msrStreamsExampleOptionsContactItem::print (ostream& os) const
{
  const int fieldWidth = K_FIELD_WIDTH;
  
  os <<
    "msrStreamsExampleOptionsContactItem:" <<
    endl;

  gIndenter++;

  optionsElement::printElementEssentials (
    os, fieldWidth);

  gIndenter++;
  os <<
    gIndenter.indentMultiLineString (
      fOptionsElementDescription) <<
    endl;
  gIndenter--;

  gIndenter--;
}

void msrStreamsExampleOptionsContactItem::printContact (ostream& os) const
{  
  os <<
    endl <<
R"(To contact msrStreamsExample maintainers:

    Send a mail to mailto:lilypond-user@gnu.org describing the problem
    and error messages you obtain if relevant.)" <<
    endl <<
    endl;
}

void msrStreamsExampleOptionsContactItem::printOptionsValues (
  ostream& os,
  int      valueFieldWidth) const
{
  // nothing to print here
}
                            
ostream& operator<< (ostream& os, const S_msrStreamsExampleOptionsContactItem& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_msrStreamsExampleOptionsHandler msrStreamsExampleOptionsHandler::create (
  indentedOstream& ios)
{
  msrStreamsExampleOptionsHandler* o = new
    msrStreamsExampleOptionsHandler (
      ios);
  assert(o!=0);
  return o;
}

msrStreamsExampleOptionsHandler::msrStreamsExampleOptionsHandler (
  indentedOstream& ios)
  : optionsHandler (
    "Available options",
    "Options values",
    "h", "help",
    "hs", "helpSummary",
R"(
                Welcome to msrStreamsExample, 
              the MusicXML to LilyPond translator
          delivered as part of the libmusicxml2 library.
      https://github.com/grame-cncm/libmusicxml/tree/lilypond

Usage:

    msrStreamsExample [options] [MusicXMLFile|-] [options] 
)",
R"(
Options '-h, -help' print the full help,
while '-hs, -helpSummary' only print a help summary.)",
    ios
    )
{
  initializeOptionsHandler ();
}

msrStreamsExampleOptionsHandler::~msrStreamsExampleOptionsHandler ()
{}

void msrStreamsExampleOptionsHandler::initializeOptionsHandler ()
{
  // initialize options handling
  // ------------------------------------------------------

  initializeMsrStreamsExampleOptionsHandling (
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

  initializeExtraOptionsHandling (
    this);


  if (gTraceOptions->fTraceOptions) {
    // print the options handler initial state
    fOptionsHandlerLogIOstream <<
      "msrStreamsExampleOptionsHandler has been initialized as:" <<
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

  if (gTraceOptions->fTraceOptions) {
    fOptionsHandlerLogIOstream <<
      "msrStreamsExampleOptionsHandler help:" <<
      endl;

    this->
      printHelp (
        fOptionsHandlerLogIOstream);
  }
}

void msrStreamsExampleOptionsHandler::checkOptionsAndArguments ()
{
  unsigned int argumentsNumber =
    fArgumentsVector.size ();

  if (gTraceOptions->fTraceOptions) {    
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
      gMsrStreamsExampleOptions->fInputSourceName =
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
    
  if (gMsrStreamsExampleOptions->fInputSourceName != "-") {
    // determine potential output file name,
    // may be set differently by '--ofn, --outputFileName' option
    potentialOutputFileName =
      baseName (
        gMsrStreamsExampleOptions->fInputSourceName);

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

  if (gMsrStreamsExampleOptions->fAutoOutputFile) {
    if (gMsrStreamsExampleOptions->fOutputFileName.size ()) {
      stringstream s;
  
      s <<
        "options '--aofn, --autoOutputFileName' and '--ofn, --outputFileName'"  <<
        endl <<
        "cannot be chosen simultaneously";
        
      optionError (s.str ());

      exit (3);
    }
  
    else if (gMsrStreamsExampleOptions->fInputSourceName == "-") {
      stringstream s;
  
      s <<
        "option '--aofn, --autoOutputFileName'"  <<
        endl <<
        "cannot be used when reading from standard input";
        
      optionError (s.str ());

      exit (4);
    }

    else {
      gMsrStreamsExampleOptions->fOutputFileName =
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

void msrStreamsExampleOptionsHandler::print (ostream& os) const
{
  const int fieldWidth = 27;
  
  os <<
    "msrStreamsExampleOptionsHandler:" <<
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

ostream& operator<< (ostream& os, const S_msrStreamsExampleOptionsHandler& elt)
{
  elt->print (os);
  return os;
}

//_______________________________________________________________________________
S_msrStreamsExampleOptions gMsrStreamsExampleOptions;

S_msrStreamsExampleOptions msrStreamsExampleOptions::create (
  S_optionsHandler optionsHandler)
{
  msrStreamsExampleOptions* o = new msrStreamsExampleOptions (
    optionsHandler);
  assert(o!=0);

  return o;
}

msrStreamsExampleOptions::msrStreamsExampleOptions (
  S_optionsHandler optionsHandler)
  : optionsGroup (
    "msrStreamsExample",
    "hmse", "helpMsrStreamsExample",
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
  initializeMsrStreamsExampleOptions ();
}

msrStreamsExampleOptions::~msrStreamsExampleOptions ()
{}

void msrStreamsExampleOptions::initializeMsrStreamsExampleOptions ()
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
          "hmsev", "helpMsrStreamsExampleVersion",
R"()",
        optionsSubGroup::kAlwaysShowDescription,
        this);
    
    appendOptionsSubGroup (versionSubGroup);
  
    versionSubGroup->
      appendOptionsItem (
        msrStreamsExampleOptionsVersionItem::create (
          "v", "version",
R"(Display msrStreamsExample's version number and history and exit.)"));
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
          "hmsea", "helpmsrStreamsExampleAbout",
R"()",
        optionsSubGroup::kAlwaysShowDescription,
        this);
    
    appendOptionsSubGroup (aboutSubGroup);
  
    aboutSubGroup->
      appendOptionsItem (
        msrStreamsExampleOptionsAboutItem::create (
          "a", "about",
R"(Display information about msrStreamsExample and exit.)"));
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
          "hmsec", "helpmsrStreamsExampleContact",
R"()",
        optionsSubGroup::kAlwaysShowDescription,
        this);
    
    appendOptionsSubGroup (contactSubGroup);
  
    contactSubGroup->
      appendOptionsItem (
        msrStreamsExampleOptionsContactItem::create (
          "c", "contact",
R"(Display information about how to contacct msrStreamsExample maintainers and exit.)"));
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
          "hmseof", "helpmsrStreamsExampleOutputFile",
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

void msrStreamsExampleOptions::printMsrStreamsExampleOptionsValues (int fieldWidth)
{  
  gLogIOstream <<
    "The msrStreamsExample options are:" <<
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

S_optionsItem msrStreamsExampleOptions::handleOptionsItem (
  ostream&      os,
  S_optionsItem item)
{
  S_optionsItem result;
  
  if (gTraceOptions->fTraceOptions) {
    os <<
      "---> Handling options item '" <<
      item <<
      "'" <<
      endl;
  }

  if (
    // version item?
    S_msrStreamsExampleOptionsVersionItem
      versionItem =
        dynamic_cast<msrStreamsExampleOptionsVersionItem*>(&(*item))
    ) {
    if (gTraceOptions->fTraceOptions) {
      os <<
        "==> optionsItem is of type 'optionsVersionItem'" <<
        endl;
    }

    // handle it at once
    versionItem->
      printVersion (os);

    // exit
    exit (0);
  }
  
  else if (
    // about item?
    S_msrStreamsExampleOptionsAboutItem
      aboutItem =
        dynamic_cast<msrStreamsExampleOptionsAboutItem*>(&(*item))
    ) {
    if (gTraceOptions->fTraceOptions) {
      os <<
        "==> optionsItem is of type 'optionsAboutItem'" <<
        endl;
    }

    // handle it at once
    aboutItem->
      printAbout (os);

    // exit
    exit (0);
  }
  
  else if (
    // contact item?
    S_msrStreamsExampleOptionsContactItem
      contactItem =
        dynamic_cast<msrStreamsExampleOptionsContactItem*>(&(*item))
    ) {
    if (gTraceOptions->fTraceOptions) {
      os <<
        "==> optionsItem is of type 'optionsContactItem'" <<
        endl;
    }

    // handle it at once
    contactItem->
      printContact (os);

    // exit
    exit (0);
  }

  return result;
}

//______________________________________________________________________________
void initializeMsrStreamsExampleOptionsHandling (
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
  
  gMsrStreamsExampleOptions = msrStreamsExampleOptions::create (
    optionsHandler);
  assert (gMsrStreamsExampleOptions != 0);
}


}
