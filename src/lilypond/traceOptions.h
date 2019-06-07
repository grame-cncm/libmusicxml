/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef ___traceOptions___
#define ___traceOptions___


#include "setTraceOptionsIfDesired.h"

#ifdef TRACE_OPTIONS


#include <set>

#include "exports.h"

#include "optionsBasicTypes.h"


namespace MusicXML2
{

//______________________________________________________________________________
class traceOptions : public optionsGroup
{
  public:

    static SMARTP<traceOptions> create (
      S_optionsHandler optionsHandler);

    SMARTP<traceOptions>        createCloneWithTrueValues (); // JMI

  public:

    // initialisation
    // ------------------------------------------------------

    void                  initializeTraceOptions (
                            bool boolOptionsInitialValue);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    traceOptions (
      S_optionsHandler optionsHandler);

    virtual ~traceOptions ();

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

   /* JMI
   void                  setAllGeneralTraceOptions (
                            bool boolOptionsInitialValue);
                            */

  public:

    // services
    // ------------------------------------------------------

    virtual S_optionsItem handleOptionsItem (
                            ostream&      os,
                            S_optionsItem item);

  public:

    // print
    // ------------------------------------------------------

    void                  printTraceOptionsHelp ();

    void                  printTraceOptionsValues (int fieldWidth);

  public:


    // options
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

    // geometry
    bool                  fTraceGeometry;

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

    // line breaks
    bool                  fTraceLineBreaks;

    // page breaks
    bool                  fTracePageBreaks;

    // staff changes
    bool                  fTraceStaffChanges;

    // transpositions
    bool                  fTraceTranspositions;

    // octave shifts
    bool                  fTraceOctaveShifts;

    // segments
    bool                  fTraceSegments;
    bool                  fTraceSegmentsDetails;

    // barlines
    bool                  fTraceBarLines;
    bool                  fTraceBarLinesDetails;

    // repeats
    bool                  fTraceRepeats;
    bool                  fTraceRepeatsDetails;

    // measures repeats
    bool                  fTraceMeasuresRepeats;

    // rest measures
    bool                  fTraceRestMeasures;

    // slashes
    bool                  fTraceSlashes;

    // notes
    bool                  fTraceNotes;
    bool                  fTraceNotesDetails;
    bool                  fTraceWholeNotes;

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

    // figured basses
    bool                  fTraceFiguredBasses;

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

    // staff details
    bool                  fTraceStaffDetails;

    // scordaturas
    bool                  fTraceScordaturas;

    // segnos
    bool                  fTraceSegnos;

    // codas
    bool                  fTraceCodas;

    // accordion registrations
    bool                  fTraceAccordionRegistrations;

    // harp pedals
    bool                  fTraceHarpPedals;

    // harp pedals tuning
    bool                  fTraceHarpPedalsTunings;

    // extra chords
    bool                  fTraceExtraChords;

    // msrStreams
    bool                  fTraceMsrStreams;

    // midi
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


#endif
