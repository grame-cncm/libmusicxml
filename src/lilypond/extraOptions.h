/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef ___extraOptions___
#define ___extraOptions___


#include "setExtraOptionsIfDesired.h"

#ifdef EXTRA_OPTIONS


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

    // services
    // ------------------------------------------------------

    // print
    // ------------------------------------------------------

    void                  print (ostream& os) const;

    void                  printAllChordsStructures (ostream& os) const;

    void                  printOptionsValues (
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

    // services
    // ------------------------------------------------------

    void                  handleValue (
                            string           theString,
                            indentedOstream& os);

    // print
    // ------------------------------------------------------

    void                  print (ostream& os) const;

    void                  printAllChordsContents (
                            ostream&              os,
                            msrSemiTonesPitchKind semiTonesPitchKind) const;

    void                  printOptionsValues (
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

    // services
    // ------------------------------------------------------

    void                  handleValue (
                            string           theString,
                            indentedOstream& os);

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

    // services
    // ------------------------------------------------------

    void                  handleValue (
                            string           theString,
                            indentedOstream& os);

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
typedef SMARTP<extraShowChordAnalysisAtom> S_extraShowChordAnalysisAtom;
EXP ostream& operator<< (ostream& os, const S_extraShowChordAnalysisAtom& elt);

//______________________________________________________________________________
class extraOptions : public oahGroup
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<extraOptions> create (
      S_oahHandler handler);

    SMARTP<extraOptions>        createCloneWithDetailedTrace ();

  private:

    // initialisation
    // ------------------------------------------------------

    void                  initializeExtraOptions (
                            bool boolOptionsInitialValue);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    extraOptions (
      S_oahHandler handler);

    virtual ~extraOptions ();

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

    virtual S_oahValuedAtom
                          handleAtom (
                            ostream&  os,
                            S_oahAtom atom);

    virtual void          handleValuedAtomValue (
                            indentedOstream&  os,
                            S_oahAtom         atom,
                            string            theString);

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

    // print
    // ------------------------------------------------------

    void                  printExtraOptionsHelp ();

    void                  printExtraOptionsValues (int fieldWidth);

  public:

    // fields
    // ------------------------------------------------------

    string                fChordsRootAsString; // JMI
};
typedef SMARTP<extraOptions> S_extraOptions;
EXP ostream& operator<< (ostream& os, const S_extraOptions& elt);

EXP extern S_extraOptions gExtraOptions;
EXP extern S_extraOptions gExtraOptionsUserChoices;
EXP extern S_extraOptions gExtraOptionsWithDetailedTrace;

//______________________________________________________________________________
void initializeExtraOptionsHandling (
  S_oahHandler handler);


}


#endif


#endif
