/*
  This file is to be included only by msr_MUTUAL_DEPENDENCIES.h,
  to satisfy declarations mutual dependencies.
*/

#ifndef ___msrGlissandos_MUT_DEP___
#define ___msrGlissandos_MUT_DEP___

//______________________________________________________________________________
class EXP msrGlissando : public msrElement
{
  public:

    // data types
    // ------------------------------------------------------

    enum msrGlissandoTypeKind {
      kGlissandoTypeNone,
      kGlissandoTypeStart, kGlissandoTypeStop
    };

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

  public:

    // public services
    // ------------------------------------------------------

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

  private:

    // private fields
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


#endif
