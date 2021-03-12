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

#include "enableExtraOahIfDesired.h"

#include "enableTracingIfDesired.h"
#ifdef TRACING_IS_ENABLED
  #include "traceOah.h"
#endif

#include "msr.h"

#include "oahOah.h"
#include "outputFileOah.h"
#include "generalOah.h"

#include "musicxmlOah.h"
#include "mxmlTreeOah.h"
#include "msr2mxmlTreeOah.h"
#include "msrOah.h"
#include "msr2msrOah.h"
#include "msr2mxmlTreeOah.h"
#include "mxmlTree2msrOah.h"

#include "version.h"

#include "xml2xmlManPageOah.h"

#include "xml2xmlInsiderHandler.h"


using namespace std;

namespace MusicXML2
{

//______________________________________________________________________________
S_xml2xmlInsiderHandler xml2xmlInsiderHandler::create (
  string executableName,
  string handlerHeader)
{
  // create the insider handler
  xml2xmlInsiderHandler* o = new
    xml2xmlInsiderHandler (
      executableName,
      handlerHeader);
  assert (o!=0);

  return o;
}

xml2xmlInsiderHandler::xml2xmlInsiderHandler (
  string executableName,
  string handlerHeader)
  : oahInsiderHandler (
      executableName,
      handlerHeader,
R"(                      Welcome to xml2xml,
              the MusicXML to MusicXML translator
          delivered as part of the libmusicxml2 library.
      https://github.com/grame-cncm/libmusicxml/tree/lilypond
)",
R"(
Usage: xml2xml [[option]* [MusicXMLFile|-] [[option]*
)")
{
#ifdef TRACING_IS_ENABLED
  if (getTraceOah ()) {
    gLogStream <<
      "Initializing \"" <<
      fHandlerHeader <<
      "\" regular options handler" <<
      endl;
  }
#endif

  // create the xml2xml prefixes
  createTheXml2xmlPrefixes ();

  // create the xml2xml option groups
  createTheXml2xmlOptionGroups (executableName);
}

xml2xmlInsiderHandler::~xml2xmlInsiderHandler ()
{}

string xml2xmlInsiderHandler::handlerExecutableAboutInformation () const
{
  return
R"(What xml2xml does:

    This multi-pass translator basically performs 6 passes:
        Pass 1:  reads the contents of MusicXMLFile or stdin ('-')
                 and converts it to a MusicXML tree;
        Pass 2a: converts that MusicXML treeinto
                 a Music Score Representation (MSR) skeleton;
        Pass 2b: populates the MSR skeleton from the MusicXML tree
                 to get a full MSR;
        Pass 3:  converts the MSR into a second MSR;
        Pass 4:  converts the second MSR into a second MusicXML tree;
        Pass 5:  converts the second MusicXML tree to MusicXML code
                 and writes it to standard output.

    Other passes are performed according to the options, such as
    printing views of the internal data or printing a summary of the score.

    The activity log and warning/error messages go to standard error.)";
}

//______________________________________________________________________________
void xml2xmlInsiderHandler::createTheXml2xmlPrefixes ()
{
#ifdef TRACING_IS_ENABLED
  if (getTraceOah ()) {
    gLogStream <<
      "Creating the xml2xml prefixes in \"" <<
      fHandlerHeader <<
      "\"" <<
      endl;
  }
#endif

  createTheCommonPrefixes ();
}

//______________________________________________________________________________
void xml2xmlInsiderHandler::createTheXml2xmlOptionGroups (
  string executableName)
{
#ifdef TRACING_IS_ENABLED
  if (getTraceOah ()) {
    gLogStream <<
      "Creating the prefixes in \"" <<
      fHandlerHeader <<
      "\" insider option groups" <<
      endl;
  }
#endif

  // initialize options handling, phase 1
  // ------------------------------------------------------

#ifdef TRACING_IS_ENABLED
  // create the trace OAH group
  appendGroupToHandler (
    createGlobalTraceOahGroup (
      this));
#endif

  // create the OAH OAH group
  appendGroupToHandler (
    createGlobalOahOahGroup (
      executableName));

  // create the general OAH group
  appendGroupToHandler (
    createGlobalGeneralOahGroup ());

  // create the output file OAH group
  appendGroupToHandler (
    createGlobalOutputFileOahGroup ());

  // initialize the library
  // ------------------------------------------------------

  initializeMSR ();

  // initialize options handling, phase 2
  // ------------------------------------------------------

  // create the xml2xml OAH group
  appendGroupToHandler (
    createGlobalXml2xmlOahGroup ());

  // create the MusicXML OAH group
  appendGroupToHandler (
    createGlobalMusicxmlOahGroup ());

  // create the mxmlTree OAH group
  appendGroupToHandler (
    createGlobalMxmlTreeOahGroup ());

  // create the mxmlTree2msr OAH group
  appendGroupToHandler (
    createGlobalMxmlTree2msrOahGroup (
      this));

  // create the MSR OAH group
  appendGroupToHandler (
    createGlobalMsrOahGroup ());

  // create the msr2msr OAH group
  appendGroupToHandler (
    createGlobalMsr2msrOahGroup ());

  // create the msr2mxmlTree OAH group
  appendGroupToHandler (
    createGlobalMsr2mxmlTreeOahGroup ());

#ifdef EXTRA_OAH_IS_ENABLED
  // create the extra OAH group
  appendGroupToHandler (
    createGlobalExtraOahGroup ());
#endif
}

//______________________________________________________________________________
void xml2xmlInsiderHandler::checkOptionsAndArgumentsFromArgcAndArgv () const
{
#ifdef TRACING_IS_ENABLED
  if (getTraceOah ()) {
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
string xml2xmlInsiderHandler::fetchOutputFileNameFromTheOptions () const
{
#ifdef TRACING_IS_ENABLED
  if (getTraceOah ()) {
    gLogStream <<
      "Fetching the output file name from the options in OAH handler \"" <<
      fHandlerHeader <<
      "\"" <<
      endl;
  }
#endif

  S_oahStringAtom
    outputFileNameStringAtom =
      gGlobalOutputFileOahGroup->
        getOutputFileNameStringAtom ();

  S_oahBooleanAtom
    autoOutputFileNameAtom =
      gGlobalOutputFileOahGroup->
        getAutoOutputFileNameAtom ();

  bool
    outputFileNameHasBeenSet =
      outputFileNameStringAtom->
        getVariableHasBeenSet ();

  bool
    autoOutputFileNameHasBeenSet =
      autoOutputFileNameAtom->
        getVariableHasBeenSet ();

  string outputFileName;

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
      outputFileName =
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
        outputFileName = "stdin";
      }

      else {
        // determine output file name,
        outputFileName =
          baseName (inputSourceName);

        size_t
          posInString =
            outputFileName.rfind ('.');

        // remove file extension
        if (posInString != string::npos) {
          outputFileName.replace (
            posInString,
            outputFileName.size () - posInString,
            "");
        }
      }

#ifdef TRACING_IS_ENABLED
      if (getTraceOah ()) {
        gLogStream <<
          "xml2xmlInsiderHandler::fetchOutputFileNameFromTheOptions(): outputFileName 1 = \"" <<
          outputFileName <<
          "\"" <<
          endl;
      }
#endif

      // append the file extension to the output file name
       outputFileName += ".xml";

#ifdef TRACING_IS_ENABLED
      if (getTraceOah ()) {
        gLogStream <<
          "xml2xmlInsiderHandler::fetchOutputFileNameFromTheOptions(): outputFileName 2 = " <<
          outputFileName <<
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

  return outputFileName;
}

//______________________________________________________________________________
void xml2xmlInsiderHandler::checkHandlerOptionsConsistency ()
{}

//______________________________________________________________________________
void xml2xmlInsiderHandler::enforceHandlerQuietness ()
{
#ifdef TRACING_IS_ENABLED
  gGlobalTraceOahGroup->
    enforceGroupQuietness ();
#endif

  gGlobalXml2xmlInsiderOahGroup->
    enforceGroupQuietness ();

  gGlobalGeneralOahGroup->
    enforceGroupQuietness ();

  gGlobalMusicxmlOahGroup->
    enforceGroupQuietness ();

  gGlobalMxmlTreeOahGroup->
    enforceGroupQuietness ();

  gGlobalMxmlTree2msrOahGroup->
    enforceGroupQuietness ();

  gGlobalMsrOahGroup->
    enforceGroupQuietness ();

  gGlobalMsr2mxmlTreeOahGroup->
    enforceGroupQuietness ();

#ifdef EXTRA_OAH_IS_ENABLED
  gGlobalExtraOahGroup->
    enforceGroupQuietness ();
#endif
}

//______________________________________________________________________________
void xml2xmlInsiderOahGroup::enforceGroupQuietness ()
{}

//______________________________________________________________________________
void xml2xmlInsiderOahGroup::checkGroupOptionsConsistency ()
{
/* JMI

  if (inputSourceName.size () > 0 && inputSourceName == outputFileName) {
    stringstream s;

    s <<
      "\"" << inputSourceName << "\" is both the input and output file name";;

    oahError (s.str ());
  }




  if (! fOutputFileName.size ()) {
    stringstream s;

    s <<
      "xml2xmlInsiderOahGroup: a MusicXML output file name must be chosen with '-o, -output-file-name";

    oahError (s.str ());
  }

  else if (fOutputFileName == gGlobalOahOahGroup->getInputSourceName ()) {
    stringstream s;

    s <<
      "\"" << fOutputFileName << "\" is both the input and output file name";

    oahError (s.str ());
  }
  */
}

//______________________________________________________________________________
void xml2xmlInsiderOahGroup::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> xml2xmlInsiderOahGroup::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_xml2xmlInsiderOahGroup>*
    p =
      dynamic_cast<visitor<S_xml2xmlInsiderOahGroup>*> (v)) {
        S_xml2xmlInsiderOahGroup elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching xml2xmlInsiderOahGroup::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void xml2xmlInsiderOahGroup::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> xml2xmlInsiderOahGroup::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_xml2xmlInsiderOahGroup>*
    p =
      dynamic_cast<visitor<S_xml2xmlInsiderOahGroup>*> (v)) {
        S_xml2xmlInsiderOahGroup elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching xml2xmlInsiderOahGroup::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void xml2xmlInsiderOahGroup::browseData (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> xml2xmlInsiderOahGroup::browseData ()" <<
      endl;
  }
#endif

  oahGroup::browseData (v);
}

//______________________________________________________________________________
void xml2xmlInsiderHandler::print (ostream& os) const
{
  const unsigned int fieldWidth = 27;

  os <<
    "xml2xmlInsiderHandler:" <<
    endl;

  ++gIndenter;

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

    ++gIndenter;

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

    --gIndenter;
  }

  --gIndenter;

  os << endl;
}

ostream& operator<< (ostream& os, const S_xml2xmlInsiderHandler& elt)
{
  elt->print (os);
  return os;
}

//_______________________________________________________________________________
S_xml2xmlInsiderOahGroup gGlobalXml2xmlInsiderOahGroup;

S_xml2xmlInsiderOahGroup xml2xmlInsiderOahGroup::create ()
{
  xml2xmlInsiderOahGroup* o = new xml2xmlInsiderOahGroup ();
  assert (o!=0);

  return o;
}

xml2xmlInsiderOahGroup::xml2xmlInsiderOahGroup ()
  : oahGroup (
    "xml2xml",
    "hm2x", "help-xml2xml",
R"(Options that are used by xml2xml are grouped here.)",
    oahElementVisibilityKind::kElementVisibilityWhole)
{
  initializeXml2xmlInsiderOahGroup ();
}

xml2xmlInsiderOahGroup::~xml2xmlInsiderOahGroup ()
{}

//_______________________________________________________________________________
void xml2xmlInsiderOahGroup::initializeXml2xmlInsiderOahGroup ()
{
#ifdef TRACING_IS_ENABLED
  if (getTraceOah ()) {
  gLogStream << left <<
    "Initializing \"" <<
    fGroupHeader <<
    "\" group" <<
    endl;
  }
#endif

  // quit after some passes
  // --------------------------------------

  createInsiderQuitSubGroup ();
}

//_______________________________________________________________________________
void xml2xmlInsiderOahGroup::createInsiderQuitSubGroup ()
{
#ifdef TRACING_IS_ENABLED
  if (getTraceOah ()) {
  gLogStream << left <<
    "Creating insider quit subgroup in \"" <<
    fGroupHeader <<
    "\"" <<
    endl;
  }
#endif

  S_oahSubGroup
    quitAfterSomePassesSubGroup =
      oahSubGroup::create (
        "Quit after some passes",
        "hm2xquit", "help-msr2xml-quit",
R"()",
      oahElementVisibilityKind::kElementVisibilityWhole,
      this);

  appendSubGroupToGroup (quitAfterSomePassesSubGroup);

  // quit after pass 2a

  fQuitAfterPass2a = false;

  S_oahBooleanAtom
    quit2aOahBooleanAtom =
      oahBooleanAtom::create (
        "qap2a", "quitAfterPass-after-pass2a",
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
        "qap2b", "quitAfterPass-after-pass2b",
R"(Quit after pass 2b, i.e. after conversion
of the MusicXML tree to MSR.)",
        "quitAfterPass2b",
        fQuitAfterPass2b);

  quitAfterSomePassesSubGroup->
    appendAtomToSubGroup (
      quit2bOahBooleanAtom);
}

//______________________________________________________________________________
void xml2xmlInsiderOahGroup::printXml2xmlInsiderOahGroupValues (unsigned int fieldWidth)
{
  gLogStream <<
    "The xml2xml options are:" <<
    endl;

  ++gIndenter;

  // quit after some passes
  // --------------------------------------

  gLogStream <<
    "Quit after some passes:" <<
    endl;

  ++gIndenter;

  gLogStream << left <<
    setw (fieldWidth) << "quitAfterPass2a" << " : " <<
    booleanAsString (fQuitAfterPass2a) <<
    endl <<
    setw (fieldWidth) << "quitAfterPass2b" << " : " <<
    booleanAsString (fQuitAfterPass2b) <<
    endl;

  --gIndenter;

  --gIndenter;
}

//______________________________________________________________________________
S_xml2xmlInsiderOahGroup createGlobalXml2xmlOahGroup ()
{
#ifdef TRACING_IS_ENABLED
  if (getTraceOah ()) {
    gLogStream <<
      "Creating global xml2xml OAH group" <<
      endl;
  }
#endif

  // protect library against multiple initializations
  if (! gGlobalXml2xmlInsiderOahGroup) {

    // create the global OAH group
    // ------------------------------------------------------

    gGlobalXml2xmlInsiderOahGroup =
      xml2xmlInsiderOahGroup::create ();
    assert (gGlobalXml2xmlInsiderOahGroup != 0);

    // append versions information to list
    // ------------------------------------------------------

    appendVersionToVersionInfoList (
      "Initial",
      "April 2020",
      "First draft version");

    appendVersionToVersionInfoList (
      "0.02",
      "November 2020",
      "OAH finalization");
  }

  // return the global OAH group
  return gGlobalXml2xmlInsiderOahGroup;
}


}
