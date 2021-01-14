#ifndef ___msrBeatsRepeats___
#define ___msrBeatsRepeats___

#include "msrElements.h"
#include "msrSegments.h"
#include "msrVoices.h"
#include "msrVoiceElements.h"


namespace MusicXML2
{

//______________________________________________________________________________
// pre-declaration
class msrBeatsRepeat;
typedef SMARTP<msrBeatsRepeat> S_msrBeatsRepeat;

class msrVoice;
typedef SMARTP<msrVoice> S_msrVoice;

class msrSegment;
typedef SMARTP<msrSegment> S_msrSegment; // superflous ??? JMI

//______________________________________________________________________________
class EXP msrBeatsRepeatPattern : public msrElement
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

    virtual               ~msrBeatsRepeatPattern ();

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

  public:

    // public services
    // ------------------------------------------------------

    int                   fetchMeasuresNumber () const;

    void                  collectRepeatPatternMeasuresIntoFlatList (
                            int inputLineNumber);

  public:

    // visitors
    // ------------------------------------------------------

    void                  acceptIn  (basevisitor* v) override;
    void                  acceptOut (basevisitor* v) override;

    void                  browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    string                asString () const override;

    void                  print (ostream& os) const override;

  private:

    // private fields
    // ------------------------------------------------------

    // upLinks
    S_msrBeatsRepeat      fBeatsRepeatUpLink;

    // segment
    S_msrSegment          fBeatsRepeatPatternSegment;
};
typedef SMARTP<msrBeatsRepeatPattern> S_msrBeatsRepeatPattern;
EXP ostream& operator<< (ostream& os, const S_msrBeatsRepeatPattern& elt);

//______________________________________________________________________________
class EXP msrBeatsRepeatReplicas : public msrElement
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

    virtual               ~msrBeatsRepeatReplicas ();

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

  public:

    // public services
    // ------------------------------------------------------

    int                   fetchMeasuresNumber () const;

    int                   fetchBeatsRepeatReplicasNumber () const;

    void                  collectRepeatReplicasMeasuresIntoFlatList (
                            int inputLineNumber);

  public:

    // visitors
    // ------------------------------------------------------

    void                  acceptIn  (basevisitor* v) override;
    void                  acceptOut (basevisitor* v) override;

    void                  browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    string                asString () const override;

    void                  print (ostream& os) const override;

  private:

    // private fields
    // ------------------------------------------------------

    // upLinks
    S_msrBeatsRepeat      fBeatsRepeatUpLink;

    // segment
    S_msrSegment          fBeatsRepeatReplicasSegment;
};
typedef SMARTP<msrBeatsRepeatReplicas> S_msrBeatsRepeatReplicas;
EXP ostream& operator<< (ostream& os, const S_msrBeatsRepeatReplicas& elt);

//______________________________________________________________________________
class EXP msrBeatsRepeat : public msrVoiceElement
{
  public:

    // data types
    // ------------------------------------------------------

    enum msrBeatsRepeatKind {
      k_NoBeatsRepeat,
      kStartBeatsRepeat, kStopBeatsRepeat
    };

    static string beatsRepeatKindAsString (
      msrBeatsRepeatKind beatsRepeatKind);

    enum msrBeatsRepeatBuildPhaseKind {
      kBeatsRepeatBuildPhaseJustCreated,
      kBeatsRepeatBuildPhaseInPattern,
      kBeatsRepeatBuildPhaseInReplicas,
      kBeatsRepeatBuildPhaseCompleted
    };

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

    virtual               ~msrBeatsRepeat ();

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

  public:

    // public services
    // ------------------------------------------------------

    int                   fetchBeatsRepeatReplicasNumber () const;

    int                   fetchBeatsRepeatPatternMeasuresNumber () const;
    int                   fetchBeatsRepeatReplicasMeasuresNumber () const;

    void                  collectBeatsRepeatMeasuresIntoFlatList (
                            int inputLineNumber);

  public:

    // visitors
    // ------------------------------------------------------

    void                  acceptIn  (basevisitor* v) override;
    void                  acceptOut (basevisitor* v) override;

    void                  browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    string                asString () const override;

    void                  displayBeatsRepeat (
                            int    inputLineNumber,
                            string context);

    void                  print (ostream& os) const override;

  private:

    // private fields
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


}


#endif
