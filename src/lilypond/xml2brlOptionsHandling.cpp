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

#include "xml2brlOptionsHandling.h"

#include "generalOptions.h"

#include "setTraceOptionsIfDesired.h"
#include "traceOptions.h"

#include "musicXMLOptions.h"
#include "msrOptions.h"
#include "bsrOptions.h"
#include "brailleOptions.h"

#include "extraOptions.h"

#include "version.h"
#include "utilities.h"

#include "msr.h"
#include "bsr.h"

using namespace std;

namespace MusicXML2
{

//______________________________________________________________________________
S_xml2brlOptionsVersionItem xml2brlOptionsVersionItem::create (
  string shortName,
  string longName,
  string description)
{
  xml2brlOptionsVersionItem* o = new
    xml2brlOptionsVersionItem (
      shortName,
      longName,
      description);
  assert(o!=0);
  return o;
}

xml2brlOptionsVersionItem::xml2brlOptionsVersionItem (
  string shortName,
  string longName,
  string description)
  : oahAtom (
      shortName,
      longName,
      description)
{}

xml2brlOptionsVersionItem::~xml2brlOptionsVersionItem ()
{}

void xml2brlOptionsVersionItem::print (ostream& os) const
{
  const int fieldWidth = K_OPTIONS_FIELD_WIDTH;

  os <<
    "xml2brlOptionsVersionItem:" <<
    endl;

  gIndenter++;

  oahElement::printElementEssentials (
    os, fieldWidth);

  gIndenter++;
  os <<
    gIndenter.indentMultiLineString (
      fDescription) <<
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
  string shortName,
  string longName,
  string description)
{
  xml2brlOptionsAboutItem* o = new
    xml2brlOptionsAboutItem (
      shortName,
      longName,
      description);
  assert(o!=0);
  return o;
}

xml2brlOptionsAboutItem::xml2brlOptionsAboutItem (
  string shortName,
  string longName,
  string description)
  : oahAtom (
      shortName,
      longName,
      description)
{}

xml2brlOptionsAboutItem::~xml2brlOptionsAboutItem ()
{}

void xml2brlOptionsAboutItem::print (ostream& os) const
{
  const int fieldWidth = K_OPTIONS_FIELD_WIDTH;

  os <<
    "xml2brlOptionsAboutItem:" <<
    endl;

  gIndenter++;

  oahElement::printElementEssentials (
    os, fieldWidth);

  gIndenter++;
  os <<
    gIndenter.indentMultiLineString (
      fDescription) <<
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
  string shortName,
  string longName,
  string description)
{
  xml2brlOptionsContactItem* o = new
    xml2brlOptionsContactItem (
      shortName,
      longName,
      description);
  assert(o!=0);
  return o;
}

xml2brlOptionsContactItem::xml2brlOptionsContactItem (
  string shortName,
  string longName,
  string description)
  : oahAtom (
      shortName,
      longName,
      description)
{}

xml2brlOptionsContactItem::~xml2brlOptionsContactItem ()
{}

void xml2brlOptionsContactItem::print (ostream& os) const
{
  const int fieldWidth = K_OPTIONS_FIELD_WIDTH;

  os <<
    "xml2brlOptionsContactItem:" <<
    endl;

  gIndenter++;

  oahElement::printElementEssentials (
    os, fieldWidth);

  gIndenter++;
  os <<
    gIndenter.indentMultiLineString (
      fDescription) <<
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
  : oahHandler (
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
Options can be written with '-' or '--' at will,
  even though the help information below is presented with '-'.
Option '-h, -help' prints the full help,
  while '-hs, -helpSummary' only prints a help summary.)",
    ios)
{
  initializeOptionsHandler (executableName);

  // append the help options prefixes
  S_oahPrefix
    helpPrefix =
      oahPrefix::create (
        "help",
        "help-",
        "'-help=abc,xywx-yz' is equivalent to '-help-abc, -help-xywx-yz'");
  appendPrefixToHandler (helpPrefix);

  S_oahPrefix
    hPrefix =
      oahPrefix::create (
        "h",
        "h",
        "'-h=abc,wxyz' is equivalent to '-habc, -hwxyz'");
  appendPrefixToHandler (hPrefix);

  // append the trace options prefixes
  S_oahPrefix
    tracePrefix =
      oahPrefix::create (
        "trace",
        "trace-",
        "'-trace=abc,xywx-yz' is equivalent to '-trace-abc, -trace-xywx-yz'");
  appendPrefixToHandler (tracePrefix);

  S_oahPrefix
    tPrefix =
      oahPrefix::create (
        "t",
        "t",
        "'-t=abc,wxyz' is equivalent to '-tabc, -twxyz'");
  appendPrefixToHandler (tPrefix);
}

xml2brlOptionsHandler::~xml2brlOptionsHandler ()
{}

void xml2brlOptionsHandler::initializeOptionsHandler (
  string executableName)
{
  // initialize options handling, phase 1
  // ------------------------------------------------------

#ifdef TRACE_OPTIONS
  initializeTraceOptionsHandling (
    this);
#endif

  initializeGeneralOptionsHandling (
    executableName,
    this);

  // initialize the library
  // ------------------------------------------------------

  initializeMSR ();
  initializeBSR ();

  // initialize options handling, phase 2
  // ------------------------------------------------------

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
    fHandlerLogIOstream <<
      "xml2brlOptionsHandler has been initialized as:" <<
      endl;

    gIndenter++;

    print (
      fHandlerLogIOstream);
    fHandlerLogIOstream <<
      endl <<
      endl;

    gIndenter--;
  }
#endif

  // register options handler in itself,
  // so that the 'global' help options can be handled
  this->
    registerHandlerInItself ();

#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceOptions && ! gGeneralOptions->fQuiet) {
    fHandlerLogIOstream <<
      "xml2brlOptionsHandler help:" <<
      endl;

    this->
      printHelp (
        fHandlerLogIOstream);
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
      fHandlerLogIOstream <<
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
        fHandlerLogIOstream <<
          i << " : " << fArgumentsVector [i] <<
            endl;
      } // for

      fHandlerLogIOstream << endl;

      gIndenter--;
    }
    else {
      fHandlerLogIOstream <<
        "There are no arguments to " <<
        gGeneralOptions->fExecutableName <<
        endl;
    }
  }
#endif

  // input source name
  // ------------------------------------------------------

  switch (argumentsNumber) {
    case 0:
      fHandlerLogIOstream <<
        endl <<
        "Input file name or '-' for standard input expected" <<
        endl <<
        endl;

      exit (1);
      break;

    case 1:
      // register intput file name
      gGeneralOptions->fInputSourceName =
        fArgumentsVector [0];
      break;

    default:
      fHandlerLogIOstream <<
        endl <<
        "Several input file name supplied, only the first one, \"" <<
        fArgumentsVector [0] <<
        "\", will be translated" <<
        endl <<
        endl;

      // register intput file name
      gGeneralOptions->fInputSourceName =
        fArgumentsVector [0];
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
    "fShortName" << " : " << fShortName <<
    endl <<
    setw (fieldWidth) <<
    "fLongName" << " : " << fLongName <<
    endl <<
    setw (fieldWidth) <<
    "fDescription" << " : " << fDescription <<
    endl <<
    endl;

  os <<
    "Options groups (" <<
    singularOrPlural (
      fHandlerGroupsList.size (), "element",  "elements") <<
    "):" <<
    endl;

  if (fHandlerGroupsList.size ()) {
    os << endl;

    gIndenter++;

    list<S_oahGroup>::const_iterator
      iBegin = fHandlerGroupsList.begin (),
      iEnd   = fHandlerGroupsList.end (),
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
  S_oahHandler oahHandler)
{
  xml2brlOptions* o = new xml2brlOptions (
    oahHandler);
  assert(o!=0);

  return o;
}

xml2brlOptions::xml2brlOptions (
  S_oahHandler oahHandler)
  : oahGroup (
    "xml2brl",
    "hb", "help-xml2brl",
R"(Options that are used by xml2brl are grouped here.)",
    oahHandler)
{
  // append this options group to the options handler
  // if relevant
  if (oahHandler) {
    oahHandler->
      appendGroupToHandler (this);
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
    S_oahSubGroup
      versionSubGroup =
        oahSubGroup::create (
          "Version",
          "hxv", "help-xml2brl-version",
R"()",
        oahSubGroup::kSubGroupVisibilityAlways,
        this);

    appendSubGroup (versionSubGroup);

    // version

    versionSubGroup->
      appendAtom (
        xml2brlOptionsVersionItem::create (
          "v", "version",
R"(Display xml2brl's version number and history and exit.)"));
  }


  // about
  // --------------------------------------

  {
    S_oahSubGroup
      aboutSubGroup =
        oahSubGroup::create (
          "About",
          "hxa", "help-xml2brl-about",
R"()",
        oahSubGroup::kSubGroupVisibilityAlways,
        this);

    appendSubGroup (aboutSubGroup);

    // about

    aboutSubGroup->
      appendAtom (
        xml2brlOptionsAboutItem::create (
          "a", "about",
R"(Display information about xml2brl and exit.)"));
  }


  // contact
  // --------------------------------------

  {
    S_oahSubGroup
      contactSubGroup =
        oahSubGroup::create (
          "Contact",
          "hxc", "help-xml2brl-contact",
R"()",
        oahSubGroup::kSubGroupVisibilityAlways,
        this);

    appendSubGroup (contactSubGroup);

    // contact

    contactSubGroup->
      appendAtom (
        xml2brlOptionsContactItem::create (
          "c", "contact",
R"(Display information about how to contacct xml2brl maintainers and exit.)"));
  }


  // output file
  // --------------------------------------

  {
    S_oahSubGroup
      outputFileSubGroup =
        oahSubGroup::create (
          "Output file",
          "hxof", "help-xml2brl-output-file",
R"()",
        oahSubGroup::kSubGroupVisibilityAlways,
        this);

    appendSubGroup (outputFileSubGroup);

    // output filename

    outputFileSubGroup->
      appendAtom (
        oahStringAtom::create (
          "o", "output-filename",
R"(Write Braille music to file FILENAME instead of standard output.)",
          "FILENAME",
          "outputFileName",
          fOutputFileName));

    // auto output filename

    fAutoOutputFile = false;

    outputFileSubGroup->
      appendAtom (
        oahBooleanAtom::create (
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

S_oahValuedAtom xml2brlOptions::handleAtom (
  ostream&      os,
  S_oahAtom item)
{
  S_oahValuedAtom result;

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
        "==> oahAtom is of type 'optionsVersionItem'" <<
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
        "==> oahAtom is of type 'optionsAboutItem'" <<
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
        "==> oahAtom is of type 'optionsContactItem'" <<
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
  S_oahHandler oahHandler)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceOptions && ! gGeneralOptions->fQuiet) {
    gLogIOstream <<
      "Initializing xml2brl options handling" <<
      endl;
  }
#endif

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
    oahHandler);
  assert (gXml2brlOptions != 0);
}


}
