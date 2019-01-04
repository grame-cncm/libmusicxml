/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef ___optionsBasicTypes___
#define ___optionsBasicTypes___

#include "list"
#include "vector"
#include "map"
#include "set"

#include "smartpointer.h"
#include "rational.h"

#include "utilities.h"

#include "msrBasicTypes.h"
#include "lpsrBasicTypes.h"

#include "setTraceOptionsIfDesired.h"


namespace MusicXML2 
{

//______________________________________________________________________________
// layout settings

const int K_OPTIONS_ELEMENTS_INDENTER_OFFSET = 3;
  // indent a bit more for readability

const int K_FIELD_WIDTH = 40;

//______________________________________________________________________________
// PRE-declarations for class dependencies

class optionsElement;
typedef SMARTP<optionsElement> S_optionsElement;

class optionsSubGroup;
typedef SMARTP<optionsSubGroup> S_optionsSubGroup;

class optionsGroup;
typedef SMARTP<optionsGroup> S_optionsGroup;

class EXP optionsHandler;
typedef SMARTP<optionsHandler> S_optionsHandler;

//______________________________________________________________________________
class optionsElement : public smartable
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<optionsElement> create (
      string optionsElementShortName,
      string optionsElementLongName,
      string optionsElementDescription);
     
  protected:

    // constructors/destructor
    // ------------------------------------------------------

    optionsElement (
      string optionsElementShortName,
      string optionsElementLongName,
      string optionsElementDescription);

    virtual ~optionsElement ();

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

    string                operator () () const
                              { return fOptionsElementDescription; }
  
    S_optionsElement      fetchOptionElement ( // JMI
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
typedef SMARTP<optionsElement> S_optionsElement;
EXP ostream& operator<< (ostream& os, const S_optionsElement& elt);

//______________________________________________________________________________
class optionsItem : public optionsElement
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<optionsItem> create (
      string optionsItemShortName,
      string optionsItemLongName,
      string optionsItemDescription);
     
  protected:

    // constructors/destructor
    // ------------------------------------------------------

    optionsItem (
      string optionsItemShortName,
      string optionsItemLongName,
      string optionsItemDescription);
  
    virtual ~optionsItem ();

  public:
  
    // set and get
    // ------------------------------------------------------

    void                  setOptionsSubGroupUplink (
                            S_optionsSubGroup optionsSubGroup);

    S_optionsSubGroup     getOptionsSubGroupUplink () const
                              { return fOptionsSubGroupUplink; }
        
    // services
    // ------------------------------------------------------

    void                  registerOptionsItemInHandler (
                            S_optionsHandler optionsHandler);

    // print
    // ------------------------------------------------------

    void                  print (ostream& os) const;

    virtual void          printOptionsValues (
                            ostream& os,
                            int      valueFieldWidth) const;

  protected:

    // fields
    // ------------------------------------------------------

    S_optionsSubGroup     fOptionsSubGroupUplink;
};
typedef SMARTP<optionsItem> S_optionsItem;
EXP ostream& operator<< (ostream& os, const S_optionsItem& elt);

//______________________________________________________________________________
class optionsHelpUsageItem : public optionsItem
{
  public:
  
    // creation
    // ------------------------------------------------------

    static SMARTP<optionsHelpUsageItem> create (
      string optionsItemShortName,
      string optionsItemLongName,
      string optionsItemDescription);
     
  protected:

    // constructors/destructor
    // ------------------------------------------------------

    optionsHelpUsageItem (
      string optionsItemShortName,
      string optionsItemLongName,
      string optionsItemDescription);
      
    virtual ~optionsHelpUsageItem ();

  public:
  
    // set and get
    // ------------------------------------------------------

    // services
    // ------------------------------------------------------

    // print
    // ------------------------------------------------------

    void                  print (ostream& os) const;

    void                  printHelpUsage (ostream& os) const;

    void                  printOptionsValues (
                            ostream& os,
                            int      valueFieldWidth) const;
  
  private:
  
    // fields
    // ------------------------------------------------------
};
typedef SMARTP<optionsHelpUsageItem> S_optionsHelpUsageItem;
ostream& operator<< (ostream& os, const S_optionsHelpUsageItem& elt);

//______________________________________________________________________________
class optionsHelpSummaryItem : public optionsItem
{
  public:
  
    // creation
    // ------------------------------------------------------

    static SMARTP<optionsHelpSummaryItem> create (
      string optionsItemShortName,
      string optionsItemLongName,
      string optionsItemDescription);
     
  protected:

    // constructors/destructor
    // ------------------------------------------------------

    optionsHelpSummaryItem (
      string optionsItemShortName,
      string optionsItemLongName,
      string optionsItemDescription);
      
    virtual ~optionsHelpSummaryItem ();

  public:
  
    // set and get
    // ------------------------------------------------------

    // services
    // ------------------------------------------------------

    // print
    // ------------------------------------------------------

    void                  print (ostream& os) const;

    void                  printHelpSummary (ostream& os) const;

    void                  printOptionsValues (
                            ostream& os,
                            int      valueFieldWidth) const;
  
  private:
  
    // fields
    // ------------------------------------------------------
};
typedef SMARTP<optionsHelpSummaryItem> S_optionsHelpSummaryItem;
ostream& operator<< (ostream& os, const S_optionsHelpSummaryItem& elt);

//______________________________________________________________________________
class optionsCombinedItemsItem : public optionsItem
{
  public:
  
    // creation
    // ------------------------------------------------------

    static SMARTP<optionsCombinedItemsItem> create (
      string optionsItemShortName,
      string optionsItemLongName,
      string optionsItemDescription,
      string optionsCombinedItemsItemVariableDisplayName,
      bool&  optionsCombinedItemsItemVariable);
     
  protected:

    // constructors/destructor
    // ------------------------------------------------------

    optionsCombinedItemsItem (
      string optionsItemShortName,
      string optionsItemLongName,
      string optionsItemDescription,
      string optionsCombinedItemsItemVariableDisplayName,
      bool&  optionsCombinedItemsItemVariable);
      
    virtual ~optionsCombinedItemsItem ();

  public:
  
    // set and get
    // ------------------------------------------------------

    const list<S_optionsItem>&
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

    void                  appendOptionsItemToCombinedItemsList (
                            S_optionsItem optionsItem);

    void                  appendOptionsItemToCombinedItemsList (
                            string optionsItemName);

    void                  setCombinedItemsVariablesValue (
                            bool value);

    // print
    // ------------------------------------------------------

    void                  print (ostream& os) const;

    void                  printHelp (ostream& os) const;

    void                  printOptionsValues (
                            ostream& os,
                            int      valueFieldWidth) const;
  
  private:
  
    // fields
    // ------------------------------------------------------

    string                fOptionsCombinedItemsItemVariableDisplayName;
    bool&                 fOptionsCombinedItemsItemVariable;

    list<S_optionsItem>
                          fOptionsCombinedItemsList;
};
typedef SMARTP<optionsCombinedItemsItem> S_optionsCombinedItemsItem;
ostream& operator<< (ostream& os, const S_optionsCombinedItemsItem& elt);

//______________________________________________________________________________
class optionsBooleanItem : public optionsItem
{
  public:
  
    // creation
    // ------------------------------------------------------

    static SMARTP<optionsBooleanItem> create (
      string optionsItemShortName,
      string optionsItemLongName,
      string optionsItemDescription,
      string optionsBooleanItemVariableDisplayName,
      bool&  optionsBooleanItemVariable);
     
  protected:

    // constructors/destructor
    // ------------------------------------------------------

    optionsBooleanItem (
      string optionsItemShortName,
      string optionsItemLongName,
      string optionsItemDescription,
      string optionsBooleanItemVariableDisplayName,
      bool&  optionsBooleanItemVariable);
      
    virtual ~optionsBooleanItem ();

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
typedef SMARTP<optionsBooleanItem> S_optionsBooleanItem;
ostream& operator<< (ostream& os, const S_optionsBooleanItem& elt);

//______________________________________________________________________________
class optionsTwoBooleansItem : public optionsItem
{
  public:
  
    // creation
    // ------------------------------------------------------

    static SMARTP<optionsTwoBooleansItem> create (
      string optionsItemShortName,
      string optionsItemLongName,
      string optionsItemDescription,
      string optionsTwoBooleansItemVariableDisplayName,
      bool&  optionsTwoBooleansItemVariable,
      bool&  optionsTwoBooleansItemSecondaryVariable);
     
  protected:

    // constructors/destructor
    // ------------------------------------------------------

    optionsTwoBooleansItem (
      string optionsItemShortName,
      string optionsItemLongName,
      string optionsItemDescription,
      string optionsTwoBooleansItemVariableDisplayName,
      bool&  optionsTwoBooleansItemVariable,
      bool&  optionsTwoBooleansItemSecondaryVariable);
      
    virtual ~optionsTwoBooleansItem ();

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
typedef SMARTP<optionsTwoBooleansItem> S_optionsTwoBooleansItem;
ostream& operator<< (ostream& os, const S_optionsTwoBooleansItem& elt);

//______________________________________________________________________________
class optionsThreeBooleansItem : public optionsItem
{
  public:
  
    // creation
    // ------------------------------------------------------

    static SMARTP<optionsThreeBooleansItem> create (
      string optionsItemShortName,
      string optionsItemLongName,
      string optionsItemDescription,
      string optionsThreeBooleansItemVariableDisplayName,
      bool&  optionsThreeBooleansItemVariable,
      bool&  optionsThreeBooleansItemSecondaryVariable,
      bool&  optionsThreeBooleansItemTertiaryVariable);
     
  protected:

    // constructors/destructor
    // ------------------------------------------------------

    optionsThreeBooleansItem (
      string optionsItemShortName,
      string optionsItemLongName,
      string optionsItemDescription,
      string optionsThreeBooleansItemVariableDisplayName,
      bool&  optionsThreeBooleansItemVariable,
      bool&  optionsThreeBooleansItemSecondaryVariable,
      bool&  optionsThreeBooleansItemTertiaryVariable);
      
    virtual ~optionsThreeBooleansItem ();

  public:
  
    // set and get
    // ------------------------------------------------------

    string                getOptionsThreeBooleansItemVariableDisplayName () const
                              {
                                return
                                  fOptionsThreeBooleansItemVariableDisplayName;
                              }
                              
    void                  setThreeBooleansItemVariableValue (
                            bool value)
                              {
                                fOptionsThreeBooleansItemVariable =
                                  value;
                                fOptionsThreeBooleansItemSecondaryVariable =
                                  value;
                                fOptionsThreeBooleansItemTertiaryVariable =
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

    string                fOptionsThreeBooleansItemVariableDisplayName;
    bool&                 fOptionsThreeBooleansItemVariable;
    
    bool&                 fOptionsThreeBooleansItemSecondaryVariable;
    bool&                 fOptionsThreeBooleansItemTertiaryVariable;
};
typedef SMARTP<optionsThreeBooleansItem> S_optionsThreeBooleansItem;
ostream& operator<< (ostream& os, const S_optionsThreeBooleansItem& elt);

//______________________________________________________________________________
class optionsValuedItem : public optionsItem
{
  public:

    // data types
    // ------------------------------------------------------

    enum optionsValuedItemKind {
      kOptionsItemHasNoArgument,
      kOptionsItemHasARequiredArgument,
      kOptionsItemHasAnOptionsArgument };
          
    static string optionsItemKindAsString (
      optionsValuedItemKind optionsItemKind);

    // creation
    // ------------------------------------------------------

    static SMARTP<optionsValuedItem> create (
      string optionsItemShortName,
      string optionsItemLongName,
      string optionsItemDescription,
      string optionsValueSpecification);
     
  protected:

    // constructors/destructor
    // ------------------------------------------------------

    optionsValuedItem (
      string optionsItemShortName,
      string optionsItemLongName,
      string optionsItemDescription,
      string optionsValueSpecification);
  
    virtual ~optionsValuedItem ();

  public:
  
    // set and get
    // ------------------------------------------------------

    optionsValuedItemKind
                          getOptionsItemKind () const
                              { return fOptionsItemKind; }
    
    string                getOptionsValueSpecification () const
                              { return fOptionsValueSpecification; }
    
    // services
    // ------------------------------------------------------

    void                  registerOptionsItemInHandler (
                            S_optionsHandler optionsHandler);

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

    optionsValuedItemKind fOptionsItemKind;

    string                fOptionsValueSpecification;
};
typedef SMARTP<optionsValuedItem> S_optionsValuedItem;
EXP ostream& operator<< (ostream& os, const S_optionsValuedItem& elt);

//______________________________________________________________________________
class optionsItemHelpItem : public optionsValuedItem
{
  public:
  
    // creation
    // ------------------------------------------------------

    static SMARTP<optionsItemHelpItem> create (
      string             optionsItemShortName,
      string             optionsItemLongName,
      string             optionsItemDescription,
      string             optionsValueSpecification);
     
  protected:

    // constructors/destructor
    // ------------------------------------------------------

    optionsItemHelpItem (
      string             optionsItemShortName,
      string             optionsItemLongName,
      string             optionsItemDescription,
      string             optionsValueSpecification);
           
  protected:


    virtual ~optionsItemHelpItem ();

  public:
  
    // set and get
    // ------------------------------------------------------

    // services
    // ------------------------------------------------------

    // print
    // ------------------------------------------------------

    void                  print (ostream& os) const;

    virtual void          printOptionsValues (
                            ostream& os,
                            int      valueFieldWidth) const;
  private:
  
    // fields
    // ------------------------------------------------------
};
typedef SMARTP<optionsItemHelpItem> S_optionsItemHelpItem;
ostream& operator<< (ostream& os, const S_optionsItemHelpItem& elt);

//______________________________________________________________________________
class optionsIntegerItem : public optionsValuedItem
{
  public:
  
    // creation
    // ------------------------------------------------------

    static SMARTP<optionsIntegerItem> create (
      string             optionsItemShortName,
      string             optionsItemLongName,
      string             optionsItemDescription,
      string             optionsValueSpecification,
      string             optionsIntegerItemVariableDisplayName,
      int&               optionsIntegerItemVariable);
     
  protected:

    // constructors/destructor
    // ------------------------------------------------------

    optionsIntegerItem (
      string             optionsItemShortName,
      string             optionsItemLongName,
      string             optionsItemDescription,
      string             optionsValueSpecification,
      string             optionsIntegerItemVariableDisplayName,
      int&               optionsIntegerItemVariable);
           
  protected:


    virtual ~optionsIntegerItem ();

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
typedef SMARTP<optionsIntegerItem> S_optionsIntegerItem;
ostream& operator<< (ostream& os, const S_optionsIntegerItem& elt);

//______________________________________________________________________________
class optionsFloatItem : public optionsValuedItem
{
  public:
  
    // creation
    // ------------------------------------------------------

    static SMARTP<optionsFloatItem> create (
      string             optionsItemShortName,
      string             optionsItemLongName,
      string             optionsItemDescription,
      string             optionsValueSpecification,
      string             optionsFloatItemVariableDisplayName,
      float&             optionsFloatItemVariable);
     
  protected:

    // constructors/destructor
    // ------------------------------------------------------

    optionsFloatItem (
      string             optionsItemShortName,
      string             optionsItemLongName,
      string             optionsItemDescription,
      string             optionsValueSpecification,
      string             optionsFloatItemVariableDisplayName,
      float&             optionsFloatItemVariable);
      
    virtual ~optionsFloatItem ();

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
typedef SMARTP<optionsFloatItem> S_optionsFloatItem;
ostream& operator<< (ostream& os, const S_optionsFloatItem& elt);

//______________________________________________________________________________
class optionsStringItem : public optionsValuedItem
{
  public:
  
    // creation
    // ------------------------------------------------------

    static SMARTP<optionsStringItem> create (
      string             optionsItemShortName,
      string             optionsItemLongName,
      string             optionsItemDescription,
      string             optionsValueSpecification,
      string             optionsStringItemVariableDisplayName,
      string&            optionsStringItemVariable);
     
  protected:

    // constructors/destructor
    // ------------------------------------------------------

    optionsStringItem (
      string             optionsItemShortName,
      string             optionsItemLongName,
      string             optionsItemDescription,
      string             optionsValueSpecification,
      string             optionsStringItemVariableDisplayName,
      string&            optionsStringItemVariable);
      
    virtual ~optionsStringItem ();

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
typedef SMARTP<optionsStringItem> S_optionsStringItem;
ostream& operator<< (ostream& os, const S_optionsStringItem& elt);

//______________________________________________________________________________
class optionsRationalItem : public optionsValuedItem
{
  public:
  
    // creation
    // ------------------------------------------------------

    static SMARTP<optionsRationalItem> create (
      string             optionsItemShortName,
      string             optionsItemLongName,
      string             optionsItemDescription,
      string             optionsValueSpecification,
      string             optionsRationalItemVariableDisplayName,
      rational&          optionsRationalItemVariable);
     
  protected:

    // constructors/destructor
    // ------------------------------------------------------

    optionsRationalItem (
      string             optionsItemShortName,
      string             optionsItemLongName,
      string             optionsItemDescription,
      string             optionsValueSpecification,
      string             optionsRationalItemVariableDisplayName,
      rational&          optionsRationalItemVariable);
      
    virtual ~optionsRationalItem ();

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
typedef SMARTP<optionsRationalItem> S_optionsRationalItem;
ostream& operator<< (ostream& os, const S_optionsRationalItem& elt);

//______________________________________________________________________________
class optionsNumbersSetItem : public optionsValuedItem
{
  public:
  
    // creation
    // ------------------------------------------------------

    static SMARTP<optionsNumbersSetItem> create (
      string             optionsItemShortName,
      string             optionsItemLongName,
      string             optionsItemDescription,
      string             optionsValueSpecification,
      string             optionsNumbersSetItemVariableDisplayName,
      set<int>&          optionsNumbersSetItemVariable);
     
  protected:

    // constructors/destructor
    // ------------------------------------------------------

    optionsNumbersSetItem (
      string             optionsItemShortName,
      string             optionsItemLongName,
      string             optionsItemDescription,
      string             optionsValueSpecification,
      string             optionsNumbersSetItemVariableDisplayName,
      set<int>&          optionsNumbersSetItemVariable);
      
    virtual ~optionsNumbersSetItem ();

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
typedef SMARTP<optionsNumbersSetItem> S_optionsNumbersSetItem;
ostream& operator<< (ostream& os, const S_optionsNumbersSetItem& elt);

//_______________________________________________________________________________
class optionsSubGroup : public optionsElement
{
  public:

    // data types
    // ------------------------------------------------------

    enum optionsSubGroupDescriptionVisibilityKind {
      kAlwaysShowDescription,
      kHideDescriptionByDefault };
          
    static string optionsSubGroupDescriptionVisibilityKindAsString (
      optionsSubGroupDescriptionVisibilityKind
        optionsSubGroupDescriptionVisibilityKind);

  public:
  
    // creation
    // ------------------------------------------------------

    static SMARTP<optionsSubGroup> create (
      string optionsSubGroupHelpHeader,
      string optionsSubGroupShortName,
      string optionsSubGroupLongName,
      string optionsSubGroupDescription,
      optionsSubGroupDescriptionVisibilityKind
             optionsSubGroupDescriptionVisibilityKind,
      S_optionsGroup
             optionsGroupUplink);
     
  protected:

    // constructors/destructor
    // ------------------------------------------------------

    optionsSubGroup (
      string optionsSubGroupHelpHeader,
      string optionsSubGroupShortName,
      string optionsSubGroupLongName,
      string optionsSubGroupDescription,
      optionsSubGroupDescriptionVisibilityKind
             optionsSubGroupDescriptionVisibilityKind,
      S_optionsGroup
             optionsGroupUplink);
    
    virtual ~optionsSubGroup ();

  public:
  
    // set and get
    // ------------------------------------------------------

    void                  setOptionsGroupUplink (
                            S_optionsGroup optionsGroup)
                              {
                                fOptionsGroupUplink =
                                  optionsGroup;
                              }

    S_optionsGroup        getOptionsGroupUplink () const
                              { return fOptionsGroupUplink; }
    
    string                getOptionsSubGroupHelpHeader () const
                              { return fOptionsSubGroupHelpHeader; }
                              
    optionsSubGroupDescriptionVisibilityKind
                          getOptionsSubGroupIsHiddenByDefault () const
                              {
                                return
                                  fOptionsSubGroupDescriptionVisibilityKind;
                              }
                              
    // services
    // ------------------------------------------------------

    void                  underlineHeader (ostream& os) const;

    void                  registerOptionsSubGroupInHandler (
                            S_optionsHandler optionsHandler);

    void                  appendOptionsItem (
                            S_optionsItem optionsItem);

    S_optionsElement      fetchOptionElement (
                            string optiontElementName);
                            
    // print
    // ------------------------------------------------------

    void                  print (ostream& os) const;

    void                  printHelp (ostream& os) const;

    void                  printOptionsSubGroupForcedHelp (
                            ostream& os) const;

    void                  printOptionsItemForcedHelp (
                            ostream&         os,
                            S_optionsItem targetOptionsItem) const;

    void                  printHelpSummary (ostream& os) const;

    void                  printSpecificSubGroupHelp (
                            ostream& os,
                            S_optionsSubGroup
                                     optionsSubGroup) const;

    void                  printOptionsValues (
                            ostream& os,
                            int      valueFieldWidth) const;

  private:
  
    // fields
    // ------------------------------------------------------

    S_optionsGroup        fOptionsGroupUplink;

    string                fOptionsSubGroupHelpHeader;

    optionsSubGroupDescriptionVisibilityKind
                          fOptionsSubGroupDescriptionVisibilityKind;
  
    list<S_optionsItem>   fOptionsSubGroupItemsList;
};
typedef SMARTP<optionsSubGroup> S_optionsSubGroup;
ostream& operator<< (ostream& os, const S_optionsSubGroup& elt);

//_______________________________________________________________________________
class optionsGroup : public optionsElement
{
  public:
  
    // creation
    // ------------------------------------------------------

    static SMARTP<optionsGroup> create (
      string optionsGroupHelpHeader,
      string optionGroupShortName,
      string optionGroupLongName,
      string optionGroupDescription,
      S_optionsHandler
             optionsHandlerUplink);
     
  protected:

    // constructors/destructor
    // ------------------------------------------------------

    optionsGroup (
      string optionsGroupHelpHeader,
      string optionGroupShortName,
      string optionGroupLongName,
      string optionGroupDescription,
      S_optionsHandler
             optionsHandlerUplink);
      
    virtual ~optionsGroup ();

  public:
  
    // set and get
    // ------------------------------------------------------

    void                  setOptionsHandlerUplink (
                            S_optionsHandler optionsHandler);

    S_optionsHandler      getOptionsHandlerUplink () const
                              { return fOptionsHandlerUplink; }

    string                getOptionsGroupHelpHeader () const
                              { return fOptionsGroupHelpHeader; }
                              
  public:

    // consistency check
    // ------------------------------------------------------

    void                  checkOptionsConsistency ();

  public:
  
    // services
    // ------------------------------------------------------
                              
    void                  underlineHeader (ostream& os) const;

    void                  registerOptionsGroupInHandler (
                            S_optionsHandler optionsHandler);

    void                  appendOptionsSubGroup (
                            S_optionsSubGroup optionsSubGroup);
        
    S_optionsElement      fetchOptionElement (
                            string optiontElementName);

    virtual S_optionsItem handleOptionsItem (
                            ostream&      os,
                            S_optionsItem item);
                                
    virtual void          handleOptionsItemValue (
                            ostream&      os,
                            S_optionsItem item,
                            string        theString);

    // print
    // ------------------------------------------------------

    void                  print (ostream& os) const;

    void                  printHelp (ostream& os) const;

    void                  printOptionsSubGroupForcedHelp (
                            ostream&             os,
                            S_optionsSubGroup targetOptionsSubGroup) const;

    void                  printOptionsItemForcedHelp (
                            ostream&          os,
                            S_optionsSubGroup targetOptionsSubGroup,
                            S_optionsItem     targetOptionsItem) const;

    void                  printHelpSummary (ostream& os) const;
    
    void                  printSpecificSubGroupHelp (
                            ostream& os,
                            S_optionsSubGroup
                                     optionsSubGroup) const;

    void                  printOptionsValues (
                            ostream& os,
                            int      valueFieldWidth) const;

  protected:

    // uplink
    // ------------------------------------------------------

    S_optionsHandler      fOptionsHandlerUplink;
    
  private:

    // fields
    // ------------------------------------------------------

    string                fOptionsGroupHelpHeader;

    list<S_optionsSubGroup>
                          fOptionsGroupSubGroupsList;
};
typedef SMARTP<optionsGroup> S_optionsGroup;
ostream& operator<< (ostream& os, const S_optionsGroup& elt);

//_______________________________________________________________________________
class EXP optionsHandler : public optionsElement
{
  public:
  
    // creation
    // ------------------------------------------------------
/* JMI this a pure virtual class
    static SMARTP<optionsHandler> create (
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

    optionsHandler (
      string           optionsHandlerHelpHeader,
      string           optionsHandlerValuesHeader,
      string           optionHandlerHelpShortName,
      string           optionHandlerHelpLongName,
      string           optionHandlerHelpSummaryShortName,
      string           optionHandlerHelpSummaryLongName,
      string           optionHandlerPreamble,
      string           optionHandlerDescription,
      indentedOstream& optionsHandlerLogIOstream);
      
    virtual ~optionsHandler ();

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

    const indentedOstream&
                          getOptionsHandlerLogIOstream ()
                              { return fOptionsHandlerLogIOstream; }

    string                getProgramName () const
                              { return fProgramName; }
                              
    const list<S_optionsElement>&
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

    S_optionsElement      fetchOptionsElementFromMap (
                            string optionsElementName) const;

    string                helpNamesBetweenParentheses () const; // JMI ???

    void                  registerOptionsNamesInHandler (
                            string           optionShortName,
                            string           optionLongName,
                            S_optionsElement optionsElement);
      
    void                  registerOptionsHandlerInItself ();

    void                  registerOptionsElementInHandler (
                            S_optionsElement optionsElement);

    void                  appendOptionsGroup (
                            S_optionsGroup optionsGroup);

    S_optionsElement      fetchOptionElement (
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
    void                  printHelpSummary () const
                              {
                                printHelpSummary (
                                  fOptionsHandlerLogIOstream);
                              }
    
    void                  printSpecificSubGroupHelp (
                            ostream& os,
                            S_optionsSubGroup
                                     optionsSubGroup) const;

    void                  printSpecificItemHelp (
                            ostream& os,
                            string   optionsItemName) const;

    void                  printAllOptionsValues (
                            ostream& os) const;

  protected:

    // fields
    // ------------------------------------------------------

    string                fOptionsHandlerHelpHeader;
    string                fOptionsHandlerValuesHeader;
    
    string                fOptionHandlerHelpSummaryShortName;
    string                fOptionHandlerHelpSummaryLongName;

    string                fOptionHandlerPreamble;

    S_optionsItem         fPendingOptionsItem;

    vector<string>        fArgumentsVector;
    
    list<S_optionsGroup>  fOptionsHandlerOptionsGroupsList;

    map<string, S_optionsElement>
                          fOptionsElementsMap;
    
    int                   fMaximumSubGroupsHelpHeadersSize;

    int                   fMaximumShortNameWidth;
    int                   fMaximumLongNameWidth;
    
    int                   fMaximumDisplayNameWidth;

    list<S_optionsElement>
                          fCommandOptionsElements;

    string                fProgramName;

    string                fCommandLineWithShortOptions;
    string                fCommandLineWithLongOptions;
    
    indentedOstream&      fOptionsHandlerLogIOstream;
};
typedef SMARTP<optionsHandler> S_optionsHandler;
ostream& operator<< (ostream& os, const S_optionsHandler& elt);


}


#endif
