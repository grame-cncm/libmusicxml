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

  fTraceParts = boolOptionsInitialValue;
  fTraceVoices = boolOptionsInitialValue;

  fTraceSegments = boolOptionsInitialValue;
  fTraceMeasures = boolOptionsInitialValue;
  
  fTraceChords = boolOptionsInitialValue;
  fTraceTuplets = boolOptionsInitialValue;
  
  fTraceGracenotes = boolOptionsInitialValue;

  fTraceHarmonies = boolOptionsInitialValue;
  

  fTraceScore = boolOptionsInitialValue;
  fTracePartgroups = boolOptionsInitialValue;
  fTraceStaves = boolOptionsInitialValue;
    
  fTraceNotes = boolOptionsInitialValue;
  fTraceDurations = boolOptionsInitialValue;
  
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

void msrGeneralOptions::printGeneralOptions (int fieldWidth)
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
