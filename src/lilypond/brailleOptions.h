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


namespace MusicXML2 
{

/*
//______________________________________________________________________________
class optionsAccidentalStyleItem : public optionsValuedItem
{
  public:
  
    // creation
    // ------------------------------------------------------

    static SMARTP<optionsAccidentalStyleItem> create (
      string             optionsItemShortName,
      string             optionsItemLongName,
      string             optionsItemDescription,
      string             optionsValueSpecification,
      string             optionsAccidentalStyleItemVariableDisplayName,
      lpsrAccidentalStyleKind&
                         optionsAccidentalStyleKindItemVariable);
     
  protected:

    // constructors/destructor
    // ------------------------------------------------------

    optionsAccidentalStyleItem (
      string             optionsItemShortName,
      string             optionsItemLongName,
      string             optionsItemDescription,
      string             optionsValueSpecification,
      string             optionsAccidentalStyleItemVariableDisplayName,
      lpsrAccidentalStyleKind&
                         optionsAccidentalStyleKindItemVariable);
      
    virtual ~optionsAccidentalStyleItem ();

  public:
  
    // set and get
    // ------------------------------------------------------

    string                getOptionsAccidentalStyleKindItemVariableDisplayName () const
                              {
                                return
                                  fOptionsAccidentalStyleKindItemVariableDisplayName;
                              }
                              
    void                  setAccidentalStyleKindItemVariableValue (
                            lpsrAccidentalStyleKind value)
                              {
                                fOptionsAccidentalStyleKindItemVariable = value;
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

    string                fOptionsAccidentalStyleKindItemVariableDisplayName;
    lpsrAccidentalStyleKind&
                          fOptionsAccidentalStyleKindItemVariable;
};
typedef SMARTP<optionsAccidentalStyleItem> S_optionsAccidentalStyleItem;
ostream& operator<< (ostream& os, const S_optionsAccidentalStyleItem& elt);

//______________________________________________________________________________
class optionsMidiTempoItem : public optionsValuedItem
{
  public:
  
    // creation
    // ------------------------------------------------------

    static SMARTP<optionsMidiTempoItem> create (
      string             optionsItemShortName,
      string             optionsItemLongName,
      string             optionsItemDescription,
      string             optionsValueSpecification,
      string             optionsMidiTempoItemVariableDisplayName,
      pair<string, int>&
                         optionsMidiTempoItemVariable);
     
  protected:

    // constructors/destructor
    // ------------------------------------------------------

    optionsMidiTempoItem (
      string             optionsItemShortName,
      string             optionsItemLongName,
      string             optionsItemDescription,
      string             optionsValueSpecification,
      string             optionsMidiTempoItemVariableDisplayName,
      pair<string, int>&
                         optionsMidiTempoItemVariable);
      
    virtual ~optionsMidiTempoItem ();

  public:
  
    // set and get
    // ------------------------------------------------------

    string                getOptionsMidiTempoItemVariableDisplayName () const
                              {
                                return
                                  fOptionsMidiTempoItemVariableDisplayName;
                              }
                              
    void                  setMidiTempoItemVariableValue (
                            pair<string, int> value)
                              {
                                fOptionsMidiTempoItemVariable = value;
                              }

    const pair<string, int>&
                          getOptionsMidiTempoItemVariable ()
                              {
                                return
                                  fOptionsMidiTempoItemVariable;
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

    string                fOptionsMidiTempoItemVariableDisplayName;
    pair<string, int>&    fOptionsMidiTempoItemVariable;
};
typedef SMARTP<optionsMidiTempoItem> S_optionsMidiTempoItem;
ostream& operator<< (ostream& os, const S_optionsMidiTempoItem& elt);
*/

//______________________________________________________________________________
class brailleOptions : public optionsGroup
{
  public:

    // data types
    // ------------------------------------------------------

/*
    enum {
      workNumber
workTitle
movementNumber,
movementTitle,
scoreInstrument
miscellaneousField

partGroupName
partGroupNameDisplayText
partGroupAbbrevation
partGroupInstrumentName

partID
partMsrName
partName
partAbbrevation
partInstrumentName
partInstrumentAbbreviation

staffInstrumentName
staffInstrumentAbbreviation
};
*/

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

 // JMI   bool                  setAccidentalStyle (
  //                          lpsrAccidentalStyle accidentalStyle);    
 
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

    // identification
    // --------------------------------------

    // MusicXML informations
    string                fRights;
    string                fComposer;
    string                fArranger;
    string                fPoet;
    string                fLyricist;
    string                fSoftware;
      
    // Braille informations
    string                fDedication;
    string                fPiece;
    string                fOpus;
    string                fTitle;
    string                fSubTitle;
    string                fSubSubTitle;
    string                fInstrument;
    string                fMeter;
    string                fTagline;
    string                fCopyright;

    
    // names
    // --------------------------------------
/*
workNumber
workTitle
movementNumber,
movementTitle,
scoreInstrument
miscellaneousField

partGroupName
partGroupNameDisplayText
partGroupAbbrevation
partGroupInstrumentName

partID
partMsrName
partName
partAbbrevation
partInstrumentName
partInstrumentAbbreviation

staffInstrumentName
staffInstrumentAbbreviation



pickDedication
pickTitle
pickSubTitle
pickSubSubTitle

pickInstrument

pickTagline
pickCopyright

*/
    
    // notes
    // --------------------------------------
    
    bool                  fAbsoluteOctaves;
    
    bool                  fAllDurations;
        
    bool                  fRomanStringNumbers;
    bool                  fAvoidOpenStrings;
    
    bool                  fCompressMultiMeasureRests;

    bool                  fNoteInputLineNumbers;


    // bars
    // --------------------------------------
    
    bool                  fShowAllBarNumbers;

    
    // line breaks
    // --------------------------------------
    
    bool                  fIgnoreLineBreaks;
    
    bool                  fBreakLinesAtIncompleteRightMeasures;
    

    // page breaks
    // --------------------------------------
    
    bool                  fIgnorePageBreaks;

    
    // code generation
    // --------------------------------------

    bool                  fXml2brlInfos;
    
    bool                  fNoBrailleCode;

    bool                  fNoBrailleLyrics;

    bool                  fBrailleCompileDate;

  private: // JMI

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
