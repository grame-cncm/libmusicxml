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
    
    SMARTP<msrGeneralOptions>        createCloneWithDetailedTrace ();
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

    // command line
    // --------------------------------------

    string                fProgramName;
    
    string                fCommandLineLongOptions;
    string                fCommandLineShortOptions;

    // input
    // --------------------------------------

    string                fInputSourceName;
    
    string                fTranslationDate;

    // output file
    // --------------------------------------

    string                fOutputFileName;
    bool                  fAutoOutputFile;
    
    // trace and display
    // --------------------------------------
  
    bool                  fTraceGeneral;

    bool                  fTraceDetailed;
    set<int>              fTraceAllMeasureNumbersSet;

    // CPU usage
    // --------------------------------------
  
    bool                  fDisplayCPUusage;

    // specific trace
    // --------------------------------------
    
    bool                  fTraceDivisions;

    bool                  fTraceParts;
    bool                  fTraceVoices;

    bool                  fTraceSegments;

    bool                  fTraceRepeats;

    bool                  fTraceMeasures;

    bool                  fTraceNotes;

    bool                  fTraceTremolos;

    bool                  fTraceChords;
    bool                  fTraceTuplets;
    
    bool                  fTraceGracenotes;
    
    bool                  fTraceLyrics;

    bool                  fTraceHarmonies;


    /* STUFF not yet handled JMI */

    bool                  fTraceScore;

    bool                  fTracePartgroups;
    bool                  fTraceStaves;

    bool                  fTraceDynamics;
    bool                  fTraceWords;
    bool                  fTraceSlurs;
    bool                  fTraceLigatures;
    bool                  fTraceWedges;
        
    bool                  fTraceStafftuning;
    
    bool                  fTraceMidi;
};
typedef SMARTP<msrGeneralOptions> S_msrGeneralOptions;

extern S_msrGeneralOptions gGeneralOptions;
extern S_msrGeneralOptions gGeneralOptionsUserChoices;
extern S_msrGeneralOptions gGeneralOptionsWithDetailedTrace;


/*! @} */

}


#endif
