/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef ___msrOah___
#define ___msrOah___

#include "msrBasicTypes.h"

#include "oahBasicTypes.h"


namespace MusicXML2
{

//______________________________________________________________________________
class msrPartRenameAtom : public oahValuedAtom
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<msrPartRenameAtom> create (
      string             shortName,
      string             longName,
      string             description,
      string             valueSpecification,
      string             variableName,
      map<string, string>&
                         msrPartRenameVariable);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrPartRenameAtom (
      string             shortName,
      string             longName,
      string             description,
      string             valueSpecification,
      string             variableName,
      map<string, string>&
                         msrPartRenameVariable);

    virtual ~msrPartRenameAtom ();

  public:

    // set and get
    // ------------------------------------------------------

    const map<string, string>&
                          getStringStringMapVariable ()
                              { return fStringStringMapVariable; }

    // services
    // ------------------------------------------------------

    void                  handleValue (
                            string   theString,
                            ostream& os);

    // print
    // ------------------------------------------------------

    void                  print (ostream& os) const;

    void                  printAtomOptionsValues (
                            indentedOstream& os,
                            int      valueFieldWidth) const;

  private:

    // fields
    // ------------------------------------------------------

    map<string, string>&  fStringStringMapVariable;
};
typedef SMARTP<msrPartRenameAtom> S_msrPartRenameAtom;
EXP ostream& operator<< (ostream& os, const S_msrPartRenameAtom& elt);

//______________________________________________________________________________
class msrPartTransposeAtom : public oahValuedAtom
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<msrPartTransposeAtom> create (
      string             shortName,
      string             longName,
      string             description,
      string             valueSpecification,
      string             variableName,
      map<string, S_msrSemiTonesPitchAndOctave>&
                         msrPartTransposeVariable);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrPartTransposeAtom (
      string             shortName,
      string             longName,
      string             description,
      string             valueSpecification,
      string             variableName,
      map<string, S_msrSemiTonesPitchAndOctave>&
                         msrPartTransposeVariable);

    virtual ~msrPartTransposeAtom ();

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

    // services
    // ------------------------------------------------------

    void                  handleValue (
                            string   theString,
                            ostream& os);

    // print
    // ------------------------------------------------------

    void                  print (ostream& os) const;

    void                  printAtomOptionsValues (
                            indentedOstream& os,
                            int      valueFieldWidth) const;

  private:

    // fields
    // ------------------------------------------------------

    map<string, S_msrSemiTonesPitchAndOctave>&
                          fStringMsrSemiTonesPitchAndOctaveVariable;
};
typedef SMARTP<msrPartTransposeAtom> S_msrPartTransposeAtom;
EXP ostream& operator<< (ostream& os, const S_msrPartTransposeAtom& elt);

//______________________________________________________________________________
class msrPitchesLanguageAtom : public oahValuedAtom
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<msrPitchesLanguageAtom> create (
      string             shortName,
      string             longName,
      string             description,
      string             valueSpecification,
      string             variableName,
      msrQuarterTonesPitchesLanguageKind&
                         msrMsrPitchesLanguageKindVariable);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrPitchesLanguageAtom (
      string             shortName,
      string             longName,
      string             description,
      string             valueSpecification,
      string             variableName,
      msrQuarterTonesPitchesLanguageKind&
                         msrMsrPitchesLanguageKindVariable);

    virtual ~msrPitchesLanguageAtom ();

  public:

    // set and get
    // ------------------------------------------------------

    void                  setMsrQuarterTonesPitchesLanguageKindVariable (
                            msrQuarterTonesPitchesLanguageKind value)
                              {
                                fMsrQuarterTonesPitchesLanguageKindVariable = value;
                              }

    // services
    // ------------------------------------------------------

    void                  handleValue (
                            string   theString,
                            ostream& os);

    // print
    // ------------------------------------------------------

    void                  print (ostream& os) const;

    void                  printAtomOptionsValues (
                            indentedOstream& os,
                            int      valueFieldWidth) const;

  private:

    // fields
    // ------------------------------------------------------

    msrQuarterTonesPitchesLanguageKind&
                          fMsrQuarterTonesPitchesLanguageKindVariable;
};
typedef SMARTP<msrPitchesLanguageAtom> S_msrPitchesLanguageAtom;
EXP ostream& operator<< (ostream& os, const S_msrPitchesLanguageAtom& elt);

//______________________________________________________________________________
class msrOah : public oahGroup
{
  public:

    static SMARTP<msrOah> create (
      S_oahHandler handler);

    SMARTP<msrOah>        createCloneWithDetailedTrace ();

  public:

    // initialisation
    // ------------------------------------------------------

    void                  initializeMsrOah (
                            bool boolOptionsInitialValue);

  public:

    // constructors/destructor
    // ------------------------------------------------------

    msrOah (
      S_oahHandler handler);

    virtual ~msrOah ();

    // set and get
    // ------------------------------------------------------

    bool                  setMsrQuarterTonesPitchesLanguage (
                            string language);

  public:

    // quiet mode
    // ------------------------------------------------------

    void                  enforceQuietness ();

  public:

    // consistency check
    // ------------------------------------------------------

    virtual void          checkOptionsConsistency ();

  public:

    // public services
    // ------------------------------------------------------

    virtual S_oahValuedAtom
                          handleAtom (
                            indentedOstream&  os,
                            S_oahAtom atom);

    virtual void          handleValuedAtomValue (
                            indentedOstream& os,
                            S_oahAtom        atom,
                            string           theString);

  private:

    // private services
    // ------------------------------------------------------

#ifdef TRACE_OAH
    void                  initializeMsrTraceOah (
                            bool boolOptionsInitialValue);
#endif

    void                  initializeMsrDisplayOptions (
                            bool boolOptionsInitialValue);

    void                  initializeMsrLanguagesOptions (
                            bool boolOptionsInitialValue);

    void                  initializeMsrPartsOptions (
                            bool boolOptionsInitialValue);

    void                  initializeMsrStavesOptions (
                            bool boolOptionsInitialValue);

    void                  initializeMsrVoicesOptions (
                            bool boolOptionsInitialValue);

    void                  initializeMsrRepeatsOptions (
                            bool boolOptionsInitialValue);

    void                  initializeMsrNotesOptions (
                            bool boolOptionsInitialValue);

    void                  initializeMsrLyricsOptions (
                            bool boolOptionsInitialValue);

    void                  initializeMsrHarmoniesOptions (
                            bool boolOptionsInitialValue);

    void                  initializeMsrFiguredBassOptions (
                            bool boolOptionsInitialValue);

    void                  initializeMsrExitAfterSomePassesOptions (
                            bool boolOptionsInitialValue);

  public:

    // print
    // ------------------------------------------------------

    void                  printMsrOahHelp ();

    void                  printMsrOahValues (int fieldWidth);

  public:

    // trace
    // --------------------------------------

    bool                  fTraceMsr;

    bool                  fTraceMsrVisitors;

    // display
    // --------------------------------------

    bool                  fDisplayPartGroups;

    bool                  fDisplayMsr;
    bool                  fDisplayMsrDetails;

    bool                  fDisplayMsrNames;
    bool                  fDisplayMsrSummary;


    // languages
    // --------------------------------------

    msrQuarterTonesPitchesLanguageKind
                          fMsrQuarterTonesPitchesLanguageKind;

    // parts
    // --------------------------------------

    map<string, string>   fPartsRenamingMap;
    map<string, S_msrSemiTonesPitchAndOctave>
                          fPartsTranspositionMap;


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


    // lyrics
    // --------------------------------------

    bool                  fAddStanzasNumbers; // LPSR ??? JMI


    // harmonies
    // --------------------------------------

    bool                  fShowHarmonyVoices;


    // figured bass
    // --------------------------------------

    bool                  fShowFiguredBassVoices;


    // exit after some passes
    // --------------------------------------

    bool                  fExit2a;
    bool                  fExit2b;
};
typedef SMARTP<msrOah> S_msrOah;
EXP ostream& operator<< (ostream& os, const S_msrOah& elt);

EXP extern S_msrOah gMsrOah;
EXP extern S_msrOah gMsrOahUserChoices;
EXP extern S_msrOah gMsrOahWithDetailedTrace;

//______________________________________________________________________________
void initializeMsrOahHandling (
  S_oahHandler handler);


}


#endif
