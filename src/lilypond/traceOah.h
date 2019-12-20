/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef ___traceOah___
#define ___traceOah___


#include "setTraceOahIfDesired.h"

#ifdef TRACE_OAH


#include <set>

#include "exports.h"

#include "oahBasicTypes.h"


namespace MusicXML2
{

//______________________________________________________________________________
class traceOah : public oahGroup
{
  public:

    static SMARTP<traceOah> create (
      S_oahHandler handlerUpLink);

    SMARTP<traceOah>      createCloneWithTrueValues (); // JMI

  public:

    // initialisation
    // ------------------------------------------------------

    void                  initializeTraceOah (
                            bool boolOptionsInitialValue);

    void                  initializeOptionsTraceAndDisplayOptions (
                            bool boolOptionsInitialValue);

    void                  initializeRepeatsToSlashesTraceOah (
                            bool boolOptionsInitialValue);

    void                  initializeNotesTraceOah (
                            bool boolOptionsInitialValue);

    void                  initializeNotesAttachmentsTraceOah (
                            bool boolOptionsInitialValue);

    void                  initializeScoreToVoicesTraceOah (
                            bool boolOptionsInitialValue);

    void                  initializeSegmentsAndMeasuresTraceOah (
                            bool boolOptionsInitialValue);

    void                  initializeInstrumentsTraceOah (
                            bool boolOptionsInitialValue);

    void                  initializeChordsAndTupletsTraceOah (
                            bool boolOptionsInitialValue);

    void                  initializeCreditsToWordsTraceOah (
                            bool boolOptionsInitialValue);

    void                  initializeHarmoniesAndFiguredBassesTraceOah (
                            bool boolOptionsInitialValue);

    void                  initializeSpannersTraceOah (
                            bool boolOptionsInitialValue);

    void                  initializeInterNotesTraceOah (
                            bool boolOptionsInitialValue);

    void                  initializeClefsToTemposTraceOah (
                            bool boolOptionsInitialValue);

    void                  initializeBreaksAndBarlinesTraceOah (
                            bool boolOptionsInitialValue);

    void                  initializeAboveStaffTraceOah (
                            bool boolOptionsInitialValue);

    void                  initializeTranspositionsTraceOah (
                            bool boolOptionsInitialValue);

    void                  initializeOtherTraceOah (
                            bool boolOptionsInitialValue);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    traceOah (
      S_oahHandler handlerUpLink);

    virtual ~traceOah ();

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
   void                  setAllGeneralTraceOah (
                            bool boolOptionsInitialValue);
                            */

  public:

    // services
    // ------------------------------------------------------

  public:

    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v);
    virtual void          acceptOut (basevisitor* v);

    virtual void          browseData (basevisitor* v);

  public:

    // print
    // ------------------------------------------------------

    void                  printTraceOahHelp ();

    void                  printTraceOahValues (int fieldWidth);

    virtual void          printAtomOptionsValues (
                            ostream& os,
                            int      valueFieldWidth) const;

  public:

    // options and help trace
    // --------------------------------------

    bool                  fTraceOah;
    bool                  fTraceOahDetails;

    // other
    // --------------------------------------

    // passes
    bool                  fTracePasses;
    // scaling
    bool                  fTraceGeometry;
    // varValAssocs
    bool                  fTraceVarValAssocs; // specialize ??? JMI
    // for tests, hidden
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


    // segments and measures
    // --------------------------------------

    // segments
    bool                  fTraceSegments;
    bool                  fTraceSegmentsDetails;
    // measures
    bool                  fTraceMeasuresNumbers;
    bool                  fTraceMeasures;
    bool                  fTraceMeasuresDetails;
    bool                  fTracePositionsInMeasures;
    set<int>              fTraceDetailedMeasureNumbersSet;


    // clefs to tempos
    // --------------------------------------

    // clefs
    bool                  fTraceClefs;
    // keys
    bool                  fTraceKeys;
    // times
    bool                  fTraceTimes;
    // tempos
    bool                  fTraceTempos;


    // breaks and barlines
    // --------------------------------------

    // line breaks
    bool                  fTraceLineBreaks;
    // page breaks
    bool                  fTracePageBreaks;
    // barlines
    bool                  fTraceBarLines;
    // barlines details
    bool                  fTraceBarLinesDetails;


    // transpositions
    // --------------------------------------

    // transpositions
    bool                  fTraceTranspositions;
    // octave shifts
    bool                  fTraceOctaveShifts;


    // repeats to slashes
    // --------------------------------------

    // repeats
    bool                  fTraceRepeats;
    bool                  fTraceRepeatsDetails;
    // measures repeats
    bool                  fTraceMeasuresRepeats;
    // rest measures
    bool                  fTraceRestMeasures;
    // beats repeats
    bool                  fTraceBeatsRepeats;
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
    bool                  fTraceWholeNotesDetails;
    // skip notes
    bool                  fTraceSkipNotes;
    // rest notes
    bool                  fTraceRestNotes;
    // notes octave entry
    bool                  fTraceNotesOctaveEntry;
    // grace notes
    bool                  fTraceGraceNotes;
    // tremolos
    bool                  fTraceTremolos;


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

    // spanners
    bool                  fTraceSpanners; // ???
    // wedges
    bool                  fTraceWedges;
    // spanners
    // slurs
    bool                  fTraceSlurs;
    // ligatures
    bool                  fTraceLigatures;


    // credits to words
    // --------------------------------------

    // credits
    bool                  fTraceCredits;
    // lyrics
    bool                  fTraceLyrics;
    bool                  fTraceLyricsDetails;
    // words
    bool                  fTraceWords;


    // chords and tuplets
    // --------------------------------------

    // chords
    bool                  fTraceChords;
    bool                  fTraceChordsDetails;
    // tuplets
    bool                  fTraceTuplets;
    bool                  fTraceTupletsDetails;
    // extra chords
    bool                  fTraceExtraChords;


    // inter-notes
    // --------------------------------------

    // ties
    bool                  fTraceTies;
    // glissandos
    bool                  fTraceGlissandos;


    // above staff
    // --------------------------------------

    // rehearsals
    bool                  fTraceRehearsals;
    // segnos
    bool                  fTraceSegnos;
    // codas
    bool                  fTraceCodas;
    // eyeglases
    bool                  fTraceEyeGlasses;


    // harmonies and figured basses
    // --------------------------------------

    // harmonies
    bool                  fTraceHarmonies;
    // figured basses
    bool                  fTraceFiguredBasses;


    // instruments
    // --------------------------------------

    // frames
    bool                  fTraceFrames;
    // scordaturas
    bool                  fTraceScordaturas;
    // slides
    bool                  fTraceSlides;
    // pedals
    bool                  fTracePedals;
    // accordion registrations
    bool                  fTraceAccordionRegistrations;
    // harp pedals
    bool                  fTraceHarpPedals;
    // harp pedals tuning
    bool                  fTraceHarpPedalsTunings;
    // damps
    bool                  fTraceDamps;
    // dampalls
    bool                  fTraceDampAlls;
    // midi
    bool                  fTraceMidi;


/* JMI
    // msrStreams
    // --------------------------------------
    bool                  fTraceMsrStreams;

    // layout
*/

  private:

    // private work fields
    // --------------------------------------

    S_oahPrefix           fShortTracePrefix;
    S_oahPrefix           fLongTracePrefix;
};
typedef SMARTP<traceOah> S_traceOah;
EXP ostream& operator<< (ostream& os, const S_traceOah& elt);

EXP extern S_traceOah gTraceOah;
EXP extern S_traceOah gTraceOahUserChoices;
EXP extern S_traceOah gTraceOahWithDetailedTrace;

//______________________________________________________________________________
void initializeTraceOahHandling (
  S_oahHandler handler);


}


#endif


#endif
