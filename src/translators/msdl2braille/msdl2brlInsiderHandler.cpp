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

#include "msr.h"
#include "bsr.h"

#include "oahOah.h"
#include "outputFileOah.h"
#include "generalOah.h"

#include "msdl2msrOah.h"

#include "musicxmlOah.h"
#include "mxmlTreeOah.h"
#include "mxmlTree2msrOah.h"
#include "msrOah.h"
#include "msr2bsrOah.h"
#include "bsrOah.h"
#include "bsr2brailleOah.h"
#include "brailleOah.h"

#include "version.h"

#include "msdl2brlInsiderHandler.h"


using namespace std;

namespace MusicXML2
{

//______________________________________________________________________________
S_msdl2brlInsiderHandler msdl2brlInsiderHandler::create (
  string executableName,
  string handlerHeader)
{
  // create the insider handler
  msdl2brlInsiderHandler* o = new
    msdl2brlInsiderHandler (
      executableName,
      handlerHeader);
  assert (o!=0);

  return o;
}

msdl2brlInsiderHandler::msdl2brlInsiderHandler (
  string executableName,
  string handlerHeader)
  : oahInsiderHandler (
      executableName,
      handlerHeader,
R"(                         Welcome to msdl2brl,
             the MusicXML to braille music translator
          delivered as part of the libmusicmsdl2 library.
      https://github.com/grame-cncm/libmusicxml/tree/lilypond
)",
R"(
Usage: msdl2brl [option]* [MSDLFile] [option]*
)")
{
#ifdef TRACING_IS_ENABLED
  if (getTraceOah ()) {
    gLogStream <<
      "Initializing msdl2brl insider options handler \"" <<
      fHandlerHeader <<
      "\"" <<
      endl;
  }
#endif

  // create the msdl2brl prefixes
  createTheMsdl2brlPrefixes ();

  // create the msdl2brl option groups
  createTheMsdl2brlOptionGroups (executableName);
}

msdl2brlInsiderHandler::~msdl2brlInsiderHandler ()
{}

//_______________________________________________________________________________
string msdl2brlInsiderHandler::handlerExecutableAboutInformation () const
{
  return
R"(What msdl2brl does:

    This multi-pass translator basically performs 6 passes:
        Pass 1:  reads the contents of MusicXMLFile or stdin ('-')
                 and converts it to a MusicXML tree;
        Pass 2a: converts that MusicXML treeinto
                 a Music Score Representation (MSR) skeleton;
        Pass 2b: populates the MSR skeleton from the MusicXML tree
                 to get a full MSR;
        Pass 3a: converts the MSR into a
                 Braille Score Representation (BSR)
                 containing one Braille page per MusicXML page;
        Pass 3b: converts the BSRinto another BSR
                 with as many Braille pages as needed
                 to fit the line and page lengthes;
        Pass 4:  converts the BSR to braille music text
                 and writes it to standard output.

    In this preliminary version, pass 3b merely clones the BSR it receives.

    Other passes are performed according to the options, such as
    printing views of the internal data or printing a summary of the score.

    The activity log and warning/error messages go to standard error.)";
}

//______________________________________________________________________________
void msdl2brlInsiderHandler::createTheMsdl2brlPrefixes ()
{
#ifdef TRACING_IS_ENABLED
  if (getTraceOah ()) {
    gLogStream <<
      "Creating the msdl2brl prefixes" <<
      endl;
  }
#endif

  createTheCommonPrefixes ();
}

//______________________________________________________________________________
void msdl2brlInsiderHandler::createTheMsdl2brlOptionGroups (
  string executableName)
{
#ifdef TRACING_IS_ENABLED
  if (getTraceOah ()) {
    gLogStream <<
      "Creating the msdl2brl insider option groups" <<
      fHandlerHeader <<
      "\"" <<
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
  initializeBSR ();

  // initialize options handling, phase 2
  // ------------------------------------------------------

  // create the msdl2brl OAH group
  appendGroupToHandler (
    createGlobalMsdl2brlOahGroup (
      executableName,
      fHandlerHeader));

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

  // create the msr2bsr OAH group
  appendGroupToHandler (
    createGlobalMsr2bsrOahGroup ());

  // create the BSR OAH group
  appendGroupToHandler (
    createGlobalBsrOahGroup ());

  // create the bsr2braille OAH group
  appendGroupToHandler (
    createGlobalBsr2brailleOahGroup ());

  // create the braille OAH group
  appendGroupToHandler (
    createGlobalBrailleOahGroup ());

#ifdef EXTRA_OAH_IS_ENABLED
  // create the extra OAH group
  appendGroupToHandler (
    createGlobalExtraOahGroup ());
#endif

#ifdef TRACING_IS_ENABLED
  if (getTraceOah ()) {
    // print the options handler initial state
    gLogStream <<
      "msdl2brlInsiderHandler has been initialized as:" <<
      endl;

    ++gIndenter;

    print (gLogStream);

    gLogStream <<
      endl;

    --gIndenter;
  }
#endif
}

//______________________________________________________________________________
void msdl2brlInsiderHandler::checkOptionsAndArgumentsFromArgcAndArgv () const
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
string msdl2brlInsiderHandler::fetchOutputFileNameFromTheOptions () const
{
  return ""; // JMI
}

//______________________________________________________________________________
void msdl2brlInsiderHandler::checkHandlerOptionsConsistency ()
{}

//______________________________________________________________________________
void msdl2brlInsiderHandler::enforceHandlerQuietness ()
{
#ifdef TRACING_IS_ENABLED
  gGlobalTraceOahGroup->
    enforceGroupQuietness ();
#endif

  gGlobalMsdl2brlInsiderOahGroup->
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

  gGlobalMsr2bsrOahGroup->
    enforceGroupQuietness ();

  gGlobalBsrOahGroup->
    enforceGroupQuietness ();

  gGlobalBsr2brailleOahGroup->
    enforceGroupQuietness ();

  gGlobalBrailleOahGroup->
    enforceGroupQuietness ();

#ifdef EXTRA_OAH_IS_ENABLED
  gGlobalExtraOahGroup->
    enforceGroupQuietness ();
#endif
}

//______________________________________________________________________________
void msdl2brlInsiderOahGroup::enforceGroupQuietness ()
{}

//______________________________________________________________________________
void msdl2brlInsiderOahGroup::checkGroupOptionsConsistency ()
{}

//______________________________________________________________________________
void msdl2brlInsiderOahGroup::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> msdl2brlInsiderOahGroup::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_msdl2brlInsiderOahGroup>*
    p =
      dynamic_cast<visitor<S_msdl2brlInsiderOahGroup>*> (v)) {
        S_msdl2brlInsiderOahGroup elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching msdl2brlInsiderOahGroup::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void msdl2brlInsiderOahGroup::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> msdl2brlInsiderOahGroup::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_msdl2brlInsiderOahGroup>*
    p =
      dynamic_cast<visitor<S_msdl2brlInsiderOahGroup>*> (v)) {
        S_msdl2brlInsiderOahGroup elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching msdl2brlInsiderOahGroup::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void msdl2brlInsiderOahGroup::browseData (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> msdl2brlInsiderOahGroup::browseData ()" <<
      endl;
  }
#endif

  oahGroup::browseData (v);
}

//______________________________________________________________________________
void msdl2brlInsiderHandler::print (ostream& os) const
{
  const unsigned int fieldWidth = 27;

  os <<
    "msdl2brlInsiderHandler:" <<
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

ostream& operator<< (ostream& os, const S_msdl2brlInsiderHandler& elt)
{
  elt->print (os);
  return os;
}

//_______________________________________________________________________________
S_msdl2brlInsiderOahGroup gGlobalMsdl2brlInsiderOahGroup;

S_msdl2brlInsiderOahGroup msdl2brlInsiderOahGroup::create (
  string executableName,
  string handlerHeader)
{
  msdl2brlInsiderOahGroup* o = new msdl2brlInsiderOahGroup (
    executableName,
    handlerHeader);
  assert (o!=0);

  return o;
}

msdl2brlInsiderOahGroup::msdl2brlInsiderOahGroup (
  string executableName,
  string handlerHeader)
  : oahGroup (
    executableName,
    "hx2b", "help-msdl2brl",
R"(Options that are used by msdl2brl are grouped here.)",
    oahElementVisibilityKind::kElementVisibilityWhole)
{
  initializeMsdl2brlInsiderOahGroup ();
}

msdl2brlInsiderOahGroup::~msdl2brlInsiderOahGroup ()
{}

//_______________________________________________________________________________
void msdl2brlInsiderOahGroup::initializeMsdl2brlInsiderOahGroup ()
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
void msdl2brlInsiderOahGroup::printMsdl2brlInsiderOahGroupValues (
  unsigned int fieldWidth)
{
  gLogStream <<
    "The msdl2brl options are:" <<
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
S_msdl2brlInsiderOahGroup createGlobalMsdl2brlOahGroup (
  string executableName,
  string handlerHeader)
{
#ifdef TRACING_IS_ENABLED
  if (getTraceOah ()) {
    gLogStream <<
      "Creating global msdl2brl OAH group" <<
      endl;
  }
#endif

  // protect library against multiple initializations
  if (! gGlobalMsdl2brlInsiderOahGroup) {

    // create the global OAH group
    // ------------------------------------------------------

    gGlobalMsdl2brlInsiderOahGroup =
      msdl2brlInsiderOahGroup::create (
        executableName,
        handlerHeader);
    assert (gGlobalMsdl2brlInsiderOahGroup != 0);

    // append versions information to list
    // ------------------------------------------------------

    appendVersionToVersionInfoList (
      "Initial",
      "october 2018",
      "Derived from msdl2ly, with an embryonic BSR");

    appendVersionToVersionInfoList (
      "0.02",
      "September 2019",
      "Development freeze due to lack of interest");

    appendVersionToVersionInfoList (
      "0.03",
      "Nobember 2020",
      "OAH finalization");
  }

  // return the global OAH group
  return gGlobalMsdl2brlInsiderOahGroup;
}


}

