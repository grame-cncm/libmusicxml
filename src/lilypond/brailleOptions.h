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

#include "optionsBasicTypes.h"

#include "bsrBrailleCodeGenerators.h"


namespace MusicXML2 
{

//______________________________________________________________________________
class optionsUTFKindItem : public optionsValuedItem
{
  public:
  
    // creation
    // ------------------------------------------------------

    static SMARTP<optionsUTFKindItem> create (
      string     optionsItemShortName,
      string     optionsItemLongName,
      string     optionsItemDescription,
      string     optionsValueSpecification,
      string     optionsUTFKindItemVariableDisplayName,
      bsrUTFKind optionsUTFKindItemVariable);
     
  protected:

    // constructors/destructor
    // ------------------------------------------------------

    optionsUTFKindItem (
      string     optionsItemShortName,
      string     optionsItemLongName,
      string     optionsItemDescription,
      string     optionsValueSpecification,
      string     optionsUTFKindItemVariableDisplayName,
      bsrUTFKind optionsUTFKindItemVariable);
      
    virtual ~optionsUTFKindItem ();

  public:
  
    // set and get
    // ------------------------------------------------------

    string                getOptionsUTFKindItemVariableDisplayName () const
                              {
                                return
                                  fOptionsUTFKindItemVariableDisplayName;
                              }
                              
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

    string                fOptionsUTFKindItemVariableDisplayName;
    bsrUTFKind            fOptionsUTFKindItemVariable;
};
typedef SMARTP<optionsUTFKindItem> S_optionsUTFKindItem;
ostream& operator<< (ostream& os, const S_optionsUTFKindItem& elt);

//______________________________________________________________________________
class optionsByteOrderingKindItem : public optionsValuedItem
{
  public:
  
    // creation
    // ------------------------------------------------------

    static SMARTP<optionsByteOrderingKindItem> create (
      string              optionsItemShortName,
      string              optionsItemLongName,
      string              optionsItemDescription,
      string              optionsValueSpecification,
      string              optionsByteOrderingKindItemVariableDisplayName,
      bsrByteOrderingKind optionsByteOrderingKindItemVariable);
     
  protected:

    // constructors/destructor
    // ------------------------------------------------------

    optionsByteOrderingKindItem (
      string              optionsItemShortName,
      string              optionsItemLongName,
      string              optionsItemDescription,
      string              optionsValueSpecification,
      string              optionsByteOrderingKindItemVariableDisplayName,
      bsrByteOrderingKind optionsByteOrderingKindItemVariable);
      
    virtual ~optionsByteOrderingKindItem ();

  public:
  
    // set and get
    // ------------------------------------------------------

    string                getOptionsByteOrderingKindItemVariableDisplayName () const
                              {
                                return
                                  fOptionsByteOrderingKindItemVariableDisplayName;
                              }
                              
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

    string                fOptionsByteOrderingKindItemVariableDisplayName;
    bsrByteOrderingKind   fOptionsByteOrderingKindItemVariable;
};
typedef SMARTP<optionsByteOrderingKindItem> S_optionsByteOrderingKindItem;
ostream& operator<< (ostream& os, const S_optionsByteOrderingKindItem& elt);

//______________________________________________________________________________
class optionsFacSimileKindItem : public optionsValuedItem
{
  public:
  
    // creation
    // ------------------------------------------------------

    static SMARTP<optionsFacSimileKindItem> create (
      string           optionsItemShortName,
      string           optionsItemLongName,
      string           optionsItemDescription,
      string           optionsValueSpecification,
      string           optionsFacSimileKindItemVariableDisplayName,
      bsrFacSimileKind optionsFacSimileKindItemVariable);
     
  protected:

    // constructors/destructor
    // ------------------------------------------------------

    optionsFacSimileKindItem (
      string           optionsItemShortName,
      string           optionsItemLongName,
      string           optionsItemDescription,
      string           optionsValueSpecification,
      string           optionsFacSimileKindItemVariableDisplayName,
      bsrFacSimileKind optionsFacSimileKindItemVariable);
      
    virtual ~optionsFacSimileKindItem ();

  public:
  
    // set and get
    // ------------------------------------------------------

    string                getOptionsFacSimileKindItemVariableDisplayName () const
                              {
                                return
                                  fOptionsFacSimileKindItemVariableDisplayName;
                              }
                              
    void                  setOptionsFacSimileKindItemVariable (
                            bsrFacSimileKind value)
                              {
                                fOptionsFacSimileKindItemVariable = value;
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

    string                fOptionsFacSimileKindItemVariableDisplayName;
    bsrFacSimileKind      fOptionsFacSimileKindItemVariable;
};
typedef SMARTP<optionsFacSimileKindItem> S_optionsFacSimileKindItem;
ostream& operator<< (ostream& os, const S_optionsFacSimileKindItem& elt);

//______________________________________________________________________________
class brailleOptions : public optionsGroup
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<brailleOptions> create (
      S_optionsHandler optionsHandler);
    
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
      S_optionsHandler optionsHandler);
    
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

    // services
    // ------------------------------------------------------

    virtual S_optionsItem handleOptionsItem (
                            ostream&      os,
                            S_optionsItem item);
                            
    virtual void          handleOptionsItemValue (
                            ostream&      os,
                            S_optionsItem item,
                            string        theString);

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

    bool                  fNoBrailleLyrics;

    bool                  fBrailleCompileDate;

    bsrFacSimileKind      fFacSimileKind;
};
typedef SMARTP<brailleOptions> S_brailleOptions;
EXP ostream& operator<< (ostream& os, const S_brailleOptions& elt);

EXP extern S_brailleOptions gBrailleOptions;
EXP extern S_brailleOptions gBrailleOptionsUserChoices;
EXP extern S_brailleOptions gBrailleOptionsWithDetailedTrace;

//______________________________________________________________________________
void initializeBrailleOptionsHandling (
  S_optionsHandler optionsHandler);


}


#endif
