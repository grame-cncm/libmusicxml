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

#include "setTraceOahIfDesired.h"
#ifdef TRACE_OAH
  #include "traceOah.h"
#endif
#include "oahOah.h"
#include "generalOah.h"

#include "version.h"
#include "utilities.h"
#include "messagesHandling.h"

#include "xml2lyTwoViewOahHandler.h"


using namespace std;

namespace MusicXML2
{

//#define OAH_TESTS

//______________________________________________________________________________
S_xml2lyTwoViewOahHandler xml2lyTwoViewOahHandler::create (
  string   executableName,
  ostream& os)
{
  xml2lyTwoViewOahHandler* o = new
    xml2lyTwoViewOahHandler (
      executableName,
      os);
  assert(o!=0);

  o->createTheTwoHandlers (os);

  return o;
}

xml2lyTwoViewOahHandler::xml2lyTwoViewOahHandler (
  string   executableName,
  ostream& os)
  : oahTwoViewHandler (
      "xml2lyTwoViewOahHandler",
      executableName,
      "full view",
      "",
      os)
{}

xml2lyTwoViewOahHandler::~xml2lyTwoViewOahHandler ()
{}

void xml2lyTwoViewOahHandler::createTheTwoHandlers (
  ostream& os)
{
#ifdef TRACE_OAH
#ifdef OAH_TESTS
  gLogOstream <<
    "Creating the two OAH handlers for \"" << fTwoViewHandlerName << "\"" <<
    endl;
#endif
#endif

  gIndenter++;

  // create the 'full view' handler
#ifdef TRACE_OAH
#ifdef OAH_TESTS
  gLogOstream <<
    "Creating the full view handler for \"" << fTwoViewHandlerName << "\"" <<
    endl;
#endif
#endif

/* JMI
S_oahHandler oahHandler::create (
  string   executableName,
  string   handlerHeader,
  string   handlerValuesHeader,
  string   handlerShortName,
  string   handlerLongName,
  string   handlerSummaryShortName,
  string   handlerSummaryLongName,
  string   handlerPreamble,
  string   handlerUsage,
  string   handlerDescription,
  ostream& handlerLogOstream)
*/

  string   handlerValuesHeader;
  string   handlerShortName;
  string   handlerLongName;
  string   handlerSummaryShortName;
  string   handlerSummaryLongName;
  string   handlerPreamble;
  string   handlerUsage;
  string   handlerDescription;

  fFullViewOahHandler =
    oahHandler::create (
      fExecutableName,
      fExecutableName + " full view help",
      handlerValuesHeader,
      handlerShortName,
      handlerLongName,
      handlerSummaryShortName,
      handlerSummaryLongName,
      handlerPreamble,
      handlerUsage,
      handlerDescription,
      os);

  // create the 'alternate' handler
#ifdef TRACE_OAH
#ifdef OAH_TESTS
  gLogOstream <<
    "Creating the alternate view handler for \"" << fTwoViewHandlerName << "\"" <<
    endl;
#endif
#endif

  fAlternateViewOahHandler =
    oahHandler::create (
      fExecutableName,
      fExecutableName + " alternate view help",
      handlerValuesHeader,
      handlerShortName,
      handlerLongName,
      handlerSummaryShortName,
      handlerSummaryLongName,
      handlerPreamble,
      handlerUsage,
      handlerDescription,
      os);

  // create the alternate handler groups
  createAlternateHandlerGroups (os);

  // populate the alternate handler from the full view handler
  populateAlternateViewHandlerFromFullViewHandler ();

  // register alternate handler in itself,
  // so that the 'global' help options can be handled
  fAlternateViewOahHandler->
    registerHandlerInItself ();

  // the default is to use the 'alternate' oahHandler
  fOahHandlerToBeUsed = fAlternateViewOahHandler;

#ifdef TRACE_OAH
#ifdef OAH_TESTS
  gLogOstream <<
    "Setting two view handler \"" <<
    fTwoViewHandlerName <<
    "\" handler to \"" <<
    fOahHandlerToBeUsed->getHandlerHeader () <<
    "\"" <<
    endl;
#endif
#endif

  gIndenter--;
}

void xml2lyTwoViewOahHandler::createAlternateHandlerGroups (
  ostream& os)
{
#ifdef TRACE_OAH
#ifdef OAH_TESTS
  gLogOstream <<
    "Creating the alternate handler groups for \"" << fTwoViewHandlerName << "\"" <<
    endl;
#endif
#endif

  createOahAlternateGroup (os);

  createInformationsAlternateGroup (os);

  createWarningAndErrorsAlternateGroup (os);

  createFilesAlternateGroup (os);

  createInputAlternateGroup (os);

  createPartsAlternateGroup (os);
  createStavesAlternateGroup (os);
  createVoicesAlternateGroup (os);

  createTranspositionsAlternateGroup (os);

  createClefsAlternateGroup (os);
  createKeysAlternateGroup (os);
  createTimesAlternateGroup (os);

  createRepeatsAlternateGroup (os);

  createMeasuresAlternateGroup (os);

  createRestsAlternateGroup (os);
  createNotesAlternateGroup (os);

  creatBeamsAlternateGroup (os);

  createArticulationsAlternateGroup (os);

  createOrnamentsAlternateGroup (os);

  createGraceNotesAlternateGroup (os);

  createChordsAlternateGroup (os);

  createTiesAlternateGroup (os);
  createSlursAlternateGroup (os);
  createLigaturesAlternateGroup (os);

  createDynamicsAlternateGroup (os);
  createWedgesAlternateGroup (os);

  createTupletsAlternateGroup (os);

  createLyricsAlternateGroup (os);

  createStringsAlternateGroup (os);

  createTablaturesAlternateGroup (os);

  createHarmoniesAlternateGroup (os);

  createFiguredBassesAlternateGroup (os);

  createHeaderAlternateGroup (os);
  createPaperAlternateGroup (os);
  createLayoutAlternateGroup (os);

  createOutputAlternateGroup (os);

  createMidiAlternateGroup (os);
}

void xml2lyTwoViewOahHandler::createOahAlternateGroup (
  ostream& os)
{
  // group

  S_oahGroup
    group =
      oahGroup::create (
        "Options and help group",
        "hoah-grp",
        "help-oah-grp",
        "",
        kElementVisibilityWhole,
        fAlternateViewOahHandler);
  group->
    setGroupHeaderIsToBeWritten (false);
  fAlternateViewOahHandler->
    appendGroupToHandler (group);

  // subgroup

  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Options and help",
        "hoah",
        "help-oah",
        "",
        kElementVisibilityWhole,
        group);
  group->
    appendSubGroupToGroup (subGroup);

/* JMI ???
  // the 'full view' atom

  S_oahTwoViewHandlerFullViewAtom
    dualHandlerFullViewAtom =
      oahTwoViewHandlerFullViewAtom::create (
          fTwoViewAtomShortName, fFullViewAtomLongName,
R"(In the default 'alternate' view, the options are grouped by music scoring topics,
such a slurs, tuplets and figured bass.
This option switches the options and help view to 'full view',
in which the options are grouped as they are used by the various
internal representations and translation passes.
This unleashes the full set of display and trace options.
This option should be the first one.)");

  subGroup->
    appendAtomToSubGroup (dualHandlerFullViewAtom);
*/

    /*
  dualHandlerFullViewAtom ->
    registerAtomInHandler (fAlternateViewOahHandler);

 fAlternateViewOahHandler->
    registerElementInHandler (
      dualHandlerFullViewAtom);

  fAlternateViewOahHandler->
    registerSpecificElementNamesInHandler (
      dualHandlerFullViewAtom);
      */

  // atoms

  fAtomNamesToAlternateSubGroupsMap [fTwoViewAtomShortName] = subGroup;

  fAtomNamesToAlternateSubGroupsMap ["help-options-usage"] = subGroup;
  fAtomNamesToAlternateSubGroupsMap ["option-name-help"] = subGroup;
  fAtomNamesToAlternateSubGroupsMap ["display-options-values"] = subGroup;
  fAtomNamesToAlternateSubGroupsMap ["show-options-and-arguments"] = subGroup;

  fAtomNamesToAlternateSubGroupsMap ["trace-oah"] = subGroup;
  fAtomNamesToAlternateSubGroupsMap ["trace-passes"] = subGroup;
}

void xml2lyTwoViewOahHandler::createInformationsAlternateGroup (
  ostream& os)
{
  // group

  S_oahGroup
    group =
      oahGroup::create (
        "Informations group",
        "hinfos-grp",
        "help-informations-grp",
        "",
        kElementVisibilityWhole,
        fAlternateViewOahHandler);
  group->
    setGroupHeaderIsToBeWritten (false);
  fAlternateViewOahHandler->
    appendGroupToHandler (group);

  // subgroup

  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Informations",
        "hinfos",
        "help-informations",
        "",
        kElementVisibilityWhole,
        group);
  group->
    appendSubGroupToGroup (subGroup);

  // atoms

  fAtomNamesToAlternateSubGroupsMap ["version"] = subGroup;
  fAtomNamesToAlternateSubGroupsMap ["about"] = subGroup;
  fAtomNamesToAlternateSubGroupsMap ["contact"] = subGroup;
  fAtomNamesToAlternateSubGroupsMap ["display-cpu-usage"] = subGroup;
}

void xml2lyTwoViewOahHandler::createWarningAndErrorsAlternateGroup (
  ostream& os)
{
  // group

  S_oahGroup
    group =
      oahGroup::create (
        "Warning and errors group",
        "hwae-grp",
        "help-warning-and-errors-grp",
        "",
        kElementVisibilityWhole,
        fAlternateViewOahHandler);
  group->
    setGroupHeaderIsToBeWritten (false);
  fAlternateViewOahHandler->
    appendGroupToHandler (group);

  // subgroup

  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Warning and errors",
        "hwae",
        "help-warning-and-errors",
        "",
        kElementVisibilityWhole,
        group);
  group->
    appendSubGroupToGroup (subGroup);

  // atoms

  fAtomNamesToAlternateSubGroupsMap ["quiet"] = subGroup;
  fAtomNamesToAlternateSubGroupsMap ["dont-show-errors"] = subGroup;
  fAtomNamesToAlternateSubGroupsMap ["dont-exit-on-errors"] = subGroup;
}

void xml2lyTwoViewOahHandler::createInputAlternateGroup (
  ostream& os)
{
  // group

  S_oahGroup
    group =
      oahGroup::create (
        "Input group",
        "hinput-grp",
        "help-input-grp",
        "",
        kElementVisibilityWhole,
        fAlternateViewOahHandler);
  group->
    setGroupHeaderIsToBeWritten (false);
  fAlternateViewOahHandler->
    appendGroupToHandler (group);
//  fAlternateViewOahHandler-> // JMI
//    registerElementInHandler (group);

  // subgroup

  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Input",
        "hinput",
        "help-input",
        "",
        kElementVisibilityWhole,
        group);
  group->
    appendSubGroupToGroup (subGroup);

  // atoms

  fAtomNamesToAlternateSubGroupsMap ["omit-words"] = subGroup;
  fAtomNamesToAlternateSubGroupsMap ["convert-tempos-to-rehearsal-marks"] = subGroup;
  fAtomNamesToAlternateSubGroupsMap ["convert-words-to-rehearsal-marks"] = subGroup;
  fAtomNamesToAlternateSubGroupsMap ["convert-words-to-tempo"] = subGroup;

  fAtomNamesToAlternateSubGroupsMap ["cubase"] = subGroup;

  fAtomNamesToAlternateSubGroupsMap ["dal-segno"] = subGroup;
}

void xml2lyTwoViewOahHandler::createFilesAlternateGroup (
  ostream& os)
{
  // group

  S_oahGroup
    group =
      oahGroup::create (
        "Files group",
        "hfiles-grp",
        "help-files-grp",
        "",
        kElementVisibilityWhole,
        fAlternateViewOahHandler);
  group->
    setGroupHeaderIsToBeWritten (false);
  fAlternateViewOahHandler->
    appendGroupToHandler (group);

  // subgroup

  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Files",
        "hfiles",
        "help-files",
        "",
        kElementVisibilityWhole,
        group);
  group->
    appendSubGroupToGroup (subGroup);

  // atoms

  fAtomNamesToAlternateSubGroupsMap ["output-file-name"] = subGroup;
  fAtomNamesToAlternateSubGroupsMap ["auto-output-file-name"] = subGroup;
  fAtomNamesToAlternateSubGroupsMap ["loop-back-to-musicxml"] = subGroup;
}

void xml2lyTwoViewOahHandler::createPartsAlternateGroup (
  ostream& os)
{
  // group

  S_oahGroup
    group =
      oahGroup::create (
        "Parts group",
        "hparts-grp",
        "help-parts-grp",
        "",
        kElementVisibilityWhole,
        fAlternateViewOahHandler);
  group->
    setGroupHeaderIsToBeWritten (false);
  fAlternateViewOahHandler->
    appendGroupToHandler (group);

  // subgroup

  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Parts",
        "hparts",
        "help-parts",
        "",
        kElementVisibilityWhole,
        group);
  group->
    appendSubGroupToGroup (subGroup);

  // atoms

  fAtomNamesToAlternateSubGroupsMap ["msr-omit-part-id"] = subGroup;
  fAtomNamesToAlternateSubGroupsMap ["msr-omit-part-name"] = subGroup;
  fAtomNamesToAlternateSubGroupsMap ["msr-keep-part-id"] = subGroup;
  fAtomNamesToAlternateSubGroupsMap ["msr-keep-part-name"] = subGroup;
  fAtomNamesToAlternateSubGroupsMap ["msr-rename-part"] = subGroup;
}

void xml2lyTwoViewOahHandler::createStavesAlternateGroup (
  ostream& os)
{
  // group

  S_oahGroup
    group =
      oahGroup::create (
        "Staves group",
        "hstaves-grp",
        "help-staves-grp",
        "",
        kElementVisibilityWhole,
        fAlternateViewOahHandler);
  group->
    setGroupHeaderIsToBeWritten (false);
  fAlternateViewOahHandler->
    appendGroupToHandler (group);

  // subgroup

  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Staves",
        "hstaves",
        "help-staves",
        "",
        kElementVisibilityWhole,
        group);
  group->
    appendSubGroupToGroup (subGroup);

  // atoms

  fAtomNamesToAlternateSubGroupsMap ["create-voices-staff-relative-numbers"] = subGroup;
}

void xml2lyTwoViewOahHandler::createVoicesAlternateGroup (
  ostream& os)
{
  // group

  S_oahGroup
    group =
      oahGroup::create (
        "Voices group",
        "hvoices-grp",
        "help-voices-grp",
        "",
        kElementVisibilityWhole,
        fAlternateViewOahHandler);
  group->
    setGroupHeaderIsToBeWritten (false);
  fAlternateViewOahHandler->
    appendGroupToHandler (group);

  // subgroup

  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Voices",
        "hvoices",
        "help-voices",
        "",
        kElementVisibilityWhole,
        group);
  group->
    appendSubGroupToGroup (subGroup);

  // atoms

  fAtomNamesToAlternateSubGroupsMap ["create-single-line-staves-as-rythmic"] = subGroup;
  fAtomNamesToAlternateSubGroupsMap ["connect-arpeggios"] = subGroup;
}

void xml2lyTwoViewOahHandler::createTranspositionsAlternateGroup (
  ostream& os)
{
  // group

  S_oahGroup
    group =
      oahGroup::create (
        "Transpositions group",
        "htransp-grp",
        "help-transpositions-grp",
        "",
        kElementVisibilityWhole,
        fAlternateViewOahHandler);
  group->
    setGroupHeaderIsToBeWritten (false);
  fAlternateViewOahHandler->
    appendGroupToHandler (group);

  // subgroup

  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Transpositions",
        "hetransp",
        "help-transpositions",
        "",
        kElementVisibilityWhole,
        group);
  group->
    appendSubGroupToGroup (subGroup);

  // atoms

  fAtomNamesToAlternateSubGroupsMap ["lpsr-transpose"] = subGroup;
  fAtomNamesToAlternateSubGroupsMap ["lilypond-transpose-part"] = subGroup;
}

void xml2lyTwoViewOahHandler::createClefsAlternateGroup (
  ostream& os)
{
  // group

  S_oahGroup
    group =
      oahGroup::create (
        "Clefs group",
        "hlefs-grp",
        "help-clefs-grp",
        "",
        kElementVisibilityWhole,
        fAlternateViewOahHandler);
  group->
    setGroupHeaderIsToBeWritten (false);
  fAlternateViewOahHandler->
    appendGroupToHandler (group);

  // subgroup

  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Clefs",
        "hclefs",
        "help-clefs",
        "",
        kElementVisibilityWhole,
        group);
  group->
    appendSubGroupToGroup (subGroup);

  // atoms

  fAtomNamesToAlternateSubGroupsMap ["msr-replace-clef"] = subGroup;
  fAtomNamesToAlternateSubGroupsMap ["ignore-redundant-clefs"] = subGroup;
  fAtomNamesToAlternateSubGroupsMap ["comment-clef-changes"] = subGroup;
}

void xml2lyTwoViewOahHandler::createKeysAlternateGroup (
  ostream& os)
{
  // group

  S_oahGroup
    group =
      oahGroup::create (
        "Keys group",
        "hkeys-grp",
        "help-keys-grp",
        "",
        kElementVisibilityWhole,
        fAlternateViewOahHandler);
  group->
    setGroupHeaderIsToBeWritten (false);
  fAlternateViewOahHandler->
    appendGroupToHandler (group);

  // subgroup

  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Keys",
        "hkeys",
        "help-keys",
        "",
        kElementVisibilityWhole,
        group);
  group->
    appendSubGroupToGroup (subGroup);

  // atoms

//  fAtomNamesToAlternateSubGroupsMap ["msr-replace-keys"] = subGroup; JMI
  fAtomNamesToAlternateSubGroupsMap ["ignore-redundant-keys"] = subGroup;
}

void xml2lyTwoViewOahHandler::createTimesAlternateGroup (
  ostream& os)
{
  // group

  S_oahGroup
    group =
      oahGroup::create (
        "Times group",
        "htimes-grp",
        "help-times-grp",
        "",
        kElementVisibilityWhole,
        fAlternateViewOahHandler);
  group->
    setGroupHeaderIsToBeWritten (false);
  fAlternateViewOahHandler->
    appendGroupToHandler (group);

  // subgroup

  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Times",
        "htimes",
        "help-times",
        "",
        kElementVisibilityWhole,
        group);
  group->
    appendSubGroupToGroup (subGroup);

  // atoms

  fAtomNamesToAlternateSubGroupsMap ["ignore-redundant-times"] = subGroup;
  fAtomNamesToAlternateSubGroupsMap ["numerical-time"] = subGroup;
}

void xml2lyTwoViewOahHandler::createRepeatsAlternateGroup (
  ostream& os)
{
  // group

  S_oahGroup
    group =
      oahGroup::create (
        "Repeats group",
        "hreps-grp",
        "help-repeats-grp",
        "",
        kElementVisibilityWhole,
        fAlternateViewOahHandler);
  group->
    setGroupHeaderIsToBeWritten (false);
  fAlternateViewOahHandler->
    appendGroupToHandler (group);

  // subgroup

  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Repeats",
        "hreps",
        "help-repeats",
        "",
        kElementVisibilityWhole,
        group);
  group->
    appendSubGroupToGroup (subGroup);

  // atoms

  fAtomNamesToAlternateSubGroupsMap ["create-implicit-initial-repeat-barline"] = subGroup;
  fAtomNamesToAlternateSubGroupsMap ["keep-repeat-barlines"] = subGroup;
  fAtomNamesToAlternateSubGroupsMap ["repeat-brackets"] = subGroup;
  fAtomNamesToAlternateSubGroupsMap ["ignore-repeat-numbers"] = subGroup;
}

void xml2lyTwoViewOahHandler::createMeasuresAlternateGroup (
  ostream& os)
{
  // group

  S_oahGroup
    group =
      oahGroup::create (
        "Measures group",
        "hmeas-grp",
        "help-measures-grp",
        "",
        kElementVisibilityWhole,
        fAlternateViewOahHandler);
  group->
    setGroupHeaderIsToBeWritten (false);
  fAlternateViewOahHandler->
    appendGroupToHandler (group);

  // subgroup

  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Measures",
        "hmeas",
        "help-measures",
        "",
        kElementVisibilityWhole,
        group);
  group->
    appendSubGroupToGroup (subGroup);

  // atoms

  fAtomNamesToAlternateSubGroupsMap ["add-empty-measures"] = subGroup;
  fAtomNamesToAlternateSubGroupsMap ["replicate-empty-measure"] = subGroup;
}

void xml2lyTwoViewOahHandler::createRestsAlternateGroup (
  ostream& os)
{
  // group

  S_oahGroup
    group =
      oahGroup::create (
        "Rests group",
        "hrests-grp",
        "help-rests-grp",
        "",
        kElementVisibilityWhole,
        fAlternateViewOahHandler);
  group->
    setGroupHeaderIsToBeWritten (false);
  fAlternateViewOahHandler->
    appendGroupToHandler (group);

  // subgroup

  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Rests",
        "hrests",
        "help-rests",
        "",
        kElementVisibilityWhole,
        group);
  group->
    appendSubGroupToGroup (subGroup);

  // atoms

  fAtomNamesToAlternateSubGroupsMap ["delay-rests"] = subGroup;
  fAtomNamesToAlternateSubGroupsMap ["delay-rests-slashes"] = subGroup;

  fAtomNamesToAlternateSubGroupsMap ["compress-full-measure-rests"] = subGroup;
  fAtomNamesToAlternateSubGroupsMap ["merge-rests"] = subGroup;
}

void xml2lyTwoViewOahHandler::createNotesAlternateGroup (
  ostream& os)
{
  // group

  S_oahGroup
    group =
      oahGroup::create (
        "Notes group",
        "hnotes-grp",
        "help-notes-grp",
        "",
        kElementVisibilityWhole,
        fAlternateViewOahHandler);
  group->
    setGroupHeaderIsToBeWritten (false);
  fAlternateViewOahHandler->
    appendGroupToHandler (group);

  // subgroup

  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Notes",
        "hnotes",
        "help-notes",
        "",
        kElementVisibilityWhole,
        group);
  group->
    appendSubGroupToGroup (subGroup);

  // atoms

  fAtomNamesToAlternateSubGroupsMap ["msr-pitches-language"] = subGroup;

  fAtomNamesToAlternateSubGroupsMap ["absolute"] = subGroup;
  fAtomNamesToAlternateSubGroupsMap ["relative"] = subGroup;
  fAtomNamesToAlternateSubGroupsMap ["fixed"] = subGroup;

  fAtomNamesToAlternateSubGroupsMap ["white-note-heads"] = subGroup;

  fAtomNamesToAlternateSubGroupsMap ["accidental-style"] = subGroup;

  fAtomNamesToAlternateSubGroupsMap ["no-auto-beaming"] = subGroup;
  fAtomNamesToAlternateSubGroupsMap ["no-beams"] = subGroup;

  fAtomNamesToAlternateSubGroupsMap ["stems"] = subGroup;

  fAtomNamesToAlternateSubGroupsMap ["delay-rests-beams"] = subGroup; // JMI
}

void xml2lyTwoViewOahHandler::creatBeamsAlternateGroup (
  ostream& os)
{
  // group

  S_oahGroup
    group =
      oahGroup::create (
        "Beams group",
        "hbeams-grp",
        "help-beams-grp",
        "",
        kElementVisibilityWhole,
        fAlternateViewOahHandler);
  group->
    setGroupHeaderIsToBeWritten (false);
  fAlternateViewOahHandler->
    appendGroupToHandler (group);

  // subgroup

  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Beams",
        "hbeams",
        "help-beams",
        "",
        kElementVisibilityWhole,
        group);
  group->
    appendSubGroupToGroup (subGroup);

  // atoms

  fAtomNamesToAlternateSubGroupsMap ["no-auto-beaming"] = subGroup;
  fAtomNamesToAlternateSubGroupsMap ["no-beams"] = subGroup;

  fAtomNamesToAlternateSubGroupsMap ["delay-rests-beams"] = subGroup; // JMI
}

void xml2lyTwoViewOahHandler::createArticulationsAlternateGroup (
  ostream& os)
{
  // group

  S_oahGroup
    group =
      oahGroup::create (
        "Articulations group",
        "harts-grp",
        "help-articulations-grp",
        "",
        kElementVisibilityWhole,
        fAlternateViewOahHandler);
  group->
    setGroupHeaderIsToBeWritten (false);
  fAlternateViewOahHandler->
    appendGroupToHandler (group);

  // subgroup

  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Articulations",
        "harts",
        "help-articulations",
        "",
        kElementVisibilityWhole,
        group);
  group->
    appendSubGroupToGroup (subGroup);

  // atoms

  fAtomNamesToAlternateSubGroupsMap ["omit-articulations"] = subGroup;

  fAtomNamesToAlternateSubGroupsMap ["delay-rests-pedals"] = subGroup;
}

void xml2lyTwoViewOahHandler::createOrnamentsAlternateGroup (
  ostream& os)
{
  // group

  S_oahGroup
    group =
      oahGroup::create (
        "Ornaments group",
        "horns-grp",
        "help-ornaments-grp",
        "",
        kElementVisibilityWhole,
        fAlternateViewOahHandler);
  group->
    setGroupHeaderIsToBeWritten (false);
  fAlternateViewOahHandler->
    appendGroupToHandler (group);

  // subgroup

  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Ornaments",
        "horns",
        "help-ornaments",
        "",
        kElementVisibilityWhole,
        group);
  group->
    appendSubGroupToGroup (subGroup);

  // atoms

  fAtomNamesToAlternateSubGroupsMap ["omit-ornaments"] = subGroup;

  fAtomNamesToAlternateSubGroupsMap ["delayed-ornaments-fraction"] = subGroup;
}

void xml2lyTwoViewOahHandler::createGraceNotesAlternateGroup (
  ostream& os)
{
  // group

  S_oahGroup
    group =
      oahGroup::create (
        "Grace notes group",
        "hgraces-grp",
        "help-grace-notes-grp",
        "",
        kElementVisibilityWhole,
        fAlternateViewOahHandler);
  group->
    setGroupHeaderIsToBeWritten (false);
  fAlternateViewOahHandler->
    appendGroupToHandler (group);

  // subgroup

  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Grace notes",
        "hgraces",
        "help-grace-notes",
        "",
        kElementVisibilityWhole,
        group);
  group->
    appendSubGroupToGroup (subGroup);

  // atoms

  fAtomNamesToAlternateSubGroupsMap ["slash-all-grace-notes"] = subGroup;
  fAtomNamesToAlternateSubGroupsMap ["slur-all-grace-notes"] = subGroup;
  fAtomNamesToAlternateSubGroupsMap ["beam-all-grace-notes"] = subGroup;
}

void xml2lyTwoViewOahHandler::createChordsAlternateGroup (
  ostream& os)
{
  // group

  S_oahGroup
    group =
      oahGroup::create (
        "Chords group",
        "hchords-grp",
        "help-chords-grp",
        "",
        kElementVisibilityWhole,
        fAlternateViewOahHandler);
  group->
    setGroupHeaderIsToBeWritten (false);
  fAlternateViewOahHandler->
    appendGroupToHandler (group);

  // subgroup

  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Chords",
        "hchords",
        "help-chords",
        "",
        kElementVisibilityWhole,
        group);
  group->
    appendSubGroupToGroup (subGroup);

  // atoms

  fAtomNamesToAlternateSubGroupsMap ["trace-chords"] = subGroup;
  fAtomNamesToAlternateSubGroupsMap ["add-words-from-the-chords"] = subGroup;

  fAtomNamesToAlternateSubGroupsMap ["lpsr-pitches-language"] = subGroup;

  fAtomNamesToAlternateSubGroupsMap ["lpsr-chords-language"] = subGroup;

  fAtomNamesToAlternateSubGroupsMap ["chords-display"] = subGroup;
}

void xml2lyTwoViewOahHandler::createTiesAlternateGroup (
  ostream& os)
{
  // group

  S_oahGroup
    group =
      oahGroup::create (
        "Ties group",
        "hties-grp",
        "help-ties-grp",
        "",
        kElementVisibilityWhole,
        fAlternateViewOahHandler);
  group->
    setGroupHeaderIsToBeWritten (false);
  fAlternateViewOahHandler->
    appendGroupToHandler (group);

  // subgroup

  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Ties",
        "hties",
        "help-ties",
        "",
        kElementVisibilityWhole,
        group);
  group->
    appendSubGroupToGroup (subGroup);

  // atoms

  fAtomNamesToAlternateSubGroupsMap ["omit-ties"] = subGroup;
}

void xml2lyTwoViewOahHandler::createSlursAlternateGroup (
  ostream& os)
{
  // group

  S_oahGroup
    group =
      oahGroup::create (
        "Slurs group",
        "hslurs-grp",
        "help-slurs-grp",
        "",
        kElementVisibilityWhole,
        fAlternateViewOahHandler);
  group->
    setGroupHeaderIsToBeWritten (false);
  fAlternateViewOahHandler->
    appendGroupToHandler (group);

  // subgroup

  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Slurs",
        "hslurs",
        "help-slurs",
        "",
        kElementVisibilityWhole,
        group);
  group->
    appendSubGroupToGroup (subGroup);

  // atoms

  fAtomNamesToAlternateSubGroupsMap ["omit-slurs"] = subGroup;

  fAtomNamesToAlternateSubGroupsMap ["delay-slurs"] = subGroup;
  fAtomNamesToAlternateSubGroupsMap ["delay-rests-slurs"] = subGroup;
}

void xml2lyTwoViewOahHandler::createLigaturesAlternateGroup (
  ostream& os)
{
  // group

  S_oahGroup
    group =
      oahGroup::create (
        "Ligatures group",
        "hligs-grp",
        "help-ligatures-grp",
        "",
        kElementVisibilityWhole,
        fAlternateViewOahHandler);
  group->
    setGroupHeaderIsToBeWritten (false);
  fAlternateViewOahHandler->
    appendGroupToHandler (group);

  // subgroup

  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Ligatures",
        "hligs",
        "help-ligatures",
        "",
        kElementVisibilityWhole,
        group);
  group->
    appendSubGroupToGroup (subGroup);

  // atoms

  fAtomNamesToAlternateSubGroupsMap ["delay-rests-ligatures"] = subGroup;
}

void xml2lyTwoViewOahHandler::createDynamicsAlternateGroup (
  ostream& os)
{
  // group

  S_oahGroup
    group =
      oahGroup::create (
        "Dynamics group",
        "hdyns-grp",
        "help-dynamics-grp",
        "",
        kElementVisibilityWhole,
        fAlternateViewOahHandler);
  group->
    setGroupHeaderIsToBeWritten (false);
  fAlternateViewOahHandler->
    appendGroupToHandler (group);

  // subgroup

  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Dynamics",
        "hdyns",
        "help-dynamics",
        "",
        kElementVisibilityWhole,
        group);
  group->
    appendSubGroupToGroup (subGroup);

  // atoms

  fAtomNamesToAlternateSubGroupsMap ["omit-dynamics"] = subGroup;

  fAtomNamesToAlternateSubGroupsMap ["all-dynamics-below"] = subGroup;

  fAtomNamesToAlternateSubGroupsMap ["delay-dynamics"] = subGroup;

  fAtomNamesToAlternateSubGroupsMap ["delay-rests-dynamics"] = subGroup;
}

void xml2lyTwoViewOahHandler::createWedgesAlternateGroup (
  ostream& os)
{
  // group

  S_oahGroup
    group =
      oahGroup::create (
        "Wedges group",
        "hweds-grp",
        "help-wedges-grp",
        "",
        kElementVisibilityWhole,
        fAlternateViewOahHandler);
  group->
    setGroupHeaderIsToBeWritten (false);
  fAlternateViewOahHandler->
    appendGroupToHandler (group);

  // subgroup

  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Wedges",
        "hweds",
        "help-wedges",
        "",
        kElementVisibilityWhole,
        group);
  group->
    appendSubGroupToGroup (subGroup);

  // atoms

  fAtomNamesToAlternateSubGroupsMap ["omit-wedges"] = subGroup;

  fAtomNamesToAlternateSubGroupsMap ["all-wedges-below"] = subGroup;

  fAtomNamesToAlternateSubGroupsMap ["delay-rests-wedges"] = subGroup;
}

void xml2lyTwoViewOahHandler::createTupletsAlternateGroup (
  ostream& os)
{
  // group

  S_oahGroup
    group =
      oahGroup::create (
        "Tuplets group",
        "htups-grp",
        "help-tuplets-grp",
        "",
        kElementVisibilityWhole,
        fAlternateViewOahHandler);
  group->
    setGroupHeaderIsToBeWritten (false);
  fAlternateViewOahHandler->
    appendGroupToHandler (group);

  // subgroup

  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Tuplets",
        "htups",
        "help-tuplets",
        "",
        kElementVisibilityWhole,
        group);
  group->
    appendSubGroupToGroup (subGroup);

  // atoms

  fAtomNamesToAlternateSubGroupsMap ["indent-tuplets"] = subGroup;
//  fAtomNamesToAlternateSubGroupsMap ["add-words-from-the-tuplets"] = subGroup; JMI

  fAtomNamesToAlternateSubGroupsMap ["lpsr-tuplets-language"] = subGroup;
}

void xml2lyTwoViewOahHandler::createStringsAlternateGroup (
  ostream& os)
{
  // group

  S_oahGroup
    group =
      oahGroup::create (
        "Strings group",
        "hstrings-grp",
        "help-strings-grp",
        "",
        kElementVisibilityWhole,
        fAlternateViewOahHandler);
  group->
    setGroupHeaderIsToBeWritten (false);
  fAlternateViewOahHandler->
    appendGroupToHandler (group);

  // subgroup

  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Strings",
        "hstrings",
        "",
        "",
        kElementVisibilityWhole,
        group);
  group->
    appendSubGroupToGroup (subGroup);

  // atoms

  fAtomNamesToAlternateSubGroupsMap ["roman-string-numbers"] = subGroup;
  fAtomNamesToAlternateSubGroupsMap ["avoid-open-strings"] = subGroup;
}

void xml2lyTwoViewOahHandler::createTablaturesAlternateGroup (
  ostream& os)
{
  // group

  S_oahGroup
    group =
      oahGroup::create (
        "Tablatures group",
        "htabs-grp",
        "help-tablatures-grp",
        "",
        kElementVisibilityWhole,
        fAlternateViewOahHandler);
  group->
    setGroupHeaderIsToBeWritten (false);
  fAlternateViewOahHandler->
    appendGroupToHandler (group);

  // subgroup

  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Tablatures",
        "htabs",
        "help-tablatures",
        "",
        kElementVisibilityWhole,
        group);
  group->
    appendSubGroupToGroup (subGroup);

  // atoms

  fAtomNamesToAlternateSubGroupsMap ["tabFullNotation"] = subGroup;
  fAtomNamesToAlternateSubGroupsMap ["modernTab"] = subGroup;
}

void xml2lyTwoViewOahHandler::createHarmoniesAlternateGroup (
  ostream& os)
{
  // group

  S_oahGroup
    group =
      oahGroup::create (
        "Harmonies group",
        "hharms-grp",
        "help-harmonies-grp",
        "",
        kElementVisibilityWhole,
        fAlternateViewOahHandler);
  group->
    setGroupHeaderIsToBeWritten (false);
  fAlternateViewOahHandler->
    appendGroupToHandler (group);

  // subgroup

  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Harmonies",
        "hharms",
        "help-harmonies",
        "",
        kElementVisibilityWhole,
        group);
  group->
    appendSubGroupToGroup (subGroup);

  // atoms

  fAtomNamesToAlternateSubGroupsMap ["omit-harmonies"] = subGroup;

  fAtomNamesToAlternateSubGroupsMap ["show-harmony-voices"] = subGroup;
  fAtomNamesToAlternateSubGroupsMap ["harmonies-display"] = subGroup;
  fAtomNamesToAlternateSubGroupsMap ["jazz-harmonies-display"] = subGroup;
  fAtomNamesToAlternateSubGroupsMap ["show-harmonies-structures"] = subGroup;
  fAtomNamesToAlternateSubGroupsMap ["show-all-harmonies-contents"] = subGroup;
  fAtomNamesToAlternateSubGroupsMap ["show-harmony-details"] = subGroup;
  fAtomNamesToAlternateSubGroupsMap ["show-harmony-analysis"] = subGroup;
}

void xml2lyTwoViewOahHandler::createFiguredBassesAlternateGroup (
  ostream& os)
{
  // group

  S_oahGroup
    group =
      oahGroup::create (
        "Figured basses group",
        "hfigbass-grp",
        "help-figured-basses-grp",
        "",
        kElementVisibilityWhole,
        fAlternateViewOahHandler);
  group->
    setGroupHeaderIsToBeWritten (false);
  fAlternateViewOahHandler->
    appendGroupToHandler (group);

  // subgroup

  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Figured basses",
        "hfigbass",
        "help-figured-basses",
        "",
        kElementVisibilityWhole,
        group);
  group->
    appendSubGroupToGroup (subGroup);

  // atoms

  fAtomNamesToAlternateSubGroupsMap ["omit-figured-basses"] = subGroup;

  fAtomNamesToAlternateSubGroupsMap ["show-figured-bass-voices"] = subGroup;
}

void xml2lyTwoViewOahHandler::createLyricsAlternateGroup (
  ostream& os)
{
  // group

  S_oahGroup
    group =
      oahGroup::create (
        "Lyrics group",
        "hlyrics-grp",
        "help-lyrics-grp",
        "",
        kElementVisibilityWhole,
        fAlternateViewOahHandler);
  group->
    setGroupHeaderIsToBeWritten (false);
  fAlternateViewOahHandler->
    appendGroupToHandler (group);

  // subgroup

  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Lyrics",
        "hlyrics",
        "help-lyrics",
        "",
        kElementVisibilityWhole,
        group);
  group->
    appendSubGroupToGroup (subGroup);

  // atoms

  fAtomNamesToAlternateSubGroupsMap ["omit-lyrics"] = subGroup;
  fAtomNamesToAlternateSubGroupsMap ["add-words-from-the-lyrics"] = subGroup;

  fAtomNamesToAlternateSubGroupsMap ["no-lilypond-lyrics"] =subGroup;
  fAtomNamesToAlternateSubGroupsMap ["lyrics-durations"] =subGroup;
  fAtomNamesToAlternateSubGroupsMap ["add-stanzas-numbers-to-lyrics"] =subGroup;
  fAtomNamesToAlternateSubGroupsMap ["lyrics-durations"] =subGroup;
}

void xml2lyTwoViewOahHandler::createHeaderAlternateGroup (
  ostream& os)
{
  // group

  S_oahGroup
    group =
      oahGroup::create (
        "Header group",
        "hheader-grp",
        "help-header-grp",
        "",
        kElementVisibilityWhole,
        fAlternateViewOahHandler);
  group->
    setGroupHeaderIsToBeWritten (false);
  fAlternateViewOahHandler->
    appendGroupToHandler (group);

  // subgroup

  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Header",
        "hheader",
        "help-header",
        "",
        kElementVisibilityWhole,
        group);
  group->
    appendSubGroupToGroup (subGroup);

  // atoms

  fAtomNamesToAlternateSubGroupsMap ["rights"] = subGroup;

  fAtomNamesToAlternateSubGroupsMap ["composer"] = subGroup;
  fAtomNamesToAlternateSubGroupsMap ["arranger"] = subGroup;
  fAtomNamesToAlternateSubGroupsMap ["lyricist"] = subGroup;
  fAtomNamesToAlternateSubGroupsMap ["poet"] = subGroup;

  fAtomNamesToAlternateSubGroupsMap ["software"] = subGroup;

  fAtomNamesToAlternateSubGroupsMap ["dedication"] = subGroup;
  fAtomNamesToAlternateSubGroupsMap ["piece"] = subGroup;
  fAtomNamesToAlternateSubGroupsMap ["opus"] = subGroup;

  fAtomNamesToAlternateSubGroupsMap ["title"] = subGroup;
  fAtomNamesToAlternateSubGroupsMap ["subtitle"] = subGroup;
  fAtomNamesToAlternateSubGroupsMap ["subsubtitle"] = subGroup;

  fAtomNamesToAlternateSubGroupsMap ["instrument"] = subGroup;

  fAtomNamesToAlternateSubGroupsMap ["meter"] = subGroup;

  fAtomNamesToAlternateSubGroupsMap ["copyright"] = subGroup;

  fAtomNamesToAlternateSubGroupsMap ["tagline"] = subGroup;

  fAtomNamesToAlternateSubGroupsMap ["use-lyricists-as-poets"] = subGroup;

  fAtomNamesToAlternateSubGroupsMap ["use-filename-as-work-title"] = subGroup;
}

void xml2lyTwoViewOahHandler::createPaperAlternateGroup (
  ostream& os)
{
  // group

  S_oahGroup
    group =
      oahGroup::create (
        "Paper group",
        "h-grp",
        "help-paper-grp",
        "",
        kElementVisibilityWhole,
        fAlternateViewOahHandler);
  group->
    setGroupHeaderIsToBeWritten (false);
  fAlternateViewOahHandler->
    appendGroupToHandler (group);

  // subgroup

  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Paper",
        "hpaper",
        "help-paper",
        "",
        kElementVisibilityWhole,
        group);
  group->
    appendSubGroupToGroup (subGroup);

  // atoms

  fAtomNamesToAlternateSubGroupsMap ["paper-height"] = subGroup;
  fAtomNamesToAlternateSubGroupsMap ["paper-width"] = subGroup;

  fAtomNamesToAlternateSubGroupsMap ["ragged-bottom"] = subGroup;
  fAtomNamesToAlternateSubGroupsMap ["ragged-last"] = subGroup;
  fAtomNamesToAlternateSubGroupsMap ["ragged-last-bottom"] = subGroup;
  fAtomNamesToAlternateSubGroupsMap ["ragged-right"] = subGroup;

  fAtomNamesToAlternateSubGroupsMap ["horizontal-shift"] = subGroup;
  fAtomNamesToAlternateSubGroupsMap ["indent"] = subGroup;
  fAtomNamesToAlternateSubGroupsMap ["short-indent"] = subGroup;

  fAtomNamesToAlternateSubGroupsMap ["markup-system-spacing.padding"] = subGroup;

  fAtomNamesToAlternateSubGroupsMap ["page-count"] = subGroup;
  fAtomNamesToAlternateSubGroupsMap ["system-count"] = subGroup;
}

void xml2lyTwoViewOahHandler::createLayoutAlternateGroup (
  ostream& os)
{
  // group

  S_oahGroup
    group =
      oahGroup::create (
        "Layout group",
        "hlayout-grp",
        "help-layout-grp",
        "",
        kElementVisibilityWhole,
        fAlternateViewOahHandler);
  group->
    setGroupHeaderIsToBeWritten (false);
  fAlternateViewOahHandler->
    appendGroupToHandler (group);

  // subgroup

  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Layout",
        "hlayout",
        "help-layout",
        "",
        kElementVisibilityWhole,
        group);
  group->
    appendSubGroupToGroup (subGroup);

  // atoms

  fAtomNamesToAlternateSubGroupsMap ["global-staff-size"] = subGroup;
  fAtomNamesToAlternateSubGroupsMap ["keep-staff-size"] = subGroup;

}

void xml2lyTwoViewOahHandler::createOutputAlternateGroup (
  ostream& os)
{
  // group

  S_oahGroup
    group =
      oahGroup::create (
        "Ouptut group",
        "houptut-grp",
        "help-ouptut-grp",
        "",
        kElementVisibilityWhole,
        fAlternateViewOahHandler);
  group->
    setGroupHeaderIsToBeWritten (false);
  fAlternateViewOahHandler->
    appendGroupToHandler (group);

  // subgroup

  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Ouptut",
        "houptut",
        "help-ouptut",
        "",
        kElementVisibilityWhole,
        group);
  group->
    appendSubGroupToGroup (subGroup);

  // atoms

  fAtomNamesToAlternateSubGroupsMap ["xml2ly-infos"] = subGroup;

  fAtomNamesToAlternateSubGroupsMap ["no-lilypond-code"] = subGroup;

  fAtomNamesToAlternateSubGroupsMap ["draft-mode"] = subGroup;
  fAtomNamesToAlternateSubGroupsMap ["point-and-clic-off"] = subGroup;

  fAtomNamesToAlternateSubGroupsMap ["lilypond-version"] = subGroup;
  fAtomNamesToAlternateSubGroupsMap ["lilypond-compile-date"] = subGroup;
  fAtomNamesToAlternateSubGroupsMap ["lilypond-comments"] = subGroup;

  fAtomNamesToAlternateSubGroupsMap ["msr2lpsr-score-output-kind"] = subGroup;
  fAtomNamesToAlternateSubGroupsMap ["lpsr-Languages-language"] = subGroup;

  fAtomNamesToAlternateSubGroupsMap ["global"] = subGroup;

  fAtomNamesToAlternateSubGroupsMap ["ambitus"] = subGroup;

  fAtomNamesToAlternateSubGroupsMap ["input-line-numbers"] = subGroup;
  fAtomNamesToAlternateSubGroupsMap ["positions-in-measures"] = subGroup;

  fAtomNamesToAlternateSubGroupsMap ["all-bar-numbers"] = subGroup;
  fAtomNamesToAlternateSubGroupsMap ["all-measure-numbers"] = subGroup;
  fAtomNamesToAlternateSubGroupsMap ["generate-box-around-bar-number"] = subGroup;
  fAtomNamesToAlternateSubGroupsMap ["reset-measure-number"] = subGroup;
  fAtomNamesToAlternateSubGroupsMap ["original-measure-numbers"] = subGroup;

  fAtomNamesToAlternateSubGroupsMap ["separator-line-every-n-measures"] = subGroup;

  fAtomNamesToAlternateSubGroupsMap ["ignore-musicxml-line-breaks"] = subGroup;
  fAtomNamesToAlternateSubGroupsMap ["ignore-musixcml-page-breaks"] = subGroup;
  fAtomNamesToAlternateSubGroupsMap ["break-lines-at-incomplete-right-measures"] = subGroup;
  fAtomNamesToAlternateSubGroupsMap ["custos"] = subGroup;

  fAtomNamesToAlternateSubGroupsMap ["length-unit"] = subGroup;

  fAtomNamesToAlternateSubGroupsMap ["all-durations"] = subGroup;

  fAtomNamesToAlternateSubGroupsMap ["ledger-lines-color"] = subGroup;

  fAtomNamesToAlternateSubGroupsMap ["jazz-fonts"] = subGroup;

  fAtomNamesToAlternateSubGroupsMap ["jianpu"] = subGroup;
}

void xml2lyTwoViewOahHandler::createMidiAlternateGroup (
  ostream& os)
{
  // group

  S_oahGroup
    group =
      oahGroup::create (
        "MIDI group",
        "hmidi-grp",
        "help-midi-grp",
        "",
        kElementVisibilityWhole,
        fAlternateViewOahHandler);
  group->
    setGroupHeaderIsToBeWritten (false);
  fAlternateViewOahHandler->
    appendGroupToHandler (group);

  // subgroup

  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "MIDI",
        "hmidi",
        "help-midi",
        "",
        kElementVisibilityWhole,
        group);
  group->
    appendSubGroupToGroup (subGroup);

  // atoms

  fAtomNamesToAlternateSubGroupsMap ["no-midi"] = subGroup;
  fAtomNamesToAlternateSubGroupsMap ["midi-tempo"] = subGroup;
}

void xml2lyTwoViewOahHandler::checkOptionsAndArguments ()
{
  switch (fViewHandlerKind) {
    case oahTwoViewHandler::kFullViewHandler:
      fFullViewOahHandler->
        checkOptionsAndArguments ();
      break;
    case oahTwoViewHandler::kAlternateViewHandler:
      {
        stringstream s;

        s <<
          "FullViewHandler expected for checkOptionsAndArguments()"; // JMI

        oahError (s.str ());
      }
      break;
  } // switch
}

void xml2lyTwoViewOahHandler::enforceTwoViewHandlerQuietness ()
{

  switch (fViewHandlerKind) {
    case oahTwoViewHandler::kFullViewHandler:
      fFullViewOahHandler->
        enforceHandlerQuietness ();
      break;
    case oahTwoViewHandler::kAlternateViewHandler:
      fAlternateViewOahHandler->
        enforceHandlerQuietness ();
      break;
  } // switch
}

string xml2lyTwoViewOahHandler::asString () const
{
  return "xml2lyTwoViewOahHandler";
}

void xml2lyTwoViewOahHandler::print (ostream& os) const
{
  os <<
    "xml2lyTwoViewOahHandler:" <<
    endl;

  gIndenter++;

  os <<
    "??????????? fullViewHandler" <<
    endl;

  gIndenter++;

  os << fFullViewOahHandler << endl;
  /*
  const list<S_oahGroup>
    fullViewHandlerGroupsList =
      fFullViewOahHandler->getHandlerGroupsList ();

  if (fullViewHandlerGroupsList.size ()) {
    os << endl;

    list<S_oahGroup>::const_iterator
      iBegin = fullViewHandlerGroupsList.begin (),
      iEnd   = fullViewHandlerGroupsList.end (),
      i      = iBegin;
    for ( ; ; ) {
      // print the group
      os << (*i);
      if (++i == iEnd) break;
      os << endl;
    } // for
  }
*/
  gIndenter--;

  os <<
    "??????????? alternateOahHandler" <<
    endl;

  gIndenter++;

  os << fAlternateViewOahHandler << endl;
  /*
  const list<S_oahGroup>
    alternateOahHandlerGroupsList =
      fAlternateViewOahHandler->getHandlerGroupsList ();

  if (alternateOahHandlerGroupsList.size ()) {
    os << endl;

    list<S_oahGroup>::const_iterator
      iBegin = alternateOahHandlerGroupsList.begin (),
      iEnd   = alternateOahHandlerGroupsList.end (),
      i      = iBegin;
    for ( ; ; ) {
      // print the group
      os << (*i);
      if (++i == iEnd) break;
      os << endl;
    } // for
  }
*/
  gIndenter--;

  gIndenter--;

  os << endl;
}

ostream& operator<< (ostream& os, const S_xml2lyTwoViewOahHandler& elt)
{
  elt->print (os);
  return os;
}


}
