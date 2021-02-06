#ifndef ___bsrSpaces___
#define ___bsrSpaces___

#include "bsrLineContentsElements.h"
#include "bsrCellsLists.h"


namespace MusicXML2
{

//______________________________________________________________________________
const bsrCellKind
  kCellSpace = kDotsNone;

//______________________________________________________________________________
class EXP bsrSpaces : public bsrLineContentsElement
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<bsrSpaces> create (
                            int inputLineNumber,
                            int numberOfSpaces);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          bsrSpaces (
                            int inputLineNumber,
                            int numberOfSpaces);

    virtual               ~bsrSpaces ();

  public:

    // set and get
    // ------------------------------------------------------

    int                   getNumberOfSpaces () const
                              { return fNumberOfSpaces; }

  public:

    // public services
    // ------------------------------------------------------

    S_bsrCellsList        fetchCellsList () const override
                              { return fSpacesCellsList; }

    int                   fetchCellsNumber () const override;

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

    int                   fNumberOfSpaces;

    S_bsrCellsList        fSpacesCellsList;
};
typedef SMARTP<bsrSpaces> S_bsrSpaces;
EXP ostream& operator<< (ostream& os, const S_bsrSpaces& elt);


}


#endif
