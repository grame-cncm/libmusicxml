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
/*
  The representations and passes have their own option, grouped accordingly.
  This internal view of the options is rather low-level,
  and users may prefer one more suited to their tasks.

  A oahDualHandler combines two oahHandlers:
    - the first provides this internal view of the options groups;
    - the second one provides the same subgroups in a more 'user, subject-oriented' way.
*/

class EXP oahDualHandler : public smartable
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
    static SMARTP<oahDualHandler> create ();

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    oahDualHandler ();

    virtual ~oahDualHandler ();

  public:

    // set and get
    // ------------------------------------------------------

    oahDualHandlerKind    getOahHandlerKind () const
                              { return fOahHandlerKind; }

    const list<S_oahGroup>
                          getOahHandler () const
                              { return fOahHandler; }

    // oahHandlers
    S_oahOahHandler       getInternalOahHandler () const
                              { return fInternalOahHandler; }

    S_oahOahHandler       getUserOahHandler () const
                              { return fUserOahHandler; }

  public:

    // services
    // ------------------------------------------------------

    void                  appendGroupToOahHandler (
                            S_oahGroup group)
                              { fOahHandler.push_back (group); }

    void                  prependGroupToOahHandler (
                            S_oahGroup group)
                              { fOahHandler.push_front (group); }

  public:

    // print
    // ------------------------------------------------------

    void                  print (ostream& os) const;

  private:

    // fields
    // ------------------------------------------------------

    oahDualHandlerKind    fOahHandlerKind;

    S_oahHandler          fInternalOahHandler;
    S_oahHandler          fUserOahHandler;

    S_oahHandler          fOahHandlerToBeUsed; // according to oahOahHandlerKind

};
typedef SMARTP<oahDualHandler> S_oahDualHandler;
EXP ostream& operator<< (ostream& os, const S_oahDualHandler& elt);

