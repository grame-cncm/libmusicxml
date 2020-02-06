/*
  This file is to be included only by bsrMutualDependencies.h,
  to satisfy declarations mutual dependencies.
*/

//______________________________________________________________________________
class bsrNumber : public bsrLineElement
{
  public:

    // data types
    // ------------------------------------------------------

    enum bsrNumberSignIsNeededKind {
        kNumberSignIsNeededYes, kNumberSignIsNeededNo};

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

    S_bsrCellsList        fetchCellsList () const
                              { return fNumberCellsList; }

    int                   fetchCellsNumber () const;

  private:

    // private services
    // ------------------------------------------------------

    S_bsrCellsList        numberValueAsCellsList () const;

    S_bsrCellsList        buildCellsList () const;

  public:

    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v);
    virtual void          acceptOut (basevisitor* v);

    virtual void          browseData (basevisitor* v);

  public:

    // print
    // ------------------------------------------------------

    string                asString () const;

    virtual string        asDebugString () const;

    virtual void          print (ostream& os) const;

  private:

    // fields
    // ------------------------------------------------------

    int                   fNumberValue;

    bsrNumberSignIsNeededKind
                          fNumberSignIsNeededKind;

    S_bsrCellsList        fNumberCellsList;
};
typedef SMARTP<bsrNumber> S_bsrNumber;
EXP ostream& operator<< (ostream& os, const S_bsrNumber& elt);

