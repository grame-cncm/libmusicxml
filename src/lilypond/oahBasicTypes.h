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

class oahHandler;
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
                              { return fShortName; }

    string                getOptionsElementLongName () const
                              { return fLongName; }

    string                getOptionsElementDescription () const
                              { return fDescription; }

    void                  setOptionsElementIsHidden ()
                              { fHidden = true; }

    bool                  getOptionsElementIsHidden () const
                              { return fHidden; }

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
                              { return fDescription; }

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

    string                fShortName;
    string                fLongName;
    string                fDescription;

    bool                  fHidden;
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
class oahAtomWithVariableName : public oahAtom
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<oahAtomWithVariableName> create (
      string oahAtomShortName,
      string oahAtomLongName,
      string oahAtomDescription,
      string oahAtomVariableDisplayName);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    oahAtomWithVariableName (
      string oahAtomShortName,
      string oahAtomLongName,
      string oahAtomDescription,
      string oahAtomVariableDisplayName);

    virtual ~oahAtomWithVariableName ();

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
typedef SMARTP<oahAtomWithVariableName> S_oahAtomWithVariableName;
EXP ostream& operator<< (ostream& os, const S_oahAtomWithVariableName& elt);

// options boolean items
//______________________________________________________________________________
class oahBooleanAtom : public oahAtomWithVariableName
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
class oahTwoBooleansAtom : public oahAtomWithVariableName
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
class oahThreeBooleansAtom : public oahAtomWithVariableName
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
class oahCombinedBooleansAtom : public oahAtomWithVariableName
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
class oahValuedAtom : public oahAtomWithVariableName
{
  public:

    // data types
    // ------------------------------------------------------

    enum oahValuedAtomKind {
      kOptionsItemHasNoArgument,
      kOptionsItemHasARequiredArgument,
      kOptionsItemHasAnOptionsArgument };

    static string oahAtomKindAsString (
      oahValuedAtomKind oahAtomKind);

    // creation
    // ------------------------------------------------------

    static SMARTP<oahValuedAtom> create (
      string oahAtomShortName,
      string oahAtomLongName,
      string oahAtomDescription,
      string optionsValueSpecification,
      string oahValuedAtomVariableDisplayName);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    oahValuedAtom (
      string oahAtomShortName,
      string oahAtomLongName,
      string oahAtomDescription,
      string optionsValueSpecification,
      string oahValuedAtomVariableDisplayName);

    virtual ~oahValuedAtom ();

  public:

    // set and get
    // ------------------------------------------------------

    oahValuedAtomKind
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

    oahValuedAtomKind fOptionsItemKind;

    string                fOptionsValueSpecification;

    bool                  fOptionsValueIsOptional;
};
typedef SMARTP<oahValuedAtom> S_oahValuedAtom;
EXP ostream& operator<< (ostream& os, const S_oahValuedAtom& elt);

// options items helps items
//______________________________________________________________________________
class oahElementHelpAtom : public oahValuedAtom
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<oahElementHelpAtom> create (
      string             oahAtomShortName,
      string             oahAtomLongName,
      string             oahAtomDescription,
      string             optionsValueSpecification,
      string             optionsHelpItemVariableDisplayName);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    oahElementHelpAtom (
      string             oahAtomShortName,
      string             oahAtomLongName,
      string             oahAtomDescription,
      string             optionsValueSpecification,
      string             optionsHelpItemVariableDisplayName);

  protected:


    virtual ~oahElementHelpAtom ();

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
typedef SMARTP<oahElementHelpAtom> S_oahElementHelpAtom;
EXP ostream& operator<< (ostream& os, const S_oahElementHelpAtom& elt);

// options integer items
//______________________________________________________________________________
class oahIntegerAtom : public oahValuedAtom
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<oahIntegerAtom> create (
      string             oahAtomShortName,
      string             oahAtomLongName,
      string             oahAtomDescription,
      string             optionsValueSpecification,
      string             oahIntegerAtomVariableDisplayName,
      int&               oahIntegerAtomVariable);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    oahIntegerAtom (
      string             oahAtomShortName,
      string             oahAtomLongName,
      string             oahAtomDescription,
      string             optionsValueSpecification,
      string             oahIntegerAtomVariableDisplayName,
      int&               oahIntegerAtomVariable);

  protected:


    virtual ~oahIntegerAtom ();

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
typedef SMARTP<oahIntegerAtom> S_oahIntegerAtom;
EXP ostream& operator<< (ostream& os, const S_oahIntegerAtom& elt);

// options float items
//______________________________________________________________________________
class oahFloatAtom : public oahValuedAtom
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<oahFloatAtom> create (
      string             oahAtomShortName,
      string             oahAtomLongName,
      string             oahAtomDescription,
      string             optionsValueSpecification,
      string             oahFloatAtomVariableDisplayName,
      float&             oahFloatAtomVariable);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    oahFloatAtom (
      string             oahAtomShortName,
      string             oahAtomLongName,
      string             oahAtomDescription,
      string             optionsValueSpecification,
      string             oahFloatAtomVariableDisplayName,
      float&             oahFloatAtomVariable);

    virtual ~oahFloatAtom ();

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
typedef SMARTP<oahFloatAtom> S_oahFloatAtom;
EXP ostream& operator<< (ostream& os, const S_oahFloatAtom& elt);

// options string items
//______________________________________________________________________________
class oahStringAtom : public oahValuedAtom
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<oahStringAtom> create (
      string             oahAtomShortName,
      string             oahAtomLongName,
      string             oahAtomDescription,
      string             optionsValueSpecification,
      string             oahStringAtomVariableDisplayName,
      string&            oahStringAtomVariable);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    oahStringAtom (
      string             oahAtomShortName,
      string             oahAtomLongName,
      string             oahAtomDescription,
      string             optionsValueSpecification,
      string             oahStringAtomVariableDisplayName,
      string&            oahStringAtomVariable);

    virtual ~oahStringAtom ();

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
typedef SMARTP<oahStringAtom> S_oahStringAtom;
EXP ostream& operator<< (ostream& os, const S_oahStringAtom& elt);

// options rational items
//______________________________________________________________________________
class oahRationalAtom : public oahValuedAtom
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<oahRationalAtom> create (
      string             oahAtomShortName,
      string             oahAtomLongName,
      string             oahAtomDescription,
      string             optionsValueSpecification,
      string             oahRationalAtomVariableDisplayName,
      rational&          oahRationalAtomVariable);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    oahRationalAtom (
      string             oahAtomShortName,
      string             oahAtomLongName,
      string             oahAtomDescription,
      string             optionsValueSpecification,
      string             oahRationalAtomVariableDisplayName,
      rational&          oahRationalAtomVariable);

    virtual ~oahRationalAtom ();

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
typedef SMARTP<oahRationalAtom> S_oahRationalAtom;
EXP ostream& operator<< (ostream& os, const S_oahRationalAtom& elt);

// options numbers sets items
//______________________________________________________________________________
class oahNumbersSetAtom : public oahValuedAtom
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<oahNumbersSetAtom> create (
      string             oahAtomShortName,
      string             oahAtomLongName,
      string             oahAtomDescription,
      string             optionsValueSpecification,
      string             oahNumbersSetAtomVariableDisplayName,
      set<int>&          oahNumbersSetAtomVariable);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    oahNumbersSetAtom (
      string             oahAtomShortName,
      string             oahAtomLongName,
      string             oahAtomDescription,
      string             optionsValueSpecification,
      string             oahNumbersSetAtomVariableDisplayName,
      set<int>&          oahNumbersSetAtomVariable);

    virtual ~oahNumbersSetAtom ();

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
typedef SMARTP<oahNumbersSetAtom> S_oahNumbersSetAtom;
EXP ostream& operator<< (ostream& os, const S_oahNumbersSetAtom& elt);

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

    virtual S_oahValuedAtom
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
class oahPrefix;
typedef SMARTP<oahPrefix> S_oahPrefix;

class oahPrefix : public smartable
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

    static SMARTP<oahPrefix> create (
      string oahPrefixName,
      string oahPrefixErsatz,
      string oahPrefixDescription);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    oahPrefix (
      string oahPrefixName,
      string oahPrefixErsatz,
      string oahPrefixDescription);

    virtual ~oahPrefix ();

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

    string                oahPrefixNames () const;
    string                oahPrefixNamesInColumns (
                            int subGroupsShortNameFieldWidth) const;

    string                oahPrefixNamesBetweenParentheses () const;
    string                oahPrefixNamesInColumnsBetweenParentheses (
                            int subGroupsShortNameFieldWidth) const;

    string                operator () () const
                              { return fOptionsPrefixErsatz; }

/* JMI
    S_oahPrefix       fetchOptionElement (
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
EXP ostream& operator<< (ostream& os, const S_oahPrefix& elt);

// options handlers
//_______________________________________________________________________________
class oahHandler : public oahElement
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
                            S_oahPrefix prefix);

    void                  appendOptionsGroupToHandler (
                            S_oahGroup oahGroup);

    void                  registerOptionsElementInHandler (
                            S_oahElement oahElement);

    void                  registerOptionsHandlerInItself ();

    S_oahPrefix       fetchOptionsPrefixFromMap (
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
                            S_oahElementHelpAtom itemHelpItem,
                            string                oahAtomName);

    void                  handleOptionsIntegerItemItemName (
                            S_oahIntegerAtom integerItem,
                            string               oahAtomName);

    void                  handleOptionsFloatItemItemName (
                            S_oahFloatAtom floatItem,
                            string             oahAtomName);

    void                  handleOptionsStringItemItemName (
                            S_oahStringAtom stringItem,
                            string              oahAtomName);

    void                  handleOptionsRationalItemItemName (
                            S_oahRationalAtom rationalItem,
                            string              oahAtomName);

    void                  handleOptionsNumbersSetItemItemName (
                            S_oahNumbersSetAtom numbersSetItem,
                            string                  oahAtomName);

    void                  handleOptionsItemValueOrArgument (
                            string theString);

    void                  checkMissingPendingOptionsValuedItemValue (
                            string context);

    void                  handleOptionsItemHelpValue (
                            S_oahElementHelpAtom itemHelpItem,
                            string                theString);

    void                  handleOptionsItemIntegerValue (
                            S_oahIntegerAtom integerItem,
                            string               theString);

    void                  handleOptionsItemFloatValue (
                            S_oahFloatAtom floatItem,
                            string             theString);

    void                  handleOptionsItemStringValue (
                            S_oahStringAtom stringItem,
                            string              theString);

    void                  handleOptionsItemRationalValue (
                            S_oahRationalAtom rationalItem,
                            string                theString);

    void                  handleOptionsItemNumbersSetValue (
                            S_oahNumbersSetAtom numbersSetItem,
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

    S_oahValuedAtom   fPendingOptionsValuedItem;

    vector<string>        fArgumentsVector;

    map<string, S_oahPrefix>
                          fOptionsPrefixesMap;

    list<S_oahGroup>  fOptionsHandlerOptionsGroupsList;

    map<string, S_oahElement>
                          fElementsMap;

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
