/*
  This file is to be included only by bsrMutualDependencies.h,
  to satisfy declarations mutual dependencies.
*/

#ifndef ___bsrPaginations_MUT_DEP___
#define ___bsrPaginations_MUT_DEP___

//______________________________________________________________________________
class bsrPagination : public bsrLineElement
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<bsrPagination> create (
      int inputLineNumber,
      int printPageNumber,
      int braillePageNumber);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    bsrPagination (
      int inputLineNumber,
      int printPageNumber,
      int braillePageNumber);

    virtual ~bsrPagination ();

  public:

    // set and get
    // ------------------------------------------------------

    int                   getPrintPageNumber () const
                              { return fPrintPageNumber; }

    int                   getBraillePageNumber () const
                              { return fBraillePageNumber; }

    S_bsrCellsList        fetchCellsList () const override
                              { return fPaginationCellsList; }

    int                   fetchCellsNumber () const override;

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

    int                   fPrintPageNumber;
    int                   fBraillePageNumber;

    S_bsrCellsList        fPaginationCellsList;
};
typedef SMARTP<bsrPagination> S_bsrPagination;
EXP ostream& operator<< (ostream& os, const S_bsrPagination& elt);


#endif
