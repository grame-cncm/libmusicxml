/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

/*
#include <iostream>
#include <list>
#include <algorithm>
*/
#include <iomanip>      // setw, set::precision, ...

#include "msr.h"

using namespace std;

namespace MusicXML2 
{

//_______________________________________________________________________________
S_msrMusicXMLOptions gMusicXMLOptions;
S_msrMusicXMLOptions gMusicXMLOptionsUserChoices;
S_msrMusicXMLOptions gMusicXMLOptionsWithDetailedTrace;

S_msrMusicXMLOptions msrMusicXMLOptions::create ()
{
  msrMusicXMLOptions* o = new msrMusicXMLOptions();
  assert(o!=0);
  return o;
}

msrMusicXMLOptions::msrMusicXMLOptions ()
{
  initializeMusicXMLOptions (false);
}

msrMusicXMLOptions::~msrMusicXMLOptions ()
{}

void msrMusicXMLOptions::initializeMusicXMLOptions (
  bool boolOptionsInitialValue)
{
  // trace and display
  // --------------------------------------

  fTraceMusicXMLTreeVisitors = boolOptionsInitialValue;

  // other
  // --------------------------------------

  fIgnoreMusicXMLErrors = boolOptionsInitialValue;
  fLoopToMusicXML = boolOptionsInitialValue;
}

S_msrMusicXMLOptions msrMusicXMLOptions::createCloneWithDetailedTrace ()
{
  S_msrMusicXMLOptions
    clone =
      msrMusicXMLOptions::create ();

  // trace and display
  // --------------------------------------

  clone->fTraceMusicXMLTreeVisitors =
    true;

  // other
  // --------------------------------------

  clone->fIgnoreMusicXMLErrors =
    fIgnoreMusicXMLErrors;
    
  clone->fLoopToMusicXML =
    fLoopToMusicXML;

  return clone;
}  

void msrMusicXMLOptions::printMusicXMLOptionsHelp ()
{
  idtr++;

  cerr <<
    idtr << "MusicXML:" <<
    endl <<
    idtr << "--------" <<
    endl <<
    endl;

  idtr++;

  // trace and display
  // --------------------------------------
  
  cerr <<
    idtr << "Trace and display:" <<
    endl <<
    endl;

  idtr++;

  cerr <<
    idtr <<
      "--" _TRACE_MUSICXML_TREE_VISITORS_SHORT_NAME_ ", --" _TRACE_MUSICXML_TREE_VISITORS_LONG_NAME_ <<
      endl <<
    idtr << tab << tab << tab <<
      "Write a trace of the MusicXML tree visiting activity to standard error." <<
      endl <<
    endl;

  idtr--;
      
  // other
  // --------------------------------------
  
  cerr <<
    idtr << "Other:" <<
    endl <<
    endl;

  idtr++;

  cerr <<
    idtr <<
      "--" _INGORE_MUSICXML_ERRORS_SHORT_NAME_ ", --" _INGORE_MUSICXML_ERRORS_LONG_NAME_ <<
      endl <<
    idtr << tab << tab << tab <<
      "Don't stop the translation after issuing a MusicXML error message." <<
      endl <<
    endl <<
      
    idtr <<
      "--" _LOOP_TO_MUSICXML_SHORT_NAME_ ", --" _LOOP_TO_MUSICXML_LONG_NAME_ <<
      endl <<
    idtr << tab << tab << tab <<
      "Close the loop, generating a MusicXML file from the MSR. " << endl <<
    idtr << tab << tab << tab <<
      "The file name receives a '_loop' suffix. Currently under development." <<
      endl <<
    endl;

  idtr--;
  
  idtr--;

  
  idtr--;
}

void msrMusicXMLOptions::printMusicXMLOptionsValues (int fieldWidth)
{  
  cerr << idtr <<
    "The MusicXML options are:" <<
    endl;

  idtr++;

  // trace and display
  // --------------------------------------
  
  cerr <<
    idtr << "Trace and display:" <<
    endl;

  idtr++;

  cerr <<
    idtr <<
      setw(fieldWidth) << "traceMusicXMLTreeVisitors" << " : " <<
      booleanAsString (fTraceMusicXMLTreeVisitors) <<
      endl;

  idtr--;
      
  // other
  // --------------------------------------

  cerr <<
    idtr << "Other:" <<
    endl;

  idtr++;

  cerr <<
    idtr <<
      setw(fieldWidth) << "ignoreMusicXMLErrors" << " : " <<
      booleanAsString (fIgnoreMusicXMLErrors) <<
      endl <<
      
    idtr <<
      setw(fieldWidth) << "loopToMusicXML" << " : " <<
      booleanAsString (fLoopToMusicXML) <<
      endl;

  idtr--;


  idtr--;
}

//_______________________________________________________________________________
S_msrGeneralOptions gGeneralOptions;
S_msrGeneralOptions gGeneralOptionsUserChoices;
S_msrGeneralOptions gGeneralOptionsWithDetailedTrace;

S_msrGeneralOptions msrGeneralOptions::create ()
{
  msrGeneralOptions* o = new msrGeneralOptions();
  assert(o!=0);
  return o;
}

msrGeneralOptions::msrGeneralOptions ()
{
  initializeGeneralOptions (false);
}

msrGeneralOptions::~msrGeneralOptions ()
{}

void msrGeneralOptions::initializeGeneralOptions (
  bool boolOptionsInitialValue)
{
  // output file
  // --------------------------------------

  fAutoOutputFile = false;
  
  // trace and display
  // --------------------------------------

  fTraceGeneral = boolOptionsInitialValue;

  fTraceDetailed = false;

  // CPU usage
  // --------------------------------------

  fDisplayCPUusage = boolOptionsInitialValue;
    
  // specific trace    
  // --------------------------------------

  // divisions
  fTraceDivisions = boolOptionsInitialValue;

  // part groups
  fTracePartGroups = boolOptionsInitialValue;
  // parts
  fTraceParts = boolOptionsInitialValue;
  // staves
  fTraceStaves = boolOptionsInitialValue;
  // voices
  fTraceVoices = boolOptionsInitialValue;

  // clefs
  fTraceClefs = boolOptionsInitialValue;
  // keys
  fTraceKeys  = boolOptionsInitialValue;
  // times
  fTraceTimes = boolOptionsInitialValue;

  // segments
  fTraceSegments = boolOptionsInitialValue;
  
  // repeats
  fTraceRepeats = boolOptionsInitialValue;

  // measures
  fTraceMeasures = boolOptionsInitialValue;

  // notes
  fTraceNotes = boolOptionsInitialValue;
  
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

S_msrGeneralOptions msrGeneralOptions::createCloneWithDetailedTrace ()
{
  S_msrGeneralOptions
    clone =
      msrGeneralOptions::create ();

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
  
  // output file
  // --------------------------------------

  clone->fAutoOutputFile =
    fAutoOutputFile;
  
  // trace and display
  // --------------------------------------

  clone->fTraceGeneral =
    true;
    
  clone->fTraceDetailed =
    true;

  clone->fTraceDetailedMeasureNumbersSet =
    fTraceDetailedMeasureNumbersSet;

  // CPU usage
  // --------------------------------------

  clone->fDisplayCPUusage =
    true;
    
  // specific trace
  // --------------------------------------

  // divisions
  clone->fTraceDivisions =
    true;

  // part groups
  clone->fTracePartGroups =
    true;
  // parts
  clone->fTraceParts =
    true;
  // staves
  clone->fTraceStaves =
    true;
  // voices
  clone->fTraceVoices =
    true;

  // clefs
  clone->fTraceClefs =
    true;
  // keys
  clone->fTraceKeys =
    true;
  // times
  clone->fTraceTimes =
    true;

  // segments
  clone->fTraceSegments =
    true;
    
  // repeats
  clone->fTraceRepeats =
    true;

  // measures
  clone->fTraceMeasures =
    true;
  
  // notes
  clone->fTraceNotes =
    true;
    
  // technicals
  clone->fTraceTechnicals =
    true;
  
  // words
  clone->fTraceWords =
    true;
  
  // tremolos
  clone->fTraceTremolos =
    true;

  // chords
  clone->fTraceChords =
    true;
    
  // tuplets
  clone->fTraceTuplets =
    true;
  
  // grace notes
  clone->fTraceGraceNotes =
    true;

  // lyrics
  clone->fTraceLyrics =
    true;
  
  // harmonies
  clone->fTraceHarmonies =
    true;
  
    
  /* STUFF not yet handled JMI */

  clone->fTraceScore =
    true;


  clone->fTraceDynamics =
    true;

  clone->fTraceSlurs =
    true;

  clone->fTraceLigatures =
    true;

  clone->fTraceWedges =
    true;
  
  clone->fTraceStaffTuning =
    true;
  
  clone->fTraceMidi =
    true;

  return clone;
}

void msrGeneralOptions::printGeneralOptionsHelp ()
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
      
    // parts
    idtr <<
      "--" _TRACE_PARTS_SHORT_NAME_ ", --" _TRACE_PARTS_LONG_NAME_ <<
      endl <<
    idtr << tab << tab << tab <<
      "parts" <<
      endl <<

    // staves
    idtr <<
      "--" _TRACE_STAVES_SHORT_NAME_ ", --" _TRACE_STAVES_LONG_NAME_ <<
      endl <<
    idtr << tab << tab << tab <<
      "staves" <<
      endl <<
      
    // voices
    idtr <<
      "--" _TRACE_VOICES_SHORT_NAME_ ", --" _TRACE_VOICES_LONG_NAME_ <<
      endl <<
    idtr << tab << tab << tab <<
      "voices" <<
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
      "notes (quite verbose)" <<
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
    endl;
      
  idtr--;
  
  idtr--;
  

  idtr--;
}

void msrGeneralOptions::printGeneralOptionsValues (int fieldWidth)
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
      endl;

  idtr--;

  idtr--;
}


}
