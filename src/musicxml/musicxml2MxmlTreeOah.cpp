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
#include "musicxml2MxmlTreeOah.h"


using namespace std;

namespace MusicXML2
{

//_______________________________________________________________________________

S_musicxml2MxmlTreeOah globalMusicxml2MxmlTreeOah;
S_musicxml2MxmlTreeOah globalMusicxml2MxmlTreeOahUserChoices;
S_musicxml2MxmlTreeOah globalMusicxml2MxmlTreeOahWithDetailedTrace;

S_musicxml2MxmlTreeOah musicxml2MxmlTreeOah::create (
  S_oahHandler handlerUpLink)
{
  musicxml2MxmlTreeOah* o = new musicxml2MxmlTreeOah(
    handlerUpLink);
  assert(o!=0);
  return o;
}

musicxml2MxmlTreeOah::musicxml2MxmlTreeOah (
  S_oahHandler handlerUpLink)
  : oahGroup (
    "Mxmltree2Msr",
    "hmxmlt2msr", "help-mxmltree-to-msr",
R"(These options control the way xmlelement trees are translated to MSR.)",
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
  initializeMusicxml2MxmlTree (false);
}

musicxml2MxmlTreeOah::~musicxml2MxmlTreeOah ()
{}

#ifdef TRACE_OAH
void musicxml2MxmlTreeOah::initializeMusicxml2MxmlTreeTraceOah (
  bool boolOptionsInitialValue)
{
  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Trace",
        "hmxml2mxmlt", "help-musicxml-to-mxmltree-trace",
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

  // the 'MusicXML' multiplex booleans atom

  S_oahMultiplexBooleansAtom
    musicXMLMultiplexBooleansAtom =
      oahMultiplexBooleansAtom::create (
        "Trace SHORT_NAME/LONG_NAME when converting MusicXML data to an xmlelement tree.",
        "SHORT_NAME",
        "LONG_NAME",
        shortTracePrefix,
        longTracePrefix);

  subGroup->
    appendAtomToSubGroup (
      musicXMLMultiplexBooleansAtom);
}
#endif

void musicxml2MxmlTreeOah::initializeMusicxml2MxmlTree (
  bool boolOptionsInitialValue)
{
#ifdef TRACE_OAH
  // trace
  // --------------------------------------
  initializeMusicxml2MxmlTreeTraceOah (
    boolOptionsInitialValue);
#endif
}

S_musicxml2MxmlTreeOah musicxml2MxmlTreeOah::createCloneWithDetailedTrace ()
{
  S_musicxml2MxmlTreeOah
    clone =
      musicxml2MxmlTreeOah::create (0);
      // 0 not to have it inserted twice in the option handler

  // set the options handler upLink
  clone->fHandlerUpLink =
    fHandlerUpLink;

#ifdef TRACE_OAH
  // trace
  // --------------------------------------
#endif

  return clone;
}

//______________________________________________________________________________
void musicxml2MxmlTreeOah::setAllMusicXMLTraceOah (
  bool boolOptionsInitialValue)
{
#ifdef TRACE_OAH
  // specific trace
  // JMI
#endif
}

//______________________________________________________________________________
void musicxml2MxmlTreeOah::enforceQuietness ()
{
#ifdef TRACE_OAH
#endif
}

//______________________________________________________________________________
void musicxml2MxmlTreeOah::checkOptionsConsistency ()
{
  // JMI
}

//______________________________________________________________________________
void musicxml2MxmlTreeOah::acceptIn (basevisitor* v)
{
#ifdef TRACE_OAH
  if (globalOahOah->fTraceOahVisitors) {
    gLogOstream <<
      ".\\\" ==> musicxml2MxmlTreeOah::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_musicxml2MxmlTreeOah>*
    p =
      dynamic_cast<visitor<S_musicxml2MxmlTreeOah>*> (v)) {
        S_musicxml2MxmlTreeOah elem = this;

#ifdef TRACE_OAH
        if (globalOahOah->fTraceOahVisitors) {
          gLogOstream <<
            ".\\\" ==> Launching musicxml2MxmlTreeOah::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void musicxml2MxmlTreeOah::acceptOut (basevisitor* v)
{
#ifdef TRACE_OAH
  if (globalOahOah->fTraceOahVisitors) {
    gLogOstream <<
      ".\\\" ==> musicxml2MxmlTreeOah::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_musicxml2MxmlTreeOah>*
    p =
      dynamic_cast<visitor<S_musicxml2MxmlTreeOah>*> (v)) {
        S_musicxml2MxmlTreeOah elem = this;

#ifdef TRACE_OAH
        if (globalOahOah->fTraceOahVisitors) {
          gLogOstream <<
            ".\\\" ==> Launching musicxml2MxmlTreeOah::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void musicxml2MxmlTreeOah::browseData (basevisitor* v)
{
#ifdef TRACE_OAH
  if (globalOahOah->fTraceOahVisitors) {
    gLogOstream <<
      ".\\\" ==> musicxml2MxmlTreeOah::browseData ()" <<
      endl;
  }
#endif
}

//______________________________________________________________________________
void musicxml2MxmlTreeOah::printMusicxml2MxmlTreeValues (int fieldWidth)
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

  gIndenter--;
#endif

  gIndenter--;
}

ostream& operator<< (ostream& os, const S_musicxml2MxmlTreeOah& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
void initializeMusicxml2MxmlTreeOahHandling (
  S_oahHandler handler)
{
#ifdef TRACE_OAH
  if (globalTraceOah->fTraceOah && ! globalGeneralOah->fQuiet) {
    gLogOstream <<
      "Initializing Musicxml2Mxml options handling" <<
      endl;
  }
#endif

  // protect library against multiple initializations
  static bool pThisMethodHasBeenRun = false;

  if (! pThisMethodHasBeenRun) {
    // create the Musicxml2MxmlTree options
    // ------------------------------------------------------

    globalMusicxml2MxmlTreeOahUserChoices = musicxml2MxmlTreeOah::create (
      handler);
    assert(globalMusicxml2MxmlTreeOahUserChoices != 0);

    globalMusicxml2MxmlTreeOah =
      globalMusicxml2MxmlTreeOahUserChoices;

    // prepare for measure detailed trace
    // ------------------------------------------------------

  /* JMI
    globalMusicxml2MxmlTreeOahWithDetailedTrace =
      globalMusicxml2MxmlTreeOah->
        createCloneWithDetailedTrace ();
        */

    pThisMethodHasBeenRun = true;
  }
}


}
