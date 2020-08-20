/*
  This file is to be included only by bsrMutualDependencies.h,
  to satisfy declarations mutual dependencies.
*/

#ifndef ___bsrPageHeadings_MUT_DEP___
#define ___bsrPageHeadings_MUT_DEP___

//______________________________________________________________________________
class bsrPageHeading : public bsrLine
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<bsrPageHeading> create (
      int             inputLineNumber,
      string          pageHeadingTitle,
      S_bsrPagination pageHeadingPagination,
      int             pageHeadingNumber);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    bsrPageHeading (
      int             inputLineNumber,
      string          pageHeadingTitle,
      S_bsrPagination pageHeadingPagination,
      int             pageHeadingNumber);

    virtual ~bsrPageHeading ();

  public:

    // set and get
    // ------------------------------------------------------

    string                getPageHeadingTitle () const
                              { return fPageHeadingTitle; }

    S_bsrPagination       getPageHeadingPagination () const
                              { return fPageHeadingPagination; }

    int                   getPageHeadingNumber () const
                              { return fPageHeadingNumber; }

  public:

    // public services
    // ------------------------------------------------------

    S_bsrCellsList        fetchCellsList () const
                              { return buildCellsList (); }

  private:

    // private services
    // ------------------------------------------------------

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

    string                fPageHeadingTitle;

    S_bsrPagination       fPageHeadingPagination; // before title
    int                   fPageHeadingNumber;     // after title ??? JMI
};
typedef SMARTP<bsrPageHeading> S_bsrPageHeading;
EXP ostream& operator<< (ostream& os, const S_bsrPageHeading& elt);


#endif
