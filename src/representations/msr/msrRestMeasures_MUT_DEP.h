/*
  This file is to be included only by msr_MUTUAL_DEPENDENCIES.h,
  to satisfy declarations mutual dependencies.
*/

#ifndef ___msrRestMeasures_MUT_DEP___
#define ___msrRestMeasures_MUT_DEP___

namespace MusicXML2
{

//______________________________________________________________________________
class msrMeasure;
typedef SMARTP<msrMeasure> S_msrMeasure;

class msrRestMeasures;
typedef SMARTP<msrRestMeasures> S_msrRestMeasures;

//______________________________________________________________________________
class EXP msrRestMeasuresContents : public msrElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrRestMeasuresContents> create (
      int               inputLineNumber,
      S_msrRestMeasures restMeasures);

    SMARTP<msrRestMeasuresContents> createRestMeasuresContentsNewbornClone (
      S_msrRestMeasures restMeasures);

    SMARTP<msrRestMeasuresContents> createRestMeasuresContentsDeepCopy (
      S_msrVoice containingVoice);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrRestMeasuresContents (
      int               inputLineNumber,
      S_msrRestMeasures restMeasures);

    virtual ~msrRestMeasuresContents ();

  public:

    // set and get
    // ------------------------------------------------------

    // upLink
    S_msrRestMeasures
                          getRestMeasuresContentsRestMeasuresUpLink () const
                            {
                              return fRestMeasuresContentsRestMeasuresUpLink;
                            }

    void                  setRestMeasuresContentsSegment (
                            int          inputLineNumber,
                            S_msrSegment restMeasuresContentsSegment);

    S_msrSegment          getRestMeasuresContentsSegment () const
                              { return fRestMeasuresContentsSegment; }

  public:

    // public services
    // ------------------------------------------------------

    int                   restMeasuresContentsMeasuresNumber () const;

    void                  collectRestMeasuresContentsMeasuresIntoFlatList (
                            int inputLineNumber);

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
    S_msrRestMeasures     fRestMeasuresContentsRestMeasuresUpLink;

    S_msrSegment          fRestMeasuresContentsSegment;
};
typedef SMARTP<msrRestMeasuresContents> S_msrRestMeasuresContents;
EXP ostream& operator<< (ostream& os, const S_msrRestMeasuresContents& elt);

//______________________________________________________________________________
class EXP msrRestMeasures : public msrVoiceElement
{
  public:

    // data types
    // ------------------------------------------------------

    enum msrRestMeasuresKind {
      kStartRestMeasures, kStopRestMeasures,
      k_NoRestMeasures
    };

    static string restMeasuresKindAsString (
      msrRestMeasuresKind restMeasuresKind);

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrRestMeasures> create (
      int        inputLineNumber,
      rational   restMeasuresMeasureSoundingNotes,
      int        restMeasuresNumber,
      S_msrVoice voiceUpLink);

    static SMARTP<msrRestMeasures> create (
      int          inputLineNumber,
      S_msrMeasure restMeasureClone,
      S_msrVoice   voiceUpLink);

    SMARTP<msrRestMeasures> createRestMeasuresNewbornClone (
      S_msrVoice containingVoice);

    SMARTP<msrRestMeasures> createRestMeasuresDeepCopy (
      S_msrVoice containingVoice);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrRestMeasures (
      int        inputLineNumber,
      rational   restMeasuresMeasureSoundingNotes,
      int        restMeasuresNumber,
      S_msrVoice voiceUpLink);

    msrRestMeasures (
      int          inputLineNumber,
      S_msrMeasure restMeasureClone,
      S_msrVoice   voiceUpLink);

    virtual ~msrRestMeasures ();

  public:

    // set and get
    // ------------------------------------------------------

    S_msrVoice            getRestMeasuresVoiceUpLink () const
                            { return fRestMeasuresVoiceUpLink; }

    int                   getRestMeasuresNumber () const
                              { return fRestMeasuresNumber; }

    void                  setRestMeasuresContents (
                            S_msrRestMeasuresContents
                              restMeasuresContents);

    S_msrRestMeasuresContents
                          getRestMeasuresContents () const
                              { return fRestMeasuresContents; }

    void                  setRestMeasuresNextMeasureNumber (
                            string nextMeasureNumber);

    string                getRestMeasuresNextMeasureNumber () const
                              { return fRestMeasuresNextMeasureNumber; }

    void                  setRestMeasuresLastMeasurePuristMeasureNumber (
                            int inputLineNumber);

    int                   getRestMeasuresLastMeasurePuristMeasureNumber () const
                              { return fRestMeasuresLastMeasurePuristNumber; }

    rational              getRestMeasuresMeasureSoundingNotes () const
                              { return fRestMeasuresMeasureSoundingNotes; }

  public:

    // public services
    // ------------------------------------------------------

    void                  appendMeasureCloneToRestMeasures (
                            S_msrMeasure measureClone);

    int                   restMeasuresContentsMeasuresNumber () const
                            {
                              return
                                fRestMeasuresContents->
                                  restMeasuresContentsMeasuresNumber ();
                            }

    void                  collectRestMeasuresIntoFlatList (
                            int inputLineNumber);

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

    void                  displayRestMeasures (
                            int    inputLineNumber,
                            string context);

    virtual void          print (ostream& os) const override;

  private:

    // private fields
    // ------------------------------------------------------

    S_msrVoice            fRestMeasuresVoiceUpLink;

    int                   fRestMeasuresNumber;

    S_msrRestMeasuresContents
                          fRestMeasuresContents;

    string                fRestMeasuresNextMeasureNumber;
    int                   fRestMeasuresLastMeasurePuristNumber;

    // shortcut for efficiency
    rational              fRestMeasuresMeasureSoundingNotes;
};
typedef SMARTP<msrRestMeasures> S_msrRestMeasures;
EXP ostream& operator<< (ostream& os, const S_msrRestMeasures& elt);


}


#endif
