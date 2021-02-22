/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef ___msdl2msdrInsiderHandler___
#define ___msdl2msdrInsiderHandler___

#include "msrBasicTypes.h"

#include "msdlKeywords.h"

#include "oahBasicTypes.h"

#include "oahAtomsCollection.h"

#include "oahInsiderHandlers.h"

#include "msdl2msrOah.h"


namespace MusicXML2
{

//_______________________________________________________________________________
class EXP msdl2msdrInsiderHandler : public oahInsiderHandler
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<msdl2msdrInsiderHandler> create (
                            const string&       executableName,
                            const string&       handlerHeader,
                            generatorOutputKind generatorOutputKind);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msdl2msdrInsiderHandler (
                            const string&       executableName,
                            const string&       handlerHeader,
                            generatorOutputKind generatorOutputKind);

    virtual               ~msdl2msdrInsiderHandler ();

  public:

    // set and get
    // ------------------------------------------------------

    // generated output kind
    void                  setGeneratorOutputKind (generatorOutputKind value)
                              { fGeneratorOutputKind = value; }
    generatorOutputKind   getGeneratorOutputKind () const
                              { return fGeneratorOutputKind; }

  private:

    // private initialization
    // ------------------------------------------------------

    void                  createTheMsdlCompilerPrefixes ();

    void                  createTheMsdlCompilerOptionGroups (
                            const string&        executableName,
                            generatorOutputKind generatorOutputKind);

  public:

    // public services
    // ------------------------------------------------------

    void                  handlerOahError (const string& errorMessage) override;

    string                handlerExecutableAboutInformation () const override;

    void                  checkOptionsAndArgumentsFromArgcAndArgv () const override;

    string                fetchOutputFileNameFromTheOptions () const override;

    // quiet mode
    void                  enforceHandlerQuietness () override;

    // consistency check
    void                  checkHandlerOptionsConsistency () override;

  public:

    // visitors
    // ------------------------------------------------------

  public:

    // print
    // ------------------------------------------------------

    void                  print (ostream& os) const;

  protected:

    // protected services
    // ------------------------------------------------------

  private:

    // private services
    // ------------------------------------------------------

    string                usageInformation (
                            generatorOutputKind generatorOutputKind);

    string                msdl2msdrAboutInformation (
                            generatorOutputKind theGeneratorOutputKind) const;

  private:

    // private fields
    // ------------------------------------------------------

    // generated output kind
    generatorOutputKind   fGeneratorOutputKind;
};
typedef SMARTP<msdl2msdrInsiderHandler> S_msdl2msdrInsiderHandler;
EXP ostream& operator<< (ostream& os, const S_msdl2msdrInsiderHandler& elt);

//______________________________________________________________________________
class EXP msdl2msdrInsiderOahGroup : public oahGroup
{
  public:

    static SMARTP<msdl2msdrInsiderOahGroup> create ();

  public:

    // initialisation
    // ------------------------------------------------------

    void                  initializeMsdlCompilerInsiderOahGroup ();

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msdl2msdrInsiderOahGroup ();

    virtual               ~msdl2msdrInsiderOahGroup ();

  public:

    // set and get
    // ------------------------------------------------------

    // keywords input language
    void                  setKeywordsInputLanguageKind (
                            msdlKeywordsLanguageKind value)
                              { fKeywordsInputLanguageKind = value; }
    msdlKeywordsLanguageKind
                          getKeywordsInputLanguageKind () const
                              { return fKeywordsInputLanguageKind; }

    // keywords translation language
    void                  setKeywordsTranslationLanguageKind (
                            msdlKeywordsLanguageKind value)
                              { fKeywordsTranslationLanguageKind = value; }
    msdlKeywordsLanguageKind
                          getKeywordsTranslationLanguageKind () const
                              { return fKeywordsTranslationLanguageKind; }

    // generated output kind
    void                  setGeneratorOutputKind (generatorOutputKind value)
                              { fGeneratorOutputKind = value; }
    generatorOutputKind   getGeneratorOutputKind () const
                              { return fGeneratorOutputKind; }

    // Guido
    void                  setGenerateComments ()
                              { fGenerateComments = true; }
    bool                  getGenerateComments () const
                              { return fGenerateComments; }

    void                  setGenerateStem ()
                              { fGenerateStem = true; }
    bool                  getGenerateStem () const
                              { return fGenerateStem; }

    void                  setGenerateBars ()
                              { fGenerateBars = true; }
    bool                  getGenerateBars () const
                              { return fGenerateBars; }

    // quit after some passes
    void                  setQuitAfterPass2a ()
                              { fQuitAfterPass2a = true; }
    bool                  getQuitAfterPass2a () const
                              { return fQuitAfterPass2a; }

    void                  setQuitAfterPass2b ()
                              { fQuitAfterPass2b = true; }
    bool                  getQuitAfterPass2b () const
                              { return fQuitAfterPass2b; }

/* JMI
    // UTF encoding

    void                  setUTFKind (bsrUTFKind value)
                              { fUTFKind = value; }
    bsrUTFKind            getUTFKind () const
                              { return fUTFKind;  }

    // byte ordering

    void                  setByteOrderingKind (bsrByteOrderingKind value)
                              { fByteOrderingKind = value; }
    bsrByteOrderingKind   getByteOrderingKind () const
                              { return fByteOrderingKind;  }

    // braille music file name

    void                  setUseEncodingInFileName ()
                              { fUseEncodingInFileName = true; }
    bool                  getUseEncodingInFileName () const
                              { return fUseEncodingInFileName;  }

    // page parameters

    void                  setCellsPerLine (unsigned int value)
                              { fCellsPerLine = value; }
    unsigned int          getCellsPerLine () const
                              { return fCellsPerLine;  }

    void                  setMeasuresPerLine (unsigned int value)
                              { fMeasuresPerLine = value; }
    unsigned int          getMeasuresPerLine () const
                              { return fMeasuresPerLine;  }

    void                  setLinesPerPage (unsigned int value)
                              { fLinesPerPage = value; }
    unsigned int          getLinesPerPage () const
                              { return fLinesPerPage;  }

    // code generation

    void                  setXml2brlInfos ()
                              { fXml2brlInfos = true; }
    bool                  getXml2brlInfos () const
                              { return fXml2brlInfos;  }

    void                  setNoBrailleCode ()
                              { fNoBrailleCode = true; }
    bool                  getNoBrailleCode () const
                              { return fNoBrailleCode;  }
*/

  public:

    // public services
    // ------------------------------------------------------

    // quiet mode
    void                  enforceGroupQuietness () override;

    // consistency check
    void                  checkGroupOptionsConsistency () override;

  public:

    // visitors
    // ------------------------------------------------------

    void                  acceptIn  (basevisitor* v) override;
    void                  acceptOut (basevisitor* v) override;

    void                  browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    void                  printMsdlCompilerInsiderOahGroupHelp ();

    void                  printMsdlCompilerInsiderOahGroupValues (
                            unsigned int fieldWidth);

  private:

    // private services
    // ------------------------------------------------------

    void                  createInsiderGuidoSubGroup ();

    void                  createInsiderLilypondSubGroup ();

    void                  createInsiderQuitSubGroup ();

  private:

    // private fields
    // ------------------------------------------------------

    // keywords input language
    msdlKeywordsLanguageKind
                          fKeywordsInputLanguageKind;

    // keywords translation language
    msdlKeywordsLanguageKind
                          fKeywordsTranslationLanguageKind;

    // generated output kind
    generatorOutputKind   fGeneratorOutputKind;

    // Guido
    bool                  fGenerateComments;
    bool                  fGenerateStem;
    bool                  fGenerateBars;

    // quit after some passes
    bool                  fQuitAfterPass2a;
    bool                  fQuitAfterPass2b;

    /*
    // UTF encoding (8 or 16 bits)

    bsrUTFKind            fUTFKind;
    S_brailleUTFKindAtom  fUTFKindAtom;

    // braille output kind

    bsrBrailleOutputKind  fBrailleOutputKind;
    S_brailleOutputKindAtom
                          fBrailleOutputKindAtom;

    // byte ordering

    bsrByteOrderingKind   fByteOrderingKind;
    S_brailleByteOrderingKindAtom
                          fByteOrderingKindAtom;

    */
};
typedef SMARTP<msdl2msdrInsiderOahGroup> S_msdl2msdrInsiderOahGroup;
EXP ostream& operator<< (ostream& os, const S_msdl2msdrInsiderOahGroup& elt);

EXP extern S_msdl2msdrInsiderOahGroup gGlobalMsdlCompilerInsiderOahGroup;

//______________________________________________________________________________
S_msdl2msdrInsiderOahGroup createGlobalMsdlCompilerInsiderOahGroup ();


}


#endif
