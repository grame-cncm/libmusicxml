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
class EXP generalOptions : public msrOptionsGroup
{
  public:

    static SMARTP<generalOptions> create ();
    
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

    generalOptions();
  
    virtual ~generalOptions();

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
typedef SMARTP<generalOptions> S_generalOptions;
EXP ostream& operator<< (ostream& os, const S_generalOptions& elt);

extern S_generalOptions gGeneralOptions;
extern S_generalOptions gGeneralOptionsUserChoices;
extern S_generalOptions gGeneralOptionsWithDetailedTrace;

/*!
\brief A msr midi representation.

  A midi is represented by variable/value pairs
*/
//______________________________________________________________________________
void initializeGeneralOptions ();


/*! @} */

}


#endif
