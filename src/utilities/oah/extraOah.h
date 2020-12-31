/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef ___extraOah___
#define ___extraOah___


#include "enableExtraOahIfDesired.h"

#ifdef EXTRA_OAH_IS_ENABLED


#include "msrBasicTypes.h"

#include "oahBasicTypes.h"


namespace MusicXML2
{

//______________________________________________________________________________
class EXP extraShowAllHarmoniesStructuresAtom : public oahAtomWithValue
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<extraShowAllHarmoniesStructuresAtom> create (
      string shortName,
      string longName,
      string description);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    extraShowAllHarmoniesStructuresAtom (
      string shortName,
      string longName,
      string description);

    virtual ~extraShowAllHarmoniesStructuresAtom ();

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

    void                  print (ostream& os) const override;

    void                  printAllHarmoniesStructures (ostream& os) const;

    void                  printAtomWithValueOptionsValues (
                            ostream&     os,
                            unsigned int valueFieldWidth) const override;

  private:

    // private fields
    // ------------------------------------------------------

    //  JMI ???
};
typedef SMARTP<extraShowAllHarmoniesStructuresAtom> S_extraShowAllHarmoniesStructuresAtom;
EXP ostream& operator<< (ostream& os, const S_extraShowAllHarmoniesStructuresAtom& elt);

//______________________________________________________________________________
class EXP extraShowAllHarmoniesContentsAtom : public oahAtomWithValue
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<extraShowAllHarmoniesContentsAtom> create (
      string  shortName,
      string  longName,
      string  description,
      string  valueSpecification,
      string  variableName,
      string& stringVariable);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    extraShowAllHarmoniesContentsAtom (
      string  shortName,
      string  longName,
      string  description,
      string  valueSpecification,
      string  variableName,
      string& stringVariable);

    virtual ~extraShowAllHarmoniesContentsAtom ();

  public:

    // set and get
    // ------------------------------------------------------

    void                  setStringVariable (
                            string value)
                              { fStringVariable = value; }

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

    void                  print (ostream& os) const override;

    void                  printAllHarmoniesContents (
                            ostream&              os,
                            msrSemiTonesPitchKind semiTonesPitchKind) const;

    void                  printAtomWithValueOptionsValues (
                            ostream&     os,
                            unsigned int valueFieldWidth) const override;

  private:

    // private fields
    // ------------------------------------------------------

    string&               fStringVariable;
};
typedef SMARTP<extraShowAllHarmoniesContentsAtom> S_extraShowAllHarmoniesContentsAtom;
EXP ostream& operator<< (ostream& os, const S_extraShowAllHarmoniesContentsAtom& elt);

//______________________________________________________________________________
class EXP extraShowHarmonyDetailsAtom : public oahAtomWithValue
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<extraShowHarmonyDetailsAtom> create (
      string  shortName,
      string  longName,
      string  description,
      string  valueSpecification,
      string  variableName,
      string& stringVariable);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    extraShowHarmonyDetailsAtom (
      string  shortName,
      string  longName,
      string  description,
      string  valueSpecification,
      string  variableName,
      string& stringVariable);

    virtual ~extraShowHarmonyDetailsAtom ();

  public:

    // set and get
    // ------------------------------------------------------

    void                  setShowHarmonyDetailsVariableValue (
                            string value)
                              {  fStringVariable = value; }

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

    void                  print (ostream& os) const override;

    void                  printAtomWithValueOptionsValues (
                            ostream&     os,
                            unsigned int valueFieldWidth) const override;

  private:

    // private fields
    // ------------------------------------------------------

    string&               fStringVariable;
};
typedef SMARTP<extraShowHarmonyDetailsAtom> S_extraShowHarmonyDetailsAtom;
EXP ostream& operator<< (ostream& os, const S_extraShowHarmonyDetailsAtom& elt);

//______________________________________________________________________________
class EXP extraShowHarmonyAnalysisAtom : public oahAtomWithValue
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<extraShowHarmonyAnalysisAtom> create (
      string  shortName,
      string  longName,
      string  description,
      string  valueSpecification,
      string  variableName,
      string& stringVariable);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    extraShowHarmonyAnalysisAtom (
      string  shortName,
      string  longName,
      string  description,
      string  valueSpecification,
      string  variableName,
      string& stringVariable);

    virtual ~extraShowHarmonyAnalysisAtom ();

  public:

    // set and get
    // ------------------------------------------------------

    void                  setShowHarmonyAnalysisVariableValue (
                            string value)
                              { fStringVariable = value; }

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

    void                  print (ostream& os) const override;

    void                  printAtomWithValueOptionsValues (
                            ostream&     os,
                            unsigned int valueFieldWidth) const override;

  private:

    // private fields
    // ------------------------------------------------------

    string&               fStringVariable;
};
typedef SMARTP<extraShowHarmonyAnalysisAtom> S_extraShowHarmonyAnalysisAtom;
EXP ostream& operator<< (ostream& os, const S_extraShowHarmonyAnalysisAtom& elt);

//______________________________________________________________________________
class EXP extraOahGroup : public oahGroup
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<extraOahGroup> create ();

  private:

    // initialisation
    // ------------------------------------------------------

    void                  initializeExtraOahGroup ();

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    extraOahGroup ();

    virtual ~extraOahGroup ();

  public:

    // public services
    // ------------------------------------------------------

    // quiet mode
    void                  enforceGroupQuietness () override;

    // consistency check
    void                  checkGroupOptionsConsistency () override;

  private:

    // private services
    // ------------------------------------------------------

    void                  initializeExtraShowAllHarmoniesStructuresOptions ();

    void                  initializeExtraShowAllHarmoniesContentsOptions ();

    void                  initializeExtraShowHarmonyDetailsOptions ();

    void                  initializeExtraShowHarmonyAnalysisOptions ();

  public:

    // visitors
    // ------------------------------------------------------

    void                  acceptIn  (basevisitor* v) override;
    void                  acceptOut (basevisitor* v) override;

    void                  browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    void                  printExtraOahHelp ();

    void                  printExtraOahValues (unsigned int fieldWidth);

  public:

    // fields
    // ------------------------------------------------------

    string                fHarmoniesRootAsString; // JMI
};
typedef SMARTP<extraOahGroup> S_extraOahGroup;
EXP ostream& operator<< (ostream& os, const S_extraOahGroup& elt);

EXP extern S_extraOahGroup gGlobalExtraOahGroup;

//______________________________________________________________________________
EXP S_extraOahGroup createGlobalExtraOahGroup ();


}


#endif


#endif
