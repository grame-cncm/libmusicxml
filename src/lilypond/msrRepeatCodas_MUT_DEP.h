/*
  This file is to be included only by msrMutualDependencies.h,
  to satisfy declarations mutual dependencies.
*/

//______________________________________________________________________________
class msrRepeatCoda : public msrElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrRepeatCoda> create (
      int          inputLineNumber,
      S_msrSegment segment,
      S_msrRepeat  repeatUpLink);

    SMARTP<msrRepeatCoda> createRepeatCodaNewbornClone (
      S_msrRepeat containingRepeat);

    SMARTP<msrRepeatCoda> createRepeatCodaDeepCopy (
      S_msrRepeat containingRepeat);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrRepeatCoda (
      int          inputLineNumber,
      S_msrSegment segment,
      S_msrRepeat  repeatUpLink);

    virtual ~msrRepeatCoda ();

  public:

    // set and get
    // ------------------------------------------------------

    // segment
    S_msrSegment          getRepeatCodaSegment () const
                              { return fRepeatCodaSegment; }

    // upLinks
    S_msrRepeat           getRepeatCodaRepeatUpLink () const
                              { return fRepeatCodaRepeatUpLink; }

    // services
    // ------------------------------------------------------

 //  JMI void                  appendElementToRepeatCoda (S_msrElement elem);

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

    // segment
    S_msrSegment          fRepeatCodaSegment;

    // upLinks
    S_msrRepeat           fRepeatCodaRepeatUpLink;
};
typedef SMARTP<msrRepeatCoda> S_msrRepeatCoda;
EXP ostream& operator<< (ostream& os, const S_msrRepeatCoda& elt);

