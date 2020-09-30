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
class traceOahGroup : public oahGroup
{
  public:

    static SMARTP<traceOahGroup> create (
      S_oahHandler handlerUpLink);

    SMARTP<traceOahGroup>      createCloneWithTrueValues (); // JMI

  public:

    // initialisation
    // ------------------------------------------------------

    void                  initializeTraceOahGroup (
                            bool boolOptionsInitialValue);

    void                  initializeOptionsTraceAndDisplayOptions (
                            bool boolOptionsInitialValue);

    void                  initializePrintLayoutsTraceOah (
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

    void                  initializeHarmoniesTraceOah (
                            bool boolOptionsInitialValue);

    void                  initializeFiguredBassesTraceOah (
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

    traceOahGroup (
      S_oahHandler handlerUpLink);

    virtual ~traceOahGroup ();

  public:

    // set and get
    // ------------------------------------------------------

   /* JMI
   void                  setAllGeneralTraceOah (
                            bool boolOptionsInitialValue);
                            */

    // options and help trace
    // --------------------------------------

    void                  setTraceOah ()
                              { fTraceOah = true; }
    bool                  getTraceOah () const
                              { return fTraceOah; }

    bool                  getTraceOahDetails () const
                              { return fTraceOahDetails; }

    // other
    // --------------------------------------

    // passes
    bool                  getTracePasses () const
                              { return fTracePasses; }
    // scaling
    bool                  getTraceGeometry () const
                              { return fTraceGeometry; }
    // varValAssocs
    bool                  getTraceVarValAssocs () const
                              { return fTraceVarValAssocs; } // specialize ??? JMI
    // for tests, hidden
    bool                  getTraceForTests () const
                              { return fTraceForTests; }


    // score to voices
    // --------------------------------------

    // score
    bool                  getTraceScore () const
                              { return fTraceScore; }
    // part groups
    bool                  getTracePartGroups () const
                              { return fTracePartGroups; }
    bool                  getTracePartGroupsDetails () const
                              { return fTracePartGroupsDetails; }
    // parts
    bool                  getTraceParts () const
                              { return fTraceParts; }
    // staves
    bool                  getTraceStaves () const
                              { return fTraceStaves; }
    // staff details
    bool                  getTraceStaffDetails () const
                              { return fTraceStaffDetails; }
    // staff changes
    bool                  getTraceStaffChanges () const
                              { return fTraceStaffChanges; }
    // voices
    bool                  getTraceVoices () const
                              { return fTraceVoices; }
    bool                  getTraceVoicesDetails () const
                              { return fTraceVoicesDetails; }


    // print layouts
    // --------------------------------------

    bool                  getTracePrintLayouts () const
                              { return fTracePrintLayouts; }

    // segments and measures
    // --------------------------------------

    // segments
    bool                  getTraceSegments () const
                              { return fTraceSegments; }
    bool                  getTraceSegmentsDetails () const
                              { return fTraceSegmentsDetails; }
    // measures
    bool                  getTraceMeasuresNumbers () const
                              { return fTraceMeasuresNumbers; }
    bool                  getTraceMeasures () const
                              { return fTraceMeasures; }
    bool                  getTraceMeasuresDetails () const
                              { return fTraceMeasuresDetails; }
    bool                  getTracePositionsInMeasures () const
                              { return fTracePositionsInMeasures; }
    set<int>              getTraceDetailedMeasureNumbersSet () const
                              { return fTraceDetailedMeasureNumbersSet; }


    // clefs to tempos
    // --------------------------------------

    // clefs
    bool                  getTraceClefs () const
                              { return fTraceClefs; }
    // keys
    bool                  getTraceKeys () const
                              { return fTraceKeys; }
    // times
    bool                  getTraceTimes () const
                              { return fTraceTimes; }
    // tempos
    bool                  getTraceTempos () const
                              { return fTraceTempos; }


    // breaks and barlines
    // --------------------------------------

    // line breaks
    bool                  getTraceLineBreaks () const
                              { return fTraceLineBreaks; }
    // page breaks
    bool                  getTracePageBreaks () const
                              { return fTracePageBreaks; }
    // barlines
    bool                  getTraceBarlines () const
                              { return fTraceBarlines; }
    // barlines details
    bool                  getTraceBarlinesDetails () const
                              { return fTraceBarlinesDetails; }


    // bar checks
    // --------------------------------------

    bool                  getTraceBarChecks () const
                              { return fTraceBarChecks; }


    // bar number checks
    // --------------------------------------

    bool                  getTraceBarNumberChecks () const
                              { return fTraceBarNumberChecks; }


    // transpositions
    // --------------------------------------

    // transpositions
    bool                  getTraceTranspositions () const
                              { return fTraceTranspositions; }
    // octave shifts
    bool                  getTraceOctaveShifts () const
                              { return fTraceOctaveShifts; }


    // repeats to slashes
    // --------------------------------------

    // repeats
    bool                  getTraceRepeats () const
                              { return fTraceRepeats; }
    bool                  getTraceRepeatsDetails () const
                              { return fTraceRepeatsDetails; }
    // measures repeats
    bool                  getTraceMeasuresRepeats () const
                              { return fTraceMeasuresRepeats; }
    // rest measures
    bool                  getTraceRestMeasures () const
                              { return fTraceRestMeasures; }
    // beats repeats
    bool                  getTraceBeatsRepeats () const
                              { return fTraceBeatsRepeats; }
    // slashes
    bool                  getTraceSlashes () const
                              { return fTraceSlashes; }


    // notes
    // --------------------------------------

    // notes
    bool                  getTraceNotes () const
                              { return fTraceNotes; }
    // notes details
    bool                  getTraceNotesDetails () const
                              { return fTraceNotesDetails; }
    // whole notes
    bool                  getTraceWholeNotes () const
                              { return fTraceWholeNotes; }
    bool                  getTraceWholeNotesDetails () const
                              { return fTraceWholeNotesDetails; }
    // skip notes
    bool                  getTraceSkipNotes () const
                              { return fTraceSkipNotes; }
    // rest notes
    bool                  getTraceRestNotes () const
                              { return fTraceRestNotes; }
    // notes octave entry
    bool                  getTraceNotesOctaveEntry () const
                              { return fTraceNotesOctaveEntry; }
    // grace notes
    bool                  getTraceGraceNotes () const
                              { return fTraceGraceNotes; }
    // tremolos
    bool                  getTraceTremolos () const
                              { return fTraceTremolos; }


    // note attachments
    // --------------------------------------

    // stems
    bool                  getTraceStems () const
                              { return fTraceStems; }
    // beams
    bool                  getTraceBeams () const
                              { return fTraceBeams; }
    // articulations
    bool                  getTraceArticulations () const
                              { return fTraceArticulations; }
    // technicals
    bool                  getTraceTechnicals () const
                              { return fTraceTechnicals; }
    // ornaments
    bool                  getTraceOrnaments () const
                              { return fTraceOrnaments; }
    // dynamics
    bool                  getTraceDynamics () const
                              { return fTraceDynamics; }


    // spanners
    // --------------------------------------

    // spanners
    bool                  getTraceSpanners () const
                              { return fTraceSpanners; } // ???
    // wedges
    bool                  getTraceWedges () const
                              { return fTraceWedges; }
    // spanners
    // slurs
    bool                  getTraceSlurs () const
                              { return fTraceSlurs; }
    bool                  getTraceSlursDetails () const
                              { return fTraceSlursDetails; }
    // ligatures
    bool                  getTraceLigatures () const
                              { return fTraceLigatures; }


    // credits to words
    // --------------------------------------

    // credits
    bool                  getTraceCredits () const
                              { return fTraceCredits; }
    // lyrics
    bool                  getTraceLyrics () const
                              { return fTraceLyrics; }
    bool                  getTraceLyricsDetails () const
                              { return fTraceLyricsDetails; }
    // words
    bool                  getTraceWords () const
                              { return fTraceWords; }


    // chords and tuplets
    // --------------------------------------

    // chords
    bool                  getTraceChords () const
                              { return fTraceChords; }
    bool                  getTraceChordsDetails () const
                              { return fTraceChordsDetails; }
    // tuplets
    bool                  getTraceTuplets () const
                              { return fTraceTuplets; }
    bool                  getTraceTupletsDetails () const
                              { return fTraceTupletsDetails; }

    // inter-notes
    // --------------------------------------

    // ties
    bool                  getTraceTies () const
                              { return fTraceTies; }
    // glissandos
    bool                  getTraceGlissandos () const
                              { return fTraceGlissandos; }

    // above staff
    // --------------------------------------

    // rehearsals
    bool                  getTraceRehearsals () const
                              { return fTraceRehearsals; }
    // segnos
    bool                  getTraceSegnos () const
                              { return fTraceSegnos; }
    // dal segnos
    bool                  getTraceDalSegnos () const
                              { return fTraceDalSegnos; }
    // codas
    bool                  getTraceCodas () const
                              { return fTraceCodas; }
    // eyeglases
    bool                  getTraceEyeGlasses () const
                              { return fTraceEyeGlasses; }

    // harmonies
    // --------------------------------------

    bool                  getTraceHarmonies () const
                              { return fTraceHarmonies; }
    bool                  getTraceHarmoniesDetails () const
                              { return fTraceHarmoniesDetails; }

    bool                  getTraceExtraHarmonies () const
                              { return fTraceExtraHarmonies; }

    // figured basses
    // --------------------------------------

    bool                  getTraceFiguredBasses () const
                              { return fTraceFiguredBasses; }

    // instruments
    // --------------------------------------

    // frames
    bool                  getTraceFrames () const
                              { return fTraceFrames; }
    // scordaturas
    bool                  getTraceScordaturas () const
                              { return fTraceScordaturas; }
    // slides
    bool                  getTraceSlides () const
                              { return fTraceSlides; }
    // pedals
    bool                  getTracePedals () const
                              { return fTracePedals; }
    // accordion registrations
    bool                  getTraceAccordionRegistrations () const
                              { return fTraceAccordionRegistrations; }
    // harp pedals
    bool                  getTraceHarpPedals () const
                              { return fTraceHarpPedals; }
    // harp pedals tuning
    bool                  getTraceHarpPedalsTunings () const
                              { return fTraceHarpPedalsTunings; }
    // damps
    bool                  getTraceDamps () const
                              { return fTraceDamps; }
    // dampalls
    bool                  getTraceDampAlls () const
                              { return fTraceDampAlls; }
    // midi
    bool                  getTraceMidi () const
                              { return fTraceMidi; }

/* JMI
    // msrStreams
    // --------------------------------------
    bool                  getTraceMsrStreams () const
                              { return fTraceMsrStreams; }

    // layout
*/

  public:

    // public services
    // ------------------------------------------------------

    // quiet mode
    void                  enforceGroupQuietness () override;

    // consistency check
    virtual void          checkGroupOptionsConsistency () override;

  public:

    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v) override;
    virtual void          acceptOut (basevisitor* v) override;

    virtual void          browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    void                  printTraceOahHelp ();

    void                  printTraceOahValues (int fieldWidth);

    virtual void          printValuedAtomOptionsValues (
                            ostream& os,
                            int      valueFieldWidth) const;

  private:

    // options and help trace
    // --------------------------------------

    bool                  fTraceOah;
    bool                  fTraceOahDetails;

    // other
    // --------------------------------------

    // passes
    bool                  fTracePasses;

  public:

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


    // print layouts
    // --------------------------------------

    bool                  fTracePrintLayouts;

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
    bool                  fTraceBarlines;
    // barlines details
    bool                  fTraceBarlinesDetails;


    // bar checks
    // --------------------------------------

    bool                  fTraceBarChecks;


    // bar number checks
    // --------------------------------------

    bool                  fTraceBarNumberChecks;


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
    bool                  fTraceSlursDetails;
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
    // dal segnos
    bool                  fTraceDalSegnos;
    // codas
    bool                  fTraceCodas;
    // eyeglases
    bool                  fTraceEyeGlasses;

    // harmonies
    // --------------------------------------

    bool                  fTraceHarmonies;
    bool                  fTraceHarmoniesDetails;

    bool                  fTraceExtraHarmonies;

    // figured basses
    // --------------------------------------

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
typedef SMARTP<traceOahGroup> S_traceOahGroup;
EXP ostream& operator<< (ostream& os, const S_traceOahGroup& elt);

EXP extern S_traceOahGroup gGlobalTraceOahGroup;
EXP extern S_traceOahGroup gGlobalTraceOahGroupUserChoices;
EXP extern S_traceOahGroup gGlobalTraceOahGroupWithDetailedTrace;

//______________________________________________________________________________
void initializeTraceOahHandling (
  S_oahHandler handler);


}


#endif


#endif
