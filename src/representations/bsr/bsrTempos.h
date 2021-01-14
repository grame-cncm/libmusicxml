#ifndef ___bsrTempos___
#define ___bsrTempos___

#include "msrTempos.h"

#include "bsrLineContentsElements.h"
#include "bsrCellsLists.h"


namespace MusicXML2
{

//______________________________________________________________________________
string tempoKindAsDebugString (
  msrTempo::msrTempoKind tempoKind);

//______________________________________________________________________________
const bsrCellKind
  kCellTempoEquals = kDots2356,
  kCellTempoHyphen = kDots36;

class EXP bsrTempo : public bsrLineContentsElement
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

    virtual               ~bsrTempo ();

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

    S_msrTempo            fMsrTempo;

    S_bsrCellsList        fTempoCellsList;
};
typedef SMARTP<bsrTempo> S_bsrTempo;
EXP ostream& operator<< (ostream& os, const S_bsrTempo& elt);


}


#endif
