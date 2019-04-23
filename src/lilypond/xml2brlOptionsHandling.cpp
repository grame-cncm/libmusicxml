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

#include <climits>      // INT_MIN, INT_MAX
#include <iomanip>      // setw, setprecision, ...

#include "libmusicxml.h"

#include "version.h"
#include "utilities.h"

#include "xml2brlOptionsHandling.h"

#include "generalOptions.h"

#include "setTraceOptionsIfDesired.h"
#ifdef TRACE_OPTIONS
  #include "traceOptions.h"
#endif

#include "musicXMLOptions.h"
#include "msrOptions.h"
#include "bsrOptions.h"
#include "brailleOptions.h"

#ifdef EXTRA_OPTIONS
  #include "extraOptions.h"
#endif


using namespace std;

namespace MusicXML2
{

//______________________________________________________________________________
S_xml2brlOptionsVersionItem xml2brlOptionsVersionItem::create (
  string optionsItemShortName,
  string optionsItemLongName,
  string optionsItemDescription)
{
  xml2brlOptionsVersionItem* o = new
    xml2brlOptionsVersionItem (
      optionsItemShortName,
      optionsItemLongName,
      optionsItemDescription);
  assert(o!=0);
  return o;
}

xml2brlOptionsVersionItem::xml2brlOptionsVersionItem (
  string optionsItemShortName,
  string optionsItemLongName,
  string optionsItemDescription)
  : optionsItem (
      optionsItemShortName,
      optionsItemLongName,
      optionsItemDescription)
{}

xml2brlOptionsVersionItem::~xml2brlOptionsVersionItem ()
{}

void xml2brlOptionsVersionItem::print (ostream& os) const
{
  const int fieldWidth = K_FIELD_WIDTH;

  os <<
    "xml2brlOptionsVersionItem:" <<
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

void xml2brlOptionsVersionItem::printVersion (ostream& os) const
{
  os <<
    endl <<
    "This is xml2brl" <<
    " version " << currentVersionNumber () <<
    endl <<
    endl;

  // print versions history
  printVersionsHistory (os);
}

void xml2brlOptionsVersionItem::printOptionsValues (
  ostream& os,
  int      valueFieldWidth) const
{
  // nothing to print here
}

ostream& operator<< (ostream& os, const S_xml2brlOptionsVersionItem& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_xml2brlOptionsAboutItem xml2brlOptionsAboutItem::create (
  string optionsItemShortName,
  string optionsItemLongName,
  string optionsItemDescription)
{
  xml2brlOptionsAboutItem* o = new
    xml2brlOptionsAboutItem (
      optionsItemShortName,
      optionsItemLongName,
      optionsItemDescription);
  assert(o!=0);
  return o;
}

xml2brlOptionsAboutItem::xml2brlOptionsAboutItem (
  string optionsItemShortName,
  string optionsItemLongName,
  string optionsItemDescription)
  : optionsItem (
      optionsItemShortName,
      optionsItemLongName,
      optionsItemDescription)
{}

xml2brlOptionsAboutItem::~xml2brlOptionsAboutItem ()
{}

void xml2brlOptionsAboutItem::print (ostream& os) const
{
  const int fieldWidth = K_FIELD_WIDTH;

  os <<
    "xml2brlOptionsAboutItem:" <<
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

void xml2brlOptionsAboutItem::printAbout (ostream& os) const
{
  os <<
    endl <<
R"(What xml2brl does:

    This multi-pass translator basically performs 6 passes:
        Pass 1:  reads the contents of MusicXMLFile or stdin ('-')
                 and converts it to a MusicXML tree;
        Pass 2a: converts that MusicXML tree into to
                 a Music Score Representation (MSR) skeleton;
        Pass 2b: converts that tree and the skeleton into a
                 Music Score Representation (MSR);
        Pass 3a: converts the MSR into a
                 Braille Score Representation (BSR)
                 containing one Braille page per MusicXML page;
        Pass 3b: converts the BSR into to another BSR
                 with as many Braille pages as needed
                 to fit the line and page lengthes;
        Pass 4:  converts the BSR to Unicode text
                 and writes it to standard output.

    In this preliminary version, pass 3b merely clones the BSR it receives.

    Other passes are performed according to the options, such as
    printing views of the internal data or printing a summary of the score.

    The activity log and warning/error messages go to standard error.)" <<
    endl <<
    endl;
}

void xml2brlOptionsAboutItem::printOptionsValues (
  ostream& os,
  int      valueFieldWidth) const
{
  // nothing to print here
}

ostream& operator<< (ostream& os, const S_xml2brlOptionsAboutItem& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_xml2brlOptionsContactItem xml2brlOptionsContactItem::create (
  string optionsItemShortName,
  string optionsItemLongName,
  string optionsItemDescription)
{
  xml2brlOptionsContactItem* o = new
    xml2brlOptionsContactItem (
      optionsItemShortName,
      optionsItemLongName,
      optionsItemDescription);
  assert(o!=0);
  return o;
}

xml2brlOptionsContactItem::xml2brlOptionsContactItem (
  string optionsItemShortName,
  string optionsItemLongName,
  string optionsItemDescription)
  : optionsItem (
      optionsItemShortName,
      optionsItemLongName,
      optionsItemDescription)
{}

xml2brlOptionsContactItem::~xml2brlOptionsContactItem ()
{}

void xml2brlOptionsContactItem::print (ostream& os) const
{
  const int fieldWidth = K_FIELD_WIDTH;

  os <<
    "xml2brlOptionsContactItem:" <<
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

void xml2brlOptionsContactItem::printContact (ostream& os) const
{
  os <<
    endl <<
R"(To contact xml2brl maintainers:

    Send a mail to mailto:lilypond-user@gnu.org describing the problem
    and error messages you obtain if relevant.)" <<
    endl <<
    endl;
}

void xml2brlOptionsContactItem::printOptionsValues (
  ostream& os,
  int      valueFieldWidth) const
{
  // nothing to print here
}

ostream& operator<< (ostream& os, const S_xml2brlOptionsContactItem& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_xml2brlOptionsHandler xml2brlOptionsHandler::create (
  string           executableName,
  indentedOstream& ios)
{
  xml2brlOptionsHandler* o = new
    xml2brlOptionsHandler (
      executableName,
      ios);
  assert(o!=0);
  return o;
}

xml2brlOptionsHandler::xml2brlOptionsHandler (
  string           executableName,
  indentedOstream& ios)
  : optionsHandler (
    "Available options",
    "Options values",
    "h", "help",
    "hs", "helpSummary",
R"(
                      Welcome to xml2brl,
            the MusicXML to Braille music translator
          delivered as part of the libmusicxml2 library.
      https://github.com/grame-cncm/libmusicxml/tree/lilypond

Usage:
    xml2brl [options] [MusicXMLFile|-] [options]
)",
R"(
Option '-h, -help' prints the full help,
  while '-hs, -helpSummary' only prints a help summary.)",
    ios
    )
{
  initializeOptionsHandler (executableName);
}

xml2brlOptionsHandler::~xml2brlOptionsHandler ()
{}

void xml2brlOptionsHandler::initializeOptionsHandler (
  string executableName)
{
  // initialize options handling
  // ------------------------------------------------------

  initializeGeneralOptionsHandling (
    executableName,
    this);

#ifdef TRACE_OPTIONS
  initializeTraceOptionsHandling (
    this);
#endif

  initializeMusicXMLOptionsHandling (
    this);

  initializeMsrOptionsHandling (
    this);

  initializeBsrOptionsHandling (
    this);

  initializeBrailleOptionsHandling (
    this);

#ifdef EXTRA_OPTIONS
  initializeExtraOptionsHandling (
    this);
#endif

  initializeXml2brlOptionsHandling (
    this);

#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceOptions && ! gGeneralOptions->fQuiet) {
    // print the options handler initial state
    fOptionsHandlerLogIOstream <<
      "xml2brlOptionsHandler has been initialized as:" <<
      endl;

    gIndenter++;

    print (
      fOptionsHandlerLogIOstream);
    fOptionsHandlerLogIOstream <<
      endl <<
      endl;

    gIndenter--;
  }
#endif

  // register options handler in itself,
  // so that the 'global' help options can be handled
  this->
    registerOptionsHandlerInItself ();

#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceOptions && ! gGeneralOptions->fQuiet) {
    fOptionsHandlerLogIOstream <<
      "xml2brlOptionsHandler help:" <<
      endl;

    this->
      printHelp (
        fOptionsHandlerLogIOstream);
  }
#endif
}

void xml2brlOptionsHandler::checkOptionsAndArguments ()
{
  unsigned int argumentsNumber =
    fArgumentsVector.size ();

#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceOptions && ! gGeneralOptions->fQuiet) {
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
#endif

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

    size_t
      posInString =
        potentialOutputFileName.rfind ('.');

    // remove file extension
    if (posInString != string::npos) {
      potentialOutputFileName.replace (
        posInString,
        potentialOutputFileName.size () - posInString,
        "");
    }

    // should encoding be used by the file name?
    if (gBrailleOptions->fUseEncodingInFileName) {
      switch (gBrailleOptions->fUTFKind) {
        case kUTF8:
          potentialOutputFileName +=
            "_UTF8";
          switch (gBrailleOptions->fByteOrderingKind) {
            case kByteOrderingNone:
              break;
            case kByteOrderingBigEndian:
              potentialOutputFileName +=
                "_BE";
              break;
            case kByteOrderingSmallEndian:
              // should not occur JMI
              break;
          } // switch
          break;

        case kUTF16:
          potentialOutputFileName +=
            "_UTF16";
          switch (gBrailleOptions->fByteOrderingKind) {
            case kByteOrderingNone:
              break;

            case kByteOrderingBigEndian:
              potentialOutputFileName +=
                "_BE";
              break;

            case kByteOrderingSmallEndian:
              potentialOutputFileName +=
                "_SE";
              break;
          } // switch
          break;
      } // switch
    }

    // append '.txt' extension
    potentialOutputFileName +=
      ".txt";
  }

  // check auto output file option usage
  // ------------------------------------------------------

  if (gXml2brlOptions->fAutoOutputFile) {
    if (gXml2brlOptions->fOutputFileName.size ()) {
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
      gXml2brlOptions->fOutputFileName =
        potentialOutputFileName;
    }
  }

  // register command line informations in gGeneralOptions
  // ------------------------------------------------------

  gGeneralOptions->fCommandLineWithShortOptions =
    fCommandLineWithShortOptions;
  gGeneralOptions->fCommandLineWithLongOptions =
    fCommandLineWithLongOptions;
}

//______________________________________________________________________________
void xml2brlOptionsHandler::enforceOptionsHandlerQuietness ()
{
#ifdef TRACE_OPTIONS
  gGeneralOptions->
    enforceQuietness ();
#endif

  gGeneralOptions->
    enforceQuietness ();

  gMusicXMLOptions->
    enforceQuietness ();

  gMsrOptions->
    enforceQuietness ();

  gBsrOptions->
    enforceQuietness ();

  gBrailleOptions->
    enforceQuietness ();

#ifdef EXTRA_OPTIONS
  gExtraOptions->
    enforceQuietness ();
#endif

  gXml2brlOptions->
    enforceQuietness ();
}

//______________________________________________________________________________
void xml2brlOptions::enforceQuietness ()
{}

//______________________________________________________________________________
void xml2brlOptions::checkOptionsConsistency ()
{
  gLogIOstream <<
    "xml2brlOptions::checkOptionsConsistency ()" <<
    endl;

#ifdef TRACE_OPTIONS
  gGeneralOptions->
    checkOptionsConsistency ();
#endif

  gGeneralOptions->
    checkOptionsConsistency ();

  gMusicXMLOptions->
    checkOptionsConsistency ();

  gMsrOptions->
    checkOptionsConsistency ();

  gBsrOptions->
    checkOptionsConsistency ();

  gBrailleOptions->
    checkOptionsConsistency ();

#ifdef EXTRA_OPTIONS
  gExtraOptions->
    checkOptionsConsistency ();
#endif

// JMI???  gXml2brlOptions-> // TICINO
//    checkOptionsConsistency ();
}

//______________________________________________________________________________
void xml2brlOptionsHandler::print (ostream& os) const
{
  const int fieldWidth = 27;

  os <<
    "xml2brlOptionsHandler:" <<
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

ostream& operator<< (ostream& os, const S_xml2brlOptionsHandler& elt)
{
  elt->print (os);
  return os;
}

//_______________________________________________________________________________
S_xml2brlOptions gXml2brlOptions;

S_xml2brlOptions xml2brlOptions::create (
  S_optionsHandler optionsHandler)
{
  xml2brlOptions* o = new xml2brlOptions (
    optionsHandler);
  assert(o!=0);

  return o;
}

xml2brlOptions::xml2brlOptions (
  S_optionsHandler optionsHandler)
  : optionsGroup (
    "xml2brl",
    "hb", "help-xml2brl",
R"(Options that are used by xml2brl are grouped here.)",
    optionsHandler)
{
  // append this options group to the options handler
  // if relevant
  if (optionsHandler) {
    optionsHandler->
      appendOptionsGroup (this);
  }

  // initialize it
  initializeXml2brlOptions ();
}

xml2brlOptions::~xml2brlOptions ()
{}

void xml2brlOptions::initializeXml2brlOptions ()
{
  // version
  // --------------------------------------

  {
    // variables

    // options

    S_optionsSubGroup
      versionSubGroup =
        optionsSubGroup::create (
          "Version",
          "hxv", "help-xml2brl-version",
R"()",
        optionsSubGroup::kAlwaysShowDescription,
        this);

    appendOptionsSubGroup (versionSubGroup);

    versionSubGroup->
      appendOptionsItem (
        xml2brlOptionsVersionItem::create (
          "v", "version",
R"(Display xml2brl's version number and history and exit.)"));
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
          "hxa", "help-xml2brl-about",
R"()",
        optionsSubGroup::kAlwaysShowDescription,
        this);

    appendOptionsSubGroup (aboutSubGroup);

    aboutSubGroup->
      appendOptionsItem (
        xml2brlOptionsAboutItem::create (
          "a", "about",
R"(Display information about xml2brl and exit.)"));
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
          "hxc", "help-xml2brl-contact",
R"()",
        optionsSubGroup::kAlwaysShowDescription,
        this);

    appendOptionsSubGroup (contactSubGroup);

    contactSubGroup->
      appendOptionsItem (
        xml2brlOptionsContactItem::create (
          "c", "contact",
R"(Display information about how to contacct xml2brl maintainers and exit.)"));
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
          "hxof", "help-xml2brl-output-file",
R"()",
        optionsSubGroup::kAlwaysShowDescription,
        this);

    appendOptionsSubGroup (outputFileSubGroup);

    outputFileSubGroup->
      appendOptionsItem (
        optionsStringItem::create (
          "o", "output-filename",
R"(Write Braille music to file FILENAME instead of standard output.)",
          "FILENAME",
          "outputFileName",
          fOutputFileName));

    outputFileSubGroup->
      appendOptionsItem (
        optionsBooleanItem::create (
          "aof", "auto-output-filename",
R"(This option can only be used when reading from a file.
Write Braille music to a file in the current working directory.
The file name is derived from that of the input file,
replacing any suffix after the the '.' by 'ly'
or adding '.brl' if none is present.)",
          "autoOutputFileName",
          fAutoOutputFile));
  }
}

void xml2brlOptions::printXml2brlOptionsValues (int fieldWidth)
{
  gLogIOstream <<
    "The xml2brl options are:" <<
    endl;

  gIndenter++;

  // output file
  // --------------------------------------

  gLogIOstream << left <<
    setw (fieldWidth) << "Output file:" <<
    endl;

  gIndenter++;

  gLogIOstream << left <<
    setw (fieldWidth) << "outputFileName" << " : \"" <<
    fOutputFileName <<
    "\"" <<
    endl;

  gIndenter--;
}

S_optionsItem xml2brlOptions::handleOptionsItem (
  ostream&      os,
  S_optionsItem item)
{
  S_optionsItem result;

#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceOptions && ! gGeneralOptions->fQuiet) {
    os <<
      "---> Handling options item '" <<
      item <<
      "'" <<
      endl;
  }
#endif

  if (
    // version item?
    S_xml2brlOptionsVersionItem
      versionItem =
        dynamic_cast<xml2brlOptionsVersionItem*>(&(*item))
    ) {
#ifdef TRACE_OPTIONS
    if (gTraceOptions->fTraceOptions && ! gGeneralOptions->fQuiet) {
      os <<
        "==> optionsItem is of type 'optionsVersionItem'" <<
        endl;
    }
#endif

    // handle it at once
    versionItem->
      printVersion (os);

    // exit
    exit (0);
  }

  else if (
    // about item?
    S_xml2brlOptionsAboutItem
      aboutItem =
        dynamic_cast<xml2brlOptionsAboutItem*>(&(*item))
    ) {
#ifdef TRACE_OPTIONS
    if (gTraceOptions->fTraceOptions && ! gGeneralOptions->fQuiet) {
      os <<
        "==> optionsItem is of type 'optionsAboutItem'" <<
        endl;
    }
#endif

    // handle it at once
    aboutItem->
      printAbout (os);

    // exit
    exit (0);
  }

  else if (
    // contact item?
    S_xml2brlOptionsContactItem
      contactItem =
        dynamic_cast<xml2brlOptionsContactItem*>(&(*item))
    ) {
#ifdef TRACE_OPTIONS
    if (gTraceOptions->fTraceOptions && ! gGeneralOptions->fQuiet) {
      os <<
        "==> optionsItem is of type 'optionsContactItem'" <<
        endl;
    }
#endif

    // handle it at once
    contactItem->
      printContact (os);

    // exit
    exit (0);
  }

  return result;
}

//______________________________________________________________________________
void initializeXml2brlOptionsHandling (
  S_optionsHandler optionsHandler)
{
  // enlist versions information
  // ------------------------------------------------------

  enlistVersion (
    "Initial", "october 2018",
    "Derived from xml2ly, with an embryonic BSR");

  enlistVersion (
    musicxml2brailleVersionStr (), "Nobember 2018",
    "First draft version");

  // create the options variables
  // ------------------------------------------------------

  gXml2brlOptions = xml2brlOptions::create (
    optionsHandler);
  assert (gXml2brlOptions != 0);
}


}
