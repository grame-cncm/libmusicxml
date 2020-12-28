/*
  This file is to be included only by bsr_MUTUAL_DEPENDENCIES.h,
  to satisfy declarations mutual dependencies.
*/

#ifndef ___bsrTempos_MUT_DEP___
#define ___bsrTempos_MUT_DEP___

#include "msrTempos.h"

namespace MusicXML2
{

//______________________________________________________________________________
string tempoKindAsDebugString (
  msrTempo::msrTempoKind tempoKind);

//______________________________________________________________________________
const bsrCellKind
  kCellTempoEquals = kDots2356,
  kCellTempoHyphen = kDots36;

class EXP bsrTempo : public bsrLineElement
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<bsrTempo> create (
      int        inputLineNumber,
      S_msrTempo mTempo);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    bsrTempo (
      int        inputLineNumber,
      S_msrTempo mTempo);

    virtual ~bsrTempo ();

  public:

    // set and get
    // ------------------------------------------------------

    S_msrTempo            getMsrTempo () const
                              { return fMsrTempo; }

    S_bsrCellsList        getTempoCellsList () const
                              { return fTempoCellsList; }

  public:

    // public services
    // ------------------------------------------------------

    S_bsrCellsList        fetchCellsList () const override
                              { return fTempoCellsList; }

    int                   fetchCellsNumber () const override;

  private:

    // private services
    // ------------------------------------------------------

    S_bsrCellsList        noteValueKindAsCellsList () const;

    S_bsrCellsList        noteOctaveKindAsCellsList () const;

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

    virtual string        asDebugString () const override;

    virtual void          print (ostream& os) const override;

  private:

    // private fields
    // ------------------------------------------------------

    S_msrTempo            fMsrTempo;

    S_bsrCellsList        fTempoCellsList;
};
typedef SMARTP<bsrTempo> S_bsrTempo;
EXP ostream& operator<< (ostream& os, const S_bsrTempo& elt);


}


#endif
