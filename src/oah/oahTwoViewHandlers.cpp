/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#include "utilities.h"
#include "messagesHandling.h"

#include "oahTwoViewHandlers.h"

#include "setTraceOahIfDesired.h"
#ifdef TRACE_OAH
  #include "traceOah.h"
#endif

#include "oahOah.h"
#include "generalOah.h"


namespace MusicXML2
{

//#define OAH_TESTS

//______________________________________________________________________________
/* pure virtual class
S_oahTwoViewHandler oahTwoViewHandler::create (
  string   twoViewHandlerName,
  string   executableName,
  string   twoViewAtomShortName,
  string   twoViewAtomLongName,
  ostream& os)
{
  oahTwoViewHandler* o = new
    oahTwoViewHandler (
      twoViewHandlerName,
      executableName,
      twoViewAtomShortName,
      twoViewAtomLongName,
      os);
  assert(o!=0);

// JMI  o->createTheTwoHandlers (os);

  return o;
}
*/

oahTwoViewHandler::oahTwoViewHandler (
  string   twoViewHandlerName,
  string   executableName,
  string   twoViewAtomShortName,
  string   twoViewAtomLongName,
  ostream& os)
{
  fTwoViewHandlerName = twoViewHandlerName;

#ifdef TRACE_OAH
#ifdef OAH_TESTS
    gLogOstream <<
      "Constructing OAH twoView handler \"" << fTwoViewHandlerName << "\"" <<
      endl;
#endif
#endif

  fExecutableName = executableName;

  fTwoViewAtomShortName = twoViewAtomShortName;
  fTwoViewAtomLongName  = twoViewAtomLongName;

  // createTheTwoHandlers() will be called in the derived classes 'create()' method
}

oahTwoViewHandler::~oahTwoViewHandler ()
{}

void oahTwoViewHandler::switchToTwoViewView ()
{
  fOahHandlerToBeUsed = fFullViewOahHandler;

#ifdef TRACE_OAH
#ifdef OAH_TESTS
  gLogOstream <<
    "Switching twoView handler \"" <<
    fTwoViewHandlerName <<
    "\" handler to \"" <<
    fOahHandlerToBeUsed->getHandlerHeader () <<
    "\" (twoView view)" <<
    endl;
#endif
#endif

  // fTwoViewOptionsCounter will be incremented in
  // oahTwoViewHandlerTwoViewAtom::handleOptionUnderName (),
  // since the 'twoView' option will be handled anyway
}

void oahTwoViewHandler::populateAlternateViewHandlerFromFullViewHandler ()
{
  /*
    The relative order of atoms found in the 'full' view
    is retained in the 'alternate' view
  */

#ifdef TRACE_OAH
#ifdef OAH_TESTS
  gLogOstream <<
    "Populating the alternate view handler from the full view handler for \"" << fTwoViewHandlerName << "\"" <<
    endl;
#endif
#endif

  // create the 'put aside' group
  fPutAsideInAlternateViewGroup =
    oahGroup::create (
      "Put aside group",
      "hput-aside-group",
      "",
      "",
      kElementVisibilityNone,
      fAlternateViewOahHandler);
  fPutAsideInAlternateViewGroup->
    setGroupHeaderIsToBeWritten (false);
  fAlternateViewOahHandler->
    appendGroupToHandler (fPutAsideInAlternateViewGroup);

  // create the 'put aside' subgroup
  fPutAsideInAlternateViewSubGroup =
    oahSubGroup::create (
      "Put aside",
      "hput-aside",
      "",
      "",
      kElementVisibilityWhole,
      fPutAsideInAlternateViewGroup);
  fPutAsideInAlternateViewGroup->
    appendSubGroupToGroup (fPutAsideInAlternateViewSubGroup);

  // get the twoView OAH handler groups list
  const list<S_oahGroup>
    twoViewHandlerGroupsList =
      fFullViewOahHandler->
        getHandlerGroupsList ();

  // handle it
  if (twoViewHandlerGroupsList.size ()) {
    list<S_oahGroup>::const_iterator
      iBegin = twoViewHandlerGroupsList.begin (),
      iEnd   = twoViewHandlerGroupsList.end (),
      i      = iBegin;
    for ( ; ; ) {
      // handle the group
      S_oahGroup group = (*i);

      const list<S_oahSubGroup>
        groupSubGroupsList =
          group->
            getSubGroupsList ();

      if (groupSubGroupsList.size ()) {
        list<S_oahSubGroup>::const_iterator
          iBegin = groupSubGroupsList.begin (),
          iEnd   = groupSubGroupsList.end (),
          i      = iBegin;
        for ( ; ; ) {
          S_oahSubGroup subGroup = (*i);

          // handle the subgroup
          handleSubGroupMapping (subGroup);

          if (++i == iEnd) break;
        } // for
      }

      if (++i == iEnd) break;
    } // for
  }

  // have all the mappings been used?
  checkMappingsUse ();

#ifdef TRACE_OAH
#ifdef OAH_TESTS
  printFullViewOahHandler (gLogOstream);

  gLogOstream <<
    endl << endl <<
    "*******************************************************************" <<
    endl << endl;

/* JMI
  printtwoViewHandler (gLogOstream);

  fAlternateViewOahHandler->
    print (
      gLogOstream);

  gLogOstream <<
    endl << endl <<
    "*******************************************************************" <<
    endl << endl;
*/
#endif
#endif
}

void oahTwoViewHandler::handleSubGroupMapping (S_oahSubGroup subGroup)
{
  string subGroupShortName = subGroup->getShortName ();
  string subGroupLongName  = subGroup->getLongName ();

  string
    subGroupNameToUse =
      subGroupLongName.size ()
        ? subGroupLongName
        : subGroupShortName;

 // gLogOstream << "subGroupNameToUse = " << subGroupNameToUse << endl;

  // is nameToUse known in fSubGroupNamesToAlternateGroupsMap?
  map<string, S_oahGroup>::const_iterator
    it =
      fSubGroupNamesToAlternateGroupsMap.find (
        subGroupNameToUse);

  if (it != fSubGroupNamesToAlternateGroupsMap.end ()) {
    // subGroupNameToUse is known in the map
    S_oahGroup group = (*it).second;

    // append subgroup to alternate group
#ifdef TRACE_OAH
    if (gGlobalTraceOahGroup->getTraceOahDetails ()) {
      gLogOstream <<
        "+++ appending subgroup \"" <<
        subGroupNameToUse <<
        "\" to group \"" <<
        group->getGroupHeader () <<
        "\"" <<
        endl;
    }
#endif

    group->
      appendSubGroupToGroup (subGroup);

    // remove subGroupNameToUse from the map
    it = fSubGroupNamesToAlternateGroupsMap.erase (it);
  }

  else {
    // subGroupNameToUse is not known in the map

    // are there atoms from this subgroup in fAtomNamesToAlternateSubGroupsMap?

    const list<S_oahAtom>&
      subGroupAtomsList =
        subGroup->
          getAtomsList ();

    if (subGroupAtomsList.size ()) {
      list<S_oahAtom>::const_iterator
        iBegin = subGroupAtomsList.begin (),
        iEnd   = subGroupAtomsList.end (),
        i      = iBegin;
      for ( ; ; ) {
        S_oahAtom atom = (*i);

        // handle the atom
        handleAtomMapping (atom);

        if (++i == iEnd) break;
      } // for
    }
  }
}

void oahTwoViewHandler::handleAtomMapping (S_oahAtom atom)
{
  string atomShortName = atom->getShortName ();
  string atomLongName  = atom->getLongName ();

  string
    atomNameToUse =
      atomLongName.size ()
        ? atomLongName
        : atomShortName;

  // gLogOstream << "atomNameToUse = " << atomNameToUse << endl;

  // is atomNameToUse known in fAtomNamesToAlternateSubGroupsMap?
  map<string, S_oahSubGroup>::const_iterator
    it =
      fAtomNamesToAlternateSubGroupsMap.find (
        atomNameToUse);

  if (it != fAtomNamesToAlternateSubGroupsMap.end ()) {
    // atomNameToUse is known in the map
    S_oahSubGroup subGroup = (*it).second;

    // append atom to alternate subgroup
#ifdef TRACE_OAH
    if (gGlobalTraceOahGroup->getTraceOahDetails ()) {
      gLogOstream <<
        "+++ appending atom \"" <<
        atomNameToUse <<
        "\" to subgroup and registering it in the 'alternate' handler \"" <<
        subGroup->getSubGroupHeader () <<
        "\"" <<
        endl;
    }
#endif

    subGroup->
      appendAtomToSubGroup (atom);

    atom->
      registerAtomInHandler (fAlternateViewOahHandler);

    // remove atomNameToUse from the map
    it = fAtomNamesToAlternateSubGroupsMap.erase (it);
  }
  else {
    // atomNameToUse is not known in the map
    // place it in the 'put aside' alternate subgroup

#ifdef TRACE_OAH
    if (gGlobalTraceOahGroup->getTraceOahDetails ()) {
      gLogOstream <<
        "+++ appending atom \"" <<
        atomNameToUse <<
        "\" to subgroup \"" <<
        fPutAsideInAlternateViewSubGroup->getSubGroupHeader () <<
        "\"" <<
        endl;
    }
#endif

    fPutAsideInAlternateViewSubGroup->
      appendAtomToSubGroup (atom);
    fPutAsideInAlternateViewAtomNamesList.push_back (
      atomNameToUse);
  }
}

void oahTwoViewHandler::checkMappingsUse ()
{
#ifdef TRACE_OAH
  if (gGlobalTraceOahGroup->getTraceOah ()) {
    // print the known prefixes
    fAlternateViewOahHandler->
      printKnownPrefixes (gLogOstream);
  }
#endif

  // check subgroups mapping
  int subGroupNamesTotwoViewGroupsMapSize =
    fSubGroupNamesToAlternateGroupsMap.size ();

  if (subGroupNamesTotwoViewGroupsMapSize) {
#ifdef TRACE_OAH
    if (gGlobalTraceOahGroup->getTraceOah ()) {
      stringstream s;

      s <<
        "The following " <<
        singularOrPlural (
          subGroupNamesTotwoViewGroupsMapSize, "subgroup name", "subgroup names") <<
        " could not be mapped to alternate groups: ";

      map<string, S_oahGroup>::const_iterator
        iBegin = fSubGroupNamesToAlternateGroupsMap.begin (),
        iEnd   = fSubGroupNamesToAlternateGroupsMap.end (),
        i      = iBegin;
      for ( ; ; ) {
        // handle the name
        s <<
           "\"" << (*i).first << "\"";

        if (++i == iEnd) break;
        s << ", ";
      } // for

      gLogOstream << s.str () << endl;
  //    oahError (s.str ());
    }
#endif
  }

  // check atoms mapping
  int atomNamesTotwoViewGroupsMapSize =
    fAtomNamesToAlternateSubGroupsMap.size ();

  if (atomNamesTotwoViewGroupsMapSize) {
#ifdef TRACE_OAH
    if (gGlobalTraceOahGroup->getTraceOah ()) {
      stringstream s;

      s <<
        "The following " <<
        singularOrPlural (
          atomNamesTotwoViewGroupsMapSize, "atom name", "atom names") <<
        " could not be mapped to alternate subgroups: ";

      map<string, S_oahSubGroup>::const_iterator
        iBegin = fAtomNamesToAlternateSubGroupsMap.begin (),
        iEnd   = fAtomNamesToAlternateSubGroupsMap.end (),
        i      = iBegin;
      for ( ; ; ) {
        // handle the name
        s <<
           "\"" << (*i).first << "\"";

        if (++i == iEnd) break;
        s << ", ";
      } // for

      gLogOstream << s.str () << endl;
    //    oahError (s.str ());
    }
#endif
  }

  // display the atoms that have been put aside
  int putAsideIntwoViewViewAtomNamesListSize =
    fPutAsideInAlternateViewAtomNamesList.size ();

  if (putAsideIntwoViewViewAtomNamesListSize) {
#ifdef TRACE_OAH
    if (gGlobalTraceOahGroup->getTraceOah ()) {
      stringstream s;

      // build the list of atom names to be output
      list<string> atomNamesToBeOutputList;

      for (
        list<string>::const_iterator i = fPutAsideInAlternateViewAtomNamesList.begin ();
        i != fPutAsideInAlternateViewAtomNamesList.end ();
        i++
      ) {
        // handle the atom name
        string atomName = (*i);

#ifdef OAH_TESTS
        bool
          shouldTheAtomBeOutput =
            false
              ? true
              : atomName.size ()
                  &&
                atomName.find ("trace") == string::npos
                  &&
                atomName.find ("help") == string::npos
                  &&
                atomName.find ("display") == string::npos
                  &&
                atomName.find ("_") == string::npos;

        if (shouldTheAtomBeOutput) {
          atomNamesToBeOutputList.push_back (atomName);
        }
#endif
      } // for

      // output the retained atom names if any
      int atomNamesToBeOutputListSize =
        atomNamesToBeOutputList.size ();

      if (atomNamesToBeOutputListSize) {
        s <<
          "The following non-trace nor non-display " <<
          singularOrPlural (
            atomNamesToBeOutputListSize, "atom name", "atom names") <<
          " have been left out of the mapping to alternate subgroups:" <<
          endl;

        // sort the list for legibility
        atomNamesToBeOutputList.sort ();

        list<string>::const_iterator
          iBegin = atomNamesToBeOutputList.begin (),
          iEnd   = atomNamesToBeOutputList.end (),
          i      = iBegin;
        for ( ; ; ) {
          // handle the name
          string theString = (*i);

          s <<
             "\"" << theString << "\"";

          if (++i == iEnd) break;
          s << ", ";
        } // for

        gLogOstream << s.str () << endl;
      }
    }
#endif
  }
}

oahHandler::oahHelpOptionsHaveBeenUsedKind oahTwoViewHandler::applyOptionsAndArgumentsFromArgcAndArgv (
  int   argc,
  char* argv[])
{
#ifdef TRACE_OAH
  if (gGlobalTraceOahGroup->getTraceOah ()) {
    gLogOstream <<
      "==> oahTwoViewHandler::applyOptionsAndArgumentsFromArgcAndArgv()" <<
      ", fTwoViewHandlerName: \"" << fTwoViewHandlerName << "\"" <<
      endl;
  }
#endif

  // should the twoView or alternate oahHandler be used?
  if (argc >= 2) {
    string argument1 (argv [1]);

    if (
      argument1 == string ("-") + fTwoViewAtomShortName
        ||
      argument1 == string ("-") + fTwoViewAtomLongName
    ) {
      switchToTwoViewView ();
    }
    else {
#ifdef TRACE_OAH
#ifdef OAH_TESTS
      gLogOstream <<
        "Keeping full view handler \"" <<
        fTwoViewHandlerName <<
        "\" handler \"" <<
        fOahHandlerToBeUsed->getHandlerHeader () <<
        "\" (alternate view)" <<
        endl;
#endif
#endif
    }
  }

  oahHandler::oahHelpOptionsHaveBeenUsedKind
    result =
      fOahHandlerToBeUsed->
        applyOptionsAndArgumentsFromArgcAndArgv (
          argc,
          argv);

  return result;
}

oahHandler::oahHelpOptionsHaveBeenUsedKind oahTwoViewHandler::applyOptionsFromOptionsVector (
  string               fakeExecutableName,
  const optionsVector& theOptionsVector)
{
#ifdef TRACE_OAH
  if (gGlobalTraceOahGroup->getTraceOah ()) {
    gLogOstream <<
      "==> oahTwoViewHandler::applyOptionsFromOptionsVector" <<
      endl;
  }
#endif

  // should the full or alternate oahHandler be used?
  if (theOptionsVector.size () >= 1) {
    string optionName1 (theOptionsVector [0].first);

    if (
      optionName1 == string ("-") + fTwoViewAtomShortName
        ||
      optionName1 == string ("-") + fTwoViewAtomLongName
    ) {
      switchToTwoViewView ();
    }
    else {
#ifdef TRACE_OAH
#ifdef OAH_TESTS
      gLogOstream <<
        "==> oahTwoViewHandler::applyOptionsFromOptionsVector(): remaining in alternate view" <<
        endl;
#endif
#endif
    }
  }

  return
    fOahHandlerToBeUsed->
      applyOptionsFromOptionsVector (
        fakeExecutableName,
        theOptionsVector);
}

string oahTwoViewHandler::oahViewHandlerKindAsString (
  oahViewHandlerKind viewHandlerKind)
{
  string result;

  switch (viewHandlerKind) {
    case oahTwoViewHandler::kFullViewHandler:
      result = "fullViewHandler";
      break;
    case oahTwoViewHandler::kAlternateViewHandler:
      result = "alternateViewHandler";
      break;
  } // switch

  return result;
}

string oahTwoViewHandler::commandLineAsSuppliedAsString () const
{
  return
    fOahHandlerToBeUsed->
      commandLineAsSuppliedAsString ();
}

string oahTwoViewHandler::commandLineWithShortNamesAsString () const
{
  return
    fOahHandlerToBeUsed->
      commandLineWithShortNamesAsString ();
}

string oahTwoViewHandler::commandLineWithLongNamesAsString () const
{
  return
    fOahHandlerToBeUsed->
      commandLineWithLongNamesAsString ();
}

void oahTwoViewHandler::printAllOahCommandLineValues (
  ostream& os) const
{
  fFullViewOahHandler-> printAllOahCommandLineValues (os);
  fAlternateViewOahHandler-> printAllOahCommandLineValues (os);
}

string oahTwoViewHandler::asString () const
{
  return "oahTwoViewHandlerTwoViewAtom";
}

void oahTwoViewHandler::printFullViewOahHandler (ostream& os) const
{
  os <<
    endl <<
    "------------------" <<
    "fullViewOahHandler" <<
    " -----------------" <<
   endl;

  gIndenter++;

  const list<S_oahGroup>
    twoViewHandlerGroupsList =
      fFullViewOahHandler->
        getHandlerGroupsList ();

  if (twoViewHandlerGroupsList.size ()) {
    os << endl;

    list<S_oahGroup>::const_iterator
      iBegin = twoViewHandlerGroupsList.begin (),
      iEnd   = twoViewHandlerGroupsList.end (),
      i      = iBegin;
    for ( ; ; ) {
      // print the group help
      (*i)->print (os);
      if (++i == iEnd) break;
      os << endl;
    } // for
  }

  gIndenter--;
}

void oahTwoViewHandler::printAlternateViewOahHandler (ostream& os) const
{
  os <<
    endl <<
    "-------------------" <<
    "alternateOahHandler" <<
    " ------------------" <<
    endl;

  gIndenter++;

  const list<S_oahGroup>
    alternateOahHandlerGroupsList =
      fAlternateViewOahHandler->
        getHandlerGroupsList ();

  if (alternateOahHandlerGroupsList.size ()) {
    os << endl;

    list<S_oahGroup>::const_iterator
      iBegin = alternateOahHandlerGroupsList.begin (),
      iEnd   = alternateOahHandlerGroupsList.end (),
      i      = iBegin;
    for ( ; ; ) {
      // print the group help
      (*i)->print (os);
      if (++i == iEnd) break;
      os << endl;
    } // for
  }

  gIndenter--;
}

void oahTwoViewHandler::print (ostream& os) const
{
  os <<
    "------------ " <<
    endl <<
    "TwoViewHandler \"" << fTwoViewHandlerName << "\":" <<
    ", executableName: \"" << fExecutableName <<  "\":" <<
    ", twoViewAtomShortName: \"" << fTwoViewAtomShortName <<  "\":" <<
    ", twoViewAtomLongName: \"" << fTwoViewAtomLongName <<  "\":" <<
    ", viewHandlerKind: \"" << oahViewHandlerKind (fViewHandlerKind) <<  "\":" <<
    endl <<
    " ------------" <<
    endl;

  gIndenter++;

  printFullViewOahHandler (os);

  os <<
    endl <<
    "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" <<
    endl << endl;

  printAlternateViewOahHandler (os);

  gIndenter--;

  os << endl;
}

void oahTwoViewHandler::printHelp (ostream& os) const
{
  os <<
    "------------ " <<
    endl <<
    "TwoViewHandler \"" << fTwoViewHandlerName << "\":" <<
    ", fExecutableName: \"" << fExecutableName <<  "\":" <<
    ", fTwoViewAtomShortName: \"" << fTwoViewAtomShortName <<  "\":" <<
    ", fTwoViewAtomLongName: \"" << fTwoViewAtomLongName <<  "\":" <<
    endl <<
    " ------------" <<
    endl;

  gIndenter++;

  if (true) { // JMI
    os <<
      endl <<
      "------------------" <<
      "fullViewOahHandler" <<
       " ----------------" <<
     endl;

    gIndenter++;

    const list<S_oahGroup>
      twoViewHandlerGroupsList =
        fFullViewOahHandler->
          getHandlerGroupsList ();

    if (twoViewHandlerGroupsList.size ()) {
      os << endl;

      list<S_oahGroup>::const_iterator
        iBegin = twoViewHandlerGroupsList.begin (),
        iEnd   = twoViewHandlerGroupsList.end (),
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

  {
    os <<
      endl <<
      "-------------------" <<
      "alternateOahHandler" <<
      " ------------------" <<
      endl;

    gIndenter++;

    const list<S_oahGroup>
      alternateOahHandlerGroupsList =
        fAlternateViewOahHandler->
          getHandlerGroupsList ();

    if (alternateOahHandlerGroupsList.size ()) {
      os << endl;

      list<S_oahGroup>::const_iterator
        iBegin = alternateOahHandlerGroupsList.begin (),
        iEnd   = alternateOahHandlerGroupsList.end (),
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

  gIndenter--;

  os << endl;
}

/* JMI
//______________________________________________________________________________
S_oahTwoViewHandlerTwoViewAtom oahTwoViewHandlerTwoViewAtom::create (
  string shortName,
  string longName,
  string description)
{
  oahTwoViewHandlerTwoViewAtom* o = new
    oahTwoViewHandlerTwoViewAtom (
      shortName,
      longName,
      description);
  assert(o!=0);
  return o;
}

oahTwoViewHandlerTwoViewAtom::oahTwoViewHandlerTwoViewAtom (
  string shortName,
  string longName,
  string description)
  : oahAtom (
      shortName,
      longName,
      description,
      kElementValueExpectedNo)
{
  fTwoViewOptionsCounter = 0;
}

oahTwoViewHandlerTwoViewAtom::~oahTwoViewHandlerTwoViewAtom ()
{}

S_oahValuedAtom oahTwoViewHandlerTwoViewAtom::handleOptionUnderName (
  string   optionName,
  ostream& os)
{
#ifdef TRACE_OAH
  if (gGlobalTraceOahGroup->getTraceOah ()) {
    gLogOstream <<
      "==> option '" << optionName << "' is a oahTwoViewHandlerTwoViewAtom" <<
      endl;
  }
#endif

  fTwoViewOptionsCounter++;

  / *
    The initial 'twoView' option is handled even though
    it has been trapped very early in:
     oahTwoViewHandler::applyOptionsAndArgumentsFromArgcAndArgv()
    and:
      oahTwoViewHandler::applyOptionsFromOptionsVector()
  * /

  if (fTwoViewOptionsCounter > 1) {
    stringstream s;

    s <<
      "option \"" << fShortName << "\" should be the first one";

    oahError (s.str ());
  }

  // no option value is needed
  return nullptr;
}

void oahTwoViewHandlerTwoViewAtom::acceptIn (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gGlobalOahOahGroup->fTraceOahVisitors) {
    gLogOstream <<
      "% ==> oahTwoViewHandlerTwoViewAtom::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_oahTwoViewHandlerTwoViewAtom>*
    p =
      dynamic_cast<visitor<S_oahTwoViewHandlerTwoViewAtom>*> (v)) {
        S_oahTwoViewHandlerTwoViewAtom elem = this;

#ifdef TRACE_OAH
        if (gGlobalOahOahGroup->fTraceOahVisitors) {
          gLogOstream <<
            "% ==> Launching oahTwoViewHandlerTwoViewAtom::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void oahTwoViewHandlerTwoViewAtom::acceptOut (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gGlobalOahOahGroup->fTraceOahVisitors) {
    gLogOstream <<
      "% ==> oahTwoViewHandlerTwoViewAtom::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_oahTwoViewHandlerTwoViewAtom>*
    p =
      dynamic_cast<visitor<S_oahTwoViewHandlerTwoViewAtom>*> (v)) {
        S_oahTwoViewHandlerTwoViewAtom elem = this;

#ifdef TRACE_OAH
        if (gGlobalOahOahGroup->fTraceOahVisitors) {
          gLogOstream <<
            "% ==> Launching oahTwoViewHandlerTwoViewAtom::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void oahTwoViewHandlerTwoViewAtom::browseData (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gGlobalOahOahGroup->fTraceOahVisitors) {
    gLogOstream <<
      "% ==> oahTwoViewHandlerTwoViewAtom::browseData ()" <<
      endl;
  }
#endif
}

string oahTwoViewHandlerTwoViewAtom::asString () const
{
  return "oahTwoViewHandlerTwoViewAtom";
}

void oahTwoViewHandlerTwoViewAtom::print (ostream& os) const
{
  os <<
    "TwoViewHandlerTwoViewAtom:" <<
    endl;
}

ostream& operator<< (ostream& os, const S_oahTwoViewHandlerTwoViewAtom& elt)
{
  elt->print (os);
  return os;
}
*/

}


/* JMI
  // display the empty resulting alternate subgroups if any
  const list<S_oahGroup>
    alternateOahHandlerGroupsList =
      fAlternateViewOahHandler->
        getHandlerGroupsList ();

  if (alternateOahHandlerGroupsList.size ()) {
#ifdef TRACE_OAH
    if (gGlobalTraceOahGroup->getTraceOah ()) {
      list<S_oahGroup>::const_iterator
        iBegin = alternateOahHandlerGroupsList.begin (),
        iEnd   = alternateOahHandlerGroupsList.end (),
        i      = iBegin;

      for ( ; ; ) {
        S_oahGroup group = (*i);

        // handle the group
        const list<S_oahSubGroup>
          groupSubGroupsList =
            group->
              getSubGroupsList ();

        int groupSubGroupsListSize =
          groupSubGroupsList.size ();

        if (groupSubGroupsListSize) {
          list<S_oahSubGroup>::const_iterator
            iBegin = groupSubGroupsList.begin (),
            iEnd   = groupSubGroupsList.end (),
            i      = iBegin;
          for ( ; ; ) {
            S_oahSubGroup subGroup = (*i);

            s << subGroup << endl; // JMI

            // handle the subgroup
            string subGroupHeader =
              subGroup->getSubGroupHeader ();
            int subGroupHeaderSize =
              subGroup->getSubGroupHeader ().size ();

            if (subGroupHeaderSize) {
              if (! subGroup->getAtomsList ().size ()) {
                s <<
                  "FOO \"" << subGroupHeader << "\"";
              }
            }
            else {
              s << subGroup << endl; // JMI
            }

            if (++i == iEnd) break;
          } // for
        }

        if (++i == iEnd) break;
        s << ", ";
      } // for

      string resultString = s.str ();
      int    resultStringSize = resultString.size ();

      if (resultStringSize) {
        gLogOstream <<
          "The following 'alternate' view " <<
          singularOrPlural (
            resultStringSize, "subgroup", "subgroups") <<
          " are empty:" <<
          endl <<
          resultString <<
          endl;
      }
    }
#endif
  }
  */
