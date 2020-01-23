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
R"(These options control the way MusicXML data is translated to MSR.)",
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
        "Trace SHORT_NAME/LONG_NAME when analyzing MusicXML data.",
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

void musicXMLOah::initializeMusicXMLHeaderOptions (
  bool boolOptionsInitialValue)
{
  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Header",
        "hmxmlh", "help-musicxml-header",
R"()",
        kElementVisibilityAlways,
        this);

  appendSubGroupToGroup (subGroup);

  // file name as work title

  fUseFilenameAsWorkTitle = false;

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "ufawt", "use-filename-as-work-title",
R"(Use the file name as work title if there is none in the MusicXML data.
Standard input (-) becomes 'Standard input' in that case.)",
        "useFilenameAsWorkTitle",
        fUseFilenameAsWorkTitle));

  // lyricists as poets

  fUseLyricistsAsPoets = false;

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "ulap", "use-lyricists-as-poets",
R"(Set the 'poet' to the value of 'lyricist' in the LilyPond code \header.
This is useful because LilyPond doesn't feature a 'lyricist' variable.)",
        "useLyricistsAsPoets",
        fUseLyricistsAsPoets));
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

  appendSubGroupToGroup (subGroup);

  // the 'ir' prefix

  S_oahPrefix
    shortIgnoreRedundantPrefix =
      oahPrefix::create (
        "ir",
        "ir",
        "'-ir=abc,yz' is equivalent to '-irabc, -iryz'");
  fHandlerUpLink->
    appendPrefixToHandler (shortIgnoreRedundantPrefix);

  // the 'ignore-redundant' prefix

  S_oahPrefix
    longIgnoreRedundantPrefix =
      oahPrefix::create (
        "ignore-redundant",
        "ignore-redundant-",
        "'-ignore-redundant=abc,yz' is equivalent to '-ignore-redundant-abc, -ignore-redundant-yz'");
  fHandlerUpLink->
    appendPrefixToHandler (longIgnoreRedundantPrefix);

  // the 'ignore redundant' multiplex booleans atom

  S_oahMultiplexBooleansAtom
    ignoreRedundantMultiplexBooleansAtom =
      oahMultiplexBooleansAtom::create (
        "Ignore ELEMENTS that are the same as the current one.",
        "ELEMENTS",
        "ELEMENTS",
        shortIgnoreRedundantPrefix,
        longIgnoreRedundantPrefix);

  subGroup->
    appendAtomToSubGroup (
      ignoreRedundantMultiplexBooleansAtom);

  // redundant clefs

  fIgnoreRedundantClefs = boolOptionsInitialValue;

  fIgnoreRedundantClefsAtom =
    oahBooleanAtom::create (
      "irclefs", "ignore-redundant-clefs",
R"(Ignore clefs that are the same as the current one.)",
      "ignoreRedundantClefs",
      fIgnoreRedundantClefs);
  subGroup->
    appendAtomToSubGroup (
      fIgnoreRedundantClefsAtom);
  ignoreRedundantMultiplexBooleansAtom->
    addBooleanAtom (
      fIgnoreRedundantClefsAtom);

  // redundant keys

  fIgnoreRedundantKeys  = boolOptionsInitialValue;

  fIgnoreRedundantKeysAtom =
    oahBooleanAtom::create (
      "irkeys", "ignore-redundant-keys",
R"(Ignore keys that are the same as the current one.)",
      "ignoreRedundantKeys",
      fIgnoreRedundantKeys);
  subGroup->
    appendAtomToSubGroup (
      fIgnoreRedundantKeysAtom);
  ignoreRedundantMultiplexBooleansAtom->
    addBooleanAtom (
      fIgnoreRedundantKeysAtom);

  // redundant times

  fIgnoreRedundantTimes = boolOptionsInitialValue;

  fIgnoreRedundantTimesAtom =
    oahBooleanAtom::create (
      "irtimes", "ignore-redundant-times",
R"(Ignore times that are the same as the current one.)",
      "ignoreRedundantTimes",
      fIgnoreRedundantTimes);
  subGroup->
    appendAtomToSubGroup (
      fIgnoreRedundantTimesAtom);
  ignoreRedundantMultiplexBooleansAtom->
    addBooleanAtom (
      fIgnoreRedundantTimesAtom);
}

void musicXMLOah::initializeMusicXMMeasuresOptions (
  bool boolOptionsInitialValue)
{
  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Measures",
        "hmxmlm", "help-musicxml-measures",
R"()",
        kElementVisibilityAlways,
        this);

  appendSubGroupToGroup (subGroup);

  // add empty measures

  subGroup->
    appendAtomToSubGroup (
      oahStringToIntMapAtom::create (
        "aem", "add-empty-measures",
R"###(Add empty mesure according to SPECIFICATION.
SPECIFICATION should be of the form 'MEASURE_NUMBER MEASURES_TO_ADD',
where MEASURE_NUMBER is a string, and MEASURES_TO_ADD is the number
of empty measures to add after measure MEASURE_NUMBER.
MEASURE_NUMBER should be the number of an existing, empty measure,
and MEASURES_TO_ADD should be at least 1, , such as '17 3'.
This comes in handly when MusicXML data obtained by scanning contains
a single empty measure when there were several in the original score.
This option can be used any number of times.)###",
        "SPECIFICATION",
        "addEmptyMeasuresStringToIntMap",
        fAddEmptyMeasuresStringToIntMap));
}

void musicXMLOah::initializeMusicXMLDynamicsAndWedgesOptions (
  bool boolOptionsInitialValue)
{
  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Dynamics and wedges",
        "hmxmldaw", "help-musicxml-dynamics-and-wedges",
R"()",
        kElementVisibilityAlways,
        this);

  appendSubGroupToGroup (subGroup);

  // dynamics

  fAllDynamicsBelow = false;

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "adb", "all-dynamics-below",
R"(Ignore dynamics placement and set it to 'below'.)",
        "allDynamicsBelow",
        fAllDynamicsBelow));

  // wedges

  fAllWedgesBelow = false;

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "awb", "all-wedges-below",
R"(Ignore wedges placement and set it to 'below'.)",
        "allWedgesBelow",
        fAllWedgesBelow));
}

void musicXMLOah::initializeMusicXMLCombinedOptionsOptions (
  bool boolOptionsInitialValue)
{
  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Combined options",
        "hmxmlco", "help-musicxml-combined-options",
R"()",
        kElementVisibilityAlways,
        this);

  appendSubGroupToGroup (subGroup);

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

  subGroup->
    appendAtomToSubGroup (
      cubaseCombinedBooleansAtom);

  // set the '-cubase' option by default
  cubaseCombinedBooleansAtom->
    setCombinedBooleanVariables (fCubase);

  // populate the 'cubase' combined atom
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

  subGroup->
    appendAtomToSubGroup (
      noCubaseBooleanAtom);
 }

void musicXMLOah::initializeMusicXMLLoopOptions (
  bool boolOptionsInitialValue)
{
  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Loop",
        "hmxmllo", "help-musicxml-loop-options",
R"()",
        kElementVisibilityAlways,
        this);

  appendSubGroupToGroup (subGroup);

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
    appendAtomToSubGroup (
      loopOptionsBooleanAtom);
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

  // header
  // --------------------------------------
  initializeMusicXMLHeaderOptions (
    boolOptionsInitialValue);

  // clefs, keys, times
  // --------------------------------------
  initializeMusicXMLClefsKeysTimesOptions (
    boolOptionsInitialValue);

  // measures
  // --------------------------------------
  initializeMusicXMMeasuresOptions (
    boolOptionsInitialValue);

  // dynamics and wedges
  // --------------------------------------
  initializeMusicXMLDynamicsAndWedgesOptions (
    boolOptionsInitialValue);

  // combined options
  // --------------------------------------
  initializeMusicXMLCombinedOptionsOptions (
    boolOptionsInitialValue);

  // loop to do JMI
  // --------------------------------------

  initializeMusicXMLLoopOptions (
    boolOptionsInitialValue);
}

S_musicXMLOah musicXMLOah::createCloneWithDetailedTrace ()
{
  S_musicXMLOah
    clone =
      musicXMLOah::create (0);
      // 0 not to have it inserted twice in the option handler

  // set the options handler upLink
  clone->fHandlerUpLink =
    fHandlerUpLink;


    // header
    // --------------------------------------

  clone->fUseFilenameAsWorkTitle =
    fUseFilenameAsWorkTitle;

  clone->fUseLyricistsAsPoets =
    fUseLyricistsAsPoets;


  // clefs, keys, times
  // --------------------------------------

  clone->fIgnoreRedundantClefs =
    fIgnoreRedundantClefs;
  clone->fIgnoreRedundantKeys =
    fIgnoreRedundantKeys;
  clone->fIgnoreRedundantTimes =
    fIgnoreRedundantTimes;


  // dynamics and wedges
  // --------------------------------------

  clone->fAllDynamicsBelow =
    fAllDynamicsBelow;
  clone->fAllWedgesBelow =
    fAllWedgesBelow;


  // combined options, cubase
  // --------------------------------------

  clone->fCubase =
    fCubase;
  clone->fNoCubase =
    fNoCubase;


  // loop
  // --------------------------------------

  clone->fLoopToMusicXML =
    fLoopToMusicXML;


  // trace
  // --------------------------------------

#ifdef TRACE_OAH
  clone->fTraceMusicXMLTreeVisitors =
    fTraceMusicXMLTreeVisitors;

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
  if (gOahOah->fTraceOahVisitors) {
    gLogOstream <<
      ".\\\" ==> musicXMLOah::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_musicXMLOah>*
    p =
      dynamic_cast<visitor<S_musicXMLOah>*> (v)) {
        S_musicXMLOah elem = this;

#ifdef TRACE_OAH
        if (gOahOah->fTraceOahVisitors) {
          gLogOstream <<
            ".\\\" ==> Launching musicXMLOah::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void musicXMLOah::acceptOut (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    gLogOstream <<
      ".\\\" ==> musicXMLOah::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_musicXMLOah>*
    p =
      dynamic_cast<visitor<S_musicXMLOah>*> (v)) {
        S_musicXMLOah elem = this;

#ifdef TRACE_OAH
        if (gOahOah->fTraceOahVisitors) {
          gLogOstream <<
            ".\\\" ==> Launching musicXMLOah::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void musicXMLOah::browseData (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    gLogOstream <<
      ".\\\" ==> musicXMLOah::browseData ()" <<
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


  // header
  // --------------------------------------

  gLogOstream <<
    "Header:" <<
    endl;

  gIndenter++;

  gLogOstream << left <<
    setw (fieldWidth) << "useFilenameAsWorkTitle" << " : " <<
      booleanAsString (fUseFilenameAsWorkTitle) <<
      endl <<
    setw (fieldWidth) << "useLyricistsAsPoets" << " : " <<
      booleanAsString (fUseLyricistsAsPoets) <<
      endl;

  gIndenter--;

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
    endl;

  gIndenter--;

  // measures
  // --------------------------------------

  gLogOstream <<
    "Measures:" <<
    endl;

  gIndenter++;

//     map<string,int>       fAddEmptyMeasuresStringToIntMap;

/* JMI
  gLogOstream << left <<
    setw (fieldWidth) << "ignoreRedundantClefs" << " : " <<
    booleanAsString (fIgnoreRedundantClefs) <<
    endl <<

    setw (fieldWidth) << "ignoreRedundantKeys" << " : " <<
    booleanAsString (fIgnoreRedundantKeys) <<
    endl <<

    setw (fieldWidth) << "ignoreRedundantTimes" << " : " <<
    booleanAsString (fIgnoreRedundantTimes) <<
    endl;
*/
  gIndenter--;

  // dynamics and wedges
  // --------------------------------------

  gLogOstream <<
    "Dynamics and wedges:" <<
    endl;

  gIndenter++;

  gLogOstream << left <<
    setw (fieldWidth) << "allDynamicsBelow" << " : " <<
    booleanAsString (fAllDynamicsBelow) <<
    endl <<
    setw (fieldWidth) << "allWedgesBelow" << " : " <<
    booleanAsString (fAllWedgesBelow) <<
    endl;

  gIndenter--;

  // combined options, cubase
  // --------------------------------------

  gLogOstream <<
    "Cubase:" <<
    endl;

  gIndenter++;

  gLogOstream << left <<
    setw (fieldWidth) << "cubase" << " : " <<
    booleanAsString (fCubase) <<
    endl <<
    setw (fieldWidth) << "noCubase" << " : " <<
    booleanAsString (fNoCubase) <<
    endl;

  gIndenter--;

  // loop
  // --------------------------------------

  gLogOstream <<
    "Loop:" <<
    endl;

  gIndenter++;

  gLogOstream << left <<
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
  if (gTraceOah->fTraceOah && ! gGeneralOah->fQuiet) {
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
