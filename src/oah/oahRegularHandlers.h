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

/* JMI
#include "list"
#include "map"
#include "vector"
#include "set"

#include "smartpointer.h"
#include "rational.h"
#include "visitor.h"

#include "msrBasicTypes.h"
#include "msrMidi.h"

#include "libmusicxml.h"  // for optionsVector
*/

#include "oahBasicTypes.h"


using namespace std;

namespace MusicXML2
{
//_______________________________________________________________________________
class oahRegularOahHandler : public oahHandler
/*
  A regular OAH handler relies on the existence of so-called 'insider' handler,
  that contains all the options values gathered from the user,
  grouped according to the internal representations and passes used.

  The variables containing the options chosen by the user
  are actually those of the insider OAH handler.
*/
{
/* this class is purely virtual
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<oahRegularOahHandler> create (
      string       executableName,
      string       handlerDescription,
      S_oahHandler insiderOahHandler);
*/

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    oahRegularOahHandler (
      string       executableName,
      string       handlerHeader,
      S_oahHandler insiderOahHandler);

    virtual ~oahRegularOahHandler ();

  protected:

    // protected initialization
    // ------------------------------------------------------

    void                  initializeOahRegularOahHandler ();

  public:

    // public services
    // ------------------------------------------------------

/* JMI
    // the OAH variables are actually in the insider OAH handler,
    // the following method are thus delegated to the latter
    string                fetchOutputFileNameFromTheOptions () const override
                              {
                                return
                                  fInsiderOahHandler->
                                    fetchOutputFileNameFromTheOptions ();
                              }
*/

    void                  checkOptionsAndArgumentsFromArgcAndArgv (
                            vector<string>& theArgumentsVector) const override
                              {
                                fInsiderOahHandler->
                                  checkOptionsAndArgumentsFromArgcAndArgv (
                                    theArgumentsVector);
                              }

/* JMI

    // quiet mode
    virtual void          enforceHandlerQuietness () override
                              {
                                fInsiderOahHandler->
                                  checkHandlerOptionsConsistency ();
                              }

    // consistency check
    virtual void          checkHandlerOptionsConsistency () override
                              {
                                fInsiderOahHandler->
                                  checkHandlerOptionsConsistency ();
                              }
*/

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

    S_oahHandler          fInsiderOahHandler;
};
typedef SMARTP<oahRegularOahHandler> S_oahRegularOahHandler;
EXP ostream& operator<< (ostream& os, const S_oahRegularOahHandler& elt);


}


#endif
