/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef ___generalOptions___
#define ___generalOptions___

#include <set>

#include "exports.h"

#include "optionsBasicTypes.h"


namespace MusicXML2 
{

//______________________________________________________________________________
class generalOptions : public optionsGroup
{
  public:

    static SMARTP<generalOptions> create (
      S_optionsHandler optionsHandler);
        
    SMARTP<generalOptions>        createCloneWithTrueValues (); // JMI

  public:

    // initialisation
    // ------------------------------------------------------

    void                  initializeGeneralOptions (
                            bool boolOptionsInitialValue);
        
  protected:
  
    // constructors/destructor
    // ------------------------------------------------------

    generalOptions (
      S_optionsHandler optionsHandler);
  
    virtual ~generalOptions ();

  public:

    // quiet mode
    // ------------------------------------------------------

    void                  enforceQuietness ();

  public:

    // consistency check
    // ------------------------------------------------------

    virtual void          checkOptionsConsistency ();

  public:

    // set and get
    // ------------------------------------------------------

    void                  setAllGeneralTraceOptions (
                            bool boolOptionsInitialValue);

  public:

    // services
    // ------------------------------------------------------

    virtual S_optionsItem handleOptionsItem (
                            ostream&      os,
                            S_optionsItem item);

  public:

    // print
    // ------------------------------------------------------

    void                  printGeneralOptionsHelp ();

    void                  printGeneralOptionsValues (int fieldWidth);
    
  public:

    // command line
    // --------------------------------------

    string                fProgramName;
    string                fInputSourceName;
    string                fTranslationDate;
    
    string                fCommandLineWithLongOptions;
    string                fCommandLineWithShortOptions;


    // warning and error handling
    // --------------------------------------
      
    bool                  fQuiet;
    bool                  fIgnoreErrors;
    bool                  fAbortOnErrors;
    bool                  fDisplaySourceCodePosition;
    
    // geometry
    // --------------------------------------

    bool                  fTraceGeometry;
    
    // CPU usage
    // --------------------------------------
  
    bool                  fDisplayCPUusage;

    // trace and display  
    // --------------------------------------

    bool                  fTracePasses;
    
    bool                  fTraceOptions;
    bool                  fDisplayOptionsValues;
    bool                  fDisplayOptionsHandler;

    // score
    bool                  fTraceScore;

    // varValAssocs
    bool                  fTraceVarValAssocs;
    
    // credits
    bool                  fTraceCredits;

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

    // measures
    bool                  fTraceMeasures;
    bool                  fTraceMeasuresDetails;
    set<int>              fTraceDetailedMeasureNumbersSet;

    // clefs
    bool                  fTraceClefs;

    // keys
    bool                  fTraceKeys;

    // times
    bool                  fTraceTimes;

    // tempos
    bool                  fTraceTempos;

    // rehearsals
    bool                  fTraceRehearsals;

    // page breaks
    bool                  fTracePageBreaks;

    // transpositions
    bool                  fTraceTranspositions;

    // octave shifts
    bool                  fTraceOctaveShifts;

    // segments
    bool                  fTraceSegments;
    bool                  fTraceSegmentsDetails;

    // barlines
    bool                  fTraceBarlines;
    bool                  fTraceBarlinesDetails;

    // multiple rests
    bool                  fTraceMultipleRests;

    // repeats
    bool                  fTraceRepeats;
    bool                  fTraceRepeatsDetails;

    // rmeasure repeats
    bool                  fTraceMeasureRepeats;

    // slashes
    bool                  fTraceSlashes;

    // notes
    bool                  fTraceNotes;
    bool                  fTraceNotesDetails;

    // stems
    bool                  fTraceStems;

    // beams
    bool                  fTraceBeams;

    // articulations
    bool                  fTraceArticulations;

    // technicals
    bool                  fTraceTechnicals;

    // ornaments
    bool                  fTraceOrnaments;

    // dynamics
    bool                  fTraceGraceDynamics;

    // spanners
    bool                  fTraceSpanners;

    // words
    bool                  fTraceWords;

    // tremolos
    bool                  fTraceTremolos;

    // chords
    bool                  fTraceChords;
    bool                  fTraceChordsDetails;

    // tuplets
    bool                  fTraceTuplets;
    bool                  fTraceTupletsDetails;

    // glissandos
    bool                  fTraceGlissandos;

    // eyeglases
    bool                  fTraceEyeGlasses;

    // damps
    bool                  fTraceDamps;

    // dampalls
    bool                  fTraceDampAlls;

    // slides
    bool                  fTraceSlides;

    // grace notes
    bool                  fTraceGraceNotes;

    // lyrics
    bool                  fTraceLyrics;
    bool                  fTraceLyricsDetails;

    // harmonies
    bool                  fTraceHarmonies;

    // frames
    bool                  fTraceFrames;

    // figured bass
    bool                  fTraceFiguredBass;

    // dynamics
    bool                  fTraceDynamics;

    // ties
    bool                  fTraceTies;

    // slurs
    bool                  fTraceSlurs;

    // ligatures
    bool                  fTraceLigatures;

    // pedals
    bool                  fTracePedals;

    // wedges
    bool                  fTraceWedges;

    // staff tuning
    bool                  fTraceStaffTuning;
    
    // scordaturas
    bool                  fTraceScordaturas;
    
    // harp pedals tuning
    bool                  fTraceHarpPedalsTuning;

    // extra chords
    bool                  fTraceExtraChords;

    // msrStreams
    bool                  fTraceMsrStreams;
    
    // midi
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
