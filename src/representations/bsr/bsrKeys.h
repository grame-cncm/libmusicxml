#ifndef ___bsrKeys___
#define ___bsrKeys___

#include "bsrCellsLists.h"
#include "bsrLineContentsElements.h"


namespace MusicXML2
{

//______________________________________________________________________________
class EXP bsrKey : public bsrLineContentsElement
{
  public:

    // data types
    // ------------------------------------------------------

    enum bsrKeyKind {
      kKeyKindNone,
      kKeyKindFlats, kKeyKindNaturals, kKeyKindSharps
    };

    static string keyKindAsString (
      bsrKeyKind keyKind);

    // creation
    // ------------------------------------------------------

    static SMARTP<bsrKey> create (
      int        inputLineNumber,
      bsrKeyKind keyKind,
      int        numberOfAlterations);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    bsrKey (
      int        inputLineNumber,
      bsrKeyKind keyKind,
      int        numberOfAlterations);

    virtual ~bsrKey ();

  public:

    // set and get
    // ------------------------------------------------------

    bsrKeyKind            getKeyKind () const
                              { return fKeyKind; }

    int                   getNumberOfAlterations () const
                              { return fNumberOfAlterations; }

  public:

    // public services
    // ------------------------------------------------------

    S_bsrCellsList        fetchCellsList () const override
                              { return fKeyCellsList; }

    int                   fetchCellsNumber () const override;

  private:

    // private services
    // ------------------------------------------------------

    S_bsrCellsList        keyKindAsCellsList () const;

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

    bsrKeyKind            fKeyKind;
    int                   fNumberOfAlterations;

    S_bsrCellsList        fKeyCellsList;
};
typedef SMARTP<bsrKey> S_bsrKey;
EXP ostream& operator<< (ostream& os, const S_bsrKey& elt);


}


#endif
