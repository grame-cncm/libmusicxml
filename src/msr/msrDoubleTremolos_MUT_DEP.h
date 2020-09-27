/*
  This file is to be included only by msrMutualDependencies.h,
  to satisfy declarations mutual dependencies.
*/

#ifndef ___msrDoubleTremolos_MUT_DEP___
#define ___msrDoubleTremolos_MUT_DEP___

//______________________________________________________________________________
class msrDoubleTremolo : public msrMeasureElement
{
  public:

    // data types
    // ------------------------------------------------------

    enum msrDoubleTremoloKind {
      kNotesDoubleTremolo, kChordsDoubleTremolo};

    static string msrDoubleTremoloKindAsString (
      msrDoubleTremoloKind doubleTremolotKind);

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrDoubleTremolo> create (
      int                  inputLineNumber,
      msrDoubleTremoloKind doubleTremoloKind,
      msrTremoloTypeKind   doubleTremoloTypeKind,
      int                  doubleTremoloMarksNumber,
      msrPlacementKind     doubleTremoloPlacementKind);


    SMARTP<msrDoubleTremolo> createDoubleTremoloNewbornClone (
      S_msrElement doubleTremoloFirstElement,
      S_msrElement doubleTremoloSecondElement);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrDoubleTremolo (
      int                  inputLineNumber,
      msrDoubleTremoloKind doubleTremoloKind,
      msrTremoloTypeKind   doubleTremoloTypeKind,
      int                  doubleTremoloMarksNumber,
      msrPlacementKind     doubleTremoloPlacementKind);

    virtual ~msrDoubleTremolo ();

  public:

    // set and get
    // ------------------------------------------------------

    // uplink

    void                  setDoubleTremoloMeasureUpLink (
                            S_msrMeasure measure)
                              { fDoubleTremoloMeasureUpLink = measure; }

    S_msrMeasure          getDoubleTremoloMeasureUpLink () const
                              { return fDoubleTremoloMeasureUpLink; }

    // double tremolo kind

    void                  setDoubleTremoloKind (
                            msrDoubleTremoloKind doubleTremoloKind)
                              { fDoubleTremoloKind = doubleTremoloKind; }

    msrDoubleTremoloKind  getDoubleTremoloKind () const
                              { return fDoubleTremoloKind; }

    // double tremolo type kind

    void                  setDoubleTremoloTypeKind (
                            msrTremoloTypeKind doubleTremoloTypeKind)
                              { fDoubleTremoloTypeKind = doubleTremoloTypeKind; }

    msrTremoloTypeKind    getDoubleTremoloTypeKind () const
                              { return fDoubleTremoloTypeKind; }

    // double tremolo placement

    void                  setDoubleTremoloPlacementKind (
                            msrPlacementKind
                              doubleTremoloPlacementKind)
                              {
                                fDoubleTremoloPlacementKind =
                                  doubleTremoloPlacementKind;
                              }

    msrPlacementKind      getDoubleTremoloPlacementKind () const
                              { return fDoubleTremoloPlacementKind; }

    // double tremolo marks number

    int                   getDoubleTremoloMarksNumber () const
                              { return fDoubleTremoloMarksNumber; }

    // double tremolo elements duration

    rational              getDoubleTremoloElementsDuration () const
                              { return fDoubleTremoloElementsDuration; }

    // double tremolo number of repeats

    void                  setDoubleTremoloNumberOfRepeats (
                            int doubleTremoloNumberOfRepeats)
                              {
                                fDoubleTremoloNumberOfRepeats =
                                  doubleTremoloNumberOfRepeats;
                              }

    int                   getDoubleTremoloNumberOfRepeats () const
                              {
                                return
                                  fDoubleTremoloNumberOfRepeats;
                              }


    // double tremolo placement

    void                  setDoubleTremoloNoteFirstElement (
                            S_msrNote note);

    void                  setDoubleTremoloChordFirstElement (
                            S_msrChord chord);

    // double tremolo first element

    S_msrElement          getDoubleTremoloFirstElement () const
                              { return fDoubleTremoloFirstElement; }

    // double tremolo second element

    void                  setDoubleTremoloNoteSecondElement (
                            S_msrNote note);

    void                  setDoubleTremoloChordSecondElement (
                            S_msrChord chord);

    S_msrElement          getDoubleTremoloSecondElement () const
                              { return fDoubleTremoloSecondElement; }

    // sounding whole notes

    void                  setDoubleTremoloSoundingWholeNotes (
                            rational wholeNotes)
                              {
        // JMI                        fDoubleTremoloSoundingWholeNotes =
                                fMeasureElementSoundingWholeNotes =
                                  wholeNotes;
                              }

    rational              getDoubleTremoloSoundingWholeNotes () const
                              {
                                return
                 // JMI                 fDoubleTremoloSoundingWholeNotes;
                                  fMeasureElementSoundingWholeNotes;
                              }

    // measure number

    void                  setDoubleTremoloMeasureNumber (
                            string measureNumber);

    // position in measure

    void                  setDoubleTremoloPositionInMeasure (
                            rational positionInMeasure);

    // services
    // ------------------------------------------------------

    // tremolo first note

    void                  setDoubleTremoloFirstNotePositionInMeasure (
                            rational positionInMeasure);

    void                  setDoubleTremoloFirstNoteMeasureNumber (
                            string measureNumber);

  public:

    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v) override;
    virtual void          acceptOut (basevisitor* v) override;

    virtual void          browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    string                doubleTremoloPlacementKindAsString () const;

    string                asShortString () const override;
    string                asString () const override;

    virtual void          print (ostream& os) const override;

  private:

    // fields
    // ------------------------------------------------------

    // uplink
    S_msrMeasure          fDoubleTremoloMeasureUpLink;

    // sounding whole notes JMI
    // the same as the displayed divisions of both members
// JMI    rational              fDoubleTremoloSoundingWholeNotes;
    rational              fDoubleTremoloSoundingWholeNotes;

    msrDoubleTremoloKind  fDoubleTremoloKind;

    msrTremoloTypeKind    fDoubleTremoloTypeKind;

    int                   fDoubleTremoloMarksNumber;

    rational              fDoubleTremoloElementsDuration;

    int                   fDoubleTremoloNumberOfRepeats;

    msrPlacementKind      fDoubleTremoloPlacementKind;

    // the two elements of a double tremolo are notes or chords
    S_msrElement          fDoubleTremoloFirstElement;
    S_msrElement          fDoubleTremoloSecondElement;
};
typedef SMARTP<msrDoubleTremolo> S_msrDoubleTremolo;
EXP ostream& operator<< (ostream& os, const S_msrDoubleTremolo& elt);


#endif