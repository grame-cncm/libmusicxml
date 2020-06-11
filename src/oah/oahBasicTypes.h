/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef ___oahBasicTypes___
#define ___oahBasicTypes___

#include "list"
#include "vector"
#include "map"
#include "set"

#include "smartpointer.h"
#include "rational.h"

#include "tree_browser.h"
#include "visitor.h"

#include "libmusicxml.h"  // for optionsVector

#include "msrMidi.h"

#include "utilities.h"

#include "msrBasicTypes.h"
#include "lpsrBasicTypes.h"

#include "setTraceOahIfDesired.h"

#include "oahElements.h"

using namespace std;

namespace MusicXML2
{

// PRE-declarations for class dependencies
//______________________________________________________________________________
class oahValuedAtom;
typedef SMARTP<oahValuedAtom> S_oahValuedAtom;

class oahSubGroup;
typedef SMARTP<oahSubGroup> S_oahSubGroup;

class oahGroup;
typedef SMARTP<oahGroup> S_oahGroup;

class EXP oahHandler;
typedef SMARTP<oahHandler> S_oahHandler;

//______________________________________________________________________________
class oahAtom : public oahElement
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<oahAtom> create (
      string                      shortName,
      string                      longName,
      string                      description,
      oahElementValueExpectedKind atomValueExpectedKind);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    oahAtom (
      string                      shortName,
      string                      longName,
      string                      description,
      oahElementValueExpectedKind atomValueExpectedKind);

    virtual ~oahAtom ();

  public:

    // set and get
    // ------------------------------------------------------

    void                  setSubGroupUpLink (
                            S_oahSubGroup subGroup);

    S_oahSubGroup         getSubGroupUpLink () const
                              { return fSubGroupUpLink; }

  public:

    // services
    // ------------------------------------------------------

    void                  registerAtomInHandler (
                            S_oahHandler handler);

    void                  registerAtomAsBeingUsed ();

  public:

    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v);
    virtual void          acceptOut (basevisitor* v);

    virtual void          browseData (basevisitor* v);

  public:

    // print
    // ------------------------------------------------------

    void                  print (ostream& os) const;

    virtual void          printAtomOptionsValues (
                            ostream& os,
                            int      valueFieldWidth) const;

  protected:

    // fields
    // ------------------------------------------------------

    S_oahSubGroup         fSubGroupUpLink;
};
typedef SMARTP<oahAtom> S_oahAtom;
EXP ostream& operator<< (ostream& os, const S_oahAtom& elt);

//______________________________________________________________________________
class oahAtomSynonym : public oahAtom
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<oahAtomSynonym> create (
      string    shortName,
      string    longName,
      string    description,
      S_oahAtom originalOahAtom);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    oahAtomSynonym (
      string    shortName,
      string    longName,
      string    description,
      S_oahAtom originalOahAtom);

    virtual ~oahAtomSynonym ();

  public:

    // set and get
    // ------------------------------------------------------

     S_oahAtom            getOriginalOahAtom () const
                              { return fOriginalOahAtom; }

  public:

    // services
    // ------------------------------------------------------

  public:

    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v);
    virtual void          acceptOut (basevisitor* v);

    virtual void          browseData (basevisitor* v);

  public:

    // print
    // ------------------------------------------------------

    void                  print (ostream& os) const;

    void                  printAtomOptionsValues (
                            ostream& os,
                            int      valueFieldWidth) const;

  private:

    // fields
    // ------------------------------------------------------

     S_oahAtom            fOriginalOahAtom;
};
typedef SMARTP<oahAtomSynonym> S_oahAtomSynonym;
EXP ostream& operator<< (ostream& os, const S_oahAtomSynonym& elt);

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

  public:

    // services
    // ------------------------------------------------------

    S_oahValuedAtom       handleOptionUnderName (
                            string   optionName,
                            ostream& os);

  public:

    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v);
    virtual void          acceptOut (basevisitor* v);

    virtual void          browseData (basevisitor* v);

  public:

    // print
    // ------------------------------------------------------

    void                  print (ostream& os) const;

//    void                  printOptionsUsage (ostream& os) const;

    void                  printAtomOptionsValues (
                            ostream& os,
                            int      valueFieldWidth) const;

  private:

    // fields
    // ------------------------------------------------------
};
typedef SMARTP<oahOptionsUsageAtom> S_oahOptionsUsageAtom;
EXP ostream& operator<< (ostream& os, const S_oahOptionsUsageAtom& elt);

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

  public:

    // services
    // ------------------------------------------------------

    S_oahValuedAtom       handleOptionUnderName (
                            string   optionName,
                            ostream& os);

  public:

    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v);
    virtual void          acceptOut (basevisitor* v);

    virtual void          browseData (basevisitor* v);

  public:

    // print
    // ------------------------------------------------------

    void                  print (ostream& os) const;

    void                  printOptionsSummary (ostream& os) const;

    void                  printAtomOptionsValues (
                            ostream& os,
                            int      valueFieldWidth) const;

  private:

    // fields
    // ------------------------------------------------------
};
typedef SMARTP<oahOptionsSummaryAtom> S_oahOptionsSummaryAtom;
EXP ostream& operator<< (ostream& os, const S_oahOptionsSummaryAtom& elt);

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

  public:

    // services
    // ------------------------------------------------------

    virtual int           fetchVariableNameLength () const
                              { return fVariableName.size (); }

  public:

    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v);
    virtual void          acceptOut (basevisitor* v);

    virtual void          browseData (basevisitor* v);

  public:

    // print
    // ------------------------------------------------------

    void                  print (ostream& os) const;

    void                  printAtomOptionsValues (
                            ostream& os,
                            int      valueFieldWidth) const;

  protected:

    // fields
    // ------------------------------------------------------

    string                fVariableName;
};
typedef SMARTP<oahAtomWithVariableName> S_oahAtomWithVariableName;
EXP ostream& operator<< (ostream& os, const S_oahAtomWithVariableName& elt);

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

    void                  setBooleanVariable (
                            bool value)
                              { fBooleanVariable = value; }

  public:

    // services
    // ------------------------------------------------------

    S_oahValuedAtom       handleOptionUnderName (
                            string   optionName,
                            ostream& os);

  public:

    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v);
    virtual void          acceptOut (basevisitor* v);

    virtual void          browseData (basevisitor* v);

  public:

    // print
    // ------------------------------------------------------

    void                  print (ostream& os) const;

    void                  printAtomOptionsValues (
                            ostream& os,
                            int      valueFieldWidth) const;

  protected:

    // fields
    // ------------------------------------------------------

    bool&                 fBooleanVariable;
};
typedef SMARTP<oahBooleanAtom> S_oahBooleanAtom;
EXP ostream& operator<< (ostream& os, const S_oahBooleanAtom& elt);

//______________________________________________________________________________
class oahTwoBooleansAtom : public oahBooleanAtom
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

    void                  setTwoBooleansVariables (
                            bool value)
                              {
                                fBooleanVariable =
                                  value;
                                fBooleanSecondaryVariable =
                                  value;
                              }

  public:

    // services
    // ------------------------------------------------------

    S_oahValuedAtom       handleOptionUnderName (
                            string   optionName,
                            ostream& os);

  public:

    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v);
    virtual void          acceptOut (basevisitor* v);

    virtual void          browseData (basevisitor* v);

  public:

    // print
    // ------------------------------------------------------

    void                  print (ostream& os) const;

    void                  printAtomOptionsValues (
                            ostream& os,
                            int      valueFieldWidth) const;

  private:

    // fields
    // ------------------------------------------------------

    bool&                 fBooleanSecondaryVariable;
};
typedef SMARTP<oahTwoBooleansAtom> S_oahTwoBooleansAtom;
EXP ostream& operator<< (ostream& os, const S_oahTwoBooleansAtom& elt);

//______________________________________________________________________________
class oahThreeBooleansAtom : public oahBooleanAtom
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

    void                  setThreeBooleansVariables (
                            bool value)
                              {
                                fBooleanVariable =
                                  value;
                                fBooleanSecondaryVariable =
                                  value;
                                fBooleanTertiaryVariable =
                                  value;
                              }

  public:

    // services
    // ------------------------------------------------------

    S_oahValuedAtom       handleOptionUnderName (
                            string   optionName,
                            ostream& os);

  public:

    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v);
    virtual void          acceptOut (basevisitor* v);

    virtual void          browseData (basevisitor* v);

  public:

    // print
    // ------------------------------------------------------

    void                  print (ostream& os) const;

    void                  printAtomOptionsValues (
                            ostream& os,
                            int      valueFieldWidth) const;

  private:

    // fields
    // ------------------------------------------------------

    bool&                 fBooleanSecondaryVariable;
    bool&                 fBooleanTertiaryVariable;
};
typedef SMARTP<oahThreeBooleansAtom> S_oahThreeBooleansAtom;
EXP ostream& operator<< (ostream& os, const S_oahThreeBooleansAtom& elt);

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

    void                  setCombinedBooleanVariables (
                            bool value);

    S_oahValuedAtom       handleOptionUnderName (
                            string   optionName,
                            ostream& os);

  public:

    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v);
    virtual void          acceptOut (basevisitor* v);

    virtual void          browseData (basevisitor* v);

  public:

    // print
    // ------------------------------------------------------

    void                  print (ostream& os) const;

    void                  printHelp (ostream& os);

    void                  printAtomOptionsValues (
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
      string prefixName,
      string prefixErsatz,
      string prefixDescription);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    oahPrefix (
      string prefixName,
      string prefixErsatz,
      string prefixDescription);

    virtual ~oahPrefix ();

  public:

    // set and get
    // ------------------------------------------------------

    string                getPrefixName () const
                              { return fPrefixName; }

    string                getPrefixErsatz () const
                              { return fPrefixErsatz; }

    string                getPrefixDescription () const
                              { return fPrefixDescription; }

  public:

    // public services
    // ------------------------------------------------------

  private:

    // private services
    // ------------------------------------------------------

    string                prefixNames () const;
    string                prefixNamesInColumns (
                            int subGroupsShortNameFieldWidth) const;

    string                prefixNamesBetweenParentheses () const;
    string                prefixNamesInColumnsBetweenParentheses (
                            int subGroupsShortNameFieldWidth) const;

    string                operator () () const
                              { return fPrefixErsatz; }

  public:

    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v);
    virtual void          acceptOut (basevisitor* v);

    virtual void          browseData (basevisitor* v);

  public:

    // print
    // ------------------------------------------------------

    virtual void          printPrefixHeader (ostream& os) const;

    virtual void          printPrefixEssentials (
                            ostream& os,
                            int      fieldWidth) const;

    virtual void          print (ostream& os) const;

    virtual void          printHelp (ostream& os);

  protected:

    // fields
    // ------------------------------------------------------

    string                fPrefixName;
    string                fPrefixErsatz;
    string                fPrefixDescription;
};
EXP ostream& operator<< (ostream& os, const S_oahPrefix& elt);

//______________________________________________________________________________
class oahMultiplexBooleansAtom : public oahAtom
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<oahMultiplexBooleansAtom> create (
      string      description,
      string      shortSuffixDescriptor,
      string      longSuffixDescriptor,
      S_oahPrefix shortNamesPrefix,
      S_oahPrefix longNamesPrefix);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    oahMultiplexBooleansAtom (
      string      description,
      string      shortSuffixDescriptor,
      string      longSuffixDescriptor,
      S_oahPrefix shortNamesPrefix,
      S_oahPrefix longNamesPrefix);

    virtual ~oahMultiplexBooleansAtom ();

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

    S_oahValuedAtom       handleOptionUnderName (
                            string   optionName,
                            ostream& os);

  public:

    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v);
    virtual void          acceptOut (basevisitor* v);

    virtual void          browseData (basevisitor* v);

  public:

    // print
    // ------------------------------------------------------

    void                  print (ostream& os) const;

    void                  printHelp (ostream& os);

    void                  printAtomOptionsValues (
                            ostream& os,
                            int      valueFieldWidth) const;

  private:

    // fields
    // ------------------------------------------------------

    S_oahPrefix           fShortNamesPrefix;
    S_oahPrefix           fLongNamesPrefix;

    string                fShortSuffixDescriptor;
    string                fLongSuffixDescriptor;

    list<S_oahBooleanAtom>
                          fBooleanAtomsList;

    string                fShortNamesPrefixName;
    string                fLongNamesPrefixName;

    list<string>          fShortNamesSuffixes;
    list<string>          fLongNamesSuffixes;
};
typedef SMARTP<oahMultiplexBooleansAtom> S_oahMultiplexBooleansAtom;
EXP ostream& operator<< (ostream& os, const S_oahMultiplexBooleansAtom& elt);

//______________________________________________________________________________
class oahValuedAtom : public oahAtomWithVariableName
{
  public:

    // data types
    // ------------------------------------------------------

    enum oahValuedAtomKind { // JMI ???
      kAtomHasNoArgument,
      kAtomHasARequiredArgument,
      kAtomHasAnOptionsArgument };

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

  public:

    // services
    // ------------------------------------------------------

    virtual void          handleValue (
                            string   theString,
                            ostream& os) = 0;

    virtual void          handleDefaultValue ();
                            // used only if fElementValueExpectedKind
                            // is equal to kElementValueExpectedOptional

  public:

    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v);
    virtual void          acceptOut (basevisitor* v);

    virtual void          browseData (basevisitor* v);

  public:

    // print
    // ------------------------------------------------------

    virtual void          printValuedAtomEssentials (
                            ostream& os,
                            int      fieldWidth) const;

    void                  print (ostream& os) const;

    void                  printHelp (ostream& os);

    virtual void          printAtomOptionsValues (
                            ostream& os,
                            int      valueFieldWidth) const;

  protected:

    // fields
    // ------------------------------------------------------

    oahValuedAtomKind     fValuedAtomKind; // JMI

    string                fValueSpecification;
};
typedef SMARTP<oahValuedAtom> S_oahValuedAtom;
EXP ostream& operator<< (ostream& os, const S_oahValuedAtom& elt);

// optional values style
//______________________________________________________________________________
enum oahOptionalValuesStyleKind {
  kOptionalValuesStyleGNU, // default value
  kOptionalValuesStyleOAH };

string oahOptionalValuesStyleKindAsString (
  oahOptionalValuesStyleKind optionalValuesStyleKind);

extern map<string, oahOptionalValuesStyleKind>
  gOahOptionalValuesStyleKindsMap;

string existingOahOptionalValuesStyleKinds (int namesListMaxLength);

void initializeOahOptionalValuesStyleKindsMap ();

//______________________________________________________________________________
class oahIntegerAtom : public oahValuedAtom
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<oahIntegerAtom> create (
      string shortName,
      string longName,
      string description,
      string valueSpecification,
      string variableName,
      int&   integerVariable);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    oahIntegerAtom (
      string shortName,
      string longName,
      string description,
      string valueSpecification,
      string variableName,
      int&   integerVariable);

  protected:


    virtual ~oahIntegerAtom ();

  public:

    // set and get
    // ------------------------------------------------------

    void                  setIntegerVariable (
                            int value)
                              { fIntegerVariable = value; }

  public:

    // services
    // ------------------------------------------------------

    S_oahValuedAtom       handleOptionUnderName (
                            string   optionName,
                            ostream& os);

    void                  handleValue (
                            string   theString,
                            ostream& os);

  public:

    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v);
    virtual void          acceptOut (basevisitor* v);

    virtual void          browseData (basevisitor* v);

  public:

    // print
    // ------------------------------------------------------

    string                asShortNamedOptionString () const;
    string                asActualLongNamedOptionString () const;

    void                  print (ostream& os) const;

    void                  printAtomOptionsValues (
                            ostream& os,
                            int      valueFieldWidth) const;
  protected:

    // fields
    // ------------------------------------------------------

    int&                  fIntegerVariable;
};
typedef SMARTP<oahIntegerAtom> S_oahIntegerAtom;
EXP ostream& operator<< (ostream& os, const S_oahIntegerAtom& elt);

//______________________________________________________________________________
class oahTwoIntegersAtom : public oahIntegerAtom
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<oahTwoIntegersAtom> create (
      string shortName,
      string longName,
      string description,
      string valueSpecification,
      string variableName,
      int&   integerVariable,
      int&   integerSecondaryVariable);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    oahTwoIntegersAtom (
      string shortName,
      string longName,
      string description,
      string valueSpecification,
      string variableName,
      int&   integerVariable,
      int&   integerSecondaryVariable);

  protected:


    virtual ~oahTwoIntegersAtom ();

  public:

    // set and get
    // ------------------------------------------------------

    void                  setIntegerVariable (
                            int value)
                              { fIntegerVariable = value; }

  public:

    // services
    // ------------------------------------------------------

    S_oahValuedAtom       handleOptionUnderName (
                            string   optionName,
                            ostream& os);

    void                  handleValue (
                            string   theString,
                            ostream& os);

  public:

    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v);
    virtual void          acceptOut (basevisitor* v);

    virtual void          browseData (basevisitor* v);

  public:

    // print
    // ------------------------------------------------------

    string                asShortNamedOptionString () const;
    string                asActualLongNamedOptionString () const;

    void                  print (ostream& os) const;

    void                  printAtomOptionsValues (
                            ostream& os,
                            int      valueFieldWidth) const;
  private:

    // fields
    // ------------------------------------------------------

    int&                  fIntegerSecondaryVariable;
};
typedef SMARTP<oahTwoIntegersAtom> S_oahTwoIntegersAtom;
EXP ostream& operator<< (ostream& os, const S_oahTwoIntegersAtom& elt);

//______________________________________________________________________________
class oahFloatAtom : public oahValuedAtom
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<oahFloatAtom> create (
      string shortName,
      string longName,
      string description,
      string valueSpecification,
      string variableName,
      float& floatVariable);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    oahFloatAtom (
      string shortName,
      string longName,
      string description,
      string valueSpecification,
      string variableName,
      float& floatVariable);

    virtual ~oahFloatAtom ();

  public:

    // set and get
    // ------------------------------------------------------

    void                  setFloatVariable (
                            float value)
                              {
                                fFloatVariable = value;
                                fFloatVariableHasBeenSet = true;
                              }

    bool                  getFloatVariableHasBeenSet () const
                              { return fFloatVariableHasBeenSet; }

  public:

    // services
    // ------------------------------------------------------

    S_oahValuedAtom       handleOptionUnderName (
                            string   optionName,
                            ostream& os);

    void                  handleValue (
                            string   theString,
                            ostream& os);

  public:

    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v);
    virtual void          acceptOut (basevisitor* v);

    virtual void          browseData (basevisitor* v);

  public:

    // print
    // ------------------------------------------------------

    string                asShortNamedOptionString () const;
    string                asActualLongNamedOptionString () const;

    void                  print (ostream& os) const;

    void                  printAtomOptionsValues (
                            ostream& os,
                            int      valueFieldWidth) const;

  private:

    // fields
    // ------------------------------------------------------

    float&                fFloatVariable;

    bool                  fFloatVariableHasBeenSet;
};
typedef SMARTP<oahFloatAtom> S_oahFloatAtom;
EXP ostream& operator<< (ostream& os, const S_oahFloatAtom& elt);

//______________________________________________________________________________
class oahStringAtom : public oahValuedAtom
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<oahStringAtom> create (
      string  shortName,
      string  longName,
      string  description,
      string  valueSpecification,
      string  variableName,
      string& stringVariable);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    oahStringAtom (
      string  shortName,
      string  longName,
      string  description,
      string  valueSpecification,
      string  variableName,
      string& stringVariable);

    virtual ~oahStringAtom ();

  public:

    // set and get
    // ------------------------------------------------------

    void                  setStringVariable (
                            string value)
                              { fStringVariable = value; }

  public:

    // services
    // ------------------------------------------------------

    S_oahValuedAtom       handleOptionUnderName (
                            string   optionName,
                            ostream& os);

    void                  handleValue (
                            string   theString,
                            ostream& os);

  public:

    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v);
    virtual void          acceptOut (basevisitor* v);

    virtual void          browseData (basevisitor* v);

  public:

    // print
    // ------------------------------------------------------

    string                asShortNamedOptionString () const;
    string                asActualLongNamedOptionString () const;

    void                  print (ostream& os) const;

    void                  printAtomOptionsValues (
                            ostream& os,
                            int      valueFieldWidth) const;

  protected:

    // fields
    // ------------------------------------------------------

    string&               fStringVariable;
};
typedef SMARTP<oahStringAtom> S_oahStringAtom;
EXP ostream& operator<< (ostream& os, const S_oahStringAtom& elt);

//______________________________________________________________________________
class oahMonoplexStringAtom : public oahAtom
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<oahMonoplexStringAtom> create (
      string      description,
      string      atomNameDescriptor,
      string      stringValueDescriptor);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    oahMonoplexStringAtom (
      string      description,
      string      atomNameDescriptor,
      string      stringValueDescriptor);

    virtual ~oahMonoplexStringAtom ();

  public:

    // set and get
    // ------------------------------------------------------

    const list<S_oahStringAtom>&
                          getStringAtomsList ()
                              { return fStringAtomsList; }

    // services
    // ------------------------------------------------------

    void                  addStringAtom (
                            S_oahStringAtom stringAtom);

    void                  addStringAtomByName (
                            string name);

    S_oahValuedAtom       handleOptionUnderName (
                            string   optionName,
                            ostream& os);

  public:

    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v);
    virtual void          acceptOut (basevisitor* v);

    virtual void          browseData (basevisitor* v);

  public:

    // print
    // ------------------------------------------------------

    void                  print (ostream& os) const;

    void                  printHelp (ostream& os);

    void                  printAtomOptionsValues (
                            ostream& os,
                            int      valueFieldWidth) const;

  private:

    // fields
    // ------------------------------------------------------

    string                fAtomNameDescriptor;
    string                fStringValueDescriptor;

    list<S_oahStringAtom>
                          fStringAtomsList;

    list<string>          fAtomNamesList;
};
typedef SMARTP<oahMonoplexStringAtom> S_oahMonoplexStringAtom;
EXP ostream& operator<< (ostream& os, const S_oahMonoplexStringAtom& elt);

//______________________________________________________________________________
class oahStringWithDefaultValueAtom : public oahStringAtom
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<oahStringWithDefaultValueAtom> create (
      string  shortName,
      string  longName,
      string  description,
      string  valueSpecification,
      string  variableName,
      string& stringVariable,
      string  defaultStringValue);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    oahStringWithDefaultValueAtom (
      string  shortName,
      string  longName,
      string  description,
      string  valueSpecification,
      string  variableName,
      string& stringVariable,
      string  defaultStringValue);

    virtual ~oahStringWithDefaultValueAtom ();

  public:

    // set and get
    // ------------------------------------------------------

    void                  setStringVariable (
                            string value)
                              { oahStringAtom::setStringVariable (value); }

  public:

    // services
    // ------------------------------------------------------

    S_oahValuedAtom       handleOptionUnderName (
                            string   optionName,
                            ostream& os);

    void                  handleValue (
                            string   theString,
                            ostream& os);

  public:

    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v);
    virtual void          acceptOut (basevisitor* v);

    virtual void          browseData (basevisitor* v);

  public:

    // print
    // ------------------------------------------------------

    string                asShortNamedOptionString () const;
    string                asActualLongNamedOptionString () const;

    void                  print (ostream& os) const;

    void                  printAtomOptionsValues (
                            ostream& os,
                            int      valueFieldWidth) const;

  protected:

    // fields
    // ------------------------------------------------------

    string                fDefaultStringValue;
};
typedef SMARTP<oahStringWithDefaultValueAtom> S_oahStringWithDefaultValueAtom;
EXP ostream& operator<< (ostream& os, const S_oahStringWithDefaultValueAtom& elt);

//______________________________________________________________________________
class oahRationalAtom : public oahValuedAtom
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<oahRationalAtom> create (
      string    shortName,
      string    longName,
      string    description,
      string    valueSpecification,
      string    variableName,
      rational& rationalVariable);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    oahRationalAtom (
      string    shortName,
      string    longName,
      string    description,
      string    valueSpecification,
      string    variableName,
      rational& rationalVariable);

    virtual ~oahRationalAtom ();

  public:

    // set and get
    // ------------------------------------------------------

    void                  setRationalVariable (
                            rational value)
                              { fRationalVariable = value;  }

  public:

    // services
    // ------------------------------------------------------

    S_oahValuedAtom       handleOptionUnderName (
                            string   optionName,
                            ostream& os);

    void                  handleValue (
                            string   theString,
                            ostream& os);

  public:

    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v);
    virtual void          acceptOut (basevisitor* v);

    virtual void          browseData (basevisitor* v);

  public:

    // print
    // ------------------------------------------------------

    string                asShortNamedOptionString () const;
    string                asActualLongNamedOptionString () const;

    void                  print (ostream& os) const;

    void                  printAtomOptionsValues (
                            ostream& os,
                            int      valueFieldWidth) const;

  private:

    // fields
    // ------------------------------------------------------

    rational&             fRationalVariable;
};
typedef SMARTP<oahRationalAtom> S_oahRationalAtom;
EXP ostream& operator<< (ostream& os, const S_oahRationalAtom& elt);

//______________________________________________________________________________
class oahNaturalNumbersSetAtom : public oahValuedAtom
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<oahNaturalNumbersSetAtom> create (
      string    shortName,
      string    longName,
      string    description,
      string    valueSpecification,
      string    variableName,
      set<int>& naturalNumbersSetVariable);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    oahNaturalNumbersSetAtom (
      string    shortName,
      string    longName,
      string    description,
      string    valueSpecification,
      string    variableName,
      set<int>& naturalNumbersSetVariable);

    virtual ~oahNaturalNumbersSetAtom ();

  public:

    // set and get
    // ------------------------------------------------------

    void                  setNaturalNumbersSetVariable (
                            set<int> value)
                              { fNaturalNumbersSetVariable = value; }

  public:

    // services
    // ------------------------------------------------------

    S_oahValuedAtom       handleOptionUnderName (
                            string   optionName,
                            ostream& os);

    void                  handleValue (
                            string   theString,
                            ostream& os);

  public:

    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v);
    virtual void          acceptOut (basevisitor* v);

    virtual void          browseData (basevisitor* v);

  public:

    // print
    // ------------------------------------------------------

    string                asShortNamedOptionString () const;
    string                asActualLongNamedOptionString () const;

    void                  print (ostream& os) const;

    void                  printAtomOptionsValues (
                            ostream& os,
                            int      valueFieldWidth) const;

  private:

    // fields
    // ------------------------------------------------------

    set<int>&             fNaturalNumbersSetVariable;
};
typedef SMARTP<oahNaturalNumbersSetAtom> S_oahNaturalNumbersSetAtom;
EXP ostream& operator<< (ostream& os, const S_oahNaturalNumbersSetAtom& elt);

//______________________________________________________________________________
class oahRGBColorAtom : public oahValuedAtom
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<oahRGBColorAtom> create (
      string       shortName,
      string       longName,
      string       description,
      string       valueSpecification,
      string       variableName,
      msrRGBColor& RGBColorVariable,
      bool&        hasBeenSetVariable);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    oahRGBColorAtom (
      string       shortName,
      string       longName,
      string       description,
      string       valueSpecification,
      string       variableName,
      msrRGBColor& RGBColorVariable,
      bool&        hasBeenSetVariable);

    virtual ~oahRGBColorAtom ();

  public:

    // set and get
    // ------------------------------------------------------

    void                  setRGBColorVariable (
                            msrRGBColor value)
                              { fRGBColorVariable = value; }

  public:

    // services
    // ------------------------------------------------------

    S_oahValuedAtom       handleOptionUnderName (
                            string   optionName,
                            ostream& os);

    void                  handleValue (
                            string   theString,
                            ostream& os);

  public:

    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v);
    virtual void          acceptOut (basevisitor* v);

    virtual void          browseData (basevisitor* v);

  public:

    // print
    // ------------------------------------------------------

    string                asShortNamedOptionString () const;
    string                asActualLongNamedOptionString () const;

    void                  print (ostream& os) const;

    void                  printAtomOptionsValues (
                            ostream& os,
                            int      valueFieldWidth) const;

  private:

    // fields
    // ------------------------------------------------------

    msrRGBColor&          fRGBColorVariable;
    bool&                 fHasBeenSetVariable;
};
typedef SMARTP<oahRGBColorAtom> S_oahRGBColorAtom;
EXP ostream& operator<< (ostream& os, const S_oahRGBColorAtom& elt);

//______________________________________________________________________________
class oahIntSetAtom : public oahValuedAtom
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<oahIntSetAtom> create (
      string    shortName,
      string    longName,
      string    description,
      string    valueSpecification,
      string    variableName,
      set<int>& intSetVariable);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    oahIntSetAtom (
      string    shortName,
      string    longName,
      string    description,
      string    valueSpecification,
      string    variableName,
      set<int>& intSetVariable);

    virtual ~oahIntSetAtom ();

  public:

    // set and get
    // ------------------------------------------------------

    const set<int>&       getIntSetVariable ()
                              { return fIntSetVariable; }

  public:

    // services
    // ------------------------------------------------------

    S_oahValuedAtom       handleOptionUnderName (
                            string   optionName,
                            ostream& os);

    void                  handleValue (
                            string   theString,
                            ostream& os);

  public:

    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v);
    virtual void          acceptOut (basevisitor* v);

    virtual void          browseData (basevisitor* v);

  public:

    // print
    // ------------------------------------------------------

    string                asShortNamedOptionString () const;
    string                asActualLongNamedOptionString () const;

    void                  print (ostream& os) const;

    void                  printAtomOptionsValues (
                            ostream& os,
                            int      valueFieldWidth) const;

  private:

    // fields
    // ------------------------------------------------------

    set<int>&             fIntSetVariable;
};
typedef SMARTP<oahIntSetAtom> S_oahIntSetAtom;
EXP ostream& operator<< (ostream& os, const S_oahIntSetAtom& elt);

//______________________________________________________________________________
class oahStringSetAtom : public oahValuedAtom
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<oahStringSetAtom> create (
      string       shortName,
      string       longName,
      string       description,
      string       valueSpecification,
      string       variableName,
      set<string>& stringSetVariable);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    oahStringSetAtom (
      string       shortName,
      string       longName,
      string       description,
      string       valueSpecification,
      string       variableName,
      set<string>& stringSetVariable);

    virtual ~oahStringSetAtom ();

  public:

    // set and get
    // ------------------------------------------------------

    void                  setStringSetVariable (
                            string  partName)
                              { fStringSetVariable.insert (partName); }

    const set<string>&    getStringSetVariable ()
                              { return fStringSetVariable; }

  public:

    // services
    // ------------------------------------------------------

    S_oahValuedAtom       handleOptionUnderName (
                            string   optionName,
                            ostream& os);

    void                  handleValue (
                            string   theString,
                            ostream& os);

  public:

    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v);
    virtual void          acceptOut (basevisitor* v);

    virtual void          browseData (basevisitor* v);

  public:

    // print
    // ------------------------------------------------------

    string                asShortNamedOptionString () const;
    string                asActualLongNamedOptionString () const;

    void                  print (ostream& os) const;

    void                  printAtomOptionsValues (
                            ostream& os,
                            int      valueFieldWidth) const;

  private:

    // fields
    // ------------------------------------------------------

    set<string>&          fStringSetVariable;
};
typedef SMARTP<oahStringSetAtom> S_oahStringSetAtom;
EXP ostream& operator<< (ostream& os, const S_oahStringSetAtom& elt);

//______________________________________________________________________________
class oahStringToIntMapAtom : public oahValuedAtom
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<oahStringToIntMapAtom> create (
      string            shortName,
      string            longName,
      string            description,
      string            valueSpecification,
      string            variableName,
      map<string, int>& stringToIntMapVariable);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    oahStringToIntMapAtom (
      string            shortName,
      string            longName,
      string            description,
      string            valueSpecification,
      string            variableName,
      map<string, int>& stringToIntMapVariable);

    virtual ~oahStringToIntMapAtom ();

  public:

    // set and get
    // ------------------------------------------------------

    const map<string, int>&
                          getStringToIntMapVariable ()
                              { return fStringToIntMapVariable; }

  public:

    // services
    // ------------------------------------------------------

    S_oahValuedAtom       handleOptionUnderName (
                            string   optionName,
                            ostream& os);

    void                  handleValue (
                            string   theString,
                            ostream& os);

  public:

    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v);
    virtual void          acceptOut (basevisitor* v);

    virtual void          browseData (basevisitor* v);

  public:

    // print
    // ------------------------------------------------------

    string                asShortNamedOptionString () const;
    string                asActualLongNamedOptionString () const;

    void                  print (ostream& os) const;

    void                  printAtomOptionsValues (
                            ostream& os,
                            int      valueFieldWidth) const;

  private:

    // fields
    // ------------------------------------------------------

    map<string, int>&     fStringToIntMapVariable;
};
typedef SMARTP<oahStringToIntMapAtom> S_oahStringToIntMapAtom;
EXP ostream& operator<< (ostream& os, const S_oahStringToIntMapAtom& elt);

//______________________________________________________________________________
class oahStringAndIntegerAtom : public oahValuedAtom
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<oahStringAndIntegerAtom> create (
      string  shortName,
      string  longName,
      string  description,
      string  valueSpecification,
      string  stringVariableName,
      string& stringVariable,
      string  integerVariableName,
      int&    integerVariable);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    oahStringAndIntegerAtom (
      string  shortName,
      string  longName,
      string  description,
      string  valueSpecification,
      string  stringVariableName,
      string& stringVariable,
      string  integerVariableName,
      int&    integerVariable);

    virtual ~oahStringAndIntegerAtom ();

  public:

    // set and get
    // ------------------------------------------------------

    void                  setStringVariable (
                            string value)
                              { fStringVariable = value; }

    string                getStringVariable () const
                              { return fStringVariable; }

    void                  setIntegerVariable (
                            int value)
                              { fIntegerVariable = value; }

    int                   getIntegerVariable () const
                              { return fIntegerVariable; }

  public:

    // services
    // ------------------------------------------------------

    S_oahValuedAtom       handleOptionUnderName (
                            string   optionName,
                            ostream& os);

    void                  handleValue (
                            string   theString,
                            ostream& os);

  public:

    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v);
    virtual void          acceptOut (basevisitor* v);

    virtual void          browseData (basevisitor* v);

  public:

    // print
    // ------------------------------------------------------

    string                asShortNamedOptionString () const;
    string                asActualLongNamedOptionString () const;

    void                  print (ostream& os) const;

    void                  printAtomOptionsValues (
                            ostream& os,
                            int      valueFieldWidth) const;

  private:

    // fields
    // ------------------------------------------------------

    string                fStringVariableName;
    string&               fStringVariable;

    string                fIntegerVariableName;
    int&                  fIntegerVariable;
};
typedef SMARTP<oahStringAndIntegerAtom> S_oahStringAndIntegerAtom;
EXP ostream& operator<< (ostream& os, const S_oahStringAndIntegerAtom& elt);

//______________________________________________________________________________
class oahStringAndTwoIntegersAtom : public oahValuedAtom
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<oahStringAndTwoIntegersAtom> create (
      string  shortName,
      string  longName,
      string  description,
      string  valueSpecification,
      string  variableName,
      string& stringVariable,
      int&    primaryIntegerVariable,
      int&    secondaryIntegerVariable);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    oahStringAndTwoIntegersAtom (
      string  shortName,
      string  longName,
      string  description,
      string  valueSpecification,
      string  variableName,
      string& stringVariable,
      int&    primaryIntegerVariable,
      int&    secondaryIntegerVariable);

    virtual ~oahStringAndTwoIntegersAtom ();

  public:

    // set and get
    // ------------------------------------------------------

    void                  setStringVariable (
                            string value)
                              { fStringVariable = value; }

    string                getStringVariable () const
                              { return fStringVariable; }

    void                  setPrimaryIntegerVariable (
                            int value)
                              { fPrimaryIntegerVariable = value; }

    int                   getPrimaryIntegerVariable () const
                              { return fPrimaryIntegerVariable; }

    void                  setSecondaryIntegerVariable (
                            int value)
                              { fSecondaryIntegerVariable = value; }

    int                   getSecondaryIntegerVariable () const
                              { return fSecondaryIntegerVariable; }

  public:

    // services
    // ------------------------------------------------------

    S_oahValuedAtom       handleOptionUnderName (
                            string   optionName,
                            ostream& os);

    void                  handleValue (
                            string   theString,
                            ostream& os);

  public:

    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v);
    virtual void          acceptOut (basevisitor* v);

    virtual void          browseData (basevisitor* v);

  public:

    // print
    // ------------------------------------------------------

    string                asShortNamedOptionString () const;
    string                asActualLongNamedOptionString () const;

    void                  print (ostream& os) const;

    void                  printAtomOptionsValues (
                            ostream& os,
                            int      valueFieldWidth) const;

  private:

    // fields
    // ------------------------------------------------------

    string&               fStringVariable;

    int&                  fPrimaryIntegerVariable;
    int&                  fSecondaryIntegerVariable;
};
typedef SMARTP<oahStringAndTwoIntegersAtom> S_oahStringAndTwoIntegersAtom;
EXP ostream& operator<< (ostream& os, const S_oahStringAndTwoIntegersAtom& elt);

//______________________________________________________________________________
class oahLengthUnitKindAtom : public oahValuedAtom
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<oahLengthUnitKindAtom> create (
      string             shortName,
      string             longName,
      string             description,
      string             valueSpecification,
      string             variableName,
      msrLengthUnitKind& lengthUnitKindVariable);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    oahLengthUnitKindAtom (
      string             shortName,
      string             longName,
      string             description,
      string             valueSpecification,
      string             variableName,
      msrLengthUnitKind& lengthUnitKindVariable);

    virtual ~oahLengthUnitKindAtom ();

  public:

    // set and get
    // ------------------------------------------------------

    void                  setLengthUnitKindVariable (
                            msrLengthUnitKind value)
                              { fLengthUnitKindVariable = value; }

  public:

    // services
    // ------------------------------------------------------

    S_oahValuedAtom       handleOptionUnderName (
                            string   optionName,
                            ostream& os);

    void                  handleValue (
                            string   theString,
                            ostream& os);

  public:

    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v);
    virtual void          acceptOut (basevisitor* v);

    virtual void          browseData (basevisitor* v);

  public:

    // print
    // ------------------------------------------------------

    string                asShortNamedOptionString () const;
    string                asActualLongNamedOptionString () const;

    void                  print (ostream& os) const;

    void                  printAtomOptionsValues (
                            ostream& os,
                            int      valueFieldWidth) const;

  private:

    // fields
    // ------------------------------------------------------

    msrLengthUnitKind&    fLengthUnitKindVariable;
};
typedef SMARTP<oahLengthUnitKindAtom> S_oahLengthUnitKindAtom;
EXP ostream& operator<< (ostream& os, const S_oahLengthUnitKindAtom& elt);

//______________________________________________________________________________
class oahLengthAtom : public oahValuedAtom
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<oahLengthAtom> create (
      string     shortName,
      string     longName,
      string     description,
      string     valueSpecification,
      string     variableName,
      msrLength& lengthVariable);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    oahLengthAtom (
      string     shortName,
      string     longName,
      string     description,
      string     valueSpecification,
      string     variableName,
      msrLength& lengthVariable);

    virtual ~oahLengthAtom ();

  public:

    // set and get
    // ------------------------------------------------------

    void                  setLengthVariable (
                            msrLength value)
                              { fLengthVariable = value; }

  public:

    // services
    // ------------------------------------------------------

    S_oahValuedAtom       handleOptionUnderName (
                            string   optionName,
                            ostream& os);

    void                  handleValue (
                            string   theString,
                            ostream& os);

  public:

    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v);
    virtual void          acceptOut (basevisitor* v);

    virtual void          browseData (basevisitor* v);

  public:

    // print
    // ------------------------------------------------------

    string                asShortNamedOptionString () const;
    string                asActualLongNamedOptionString () const;

    void                  print (ostream& os) const;

    void                  printAtomOptionsValues (
                            ostream& os,
                            int      valueFieldWidth) const;

  private:

    // fields
    // ------------------------------------------------------

    msrLength&            fLengthVariable;
};
typedef SMARTP<oahLengthAtom> S_oahLengthAtom;
EXP ostream& operator<< (ostream& os, const S_oahLengthAtom& elt);

//______________________________________________________________________________
class oahMidiTempoAtom : public oahValuedAtom
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<oahMidiTempoAtom> create (
      string        shortName,
      string        longName,
      string        description,
      string        valueSpecification,
      string        variableName,
      msrMidiTempo& midiTempoVariable);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    oahMidiTempoAtom (
      string        shortName,
      string        longName,
      string        description,
      string        valueSpecification,
      string        variableName,
      msrMidiTempo& midiTempoVariable);

    virtual ~oahMidiTempoAtom ();

  public:

    // set and get
    // ------------------------------------------------------

    void                  setMidiTempoAtomVariable (
                            msrMidiTempo& value)
                              { fMidiTempoAtomVariable = value; }

    const msrMidiTempo&   getMidiTempoAtomVariable ()
                              { return fMidiTempoAtomVariable; }

  public:

    // services
    // ------------------------------------------------------

    S_oahValuedAtom       handleOptionUnderName (
                            string   optionName,
                            ostream& os);

    void                  handleValue (
                            string   theString,
                            ostream& os);

  public:

    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v);
    virtual void          acceptOut (basevisitor* v);

    virtual void          browseData (basevisitor* v);

  public:

    // print
    // ------------------------------------------------------

    string                asShortNamedOptionString () const;
    string                asActualLongNamedOptionString () const;

    void                  print (ostream& os) const;

    void                  printAtomOptionsValues (
                            ostream& os,
                            int      valueFieldWidth) const;

  private:

    // fields
    // ------------------------------------------------------

    msrMidiTempo&         fMidiTempoAtomVariable;
};
typedef SMARTP<oahMidiTempoAtom> S_oahMidiTempoAtom;
EXP ostream& operator<< (ostream& os, const S_oahMidiTempoAtom& elt);

//______________________________________________________________________________
class oahOptionNameHelpAtom : public oahStringWithDefaultValueAtom
{
/*
  This is where OAH is introspective:
    OAH provides a way to obtain help on any option name,
    be it one to print help about a group or subgroup,
    or the name of a 'regular' atom.
  This option is reflexive, since it can provide help about itself.
*/
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<oahOptionNameHelpAtom> create (
      string shortName,
      string longName,
      string description,
      string valueSpecification,
      string variableName,
      string& stringVariable,
      string defaultOptionName);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    oahOptionNameHelpAtom (
      string shortName,
      string longName,
      string description,
      string valueSpecification,
      string variableName,
      string& stringVariable,
      string defaultOptionName);

  protected:


    virtual ~oahOptionNameHelpAtom ();

  public:

    // set and get
    // ------------------------------------------------------

  public:

    // services
    // ------------------------------------------------------

    S_oahValuedAtom       handleOptionUnderName (
                            string   optionName,
                            ostream& os);

    void                  handleValue (
                            string   theString,
                            ostream& os);

    void                  handleDefaultValue ();

  public:

    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v);
    virtual void          acceptOut (basevisitor* v);

    virtual void          browseData (basevisitor* v);

  public:

    // print
    // ------------------------------------------------------

    string                asShortNamedOptionString () const;
    string                asActualLongNamedOptionString () const;

    void                  print (ostream& os) const;

    virtual void          printAtomOptionsValues (
                            ostream& os,
                            int      valueFieldWidth) const;
  private:

    // fields
    // ------------------------------------------------------

    string                fValueSpecification;
};
typedef SMARTP<oahOptionNameHelpAtom> S_oahOptionNameHelpAtom;
EXP ostream& operator<< (ostream& os, const S_oahOptionNameHelpAtom& elt);

//_______________________________________________________________________________
class oahSubGroup : public oahElement
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<oahSubGroup> create (
      string                  subGroupHeader,
      string                  shortName,
      string                  longName,
      string                  description,
      oahElementVisibilityKind optionVisibilityKind,
      S_oahGroup              groupUpLink);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    oahSubGroup (
      string                  subGroupHeader,
      string                  shortName,
      string                  longName,
      string                  description,
      oahElementVisibilityKind optionVisibilityKind,
      S_oahGroup              groupUpLink);

    virtual ~oahSubGroup ();

  public:

    // set and get
    // ------------------------------------------------------

    void                  setGroupUpLink (
                            S_oahGroup group)
                              { fGroupUpLink = group; }

    S_oahGroup            getGroupUpLink () const
                              { return fGroupUpLink; }

    string                getSubGroupHeader () const
                              { return fSubGroupHeader; }

    void                  incrementNumberOfUserChoseAtomsInThisSubGroup ()
                              { fNumberOfUserChoseAtomsInThisSubGroup += 1; }

    int                   getNumberOfUserChoseAtomsInThisSubGroup () const
                              { return fNumberOfUserChoseAtomsInThisSubGroup; }

    const list<S_oahAtom>&
                          getAtomsList () const
                              { return fAtomsList; }

  public:

    // consistency check
    // ------------------------------------------------------

    virtual void          checkOptionsConsistency ();

  public:

    // services
    // ------------------------------------------------------

    void                  underlineSubGroupHeader (ostream& os) const;

    void                  registerSubGroupInHandler (
                            S_oahHandler handler);

    void                  appendAtomToSubGroup (
                            S_oahAtom oahAtom);

    S_oahElement          fetchOptionByName (
                            string name);

    S_oahValuedAtom       handleOptionUnderName (
                            string   optionName,
                            ostream& os);

  public:

    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v);
    virtual void          acceptOut (basevisitor* v);

    virtual void          browseData (basevisitor* v);

  public:

    // print
    // ------------------------------------------------------

    void                  print (ostream& os) const;

    void                  printHelp (ostream& os);

    void                  printHelpWithHeaderWidth (
                            ostream& os,
                            int      subGroupHeaderWidth);

    void                  printSubGroupHeader (ostream& os) const;
    void                  printSubGroupHeaderWithHeaderWidth (
                            ostream& os,
                            int      subGroupHeaderWidth) const;

    void                  printSubGroupHelp (
                            ostream& os) const;

    void                  printSubGroupAndAtomHelp (
                            ostream&  os,
                            S_oahAtom targetAtom) const;

    void                  printOptionsSummary (ostream& os) const;

    void                  printSubGroupSpecificHelpOrOptionsSummary (
                            ostream&      os,
                            S_oahSubGroup subGroup) const;

    void                  printSubGroupOptionsValues (
                            ostream& os,
                            int      valueFieldWidth) const;

  private:

    // fields
    // ------------------------------------------------------

    S_oahGroup            fGroupUpLink;

    string                fSubGroupHeader;

    list<S_oahAtom>       fAtomsList;

    // protected work fields
    // ------------------------------------------------------

    int                   fNumberOfUserChoseAtomsInThisSubGroup;
};
typedef SMARTP<oahSubGroup> S_oahSubGroup;
EXP ostream& operator<< (ostream& os, const S_oahSubGroup& elt);

//_______________________________________________________________________________
class oahGroup : public oahElement
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<oahGroup> create (
      string                  header,
      string                  shortName,
      string                  longName,
      string                  description,
      oahElementVisibilityKind optionVisibilityKind,
      S_oahHandler            groupHandlerUpLink);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    oahGroup (
      string                  header,
      string                  shortName,
      string                  longName,
      string                  description,
      oahElementVisibilityKind optionVisibilityKind,
      S_oahHandler            groupHandlerUpLink);

    virtual ~oahGroup ();

  public:

    // set and get
    // ------------------------------------------------------

    string                getGroupHeader () const
                              { return fGroupHeader; }

    const list<S_oahSubGroup>&
                          getSubGroupsList () const
                              { return fSubGroupsList; }

    void                  incrementNumberOfUserChoseAtomsInThisGroup ()
                              { fNumberOfUserChoseAtomsInThisGroup += 1; }

    int                   getNumberOfUserChoseAtomsInThisGroup () const
                              { return fNumberOfUserChoseAtomsInThisGroup; }

  public:

    // consistency check
    // ------------------------------------------------------

    void                  checkGroupSubGroupsOptionsConsistency ();

    virtual void          checkOptionsConsistency ();

  public:

    // public services
    // ------------------------------------------------------

    void                  underlineGroupHeader (ostream& os) const;

    void                  registerGroupInHandler (
                            S_oahHandler handler);

    void                  appendSubGroupToGroup (
                            S_oahSubGroup subGroup);

    S_oahElement          fetchOptionByName (
                            string name);

    virtual void          handleAtomValue (
                            ostream&  os,
                            S_oahAtom atom,
                            string    theString);

    S_oahValuedAtom       handleOptionUnderName (
                            string   optionName,
                            ostream& os);

  public:

    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v);
    virtual void          acceptOut (basevisitor* v);

    virtual void          browseData (basevisitor* v);

  public:

    // print
    // ------------------------------------------------------

    void                  print (ostream& os) const;

    void                  printGroupHeader (ostream& os) const;

    void                  printHelp (ostream& os);

    void                  printGroupAndSubGroupHelp (
                            ostream&      os,
                            S_oahSubGroup targetSubGroup) const;

    void                  printGroupAndSubGroupAndAtomHelp (
                            ostream&      os,
                            S_oahSubGroup targetSubGroup,
                            S_oahAtom     targetAtom) const;

    void                  printOptionsSummary (ostream& os) const;

    void                  printGroupAndSubGroupSpecificHelp (
                            ostream&      os,
                            S_oahSubGroup subGroup) const;

    void                  printGroupOptionsValues (
                            ostream& os,
                            int      valueFieldWidth) const;

  protected:

    // protected fields
    // ------------------------------------------------------

    string                fGroupHeader;

    list<S_oahSubGroup>   fSubGroupsList;

  protected:

    // protected work fields
    // ------------------------------------------------------

    int                   fNumberOfUserChoseAtomsInThisGroup;
};
typedef SMARTP<oahGroup> S_oahGroup;
EXP ostream& operator<< (ostream& os, const S_oahGroup& elt);

//_______________________________________________________________________________
class EXP oahHandler : public oahElement
{
  public:

    // data types
    // ------------------------------------------------------

    enum oahHelpOptionsHaveBeenUsedKind {
        kHelpOptionsHaveBeenUsedYes,
        kHelpOptionsHaveBeenUsedNo };

    static string helpOptionsHaveBeenUsedKindAsString (
      oahHelpOptionsHaveBeenUsedKind helpOptionsHaveBeenUsedKind);

    enum oahOptionsDefaultValuesStyle {
        kGNUStyle,
        kOAHStyle };

    static string optionsDefaultValuesStyleAsString (
      oahOptionsDefaultValuesStyle optionsDefaultValuesStyle);

  public:

    // creation
    // ------------------------------------------------------
    static SMARTP<oahHandler> create (
      string   handlerHeader,
      string   handlerValuesHeader,
      string   optionHoahHandlerandlerHelpShortName,
      string   handlerLongName,
      string   handlerSummaryShortName,
      string   handlerSummaryLongName,
      string   handlerPreamble,
      string   handlerUsage,
      string   handlerDescription,
      ostream& handlerLogOstream);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    oahHandler (
      string   handlerHeader,
      string   handlerValuesHeader,
      string   handlerShortName,
      string   handlerLongName,
      string   handlerSummaryShortName,
      string   handlerSummaryLongName,
      string   handlerPreamble,
      string   handlerUsage,
      string   handlerDescription,
      ostream& handlerLogOstream);

    virtual ~oahHandler ();

  public:

    // set and get
    // ------------------------------------------------------

    string                getHandlerHeader () const
                              { return fHandlerHeader; }

    string                getOahHandlerValuesHeader () const
                              { return fHandlerValuesHeader; }

    string                getHandlerSummaryShortName () const
                              { return fHandlerSummaryShortName; }

    string                getHandlerSummaryLongName () const
                              { return fHandlerSummaryLongName; }

    string                getHandlerPreamble () const
                              { return fHandlerPreamble; }

    string                getHandlerUsage () const
                              { return fHandlerUsage; }

    string                getExecutableName () const
                              { return fHandlerExecutableName; }

    ostream&              getHandlerLogOstream ()
                              { return fHandlerLogOstream; }

    // groups lists
    const list<S_oahGroup>&
                          getHandlerGroupsList () const
                              { return fHandlerGroupsList; }

    // optional values style
    oahOptionalValuesStyleKind&
                          getHandlerOptionalValuesStyleKind ()
                              { return fHandlerOptionalValuesStyleKind; }

    const list<S_oahElement>&
                          getHandlerRegisteredElementsList () const
                              { return fHandlerRegisteredElementsList; }

    const list<S_oahElement>&
                          getHandlerCommandLineElementsList () const
                              { return fHandlerCommandLineElementsList; }
    const multiset<S_oahElement, compareOahElements>&
                          getHandlerCommandLineElementsMultiset () const
                              { return fHandlerCommandLineElementsMultiset; }

    int                   getMaximumShortNameWidth () const
                              { return fMaximumShortNameWidth; }

    int                   getMaximumLongNameWidth () const
                              { return fMaximumLongNameWidth; }

    int                   getMaximumSubGroupsHeadersSize () const
                              { return fMaximumSubGroupsHeadersSize; }

  public:

    // consistency check
    // ------------------------------------------------------

    virtual void          checkOptionsConsistency ();

  public:

    // public services
    // ------------------------------------------------------

    void                  appendPrefixToHandler (
                            S_oahPrefix prefix);

    void                  registerHandlerInItself ();

    void                  appendGroupToHandler (
                            S_oahGroup group);
    void                  prependGroupToHandler (
                            S_oahGroup group);

    void                  registerElementInHandler (
                            S_oahElement element);

    S_oahPrefix           fetchPrefixNameInPrefixesMap (
                            string prefixName);

    S_oahElement          fetchNameInElementsMap (
                            string name) const;

    oahHelpOptionsHaveBeenUsedKind
                          applyOptionsAndArgumentsFromArgcAndArgv (
                            int   argc,
                            char* argv[]);

    oahHelpOptionsHaveBeenUsedKind
                          hangleOptionsFromOptionsVector (
                            string               fakeExecutableName,
                            const optionsVector& theOptionsVector);

  public:

    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v);
    virtual void          acceptOut (basevisitor* v);

    virtual void          browseData (basevisitor* v);

  public:

    // print
    // ------------------------------------------------------

    string                commandLineAsSuppliedAsString () const;
    string                commandLineWithShortNamesAsString () const;
    string                commandLineWithLongNamesAsString () const;

    void                  print (ostream& os) const;

    void                  printHelp (ostream& os);

    void                  printOptionsSummary (ostream& os) const;
    void                  printOptionsSummary () const
                              { printOptionsSummary (fHandlerLogOstream); }

    void                  printHandlerAndGroupAndSubGroupSpecificHelp (
                            ostream&      os,
                            S_oahSubGroup subGroup) const;

    void                  printOptionNameIntrospectiveHelp (
                            ostream& os,
                            string   name) const;

    void                  printAllOahCommandLineValues (
                            ostream& os) const;

    void                  setOahHandlerFoundAHelpOption ()
                              { fHandlerFoundAHelpOption = true; }

    bool                  getOahHandlerFoundAHelpOption () const
                              { return fHandlerFoundAHelpOption; }

  private:

    // private services
    // ------------------------------------------------------

    void                  initializeHandler ();

    string                handlerOptionNamesBetweenParentheses () const;

    void                  registerElementNamesInHandler (
                            S_oahElement element);

    void                  appendSubGroupToInternalAndUserGroups (
                            S_oahSubGroup subGroup,
                            S_oahGroup    internalGroup,
                            S_oahGroup    userGroup);

    S_oahPrefix           fetchNameInPrefixesMap (
                            string name) const;

    void                  handlePrefixName (
                            string prefixName,
                            size_t equalsSignPosition,
                            string stringAfterEqualsSign);

    bool                  optionNameIsASingleCharacterOptionsCluster (
                            string optionName);

    string                decipherOption (
                            string currentString);

    void                  decipherOptionContainingEqualSign (
                            string currentOptionName,
                            size_t equalsSignPosition);

    void                  decipherOptionAndValue (
                            string optionName,
                            string optionValue);

    void                  printKnownPrefixes () const;
    void                  printKnownSingleCharacterOptions () const;
    void                  printOptionsDefaultValuesInformation () const;
    void                  printKnownOptions () const;

    S_oahElement          fetchOptionByName (
                            string name);

    void                  handleOptionName (
                            string name);

    void                  handleOptionNameAndValue (
                            string name,
                            string value);

    void                  handleHandlerName (
                            S_oahHandler handler,
                            string       name);

    void                  handleGroupName (
                            S_oahGroup group,
                            string     groupName);

    void                  handleSubGroupName (
                            S_oahSubGroup subGroup,
                            string        subGroupName);

    void                  handleAtomName (
                            S_oahAtom atom,
                            string    atomName);

    void                  handleAtomNameAndValue (
                            S_oahAtom atom,
                            string    atomName,
                            string    value);

    void                  handleOptionValueOrArgument (
                            string theString);

    void                  checkMissingPendingValuedAtomValue (
                            string atomName,
                            string context);

    virtual void          checkOptionsAndArguments ();

    void                  checkHandlerGroupsOptionsConsistency ();

  protected:

    // protected fields
    // ------------------------------------------------------

    // command line
    string                fCommandLineAsSupplied;

    // header
    string                fHandlerHeader;
    string                fHandlerValuesHeader;

    // summary
    string                fHandlerSummaryShortName;
    string                fHandlerSummaryLongName;

    // preamble
    string                fHandlerPreamble;

    // usage
    string                fHandlerUsage;

    // prefixes
    map<string, S_oahPrefix>
                          fHandlerPrefixesMap;

    // single character short names
    set<string>           fSingleCharacterShortNamesSet;

    // groups lists
    list<S_oahGroup>      fHandlerGroupsList;

    // elements map
    map<string, S_oahElement>
                          fHandlerElementsMap;

    // argument vector
    vector<string>        fHandlerArgumentsVector;

    // executable name
    string                fHandlerExecutableName;

    // optional values style
    oahOptionalValuesStyleKind
                          fHandlerOptionalValuesStyleKind;

    // log stream
    ostream&              fHandlerLogOstream;

    // has help been requested?
    // this is needed to exit if the executable is launched
    // with one or more help options,
    // i.e. options that are only used to display help to the user
    bool                  fHandlerFoundAHelpOption;

  private:

    // private services
    // ------------------------------------------------------

    int                   getMaximumVariableNameWidth () const
                              { return fMaximumVariableNameWidth; }

  private:

    // work fields
    // ------------------------------------------------------

    // all OAH elements are registered in the handler upon initialization
    list<S_oahElement>    fHandlerRegisteredElementsList;

    // those ones have be used in the command line
    list<S_oahElement>    fHandlerCommandLineElementsList;
    multiset<S_oahElement, compareOahElements>
                          fHandlerCommandLineElementsMultiset;

    // arguments handling
    bool                  fNowEverythingIsAnArgument;

    // default values style
    oahOptionsDefaultValuesStyle
                          fOahOptionsDefaultValuesStyle;

    // sizes and widths
    int                   fMaximumSubGroupsHeadersSize;

    int                   fMaximumShortNameWidth;
    int                   fMaximumLongNameWidth;

    int                   fMaximumVariableNameWidth;

    // atoms waiting for a value
    S_oahValuedAtom       fPendingValuedAtom;
};
typedef SMARTP<oahHandler> S_oahHandler;
EXP ostream& operator<< (ostream& os, const S_oahHandler& elt);

//_______________________________________________________________________________
class EXP oahDualHandler : public smartable
{
  public:

    // creation
    // ------------------------------------------------------
    static SMARTP<oahDualHandler> create (
      string   executableName,
      string   insiderAtomShortName,
      string   insiderAtomLongName,
      ostream& ios);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    oahDualHandler (
      string   executableName,
      string   insiderAtomShortName,
      string   insiderAtomLongName,
      ostream& ios);

    virtual ~oahDualHandler ();

  public:

    // set and get
    // ------------------------------------------------------

//    oahDualHandlerViewKind
//                          getOahHandlerViewKind () const
//                              { return fOahHandlerViewKind; }

    // oahHandlers
    S_oahHandler          getInsiderHandler () const
                              { return fInsiderHandler; }

    S_oahHandler          getUserOahHandler () const
                              { return fUserOahHandler; }

  public:

    // public services
    // ------------------------------------------------------

    virtual void          createInsiderHandler (
                            ostream& ios);

    virtual void          createUserHandler (
                            ostream& ios) ;

    virtual void          createUserHandlerGroups (
                            ostream& ios);

    void                  switchToInsiderView ();

    oahHandler::oahHelpOptionsHaveBeenUsedKind
                          applyOptionsAndArgumentsFromArgcAndArgv (
                            int   argc,
                            char* argv[]);

    oahHandler::oahHelpOptionsHaveBeenUsedKind
                          hangleOptionsFromOptionsVector (
                            string               fakeExecutableName,
                            const optionsVector& theOptionsVector);

  public:

    // print
    // ------------------------------------------------------

    string                asString () const;

    void                  print (ostream& os) const;

    void                  printHelp (ostream& os) const;

  protected:

    // protected services
    // ------------------------------------------------------

    void                  populateUserHandlerFromInsiderHandler ();

  protected:

    // fields
    // ------------------------------------------------------

    string                fExecutableName;

    // the insider handler provides the options used internally by the translator
    S_oahHandler          fInsiderHandler;

    // the user handler provides a user view of the options, organized by topics
    S_oahHandler          fUserOahHandler;

    // the 'insider' option names
    string                fInsiderAtomShortName;
    string                fInsiderAtomLongName;

    // the 'insider' user group is not shown
    S_oahGroup            fInsiderUserGroup;

    // the 'insider' user subgroup is a catchall
    S_oahSubGroup         fInsiderUserSubGroup;

    // the mappings
    map<string, S_oahGroup>
                          fSubGroupNamesToUserGroupsMap;
    map<string, S_oahSubGroup>
                          fAtomNamesToUserSubGroupsMap;

  private:

    // work fields
    // ------------------------------------------------------

    S_oahHandler          fOahHandlerToBeUsed; // according to '-insider'
};
typedef SMARTP<oahDualHandler> S_oahDualHandler;
EXP ostream& operator<< (ostream& os, const S_oahDualHandler& elt);

//______________________________________________________________________________
class oahDualHandlerInsiderAtom : public oahValuedAtom
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<oahDualHandlerInsiderAtom> create (
      string             shortName,
      string             longName,
      string             description,
      string             valueSpecification,
      string             variableName,
      S_oahDualHandler&  oahDualHandlerVariable);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    oahDualHandlerInsiderAtom (
      string             shortName,
      string             longName,
      string             description,
      string             valueSpecification,
      string             variableName,
      S_oahDualHandler&  oahDualHandlerVariable);

    virtual ~oahDualHandlerInsiderAtom ();

  public:

    // set and get
    // ------------------------------------------------------

    void                  setOahDualHandlerVariable (
                            S_oahDualHandler value)
                              { fOahDualHandlerVariable = value; }

  public:

    // services
    // ------------------------------------------------------

    S_oahValuedAtom       handleOptionUnderName (
                            string   optionName,
                            ostream& os);

    void                  handleValue (
                            string   theString,
                            ostream& os);

  public:

    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v);
    virtual void          acceptOut (basevisitor* v);

    virtual void          browseData (basevisitor* v);

  public:

    // print
    // ------------------------------------------------------

    virtual string        asString () const;

    void                  print (ostream& os) const;

  private:

    // fields
    // ------------------------------------------------------

    S_oahDualHandler&     fOahDualHandlerVariable;
};
typedef SMARTP<oahDualHandlerInsiderAtom> S_oahDualHandlerInsiderAtom;
EXP ostream& operator<< (ostream& os, const S_oahDualHandlerInsiderAtom& elt);


}


#endif

/*
//_______________________________________________________________________________
class EXP oahGroupsList : public smartable
{
  public:

    // data types
    // ------------------------------------------------------

    enum oahGroupsListKind {
        kUser,
        kInternal };

    static string groupsListKindAsString (
      oahGroupsListKind groupsListKind);

  public:

    // creation
    // ------------------------------------------------------
    static SMARTP<oahGroupsList> create (
      oahGroupsListKind groupsListKind);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    oahGroupsList (
      oahGroupsListKind groupsListKind);

    virtual ~oahGroupsList ();

  public:

    // set and get
    // ------------------------------------------------------

    oahGroupsListKind     getGroupsListKind () const
                              { return fGroupsListKind; }

    const list<S_oahGroup>
                          getGroupsList () const
                              { return fGroupsList; }

  public:

    // services
    // ------------------------------------------------------

    void                  appendGroupToGroupsList (
                            S_oahGroup group)
                              { fGroupsList.push_back (group); }

    void                  prependGroupToGroupsList (
                            S_oahGroup group)
                              { fGroupsList.push_front (group); }

  public:

    // print
    // ------------------------------------------------------

    void                  print (ostream& os) const;

  private:

    // fields
    // ------------------------------------------------------

    oahGroupsListKind     fGroupsListKind;

    list<S_oahGroup>      fGroupsList;

};
typedef SMARTP<oahGroupsList> S_oahGroupsList;
EXP ostream& operator<< (ostream& os, const S_oahGroupsList& elt);
*/

