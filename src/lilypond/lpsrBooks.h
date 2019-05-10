/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef ___lpsrBooks___
#define ___lpsrBooks___

/*
#include <sstream>
#include <vector>
#include <string>
#include <list>


#include "exports.h"
#include "typedefs.h"

#include "msrBasicTypes.h"


#include "lpsrVarValAssocs.h"

#include "lpsrScheme.h"
*/

#include "lpsrComments.h"

#include "lpsrScheme.h"

#include "lpsrHeaders.h"
#include "lpsrPapers.h"
#include "lpsrLayouts.h"

#include "lpsrParallelMusic.h"
#include "lpsrVarValAssocs.h"

#include "msrScores.h"

#include "msrMidi.h"


namespace MusicXML2
{

//______________________________________________________________________________
class lpsrBookPart : public lpsrElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<lpsrBookPart> create (
      int            inputLineNumber,
      S_msrScore     mScore);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    lpsrBookPart (
      int            inputLineNumber,
      S_msrScore     mScore);

    virtual ~lpsrBookPart ();

  public:

    // set and get
    // ------------------------------------------------------

    S_msrScore            getMsrScore () const
                              { return fMsrScore; }

    S_lpsrVarValAssoc
                          getLilypondVersion () const
                              { return fLilypondVersion; }

    S_lpsrSchemeVariable
                          getGlobalStaffSizeAssoc () const
                              { return fGlobalStaffSizeAssoc; }

    S_lpsrHeader          getHeader () const
                              { return fHeader; }

    S_lpsrPaper           getPaper () const
                              { return fPaper; }

    S_lpsrLayout          getScoreLayout () const
                              { return fScoreLayout; }

    const list<S_msrElement>&
                          getVoicesAndLyricsList () const
                              { return fScoreElements; }

    S_lpsrBookPartBlock      getScoreBlock () const
                              { return fScoreBlock; }

    void                  setGlobalStaffSize (float size);

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
    void                  setCustomShortBarLineSchemeFunctionIsNeeded ();

    bool                  getCustomShortBarLineSchemeFunctionIsNeeded () const
                              { return fCustomShortBarLineSchemeFunctionIsNeeded; }

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

    // glissando with text
    void                  setGlissandoWithTextSchemeFunctionIsNeeded ();

    // damps
    void                  setDampMarkupIsNeeded ();

    void                  setDampAllMarkupIsNeeded ();

    // white note heads
    void                  setWhiteNoteHeadsIsNeeded ();


  public:

    // public services
    // ------------------------------------------------------

/* JMI ???
    void                  appendCommentToScore (
                            S_lpsrComment comment)
                              { fScoreElements.push_back (comment); }

    void                  appendSchemeVariableToScore (
                            S_lpsrSchemeVariable assoc)
                              { fScoreElements.push_back (assoc); }

    void                  prependSchemeVariableToScore (
                            S_lpsrSchemeVariable assoc)
                              { fScoreElements.push_front (assoc); }

    void                  appendVoiceUseToStoreCommand (
                            S_msrVoice voice);

    void                  appendLyricsUseToStoreCommand (
                            S_msrStanza stanza);
*/

    void                  appendVoiceToScoreElements (
                            S_msrVoice voice)
                              { fScoreElements.push_back (voice); }

    void                  appendStanzaToScoreElements (
                            S_msrStanza stanza)
                              { fScoreElements.push_back (stanza); }

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

    void                  addCustomShortBarLineSchemeFunctionToScore ();

    void                  addTongueSchemeFunctionToScore ();

    void                  addEditorialAccidentalSchemeFunctionToScore ();

    void                  addDynamicsSchemeFunctionToScore ();

    void                  addTupletsCurvedBracketsSchemeFunctionToScore ();

    void                  addAfterSchemeFunctionToScore ();

    void                  addTempoRelationshipSchemeFunctionToScore ();

    // markups

    void                  addDampMarkupToScore ();
    void                  addDampAllMarkupToScore ();

    // white note heads

    void                  addWhiteNoteHeadsToScore ();

  public:

    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v);
    virtual void          acceptOut (basevisitor* v);

    virtual void          browseData (basevisitor* v);

  public:

    // print
    // ------------------------------------------------------

    virtual void          print (ostream& os);

  private:

    // fields
    // ------------------------------------------------------

    // MSR data
    S_msrScore            fMsrScore;

    // general information
    S_lpsrVarValAssoc
                          fLilypondVersion;

    S_lpsrComment         fInputSourceNameComment;
    S_lpsrComment         fTranslationDateComment;
    S_lpsrComment         fCommandLineLongOptionsComment;
    S_lpsrComment         fCommandLineShortOptionsComment;

    S_lpsrSchemeVariable
                          fGlobalStaffSizeAssoc;

    S_lpsrHeader          fHeader;
    S_lpsrPaper           fPaper;
    S_lpsrLayout          fScoreLayout;

    // to keep the original line breaks
    S_lpsrVarValAssoc
                          fMyBreakIsBreakAssoc;
    S_lpsrVarValAssoc
                          fMyBreakIsEmptyAssoc;

    // to keep the original page breaks
    S_lpsrVarValAssoc
                          fMyPageBreakIsPageBreakAssoc;
    S_lpsrVarValAssoc
                          fMyPageBreakIsEmptyAssoc;

    // to generate 'global' variable
    S_lpsrVarValAssoc
                          fGlobalAssoc;

    // variables, voices and stanzas
    list<S_msrElement>    fScoreElements;

    // score command
    S_lpsrBookPartBlock       fScoreBlock;

    // files includes
    bool                  fJianpuFileIncludeIsNeeded;

    // Scheme modules
    bool                  fScmAndAccregSchemeModulesAreNeeded;

    // Scheme functions
    bool                  fTongueSchemeFunctionIsNeeded;
    bool                  fCustomShortBarLineSchemeFunctionIsNeeded;
    bool                  fEditorialAccidentalSchemeFunctionIsNeeded;
    bool                  fDynamicsSchemeFunctionIsNeeded;
    bool                  fTupletsCurvedBracketsSchemeFunctionIsNeeded;
    bool                  fAfterSchemeFunctionIsNeeded;
    bool                  fTempoRelationshipSchemeFunctionIsNeeded;
    bool                  fGlissandoWithTextSchemeFunctionIsNeeded;

    // markups
    bool                  fDampMarkupIsNeeded;
    bool                  fDampAllMarkupIsNeeded;

    // white note heads
    bool                  fWhiteNoteHeadsIsNeeded;

    map<string, S_lpsrSchemeFunction>
                          fScoreSchemeFunctionsMap;
};
typedef SMARTP<lpsrBookPart> S_lpsrBookPart;
EXP ostream& operator<< (ostream& os, const S_lpsrBookPart& elt);

//______________________________________________________________________________
class lpsrBook : public lpsrElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<lpsrBook> create (
      int            inputLineNumber,
      S_msrScore     mScore);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    lpsrBook (
      int            inputLineNumber,
      S_msrScore     mScore);

    virtual ~lpsrBook ();

  public:

    // set and get
    // ------------------------------------------------------

    S_msrScore            getMsrScore () const
                              { return fMsrScore; }

    S_lpsrVarValAssoc
                          getLilypondVersion () const
                              { return fLilypondVersion; }

    S_lpsrSchemeVariable
                          getGlobalStaffSizeAssoc () const
                              { return fGlobalStaffSizeAssoc; }

    S_lpsrHeader          getHeader () const
                              { return fHeader; }

    S_lpsrPaper           getPaper () const
                              { return fPaper; }

    S_lpsrLayout          getScoreLayout () const
                              { return fScoreLayout; }

    const list<S_msrElement>&
                          getVoicesAndLyricsList () const
                              { return fScoreElements; }

    S_lpsrBookBlock      getScoreBlock () const
                              { return fScoreBlock; }

    void                  setGlobalStaffSize (float size);

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
    void                  setCustomShortBarLineSchemeFunctionIsNeeded ();

    bool                  getCustomShortBarLineSchemeFunctionIsNeeded () const
                              { return fCustomShortBarLineSchemeFunctionIsNeeded; }

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

    // glissando with text
    void                  setGlissandoWithTextSchemeFunctionIsNeeded ();

    // damps
    void                  setDampMarkupIsNeeded ();

    void                  setDampAllMarkupIsNeeded ();

    // white note heads
    void                  setWhiteNoteHeadsIsNeeded ();


  public:

    // public services
    // ------------------------------------------------------

/* JMI ???
    void                  appendCommentToScore (
                            S_lpsrComment comment)
                              { fScoreElements.push_back (comment); }

    void                  appendSchemeVariableToScore (
                            S_lpsrSchemeVariable assoc)
                              { fScoreElements.push_back (assoc); }

    void                  prependSchemeVariableToScore (
                            S_lpsrSchemeVariable assoc)
                              { fScoreElements.push_front (assoc); }

    void                  appendVoiceUseToStoreCommand (
                            S_msrVoice voice);

    void                  appendLyricsUseToStoreCommand (
                            S_msrStanza stanza);
*/

    void                  appendVoiceToScoreElements (
                            S_msrVoice voice)
                              { fScoreElements.push_back (voice); }

    void                  appendStanzaToScoreElements (
                            S_msrStanza stanza)
                              { fScoreElements.push_back (stanza); }

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

    void                  addCustomShortBarLineSchemeFunctionToScore ();

    void                  addTongueSchemeFunctionToScore ();

    void                  addEditorialAccidentalSchemeFunctionToScore ();

    void                  addDynamicsSchemeFunctionToScore ();

    void                  addTupletsCurvedBracketsSchemeFunctionToScore ();

    void                  addAfterSchemeFunctionToScore ();

    void                  addTempoRelationshipSchemeFunctionToScore ();

    // markups

    void                  addDampMarkupToScore ();
    void                  addDampAllMarkupToScore ();

    // white note heads

    void                  addWhiteNoteHeadsToScore ();

  public:

    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v);
    virtual void          acceptOut (basevisitor* v);

    virtual void          browseData (basevisitor* v);

  public:

    // print
    // ------------------------------------------------------

    virtual void          print (ostream& os);

  private:

    // fields
    // ------------------------------------------------------

    // MSR data
    S_msrScore            fMsrScore;

    // general information
    S_lpsrVarValAssoc
                          fLilypondVersion;

    S_lpsrComment         fInputSourceNameComment;
    S_lpsrComment         fTranslationDateComment;
    S_lpsrComment         fCommandLineLongOptionsComment;
    S_lpsrComment         fCommandLineShortOptionsComment;

    S_lpsrSchemeVariable
                          fGlobalStaffSizeAssoc;

    S_lpsrHeader          fHeader;
    S_lpsrPaper           fPaper;
    S_lpsrLayout          fScoreLayout;

    // to keep the original line breaks
    S_lpsrVarValAssoc
                          fMyBreakIsBreakAssoc;
    S_lpsrVarValAssoc
                          fMyBreakIsEmptyAssoc;

    // to keep the original page breaks
    S_lpsrVarValAssoc
                          fMyPageBreakIsPageBreakAssoc;
    S_lpsrVarValAssoc
                          fMyPageBreakIsEmptyAssoc;

    // to generate 'global' variable
    S_lpsrVarValAssoc
                          fGlobalAssoc;

    // variables, voices and stanzas
    list<S_msrElement>    fScoreElements;

    // score command
    S_lpsrBookBlock       fScoreBlock;

    // files includes
    bool                  fJianpuFileIncludeIsNeeded;

    // Scheme modules
    bool                  fScmAndAccregSchemeModulesAreNeeded;

    // Scheme functions
    bool                  fTongueSchemeFunctionIsNeeded;
    bool                  fCustomShortBarLineSchemeFunctionIsNeeded;
    bool                  fEditorialAccidentalSchemeFunctionIsNeeded;
    bool                  fDynamicsSchemeFunctionIsNeeded;
    bool                  fTupletsCurvedBracketsSchemeFunctionIsNeeded;
    bool                  fAfterSchemeFunctionIsNeeded;
    bool                  fTempoRelationshipSchemeFunctionIsNeeded;
    bool                  fGlissandoWithTextSchemeFunctionIsNeeded;

    // markups
    bool                  fDampMarkupIsNeeded;
    bool                  fDampAllMarkupIsNeeded;

    // white note heads
    bool                  fWhiteNoteHeadsIsNeeded;

    map<string, S_lpsrSchemeFunction>
                          fScoreSchemeFunctionsMap;
};
typedef SMARTP<lpsrBook> S_lpsrBook;
EXP ostream& operator<< (ostream& os, const S_lpsrBook& elt);


}


#endif
