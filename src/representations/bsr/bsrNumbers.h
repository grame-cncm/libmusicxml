#ifndef ___bsrNumbers___
#define ___bsrNumbers___

#include "bsrLineContentsElements.h"
#include "bsrCellsLists.h"


namespace MusicXML2
{

//______________________________________________________________________________
class EXP bsrNumber : public bsrLineContentsElement
{
  public:

    // data types
    // ------------------------------------------------------

    enum bsrNumberSignIsNeededKind {
      kNumberSignIsNeededYes, kNumberSignIsNeededNo
    };

    static string numberSignIsNeededKindAsString (
      bsrNumberSignIsNeededKind numberSignIsNeededKind);

    // creation
    // ------------------------------------------------------

    static SMARTP<bsrNumber> create (
      int       inputLineNumber,
      int       numberValue,
      bsrNumberSignIsNeededKind
                numberSignIsNeededKind);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    bsrNumber (
      int       inputLineNumber,
      int       numberValue,
      bsrNumberSignIsNeededKind
                numberSignIsNeededKind);

    virtual ~bsrNumber ();

  public:

    // set and get
    // ------------------------------------------------------

    int                   getNumberValue () const
                              { return fNumberValue; }

    void                  setNumberSignIsNeededKind (
                            bsrNumberSignIsNeededKind
                              numberSignIsNeededKindvalue)
                              {
                                fNumberSignIsNeededKind =
                                  numberSignIsNeededKindvalue;
                              }

    bsrNumberSignIsNeededKind
                          getNumberSignIsNeededKind () const
                              { return fNumberSignIsNeededKind; }

    S_bsrCellsList        getNumberCellsList () const
                              { return fNumberCellsList; }

    // public services
    // ------------------------------------------------------

    S_bsrCellsList        fetchCellsList () const override
                              { return fNumberCellsList; }

    int                   fetchCellsNumber () const override;

  private:

    // private services
    // ------------------------------------------------------

    S_bsrCellsList        numberValueAsCellsList () const;

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

    int                   fNumberValue;

    bsrNumberSignIsNeededKind
                          fNumberSignIsNeededKind;

    S_bsrCellsList        fNumberCellsList;
};
typedef SMARTP<bsrNumber> S_bsrNumber;
EXP ostream& operator<< (ostream& os, const S_bsrNumber& elt);


}


#endif
