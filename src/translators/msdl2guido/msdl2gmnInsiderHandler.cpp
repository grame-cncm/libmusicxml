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

#include "msdl2msrOah.h"

#include "musicxmlOah.h"
#include "mxmlTreeOah.h"
#include "msr2mxmlTreeOah.h"
#include "msrOah.h"
#include "msr2msrOah.h"
#include "msr2mxmlTreeOah.h"
#include "mxmlTree2msrOah.h"

#include "version.h"

#include "msdl2gmnManPageOah.h"

#include "msdl2gmnInsiderHandler.h"


using namespace std;

namespace MusicXML2
{

//______________________________________________________________________________
S_msdl2gmnInsiderHandler msdl2gmnInsiderHandler::create (
  string executableName,
  string handlerHeader)
{
  // create the insider handler
  msdl2gmnInsiderHandler* o = new
    msdl2gmnInsiderHandler (
      executableName,
      handlerHeader);
  assert (o!=0);

  return o;
}

msdl2gmnInsiderHandler::msdl2gmnInsiderHandler (
  string executableName,
  string handlerHeader)
  : oahInsiderHandler (
      executableName,
      handlerHeader,
R"(                      Welcome to msdl2gmn,
              another MusicXML to Guido translator
          delivered as part of the libmusicmsdl2 library.
      https://github.com/grame-cncm/libmusicxml/tree/lilypond
)",
R"(
Usage: msdl2gmn [option]* [MSDLFile] [option]*
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

  // create the msdl2gmn prefixes
  createTheMsdl2gmnPrefixes ();

  // create the msdl2gmn option groups
  createTheMsdl2gmnOptionGroups (executableName);
}

msdl2gmnInsiderHandler::~msdl2gmnInsiderHandler ()
{}

//_______________________________________________________________________________
string msdl2gmnInsiderHandler::handlerExecutableAboutInformation () const
{
  return
R"(What msdl2gmn does:

    This multi-pass translator basically performs 6 passes:
        Pass 1:  reads the contents of MusicXMLFile or stdin ('-')
                 and converts it to a first MusicXML tree;
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
void msdl2gmnInsiderHandler::createTheMsdl2gmnPrefixes ()
{
#ifdef TRACING_IS_ENABLED
  if (getTraceOah ()) {
    gLogStream <<
      "Creating the msdl2gmn prefixes in \"" <<
      fHandlerHeader <<
      "\"" <<
      endl;
  }
#endif

  createTheCommonPrefixes ();
}

//______________________________________________________________________________
void msdl2gmnInsiderHandler::createTheMsdl2gmnOptionGroups (
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

  // create the msdl2gmn OAH group
  appendGroupToHandler (
    createGlobalMsdl2gmnInsiderOahGroup ());

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
void msdl2gmnInsiderHandler::checkOptionsAndArgumentsFromArgcAndArgv () const
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
string msdl2gmnInsiderHandler::fetchOutputFileNameFromTheOptions () const
{
  return ""; // JMI
}

//______________________________________________________________________________
void msdl2gmnInsiderHandler::checkHandlerOptionsConsistency ()
{}

//______________________________________________________________________________
void msdl2gmnInsiderHandler::enforceHandlerQuietness ()
{
#ifdef TRACING_IS_ENABLED
  gGlobalTraceOahGroup->
    enforceGroupQuietness ();
#endif

  gGlobalMsdl2gmnInsiderOahGroup->
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
void msdl2gmnInsiderOahGroup::enforceGroupQuietness ()
{}

//______________________________________________________________________________
void msdl2gmnInsiderOahGroup::checkGroupOptionsConsistency ()
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
      "msdl2gmnInsiderOahGroup: a MusicXML output file name must be chosen with '-o, -output-file-name";

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
void msdl2gmnInsiderOahGroup::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> msdl2gmnInsiderOahGroup::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_msdl2gmnInsiderOahGroup>*
    p =
      dynamic_cast<visitor<S_msdl2gmnInsiderOahGroup>*> (v)) {
        S_msdl2gmnInsiderOahGroup elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching msdl2gmnInsiderOahGroup::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void msdl2gmnInsiderOahGroup::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> msdl2gmnInsiderOahGroup::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_msdl2gmnInsiderOahGroup>*
    p =
      dynamic_cast<visitor<S_msdl2gmnInsiderOahGroup>*> (v)) {
        S_msdl2gmnInsiderOahGroup elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching msdl2gmnInsiderOahGroup::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void msdl2gmnInsiderOahGroup::browseData (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> msdl2gmnInsiderOahGroup::browseData ()" <<
      endl;
  }
#endif

  oahGroup::browseData (v);
}

//______________________________________________________________________________
void msdl2gmnInsiderHandler::print (ostream& os) const
{
  const unsigned int fieldWidth = 27;

  os <<
    "msdl2gmnInsiderHandler:" <<
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

ostream& operator<< (ostream& os, const S_msdl2gmnInsiderHandler& elt)
{
  elt->print (os);
  return os;
}

//_______________________________________________________________________________
S_msdl2gmnInsiderOahGroup gGlobalMsdl2gmnInsiderOahGroup;

S_msdl2gmnInsiderOahGroup msdl2gmnInsiderOahGroup::create ()
{
  msdl2gmnInsiderOahGroup* o = new msdl2gmnInsiderOahGroup ();
  assert (o!=0);

  return o;
}

msdl2gmnInsiderOahGroup::msdl2gmnInsiderOahGroup ()
  : oahGroup (
    "msdl2gmn",
    "hx2g", "help-msdl2gmn",
R"(Options that are used by msdl2gmn are grouped here.)",
    oahElementVisibilityKind::kElementVisibilityWhole)
{
  initializeMsdl2gmnInsiderOahGroup ();
}

msdl2gmnInsiderOahGroup::~msdl2gmnInsiderOahGroup ()
{}

//_______________________________________________________________________________
void msdl2gmnInsiderOahGroup::initializeMsdl2gmnInsiderOahGroup ()
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
void msdl2gmnInsiderOahGroup::printMsdl2gmnInsiderOahGroupValues (
  unsigned int fieldWidth)
{
  gLogStream <<
    "The msdl2gmn options are:" <<
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
S_msdl2gmnInsiderOahGroup createGlobalMsdl2gmnInsiderOahGroup ()
{
#ifdef TRACING_IS_ENABLED
  if (getTraceOah ()) {
    gLogStream <<
      "Creating global msdl2gmn insider OAH group" <<
      endl;
  }
#endif

  // protect library against multiple initializations
  if (! gGlobalMsdl2gmnInsiderOahGroup) {

    // create the global OAH group
    // ------------------------------------------------------

    gGlobalMsdl2gmnInsiderOahGroup =
      msdl2gmnInsiderOahGroup::create ();
    assert (gGlobalMsdl2gmnInsiderOahGroup != 0);

    // append versions information to list
    // ------------------------------------------------------

    appendVersionToVersionInfoList (
      "Initial",
      "December 2020",
      "First draft version");
  }

  // return the global OAH group
  return gGlobalMsdl2gmnInsiderOahGroup;
}


}
