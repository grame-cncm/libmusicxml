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
#include "outputFileOah.h"
#include "generalOah.h"

#include "msdl2msdrOah.h"

#include "musicxmlOah.h"
#include "mxmlTreeOah.h"
#include "mxmlTree2msrOah.h"
#include "msrOah.h"
#include "msr2lpsrOah.h"
#include "lpsrOah.h"
#include "lpsr2lilypondOah.h"
#include "lilypondOah.h"

#include "version.h"

#include "msdl2lyInsiderHandler.h"


using namespace std;

namespace MusicXML2
{

//______________________________________________________________________________
S_msdl2lyInsiderHandler msdl2lyInsiderHandler::create (
  string                 executableName,
  string                 handlerHeader,
  oahHandlerUsedThruKind handlerUsedThruKind)
{
  // create the insider handler
  msdl2lyInsiderHandler* o = new
    msdl2lyInsiderHandler (
      executableName,
      handlerHeader,
      handlerUsedThruKind);
  assert (o!=0);

  return o;
}

msdl2lyInsiderHandler::msdl2lyInsiderHandler (
  string                 executableName,
  string                 handlerHeader,
  oahHandlerUsedThruKind handlerUsedThruKind)
  : oahInsiderHandler (
      executableName,
      handlerHeader,
R"(                       Welcome to msdl2ly,
              the MusicXML to LilyPond translator
          delivered as part of the libmusicmsdl2 library.
      https://github.com/grame-cncm/libmusicxml/tree/lilypond
)",
/* JMI
R"(
Usage: msdl2ly [option]* [MSDLFile] [option]*
)"
*/
      usageFromUsedThruKind (handlerUsedThruKind)
    )
{
#ifdef TRACING_IS_ENABLED
  if (getTraceOah ()) {
    gLogStream <<
      "Initializing msdl2ly insider options handler \"" <<
      fHandlerHeader <<
      "\"" <<
      endl;
  }
#endif

  // create the msdl2ly prefixes
  createTheMsdl2lyPrefixes ();

  // create the msdl2xml option groups
  createTheMsdl2lyOptionGroups (
    executableName);
}

msdl2lyInsiderHandler::~msdl2lyInsiderHandler ()
{}

//_______________________________________________________________________________
string msdl2lyInsiderHandler::handlerExecutableAboutInformation () const
{
  return
R"(What msdl2ly does:

    This multi-pass translator basically performs 5 passes:
        Pass 1:  reads the contents of MusicXMLFile or stdin ('-')
                 and converts it to a MusicXML tree;
        Pass 2a: converts that MusicXML treeinto
                 a Music Score Representation (MSR) skeleton;
        Pass 2b: populates the MSR skeleton from the MusicXML tree
                 to get a full MSR;
        Pass 3:  converts the MSR into a
                 LilyPond Score Representation (LPSR);
        Pass 4:  converts the LPSR to LilyPond code
                 and writes it to standard output.

    Other passes are performed according to the options, such as
    printing views of the internal data or printing a summary of the score.

    The activity log and warning/error messages go to standard error.)";
}

//______________________________________________________________________________
string msdl2lyInsiderHandler::usageFromUsedThruKind (
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
      result = fHandlerExecutableName + " ([options] | [MSDLFile])+";
      break;
    case kHandlerUsedThruOptionsVector:
      result = "Usage: [options]";
      break;
  } // switch

  result = "Usage: " + result;

  return result;
}

//______________________________________________________________________________
void msdl2lyInsiderHandler::createTheMsdl2lyPrefixes ()
{
#ifdef TRACING_IS_ENABLED
  if (getTraceOah ()) {
    gLogStream <<
      "Creating the msdl2ly prefixes" <<
      endl;
  }
#endif

  createTheCommonPrefixes ();
}

//______________________________________________________________________________
void msdl2lyInsiderHandler::createTheMsdl2lyOptionGroups (
  string executableName)
{
#ifdef TRACING_IS_ENABLED
  if (getTraceOah ()) {
    gLogStream <<
      "Creating the msdl2ly insider option groups" <<
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
  initializeLPSR ();

  // initialize options handling, phase 2
  // ------------------------------------------------------

  // create the msdl2ly OAH group
  appendGroupToHandler (
    createGlobalMsdl2lyInsiderOahGroup ());

  // create the MusicXML OAH group
  appendGroupToHandler (
    createGlobalMusicxmlOahGroup ());

  // create the mxmlTree OAH group
  appendGroupToHandler (
    createGlobalMxmlTreeOahGroup ());

  // create create the mxmlTree2msr OAH group
  appendGroupToHandler (
    createGlobalMxmlTree2msrOahGroup (
      this));

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
      "msdl2lyInsiderHandler has been initialized as:" <<
      endl;

    ++gIndenter;

    print (gLogStream);

    gLogStream <<
      endl << endl;

    --gIndenter;
  }
#endif

#ifdef TRACING_IS_ENABLED
if (gGlobalTraceOahGroup->getTraceOahDetails ()) {
    gLogStream <<
      "msdl2lyInsiderHandler help:" <<
      endl;

    this->printHelp (gOutputStream);
  }
#endif
}

//______________________________________________________________________________
void msdl2lyInsiderHandler::checkOptionsAndArgumentsFromArgcAndArgv () const
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
string msdl2lyInsiderHandler::fetchOutputFileNameFromTheOptions () const
{
  return ""; // JMI
}

//______________________________________________________________________________
void msdl2lyInsiderHandler::checkHandlerOptionsConsistency ()
{}

//______________________________________________________________________________
void msdl2lyInsiderHandler::enforceHandlerQuietness ()
{
#ifdef TRACING_IS_ENABLED
  gGlobalTraceOahGroup->
    enforceGroupQuietness ();
#endif

  gGlobalMsdl2lyInsiderOahGroup->
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
void msdl2lyInsiderOahGroup::enforceGroupQuietness ()
{}

//______________________________________________________________________________
void msdl2lyInsiderOahGroup::checkGroupOptionsConsistency ()
{}

//______________________________________________________________________________
void msdl2lyInsiderOahGroup::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> msdl2lyInsiderOahGroup::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_msdl2lyInsiderOahGroup>*
    p =
      dynamic_cast<visitor<S_msdl2lyInsiderOahGroup>*> (v)) {
        S_msdl2lyInsiderOahGroup elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching msdl2lyInsiderOahGroup::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void msdl2lyInsiderOahGroup::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> msdl2lyInsiderOahGroup::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_msdl2lyInsiderOahGroup>*
    p =
      dynamic_cast<visitor<S_msdl2lyInsiderOahGroup>*> (v)) {
        S_msdl2lyInsiderOahGroup elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching msdl2lyInsiderOahGroup::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void msdl2lyInsiderOahGroup::browseData (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> msdl2lyInsiderOahGroup::browseData ()" <<
      endl;
  }
#endif

  oahGroup::browseData (v);
}

//______________________________________________________________________________
void msdl2lyInsiderHandler::print (ostream& os) const
{
  const unsigned int fieldWidth = 27;

  os <<
    "msdl2lyInsiderHandler '" << fHandlerHeader << "':" <<
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
      // print the options group
      os << (*i);
      if (++i == iEnd) break;
      os << endl;
    } // for

    --gIndenter;
  }

  --gIndenter;

  os << endl;
}

ostream& operator<< (ostream& os, const S_msdl2lyInsiderHandler& elt)
{
  elt->print (os);
  return os;
}

//_______________________________________________________________________________
S_msdl2lyInsiderOahGroup gGlobalMsdl2lyInsiderOahGroup;

S_msdl2lyInsiderOahGroup msdl2lyInsiderOahGroup::create ()
{
  msdl2lyInsiderOahGroup* o = new msdl2lyInsiderOahGroup ();
  assert (o!=0);

  return o;
}

msdl2lyInsiderOahGroup::msdl2lyInsiderOahGroup ()
  : oahGroup (
    "msdl2ly",
    "hx2l", "help-msdl2ly",
R"(Options that are used by msdl2ly are grouped here.)",
    kElementVisibilityWhole)
{
  initializeMsdl2lyInsiderOahGroup ();
}

msdl2lyInsiderOahGroup::~msdl2lyInsiderOahGroup ()
{}

//_______________________________________________________________________________
void msdl2lyInsiderOahGroup::initializeMsdl2lyInsiderOahGroup ()
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
void msdl2lyInsiderOahGroup::createInsiderQuitSubGroup ()
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
void msdl2lyInsiderOahGroup::printMsdl2lyInsiderOahGroupValues (
  unsigned int fieldWidth)
{
  gLogStream <<
    "The msdl2ly options are:" <<
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
S_msdl2lyInsiderOahGroup createGlobalMsdl2lyInsiderOahGroup ()
{
#ifdef TRACING_IS_ENABLED
  if (getTraceOah ()) {
    gLogStream <<
      "Creating global msdl2ly OAH group" <<
      endl;
  }
#endif

  // protect library against multiple initializations
  if (! gGlobalMsdl2lyInsiderOahGroup) {

    // create the global OAH group
    // ------------------------------------------------------

    gGlobalMsdl2lyInsiderOahGroup =
      msdl2lyInsiderOahGroup::create ();
    assert (gGlobalMsdl2lyInsiderOahGroup != 0);

    // append versions information to list
    // ------------------------------------------------------

    appendVersionToVersionInfoList (
      "Initial",
      "early 2016",
      "Start as msdl2lilypond, a clone of msdl2guido");

    appendVersionToVersionInfoList (
      "0.94",
      "November 2020",
      "OAH finalization");
  }

  // return the global OAH group
  return gGlobalMsdl2lyInsiderOahGroup;
}


}
