/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef ___oahRegularHandlers___
#define ___oahRegularHandlers___

#include "oahBasicTypes.h"


using namespace std;

namespace MusicXML2
{
//_______________________________________________________________________________
class EXP oahRegularHandler : public oahHandler
/*
  A regular OAH handler relies on the existence of so-called 'insider' handler,
  that contains all the options values gathered from the user,
  grouped according to the internal representations and passes used.

  The variables containing the values of the options chosen by the user
  are actually held by the insider handler.
*/
{
/* this class is purely virtual
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<oahRegularHandler> create (
      string       executableName,
      string       handlerDescription,
      S_oahHandler insiderOahHandler);
*/

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    oahRegularHandler (
      string       executableName,
      string       handlerHeader,
      S_oahHandler insiderOahHandler);

    virtual ~oahRegularHandler ();

  protected:

    // protected initialization
    // ------------------------------------------------------

    void                  initializeOahRegularHandler ();

  public:

    // public services
    // ------------------------------------------------------

    void                  handlerOahError (const string& errorMessage) override
                              {
                                fInsiderHandler->
                                  handlerOahError (
                                    errorMessage);
                              }

    string                handlerExecutableAboutInformation () const override
                              {
                                return
                                  fInsiderHandler->
                                    handlerExecutableAboutInformation ();
                              }

    void                  appendGroupToRegulalHandler (
                            S_oahGroup group);
    void                  prependGroupToRegularHandler (
                            S_oahGroup group);

  public:

    // visitors
    // ------------------------------------------------------

  public:

    // print
    // ------------------------------------------------------

    void                  print (ostream& os) const;

  protected:

    // protected services
    // ------------------------------------------------------

    // regular handler groups creation

    virtual void          createRegularHandlerGroups () = 0;

    void                  registerAtomInRegularSubgroup (
                            string        atomName,
                            S_oahSubGroup subGroup);

  protected:

    // protected fields
    // ------------------------------------------------------

    S_oahHandler          fInsiderHandler;
};
typedef SMARTP<oahRegularHandler> S_oahRegularHandler;
EXP ostream& operator<< (ostream& os, const S_oahRegularHandler& elt);


}


#endif
