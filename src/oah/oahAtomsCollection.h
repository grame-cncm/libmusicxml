/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef ___oahAtomsCollection___
#define ___oahAtomsCollection___

#include "list"
#include "map"
#include "set"

#include "rational.h"

#include "msrBasicTypes.h"
#include "msrMidi.h"

#include "oahBasicTypes.h"


using namespace std;

namespace MusicXML2
{

//______________________________________________________________________________
class EXP oahAtomSynonym : public oahAtom
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

    // public services
    // ------------------------------------------------------

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

    void                  printAtomWithValueOptionsValues (
                            ostream&     os,
                            unsigned int valueFieldWidth) const override;

  private:

    // private fields
    // ------------------------------------------------------

     S_oahAtom            fOriginalOahAtom;
};
typedef SMARTP<oahAtomSynonym> S_oahAtomSynonym;
EXP ostream& operator<< (ostream& os, const S_oahAtomSynonym& elt);

//______________________________________________________________________________
class EXP oahHelpOnlyAtom : public oahAtom
{
  public:

    // creation
    // ------------------------------------------------------

/* this class is purely virtual
    static SMARTP<oahHelpOnlyAtom> create (
      string shortName,
      string longName,
      string description,
      string executableName);
*/

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    oahHelpOnlyAtom (
      string shortName,
      string longName,
      string description,
      string executableName);

    virtual ~oahHelpOnlyAtom ();

  public:

    // set and get
    // ------------------------------------------------------

  public:

    // public services
    // ------------------------------------------------------

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

    void                  printAtomWithValueOptionsValues (
                            ostream&     os,
                            unsigned int valueFieldWidth) const override;

  protected:

    // protected fields
    // ------------------------------------------------------

    string                fHelpOnlyAtomExecutableName;
};
typedef SMARTP<oahHelpOnlyAtom> S_oahHelpOnlyAtom;
EXP ostream& operator<< (ostream& os, const S_oahHelpOnlyAtom& elt);

//______________________________________________________________________________
class EXP oahOptionsUsageAtom : public oahHelpOnlyAtom
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<oahOptionsUsageAtom> create (
      string shortName,
      string longName,
      string description,
      string executableName);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    oahOptionsUsageAtom (
      string shortName,
      string longName,
      string description,
      string executableName);

    virtual ~oahOptionsUsageAtom ();

  public:

    // set and get
    // ------------------------------------------------------

  public:

    // public services
    // ------------------------------------------------------

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

    void                  printOptionsUsage (ostream& os) const;

    void                  print (ostream& os) const override;

    void                  printAtomWithValueOptionsValues (
                            ostream&     os,
                            unsigned int valueFieldWidth) const override;

  private:

    // private fields
    // ------------------------------------------------------
};
typedef SMARTP<oahOptionsUsageAtom> S_oahOptionsUsageAtom;
EXP ostream& operator<< (ostream& os, const S_oahOptionsUsageAtom& elt);

//______________________________________________________________________________
class EXP oahHelpAtom : public oahHelpOnlyAtom
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<oahHelpAtom> create (
      string shortName,
      string longName,
      string description,
      string executableName);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    oahHelpAtom (
      string shortName,
      string longName,
      string description,
      string executableName);

    virtual ~oahHelpAtom ();

  public:

    // set and get
    // ------------------------------------------------------

  public:

    // public services
    // ------------------------------------------------------

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

    void                  printOptionsSummary (ostream& os) const;

    void                  printAtomWithValueOptionsValues (
                            ostream&     os,
                            unsigned int valueFieldWidth) const override;

  private:

    // private fields
    // ------------------------------------------------------
};
typedef SMARTP<oahHelpAtom> S_oahHelpAtom;
EXP ostream& operator<< (ostream& os, const S_oahHelpAtom& elt);

//______________________________________________________________________________
class EXP oahHelpSummaryAtom : public oahHelpOnlyAtom
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<oahHelpSummaryAtom> create (
      string shortName,
      string longName,
      string description,
      string executableName);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    oahHelpSummaryAtom (
      string shortName,
      string longName,
      string description,
      string executableName);

    virtual ~oahHelpSummaryAtom ();

  public:

    // set and get
    // ------------------------------------------------------

  public:

    // public services
    // ------------------------------------------------------

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

    void                  printOptionsSummary (ostream& os) const;

    void                  printAtomWithValueOptionsValues (
                            ostream&     os,
                            unsigned int valueFieldWidth) const override;

  private:

    // private fields
    // ------------------------------------------------------
};
typedef SMARTP<oahHelpSummaryAtom> S_oahHelpSummaryAtom;
EXP ostream& operator<< (ostream& os, const S_oahHelpSummaryAtom& elt);

//______________________________________________________________________________
class EXP oahAboutAtom : public oahHelpOnlyAtom
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<oahAboutAtom> create (
      string shortName,
      string longName,
      string description,
      string executableName);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    oahAboutAtom (
      string shortName,
      string longName,
      string description,
      string executableName);

    virtual ~oahAboutAtom ();

  public:

    // set and get
    // ------------------------------------------------------

  public:

    // public services
    // ------------------------------------------------------

    void                  applyElement (ostream& os) override;

  public:

    // visitors
    // ------------------------------------------------------

    void                  acceptIn  (basevisitor* v) override;
    void                  acceptOut (basevisitor* v) override;

    void                  browseData (basevisitor* v) override;

    // print
    // ------------------------------------------------------

    void                  print (ostream& os) const override;

    void                  printAbout (ostream& os) const;

    void                  printAtomWithValueOptionsValues (
                            ostream&     os,
                            unsigned int valueFieldWidth) const override;

  private:

    // private fields
    // ------------------------------------------------------
};
typedef SMARTP<oahAboutAtom> S_oahAboutAtom;
EXP ostream& operator<< (ostream& os, const S_oahAboutAtom& elt);

//______________________________________________________________________________
class EXP oahVersionAtom : public oahHelpOnlyAtom
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<oahVersionAtom> create (
      string shortName,
      string longName,
      string description,
      string executableName);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    oahVersionAtom (
      string shortName,
      string longName,
      string description,
      string executableName);

    virtual ~oahVersionAtom ();

  public:

    // set and get
    // ------------------------------------------------------

  public:

    // public services
    // ------------------------------------------------------

    void                  applyElement (ostream& os) override;

  public:

    // visitors
    // ------------------------------------------------------

    void                  acceptIn  (basevisitor* v) override;
    void                  acceptOut (basevisitor* v) override;

    void                  browseData (basevisitor* v) override;

    // print
    // ------------------------------------------------------

    void                  print (ostream& os) const override;

    void                  printVersion (ostream& os) const;

    void                  printAtomWithValueOptionsValues (
                            ostream&     os,
                            unsigned int valueFieldWidth) const override;

  private:

    // private fields
    // ------------------------------------------------------
};
typedef SMARTP<oahVersionAtom> S_oahVersionAtom;
EXP ostream& operator<< (ostream& os, const S_oahVersionAtom& elt);

//______________________________________________________________________________
class EXP oahContactAtom : public oahHelpOnlyAtom
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<oahContactAtom> create (
      string shortName,
      string longName,
      string description,
      string executableName);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    oahContactAtom (
      string shortName,
      string longName,
      string description,
      string executableName);


    virtual ~oahContactAtom ();

  public:

    // set and get
    // ------------------------------------------------------

  public:

    // public services
    // ------------------------------------------------------

    void                  applyElement (ostream& os) override;

  public:

    // visitors
    // ------------------------------------------------------

    void                  acceptIn  (basevisitor* v) override;
    void                  acceptOut (basevisitor* v) override;

    void                  browseData (basevisitor* v) override;

    // print
    // ------------------------------------------------------

    void                  print (ostream& os) const override;

    void                  printContact (ostream& os) const;

    void                  printAtomWithValueOptionsValues (
                            ostream&     os,
                            unsigned int valueFieldWidth) const override;

  private:

    // private fields
    // ------------------------------------------------------
};
typedef SMARTP<oahContactAtom> S_oahContactAtom;
EXP ostream& operator<< (ostream& os, const S_oahContactAtom& elt);

//______________________________________________________________________________
class EXP oahBooleanAtom : public oahAtom
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

    void                  setBooleanVariable (bool value);

    bool                  getBooleanVariable () const
                              { return fBooleanVariable; }

    bool                  getVariableHasBeenSet () const
                              { return fVariableHasBeenSet; }

  public:

    // public services
    // ------------------------------------------------------

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

    void                  printAtomWithValueOptionsValues (
                            ostream&     os,
                            unsigned int valueFieldWidth) const override;

  protected:

    // protected fields
    // ------------------------------------------------------

    bool&                 fBooleanVariable;
    string                fVariableName;

    bool                  fVariableHasBeenSet;
};
typedef SMARTP<oahBooleanAtom> S_oahBooleanAtom;
EXP ostream& operator<< (ostream& os, const S_oahBooleanAtom& elt);

//______________________________________________________________________________
class EXP oahTwoBooleansAtom : public oahBooleanAtom
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

    void                  setTwoBooleansVariables (bool value);

  public:

    // public services
    // ------------------------------------------------------

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

    void                  printAtomWithValueOptionsValues (
                            ostream&     os,
                            unsigned int valueFieldWidth) const override;

  private:

    // private fields
    // ------------------------------------------------------

    bool&                 fBooleanSecondaryVariable;
};
typedef SMARTP<oahTwoBooleansAtom> S_oahTwoBooleansAtom;
EXP ostream& operator<< (ostream& os, const S_oahTwoBooleansAtom& elt);

//______________________________________________________________________________
class EXP oahThreeBooleansAtom : public oahBooleanAtom
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

    void                  setThreeBooleansVariables (bool value);

  public:

    // public services
    // ------------------------------------------------------

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

    void                  printAtomWithValueOptionsValues (
                            ostream&     os,
                            unsigned int valueFieldWidth) const override;

  private:

    // private fields
    // ------------------------------------------------------

    bool&                 fBooleanSecondaryVariable;
    bool&                 fBooleanTertiaryVariable;
};
typedef SMARTP<oahThreeBooleansAtom> S_oahThreeBooleansAtom;
EXP ostream& operator<< (ostream& os, const S_oahThreeBooleansAtom& elt);

//______________________________________________________________________________
class EXP oahCombinedBooleansAtom : public oahAtomWithVariableName
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
                          getBooleanAtomsList () const
                              { return fBooleanAtomsList; }

  public:

    // public services
    // ------------------------------------------------------

    void                  addBooleanAtom (
                            S_oahBooleanAtom booleanAtom);

    void                  addBooleanAtomByName (
                            string name);

    void                  setCombinedBooleanVariables (bool value);

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

    virtual void          printCombinedBooleansEssentials (
                            ostream& os,
                            unsigned int fieldWidth) const;
    virtual void          printCombinedBooleansEssentialsShort (
                            ostream& os,
                            unsigned int fieldWidth) const;

    void                  print (ostream& os) const override;

    void                  printHelp (ostream& os) const override;

    void                  printAtomWithValueOptionsValues (
                            ostream&     os,
                            unsigned int valueFieldWidth) const override;

  private:

    // private fields
    // ------------------------------------------------------

    bool&                 fBooleanVariable;

    list<S_oahBooleanAtom>
                          fBooleanAtomsList;
};
typedef SMARTP<oahCombinedBooleansAtom> S_oahCombinedBooleansAtom;
EXP ostream& operator<< (ostream& os, const S_oahCombinedBooleansAtom& elt);

//______________________________________________________________________________
class EXP oahMultiplexBooleansAtom : public oahAtom
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<oahMultiplexBooleansAtom> create (
      string      shortName,
      string      longName,
      string      description,
      string      shortSuffixDescriptor,
      string      longSuffixDescriptor,
      S_oahPrefix shortNamesPrefix,
      S_oahPrefix longNamesPrefix);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    oahMultiplexBooleansAtom (
      string      shortName,
      string      longName,
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
                          getBooleanAtomsList () const
                              { return fBooleanAtomsList; }

  public:

    // public services
    // ------------------------------------------------------

    void                  addBooleanAtom (
                            S_oahBooleanAtom booleanAtom);

    void                  addBooleanAtomByName (
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

    void                  printHelp (ostream& os) const override;

    void                  printAtomWithValueOptionsValues (
                            ostream&     os,
                            unsigned int valueFieldWidth) const override;

  private:

    // private fields
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

    // we need unique short and long names
    static int            sMultiplexAtomsCounter;
};
typedef SMARTP<oahMultiplexBooleansAtom> S_oahMultiplexBooleansAtom;
EXP ostream& operator<< (ostream& os, const S_oahMultiplexBooleansAtom& elt);

//______________________________________________________________________________
class EXP oahIntegerAtom : public oahAtomWithValue
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

    void                  setIntegerVariable (int value);

    int                   getIntegerVariable () const
                              { return fIntegerVariable; }

  public:

    // public services
    // ------------------------------------------------------

    void                  applyAtomWithValue (
                            string   theString,
                            ostream& os) override;

  public:

    // visitors
    // ------------------------------------------------------

    void                  acceptIn  (basevisitor* v) override;
    void                  acceptOut (basevisitor* v) override;

    void                  browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    string                asShortNamedOptionString () const override;
    string                asActualLongNamedOptionString () const override;

    void                  print (ostream& os) const override;

    void                  printAtomWithValueOptionsValues (
                            ostream&     os,
                            unsigned int valueFieldWidth) const override;
  protected:

    // protected fields
    // ------------------------------------------------------

    int&                  fIntegerVariable;
};
typedef SMARTP<oahIntegerAtom> S_oahIntegerAtom;
EXP ostream& operator<< (ostream& os, const S_oahIntegerAtom& elt);

//______________________________________________________________________________
class EXP oahTwoIntegersAtom : public oahIntegerAtom
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

    void                  setIntegerVariable (int value);

  public:

    // public services
    // ------------------------------------------------------

    void                  applyAtomWithValue (
                            string   theString,
                            ostream& os) override;

  public:

    // visitors
    // ------------------------------------------------------

    void                  acceptIn  (basevisitor* v) override;
    void                  acceptOut (basevisitor* v) override;

    void                  browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    string                asShortNamedOptionString () const override;
    string                asActualLongNamedOptionString () const override;

    void                  print (ostream& os) const override;

    void                  printAtomWithValueOptionsValues (
                            ostream&     os,
                            unsigned int valueFieldWidth) const override;
  private:

    // private fields
    // ------------------------------------------------------

    int&                  fIntegerSecondaryVariable;
};
typedef SMARTP<oahTwoIntegersAtom> S_oahTwoIntegersAtom;
EXP ostream& operator<< (ostream& os, const S_oahTwoIntegersAtom& elt);

//______________________________________________________________________________
class EXP oahFloatAtom : public oahAtomWithValue
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

    void                  setFloatVariable (float value);

    float                 getFloatVariable () const
                              { return fFloatVariable; }

  public:

    // public services
    // ------------------------------------------------------

    void                  applyAtomWithValue (
                            string   theString,
                            ostream& os) override;

  public:

    // visitors
    // ------------------------------------------------------

    void                  acceptIn  (basevisitor* v) override;
    void                  acceptOut (basevisitor* v) override;

    void                  browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    string                asShortNamedOptionString () const override;
    string                asActualLongNamedOptionString () const override;

    void                  print (ostream& os) const override;

    void                  printAtomWithValueOptionsValues (
                            ostream&     os,
                            unsigned int valueFieldWidth) const override;

  private:

    // private fields
    // ------------------------------------------------------

    float&                fFloatVariable;
};
typedef SMARTP<oahFloatAtom> S_oahFloatAtom;
EXP ostream& operator<< (ostream& os, const S_oahFloatAtom& elt);

//______________________________________________________________________________
class EXP oahStringAtom : public oahAtomWithValue
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

    void                  setStringVariable (string value);

    string                getStringVariable () const
                              { return fStringVariable; }

  public:

    // public services
    // ------------------------------------------------------

    void                  applyAtomWithValue (
                            string   theString,
                            ostream& os) override;

  public:

    // visitors
    // ------------------------------------------------------

    void                  acceptIn  (basevisitor* v) override;
    void                  acceptOut (basevisitor* v) override;

    void                  browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    string                asShortNamedOptionString () const override;
    string                asActualLongNamedOptionString () const override;

    void                  print (ostream& os) const override;

    void                  printAtomWithValueOptionsValues (
                            ostream&     os,
                            unsigned int valueFieldWidth) const override;

  protected:

    // protected fields
    // ------------------------------------------------------

    string&               fStringVariable;
};
typedef SMARTP<oahStringAtom> S_oahStringAtom;
EXP ostream& operator<< (ostream& os, const S_oahStringAtom& elt);

//______________________________________________________________________________
class EXP oahMonoplexStringAtom : public oahAtom
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<oahMonoplexStringAtom> create (
      string shortName,
      string longName,
      string description,
      string atomNameDescriptor,
      string stringValueDescriptor);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    oahMonoplexStringAtom (
      string shortName,
      string longName,
      string description,
      string atomNameDescriptor,
      string stringValueDescriptor);

    virtual ~oahMonoplexStringAtom ();

  public:

    // set and get
    // ------------------------------------------------------

    const list<S_oahStringAtom>&
                          getStringAtomsList () const
                              { return fStringAtomsList; }

  public:

    // public services
    // ------------------------------------------------------

    void                  addStringAtom (
                            S_oahStringAtom stringAtom);

    void                  addStringAtomByName (
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

    void                  printHelp (ostream& os) const override;

    void                  printAtomWithValueOptionsValues (
                            ostream&     os,
                            unsigned int valueFieldWidth) const override;

  private:

    // private fields
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
class EXP oahStringWithDefaultValueAtom : public oahStringAtom
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

    void                  setStringVariable (string value);

    string                getStringVariable () const
                              { return fStringVariable; }

  public:

    // public services
    // ------------------------------------------------------

    void                  applyAtomWithValue (
                            string   theString,
                            ostream& os) override;

  public:

    // visitors
    // ------------------------------------------------------

    void                  acceptIn  (basevisitor* v) override;
    void                  acceptOut (basevisitor* v) override;

    void                  browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    string                asShortNamedOptionString () const override;
    string                asActualLongNamedOptionString () const override;

    void                  print (ostream& os) const override;

    void                  printAtomWithValueOptionsValues (
                            ostream&     os,
                            unsigned int valueFieldWidth) const override;

  protected:

    // protected fields
    // ------------------------------------------------------

    string                fDefaultStringValue;
};
typedef SMARTP<oahStringWithDefaultValueAtom> S_oahStringWithDefaultValueAtom;
EXP ostream& operator<< (ostream& os, const S_oahStringWithDefaultValueAtom& elt);

//______________________________________________________________________________
class EXP oahRationalAtom : public oahAtomWithValue
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
                              {
                                fRationalVariable = value;
                                fVariableHasBeenSet = true;
                              }

    rational              getRationalVariable () const
                              { return fRationalVariable; }

  public:

    // public services
    // ------------------------------------------------------

    void                  applyAtomWithValue (
                            string   theString,
                            ostream& os) override;

  public:

    // visitors
    // ------------------------------------------------------

    void                  acceptIn  (basevisitor* v) override;
    void                  acceptOut (basevisitor* v) override;

    void                  browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    string                asShortNamedOptionString () const override;
    string                asActualLongNamedOptionString () const override;

    void                  print (ostream& os) const override;

    void                  printAtomWithValueOptionsValues (
                            ostream&     os,
                            unsigned int valueFieldWidth) const override;

  private:

    // private fields
    // ------------------------------------------------------

    rational&             fRationalVariable;
};
typedef SMARTP<oahRationalAtom> S_oahRationalAtom;
EXP ostream& operator<< (ostream& os, const S_oahRationalAtom& elt);

//______________________________________________________________________________
class EXP oahNaturalNumbersSetAtom : public oahAtomWithValue
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
                            set<int>& value)
                              {
                                fNaturalNumbersSetVariable = value;
                                fVariableHasBeenSet = true;
                              }

    const set<int>&       getNaturalNumbersSetVariable () const
                              { return fNaturalNumbersSetVariable; }

  public:

    // public services
    // ------------------------------------------------------

    void                  applyAtomWithValue (
                            string   theString,
                            ostream& os) override;

  public:

    // visitors
    // ------------------------------------------------------

    void                  acceptIn  (basevisitor* v) override;
    void                  acceptOut (basevisitor* v) override;

    void                  browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    string                asShortNamedOptionString () const override;
    string                asActualLongNamedOptionString () const override;

    void                  print (ostream& os) const override;

    void                  printAtomWithValueOptionsValues (
                            ostream&     os,
                            unsigned int valueFieldWidth) const override;

  private:

    // private fields
    // ------------------------------------------------------

    set<int>&             fNaturalNumbersSetVariable;
};
typedef SMARTP<oahNaturalNumbersSetAtom> S_oahNaturalNumbersSetAtom;
EXP ostream& operator<< (ostream& os, const S_oahNaturalNumbersSetAtom& elt);

//______________________________________________________________________________
class EXP oahRGBColorAtom : public oahAtomWithValue
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
      msrRGBColor& RGBColorVariable);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    oahRGBColorAtom (
      string       shortName,
      string       longName,
      string       description,
      string       valueSpecification,
      string       variableName,
      msrRGBColor& RGBColorVariable);

    virtual ~oahRGBColorAtom ();

  public:

    // set and get
    // ------------------------------------------------------

    void                  setRGBColorVariable (
                            msrRGBColor& value)
                              {
                                fRGBColorVariable = value;
                                fVariableHasBeenSet = true;
                              }

    const msrRGBColor&    getRGBColorVariable () const
                              { return fRGBColorVariable; }

  public:

    // public services
    // ------------------------------------------------------

    void                  applyAtomWithValue (
                            string   theString,
                            ostream& os) override;

  public:

    // visitors
    // ------------------------------------------------------

    void                  acceptIn  (basevisitor* v) override;
    void                  acceptOut (basevisitor* v) override;

    void                  browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    string                asShortNamedOptionString () const override;
    string                asActualLongNamedOptionString () const override;

    void                  print (ostream& os) const override;

    void                  printAtomWithValueOptionsValues (
                            ostream&     os,
                            unsigned int valueFieldWidth) const override;

  private:

    // private fields
    // ------------------------------------------------------

    msrRGBColor&          fRGBColorVariable;
};
typedef SMARTP<oahRGBColorAtom> S_oahRGBColorAtom;
EXP ostream& operator<< (ostream& os, const S_oahRGBColorAtom& elt);

//______________________________________________________________________________
class EXP oahIntSetAtom : public oahAtomWithValue
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

    const set<int>&       getIntSetVariable () const
                              { return fIntSetVariable; }
// JMI                                 fVariableHasBeenSet = true;

  public:

    // public services
    // ------------------------------------------------------

    void                  applyAtomWithValue (
                            string   theString,
                            ostream& os) override;

  public:

    // visitors
    // ------------------------------------------------------

    void                  acceptIn  (basevisitor* v) override;
    void                  acceptOut (basevisitor* v) override;

    void                  browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    string                asShortNamedOptionString () const override;
    string                asActualLongNamedOptionString () const override;

    void                  print (ostream& os) const override;

    void                  printAtomWithValueOptionsValues (
                            ostream&     os,
                            unsigned int valueFieldWidth) const override;

  private:

    // private fields
    // ------------------------------------------------------

    set<int>&             fIntSetVariable;
};
typedef SMARTP<oahIntSetAtom> S_oahIntSetAtom;
EXP ostream& operator<< (ostream& os, const S_oahIntSetAtom& elt);

//______________________________________________________________________________
class EXP oahStringSetAtom : public oahAtomWithValue
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
                              {
                                fStringSetVariable.insert (partName);
                                fVariableHasBeenSet = true;
                              }

    const set<string>&    getStringSetVariable () const
                              { return fStringSetVariable; }

  public:

    // public services
    // ------------------------------------------------------

    void                  applyAtomWithValue (
                            string   theString,
                            ostream& os) override;

  public:

    // visitors
    // ------------------------------------------------------

    void                  acceptIn  (basevisitor* v) override;
    void                  acceptOut (basevisitor* v) override;

    void                  browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    string                asShortNamedOptionString () const override;
    string                asActualLongNamedOptionString () const override;

    void                  print (ostream& os) const override;

    void                  printAtomWithValueOptionsValues (
                            ostream&     os,
                            unsigned int valueFieldWidth) const override;

  private:

    // private fields
    // ------------------------------------------------------

    set<string>&          fStringSetVariable;
};
typedef SMARTP<oahStringSetAtom> S_oahStringSetAtom;
EXP ostream& operator<< (ostream& os, const S_oahStringSetAtom& elt);

//______________________________________________________________________________
class EXP oahStringToIntMapAtom : public oahAtomWithValue
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
                          getStringToIntMapVariable () const
                              { return fStringToIntMapVariable; }
/// JMI                                 fVariableHasBeenSet = true;

  public:

    // public services
    // ------------------------------------------------------

    void                  applyAtomWithValue (
                            string   theString,
                            ostream& os) override;

  public:

    // visitors
    // ------------------------------------------------------

    void                  acceptIn  (basevisitor* v) override;
    void                  acceptOut (basevisitor* v) override;

    void                  browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    string                asShortNamedOptionString () const override;
    string                asActualLongNamedOptionString () const override;

    void                  print (ostream& os) const override;

    void                  printAtomWithValueOptionsValues (
                            ostream&     os,
                            unsigned int valueFieldWidth) const override;

  private:

    // private fields
    // ------------------------------------------------------

    map<string, int>&     fStringToIntMapVariable;
};
typedef SMARTP<oahStringToIntMapAtom> S_oahStringToIntMapAtom;
EXP ostream& operator<< (ostream& os, const S_oahStringToIntMapAtom& elt);

//______________________________________________________________________________
class EXP oahStringAndIntegerAtom : public oahAtomWithValue
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

    void                  setStringVariable (string value);

    string                getStringVariable () const
                              { return fStringVariable; }

    void                  setIntegerVariable (int value);

    int                   getIntegerVariable () const
                              { return fIntegerVariable; }

  public:

    // public services
    // ------------------------------------------------------

    void                  applyAtomWithValue (
                            string   theString,
                            ostream& os) override;

  public:

    // visitors
    // ------------------------------------------------------

    void                  acceptIn  (basevisitor* v) override;
    void                  acceptOut (basevisitor* v) override;

    void                  browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    string                asShortNamedOptionString () const override;
    string                asActualLongNamedOptionString () const override;

    void                  print (ostream& os) const override;

    void                  printAtomWithValueOptionsValues (
                            ostream&     os,
                            unsigned int valueFieldWidth) const override;

  private:

    // private fields
    // ------------------------------------------------------

    string                fStringVariableName;
    string&               fStringVariable;

    string                fIntegerVariableName;
    int&                  fIntegerVariable;
};
typedef SMARTP<oahStringAndIntegerAtom> S_oahStringAndIntegerAtom;
EXP ostream& operator<< (ostream& os, const S_oahStringAndIntegerAtom& elt);

//______________________________________________________________________________
class EXP oahStringAndTwoIntegersAtom : public oahAtomWithValue
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

    void                  setStringVariable (string value);

    string                getStringVariable () const
                              { return fStringVariable; }

    void                  setPrimaryIntegerVariable (int value);

    int                   getPrimaryIntegerVariable () const
                              { return fPrimaryIntegerVariable; }

    void                  setSecondaryIntegerVariable (int value);

    int                   getSecondaryIntegerVariable () const
                              { return fSecondaryIntegerVariable; }

  public:

    // public services
    // ------------------------------------------------------

    void                  applyAtomWithValue (
                            string   theString,
                            ostream& os) override;

  public:

    // visitors
    // ------------------------------------------------------

    void                  acceptIn  (basevisitor* v) override;
    void                  acceptOut (basevisitor* v) override;

    void                  browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    string                asShortNamedOptionString () const override;
    string                asActualLongNamedOptionString () const override;

    void                  print (ostream& os) const override;

    void                  printAtomWithValueOptionsValues (
                            ostream&     os,
                            unsigned int valueFieldWidth) const override;

  private:

    // private fields
    // ------------------------------------------------------

    string&               fStringVariable;

    int&                  fPrimaryIntegerVariable;
    int&                  fSecondaryIntegerVariable;
};
typedef SMARTP<oahStringAndTwoIntegersAtom> S_oahStringAndTwoIntegersAtom;
EXP ostream& operator<< (ostream& os, const S_oahStringAndTwoIntegersAtom& elt);

//______________________________________________________________________________
class EXP oahLengthUnitKindAtom : public oahAtomWithValue
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
                              {
                                fLengthUnitKindVariable = value;
                                fVariableHasBeenSet = true;
                              }

    msrLengthUnitKind     getLengthUnitKindVariable () const
                              { return fLengthUnitKindVariable; }

  public:

    // public services
    // ------------------------------------------------------

    void                  applyAtomWithValue (
                            string   theString,
                            ostream& os) override;

  public:

    // visitors
    // ------------------------------------------------------

    void                  acceptIn  (basevisitor* v) override;
    void                  acceptOut (basevisitor* v) override;

    void                  browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    string                asShortNamedOptionString () const override;
    string                asActualLongNamedOptionString () const override;

    void                  print (ostream& os) const override;

    void                  printAtomWithValueOptionsValues (
                            ostream&     os,
                            unsigned int valueFieldWidth) const override;

  private:

    // private fields
    // ------------------------------------------------------

    msrLengthUnitKind&    fLengthUnitKindVariable;
};
typedef SMARTP<oahLengthUnitKindAtom> S_oahLengthUnitKindAtom;
EXP ostream& operator<< (ostream& os, const S_oahLengthUnitKindAtom& elt);

//______________________________________________________________________________
class EXP oahLengthAtom : public oahAtomWithValue
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
                              {
                                fLengthVariable = value;
                                fVariableHasBeenSet = true;
                              }

    msrLength             getLengthVariable () const
                              { return fLengthVariable; }

  public:

    // public services
    // ------------------------------------------------------

    void                  applyAtomWithValue (
                            string   theString,
                            ostream& os) override;

  public:

    // visitors
    // ------------------------------------------------------

    void                  acceptIn  (basevisitor* v) override;
    void                  acceptOut (basevisitor* v) override;

    void                  browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    string                asShortNamedOptionString () const override;
    string                asActualLongNamedOptionString () const override;

    void                  print (ostream& os) const override;

    void                  printAtomWithValueOptionsValues (
                            ostream&     os,
                            unsigned int valueFieldWidth) const override;

  private:

    // private fields
    // ------------------------------------------------------

    msrLength&            fLengthVariable;
};
typedef SMARTP<oahLengthAtom> S_oahLengthAtom;
EXP ostream& operator<< (ostream& os, const S_oahLengthAtom& elt);

//______________________________________________________________________________
class EXP oahMidiTempoAtom : public oahAtomWithValue
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

    void                  setMidiTempoVariable (
                            msrMidiTempo& value)
                              {
                                fMidiTempoVariable = value;
                                fVariableHasBeenSet = true;
                              }

    const msrMidiTempo&   getMidiTempoVariable () const
                              { return fMidiTempoVariable; }

  public:

    // public services
    // ------------------------------------------------------

    void                  applyAtomWithValue (
                            string   theString,
                            ostream& os) override;

  public:

    // visitors
    // ------------------------------------------------------

    void                  acceptIn  (basevisitor* v) override;
    void                  acceptOut (basevisitor* v) override;

    void                  browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    string                asShortNamedOptionString () const override;
    string                asActualLongNamedOptionString () const override;

    void                  print (ostream& os) const override;

    void                  printAtomWithValueOptionsValues (
                            ostream&     os,
                            unsigned int valueFieldWidth) const override;

  private:

    // private fields
    // ------------------------------------------------------

    msrMidiTempo&         fMidiTempoVariable;
};
typedef SMARTP<oahMidiTempoAtom> S_oahMidiTempoAtom;
EXP ostream& operator<< (ostream& os, const S_oahMidiTempoAtom& elt);

//______________________________________________________________________________
class EXP oahOptionNameHelpAtom : public oahStringWithDefaultValueAtom
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

    // public services
    // ------------------------------------------------------

    void                  applyAtomWithValue (
                            string   theString,
                            ostream& os) override;

    void                  applyAtomWithValueDefaultValue (ostream& os) override;

  public:

    // visitors
    // ------------------------------------------------------

    void                  acceptIn  (basevisitor* v) override;
    void                  acceptOut (basevisitor* v) override;

    void                  browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    string                asShortNamedOptionString () const override;
    string                asActualLongNamedOptionString () const override;

    void                  print (ostream& os) const override;

    virtual void          printAtomWithValueOptionsValues (
                            ostream&     os,
                            unsigned int valueFieldWidth) const override;
  private:

    // private fields
    // ------------------------------------------------------

    string                fValueSpecification;
};
typedef SMARTP<oahOptionNameHelpAtom> S_oahOptionNameHelpAtom;
EXP ostream& operator<< (ostream& os, const S_oahOptionNameHelpAtom& elt);

//______________________________________________________________________________
class EXP oahAProposOptionNameAtom : public oahStringAtom
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<oahAProposOptionNameAtom> create (
      string shortName,
      string longName,
      string description,
      string valueSpecification,
      string variableName,
      string& stringVariable);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    oahAProposOptionNameAtom (
      string shortName,
      string longName,
      string description,
      string valueSpecification,
      string variableName,
      string& stringVariable);

  protected:

    virtual ~oahAProposOptionNameAtom ();

  public:

    // set and get
    // ------------------------------------------------------

  public:

    // public services
    // ------------------------------------------------------

    void                  applyAtomWithValue (
                            string   theString,
                            ostream& os) override;

  public:

    // visitors
    // ------------------------------------------------------

    void                  acceptIn  (basevisitor* v) override;
    void                  acceptOut (basevisitor* v) override;

    void                  browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    string                asShortNamedOptionString () const override;
    string                asActualLongNamedOptionString () const override;

    void                  print (ostream& os) const override;

    virtual void          printAtomWithValueOptionsValues (
                            ostream&     os,
                            unsigned int valueFieldWidth) const override;
  private:

    // private fields
    // ------------------------------------------------------

    string                fValueSpecification;
};
typedef SMARTP<oahAProposOptionNameAtom> S_oahAProposOptionNameAtom;
EXP ostream& operator<< (ostream& os, const S_oahAProposOptionNameAtom& elt);

//______________________________________________________________________________
class EXP oahFindStringAtom : public oahStringAtom
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<oahFindStringAtom> create (
      string shortName,
      string longName,
      string description,
      string valueSpecification,
      string variableName,
      string& stringVariable);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    oahFindStringAtom (
      string shortName,
      string longName,
      string description,
      string valueSpecification,
      string variableName,
      string& stringVariable);

  protected:

    virtual ~oahFindStringAtom ();

  public:

    // set and get
    // ------------------------------------------------------

  public:

    // public services
    // ------------------------------------------------------

    void                  applyAtomWithValue (
                            string   theString,
                            ostream& os) override;

  public:

    // visitors
    // ------------------------------------------------------

    void                  acceptIn  (basevisitor* v) override;
    void                  acceptOut (basevisitor* v) override;

    void                  browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    string                asShortNamedOptionString () const override;
    string                asActualLongNamedOptionString () const override;

    void                  print (ostream& os) const override;

    virtual void          printAtomWithValueOptionsValues (
                            ostream&     os,
                            unsigned int valueFieldWidth) const override;
  private:

    // private fields
    // ------------------------------------------------------

    string                fValueSpecification;
};
typedef SMARTP<oahFindStringAtom> S_oahFindStringAtom;
EXP ostream& operator<< (ostream& os, const S_oahFindStringAtom& elt);


}


#endif
