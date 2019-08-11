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
#include "musicXMLOah.h"


using namespace std;

namespace MusicXML2
{

//_______________________________________________________________________________

S_musicXMLOah gMusicXMLOah;
S_musicXMLOah gMusicXMLOahUserChoices;
S_musicXMLOah gMusicXMLOahWithDetailedTrace;

S_musicXMLOah musicXMLOah::create (
  S_oahHandler handlerUpLink)
{
  musicXMLOah* o = new musicXMLOah(
    handlerUpLink);
  assert(o!=0);
  return o;
}

musicXMLOah::musicXMLOah (
  S_oahHandler handlerUpLink)
  : oahGroup (
    "MusicXML",
    "hmxml", "help-musicxml",
R"(These options control the way MusicXML data is translated.)",
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
  initializeMusicXMLOah (false);
}

musicXMLOah::~musicXMLOah ()
{}

#ifdef TRACE_OAH
void musicXMLOah::initializeMusicXMLTraceOah (
  bool boolOptionsInitialValue)
{
  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Trace",
        "hmxmlt", "help-musicxml-trace",
R"()",
        kElementVisibilityAlways,
        this);

  appendSubGroup (subGroup);

  // encoding

  fTraceEncoding = boolOptionsInitialValue;

  subGroup->
    appendAtom (
      oahTwoBooleansAtom::create (
        "tenc", "trace-encoding",
R"(Encoding)",
        "traceEncoding",
        fTraceEncoding,
        gTraceOah->fTracePasses));

  // divisions

  fTraceDivisions = boolOptionsInitialValue;

  subGroup->
    appendAtom (
      oahTwoBooleansAtom::create (
        "tdivs", "trace-divisions",
R"(Divisions)",
        "traceDivisions",
        fTraceDivisions,
        gTraceOah->fTracePasses));

  // backup

  fTraceBackup = boolOptionsInitialValue;

  subGroup->
    appendAtom (
      oahTwoBooleansAtom::create (
        "tbackup", "trace-backup",
R"(Backup)",
        "traceBackup",
        fTraceBackup,
        gTraceOah->fTracePasses));

  // forward

  fTraceForward = boolOptionsInitialValue;

  subGroup->
    appendAtom (
      oahTwoBooleansAtom::create (
        "tforward", "trace-forward",
R"(Forward)",
        "traceForward",
        fTraceForward,
        gTraceOah->fTracePasses));

  // MusicXML tree visiting

  fTraceMusicXMLTreeVisitors = boolOptionsInitialValue;

  subGroup->
    appendAtom (
      oahBooleanAtom::create (
        "tmxmltv", "trace-musicxml-tree-visitors",
R"(Write a trace of the MusicXML tree visiting activity to standard error.)",
        "traceMusicXMLTreeVisitors",
        fTraceMusicXMLTreeVisitors));
}
#endif

void musicXMLOah::initializeMusicXMLWorkOptions (
  bool boolOptionsInitialValue)
{
  S_oahSubGroup
    workSubGroup =
      oahSubGroup::create (
        "Work",
        "hmxmlw", "help-musicxml-work",
R"()",
        kElementVisibilityAlways,
        this);

  appendSubGroup (workSubGroup);

  // file name as work title

  fUseFilenameAsWorkTitle = false;

  workSubGroup->
    appendAtom (
      oahBooleanAtom::create (
        "ufawt", "use-filename-as-work-title",
R"(Use the file name as work title if there is none in the MusicXML data.
Standard input (-) becomes 'Standard input' in that case.)",
        "useFilenameAsWorkTitle",
        fUseFilenameAsWorkTitle));
}

void musicXMLOah::initializeMusicXMLClefsKeysTimesOptions (
  bool boolOptionsInitialValue)
{
  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Clefs, keys, times",
        "hmxmlckt", "help-musicxml-clefs-keys-times",
R"()",
        kElementVisibilityAlways,
        this);

  appendSubGroup (subGroup);

  // the 'ignore redundant' prefix

  S_oahPrefix
    ignoreRedundantPrefix =
      oahPrefix::create (
        "ir",
        "ignore-redundant-",
        "'-ir=abc,xywx,yz' is equivalent to '-irabc, -irxywx, -iryz'");
  fHandlerUpLink->
    appendPrefixToHandler (ignoreRedundantPrefix);

  // the 'ignore redundant' prefixed booleans atom

  S_oahPrefixedBooleansAtom
    prefixedBooleansAtom =
      oahPrefixedBooleansAtom::create (
        "ir[ELEMENTS]",
        "ignore-redundant-[ELEMENTS]",
        "ignore ELEMENTS that are the same as the current one.",
        ignoreRedundantPrefix);

  subGroup->
    appendAtom (
      prefixedBooleansAtom);

  // redundant clefs

  fIgnoreRedundantClefs = boolOptionsInitialValue;

  fIgnoreRedundantClefsAtom =
    oahBooleanAtom::create (
      "irclef", "ignore-redundant-clefs",
R"(Ignore clefs that are the same as the current one.)",
      "ignoreRedundantClefs",
      fIgnoreRedundantClefs);
  subGroup->
    appendAtom (
      fIgnoreRedundantClefsAtom);

  // redundant keys

  fIgnoreRedundantKeys  = boolOptionsInitialValue;

  fIgnoreRedundantKeysAtom =
    oahBooleanAtom::create (
      "irkey", "ignore-redundant-keys",
R"(Ignore keys that are the same as the current one.)",
      "ignoreRedundantKeys",
      fIgnoreRedundantKeys);
  subGroup->
    appendAtom (
      fIgnoreRedundantKeysAtom);

  // redundant times

  fIgnoreRedundantTimes = boolOptionsInitialValue;

  fIgnoreRedundantTimesAtom =
    oahBooleanAtom::create (
      "irtime", "ignore-redundant-times",
R"(Ignore times that are the same as the current one.)",
      "ignoreRedundantTimes",
      fIgnoreRedundantTimes);
  subGroup->
    appendAtom (
      fIgnoreRedundantTimesAtom);

  // populate prefixedBooleansAtom

  prefixedBooleansAtom->
    addBooleanAtom (
      fIgnoreRedundantClefsAtom);
  prefixedBooleansAtom->
    addBooleanAtom (
      fIgnoreRedundantKeysAtom);
  prefixedBooleansAtom->
    addBooleanAtom (
      fIgnoreRedundantTimesAtom);

  // '-loop' is hidden...

  fLoopToMusicXML = boolOptionsInitialValue;

  S_oahBooleanAtom
    loopOptionsBooleanAtom =
      oahBooleanAtom::create (
        "loop", "loop-to-musicxml",
R"(Close the loop, generating a MusicXML file from the MSR.
The file name receives a '_loop' suffix. Currently under development.)",
        "loopToMusicXML",
        fLoopToMusicXML);
  loopOptionsBooleanAtom->
    setIsHidden ();

  subGroup->
    appendAtom (
      loopOptionsBooleanAtom);
}

void musicXMLOah::initializeMusicXMLCombinedOptionsOptions (
  bool boolOptionsInitialValue)
{
  S_oahSubGroup
    combinedSubGroup =
      oahSubGroup::create (
        "Combined options",
        "hmxmlco", "help-musicxml-combined-options",
R"()",
        kElementVisibilityAlways,
        this);

  appendSubGroup (combinedSubGroup);

  // cubase

  fCubase = false;

  // create the 'cubase' combined atom
  S_oahCombinedBooleansAtom
    cubaseCombinedBooleansAtom =
      oahCombinedBooleansAtom::create (
        "cubase", "",
R"(Useful settings for MusicXML data exported from Cubase.
This option is set by default, and can be unset by 'noCubase'.)",
        "cubase",
        fCubase);

  combinedSubGroup->
    appendAtom (
      cubaseCombinedBooleansAtom);

  // set the '-cubase' option by default
  cubaseCombinedBooleansAtom->
    setCombinedBooleanVariables (fCubase);

  // populate the combined atoms
  cubaseCombinedBooleansAtom->
    addBooleanAtom (
      fIgnoreRedundantClefsAtom);
  cubaseCombinedBooleansAtom->
    addBooleanAtom (
      fIgnoreRedundantKeysAtom);
  cubaseCombinedBooleansAtom->
    addBooleanAtom (
      fIgnoreRedundantTimesAtom);

  // noCubase

  fNoCubase = false;

  S_oahBooleanAtom
    noCubaseBooleanAtom =
      oahBooleanAtom::create (
        "no-cubase", "",
R"(Prevents the default 'cubase' option.)",
        "noCubase",
        fNoCubase);

  combinedSubGroup->
    appendAtom (
      noCubaseBooleanAtom);
 }

void musicXMLOah::initializeMusicXMLOah (
  bool boolOptionsInitialValue)
{
#ifdef TRACE_OAH
  // trace
  // --------------------------------------
  initializeMusicXMLTraceOah (
    boolOptionsInitialValue);
#endif

  // worktitle
  initializeMusicXMLWorkOptions (
    boolOptionsInitialValue);

  // clefs, keys, times
  // --------------------------------------
  initializeMusicXMLClefsKeysTimesOptions (
    boolOptionsInitialValue);

  // combined options
  // --------------------------------------
  initializeMusicXMLCombinedOptionsOptions (
    boolOptionsInitialValue);
}

S_musicXMLOah musicXMLOah::createCloneWithDetailedTrace ()
{
/* JMI
  S_musicXMLOah
    clone =
      musicXMLOah::create (0);
      // 0 not to have it inserted twice in the option handler

  // set the options handler upLink
  clone->fHandlerUpLink =
    fHandlerUpLink;


  // clefs, keys, times
  // --------------------------------------

  clone->fIgnoreRedundantClefs =
    fIgnoreRedundantClefs;

  clone->fIgnoreRedundantKeys =
    fIgnoreRedundantKeys;

  clone->fIgnoreRedundantTimes =
    fIgnoreRedundantTimes;

  clone->fLoopToMusicXML =
    fLoopToMusicXML;


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

  clone->fTraceMusicXMLTreeVisitors =
    fTraceMusicXMLTreeVisitors;

  return clone;
  */
}

//______________________________________________________________________________
void musicXMLOah::setAllMusicXMLTraceOah (
  bool boolOptionsInitialValue)
{
#ifdef TRACE_OAH
  // specific trace

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
void musicXMLOah::enforceQuietness ()
{
  fTraceMusicXMLTreeVisitors = false;
}

//______________________________________________________________________________
void musicXMLOah::checkOptionsConsistency ()
{
  // JMI
}

//______________________________________________________________________________
void musicXMLOah::acceptIn (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gExecutableOah->fTraceOahVisitors) {
    gLogOstream <<
      "% ==> musicXMLOah::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_musicXMLOah>*
    p =
      dynamic_cast<visitor<S_musicXMLOah>*> (v)) {
        S_musicXMLOah elem = this;

#ifdef TRACE_OAH
        if (gExecutableOah->fTraceOahVisitors) {
          gLogOstream <<
            "% ==> Launching musicXMLOah::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void musicXMLOah::acceptOut (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gExecutableOah->fTraceOahVisitors) {
    gLogOstream <<
      "% ==> musicXMLOah::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_musicXMLOah>*
    p =
      dynamic_cast<visitor<S_musicXMLOah>*> (v)) {
        S_musicXMLOah elem = this;

#ifdef TRACE_OAH
        if (gExecutableOah->fTraceOahVisitors) {
          gLogOstream <<
            "% ==> Launching musicXMLOah::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void musicXMLOah::browseData (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gExecutableOah->fTraceOahVisitors) {
    gLogOstream <<
      "% ==> musicXMLOah::browseData ()" <<
      endl;
  }
#endif
}

//______________________________________________________________________________
void musicXMLOah::printMusicXMLOahValues (int fieldWidth)
{
  gLogOstream <<
    "The MusicXML options are:" <<
    endl;

  gIndenter++;

  // clefs, keys, times
  // --------------------------------------

  gLogOstream <<
    "Clefs, keys, times:" <<
    endl;

  gIndenter++;

  gLogOstream << left <<
    setw (fieldWidth) << "ignoreRedundantClefs" << " : " <<
    booleanAsString (fIgnoreRedundantClefs) <<
    endl <<

    setw (fieldWidth) << "ignoreRedundantKeys" << " : " <<
    booleanAsString (fIgnoreRedundantKeys) <<
    endl <<

    setw (fieldWidth) << "ignoreRedundantTimes" << " : " <<
    booleanAsString (fIgnoreRedundantTimes) <<
    endl <<

    setw (fieldWidth) << "loopToMusicXML" << " : " <<
    booleanAsString (fLoopToMusicXML) <<
    endl;

  gIndenter--;

  // trace
  // --------------------------------------

  gLogOstream <<
    "Trace:" <<
    endl;

  gIndenter++;

  gLogOstream << left <<
#ifdef TRACE_OAH
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
#endif

    setw (fieldWidth) << "traceMusicXMLTreeVisitors" << " : " <<
    booleanAsString (fTraceMusicXMLTreeVisitors) <<
    endl;

  gIndenter--;

  gIndenter--;
}

ostream& operator<< (ostream& os, const S_musicXMLOah& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
void initializeMusicXMLOahHandling (
  S_oahHandler handler)
{
#ifdef TRACE_OAH
  if (gExecutableOah->fTraceOah && ! gGeneralOah->fQuiet) {
    gLogOstream <<
      "Initializing MusicXML options handling" <<
      endl;
  }
#endif

  // create the MusicXML options
  // ------------------------------------------------------

  gMusicXMLOahUserChoices = musicXMLOah::create (
    handler);
  assert(gMusicXMLOahUserChoices != 0);

  gMusicXMLOah =
    gMusicXMLOahUserChoices;

  // prepare for measure detailed trace
  // ------------------------------------------------------

/* JMI
  gMusicXMLOahWithDetailedTrace =
    gMusicXMLOah->
      createCloneWithDetailedTrace ();
      */
}


}
