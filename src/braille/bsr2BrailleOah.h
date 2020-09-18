/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef ___bsr2BrailleOah___
#define ___bsr2BrailleOah___

#include "msrSegnosAndCodas.h"

#include "oahBasicTypes.h"

#include "bsrBasicTypes.h"

#include "bsrBrailleCodeGenerators.h"


namespace MusicXML2
{

//______________________________________________________________________________
class brailleOutputKindAtom : public oahValuedAtom
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

    // services
    // ------------------------------------------------------

    S_oahValuedAtom       handleOptionUnderName (
                            string   optionName,
                            ostream& os) override;

    void                  handleValuedAtomValue (
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

    void                  printValuedAtomOptionsValues (
                            ostream& os,
                            int      valueFieldWidth) const override;

  private:

    // fields
    // ------------------------------------------------------

    bsrBrailleOutputKind& fBrailleOutputKindVariable;
};
typedef SMARTP<brailleOutputKindAtom> S_brailleOutputKindAtom;
EXP ostream& operator<< (ostream& os, const S_brailleOutputKindAtom& elt);

//______________________________________________________________________________
class brailleUTFKindAtom : public oahValuedAtom
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

    // services
    // ------------------------------------------------------

    void                  handleValuedAtomValue (
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

    void                  printValuedAtomOptionsValues (
                            ostream& os,
                            int      valueFieldWidth) const override;

  private:

    // fields
    // ------------------------------------------------------

    bsrUTFKind&           fBsrUTFKindVariable;
};
typedef SMARTP<brailleUTFKindAtom> S_brailleUTFKindAtom;
EXP ostream& operator<< (ostream& os, const S_brailleUTFKindAtom& elt);

//______________________________________________________________________________
class brailleByteOrderingKindAtom : public oahValuedAtom
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

    // services
    // ------------------------------------------------------

    void                  handleValuedAtomValue (
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

    void                  printValuedAtomOptionsValues (
                            ostream& os,
                            int      valueFieldWidth) const override;

  private:

    // fields
    // ------------------------------------------------------

    bsrByteOrderingKind&  fBsrByteOrderingKindVariable;
};
typedef SMARTP<brailleByteOrderingKindAtom> S_brailleByteOrderingKindAtom;
EXP ostream& operator<< (ostream& os, const S_brailleByteOrderingKindAtom& elt);

//______________________________________________________________________________
class bsr2BrailleOah : public oahGroup
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<bsr2BrailleOah> create (
      S_oahHandler handlerUpLink);

    SMARTP<bsr2BrailleOah>   createCloneWithDetailedTrace ();

  private:

    // initialisation
    // ------------------------------------------------------

    void                  initializeBsr2BrailleOah (
                            bool boolOptionsInitialValue);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    bsr2BrailleOah (
      S_oahHandler handlerUpLink);

    virtual ~bsr2BrailleOah ();

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

    void                  setAllBsr2BrailleTraceOah (
                            bool boolOptionsInitialValue);

  public:

    // public services
    // ------------------------------------------------------

  private:

    // private services
    // ------------------------------------------------------

#ifdef TRACE_OAH
    void                  initializeBsr2BrailleTraceOah (
                            bool boolOptionsInitialValue);
#endif

    void                  initializeBrailleEncodingOptions (
                            bool boolOptionsInitialValue);

    void                  initializeBrailleByteOrderingOptions (
                            bool boolOptionsInitialValue);

    void                  initializeBrailleMusicFileNameOptions (
                            bool boolOptionsInitialValue);

    void                  initializeBraillePageParametersOptions (
                            bool boolOptionsInitialValue);

    void                  initializeBrailleCodeGenerationOptions (
                            bool boolOptionsInitialValue);

  public:

    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v) override;
    virtual void          acceptOut (basevisitor* v) override;

    virtual void          browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    void                  printBsr2BrailleOahHelp ();

    void                  printBsr2BrailleOahValues (int fieldWidth);

  public:

    // fields
    // ------------------------------------------------------

    // braille output kind

    bsrBrailleOutputKind  fBrailleOutputKind;

    // UTF encoding

    bsrUTFKind            fUTFKind;

    // byte ordering

    bsrByteOrderingKind   fByteOrderingKind;

    // braille music file name

    bool                  fDontUseEncodingInFileName;

    // page parameters

    int                   fCellsPerLine;
    int                   fMeasuresPerLine;
    int                   fLinesPerPage;

    // code generation

    bool                  fXml2brlInfos;

    bool                  fNoBrailleCode;
};
typedef SMARTP<bsr2BrailleOah> S_bsr2BrailleOah;
EXP ostream& operator<< (ostream& os, const S_bsr2BrailleOah& elt);

EXP extern S_bsr2BrailleOah gGlobalBsr2BrailleOah;
EXP extern S_bsr2BrailleOah gGlobalBsr2BrailleOahUserChoices;
EXP extern S_bsr2BrailleOah gGlobalBsr2BrailleOahWithDetailedTrace;

//______________________________________________________________________________
void initializeBsr2BrailleOahHandling (
  S_oahHandler handler);


}


#endif
