/*
  This file is to be included only by msrMutualDependencies.h,
  to satisfy declarations mutual dependencies.
*/

//______________________________________________________________________________
class msrTuplet : public msrTupletElement
{
  public:

    // data types
    // ------------------------------------------------------

    enum msrTupletTypeKind {
      kTupletTypeNone,
      kTupletTypeStart, kTupletTypeContinue, kTupletTypeStop,
      kTupletTypeStartAndStopInARow };

    static string tupletTypeKindAsString (
      msrTupletTypeKind tupletTypeKind);

    enum msrTupletBracketKind {
      kTupletBracketYes, kTupletBracketNo};

    static string tupletBracketKindAsString (
      msrTupletBracketKind tupletBracketKind);

    enum msrTupletLineShapeKind {
      kTupletLineShapeStraight, kTupletLineShapeCurved};

    static string tupletLineShapeKindAsString (
      msrTupletLineShapeKind tupletLineShapeKind);

    enum msrTupletShowNumberKind {
      kTupletShowNumberActual, kTupletShowNumberBoth, kTupletShowNumberNone };

    static string tupletShowNumberKindAsString (
      msrTupletShowNumberKind tupletShowNumberKind);

    enum msrTupletShowTypeKind {
      kTupletShowTypeActual, kTupletShowTypeBoth, kTupletShowTypeNone };

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

    // tuplet uplink
    void                  setTupletTupletUpLink (
                            const S_msrTuplet& tuplet)
                              { fTupletTupletUpLink = tuplet; }

    S_msrTuplet           getTupletTupletUpLink () const
                              { return fTupletTupletUpLink; }

    // measure upLink
    void                  setTupletMeasureUpLink (
                            const S_msrMeasure& measure)
                              { fTupletMeasureUpLink = measure; }

    S_msrMeasure          getTupletMeasureUpLink () const
                              { return fTupletMeasureUpLink; }

    // positions in measures
    rational              setTupletMembersPositionInMeasure (
                            S_msrMeasure measure,
                            rational     positionInMeasure);
                              // returns the position after the tuplet JMI ???

    int                   getTupletNumber () const
                              { return fTupletNumber; }

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

    const msrTupletFactor&
                          getTupletFactor ()
                              { return fTupletFactor; }

    rational              getMemberNotesSoundingWholeNotes () const
                              { return fMemberNotesSoundingWholeNotes; }
    rational              getMemberNotesDisplayWholeNotes () const
                              { return fMemberNotesDisplayWholeNotes; }

/* JMI
    const list<S_msrTupletElement>&
                          getTupletElementsList () const
                              { return fTupletElementsList; }
                              */

    rational              getTupletSoundingWholeNotes () const
                    // JMI          { return fTupletSoundingWholeNotes; }
                              { return fMeasureElementSoundingWholeNotes; }

    rational              getTupletDisplayWholeNotes () const
                              { return fTupletDisplayWholeNotes; }

    // services
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

    virtual void          acceptIn  (basevisitor* v);
    virtual void          acceptOut (basevisitor* v);

    virtual void          browseData (basevisitor* v);

  public:

    // print
    // ------------------------------------------------------

    string                asString () const;

    virtual void          print (ostream& os) const;

    virtual void          printShort (indentedOstream& os);

  private:

    // fields
    // ------------------------------------------------------

    // upLink
    S_msrMeasure          fTupletMeasureUpLink;
    S_msrTuplet           fTupletTupletUpLink;

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

    // member notes
    rational              fMemberNotesSoundingWholeNotes;
    rational              fMemberNotesDisplayWholeNotes;

// JMI    rational              fTupletSoundingWholeNotes;
    // display whole notes
    rational              fTupletDisplayWholeNotes;

    list<S_msrTupletElement>
                          fTupletElementsList;
};
typedef SMARTP<msrTuplet> S_msrTuplet;
EXP ostream& operator<< (ostream& os, const S_msrTuplet& elt);

