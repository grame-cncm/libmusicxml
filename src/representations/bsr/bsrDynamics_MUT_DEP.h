/*
  This file is to be included only by bsr_MUTUAL_DEPENDENCIES.h,
  to satisfy declarations mutual dependencies.
*/

#ifndef ___bsrDynamics_MUT_DEP___
#define ___bsrDynamics_MUT_DEP___

//______________________________________________________________________________
class EXP bsrDynamics : public bsrLineElement
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<bsrDynamics> create (
      int                          inputLineNumber,
      msrDynamics::msrDynamicsKind dynamicsKind);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    bsrDynamics (
      int                          inputLineNumber,
      msrDynamics::msrDynamicsKind dynamicsKind);

    virtual ~bsrDynamics ();

  public:

    // set and get
    // ------------------------------------------------------

    msrDynamics::msrDynamicsKind
                          getDynamicsKind () const
                              { return fDynamicsKind; }

  public:

    // public services
    // ------------------------------------------------------

    S_bsrCellsList        fetchCellsList () const override
                              { return fDynamicsCellsList; }


    int                   fetchCellsNumber () const override;

  private:

    // private services
    // ------------------------------------------------------

    S_bsrCellsList        noteValueKindAsCellsList () const;

    S_bsrCellsList        noteOctaveKindAsCellsList () const;

    S_bsrCellsList        buildCellsList () const;

  public:

    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v) override;
    virtual void          acceptOut (basevisitor* v) override;

    virtual void          browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    string                asString () const override;

    string                asDebugString () const override;

    virtual void          print (ostream& os) const override;

  private:

    // private fields
    // ------------------------------------------------------

    msrDynamics::msrDynamicsKind
                          fDynamicsKind;

    S_bsrCellsList        fDynamicsCellsList;
};
typedef SMARTP<bsrDynamics> S_bsrDynamics;
EXP ostream& operator<< (ostream& os, const S_bsrDynamics& elt);


#endif
