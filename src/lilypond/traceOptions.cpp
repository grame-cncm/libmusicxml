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
  S_oahHandler handler)
{
  traceOptions* o = new traceOptions (
    handler);
  assert(o!=0);

  return o;
}

traceOptions::traceOptions (
  S_oahHandler handler)
  : oahGroup (
    "Trace",
    "ht", "help-trace",
R"(There is a range of trace options, showing what's going on during translation.
They're provided as a help to the maintainers, as well as for the curious.
The options in this group can be quite verbose, use with small input data!
All of them imply '-tpasses, -trace-passes'.)",
    kElementVisibilityHiddenByDefault,
    handler)
{
  // append this options group to the options handler if relevant
  if (handler) {
    handler->
      appendGroupToHandler (this);
  }

  // initialize it
  initializeTraceOptions (false);

  fMaximumSubGroupHeaderLength = 0;
}

traceOptions::~traceOptions ()
{}

void traceOptions::initializeTranspositionsTraceOptions (
  bool boolOptionsInitialValue)
{
  S_oahSubGroup
   transpositionsTraceSubGroup =
      oahSubGroup::create (
        "Transpositions",
        "htt", "help-transpositions-trace",
R"()",
      kElementVisibilityHiddenByDefault,
      this);

  appendSubGroup (transpositionsTraceSubGroup);

  // transpositions

  fTraceTranspositions = boolOptionsInitialValue;

  transpositionsTraceSubGroup->
    appendAtom (
      oahTwoBooleansAtom::create (
        "ttransps", "trace-transpositions",
R"(Transpositions (<transpose/> in MusicXML, \transposition in LilyPond))",
        "traceTranspositions",
        fTraceTranspositions,
        fTracePasses));

  // octave shifts

  fTraceOctaveShifts = boolOptionsInitialValue;

  transpositionsTraceSubGroup->
    appendAtom (
      oahTwoBooleansAtom::create (
        "toshifts", "trace-octaves-shifts",
R"(Octave shifts (<octave-shift/> in MusicXML, \ottava in LilyPond))",
        "traceOctaveShifts",
        fTraceOctaveShifts,
        fTracePasses));
}

void traceOptions::initializeAboveStaffTraceOptions (
  bool boolOptionsInitialValue)
{
  S_oahSubGroup
    aboveStaffTraceSubGroup =
      oahSubGroup::create (
        "Above staff",
        "hast", "help-above-staff-trace",
R"()",
      kElementVisibilityHiddenByDefault,
      this);

  appendSubGroup (aboveStaffTraceSubGroup);

  // rehearsals

  fTraceRehearsals = boolOptionsInitialValue;

  aboveStaffTraceSubGroup->
    appendAtom (
      oahTwoBooleansAtom::create (
        "trehears", "trace-rehearsals",
R"(Rehearsals)",
        "traceRehearsals",
        fTraceRehearsals,
        fTracePasses));

  // segnos

  fTraceSegnos = boolOptionsInitialValue;

  aboveStaffTraceSubGroup->
    appendAtom (
      oahTwoBooleansAtom::create (
        "trace-segnos", "",
R"(Segnos)",
        "traceSegnos",
        fTraceSegnos,
        fTracePasses));

  // codas

  fTraceCodas = boolOptionsInitialValue;

  aboveStaffTraceSubGroup->
    appendAtom (
      oahTwoBooleansAtom::create (
        "trace-codas", "",
R"(Codas)",
        "traceCodas",
        fTraceCodas,
        fTracePasses));

  // eyeglasses

  fTraceEyeGlasses = boolOptionsInitialValue;

  aboveStaffTraceSubGroup->
    appendAtom (
      oahTwoBooleansAtom::create (
        "teyes", "trace-eyeglasses",
R"(Eyeglasses)",
        "traceEyeGlasses",
        fTraceEyeGlasses,
        fTracePasses));
}

void traceOptions::initializeBreaksAndBarlinesTraceOptions (
  bool boolOptionsInitialValue)
{
  S_oahSubGroup
    breaksAndBarlinesTraceSubGroup =
      oahSubGroup::create (
        "Breaks and barlines",
        "hbt", "help-breaks-and-barlines-trace",
R"()",
      kElementVisibilityHiddenByDefault,
      this);

  appendSubGroup (breaksAndBarlinesTraceSubGroup);

  // line breaks

  fTraceLineBreaks = boolOptionsInitialValue;

  breaksAndBarlinesTraceSubGroup->
    appendAtom (
      oahTwoBooleansAtom::create (
        "tlbreaks", "trace-line-breaks",
R"(Line breaks)",
        "traceLineBreaks",
        fTraceLineBreaks,
        fTracePasses));

  // page breaks

  fTracePageBreaks = boolOptionsInitialValue;

  breaksAndBarlinesTraceSubGroup->
    appendAtom (
      oahTwoBooleansAtom::create (
        "tpbreaks", "trace-page-breaks",
R"(Page breaks)",
        "tracePageBreaks",
        fTracePageBreaks,
        fTracePasses));

  // barlines

  fTraceBarLines = boolOptionsInitialValue;

  breaksAndBarlinesTraceSubGroup->
    appendAtom (
      oahTwoBooleansAtom::create (
        "tblines", "trace-barlines",
R"(Barlines)",
        "traceBarlines",
        fTraceBarLines,
        fTracePasses));

  // barlines details

  fTraceBarLinesDetails = boolOptionsInitialValue;

  breaksAndBarlinesTraceSubGroup->
    appendAtom (
      oahThreeBooleansAtom::create (
        "tbarlinesd", "trace-barlines-details",
R"(Barlines details)",
        "traceBarlinesDetails",
        fTraceBarLinesDetails,
        fTraceBarLines,
        fTracePasses));
}

void traceOptions::initializeClefsToTemposTraceOptions (
  bool boolOptionsInitialValue)
{
  S_oahSubGroup
    clefsToTemposTraceSubGroup =
      oahSubGroup::create (
        "Clefs to tempos",
        "hcttt", "help-clefs-to-tempos-trace",
R"()",
      kElementVisibilityHiddenByDefault,
      this);

  appendSubGroup (clefsToTemposTraceSubGroup);

  // clefs

  fTraceClefs = boolOptionsInitialValue;

  clefsToTemposTraceSubGroup->
    appendAtom (
      oahTwoBooleansAtom::create (
        "tclefs", "trace-clefs",
R"(Clefs)",
        "traceClefs",
        fTraceClefs,
        fTracePasses));

  // keys

  fTraceKeys  = boolOptionsInitialValue;

  clefsToTemposTraceSubGroup->
    appendAtom (
      oahTwoBooleansAtom::create (
        "tkeys", "trace-keys",
R"(Keys)",
        "traceKeys",
        fTraceKeys,
        fTracePasses));

  // times

  fTraceTimes = boolOptionsInitialValue;

  clefsToTemposTraceSubGroup->
    appendAtom (
      oahTwoBooleansAtom::create (
        "ttimes", "trace-times",
R"(Times)",
        "traceTimes",
        fTraceTimes,
        fTracePasses));

  // tempos

  fTraceTempos = boolOptionsInitialValue;

  clefsToTemposTraceSubGroup->
    appendAtom (
      oahTwoBooleansAtom::create (
        "ttempos", "trace-tempos",
R"(Tempos)",
        "traceTempos",
        fTraceTempos,
        fTracePasses));
}

void traceOptions::initializeInterNotesTraceOptions (
  bool boolOptionsInitialValue)
{
  S_oahSubGroup
    interNotesTraceSubGroup =
      oahSubGroup::create (
        "Inter-notes",
        "hint", "help-inter-notes-trace",
R"()",
      kElementVisibilityHiddenByDefault,
      this);

  appendSubGroup (interNotesTraceSubGroup);

  // ties

  fTraceTies = boolOptionsInitialValue;

  interNotesTraceSubGroup->
    appendAtom (
      oahTwoBooleansAtom::create (
        "tties", "trace-ties",
R"(Ties)",
        "traceTies",
        fTraceTies,
        fTracePasses));

  // glissandos

  fTraceGlissandos = boolOptionsInitialValue;

  interNotesTraceSubGroup->
    appendAtom (
      oahTwoBooleansAtom::create (
        "tgliss", "trace-glissandos",
R"(Glissandos)",
        "traceGlissandos",
        fTraceGlissandos,
        fTracePasses));
}

void traceOptions::initializeSpannersTraceOptions (
  bool boolOptionsInitialValue)
{
  S_oahSubGroup
    spannersTraceSubGroup =
      oahSubGroup::create (
        "Spanners",
        "hst", "help-spanners-trace",
R"()",
      kElementVisibilityHiddenByDefault,
      this);

  appendSubGroup (spannersTraceSubGroup);

  // spanners

  fTraceSpanners = boolOptionsInitialValue;

  spannersTraceSubGroup->
    appendAtom (
      oahTwoBooleansAtom::create (
        "tspans", "trace-spanners",
R"(Spanners)",
        "traceSpanners",
        fTraceSpanners,
        fTracePasses));

  // wedges

  fTraceWedges = boolOptionsInitialValue;

  spannersTraceSubGroup->
    appendAtom (
      oahTwoBooleansAtom::create (
        "twedges", "trace-wedges",
R"(Wedges)",
        "traceWedges",
        fTraceWedges,
        fTracePasses));

  // slurs

  fTraceSlurs = boolOptionsInitialValue;

  spannersTraceSubGroup->
    appendAtom (
      oahTwoBooleansAtom::create (
        "tslurs", "trace-slurs",
R"(Slurs)",
        "traceSlurs",
        fTraceSlurs,
        fTracePasses));

  // ligatures

  fTraceLigatures = boolOptionsInitialValue;

  spannersTraceSubGroup->
    appendAtom (
      oahTwoBooleansAtom::create (
        "tligs", "traceLigatures",
R"(Ligatures)",
        "traceLigatures",
        fTraceLigatures,
        fTracePasses));
}

void traceOptions::initializeHarmoniesAndFiguredBassesTraceOptions (
  bool boolOptionsInitialValue)
{
  S_oahSubGroup
    harmoniesAndFiguredBassesTraceSubGroup =
      oahSubGroup::create (
        "Harmonies and figured basses",
        "hhafbt", "help-harmonies-and-figured-basses-trace",
R"()",
      kElementVisibilityHiddenByDefault,
      this);

  appendSubGroup (harmoniesAndFiguredBassesTraceSubGroup);

  // harmonies

  fTraceHarmonies = boolOptionsInitialValue;

  harmoniesAndFiguredBassesTraceSubGroup->
    appendAtom (
      oahTwoBooleansAtom::create (
        "tharms", "trace-harmonies",
R"(<harmony/> in MusicXML, \chordmode in LilyPond)",
        "traceHarmonies",
        fTraceHarmonies,
        fTracePasses));

  // figured basses

  fTraceFiguredBasses = boolOptionsInitialValue;

  harmoniesAndFiguredBassesTraceSubGroup->
    appendAtom (
      oahTwoBooleansAtom::create (
        "tfigbass", "trace-figured-basses",
R"(<figured-bass> in MusicXML, \figuremode in LilyPond)",
        "traceFiguredBasses",
        fTraceFiguredBasses,
        fTracePasses));
}

void traceOptions::initializeCreditsToWordsTraceOptions (
  bool boolOptionsInitialValue)
{
  S_oahSubGroup
    creditsToWordsTraceSubGroup =
      oahSubGroup::create (
        "Credits to words",
        "hctwt", "help-credits-to-words-trace",
R"()",
      kElementVisibilityHiddenByDefault,
      this);

  appendSubGroup (creditsToWordsTraceSubGroup);

  // credits

  fTraceCredits = boolOptionsInitialValue;

  creditsToWordsTraceSubGroup->
    appendAtom (
      oahTwoBooleansAtom::create (
        "tcredits", "trace-credits",
R"(Credits)",
        "traceCredits",
        fTraceCredits,
        fTracePasses));

  // lyrics

  fTraceLyrics        = boolOptionsInitialValue;

  creditsToWordsTraceSubGroup->
    appendAtom (
      oahTwoBooleansAtom::create (
        "tlyrics", "trace-lyrics",
R"(Lyrics in MusicXML, stanzas in MSR)",
        "traceLyrics",
        fTraceLyrics,
        fTracePasses));

  // lyrics details

  fTraceLyricsDetails = boolOptionsInitialValue;

  creditsToWordsTraceSubGroup->
    appendAtom (
      oahTwoBooleansAtom::create (
        "tlyricsd", "trace-lyrics-details",
R"(Lyrics in MusicXML, stanzas in MSR)",
        "traceLyricsDetails",
        fTraceLyricsDetails,
        fTracePasses));

  // words

  fTraceWords = boolOptionsInitialValue;

  creditsToWordsTraceSubGroup->
    appendAtom (
      oahTwoBooleansAtom::create (
        "twords", "trace-words",
R"(Words)",
        "traceWords",
        fTraceWords,
        fTracePasses));
}

void traceOptions::initializeChordsAndTupletsTraceOptions (
  bool boolOptionsInitialValue)
{
  S_oahSubGroup
    chordsAndTupletsTraceSubGroup =
      oahSubGroup::create (
        "Chords and tuplets",
        "hcatt", "help-chords-and-tuplets-trace",
R"()",
      kElementVisibilityHiddenByDefault,
      this);

  appendSubGroup (chordsAndTupletsTraceSubGroup);

  // chords

  fTraceChords = boolOptionsInitialValue;

  chordsAndTupletsTraceSubGroup->
    appendAtom (
      oahTwoBooleansAtom::create (
        "tchords", "trace-chords",
R"(Chords)",
        "traceChords",
        fTraceChords,
        fTracePasses));

  // chords details

  fTraceChordsDetails = boolOptionsInitialValue;

  chordsAndTupletsTraceSubGroup->
    appendAtom (
      oahThreeBooleansAtom::create (
        "tchordsd", "trace-chords-details",
R"(Chords details)",
        "traceChordsDetails",
        fTraceChordsDetails,
        fTraceChords,
        fTracePasses));

  // tuplets

  fTraceTuplets = boolOptionsInitialValue;

  chordsAndTupletsTraceSubGroup->
    appendAtom (
      oahTwoBooleansAtom::create (
        "ttups", "trace-tuplets",
R"(Tuplets)",
        "traceTuplets",
        fTraceTuplets,
        fTracePasses));

  // tuplets details

  fTraceTupletsDetails = boolOptionsInitialValue;

  chordsAndTupletsTraceSubGroup->
    appendAtom (
      oahThreeBooleansAtom::create (
        "ttupsd", "trace-tuplets-details",
R"(Tuplets details)",
        "traceTupletsDetails",
        fTraceTupletsDetails,
        fTraceTuplets,
        fTracePasses));

  // extra chords

  fTraceExtraChords = boolOptionsInitialValue;

  chordsAndTupletsTraceSubGroup->
    appendAtom (
      oahTwoBooleansAtom::create (
        "tec", "trace-extra-chords",
R"(Extra chords handling)",
        "traceExtraChords",
        fTraceExtraChords,
        fTracePasses));
}

void traceOptions::initializeInstrumentsTraceOptions (
  bool boolOptionsInitialValue)
{
  S_oahSubGroup
    instrumentsTraceSubGroup =
      oahSubGroup::create (
        "Instruments",
        "hit", "help-instruments-trace-trace",
R"()",
      kElementVisibilityHiddenByDefault,
      this);

  appendSubGroup (instrumentsTraceSubGroup);

  // frames

  fTraceFrames = boolOptionsInitialValue;

  instrumentsTraceSubGroup->
    appendAtom (
      oahTwoBooleansAtom::create (
        "tframes", "trace-frames",
R"(<frame/> in MusicXML, \fret-diagram in LilyPond)",
        "traceFrames",
        fTraceFrames,
        fTracePasses));

  // scordaturas

  fTraceScordaturas = boolOptionsInitialValue;

  instrumentsTraceSubGroup->
    appendAtom (
      oahTwoBooleansAtom::create (
        "trace-scordaturas", "",
R"(Scordaturas)",
        "traceScordaturas",
        fTraceScordaturas,
        fTracePasses));

  // slides

  fTraceSlides = boolOptionsInitialValue;

  instrumentsTraceSubGroup->
    appendAtom (
      oahTwoBooleansAtom::create (
        "tslides", "trace-slides",
R"(Slides)",
        "traceSlides",
        fTraceSlides,
        fTracePasses));

  // pedals

  fTracePedals = boolOptionsInitialValue;

  instrumentsTraceSubGroup->
    appendAtom (
      oahTwoBooleansAtom::create (
        "tpedals", "trace-pedals",
R"(Pedals)",
        "tracePedals",
        fTracePedals,
        fTracePasses));

  // accordion registrations

  fTraceAccordionRegistrations = boolOptionsInitialValue;

  // accordion registrations
  instrumentsTraceSubGroup->
    appendAtom (
      oahTwoBooleansAtom::create (
        "trace-accordion-registrations", "",
R"(Accordion registrations)",
        "traceAccordionRegistrations",
        fTraceAccordionRegistrations,
        fTracePasses));

  // harp pedals

  fTraceHarpPedals = boolOptionsInitialValue;

  instrumentsTraceSubGroup->
    appendAtom (
      oahTwoBooleansAtom::create (
        "trace-harp-pedals", "",
R"(Harp pedals)",
        "traceHarpPedals",
        fTraceHarpPedals,
        fTracePasses));

  // harp pedals tuning

  fTraceHarpPedalsTunings = boolOptionsInitialValue;

  instrumentsTraceSubGroup->
    appendAtom (
      oahTwoBooleansAtom::create (
        "thpt", "traceHarpPedalsTunings",
R"(Harp pedals tuning)",
        "traceHarpPedalsTunings",
        fTraceHarpPedalsTunings,
        fTracePasses));

  // damps

  fTraceDamps = boolOptionsInitialValue;

  instrumentsTraceSubGroup->
    appendAtom (
      oahTwoBooleansAtom::create (
        "tdamps", "trace-damps",
R"(Damps)",
        "traceDamps",
        fTraceDamps,
        fTracePasses));

  // dampalls

  fTraceDampAlls = boolOptionsInitialValue;

  instrumentsTraceSubGroup->
    appendAtom (
      oahTwoBooleansAtom::create (
        "tdampalls", "trace-dampalls",
R"(Dampalls)",
        "traceDampAlls",
        fTraceDampAlls,
        fTracePasses));

  // MIDI

  fTraceMidi = boolOptionsInitialValue;

    instrumentsTraceSubGroup->
      appendAtom (
        oahTwoBooleansAtom::create (
          "midi", "trace-midi",
R"(MIDI)",
          "traceMidi",
          fTraceMidi,
          fTracePasses));
}

void traceOptions::initializeNotesAttachmentsTraceOptions (
  bool boolOptionsInitialValue)
{
  S_oahSubGroup
    notesAttachmentsTraceSubGroup =
      oahSubGroup::create (
        "Notes attachments",
        "hnat", "help-notes-attachments-trace",
R"()",
      kElementVisibilityHiddenByDefault,
      this);

  appendSubGroup (notesAttachmentsTraceSubGroup);

  // stems

  fTraceStems = boolOptionsInitialValue;

  notesAttachmentsTraceSubGroup->
    appendAtom (
      oahTwoBooleansAtom::create (
        "tstems", "trace-stems",
R"(Stems)",
        "traceStems",
        fTraceStems,
        fTracePasses));

  // beams

  fTraceBeams = boolOptionsInitialValue;

  notesAttachmentsTraceSubGroup->
    appendAtom (
      oahTwoBooleansAtom::create (
        "tbeams", "trace-beams",
R"(Beams)",
        "traceBeams",
        fTraceBeams,
        fTracePasses));

  // articulations

  fTraceArticulations = boolOptionsInitialValue;

  notesAttachmentsTraceSubGroup->
    appendAtom (
      oahTwoBooleansAtom::create (
        "tarts", "trace-articulations",
R"(Articulations)",
        "traceArticulations",
        fTraceArticulations,
        fTracePasses));

  // technicals

  fTraceTechnicals = boolOptionsInitialValue;

  notesAttachmentsTraceSubGroup->
    appendAtom (
      oahTwoBooleansAtom::create (
        "ttechs", "trace-technicals",
R"(Technicals)",
        "traceTechnicals",
        fTraceTechnicals,
        fTracePasses));

  // ornaments

  fTraceOrnaments = boolOptionsInitialValue;

  notesAttachmentsTraceSubGroup->
    appendAtom (
      oahTwoBooleansAtom::create (
        "torns", "trace-ornaments",
R"(Ornaments)",
        "traceOrnaments",
        fTraceOrnaments,
        fTracePasses));

  // dynamics

  fTraceDynamics = boolOptionsInitialValue;

  notesAttachmentsTraceSubGroup->
    appendAtom (
      oahTwoBooleansAtom::create (
        "tdyns", "trace-dynamics",
R"(Dynamics)",
        "traceDynamics",
        fTraceDynamics,
        fTracePasses));
}

void traceOptions::initializeSegmentsAndMeasuresTraceOptions (
  bool boolOptionsInitialValue)
{
  S_oahSubGroup
    segmentsAndMeasuresTraceSubGroup =
      oahSubGroup::create (
        "Segments and measures",
        "hsamt", "help-segments-and-measures-trace",
R"()",
      kElementVisibilityHiddenByDefault,
      this);

  appendSubGroup (segmentsAndMeasuresTraceSubGroup);

  // segments

  fTraceSegments = boolOptionsInitialValue;

  segmentsAndMeasuresTraceSubGroup->
    appendAtom (
      oahTwoBooleansAtom::create (
        "tsegs", "trace-segments",
R"(Voices segments)",
        "traceSegments",
        fTraceSegments,
        fTracePasses));

  // segments details

  fTraceSegmentsDetails = boolOptionsInitialValue;

  segmentsAndMeasuresTraceSubGroup->
    appendAtom (
      oahThreeBooleansAtom::create (
        "tsegsd", "trace-segments-details",
R"(Voices segments details)",
        "traceSegments",
        fTraceSegmentsDetails,
        fTraceSegments,
        fTracePasses));

  // measures

  fTraceMeasures = boolOptionsInitialValue;

  segmentsAndMeasuresTraceSubGroup->
    appendAtom (
      oahTwoBooleansAtom::create (
        "tmeas", "trace-measures",
R"(Measures)",
        "traceMeasures",
        fTraceMeasures,
        fTracePasses));

  // measures details

  fTraceMeasuresDetails = boolOptionsInitialValue;

  segmentsAndMeasuresTraceSubGroup->
    appendAtom (
      oahThreeBooleansAtom::create (
        "tmeasd", "trace-measures-details",
R"(Measures details)",
        "traceMeasuresDetails",
        fTraceMeasuresDetails,
        fTraceMeasures,
        fTracePasses));

  // positions in measures

  fTracePositionsInMeasures = boolOptionsInitialValue;

  segmentsAndMeasuresTraceSubGroup->
    appendAtom (
      oahTwoBooleansAtom::create (
        "tpim", "trace-positions-in-measures",
R"(Positions in measures)",
        "tracePositionsInMeasures",
        fTracePositionsInMeasures,
        fTracePasses));

  // fTraceDetailedMeasureNumbersSet is empty
}

void traceOptions::initializeScoreToVoicesTraceOptions (
  bool boolOptionsInitialValue)
{
  S_oahSubGroup
    scoreToVoicesTraceSubGroup =
      oahSubGroup::create (
        "Score to voices",
        "hstvt", "help-score-to-voices-trace",
R"()",
      kElementVisibilityHiddenByDefault,
      this);

  appendSubGroup (scoreToVoicesTraceSubGroup);

  // score

  fTraceScore = boolOptionsInitialValue;

  scoreToVoicesTraceSubGroup->
    appendAtom (
      oahTwoBooleansAtom::create (
        "tscore", "trace-score",
R"(Score)",
        "traceScore",
        fTraceScore,
        fTracePasses));

  // part groups

  fTracePartGroups = boolOptionsInitialValue;
  fTracePartGroupsDetails = boolOptionsInitialValue;

  scoreToVoicesTraceSubGroup->
    appendAtom (
      oahTwoBooleansAtom::create (
        "tpgroups", "trace-part-groups",
R"(Part groups)",
        "tracePartGroups",
        fTracePartGroups,
        fTracePasses));

  scoreToVoicesTraceSubGroup->
    appendAtom (
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

  scoreToVoicesTraceSubGroup->
    appendAtom (
      oahTwoBooleansAtom::create (
        "tparts", "trace-parts",
R"(Parts)",
        "traceParts",
        fTraceParts,
        fTracePasses));

  // staves

  fTraceStaves = boolOptionsInitialValue;

  scoreToVoicesTraceSubGroup->
    appendAtom (
      oahTwoBooleansAtom::create (
        "tstaves", "trace-staves",
R"(Staves)",
        "traceStaves",
        fTraceStaves,
        fTracePasses));

  // staff details

  fTraceStaffDetails = boolOptionsInitialValue;

  scoreToVoicesTraceSubGroup->
    appendAtom (
      oahTwoBooleansAtom::create (
        "tst", "trace-staff-details",
R"(Staff details)",
        "traceStaffDetails",
        fTraceStaffDetails,
        fTracePasses));

  // staff changes

  fTraceStaffChanges = boolOptionsInitialValue;

  scoreToVoicesTraceSubGroup->
    appendAtom (
      oahTwoBooleansAtom::create (
        "tschanges", "trace-staff-changes",
R"(Staff changes)",
        "traceStaffChanges",
        fTraceStaffChanges,
        fTracePasses));

  // voices

  fTraceVoices = boolOptionsInitialValue;

  scoreToVoicesTraceSubGroup->
    appendAtom (
      oahTwoBooleansAtom::create (
        "tvoices", "trace-voices",
R"(Voices)",
        "traceVoices",
        fTraceVoices,
        fTracePasses));

  // voices details

  fTraceVoicesDetails = boolOptionsInitialValue;

  scoreToVoicesTraceSubGroup->
    appendAtom (
      oahThreeBooleansAtom::create (
        "tvoicesd", "trace-voices-details",
R"(Voices with more details (quite verbose)..
This option implies '-tvdetails, -traceVoicesDetails'.)",
        "traceVoicesDetails",
        fTraceVoicesDetails,
        fTraceVoices,
        fTracePasses));
}

void traceOptions::initializeNotesTraceOptions (
  bool boolOptionsInitialValue)
{
  S_oahSubGroup
    notesTraceSubGroup =
      oahSubGroup::create (
        "Notes trace",
        "hnt", "help-notes-trace",
R"()",
      kElementVisibilityHiddenByDefault,
      this);

  appendSubGroup (notesTraceSubGroup);

  // notes

  fTraceNotes = boolOptionsInitialValue;

  notesTraceSubGroup->
    appendAtom (
      oahTwoBooleansAtom::create (
        "tnotes", "trace-notes",
R"(Notes)",
        "traceNotes",
        fTraceNotes,
        fTracePasses));

  fTraceNotesDetails = boolOptionsInitialValue;

  notesTraceSubGroup->
    appendAtom (
      oahThreeBooleansAtom::create (
        "tnotesd", "trace-notes-details",
R"(Notes with more details, including divisions handling (quite verbose)...
This option implies '-tnnotes, -traceNotes'.)",
        "traceNotesDetails",
        fTraceNotesDetails,
        fTraceNotes,
        fTracePasses));

  fTraceWholeNotes = boolOptionsInitialValue;

  notesTraceSubGroup->
    appendAtom (
      oahTwoBooleansAtom::create (
        "twn", "trace-whole-notes",
R"(Whole notes computations (quite verbose)...)",
        "traceWholeNotes",
        fTraceWholeNotes,
        fTracePasses));

  fTraceRestNotes = boolOptionsInitialValue;

  notesTraceSubGroup->
    appendAtom (
      oahTwoBooleansAtom::create (
        "trn", "trace-rest-notes",
R"(Rest notes)",
        "traceRestNotes",
        fTraceRestNotes,
        fTracePasses));

  fTraceSkipNotes = boolOptionsInitialValue;

  notesTraceSubGroup->
    appendAtom (
      oahTwoBooleansAtom::create (
        "tsn", "trace-skip-notes",
R"(Skip notes)",
        "traceSkipNotes",
        fTraceSkipNotes,
        fTracePasses));

  fTraceNotesOctaveEntry = boolOptionsInitialValue;

  notesTraceSubGroup->
    appendAtom (
      oahTwoBooleansAtom::create (
        "tnoe", "trace-notes-octave-entry",
R"(Notes octave entry)",
        "traceNotesOctaveEntry",
        fTraceNotesOctaveEntry,
        fTracePasses));

  // grace notes

  fTraceGraceNotes = boolOptionsInitialValue;

  notesTraceSubGroup->
    appendAtom (
      oahTwoBooleansAtom::create (
        "tgraces", "trace-gracenotes",
R"(Grace notes)",
        "traceGraceNotes",
        fTraceGraceNotes,
        fTracePasses));

  // tremolos

  fTraceTremolos = boolOptionsInitialValue;

  notesTraceSubGroup->
    appendAtom (
      oahTwoBooleansAtom::create (
        "ttrems", "trace-tremolos",
R"(Tremolos)",
        "traceTremolos",
        fTraceTremolos,
        fTracePasses));
}

void traceOptions::initializeRepeatsToSlashesTraceOptions (
  bool boolOptionsInitialValue)
{
  S_oahSubGroup
    repeatsTraceSubGroup =
      oahSubGroup::create (
        "Repeats to slashes",
        "hrtst", "help-repeats-to-slashes-trace",
R"()",
      kElementVisibilityHiddenByDefault,
      this);

  appendSubGroup (repeatsTraceSubGroup);

  // repeats

  fTraceRepeats = boolOptionsInitialValue;
  fTraceRepeatsDetails = boolOptionsInitialValue;

  repeatsTraceSubGroup->
    appendAtom (
      oahTwoBooleansAtom::create (
        "treps", "trace-repeats",
R"(Repeats)",
        "traceRepeats",
        fTraceRepeats,
        fTracePasses));

  repeatsTraceSubGroup->
    appendAtom (
      oahThreeBooleansAtom::create (
        "trepsd", "trace-repeats-details",
R"(Repeats details)",
        "traceRepeats",
        fTraceRepeatsDetails,
        fTraceRepeats,
        fTracePasses));

  // measures repeats

  fTraceMeasuresRepeats = boolOptionsInitialValue;

  repeatsTraceSubGroup->
    appendAtom (
      oahTwoBooleansAtom::create (
        "tmreps", "trace-measures-repeats",
R"(Measures repeats)",
        "traceMeasureRepeats",
        fTraceMeasuresRepeats,
        fTracePasses));

  // rest measures

  fTraceRestMeasures = boolOptionsInitialValue;

  repeatsTraceSubGroup->
    appendAtom (
      oahTwoBooleansAtom::create (
        "trmeas", "trace-rest-measures",
R"(Multiple rests)",
        "traceRestMeasures",
        fTraceRestMeasures,
        fTracePasses));

  // beats repeats

  fTraceBeatsRepeats = boolOptionsInitialValue;

  repeatsTraceSubGroup->
    appendAtom (
      oahTwoBooleansAtom::create (
        "tbreps", "trace-beats-repeats",
R"(Beats repeatss)",
        "traceBeatsRepeats",
        fTraceBeatsRepeats,
        fTracePasses));

  // slashes

  fTraceSlashes = boolOptionsInitialValue;

  repeatsTraceSubGroup->
    appendAtom (
      oahTwoBooleansAtom::create (
        "tslashes", "trace-slashes",
R"(Slashes)",
        "traceSlashes",
        fTraceSlashes,
        fTracePasses));
}

void traceOptions::initializeOtherTraceOptions (
  bool boolOptionsInitialValue)
{
  S_oahSubGroup
    lowLevelTraceSubGroup =
      oahSubGroup::create (
        "Other",
        "hot", "help-other-trace",
R"()",
      kElementVisibilityHiddenByDefault,
      this);

  appendSubGroup (lowLevelTraceSubGroup);

  // passes

  fTracePasses = boolOptionsInitialValue; // TEMP boolOptionsInitialValue;

  lowLevelTraceSubGroup->
    appendAtom (
      oahBooleanAtom::create (
        "tpasses", "trace-passes",
R"(Write a trace of the passes to standard error.)",
        "tracePasses",
        fTracePasses));

  // geometry

  fTraceGeometry = boolOptionsInitialValue;

  lowLevelTraceSubGroup->
    appendAtom (
      oahTwoBooleansAtom::create (
        "tgeom", "trace-geometry",
R"(Geometry)",
        "traceGeometry",
        fTraceGeometry,
        fTracePasses));

  // varValAssocs

  fTraceVarValAssocs = boolOptionsInitialValue;

  lowLevelTraceSubGroup->
    appendAtom (
      oahTwoBooleansAtom::create (
        "tvva", "trace-var-val-assocs",
R"(VarValAssocs)",
        "traceVarValAssocs",
        fTraceVarValAssocs,
        fTracePasses));

  // '-trace-for-tests' is hidden...

  fTraceForTests = boolOptionsInitialValue;

  S_oahBooleanAtom
    traceForTestsOptionsBooleanAtom =
      oahBooleanAtom::create (
        "tft", "trace-for-tests",
R"(Write a trace for tests to standard error.)",
        "traceForTests",
        fTraceForTests);
  traceForTestsOptionsBooleanAtom->
    setIsHidden ();

  lowLevelTraceSubGroup->
    appendAtom (
      traceForTestsOptionsBooleanAtom);

/* JMI
  // MSR streams

  fTraceMsrStreams = boolOptionsInitialValue;

  lowLevelTraceSubGroup->
    appendAtom (
      oahTwoBooleansAtom::create (
        "tms", "traceMsrStreams",
R"(MSR Streams API for applications)",
        "traceMsrStreams",
        fTraceMsrStreams,
        fTracePasses));
*/
}

void traceOptions::initializeTraceOptions (
  bool boolOptionsInitialValue)
{
  // score to voices
  initializeScoreToVoicesTraceOptions (
    boolOptionsInitialValue);

  // segments and measures
  initializeSegmentsAndMeasuresTraceOptions (
    boolOptionsInitialValue);

  // notes
  initializeNotesTraceOptions (
    boolOptionsInitialValue);

  // notes attachments
  initializeNotesAttachmentsTraceOptions (
    boolOptionsInitialValue);

  // repeats to slashes
  initializeRepeatsToSlashesTraceOptions (
    boolOptionsInitialValue);

  // instruments
  initializeInstrumentsTraceOptions (
    boolOptionsInitialValue);

  // chords and tuplets
  initializeChordsAndTupletsTraceOptions (
    boolOptionsInitialValue);

  // texts
  initializeCreditsToWordsTraceOptions (
    boolOptionsInitialValue);

  // harmonies and figured basses
  initializeHarmoniesAndFiguredBassesTraceOptions (
    boolOptionsInitialValue);

  // spanners
  initializeSpannersTraceOptions (
    boolOptionsInitialValue);

  // inter-notes
  initializeInterNotesTraceOptions (
    boolOptionsInitialValue);

  // clefs to tempos
  initializeClefsToTemposTraceOptions (
    boolOptionsInitialValue);

  // breaks
  initializeBreaksAndBarlinesTraceOptions (
    boolOptionsInitialValue);

  // above staff
  initializeAboveStaffTraceOptions (
    boolOptionsInitialValue);

  //transpositions
  initializeTranspositionsTraceOptions (
    boolOptionsInitialValue);

  // other trace
  initializeOtherTraceOptions (
    boolOptionsInitialValue);
}

S_traceOptions traceOptions::createCloneWithTrueValues ()
{
  S_traceOptions
    clone =
      traceOptions::create (0);
      // 0 not to have it inserted twice in the option handler

  // set the options handler upLink
  clone->fHandlerUpLink =
    fHandlerUpLink;

  // specific trace
  // --------------------------------------

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

  // repeats trace
  clone->fTraceRepeats = true;
  clone->fTraceRepeatsDetails = true;

  // measures repeats
  clone->fTraceMeasuresRepeats = true;

  // rest measures
  clone->fTraceRestMeasures = true;

  // slashes
  clone->fTraceSlashes = true;

  // notes trace
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
  gLogOstream <<
    "The trace options are:" <<
    endl;

  gIndenter++;

  gLogOstream << left <<
    setw (fieldWidth) << "Trace:" <<
    endl;

  gIndenter++;

  gLogOstream << left <<
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

  gLogOstream << left <<
    setw (fieldWidth) << "traceDetailedMeasureNumbersSet" << " : " <<
    endl;

  if (fTraceDetailedMeasureNumbersSet.size ()) {
    set<int>::const_iterator
      iBegin = fTraceDetailedMeasureNumbersSet.begin (),
      iEnd   = fTraceDetailedMeasureNumbersSet.end (),
      i      = iBegin;

    gIndenter++;

    for ( ; ; ) {
      gLogOstream << "v " << (*i);
      if (++i == iEnd) break;
      // no endl here
    } // for

    gIndenter--;
  }
  else {
    gLogOstream <<
      "none";
  }
  gLogOstream << endl;

    // segments
  gLogOstream << left <<
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

S_oahValuedAtom traceOptions::handleAtom (
  ostream&  os,
  S_oahAtom atom)
{
  S_oahValuedAtom result;

  // JMI ???

  return result;
}

void traceOptions::printHelp (ostream& os)
{
  int undef = 3;

  // print the header and option names
  printGroupHeader (os);

  // print the description if any
  if (fDescription.size ()) {
    gIndenter++;
    os <<
      gIndenter.indentMultiLineString (
        fDescription) <<
      endl;
    gIndenter--;
  }

  // underline the options group header
  underlineGroupHeader (os);

  // print the options subgroups
  if (fSubGroupsList.size ()) {
    // compute the maximun sub group header length
    for (
      list<S_oahSubGroup>::const_iterator i = fSubGroupsList.begin ();
      i != fSubGroupsList.end ();
      i++
    ) {
      string subGroupHeader  = (*i)->getSubGroupHeader ();
      int subGroupHeaderSize = subGroupHeader.size ();

      if (subGroupHeaderSize > fMaximumSubGroupHeaderLength) {
        fMaximumSubGroupHeaderLength = subGroupHeaderSize;
      }
    } // for

    gIndenter++;

    list<S_oahSubGroup>::const_iterator
      iBegin = fSubGroupsList.begin (),
      iEnd   = fSubGroupsList.end (),
      i      = iBegin;
    for ( ; ; ) {
      S_oahSubGroup subGroup = (*i);

      // print the options subgroup help
      subGroup->printHelpWithHeaderWidth (
        os,
        fMaximumSubGroupHeaderLength);

      if (++i == iEnd) break;
  // JMI    os << endl;
    } // for

    gIndenter--;
  }

  // register help print action in options handler upLink
  fHandlerUpLink->setOptionsHandlerFoundAHelpOption ();
}

ostream& operator<< (ostream& os, const S_traceOptions& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
void initializeTraceOptionsHandling (
  S_oahHandler handler)
{
#ifdef TRACE_OPTIONS
  if (false && ! gGeneralOptions->fQuiet) { // JMI
    gLogOstream <<
      "Initializing trace options handling" <<
      endl;
  }
#endif

  // create the options variables
  // ------------------------------------------------------

  gTraceOptionsUserChoices = traceOptions::create (
    handler);
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
      kElementVisibilityAlways,
      this);

    appendSubGroup (traceAndDisplaySubGroup);

  // detailed measure numbers set

  // fTraceDetailedMeasureNumbersSet is empty

    traceAndDisplaySubGroup->
      appendAtom (
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

