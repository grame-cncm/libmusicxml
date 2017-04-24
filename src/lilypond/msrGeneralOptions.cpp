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
  
  fTraceWords = boolOptionsInitialValue;

  
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
      "--v, --version" <<
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
      "--h, --help" <<
      endl <<
    idtr << tab << tab << tab <<
      "Display this help and exit." <<
      endl <<
    endl <<
      
    idtr <<
      "--hg, --helpGeneral" <<
      endl <<
    idtr << tab << tab << tab <<
      "Display general help and exit." <<
      endl <<
    idtr <<
      "--hm, --helpMsr" <<
      endl <<
    idtr << tab << tab << tab <<
      "Display MSR help and exit." <<
      endl <<
    idtr <<
      "--hlp, --helpLpsr" <<
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
      "--of, --outputFile fileName" <<
      endl <<
    idtr << tab << tab << tab <<
      "Write LilyPond code to file 'fileName' instead of standard output." <<
      endl <<
    endl <<
    
    idtr <<
      "--aof, --autoOutputFile" <<
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
      "--t, --traceGeneral" <<
      endl <<
    idtr << tab << tab << tab <<
      "Write a trace of the general activity to standard error." <<
      endl <<
    endl <<
      
    idtr <<
      "--td, --traceDetailed measureNumbersSet" <<
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

  // CPU usage
  // --------------------------------------
  cerr <<
    idtr << "CPU usage:" <<
    endl <<
    endl;

  idtr++;

  cerr <<
    idtr <<
      "--dCPU, --displayCPUusage" <<
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
      "--tdivs, --traceDivisions " <<
      endl <<
    idtr << tab << tab << tab <<
      " divisions" <<
      endl <<
    endl <<
      
    idtr <<
      "--tparts, --traceparts " <<
      endl <<
    idtr << tab << tab << tab <<
      "score, part groups and parts" <<
      endl <<
    idtr <<
      "--tvoices, --traceVoices " <<
      endl <<
    idtr << tab << tab << tab <<
      "staves and voices" <<
      endl <<
    endl <<
      
    idtr <<
      "--tsegs, --traceSegments " <<
      endl <<
    idtr << tab << tab << tab <<
      "voices' segments" <<
      endl <<
    endl <<
      
    idtr <<
      "--trepeats, --traceRepeats " <<
      endl <<
    idtr << tab << tab << tab <<
      "repeats" <<
      endl <<
    endl <<
      
    idtr <<
      "--tmeas, --traceMeasures " <<
      endl <<
    idtr << tab << tab << tab <<
      "segments' measures" <<
      endl <<
    endl <<

    idtr <<
      "--tnotes, --traceNotes " <<
      endl <<
    idtr << tab << tab << tab <<
      "notes" <<
      endl <<
    idtr <<
      "--ttrems, --traceTremolos " <<
      endl <<
    idtr << tab << tab << tab <<
      "tremolos" <<
      endl <<
    endl <<

    idtr <<
      "--tchords, --traceChords " <<
      endl <<
    idtr << tab << tab << tab <<
      "chords" <<
      endl <<
    idtr <<
      "--ttuplets, --traceTuplets " <<
      endl <<
    idtr << tab << tab << tab <<
      "tuplets" <<
      endl <<
    endl <<

    idtr <<
      "--tgrace, --traceGracenotes " <<
      endl <<
    idtr << tab << tab << tab <<
      "grace notes" <<
      endl <<
    endl <<

    idtr <<
      "--tlyrics, --traceLyrics " <<
      endl <<
    idtr << tab << tab << tab <<
      "lyrics in MusicXML, stanza in MSR" <<
      endl <<
    endl <<

    idtr <<
      "--tharm, --traceHarmonies " <<
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
  cerr << indenter::gIndenter <<
    "The general options are:" <<
    endl;

  indenter::gIndenter++;

  cerr << left <<
    indenter::gIndenter <<
      setw(fieldWidth) << "input source name" << " : " <<
      gGeneralOptions->fInputSourceName <<
      endl <<
      
    indenter::gIndenter <<
      setw(fieldWidth) << "translation date" << " : " <<
      gGeneralOptions->fTranslationDate <<
      endl <<
        
    indenter::gIndenter <<
      setw(fieldWidth) << "outputFileName" << " : \"" <<
      gGeneralOptions->fOutputFileName <<
      "\"" <<
      endl <<
    indenter::gIndenter <<
      setw(fieldWidth) << "autoOutputFile" << " : " <<
      booleanAsString (gGeneralOptions->fAutoOutputFile) <<
      endl <<
        
    indenter::gIndenter <<
      setw(fieldWidth) << "traceGeneral" << " : " <<
      booleanAsString (gGeneralOptions->fTraceGeneral) <<
      endl;

  cerr << left <<
    indenter::gIndenter <<
      setw(fieldWidth) << "Specific tracing:" <<
      endl;

  idtr++;

  const int indentedFieldWidth = fieldWidth - 2;
  
  cerr << left <<
    indenter::gIndenter <<
      setw(indentedFieldWidth) << "traceDivisions" << " : " <<
      booleanAsString (gGeneralOptions->fTraceDivisions) <<
      endl <<
        
    indenter::gIndenter <<
      setw(indentedFieldWidth) << "traceparts" << " : " <<
      booleanAsString (gGeneralOptions->fTraceParts) <<
      endl <<
    indenter::gIndenter <<
      setw(indentedFieldWidth) << "traceVoices" << " : " <<
      booleanAsString (gGeneralOptions->fTraceVoices) <<
      endl <<
    indenter::gIndenter <<
      setw(indentedFieldWidth) << "traceSegments" << " : " <<
      booleanAsString (gGeneralOptions->fTraceSegments) <<
      endl <<
    indenter::gIndenter <<
      setw(indentedFieldWidth) << "traceRepeats" << " : " <<
      booleanAsString (gGeneralOptions->fTraceRepeats) <<
      endl <<
    indenter::gIndenter <<
      setw(indentedFieldWidth) << "traceMeasures" << " : " <<
      booleanAsString (gGeneralOptions->fTraceMeasures) <<
      endl <<
    indenter::gIndenter <<
      setw(indentedFieldWidth) << "traceNotes" << " : " <<
      booleanAsString (gGeneralOptions->fTraceNotes) <<
      endl <<
    indenter::gIndenter <<
      setw(indentedFieldWidth) << "traceTremolos" << " : " <<
      booleanAsString (gGeneralOptions->fTraceTremolos) <<
      endl <<
    indenter::gIndenter <<
      setw(indentedFieldWidth) << "traceChords" << " : " <<
      booleanAsString (gGeneralOptions->fTraceChords) <<
      endl <<
    indenter::gIndenter <<
      setw(indentedFieldWidth) << "traceTuplets" << " : " <<
      booleanAsString (gGeneralOptions->fTraceTuplets) <<
      endl <<
    indenter::gIndenter <<
      setw(indentedFieldWidth) << "traceGracenotes" << " : " <<
      booleanAsString (gGeneralOptions->fTraceGracenotes) <<
      endl <<

    indenter::gIndenter <<
      setw(indentedFieldWidth) << "traceLyrics" << " : " <<
      booleanAsString (gGeneralOptions->fTraceLyrics) <<
      endl <<
      
    indenter::gIndenter <<
      setw(indentedFieldWidth) << "traceHarmonies" << " : " <<
      booleanAsString (gGeneralOptions->fTraceHarmonies) <<
      endl <<
    indenter::gIndenter <<
      setw(indentedFieldWidth) << "traceDetailed" << " : " <<
      booleanAsString (gGeneralOptions->fTraceDetailed) <<
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

  // CPU usage
  // --------------------------------------  

  cerr <<
    indenter::gIndenter <<
      setw(fieldWidth) << "displayCPUusage" << " : " <<
      booleanAsString (gGeneralOptions->fDisplayCPUusage) <<
      endl;

  indenter::gIndenter--;
}


}
