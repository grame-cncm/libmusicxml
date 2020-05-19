/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#include <iomanip>      // setw, setprecision, ...

#include "setTraceOahIfDesired.h"
#include "traceOah.h"

#include "libmusicxml.h"

#include "msr.h"
//#include "lpsr.h" // JMI only if pass5 msr2lpsr is run

#include "oahOah.h"

#include "generalOah.h"

#include "musicxmlOah.h"
#include "mxmlTreeOah.h"
#include "msr2MxmltreeOah.h"
#include "msrOah.h"
#include "msr2MxmltreeOah.h"
#include "mxmlTree2MsrOah.h"
#include "msr2LpsrOah.h" // JMI ???
//#include "lpsrOah.h" // JMI only if pass5 msr2lpsr is run

#include "version.h"

#include "xml2xmlManPageOah.h"

#include "xml2xmlOah.h"

using namespace std;

namespace MusicXML2
{

//______________________________________________________________________________
S_xml2xmlOahHandler xml2xmlOahHandler::create (
  string           executableName,
  indentedOstream& ios)
{
  xml2xmlOahHandler* o = new
    xml2xmlOahHandler (
      executableName,
      ios);
  assert(o!=0);
  return o;
}

xml2xmlOahHandler::xml2xmlOahHandler (
  string           executableName,
  indentedOstream& ios)
  : oahHandler (
    executableName + " available options",
    "Options values",
    "h", "help",
    "hs", "helpSummary",
R"(                      Welcome to xml2xml,
              the MusicXML to MusicXML translator
          delivered as part of the libmusicxml2 library.
      https://github.com/grame-cncm/libmusicxml/tree/lilypond
)",
R"(
xml2xml [options] [MusicXMLFile|-] [options]
)",
R"(
Option '-h, -help' prints the full help,
  while '-hs, -helpSummary' only prints a help summary.)",
    ios)
{
  // create and append the help options prefixes
  S_oahPrefix
    helpPrefix =
      oahPrefix::create (
        "help",
        "help-",
        "'-help=abc,yz' is equivalent to '-help-abc, -help-yz'");
  appendPrefixToHandler (helpPrefix);

  S_oahPrefix
    hPrefix =
      oahPrefix::create (
        "h",
        "h",
        "'-h=abc,wxyz' is equivalent to '-habc, -hwxyz'");
  appendPrefixToHandler (hPrefix);

  // create and append the trace options prefixes
  S_oahPrefix
    tracePrefix =
      oahPrefix::create (
        "trace",
        "trace-",
        "'-trace=abc,yz' is equivalent to '-trace-abc, -trace-yz'");
  appendPrefixToHandler (tracePrefix);

  S_oahPrefix
    tPrefix =
      oahPrefix::create (
        "t",
        "t",
        "'-t=abc,wxyz' is equivalent to '-tabc, -twxyz'");
  appendPrefixToHandler (tPrefix);

  // create and append the omit options prefixes
  S_oahPrefix
    omitPrefix =
      oahPrefix::create (
        "omit",
        "omit-",
        "'-omit=abc,yz' is equivalent to '-omit-abc, -omit-yz'");
  appendPrefixToHandler (omitPrefix);

  S_oahPrefix
    oPrefix =
      oahPrefix::create (
        "o",
        "o",
        "'-o=abc,wxyz' is equivalent to '-oabc, -owxyz'");
  appendPrefixToHandler (oPrefix);

  // create an xml2xmlOah2ManPageGenerator
  S_xml2xmlOah2ManPageGenerator
    generator =
      xml2xmlOah2ManPageGenerator::create (
        this,
        gLogOstream,
        gOutputOstream);

  // initialize the handler only now, since it may use prefixes
  initializeXml2xmlOptionsHandler (
    executableName,
    generator);
}

xml2xmlOahHandler::~xml2xmlOahHandler ()
{}

void xml2xmlOahHandler::initializeXml2xmlOptionsHandler (
  string executableName,
  S_xml2xmlOah2ManPageGenerator
         theOah2ManPageGenerator)
{
  // initialize options handling, phase 1
  // ------------------------------------------------------

#ifdef TRACE_OAH
  initializeTraceOahHandling (
    this);
#endif

  initializeOahOahHandling (
    executableName,
    this);

  initializeGeneralOahHandling (
    this);

  // initialize the library
  // ------------------------------------------------------

  initializeMSR ();
//  initializeLPSR (); // JMI only if pass5 msr2lpsr is run

  // initialize options handling, phase 2
  // ------------------------------------------------------

  initializeMusicxmlOahHandling (
    this);

  initializeMxmlTreeOahHandling (
    this);

  initializeMxmlTree2MsrOahHandling (
    this);

  initializeMsrOahHandling (
    this);

  initializeMsr2LpsrOahHandling (
    this);

  initializeMsr2MxmltreeOahHandling (
    this);

//  initializeLpsrOahHandling ( // JMI only if pass5 msr2lpsr is run
//    this);

#ifdef EXTRA_OAH
  initializeExtraOahHandling (
    this);
#endif

  initializeXml2xmlManPageOahHandling (
    this,
    theOah2ManPageGenerator);

  initializeXml2xmlOah (
    this);

#ifdef TRACE_OAH
  if (gTraceOah->fTraceOah && ! gGeneralOah->fQuiet) {
    // print the options handler initial state
    fHandlerLogOstream <<
      "xml2xmlOahHandler has been initialized as:" <<
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
  if (gTraceOah->fTraceOah && ! gGeneralOah->fQuiet) {
    fHandlerLogOstream <<
      "xml2xmlOahHandler help:" <<
      endl;

    this->
      printHelp (
        fHandlerLogOstream);
  }
#endif
}

void xml2xmlOahHandler::checkOptionsAndArguments ()
{
  unsigned int argumentsNumber =
    fHandlerArgumentsVector.size ();

#ifdef TRACE_OAH
  if (gTraceOah->fTraceOah && ! gGeneralOah->fQuiet) {
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
        gOahOah->fHandlerExecutableName <<
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
      gOahOah->fInputSourceName =
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
      gOahOah->fInputSourceName =
        fHandlerArgumentsVector [0];
      break;
  } //  switch

  // build potential output file name
  // ------------------------------------------------------

  string potentialOutputFileName;

  if (gOahOah->fInputSourceName != "-") {
    // determine potential output file name,
    // may be set differently by '--o, --outputFileName' option
    potentialOutputFileName =
      baseName (gOahOah->fInputSourceName) + "_LOOP";

    // set '.xml' suffix
    size_t
      posInString =
        potentialOutputFileName.rfind ('.');

    if (posInString != string::npos) {
      potentialOutputFileName.replace (
        posInString,
        potentialOutputFileName.size () - posInString,
        ".xml");
    }

    // keep potentialOutputFileName
    gXml2xmlOah->fMusicXMLOutputFileName = potentialOutputFileName;
  }

  // check auto output file option usage
  // ------------------------------------------------------

  if (gXml2xmlOah->fAutoOutputFileName) {
    if (gXml2xmlOah->fMusicXMLOutputFileName.size ()) {
      stringstream s;

      s <<
        "options '-aofn, -auto-output-file-name' and '-o, -output-file-name'"  <<
        endl <<
        "cannot be chosen simultaneously";

      oahError (s.str ());
    }

    else if (gOahOah->fInputSourceName == "-") {
      stringstream s;

      s <<
        "option '-aofn, -auto-output-file-name'"  <<
        endl <<
        "cannot be used when reading from standard input";

      oahError (s.str ());
    }

    else {
      gXml2xmlOah->fMusicXMLOutputFileName =
        potentialOutputFileName;
    }
  }
}

//______________________________________________________________________________
void xml2xmlOahHandler::enforceOahHandlerQuietness ()
{
#ifdef TRACE_OAH
  gGeneralOah->
    enforceQuietness ();
#endif

  gGeneralOah->
    enforceQuietness ();

  gMxmlTreeOah->
    enforceQuietness ();

  gMsrOah->
    enforceQuietness ();

//  gLpsrOah-> // JMI only if pass5 msr2lpsr is run
//    enforceQuietness ();

#ifdef EXTRA_OAH
  gExtraOah->
    enforceQuietness ();
#endif

  gXml2xmlOah->
    enforceQuietness ();
}

//______________________________________________________________________________
void xml2xmlOah::enforceQuietness ()
{}

//______________________________________________________________________________
void xml2xmlOah::checkOptionsConsistency ()
{}

//______________________________________________________________________________
void xml2xmlOah::acceptIn (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    gLogOstream <<
      ".\\\" ==> xml2xmlOah::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_xml2xmlOah>*
    p =
      dynamic_cast<visitor<S_xml2xmlOah>*> (v)) {
        S_xml2xmlOah elem = this;

#ifdef TRACE_OAH
        if (gOahOah->fTraceOahVisitors) {
          gLogOstream <<
            ".\\\" ==> Launching xml2xmlOah::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void xml2xmlOah::acceptOut (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    gLogOstream <<
      ".\\\" ==> xml2xmlOah::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_xml2xmlOah>*
    p =
      dynamic_cast<visitor<S_xml2xmlOah>*> (v)) {
        S_xml2xmlOah elem = this;

#ifdef TRACE_OAH
        if (gOahOah->fTraceOahVisitors) {
          gLogOstream <<
            ".\\\" ==> Launching xml2xmlOah::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void xml2xmlOah::browseData (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    gLogOstream <<
      ".\\\" ==> xml2xmlOah::browseData ()" <<
      endl;
  }
#endif

  oahGroup::browseData (v);
}

//______________________________________________________________________________
void xml2xmlOahHandler::print (ostream& os) const
{
  const int fieldWidth = 27;

  os <<
    "xml2xmlOahHandler:" <<
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

ostream& operator<< (ostream& os, const S_xml2xmlOahHandler& elt)
{
  elt->print (os);
  return os;
}

//_______________________________________________________________________________
S_xml2xmlOah gXml2xmlOah;

S_xml2xmlOah xml2xmlOah::create (
  S_oahHandler handlerUpLink)
{
  xml2xmlOah* o = new xml2xmlOah (
    handlerUpLink);
  assert(o!=0);

  return o;
}

xml2xmlOah::xml2xmlOah (
  S_oahHandler handlerUpLink)
  : oahGroup (
    "xml2xml",
    "hxl", "help-xml2xml",
R"(Options that are used by xml2xml are grouped here.)",
    kElementVisibilityAlways,
    handlerUpLink)
{
  // append this options group to the options handler
  // if relevant
  if (handlerUpLink) {
    handlerUpLink->
      prependGroupToHandler (this);
  }

  // initialize it
  initializeXml2xmlOah ();
}

xml2xmlOah::~xml2xmlOah ()
{}

void xml2xmlOah::initializeXml2xmlOah ()
{
  // version
  // --------------------------------------

  {
    S_oahSubGroup
      versionSubGroup =
        oahSubGroup::create (
          "Version",
          "hxv", "help-xml2xml-version",
R"()",
        kElementVisibilityAlways,
        this);

    appendSubGroupToGroup (versionSubGroup);

    // version

    versionSubGroup->
      appendAtomToSubGroup (
        xml2xmlVersionOahAtom::create (
          "v", "version",
R"(Display xml2xml's version number and history.)"));
  }

  // about
  // --------------------------------------

  {
    S_oahSubGroup
      aboutSubGroup =
        oahSubGroup::create (
          "About",
          "hxa", "help-xml2xml-about",
R"()",
        kElementVisibilityAlways,
        this);

    appendSubGroupToGroup (aboutSubGroup);

    // about

    aboutSubGroup->
      appendAtomToSubGroup (
        xml2xmlAboutOahAtom::create (
          "a", "about",
R"(Display information about xml2xml.)"));
  }

  // contact
  // --------------------------------------

  {
    S_oahSubGroup
      contactSubGroup =
        oahSubGroup::create (
          "Contact",
          "hxc", "help-xml2xml-contact",
R"()",
        kElementVisibilityAlways,
        this);

    appendSubGroupToGroup (contactSubGroup);

    // contact

    contactSubGroup->
      appendAtomToSubGroup (
        xml2xmlContactOahAtom::create (
          "c", "contact",
R"(Display information about how to contacct xml2xml maintainers.)"));
  }

  // output file
  // --------------------------------------

  {
    S_oahSubGroup
      outputFileSubGroup =
        oahSubGroup::create (
          "Output file",
          "hxof", "help-xml2xml-output-file",
R"()",
        kElementVisibilityAlways,
        this);

    appendSubGroupToGroup (outputFileSubGroup);

    // output filename

    outputFileSubGroup->
      appendAtomToSubGroup (
        oahStringAtom::create (
          "o", "output-file-name",
R"(Write MusicXML code to file FILENAME instead of standard output.)",
          "FILENAME",
          "musicXMLOutputFileName",
          fMusicXMLOutputFileName));

    // auto output filename

    fAutoOutputFileName = false;

    outputFileSubGroup->
      appendAtomToSubGroup (
        oahBooleanAtom::create (
          "aofn", "auto-output-file-name",
R"(This option can only be used when reading from a file.
Write MusicXML code to a file in the current working directory.
The file name is derived from that of the input file,
adding '_LOOP' to the file name and
replacing any suffix after the the '.' by 'xml'
or adding '.xml' if none is present.)",
          "autoOutputFileName",
          fAutoOutputFileName));
  }
}

//______________________________________________________________________________
void xml2xmlOah::printXml2xmlOahValues (int fieldWidth)
{
  gLogOstream <<
    "The xml2xml options are:" <<
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
    fMusicXMLOutputFileName <<
    "\"" <<
    endl;

  gIndenter--;
}

//______________________________________________________________________________
void initializeXml2xmlOah (
  S_oahHandler handler)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceOah && ! gGeneralOah->fQuiet) {
    gLogOstream <<
      "Initializing xml2xml options handling" <<
      endl;
  }
#endif

  // enlist versions information
  // ------------------------------------------------------

  enlistVersion (
    musicxml2musicxmlVersionStr (),
    "April 2020",
    "First draft version");

  // create the options variables
  // ------------------------------------------------------

  gXml2xmlOah = xml2xmlOah::create (
    handler);
  assert (gXml2xmlOah != 0);
}


}
