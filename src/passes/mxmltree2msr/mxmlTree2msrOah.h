/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef ___mxmlTree2msrOah___
#define ___mxmlTree2msrOah___

#include "msrSegnosAndCodas.h"

#include "msrBasicTypes.h"

#include "oahAtomsCollection.h"


namespace MusicXML2
{

//______________________________________________________________________________
class EXP msrDalSegnoAtom : public oahAtomWithValue
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<msrDalSegnoAtom> create (
      string              shortName,
      string              longName,
      string              description,
      string              valueSpecification,
      string              variableName,
      map<string, msrDalSegno::msrDalSegnoKind>&
                          stringToDalSegnoKindMapVariable);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrDalSegnoAtom (
      string              shortName,
      string              longName,
      string              description,
      string              valueSpecification,
      string              variableName,
      map<string, msrDalSegno::msrDalSegnoKind>&
                          stringToDalSegnoKindMapVariable);

    virtual ~msrDalSegnoAtom ();

  public:

    // set and get
    // ------------------------------------------------------

    const map<string, msrDalSegno::msrDalSegnoKind>&
                          getStringToDalSegnoKindMapVariable () const
                              { return fStringToDalSegnoKindMapVariable; }

  public:

    // public services
    // ------------------------------------------------------

    void                  applyAtomWithValue (
                            string   theString,
                            ostream& os) override;

  public:

    // visitors
    // ------------------------------------------------------

    void                  acceptIn  (basevisitor* v) override;
    void                  acceptOut (basevisitor* v) override;

    void                  browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    string                asShortNamedOptionString () const override;
    string                asActualLongNamedOptionString () const override;

    void                  print (ostream& os) const override;

    void                  printAtomWithValueOptionsValues (
                            ostream&     os,
                            unsigned int valueFieldWidth) const override;

  private:

    // private fields
    // ------------------------------------------------------

    map<string, msrDalSegno::msrDalSegnoKind>&
                          fStringToDalSegnoKindMapVariable;
};
typedef SMARTP<msrDalSegnoAtom> S_msrDalSegnoAtom;
EXP ostream& operator<< (ostream& os, const S_msrDalSegnoAtom& elt);

//______________________________________________________________________________
class EXP msrReplaceClefAtom : public oahAtomWithValue
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<msrReplaceClefAtom> create (
      string              shortName,
      string              longName,
      string              description,
      string              valueSpecification,
      string              variableName,
      map<msrClefKind, msrClefKind>&
                          clefKindToClefKindMapVariable);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrReplaceClefAtom (
      string              shortName,
      string              longName,
      string              description,
      string              valueSpecification,
      string              variableName,
      map<msrClefKind, msrClefKind>&
                          clefKindToClefKindMapVariable);

    virtual ~msrReplaceClefAtom ();

  public:

    // set and get
    // ------------------------------------------------------

    const map<msrClefKind, msrClefKind>&
                          getClefKindToClefKindMapVariable () const
                              { return fClefKindToClefKindMapVariable; }

  public:

    // public services
    // ------------------------------------------------------

    void                  applyAtomWithValue (
                            string   theString,
                            ostream& os) override;

  public:

    // visitors
    // ------------------------------------------------------

    void                  acceptIn  (basevisitor* v) override;
    void                  acceptOut (basevisitor* v) override;

    void                  browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    string                asShortNamedOptionString () const override;
    string                asActualLongNamedOptionString () const override;

    void                  print (ostream& os) const override;

    void                  printAtomWithValueOptionsValues (
                            ostream&     os,
                            unsigned int valueFieldWidth) const override;

  private:

    // private fields
    // ------------------------------------------------------

    map<msrClefKind, msrClefKind>&
                          fClefKindToClefKindMapVariable;
};
typedef SMARTP<msrReplaceClefAtom> S_msrReplaceClefAtom;
EXP ostream& operator<< (ostream& os, const S_msrReplaceClefAtom& elt);

//______________________________________________________________________________
class EXP mxmlTree2msrOahGroup : public oahGroup
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<mxmlTree2msrOahGroup> create (
      S_oahHandler handler);

  private:

    // initialisation
    // ------------------------------------------------------

    void                  createTheMxmlTree2msrPrefixes (
                            S_oahHandler handler);

    void                  initializeMxmlTree2msrOahGroup ();

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    mxmlTree2msrOahGroup (
      S_oahHandler handler);

    virtual ~mxmlTree2msrOahGroup ();

  public:

    // quiet mode
    // ------------------------------------------------------

    void                  enforceGroupQuietness () override;

  public:

    // consistency check
    // ------------------------------------------------------

    void                  checkGroupOptionsConsistency () override;

  public:

    // set and get
    // ------------------------------------------------------

    // options and help trace
    // --------------------------------------

    S_oahPrefix           getShortIgnoreRedundantPrefix () const
                              { return fShortIgnoreRedundantPrefix; }
    S_oahPrefix           getLongIgnoreRedundantPrefix () const
                              { return fLongIgnoreRedundantPrefix; }

    S_oahPrefix           getShortDelayRestsPrefix () const
                              { return fShortDelayRestsPrefix; }
    S_oahPrefix           getLongDelayRestsPrefix () const
                              { return fLongDelayRestsPrefix; }

    // header
    // --------------------------------------

    bool                  getUseFilenameAsWorkTitle () const
                              { return fUseFilenameAsWorkTitle; }

    bool                  getUseLyricistsAsPoets () const
                              { return fUseLyricistsAsPoets; }

    // parts
    // --------------------------------------

    const set<string>&    getPartsOmitIDSet () const
                              { return fPartsOmitIDSet; }
    const set<string>&    getPartsKeepIDSet () const
                              { return fPartsKeepIDSet; }

    const set<string>&    getPartsOmitNameSet () const
                              { return fPartsOmitNameSet; }
    const set<string>&    getPartsKeepNameSet () const
                              { return fPartsKeepNameSet; }

    // for checkOptionsConsistency()
    S_oahStringSetAtom    getOmitPartIDAtom () const
                              { return fOmitPartIDAtom; }
    S_oahStringSetAtom    getKeepPartIDAtom () const
                              { return fKeepPartIDAtom; }

    S_oahStringSetAtom    getOmitPartNameAtom () const
                              { return fOmitPartNameAtom; }
    S_oahStringSetAtom    getKeepPartNameAtom () const
                              { return fKeepPartNameAtom; }

    // clefs, keys, times
    // --------------------------------------

    const map<msrClefKind, msrClefKind>&
                          getReplaceClefKindToClefKindMapVariable () const
                              { return fReplaceClefKindToClefKindMapVariable; }

    bool                  getIgnoreRedundantClefs () const
                              { return fIgnoreRedundantClefs; }
    bool                  getIgnoreRedundantKeys () const
                              { return fIgnoreRedundantKeys; }
    bool                  getIgnoreRedundantTimes () const
                              { return fIgnoreRedundantTimes; }

    S_oahBooleanAtom      getIgnoreRedundantClefsAtom () const
                              { return fIgnoreRedundantClefsAtom; }
    S_oahBooleanAtom      getIgnoreRedundantKeysAtom () const
                              { return fIgnoreRedundantKeysAtom; }
    S_oahBooleanAtom      getIgnoreRedundantTimesAtom () const
                              { return fIgnoreRedundantTimesAtom; }

    // measures
    // --------------------------------------

    const map<string,int>&
                          getAddEmptyMeasuresStringToIntMap () const
                              { return fAddEmptyMeasuresStringToIntMap; }

    // notes
    // --------------------------------------

    bool                  getDelayRestsDynamics () const
                              { return fDelayRestsDynamics; }
    bool                  getDelayRestsWords () const
                              { return fDelayRestsWords; }
    bool                  getDelayRestsBeams () const
                              { return fDelayRestsBeams; }
    bool                  getDelayRestsSlurs () const
                              { return fDelayRestsSlurs; }
    bool                  getDelayRestsLigatures () const
                              { return fDelayRestsLigatures; }
    bool                  getDelayRestsPedals () const
                              { return fDelayRestsPedals; }
    bool                  getDelayRestsSlashes () const
                              { return fDelayRestsSlashes; }
    bool                  getDelayRestsWedges () const
                              { return fDelayRestsWedges; }

    bool                  getSlashAllGraceNotes () const
                              { return fSlashAllGraceNotes; }
    bool                  getSlurAllGraceNotes () const
                              { return fSlurAllGraceNotes; }
    bool                  getBeamAllGraceNotes () const
                              { return fBeamAllGraceNotes; }

    // articulations
    // --------------------------------------

    bool                  getOmitArticulations () const
                              { return fOmitArticulations; }

    // ornaments
    // --------------------------------------

    bool                  getOmitOrnaments () const
                              { return fOmitOrnaments; }

    // words
    // --------------------------------------

    bool                  getOmitWords () const
                              { return fOmitWords; }

    // ties
    // --------------------------------------

    bool                  getOmitTies () const
                              { return fOmitTies; }

    // dynamics
    // --------------------------------------

    bool                  getOmitDynamics () const
                              { return fOmitDynamics; }

    // slurs
    // --------------------------------------

    bool                  getOmitSlurs () const
                              { return fOmitSlurs; }

    // wedges
    // --------------------------------------

    bool                  getOmitWedges () const
                              { return fOmitWedges; }

    // lyrics
    // --------------------------------------

    bool                  getOmitLyrics () const
                              { return fOmitLyrics; }

    // harmonies
    // --------------------------------------

    bool                  getOmitHarmonies () const
                              { return fOmitHarmonies; }

    // figured bass
    // --------------------------------------

    bool                  getOmitFiguredBasses () const
                              { return fOmitFiguredBasses; }

    // dynamics and wedges
    // --------------------------------------

    bool                  getAllDynamicsBelow () const
                              { return fAllDynamicsBelow; }
    bool                  getAllWedgesBelow () const
                              { return fAllWedgesBelow; }

    // convert words to dal segno
    // --------------------------------------

    const map<string, msrDalSegno::msrDalSegnoKind>&
                          getConvertWordsToDalSegno () const
                              { return fConvertWordsToDalSegno; }

    // combined options, cubase
    // --------------------------------------

    void                  setCubase ()
                              { fCubase = true; }
    bool                  getCubase () const
                              { return fCubase; }

  public:

    // public services
    // ------------------------------------------------------

  private:

    // private services
    // ------------------------------------------------------

#ifdef TRACING_IS_ENABLED
    void                  initializeMxmlTree2msrTraceOptions ();
#endif

    void                  initializeHeaderOptions ();

    void                  initializePartsOptions ();

    void                  initializeClefsKeysTimesOptions ();

    void                  initializeMeasuresOptions ();

    void                  initializeNotesOptions ();

    void                  initializeDynamicsAndWedgesOptions ();

    void                  initializeWordsOptions ();

    void                  initializeCombinedOptionsOptions ();

  public:

    // visitors
    // ------------------------------------------------------

    void                  acceptIn  (basevisitor* v) override;
    void                  acceptOut (basevisitor* v) override;

    void                  browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    void                  printMxmlTree2msrHelp ();

    void                  printMxmlTree2msrValues (int valueFieldWidth);

  private:

    // private fields
    // ------------------------------------------------------

    // options and help trace
    // --------------------------------------

    S_oahPrefix           fShortIgnoreRedundantPrefix;
    S_oahPrefix           fLongIgnoreRedundantPrefix;

    S_oahPrefix           fShortDelayRestsPrefix;
    S_oahPrefix           fLongDelayRestsPrefix;

    // header
    // --------------------------------------

    bool                  fUseFilenameAsWorkTitle;

    bool                  fUseLyricistsAsPoets; // lyricist is unknown to LilyPond

    // parts
    // --------------------------------------

    set<string>           fPartsOmitIDSet;
    set<string>           fPartsKeepIDSet;

    set<string>           fPartsOmitNameSet;
    set<string>           fPartsKeepNameSet;

    // for checkOptionsConsistency()
    S_oahStringSetAtom    fOmitPartIDAtom;
    S_oahStringSetAtom    fKeepPartIDAtom;

    S_oahStringSetAtom    fOmitPartNameAtom;
    S_oahStringSetAtom    fKeepPartNameAtom;

    // clefs, keys, times
    // --------------------------------------

    map<msrClefKind, msrClefKind>
                          fReplaceClefKindToClefKindMapVariable;

    bool                  fIgnoreRedundantClefs;
    bool                  fIgnoreRedundantKeys;
    bool                  fIgnoreRedundantTimes;

    S_oahMultiplexBooleansAtom
                          fIgnoreRedundantMultiplexBooleansAtom;

    // these atoms are used by the '-ignore-redundant*' multiplex options
    S_oahBooleanAtom      fIgnoreRedundantClefsAtom;
    S_oahBooleanAtom      fIgnoreRedundantKeysAtom;
    S_oahBooleanAtom      fIgnoreRedundantTimesAtom;

    // measures
    // --------------------------------------

    map<string,int>       fAddEmptyMeasuresStringToIntMap;

    // notes
    // --------------------------------------

    bool                  fDelayRestsDynamics;
    bool                  fDelayRestsWords;
    bool                  fDelayRestsBeams;
    bool                  fDelayRestsSlurs;
    bool                  fDelayRestsLigatures;
    bool                  fDelayRestsPedals;
    bool                  fDelayRestsSlashes;
    bool                  fDelayRestsWedges;

    bool                  fSlashAllGraceNotes;
    bool                  fSlurAllGraceNotes;
    bool                  fBeamAllGraceNotes;

    // articulations
    // --------------------------------------

    bool                  fOmitArticulations;

    // ornaments
    // --------------------------------------

    bool                  fOmitOrnaments;

    // words
    // --------------------------------------

    bool                  fOmitWords;

    // ties
    // --------------------------------------

    bool                  fOmitTies;

    // dynamics
    // --------------------------------------

    bool                  fOmitDynamics;

    // slurs
    // --------------------------------------

    bool                  fOmitSlurs;

    // wedges
    // --------------------------------------

    bool                  fOmitWedges;

    // lyrics
    // --------------------------------------

    bool                  fOmitLyrics;

    // harmonies
    // --------------------------------------

    bool                  fOmitHarmonies;

    // figured bass
    // --------------------------------------

    bool                  fOmitFiguredBasses;

    // dynamics and wedges
    // --------------------------------------

    bool                  fAllDynamicsBelow;
    bool                  fAllWedgesBelow;

    // convert words to dal segno
    // --------------------------------------

    map<string, msrDalSegno::msrDalSegnoKind>
                          fConvertWordsToDalSegno;

    // combined options, cubase
    // --------------------------------------

    bool                  fCubase;
    S_oahCombinedBooleansAtom
                          fCubaseCombinedBooleansAtom;


#ifdef TRACING_IS_ENABLED
    // specific trace
    // --------------------------------------
#endif
};
typedef SMARTP<mxmlTree2msrOahGroup> S_mxmlTree2msrOahGroup;
EXP ostream& operator<< (ostream& os, const S_mxmlTree2msrOahGroup& elt);

EXP extern S_mxmlTree2msrOahGroup gGlobalMxmlTree2msrOahGroup;

//______________________________________________________________________________
EXP S_mxmlTree2msrOahGroup createGlobalMxmlTree2msrOahGroup (
  S_oahHandler handler);


}


#endif
