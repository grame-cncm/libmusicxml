#ifndef ___bsrLineContentsElements___
#define ___bsrLineContentsElements___

#include "bsrElements.h"

#include "bsrCellsLists.h"


namespace MusicXML2
{

//______________________________________________________________________________
// this class is purely virtual
class EXP bsrLineContentsElement : public bsrElement
{
  public:

    // creation
    // ------------------------------------------------------

/*
    static SMARTP<bsrLineContentsElement> create (
                            int inputLineNumber);
*/

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          bsrLineContentsElement (
                            int inputLineNumber);

    virtual               ~bsrLineContentsElement ();

  public:

    // set and get
    // ------------------------------------------------------
/*
    void                  setBsrLineContentsUpLink (
                            S_bsrLineContents lineContents);
*/

    void                  setSpacesBefore (int value)
                              { fSpacesBefore = value; }

    int                   getSpacesBefore () const
                              { return fSpacesBefore; }

  public:

    // public services
    // ------------------------------------------------------


    virtual S_bsrCellsList
                          fetchCellsList () const = 0;

    virtual int           fetchCellsNumber () const = 0;

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

    string                asShortString () const override;

    virtual string        asDebugString () const = 0;

    void                  print (ostream& os) const override;

  protected:

    // protected fields
    // ------------------------------------------------------

/*
    S_bsrLineContents     fBsrLineContentsUpLink;
*/

    int                   fSpacesBefore;
};
typedef SMARTP<bsrLineContentsElement> S_bsrLineContentsElement;
EXP ostream& operator<< (ostream& os, const S_bsrLineContentsElement& elt);


}


#endif
