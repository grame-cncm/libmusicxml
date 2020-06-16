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

#include "messagesHandling.h"

#include "xml2lyInsiderOah.h"

#include "xml2lyOahDualHandler.h"


using namespace std;

namespace MusicXML2
{
/*
//______________________________________________________________________________
string oahDualHandlerViewKindAsString (
  oahDualHandlerViewKind dualHandlerViewKind)
{
  string result;

  switch (dualHandlerViewKind) {
    case kViewInsider:
      return "kViewInsider";
      break;
    case kViewUser:
      return "kViewUser";
      break;
  }

  return result;
}
*/
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
  return o;
}

xml2lyOahDualHandler::xml2lyOahDualHandler (
  string   executableName,
  ostream& os)
  : oahDualHandler (
      executableName,
      "insider",
      "insider-group",
      os)
{
  initializaOahDualHandler (os);
}

xml2lyOahDualHandler::~xml2lyOahDualHandler ()
{}

void xml2lyOahDualHandler::createInsiderHandler (
  ostream& os)
{
// JMI  os << "createInsiderHandler(), fExecutableName = " << fExecutableName << endl;

  fInsiderHandler =
    xml2lyInsiderOahHandler::create (
      fExecutableName,
      os);
}

void xml2lyOahDualHandler::createUserHandler (
  ostream& os)
{
// JMI  os << "createUserHandler(), fExecutableName = " << fExecutableName << endl;

/* JMI
  fUserOahHandler =
    oahHandler::create (
      "xml2ly user handler",
      "xml2ly user handler values",
      "xml2ly user handler help short name",
      "xml2ly user handler header long name",
      "xml2ly user handler summary short name",
      "xml2ly user handler summary long name",
      "xml2ly user handler user handlerPreamble",
      "xml2ly user handler handlerUsage",
      "xml2ly user handler handlerDescription",
      os);
      */

  fUserOahHandler =
    fInsiderHandler->
      createHandlerNewbornCloneWithoutGroups ();
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

  createDynamicsAndWedgesUserGroup (os);

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
  fUserOahHandler->
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
        "options-and-help-group",
        "",
        "",
        kElementVisibilityWhole,
        fUserOahHandler);
  group->
    setGroupHeaderIsToBeWritten (false);
  fUserOahHandler->
    appendGroupToHandler (group);

  // subgroup

  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Options and help",
        "oah",
        "options-and-help",
        "",
        kElementVisibilityWhole,
        group);
  group->
    appendSubGroupToGroup (subGroup);

  // atoms

  fAtomNamesToUserSubGroupsMap ["option-name-help"] = subGroup;
  fAtomNamesToUserSubGroupsMap ["display-options-values"] = subGroup;
  fAtomNamesToUserSubGroupsMap ["show-options-and-arguments"] = subGroup;
  fAtomNamesToUserSubGroupsMap ["insider"] = subGroup;
}

void xml2lyOahDualHandler::createInformationsUserGroup (
  ostream& os)
{
  // group

  S_oahGroup
    group =
      oahGroup::create (
        "Informations group",
        "informations-group",
        "",
        "",
        kElementVisibilityWhole,
        fUserOahHandler);
  group->
    setGroupHeaderIsToBeWritten (false);
  fUserOahHandler->
    appendGroupToHandler (group);
//  fUserOahHandler-> // JMI
//    registerElementInHandler (group);

  // subgroup

  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Informations",
        "informations",
        "",
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
        "warning-and-errors-group",
        "",
        "",
        kElementVisibilityWhole,
        fUserOahHandler);
  group->
    setGroupHeaderIsToBeWritten (false);
  fUserOahHandler->
    appendGroupToHandler (group);

  // subgroup

  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Warning and errors",
        "warning-and-errors",
        "",
        "",
        kElementVisibilityWhole,
        group);
  group->
    appendSubGroupToGroup (subGroup);

  // atoms

  fAtomNamesToUserSubGroupsMap ["quiet"] = subGroup;
  fAtomNamesToUserSubGroupsMap ["dont-show-errors"] = subGroup;
  fAtomNamesToUserSubGroupsMap ["dont-abort-on-errors"] = subGroup;
  fAtomNamesToUserSubGroupsMap ["display-source-code-position"] = subGroup;
}

void xml2lyOahDualHandler::createInputUserGroup (
  ostream& os)
{
  // group

  S_oahGroup
    group =
      oahGroup::create (
        "Input group",
        "input-group",
        "",
        "",
        kElementVisibilityWhole,
        fUserOahHandler);
  group->
    setGroupHeaderIsToBeWritten (false);
  fUserOahHandler->
    appendGroupToHandler (group);
//  fUserOahHandler-> // JMI
//    registerElementInHandler (group);

  // subgroup

  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Input",
        "input",
        "",
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
        "files-group",
        "",
        "",
        kElementVisibilityWhole,
        fUserOahHandler);
  group->
    setGroupHeaderIsToBeWritten (false);
  fUserOahHandler->
    appendGroupToHandler (group);

  // subgroup

  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Files",
        "files",
        "",
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
        "parts-group",
        "",
        "",
        kElementVisibilityWhole,
        fUserOahHandler);
  group->
    setGroupHeaderIsToBeWritten (false);
  fUserOahHandler->
    appendGroupToHandler (group);

  // subgroup

  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Parts",
        "parts",
        "",
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
        "staves-group",
        "",
        "",
        kElementVisibilityWhole,
        fUserOahHandler);
  group->
    setGroupHeaderIsToBeWritten (false);
  fUserOahHandler->
    appendGroupToHandler (group);

  // subgroup

  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Staves",
        "staves",
        "",
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
        "voices-group",
        "",
        "",
        kElementVisibilityWhole,
        fUserOahHandler);
  group->
    setGroupHeaderIsToBeWritten (false);
  fUserOahHandler->
    appendGroupToHandler (group);

  // subgroup

  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Voices",
        "voices",
        "",
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
        "transpositions-group",
        "",
        "",
        kElementVisibilityWhole,
        fUserOahHandler);
  group->
    setGroupHeaderIsToBeWritten (false);
  fUserOahHandler->
    appendGroupToHandler (group);

  // subgroup

  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Transpositions",
        "transpositions",
        "",
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
        "clefs-group",
        "",
        "",
        kElementVisibilityWhole,
        fUserOahHandler);
  group->
    setGroupHeaderIsToBeWritten (false);
  fUserOahHandler->
    appendGroupToHandler (group);

  // subgroup

  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Clefs",
        "clefs",
        "",
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
        "keys-group",
        "",
        "",
        kElementVisibilityWhole,
        fUserOahHandler);
  group->
    setGroupHeaderIsToBeWritten (false);
  fUserOahHandler->
    appendGroupToHandler (group);

  // subgroup

  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Keys",
        "keys",
        "",
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
        "times-group",
        "",
        "",
        kElementVisibilityWhole,
        fUserOahHandler);
  group->
    setGroupHeaderIsToBeWritten (false);
  fUserOahHandler->
    appendGroupToHandler (group);

  // subgroup

  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Times",
        "times",
        "",
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
        "repeats-group",
        "",
        "",
        kElementVisibilityWhole,
        fUserOahHandler);
  group->
    setGroupHeaderIsToBeWritten (false);
  fUserOahHandler->
    appendGroupToHandler (group);

  // subgroup

  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Repeats",
        "repeats",
        "",
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
        "measures-group",
        "",
        "",
        kElementVisibilityWhole,
        fUserOahHandler);
  group->
    setGroupHeaderIsToBeWritten (false);
  fUserOahHandler->
    appendGroupToHandler (group);

  // subgroup

  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Measures",
        "measures",
        "",
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
        "rests-group",
        "",
        "",
        kElementVisibilityWhole,
        fUserOahHandler);
  group->
    setGroupHeaderIsToBeWritten (false);
  fUserOahHandler->
    appendGroupToHandler (group);

  // subgroup

  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Rests",
        "rests",
        "",
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
        "notes-group",
        "",
        "",
        kElementVisibilityWhole,
        fUserOahHandler);
  group->
    setGroupHeaderIsToBeWritten (false);
  fUserOahHandler->
    appendGroupToHandler (group);

  // subgroup

  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Notes",
        "notes",
        "",
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
        "beams-group",
        "",
        "",
        kElementVisibilityWhole,
        fUserOahHandler);
  group->
    setGroupHeaderIsToBeWritten (false);
  fUserOahHandler->
    appendGroupToHandler (group);

  // subgroup

  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Beams",
        "beams",
        "",
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
        "articulations-group",
        "",
        "",
        kElementVisibilityWhole,
        fUserOahHandler);
  group->
    setGroupHeaderIsToBeWritten (false);
  fUserOahHandler->
    appendGroupToHandler (group);

  // subgroup

  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Articulations",
        "articulations",
        "",
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
        "ornaments-group",
        "",
        "",
        kElementVisibilityWhole,
        fUserOahHandler);
  group->
    setGroupHeaderIsToBeWritten (false);
  fUserOahHandler->
    appendGroupToHandler (group);

  // subgroup

  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Ornaments",
        "ornaments",
        "",
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
        "grace-notes-group",
        "",
        "",
        kElementVisibilityWhole,
        fUserOahHandler);
  group->
    setGroupHeaderIsToBeWritten (false);
  fUserOahHandler->
    appendGroupToHandler (group);

  // subgroup

  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Grace notes",
        "grace-notes",
        "",
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
        "chords-group",
        "",
        "",
        kElementVisibilityWhole,
        fUserOahHandler);
  group->
    setGroupHeaderIsToBeWritten (false);
  fUserOahHandler->
    appendGroupToHandler (group);

  // subgroup

  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Chords",
        "chords",
        "",
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
        "ties-group",
        "",
        "",
        kElementVisibilityWhole,
        fUserOahHandler);
  group->
    setGroupHeaderIsToBeWritten (false);
  fUserOahHandler->
    appendGroupToHandler (group);

  // subgroup

  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Ties",
        "ties",
        "",
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
        "slurs-group",
        "",
        "",
        kElementVisibilityWhole,
        fUserOahHandler);
  group->
    setGroupHeaderIsToBeWritten (false);
  fUserOahHandler->
    appendGroupToHandler (group);

  // subgroup

  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Slurs",
        "slurs",
        "",
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
        "ligatures-group",
        "",
        "",
        kElementVisibilityWhole,
        fUserOahHandler);
  group->
    setGroupHeaderIsToBeWritten (false);
  fUserOahHandler->
    appendGroupToHandler (group);

  // subgroup

  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Ligatures",
        "ligatures",
        "",
        "",
        kElementVisibilityWhole,
        group);
  group->
    appendSubGroupToGroup (subGroup);

  // atoms

  fAtomNamesToUserSubGroupsMap ["delay-rests-ligatures"] = subGroup;
}

void xml2lyOahDualHandler::createDynamicsAndWedgesUserGroup (
  ostream& os)
{
  // group

  S_oahGroup
    group =
      oahGroup::create (
        "Dynamics and wedges group",
        "dynamics-and-wedges group",
        "",
        "",
        kElementVisibilityWhole,
        fUserOahHandler);
  group->
    setGroupHeaderIsToBeWritten (false);
  fUserOahHandler->
    appendGroupToHandler (group);

  // subgroup

  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Dynamics and wedges",
        "dynamics-and-wedges",
        "",
        "",
        kElementVisibilityWhole,
        group);
  group->
    appendSubGroupToGroup (subGroup);

  // atoms

  fAtomNamesToUserSubGroupsMap ["omit-dynamics"] = subGroup;
  fAtomNamesToUserSubGroupsMap ["omit-wedges"] = subGroup;

  fAtomNamesToUserSubGroupsMap ["all-dynamics-below"] = subGroup;
  fAtomNamesToUserSubGroupsMap ["all-wedges-below"] = subGroup;

  fAtomNamesToUserSubGroupsMap ["delay-dynamics"] = subGroup;

  fAtomNamesToUserSubGroupsMap ["delay-rests-dynamics"] = subGroup;
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
        "tuplets-group",
        "",
        "",
        kElementVisibilityWhole,
        fUserOahHandler);
  group->
    setGroupHeaderIsToBeWritten (false);
  fUserOahHandler->
    appendGroupToHandler (group);

  // subgroup

  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Tuplets",
        "tuplets",
        "",
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
        "strings-group",
        "",
        "",
        kElementVisibilityWhole,
        fUserOahHandler);
  group->
    setGroupHeaderIsToBeWritten (false);
  fUserOahHandler->
    appendGroupToHandler (group);

  // subgroup

  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Strings",
        "strings",
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
        "tablatures-group",
        "",
        "",
        kElementVisibilityWhole,
        fUserOahHandler);
  group->
    setGroupHeaderIsToBeWritten (false);
  fUserOahHandler->
    appendGroupToHandler (group);

  // subgroup

  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Tablatures",
        "tablatures",
        "",
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
        "harmonies-group",
        "",
        "",
        kElementVisibilityWhole,
        fUserOahHandler);
  group->
    setGroupHeaderIsToBeWritten (false);
  fUserOahHandler->
    appendGroupToHandler (group);

  // subgroup

  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Harmonies",
        "harmonies",
        "",
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
        "figured-basses-group",
        "",
        "",
        kElementVisibilityWhole,
        fUserOahHandler);
  group->
    setGroupHeaderIsToBeWritten (false);
  fUserOahHandler->
    appendGroupToHandler (group);

  // subgroup

  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Figured basses",
        "figured-basses",
        "",
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
        "lyrics-group",
        "",
        "",
        kElementVisibilityWhole,
        fUserOahHandler);
  group->
    setGroupHeaderIsToBeWritten (false);
  fUserOahHandler->
    appendGroupToHandler (group);

  // subgroup

  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Lyrics",
        "lyrics",
        "",
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
        "header-group",
        "",
        "",
        kElementVisibilityWhole,
        fUserOahHandler);
  group->
    setGroupHeaderIsToBeWritten (false);
  fUserOahHandler->
    appendGroupToHandler (group);

  // subgroup

  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Header",
        "header",
        "",
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
        "paper-group",
        "",
        "",
        kElementVisibilityWhole,
        fUserOahHandler);
  group->
    setGroupHeaderIsToBeWritten (false);
  fUserOahHandler->
    appendGroupToHandler (group);

  // subgroup

  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Paper",
        "paper",
        "",
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
        "layout-group",
        "",
        "",
        kElementVisibilityWhole,
        fUserOahHandler);
  group->
    setGroupHeaderIsToBeWritten (false);
  fUserOahHandler->
    appendGroupToHandler (group);

  // subgroup

  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Layout",
        "layout",
        "",
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
        "ouptut-group",
        "",
        "",
        kElementVisibilityWhole,
        fUserOahHandler);
  group->
    setGroupHeaderIsToBeWritten (false);
  fUserOahHandler->
    appendGroupToHandler (group);

  // subgroup

  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Ouptut",
        "ouptut",
        "",
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
        "midi-group",
        "",
        "",
        kElementVisibilityWhole,
        fUserOahHandler);
  group->
    setGroupHeaderIsToBeWritten (false);
  fUserOahHandler->
    appendGroupToHandler (group);

  // subgroup

  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "MIDI",
        "midi",
        "",
        "",
        kElementVisibilityWhole,
        group);
  group->
    appendSubGroupToGroup (subGroup);

  // atoms

  fAtomNamesToUserSubGroupsMap ["no-midi"] = subGroup;
  fAtomNamesToUserSubGroupsMap ["midi-tempo"] = subGroup;
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
    "insiderHandler" <<
    endl;

  gIndenter++;

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

  gIndenter--;

  os <<
    "userOahHandler" <<
    endl;

  gIndenter++;

  const list<S_oahGroup>
    userOahHandlerGroupsList =
      fUserOahHandler->getHandlerGroupsList ();

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

  gIndenter--;

  gIndenter--;

  os << endl;
}

ostream& operator<< (ostream& os, const S_xml2lyOahDualHandler& elt)
{
  elt->print (os);
  return os;
}

/*
string xml2lyDualHandlerInsiderAtom::asShortNamedOptionString () const
{
  stringstream s;

  s <<
    "-" << fShortName << " " <<
    S_xml2lyOahDualHandlerAsString (fXml2lyOahDualHandlerVariable);

  return s.str ();
}

string xml2lyDualHandlerInsiderAtom::asActualLongNamedOptionString () const
{
  stringstream s;

  s <<
    "-" << fLongName << " " <<
    S_xml2lyOahDualHandlerAsString (fXml2lyOahDualHandlerVariable);

  return s.str ();
}
*/


}


/*
  map<string, oahDualHandlerViewKind>::const_iterator
    it =
      goahDualHandlerViewKindsMap.find (
        theString);

  if (it == goahDualHandlerViewKindsMap.end ()) {
    // no, optional values style kind is unknown in the map
    stringstream s;

    s <<
      "OAH optional values style kind '" << theString <<
      "' is unknown" <<
      endl <<
      "The " <<
      goahDualHandlerViewKindsMap.size () - 1 <<
      " known OAH optional values style kind are:" <<
      endl;

    gIndenter++;

    s <<
      existingoahDualHandlerViewKinds (K_NAMES_LIST_MAX_LENGTH);

    gIndenter--;

    oahError (s.str ());
  }

  setoahDualHandlerViewKindVariable (
    (*it).second);
    */
