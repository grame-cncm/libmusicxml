/*
  This file is to be included only by msrMutualDependencies.h,
  to satisfy declarations mutual dependencies.
*/

#ifndef ___msrGraceNotes_MUT_DEP___
#define ___msrGraceNotes_MUT_DEP___

//______________________________________________________________________________
class msrGraceNotesGroup : public msrElement
{
  public:

    // data types
    // ------------------------------------------------------

    enum msrGraceNotesGroupKind {
      kGraceNotesGroupBefore,
      kGraceNotesGroupAfter };

    static string graceNotesGroupKindAsString (
      msrGraceNotesGroupKind graceNotesGroupKind);

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrGraceNotesGroup> create (
      int                    inputLineNumber,
      msrGraceNotesGroupKind graceNotesGroupKind,
      bool                   graceNotesGroupIsSlashed,
      bool                   graceNotesGroupIsBeamed,
      S_msrVoice             graceNotesGroupVoiceUpLink);

    SMARTP<msrGraceNotesGroup> createGraceNotesGroupNewbornClone (
      S_msrVoice containingVoice);

    SMARTP<msrGraceNotesGroup> createSkipGraceNotesGroupClone (
      S_msrVoice containingVoice);

    SMARTP<msrGraceNotesGroup> createGraceNotesGroupDeepCopy (
      S_msrVoice containingVoice);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrGraceNotesGroup (
      int                    inputLineNumber,
      msrGraceNotesGroupKind graceNotesGroupKind,
      bool                   graceNotesGroupIsSlashed,
      bool                   graceNotesGroupIsBeamed,
      S_msrVoice             graceNotesGroupVoiceUpLink);

    virtual ~msrGraceNotesGroup ();

  public:

    // set and get
    // ------------------------------------------------------

    S_msrVoice            getGraceNotesGroupVoiceUpLink () const
                              { return fGraceNotesGroupVoiceUpLink; }

    void                  setGraceNotesGroupNoteUpLink (
                            S_msrNote note)
                              { fGraceNotesGroupNoteUpLink = note; }

    S_msrNote             getGraceNotesGroupNoteUpLink () const
                              { return fGraceNotesGroupNoteUpLink; }

    msrGraceNotesGroupKind
                          getGraceNotesGroupKind () const
                              { return fGraceNotesGroupKind; }

    void                  setGraceNotesGroupKind (
                            msrGraceNotesGroupKind graceNotesGroupKind)
                              { fGraceNotesGroupKind = graceNotesGroupKind; }

    list<S_msrMeasureElement>&
                          getGraceNotesGroupElementsList ()
                              { return fGraceNotesGroupElementsList; }

    bool                  getGraceNotesGroupIsSlashed () const
                              { return fGraceNotesGroupIsSlashed; }

    void                  setGraceNotesGroupIsTied ()
                              { fGraceNotesGroupIsTied = true; }

    bool                  getGraceNotesGroupIsTied () const
                              { return fGraceNotesGroupIsTied; }

    void                  setGraceNotesGroupIsBeamed ()
                              { fGraceNotesGroupIsBeamed = true; }

    bool                  getGraceNotesGroupIsBeamed () const
                              { return fGraceNotesGroupIsBeamed; }

    void                  setGraceNotesGroupIsFollowedByNotes (bool value)
                              { fGraceNotesGroupIsFollowedByNotes = value; }

    bool                  getGraceNotesGroupIsFollowedByNotes () const
                              { return fGraceNotesGroupIsFollowedByNotes; }

    void                  setGraceNotesGroupMeasureNumber (
                            string graceNotesGroupMeasureNumber)
                              {
                                fGraceNotesGroupMeasureNumber =
                                  graceNotesGroupMeasureNumber;
                              }

    string                getGraceNotesGroupMeasureNumber () const
                              { return fGraceNotesGroupMeasureNumber; }

    // services
    // ------------------------------------------------------

    S_msrPart             fetchGraceNotesGroupPartUpLink () const;

    void                  appendNoteToGraceNotesGroup (S_msrNote note);
    void                  appendChordToGraceNotesGroup (S_msrChord chord);

    S_msrNote             removeLastNoteFromGraceNotesGroup (
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

    virtual void          print (ostream& os) const;

  private:

    // fields
    // ------------------------------------------------------

    // upLinks
    S_msrVoice            fGraceNotesGroupVoiceUpLink;
    S_msrNote             fGraceNotesGroupNoteUpLink;

    msrGraceNotesGroupKind
                          fGraceNotesGroupKind;

    list<S_msrMeasureElement>
                          fGraceNotesGroupElementsList;

    bool                  fGraceNotesGroupIsSlashed;
    bool                  fGraceNotesGroupIsTied;
    bool                  fGraceNotesGroupIsBeamed;

    bool                  fGraceNotesGroupIsFollowedByNotes;

    // LilyPond issue #34 may lead to add skip grace notes to voices
    // other than the one containing these grace notes:
    // the measure number is needed to create the first measure
    // in case the grace notes are at the beginning of the voice
    string                fGraceNotesGroupMeasureNumber; // JMI ???
};
typedef SMARTP<msrGraceNotesGroup> S_msrGraceNotesGroup;
EXP ostream& operator<< (ostream& os, const S_msrGraceNotesGroup& elt);


#endif
