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

/* JMI
#include "setTraceOahIfDesired.h"
#include "traceOah.h"

#include "msr.h"
//#include "lpsr.h" // JMI only if pass5 msr2lpsr is run

#include "oahOah.h"

#include "generalOah.h"

#include "musicxmlOah.h"
#include "mxmlTreeOah.h"
#include "msr2MxmltreeOah.h"
#include "msrOah.h"
#include "msr2MxmltreeOah.h"
#include "mxmlTree2MsrOah.h"
#include "msr2LpsrOah.h" // JMI ???
//#include "lpsrOah.h" // JMI only if pass5 msr2lpsr is run

#include "version.h"

#include "xml2xmlManPageOah.h"
*/

#include "xml2xmlOah.h"

#include "xml2xmlOahDualHandler.h"

using namespace std;

namespace MusicXML2
{

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
  fInternalOahHandler =
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

  createUserGroups (ios);
  createUserSubGroups (ios);

  createSubGroupNamesToUserGroupsMap ();
  createAtomNamesToUserSubGroupsMap ();

  populateUserGroupsFromInteralGroups ();
}

void xml2xmlOahDualHandler::createUserGroups (
  ostream& ios)
{
  // populate the user groups list with empty groups
/*
    static SMARTP<oahGroup> create (
      string                  header,
      string                  shortName,
      string                  longName,
      string                  description,
      oahElementVisibilityKind optionVisibilityKind,
      S_oahHandler            groupHandlerUpLink);
*/

  // application
  fApplicationUserGroup =
    oahGroup::create (
      "xml2xml",
      "app",
      "application",
      "",
      kElementVisibilityAlways,
      fUserOahHandler);
  fUserOahHandler->
    appendGroupToHandler (fApplicationUserGroup);

  // documentation
  fDocumentationUserGroup =
    oahGroup::create (
      "Documentation",
      "doc",
      "",
      "",
      kElementVisibilityAlways,
      fUserOahHandler);
  fUserOahHandler->
    appendGroupToHandler (fDocumentationUserGroup);

  // files
  fFilesUserGroup =
    oahGroup::create (
      "Files",
      "files",
      "",
      "",
      kElementVisibilityAlways,
      fUserOahHandler);
  fUserOahHandler->
    appendGroupToHandler (fFilesUserGroup);

  // trace
  fTraceUserGroup =
    oahGroup::create (
      "Trace",
      "trace",
      "",
      "",
      kElementVisibilityHiddenByDefault,
      fUserOahHandler);
  fUserOahHandler->
    appendGroupToHandler (fTraceUserGroup);

  // display
  fDisplayUserGroup =
    oahGroup::create (
      "Display",
      "display",
      "",
      "",
      kElementVisibilityHiddenByDefault,
      fUserOahHandler);
  fUserOahHandler->
    appendGroupToHandler (fDisplayUserGroup);

  // layout
  fLayoutUserGroup =
    oahGroup::create (
      "Layout",
      "layout",
      "",
      "",
      kElementVisibilityAlways,
      fUserOahHandler);
  fUserOahHandler->
    appendGroupToHandler (fLayoutUserGroup);

  // languages
  fLanguagesUserGroup =
    oahGroup::create (
      "Languages",
      "langs",
      "languages",
      "",
      kElementVisibilityAlways,
      fUserOahHandler);
  fUserOahHandler->
    appendGroupToHandler (fLanguagesUserGroup);

  // part groups and parts
  fPartGroupsAndPartsUserGroup =
    oahGroup::create (
      "Part groups & parts",
      "parts",
      "partgroups-and-parts",
      "",
      kElementVisibilityAlways,
      fUserOahHandler);
  fUserOahHandler->
    appendGroupToHandler (fPartGroupsAndPartsUserGroup);

  // staves
  fStavesUserGroup =
    oahGroup::create (
      "Staves",
      "staves",
      "",
      "",
      kElementVisibilityAlways,
      fUserOahHandler);
  fUserOahHandler->
    appendGroupToHandler (fStavesUserGroup);

  // voices
  fVoicesUserGroup =
    oahGroup::create (
      "Voices",
      "voices",
      "",
      "",
      kElementVisibilityAlways,
      fUserOahHandler);
  fUserOahHandler->
    appendGroupToHandler (fVoicesUserGroup);

  // measures
  fMeasuresUserGroup =
    oahGroup::create (
      "Measures",
      "measures",
      "",
      "",
      kElementVisibilityAlways,
      fUserOahHandler);
  fUserOahHandler->
    appendGroupToHandler (fMeasuresUserGroup);

  // clefs, keys and times
  fClefsKeysAndTimesUserGroup =
    oahGroup::create (
      "Clefs, keys & times",
      "ckat",
      "clefs-keys-and-times",
      "",
      kElementVisibilityAlways,
      fUserOahHandler);
  fUserOahHandler->
    appendGroupToHandler (fClefsKeysAndTimesUserGroup);

  // notes
  fNotesUserGroup =
    oahGroup::create (
      "Notes",
      "notes",
      "",
      "",
      kElementVisibilityAlways,
      fUserOahHandler);
  fUserOahHandler->
    appendGroupToHandler (fNotesUserGroup);

  // dynamics and wedges
  fDynamicsAndWedgesUserGroup =
    oahGroup::create (
      "Dynamics & wedges",
      "dynwed",
      "dynamics-and-wedges",
      "",
      kElementVisibilityAlways,
      fUserOahHandler);
  fUserOahHandler->
    appendGroupToHandler (fDynamicsAndWedgesUserGroup);

  // chords
  fChordsUserGroup =
    oahGroup::create (
      "Chords",
      "chords",
      "",
      "",
      kElementVisibilityAlways,
      fUserOahHandler);
  fUserOahHandler->
    appendGroupToHandler (fChordsUserGroup);

  // tuplets
  fTupletsUserGroup =
    oahGroup::create (
      "Tuplets",
      "tuplets",
      "",
      "",
      kElementVisibilityAlways,
      fUserOahHandler);
  fUserOahHandler->
    appendGroupToHandler (fTupletsUserGroup);

  // transposition
  fTranspositionUserGroup =
    oahGroup::create (
      "Transposition",
      "transp",
      "transposition",
      "",
      kElementVisibilityAlways,
      fUserOahHandler);
  fUserOahHandler->
    appendGroupToHandler (fTranspositionUserGroup);

  // lyrics
  fLyricsUserGroup =
    oahGroup::create (
      "Lyrics",
      "lyrics",
      "",
      "",
      kElementVisibilityAlways,
      fUserOahHandler);
  fUserOahHandler->
    appendGroupToHandler (fLyricsUserGroup);

  // harmonies
  fHarmoniesUserGroup =
    oahGroup::create (
      "Harmonies",
      "harms",
      "harmonies",
      "",
      kElementVisibilityAlways,
      fUserOahHandler);
  fUserOahHandler->
    appendGroupToHandler (fHarmoniesUserGroup);

  // figured basses
  fFiguredBassesUserGroup =
    oahGroup::create (
      "Figured basses",
      "fgs",
      "figured-basses",
      "",
      kElementVisibilityAlways,
      fUserOahHandler);
  fUserOahHandler->
    appendGroupToHandler (fFiguredBassesUserGroup);

  // midi
  fMidiUserGroup =
    oahGroup::create (
      "Midi",
      "midi",
      "",
      "",
      kElementVisibilityAlways,
      fUserOahHandler);
  fUserOahHandler->
    appendGroupToHandler (fMidiUserGroup);

  // exit
  fExitUserGroup =
    oahGroup::create (
      "Exit",
      "exit",
      "",
      "",
      kElementVisibilityHiddenByDefault,
      fUserOahHandler);
  fUserOahHandler->
    appendGroupToHandler (fExitUserGroup);

  // extra
  fExtraUserGroup =
    oahGroup::create (
      "Extra",
      "extra",
      "",
      "",
      kElementVisibilityHiddenByDefault,
      fUserOahHandler);
  fUserOahHandler->
    appendGroupToHandler (fExtraUserGroup);

  // other
  fOtherUserGroup =
    oahGroup::create (
      "Other",
      "other",
      "",
      "",
      kElementVisibilityAlways,
      fUserOahHandler);
  fUserOahHandler->
    appendGroupToHandler (fOtherUserGroup);

  // set the groups list to be used
  fOahHandlerToBeUsed = fUserOahHandler; // default value
}

void xml2xmlOahDualHandler::createSubGroupNamesToUserGroupsMap ()
{
//  fSubGroupNamesToUserGroupsMap ["help-xml2mxml"] = fDocumentationUserGroup;

//  fSubGroupNamesToUserGroupsMap ["help-general"] = fDocumentationUserGroup;
}

void xml2xmlOahDualHandler::createUserSubGroups (
  ostream& ios)
{
  // populate the user groups with empty subgroups
/*
    oahSubGroup (
      string                  subGroupHeader,
      string                  shortName,
      string                  longName,
      string                  description,
      oahElementVisibilityKind optionVisibilityKind,
      S_oahGroup              groupUpLink);
*/

  fInformationUserSubGroup =
    oahSubGroup::create (
      "Information",
      "info",
      "",
      "",
      kElementVisibilityAlways,
      fApplicationUserGroup);
  fApplicationUserGroup->
    appendSubGroupToGroup (fInformationUserSubGroup);

  fExitUserSubGroup =
    oahSubGroup::create (
      "Exit",
      "exit",
      "",
      "",
      kElementVisibilityAlways,
      fApplicationUserGroup);
  fApplicationUserGroup->
    appendSubGroupToGroup (fExitUserSubGroup);

  fFilesUserSubGroup =
    oahSubGroup::create (
      "Files",
      "files",
      "",
      "",
      kElementVisibilityAlways,
      fApplicationUserGroup);
  fFilesUserGroup->
    appendSubGroupToGroup (fFilesUserSubGroup);

  fOtherUserSubGroup =
    oahSubGroup::create (
      "Other",
      "other",
      "",
      "",
      kElementVisibilityAlways,
      fOtherUserGroup);
  fOtherUserGroup->
    appendSubGroupToGroup (fOtherUserSubGroup);
}

void xml2xmlOahDualHandler::createAtomNamesToUserSubGroupsMap ()
{
  fAtomNamesToUserSubGroupsMap ["version"] = fInformationUserSubGroup;
  fAtomNamesToUserSubGroupsMap ["about"] = fInformationUserSubGroup;
  fAtomNamesToUserSubGroupsMap ["contact"] = fInformationUserSubGroup;

  fAtomNamesToUserSubGroupsMap ["exit-2a"] = fExitUserSubGroup;
  fAtomNamesToUserSubGroupsMap ["exit-2b"] = fExitUserSubGroup;

  fAtomNamesToUserSubGroupsMap ["output-file-name"] = fFilesUserSubGroup;
}

void xml2xmlOahDualHandler::populateUserGroupsFromInteralGroups ()
{
  const list<S_oahGroup>
    internalOahHandlerGroupsList =
      fInternalOahHandler->getHandlerGroupsList ();

  if (internalOahHandlerGroupsList.size ()) {
    list<S_oahGroup>::const_iterator
      iBegin = internalOahHandlerGroupsList.begin (),
      iEnd   = internalOahHandlerGroupsList.end (),
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
            (*it).second->
              appendSubGroupToGroup (subGroup);
          }
          else {
            // subGroupNameToUse is not known in the map

            // are there atom in this subgroup in fAtomNamesToUserSubGroupsMap?

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
                  (*it).second->
                    appendAtomToSubGroup (atom);
                }
                else {
                  // place it in the 'other' user subgroup
                  fOtherUserSubGroup->
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
}

void xml2xmlOahDualHandler::print (ostream& os) const
{
  os <<
    "xml2xmlOahDualHandler:" <<
    endl;

  gIndenter++;

  os <<
    "internalOahHandler" <<
    endl;

  gIndenter++;

  const list<S_oahGroup>
    internalOahHandlerGroupsList =
      fInternalOahHandler->getHandlerGroupsList ();

  if (internalOahHandlerGroupsList.size ()) {
    os << endl;

    list<S_oahGroup>::const_iterator
      iBegin = internalOahHandlerGroupsList.begin (),
      iEnd   = internalOahHandlerGroupsList.end (),
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

  os <<
    endl <<
    "------------ " <<
    "internalOahHandler" <<
     " ------------" <<
   endl;

  gIndenter++;

  const list<S_oahGroup>
    internalOahHandlerGroupsList =
      fInternalOahHandler->getHandlerGroupsList ();

  if (internalOahHandlerGroupsList.size ()) {
    os << endl;

    list<S_oahGroup>::const_iterator
      iBegin = internalOahHandlerGroupsList.begin (),
      iEnd   = internalOahHandlerGroupsList.end (),
      i      = iBegin;
    for ( ; ; ) {
      // print the group help
      (*i)->printHelp (os);
      if (++i == iEnd) break;
      os << endl;
    } // for
  }

  gIndenter--;

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


}
