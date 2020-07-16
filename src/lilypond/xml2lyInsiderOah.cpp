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
#include "msr2MxmltreeOah.h"    // this is in case the '-loop' option is selected

#include "version.h"

#include "xml2lyOahTypes.h"

#include "xml2lyInsiderOah.h"


using namespace std;

namespace MusicXML2
{

//#define TRACE_TESTS

//______________________________________________________________________________
S_xml2lyInsiderOahHandler xml2lyInsiderOahHandler::create (
  string   executableName,
  string   handlerHeader,
  ostream& os)
{
  // create the insider handler
  xml2lyInsiderOahHandler* o = new
    xml2lyInsiderOahHandler (
      executableName,
      handlerHeader,
      os);
  assert(o!=0);

  o->createThePrefixesAndInitializeOahHandling (executableName);

  return o;
}

S_xml2lyInsiderOahHandler xml2lyInsiderOahHandler::createWithOnlyThePrefixes (
  string   executableName,
  string   handlerHeader,
  ostream& os)
{
  // create the insider handler
  xml2lyInsiderOahHandler* o = new
    xml2lyInsiderOahHandler (
      executableName,
      handlerHeader,
      os);
  assert(o!=0);

  o->createThePrefixes ();

  return o;
}

xml2lyInsiderOahHandler::xml2lyInsiderOahHandler (
  string   executableName,
  string   handlerHeader,
  ostream& os)
  : oahHandler (
      executableName,
      handlerHeader,
      executableName + " options values",
      "h", "help",
      "hs","helpSummary",
R"(                      Welcome to xml2ly,
              the MusicXML to LilyPond translator
          delivered as part of the libmusicxml2 library.
      https://github.com/grame-cncm/libmusicxml/tree/lilypond
)",
R"(
Usage: xml2ly [options] [MusicXMLFile|-] [options]
)",
R"(
Option '-h, -help' prints the full help,
  while '-hs, -helpSummary' only prints a help summary.)",
    os)
{}

xml2lyInsiderOahHandler::~xml2lyInsiderOahHandler ()
{}

void xml2lyInsiderOahHandler::createThePrefixesAndInitializeOahHandling (
  string executableName)
{
  // create the prefixes
  createThePrefixes ();

  // initialize the insider OAH handling only now, since it may use prefixes
  initializeXml2lyInsiderOahHandling (
    executableName);
}

void xml2lyInsiderOahHandler::createThePrefixes ()
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

/* JMI
  // create an xml2lyOah2ManPageGenerator
  S_xml2lyOah2ManPageGenerator
    generator =
      xml2lyOah2ManPageGenerator::create (
        this,
        gLogOstream,
        gOutputOstream);
        */
}

void xml2lyInsiderOahHandler::initializeXml2lyInsiderOahHandling (
  string executableName)
/* JMI
  ,
  S_xml2lyOah2ManPageGenerator
         theOah2ManPageGenerator)
         */
{
  /*
    The order of the initializations below determines
    the relative order of the atoms in the help output,
    which is retained in oahDualHandler::populateUserHandlerFromInsiderHandler()
  */

  // protect library against multiple initializations
  static bool pThisMethodHasBeenRun = false;

  if (! pThisMethodHasBeenRun) {
    /* JMI
  #ifdef TRACE_OAH
      if (gTraceOah->fTraceOah && ! gGeneralOah->fQuiet) {
        gLogOstream <<
          "Initializing xml2ly insider options handling" <<
          endl;
      }
  #endif
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

    initializeLpsr2LilypondOahHandling (
      this);

    initializeLilypondOahHandling (
      this);

    // this is in case the '-loop' option is selected
    initializeMsr2MxmltreeOahHandling (
      this);

#ifdef EXTRA_OAH
    initializeExtraOahHandling (
      this);
#endif

  /* JMI
    initializeXml2lyManPageOahHandling (
      this,
      theOah2ManPageGenerator);
  */

    initializeXml2lyOah (
      this);

#ifdef TRACE_OAH
#ifdef TRACE_TESTS
    // print the options handler initial state
    fHandlerLogOstream <<
      "xml2lyInsiderOahHandler has been initialized as:" <<
      endl;

    gIndenter++;

    print (fHandlerLogOstream);

    fHandlerLogOstream <<
      endl <<
      endl;

    gIndenter--;
#endif
#endif

    // register options handler in itself,
    // so that the 'global' help options can be handled
    this->
      registerHandlerInItself ();
    // register options handler names in itself
    this->
      registerHandlerOptionNamesInItself ();

#ifdef TRACE_OAH
    if (gTraceOah->fTraceOah && ! gGeneralOah->fQuiet) {
      fHandlerLogOstream <<
        "xml2lyInsiderOahHandler help:" <<
        endl;

      this->
        printHelp (
          fHandlerLogOstream);
    }
#endif

    pThisMethodHasBeenRun = true;
  }
}

void xml2lyInsiderOahHandler::checkOptionsAndArguments ()
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceOahDetails) {
    gOutputOstream <<
      "xml2lyInsiderOahHandler::checkOptionsAndArguments() " <<
      fHandlerHeader <<
      "\"" <<
      endl;
  }
#endif

  static bool pThisMethodHasBeenRun = false;

  if (! pThisMethodHasBeenRun) {
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
          " in handlerArgumentsVector for " <<
          gOahOah->fHandlerExecutableName <<
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
          "Several input file names supplied, only the first one, \"" <<
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

    string
      inputSourceName =
        gOahOah->fInputSourceName;

    string potentialOutputFileName;

    if (inputSourceName != "-") {
      // determine potential output file name,
      // may be set differently by '--o, --outputFileName' option
      potentialOutputFileName =
        baseName (inputSourceName);

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

    S_oahStringAtom
      outputFileNameStringAtom =
        gXml2lyOah->
          getOutputFileNameStringAtom ();

    S_oahBooleanAtom
      autoOutputFileNameAtom =
        gXml2lyOah->
          getAutoOutputFileNameAtom ();

#ifdef TRACE_OAH
    if (gTraceOah->fTraceOah && ! gGeneralOah->fQuiet) {
      // print the options handler initial state
      fHandlerLogOstream <<
        "xml2lyInsiderOahHandler::checkOptionsAndArguments(): " <<
        endl;

      gIndenter++;

      fHandlerLogOstream <<
        "outputFileNameStringAtom:" <<
        endl;

      gIndenter++;
      fHandlerLogOstream <<
        outputFileNameStringAtom <<
        endl;
      gIndenter--;

      fHandlerLogOstream <<
        "autoOutputFileNameAtom:" <<
          endl;

      gIndenter++;
      fHandlerLogOstream <<
        autoOutputFileNameAtom <<
        endl;
      gIndenter--;

      gIndenter--;
    }
#endif

    if (autoOutputFileNameAtom->getVariableHasBeenSet ()) {
      // '-aofn, -auto-output-file-name' has been chosen
      // ---------------------------------------------------

      if (inputSourceName == "-") {
        stringstream s;

        s <<
          "option '-aofn, -auto-output-file-name'"  <<
          " cannot be used when reading from standard input";

        oahError (s.str ());
      }

      else {
        if (outputFileNameStringAtom->getVariableHasBeenSet ()) {
          // '-aofn, -auto-output-file-name' has been chosen
          // '-o, -output-file-name' has been chosen

#ifdef TRACE_OAH
          if (gTraceOah->fTraceOah && ! gGeneralOah->fQuiet) {
            fHandlerLogOstream <<
              "'-aofn, -auto-output-file-name' has been chosen" <<
              endl <<
              "'-o, -output-file-name' has been chosen" <<
              endl;
          }
#endif

          stringstream s;

          s <<
            "options '-aofn, -auto-output-file-name' and '-o, -output-file-name'"  <<
            endl <<
            "cannot be chosen simultaneously" <<
            " (lilyPondOutputFileName: \"" <<
            outputFileNameStringAtom->getStringVariable () <<
            "\")";

          oahError (s.str ());
        }

        else {
          // '-aofn, -auto-output-file-name' has been chosen
          // '-o, -output-file-name' has NOT been chosen

#ifdef TRACE_OAH
          if (gTraceOah->fTraceOah && ! gGeneralOah->fQuiet) {
            fHandlerLogOstream <<
              "'-aofn, -auto-output-file-name' has been chosen" <<
              endl <<
              "'-o, -output-file-name' has NOT been chosen" <<
              endl <<
              "====> output goes to \"" <<
              potentialOutputFileName <<
              "\"" <<
              endl;
          }
#endif

          outputFileNameStringAtom->
            setStringVariable (
              potentialOutputFileName);
        }
      }
    }

    else {
      // '-aofn, -auto-output-file-name' has NOT been chosen
      // ---------------------------------------------------

      if (outputFileNameStringAtom->getVariableHasBeenSet ()) {
        // '-aofn, -auto-output-file-name' has NOT been chosen
        // '-o, -output-file-name' has been chosen

#ifdef TRACE_OAH
        if (gTraceOah->fTraceOah && ! gGeneralOah->fQuiet) {
          fHandlerLogOstream <<
            "'-aofn, -auto-output-file-name' has NOT been chosen" <<
            endl <<
            "'-o, -output-file-name' has been chosen" <<
            endl <<
            "====> output goes to \"" <<
            outputFileNameStringAtom->getStringVariable () <<
            "\"" <<
            endl;
        }
#endif
      }

      else {
        // '-aofn, -auto-output-file-name' has NOT been chosen
        // '-o, -output-file-name' has NOT been chosen

#ifdef TRACE_OAH
        if (gTraceOah->fTraceOah && ! gGeneralOah->fQuiet) {
          fHandlerLogOstream <<
            "'-aofn, -auto-output-file-name' has NOT been chosen" <<
            endl <<
            "'-o, -output-file-name' has NOT been chosen" <<
            endl <<
            "====> output goes to standard output" <<
            endl;
        }
#endif
      }
    }

    pThisMethodHasBeenRun = true;
  }
}

void xml2lyInsiderOahHandler::checkOptionsConsistency ()
{}

//______________________________________________________________________________
void xml2lyInsiderOahHandler::enforceOahHandlerQuietness ()
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

  gLpsr2LilypondOah->
    enforceQuietness ();

#ifdef EXTRA_OAH
  gExtraOah->
    enforceQuietness ();
#endif

  gXml2lyOah->
    enforceQuietness ();
}

//______________________________________________________________________________
void xml2lyOah::enforceQuietness ()
{}

//______________________________________________________________________________
void xml2lyOah::checkOptionsConsistency ()
{}

//______________________________________________________________________________
void xml2lyOah::acceptIn (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    gLogOstream <<
      ".\\\" ==> xml2lyOah::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_xml2lyOah>*
    p =
      dynamic_cast<visitor<S_xml2lyOah>*> (v)) {
        S_xml2lyOah elem = this;

#ifdef TRACE_OAH
        if (gOahOah->fTraceOahVisitors) {
          gLogOstream <<
            ".\\\" ==> Launching xml2lyOah::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void xml2lyOah::acceptOut (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    gLogOstream <<
      ".\\\" ==> xml2lyOah::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_xml2lyOah>*
    p =
      dynamic_cast<visitor<S_xml2lyOah>*> (v)) {
        S_xml2lyOah elem = this;

#ifdef TRACE_OAH
        if (gOahOah->fTraceOahVisitors) {
          gLogOstream <<
            ".\\\" ==> Launching xml2lyOah::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void xml2lyOah::browseData (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    gLogOstream <<
      ".\\\" ==> xml2lyOah::browseData ()" <<
      endl;
  }
#endif

  oahGroup::browseData (v);
}

//______________________________________________________________________________
void xml2lyInsiderOahHandler::print (ostream& os) const
{
  const int fieldWidth = 27;

  os <<
    "xml2lyInsiderOahHandler:" <<
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
      // print the options group
      os << (*i);
      if (++i == iEnd) break;
      os << endl;
    } // for

    gIndenter--;
  }

if (false) { // JMI
  // print the known options
  os <<
    "xml2lyInsiderOahHandler known options +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" <<
    endl <<
    "xml2lyInsiderOahHandler known options +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" <<
    endl;
  printKnownOptions (os);
}

  gIndenter--;

  os << endl;
}

ostream& operator<< (ostream& os, const S_xml2lyInsiderOahHandler& elt)
{
  elt->print (os);
  return os;
}

//_______________________________________________________________________________
S_xml2lyOah gXml2lyOah;

S_xml2lyOah xml2lyOah::create (
  S_oahHandler handlerUpLink)
{
  xml2lyOah* o = new xml2lyOah (
    handlerUpLink);
  assert(o!=0);

  return o;
}

xml2lyOah::xml2lyOah (
  S_oahHandler handlerUpLink)
  : oahGroup (
    "xml2ly",
    "hxl", "help-xml2ly",
R"(Options that are used by xml2ly are grouped here.)",
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
  initializeXml2lyOah ();
}

xml2lyOah::~xml2lyOah ()
{}

void xml2lyOah::initializeXml2lyOah ()
{
  // version
  // --------------------------------------

  {
    S_oahSubGroup
      subGroup =
        oahSubGroup::create (
          "Version",
          "hxv", "help-xml2ly-version",
R"()",
        kElementVisibilityWhole,
        this);

    appendSubGroupToGroup (subGroup);

    // version

    subGroup->
      appendAtomToSubGroup (
        xml2lyVersionOahAtom::create (
          "v", "version",
R"(Display xml2ly's version number and history.)"));
  }

  // about
  // --------------------------------------

  {
    S_oahSubGroup
      subGroup =
        oahSubGroup::create (
          "About",
          "hxa", "help-xml2ly-about",
R"()",
        kElementVisibilityWhole,
        this);

    appendSubGroupToGroup (subGroup);

    // about

    subGroup->
      appendAtomToSubGroup (
        xml2lyAboutOahAtom::create (
          "a", "about",
R"(Display information about xml2ly.)"));
  }

  // contact
  // --------------------------------------

  {
    S_oahSubGroup
      subGroup =
        oahSubGroup::create (
          "Contact",
          "hxc", "help-xml2ly-contact",
R"()",
        kElementVisibilityWhole,
        this);

    appendSubGroupToGroup (subGroup);

    // contact

    subGroup->
      appendAtomToSubGroup (
        xml2lyContactOahAtom::create (
          "c", "contact",
R"(Display information about how to contacct xml2ly maintainers.)"));
  }

  // output file
  // --------------------------------------

  {
    S_oahSubGroup
      subGroup =
        oahSubGroup::create (
          "Output file",
          "hxof", "help-xml2ly-output-file",
R"()",
        kElementVisibilityWhole,
        this);

    appendSubGroupToGroup (subGroup);

    // output filename

    fOutputFileNameStringAtom =
      oahStringAtom::create (
        "o", "output-file-name",
R"(Write LilyPond code to file FILENAME instead of standard output.)",
        "FILENAME",
        "lilyPondOutputFileName",
        fLilyPondOutputFileName);

    subGroup->
      appendAtomToSubGroup (
        fOutputFileNameStringAtom);

    // auto output filename

    fAutoOutputFileName = false;

    fAutoOutputFileNameAtom =
      oahBooleanAtom::create (
        "aofn", "auto-output-file-name",
R"(This option can only be used when reading from a file.
Write LilyPond code to a file in the current working directory.
The file name is derived from that of the input file,
replacing any suffix after the the '.' by 'ly'
or adding '.ly' if none is present.)",
        "autoOutputFileName",
        fAutoOutputFileName);

    subGroup->
      appendAtomToSubGroup (
        fAutoOutputFileNameAtom);
  }

  // loop back to MusicXML
  // --------------------------------------

  {
    S_oahSubGroup
      subGroup =
        oahSubGroup::create (
          "Loop",
          "hxml2lylo", "help-xml2ly-loopback-options",
  R"()",
          kElementVisibilityWhole,
          this);

    appendSubGroupToGroup (subGroup);

    // loop

    fLoopBackToMusicXML = false;

    S_oahBooleanAtom
      loopOptionsBooleanAtom =
        oahBooleanAtom::create (
          "loop", "loop-back-to-musicxml",
  R"(Close the loop, generating a MusicXML file from the MSR.
  The file name receives a '_LOOP.xml' suffix.
  This is equivalent to using xml2xml)",
          "loopBackToMusicXML",
          fLoopBackToMusicXML);
    // make this atom imvisible
    loopOptionsBooleanAtom->
      setElementVisibilityKind (
        kElementVisibilityNone);

    subGroup->
      appendAtomToSubGroup (
        loopOptionsBooleanAtom);
  }

  // exit after some passes
  // --------------------------------------

  {
    S_oahSubGroup
      subGroup =
        oahSubGroup::create (
          "Exit after some passes",
          "hmexit", "help-msr-exit",
  R"()",
        kElementVisibilityWhole,
        this);

    appendSubGroupToGroup (subGroup);

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

    subGroup->
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

    subGroup->
      appendAtomToSubGroup (
        exit2bOahBooleanAtom);
  }
}

//______________________________________________________________________________
void xml2lyOah::printXml2lyOahValues (int fieldWidth)
{
  gLogOstream <<
    "The xml2ly options are:" <<
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
    endl <<
    setw (fieldWidth) << "autoOutputFileName" << " : \"" <<
    booleanAsString (fAutoOutputFileName) <<
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

  // exit after some passes
  // --------------------------------------

  gLogOstream <<
    "Exit after some passes:" <<
    endl;

  gIndenter++;

  gLogOstream << left <<
    setw (fieldWidth) << "exit2a" << " : " <<
    booleanAsString (fExit2a) <<
    endl <<
    setw (fieldWidth) << "exit2b" << " : " <<
    booleanAsString (fExit2b) <<
    endl;

  gIndenter--;

  gIndenter--;
}

//______________________________________________________________________________
void initializeXml2lyOah (
  S_oahHandler handler)
{
  // protect library against multiple initializations
  static bool pThisMethodHasBeenRun = false;

  if (! pThisMethodHasBeenRun) {
#ifdef TRACE_OAH
    if (gTraceOah->fTraceOah && ! gGeneralOah->fQuiet) {
      gLogOstream <<
        "Initializing xml2ly options handling" <<
        endl;
    }
#endif

    // enlist versions information
    // ------------------------------------------------------

    enlistVersion (
      "Initial", "early 2016",
      "Start as xml2lilypond, a clone of xml2guido");

    enlistVersion (
      musicxml2lilypondVersionStr (), "October 2018",
      "First draft version");

    // create the options variables
    // ------------------------------------------------------

    gXml2lyOah = xml2lyOah::create (
      handler);
    assert (gXml2lyOah != 0);

    pThisMethodHasBeenRun = true;
  }
}


}
