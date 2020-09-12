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


#include "setExtraOahIfDesired.h"

#ifdef EXTRA_OAH


#include "oahBasicTypes.h"

#include "exports.h"


namespace MusicXML2
{

//______________________________________________________________________________
class extraShowAllHarmoniesStructuresAtom : public oahAtom
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

    // services
    // ------------------------------------------------------

    S_oahValuedAtom       handleOptionUnderName (
                            string   optionName,
                            ostream& os) override;

  public:

    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v) override;
    virtual void          acceptOut (basevisitor* v) override;

    virtual void          browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    void                  print (ostream& os) const override;

    void                  printAllHarmoniesStructures (ostream& os) const;

    void                  printAtomOptionsValues (
                            ostream& os,
                            int      valueFieldWidth) const override;

  private:

    // fields
    // ------------------------------------------------------
};
typedef SMARTP<extraShowAllHarmoniesStructuresAtom> S_extraShowAllHarmoniesStructuresAtom;
EXP ostream& operator<< (ostream& os, const S_extraShowAllHarmoniesStructuresAtom& elt);

//______________________________________________________________________________
class extraShowAllHarmoniesContentsAtom : public oahValuedAtom
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

    // services
    // ------------------------------------------------------

    S_oahValuedAtom       handleOptionUnderName (
                            string   optionName,
                            ostream& os) override;

    void                  handleValue (
                            string   theString,
                            ostream& os) override;

  public:

    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v) override;
    virtual void          acceptOut (basevisitor* v) override;

    virtual void          browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    void                  print (ostream& os) const override;

    void                  printAllHarmoniesContents (
                            ostream&              os,
                            msrSemiTonesPitchKind semiTonesPitchKind) const;

    void                  printAtomOptionsValues (
                            ostream& os,
                            int      valueFieldWidth) const override;

  private:

    // fields
    // ------------------------------------------------------

    string&               fStringVariable;
};
typedef SMARTP<extraShowAllHarmoniesContentsAtom> S_extraShowAllHarmoniesContentsAtom;
EXP ostream& operator<< (ostream& os, const S_extraShowAllHarmoniesContentsAtom& elt);

//______________________________________________________________________________
class extraShowHarmonyDetailsAtom : public oahValuedAtom
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

    // services
    // ------------------------------------------------------

    S_oahValuedAtom       handleOptionUnderName (
                            string   optionName,
                            ostream& os) override;

    void                  handleValue (
                            string   theString,
                            ostream& os) override;

  public:

    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v) override;
    virtual void          acceptOut (basevisitor* v) override;

    virtual void          browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    void                  print (ostream& os) const override;

    void                  printAtomOptionsValues (
                            ostream& os,
                            int      valueFieldWidth) const override;

  private:

    // fields
    // ------------------------------------------------------

    string&               fStringVariable;
};
typedef SMARTP<extraShowHarmonyDetailsAtom> S_extraShowHarmonyDetailsAtom;
EXP ostream& operator<< (ostream& os, const S_extraShowHarmonyDetailsAtom& elt);

//______________________________________________________________________________
class extraShowHarmonyAnalysisAtom : public oahValuedAtom
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

    // services
    // ------------------------------------------------------

    S_oahValuedAtom       handleOptionUnderName (
                            string   optionName,
                            ostream& os) override;

    void                  handleValue (
                            string   theString,
                            ostream& os) override;

  public:

    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v) override;
    virtual void          acceptOut (basevisitor* v) override;

    virtual void          browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    void                  print (ostream& os) const override;

    void                  printAtomOptionsValues (
                            ostream& os,
                            int      valueFieldWidth) const override;

  private:

    // fields
    // ------------------------------------------------------

    string&               fStringVariable;
};
typedef SMARTP<extraShowHarmonyAnalysisAtom> S_extraShowHarmonyAnalysisAtom;
EXP ostream& operator<< (ostream& os, const S_extraShowHarmonyAnalysisAtom& elt);

//______________________________________________________________________________
class extraOah : public oahGroup
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<extraOah> create (
      S_oahHandler handlerUpLink);

    SMARTP<extraOah>      createCloneWithDetailedTrace ();

  private:

    // initialisation
    // ------------------------------------------------------

    void                  initializeExtraOah (
                            bool boolOptionsInitialValue);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    extraOah (
      S_oahHandler handlerUpLink);

    virtual ~extraOah ();

  public:

    // quiet mode
    // ------------------------------------------------------

    void                  enforceQuietness ();

  public:

    // consistency check
    // ------------------------------------------------------

    virtual void          checkOptionsConsistency () override;

  public:

    // public services
    // ------------------------------------------------------

  private:

    // private services
    // ------------------------------------------------------

    void                  initializeExtraShowAllHarmoniesStructuresOptions (
                            bool boolOptionsInitialValue);

    void                  initializeExtraShowAllHarmoniesContentsOptions (
                            bool boolOptionsInitialValue);

    void                  initializeExtraShowHarmonyDetailsOptions (
                            bool boolOptionsInitialValue);

    void                  initializeExtraShowHarmonyAnalysisOptions (
                            bool boolOptionsInitialValue);

  public:

    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v) override;
    virtual void          acceptOut (basevisitor* v) override;

    virtual void          browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    void                  printExtraOahHelp ();

    void                  printExtraOahValues (int fieldWidth);

  public:

    // fields
    // ------------------------------------------------------

    string                fHarmoniesRootAsString; // JMI
};
typedef SMARTP<extraOah> S_extraOah;
EXP ostream& operator<< (ostream& os, const S_extraOah& elt);

EXP extern S_extraOah ggGlobalExtraOah;
EXP extern S_extraOah ggGlobalExtraOahUserChoices;
EXP extern S_extraOah ggGlobalExtraOahWithDetailedTrace;

//______________________________________________________________________________
void initializeExtraOahHandler (
  S_oahHandler handler);


}


#endif


#endif
