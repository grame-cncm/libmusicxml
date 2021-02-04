/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef ___msrTies___
#define ___msrTies___

#include "msrElements.h"

#include "msrBasicTypes.h"


namespace MusicXML2
{

//______________________________________________________________________________
class EXP msrTie : public msrElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrTie> create (
                            int              inputLineNumber,
                            msrTieKind       tieKind,
                            msrPlacementKind placementKind);

    static SMARTP<msrTie> create (
                            int              inputLineNumber,
                            msrTieKind       tieKind);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msrTie (
                            int              inputLineNumber,
                            msrTieKind       tieKind,
                            msrPlacementKind placementKind);

    virtual               ~msrTie ();

  public:

    // set and get
    // ------------------------------------------------------

    void                  setTieKind (msrTieKind tieKind)
                              { fTieKind = tieKind; }

    msrTieKind            getTieKind () const
                              { return fTieKind; }

    void                  setTiePlacementKind (msrPlacementKind placementKind)
                              { fTiePlacementKind = placementKind; }

    msrPlacementKind      getTiePlacementKind () const
                              { return fTiePlacementKind; }

          ;
  public:

    // public services
    // ------------------------------------------------------

  public:

    // visitors
    // ------------------------------------------------------

    void                  acceptIn  (basevisitor* v) override;
    void                  acceptOut (basevisitor* v) override;

    void                  browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    string                asString () const override;

    void                  print (ostream& os) const override;

  private:

    // private fields
    // ------------------------------------------------------

    msrTieKind            fTieKind;

    msrPlacementKind      fTiePlacementKind;
};
typedef SMARTP<msrTie> S_msrTie;
EXP ostream& operator<< (ostream& os, const S_msrTie& elt);


} // namespace MusicXML2


#endif
