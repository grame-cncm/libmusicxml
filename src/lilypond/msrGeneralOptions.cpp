/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#include <iostream>
#include <list>
#include <algorithm>
#include <iomanip>      // setw, set::precision, ...

#include "msr.h"

using namespace std;

namespace MusicXML2 
{

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

msrGeneralOptions::~msrGeneralOptions () {}

void msrGeneralOptions::initializeGeneralOptions (
  bool boolOptionsInitialValue)
{
  // output file

  fAutoOutputFile = false;
  
  // trace and display

  fTraceGeneral = boolOptionsInitialValue;

  fTraceDetailed = false;

  // MusicXML

  fIgnoreMusicXMLErrors = boolOptionsInitialValue;
  
  // CPU usage
  
  fDisplayCPUusage = boolOptionsInitialValue;
    
  // specific trace    

  fTraceDivisions = boolOptionsInitialValue;

  fTraceParts = boolOptionsInitialValue;
  fTraceVoices = boolOptionsInitialValue;

  fTraceSegments = boolOptionsInitialValue;
  
  fTraceRepeats = boolOptionsInitialValue;

  fTraceMeasures = boolOptionsInitialValue;

  fTraceNotes = boolOptionsInitialValue;
  
  fTraceTremolos = boolOptionsInitialValue;
  
  fTraceChords = boolOptionsInitialValue;
  fTraceTuplets = boolOptionsInitialValue;
  
  fTraceGracenotes = boolOptionsInitialValue;

  fTraceLyrics = boolOptionsInitialValue;

  fTraceHarmonies = boolOptionsInitialValue;


  /* STUFF not yet handled JMI */

  fTraceScore = boolOptionsInitialValue;

  fTracePartgroups = boolOptionsInitialValue;
  fTraceStaves = boolOptionsInitialValue;

  fTraceDynamics = boolOptionsInitialValue;
  fTraceWords = boolOptionsInitialValue;
  fTraceSlurs = boolOptionsInitialValue;
  fTraceLigatures = boolOptionsInitialValue;
  fTraceWedges = boolOptionsInitialValue;
  
  fTraceStafftuning = boolOptionsInitialValue;
  
  fTraceMidi = boolOptionsInitialValue;
}

S_msrGeneralOptions msrGeneralOptions::createCloneWithDetailedTrace ()
{
  S_msrGeneralOptions
    clone =
      msrGeneralOptions::create ();

  // output file

  clone->fAutoOutputFile =
    fAutoOutputFile;
  
  // trace and display

  clone->fTraceGeneral =
    true;
    
  clone->fTraceDetailed =
    true;

  // MusicXML

  clone->fIgnoreMusicXMLErrors =
    true;
  
  // CPU usage

  clone->fDisplayCPUusage =
    true;
    
  // specific trace

  clone->fTraceDivisions =
    true;

  clone->fTraceParts =
    true;
  clone->fTraceVoices =
    true;

  clone->fTraceSegments =
    true;
    
  clone->fTraceRepeats =
    true;

  clone->fTraceMeasures =
    true;
  
  clone->fTraceNotes =
    true;
  
  clone->fTraceTremolos =
    true;

  clone->fTraceChords =
    true;
  clone->fTraceTuplets =
    true;
  
  clone->fTraceGracenotes =
    true;

  clone->fTraceLyrics =
    true;
  
  clone->fTraceHarmonies =
    true;
  
    
  /* STUFF not yet handled JMI */

  clone->fTraceScore =
    true;

  clone->fTracePartgroups =
    true;
  clone->fTraceStaves =
    true;

  clone->fTraceDynamics =
    true;
  clone->fTraceWords =
    true;
  clone->fTraceSlurs =
    true;
  clone->fTraceLigatures =
    true;
  clone->fTraceWedges =
    true;
  
  clone->fTraceStafftuning =
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
      "Write LilyPond code to a file whose name is derived from that of the input file," <<
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
      "and 0 applies to the '<part-list>' and anacrusis if present." <<endl <<
    idtr << tab << tab << tab <<
      "Generate a detailed trace of the activity and print additional" <<
      endl <<
    idtr << tab << tab << tab <<
      "debugging information to standard error for the specified measures." <<
      endl <<
    endl;

  idtr--;

  // MusicXML
  // --------------------------------------

  cerr <<
    idtr << "MusicXML:" <<
    endl <<
    endl;

  idtr++;

  cerr <<
    idtr <<
      "--" _INGORE_MUSICXML_ERRORS_SHORT_NAME_ ", --" _INGORE_MUSICXML_ERRORS_LONG_NAME_ <<
      endl <<
    idtr << tab << tab << tab <<
      "Don't stop the translation after issuing a MusicXML error message'." <<
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
    idtr <<
      "--" _TRACE_DIVISIONS_SHORT_NAME_ ", --" _TRACE_DIVISIONS_LONG_NAME_ <<
      endl <<
    idtr << tab << tab << tab <<
      " divisions" <<
      endl <<
    endl <<
      
    idtr <<
      "--" _TRACE_PARTS_SHORT_NAME_ ", --" _TRACE_PARTS_LONG_NAME_ <<
      endl <<
    idtr << tab << tab << tab <<
      "score, part groups and parts" <<
      endl <<
    idtr <<
      "--" _TRACE_VOICES_SHORT_NAME_ ", --" _TRACE_VOICES_LONG_NAME_ <<
      endl <<
    idtr << tab << tab << tab <<
      "staves and voices" <<
      endl <<
    endl <<
      
    idtr <<
      "--" _TRACE_SEGMENTS_SHORT_NAME_ ", --" _TRACE_SEGMENTS_LONG_NAME_ <<
      endl <<
    idtr << tab << tab << tab <<
      "voices' segments" <<
      endl <<
    endl <<
      
    idtr <<
      "--" _TRACE_REPEATS_SHORT_NAME_ ", --" _TRACE_REPEATS_LONG_NAME_ <<
      endl <<
    idtr << tab << tab << tab <<
      "repeats" <<
      endl <<
    endl <<
      
    idtr <<
      "--" _TRACE_MEASURES_SHORT_NAME_ ", --" _TRACE_MEASURES_LONG_NAME_ <<
      endl <<
    idtr << tab << tab << tab <<
      "segments' measures" <<
      endl <<
    endl <<

    idtr <<
      "--" _TRACE_NOTES_SHORT_NAME_ ", --" _TRACE_NOTES_LONG_NAME_ <<
      endl <<
    idtr << tab << tab << tab <<
      "notes" <<
      endl <<
    idtr <<
      "--" _TRACE_TREMOLOS_SHORT_NAME_ ", --" _TRACE_TREMOLOS_LONG_NAME_ <<
      endl <<
    idtr << tab << tab << tab <<
      "tremolos" <<
      endl <<
    endl <<

    idtr <<
      "--" _TRACE_CHORDS_SHORT_NAME_ ", --" _TRACE_CHORDS_LONG_NAME_ <<
      endl <<
    idtr << tab << tab << tab <<
      "chords" <<
      endl <<
    idtr <<
      "--" _TRACE_TUPLETS_SHORT_NAME_ ", --" _TRACE_TUPLETS_LONG_NAME_ <<
      endl <<
    idtr << tab << tab << tab <<
      "tuplets" <<
      endl <<
    endl <<

    idtr <<
      "--" _TRACE_GRACENOTES_SHORT_NAME_ ", --" _TRACE_GRACENOTES_LONG_NAME_ <<
      endl <<
    idtr << tab << tab << tab <<
      "grace notes" <<
      endl <<
    endl <<

    idtr <<
      "--" _TRACE_LYRICS_SHORT_NAME_ ", --" _TRACE_LYRICS_LONG_NAME_ <<
      endl <<
    idtr << tab << tab << tab <<
      "lyrics in MusicXML, stanza in MSR" <<
      endl <<
    endl <<

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
  
  cerr << left <<
    idtr <<
      setw(fieldWidth) << "Command line:" <<
      endl;

  idtr++;

  cerr << left <<
    idtr <<
      setw(fieldWidth) << "input source name" << " : " <<
      gGeneralOptions->fInputSourceName <<
      endl <<
      
    idtr <<
      setw(fieldWidth) << "translation date" << " : " <<
      gGeneralOptions->fTranslationDate <<
      endl;

  idtr--;

  // output file

  cerr << left <<
    idtr <<
      setw(fieldWidth) << "Output file:" <<
      endl;

  idtr++;

  cerr << left <<        
    idtr <<
      setw(fieldWidth) << "outputFileName" << " : \"" <<
      gGeneralOptions->fOutputFileName <<
      "\"" <<
      endl <<
    idtr <<
      setw(fieldWidth) << "autoOutputFile" << " : " <<
      booleanAsString (gGeneralOptions->fAutoOutputFile) <<
      endl;

  idtr--;

  // trace and display

  cerr << left <<
    idtr <<
      setw(fieldWidth) << "Trace and display:" <<
      endl;

  idtr++;

  cerr << left <<
    idtr <<
      setw(fieldWidth) << "traceGeneral" << " : " <<
      booleanAsString (gGeneralOptions->fTraceGeneral) <<
       endl <<
       
    idtr <<
      setw(fieldWidth) << "traceDetailed" << " : " <<
      booleanAsString (gGeneralOptions->fTraceDetailed) <<
     endl;
        
  idtr--;

  // MusicXML

  cerr << left <<
    idtr <<
      setw(fieldWidth) << "MusicXML:" <<
      endl;

  idtr++;

  cerr <<
    idtr <<
      setw(fieldWidth) << "ignoreMusicXMLErrors" << " : " <<
      booleanAsString (gGeneralOptions->fIgnoreMusicXMLErrors) <<
      endl;

  idtr--;
  
  // CPU usage

  cerr << left <<
    idtr <<
      setw(fieldWidth) << "CPU usage:" <<
      endl;

  idtr++;

  cerr <<
    idtr <<
      setw(fieldWidth) << "displayCPUusage" << " : " <<
      booleanAsString (gGeneralOptions->fDisplayCPUusage) <<
      endl;

  idtr--;
  
  // specific trace    

  cerr << left <<
    idtr <<
      setw(fieldWidth) << "Specific trace:" <<
      endl;

  idtr++;

  cerr << left <<
    idtr <<
      setw(fieldWidth) << "traceDivisions" << " : " <<
      booleanAsString (gGeneralOptions->fTraceDivisions) <<
      endl <<
        
    idtr <<
      setw(fieldWidth) << "traceparts" << " : " <<
      booleanAsString (gGeneralOptions->fTraceParts) <<
      endl <<
    idtr <<
      setw(fieldWidth) << "traceVoices" << " : " <<
      booleanAsString (gGeneralOptions->fTraceVoices) <<
      endl <<
    idtr <<
      setw(fieldWidth) << "traceSegments" << " : " <<
      booleanAsString (gGeneralOptions->fTraceSegments) <<
      endl <<
    idtr <<
      setw(fieldWidth) << "traceRepeats" << " : " <<
      booleanAsString (gGeneralOptions->fTraceRepeats) <<
      endl <<
    idtr <<
      setw(fieldWidth) << "traceMeasures" << " : " <<
      booleanAsString (gGeneralOptions->fTraceMeasures) <<
      endl <<
    idtr <<
      setw(fieldWidth) << "traceNotes" << " : " <<
      booleanAsString (gGeneralOptions->fTraceNotes) <<
      endl <<
    idtr <<
      setw(fieldWidth) << "traceTremolos" << " : " <<
      booleanAsString (gGeneralOptions->fTraceTremolos) <<
      endl <<
    idtr <<
      setw(fieldWidth) << "traceChords" << " : " <<
      booleanAsString (gGeneralOptions->fTraceChords) <<
      endl <<
    idtr <<
      setw(fieldWidth) << "traceTuplets" << " : " <<
      booleanAsString (gGeneralOptions->fTraceTuplets) <<
      endl <<
    idtr <<
      setw(fieldWidth) << "traceGracenotes" << " : " <<
      booleanAsString (gGeneralOptions->fTraceGracenotes) <<
      endl <<

    idtr <<
      setw(fieldWidth) << "traceLyrics" << " : " <<
      booleanAsString (gGeneralOptions->fTraceLyrics) <<
      endl <<
      
    idtr <<
      setw(fieldWidth) << "traceHarmonies" << " : " <<
      booleanAsString (gGeneralOptions->fTraceHarmonies) <<
      endl;

  if (gGeneralOptions->fTraceDetailed) {
    cerr << left <<
      "fTraceAllMeasureNumbersSet: ";
      
    if (gGeneralOptions->fTraceAllMeasureNumbersSet.empty ())
      cerr << "none";
    else
      for (
        set<int>::const_iterator i =
          gGeneralOptions->fTraceAllMeasureNumbersSet.begin();
        i != gGeneralOptions->fTraceAllMeasureNumbersSet.end();
        i++) {
          cerr << (*i) << " ";
      } // for
    cerr << endl;
  }

  idtr--;

  idtr--;
}


}
