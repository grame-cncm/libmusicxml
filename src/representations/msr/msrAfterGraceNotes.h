#ifndef ___msrAfterGraceNotes___
#define ___msrAfterGraceNotes___

#include "msrElements.h"
#include "msrVoices.h"


namespace MusicXML2
{

//______________________________________________________________________________
class EXP msrAfterGraceNotesGroupContents : public msrElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrAfterGraceNotesGroupContents> create (
                            int        inputLineNumber,
                            S_msrVoice afterGraceNotesGroupContentsVoiceUpLink);

    SMARTP<msrAfterGraceNotesGroupContents> createAfterGraceNotesGroupContentsNewbornClone (
                            S_msrVoice containingVoice);

    SMARTP<msrVoice> createAfterGraceNotesGroupContentsDeepCopy (
                            S_msrVoice containingVoice);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msrAfterGraceNotesGroupContents (
                            int        inputLineNumber,
                            S_msrVoice afterGraceNotesGroupContentsVoiceUpLink);

    virtual               ~msrAfterGraceNotesGroupContents ();

  public:

    // set and get
    // ------------------------------------------------------

    const list<S_msrMeasureElement>&
                          getAfterGraceNotesGroupContentsNotesList () const
                              {
                                return
                                  fAfterGraceNotesGroupContentsNotesList;
                              }

  public:

    // public services
    // ------------------------------------------------------

    // upLinks
    S_msrPart             fetchAfterGraceNotesGroupContentsPartUpLink () const;

    // notes
    void                  appendNoteToAfterGraceNotesGroupContents (
                            S_msrNote note);
    void                  appendNoteToAfterGraceNotesGroupContents (
                            S_msrChord chord);

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

    void                  print (ostream& os) const override;

  private:

    // private fields
    // ------------------------------------------------------

    // upLinks
    S_msrVoice            fAfterGraceNotesGroupContentsVoiceUpLink;

    // the notes list
    list<S_msrMeasureElement>
                          fAfterGraceNotesGroupContentsNotesList;
};
typedef SMARTP<msrAfterGraceNotesGroupContents> S_msrAfterGraceNotesGroupContents;
EXP ostream& operator<< (ostream& os, const S_msrAfterGraceNotesGroupContents& elt);

//______________________________________________________________________________
class EXP msrAfterGraceNotesGroup : public msrElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrAfterGraceNotesGroup> create (
                            int          inputLineNumber,
                            S_msrElement afterGraceNotesGroupElement,
                            bool         afterGraceNotesGroupIsSlashed,
                            S_msrVoice   afterGraceNotesGroupVoiceUpLink);

    SMARTP<msrAfterGraceNotesGroup> createAfterGraceNotesGroupNewbornClone (
                            S_msrNote  noteClone,
                            S_msrVoice containingVoice);

    SMARTP<msrVoice> createAfterGraceNotesGroupDeepCopy (
                            S_msrNote  noteClone,
                            S_msrVoice containingVoice);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msrAfterGraceNotesGroup (
                            int          inputLineNumber,
                            S_msrElement afterGraceNotesGroupElement,
                            bool         afterGraceNotesGroupIsSlashed,
                            S_msrVoice   afterGraceNotesGroupVoiceUpLink);

    virtual               ~msrAfterGraceNotesGroup ();

  public:

    // set and get
    // ------------------------------------------------------

    S_msrElement          getAfterGraceNotesGroupElement () const
                              { return fAfterGraceNotesGroupElement; }

    S_msrAfterGraceNotesGroupContents
                          getAfterGraceNotesGroupContents () const
                              { return fAfterGraceNotesGroupContents; }

  public:

    // public services
    // ------------------------------------------------------

    // upLinks
    S_msrPart             fetchAfterGraceNotesGroupPartUpLink () const;

    // notes
    void                  appendNoteToAfterGraceNotesGroupContents (
                            S_msrNote note);

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

    void                  print (ostream& os) const override;

  private:

    // private fields
    // ------------------------------------------------------

    // upLinks
    S_msrVoice            fAfterGraceNotesGroupVoiceUpLink;

    S_msrElement          fAfterGraceNotesGroupElement;

    bool                  fAfterGraceNotesGroupIsSlashed;

    S_msrAfterGraceNotesGroupContents
                          fAfterGraceNotesGroupContents;
};
typedef SMARTP<msrAfterGraceNotesGroup> S_msrAfterGraceNotesGroup;
EXP ostream& operator<< (ostream& os, const S_msrAfterGraceNotesGroup& elt);


}


#endif
