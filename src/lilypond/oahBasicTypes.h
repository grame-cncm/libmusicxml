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
      string shortName,
      string longName,
      string description);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    oahElement (
      string shortName,
      string longName,
      string description);

    virtual ~oahElement ();

  public:

    // set and get
    // ------------------------------------------------------

    string                getShortName () const
                              { return fShortName; }

    string                getLongName () const
                              { return fLongName; }

    string                getDescription () const
                              { return fDescription; }

    void                  setOptionsElementIsHidden ()
                              { fIsHidden = true; }

    bool                  getIsHidden () const
                              { return fIsHidden; }

    // services
    // ------------------------------------------------------

    string                fetchNames () const;
    string                fetchNamesInColumns (
                            int subGroupsShortNameFieldWidth) const;

    string                fetchNamesBetweenParentheses () const;
    string                fetchNamesInColumnsBetweenParentheses (
                            int subGroupsShortNameFieldWidth) const;

    virtual int           fetchVariableNameLength () const
                              { return 0; }

/* JMI ???
    string                operator () () const
                              { return fDescription; }
*/

    S_oahElement          fetchElementByName (
                            string name);

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

    bool                  fIsHidden;
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
      string shortName,
      string longName,
      string description);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    oahAtom (
      string shortName,
      string longName,
      string description);

    virtual ~oahAtom ();

  public:

    // set and get
    // ------------------------------------------------------

    void                  setSubGroupUpLink (
                            S_oahSubGroup subGroup);

    S_oahSubGroup         getSubGroupUpLink () const
                              { return fSubGroupUpLink; }

    // services
    // ------------------------------------------------------

    void                  registerOptionsItemInHandler (
                            S_oahHandler handler);

    // print
    // ------------------------------------------------------

    void                  print (ostream& os) const;

    virtual void          printOptionsValues (
                            ostream& os,
                            int      valueFieldWidth) const;

  protected:

    // fields
    // ------------------------------------------------------

    S_oahSubGroup         fSubGroupUpLink;
};
typedef SMARTP<oahAtom> S_oahAtom;
EXP ostream& operator<< (ostream& os, const S_oahAtom& elt);

/* JMI
// pointers to options items methods
//______________________________________________________________________________
typedef void (oahAtom::*oahAtomMethodPtrType)();

  It's worth noting that, as of C++11, you could write this expression
  as a more legible using statement:
    using oahAtomMethodPtrType = void (oahAtom::*)();
*/

// options help usage items
//______________________________________________________________________________
class oahOptionsUsageAtom : public oahAtom
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<oahOptionsUsageAtom> create (
      string shortName,
      string longName,
      string description);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    oahOptionsUsageAtom (
      string shortName,
      string longName,
      string description);

    virtual ~oahOptionsUsageAtom ();

  public:

    // set and get
    // ------------------------------------------------------

    // services
    // ------------------------------------------------------

    // print
    // ------------------------------------------------------

    void                  print (ostream& os) const;

    void                  printOptionsUsage (ostream& os) const;

    void                  printOptionsValues (
                            ostream& os,
                            int      valueFieldWidth) const;

  private:

    // fields
    // ------------------------------------------------------
};
typedef SMARTP<oahOptionsUsageAtom> S_oahOptionsUsageAtom;
EXP ostream& operator<< (ostream& os, const S_oahOptionsUsageAtom& elt);

// options help summary items
//______________________________________________________________________________
class oahOptionsSummaryAtom : public oahAtom
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<oahOptionsSummaryAtom> create (
      string shortName,
      string longName,
      string description);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    oahOptionsSummaryAtom (
      string shortName,
      string longName,
      string description);

    virtual ~oahOptionsSummaryAtom ();

  public:

    // set and get
    // ------------------------------------------------------

    // services
    // ------------------------------------------------------

    // print
    // ------------------------------------------------------

    void                  print (ostream& os) const;

    void                  printOptionsSummary (ostream& os) const;

    void                  printOptionsValues (
                            ostream& os,
                            int      valueFieldWidth) const;

  private:

    // fields
    // ------------------------------------------------------
};
typedef SMARTP<oahOptionsSummaryAtom> S_oahOptionsSummaryAtom;
EXP ostream& operator<< (ostream& os, const S_oahOptionsSummaryAtom& elt);

// options items with variable name
//______________________________________________________________________________
class oahAtomWithVariableName : public oahAtom
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<oahAtomWithVariableName> create (
      string shortName,
      string longName,
      string description,
      string variableName);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    oahAtomWithVariableName (
      string shortName,
      string longName,
      string description,
      string variableName);

    virtual ~oahAtomWithVariableName ();

  public:

    // set and get
    // ------------------------------------------------------

    string                getVariableName () const
                              { return fVariableName; }

    // services
    // ------------------------------------------------------

    virtual int           fetchVariableNameLength () const
                              { return fVariableName.size (); }

    // print
    // ------------------------------------------------------

    void                  print (ostream& os) const;

    void                  printOptionsValues (
                            ostream& os,
                            int      valueFieldWidth) const;

  protected:

    // fields
    // ------------------------------------------------------

    string                fVariableName;
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
      string shortName,
      string longName,
      string description,
      string variableName,
      bool&  booleanVariable);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    oahBooleanAtom (
      string shortName,
      string longName,
      string description,
      string variableName,
      bool&  booleanVariable);

    virtual ~oahBooleanAtom ();

  public:

    // set and get
    // ------------------------------------------------------

    void                  setBooleanItemVariableValue (
                            bool value)
                              { fBooleanVariable = value; }

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

    bool&                 fBooleanVariable;
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
      string shortName,
      string longName,
      string description,
      string variableName,
      bool&  booleanVariable,
      bool&  booleanSecondaryVariable);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    oahTwoBooleansAtom (
      string shortName,
      string longName,
      string description,
      string variableName,
      bool&  booleanVariable,
      bool&  booleanSecondaryVariable);

    virtual ~oahTwoBooleansAtom ();

  public:

    // set and get
    // ------------------------------------------------------

    void                  setTwoBooleansItemVariableValue (
                            bool value)
                              {
                                fBooleanVariable =
                                  value;
                                fBooleanSecondaryVariable =
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

    bool&                 fBooleanVariable;

    bool&                 fBooleanSecondaryVariable;
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
      string shortName,
      string longName,
      string description,
      string variableName,
      bool&  booleanVariable,
      bool&  booleanSecondaryVariable,
      bool&  booleanTertiaryVariable);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    oahThreeBooleansAtom (
      string shortName,
      string longName,
      string description,
      string variableName,
      bool&  booleanVariable,
      bool&  booleanSecondaryVariable,
      bool&  booleanTertiaryVariable);

    virtual ~oahThreeBooleansAtom ();

  public:

    // set and get
    // ------------------------------------------------------

    void                  setThreeBooleansItemVariableValue (
                            bool value)
                              {
                                fBooleanVariable =
                                  value;
                                fBooleanSecondaryVariable =
                                  value;
                                fBooleanTertiaryVariable =
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

    bool&                 fBooleanVariable;

    bool&                 fBooleanSecondaryVariable;
    bool&                 fBooleanTertiaryVariable;
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
      string shortName,
      string longName,
      string description,
      string variableName,
      bool&  booleanVariable);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    oahCombinedBooleansAtom (
      string shortName,
      string longName,
      string description,
      string variableName,
      bool&  booleanVariable);

    virtual ~oahCombinedBooleansAtom ();

  public:

    // set and get
    // ------------------------------------------------------

    const list<S_oahBooleanAtom>&
                          getBooleanAtomsList ()
                              { return fBooleanAtomsList; }

    // services
    // ------------------------------------------------------

    void                  addBooleanAtom (
                            S_oahBooleanAtom booleanAtom);

    void                  addBooleanAtomByName (
                            string name);

    void                  setBooleanVariablesValues (
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

    bool&                 fBooleanVariable;

    list<S_oahBooleanAtom>
                          fBooleanAtomsList;
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

    enum oahValuedAtomKind { // JMI ???
      kOptionsItemHasNoArgument,
      kOptionsItemHasARequiredArgument,
      kOptionsItemHasAnOptionsArgument };

    static string oahAtomKindAsString (
      oahValuedAtomKind oahAtomKind);

    // creation
    // ------------------------------------------------------

    static SMARTP<oahValuedAtom> create (
      string shortName,
      string longName,
      string description,
      string valueSpecification,
      string variableName);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    oahValuedAtom (
      string shortName,
      string longName,
      string description,
      string valueSpecification,
      string variableName);

    virtual ~oahValuedAtom ();

  public:

    // set and get
    // ------------------------------------------------------

    oahValuedAtomKind
                          getValuedAtomKind () const
                              { return fValuedAtomKind; }

    string                getValueSpecification () const
                              { return fValueSpecification; }

    void                  setValueIsOptional ()
                              { fValueIsOptional = true; }

    bool                  getValueIsOptional () const
                              { return fValueIsOptional; }

    // services
    // ------------------------------------------------------

    // print
    // ------------------------------------------------------

    virtual void          printValuedAtomEssentials (
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

    oahValuedAtomKind     fValuedAtomKind; // JMI

    string                fValueSpecification;

    bool                  fValueIsOptional; // not yet supported JMI
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
      string             shortName,
      string             longName,
      string             description,
      string             valueSpecification,
      string             variableName);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    oahElementHelpAtom (
      string             shortName,
      string             longName,
      string             description,
      string             valueSpecification,
      string             variableName);

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

    string                fValueSpecification;
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
      string             shortName,
      string             longName,
      string             description,
      string             valueSpecification,
      string             variableName,
      int&               integerVariable);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    oahIntegerAtom (
      string             shortName,
      string             longName,
      string             description,
      string             valueSpecification,
      string             variableName,
      int&               integerVariable);

  protected:


    virtual ~oahIntegerAtom ();

  public:

    // set and get
    // ------------------------------------------------------

    void                  setIntegerVariableValue (
                            int value)
                              { fIntegerVariable = value; }

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

    int&                  fIntegerVariable;
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
      string             shortName,
      string             longName,
      string             description,
      string             valueSpecification,
      string             variableName,
      float&             floatVariable);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    oahFloatAtom (
      string             shortName,
      string             longName,
      string             description,
      string             valueSpecification,
      string             variableName,
      float&             floatVariable);

    virtual ~oahFloatAtom ();

  public:

    // set and get
    // ------------------------------------------------------

    void                  setFloatItemVariableValue (
                            float value)
                              { fFloatVariable = value; }

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

    float&                fFloatVariable;
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
      string             shortName,
      string             longName,
      string             description,
      string             valueSpecification,
      string             variableName,
      string&            stringVariable);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    oahStringAtom (
      string             shortName,
      string             longName,
      string             description,
      string             valueSpecification,
      string             variableName,
      string&            stringVariable);

    virtual ~oahStringAtom ();

  public:

    // set and get
    // ------------------------------------------------------

    void                  setStringItemVariableValue (
                            string value)
                              { fStringVariable = value; }

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

    string&               fStringVariable;
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
      string             shortName,
      string             longName,
      string             description,
      string             valueSpecification,
      string             variableName,
      rational&          rationalVariable);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    oahRationalAtom (
      string             shortName,
      string             longName,
      string             description,
      string             valueSpecification,
      string             variableName,
      rational&          rationalVariable);

    virtual ~oahRationalAtom ();

  public:

    // set and get
    // ------------------------------------------------------

    void                  setRationalItemVariableValue (
                            rational value)
                              { fRationalVariable = value;  }

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

    rational&             fRationalVariable;
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
      string             shortName,
      string             longName,
      string             description,
      string             valueSpecification,
      string             variableName,
      set<int>&          numbersSetVariable);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    oahNumbersSetAtom (
      string             shortName,
      string             longName,
      string             description,
      string             valueSpecification,
      string             variableName,
      set<int>&          numbersSetVariable);

    virtual ~oahNumbersSetAtom ();

  public:

    // set and get
    // ------------------------------------------------------

    void                  setNumbersSetItemVariableValue (
                            set<int> value)
                              { fNumbersSetVariable = value; }

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

    set<int>&             fNumbersSetVariable;
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

    enum oahSubGroupVisibilityKind {
      kSubGroupVisibilityAlways,
      kSubGroupVisibilityHiddenByDefault };

    static string subGroupVisibilityKindAsString (
      oahSubGroupVisibilityKind subGroupVisibilityKind);

  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<oahSubGroup> create (
      string                    header,
      string                    shortName,
      string                    longName,
      string                    description,
      oahSubGroupVisibilityKind subGroupVisibilityKind,
      S_oahGroup                groupUpLink);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    oahSubGroup (
      string                    header,
      string                    shortName,
      string                    longName,
      string                    description,
      oahSubGroupVisibilityKind subGroupVisibilityKind,
      S_oahGroup                groupUpLink);

    virtual ~oahSubGroup ();

  public:

    // set and get
    // ------------------------------------------------------

    void                  setGroupUpLink (
                            S_oahGroup oahGroup)
                              { fGroupUpLink = oahGroup; }

    S_oahGroup            getGroupUpLink () const
                              { return fGroupUpLink; }

    string                getHeader () const
                              { return fHeader; }

    oahSubGroupVisibilityKind
                          getSubGroupVisibilityKind () const
                              { return fSubGroupVisibilityKind; }

    // services
    // ------------------------------------------------------

    void                  underlineHeader (ostream& os) const;

    void                  registerSubGroupInHandler (
                            S_oahHandler handler);

    void                  appendAtom (
                            S_oahAtom oahAtom);

    S_oahElement          fetchElementByName (
                            string name);

    // print
    // ------------------------------------------------------

    void                  print (ostream& os) const;

    void                  printHelp (ostream& os) const;

    void                  printSubGroupForcedHelp (
                            ostream& os) const;

    void                  printAtomForcedHelp (
                            ostream&  os,
                            S_oahAtom targetAtom) const;

    void                  printOptionsSummary (ostream& os) const;

    void                  printSpecificSubGroupHelp (
                            ostream&      os,
                            S_oahSubGroup subGroup) const;

    void                  printOptionsValues (
                            ostream& os,
                            int      valueFieldWidth) const;

  private:

    // fields
    // ------------------------------------------------------

    S_oahGroup            fGroupUpLink;

    string                fHeader;

    oahSubGroupVisibilityKind
                          fSubGroupVisibilityKind;

    list<S_oahAtom>       fSubGroupItemsList;
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
                            S_oahHandler handler);

    S_oahHandler          getOptionsHandlerUpLink () const
                              { return fHandlerUpLink; }

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
                            S_oahHandler handler);

    void                  appendSubGroup (
                            S_oahSubGroup subGroup);

    S_oahElement          fetchElementByName (
                            string name);

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

    void                  printSubGroupForcedHelp (
                            ostream&      os,
                            S_oahSubGroup targetSubGroup) const;

    void                  printGroupForcedHelp (
                            ostream&      os,
                            S_oahSubGroup targetSubGroup,
                            S_oahAtom     targetAtom) const;

    void                  printOptionsSummary (ostream& os) const;

    void                  printSpecificSubGroupHelp (
                            ostream&      os,
                            S_oahSubGroup subGroup) const;

    void                  printOptionsValues (
                            ostream& os,
                            int      valueFieldWidth) const;

  protected:

    // upLink
    // ------------------------------------------------------

    S_oahHandler          fHandlerUpLink;

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
    S_oahPrefix           fetchElementByName (
                            string name);
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
                              { return fHandlerHelpHeader; }

    string                getOptionsHandlerValuesHeader () const
                              { return fHandlerValuesHeader; }

    string                getOptionHandlerHelpSummaryShortName () const
                              { return fHandlerHelpSummaryShortName; }

    string                getOptionHandlerHelpSummaryLongName () const
                              { return fHandlerHelpSummaryLongName; }

    string                getOptionHandlerPreamble () const
                              { return fHandlerPreamble; }

    const indentedOstream&
                          getOptionsHandlerlogIOstream ()
                              { return fHandlerLogIOstream; }

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

    int                   getMaximumVariableNameWidth () const
                              { return fMaximumVariableNameWidth; }

  public:

    // public services
    // ------------------------------------------------------

    void                  appendPrefixToHandler (
                            S_oahPrefix prefix);

    void                  appendGroupToHandler (
                            S_oahGroup oahGroup);

    void                  registerElementInHandler (
                            S_oahElement element);

    void                  registerHandlerInItself ();

    S_oahPrefix           fetchPrefixFromMap (
                            string oahElementName) const;

    S_oahElement          fetchElementFromMap (
                            string oahElementName) const;

    const vector<string>  decipherOptionsAndArguments (
                            int   argc,
                            char* argv[]);

  public:

    // print
    // ------------------------------------------------------

    void                  print (ostream& os) const;

    void                  printHelp (ostream& os) const;

    void                  printOptionsSummary (ostream& os) const;
    void                  printOptionsSummary () const
                              {
                                printOptionsSummary (
                                  fHandlerLogIOstream);
                              }

    void                  printSpecificSubGroupHelp (
                            ostream&      os,
                            S_oahSubGroup subGroup) const;

    void                  printSpecificElementHelp (
                            ostream& os,
                            string   name) const;

    void                  printAllOptionsValues (
                            ostream& os) const;

    bool                  getOptionsHandlerFoundAHelpItem () const
                              { return fHandlerFoundAHelpItem; }

    void                  setOptionsHandlerFoundAHelpItem ()
                              { fHandlerFoundAHelpItem = true; }

  private:

    // private services
    // ------------------------------------------------------

    string                helpNamesBetweenParentheses () const; // JMI ???

    void                  registerNamesInHandler (
                            string       optionShortName,
                            string       optionLongName,
                            S_oahElement element);

    void                  printKnownPrefixes () const;
    void                  printKnownElements () const;

    S_oahElement          fetchElementByName (
                            string name);

    void                  handleElementName (
                            string name);

    void                  handleOptionsHandlerItemName (
                            S_oahHandler handler,
                            string       oahAtomName);

    void                  handleOptionsGroupItemName (
                            S_oahGroup group,
                            string     oahAtomName);

    void                  handleSubGroupItemName (
                            S_oahSubGroup subGroup,
                            string        oahAtomName);

    void                  handleOptionsItemItemName (
                            S_oahAtom item,
                            string    oahAtomName);

    void                  handleOptionsHelpUsageItemName (
                            S_oahOptionsUsageAtom helpUsageItem,
                            string             oahAtomName);

    void                  handleOptionsHelpSummaryItemName (
                            S_oahOptionsSummaryAtom helpSummaryItem,
                            string               oahAtomName);

    void                  handleOptionsCombinedBooleanItemsItemName (
                            S_oahCombinedBooleansAtom combinedBooleanItemsItem,
                            string                    oahAtomName);

    void                  handleOptionsBooleanItemItemName (
                            S_oahBooleanAtom booleanItem,
                            string           oahAtomName);

    void                  handleOptionsTwoBooleansItemItemName (
                            S_oahTwoBooleansAtom twoBooleansItem,
                            string               oahAtomName);

    void                  handleOptionsThreeBooleansItemItemName (
                            S_oahThreeBooleansAtom threeBooleansItem,
                            string                 oahAtomName);

    void                  handleOptionsItemHelpItemName (
                            S_oahElementHelpAtom itemHelpItem,
                            string               oahAtomName);

    void                  handleOptionsIntegerItemItemName (
                            S_oahIntegerAtom integerItem,
                            string           oahAtomName);

    void                  handleOptionsFloatItemItemName (
                            S_oahFloatAtom floatItem,
                            string         oahAtomName);

    void                  handleOptionsStringItemItemName (
                            S_oahStringAtom stringItem,
                            string          oahAtomName);

    void                  handleOptionsRationalItemItemName (
                            S_oahRationalAtom rationalItem,
                            string            oahAtomName);

    void                  handleOptionsNumbersSetItemItemName (
                            S_oahNumbersSetAtom numbersSetItem,
                            string              oahAtomName);

    void                  handleOptionsItemValueOrArgument (
                            string theString);

    void                  checkMissingPendingOptionsValuedAtomValue (
                            string context);

    void                  handleOptionsItemHelpValue (
                            S_oahElementHelpAtom itemHelpItem,
                            string               theString);

    void                  handleOptionsItemIntegerValue (
                            S_oahIntegerAtom integerItem,
                            string           theString);

    void                  handleOptionsItemFloatValue (
                            S_oahFloatAtom floatItem,
                            string         theString);

    void                  handleOptionsItemStringValue (
                            S_oahStringAtom stringItem,
                            string          theString);

    void                  handleOptionsItemRationalValue (
                            S_oahRationalAtom rationalItem,
                            string            theString);

    void                  handleOptionsItemNumbersSetValue (
                            S_oahNumbersSetAtom numbersSetItem,
                            string              theString);

    virtual void          checkOptionsAndArguments () = 0;

  protected:

    // fields
    // ------------------------------------------------------

    string                fHandlerHelpHeader;
    string                fHandlerValuesHeader;

    string                fHandlerHelpSummaryShortName;
    string                fHandlerHelpSummaryLongName;

    string                fHandlerPreamble;

    S_oahValuedAtom       fPendingValuedAtom;

    vector<string>        fArgumentsVector;

    map<string, S_oahPrefix>
                          fPrefixesMap;

    list<S_oahGroup>      fHandlerGroupsList;

    map<string, S_oahElement>
                          fElementsMap;

    int                   fMaximumSubGroupsHelpHeadersSize;

    int                   fMaximumShortNameWidth;
    int                   fMaximumLongNameWidth;

    int                   fMaximumVariableNameWidth;

    list<S_oahElement>    fCommandOptionsElements;

    string                fExecutableName;

    string                fCommandLineWithShortOptions;
    string                fCommandLineWithLongOptions;

    indentedOstream&      fHandlerLogIOstream;

    // this is needed to exit if the executable is launched
    // with one or more help options,
    // i.e. items that are only used to display help to the user
    bool                  fHandlerFoundAHelpItem;
};
typedef SMARTP<oahHandler> S_oahHandler;
EXP ostream& operator<< (ostream& os, const S_oahHandler& elt);


}


#endif
