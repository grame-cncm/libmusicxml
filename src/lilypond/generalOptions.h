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

#include "optionsHandling.h"


namespace MusicXML2 
{

//______________________________________________________________________________
// General options
//______________________________________________________________________________

// version
// --------------------------------------

#define _VERSION_LONG_NAME_  "version"
#define _VERSION_SHORT_NAME_ "v"

// help
// --------------------------------------

#define _HELP_LONG_NAME_  "help"
#define _HELP_SHORT_NAME_ "h"

#define _HELP_GENERAL_LONG_NAME_  "helpGeneral"
#define _HELP_GENERAL_SHORT_NAME_ "hg"

#define _HELP_MUSICXML_LONG_NAME_  "helpMusicXML"
#define _HELP_MUSICXML_SHORT_NAME_ "hmxml"

#define _HELP_MSR_LONG_NAME_  "helpMsr"
#define _HELP_MSR_SHORT_NAME_ "hm"

#define _HELP_LPSR_LONG_NAME_  "helpLpsr"
#define _HELP_LPSR_SHORT_NAME_ "hlp"

#define _HELP_LILYPOND_LONG_NAME_  "helpLilyPond"
#define _HELP_LILYPOND_SHORT_NAME_ "hlily"

// output file
// --------------------------------------

#define _OUTPUT_FILE_LONG_NAME_  "outputFile"
#define _OUTPUT_FILE_SHORT_NAME_ "of"

#define _AUTO_OUTPUT_FILE_LONG_NAME_  "autoOutputFile"
#define _AUTO_OUTPUT_FILE_SHORT_NAME_ "aof"

// trace and display
// --------------------------------------

#define _TRACE_GENERAL_LONG_NAME_  "traceGeneral"
#define _TRACE_GENERAL_SHORT_NAME_ "t"

#define _TRACE_DETAILED_LONG_NAME_  "traceDetailed"
#define _TRACE_DETAILED_SHORT_NAME_ "tdetail"

// CPU usage
// --------------------------------------

#define _CPU_USAGE_LONG_NAME_  "displayCPUusage"
#define _CPU_USAGE_SHORT_NAME_ "dCPU"

// specific trace    
// --------------------------------------

// divisions
#define _TRACE_DIVISIONS_LONG_NAME_  "traceDivisions"
#define _TRACE_DIVISIONS_SHORT_NAME_ "tdivs"

// geometry
#define _TRACE_GEOMETRY_LONG_NAME_  "traceGeometry"
#define _TRACE_GEOMETRY_SHORT_NAME_ "tgeom"

// part groups
#define _TRACE_PARTGROUPS_LONG_NAME_  "tracePartGroups"
#define _TRACE_PARTGROUPS_SHORT_NAME_ "tpgrps"

// parts
#define _TRACE_PARTS_LONG_NAME_  "traceParts"
#define _TRACE_PARTS_SHORT_NAME_ "tparts"

// staves
#define _TRACE_STAVES_LONG_NAME_  "traceStaves"
#define _TRACE_STAVES_SHORT_NAME_ "tstaves"

// voices
#define _TRACE_VOICES_LONG_NAME_  "traceVoices"
#define _TRACE_VOICES_SHORT_NAME_ "tvoices"

#define _TRACE_VOICES_DETAILS_LONG_NAME_  "traceVoicesDetails"
#define _TRACE_VOICES_DETAILS_SHORT_NAME_ "tvdetails"

// clefs
#define _TRACE_CLEFS_LONG_NAME_  "traceClefs"
#define _TRACE_CLEFS_SHORT_NAME_ "tclefs"

// keys
#define _TRACE_KEYS_LONG_NAME_  "traceKeys"
#define _TRACE_KEYS_SHORT_NAME_ "tkeys"

// times
#define _TRACE_TIMES_LONG_NAME_  "traceTimes"
#define _TRACE_TIMES_SHORT_NAME_ "ttimes"

// transpositions
#define _TRACE_TRANSPOSITIONS_LONG_NAME_  "traceTranspositions"
#define _TRACE_TRANSPOSITIONS_SHORT_NAME_ "ttransps"

// segments
#define _TRACE_SEGMENTS_LONG_NAME_  "traceSegments"
#define _TRACE_SEGMENTS_SHORT_NAME_ "tsegs"

// repeats
#define _TRACE_REPEATS_LONG_NAME_  "traceRepeats"
#define _TRACE_REPEATS_SHORT_NAME_ "treps"

// measures
#define _TRACE_MEASURES_LONG_NAME_  "traceMeasures"
#define _TRACE_MEASURES_SHORT_NAME_ "tmeas"

// notes
#define _TRACE_NOTES_LONG_NAME_  "traceNotes"
#define _TRACE_NOTES_SHORT_NAME_ "tnotes"

#define _TRACE_NOTES_DETAILS_LONG_NAME_  "traceNotesDetails"
#define _TRACE_NOTES_DETAILS_SHORT_NAME_ "tndetails"

// beams
#define _TRACE_BEAMS_LONG_NAME_  "traceBeams"
#define _TRACE_BEAMS_SHORT_NAME_ "tbeams"

// technicals
#define _TRACE_TECHNICALS_LONG_NAME_  "traceTechnicals"
#define _TRACE_TECHNICALS_SHORT_NAME_ "ttechs"

// words
#define _TRACE_WORDS_LONG_NAME_  "traceWordss"
#define _TRACE_WORDS_SHORT_NAME_ "twords"

// tremolos
#define _TRACE_TREMOLOS_LONG_NAME_  "traceTremolos"
#define _TRACE_TREMOLOS_SHORT_NAME_ "ttrems"

// chords
#define _TRACE_CHORDS_LONG_NAME_  "traceChords"
#define _TRACE_CHORDS_SHORT_NAME_ "tchords"

// tuplets
#define _TRACE_TUPLETS_LONG_NAME_  "traceTuplets"
#define _TRACE_TUPLETS_SHORT_NAME_ "ttups"

// grace notes
#define _TRACE_GRACENOTES_LONG_NAME_  "traceGraceNotes"
#define _TRACE_GRACENOTES_SHORT_NAME_ "tgraces"

// lyrics
#define _TRACE_LYRICS_LONG_NAME_  "traceLyrics"
#define _TRACE_LYRICS_SHORT_NAME_ "tlyrics"

// harmonies
#define _TRACE_HARMONIES_LONG_NAME_  "traceHarmonies"
#define _TRACE_HARMONIES_SHORT_NAME_ "tharms"

// figured bass
#define _TRACE_FIGURED_BASS_LONG_NAME_  "traceFiguredBass"
#define _TRACE_FIGURED_BASS_SHORT_NAME_ "tfigbass"

// credits
#define _TRACE_CREDITS_LONG_NAME_  "traceCredits"
#define _TRACE_CREDITS_SHORT_NAME_ "tcredits"


class EXP msrGeneralOptions : public msrOptionsGroup
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
    set<int>              fTraceDetailedMeasureNumbersSet;

    // CPU usage
    // --------------------------------------
  
    bool                  fDisplayCPUusage;

    // specific trace
    // --------------------------------------
    
    // divisions
    bool                  fTraceDivisions;

    // geometry
    bool                  fTraceGeometry;

    // part groups
    bool                  fTracePartGroups;
    
    // parts
    bool                  fTraceParts;
    
    // staves
    bool                  fTraceStaves;
    
    // voices
    bool                  fTraceVoices;
    bool                  fTraceVoicesDetails;

    // clefs
    bool                  fTraceClefs;
    // keys
    bool                  fTraceKeys;
    // times
    bool                  fTraceTimes;

    //transpositions
    bool                  fTraceTranspositions;

    // segments
    bool                  fTraceSegments;

    // repeats
    bool                  fTraceRepeats;

    // measures
    bool                  fTraceMeasures;

    // notes
    bool                  fTraceNotes;
    bool                  fTraceNotesDetails;

    // beams
    bool                  fTraceBeams;
    
    // technicals
    bool                  fTraceTechnicals;

    // words
    bool                  fTraceWords;

    // tremolos
    bool                  fTraceTremolos;

    // chords
    bool                  fTraceChords;
    
    // tuplets
    bool                  fTraceTuplets;
    
    // grace notes
    bool                  fTraceGraceNotes;
    
    // lyrics
    bool                  fTraceLyrics;

    // harmonies
    bool                  fTraceHarmonies;

    // figured bass
    bool                  fTraceFiguredBass;
    
    // credits
    bool                  fTraceCredits;
    
    /* STUFF not yet handled JMI */

    bool                  fTraceScore;


    bool                  fTraceDynamics;
    
    bool                  fTraceSlurs;
    
    bool                  fTraceLigatures;
    
    bool                  fTraceWedges;
        
    bool                  fTraceStaffTuning;
    
    bool                  fTraceMidi;
};
typedef SMARTP<msrGeneralOptions> S_msrGeneralOptions;

extern S_msrGeneralOptions gGeneralOptions;
extern S_msrGeneralOptions gGeneralOptionsUserChoices;
extern S_msrGeneralOptions gGeneralOptionsWithDetailedTrace;

/*!
\brief A msr midi representation.

  A midi is represented by variable/value pairs
*/
//______________________________________________________________________________
void initializeGeneralOptions ();

/*! @} */

}


#endif
