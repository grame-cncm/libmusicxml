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

#include "oahBasicTypes.h"


namespace MusicXML2
{

//______________________________________________________________________________
class traceOptions : public oahGroup
{
  public:

    static SMARTP<traceOptions> create (
      S_oahHandler handler);

    SMARTP<traceOptions>        createCloneWithTrueValues (); // JMI

  public:

    // initialisation
    // ------------------------------------------------------

    void                  initializeTraceOptions (
                            bool boolOptionsInitialValue);

    void                  initializeLowLevelTraceOptions (
                            bool boolOptionsInitialValue);

    void                  initializeRepeatsTraceOptions (
                            bool boolOptionsInitialValue);

    void                  initializeNotesTraceOptions (
                            bool boolOptionsInitialValue);

    void                  initializeNotesAttachmentsTraceOptions (
                            bool boolOptionsInitialValue);

    void                  initializeScoreToVoicesTraceOptions (
                            bool boolOptionsInitialValue);

    void                  initializeSegmentsAndMeasuresTraceOptions (
                            bool boolOptionsInitialValue);

    void                  initializeInstrumentsTraceOptions (
                            bool boolOptionsInitialValue);

    void                  initializeChordsAndTupletsTraceOptions (
                            bool boolOptionsInitialValue);

    void                  initializeLyricsAndWordsTraceOptions (
                            bool boolOptionsInitialValue);

    void                  initializeHarmoniesAndFiguredBassesTraceOptions (
                            bool boolOptionsInitialValue);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    traceOptions (
      S_oahHandler handler);

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

    virtual S_oahValuedAtom
                          handleAtom (
                            ostream&  os,
                            S_oahAtom atom);

  public:

    // print
    // ------------------------------------------------------

    void                  printTraceOptionsHelp ();

    void                  printTraceOptionsValues (int fieldWidth);

    virtual void          printOptionsValues (
                            ostream& os,
                            int      valueFieldWidth) const;

  public:

    // passes
    // --------------------------------------
    bool                  fTracePasses;

    // for tests, hidden
    // --------------------------------------
    bool                  fTraceForTests;

    // score to voices
    // --------------------------------------
    // score
    bool                  fTraceScore;
    // part groups
    bool                  fTracePartGroups;
    bool                  fTracePartGroupsDetails;
    // parts
    bool                  fTraceParts;
    // staves
    bool                  fTraceStaves;
    // staff details
    bool                  fTraceStaffDetails;
    // staff changes
    bool                  fTraceStaffChanges;
    // voices
    bool                  fTraceVoices;
    bool                  fTraceVoicesDetails;

    // varValAssocs
    // --------------------------------------
    bool                  fTraceVarValAssocs;

    // credits
    // --------------------------------------
    bool                  fTraceCredits;

    // geometry
    // --------------------------------------
    bool                  fTraceGeometry;

    // segments and measures
    // --------------------------------------
    // segments
    bool                  fTraceSegments;
    bool                  fTraceSegmentsDetails;
    // measures
    bool                  fTraceMeasures;
    bool                  fTraceMeasuresDetails;
    bool                  fTracePositionsInMeasures;
    set<int>              fTraceDetailedMeasureNumbersSet;

    // clefs
    // --------------------------------------
    bool                  fTraceClefs;

    // keys
    // --------------------------------------
    bool                  fTraceKeys;

    // times
    // --------------------------------------
    bool                  fTraceTimes;

    // tempos
    // --------------------------------------
    bool                  fTraceTempos;

    // rehearsals
    // --------------------------------------
    bool                  fTraceRehearsals;

    // line breaks
    // --------------------------------------
    bool                  fTraceLineBreaks;

    // page breaks
    // --------------------------------------
    bool                  fTracePageBreaks;

    // transpositions
    // --------------------------------------
    bool                  fTraceTranspositions;

    // octave shifts
    // --------------------------------------
    bool                  fTraceOctaveShifts;

    // barlines
    // --------------------------------------
    bool                  fTraceBarLines;
    bool                  fTraceBarLinesDetails;

    // repeats
    // --------------------------------------

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
    // --------------------------------------

    // notes
    bool                  fTraceNotes;
    // notes details
    bool                  fTraceNotesDetails;
    // whole notes
    bool                  fTraceWholeNotes;
    // skip notes
    bool                  fTraceSkipNotes;
    // rest notes
    bool                  fTraceRestNotes;
    // notes octave entry
    bool                  fTraceNotesOctaveEntry;

    // note attachments
    // --------------------------------------

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
    bool                  fTraceDynamics;

    // spanners
    // --------------------------------------
    bool                  fTraceSpanners;

    // lyrics and words
    // --------------------------------------

    // lyrics
    bool                  fTraceLyrics;
    bool                  fTraceLyricsDetails;
    // words
    bool                  fTraceWords;

    // tremolos
    // --------------------------------------
    bool                  fTraceTremolos;

    // chords and tuplets
    // --------------------------------------

    // chords
    bool                  fTraceChords;
    bool                  fTraceChordsDetails;
    // tuplets
    bool                  fTraceTuplets;
    bool                  fTraceTupletsDetails;

    // glissandos
    // --------------------------------------
    bool                  fTraceGlissandos;

    // eyeglases
    // --------------------------------------
    bool                  fTraceEyeGlasses;

    // damps
    // --------------------------------------
    bool                  fTraceDamps;

    // dampalls
    // --------------------------------------
    bool                  fTraceDampAlls;

    // slides
    // --------------------------------------
    bool                  fTraceSlides;

    // grace notes
    // --------------------------------------
    bool                  fTraceGraceNotes;

    // harmonies and figured basses
    // --------------------------------------

    // harmonies
    bool                  fTraceHarmonies;
    // figured basses
    bool                  fTraceFiguredBasses;

    // ties
    // --------------------------------------
    bool                  fTraceTies;

    // slurs
    // --------------------------------------
    bool                  fTraceSlurs;

    // ligatures
    // --------------------------------------
    bool                  fTraceLigatures;

    // wedges
    // --------------------------------------
    bool                  fTraceWedges;

    // segnos
    // --------------------------------------
    bool                  fTraceSegnos;

    // codas
    // --------------------------------------
    bool                  fTraceCodas;

    // instruments
    // --------------------------------------

    // frames
    bool                  fTraceFrames;
    // scordaturas
    bool                  fTraceScordaturas;
    // pedals
    bool                  fTracePedals;
    // accordion registrations
    bool                  fTraceAccordionRegistrations;
    // harp pedals
    bool                  fTraceHarpPedals;
    // harp pedals tuning
    bool                  fTraceHarpPedalsTunings;

    // extra chords
    // --------------------------------------
    bool                  fTraceExtraChords;

/* JMI
    // msrStreams
    // --------------------------------------
    bool                  fTraceMsrStreams;
*/

    // midi
    // --------------------------------------
    bool                  fTraceMidi;
};
typedef SMARTP<traceOptions> S_traceOptions;
EXP ostream& operator<< (ostream& os, const S_traceOptions& elt);

EXP extern S_traceOptions gTraceOptions;
EXP extern S_traceOptions gTraceOptionsUserChoices;
EXP extern S_traceOptions gTraceOptionsWithDetailedTrace;

//______________________________________________________________________________
void initializeTraceOptionsHandling (
  S_oahHandler handler);


}


#endif


#endif
