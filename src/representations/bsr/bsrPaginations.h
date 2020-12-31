#ifndef ___bsrPaginations___
#define ___bsrPaginations___

#include "bsrLineContentsElements.h"
#include "bsrCellsLists.h"


namespace MusicXML2
{

//______________________________________________________________________________
class EXP bsrPagination : public bsrLineContentsElement
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

    void                  acceptIn  (basevisitor* v) override;
    void                  acceptOut (basevisitor* v) override;

    void                  browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    string                asString () const override;

    string                asDebugString () const override;

    void                  print (ostream& os) const override;

  private:

    // private fields
    // ------------------------------------------------------

    int                   fPrintPageNumber;
    int                   fBraillePageNumber;

    S_bsrCellsList        fPaginationCellsList;
};
typedef SMARTP<bsrPagination> S_bsrPagination;
EXP ostream& operator<< (ostream& os, const S_bsrPagination& elt);


}


#endif
