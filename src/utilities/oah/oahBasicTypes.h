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

#include <list>
#include <map>
#include <vector>
#include <set>

#include "smartpointer.h"

#include "liboptionsvector.h"
#include "utilities.h"

#include "oahElements.h"


using namespace std;

namespace MusicXML2
{

//______________________________________________________________________________
// PRE-declarations for class mutual dependencies

class oahAtom;
typedef SMARTP<oahAtom> S_oahAtom;

class oahSubGroup;
typedef SMARTP<oahSubGroup> S_oahSubGroup;

class oahGroup;
typedef SMARTP<oahGroup> S_oahGroup;

class oahHandler;
typedef SMARTP<oahHandler> S_oahHandler;

// options default value styles // superfluous ??? JMI
// ------------------------------------------------------

enum oahOptionsDefaultValuesStyle {
  kGNUStyle,
  kOAHStyle
};

string optionsDefaultValuesStyleAsString (
  oahOptionsDefaultValuesStyle optionsDefaultValuesStyle);

// handler used thru...?
//______________________________________________________________________________
enum oahHandlerUsedThruKind {
  kHandlerUsedThruUnknown, // default value
  kHandlerUsedThruArgcAndArgv,
  kHandlerUsedThruOptionsVector
};

string oahHandlerUsedThruKindAsString (
  oahHandlerUsedThruKind handlerUsedThruKind);

//______________________________________________________________________________
EXP void displayOptionsVector (
  const optionsVector& theOptionsVector,
  ostream&             os);

//______________________________________________________________________________
class oahPrefix;
typedef SMARTP<oahPrefix> S_oahPrefix;

class EXP oahPrefix : public smartable
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

    string                fetchPrefixNames () const;

    void                  findStringInPrefix (
                            string        lowerCaseString,
                            list<string>& foundStringsList,
                            ostream&      os) const;

  public:

    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v);
    virtual void          acceptOut (basevisitor* v);

    virtual void          browseData (basevisitor* v);

  public:

    // print
    // ------------------------------------------------------

    string                asString () const;

    void                  printPrefixHeader (ostream& os) const;

    virtual void          printPrefixEssentials (
                            ostream& os,
                            unsigned int fieldWidth) const;

    virtual void          print (ostream& os) const;
    virtual void          printShort (ostream& os) const;

    virtual void          printHelp (ostream& os) const;

  protected:

    // protected fields
    // ------------------------------------------------------

    string                fPrefixName;
    string                fPrefixErsatz;
    string                fPrefixDescription;
};
EXP ostream& operator<< (ostream& os, const S_oahPrefix& elt);

//______________________________________________________________________________
class EXP oahAtom : public oahElement
{
  public:

    // creation
    // ------------------------------------------------------

/* this class is purely virtual
    static SMARTP<oahAtom> create (
      string         shortName,
      string         longName,
      string         description,
      oahElementKind atomValueExpectedKind);
*/

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    oahAtom (
      string         shortName,
      string         longName,
      string         description,
      oahElementKind atomValueExpectedKind);

    virtual ~oahAtom ();

  public:

    // set and get
    // ------------------------------------------------------

    // uplinks
    void                  setSubGroupUpLink (
                            S_oahSubGroup subGroup);

    S_oahSubGroup         getSubGroupUpLink () const
                              { return fSubGroupUpLink; }

  public:

    // public services
    // ------------------------------------------------------

    // uplinks
    S_oahGroup            fetchAtomGroupUpLink () const;

    S_oahHandler          fetchAtomHandlerUpLink () const;

    // atoms handling
    void                  appendAtomToElementsList (
                            S_oahHandler handler);

    void                  registerAtomAsBeingUsed ();

  public:

    // visitors
    // ------------------------------------------------------

    void                  acceptIn  (basevisitor* v) override;
    void                  acceptOut (basevisitor* v) override;

    void                  browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    void                  print (ostream& os) const override;
    void                  printShort (ostream& os) const override;

    void                  findStringInAtom (
                            string        lowerCaseString,
                            list<string>& foundStringsList,
                            ostream&      os) const;

    virtual void          printAtomWithValueOptionsValues (
                            ostream&     os,
                            unsigned int valueFieldWidth) const;

  protected:

    // protected services
    // ------------------------------------------------------

  protected:

    // protected fields
    // ------------------------------------------------------

    // uplink
    S_oahSubGroup         fSubGroupUpLink;
};
typedef SMARTP<oahAtom> S_oahAtom;
EXP ostream& operator<< (ostream& os, const S_oahAtom& elt);

//______________________________________________________________________________
class EXP oahAtomWithVariableName : public oahAtom
{
  public:

    // creation
    // ------------------------------------------------------
/* this class is purely virtual
    static SMARTP<oahAtomWithVariableName> create (
      string shortName,
      string longName,
      string description,
      string variableName);
*/
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

    // public services
    // ------------------------------------------------------

    int                   fetchVariableNameLength () const override // JMI ???
                              { return fVariableName.size (); }

  public:

    // visitors
    // ------------------------------------------------------

    void                  acceptIn  (basevisitor* v) override;
    void                  acceptOut (basevisitor* v) override;

    void                  browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    virtual void          printAtomWithVariableNameEssentials (
                            ostream& os,
                            unsigned int fieldWidth) const;
    virtual void          printAtomWithVariableNameEssentialsShort (
                            ostream& os,
                            unsigned int fieldWidth) const;

    void                  print (ostream& os) const override;
    void                  printShort (ostream& os) const override;

    void                  printAtomWithValueOptionsValues (
                            ostream&     os,
                            unsigned int valueFieldWidth) const override;

  protected:

    // protected fields
    // ------------------------------------------------------

    string                fVariableName;
};
typedef SMARTP<oahAtomWithVariableName> S_oahAtomWithVariableName;
EXP ostream& operator<< (ostream& os, const S_oahAtomWithVariableName& elt);

//______________________________________________________________________________
class EXP oahAtomWithValue : public oahAtomWithVariableName
{
  public:

    // creation
    // ------------------------------------------------------
/* this class is purely virtual
    static SMARTP<oahAtomWithValue> create (
      string shortName,
      string longName,
      string description,
      string valueSpecification,
      string variableName);
*/

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    oahAtomWithValue (
      string shortName,
      string longName,
      string description,
      string valueSpecification,
      string variableName);

    virtual ~oahAtomWithValue ();

  public:

    // set and get
    // ------------------------------------------------------

    string                getValueSpecification () const
                              { return fValueSpecification; }

    bool                  getVariableHasBeenSet () const
                              { return fVariableHasBeenSet; }

  public:

    // public services
    // ------------------------------------------------------

    void                  applyElement (ostream& os) override;
                            // repors an error

    virtual void          applyAtomWithValue (
                            string   theString,
                            ostream& os) = 0;

    virtual void          applyAtomWithValueDefaultValue (ostream& os);
                            // used only if fElementKind
                            // is equal to kElementWithOptionalValue

  public:

    // visitors
    // ------------------------------------------------------

    void                  acceptIn  (basevisitor* v) override;
    void                  acceptOut (basevisitor* v) override;

    void                  browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    virtual void          printAtomWithValueEssentials (
                            ostream& os,
                            unsigned int fieldWidth) const;
    virtual void          printAtomWithValueEssentialsShort (
                            ostream& os,
                            unsigned int fieldWidth) const;

    void                  print (ostream& os) const override;
    void                  printShort (ostream& os) const override;

    void                  printHelp (ostream& os) const override;

    virtual void          printAtomWithValueOptionsValues (
                            ostream&     os,
                            unsigned int valueFieldWidth) const override;

  protected:

    // protected fields
    // ------------------------------------------------------

    string                fValueSpecification;

    bool                  fVariableHasBeenSet;
};
typedef SMARTP<oahAtomWithValue> S_oahAtomWithValue;
EXP ostream& operator<< (ostream& os, const S_oahAtomWithValue& elt);

//_______________________________________________________________________________
class EXP oahSubGroup : public oahElement
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<oahSubGroup> create (
      string                   subGroupHeader,
      string                   shortName,
      string                   longName,
      string                   description,
      oahElementVisibilityKind optionVisibilityKind,
      S_oahGroup               groupUpLink);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    oahSubGroup (
      string                   subGroupHeader,
      string                   shortName,
      string                   longName,
      string                   description,
      oahElementVisibilityKind optionVisibilityKind,
      S_oahGroup               groupUpLink);

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
                          getSubGroupAtomsList () const
                              { return fSubGroupAtomsList; }

  public:

    // consistency check
    // ------------------------------------------------------

    virtual void          checkSubGroupOptionsConsistency ();

  public:

    // public services
    // ------------------------------------------------------

    // uplink
    S_oahHandler          fetchSubGroupHandlerUpLink () const;

    void                  appendSubGroupToElementsList (
                            S_oahHandler handler);

    void                  registerNamesInSubGroupToTheNamesToElementsMap (
                            S_oahHandler handler);

    void                  appendAtomToSubGroup (
                            S_oahAtom oahAtom);

    S_oahElement          fetchOptionByNameInSubGroup (
                            string name);

    void                  applyElement (ostream& os) override;

  public:

    // visitors
    // ------------------------------------------------------

    void                  acceptIn  (basevisitor* v) override;
    void                  acceptOut (basevisitor* v) override;

    void                  browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    void                  print (ostream& os) const override;
    void                  printShort (ostream& os) const override;

    void                  printHelp (ostream& os) const override;

    void                  printHelpWithHeaderWidth (
                            ostream& os,
                            int      subGroupHeaderWidth) const;

    void                  underlineSubGroupHeader (ostream& os) const;

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

    void                  findStringInSubGroup (
                            string        lowerCaseString,
                            list<string>& foundStringsList,
                            ostream&      os) const;

    void                  printSubGroupOptionsValues (
                            ostream&     os,
                            unsigned int valueFieldWidth) const;

  private:

    // private fields
    // ------------------------------------------------------

    S_oahGroup            fGroupUpLink;

    string                fSubGroupHeader;

    list<S_oahAtom>       fSubGroupAtomsList;

    // protected work fields
    // ------------------------------------------------------

    int                   fNumberOfUserChoseAtomsInThisSubGroup;
};
typedef SMARTP<oahSubGroup> S_oahSubGroup;
EXP ostream& operator<< (ostream& os, const S_oahSubGroup& elt);

//_______________________________________________________________________________
class EXP oahGroup : public oahElement
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<oahGroup> create (
      string                   header,
      string                   shortName,
      string                   longName,
      string                   description,
      oahElementVisibilityKind optionVisibilityKind);

    static SMARTP<oahGroup> create (
      string                   header,
      string                   shortName,
      string                   longName,
      string                   description,
      oahElementVisibilityKind optionVisibilityKind,
      S_oahHandler             groupHandlerUpLink);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    oahGroup (
      string                   header,
      string                   shortName,
      string                   longName,
      string                   description,
      oahElementVisibilityKind optionVisibilityKind);

    oahGroup (
      string                   header,
      string                   shortName,
      string                   longName,
      string                   description,
      oahElementVisibilityKind optionVisibilityKind,
      S_oahHandler             groupHandlerUpLink);

    virtual ~oahGroup ();

  public:

    // set and get
    // ------------------------------------------------------

    // uplink
    void                  setHandlerUpLink (
                            S_oahHandler handlerUpLink);

    S_oahHandler          getHandlerUpLink () const
                              { return fHandlerUpLink; }

    // header
    string                getGroupHeader () const
                              { return fGroupHeader; }

    // groups list
    const list<S_oahSubGroup>&
                          getGroupSubGroupsList () const
                              { return fGroupSubGroupsList; }

    // is the header to be written?
    void                  setGroupHeaderIsToBeWritten (bool value)
                              { fGroupHeaderIsToBeWritten = value; }

    bool                  getGroupHeaderIsToBeWritten () const
                              { return fGroupHeaderIsToBeWritten; }

    // number of chosen atoms
    void                  incrementNumberOfUserChoseAtomsInThisGroup ()
                              { fNumberOfUserChoseAtomsInThisGroup += 1; }

    // number of chosen atoms
    int                   getNumberOfUserChoseAtomsInThisGroup () const
                              { return fNumberOfUserChoseAtomsInThisGroup; }

  public:

    // quiet mode
    // ------------------------------------------------------

    virtual void          enforceGroupQuietness ();

  public:

    // consistency check
    // ------------------------------------------------------

    void                  checkGroupSubGroupsOptionsConsistency ();

    virtual void          checkGroupOptionsConsistency ();

  public:

    // public services
    // ------------------------------------------------------

    void                  appendGroupToElementsList (
                            S_oahHandler handler);

    void                  registerNamesInGroupToTheNamesToElementsMap (
                            S_oahHandler handler);

    void                  appendSubGroupToGroup (
                            S_oahSubGroup subGroup);

    S_oahElement          fetchOptionByNameInGroup (
                            string name);

    void                  applyElement (ostream& os) override;

    virtual void          handleAtomValue (
                            ostream&  os,
                            S_oahAtom atom,
                            string    theString);

  public:

    // visitors
    // ------------------------------------------------------

    void                  acceptIn  (basevisitor* v) override;
    void                  acceptOut (basevisitor* v) override;

    void                  browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    void                  print (ostream& os) const override;
    void                  printShort (ostream& os) const override;

    void                  printGroupHeader (ostream& os) const;

    void                  printHelp (ostream& os) const override;

    void                  underlineGroupHeader (ostream& os) const;

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

    void                  findStringInGroup (
                            string        lowerCaseString,
                            list<string>& foundStringsList,
                            ostream&      os) const;

    void                  printGroupOptionsValues (
                            ostream&     os,
                            unsigned int valueFieldWidth) const;

  private:

    // private services
    // ------------------------------------------------------

/* JMI STRANGE
    // prefixes
    string                prefixNamesInColumns (
                            int subGroupsShortNameFieldWidth) const;

    string                prefixNamesBetweenParentheses () const;
    string                prefixNamesInColumnsBetweenParentheses (
                            int subGroupsShortNameFieldWidth) const;

    string                operator () () const
                              { return fPrefixErsatz; }
*/

  protected:

    // protected fields
    // ------------------------------------------------------

    // uplink
    S_oahHandler          fHandlerUpLink;

    string                fGroupHeader;

    list<S_oahSubGroup>   fGroupSubGroupsList;

    bool                  fGroupHeaderIsToBeWritten;

  protected:

    // protected work fields
    // ------------------------------------------------------

    int                   fNumberOfUserChoseAtomsInThisGroup;
};
typedef SMARTP<oahGroup> S_oahGroup;
EXP ostream& operator<< (ostream& os, const S_oahGroup& elt);

//_______________________________________________________________________________
class EXP oahHandler : public smartable
{
  public:

    // creation
    // ------------------------------------------------------

 /* this class is purely virtual
    static SMARTP<oahHandler> create (
      string   executableName,
      string   executableAboutInformation,
      string   handlerHeader,
      string   handlerDescription,
      string   handlerUsage);
*/

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    oahHandler (
      string   executableName,
      string   executableAboutInformation,
      string   handlerHeader,
      string   handlerDescription,
      string   handlerUsage);

    virtual ~oahHandler ();

  protected:

    // protected initialization
    // ------------------------------------------------------

  public:

    // set and get
    // ------------------------------------------------------

    string                getHandlerExecutableName () const
                              { return fHandlerExecutableName; }
    string                getHandlerExecutableAboutInformation () const
                              { return fHandlerExecutableAboutInformation; }

    string                getHandlerHeader () const
                              { return fHandlerHeader; }

    string                getHandlerDescription () const
                              { return fHandlerDescription; }

    string                getHandlerUsage () const
                              { return fHandlerUsage; }

    // prefixes
    const map<string, S_oahPrefix>&
                          getHandlerPrefixesMap () const
                              { return fHandlerPrefixesMap; }

    // groups lists
    const list<S_oahGroup>&
                          getHandlerGroupsList () const
                              { return fHandlerGroupsList; }

/* JMI
    // optional values style
    oahOptionalValuesStyleKind
                          getHandlerOptionalValuesStyleKind () const
                              { return fHandlerOptionalValuesStyleKind; }
*/

    // handler used thru...?
    oahHandlerUsedThruKind
                          getHandlerUsedThruKind () const
                              { return fHandlerUsedThruKind; }

    // elements
    const list<S_oahElement>&
                          getHandlerElementsList () const
                              { return fHandlerElementsList; }

    const list<S_oahElement>&
                          getHandlerCommandLineElementsList () const
                              { return fHandlerCommandLineElementsList; }

    const multiset<S_oahElement, compareOahElements>&
                          getHandlerCommandLineElementsMultiset () const
                              { return fHandlerCommandLineElementsMultiset; }

    // widths and sizes
    unsigned int          getMaximumShortNameWidth () const
                              { return fMaximumShortNameWidth; }

    unsigned int          getMaximumLongNameWidth () const
                              { return fMaximumLongNameWidth; }

    unsigned int          getMaximumSubGroupsHeadersSize () const
                              { return fMaximumSubGroupsHeadersSize; }

    bool                  getOahHandlerFoundAHelpOption () const
                              { return fOahHandlerFoundAHelpOption; }

  public:

    // consistency check
    // ------------------------------------------------------

    virtual void          checkHandlerOptionsConsistency ();

  public:

    // public services
    // ------------------------------------------------------

    S_oahElement          fetchElementByNameInHandler (
                            string name);

    void                  registerPrefixInHandler (
                            S_oahPrefix prefix);

    void                  appendGroupToHandler (
                            S_oahGroup group);
    void                  prependGroupToHandler (
                            S_oahGroup group);

    void                  appendElementToElementsList (
                            S_oahElement element);

    S_oahPrefix           fetchNameInPrefixesMap (
                            string prefixName) const;

    S_oahElement          fetchNameInNamesToElementsMap (
                            string name) const;

    void                  registerElementNamesInHandler (
                            S_oahElement element);

    // options and arguments handling
    oahElementHelpOnlyKind
                          handleOptionsFromOptionsVector (
                            string               fakeExecutableName,
                            const optionsVector& theOptionsVector);

    oahElementHelpOnlyKind
                          handleOptionsAndArgumentsFromArgcAndArgv (
                            int   argc,
                            char* argv[]);

    virtual void          checkOptionsAndArgumentsFromOptionsVector ();

    virtual void          checkOptionsAndArgumentsFromArgcAndArgv () const;

    void                  setOahHandlerFoundAHelpOption (
                            string context);

    virtual string        fetchOutputFileNameFromTheOptions () const;

    // quiet mode
    virtual void          enforceHandlerQuietness ();

  public:

    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v);
    virtual void          acceptOut (basevisitor* v);

    virtual void          browseData (basevisitor* v);

  public:

    // print
    // ------------------------------------------------------

    virtual void          printHandlerEssentials (
                            ostream& os,
                            unsigned int fieldWidth) const;

    void                  printKnownPrefixes (ostream& os) const;
    void                  printKnownSingleCharacterOptions (ostream& os) const;
    void                  printOptionsDefaultValuesInformation (ostream& os) const;

    void                  displayNamesToElementsMap (ostream& os) const;
    void                  displayNamesToElementsMapShort (ostream& os) const;

    string                commandLineAsSuppliedAsString () const;
    string                commandLineWithShortNamesAsString () const;
    string                commandLineWithLongNamesAsString () const;

    void                  displayArgumentsVector (
                            const vector<string>& theArgumentsVector);

    void                  displayNamesToElementsMap ();

    void                  displayElementUsesList ();

    void                  print (ostream& os) const;
    void                  printShort (ostream& os) const;

    void                  printHelp (ostream& os) const;

    void                  printOptionsSummary (ostream& os) const;
    void                  printOptionsSummary () const
                              { printOptionsSummary (gLogStream); }

    void                  printHandlerAndGroupAndSubGroupSpecificHelp (
                            ostream&      os,
                            S_oahSubGroup subGroup) const;

    void                  printNameIntrospectiveHelp (
                            ostream& os,
                            string   name);

    void                  findStringInHandler (
                            string        lowerCaseString,
                            list<string>& foundStringsList,
                            ostream&      os) const;

    void                  printHandlerOptionsValues (
                            ostream& os) const;

  private:

    // private services
    // ------------------------------------------------------

    void                  initializeHandler ();

    // names handling
    S_oahPrefix           fetchNameInHandlerPrefixesMap (
                            string name) const;

    void                  registerNamesInHandlerToTheNamesToElementsMap ();

    void                  registerElementUse (
                            S_oahElement elementUsed,
                            string       nameUsed,
                            string       valueUsed);

    void                  handleOptionPrefixName (
                            string prefixName,
                            size_t equalsSignPosition,
                            string stringAfterEqualsSign);

    bool                  nameIsASingleCharacterOptionsCluster (
                            string optionName);

    void                  handleUnknownOptionName (
                            string optionName);

    // options and arguments handling
    // pass 1
    void                  createElementUsesListFromOptionsVector (
                            string               fakeExecutableName,
                            const optionsVector& theOptionsVector);

    void                  createElementUsesListFromArgcAndArgv (
                            int   argc,
                            char* argv[]);

    // pass 2
    oahElementHelpOnlyKind
                          applyOptionsFromElementUsesList ();

    // optionsVector elements handling
    void                  handleOptionsVectorElement (
                            string optionName,
                            string optionValue);

    void                  handleOptionsVectorOptionName (
                            string theString,
                            string valueUsed);

    void                  handleKnownOptionsVectorElementUnderName (
                            S_oahElement element,
                            string       optionNameUsed,
                            string       valueUsed);

    void                  handleKnownOptionsVectorAtomUnderName (
                            S_oahAtom atom,
                            string    optionNameUsed,
                            string    valueUsed);

    // argc/argv handling
    void                  handleArgvOptionValueOrArgument (
                            string theString);

    string                handleArgvOptionName (
                            string theString);

    void                  handleKnownArgvAtomUnderName (
                            S_oahAtom atom,
                            string    optionNameUsed);

    void                  handleKnownArgvElementUnderName (
                            S_oahElement element,
                            string       optionNameUsed);

    void                  checkMissingPendingArgvAtomWithValueValue (
                            string atomName,
                            string context);

    // common methods
    void                  handleNameContainingEqualSign (
                            string optionName,
                            size_t equalsSignPosition);

    void                  handleOptionNameCommon (
                            string theString);

    // options concistency
    void                  checkHandlerOwnOptionsConsistency ();

  protected:

    // protected fields
    // ------------------------------------------------------

    // executable
    string                fHandlerExecutableName;
    string                fHandlerExecutableAboutInformation;

    // header
    string                fHandlerHeader;

    // description
    string                fHandlerDescription;

    // usage
    string                fHandlerUsage;

    // prefixes map
    map<string, S_oahPrefix>
                          fHandlerPrefixesMap;

    // single character short names
    set<string>           fSingleCharacterShortNamesSet;

    // groups list
    list<S_oahGroup>      fHandlerGroupsList;

    // elements map
    map<string, S_oahElement>
                          fHandlerNamesToElementsMap;

    // argument vector
    vector<string>        fHandlerArgumentsVector;

/* JMI
    // optional values style
    oahOptionalValuesStyleKind
                          fHandlerOptionalValuesStyleKind;
*/

    // handler used thru...?
    oahHandlerUsedThruKind
                          fHandlerUsedThruKind;

    // has help been requested?
    // this is needed to quit if the API function is launched
    // with one or more help options,
    // i.e. options that are only used to display help to the user
    bool                  fOahHandlerFoundAHelpOption;

  private:

    // private services
    // ------------------------------------------------------

    unsigned int          getMaximumVariableNameWidth () const
                              { return fMaximumVariableNameWidth; }

  private:

    // work fields
    // ------------------------------------------------------

    // command line
    string                fCommandLineAsSupplied;

    // all OAH elements are registered in the handler upon initialization
    list<S_oahElement>    fHandlerElementsList;

    // those ones have be used in the command line
    list<S_oahElement>    fHandlerCommandLineElementsList;
    multiset<S_oahElement, compareOahElements>
                          fHandlerCommandLineElementsMultiset;

    // arguments handling
    bool                  fNowEverythingIsAnArgument; // JMI ???

    // default values style
    oahOptionsDefaultValuesStyle
                          fOahOptionsDefaultValuesStyle;

    // sizes and widths
    unsigned int          fMaximumSubGroupsHeadersSize;

    unsigned int          fMaximumShortNameWidth;
    unsigned int          fMaximumLongNameWidth;

    unsigned int          fMaximumVariableNameWidth;

    // elements uses
    list<S_oahElementUse> fElementUsesList;

    // atoms waiting for a value
    S_oahAtomWithValue    fPendingArgvAtomWithValue;
    string                fNameUsedForPendingArgvAtomWithValue;
};
typedef SMARTP<oahHandler> S_oahHandler;
EXP ostream& operator<< (ostream& os, const S_oahHandler& elt);


}


#endif

/* JMI
// optional values style
//______________________________________________________________________________
enum oahOptionalValuesStyleKind {
  kOptionalValuesStyleGNU, // default value
  kOptionalValuesStyleOAH
};

string oahOptionalValuesStyleKindAsString (
  oahOptionalValuesStyleKind optionalValuesStyleKind);

extern map<string, oahOptionalValuesStyleKind>
  gGlobalOahOptionalValuesStyleKindsMap;

string existingOahOptionalValuesStyleKinds (int namesListMaxLength);

void initializeOahOptionalValuesStyleKindsMap ();
*/

