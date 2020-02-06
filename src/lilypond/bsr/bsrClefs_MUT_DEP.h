/*
  This file is to be included only by bsrMutualDependencies.h,
  to satisfy declarations mutual dependencies.
*/

//______________________________________________________________________________
class bsrClef : public bsrLineElement
{
  public:

    // data types
    // ------------------------------------------------------

    enum bsrClefKind {
        kClefKindNone,

        kClefKindGTreble,
        kClefKindFBass,
        kClefKindCAlto,
        kClefKindGSoprano, // first line, french violin
        kClefKindFBaritone,
        kClefKindCTenor,
        kClefKindGOttavaAlta,
        kClefKindGOttavaBassa,
        kClefKindModifiedBassForRightHandPart,
        kClefKindModifiedTrebleForLeftHandPart };

    static string clefKindAsString (
      bsrClefKind clefKind);

    // creation
    // ------------------------------------------------------

    static SMARTP<bsrClef> create (
      int         inputLineNumber,
      bsrClefKind clefKind);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    bsrClef (
      int         inputLineNumber,
      bsrClefKind clefKind);

    virtual ~bsrClef ();

  public:

    // set and get
    // ------------------------------------------------------

    bsrClefKind           getClefKind () const
                              { return fClefKind; }

  public:

    // public services
    // ------------------------------------------------------

    S_bsrCellsList        fetchCellsList () const
                              { return fClefCellsList; }

    int                   fetchCellsNumber () const;

  private:

    // private services
    // ------------------------------------------------------

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

    string                asDebugString () const;

    virtual void          print (ostream& os) const;

  private:

    // fields
    // ------------------------------------------------------

    bsrClefKind           fClefKind;

    S_bsrCellsList        fClefCellsList;
};
typedef SMARTP<bsrClef> S_bsrClef;
EXP ostream& operator<< (ostream& os, const S_bsrClef& elt);

