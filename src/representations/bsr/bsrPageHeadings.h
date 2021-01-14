#ifndef ___bsrPageHeadings___
#define ___bsrPageHeadings___

#include "bsrCellsLists.h"
#include "bsrLines.h"
#include "bsrPaginations.h"


namespace MusicXML2
{

//______________________________________________________________________________
class EXP bsrPageHeading : public bsrLine
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

    virtual               ~bsrPageHeading ();

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

    string                fPageHeadingTitle;

    S_bsrPagination       fPageHeadingPagination; // before title
    int                   fPageHeadingNumber;     // after title ??? JMI
};
typedef SMARTP<bsrPageHeading> S_bsrPageHeading;
EXP ostream& operator<< (ostream& os, const S_bsrPageHeading& elt);


}


#endif
