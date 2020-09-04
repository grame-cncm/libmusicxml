/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef ___xml2bmmlOahDualHandlers___
#define ___xml2bmmlOahDualHandlers___

#include "oahDualHandlers.h"


using namespace std;

namespace MusicXML2
{
/*
//_______________________________________________________________________________
// data types
enum oahDualHandlerViewKind {
  kViewInsider,
  kViewUser };

static string oahDualHandlerViewKindAsString (
  oahDualHandlerViewKind dualHandlerViewKind);
*/
//_______________________________________________________________________________
class EXP xml2bmmlOahDualHandler : public oahDualHandler
{
  public:

    // creation
    // ------------------------------------------------------
    static SMARTP<xml2bmmlOahDualHandler> create (
      string   executableName,
      ostream& os);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    xml2bmmlOahDualHandler (
      string   executableName,
      ostream& os);

    virtual ~xml2bmmlOahDualHandler ();

  public:

    // set and get
    // ------------------------------------------------------

  public:

    // public services
    // ------------------------------------------------------

    void                  checkOptionsAndArguments ();

    void                  enforceOahDualHandlerQuietness ();

  public:

    // print
    // ------------------------------------------------------

    string                asString () const;

    void                  print (ostream& os) const;

  private:

    // private services
    // ------------------------------------------------------

    // handlers creation
    void                  createTheTwoHandlers (
                            ostream& os);

    // user handler groups creation
    void                  createUserHandlerGroups (
                            ostream& os);

    void                  createOahUserGroup (
                            ostream& os);

    void                  createInformationsUserGroup (
                            ostream& os);

    void                  createWarningAndErrorsUserGroup (
                            ostream& os);

    void                  createFilesUserGroup (
                            ostream& os);

    void                  createInputUserGroup (
                            ostream& os);

    void                  createPresentationUserGroup (
                            ostream& os);

    void                  createPartsUserGroup (
                            ostream& os);

    void                  createStavesUserGroup (
                            ostream& os);

    void                  createVoicesUserGroup (
                            ostream& os);

    void                  createTranspositionsUserGroup (
                            ostream& os);

    void                  createClefsUserGroup (
                            ostream& os);

    void                  createKeysUserGroup (
                            ostream& os);

    void                  createTimesUserGroup (
                            ostream& os);

    void                  createRepeatsUserGroup (
                            ostream& os);

    void                  createMeasuresUserGroup (
                            ostream& os);

    void                  createRestsUserGroup (
                            ostream& os);

    void                  createNotesUserGroup (
                            ostream& os);

    void                  creatBeamsUserGroup (
                            ostream& os);

    void                  createArticulationsUserGroup (
                            ostream& os);

    void                  createOrnamentsUserGroup (
                            ostream& os);

    void                  createGraceNotesUserGroup (
                            ostream& os);

    void                  createChordsUserGroup (
                            ostream& os);

    void                  createTiesUserGroup (
                            ostream& os);

    void                  createSlursUserGroup (
                            ostream& os);

    void                  createLigaturesUserGroup (
                            ostream& os);

    void                  createDynamicsUserGroup (
                            ostream& os);

    void                  createWedgesUserGroup (
                            ostream& os);

    void                  createTupletsUserGroup (
                            ostream& os);

    void                  createLyricsUserGroup (
                            ostream& os);

    void                  createStringsUserGroup (
                            ostream& os);

    void                  createTablaturesUserGroup (
                            ostream& os);

    void                  createHarmoniesUserGroup (
                            ostream& os);

    void                  createFiguredBassesUserGroup (
                            ostream& os);

    void                  createOutputUserGroup (
                            ostream& os);
};
typedef SMARTP<xml2bmmlOahDualHandler> S_xml2bmmlOahDualHandler;
EXP ostream& operator<< (ostream& os, const S_xml2bmmlOahDualHandler& elt);


}


#endif
