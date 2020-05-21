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

#include "xml2brlManPageOah.h"

#include "xml2brlOah.h"

using namespace std;

namespace MusicXML2
{

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
    executableName + " available options",
    "Options values",
    "h", "help",
    "hs", "helpSummary",
R"(                      Welcome to xml2brl,
            the MusicXML to Braille music translator
          delivered as part of the libmusicxml2 library.
      https://github.com/grame-cncm/libmusicxml/tree/lilypond
)",
R"(
xml2brl [options] [MusicXMLFile|-] [options]
)",
R"(
Options can be written with '-' or '--' at will,
  even though the help information below is presented with '-'.
Option '-h, -help' prints the full help,
  while '-hs, -helpSummary' only prints a help summary.)",
    ios)
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

  // create an xml2lyOah2ManPageGenerator
  S_xml2brlOah2ManPageGenerator
    generator =
      xml2brlOah2ManPageGenerator::create (
        this,
        gLogOstream,
        gOutputOstream);

  // initialize the handler only now, since it may use prefixes
  initializeXml2brlOptionsHandler (
    executableName,
    generator);
}

xml2brlOahHandler::~xml2brlOahHandler ()
{}

void xml2brlOahHandler::initializeXml2brlOptionsHandler (
  string executableName,
  S_xml2brlOah2ManPageGenerator
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

  initializeXml2brlManPageOahHandling (
    this,
    theOah2ManPageGenerator);

  initializeXml2brlOah (
    this);

#ifdef TRACE_OAH
  if (gTraceOah->fTraceOah && ! gGeneralOah->fQuiet) {
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
  if (gTraceOah->fTraceOah && ! gGeneralOah->fQuiet) {
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
      baseName (
        gOahOah->fInputSourceName);

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

  if (gXml2brlOah->fAutoOutputFile) {
    if (gXml2brlOah->fBrailleMusicOutputFileName.size ()) {
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
      gXml2brlOah->fBrailleMusicOutputFileName =
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
void xml2brlOahHandler::acceptIn (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    gLogOstream <<
      ".\\\" ==> xml2brlOahHandler::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_xml2brlOahHandler>*
    p =
      dynamic_cast<visitor<S_xml2brlOahHandler>*> (v)) {
        S_xml2brlOahHandler elem = this;

#ifdef TRACE_OAH
        if (gOahOah->fTraceOahVisitors) {
          gLogOstream <<
            ".\\\" ==> Launching xml2brlOahHandler::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void xml2brlOahHandler::acceptOut (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    gLogOstream <<
      ".\\\" ==> xml2brlOahHandler::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_xml2brlOahHandler>*
    p =
      dynamic_cast<visitor<S_xml2brlOahHandler>*> (v)) {
        S_xml2brlOahHandler elem = this;

#ifdef TRACE_OAH
        if (gOahOah->fTraceOahVisitors) {
          gLogOstream <<
            ".\\\" ==> Launching xml2brlOahHandler::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void xml2brlOahHandler::browseData (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    gLogOstream <<
      ".\\\" ==> xml2brlOahHandler::browseData ()" <<
      endl;
  }
#endif

//  oahGroup::browseData (v);
  oahHandler::browseData (v);

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

    appendSubGroupToGroup (versionSubGroup);

    // version

    versionSubGroup->
      appendAtomToSubGroup (
        xml2brlVersionOahAtom::create (
          "v", "version",
R"(Display xml2brl's version number and history.)"));
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

    appendSubGroupToGroup (aboutSubGroup);

    // about

    aboutSubGroup->
      appendAtomToSubGroup (
        xml2brlAboutOahAtom::create (
          "a", "about",
R"(Display information about xml2brl.)"));
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

    appendSubGroupToGroup (contactSubGroup);

    // contact

    contactSubGroup->
      appendAtomToSubGroup (
        xml2brlContactOahAtom::create (
          "c", "contact",
R"(Display information about how to contacct xml2brl maintainers.)"));
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

    appendSubGroupToGroup (outputFileSubGroup);

    // output filename

    outputFileSubGroup->
      appendAtomToSubGroup (
        oahStringAtom::create (
          "o", "output-file-name",
R"(Write Braille music to file FILENAME instead of standard output.)",
          "FILENAME",
          "outputFileName",
          fBrailleMusicOutputFileName));

    // auto output filename

    fAutoOutputFile = false;

    outputFileSubGroup->
      appendAtomToSubGroup (
        oahBooleanAtom::create (
          "aofn", "auto-output-file-name",
R"(This option can only be used when reading from a file.
Write Braille music to a file in the current working directory.
The file name is derived from that of the input file,
replacing any suffix after the the '.' by 'brl'
or adding '.brl' if none is present.)",
          "autoOutputFileName",
          fAutoOutputFile));
  }
}

//______________________________________________________________________________
void xml2brlOah::acceptIn (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    gLogOstream <<
      ".\\\" ==> xml2brlOah::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_xml2brlOah>*
    p =
      dynamic_cast<visitor<S_xml2brlOah>*> (v)) {
        S_xml2brlOah elem = this;

#ifdef TRACE_OAH
        if (gOahOah->fTraceOahVisitors) {
          gLogOstream <<
            ".\\\" ==> Launching xml2brlOah::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void xml2brlOah::acceptOut (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    gLogOstream <<
      ".\\\" ==> xml2brlOah::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_xml2brlOah>*
    p =
      dynamic_cast<visitor<S_xml2brlOah>*> (v)) {
        S_xml2brlOah elem = this;

#ifdef TRACE_OAH
        if (gOahOah->fTraceOahVisitors) {
          gLogOstream <<
            ".\\\" ==> Launching xml2brlOah::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void xml2brlOah::browseData (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    gLogOstream <<
      ".\\\" ==> xml2brlOah::browseData ()" <<
      endl;
  }
#endif

  oahGroup::browseData (v);
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
    endl;

  gIndenter--;
}

//______________________________________________________________________________
void initializeXml2brlOah (
  S_oahHandler handler)
{
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
