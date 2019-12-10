/*
  This file is to be included only by msrMutualDependencies.h,
  to satisfy declarations mutual dependencies.
*/

//______________________________________________________________________________
class msrStanza : public msrElement
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

    // initialization
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

    // services
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

    S_msrSyllable         appendLineBreakSyllableToStanza (
                            int inputLineNumber);

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

    S_msrSyllable         appendLineBreakSyllableToStanza ( // JMI ???
                            int    inputLineNumber,
                            string nextMeasureNumber);

    S_msrSyllable         appendPageBreakSyllableToStanza ( // JMI ???
                            int    inputLineNumber,
                            string nextMeasureNumber);

    void                  padUpToCurrentMeasureWholeNotesDurationInStanza ( // JMI
                            int      inputLineNumber,
                            rational wholeNotes);

    void                  appendPaddingNoteToStanza (
                            int      inputLineNumber,
                            rational forwardStepLength);

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

