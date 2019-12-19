/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef ___generalOah___
#define ___generalOah___

#include <set>

#include "exports.h"

#include "oahOah.h"


namespace MusicXML2
{

//______________________________________________________________________________
class generalOah : public oahGroup
{
  public:

    static SMARTP<generalOah> create (
      S_oahHandler handlerUpLink);

    SMARTP<generalOah>    createCloneWithTrueValues (); // JMI

  public:

    // initialisation
    // ------------------------------------------------------

    void                  initializeGeneralOah (
                            bool boolOptionsInitialValue);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    generalOah (
      S_oahHandler handlerUpLink);

    virtual ~generalOah ();

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
    void                  setAllGeneralTraceOah (
                            bool boolOptionsInitialValue);
                            */

  public:

    // public services
    // ------------------------------------------------------

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

    virtual void          acceptIn  (basevisitor* v);
    virtual void          acceptOut (basevisitor* v);

    virtual void          browseData (basevisitor* v);

  public:

    // print
    // ------------------------------------------------------

    void                  printGeneralOahHelp ();

    void                  printGeneralOahValues (int fieldWidth);

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
typedef SMARTP<generalOah> S_generalOah;
EXP ostream& operator<< (ostream& os, const S_generalOah& elt);

EXP extern S_generalOah gGeneralOah;
EXP extern S_generalOah gGeneralOahUserChoices;
EXP extern S_generalOah gGeneralOahWithDetailedTrace;

//______________________________________________________________________________
void initializeGeneralOahHandling (
  S_oahHandler handler);


}


#endif
