/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef __lpsrOptions__
#define __lpsrOptions__

#include "lpsr.h"


using namespace std;

namespace MusicXML2 
{

//______________________________________________________________________________
// LPSR options

// trace and display

#define _TRACE_LPSR_LONG_NAME_  "traceLpsr"
#define _TRACE_LPSR_SHORT_NAME_ "tlpsr"

#define _TRACE_LPSR_VISITORS_LONG_NAME_  "traceLpsrVisitors"
#define _TRACE_LPSR_VISITORS_SHORT_NAME_ "tlpvisits"

#define _DISPLAY_LPSR_LONG_NAME_  "displayLpsr"
#define _DISPLAY_LPSR_SHORT_NAME_ "lpsr"

// Scheme functions

#define _TRACE_SCHEME_FUNCTIONS_LONG_NAME_  "traceSchemeFunctions"
#define _TRACE_SCHEME_FUNCTIONS_SHORT_NAME_ "tscheme"

// languages

#define _LPSR_PITCHES_LANGUAGE_LONG_NAME_  "lpsrPitchesLanguage"
#define _LPSR_PITCHES_LANGUAGE_SHORT_NAME_ "lppl"

#define _LPSR_CHORDS_LANGUAGE_LONG_NAME_  "lpsrChordsLanguage"
#define _LPSR_CHORDS_LANGUAGE_SHORT_NAME_ "lpcl"

class EXP lpsrOptions : public msrOptionsGroup
{
  public:

    static SMARTP<lpsrOptions> create ();
    
    SMARTP<lpsrOptions>        createCloneWithDetailedTrace ();

  public:

    // initialisation
    // ------------------------------------------------------

    void                  initializeLpsrOptions (
                            bool boolOptionsInitialValue);
    
    void                  printLpsrOptionsHelp ();

    void                  printLpsrOptionsValues (int fieldWidth);
    
  public:
  
    // constructors/destructor
    // ------------------------------------------------------

    lpsrOptions();
    
    virtual ~lpsrOptions();

    // set and get
    // ------------------------------------------------------

    bool                  setLpsrQuarterTonesPitchesLanguage (
                            string language);    
 
    bool                  setLpsrChordsLanguage (
                            string language);    
 
  public:

    // trace and display
    // --------------------------------------
    
    bool                  fTraceLpsr;
    
    bool                  fTraceLpsrVisitors;
    
    bool                  fDisplayLpsr;
    
    // Scheme functions
    bool                  fTraceSchemeFunctions;

    // languages
    // --------------------------------------

    msrQuarterTonesPitchesLanguage
                          fLpsrQuarterTonesPitchesLanguage;
    
    lpsrChordsLanguage    fLpsrChordsLanguage;
};
typedef SMARTP<lpsrOptions> S_lpsrOptions;

extern S_lpsrOptions gLpsrOptions;
extern S_lpsrOptions gLpsrOptionsUserChoices;
extern S_lpsrOptions gLpsrOptionsWithDetailedTrace;


/*! @} */

}


#endif
