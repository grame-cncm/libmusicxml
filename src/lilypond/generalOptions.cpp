/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#include <iomanip>      // setw, set::precision, ...

#include "msr.h"

#include "generalOptions.h"


using namespace std;

namespace MusicXML2 
{

// useful shortcut macros
#define idtr indenter::gIndenter
#define tab  indenter::gIndenter.getSpacer ()

//_______________________________________________________________________________
S_generalOptions gGeneralOptions;
S_generalOptions gGeneralOptionsUserChoices;
S_generalOptions gGeneralOptionsWithDetailedTrace;

S_generalOptions generalOptions::create ()
{
  generalOptions* o = new generalOptions();
  assert(o!=0);
  return o;
}

generalOptions::generalOptions ()
  : msrOptionsGroup (
    "hg", "helpGeneral",
R"(General)"
    )
{
  initializeGeneralOptions (false);
}

generalOptions::~generalOptions ()
{}

void generalOptions::initializeGeneralOptions (
  bool boolOptionsInitialValue)
{
  // version
  // --------------------------------------

  {
    // variables  
  
    fVersion = false;
  
    // options
  
    S_msrOptionsSubGroup versionSubGroup =
      msrOptionsSubGroup::create (
      "hgb", "helpGeneralVersion",
  R"(Version)"
      );
  
    appendOptionsSubGroup (versionSubGroup);
  
    versionSubGroup->
      appendOptionsItem (
        msrOptionsBooleanItem::create (
          "v", "version",
  R"(Display xml2lilypond's version number and exit.)",
          "version",
          fVersion));
  }

  // output file
  // --------------------------------------

  {
    // variables  
  
    fAutoOutputFile = false;
    
    // options
  
    S_msrOptionsSubGroup outputFileSubGroup =
      msrOptionsSubGroup::create (
      "hgof", "helpGeneralOutputFile",
R"(Output file)"
      );
            
    appendOptionsSubGroup (outputFileSubGroup);
    outputFileSubGroup->
      appendOptionsItem (
        msrOptionsStringItem::create (
          "of", "outputFile",
  R"(Write LilyPond code to file 'fileName' instead of standard output.)",
          "outputFile",
          fOutputFileName));
  }
  
  // trace and display
  // --------------------------------------

  {
    // variables  
    
    fTraceGeneral = boolOptionsInitialValue;
    
    fTraceDetailed = false;
    
    // options
    
      S_msrOptionsSubGroup traceAndDisplaySubGroup =
        msrOptionsSubGroup::create (
        "hgtd", "helpGeneralTraceDansDisplay",
R"(Trace and display)"
        );
    
      appendOptionsSubGroup (traceAndDisplaySubGroup);
          
      traceAndDisplaySubGroup->
        appendOptionsItem (
          msrOptionsBooleanItem::create (
            "t", "traceGeneral",
R"(Write a trace of the general activity to standard error.)",
            "traceGeneral",
            fTraceGeneral));
    
      traceAndDisplaySubGroup->
        appendOptionsItem (
          msrOptionsBooleanItem::create (
            "tdetail", "traceDetailed",
R"('measureNumbersSet' has a form such as '0,2-14,^8-10',
where '^' excludes the corresponding numbers interval
and 0 applies to the '<part-list>' and anacrusis if present.
The measure numbers should be those of the produced score, 
since MusicXML measure numbers are arbitrary strings.
Generate a detailed trace of the activity and print additional
debugging information to standard error for the specified measures.)",
            "traceDetailed",
            fTraceDetailed));
  }

  // CPU usage
  // --------------------------------------

  {
    // variables  
  
    fDisplayCPUusage = boolOptionsInitialValue;
      
    // options
  
    S_msrOptionsSubGroup CPUUsageSubGroup =
      msrOptionsSubGroup::create (
      "hgcu", "helpGeneralCPUUsage",
  R"(CPU usage)"
      );
  
    appendOptionsSubGroup (CPUUsageSubGroup);
        
    CPUUsageSubGroup->
      appendOptionsItem (
        msrOptionsBooleanItem::create (
          "dCPU", "displayCPUusage",
  R"(Write a trace of the general activity to standard error.)",
          "displayCPUusage",
          fDisplayCPUusage));
  }

  // specific trace    
  // --------------------------------------

  {
    // variables  
  
    // divisions
    fTraceDivisions = boolOptionsInitialValue;
  
    // geometry
    fTraceGeometry = boolOptionsInitialValue;
  
    // part groups
    fTracePartGroups = boolOptionsInitialValue;
    
    // parts
    fTraceParts = boolOptionsInitialValue;
    
    // staves
    fTraceStaves = boolOptionsInitialValue;
    
    // voices
    fTraceVoices = boolOptionsInitialValue;
    fTraceVoicesDetails = boolOptionsInitialValue;
  
    // clefs
    fTraceClefs = boolOptionsInitialValue;
    
    // keys
    fTraceKeys  = boolOptionsInitialValue;
    
    // times
    fTraceTimes = boolOptionsInitialValue;
  
    // transpositions
    fTraceTranspositions = boolOptionsInitialValue;
  
    // segments
    fTraceSegments = boolOptionsInitialValue;
    
    // repeats
    fTraceRepeats = boolOptionsInitialValue;
  
    // measures
    fTraceMeasures = boolOptionsInitialValue;
  
    // notes
    fTraceNotes = boolOptionsInitialValue;
    fTraceNotesDetails = boolOptionsInitialValue;
    
    // beams
    fTraceBeams = boolOptionsInitialValue;
    
    // technicals
    fTraceTechnicals = boolOptionsInitialValue;
    
    // words
    fTraceWords = boolOptionsInitialValue;
    
    // tremolos
    fTraceTremolos = boolOptionsInitialValue;
    
    // chords
    fTraceChords = boolOptionsInitialValue;
    
    // tuplets
    fTraceTuplets = boolOptionsInitialValue;
    
    // grace notes
    fTraceGraceNotes = boolOptionsInitialValue;
  
    // lyrics
    fTraceLyrics = boolOptionsInitialValue;
  
    // harmonies
    fTraceHarmonies = boolOptionsInitialValue;
  
    // figured bass
    fTraceFiguredBass = boolOptionsInitialValue;
      
    // credits
    fTraceCredits = boolOptionsInitialValue;
      
    // options
  
    S_msrOptionsSubGroup specificTraceSubGroup =
      msrOptionsSubGroup::create (
      "hgst", "helpGeneralSpecificTrace",
R"(Specific trace about elements)"
      );
  
    appendOptionsSubGroup (specificTraceSubGroup);

    specificTraceSubGroup->
      appendOptionsItem (
        msrOptionsBooleanItem::create (
          "tdivs", "traceDivisions",
R"(divisions)",
          "traceDivisions",
          fTraceDivisions));
  }
      

  /* STUFF not yet handled JMI */

  fTraceScore = boolOptionsInitialValue;


  fTraceDynamics = boolOptionsInitialValue;
  fTraceWords = boolOptionsInitialValue;
  fTraceSlurs = boolOptionsInitialValue;
  fTraceLigatures = boolOptionsInitialValue;
  fTraceWedges = boolOptionsInitialValue;
  
  fTraceStaffTuning = boolOptionsInitialValue;
  
  fTraceMidi = boolOptionsInitialValue;
}

S_generalOptions generalOptions::createCloneWithDetailedTrace ()
{
  S_generalOptions
    clone =
      generalOptions::create ();

  // command line
  // --------------------------------------

  clone->fProgramName =
    fProgramName;
      
  clone->fCommandLineLongOptions =
    fCommandLineLongOptions;
  clone->fCommandLineShortOptions =
    fCommandLineShortOptions;
  
  // input
  // --------------------------------------

  clone->fInputSourceName =
    fInputSourceName;
      
  clone->fTranslationDate =
    fTranslationDate;

  // version
  // --------------------------------------

  clone->fVersion =
    fVersion;

  // output file
  // --------------------------------------

  clone->fAutoOutputFile =
    fAutoOutputFile;
  
  // trace and display
  // --------------------------------------

  clone->fTraceGeneral = true;
    
  clone->fTraceDetailed = true;

  clone->fTraceDetailedMeasureNumbersSet =
    fTraceDetailedMeasureNumbersSet;

  // CPU usage
  // --------------------------------------

  clone->fDisplayCPUusage =
    true;
    
  // specific trace
  // --------------------------------------

  // divisions
  clone->fTraceDivisions = true;

  // geometry
  clone->fTraceGeometry = true;

  // part groups
  clone->fTracePartGroups = true;
  
  // parts
  clone->fTraceParts = true;
  
  // staves
  clone->fTraceStaves = true;
  
  // voices
  clone->fTraceVoices = true;
  clone->fTraceVoicesDetails = true;

  // clefs
  clone->fTraceClefs = true;
  
  // keys
  clone->fTraceKeys = true;
  
  // times
  clone->fTraceTimes = true;

  // transpositions
  clone->fTraceTranspositions = true;

  // segments
  clone->fTraceSegments = true;
    
  // repeats
  clone->fTraceRepeats = true;

  // measures
  clone->fTraceMeasures = true;
  
  // notes
  clone->fTraceNotes = true;
  clone->fTraceNotesDetails = true;
    
  // beams
  clone->fTraceBeams = true;
    
  // technicals
  clone->fTraceTechnicals = true;
  
  // words
  clone->fTraceWords = true;
  
  // tremolos
  clone->fTraceTremolos = true;

  // chords
  clone->fTraceChords = true;
    
  // tuplets
  clone->fTraceTuplets = true;
  
  // grace notes
  clone->fTraceGraceNotes = true;

  // lyrics
  clone->fTraceLyrics = true;
  
  // harmonies
  clone->fTraceHarmonies = true;
  
  // figured bass
  clone->fTraceFiguredBass = true;
    
  // credits
  clone->fTraceCredits = true;

    
  /* STUFF not yet handled JMI */

  clone->fTraceScore = true;


  clone->fTraceDynamics = true;

  clone->fTraceSlurs = true;

  clone->fTraceLigatures = true;

  clone->fTraceWedges = true;
  
  clone->fTraceStaffTuning = true;
  
  clone->fTraceMidi = true;

  return clone;
}

void generalOptions::printGeneralOptionsHelp ()
{
  idtr++;

  cerr <<
    idtr << "General:" <<
    endl <<
    idtr << "-------" <<
    endl <<
    endl;

  idtr++;

  // version
  // --------------------------------------
  
  cerr <<
    idtr << "Version:" <<
    endl <<
    endl;

  idtr++;

  cerr <<
    idtr <<
      "--" _VERSION_SHORT_NAME_ ", --" _VERSION_LONG_NAME_ <<
      endl <<
    idtr << tab << tab << tab <<
      "Display xml2lilypond's version number and exit." <<
      endl <<
    endl;

  idtr--;

  // help
  // --------------------------------------
  
  cerr <<
    idtr << "Help:" <<
    endl <<
    endl;

  idtr++;

  cerr <<
    idtr <<
      "--" _HELP_SHORT_NAME_ ", --" _HELP_LONG_NAME_ <<
      endl <<
    idtr << tab << tab << tab <<
      "Display this help and exit." <<
      endl <<
    endl <<
      
    idtr <<
      "--" _HELP_GENERAL_SHORT_NAME_ ", --" _HELP_GENERAL_LONG_NAME_ <<
      endl <<
    idtr << tab << tab << tab <<
      "Display general help and exit." <<
      endl <<
      
    idtr <<
      "--" _HELP_MUSICXML_SHORT_NAME_ ", --" _HELP_MUSICXML_LONG_NAME_ <<
      endl <<
    idtr << tab << tab << tab <<
      "Display MusicXML help and exit." <<
      endl <<
      
    idtr <<
      "--" _HELP_MSR_SHORT_NAME_ ", --" _HELP_MSR_LONG_NAME_ <<
      endl <<
    idtr << tab << tab << tab <<
      "Display MSR help and exit." <<
      endl <<
      
    idtr <<
      "--" _HELP_LPSR_SHORT_NAME_ ", --" _HELP_LPSR_LONG_NAME_ <<
      endl <<
    idtr << tab << tab << tab <<
      "Display LPSR help and exit." <<
      endl <<
      
    idtr <<
      "--" _HELP_LILYPOND_SHORT_NAME_ ", --" _HELP_LILYPOND_LONG_NAME_ <<
      endl <<
    idtr << tab << tab << tab <<
      "Display LilyPond help and exit." <<
      endl <<
    endl;

  idtr--;

  // output file
  // --------------------------------------
  
  cerr <<
    idtr << "Output file:" <<
    endl <<
    endl;

  idtr++;

  cerr <<
    idtr <<
      "--" _OUTPUT_FILE_SHORT_NAME_ ", --" _OUTPUT_FILE_LONG_NAME_ " fileName" <<
      endl <<
    idtr << tab << tab << tab <<
      "Write LilyPond code to file 'fileName' instead of standard output." <<
      endl <<
    endl <<
    
    idtr <<
      "--" _AUTO_OUTPUT_FILE_SHORT_NAME_ ", --" _AUTO_OUTPUT_FILE_LONG_NAME_ <<
      endl <<
    idtr << tab << tab << tab <<
      "This option can only be used when reading from a file." <<
      endl <<
    idtr << tab << tab << tab <<
      "Write LilyPond code to a file in the current working directory." <<
      endl <<
    idtr << tab << tab << tab <<
      "The file name is derived from that of the input file," <<
      endl <<
    idtr << tab << tab << tab <<
      "replacing any suffix after the the '.' by 'ly' or adding '.ly' if none is present." <<
      endl <<
    endl;

  idtr--;

  // trace and display
  // --------------------------------------
  
  cerr <<
    idtr << "Trace and display:" <<
    endl <<
    endl;

  idtr++;

  cerr <<
    idtr <<
      "--" _TRACE_GENERAL_SHORT_NAME_ ", --" _TRACE_GENERAL_LONG_NAME_ <<
      endl <<
    idtr << tab << tab << tab <<
      "Write a trace of the general activity to standard error." <<
      endl <<
    endl <<
      
    idtr <<
      "--" _TRACE_DETAILED_SHORT_NAME_ ", --" _TRACE_DETAILED_LONG_NAME_ " measureNumbersSet" <<
      endl <<
    idtr << tab << tab << tab <<
      "'measureNumbersSet' has a form such as '0,2-14,^8-10'," <<
      endl <<
    idtr << tab << tab << tab <<
      "where '^' excludes the corresponding numbers interval" <<
      endl <<
    idtr << tab << tab << tab <<
      "and 0 applies to the '<part-list>' and anacrusis if present." << endl <<
    idtr << tab << tab << tab <<
      "The measure numbers should be those of the produced score, " << endl <<
    idtr << tab << tab << tab <<
      "since MusicXML measure numbers are arbitrary strings." <<
      endl <<
    idtr << tab << tab << tab <<
      "Generate a detailed trace of the activity and print additional" <<
      endl <<
    idtr << tab << tab << tab <<
      "debugging information to standard error for the specified measures." <<
      endl <<
    endl;

  idtr--;

  // CPU usage
  // --------------------------------------
  
  cerr <<
    idtr << "CPU usage:" <<
    endl <<
    endl;

  idtr++;

  cerr <<
    idtr <<
      "--" _CPU_USAGE_SHORT_NAME_ ", --" _CPU_USAGE_LONG_NAME_ <<
      endl <<
    idtr << tab << tab << tab <<
      "Write CPU usage by the passes at the end of the translation." <<
      endl <<
    endl;

  idtr--;

  // specific trace
  // --------------------------------------
  
  cerr <<
    idtr << "Specific trace about elements:" <<
    endl <<
    endl;

  idtr++;

  cerr <<
    // divisions
    idtr <<
      "--" _TRACE_DIVISIONS_SHORT_NAME_ ", --" _TRACE_DIVISIONS_LONG_NAME_ <<
      endl <<
    idtr << tab << tab << tab <<
      " divisions" <<
      endl <<
    endl <<
      
    // part groups
    idtr <<
      "--" _TRACE_PARTGROUPS_SHORT_NAME_ ", --" _TRACE_PARTGROUPS_LONG_NAME_ <<
      endl <<
    idtr << tab << tab << tab <<
      "part groups" <<
      endl <<
    endl <<
      
    // parts
    idtr <<
      "--" _TRACE_PARTS_SHORT_NAME_ ", --" _TRACE_PARTS_LONG_NAME_ <<
      endl <<
    idtr << tab << tab << tab <<
      "parts" <<
      endl <<
    endl <<

    // staves
    idtr <<
      "--" _TRACE_STAVES_SHORT_NAME_ ", --" _TRACE_STAVES_LONG_NAME_ <<
      endl <<
    idtr << tab << tab << tab <<
      "staves" <<
      endl <<
    endl <<
      
    // voices
    idtr <<
      "--" _TRACE_VOICES_SHORT_NAME_ ", --" _TRACE_VOICES_LONG_NAME_ <<
      endl <<
    idtr << tab << tab << tab <<
      "voices" <<
      endl <<

    idtr <<
      "--" _TRACE_VOICES_DETAILS_SHORT_NAME_ ", --" _TRACE_VOICES_DETAILS_LONG_NAME_ <<
      endl <<
    idtr << tab << tab << tab <<
      "voices with more details (quite verbose)" <<
      endl <<
    endl <<

     // clefs
    idtr <<
      "--" _TRACE_CLEFS_SHORT_NAME_ ", --" _TRACE_CLEFS_LONG_NAME_ <<
      endl <<
    idtr << tab << tab << tab <<
      "clefs" <<
      endl <<
      
    // keys
    idtr <<
      "--" _TRACE_KEYS_SHORT_NAME_ ", --" _TRACE_KEYS_LONG_NAME_ <<
      endl <<
    idtr << tab << tab << tab <<
      "keys" <<
      endl <<
      
    // times
    idtr <<
      "--" _TRACE_TIMES_SHORT_NAME_ ", --" _TRACE_TIMES_LONG_NAME_ <<
      endl <<
    idtr << tab << tab << tab <<
      "times" <<
      endl <<
    endl <<
     
    // transpositions
    idtr <<
      "--" _TRACE_TRANSPOSITIONS_SHORT_NAME_ ", --" _TRACE_TRANSPOSITIONS_LONG_NAME_ <<
      endl <<
    idtr << tab << tab << tab <<
      "transpositions (<transpose> in MusicXML, \transposition in LilyPond)" <<
      endl <<
    endl <<
     
     // segments
   idtr <<
      "--" _TRACE_SEGMENTS_SHORT_NAME_ ", --" _TRACE_SEGMENTS_LONG_NAME_ <<
      endl <<
    idtr << tab << tab << tab <<
      "voices segments" <<
      endl <<
    endl <<
      
    // repeats
    idtr <<
      "--" _TRACE_REPEATS_SHORT_NAME_ ", --" _TRACE_REPEATS_LONG_NAME_ <<
      endl <<
    idtr << tab << tab << tab <<
      "repeats" <<
      endl <<
    endl <<
      
    // measures
    idtr <<
      "--" _TRACE_MEASURES_SHORT_NAME_ ", --" _TRACE_MEASURES_LONG_NAME_ <<
      endl <<
    idtr << tab << tab << tab <<
      "measures" <<
      endl <<
    endl <<

    // notes
    idtr <<
      "--" _TRACE_NOTES_SHORT_NAME_ ", --" _TRACE_NOTES_LONG_NAME_ <<
      endl <<
    idtr << tab << tab << tab <<
      "notes" <<
      endl <<
    
    idtr <<
      "--" _TRACE_NOTES_DETAILS_SHORT_NAME_ ", --" _TRACE_NOTES_DETAILS_LONG_NAME_ <<
      endl <<
    idtr << tab << tab << tab <<
      "notes with more details, including divisions handling (quite verbose)" <<
      endl <<
    endl <<

    // beams
    idtr <<
      "--" _TRACE_BEAMS_SHORT_NAME_ ", --" _TRACE_BEAMS_LONG_NAME_ <<
      endl <<
    idtr << tab << tab << tab <<
      "beams" <<
      endl <<
    endl <<
      
    // technicals
    idtr <<
      "--" _TRACE_TECHNICALS_SHORT_NAME_ ", --" _TRACE_TECHNICALS_LONG_NAME_ <<
      endl <<
    idtr << tab << tab << tab <<
      "technicals" <<
      endl <<
    endl <<
      
    // words
    idtr <<
      "--" _TRACE_WORDS_SHORT_NAME_ ", --" _TRACE_WORDS_LONG_NAME_ <<
      endl <<
    idtr << tab << tab << tab <<
      "words" <<
      endl <<
    endl <<
      
    // tremolos
    idtr <<
      "--" _TRACE_TREMOLOS_SHORT_NAME_ ", --" _TRACE_TREMOLOS_LONG_NAME_ <<
      endl <<
    idtr << tab << tab << tab <<
      "tremolos" <<
      endl <<
    endl <<

    // chords
    idtr <<
      "--" _TRACE_CHORDS_SHORT_NAME_ ", --" _TRACE_CHORDS_LONG_NAME_ <<
      endl <<
    idtr << tab << tab << tab <<
      "chords" <<
      endl <<
    endl <<

    // tuplets
    idtr <<
      "--" _TRACE_TUPLETS_SHORT_NAME_ ", --" _TRACE_TUPLETS_LONG_NAME_ <<
      endl <<
    idtr << tab << tab << tab <<
      "tuplets" <<
      endl <<
    endl <<

    // grace notes
    idtr <<
      "--" _TRACE_GRACENOTES_SHORT_NAME_ ", --" _TRACE_GRACENOTES_LONG_NAME_ <<
      endl <<
    idtr << tab << tab << tab <<
      "grace notes" <<
      endl <<
    endl <<

    // lyrics
    idtr <<
      "--" _TRACE_LYRICS_SHORT_NAME_ ", --" _TRACE_LYRICS_LONG_NAME_ <<
      endl <<
    idtr << tab << tab << tab <<
      "lyrics in MusicXML, stanzas in MSR" <<
      endl <<
    endl <<

    // harmonies
    idtr <<
      "--" _TRACE_HARMONIES_SHORT_NAME_ ", --" _TRACE_HARMONIES_LONG_NAME_ <<
      endl <<
    idtr << tab << tab << tab <<
      "<harmony/> in MusicXML, \\chordmode in LilyPond" <<
      endl <<      
    endl <<
      
    // figured bass
    idtr <<
      "--" _TRACE_FIGURED_BASS_SHORT_NAME_ ", --" _TRACE_FIGURED_BASS_LONG_NAME_ <<
      endl <<
    idtr << tab << tab << tab <<
      "<figured-bass> in MusicXML, \\figuremode in LilyPond" <<
      endl <<      
    endl <<
      
    // credits
    idtr <<
      "--" _TRACE_CREDITS_SHORT_NAME_ ", --" _TRACE_CREDITS_LONG_NAME_ <<
      endl <<
    idtr << tab << tab << tab <<
      "credits" <<
      endl <<      
    endl;
      
  idtr--;
  
  idtr--;
  

  idtr--;
}

void generalOptions::printGeneralOptionsValues (int fieldWidth)
{  
  cerr << idtr <<
    "The general options are:" <<
    endl;

  idtr++;

  // command line
  // --------------------------------------

  cerr << left <<
    idtr <<
      setw(fieldWidth) << "Command line:" <<
      endl;

  idtr++;

  cerr << left <<
    idtr <<
      setw(fieldWidth) << "input source name" << " : " <<
      fInputSourceName <<
      endl <<
      
    idtr <<
      setw(fieldWidth) << "translation date" << " : " <<
      fTranslationDate <<
      endl;

  idtr--;

  // output file
  // --------------------------------------

  cerr << left <<
    idtr <<
      setw(fieldWidth) << "Output file:" <<
      endl;

  idtr++;

  cerr << left <<        
    idtr <<
      setw(fieldWidth) << "outputFileName" << " : \"" <<
      fOutputFileName <<
      "\"" <<
      endl <<
    idtr <<
      setw(fieldWidth) << "autoOutputFile" << " : " <<
      booleanAsString (fAutoOutputFile) <<
      endl;

  idtr--;

  // trace and display
  // --------------------------------------

  cerr << left <<
    idtr <<
      setw(fieldWidth) << "Trace and display:" <<
      endl;

  idtr++;

  cerr << left <<
    idtr <<
      setw(fieldWidth) << "traceGeneral" << " : " <<
      booleanAsString (fTraceGeneral) <<
       endl <<
       
    idtr <<
      setw(fieldWidth) << "traceDetailed" << " : " <<
      booleanAsString (fTraceDetailed) <<
     endl;
        
    if (fTraceDetailed) {
      cerr <<
        idtr <<
          setw(fieldWidth) << "traceDetailedMeasureNumbersSet" << " : ";
        
      if (fTraceDetailedMeasureNumbersSet.empty ())
        cerr << "none";
        
      else {
        cerr <<
          "'";
          
        set<int>::const_iterator
          iBegin = fTraceDetailedMeasureNumbersSet.begin(),
          iEnd   = fTraceDetailedMeasureNumbersSet.end(),
          i      = iBegin;
          
        for ( ; ; ) {
          cerr << (*i);
          if (++i == iEnd) break;
          cerr << " ";
        } // for
      
        cerr <<
          "'";
      }
      
      cerr <<
        endl;
    }

  idtr--;

  // CPU usage
  // --------------------------------------

  cerr << left <<
    idtr <<
      setw(fieldWidth) << "CPU usage:" <<
      endl;

  idtr++;

  cerr <<
    idtr <<
      setw(fieldWidth) << "displayCPUusage" << " : " <<
      booleanAsString (fDisplayCPUusage) <<
      endl;

  idtr--;
  
  // specific trace    
  // --------------------------------------

  cerr << left <<
    idtr <<
      setw(fieldWidth) << "Specific trace:" <<
      endl;

  idtr++;

  cerr << left <<
    // divisions
    idtr <<
      setw(fieldWidth) << "traceDivisions" << " : " <<
      booleanAsString (fTraceDivisions) <<
      endl <<
        
    // geometry
    idtr <<
      setw(fieldWidth) << "traceGeometry" << " : " <<
      booleanAsString (fTraceGeometry) <<
      endl <<
        
    // part groups
    idtr <<
      setw(fieldWidth) << "tracePartGroups" << " : " <<
      booleanAsString (fTracePartGroups) <<
      endl <<
      
    // parts
    idtr <<
      setw(fieldWidth) << "traceParts" << " : " <<
      booleanAsString (fTraceParts) <<
      endl <<
      
    // staves
    idtr <<
      setw(fieldWidth) << "traceStaves" << " : " <<
      booleanAsString (fTraceStaves) <<
      endl <<
      
    // voices
    idtr <<
      setw(fieldWidth) << "traceVoices" << " : " <<
      booleanAsString (fTraceVoices) <<
      endl <<
    idtr <<
      setw(fieldWidth) << "traceVoicesDetails" << " : " <<
      booleanAsString (fTraceVoicesDetails) <<
      endl <<

    // clefs
    idtr <<
      setw(fieldWidth) << "traceClefs" << " : " <<
      booleanAsString (fTraceClefs) <<
      endl <<
    // keys
    idtr <<
      setw(fieldWidth) << "traceKeys" << " : " <<
      booleanAsString (fTraceKeys) <<
      endl <<
    // times
    idtr <<
      setw(fieldWidth) << "traceTimes" << " : " <<
      booleanAsString (fTraceTimes) <<
      endl <<

    // transpositions
    idtr <<
      setw(fieldWidth) << "traceTranspositions" << " : " <<
      booleanAsString (fTraceTranspositions) <<
      endl <<

    // segments
    idtr <<
      setw(fieldWidth) << "traceSegments" << " : " <<
      booleanAsString (fTraceSegments) <<
      endl <<

    // repeats
    idtr <<
      setw(fieldWidth) << "traceRepeats" << " : " <<
      booleanAsString (fTraceRepeats) <<
      endl <<

    // measures
    idtr <<
      setw(fieldWidth) << "traceMeasures" << " : " <<
      booleanAsString (fTraceMeasures) <<
      endl <<
      
    // notes
    idtr <<
      setw(fieldWidth) << "traceNotes" << " : " <<
      booleanAsString (fTraceNotes) <<
      endl <<
    idtr <<
      setw(fieldWidth) << "traceNotesDetails" << " : " <<
      booleanAsString (fTraceNotesDetails) <<
      endl <<

    // beams
    idtr <<
      setw(fieldWidth) << "traceBeams" << " : " <<
      booleanAsString (fTraceBeams) <<
      endl <<

    // technicals
    idtr <<
      setw(fieldWidth) << "traceTechnicals" << " : " <<
      booleanAsString (fTraceTechnicals) <<
      endl <<

    // words
    idtr <<
      setw(fieldWidth) << "traceWords" << " : " <<
      booleanAsString (fTraceWords) <<
      endl <<

    // tremolos
    idtr <<
      setw(fieldWidth) << "traceTremolos" << " : " <<
      booleanAsString (fTraceTremolos) <<
      endl <<

    // chords
    idtr <<
      setw(fieldWidth) << "traceChords" << " : " <<
      booleanAsString (fTraceChords) <<
      endl <<

    // tuplets
    idtr <<
      setw(fieldWidth) << "traceTuplets" << " : " <<
      booleanAsString (fTraceTuplets) <<
      endl <<

    // grace notes
    idtr <<
      setw(fieldWidth) << "traceGraceNotes" << " : " <<
      booleanAsString (fTraceGraceNotes) <<
      endl <<

    // lyrics
    idtr <<
      setw(fieldWidth) << "traceLyrics" << " : " <<
      booleanAsString (fTraceLyrics) <<
      endl <<
      
    // harmonies
    idtr <<
      setw(fieldWidth) << "traceHarmonies" << " : " <<
      booleanAsString (fTraceHarmonies) <<
      endl <<
      
    // figured bass
    idtr <<
      setw(fieldWidth) << "traceFiguredBass" << " : " <<
      booleanAsString (fTraceFiguredBass) <<
      endl <<
      
    // credits
    idtr <<
      setw(fieldWidth) << "traceCredits" << " : " <<
      booleanAsString (fTraceCredits) <<
      endl;

  idtr--;

  idtr--;
}

//______________________________________________________________________________
void initializeGeneralOptions ()
{  
  gGeneralOptionsUserChoices = generalOptions::create ();
  assert(gGeneralOptionsUserChoices != 0);

  gGeneralOptions =
    gGeneralOptionsUserChoices;

  gGeneralOptionsWithDetailedTrace =
    gGeneralOptions->
      createCloneWithDetailedTrace ();
}


}
