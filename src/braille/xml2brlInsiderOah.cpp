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
#include "bsr.h"

#include "oahOah.h"

#include "generalOah.h"

#include "musicxmlOah.h"
#include "mxmlTreeOah.h"
#include "mxmlTree2MsrOah.h"
#include "msrOah.h"
#include "msr2BsrOah.h"
#include "bsrOah.h"
#include "bsr2BrailleOah.h"
#include "brailleOah.h"

#include "version.h"

#include "xml2brlOahTypes.h"

#include "xml2brlInsiderOah.h"


using namespace std;

namespace MusicXML2
{

//#define OAH_TESTS

//______________________________________________________________________________
S_xml2brlInsiderOahHandler xml2brlInsiderOahHandler::create (
  string   executableName,
  string   handlerHeader,
  ostream& os)
{
  // create the insider handler
  xml2brlInsiderOahHandler* o = new
    xml2brlInsiderOahHandler (
      executableName,
      handlerHeader,
      os);
  assert(o!=0);

  o->createThePrefixesAndInitializeOahHandling (executableName);

  return o;
}

S_xml2brlInsiderOahHandler xml2brlInsiderOahHandler::createWithOnlyThePrefixes (
  string   executableName,
  string   handlerHeader,
  ostream& os)
{
  // create the insider handler
  xml2brlInsiderOahHandler* o = new
    xml2brlInsiderOahHandler (
      executableName,
      handlerHeader,
      os);
  assert(o!=0);

  o->createThePrefixes ();

  return o;
}

xml2brlInsiderOahHandler::xml2brlInsiderOahHandler (
  string   executableName,
  string   handlerHeader,
  ostream& os)
  : oahHandler (
      executableName,
      handlerHeader,
      executableName + " options values",
      "h", "help",
      "hs", "helpSummary",
R"(                      Welcome to xml2brl,
            the MusicXML to Braille music translator
          delivered as part of the libmusicxml2 library.
      https://github.com/grame-cncm/libmusicxml/tree/lilypond
)",
R"(
Usage: xml2brl [options] [MusicXMLFile|-] [options]
)",
R"(
Option '-h, -help' prints the full help,
  while '-hs, -helpSummary' only prints a help summary.)",
    os)
{}

xml2brlInsiderOahHandler::~xml2brlInsiderOahHandler ()
{}

void xml2brlInsiderOahHandler::createThePrefixesAndInitializeOahHandling (
  string executableName)
{
  // create the prefixes
  createThePrefixes ();

  // initialize the insider OAH handling only now, since it may use prefixes
  initializeXml2brlInsiderOahHandling (
    executableName);
}

void xml2brlInsiderOahHandler::createThePrefixes ()
{
  // append the help options prefixes
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

  // append the trace options prefixes
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
}

void xml2brlInsiderOahHandler::initializeXml2brlInsiderOahHandling (
  string executableName)
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
          "Initializing xml2brl insider options handling" <<
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
    initializeBSR ();

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

    initializeMsr2BsrOahHandling (
      this);

    initializeBsrOahHandling (
      this);

    initializeBsr2BrailleOahHandling (
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
#ifdef OAH_TESTS
    // print the options handler initial state
    fHandlerLogOstream <<
      "xml2brlInsiderOahHandler has been initialized as:" <<
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
        "xml2brlInsiderOahHandler help:" <<
        endl;

      this->
        printHelp (
          fHandlerLogOstream);
    }
#endif

  pThisMethodHasBeenRun = true;
  }
}

void xml2brlInsiderOahHandler::checkOptionsAndArguments ()
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceOahDetails) {
    gOutputOstream <<
      "xml2brlInsiderOahHandler::checkOptionsAndArguments() " <<
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
    if (gTraceOah->fTraceOahDetails && ! gGeneralOah->fQuiet) {
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
        baseName (
          inputSourceName);

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
      if (! gBsr2BrailleOah->fDontUseEncodingInFileName) {
        switch (gBsr2BrailleOah->fBrailleOutputKind) {
          case kBrailleOutputAscii:
            potentialOutputFileName += "_ASCII";
            break;

          case kBrailleOutputUTF8:
            potentialOutputFileName += "_UTF8";
              /* JMI
            switch (gBsr2BrailleOah->fByteOrderingKind) {
              case kByteOrderingNone:
                break;
              case kByteOrderingBigEndian:
                potentialOutputFileName += "_BE";
                break;
              case kByteOrderingSmallEndian:
                // should not occur JMI
                break;
            } // switch
            */
            break;

          case kBrailleOutputUTF8Debug:
            potentialOutputFileName += "_UTF8Debug";
            break;

          case kBrailleOutputUTF16:
            potentialOutputFileName += "_UTF16";
              /* JMI
            switch (gBsr2BrailleOah->fByteOrderingKind) {
              case kByteOrderingNone:
                break;

              case kByteOrderingBigEndian:
                potentialOutputFileName += "_BE";
                break;

              case kByteOrderingSmallEndian:
                potentialOutputFileName += "_SE";
                break;
            } // switch
            */
            break;
        } // switch
      }

      // append the file extension
      switch (gBsr2BrailleOah->fBrailleOutputKind) {
        case kBrailleOutputAscii:
         potentialOutputFileName += ".brf";
          break;

        case kBrailleOutputUTF8:
        case kBrailleOutputUTF8Debug:
        case kBrailleOutputUTF16:
          potentialOutputFileName += ".txt";
          break;
      } // switch
    }

    // check auto output file option usage
    // ------------------------------------------------------

    S_oahStringAtom
      outputFileNameStringAtom =
        gXml2brlOah->
          getOutputFileNameStringAtom ();

    S_oahBooleanAtom
      autoOutputFileNameAtom =
        gXml2brlOah->
          getAutoOutputFileNameAtom ();

#ifdef TRACE_OAH
    if (gTraceOah->fTraceOah && ! gGeneralOah->fQuiet) {
      // print the options handler initial state
      fHandlerLogOstream <<
        "xml2brlInsiderOahHandler::checkOptionsAndArguments(): " <<
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

//______________________________________________________________________________
void xml2brlInsiderOahHandler::checkOptionsConsistency ()
{}

void xml2brlInsiderOahHandler::enforceOahHandlerQuietness ()
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

  gBsrOah->
    enforceQuietness ();

  gBrailleOah->
    enforceQuietness ();

  gBsr2BrailleOah->
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
{}

//______________________________________________________________________________
void xml2brlInsiderOahHandler::print (ostream& os) const
{
  const int fieldWidth = 27;

  os <<
    "xml2brlInsiderOahHandler:" <<
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

ostream& operator<< (ostream& os, const S_xml2brlInsiderOahHandler& elt)
{
  elt->print (os);
  return os;
}

//_______________________________________________________________________________
S_xml2brlOah gXml2brlOah;

S_xml2brlOah xml2brlOah::create (
  S_oahHandler handlerUpLink)
{
  xml2brlOah* o = new xml2brlOah (
    handlerUpLink);
  assert(o!=0);

  return o;
}

xml2brlOah::xml2brlOah (
  S_oahHandler handlerUpLink)
  : oahGroup (
    "xml2brl",
    "hxb", "help-xml2brl",
R"(Options that are used by xml2brl are grouped here.)",
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
      subGroup =
        oahSubGroup::create (
          "Version",
          "hxv", "help-xml2brl-version",
R"()",
        kElementVisibilityWhole,
        this);

    appendSubGroupToGroup (subGroup);

    // version

    subGroup->
      appendAtomToSubGroup (
        xml2brlVersionOahAtom::create (
          "v", "version",
R"(Display xml2brl's version number and history.)"));
  }

  // about
  // --------------------------------------

  {
    S_oahSubGroup
      subGroup =
        oahSubGroup::create (
          "About",
          "hxa", "help-xml2brl-about",
R"()",
        kElementVisibilityWhole,
        this);

    appendSubGroupToGroup (subGroup);

    // about

    subGroup->
      appendAtomToSubGroup (
        xml2brlAboutOahAtom::create (
          "a", "about",
R"(Display information about xml2brl.)"));
  }

  // contact
  // --------------------------------------

  {
    S_oahSubGroup
      subGroup =
        oahSubGroup::create (
          "Contact",
          "hxc", "help-xml2brl-contact",
R"()",
        kElementVisibilityWhole,
        this);

    appendSubGroupToGroup (subGroup);

    // contact

    subGroup->
      appendAtomToSubGroup (
        xml2brlContactOahAtom::create (
          "c", "contact",
R"(Display information about how to contacct xml2brl maintainers.)"));
  }

  // output file
  // --------------------------------------

  {
    S_oahSubGroup
      subGroup =
        oahSubGroup::create (
          "Output file",
          "hxof", "help-xml2brl-output-file",
R"()",
        kElementVisibilityWhole,
        this);

    appendSubGroupToGroup (subGroup);

    // output filename

    fOutputFileNameStringAtom =
      oahStringAtom::create (
        "o", "output-file-name",
R"(Write Braille music to file FILENAME instead of standard output.)",
        "FILENAME",
        "outputFileName",
        fBrailleMusicOutputFileName);

    subGroup->
      appendAtomToSubGroup (
        fOutputFileNameStringAtom);

    // auto output filename

    fAutoOutputFileName = false;

    fAutoOutputFileNameAtom =
      oahBooleanAtom::create (
        "aofn", "auto-output-file-name",
R"(This option can only be used when reading from a file.
Write Braille music to a file in the current working directory.
The file name is derived from that of the input file,
replacing any suffix after the the '.' by 'brl'
or adding '.brl' if none is present.)",
        "autoOutputFileName",
        fAutoOutputFileName);

    subGroup->
      appendAtomToSubGroup (
        fAutoOutputFileNameAtom);
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
    setw (fieldWidth) << "brailleMusicOutputFileName" << " : \"" <<
    fBrailleMusicOutputFileName <<
    "\"" <<
    endl <<
    setw (fieldWidth) << "autoOutputFileName" << " : \"" <<
    booleanAsString (fAutoOutputFileName) <<
    "\"" <<
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
void initializeXml2brlOah (
  S_oahHandler handler)
{
  // protect library against multiple initializations
  static bool pThisMethodHasBeenRun = false;

  if (! pThisMethodHasBeenRun) {
#ifdef TRACE_OAH
    if (gTraceOah->fTraceOah && ! gGeneralOah->fQuiet) {
      gLogOstream <<
        "Initializing xml2brl options handling" <<
        endl;
    }
#endif

    // enlist versions information
    // ------------------------------------------------------

    enlistVersion (
      "Initial", "october 2018",
      "Derived from xml2brl, with an embryonic BSR");

    enlistVersion (
      musicxml2brailleVersionStr (), "Nobember 2018",
      "First draft version");

    // create the options variables
    // ------------------------------------------------------

    gXml2brlOah = xml2brlOah::create (
      handler);
    assert (gXml2brlOah != 0);

    pThisMethodHasBeenRun = true;
  }
}


}
