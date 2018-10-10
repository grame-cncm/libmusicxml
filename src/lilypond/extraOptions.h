/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef ___extraOptions___
#define ___extraOptions___

#include "optionsBasicTypes.h"

#include "exports.h"


namespace MusicXML2 
{

//______________________________________________________________________________
class optionsShowAllChordsStructuresItem : public optionsItem
{
  public:
  
    // creation
    // ------------------------------------------------------

    static SMARTP<optionsShowAllChordsStructuresItem> create (
      string optionsItemShortName,
      string optionsItemLongName,
      string optionsItemDescription);
     
  protected:

    // constructors/destructor
    // ------------------------------------------------------

    optionsShowAllChordsStructuresItem (
      string optionsItemShortName,
      string optionsItemLongName,
      string optionsItemDescription);
      
    virtual ~optionsShowAllChordsStructuresItem ();

  public:
  
    // set and get
    // ------------------------------------------------------

    // services
    // ------------------------------------------------------

    // print
    // ------------------------------------------------------

    void                  print (ostream& os) const;

    void                  printAllChordsStructures (ostream& os) const;

    void                  printOptionsValues (
                            ostream& os,
                            int      valueFieldWidth) const;
  
  private:
  
    // fields
    // ------------------------------------------------------
};
typedef SMARTP<optionsShowAllChordsStructuresItem> S_optionsShowAllChordsStructuresItem;
ostream& operator<< (ostream& os, const S_optionsShowAllChordsStructuresItem& elt);

//______________________________________________________________________________
class optionsShowAllChordsContentsItem : public optionsValuedItem
{
  public:
  
    // creation
    // ------------------------------------------------------

    static SMARTP<optionsShowAllChordsContentsItem> create (
      string  optionsItemShortName,
      string  optionsItemLongName,
      string  optionsItemDescription,
      string  optionsValueSpecification,
      string  optionsStringItemVariableDisplayName,
      string& optionsStringItemVariable);
     
  protected:

    // constructors/destructor
    // ------------------------------------------------------

    optionsShowAllChordsContentsItem (
      string  optionsItemShortName,
      string  optionsItemLongName,
      string  optionsItemDescription,
      string  optionsValueSpecification,
      string  optionsStringItemVariableDisplayName,
      string& optionsStringItemVariable);
      
    virtual ~optionsShowAllChordsContentsItem ();

  public:
  
    // set and get
    // ------------------------------------------------------

    string                getOptionsShowAllChordsContentsItemVariableDisplayName () const
                              {
                                return
                                  fOptionsShowAllChordsContentsItemVariableDisplayName;
                              }
                              
    void                  setShowAllChordsContentsItemVariableValue (
                            string value)
                              {
                                fOptionsShowAllChordsContentsItemVariable = value;
                              }

    // services
    // ------------------------------------------------------

    // print
    // ------------------------------------------------------

    void                  print (ostream& os) const;

    void                  printAllChordsContents (
                            ostream&              os,
                            msrSemiTonesPitchKind semiTonesPitchKind) const;

    void                  printOptionsValues (
                            ostream& os,
                            int      valueFieldWidth) const;
  
  private:
  
    // fields
    // ------------------------------------------------------

    string                fOptionsShowAllChordsContentsItemVariableDisplayName;
    string&               fOptionsShowAllChordsContentsItemVariable;
};
typedef SMARTP<optionsShowAllChordsContentsItem> S_optionsShowAllChordsContentsItem;
ostream& operator<< (ostream& os, const S_optionsShowAllChordsContentsItem& elt);

//______________________________________________________________________________
class optionsShowChordDetailsItem : public optionsValuedItem
{
  public:
  
    // creation
    // ------------------------------------------------------

    static SMARTP<optionsShowChordDetailsItem> create (
      string  optionsItemShortName,
      string  optionsItemLongName,
      string  optionsItemDescription,
      string  optionsValueSpecification,
      string  optionsStringItemVariableDisplayName,
      string& optionsStringItemVariable);
     
  protected:

    // constructors/destructor
    // ------------------------------------------------------

    optionsShowChordDetailsItem (
      string  optionsItemShortName,
      string  optionsItemLongName,
      string  optionsItemDescription,
      string  optionsValueSpecification,
      string  optionsStringItemVariableDisplayName,
      string& optionsStringItemVariable);
      
    virtual ~optionsShowChordDetailsItem ();

  public:
  
    // set and get
    // ------------------------------------------------------

    string                getOptionsShowChordDetailsItemVariableDisplayName () const
                              {
                                return
                                  fOptionsShowChordDetailsItemVariableDisplayName;
                              }
                              
    void                  setShowChordDetailsItemVariableValue (
                            string value)
                              {
                                fOptionsShowChordDetailsItemVariable = value;
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

    string                fOptionsShowChordDetailsItemVariableDisplayName;
    string&               fOptionsShowChordDetailsItemVariable;
};
typedef SMARTP<optionsShowChordDetailsItem> S_optionsShowChordDetailsItem;
ostream& operator<< (ostream& os, const S_optionsShowChordDetailsItem& elt);

//______________________________________________________________________________
class optionsShowChordAnalysisItem : public optionsValuedItem
{
  public:
  
    // creation
    // ------------------------------------------------------

    static SMARTP<optionsShowChordAnalysisItem> create (
      string  optionsItemShortName,
      string  optionsItemLongName,
      string  optionsItemDescription,
      string  optionsValueSpecification,
      string  optionsStringItemVariableDisplayName,
      string& optionsStringItemVariable);
     
  protected:

    // constructors/destructor
    // ------------------------------------------------------

    optionsShowChordAnalysisItem (
      string  optionsItemShortName,
      string  optionsItemLongName,
      string  optionsItemDescription,
      string  optionsValueSpecification,
      string  optionsStringItemVariableDisplayName,
      string& optionsStringItemVariable);
      
    virtual ~optionsShowChordAnalysisItem ();

  public:
  
    // set and get
    // ------------------------------------------------------

    string                getOptionsShowChordAnalysisItemVariableDisplayName () const
                              {
                                return
                                  fOptionsShowChordAnalysisItemVariableDisplayName;
                              }
                              
    void                  setShowChordAnalysisItemVariableValue (
                            string value)
                              {
                                fOptionsShowChordAnalysisItemVariable = value;
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

    string                fOptionsShowChordAnalysisItemVariableDisplayName;
    string&               fOptionsShowChordAnalysisItemVariable;
};
typedef SMARTP<optionsShowChordAnalysisItem> S_optionsShowChordAnalysisItem;
ostream& operator<< (ostream& os, const S_optionsShowChordAnalysisItem& elt);

//______________________________________________________________________________
class extraOptions : public optionsGroup
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<extraOptions> create (
      S_optionsHandler optionsHandler);
    
    SMARTP<extraOptions>        createCloneWithDetailedTrace ();
    
  private:

    // initialisation
    // ------------------------------------------------------

    void                  initializeExtraOptions (
                            bool boolOptionsInitialValue);
        
  protected:
  
    // constructors/destructor
    // ------------------------------------------------------

    extraOptions (
      S_optionsHandler optionsHandler);
  
    virtual ~extraOptions ();
 
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

    // print
    // ------------------------------------------------------

    void                  printExtraOptionsHelp ();

    void                  printExtraOptionsValues (int fieldWidth);
    
  public:

    // fields
    // ------------------------------------------------------

    string                fChordsRootAsString;
};
typedef SMARTP<extraOptions> S_extraOptions;
EXP ostream& operator<< (ostream& os, const S_extraOptions& elt);

EXP extern S_extraOptions gExtraOptions;
EXP extern S_extraOptions gExtraOptionsUserChoices;
EXP extern S_extraOptions gExtraOptionsWithDetailedTrace;

//______________________________________________________________________________
void initializeExtraOptionsHandling (
  S_optionsHandler optionsHandler);


}


#endif
