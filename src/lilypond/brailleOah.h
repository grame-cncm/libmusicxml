/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef ___brailleOah___
#define ___brailleOah___

#include <set>

#include "oahBasicTypes.h"

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
                            ostream& os);

    void                  handleValue (
                            string   theString,
                            ostream& os);

  public:

    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v);
    virtual void          acceptOut (basevisitor* v);

    virtual void          browseData (basevisitor* v);

  public:

    // print
    // ------------------------------------------------------

    string                asShortNamedOptionString () const;
    string                asActualLongNamedOptionString () const;

    void                  print (ostream& os) const;

    void                  printAtomOptionsValues (
                            ostream& os,
                            int      valueFieldWidth) const;

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

    void                  handleValue (
                            string   theString,
                            ostream& os);

  public:

    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v);
    virtual void          acceptOut (basevisitor* v);

    virtual void          browseData (basevisitor* v);

  public:

    // print
    // ------------------------------------------------------

    string                asShortNamedOptionString () const;
    string                asActualLongNamedOptionString () const;

    void                  print (ostream& os) const;

    void                  printAtomOptionsValues (
                            ostream& os,
                            int      valueFieldWidth) const;

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

    void                  handleValue (
                            string   theString,
                            ostream& os);

  public:

    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v);
    virtual void          acceptOut (basevisitor* v);

    virtual void          browseData (basevisitor* v);

    // print
    // ------------------------------------------------------

    string                asShortNamedOptionString () const;
    string                asActualLongNamedOptionString () const;

    void                  print (ostream& os) const;

    void                  printAtomOptionsValues (
                            ostream& os,
                            int      valueFieldWidth) const;

  private:

    // fields
    // ------------------------------------------------------

    bsrByteOrderingKind&  fBsrByteOrderingKindVariable;
};
typedef SMARTP<brailleByteOrderingKindAtom> S_brailleByteOrderingKindAtom;
EXP ostream& operator<< (ostream& os, const S_brailleByteOrderingKindAtom& elt);

//______________________________________________________________________________
class brailleOah : public oahGroup
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<brailleOah> create (
      S_oahHandler handlerUpLink);

    SMARTP<brailleOah>    createCloneWithDetailedTrace ();

  public:

    // initialisation
    // ------------------------------------------------------

    void                  initializeBrailleOah (
                            bool boolOptionsInitialValue);

    void                  printBrailleOahHelp ();

    void                  printBrailleOahValues (int fieldWidth);

  public:

    // constructors/destructor
    // ------------------------------------------------------

    brailleOah (
      S_oahHandler handlerUpLink);

    virtual ~brailleOah ();

    // set and get
    // ------------------------------------------------------

  public:

    // quiet mode
    // ------------------------------------------------------

    void                  enforceQuietness ();

  public:

    // consistency check
    // ------------------------------------------------------

    virtual void          checkOptionsConsistency ();

  public:

    // public services
    // ------------------------------------------------------

  public:

    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v);
    virtual void          acceptOut (basevisitor* v);

    virtual void          browseData (basevisitor* v);

  private:

    // private services
    // ------------------------------------------------------

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

    // braille output kind
    // --------------------------------------

    bsrBrailleOutputKind  fBrailleOutputKind;

    // UTF encoding
    // --------------------------------------

    bsrUTFKind            fUTFKind;

    // byte ordering
    // --------------------------------------

    bsrByteOrderingKind   fByteOrderingKind;

    // braille music file name
    // --------------------------------------

    bool                  fDontUseEncodingInFileName;

    // page parameters
    // --------------------------------------

    int                   fCellsPerLine;
    int                   fMeasuresPerLine;
    int                   fLinesPerPage;

    // code generation
    // --------------------------------------

    bool                  fXml2brlInfos;

    bool                  fNoBrailleCode;
};
typedef SMARTP<brailleOah> S_brailleOah;
EXP ostream& operator<< (ostream& os, const S_brailleOah& elt);

EXP extern S_brailleOah gBrailleOah;
EXP extern S_brailleOah gBrailleOahUserChoices;
EXP extern S_brailleOah gBrailleOahWithDetailedTrace;

//______________________________________________________________________________
void initializeBrailleOahHandling (
  S_oahHandler handler);


}


#endif
