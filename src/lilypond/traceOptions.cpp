/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#include "traceOptions.h"

#ifdef TRACE_OPTIONS


#include <iomanip>      // setw, setprecision, ...

#include "generalOptions.h"

using namespace std;

namespace MusicXML2
{

//_______________________________________________________________________________
S_traceOptions gTraceOptions;
S_traceOptions gTraceOptionsUserChoices;

S_traceOptions traceOptions::create (
  S_oahHandler oahHandler)
{
  traceOptions* o = new traceOptions (
    oahHandler);
  assert(o!=0);

  return o;
}

traceOptions::traceOptions (
  S_oahHandler oahHandler)
  : oahGroup (
    "Trace",
    "ht", "help-trace",
R"()",
    oahHandler)
{
  // append this options group to the options handler if relevant
  if (oahHandler) {
    oahHandler->
      appendOptionsGroupToHandler (this);
  }

  // initialize it
  initializeTraceOptions (false);
}

traceOptions::~traceOptions ()
{}

void traceOptions::initializeOptionsHandlingTraceOptions (
  bool boolOptionsInitialValue)
{
  S_oahSubGroup
    traceOptionsSubGroup =
      oahSubGroup::create (
        "Options handling",
        "htoh", "help-trace-options-handling",
R"()",
    oahSubGroup::kAlwaysShowDescription,
    this);

  appendOptionsSubGroup (traceOptionsSubGroup);

  // options

  fTraceOptions = boolOptionsInitialValue;

  traceOptionsSubGroup->
    appendOptionsItem (
      oahBooleanAtom::create (
        "topts", "trace-options",
R"(Write a trace of options handling to standard error.
This option should best appear first.)",
        "traceOptions",
        fTraceOptions));

  // options details

  fTraceOptionsDetails = boolOptionsInitialValue;

  traceOptionsSubGroup->
    appendOptionsItem (
      oahBooleanAtom::create (
        "toptsd", "trace-options-details",
R"(Write a trace of options handling with more details to standard error.
This option should best appear first.)",
        "traceOptionsDetails",
        fTraceOptionsDetails));

  // options values

  fDisplayOptionsValues = boolOptionsInitialValue;

  traceOptionsSubGroup->
    appendOptionsItem (
      oahBooleanAtom::create (
        "dov", "display-options-values",
R"(Write the chosen options values to standard error.)",
        "displayOptionsValues",
        fDisplayOptionsValues));

  // options handler

  fDisplayOptionsHandler = boolOptionsInitialValue;

  traceOptionsSubGroup->
    appendOptionsItem (
      oahBooleanAtom::create (
        "doh", "display-options-handler",
R"(Write the contents of the options handler to standard error.)",
        "displayOptionsHandler",
        fDisplayOptionsHandler));

  // fTraceDetailedMeasureNumbersSet is empty
}

void traceOptions::initializeLowLevelTraceOptions (
  bool boolOptionsInitialValue)
{
  S_oahSubGroup
    lowLevelTraceSubGroup =
      oahSubGroup::create (
        "Low level trace",
        "hllt", "help-low-level-trace",
R"(  Can be quite verbose, use with small input data!
Note: the options in this group imply '-t, -trace-passes'.)",
      oahSubGroup::kHideDescriptionByDefault,
      this);

  appendOptionsSubGroup (lowLevelTraceSubGroup);

  // passes

  fTracePasses = boolOptionsInitialValue;

  lowLevelTraceSubGroup->
    appendOptionsItem (
      oahBooleanAtom::create (
        "passes", "trace-passes",
R"(Write a trace of the passes to standard error.)",
        "tracePasses",
        fTracePasses));

  // for tests, hidden

  fTraceForTests = boolOptionsInitialValue;

  S_oahBooleanAtom
    traceForTestsOptionsBooleanItem =
      oahBooleanAtom::create (
        "tft", "trace-for-tests",
R"(Write a trace for tests to standard error.)",
        "traceForTests",
        fTraceForTests);
  // '-trace-for-tests' is hidden...
  traceForTestsOptionsBooleanItem->
    setOptionsElementIsHidden ();

  lowLevelTraceSubGroup->
    appendOptionsItem (
      traceForTestsOptionsBooleanItem);

  // score

  fTraceScore = boolOptionsInitialValue;

  lowLevelTraceSubGroup->
    appendOptionsItem (
      oahTwoBooleansAtom::create (
        "tscore", "trace-score",
R"(Score)",
        "traceScore",
        fTraceScore,
        fTracePasses));

  // varValAssocs

  fTraceVarValAssocs = boolOptionsInitialValue;

  lowLevelTraceSubGroup->
    appendOptionsItem (
      oahTwoBooleansAtom::create (
        "tvva", "trace-var-val-assocs",
R"(VarValAssocs)",
        "traceVarValAssocs",
        fTraceVarValAssocs,
        fTracePasses));

  // credits

  fTraceCredits = boolOptionsInitialValue;

  lowLevelTraceSubGroup->
    appendOptionsItem (
      oahTwoBooleansAtom::create (
        "tcredits", "trace-credits",
R"(Credits)",
        "traceCredits",
        fTraceCredits,
        fTracePasses));

  // geometry

  fTraceGeometry = boolOptionsInitialValue;

  lowLevelTraceSubGroup->
    appendOptionsItem (
      oahTwoBooleansAtom::create (
        "tgeom", "trace-geometry",
R"(Geometry)",
        "traceGeometry",
        fTraceGeometry,
        fTracePasses));

  // part groups

  fTracePartGroups = boolOptionsInitialValue;
  fTracePartGroupsDetails = boolOptionsInitialValue;

  lowLevelTraceSubGroup->
    appendOptionsItem (
      oahTwoBooleansAtom::create (
        "tpgroups", "trace-part-groups",
R"(Part groups)",
        "tracePartGroups",
        fTracePartGroups,
        fTracePasses));

  lowLevelTraceSubGroup->
    appendOptionsItem (
      oahThreeBooleansAtom::create (
        "tpgroupsd", "trace-part-groups-details",
R"(Part groups with more details.
This option implies '-tpgrps, -tracePartGroups'.)",
        "tracePartGroupsDetails",
        fTracePartGroupsDetails,
        fTracePartGroups,
        fTracePasses));

  // parts

  fTraceParts = boolOptionsInitialValue;

  lowLevelTraceSubGroup->
    appendOptionsItem (
      oahTwoBooleansAtom::create (
        "tparts", "trace-parts",
R"(Parts)",
        "traceParts",
        fTraceParts,
        fTracePasses));

  // staves

  fTraceStaves = boolOptionsInitialValue;

  lowLevelTraceSubGroup->
    appendOptionsItem (
      oahTwoBooleansAtom::create (
        "tstaves", "trace-staves",
R"(Staves)",
        "traceStaves",
        fTraceStaves,
        fTracePasses));

  // voices

  fTraceVoices = boolOptionsInitialValue;
  fTraceVoicesDetails = boolOptionsInitialValue;

  lowLevelTraceSubGroup->
    appendOptionsItem (
      oahTwoBooleansAtom::create (
        "tvoices", "trace-voices",
R"(Voices)",
        "traceVoices",
        fTraceVoices,
        fTracePasses));

  lowLevelTraceSubGroup->
    appendOptionsItem (
      oahThreeBooleansAtom::create (
        "tvoicesd", "trace-voices-details",
R"(Voices with more details (quite verbose)..
This option implies '-tvdetails, -traceVoicesDetails'.)",
        "traceVoicesDetails",
        fTraceVoicesDetails,
        fTraceVoices,
        fTracePasses));

  // measures

  fTraceMeasures = boolOptionsInitialValue;
  fTraceMeasuresDetails = boolOptionsInitialValue;
  fTracePositionsInMeasures = boolOptionsInitialValue;
  // fTraceDetailedMeasureNumbersSet is empty

  lowLevelTraceSubGroup->
    appendOptionsItem (
      oahTwoBooleansAtom::create (
        "tmeas", "trace-measures",
R"(Measures)",
        "traceMeasures",
        fTraceMeasures,
        fTracePasses));

  lowLevelTraceSubGroup->
    appendOptionsItem (
      oahThreeBooleansAtom::create (
        "tmeasd", "trace-measures-details",
R"(Measures details)",
        "traceMeasuresDetails",
        fTraceMeasuresDetails,
        fTraceMeasures,
        fTracePasses));

  lowLevelTraceSubGroup->
    appendOptionsItem (
      oahTwoBooleansAtom::create (
        "tpim", "trace-positions-in-measures",
R"(Positions in measures)",
        "tracePositionsInMeasures",
        fTracePositionsInMeasures,
        fTracePasses));

  // segments

  fTraceSegments = boolOptionsInitialValue;
  fTraceSegmentsDetails = boolOptionsInitialValue;

  lowLevelTraceSubGroup->
    appendOptionsItem (
      oahTwoBooleansAtom::create (
        "tsegs", "trace-segments",
R"(Voices segments)",
        "traceSegments",
        fTraceSegments,
        fTracePasses));

  lowLevelTraceSubGroup->
    appendOptionsItem (
      oahThreeBooleansAtom::create (
        "tsegsd", "trace-segments-details",
R"(Voices segments details)",
        "traceSegments",
        fTraceSegmentsDetails,
        fTraceSegments,
        fTracePasses));

  // clefs

  fTraceClefs = boolOptionsInitialValue;

  lowLevelTraceSubGroup->
    appendOptionsItem (
      oahTwoBooleansAtom::create (
        "tclefs", "trace-clefs",
R"(Clefs)",
        "traceClefs",
        fTraceClefs,
        fTracePasses));

  // keys

  fTraceKeys  = boolOptionsInitialValue;

  lowLevelTraceSubGroup->
    appendOptionsItem (
      oahTwoBooleansAtom::create (
        "tkeys", "trace-keys",
R"(Keys)",
        "traceKeys",
        fTraceKeys,
        fTracePasses));

  // times

  fTraceTimes = boolOptionsInitialValue;

  lowLevelTraceSubGroup->
    appendOptionsItem (
      oahTwoBooleansAtom::create (
        "ttimes", "trace-times",
R"(Times)",
        "traceTimes",
        fTraceTimes,
        fTracePasses));

  // tempos

  fTraceTempos = boolOptionsInitialValue;

  lowLevelTraceSubGroup->
    appendOptionsItem (
      oahTwoBooleansAtom::create (
        "ttempos", "trace-tempos",
R"(Tempos)",
        "traceTempos",
        fTraceTempos,
        fTracePasses));

  // rehearsals

  fTraceRehearsals = boolOptionsInitialValue;

  lowLevelTraceSubGroup->
    appendOptionsItem (
      oahTwoBooleansAtom::create (
        "trehears", "trace-rehearsals",
R"(Rehearsals)",
        "traceRehearsals",
        fTraceRehearsals,
        fTracePasses));

  // line breaks

  fTraceLineBreaks = boolOptionsInitialValue;

  lowLevelTraceSubGroup->
    appendOptionsItem (
      oahTwoBooleansAtom::create (
        "tlbreaks", "trace-line-breaks",
R"(Line breaks)",
        "traceLineBreaks",
        fTraceLineBreaks,
        fTracePasses));

  // page breaks

  fTracePageBreaks = boolOptionsInitialValue;

  lowLevelTraceSubGroup->
    appendOptionsItem (
      oahTwoBooleansAtom::create (
        "tpbreaks", "trace-page-breaks",
R"(Page breaks)",
        "tracePageBreaks",
        fTracePageBreaks,
        fTracePasses));

  // staff changes

  fTraceStaffChanges = boolOptionsInitialValue;

  lowLevelTraceSubGroup->
    appendOptionsItem (
      oahTwoBooleansAtom::create (
        "tschanges", "trace-staff-changes",
R"(Staff changes)",
        "traceStaffChanges",
        fTraceStaffChanges,
        fTracePasses));

  // transpositions

  fTraceTranspositions = boolOptionsInitialValue;

  lowLevelTraceSubGroup->
    appendOptionsItem (
      oahTwoBooleansAtom::create (
        "ttransps", "trace-transpositions",
R"(Transpositions (<transpose/> in MusicXML, \transposition in LilyPond))",
        "traceTranspositions",
        fTraceTranspositions,
        fTracePasses));

  // octave shifts

  fTraceOctaveShifts = boolOptionsInitialValue;

  lowLevelTraceSubGroup->
    appendOptionsItem (
      oahTwoBooleansAtom::create (
        "toshifts", "trace-octaves-shifts",
R"(Octave shifts (<octave-shift/> in MusicXML, \ottava in LilyPond))",
        "traceOctaveShifts",
        fTraceOctaveShifts,
        fTracePasses));

  // barlines

  fTraceBarLines = boolOptionsInitialValue;
  fTraceBarLinesDetails = boolOptionsInitialValue;

  lowLevelTraceSubGroup->
    appendOptionsItem (
      oahTwoBooleansAtom::create (
        "tblines", "trace-barlines",
R"(Barlines)",
        "traceBarlines",
        fTraceBarLines,
        fTracePasses));

  lowLevelTraceSubGroup->
    appendOptionsItem (
      oahThreeBooleansAtom::create (
        "tbarlinesd", "trace-barlines-details",
R"(Barlines details)",
        "traceBarlinesDetails",
        fTraceBarLinesDetails,
        fTraceBarLines,
        fTracePasses));

  // repeats

  fTraceRepeats = boolOptionsInitialValue;
  fTraceRepeatsDetails = boolOptionsInitialValue;

  lowLevelTraceSubGroup->
    appendOptionsItem (
      oahTwoBooleansAtom::create (
        "treps", "trace-repeats",
R"(Repeats)",
        "traceRepeats",
        fTraceRepeats,
        fTracePasses));

  lowLevelTraceSubGroup->
    appendOptionsItem (
      oahThreeBooleansAtom::create (
        "trepsd", "trace-repeats-details",
R"(Repeats details)",
        "traceRepeats",
        fTraceRepeatsDetails,
        fTraceRepeats,
        fTracePasses));

  // measures repeats

  fTraceMeasuresRepeats = boolOptionsInitialValue;

  lowLevelTraceSubGroup->
    appendOptionsItem (
      oahTwoBooleansAtom::create (
        "tmreps", "trace-measures-repeats",
R"(Measures repeats)",
        "traceMeasureRepeats",
        fTraceMeasuresRepeats,
        fTracePasses));

  // rest measures

  fTraceRestMeasures = boolOptionsInitialValue;

  lowLevelTraceSubGroup->
    appendOptionsItem (
      oahTwoBooleansAtom::create (
        "trmeas", "trace-rest-measures",
R"(Multiple rests)",
        "traceRestMeasures",
        fTraceRestMeasures,
        fTracePasses));

  // slashes

  fTraceSlashes = boolOptionsInitialValue;

  lowLevelTraceSubGroup->
    appendOptionsItem (
      oahTwoBooleansAtom::create (
        "tslashes", "trace-slashes",
R"(Slashes)",
        "traceSlashes",
        fTraceSlashes,
        fTracePasses));

  // notes


  fTraceNotes = boolOptionsInitialValue;

  lowLevelTraceSubGroup->
    appendOptionsItem (
      oahTwoBooleansAtom::create (
        "tnotes", "trace-notes",
R"(Notes)",
        "traceNotes",
        fTraceNotes,
        fTracePasses));

  fTraceNotesDetails = boolOptionsInitialValue;

  lowLevelTraceSubGroup->
    appendOptionsItem (
      oahThreeBooleansAtom::create (
        "tnotesd", "trace-notes-details",
R"(Notes with more details, including divisions handling (quite verbose)...
This option implies '-tnnotes, -traceNotes'.)",
        "traceNotesDetails",
        fTraceNotesDetails,
        fTraceNotes,
        fTracePasses));

  fTraceWholeNotes = boolOptionsInitialValue;

  lowLevelTraceSubGroup->
    appendOptionsItem (
      oahTwoBooleansAtom::create (
        "twn", "trace-whole-notes",
R"(Whole notes computations (quite verbose)...)",
        "traceWholeNotes",
        fTraceWholeNotes,
        fTracePasses));

  fTraceRestNotes = boolOptionsInitialValue;

  lowLevelTraceSubGroup->
    appendOptionsItem (
      oahTwoBooleansAtom::create (
        "trn", "trace-rest-notes",
R"(Rest notes)",
        "traceRestNotes",
        fTraceRestNotes,
        fTracePasses));

  fTraceSkipNotes = boolOptionsInitialValue;

  lowLevelTraceSubGroup->
    appendOptionsItem (
      oahTwoBooleansAtom::create (
        "tsn", "trace-skip-notes",
R"(Skip notes)",
        "traceSkipNotes",
        fTraceSkipNotes,
        fTracePasses));

  fTraceNotesOctaveEntry = boolOptionsInitialValue;

  lowLevelTraceSubGroup->
    appendOptionsItem (
      oahTwoBooleansAtom::create (
        "tnoe", "trace-notes-octave-entry",
R"(Notes octave entry)",
        "traceNotesOctaveEntry",
        fTraceNotesOctaveEntry,
        fTracePasses));

  // stems

  fTraceStems = boolOptionsInitialValue;

  lowLevelTraceSubGroup->
    appendOptionsItem (
      oahTwoBooleansAtom::create (
        "tstems", "trace-stems",
R"(Stems)",
        "traceStems",
        fTraceStems,
        fTracePasses));

  // beams

  fTraceBeams = boolOptionsInitialValue;

  lowLevelTraceSubGroup->
    appendOptionsItem (
      oahTwoBooleansAtom::create (
        "tbeams", "trace-beams",
R"(Beams)",
        "traceBeams",
        fTraceBeams,
        fTracePasses));

  // articulations

  fTraceArticulations = boolOptionsInitialValue;

  lowLevelTraceSubGroup->
    appendOptionsItem (
      oahTwoBooleansAtom::create (
        "tarts", "trace-articulations",
R"(Articulations)",
        "traceArticulations",
        fTraceArticulations,
        fTracePasses));

  // technicals

  fTraceTechnicals = boolOptionsInitialValue;

  lowLevelTraceSubGroup->
    appendOptionsItem (
      oahTwoBooleansAtom::create (
        "ttechs", "trace-technicals",
R"(Technicals)",
        "traceTechnicals",
        fTraceTechnicals,
        fTracePasses));

  // ornaments

  fTraceOrnaments = boolOptionsInitialValue;

  lowLevelTraceSubGroup->
    appendOptionsItem (
      oahTwoBooleansAtom::create (
        "torns", "trace-ornaments",
R"(Ornaments)",
        "traceOrnaments",
        fTraceOrnaments,
        fTracePasses));

  // dynamics

  fTraceDynamics = boolOptionsInitialValue;

  lowLevelTraceSubGroup->
    appendOptionsItem (
      oahTwoBooleansAtom::create (
        "tdyns", "trace-dynamics",
R"(Dynamics)",
        "traceDynamics",
        fTraceDynamics,
        fTracePasses));

  // spanners

  fTraceSpanners = boolOptionsInitialValue;

  lowLevelTraceSubGroup->
    appendOptionsItem (
      oahTwoBooleansAtom::create (
        "tspans", "trace-spanners",
R"(Spanners)",
        "traceSpanners",
        fTraceSpanners,
        fTracePasses));

  // words

  fTraceWords = boolOptionsInitialValue;

  lowLevelTraceSubGroup->
    appendOptionsItem (
      oahTwoBooleansAtom::create (
        "twords", "trace-words",
R"(Words)",
        "traceWords",
        fTraceWords,
        fTracePasses));

  // tremolos

  fTraceTremolos = boolOptionsInitialValue;

  lowLevelTraceSubGroup->
    appendOptionsItem (
      oahTwoBooleansAtom::create (
        "ttrems", "trace-tremolos",
R"(Tremolos)",
        "traceTremolos",
        fTraceTremolos,
        fTracePasses));

  // chords

  fTraceChords = boolOptionsInitialValue;
  fTraceChordsDetails = boolOptionsInitialValue;

  lowLevelTraceSubGroup->
    appendOptionsItem (
      oahTwoBooleansAtom::create (
        "tchords", "trace-chords",
R"(Chords)",
        "traceChords",
        fTraceChords,
        fTracePasses));

  lowLevelTraceSubGroup->
    appendOptionsItem (
      oahThreeBooleansAtom::create (
        "tchordsd", "trace-chords-details",
R"(Chords details)",
        "traceChordsDetails",
        fTraceChordsDetails,
        fTraceChords,
        fTracePasses));

  // tuplets

  fTraceTuplets = boolOptionsInitialValue;
  fTraceTupletsDetails = boolOptionsInitialValue;

  lowLevelTraceSubGroup->
    appendOptionsItem (
      oahTwoBooleansAtom::create (
        "ttups", "trace-tuplets",
R"(Tuplets)",
        "traceTuplets",
        fTraceTuplets,
        fTracePasses));

  lowLevelTraceSubGroup->
    appendOptionsItem (
      oahThreeBooleansAtom::create (
        "ttupsd", "trace-tuplets-details",
R"(Tuplets details)",
        "traceTupletsDetails",
        fTraceTupletsDetails,
        fTraceTuplets,
        fTracePasses));

  // glissandos

  fTraceGlissandos = boolOptionsInitialValue;

  lowLevelTraceSubGroup->
    appendOptionsItem (
      oahTwoBooleansAtom::create (
        "tgliss", "trace-glissandos",
R"(Glissandos)",
        "traceGlissandos",
        fTraceGlissandos,
        fTracePasses));

  // eyeglasses

  fTraceEyeGlasses = boolOptionsInitialValue;

  lowLevelTraceSubGroup->
    appendOptionsItem (
      oahTwoBooleansAtom::create (
        "teyes", "trace-eyeglasses",
R"(Eyeglasses)",
        "traceEyeGlasses",
        fTraceEyeGlasses,
        fTracePasses));

  // damps

  fTraceDamps = boolOptionsInitialValue;

  lowLevelTraceSubGroup->
    appendOptionsItem (
      oahTwoBooleansAtom::create (
        "tdamps", "trace-damps",
R"(Damps)",
        "traceDamps",
        fTraceDamps,
        fTracePasses));

  // dampalls

  fTraceDampAlls = boolOptionsInitialValue;

  lowLevelTraceSubGroup->
    appendOptionsItem (
      oahTwoBooleansAtom::create (
        "tdampalls", "trace-dampalls",
R"(Dampalls)",
        "traceDampAlls",
        fTraceDampAlls,
        fTracePasses));

  // slides

  fTraceSlides = boolOptionsInitialValue;

  lowLevelTraceSubGroup->
    appendOptionsItem (
      oahTwoBooleansAtom::create (
        "tslides", "trace-slides",
R"(Slides)",
        "traceSlides",
        fTraceSlides,
        fTracePasses));

  // grace notes

  fTraceGraceNotes = boolOptionsInitialValue;

  lowLevelTraceSubGroup->
    appendOptionsItem (
      oahTwoBooleansAtom::create (
        "tgraces", "trace-gracenotes",
R"(Grace notes)",
        "traceGraceNotes",
        fTraceGraceNotes,
        fTracePasses));

  // lyrics

  fTraceLyrics        = boolOptionsInitialValue;
  fTraceLyricsDetails = boolOptionsInitialValue;

  lowLevelTraceSubGroup->
    appendOptionsItem (
      oahTwoBooleansAtom::create (
        "tlyrics", "trace-lyrics",
R"(Lyrics in MusicXML, stanzas in MSR)",
        "traceLyrics",
        fTraceLyrics,
        fTracePasses));

  lowLevelTraceSubGroup->
    appendOptionsItem (
      oahTwoBooleansAtom::create (
        "tlyricsd", "trace-lyrics-details",
R"(Lyrics in MusicXML, stanzas in MSR)",
        "traceLyricsDetails",
        fTraceLyricsDetails,
        fTracePasses));

  // harmonies

  fTraceHarmonies = boolOptionsInitialValue;

  lowLevelTraceSubGroup->
    appendOptionsItem (
      oahTwoBooleansAtom::create (
        "tharms", "trace-harmonies",
R"(<harmony/> in MusicXML, \chordmode in LilyPond)",
        "traceHarmonies",
        fTraceHarmonies,
        fTracePasses));

  // frames

  fTraceFrames = boolOptionsInitialValue;

  lowLevelTraceSubGroup->
    appendOptionsItem (
      oahTwoBooleansAtom::create (
        "tframes", "trace-frames",
R"(<frame/> in MusicXML, \fret-diagram in LilyPond)",
        "traceFrames",
        fTraceFrames,
        fTracePasses));

  // figured basses

  fTraceFiguredBasses = boolOptionsInitialValue;

  lowLevelTraceSubGroup->
    appendOptionsItem (
      oahTwoBooleansAtom::create (
        "tfigbass", "trace-figured-basses",
R"(<figured-bass> in MusicXML, \figuremode in LilyPond)",
        "traceFiguredBasses",
        fTraceFiguredBasses,
        fTracePasses));

  // ties

  fTraceTies = boolOptionsInitialValue;

  lowLevelTraceSubGroup->
    appendOptionsItem (
      oahTwoBooleansAtom::create (
        "tties", "trace-ties",
R"(Ties)",
        "traceTies",
        fTraceTies,
        fTracePasses));

  // slurs

  fTraceSlurs = boolOptionsInitialValue;

  lowLevelTraceSubGroup->
    appendOptionsItem (
      oahTwoBooleansAtom::create (
        "tslurs", "trace-slurs",
R"(Slurs)",
        "traceSlurs",
        fTraceSlurs,
        fTracePasses));

  // ligatures

  fTraceLigatures = boolOptionsInitialValue;

  lowLevelTraceSubGroup->
    appendOptionsItem (
      oahTwoBooleansAtom::create (
        "tligs", "traceLigatures",
R"(Ligatures)",
        "traceLigatures",
        fTraceLigatures,
        fTracePasses));

  // pedals

  fTracePedals = boolOptionsInitialValue;

  lowLevelTraceSubGroup->
    appendOptionsItem (
      oahTwoBooleansAtom::create (
        "tpedals", "trace-pedals",
R"(Pedals)",
        "tracePedals",
        fTracePedals,
        fTracePasses));

  // wedges

  fTraceWedges = boolOptionsInitialValue;

  lowLevelTraceSubGroup->
    appendOptionsItem (
      oahTwoBooleansAtom::create (
        "twedges", "trace-wedges",
R"(Wedges)",
        "traceWedges",
        fTraceWedges,
        fTracePasses));

  // staff details

  fTraceStaffDetails = boolOptionsInitialValue;

  lowLevelTraceSubGroup->
    appendOptionsItem (
      oahTwoBooleansAtom::create (
        "tst", "trace-staff-details",
R"(Staff details)",
        "traceStaffDetails",
        fTraceStaffDetails,
        fTracePasses));

  // scordaturas

  fTraceScordaturas = boolOptionsInitialValue;

  lowLevelTraceSubGroup->
    appendOptionsItem (
      oahTwoBooleansAtom::create (
        "trace-scordaturas", "",
R"(Scordaturas)",
        "traceScordaturas",
        fTraceScordaturas,
        fTracePasses));

  // segnos

  fTraceSegnos = boolOptionsInitialValue;

  lowLevelTraceSubGroup->
    appendOptionsItem (
      oahTwoBooleansAtom::create (
        "trace-segnos", "",
R"(Segnos)",
        "traceSegnos",
        fTraceSegnos,
        fTracePasses));

  // codas

  fTraceCodas = boolOptionsInitialValue;

  lowLevelTraceSubGroup->
    appendOptionsItem (
      oahTwoBooleansAtom::create (
        "trace-codas", "",
R"(Codas)",
        "traceCodas",
        fTraceCodas,
        fTracePasses));

  // accordion registrations

  fTraceAccordionRegistrations = boolOptionsInitialValue;

  // accordion registrations
  lowLevelTraceSubGroup->
    appendOptionsItem (
      oahTwoBooleansAtom::create (
        "trace-accordion-registrations", "",
R"(Accordion registrations)",
        "traceAccordionRegistrations",
        fTraceAccordionRegistrations,
        fTracePasses));

  // harp pedals

  fTraceHarpPedals = boolOptionsInitialValue;

  lowLevelTraceSubGroup->
    appendOptionsItem (
      oahTwoBooleansAtom::create (
        "trace-harp-pedals", "",
R"(Harp pedals)",
        "traceHarpPedals",
        fTraceHarpPedals,
        fTracePasses));

  // harp pedals tuning

  fTraceHarpPedalsTunings = boolOptionsInitialValue;

  lowLevelTraceSubGroup->
    appendOptionsItem (
      oahTwoBooleansAtom::create (
        "thpt", "traceHarpPedalsTunings",
R"(Harp pedals tuning)",
        "traceHarpPedalsTunings",
        fTraceHarpPedalsTunings,
        fTracePasses));

  // extra chords

  fTraceExtraChords = boolOptionsInitialValue;

  lowLevelTraceSubGroup->
    appendOptionsItem (
      oahTwoBooleansAtom::create (
        "tec", "trace-extra-chords",
R"(Extra chords handling)",
        "traceExtraChords",
        fTraceExtraChords,
        fTracePasses));

/* JMI
  // MSR streams

  fTraceMsrStreams = boolOptionsInitialValue;

  lowLevelTraceSubGroup->
    appendOptionsItem (
      oahTwoBooleansAtom::create (
        "tms", "traceMsrStreams",
R"(MSR Streams API for applications)",
        "traceMsrStreams",
        fTraceMsrStreams,
        fTracePasses));
*/

  // MIDI

  fTraceMidi = boolOptionsInitialValue;

    lowLevelTraceSubGroup->
      appendOptionsItem (
        oahTwoBooleansAtom::create (
          "midi", "trace-midi",
R"(MIDI)",
          "traceMidi",
          fTraceMidi,
          fTracePasses));
}

void traceOptions::initializeTraceOptions (
  bool boolOptionsInitialValue)
{
  // options handling
  initializeOptionsHandlingTraceOptions (
    boolOptionsInitialValue);

  // low level trace
  initializeLowLevelTraceOptions (
    boolOptionsInitialValue);
}

S_traceOptions traceOptions::createCloneWithTrueValues ()
{
  S_traceOptions
    clone =
      traceOptions::create (0);
      // 0 not to have it inserted twice in the option handler

  // set the options handler upLink
  clone->
    setOptionsHandlerUpLink (
      fOptionsHandlerUpLink);


  // specific trace
  // --------------------------------------

  // options
  clone->fTraceOptions = true;
  clone->fTraceOptionsDetails = true;
  clone->fDisplayOptionsValues = true;
  clone->fDisplayOptionsHandler = true;

  // passes
  clone->fTracePasses = true;

  // score
  clone->fTraceScore = true;

  // for tests, hidden
  clone->fTraceForTests = true;

  // varValAssocs
  clone->fTraceVarValAssocs = true;

  // credits
  clone->fTraceCredits = true;

  // geometry
  clone->fTraceGeometry = true;

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

  // measures
  clone->fTraceMeasures = true;
  clone->fTraceMeasuresDetails = true;
  clone->fTracePositionsInMeasures = true;
  clone->fTraceDetailedMeasureNumbersSet =
    fTraceDetailedMeasureNumbersSet;

  // segments
  clone->fTraceSegments = true;
  clone->fTraceSegmentsDetails = true;

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

  // line breaks
  clone->fTraceLineBreaks = true;

  // page breaks
  clone->fTracePageBreaks = true;

  // staff changes
  clone->fTraceStaffChanges = true;

  // transpositions
  clone->fTraceTranspositions = true;

  // octave shifts
  clone->fTraceOctaveShifts = true;

  // barlines
  clone->fTraceBarLines = true;
  clone->fTraceBarLinesDetails = true;

  // repeats
  clone->fTraceRepeats = true;
  clone->fTraceRepeatsDetails = true;

  // measures repeats
  clone->fTraceMeasuresRepeats = true;

  // rest measures
  clone->fTraceRestMeasures = true;

  // slashes
  clone->fTraceSlashes = true;

  // notes
  clone->fTraceNotes = true;
  clone->fTraceNotesDetails = true;
  clone->fTraceWholeNotes = true;
  clone->fTraceRestNotes = true;
  clone->fTraceSkipNotes = true;
  clone->fTraceNotesOctaveEntry = true;

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
  clone->fTraceDynamics = true;

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

  // glissandos
  clone->fTraceTuplets = true;

  // eyeglases
  clone->fTraceGlissandos = true;

  // damps
  clone->fTraceDamps = true;

  // dampalls
  clone->fTraceDampAlls = true;

  // slides
  clone->fTraceSlides = true;

  // grace notes
  clone->fTraceGraceNotes = true;

  // lyrics
  clone->fTraceLyrics        = true;
  clone->fTraceLyricsDetails = true;

  // harmonies
  clone->fTraceHarmonies = true;

  // frames
  clone->fTraceFrames = true;

  // figured basses
  clone->fTraceFiguredBasses = true;

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

  // staff details
  clone->fTraceStaffDetails = true;

  // scordaturas
  clone->fTraceScordaturas = true;

  // segnos
  clone->fTraceSegnos = true;

  // codas
  clone->fTraceCodas = true;

  // accordion registrations
  clone->fTraceAccordionRegistrations = true;

  // harp pedals
  clone->fTraceHarpPedals = true;

  // harp pedals tuning
  clone->fTraceHarpPedalsTunings = true;

  // extra chords
  clone->fTraceExtraChords = true;

/* JMI
  // msrStreams
  clone->fTraceMsrStreams = true;
*/

  // midi
  clone->fTraceMidi = true;

  return clone;
}

/* JMI
void traceOptions::setAllGeneralTraceOptions (
  bool boolOptionsInitialValue)
{
  // options
  fTraceOptions          = boolOptionsInitialValue;
  fTraceOptionsDetails   = boolOptionsInitialValue;
  fDisplayOptionsValues  = boolOptionsInitialValue;
  fDisplayOptionsHandler = boolOptionsInitialValue;

  // passes
  clone->fTracePasses = boolOptionsInitialValue;

  // score
  clone->fTraceScore = boolOptionsInitialValue;

  // for tests, hidden
  clone->fTraceForTests = boolOptionsInitialValue;

  // varValAssocs
  fTraceVarValAssocs = boolOptionsInitialValue;

  // credits
  clone->fTraceCredits = boolOptionsInitialValue;

  // geometry
  fTraceGeometry = boolOptionsInitialValue;

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

  // measures
  clone->fTraceMeasures = boolOptionsInitialValue;
  clone->fTraceMeasuresDetails = boolOptionsInitialValue;
  clone->fTracePositionsInMeasures = boolOptionsInitialValue;
  clone->fTraceDetailedMeasureNumbersSet =
    fTraceDetailedMeasureNumbersSet;

  // segments
  fTraceSegments = boolOptionsInitialValue;
  fTraceSegmentsDetails = boolOptionsInitialValue;

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

  // line breaks
  fTraceLineBreaks = boolOptionsInitialValue;

  // page breaks
  fTracePageBreaks = boolOptionsInitialValue;

  // staff changes
  fTraceStaffChanges = boolOptionsInitialValue;

  // transpositions
  fTraceTranspositions = boolOptionsInitialValue;

  // octave shifts
  fTraceOctaveShifts = boolOptionsInitialValue;

  // barlines
  fTraceBarLines = boolOptionsInitialValue;
  fTraceBarLinesDetails = boolOptionsInitialValue;

  // repeats
  fTraceRepeats = boolOptionsInitialValue;
  fTraceRepeatsDetails = boolOptionsInitialValue;

  // measures repeats
  fTraceMeasuresRepeats = boolOptionsInitialValue;

  // rest measures
  fTraceRestMeasures = boolOptionsInitialValue;

  // slashes
  fTraceSlashes = boolOptionsInitialValue;

  // notes
  fTraceNotes = boolOptionsInitialValue;
  fTraceNotesDetails = boolOptionsInitialValue;
  fTraceWholeNotes = boolOptionsInitialValue;
  fTraceRestNotes = boolOptionsInitialValue;
  fTraceSkipNotes = boolOptionsInitialValue;
  fTraceNotesOctaveEntry = boolOptionsInitialValue;

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
  fTraceDynamics = boolOptionsInitialValue;

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

  // eyeglasses
  fTraceEyeGlasses = boolOptionsInitialValue;

  // damps
  fTraceDamps = boolOptionsInitialValue;

  // dampalls
  fTraceDampAlls = boolOptionsInitialValue;

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

  // figured basses
  fTraceFiguredBasses = boolOptionsInitialValue;

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

  // staff details
  fTraceStaffDetails = boolOptionsInitialValue;

  // scordaturas
  fTraceScordaturas = boolOptionsInitialValue;

  // segnos
  fTraceSegnos = boolOptionsInitialValue;

  // codas
  fTraceCodas = boolOptionsInitialValue;

  // accordion registrations
  fTraceAccordionRegistrations = boolOptionsInitialValue;

  // harp pedals
  fTraceScordaturas = boolOptionsInitialValue;

  // harp pedals tuning
  fTraceHarpPedals = boolOptionsInitialValue;

  // extra chords
  fTraceExtraChords = boolOptionsInitialValue;

/ * JMI
  // msrStreams
  fTraceMsrStreams = boolOptionsInitialValue;
* /

  fTraceMidi = boolOptionsInitialValue;
}
  */

//______________________________________________________________________________
void traceOptions::enforceQuietness ()
{}

//______________________________________________________________________________
void traceOptions::checkOptionsConsistency ()
{
  // JMI
}

//______________________________________________________________________________
void traceOptions::printOptionsValues (
  ostream& os,
  int      valueFieldWidth) const
{
  os <<
    "The trace options are:" <<
    endl;

  gIndenter++;

  os << left <<
    setw (valueFieldWidth) << "Trace:" <<
    endl;

  gIndenter++;

  os << left <<
    // options
    setw (valueFieldWidth) << "traceOptions" << " : " <<
    booleanAsString (fTraceOptions) <<
    endl <<
    setw (valueFieldWidth) << "traceOptionsDetails" << " : " <<
    booleanAsString (fTraceOptionsDetails) <<
    endl <<
    setw (valueFieldWidth) << "displayOptionsValues" << " : " <<
    booleanAsString (fDisplayOptionsValues) <<
    endl <<
    setw (valueFieldWidth) << "displayOptionsHandler" << " : " <<
    booleanAsString (fDisplayOptionsHandler) <<
    endl <<

    // passes
    setw (valueFieldWidth) << "tracePasses" << " : " <<
    booleanAsString (fTracePasses) <<
    endl <<

  // for tests, hidden
    setw (valueFieldWidth) << "traceForTests" << " : " <<
    booleanAsString (fTraceForTests) <<
    endl <<

    // score
    setw (valueFieldWidth) << "traceScore" << " : " <<
    booleanAsString (fTraceScore) <<
    endl <<

    // varValAssocs
    setw (valueFieldWidth) << "traceVarValAssocs" << " : " <<
    booleanAsString (fTraceVarValAssocs) <<
    endl <<

    // credits
    setw (valueFieldWidth) << "traceCredits" << " : " <<
    booleanAsString (fTraceCredits) <<
    endl <<

    // geometry
    setw (valueFieldWidth) << "traceGeometry" << " : " <<
    booleanAsString (fTraceGeometry) <<
    endl <<

    // part groups
    setw (valueFieldWidth) << "tracePartGroups" << " : " <<
    booleanAsString (fTracePartGroups) <<
    endl <<
    setw (valueFieldWidth) << "tracePartGroupsDetails" << " : " <<
    booleanAsString (fTracePartGroupsDetails) <<
    endl <<

    // parts
    setw (valueFieldWidth) << "traceParts" << " : " <<
    booleanAsString (fTraceParts) <<
    endl <<

    // staves
    setw (valueFieldWidth) << "traceStaves" << " : " <<
    booleanAsString (fTraceStaves) <<
    endl <<

    // voices
    setw (valueFieldWidth) << "traceVoices" << " : " <<
    booleanAsString (fTraceVoices) <<
    endl <<
    setw (valueFieldWidth) << "traceVoicesDetails" << " : " <<
    booleanAsString (fTraceVoicesDetails) <<
    endl <<

    // measures
    setw (valueFieldWidth) << "traceMeasures" << " : " <<
    booleanAsString (fTraceMeasures) <<
    endl <<
    setw (valueFieldWidth) << "traceMeasuresDetails" << " : " <<
    booleanAsString (fTraceMeasuresDetails) <<
    endl <<
    setw (valueFieldWidth) << "tracePositionsInMeasures" << " : " <<
    booleanAsString (fTracePositionsInMeasures) <<
    endl;

  os << left <<
    setw (valueFieldWidth) << "traceDetailedMeasureNumbersSet" << " : " <<
    endl;

  if (fTraceDetailedMeasureNumbersSet.size ()) {
    set<int>::const_iterator
      iBegin = fTraceDetailedMeasureNumbersSet.begin (),
      iEnd   = fTraceDetailedMeasureNumbersSet.end (),
      i      = iBegin;

    gIndenter++;

    for ( ; ; ) {
      os << "v " << (*i);
      if (++i == iEnd) break;
      // no endl here
    } // for

    gIndenter--;
  }
  else {
    os <<
      "none";
  }
  os << endl;

    // segments
  os << left <<
    setw (valueFieldWidth) << "traceSegments" << " : " <<
    booleanAsString (fTraceSegments) <<
    endl <<
    setw (valueFieldWidth) << "traceSegmentsDetails" << " : " <<
    booleanAsString (fTraceSegmentsDetails) <<
    endl <<

    // clefs
    setw (valueFieldWidth) << "traceClefs" << " : " <<
    booleanAsString (fTraceClefs) <<
    endl <<

    // keys
    setw (valueFieldWidth) << "traceKeys" << " : " <<
    booleanAsString (fTraceKeys) <<
    endl <<

    // times
    setw (valueFieldWidth) << "traceTimes" << " : " <<
    booleanAsString (fTraceTimes) <<
    endl <<

    // tempos
    setw (valueFieldWidth) << "traceTempos" << " : " <<
    booleanAsString (fTraceTempos) <<
    endl <<

    // rehearsals
    setw (valueFieldWidth) << "traceRehearsals" << " : " <<
    booleanAsString (fTraceRehearsals) <<
    endl <<

    // line breaks
    setw (valueFieldWidth) << "traceLineBreaks" << " : " <<
    booleanAsString (fTraceLineBreaks) <<
    endl <<

    // page breaks
    setw (valueFieldWidth) << "tracePageBreaks" << " : " <<
    booleanAsString (fTracePageBreaks) <<
    endl <<

    // staff changes
    setw (valueFieldWidth) << "traceStaffChanges" << " : " <<
    booleanAsString (fTraceStaffChanges) <<
    endl <<

    // transpositions
    setw (valueFieldWidth) << "traceTranspositions" << " : " <<
    booleanAsString (fTraceTranspositions) <<
    endl <<

    // octave shifts
    setw (valueFieldWidth) << "traceOctaveShifts" << " : " <<
    booleanAsString (fTraceOctaveShifts) <<
    endl <<

    // barlines
    setw (valueFieldWidth) << "traceBarlines" << " : " <<
    booleanAsString (fTraceBarLines) <<
    endl <<
    setw (valueFieldWidth) << "traceBarlinesDetails" << " : " <<
    booleanAsString (fTraceBarLinesDetails) <<
    endl <<

    // repeats
    setw (valueFieldWidth) << "traceRepeats" << " : " <<
    booleanAsString (fTraceRepeats) <<
    endl <<
    setw (valueFieldWidth) << "traceRepeatsDetails" << " : " <<
    booleanAsString (fTraceRepeatsDetails) <<
    endl <<

    // measures repeats
    setw (valueFieldWidth) << "traceMeasuresRepeats" << " : " <<
    booleanAsString (fTraceMeasuresRepeats) <<
    endl <<

    // rest measures
    setw (valueFieldWidth) << "traceRestMeasures" << " : " <<
    booleanAsString (fTraceRestMeasures) <<
    endl <<

    // slashes
    setw (valueFieldWidth) << "traceSlashes" << " : " <<
    booleanAsString (fTraceSlashes) <<
    endl <<

    // notes
    setw (valueFieldWidth) << "traceNotes" << " : " <<
    booleanAsString (fTraceNotes) <<
    endl <<
    setw (valueFieldWidth) << "traceNotesDetails" << " : " <<
    booleanAsString (fTraceNotesDetails) <<
    endl <<
    setw (valueFieldWidth) << "traceWholeNotes" << " : " <<
    booleanAsString (fTraceWholeNotes) <<
    endl <<
    setw (valueFieldWidth) << "traceRestNotes" << " : " <<
    booleanAsString (fTraceRestNotes) <<
    endl <<
    setw (valueFieldWidth) << "traceSkipNotes" << " : " <<
    booleanAsString (fTraceSkipNotes) <<
    endl <<
    setw (valueFieldWidth) << "traceNotesOctaveEntry" << " : " <<
    booleanAsString (fTraceNotesOctaveEntry) <<
    endl <<

    // stems
    setw (valueFieldWidth) << "traceStems" << " : " <<
    booleanAsString (fTraceStems) <<
    endl <<

    // beams
    setw (valueFieldWidth) << "traceBeams" << " : " <<
    booleanAsString (fTraceBeams) <<
    endl <<

    // articulations
    setw (valueFieldWidth) << "traceArticulations" << " : " <<
    booleanAsString (fTraceArticulations) <<
    endl <<

    // technicals
    setw (valueFieldWidth) << "traceTechnicals" << " : " <<
    booleanAsString (fTraceTechnicals) <<
    endl <<

    // ornaments
    setw (valueFieldWidth) << "traceOrnaments" << " : " <<
    booleanAsString (fTraceOrnaments) <<
    endl <<

    // dynamics
    setw (valueFieldWidth) << "traceGraceDynamics" << " : " <<
    booleanAsString (fTraceDynamics) <<
    endl <<

    // spanners
    setw (valueFieldWidth) << "traceSpanners" << " : " <<
    booleanAsString (fTraceSpanners) <<
    endl <<

    // words
    setw (valueFieldWidth) << "traceWords" << " : " <<
    booleanAsString (fTraceWords) <<
    endl <<

    // tremolos
    setw (valueFieldWidth) << "traceTremolos" << " : " <<
    booleanAsString (fTraceTremolos) <<
    endl <<

    // chords
    setw (valueFieldWidth) << "traceChords" << " : " <<
    booleanAsString (fTraceChords) <<
    endl <<
    setw (valueFieldWidth) << "traceChordsDetails" << " : " <<
    booleanAsString (fTraceChordsDetails) <<
    endl <<

    // tuplets
    setw (valueFieldWidth) << "traceTuplets" << " : " <<
    booleanAsString (fTraceTuplets) <<
    endl <<
    setw (valueFieldWidth) << "traceTupletsDetails" << " : " <<
    booleanAsString (fTraceTupletsDetails) <<
    endl <<

    // glissandos
    setw (valueFieldWidth) << "traceGlissandos" << " : " <<
    booleanAsString (fTraceGlissandos) <<
    endl <<

    // eyeglases
    setw (valueFieldWidth) << "traceEyeGlasses" << " : " <<
    booleanAsString (fTraceEyeGlasses) <<
    endl <<

    // damps
    setw (valueFieldWidth) << "traceDamps" << " : " <<
    booleanAsString (fTraceDamps) <<
    endl <<

    // dampalls
    setw (valueFieldWidth) << "traceDampAlls" << " : " <<
    booleanAsString (fTraceDampAlls) <<
    endl <<

    // slides
    setw (valueFieldWidth) << "traceSlides" << " : " <<
    booleanAsString (fTraceSlides) <<
    endl <<

    // grace notes
    setw (valueFieldWidth) << "traceGraceNotes" << " : " <<
    booleanAsString (fTraceGraceNotes) <<
    endl <<

    // lyrics
    setw (valueFieldWidth) << "traceLyrics" << " : " <<
    booleanAsString (fTraceLyrics) <<
    endl <<
    setw (valueFieldWidth) << "traceLyricsDetails" << " : " <<
    booleanAsString (fTraceLyricsDetails) <<
    endl <<

    // harmonies
    setw (valueFieldWidth) << "traceHarmonies" << " : " <<
    booleanAsString (fTraceHarmonies) <<
    endl <<

    // frames
    setw (valueFieldWidth) << "traceFrames" << " : " <<
    booleanAsString (fTraceFrames) <<
    endl <<

    // figured basses
    setw (valueFieldWidth) << "traceFiguredBasses" << " : " <<
    booleanAsString (fTraceFiguredBasses) <<
    endl <<

    // ties
    setw (valueFieldWidth) << "traceTies" << " : " <<
    booleanAsString (fTraceTies) <<
    endl <<

    // slurs
    setw (valueFieldWidth) << "traceSlurs" << " : " <<
    booleanAsString (fTraceSlurs) <<
    endl <<

    // ligatures
    setw (valueFieldWidth) << "traceLigatures" << " : " <<
    booleanAsString (fTraceLigatures) <<
    endl <<

    // pedals
    setw (valueFieldWidth) << "tracePedals" << " : " <<
    booleanAsString (fTracePedals) <<
    endl <<

    // wedges
    setw (valueFieldWidth) << "traceWedges" << " : " <<
    booleanAsString (fTraceWedges) <<
    endl <<

    // staff details
    setw (valueFieldWidth) << "traceStaffDetails" << " : " <<
    booleanAsString (fTraceStaffDetails) <<
    endl <<

    // scordaturas
    setw (valueFieldWidth) << "traceScordaturas" << " : " <<
    booleanAsString (fTraceScordaturas) <<
    endl <<

    // segnos
    setw (valueFieldWidth) << "traceSegnos" << " : " <<
    booleanAsString (fTraceSegnos) <<
    endl <<

    // codas
    setw (valueFieldWidth) << "traceCodas" << " : " <<
    booleanAsString (fTraceCodas) <<
    endl <<

    // accordion registrations
    setw (valueFieldWidth) << "traceAccordionRegistrations" << " : " <<
    booleanAsString (fTraceAccordionRegistrations) <<
    endl <<

    // harp pedals
    setw (valueFieldWidth) << "traceHarpPedals" << " : " <<
    booleanAsString (fTraceHarpPedals) <<
    endl <<

    // harp pedals tuning
    setw (valueFieldWidth) << "traceHarpPedalsTunings" << " : " <<
    booleanAsString (fTraceHarpPedalsTunings) <<
    endl <<

    // extra chords
    setw (valueFieldWidth) << "traceExtraChords" << " : " <<
    booleanAsString (fTraceExtraChords) <<
    endl <<

/* JMI
    // msrStreams
    setw (valueFieldWidth) << "traceMsrStreams" << " : " <<
    booleanAsString (fTraceMsrStreams) <<
    endl <<
*/

    // midi
    setw (valueFieldWidth) << "traceMidi" << " : " <<
    booleanAsString (fTraceMidi) <<
    endl;

  gIndenter--;

  gIndenter--;
}

void traceOptions::printTraceOptionsValues (int fieldWidth)
{
  gLogIOstream <<
    "The trace options are:" <<
    endl;

  gIndenter++;

  gLogIOstream << left <<
    setw (fieldWidth) << "Trace:" <<
    endl;

  gIndenter++;

  gLogIOstream << left <<
    // options
    setw (fieldWidth) << "traceOptions" << " : " <<
    booleanAsString (fTraceOptions) <<
    endl <<
    setw (fieldWidth) << "traceOptionsDetails" << " : " <<
    booleanAsString (fTraceOptionsDetails) <<
    endl <<
    setw (fieldWidth) << "displayOptionsValues" << " : " <<
    booleanAsString (fDisplayOptionsValues) <<
    endl <<
    setw (fieldWidth) << "displayOptionsHandler" << " : " <<
    booleanAsString (fDisplayOptionsHandler) <<
    endl <<

    // passes
    setw (fieldWidth) << "tracePasses" << " : " <<
    booleanAsString (fTracePasses) <<
    endl <<

    // for tests, hidden
    setw (fieldWidth) << "traceForTests" << " : " <<
    booleanAsString (fTraceForTests) <<
    endl <<

    // score
    setw (fieldWidth) << "traceScore" << " : " <<
    booleanAsString (fTraceScore) <<
    endl <<

    // varValAssocs
    setw (fieldWidth) << "traceVarValAssocs" << " : " <<
    booleanAsString (fTraceVarValAssocs) <<
    endl <<

    // credits
    setw (fieldWidth) << "traceCredits" << " : " <<
    booleanAsString (fTraceCredits) <<
    endl <<

    // geometry
    setw (fieldWidth) << "traceGeometry" << " : " <<
    booleanAsString (fTraceGeometry) <<
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

    // measures
    setw (fieldWidth) << "traceMeasures" << " : " <<
    booleanAsString (fTraceMeasures) <<
    endl <<
    setw (fieldWidth) << "traceMeasuresDetails" << " : " <<
    booleanAsString (fTraceMeasuresDetails) <<
    endl <<
    setw (fieldWidth) << "tracePositionsInMeasures" << " : " <<
    booleanAsString (fTracePositionsInMeasures) <<
    endl;

  gLogIOstream << left <<
    setw (fieldWidth) << "traceDetailedMeasureNumbersSet" << " : " <<
    endl;

  if (fTraceDetailedMeasureNumbersSet.size ()) {
    set<int>::const_iterator
      iBegin = fTraceDetailedMeasureNumbersSet.begin (),
      iEnd   = fTraceDetailedMeasureNumbersSet.end (),
      i      = iBegin;

    gIndenter++;

    for ( ; ; ) {
      gLogIOstream << "v " << (*i);
      if (++i == iEnd) break;
      // no endl here
    } // for

    gIndenter--;
  }
  else {
    gLogIOstream <<
      "none";
  }
  gLogIOstream << endl;

    // segments
  gLogIOstream << left <<
    setw (fieldWidth) << "traceSegments" << " : " <<
    booleanAsString (fTraceSegments) <<
    endl <<
    setw (fieldWidth) << "traceSegmentsDetails" << " : " <<
    booleanAsString (fTraceSegmentsDetails) <<
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

    // line breaks
    setw (fieldWidth) << "traceLineBreaks" << " : " <<
    booleanAsString (fTraceLineBreaks) <<
    endl <<

    // page breaks
    setw (fieldWidth) << "tracePageBreaks" << " : " <<
    booleanAsString (fTracePageBreaks) <<
    endl <<

    // staff changes
    setw (fieldWidth) << "traceStaffChanges" << " : " <<
    booleanAsString (fTraceStaffChanges) <<
    endl <<

    // transpositions
    setw (fieldWidth) << "traceTranspositions" << " : " <<
    booleanAsString (fTraceTranspositions) <<
    endl <<

    // octave shifts
    setw (fieldWidth) << "traceOctaveShifts" << " : " <<
    booleanAsString (fTraceOctaveShifts) <<
    endl <<

    // barlines
    setw (fieldWidth) << "traceBarlines" << " : " <<
    booleanAsString (fTraceBarLines) <<
    endl <<
    setw (fieldWidth) << "traceBarlinesDetails" << " : " <<
    booleanAsString (fTraceBarLinesDetails) <<
    endl <<

    // repeats
    setw (fieldWidth) << "traceRepeats" << " : " <<
    booleanAsString (fTraceRepeats) <<
    endl <<
    setw (fieldWidth) << "traceRepeatsDetails" << " : " <<
    booleanAsString (fTraceRepeatsDetails) <<
    endl <<

    // measures repeats
    setw (fieldWidth) << "traceMeasuresRepeats" << " : " <<
    booleanAsString (fTraceMeasuresRepeats) <<
    endl <<

    // rest measures
    setw (fieldWidth) << "traceRestMeasures" << " : " <<
    booleanAsString (fTraceRestMeasures) <<
    endl <<

    // slashes
    setw (fieldWidth) << "traceSlashes" << " : " <<
    booleanAsString (fTraceSlashes) <<
    endl <<

    // notes
    setw (fieldWidth) << "traceNotes" << " : " <<
    booleanAsString (fTraceNotes) <<
    endl <<
    setw (fieldWidth) << "traceNotesDetails" << " : " <<
    booleanAsString (fTraceNotesDetails) <<
    endl <<
    setw (fieldWidth) << "traceWholeNotes" << " : " <<
    booleanAsString (fTraceWholeNotes) <<
    endl <<
    setw (fieldWidth) << "traceRestNotes" << " : " <<
    booleanAsString (fTraceRestNotes) <<
    endl <<
    setw (fieldWidth) << "traceSkipNotes" << " : " <<
    booleanAsString (fTraceSkipNotes) <<
    endl <<
    setw (fieldWidth) << "traceNotesOctaveEntry" << " : " <<
    booleanAsString (fTraceNotesOctaveEntry) <<
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
    booleanAsString (fTraceDynamics) <<
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

    // glissandos
    setw (fieldWidth) << "traceGlissandos" << " : " <<
    booleanAsString (fTraceGlissandos) <<
    endl <<

    // eyeglases
    setw (fieldWidth) << "traceEyeGlasses" << " : " <<
    booleanAsString (fTraceEyeGlasses) <<
    endl <<

    // damps
    setw (fieldWidth) << "traceDamps" << " : " <<
    booleanAsString (fTraceDamps) <<
    endl <<

    // dampalls
    setw (fieldWidth) << "traceDampAlls" << " : " <<
    booleanAsString (fTraceDampAlls) <<
    endl <<

    // slides
    setw (fieldWidth) << "traceSlides" << " : " <<
    booleanAsString (fTraceSlides) <<
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

    // figured basses
    setw (fieldWidth) << "traceFiguredBasses" << " : " <<
    booleanAsString (fTraceFiguredBasses) <<
    endl <<

    // ties
    setw (fieldWidth) << "traceTies" << " : " <<
    booleanAsString (fTraceTies) <<
    endl <<

    // slurs
    setw (fieldWidth) << "traceSlurs" << " : " <<
    booleanAsString (fTraceSlurs) <<
    endl <<

    // ligatures
    setw (fieldWidth) << "traceLigatures" << " : " <<
    booleanAsString (fTraceLigatures) <<
    endl <<

    // pedals
    setw (fieldWidth) << "tracePedals" << " : " <<
    booleanAsString (fTracePedals) <<
    endl <<

    // wedges
    setw (fieldWidth) << "traceWedges" << " : " <<
    booleanAsString (fTraceWedges) <<
    endl <<

    // staff details
    setw (fieldWidth) << "traceStaffDetails" << " : " <<
    booleanAsString (fTraceStaffDetails) <<
    endl <<

    // scordaturas
    setw (fieldWidth) << "traceScordaturas" << " : " <<
    booleanAsString (fTraceScordaturas) <<
    endl <<

    // segnos
    setw (fieldWidth) << "traceSegnos" << " : " <<
    booleanAsString (fTraceSegnos) <<
    endl <<

    // codas
    setw (fieldWidth) << "traceCodas" << " : " <<
    booleanAsString (fTraceCodas) <<
    endl <<

    // accordion registrations
    setw (fieldWidth) << "traceAccordionRegistrations" << " : " <<
    booleanAsString (fTraceAccordionRegistrations) <<
    endl <<

    // harp pedals
    setw (fieldWidth) << "traceHarpPedals" << " : " <<
    booleanAsString (fTraceHarpPedals) <<
    endl <<

    // harp pedals tuning
    setw (fieldWidth) << "traceHarpPedalsTunings" << " : " <<
    booleanAsString (fTraceHarpPedalsTunings) <<
    endl <<

    // extra chords
    setw (fieldWidth) << "traceExtraChords" << " : " <<
    booleanAsString (fTraceExtraChords) <<
    endl <<

/* JMI
    // msrStreams
    setw (fieldWidth) << "traceMsrStreams" << " : " <<
    booleanAsString (fTraceMsrStreams) <<
    endl <<
*/

    // midi
    setw (fieldWidth) << "traceMidi" << " : " <<
    booleanAsString (fTraceMidi) <<
    endl;

  gIndenter--;

  gIndenter--;
}

S_oahValuedAtom traceOptions::handleOptionsItem (
  ostream&      os,
  S_oahAtom item)
{
  S_oahValuedAtom result;

  return result;
}

ostream& operator<< (ostream& os, const S_traceOptions& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
void initializeTraceOptionsHandling (
  S_oahHandler oahHandler)
{
#ifdef TRACE_OPTIONS
  if (false && ! gGeneralOptions->fQuiet) { // JMI
    gLogIOstream <<
      "Initializing trace options handling" <<
      endl;
  }
#endif

  // create the options variables
  // ------------------------------------------------------

  gTraceOptionsUserChoices = traceOptions::create (
    oahHandler);
  assert(gTraceOptionsUserChoices != 0);

  gTraceOptions =
    gTraceOptionsUserChoices;

  gTraceOptions->
    checkOptionsConsistency ();
}


}

#endif


/* JMI
  // traceDetailedMeasureNumbersSet
  // --------------------------------------

  {
    S_oahSubGroup
      traceAndDisplaySubGroup =
        oahSubGroup::create (
          "Trace and display",
          "htd", "help-trace-and-display",
R"()",
      oahSubGroup::kAlwaysShowDescription,
      this);

    appendOptionsSubGroup (traceAndDisplaySubGroup);

  // detailed measure numbers set

  // fTraceDetailedMeasureNumbersSet is empty

    traceAndDisplaySubGroup->
      appendOptionsItem (
        oahNumbersSetAtom::create (
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
*/

