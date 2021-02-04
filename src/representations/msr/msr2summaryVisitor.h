/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef ___msr2summaryVisitor___
#define ___msr2summaryVisitor___

#include "msr.h"

#include "msrOah.h"

#include "oahBasicTypes.h"


namespace MusicXML2
{

//________________________________________________________________________
class EXP msr2summaryVisitor :

  public visitor<S_msrScore>,

  // parts & part groups

  public visitor<S_msrPartGroup>,

  public visitor<S_msrPart>,

  // staves

  public visitor<S_msrStaff>,

  public visitor<S_msrVoice>,

  // lyrics

  public visitor<S_msrStanza>,
  public visitor<S_msrSyllable>,

  public visitor<S_msrClef>,
  public visitor<S_msrKey>,
  public visitor<S_msrTime>,
  public visitor<S_msrTempo>,

  public visitor<S_msrSegment>,

  public visitor<S_msrArticulation>,

  public visitor<S_msrDynamics>,
  public visitor<S_msrWedge>,

  public visitor<S_msrGraceNotesGroup>,

  public visitor<S_msrNote>,

  public visitor<S_msrStem>,
  public visitor<S_msrBeam>,

  public visitor<S_msrChord>,

  public visitor<S_msrTuplet>,

  public visitor<S_msrSlur>,

  public visitor<S_msrBarline>,
  public visitor<S_msrBarNumberCheck>,
  public visitor<S_msrLineBreak>,

  public visitor<S_msrRepeat>,

  public visitor<S_msrScaling>,
  public visitor<S_msrPageLayout>,

  public visitor<S_msrMidiTempo>

{
  public:

                          msr2summaryVisitor (
                            S_msrOahGroup& msrOpts);

    virtual               ~msr2summaryVisitor ();

    void printSummaryFromMsrScore (
      const S_msrScore& score);

  protected:

    virtual void visitStart (S_msrScore& elt);
    virtual void visitEnd   (S_msrScore& elt);

    virtual void visitStart (S_msrPartGroup& elt);
    virtual void visitEnd   (S_msrPartGroup& elt);

    virtual void visitStart (S_msrPart& elt);
    virtual void visitEnd   (S_msrPart& elt);

    virtual void visitStart (S_msrStaff& elt);
    virtual void visitEnd   (S_msrStaff& elt);

    virtual void visitStart (S_msrVoice& elt);
    virtual void visitEnd   (S_msrVoice& elt);

    virtual void visitStart (S_msrStanza& elt);
    virtual void visitEnd   (S_msrStanza& elt);

    virtual void visitStart (S_msrSyllable& elt);
    virtual void visitEnd   (S_msrSyllable& elt);

    virtual void visitStart (S_msrClef& elt);
    virtual void visitEnd   (S_msrClef& elt);

    virtual void visitStart (S_msrKey& elt);
    virtual void visitEnd   (S_msrKey& elt);

    virtual void visitStart (S_msrTime& elt);
    virtual void visitEnd   (S_msrTime& elt);

    virtual void visitStart (S_msrTempo& elt);
    virtual void visitEnd   (S_msrTempo& elt);

    virtual void visitStart (S_msrSegment& elt);
    virtual void visitEnd   (S_msrSegment& elt);

    virtual void visitStart (S_msrArticulation& elt);
    virtual void visitEnd   (S_msrArticulation& elt);

    virtual void visitStart (S_msrDynamics& elt);
    virtual void visitEnd   (S_msrDynamics& elt);

    virtual void visitStart (S_msrWedge& elt);
    virtual void visitEnd   (S_msrWedge& elt);

    virtual void visitStart (S_msrGraceNotesGroup& elt);
    virtual void visitEnd   (S_msrGraceNotesGroup& elt);

    virtual void visitStart (S_msrNote& elt);
    virtual void visitEnd   (S_msrNote& elt);

    virtual void visitStart (S_msrStem& elt);
    virtual void visitEnd   (S_msrStem& elt);

    virtual void visitStart (S_msrBeam& elt);
    virtual void visitEnd   (S_msrBeam& elt);

    virtual void visitStart (S_msrChord& elt);
    virtual void visitEnd   (S_msrChord& elt);

    virtual void visitStart (S_msrTuplet& elt);
    virtual void visitEnd   (S_msrTuplet& elt);

    virtual void visitStart (S_msrSlur& elt);
    virtual void visitEnd   (S_msrSlur& elt);

    virtual void visitStart (S_msrBarline& elt);
    virtual void visitEnd   (S_msrBarline& elt);

    virtual void visitStart (S_msrBarNumberCheck& elt);
    virtual void visitEnd   (S_msrBarNumberCheck& elt);

    virtual void visitStart (S_msrLineBreak& elt);
    virtual void visitEnd   (S_msrLineBreak& elt);

    virtual void visitStart (S_msrRepeat& elt);
    virtual void visitEnd   (S_msrRepeat& elt);

    virtual void visitStart (S_msrScaling& elt);
    virtual void visitEnd   (S_msrScaling& elt);

    virtual void visitStart (S_msrPageLayout& elt);
    virtual void visitEnd   (S_msrPageLayout& elt);

    virtual void visitStart (S_msrMidiTempo& elt);
    virtual void visitEnd   (S_msrMidiTempo& elt);

  private:

    S_msrOahGroup              fMsrOahGroup;

    // score
    // ------------------------------------------------------

    // part groups
    // ------------------------------------------------------

    int                   fPartGroupsCounter;

    // parts
    // ------------------------------------------------------

    int                   fPartsCounter;

    // staves
    // ------------------------------------------------------

    int                   fStavesCounter;

    // prevent clef, key and time from being handled twice
    bool                  fOnGoingStaff;

    // voices
    // ------------------------------------------------------

    int                   fVoicesCounter;

    // stanzas
    // ------------------------------------------------------

    int                   fStanzasCounter;

    // notes
    // ------------------------------------------------------

    int                   fScoreUnpitchedNotesCounter;
    int                   fScoreRegularNotesCounter;
    int                   fScoreRestNotesCounter;
    int                   fScoreSkipNotesCounter;
    int                   fScoreDoubleTremoloNotesCounter;
    int                   fScoreGraceNotesGroupCounter;

    // chords
    // ------------------------------------------------------

    int                   fScoreChordNotesCounter;
    int                   fScoreChordsCounter;

    // tuplets
    // ------------------------------------------------------

    int                   fScoreTupletNotesCounter;
    int                   fScoreTupletsCounter;
};


}


#endif
