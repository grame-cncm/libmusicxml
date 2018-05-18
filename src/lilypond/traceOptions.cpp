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
    "ht", "helpTrace",
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
    
    fTraceBasic = boolOptionsInitialValue;
    
    fTraceOptions         = boolOptionsInitialValue;
    fDisplayOptionsValues = boolOptionsInitialValue;
    
    // fTraceDetailedMeasureNumbersSet is empty
    
    // options
    
      S_optionsSubGroup
        traceAndDisplaySubGroup =
          optionsSubGroup::create (
            "Trace and display",
            "htd", "helpTraceDansDisplay",
R"()",
        optionsSubGroup::kAlwaysShowDescription,
        this);
    
      appendOptionsSubGroup (traceAndDisplaySubGroup);
          
      traceAndDisplaySubGroup->
        appendOptionsItem (
          optionsBooleanItem::create (
            "t", "traceTrace",
R"(Write a trace of the trace activity to standard error.)",
            "traceTrace",
            fTraceBasic));
    
      traceAndDisplaySubGroup->
        appendOptionsItem (
          optionsBooleanItem::create (
            "dov", "displayOptionsValues",
R"(Write the chosen options values to standard error.)",
            "displayOptionsValues",
            fDisplayOptionsValues));
    
      traceAndDisplaySubGroup->
        appendOptionsItem (
          optionsBooleanItem::create (
            "topts", "traceOptions",
R"(Write a trace of options handling to standard error.
This option should best appear first.)",
            "traceOptions",
            fTraceOptions));
    
      traceAndDisplaySubGroup->
        appendOptionsItem (
          optionsNumbersSetItem::create (
            "tdetail", "traceDetailed",
R"('measureNumbersSet' has a form such as '0,2-14,^8-10',
where '^' excludes the corresponding numbers interval
and 0 applies to the '<part-list>' and anacrusis if present.
The measure numbers should be those of the produced score, 
since MusicXML measure numbers are arbitrary strings.
Generate a detailed trace of the activity and print additional
debugging information to standard error for the specified measures.)",
            "measureNumbersSet",
            "traceDetailed",
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
  
    // transpositions
    fTraceTranspositions = boolOptionsInitialValue;
  
    // octave shifts
    fTraceOctaveShifts = boolOptionsInitialValue;

    // segments
    fTraceSegments = boolOptionsInitialValue;
    
    // barlines
    fTraceBarlines = boolOptionsInitialValue;

    // repeats
    fTraceRepeats = boolOptionsInitialValue;
  
    // measures
    fTraceMeasures = boolOptionsInitialValue;
  
    // notes
    fTraceNotes = boolOptionsInitialValue;
    fTraceNotesDetails = boolOptionsInitialValue;
    
    // beams
    fTraceBeams = boolOptionsInitialValue;
    
    // technicals
    fTraceTechnicals = boolOptionsInitialValue;
    
    // ornaments
    fTraceOrnaments = boolOptionsInitialValue;

    // spanners
    fTraceSpanners = boolOptionsInitialValue;

    // words
    fTraceWords = boolOptionsInitialValue;
    
    // tremolos
    fTraceTremolos = boolOptionsInitialValue;
    
    // chords
    fTraceChords = boolOptionsInitialValue;
    
    // tuplets
    fTraceTuplets = boolOptionsInitialValue;
    
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

    // slurs
    fTraceSlurs = boolOptionsInitialValue;

    // ligatures
    fTraceLigatures = boolOptionsInitialValue;

    // wedges
    fTraceWedges = boolOptionsInitialValue;

    // staff tuning
    fTraceStaffTuning = boolOptionsInitialValue;
    
    // harp pedals tuning
    fTraceHarpPedalsTuning = boolOptionsInitialValue;
    
    // options
  
    S_optionsSubGroup
      specificTraceSubGroup =
        optionsSubGroup::create (
          "Specific trace",
          "hst", "helpSpecificTrace",
R"(Note: the options in this group imply '-t, -traceTrace'.)",
        optionsSubGroup::kHideDescriptionByDefault,
        this);
  
    appendOptionsSubGroup (specificTraceSubGroup);

    specificTraceSubGroup->
      appendOptionsItem (
        optionsTwoBooleansItem::create (
          "tenc", "traceEncoding",
R"(Encoding)",
          "traceEncoding",
          fTraceEncoding,
          fTraceBasic));
      
    specificTraceSubGroup->
      appendOptionsItem (
        optionsTwoBooleansItem::create (
          "tdivs", "traceDivisions",
R"(Divisions)",
          "traceDivisions",
          fTraceDivisions,
          fTraceBasic));
      
    specificTraceSubGroup->
      appendOptionsItem (
        optionsTwoBooleansItem::create (
          "tpgrps", "tracePartGroups",
R"(Part groups)",
          "tracePartGroups",
          fTracePartGroups,
          fTraceBasic));
      
    specificTraceSubGroup->
      appendOptionsItem (
        optionsTwoBooleansItem::create (
          "tgeom", "traceGeometry",
R"(Geometry)",
          "traceGeometry",
          fTraceGeometry,
          fTraceBasic));
      
    specificTraceSubGroup->
      appendOptionsItem (
        optionsThreeBooleansItem::create (
          "tpgrpsd", "tracePartGroupsDetails",
R"(Part groups with more details.
This option implies '-tpgrps, -tracePartGroups'.)",
          "tracePartGroupsDetails",
          fTracePartGroupsDetails,
          fTracePartGroups,
          fTraceBasic));
      
    specificTraceSubGroup->
      appendOptionsItem (
        optionsTwoBooleansItem::create (
          "tparts", "traceParts",
R"(Parts)",
          "traceParts",
          fTraceParts,
          fTraceBasic));
      
    specificTraceSubGroup->
      appendOptionsItem (
        optionsTwoBooleansItem::create (
          "tstaves", "traceStaves",
R"(Staves)",
          "traceStaves",
          fTraceStaves,
          fTraceBasic));
      
    specificTraceSubGroup->
      appendOptionsItem (
        optionsTwoBooleansItem::create (
          "tvoices", "traceVoices",
R"(Voices)",
          "traceVoices",
          fTraceVoices,
          fTraceBasic));
      
    specificTraceSubGroup->
      appendOptionsItem (
        optionsThreeBooleansItem::create (
          "tvoicesd", "traceVoicesDetails",
R"(Voices with more details (quite verbose)..
This option implies '-tvdetails, -traceVoicesDetails'.)",
          "traceVoicesDetails",
          fTraceVoicesDetails,
          fTraceVoices,
          fTraceBasic));
      
    specificTraceSubGroup->
      appendOptionsItem (
        optionsTwoBooleansItem::create (
          "tclefs", "traceClefs",
R"(Clefs)",
          "traceClefs",
          fTraceClefs,
          fTraceBasic));
      
    specificTraceSubGroup->
      appendOptionsItem (
        optionsTwoBooleansItem::create (
          "tkeys", "traceKeys",
R"(Keys)",
          "traceKeys",
          fTraceKeys,
          fTraceBasic));
      
    specificTraceSubGroup->
      appendOptionsItem (
        optionsTwoBooleansItem::create (
          "ttimes", "traceTimes",
R"(Times)",
          "traceTimes",
          fTraceTimes,
          fTraceBasic));
      
    specificTraceSubGroup->
      appendOptionsItem (
        optionsTwoBooleansItem::create (
          "ttempos", "traceTempos",
R"(Tempos)",
          "traceTempos",
          fTraceTempos,
          fTraceBasic));
      
    specificTraceSubGroup->
      appendOptionsItem (
        optionsTwoBooleansItem::create (
          "ttransps", "traceTranspositions",
R"(Transpositions (<transpose/> in MusicXML, \transposition in LilyPond))",
          "traceTranspositions",
          fTraceTranspositions,
          fTraceBasic));
      
    specificTraceSubGroup->
      appendOptionsItem (
        optionsTwoBooleansItem::create (
          "toctshifts", "traceOctaveShifts",
R"(Octave shifts (<octave-shift/> in MusicXML, \ottava in LilyPond))",
          "traceOctaveShifts",
          fTraceOctaveShifts,
          fTraceBasic));
      
    specificTraceSubGroup->
      appendOptionsItem (
        optionsTwoBooleansItem::create (
          "tsegs", "traceSegments",
R"(Voices segments)",
          "traceSegments",
          fTraceSegments,
          fTraceBasic));
      
    specificTraceSubGroup->
      appendOptionsItem (
        optionsTwoBooleansItem::create (
          "tbars", "traceBarlines",
R"(Barlines)",
          "traceBarlines",
          fTraceBarlines,
          fTraceBasic));
      
    specificTraceSubGroup->
      appendOptionsItem (
        optionsTwoBooleansItem::create (
          "treps", "traceRepeats",
R"(Repeats, including multiple mesure repeats)",
          "traceRepeats",
          fTraceRepeats,
          fTraceBasic));
      
    specificTraceSubGroup->
      appendOptionsItem (
        optionsTwoBooleansItem::create (
          "tmeas", "traceMeasures",
R"(Measures)",
          "traceMeasures",
          fTraceMeasures,
          fTraceBasic));
      
    specificTraceSubGroup->
      appendOptionsItem (
        optionsTwoBooleansItem::create (
          "tnotes", "traceNotes",
R"(Notes)",
          "traceNotes",
          fTraceNotes,
          fTraceBasic));
          
    specificTraceSubGroup->
      appendOptionsItem (
        optionsThreeBooleansItem::create (
          "tnotesd", "traceNotesDetails",
R"(Notes with more details, including divisions handling (quite verbose)..
This option implies '-tndetails, -traceNotesDetails'.)",
          "traceNotesDetails",
          fTraceNotesDetails,
          fTraceNotes,
          fTraceBasic));
      
    specificTraceSubGroup->
      appendOptionsItem (
        optionsTwoBooleansItem::create (
          "tbeams", "traceBeams",
R"(Beams)",
          "traceBeams",
          fTraceBeams,
          fTraceBasic));
      
    specificTraceSubGroup->
      appendOptionsItem (
        optionsTwoBooleansItem::create (
          "ttechs", "traceTechnicals",
R"(Technicals)",
          "traceTechnicals",
          fTraceTechnicals,
          fTraceBasic));
      
    specificTraceSubGroup->
      appendOptionsItem (
        optionsTwoBooleansItem::create (
          "torns", "traceOrnaments",
R"(Ornaments)",
          "traceOrnaments",
          fTraceOrnaments,
          fTraceBasic));
      
    specificTraceSubGroup->
      appendOptionsItem (
        optionsTwoBooleansItem::create (
          "tspans", "traceSpanners",
R"(Spanners)",
          "traceSpanners",
          fTraceSpanners,
          fTraceBasic));
      
    specificTraceSubGroup->
      appendOptionsItem (
        optionsTwoBooleansItem::create (
          "twords", "traceWords",
R"(Words)",
          "traceWords",
          fTraceWords,
          fTraceBasic));
      
    specificTraceSubGroup->
      appendOptionsItem (
        optionsTwoBooleansItem::create (
          "ttrems", "traceTremolos",
R"(Tremolos)",
          "traceTremolos",
          fTraceTremolos,
          fTraceBasic));
      
    specificTraceSubGroup->
      appendOptionsItem (
        optionsTwoBooleansItem::create (
          "tchords", "traceChords",
R"(Chords)",
          "traceChords",
          fTraceChords,
          fTraceBasic));
      
    specificTraceSubGroup->
      appendOptionsItem (
        optionsTwoBooleansItem::create (
          "ttups", "traceTuplets",
R"(Tuplets)",
          "traceTuplets",
          fTraceTuplets,
          fTraceBasic));
      
    specificTraceSubGroup->
      appendOptionsItem (
        optionsTwoBooleansItem::create (
          "tgliss", "traceGlissandos",
R"(Glissandos)",
          "traceGlissandos",
          fTraceGlissandos,
          fTraceBasic));
      
    specificTraceSubGroup->
      appendOptionsItem (
        optionsTwoBooleansItem::create (
          "tslides", "traceSlides",
R"(Slides)",
          "traceSlides",
          fTraceSlides,
          fTraceBasic));
      
    specificTraceSubGroup->
      appendOptionsItem (
        optionsTwoBooleansItem::create (
          "tgraces", "traceGraceNotes",
R"(Grace notes)",
          "traceGraceNotes",
          fTraceGraceNotes,
          fTraceBasic));
      
    specificTraceSubGroup->
      appendOptionsItem (
        optionsTwoBooleansItem::create (
          "tlyrics", "traceLyrics",
R"(Lyrics in MusicXML, stanzas in MSR)",
          "traceLyrics",
          fTraceLyrics,
          fTraceBasic));
      
    specificTraceSubGroup->
      appendOptionsItem (
        optionsTwoBooleansItem::create (
          "tlyricsd", "traceLyricsDetails",
R"(Lyrics in MusicXML, stanzas in MSR)",
          "traceLyricsDetails",
          fTraceLyricsDetails,
          fTraceBasic));
      
    specificTraceSubGroup->
      appendOptionsItem (
        optionsTwoBooleansItem::create (
          "tharms", "traceHarmonies",
R"(<harmony/> in MusicXML, \chordmode in LilyPond)",
          "traceHarmonies",
          fTraceHarmonies,
          fTraceBasic));
      
    specificTraceSubGroup->
      appendOptionsItem (
        optionsTwoBooleansItem::create (
          "tFrames", "traceFrames",
R"(<frame/> in MusicXML, \fret-diagram in LilyPond)",
          "traceFrames",
          fTraceFrames,
          fTraceBasic));
      
    specificTraceSubGroup->
      appendOptionsItem (
        optionsTwoBooleansItem::create (
          "tfigbass", "traceFiguredBass",
R"(<figured-bass> in MusicXML, \figuremode in LilyPond)",
          "traceFiguredBass",
          fTraceFiguredBass,
          fTraceBasic));
      
    specificTraceSubGroup->
      appendOptionsItem (
        optionsTwoBooleansItem::create (
          "tcredits", "traceCredits",
R"(Credits)",
          "traceCredits",
          fTraceCredits,
          fTraceBasic));
      
    specificTraceSubGroup->
      appendOptionsItem (
        optionsTwoBooleansItem::create (
          "tdynamics", "traceDynamics",
R"(Dynamics)",
          "traceDynamics",
          fTraceDynamics,
          fTraceBasic));
      
    specificTraceSubGroup->
      appendOptionsItem (
        optionsTwoBooleansItem::create (
          "tslurs", "traceSlurs",
R"(Slurs)",
          "fTraceSlurs",
          fTraceSlurs,
          fTraceBasic));
      
    specificTraceSubGroup->
      appendOptionsItem (
        optionsTwoBooleansItem::create (
          "tligatures", "traceLigatures",
R"(Ligatures)",
          "traceLigatures",
          fTraceLigatures,
          fTraceBasic));
      
    specificTraceSubGroup->
      appendOptionsItem (
        optionsTwoBooleansItem::create (
          "twedges", "traceWedges",
R"(Wedges)",
          "traceWedges",
          fTraceWedges,
          fTraceBasic));
      
    specificTraceSubGroup->
      appendOptionsItem (
        optionsTwoBooleansItem::create (
          "traceStaffTuning", "",
R"(Staff tuning)",
          "traceStaffTuning",
          fTraceStaffTuning,
          fTraceBasic));
      
    specificTraceSubGroup->
      appendOptionsItem (
        optionsTwoBooleansItem::create (
          "traceHarpPedalsTuning", "",
R"(Harp pedals tuning)",
          "traceHarpPedalsTuning",
          fTraceHarpPedalsTuning,
          fTraceBasic));
  }
      

  /* STUFF not yet handled JMI */

  fTraceScore = boolOptionsInitialValue;


  fTraceDynamics = boolOptionsInitialValue;
  fTraceWords = boolOptionsInitialValue;
  fTraceSlurs = boolOptionsInitialValue;
  fTraceLigatures = boolOptionsInitialValue;
  fTraceWedges = boolOptionsInitialValue;
  
  fTraceStaffTuning = boolOptionsInitialValue;
  
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

  clone->fTraceBasic = true;
    
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

  // transpositions
  clone->fTraceTranspositions = true;

  // octave shifts
  clone->fTraceOctaveShifts = true;

  // segments
  clone->fTraceSegments = true;
    
  // barlines
  clone->fTraceBarlines = true;

  // repeats
  clone->fTraceRepeats = true;

  // measures
  clone->fTraceMeasures = true;
  
  // notes
  clone->fTraceNotes = true;
  clone->fTraceNotesDetails = true;
    
  // beams
  clone->fTraceBeams = true;
    
  // technicals
  clone->fTraceTechnicals = true;
  
  // ornaments
  clone->fTraceOrnaments = true;
  
  // spanners
  clone->fTraceSpanners = true;
  
  // words
  clone->fTraceWords = true;
  
  // tremolos
  clone->fTraceTremolos = true;

  // chords
  clone->fTraceChords = true;
    
  // tuplets
  clone->fTraceTuplets = true;
  
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

    
  /* STUFF not yet handled JMI */

  clone->fTraceScore = true;


  clone->fTraceDynamics = true;

  clone->fTraceSlurs = true;

  clone->fTraceLigatures = true;

  clone->fTraceWedges = true;
  
  clone->fTraceStaffTuning = true;
  
  clone->fTraceMidi = true;

  return clone;
}

void traceOptions::printTraceOptionsValues (int fieldWidth)
{  
  gLogIOstream <<
    "The trace options are:" <<
    endl;

  gIndenter++;

/*
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
    setw (fieldWidth) << "traceTrace" << " : " <<
    booleanAsString (fTraceBasic) <<
     endl <<
    setw (fieldWidth) << "traceDetailedMeasureNumbersSet" << " : ";
        
  if (! fTraceDetailedMeasureNumbersSet.size ())
    gLogIOstream << "none";
    
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
    
    // barlines
    setw (fieldWidth) << "traceBarlines" << " : " <<
    booleanAsString (fTraceBarlines) <<
    endl <<
    
    // repeats
    setw (fieldWidth) << "traceRepeats" << " : " <<
    booleanAsString (fTraceRepeats) <<
    endl <<
    
    // measures
    setw (fieldWidth) << "traceMeasures" << " : " <<
    booleanAsString (fTraceMeasures) <<
    endl <<
    
    // notes
    setw (fieldWidth) << "traceNotes" << " : " <<
    booleanAsString (fTraceNotes) <<
    endl <<
    setw (fieldWidth) << "traceNotesDetails" << " : " <<
    booleanAsString (fTraceNotesDetails) <<
    endl <<
    
    // beams
    setw (fieldWidth) << "traceBeams" << " : " <<
    booleanAsString (fTraceBeams) <<
    endl <<
    
    // technicals
    setw (fieldWidth) << "traceTechnicals" << " : " <<
    booleanAsString (fTraceTechnicals) <<
    endl <<
    
    // ornaments
    setw (fieldWidth) << "traceOrnaments" << " : " <<
    booleanAsString (fTraceOrnaments) <<
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
    
    // tuplets
    setw (fieldWidth) << "traceTuplets" << " : " <<
    booleanAsString (fTraceTuplets) <<
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
