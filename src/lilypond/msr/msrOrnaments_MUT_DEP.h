/*
  This file is to be included only by msrMutualDependencies.h,
  to satisfy declarations mutual dependencies.
*/

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
        kOrnamentAccidentalMark };

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

    void                  setOrnamentAccidentalMarkKind (
                            msrAlterationKind
                              ornamentAccidentalMark)
                              {
                                fOrnamentAccidentalMark =
                                  ornamentAccidentalMark;
                              }

    msrAlterationKind     getOrnamentAccidentalMark () const
                              { return fOrnamentAccidentalMark; }

    // services
    // ------------------------------------------------------

    string                ornamentKindAsString () const;

    string                ornamentPlacementKindAsString () const;

    string                ornamentAccidentalMarkAsString () const;

  public:

    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v);
    virtual void          acceptOut (basevisitor* v);

    virtual void          browseData (basevisitor* v);

  public:

    // print
    // ------------------------------------------------------

    virtual void          print (ostream& os) const;

  private:

    // fields
    // ------------------------------------------------------

    S_msrNote             fOrnamentNoteUpLink;

    msrOrnamentKind       fOrnamentKind;

    msrPlacementKind      fOrnamentPlacementKind;

    msrAlterationKind     fOrnamentAccidentalMark;
};
typedef SMARTP<msrOrnament> S_msrOrnament;
EXP ostream& operator<< (ostream& os, const S_msrOrnament& elt);

