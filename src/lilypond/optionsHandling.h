/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef __msrOptionsHandling__
#define __msrOptionsHandling__

#include "list"
#include "vector"
#include "map"

#include "smartpointer.h"
#include "rational.h"


using namespace std;

namespace MusicXML2 
{

//______________________________________________________________________________
// PRE-declarations for class dependencies

class msrOptionsElement;
typedef SMARTP<msrOptionsElement> S_msrOptionsElement;

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
  
    S_msrOptionsElement   fetchOptionElement (
                            string optiontElementName);
                            
    // print
    // ------------------------------------------------------

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
      msrOptionsItemKind optionsItemKind);
     
  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrOptionsItem (
      string             optionsItemShortName,
      string             optionsItemLongName,
      string             optionsItemDescription,
      msrOptionsItemKind optionsItemKind);
  
    virtual ~msrOptionsItem();

  public:
  
    // set and get
    // ------------------------------------------------------

    msrOptionsItemKind    getOptionsItemKind () const
                              { return fOptionsItemKind; }
  
    void                  setOptionsItemHasBeenSelected ()
                              { fOptionsItemHasBeenSelected = true; }
  
    bool                  getOptionsItemHasBeenSelected () const
                              { return fOptionsItemHasBeenSelected; }
  
    // services
    // ------------------------------------------------------

    // print
    // ------------------------------------------------------

    void                  print (ostream& os) const;

  protected:

    msrOptionsItemKind    fOptionsItemKind;
    
    bool                  fOptionsItemHasBeenSelected;
};
typedef SMARTP<msrOptionsItem> S_msrOptionsItem;
EXP ostream& operator<< (ostream& os, const S_msrOptionsItem& elt);

//______________________________________________________________________________
class EXP msrOptionsBoolItem : public msrOptionsItem
{
  public:
  
    // creation
    // ------------------------------------------------------

    static SMARTP<msrOptionsBoolItem> create (
      string optionsItemShortName,
      string optionsItemLongName,
      string optionsItemDescription,
      bool&  optionsBoolItemVariable);
     
  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrOptionsBoolItem (
      string optionsItemShortName,
      string optionsItemLongName,
      string optionsItemDescription,
      bool&  optionsBoolItemVariable);
      
    virtual ~msrOptionsBoolItem();

  public:
  
    // set and get
    // ------------------------------------------------------

    void                  setBoolItemVariableValue (
                            bool value)
                              {
                                fOptionsBoolItemVariable = value;
                              }

    // services
    // ------------------------------------------------------

    // print
    // ------------------------------------------------------

    void                  print (ostream& os) const;

  private:
  
    bool&                 fOptionsBoolItemVariable;
};
typedef SMARTP<msrOptionsBoolItem> S_msrOptionsBoolItem;
ostream& operator<< (ostream& os, const S_msrOptionsBoolItem& elt);

//______________________________________________________________________________
class EXP msrOptionsIntItem : public msrOptionsItem
{
  public:
  
    // creation
    // ------------------------------------------------------

    static SMARTP<msrOptionsIntItem> create (
      string             optionsItemShortName,
      string             optionsItemLongName,
      string             optionsItemDescription,
      int&               optionsIntItemVariable,
      msrOptionsItemKind optionsItemKind);
     
  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrOptionsIntItem (
      string             optionsItemShortName,
      string             optionsItemLongName,
      string             optionsItemDescription,
      int&               optionsIntItemVariable,
      msrOptionsItemKind optionsItemKind);
           
  protected:


    virtual ~msrOptionsIntItem();

  public:
  
    // set and get
    // ------------------------------------------------------

    void                  setIntItemVariableValue (
                            int value)
                              {
                                fOptionsIntItemVariable = value;
                              }

    // services
    // ------------------------------------------------------

    // print
    // ------------------------------------------------------

    void                  print (ostream& os) const;

  private:
  
    int&                  fOptionsIntItemVariable;
};
typedef SMARTP<msrOptionsIntItem> S_msrOptionsIntItem;
ostream& operator<< (ostream& os, const S_msrOptionsIntItem& elt);

//______________________________________________________________________________
class EXP msrOptionsFloatItem : public msrOptionsItem
{
  public:
  
    // creation
    // ------------------------------------------------------

    static SMARTP<msrOptionsFloatItem> create (
      string             optionsItemShortName,
      string             optionsItemLongName,
      string             optionsItemDescription,
      float&             optionsFloatItemVariable,
      msrOptionsItemKind optionsItemKind);
     
  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrOptionsFloatItem (
      string             optionsItemShortName,
      string             optionsItemLongName,
      string             optionsItemDescription,
      float&             optionsFloatItemVariable,
      msrOptionsItemKind optionsItemKind);
      
    virtual ~msrOptionsFloatItem();

  public:
  
    // set and get
    // ------------------------------------------------------

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

  private:
  
    float&                fOptionsFloatItemVariable;
};
typedef SMARTP<msrOptionsFloatItem> S_msrOptionsFloatItem;
ostream& operator<< (ostream& os, const S_msrOptionsFloatItem& elt);

//______________________________________________________________________________
class EXP msrOptionsStringItem : public msrOptionsItem
{
  public:
  
    // creation
    // ------------------------------------------------------

    static SMARTP<msrOptionsStringItem> create (
      string             optionsItemShortName,
      string             optionsItemLongName,
      string             optionsItemDescription,
      string&            optionsStringItemVariable,
      msrOptionsItemKind optionsItemKind);
     
  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrOptionsStringItem (
      string             optionsItemShortName,
      string             optionsItemLongName,
      string             optionsItemDescription,
      string&            optionsStringItemVariable,
      msrOptionsItemKind optionsItemKind);
      
    virtual ~msrOptionsStringItem();

  public:
  
    // set and get
    // ------------------------------------------------------

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

  private:
  
    string&               fOptionsStringItemVariable;
};
typedef SMARTP<msrOptionsStringItem> S_msrOptionsStringItem;
ostream& operator<< (ostream& os, const S_msrOptionsStringItem& elt);

//______________________________________________________________________________
class EXP msrOptionsRationalItem : public msrOptionsItem
{
  public:
  
    // creation
    // ------------------------------------------------------

    static SMARTP<msrOptionsRationalItem> create (
      string             optionsItemShortName,
      string             optionsItemLongName,
      string             optionsItemDescription,
      rational&          optionsRationalItemVariable,
      msrOptionsItemKind optionsItemKind);
     
  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrOptionsRationalItem (
      string             optionsItemShortName,
      string             optionsItemLongName,
      string             optionsItemDescription,
      rational&          optionsRationalItemVariable,
      msrOptionsItemKind optionsItemKind);
      
    virtual ~msrOptionsRationalItem();

  public:
  
    // set and get
    // ------------------------------------------------------

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

  private:
  
    rational&             fOptionsRationalItemVariable;
};
typedef SMARTP<msrOptionsRationalItem> S_msrOptionsRationalItem;
ostream& operator<< (ostream& os, const S_msrOptionsRationalItem& elt);

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

    void                  appendOptionsSubGroup (
                            S_msrOptionsSubGroup optionsSubGroup);
        
    S_msrOptionsElement   fetchOptionElement (
                            string optiontElementName);
                            
    // print
    // ------------------------------------------------------

    void                  print (ostream& os) const;

  private:

    list<S_msrOptionsSubGroup>
                          fOptionsGroupSubGroupsList;
};
typedef SMARTP<msrOptionsGroup> S_msrOptionsGroup;
ostream& operator<< (ostream& os, const S_msrOptionsGroup& elt);

//_______________________________________________________________________________
class EXP msrOptionsGroupsList : public msrOptionsElement
{
  public:
  
    // creation
    // ------------------------------------------------------

    static SMARTP<msrOptionsGroupsList> create (
      string optionGroupsListShortName,
      string optionGroupsListLongName,
      string optionGroupsListDescription);
     
  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrOptionsGroupsList (
      string optionGroupsListShortName,
      string optionGroupsListLongName,
      string optionGroupsListDescription);
      
    virtual ~msrOptionsGroupsList();

  public:
  
    // set and get
    // ------------------------------------------------------

    // services
    // ------------------------------------------------------

    void                  appendOptionsGroup (
                            S_msrOptionsGroup optionsGroup);
        
    S_msrOptionsElement   fetchOptionElement (
                            string optiontElementName);
                            
    // print
    // ------------------------------------------------------

    void                  print (ostream& os) const;

  private:

    list<S_msrOptionsGroup>
                          fOptionsGroupGroupsList;
};
typedef SMARTP<msrOptionsGroupsList> S_msrOptionsGroupsList;
ostream& operator<< (ostream& os, const S_msrOptionsGroupsList& elt);

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

  public:
  
    // services
    // ------------------------------------------------------

    void                  registerOptionsElement ( // protected ???
                            S_msrOptionsElement optionsElement);

    void                  appendOptionsGroup (
                            S_msrOptionsGroup optionsGroup);

    S_msrOptionsElement   fetchOptionElement (
                            string optiontElementName);
                            
    const vector<string>  analyzeOptions (
                            int   argc,
                            char* argv[]);

    // print
    // ------------------------------------------------------

    void                  print (ostream& os) const;

  protected:

    list<S_msrOptionsGroup>
                          fOptionsGroupHandler;

    map<string, S_msrOptionsElement>
                          fOptionsElementsMap;
};
typedef SMARTP<msrOptionsHandler> S_msrOptionsHandler;
ostream& operator<< (ostream& os, const S_msrOptionsHandler& elt);


/*! @} */

}


#endif
