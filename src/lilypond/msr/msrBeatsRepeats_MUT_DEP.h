/*
  This file is to be included only by msrMutualDependencies.h,
  to satisfy declarations mutual dependencies.
*/

#ifndef ___msrBeatsRepeats_MUT_DEP___
#define ___msrBeatsRepeats_MUT_DEP___

//______________________________________________________________________________
class msrBeatsRepeatPattern : public msrElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrBeatsRepeatPattern> create (
      int              inputLineNumber,
      S_msrBeatsRepeat beatsRepeatUpLink);

    /* JMI
    SMARTP<msrBeatsRepeatPattern> createBeatsRepeatPatternNewbornClone (
      S_msrVoice containingVoice);

    SMARTP<msrBeatsRepeatPattern> createBeatsRepeatPatternDeepCopy (
      S_msrVoice containingVoice);
*/

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrBeatsRepeatPattern (
      int              inputLineNumber,
      S_msrBeatsRepeat beatsRepeatUpLink);

    virtual ~msrBeatsRepeatPattern ();

  public:

    // set and get
    // ------------------------------------------------------

    // segment
    void                  setBeatsRepeatPatternSegment (
                            S_msrSegment beatsRepeatPatternSegment);

    S_msrSegment          getBeatsRepeatPatternSegment () const
                              { return fBeatsRepeatPatternSegment; }

    // upLinks
    S_msrBeatsRepeat      getBeatsRepeatUpLink () const
                            { return fBeatsRepeatUpLink; }

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
    S_msrBeatsRepeat      fBeatsRepeatUpLink;

    // segment
    S_msrSegment          fBeatsRepeatPatternSegment;
};
typedef SMARTP<msrBeatsRepeatPattern> S_msrBeatsRepeatPattern;
EXP ostream& operator<< (ostream& os, const S_msrBeatsRepeatPattern& elt);

//______________________________________________________________________________
class msrBeatsRepeatReplicas : public msrElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrBeatsRepeatReplicas> create (
      int              inputLineNumber,
      S_msrBeatsRepeat beatsRepeatUpLink);

    /* JMI
    SMARTP<msrBeatsRepeatReplicas> createBeatsRepeatReplicasNewbornClone (
      S_msrVoice containingVoice);

    SMARTP<msrBeatsRepeatReplicas> createBeatsRepeatReplicasDeepCopy (
      S_msrVoice containingVoice);
      */

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrBeatsRepeatReplicas (
      int              inputLineNumber,
      S_msrBeatsRepeat beatsRepeatUpLink);

    virtual ~msrBeatsRepeatReplicas ();

  public:

    // set and get
    // ------------------------------------------------------

    // upLinks
    S_msrBeatsRepeat      getBeatsRepeatUpLink () const
                            { return fBeatsRepeatUpLink; }
    // segment
    void                  setBeatsRepeatReplicasSegment (
                            S_msrSegment beatsRepeatReplicasSegment);

    S_msrSegment          getBeatsRepeatReplicasSegment () const
                              { return fBeatsRepeatReplicasSegment; }

    // services
    // ------------------------------------------------------

    int                   fetchMeasuresNumber () const;

    int                   fetchBeatsRepeatReplicasNumber () const;

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
    S_msrBeatsRepeat      fBeatsRepeatUpLink;

    // segment
    S_msrSegment          fBeatsRepeatReplicasSegment;
};
typedef SMARTP<msrBeatsRepeatReplicas> S_msrBeatsRepeatReplicas;
EXP ostream& operator<< (ostream& os, const S_msrBeatsRepeatReplicas& elt);

//______________________________________________________________________________
class msrBeatsRepeat : public msrVoiceElement
{
  public:

    // data types
    // ------------------------------------------------------

    enum msrBeatsRepeatKind {
      k_NoBeatsRepeat,
      kStartBeatsRepeat, kStopBeatsRepeat };

    static string beatsRepeatKindAsString (
      msrBeatsRepeatKind beatsRepeatKind);

    enum msrBeatsRepeatBuildPhaseKind {
      kBeatsRepeatBuildPhaseJustCreated,
      kBeatsRepeatBuildPhaseInPattern,
      kBeatsRepeatBuildPhaseInReplicas,
      kBeatsRepeatBuildPhaseCompleted};

    static string beatsRepeatBuildPhaseKindAsString (
      msrBeatsRepeatBuildPhaseKind beatsRepeatBuildPhaseKind);

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrBeatsRepeat> create (
      int        inputLineNumber,
      int        beatsRepeatMeasuresNumber,
      int        beatsRepeatSlashesNumber,
      S_msrVoice voiceUpLink);

    SMARTP<msrBeatsRepeat> createBeatsRepeatNewbornClone (
      S_msrVoice containingVoice);

    SMARTP<msrBeatsRepeat> createBeatsRepeatDeepCopy (
      S_msrVoice containingVoice);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrBeatsRepeat (
      int        inputLineNumber,
      int        beatsRepeatMeasuresNumber,
      int        beatsRepeatSlashesNumber,
      S_msrVoice voiceUpLink);

    virtual ~msrBeatsRepeat ();

  public:

    // set and get
    // ------------------------------------------------------

    // upLinks
    S_msrVoice            getBeatsRepeatVoiceUpLink () const
                            { return fBeatsRepeatVoiceUpLink; }

    // numbers
    int                   getBeatsRepeatMeasuresNumber () const
                              { return fBeatsRepeatMeasuresNumber; }

    int                   getBeatsRepeatSlashesNumber () const
                              { return fBeatsRepeatSlashesNumber; }

    // beatsRepeat pattern
    void                  setBeatsRepeatPattern (
                            S_msrBeatsRepeatPattern
                              beatsRepeatPattern);

    S_msrBeatsRepeatPattern
                          getBeatsRepeatPattern () const
                              { return fBeatsRepeatPattern; }

    // measures repeat replicas
    void                  setBeatsRepeatReplicas (
                            S_msrBeatsRepeatReplicas
                              beatsRepeatReplicas);

    S_msrBeatsRepeatReplicas
                          getBeatsRepeatReplicas () const
                              { return fBeatsRepeatReplicas; }

    // measures repeat build phase
    void                  setCurrentBeatsRepeatBuildPhaseKind (
                            msrBeatsRepeatBuildPhaseKind
                              beatsRepeatBuildPhaseKind)
                              {
                                fCurrentBeatsRepeatBuildPhaseKind =
                                  beatsRepeatBuildPhaseKind;
                              }

    msrBeatsRepeatBuildPhaseKind
                          getCurrentBeatsRepeatBuildPhaseKind () const
                            { return fCurrentBeatsRepeatBuildPhaseKind; }

    // services
    // ------------------------------------------------------

    int                   fetchBeatsRepeatReplicasNumber () const;

    int                   fetchBeatsRepeatPatternMeasuresNumber () const;
    int                   fetchBeatsRepeatReplicasMeasuresNumber () const;

    void                  collectBeatsRepeatMeasuresIntoFlatList (
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

    void                  displayBeatsRepeat (
                            int    inputLineNumber,
                            string context);

    virtual void          print (ostream& os) const;

  private:

    // fields
    // ------------------------------------------------------

    // upLinks
    S_msrVoice            fBeatsRepeatVoiceUpLink;

    // numbers
    int                   fBeatsRepeatMeasuresNumber;
    int                   fBeatsRepeatSlashesNumber;

    // measures repeat pattern
    S_msrBeatsRepeatPattern
                          fBeatsRepeatPattern;

    // measures repeat replicas
    S_msrBeatsRepeatReplicas
                          fBeatsRepeatReplicas;

    // measures repeat build phase, used when building the measures repeat
    msrBeatsRepeatBuildPhaseKind
                          fCurrentBeatsRepeatBuildPhaseKind;
};
typedef SMARTP<msrBeatsRepeat> S_msrBeatsRepeat;
EXP ostream& operator<< (ostream& os, const S_msrBeatsRepeat& elt);


#endif
