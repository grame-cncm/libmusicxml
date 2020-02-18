/*
  This file is to be included only by msrMutualDependencies.h,
  to satisfy declarations mutual dependencies.
*/

//______________________________________________________________________________
class msrFigure : public msrElement
{
  public:

    // data types
    // ------------------------------------------------------

    enum msrFigurePrefixKind {
      k_NoFigurePrefix,
      kDoubleFlatPrefix, kFlatPrefix, kFlatFlatPrefix,
      kNaturalPrefix,
      kSharpSharpPrefix, kSharpPrefix, kDoubleSharpPrefix };

    static string figurePrefixKindAsString (
      msrFigurePrefixKind figurePrefixKind);

    enum msrFigureSuffixKind {
      k_NoFigureSuffix,
      kDoubleFlatSuffix, kFlatSuffix, kFlatFlatSuffix,
      kNaturalSuffix,
      kSharpSharpSuffix, kSharpSuffix, kDoubleSharpSuffix,
      kSlashSuffix };

    static string figureSuffixKindAsString (
      msrFigureSuffixKind figureSuffixKind);

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrFigure> create (
      int                 inputLineNumber,
      S_msrPart           figurePartUpLink,
      msrFigurePrefixKind figurePrefixKind,
      int                 figureNumber,
      msrFigureSuffixKind figureSuffixKind);

    SMARTP<msrFigure> createFigureNewbornClone (
      S_msrPart containingPart);

    SMARTP<msrFigure> createFigureDeepCopy ( // JMI ???
      S_msrPart containingPart);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrFigure (
      int                 inputLineNumber,
      S_msrPart           figurePartUpLink,
      msrFigurePrefixKind figurePrefixKind,
      int                 figureNumber,
      msrFigureSuffixKind figureSuffixKind);

    virtual ~msrFigure ();

  public:

    // set and get
    // ------------------------------------------------------

    S_msrPart             getFigurePartUpLink () const
                              { return fFigurePartUpLink; }

    msrFigurePrefixKind
                          getFigurePrefixKind () const
                              { return fFigurePrefixKind; }

    int                   getFigureNumber () const
                              { return fFigureNumber; }

    msrFigureSuffixKind
                          getFigureSuffixKind () const
                              { return fFigureSuffixKind; }

    // services
    // ------------------------------------------------------

  public:

    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v);
    virtual void          acceptOut (basevisitor* v);

    virtual void          browseData (basevisitor* v);

  public:

    // print
    // ------------------------------------------------------

    string                figurePrefixKindAsString () const;
    string                figureSuffixKindAsShortString () const;

    string                asString () const;

    virtual void          print (ostream& os) const;

  private:

    // fields
    // ------------------------------------------------------

    // upLinks
    S_msrPart             fFigurePartUpLink;

    msrFigurePrefixKind
                          fFigurePrefixKind;
    int                   fFigureNumber;
    msrFigureSuffixKind
                          fFigureSuffixKind;
};
typedef SMARTP<msrFigure> S_msrFigure;
EXP ostream& operator<< (ostream& os, const S_msrFigure& elt);

//______________________________________________________________________________
class msrFiguredBass : public msrMeasureElement
{
  public:

    // data types
    // ------------------------------------------------------

    enum msrFiguredBassParenthesesKind {
      kFiguredBassParenthesesYes, kFiguredBassParenthesesNo };

    static string figuredBassParenthesesKindAsString (
      msrFiguredBassParenthesesKind figuredBassParenthesesKind);

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrFiguredBass> create (
      int       inputLineNumber); // JMI ???

    static SMARTP<msrFiguredBass> create (
      int                           inputLineNumber,
 //   S_msrPart                     figuredBassPartUpLink,
      rational                      figuredBassSoundingWholeNotes,
      rational                      figuredBassDisplayWholeNotes,
      msrFiguredBassParenthesesKind figuredBassParenthesesKind,
      msrTupletFactor               figuredBassTupletFactor);

    SMARTP<msrFiguredBass> createFiguredBassNewbornClone (
      S_msrVoice containingVoice);

    SMARTP<msrFiguredBass> createFiguredBassDeepCopy (); // JMI ???

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrFiguredBass (
      int                           inputLineNumber,
 //   S_msrPart                     figuredBassPartUpLink,
      rational                      figuredBassSoundingWholeNotes,
      rational                      figuredBassDisplayWholeNotes,
      msrFiguredBassParenthesesKind figuredBassParenthesesKind,
      msrTupletFactor               figuredBassTupletFactor);

    virtual ~msrFiguredBass ();

  public:

    // set and get
    // ------------------------------------------------------
    void                  setFiguredBassNoteUpLink (
                            S_msrNote note)
                              { fFiguredBassNoteUpLink = note; }

    S_msrNote             getFiguredBassNoteUpLink () const
                             { return fFiguredBassNoteUpLink; }

    void                  setFiguredBassVoiceUpLink (
                            S_msrVoice voice)
                              { fFiguredBassVoiceUpLink = voice; }

    S_msrVoice            getFiguredBassVoiceUpLink () const
                             { return fFiguredBassVoiceUpLink; }

/* JMI
    S_msrPart             getFiguredBassPartUpLink () const
                              { return fFiguredBassPartUpLink; }
*/

    void                  setFiguredBassDisplayWholeNotes (
                            rational wholeNotes)
                              { fFiguredBassDisplayWholeNotes = wholeNotes; }

    rational              getFiguredBassDisplayWholeNotes () const
                              { return fFiguredBassDisplayWholeNotes; }

    msrFiguredBassParenthesesKind
                          getFiguredBassParenthesesKind () const
                              { return fFiguredBassParenthesesKind; }

    const list<S_msrFigure>&
                          getFiguredBassFiguresList ()
                              { return fFiguredBassFiguresList;  }

    void                  setFiguredBassTupletFactor (
                            msrTupletFactor tupletFactor)
                              { fFiguredBassTupletFactor = tupletFactor; }

    msrTupletFactor       getFiguredBassTupletFactor () const
                              { return fFiguredBassTupletFactor; }

    void                  setFiguredBassPositionInMeasure (
                            rational positionInMeasure);

    // services
    // ------------------------------------------------------

    void                  appendFigureToFiguredBass (
                            S_msrFigure figure);

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

  private:

    // fields
    // ------------------------------------------------------

    // upLinks
 // JMI   S_msrPart             fFiguredBassPartUpLink;
    S_msrNote             fFiguredBassNoteUpLink;
    S_msrVoice            fFiguredBassVoiceUpLink;

    rational              fFiguredBassDisplayWholeNotes; // JMI useless???

    msrFiguredBassParenthesesKind
                          fFiguredBassParenthesesKind;

    list<S_msrFigure>     fFiguredBassFiguresList;

    msrTupletFactor       fFiguredBassTupletFactor;
};
typedef SMARTP<msrFiguredBass> S_msrFiguredBass;
EXP ostream& operator<< (ostream& os, const S_msrFiguredBass& elt);

