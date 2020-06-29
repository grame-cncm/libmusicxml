/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef ___oahOah___
#define ___oahOah___

#include <set>

#include "exports.h"

#include "oahBasicTypes.h"


namespace MusicXML2
{

//______________________________________________________________________________
class oahOptionalValuesStyleKindAtom : public oahValuedAtom
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<oahOptionalValuesStyleKindAtom> create (
      string             shortName,
      string             longName,
      string             description,
      string             valueSpecification,
      string             variableName,
      oahOptionalValuesStyleKind&
                         oahOptionalValuesStyleKindVariable);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    oahOptionalValuesStyleKindAtom (
      string             shortName,
      string             longName,
      string             description,
      string             valueSpecification,
      string             variableName,
      oahOptionalValuesStyleKind&
                         oahOptionalValuesStyleKindVariable);

    virtual ~oahOptionalValuesStyleKindAtom ();

  public:

    // set and get
    // ------------------------------------------------------

    void                  setOahOptionalValuesStyleKindVariable (
                            oahOptionalValuesStyleKind value)
                              {
                                fOahOptionalValuesStyleKindVariable = value;
                              }

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

  private:

    // fields
    // ------------------------------------------------------

    oahOptionalValuesStyleKind&
                          fOahOptionalValuesStyleKindVariable;
};
typedef SMARTP<oahOptionalValuesStyleKindAtom> S_oahOptionalValuesStyleKindAtom;
EXP ostream& operator<< (ostream& os, const S_oahOptionalValuesStyleKindAtom& elt);

//______________________________________________________________________________
class oahOah : public oahGroup
{
  public:

    static SMARTP<oahOah> create (
      string       executableName,
      S_oahHandler handlerUpLink);

    SMARTP<oahOah> createCloneWithTrueValues (); // JMI

  public:

    // initialisation
    // ------------------------------------------------------

    void                  initializeOahOah (
                            bool boolOptionsInitialValue);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    oahOah (
      string       executableName,
      S_oahHandler handlerUpLink);

    virtual ~oahOah ();

  public:

    // quiet mode
    // ------------------------------------------------------

    void                  enforceQuietness ();

  public:

    // consistency check
    // ------------------------------------------------------

    virtual void          checkOptionsConsistency ();

  public:

    // set and get
    // ------------------------------------------------------

  public:

    // public services
    // ------------------------------------------------------

  private:

    // private services
    // ------------------------------------------------------

    void                  initializeOahBasicHelpOptions (
                            bool boolOptionsInitialValue);

    void                  initializeOahOahAndArgumentsOptions (
                            bool boolOptionsInitialValue);

#ifdef TRACE_OAH
    void                  initializeOahTraceOah (
                            bool boolOptionsInitialValue);
#endif

  public:

    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v);
    virtual void          acceptOut (basevisitor* v);

    virtual void          browseData (basevisitor* v);

  public:

    // print
    // ------------------------------------------------------

    void                  printOahOahHelp ();

    void                  printOahOahValues (int valueFieldWidth);

    void                  printAtomOptionsValues (
                            ostream& os,
                            int      valueFieldWidth) const;

  public:

    // command line
    // --------------------------------------

    string                fHandlerExecutableName;

    string                fInputSourceName;

    // options and arguments
    // --------------------------------------

    bool                  fShowOptionsAndArguments;

    string                fCommandLineAsSupplied;
    string                fCommandLineWithShortOptionsNames;
    string                fCommandLineWithLongOptionsNames;

    // option name
    // --------------------------------------

    string                fOptionName;

    // trace
    // --------------------------------------

    bool                  fTraceOahVisitors;

    // display
    // --------------------------------------

    bool                  fDisplayOahValues;
    bool                  fDisplayOahHandler;
};
typedef SMARTP<oahOah> S_oahOah;
EXP ostream& operator<< (ostream& os, const S_oahOah& elt);

EXP extern S_oahOah gOahOah;
EXP extern S_oahOah gOahOahUserChoices;
EXP extern S_oahOah gOahOahWithDetailedTrace;

//______________________________________________________________________________
void initializeOahOahHandling (
  string       executableName,
  S_oahHandler handler);


}


#endif
