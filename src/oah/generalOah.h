/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef ___generalOahGroup___
#define ___generalOahGroup___

#include <set>

#include "exports.h"

#include "oahOah.h"


namespace MusicXML2
{

//______________________________________________________________________________
class generalOahGroup : public oahGroup
{
  public:

    static SMARTP<generalOahGroup> create (
      S_oahHandler handlerUpLink);

    SMARTP<generalOahGroup>    createCloneWithTrueValues (); // JMI

  public:

    // initialisation
    // ------------------------------------------------------

    void                  initializeGeneralOah (
                            bool boolOptionsInitialValue);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    generalOahGroup (
      S_oahHandler handlerUpLink);

    virtual ~generalOahGroup ();

  public:

    // set and get
    // ------------------------------------------------------

/* JMI ???
    void                  setAllGeneralTraceOah (
                            bool boolOptionsInitialValue);
                            */

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

    void                  initializeGeneralWarningAndErrorsOptions (
                            bool boolOptionsInitialValue);

    void                  initializeGeneralCPUUsageOptions (
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

    void                  printGeneralOahHelp ();

    void                  printGeneralOahValues (int fieldWidth);

  public:

    // translation date
    // --------------------------------------

    string                fTranslationDateFull;
    string                fTranslationDateYYYYMMDD;

    // warning and error handling
    // --------------------------------------

    bool                  fQuiet;
    bool                  fDontShowErrors;
    bool                  fDontExitOnErrors;
    bool                  fDisplaySourceCodePosition;

    // CPU usage
    // --------------------------------------

    bool                  fDisplayCPUusage;
};
typedef SMARTP<generalOahGroup> S_generalOahGroup;
EXP ostream& operator<< (ostream& os, const S_generalOahGroup& elt);

EXP extern S_generalOahGroup gGlobalGeneralOahGroup;
EXP extern S_generalOahGroup gGlobalGeneralOahGroupUserChoices;
EXP extern S_generalOahGroup gGlobalGeneralOahGroupWithDetailedTrace;

//______________________________________________________________________________
void initializeGeneralOahHandling (
  S_oahHandler handler);


}


#endif
