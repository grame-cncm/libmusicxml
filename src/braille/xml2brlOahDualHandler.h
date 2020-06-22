/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef ___xml2brlOahDualHandlers___
#define ___xml2brlOahDualHandlers___

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
class EXP xml2brlOahDualHandler : public oahDualHandler
{
  public:

    // creation
    // ------------------------------------------------------
    static SMARTP<xml2brlOahDualHandler> create (
      string   executableName,
      ostream& os);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    xml2brlOahDualHandler (
      string   executableName,
      ostream& os);

    virtual ~xml2brlOahDualHandler ();

  public:

    // set and get
    // ------------------------------------------------------

  public:

    // public services
    // ------------------------------------------------------

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

    void                  createFilesUserGroup (
                            ostream& os);

    void                  createInformationsUserGroup (
                            ostream& os);

    void                  createWarningAndErrorsUserGroup (
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

    void                  createArticulationsUserGroup (
                            ostream& os);

    void                  createOrnamentsUserGroup (
                            ostream& os);

    void                  createGraceNotesUserGroup (
                            ostream& os);

    void                  createChordsUserGroup (
                            ostream& os);

    void                  createTiesAndSlursUserGroup (
                            ostream& os);

    void                  createDynamicsAndWedgesUserGroup (
                            ostream& os);

    void                  createTupletsUserGroup (
                            ostream& os);

    void                  createLyricsUserGroup (
                            ostream& os);

    void                  createHarmoniesUserGroup (
                            ostream& os);

    void                  createFiguredBassesUserGroup (
                            ostream& os);

    void                  createOutputUserGroup (
                            ostream& os);

    void                  createInsiderUserGroup (
                            ostream& os);

  protected:

    // fields
    // ------------------------------------------------------

  private:

    // work fields
    // ------------------------------------------------------

    // it would be way too cumbersome for oahDualHandler to be a template class,
    // so we double its two variable here
    S_xml2brlInsiderOahHandler
                          fXml2brlInsiderHandler;
    S_xml2brlInsiderOahHandler
                          fXml2brlUserHandler;
};
typedef SMARTP<xml2brlOahDualHandler> S_xml2brlOahDualHandler;
EXP ostream& operator<< (ostream& os, const S_xml2brlOahDualHandler& elt);


}


#endif
