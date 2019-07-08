/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef ___brailleOptions___
#define ___brailleOptions___

#include <set>

#include "oahBasicTypes.h"

#include "bsrBrailleCodeGenerators.h"


namespace MusicXML2
{

//______________________________________________________________________________
class optionsUTFKindItem : public oahValuedAtom
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<optionsUTFKindItem> create (
      string     shortName,
      string     longName,
      string     description,
      string     optionsValueSpecification,
      string     optionsUTFKindItemVariableName,
      bsrUTFKind optionsUTFKindItemVariable);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    optionsUTFKindItem (
      string     shortName,
      string     longName,
      string     description,
      string     optionsValueSpecification,
      string     optionsUTFKindItemVariableName,
      bsrUTFKind optionsUTFKindItemVariable);

    virtual ~optionsUTFKindItem ();

  public:

    // set and get
    // ------------------------------------------------------

    void                  setOptionsUTFKindItemVariable (bsrUTFKind value)
                              {
                                fOptionsUTFKindItemVariable = value;
                              }

    // services
    // ------------------------------------------------------

    // print
    // ------------------------------------------------------

    void                  print (ostream& os) const;

    void                  printOptionsValues (
                            ostream& os,
                            int      valueFieldWidth) const;

  private:

    // fields
    // ------------------------------------------------------

    bsrUTFKind            fOptionsUTFKindItemVariable;
};
typedef SMARTP<optionsUTFKindItem> S_optionsUTFKindItem;
EXP ostream& operator<< (ostream& os, const S_optionsUTFKindItem& elt);

//______________________________________________________________________________
class optionsByteOrderingKindItem : public oahValuedAtom
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<optionsByteOrderingKindItem> create (
      string              shortName,
      string              longName,
      string              description,
      string              optionsValueSpecification,
      string              optionsByteOrderingKindItemVariableName,
      bsrByteOrderingKind optionsByteOrderingKindItemVariable);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    optionsByteOrderingKindItem (
      string              shortName,
      string              longName,
      string              description,
      string              optionsValueSpecification,
      string              optionsByteOrderingKindItemVariableName,
      bsrByteOrderingKind optionsByteOrderingKindItemVariable);

    virtual ~optionsByteOrderingKindItem ();

  public:

    // set and get
    // ------------------------------------------------------

    void                  setOptionsByteOrderingKindItemVariable (
                            bsrByteOrderingKind value)
                              {
                                fOptionsByteOrderingKindItemVariable = value;
                              }

    // services
    // ------------------------------------------------------

    // print
    // ------------------------------------------------------

    void                  print (ostream& os) const;

    void                  printOptionsValues (
                            ostream& os,
                            int      valueFieldWidth) const;

  private:

    // fields
    // ------------------------------------------------------

    bsrByteOrderingKind   fOptionsByteOrderingKindItemVariable;
};
typedef SMARTP<optionsByteOrderingKindItem> S_optionsByteOrderingKindItem;
EXP ostream& operator<< (ostream& os, const S_optionsByteOrderingKindItem& elt);

//______________________________________________________________________________
class brailleOptions : public oahGroup
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<brailleOptions> create (
      S_oahHandler oahHandler);

    SMARTP<brailleOptions>        createCloneWithDetailedTrace ();

  public:

    // initialisation
    // ------------------------------------------------------

    void                  initializeBrailleOptions (
                            bool boolOptionsInitialValue);

    void                  printBrailleOptionsHelp ();

    void                  printBrailleOptionsValues (int fieldWidth);

  public:

    // constructors/destructor
    // ------------------------------------------------------

    brailleOptions (
      S_oahHandler oahHandler);

    virtual ~brailleOptions ();

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

    virtual S_oahValuedAtom
                          handleOptionsItem (
                            ostream&      os,
                            S_oahAtom item);

    virtual void          handleOptionsItemValue (
                            ostream&      os,
                            S_oahAtom item,
                            string        theString);

  private:

    // private services
    // ------------------------------------------------------

    void                  initializeBrailleUTFEncodingOptions (
                            bool boolOptionsInitialValue);

    void                  initializeBrailleByteOrderingOptions (
                            bool boolOptionsInitialValue);

    void                  initializeBrailleMusicFileNameOptions (
                            bool boolOptionsInitialValue);

    void                  initializeBraillePageParametersOptions (
                            bool boolOptionsInitialValue);

    void                  initializeBrailleCodeGenerationOptions (
                            bool boolOptionsInitialValue);

    virtual void          handleOptionsUTFKindItemValue (
                            ostream&             os,
                            S_optionsUTFKindItem UTFKindItem,
                            string               theString);

    virtual void          handleOptionsByteOrderingKindItemValue (
                            ostream&                      os,
                            S_optionsByteOrderingKindItem byteOrderingKindItem,
                            string                        theString);

  public:

    // UTF encoding
    // --------------------------------------

    bsrUTFKind            fUTFKind;

    // byte ordering
    // --------------------------------------

    bsrByteOrderingKind   fByteOrderingKind;

    // braille music file name
    // --------------------------------------

    bool                  fUseEncodingInFileName;

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
typedef SMARTP<brailleOptions> S_brailleOptions;
EXP ostream& operator<< (ostream& os, const S_brailleOptions& elt);

EXP extern S_brailleOptions gBrailleOptions;
EXP extern S_brailleOptions gBrailleOptionsUserChoices;
EXP extern S_brailleOptions gBrailleOptionsWithDetailedTrace;

//______________________________________________________________________________
void initializeBrailleOptionsHandling (
  S_oahHandler oahHandler);


}


#endif
