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

#include "oahAtomsCollection.h"


namespace MusicXML2
{
//______________________________________________________________________________
#define K_INSIDER_OPTION_NAME "insider"

class insiderOahAtom : public oahAtom
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<insiderOahAtom> create (
      string shortName,
      string longName,
      string description);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    insiderOahAtom (
      string shortName,
      string longName,
      string description);

    virtual ~insiderOahAtom ();

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

    virtual void          acceptIn  (basevisitor* v) override;
    virtual void          acceptOut (basevisitor* v) override;

    virtual void          browseData (basevisitor* v) override;

    // print
    // ------------------------------------------------------

    void                  print (ostream& os) const override;

  private:

    // private fields
    // ------------------------------------------------------
};
typedef SMARTP<insiderOahAtom> S_insiderOahAtom;
EXP ostream& operator<< (ostream& os, const S_insiderOahAtom& elt);

//______________________________________________________________________________
#define K_REGULAR_OPTION_NAME "regular"

class regularOahAtom : public oahAtom
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<regularOahAtom> create (
      string shortName,
      string longName,
      string description);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    regularOahAtom (
      string shortName,
      string longName,
      string description);

    virtual ~regularOahAtom ();

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

    virtual void          acceptIn  (basevisitor* v) override;
    virtual void          acceptOut (basevisitor* v) override;

    virtual void          browseData (basevisitor* v) override;

    // print
    // ------------------------------------------------------

    void                  print (ostream& os) const override;

  private:

    // private fields
    // ------------------------------------------------------
};
typedef SMARTP<regularOahAtom> S_regularOahAtom;
EXP ostream& operator<< (ostream& os, const S_regularOahAtom& elt);

//______________________________________________________________________________
class oahOahGroup : public oahGroup
{
  public:

    static SMARTP<oahOahGroup> create (
      string executableName);

  public:

    // initialisation
    // ------------------------------------------------------

    void                  initializeOahOahGroup (
                            string executableName);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    oahOahGroup (
      string executableName);

    virtual ~oahOahGroup ();

  public:

    // set and get
    // ------------------------------------------------------

    // command line
    string                getHandlerExecutableName () const
                              { return fHandlerExecutableName; }

    void                  setInputSourceName (string name)
                              { fInputSourceName = name; }

    string                getInputSourceName () const
                              { return fInputSourceName; }

    // options and arguments
    bool                  getShowOptionsAndArguments () const
                              { return fShowOptionsAndArguments; }

    string                getCommandLineAsSupplied () const
                              { return fCommandLineAsSupplied; }
    string                getCommandLineWithShortOptionsNames () const
                              { return fCommandLineWithShortOptionsNames; }
    string                getCommandLineWithLongOptionsNames () const
                              { return fCommandLineWithLongOptionsNames; }

    // help about option name
    string                getOptionNameToProvideHelpAbout () const
                              { return fOptionNameToProvideHelpAbout; }
    S_oahOptionNameHelpAtom
                          getOptionNameHelpAtom () const
                              { return fOptionNameHelpAtom; }

    // apropos about option name
    string                getOptionNameToProvideAProposAbout () const
                              { return fOptionNameToProvideAProposAbout; }
    S_oahAProposOptionNameAtom
                          getAProposOptionNameAtom () const
                              { return fAProposOptionNameAtom; }

    // find string
    string                getStringToBeFound () const
                              { return fStringToBeFound; }
    S_oahFindStringAtom   getFindStringAtom () const
                              { return fFindStringAtom; }

    // trace
    bool                  getTraceOahVisitors () const
                              { return fTraceOahVisitors; }

    // display
    bool                  getDisplayOahValues () const
                              { return fDisplayOahValues; }
    bool                  getDisplayOahHandler () const
                              { return fDisplayOahHandler; }

  public:

    // public services
    // ------------------------------------------------------

    // quiet mode
    void                  enforceGroupQuietness () override;

    // consistency check
    virtual void          checkGroupOptionsConsistency () override;

  private:

    // private services
    // ------------------------------------------------------

    void                  initializeOahBasicHelpOptions (
                            string executableName);

    void                  initializeOahOahAndArgumentsOptions ();

#ifdef TRACING_IS_ENABLED
    void                  initializeOahTraceOah ();
#endif

  public:

    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v) override;
    virtual void          acceptOut (basevisitor* v) override;

    virtual void          browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    void                  printOahOahHelp ();

    void                  printOahOahValues (int valueFieldWidth);

    void                  printAtomWithValueOptionsValues (
                            ostream& os,
                            int      valueFieldWidth) const;

  private:

    // private work fields
    // --------------------------------------

    // command line
    string                fHandlerExecutableName;

    string                fInputSourceName;

    // help about option name
    string                fOptionNameToProvideHelpAbout;
    S_oahOptionNameHelpAtom
                          fOptionNameHelpAtom;

    // apropos about option name
    string                fOptionNameToProvideAProposAbout;
    S_oahAProposOptionNameAtom
                          fAProposOptionNameAtom;

    // find string
    string                fStringToBeFound;
    S_oahFindStringAtom   fFindStringAtom;

  private:

    // private fields
    // --------------------------------------

    // options and arguments
    bool                  fShowOptionsAndArguments;

    string                fCommandLineAsSupplied;
    string                fCommandLineWithShortOptionsNames;
    string                fCommandLineWithLongOptionsNames;

    // trace
    bool                  fTraceOahVisitors;

    // display
    bool                  fDisplayOahValues;
    bool                  fDisplayOahHandler;

};
typedef SMARTP<oahOahGroup> S_oahOahGroup;
EXP ostream& operator<< (ostream& os, const S_oahOahGroup& elt);

EXP extern S_oahOahGroup gGlobalOahOahGroup;

//______________________________________________________________________________
S_oahOahGroup createGlobalOahOahGroup (
  string executableName);


}


#endif

/* JMI
//______________________________________________________________________________
class oahOptionalValuesStyleKindAtom : public oahAtomWithValue
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

    // public services
    // ------------------------------------------------------

    void                  applyAtomWithValue (
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

    string                asShortNamedOptionString () const override;
    string                asActualLongNamedOptionString () const override;

    void                  print (ostream& os) const override;

  private:

    // private fields
    // ------------------------------------------------------

    oahOptionalValuesStyleKind&
                          fOahOptionalValuesStyleKindVariable;
};
typedef SMARTP<oahOptionalValuesStyleKindAtom> S_oahOptionalValuesStyleKindAtom;
EXP ostream& operator<< (ostream& os, const S_oahOptionalValuesStyleKindAtom& elt);
*/

