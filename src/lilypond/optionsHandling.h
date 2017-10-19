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

#include "utilities.h"

#include "msrBasicTypes.h"
#include "lpsrBasicTypes.h"


using namespace std;

namespace MusicXML2 
{

//______________________________________________________________________________
// PRE-declarations for class dependencies

class msrOptionsElement;
typedef SMARTP<msrOptionsElement> S_msrOptionsElement;

class msrOptionsSubGroup;
typedef SMARTP<msrOptionsSubGroup> S_msrOptionsSubGroup;

class msrOptionsGroup;
typedef SMARTP<msrOptionsGroup> S_msrOptionsGroup;

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

    void                  setOptionsElementIsHidden ()
                              { fOptionsElementIsHidden = true; }
    
    bool                  getOptionsElementIsHidden () const
                              { return fOptionsElementIsHidden; }

    // services
    // ------------------------------------------------------

    string                optionsElementNames () const;
    string                optionsElementNamesInColumns (
                            int subGroupsShortNameFieldWidth) const;
    
    string                optionsElementNamesBetweenParentheses () const;
    string                optionsElementNamesInColumnsBetweenParentheses (
                            int subGroupsShortNameFieldWidth) const;

    string                operator() () const
                              { return fOptionsElementDescription; }
  
    S_msrOptionsElement   fetchOptionElement ( // JMI
                            string optiontElementName);
                            
    // print
    // ------------------------------------------------------
    
    virtual void          printHeader (ostream& os) const;
    
    virtual void          printElementEssentials (
                            ostream& os,
                            int      fieldWidth) const;
    
    virtual void          print (ostream& os) const;
    
    virtual void          printHelp (ostream& os) const;
    
  protected:
     
    // fields
    // ------------------------------------------------------

    string                fOptionsElementShortName;
    string                fOptionsElementLongName;
    string                fOptionsElementDescription;

    bool                  fOptionsElementIsHidden;
};
typedef SMARTP<msrOptionsElement> S_msrOptionsElement;
EXP ostream& operator<< (ostream& os, const S_msrOptionsElement& elt);

//______________________________________________________________________________
class EXP msrOptionsItem : public msrOptionsElement
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<msrOptionsItem> create (
      string             optionsItemShortName,
      string             optionsItemLongName,
      string             optionsItemDescription);
     
  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrOptionsItem (
      string             optionsItemShortName,
      string             optionsItemLongName,
      string             optionsItemDescription);
  
    virtual ~msrOptionsItem();

  public:
  
    // set and get
    // ------------------------------------------------------

    void                  setOptionsSubGroupUplink (
                            S_msrOptionsSubGroup optionsSubGroup)
                              {
                                fOptionsSubGroupUplink =
                                  optionsSubGroup;
                              }

    S_msrOptionsSubGroup  getOptionsSubGroupUplink () const
                              { return fOptionsSubGroupUplink; }
        
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

    // fields
    // ------------------------------------------------------

    S_msrOptionsSubGroup  fOptionsSubGroupUplink;
};
typedef SMARTP<msrOptionsItem> S_msrOptionsItem;
EXP ostream& operator<< (ostream& os, const S_msrOptionsItem& elt);

//______________________________________________________________________________
class EXP msrOptionsVersionItem : public msrOptionsItem
{
  public:
  
    // creation
    // ------------------------------------------------------

    static SMARTP<msrOptionsVersionItem> create (
      string optionsItemShortName,
      string optionsItemLongName,
      string optionsItemDescription);
     
  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrOptionsVersionItem (
      string optionsItemShortName,
      string optionsItemLongName,
      string optionsItemDescription);
      
    virtual ~msrOptionsVersionItem();

  public:
  
    // set and get
    // ------------------------------------------------------

    // services
    // ------------------------------------------------------

    // print
    // ------------------------------------------------------

    void                  print (ostream& os) const;

    void                  printVersion (ostream& os) const;

  private:
  
    // fields
    // ------------------------------------------------------
};
typedef SMARTP<msrOptionsVersionItem> S_msrOptionsVersionItem;
ostream& operator<< (ostream& os, const S_msrOptionsVersionItem& elt);

//______________________________________________________________________________
class EXP msrOptionsHelpSummaryItem : public msrOptionsItem
{
  public:
  
    // creation
    // ------------------------------------------------------

    static SMARTP<msrOptionsHelpSummaryItem> create (
      string optionsItemShortName,
      string optionsItemLongName,
      string optionsItemDescription);
     
  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrOptionsHelpSummaryItem (
      string optionsItemShortName,
      string optionsItemLongName,
      string optionsItemDescription);
      
    virtual ~msrOptionsHelpSummaryItem();

  public:
  
    // set and get
    // ------------------------------------------------------

    // services
    // ------------------------------------------------------

    // print
    // ------------------------------------------------------

    void                  print (ostream& os) const;

    void                  printHelpSummary (ostream& os) const;

  private:
  
    // fields
    // ------------------------------------------------------
};
typedef SMARTP<msrOptionsHelpSummaryItem> S_msrOptionsHelpSummaryItem;
ostream& operator<< (ostream& os, const S_msrOptionsHelpSummaryItem& elt);

//______________________________________________________________________________
class EXP msrOptionsCombinedItemsItem : public msrOptionsItem
{
  public:
  
    // creation
    // ------------------------------------------------------

    static SMARTP<msrOptionsCombinedItemsItem> create (
      string             optionsItemShortName,
      string             optionsItemLongName,
      string             optionsItemDescription);
     
  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrOptionsCombinedItemsItem (
      string             optionsItemShortName,
      string             optionsItemLongName,
      string             optionsItemDescription);
      
    virtual ~msrOptionsCombinedItemsItem();

  public:
  
    // set and get
    // ------------------------------------------------------

    const list<S_msrOptionsItem>&
                          getOptionsCombinedItemsList ()
                              {
                                return
                                  fOptionsCombinedItemsList;
                              }
                              
/*
    void                  setCombinedItemsItemVariableValue (
                            set<int> value)
                              {
                                fOptionsCombinedItemsItemVariable = value;
                              }
*/
    // services
    // ------------------------------------------------------

    void                  appendOptionsItem (
                            S_msrOptionsItem optionsItem);

    void                  appendOptionsItem (
                            string optionsItemName);

    // print
    // ------------------------------------------------------

    void                  print (ostream& os) const;

    void                  printOptionsValues (
                            ostream& os,
                            int      valueFieldWidth) const;

  private:
  
    // fields
    // ------------------------------------------------------

//    string                fOptionsCombinedItemsItemVariableDisplayName;
    list<S_msrOptionsItem>
                          fOptionsCombinedItemsList;
};
typedef SMARTP<msrOptionsCombinedItemsItem> S_msrOptionsCombinedItemsItem;
ostream& operator<< (ostream& os, const S_msrOptionsCombinedItemsItem& elt);

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
  
    // fields
    // ------------------------------------------------------

    string                fOptionsBooleanItemVariableDisplayName;
    bool&                 fOptionsBooleanItemVariable;
};
typedef SMARTP<msrOptionsBooleanItem> S_msrOptionsBooleanItem;
ostream& operator<< (ostream& os, const S_msrOptionsBooleanItem& elt);

//______________________________________________________________________________
class EXP msrOptionsTwoBooleansItem : public msrOptionsItem
{
  public:
  
    // creation
    // ------------------------------------------------------

    static SMARTP<msrOptionsTwoBooleansItem> create (
      string optionsItemShortName,
      string optionsItemLongName,
      string optionsItemDescription,
      string optionsTwoBooleansItemVariableDisplayName,
      bool&  optionsTwoBooleansItemVariable,
      bool&  optionsTwoBooleansItemSecondaryVariable);
     
  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrOptionsTwoBooleansItem (
      string optionsItemShortName,
      string optionsItemLongName,
      string optionsItemDescription,
      string optionsTwoBooleansItemVariableDisplayName,
      bool&  optionsTwoBooleansItemVariable,
      bool&  optionsTwoBooleansItemSecondaryVariable);
      
    virtual ~msrOptionsTwoBooleansItem();

  public:
  
    // set and get
    // ------------------------------------------------------

    string                getOptionsTwoBooleansItemVariableDisplayName () const
                              {
                                return
                                  fOptionsTwoBooleansItemVariableDisplayName;
                              }
                              
    void                  setTwoBooleansItemVariableValue (
                            bool value)
                              {
                                fOptionsTwoBooleansItemVariable =
                                  value;
                                fOptionsTwoBooleansItemSecondaryVariable =
                                  value;
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

    string                fOptionsTwoBooleansItemVariableDisplayName;
    bool&                 fOptionsTwoBooleansItemVariable;
    
    bool&                 fOptionsTwoBooleansItemSecondaryVariable;
};
typedef SMARTP<msrOptionsTwoBooleansItem> S_msrOptionsTwoBooleansItem;
ostream& operator<< (ostream& os, const S_msrOptionsTwoBooleansItem& elt);

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
      string             optionsValueSpecification);
     
  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrOptionsValuedItem (
      string             optionsItemShortName,
      string             optionsItemLongName,
      string             optionsItemDescription,
      string             optionsValueSpecification);
  
    virtual ~msrOptionsValuedItem();

  public:
  
    // set and get
    // ------------------------------------------------------

    msrOptionsValuedItemKind
                          getOptionsItemKind () const
                              { return fOptionsItemKind; }
    
    string                getOptionsValueSpecification () const
                              { return fOptionsValueSpecification; }
    
    // services
    // ------------------------------------------------------

    void                  registerOptionsItemInHandler (
                            S_msrOptionsHandler optionsHandler);

    // print
    // ------------------------------------------------------

    virtual void          printValuedItemEssentials (
                            ostream& os,
                            int      fieldWidth) const;

    void                  print (ostream& os) const;

    void                  printHelp (ostream& os) const;

    virtual void          printOptionsValues (
                            ostream& os,
                            int      valueFieldWidth) const;

  protected:

    // fields
    // ------------------------------------------------------

    msrOptionsValuedItemKind
                          fOptionsItemKind;

    string                fOptionsValueSpecification;
};
typedef SMARTP<msrOptionsValuedItem> S_msrOptionsValuedItem;
EXP ostream& operator<< (ostream& os, const S_msrOptionsValuedItem& elt);

//______________________________________________________________________________
/*!
\brief An option to provide help on any option.

  An element is represented by its name and the
  list of its enclosed elements plus optional parameters.
*/
class EXP msrOptionsItemHelpItem : public msrOptionsValuedItem
{
  public:
  
    // creation
    // ------------------------------------------------------

    static SMARTP<msrOptionsItemHelpItem> create (
      string             optionsItemShortName,
      string             optionsItemLongName,
      string             optionsItemDescription,
      string             optionsValueSpecification);
     
  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrOptionsItemHelpItem (
      string             optionsItemShortName,
      string             optionsItemLongName,
      string             optionsItemDescription,
      string             optionsValueSpecification);
           
  protected:


    virtual ~msrOptionsItemHelpItem();

  public:
  
    // set and get
    // ------------------------------------------------------

    // services
    // ------------------------------------------------------

    // print
    // ------------------------------------------------------

    void                  print (ostream& os) const;

  private:
  
    // fields
    // ------------------------------------------------------
};
typedef SMARTP<msrOptionsItemHelpItem> S_msrOptionsItemHelpItem;
ostream& operator<< (ostream& os, const S_msrOptionsItemHelpItem& elt);

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
  
    // fields
    // ------------------------------------------------------

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
  
    // fields
    // ------------------------------------------------------

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
  
    // fields
    // ------------------------------------------------------

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
  
    // fields
    // ------------------------------------------------------

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
  
    // fields
    // ------------------------------------------------------

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
  
    // fields
    // ------------------------------------------------------

    string                fOptionsPitchesLanguageItemVariableDisplayName;
    msrQuarterTonesPitchesLanguage&
                          fOptionsPitchesLanguageItemVariable;
};
typedef SMARTP<msrOptionsPitchesLanguageItem> S_msrOptionsPitchesLanguageItem;
ostream& operator<< (ostream& os, const S_msrOptionsPitchesLanguageItem& elt);

//______________________________________________________________________________
class EXP msrOptionsAccidentalStyleItem : public msrOptionsValuedItem
{
  public:
  
    // creation
    // ------------------------------------------------------

    static SMARTP<msrOptionsAccidentalStyleItem> create (
      string             optionsItemShortName,
      string             optionsItemLongName,
      string             optionsItemDescription,
      string             optionsValueSpecification,
      string             optionsAccidentalStyleItemVariableDisplayName,
      lpsrAccidentalStyle&
                         optionsAccidentalStyleItemVariable);
     
  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrOptionsAccidentalStyleItem (
      string             optionsItemShortName,
      string             optionsItemLongName,
      string             optionsItemDescription,
      string             optionsValueSpecification,
      string             optionsAccidentalStyleItemVariableDisplayName,
      lpsrAccidentalStyle&
                         optionsAccidentalStyleItemVariable);
      
    virtual ~msrOptionsAccidentalStyleItem();

  public:
  
    // set and get
    // ------------------------------------------------------

    string                getOptionsAccidentalStyleItemVariableDisplayName () const
                              {
                                return
                                  fOptionsAccidentalStyleItemVariableDisplayName;
                              }
                              
    void                  setAccidentalStyleItemVariableValue (
                            lpsrAccidentalStyle value)
                              {
                                fOptionsAccidentalStyleItemVariable = value;
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

    string                fOptionsAccidentalStyleItemVariableDisplayName;
    lpsrAccidentalStyle&  fOptionsAccidentalStyleItemVariable;
};
typedef SMARTP<msrOptionsAccidentalStyleItem> S_msrOptionsAccidentalStyleItem;
ostream& operator<< (ostream& os, const S_msrOptionsAccidentalStyleItem& elt);

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
  
    // fields
    // ------------------------------------------------------

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
                            string oldPartName,
                            string newPartName)
                              {
                                fOptionsPartRenameItemVariable [oldPartName] =
                                  newPartName;
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
  
    // fields
    // ------------------------------------------------------

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
  
    // fields
    // ------------------------------------------------------

    string                fOptionsMidiTempoItemVariableDisplayName;
    pair<string, int>&    fOptionsMidiTempoItemVariable;
};
typedef SMARTP<msrOptionsMidiTempoItem> S_msrOptionsMidiTempoItem;
ostream& operator<< (ostream& os, const S_msrOptionsMidiTempoItem& elt);

//_______________________________________________________________________________
class EXP msrOptionsSubGroup : public msrOptionsElement
{
  public:

    // data types
    // ------------------------------------------------------

    enum msrOptionsSubGroupDescriptionVisibility {
      kAlwaysShowDescription,
      kHideDescriptionByDefault };
          
    static string optionsSubGroupDescriptionVisibilityAsString (
      msrOptionsSubGroupDescriptionVisibility
        optionsSubGroupDescriptionVisibility);

  public:
  
    // creation
    // ------------------------------------------------------

    static SMARTP<msrOptionsSubGroup> create (
      string optionsSubGroupHelpHeader,
      string optionsSubGroupShortName,
      string optionsSubGroupLongName,
      string optionsSubGroupDescription,
      msrOptionsSubGroupDescriptionVisibility
             optionsSubGroupDescriptionVisibility);
     
  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrOptionsSubGroup (
      string optionsSubGroupHelpHeader,
      string optionsSubGroupShortName,
      string optionsSubGroupLongName,
      string optionsSubGroupDescription,
      msrOptionsSubGroupDescriptionVisibility
             optionsSubGroupDescriptionVisibility);
    
    virtual ~msrOptionsSubGroup();

  public:
  
    // set and get
    // ------------------------------------------------------

    void                  setOptionsGroupUplink (
                            S_msrOptionsGroup optionsGroup)
                              {
                                fOptionsGroupUplink =
                                  optionsGroup;
                              }

    S_msrOptionsGroup     getOptionsGroupUplink () const
                              { return fOptionsGroupUplink; }
    
    string                getOptionsSubGroupHelpHeader () const
                              { return fOptionsSubGroupHelpHeader; }
                              
    msrOptionsSubGroupDescriptionVisibility
                          getOptionsSubGroupIsHiddenByDefault () const
                              {
                                return
                                  fOptionsSubGroupDescriptionVisibility;
                              }
                              
    // services
    // ------------------------------------------------------

    void                  underlineHeader (ostream& os) const;

    void                  registerOptionsSubGroupInHandler (
                            S_msrOptionsHandler optionsHandler);

    void                  appendOptionsItem (
                            S_msrOptionsItem optionsItem);

    S_msrOptionsElement   fetchOptionElement (
                            string optiontElementName);
                            
    // print
    // ------------------------------------------------------

    void                  print (ostream& os) const;

    void                  printHelp (ostream& os) const;

    void                  printOptionsSubGroupForcedHelp (
                            ostream& os) const;

    void                  printOptionsItemForcedHelp (
                            ostream&             os,
                            S_msrOptionsItem     targetOptionsItem) const;

    void                  printHelpSummary (ostream& os) const;

    void                  printSpecificSubGroupHelp (
                            ostream& os,
                            S_msrOptionsSubGroup
                                     optionsSubGroup) const;

    void                  printOptionsValues (
                            ostream& os,
                            int      valueFieldWidth) const;

  private:
  
    // fields
    // ------------------------------------------------------

    S_msrOptionsGroup     fOptionsGroupUplink;

    string                fOptionsSubGroupHelpHeader;

    msrOptionsSubGroupDescriptionVisibility
                          fOptionsSubGroupDescriptionVisibility;
  
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
      string optionsGroupHelpHeader,
      string optionGroupShortName,
      string optionGroupLongName,
      string optionGroupDescription);
     
  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrOptionsGroup (
      string optionsGroupHelpHeader,
      string optionGroupShortName,
      string optionGroupLongName,
      string optionGroupDescription);
      
    virtual ~msrOptionsGroup();

  public:
  
    // set and get
    // ------------------------------------------------------

    void                  setOptionsHandlerUplink (
                            S_msrOptionsHandler optionsHandler)
                              {
                                fOptionsHandlerUplink =
                                  optionsHandler;
                              }

    S_msrOptionsHandler   getOptionsHandlerUplink () const
                              { return fOptionsHandlerUplink; }

    string                getOptionsGroupHelpHeader () const
                              { return fOptionsGroupHelpHeader; }
                              
    // services
    // ------------------------------------------------------
                              
    void                  underlineHeader (ostream& os) const;

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

    void                  printOptionsSubGroupForcedHelp (
                            ostream&             os,
                            S_msrOptionsSubGroup targetOptionsSubGroup) const;

    void                  printOptionsItemForcedHelp (
                            ostream&             os,
                            S_msrOptionsSubGroup targetOptionsSubGroup,
                            S_msrOptionsItem     targetOptionsItem) const;

    void                  printHelpSummary (ostream& os) const;
    
    void                  printSpecificSubGroupHelp (
                            ostream& os,
                            S_msrOptionsSubGroup
                                     optionsSubGroup) const;

    void                  printOptionsValues (
                            ostream& os,
                            int      valueFieldWidth) const;

  private:

    // fields
    // ------------------------------------------------------

    S_msrOptionsHandler   fOptionsHandlerUplink;
    
    string                fOptionsGroupHelpHeader;

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
/* JMI this a pure virtual class
    static SMARTP<msrOptionsHandler> create (
      string           optionsHandlerHelpHeader,
      string           optionsHandlerValuesHeader,
      string           optionHandlerHelpShortName,
      string           optionHandlerHelpLongName,
      string           optionHandlerHelpSummaryShortName,
      string           optionHandlerHelpSummaryLongName,
      string           optionHandlerPreamble,
      string           optionHandlerDescription,
      indentedOstream& optionsHandlerLogIOstream);
*/

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrOptionsHandler (
      string           optionsHandlerHelpHeader,
      string           optionsHandlerValuesHeader,
      string           optionHandlerHelpShortName,
      string           optionHandlerHelpLongName,
      string           optionHandlerHelpSummaryShortName,
      string           optionHandlerHelpSummaryLongName,
      string           optionHandlerPreamble,
      string           optionHandlerDescription,
      indentedOstream& optionsHandlerLogIOstream);
      
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
                              
    string                getOptionsHandlerValuesHeader () const
                              { return fOptionsHandlerValuesHeader; }

    string                getOptionHandlerHelpSummaryShortName () const
                              { return fOptionHandlerHelpSummaryShortName; }

    string                getOptionHandlerHelpSummaryLongName () const
                              { return fOptionHandlerHelpSummaryLongName; }

    string                getOptionHandlerPreamble () const
                              { return fOptionHandlerPreamble; }

    string                getProgramName () const
                              { return fProgramName; }
                              
    const list<S_msrOptionsElement>&
                          getCommandOptionsElements () const
                              { return fCommandOptionsElements; }
                              
    string                getCommandLineWithShortOptions () const
                              { return fCommandLineWithShortOptions; }
                              
    string                getCommandLineWithLongOptions () const
                              { return fCommandLineWithLongOptions; }
                              
    int                   getMaximumSubGroupsHelpHeadersSize () const
                              { return fMaximumSubGroupsHelpHeadersSize; }

    int                   getMaximumShortNameWidth () const
                              { return fMaximumShortNameWidth; }

    int                   getMaximumLongNameWidth () const
                              { return fMaximumLongNameWidth; }

    int                   getMaximumDisplayNameWidth () const
                              { return fMaximumDisplayNameWidth; }

  public:
  
    // services
    // ------------------------------------------------------

    S_msrOptionsElement   fetchOptionsElementInMap (
                            string optionsElementName) const;

    string                helpNamesBetweenParentheses () const; // JMI ???

    void                  registerOptionsNamesInHandler (
                            string              optionShortName,
                            string              optionLongName,
                            S_msrOptionsElement optionsElement);
      
    void                  registerOptionsHandlerInItself ();

    void                  registerOptionsElementInHandler (
                            S_msrOptionsElement optionsElement);

    void                  appendOptionsGroup (
                            S_msrOptionsGroup optionsGroup);

    S_msrOptionsElement   fetchOptionElement (
                            string optiontElementName);
                            
    void                  handleOptionsItemName (
                            string optionsItemName);

    void                  handleOptionsItemValueOrArgument (
                            string theString);

    const vector<string>  decipherOptionsAndArguments (
                            int   argc,
                            char* argv[]);

    virtual void          checkOptionsAndArguments () = 0;

    // print
    // ------------------------------------------------------

    void                  print (ostream& os) const;

    void                  printHelp (ostream& os) const;

    void                  printHelpSummary (ostream& os) const;
    
    void                  printSpecificSubGroupHelp (
                            ostream& os,
                            S_msrOptionsSubGroup
                                     optionsSubGroup) const;

    void                  printSpecificItemHelp (
                            ostream& os,
                            string   optionsItemName) const;

    void                  printOptionsValues (
                            ostream& os) const;

  protected:

    // fields
    // ------------------------------------------------------

    string                fOptionsHandlerHelpHeader;
    string                fOptionsHandlerValuesHeader;
    
    string                fOptionHandlerHelpSummaryShortName;
    string                fOptionHandlerHelpSummaryLongName;

    string                fOptionHandlerPreamble;

    S_msrOptionsItem      fPendingOptionsItem;

    vector<string>        fArgumentsVector;
    
    list<S_msrOptionsGroup>
                          fOptionsHandlerOptionsGroupsList;

    map<string, S_msrOptionsElement>
                          fOptionsElementsMap;
    
    int                   fMaximumSubGroupsHelpHeadersSize;

    int                   fMaximumShortNameWidth;
    int                   fMaximumLongNameWidth;
    
    int                   fMaximumDisplayNameWidth;

    list<S_msrOptionsElement>
                          fCommandOptionsElements;

    string                fProgramName;

    string                fCommandLineWithShortOptions;
    string                fCommandLineWithLongOptions;

    string                fInputSourceName;
    
    string                fOutputFileName;
    
    indentedOstream&      fOptionsHandlerLogIOstream;
};
typedef SMARTP<msrOptionsHandler> S_msrOptionsHandler;
ostream& operator<< (ostream& os, const S_msrOptionsHandler& elt);


/*! @} */

}


#endif
