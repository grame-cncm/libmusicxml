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
class extraShowAllChordsStructuresAtom : public oahAtom
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<extraShowAllChordsStructuresAtom> create (
      string shortName,
      string longName,
      string description);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    extraShowAllChordsStructuresAtom (
      string shortName,
      string longName,
      string description);

    virtual ~extraShowAllChordsStructuresAtom ();

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

    void                  printAllChordsStructures (ostream& os) const;

    void                  printAtomOptionsValues (
                            ostream& os,
                            int      valueFieldWidth) const;

  private:

    // fields
    // ------------------------------------------------------
};
typedef SMARTP<extraShowAllChordsStructuresAtom> S_extraShowAllChordsStructuresAtom;
EXP ostream& operator<< (ostream& os, const S_extraShowAllChordsStructuresAtom& elt);

//______________________________________________________________________________
class extraShowAllChordsContentsAtom : public oahValuedAtom
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<extraShowAllChordsContentsAtom> create (
      string  shortName,
      string  longName,
      string  description,
      string  valueSpecification,
      string  variableName,
      string& stringVariable);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    extraShowAllChordsContentsAtom (
      string  shortName,
      string  longName,
      string  description,
      string  valueSpecification,
      string  variableName,
      string& stringVariable);

    virtual ~extraShowAllChordsContentsAtom ();

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

    void                  print (ostream& os) const;

    void                  printAllChordsContents (
                            ostream&              os,
                            msrSemiTonesPitchKind semiTonesPitchKind) const;

    void                  printAtomOptionsValues (
                            ostream& os,
                            int      valueFieldWidth) const;

  private:

    // fields
    // ------------------------------------------------------

    string&               fStringVariable;
};
typedef SMARTP<extraShowAllChordsContentsAtom> S_extraShowAllChordsContentsAtom;
EXP ostream& operator<< (ostream& os, const S_extraShowAllChordsContentsAtom& elt);

//______________________________________________________________________________
class extraShowChordDetailsAtom : public oahValuedAtom
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<extraShowChordDetailsAtom> create (
      string  shortName,
      string  longName,
      string  description,
      string  valueSpecification,
      string  variableName,
      string& stringVariable);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    extraShowChordDetailsAtom (
      string  shortName,
      string  longName,
      string  description,
      string  valueSpecification,
      string  variableName,
      string& stringVariable);

    virtual ~extraShowChordDetailsAtom ();

  public:

    // set and get
    // ------------------------------------------------------

    void                  setShowChordDetailsVariableValue (
                            string value)
                              {  fStringVariable = value; }

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

    void                  print (ostream& os) const;

    void                  printAtomOptionsValues (
                            ostream& os,
                            int      valueFieldWidth) const;

  private:

    // fields
    // ------------------------------------------------------

    string&               fStringVariable;
};
typedef SMARTP<extraShowChordDetailsAtom> S_extraShowChordDetailsAtom;
EXP ostream& operator<< (ostream& os, const S_extraShowChordDetailsAtom& elt);

//______________________________________________________________________________
class extraShowChordAnalysisAtom : public oahValuedAtom
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<extraShowChordAnalysisAtom> create (
      string  shortName,
      string  longName,
      string  description,
      string  valueSpecification,
      string  variableName,
      string& stringVariable);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    extraShowChordAnalysisAtom (
      string  shortName,
      string  longName,
      string  description,
      string  valueSpecification,
      string  variableName,
      string& stringVariable);

    virtual ~extraShowChordAnalysisAtom ();

  public:

    // set and get
    // ------------------------------------------------------

    void                  setShowChordAnalysisVariableValue (
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

    void                  print (ostream& os) const;

    void                  printAtomOptionsValues (
                            ostream& os,
                            int      valueFieldWidth) const;

  private:

    // fields
    // ------------------------------------------------------

    string&               fStringVariable;
};
typedef SMARTP<extraShowChordAnalysisAtom> S_extraShowChordAnalysisAtom;
EXP ostream& operator<< (ostream& os, const S_extraShowChordAnalysisAtom& elt);

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

    virtual void          checkOptionsConsistency ();

  public:

    // public services
    // ------------------------------------------------------

  private:

    // private services
    // ------------------------------------------------------

    void                  initializeExtraShowAllChordsStructuresOptions (
                            bool boolOptionsInitialValue);

    void                  initializeExtraShowAllChordsContentsOptions (
                            bool boolOptionsInitialValue);

    void                  initializeExtraShowChordDetailsOptions (
                            bool boolOptionsInitialValue);

    void                  initializeExtraShowChordAnalysisOptions (
                            bool boolOptionsInitialValue);

  public:

    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v);
    virtual void          acceptOut (basevisitor* v);

    virtual void          browseData (basevisitor* v);

  public:

    // print
    // ------------------------------------------------------

    void                  printExtraOahHelp ();

    void                  printExtraOahValues (int fieldWidth);

  public:

    // fields
    // ------------------------------------------------------

    string                fChordsRootAsString; // JMI
};
typedef SMARTP<extraOah> S_extraOah;
EXP ostream& operator<< (ostream& os, const S_extraOah& elt);

EXP extern S_extraOah gExtraOah;
EXP extern S_extraOah gExtraOahUserChoices;
EXP extern S_extraOah gExtraOahWithDetailedTrace;

//______________________________________________________________________________
void initializeExtraOahHandling (
  S_oahHandler handler);


}


#endif


#endif
