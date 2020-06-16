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
#include "musicxmlOah.h"


using namespace std;

namespace MusicXML2
{

//_______________________________________________________________________________

S_musicxmlOah gMusicxmlOah;
S_musicxmlOah gMusicxmlOahUserChoices;
S_musicxmlOah gMusicxmlOahWithDetailedTrace;

S_musicxmlOah musicxmlOah::create (
  S_oahHandler handlerUpLink)
{
  musicxmlOah* o = new musicxmlOah(
    handlerUpLink);
  assert(o!=0);
  return o;
}

musicxmlOah::musicxmlOah (
  S_oahHandler handlerUpLink)
  : oahGroup (
    "MusicXML",
    "hmxml", "help-mxml",
R"(These options control the way MusicXML data is handled.)",
    kElementVisibilityWhole,
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
  initializeMusicxmlOah (false);
}

musicxmlOah::~musicxmlOah ()
{}

#ifdef TRACE_OAH
void musicxmlOah::initializeMusicxmlTraceOah (
  bool boolOptionsInitialValue)
{
  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Trace",
        "hmxmltr", "help-musicxml-trace",
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

  msrAssert (
    shortTracePrefix != nullptr,
    "shortTracePrefix is null");

  // fetch the 'trace' prefix

  S_oahPrefix
    longTracePrefix =
      fHandlerUpLink->
        fetchPrefixNameInPrefixesMap (
          "trace");

  msrAssert (
    longTracePrefix != nullptr,
    "longTracePrefix is null");

  // the 'MusicXML' multiplex booleans atom

  S_oahMultiplexBooleansAtom
    musicXMLMultiplexBooleansAtom =
      oahMultiplexBooleansAtom::create (
        "Trace SHORT_NAME/LONG_NAME when handling MusicXML data.",
        "SHORT_NAME",
        "LONG_NAME",
        shortTracePrefix,
        longTracePrefix);

  subGroup->
    appendAtomToSubGroup (
      musicXMLMultiplexBooleansAtom);

  // encoding

  fTraceEncoding = boolOptionsInitialValue;

  S_oahBooleanAtom
    traceEncodingAtom =
      oahTwoBooleansAtom::create (
        "tenc", "trace-encoding",
R"(Encoding)",
        "traceEncoding",
        fTraceEncoding,
        gTraceOah->fTracePasses);
  subGroup->
    appendAtomToSubGroup (
      traceEncodingAtom);
  musicXMLMultiplexBooleansAtom->
    addBooleanAtom (
      traceEncodingAtom);

  // divisions

  fTraceDivisions = boolOptionsInitialValue;

  S_oahBooleanAtom
    traceDivisionsAtom =
      oahTwoBooleansAtom::create (
        "tdivs", "trace-divisions",
R"(Divisions)",
        "traceDivisions",
        fTraceDivisions,
        gTraceOah->fTracePasses);
  subGroup->
    appendAtomToSubGroup (
      traceDivisionsAtom);
  musicXMLMultiplexBooleansAtom->
    addBooleanAtom (
      traceDivisionsAtom);

  // backup

  fTraceBackup = boolOptionsInitialValue;

  S_oahBooleanAtom
    traceBackupAtom =
      oahTwoBooleansAtom::create (
        "tbackup", "trace-backup",
R"(Backup)",
        "traceBackup",
        fTraceBackup,
        gTraceOah->fTracePasses);
  subGroup->
    appendAtomToSubGroup (
      traceBackupAtom);
  musicXMLMultiplexBooleansAtom->
    addBooleanAtom (
      traceBackupAtom);

  // forward

  fTraceForward = boolOptionsInitialValue;

  S_oahBooleanAtom
    traceForwardAtom =
      oahTwoBooleansAtom::create (
        "tforward", "trace-forward",
R"(Forward)",
        "traceForward",
        fTraceForward,
        gTraceOah->fTracePasses);
  subGroup->
    appendAtomToSubGroup (
      traceForwardAtom);
  musicXMLMultiplexBooleansAtom->
    addBooleanAtom (
      traceForwardAtom);
}
#endif

void musicxmlOah::initializeMusicxmlOah (
  bool boolOptionsInitialValue)
{
#ifdef TRACE_OAH
  // trace
  // --------------------------------------
  initializeMusicxmlTraceOah (
    boolOptionsInitialValue);
#endif
}

S_musicxmlOah musicxmlOah::createCloneWithDetailedTrace ()
{
  S_musicxmlOah
    clone =
      musicxmlOah::create (0);
      // 0 not to have it inserted twice in the option handler

  // set the options handler upLink
  clone->fHandlerUpLink =
    fHandlerUpLink;

  // trace
  // --------------------------------------

#ifdef TRACE_OAH
  clone->fTraceEncoding =
    fTraceEncoding;

  clone->fTraceDivisions =
    fTraceDivisions;

  clone->fTraceBackup =
    fTraceBackup;

  clone->fTraceForward =
    fTraceForward;
#endif
  return clone;
}

//______________________________________________________________________________
void musicxmlOah::setAllMusicXMLTraceOah (
  bool boolOptionsInitialValue)
{
#ifdef TRACE_OAH
  // specific trace
  // JMI

  // encoding
  fTraceEncoding = boolOptionsInitialValue;

  // divisions
  fTraceDivisions = boolOptionsInitialValue;

  // backup
  fTraceBackup = boolOptionsInitialValue;

  // forward
  fTraceForward = boolOptionsInitialValue;
#endif
}

//______________________________________________________________________________
void musicxmlOah::enforceQuietness ()
{}

//______________________________________________________________________________
void musicxmlOah::checkOptionsConsistency ()
{}

//______________________________________________________________________________
void musicxmlOah::acceptIn (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    gLogOstream <<
      ".\\\" ==> musicxmlOah::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_musicxmlOah>*
    p =
      dynamic_cast<visitor<S_musicxmlOah>*> (v)) {
        S_musicxmlOah elem = this;

#ifdef TRACE_OAH
        if (gOahOah->fTraceOahVisitors) {
          gLogOstream <<
            ".\\\" ==> Launching musicxmlOah::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void musicxmlOah::acceptOut (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    gLogOstream <<
      ".\\\" ==> musicxmlOah::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_musicxmlOah>*
    p =
      dynamic_cast<visitor<S_musicxmlOah>*> (v)) {
        S_musicxmlOah elem = this;

#ifdef TRACE_OAH
        if (gOahOah->fTraceOahVisitors) {
          gLogOstream <<
            ".\\\" ==> Launching musicxmlOah::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void musicxmlOah::browseData (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    gLogOstream <<
      ".\\\" ==> musicxmlOah::browseData ()" <<
      endl;
  }
#endif
}

//______________________________________________________________________________
void musicxmlOah::printMusicxmlValues (int fieldWidth)
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

  gIndenter--;
#endif

  gIndenter--;
}

ostream& operator<< (ostream& os, const S_musicxmlOah& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
void initializeMusicxmlOahHandling (
  S_oahHandler handler)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceOah && ! gGeneralOah->fQuiet) {
    gLogOstream <<
      "Initializing Musicxml2Mxml options handling" <<
      endl;
  }
#endif

  // create the Musicxml options
  // ------------------------------------------------------

  gMusicxmlOahUserChoices = musicxmlOah::create (
    handler);
  assert(gMusicxmlOahUserChoices != 0);

  gMusicxmlOah =
    gMusicxmlOahUserChoices;

  // prepare for measure detailed trace
  // ------------------------------------------------------

/* JMI
  gMusicxmlOahWithDetailedTrace =
    gMusicxmlOah->
      createCloneWithDetailedTrace ();
      */
}


}
