/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef __optionsHandling__
#define __optionsHandling__

#include "list"
#include "vector"
#include "map"
#include "set"

#include "smartpointer.h"
#include "rational.h"

#include "msrBasicTypes.h"
#include "lpsrBasicTypes.h"


using namespace std;

namespace MusicXML2 
{

//______________________________________________________________________________
// PRE-declarations for class dependencies

class msrOptionsElement;
typedef SMARTP<msrOptionsElement> S_msrOptionsElement;

class msrOptionsHandler;
typedef SMARTP<msrOptionsHandler> S_msrOptionsHandler;

//______________________________________________________________________________
class EXP msrOptionsElement : public smartable
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<msrOptionsElement> create (
      string optionsElementShortName,
      string optionsElementLongName,
      string optionsElementDescription);
     
  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrOptionsElement (
      string optionsElementShortName,
      string optionsElementLongName,
      string optionsElementDescription);

    virtual ~msrOptionsElement();

  public:

    // set and get
    // ------------------------------------------------------

    string                getOptionsElementShortName () const
                              { return fOptionsElementShortName; }

    string                getOptionsElementLongName () const
                              { return fOptionsElementLongName; }

    string                getOptionsElementDescription () const
                              { return fOptionsElementDescription; }

    // services
    // ------------------------------------------------------

    string                operator() () const
                              { return fOptionsElementDescription; }
  
    S_msrOptionsElement   fetchOptionElement ( // JMI
                            string optiontElementName);
                            
    // print
    // ------------------------------------------------------

    virtual void          printHeader (ostream& os) const;
    virtual void          underlineHeader (ostream& os) const;
    
    virtual void          print (ostream& os) const;
    
    virtual void          printHelp (ostream& os) const;
    
  protected:
     
    string                fOptionsElementShortName;
    string                fOptionsElementLongName;
    string                fOptionsElementDescription;
};
typedef SMARTP<msrOptionsElement> S_msrOptionsElement;
EXP ostream& operator<< (ostream& os, const S_msrOptionsElement& elt);

//______________________________________________________________________________
class EXP msrOptionsItem : public msrOptionsElement
{
  public:

    // data types
    // ------------------------------------------------------

    enum msrOptionsItemKind {
      kOptionsItemHasNoArgument,
      kOptionsItemHasARequiredArgument,
      kOptionsItemHasAnOptionsArgument };
          
    static string optionsItemKindAsString (
      msrOptionsItemKind optionsItemKind);

    // creation
    // ------------------------------------------------------

    static SMARTP<msrOptionsItem> create (
      string             optionsItemShortName,
      string             optionsItemLongName,
      string             optionsItemDescription,
      int                optionsItemValuesNumber);
     
  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrOptionsItem (
      string             optionsItemShortName,
      string             optionsItemLongName,
      string             optionsItemDescription,
      int                optionsItemValuesNumber);
  
    virtual ~msrOptionsItem();

  public:
  
    // set and get
    // ------------------------------------------------------

    msrOptionsItemKind    getOptionsItemKind () const
                              { return fOptionsItemKind; }
    
    int                   getOptionsItemValuesNumber () const
                              { return fOptionsItemValuesNumber; }
    
    // services
    // ------------------------------------------------------

    void                  registerOptionsItemInHandler (
                            S_msrOptionsHandler optionsHandler);

    // print
    // ------------------------------------------------------

    void                  print (ostream& os) const;

    virtual void          printOptionsValues (
                            ostream& os,
                            int      valueFieldWidth) const;

  protected:

    msrOptionsItemKind    fOptionsItemKind;

  int                     fOptionsItemValuesNumber;
};
typedef SMARTP<msrOptionsItem> S_msrOptionsItem;
EXP ostream& operator<< (ostream& os, const S_msrOptionsItem& elt);

//______________________________________________________________________________
class EXP msrOptionsBooleanItem : public msrOptionsItem
{
  public:
  
    // creation
    // ------------------------------------------------------

    static SMARTP<msrOptionsBooleanItem> create (
      string optionsItemShortName,
      string optionsItemLongName,
      string optionsItemDescription,
      string optionsBooleanItemVariableDisplayName,
      bool&  optionsBooleanItemVariable);
     
  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrOptionsBooleanItem (
      string optionsItemShortName,
      string optionsItemLongName,
      string optionsItemDescription,
      string optionsBooleanItemVariableDisplayName,
      bool&  optionsBooleanItemVariable);
      
    virtual ~msrOptionsBooleanItem();

  public:
  
    // set and get
    // ------------------------------------------------------

    string                getOptionsBooleanItemVariableDisplayName () const
                              {
                                return
                                  fOptionsBooleanItemVariableDisplayName;
                              }
                              
    void                  setBooleanItemVariableValue (
                            bool value)
                              {
                                fOptionsBooleanItemVariable = value;
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
  
    string                fOptionsBooleanItemVariableDisplayName;
    bool&                 fOptionsBooleanItemVariable;
};
typedef SMARTP<msrOptionsBooleanItem> S_msrOptionsBooleanItem;
ostream& operator<< (ostream& os, const S_msrOptionsBooleanItem& elt);

//______________________________________________________________________________
class EXP msrOptionsValuedItem : public msrOptionsItem
{
  public:

    // data types
    // ------------------------------------------------------

    enum msrOptionsValuedItemKind {
      kOptionsItemHasNoArgument,
      kOptionsItemHasARequiredArgument,
      kOptionsItemHasAnOptionsArgument };
          
    static string optionsItemKindAsString (
      msrOptionsValuedItemKind optionsItemKind);

    // creation
    // ------------------------------------------------------

    static SMARTP<msrOptionsValuedItem> create (
      string             optionsItemShortName,
      string             optionsItemLongName,
      string             optionsItemDescription,
      string             optionsValueSpecification,
      int                optionsItemValuesNumber);
     
  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrOptionsValuedItem (
      string             optionsItemShortName,
      string             optionsItemLongName,
      string             optionsItemDescription,
      string             optionsValueSpecification,
      int                optionsItemValuesNumber);
  
    virtual ~msrOptionsValuedItem();

  public:
  
    // set and get
    // ------------------------------------------------------

    msrOptionsValuedItemKind
                          getOptionsItemKind () const
                              { return fOptionsItemKind; }
    
    string                getOptionsValueSpecification () const
                              { return fOptionsValueSpecification; }
    
    int                   getOptionsItemValuesNumber () const
                              { return fOptionsItemValuesNumber; }
    
    // services
    // ------------------------------------------------------

    void                  registerOptionsItemInHandler (
                            S_msrOptionsHandler optionsHandler);

    // print
    // ------------------------------------------------------

    void                  print (ostream& os) const;

    void                  printHelp (ostream& os) const;

    virtual void          printOptionsValues (
                            ostream& os,
                            int      valueFieldWidth) const;

  protected:

    msrOptionsValuedItemKind
                          fOptionsItemKind;

    string                fOptionsValueSpecification;
    
  int                     fOptionsItemValuesNumber;
};
typedef SMARTP<msrOptionsValuedItem> S_msrOptionsValuedItem;
EXP ostream& operator<< (ostream& os, const S_msrOptionsValuedItem& elt);

//______________________________________________________________________________
class EXP msrOptionsIntegerItem : public msrOptionsValuedItem
{
  public:
  
    // creation
    // ------------------------------------------------------

    static SMARTP<msrOptionsIntegerItem> create (
      string             optionsItemShortName,
      string             optionsItemLongName,
      string             optionsItemDescription,
      string             optionsValueSpecification,
      string             optionsIntegerItemVariableDisplayName,
      int&               optionsIntegerItemVariable);
     
  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrOptionsIntegerItem (
      string             optionsItemShortName,
      string             optionsItemLongName,
      string             optionsItemDescription,
      string             optionsValueSpecification,
      string             optionsIntegerItemVariableDisplayName,
      int&               optionsIntegerItemVariable);
           
  protected:


    virtual ~msrOptionsIntegerItem();

  public:
  
    // set and get
    // ------------------------------------------------------

    string                getOptionsIntegerItemVariableDisplayName () const
                              {
                                return
                                  fOptionsIntegerItemVariableDisplayName;
                              }
                              
    void                  setIntegerItemVariableValue (
                            int value)
                              {
                                fOptionsIntegerItemVariable = value;
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
  
    string                fOptionsIntegerItemVariableDisplayName;
    int&                  fOptionsIntegerItemVariable;
};
typedef SMARTP<msrOptionsIntegerItem> S_msrOptionsIntegerItem;
ostream& operator<< (ostream& os, const S_msrOptionsIntegerItem& elt);

//______________________________________________________________________________
class EXP msrOptionsFloatItem : public msrOptionsValuedItem
{
  public:
  
    // creation
    // ------------------------------------------------------

    static SMARTP<msrOptionsFloatItem> create (
      string             optionsItemShortName,
      string             optionsItemLongName,
      string             optionsItemDescription,
      string             optionsValueSpecification,
      string             optionsFloatItemVariableDisplayName,
      float&             optionsFloatItemVariable);
     
  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrOptionsFloatItem (
      string             optionsItemShortName,
      string             optionsItemLongName,
      string             optionsItemDescription,
      string             optionsValueSpecification,
      string             optionsFloatItemVariableDisplayName,
      float&             optionsFloatItemVariable);
      
    virtual ~msrOptionsFloatItem();

  public:
  
    // set and get
    // ------------------------------------------------------

    string                getOptionsFloatItemVariableDisplayName () const
                              {
                                return
                                  fOptionsFloatItemVariableDisplayName;
                              }
                              
    void                  setFloatItemVariableValue (
                            float value)
                              {
                                fOptionsFloatItemVariable = value;
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
  
    string                fOptionsFloatItemVariableDisplayName;
    float&                fOptionsFloatItemVariable;
};
typedef SMARTP<msrOptionsFloatItem> S_msrOptionsFloatItem;
ostream& operator<< (ostream& os, const S_msrOptionsFloatItem& elt);

//______________________________________________________________________________
class EXP msrOptionsStringItem : public msrOptionsValuedItem
{
  public:
  
    // creation
    // ------------------------------------------------------

    static SMARTP<msrOptionsStringItem> create (
      string             optionsItemShortName,
      string             optionsItemLongName,
      string             optionsItemDescription,
      string             optionsValueSpecification,
      string             optionsStringItemVariableDisplayName,
      string&            optionsStringItemVariable);
     
  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrOptionsStringItem (
      string             optionsItemShortName,
      string             optionsItemLongName,
      string             optionsItemDescription,
      string             optionsValueSpecification,
      string             optionsStringItemVariableDisplayName,
      string&            optionsStringItemVariable);
      
    virtual ~msrOptionsStringItem();

  public:
  
    // set and get
    // ------------------------------------------------------

    string                getOptionsStringItemVariableDisplayName () const
                              {
                                return
                                  fOptionsStringItemVariableDisplayName;
                              }
                              
    void                  setStringItemVariableValue (
                            string value)
                              {
                                fOptionsStringItemVariable = value;
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
  
    string                fOptionsStringItemVariableDisplayName;
    string&               fOptionsStringItemVariable;
};
typedef SMARTP<msrOptionsStringItem> S_msrOptionsStringItem;
ostream& operator<< (ostream& os, const S_msrOptionsStringItem& elt);

//______________________________________________________________________________
class EXP msrOptionsRationalItem : public msrOptionsValuedItem
{
  public:
  
    // creation
    // ------------------------------------------------------

    static SMARTP<msrOptionsRationalItem> create (
      string             optionsItemShortName,
      string             optionsItemLongName,
      string             optionsItemDescription,
      string             optionsValueSpecification,
      string             optionsRationalItemVariableDisplayName,
      rational&          optionsRationalItemVariable);
     
  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrOptionsRationalItem (
      string             optionsItemShortName,
      string             optionsItemLongName,
      string             optionsItemDescription,
      string             optionsValueSpecification,
      string             optionsRationalItemVariableDisplayName,
      rational&          optionsRationalItemVariable);
      
    virtual ~msrOptionsRationalItem();

  public:
  
    // set and get
    // ------------------------------------------------------

    string                getOptionsRationalItemVariableDisplayName () const
                              {
                                return
                                  fOptionsRationalItemVariableDisplayName;
                              }
                              
    void                  setRationalItemVariableValue (
                            rational value)
                              {
                                fOptionsRationalItemVariable = value;
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
  
    string                fOptionsRationalItemVariableDisplayName;
    rational&             fOptionsRationalItemVariable;
};
typedef SMARTP<msrOptionsRationalItem> S_msrOptionsRationalItem;
ostream& operator<< (ostream& os, const S_msrOptionsRationalItem& elt);

//______________________________________________________________________________
class EXP msrOptionsNumbersSetItem : public msrOptionsValuedItem
{
  public:
  
    // creation
    // ------------------------------------------------------

    static SMARTP<msrOptionsNumbersSetItem> create (
      string             optionsItemShortName,
      string             optionsItemLongName,
      string             optionsItemDescription,
      string             optionsValueSpecification,
      string             optionsNumbersSetItemVariableDisplayName,
      set<int>&          optionsNumbersSetItemVariable);
     
  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrOptionsNumbersSetItem (
      string             optionsItemShortName,
      string             optionsItemLongName,
      string             optionsItemDescription,
      string             optionsValueSpecification,
      string             optionsNumbersSetItemVariableDisplayName,
      set<int>&          optionsNumbersSetItemVariable);
      
    virtual ~msrOptionsNumbersSetItem();

  public:
  
    // set and get
    // ------------------------------------------------------

    string                getOptionsNumbersSetItemVariableDisplayName () const
                              {
                                return
                                  fOptionsNumbersSetItemVariableDisplayName;
                              }
                              
    void                  setNumbersSetItemVariableValue (
                            set<int> value)
                              {
                                fOptionsNumbersSetItemVariable = value;
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
  
    string                fOptionsNumbersSetItemVariableDisplayName;
    set<int>&             fOptionsNumbersSetItemVariable;
};
typedef SMARTP<msrOptionsNumbersSetItem> S_msrOptionsNumbersSetItem;
ostream& operator<< (ostream& os, const S_msrOptionsNumbersSetItem& elt);

//______________________________________________________________________________
class EXP msrOptionsPitchesLanguageItem : public msrOptionsValuedItem
{
  public:
  
    // creation
    // ------------------------------------------------------

    static SMARTP<msrOptionsPitchesLanguageItem> create (
      string             optionsItemShortName,
      string             optionsItemLongName,
      string             optionsItemDescription,
      string             optionsValueSpecification,
      string             optionsPitchesLanguageItemVariableDisplayName,
      msrQuarterTonesPitchesLanguage&
                         optionsPitchesLanguageItemVariable);
     
  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrOptionsPitchesLanguageItem (
      string             optionsItemShortName,
      string             optionsItemLongName,
      string             optionsItemDescription,
      string             optionsValueSpecification,
      string             optionsPitchesLanguageItemVariableDisplayName,
      msrQuarterTonesPitchesLanguage&
                         optionsPitchesLanguageItemVariable);
      
    virtual ~msrOptionsPitchesLanguageItem();

  public:
  
    // set and get
    // ------------------------------------------------------

    string                getOptionsPitchesLanguageItemVariableDisplayName () const
                              {
                                return
                                  fOptionsPitchesLanguageItemVariableDisplayName;
                              }
                              
    void                  setPitchesLanguageItemVariableValue (
                            msrQuarterTonesPitchesLanguage value)
                              {
                                fOptionsPitchesLanguageItemVariable = value;
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
  
    string                fOptionsPitchesLanguageItemVariableDisplayName;
    msrQuarterTonesPitchesLanguage&
                          fOptionsPitchesLanguageItemVariable;
};
typedef SMARTP<msrOptionsPitchesLanguageItem> S_msrOptionsPitchesLanguageItem;
ostream& operator<< (ostream& os, const S_msrOptionsPitchesLanguageItem& elt);

//______________________________________________________________________________
class EXP msrOptionsChordsLanguageItem : public msrOptionsValuedItem
{
  public:
  
    // creation
    // ------------------------------------------------------

    static SMARTP<msrOptionsChordsLanguageItem> create (
      string             optionsItemShortName,
      string             optionsItemLongName,
      string             optionsItemDescription,
      string             optionsValueSpecification,
      string             optionsChordsLanguageItemVariableDisplayName,
      lpsrChordsLanguage&
                         optionsChordsLanguageItemVariable);
     
  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrOptionsChordsLanguageItem (
      string             optionsItemShortName,
      string             optionsItemLongName,
      string             optionsItemDescription,
      string             optionsValueSpecification,
      string             optionsChordsLanguageItemVariableDisplayName,
      lpsrChordsLanguage&
                         optionsChordsLanguageItemVariable);
      
    virtual ~msrOptionsChordsLanguageItem();

  public:
  
    // set and get
    // ------------------------------------------------------

    string                getOptionsChordsLanguageItemVariableDisplayName () const
                              {
                                return
                                  fOptionsChordsLanguageItemVariableDisplayName;
                              }
                              
    void                  setChordsLanguageItemVariableValue (
                            lpsrChordsLanguage value)
                              {
                                fOptionsChordsLanguageItemVariable = value;
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
  
    string                fOptionsChordsLanguageItemVariableDisplayName;
    lpsrChordsLanguage&   fOptionsChordsLanguageItemVariable;
};
typedef SMARTP<msrOptionsChordsLanguageItem> S_msrOptionsChordsLanguageItem;
ostream& operator<< (ostream& os, const S_msrOptionsChordsLanguageItem& elt);

//______________________________________________________________________________
class EXP msrOptionsPartRenameItem : public msrOptionsValuedItem
{
  public:
  
    // creation
    // ------------------------------------------------------

    static SMARTP<msrOptionsPartRenameItem> create (
      string             optionsItemShortName,
      string             optionsItemLongName,
      string             optionsItemDescription,
      string             optionsValueSpecification,
      string             optionsPartRenameItemVariableDisplayName,
      map<string, string>&
                         optionsPartRenameItemVariable);
     
  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrOptionsPartRenameItem (
      string             optionsItemShortName,
      string             optionsItemLongName,
      string             optionsItemDescription,
      string             optionsValueSpecification,
      string             optionsPartRenameItemVariableDisplayName,
      map<string, string>&
                         optionsPartRenameItemVariable);
      
    virtual ~msrOptionsPartRenameItem();

  public:
  
    // set and get
    // ------------------------------------------------------

    string                getOptionsPartRenameItemVariableDisplayName () const
                              {
                                return
                                  fOptionsPartRenameItemVariableDisplayName;
                              }
                              
    void                  setPartRenameItemVariableValue (
                            map<string, string> value)
                              {
                                fOptionsPartRenameItemVariable = value;
                              }

    const map<string, string>&
                          getOptionsPartRenameItemVariable ()
                              {
                                return
                                  fOptionsPartRenameItemVariable;
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
  
    string                fOptionsPartRenameItemVariableDisplayName;
    map<string, string>&  fOptionsPartRenameItemVariable;
};
typedef SMARTP<msrOptionsPartRenameItem> S_msrOptionsPartRenameItem;
ostream& operator<< (ostream& os, const S_msrOptionsPartRenameItem& elt);

//______________________________________________________________________________
class EXP msrOptionsMidiTempoItem : public msrOptionsValuedItem
{
  public:
  
    // creation
    // ------------------------------------------------------

    static SMARTP<msrOptionsMidiTempoItem> create (
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

    msrOptionsMidiTempoItem (
      string             optionsItemShortName,
      string             optionsItemLongName,
      string             optionsItemDescription,
      string             optionsValueSpecification,
      string             optionsMidiTempoItemVariableDisplayName,
      pair<string, int>&
                         optionsMidiTempoItemVariable);
      
    virtual ~msrOptionsMidiTempoItem();

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
  
    string                fOptionsMidiTempoItemVariableDisplayName;
    pair<string, int>&    fOptionsMidiTempoItemVariable;
};
typedef SMARTP<msrOptionsMidiTempoItem> S_msrOptionsMidiTempoItem;
ostream& operator<< (ostream& os, const S_msrOptionsMidiTempoItem& elt);

//_______________________________________________________________________________
class EXP msrOptionsSubGroup : public msrOptionsElement
{
  public:
  
    // creation
    // ------------------------------------------------------

    static SMARTP<msrOptionsSubGroup> create (
      string optionsSubGroupShortName,
      string optionsSubGroupLongName,
      string optionsSubGroupDescription);
     
  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrOptionsSubGroup (
      string optionsSubGroupShortName,
      string optionsSubGroupLongName,
      string optionsSubGroupDescription);
    
    virtual ~msrOptionsSubGroup();

  public:
  
    // set and get
    // ------------------------------------------------------

    // services
    // ------------------------------------------------------

    void                  registerOptionsSubGroupInHandler (
                            S_msrOptionsHandler optionsHandler);

    void                  appendOptionsItem (
                            S_msrOptionsItem optionsItem)
                              {
                                fOptionsSubGroupItemsList.push_back (
                                  optionsItem);
                              }

    S_msrOptionsElement   fetchOptionElement (
                            string optiontElementName);
                            
    // print
    // ------------------------------------------------------

    void                  print (ostream& os) const;

    void                  printHelp (ostream& os) const;

    void                  printHelpSummary (ostream& os) const;

    void                  printOptionsValues (
                            ostream& os,
                            int      valueFieldWidth) const;

  private:
  
    list<S_msrOptionsItem>
                          fOptionsSubGroupItemsList;
};
typedef SMARTP<msrOptionsSubGroup> S_msrOptionsSubGroup;
ostream& operator<< (ostream& os, const S_msrOptionsSubGroup& elt);

//_______________________________________________________________________________
class EXP msrOptionsGroup : public msrOptionsElement
{
  public:
  
    // creation
    // ------------------------------------------------------

    static SMARTP<msrOptionsGroup> create (
      string optionGroupShortName,
      string optionGroupLongName,
      string optionGroupDescription);
     
  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrOptionsGroup (
      string optionGroupShortName,
      string optionGroupLongName,
      string optionGroupDescription);
      
    virtual ~msrOptionsGroup();

  public:
  
    // set and get
    // ------------------------------------------------------

    // services
    // ------------------------------------------------------

    void                  registerOptionsGroupInHandler (
                            S_msrOptionsHandler optionsHandler);

    void                  appendOptionsSubGroup (
                            S_msrOptionsSubGroup optionsSubGroup);
        
    S_msrOptionsElement   fetchOptionElement (
                            string optiontElementName);
                            
    // print
    // ------------------------------------------------------

    void                  print (ostream& os) const;

    void                  printHelp (ostream& os) const;

    void                  printHelpSummary (ostream& os) const;
    
    void                  printOptionsValues (
                            ostream& os,
                            int      valueFieldWidth) const;

  private:

    list<S_msrOptionsSubGroup>
                          fOptionsGroupSubGroupsList;
};
typedef SMARTP<msrOptionsGroup> S_msrOptionsGroup;
ostream& operator<< (ostream& os, const S_msrOptionsGroup& elt);

//_______________________________________________________________________________
class EXP msrOptionsHandler : public msrOptionsElement
{
  public:
  
    // creation
    // ------------------------------------------------------
/* JMI
    static SMARTP<msrOptionsHandler> create (
      string optionHandlerShortName,
      string optionHandlerLongName,
      string optionHandlerDescription);
     */
  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrOptionsHandler (
      string optionsHandlerHelpHeader,
      string optionHandlerShortName,
      string optionHandlerLongName,
      string optionHandlerDescription);
      
    virtual ~msrOptionsHandler();

  private:

    // initialization
    // ------------------------------------------------------

    virtual void          initializeOptionsHandler () = 0;

  public:

    // set and get
    // ------------------------------------------------------

    string                getOptionsHandlerHelpHeader () const
                              { return fOptionsHandlerHelpHeader; }
                              
    string                getProgramName () const
                              { return fProgramName; }
                              
    const list<S_msrOptionsElement>&
                          getCommandOptionsElements () const
                              { return fCommandOptionsElements; }
                              
    string                getCommandLineWithShortOptions () const
                              { return fCommandLineWithShortOptions; }
                              
    string                getCommandLineWithLongOptions () const
                              { return fCommandLineWithLongOptions; }
                              
  public:
  
    // services
    // ------------------------------------------------------

    void                  registerOptionsHandlerInSelf ();

    void                  registerOptionsElementNamesInHandler (
                            S_msrOptionsElement optionsElement);

    void                  appendOptionsGroup (
                            S_msrOptionsGroup optionsGroup);

    S_msrOptionsElement   fetchOptionElement (
                            string optiontElementName);
                            
    const vector<string>  analyzeOptions (
                            int   argc,
                            char* argv[]);

    void                  handleOptionsItemName (
                            string optionsItemName);

    void                  handleOptionsItemValueOrArgument (
                            string theString);

    // print
    // ------------------------------------------------------

    void                  print (ostream& os) const;

    void                  printHelp (ostream& os) const;

    void                  printHelpSummary (ostream& os) const;
    
    void                  printOptionsValues (
                            ostream& os) const;

  protected:

    string                fOptionsHandlerHelpHeader;

    S_msrOptionsItem      fPendingOptionsItem;
    int                   fExpectedValuesNumber;
    vector<string>        fArgumentsVector;
    
    list<S_msrOptionsGroup>
                          fOptionsHandlerOptionsGroupsList;

    map<string, S_msrOptionsElement>
                          fOptionsElementsMap;

    string                fProgramName;
    list<S_msrOptionsElement>
                          fCommandOptionsElements;

    string                fCommandLineWithShortOptions;
    string                fCommandLineWithLongOptions;
    int                   fMaximumDisplayNameWidth;
};
typedef SMARTP<msrOptionsHandler> S_msrOptionsHandler;
ostream& operator<< (ostream& os, const S_msrOptionsHandler& elt);


/*! @} */

}


#endif
