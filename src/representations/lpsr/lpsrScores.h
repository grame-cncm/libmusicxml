/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef ___lpsrScores___
#define ___lpsrScores___

#include "lpsrComments.h"

#include "lpsrScheme.h"

#include "lpsrHeaders.h"
#include "lpsrPapers.h"
#include "lpsrLayouts.h"

#include "lpsrParallelMusic.h"

#include "lpsrBookBlockElements.h"

#include "msr.h"


namespace MusicXML2
{

//______________________________________________________________________________
class EXP lpsrScore : public lpsrElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<lpsrScore> create (
      int        inputLineNumber,
      S_msrScore mScore);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    lpsrScore (
      int        inputLineNumber,
      S_msrScore mScore);

    virtual ~lpsrScore ();

  public:

    // set and get
    // ------------------------------------------------------

    S_msrScore            getMsrScore () const
                              { return fMsrScore; }

    S_lpsrHeader          getScoreHeader () const
                              { return fScoreHeader; }

    S_lpsrPaper           getScorePaper () const
                              { return fScorePaper; }

    S_lpsrLayout          getScoreLayout () const
                              { return fScoreLayout; }

    const list<S_msrElement>&
                          getScoreElementsList () const
                              { return fScoreElementsList; }

    const list<S_lpsrBookBlock>&
                          getScoreBookBlocksList () const
                              { return fScoreBookBlocksList; }

    S_lpsrScoreBlock      getScoreScoreBlock () const
                              { return fScoreScoreBlock; }

    // files includes
    // ---------------------

    void                  setJianpuFileIncludeIsNeeded ();

    bool                  getJianpuFileIncludeIsNeeded () const
                              { return fJianpuFileIncludeIsNeeded; }

    // Scheme modules
    // ---------------------

    void                  setScmAndAccregSchemeModulesAreNeeded ();

    bool                  getScmAndAccregSchemeModulesAreNeeded () const
                              { return fScmAndAccregSchemeModulesAreNeeded; }

    // Scheme functions
    // ---------------------

    // custom short bar lines
    void                  setCustomShortBarlineSchemeFunctionIsNeeded ();

    bool                  getCustomShortBarlineSchemeFunctionIsNeeded () const
                              { return fCustomShortBarlineSchemeFunctionIsNeeded; }

    // tongue
    void                  setTongueSchemeFunctionIsNeeded ();

    bool                  getTongueSchemeFunctionIsNeeded () const
                              { return fTongueSchemeFunctionIsNeeded; }


    // editorial accidentals
    void                  setEditorialAccidentalSchemeFunctionIsNeeded ();

    bool                  getEditorialAccidentalSchemeFunctionIsNeeded () const
                              {
                                return
                                  fEditorialAccidentalSchemeFunctionIsNeeded;
                              }

    // dynamics
    void                  setDynamicsSchemeFunctionIsNeeded ();

    bool                  getDynamicsSchemeFunctionIsNeeded () const
                              {
                                return
                                  fDynamicsSchemeFunctionIsNeeded;
                              }

    // tuplets curved brackets
    void                  setTupletsCurvedBracketsSchemeFunctionIsNeeded ();

    bool                  getTupletsCurvedBracketsSchemeFunctionIsNeeded () const
                              {
                                return
                                  fTupletsCurvedBracketsSchemeFunctionIsNeeded;
                              }

    // after
    void                  setAfterSchemeFunctionIsNeeded ();

    bool                  getAfterSchemeFunctionIsNeeded () const
                              {
                                return
                                  fAfterSchemeFunctionIsNeeded;
                              }

    // tempo relationships
    void                  setTempoRelationshipSchemeFunctionIsNeeded ();

    bool                  getTempoRelationshipSchemeFunctionIsNeeded () const
                              {
                                return
                                  fTempoRelationshipSchemeFunctionIsNeeded;
                              }

    // glissando with text
    void                  setGlissandoWithTextSchemeFunctionsIsNeeded ();

    bool                  getGlissandoWithTextSchemeFunctionsIsNeeded () const
                              {
                                return
                                  fGlissandoWithTextSchemeFunctionsIsNeeded;
                              }

    // other dynamic
    void                  setOtherDynamicSchemeFunctionIsNeeded ();

    bool                  getOtherDynamicSchemeFunctionIsNeeded () const
                              {
                                return
                                  fOtherDynamicSchemeFunctionIsNeeded;
                              }

    // damps
    void                  setDampMarkupIsNeeded ();

    bool                  getDampMarkupIsNeeded () const
                              {
                                return
                                  fDampMarkupIsNeeded;
                              }

    void                  setDampAllMarkupIsNeeded ();

    bool                  getDampAllMarkupIsNeeded () const
                              {
                                return
                                  fDampAllMarkupIsNeeded;
                              }

    // white note heads
    void                  setWhiteNoteHeadsIsNeeded ();

    bool                  getWhiteNoteHeadsIsNeeded () const
                              {
                                return
                                  fWhiteNoteHeadsIsNeeded;
                              }

    // bar numbers
    void                  setBoxAroundNextBarNumberIsNeeded ();

    bool                  getBoxAroundNextBarNumberIsNeeded () const
                              {
                                return
                                  fBoxAroundNextBarNumberIsNeeded;
                              }

    // jazz chords display
    void                  setJazzChordsDisplayIsNeeded ();

    bool                  getJazzChordsDisplayIsNeeded () const
                              {
                                return
                                  fJazzChordsDisplayIsNeeded;
                              }

    // colored ledger lines
    void                  setColoredLedgerLinesIsNeeded ();

    bool                  getColoredLedgerLinesIsNeeded () const
                              {
                                return
                                  fColoredLedgerLinesIsNeeded;
                              }

    // hidden measure and barline
    void                  setHiddenMeasureAndBarlineIsNeeded ();

    bool                  getHiddenMeasureAndBarlineIsNeeded () const
                              {
                                return
                                  fHiddenMeasureAndBarlineIsNeeded;
                              }

    // merge rests
    void                  setMergeRestsIsNeeded ();

    bool                  getMergeRestsIsNeeded () const
                              {
                                return
                                  fMergeRestsIsNeeded;
                              }

  public:

    // public services
    // ------------------------------------------------------

    void                  appendVoiceToLpsrScoreElementsList (
                            S_msrVoice voice)
                              { fScoreElementsList.push_back (voice); }

    void                  appendStanzaToLpsrScoreElementsList (
                            S_msrStanza stanza)
                              { fScoreElementsList.push_back (stanza); }

    void                  appendBookBlockToBookBlocksList (
                            S_lpsrBookBlock bookBlock)
                              { fScoreBookBlocksList.push_back (bookBlock); }

    void                  addGlissandoWithTextSchemeFunctionsToScore ();

  private:

    // private services
    // ------------------------------------------------------

    // files includes

    void                  addJianpuFileIncludeToScore ();

    // Scheme modules

    void                  addAccordionRegistrationSchemeModulesToScore ();

    // Scheme functions

    void                  addDateAndTimeSchemeFunctionsToScore ();

    void                  addPointAndClickOffSchemeFunctionsToScore ();

    void                  addCustomShortBarlineSchemeFunctionToScore ();

    void                  addTongueSchemeFunctionToScore ();

    void                  addEditorialAccidentalSchemeFunctionToScore ();

    void                  addDynamicsSchemeFunctionToScore ();

    void                  addTupletsCurvedBracketsSchemeFunctionToScore ();

    void                  addAfterSchemeFunctionToScore ();

    void                  addTempoRelationshipSchemeFunctionToScore ();

    void                  addOtherDynamicSchemeFunctionToScore ();

    // markups

    void                  addDampMarkupToScore ();
    void                  addDampAllMarkupToScore ();

    // white note heads

    void                  addWhiteNoteHeadsToScore ();

    // bar numbers
    void                  addBoxAroundNextBarNumberToScore ();

    // jazz chords display

    void                  addJazzChordsDisplayToScore ();

    // colored ledger lines

    void                  addColoredLedgerLinesToScore ();

    // hidden measure and barline

    void                  addHiddenMeasureAndBarlineToScore ();

    // merge rests

    void                  addMergeRestsToScore ();

  public:

    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v) override;
    virtual void          acceptOut (basevisitor* v) override;

    virtual void          browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    virtual void          print (ostream& os) const override;

    virtual void          printShort (ostream& os) const override;

  private:

    // private fields
    // ------------------------------------------------------

    // MSR data
    S_msrScore            fMsrScore;

    // command line
    S_lpsrComment         fInputSourceNameComment;
    S_lpsrComment         fTranslationDateComment;
    S_lpsrComment         fCommandLineAsSuppliedComment;
    S_lpsrComment         fCommandLineLongOptionsComment;
    S_lpsrComment         fCommandLineShortOptionsComment;

    // LilyPond stuff
    S_lpsrHeader          fScoreHeader;
    S_lpsrPaper           fScorePaper;
    S_lpsrLayout          fScoreLayout;

    // variables, voices and stanzas
    list<S_msrElement>    fScoreElementsList;

    // score LPSR book blocks list
    list<S_lpsrBookBlock> fScoreBookBlocksList;
    S_lpsrScoreBlock      fScoreScoreBlock; // JMI ???

    // files includes
    bool                  fJianpuFileIncludeIsNeeded;

    // Scheme modules
    bool                  fScmAndAccregSchemeModulesAreNeeded;

    // Scheme functions
    bool                  fTongueSchemeFunctionIsNeeded;
    bool                  fCustomShortBarlineSchemeFunctionIsNeeded;
    bool                  fEditorialAccidentalSchemeFunctionIsNeeded;
    bool                  fDynamicsSchemeFunctionIsNeeded;
    bool                  fTupletsCurvedBracketsSchemeFunctionIsNeeded;
    bool                  fAfterSchemeFunctionIsNeeded;
    bool                  fTempoRelationshipSchemeFunctionIsNeeded;
    bool                  fGlissandoWithTextSchemeFunctionsIsNeeded;
    bool                  fOtherDynamicSchemeFunctionIsNeeded;

    // markups
    bool                  fDampMarkupIsNeeded;
    bool                  fDampAllMarkupIsNeeded;

    // white note heads
    bool                  fWhiteNoteHeadsIsNeeded;

    // bar numbers
    bool                  fBoxAroundNextBarNumberIsNeeded;

    // jazz chords display
    bool                  fJazzChordsDisplayIsNeeded;

    // colored ledger lines
    bool                  fColoredLedgerLinesIsNeeded;

    // hidden measure and barline
    bool                  fHiddenMeasureAndBarlineIsNeeded;

    // merge rests
    bool                  fMergeRestsIsNeeded;

    // the Scheme functions map
    map<string, S_lpsrSchemeFunction>
                          fScoreSchemeFunctionsMap;
};
typedef SMARTP<lpsrScore> S_lpsrScore;
EXP ostream& operator<< (ostream& os, const S_lpsrScore& elt);


}


#endif
