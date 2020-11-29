/*
  This file is to be included only by msr_MUTUAL_DEPENDENCIES.h,
  to satisfy declarations mutual dependencies.
*/

#ifndef ___msrSyllables_MUT_DEP___
#define ___msrSyllables_MUT_DEP___

//______________________________________________________________________________
class msrSyllable : public msrMeasureElement
{
  public:

    // data types
    // ------------------------------------------------------

    // we want to end the line in the LilyPond code at a break
    enum msrSyllableKind {
      kSyllableNone,
      kSyllableSingle,
      kSyllableBegin, kSyllableMiddle, kSyllableEnd,

      kSyllableOnRestNote,
      kSyllableSkipRestNote,
      kSyllableSkipNonRestNote,

      kSyllableMeasureEnd,
      kSyllableLineBreak, kSyllablePageBreak};

    static string syllableKindAsString (
      msrSyllableKind syllableKind);

    enum msrSyllableExtendKind {
      kSyllableExtendNone,
      kSyllableExtendEmpty,
      kSyllableExtendSingle,
      kSyllableExtendStart, kSyllableExtendContinue, kSyllableExtendStop };

    static string syllableExtendKindAsString (
      msrSyllableExtendKind syllableExtendKind);


    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrSyllable> create (
      int                   inputLineNumber,
      msrSyllableKind       syllableKind,
      msrSyllableExtendKind syllableExtendKind,
      string                syllableStanzaNumber,
      rational              syllableWholeNotes,
      msrTupletFactor       syllableTupletFactor,
      S_msrStanza           syllableStanzaUpLink);

    static SMARTP<msrSyllable> createWithNextMeasurePuristNumber (
      int                   inputLineNumber,
      msrSyllableKind       syllableKind,
      msrSyllableExtendKind syllableExtendKind,
      string                syllableStanzaNumber,
      rational              syllableWholeNotes,
      msrTupletFactor       syllableTupletFactor,
      S_msrStanza           syllableStanzaUpLink,
      int                   syllableNextMeasurePuristNumber);

    SMARTP<msrSyllable> createSyllableNewbornClone (
      S_msrPart containingPart); // JMI

    SMARTP<msrSyllable> createSyllableDeepCopy (
      S_msrPart containingPart);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrSyllable (
      int                   inputLineNumber,
      msrSyllableKind       syllableKind,
      msrSyllableExtendKind syllableExtendKind,
      string                syllableStanzaNumber,
      rational              syllableWholeNotes,
      msrTupletFactor       syllableTupletFactor,
      S_msrStanza           syllableStanzaUpLink);

    msrSyllable (
      int                   inputLineNumber,
      msrSyllableKind       syllableKind,
      msrSyllableExtendKind syllableExtendKind,
      string                syllableStanzaNumber,
      rational              syllableWholeNotes,
      msrTupletFactor       syllableTupletFactor,
      S_msrStanza           syllableStanzaUpLink,
      int                   syllableNextMeasurePuristNumber);

    virtual ~msrSyllable ();

  public:

    // set and get
    // ------------------------------------------------------

    // upLinks
    S_msrNote             getSyllableNoteUpLink () const
                              { return fSyllableNoteUpLink; }

    S_msrStanza           getSyllableStanzaUpLink () const
                              { return fSyllableStanzaUpLink; }

    // syllable kind
    msrSyllableKind       getSyllableKind () const
                              { return fSyllableKind; }

    // texts list
    const list<string>&   getSyllableTextsList ()
                              { return fSyllableTextsList; }

    // extend kind
    msrSyllableExtendKind getSyllableExtendKind () const
                              { return fSyllableExtendKind; }

    // stanza number
    string                getSyllableStanzaNumber () const
                              { return fSyllableStanzaNumber; }

    // syllable whole notes
    rational              getSyllableWholeNotes () const
                              { return fSyllableWholeNotes; }

    // syllable tuplet factor
    msrTupletFactor       getSyllableTupletFactor () const
                              { return fSyllableTupletFactor; }

    // syllable next measure purist number
    void                  setSyllableNextMeasurePuristNumber (
                            int puristMeasureNumber)
                              {
                                fSyllableNextMeasurePuristNumber =
                                  puristMeasureNumber;
                              }

    int                   getSyllableNextMeasurePuristNumber () const
                              { return fSyllableNextMeasurePuristNumber; }

  public:

    // public services
    // ------------------------------------------------------

    void                  appendSyllableToNoteAndSetItsNoteUpLink (
                            S_msrNote note);

    void                  appendLyricTextToSyllable (string text);

  public:

    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v) override;
    virtual void          acceptOut (basevisitor* v) override;

    virtual void          browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    string                syllableNoteUpLinkAsString () const;

    string                syllableWholeNotesAsMsrString () const;

    string                syllableKindAsString () const;

    string                syllableExtendKindAsString () const;

    string                syllableTextsListAsString () const;

    static void           writeTextsList (
                            const list<string>& textsList,
                            ostream&            os);

    string                asString () const override;

    virtual void          print (ostream& os) const override;

  private:

    // private fields
    // ------------------------------------------------------

    // upLinks
    S_msrNote             fSyllableNoteUpLink;
    S_msrStanza           fSyllableStanzaUpLink;

    // syllable kind
    msrSyllableKind       fSyllableKind;

    // texts list
    list<string>          fSyllableTextsList;

    // extend kind
    msrSyllableExtendKind fSyllableExtendKind;

    // stanza number, may contain non-digits
    string                fSyllableStanzaNumber;

    // syllable whole notes
    rational              fSyllableWholeNotes;

    // syllable tuplet factor
    msrTupletFactor       fSyllableTupletFactor;

    // syllable complementary measure number
    // for kSyllableMeasureEnd, kSyllableLineBreak and kSyllablePageBreak
    int                   fSyllableNextMeasurePuristNumber;
};
typedef SMARTP<msrSyllable> S_msrSyllable;
EXP ostream& operator<< (ostream& os, const S_msrSyllable& elt);


#endif
