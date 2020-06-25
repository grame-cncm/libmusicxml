/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef ___oahDualHandlers___
#define ___oahDualHandlers___

/*
#include "list"
#include "vector"
#include "map"
#include "set"

#include "smartpointer.h"
#include "rational.h"

#include "tree_browser.h"
#include "visitor.h"

#include "libmusicxml.h"  // for optionsVector

#include "msrMidi.h"

#include "utilities.h"

#include "msrBasicTypes.h"
#include "lpsrBasicTypes.h"

#include "setTraceOahIfDesired.h"
*/

#include "oahBasicTypes.h"


using namespace std;

namespace MusicXML2
{

//_______________________________________________________________________________
class EXP oahDualHandler : public smartable
{
/* pure virtual class
  public:

    // creation
    // ------------------------------------------------------
    static SMARTP<oahDualHandler> create (
      string   dualHandlerName,
      string   executableName,
      string   insiderAtomShortName,
      string   insiderAtomLongName,
      ostream& os);
*/

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    oahDualHandler (
      string   dualHandlerName,
      string   executableName,
      string   insiderAtomShortName,
      string   insiderAtomLongName,
      ostream& os);

    virtual ~oahDualHandler ();

  public:

    // set and get
    // ------------------------------------------------------

//    oahDualHandlerViewKind
//                          getOahHandlerViewKind () const
//                              { return fOahHandlerViewKind; }

    string                getDualHandlerName () const
                              { return fDualHandlerName; }

    // oahHandlers
    void                  setInsiderHandler (
                            S_oahHandler handler)
                              { fInsiderHandler = handler; }

    S_oahHandler          getInsiderHandler () const
                              { return fInsiderHandler; }

    void                  setUserHandler (
                            S_oahHandler handler)
                              { fUserHandler = handler; }

    S_oahHandler          getUserHandler () const
                              { return fUserHandler; }

  public:

    // public services
    // ------------------------------------------------------

    oahHandler::oahHelpOptionsHaveBeenUsedKind
                          applyOptionsAndArgumentsFromArgcAndArgv (
                            int   argc,
                            char* argv[]);

    oahHandler::oahHelpOptionsHaveBeenUsedKind
                          hangleOptionsFromOptionsVector (
                            string               fakeExecutableName,
                            const optionsVector& theOptionsVector);

  public:

    // print
    // ------------------------------------------------------

    string                commandLineAsSuppliedAsString () const;
    string                commandLineWithShortNamesAsString () const;
    string                commandLineWithLongNamesAsString () const;

    string                asString () const;

    void                  printInsiderHandler (ostream& os) const;
    void                  printUserHandler (ostream& os) const;

    void                  print (ostream& os) const;

    void                  printHelp (ostream& os) const;

  protected:

    // protected services
    // ------------------------------------------------------

    virtual void          createTheTwoHandlers (ostream& os) = 0;

    void                  switchToInsiderView ();

/*
    virtual void          createInsiderHandler (
                            ostream& os) = 0;

    virtual void          createUserHandler (
                            ostream& os) = 0 ;

    virtual void          createUserHandlerGroups (
                            ostream& os) = 0;
*/

    void                  populateUserHandlerFromInsiderHandler ();

    void                  handleSubGroupMapping (S_oahSubGroup subGroup);
    void                  handleAtomMapping (S_oahAtom atom);

    void                  checkMappingsUse ();

  protected:

    // fields
    // ------------------------------------------------------
    string                fDualHandlerName;

    string                fExecutableName;

    // the insider handler provides the options used internally by the translator,
    // organized by representations and passes,
    // while the user handler provides another view of the options, organized by topics
    S_oahHandler          fInsiderHandler;
    S_oahHandler          fUserHandler;

    // the 'insider' option names
    string                fInsiderAtomShortName;
    string                fInsiderAtomLongName;

    // the mappings
    map<string, S_oahGroup>
                          fSubGroupNamesToUserGroupsMap;
    map<string, S_oahSubGroup>
                          fAtomNamesToUserSubGroupsMap;

    // atoms not present in the 'user' view are put aside
    S_oahGroup            fPutAsideInUserViewGroup;
    S_oahSubGroup         fPutAsideInUserViewSubGroup;
    list<string>          fPutAsideInUserViewAtomNamesList;

  protected:

    // protected work fields
    // ------------------------------------------------------

    S_oahHandler          fOahHandlerToBeUsed; // according to '-insider' option
};
typedef SMARTP<oahDualHandler> S_oahDualHandler;
EXP ostream& operator<< (ostream& os, const S_oahDualHandler& elt);

//______________________________________________________________________________
class oahDualHandlerInsiderAtom : public oahAtom
/*
  This atom name is trapped very early in:
    oahDualHandler::applyOptionsAndArgumentsFromArgcAndArgv()
  and:
    oahDualHandler::hangleOptionsFromOptionsVector()

  It does nothing on its own, it is created only to show
  that this option exists
*/
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<oahDualHandlerInsiderAtom> create (
      string shortName,
      string longName,
      string description);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    oahDualHandlerInsiderAtom (
      string shortName,
      string longName,
      string description);

    virtual ~oahDualHandlerInsiderAtom ();

  public:

    // set and get
    // ------------------------------------------------------

  public:

    // services
    // ------------------------------------------------------

    S_oahValuedAtom       handleOptionUnderName (
                            string   optionName,
                            ostream& os);

  public:

    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v);
    virtual void          acceptOut (basevisitor* v);

    virtual void          browseData (basevisitor* v);

  public:

    // print
    // ------------------------------------------------------

    virtual string        asString () const;

    void                  printOutsiderHandler (ostream& os) const;
    void                  printuserHandler (ostream& os) const;

    void                  print (ostream& os) const;

  private:

    // fields
    // ------------------------------------------------------

    int                   fInsiderOptionsCounter;
};
typedef SMARTP<oahDualHandlerInsiderAtom> S_oahDualHandlerInsiderAtom;
EXP ostream& operator<< (ostream& os, const S_oahDualHandlerInsiderAtom& elt);


}


#endif
