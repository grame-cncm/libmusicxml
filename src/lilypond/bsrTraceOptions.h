/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef ___bsrTraceOptions___
#define ___bsrTraceOptions___

#include "exports.h"

#include "optionsBasicTypes.h"


//using namespace std;

namespace MusicXML2
{

//______________________________________________________________________________
class bsrTraceOptions : public optionsGroup
{
  public:

    static SMARTP<bsrTraceOptions> create (
      S_optionsHandler optionsHandler);
        
    SMARTP<bsrTraceOptions>        createCloneWithDetailedTrace ();

  public:

    // initialisation
    // ------------------------------------------------------

    void                  initializeTraceOptions (
                            bool boolOptionsInitialValue);
        
  protected:
  
    // constructors/destructor
    // ------------------------------------------------------

    bsrTraceOptions (
      S_optionsHandler optionsHandler);
  
    virtual ~bsrTraceOptions ();

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

    void                  setAllTraceOptions (
                            bool boolOptionsInitialValue);

  public:

    // services
    // ------------------------------------------------------

    virtual S_optionsItem handleOptionsItem (
                            ostream&      os,
                            S_optionsItem item);

  public:

    // print
    // ------------------------------------------------------

    void                  printTraceOptionsHelp ();

    void                  printTraceOptionsValues (int fieldWidth);
    
  public:

    // BSR
    bool                  fTraceBsr;

    // pages & lines  
    bool                  fTracePages;
    
    bool                  fTraceLines;
    
    bool                  fTraceSpaces;
    bool                  fTraceNumbers;

    // parallels
    bool                  fTraceParallels;

    // visitors
    bool                  fTraceBsrVisitors;
};
typedef SMARTP<bsrTraceOptions> S_bsrTraceOptions;
EXP ostream& operator<< (ostream& os, const S_bsrTraceOptions& elt);

EXP extern S_bsrTraceOptions gBsrTraceOptions;
EXP extern S_bsrTraceOptions gBsrTraceOptionsUserChoices;
EXP extern S_bsrTraceOptions gBsrTraceOptionsWithDetailedTrace;

//______________________________________________________________________________
void initializeBsrTraceOptionsHandling (
  S_optionsHandler optionsHandler);


}


#endif
