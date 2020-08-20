/*
  This file is to be included only by bsrMutualDependencies.h,
  to satisfy declarations mutual dependencies.
*/

//______________________________________________________________________________
class bsrKey;
typedef SMARTP<bsrKey> S_bsrKey;

class bsrKey : public bsrLineElement
{
  public:

    // data types
    // ------------------------------------------------------

    enum bsrKeyKind {
        kKeyKindNone,
        kKeyKindFlats, kKeyKindNaturals, kKeyKindSharps };

    static string keyKindAsString (
      bsrKeyKind keyKind);

    // creation
    // ------------------------------------------------------

    static SMARTP<bsrKey> create (
      int        inputLineNumber,
      bsrKeyKind keyKind,
      int        numberOfAlterations);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    bsrKey (
      int        inputLineNumber,
      bsrKeyKind keyKind,
      int        numberOfAlterations);

    virtual ~bsrKey ();

  public:

    // set and get
    // ------------------------------------------------------

    bsrKeyKind            getKeyKind () const
                              { return fKeyKind; }

    int                   getNumberOfAlterations () const
                              { return fNumberOfAlterations; }

  public:

    // public services
    // ------------------------------------------------------

    S_bsrCellsList        fetchCellsList () const override
                              { return fKeyCellsList; }

    int                   fetchCellsNumber () const override;

  private:

    // private services
    // ------------------------------------------------------

    S_bsrCellsList        keyKindAsCellsList () const;

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

    // fields
    // ------------------------------------------------------

    bsrKeyKind            fKeyKind;
    int                   fNumberOfAlterations;

    S_bsrCellsList        fKeyCellsList;
};
typedef SMARTP<bsrKey> S_bsrKey;
EXP ostream& operator<< (ostream& os, const S_bsrKey& elt);

