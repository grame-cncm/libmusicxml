/*
  This file is to be included only by bsr_MUTUAL_DEPENDENCIES.h,
  to satisfy declarations mutual dependencies.
*/

//______________________________________________________________________________
class EXP bsrNumber : public bsrLineElement
{
  public:

    // data types
    // ------------------------------------------------------

    enum bsrNumberSignIsNeededKind {
      kNumberSignIsNeededYes, kNumberSignIsNeededNo
    };

    static string numberSignIsNeededKindAsString (
      bsrNumberSignIsNeededKind numberSignIsNeededKind);

    // creation
    // ------------------------------------------------------

    static SMARTP<bsrNumber> create (
      int       inputLineNumber,
      int       numberValue,
      bsrNumberSignIsNeededKind
                numberSignIsNeededKind);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    bsrNumber (
      int       inputLineNumber,
      int       numberValue,
      bsrNumberSignIsNeededKind
                numberSignIsNeededKind);

    virtual ~bsrNumber ();

  public:

    // set and get
    // ------------------------------------------------------

    int                   getNumberValue () const
                              { return fNumberValue; }

    void                  setNumberSignIsNeededKind (
                            bsrNumberSignIsNeededKind
                              numberSignIsNeededKindvalue)
                              {
                                fNumberSignIsNeededKind =
                                  numberSignIsNeededKindvalue;
                              }

    bsrNumberSignIsNeededKind
                          getNumberSignIsNeededKind () const
                              { return fNumberSignIsNeededKind; }

    S_bsrCellsList        getNumberCellsList () const
                              { return fNumberCellsList; }

    // public services
    // ------------------------------------------------------

    S_bsrCellsList        fetchCellsList () const override
                              { return fNumberCellsList; }

    int                   fetchCellsNumber () const override;

  private:

    // private services
    // ------------------------------------------------------

    S_bsrCellsList        numberValueAsCellsList () const;

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

    virtual string        asDebugString () const override;

    virtual void          print (ostream& os) const override;

  private:

    // private fields
    // ------------------------------------------------------

    int                   fNumberValue;

    bsrNumberSignIsNeededKind
                          fNumberSignIsNeededKind;

    S_bsrCellsList        fNumberCellsList;
};
typedef SMARTP<bsrNumber> S_bsrNumber;
EXP ostream& operator<< (ostream& os, const S_bsrNumber& elt);

