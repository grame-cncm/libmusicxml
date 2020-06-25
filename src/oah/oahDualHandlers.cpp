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
  if (true) { // JMI
    gLogOstream <<
      "Constructing OAH dual handler \"" << fDualHandlerName << "\"" <<
      endl;
  }
#endif

  fExecutableName = executableName;

  fInsiderAtomShortName = insiderAtomShortName;
  fInsiderAtomLongName  = insiderAtomLongName;

  // createTheTwoHandlers() will be called in the derived classes 'create()' method
}

oahDualHandler::~oahDualHandler ()
{}

void oahDualHandler::switchToInsiderView ()
{
  fOahHandlerToBeUsed = fInsiderHandler;

#ifdef TRACE_OAH
  if (true) { // JMI
    gLogOstream <<
      "Switching dual handler \"" <<
      fDualHandlerName <<
      "\" handler to \"" <<
      fOahHandlerToBeUsed->getHandlerHeader () <<
      "\"" <<
      endl;
  }
#endif

  // fInsiderOptionsCounter will be incremented in
  // oahDualHandlerInsiderAtom::handleOptionUnderName (),
  // since the 'insider' option will be handled anyway
}

void oahDualHandler::populateUserHandlerFromInsiderHandler ()
{
  /*
    The relative order of atoms found in the 'insider' view
    is retained in the 'user' view
  */

//  bool saveTraceOah = gTraceOah->fTraceOah;
//  gTraceOah->fTraceOah = true; // JMI, TESTS

#ifdef TRACE_OAH
  if (true) { // JMI
    gLogOstream <<
      "Populating the user handler from the insider handler for \"" << fDualHandlerName << "\"" <<
      endl;
  }
#endif

  // create the 'put aside' group
  fPutAsideInUserViewGroup =
    oahGroup::create (
      "Put aside group",
      "hput-aside-group",
      "",
      "",
      kElementVisibilityNone,
      fUserHandler);
  fPutAsideInUserViewGroup->
    setGroupHeaderIsToBeWritten (false);
  fUserHandler->
    appendGroupToHandler (fPutAsideInUserViewGroup);

  // create the 'put aside' subgroup
  fPutAsideInUserViewSubGroup =
    oahSubGroup::create (
      "Put aside",
      "hput-aside",
      "",
      "",
      kElementVisibilityWhole,
      fPutAsideInUserViewGroup);
  fPutAsideInUserViewGroup->
    appendSubGroupToGroup (fPutAsideInUserViewSubGroup);

  // get the insider OAH handler groups list
  const list<S_oahGroup>
    insiderHandlerGroupsList =
      fInsiderHandler->
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
  if (gTraceOah->fTraceOah && ! gGeneralOah->fQuiet) {
// JMI  if (true || (gTraceOah->fTraceOah && ! gGeneralOah->fQuiet)) {
    printInsiderHandler (gLogOstream);

    gLogOstream <<
      endl << endl <<
      "*******************************************************************" <<
      endl << endl;

/* JMI
    printUserHandler (gLogOstream);

    fUserHandler->
      print (
        gLogOstream);

    gLogOstream <<
      endl << endl <<
      "*******************************************************************" <<
      endl << endl;
*/
  }
#endif

//  gTraceOah->fTraceOah = saveTraceOah;
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

  // is nameToUse known in fSubGroupNamesToUserGroupsMap?
  map<string, S_oahGroup>::const_iterator
    it =
      fSubGroupNamesToUserGroupsMap.find (
        subGroupNameToUse);

  if (it != fSubGroupNamesToUserGroupsMap.end ()) {
    // subGroupNameToUse is known in the map
    S_oahGroup group = (*it).second;

    // append subgroup to user group
#ifdef TRACE_OAH
    if (gTraceOah->fTraceOahDetails) {
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
    it = fSubGroupNamesToUserGroupsMap.erase (it);
  }
  else {
    // subGroupNameToUse is not known in the map

    // are there atoms from this subgroup in fAtomNamesToUserSubGroupsMap?

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

  // is atomNameToUse known in fAtomNamesToUserSubGroupsMap?
  map<string, S_oahSubGroup>::const_iterator
    it =
      fAtomNamesToUserSubGroupsMap.find (
        atomNameToUse);

  if (it != fAtomNamesToUserSubGroupsMap.end ()) {
    // atomNameToUse is known in the map
    S_oahSubGroup subGroup = (*it).second;

    // append atom to user subgroup
#ifdef TRACE_OAH
    if (gTraceOah->fTraceOahDetails) {
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
      registerAtomInHandler (fUserHandler);

    // remove atomNameToUse from the map
    it = fAtomNamesToUserSubGroupsMap.erase (it);
  }
  else {
    // atomNameToUse is not known in the map
    // place it in the 'put aside' user subgroup

#ifdef TRACE_OAH
    if (gTraceOah->fTraceOahDetails) {
      gLogOstream <<
        "+++ appending atom \"" <<
        atomNameToUse <<
        "\" to subgroup \"" <<
        fPutAsideInUserViewSubGroup->getSubGroupHeader () <<
        "\"" <<
        endl;
    }
#endif

    fPutAsideInUserViewSubGroup->
      appendAtomToSubGroup (atom);
    fPutAsideInUserViewAtomNamesList.push_back (
      atomNameToUse);
  }
}

void oahDualHandler::checkMappingsUse ()
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceOah) {
    // print the known prefixes
    fUserHandler->
      printKnownPrefixes (gLogOstream);
  }
#endif

  // check subgroups mapping
  int subGroupNamesToUserGroupsMapSize =
    fSubGroupNamesToUserGroupsMap.size ();

  if (subGroupNamesToUserGroupsMapSize) {
#ifdef TRACE_OAH
    if (gTraceOah->fTraceOah) {
      stringstream s;

      s <<
        "The following " <<
        singularOrPlural (
          subGroupNamesToUserGroupsMapSize, "subgroup name", "subgroup names") <<
        " could not be mapped to user groups: ";

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

      gLogOstream << s.str () << endl;
  //    oahError (s.str ());
    }
#endif
  }

  // check atoms mapping
  int atomNamesToUserGroupsMapSize =
    fAtomNamesToUserSubGroupsMap.size ();

  if (atomNamesToUserGroupsMapSize) {
#ifdef TRACE_OAH
    if (gTraceOah->fTraceOah) {
      stringstream s;

      s <<
        "The following " <<
        singularOrPlural (
          atomNamesToUserGroupsMapSize, "atom name", "atom names") <<
        " could not be mapped to user subgroups: ";

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

      gLogOstream << s.str () << endl;
    //    oahError (s.str ());
    }
#endif
  }

  // display the atoms that have been put aside
  int putAsideInUserViewAtomNamesListSize =
    fPutAsideInUserViewAtomNamesList.size ();

  if (putAsideInUserViewAtomNamesListSize) {
#ifdef TRACE_OAH
    if (gTraceOah->fTraceOah) {
      stringstream s;

      // build the list of atom names to be output
      list<string> atomNamesToBeOutputList;

      for (
        list<string>::const_iterator i = fPutAsideInUserViewAtomNamesList.begin ();
        i != fPutAsideInUserViewAtomNamesList.end ();
        i++
      ) {
        // handle the atom name
        string atomName = (*i);

        bool
          shouldTheAtomBeOutput =
            false // JMI TESTS
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
  bool saveTraceOah = gTraceOah->fTraceOah;
  gTraceOah->fTraceOah = true; // JMI, TESTS

#ifdef TRACE_OAH
  if (gTraceOah->fTraceOah) {
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
    argument1 == string ("-") + fInsiderAtomShortName // "insider"
      ||
    argument1 == string ("-") + fInsiderAtomLongName
    ) {
      switchToInsiderView ();
    }
    else {
#ifdef TRACE_OAH
      if (gTraceOah->fTraceOah) {
        gLogOstream <<
          "==> oahDualHandler::applyOptionsAndArgumentsFromArgcAndArgv(): remaining in user view" <<
          endl;
      }
#endif
    }
  }

  gTraceOah->fTraceOah = saveTraceOah;

  oahHandler::oahHelpOptionsHaveBeenUsedKind
    result =
      fOahHandlerToBeUsed->
        applyOptionsAndArgumentsFromArgcAndArgv (
          argc,
          argv);

  // check the 'user' handler's options and arguments
  fUserHandler->
    checkOptionsAndArguments ();

  return result;
}

oahHandler::oahHelpOptionsHaveBeenUsedKind oahDualHandler::hangleOptionsFromOptionsVector (
  string               fakeExecutableName,
  const optionsVector& theOptionsVector)
{
//  bool saveTraceOah = gTraceOah->fTraceOah;
//  gTraceOah->fTraceOah = true; // JMI, TESTS

#ifdef TRACE_OAH
  if (gTraceOah->fTraceOah) {
    gLogOstream <<
      "==> oahDualHandler::hangleOptionsFromOptionsVector" <<
      endl;
  }
#endif

  // should the insider or user oahHandler be used?
  if (theOptionsVector.size () >= 1) {
    string optionName1 (theOptionsVector [0].first);

    if (
      optionName1 == string ("-") + fInsiderAtomShortName
        ||
      optionName1 == string ("-") + fInsiderAtomLongName
    ) {
      switchToInsiderView ();
    }
    else {
#ifdef TRACE_OAH
      if (gTraceOah->fTraceOah) {
        gLogOstream <<
          "==> oahDualHandler::hangleOptionsFromOptionsVector(): remaining in user view" <<
          endl;
      }
#endif
    }
  }

//  gTraceOah->fTraceOah = saveTraceOah;

  return
    fOahHandlerToBeUsed->
      hangleOptionsFromOptionsVector (
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

string oahDualHandler::asString () const
{
  return "oahDualHandlerInsiderAtom";
}

void oahDualHandler::printInsiderHandler (ostream& os) const
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
      fInsiderHandler->
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

void oahDualHandler::printUserHandler (ostream& os) const
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
      fUserHandler->
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
    ", fInsiderAtomShortName: \"" << fInsiderAtomShortName <<  "\":" <<
    ", fInsiderAtomLongName: \"" << fInsiderAtomLongName <<  "\":" <<
    endl <<
    " ------------" <<
    endl;

  gIndenter++;

  printInsiderHandler (os);

  os <<
    endl <<
    "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" <<
    endl << endl;

  printUserHandler (os);

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
    ", fInsiderAtomShortName: \"" << fInsiderAtomShortName <<  "\":" <<
    ", fInsiderAtomLongName: \"" << fInsiderAtomLongName <<  "\":" <<
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
        fInsiderHandler->
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
        fUserHandler->
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
S_oahDualHandlerInsiderAtom oahDualHandlerInsiderAtom::create (
  string shortName,
  string longName,
  string description)
{
  oahDualHandlerInsiderAtom* o = new
    oahDualHandlerInsiderAtom (
      shortName,
      longName,
      description);
  assert(o!=0);
  return o;
}

oahDualHandlerInsiderAtom::oahDualHandlerInsiderAtom (
  string shortName,
  string longName,
  string description)
  : oahAtom (
      shortName,
      longName,
      description,
      kElementValueExpectedNo)
{
  fInsiderOptionsCounter = 0;
}

oahDualHandlerInsiderAtom::~oahDualHandlerInsiderAtom ()
{}

S_oahValuedAtom oahDualHandlerInsiderAtom::handleOptionUnderName (
  string   optionName,
  ostream& os)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceOah) {
    gLogOstream <<
      "==> option '" << optionName << "' is a oahDualHandlerInsiderAtom" <<
      endl;
  }
#endif

  fInsiderOptionsCounter++;

  /*
    The initial 'insider' option is handled even though
    it has been trapped very early in:
     oahDualHandler::applyOptionsAndArgumentsFromArgcAndArgv()
    and:
      oahDualHandler::hangleOptionsFromOptionsVector()
  */

  if (fInsiderOptionsCounter > 1) {
    stringstream s;

    s <<
      "option \"" << fShortName << "\" should be the first one";

    oahError (s.str ());
  }

  // no option value is needed
  return nullptr;
}

void oahDualHandlerInsiderAtom::acceptIn (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    gLogOstream <<
      "% ==> oahDualHandlerInsiderAtom::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_oahDualHandlerInsiderAtom>*
    p =
      dynamic_cast<visitor<S_oahDualHandlerInsiderAtom>*> (v)) {
        S_oahDualHandlerInsiderAtom elem = this;

#ifdef TRACE_OAH
        if (gOahOah->fTraceOahVisitors) {
          gLogOstream <<
            "% ==> Launching oahDualHandlerInsiderAtom::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void oahDualHandlerInsiderAtom::acceptOut (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    gLogOstream <<
      "% ==> oahDualHandlerInsiderAtom::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_oahDualHandlerInsiderAtom>*
    p =
      dynamic_cast<visitor<S_oahDualHandlerInsiderAtom>*> (v)) {
        S_oahDualHandlerInsiderAtom elem = this;

#ifdef TRACE_OAH
        if (gOahOah->fTraceOahVisitors) {
          gLogOstream <<
            "% ==> Launching oahDualHandlerInsiderAtom::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void oahDualHandlerInsiderAtom::browseData (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    gLogOstream <<
      "% ==> oahDualHandlerInsiderAtom::browseData ()" <<
      endl;
  }
#endif
}

string oahDualHandlerInsiderAtom::asString () const
{
  return "oahDualHandlerInsiderAtom";
}

void oahDualHandlerInsiderAtom::print (ostream& os) const
{
  os <<
    "DualHandlerInsiderAtom:" <<
    endl;
}

ostream& operator<< (ostream& os, const S_oahDualHandlerInsiderAtom& elt)
{
  elt->print (os);
  return os;
}


}


/* JMI
  // display the empty resulting user subgroups if any
  const list<S_oahGroup>
    userOahHandlerGroupsList =
      fUserHandler->
        getHandlerGroupsList ();

  if (userOahHandlerGroupsList.size ()) {
#ifdef TRACE_OAH
    if (gTraceOah->fTraceOah) {
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
