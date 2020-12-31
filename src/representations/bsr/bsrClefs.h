#ifndef ___bsrClefs___
#define ___bsrClefs___

#include "bsrCellsLists.h"
#include "bsrLineContentsElements.h"


namespace MusicXML2
{

//______________________________________________________________________________
class EXP bsrClef : public bsrLineContentsElement
{
  public:

    // data types
    // ------------------------------------------------------

    enum bsrClefKind {
      kClefKindNone,

      kClefKindGTreble,
      kClefKindFBass,
      kClefKindCAlto,
      kClefKindGSoprano, // first line, french violin
      kClefKindCBaritone,
      kClefKindCTenor,
      kClefKindGOttavaAlta,
      kClefKindGOttavaBassa,
      kClefKindModifiedBassForRightHandPart,
      kClefKindModifiedTrebleForLeftHandPart
    };

    static string clefKindAsString (
      bsrClefKind clefKind);

    static string clefKindAsDebugString (
      bsrClefKind clefKind);

    // creation
    // ------------------------------------------------------

    static SMARTP<bsrClef> create (
      int         inputLineNumber,
      bsrClefKind clefKind);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    bsrClef (
      int         inputLineNumber,
      bsrClefKind clefKind);

    virtual ~bsrClef ();

  public:

    // set and get
    // ------------------------------------------------------

    bsrClefKind           getClefKind () const
                              { return fClefKind; }

  public:

    // public services
    // ------------------------------------------------------

    S_bsrCellsList        fetchCellsList () const override
                              { return fClefCellsList; }

    int                   fetchCellsNumber () const override;

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

    bsrClefKind           fClefKind;

    S_bsrCellsList        fClefCellsList;
};
typedef SMARTP<bsrClef> S_bsrClef;
EXP ostream& operator<< (ostream& os, const S_bsrClef& elt);


}


#endif
