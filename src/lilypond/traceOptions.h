/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef __traceOptions__
#define __traceOptions__

#include <set>

#include "optionsHandling.h"
#include "exports.h"


namespace MusicXML2 
{

//______________________________________________________________________________
class traceOptions : public optionsGroup
{
  public:

    static SMARTP<traceOptions> create (
      S_optionsHandler optionsHandler);
        
    SMARTP<traceOptions>        createCloneWithDetailedTrace ();

  public:

    // initialisation
    // ------------------------------------------------------

    void                  initializeTraceOptions (
                            bool boolOptionsInitialValue);
        
    void                  printTraceOptionsHelp ();

    void                  printTraceOptionsValues (int fieldWidth);
    
  protected:
  
    // constructors/destructor
    // ------------------------------------------------------

    traceOptions (
      S_optionsHandler optionsHandler);
  
    virtual ~traceOptions ();

  public:

    // consistency check
    // ------------------------------------------------------

    void                  checkTraceOptionsConsistency () const;

  public:

    // trace and display  
    bool                  fTraceBasic;
    
    bool                  fDisplayOptionsValues;
    
    set<int>              fTraceDetailedMeasureNumbersSet;


    // encoding
    bool                  fTraceEncoding;

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

    // tempos
    bool                  fTraceTempos;

    // transpositions
    bool                  fTraceTranspositions;

    // octave shifts
    bool                  fTraceOctaveShifts;

    // segments
    bool                  fTraceSegments;

    // barlines
    bool                  fTraceBarlines;

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

    // ornaments
    bool                  fTraceOrnaments;

    // spanners
    bool                  fTraceSpanners;

    // words
    bool                  fTraceWords;

    // tremolos
    bool                  fTraceTremolos;

    // chords
    bool                  fTraceChords;

    // tuplets
    bool                  fTraceTuplets;

    // glissandos
    bool                  fTraceGlissandos;

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
    
    /* STUFF not yet handled JMI */
    bool                  fTraceMidi;
};
typedef SMARTP<traceOptions> S_traceOptions;
EXP ostream& operator<< (ostream& os, const S_traceOptions& elt);

EXP extern S_traceOptions gTraceOptions;
EXP extern S_traceOptions gTraceOptionsUserChoices;
EXP extern S_traceOptions gTraceOptionsWithDetailedTrace;

//______________________________________________________________________________
void initializeTraceOptionsHandling (
  S_optionsHandler optionsHandler);


}


#endif
