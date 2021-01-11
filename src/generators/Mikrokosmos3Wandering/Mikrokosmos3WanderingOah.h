/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef ___Mikrokosmos3WanderingOah___
#define ___Mikrokosmos3WanderingOah___

#include "msr.h"

#include "generatorsBasicTypes.h"

#include "msrGenerationAPIs.h"

#include "bsr2brailleOah.h"
#include "bsr2brailleCodeGenerators.h"

#include "Mikrokosmos3WanderingOahTypes.h"


namespace MusicXML2
{

//_______________________________________________________________________________
#define K_GENERATED_OUTPUT_KIND_SHORT_NAME "gen"
#define K_GENERATED_OUTPUT_KIND_LONG_NAME  "generate"

//______________________________________________________________________________
class EXP generatorOutputKindAtom : public oahAtomWithValue
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<generatorOutputKindAtom> create (
      string             shortName,
      string             longName,
      string             description,
      string             valueSpecification,
      string             variableName,
      generatorOutputKind& generatorOutputKindVariable);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    generatorOutputKindAtom (
      string             shortName,
      string             longName,
      string             description,
      string             valueSpecification,
      string             variableName,
      generatorOutputKind& generatorOutputKindVariable);

    virtual ~generatorOutputKindAtom ();

  public:

    // set and get
    // ------------------------------------------------------

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

    generatorOutputKind&    fGeneratorOutputKindVariable;
};
typedef SMARTP<generatorOutputKindAtom> S_generatorOutputKindAtom;
EXP ostream& operator<< (ostream& os, const S_generatorOutputKindAtom& elt);

//______________________________________________________________________________
class EXP Mikrokosmos3WanderingOahGroup : public oahGroup
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<Mikrokosmos3WanderingOahGroup> create ();

  private:

    // initialisation
    // ------------------------------------------------------

    void                  initializeMikrokosmos3WanderingOahGroup ();

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    Mikrokosmos3WanderingOahGroup ();

    virtual ~Mikrokosmos3WanderingOahGroup ();

  public:

    // quiet mode
    // ------------------------------------------------------

    void                  enforceGroupQuietness () override;

  public:

    // consistency check
    // ------------------------------------------------------

    void                  checkGroupOptionsConsistency () override;

  public:

    // set and get
    // ------------------------------------------------------

    // fields
    // ------------------------------------------------------

    // generaton API kind

    void                  setGenerationAPIKind (msrGenerationAPIKind value)
                              { fGenerationAPIKind = value; }

    msrGenerationAPIKind  getGenerationAPIKind () const
                              { return fGenerationAPIKind;  }

    // generate code kind

    void                  setGeneratorOutputKind (generatorOutputKind value)
                              { fGeneratorOutputKind = value; }

    generatorOutputKind  getGeneratorOutputKind () const
                              { return fGeneratorOutputKind;  }

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

  private:

    // private services
    // ------------------------------------------------------

#ifdef TRACING_IS_ENABLED
// JMI    void                  initializeMikrokosmos3WanderingTraceOah ();
#endif

    void                  initializeGenerationAPIOptions ();

    void                  initializeGenerateCodeOptions ();

    void                  initializeBrailleEncodingOptions ();

    void                  initializeBrailleByteOrderingOptions ();

    void                  initializeBrailleMusicFileNameOptions ();

    void                  initializeBraillePageParametersOptions ();

    void                  initializeBrailleCodeGenerationOptions ();
    /* JMI
    */

  public:

    // visitors
    // ------------------------------------------------------

    void                  acceptIn  (basevisitor* v) override;
    void                  acceptOut (basevisitor* v) override;

    void                  browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    void                  printMikrokosmos3WanderingOahHelp ();

    void                  printMikrokosmos3WanderingOahValues (
                            unsigned int fieldWidth);

  private:

    // private fields
    // ------------------------------------------------------

    // generation API kind

    msrGenerationAPIKind  fGenerationAPIKind;
    S_msrGenerationAPIKindAtom
                          fGenerationAPIKindAtom;

    // generator output kind

    generatorOutputKind   fGeneratorOutputKind;
    S_generatorOutputKindAtom
                          fGeneratorOutputKindAtom;

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

/* JMI
    // braille music file name

    bool                  fUseEncodingInFileName;

    // page parameters

    unsigned int          fCellsPerLine;
    unsigned int          fMeasuresPerLine;
    unsigned int          fLinesPerPage;

    // code generation

    bool                  fXml2brlInfos;

    bool                  fNoBrailleCode;
    */
};
typedef SMARTP<Mikrokosmos3WanderingOahGroup> S_Mikrokosmos3WanderingOahGroup;
EXP ostream& operator<< (ostream& os, const S_Mikrokosmos3WanderingOahGroup& elt);

EXP extern S_Mikrokosmos3WanderingOahGroup gGlobalMikrokosmos3WanderingOahGroup;

//______________________________________________________________________________
EXP S_Mikrokosmos3WanderingOahGroup createGlobalMikrokosmos3WanderingOahGroup ();


}


#endif
