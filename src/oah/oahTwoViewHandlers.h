/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef ___oahTwoViewHandlers___
#define ___oahTwoViewHandlers___

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
class EXP oahTwoViewHandler : public smartable
{
  public:

    // data types
    // ------------------------------------------------------

    enum oahViewHandlerKind {
        kFullViewHandler,
        kAlternateViewHandler };

    static string oahViewHandlerKindAsString (
      oahViewHandlerKind viewHandlerKind);

/* pure virtual class
  public:

    // creation
    // ------------------------------------------------------
    static SMARTP<oahTwoViewHandler> create (
      string   twoViewHandlerName,
      string   executableName,
      string   twoViewAtomShortName,
      string   twoViewAtomLongName,
      ostream& os);
*/

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    oahTwoViewHandler (
      string   twoViewHandlerName,
      string   executableName,
      string   twoViewAtomShortName,
      string   twoViewAtomLongName,
      ostream& os);

    virtual ~oahTwoViewHandler ();

  public:

    // set and get
    // ------------------------------------------------------

    oahViewHandlerKind    getViewHandlerKind () const
                              { return fViewHandlerKind; }

    string                getTwoViewHandlerName () const
                              { return fTwoViewHandlerName; }

    // oahHandlers
    void                  setFullViewOahHandler (
                            S_oahHandler handler)
                              { fFullViewOahHandler = handler; }

    S_oahHandler          getFullViewOahHandler () const
                              { return fFullViewOahHandler; }

    void                  setAlternateViewOahHandler (
                            S_oahHandler handler)
                              { fAlternateViewOahHandler = handler; }

    S_oahHandler          getAlternateViewOahHandler () const
                              { return fAlternateViewOahHandler; }

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

    void                  printFullViewOahHandler (ostream& os) const;
    void                  printAlternateViewOahHandler (ostream& os) const;

    void                  print (ostream& os) const;

    void                  printHelp (ostream& os) const;

  protected:

    // protected services
    // ------------------------------------------------------

    virtual void          createTheTwoHandlers (ostream& os) = 0;

    void                  switchToTwoViewView ();

/*
    virtual void          createFullViewOahHandler (
                            ostream& os) = 0;

    virtual void          createAlternateViewOahHandler (
                            ostream& os) = 0 ;

    virtual void          createAlternateViewOahHandlerGroups (
                            ostream& os) = 0;
*/

    void                  populateAlternateViewHandlerFromFullViewHandler ();

    void                  handleSubGroupMapping (S_oahSubGroup subGroup);
    void                  handleAtomMapping (S_oahAtom atom);

    void                  checkMappingsUse ();

  protected:

    // fields
    // ------------------------------------------------------
    string                fTwoViewHandlerName;

    string                fExecutableName;

    // the twoView handler provides the options used internally by the translator,
    // organized by representations and passes,
    // while the alternate handler provides another view of the options, organized by topics
    oahViewHandlerKind    fViewHandlerKind;

    S_oahHandler          fFullViewOahHandler;
    S_oahHandler          fAlternateViewOahHandler;

    // the 'twoView' option names
    string                fTwoViewAtomShortName;
    string                fTwoViewAtomLongName;

    // the mappings
    map<string, S_oahGroup>
                          fSubGroupNamesToAlternateGroupsMap;
    map<string, S_oahSubGroup>
                          fAtomNamesToAlternateSubGroupsMap;

    // atoms not present in the 'alternate' view are put aside
    S_oahGroup            fPutAsideInAlternateViewGroup;
    S_oahSubGroup         fPutAsideInAlternateViewSubGroup;
    list<string>          fPutAsideInAlternateViewAtomNamesList;

  protected:

    // protected work fields
    // ------------------------------------------------------

    S_oahHandler          fOahHandlerToBeUsed; // according to '-twoView' option
};
typedef SMARTP<oahTwoViewHandler> S_oahTwoViewHandler;
EXP ostream& operator<< (ostream& os, const S_oahTwoViewHandler& elt);

/* JMI
//______________________________________________________________________________
class oahTwoViewHandlerTwoViewAtom : public oahAtom
/ *
  This atom name is trapped very early in:
    oahTwoViewHandler::applyOptionsAndArgumentsFromArgcAndArgv()
  and:
    oahTwoViewHandler::applyOptionsFromOptionsVector()

  It does nothing on its own, it is created only to show
  that this option exists
* /
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<oahTwoViewHandlerTwoViewAtom> create (
      string shortName,
      string longName,
      string description);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    oahTwoViewHandlerTwoViewAtom (
      string shortName,
      string longName,
      string description);

    virtual ~oahTwoViewHandlerTwoViewAtom ();

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
    void                  printAlternateHandler (ostream& os) const;

    void                  print (ostream& os) const override;

  private:

    // fields
    // ------------------------------------------------------

    int                   fTwoViewOptionsCounter;
};
typedef SMARTP<oahTwoViewHandlerTwoViewAtom> S_oahTwoViewHandlerTwoViewAtom;
EXP ostream& operator<< (ostream& os, const S_oahTwoViewHandlerTwoViewAtom& elt);
*/

}


#endif
