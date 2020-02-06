/*
  This file is to be included only by bsrMutualDependencies.h,
  to satisfy declarations mutual dependencies.
*/

//______________________________________________________________________________
string tempoKindAsDebugString (
  msrTempo::msrTempoKind tempoKind);

//______________________________________________________________________________
const bsrCellKind
  kCellTempoEquals = kDots2356,
  kCellTempoHyphen = kDots36;

class bsrTempo : public bsrLineElement
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<bsrTempo> create (
      int        inputLineNumber,
      S_msrTempo mTempo);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    bsrTempo (
      int        inputLineNumber,
      S_msrTempo mTempo);

    virtual ~bsrTempo ();

  public:

    // set and get
    // ------------------------------------------------------

    S_msrTempo            getMsrTempo () const
                              { return fMsrTempo; }

    S_bsrCellsList        getTempoCellsList () const
                              { return fTempoCellsList; }

  public:

    // public services
    // ------------------------------------------------------

    S_bsrCellsList        fetchCellsList () const
                              { return fTempoCellsList; }

    int                   fetchCellsNumber () const;

  private:

    // private services
    // ------------------------------------------------------

    S_bsrCellsList        noteValueKindAsCellsList () const;

    S_bsrCellsList        noteOctaveKindAsCellsList () const;

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

    S_msrTempo            fMsrTempo;

    S_bsrCellsList        fTempoCellsList;
};
typedef SMARTP<bsrTempo> S_bsrTempo;
EXP ostream& operator<< (ostream& os, const S_bsrTempo& elt);

