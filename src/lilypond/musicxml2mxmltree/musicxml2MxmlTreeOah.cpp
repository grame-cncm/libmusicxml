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

S_musicxml2MxmlTreeOah gMusicxml2MxmlTreeOah;
S_musicxml2MxmlTreeOah gMusicxml2MxmlTreeOahUserChoices;
S_musicxml2MxmlTreeOah gMusicxml2MxmlTreeOahWithDetailedTrace;

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
R"(These options control the way xmlelement tree is translated to MSR.)",
    kElementVisibilityAlways,
    handlerUpLink)
{
/* JMI
  // sanity check
  msrAssert (
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
void musicxml2MxmlTreeOah::initializeMxmlTreeTraceOah (
  bool boolOptionsInitialValue)
{
  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Trace",
        "hmxmlt2mxmlgree", "help-mxml-to-mxmltree-trace",
R"()",
        kElementVisibilityAlways,
        this);

  appendSubGroupToGroup (subGroup);

  // fetch the 't' prefix

  S_oahPrefix
    shortTracePrefix =
      fHandlerUpLink->
        fetchPrefixInMapByItsName (
          "t");

  msrAssert (
    shortTracePrefix != nullptr,
    "shortTracePrefix is null");

  // fetch the 'trace' prefix

  S_oahPrefix
    longTracePrefix =
      fHandlerUpLink->
        fetchPrefixInMapByItsName (
          "trace");

  msrAssert (
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

  // MusicXML tree visiting

  fTraceMusicXMLTreeVisitors = boolOptionsInitialValue;

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "tmxmltv", "trace-musicxml-tree-visitors",
R"(Write a trace of the MusicXML tree visiting activity to standard error.)",
        "traceMusicXMLTreeVisitors",
        fTraceMusicXMLTreeVisitors));
}
#endif

void musicxml2MxmlTreeOah::initializeMusicxml2MxmlTree (
  bool boolOptionsInitialValue)
{
#ifdef TRACE_OAH
  // trace
  // --------------------------------------
  initializeMxmlTreeTraceOah (
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

  // trace
  // --------------------------------------

#ifdef TRACE_OAH
  clone->fTraceMusicXMLTreeVisitors =
    fTraceMusicXMLTreeVisitors;
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
  fTraceMusicXMLTreeVisitors = false;
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
  if (gOahOah->fTraceOahVisitors) {
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
        if (gOahOah->fTraceOahVisitors) {
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
  if (gOahOah->fTraceOahVisitors) {
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
        if (gOahOah->fTraceOahVisitors) {
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
  if (gOahOah->fTraceOahVisitors) {
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

  gLogOstream << left <<
    setw (fieldWidth) << "traceMusicXMLTreeVisitors" << " : " <<
    booleanAsString (fTraceMusicXMLTreeVisitors) <<
    endl;

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
  if (gTraceOah->fTraceOah && ! gGeneralOah->fQuiet) {
    gLogOstream <<
      "Initializing Musicxml2Mxml options handling" <<
      endl;
  }
#endif

  // create the Musicxml2MxmlTree options
  // ------------------------------------------------------

  gMusicxml2MxmlTreeOahUserChoices = musicxml2MxmlTreeOah::create (
    handler);
  assert(gMusicxml2MxmlTreeOahUserChoices != 0);

  gMusicxml2MxmlTreeOah =
    gMusicxml2MxmlTreeOahUserChoices;

  // prepare for measure detailed trace
  // ------------------------------------------------------

/* JMI
  gMusicxml2MxmlTreeOahWithDetailedTrace =
    gMusicxml2MxmlTreeOah->
      createCloneWithDetailedTrace ();
      */
}


}
