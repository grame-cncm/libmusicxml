/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef ___bsr2brailleOah___
#define ___bsr2brailleOah___

#include "msrSegnosAndCodas.h"

#include "oahBasicTypes.h"

#include "bsrBasicTypes.h"

#include "bsr2brailleCodeGenerators.h"


namespace MusicXML2
{

//______________________________________________________________________________
class brailleOutputKindAtom : public oahAtomWithValue
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<brailleOutputKindAtom> create (
      string                shortName,
      string                longName,
      string                description,
      string                valueSpecification,
      string                variableName,
      bsrBrailleOutputKind& brailleOutputKindVariable);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    brailleOutputKindAtom (
      string                shortName,
      string                longName,
      string                description,
      string                valueSpecification,
      string                variableName,
      bsrBrailleOutputKind& brailleOutputKindVariable);

    virtual ~brailleOutputKindAtom ();

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

    bsrBrailleOutputKind& fBrailleOutputKindVariable;
};
typedef SMARTP<brailleOutputKindAtom> S_brailleOutputKindAtom;
EXP ostream& operator<< (ostream& os, const S_brailleOutputKindAtom& elt);

//______________________________________________________________________________
class brailleUTFKindAtom : public oahAtomWithValue
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<brailleUTFKindAtom> create (
      string      shortName,
      string      longName,
      string      description,
      string      valueSpecification,
      string      variableName,
      bsrUTFKind& brailleUTFKindVariable);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    brailleUTFKindAtom (
      string      shortName,
      string      longName,
      string      description,
      string      valueSpecification,
      string      variableName,
      bsrUTFKind& brailleUTFKindVariable);

    virtual ~brailleUTFKindAtom ();

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

    bsrUTFKind&           fBsrUTFKindVariable;
};
typedef SMARTP<brailleUTFKindAtom> S_brailleUTFKindAtom;
EXP ostream& operator<< (ostream& os, const S_brailleUTFKindAtom& elt);

//______________________________________________________________________________
class brailleByteOrderingKindAtom : public oahAtomWithValue
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<brailleByteOrderingKindAtom> create (
      string               shortName,
      string               longName,
      string               description,
      string               valueSpecification,
      string               variableName,
      bsrByteOrderingKind& brailleByteOrderingKindVariable);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    brailleByteOrderingKindAtom (
      string               shortName,
      string               longName,
      string               description,
      string               valueSpecification,
      string               variableName,
      bsrByteOrderingKind& brailleByteOrderingKindVariable);

    virtual ~brailleByteOrderingKindAtom ();

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

    virtual void          acceptIn  (basevisitor* v) override;
    virtual void          acceptOut (basevisitor* v) override;

    virtual void          browseData (basevisitor* v) override;

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

    bsrByteOrderingKind&  fBsrByteOrderingKindVariable;
};
typedef SMARTP<brailleByteOrderingKindAtom> S_brailleByteOrderingKindAtom;
EXP ostream& operator<< (ostream& os, const S_brailleByteOrderingKindAtom& elt);

//______________________________________________________________________________
class bsr2brailleOahGroup : public oahGroup
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<bsr2brailleOahGroup> create ();

  private:

    // initialisation
    // ------------------------------------------------------

    void                  initializeBsr2brailleOahGroup ();

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    bsr2brailleOahGroup ();

    virtual ~bsr2brailleOahGroup ();

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

    // fields
    // ------------------------------------------------------

    // braille output kind

    bsrBrailleOutputKind  getBrailleOutputKind () const
                              { return fBrailleOutputKind;  }

    // UTF encoding

    bsrUTFKind            getUTFKind () const
                              { return fUTFKind;  }

    // byte ordering

    bsrByteOrderingKind   getByteOrderingKind () const
                              { return fByteOrderingKind;  }

    // braille music file name

    bool                  getUseEncodingInFileName () const
                              { return fUseEncodingInFileName;  }

    // page parameters

    int                   getCellsPerLine () const
                              { return fCellsPerLine;  }
    int                   getMeasuresPerLine () const
                              { return fMeasuresPerLine;  }
    int                   getLinesPerPage () const
                              { return fLinesPerPage;  }

    // code generation

    bool                  getXml2brlInfos () const
                              { return fXml2brlInfos;  }

    bool                  getNoBrailleCode () const
                              { return fNoBrailleCode;  }

  public:

    // public services
    // ------------------------------------------------------

  private:

    // private services
    // ------------------------------------------------------

#ifdef TRACING_IS_ENABLED
// JMI    void                  initializeBsr2brailleTraceOah ();
#endif

    void                  initializeBrailleEncodingOptions ();

    void                  initializeBrailleByteOrderingOptions ();

    void                  initializeBrailleMusicFileNameOptions ();

    void                  initializeBraillePageParametersOptions ();

    void                  initializeBrailleCodeGenerationOptions ();

  public:

    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v) override;
    virtual void          acceptOut (basevisitor* v) override;

    virtual void          browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    void                  printBsr2brailleOahHelp ();

    void                  printBsr2brailleOahValues (int fieldWidth);

  private:

    // private fields
    // ------------------------------------------------------

    // braille output kind

    bsrBrailleOutputKind  fBrailleOutputKind;
    S_brailleOutputKindAtom
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
};
typedef SMARTP<bsr2brailleOahGroup> S_bsr2brailleOahGroup;
EXP ostream& operator<< (ostream& os, const S_bsr2brailleOahGroup& elt);

EXP extern S_bsr2brailleOahGroup gGlobalBsr2brailleOahGroup;

//______________________________________________________________________________
EXP S_bsr2brailleOahGroup createGlobalBsr2brailleOahGroup ();


}


#endif
