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
  fAutoOutputFile = false;
  
  // trace
  fTraceGeneral = boolOptionsInitialValue;
    
  fTraceDivisions = boolOptionsInitialValue;

  fTraceScore = boolOptionsInitialValue;

  fTracePartgroups = boolOptionsInitialValue;
  fTraceParts = boolOptionsInitialValue;
  fTraceStaves = boolOptionsInitialValue;
  fTraceVoices = boolOptionsInitialValue;

  fTraceSegments = boolOptionsInitialValue;
  fTraceMeasures = boolOptionsInitialValue;
  
  fTraceChords = boolOptionsInitialValue;
  fTraceTuplets = boolOptionsInitialValue;
  
  fTraceGracenotes = boolOptionsInitialValue;

  fTraceHarmonies = boolOptionsInitialValue;
  

    
  fTraceNotes = boolOptionsInitialValue;
  
  fTraceDynamics = boolOptionsInitialValue;
  fTraceWords = boolOptionsInitialValue;
  fTraceSlurs = boolOptionsInitialValue;
  fTraceLigatures = boolOptionsInitialValue;
  fTraceWedges = boolOptionsInitialValue;
  
  
  fTraceLyrics = boolOptionsInitialValue;
  
  fTraceWords = boolOptionsInitialValue;

  fTraceRepeats = boolOptionsInitialValue;
  
  fTraceStafftuning = boolOptionsInitialValue;
  
  fTraceMidi = boolOptionsInitialValue;

  fTraceALL = boolOptionsInitialValue;

  // timing information
  fDisplayCPUusage = boolOptionsInitialValue;

  // measure number-selective debug
  fSaveDebug = boolOptionsInitialValue;
  fSaveDebugDebug = boolOptionsInitialValue;
}

void msrGeneralOptions::printGeneralOptionsHelp ()
{
 cerr <<
    "  General:" << endl <<
    endl <<

    "    --h, --help" << endl <<
    "          Display this help and exit." << endl <<
    "    --v, --version" << endl <<
    "          Display xml2lilypond's version number and exit." << endl <<
    endl <<

    "    --of, --outputFile fileName" << endl <<
    "          Write LilyPond code to file 'fileName' instead of standard output." << endl <<
    "    --aof, --autoOutputFile" << endl <<
    "          This option can only be used when reading from a file." << endl <<
    "          Write LilyPond code to a file whose name is derived from that of the input file," << endl <<
    "          replacing any suffix after the the '.' by 'ly' or adding '.ly' if none is present." << endl <<
    endl <<

    "    --t, --traceGeneral" << endl <<
    "          Write a trace of the general activity to standard error." << endl <<
    endl <<
    "    --tdivs, --traceDivisions " << endl <<
    "          Write a trace of the activity regarding divisions" << endl <<
    "          to standard error." << endl <<
    endl <<
    "    --tparts, --traceparts " << endl <<
    "          Write a trace of the activity regarding score, part groups and parts" << endl <<
    "          to standard error." << endl <<
    "    --tvoices, --traceVoices " << endl <<
    "          Write a trace of the activity regarding staves and voices" << endl <<
    "          to standard error." << endl <<
    "    --tsegs, --traceSegments " << endl <<
    "          Write a trace of the activity regarding voices' segments" << endl <<
    "          to standard error." << endl <<
    "    --trepeats, --traceRepeats " << endl <<
    "          Write a trace of the activity regarding repeats" << endl <<
    "          to standard error." << endl <<
    "    --tmeas, --traceMeasures " << endl <<
    "          Write a trace of the activity regarding segments' measures" << endl <<
    "          to standard error." << endl <<
    "    --tnotes, --traceNotes " << endl <<
    "          Write a trace of the activity regarding notes" << endl <<
    "          to standard error." << endl <<
    "    --tchords, --traceChords " << endl <<
    "          Write a trace of the activity regarding chords" << endl <<
    "          to standard error." << endl <<
    "    --ttuplets, --traceTuplets " << endl <<
    "          Write a trace of the activity regarding tuplets" << endl <<
    "          to standard error." << endl <<
    "    --tgrace, --traceGracenotes " << endl <<
    "          Write a trace of the activity regarding grace notes" << endl <<
    "          to standard error." << endl <<
    endl <<
    "    --tlyrics, --traceLyrics " << endl <<
    "          Write at trace of the activity regarding lyrics" << endl <<
    "          to standard error." << endl <<
    endl <<
    "    --tharm, --traceHarmonies " << endl <<
    "          Write at trace of the activity regarding harmonies" << endl <<
    "          to standard error." << endl <<
    endl <<
    "    --dm, --debugMeasures measureNumbersSet" << endl <<
    "          'measureNumbersSet' has a form such as '0,2-14,^8-10'," << endl <<
    "          where '^' excludes the corresponding numbers interval" << endl <<
    "          and 0 applies to the '<part-list>' and anacrusis if present." <<endl <<
    "          Generate a trace of the activity and print additional" << endl <<
    "          debugging information to standard error for the specified measures." << endl <<
    "    --ddm, --debugDebugMeasures <measuresSpec>" << endl <<
    "          Same as above, but print even more debugging information." << endl <<
    endl <<
    "    --dCPU, --displayCPUusage" << endl <<
    "          Write CPU usage by the passes at the end of the translation." << endl <<
    endl;
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
      endl <<
        
    indenter::gIndenter <<
      setw(fieldWidth) << "traceDivisions" << " : " <<
      booleanAsString (gGeneralOptions->fTraceDivisions) <<
      endl <<
      
    indenter::gIndenter <<
      setw(fieldWidth) << "traceparts" << " : " <<
      booleanAsString (gGeneralOptions->fTraceParts) <<
      endl <<
    indenter::gIndenter <<
      setw(fieldWidth) << "traceVoices" << " : " <<
      booleanAsString (gGeneralOptions->fTraceVoices) <<
      endl <<
    indenter::gIndenter <<
      setw(fieldWidth) << "traceSegments" << " : " <<
      booleanAsString (gGeneralOptions->fTraceSegments) <<
      endl <<
    indenter::gIndenter <<
      setw(fieldWidth) << "traceRepeats" << " : " <<
      booleanAsString (gGeneralOptions->fTraceRepeats) <<
      endl <<
    indenter::gIndenter <<
      setw(fieldWidth) << "traceMeasures" << " : " <<
      booleanAsString (gGeneralOptions->fTraceMeasures) <<
      endl <<
    indenter::gIndenter <<
      setw(fieldWidth) << "traceNotes" << " : " <<
      booleanAsString (gGeneralOptions->fTraceNotes) <<
      endl <<
    indenter::gIndenter <<
      setw(fieldWidth) << "traceChords" << " : " <<
      booleanAsString (gGeneralOptions->fTraceChords) <<
      endl <<
    indenter::gIndenter <<
      setw(fieldWidth) << "traceTuplets" << " : " <<
      booleanAsString (gGeneralOptions->fTraceTuplets) <<
      endl <<
    indenter::gIndenter <<
      setw(fieldWidth) << "traceGracenotes" << " : " <<
      booleanAsString (gGeneralOptions->fTraceGracenotes) <<
      endl <<

    indenter::gIndenter <<
      setw(fieldWidth) << "traceLyrics" << " : " <<
      booleanAsString (gGeneralOptions->fTraceLyrics) <<
      endl <<
      
    indenter::gIndenter <<
      setw(fieldWidth) << "traceHarmonies" << " : " <<
      booleanAsString (gGeneralOptions->fTraceHarmonies) <<
      endl <<
    indenter::gIndenter <<
      setw(fieldWidth) << "debugMeasureNumbersSet" << " : ";

  if (gGeneralOptions->fDebugMeasureNumbersSet.empty ())
    cerr << "none";
  else
    for (
      set<int>::const_iterator i =
        gGeneralOptions->fDebugMeasureNumbersSet.begin();
      i != gGeneralOptions->fDebugMeasureNumbersSet.end();
      i++) {
        cerr << (*i) << " ";
    } // for
  cerr << endl;

  cerr <<
    indenter::gIndenter <<
      setw(fieldWidth) << "displayCPUusage" << " : " <<
      booleanAsString (gGeneralOptions->fDisplayCPUusage) <<
      endl;

  indenter::gIndenter--;
}


}
