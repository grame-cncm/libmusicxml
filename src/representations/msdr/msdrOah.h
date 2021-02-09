/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef ___msdrOah___
#define ___msdrOah___

#include "msdrBasicTypes.h"
#include "msdrKeywords.h"

#include "enableTracingIfDesired.h"
#ifdef TRACING_IS_ENABLED
  #include "traceOah.h"
#endif

#include "oahAtomsCollection.h"


namespace MusicXML2
{

//______________________________________________________________________________
class EXP msdrKeywordsLanguageAtom : public oahAtomWithValue
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<msdrKeywordsLanguageAtom> create (
                            string             shortName,
                            string             longName,
                            string             description,
                            string             valueSpecification,
                            string             variableName,
                            msdrKeywordsLanguageKind&
                                               msdrKeywordsLanguageKindVariable);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msdrKeywordsLanguageAtom (
                            string             shortName,
                            string             longName,
                            string             description,
                            string             valueSpecification,
                            string             variableName,
                            msdrKeywordsLanguageKind&
                                               msdrKeywordsLanguageKindVariable);

    virtual               ~msdrKeywordsLanguageAtom ();

  public:

    // set and get
    // ------------------------------------------------------

    void                  setMsdrKeywordsLanguageKindVariable (
                            msdrKeywordsLanguageKind value)
                              {
                                fMsdrKeywordsLanguageKindVariable = value;
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

    msdrKeywordsLanguageKind&
                          fMsdrKeywordsLanguageKindVariable;
};
typedef SMARTP<msdrKeywordsLanguageAtom> S_msdrKeywordsLanguageAtom;
EXP ostream& operator<< (ostream& os, const S_msdrKeywordsLanguageAtom& elt);

//______________________________________________________________________________
class EXP msdrPitchesLanguageAtom : public oahAtomWithValue
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<msdrPitchesLanguageAtom> create (
                            string             shortName,
                            string             longName,
                            string             description,
                            string             valueSpecification,
                            string             variableName,
                            msrQuarterTonesPitchesLanguageKind&
                                               msdrPitchesLanguageKindVariable);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msdrPitchesLanguageAtom (
                            string             shortName,
                            string             longName,
                            string             description,
                            string             valueSpecification,
                            string             variableName,
                            msrQuarterTonesPitchesLanguageKind&
                                               msdrPitchesLanguageKindVariable);

    virtual               ~msdrPitchesLanguageAtom ();

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
typedef SMARTP<msdrPitchesLanguageAtom> S_msdrPitchesLanguageAtom;
EXP ostream& operator<< (ostream& os, const S_msdrPitchesLanguageAtom& elt);

//______________________________________________________________________________
class EXP msdrOahGroup : public oahGroup
{
  public:

    static SMARTP<msdrOahGroup> create ();

  public:

    // initialisation
    // ------------------------------------------------------

    void                  initializeMsdrOahGroup ();

  public:

    // constructors/destructor
    // ------------------------------------------------------

                          msdrOahGroup ();

    virtual               ~msdrOahGroup ();

    // set and get
    // ------------------------------------------------------

    // trace
    // --------------------------------------

#ifdef TRACING_IS_ENABLED
    void                  setTraceMsdr ()
                              { fTraceMsdr = true; }
    bool                  getTraceMsdr () const
                              { return fTraceMsdr; }
#endif

    // display
    // --------------------------------------

    void                  setDisplayMsdr ()
                              { fDisplayMsdr = true; }

    bool                  getDisplayMsdr () const
                              { return fDisplayMsdr; }

    void                  setDisplayMsdrShort ()
                              { fDisplayMsdrShort = true; }

    bool                  getDisplayMsdrShort () const
                              { return fDisplayMsdrShort; }

    msrQuarterTonesPitchesLanguageKind
                          getMsdrQuarterTonesPitchesLanguageKind () const
                              { return fMsdrQuarterTonesPitchesLanguageKind; }

  public:

    // public services
    // ------------------------------------------------------

    // languages
    bool                  setMsdrQuarterTonesPitchesLanguage (string language);

    bool                  setMsdrKeywordsLanguage (string language);

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

    void                  printMsdrOahHelp ();

    void                  printMsdrOahValues (unsigned int fieldWidth);

  private:

    // private methods
    // ------------------------------------------------------

#ifdef TRACING_IS_ENABLED
    void                  initializeMsdrTraceOah ();
#endif

    void                  initializeMsdrDisplayOptions ();

    void                  initializeMsdrLanguagesOptions ();

    void                  initializeMsdrQuitAfterSomePassesOptions ();

  private:

    // private fields
    // --------------------------------------

    // trace
    // --------------------------------------

#ifdef TRACING_IS_ENABLED
    bool                  fTraceMsdr;

    bool                  fTraceMsdrVisitors;
#endif

    // display
    // --------------------------------------

    bool                  fDisplayMsdr;
    bool                  fDisplayMsdrShort;

    // languages
    // --------------------------------------

    msdrKeywordsLanguageKind
                          fMsdrKeywordsLanguageKind;

    msrQuarterTonesPitchesLanguageKind
                          fMsdrQuarterTonesPitchesLanguageKind;

    // quit after some passes
    // --------------------------------------

    bool                  fQuitAfterPass3;
};
typedef SMARTP<msdrOahGroup> S_msdrOahGroup;
EXP ostream& operator<< (ostream& os, const S_msdrOahGroup& elt);

EXP extern S_msdrOahGroup gGlobalMsdrOahGroup;

//______________________________________________________________________________
EXP S_msdrOahGroup createGlobalMsdrOahGroup ();


}


#endif
