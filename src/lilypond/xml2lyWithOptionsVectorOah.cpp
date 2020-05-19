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
#include "lpsr.h"

#include "oahOah.h"

#include "generalOah.h"

#include "musicxmlOah.h"
#include "mxmlTreeOah.h"
#include "mxmlTree2MsrOah.h"
#include "msrOah.h"
#include "msr2LpsrOah.h"
#include "lpsrOah.h"
#include "lpsr2LilypondOah.h"
#include "lilypondOah.h"

#include "version.h"

#include "xml2lyWithOptionsVectorManPageOah.h"

#include "xml2lyWithOptionsVectorOah.h"

using namespace std;

namespace MusicXML2
{

//______________________________________________________________________________
S_xml2lyWithOptionsVectorOahHandler xml2lyWithOptionsVectorOahHandler::create (
  string           executableName,
  indentedOstream& ios)
{
  xml2lyWithOptionsVectorOahHandler* o = new
    xml2lyWithOptionsVectorOahHandler (
      executableName,
      ios);
  assert(o!=0);
  return o;
}

xml2lyWithOptionsVectorOahHandler::xml2lyWithOptionsVectorOahHandler (
  string           executableName,
  indentedOstream& ios)
  : oahHandler (
    executableName + " available options",
    "Options values",
    "h", "help",
    "hs", "helpSummary",
R"(                      Welcome to xml2lyWithOptionsVector,
              the MusicXML to LilyPond translator
          delivered as part of the libmusicxml2 library.
      https://github.com/grame-cncm/libmusicxml/tree/lilypond
)",
R"(
xml2lyWithOptionsVector [options] [MusicXMLFile|-] [options]
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

  // create an xml2lyWithOptionsVectorOah2ManPageGenerator
  S_xml2lyWithOptionsVectorOah2ManPageGenerator
    generator =
      xml2lyWithOptionsVectorOah2ManPageGenerator::create (
        this,
        gLogOstream,
        gOutputOstream);

  // initialize the handler only now, since it may use prefixes
  initializeXml2lyWithOptionsVectorOptionsHandler (
    executableName,
    generator);
}

xml2lyWithOptionsVectorOahHandler::~xml2lyWithOptionsVectorOahHandler ()
{}

void xml2lyWithOptionsVectorOahHandler::initializeXml2lyWithOptionsVectorOptionsHandler (
  string executableName,
  S_xml2lyWithOptionsVectorOah2ManPageGenerator
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
  initializeLPSR ();

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

  initializeLpsrOahHandling (
    this);

  initializeLpsr2LilyPondOahHandling (
    this);

  initializeLilypondOahHandling (
    this);

#ifdef EXTRA_OAH
  initializeExtraOahHandling (
    this);
#endif

  initializeXml2lyWithOptionsVectorManPageOahHandling (
    this,
    theOah2ManPageGenerator);

  initializeXml2lyWithOptionsVectorOah (
    this);

#ifdef TRACE_OAH
  if (gTraceOah->fTraceOah && ! gGeneralOah->fQuiet) {
    // print the options handler initial state
    fHandlerLogOstream <<
      "xml2lyWithOptionsVectorOahHandler has been initialized as:" <<
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
      "xml2lyWithOptionsVectorOahHandler help:" <<
      endl;

    this->
      printHelp (
        fHandlerLogOstream);
  }
#endif
}

void xml2lyWithOptionsVectorOahHandler::checkOptionsAndArguments ()
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

/* JMI
  // input source name
  // ------------------------------------------------------

  switch (argumentsNumber) {
    case 0:
      fHandlerLogOstream <<
        endl <<
        "xml2lyWithOptionsVectorOahHandler(): Input file name or '-' for standard input expected" <<
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
*/

  // build potential output file name
  // ------------------------------------------------------

  string potentialOutputFileName;

  if (gOahOah->fInputSourceName != "-") {
    // determine potential output file name,
    // may be set differently by '--o, --outputFileName' option
    potentialOutputFileName =
      baseName (
        gOahOah->fInputSourceName);

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

  if (gXml2lyWithOptionsVectorOah->fAutoOutputFileName) {
    if (gXml2lyWithOptionsVectorOah->fLilyPondOutputFileName.size ()) {
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
      gXml2lyWithOptionsVectorOah->fLilyPondOutputFileName =
        potentialOutputFileName;
    }
  }
}

//______________________________________________________________________________
void xml2lyWithOptionsVectorOahHandler::enforceOahHandlerQuietness ()
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

  gLpsrOah->
    enforceQuietness ();

  gLilypondOah->
    enforceQuietness ();

#ifdef EXTRA_OAH
  gExtraOah->
    enforceQuietness ();
#endif

  gXml2lyWithOptionsVectorOah->
    enforceQuietness ();
}

//______________________________________________________________________________
void xml2lyWithOptionsVectorOah::enforceQuietness ()
{}

//______________________________________________________________________________
void xml2lyWithOptionsVectorOah::checkOptionsConsistency ()
{}

//______________________________________________________________________________
void xml2lyWithOptionsVectorOah::acceptIn (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    gLogOstream <<
      ".\\\" ==> xml2lyWithOptionsVectorOah::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_xml2lyWithOptionsVectorOah>*
    p =
      dynamic_cast<visitor<S_xml2lyWithOptionsVectorOah>*> (v)) {
        S_xml2lyWithOptionsVectorOah elem = this;

#ifdef TRACE_OAH
        if (gOahOah->fTraceOahVisitors) {
          gLogOstream <<
            ".\\\" ==> Launching xml2lyWithOptionsVectorOah::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void xml2lyWithOptionsVectorOah::acceptOut (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    gLogOstream <<
      ".\\\" ==> xml2lyWithOptionsVectorOah::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_xml2lyWithOptionsVectorOah>*
    p =
      dynamic_cast<visitor<S_xml2lyWithOptionsVectorOah>*> (v)) {
        S_xml2lyWithOptionsVectorOah elem = this;

#ifdef TRACE_OAH
        if (gOahOah->fTraceOahVisitors) {
          gLogOstream <<
            ".\\\" ==> Launching xml2lyWithOptionsVectorOah::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void xml2lyWithOptionsVectorOah::browseData (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    gLogOstream <<
      ".\\\" ==> xml2lyWithOptionsVectorOah::browseData ()" <<
      endl;
  }
#endif

  oahGroup::browseData (v);
}

//______________________________________________________________________________
void xml2lyWithOptionsVectorOahHandler::print (ostream& os) const
{
  const int fieldWidth = 27;

  os <<
    "xml2lyWithOptionsVectorOahHandler:" <<
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

ostream& operator<< (ostream& os, const S_xml2lyWithOptionsVectorOahHandler& elt)
{
  elt->print (os);
  return os;
}

//_______________________________________________________________________________
S_xml2lyWithOptionsVectorOah gXml2lyWithOptionsVectorOah;

S_xml2lyWithOptionsVectorOah xml2lyWithOptionsVectorOah::create (
  S_oahHandler handlerUpLink)
{
  xml2lyWithOptionsVectorOah* o = new xml2lyWithOptionsVectorOah (
    handlerUpLink);
  assert(o!=0);

  return o;
}

xml2lyWithOptionsVectorOah::xml2lyWithOptionsVectorOah (
  S_oahHandler handlerUpLink)
  : oahGroup (
    "xml2lyWithOptionsVector",
    "hxlwov", "help-xml2lyWithOptionsVector",
R"(Options that are used by xml2lyWithOptionsVector are grouped here.)",
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
  initializeXml2lyWithOptionsVectorOah ();
}

xml2lyWithOptionsVectorOah::~xml2lyWithOptionsVectorOah ()
{}

void xml2lyWithOptionsVectorOah::initializeXml2lyWithOptionsVectorOah ()
{
  // version
  // --------------------------------------

  {
    S_oahSubGroup
      versionSubGroup =
        oahSubGroup::create (
          "Version",
          "hxv", "help-xml2lyWithOptionsVector-version",
R"()",
        kElementVisibilityAlways,
        this);

    appendSubGroupToGroup (versionSubGroup);

    // version

    versionSubGroup->
      appendAtomToSubGroup (
        xml2lyWithOptionsVectorVersionOahAtom::create (
          "v", "version",
R"(Display xml2lyWithOptionsVector's version number and history.)"));
  }

  // about
  // --------------------------------------

  {
    S_oahSubGroup
      aboutSubGroup =
        oahSubGroup::create (
          "About",
          "hxa", "help-xml2lyWithOptionsVector-about",
R"()",
        kElementVisibilityAlways,
        this);

    appendSubGroupToGroup (aboutSubGroup);

    // about

    aboutSubGroup->
      appendAtomToSubGroup (
        xml2lyWithOptionsVectorAboutOahAtom::create (
          "a", "about",
R"(Display information about xml2lyWithOptionsVector.)"));
  }

  // contact
  // --------------------------------------

  {
    S_oahSubGroup
      contactSubGroup =
        oahSubGroup::create (
          "Contact",
          "hxc", "help-xml2lyWithOptionsVector-contact",
R"()",
        kElementVisibilityAlways,
        this);

    appendSubGroupToGroup (contactSubGroup);

    // contact

    contactSubGroup->
      appendAtomToSubGroup (
        xml2lyWithOptionsVectorContactOahAtom::create (
          "c", "contact",
R"(Display information about how to contacct xml2lyWithOptionsVector maintainers.)"));
  }

  // output file
  // --------------------------------------

  {
    S_oahSubGroup
      outputFileSubGroup =
        oahSubGroup::create (
          "Output file",
          "hxof", "help-xml2lyWithOptionsVector-output-file",
R"()",
        kElementVisibilityAlways,
        this);

    appendSubGroupToGroup (outputFileSubGroup);

    // output filename

    outputFileSubGroup->
      appendAtomToSubGroup (
        oahStringAtom::create (
          "o", "output-file-name",
R"(Write LilyPond code to file FILENAME instead of standard output.)",
          "FILENAME",
          "lilyPondOutputFileName",
          fLilyPondOutputFileName));

    // auto output filename

    fAutoOutputFileName = false;

    outputFileSubGroup->
      appendAtomToSubGroup (
        oahBooleanAtom::create (
          "aofn", "auto-output-file-name",
R"(This option can only be used when reading from a file.
Write LilyPond code to a file in the current working directory.
The file name is derived from that of the input file,
replacing any suffix after the the '.' by 'ly'
or adding '.ly' if none is present.)",
          "autoOutputFileName",
          fAutoOutputFileName));
  }

  // loop back to MusicXML
  initializeXml2lyWithOptionsVectorLoopOptions (false);
}

void xml2lyWithOptionsVectorOah::initializeXml2lyWithOptionsVectorLoopOptions (
  bool boolOptionsInitialValue)
{
  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Loop",
        "hxml2lyWithOptionsVectorlo", "help-xml2lyWithOptionsVector-loopback-options",
R"()",
        kElementVisibilityAlways,
        this);

  appendSubGroupToGroup (subGroup);

  // '-loop' is hidden...

  fLoopBackToMusicXML = boolOptionsInitialValue;

  S_oahBooleanAtom
    loopOptionsBooleanAtom =
      oahBooleanAtom::create (
        "loop", "loop-back-to-musicxml",
R"(Close the loop, generating a MusicXML file from the MSR.
The file name receives a '_LOOP.xml' suffix.
This is equivalent to using xml2xml)",
        "loopBackToMusicXML",
        fLoopBackToMusicXML);
  loopOptionsBooleanAtom->
    setIsHidden ();

  subGroup->
    appendAtomToSubGroup (
      loopOptionsBooleanAtom);
}

//______________________________________________________________________________
void xml2lyWithOptionsVectorOah::printXml2lyWithOptionsVectorOahValues (int fieldWidth)
{
  gLogOstream <<
    "The xml2lyWithOptionsVector options are:" <<
    endl;

  gIndenter++;

  // output file
  // --------------------------------------

  gLogOstream << left <<
    setw (fieldWidth) << "Output file:" <<
    endl;

  gIndenter++;

  gLogOstream << left <<
    setw (fieldWidth) << "lilyPondOutputFileName" << " : \"" <<
    fLilyPondOutputFileName <<
    "\"" <<
    endl;

  gIndenter--;

  // loop back to MusicXML
  // --------------------------------------

  gLogOstream <<
    "Loop:" <<
    endl;

  gIndenter++;

  gLogOstream << left <<
    setw (fieldWidth) << "loopToMusicXML" << " : " <<
    booleanAsString (fLoopBackToMusicXML) <<
    endl;

  gIndenter--;
}

//______________________________________________________________________________
void initializeXml2lyWithOptionsVectorOah (
  S_oahHandler handler)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceOah && ! gGeneralOah->fQuiet) {
    gLogOstream <<
      "Initializing xml2lyWithOptionsVector options handling" <<
      endl;
  }
#endif

  // enlist versions information
  // ------------------------------------------------------

  enlistVersion (
    musicxml2lilypondVersionStr (), "May 2020",
    "First draft version");

  // create the options variables
  // ------------------------------------------------------

  gXml2lyWithOptionsVectorOah = xml2lyWithOptionsVectorOah::create (
    handler);
  assert (gXml2lyWithOptionsVectorOah != 0);
}


}
