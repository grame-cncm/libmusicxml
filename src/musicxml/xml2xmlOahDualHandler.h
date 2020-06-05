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
class EXP xml2xmlOahDualHandler : public smartable
{
  public:

    // data types
    // ------------------------------------------------------

    enum oahDualHandlerKind {
        kUser,
        kInternal };

    static string OahHandlerKindAsString ();

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

    oahDualHandlerKind    getOahHandlerKind () const
                              { return fOahHandlerKind; }

    // oahHandlers
    S_oahHandler          getInternalOahHandler () const
                              { return fInternalOahHandler; }

    S_oahHandler          getUserOahHandler () const
                              { return fUserOahHandler; }

  public:

    // services
    // ------------------------------------------------------

    void                  initializeXml2xmlOahDualHandler (
                            string   executableName,
                            ostream& ios);

    void                  createUserGroups (
                            ostream& ios);

    void                  createSubGroupNamesToUserGroupsMap ();

    void                  createUserSubGroups (
                            ostream& ios);

    void                  createAtomNamesToUserSubGroupsMap ();

    void                  populateUserGroupsFromInteralGroups ();

  public:

    // print
    // ------------------------------------------------------

    void                  print (ostream& os) const;

    void                  printHelp (ostream& os) const;

  private:

    // fields
    // ------------------------------------------------------

    // oahHandlers
    oahDualHandlerKind    fOahHandlerKind;

    S_oahHandler          fInternalOahHandler;
    S_oahHandler          fUserOahHandler;

    S_oahHandler          fOahHandlerToBeUsed; // according to oahOahHandlerKind

    // user groups
    S_oahGroup            fApplicationUserGroup;

    S_oahGroup            fDocumentationUserGroup;

    S_oahGroup            fFilesUserGroup;

    S_oahGroup            fTraceUserGroup;
    S_oahGroup            fDisplayUserGroup;

    S_oahGroup            fLayoutUserGroup;

    S_oahGroup            fLanguagesUserGroup;

    S_oahGroup            fPartGroupsAndPartsUserGroup;
    S_oahGroup            fStavesUserGroup;
    S_oahGroup            fVoicesUserGroup;
    S_oahGroup            fMeasuresUserGroup;

    S_oahGroup            fClefsKeysAndTimesUserGroup;

    S_oahGroup            fNotesUserGroup;

    S_oahGroup            fDynamicsAndWedgesUserGroup;

    S_oahGroup            fChordsUserGroup;
    S_oahGroup            fTupletsUserGroup;

    S_oahGroup            fTranspositionUserGroup;

    S_oahGroup            fLyricsUserGroup;

    S_oahGroup            fHarmoniesUserGroup;
    S_oahGroup            fFiguredBassesUserGroup;

    S_oahGroup            fMidiUserGroup;

    S_oahGroup            fExitUserGroup;

    S_oahGroup            fExtraUserGroup;

    S_oahGroup            fOtherUserGroup; // should be empty JMI

    map<string, S_oahGroup>
                          fSubGroupNamesToUserGroupsMap;

    // user subgroups

    S_oahSubGroup         fInformationUserSubGroup;
    S_oahSubGroup         fExitUserSubGroup;

    S_oahSubGroup         fFilesUserSubGroup;

    S_oahSubGroup         fOtherUserSubGroup; // should be empty JMI

    map<string, S_oahSubGroup>
                          fAtomNamesToUserSubGroupsMap;
};
typedef SMARTP<xml2xmlOahDualHandler> S_xml2xmlOahDualHandler;
EXP ostream& operator<< (ostream& os, const S_xml2xmlOahDualHandler& elt);


}


#endif
