/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef ___oahInsiderHandlers___
#define ___oahInsiderHandlers___

#include "msrBasicTypes.h"

#include "oahBasicTypes.h"

#include "oahAtomsCollection.h"


namespace MusicXML2
{

//_______________________________________________________________________________
class EXP oahInsiderHandler : public oahHandler
/*
  An insider OAH handler proposes a full set of options,
  displayed in the group-subgroup-atom hierarchy,
  some of which are selected by so-called 'regular' handlers,
  which display them in a simpler group-atom hierarchy.

  The idea that options in insider handlers are grouped
  by internal epresentations and passes used,
  when they are grouped by user topics
  such as tuplets or grace notes in regular handlers.

  The variables containing the values of the options chosen by the user
  are actually held by the insider handler.
*/
{
/* this class is purely virtual
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<oahInsiderHandler> create (
      const string& executableName,
      const string& handlerHeader,
      const string& handlerDescription,
      const string& handlerUsage);
*/

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    oahInsiderHandler (
      const string& executableName,
      const string& handlerHeader,
      const string& handlerDescription,
      const string& handlerUsage);

    virtual ~oahInsiderHandler ();

  public:

    // set and get
    // ------------------------------------------------------

  private:

    // private initialization
    // ------------------------------------------------------

  public:

    // public services
    // ------------------------------------------------------

    void                  handlerOahError (const string& errorMessage) override;

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

  private:

    // private services
    // ------------------------------------------------------

  private:

    // private fields
    // ------------------------------------------------------

};
typedef SMARTP<oahInsiderHandler> S_oahInsiderHandler;
EXP ostream& operator<< (ostream& os, const S_oahInsiderHandler& elt);


}


#endif
