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

#include "msrSegnosAndCodas.h"

//#include "oahBasicTypes.h"


namespace MusicXML2
{

//______________________________________________________________________________
enum generatedCodeKind {
  k_NoGeneratedCode,
  kGuido, kLilyPond, kBrailleMusic, kMusicXML
};

string generatedCodeKindAsString (generatedCodeKind kind);

extern map<string, generatedCodeKind>
  gGlobalGeneratedCodeKindsMap;

string existingGeneratedCodeKinds (unsigned int namesListMaxLength);

void initializeGeneratedCodeKindsMap ();

//______________________________________________________________________________
class EXP generatedCodeKindAtom : public oahAtomWithValue
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<generatedCodeKindAtom> create (
      string                shortName,
      string                longName,
      string                description,
      string                valueSpecification,
      string                variableName,
      generatedCodeKind& generatedCodeKindVariable);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    generatedCodeKindAtom (
      string                shortName,
      string                longName,
      string                description,
      string                valueSpecification,
      string                variableName,
      generatedCodeKind& generatedCodeKindVariable);

    virtual ~generatedCodeKindAtom ();

  public:

    // set and get
    // ------------------------------------------------------

  public:

    // public services
    // ------------------------------------------------------

    void                  applyAtomWithValue (
                            string   theString,
                            ostream& os) override;

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

    generatedCodeKind&    fGeneratedCodeKindVariable;
};
typedef SMARTP<generatedCodeKindAtom> S_generatedCodeKindAtom;
EXP ostream& operator<< (ostream& os, const S_generatedCodeKindAtom& elt);

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

    // braille output kind

    void                  setGeneratedCodeKind (generatedCodeKind value)
                              { fGeneratedCodeKind = value; }
    generatedCodeKind     getGeneratedCodeKind () const
                              { return fGeneratedCodeKind;  }

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

    void                  setCellsPerLine (int value)
                              { fCellsPerLine = value; }
    int                   getCellsPerLine () const
                              { return fCellsPerLine;  }

    void                  setMeasuresPerLine (int value)
                              { fMeasuresPerLine = value; }
    int                   getMeasuresPerLine () const
                              { return fMeasuresPerLine;  }

    void                  setLinesPerPage (int value)
                              { fLinesPerPage = value; }
    int                   getLinesPerPage () const
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

    void                  initializeGeneratedCodeOptions ();

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

    void                  printMikrokosmos3WanderingOahValues (unsigned int fieldWidth);

  private:

    // private fields
    // ------------------------------------------------------

    // generate code kind

    generatedCodeKind     fGeneratedCodeKind;
    S_generatedCodeKindAtom
                          fGeneratedCodeKindAtom;

/* JMI
    // braille output kind

    generatedCodeKind     fBrailleOutputKind;
    S_generatedCodeKindAtom
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

    int                   fCellsPerLine;
    int                   fMeasuresPerLine;
    int                   fLinesPerPage;

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
