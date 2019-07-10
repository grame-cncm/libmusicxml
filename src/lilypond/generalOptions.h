/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef ___generalOptions___
#define ___generalOptions___

#include <set>

#include "exports.h"

#include "oahBasicOptions.h"


namespace MusicXML2
{

//______________________________________________________________________________
class generalOptions : public oahGroup
{
  public:

    static SMARTP<generalOptions> create (
      S_oahHandler handler);

    SMARTP<generalOptions>        createCloneWithTrueValues (); // JMI

  public:

    // initialisation
    // ------------------------------------------------------

    void                  initializeGeneralOptions (
                            bool boolOptionsInitialValue);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    generalOptions (
      S_oahHandler handler);

    virtual ~generalOptions ();

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

/* JMI ???
    void                  setAllGeneralTraceOptions (
                            bool boolOptionsInitialValue);
                            */

  public:

    // public services
    // ------------------------------------------------------

    virtual S_oahValuedAtom
                          handleAtom (
                            ostream&  os,
                            S_oahAtom atom);

  private:

    // private services
    // ------------------------------------------------------

    void                  initializeGeneralWarningAndErrorsOptions (
                            bool boolOptionsInitialValue);

    void                  initializeGeneralCPUUsageOptions (
                            bool boolOptionsInitialValue);

  public:

    // print
    // ------------------------------------------------------

    void                  printGeneralOptionsHelp ();

    void                  printGeneralOptionsValues (int fieldWidth);

  public:

    // translation date
    // --------------------------------------

    string                fTranslationDate;

    // warning and error handling
    // --------------------------------------

    bool                  fQuiet;
    bool                  fDontShowErrors;
    bool                  fDontAbortOnErrors;
    bool                  fDisplaySourceCodePosition;

    // CPU usage
    // --------------------------------------

    bool                  fDisplayCPUusage;
};
typedef SMARTP<generalOptions> S_generalOptions;
EXP ostream& operator<< (ostream& os, const S_generalOptions& elt);

EXP extern S_generalOptions gGeneralOptions;
EXP extern S_generalOptions gGeneralOptionsUserChoices;
EXP extern S_generalOptions gGeneralOptionsWithDetailedTrace;

//______________________________________________________________________________
void initializeGeneralOptionsHandling (
  S_oahHandler handler);


}


#endif
