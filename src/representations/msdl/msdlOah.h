/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef ___msdlOah___
#define ___msdlOah___

#include "msdlBasicTypes.h"
#include "msdlKeywords.h"

#include "enableTracingIfDesired.h"
#ifdef TRACING_IS_ENABLED
  #include "traceOah.h"
#endif

#include "oahAtomsCollection.h"


namespace MusicXML2
{

//______________________________________________________________________________
class EXP msdlKeywordsLanguageAtom : public oahAtomWithValue
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<msdlKeywordsLanguageAtom> create (
                            string             shortName,
                            string             longName,
                            string             description,
                            string             valueSpecification,
                            string             variableName,
                            msdlKeywordsLanguageKind&
                                               msdlKeywordsLanguageKindVariable);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msdlKeywordsLanguageAtom (
                            string             shortName,
                            string             longName,
                            string             description,
                            string             valueSpecification,
                            string             variableName,
                            msdlKeywordsLanguageKind&
                                               msdlKeywordsLanguageKindVariable);

    virtual               ~msdlKeywordsLanguageAtom ();

  public:

    // set and get
    // ------------------------------------------------------

    void                  setMsdlKeywordsLanguageKindVariable (
                            msdlKeywordsLanguageKind value)
                              {
                                fMsdlKeywordsLanguageKindVariable = value;
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

    msdlKeywordsLanguageKind&
                          fMsdlKeywordsLanguageKindVariable;
};
typedef SMARTP<msdlKeywordsLanguageAtom> S_msdlKeywordsLanguageAtom;
EXP ostream& operator<< (ostream& os, const S_msdlKeywordsLanguageAtom& elt);

//______________________________________________________________________________
class EXP msdlPitchesLanguageAtom : public oahAtomWithValue
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<msdlPitchesLanguageAtom> create (
                            string             shortName,
                            string             longName,
                            string             description,
                            string             valueSpecification,
                            string             variableName,
                            msrQuarterTonesPitchesLanguageKind&
                                               msdlPitchesLanguageKindVariable);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msdlPitchesLanguageAtom (
                            string             shortName,
                            string             longName,
                            string             description,
                            string             valueSpecification,
                            string             variableName,
                            msrQuarterTonesPitchesLanguageKind&
                                               msdlPitchesLanguageKindVariable);

    virtual               ~msdlPitchesLanguageAtom ();

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
typedef SMARTP<msdlPitchesLanguageAtom> S_msdlPitchesLanguageAtom;
EXP ostream& operator<< (ostream& os, const S_msdlPitchesLanguageAtom& elt);

//______________________________________________________________________________
class EXP msdlOahGroup : public oahGroup
{
  public:

    static SMARTP<msdlOahGroup> create ();

  public:

    // initialisation
    // ------------------------------------------------------

    void                  initializeMsdlOahGroup ();

  public:

    // constructors/destructor
    // ------------------------------------------------------

                          msdlOahGroup ();

    virtual               ~msdlOahGroup ();

    // set and get
    // ------------------------------------------------------

    // trace
    // --------------------------------------

#ifdef TRACING_IS_ENABLED
    void                  setTraceMsdl ()
                              { fTraceMsdl = true; }
    bool                  getTraceMsdl () const
                              { return fTraceMsdl; }
#endif

    // display
    // --------------------------------------

    void                  setDisplayMsdl ()
                              { fDisplayMsdl = true; }

    bool                  getDisplayMsdl () const
                              { return fDisplayMsdl; }

    void                  setDisplayMsdlShort ()
                              { fDisplayMsdlShort = true; }

    bool                  getDisplayMsdlShort () const
                              { return fDisplayMsdlShort; }

    msrQuarterTonesPitchesLanguageKind
                          getMsdlQuarterTonesPitchesLanguageKind () const
                              { return fMsdlQuarterTonesPitchesLanguageKind; }

  public:

    // public services
    // ------------------------------------------------------

    // languages
    bool                  setMsdlQuarterTonesPitchesLanguage (string language);

    bool                  setMsdlKeywordsLanguage (string language);

    // quiet mode
    void                  enforceGroupQuietness () override;

    // consistency check
    void                  checkGroupOptionsConsistency () override;

    void                  enforceQuietness (); // JMI

  public:

    // visitors
    // ------------------------------------------------------

    void                  acceptIn  (basevisitor* v) override;
    void                  acceptOut (basevisitor* v) override;

    void                  browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    void                  printMsdlOahHelp ();

    void                  printMsdlOahValues (unsigned int fieldWidth);

  private:

    // private methods
    // ------------------------------------------------------

#ifdef TRACING_IS_ENABLED
    void                  initializeMsdlTraceOah ();
#endif

    void                  initializeMsdlDisplayOptions ();

    void                  initializeMsdlLanguagesOptions ();

    void                  initializeMsdlQuitAfterSomePassesOptions ();

  private:

    // private fields
    // --------------------------------------

    // trace
    // --------------------------------------

#ifdef TRACING_IS_ENABLED
    bool                  fTraceMsdl;

    bool                  fTraceMsdlVisitors;
#endif

    // display
    // --------------------------------------

    bool                  fDisplayMsdl;
    bool                  fDisplayMsdlShort;

    // languages
    // --------------------------------------

    msdlKeywordsLanguageKind
                          fMsdlKeywordsLanguageKind;

    msrQuarterTonesPitchesLanguageKind
                          fMsdlQuarterTonesPitchesLanguageKind;

    // quit after some passes
    // --------------------------------------

    bool                  fQuitAfterPass3;
};
typedef SMARTP<msdlOahGroup> S_msdlOahGroup;
EXP ostream& operator<< (ostream& os, const S_msdlOahGroup& elt);

EXP extern S_msdlOahGroup gGlobalMsdlOahGroup;

//______________________________________________________________________________
EXP S_msdlOahGroup createGlobalMsdlOahGroup ();


}


#endif
