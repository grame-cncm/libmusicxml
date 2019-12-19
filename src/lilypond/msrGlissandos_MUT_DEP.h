/*
  This file is to be included only by msrMutualDependencies.h,
  to satisfy declarations mutual dependencies.
*/

//______________________________________________________________________________
class msrGlissando : public msrElement
{
  public:

    // data types
    // ------------------------------------------------------

    enum msrGlissandoTypeKind {
      kGlissandoTypeNone,
      kGlissandoTypeStart, kGlissandoTypeStop };

    static string glissandoTypeKindAsString (
      msrGlissandoTypeKind glissandoTypeKind);

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrGlissando> create (
      int                  inputLineNumber,
      int                  glissandoNumber,
      msrGlissandoTypeKind glissandoTypeKind,
      msrLineTypeKind      glissandoLineTypeKind,
      string               glissandoTextValue);

    SMARTP<msrGlissando> createGlissandoNewbornClone ();

    SMARTP<msrGlissando> createGlissandoDeepCopy ();

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrGlissando (
      int                  inputLineNumber,
      int                  glissandoNumber,
      msrGlissandoTypeKind glissandoTypeKind,
      msrLineTypeKind      glissandoLineTypeKind,
      string               glissandoTextValue);

    virtual ~msrGlissando ();

  public:

    // set and get
    // ------------------------------------------------------

    int                   getGlissandoNumber () const
                              { return fGlissandoNumber; }

    msrGlissandoTypeKind  getGlissandoTypeKind () const
                              { return fGlissandoTypeKind; }

    msrLineTypeKind       getGlissandoLineTypeKind () const
                              { return fGlissandoLineTypeKind; }

    string                getGlissandoTextValue () const
                              { return fGlissandoTextValue; }

    // measure upLink
    void                  setGlissandoMeasureUpLink (
                            const S_msrMeasure& measure)
                              { fGlissandoMeasureUpLink = measure; }

    S_msrMeasure          getGlissandoMeasureUpLink () const
                              { return fGlissandoMeasureUpLink; }

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
    S_msrMeasure          fGlissandoMeasureUpLink;

    int                   fGlissandoNumber;

    msrGlissandoTypeKind  fGlissandoTypeKind;

    msrLineTypeKind       fGlissandoLineTypeKind;

    string                fGlissandoTextValue;
};
typedef SMARTP<msrGlissando> S_msrGlissando;
EXP ostream& operator<< (ostream& os, const S_msrGlissando& elt);

