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

#include "waeMessagesHandling.h"

#include "oahOah.h"
#include "generalOah.h"

#include "msdl2msdrOah.h"

#include "oahAtomsCollection.h"


using namespace std;

namespace MusicXML2
{

//_______________________________________________________________________________

S_msdl2msdrOahGroup gGlobalMsdl2msdrOahGroup;

S_msdl2msdrOahGroup msdl2msdrOahGroup::create ()
{
  msdl2msdrOahGroup* o = new msdl2msdrOahGroup ();
  assert (o != nullptr);
  return o;
}

msdl2msdrOahGroup::msdl2msdrOahGroup ()
  : oahGroup (
    "Msdl2msdr",
    "hmsdl2msdr", "help-msdl-to-msdr",
R"(These options control the way xmlelement are translated to MSDR.)",
    kElementVisibilityWhole)
{
  initializeMsdl2msdr ();
}

msdl2msdrOahGroup::~msdl2msdrOahGroup ()
{}

#ifdef TRACING_IS_ENABLED
void msdl2msdrOahGroup::initializeMsdl2msdrTraceOah ()
{
  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "msdl2msdl Trace",
        "hmsdl2msdlt", "help-msdl-to-msdl-trace",
R"()",
        kElementVisibilityWhole,
        this);

  appendSubGroupToGroup (subGroup);

  // MSDL tokens

  fTraceTokens = false;

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "ttoks", "trace-tokens",
R"(Write a trace of the MSDL tokens handling activity to standard error.)",
        "traceTokens",
        fTraceTokens));
}
#endif

void msdl2msdrOahGroup::initializeMsdl2msdr ()
{
#ifdef TRACING_IS_ENABLED
  // trace
  // --------------------------------------
  initializeMsdl2msdrTraceOah ();
#endif
}

//______________________________________________________________________________
void msdl2msdrOahGroup::enforceGroupQuietness ()
{
#ifdef TRACING_IS_ENABLED
// JMI
#endif
}

//______________________________________________________________________________
void msdl2msdrOahGroup::checkGroupOptionsConsistency ()
{
  // JMI
}

//______________________________________________________________________________
void msdl2msdrOahGroup::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> msdl2msdrOahGroup::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_msdl2msdrOahGroup>*
    p =
      dynamic_cast<visitor<S_msdl2msdrOahGroup>*> (v)) {
        S_msdl2msdrOahGroup elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching msdl2msdrOahGroup::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void msdl2msdrOahGroup::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> msdl2msdrOahGroup::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_msdl2msdrOahGroup>*
    p =
      dynamic_cast<visitor<S_msdl2msdrOahGroup>*> (v)) {
        S_msdl2msdrOahGroup elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching msdl2msdrOahGroup::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void msdl2msdrOahGroup::browseData (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> msdl2msdrOahGroup::browseData ()" <<
      endl;
  }
#endif
}

//______________________________________________________________________________
void msdl2msdrOahGroup::printMsdl2msdrValues (unsigned int fieldWidth)
{
  gLogStream <<
    "The msdl2msdr options are:" <<
    endl;

  ++gIndenter;

#ifdef TRACING_IS_ENABLED
  // trace
  // --------------------------------------

  gLogStream <<
    "Trace:" <<
    endl;

  ++gIndenter;

  // MSDL tokens
  // --------------------------------------

  gLogStream <<
    "MSDL tokens:" <<
    endl;

  ++gIndenter;

  gLogStream << left <<
    setw (fieldWidth) << "traceTokens" << " : " <<
      booleanAsString (fTraceTokens) <<
      endl;

  --gIndenter;
#endif

  --gIndenter;
}

ostream& operator<< (ostream& os, const S_msdl2msdrOahGroup& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_msdl2msdrOahGroup createGlobalMsdl2msdrOahGroup ()
#ifdef TRACING_IS_ENABLED
#ifdef ENFORCE_TRACE_OAH
  gLogStream <<
    "Creating global msdl2msdr OAH group" <<
    endl;
#endif
#endif
{
  // protect library against multiple initializations
  if (! gGlobalMsdl2msdrOahGroup) {
    // create the global OAH group
    gGlobalMsdl2msdrOahGroup =
      msdl2msdrOahGroup::create ();
    assert (gGlobalMsdl2msdrOahGroup != 0);
  }

  // return global OAH group
  return gGlobalMsdl2msdrOahGroup;
}


}
