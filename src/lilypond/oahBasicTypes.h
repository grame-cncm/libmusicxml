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

#include "tree_browser.h"
#include "visitor.h"

#include "utilities.h"

#include "msrBasicTypes.h"
#include "lpsrBasicTypes.h"

#include "setTraceOahIfDesired.h"


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

class oahValuedAtom;
typedef SMARTP<oahValuedAtom> S_oahValuedAtom;

class oahSubGroup;
typedef SMARTP<oahSubGroup> S_oahSubGroup;

class oahGroup;
typedef SMARTP<oahGroup> S_oahGroup;

class EXP oahHandler;
typedef SMARTP<oahHandler> S_oahHandler;

// data types
// ------------------------------------------------------

enum oahElementVisibilityKind {
  kElementVisibilityAlways,
  kElementVisibilityHiddenByDefault };

string optionVisibilityKindAsString (
  oahElementVisibilityKind optionVisibilityKind);

//______________________________________________________________________________
class oahElement : public smartable
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<oahElement> create (
      string                   shortName,
      string                   longName,
      string                   description,
      oahElementVisibilityKind optionVisibilityKind);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    oahElement (
      string                   shortName,
      string                   longName,
      string                   description,
      oahElementVisibilityKind optionVisibilityKind);

    virtual ~oahElement ();

  public:

    // set and get
    // ------------------------------------------------------

    // uplink
    void                  setHandlerUpLink (
                            S_oahHandler handlerUpLink)
                              { fHandlerUpLink = handlerUpLink; }

    S_oahHandler          getHandlerUpLink () const
                              { return fHandlerUpLink; }

    string                getShortName () const
                              { return fShortName; }

    string                getLongName () const
                              { return fLongName; }

    string                getDescription () const
                              { return fDescription; }

    oahElementVisibilityKind
                          getElementVisibilityKind () const
                              { return fElementVisibilityKind; }

    void                  setIsHidden ()
                              { fIsHidden = true; }

    bool                  getIsHidden () const
                              { return fIsHidden; }

    void                  setMultipleOccurrencesAllowed ()
                              { fMultipleOccurrencesAllowed = true; }

    bool                  getMultipleOccurrencesAllowed () const
                              { return fMultipleOccurrencesAllowed; }

  public:

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

    S_oahElement          fetchOptionByName (
                            string name);

    virtual S_oahValuedAtom
                          handleOptionUnderName (
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

    virtual string        asShortNamedOptionString () const;
    virtual string        asLongNamedOptionString () const;

    string                asString () const;

    virtual void          printOptionHeader (ostream& os) const; // virtual ??? JMI

    virtual void          printOptionEssentials (
                            ostream& os,
                            int      fieldWidth) const;

    virtual void          print (ostream& os) const;

    virtual void          printHelp (ostream& os);

  protected:

    // fields
    // ------------------------------------------------------

    // uplink
    S_oahHandler          fHandlerUpLink;

    string                fShortName;
    string                fLongName;
    string                fDescription;

    oahElementVisibilityKind
                          fElementVisibilityKind;

    bool                  fIsHidden;

    bool                  fMultipleOccurrencesAllowed;
};
typedef SMARTP<oahElement> S_oahElement;
EXP ostream& operator<< (ostream& os, const S_oahElement& elt);

/*
Because the set needs a comparison functor to work with. If you don't specify one, it will make a default-constructed one. In this case, since you're using a function-pointer type, the default-constructed one will be a null pointer, which can't be called; so instead, you have to provide the correct function pointer at run time.

A better approach might be to use a function class type (a.k.a. functor type); then the function call can be resolved at compile time, and a default-constructed object will do the right thing.
*/
struct compareOahElements {
  bool operator() (
    const S_oahElement firstElement,
    const S_oahElement secondElement) const;
};

/* JMI
//______________________________________________________________________________
template <typename T> class oahBrowser : public browser<T>
{
  protected:

    basevisitor*  fVisitor;

    virtual void enter (T& t) { t.acceptIn  (fVisitor); }
    virtual void leave (T& t) { t.acceptOut (fVisitor); }

  public:

    oahBrowser (basevisitor* v) : fVisitor (v)
    {}

    virtual ~oahBrowser ()
    {}

    virtual void          set (basevisitor* v)
                            { fVisitor = v; }

    virtual               void browse (T& t)
                            {
                              enter (t);

                              t.browseData (fVisitor);

                              leave (t);
                            }
};
*/

template <typename T> class oahBrowser : public browser <T>
{
  public:

    oahBrowser (basevisitor* v) : fVisitor (v) {}

    virtual ~oahBrowser () {}

  public:

    virtual void set (basevisitor* v) { fVisitor = v; }

    virtual void browse (T& t) {
      enter (t);

      t.browseData (fVisitor);

      leave (t);
    }

  protected:

    basevisitor*  fVisitor;

    virtual void enter (T& t) { t.acceptIn  (fVisitor); }
    virtual void leave (T& t) { t.acceptOut (fVisitor); }
};



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

  public:

    // services
    // ------------------------------------------------------

    void                  registerAtomInHandler (
                            S_oahHandler handler);

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

    void                  setValueIsOptional ();

    bool                  getValueIsOptional () const
                              { return fValueIsOptional; }

  public:

    // services
    // ------------------------------------------------------

    virtual void          handleValue (
                            string   theString,
                            ostream& os) = 0;

    virtual void          handleDefaultValue ();
                            // used only fValueIsOptional is true

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

    bool                  fValueIsOptional; // not yet supported JMI
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
    string                asLongNamedOptionString () const;

    void                  print (ostream& os) const;

    void                  printAtomOptionsValues (
                            ostream& os,
                            int      valueFieldWidth) const;
  private:

    // fields
    // ------------------------------------------------------

    int&                  fIntegerVariable;
};
typedef SMARTP<oahIntegerAtom> S_oahIntegerAtom;
EXP ostream& operator<< (ostream& os, const S_oahIntegerAtom& elt);

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
                              { fFloatVariable = value; }

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
    string                asLongNamedOptionString () const;

    void                  print (ostream& os) const;

    void                  printAtomOptionsValues (
                            ostream& os,
                            int      valueFieldWidth) const;

  private:

    // fields
    // ------------------------------------------------------

    float&                fFloatVariable;
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
    string                asLongNamedOptionString () const;

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
    string                asLongNamedOptionString () const;

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
    string                asLongNamedOptionString () const;

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
class oahNaturalNumbersSetElementAtom : public oahValuedAtom
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<oahNaturalNumbersSetElementAtom> create (
      string    shortName,
      string    longName,
      string    description,
      string    valueSpecification,
      string    variableName,
      set<int>& naturalNumbersSetVariable);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    oahNaturalNumbersSetElementAtom (
      string    shortName,
      string    longName,
      string    description,
      string    valueSpecification,
      string    variableName,
      set<int>& naturalNumbersSetVariable);

    virtual ~oahNaturalNumbersSetElementAtom ();

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
    string                asLongNamedOptionString () const;

    void                  print (ostream& os) const;

    void                  printAtomOptionsValues (
                            ostream& os,
                            int      valueFieldWidth) const;

  private:

    // fields
    // ------------------------------------------------------

    set<int>&             fNaturalNumbersSetVariable;
};
typedef SMARTP<oahNaturalNumbersSetElementAtom> S_oahNaturalNumbersSetElementAtom;
EXP ostream& operator<< (ostream& os, const S_oahNaturalNumbersSetElementAtom& elt);

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
    string                asLongNamedOptionString () const;

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
class oahStringsSetElementAtom : public oahValuedAtom
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<oahStringsSetElementAtom> create (
      string       shortName,
      string       longName,
      string       description,
      string       valueSpecification,
      string       variableName,
      set<string>& stringsSetVariable);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    oahStringsSetElementAtom (
      string       shortName,
      string       longName,
      string       description,
      string       valueSpecification,
      string       variableName,
      set<string>& stringsSetVariable);

    virtual ~oahStringsSetElementAtom ();

  public:

    // set and get
    // ------------------------------------------------------

    void                  setStringsSetVariable (
                            set<string> value)
                              { fStringsSetVariable = value; }

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
    string                asLongNamedOptionString () const;

    void                  print (ostream& os) const;

    void                  printAtomOptionsValues (
                            ostream& os,
                            int      valueFieldWidth) const;

  private:

    // fields
    // ------------------------------------------------------

    set<string>&          fStringsSetVariable;
};
typedef SMARTP<oahStringsSetElementAtom> S_oahStringsSetElementAtom;
EXP ostream& operator<< (ostream& os, const S_oahStringsSetElementAtom& elt);

//______________________________________________________________________________
class oahStringsSetAtom : public oahValuedAtom
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<oahStringsSetAtom> create (
      string       shortName,
      string       longName,
      string       description,
      string       valueSpecification,
      string       variableName,
      set<string>& stringsSetVariable);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    oahStringsSetAtom (
      string       shortName,
      string       longName,
      string       description,
      string       valueSpecification,
      string       variableName,
      set<string>& stringsSetVariable);

    virtual ~oahStringsSetAtom ();

  public:

    // set and get
    // ------------------------------------------------------

    void                  setStringsSetVariable (
                            set<string> value)
                              { fStringsSetVariable = value; }

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
    string                asLongNamedOptionString () const;

    void                  print (ostream& os) const;

    void                  printAtomOptionsValues (
                            ostream& os,
                            int      valueFieldWidth) const;

  private:

    // fields
    // ------------------------------------------------------

    set<string>&          fStringsSetVariable;
};
typedef SMARTP<oahStringsSetAtom> S_oahStringsSetAtom;
EXP ostream& operator<< (ostream& os, const S_oahStringsSetAtom& elt);

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
    string                asLongNamedOptionString () const;

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
    string                asLongNamedOptionString () const;

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
                            S_oahGroup oahGroup)
                              { fGroupUpLink = oahGroup; }

    S_oahGroup            getGroupUpLink () const
                              { return fGroupUpLink; }

    string                getSubGroupHeader () const
                              { return fSubGroupHeader; }

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
};
typedef SMARTP<oahGroup> S_oahGroup;
EXP ostream& operator<< (ostream& os, const S_oahGroup& elt);

//_______________________________________________________________________________
class EXP oahHandler : public oahElement
{
    // data types
    // ------------------------------------------------------

    enum oahOptionsDefaultValuesStyle {
        kGNUStyle,
        kOAHStyle };

    static string optionsDefaultValuesStyleAsString (
      oahOptionsDefaultValuesStyle optionsDefaultValuesStyle);

  public:

    // creation
    // ------------------------------------------------------
/* JMI this a pure virtual class
    static SMARTP<oahHandler> create (
      string           handlerHeader,
      string           handlerValuesHeader,
      string           optionHoahHandlerandlerHelpShortName,
      string           handlerLongName,
      string           handlerSummaryShortName,
      string           handlerSummaryLongName,
      string           handlerPreamble,
      string           handlerUsage,
      string           handlerDescription,
      indentedOstream& handlerLogOstream);
*/

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    oahHandler (
      string           handlerHeader,
      string           handlerValuesHeader,
      string           handlerShortName,
      string           handlerLongName,
      string           handlerSummaryShortName,
      string           handlerSummaryLongName,
      string           handlerPreamble,
      string           handlerUsage,
      string           handlerDescription,
      indentedOstream& handlerLogOstream);

    virtual ~oahHandler ();

  private:

    // initialization
    // ------------------------------------------------------

    virtual void          initializeOptionsHandler (
                            string executableName) = 0;

  public:

    // set and get
    // ------------------------------------------------------

    string                getHandlerHeader () const
                              { return fHandlerHeader; }

    string                getOptionsHandlerValuesHeader () const
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

    S_oahPrefix           fetchPrefixInMapByItsName (
                            string prefixName);

    void                  appendGroupToHandler (
                            S_oahGroup oahGroup);
    void                  prependGroupToHandler (
                            S_oahGroup oahGroup);

    void                  registerElementInHandler (
                            S_oahElement element);

    void                  registerHandlerInItself ();

    S_oahPrefix           fetchPrefixFromMap (
                            string name) const;

    S_oahElement          fetchElementFromMap (
                            string name) const;

    void                  handlePrefixName (
                            string prefixName,
                            size_t equalsSignPosition,
                            string stringAfterEqualsSign);

    bool                  optionNameIsASingleCharacterOptionsCluster (
                            string optionName);

    const vector<string>  decipherOptionsAndArguments (
                            int   argc,
                            char* argv[]);

    string                decipherOption (
                            string currentString);

    void                  decipherOptionContainingEqualSign (
                            string currentOptionName,
                            size_t equalsSignPosition);

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

    void                  setOptionsHandlerFoundAHelpOption ()
                              { fHandlerFoundAHelpOption = true; }

    bool                  getOptionsHandlerFoundAHelpOption () const
                              { return fHandlerFoundAHelpOption; }

  private:

    // private services
    // ------------------------------------------------------

    string                handlerOptionNamesBetweenParentheses () const;

    void                  registerElementNamesInHandler (
                            S_oahElement element);

    void                  printKnownPrefixes () const;
    void                  printKnownSingleCharacterOptions () const;
    void                  printOptionsDefaultValuesInformation () const;
    void                  printKnownOptions () const;

    S_oahElement          fetchOptionByName (
                            string name);

    void                  handleOptionName (
                            string name);

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

    void                  handleOptionValueOrArgument (
                            string theString);

    void                  checkMissingPendingValuedAtomValue (
                            string atomName,
                            string context);

    virtual void          checkOptionsAndArguments () = 0;

    void                  checkHandlerGroupsOptionsConsistency ();

  protected:

    // protected fields
    // ------------------------------------------------------

    string                fCommandLineAsSupplied;

    string                fHandlerHeader;
    string                fHandlerValuesHeader;

    string                fHandlerSummaryShortName;
    string                fHandlerSummaryLongName;

    string                fHandlerPreamble;

    string                fHandlerUsage;

    map<string, S_oahPrefix>
                          fHandlerPrefixesMap;

    set<string>           fSingleCharacterShortNamesSet;

    list<S_oahGroup>      fHandlerGroupsList;

    map<string, S_oahElement>
                          fHandlerElementsMap;

    vector<string>        fHandlerArgumentsVector;

    string                fHandlerExecutableName;

    oahOptionalValuesStyleKind
                          fHandlerOptionalValuesStyleKind;

    ostream&              fHandlerLogOstream;

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

    bool                  fNowEverythingIsAnArgument;

    oahOptionsDefaultValuesStyle
                          fOahOptionsDefaultValuesStyle;

    int                   fMaximumSubGroupsHeadersSize;

    int                   fMaximumShortNameWidth;
    int                   fMaximumLongNameWidth;

    int                   fMaximumVariableNameWidth;

    S_oahValuedAtom       fPendingValuedAtom;
};
typedef SMARTP<oahHandler> S_oahHandler;
EXP ostream& operator<< (ostream& os, const S_oahHandler& elt);


}


#endif
