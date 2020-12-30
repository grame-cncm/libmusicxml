/*
  This file is to be included only by bsr_MUTUAL_DEPENDENCIES.h,
  to satisfy declarations mutual dependencies.
*/

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

    virtual ~bsrSpaces ();

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

    int                   fNumberOfSpaces;

    S_bsrCellsList        fSpacesCellsList;
};
typedef SMARTP<bsrSpaces> S_bsrSpaces;
EXP ostream& operator<< (ostream& os, const S_bsrSpaces& elt);


}


#endif
