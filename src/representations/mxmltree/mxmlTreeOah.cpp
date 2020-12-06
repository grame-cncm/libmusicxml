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

#include "enableTracingIfDesired.h"
#ifdef TRACING_IS_ENABLED
  #include "traceOah.h"
#endif

#include "messagesHandling.h"

#include "oahOah.h"
#include "generalOah.h"

#include "mxmlTreeOah.h"

#include "oahAtomsCollection.h"


using namespace std;

namespace MusicXML2
{

//_______________________________________________________________________________

S_mxmlTreeOahGroup gGlobalMxmlTreeOahGroup;

S_mxmlTreeOahGroup mxmlTreeOahGroup::create ()
{
  mxmlTreeOahGroup* o = new mxmlTreeOahGroup ();
  assert (o!=0);
  return o;
}

mxmlTreeOahGroup::mxmlTreeOahGroup ()
  : oahGroup (
    "MxmlTree",
    "hmxmlttr", "help-mxml-element-tree",
R"(These options control the way mxml-element trees are handled.)",
    kElementVisibilityWhole)
{
  initializeMxmlTreeOah ();
}

mxmlTreeOahGroup::~mxmlTreeOahGroup ()
{}

#ifdef TRACING_IS_ENABLED
void mxmlTreeOahGroup::initializeMxmlTreeTraceOah ()
{
  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "mxmlTree Trace",
        "hmxmltt", "help-mxml-tree-trace",
R"()",
        kElementVisibilityWhole,
        this);

  appendSubGroupToGroup (subGroup);

  // MusicXML tree visiting

  fTraceMusicXMLTreeVisitors = false;

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "tmxmltv", "trace-mxml-tree-visitors",
R"(Write a trace of the MusicXML tree visiting activity to standard error.)",
        "traceMusicXMLTreeVisitors",
        fTraceMusicXMLTreeVisitors));
}
#endif

void mxmlTreeOahGroup::initializeMxmlTreeOah ()
{
#ifdef TRACING_IS_ENABLED
  // trace
  // --------------------------------------
  initializeMxmlTreeTraceOah ();
#endif
}

//______________________________________________________________________________
void mxmlTreeOahGroup::enforceGroupQuietness ()
{
#ifdef TRACING_IS_ENABLED
  fTraceMusicXMLTreeVisitors = false;
#endif
}

//______________________________________________________________________________
void mxmlTreeOahGroup::checkGroupOptionsConsistency ()
{
  // JMI
}

//______________________________________________________________________________
void mxmlTreeOahGroup::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> mxmlTreeOahGroup::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_mxmlTreeOahGroup>*
    p =
      dynamic_cast<visitor<S_mxmlTreeOahGroup>*> (v)) {
        S_mxmlTreeOahGroup elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching mxmlTreeOahGroup::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void mxmlTreeOahGroup::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> mxmlTreeOahGroup::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_mxmlTreeOahGroup>*
    p =
      dynamic_cast<visitor<S_mxmlTreeOahGroup>*> (v)) {
        S_mxmlTreeOahGroup elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching mxmlTreeOahGroup::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void mxmlTreeOahGroup::browseData (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> mxmlTreeOahGroup::browseData ()" <<
      endl;
  }
#endif
}

//______________________________________________________________________________
void mxmlTreeOahGroup::printMxmlTreeOahValues (int fieldWidth)
{
  gLogStream <<
    "The MusicXML options are:" <<
    endl;

  gIndenter++;

#ifdef TRACING_IS_ENABLED
  // trace
  // --------------------------------------

  gLogStream <<
    "Trace:" <<
    endl;

  gIndenter++;

  gLogStream << left <<
    setw (fieldWidth) << "traceMusicXMLTreeVisitors" << " : " <<
    booleanAsString (fTraceMusicXMLTreeVisitors) <<
    endl;

  gIndenter--;
#endif

  gIndenter--;
}

ostream& operator<< (ostream& os, const S_mxmlTreeOahGroup& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_mxmlTreeOahGroup createGlobalMxmlTreeOahGroup ()
{
#ifdef TRACING_IS_ENABLED
#ifdef ENFORCE_TRACE_OAH
  gLogStream <<
    "Creating global mxmlTree OAH group" <<
    endl;
#endif
#endif

  // protect library against multiple initializations
  if (! gGlobalMxmlTreeOahGroup) {
    // create the options groups
    gGlobalMxmlTreeOahGroup =
      mxmlTreeOahGroup::create ();
    assert (gGlobalMxmlTreeOahGroup != 0);
  }

  // return global OAH group
  return gGlobalMxmlTreeOahGroup;
}


}
