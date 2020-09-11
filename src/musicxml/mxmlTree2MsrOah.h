/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef ___mxmlTree2MsrOah___
#define ___mxmlTree2MsrOah___

#include "msrSegnosAndCodas.h"

#include "oahBasicTypes.h"


namespace MusicXML2
{

//______________________________________________________________________________
class msrDalSegnoAtom : public oahValuedAtom
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
                          stringDalSegnoKindMapVariable);

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
                          stringDalSegnoKindMapVariable);

    virtual ~msrDalSegnoAtom ();

  public:

    // set and get
    // ------------------------------------------------------

    const map<string, msrDalSegno::msrDalSegnoKind>&
                          getStringDalSegnoKindMapVariable ()
                              { return fStringDalSegnoKindMapVariable; }

  public:

    // services
    // ------------------------------------------------------

    S_oahValuedAtom       handleOptionUnderName (
                            string   optionName,
                            ostream& os) override;

    void                  handleValue (
                            string   theString,
                            ostream& os) override;

  public:

    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v) override;
    virtual void          acceptOut (basevisitor* v) override;

    virtual void          browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    string                asShortNamedOptionString () const override;
    string                asActualLongNamedOptionString () const override;

    void                  print (ostream& os) const override;

    void                  printAtomOptionsValues (
                            ostream& os,
                            int      valueFieldWidth) const override;

  private:

    // fields
    // ------------------------------------------------------

    map<string, msrDalSegno::msrDalSegnoKind>&
                          fStringDalSegnoKindMapVariable;
};
typedef SMARTP<msrDalSegnoAtom> S_msrDalSegnoAtom;
EXP ostream& operator<< (ostream& os, const S_msrDalSegnoAtom& elt);

//______________________________________________________________________________
class msrReplaceClefAtom : public oahValuedAtom
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
                          clefKindClefKindMapVariable);

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
                          clefKindClefKindMapVariable);

    virtual ~msrReplaceClefAtom ();

  public:

    // set and get
    // ------------------------------------------------------

    const map<msrClefKind, msrClefKind>&
                          getClefKindClefKindMapVariable ()
                              { return fClefKindClefKindMapVariable; }

  public:

    // services
    // ------------------------------------------------------

    S_oahValuedAtom       handleOptionUnderName (
                            string   optionName,
                            ostream& os) override;

    void                  handleValue (
                            string   theString,
                            ostream& os) override;

  public:

    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v) override;
    virtual void          acceptOut (basevisitor* v) override;

    virtual void          browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    string                asShortNamedOptionString () const override;
    string                asActualLongNamedOptionString () const override;

    void                  print (ostream& os) const override;

    void                  printAtomOptionsValues (
                            ostream& os,
                            int      valueFieldWidth) const override;

  private:

    // fields
    // ------------------------------------------------------

    map<msrClefKind, msrClefKind>&
                          fClefKindClefKindMapVariable;
};
typedef SMARTP<msrReplaceClefAtom> S_msrReplaceClefAtom;
EXP ostream& operator<< (ostream& os, const S_msrReplaceClefAtom& elt);

//______________________________________________________________________________
class mxmlTree2MsrOah : public oahGroup
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<mxmlTree2MsrOah> create (
      S_oahHandler handlerUpLink);

    SMARTP<mxmlTree2MsrOah>   createCloneWithDetailedTrace ();

  private:

    // initialisation
    // ------------------------------------------------------

    void                  initializeMxmlTree2Msr (
                            bool boolOptionsInitialValue);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    mxmlTree2MsrOah (
      S_oahHandler handlerUpLink);

    virtual ~mxmlTree2MsrOah ();

  public:

    // quiet mode
    // ------------------------------------------------------

    void                  enforceQuietness ();

  public:

    // consistency check
    // ------------------------------------------------------

    virtual void          checkOptionsConsistency () override;

  public:

    // set and get
    // ------------------------------------------------------

    void                  setAllMusicXMLTraceOah (
                            bool boolOptionsInitialValue);

  public:

    // public services
    // ------------------------------------------------------

  private:

    // private services
    // ------------------------------------------------------

#ifdef TRACE_OAH
    void                  initializeTraceOah (
                            bool boolOptionsInitialValue);
#endif

    void                  initializeHeaderOptions (
                            bool boolOptionsInitialValue);

    void                  initializePartsOptions (
                            bool boolOptionsInitialValue);

    void                  initializeClefsKeysTimesOptions (
                            bool boolOptionsInitialValue);

    void                  initializeMeasuresOptions (
                            bool boolOptionsInitialValue);

    void                  initializeNotesOptions (
                            bool boolOptionsInitialValue);

    void                  initializeDynamicsAndWedgesOptions (
                            bool boolOptionsInitialValue);

    void                  initializeWordsOptions (
                            bool boolOptionsInitialValue);

    void                  initializeCombinedOptionsOptions (
                            bool boolOptionsInitialValue);

  public:

    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v) override;
    virtual void          acceptOut (basevisitor* v) override;

    virtual void          browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    void                  printMxmlTree2MsrHelp ();

    void                  printMxmlTree2MsrValues (int valueFieldWidth);

  public:

    // fields
    // ------------------------------------------------------

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
                          fReplaceClefMapVariable;

    bool                  fIgnoreRedundantClefs;
    bool                  fIgnoreRedundantKeys;
    bool                  fIgnoreRedundantTimes;

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

#ifdef TRACE_OAH
    // specific trace
    // --------------------------------------
#endif
};
typedef SMARTP<mxmlTree2MsrOah> S_mxmlTree2MsrOah;
EXP ostream& operator<< (ostream& os, const S_mxmlTree2MsrOah& elt);

EXP extern S_mxmlTree2MsrOah globalMxmlTree2MsrOah;
EXP extern S_mxmlTree2MsrOah globalMxmlTree2MsrOahUserChoices;
EXP extern S_mxmlTree2MsrOah globalMxmlTree2MsrOahWithDetailedTrace;

//______________________________________________________________________________
void initializeMxmlTree2MsrOahHandling (
  S_oahHandler handler);


}


#endif
