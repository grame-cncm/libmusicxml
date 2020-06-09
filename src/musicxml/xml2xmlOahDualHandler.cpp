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

#include "xml2xmlOah.h"

#include "xml2xmlOahDualHandler.h"


using namespace std;

namespace MusicXML2
{

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

//______________________________________________________________________________
S_xml2xmlOahDualHandler xml2xmlOahDualHandler::create (
  string   executableName,
  ostream& ios)
{
  xml2xmlOahDualHandler* o = new
    xml2xmlOahDualHandler (
      executableName,
      ios);
  assert(o!=0);
  return o;
}

xml2xmlOahDualHandler::xml2xmlOahDualHandler (
  string   executableName,
  ostream& ios)
{
  fInsiderAtomShortName = "insider";
  fInsiderAtomLongName  = "insider-options";

  initializeXml2xmlOahDualHandler (
    executableName,
    ios);
}

xml2xmlOahDualHandler::~xml2xmlOahDualHandler ()
{}

void xml2xmlOahDualHandler::initializeXml2xmlOahDualHandler (
  string   executableName,
  ostream& ios)
{
  // create the two oahHandlers
  fInsiderOahHandler =
    xml2xmlOahHandler::create (
      executableName,
      ios);

  fUserOahHandler =
    oahHandler::create (
      "xml2xml user handler",
      "xml2xml user handler values",
      "xml2xml user handler help short name",
      "xml2xml user handler header long name",
      "xml2xml user handler summary short name",
      "xml2xml user handler summary long name",
      "xml2xml user handler user handlerPreamble",
      "xml2xml user handler handlerUsage",
      "xml2xml user handler handlerDescription",
      ios);

  // create the user groups
  createInsiderUserGroup (ios);

  createInformationsUserGroup (ios);

  createWarningAndErrorsUserGroup (ios);

  createPresentationUserGroup (ios);

  createFilesUserGroup (ios);

  createPartsUserGroup (ios);
  createStavesUserGroup (ios);
  createVoicesUserGroup (ios);

  createTranspositionsUserGroup (ios);

  createClefsUserGroup (ios);
  createKeysUserGroup (ios);
  createTimesUserGroup (ios);

  createRepeatsUserGroup (ios);

  createMeasuresUserGroup (ios);

  createRestsUserGroup (ios);
  createNotesUserGroup (ios);

  createArticulationsUserGroup (ios);

  createOrnamentsUserGroup (ios);

  createGraceNotesUserGroup (ios);

  createChordsUserGroup (ios);

  createTiesAndSlursUserGroup (ios);

  createDynamicsAndWedgesUserGroup (ios);

  createTupletsUserGroup (ios);

  createLyricsUserGroup (ios);

  createHarmoniesUserGroup (ios);

  createFiguredBassesUserGroup (ios);

  createOutputGenerationUserGroup (ios);

  // populate the user groups from the internal groups
  populateUserGroupsFromInsiderGroups ();
}

void xml2xmlOahDualHandler::createInsiderUserGroup (
  ostream& ios)
{
  // group

  fInsiderUserGroup =
    oahGroup::create (
      "Insider group",
      "insider-group",
      "",
      "",
      kElementVisibilityAlways,
      fUserOahHandler);
  fUserOahHandler->
    appendGroupToHandler (fInsiderUserGroup);
  // this group is hidden in user view
  fInsiderUserGroup->
    setIsHidden ();

  // subgroup, to be populated later

  fInsiderUserSubGroup =
    oahSubGroup::create (
      "Insider",
      "insider",
      "",
      "",
      kElementVisibilityAlways,
      fInsiderUserGroup);
  fInsiderUserGroup->
    appendSubGroupToGroup (fInsiderUserSubGroup);
}

void xml2xmlOahDualHandler::createInformationsUserGroup (
  ostream& ios)
{
  // group

  S_oahGroup
    group =
      oahGroup::create (
        "Informations group",
        "informations-group",
        "",
        "",
        kElementVisibilityAlways,
        fUserOahHandler);
  fUserOahHandler->
    appendGroupToHandler (group);

  // subgroup

  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Informations",
        "informations",
        "",
        "",
        kElementVisibilityAlways,
        group);
  group->
    appendSubGroupToGroup (subGroup);

  // atoms

  fAtomNamesToUserSubGroupsMap ["version"] = subGroup;
  fAtomNamesToUserSubGroupsMap ["about"] = subGroup;
  fAtomNamesToUserSubGroupsMap ["contact"] = subGroup;
  fAtomNamesToUserSubGroupsMap ["help-options"] = subGroup;
  fAtomNamesToUserSubGroupsMap ["option-name-help"] = subGroup;
  fAtomNamesToUserSubGroupsMap ["display-options-values"] = subGroup;
  fAtomNamesToUserSubGroupsMap ["show-options-and-arguments"] = subGroup;
  fAtomNamesToUserSubGroupsMap ["display-cpu-usage"] = subGroup;
}

void xml2xmlOahDualHandler::createWarningAndErrorsUserGroup (
  ostream& ios)
{
  // group

  S_oahGroup
    group =
      oahGroup::create (
        "Warning and errors group",
        "warning-and-errors-group",
        "",
        "",
        kElementVisibilityAlways,
        fUserOahHandler);
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
        kElementVisibilityAlways,
        group);
  group->
    appendSubGroupToGroup (subGroup);

  // atoms

  fAtomNamesToUserSubGroupsMap ["quiet"] = subGroup;
  fAtomNamesToUserSubGroupsMap ["dont-show-errors"] = subGroup;
  fAtomNamesToUserSubGroupsMap ["dont-abort-on-errors"] = subGroup;
  fAtomNamesToUserSubGroupsMap ["display-source-code-position"] = subGroup;
}

void xml2xmlOahDualHandler::createPresentationUserGroup (
  ostream& ios)
{
  // group

  S_oahGroup
    group =
      oahGroup::create (
        "Presentation group",
        "presentation-group",
        "",
        "",
        kElementVisibilityAlways,
        fUserOahHandler);
  fUserOahHandler->
    appendGroupToHandler (group);

  // subgroup

  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Presentation",
        "presentation",
        "",
        "",
        kElementVisibilityAlways,
        group);
  group->
    appendSubGroupToGroup (subGroup);

  // atoms

  fAtomNamesToUserSubGroupsMap ["use-filename-as-work-title"] = subGroup;
  fAtomNamesToUserSubGroupsMap ["use-lyricists-as-poets"] = subGroup;
}

void xml2xmlOahDualHandler::createFilesUserGroup (
  ostream& ios)
{
  // group

  S_oahGroup
    group =
      oahGroup::create (
        "Files group",
        "files-group",
        "",
        "",
        kElementVisibilityAlways,
        fUserOahHandler);
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
        kElementVisibilityAlways,
        group);
  group->
    appendSubGroupToGroup (subGroup);

  // atoms

  fAtomNamesToUserSubGroupsMap ["output-file-name"] = subGroup;
  fAtomNamesToUserSubGroupsMap ["auto-output-file-name"] = subGroup;
  fAtomNamesToUserSubGroupsMap ["loop"] = subGroup;
}

void xml2xmlOahDualHandler::createPartsUserGroup (
  ostream& ios)
{
  // group

  S_oahGroup
    group =
      oahGroup::create (
        "Parts group",
        "parts-group",
        "",
        "",
        kElementVisibilityAlways,
        fUserOahHandler);
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
        kElementVisibilityAlways,
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

void xml2xmlOahDualHandler::createStavesUserGroup (
  ostream& ios)
{
  // group

  S_oahGroup
    group =
      oahGroup::create (
        "Staves group",
        "staves-group",
        "",
        "",
        kElementVisibilityAlways,
        fUserOahHandler);
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
        kElementVisibilityAlways,
        group);
  group->
    appendSubGroupToGroup (subGroup);

  // atoms

  fAtomNamesToUserSubGroupsMap ["create-voices-staff-relative-numbers"] = subGroup;
}

void xml2xmlOahDualHandler::createVoicesUserGroup (
  ostream& ios)
{
  // group

  S_oahGroup
    group =
      oahGroup::create (
        "Voices group",
        "voices-group",
        "",
        "",
        kElementVisibilityAlways,
        fUserOahHandler);
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
        kElementVisibilityAlways,
        group);
  group->
    appendSubGroupToGroup (subGroup);

  // atoms

  fAtomNamesToUserSubGroupsMap ["create-single-line-staves-as-rythmic"] = subGroup;
  fAtomNamesToUserSubGroupsMap ["connect-arpeggios"] = subGroup;
}

void xml2xmlOahDualHandler::createTranspositionsUserGroup (
  ostream& ios)
{
  // group

  S_oahGroup
    group =
      oahGroup::create (
        "Transpositions group",
        "transpositions-group",
        "",
        "",
        kElementVisibilityAlways,
        fUserOahHandler);
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
        kElementVisibilityAlways,
        group);
  group->
    appendSubGroupToGroup (subGroup);

  // atoms

  fAtomNamesToUserSubGroupsMap ["lpsr-transpose"] = subGroup;
  fAtomNamesToUserSubGroupsMap ["lilypond-transpose-part"] = subGroup;
}

void xml2xmlOahDualHandler::createClefsUserGroup (
  ostream& ios)
{
  // group

  S_oahGroup
    group =
      oahGroup::create (
        "Clefs group",
        "clefs-group",
        "",
        "",
        kElementVisibilityAlways,
        fUserOahHandler);
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
        kElementVisibilityAlways,
        group);
  group->
    appendSubGroupToGroup (subGroup);

  // atoms

  fAtomNamesToUserSubGroupsMap ["msr-replace-clef"] = subGroup;
  fAtomNamesToUserSubGroupsMap ["ignore-redundant-clefs"] = subGroup;
  fAtomNamesToUserSubGroupsMap ["comment-clef-changes"] = subGroup;
}

void xml2xmlOahDualHandler::createKeysUserGroup (
  ostream& ios)
{
  // group

  S_oahGroup
    group =
      oahGroup::create (
        "Keys group",
        "keys-group",
        "",
        "",
        kElementVisibilityAlways,
        fUserOahHandler);
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
        kElementVisibilityAlways,
        group);
  group->
    appendSubGroupToGroup (subGroup);

  // atoms

  fAtomNamesToUserSubGroupsMap ["msr-replace-keys"] = subGroup;
  fAtomNamesToUserSubGroupsMap ["ignore-redundant-keys"] = subGroup;
}

void xml2xmlOahDualHandler::createTimesUserGroup (
  ostream& ios)
{
  // group

  S_oahGroup
    group =
      oahGroup::create (
        "Times group",
        "times-group",
        "",
        "",
        kElementVisibilityAlways,
        fUserOahHandler);
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
        kElementVisibilityAlways,
        group);
  group->
    appendSubGroupToGroup (subGroup);

  // atoms

  fAtomNamesToUserSubGroupsMap ["ignore-redundant-times"] = subGroup;
  fAtomNamesToUserSubGroupsMap ["numerical-time"] = subGroup;
}

void xml2xmlOahDualHandler::createRepeatsUserGroup (
  ostream& ios)
{
  // group

  S_oahGroup
    group =
      oahGroup::create (
        "Repeats group",
        "repeats-group",
        "",
        "",
        kElementVisibilityAlways,
        fUserOahHandler);
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
        kElementVisibilityAlways,
        group);
  group->
    appendSubGroupToGroup (subGroup);

  // atoms

  fAtomNamesToUserSubGroupsMap ["create-implicit-initial-repeat-barline"] = subGroup;
  fAtomNamesToUserSubGroupsMap ["keep-repeat-barlines"] = subGroup;
  fAtomNamesToUserSubGroupsMap ["repeat-brackets"] = subGroup;
  fAtomNamesToUserSubGroupsMap ["ignore-repeat-numbers"] = subGroup;
}

void xml2xmlOahDualHandler::createMeasuresUserGroup (
  ostream& ios)
{
  // group

  S_oahGroup
    group =
      oahGroup::create (
        "Measures group",
        "measures-group",
        "",
        "",
        kElementVisibilityAlways,
        fUserOahHandler);
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
        kElementVisibilityAlways,
        group);
  group->
    appendSubGroupToGroup (subGroup);

  // atoms

  fAtomNamesToUserSubGroupsMap ["add-empty-measures"] = subGroup;
  fAtomNamesToUserSubGroupsMap ["replicate-empty-measure"] = subGroup;
}

void xml2xmlOahDualHandler::createRestsUserGroup (
  ostream& ios)
{
  // group

  S_oahGroup
    group =
      oahGroup::create (
        "Rests group",
        "rests-group",
        "",
        "",
        kElementVisibilityAlways,
        fUserOahHandler);
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
        kElementVisibilityAlways,
        group);
  group->
    appendSubGroupToGroup (subGroup);

  // atoms

  fAtomNamesToUserSubGroupsMap ["delay-rests"] = subGroup;
  fAtomNamesToUserSubGroupsMap ["delay-rests-slashes"] = subGroup;
  fAtomNamesToUserSubGroupsMap ["compress-full-measure-rests"] = subGroup;
  fAtomNamesToUserSubGroupsMap ["merges-rests"] = subGroup;
}

void xml2xmlOahDualHandler::createNotesUserGroup (
  ostream& ios)
{
  // group

  S_oahGroup
    group =
      oahGroup::create (
        "Notes group",
        "notes-group",
        "",
        "",
        kElementVisibilityAlways,
        fUserOahHandler);
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
        kElementVisibilityAlways,
        group);
  group->
    appendSubGroupToGroup (subGroup);

  // atoms

  fAtomNamesToUserSubGroupsMap ["msr-pitches-language"] = subGroup;
  fAtomNamesToUserSubGroupsMap ["delay-rests-slashes"] = subGroup;
  fAtomNamesToUserSubGroupsMap ["compress-full-measure-rests"] = subGroup;
  fAtomNamesToUserSubGroupsMap ["merges-rests"] = subGroup;
}

void xml2xmlOahDualHandler::createArticulationsUserGroup (
  ostream& ios)
{
  // group

  S_oahGroup
    group =
      oahGroup::create (
        "Articulations group",
        "articulations-group",
        "",
        "",
        kElementVisibilityAlways,
        fUserOahHandler);
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
        kElementVisibilityAlways,
        group);
  group->
    appendSubGroupToGroup (subGroup);

  // atoms

  fAtomNamesToUserSubGroupsMap ["omit-articulations"] = subGroup;
}

void xml2xmlOahDualHandler::createOrnamentsUserGroup (
  ostream& ios)
{
  // group

  S_oahGroup
    group =
      oahGroup::create (
        "Ornaments group",
        "ornaments-group",
        "",
        "",
        kElementVisibilityAlways,
        fUserOahHandler);
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
        kElementVisibilityAlways,
        group);
  group->
    appendSubGroupToGroup (subGroup);

  // atoms

  fAtomNamesToUserSubGroupsMap ["omit-ornaments"] = subGroup;

  fAtomNamesToUserSubGroupsMap ["delayed-ornaments-fraction"] = subGroup;
}

void xml2xmlOahDualHandler::createGraceNotesUserGroup (
  ostream& ios)
{
  // group

  S_oahGroup
    group =
      oahGroup::create (
        "Grace notes group",
        "grace-notes-group",
        "",
        "",
        kElementVisibilityAlways,
        fUserOahHandler);
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
        kElementVisibilityAlways,
        group);
  group->
    appendSubGroupToGroup (subGroup);

  // atoms

  fAtomNamesToUserSubGroupsMap ["slash-all-grace-notes"] = subGroup;
  fAtomNamesToUserSubGroupsMap ["slur-all-grace-notes"] = subGroup;
  fAtomNamesToUserSubGroupsMap ["beam-all-grace-notes"] = subGroup;
}

void xml2xmlOahDualHandler::createChordsUserGroup (
  ostream& ios)
{
  // group

  S_oahGroup
    group =
      oahGroup::create (
        "Chords group",
        "chords-group",
        "",
        "",
        kElementVisibilityAlways,
        fUserOahHandler);
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
        kElementVisibilityAlways,
        group);
  group->
    appendSubGroupToGroup (subGroup);

  // atoms

  fAtomNamesToUserSubGroupsMap ["trace-chords"] = subGroup;
  fAtomNamesToUserSubGroupsMap ["add-words-from-the-chords"] = subGroup;

  fAtomNamesToUserSubGroupsMap ["lpsr-chords-language"] = subGroup;

  fAtomNamesToUserSubGroupsMap ["lpsr-pitches-language"] = subGroup;
}

void xml2xmlOahDualHandler::createTiesAndSlursUserGroup (
  ostream& ios)
{
  // group

  S_oahGroup
    group =
      oahGroup::create (
        "Ties and slurs group",
        "ties-and-slurs-group",
        "",
        "",
        kElementVisibilityAlways,
        fUserOahHandler);
  fUserOahHandler->
    appendGroupToHandler (group);

  // subgroup

  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Ties and slurs",
        "ties-and-slurs",
        "",
        "",
        kElementVisibilityAlways,
        group);
  group->
    appendSubGroupToGroup (subGroup);

  // atoms

  fAtomNamesToUserSubGroupsMap ["omit-ties"] = subGroup;
  fAtomNamesToUserSubGroupsMap ["omit-slurs"] = subGroup;

  fAtomNamesToUserSubGroupsMap ["delay-slurs"] = subGroup;
}

void xml2xmlOahDualHandler::createDynamicsAndWedgesUserGroup (
  ostream& ios)
{
  // group

  S_oahGroup
    group =
      oahGroup::create (
        "Dynamics and wedges group",
        "dynamics-and-wedges group",
        "",
        "",
        kElementVisibilityAlways,
        fUserOahHandler);
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
        kElementVisibilityAlways,
        group);
  group->
    appendSubGroupToGroup (subGroup);

  // atoms

  fAtomNamesToUserSubGroupsMap ["omit-dynamics"] = subGroup;
  fAtomNamesToUserSubGroupsMap ["omit-wedges"] = subGroup;

  fAtomNamesToUserSubGroupsMap ["all-dynamics-below"] = subGroup;
  fAtomNamesToUserSubGroupsMap ["all-wedges-below"] = subGroup;

  fAtomNamesToUserSubGroupsMap ["delay-dynamics"] = subGroup;
}

void xml2xmlOahDualHandler::createTupletsUserGroup (
  ostream& ios)
{
  // group

  S_oahGroup
    group =
      oahGroup::create (
        "Tuplets group",
        "tuplets-group",
        "",
        "",
        kElementVisibilityAlways,
        fUserOahHandler);
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
        kElementVisibilityAlways,
        group);
  group->
    appendSubGroupToGroup (subGroup);

  // atoms

  fAtomNamesToUserSubGroupsMap ["trace-tuplets"] = subGroup;
  fAtomNamesToUserSubGroupsMap ["add-words-from-the-tuplets"] = subGroup;

  fAtomNamesToUserSubGroupsMap ["lpsr-tuplets-language"] = subGroup;

  fAtomNamesToUserSubGroupsMap ["lpsr-pitches-language"] = subGroup;
}

void xml2xmlOahDualHandler::createHarmoniesUserGroup (
  ostream& ios)
{
  // group

  S_oahGroup
    group =
      oahGroup::create (
        "Harmonies group",
        "harmonies-group",
        "",
        "",
        kElementVisibilityAlways,
        fUserOahHandler);
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
        kElementVisibilityAlways,
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

void xml2xmlOahDualHandler::createFiguredBassesUserGroup (
  ostream& ios)
{
  // group

  S_oahGroup
    group =
      oahGroup::create (
        "Figured basses group",
        "figured-basses-group",
        "",
        "",
        kElementVisibilityAlways,
        fUserOahHandler);
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
        kElementVisibilityAlways,
        group);
  group->
    appendSubGroupToGroup (subGroup);

  // atoms

  fAtomNamesToUserSubGroupsMap ["omit-figured-basses"] = subGroup;

  fAtomNamesToUserSubGroupsMap ["show-figured-bass-voices"] = subGroup;
}

void xml2xmlOahDualHandler::createLyricsUserGroup (
  ostream& ios)
{
  // group

  S_oahGroup
    group =
      oahGroup::create (
        "Lyrics group",
        "lyrics-group",
        "",
        "",
        kElementVisibilityAlways,
        fUserOahHandler);
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
        kElementVisibilityAlways,
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

void xml2xmlOahDualHandler::createOutputGenerationUserGroup (
  ostream& ios)
{
  // group

  S_oahGroup
    group =
      oahGroup::create (
        "Ouptut generation group",
        "ouptut-generation-group",
        "",
        "",
        kElementVisibilityAlways,
        fUserOahHandler);
  fUserOahHandler->
    appendGroupToHandler (group);

  // subgroup

  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Ouptut generation",
        "ouptut-generation",
        "",
        "",
        kElementVisibilityAlways,
        group);
  group->
    appendSubGroupToGroup (subGroup);

  // atoms

  fAtomNamesToUserSubGroupsMap ["msr2lpsr-score-output-kind"] = subGroup;
  fAtomNamesToUserSubGroupsMap ["lpsr-Languages-language"] = subGroup;
}

void xml2xmlOahDualHandler::populateUserGroupsFromInsiderGroups ()
{
  const list<S_oahGroup>
    insiderOahHandlerGroupsList =
      fInsiderOahHandler->getHandlerGroupsList ();

  if (insiderOahHandlerGroupsList.size ()) {
    list<S_oahGroup>::const_iterator
      iBegin = insiderOahHandlerGroupsList.begin (),
      iEnd   = insiderOahHandlerGroupsList.end (),
      i      = iBegin;
    for ( ; ; ) {
      // handle the group
      S_oahGroup group = (*i);

      const list<S_oahSubGroup>
        groupSubGroupsList =
          group->getSubGroupsList ();

      if (groupSubGroupsList.size ()) {
        list<S_oahSubGroup>::const_iterator
          iBegin = groupSubGroupsList.begin (),
          iEnd   = groupSubGroupsList.end (),
          i      = iBegin;
        for ( ; ; ) {
          // handle the subgroup
          S_oahSubGroup subGroup = (*i);

          string subGroupShortName = subGroup->getShortName ();
          string subGroupLongName  = subGroup->getLongName ();

          string
            subGroupNameToUse =
              subGroupLongName.size ()
                ? subGroupLongName
                : subGroupShortName;

         // cout << "subGroupNameToUse = " << subGroupNameToUse << endl;

          // is nameToUse known in fSubGroupNamesToUserGroupsMap?
          map<string, S_oahGroup>::const_iterator
            it =
              fSubGroupNamesToUserGroupsMap.find (
                subGroupNameToUse);

          if (it != fSubGroupNamesToUserGroupsMap.end ()) {
            // subGroupNameToUse is known in the map
            S_oahGroup group = (*it).second;

            // append subgroup to user group
            // cout << "+++ adding subgroup \"" << subGroupNameToUse << "\" to group \"" << group->getGroupHeader () << "\"" << endl;

            group->
              appendSubGroupToGroup (subGroup);

            // remove subGroupNameToUse from the map
            it = fSubGroupNamesToUserGroupsMap.erase (it);
          }
          else {
            // subGroupNameToUse is not known in the map

            // are there atoms from this subgroup in fAtomNamesToUserSubGroupsMap?

            const list<S_oahAtom>&
              subGroupAtomsList =
                subGroup->getAtomsList ();

            if (subGroupAtomsList.size ()) {
              list<S_oahAtom>::const_iterator
                iBegin = subGroupAtomsList.begin (),
                iEnd   = subGroupAtomsList.end (),
                i      = iBegin;
              for ( ; ; ) {
                S_oahAtom atom = (*i);

                // handle the atom

                string atomShortName = atom->getShortName ();
                string atomLongName  = atom->getLongName ();

                string
                  atomNameToUse =
                    atomLongName.size ()
                      ? atomLongName
                      : atomShortName;

                // cout << "atomNameToUse = " << atomNameToUse << endl;

                // is nameToUse known in fSubGroupNamesToUserGroupsMap?
                map<string, S_oahSubGroup>::const_iterator
                  it =
                    fAtomNamesToUserSubGroupsMap.find (
                      atomNameToUse);

                if (it != fAtomNamesToUserSubGroupsMap.end ()) {
                  // atomNameToUse is known in the map
                  S_oahSubGroup subGroup = (*it).second;

                  // append atom to user subgroup
                  // cout << "+++ adding atom \"" << atomNameToUse << "\" to subgroup \"" << subGroup->getSubGroupHeader () << "\"" << endl;

                  subGroup->
                    appendAtomToSubGroup (atom);

                  // remove atomNameToUse from the map
                  it = fAtomNamesToUserSubGroupsMap.erase (it);
                }
                else {
                  // atomNameToUse is not known in the map
                  // place it in the 'insider' user subgroup

                  // cout << "--- adding atom \"" << atomNameToUse << "\" to subgroup \"" << fInsiderUserSubGroup->getSubGroupHeader () << "\"" << endl;

                  fInsiderUserSubGroup->
                    appendAtomToSubGroup (atom);
                }

                if (++i == iEnd) break;
              } // for
            }
          }

          if (++i == iEnd) break;
        } // for
      }

      if (++i == iEnd) break;
    } // for
  }

  // have all the mappings been used?
  int subGroupNamesToUserGroupsMapSize =
    fSubGroupNamesToUserGroupsMap.size ();

  if (subGroupNamesToUserGroupsMapSize) {
    stringstream s;

    s <<
      "The following " <<
      singularOrPlural (
        subGroupNamesToUserGroupsMapSize, "subgroup name", "subgroup names") <<
      " have not been mapped to user groups: ";

    map<string, S_oahGroup>::const_iterator
      iBegin = fSubGroupNamesToUserGroupsMap.begin (),
      iEnd   = fSubGroupNamesToUserGroupsMap.end (),
      i      = iBegin;
    for ( ; ; ) {
      // handle the name
      s <<
         "\"" << (*i).first << "\"";

      if (++i == iEnd) break;
      s << ", ";
    } // for

    cout << s.str () << endl;
//    oahError (s.str ());
  }

  int atomNamesToUserGroupsMapSize =
    fAtomNamesToUserSubGroupsMap.size ();

  if (atomNamesToUserGroupsMapSize) {
    stringstream s;

    s <<
      "The following " <<
      singularOrPlural (
        atomNamesToUserGroupsMapSize, "atom name", "atom names") <<
      " have not been mapped to user subgroups: ";

    map<string, S_oahSubGroup>::const_iterator
      iBegin = fAtomNamesToUserSubGroupsMap.begin (),
      iEnd   = fAtomNamesToUserSubGroupsMap.end (),
      i      = iBegin;
    for ( ; ; ) {
      // handle the name
      s <<
         "\"" << (*i).first << "\"";

      if (++i == iEnd) break;
      s << ", ";
    } // for

    cout << s.str () << endl;
//    oahError (s.str ());
  }
}

oahHandler::oahHelpOptionsHaveBeenUsedKind xml2xmlOahDualHandler::applyOptionsAndArgumentsFromArgcAndArgv (
  int   argc,
  char* argv[])
{
  // should the insider or user oahHandler be used?
  if (
    argc >= 2
      &&
    (
      argv [1] == fInsiderAtomShortName
        ||
      argv [1] == fInsiderAtomLongName
    )
  ) {
    return
      fInsiderOahHandler->
        applyOptionsAndArgumentsFromArgcAndArgv (
          argc,
          argv);
  }
  else {
    return
      fUserOahHandler->
        applyOptionsAndArgumentsFromArgcAndArgv (
          argc,
          argv);
  }
}

oahHandler::oahHelpOptionsHaveBeenUsedKind xml2xmlOahDualHandler::hangleOptionsFromOptionsVector (
  string               fakeExecutableName,
  const optionsVector& theOptionsVector)
{
  // should the insider or user oahHandler be used?
  if (
    theOptionsVector.size () >= 1
      &&
    (
      theOptionsVector [0].first == fInsiderAtomShortName
        ||
      theOptionsVector [0].first == fInsiderAtomLongName
    )
  ) {
    return
      fInsiderOahHandler->
        hangleOptionsFromOptionsVector (
          fakeExecutableName,
          theOptionsVector);
  }
  else {
    return
      fUserOahHandler->
        hangleOptionsFromOptionsVector (
          fakeExecutableName,
          theOptionsVector);
  }
}

void xml2xmlOahDualHandler::enforceOahHandlerQuietness ()
{
  // fInsiderOahHandler->enforceOahHandlerQuietness (); // JMI ???
}

string xml2xmlOahDualHandler::commandLineAsSuppliedAsString () const
{
  return
    fInsiderOahHandler->
      commandLineAsSuppliedAsString ();
}

string xml2xmlOahDualHandler::commandLineWithShortNamesAsString () const
{
  return
    fInsiderOahHandler->
      commandLineWithShortNamesAsString ();
}

string xml2xmlOahDualHandler::commandLineWithLongNamesAsString () const
{
  return
    fInsiderOahHandler->
      commandLineWithLongNamesAsString ();
}

void xml2xmlOahDualHandler::print (ostream& os) const
{
  os <<
    "xml2xmlOahDualHandler:" <<
    endl;

  gIndenter++;

  os <<
    "insiderOahHandler" <<
    endl;

  gIndenter++;

  const list<S_oahGroup>
    insiderOahHandlerGroupsList =
      fInsiderOahHandler->getHandlerGroupsList ();

  if (insiderOahHandlerGroupsList.size ()) {
    os << endl;

    list<S_oahGroup>::const_iterator
      iBegin = insiderOahHandlerGroupsList.begin (),
      iEnd   = insiderOahHandlerGroupsList.end (),
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

void xml2xmlOahDualHandler::printHelp (ostream& os) const
{
  os <<
    "------------ " <<
    "xml2xmlOahDualHandler:" <<
    " ------------" <<
    endl;

  gIndenter++;
if (false) { // JMI
  os <<
    endl <<
    "------------ " <<
    "insiderOahHandler" <<
     " ------------" <<
   endl;

  gIndenter++;

  const list<S_oahGroup>
    insiderOahHandlerGroupsList =
      fInsiderOahHandler->getHandlerGroupsList ();

  if (insiderOahHandlerGroupsList.size ()) {
    os << endl;

    list<S_oahGroup>::const_iterator
      iBegin = insiderOahHandlerGroupsList.begin (),
      iEnd   = insiderOahHandlerGroupsList.end (),
      i      = iBegin;
    for ( ; ; ) {
      // print the group help
      (*i)->printHelp (os);
      if (++i == iEnd) break;
      os << endl;
    } // for
  }

  gIndenter--;
}
  os <<
    endl <<
    "------------ " <<
    "userOahHandler" <<
    " ------------" <<
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
      // print the group help
      (*i)->printHelp (os);
      if (++i == iEnd) break;
      os << endl;
    } // for
  }

  gIndenter--;

  gIndenter--;

  os << endl;
}

ostream& operator<< (ostream& os, const S_xml2xmlOahDualHandler& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_oahDualHandlerViewKindAtom oahDualHandlerViewKindAtom::create (
  string             shortName,
  string             longName,
  string             description,
  string             valueSpecification,
  string             variableName,
  oahDualHandlerViewKind&
                     oahDualHandlerViewKindVariable)
{
  oahDualHandlerViewKindAtom* o = new
    oahDualHandlerViewKindAtom (
      shortName,
      longName,
      description,
      valueSpecification,
      variableName,
      oahDualHandlerViewKindVariable);
  assert(o!=0);
  return o;
}

oahDualHandlerViewKindAtom::oahDualHandlerViewKindAtom (
  string             shortName,
  string             longName,
  string             description,
  string             valueSpecification,
  string             variableName,
  oahDualHandlerViewKind&
                     oahDualHandlerViewKindVariable)
  : oahValuedAtom (
      shortName,
      longName,
      description,
      valueSpecification,
      variableName),
    foahDualHandlerViewKindVariable (
      oahDualHandlerViewKindVariable)
{}

oahDualHandlerViewKindAtom::~oahDualHandlerViewKindAtom ()
{}

S_oahValuedAtom oahDualHandlerViewKindAtom::handleOptionUnderName (
  string   optionName,
  ostream& os)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceOah) {
    gLogOstream <<
      "==> option '" << optionName << "' is a oahDualHandlerViewKindAtom" <<
      endl;
  }
#endif

  // an option value is needed
  return this;
}

void oahDualHandlerViewKindAtom::handleValue (
  string   theString,
  ostream& os)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceOah) {
    os <<
      "==> oahAtom is of type 'oahDualHandlerViewKindAtom'" <<
      endl;
  }
#endif

  // theString contains the language name:
  // is it in the optional values style kinds map?

#ifdef TRACE_OAH
  if (gTraceOah->fTraceOah) {
    os <<
      "==> oahAtom is of type 'oahDualHandlerViewKindAtom'" <<
      endl;
  }
#endif

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
}

void oahDualHandlerViewKindAtom::acceptIn (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    gLogOstream <<
      "% ==> oahDualHandlerViewKindAtom::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_oahDualHandlerViewKindAtom>*
    p =
      dynamic_cast<visitor<S_oahDualHandlerViewKindAtom>*> (v)) {
        S_oahDualHandlerViewKindAtom elem = this;

#ifdef TRACE_OAH
        if (gOahOah->fTraceOahVisitors) {
          gLogOstream <<
            "% ==> Launching oahDualHandlerViewKindAtom::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void oahDualHandlerViewKindAtom::acceptOut (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    gLogOstream <<
      "% ==> oahDualHandlerViewKindAtom::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_oahDualHandlerViewKindAtom>*
    p =
      dynamic_cast<visitor<S_oahDualHandlerViewKindAtom>*> (v)) {
        S_oahDualHandlerViewKindAtom elem = this;

#ifdef TRACE_OAH
        if (gOahOah->fTraceOahVisitors) {
          gLogOstream <<
            "% ==> Launching oahDualHandlerViewKindAtom::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void oahDualHandlerViewKindAtom::browseData (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    gLogOstream <<
      "% ==> oahDualHandlerViewKindAtom::browseData ()" <<
      endl;
  }
#endif
}

string oahDualHandlerViewKindAtom::asShortNamedOptionString () const
{
  stringstream s;

  s <<
    "-" << fShortName << " " <<
    oahDualHandlerViewKindAsString (foahDualHandlerViewKindVariable);

  return s.str ();
}

string oahDualHandlerViewKindAtom::asActualLongNamedOptionString () const
{
  stringstream s;

  s <<
    "-" << fLongName << " " <<
    oahDualHandlerViewKindAsString (foahDualHandlerViewKindVariable);

  return s.str ();
}

void oahDualHandlerViewKindAtom::print (ostream& os) const
{
  const int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "OptionsOptionalValuesStyleKindAtom:" <<
    endl;

  gIndenter++;

  printValuedAtomEssentials (
    os, fieldWidth);

  os << left <<
    setw (fieldWidth) <<
    "fVariableName" << " : " <<
    fVariableName <<
    endl <<
    setw (fieldWidth) <<
    "foahDualHandlerViewKindVariable" << " : \"" <<
    oahDualHandlerViewKindAsString (
      foahDualHandlerViewKindVariable) <<
    "\"" <<
    endl;

  gIndenter--;
}

ostream& operator<< (ostream& os, const S_oahDualHandlerViewKindAtom& elt)
{
  elt->print (os);
  return os;
}


}
