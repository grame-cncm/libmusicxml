#ifndef ___msdrVoices___
#define ___msdrVoices___

#include "msrVoices.h"
//#include "msrMeasures.h"


namespace MusicXML2
{

//______________________________________________________________________________
class EXP msdrVoice : public msrVoice
{
  public:

    // constants
    // ------------------------------------------------------

    #define K_NO_VOICE_NUMBER                     -99

    #define K_VOICE_HARMONY_VOICE_BASE_NUMBER      20
    #define K_VOICE_FIGURED_BASS_VOICE_BASE_NUMBER 40

  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msdrVoice> create (
                            int          inputLineNumber,
                            msrVoiceKind voiceKind,
                            int          voiceNumber,
                            msrVoiceCreateInitialLastSegmentKind
                                         voiceCreateInitialLastSegmentKind,
                            S_msrStaff   voiceStaffUpLink);

    SMARTP<msdrVoice> createVoiceNewbornClone (
                            S_msrStaff containingStaff);

    SMARTP<msdrVoice> createVoiceDeepCopy (
                            int          inputLineNumber,
                            msrVoiceKind voiceKind,
                            int          voiceNumber,
                            S_msrStaff   containingStaff);

    // creation from the API
    // ------------------------------------------------------

    static SMARTP<msdrVoice> createRegularVoice (
                            int        inputLineNumber,
                            int        voiceNumber,
                            S_msrStaff voiceStaffUpLink);

    static SMARTP<msdrVoice> createHarmonyVoice (
                            int        inputLineNumber,
                            int        voiceNumber,
                            S_msrStaff voiceStaffUpLink);

    static SMARTP<msdrVoice> createFiguredBassVoice (
                            int        inputLineNumber,
                            int        voiceNumber,
                            S_msrStaff voiceStaffUpLink);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    // for regular voices
                          msdrVoice (
                            int          inputLineNumber,
                            msrVoiceKind voiceKind,
                            int          voiceNumber,
                            msrVoiceCreateInitialLastSegmentKind
                                         voiceCreateInitialLastSegmentKind,
                            S_msrStaff   voiceStaffUpLink);

                          msdrVoice (
                            int          inputLineNumber,
                            msrVoiceKind voiceKind,
                            int          voiceNumber);

    // destructor
    virtual               ~msdrVoice ();

  private:

    // initialisation
    // ------------------------------------------------------

    void                  initializeVoice (
                            msrVoiceCreateInitialLastSegmentKind
                             voiceCreateInitialLastSegmentKind);

  public:

    // set and get
    // ------------------------------------------------------

    // measures flat list
    const list<S_msrMeasure>&
                          getVoiceMeasuresFlatList () const
                              { return fVoiceMeasuresFlatList; }

  public:

    // public services
    // ------------------------------------------------------

  private:

    // private services
    // ------------------------------------------------------

  public:

    // visitors
    // ------------------------------------------------------

    void                  acceptIn  (basevisitor* v) override;
    void                  acceptOut (basevisitor* v) override;

    void                  browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    string                asShortString () const override;
    string                asString () const override;

    void                  displayVoice (
                            int    inputLineNumber,
                            string context) const override;

    void                  print (ostream& os) const override;

    void                  printShort (ostream& os) const override;

  private:

    // private fields
    // ------------------------------------------------------

    // measures flat list
    // i.e. without segments nor repeats,
    // gathered from fVoiceInitialElementsList and fVoiceLastSegment
    // by finalizeMeasure()
    list<S_msrMeasure>    fVoiceMeasuresFlatList;

  public:

    // public work services
    // ------------------------------------------------------


  private:

    // work fields
    // ------------------------------------------------------

    // the measures vector
    // each measure can have up to 4 voices
    vector<vector <msrMeasure> >
                          fVoiceMeasuresVectorVector;

    // voice finalization
    bool                  fVoiceHasBeenFinalized;
};
typedef SMARTP<msdrVoice> S_msdrVoice;
EXP ostream& operator<< (ostream& os, const S_msdrVoice& elt);


}


#endif
