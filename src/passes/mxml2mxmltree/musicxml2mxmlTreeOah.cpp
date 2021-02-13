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

#include "musicxml2mxmlTreeOah.h"

#include "oahAtomsCollection.h"


using namespace std;

namespace MusicXML2
{

//_______________________________________________________________________________

S_musicxml2mxmlTreeOahGroup gGlobalMusicxml2mxmlTreeOahGroup;

S_musicxml2mxmlTreeOahGroup musicxml2mxmlTreeOahGroup::create ()
{
  musicxml2mxmlTreeOahGroup* o = new musicxml2mxmlTreeOahGroup ();
  assert (o != nullptr);
  return o;
}

musicxml2mxmlTreeOahGroup::musicxml2mxmlTreeOahGroup ()
  : oahGroup (
    "Mxmltree2msr",
    "hmxmlt2msr", "help-mxmlTree-to-msr",
R"(These options control the way xmlelement trees are translated to MSR.)",
    kElementVisibilityWhole)
{
  initializeMusicxml2mxmlTree ();
}

musicxml2mxmlTreeOahGroup::~musicxml2mxmlTreeOahGroup ()
{}

#ifdef TRACING_IS_ENABLED
void musicxml2mxmlTreeOahGroup::initializeMusicxml2mxmlTreeTraceOah ()
{
  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "musicxml2mxmlTree Trace",
        "hmxml2mxmlt", "help-musicxml-to-mxmlTree-trace",
R"()",
        kElementVisibilityWhole,
        this);

  appendSubGroupToGroup (subGroup);

  // the 'MusicXML' multiplex booleans atom

  S_oahMultiplexBooleansAtom
    musicXMLMultiplexBooleansAtom =
      oahMultiplexBooleansAtom::create (
        "tmd", "trace-musicxml-data",
        "Trace SHORT_NAME/LONG_NAME when converting MusicXML data to an xmlelement tree.",
        "SHORT_NAME",
        "LONG_NAME",
        gGlobalTraceOahGroup->getShortTracePrefix (),
        gGlobalTraceOahGroup->getLongTracePrefix ());

  subGroup->
    appendAtomToSubGroup (
      musicXMLMultiplexBooleansAtom);
}
#endif

void musicxml2mxmlTreeOahGroup::initializeMusicxml2mxmlTree ()
{
#ifdef TRACING_IS_ENABLED
  // trace
  // --------------------------------------
  initializeMusicxml2mxmlTreeTraceOah ();
#endif
}

//______________________________________________________________________________
void musicxml2mxmlTreeOahGroup::enforceGroupQuietness ()
{
#ifdef TRACING_IS_ENABLED
// JMI
#endif
}

//______________________________________________________________________________
void musicxml2mxmlTreeOahGroup::checkGroupOptionsConsistency ()
{
  // JMI
}

//______________________________________________________________________________
void musicxml2mxmlTreeOahGroup::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> musicxml2mxmlTreeOahGroup::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_musicxml2mxmlTreeOahGroup>*
    p =
      dynamic_cast<visitor<S_musicxml2mxmlTreeOahGroup>*> (v)) {
        S_musicxml2mxmlTreeOahGroup elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching musicxml2mxmlTreeOahGroup::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void musicxml2mxmlTreeOahGroup::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> musicxml2mxmlTreeOahGroup::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_musicxml2mxmlTreeOahGroup>*
    p =
      dynamic_cast<visitor<S_musicxml2mxmlTreeOahGroup>*> (v)) {
        S_musicxml2mxmlTreeOahGroup elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching musicxml2mxmlTreeOahGroup::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void musicxml2mxmlTreeOahGroup::browseData (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> musicxml2mxmlTreeOahGroup::browseData ()" <<
      endl;
  }
#endif
}

//______________________________________________________________________________
void musicxml2mxmlTreeOahGroup::printMusicxml2mxmlTreeValues (unsigned int fieldWidth)
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

  --gIndenter;
#endif

  --gIndenter;
}

ostream& operator<< (ostream& os, const S_musicxml2mxmlTreeOahGroup& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_musicxml2mxmlTreeOahGroup createGlobalMusicxml2mxmlTreeOahGroup ()
#ifdef TRACING_IS_ENABLED
#ifdef ENFORCE_TRACE_OAH
  gLogStream <<
    "Creating global xml2mxmlTree OAH group" <<
    endl;
#endif
#endif
{
  // protect library against multiple initializations
  if (! gGlobalMusicxml2mxmlTreeOahGroup) {
    // create the global OAH group
    gGlobalMusicxml2mxmlTreeOahGroup =
      musicxml2mxmlTreeOahGroup::create ();
    assert (gGlobalMusicxml2mxmlTreeOahGroup != 0);
  }

  // return global OAH group
  return gGlobalMusicxml2mxmlTreeOahGroup;
}


}
