/*
  This file is to be included only by msrMutualDependencies.h,
  to satisfy declarations mutual dependencies.
*/

//______________________________________________________________________________
class msrSlide : public msrElement
{
  public:

    // data types
    // ------------------------------------------------------

    enum msrSlideTypeKind {
      kSlideTypeNone,
      kSlideTypeStart, kSlideTypeStop };

    static string slideTypeKindAsString (
      msrSlideTypeKind slideTypeKind);

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrSlide> create (
      int              inputLineNumber,
      int              slideNumber,
      msrSlideTypeKind slideTypeKind,
      msrLineTypeKind  slideLineTypeKind,
      string           slideTextValue);

    SMARTP<msrSlide> createSlideNewbornClone ();

    SMARTP<msrSlide> createSlideDeepCopy ();

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrSlide (
      int              inputLineNumber,
      int              slideNumber,
      msrSlideTypeKind slideTypeKind,
      msrLineTypeKind  slideLineTypeKind,
      string           slideTextValue);

    virtual ~msrSlide ();

  public:

    // set and get
    // ------------------------------------------------------

    int                   getSlideNumber () const
                              { return fSlideNumber; }

    msrSlideTypeKind      getSlideTypeKind () const
                              { return fSlideTypeKind; }

    msrLineTypeKind       getSlideLineTypeKind () const
                              { return fSlideLineTypeKind; }

    string                getSlideTextValue () const
                              { return fSlideTextValue; }

    // measure upLink
    void                  setSlideMeasureUpLink (
                            const S_msrMeasure& measure)
                              { fSlideMeasureUpLink = measure; }

    S_msrMeasure          getSlideMeasureUpLink () const
                              { return fSlideMeasureUpLink; }

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

    string                asString () const;

    virtual void          print (ostream& os) const;

  private:

    // fields
    // ------------------------------------------------------

    // upLink
    S_msrMeasure          fSlideMeasureUpLink;

    int                   fSlideNumber;

    msrSlideTypeKind      fSlideTypeKind;

    msrLineTypeKind       fSlideLineTypeKind;

    string                fSlideTextValue;
};
typedef SMARTP<msrSlide> S_msrSlide;
EXP ostream& operator<< (ostream& os, const S_msrSlide& elt);

