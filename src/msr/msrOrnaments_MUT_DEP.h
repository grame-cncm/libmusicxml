/*
  This file is to be included only by msr_MUTUAL_DEPENDENCIES.h,
  to satisfy declarations mutual dependencies.
*/

#ifndef ___msrOrnaments_MUT_DEP___
#define ___msrOrnaments_MUT_DEP___

//______________________________________________________________________________
class msrOrnament : public msrElement
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
        kOrnamentAccidentalKind };

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

    void                  setOrnamentAccidentalKindKind (
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

    virtual void          acceptIn  (basevisitor* v) override;
    virtual void          acceptOut (basevisitor* v) override;

    virtual void          browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    string                asString () const override;

    virtual void          print (ostream& os) const override;

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


#endif
