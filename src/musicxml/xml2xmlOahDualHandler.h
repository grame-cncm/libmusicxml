/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef ___xml2xmlOahDualHandlers___
#define ___xml2xmlOahDualHandlers___

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

#include "oahElements.h"

using namespace std;

namespace MusicXML2
{
//_______________________________________________________________________________
// data types
enum oahDualHandlerViewKind {
  kViewInsider,
  kViewUser };

static string oahDualHandlerViewKindAsString (
  oahDualHandlerViewKind dualHandlerViewKind);

//_______________________________________________________________________________
class EXP xml2xmlOahDualHandler : public smartable
{
  public:

    // creation
    // ------------------------------------------------------
    static SMARTP<xml2xmlOahDualHandler> create (
      string   executableName,
      ostream& ios);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    xml2xmlOahDualHandler (
      string   executableName,
      ostream& ios);

    virtual ~xml2xmlOahDualHandler ();

  public:

    // set and get
    // ------------------------------------------------------

    oahDualHandlerViewKind
                          getOahHandlerViewKind () const
                              { return fOahHandlerViewKind; }

    // oahHandlers
    S_oahHandler          getInsiderOahHandler () const
                              { return fInsiderOahHandler; }

    S_oahHandler          getUserOahHandler () const
                              { return fUserOahHandler; }

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

    void                  enforceOahHandlerQuietness ();

  public:

    // print
    // ------------------------------------------------------

    string                commandLineAsSuppliedAsString () const;
    string                commandLineWithShortNamesAsString () const;
    string                commandLineWithLongNamesAsString () const;

    void                  print (ostream& os) const;

    void                  printHelp (ostream& os) const;

  private:

    // private services
    // ------------------------------------------------------

    void                  initializeXml2xmlOahDualHandler (
                            string   executableName,
                            ostream& ios);

    void                  createInformationsUserGroup (
                            ostream& ios);

    void                  createWarningAndErrorsUserGroup (
                            ostream& ios);

    void                  createFilesUserGroup (
                            ostream& ios);

    void                  createPresentationUserGroup (
                            ostream& ios);

    void                  createPartsUserGroup (
                            ostream& ios);

    void                  createStavesUserGroup (
                            ostream& ios);

    void                  createVoicesUserGroup (
                            ostream& ios);

    void                  createTranspositionsUserGroup (
                            ostream& ios);

    void                  createClefsUserGroup (
                            ostream& ios);

    void                  createKeysUserGroup (
                            ostream& ios);

    void                  createTimesUserGroup (
                            ostream& ios);

    void                  createRepeatsUserGroup (
                            ostream& ios);

    void                  createMeasuresUserGroup (
                            ostream& ios);

    void                  createRestsUserGroup (
                            ostream& ios);

    void                  createNotesUserGroup (
                            ostream& ios);

    void                  createArticulationsUserGroup (
                            ostream& ios);

    void                  createOrnamentsUserGroup (
                            ostream& ios);

    void                  createGraceNotesUserGroup (
                            ostream& ios);

    void                  createChordsUserGroup (
                            ostream& ios);

    void                  createTiesAndSlursUserGroup (
                            ostream& ios);

    void                  createDynamicsAndWedgesUserGroup (
                            ostream& ios);

    void                  createTupletsUserGroup (
                            ostream& ios);

    void                  createLyricsUserGroup (
                            ostream& ios);

    void                  createHarmoniesUserGroup (
                            ostream& ios);

    void                  createFiguredBassesUserGroup (
                            ostream& ios);

    void                  createOutputGenerationUserGroup (
                            ostream& ios);

    void                  createInsiderUserGroup (
                            ostream& ios);

    void                  populateUserGroupsFromInsiderGroups ();

  protected:

    // fields
    // ------------------------------------------------------

    // oahHandlers

    oahDualHandlerViewKind
                          fOahHandlerViewKind;

    // the insider handler provides the options used internally by the translator
    S_oahHandler          fInsiderOahHandler;

    // the user handler provides a user view of the options, organized by topics
    S_oahHandler          fUserOahHandler;

    // the 'insider' option names
    string                fInsiderAtomShortName;
    string                fInsiderAtomLongName;

    // the 'insider' user group is not shown
    S_oahGroup            fInsiderUserGroup;

    // the 'insider' user subgroup is a catchall
    S_oahSubGroup         fInsiderUserSubGroup;

    // the mappings
    map<string, S_oahGroup>
                          fSubGroupNamesToUserGroupsMap;
    map<string, S_oahSubGroup>
                          fAtomNamesToUserSubGroupsMap;

  private:

    // work fields
    // ------------------------------------------------------

//    S_oahHandler          fOahHandlerToBeUsed; // according to oahOahHandlerKind
};
typedef SMARTP<xml2xmlOahDualHandler> S_xml2xmlOahDualHandler;
EXP ostream& operator<< (ostream& os, const S_xml2xmlOahDualHandler& elt);

//______________________________________________________________________________
class oahDualHandlerViewKindAtom : public oahValuedAtom
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<oahDualHandlerViewKindAtom> create (
      string             shortName,
      string             longName,
      string             description,
      string             valueSpecification,
      string             variableName,
      oahDualHandlerViewKind&
                         oahDualHandlerViewKindVariable);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    oahDualHandlerViewKindAtom (
      string             shortName,
      string             longName,
      string             description,
      string             valueSpecification,
      string             variableName,
      oahDualHandlerViewKind&
                         oahDualHandlerViewKindVariable);

    virtual ~oahDualHandlerViewKindAtom ();

  public:

    // set and get
    // ------------------------------------------------------

    void                  setoahDualHandlerViewKindVariable (
                            oahDualHandlerViewKind value)
                              {
                                foahDualHandlerViewKindVariable = value;
                              }

  public:

    // services
    // ------------------------------------------------------

    S_oahValuedAtom       handleOptionUnderName (
                            string   optionName,
                            ostream& os);

    void                  handleValue (
                            string   theString,
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

    string                asShortNamedOptionString () const;
    string                asActualLongNamedOptionString () const;

    void                  print (ostream& os) const;

  private:

    // fields
    // ------------------------------------------------------

    oahDualHandlerViewKind&
                          foahDualHandlerViewKindVariable;
};
typedef SMARTP<oahDualHandlerViewKindAtom> S_oahDualHandlerViewKindAtom;
EXP ostream& operator<< (ostream& os, const S_oahDualHandlerViewKindAtom& elt);


}


#endif
