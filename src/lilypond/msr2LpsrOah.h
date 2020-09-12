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

#include "lpsrBasicTypes.h"


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

    lpsrScoreOutputKind&  fLpsrScoreOutputKindVariable;
};
typedef SMARTP<msr2LpsrScoreOutputKindAtom> S_msr2LpsrScoreOutputKindAtom;
EXP ostream& operator<< (ostream& os, const S_msr2LpsrScoreOutputKindAtom& elt);

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

    virtual void          checkOptionsConsistency () override;

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
    void                  initializeTraceOah (
                            bool boolOptionsInitialValue);
#endif

    void                  initializeScoreOutputOptions (
                            bool boolOptionsInitialValue);

    void                  initializeRepeatsOptions (
                            bool boolOptionsInitialValue);

    void                  initializeNotesOptions (
                            bool boolOptionsInitialValue);

    void                  initializeLyricsOptions (
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

    void                  printMsr2LpsrOahHelp ();

    void                  printMsr2LpsrOahValues (int valueFieldWidth);

  public:

    // fields
    // ------------------------------------------------------

    // score output kind
    // --------------------------------------

    lpsrScoreOutputKind   fScoreOutputKind;

    // repeats
    // --------------------------------------

    bool                  fCreateImplicitInitialRepeatBarline;
};
typedef SMARTP<msr2LpsrOah> S_msr2LpsrOah;
EXP ostream& operator<< (ostream& os, const S_msr2LpsrOah& elt);

EXP extern S_msr2LpsrOah gGlobalMsr2LpsrOah;
EXP extern S_msr2LpsrOah gGlobalMsr2LpsrOahUserChoices;
EXP extern S_msr2LpsrOah gGlobalMsr2LpsrOahWithDetailedTrace;

//______________________________________________________________________________
void initializeMsr2LpsrOahHandler (
  S_oahHandler handler);


}


#endif
