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

#include "smartpointer.h"
#include "rational.h"


using namespace std;

namespace MusicXML2 
{

//______________________________________________________________________________
class EXP msrOptionsElement : public smartable
{
  public:

    // creation
    // ------------------------------------------------------

  protected:

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
  
    // print
    // ------------------------------------------------------

    virtual void          print (ostream& os) const;
    
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

  public:
  
    msrOptionsItem (
      string             optionsItemShortName,
      string             optionsItemLongName,
      string             optionsItemDescription,
      msrOptionsItemKind optionsItemKind);

    msrOptionsItemKind    getOptionsItemKind () const
                              { return fOptionsItemKind; }
  
    void                  setOptionsItemHasBeenSelected ()
                              { fOptionsItemHasBeenSelected = true; }
  
    bool                  getOptionsItemHasBeenSelected () const
                              { return fOptionsItemHasBeenSelected; }
  
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
  
    msrOptionsBoolItem (
      string optionsItemShortName,
      string optionsItemLongName,
      string optionsItemDescription,
      bool&  optionsBoolItemVariable);
      
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
  
    msrOptionsIntItem (
      string             optionsItemShortName,
      string             optionsItemLongName,
      string             optionsItemDescription,
      int&               optionsIntItemVariable,
      msrOptionsItemKind optionsItemKind);
      
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
  
    msrOptionsFloatItem (
      string             optionsItemShortName,
      string             optionsItemLongName,
      string             optionsItemDescription,
      float&             optionsFloatItemVariable,
      msrOptionsItemKind optionsItemKind);
      
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
  
    msrOptionsStringItem (
      string             optionsItemShortName,
      string             optionsItemLongName,
      string             optionsItemDescription,
      string&            optionsStringItemVariable,
      msrOptionsItemKind optionsItemKind);
      
    void                  print (ostream& os) const;

  private:
  
    string&                  fOptionsStringItemVariable;
};
typedef SMARTP<msrOptionsStringItem> S_msrOptionsStringItem;
ostream& operator<< (ostream& os, const S_msrOptionsStringItem& elt);

//_______________________________________________________________________________
class EXP msrOptionsSubGroup : public msrOptionsElement
{
  public:
  
    msrOptionsSubGroup (
      string optionsSubGroupShortName,
      string optionsSubGroupLongName,
      string optionsSubGroupDescription);
    
    void                  appendOptionsItem (
                            S_msrOptionsItem optionsItem)
                              {
                                fOptionsSubGroupItemsList.push_back (
                                  optionsItem);
                              }

    void                  print (ostream& os) const;

  private:
  
    list<S_msrOptionsItem>
                          fOptionsSubGroupItemsList;
};
typedef msrOptionsSubGroup* S_msrOptionsSubGroup;
typedef SMARTP<msrOptionsItem> S_msrOptionsItem;

//_______________________________________________________________________________
class EXP msrOptionsGroup : public msrOptionsElement
{
  public:
  
    msrOptionsGroup (
      string optionGroupShortName,
      string optionGroupLongName,
      string optionGroupDescription)
      : msrOptionsElement (
          optionGroupShortName,
          optionGroupLongName,
          optionGroupDescription)
      {}
      
    void                  appendOptionsSubGroup (
                            S_msrOptionsSubGroup optionsSubGroup)
      {
        fOptionsGroupSubGroupsList.push_back (
          optionsSubGroup);
      }
        
    void                  print (ostream& os) const
      {
        const int fieldWidth = 19;
        
        os <<
          "OptionsGroup:" <<
          endl <<
          setw(fieldWidth) <<
          "fOptionsElementShortName" << " : " << fOptionsElementShortName <<
          endl <<
          setw(fieldWidth) <<
          "fOptionsElementLongName" << " : " << fOptionsElementLongName <<
          endl <<
          setw(fieldWidth) <<
          "fOptionsElementDescription" << " : " << fOptionsElementDescription <<
          endl;
  
        for (
          list<S_msrOptionsSubGroup>::const_iterator
            i = fOptionsGroupSubGroupsList.begin();
          i != fOptionsGroupSubGroupsList.end();
          i++) {
          // print the element
          os << (*i);
        } // for
      }

  private:

    list<S_msrOptionsSubGroup>
                          fOptionsGroupSubGroupsList;
};
typedef msrOptionsGroup* S_msrOptionsGroup;
typedef SMARTP<msrOptionsItem> S_msrOptionsItem;

ostream& operator<< (ostream& os, const msrOptionsGroup& elt)
{
  os <<
    "OptionsGroup:" <<
    endl;
  elt.print (os);
  return os;
}

//_______________________________________________________________________________
class EXP msrOptionsGroupsList : public msrOptionsElement
{
  public:
  
    msrOptionsGroupsList (
      string optionGroupsListShortName,
      string optionGroupsListLongName,
      string optionGroupsListDescription)
      : msrOptionsElement (
          optionGroupsListShortName,
          optionGroupsListLongName,
          optionGroupsListDescription)
      {}
      
    void                  appendOptionsGroup (
                            S_msrOptionsGroup optionsGroup)
      {
        fOptionsGroupGroupsList.push_back (
          optionsGroup);
      }
        
    void                  print (ostream& os) const
      {
        const int fieldWidth = 19;
        
        os <<
          "OptionsGroupsList:" <<
          endl <<
          setw(fieldWidth) <<
          "fOptionsElementShortName" << " : " << fOptionsElementShortName <<
          endl <<
          setw(fieldWidth) <<
          "fOptionsElementLongName" << " : " << fOptionsElementLongName <<
          endl <<
          setw(fieldWidth) <<
          "fOptionsElementDescription" << " : " << fOptionsElementDescription <<
          endl;
  
        for (
          list<S_msrOptionsGroup>::const_iterator
            i = fOptionsGroupGroupsList.begin();
          i != fOptionsGroupGroupsList.end();
          i++) {
          // print the element
          os << (*i);
        } // for
      }

  private:

    list<S_msrOptionsGroup>
                          fOptionsGroupGroupsList;
};
typedef msrOptionsGroupsList* S_msrOptionsGroupsList;
typedef SMARTP<msrOptionsItem> S_msrOptionsItem;

ostream& operator<< (ostream& os, const msrOptionsGroupsList& elt)
{
  os <<
    "OptionsGroupsList:" <<
    endl;
  elt.print (os);
  return os;
}

//______________________________________________________________________________
void initializeOptionsHandling ();


/*! @} */

}


#endif
