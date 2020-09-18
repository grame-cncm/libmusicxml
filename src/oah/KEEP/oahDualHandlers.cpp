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

#include "oahDualHandlers.h"

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
S_oahDualHandler oahDualHandler::create (
  string   dualHandlerName,
  string   executableName,
  string   insiderAtomShortName,
  string   insiderAtomLongName,
  ostream& os)
{
  oahDualHandler* o = new
    oahDualHandler (
      dualHandlerName,
      executableName,
      insiderAtomShortName,
      insiderAtomLongName,
      os);
  assert(o!=0);

// JMI  o->createTheTwoHandlers (os);

  return o;
}
*/

oahDualHandler::oahDualHandler (
  string   dualHandlerName,
  string   executableName,
  string   insiderAtomShortName,
  string   insiderAtomLongName,
  ostream& os)
{
  fDualHandlerName = dualHandlerName;

#ifdef TRACE_OAH
#ifdef OAH_TESTS
    gLogOstream <<
      "Constructing OAH dual handler \"" << fDualHandlerName << "\"" <<
      endl;
#endif
#endif

  fExecutableName = executableName;

  fFullViewAtomShortName = insiderAtomShortName;
  fFullViewAtomLongName  = insiderAtomLongName;

  // createTheTwoHandlers() will be called in the derived classes 'create()' method
}

oahDualHandler::~oahDualHandler ()
{}

void oahDualHandler::switchToFullViewView ()
{
  fOahHandlerToBeUsed = fFullViewHandler;

#ifdef TRACE_OAH
#ifdef OAH_TESTS
  gLogOstream <<
    "Switching dual handler \"" <<
    fDualHandlerName <<
    "\" handler to \"" <<
    fOahHandlerToBeUsed->getHandlerHeader () <<
    "\" (insider view)" <<
    endl;
#endif
#endif

  // fFullViewOptionsCounter will be incremented in
  // oahDualHandlerFullViewAtom::handleOptionUnderName (),
  // since the 'insider' option will be handled anyway
}

void oahDualHandler::populateAlternateViewHandlerFromFullViewHandler ()
{
  /*
    The relative order of atoms found in the 'insider' view
    is retained in the 'user' view
  */

#ifdef TRACE_OAH
#ifdef OAH_TESTS
  gLogOstream <<
    "Populating the user handler from the insider handler for \"" << fDualHandlerName << "\"" <<
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
      fAlternateHandler);
  fPutAsideInAlternateViewGroup->
    setGroupHeaderIsToBeWritten (false);
  fAlternateHandler->
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

  // get the insider OAH handler groups list
  const list<S_oahGroup>
    insiderHandlerGroupsList =
      fFullViewHandler->
        getHandlerGroupsList ();

  // handle it
  if (insiderHandlerGroupsList.size ()) {
    list<S_oahGroup>::const_iterator
      iBegin = insiderHandlerGroupsList.begin (),
      iEnd   = insiderHandlerGroupsList.end (),
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
  printFullViewHandler (gLogOstream);

  gLogOstream <<
    endl << endl <<
    "*******************************************************************" <<
    endl << endl;

/* JMI
  printAlternateHandler (gLogOstream);

  fAlternateHandler->
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

void oahDualHandler::handleSubGroupMapping (S_oahSubGroup subGroup)
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

    // append subgroup to user group
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

void oahDualHandler::handleAtomMapping (S_oahAtom atom)
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

    // append atom to user subgroup
#ifdef TRACE_OAH
    if (gGlobalTraceOahGroup->getTraceOahDetails ()) {
      gLogOstream <<
        "+++ appending atom \"" <<
        atomNameToUse <<
        "\" to subgroup and registering it in the 'user' handler \"" <<
        subGroup->getSubGroupHeader () <<
        "\"" <<
        endl;
    }
#endif

    subGroup->
      appendAtomToSubGroup (atom);

    atom->
      registerAtomInHandler (fAlternateHandler);

    // remove atomNameToUse from the map
    it = fAtomNamesToAlternateSubGroupsMap.erase (it);
  }
  else {
    // atomNameToUse is not known in the map
    // place it in the 'put aside' user subgroup

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

void oahDualHandler::checkMappingsUse ()
{
#ifdef TRACE_OAH
  if (gGlobalTraceOahGroup->getTraceOah ()) {
    // print the known prefixes
    fAlternateHandler->
      printKnownPrefixes (gLogOstream);
  }
#endif

  // check subgroups mapping
  int subGroupNamesToAlternateGroupsMapSize =
    fSubGroupNamesToAlternateGroupsMap.size ();

  if (subGroupNamesToAlternateGroupsMapSize) {
#ifdef TRACE_OAH
    if (gGlobalTraceOahGroup->getTraceOah ()) {
      stringstream s;

      s <<
        "The following " <<
        singularOrPlural (
          subGroupNamesToAlternateGroupsMapSize, "subgroup name", "subgroup names") <<
        " could not be mapped to user groups: ";

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
  int atomNamesToAlternateGroupsMapSize =
    fAtomNamesToAlternateSubGroupsMap.size ();

  if (atomNamesToAlternateGroupsMapSize) {
#ifdef TRACE_OAH
    if (gGlobalTraceOahGroup->getTraceOah ()) {
      stringstream s;

      s <<
        "The following " <<
        singularOrPlural (
          atomNamesToAlternateGroupsMapSize, "atom name", "atom names") <<
        " could not be mapped to user subgroups: ";

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
  int putAsideInAlternateViewAtomNamesListSize =
    fPutAsideInAlternateViewAtomNamesList.size ();

  if (putAsideInAlternateViewAtomNamesListSize) {
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
          " have been left out of the mapping to user subgroups:" <<
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

oahHandler::oahHelpOptionsHaveBeenUsedKind oahDualHandler::applyOptionsAndArgumentsFromArgcAndArgv (
  int   argc,
  char* argv[])
{
#ifdef TRACE_OAH
  if (gGlobalTraceOahGroup->getTraceOah ()) {
    gLogOstream <<
      "==> oahDualHandler::applyOptionsAndArgumentsFromArgcAndArgv()" <<
      ", fDualHandlerName: \"" << fDualHandlerName << "\"" <<
      endl;
  }
#endif

  // should the insider or user oahHandler be used?
  if (argc >= 2) {
    string argument1 (argv [1]);

    if (
      argument1 == string ("-") + fFullViewAtomShortName
        ||
      argument1 == string ("-") + fFullViewAtomLongName
    ) {
      switchToFullViewView ();
    }
    else {
#ifdef TRACE_OAH
#ifdef OAH_TESTS
      gLogOstream <<
        "Keeping dual handler \"" <<
        fDualHandlerName <<
        "\" handler \"" <<
        fOahHandlerToBeUsed->getHandlerHeader () <<
        "\" (user view)" <<
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

oahHandler::oahHelpOptionsHaveBeenUsedKind oahDualHandler::applyOptionsFromOptionsVector (
  string               fakeExecutableName,
  const optionsVector& theOptionsVector)
{
#ifdef TRACE_OAH
  if (gGlobalTraceOahGroup->getTraceOah ()) {
    gLogOstream <<
      "==> oahDualHandler::applyOptionsFromOptionsVector" <<
      endl;
  }
#endif

  // should the insider or user oahHandler be used?
  if (theOptionsVector.size () >= 1) {
    string optionName1 (theOptionsVector [0].first);

    if (
      optionName1 == string ("-") + fFullViewAtomShortName
        ||
      optionName1 == string ("-") + fFullViewAtomLongName
    ) {
      switchToFullViewView ();
    }
    else {
#ifdef TRACE_OAH
#ifdef OAH_TESTS
      gLogOstream <<
        "==> oahDualHandler::applyOptionsFromOptionsVector(): remaining in user view" <<
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

string oahDualHandler::commandLineAsSuppliedAsString () const
{
  return
    fOahHandlerToBeUsed->
      commandLineAsSuppliedAsString ();
}

string oahDualHandler::commandLineWithShortNamesAsString () const
{
  return
    fOahHandlerToBeUsed->
      commandLineWithShortNamesAsString ();
}

string oahDualHandler::commandLineWithLongNamesAsString () const
{
  return
    fOahHandlerToBeUsed->
      commandLineWithLongNamesAsString ();
}

void oahDualHandler::printAllOahCommandLineValues (
  ostream& os) const
{
  fFullViewHandler-> printAllOahCommandLineValues (os);
  fAlternateHandler-> printAllOahCommandLineValues (os);
}

string oahDualHandler::asString () const
{
  return "oahDualHandlerFullViewAtom";
}

void oahDualHandler::printFullViewHandler (ostream& os) const
{
  os <<
    endl <<
    "------------ " <<
    "insiderHandler" <<
     " ------------" <<
   endl;

  gIndenter++;

  const list<S_oahGroup>
    insiderHandlerGroupsList =
      fFullViewHandler->
        getHandlerGroupsList ();

  if (insiderHandlerGroupsList.size ()) {
    os << endl;

    list<S_oahGroup>::const_iterator
      iBegin = insiderHandlerGroupsList.begin (),
      iEnd   = insiderHandlerGroupsList.end (),
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

void oahDualHandler::printAlternateHandler (ostream& os) const
{
  os <<
    endl <<
    "------------ " <<
    "userOahHandler" <<
    " ------------" <<
    endl;

  gIndenter++;

  const list<S_oahGroup>
    userOahHandlerGroupsList =
      fAlternateHandler->
        getHandlerGroupsList ();

  if (userOahHandlerGroupsList.size ()) {
    os << endl;

    list<S_oahGroup>::const_iterator
      iBegin = userOahHandlerGroupsList.begin (),
      iEnd   = userOahHandlerGroupsList.end (),
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

void oahDualHandler::print (ostream& os) const
{
  os <<
    "------------ " <<
    endl <<
    "DualHandler \"" << fDualHandlerName << "\":" <<
    ", fExecutableName: \"" << fExecutableName <<  "\":" <<
    ", fFullViewAtomShortName: \"" << fFullViewAtomShortName <<  "\":" <<
    ", fFullViewAtomLongName: \"" << fFullViewAtomLongName <<  "\":" <<
    endl <<
    " ------------" <<
    endl;

  gIndenter++;

  printFullViewHandler (os);

  os <<
    endl <<
    "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" <<
    endl << endl;

  printAlternateHandler (os);

  gIndenter--;

  os << endl;
}

void oahDualHandler::printHelp (ostream& os) const
{
  os <<
    "------------ " <<
    endl <<
    "DualHandler \"" << fDualHandlerName << "\":" <<
    ", fExecutableName: \"" << fExecutableName <<  "\":" <<
    ", fFullViewAtomShortName: \"" << fFullViewAtomShortName <<  "\":" <<
    ", fFullViewAtomLongName: \"" << fFullViewAtomLongName <<  "\":" <<
    endl <<
    " ------------" <<
    endl;

  gIndenter++;

  if (true) { // JMI
    os <<
      endl <<
      "------------ " <<
      "insiderHandler" <<
       " ------------" <<
     endl;

    gIndenter++;

    const list<S_oahGroup>
      insiderHandlerGroupsList =
        fFullViewHandler->
          getHandlerGroupsList ();

    if (insiderHandlerGroupsList.size ()) {
      os << endl;

      list<S_oahGroup>::const_iterator
        iBegin = insiderHandlerGroupsList.begin (),
        iEnd   = insiderHandlerGroupsList.end (),
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
      "------------ " <<
      "userOahHandler" <<
      " ------------" <<
      endl;

    gIndenter++;

    const list<S_oahGroup>
      userOahHandlerGroupsList =
        fAlternateHandler->
          getHandlerGroupsList ();

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
  }

  gIndenter--;

  os << endl;
}

//______________________________________________________________________________
S_oahDualHandlerFullViewAtom oahDualHandlerFullViewAtom::create (
  string shortName,
  string longName,
  string description)
{
  oahDualHandlerFullViewAtom* o = new
    oahDualHandlerFullViewAtom (
      shortName,
      longName,
      description);
  assert(o!=0);
  return o;
}

oahDualHandlerFullViewAtom::oahDualHandlerFullViewAtom (
  string shortName,
  string longName,
  string description)
  : oahAtom (
      shortName,
      longName,
      description,
      kElementValueExpectedNo)
{
  fFullViewOptionsCounter = 0;
}

oahDualHandlerFullViewAtom::~oahDualHandlerFullViewAtom ()
{}

S_oahValuedAtom oahDualHandlerFullViewAtom::handleOptionUnderName (
  string   optionName,
  ostream& os)
{
#ifdef TRACE_OAH
  if (gGlobalTraceOahGroup->getTraceOah ()) {
    gLogOstream <<
      "==> option '" << optionName << "' is a oahDualHandlerFullViewAtom" <<
      endl;
  }
#endif

  fFullViewOptionsCounter++;

  /*
    The initial 'insider' option is handled even though
    it has been trapped very early in:
     oahDualHandler::applyOptionsAndArgumentsFromArgcAndArgv()
    and:
      oahDualHandler::applyOptionsFromOptionsVector()
  */

  if (fFullViewOptionsCounter > 1) {
    stringstream s;

    s <<
      "option \"" << fShortName << "\" should be the first one";

    oahError (s.str ());
  }

  // no option value is needed
  return nullptr;
}

void oahDualHandlerFullViewAtom::acceptIn (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gGlobalOahOahGroup->fTraceOahVisitors) {
    gLogOstream <<
      "% ==> oahDualHandlerFullViewAtom::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_oahDualHandlerFullViewAtom>*
    p =
      dynamic_cast<visitor<S_oahDualHandlerFullViewAtom>*> (v)) {
        S_oahDualHandlerFullViewAtom elem = this;

#ifdef TRACE_OAH
        if (gGlobalOahOahGroup->fTraceOahVisitors) {
          gLogOstream <<
            "% ==> Launching oahDualHandlerFullViewAtom::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void oahDualHandlerFullViewAtom::acceptOut (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gGlobalOahOahGroup->fTraceOahVisitors) {
    gLogOstream <<
      "% ==> oahDualHandlerFullViewAtom::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_oahDualHandlerFullViewAtom>*
    p =
      dynamic_cast<visitor<S_oahDualHandlerFullViewAtom>*> (v)) {
        S_oahDualHandlerFullViewAtom elem = this;

#ifdef TRACE_OAH
        if (gGlobalOahOahGroup->fTraceOahVisitors) {
          gLogOstream <<
            "% ==> Launching oahDualHandlerFullViewAtom::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void oahDualHandlerFullViewAtom::browseData (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gGlobalOahOahGroup->fTraceOahVisitors) {
    gLogOstream <<
      "% ==> oahDualHandlerFullViewAtom::browseData ()" <<
      endl;
  }
#endif
}

string oahDualHandlerFullViewAtom::asString () const
{
  return "oahDualHandlerFullViewAtom";
}

void oahDualHandlerFullViewAtom::print (ostream& os) const
{
  os <<
    "DualHandlerFullViewAtom:" <<
    endl;
}

ostream& operator<< (ostream& os, const S_oahDualHandlerFullViewAtom& elt)
{
  elt->print (os);
  return os;
}


}


/* JMI
  // display the empty resulting user subgroups if any
  const list<S_oahGroup>
    userOahHandlerGroupsList =
      fAlternateHandler->
        getHandlerGroupsList ();

  if (userOahHandlerGroupsList.size ()) {
#ifdef TRACE_OAH
    if (gGlobalTraceOahGroup->getTraceOah ()) {
      list<S_oahGroup>::const_iterator
        iBegin = userOahHandlerGroupsList.begin (),
        iEnd   = userOahHandlerGroupsList.end (),
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
          "The following 'user' view " <<
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
