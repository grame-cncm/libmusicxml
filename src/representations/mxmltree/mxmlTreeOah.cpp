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
  assert (o != nullptr);
  return o;
}

mxmlTreeOahGroup::mxmlTreeOahGroup ()
  : oahGroup (
    "MxmlTree",
    "hmxmlttr", "help-mxml-element-tree",
R"(These options control the way mxml-element trees are handled.)",
    oahElementVisibilityKind::kElementVisibilityWhole)
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
        oahElementVisibilityKind::kElementVisibilityWhole,
        this);

  appendSubGroupToGroup (subGroup);

  // the 'MusicXML' multiplex booleans atom

  S_oahMultiplexBooleansAtom
    musicXMLMultiplexBooleansAtom =
      oahMultiplexBooleansAtom::create (
       "twhmd", "trace-when-handling-musicxml-data",
        "Trace SHORT_NAME/LONG_NAME when handling MusicXML data.",
        "SHORT_NAME",
        "LONG_NAME",
        gGlobalTraceOahGroup->getShortTracePrefix (),
        gGlobalTraceOahGroup->getLongTracePrefix ());

  subGroup->
    appendAtomToSubGroup (
      musicXMLMultiplexBooleansAtom);

  // encoding

  fTraceEncoding = false;

  S_oahBooleanAtom
    traceEncodingAtom =
      oahBooleanAtom::create (
        "tenc", "trace-encoding",
R"(Encoding)",
        "traceEncoding",
        fTraceEncoding);
  subGroup->
    appendAtomToSubGroup (
      traceEncodingAtom);
  musicXMLMultiplexBooleansAtom->
    addBooleanAtom (
      traceEncodingAtom);

  // divisions

  fTraceDivisions = false;

  S_oahBooleanAtom
    traceDivisionsAtom =
      oahBooleanAtom::create (
        "tdivs", "trace-divisions",
R"(Divisions)",
        "traceDivisions",
        fTraceDivisions);
  subGroup->
    appendAtomToSubGroup (
      traceDivisionsAtom);
  musicXMLMultiplexBooleansAtom->
    addBooleanAtom (
      traceDivisionsAtom);

  // backup

  fTraceBackup = false;

  S_oahBooleanAtom
    traceBackupAtom =
      oahBooleanAtom::create (
        "tbackup", "trace-backup",
R"(Backup)",
        "traceBackup",
        fTraceBackup);
  subGroup->
    appendAtomToSubGroup (
      traceBackupAtom);
  musicXMLMultiplexBooleansAtom->
    addBooleanAtom (
      traceBackupAtom);

  // forward

  fTraceForward = false;

  S_oahBooleanAtom
    traceForwardAtom =
      oahBooleanAtom::create (
        "tforward", "trace-forward",
R"(Forward)",
        "traceForward",
        fTraceForward);
  subGroup->
    appendAtomToSubGroup (
      traceForwardAtom);
  musicXMLMultiplexBooleansAtom->
    addBooleanAtom (
      traceForwardAtom);

  // MusicXML tree

  fTraceMusicXMLTree = false;

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "tmxmlt", "trace-mxml-tree",
R"(Write a trace of the MusicXML tree activity to standard error.)",
        "traceMusicXMLTree",
        fTraceMusicXMLTree));

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

void mxmlTreeOahGroup::enforceGroupQuietness ()
{
#ifdef TRACING_IS_ENABLED
  fTraceMusicXMLTreeVisitors = false;
#endif
}

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

void mxmlTreeOahGroup::printMxmlTreeOahValues (unsigned int fieldWidth)
{
  gLogStream <<
    "The MusicXML options are:" <<
    endl;

  ++gIndenter;

#ifdef TRACING_IS_ENABLED
  // trace
  // --------------------------------------

  gLogStream <<
    "Trace:" <<
    endl;

  ++gIndenter;

  gLogStream << left <<
    setw (fieldWidth) << "traceEncoding" << " : " <<
    booleanAsString (fTraceEncoding) <<
    endl <<

    setw (fieldWidth) << "traceDivisions" << " : " <<
    booleanAsString (fTraceDivisions) <<
    endl <<

    setw (fieldWidth) << "traceBackup" << " : " <<
    booleanAsString (fTraceBackup) <<
    endl <<

    setw (fieldWidth) << "traceForward" << " : " <<
    booleanAsString (fTraceForward) <<
    endl <<

    setw (fieldWidth) << "traceMusicXMLTree" << " : " <<
    booleanAsString (fTraceMusicXMLTree) <<
    endl <<
    setw (fieldWidth) << "traceMusicXMLTreeVisitors" << " : " <<
    booleanAsString (fTraceMusicXMLTreeVisitors) <<
    endl;

  --gIndenter;
#endif

  --gIndenter;
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
  if (getTraceOah ()) {
    gLogStream <<
      "Creating global mxmlTree OAH group" <<
      endl;
  }
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
