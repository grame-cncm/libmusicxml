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

#include <regex>

#include "enableExtraOahIfDesired.h"
#ifdef EXTRA_OAH_IS_ENABLED
  #include "extraOah.h"
#endif

#include "enableTracingIfDesired.h"
#ifdef TRACING_IS_ENABLED
  #include "traceOah.h"
#endif

#include "lpsr.h"

#include "oahOah.h"
#include "generalOah.h"

#include "musicxmlOah.h"
#include "mxmlTreeOah.h"
#include "mxmlTree2msrOah.h"
#include "msrOah.h"
#include "msr2lpsrOah.h"
#include "lpsrOah.h"
#include "lpsr2lilypondOah.h"
#include "lilypondOah.h"

#include "version.h"

#include "xml2lyInsiderOahHandler.h"


using namespace std;

namespace MusicXML2
{
/*
  ENFORCE_TRACE_OAH can be used to issue trace messages
  before gGlobalTraceOahGroup->fTraceOah has been initialized
*/
//#define ENFORCE_TRACE_OAH

//_______________________________________________________________________________
string xml2lyAboutInformation ()
{
  return
R"(What xml2ly does:

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

    The activity log and warning/error messages go to standard error.)";
}

//______________________________________________________________________________
S_xml2lyInsiderOahHandler xml2lyInsiderOahHandler::create (
  string                 executableName,
  string                 executableAboutInformation,
  string                 handlerHeader,
  oahHandlerUsedThruKind handlerUsedThruKind)
{
  // create the insider handler
  xml2lyInsiderOahHandler* o = new
    xml2lyInsiderOahHandler (
      executableName,
      executableAboutInformation,
      handlerHeader,
      handlerUsedThruKind);
  assert (o!=0);

  return o;
}

xml2lyInsiderOahHandler::xml2lyInsiderOahHandler (
  string                 executableName,
  string                 executableAboutInformation,
  string                 handlerHeader,
  oahHandlerUsedThruKind handlerUsedThruKind)
  : oahHandler (
      executableName,
      executableAboutInformation,
      handlerHeader,
R"(                      Welcome to xml2ly,
              the MusicXML to LilyPond translator
          delivered as part of the libmusicxml2 library.
      https://github.com/grame-cncm/libmusicxml/tree/lilypond
)",
/* JMI
R"(
Usage: xml2ly ([options] | [MusicXMLFile|-])+
)"
*/
      usageFromUsedThruKind (handlerUsedThruKind)
    )
{
#ifdef TRACING_IS_ENABLED
#ifdef ENFORCE_TRACE_OAH
  gLogStream <<
    "Initializing xml2ly insider options handler \"" <<
    fHandlerHeader <<
    "\"" <<
    endl;
#endif
#endif

  // create the xml2ly prefixes
  createTheXml2lyPrefixes ();

  // create the xml2xml option groups
  createTheXml2lyOptionGroups (
    executableName);
}

xml2lyInsiderOahHandler::~xml2lyInsiderOahHandler ()
{}

//______________________________________________________________________________
string xml2lyInsiderOahHandler::usageFromUsedThruKind (
   oahHandlerUsedThruKind handlerUsedThruKind) const
{
  string result;

  switch (handlerUsedThruKind) {
    case kHandlerUsedThruUnknown:
      {
        stringstream s;

        s <<
          "kHandlerUsedThruUnknown found in usageFromUsedThruKind() in handler \"" <<
          fHandlerHeader <<
          "\"";

        oahInternalError (s.str ());
      }
      break;
    case kHandlerUsedThruArgcAndArgv:
      result = fHandlerExecutableName + " ([options] | [MusicXMLFile|-])+";
      break;
    case kHandlerUsedThruOptionsVector:
      result = "Usage: [options]";
      break;
  } // switch

  result = "Usage: " + result;

  return result;
}

//______________________________________________________________________________
void xml2lyInsiderOahHandler::createTheXml2lyPrefixes ()
{
#ifdef TRACING_IS_ENABLED
#ifdef ENFORCE_TRACE_OAH
  gLogStream <<
    "Creating the xml2ly prefixes" <<
    endl;
#endif
#endif

  gIndenter++;

#ifdef TRACING_IS_ENABLED
  // the 'trace' prefixes
  // --------------------------------------

 fShortTracePrefix =
    oahPrefix::create (
      "t", "t",
      "'-t=abc,wxyz' is equivalent to '-tabc, -twxyz'");
  registerPrefixInHandler (
    fShortTracePrefix);

  fLongTracePrefix =
    oahPrefix::create (
      "trace", "trace-",
      "'-trace=abc,yz' is equivalent to '-trace-abc, -trace-yz'");
  registerPrefixInHandler (
    fLongTracePrefix);
#endif

  // the 'help' prefixes
  // --------------------------------------

  registerPrefixInHandler (
    oahPrefix::create (
      "help", "help-",
      "'-help=abc,yz' is equivalent to '-help-abc, -help-yz'"));

  registerPrefixInHandler (
    oahPrefix::create (
      "h", "h",
      "'-h=abc,wxyz' is equivalent to '-habc, -hwxyz'"));

  // the 'display' prefixes
  // --------------------------------------

  registerPrefixInHandler (
    oahPrefix::create (
      "display", "display-",
      "'-display=abc,yz' is equivalent to '-display-abc, -display-yz'"));

  registerPrefixInHandler (
    oahPrefix::create (
      "d", "d",
      "'-d=abc,wxyz' is equivalent to '-dabc, -dwxyz'"));

  // the 'omit' prefixes
  // --------------------------------------

  registerPrefixInHandler (
    oahPrefix::create (
      "omit", "omit-",
      "'-omit=abc,yz' is equivalent to '-omit-abc, -omit-yz'"));

  registerPrefixInHandler (
    oahPrefix::create (
      "o", "o",
      "'-o=abc,wxyz' is equivalent to '-oabc, -owxyz'"));

  // the 'ignore-redundant' prefixes
  // --------------------------------------

  fShortIgnoreRedundantPrefix =
    oahPrefix::create (
      "ir", "ir",
      "'-ir=abc,yz' is equivalent to '-irabc, -iryz'");
  registerPrefixInHandler (
    fShortIgnoreRedundantPrefix);

  fLongIgnoreRedundantPrefix =
    oahPrefix::create (
      "ignore-redundant", "ignore-redundant-",
      "'-ignore-redundant=abc,yz' is equivalent to '-ignore-redundant-abc, -ignore-redundant-yz'");
  registerPrefixInHandler (
    fLongIgnoreRedundantPrefix);

  // the 'delay-rests' prefixes
  // --------------------------------------

  fShortDelayRestsPrefix =
    oahPrefix::create (
      "dr", "dr",
      "'-dr=abc,yz' is equivalent to '-drabc, -dryz'");
  registerPrefixInHandler (
    fShortDelayRestsPrefix);

  fLongDelayRestsPrefix =
    oahPrefix::create (
      "delay-rests", "delay-rests-",
      "'-delay-rests=abc,yz' is equivalent to '-delay-rests-abc, -delay-rests-yz'");
  registerPrefixInHandler (
    fLongDelayRestsPrefix);

  gIndenter--;
}

//______________________________________________________________________________
void xml2lyInsiderOahHandler::createTheXml2lyOptionGroups (
  string executableName)
{
#ifdef TRACING_IS_ENABLED
#ifdef ENFORCE_TRACE_OAH
  gLogStream <<
    "Creating the xml2ly insider option groups" <<
    fHandlerHeader <<
    "\"" <<
    endl;
#endif
#endif

  // initialize options handling, phase 1
  // ------------------------------------------------------

#ifdef TRACING_IS_ENABLED
  // create the trace OAH group
  appendGroupToHandler (
    createGlobalTraceOahGroup (
      fShortTracePrefix,
      fLongTracePrefix));
#endif

  // create the OAH OAH group
  appendGroupToHandler (
    createGlobalOahOahGroup (
      executableName));

  // create the general OAH group
  appendGroupToHandler (
    createGlobalGeneralOahGroup ());

  // initialize the library
  // ------------------------------------------------------

  initializeMSR ();
  initializeLPSR ();

  // initialize options handling, phase 2
  // ------------------------------------------------------

  // create the xml2ly OAH group
  appendGroupToHandler (
    createGlobalXml2lyInsiderOahGroup ());

  // create the MusicXML OAH group
  appendGroupToHandler (
    createGlobalMusicxmlOahGroup ());

  // create the mxmlTree OAH group
  appendGroupToHandler (
    createGlobalMxmlTreeOahGroup ());

  // create create the mxmlTree2msr OAH group
  appendGroupToHandler (
    createGlobalMxmlTree2msrOahGroup (
      fShortIgnoreRedundantPrefix,
      fLongIgnoreRedundantPrefix,
      fShortDelayRestsPrefix,
      fLongDelayRestsPrefix));

  // create the MSR OAH group
  appendGroupToHandler (
    createGlobalMsrOahGroup ());

  // create the msr2lpsr OAH group
  appendGroupToHandler (
    createGlobalMsr2lpsrOahGroup ());

  // create the LPSR OAH group
  appendGroupToHandler (
    createGlobalLpsrOahGroup ());

  // create the lpsr2lilypond OAH group
  appendGroupToHandler (
    createGlobalLpsr2lilypondOahGroup ());

  // create the LilyPond OAH group
  appendGroupToHandler (
    createGlobalLilypondOahGroup ());

#ifdef EXTRA_OAH_IS_ENABLED
  // create the extra OAH group
  appendGroupToHandler (
    createGlobalExtraOahGroup ());
#endif

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceOahDetails ()) {
    // print the options handler initial state
    gLogStream <<
      "xml2lyInsiderOahHandler has been initialized as:" <<
      endl;

    gIndenter++;

    print (gLogStream);

    gLogStream <<
      endl << endl;

    gIndenter--;
  }
#endif

#ifdef TRACING_IS_ENABLED
if (gGlobalTraceOahGroup->getTraceOahDetails ()) {
    gLogStream <<
      "xml2lyInsiderOahHandler help:" <<
      endl;

    this->printHelp (gOutputStream);
  }
#endif
}

//______________________________________________________________________________
void xml2lyInsiderOahHandler::checkOptionsAndArgumentsFromArgcAndArgv () const
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceOah ()) {
    gLogStream <<
      "checking options and arguments from argc/argv in \"" <<
      fHandlerHeader <<
      "\"" <<
      endl;
  }
#endif

  checkSingleInputSourceInArgumentsVector ();
}

//______________________________________________________________________________
string xml2lyInsiderOahHandler::fetchOutputFileNameFromTheOptions () const
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceOah ()) {
    gLogStream <<
      "Fetching the output file name from the options in OAH handler \"" <<
      fHandlerHeader <<
      "\"" <<
      endl;
  }
#endif

  string result;

  S_oahStringAtom
    outputFileNameStringAtom =
      gGlobalXml2lyInsiderOahGroup->
        getOutputFileNameStringAtom ();

  S_oahBooleanAtom
    autoOutputFileNameAtom =
      gGlobalXml2lyInsiderOahGroup->
        getAutoOutputFileNameAtom ();

  bool
    outputFileNameHasBeenSet =
      outputFileNameStringAtom->
        getVariableHasBeenSet ();

  bool
    autoOutputFileNameHasBeenSet =
      autoOutputFileNameAtom->
        getVariableHasBeenSet ();

  if (outputFileNameHasBeenSet) {
    if (autoOutputFileNameHasBeenSet) {
      // '-o, -output-file-name' has been chosen
      // '-aofn, -auto-output-file-name' has been chosen
      stringstream s;

      s <<
        "options' " <<
        outputFileNameStringAtom->fetchNames () <<
        "' and '" <<
        autoOutputFileNameAtom->fetchNames () <<
        "' cannot be chosen simultaneously" <<
        "\")";

      oahError (s.str ());
    }
    else {
      // '-o, -output-file-name' has been chosen
      // '-aofn, -auto-output-file-name' has NOT been chosen
      result =
        outputFileNameStringAtom->
          getStringVariable ();
    }
  }

  else {
    if (autoOutputFileNameHasBeenSet) {
      // '-o, -output-file-name' has NOT been chosen
      // '-aofn, -auto-output-file-name' has been chosen
      string
        inputSourceName =
          gGlobalOahOahGroup->getInputSourceName ();

      // determine output file base name
      if (inputSourceName == "-") {
        result = "stdin";
      }

      else {
        // determine output file name,
        result =
          baseName (inputSourceName);

        size_t
          posInString =
            result.rfind ('.');

        // remove file extension
        if (posInString != string::npos) {
          result.replace (
            posInString,
            result.size () - posInString,
            "");
        }
      }

#ifdef TRACING_IS_ENABLED
      if (gGlobalTraceOahGroup->getTraceOah ()) {
        gLogStream <<
          "xml2lyInsiderOahHandler::fetchOutputFileNameFromTheOptions(): result 1 = \"" <<
          result <<
          "\"" <<
          endl;
      }
#endif

      // append the file extension to the output file name
       result += ".ly";

#ifdef TRACING_IS_ENABLED
      if (gGlobalTraceOahGroup->getTraceOah ()) {
        gLogStream <<
          "xml2lyInsiderOahHandler::fetchOutputFileNameFromTheOptions(): result 2 = " <<
          result <<
          "\"" <<
          endl;
      }
#endif
    }

    else {
      // '-o, -output-file-name' has NOT been chosen
      // '-aofn, -auto-output-file-name' has NOT been chosen
      // nothing to do
    }
  }

  return result;
}

//______________________________________________________________________________
void xml2lyInsiderOahHandler::checkHandlerOptionsConsistency ()
{}

//______________________________________________________________________________
void xml2lyInsiderOahHandler::enforceHandlerQuietness ()
{
#ifdef TRACING_IS_ENABLED
  gGlobalTraceOahGroup->
    enforceGroupQuietness ();
#endif

  gGlobalXml2lyInsiderOahGroup->
    enforceGroupQuietness ();

  gGlobalGeneralOahGroup->
    enforceGroupQuietness ();

  gGlobalMusicxmlOahGroup->
    enforceGroupQuietness ();

  gGlobalMxmlTreeOahGroup->
    enforceGroupQuietness ();

  gGlobalMsrOahGroup->
    enforceGroupQuietness ();

  gGlobalMsr2lpsrOahGroup->
    enforceGroupQuietness ();

  gGlobalLpsrOahGroup->
    enforceGroupQuietness ();

  gGlobalLpsr2lilypondOahGroup->
    enforceGroupQuietness ();

  gGlobalLilypondOahGroup->
    enforceGroupQuietness ();

#ifdef EXTRA_OAH_IS_ENABLED
  gGlobalExtraOahGroup->
    enforceGroupQuietness ();
#endif
}

//______________________________________________________________________________
void xml2lyInsiderOahGroup::enforceGroupQuietness ()
{}

//______________________________________________________________________________
void xml2lyInsiderOahGroup::checkGroupOptionsConsistency ()
{}

//______________________________________________________________________________
void xml2lyInsiderOahGroup::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> xml2lyInsiderOahGroup::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_xml2lyInsiderOahGroup>*
    p =
      dynamic_cast<visitor<S_xml2lyInsiderOahGroup>*> (v)) {
        S_xml2lyInsiderOahGroup elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching xml2lyInsiderOahGroup::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void xml2lyInsiderOahGroup::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> xml2lyInsiderOahGroup::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_xml2lyInsiderOahGroup>*
    p =
      dynamic_cast<visitor<S_xml2lyInsiderOahGroup>*> (v)) {
        S_xml2lyInsiderOahGroup elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching xml2lyInsiderOahGroup::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void xml2lyInsiderOahGroup::browseData (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> xml2lyInsiderOahGroup::browseData ()" <<
      endl;
  }
#endif

  oahGroup::browseData (v);
}

//______________________________________________________________________________
void xml2lyInsiderOahHandler::print (ostream& os) const
{
  const unsigned int fieldWidth = 27;

  os <<
    "xml2lyInsiderOahHandler '" << fHandlerHeader << "':" <<
    endl;

  gIndenter++;

  printHandlerEssentials (
    os, fieldWidth);
  os << endl;

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

  gIndenter--;

  os << endl;
}

ostream& operator<< (ostream& os, const S_xml2lyInsiderOahHandler& elt)
{
  elt->print (os);
  return os;
}

//_______________________________________________________________________________
S_xml2lyInsiderOahGroup gGlobalXml2lyInsiderOahGroup;

S_xml2lyInsiderOahGroup xml2lyInsiderOahGroup::create ()
{
  xml2lyInsiderOahGroup* o = new xml2lyInsiderOahGroup ();
  assert (o!=0);

  return o;
}

xml2lyInsiderOahGroup::xml2lyInsiderOahGroup ()
  : oahGroup (
    "xml2ly",
    "hx2l", "help-xml2ly",
R"(Options that are used by xml2ly are grouped here.)",
    kElementVisibilityWhole)
{
  initializeXml2lyInsiderOahGroup ();
}

xml2lyInsiderOahGroup::~xml2lyInsiderOahGroup ()
{}

//_______________________________________________________________________________
void xml2lyInsiderOahGroup::initializeXml2lyInsiderOahGroup ()
{
#ifdef TRACING_IS_ENABLED
#ifdef ENFORCE_TRACE_OAH
  gLogStream << left <<
    "Initializing \"" <<
    fGroupHeader <<
    "\" group" <<
    endl;
#endif
#endif

  // output
  // --------------------------------------

  createInsiderOutputSubGroup ();

  // quit after some passes
  // --------------------------------------

  createInsiderQuitSubGroup ();
}

//_______________________________________________________________________________
void xml2lyInsiderOahGroup::createInsiderOutputSubGroup ()
{
#ifdef TRACING_IS_ENABLED
#ifdef ENFORCE_TRACE_OAH
  gLogStream << left <<
    "Creating insider output subgroup in \"" <<
    fGroupHeader <<
    "\"" <<
    endl;
#endif
#endif

  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Output file",
        "hx2lxof", "help-xml2ly-output-file",
R"()",
      kElementVisibilityWhole,
      this);

  appendSubGroupToGroup (subGroup);

  // output filename

  fOutputFileNameStringAtom =
    oahStringAtom::create (
      "o", "output-file-name",
R"(Write MusicXML code to file FILENAME instead of standard output.)",
      "FILENAME",
      "outputFileName",
      fOutputFileName);

  subGroup->
    appendAtomToSubGroup (
      fOutputFileNameStringAtom);

  // auto output filename

  fAutoOutputFileName = false;

  fAutoOutputFileNameAtom =
    oahBooleanAtom::create (
      "aofn", "auto-output-file-name",
R"(This option can only be used when reading from a file.
Write MusicXML code to a file in the current working directory.
The file name is derived from that of the input file,
replacing any suffix after the the '.' by 'xml'
or adding '.xml' if none is present.)",
      "autoOutputFileName",
      fAutoOutputFileName);

  subGroup->
    appendAtomToSubGroup (
      fAutoOutputFileNameAtom);
}

//_______________________________________________________________________________
void xml2lyInsiderOahGroup::createInsiderQuitSubGroup ()
{
#ifdef TRACING_IS_ENABLED
#ifdef ENFORCE_TRACE_OAH
  gLogStream << left <<
    "Creating insider quit subgroup in \"" <<
    fGroupHeader <<
    "\"" <<
    endl;
#endif
#endif

  S_oahSubGroup
    quitAfterSomePassesSubGroup =
      oahSubGroup::create (
        "Quit after some passes",
        "hm2lquit", "help-msr2ly-quit",
R"()",
      kElementVisibilityWhole,
      this);

  appendSubGroupToGroup (quitAfterSomePassesSubGroup);

  // quit after pass 2a

  fQuitAfterPass2a = false;

  S_oahBooleanAtom
    quit2aOahBooleanAtom =
      oahBooleanAtom::create (
        "q2a", "quitAfterPass-2a",
R"(Quit after pass 2a, i.e. after conversion
of the MusicXML tree to an MSR skeleton.)",
        "quitAfterPass2a",
        fQuitAfterPass2a);

  quitAfterSomePassesSubGroup->
    appendAtomToSubGroup (
      quit2aOahBooleanAtom);

  // quit after pass 2b

  fQuitAfterPass2b = false;

  S_oahBooleanAtom
    quit2bOahBooleanAtom =
      oahBooleanAtom::create (
        "q2b", "quitAfterPass-2b",
R"(Quit after pass 2b, i.e. after conversion
of the MusicXML tree to MSR.)",
        "quitAfterPass2b",
        fQuitAfterPass2b);

  quitAfterSomePassesSubGroup->
    appendAtomToSubGroup (
      quit2bOahBooleanAtom);
}

//______________________________________________________________________________
void xml2lyInsiderOahGroup::printXml2lyInsiderOahGroupValues (unsigned int fieldWidth)
{
  gLogStream <<
    "The xml2ly options are:" <<
    endl;

  gIndenter++;

  // output file
  // --------------------------------------

  gLogStream << left <<
    setw (fieldWidth) << "Output file:" <<
    endl;

  gIndenter++;

  gLogStream << left <<
    setw (fieldWidth) << "outputFileName" << " : \"" <<
    fOutputFileName <<
    "\"" <<
    endl <<
    setw (fieldWidth) << "autoOutputFileName" << " : \"" <<
    booleanAsString (fAutoOutputFileName) <<
    "\"" <<
    endl;

  gIndenter--;

  // quit after some passes
  // --------------------------------------

  gLogStream <<
    "Quit after some passes:" <<
    endl;

  gIndenter++;

  gLogStream << left <<
    setw (fieldWidth) << "quitAfterPass2a" << " : " <<
    booleanAsString (fQuitAfterPass2a) <<
    endl <<
    setw (fieldWidth) << "quitAfterPass2b" << " : " <<
    booleanAsString (fQuitAfterPass2b) <<
    endl;

  gIndenter--;

  gIndenter--;
}

//______________________________________________________________________________
S_xml2lyInsiderOahGroup createGlobalXml2lyInsiderOahGroup ()
{
#ifdef TRACING_IS_ENABLED
#ifdef ENFORCE_TRACE_OAH
  gLogStream <<
    "Creating global xml2ly OAH group" <<
    endl;
#endif
#endif

  // protect library against multiple initializations
  if (! gGlobalXml2lyInsiderOahGroup) {

    // create the global OAH group
    // ------------------------------------------------------

    gGlobalXml2lyInsiderOahGroup =
      xml2lyInsiderOahGroup::create ();
    assert (gGlobalXml2lyInsiderOahGroup != 0);

    // append versions information to list
    // ------------------------------------------------------

    appendVersionToVersionInfoList (
      "Initial",
      "early 2016",
      "Start as xml2lilypond, a clone of xml2guido");

    appendVersionToVersionInfoList (
      "0.94",
      "November 2020",
      "OAH finalization");
  }

  // return the global OAH group
  return gGlobalXml2lyInsiderOahGroup;
}


}
