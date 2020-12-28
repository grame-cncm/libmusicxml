/*
  This file is to be included only by msr_MUTUAL_DEPENDENCIES.h,
  to satisfy declarations mutual dependencies.
*/

#ifndef ___msrTuplets_MUT_DEP___
#define ___msrTuplets_MUT_DEP___

namespace MusicXML2
{

//______________________________________________________________________________
class EXP msrTuplet : public msrTupletElement
{
  public:

    // data types
    // ------------------------------------------------------

    enum msrTupletTypeKind {
      kTupletTypeNone,
      kTupletTypeStart, kTupletTypeContinue, kTupletTypeStop,
      kTupletTypeStartAndStopInARow
    };

    static string tupletTypeKindAsString (
      msrTupletTypeKind tupletTypeKind);

    enum msrTupletBracketKind {
      kTupletBracketYes, kTupletBracketNo
    };

    static string tupletBracketKindAsString (
      msrTupletBracketKind tupletBracketKind);

    enum msrTupletLineShapeKind {
      kTupletLineShapeStraight, kTupletLineShapeCurved
    };

    static string tupletLineShapeKindAsString (
      msrTupletLineShapeKind tupletLineShapeKind);

    enum msrTupletShowNumberKind {
      kTupletShowNumberActual, kTupletShowNumberBoth, kTupletShowNumberNone
    };

    static string tupletShowNumberKindAsString (
      msrTupletShowNumberKind tupletShowNumberKind);

    enum msrTupletShowTypeKind {
      kTupletShowTypeActual, kTupletShowTypeBoth, kTupletShowTypeNone
    };

    static string tupletShowTypeKindAsString (
      msrTupletShowTypeKind tupletShowTypeKind);

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrTuplet> create (
      int                     inputLineNumber,
      string                  tupletMeasureNumber,
      int                     tupletNumber,
      msrTupletBracketKind    tupletBracketKind,
      msrTupletLineShapeKind  tupletLineShapeKind,
      msrTupletShowNumberKind tupletShowNumberKind,
      msrTupletShowTypeKind   tupletShowTypeKind,
      msrTupletFactor         tupletFactor,
      rational                memberNotesSoundingWholeNotes,
      rational                memberNotesDisplayWholeNotes);

    SMARTP<msrTuplet> createTupletNewbornClone ();

    SMARTP<msrTuplet> createTupletDeepCopy ();

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrTuplet (
      int                     inputLineNumber,
      string                  tupletMeasureNumber,
      int                     tupletNumber,
      msrTupletBracketKind    tupletBracketKind,
      msrTupletLineShapeKind  tupletLineShapeKind,
      msrTupletShowNumberKind tupletShowNumberKind,
      msrTupletShowTypeKind   tupletShowTypeKind,
      msrTupletFactor         tupletFactor,
      rational                memberNotesSoundingWholeNotes,
      rational                memberNotesDisplayWholeNotes);

    virtual ~msrTuplet ();

  public:

    // set and get
    // ------------------------------------------------------

    // measure upLink
    void                  setTupletMeasureUpLink (
                            const S_msrMeasure& measure)
                              { fTupletMeasureUpLink = measure; }

    S_msrMeasure          getTupletMeasureUpLink () const
                              { return fTupletMeasureUpLink; }

    // tuplet uplink
    void                  setTupletTupletUpLink (
                            const S_msrTuplet& tuplet)
                              { fTupletTupletUpLink = tuplet; }

    S_msrTuplet           getTupletTupletUpLink () const
                              { return fTupletTupletUpLink; }

    // positions in measures
    rational              setTupletMembersPositionInMeasure (
                            S_msrMeasure measure,
                            rational     positionInMeasure);
                              // returns the position after the tuplet JMI ???

    // number
    int                   getTupletNumber () const
                              { return fTupletNumber; }

    // appearance
    msrTupletBracketKind  getTupletBracketKind () const
                              { return fTupletBracketKind; }

    msrTupletLineShapeKind
                          getTupletLineShapeKind () const
                              { return fTupletLineShapeKind; }

    msrTupletShowNumberKind
                          getTupletShowNumberKind () const
                              { return fTupletShowNumberKind; }

    msrTupletShowTypeKind getTupletShowTypeKind () const
                              { return fTupletShowTypeKind; }

    // tuplet factor
    const msrTupletFactor&
                          getTupletFactor () const
                              { return fTupletFactor; }

    // member notes durations
    rational              getMemberNotesSoundingWholeNotes () const
                              { return fMemberNotesSoundingWholeNotes; }
    rational              getMemberNotesDisplayWholeNotes () const
                              { return fMemberNotesDisplayWholeNotes; }

    // elements
    const list<S_msrTupletElement>&
                          getTupletElementsList () const
                              { return fTupletElementsList; }

    // tuplet durations
    rational              getTupletSoundingWholeNotes () const
                    // JMI          { return fTupletSoundingWholeNotes; }
                              { return fMeasureElementSoundingWholeNotes; }

    rational              getTupletDisplayWholeNotes () const
                              { return fTupletDisplayWholeNotes; }

  public:

    // public services
    // ------------------------------------------------------

    void                  appendNoteToTuplet (
                            S_msrNote  note,
                            S_msrVoice voice);

    void                  appendChordToTuplet (S_msrChord chord);

    void                  addTupletToTuplet (S_msrTuplet tuplet);

    void                  addTupletToTupletClone (S_msrTuplet tuplet);

    S_msrNote             fetchTupletFirstNonGraceNote () const;

    S_msrNote             removeFirstNoteFromTuplet ( // JMI
                            int inputLineNumber);
    S_msrNote             removeLastNoteFromTuplet (
                            int inputLineNumber);

 // JMI   void                  applyDisplayFactorToTupletMembers ();

    void                  unapplySoundingFactorToTupletMembers (
                            const msrTupletFactor& containingTupletFactor);

    // finalization
    /* JMI
    void                  finalizeTuplet (
                            int inputLineNumber);
*/

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

    virtual void          printShort (ostream& os);

  private:

    // private fields
    // ------------------------------------------------------

    // measure upLink
    S_msrMeasure          fTupletMeasureUpLink;

    // tuplet uplink
    S_msrTuplet           fTupletTupletUpLink;

    // number
    int                   fTupletNumber;

    // appearance
    msrTupletBracketKind  fTupletBracketKind;

    msrTupletLineShapeKind
                          fTupletLineShapeKind;

    msrTupletShowNumberKind
                          fTupletShowNumberKind;

    msrTupletShowTypeKind fTupletShowTypeKind;

    // tuplet factor
    msrTupletFactor       fTupletFactor;

    // member notes durations
    rational              fMemberNotesSoundingWholeNotes;
    rational              fMemberNotesDisplayWholeNotes;

// JMI    rational              fTupletSoundingWholeNotes;
    // display whole notes
    rational              fTupletDisplayWholeNotes;

    // elements
    list<S_msrTupletElement>
                          fTupletElementsList;
};
typedef SMARTP<msrTuplet> S_msrTuplet;
EXP ostream& operator<< (ostream& os, const S_msrTuplet& elt);


}


#endif
