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


using namespace std;

namespace MusicXML2
{

// layout settings
//______________________________________________________________________________
const int K_OPTIONS_ELEMENTS_INDENTER_OFFSET = 3;
  // indent a bit more for readability

const int K_OPTIONS_FIELD_WIDTH = 40;

// PRE-declarations for class dependencies
//______________________________________________________________________________
class oahElement;
typedef SMARTP<oahElement> S_oahElement;

class oahSubGroup;
typedef SMARTP<oahSubGroup> S_oahSubGroup;

class oahGroup;
typedef SMARTP<oahGroup> S_oahGroup;

class EXP oahHandler;
typedef SMARTP<oahHandler> S_oahHandler;

// options elements
//______________________________________________________________________________
class oahElement : public smartable
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<oahElement> create (
      string oahElementShortName,
      string oahElementLongName,
      string oahElementDescription);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    oahElement (
      string oahElementShortName,
      string oahElementLongName,
      string oahElementDescription);

    virtual ~oahElement ();

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

    string                oahElementNames () const;
    string                oahElementNamesInColumns (
                            int subGroupsShortNameFieldWidth) const;

    string                oahElementNamesBetweenParentheses () const;
    string                oahElementNamesInColumnsBetweenParentheses (
                            int subGroupsShortNameFieldWidth) const;

    virtual int           fetchOptionsElementVariableDisplayNameLength () const
                              { return 0; }

    string                operator () () const
                              { return fOptionsElementDescription; }

    S_oahElement      fetchOptionElement ( // JMI
                            string optiontElementName);

    // print
    // ------------------------------------------------------

    string                asString () const;

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
typedef SMARTP<oahElement> S_oahElement;
EXP ostream& operator<< (ostream& os, const S_oahElement& elt);

// options items
//______________________________________________________________________________
class oahAtom : public oahElement
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<oahAtom> create (
      string oahAtomShortName,
      string oahAtomLongName,
      string oahAtomDescription);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    oahAtom (
      string oahAtomShortName,
      string oahAtomLongName,
      string oahAtomDescription);

    virtual ~oahAtom ();

  public:

    // set and get
    // ------------------------------------------------------

    void                  setOptionsSubGroupUpLink (
                            S_oahSubGroup oahSubGroup);

    S_oahSubGroup     getOptionsSubGroupUpLink () const
                              { return fOptionsSubGroupUpLink; }

    // services
    // ------------------------------------------------------

    void                  registerOptionsItemInHandler (
                            S_oahHandler oahHandler);

    // print
    // ------------------------------------------------------

    void                  print (ostream& os) const;

    virtual void          printOptionsValues (
                            ostream& os,
                            int      valueFieldWidth) const;

  protected:

    // fields
    // ------------------------------------------------------

    S_oahSubGroup     fOptionsSubGroupUpLink;
};
typedef SMARTP<oahAtom> S_oahAtom;
EXP ostream& operator<< (ostream& os, const S_oahAtom& elt);

// pointers to options items methods
//______________________________________________________________________________
typedef void (oahAtom::*oahAtomMethodPtrType)();

/*
  It's worth noting that, as of C++11, you could write this expression
  as a more legible using statement:
    using oahAtomMethodPtrType = void (oahAtom::*)();
*/

// options help usage items
//______________________________________________________________________________
class oahHelpUsageAtom : public oahAtom
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<oahHelpUsageAtom> create (
      string oahAtomShortName,
      string oahAtomLongName,
      string oahAtomDescription);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    oahHelpUsageAtom (
      string oahAtomShortName,
      string oahAtomLongName,
      string oahAtomDescription);

    virtual ~oahHelpUsageAtom ();

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
typedef SMARTP<oahHelpUsageAtom> S_oahHelpUsageAtom;
EXP ostream& operator<< (ostream& os, const S_oahHelpUsageAtom& elt);

// options help summary items
//______________________________________________________________________________
class oahHelpSummaryAtom : public oahAtom
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<oahHelpSummaryAtom> create (
      string oahAtomShortName,
      string oahAtomLongName,
      string oahAtomDescription);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    oahHelpSummaryAtom (
      string oahAtomShortName,
      string oahAtomLongName,
      string oahAtomDescription);

    virtual ~oahHelpSummaryAtom ();

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
typedef SMARTP<oahHelpSummaryAtom> S_oahHelpSummaryAtom;
EXP ostream& operator<< (ostream& os, const S_oahHelpSummaryAtom& elt);

// options items with variable name
//______________________________________________________________________________
class oahAtomWithVariableDisplayName : public oahAtom
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<oahAtomWithVariableDisplayName> create (
      string oahAtomShortName,
      string oahAtomLongName,
      string oahAtomDescription,
      string oahAtomVariableDisplayName);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    oahAtomWithVariableDisplayName (
      string oahAtomShortName,
      string oahAtomLongName,
      string oahAtomDescription,
      string oahAtomVariableDisplayName);

    virtual ~oahAtomWithVariableDisplayName ();

  public:

    // set and get
    // ------------------------------------------------------

    string                getOptionsItemVariableDisplayName () const
                              {
                                return
                                  fOptionsItemVariableDisplayName;
                              }

    // services
    // ------------------------------------------------------

    virtual int           fetchOptionsElementVariableDisplayNameLength () const
                              {
                                return
                                  fOptionsItemVariableDisplayName.size ();
                              }

    // print
    // ------------------------------------------------------

    void                  print (ostream& os) const;

    void                  printOptionsValues (
                            ostream& os,
                            int      valueFieldWidth) const;

  protected:

    // fields
    // ------------------------------------------------------

    string                fOptionsItemVariableDisplayName;
};
typedef SMARTP<oahAtomWithVariableDisplayName> S_oahAtomWithVariableDisplayName;
EXP ostream& operator<< (ostream& os, const S_oahAtomWithVariableDisplayName& elt);

// options boolean items
//______________________________________________________________________________
class oahBooleanAtom : public oahAtomWithVariableDisplayName
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<oahBooleanAtom> create (
      string oahAtomShortName,
      string oahAtomLongName,
      string oahAtomDescription,
      string oahBooleanAtomVariableDisplayName,
      bool&  oahBooleanAtomVariable);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    oahBooleanAtom (
      string oahAtomShortName,
      string oahAtomLongName,
      string oahAtomDescription,
      string oahBooleanAtomVariableDisplayName,
      bool&  oahBooleanAtomVariable);

    virtual ~oahBooleanAtom ();

  public:

    // set and get
    // ------------------------------------------------------

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

    bool&                 fOptionsBooleanItemVariable;
};
typedef SMARTP<oahBooleanAtom> S_oahBooleanAtom;
EXP ostream& operator<< (ostream& os, const S_oahBooleanAtom& elt);

// options two boolean items
//______________________________________________________________________________
class oahTwoBooleansAtom : public oahAtomWithVariableDisplayName
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<oahTwoBooleansAtom> create (
      string oahAtomShortName,
      string oahAtomLongName,
      string oahAtomDescription,
      string oahTwoBooleansAtomVariableDisplayName,
      bool&  oahTwoBooleansAtomVariable,
      bool&  oahTwoBooleansAtomSecondaryVariable);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    oahTwoBooleansAtom (
      string oahAtomShortName,
      string oahAtomLongName,
      string oahAtomDescription,
      string oahTwoBooleansAtomVariableDisplayName,
      bool&  oahTwoBooleansAtomVariable,
      bool&  oahTwoBooleansAtomSecondaryVariable);

    virtual ~oahTwoBooleansAtom ();

  public:

    // set and get
    // ------------------------------------------------------

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

    bool&                 fOptionsTwoBooleansItemVariable;

    bool&                 fOptionsTwoBooleansItemSecondaryVariable;
};
typedef SMARTP<oahTwoBooleansAtom> S_oahTwoBooleansAtom;
EXP ostream& operator<< (ostream& os, const S_oahTwoBooleansAtom& elt);

// options three boolean items
//______________________________________________________________________________
class oahThreeBooleansAtom : public oahAtomWithVariableDisplayName
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<oahThreeBooleansAtom> create (
      string oahAtomShortName,
      string oahAtomLongName,
      string oahAtomDescription,
      string oahThreeBooleansAtomVariableDisplayName,
      bool&  oahThreeBooleansAtomVariable,
      bool&  oahThreeBooleansAtomSecondaryVariable,
      bool&  oahThreeBooleansAtomTertiaryVariable);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    oahThreeBooleansAtom (
      string oahAtomShortName,
      string oahAtomLongName,
      string oahAtomDescription,
      string oahThreeBooleansAtomVariableDisplayName,
      bool&  oahThreeBooleansAtomVariable,
      bool&  oahThreeBooleansAtomSecondaryVariable,
      bool&  oahThreeBooleansAtomTertiaryVariable);

    virtual ~oahThreeBooleansAtom ();

  public:

    // set and get
    // ------------------------------------------------------

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

    bool&                 fOptionsThreeBooleansItemVariable;

    bool&                 fOptionsThreeBooleansItemSecondaryVariable;
    bool&                 fOptionsThreeBooleansItemTertiaryVariable;
};
typedef SMARTP<oahThreeBooleansAtom> S_oahThreeBooleansAtom;
EXP ostream& operator<< (ostream& os, const S_oahThreeBooleansAtom& elt);

// options combined items items
//______________________________________________________________________________
class oahCombinedBooleansAtom : public oahAtomWithVariableDisplayName
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<oahCombinedBooleansAtom> create (
      string oahAtomShortName,
      string oahAtomLongName,
      string oahAtomDescription,
      string oahCombinedBooleansAtomVariableDisplayName,
      bool&  oahCombinedBooleansAtomVariable);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    oahCombinedBooleansAtom (
      string oahAtomShortName,
      string oahAtomLongName,
      string oahAtomDescription,
      string oahCombinedBooleansAtomVariableDisplayName,
      bool&  oahCombinedBooleansAtomVariable);

    virtual ~oahCombinedBooleansAtom ();

  public:

    // set and get
    // ------------------------------------------------------

    const list<S_oahAtom>&
                          getOptionsCombinedBooleanItemsList ()
                              {
                                return
                                  fOptionsCombinedBooleanItemsList;
                              }

/*
    void                  setCombinedBooleanItemsItemVariableValue (
                            set<int> value)
                              {
                                fOptionsCombinedBooleanItemsItemVariable = value;
                              }
*/
    // services
    // ------------------------------------------------------

    void                  appendOptionsItemToCombinedBooleanItemsList (
                            S_oahAtom oahAtom);

    void                  appendOptionsItemToCombinedBooleanItemsList (
                            string oahAtomName);

    void                  setCombinedBooleanItemsVariablesValue (
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

    bool&                 fOptionsCombinedBooleanItemsItemVariable;

    list<S_oahAtom>       fOptionsCombinedBooleanItemsList;
};
typedef SMARTP<oahCombinedBooleansAtom> S_oahCombinedBooleansAtom;
EXP ostream& operator<< (ostream& os, const S_oahCombinedBooleansAtom& elt);

// options valued items
//______________________________________________________________________________
class optionsValuedItem : public oahAtomWithVariableDisplayName
{
  public:

    // data types
    // ------------------------------------------------------

    enum optionsValuedItemKind {
      kOptionsItemHasNoArgument,
      kOptionsItemHasARequiredArgument,
      kOptionsItemHasAnOptionsArgument };

    static string oahAtomKindAsString (
      optionsValuedItemKind oahAtomKind);

    // creation
    // ------------------------------------------------------

    static SMARTP<optionsValuedItem> create (
      string oahAtomShortName,
      string oahAtomLongName,
      string oahAtomDescription,
      string optionsValueSpecification,
      string optionsValuedItemVariableDisplayName);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    optionsValuedItem (
      string oahAtomShortName,
      string oahAtomLongName,
      string oahAtomDescription,
      string optionsValueSpecification,
      string optionsValuedItemVariableDisplayName);

    virtual ~optionsValuedItem ();

  public:

    // set and get
    // ------------------------------------------------------

    optionsValuedItemKind
                          getOptionsItemKind () const
                              { return fOptionsItemKind; }

    string                getOptionsValueSpecification () const
                              { return fOptionsValueSpecification; }

    void                  setOptionsValueIsOptional ()
                              { fOptionsValueIsOptional = true; }

    bool                  getOptionsValueIsOptional () const
                              { return fOptionsValueIsOptional; }

    // services
    // ------------------------------------------------------

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

    bool                  fOptionsValueIsOptional;
};
typedef SMARTP<optionsValuedItem> S_optionsValuedItem;
EXP ostream& operator<< (ostream& os, const S_optionsValuedItem& elt);

// options items helps items
//______________________________________________________________________________
class oahAtomHelpItem : public optionsValuedItem
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<oahAtomHelpItem> create (
      string             oahAtomShortName,
      string             oahAtomLongName,
      string             oahAtomDescription,
      string             optionsValueSpecification,
      string             optionsHelpItemVariableDisplayName);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    oahAtomHelpItem (
      string             oahAtomShortName,
      string             oahAtomLongName,
      string             oahAtomDescription,
      string             optionsValueSpecification,
      string             optionsHelpItemVariableDisplayName);

  protected:


    virtual ~oahAtomHelpItem ();

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

    string             fOptionsValueSpecification;
};
typedef SMARTP<oahAtomHelpItem> S_oahAtomHelpItem;
EXP ostream& operator<< (ostream& os, const S_oahAtomHelpItem& elt);

// options integer items
//______________________________________________________________________________
class optionsIntegerItem : public optionsValuedItem
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<optionsIntegerItem> create (
      string             oahAtomShortName,
      string             oahAtomLongName,
      string             oahAtomDescription,
      string             optionsValueSpecification,
      string             optionsIntegerItemVariableDisplayName,
      int&               optionsIntegerItemVariable);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    optionsIntegerItem (
      string             oahAtomShortName,
      string             oahAtomLongName,
      string             oahAtomDescription,
      string             optionsValueSpecification,
      string             optionsIntegerItemVariableDisplayName,
      int&               optionsIntegerItemVariable);

  protected:


    virtual ~optionsIntegerItem ();

  public:

    // set and get
    // ------------------------------------------------------

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

    int&                  fOptionsIntegerItemVariable;
};
typedef SMARTP<optionsIntegerItem> S_optionsIntegerItem;
EXP ostream& operator<< (ostream& os, const S_optionsIntegerItem& elt);

// options float items
//______________________________________________________________________________
class optionsFloatItem : public optionsValuedItem
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<optionsFloatItem> create (
      string             oahAtomShortName,
      string             oahAtomLongName,
      string             oahAtomDescription,
      string             optionsValueSpecification,
      string             optionsFloatItemVariableDisplayName,
      float&             optionsFloatItemVariable);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    optionsFloatItem (
      string             oahAtomShortName,
      string             oahAtomLongName,
      string             oahAtomDescription,
      string             optionsValueSpecification,
      string             optionsFloatItemVariableDisplayName,
      float&             optionsFloatItemVariable);

    virtual ~optionsFloatItem ();

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

    void                  printOptionsValues (
                            ostream& os,
                            int      valueFieldWidth) const;

  private:

    // fields
    // ------------------------------------------------------

    float&                fOptionsFloatItemVariable;
};
typedef SMARTP<optionsFloatItem> S_optionsFloatItem;
EXP ostream& operator<< (ostream& os, const S_optionsFloatItem& elt);

// options string items
//______________________________________________________________________________
class optionsStringItem : public optionsValuedItem
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<optionsStringItem> create (
      string             oahAtomShortName,
      string             oahAtomLongName,
      string             oahAtomDescription,
      string             optionsValueSpecification,
      string             optionsStringItemVariableDisplayName,
      string&            optionsStringItemVariable);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    optionsStringItem (
      string             oahAtomShortName,
      string             oahAtomLongName,
      string             oahAtomDescription,
      string             optionsValueSpecification,
      string             optionsStringItemVariableDisplayName,
      string&            optionsStringItemVariable);

    virtual ~optionsStringItem ();

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

    void                  printOptionsValues (
                            ostream& os,
                            int      valueFieldWidth) const;

  private:

    // fields
    // ------------------------------------------------------

    string&               fOptionsStringItemVariable;
};
typedef SMARTP<optionsStringItem> S_optionsStringItem;
EXP ostream& operator<< (ostream& os, const S_optionsStringItem& elt);

// options rational items
//______________________________________________________________________________
class optionsRationalItem : public optionsValuedItem
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<optionsRationalItem> create (
      string             oahAtomShortName,
      string             oahAtomLongName,
      string             oahAtomDescription,
      string             optionsValueSpecification,
      string             optionsRationalItemVariableDisplayName,
      rational&          optionsRationalItemVariable);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    optionsRationalItem (
      string             oahAtomShortName,
      string             oahAtomLongName,
      string             oahAtomDescription,
      string             optionsValueSpecification,
      string             optionsRationalItemVariableDisplayName,
      rational&          optionsRationalItemVariable);

    virtual ~optionsRationalItem ();

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

    void                  printOptionsValues (
                            ostream& os,
                            int      valueFieldWidth) const;

  private:

    // fields
    // ------------------------------------------------------

    rational&             fOptionsRationalItemVariable;
};
typedef SMARTP<optionsRationalItem> S_optionsRationalItem;
EXP ostream& operator<< (ostream& os, const S_optionsRationalItem& elt);

// options numbers sets items
//______________________________________________________________________________
class optionsNumbersSetItem : public optionsValuedItem
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<optionsNumbersSetItem> create (
      string             oahAtomShortName,
      string             oahAtomLongName,
      string             oahAtomDescription,
      string             optionsValueSpecification,
      string             optionsNumbersSetItemVariableDisplayName,
      set<int>&          optionsNumbersSetItemVariable);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    optionsNumbersSetItem (
      string             oahAtomShortName,
      string             oahAtomLongName,
      string             oahAtomDescription,
      string             optionsValueSpecification,
      string             optionsNumbersSetItemVariableDisplayName,
      set<int>&          optionsNumbersSetItemVariable);

    virtual ~optionsNumbersSetItem ();

  public:

    // set and get
    // ------------------------------------------------------

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

    set<int>&             fOptionsNumbersSetItemVariable;
};
typedef SMARTP<optionsNumbersSetItem> S_optionsNumbersSetItem;
EXP ostream& operator<< (ostream& os, const S_optionsNumbersSetItem& elt);

// options subgroups
//_______________________________________________________________________________
class oahSubGroup : public oahElement
{
  public:

    // data types
    // ------------------------------------------------------

    enum oahSubGroupDescriptionVisibilityKind {
      kAlwaysShowDescription,
      kHideDescriptionByDefault };

    static string oahSubGroupDescriptionVisibilityKindAsString (
      oahSubGroupDescriptionVisibilityKind
        oahSubGroupDescriptionVisibilityKind);

  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<oahSubGroup> create (
      string oahSubGroupHelpHeader,
      string oahSubGroupShortName,
      string oahSubGroupLongName,
      string oahSubGroupDescription,
      oahSubGroupDescriptionVisibilityKind
             oahSubGroupDescriptionVisibilityKind,
      S_oahGroup
             oahGroupUpLink);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    oahSubGroup (
      string oahSubGroupHelpHeader,
      string oahSubGroupShortName,
      string oahSubGroupLongName,
      string oahSubGroupDescription,
      oahSubGroupDescriptionVisibilityKind
             oahSubGroupDescriptionVisibilityKind,
      S_oahGroup
             oahGroupUpLink);

    virtual ~oahSubGroup ();

  public:

    // set and get
    // ------------------------------------------------------

    void                  setOptionsGroupUpLink (
                            S_oahGroup oahGroup)
                              {
                                fOptionsGroupUpLink =
                                  oahGroup;
                              }

    S_oahGroup        getOptionsGroupUpLink () const
                              { return fOptionsGroupUpLink; }

    string                getOptionsSubGroupHelpHeader () const
                              { return fOptionsSubGroupHelpHeader; }

    oahSubGroupDescriptionVisibilityKind
                          getOptionsSubGroupIsHiddenByDefault () const
                              {
                                return
                                  fOptionsSubGroupDescriptionVisibilityKind;
                              }

    // services
    // ------------------------------------------------------

    void                  underlineHeader (ostream& os) const;

    void                  registerOptionsSubGroupInHandler (
                            S_oahHandler oahHandler);

    void                  appendOptionsItem (
                            S_oahAtom oahAtom);

    S_oahElement      fetchOptionElement (
                            string optiontElementName);

    // print
    // ------------------------------------------------------

    void                  print (ostream& os) const;

    void                  printHelp (ostream& os) const;

    void                  printOptionsSubGroupForcedHelp (
                            ostream& os) const;

    void                  printOptionsItemForcedHelp (
                            ostream&         os,
                            S_oahAtom targetOptionsItem) const;

    void                  printHelpSummary (ostream& os) const;

    void                  printSpecificSubGroupHelp (
                            ostream& os,
                            S_oahSubGroup
                                     oahSubGroup) const;

    void                  printOptionsValues (
                            ostream& os,
                            int      valueFieldWidth) const;

  private:

    // fields
    // ------------------------------------------------------

    S_oahGroup        fOptionsGroupUpLink;

    string                fOptionsSubGroupHelpHeader;

    oahSubGroupDescriptionVisibilityKind
                          fOptionsSubGroupDescriptionVisibilityKind;

    list<S_oahAtom>   fOptionsSubGroupItemsList;
};
typedef SMARTP<oahSubGroup> S_oahSubGroup;
EXP ostream& operator<< (ostream& os, const S_oahSubGroup& elt);

// options groups
//_______________________________________________________________________________
class oahGroup : public oahElement
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<oahGroup> create (
      string           oahGroupHelpHeader,
      string           optionGroupShortName,
      string           optionGroupLongName,
      string           optionGroupDescription,
      S_oahHandler oahHandlerUpLink);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    oahGroup (
      string           oahGroupHelpHeader,
      string           optionGroupShortName,
      string           optionGroupLongName,
      string           optionGroupDescription,
      S_oahHandler oahHandlerUpLink);

    virtual ~oahGroup ();

  public:

    // set and get
    // ------------------------------------------------------

    void                  setOptionsHandlerUpLink (
                            S_oahHandler oahHandler);

    S_oahHandler      getOptionsHandlerUpLink () const
                              { return fOptionsHandlerUpLink; }

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
                            S_oahHandler oahHandler);

    void                  appendOptionsSubGroup (
                            S_oahSubGroup oahSubGroup);

    S_oahElement      fetchOptionElement (
                            string optiontElementName);

    virtual S_optionsValuedItem
                          handleOptionsItem (
                            ostream&      os,
                            S_oahAtom item);

    virtual void          handleOptionsItemValue (
                            ostream&      os,
                            S_oahAtom item,
                            string        theString);

    // print
    // ------------------------------------------------------

    void                  print (ostream& os) const;

    void                  printHelp (ostream& os) const;

    void                  printOptionsSubGroupForcedHelp (
                            ostream&             os,
                            S_oahSubGroup targetOptionsSubGroup) const;

    void                  printOptionsItemForcedHelp (
                            ostream&          os,
                            S_oahSubGroup targetOptionsSubGroup,
                            S_oahAtom     targetOptionsItem) const;

    void                  printHelpSummary (ostream& os) const;

    void                  printSpecificSubGroupHelp (
                            ostream& os,
                            S_oahSubGroup
                                     oahSubGroup) const;

    void                  printOptionsValues (
                            ostream& os,
                            int      valueFieldWidth) const;

  protected:

    // upLink
    // ------------------------------------------------------

    S_oahHandler      fOptionsHandlerUpLink;

  private:

    // fields
    // ------------------------------------------------------

    string                fOptionsGroupHelpHeader;

    list<S_oahSubGroup>
                          fOptionsGroupSubGroupsList;
};
typedef SMARTP<oahGroup> S_oahGroup;
EXP ostream& operator<< (ostream& os, const S_oahGroup& elt);

// options prefixes
//______________________________________________________________________________
class optionsPrefix;
typedef SMARTP<optionsPrefix> S_optionsPrefix;

class optionsPrefix : public smartable
/*
An options prefix 'trace' --> 'trace-' allows:
  -trace=abc,def,gh
to be developped into :
  -trace-abc -trace-def -trace-gh
*/
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<optionsPrefix> create (
      string optionsPrefixName,
      string optionsPrefixErsatz,
      string optionsPrefixDescription);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    optionsPrefix (
      string optionsPrefixName,
      string optionsPrefixErsatz,
      string optionsPrefixDescription);

    virtual ~optionsPrefix ();

  public:

    // set and get
    // ------------------------------------------------------

    string                getOptionsPrefixName () const
                              { return fOptionsPrefixName; }

    string                getOptionsPrefixErsatz () const
                              { return fOptionsPrefixErsatz; }

    string                getOptionsPrefixDescription () const
                              { return fOptionsPrefixDescription; }

  public:

    // public services
    // ------------------------------------------------------

  private:

    // private services
    // ------------------------------------------------------

    string                optionsPrefixNames () const;
    string                optionsPrefixNamesInColumns (
                            int subGroupsShortNameFieldWidth) const;

    string                optionsPrefixNamesBetweenParentheses () const;
    string                optionsPrefixNamesInColumnsBetweenParentheses (
                            int subGroupsShortNameFieldWidth) const;

    string                operator () () const
                              { return fOptionsPrefixErsatz; }

/* JMI
    S_optionsPrefix       fetchOptionElement (
                            string optiontElementName);
                            */

  public:

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

    string                fOptionsPrefixName;
    string                fOptionsPrefixErsatz;
    string                fOptionsPrefixDescription;
};
EXP ostream& operator<< (ostream& os, const S_optionsPrefix& elt);

// options handlers
//_______________________________________________________________________________
class EXP oahHandler : public oahElement
{
  public:

    // creation
    // ------------------------------------------------------
/* JMI this a pure virtual class
    static SMARTP<oahHandler> create (
      string           oahHandlerHelpHeader,
      string           oahHandlerValuesHeader,
      string           optionHandlerHelpShortName,
      string           optionHandlerHelpLongName,
      string           optionHandlerHelpSummaryShortName,
      string           optionHandlerHelpSummaryLongName,
      string           optionHandlerPreamble,
      string           optionHandlerDescription,
      indentedOstream& oahHandlerlogIOstream);
*/

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    oahHandler (
      string           oahHandlerHelpHeader,
      string           oahHandlerValuesHeader,
      string           optionHandlerHelpShortName,
      string           optionHandlerHelpLongName,
      string           optionHandlerHelpSummaryShortName,
      string           optionHandlerHelpSummaryLongName,
      string           optionHandlerPreamble,
      string           optionHandlerDescription,
      indentedOstream& oahHandlerlogIOstream);

    virtual ~oahHandler ();

  private:

    // initialization
    // ------------------------------------------------------

    virtual void          initializeOptionsHandler (
                            string executableName) = 0;

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
                          getOptionsHandlerlogIOstream ()
                              { return fOptionsHandlerlogIOstream; }

    string                getExecutableName () const
                              { return fExecutableName; }

    const list<S_oahElement>&
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

    int                   getMaximumVariableDisplayNameWidth () const
                              { return fMaximumVariableDisplayNameWidth; }

  public:

    // public services
    // ------------------------------------------------------

    void                  appendOptionsPrefixToHandler (
                            S_optionsPrefix prefix);

    void                  appendOptionsGroupToHandler (
                            S_oahGroup oahGroup);

    void                  registerOptionsElementInHandler (
                            S_oahElement oahElement);

    void                  registerOptionsHandlerInItself ();

    S_optionsPrefix       fetchOptionsPrefixFromMap (
                            string oahElementName) const;

    S_oahElement      fetchOptionsElementFromMap (
                            string oahElementName) const;

    const vector<string>  decipherOptionsAndArguments (
                            int   argc,
                            char* argv[]);

  public:

    // print
    // ------------------------------------------------------

    void                  print (ostream& os) const;

    void                  printHelp (ostream& os) const;

    void                  printHelpSummary (ostream& os) const;
    void                  printHelpSummary () const
                              {
                                printHelpSummary (
                                  fOptionsHandlerlogIOstream);
                              }

    void                  printSpecificSubGroupHelp (
                            ostream&          os,
                            S_oahSubGroup oahSubGroup) const;

    void                  printSpecificItemHelp (
                            ostream& os,
                            string   oahAtomName) const;

    void                  printAllOptionsValues (
                            ostream& os) const;

    bool                  getOptionsHandlerFoundAHelpItem () const
                              { return fOptionsHandlerFoundAHelpItem; }

    void                  setOptionsHandlerFoundAHelpItem ()
                              { fOptionsHandlerFoundAHelpItem = true; }

  private:

    // private services
    // ------------------------------------------------------

    string                helpNamesBetweenParentheses () const; // JMI ???

    void                  registerOptionsNamesInHandler (
                            string           optionShortName,
                            string           optionLongName,
                            S_oahElement oahElement);

    void                  printKnownOptionsPrefixes () const;
    void                  printKnownOptionsElements () const;

    S_oahElement      fetchOptionElement (
                            string optiontElementName);

    void                  handleOptionsItemName (
                            string oahAtomName);

    void                  handleOptionsHandlerItemName (
                            S_oahHandler handler,
                            string           oahAtomName);

    void                  handleOptionsGroupItemName (
                            S_oahGroup group,
                            string         oahAtomName);

    void                  handleOptionsSubGroupItemName (
                            S_oahSubGroup subGroup,
                            string            oahAtomName);

    void                  handleOptionsItemItemName (
                            S_oahAtom item,
                            string        oahAtomName);

    void                  handleOptionsHelpUsageItemName (
                            S_oahHelpUsageAtom helpUsageItem,
                            string                 oahAtomName);

    void                  handleOptionsHelpSummaryItemName (
                            S_oahHelpSummaryAtom helpSummaryItem,
                            string                   oahAtomName);

    void                  handleOptionsCombinedBooleanItemsItemName (
                            S_oahCombinedBooleansAtom combinedBooleanItemsItem,
                            string                     oahAtomName);

    void                  handleOptionsBooleanItemItemName (
                            S_oahBooleanAtom booleanItem,
                            string               oahAtomName);

    void                  handleOptionsTwoBooleansItemItemName (
                            S_oahTwoBooleansAtom twoBooleansItem,
                            string                   oahAtomName);

    void                  handleOptionsThreeBooleansItemItemName (
                            S_oahThreeBooleansAtom threeBooleansItem,
                            string                     oahAtomName);

    void                  handleOptionsItemHelpItemName (
                            S_oahAtomHelpItem itemHelpItem,
                            string                oahAtomName);

    void                  handleOptionsIntegerItemItemName (
                            S_optionsIntegerItem integerItem,
                            string               oahAtomName);

    void                  handleOptionsFloatItemItemName (
                            S_optionsFloatItem floatItem,
                            string             oahAtomName);

    void                  handleOptionsStringItemItemName (
                            S_optionsStringItem stringItem,
                            string              oahAtomName);

    void                  handleOptionsRationalItemItemName (
                            S_optionsRationalItem rationalItem,
                            string              oahAtomName);

    void                  handleOptionsNumbersSetItemItemName (
                            S_optionsNumbersSetItem numbersSetItem,
                            string                  oahAtomName);

    void                  handleOptionsItemValueOrArgument (
                            string theString);

    void                  checkMissingPendingOptionsValuedItemValue (
                            string context);

    void                  handleOptionsItemHelpValue (
                            S_oahAtomHelpItem itemHelpItem,
                            string                theString);

    void                  handleOptionsItemIntegerValue (
                            S_optionsIntegerItem integerItem,
                            string               theString);

    void                  handleOptionsItemFloatValue (
                            S_optionsFloatItem floatItem,
                            string             theString);

    void                  handleOptionsItemStringValue (
                            S_optionsStringItem stringItem,
                            string              theString);

    void                  handleOptionsItemRationalValue (
                            S_optionsRationalItem rationalItem,
                            string                theString);

    void                  handleOptionsItemNumbersSetValue (
                            S_optionsNumbersSetItem numbersSetItem,
                            string                  theString);

    virtual void          checkOptionsAndArguments () = 0;

  protected:

    // fields
    // ------------------------------------------------------

    string                fOptionsHandlerHelpHeader;
    string                fOptionsHandlerValuesHeader;

    string                fOptionHandlerHelpSummaryShortName;
    string                fOptionHandlerHelpSummaryLongName;

    string                fOptionHandlerPreamble;

    S_optionsValuedItem   fPendingOptionsValuedItem;

    vector<string>        fArgumentsVector;

    map<string, S_optionsPrefix>
                          fOptionsPrefixesMap;

    list<S_oahGroup>  fOptionsHandlerOptionsGroupsList;

    map<string, S_oahElement>
                          fOptionsElementsMap;

    int                   fMaximumSubGroupsHelpHeadersSize;

    int                   fMaximumShortNameWidth;
    int                   fMaximumLongNameWidth;

    int                   fMaximumVariableDisplayNameWidth;

    list<S_oahElement>
                          fCommandOptionsElements;

    string                fExecutableName;

    string                fCommandLineWithShortOptions;
    string                fCommandLineWithLongOptions;

    indentedOstream&      fOptionsHandlerlogIOstream;

    // this is needed to exit if the executable is launched with help items,
    // i.e. items that are only used to display help to the user
    bool                  fOptionsHandlerFoundAHelpItem;
};
typedef SMARTP<oahHandler> S_oahHandler;
EXP ostream& operator<< (ostream& os, const S_oahHandler& elt);


}


#endif
