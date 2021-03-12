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

#include "msr2musicxmlManPageOah.h"

#include "msr2musicxmlInsiderHandler.h"


using namespace std;

namespace MusicXML2
{

//______________________________________________________________________________
S_msr2musicxmlInsiderHandler msr2musicxmlInsiderHandler::create (
  string executableName,
  string handlerHeader)
{
  // create the insider handler
  msr2musicxmlInsiderHandler* o = new
    msr2musicxmlInsiderHandler (
      executableName,
      handlerHeader);
  assert (o!=0);

  return o;
}

msr2musicxmlInsiderHandler::msr2musicxmlInsiderHandler (
  string executableName,
  string handlerHeader)
  : oahInsiderHandler (
      executableName,
      handlerHeader,
R"(                      Welcome to msr2musicxml,
              the MusicXML to MusicXML translator
          delivered as part of the libmusicxml2 library.
      https://github.com/grame-cncm/libmusicxml/tree/lilypond
)",
R"(
Usage: msr2musicxml [[option]* [MusicXMLFile|-] [[option]*
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

  // create the msr2musicxml prefixes
  createTheMsr2musicxmlPrefixes ();

  // create the msr2musicxml option groups
  createTheMsr2musicxmlOptionGroups (executableName);
}

msr2musicxmlInsiderHandler::~msr2musicxmlInsiderHandler ()
{}

string msr2musicxmlInsiderHandler::handlerExecutableAboutInformation () const
{
  return
R"(What msr2musicxml does:

    This multi-pass translator basically performs 6 passes:
        Pass 1:  reads the contents of MusicXMLFile or stdin ('-')
                 and converts it to a MusicXML tree;
        Pass 2a: converts that MusicXML treeinto
                 a Music Score Representation (MSR) skeleton;
        Pass 2b: populates the MSR skeleton from the MusicXML tree
                 to get a full MSR;
        Pass 3:  converts the MSR into a second MSR;
        Pass 4:  converts the second MSR into a second MusicXML tree;
        Pass 5:  converts the second MusicXML tree to Guido code
                 and writes it to standard output.

    Other passes are performed according to the options, such as
    printing views of the internal data or printing a summary of the score.

    The activity log and warning/error messages go to standard error.)";
}

//______________________________________________________________________________
void msr2musicxmlInsiderHandler::createTheMsr2musicxmlPrefixes ()
{
#ifdef TRACING_IS_ENABLED
  if (getTraceOah ()) {
    gLogStream <<
      "Creating the msr2musicxml prefixes in \"" <<
      fHandlerHeader <<
      "\"" <<
      endl;
  }
#endif

  createTheCommonPrefixes ();
}

//______________________________________________________________________________
void msr2musicxmlInsiderHandler::createTheMsr2musicxmlOptionGroups (
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

  // create the msr2musicxml OAH group
  appendGroupToHandler (
    createGlobalMsr2musicxmlOahGroup ());

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
void msr2musicxmlInsiderHandler::checkOptionsAndArgumentsFromArgcAndArgv () const
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
string msr2musicxmlInsiderHandler::fetchOutputFileNameFromTheOptions () const
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
          "msr2musicxmlInsiderHandler::fetchOutputFileNameFromTheOptions(): outputFileName 1 = \"" <<
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
          "msr2musicxmlInsiderHandler::fetchOutputFileNameFromTheOptions(): outputFileName 2 = " <<
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
void msr2musicxmlInsiderHandler::checkHandlerOptionsConsistency ()
{}

//______________________________________________________________________________
void msr2musicxmlInsiderHandler::enforceHandlerQuietness ()
{
#ifdef TRACING_IS_ENABLED
  gGlobalTraceOahGroup->
    enforceGroupQuietness ();
#endif

  gGlobalMsr2musicxmlInsiderOahGroup->
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
void msr2musicxmlInsiderOahGroup::enforceGroupQuietness ()
{}

//______________________________________________________________________________
void msr2musicxmlInsiderOahGroup::checkGroupOptionsConsistency ()
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
      "msr2musicxmlInsiderOahGroup: a MusicXML output file name must be chosen with '-o, -output-file-name";

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
void msr2musicxmlInsiderOahGroup::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> msr2musicxmlInsiderOahGroup::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_msr2musicxmlInsiderOahGroup>*
    p =
      dynamic_cast<visitor<S_msr2musicxmlInsiderOahGroup>*> (v)) {
        S_msr2musicxmlInsiderOahGroup elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching msr2musicxmlInsiderOahGroup::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void msr2musicxmlInsiderOahGroup::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> msr2musicxmlInsiderOahGroup::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_msr2musicxmlInsiderOahGroup>*
    p =
      dynamic_cast<visitor<S_msr2musicxmlInsiderOahGroup>*> (v)) {
        S_msr2musicxmlInsiderOahGroup elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching msr2musicxmlInsiderOahGroup::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void msr2musicxmlInsiderOahGroup::browseData (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> msr2musicxmlInsiderOahGroup::browseData ()" <<
      endl;
  }
#endif

  oahGroup::browseData (v);
}

//______________________________________________________________________________
void msr2musicxmlInsiderHandler::print (ostream& os) const
{
  const unsigned int fieldWidth = 27;

  os <<
    "msr2musicxmlInsiderHandler:" <<
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

ostream& operator<< (ostream& os, const S_msr2musicxmlInsiderHandler& elt)
{
  elt->print (os);
  return os;
}

//_______________________________________________________________________________
S_msr2musicxmlInsiderOahGroup gGlobalMsr2musicxmlInsiderOahGroup;

S_msr2musicxmlInsiderOahGroup msr2musicxmlInsiderOahGroup::create ()
{
  msr2musicxmlInsiderOahGroup* o = new msr2musicxmlInsiderOahGroup ();
  assert (o!=0);

  return o;
}

msr2musicxmlInsiderOahGroup::msr2musicxmlInsiderOahGroup ()
  : oahGroup (
    "msr2musicxml",
    "hm2m", "help-msr2musicxml",
R"(Options that are used by msr2musicxml are grouped here.)",
    oahElementVisibilityKind::kElementVisibilityWhole)
{
  initializeMsr2musicxmlInsiderOahGroup ();
}

msr2musicxmlInsiderOahGroup::~msr2musicxmlInsiderOahGroup ()
{}

//_______________________________________________________________________________
void msr2musicxmlInsiderOahGroup::initializeMsr2musicxmlInsiderOahGroup ()
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
}

//______________________________________________________________________________
void msr2musicxmlInsiderOahGroup::printMsr2musicxmlInsiderOahGroupValues (unsigned int fieldWidth)
{
  gLogStream <<
    "The msr2musicxml options are:" <<
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
S_msr2musicxmlInsiderOahGroup createGlobalMsr2musicxmlOahGroup ()
{
#ifdef TRACING_IS_ENABLED
  if (getTraceOah ()) {
    gLogStream <<
      "Creating global msr2musicxml OAH group" <<
      endl;
  }
#endif

  // protect library against multiple initializations
  if (! gGlobalMsr2musicxmlInsiderOahGroup) {

    // create the global OAH group
    // ------------------------------------------------------

    gGlobalMsr2musicxmlInsiderOahGroup =
      msr2musicxmlInsiderOahGroup::create ();
    assert (gGlobalMsr2musicxmlInsiderOahGroup != 0);

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
  return gGlobalMsr2musicxmlInsiderOahGroup;
}


}
