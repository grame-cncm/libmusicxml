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

#include "utilities.h"
#include "messagesHandling.h"

#include "xml2lyInsiderOah.h"

#include "xml2lyOahDualHandler.h"


using namespace std;

namespace MusicXML2
{
//______________________________________________________________________________
S_xml2lyOahDualHandler xml2lyOahDualHandler::create (
  string   executableName,
  ostream& os)
{
  xml2lyOahDualHandler* o = new
    xml2lyOahDualHandler (
      executableName,
      os);
  assert(o!=0);

  o->createTheTwoHandlers (os);

  return o;
}

xml2lyOahDualHandler::xml2lyOahDualHandler (
  string   executableName,
  ostream& os)
  : oahDualHandler (
      "xml2lyOahDualHandler",
      executableName,
      "insider",
      "",
      os)
{}

xml2lyOahDualHandler::~xml2lyOahDualHandler ()
{}

void xml2lyOahDualHandler::createTheTwoHandlers (
  ostream& os)
{
#ifdef TRACE_OAH
  if (true) { // JMI
    gLogOstream <<
      "Creating the two OAH handlers for \"" << fDualHandlerName << "\"" <<
      endl;
  }
#endif

  // create the 'insider' handler
// JMI  os << "createInsiderHandler(), fExecutableName = " << fExecutableName << endl;
  fXml2lyInsiderHandler =
    xml2lyInsiderOahHandler::create (
      fExecutableName,
      fDualHandlerName,
      os);

  // propagate it into the base class
  setInsiderHandler (fXml2lyInsiderHandler);

  // create the 'user' handler
// JMI  os << "createUserHandler(), fExecutableName = " << fExecutableName << endl;
  fXml2lyUserHandler =
    fXml2lyInsiderHandler->
      createHandlerNewbornCloneWithoutGroups ();

  // propagate it into the base class
  setUserHandler (fXml2lyUserHandler);

  // create the user handler groups
  createUserHandlerGroups (os);

  // populate the user handler from the insider handler
  populateUserHandlerFromInsiderHandler ();

  // the default is to use the 'user' oahHandler
if (true) { // JMI TESTS
  fOahHandlerToBeUsed = fUserHandler;
} else {
  fOahHandlerToBeUsed = fInsiderHandler;
}
}

void xml2lyOahDualHandler::createUserHandlerGroups (
  ostream& os)
{
// JMI  os << "createUserHandlerGroups()" << endl;

  createOahUserGroup (os);

  createInformationsUserGroup (os);

  createWarningAndErrorsUserGroup (os);

  createFilesUserGroup (os);

  createInputUserGroup (os);

  createPartsUserGroup (os);
  createStavesUserGroup (os);
  createVoicesUserGroup (os);

  createTranspositionsUserGroup (os);

  createClefsUserGroup (os);
  createKeysUserGroup (os);
  createTimesUserGroup (os);

  createRepeatsUserGroup (os);

  createMeasuresUserGroup (os);

  createRestsUserGroup (os);
  createNotesUserGroup (os);

  creatBeamsUserGroup (os);

  createArticulationsUserGroup (os);

  createOrnamentsUserGroup (os);

  createGraceNotesUserGroup (os);

  createChordsUserGroup (os);

  createTiesUserGroup (os);
  createSlursUserGroup (os);
  createLigaturesUserGroup (os);

  createDynamicsUserGroup (os);
  createWedgesUserGroup (os);

  createTupletsUserGroup (os);

  createLyricsUserGroup (os);

  createStringsUserGroup (os);

  createTablaturesUserGroup (os);

  createHarmoniesUserGroup (os);

  createFiguredBassesUserGroup (os);

  createHeaderUserGroup (os);
  createPaperUserGroup (os);
  createLayoutUserGroup (os);

  createOutputUserGroup (os);

  createMidiUserGroup (os);

  // register options handler in itself,
  // so that the 'global' help options can be handled

  fUserHandler->
    registerHandlerInItself ();
}

void xml2lyOahDualHandler::createOahUserGroup (
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
        fUserHandler);
  group->
    setGroupHeaderIsToBeWritten (false);
  fUserHandler->
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

  // atoms

  fAtomNamesToUserSubGroupsMap ["insider"] = subGroup;

  fAtomNamesToUserSubGroupsMap ["help-options-usage"] = subGroup;
  fAtomNamesToUserSubGroupsMap ["option-name-help"] = subGroup;
  fAtomNamesToUserSubGroupsMap ["display-options-values"] = subGroup;
  fAtomNamesToUserSubGroupsMap ["show-options-and-arguments"] = subGroup;

  fAtomNamesToUserSubGroupsMap ["trace-oah"] = subGroup;
  fAtomNamesToUserSubGroupsMap ["trace-passes"] = subGroup;
}

void xml2lyOahDualHandler::createInformationsUserGroup (
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
        fUserHandler);
  group->
    setGroupHeaderIsToBeWritten (false);
  fUserHandler->
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

  fAtomNamesToUserSubGroupsMap ["version"] = subGroup;
  fAtomNamesToUserSubGroupsMap ["about"] = subGroup;
  fAtomNamesToUserSubGroupsMap ["contact"] = subGroup;
  fAtomNamesToUserSubGroupsMap ["display-cpu-usage"] = subGroup;
}

void xml2lyOahDualHandler::createWarningAndErrorsUserGroup (
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
        fUserHandler);
  group->
    setGroupHeaderIsToBeWritten (false);
  fUserHandler->
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

  fAtomNamesToUserSubGroupsMap ["quiet"] = subGroup;
  fAtomNamesToUserSubGroupsMap ["dont-show-errors"] = subGroup;
  fAtomNamesToUserSubGroupsMap ["dont-exit-on-errors"] = subGroup;
// JMI ???  fAtomNamesToUserSubGroupsMap ["display-source-code-position"] = subGroup;
}

void xml2lyOahDualHandler::createInputUserGroup (
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
        fUserHandler);
  group->
    setGroupHeaderIsToBeWritten (false);
  fUserHandler->
    appendGroupToHandler (group);
//  fUserHandler-> // JMI
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

  fAtomNamesToUserSubGroupsMap ["omit-words"] = subGroup;
  fAtomNamesToUserSubGroupsMap ["convert-tempos-to-rehearsal-marks"] = subGroup;
  fAtomNamesToUserSubGroupsMap ["convert-words-to-rehearsal-marks"] = subGroup;
  fAtomNamesToUserSubGroupsMap ["convert-words-to-tempo"] = subGroup;

  fAtomNamesToUserSubGroupsMap ["cubase"] = subGroup;

  fAtomNamesToUserSubGroupsMap ["dal-segno"] = subGroup;
}

void xml2lyOahDualHandler::createFilesUserGroup (
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
        fUserHandler);
  group->
    setGroupHeaderIsToBeWritten (false);
  fUserHandler->
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

  fAtomNamesToUserSubGroupsMap ["output-file-name"] = subGroup;
  fAtomNamesToUserSubGroupsMap ["auto-output-file-name"] = subGroup;
  fAtomNamesToUserSubGroupsMap ["loop-back-to-musicxml"] = subGroup;
}

void xml2lyOahDualHandler::createPartsUserGroup (
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
        fUserHandler);
  group->
    setGroupHeaderIsToBeWritten (false);
  fUserHandler->
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

  fAtomNamesToUserSubGroupsMap ["msr-omit-part-id"] = subGroup;
  fAtomNamesToUserSubGroupsMap ["msr-omit-part-name"] = subGroup;
  fAtomNamesToUserSubGroupsMap ["msr-keep-part-id"] = subGroup;
  fAtomNamesToUserSubGroupsMap ["msr-keep-part-name"] = subGroup;
  fAtomNamesToUserSubGroupsMap ["msr-rename-part"] = subGroup;
}

void xml2lyOahDualHandler::createStavesUserGroup (
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
        fUserHandler);
  group->
    setGroupHeaderIsToBeWritten (false);
  fUserHandler->
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

  fAtomNamesToUserSubGroupsMap ["create-voices-staff-relative-numbers"] = subGroup;
}

void xml2lyOahDualHandler::createVoicesUserGroup (
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
        fUserHandler);
  group->
    setGroupHeaderIsToBeWritten (false);
  fUserHandler->
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

  fAtomNamesToUserSubGroupsMap ["create-single-line-staves-as-rythmic"] = subGroup;
  fAtomNamesToUserSubGroupsMap ["connect-arpeggios"] = subGroup;
}

void xml2lyOahDualHandler::createTranspositionsUserGroup (
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
        fUserHandler);
  group->
    setGroupHeaderIsToBeWritten (false);
  fUserHandler->
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

  fAtomNamesToUserSubGroupsMap ["lpsr-transpose"] = subGroup;
  fAtomNamesToUserSubGroupsMap ["lilypond-transpose-part"] = subGroup;
}

void xml2lyOahDualHandler::createClefsUserGroup (
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
        fUserHandler);
  group->
    setGroupHeaderIsToBeWritten (false);
  fUserHandler->
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

  fAtomNamesToUserSubGroupsMap ["msr-replace-clef"] = subGroup;
  fAtomNamesToUserSubGroupsMap ["ignore-redundant-clefs"] = subGroup;
  fAtomNamesToUserSubGroupsMap ["comment-clef-changes"] = subGroup;
}

void xml2lyOahDualHandler::createKeysUserGroup (
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
        fUserHandler);
  group->
    setGroupHeaderIsToBeWritten (false);
  fUserHandler->
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

//  fAtomNamesToUserSubGroupsMap ["msr-replace-keys"] = subGroup; JMI
  fAtomNamesToUserSubGroupsMap ["ignore-redundant-keys"] = subGroup;
}

void xml2lyOahDualHandler::createTimesUserGroup (
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
        fUserHandler);
  group->
    setGroupHeaderIsToBeWritten (false);
  fUserHandler->
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

  fAtomNamesToUserSubGroupsMap ["ignore-redundant-times"] = subGroup;
  fAtomNamesToUserSubGroupsMap ["numerical-time"] = subGroup;
}

void xml2lyOahDualHandler::createRepeatsUserGroup (
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
        fUserHandler);
  group->
    setGroupHeaderIsToBeWritten (false);
  fUserHandler->
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

  fAtomNamesToUserSubGroupsMap ["create-implicit-initial-repeat-barline"] = subGroup;
  fAtomNamesToUserSubGroupsMap ["keep-repeat-barlines"] = subGroup;
  fAtomNamesToUserSubGroupsMap ["repeat-brackets"] = subGroup;
  fAtomNamesToUserSubGroupsMap ["ignore-repeat-numbers"] = subGroup;
}

void xml2lyOahDualHandler::createMeasuresUserGroup (
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
        fUserHandler);
  group->
    setGroupHeaderIsToBeWritten (false);
  fUserHandler->
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

  fAtomNamesToUserSubGroupsMap ["add-empty-measures"] = subGroup;
  fAtomNamesToUserSubGroupsMap ["replicate-empty-measure"] = subGroup;
}

void xml2lyOahDualHandler::createRestsUserGroup (
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
        fUserHandler);
  group->
    setGroupHeaderIsToBeWritten (false);
  fUserHandler->
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

  fAtomNamesToUserSubGroupsMap ["delay-rests"] = subGroup;
  fAtomNamesToUserSubGroupsMap ["delay-rests-slashes"] = subGroup;

  fAtomNamesToUserSubGroupsMap ["compress-full-measure-rests"] = subGroup;
  fAtomNamesToUserSubGroupsMap ["merge-rests"] = subGroup;
}

void xml2lyOahDualHandler::createNotesUserGroup (
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
        fUserHandler);
  group->
    setGroupHeaderIsToBeWritten (false);
  fUserHandler->
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

  fAtomNamesToUserSubGroupsMap ["msr-pitches-language"] = subGroup;

  fAtomNamesToUserSubGroupsMap ["absolute"] = subGroup;
  fAtomNamesToUserSubGroupsMap ["relative"] = subGroup;
  fAtomNamesToUserSubGroupsMap ["fixed"] = subGroup;

  fAtomNamesToUserSubGroupsMap ["white-note-heads"] = subGroup;

  fAtomNamesToUserSubGroupsMap ["accidental-style"] = subGroup;

  fAtomNamesToUserSubGroupsMap ["no-auto-beaming"] = subGroup;
  fAtomNamesToUserSubGroupsMap ["no-beams"] = subGroup;

  fAtomNamesToUserSubGroupsMap ["stems"] = subGroup;

  fAtomNamesToUserSubGroupsMap ["delay-rests-beams"] = subGroup; // JMI
}

void xml2lyOahDualHandler::creatBeamsUserGroup (
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
        fUserHandler);
  group->
    setGroupHeaderIsToBeWritten (false);
  fUserHandler->
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

  fAtomNamesToUserSubGroupsMap ["no-auto-beaming"] = subGroup;
  fAtomNamesToUserSubGroupsMap ["no-beams"] = subGroup;

  fAtomNamesToUserSubGroupsMap ["delay-rests-beams"] = subGroup; // JMI
}

void xml2lyOahDualHandler::createArticulationsUserGroup (
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
        fUserHandler);
  group->
    setGroupHeaderIsToBeWritten (false);
  fUserHandler->
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

  fAtomNamesToUserSubGroupsMap ["omit-articulations"] = subGroup;

  fAtomNamesToUserSubGroupsMap ["delay-rests-pedals"] = subGroup;
}

void xml2lyOahDualHandler::createOrnamentsUserGroup (
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
        fUserHandler);
  group->
    setGroupHeaderIsToBeWritten (false);
  fUserHandler->
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

  fAtomNamesToUserSubGroupsMap ["omit-ornaments"] = subGroup;

  fAtomNamesToUserSubGroupsMap ["delayed-ornaments-fraction"] = subGroup;
}

void xml2lyOahDualHandler::createGraceNotesUserGroup (
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
        fUserHandler);
  group->
    setGroupHeaderIsToBeWritten (false);
  fUserHandler->
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

  fAtomNamesToUserSubGroupsMap ["slash-all-grace-notes"] = subGroup;
  fAtomNamesToUserSubGroupsMap ["slur-all-grace-notes"] = subGroup;
  fAtomNamesToUserSubGroupsMap ["beam-all-grace-notes"] = subGroup;
}

void xml2lyOahDualHandler::createChordsUserGroup (
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
        fUserHandler);
  group->
    setGroupHeaderIsToBeWritten (false);
  fUserHandler->
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

  fAtomNamesToUserSubGroupsMap ["trace-chords"] = subGroup;
  fAtomNamesToUserSubGroupsMap ["add-words-from-the-chords"] = subGroup;

  fAtomNamesToUserSubGroupsMap ["lpsr-pitches-language"] = subGroup;

  fAtomNamesToUserSubGroupsMap ["lpsr-chords-language"] = subGroup;

  fAtomNamesToUserSubGroupsMap ["chords-display"] = subGroup;
}

void xml2lyOahDualHandler::createTiesUserGroup (
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
        fUserHandler);
  group->
    setGroupHeaderIsToBeWritten (false);
  fUserHandler->
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

  fAtomNamesToUserSubGroupsMap ["omit-ties"] = subGroup;
}

void xml2lyOahDualHandler::createSlursUserGroup (
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
        fUserHandler);
  group->
    setGroupHeaderIsToBeWritten (false);
  fUserHandler->
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

  fAtomNamesToUserSubGroupsMap ["omit-slurs"] = subGroup;

  fAtomNamesToUserSubGroupsMap ["delay-slurs"] = subGroup;
  fAtomNamesToUserSubGroupsMap ["delay-rests-slurs"] = subGroup;
}

void xml2lyOahDualHandler::createLigaturesUserGroup (
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
        fUserHandler);
  group->
    setGroupHeaderIsToBeWritten (false);
  fUserHandler->
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

  fAtomNamesToUserSubGroupsMap ["delay-rests-ligatures"] = subGroup;
}

void xml2lyOahDualHandler::createDynamicsUserGroup (
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
        fUserHandler);
  group->
    setGroupHeaderIsToBeWritten (false);
  fUserHandler->
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

  fAtomNamesToUserSubGroupsMap ["omit-dynamics"] = subGroup;

  fAtomNamesToUserSubGroupsMap ["all-dynamics-below"] = subGroup;

  fAtomNamesToUserSubGroupsMap ["delay-dynamics"] = subGroup;

  fAtomNamesToUserSubGroupsMap ["delay-rests-dynamics"] = subGroup;
}

void xml2lyOahDualHandler::createWedgesUserGroup (
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
        fUserHandler);
  group->
    setGroupHeaderIsToBeWritten (false);
  fUserHandler->
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

  fAtomNamesToUserSubGroupsMap ["omit-wedges"] = subGroup;

  fAtomNamesToUserSubGroupsMap ["all-wedges-below"] = subGroup;

  fAtomNamesToUserSubGroupsMap ["delay-rests-wedges"] = subGroup;
}

void xml2lyOahDualHandler::createTupletsUserGroup (
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
        fUserHandler);
  group->
    setGroupHeaderIsToBeWritten (false);
  fUserHandler->
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

  fAtomNamesToUserSubGroupsMap ["indent-tuplets"] = subGroup;
//  fAtomNamesToUserSubGroupsMap ["add-words-from-the-tuplets"] = subGroup; JMI

  fAtomNamesToUserSubGroupsMap ["lpsr-tuplets-language"] = subGroup;
}

void xml2lyOahDualHandler::createStringsUserGroup (
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
        fUserHandler);
  group->
    setGroupHeaderIsToBeWritten (false);
  fUserHandler->
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

  fAtomNamesToUserSubGroupsMap ["roman-string-numbers"] = subGroup;
  fAtomNamesToUserSubGroupsMap ["avoid-open-strings"] = subGroup;
}

void xml2lyOahDualHandler::createTablaturesUserGroup (
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
        fUserHandler);
  group->
    setGroupHeaderIsToBeWritten (false);
  fUserHandler->
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

  fAtomNamesToUserSubGroupsMap ["tabFullNotation"] = subGroup;
  fAtomNamesToUserSubGroupsMap ["modernTab"] = subGroup;
}

void xml2lyOahDualHandler::createHarmoniesUserGroup (
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
        fUserHandler);
  group->
    setGroupHeaderIsToBeWritten (false);
  fUserHandler->
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

  fAtomNamesToUserSubGroupsMap ["omit-harmonies"] = subGroup;

  fAtomNamesToUserSubGroupsMap ["show-harmony-voices"] = subGroup;
  fAtomNamesToUserSubGroupsMap ["harmonies-display"] = subGroup;
  fAtomNamesToUserSubGroupsMap ["jazz-harmonies-display"] = subGroup;
  fAtomNamesToUserSubGroupsMap ["show-harmonies-structures"] = subGroup;
  fAtomNamesToUserSubGroupsMap ["show-all-harmonies-contents"] = subGroup;
  fAtomNamesToUserSubGroupsMap ["show-harmony-details"] = subGroup;
  fAtomNamesToUserSubGroupsMap ["show-harmony-analysis"] = subGroup;
}

void xml2lyOahDualHandler::createFiguredBassesUserGroup (
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
        fUserHandler);
  group->
    setGroupHeaderIsToBeWritten (false);
  fUserHandler->
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

  fAtomNamesToUserSubGroupsMap ["omit-figured-basses"] = subGroup;

  fAtomNamesToUserSubGroupsMap ["show-figured-bass-voices"] = subGroup;
}

void xml2lyOahDualHandler::createLyricsUserGroup (
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
        fUserHandler);
  group->
    setGroupHeaderIsToBeWritten (false);
  fUserHandler->
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

  fAtomNamesToUserSubGroupsMap ["omit-lyrics"] = subGroup;
  fAtomNamesToUserSubGroupsMap ["add-words-from-the-lyrics"] = subGroup;

  fAtomNamesToUserSubGroupsMap ["no-lilypond-lyrics"] =subGroup;
  fAtomNamesToUserSubGroupsMap ["lyrics-durations"] =subGroup;
  fAtomNamesToUserSubGroupsMap ["add-stanzas-numbers-to-lyrics"] =subGroup;
  fAtomNamesToUserSubGroupsMap ["lyrics-durations"] =subGroup;
}

void xml2lyOahDualHandler::createHeaderUserGroup (
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
        fUserHandler);
  group->
    setGroupHeaderIsToBeWritten (false);
  fUserHandler->
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

  fAtomNamesToUserSubGroupsMap ["rights"] = subGroup;

  fAtomNamesToUserSubGroupsMap ["composer"] = subGroup;
  fAtomNamesToUserSubGroupsMap ["arranger"] = subGroup;
  fAtomNamesToUserSubGroupsMap ["lyricist"] = subGroup;
  fAtomNamesToUserSubGroupsMap ["poet"] = subGroup;

  fAtomNamesToUserSubGroupsMap ["software"] = subGroup;

  fAtomNamesToUserSubGroupsMap ["dedication"] = subGroup;
  fAtomNamesToUserSubGroupsMap ["piece"] = subGroup;
  fAtomNamesToUserSubGroupsMap ["opus"] = subGroup;

  fAtomNamesToUserSubGroupsMap ["title"] = subGroup;
  fAtomNamesToUserSubGroupsMap ["subtitle"] = subGroup;
  fAtomNamesToUserSubGroupsMap ["subsubtitle"] = subGroup;

  fAtomNamesToUserSubGroupsMap ["instrument"] = subGroup;

  fAtomNamesToUserSubGroupsMap ["meter"] = subGroup;

  fAtomNamesToUserSubGroupsMap ["copyright"] = subGroup;

  fAtomNamesToUserSubGroupsMap ["tagline"] = subGroup;

  fAtomNamesToUserSubGroupsMap ["use-lyricists-as-poets"] = subGroup;

  fAtomNamesToUserSubGroupsMap ["use-filename-as-work-title"] = subGroup;
}

void xml2lyOahDualHandler::createPaperUserGroup (
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
        fUserHandler);
  group->
    setGroupHeaderIsToBeWritten (false);
  fUserHandler->
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

  fAtomNamesToUserSubGroupsMap ["paper-height"] = subGroup;
  fAtomNamesToUserSubGroupsMap ["paper-width"] = subGroup;

  fAtomNamesToUserSubGroupsMap ["ragged-bottom"] = subGroup;
  fAtomNamesToUserSubGroupsMap ["ragged-last"] = subGroup;
  fAtomNamesToUserSubGroupsMap ["ragged-last-bottom"] = subGroup;
  fAtomNamesToUserSubGroupsMap ["ragged-right"] = subGroup;

  fAtomNamesToUserSubGroupsMap ["horizontal-shift"] = subGroup;
  fAtomNamesToUserSubGroupsMap ["indent"] = subGroup;
  fAtomNamesToUserSubGroupsMap ["short-indent"] = subGroup;

  fAtomNamesToUserSubGroupsMap ["markup-system-spacing.padding"] = subGroup;

  fAtomNamesToUserSubGroupsMap ["page-count"] = subGroup;
  fAtomNamesToUserSubGroupsMap ["system-count"] = subGroup;
}

void xml2lyOahDualHandler::createLayoutUserGroup (
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
        fUserHandler);
  group->
    setGroupHeaderIsToBeWritten (false);
  fUserHandler->
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

  fAtomNamesToUserSubGroupsMap ["global-staff-size"] = subGroup;
  fAtomNamesToUserSubGroupsMap ["keep-staff-size"] = subGroup;

}

void xml2lyOahDualHandler::createOutputUserGroup (
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
        fUserHandler);
  group->
    setGroupHeaderIsToBeWritten (false);
  fUserHandler->
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

  fAtomNamesToUserSubGroupsMap ["xml2ly-infos"] = subGroup;

  fAtomNamesToUserSubGroupsMap ["no-lilypond-code"] = subGroup;

  fAtomNamesToUserSubGroupsMap ["draft-mode"] = subGroup;
  fAtomNamesToUserSubGroupsMap ["point-and-clic-off"] = subGroup;

  fAtomNamesToUserSubGroupsMap ["lilypond-version"] = subGroup;
  fAtomNamesToUserSubGroupsMap ["lilypond-compile-date"] = subGroup;
  fAtomNamesToUserSubGroupsMap ["lilypond-comments"] = subGroup;

  fAtomNamesToUserSubGroupsMap ["msr2lpsr-score-output-kind"] = subGroup;
  fAtomNamesToUserSubGroupsMap ["lpsr-Languages-language"] = subGroup;

  fAtomNamesToUserSubGroupsMap ["global"] = subGroup;

  fAtomNamesToUserSubGroupsMap ["ambitus"] = subGroup;

  fAtomNamesToUserSubGroupsMap ["input-line-numbers"] = subGroup;
  fAtomNamesToUserSubGroupsMap ["positions-in-measures"] = subGroup;

  fAtomNamesToUserSubGroupsMap ["all-bar-numbers"] = subGroup;
  fAtomNamesToUserSubGroupsMap ["all-measure-numbers"] = subGroup;
  fAtomNamesToUserSubGroupsMap ["generate-box-around-bar-number"] = subGroup;
  fAtomNamesToUserSubGroupsMap ["reset-measure-number"] = subGroup;
  fAtomNamesToUserSubGroupsMap ["original-measure-numbers"] = subGroup;

  fAtomNamesToUserSubGroupsMap ["separator-line-every-n-measures"] = subGroup;

  fAtomNamesToUserSubGroupsMap ["ignore-musicxml-line-breaks"] = subGroup;
  fAtomNamesToUserSubGroupsMap ["ignore-musixcml-page-breaks"] = subGroup;
  fAtomNamesToUserSubGroupsMap ["break-lines-at-incomplete-right-measures"] = subGroup;
  fAtomNamesToUserSubGroupsMap ["custos"] = subGroup;

  fAtomNamesToUserSubGroupsMap ["length-unit"] = subGroup;

  fAtomNamesToUserSubGroupsMap ["all-durations"] = subGroup;

  fAtomNamesToUserSubGroupsMap ["ledger-lines-color"] = subGroup;

  fAtomNamesToUserSubGroupsMap ["jazz-fonts"] = subGroup;

  fAtomNamesToUserSubGroupsMap ["jianpu"] = subGroup;
}

void xml2lyOahDualHandler::createMidiUserGroup (
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
        fUserHandler);
  group->
    setGroupHeaderIsToBeWritten (false);
  fUserHandler->
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

  fAtomNamesToUserSubGroupsMap ["no-midi"] = subGroup;
  fAtomNamesToUserSubGroupsMap ["midi-tempo"] = subGroup;
}

void xml2lyOahDualHandler::checkOptionsAndArguments ()
{
  if (
    // xml2lyInsiderOahHandler?
    S_xml2lyInsiderOahHandler
      theXml2lyInsiderOahHandler =
        dynamic_cast<xml2lyInsiderOahHandler*>(&(*fOahHandlerToBeUsed))
  ) {
    theXml2lyInsiderOahHandler->
      checkOptionsAndArguments ();
  }
  else {
    stringstream s;

    s <<
      "xml2lyInsiderOahHandler expected for checkOptionsAndArguments()";

    oahError (s.str ());
  }
}

void xml2lyOahDualHandler::enforceOahDualHandlerQuietness ()
{
  if (
    // xml2lyInsiderOahHandler?
    S_xml2lyInsiderOahHandler
      theXml2lyInsiderOahHandler =
        dynamic_cast<xml2lyInsiderOahHandler*>(&(*fOahHandlerToBeUsed))
  ) {
    theXml2lyInsiderOahHandler->
      enforceOahHandlerQuietness ();
  }
  else {
    stringstream s;

    s <<
      "xml2lyInsiderOahHandler expected for enforceOahDualHandlerQuietness()";

    oahError (s.str ());
  }
}

string xml2lyOahDualHandler::asString () const
{
  return "xml2lyOahDualHandler";
}

void xml2lyOahDualHandler::print (ostream& os) const
{
  os <<
    "xml2lyOahDualHandler:" <<
    endl;

  gIndenter++;

  os <<
    "??????????? insiderHandler" <<
    endl;

  gIndenter++;

  os << fInsiderHandler << endl;
  /*
  const list<S_oahGroup>
    insiderHandlerGroupsList =
      fInsiderHandler->getHandlerGroupsList ();

  if (insiderHandlerGroupsList.size ()) {
    os << endl;

    list<S_oahGroup>::const_iterator
      iBegin = insiderHandlerGroupsList.begin (),
      iEnd   = insiderHandlerGroupsList.end (),
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
    "??????????? userOahHandler" <<
    endl;

  gIndenter++;

  os << fUserHandler << endl;
  /*
  const list<S_oahGroup>
    userOahHandlerGroupsList =
      fUserHandler->getHandlerGroupsList ();

  if (userOahHandlerGroupsList.size ()) {
    os << endl;

    list<S_oahGroup>::const_iterator
      iBegin = userOahHandlerGroupsList.begin (),
      iEnd   = userOahHandlerGroupsList.end (),
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

ostream& operator<< (ostream& os, const S_xml2lyOahDualHandler& elt)
{
  elt->print (os);
  return os;
}


}
