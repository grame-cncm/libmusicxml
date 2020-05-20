/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef ___msr2LpsrOah___
#define ___msr2LpsrOah___

#include "msrSegnosAndCodas.h"

#include "oahBasicTypes.h"


//#include "msrBasicTypes.h"


namespace MusicXML2
{

//______________________________________________________________________________
class msr2LpsrScoreOutputKindAtom : public oahValuedAtom
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<msr2LpsrScoreOutputKindAtom> create (
      string               shortName,
      string               longName,
      string               description,
      string               valueSpecification,
      string               variableName,
      lpsrScoreOutputKind& lpsrScoreOutputKindVariable);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msr2LpsrScoreOutputKindAtom (
      string               shortName,
      string               longName,
      string               description,
      string               valueSpecification,
      string               variableName,
      lpsrScoreOutputKind& lpsrScoreOutputKindVariable);

    virtual ~msr2LpsrScoreOutputKindAtom ();

  public:

    // set and get
    // ------------------------------------------------------

    void                  setLpsrScoreOutputKindVariable (
                            lpsrScoreOutputKind value)
                              { fLpsrScoreOutputKindVariable = value; }

  public:

    // services
    // ------------------------------------------------------

    S_oahValuedAtom       handleOptionUnderName (
                            string   optionName,
                            ostream& os);

    void                  handleValue (
                            string   theString,
                            ostream& os);

  public:

    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v);
    virtual void          acceptOut (basevisitor* v);

    virtual void          browseData (basevisitor* v);

  public:

    // print
    // ------------------------------------------------------

    string                asShortNamedOptionString () const;
    string                asActualLongNamedOptionString () const;

    void                  print (ostream& os) const;

    void                  printAtomOptionsValues (
                            ostream& os,
                            int      valueFieldWidth) const;

  private:

    // fields
    // ------------------------------------------------------

    lpsrScoreOutputKind&  fLpsrScoreOutputKindVariable;
};
typedef SMARTP<msr2LpsrScoreOutputKindAtom> S_msr2LpsrScoreOutputKindAtom;
EXP ostream& operator<< (ostream& os, const S_msr2LpsrScoreOutputKindAtom& elt);

//______________________________________________________________________________
class msrRenamePartAtom : public oahValuedAtom
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<msrRenamePartAtom> create (
      string               shortName,
      string               longName,
      string               description,
      string               valueSpecification,
      string               variableName,
      map<string, string>& stringStringMapVariable);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrRenamePartAtom (
      string               shortName,
      string               longName,
      string               description,
      string               valueSpecification,
      string               variableName,
      map<string, string>& stringStringMapVariable);

    virtual ~msrRenamePartAtom ();

  public:

    // set and get
    // ------------------------------------------------------

    const map<string, string>&
                          getStringStringMapVariable ()
                              { return fStringStringMapVariable; }

  public:

    // services
    // ------------------------------------------------------

    S_oahValuedAtom       handleOptionUnderName (
                            string   optionName,
                            ostream& os);

    void                  handleValue (
                            string   theString,
                            ostream& os);

  public:

    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v);
    virtual void          acceptOut (basevisitor* v);

    virtual void          browseData (basevisitor* v);

  public:

    // print
    // ------------------------------------------------------

    string                asShortNamedOptionString () const;
    string                asActualLongNamedOptionString () const;

    void                  print (ostream& os) const;

    void                  printAtomOptionsValues (
                            ostream& os,
                            int      valueFieldWidth) const;

  private:

    // fields
    // ------------------------------------------------------

    map<string, string>&  fStringStringMapVariable;
};
typedef SMARTP<msrRenamePartAtom> S_msrRenamePartAtom;
EXP ostream& operator<< (ostream& os, const S_msrRenamePartAtom& elt);

//______________________________________________________________________________
class msrTransposePartAtom : public oahValuedAtom
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<msrTransposePartAtom> create (
      string             shortName,
      string             longName,
      string             description,
      string             valueSpecification,
      string             variableName,
      map<string, S_msrSemiTonesPitchAndOctave>&
                         stringMsrSemiTonesPitchAndOctaveVariable);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrTransposePartAtom (
      string             shortName,
      string             longName,
      string             description,
      string             valueSpecification,
      string             variableName,
      map<string, S_msrSemiTonesPitchAndOctave>&
                         stringMsrSemiTonesPitchAndOctaveVariable);

    virtual ~msrTransposePartAtom ();

  public:

    // set and get
    // ------------------------------------------------------

    void                  setStringMsrSemiTonesPitchAndOctaveVariable (
                            string  partName,
                            S_msrSemiTonesPitchAndOctave
                                    semiTonesPitchAndOctave)
                              {
                                fStringMsrSemiTonesPitchAndOctaveVariable [
                                  partName
                                ] =
                                  semiTonesPitchAndOctave;
                              }

    const map<string, S_msrSemiTonesPitchAndOctave>&
                          getStringMsrSemiTonesPitchAndOctaveVariable ()
                              {
                                return
                                  fStringMsrSemiTonesPitchAndOctaveVariable;
                              }

  public:

    // services
    // ------------------------------------------------------

    S_oahValuedAtom       handleOptionUnderName (
                            string   optionName,
                            ostream& os);

    void                  handleValue (
                            string   theString,
                            ostream& os);

  public:

    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v);
    virtual void          acceptOut (basevisitor* v);

    virtual void          browseData (basevisitor* v);

  public:

    // print
    // ------------------------------------------------------

    string                asShortNamedOptionString () const;
    string                asActualLongNamedOptionString () const;

    void                  print (ostream& os) const;

    void                  printAtomOptionsValues (
                            ostream& os,
                            int      valueFieldWidth) const;

  private:

    // fields
    // ------------------------------------------------------

    map<string, S_msrSemiTonesPitchAndOctave>&
                          fStringMsrSemiTonesPitchAndOctaveVariable;
};
typedef SMARTP<msrTransposePartAtom> S_msrTransposePartAtom;
EXP ostream& operator<< (ostream& os, const S_msrTransposePartAtom& elt);

//______________________________________________________________________________
class msrOmitPartAtom : public oahValuedAtom
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<msrOmitPartAtom> create (
      string       shortName,
      string       longName,
      string       description,
      string       valueSpecification,
      string       variableName,
      set<string>& stringSetVariable);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrOmitPartAtom (
      string       shortName,
      string       longName,
      string       description,
      string       valueSpecification,
      string       variableName,
      set<string>& stringSetVariable);

    virtual ~msrOmitPartAtom ();

  public:

    // set and get
    // ------------------------------------------------------

    void                  setStringSetVariable (
                            string  partName)
                              { fStringSetVariable.insert (partName); }

    const set<string>&    getStringSetVariable ()
                              { return fStringSetVariable; }

  public:

    // services
    // ------------------------------------------------------

    S_oahValuedAtom       handleOptionUnderName (
                            string   optionName,
                            ostream& os);

    void                  handleValue (
                            string   theString,
                            ostream& os);

  public:

    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v);
    virtual void          acceptOut (basevisitor* v);

    virtual void          browseData (basevisitor* v);

  public:

    // print
    // ------------------------------------------------------

    string                asShortNamedOptionString () const;
    string                asActualLongNamedOptionString () const;

    void                  print (ostream& os) const;

    void                  printAtomOptionsValues (
                            ostream& os,
                            int      valueFieldWidth) const;

  private:

    // fields
    // ------------------------------------------------------

    set<string>&          fStringSetVariable;
};
typedef SMARTP<msrOmitPartAtom> S_msrOmitPartAtom;
EXP ostream& operator<< (ostream& os, const S_msrOmitPartAtom& elt);

//______________________________________________________________________________
class msrKeepPartAtom : public oahValuedAtom
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<msrKeepPartAtom> create (
      string       shortName,
      string       longName,
      string       description,
      string       valueSpecification,
      string       variableName,
      set<string>& stringSetVariable);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrKeepPartAtom (
      string       shortName,
      string       longName,
      string       description,
      string       valueSpecification,
      string       variableName,
      set<string>& stringSetVariable);

    virtual ~msrKeepPartAtom ();

  public:

    // set and get
    // ------------------------------------------------------

    void                  setStringSetVariable (
                            string  partName)
                              { fStringSetVariable.insert (partName); }

    const set<string>&    getStringSetVariable ()
                              { return fStringSetVariable; }

  public:

    // services
    // ------------------------------------------------------

    S_oahValuedAtom       handleOptionUnderName (
                            string   optionName,
                            ostream& os);

    void                  handleValue (
                            string   theString,
                            ostream& os);

  public:

    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v);
    virtual void          acceptOut (basevisitor* v);

    virtual void          browseData (basevisitor* v);

  public:

    // print
    // ------------------------------------------------------

    string                asShortNamedOptionString () const;
    string                asActualLongNamedOptionString () const;

    void                  print (ostream& os) const;

    void                  printAtomOptionsValues (
                            ostream& os,
                            int      valueFieldWidth) const;

  private:

    // fields
    // ------------------------------------------------------

    set<string>&          fStringSetVariable;
};
typedef SMARTP<msrKeepPartAtom> S_msrKeepPartAtom;
EXP ostream& operator<< (ostream& os, const S_msrKeepPartAtom& elt);

//______________________________________________________________________________
class msr2LpsrOah : public oahGroup
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<msr2LpsrOah> create (
      S_oahHandler handlerUpLink);

    SMARTP<msr2LpsrOah>   createCloneWithDetailedTrace ();

  private:

    // initialisation
    // ------------------------------------------------------

    void                  initializeMsr2LpsrOah (
                            bool boolOptionsInitialValue);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msr2LpsrOah (
      S_oahHandler handlerUpLink);

    virtual ~msr2LpsrOah ();

  public:

    // quiet mode
    // ------------------------------------------------------

    void                  enforceQuietness ();

  public:

    // consistency check
    // ------------------------------------------------------

    virtual void          checkOptionsConsistency ();

  public:

    // set and get
    // ------------------------------------------------------

    void                  setAllMsr2LpsrTraceOah (
                            bool boolOptionsInitialValue);

  public:

    // public services
    // ------------------------------------------------------

  private:

    // private services
    // ------------------------------------------------------

#ifdef TRACE_OAH
    void                  initializeMsr2LpsrTraceOah (
                            bool boolOptionsInitialValue);
#endif

    void                  initializeMsr2LpsrScoreOutputOptions (
                            bool boolOptionsInitialValue);

    void                  initializeMsr2LpsrPartsOptions (
                            bool boolOptionsInitialValue);

    void                  initializeMsr2LpsrStavesOptions (
                            bool boolOptionsInitialValue);

    void                  initializeMsr2LpsrVoicesOptions (
                            bool boolOptionsInitialValue);

    void                  initializeMsr2LpsrRepeatsOptions (
                            bool boolOptionsInitialValue);

    void                  initializeMsr2LpsrNotesOptions (
                            bool boolOptionsInitialValue);

    void                  initializeMsr2LpsrLyricsOptions (
                            bool boolOptionsInitialValue);

    void                  initializeMsr2LpsrHarmoniesOptions (
                            bool boolOptionsInitialValue);

    void                  initializeMsr2LpsrFiguredBassOptions (
                            bool boolOptionsInitialValue);

    void                  initializeMsr2LpsrExitAfterSomePassesOptions (
                            bool boolOptionsInitialValue);

  public:

    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v);
    virtual void          acceptOut (basevisitor* v);

    virtual void          browseData (basevisitor* v);

  public:

    // print
    // ------------------------------------------------------

    void                  printMsr2LpsrOahHelp ();

    void                  printMsr2LpsrOahValues (int fieldWidth);

  public:

    // fields
    // ------------------------------------------------------

    // score output kind
    // --------------------------------------

    lpsrScoreOutputKind   fScoreOutputKind;

    // parts
    // --------------------------------------

    map<string, string>   fPartsRenamingMap;

    map<string, S_msrSemiTonesPitchAndOctave>
                          fPartsTranspositionMap;

    set<string>           fPartsOmitIDSet;
    set<string>           fPartsKeepIDSet;

    set<string>           fPartsOmitNameSet;
    set<string>           fPartsKeepNameSet;

    // for checkOptionsConsistency()
    S_oahStringSetAtom    fOmitPartIDAtom;
    S_oahStringSetAtom    fKeepPartIDAtom;

    S_oahStringSetAtom    fOmitPartNameAtom;
    S_oahStringSetAtom    fKeepPartNameAtom;


    // staves
    // --------------------------------------

    bool                  fCreateSingleLineStavesAsRythmic;


    // voices
    // --------------------------------------

    bool                  fCreateVoicesStaffRelativeNumbers;


    // repeats
    // --------------------------------------

    bool                  fCreateImplicitInitialRepeatBarline;


    // notes
    // --------------------------------------

    bool                  fDelayRestsDynamics;
    bool                  fDelayRestsWords; // JMI
    bool                  fDelayRestsBeams; // JMI
    bool                  fDelayRestsSlurs; // JMI
    bool                  fDelayRestsLigatures; // JMI
    bool                  fDelayRestsPedals; // JMI
    bool                  fDelayRestsSlashes; // JMI
    bool                  fDelayRestsWedges; // JMI

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

    bool                  fAddStanzasNumbers; // LPSR ??? JMI

    bool                  fOmitLyrics;


    // harmonies
    // --------------------------------------

    bool                  fShowHarmonyVoices;

    bool                  fOmitHarmonies;


    // figured bass
    // --------------------------------------

    bool                  fShowFiguredBassVoices;

    bool                  fOmitFiguredBasses;


    // exit after some passes
    // --------------------------------------

    bool                  fExit2a;
    bool                  fExit2b;
};
typedef SMARTP<msr2LpsrOah> S_msr2LpsrOah;
EXP ostream& operator<< (ostream& os, const S_msr2LpsrOah& elt);

EXP extern S_msr2LpsrOah gMsr2LpsrOah;
EXP extern S_msr2LpsrOah gMsr2LpsrOahUserChoices;
EXP extern S_msr2LpsrOah gMsr2LpsrOahWithDetailedTrace;

//______________________________________________________________________________
void initializeMsr2LpsrOahHandling (
  S_oahHandler handler);


}


#endif
