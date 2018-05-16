/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef __generalOptions__
#define __generalOptions__

#include <set>

#include "optionsBasicTypes.h"
#include "exports.h"


namespace MusicXML2 
{

//______________________________________________________________________________
class generalOptions : public optionsGroup
{
  public:

    static SMARTP<generalOptions> create (
      S_optionsHandler optionsHandler);
        
    SMARTP<generalOptions>        createCloneWithTrueValues (); // JMI

  public:

    // initialisation
    // ------------------------------------------------------

    void                  initializeGeneralOptions (
                            bool boolOptionsInitialValue);
        
    void                  printGeneralOptionsHelp ();

    void                  printGeneralOptionsValues (int fieldWidth);
    
  protected:
  
    // constructors/destructor
    // ------------------------------------------------------

    generalOptions (
      S_optionsHandler optionsHandler);
  
    virtual ~generalOptions ();

  public:

    // consistency check
    // ------------------------------------------------------

    void                  checkGeneralOptionsConsistency () const;

    // services
    // ------------------------------------------------------

    virtual S_optionsItem handleOptionsItem (
                            ostream&      os,
                            S_optionsItem item);

  public:

    // command line
    // --------------------------------------

    string                fProgramName;
    
    string                fCommandLineWithLongOptions;
    string                fCommandLineWithShortOptions;


    // display
    // --------------------------------------
  
    bool                  fDisplayOptionsValues;

    // warning and error handling
    // --------------------------------------
      
    bool                  fQuiet;
    bool                  fIgnoreErrors;
    bool                  fAbortOnErrors;
    
    // CPU usage
    // --------------------------------------
  
    bool                  fDisplayCPUusage;

    // exit after some passes
    // --------------------------------------

    bool                  fExit2a;
    bool                  fExit2b;
    bool                  fExit3;
};
typedef SMARTP<generalOptions> S_generalOptions;
EXP ostream& operator<< (ostream& os, const S_generalOptions& elt);

EXP extern S_generalOptions gGeneralOptions;
EXP extern S_generalOptions gGeneralOptionsUserChoices;
EXP extern S_generalOptions gGeneralOptionsWithDetailedTrace;

//______________________________________________________________________________
void initializeGeneralOptionsHandling (
  S_optionsHandler optionsHandler);


}


#endif
