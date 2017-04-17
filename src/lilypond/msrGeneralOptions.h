/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef __msrGeneralOptions__
#define __msrGeneralOptions__

#include <sstream>
#include <vector>
#include <string>
#include <list>
#include <map>

#include "smartpointer.h"
/*
#include "exports.h"
#include "typedefs.h"

#include "tree_browser.h"

#include "msrUtilities.h"

//JMI #include "musicxml2msr.h"
*/

namespace MusicXML2 
{

//______________________________________________________________________________
/*!
  \brief The LilyPond note names language.
*/

//______________________________________________________________________________
/*!
  \brief The general options.

  A class is used to avoid passing arguments one by one
  to the various methods that need them.
*/
class EXP msrGeneralOptions : public smartable
{
  public:

    static SMARTP<msrGeneralOptions> create ();
    
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

    msrGeneralOptions();
  
    virtual ~msrGeneralOptions();
 
  public:

    // synthetic view
    string                fProgramName;
    string                fCommandLineOptions;
    string                fInputSourceName;
    string                fTranslationDate;

    string                fOutputFileName;
    bool                  fAutoOutputFile;
    
    // trace
    bool                  fTraceGeneral;
    
    bool                  fTraceDivisions;

    bool                  fTraceScore;

    bool                  fTracePartgroups;
    bool                  fTraceParts;
    bool                  fTraceStaves;
    bool                  fTraceVoices;

    bool                  fTraceSegments;
    bool                  fTraceMeasures;
    
    bool                  fTraceChords;
    bool                  fTraceTuplets;
    
    bool                  fTraceGracenotes;
    
    bool                  fTraceLyrics;

    bool                  fTraceHarmonies;



    
    
    
    bool                  fTraceNotes;
    
    bool                  fTraceTremolos;
    
    bool                  fTraceDynamics;
    bool                  fTraceWords;
    bool                  fTraceSlurs;
    bool                  fTraceLigatures;
    bool                  fTraceWedges;
    
    bool                  fTraceRepeats;
        
    bool                  fTraceStafftuning;
    
    bool                  fTraceMidi;

    bool                  fTraceALL;

    // CPU usage
    bool                  fDisplayCPUusage;
        
    // measure number-selective debug
    set<int>              fTraceAllMeasureNumbersSet;
    bool                  fSaveDebug;
    bool                  fSaveDebugDebug;
};
typedef SMARTP<msrGeneralOptions> S_msrGeneralOptions;

extern S_msrGeneralOptions gGeneralOptions;
extern S_msrGeneralOptions gGeneralOptionsUserChoices;
extern S_msrGeneralOptions gGeneralOptionsTraceAll;


/*! @} */

}


#endif
