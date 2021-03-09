/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef ___msdl2msrOah___
#define ___msdl2msrOah___

#include "msrSegnosAndCodas.h"

#include "oahBasicTypes.h"

#include "msdlKeywords.h"

#include "generatorsBasicTypes.h"

#include "msdlOah.h"


namespace MusicXML2
{

//______________________________________________________________________________
class EXP msdl2msrOahGroup : public oahGroup
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<msdl2msrOahGroup> create ();

  private:

    // initialisation
    // ------------------------------------------------------

    void                  initializeMsdl2msrGroup ();

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msdl2msrOahGroup ();

    virtual               ~msdl2msrOahGroup ();

  public:

    // set and get
    // ------------------------------------------------------

#ifdef TRACING_IS_ENABLED
    bool                  getTraceTokens () const
                              { return fTraceTokens; }
    bool                  getTraceTokensDetails () const
                              { return fTraceTokensDetails; }

    bool                  getTraceSyntax () const
                              { return fTraceSyntax; }
    bool                  getTraceSyntaxDetails () const
                              { return fTraceSyntaxDetails; }

    bool                  getTraceSyntaxErrorRecovery () const
                              { return fTraceSyntaxErrorRecovery; }

    bool                  getTraceSyntaxErrorRecoveryDetails () const
                              { return fTraceSyntaxErrorRecoveryDetails; }
#endif

    msdlUserLanguageKind  getMsdlUserLanguageKind () const
                              { return fMsdlUserLanguageKind; }

    msdlKeywordsLanguageKind
                          getMsdlKeywordsInputLanguageKind () const
                              { return fMsdlKeywordsInputLanguageKind; }

    msrQuarterTonesPitchesLanguageKind
                          getMsdlQuarterTonesPitchesLanguageKind () const
                              { return fMsdlQuarterTonesPitchesLanguageKind; }

    bool                  getLexicalAnalysisOnly () const
                              { return fLexicalAnalysisOnly; }

    void                  setIgnoreSeparatorTokens (bool value)
                              { fIgnoreSeparatorTokens = value; }

    bool                  getIgnoreSeparatorTokens () const
                              { return fIgnoreSeparatorTokens; }

    msdlKeywordsLanguageKind
                          getMsdlKeywordsTranslationLanguageKind () const
                              { return fMsdlKeywordsTranslationLanguageKind; }

    msdlCommentsTypeKind  getMsdlCommentsTypeTranslationKind () const
                              { return fMsdlCommentsTypeTranslationKind; }

    generatorOutputKind   getGeneratorOutputKind () const
                              { return fGeneratorOutputKind; }

    bool                  getQuitOnLexicalErrors () const
                              { return fQuitOnLexicalErrors; }

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

  private:

    // private services
    // ------------------------------------------------------

#ifdef TRACING_IS_ENABLED
    void                  initializeMsdl2msrTraceOah ();
#endif

    void                  initializeMsdlLanguagesOptions ();

    void                  initializeMsdlWholeInputScansOptions ();

    void                  initializeGenerateCodeOptions ();

  public:

    // visitors
    // ------------------------------------------------------

    void                  acceptIn  (basevisitor* v) override;
    void                  acceptOut (basevisitor* v) override;

    void                  browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    void                  printMsdl2msrHelp ();

    void                  printMsdl2msrValues (unsigned int fieldWidth);

  public:

    // fields
    // ------------------------------------------------------

    // trace
#ifdef TRACING_IS_ENABLED
    bool                  fTraceTokens;
    bool                  fTraceTokensDetails;

    bool                  fTraceSyntax;
    bool                  fTraceSyntaxDetails;

    bool                  fTraceSyntaxErrorRecovery;
    bool                  fTraceSyntaxErrorRecoveryDetails;
#endif

    // languages
    // --------------------------------------

    msdlUserLanguageKind  fMsdlUserLanguageKind;

    msdlKeywordsLanguageKind
                          fMsdlKeywordsInputLanguageKind;

    msrQuarterTonesPitchesLanguageKind
                          fMsdlQuarterTonesPitchesLanguageKind;

    // whole input scan
    // --------------------------------------

    bool                  fLexicalAnalysisOnly;

    bool                  fIgnoreSeparatorTokens;

    msdlKeywordsLanguageKind
                          fMsdlKeywordsTranslationLanguageKind;

    msdlCommentsTypeKind  fMsdlCommentsTypeTranslationKind;

    // output kind
    // --------------------------------------

    generatorOutputKind   fGeneratorOutputKind;
    S_generatorOutputKindAtom
                          fGeneratorOutputKindAtom;

    // quit
    // --------------------------------------

    bool                  fQuitOnLexicalErrors;
};
typedef SMARTP<msdl2msrOahGroup> S_msdl2msrOahGroup;
EXP ostream& operator<< (ostream& os, const S_msdl2msrOahGroup& elt);

EXP extern S_msdl2msrOahGroup gGlobalMsdl2msrOahGroup;

//______________________________________________________________________________
EXP S_msdl2msrOahGroup createGlobalMsdl2msrOahGroup ();


}


#endif
