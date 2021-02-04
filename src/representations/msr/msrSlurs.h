/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef ___msrSlurs___
#define ___msrSlurs___

#include "msrElements.h"

#include "msrBasicTypes.h"


namespace MusicXML2
{

//______________________________________________________________________________
class EXP msrSlur : public msrElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrSlur> create (
                            int              inputLineNumber,
                            int              slurNumber,
                            msrSlurTypeKind  slurTypeKind,
                            msrLineTypeKind  slurLineTypeKind,
                            msrPlacementKind slurPlacementKind);

    static SMARTP<msrSlur> create (
                            int              inputLineNumber,
                            int              slurNumber,
                            msrSlurTypeKind  slurTypeKind,
                            msrLineTypeKind  slurLineTypeKind);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msrSlur (
                            int              inputLineNumber,
                            int              slurNumber,
                            msrSlurTypeKind  slurTypeKind,
                            msrLineTypeKind  slurLineTypeKind,
                            msrPlacementKind slurPlacementKind);

    virtual               ~msrSlur ();

  public:

    // set and get
    // ------------------------------------------------------

    void                  setSlurNumber (int slurNumber)
                              { fSlurNumber = slurNumber; }

    int                   getSlurNumber () const
                              { return fSlurNumber; }

    void                  setSlurTypeKind (msrSlurTypeKind slurTypeKind)
                              { fSlurTypeKind = slurTypeKind; }

    msrSlurTypeKind       getSlurTypeKind () const
                              { return fSlurTypeKind; }

    void                  setSlurLineTypeKind (msrLineTypeKind slurLineTypeKind)
                              { fSlurLineTypeKind = slurLineTypeKind; }

    msrLineTypeKind       getSlurLineTypeKind () const
                              { return fSlurLineTypeKind; }

    void                  setSlurPlacementKind (msrPlacementKind slurPlacementKind)
                              { fSlurPlacementKind = slurPlacementKind; }

    msrPlacementKind      getSlurPlacementKind () const
                              { return fSlurPlacementKind; }

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

    int                   fSlurNumber;

    msrSlurTypeKind       fSlurTypeKind;

    msrLineTypeKind       fSlurLineTypeKind;

    msrPlacementKind      fSlurPlacementKind;
};
typedef SMARTP<msrSlur> S_msrSlur;
EXP ostream& operator<< (ostream& os, const S_msrSlur& elt);


} // namespace MusicXML2


#endif
