/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef ___msr2lpsrOah___
#define ___msr2lpsrOah___

#include "msrSegnosAndCodas.h"

#include "oahBasicTypes.h"

#include "lpsrBasicTypes.h"


namespace MusicXML2
{

//______________________________________________________________________________
class msr2lpsrScoreOutputKindAtom : public oahAtomWithValue
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<msr2lpsrScoreOutputKindAtom> create (
      string               shortName,
      string               longName,
      string               description,
      string               valueSpecification,
      string               variableName,
      lpsrScoreOutputKind& lpsrScoreOutputKindVariable);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msr2lpsrScoreOutputKindAtom (
      string               shortName,
      string               longName,
      string               description,
      string               valueSpecification,
      string               variableName,
      lpsrScoreOutputKind& lpsrScoreOutputKindVariable);

    virtual ~msr2lpsrScoreOutputKindAtom ();

  public:

    // set and get
    // ------------------------------------------------------

    void                  setLpsrScoreOutputKindVariable (
                            lpsrScoreOutputKind value)
                              { fLpsrScoreOutputKindVariable = value; }

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

    lpsrScoreOutputKind&  fLpsrScoreOutputKindVariable;
};
typedef SMARTP<msr2lpsrScoreOutputKindAtom> S_msr2lpsrScoreOutputKindAtom;
EXP ostream& operator<< (ostream& os, const S_msr2lpsrScoreOutputKindAtom& elt);

//______________________________________________________________________________
class msr2lpsrOahGroup : public oahGroup
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<msr2lpsrOahGroup> create ();

  private:

    // initialisation
    // ------------------------------------------------------

    void                  initializeMsr2lpsrOahGroup ();

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msr2lpsrOahGroup ();

    virtual ~msr2lpsrOahGroup ();

  public:

    // quiet mode
    // ------------------------------------------------------

    void                  enforceGroupQuietness () override;

  public:

    // consistency check
    // ------------------------------------------------------

    virtual void          checkGroupOptionsConsistency () override;

  public:

    // set and get
    // ------------------------------------------------------

    // score output kind
    lpsrScoreOutputKind   getScoreOutputKind () const
                              { return fScoreOutputKind; }

    // repeats
    bool                  getCreateImplicitInitialRepeatBarline () const
                              { return fCreateImplicitInitialRepeatBarline; }

  public:

    // public services
    // ------------------------------------------------------

  private:

    // private services
    // ------------------------------------------------------

#ifdef TRACING_IS_ENABLED
    void                  initializMsr2lpsrTraceOptions ();
#endif

    void                  initializeScoreOutputOptions ();

    void                  initializeRepeatsOptions ();

    void                  initializeNotesOptions ();

    void                  initializeLyricsOptions ();

  public:

    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v) override;
    virtual void          acceptOut (basevisitor* v) override;

    virtual void          browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    void                  printMsr2lpsrOahHelp ();

    void                  printMsr2lpsrOahValues (int valueFieldWidth);

  private:

    // private fields
    // ------------------------------------------------------

    // score output kind
    lpsrScoreOutputKind   fScoreOutputKind;

    // repeats
    bool                  fCreateImplicitInitialRepeatBarline;
};
typedef SMARTP<msr2lpsrOahGroup> S_msr2lpsrOahGroup;
EXP ostream& operator<< (ostream& os, const S_msr2lpsrOahGroup& elt);

EXP extern S_msr2lpsrOahGroup gGlobalMsr2lpsrOahGroup;

//______________________________________________________________________________
EXP S_msr2lpsrOahGroup createGlobalMsr2lpsrOahGroup ();


}


#endif
