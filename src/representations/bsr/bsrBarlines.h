/*
  This file is to be included only by bsr_MUTUAL_DEPENDENCIES.h,
  to satisfy declarations mutual dependencies.
*/

#ifndef ___bsrBarlines___
#define ___bsrBarlines___

#include "bsrCellsLists.h"
#include "bsrLineContentsElements.h"

namespace MusicXML2
{

//______________________________________________________________________________
class EXP bsrBarline : public bsrLineContentsElement
{
  public:

    // data types
    // ------------------------------------------------------

    enum bsrBarlineKind {
      kBarlineKindNone,
      kBarlineKindSpecial, // for dotted or dashed barlines
      kBarlineKindUnusual, // irregular spacing, ...
      kBarlineKindFinalDouble, kBarlineKindSectionalDouble
    };

    static string barlineKindAsString (
      bsrBarlineKind barlineKind);

    static string barlineKindAsDebugString (
      bsrBarlineKind barlineKind);

    // creation
    // ------------------------------------------------------

    static SMARTP<bsrBarline> create (
      int            inputLineNumber,
      bsrBarlineKind barlineKind);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    bsrBarline (
      int            inputLineNumber,
      bsrBarlineKind barlineKin);

    virtual ~bsrBarline ();

  public:

    // set and get
    // ------------------------------------------------------

    bsrBarlineKind        getBarlineKind () const
                              { return fBarlineKind; }

  public:

    // public services
    // ------------------------------------------------------

    S_bsrCellsList        fetchCellsList () const override
                              { return fBarlineCellsList; }

    int                   fetchCellsNumber () const override;

  private:

    // private services
    // ------------------------------------------------------

    S_bsrCellsList        barlineKindAsCellsList ();

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

    // private fields
    // ------------------------------------------------------

    bsrBarlineKind        fBarlineKind;

    S_bsrCellsList        fBarlineCellsList;
};
typedef SMARTP<bsrBarline> S_bsrBarline;
EXP ostream& operator<< (ostream& os, const S_bsrBarline& elt);

}


#endif
