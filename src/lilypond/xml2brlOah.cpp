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

#include "xml2brlOah.h"

#include "generalOah.h"

#include "setTraceOahIfDesired.h"
#include "traceOah.h"

#include "executableOah.h"
#include "musicXMLOah.h"
#include "msrOah.h"
#include "bsrOah.h"
#include "brailleOah.h"

#include "extraOah.h"

#include "version.h"
#include "utilities.h"

#include "msr.h"
#include "bsr.h"

using namespace std;

namespace MusicXML2
{

//______________________________________________________________________________
S_xml2brlOahVersionAtom xml2brlOahVersionAtom::create (
  string shortName,
  string longName,
  string description)
{
  xml2brlOahVersionAtom* o = new
    xml2brlOahVersionAtom (
      shortName,
      longName,
      description);
  assert(o!=0);
  return o;
}

xml2brlOahVersionAtom::xml2brlOahVersionAtom (
  string shortName,
  string longName,
  string description)
  : oahAtom (
      shortName,
      longName,
      description)
{}

xml2brlOahVersionAtom::~xml2brlOahVersionAtom ()
{}

void xml2brlOahVersionAtom::print (ostream& os) const
{
  const int fieldWidth = K_OPTIONS_FIELD_WIDTH;

  os <<
    "xml2brlOahVersionAtom:" <<
    endl;

  gIndenter++;

  oahOption::printOptionEssentials (
    os, fieldWidth);

  gIndenter++;
  os <<
    gIndenter.indentMultiLineString (
      fDescription) <<
    endl;
  gIndenter--;

  gIndenter--;
}

void xml2brlOahVersionAtom::printVersion (ostream& os) const
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

void xml2brlOahVersionAtom::printAtomOptionsValues (
  ostream& os,
  int      valueFieldWidth) const
{
  // nothing to print here
}

ostream& operator<< (ostream& os, const S_xml2brlOahVersionAtom& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_xml2brlOahAboutAtom xml2brlOahAboutAtom::create (
  string shortName,
  string longName,
  string description)
{
  xml2brlOahAboutAtom* o = new
    xml2brlOahAboutAtom (
      shortName,
      longName,
      description);
  assert(o!=0);
  return o;
}

xml2brlOahAboutAtom::xml2brlOahAboutAtom (
  string shortName,
  string longName,
  string description)
  : oahAtom (
      shortName,
      longName,
      description)
{}

xml2brlOahAboutAtom::~xml2brlOahAboutAtom ()
{}

void xml2brlOahAboutAtom::print (ostream& os) const
{
  const int fieldWidth = K_OPTIONS_FIELD_WIDTH;

  os <<
    "xml2brlOahAboutAtom:" <<
    endl;

  gIndenter++;

  oahOption::printOptionEssentials (
    os, fieldWidth);

  gIndenter++;
  os <<
    gIndenter.indentMultiLineString (
      fDescription) <<
    endl;
  gIndenter--;

  gIndenter--;
}

void xml2brlOahAboutAtom::printAbout (ostream& os) const
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

void xml2brlOahAboutAtom::printAtomOptionsValues (
  ostream& os,
  int      valueFieldWidth) const
{
  // nothing to print here
}

ostream& operator<< (ostream& os, const S_xml2brlOahAboutAtom& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_xml2brlOahContactAtom xml2brlOahContactAtom::create (
  string shortName,
  string longName,
  string description)
{
  xml2brlOahContactAtom* o = new
    xml2brlOahContactAtom (
      shortName,
      longName,
      description);
  assert(o!=0);
  return o;
}

xml2brlOahContactAtom::xml2brlOahContactAtom (
  string shortName,
  string longName,
  string description)
  : oahAtom (
      shortName,
      longName,
      description)
{}

xml2brlOahContactAtom::~xml2brlOahContactAtom ()
{}

void xml2brlOahContactAtom::print (ostream& os) const
{
  const int fieldWidth = K_OPTIONS_FIELD_WIDTH;

  os <<
    "xml2brlOahContactAtom:" <<
    endl;

  gIndenter++;

  oahOption::printOptionEssentials (
    os, fieldWidth);

  gIndenter++;
  os <<
    gIndenter.indentMultiLineString (
      fDescription) <<
    endl;
  gIndenter--;

  gIndenter--;
}

void xml2brlOahContactAtom::printContact (ostream& os) const
{
  os <<
    endl <<
R"(To contact xml2brl maintainers:

    Send a mail to mailto:lilypond-user@gnu.org describing the problem
    and error messages you obtain if relevant.)" <<
    endl <<
    endl;
}

void xml2brlOahContactAtom::printAtomOptionsValues (
  ostream& os,
  int      valueFieldWidth) const
{
  // nothing to print here
}

ostream& operator<< (ostream& os, const S_xml2brlOahContactAtom& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_xml2brlOahHandler xml2brlOahHandler::create (
  string           executableName,
  indentedOstream& ios)
{
  xml2brlOahHandler* o = new
    xml2brlOahHandler (
      executableName,
      ios);
  assert(o!=0);
  return o;
}

xml2brlOahHandler::xml2brlOahHandler (
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

xml2brlOahHandler::~xml2brlOahHandler ()
{}

void xml2brlOahHandler::initializeOptionsHandler (
  string executableName)
{
  // initialize options handling, phase 1
  // ------------------------------------------------------

#ifdef TRACE_OAH
  initializeTraceOahHandling (
    this);
#endif

  initializeExecutableOahHandling (
    executableName,
    this);

  initializeGeneralOahHandling (
    this);

  // initialize the library
  // ------------------------------------------------------

  initializeMSR ();
  initializeBSR ();

  // initialize options handling, phase 2
  // ------------------------------------------------------

  initializeMusicXMLOahHandling (
    this);

  initializeMsrOahHandling (
    this);

  initializeBsrOahHandling (
    this);

  initializeBrailleOahHandling (
    this);

#ifdef EXTRA_OAH
  initializeExtraOahHandling (
    this);
#endif

  initializeXml2brlOah (
    this);

#ifdef TRACE_OAH
  if (gExecutableOah->fTraceOah && ! gGeneralOah->fQuiet) {
    // print the options handler initial state
    fHandlerLogOstream <<
      "xml2brlOahHandler has been initialized as:" <<
      endl;

    gIndenter++;

    print (
      fHandlerLogOstream);
    fHandlerLogOstream <<
      endl <<
      endl;

    gIndenter--;
  }
#endif

  // register options handler in itself,
  // so that the 'global' help options can be handled
  this->
    registerHandlerInItself ();

#ifdef TRACE_OAH
  if (gExecutableOah->fTraceOah && ! gGeneralOah->fQuiet) {
    fHandlerLogOstream <<
      "xml2brlOahHandler help:" <<
      endl;

    this->
      printHelp (
        fHandlerLogOstream);
  }
#endif
}

void xml2brlOahHandler::checkOptionsAndArguments ()
{
  unsigned int argumentsNumber =
    fHandlerArgumentsVector.size ();

#ifdef TRACE_OAH
  if (gExecutableOah->fTraceOah && ! gGeneralOah->fQuiet) {
    if (argumentsNumber > 0) {
      fHandlerLogOstream <<
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
        fHandlerLogOstream <<
          i << " : " << fHandlerArgumentsVector [i] <<
            endl;
      } // for

      fHandlerLogOstream << endl;

      gIndenter--;
    }
    else {
      fHandlerLogOstream <<
        "There are no arguments to " <<
        gExecutableOah->fHandlerExecutableName <<
        endl;
    }
  }
#endif

  // input source name
  // ------------------------------------------------------

  switch (argumentsNumber) {
    case 0:
      fHandlerLogOstream <<
        endl <<
        "Input file name or '-' for standard input expected" <<
        endl <<
        endl;

      exit (1);
      break;

    case 1:
      // register intput file name
      gExecutableOah->fInputSourceName =
        fHandlerArgumentsVector [0];
      break;

    default:
      fHandlerLogOstream <<
        endl <<
        "Several input file name supplied, only the first one, \"" <<
        fHandlerArgumentsVector [0] <<
        "\", will be translated" <<
        endl <<
        endl;

      // register intput file name
      gExecutableOah->fInputSourceName =
        fHandlerArgumentsVector [0];
      break;
  } //  switch

  // build potential output file name
  // ------------------------------------------------------

  string potentialOutputFileName;

  if (gExecutableOah->fInputSourceName != "-") {
    // determine potential output file name,
    // may be set differently by '--ofn, --outputFileName' option
    potentialOutputFileName =
      baseName (
        gExecutableOah->fInputSourceName);

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
    if (gBrailleOah->fUseEncodingInFileName) {
      switch (gBrailleOah->fUTFKind) {
        case kUTF8:
          potentialOutputFileName +=
            "_UTF8";
          switch (gBrailleOah->fByteOrderingKind) {
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
          switch (gBrailleOah->fByteOrderingKind) {
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

  if (gXml2brlOah->fAutoOutputFile) {
    if (gXml2brlOah->fOutputFileName.size ()) {
      stringstream s;

      s <<
        "options '-aofn, -auto-output-file-name' and '-o, -output-file-name'"  <<
        endl <<
        "cannot be chosen simultaneously";

      optionError (s.str ());

      exit (3);
    }

    else if (gExecutableOah->fInputSourceName == "-") {
      stringstream s;

      s <<
        "option '-aofn, -auto-output-file-name'"  <<
        endl <<
        "cannot be used when reading from standard input";

      optionError (s.str ());

      exit (4);
    }

    else {
      gXml2brlOah->fOutputFileName =
        potentialOutputFileName;
    }
  }

  // register command line informations in gGeneralOah
  // ------------------------------------------------------

/* JMI
  gGeneralOah->fCommandLineWithShortOptions =
    fCommandLineWithShortOptions;
  gGeneralOah->fCommandLineWithLongOptions =
    fCommandLineWithLongOptions;
    */
}

//______________________________________________________________________________
void xml2brlOahHandler::enforceOahHandlerQuietness ()
{
#ifdef TRACE_OAH
  gGeneralOah->
    enforceQuietness ();
#endif

  gGeneralOah->
    enforceQuietness ();

  gMusicXMLOah->
    enforceQuietness ();

  gMsrOah->
    enforceQuietness ();

  gBsrOah->
    enforceQuietness ();

  gBrailleOah->
    enforceQuietness ();

#ifdef EXTRA_OAH
  gExtraOah->
    enforceQuietness ();
#endif

  gXml2brlOah->
    enforceQuietness ();
}

//______________________________________________________________________________
void xml2brlOah::enforceQuietness ()
{}

//______________________________________________________________________________
void xml2brlOah::checkOptionsConsistency ()
{
  gLogOstream <<
    "xml2brlOah::checkOptionsConsistency ()" <<
    endl;

#ifdef TRACE_OAH
  gGeneralOah->
    checkOptionsConsistency ();
#endif

  gGeneralOah->
    checkOptionsConsistency ();

  gMusicXMLOah->
    checkOptionsConsistency ();

  gMsrOah->
    checkOptionsConsistency ();

  gBsrOah->
    checkOptionsConsistency ();

  gBrailleOah->
    checkOptionsConsistency ();

#ifdef EXTRA_OAH
  gExtraOah->
    checkOptionsConsistency ();
#endif

// JMI???  gXml2brlOah-> // TICINO
//    checkOptionsConsistency ();
}

//______________________________________________________________________________
void xml2brlOahHandler::print (ostream& os) const
{
  const int fieldWidth = 27;

  os <<
    "xml2brlOahHandler:" <<
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

ostream& operator<< (ostream& os, const S_xml2brlOahHandler& elt)
{
  elt->print (os);
  return os;
}

//_______________________________________________________________________________
S_xml2brlOah gXml2brlOah;

S_xml2brlOah xml2brlOah::create (
  S_oahHandler handler)
{
  xml2brlOah* o = new xml2brlOah (
    handler);
  assert(o!=0);

  return o;
}

xml2brlOah::xml2brlOah (
  S_oahHandler handler)
  : oahGroup (
    "xml2brl",
    "hxb", "help-xml2brl",
R"(Options that are used by xml2brl are grouped here.)",
    kElementVisibilityAlways,
    handler)
{
  // append this options group to the options handler
  // if relevant
  if (handler) {
    handler->
      appendGroupToHandler (this);
  }

  // initialize it
  initializeXml2brlOah ();
}

xml2brlOah::~xml2brlOah ()
{}

void xml2brlOah::initializeXml2brlOah ()
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
        kElementVisibilityAlways,
        this);

    appendSubGroup (versionSubGroup);

    // version

    versionSubGroup->
      appendAtom (
        xml2brlOahVersionAtom::create (
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
        kElementVisibilityAlways,
        this);

    appendSubGroup (aboutSubGroup);

    // about

    aboutSubGroup->
      appendAtom (
        xml2brlOahAboutAtom::create (
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
        kElementVisibilityAlways,
        this);

    appendSubGroup (contactSubGroup);

    // contact

    contactSubGroup->
      appendAtom (
        xml2brlOahContactAtom::create (
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
        kElementVisibilityAlways,
        this);

    appendSubGroup (outputFileSubGroup);

    // output filename

    outputFileSubGroup->
      appendAtom (
        oahStringAtom::create (
          "o", "output-file-name",
R"(Write Braille music to file FILENAME instead of standard output.)",
          "FILENAME",
          "outputFileName",
          fOutputFileName));

    // auto output filename

    fAutoOutputFile = false;

    outputFileSubGroup->
      appendAtom (
        oahBooleanAtom::create (
          "aofn", "auto-output-file-name",
R"(This option can only be used when reading from a file.
Write Braille music to a file in the current working directory.
The file name is derived from that of the input file,
replacing any suffix after the the '.' by 'ly'
or adding '.brl' if none is present.)",
          "autoOutputFileName",
          fAutoOutputFile));
  }
}

void xml2brlOah::printXml2brlOahValues (int fieldWidth)
{
  gLogOstream <<
    "The xml2brl options are:" <<
    endl;

  gIndenter++;

  // output file
  // --------------------------------------

  gLogOstream << left <<
    setw (fieldWidth) << "Output file:" <<
    endl;

  gIndenter++;

  gLogOstream << left <<
    setw (fieldWidth) << "outputFileName" << " : \"" <<
    fOutputFileName <<
    "\"" <<
    endl;

  gIndenter--;
}

//______________________________________________________________________________
void initializeXml2brlOah (
  S_oahHandler handler)
{
#ifdef TRACE_OAH
  if (gExecutableOah->fTraceOah && ! gGeneralOah->fQuiet) {
    gLogOstream <<
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

  gXml2brlOah = xml2brlOah::create (
    handler);
  assert (gXml2brlOah != 0);
}


}
