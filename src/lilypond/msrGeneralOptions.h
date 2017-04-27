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

// version

#define _VERSION_LONG_NAME_  "version"
#define _VERSION_SHORT_NAME_ "v"

// help

#define _HELP_LONG_NAME_  "help"
#define _HELP_SHORT_NAME_ "h"

#define _HELP_GENERAL_LONG_NAME_  "helpGeneral"
#define _HELP_GENERAL_SHORT_NAME_ "hg"

#define _HELP_MSR_LONG_NAME_  "helpMsr"
#define _HELP_MSR_SHORT_NAME_ "hm"

#define _HELP_LPSR_LONG_NAME_  "helpLpsr"
#define _HELP_LPSR_SHORT_NAME_ "hlp"

// output file

#define _OUTPUT_FILE_LONG_NAME_  "outputFile"
#define _OUTPUT_FILE_SHORT_NAME_ "of"

#define _AUTO_OUTPUT_FILE_LONG_NAME_  "autoOutputFile"
#define _AUTO_OUTPUT_FILE_SHORT_NAME_ "aof"

// trace and display

#define _TRACE_GENERAL_LONG_NAME_  "traceGeneral"
#define _TRACE_GENERAL_SHORT_NAME_ "t"

#define _TRACE_DETAILED_LONG_NAME_  "traceDetailed"
#define _TRACE_DETAILED_SHORT_NAME_ "tdetail"

// MusicXML

#define _INGORE_MUSICXML_ERRORS_LONG_NAME_  "ingoreMusicXMLErrors"
#define _INGORE_MUSICXML_ERRORS_SHORT_NAME_ "ime"

// CPU usage

#define _CPU_USAGE_LONG_NAME_  "displayCPUusage"
#define _CPU_USAGE_SHORT_NAME_ "dCPU"

// specific trace    

#define _TRACE_DIVISIONS_LONG_NAME_  "traceDivisions"
#define _TRACE_DIVISIONS_SHORT_NAME_ "tdivs"

#define _TRACE_PARTS_LONG_NAME_  "traceParts"
#define _TRACE_PARTS_SHORT_NAME_ "tparts"

#define _TRACE_VOICES_LONG_NAME_  "traceVoices"
#define _TRACE_VOICES_SHORT_NAME_ "tvoices"

#define _TRACE_SEGMENTS_LONG_NAME_  "traceSegments"
#define _TRACE_SEGMENTS_SHORT_NAME_ "tsegs"

#define _TRACE_REPEATS_LONG_NAME_  "traceRepeats"
#define _TRACE_REPEATS_SHORT_NAME_ "treps"

#define _TRACE_MEASURES_LONG_NAME_  "traceMeasures"
#define _TRACE_MEASURES_SHORT_NAME_ "tmeas"

#define _TRACE_NOTES_LONG_NAME_  "traceNotes"
#define _TRACE_NOTES_SHORT_NAME_ "tnotes"

#define _TRACE_TECHNICALS_LONG_NAME_  "traceTechnicals"
#define _TRACE_TECHNICALS_SHORT_NAME_ "ttechs"

#define _TRACE_TREMOLOS_LONG_NAME_  "traceTremolos"
#define _TRACE_TREMOLOS_SHORT_NAME_ "ttrem"

#define _TRACE_CHORDS_LONG_NAME_  "traceChords"
#define _TRACE_CHORDS_SHORT_NAME_ "tchords"

#define _TRACE_TUPLETS_LONG_NAME_  "traceTuplets"
#define _TRACE_TUPLETS_SHORT_NAME_ "ttuplets"

#define _TRACE_GRACENOTES_LONG_NAME_  "traceGracenotes"
#define _TRACE_GRACENOTES_SHORT_NAME_ "tgrace"

#define _TRACE_LYRICS_LONG_NAME_  "traceLyrics"
#define _TRACE_LYRICS_SHORT_NAME_ "tlyrics"

#define _TRACE_HARMONIES_LONG_NAME_  "traceHarmonies"
#define _TRACE_HARMONIES_SHORT_NAME_ "tharms"


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

    // MusicXML
    // --------------------------------------
  
    bool                  fIgnoreMusicXMLErrors;

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
    
    bool                  fTraceTechnicals;

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
