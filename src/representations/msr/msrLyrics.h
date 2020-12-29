/*
  This file is to be included only by msr_MUTUAL_DEPENDENCIES.h,
  to satisfy declarations mutual dependencies.
*/

#ifndef ___msrLyrics___
#define ___msrLyrics___

#include "msrElements.h"
#include "msrMeasureElements.h"
#include "msrNotes_MUT_DEP.h"
#include "msrVoices_MUT_DEP.h"


namespace MusicXML2
{

//______________________________________________________________________________
// pre-declaration
class msrStanza;
typedef SMARTP<msrStanza> S_msrStanza;

class msrPart;
typedef SMARTP<msrPart> S_msrPart;

class msrVoice;
typedef SMARTP<msrVoice> S_msrVoice;

class msrNote;
typedef SMARTP<msrNote> S_msrNote;

//______________________________________________________________________________
class EXP msrSyllable : public msrMeasureElement
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
      kSyllableLineBreak, kSyllablePageBreak
    };

    static string syllableKindAsString (
      msrSyllableKind syllableKind);

    enum msrSyllableExtendKind {
      kSyllableExtendNone,
      kSyllableExtendEmpty,
      kSyllableExtendSingle,
      kSyllableExtendStart, kSyllableExtendContinue, kSyllableExtendStop
    };

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

//______________________________________________________________________________
class EXP msrStanza : public msrElement
{
  public:

    // constants
    // ------------------------------------------------------

    #define K_NO_STANZA_NUMBER "-1"
    #define K_NO_STANZA_NAME   "Unknown stanza"

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrStanza> create (
      int           inputLineNumber,
      string        stanzaNumber,
      S_msrVoice    stanzaVoiceUpLink);

    SMARTP<msrStanza> createStanzaNewbornClone (
      S_msrVoice containingVoice);

    SMARTP<msrStanza> createStanzaDeepCopy (
      S_msrVoice containingVoice);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrStanza (
      int           inputLineNumber,
      string        stanzaNumber,
      S_msrVoice    stanzaVoiceUpLink);

  public:

    virtual ~msrStanza ();

  private:

    // private initialization
    // ------------------------------------------------------

    void                  initializeStanza ();

  public:

    // set and get
    // ------------------------------------------------------

    // number
    string                getStanzaNumber () const
                              { return fStanzaNumber; }

    // name
    string                getStanzaName () const
                              { return fStanzaName; }

    // contents
    const vector<S_msrSyllable>&
                          getSyllables () const
                              { return fSyllables; }

    void                  setStanzaTextPresent ()
                              { fStanzaTextPresent = true; }

    bool                  getStanzaTextPresent () const
                              { return fStanzaTextPresent; }

    // upLinks
    S_msrVoice            getStanzaVoiceUpLink () const
                              { return fStanzaVoiceUpLink; }

  public:

    // public services
    // ------------------------------------------------------

    void                  appendSyllableToStanza (
                            S_msrSyllable syllable);

    S_msrSyllable         appendRestSyllableToStanza (
                            int      inputLineNumber,
                            rational wholeNotes);

    S_msrSyllable         appendSkipSyllableToStanza (
                            int      inputLineNumber,
                            rational wholeNotes);

    S_msrSyllable         appendMeasureEndSyllableToStanza (
                            int inputLineNumber);

    S_msrSyllable         appendMelismaSyllableToStanza (
                            int             inputLineNumber,
                            msrSyllable::msrSyllableKind
                                            syllableKind,
                            rational        wholeNote);

/* JMI
    S_msrSyllable         appendTiedSyllableToStanza (
                            int      inputLineNumber,
                            rational wholeNotes);

    S_msrSyllable         appendSlurSyllableToStanza (
                            int      inputLineNumber,
                            rational wholeNotes);

    S_msrSyllable         appendSlurBeyondEndSyllableToStanza (
                            int      inputLineNumber,
                            rational wholeNotes);

    S_msrSyllable         appendLigatureSyllableToStanza (
                            int      inputLineNumber,
                            rational wholeNotes);

    S_msrSyllable         appendLigatureBeyondEndSyllableToStanza (
                            int      inputLineNumber,
                            rational wholeNotes);

    S_msrSyllable         appendBarNumberCheckSyllableToStanza (
                            int    inputLineNumber,
                            string nextMeasureNumber);

    S_msrSyllable         appendBarcheckSyllableToStanza (
                            int    inputLineNumber,
                            string nextMeasureNumber);
*/

    S_msrSyllable         appendLineBreakSyllableToStanza (
                            int inputLineNumber,
                            int nextMeasurePuristNumber);

    S_msrSyllable         appendPageBreakSyllableToStanza (
                            int inputLineNumber,
                            int nextMeasurePuristNumber);

    void                  padUpToCurrentMeasureWholeNotesDurationInStanza ( // JMI
                            int      inputLineNumber,
                            rational wholeNotes);

    void                  appendPaddingNoteToStanza (
                            int      inputLineNumber,
                            rational forwardStepLength);

  public:

    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v) override;
    virtual void          acceptOut (basevisitor* v) override;

    virtual void          browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    virtual void          print (ostream& os) const override;

  private:

    // private fields
    // ------------------------------------------------------

    // upLinks
    S_msrVoice            fStanzaVoiceUpLink;

    // number
    // The lyric number indicates multiple lines,
    // though a name can be used as well (as in Finale's verse/chorus/section specification)
    string                fStanzaNumber;

    // name
    string                fStanzaName;

    // contents
    vector<S_msrSyllable> fSyllables;

    bool                  fStanzaTextPresent;

    // current measure whole notes
    rational              fStanzaCurrentMeasureWholeNotesDuration;
};
typedef SMARTP<msrStanza> S_msrStanza;
EXP ostream& operator<< (ostream& os, const S_msrStanza& elt);


}


#endif
