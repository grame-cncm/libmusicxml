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
    void                  setFullViewHandler (
                            S_oahHandler handler)
                              { fFullViewHandler = handler; }

    S_oahHandler          getFullViewHandler () const
                              { return fFullViewHandler; }

    void                  setAlternateHandler (
                            S_oahHandler handler)
                              { fAlternateHandler = handler; }

    S_oahHandler          getAlternateHandler () const
                              { return fAlternateHandler; }

  public:

    // public services
    // ------------------------------------------------------

    oahHandler::oahHelpOptionsHaveBeenUsedKind
                          applyOptionsAndArgumentsFromArgcAndArgv (
                            int   argc,
                            char* argv[]);

    oahHandler::oahHelpOptionsHaveBeenUsedKind
                          applyOptionsFromOptionsVector (
                            string               fakeExecutableName,
                            const optionsVector& theOptionsVector);

  public:

    // print
    // ------------------------------------------------------

    string                commandLineAsSuppliedAsString () const;
    string                commandLineWithShortNamesAsString () const;
    string                commandLineWithLongNamesAsString () const;

    void                  printAllOahCommandLineValues (
                            ostream& os) const;

    string                asString () const;

    void                  printFullViewHandler (ostream& os) const;
    void                  printAlternateHandler (ostream& os) const;

    void                  print (ostream& os) const;

    void                  printHelp (ostream& os) const;

  protected:

    // protected services
    // ------------------------------------------------------

    virtual void          createTheTwoHandlers (ostream& os) = 0;

    void                  switchToFullViewView ();

/*
    virtual void          createFullViewHandler (
                            ostream& os) = 0;

    virtual void          createAlternateHandler (
                            ostream& os) = 0 ;

    virtual void          createAlternateHandlerGroups (
                            ostream& os) = 0;
*/

    void                  populateAlternateViewHandlerFromFullViewHandler ();

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
    S_oahHandler          fFullViewHandler;
    S_oahHandler          fAlternateHandler;

    // the 'insider' option names
    string                fFullViewAtomShortName;
    string                fFullViewAtomLongName;

    // the mappings
    map<string, S_oahGroup>
                          fSubGroupNamesToAlternateGroupsMap;
    map<string, S_oahSubGroup>
                          fAtomNamesToAlternateSubGroupsMap;

    // atoms not present in the 'user' view are put aside
    S_oahGroup            fPutAsideInAlternateViewGroup;
    S_oahSubGroup         fPutAsideInAlternateViewSubGroup;
    list<string>          fPutAsideInAlternateViewAtomNamesList;

  protected:

    // protected work fields
    // ------------------------------------------------------

    S_oahHandler          fOahHandlerToBeUsed; // according to '-insider' option
};
typedef SMARTP<oahDualHandler> S_oahDualHandler;
EXP ostream& operator<< (ostream& os, const S_oahDualHandler& elt);

//______________________________________________________________________________
class oahDualHandlerFullViewAtom : public oahAtom
/*
  This atom name is trapped very early in:
    oahDualHandler::applyOptionsAndArgumentsFromArgcAndArgv()
  and:
    oahDualHandler::applyOptionsFromOptionsVector()

  It does nothing on its own, it is created only to show
  that this option exists
*/
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<oahDualHandlerFullViewAtom> create (
      string shortName,
      string longName,
      string description);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    oahDualHandlerFullViewAtom (
      string shortName,
      string longName,
      string description);

    virtual ~oahDualHandlerFullViewAtom ();

  public:

    // set and get
    // ------------------------------------------------------

  public:

    // services
    // ------------------------------------------------------

    S_oahValuedAtom       handleOptionUnderName (
                            string   optionName,
                            ostream& os) override;

  public:

    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v) override;
    virtual void          acceptOut (basevisitor* v) override;

    virtual void          browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    virtual string        asString () const;

    void                  printOutsiderHandler (ostream& os) const;
    void                  printuserHandler (ostream& os) const;

    void                  print (ostream& os) const override;

  private:

    // fields
    // ------------------------------------------------------

    int                   fFullViewOptionsCounter;
};
typedef SMARTP<oahDualHandlerFullViewAtom> S_oahDualHandlerFullViewAtom;
EXP ostream& operator<< (ostream& os, const S_oahDualHandlerFullViewAtom& elt);


}


#endif
