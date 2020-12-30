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


#include "enableTracingIfDesired.h"

#ifdef TRACING_IS_ENABLED

#include <set>

#include "exports.h"

#include "oahBasicTypes.h"


namespace MusicXML2
{

//______________________________________________________________________________
class EXP traceOahGroup : public oahGroup
{
  public:

    static SMARTP<traceOahGroup> create (
      S_oahPrefix shortTracePrefix,
      S_oahPrefix longTracePrefix);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    traceOahGroup (
      S_oahPrefix shortTracePrefix,
      S_oahPrefix longTracePrefix);

    virtual ~traceOahGroup ();

  public:

    // initialisation
    // ------------------------------------------------------

    void                  initializeTraceOahGroup ();

    void                  initializeOptionsTraceAndDisplayOptions ();

    void                  initializePrintLayoutsTraceOah ();

    void                  initializeRepeatsToSlashesTraceOah ();

    void                  initializeNotesTraceOah ();

    void                  initializeNotesAttachmentsTraceOah ();

    void                  initializeScoreToVoicesTraceOah ();

    void                  initializeSegmentsAndMeasuresTraceOah ();

    void                  initializeInstrumentsTraceOah ();

    void                  initializeChordsAndTupletsTraceOah ();

    void                  initializeCreditsToWordsTraceOah ();

    void                  initializeHarmoniesTraceOah ();

    void                  initializeFiguredBassesTraceOah ();

    void                  initializeSpannersTraceOah ();

    void                  initializeInterNotesTraceOah ();

    void                  initializeClefsToTemposTraceOah ();

    void                  initializeBreaksAndBarlinesTraceOah ();

    void                  initializeAboveStaffTraceOah ();

    void                  initializeTranspositionsTraceOah ();

    void                  initializeOtherTraceOah ();

  public:

    // set and get
    // ------------------------------------------------------

    // options and help trace
    // --------------------------------------

    S_oahPrefix           getShortTracePrefix () const
                              { return fShortTracePrefix; }
    S_oahPrefix           getLongTracePrefix () const
                              { return fLongTracePrefix; }

    void                  setTraceOah ()
                              { fTraceOah = true; }
    bool                  getTraceOah () const
                              { return fTraceOah; }

    S_oahAtom             getTraceOahAtom () const
                              { return fTraceOahAtom; }

    bool                  getTraceOahDetails () const
                              { return fTraceOahDetails; }

    // other
    // --------------------------------------

    // passes
    void                  setTracePasses ()
                              { fTracePasses = true; }
    bool                  getTracePasses () const
                              { return fTracePasses; }
    // geometry
    void                  setTraceGeometry ()
                              { fTraceGeometry = true; }
    bool                  getTraceGeometry () const
                              { return fTraceGeometry; }
    // identification
    void                  setTraceIdentification ()
                              { fTraceIdentification = true; }
    bool                  getTraceIdentification () const
                              { return fTraceIdentification; }
    // for tests, hidden
    void                  setTraceForTests ()
                              { fTraceForTests = true; }
    bool                  getTraceForTests () const
                              { return fTraceForTests; }


    // score to voices
    // --------------------------------------

    // score
    void                  setTraceScore ()
                              { fTraceScore = true; }
    bool                  getTraceScore () const
                              { return fTraceScore; }
    // part groups
    void                  setTracePartGroups ()
                              { fTracePartGroups = true; }
    bool                  getTracePartGroups () const
                              { return fTracePartGroups; }
    bool                  getTracePartGroupsDetails () const
                              { return fTracePartGroupsDetails; }
    // parts
    void                  setTraceParts ()
                              { fTraceParts = true; }
    bool                  getTraceParts () const
                              { return fTraceParts; }
    // staves
    void                  setTraceStaves ()
                              { fTraceStaves = true; }
    bool                  getTraceStaves () const
                              { return fTraceStaves; }
    // staff details
    bool                  getTraceStaffDetails () const
                              { return fTraceStaffDetails; }
    // staff changes
    bool                  getTraceStaffChanges () const
                              { return fTraceStaffChanges; }
    // voices
    void                  setTraceVoices ()
                              { fTraceVoices = true; }
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
    void                  setTraceSegments ()
                              { fTraceSegments = true; }
    bool                  getTraceSegments () const
                              { return fTraceSegments; }

    bool                  getTraceSegmentsDetails () const
                              { return fTraceSegmentsDetails; }
    // measures
    bool                  getTraceMeasuresNumbers () const
                              { return fTraceMeasuresNumbers; }

    void                  setTraceMeasures ()
                              { fTraceMeasures = true; }
    bool                  getTraceMeasures () const
                              { return fTraceMeasures; }

    bool                  getTraceMeasuresDetails () const
                              { return fTraceMeasuresDetails; }

    bool                  getTracePositionsInMeasures () const
                              { return fTracePositionsInMeasures; }
    bool                  getTraceMomentsInMeasures () const
                              { return fTraceMomentsInMeasures; }

    set<int>              getTraceDetailedMeasureNumbersSet () const
                              { return fTraceDetailedMeasureNumbersSet; }


    // clefs to tempos
    // --------------------------------------

    // clefs
    void                  setTraceClefs ()
                              { fTraceClefs = true; }
    bool                  getTraceClefs () const
                              { return fTraceClefs; }
    // keys
    void                  setTraceKeys ()
                              { fTraceKeys = true; }
    bool                  getTraceKeys () const
                              { return fTraceKeys; }
    // times
    void                  setTraceTimes ()
                              { fTraceTimes = true; }
    bool                  getTraceTimes () const
                              { return fTraceTimes; }
    // tempos
    void                  setTraceTempos ()
                              { fTraceTempos = true; }
    bool                  getTraceTempos () const
                              { return fTraceTempos; }


    // breaks and barlines
    // --------------------------------------

    // line breaks
    void                  setTraceLineBreaks ()
                              { fTraceLineBreaks = true; }
    bool                  getTraceLineBreaks () const
                              { return fTraceLineBreaks; }
    // page breaks
    void                  setTracePageBreaks ()
                              { fTracePageBreaks = true; }
    bool                  getTracePageBreaks () const
                              { return fTracePageBreaks; }
    // barlines
    void                  setTraceBarlines ()
                              { fTraceBarlines = true; }
    bool                  getTraceBarlines () const
                              { return fTraceBarlines; }
    // barlines details
    bool                  getTraceBarlinesDetails () const
                              { return fTraceBarlinesDetails; }


    // bar checks
    // --------------------------------------

    void                  setTraceBarChecks ()
                              { fTraceBarChecks = true; }
    bool                  getTraceBarChecks () const
                              { return fTraceBarChecks; }


    // bar number checks
    // --------------------------------------

    void                  setTraceBarNumberChecks ()
                              { fTraceBarNumberChecks = true; }
    bool                  getTraceBarNumberChecks () const
                              { return fTraceBarNumberChecks; }


    // transpositions
    // --------------------------------------

    // transpositions
    void                  setTraceTraceTranspositions ()
                              { fTraceTranspositions = true; }
    bool                  getTraceTranspositions () const
                              { return fTraceTranspositions; }
    // octave shifts
    void                  setTraceOctaveShifts ()
                              { fTraceOctaveShifts = true; }
    bool                  getTraceOctaveShifts () const
                              { return fTraceOctaveShifts; }


    // repeats to slashes
    // --------------------------------------

    // repeats
    void                  setTraceRepeats ()
                              { fTraceRepeats = true; }
    bool                  getTraceRepeats () const
                              { return fTraceRepeats; }

    bool                  getTraceRepeatsDetails () const
                              { return fTraceRepeatsDetails; }
    // measures repeats
    void                  setTraceMeasuresRepeats ()
                              { fTraceMeasuresRepeats = true; }
    bool                  getTraceMeasuresRepeats () const
                              { return fTraceMeasuresRepeats; }
    // rest measures
    void                  setTraceRestMeasures ()
                              { fTraceRestMeasures = true; }
    bool                  getTraceRestMeasures () const
                              { return fTraceRestMeasures; }
    // beats repeats
    void                  setTraceBeatsRepeats ()
                              { fTraceBeatsRepeats = true; }
    bool                  getTraceBeatsRepeats () const
                              { return fTraceBeatsRepeats; }
    // slashes
    void                  setTraceSlashes ()
                              { fTraceSlashes = true; }
    bool                  getTraceSlashes () const
                              { return fTraceSlashes; }


    // notes
    // --------------------------------------

    // notes
    void                  setTraceNotes ()
                              { fTraceNotes = true; }
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
    void                  setTraceSkipNotes ()
                              { fTraceSkipNotes = true; }
    bool                  getTraceSkipNotes () const
                              { return fTraceSkipNotes; }
    // rest notes
    void                  setTraceRestNotes ()
                              { fTraceRestNotes = true; }
    bool                  getTraceRestNotes () const
                              { return fTraceRestNotes; }
    // notes octave entry
    void                  setTraceNotesOctaveEntry ()
                              { fTraceNotesOctaveEntry = true; }
    bool                  getTraceNotesOctaveEntry () const
                              { return fTraceNotesOctaveEntry; }
    // grace notes
    void                  setTraceGraceNotes ()
                              { fTraceGraceNotes = true; }
    bool                  getTraceGraceNotes () const
                              { return fTraceGraceNotes; }
    // tremolos
    void                  setTraceTremolos ()
                              { fTraceTremolos = true; }
    bool                  getTraceTremolos () const
                              { return fTraceTremolos; }


    // note attachments
    // --------------------------------------

    // stems
    void                  setTraceStems ()
                              { fTraceStems = true; }
    bool                  getTraceStems () const
                              { return fTraceStems; }
    // beams
    void                  setTraceBeams ()
                              { fTraceBeams = true; }
    bool                  getTraceBeams () const
                              { return fTraceBeams; }
    // articulations
    void                  setTraceArticulations ()
                              { fTraceArticulations = true; }
    bool                  getTraceArticulations () const
                              { return fTraceArticulations; }
    // technicals
    void                  setTraceTechnicals ()
                              { fTraceTechnicals = true; }
    bool                  getTraceTechnicals () const
                              { return fTraceTechnicals; }
    // ornaments
    void                  setTraceOrnaments ()
                              { fTraceOrnaments = true; }
    bool                  getTraceOrnaments () const
                              { return fTraceOrnaments; }
    // dynamics
    void                  setTraceDynamics ()
                              { fTraceDynamics = true; }
    bool                  getTraceDynamics () const
                              { return fTraceDynamics; }


    // spanners
    // --------------------------------------

    // spanners
    void                  setTraceSpanners ()
                              { fTraceSpanners = true; }
    bool                  getTraceSpanners () const
                              { return fTraceSpanners; }
    // wedges
    void                  setTraceWedges ()
                              { fTraceWedges = true; }
    bool                  getTraceWedges () const
                              { return fTraceWedges; }
    // slurs
    void                  setTraceSlurs ()
                              { fTraceSlurs = true; }
    bool                  getTraceSlurs () const
                              { return fTraceSlurs; }

    bool                  getTraceSlursDetails () const
                              { return fTraceSlursDetails; }
    // ligatures
    void                  setTraceLigatures ()
                              { fTraceLigatures = true; }
    bool                  getTraceLigatures () const
                              { return fTraceLigatures; }


    // credits to words
    // --------------------------------------

    // credits
    void                  setTraceCredits ()
                              { fTraceCredits = true; }
    bool                  getTraceCredits () const
                              { return fTraceCredits; }
    // lyrics
    void                  setTraceLyrics ()
                              { fTraceLyrics = true; }
    bool                  getTraceLyrics () const
                              { return fTraceLyrics; }

    bool                  getTraceLyricsDetails () const
                              { return fTraceLyricsDetails; }
    // words
    void                  setTraceWords ()
                              { fTraceWords = true; }
    bool                  getTraceWords () const
                              { return fTraceWords; }


    // chords and tuplets
    // --------------------------------------

    // chords
    void                  setTraceChords ()
                              { fTraceChords = true; }
    bool                  getTraceChords () const
                              { return fTraceChords; }

    bool                  getTraceChordsDetails () const
                              { return fTraceChordsDetails; }
    // tuplets
    void                  setTraceTuplets ()
                              { fTraceTuplets = true; }
    bool                  getTraceTuplets () const
                              { return fTraceTuplets; }

    bool                  getTraceTupletsDetails () const
                              { return fTraceTupletsDetails; }

    // inter-notes
    // --------------------------------------

    // ties
    void                  setTraceTies ()
                              { fTraceTies = true; }
    bool                  getTraceTies () const
                              { return fTraceTies; }
    // glissandos
    void                  setTraceGlissandos ()
                              { fTraceGlissandos = true; }
    bool                  getTraceGlissandos () const
                              { return fTraceGlissandos; }

    // above staff
    // --------------------------------------

    // rehearsals
    void                  setTraceRehearsals ()
                              { fTraceRehearsals = true; }
    bool                  getTraceRehearsals () const
                              { return fTraceRehearsals; }
    // segnos
    void                  setTraceSegnos ()
                              { fTraceSegnos = true; }
    bool                  getTraceSegnos () const
                              { return fTraceSegnos; }
    // dal segnos
    void                  setTraceDalSegnos ()
                              { fTraceDalSegnos = true; }
    bool                  getTraceDalSegnos () const
                              { return fTraceDalSegnos; }
    // codas
    void                  setTraceCodas ()
                              { fTraceCodas = true; }
    bool                  getTraceCodas () const
                              { return fTraceCodas; }
    // eyeglases
    void                  setTraceEyeGlasses ()
                              { fTraceEyeGlasses = true; }
    bool                  getTraceEyeGlasses () const
                              { return fTraceEyeGlasses; }

    // harmonies
    // --------------------------------------

    void                  setTraceHarmonies ()
                              { fTraceHarmonies = true; }
    bool                  getTraceHarmonies () const
                              { return fTraceHarmonies; }

    bool                  getTraceHarmoniesDetails () const
                              { return fTraceHarmoniesDetails; }

    void                  setTraceExtraHarmonies ()
                              { fTraceExtraHarmonies = true; }
    bool                  getTraceExtraHarmonies () const
                              { return fTraceExtraHarmonies; }

    // figured basses
    // --------------------------------------

    void                  setTraceFiguredBasses ()
                              { fTraceFiguredBasses = true; }
    bool                  getTraceFiguredBasses () const
                              { return fTraceFiguredBasses; }

    // instruments
    // --------------------------------------

    // frames
    void                  setTraceFrames ()
                              { fTraceFrames = true; }
    bool                  getTraceFrames () const
                              { return fTraceFrames; }
    // scordaturas
    void                  setTraceScordaturas ()
                              { fTraceScordaturas = true; }
    bool                  getTraceScordaturas () const
                              { return fTraceScordaturas; }
    // slides
    void                  setTraceSlides ()
                              { fTraceSlides = true; }
    bool                  getTraceSlides () const
                              { return fTraceSlides; }
    // pedals
    void                  setTracePedals ()
                              { fTracePedals = true; }
    bool                  getTracePedals () const
                              { return fTracePedals; }
    // accordion registrations
    void                  setTraceAccordionRegistrations ()
                              { fTraceAccordionRegistrations = true; }
    bool                  getTraceAccordionRegistrations () const
                              { return fTraceAccordionRegistrations; }
    // harp pedals
    void                  setTraceHarpPedals ()
                              { fTraceHarpPedals = true; }
    bool                  getTraceHarpPedals () const
                              { return fTraceHarpPedals; }
    // harp pedals tuning
    void                  setTraceHarpPedalsTunings ()
                              { fTraceHarpPedalsTunings = true; }
    bool                  getTraceHarpPedalsTunings () const
                              { return fTraceHarpPedalsTunings; }
    // damps
    void                  setTraceDamps ()
                              { fTraceDamps = true; }
    bool                  getTraceDamps () const
                              { return fTraceDamps; }
    // dampalls
    void                  setTraceDampAlls ()
                              { fTraceDampAlls = true; }
    bool                  getTraceDampAlls () const
                              { return fTraceDampAlls; }
    // midi
    void                  setTraceMidi ()
                              { fTraceMidi = true; }
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

    void                  printTraceOahValues (unsigned int fieldWidth);

    virtual void          printAtomWithValueOptionsValues (
                            ostream& os,
                            int      valueFieldWidth) const;

  private:

    // private services
    // ------------------------------------------------------

  private:

    // options and help trace
    // --------------------------------------

    S_oahPrefix           fShortTracePrefix;
    S_oahPrefix           fLongTracePrefix;

    bool                  fTraceOah;
    S_oahAtom             fTraceOahAtom; // to detect its use early

    bool                  fTraceOahDetails;

    // other
    // --------------------------------------

    // passes
    bool                  fTracePasses;

  public:

    // scaling
    bool                  fTraceGeometry;
    // identification
    bool                  fTraceIdentification; // specialize ??? JMI
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
    bool                  fTraceMomentsInMeasures;

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
};
typedef SMARTP<traceOahGroup> S_traceOahGroup;
EXP ostream& operator<< (ostream& os, const S_traceOahGroup& elt);

EXP extern S_traceOahGroup gGlobalTraceOahGroup;

//______________________________________________________________________________
EXP S_traceOahGroup createGlobalTraceOahGroup (
  S_oahPrefix shortTracePrefix,
  S_oahPrefix longTracePrefix);


}


#endif


#endif
