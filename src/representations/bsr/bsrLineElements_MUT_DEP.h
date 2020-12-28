/*
  This file is to be included only by bsr_MUTUAL_DEPENDENCIES.h,
  to satisfy declarations mutual dependencies.
*/

#ifndef ___bsrLineElements_MUT_DEP___
#define ___bsrLineElements_MUT_DEP___

#include "bsrElements.h"
//#include "bsrLines_MUT_DEP.h"
#include "bsrCellsLists.h"


namespace MusicXML2
{

//______________________________________________________________________________
class bsrLine;
typedef SMARTP<bsrLine> S_bsrLine;

//______________________________________________________________________________
// this class is purely virtual

class EXP bsrLineElement : public bsrElement
{
  public:

    // creation
    // ------------------------------------------------------

/*
    static SMARTP<bsrLineElement> create (
      int inputLineNumber);
*/

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    bsrLineElement (
      int inputLineNumber);

    virtual ~bsrLineElement ();

  public:

    // set and get
    // ------------------------------------------------------

    void                  setBsrLineUpLink (
                            S_bsrLine bsrLineUpLink);

    S_bsrLine             getBsrLineUpLink () const;


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

    virtual void          acceptIn  (basevisitor* v) override;
    virtual void          acceptOut (basevisitor* v) override;

    virtual void          browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    virtual std::string   asString () const override;

    virtual std::string   asShortString () const override;

    virtual std::string   asDebugString () const = 0;

    virtual void          print (ostream& os) const override;

  protected:

    // protected fields
    // ------------------------------------------------------

    S_bsrLine             fBsrLineUpLink;

    int                   fSpacesBefore;
};
typedef SMARTP<bsrLineElement> S_bsrLineElement;
EXP ostream& operator<< (ostream& os, const S_bsrLineElement& elt);


}


#endif
