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

#include "musicxmlOah.h"

#include "oahAtomsCollection.h"


using namespace std;

namespace MusicXML2
{

//_______________________________________________________________________________

S_musicxmlOahGroup gGlobalMusicxmlOahGroup;

S_musicxmlOahGroup musicxmlOahGroup::create ()
{
  musicxmlOahGroup* o = new musicxmlOahGroup ();
  assert (o != nullptr);
  return o;
}

musicxmlOahGroup::musicxmlOahGroup ()
  : oahGroup (
    "MusicXML",
    "hmxml", "help-mxml",
R"(These options control the way MusicXML data is handled.)",
    oahElementVisibilityKind::kElementVisibilityWhole)
{
  initializeMusicxmlOahGroup ();
}

musicxmlOahGroup::~musicxmlOahGroup ()
{}

#ifdef TRACING_IS_ENABLED
void musicxmlOahGroup::initializeMusicxmlTraceOah ()
{
  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "MusicXML Trace",
        "hmxmltr", "help-musicxml-trace",
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
}
#endif

void musicxmlOahGroup::initializeMusicxmlOahGroup ()
{
#ifdef TRACING_IS_ENABLED
  // trace
  // --------------------------------------
  initializeMusicxmlTraceOah ();
#endif
}

//______________________________________________________________________________
void musicxmlOahGroup::enforceGroupQuietness ()
{}

//______________________________________________________________________________
void musicxmlOahGroup::checkGroupOptionsConsistency ()
{}

//______________________________________________________________________________
void musicxmlOahGroup::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> musicxmlOahGroup::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_musicxmlOahGroup>*
    p =
      dynamic_cast<visitor<S_musicxmlOahGroup>*> (v)) {
        S_musicxmlOahGroup elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching musicxmlOahGroup::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void musicxmlOahGroup::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> musicxmlOahGroup::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_musicxmlOahGroup>*
    p =
      dynamic_cast<visitor<S_musicxmlOahGroup>*> (v)) {
        S_musicxmlOahGroup elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching musicxmlOahGroup::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void musicxmlOahGroup::browseData (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> musicxmlOahGroup::browseData ()" <<
      endl;
  }
#endif
}

//______________________________________________________________________________
void musicxmlOahGroup::printMusicxmlValues (unsigned int fieldWidth)
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
    endl;

  --gIndenter;
#endif

  --gIndenter;
}

ostream& operator<< (ostream& os, const S_musicxmlOahGroup& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_musicxmlOahGroup createGlobalMusicxmlOahGroup ()
{
#ifdef TRACING_IS_ENABLED
  if (getTraceOah ()) {
    gLogStream <<
      "Creating global MusicXML OAH group" <<
      endl;
  }
#endif

  // protect library against multiple initializations
  if (! gGlobalMusicxmlOahGroup) {
    // create the global MusicXml options group
    gGlobalMusicxmlOahGroup =
      musicxmlOahGroup::create ();
    assert (gGlobalMusicxmlOahGroup != 0);
  }

  // return global OAH group
  return gGlobalMusicxmlOahGroup;
}


}
