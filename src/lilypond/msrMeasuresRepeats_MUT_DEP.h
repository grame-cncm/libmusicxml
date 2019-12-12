/*
  This file is to be included only by msrMutualDependencies.h,
  to satisfy declarations mutual dependencies.
*/

//______________________________________________________________________________
/* JMI
class msrMeasuresRepeatElement : public msrElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrMeasuresRepeatElement> create (
      int                 inputLineNumber,
      S_msrMeasuresRepeat measuresRepeatUpLink);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrMeasuresRepeatElement (
      int                 inputLineNumber,
      S_msrMeasuresRepeat measuresRepeatUpLink);

    virtual ~msrMeasuresRepeatElement ();

  public:

    // set and get
    // ------------------------------------------------------

    // upLinks
    S_msrMeasuresRepeat   getMeasuresRepeatElementMeasuresRepeatUpLink () const
                              { return fMeasuresRepeatElementMeasuresRepeatUpLink; }

    // elements
    const list<S_msrVoiceElement>&
                          getMeasuresRepeatElementElementsList ()
                              { return fMeasuresRepeatElementElementsList; }

  public:

    // public services
    // ------------------------------------------------------

    void                  appendSegmentToMeasuresRepeatElementsList (
                            int          inputLineNumber,
                            S_msrSegment segment,
                            string       context);

    void                  appendMeasuresRepeatToMeasuresRepeatElementsList (
                            int                  inputLineNumber,
                            S_msrMeasuresRepeat  measuresRepeat,
                            string               context);


    void                  appendRestMeasuresToMeasuresRepeatElementsList (
                            int                         inputLineNumber,
                            S_msrMeasuresRepeat measuresRepeat,
                            string                      context);

    S_msrNote             fetchMeasuresRepeatElementFirstNonGraceNote () const;

    void                  collectMeasuresRepeatElementMeasuresIntoFlatList (
                            int inputLineNumber);

  private:

    // private services
    // ------------------------------------------------------

    void                  appendVoiceElementToMeasuresRepeatElementsList ( // JMI
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
    S_msrMeasuresRepeat   fMeasuresRepeatElementMeasuresRepeatUpLink;

    // elements list
    list<S_msrVoiceElement>
                          fMeasuresRepeatElementElementsList;
};
typedef SMARTP<msrMeasuresRepeatElement> S_msrMeasuresRepeatElement;
EXP ostream& operator<< (ostream& os, const S_msrMeasuresRepeatElement& elt);
*/

//______________________________________________________________________________
class msrMeasuresRepeatPattern : public msrElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrMeasuresRepeatPattern> create (
      int                 inputLineNumber,
      S_msrMeasuresRepeat measuresRepeatUpLink);

    /* JMI
    SMARTP<msrMeasuresRepeatPattern> createMeasuresRepeatPatternNewbornClone (
      S_msrVoice containingVoice);

    SMARTP<msrMeasuresRepeatPattern> createMeasuresRepeatPatternDeepCopy (
      S_msrVoice containingVoice);
*/

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrMeasuresRepeatPattern (
      int                 inputLineNumber,
      S_msrMeasuresRepeat measuresRepeatUpLink);

    virtual ~msrMeasuresRepeatPattern ();

  public:

    // set and get
    // ------------------------------------------------------

    // segment
    void                  setMeasuresRepeatPatternSegment (
                            S_msrSegment measuresRepeatPatternSegment);

    S_msrSegment          getMeasuresRepeatPatternSegment () const
                              { return fMeasuresRepeatPatternSegment; }

    // upLinks
    S_msrMeasuresRepeat   getMeasuresRepeatUpLink () const
                            { return fMeasuresRepeatUpLink; }

    // services
    // ------------------------------------------------------

    int                   fetchMeasuresNumber () const;

    void                  collectRepeatPatternMeasuresIntoFlatList (
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

  private:

    // fields
    // ------------------------------------------------------

    // upLinks
    S_msrMeasuresRepeat   fMeasuresRepeatUpLink;

    // segment
    S_msrSegment          fMeasuresRepeatPatternSegment;
};
typedef SMARTP<msrMeasuresRepeatPattern> S_msrMeasuresRepeatPattern;
EXP ostream& operator<< (ostream& os, const S_msrMeasuresRepeatPattern& elt);

//______________________________________________________________________________
class msrMeasuresRepeatReplicas : public msrElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrMeasuresRepeatReplicas> create (
      int                 inputLineNumber,
      S_msrMeasuresRepeat measuresRepeatUpLink);

    /* JMI
    SMARTP<msrMeasuresRepeatReplicas> createMeasuresRepeatReplicasNewbornClone (
      S_msrVoice containingVoice);

    SMARTP<msrMeasuresRepeatReplicas> createMeasuresRepeatReplicasDeepCopy (
      S_msrVoice containingVoice);
      */

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrMeasuresRepeatReplicas (
      int                 inputLineNumber,
      S_msrMeasuresRepeat measuresRepeatUpLink);

    virtual ~msrMeasuresRepeatReplicas ();

  public:

    // set and get
    // ------------------------------------------------------

    // upLinks
    S_msrMeasuresRepeat   getMeasuresRepeatUpLink () const
                            { return fMeasuresRepeatUpLink; }
    // segment
    void                  setMeasuresRepeatReplicasSegment (
                            S_msrSegment measuresRepeatReplicasSegment);

    S_msrSegment          getMeasuresRepeatReplicasSegment () const
                              { return fMeasuresRepeatReplicasSegment; }

    // services
    // ------------------------------------------------------

    int                   fetchMeasuresNumber () const;

    int                   fetchMeasuresRepeatReplicasNumber () const;

    void                  collectRepeatReplicasMeasuresIntoFlatList (
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

  private:

    // fields
    // ------------------------------------------------------

    // upLinks
    S_msrMeasuresRepeat   fMeasuresRepeatUpLink;

    // segment
    S_msrSegment          fMeasuresRepeatReplicasSegment;
};
typedef SMARTP<msrMeasuresRepeatReplicas> S_msrMeasuresRepeatReplicas;
EXP ostream& operator<< (ostream& os, const S_msrMeasuresRepeatReplicas& elt);

//______________________________________________________________________________
class msrMeasuresRepeat : public msrVoiceElement
{
  public:

    // data types
    // ------------------------------------------------------

    enum msrMeasuresRepeatKind {
      k_NoMeasuresRepeat,
      kStartMeasuresRepeat, kStopMeasuresRepeat };

    static string measuresRepeatKindAsString (
      msrMeasuresRepeatKind measuresRepeatKind);

    enum msrMeasuresRepeatBuildPhaseKind {
      kMeasuresRepeatBuildPhaseJustCreated,
      kMeasuresRepeatBuildPhaseInPattern,
      kMeasuresRepeatBuildPhaseInReplicas,
      kMeasuresRepeatBuildPhaseCompleted};

    static string measuresRepeatBuildPhaseKindAsString (
      msrMeasuresRepeatBuildPhaseKind measuresRepeatBuildPhaseKind);

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrMeasuresRepeat> create (
      int        inputLineNumber,
      int        measuresRepeatMeasuresNumber,
      int        measuresRepeatSlashesNumber,
      S_msrVoice voiceUpLink);

    SMARTP<msrMeasuresRepeat> createMeasuresRepeatNewbornClone (
      S_msrVoice containingVoice);

    SMARTP<msrMeasuresRepeat> createMeasuresRepeatDeepCopy (
      S_msrVoice containingVoice);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrMeasuresRepeat (
      int        inputLineNumber,
      int        measuresRepeatMeasuresNumber,
      int        measuresRepeatSlashesNumber,
      S_msrVoice voiceUpLink);

    virtual ~msrMeasuresRepeat ();

  public:

    // set and get
    // ------------------------------------------------------

    // upLinks
    S_msrVoice            getMeasuresRepeatVoiceUpLink () const
                            { return fMeasuresRepeatVoiceUpLink; }

    // numbers
    int                   getMeasuresRepeatMeasuresNumber () const
                              { return fMeasuresRepeatMeasuresNumber; }

    int                   getMeasuresRepeatSlashesNumber () const
                              { return fMeasuresRepeatSlashesNumber; }

    // measuresRepeat pattern
    void                  setMeasuresRepeatPattern (
                            S_msrMeasuresRepeatPattern
                              measuresRepeatPattern);

    S_msrMeasuresRepeatPattern
                          getMeasuresRepeatPattern () const
                              { return fMeasuresRepeatPattern; }

    // measures repeat replicas
    void                  setMeasuresRepeatReplicas (
                            S_msrMeasuresRepeatReplicas
                              measuresRepeatReplicas);

    S_msrMeasuresRepeatReplicas
                          getMeasuresRepeatReplicas () const
                              { return fMeasuresRepeatReplicas; }

    // measures repeat build phase
    void                  setCurrentMeasuresRepeatBuildPhaseKind (
                            msrMeasuresRepeatBuildPhaseKind
                              measuresRepeatBuildPhaseKind)
                              {
                                fCurrentMeasuresRepeatBuildPhaseKind =
                                  measuresRepeatBuildPhaseKind;
                              }

    msrMeasuresRepeatBuildPhaseKind
                          getCurrentMeasuresRepeatBuildPhaseKind () const
                            { return fCurrentMeasuresRepeatBuildPhaseKind; }

    // services
    // ------------------------------------------------------

    int                   fetchMeasuresRepeatReplicasNumber () const;

    int                   fetchMeasuresRepeatPatternMeasuresNumber () const;
    int                   fetchMeasuresRepeatReplicasMeasuresNumber () const;

    void                  collectMeasuresRepeatMeasuresIntoFlatList (
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

    void                  displayMeasuresRepeat (
                            int    inputLineNumber,
                            string context);

    virtual void          print (ostream& os) const;

  private:

    // fields
    // ------------------------------------------------------

    // upLinks
    S_msrVoice            fMeasuresRepeatVoiceUpLink;

    // numbers
    int                   fMeasuresRepeatMeasuresNumber;
    int                   fMeasuresRepeatSlashesNumber;

    // measures repeat pattern
    S_msrMeasuresRepeatPattern
                          fMeasuresRepeatPattern;

    // measures repeat replicas
    S_msrMeasuresRepeatReplicas
                          fMeasuresRepeatReplicas;

    // measures repeat build phase, used when building the measures repeat
    msrMeasuresRepeatBuildPhaseKind
                          fCurrentMeasuresRepeatBuildPhaseKind;
};
typedef SMARTP<msrMeasuresRepeat> S_msrMeasuresRepeat;
EXP ostream& operator<< (ostream& os, const S_msrMeasuresRepeat& elt);

