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

#include "Mikrokosmos3WanderingOahTypes.h"
#include "msrGenerationAPIs.h"


namespace MusicXML2
{

//_______________________________________________________________________________
#define K_GENERATED_CODE_KIND_SHORT_NAME "gen"
#define K_GENERATED_CODE_KIND_LONG_NAME  "generate-code"

//______________________________________________________________________________
class EXP mkkGenerateCodeKindAtom : public oahAtomWithValue
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<mkkGenerateCodeKindAtom> create (
      string             shortName,
      string             longName,
      string             description,
      string             valueSpecification,
      string             variableName,
      mkkGenerateCodeKind& mkkGenerateCodeKindVariable);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    mkkGenerateCodeKindAtom (
      string             shortName,
      string             longName,
      string             description,
      string             valueSpecification,
      string             variableName,
      mkkGenerateCodeKind& mkkGenerateCodeKindVariable);

    virtual ~mkkGenerateCodeKindAtom ();

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

    mkkGenerateCodeKind&    fGenerateCodeKindVariable;
};
typedef SMARTP<mkkGenerateCodeKindAtom> S_mkkGenerateCodeKindAtom;
EXP ostream& operator<< (ostream& os, const S_mkkGenerateCodeKindAtom& elt);

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

    void                  setGenerateCodeKind (mkkGenerateCodeKind value)
                              { fGenerateCodeKind = value; }

    mkkGenerateCodeKind  getGenerateCodeKind () const
                              { return fGenerateCodeKind;  }

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

    /* JMI
    void                  initializeBrailleEncodingOptions ();

    void                  initializeBrailleByteOrderingOptions ();

    void                  initializeBrailleMusicFileNameOptions ();

    void                  initializeBraillePageParametersOptions ();

    void                  initializeBrailleCodeGenerationOptions ();
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

    // generate code kind

    mkkGenerateCodeKind  fGenerateCodeKind;
    S_mkkGenerateCodeKindAtom
                          fGenerateCodeKindAtom;

/* JMI
    // braille output kind

    mkkGenerateCodeKind     fBrailleOutputKind;
    S_mkkGenerateCodeKindAtom
                          fBrailleOutputKindAtom;

    // UTF encoding

    bsrUTFKind            fUTFKind;
    S_brailleUTFKindAtom  fUTFKindAtom;

    // byte ordering

    bsrByteOrderingKind   fByteOrderingKind;
    S_brailleByteOrderingKindAtom
                          fByteOrderingKindAtom;

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
