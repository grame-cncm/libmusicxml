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

#include "xml2xmlInsiderOah.h"


using namespace std;

namespace MusicXML2
{

//______________________________________________________________________________
S_xml2xmlInsiderOahHandler xml2xmlInsiderOahHandler::create (
  string   executableName,
  string   handlerHeader,
  ostream& os)
{
  xml2xmlInsiderOahHandler* o = new
    xml2xmlInsiderOahHandler (
      executableName,
      handlerHeader,
      os);
  assert(o!=0);

  o->createThePrefixesAndInitialize (executableName);

  return o;
}

xml2xmlInsiderOahHandler::xml2xmlInsiderOahHandler (
  string   executableName,
  string   handlerHeader,
  ostream& os)
  : oahHandler (
      executableName,
      handlerHeader,
      executableName + " options values",
      "h", "help",
      "hs", "helpSummary",
R"(                      Welcome to xml2xml,
              the MusicXML to MusicXML translator
          delivered as part of the libmusicxml2 library.
      https://github.com/grame-cncm/libmusicxml/tree/lilypond
)",
R"(
Usage: xml2xml [options] [MusicXMLFile|-] [options]
)",
R"(
Option '-h, -help' prints the full help,
  while '-hs, -helpSummary' only prints a help summary.)",
    os)
{}

xml2xmlInsiderOahHandler::~xml2xmlInsiderOahHandler ()
{}

S_xml2xmlInsiderOahHandler xml2xmlInsiderOahHandler::createHandlerNewbornCloneWithoutGroups ()
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceOah) {
    gLogOstream <<
      "Creating a newborn clone of xml2xmlInsiderOahHandler" <<
      endl;
  }
#endif

  S_xml2xmlInsiderOahHandler
    newbornClone =
      xml2xmlInsiderOahHandler::create (
        fExecutableName,
        fHandlerHeader + "_clone",
        fHandlerLogOstream);

  newbornClone->fShortName =
    fShortName + "_clone";
  newbornClone->fLongName =
    fLongName + "_clone";

  newbornClone->fHandlerPrefixesMap =
    fHandlerPrefixesMap;

  return newbornClone;
}

void xml2xmlInsiderOahHandler::createThePrefixesAndInitialize (
  string executableName)
{
  // create the prefixes
  createThePrefixes ();

  // initialize the insider OAH handling only now, since it may use prefixes
  initializeXml2xmlInsiderOahHandling (
    executableName);
}

void xml2xmlInsiderOahHandler::createThePrefixes ()
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
}

void xml2xmlInsiderOahHandler::initializeXml2xmlInsiderOahHandling (
  string executableName)
{
  /*
    The order of the initializations below determines
    the relative order of the atoms in the help output,
    which is retained in oahDualHandler::populateUserHandlerFromInsiderHandler()
  */

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

  initializeXml2xmlOah (
    this);

#ifdef TRACE_OAH
  if (gTraceOah->fTraceOah && ! gGeneralOah->fQuiet) {
    // print the options handler initial state
    fHandlerLogOstream <<
      "xml2xmlInsiderOahHandler has been initialized as:" <<
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
      "xml2xmlInsiderOahHandler help:" <<
      endl;

    this->
      printHelp (
        fHandlerLogOstream);
  }
#endif
}

void xml2xmlInsiderOahHandler::checkOptionsAndArguments ()
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
      {
        if (! fHandlerFoundAHelpOption) {
          string message =
            "Input file name or '-' for standard input expected";

          fHandlerLogOstream <<
            endl <<
            message <<
            endl <<
            endl;

          throw msrOahException (message);
        }
      }
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

  // determine output file name
  // ------------------------------------------------------

#ifdef TRACE_OAH
  if (gTraceOah->fTracePasses) {
    string separator =
      "%--------------------------------------------------------------";

    gLogOstream <<
      "xml2xmlInsiderOahHandler::checkOptionsAndArguments(): " <<
      "gOahOah->fInputSourceName: \"" << gOahOah->fInputSourceName << "\"" <<
      ", gOahOah->fInputSourceName: \"" << gOahOah->fInputSourceName << "\"" <<
      ", gXml2xmlOah->fAutoOutputFileName: \"" << booleanAsString (gXml2xmlOah->fAutoOutputFileName) << "\"" <<
      ", gXml2xmlOah->fMusicXMLOutputFileName: \"" << gXml2xmlOah->fMusicXMLOutputFileName << "\"" <<
      endl <<
      separator <<
      endl;
  }
#endif

  if (gXml2xmlOah->fAutoOutputFileName) {
    if (gOahOah->fInputSourceName == "-") {
      gXml2xmlOah->fMusicXMLOutputFileName =
        "stdout.xml";
    }
    else {
      determineOutputFileNameFromInputFileName ();
    }
  }

  else {
    if (gXml2xmlOah->fMusicXMLOutputFileName.size ()) {
      // the '--o, --outputFileName' option has been used,
      // use the user chosen output file name
    }
    else {
      stringstream s;

      s <<
        "xml2xmlInsiderOahHandler: a MusicXML output file name must be chosen with '-o, -output-file-name";

      oahError (s.str ());
    }
  }
}

void xml2xmlInsiderOahHandler::determineOutputFileNameFromInputFileName ()
{
#ifdef TRACE_OAH
  if (gTraceOah->fTracePasses) {
    string separator =
      "%--------------------------------------------------------------";

    gLogOstream <<
      "xml2xmlInsiderOahHandler::determineOutputFileNameFromInputFileName(): " <<
      endl <<
      separator <<
      endl;
  }
#endif
  // get input source base name
  string inputSourceBaseName =
    baseName (gOahOah->fInputSourceName);

  // get input source base name prefix
  string inputSourceBaseNamePrefix;

  size_t
    posInString =
      inputSourceBaseName.rfind ('.');

  if (posInString != string::npos) {
    inputSourceBaseNamePrefix =
      inputSourceBaseName.substr (0, posInString);
  }
  else {
    inputSourceBaseNamePrefix =
      inputSourceBaseName;
  }

  string outputFileName =
    inputSourceBaseNamePrefix + "_LOOP.xml";

#ifdef TRACE_OAH
  if (gTraceOah->fTracePasses) {
    string separator =
      "%--------------------------------------------------------------";

    gLogOstream <<
      "xml2xmlInsiderOahHandler::determineOutputFileNameFromInputFileName(): " <<
      "gOahOah->fInputSourceName: \"" << gOahOah->fInputSourceName << "\"" <<
      ", inputSourceBaseName: \"" << inputSourceBaseName << "\"" <<
      ", inputSourceBaseNamePrefix: \"" << inputSourceBaseNamePrefix << "\"" <<
      ", outputFileName: \"" << outputFileName << "\"" <<
      endl <<
      separator <<
      endl;
  }
#endif

  // use outputFileName
  gXml2xmlOah->fMusicXMLOutputFileName =
    outputFileName;
}

void xml2xmlInsiderOahHandler::checkOptionsConsistency ()
{}

//______________________________________________________________________________
void xml2xmlInsiderOahHandler::enforceOahHandlerQuietness ()
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
{
/* JMI
  if (! fMusicXMLOutputFileName.size ()) { // JMI
    stringstream s;

    s <<
      "xml2xmlOah: a MusicXML output file name must be chosen with '-o, -output-file-name";

    oahError (s.str ());
  }

  else if (fMusicXMLOutputFileName == gOahOah->fInputSourceName) {
    stringstream s;

    s <<
      "\"" << fMusicXMLOutputFileName << "\" is both the input and output file name";

    oahError (s.str ());
  }
  */
}

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
void xml2xmlInsiderOahHandler::print (ostream& os) const
{
  const int fieldWidth = 27;

  os <<
    "xml2xmlInsiderOahHandler:" <<
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

ostream& operator<< (ostream& os, const S_xml2xmlInsiderOahHandler& elt)
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
    kElementVisibilityWhole,
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
      subGroup =
        oahSubGroup::create (
          "Version",
          "hxv", "help-xml2xml-version",
R"()",
        kElementVisibilityWhole,
        this);

    appendSubGroupToGroup (subGroup);

/* JMI
    fHandlerUpLink->
      appendSubGroupToInternalAndUserGroups (
        subGroup,
        this,
        fHandlerUpLink->getInformationUserGroup ());
*/

    // version

    subGroup->
      appendAtomToSubGroup (
        xml2xmlVersionOahAtom::create (
          "v", "version",
R"(Display xml2xml's version number and history.)"));
  }

  // about
  // --------------------------------------

  {
    S_oahSubGroup
      subGroup =
        oahSubGroup::create (
          "About",
          "hxa", "help-xml2xml-about",
R"()",
        kElementVisibilityWhole,
        this);

    appendSubGroupToGroup (subGroup);

    // about

    subGroup->
      appendAtomToSubGroup (
        xml2xmlAboutOahAtom::create (
          "a", "about",
R"(Display information about xml2xml.)"));
  }

  // contact
  // --------------------------------------

  {
    S_oahSubGroup
      subGroup =
        oahSubGroup::create (
          "Contact",
          "hxc", "help-xml2xml-contact",
R"()",
        kElementVisibilityWhole,
        this);

    appendSubGroupToGroup (subGroup);

    // contact

    subGroup->
      appendAtomToSubGroup (
        xml2xmlContactOahAtom::create (
          "c", "contact",
R"(Display information about how to contacct xml2xml maintainers.)"));
  }

  // output file
  // --------------------------------------

  {
    S_oahSubGroup
      subGroup =
        oahSubGroup::create (
          "Output file",
          "hxof", "help-xml2xml-output-file",
R"()",
        kElementVisibilityWhole,
        this);

    appendSubGroupToGroup (subGroup);

    // output filename

 //   fMusicXMLOutputFileName = "foo.xml"; // JMI TEMP

    subGroup->
      appendAtomToSubGroup (
        oahStringAtom::create (
          "o", "output-file-name",
R"(Write MusicXML code to file FILENAME instead of standard output.)",
          "FILENAME",
          "musicXMLOutputFileName",
          fMusicXMLOutputFileName));

    // auto output filename

    fAutoOutputFileName = false;

    subGroup->
      appendAtomToSubGroup (
        oahBooleanAtom::create (
          "aofn", "auto-output-file-name",
R"(This option can only be used when reading from a file.
Write MusicXML code to a file in the current working directory.
The file name is derived from that of the input file,
replacing any suffix after the the '.' by 'xml'
or adding '.xml' if none is present.)",
          "autoOutputFileName",
          fAutoOutputFileName));
  }

  // exit after some passes
  // --------------------------------------

  {
    S_oahSubGroup
      exitAfterSomePassesSubGroup =
        oahSubGroup::create (
          "Exit after some passes",
          "hmexit", "help-msr-exit",
  R"()",
        kElementVisibilityWhole,
        this);

    appendSubGroupToGroup (exitAfterSomePassesSubGroup);

    // exit after pass 2a

    fExit2a = false;

    S_oahBooleanAtom
      exit2aOahBooleanAtom =
        oahBooleanAtom::create (
          "e2a", "exit-2a",
  R"(Exit after pass 2a, i.e. after conversion
  of the MusicXML tree to an MSR skeleton.)",
          "exit2a",
          fExit2a);

    exitAfterSomePassesSubGroup->
      appendAtomToSubGroup (
        exit2aOahBooleanAtom);

    // exit after pass 2b

    fExit2b = false;

    S_oahBooleanAtom
      exit2bOahBooleanAtom =
        oahBooleanAtom::create (
          "e2b", "exit-2b",
  R"(Exit after pass 2b, i.e. after conversion
  of the MusicXML tree to MSR.)",
          "exit2b",
          fExit2b);

    exitAfterSomePassesSubGroup->
      appendAtomToSubGroup (
        exit2bOahBooleanAtom);
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
  // protect library against multiple initializations
  static bool initializeXml2xmlOahBeenRun = false;

  if (! initializeXml2xmlOahBeenRun) {
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

    // create the options groups
    // ------------------------------------------------------

    gXml2xmlOah = xml2xmlOah::create (
      handler);
    assert (gXml2xmlOah != 0);
  }

  initializeXml2xmlOahBeenRun = true;
}


}
