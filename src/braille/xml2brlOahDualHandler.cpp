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

#include "xml2brlInsiderOah.h"

#include "xml2brlOahDualHandler.h"


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
S_xml2brlOahDualHandler xml2brlOahDualHandler::create (
  string   executableName,
  ostream& os)
{
  xml2brlOahDualHandler* o = new
    xml2brlOahDualHandler (
      executableName,
      os);
  assert(o!=0);

  o->createTheTwoHandlers (os);

  return o;
}

xml2brlOahDualHandler::xml2brlOahDualHandler (
  string   executableName,
  ostream& os)
  : oahDualHandler (
      "xml2brlOahDualHandler",
      executableName,
      "insider",
      "insider-group",
      os)
{}

xml2brlOahDualHandler::~xml2brlOahDualHandler ()
{}

void xml2brlOahDualHandler::createTheTwoHandlers (
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
  fXml2brlInsiderHandler =
    xml2brlInsiderOahHandler::create (
      fExecutableName,
      fDualHandlerName,
      os);

  // propagate it into the base class
  setInsiderHandler (fXml2brlInsiderHandler);

  // create the 'user' handler
// JMI  os << "createUserHandler(), fExecutableName = " << fExecutableName << endl;
  fXml2brlUserHandler =
    fXml2brlInsiderHandler->
      createHandlerNewbornCloneWithoutGroups ();

  // propagate it into the base class
  setUserHandler (fXml2brlUserHandler);

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

void xml2brlOahDualHandler::createUserHandlerGroups (
  ostream& os)
{
// JMI  os << "createUserHandlerGroups()" << endl;

  createInsiderUserGroup (os);

  createFilesUserGroup (os);

  createInformationsUserGroup (os);

  createWarningAndErrorsUserGroup (os);

  createPresentationUserGroup (os);

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

  createArticulationsUserGroup (os);

  createOrnamentsUserGroup (os);

  createGraceNotesUserGroup (os);

  createChordsUserGroup (os);

  createTiesAndSlursUserGroup (os);

  createDynamicsAndWedgesUserGroup (os);

  createTupletsUserGroup (os);

  createLyricsUserGroup (os);

  createHarmoniesUserGroup (os);

  createFiguredBassesUserGroup (os);

  createOutputUserGroup (os);
}

void xml2brlOahDualHandler::createInsiderUserGroup (
  ostream& os)
{
  // group

  S_oahGroup
    group =
      oahGroup::create (
        "Insider group",
        "insider-group",
        "",
        "",
        kElementVisibilityWhole,
        fUserHandler);
  group->
    setGroupHeaderIsToBeWritten (false);
  fUserHandler->
    appendGroupToHandler (group);

  // this group is hidden in user view JMI
//  group->
//    setIsHidden ();

  // subgroup

  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Insider",
        "insider",
        "",
        "",
        kElementVisibilityWhole,
        group);
  group->
    appendSubGroupToGroup (subGroup);
}

void xml2brlOahDualHandler::createInformationsUserGroup (
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
        fUserHandler);
  fUserHandler->
    appendGroupToHandler (group);

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
  fAtomNamesToUserSubGroupsMap ["help-options"] = subGroup;
  fAtomNamesToUserSubGroupsMap ["option-name-help"] = subGroup;
  fAtomNamesToUserSubGroupsMap ["display-options-values"] = subGroup;
  fAtomNamesToUserSubGroupsMap ["show-options-and-arguments"] = subGroup;
  fAtomNamesToUserSubGroupsMap ["display-cpu-usage"] = subGroup;
}

void xml2brlOahDualHandler::createWarningAndErrorsUserGroup (
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
        fUserHandler);
  fUserHandler->
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
  fAtomNamesToUserSubGroupsMap ["dont-exit-on-errors"] = subGroup;
  fAtomNamesToUserSubGroupsMap ["display-source-code-position"] = subGroup;
}

void xml2brlOahDualHandler::createPresentationUserGroup (
  ostream& os)
{
  // group

  S_oahGroup
    group =
      oahGroup::create (
        "Presentation group",
        "presentation-group",
        "",
        "",
        kElementVisibilityWhole,
        fUserHandler);
  fUserHandler->
    appendGroupToHandler (group);

  // subgroup

  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Presentation",
        "presentation",
        "",
        "",
        kElementVisibilityWhole,
        group);
  group->
    appendSubGroupToGroup (subGroup);

  // atoms

  fAtomNamesToUserSubGroupsMap ["use-filename-as-work-title"] = subGroup;
  fAtomNamesToUserSubGroupsMap ["use-lyricists-as-poets"] = subGroup;
}

void xml2brlOahDualHandler::createFilesUserGroup (
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
        fUserHandler);
  fUserHandler->
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
}

void xml2brlOahDualHandler::createPartsUserGroup (
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
        fUserHandler);
  fUserHandler->
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

void xml2brlOahDualHandler::createStavesUserGroup (
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
        fUserHandler);
  fUserHandler->
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

void xml2brlOahDualHandler::createVoicesUserGroup (
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
        fUserHandler);
  fUserHandler->
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

void xml2brlOahDualHandler::createTranspositionsUserGroup (
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
        fUserHandler);
  fUserHandler->
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

void xml2brlOahDualHandler::createClefsUserGroup (
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
        fUserHandler);
  fUserHandler->
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

void xml2brlOahDualHandler::createKeysUserGroup (
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
        fUserHandler);
  fUserHandler->
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

  fAtomNamesToUserSubGroupsMap ["msr-replace-keys"] = subGroup;
  fAtomNamesToUserSubGroupsMap ["ignore-redundant-keys"] = subGroup;
}

void xml2brlOahDualHandler::createTimesUserGroup (
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
        fUserHandler);
  fUserHandler->
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

void xml2brlOahDualHandler::createRepeatsUserGroup (
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
        fUserHandler);
  fUserHandler->
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
//  fAtomNamesToUserSubGroupsMap ["keep-repeat-barlines"] = subGroup;
  fAtomNamesToUserSubGroupsMap ["repeat-brackets"] = subGroup;
  fAtomNamesToUserSubGroupsMap ["ignore-repeat-numbers"] = subGroup;
}

void xml2brlOahDualHandler::createMeasuresUserGroup (
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
        fUserHandler);
  fUserHandler->
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

void xml2brlOahDualHandler::createRestsUserGroup (
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
        fUserHandler);
  fUserHandler->
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
  fAtomNamesToUserSubGroupsMap ["merges-rests"] = subGroup;
}

void xml2brlOahDualHandler::createNotesUserGroup (
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
        fUserHandler);
  fUserHandler->
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
  fAtomNamesToUserSubGroupsMap ["delay-rests-slashes"] = subGroup;
  fAtomNamesToUserSubGroupsMap ["compress-full-measure-rests"] = subGroup;
  fAtomNamesToUserSubGroupsMap ["merges-rests"] = subGroup;
}

void xml2brlOahDualHandler::createArticulationsUserGroup (
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
        fUserHandler);
  fUserHandler->
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
}

void xml2brlOahDualHandler::createOrnamentsUserGroup (
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
        fUserHandler);
  fUserHandler->
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

void xml2brlOahDualHandler::createGraceNotesUserGroup (
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
        fUserHandler);
  fUserHandler->
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

void xml2brlOahDualHandler::createChordsUserGroup (
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
        fUserHandler);
  fUserHandler->
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

  fAtomNamesToUserSubGroupsMap ["lpsr-chords-language"] = subGroup;

  fAtomNamesToUserSubGroupsMap ["lpsr-pitches-language"] = subGroup;
}

void xml2brlOahDualHandler::createTiesAndSlursUserGroup (
  ostream& os)
{
  // group

  S_oahGroup
    group =
      oahGroup::create (
        "Ties and slurs group",
        "ties-and-slurs-group",
        "",
        "",
        kElementVisibilityWhole,
        fUserHandler);
  fUserHandler->
    appendGroupToHandler (group);

  // subgroup

  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Ties and slurs",
        "ties-and-slurs",
        "",
        "",
        kElementVisibilityWhole,
        group);
  group->
    appendSubGroupToGroup (subGroup);

  // atoms

  fAtomNamesToUserSubGroupsMap ["omit-ties"] = subGroup;
  fAtomNamesToUserSubGroupsMap ["omit-slurs"] = subGroup;

  fAtomNamesToUserSubGroupsMap ["delay-slurs"] = subGroup;
}

void xml2brlOahDualHandler::createDynamicsAndWedgesUserGroup (
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
        fUserHandler);
  fUserHandler->
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
}

void xml2brlOahDualHandler::createTupletsUserGroup (
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
        fUserHandler);
  fUserHandler->
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

  fAtomNamesToUserSubGroupsMap ["trace-tuplets"] = subGroup;
//  fAtomNamesToUserSubGroupsMap ["add-words-from-the-tuplets"] = subGroup;

  fAtomNamesToUserSubGroupsMap ["lpsr-tuplets-language"] = subGroup;
}

void xml2brlOahDualHandler::createHarmoniesUserGroup (
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
        fUserHandler);
  fUserHandler->
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

void xml2brlOahDualHandler::createFiguredBassesUserGroup (
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
        fUserHandler);
  fUserHandler->
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

void xml2brlOahDualHandler::createLyricsUserGroup (
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
        fUserHandler);
  fUserHandler->
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

void xml2brlOahDualHandler::createOutputUserGroup (
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
        fUserHandler);
  fUserHandler->
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

  fAtomNamesToUserSubGroupsMap ["msr2lpsr-score-output-kind"] = subGroup;
  fAtomNamesToUserSubGroupsMap ["lpsr-Languages-language"] = subGroup;
}

string xml2brlOahDualHandler::asString () const
{
  return "xml2brlOahDualHandler";
}

void xml2brlOahDualHandler::print (ostream& os) const
{
  os <<
    "xml2brlOahDualHandler:" <<
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

  gIndenter--;

  gIndenter--;

  os << endl;
}

ostream& operator<< (ostream& os, const S_xml2brlOahDualHandler& elt)
{
  elt->print (os);
  return os;
}

/*
string xml2brlDualHandlerInsiderAtom::asShortNamedOptionString () const
{
  stringstream s;

  s <<
    "-" << fShortName << " " <<
    S_xml2brlOahDualHandlerAsString (fXml2brlOahDualHandlerVariable);

  return s.str ();
}

string xml2brlDualHandlerInsiderAtom::asActualLongNamedOptionString () const
{
  stringstream s;

  s <<
    "-" << fLongName << " " <<
    S_xml2brlOahDualHandlerAsString (fXml2brlOahDualHandlerVariable);

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
