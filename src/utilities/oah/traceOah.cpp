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

#include "utilities.h"

#include "enableTracingIfDesired.h"

#ifdef TRACING_IS_ENABLED

#include "traceOah.h"

#include "oahOah.h"
#include "generalOah.h"

#include "messagesHandling.h"

#include "oahAtomsCollection.h"


using namespace std;

namespace MusicXML2
{

//_______________________________________________________________________________
S_traceOahGroup gGlobalTraceOahGroup;

S_traceOahGroup traceOahGroup::create (
  S_oahPrefix shortTracePrefix,
  S_oahPrefix longTracePrefix)
{
  traceOahGroup* o = new traceOahGroup (
    shortTracePrefix,
    longTracePrefix);
  assert (o!=0);

  return o;
}

traceOahGroup::traceOahGroup (
  S_oahPrefix shortTracePrefix,
  S_oahPrefix longTracePrefix)
  : oahGroup (
      "OAH Trace",
      "ht", "help-trace",
  R"(There are trace options transversal to the successive passes,
  showing what's going on in the various translation activities.
  They're provided as a help to the maintainers, as well as for the curious.
  The options in this group can be quite verbose, use them with small input data!
  All of them imply '-tpasses, -trace-passes'.)",
      kElementVisibilityHeaderOnly)
{
  fShortTracePrefix = shortTracePrefix;
  fLongTracePrefix  = longTracePrefix;

  initializeTraceOahGroup ();
}

traceOahGroup::~traceOahGroup ()
{}

void traceOahGroup::initializePrintLayoutsTraceOah ()
{
  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Print layouts",
        "hpl", "help-print-layouts",
R"()",
      kElementVisibilityWhole,
      this);

  appendSubGroupToGroup (subGroup);

  // print layouts

  fTracePrintLayouts = false;

  S_oahTwoBooleansAtom
    tracePrintLayoutsAtom =
      oahTwoBooleansAtom::create (
        "tpl", "trace-print-layouts",
R"(Print layouts)",
        "tracePrintLayouts",
        fTracePrintLayouts,
        fTracePasses);
  subGroup->
    appendAtomToSubGroup (
      tracePrintLayoutsAtom);
}

void traceOahGroup::initializeTranspositionsTraceOah ()
{
  S_oahSubGroup
   subGroup =
      oahSubGroup::create (
        "Transpositions",
        "htt", "help-trace-transpositions",
R"()",
      kElementVisibilityWhole,
      this);

  appendSubGroupToGroup (subGroup);

  // transpositions

  fTraceTranspositions = false;

  S_oahTwoBooleansAtom
    traceTranspositionsAtom =
      oahTwoBooleansAtom::create (
        "ttransps", "trace-transpositions",
R"(Transpositions (<transpose/> in MusicXML, \transposition in LilyPond))",
        "traceTranspositions",
        fTraceTranspositions,
        fTracePasses);
  subGroup->
    appendAtomToSubGroup (
      traceTranspositionsAtom);

  // octave shifts

  fTraceOctaveShifts = false;

  S_oahTwoBooleansAtom
    traceOctaveShiftsAtom =
      oahTwoBooleansAtom::create (
        "toshifts", "trace-octaves-shifts",
R"(Octave shifts (<octave-shift/> in MusicXML, \ottava in LilyPond))",
        "traceOctaveShifts",
        fTraceOctaveShifts,
        fTracePasses);
  subGroup->
    appendAtomToSubGroup (
      traceOctaveShiftsAtom);
            /* JMI
*/
}

void traceOahGroup::initializeAboveStaffTraceOah ()
{
  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Above staff",
        "htas", "help-trace-above-staff",
R"()",
      kElementVisibilityWhole,
      this);

  appendSubGroupToGroup (subGroup);

  // the 'above staff' multiplex booleans atom

  S_oahMultiplexBooleansAtom
    aboveStaffMultiplexBooleansAtom =
      oahMultiplexBooleansAtom::create (
        "Trace SHORT_NAME/LONG_NAME in above staff.",
        "SHORT_NAME",
        "LONG_NAME",
        fShortTracePrefix,
        fLongTracePrefix);

  subGroup->
    appendAtomToSubGroup (
      aboveStaffMultiplexBooleansAtom);

  // rehearsals

  fTraceRehearsals = false;

  S_oahTwoBooleansAtom
    traceRehearsalsAtom =
      oahTwoBooleansAtom::create (
        "trehears", "trace-rehearsals",
R"(Rehearsals)",
        "traceRehearsals",
        fTraceRehearsals,
        fTracePasses);
  subGroup->
    appendAtomToSubGroup (
      traceRehearsalsAtom);
  aboveStaffMultiplexBooleansAtom->
    addBooleanAtom (
      traceRehearsalsAtom);

  // segnos

  fTraceSegnos = false;

  S_oahTwoBooleansAtom
    traceSegnosAtom =
      oahTwoBooleansAtom::create (
        "tsegnos", "trace-segnos",
R"(Segnos)",
        "traceSegnos",
        fTraceSegnos,
        fTracePasses);
  subGroup->
    appendAtomToSubGroup (
      traceSegnosAtom);
  aboveStaffMultiplexBooleansAtom->
    addBooleanAtom (
      traceSegnosAtom);

  // dal segnos

  fTraceDalSegnos = false;

  S_oahTwoBooleansAtom
    traceDalSegnosAtom =
      oahTwoBooleansAtom::create (
        "tdalsegnos", "trace-dal-segnos",
R"(Dal segnos)",
        "traceDalSegnos",
        fTraceDalSegnos,
        fTracePasses);
  subGroup->
    appendAtomToSubGroup (
      traceDalSegnosAtom);
  aboveStaffMultiplexBooleansAtom->
    addBooleanAtom (
      traceDalSegnosAtom);

  // codas

  fTraceCodas = false;

  S_oahTwoBooleansAtom
    traceCodasAtom =
      oahTwoBooleansAtom::create (
        "tcodas", "trace-codas",
R"(Codas)",
        "traceCodas",
        fTraceCodas,
        fTracePasses);
  subGroup->
    appendAtomToSubGroup (
      traceCodasAtom);
  aboveStaffMultiplexBooleansAtom->
    addBooleanAtom (
      traceCodasAtom);

  // eyeglasses

  fTraceEyeGlasses = false;

  S_oahTwoBooleansAtom
    traceEyeGlassesAtom =
      oahTwoBooleansAtom::create (
        "teyes", "trace-eyeglasses",
R"(Eyeglasses)",
        "traceEyeGlasses",
        fTraceEyeGlasses,
        fTracePasses);
  subGroup->
    appendAtomToSubGroup (
      traceEyeGlassesAtom);
  aboveStaffMultiplexBooleansAtom->
    addBooleanAtom (
      traceEyeGlassesAtom);
}

void traceOahGroup::initializeBreaksAndBarlinesTraceOah ()
{
  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Breaks and barlines",
        "htbab", "help-trace-breaks-and-barlines",
R"()",
      kElementVisibilityWhole,
      this);

  appendSubGroupToGroup (subGroup);

  // the 'breaks and barlines' multiplex booleans atom

  S_oahMultiplexBooleansAtom
    breaksAndBarlinesMultiplexBooleansAtom =
      oahMultiplexBooleansAtom::create (
        "Trace SHORT_NAME/LONG_NAME in breaks and barlines.",
        "SHORT_NAME",
        "LONG_NAME",
        fShortTracePrefix,
        fLongTracePrefix);

  subGroup->
    appendAtomToSubGroup (
      breaksAndBarlinesMultiplexBooleansAtom);

  // line breaks

  fTraceLineBreaks = false;

  S_oahTwoBooleansAtom
    traceLineBreaksAtom =
      oahTwoBooleansAtom::create (
        "tlbreaks", "trace-line-breaks",
R"(Line breaks)",
        "traceLineBreaks",
        fTraceLineBreaks,
        fTracePasses);
  subGroup->
    appendAtomToSubGroup (
      traceLineBreaksAtom);
  breaksAndBarlinesMultiplexBooleansAtom->
    addBooleanAtom (
      traceLineBreaksAtom);

  // page breaks

  fTracePageBreaks = false;

  S_oahTwoBooleansAtom
    tracePageBreaksAtom =
      oahTwoBooleansAtom::create (
        "tpbreaks", "trace-page-breaks",
R"(Page breaks)",
        "tracePageBreaks",
        fTracePageBreaks,
        fTracePasses);
  subGroup->
    appendAtomToSubGroup (
      tracePageBreaksAtom);
  breaksAndBarlinesMultiplexBooleansAtom->
    addBooleanAtom (
      tracePageBreaksAtom);

  // barlines

  fTraceBarlines = false;

  S_oahTwoBooleansAtom
    traceBarlinesAtom =
      oahTwoBooleansAtom::create (
        "tblines", "trace-barlines",
R"(Barlines)",
        "traceBarlines",
        fTraceBarlines,
        fTracePasses);
  subGroup->
    appendAtomToSubGroup (
      traceBarlinesAtom);
  breaksAndBarlinesMultiplexBooleansAtom->
    addBooleanAtom (
      traceBarlinesAtom);

  // barlines details

  fTraceBarlinesDetails = false;

  S_oahThreeBooleansAtom
    traceBarlinesDetailsAtom =
      oahThreeBooleansAtom::create (
        "tblinesd", "trace-barlines-details",
R"(Barlines details)",
        "traceBarlinesDetails",
        fTraceBarlinesDetails,
        fTraceBarlines,
        fTracePasses);
  subGroup->
    appendAtomToSubGroup (
      traceBarlinesDetailsAtom);
  breaksAndBarlinesMultiplexBooleansAtom->
    addBooleanAtom (
      traceBarlinesDetailsAtom);

  // bar checks

  fTraceBarChecks = false;

  S_oahThreeBooleansAtom
    traceBarChecksAtom =
      oahThreeBooleansAtom::create (
        "tbc", "trace-bar-checks",
R"(Bar checks)",
        "traceBarChecks",
        fTraceBarChecks,
        fTraceBarlines,
        fTracePasses);
  subGroup->
    appendAtomToSubGroup (
      traceBarChecksAtom);
  breaksAndBarlinesMultiplexBooleansAtom->
    addBooleanAtom (
      traceBarChecksAtom);

  // bar number checks

  fTraceBarNumberChecks = false;

  S_oahThreeBooleansAtom
    traceBarNumberChecksAtom =
      oahThreeBooleansAtom::create (
        "tbnc", "trace-bar-number-checks",
R"(Bar number checks)",
        "traceBarNumberChecks",
        fTraceBarNumberChecks,
        fTraceBarlines,
        fTracePasses);
  subGroup->
    appendAtomToSubGroup (
      traceBarNumberChecksAtom);
  breaksAndBarlinesMultiplexBooleansAtom->
    addBooleanAtom (
      traceBarNumberChecksAtom);
}

void traceOahGroup::initializeClefsToTemposTraceOah ()
{
  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Clefs to tempos",
        "htctt", "help-trace-clefs-to-tempos",
R"()",
      kElementVisibilityWhole,
      this);

  appendSubGroupToGroup (subGroup);

  // the 'clefs to tempo' multiplex booleans atom

  S_oahMultiplexBooleansAtom
    clefsToTemposMultiplexBooleansAtom =
      oahMultiplexBooleansAtom::create (
        "Trace SHORT_NAME/LONG_NAME in clefs to tempo.",
        "SHORT_NAME",
        "LONG_NAME",
        fShortTracePrefix,
        fLongTracePrefix);

  subGroup->
    appendAtomToSubGroup (
      clefsToTemposMultiplexBooleansAtom);

  // clefs

  fTraceClefs = false;

  S_oahTwoBooleansAtom
    traceClefsAtom =
      oahTwoBooleansAtom::create (
        "tclefs", "trace-clefs",
R"(Clefs)",
        "traceClefs",
        fTraceClefs,
        fTracePasses);
  subGroup->
    appendAtomToSubGroup (
      traceClefsAtom);
  clefsToTemposMultiplexBooleansAtom->
    addBooleanAtom (
      traceClefsAtom);

  // keys

  fTraceKeys  = false;

  S_oahTwoBooleansAtom
    traceKeysAtom =
      oahTwoBooleansAtom::create (
        "tkeys", "trace-keys",
R"(Keys)",
        "traceKeys",
        fTraceKeys,
        fTracePasses);
  subGroup->
    appendAtomToSubGroup (
      traceKeysAtom);
  clefsToTemposMultiplexBooleansAtom->
    addBooleanAtom (
      traceKeysAtom);

  // times

  fTraceTimes = false;

  S_oahTwoBooleansAtom
    traceTimesAtom =
      oahTwoBooleansAtom::create (
        "ttimes", "trace-times",
R"(Times)",
        "traceTimes",
        fTraceTimes,
        fTracePasses);
  subGroup->
    appendAtomToSubGroup (
      traceTimesAtom);
  clefsToTemposMultiplexBooleansAtom->
    addBooleanAtom (
      traceTimesAtom);

  // tempos

  fTraceTempos = false;

  S_oahTwoBooleansAtom
    traceTemposAtom =
      oahTwoBooleansAtom::create (
        "ttempos", "trace-tempos",
R"(Tempos)",
        "traceTempos",
        fTraceTempos,
        fTracePasses);
  subGroup->
    appendAtomToSubGroup (
      traceTemposAtom);
  clefsToTemposMultiplexBooleansAtom->
    addBooleanAtom (
      traceTemposAtom);
}

void traceOahGroup::initializeInterNotesTraceOah ()
{
  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Inter-notes",
        "htin", "help-trace-inter-notes",
R"()",
      kElementVisibilityWhole,
      this);

  appendSubGroupToGroup (subGroup);

  // the 'inter notes' multiplex booleans atom

  S_oahMultiplexBooleansAtom
    interNotesMultiplexBooleansAtom =
      oahMultiplexBooleansAtom::create (
        "Trace SHORT_NAME/LONG_NAME in inter notes.",
        "SHORT_NAME",
        "LONG_NAME",
        fShortTracePrefix,
        fLongTracePrefix);

  subGroup->
    appendAtomToSubGroup (
      interNotesMultiplexBooleansAtom);

  // ties

  fTraceTies = false;

  S_oahTwoBooleansAtom
    traceTiesAtom =
      oahTwoBooleansAtom::create (
        "tties", "trace-ties",
R"(Ties)",
        "traceTies",
        fTraceTies,
        fTracePasses);
  subGroup->
    appendAtomToSubGroup (
      traceTiesAtom);
  interNotesMultiplexBooleansAtom->
    addBooleanAtom (
      traceTiesAtom);

  // glissandos

  fTraceGlissandos = false;

  S_oahTwoBooleansAtom
    traceGlissandosAtom =
      oahTwoBooleansAtom::create (
        "tgliss", "trace-glissandos",
R"(Glissandos)",
        "traceGlissandos",
        fTraceGlissandos,
        fTracePasses);
  subGroup->
    appendAtomToSubGroup (
      traceGlissandosAtom);
  interNotesMultiplexBooleansAtom->
    addBooleanAtom (
      traceGlissandosAtom);
}

void traceOahGroup::initializeSpannersTraceOah ()
{
  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Spanners",
        "hts", "help-trace-spanners",
R"()",
      kElementVisibilityWhole,
      this);

  appendSubGroupToGroup (subGroup);

  // the 'spanners' multiplex booleans atom

  S_oahMultiplexBooleansAtom
    spannersMultiplexBooleansAtom =
      oahMultiplexBooleansAtom::create (
        "Trace SHORT_NAME/LONG_NAME in spanners.",
        "SHORT_NAME",
        "LONG_NAME",
        fShortTracePrefix,
        fLongTracePrefix);

  subGroup->
    appendAtomToSubGroup (
      spannersMultiplexBooleansAtom);

  // spanners

  fTraceSpanners = false;

  S_oahTwoBooleansAtom
    traceSpannersAtom =
      oahTwoBooleansAtom::create (
        "tspans", "trace-spanners",
R"(Spanners)",
        "traceSpanners",
        fTraceSpanners,
        fTracePasses);
  subGroup->
    appendAtomToSubGroup (
      traceSpannersAtom);
  spannersMultiplexBooleansAtom->
    addBooleanAtom (
      traceSpannersAtom);

  // wedges

  fTraceWedges = false;

  S_oahTwoBooleansAtom
    traceWedgesAtom =
      oahTwoBooleansAtom::create (
        "twedges", "trace-wedges",
R"(Wedges)",
        "traceWedges",
        fTraceWedges,
        fTracePasses);
  subGroup->
    appendAtomToSubGroup (
      traceWedgesAtom);
  spannersMultiplexBooleansAtom->
    addBooleanAtom (
      traceWedgesAtom);

  // slurs

  fTraceSlurs = false;

  S_oahTwoBooleansAtom
    traceSlursAtom =
      oahTwoBooleansAtom::create (
        "tslurs", "trace-slurs",
R"(Slurs)",
        "traceSlurs",
        fTraceSlurs,
        fTracePasses);
  subGroup->
    appendAtomToSubGroup (
      traceSlursAtom);
  spannersMultiplexBooleansAtom->
    addBooleanAtom (
      traceSlursAtom);

  // slurs details

  fTraceSlursDetails = false;

  S_oahThreeBooleansAtom
    traceSlursDetailsAtom =
      oahThreeBooleansAtom::create (
        "tslursd", "trace-slurs-details",
R"(Slurs details)",
        "traceSlursDetails",
        fTraceSlursDetails,
        fTraceSlurs,
        fTracePasses);
  subGroup->
    appendAtomToSubGroup (
      traceSlursDetailsAtom);
  spannersMultiplexBooleansAtom->
    addBooleanAtom (
      traceSlursDetailsAtom);

  // ligatures

  fTraceLigatures = false;

  S_oahTwoBooleansAtom
    traceLigaturesAtom =
      oahTwoBooleansAtom::create (
        "tligs", "trace-ligatures",
R"(Ligatures)",
        "traceLigatures",
        fTraceLigatures,
        fTracePasses);
  subGroup->
    appendAtomToSubGroup (
      traceLigaturesAtom);
  spannersMultiplexBooleansAtom->
    addBooleanAtom (
      traceLigaturesAtom);
}

void traceOahGroup::initializeHarmoniesTraceOah ()
{
  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Harmonies",
        "htharms", "help-trace-harmonies",
R"()",
      kElementVisibilityWhole,
      this);

  appendSubGroupToGroup (subGroup);

/* JMI
  // the 'harmonies and figured basses' multiplex booleans atom

  S_oahMultiplexBooleansAtom
    harmoniesAndFiguredBassesMultiplexBooleansAtom =
      oahMultiplexBooleansAtom::create (
        "Trace SHORT_NAME/LONG_NAME in harmonies and figured basses.",
        "SHORT_NAME",
        "LONG_NAME",
        fShortTracePrefix,
        fLongTracePrefix);

  subGroup->
    appendAtomToSubGroup (
      harmoniesAndFiguredBassesMultiplexBooleansAtom);
*/

  // harmonies

  fTraceHarmonies = false;

  S_oahTwoBooleansAtom
    traceHarmoniesAtom =
      oahTwoBooleansAtom::create (
        "tharms", "trace-harmonies",
R"(<harmony/> in MusicXML, \chordmode in LilyPond)",
        "traceHarmonies",
        fTraceHarmonies,
        fTracePasses);
  subGroup->
    appendAtomToSubGroup (
      traceHarmoniesAtom);
      /* JMI
  harmoniesAndFiguredBassesMultiplexBooleansAtom->
    addBooleanAtom (
      traceHarmoniesAtom);
*/

  // harmonies details

  fTraceHarmoniesDetails = false;

  S_oahTwoBooleansAtom
    traceHarmoniesDetailsAtom =
      oahTwoBooleansAtom::create (
        "tharmsd", "trace-harmonies-details",
R"(<harmony/> in MusicXML, \chordmode in LilyPond)",
        "traceHarmoniesDetails",
        fTraceHarmoniesDetails,
        fTracePasses);
  subGroup->
    appendAtomToSubGroup (
      traceHarmoniesDetailsAtom);

  // extra harmonies

  fTraceExtraHarmonies = false;

  S_oahTwoBooleansAtom
    traceExtraHarmoniesAtom =
      oahTwoBooleansAtom::create (
        "teharms", "trace-extra-harmonies",
R"(<harmony/> in MusicXML, \chordmode in LilyPond)",
        "traceExtraHarmonies",
        fTraceExtraHarmonies,
        fTracePasses);
  subGroup->
    appendAtomToSubGroup (
      traceExtraHarmoniesAtom);
}

void traceOahGroup::initializeFiguredBassesTraceOah ()
{
  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Figured basses",
        "hthfigbass", "help-trace-figured-basses",
R"()",
      kElementVisibilityWhole,
      this);

  appendSubGroupToGroup (subGroup);

  // figured basses

  fTraceFiguredBasses = false;

  S_oahTwoBooleansAtom
    traceFiguredBasseAtom =
      oahTwoBooleansAtom::create (
        "tfigbass", "trace-figured-basses",
R"(<figured-bass> in MusicXML, \figuremode in LilyPond)",
        "traceFiguredBasses",
        fTraceFiguredBasses,
        fTracePasses);
  subGroup->
    appendAtomToSubGroup (
      traceFiguredBasseAtom);
}

void traceOahGroup::initializeCreditsToWordsTraceOah ()
{
  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Credits to words",
        "htctw", "help-trace-credits-to-words",
R"()",
      kElementVisibilityWhole,
      this);

  appendSubGroupToGroup (subGroup);

  // the 'credits to words' multiplex booleans atom

  S_oahMultiplexBooleansAtom
    creditsToWordsMultiplexBooleansAtom =
      oahMultiplexBooleansAtom::create (
        "Trace SHORT_NAME/LONG_NAME in 'credits to words'.",
        "SHORT_NAME",
        "LONG_NAME",
        fShortTracePrefix,
        fLongTracePrefix);

  subGroup->
    appendAtomToSubGroup (
      creditsToWordsMultiplexBooleansAtom);

  // credits

  fTraceCredits = false;

  S_oahTwoBooleansAtom
    traceCreditsBooleanAtom =
      oahTwoBooleansAtom::create (
        "tcredits", "trace-credits",
R"(Credits)",
        "traceCredits",
        fTraceCredits,
        fTracePasses);
  subGroup->
    appendAtomToSubGroup (
      traceCreditsBooleanAtom);
  creditsToWordsMultiplexBooleansAtom->
    addBooleanAtom (
      traceCreditsBooleanAtom);

  // lyrics

  fTraceLyrics = false;

  S_oahTwoBooleansAtom
    traceLyricsBooleanAtom =
      oahTwoBooleansAtom::create (
        "tlyrics", "trace-lyrics",
R"(Lyrics)",
        "traceLyrics",
        fTraceLyrics,
        fTracePasses);
  subGroup->
    appendAtomToSubGroup (
      traceLyricsBooleanAtom);
  creditsToWordsMultiplexBooleansAtom->
    addBooleanAtom (
      traceLyricsBooleanAtom);

  // lyrics details

  fTraceLyricsDetails = false;

  S_oahTwoBooleansAtom
    traceLyricsDetailsBooleanAtom =
      oahTwoBooleansAtom::create (
        "tlyricsd", "trace-lyrics-details",
R"(Lyrics in MusicXML, stanzas in MSR)",
        "traceLyricsDetails",
        fTraceLyricsDetails,
        fTracePasses);
  subGroup->
    appendAtomToSubGroup (
      traceLyricsDetailsBooleanAtom);
  creditsToWordsMultiplexBooleansAtom->
    addBooleanAtom (
      traceLyricsDetailsBooleanAtom);

  // words

  fTraceWords = false;

  S_oahTwoBooleansAtom
    traceWordsBooleanAtom =
      oahTwoBooleansAtom::create (
        "twords", "trace-words",
R"(Words)",
        "traceWords",
        fTraceWords,
        fTracePasses);
  subGroup->
    appendAtomToSubGroup (
      traceWordsBooleanAtom);
  creditsToWordsMultiplexBooleansAtom->
    addBooleanAtom (
      traceWordsBooleanAtom);
}

void traceOahGroup::initializeChordsAndTupletsTraceOah ()
{
  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Chords and tuplets",
        "htcat", "help-trace-chords-and-tuplets",
R"()",
      kElementVisibilityWhole,
      this);

  appendSubGroupToGroup (subGroup);

  // the 'chords and tuplets' multiplex booleans atom

  S_oahMultiplexBooleansAtom
    chordsAndTupletsMultiplexBooleansAtom =
      oahMultiplexBooleansAtom::create (
        "Trace SHORT_NAME/LONG_NAME in chords and tuplets.",
        "SHORT_NAME",
        "LONG_NAME",
        fShortTracePrefix,
        fLongTracePrefix);

  subGroup->
    appendAtomToSubGroup (
      chordsAndTupletsMultiplexBooleansAtom);

  // chords

  fTraceChords = false;

  S_oahTwoBooleansAtom
    taceChordsBooleanAtom =
      oahTwoBooleansAtom::create (
        "tchords", "trace-chords",
R"(Chords)",
        "traceChords",
        fTraceChords,
        fTracePasses);
  subGroup->
    appendAtomToSubGroup (
      taceChordsBooleanAtom);
  chordsAndTupletsMultiplexBooleansAtom->
    addBooleanAtom (
      taceChordsBooleanAtom);

  // chords details

  fTraceChordsDetails = false;

  S_oahThreeBooleansAtom
    traceChordsDetailsBooleanAtom =
      oahThreeBooleansAtom::create (
        "tchordsd", "trace-chords-details",
R"(Chords details)",
        "traceChordsDetails",
        fTraceChordsDetails,
        fTraceChords,
        fTracePasses);
  subGroup->
    appendAtomToSubGroup (
      traceChordsDetailsBooleanAtom);
  chordsAndTupletsMultiplexBooleansAtom->
    addBooleanAtom (
      traceChordsDetailsBooleanAtom);

  // tuplets

  fTraceTuplets = false;

  S_oahTwoBooleansAtom
    traceTupletsBooleanAtom =
      oahTwoBooleansAtom::create (
        "ttups", "trace-tuplets",
R"(Tuplets)",
        "traceTuplets",
        fTraceTuplets,
        fTracePasses);
  subGroup->
    appendAtomToSubGroup (
      traceTupletsBooleanAtom);
  chordsAndTupletsMultiplexBooleansAtom->
    addBooleanAtom (
      traceTupletsBooleanAtom);

  // tuplets details

  fTraceTupletsDetails = false;

  S_oahThreeBooleansAtom
    traceTupletsDetailsBooleanAtom =
      oahThreeBooleansAtom::create (
        "ttupsd", "trace-tuplets-details",
R"(Tuplets details)",
        "traceTupletsDetails",
        fTraceTupletsDetails,
        fTraceTuplets,
        fTracePasses);
  subGroup->
    appendAtomToSubGroup (
      traceTupletsDetailsBooleanAtom);
  chordsAndTupletsMultiplexBooleansAtom->
    addBooleanAtom (
      traceTupletsDetailsBooleanAtom);
}

void traceOahGroup::initializeInstrumentsTraceOah ()
{
  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Instruments",
        "hti", "help-trace-instruments",
R"()",
      kElementVisibilityWhole,
      this);

  appendSubGroupToGroup (subGroup);

  // the 'instruments' multiplex booleans atom

  S_oahMultiplexBooleansAtom
    instrumentsMultiplexBooleansAtom =
      oahMultiplexBooleansAtom::create (
        "Trace SHORT_NAME/LONG_NAME in instruments.",
        "SHORT_NAME",
        "LONG_NAME",
        fShortTracePrefix,
        fLongTracePrefix);

  subGroup->
    appendAtomToSubGroup (
      instrumentsMultiplexBooleansAtom);

  // frames

  fTraceFrames = false;

  S_oahTwoBooleansAtom
    traceFramesBooleanAtom =
      oahTwoBooleansAtom::create (
        "tframes", "trace-frames",
R"(<frame/> in MusicXML, \fret-diagram in LilyPond)",
        "traceFrames",
        fTraceFrames,
        fTracePasses);
  subGroup->
    appendAtomToSubGroup (
      traceFramesBooleanAtom);
      /* JMI
  instrumentsMultiplexBooleansAtom->
    addBooleanAtom (
      traceFramesBooleanAtom);
      */

  // scordaturas

  fTraceScordaturas = false;

  S_oahTwoBooleansAtom
    traceScordaturasBooleanAtom =
      oahTwoBooleansAtom::create (
        "tscords", "trace-scordaturas",
R"(Scordaturas)",
        "traceScordaturas",
        fTraceScordaturas,
        fTracePasses);
  subGroup->
    appendAtomToSubGroup (
      traceScordaturasBooleanAtom);
  instrumentsMultiplexBooleansAtom->
    addBooleanAtom (
      traceScordaturasBooleanAtom);

  // slides

  fTraceSlides = false;

  S_oahTwoBooleansAtom
    traceSlidesBooleanAtom =
      oahTwoBooleansAtom::create (
        "tslides", "trace-slides",
R"(Slides)",
        "traceSlides",
        fTraceSlides,
        fTracePasses);
  subGroup->
    appendAtomToSubGroup (
      traceSlidesBooleanAtom);
  instrumentsMultiplexBooleansAtom->
    addBooleanAtom (
      traceSlidesBooleanAtom);

  // pedals

  fTracePedals = false;

  S_oahTwoBooleansAtom
    tracePedalsBooleanAtom =
      oahTwoBooleansAtom::create (
        "tpedals", "trace-pedals",
R"(Pedals)",
        "tracePedals",
        fTracePedals,
        fTracePasses);
  subGroup->
    appendAtomToSubGroup (
      tracePedalsBooleanAtom);
  instrumentsMultiplexBooleansAtom->
    addBooleanAtom (
      tracePedalsBooleanAtom);

  // accordion registrations

  fTraceAccordionRegistrations = false;

  // accordion registrations
  S_oahTwoBooleansAtom
    traceAccordionRegistrationsBooleanAtom =
      oahTwoBooleansAtom::create (
        "taccreg", "trace-accordion-registrations",
R"(Accordion registrations)",
        "traceAccordionRegistrations",
        fTraceAccordionRegistrations,
        fTracePasses);
  subGroup->
    appendAtomToSubGroup (
      traceAccordionRegistrationsBooleanAtom);
  instrumentsMultiplexBooleansAtom->
    addBooleanAtom (
      traceAccordionRegistrationsBooleanAtom);

  // harp pedals

  fTraceHarpPedals = false;

  S_oahTwoBooleansAtom
    traceHarpPedalsBooleanAtom =
      oahTwoBooleansAtom::create (
        "thpedals", "trace-harp-pedals",
R"(Harp pedals)",
        "traceHarpPedals",
        fTraceHarpPedals,
        fTracePasses);
  subGroup->
    appendAtomToSubGroup (
      traceHarpPedalsBooleanAtom);
  instrumentsMultiplexBooleansAtom->
    addBooleanAtom (
      traceHarpPedalsBooleanAtom);

  // harp pedals tuning

  fTraceHarpPedalsTunings = false;

  S_oahTwoBooleansAtom
    traceHarpPedalsTuningsBooleanAtom =
      oahTwoBooleansAtom::create (
        "thptuns", "traceHarpPedalsTunings",
R"(Harp pedals tuning)",
        "traceHarpPedalsTunings",
        fTraceHarpPedalsTunings,
        fTracePasses);
  subGroup->
    appendAtomToSubGroup (
      traceHarpPedalsTuningsBooleanAtom);
  instrumentsMultiplexBooleansAtom->
    addBooleanAtom (
      traceHarpPedalsTuningsBooleanAtom);

  // damps

  fTraceDamps = false;

  S_oahTwoBooleansAtom
    traceDampsBooleanAtom =
      oahTwoBooleansAtom::create (
        "tdamps", "trace-damps",
R"(Damps)",
        "traceDamps",
        fTraceDamps,
        fTracePasses);
  subGroup->
    appendAtomToSubGroup (
      traceDampsBooleanAtom);
  instrumentsMultiplexBooleansAtom->
    addBooleanAtom (
      traceDampsBooleanAtom);

  // dampalls

  fTraceDampAlls = false;

  S_oahTwoBooleansAtom
    traceDampAllsBooleanAtom =
      oahTwoBooleansAtom::create (
        "tdampalls", "trace-dampalls",
R"(Dampalls)",
        "traceDampAlls",
        fTraceDampAlls,
        fTracePasses);
  subGroup->
    appendAtomToSubGroup (
      traceDampAllsBooleanAtom);
  instrumentsMultiplexBooleansAtom->
    addBooleanAtom (
      traceDampAllsBooleanAtom);

  // MIDI

  fTraceMidi = false;

  S_oahTwoBooleansAtom
    traceMidiBooleanAtom =
      oahTwoBooleansAtom::create (
        "tmidi", "trace-midi",
R"(MIDI)",
        "traceMidi",
        fTraceMidi,
        fTracePasses);
  subGroup->
    appendAtomToSubGroup (
      traceMidiBooleanAtom);
  instrumentsMultiplexBooleansAtom->
    addBooleanAtom (
      traceMidiBooleanAtom);
}

void traceOahGroup::initializeNotesAttachmentsTraceOah ()
{
  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Notes attachments",
        "htna", "help-trace-notes-attachments",
R"()",
      kElementVisibilityWhole,
      this);

  appendSubGroupToGroup (subGroup);

  // the 'notes attachments' multiplex booleans atom

  S_oahMultiplexBooleansAtom
    notesAttachmentsMultiplexBooleansAtom =
      oahMultiplexBooleansAtom::create (
        "Trace SHORT_NAME/LONG_NAME in notes attachments.",
        "SHORT_NAME",
        "LONG_NAME",
        fShortTracePrefix,
        fLongTracePrefix);

  subGroup->
    appendAtomToSubGroup (
      notesAttachmentsMultiplexBooleansAtom);

  // stems

  fTraceStems = false;

  S_oahTwoBooleansAtom
    traceStemsBooleanAtom =
      oahTwoBooleansAtom::create (
        "tstems", "trace-stems",
R"(Stems)",
        "traceStems",
        fTraceStems,
        fTracePasses);
  subGroup->
    appendAtomToSubGroup (
      traceStemsBooleanAtom);
  notesAttachmentsMultiplexBooleansAtom->
    addBooleanAtom (
      traceStemsBooleanAtom);

  // beams

  fTraceBeams = false;

  S_oahTwoBooleansAtom
    traceBeamsBooleanAtom =
      oahTwoBooleansAtom::create (
        "tbeams", "trace-beams",
R"(Beams)",
        "traceBeams",
        fTraceBeams,
        fTracePasses);
  subGroup->
    appendAtomToSubGroup (
      traceBeamsBooleanAtom);
  notesAttachmentsMultiplexBooleansAtom->
    addBooleanAtom (
      traceBeamsBooleanAtom);

  // articulations

  fTraceArticulations = false;

  S_oahTwoBooleansAtom
    traceArticulationsBooleanAtom =
      oahTwoBooleansAtom::create (
        "tarts", "trace-articulations",
R"(Articulations)",
        "traceArticulations",
        fTraceArticulations,
        fTracePasses);
  subGroup->
    appendAtomToSubGroup (
      traceArticulationsBooleanAtom);
  notesAttachmentsMultiplexBooleansAtom->
    addBooleanAtom (
      traceArticulationsBooleanAtom);

  // technicals

  fTraceTechnicals = false;

  S_oahTwoBooleansAtom
    traceTechnicalsBooleanAtom =
      oahTwoBooleansAtom::create (
        "ttechs", "trace-technicals",
R"(Technicals)",
        "traceTechnicals",
        fTraceTechnicals,
        fTracePasses);
  subGroup->
    appendAtomToSubGroup (
      traceTechnicalsBooleanAtom);
  notesAttachmentsMultiplexBooleansAtom->
    addBooleanAtom (
      traceTechnicalsBooleanAtom);

  // ornaments

  fTraceOrnaments = false;

  S_oahTwoBooleansAtom
    traceOrnamentsBooleanAtom =
      oahTwoBooleansAtom::create (
        "torns", "trace-ornaments",
R"(Ornaments)",
        "traceOrnaments",
        fTraceOrnaments,
        fTracePasses);
  subGroup->
    appendAtomToSubGroup (
      traceOrnamentsBooleanAtom);
  notesAttachmentsMultiplexBooleansAtom->
    addBooleanAtom (
      traceOrnamentsBooleanAtom);

  // dynamics

  fTraceDynamics = false;

  S_oahTwoBooleansAtom
    traceDynamicsBooleanAtom =
      oahTwoBooleansAtom::create (
        "tdyns", "trace-dynamics",
R"(Dynamics)",
        "traceDynamics",
        fTraceDynamics,
        fTracePasses);
  subGroup->
    appendAtomToSubGroup (
      traceDynamicsBooleanAtom);
  notesAttachmentsMultiplexBooleansAtom->
    addBooleanAtom (
      traceDynamicsBooleanAtom);
}

void traceOahGroup::initializeSegmentsAndMeasuresTraceOah ()
{
  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Segments and measures",
        "htsam", "help-trace-segments-and-measures",
R"()",
      kElementVisibilityWhole,
      this);

  appendSubGroupToGroup (subGroup);

  // the 'segments and measures' multiplex booleans atom

  S_oahMultiplexBooleansAtom
    segmentsAndMeasuresMultiplexBooleansAtom =
      oahMultiplexBooleansAtom::create (
        "Trace SHORT_NAME/LONG_NAME in segments and measures.",
        "SHORT_NAME",
        "LONG_NAME",
        fShortTracePrefix,
        fLongTracePrefix);

  subGroup->
    appendAtomToSubGroup (
      segmentsAndMeasuresMultiplexBooleansAtom);

  // segments

  fTraceSegments = false;

  S_oahTwoBooleansAtom
    traceSegmentsBooleanAtom =
      oahTwoBooleansAtom::create (
        "tsegs", "trace-segments",
R"(Voices segments)",
        "traceSegments",
        fTraceSegments,
        fTracePasses);
  subGroup->
    appendAtomToSubGroup (
      traceSegmentsBooleanAtom);
  segmentsAndMeasuresMultiplexBooleansAtom->
    addBooleanAtom (
      traceSegmentsBooleanAtom);

  // segments details

  fTraceSegmentsDetails = false;

  S_oahThreeBooleansAtom
    traceSegmentsDetailsBooleanAtom =
      oahThreeBooleansAtom::create (
        "tsegsd", "trace-segments-details",
R"(Voices segments details)",
        "traceSegments",
        fTraceSegmentsDetails,
        fTraceSegments,
        fTracePasses);
  subGroup->
    appendAtomToSubGroup (
      traceSegmentsDetailsBooleanAtom);
  segmentsAndMeasuresMultiplexBooleansAtom->
    addBooleanAtom (
      traceSegmentsDetailsBooleanAtom);

  // measure numbers

  fTraceMeasuresNumbers = false;

  S_oahTwoBooleansAtom
    traceMeasuresNumbersBooleanAtom =
      oahTwoBooleansAtom::create (
        "tmn", "trace-measure-numbers",
R"(Measure numberss)",
        "traceMeasuresNumbers",
        fTraceMeasuresNumbers,
        fTracePasses);
  subGroup->
    appendAtomToSubGroup (
      traceMeasuresNumbersBooleanAtom);
  segmentsAndMeasuresMultiplexBooleansAtom->
    addBooleanAtom (
      traceMeasuresNumbersBooleanAtom);

  // measures

  fTraceMeasures = false;

  S_oahTwoBooleansAtom
    traceMeasuresBooleanAtom =
      oahTwoBooleansAtom::create (
        "tmeas", "trace-measures",
R"(Measures)",
        "traceMeasures",
        fTraceMeasures,
        fTracePasses);
  subGroup->
    appendAtomToSubGroup (
      traceMeasuresBooleanAtom);
  segmentsAndMeasuresMultiplexBooleansAtom->
    addBooleanAtom (
      traceMeasuresBooleanAtom);

  // measures details

  fTraceMeasuresDetails = false;

  S_oahThreeBooleansAtom
    traceMeasuresDetailsBooleanAtom =
      oahThreeBooleansAtom::create (
        "tmeasd", "trace-measures-details",
R"(Measures details)",
        "traceMeasuresDetails",
        fTraceMeasuresDetails,
        fTraceMeasures,
        fTracePasses);
  subGroup->
    appendAtomToSubGroup (
      traceMeasuresDetailsBooleanAtom);
  segmentsAndMeasuresMultiplexBooleansAtom->
    addBooleanAtom (
      traceMeasuresDetailsBooleanAtom);

  // positions in measures

  fTracePositionsInMeasures = false;

  S_oahTwoBooleansAtom
    tracePositionsInMeasuresBooleanAtom =
      oahTwoBooleansAtom::create (
        "tpim", "trace-positions-in-measures",
R"(Positions in measures)",
        "tracePositionsInMeasures",
        fTracePositionsInMeasures,
        fTracePasses);
  subGroup->
    appendAtomToSubGroup (
      tracePositionsInMeasuresBooleanAtom);
  segmentsAndMeasuresMultiplexBooleansAtom->
    addBooleanAtom (
      tracePositionsInMeasuresBooleanAtom);

  // moments in measures

  fTraceMomentsInMeasures = false;

  S_oahTwoBooleansAtom
    traceMomentsInMeasuresBooleanAtom =
      oahTwoBooleansAtom::create (
        "tmim", "trace-moments-in-measures",
R"(Moments in measures)",
        "traceMomentsInMeasures",
        fTraceMomentsInMeasures,
        fTracePasses);
  subGroup->
    appendAtomToSubGroup (
      traceMomentsInMeasuresBooleanAtom);
  segmentsAndMeasuresMultiplexBooleansAtom->
    addBooleanAtom (
      traceMomentsInMeasuresBooleanAtom);

  // fTraceDetailedMeasureNumbersSet is empty
}

void traceOahGroup::initializeScoreToVoicesTraceOah ()
{
  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Score to voices",
        "htstv", "help-trace-score-to-voices",
R"()",
      kElementVisibilityWhole,
      this);

  appendSubGroupToGroup (subGroup);

  // the 'score to voices' multiplex booleans atom

  S_oahMultiplexBooleansAtom
    scoreToVoicesMultiplexBooleansAtom =
      oahMultiplexBooleansAtom::create (
        "Trace SHORT_NAME/LONG_NAME in score to voices.",
        "SHORT_NAME",
        "LONG_NAME",
        fShortTracePrefix,
        fLongTracePrefix);

  subGroup->
    appendAtomToSubGroup (
      scoreToVoicesMultiplexBooleansAtom);

  // score

  fTraceScore = false;

  S_oahTwoBooleansAtom
    traceScoreBooleanAtom =
      oahTwoBooleansAtom::create (
        "tscore", "trace-score",
R"(Score)",
        "traceScore",
        fTraceScore,
        fTracePasses);
  subGroup->
    appendAtomToSubGroup (
      traceScoreBooleanAtom);
  scoreToVoicesMultiplexBooleansAtom->
    addBooleanAtom (
      traceScoreBooleanAtom);

  // part groups

  fTracePartGroups = false;

  S_oahTwoBooleansAtom
    tracePartGroupsBooleanAtom =
      oahTwoBooleansAtom::create (
        "tpgroups", "trace-part-groups",
R"(Part groups)",
        "tracePartGroups",
        fTracePartGroups,
        fTracePasses);
  subGroup->
    appendAtomToSubGroup (
      tracePartGroupsBooleanAtom);
  scoreToVoicesMultiplexBooleansAtom->
    addBooleanAtom (
      tracePartGroupsBooleanAtom);

  // part groups details

  fTracePartGroupsDetails = false;

  S_oahThreeBooleansAtom
    tracePartGroupsDetailsBooleanAtom =
      oahThreeBooleansAtom::create (
        "tpgroupsd", "trace-part-groups-details",
R"(Part groups with more details.
This option implies '-tpgrps, -trace-part-groups'.)",
        "tracePartGroupsDetails",
        fTracePartGroupsDetails,
        fTracePartGroups,
        fTracePasses);
  subGroup->
    appendAtomToSubGroup (
      tracePartGroupsDetailsBooleanAtom);
  scoreToVoicesMultiplexBooleansAtom->
    addBooleanAtom (
      tracePartGroupsDetailsBooleanAtom);

  // parts

  fTraceParts = false;

  S_oahTwoBooleansAtom
    tracePartsBooleanAtom =
      oahTwoBooleansAtom::create (
        "tparts", "trace-parts",
R"(Parts)",
        "traceParts",
        fTraceParts,
        fTracePasses);
  subGroup->
    appendAtomToSubGroup (
      tracePartsBooleanAtom);
  scoreToVoicesMultiplexBooleansAtom->
    addBooleanAtom (
      tracePartsBooleanAtom);

  // staves

  fTraceStaves = false;

  S_oahTwoBooleansAtom
    traceStavesBooleanAtom =
      oahTwoBooleansAtom::create (
        "tstaves", "trace-staves",
R"(Staves)",
        "traceStaves",
        fTraceStaves,
        fTracePasses);
  subGroup->
    appendAtomToSubGroup (
      traceStavesBooleanAtom);
  scoreToVoicesMultiplexBooleansAtom->
    addBooleanAtom (
      traceStavesBooleanAtom);

  // staff details

  fTraceStaffDetails = false;

  S_oahTwoBooleansAtom
    traceStaffDetailsBooleanAtom =
      oahTwoBooleansAtom::create (
        "tst", "trace-staff-details",
R"(Staff details)",
        "traceStaffDetails",
        fTraceStaffDetails,
        fTracePasses);
  subGroup->
    appendAtomToSubGroup (
      traceStaffDetailsBooleanAtom);
  scoreToVoicesMultiplexBooleansAtom->
    addBooleanAtom (
      traceStaffDetailsBooleanAtom);

  // staff changes

  fTraceStaffChanges = false;

  S_oahTwoBooleansAtom
    traceStaffChangesBooleanAtom =
      oahTwoBooleansAtom::create (
        "tschanges", "trace-staff-changes",
R"(Staff changes)",
        "traceStaffChanges",
        fTraceStaffChanges,
        fTracePasses);
  subGroup->
    appendAtomToSubGroup (
      traceStaffChangesBooleanAtom);
  scoreToVoicesMultiplexBooleansAtom->
    addBooleanAtom (
      traceStaffChangesBooleanAtom);

  // voices

  fTraceVoices = false;

  S_oahTwoBooleansAtom
    traceVoicesBooleanAtom =
      oahTwoBooleansAtom::create (
        "tvoices", "trace-voices",
R"(Voices)",
        "traceVoices",
        fTraceVoices,
        fTracePasses);
  subGroup->
    appendAtomToSubGroup (
      traceVoicesBooleanAtom);
  scoreToVoicesMultiplexBooleansAtom->
    addBooleanAtom (
      traceVoicesBooleanAtom);

  // voices details

  fTraceVoicesDetails = false;

  S_oahThreeBooleansAtom
    traceVoicesDetailsBooleanAtom =
      oahThreeBooleansAtom::create (
        "tvoicesd", "trace-voices-details",
R"(Voices with more details (quite verbose)..
This option implies '-tvoices, -trace-voices'.)",
        "traceVoicesDetails",
        fTraceVoicesDetails,
        fTraceVoices,
        fTracePasses);
  subGroup->
    appendAtomToSubGroup (
      traceVoicesDetailsBooleanAtom);
  scoreToVoicesMultiplexBooleansAtom->
    addBooleanAtom (
      traceVoicesDetailsBooleanAtom);
}

void traceOahGroup::initializeNotesTraceOah ()
{
  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Notes",
        "htn", "help-trace-notes",
R"()",
      kElementVisibilityWhole,
      this);

  appendSubGroupToGroup (subGroup);

  // the 'notes' multiplex booleans atom

  S_oahMultiplexBooleansAtom
    notesMultiplexBooleansAtom =
      oahMultiplexBooleansAtom::create (
        "Trace SHORT_NAME/LONG_NAME in notes.",
        "SHORT_NAME",
        "LONG_NAME",
        fShortTracePrefix,
        fLongTracePrefix);

  subGroup->
    appendAtomToSubGroup (
      notesMultiplexBooleansAtom);

  // notes

  fTraceNotes = false;

  S_oahTwoBooleansAtom
    traceNotesBooleanAtom =
      oahTwoBooleansAtom::create (
        "tnotes", "trace-notes",
R"(Notes)",
        "traceNotes",
        fTraceNotes,
        fTracePasses);
  subGroup->
    appendAtomToSubGroup (
      traceNotesBooleanAtom);
  notesMultiplexBooleansAtom->
    addBooleanAtom (
      traceNotesBooleanAtom);

  // notes details

  fTraceNotesDetails = false;

  S_oahThreeBooleansAtom
    traceNotesDetailsBooleanAtom =
      oahThreeBooleansAtom::create (
        "tnotesd", "trace-notes-details",
R"(Notes with more details, including divisions handling (quite verbose)...
This option implies '-tnotes, -trace-notes'.)",
        "traceNotesDetails",
        fTraceNotesDetails,
        fTraceNotes,
        fTracePasses);
  subGroup->
    appendAtomToSubGroup (
      traceNotesDetailsBooleanAtom);
  notesMultiplexBooleansAtom->
    addBooleanAtom (
      traceNotesDetailsBooleanAtom);

  // whole notes

  fTraceWholeNotes = false;

  S_oahTwoBooleansAtom
    traceWholeNotesBooleanAtom =
      oahTwoBooleansAtom::create (
        "twn", "trace-whole-notes",
R"(Whole notes computations (quite verbose)...)",
        "traceWholeNotes",
        fTraceWholeNotes,
        fTracePasses);
  subGroup->
    appendAtomToSubGroup (
      traceWholeNotesBooleanAtom);
  notesMultiplexBooleansAtom->
    addBooleanAtom (
      traceWholeNotesBooleanAtom);

  // whole notes details

  fTraceWholeNotesDetails = false;

  S_oahThreeBooleansAtom
    traceWholeNotesDetailsBooleanAtom =
      oahThreeBooleansAtom::create (
        "twnd", "trace-whole-notes-details",
R"(Whole notes computations details (event more verbose)...)",
        "traceWholeNotesDetails",
        fTraceWholeNotesDetails,
        fTraceWholeNotes,
        fTracePasses);
  subGroup->
    appendAtomToSubGroup (
      traceWholeNotesDetailsBooleanAtom);
  notesMultiplexBooleansAtom->
    addBooleanAtom (
      traceWholeNotesDetailsBooleanAtom);

  // rest notes

  fTraceRestNotes = false;

  S_oahTwoBooleansAtom
    traceRestNotesBooleanAtom =
      oahTwoBooleansAtom::create (
        "trn", "trace-rest-notes",
R"(Rest notes)",
        "traceRestNotes",
        fTraceRestNotes,
        fTracePasses);
  subGroup->
    appendAtomToSubGroup (
      traceRestNotesBooleanAtom);
  notesMultiplexBooleansAtom->
    addBooleanAtom (
      traceRestNotesBooleanAtom);

  // skip notes

  fTraceSkipNotes = false;

  S_oahTwoBooleansAtom
    traceSkipNotesBooleanAtom =
      oahTwoBooleansAtom::create (
        "tsn", "trace-skip-notes",
R"(Skip notes)",
        "traceSkipNotes",
        fTraceSkipNotes,
        fTracePasses);
  subGroup->
    appendAtomToSubGroup (
      traceSkipNotesBooleanAtom);
  notesMultiplexBooleansAtom->
    addBooleanAtom (
      traceSkipNotesBooleanAtom);

  // notes octave entry

  fTraceNotesOctaveEntry = false;

  S_oahTwoBooleansAtom
    traceNotesOctaveEntryBooleanAtom =
      oahTwoBooleansAtom::create (
        "tnoe", "trace-notes-octave-entry",
R"(Notes octave entry)",
        "traceNotesOctaveEntry",
        fTraceNotesOctaveEntry,
        fTracePasses);
  subGroup->
    appendAtomToSubGroup (
      traceNotesOctaveEntryBooleanAtom);
  notesMultiplexBooleansAtom->
    addBooleanAtom (
      traceNotesOctaveEntryBooleanAtom);

  // grace notes

  fTraceGraceNotes = false;

  S_oahTwoBooleansAtom
    traceGraceNotesBooleanAtom =
      oahTwoBooleansAtom::create (
        "tgraces", "trace-gracenotes",
R"(Grace notes)",
        "traceGraceNotes",
        fTraceGraceNotes,
        fTracePasses);
  subGroup->
    appendAtomToSubGroup (
      traceGraceNotesBooleanAtom);
  notesMultiplexBooleansAtom->
    addBooleanAtom (
      traceGraceNotesBooleanAtom);

  // tremolos

  fTraceTremolos = false;

  S_oahTwoBooleansAtom
    traceTremolosBooleanAtom =
      oahTwoBooleansAtom::create (
        "ttrems", "trace-tremolos",
R"(Tremolos)",
        "traceTremolos",
        fTraceTremolos,
        fTracePasses);
  subGroup->
    appendAtomToSubGroup (
      traceTremolosBooleanAtom);
  notesMultiplexBooleansAtom->
    addBooleanAtom (
      traceTremolosBooleanAtom);
}

void traceOahGroup::initializeOptionsTraceAndDisplayOptions ()
{
  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Options handling trace",
        "htoh", "help-trace-options-handling",
R"()",
    kElementVisibilityWhole,
    this);

  appendSubGroupToGroup (subGroup);

  // trace options

  fTraceOah = false;

  fTraceOahAtom =
    oahBooleanAtom::create (
      "toah", "trace-oah",
R"(Write a trace of options and help handling to standard error.
This option should best appear early.)",
      "traceOah",
      fTraceOah);

  subGroup->
    appendAtomToSubGroup (
      fTraceOahAtom);

 // options details

  fTraceOahDetails = false;

  subGroup->
    appendAtomToSubGroup (
      oahTwoBooleansAtom::create (
        "toahd", "trace-oah-details",
R"(Write a trace of options and help handling with more details to standard error.
This option should best appear early.)",
        "traceOahDetails",
        fTraceOahDetails,
        fTraceOah));

  // fTraceDetailedMeasureNumbersSet is intially empty
}

void traceOahGroup::initializeRepeatsToSlashesTraceOah ()
{
  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Repeats to slashes",
        "htrts", "help-trace-repeats-to-slashes",
R"()",
      kElementVisibilityWhole,
      this);

  appendSubGroupToGroup (subGroup);

  // the 'repeats to slashes' multiplex booleans atom

  S_oahMultiplexBooleansAtom
    repeatsToSlashesMultiplexBooleansAtom =
      oahMultiplexBooleansAtom::create (
        "Trace SHORT_NAME/LONG_NAME in repeats to slashes.",
        "SHORT_NAME",
        "LONG_NAME",
        fShortTracePrefix,
        fLongTracePrefix);

  subGroup->
    appendAtomToSubGroup (
      repeatsToSlashesMultiplexBooleansAtom);

  // repeats

  fTraceRepeats = false;

  S_oahTwoBooleansAtom
    traceRepeatsBooleanAtom =
      oahTwoBooleansAtom::create (
        "treps", "trace-repeats",
R"(Repeats)",
        "traceRepeats",
        fTraceRepeats,
        fTracePasses);
  subGroup->
    appendAtomToSubGroup (
      traceRepeatsBooleanAtom);
  repeatsToSlashesMultiplexBooleansAtom->
    addBooleanAtom (
      traceRepeatsBooleanAtom);

  // repeats details

  fTraceRepeatsDetails = false;

  S_oahThreeBooleansAtom
    traceRepeatsDetailsBooleanAtom =
      oahThreeBooleansAtom::create (
        "trepsd", "trace-repeats-details",
R"(Repeats details)",
        "traceRepeats",
        fTraceRepeatsDetails,
        fTraceRepeats,
        fTracePasses);
  subGroup->
    appendAtomToSubGroup (
      traceRepeatsDetailsBooleanAtom);
  repeatsToSlashesMultiplexBooleansAtom->
    addBooleanAtom (
      traceRepeatsDetailsBooleanAtom);

  // measures repeats

  fTraceMeasuresRepeats = false;

  S_oahTwoBooleansAtom
    traceMeasuresRepeatsBooleanAtom =
      oahTwoBooleansAtom::create (
        "tmreps", "trace-measures-repeats",
R"(Measures repeats)",
        "traceMeasureRepeats",
        fTraceMeasuresRepeats,
        fTracePasses);
  subGroup->
    appendAtomToSubGroup (
      traceMeasuresRepeatsBooleanAtom);
  repeatsToSlashesMultiplexBooleansAtom->
    addBooleanAtom (
      traceMeasuresRepeatsBooleanAtom);

  // rest measures

  fTraceRestMeasures = false;

  S_oahTwoBooleansAtom
    traceRestMeasuresBooleanAtom =
      oahTwoBooleansAtom::create (
        "trmeas", "trace-rest-measures",
R"(Multiple rests)",
        "traceRestMeasures",
        fTraceRestMeasures,
        fTracePasses);
  subGroup->
    appendAtomToSubGroup (
      traceRestMeasuresBooleanAtom);
  repeatsToSlashesMultiplexBooleansAtom->
    addBooleanAtom (
      traceRestMeasuresBooleanAtom);

  // beats repeats

  fTraceBeatsRepeats = false;

  S_oahTwoBooleansAtom
    traceBeatsRepeatsBooleanAtom =
      oahTwoBooleansAtom::create (
        "tbreps", "trace-beats-repeats",
R"(Beats repeatss)",
        "traceBeatsRepeats",
        fTraceBeatsRepeats,
        fTracePasses);
  subGroup->
    appendAtomToSubGroup (
      traceBeatsRepeatsBooleanAtom);
  repeatsToSlashesMultiplexBooleansAtom->
    addBooleanAtom (
      traceBeatsRepeatsBooleanAtom);

  // slashes

  fTraceSlashes = false;

  S_oahTwoBooleansAtom
    traceSlashesBooleanAtom =
      oahTwoBooleansAtom::create (
        "tslashes", "trace-slashes",
R"(Slashes)",
        "traceSlashes",
        fTraceSlashes,
        fTracePasses);
  subGroup->
    appendAtomToSubGroup (
      traceSlashesBooleanAtom);
  repeatsToSlashesMultiplexBooleansAtom->
    addBooleanAtom (
      traceSlashesBooleanAtom);
}

void traceOahGroup::initializeOtherTraceOah ()
{
  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Other",
        "hto", "help-trace-other",
R"()",
      kElementVisibilityWhole,
      this);

  appendSubGroupToGroup (subGroup);

  // the 'other' multiplex booleans atom

  S_oahMultiplexBooleansAtom
    otherMultiplexBooleansAtom =
      oahMultiplexBooleansAtom::create (
        "Trace SHORT_NAME/LONG_NAME in other.",
        "SHORT_NAME",
        "LONG_NAME",
        fShortTracePrefix,
        fLongTracePrefix);

  subGroup->
    appendAtomToSubGroup (
      otherMultiplexBooleansAtom);

  // passes

  fTracePasses = false;

  S_oahBooleanAtom
    tracePassesBooleanAtom =
      oahBooleanAtom::create (
        "tpasses", "trace-passes",
R"(Write a trace of the passes to standard error.)",
        "tracePasses",
        fTracePasses);
  subGroup->
    appendAtomToSubGroup (
      tracePassesBooleanAtom);
  otherMultiplexBooleansAtom->
    addBooleanAtom (
      tracePassesBooleanAtom);

  // scaling

  fTraceGeometry = false;

  S_oahTwoBooleansAtom
    traceGeometryBooleanAtom =
      oahTwoBooleansAtom::create (
        "tgeom", "trace-scaling",
R"(Scaling)",
        "traceGeometry",
        fTraceGeometry,
        fTracePasses);
  subGroup->
    appendAtomToSubGroup (
      traceGeometryBooleanAtom);
  otherMultiplexBooleansAtom->
    addBooleanAtom (
      traceGeometryBooleanAtom);

  // varValAssocs

  fTraceVarValAssocs = false;

  S_oahTwoBooleansAtom
    traceVarValAssocsBooleanAtom =
      oahTwoBooleansAtom::create (
        "tvva", "trace-var-val-assocs",
R"(VarValAssocs)",
        "traceVarValAssocs",
        fTraceVarValAssocs,
        fTracePasses);
  subGroup->
    appendAtomToSubGroup (
      traceVarValAssocsBooleanAtom);
  otherMultiplexBooleansAtom->
    addBooleanAtom (
      traceVarValAssocsBooleanAtom);

  // '-tft, -trace-for-tests' is hidden...

  fTraceForTests = false;

  S_oahBooleanAtom
    traceForTestsOahBooleanAtom =
      oahBooleanAtom::create (
        "tft", "trace-for-tests",
R"(Write a trace for tests to standard error.)",
        "traceForTests",
        fTraceForTests);
  traceForTestsOahBooleanAtom->
    setElementVisibilityKind (
      kElementVisibilityHidden);

  subGroup->
    appendAtomToSubGroup (
      traceForTestsOahBooleanAtom);
}

void traceOahGroup::initializeTraceOahGroup ()
{
  // options and help trace and display
  initializeOptionsTraceAndDisplayOptions ();

  // score to voices
  initializeScoreToVoicesTraceOah ();

  // print layouts
  initializePrintLayoutsTraceOah ();

  // segments and measures
  initializeSegmentsAndMeasuresTraceOah ();

  // notes
  initializeNotesTraceOah ();

  // notes attachments
  initializeNotesAttachmentsTraceOah ();

  // repeats to slashes
  initializeRepeatsToSlashesTraceOah ();

  // instruments
  initializeInstrumentsTraceOah ();

  // chords and tuplets
  initializeChordsAndTupletsTraceOah ();

  // texts
  initializeCreditsToWordsTraceOah ();

  // harmonies
  initializeHarmoniesTraceOah ();

  // figured basses
  initializeFiguredBassesTraceOah ();

  // spanners
  initializeSpannersTraceOah ();

  // inter-notes
  initializeInterNotesTraceOah ();

  // clefs to tempos
  initializeClefsToTemposTraceOah ();

  // breaks
  initializeBreaksAndBarlinesTraceOah ();

  // above staff
  initializeAboveStaffTraceOah ();

  //transpositions
  initializeTranspositionsTraceOah ();

  // other trace
  initializeOtherTraceOah ();
}

//______________________________________________________________________________
void traceOahGroup::enforceGroupQuietness ()
{}

//______________________________________________________________________________
void traceOahGroup::checkGroupOptionsConsistency ()
{
  // JMI
}

//______________________________________________________________________________
void traceOahGroup::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> traceOahGroup::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_traceOahGroup>*
    p =
      dynamic_cast<visitor<S_traceOahGroup>*> (v)) {
        S_traceOahGroup elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching traceOahGroup::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void traceOahGroup::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> traceOahGroup::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_traceOahGroup>*
    p =
      dynamic_cast<visitor<S_traceOahGroup>*> (v)) {
        S_traceOahGroup elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching traceOahGroup::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void traceOahGroup::browseData (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> traceOahGroup::browseData ()" <<
      endl;
  }
#endif

  oahGroup::browseData (v);
}

//______________________________________________________________________________
void traceOahGroup::printAtomWithValueOptionsValues (
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
    // options and help display
    setw (valueFieldWidth) << "traceOah" << " : " <<
    booleanAsString (fTraceOah) <<
    endl <<
    setw (valueFieldWidth) << "traceOahDetails" << " : " <<
    booleanAsString (fTraceOahDetails) <<
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

    // scaling
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
    setw (valueFieldWidth) << "traceMeasuresNumbers" << " : " <<
    booleanAsString (fTraceMeasuresNumbers) <<
    endl <<
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
    booleanAsString (fTraceBarlines) <<
    endl <<
    setw (valueFieldWidth) << "traceBarlinesDetails" << " : " <<
    booleanAsString (fTraceBarlinesDetails) <<
    endl <<

    // bar checks
    setw (valueFieldWidth) << "traceBarChecks" << " : " <<
    booleanAsString (fTraceBarChecks) <<
    endl <<

    // bar number checks
    setw (valueFieldWidth) << "traceBarNumberChecks" << " : " <<
    booleanAsString (fTraceBarNumberChecks) <<
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
    setw (valueFieldWidth) << "traceWholeNotesDetails" << " : " <<
    booleanAsString (fTraceWholeNotesDetails) <<
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
    setw (valueFieldWidth) << "traceExtraHarmonies" << " : " <<
    booleanAsString (fTraceExtraHarmonies) <<
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

    // dal segnos
    setw (valueFieldWidth) << "traceDalSegnos" << " : " <<
    booleanAsString (fTraceDalSegnos) <<
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

    // extra harmonies
    setw (valueFieldWidth) << "traceExtraHarmonies" << " : " <<
    booleanAsString (fTraceExtraHarmonies) <<
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

void traceOahGroup::printTraceOahValues (int fieldWidth)
{
  gLogStream <<
    "The trace options are:" <<
    endl;

  gIndenter++;

  gLogStream << left <<
    setw (fieldWidth) << "Trace:" <<
    endl;

  gIndenter++;

  gLogStream << left <<
    // options and help display
    setw (fieldWidth) << "traceOah" << " : " <<
    booleanAsString (fTraceOah) <<
    endl <<
    setw (fieldWidth) << "traceOahDetails" << " : " <<
    booleanAsString (fTraceOahDetails) <<
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

    // scaling
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
    setw (fieldWidth) << "traceMeasuresNumbers" << " : " <<
    booleanAsString (fTraceMeasuresNumbers) <<
    endl <<
    setw (fieldWidth) << "traceMeasures" << " : " <<
    booleanAsString (fTraceMeasures) <<
    endl <<
    setw (fieldWidth) << "traceMeasuresDetails" << " : " <<
    booleanAsString (fTraceMeasuresDetails) <<
    endl <<
    setw (fieldWidth) << "tracePositionsInMeasures" << " : " <<
    booleanAsString (fTracePositionsInMeasures) <<
    endl;

  gLogStream << left <<
    setw (fieldWidth) << "traceDetailedMeasureNumbersSet" << " : " <<
    endl;

  if (fTraceDetailedMeasureNumbersSet.size ()) {
    set<int>::const_iterator
      iBegin = fTraceDetailedMeasureNumbersSet.begin (),
      iEnd   = fTraceDetailedMeasureNumbersSet.end (),
      i      = iBegin;

    gIndenter++;

    for ( ; ; ) {
      gLogStream << "v " << (*i);
      if (++i == iEnd) break;
      // no endl here
    } // for

    gIndenter--;
  }
  else {
    gLogStream <<
      "none";
  }
  gLogStream << endl;

    // segments
  gLogStream << left <<
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
    booleanAsString (fTraceBarlines) <<
    endl <<
    setw (fieldWidth) << "traceBarlinesDetails" << " : " <<
    booleanAsString (fTraceBarlinesDetails) <<
    endl <<

    // bar checks
    setw (fieldWidth) << "traceBarChecks" << " : " <<
    booleanAsString (fTraceBarChecks) <<
    endl <<

    // bar number checks
    setw (fieldWidth) << "traceBarNumberChecks" << " : " <<
    booleanAsString (fTraceBarNumberChecks) <<
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
    setw (fieldWidth) << "traceWholeNotesDetails" << " : " <<
    booleanAsString (fTraceWholeNotesDetails) <<
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
    setw (fieldWidth) << "traceExtraHarmonies" << " : " <<
    booleanAsString (fTraceExtraHarmonies) <<
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

    // dal segnos
    setw (fieldWidth) << "ftraceDalSegnos" << " : " <<
    booleanAsString (fTraceDalSegnos) <<
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

    // extra harmonies
    setw (fieldWidth) << "traceExtraHarmonies" << " : " <<
    booleanAsString (fTraceExtraHarmonies) <<
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

ostream& operator<< (ostream& os, const S_traceOahGroup& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_traceOahGroup createGlobalTraceOahGroup (
  S_oahPrefix shortTracePrefix,
  S_oahPrefix longTracePrefix)
{
#ifdef TRACING_IS_ENABLED
#ifdef ENFORCE_TRACE_OAH
  gLogStream <<
    "Creating global trace OAH group" <<
    endl;
#endif
#endif

  // protect library against multiple initializations
  if (! gGlobalTraceOahGroup) {
    // create the global OAH group
    gGlobalTraceOahGroup =
      traceOahGroup::create (
        shortTracePrefix,
        longTracePrefix);
    assert (gGlobalTraceOahGroup != 0);
  }

  // return the global OAH group
  return gGlobalTraceOahGroup;
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
      kElementVisibilityWhole,
      this);

    appendSubGroupToGroup (traceAndDisplaySubGroup);

  // detailed measure numbers set

  // fTraceDetailedMeasureNumbersSet is empty

    traceAndDisplaySubGroup->
      appendAtomToSubGroup (
        oahNaturalNumbersSetAtom::create (
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

