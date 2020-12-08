/*
  This file is to be included only by msr_MUTUAL_DEPENDENCIES.h,
  to satisfy declarations mutual dependencies.
*/

#ifndef ___msrSpanners_MUT_DEP___
#define ___msrSpanners_MUT_DEP___

//______________________________________________________________________________
// pre-declaration for two-way sideLinks
class msrSpanner;
typedef SMARTP<msrSpanner> S_msrSpanner;

class msrSpanner : public msrElement
{
  public:

    // data types
    // ------------------------------------------------------

    enum msrSpannerKind {
        kSpannerDashes, kSpannerWavyLine };

    static string spannerKindAsString (
      msrSpannerKind spannerKind);

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrSpanner> create (
      int                inputLineNumber,
      int                spannerNumber,
      msrSpannerKind     spannerKind,
      msrSpannerTypeKind spannerTypeKind,
      msrPlacementKind   spannerPlacementKind,
      S_msrNote          spannerNoteUpLink);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrSpanner (
      int                inputLineNumber,
      int                spannerNumber,
      msrSpannerKind     spannerKind,
      msrSpannerTypeKind spannerTypeKind,
      msrPlacementKind   spannerPlacementKind,
      S_msrNote          spannerNoteUpLink);

    virtual ~msrSpanner ();

  public:

    // set and get
    // ------------------------------------------------------

    // upLinks
    void                  setSpannerNoteUpLink (
                            S_msrNote spannerNoteUpLink)
                              { fSpannerNoteUpLink = spannerNoteUpLink; }

    S_msrNote             getSpannerNoteUpLink () const
                              { return fSpannerNoteUpLink; }

    // sideLinks
    void                  setSpannerOtherEndSideLink (
                            S_msrSpanner otherEndSideLink);

    S_msrSpanner          getSpannerOtherEndSideLink () const
                              { return fSpannerOtherEndSideLink; }

    int                   getSpannerNumber () const
                              { return fSpannerNumber; }

    msrSpannerKind        getSpannerKind () const
                              { return fSpannerKind; }

    msrSpannerTypeKind    getSpannerTypeKind () const
                              { return fSpannerTypeKind; }

    msrPlacementKind      getSpannerPlacementKind () const
                              { return fSpannerPlacementKind; }

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

    string                spannerKindAsString () const;

    string                spannerTypeKindAsString () const;

    string                spannerPlacementKindAsString () const;

    string                asShortString () const override;

    virtual void          print (ostream& os) const override;

  protected:

    // upLinks
    // ------------------------------------------------------
    S_msrNote             fSpannerNoteUpLink;

    // sideLinks
    // ------------------------------------------------------
    S_msrSpanner          fSpannerOtherEndSideLink; // two-way

    // fields
    // ------------------------------------------------------

    int                   fSpannerNumber;

    msrSpannerKind        fSpannerKind;

    msrSpannerTypeKind    fSpannerTypeKind;

    msrPlacementKind      fSpannerPlacementKind;
};
typedef SMARTP<msrSpanner> S_msrSpanner;
EXP ostream& operator<< (ostream& os, const S_msrSpanner& elt);


#endif
