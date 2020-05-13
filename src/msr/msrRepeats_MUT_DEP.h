/*
  This file is to be included only by msrMutualDependencies.h,
  to satisfy declarations mutual dependencies.
*/

//______________________________________________________________________________
/* JMI
class msrRepeatElement : public msrElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrRepeatElement> create (
      int          inputLineNumber,
      S_msrRepeat  repeatUpLink);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrRepeatElement (
      int          inputLineNumber,
      S_msrRepeat  repeatUpLink);

    virtual ~msrRepeatElement ();

  public:

    // set and get
    // ------------------------------------------------------

    // upLinks
    S_msrRepeat           getRepeatElementRepeatUpLink () const
                              { return fRepeatElementRepeatUpLink; }

    // elements
    const list<S_msrVoiceElement>&
                          getRepeatElementElementsList ()
                              { return fRepeatElementElementsList; }

  public:

    // public services
    // ------------------------------------------------------

    void                  appendSegmentToRepeatElementsList (
                            int          inputLineNumber,
                            S_msrSegment segment,
                            string       context);

    void                  appendRepeatToRepeatElementsList (
                            int          inputLineNumber,
                            S_msrRepeat  repeat,
                            string       context);

    void                  appendMeasuresRepeatToRepeatElementsList (
                            int                 inputLineNumber,
                            S_msrMeasuresRepeat measuresRepeat,
                            string              context);

    void                  appendRestMeasuresToRepeatElementsList (
                            int                 inputLineNumber,
                            S_msrMeasuresRepeat measuresRepeat,
                            string              context);

    S_msrNote             fetchRepeatElementFirstNonGraceNote () const;

    void                  collectRepeatElementMeasuresIntoFlatList (
                            int inputLineNumber);

  private:

    // private services
    // ------------------------------------------------------

    void                  appendVoiceElementToRepeatElementsList ( // JMI
                            int               inputLineNumber,
                            S_msrVoiceElement voiceElement,
                            string            context);

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

    virtual void          shortPrint (ostream& os) const;

  private:

    // fields
    // ------------------------------------------------------

    // upLinks
    S_msrRepeat           fRepeatElementRepeatUpLink;

    // elements list
    list<S_msrVoiceElement>
                          fRepeatElementElementsList;
};
typedef SMARTP<msrRepeatElement> S_msrRepeatElement;
EXP ostream& operator<< (ostream& os, const S_msrRepeatElement& elt);
*/

//______________________________________________________________________________
class msrRepeatCommonPart : public msrElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrRepeatCommonPart> create (
      int          inputLineNumber,
      S_msrRepeat  repeatUpLink);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrRepeatCommonPart (
      int          inputLineNumber,
      S_msrRepeat  repeatUpLink);

    virtual ~msrRepeatCommonPart ();

  public:

    // set and get
    // ------------------------------------------------------

    // upLinks
    S_msrRepeat           getRepeatCommonPartRepeatUpLink () const
                              { return fRepeatCommonPartRepeatUpLink; }

    // elements
    const list<S_msrVoiceElement>&
                          getRepeatCommonPartElementsList ()
                              { return fRepeatCommonPartElementsList; }

  public:

    // services
    // ------------------------------------------------------

    void                  appendVoiceElementToRepeatCommonPart (
                            int               inputLineNumber,
                            S_msrVoiceElement voiceElement,
                            string            context);

    void                  appendSegmentToRepeatCommonPart (
                            int          inputLineNumber,
                            S_msrSegment segment,
                            string       context);

    void                  appendRepeatToRepeatCommonPart (
                            int          inputLineNumber,
                            S_msrRepeat  repeat,
                            string       context);

    void                  appendMeasuresRepeatToRepeatCommonPart (
                            int                 inputLineNumber,
                            S_msrMeasuresRepeat measuresRepeat,
                            string              context);

    void                  appendRestMeasuresToRepeatCommonPart (
                            int               inputLineNumber,
                            S_msrRestMeasures restMeasures,
                            string            context);

    S_msrNote             fetchRepeatCommonPartFirstNonGraceNote () const;

    void                  collectRepeatCommonPartMeasuresIntoFlatList (
                            int inputLineNumber);

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

    virtual void          shortPrint (ostream& os) const;

  private:

    // fields
    // ------------------------------------------------------

    // upLinks
    S_msrRepeat           fRepeatCommonPartRepeatUpLink;

    // elements list
    list<S_msrVoiceElement>
                          fRepeatCommonPartElementsList;
};
typedef SMARTP<msrRepeatCommonPart> S_msrRepeatCommonPart;
EXP ostream& operator<< (ostream& os, const S_msrRepeatCommonPart& elt);

//______________________________________________________________________________
class msrRepeatEnding : public msrElement
{
  public:

    // data types
    // ------------------------------------------------------

    enum msrRepeatEndingKind {
      kHookedEnding,
      kHooklessEnding};

    static string repeatEndingKindAsString (
      msrRepeatEndingKind repeatEndingKind);

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrRepeatEnding> create (
      int                 inputLineNumber,
      string              repeatEndingNumber, // may be "1, 2"
      msrRepeatEndingKind repeatEndingKind,
      S_msrRepeat         repeatUpLink);

    /* JMI
    SMARTP<msrRepeatEnding> createRepeatEndingNewbornClone (
      S_msrRepeat containingRepeat);
      */

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrRepeatEnding (
      int                 inputLineNumber,
      string              repeatEndingNumber, // may be "1, 2"
      msrRepeatEndingKind repeatEndingKind,
      S_msrRepeat         repeatUpLink);

    virtual ~msrRepeatEnding ();

  public:

    // set and get
    // ------------------------------------------------------

    // upLinks
    S_msrRepeat           getRepeatEndingRepeatUpLink () const
                              { return fRepeatEndingRepeatUpLink; }

    // numbers
    string                getRepeatEndingNumber () const
                              { return fRepeatEndingNumber; }

    void                  setRepeatEndingNumber (int repeatEndingNumber)
                              { fRepeatEndingNumber = repeatEndingNumber; }

    void                  setRepeatEndingInternalNumber (
                            int repeatEndingInternalNumber)
                              {
                                fRepeatEndingInternalNumber =
                                  repeatEndingInternalNumber;
                              }

    int                   getRepeatEndingInternalNumber () const
                              { return fRepeatEndingInternalNumber; }

    // kind
    msrRepeatEndingKind   getRepeatEndingKind () const
                              { return fRepeatEndingKind; }

    // elements
    const list<S_msrVoiceElement>&
                          getRepeatEndingElementsList ()
                              { return fRepeatEndingElementsList; }

    // services
    // ------------------------------------------------------

    void                  appendVoiceElementToRepeatEnding ( // JMI
                            int               inputLineNumber,
                            S_msrVoiceElement voiceElement,
                            string            context);

    void                  appendSegmentToRepeatEnding (
                            int          inputLineNumber,
                            S_msrSegment segment,
                            string       context);

    void                  appendRepeatToRepeatEnding (
                            int          inputLineNumber,
                            S_msrRepeat  repeat,
                            string       context);

    void                  appendMeasuresRepeatToRepeatEnding (
                            int                 inputLineNumber,
                            S_msrMeasuresRepeat measuresRepeat,
                            string              context);

    void                  appendRestMeasuresToRepeatEnding (
                            int                       inputLineNumber,
                            S_msrRestMeasures restMeasures,
                            string                    context);

    void                  collectRepeatEndingMeasuresIntoFlatList (
                            int inputLineNumber);

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

    virtual void          shortPrint (ostream& os) const;

  private:

    // fields
    // ------------------------------------------------------

    // upLinks
    S_msrRepeat           fRepeatEndingRepeatUpLink;

    // numbers
    string                fRepeatEndingNumber; // may be "1, 2"
    int                   fRepeatEndingInternalNumber; // internally assigned

    // kind
    msrRepeatEndingKind   fRepeatEndingKind;

    // elements list
    list<S_msrVoiceElement>
                          fRepeatEndingElementsList;
};
typedef SMARTP<msrRepeatEnding> S_msrRepeatEnding;
EXP ostream& operator<< (ostream& os, const S_msrRepeatEnding& elt);

//______________________________________________________________________________
class msrRepeat : public msrVoiceElement
{
  public:

    // data types
    // ------------------------------------------------------

    enum msrRepeatExplicitStartKind {
      kRepeatExplicitStartNo,
      kRepeatExplicitStartYes };

    static string repeatExplicitStartKindAsString (
      msrRepeatExplicitStartKind repeatExplicitStartKind);

    enum msrRepeatBuildPhaseKind {
      kRepeatBuildPhaseJustCreated,
      kRepeatBuildPhaseInCommonPart,
      kRepeatBuildPhaseInEndings,
      kRepeatBuildPhaseCompleted};

    static string repeatBuildPhaseKindAsString (
      msrRepeatBuildPhaseKind repeatBuildPhaseKind);

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrRepeat> create (
      int        inputLineNumber,
      int        repeatTimes,
      S_msrVoice voiceUpLink);

    SMARTP<msrRepeat> createRepeatNewbornClone (
      S_msrVoice containingVoice);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrRepeat (
      int        inputLineNumber,
      int        repeatTimes,
      S_msrVoice voiceUpLink);

    virtual ~msrRepeat ();

  public:

    // set and get
    // ------------------------------------------------------

    // upLinks
    S_msrVoice            getRepeatVoiceUpLink () const
                              { return fRepeatVoiceUpLink; }
    // times
    int                   getRepeatTimes () const
                              { return fRepeatTimes; }

    void                  setRepeatTimes (int repeatTimes) // JMI
                              { fRepeatTimes = repeatTimes; }

    // implicit start?
    void                  setRepeatExplicitStartKind (
                            msrRepeatExplicitStartKind repeatExplicitStartKind)
                              {
                                fRepeatExplicitStartKind =
                                  repeatExplicitStartKind;
                              }

    msrRepeatExplicitStartKind
                          getRepeatExplicitStartKind () const
                              { return fRepeatExplicitStartKind; }

    // common part
    void                  setRepeatCommonPart (
                            S_msrRepeatCommonPart repeatCommonPart);

    S_msrRepeatCommonPart getRepeatCommonPart () const
                              { return fRepeatCommonPart; }

    // endings
    const vector<S_msrRepeatEnding>&
                          getRepeatEndings () const
                              { return fRepeatEndings; }

  public:

    // public services
    // ------------------------------------------------------

    void                  addRepeatEndingToRepeat (
                            int               inputLineNumber,
                            S_msrRepeatEnding repeatEnding);

    void                  appendSegmentToRepeat (
                            int          inputLineNumber,
                            S_msrSegment segment,
                            string       context);

    void                  appendRepeatToRepeat (
                            int         inputLineNumber,
                            S_msrRepeat repeat,
                            string      context);

    void                  appendMeasuresRepeatToRepeat (
                            int                 inputLineNumber,
                            S_msrMeasuresRepeat measuresRepeat,
                            string              context);

    void                  appendRestMeasuresToRepeat (
                            int                       inputLineNumber,
                            S_msrRestMeasures restMeasures,
                            string                    context);

    S_msrNote             fetchRepeatFirstNonGraceNote () const;

    void                  collectRepeatMeasuresIntoFlatList (
                            int inputLineNumber);

  public:

    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v);
    virtual void          acceptOut (basevisitor* v);

    virtual void          browseData (basevisitor* v);

  public:

    // print
    // ------------------------------------------------------

    string                asShortString () const;
    string                asString () const;

    void                  displayRepeat (
                            int    inputLineNumber,
                            string context);

    virtual void          print (ostream& os) const;

    virtual void          shortPrint (ostream& os) const;

  private:

    // fields
    // ------------------------------------------------------

    // upLinks
    S_msrVoice            fRepeatVoiceUpLink;

    // number of repetitions
    int                   fRepeatTimes;

    // explicit start?
    msrRepeatExplicitStartKind
                          fRepeatExplicitStartKind;

    // common part
    S_msrRepeatCommonPart fRepeatCommonPart;

    // repeat endings
    vector<S_msrRepeatEnding>
                          fRepeatEndings;
    int                   fRepeatEndingsInternalCounter;

  public:

    // public work services
    // ------------------------------------------------------

    // repeat build phase
    void                  setCurrentRepeatBuildPhaseKind (
                            msrRepeatBuildPhaseKind repeatBuildPhaseKind)
                              {
                                fCurrentRepeatBuildPhaseKind =
                                  repeatBuildPhaseKind;
                              }

    msrRepeatBuildPhaseKind
                          getCurrentRepeatBuildPhaseKind () const
                            { return fCurrentRepeatBuildPhaseKind; }

  private:

    // private work services
    // ------------------------------------------------------

  private:

    // work fields
    // ------------------------------------------------------

    // repeat build phase, used when building the repeat
    msrRepeatBuildPhaseKind
                          fCurrentRepeatBuildPhaseKind;
};
typedef SMARTP<msrRepeat> S_msrRepeat;
EXP ostream& operator<< (ostream& os, const S_msrRepeat& elt);

//________________________________________________________________________
struct msrRepeatDescr : public smartable
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<msrRepeatDescr> create (
      int         repeatDescrStartInputLineNumber,
      S_msrRepeat repeatDescrRepeat);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrRepeatDescr (
      int         repeatDescrStartInputLineNumber,
      S_msrRepeat fRepeatDescrRepeat);

    virtual ~msrRepeatDescr ();

  public:

    // set and get
    // ------------------------------------------------------

    S_msrRepeat           getRepeatDescrRepeat () const
                              { return fRepeatDescrRepeat; }

    void                  setRepeatDescrStartInputLineNumber (
                            int inputLineNumber)
                              {
                                fRepeatDescrStartInputLineNumber =
                                  inputLineNumber;
                              }

    int                   getRepeatDescrStartInputLineNumber () const
                              {
                                return
                                  fRepeatDescrStartInputLineNumber;
                              }

    // services
    // ------------------------------------------------------

    string                repeatDescrAsString () const;

    // print
    // ------------------------------------------------------

    virtual void          print (ostream& os) const;

  private:

    // fields
    // ------------------------------------------------------

    // the repeat
    S_msrRepeat           fRepeatDescrRepeat;

    // its start input line number
    int                   fRepeatDescrStartInputLineNumber;
};
typedef SMARTP<msrRepeatDescr> S_msrRepeatDescr;
EXP ostream& operator<< (ostream& os, const S_msrRepeatDescr& elt);

