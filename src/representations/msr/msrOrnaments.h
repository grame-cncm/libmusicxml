#ifndef ___msrOrnaments___
#define ___msrOrnaments___

#include "msrElements.h"
#include "msrNotes.h"


namespace MusicXML2
{

//______________________________________________________________________________
class msrNote;
typedef SMARTP<msrNote> S_msrNote;

//______________________________________________________________________________
class EXP msrOrnament : public msrElement
{
  public:

    // data types
    // ------------------------------------------------------

    enum msrOrnamentKind {
        kOrnamentTrill, // JMI kOrnamentDashes,
        kOrnamentTurn, kOrnamentInvertedTurn, kOrnamentDelayedTurn,
        kOrnamentDelayedInvertedTurn, kOrnamentVerticalTurn,
        kOrnamentMordent, kOrnamentInvertedMordent,
        kOrnamentSchleifer, kOrnamentShake,
        kOrnamentAccidentalKind
      };

    static string ornamentKindAsString (
      msrOrnamentKind ornamentKind);

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrOrnament> create (
      int              inputLineNumber,
      msrOrnamentKind  ornamentKind,
      msrPlacementKind ornamentPlacementKind);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrOrnament (
      int              inputLineNumber,
      msrOrnamentKind  ornamentKind,
      msrPlacementKind ornamentPlacementKind);

    virtual ~msrOrnament ();

  public:

    // set and get
    // ------------------------------------------------------

    void                  setOrnamentNoteUpLink (S_msrNote note)
                              { fOrnamentNoteUpLink = note; }

    S_msrNote             getOrnamentNoteUpLink () const
                              { return fOrnamentNoteUpLink; }

    msrOrnamentKind       getOrnamentKind () const
                              { return fOrnamentKind; }

    void                  setOrnamentPlacementKind (
                            msrPlacementKind
                              ornamentPlacementKind)
                              {
                                fOrnamentPlacementKind =
                                  ornamentPlacementKind;
                              }

    msrPlacementKind      getOrnamentPlacementKind () const
                              { return fOrnamentPlacementKind; }

    void                  setOrnamentAccidentalKind (
                            msrAccidentalKind
                              ornamentAccidentalKind)
                              {
                                fOrnamentAccidentalKind =
                                  ornamentAccidentalKind;
                              }

    msrAccidentalKind getOrnamentAccidentalKind () const
                              { return fOrnamentAccidentalKind; }

  public:

    // public services
    // ------------------------------------------------------

    string                ornamentKindAsString () const;

    string                ornamentPlacementKindAsString () const;

    string                ornamentAccidentalKindAsString () const;

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

    void                  print (ostream& os) const override;

  private:

    // private fields
    // ------------------------------------------------------

    S_msrNote             fOrnamentNoteUpLink;

    msrOrnamentKind       fOrnamentKind;

    msrPlacementKind      fOrnamentPlacementKind;

    msrAccidentalKind     fOrnamentAccidentalKind;
};
typedef SMARTP<msrOrnament> S_msrOrnament;
EXP ostream& operator<< (ostream& os, const S_msrOrnament& elt);


}


#endif
