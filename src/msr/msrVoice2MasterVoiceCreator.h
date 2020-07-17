/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef ___msrVoice2MasterVoiceCreator___
#define ___msrVoice2MasterVoiceCreator___

#include <map>
#include <vector>

#include "msr.h"


namespace MusicXML2
{

//________________________________________________________________________
class msrVoice2MasterVoiceCreator :

  public visitor<S_msrVoice>,

  // keys

  public visitor<S_msrKey>,

  // times

  public visitor<S_msrTime>,

  // tempo

  public visitor<S_msrTempo>,

  public visitor<S_msrSegment>,

  // rehearsal

  public visitor<S_msrRehearsal>,

  // harmonies

  public visitor<S_msrHarmony>,
  public visitor<S_msrHarmonyDegree>,

  // frames

  public visitor<S_msrFrame>,

  // figured bass

  public visitor<S_msrFiguredBass>,
  public visitor<S_msrFigure>,

  // measures

  public visitor<S_msrMeasure>,

  // fermatas

  public visitor<S_msrFermata>,

  // notes

  public visitor<S_msrNote>,

  // chords

  public visitor<S_msrChord>,

  // tuplets

  public visitor<S_msrTuplet>,

  // ?

  public visitor<S_msrBarline>,

  public visitor<S_msrSegno>,
  public visitor<S_msrCoda>,

  public visitor<S_msrScordatura>,

  public visitor<S_msrBarCheck>,
  public visitor<S_msrBarNumberCheck>,

  public visitor<S_msrLineBreak>,

  public visitor<S_msrPageBreak>,

  // repeats

  public visitor<S_msrRepeat>,
  public visitor<S_msrRepeatCommonPart>,
  public visitor<S_msrRepeatEnding>,

  public visitor<S_msrMeasuresRepeat>,
  public visitor<S_msrMeasuresRepeatPattern>,
  public visitor<S_msrMeasuresRepeatReplicas>,

  public visitor<S_msrRestMeasures>,
  public visitor<S_msrRestMeasuresContents>

{
  public:

    msrVoice2MasterVoiceCreator (
      ostream&   os,
      S_msrVoice mVoice);

    virtual ~msrVoice2MasterVoiceCreator ();

    void buildMsrMasterFromMsrVoice ();

    S_msrVoice getMasterVoice () const
        { return fMasterVoice; }

  protected:

    virtual void visitStart (S_msrVoice& elt);
    virtual void visitEnd   (S_msrVoice& elt);

    virtual void visitStart (S_msrKey& elt);
    virtual void visitEnd   (S_msrKey& elt);

    virtual void visitStart (S_msrTime& elt);
    virtual void visitEnd   (S_msrTime& elt);

    virtual void visitStart (S_msrTempo& elt);
    virtual void visitEnd   (S_msrTempo& elt);

    virtual void visitStart (S_msrRehearsal& elt);
    virtual void visitEnd   (S_msrRehearsal& elt);

    virtual void visitStart (S_msrSegment& elt);
    virtual void visitEnd   (S_msrSegment& elt);

    virtual void visitStart (S_msrHarmony& elt);
    virtual void visitEnd   (S_msrHarmony& elt);
    virtual void visitStart (S_msrHarmonyDegree& elt);

    virtual void visitStart (S_msrFrame& elt);

    virtual void visitStart (S_msrFiguredBass& elt);
    virtual void visitEnd   (S_msrFiguredBass& elt);
    virtual void visitStart (S_msrFigure& elt);

    virtual void visitStart (S_msrMeasure& elt);
    virtual void visitEnd   (S_msrMeasure& elt);

    virtual void visitStart (S_msrFermata& elt);

    virtual void visitStart (S_msrSlash& elt);

    virtual void visitStart (S_msrNote& elt);
    virtual void visitEnd   (S_msrNote& elt);

    virtual void visitStart (S_msrChord& elt);
    virtual void visitEnd   (S_msrChord& elt);

    virtual void visitStart (S_msrTuplet& elt);
    virtual void visitEnd   (S_msrTuplet& elt);

    virtual void visitStart (S_msrBarline& elt);
    virtual void visitEnd   (S_msrBarline& elt);

    virtual void visitStart (S_msrSegno& elt);
    virtual void visitStart (S_msrCoda& elt);

    virtual void visitStart (S_msrScordatura& elt);

    virtual void visitStart (S_msrBarCheck& elt);
    virtual void visitEnd   (S_msrBarCheck& elt);
    virtual void visitStart (S_msrBarNumberCheck& elt);
    virtual void visitEnd   (S_msrBarNumberCheck& elt);

    virtual void visitStart (S_msrLineBreak& elt);
    virtual void visitEnd   (S_msrLineBreak& elt);

    virtual void visitStart (S_msrPageBreak& elt);
    virtual void visitEnd   (S_msrPageBreak& elt);

    virtual void visitStart (S_msrRepeat& elt);
    virtual void visitEnd   (S_msrRepeat& elt);
    virtual void visitStart (S_msrRepeatCommonPart& elt);
    virtual void visitEnd   (S_msrRepeatCommonPart& elt);
    virtual void visitStart (S_msrRepeatEnding& elt);
    virtual void visitEnd   (S_msrRepeatEnding& elt);

    virtual void visitStart (S_msrMeasuresRepeat& elt);
    virtual void visitEnd   (S_msrMeasuresRepeat& elt);
    virtual void visitStart (S_msrMeasuresRepeatPattern& elt);
    virtual void visitEnd   (S_msrMeasuresRepeatPattern& elt);
    virtual void visitStart (S_msrMeasuresRepeatReplicas& elt);
    virtual void visitEnd   (S_msrMeasuresRepeatReplicas& elt);

    virtual void visitStart (S_msrRestMeasures& elt);
    virtual void visitEnd   (S_msrRestMeasures& elt);
    virtual void visitStart (S_msrRestMeasuresContents& elt);
    virtual void visitEnd   (S_msrRestMeasuresContents& elt);

  private:

#ifdef TRACE_OAH
    ostream&                  fLogOutputStream;
#endif

    // the MSR voice we're visiting
    // ------------------------------------------------------
    S_msrVoice                fVisitedMsrVoice;


    // the MSR master voice we're building
    // ------------------------------------------------------
    S_msrVoice                fMasterVoice;


    // harmonies
    // ------------------------------------------------------
    bool                      fOnGoingHarmonyVoice;

    bool                      fOnGoingHarmony;
    S_msrHarmony              fCurrentHarmonyClone;

    list<S_msrHarmony>        fPendingHarmoniesList;


    // figured bass
    // ------------------------------------------------------
    bool                      fOnGoingFiguredBassVoice;
    S_msrFiguredBass          fCurrentFiguredBassClone;


    // segments
    // ------------------------------------------------------
    // the current segment clone is the one at the top of the stack
    S_msrSegment              fCurrentSegmentClone;


    // measures
    // ------------------------------------------------------
    S_msrMeasure              fCurrentMeasureClone;

    // full measure rests compression
    S_msrMeasure              fCurrentRestMeasure;
    S_msrRestMeasures         fCurrentRestMeasures;


    // bar checks
    // ------------------------------------------------------
    S_msrBarCheck             fLastBarCheck;


    // notes
    // ------------------------------------------------------
    bool                      fOnGoingNote;

    // fCurrentNonGraceNoteClone is not used for grace notes,
    // which are visited while the note they're attached to
    // is being visited too
    S_msrNote                 fCurrentNonGraceNoteClone;

    // to help workaround LilyPond issue #34
    S_msrNote                 fFirstNoteCloneInVoice;

    S_msrGraceNotesGroup      fCurrentSkipGraceNotesGroup;


    // chords
    // ------------------------------------------------------
    bool                      fOnGoingChord;
    S_msrChord                fCurrentChordClone;

    // tuplets
    // ------------------------------------------------------
//    S_msrTuplet             fCurrentTupletClone;
 //   bool                      fOnGoingTuplet;
    stack<S_msrTuplet>        fTupletClonesStack;
};


}


#endif
