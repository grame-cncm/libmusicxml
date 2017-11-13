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
#include "exports.h"


namespace MusicXML2 
{

//______________________________________________________________________________
class generalOptions : public optionsGroup
{
  public:

    static SMARTP<generalOptions> create (
      S_optionsHandler optionsHandler);
        
    SMARTP<generalOptions>        createCloneWithDetailedTrace ();
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

    generalOptions (
      S_optionsHandler optionsHandler);
  
    virtual ~generalOptions ();

  public:

    // consistency check
    // ------------------------------------------------------

    void                  checkGeneralOptionsConsistency () const;

  public:

    // command line
    // --------------------------------------

    string                fProgramName;
    
    string                fCommandLineWithLongOptions;
    string                fCommandLineWithShortOptions;


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
    
    bool                  fDisplayOptionsValues;
    
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

    // score
    bool                  fTraceScore; // JMI ???

    // part groups
    bool                  fTracePartGroups;
    bool                  fTracePartGroupsDetails;

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

    // dynamics
    bool                  fTraceDynamics;

    // slurs
    bool                  fTraceSlurs;

    // ligatures
    bool                  fTraceLigatures;

    // wedges
    bool                  fTraceWedges;

    // staff tuning
    bool                  fTraceStaffTuning;
    
    // harp pedals tuning
    bool                  fTraceHarpPedalsTuning;

    // exit after some passes
    bool                  fExit2a;
    bool                  fExit2b;
    bool                  fExit3;
    
    /* STUFF not yet handled JMI */
    bool                  fTraceMidi;
};
typedef SMARTP<generalOptions> S_generalOptions;
EXP ostream& operator<< (ostream& os, const S_generalOptions& elt);

EXP extern S_generalOptions gGeneralOptions;
EXP extern S_generalOptions gGeneralOptionsUserChoices;
EXP extern S_generalOptions gGeneralOptionsWithDetailedTrace;

//______________________________________________________________________________
void initializeGeneralOptionsHandling (
  S_optionsHandler optionsHandler);


}


#endif
