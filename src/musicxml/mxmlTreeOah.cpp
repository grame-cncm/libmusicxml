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

#include "version.h"
#include "utilities.h"

#include "setTraceOahIfDesired.h"
#ifdef TRACE_OAH
  #include "traceOah.h"
#endif

#include "messagesHandling.h"

#include "generalOah.h"
#include "mxmlTreeOah.h"


using namespace std;

namespace MusicXML2
{

//_______________________________________________________________________________

S_mxmlTreeOah gGlobalMxmlTreeOah;
S_mxmlTreeOah gGlobalMxmlTreeOahUserChoices;
S_mxmlTreeOah gGlobalMxmlTreeOahWithDetailedTrace;

S_mxmlTreeOah mxmlTreeOah::create (
  S_oahHandler handlerUpLink)
{
  mxmlTreeOah* o = new mxmlTreeOah(
    handlerUpLink);
  assert(o!=0);
  return o;
}

mxmlTreeOah::mxmlTreeOah (
  S_oahHandler handlerUpLink)
  : oahGroup (
    "MxmlTree",
    "hmxmlttr", "help-mxml-element-tree",
R"(These options control the way mxml-element trees are handled.)",
    kElementVisibilityWhole,
    handlerUpLink)
{
/* JMI
  // sanity check
  msgAssert (
    handlerUpLink != nullptr,
    "handlerUpLink is null");
*/

  // append this options group to the options handler
  // if relevant
  if (handlerUpLink) {
    handlerUpLink->
      appendGroupToHandler (this);
  }

  // initialize it
  initializeMxmlTreeOah (false);
}

mxmlTreeOah::~mxmlTreeOah ()
{}

#ifdef TRACE_OAH
void mxmlTreeOah::initializeMxmlTreeTraceOah (
  bool boolOptionsInitialValue)
{
  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Trace",
        "hmxmltt", "help-mxml-tree-trace",
R"()",
        kElementVisibilityWhole,
        this);

  appendSubGroupToGroup (subGroup);

  // fetch the 't' prefix

  S_oahPrefix
    shortTracePrefix =
      fHandlerUpLink->
        fetchPrefixNameInPrefixesMap (
          "t");

  msgAssert (
    shortTracePrefix != nullptr,
    "shortTracePrefix is null");

  // fetch the 'trace' prefix

  S_oahPrefix
    longTracePrefix =
      fHandlerUpLink->
        fetchPrefixNameInPrefixesMap (
          "trace");

  msgAssert (
    longTracePrefix != nullptr,
    "longTracePrefix is null");

/* not used as of may 2020 JMI
  // the 'MusicXML' multiplex booleans atom

  S_oahMultiplexBooleansAtom
    musicXMLMultiplexBooleansAtom =
      oahMultiplexBooleansAtom::create (
        "Trace SHORT_NAME/LONG_NAME mxmlTree handling.",
        "SHORT_NAME",
        "LONG_NAME",
        shortTracePrefix,
        longTracePrefix);

  subGroup->
    appendAtomToSubGroup (
      musicXMLMultiplexBooleansAtom);
*/

  // MusicXML tree visiting

  fTraceMusicXMLTreeVisitors = boolOptionsInitialValue;

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "tmxmltv", "trace-mxml-tree-visitors",
R"(Write a trace of the MusicXML tree visiting activity to standard error.)",
        "traceMusicXMLTreeVisitors",
        fTraceMusicXMLTreeVisitors));
}
#endif

void mxmlTreeOah::initializeMxmlTreeOah (
  bool boolOptionsInitialValue)
{
#ifdef TRACE_OAH
  // trace
  // --------------------------------------
  initializeMxmlTreeTraceOah (
    boolOptionsInitialValue);
#endif
}

S_mxmlTreeOah mxmlTreeOah::createCloneWithDetailedTrace ()
{
  S_mxmlTreeOah
    clone =
      mxmlTreeOah::create (0);
      // 0 not to have it inserted twice in the option handler

  // set the options handler upLink
  clone->fHandlerUpLink =
    fHandlerUpLink;

  // trace
  // --------------------------------------

#ifdef TRACE_OAH
  clone->fTraceMusicXMLTreeVisitors =
    fTraceMusicXMLTreeVisitors;
#endif

  return clone;
}

//______________________________________________________________________________
void mxmlTreeOah::setAllMxmlTreeTraceOah (
  bool boolOptionsInitialValue)
{
#ifdef TRACE_OAH
  // specific trace
  // JMI
#endif
}

//______________________________________________________________________________
void mxmlTreeOah::enforceQuietness ()
{
#ifdef TRACE_OAH
  fTraceMusicXMLTreeVisitors = false;
#endif
}

//______________________________________________________________________________
void mxmlTreeOah::checkOptionsConsistency ()
{
  // JMI
}

//______________________________________________________________________________
void mxmlTreeOah::acceptIn (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gGlobalOahOah->fTraceOahVisitors) {
    gLogOstream <<
      ".\\\" ==> mxmlTreeOah::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_mxmlTreeOah>*
    p =
      dynamic_cast<visitor<S_mxmlTreeOah>*> (v)) {
        S_mxmlTreeOah elem = this;

#ifdef TRACE_OAH
        if (gGlobalOahOah->fTraceOahVisitors) {
          gLogOstream <<
            ".\\\" ==> Launching mxmlTreeOah::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void mxmlTreeOah::acceptOut (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gGlobalOahOah->fTraceOahVisitors) {
    gLogOstream <<
      ".\\\" ==> mxmlTreeOah::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_mxmlTreeOah>*
    p =
      dynamic_cast<visitor<S_mxmlTreeOah>*> (v)) {
        S_mxmlTreeOah elem = this;

#ifdef TRACE_OAH
        if (gGlobalOahOah->fTraceOahVisitors) {
          gLogOstream <<
            ".\\\" ==> Launching mxmlTreeOah::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void mxmlTreeOah::browseData (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gGlobalOahOah->fTraceOahVisitors) {
    gLogOstream <<
      ".\\\" ==> mxmlTreeOah::browseData ()" <<
      endl;
  }
#endif
}

//______________________________________________________________________________
void mxmlTreeOah::printMxmlTreeOahValues (int fieldWidth)
{
  gLogOstream <<
    "The MusicXML options are:" <<
    endl;

  gIndenter++;

#ifdef TRACE_OAH
  // trace
  // --------------------------------------

  gLogOstream <<
    "Trace:" <<
    endl;

  gIndenter++;

  gLogOstream << left <<
    setw (fieldWidth) << "traceMusicXMLTreeVisitors" << " : " <<
    booleanAsString (fTraceMusicXMLTreeVisitors) <<
    endl;

  gIndenter--;
#endif

  gIndenter--;
}

ostream& operator<< (ostream& os, const S_mxmlTreeOah& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
void initializeMxmlTreeOahHandler (
  S_oahHandler handler)
{
#ifdef TRACE_OAH
  if (gGlobalTraceOah->fTraceOah && ! gGlobalGeneralOah->fQuiet) {
    gLogOstream <<
      "Initializing MusicXML options handling" <<
      endl;
  }
#endif

  // protect library against multiple initializations
  static bool pThisMethodHasBeenRun = false;

  if (! pThisMethodHasBeenRun) {
    // create the MusicXML options
    // ------------------------------------------------------

    gGlobalMxmlTreeOahUserChoices = mxmlTreeOah::create (
      handler);
    assert(gGlobalMxmlTreeOahUserChoices != 0);

    gGlobalMxmlTreeOah =
      gGlobalMxmlTreeOahUserChoices;

    // prepare for measure detailed trace
    // ------------------------------------------------------

  /* JMI
    gGlobalMxmlTreeOahWithDetailedTrace =
      gGlobalMxmlTreeOah->
        createCloneWithDetailedTrace ();
        */

// JMI    pThisMethodHasBeenRun = true;
  }
}


}
