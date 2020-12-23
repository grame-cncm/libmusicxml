/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef ___msrDynamics___
#define ___msrDynamics___

#include "msrElements.h"

#include "msrBasicTypes.h"


namespace MusicXML2
{

//______________________________________________________________________________
class EXP msrDynamics : public msrElement
{
  public:

    // data types
    // ------------------------------------------------------

    enum msrDynamicsKind {
      kF, kFF, kFFF, kFFFF, kFFFFF, kFFFFFF,
      kP, kPP, kPPP, kPPPP, kPPPPP, kPPPPPP,
      kMF, kMP, kFP, kFZ, kRF, kSF, kRFZ, kSFZ, kSFP, kSFPP, kSFFZ,
      k_NoDynamics
    };

    static string dynamicsKindAsString (
      msrDynamicsKind dynamicsKind);

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrDynamics> create (
      int              inputLineNumber,
      msrDynamicsKind  dynamicsKind,
      msrPlacementKind dynamicsPlacementKind);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrDynamics (
      int              inputLineNumber,
      msrDynamicsKind  dynamicsKind,
      msrPlacementKind dynamicsPlacementKind);

    virtual ~msrDynamics ();

  public:

    // set and get
    // ------------------------------------------------------

    msrDynamicsKind       getDynamicsKind () const
                              { return fDynamicsKind; }

    msrPlacementKind      getDynamicsPlacementKind () const
                              { return fDynamicsPlacementKind; }

  public:

    // public services
    // ------------------------------------------------------

  public:

    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v) override;
    virtual void          acceptOut (basevisitor* v) override;

    virtual void          browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    string                dynamicsKindAsString () const;

    string                dynamicsPlacementKindAsString () const;

    virtual void          print (ostream& os) const override;

  private:

    // private fields
    // ------------------------------------------------------

    msrDynamicsKind       fDynamicsKind;

    msrPlacementKind      fDynamicsPlacementKind;
};
typedef SMARTP<msrDynamics> S_msrDynamics;
EXP ostream& operator<< (ostream& os, const S_msrDynamics& elt);

//______________________________________________________________________________
class EXP msrOtherDynamics : public msrElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrOtherDynamics> create (
      int              inputLineNumber,
      string           otherDynamicsString,
      msrPlacementKind otherDynamicsPlacementKind);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrOtherDynamics (
      int              inputLineNumber,
      string           otherDynamicsString,
      msrPlacementKind otherDynamicsPlacementKind);

    virtual ~msrOtherDynamics ();

  public:

    // set and get
    // ------------------------------------------------------

    string                getOtherDynamicsString () const
                              { return fOtherDynamicsString; }

    msrPlacementKind      getOtherDynamicsPlacementKind () const
                              { return fOtherDynamicsPlacementKind; }

  public:

    // public services
    // ------------------------------------------------------

  public:

    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v) override;
    virtual void          acceptOut (basevisitor* v) override;

    virtual void          browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    string                otherDynamicsPlacementKindAsString () const;

    string                asString () const override;

    virtual void          print (ostream& os) const override;

  private:

    // private fields
    // ------------------------------------------------------

    string                fOtherDynamicsString;

    msrPlacementKind      fOtherDynamicsPlacementKind;
};
typedef SMARTP<msrOtherDynamics> S_msrOtherDynamics;
EXP ostream& operator<< (ostream& os, const S_msrOtherDynamics& elt);

//______________________________________________________________________________
class EXP msrWedge : public msrElement
{
  public:

    // data types
    // ------------------------------------------------------

    enum msrWedgeKind {
      kWedgeKindNone,
      kWedgeCrescendo, kWedgeDecrescendo, kWedgeStop
    };

/* JMI
    static string wedgeKindAsString (
      msrWedgeKind wedgeKind);
*/

    enum msrWedgeNienteKind {
      kWedgeNienteYes, kWedgeNienteNo
    };

    static string wedgeNienteKindAsString (
      msrWedgeNienteKind wedgeNienteKind);

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrWedge> create (
      int                inputLineNumber,
      msrWedgeKind       wedgeKind,
      msrWedgeNienteKind wedgeNienteKind,
      msrLineTypeKind    wedgeLineTypeKind,
      msrPlacementKind   wedgePlacementKind);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrWedge (
      int                inputLineNumber,
      msrWedgeKind       wedgeKind,
      msrWedgeNienteKind wedgeNienteKind,
      msrLineTypeKind    wedgeLineTypeKind,
      msrPlacementKind   wedgePlacementKind);

    virtual ~msrWedge ();

  public:

    // set and get
    // ------------------------------------------------------

    msrWedgeKind          getWedgeKind () const
                              { return fWedgeKind; }

    msrWedgeNienteKind    getWedgeNienteKind () const
                              { return fWedgeNienteKind; }

    msrLineTypeKind       getWedgeLineTypeKind () const
                              { return fWedgeLineTypeKind; }

    msrPlacementKind      getWedgePlacementKind () const
                              { return fWedgePlacementKind; }

  public:

    // public services
    // ------------------------------------------------------

  public:

    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v) override;
    virtual void          acceptOut (basevisitor* v) override;

    virtual void          browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    string                wedgeKindAsString () const;

    virtual void          print (ostream& os) const override;

  private:

    // private fields
    // ------------------------------------------------------

    msrWedgeKind          fWedgeKind;

    msrWedgeNienteKind    fWedgeNienteKind;

    msrLineTypeKind       fWedgeLineTypeKind;

    msrPlacementKind      fWedgePlacementKind;
};
typedef SMARTP<msrWedge> S_msrWedge;
EXP ostream& operator<< (ostream& os, const S_msrWedge& elt);


} // namespace MusicXML2


#endif
