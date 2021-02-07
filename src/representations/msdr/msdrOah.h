/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef ___msprOah___
#define ___msprOah___

#include "msprBasicTypes.h"

#include "enableTracingIfDesired.h"
#ifdef TRACING_IS_ENABLED
  #include "traceOah.h"
#endif

#include "oahAtomsCollection.h"


namespace MusicXML2
{

//______________________________________________________________________________
class EXP msprPitchesLanguageAtom : public oahAtomWithValue
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<msprPitchesLanguageAtom> create (
                            string             shortName,
                            string             longName,
                            string             description,
                            string             valueSpecification,
                            string             variableName,
                            msrQuarterTonesPitchesLanguageKind&
                                               msprPitchesLanguageKindVariable);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msprPitchesLanguageAtom (
                            string             shortName,
                            string             longName,
                            string             description,
                            string             valueSpecification,
                            string             variableName,
                            msrQuarterTonesPitchesLanguageKind&
                                               msprPitchesLanguageKindVariable);

    virtual               ~msprPitchesLanguageAtom ();

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
                            const string& theString,
                            ostream&      os) override;

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

    msrQuarterTonesPitchesLanguageKind&
                          fMsrQuarterTonesPitchesLanguageKindVariable;
};
typedef SMARTP<msprPitchesLanguageAtom> S_msprPitchesLanguageAtom;
EXP ostream& operator<< (ostream& os, const S_msprPitchesLanguageAtom& elt);

//______________________________________________________________________________
class EXP msprChordsLanguageAtom : public oahAtomWithValue
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<msprChordsLanguageAtom> create (
                            string             shortName,
                            string             longName,
                            string             description,
                            string             valueSpecification,
                            string             variableName,
                            msprChordsLanguageKind&
                                               msprChordsLanguageKindVariable);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msprChordsLanguageAtom (
                            string             shortName,
                            string             longName,
                            string             description,
                            string             valueSpecification,
                            string             variableName,
                            msprChordsLanguageKind&
                                               msprChordsLanguageKindVariable);

    virtual               ~msprChordsLanguageAtom ();

  public:

    // set and get
    // ------------------------------------------------------

    void                  setmsprChordsLanguageKindVariable (
                            msprChordsLanguageKind value)
                              {
                                fmsprChordsLanguageKindVariable = value;
                              }

  public:

    // public services
    // ------------------------------------------------------

    void                  applyAtomWithValue (
                            const string& theString,
                            ostream&      os) override;

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

    msprChordsLanguageKind&
                          fmsprChordsLanguageKindVariable;
};
typedef SMARTP<msprChordsLanguageAtom> S_msprChordsLanguageAtom;
EXP ostream& operator<< (ostream& os, const S_msprChordsLanguageAtom& elt);

//______________________________________________________________________________
class EXP msprOahGroup : public oahGroup
{
  public:

    static SMARTP<msprOahGroup> create ();

  public:

    // initialisation
    // ------------------------------------------------------

    void                  initializeMsprOahGroup ();

  public:

    // constructors/destructor
    // ------------------------------------------------------

                          msprOahGroup ();

    virtual               ~msprOahGroup ();

    // set and get
    // ------------------------------------------------------

    // trace
    // --------------------------------------

#ifdef TRACING_IS_ENABLED
    void                  setTraceMspr ()
                              { fTraceMspr = true; }
    bool                  getTraceMspr () const
                              { return fTraceMspr; }
#endif

    // display
    // --------------------------------------

    void                  setDisplayMspr ()
                              { fDisplayMspr = true; }
    bool                  getDisplayMspr () const
                              { return fDisplayMspr; }

    void                  setDisplayMsprShort ()
                              { fDisplayMsprShort = true; }
    bool                  getDisplayMsprShort () const
                              { return fDisplayMsprShort; }

  public:

    // quiet mode
    // ------------------------------------------------------

    void                  enforceQuietness ();

  public:

    // public services
    // ------------------------------------------------------

    // quiet mode
    void                  enforceGroupQuietness () override;

    // consistency check
    void                  checkGroupOptionsConsistency () override;

  public:

    // print
    // ------------------------------------------------------

    void                  printMsprOahHelp ();

    void                  printMsprOahValues (unsigned int fieldWidth);

  private:

    // private methods
    // ------------------------------------------------------

#ifdef TRACING_IS_ENABLED
    void                  initializeMsprTraceOah ();
#endif

    void                  initializeMsprDisplayOptions ();

    void                  initializeMsprQuitAfterSomePassesOptions ();

  private:

    // private fields
    // --------------------------------------

    // trace
    // --------------------------------------

#ifdef TRACING_IS_ENABLED
    bool                  fTraceMspr;

    bool                  fTraceMsprVisitors;
#endif

    // display
    // --------------------------------------

    bool                  fDisplayMspr;
    bool                  fDisplayMsprShort;

    // languages
    // --------------------------------------

    msrQuarterTonesPitchesLanguageKind
                          fMsprQuarterTonesPitchesLanguageKind;

    msprChordsLanguageKind
                          fMsprChordsLanguageKind;

    // quit after some passes
    // --------------------------------------

    bool                  fQuitAfterPass3;
};
typedef SMARTP<msprOahGroup> S_msprOahGroup;
EXP ostream& operator<< (ostream& os, const S_msprOahGroup& elt);

EXP extern S_msprOahGroup gGlobalMsprOahGroup;

//______________________________________________________________________________
EXP S_msprOahGroup createGlobalMsprOahGroup ();


}


#endif
