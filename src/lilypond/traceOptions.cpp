/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#include <iomanip>      // setw, setprecision, ...

#include "msr.h"

#include "traceOptions.h"


using namespace std;

namespace MusicXML2 
{

//_______________________________________________________________________________
S_traceOptions gTraceOptions;
S_traceOptions gTraceOptionsUserChoices;
S_traceOptions gTraceOptionsWithDetailedTrace;

S_traceOptions traceOptions::create (
  S_optionsHandler optionsHandler)
{
  traceOptions* o = new traceOptions (
    optionsHandler);
  assert(o!=0);

  return o;
}

traceOptions::traceOptions (
  S_optionsHandler optionsHandler)
  : optionsGroup (
    "Trace",
    "ht", "help=trace",
R"(Options that are used by various components of the library
are grouped here.)",
    optionsHandler)
{
  // append this options group to the options handler
  // if relevant
  if (optionsHandler) {
    optionsHandler->
      appendOptionsGroup (this);
  }

  // initialize it
  initializeTraceOptions (false);
}

traceOptions::~traceOptions ()
{}

void traceOptions::initializeTraceOptions (
  bool boolOptionsInitialValue)
{
  // trace and display
  // --------------------------------------

  {
    // variables  
    
    fTracePasses = boolOptionsInitialValue;
    
    fTraceOptions          = boolOptionsInitialValue;
    fDisplayOptionsValues  = boolOptionsInitialValue;
    fDisplayOptionsHandler = boolOptionsInitialValue;
    
    // fTraceDetailedMeasureNumbersSet is empty
    
    // options
    
      S_optionsSubGroup
        traceAndDisplaySubGroup =
          optionsSubGroup::create (
            "Trace and display",
            "htd", "help=trace-and-display",
R"()",
        optionsSubGroup::kAlwaysShowDescription,
        this);
    
      appendOptionsSubGroup (traceAndDisplaySubGroup);
          
      traceAndDisplaySubGroup->
        appendOptionsItem (
          optionsBooleanItem::create (
            "topts", "trace-options",
R"(Write a trace of options handling to standard error.
This option should best appear first.)",
            "traceOptions",
            fTraceOptions));
    
      traceAndDisplaySubGroup->
        appendOptionsItem (
          optionsBooleanItem::create (
            "t", "trace-passes",
R"(Write a trace of the passes to standard error.)",
            "tracePasses",
            fTracePasses));
    
      traceAndDisplaySubGroup->
        appendOptionsItem (
          optionsBooleanItem::create (
            "dov", "display-options-values",
R"(Write the chosen options values to standard error.)",
            "displayOptionsValues",
            fDisplayOptionsValues));
    
      traceAndDisplaySubGroup->
        appendOptionsItem (
          optionsBooleanItem::create (
            "doh", "display-options-handler",
R"(Write the contents of the options handler to standard error.)",
            "displayOptionsHandler",
            fDisplayOptionsHandler));
    
      traceAndDisplaySubGroup->
        appendOptionsItem (
          optionsNumbersSetItem::create (
            "tdetail", "trace-detailed",
R"('measureNumbersSet' has a form such as '0,2-14,^8-10',
where '^' excludes the corresponding numbers interval
and 0 applies to the '<part-list>' and anacrusis if present.
The measure numbers should be those of the produced score, 
since MusicXML measure numbers are arbitrary strings.
Generate a detailed trace of the activity and print additional
debugging information to standard error for the specified measures.)",
            "measureNumbersSet",
            "traceDetailedMeasureNumbersSet",
            fTraceDetailedMeasureNumbersSet));
  }


  // specific trace    
  // --------------------------------------

  {
    // variables  
  
    // encoding
    fTraceEncoding = boolOptionsInitialValue;

    // divisions
    fTraceDivisions = boolOptionsInitialValue;
  
    // geometry
    fTraceGeometry = boolOptionsInitialValue;
  
    // varValAssocs
    fTraceVarValAssocs = boolOptionsInitialValue;

    // part groups
    fTracePartGroups = boolOptionsInitialValue;
    fTracePartGroupsDetails = boolOptionsInitialValue;
    
    // parts
    fTraceParts = boolOptionsInitialValue;
    
    // staves
    fTraceStaves = boolOptionsInitialValue;
    
    // voices
    fTraceVoices = boolOptionsInitialValue;
    fTraceVoicesDetails = boolOptionsInitialValue;
  
    // clefs
    fTraceClefs = boolOptionsInitialValue;
    
    // keys
    fTraceKeys  = boolOptionsInitialValue;
    
    // times
    fTraceTimes = boolOptionsInitialValue;

    // tempos
    fTraceTempos = boolOptionsInitialValue;
  
    // rehearsals
    fTraceRehearsals = boolOptionsInitialValue;

    // transpositions
    fTraceTranspositions = boolOptionsInitialValue;
  
    // octave shifts
    fTraceOctaveShifts = boolOptionsInitialValue;

    // segments
    fTraceSegments = boolOptionsInitialValue;
    fTraceSegmentsDetails = boolOptionsInitialValue;
    
    // barlines
    fTraceBarlines = boolOptionsInitialValue;
    fTraceBarlinesDetails = boolOptionsInitialValue;

    // multiple rests
    fTraceMultipleRests = boolOptionsInitialValue;

    // repeats
    fTraceRepeats = boolOptionsInitialValue;
    fTraceRepeatsDetails = boolOptionsInitialValue;
  
    // measure repeats
    fTraceMeasureRepeats = boolOptionsInitialValue;
  
    // slashes
    fTraceSlashes = boolOptionsInitialValue;

    // measures
    fTraceMeasures = boolOptionsInitialValue;
    fTraceMeasuresDetails = boolOptionsInitialValue;
  
    // notes
    fTraceNotes = boolOptionsInitialValue;
    fTraceNotesDetails = boolOptionsInitialValue;
    
    // stems
    fTraceStems = boolOptionsInitialValue;

    // beams
    fTraceBeams = boolOptionsInitialValue;
    
    // articulations
    fTraceArticulations = boolOptionsInitialValue;
    
    // technicals
    fTraceTechnicals = boolOptionsInitialValue;
    
    // ornaments
    fTraceOrnaments = boolOptionsInitialValue;

    // dynamics
    fTraceGraceDynamics = boolOptionsInitialValue;

    // spanners
    fTraceSpanners = boolOptionsInitialValue;

    // words
    fTraceWords = boolOptionsInitialValue;
    
    // tremolos
    fTraceTremolos = boolOptionsInitialValue;
    
    // chords
    fTraceChords = boolOptionsInitialValue;
    fTraceChordsDetails = boolOptionsInitialValue;
    
    // tuplets
    fTraceTuplets = boolOptionsInitialValue;
    fTraceTupletsDetails = boolOptionsInitialValue;
    
    // glissandos
    fTraceGlissandos = boolOptionsInitialValue;
    
    // slides
    fTraceSlides = boolOptionsInitialValue;
    
    // grace notes
    fTraceGraceNotes = boolOptionsInitialValue;
  
    // lyrics
    fTraceLyrics        = boolOptionsInitialValue;
    fTraceLyricsDetails = boolOptionsInitialValue;
  
    // harmonies
    fTraceHarmonies = boolOptionsInitialValue;
  
    // frames
    fTraceFrames = boolOptionsInitialValue;

    // figured bass
    fTraceFiguredBass = boolOptionsInitialValue;
      
    // credits
    fTraceCredits = boolOptionsInitialValue;
      
    // dynamics
    fTraceDynamics = boolOptionsInitialValue;

    // ties
    fTraceTies = boolOptionsInitialValue;

    // slurs
    fTraceSlurs = boolOptionsInitialValue;

    // ligatures
    fTraceLigatures = boolOptionsInitialValue;

    // pedals
    fTracePedals = boolOptionsInitialValue;

    // wedges
    fTraceWedges = boolOptionsInitialValue;

    // staff tuning
    fTraceStaffTuning = boolOptionsInitialValue;
    
    // scordaturas
    fTraceScordaturas = boolOptionsInitialValue;
    
    // harp pedals tuning
    fTraceHarpPedalsTuning = boolOptionsInitialValue;
    
    // extra chords
    fTraceExtraChords = boolOptionsInitialValue;
    
    // msrStreams
    fTraceMsrStreams = boolOptionsInitialValue;

    // options
  
    S_optionsSubGroup
      specificTraceSubGroup =
        optionsSubGroup::create (
          "Specific trace",
          "hst", "help=specific-trace",
R"(Note: the options in this group imply '-t, -trace-passes'.)",
        optionsSubGroup::kHideDescriptionByDefault,
        this);
  
    appendOptionsSubGroup (specificTraceSubGroup);

    // encoding
    
    specificTraceSubGroup->
      appendOptionsItem (
        optionsTwoBooleansItem::create (
          "tenc", "trace-encoding",
R"(Encoding)",
          "traceEncoding",
          fTraceEncoding,
          fTracePasses));
      
    // varValAssocs
    
    specificTraceSubGroup->
      appendOptionsItem (
        optionsTwoBooleansItem::create (
          "tvvas", "trace-varv-val-assocs",
R"(VarValAssocs)",
          "traceVarValAssocs",
          fTraceVarValAssocs,
          fTracePasses));
      
    // divisions
    
   specificTraceSubGroup->
      appendOptionsItem (
        optionsTwoBooleansItem::create (
          "tdivs", "trace-divisions",
R"(Divisions)",
          "traceDivisions",
          fTraceDivisions,
          fTracePasses));
      
    // geometry
    
    specificTraceSubGroup->
      appendOptionsItem (
        optionsTwoBooleansItem::create (
          "tgeom", "trace-geometry",
R"(Geometry)",
          "traceGeometry",
          fTraceGeometry,
          fTracePasses));

    // part groups
    
    specificTraceSubGroup->
      appendOptionsItem (
        optionsTwoBooleansItem::create (
          "tpgrps", "trace-partgroups",
R"(Part groups)",
          "tracePartGroups",
          fTracePartGroups,
          fTracePasses));
      
    specificTraceSubGroup->
      appendOptionsItem (
        optionsThreeBooleansItem::create (
          "tpgrpsd", "trace-partgroups-details",
R"(Part groups with more details.
This option implies '-tpgrps, -tracePartGroups'.)",
          "tracePartGroupsDetails",
          fTracePartGroupsDetails,
          fTracePartGroups,
          fTracePasses));
      
    // parts
    
    specificTraceSubGroup->
      appendOptionsItem (
        optionsTwoBooleansItem::create (
          "tparts", "trace-parts",
R"(Parts)",
          "traceParts",
          fTraceParts,
          fTracePasses));
      
    // staves
    
    specificTraceSubGroup->
      appendOptionsItem (
        optionsTwoBooleansItem::create (
          "tstaves", "trace-staves",
R"(Staves)",
          "traceStaves",
          fTraceStaves,
          fTracePasses));
      
    // voices
    
    specificTraceSubGroup->
      appendOptionsItem (
        optionsTwoBooleansItem::create (
          "tvoices", "trace-voices",
R"(Voices)",
          "traceVoices",
          fTraceVoices,
          fTracePasses));
      
    specificTraceSubGroup->
      appendOptionsItem (
        optionsThreeBooleansItem::create (
          "tvoicesd", "trace-voices-details",
R"(Voices with more details (quite verbose)..
This option implies '-tvdetails, -traceVoicesDetails'.)",
          "traceVoicesDetails",
          fTraceVoicesDetails,
          fTraceVoices,
          fTracePasses));
      
    // clefs
    
    specificTraceSubGroup->
      appendOptionsItem (
        optionsTwoBooleansItem::create (
          "tclefs", "trace-clefs",
R"(Clefs)",
          "traceClefs",
          fTraceClefs,
          fTracePasses));
      
    // keys
    
    specificTraceSubGroup->
      appendOptionsItem (
        optionsTwoBooleansItem::create (
          "tkeys", "trace-keys",
R"(Keys)",
          "traceKeys",
          fTraceKeys,
          fTracePasses));
      
    // times
    
    specificTraceSubGroup->
      appendOptionsItem (
        optionsTwoBooleansItem::create (
          "ttimes", "trace-times",
R"(Times)",
          "traceTimes",
          fTraceTimes,
          fTracePasses));

    // tempos
    
    specificTraceSubGroup->
      appendOptionsItem (
        optionsTwoBooleansItem::create (
          "ttempos", "trace-tempos",
R"(Tempos)",
          "traceTempos",
          fTraceTempos,
          fTracePasses));
      
    // rehearsals
    
    specificTraceSubGroup->
      appendOptionsItem (
        optionsTwoBooleansItem::create (
          "trehears", "trace-rehearsals",
R"(Rehearsals)",
          "traceRehearsals",
          fTraceRehearsals,
          fTracePasses));

    // transpositions
    
    specificTraceSubGroup->
      appendOptionsItem (
        optionsTwoBooleansItem::create (
          "ttransps", "trace-transpositions",
R"(Transpositions (<transpose/> in MusicXML, \transposition in LilyPond))",
          "traceTranspositions",
          fTraceTranspositions,
          fTracePasses));
      
    // octave shifts
    
    specificTraceSubGroup->
      appendOptionsItem (
        optionsTwoBooleansItem::create (
          "toctshifts", "trace-octaves-shifts",
R"(Octave shifts (<octave-shift/> in MusicXML, \ottava in LilyPond))",
          "traceOctaveShifts",
          fTraceOctaveShifts,
          fTracePasses));
      
    // segments
    
    specificTraceSubGroup->
      appendOptionsItem (
        optionsTwoBooleansItem::create (
          "tsegs", "trace-segments",
R"(Voices segments)",
          "traceSegments",
          fTraceSegments,
          fTracePasses));
      
    specificTraceSubGroup->
      appendOptionsItem (
        optionsThreeBooleansItem::create (
          "tsegsd", "trace-segments-details",
R"(Voices segments details)",
          "traceSegments",
          fTraceSegmentsDetails,
          fTraceSegments,
          fTracePasses));
      
    // barlines
    
    specificTraceSubGroup->
      appendOptionsItem (
        optionsTwoBooleansItem::create (
          "tbars", "trace-barlines",
R"(Barlines)",
          "traceBarlines",
          fTraceBarlines,
          fTracePasses));
      
    specificTraceSubGroup->
      appendOptionsItem (
        optionsThreeBooleansItem::create (
          "tbarsd", "trace-barlines-details",
R"(Barlines details)",
          "traceBarlinesDetails",
          fTraceBarlinesDetails,
          fTraceBarlines,
          fTracePasses));
      
    // multiple rests
    
    specificTraceSubGroup->
      appendOptionsItem (
        optionsTwoBooleansItem::create (
          "tmrests", "trace-multiple-rests",
R"(Multiple rests)",
          "traceMultipleRests",
          fTraceMultipleRests,
          fTracePasses));
      
    // repeats
    
    specificTraceSubGroup->
      appendOptionsItem (
        optionsTwoBooleansItem::create (
          "treps", "trace-repeats",
R"(Repeats)",
          "traceRepeats",
          fTraceRepeats,
          fTracePasses));
      
    specificTraceSubGroup->
      appendOptionsItem (
        optionsThreeBooleansItem::create (
          "trepsd", "trace-repeats-details",
R"(Repeats details)",
          "traceRepeats",
          fTraceRepeatsDetails,
          fTraceRepeats,
          fTracePasses));
      
    // measure repeats
    
    specificTraceSubGroup->
      appendOptionsItem (
        optionsTwoBooleansItem::create (
          "tmreps", "trace-measure-repeats",
R"(Measure repeats)",
          "traceMeasureRepeats",
          fTraceMeasureRepeats,
          fTracePasses));
      
    // slashes
    
    specificTraceSubGroup->
      appendOptionsItem (
        optionsTwoBooleansItem::create (
          "tslashes", "trace-slashes",
R"(Slashes)",
          "traceSlashes",
          fTraceSlashes,
          fTracePasses));
      
    // measures
    
    specificTraceSubGroup->
      appendOptionsItem (
        optionsTwoBooleansItem::create (
          "tmeas", "trace-measures",
R"(Measures)",
          "traceMeasures",
          fTraceMeasures,
          fTracePasses));
      
    specificTraceSubGroup->
      appendOptionsItem (
        optionsThreeBooleansItem::create (
          "tmeasd", "trace-measures-details",
R"(Measures details)",
          "traceMeasures",
          fTraceMeasuresDetails,
          fTraceMeasures,
          fTracePasses));
      
    // notes
    
    specificTraceSubGroup->
      appendOptionsItem (
        optionsTwoBooleansItem::create (
          "tnotes", "trace-notes",
R"(Notes)",
          "traceNotes",
          fTraceNotes,
          fTracePasses));
          
    specificTraceSubGroup->
      appendOptionsItem (
        optionsThreeBooleansItem::create (
          "tnotesd", "trace-notes-details",
R"(Notes with more details, including divisions handling (quite verbose)..
This option implies '-tnnotes, -traceNotes'.)",
          "traceNotesDetails",
          fTraceNotesDetails,
          fTraceNotes,
          fTracePasses));
      
    // stems
    
    specificTraceSubGroup->
      appendOptionsItem (
        optionsTwoBooleansItem::create (
          "tstems", "trace-stems",
R"(Stems)",
          "traceStems",
          fTraceStems,
          fTracePasses));
      
    // beams
    
    specificTraceSubGroup->
      appendOptionsItem (
        optionsTwoBooleansItem::create (
          "tbeams", "trace-beams",
R"(Beams)",
          "traceBeams",
          fTraceBeams,
          fTracePasses));
      
    // articulations
    
    specificTraceSubGroup->
      appendOptionsItem (
        optionsTwoBooleansItem::create (
          "tarts", "trace-articulations",
R"(Articulations)",
          "traceArticulations",
          fTraceArticulations,
          fTracePasses));
      
    // technicals
    
    specificTraceSubGroup->
      appendOptionsItem (
        optionsTwoBooleansItem::create (
          "ttechs", "trace-technicals",
R"(Technicals)",
          "traceTechnicals",
          fTraceTechnicals,
          fTracePasses));
      
    // ornaments
    
    specificTraceSubGroup->
      appendOptionsItem (
        optionsTwoBooleansItem::create (
          "torns", "trace-ornaments",
R"(Ornaments)",
          "traceOrnaments",
          fTraceOrnaments,
          fTracePasses));
      
    // dynamics
    
    specificTraceSubGroup->
      appendOptionsItem (
        optionsTwoBooleansItem::create (
          "tdyns", "trace-graceDynamics",
R"(Dynamics)",
          "traceGraceDynamics",
          fTraceGraceDynamics,
          fTracePasses));
      
    // spanners
    
    specificTraceSubGroup->
      appendOptionsItem (
        optionsTwoBooleansItem::create (
          "tspans", "trace-spanners",
R"(Spanners)",
          "traceSpanners",
          fTraceSpanners,
          fTracePasses));
      
    // words
    
    specificTraceSubGroup->
      appendOptionsItem (
        optionsTwoBooleansItem::create (
          "twords", "trace-words",
R"(Words)",
          "traceWords",
          fTraceWords,
          fTracePasses));
      
    // tremolos
    
    specificTraceSubGroup->
      appendOptionsItem (
        optionsTwoBooleansItem::create (
          "ttrems", "trace-tremolos",
R"(Tremolos)",
          "traceTremolos",
          fTraceTremolos,
          fTracePasses));
      
    // shords
    
    specificTraceSubGroup->
      appendOptionsItem (
        optionsTwoBooleansItem::create (
          "tchords", "trace-chords",
R"(Chords)",
          "traceChords",
          fTraceChords,
          fTracePasses));
      
    specificTraceSubGroup->
      appendOptionsItem (
        optionsThreeBooleansItem::create (
          "tchordsd", "trace-chords-details",
R"(Chords details)",
          "traceChordsDetails",
          fTraceChordsDetails,
          fTraceChords,
          fTracePasses));
      
    // tuplets
    
    specificTraceSubGroup->
      appendOptionsItem (
        optionsTwoBooleansItem::create (
          "ttups", "trace-tuplets",
R"(Tuplets)",
          "traceTuplets",
          fTraceTuplets,
          fTracePasses));
      
    specificTraceSubGroup->
      appendOptionsItem (
        optionsThreeBooleansItem::create (
          "ttupsd", "trace-tuplets-details",
R"(Tuplets details)",
          "traceTupletsDetails",
          fTraceTupletsDetails,
          fTraceTuplets,
          fTracePasses));
      
    // glissandos
    
    specificTraceSubGroup->
      appendOptionsItem (
        optionsTwoBooleansItem::create (
          "tgliss", "trace-glissandos",
R"(Glissandos)",
          "traceGlissandos",
          fTraceGlissandos,
          fTracePasses));
      
    // slides
    
    specificTraceSubGroup->
      appendOptionsItem (
        optionsTwoBooleansItem::create (
          "tslides", "trace-slides",
R"(Slides)",
          "traceSlides",
          fTraceSlides,
          fTracePasses));
      
    // grace notes
    
    specificTraceSubGroup->
      appendOptionsItem (
        optionsTwoBooleansItem::create (
          "tgraces", "trace-gracenotes",
R"(Grace notes)",
          "traceGraceNotes",
          fTraceGraceNotes,
          fTracePasses));
      
    // lyrics
    
    specificTraceSubGroup->
      appendOptionsItem (
        optionsTwoBooleansItem::create (
          "tlyrics", "trace-lyrics",
R"(Lyrics in MusicXML, stanzas in MSR)",
          "traceLyrics",
          fTraceLyrics,
          fTracePasses));
      
    specificTraceSubGroup->
      appendOptionsItem (
        optionsTwoBooleansItem::create (
          "tlyricsd", "trace-lyrics-details",
R"(Lyrics in MusicXML, stanzas in MSR)",
          "traceLyricsDetails",
          fTraceLyricsDetails,
          fTracePasses));
      
    // harmonies
    
    specificTraceSubGroup->
      appendOptionsItem (
        optionsTwoBooleansItem::create (
          "tharms", "trace-harmonies",
R"(<harmony/> in MusicXML, \chordmode in LilyPond)",
          "traceHarmonies",
          fTraceHarmonies,
          fTracePasses));
      
    // frames
    
    specificTraceSubGroup->
      appendOptionsItem (
        optionsTwoBooleansItem::create (
          "tFrames", "trace-frames",
R"(<frame/> in MusicXML, \fret-diagram in LilyPond)",
          "traceFrames",
          fTraceFrames,
          fTracePasses));
      
    // figured bass
    
    specificTraceSubGroup->
      appendOptionsItem (
        optionsTwoBooleansItem::create (
          "tfigbass", "trace-figured-bass",
R"(<figured-bass> in MusicXML, \figuremode in LilyPond)",
          "traceFiguredBass",
          fTraceFiguredBass,
          fTracePasses));
      
    // credits
    
    specificTraceSubGroup->
      appendOptionsItem (
        optionsTwoBooleansItem::create (
          "tcredits", "trace-credits",
R"(Credits)",
          "traceCredits",
          fTraceCredits,
          fTracePasses));
      
    // dynamics
    
    specificTraceSubGroup->
      appendOptionsItem (
        optionsTwoBooleansItem::create (
          "tdynamics", "trace-cynamics",
R"(Dynamics)",
          "traceDynamics",
          fTraceDynamics,
          fTracePasses));
      
    // slurs
    
    specificTraceSubGroup->
      appendOptionsItem (
        optionsTwoBooleansItem::create (
          "tties", "trace-ties",
R"(Ties)",
          "traceTies",
          fTraceTies,
          fTracePasses));
      
    specificTraceSubGroup->
      appendOptionsItem (
        optionsTwoBooleansItem::create (
          "tslurs", "trace-slurs",
R"(Slurs)",
          "traceSlurs",
          fTraceSlurs,
          fTracePasses));
      
    // ligatures
    
    specificTraceSubGroup->
      appendOptionsItem (
        optionsTwoBooleansItem::create (
          "tligs", "traceLigatures",
R"(Ligatures)",
          "traceLigatures",
          fTraceLigatures,
          fTracePasses));
      
    // wedges
    
    specificTraceSubGroup->
      appendOptionsItem (
        optionsTwoBooleansItem::create (
          "twedges", "trace-wedges",
R"(Wedges)",
          "traceWedges",
          fTraceWedges,
          fTracePasses));
      
    // pedals
    
    specificTraceSubGroup->
      appendOptionsItem (
        optionsTwoBooleansItem::create (
          "tpedals", "trace-pedals",
R"(Pedals)",
          "tracePedals",
          fTracePedals,
          fTracePasses));
      
    // staff tuning
    
    specificTraceSubGroup->
      appendOptionsItem (
        optionsTwoBooleansItem::create (
          "trace-staff-tuning", "",
R"(Staff tuning)",
          "traceStaffTuning",
          fTraceStaffTuning,
          fTracePasses));
      
    // scordaturas
    
    specificTraceSubGroup->
      appendOptionsItem (
        optionsTwoBooleansItem::create (
          "trace-scordaturas", "",
R"(Scordaturas)",
          "traceScordaturas",
          fTraceScordaturas,
          fTracePasses));
      
    // pedal tunings
    
    specificTraceSubGroup->
      appendOptionsItem (
        optionsTwoBooleansItem::create (
          "traceHarpPedalsTuning", "",
R"(Harp pedals tuning)",
          "traceHarpPedalsTuning",
          fTraceHarpPedalsTuning,
          fTracePasses));
      
    // extra chords
    
    specificTraceSubGroup->
      appendOptionsItem (
        optionsTwoBooleansItem::create (
          "tec", "trace-extra-chords",
R"(Extra chords handling)",
          "traceExtraChords",
          fTraceExtraChords,
          fTracePasses));

    /* JMI
    // MSR streams
    
    specificTraceSubGroup->
      appendOptionsItem (
        optionsTwoBooleansItem::create (
          "tms", "traceMsrStreams",
R"(MSR Streams API for applications)",
          "traceMsrStreams",
          fTraceMsrStreams,
          fTracePasses));
    */
  }
      

  fTraceDynamics = boolOptionsInitialValue;
  fTraceWords = boolOptionsInitialValue;
  fTraceTies = boolOptionsInitialValue;
  fTraceSlurs = boolOptionsInitialValue;
  fTraceLigatures = boolOptionsInitialValue;
  fTracePedals = boolOptionsInitialValue;
  fTraceWedges = boolOptionsInitialValue;
  
  fTraceStaffTuning = boolOptionsInitialValue;
  fTraceScordaturas = boolOptionsInitialValue;
  

  /* STUFF not yet handled JMI */

  fTraceScore = boolOptionsInitialValue;


  fTraceMidi = boolOptionsInitialValue;
}

S_traceOptions traceOptions::createCloneWithDetailedTrace ()
{
  S_traceOptions
    clone =
      traceOptions::create (0);
      // 0 not to have it inserted twice in the option handler

  // set the options handler uplink
  clone->
    setOptionsHandlerUplink (
      fOptionsHandlerUplink);


  // trace and display
  // --------------------------------------

  clone->fTracePasses = true;
    
  clone->fTraceDetailedMeasureNumbersSet =
    fTraceDetailedMeasureNumbersSet;


  // specific trace
  // --------------------------------------

    // encoding
  clone->fTraceEncoding = true;

  // divisions
  clone->fTraceDivisions = true;

  // geometry
  clone->fTraceGeometry = true;

  // varValAssocs
  clone->fTraceVarValAssocs = true;

  // part groups
  clone->fTracePartGroups = true;
  clone->fTracePartGroupsDetails = true;
  
  // parts
  clone->fTraceParts = true;
  
  // staves
  clone->fTraceStaves = true;
  
  // voices
  clone->fTraceVoices = true;
  clone->fTraceVoicesDetails = true;

  // clefs
  clone->fTraceClefs = true;
  
  // keys
  clone->fTraceKeys = true;
  
  // times
  clone->fTraceTimes = true;

  // tempos
  clone->fTraceTempos = true;

  // rehearsals
  clone->fTraceRehearsals = true;

  // transpositions
  clone->fTraceTranspositions = true;

  // octave shifts
  clone->fTraceOctaveShifts = true;

  // segments
  clone->fTraceSegments = true;
  clone->fTraceSegmentsDetails = true;
    
  // barlines
  clone->fTraceBarlines = true;
  clone->fTraceBarlinesDetails = true;

  // multiple rests
  clone->fTraceMultipleRests = true;

  // repeats
  clone->fTraceRepeats = true;
  clone->fTraceRepeatsDetails = true;

  // measure repeats
  clone->fTraceMeasureRepeats = true;

  // slashes
  clone->fTraceSlashes = true;

  // measures
  clone->fTraceMeasures = true;
  clone->fTraceMeasuresDetails = true;
  
  // notes
  clone->fTraceNotes = true;
  clone->fTraceNotesDetails = true;
    
  // stems
  clone->fTraceStems = true;

  // beams
  clone->fTraceBeams = true;
    
  // articulations
  clone->fTraceArticulations = true;
  
  // technicals
  clone->fTraceTechnicals = true;
  
  // ornaments
  clone->fTraceOrnaments = true;
  
  // dynamics
  clone->fTraceGraceDynamics = true;
  
  // spanners
  clone->fTraceSpanners = true;
  
  // words
  clone->fTraceWords = true;
  
  // tremolos
  clone->fTraceTremolos = true;

  // chords
  clone->fTraceChords = true;
  clone->fTraceChordsDetails = true;
    
  // tuplets
  clone->fTraceTuplets = true;
  clone->fTraceTupletsDetails = true;
  
  // grace notes
  clone->fTraceGraceNotes = true;

  // lyrics
  clone->fTraceLyrics        = true;
  clone->fTraceLyricsDetails = true;
  
  // harmonies
  clone->fTraceHarmonies = true;
  
  // frames
  clone->fTraceFrames = true;

  // figured bass
  clone->fTraceFiguredBass = true;
    
  // credits
  clone->fTraceCredits = true;

  // dynamics
  clone->fTraceDynamics = true;

  // ties
  clone->fTraceTies = true;

  // slurs
  clone->fTraceSlurs = true;

  // ligatures
  clone->fTraceLigatures = true;

  // pedals
  clone->fTracePedals = true;

  // wedges
  clone->fTraceWedges = true;
  
  // stafftuning
  clone->fTraceStaffTuning = true;

  // scordaturas
  clone->fTraceScordaturas = true;

    
  /* STUFF not yet handled JMI */

  clone->fTraceScore = true;

  // midi
  clone->fTraceMidi = true;

  return clone;
}

void traceOptions::setAllTraceOptions (
  bool boolOptionsInitialValue)
{
  // trace and display

  fTracePasses = boolOptionsInitialValue;
  
  fTraceOptions          = boolOptionsInitialValue;
  fDisplayOptionsValues  = boolOptionsInitialValue;
  fDisplayOptionsHandler = boolOptionsInitialValue;

  // specific trace    

    // encoding
    fTraceEncoding = boolOptionsInitialValue;

    // divisions
    fTraceDivisions = boolOptionsInitialValue;
  
    // geometry
    fTraceGeometry = boolOptionsInitialValue;
  
    // varValAssocs
    fTraceVarValAssocs = boolOptionsInitialValue;

    // part groups
    fTracePartGroups = boolOptionsInitialValue;
    fTracePartGroupsDetails = boolOptionsInitialValue;
    
    // parts
    fTraceParts = boolOptionsInitialValue;
    
    // staves
    fTraceStaves = boolOptionsInitialValue;
    
    // voices
    fTraceVoices = boolOptionsInitialValue;
    fTraceVoicesDetails = boolOptionsInitialValue;
  
    // clefs
    fTraceClefs = boolOptionsInitialValue;
    
    // keys
    fTraceKeys  = boolOptionsInitialValue;
    
    // times
    fTraceTimes = boolOptionsInitialValue;

    // tempos
    fTraceTempos = boolOptionsInitialValue;
  
    // rehearsals
    fTraceRehearsals = boolOptionsInitialValue;

    // transpositions
    fTraceTranspositions = boolOptionsInitialValue;
  
    // octave shifts
    fTraceOctaveShifts = boolOptionsInitialValue;

    // segments
    fTraceSegments = boolOptionsInitialValue;
    fTraceSegmentsDetails = boolOptionsInitialValue;
    
    // barlines
    fTraceBarlines = boolOptionsInitialValue;
    fTraceBarlinesDetails = boolOptionsInitialValue;

    // multiple rests
    fTraceMultipleRests = boolOptionsInitialValue;

    // repeats
    fTraceRepeats = boolOptionsInitialValue;
    fTraceRepeatsDetails = boolOptionsInitialValue;
  
    // measure repeats
    fTraceMeasureRepeats = boolOptionsInitialValue;
  
    // slashes
    fTraceSlashes = boolOptionsInitialValue;

    // measures
    fTraceMeasures = boolOptionsInitialValue;
    fTraceMeasuresDetails = boolOptionsInitialValue;
  
    // notes
    fTraceNotes = boolOptionsInitialValue;
    fTraceNotesDetails = boolOptionsInitialValue;
    
    // stems
    fTraceStems = boolOptionsInitialValue;

    // beams
    fTraceBeams = boolOptionsInitialValue;
    
    // articulations
    fTraceArticulations = boolOptionsInitialValue;
    
    // technicals
    fTraceTechnicals = boolOptionsInitialValue;
    
    // ornaments
    fTraceOrnaments = boolOptionsInitialValue;

    // dynamics
    fTraceGraceDynamics = boolOptionsInitialValue;

    // spanners
    fTraceSpanners = boolOptionsInitialValue;

    // words
    fTraceWords = boolOptionsInitialValue;
    
    // tremolos
    fTraceTremolos = boolOptionsInitialValue;
    
    // chords
    fTraceChords = boolOptionsInitialValue;
    fTraceChordsDetails = boolOptionsInitialValue;
    
    // tuplets
    fTraceTuplets = boolOptionsInitialValue;
    fTraceTupletsDetails = boolOptionsInitialValue;
    
    // glissandos
    fTraceGlissandos = boolOptionsInitialValue;
    
    // slides
    fTraceSlides = boolOptionsInitialValue;
    
    // grace notes
    fTraceGraceNotes = boolOptionsInitialValue;
  
    // lyrics
    fTraceLyrics        = boolOptionsInitialValue;
    fTraceLyricsDetails = boolOptionsInitialValue;
  
    // harmonies
    fTraceHarmonies = boolOptionsInitialValue;
  
    // frames
    fTraceFrames = boolOptionsInitialValue;

    // figured bass
    fTraceFiguredBass = boolOptionsInitialValue;
      
    // credits
    fTraceCredits = boolOptionsInitialValue;
      
    // dynamics
    fTraceDynamics = boolOptionsInitialValue;

    // ties
    fTraceTies = boolOptionsInitialValue;

    // slurs
    fTraceSlurs = boolOptionsInitialValue;

    // ligatures
    fTraceLigatures = boolOptionsInitialValue;

    // pedals
    fTracePedals = boolOptionsInitialValue;

    // wedges
    fTraceWedges = boolOptionsInitialValue;

    // staff tuning
    fTraceStaffTuning = boolOptionsInitialValue;
    
    // scordaturas
    fTraceScordaturas = boolOptionsInitialValue;
    
    // harp pedals tuning
    fTraceHarpPedalsTuning = boolOptionsInitialValue;
    
    // extra chords
    fTraceExtraChords = boolOptionsInitialValue;
    
    // msrStreams
    fTraceMsrStreams = boolOptionsInitialValue;


  fTraceDynamics = boolOptionsInitialValue;
  fTraceWords = boolOptionsInitialValue;
  fTraceTies = boolOptionsInitialValue;
  fTraceSlurs = boolOptionsInitialValue;
  fTraceLigatures = boolOptionsInitialValue;
  fTracePedals = boolOptionsInitialValue;
  fTraceWedges = boolOptionsInitialValue;
  
  fTraceStaffTuning = boolOptionsInitialValue;
  fTraceScordaturas = boolOptionsInitialValue;
  

  /* STUFF not yet handled JMI */

  fTraceScore = boolOptionsInitialValue;


  fTraceMidi = boolOptionsInitialValue;  
}

//______________________________________________________________________________
void traceOptions::enforceQuietness ()
{
  setAllTraceOptions (false);
}

//______________________________________________________________________________
void traceOptions::checkOptionsConsistency ()
{
  // JMI
}

//______________________________________________________________________________
void traceOptions::printTraceOptionsValues (int fieldWidth)
{  
  gLogIOstream <<
    "The trace options are:" <<
    endl;

  gIndenter++;

/* JMI
  // command line
  // --------------------------------------

  gLogIOstream << left <<

  gIndenter++;

  gLogIOstream << left <<
    setw (fieldWidth) << "input source name" << " : " <<
    fInputSourceName <<
    endl <<
      
    setw (fieldWidth) << "translation date" << " : " <<
    fTranslationDate <<
    endl;

  gIndenter--;

  // output file
  // --------------------------------------

  gLogIOstream << left <<
    setw (fieldWidth) << "Output file:" <<
    endl;

  gIndenter++;

  gLogIOstream << left <<        
    setw (fieldWidth) << "inputSourceName" << " : \"" <<
    fInputSourceName <<
    "\"" <<
    endl <<
    setw (fieldWidth) << "outputFileName" << " : \"" <<
    fOutputFileName <<
    "\"" <<
    endl;

  gIndenter--;
*/

  // trace and display
  // --------------------------------------

  gLogIOstream << left <<
    setw (fieldWidth) << "Trace and display:" <<
    endl;

  gIndenter++;

  gLogIOstream << left <<
    setw (fieldWidth) << "tracePasses" << " : " <<
    booleanAsString (fTracePasses) <<
     endl <<
    setw (fieldWidth) << "traceDetailedMeasureNumbersSet" << " : ";
        
  if (! fTraceDetailedMeasureNumbersSet.size ()) {
    gLogIOstream << "none";
  }
    
  else {
    gLogIOstream <<
      "'";
      
    set<int>::const_iterator
      iBegin = fTraceDetailedMeasureNumbersSet.begin (),
      iEnd   = fTraceDetailedMeasureNumbersSet.end (),
      i      = iBegin;
      
    for ( ; ; ) {
      gLogIOstream << (*i);
      if (++i == iEnd) break;
      gLogIOstream << " ";
    } // for
  
    gLogIOstream <<
      "'";
  }
    
  gLogIOstream <<
    endl;

  gIndenter--;
  

  // specific trace    
  // --------------------------------------

  gLogIOstream << left <<
    setw (fieldWidth) << "Specific trace:" <<
    endl;

  gIndenter++;

  gLogIOstream << left <<
    // fTraceEncoding
    setw (fieldWidth) << "traceEncoding" << " : " <<
    booleanAsString (fTraceEncoding) <<
    endl <<
      
    // divisions
    setw (fieldWidth) << "traceDivisions" << " : " <<
    booleanAsString (fTraceDivisions) <<
    endl <<
      
    // geometry
    setw (fieldWidth) << "traceGeometry" << " : " <<
    booleanAsString (fTraceGeometry) <<
    endl <<
      
    // varValAssocs
    // geometry
    setw (fieldWidth) << "traceVarValAssocs" << " : " <<
    booleanAsString (fTraceVarValAssocs) <<
    endl <<
      
    // part groups
    setw (fieldWidth) << "tracePartGroups" << " : " <<
    booleanAsString (fTracePartGroups) <<
    endl <<
    setw (fieldWidth) << "tracePartGroupsDetails" << " : " <<
    booleanAsString (fTracePartGroupsDetails) <<
    endl <<
    
    // parts
    setw (fieldWidth) << "traceParts" << " : " <<
    booleanAsString (fTraceParts) <<
    endl <<
    
    // staves
    setw (fieldWidth) << "traceStaves" << " : " <<
    booleanAsString (fTraceStaves) <<
    endl <<
    
    // voices
    setw (fieldWidth) << "traceVoices" << " : " <<
    booleanAsString (fTraceVoices) <<
    endl <<
    setw (fieldWidth) << "traceVoicesDetails" << " : " <<
    booleanAsString (fTraceVoicesDetails) <<
    endl <<

    // clefs
    setw (fieldWidth) << "traceClefs" << " : " <<
    booleanAsString (fTraceClefs) <<
    endl <<
    
    // keys
    setw (fieldWidth) << "traceKeys" << " : " <<
    booleanAsString (fTraceKeys) <<
    endl <<
    
    // times
    setw (fieldWidth) << "traceTimes" << " : " <<
    booleanAsString (fTraceTimes) <<
    endl <<
    
    // tempos
    setw (fieldWidth) << "traceTempos" << " : " <<
    booleanAsString (fTraceTempos) <<
    endl <<
    
    // rehearsals
    setw (fieldWidth) << "traceRehearsals" << " : " <<
    booleanAsString (fTraceRehearsals) <<
    endl <<

    // transpositions
    setw (fieldWidth) << "traceTranspositions" << " : " <<
    booleanAsString (fTraceTranspositions) <<
    endl <<
    
    // octave shifts
    setw (fieldWidth) << "traceOctaveShifts" << " : " <<
    booleanAsString (fTraceOctaveShifts) <<
    endl <<
    
    // segments
    setw (fieldWidth) << "traceSegments" << " : " <<
    booleanAsString (fTraceSegments) <<
    endl <<
    setw (fieldWidth) << "traceSegmentsDetails" << " : " <<
    booleanAsString (fTraceSegmentsDetails) <<
    endl <<
    
    // barlines
    setw (fieldWidth) << "traceBarlines" << " : " <<
    booleanAsString (fTraceBarlines) <<
    endl <<
    setw (fieldWidth) << "traceBarlinesDetails" << " : " <<
    booleanAsString (fTraceBarlinesDetails) <<
    endl <<
    
    // multiple rests
    setw (fieldWidth) << "traceMultipleRests" << " : " <<
    booleanAsString (fTraceMultipleRests) <<
    endl <<

    // repeats
    setw (fieldWidth) << "traceRepeats" << " : " <<
    booleanAsString (fTraceRepeats) <<
    endl <<
    setw (fieldWidth) << "traceRepeatsDetails" << " : " <<
    booleanAsString (fTraceRepeatsDetails) <<
    endl <<
    
    // measure repeats
    setw (fieldWidth) << "traceMeasureRepeats" << " : " <<
    booleanAsString (fTraceMeasureRepeats) <<
    endl <<

    // slashes
    setw (fieldWidth) << "traceSlashes" << " : " <<
    booleanAsString (fTraceSlashes) <<
    endl <<
    
    // measures
    setw (fieldWidth) << "traceMeasures" << " : " <<
    booleanAsString (fTraceMeasures) <<
    endl <<
    setw (fieldWidth) << "traceMeasuresDetails" << " : " <<
    booleanAsString (fTraceMeasuresDetails) <<
    endl <<
    
    // notes
    setw (fieldWidth) << "traceNotes" << " : " <<
    booleanAsString (fTraceNotes) <<
    endl <<
    setw (fieldWidth) << "traceNotesDetails" << " : " <<
    booleanAsString (fTraceNotesDetails) <<
    endl <<
    
    // stems
    setw (fieldWidth) << "traceStems" << " : " <<
    booleanAsString (fTraceStems) <<
    endl <<
    
    // beams
    setw (fieldWidth) << "traceBeams" << " : " <<
    booleanAsString (fTraceBeams) <<
    endl <<
    
    // articulations
    setw (fieldWidth) << "traceArticulations" << " : " <<
    booleanAsString (fTraceArticulations) <<
    endl <<
    
    // technicals
    setw (fieldWidth) << "traceTechnicals" << " : " <<
    booleanAsString (fTraceTechnicals) <<
    endl <<
    
    // ornaments
    setw (fieldWidth) << "traceOrnaments" << " : " <<
    booleanAsString (fTraceOrnaments) <<
    endl <<
    
    // dynamics
    setw (fieldWidth) << "traceGraceDynamics" << " : " <<
    booleanAsString (fTraceGraceDynamics) <<
    endl <<
    
    // spanners
    setw (fieldWidth) << "traceSpanners" << " : " <<
    booleanAsString (fTraceSpanners) <<
    endl <<
    
    // words
    setw (fieldWidth) << "traceWords" << " : " <<
    booleanAsString (fTraceWords) <<
    endl <<
    
    // tremolos
    setw (fieldWidth) << "traceTremolos" << " : " <<
    booleanAsString (fTraceTremolos) <<
    endl <<
    
    // chords
    setw (fieldWidth) << "traceChords" << " : " <<
    booleanAsString (fTraceChords) <<
    endl <<
    setw (fieldWidth) << "traceChordsDetails" << " : " <<
    booleanAsString (fTraceChordsDetails) <<
    endl <<
    
    // tuplets
    setw (fieldWidth) << "traceTuplets" << " : " <<
    booleanAsString (fTraceTuplets) <<
    endl <<
    setw (fieldWidth) << "traceTupletsDetails" << " : " <<
    booleanAsString (fTraceTupletsDetails) <<
    endl <<
    
    // grace notes
    setw (fieldWidth) << "traceGraceNotes" << " : " <<
    booleanAsString (fTraceGraceNotes) <<
    endl <<

    // lyrics
    setw (fieldWidth) << "traceLyrics" << " : " <<
    booleanAsString (fTraceLyrics) <<
    endl <<
    setw (fieldWidth) << "traceLyricsDetails" << " : " <<
    booleanAsString (fTraceLyricsDetails) <<
    endl <<
    
    // harmonies
    setw (fieldWidth) << "traceHarmonies" << " : " <<
    booleanAsString (fTraceHarmonies) <<
    endl <<
    
    // frames
    setw (fieldWidth) << "traceFrames" << " : " <<
    booleanAsString (fTraceFrames) <<
    endl <<
    
    // figured bass
    setw (fieldWidth) << "traceFiguredBass" << " : " <<
    booleanAsString (fTraceFiguredBass) <<
    endl <<
    
    // credits
    setw (fieldWidth) << "traceCredits" << " : " <<
    booleanAsString (fTraceCredits) <<
    endl;

  gIndenter--;

  gIndenter--;
}

S_optionsItem traceOptions::handleOptionsItem (
  ostream&      os,
  S_optionsItem item)
{
  S_optionsItem result;
  
  return result;
}

ostream& operator<< (ostream& os, const S_traceOptions& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
void initializeTraceOptionsHandling (
  S_optionsHandler optionsHandler)
{  
  // create the options variables
  // ------------------------------------------------------
  
  gTraceOptionsUserChoices = traceOptions::create (
    optionsHandler);
  assert(gTraceOptionsUserChoices != 0);

  gTraceOptions =
    gTraceOptionsUserChoices;

  // prepare for measure detailed trace
  // ------------------------------------------------------

  gTraceOptionsWithDetailedTrace =
    gTraceOptions->
      createCloneWithDetailedTrace ();
}


}
