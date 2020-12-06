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

#include "oahAtomsCollection.h"


namespace MusicXML2
{

//______________________________________________________________________________
class msrPitchesLanguageAtom : public oahAtomWithValue
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

  public:

    // public services
    // ------------------------------------------------------

    void                  applyAtomWithValue (
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

    void                  printAtomWithValueOptionsValues (
                            ostream& os,
                            int      valueFieldWidth) const override;

  private:

    // private fields
    // ------------------------------------------------------

    msrQuarterTonesPitchesLanguageKind&
                          fMsrQuarterTonesPitchesLanguageKindVariable;
};
typedef SMARTP<msrPitchesLanguageAtom> S_msrPitchesLanguageAtom;
EXP ostream& operator<< (ostream& os, const S_msrPitchesLanguageAtom& elt);

//______________________________________________________________________________
class msrRenamePartAtom : public oahAtomWithValue
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
      map<string, string>& stringToStringMapVariable);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrRenamePartAtom (
      string               shortName,
      string               longName,
      string               description,
      string               valueSpecification,
      string               variableName,
      map<string, string>& stringToStringMapVariable);

    virtual ~msrRenamePartAtom ();

  public:

    // set and get
    // ------------------------------------------------------

    const map<string, string>&
                          getStringToStringMapVariable () const
                              { return fStringToStringMapVariable; }

  public:

    // public services
    // ------------------------------------------------------

    void                  applyAtomWithValue (
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

    void                  printAtomWithValueOptionsValues (
                            ostream& os,
                            int      valueFieldWidth) const override;

  private:

    // private fields
    // ------------------------------------------------------

    map<string, string>&  fStringToStringMapVariable;
};
typedef SMARTP<msrRenamePartAtom> S_msrRenamePartAtom;
EXP ostream& operator<< (ostream& os, const S_msrRenamePartAtom& elt);

//______________________________________________________________________________
class msrOahGroup : public oahGroup
{
  public:

    static SMARTP<msrOahGroup> create ();

  public:

    // initialisation
    // ------------------------------------------------------

    void                  initializeMsrOahGroup ();

  public:

    // constructors/destructor
    // ------------------------------------------------------

    msrOahGroup ();

    virtual ~msrOahGroup ();

    // set and get
    // ------------------------------------------------------

    // trace
    bool                  getTraceMsr () const
                              { return fTraceMsr; }

    bool                  getTraceMsrVisitors () const
                              { return fTraceMsrVisitors; }


    // display
    bool                  getDisplayPartGroups () const
                              { return fDisplayPartGroups; }


    bool                  getDisplayMsr () const
                              { return fDisplayMsr; }

    bool                  getDisplayMsrShort () const
                              { return fDisplayMsrShort; }

    bool                  getDisplayMsrDetails () const
                              { return fDisplayMsrDetails; }


    bool                  getDisplayMsrNames () const
                              { return fDisplayMsrNames; }

    bool                  getDisplayMsrSummary () const
                              { return fDisplayMsrSummary; }



    // languages
    msrQuarterTonesPitchesLanguageKind
                          getMsrQuarterTonesPitchesLanguageKind () const
                              { return fMsrQuarterTonesPitchesLanguageKind; }

    // parts
    const map<string, string>&
                          getPartsRenamingMap () const
                              { return fPartsRenamingMap; }

    // staves
    bool                  getCreateSingleLineStavesAsRythmic () const
                              { return fCreateSingleLineStavesAsRythmic; }


    // voices
    bool                  getCreateVoicesStaffRelativeNumbers () const
                              { return fCreateVoicesStaffRelativeNumbers; }


    // harmonies
    bool                  getShowHarmonyVoices () const
                              { return fShowHarmonyVoices; }


    // figured bass
    bool                  getShowFiguredBassVoices () const
                              { return fShowFiguredBassVoices; }

    bool                  setMsrQuarterTonesPitchesLanguage (
                            string language);

  public:

    // quiet mode
    // ------------------------------------------------------

    void                  enforceGroupQuietness () override;

  public:

    // consistency check
    // ------------------------------------------------------

    virtual void          checkGroupOptionsConsistency () override;

  public:

    // public services
    // ------------------------------------------------------

  private:

    // private services
    // ------------------------------------------------------

#ifdef TRACING_IS_ENABLED
    void                  initializeMsrTraceOah ();
#endif

    void                  initializeMsrDisplayOptions ();

    void                  initializeMsrLanguagesOptions ();

    void                  initializeMsrPartsOptions ();

    void                  initializeMsrStavesOptions ();

    void                  initializeMsrVoicesOptions ();

    void                  initializeMsrHarmoniesOptions ();

    void                  initializeMsrFiguredBassOptions ();

  public:

    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v) override;
    virtual void          acceptOut (basevisitor* v) override;

    virtual void          browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    void                  printMsrOahHelp ();

    void                  printMsrOahValues (int valueFieldWidth);

  private:

    // private fields
    // ------------------------------------------------------

    // trace
    bool                  fTraceMsr;

    bool                  fTraceMsrVisitors;

    // display
    bool                  fDisplayPartGroups;

    bool                  fDisplayMsr;
    bool                  fDisplayMsrShort;
    bool                  fDisplayMsrDetails;

    bool                  fDisplayMsrNames;
    bool                  fDisplayMsrSummary;


    // languages
    msrQuarterTonesPitchesLanguageKind
                          fMsrQuarterTonesPitchesLanguageKind;

    // parts
    map<string, string>   fPartsRenamingMap;

    // staves
    bool                  fCreateSingleLineStavesAsRythmic;

    // voices
    bool                  fCreateVoicesStaffRelativeNumbers;

    // harmonies
    bool                  fShowHarmonyVoices;

    // figured bass
    bool                  fShowFiguredBassVoices;
};
typedef SMARTP<msrOahGroup> S_msrOahGroup;
EXP ostream& operator<< (ostream& os, const S_msrOahGroup& elt);

EXP extern S_msrOahGroup gGlobalMsrOahGroup;

//______________________________________________________________________________
S_msrOahGroup createGlobalMsrOahGroup ();


}


#endif
