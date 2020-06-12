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
class EXP xml2xmlOahDualHandler : public oahDualHandler
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

  public:

    // public services
    // ------------------------------------------------------

  public:

    // print
    // ------------------------------------------------------

    string                asString () const;

    void                  print (ostream& os) const;

    void                  printHelp (ostream& os) const;

  private:

    // private services
    // ------------------------------------------------------

    // handlers creation
    void                  createInsiderHandler (
                            ostream& ios);

    void                  createUserHandler (
                            ostream& ios);

    // user handler groups creation
    void                  createUserHandlerGroups (
                            ostream& ios);

    void                  createFilesUserGroup (
                            ostream& ios);

    void                  createInformationsUserGroup (
                            ostream& ios);

    void                  createWarningAndErrorsUserGroup (
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

  protected:

    // fields
    // ------------------------------------------------------

  private:

    // work fields
    // ------------------------------------------------------

};
typedef SMARTP<xml2xmlOahDualHandler> S_xml2xmlOahDualHandler;
EXP ostream& operator<< (ostream& os, const S_xml2xmlOahDualHandler& elt);


}


#endif
