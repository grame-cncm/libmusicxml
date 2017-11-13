/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#include <iomanip>      // setw, set::precision, ...

#include "msr.h"

#include "generalOptions.h"


using namespace std;

namespace MusicXML2 
{

//_______________________________________________________________________________
S_generalOptions gGeneralOptions;
S_generalOptions gGeneralOptionsUserChoices;
S_generalOptions gGeneralOptionsWithDetailedTrace;

S_generalOptions generalOptions::create (
  S_msrOptionsHandler optionsHandler)
{
  generalOptions* o = new generalOptions (
    optionsHandler);
  assert(o!=0);

  return o;
}

generalOptions::generalOptions (
  S_msrOptionsHandler optionsHandler)
  : msrOptionsGroup (
    "General",
    "hg", "helpGeneral",
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
  initializeGeneralOptions (false);
}

generalOptions::~generalOptions ()
{}

void generalOptions::initializeGeneralOptions (
  bool boolOptionsInitialValue)
{
  // register translation date
  // ------------------------------------------------------

  {
    time_t      translationRawtime;
    struct tm*  translationTimeinfo;
    char buffer [80];
  
    time (&translationRawtime);
    translationTimeinfo = localtime (&translationRawtime);
  
    strftime (buffer, 80, "%A %F @ %T %Z", translationTimeinfo);
    fTranslationDate = buffer;
  }


  // version
  // --------------------------------------

  {
    // variables  
    
    // options
  
    S_msrOptionsSubGroup
      versionSubGroup =
        msrOptionsSubGroup::create (
          "Version",
          "hgb", "helpGeneralVersion",
R"()",
        msrOptionsSubGroup::kAlwaysShowDescription,
        this);
    
    appendOptionsSubGroup (versionSubGroup);
  
    versionSubGroup->
      appendOptionsItem (
        msrOptionsVersionItem::create (
          "v", "version",
R"(Display xml2ly's version number and history and exit.)"));
  }


  // about
  // --------------------------------------

  {
    // variables  
    
    // options
  
    S_msrOptionsSubGroup
      aboutSubGroup =
        msrOptionsSubGroup::create (
          "About",
          "hga", "helpGeneralAbout",
R"()",
        msrOptionsSubGroup::kAlwaysShowDescription,
        this);
    
    appendOptionsSubGroup (aboutSubGroup);
  
    aboutSubGroup->
      appendOptionsItem (
        msrOptionsAboutItem::create (
          "a", "about",
R"(Display information about xml2ly and exit.)"));
  }


  // contact
  // --------------------------------------

  {
    // variables  
    
    // options
  
    S_msrOptionsSubGroup
      contactSubGroup =
        msrOptionsSubGroup::create (
          "Contact",
          "hgc", "helpGeneralContact",
R"()",
        msrOptionsSubGroup::kAlwaysShowDescription,
        this);
    
    appendOptionsSubGroup (contactSubGroup);
  
    contactSubGroup->
      appendOptionsItem (
        msrOptionsContactItem::create (
          "c", "contact",
R"(Display information about how to contacct xml2ly maintainers and exit.)"));
  }


  // help
  // --------------------------------------

  {
    // variables  
    
    // options
  
    S_msrOptionsSubGroup
      helpGeneralOptionsHelpSubGroup =
        msrOptionsSubGroup::create (
          "Options help",
          "hgoh", "helpGeneralOptionsHelp",
R"()",
        msrOptionsSubGroup::kAlwaysShowDescription,
        this);
  
    appendOptionsSubGroup (helpGeneralOptionsHelpSubGroup);
  
    helpGeneralOptionsHelpSubGroup->
      appendOptionsItem (
        msrOptionsHelpUsageItem::create (
          "ho", "helpOptions",
R"(Print options usage help.)"));

    helpGeneralOptionsHelpSubGroup->
      appendOptionsItem (
        msrOptionsHelpSummaryItem::create (
          "hs", "helpSummary",
R"(Display xml2ly's help summary and exit.)"));
  
    helpGeneralOptionsHelpSubGroup->
      appendOptionsItem (
        msrOptionsItemHelpItem::create (
        "ih", "itemHelp",
R"(Print help about 'itemName'.)",
        "itemName"));
  }


  // output file
  // --------------------------------------

  {
    // variables  
  
    fAutoOutputFile = false;
    
    // options
  
    S_msrOptionsSubGroup
      outputFileSubGroup =
        msrOptionsSubGroup::create (
          "Output file",
          "hgof", "helpGeneralOutputFile",
R"()",
        msrOptionsSubGroup::kAlwaysShowDescription,
        this);
            
    appendOptionsSubGroup (outputFileSubGroup);
    
    outputFileSubGroup->
      appendOptionsItem (
        msrOptionsStringItem::create (
          "ofn", "outputFileName",
R"(Write LilyPond code to file 'fileName' instead of standard output.)",
          "fileName",
          "outputFileName",
          fOutputFileName));  
  
    outputFileSubGroup->
      appendOptionsItem (
        msrOptionsBooleanItem::create (
          "aofn", "autoOutputFileName",
R"(This option can only be used when reading from a file.
Write LilyPond code to a file in the current working directory.
The file name is derived from that of the input file,
replacing any suffix after the the '.' by 'ly'
or adding '.ly' if none is present.)",
          "autoOutputFileName",
          fAutoOutputFile));
  }


  // trace and display
  // --------------------------------------

  {
    // variables  
    
    fTraceGeneral = boolOptionsInitialValue;
    
    fDisplayOptionsValues = boolOptionsInitialValue;
    
    // fTraceDetailedMeasureNumbersSet is empty
    
    // options
    
      S_msrOptionsSubGroup
        traceAndDisplaySubGroup =
          msrOptionsSubGroup::create (
            "Trace and display",
            "hgtd", "helpGeneralTraceDansDisplay",
R"()",
        msrOptionsSubGroup::kAlwaysShowDescription,
        this);
    
      appendOptionsSubGroup (traceAndDisplaySubGroup);
          
      traceAndDisplaySubGroup->
        appendOptionsItem (
          msrOptionsBooleanItem::create (
            "t", "traceGeneral",
R"(Write a trace of the general activity to standard error.)",
            "traceGeneral",
            fTraceGeneral));
    
      traceAndDisplaySubGroup->
        appendOptionsItem (
          msrOptionsBooleanItem::create (
            "dov", "displayOptionsValues",
R"(Write the chosen options values to standard error.)",
            "displayOptionsValues",
            fDisplayOptionsValues));
    
      traceAndDisplaySubGroup->
        appendOptionsItem (
          msrOptionsNumbersSetItem::create (
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


  // CPU usage
  // --------------------------------------

  {
    // variables  
  
    fDisplayCPUusage = boolOptionsInitialValue;
      
    // options
  
    S_msrOptionsSubGroup
      CPUUsageSubGroup =
        msrOptionsSubGroup::create (
          "CPU usage",
          "hgcpu", "helpGeneralCPUUsage",
R"()",
        msrOptionsSubGroup::kAlwaysShowDescription,
        this);
  
    appendOptionsSubGroup (CPUUsageSubGroup);
        
    CPUUsageSubGroup->
      appendOptionsItem (
        msrOptionsBooleanItem::create (
          "cpu", "displayCPUusage",
R"(Write a trace of the general activity to standard error.)",
          "displayCPUusage",
          fDisplayCPUusage));
  }


  // specific trace    
  // --------------------------------------

  {
    // variables  
  
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
  
    // transpositions
    fTraceTranspositions = boolOptionsInitialValue;
  
    // segments
    fTraceSegments = boolOptionsInitialValue;
    
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
    
    // words
    fTraceWords = boolOptionsInitialValue;
    
    // tremolos
    fTraceTremolos = boolOptionsInitialValue;
    
    // chords
    fTraceChords = boolOptionsInitialValue;
    
    // tuplets
    fTraceTuplets = boolOptionsInitialValue;
    
    // grace notes
    fTraceGraceNotes = boolOptionsInitialValue;
  
    // lyrics
    fTraceLyrics = boolOptionsInitialValue;
  
    // harmonies
    fTraceHarmonies = boolOptionsInitialValue;
  
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
    
    // exit after some passes
    fExit2a = boolOptionsInitialValue;
    fExit2b = boolOptionsInitialValue;
    fExit3 = boolOptionsInitialValue;

    // options
  
    S_msrOptionsSubGroup
      specificTraceSubGroup =
        msrOptionsSubGroup::create (
          "Specific trace",
          "hgst", "helpGeneralSpecificTrace",
R"(Note: the options in this group imply '-t, -traceGeneral'.)",
        msrOptionsSubGroup::kHideDescriptionByDefault,
        this);
  
    appendOptionsSubGroup (specificTraceSubGroup);

    specificTraceSubGroup->
      appendOptionsItem (
        msrOptionsTwoBooleansItem::create (
          "tdivs", "traceDivisions",
R"(Divisions)",
          "traceDivisions",
          fTraceDivisions,
          fTraceGeneral));
      
    specificTraceSubGroup->
      appendOptionsItem (
        msrOptionsTwoBooleansItem::create (
          "tpgrps", "tracePartGroups",
R"(Part groups)",
          "tracePartGroups",
          fTracePartGroups,
          fTraceGeneral));
      
    specificTraceSubGroup->
      appendOptionsItem (
        msrOptionsThreeBooleansItem::create (
          "tpgrpsd", "tracePartGroupsDetails",
R"(Part groups with more details.
This option implies '-tpgrps, -tracePartGroups'.)",
          "tracePartGroupsDetails",
          fTracePartGroupsDetails,
          fTracePartGroups,
          fTraceGeneral));
      
    specificTraceSubGroup->
      appendOptionsItem (
        msrOptionsTwoBooleansItem::create (
          "tparts", "traceParts",
R"(Parts)",
          "traceParts",
          fTraceParts,
          fTraceGeneral));
      
    specificTraceSubGroup->
      appendOptionsItem (
        msrOptionsTwoBooleansItem::create (
          "tstaves", "traceStaves",
R"(Staves)",
          "traceStaves",
          fTraceStaves,
          fTraceGeneral));
      
    specificTraceSubGroup->
      appendOptionsItem (
        msrOptionsTwoBooleansItem::create (
          "tvoices", "traceVoices",
R"(Voices)",
          "traceVoices",
          fTraceVoices,
          fTraceGeneral));
      
    specificTraceSubGroup->
      appendOptionsItem (
        msrOptionsThreeBooleansItem::create (
          "tvdetails", "traceVoicesDetails",
R"(Voices with more details (quite verbose)..
This option implies '-tvdetails, -traceVoicesDetails'.)",
          "traceVoicesDetails",
          fTraceVoicesDetails,
          fTraceVoices,
          fTraceGeneral));
      
    specificTraceSubGroup->
      appendOptionsItem (
        msrOptionsTwoBooleansItem::create (
          "tclefs", "traceClefs",
R"(Clefs)",
          "traceClefs",
          fTraceClefs,
          fTraceGeneral));
      
    specificTraceSubGroup->
      appendOptionsItem (
        msrOptionsTwoBooleansItem::create (
          "tkeys", "traceKeys",
R"(Keys)",
          "traceKeys",
          fTraceKeys,
          fTraceGeneral));
      
    specificTraceSubGroup->
      appendOptionsItem (
        msrOptionsTwoBooleansItem::create (
          "ttimes", "traceTimes",
R"(Times)",
          "traceTimes",
          fTraceTimes,
          fTraceGeneral));
      
    specificTraceSubGroup->
      appendOptionsItem (
        msrOptionsTwoBooleansItem::create (
          "ttransps", "traceTranspositions",
R"(Transpositions (<transpose> in MusicXML,  ransposition in LilyPond))",
          "traceTranspositions",
          fTraceTranspositions,
          fTraceGeneral));
      
    specificTraceSubGroup->
      appendOptionsItem (
        msrOptionsTwoBooleansItem::create (
          "tsegs", "traceSegments",
R"(Voices segments)",
          "traceSegments",
          fTraceSegments,
          fTraceGeneral));
      
    specificTraceSubGroup->
      appendOptionsItem (
        msrOptionsTwoBooleansItem::create (
          "treps", "traceRepeats",
R"(Repeats, including multiple mesure repeats)",
          "traceRepeats",
          fTraceRepeats,
          fTraceGeneral));
      
    specificTraceSubGroup->
      appendOptionsItem (
        msrOptionsTwoBooleansItem::create (
          "tmeas", "traceMeasures",
R"(Measures)",
          "traceMeasures",
          fTraceMeasures,
          fTraceGeneral));
      
    specificTraceSubGroup->
      appendOptionsItem (
        msrOptionsTwoBooleansItem::create (
          "tnotes", "traceNotes",
R"(Notes)",
          "traceNotes",
          fTraceNotes,
          fTraceGeneral));
          
    specificTraceSubGroup->
      appendOptionsItem (
        msrOptionsThreeBooleansItem::create (
          "tndetails", "traceNotesDetails",
R"(Notes with more details, including divisions handling (quite verbose)..
This option implies '-tndetails, -traceNotesDetails'.)",
          "traceNotesDetails",
          fTraceNotesDetails,
          fTraceNotes,
          fTraceGeneral));
      
    specificTraceSubGroup->
      appendOptionsItem (
        msrOptionsTwoBooleansItem::create (
          "tbeams", "traceBeams",
R"(Beams)",
          "traceBeams",
          fTraceBeams,
          fTraceGeneral));
      
    specificTraceSubGroup->
      appendOptionsItem (
        msrOptionsTwoBooleansItem::create (
          "ttechs", "traceTechnicals",
R"(Technicals)",
          "traceTechnicals",
          fTraceTechnicals,
          fTraceGeneral));
      
    specificTraceSubGroup->
      appendOptionsItem (
        msrOptionsTwoBooleansItem::create (
          "twords", "traceWords",
R"(Words)",
          "traceWords",
          fTraceWords,
          fTraceGeneral));
      
    specificTraceSubGroup->
      appendOptionsItem (
        msrOptionsTwoBooleansItem::create (
          "ttrems", "traceTremolos",
R"(Tremolos)",
          "traceTremolos",
          fTraceTremolos,
          fTraceGeneral));
      
    specificTraceSubGroup->
      appendOptionsItem (
        msrOptionsTwoBooleansItem::create (
          "tchords", "traceChords",
R"(Chords)",
          "traceChords",
          fTraceChords,
          fTraceGeneral));
      
    specificTraceSubGroup->
      appendOptionsItem (
        msrOptionsTwoBooleansItem::create (
          "ttups", "traceTuplets",
R"(Tuplets)",
          "traceTuplets",
          fTraceTuplets,
          fTraceGeneral));
      
    specificTraceSubGroup->
      appendOptionsItem (
        msrOptionsTwoBooleansItem::create (
          "tgraces", "traceGraceNotes",
R"(Grace notes)",
          "traceGraceNotes",
          fTraceGraceNotes,
          fTraceGeneral));
      
    specificTraceSubGroup->
      appendOptionsItem (
        msrOptionsTwoBooleansItem::create (
          "tlyrics", "traceLyrics",
R"(Lyrics in MusicXML, stanzas in MSR)",
          "traceLyrics",
          fTraceLyrics,
          fTraceGeneral));
      
    specificTraceSubGroup->
      appendOptionsItem (
        msrOptionsTwoBooleansItem::create (
          "tharms", "traceHarmonies",
R"(<harmony/> in MusicXML, \chordmode in LilyPond)",
          "traceHarmonies",
          fTraceHarmonies,
          fTraceGeneral));
      
    specificTraceSubGroup->
      appendOptionsItem (
        msrOptionsTwoBooleansItem::create (
          "tfigbass", "traceFiguredBass",
R"(<figured-bass> in MusicXML, \figuremode in LilyPond)",
          "traceFiguredBass",
          fTraceFiguredBass,
          fTraceGeneral));
      
    specificTraceSubGroup->
      appendOptionsItem (
        msrOptionsTwoBooleansItem::create (
          "tcredits", "traceCredits",
R"(Credits)",
          "traceCredits",
          fTraceCredits,
          fTraceGeneral));
      
    specificTraceSubGroup->
      appendOptionsItem (
        msrOptionsTwoBooleansItem::create (
          "tdynamics", "traceDynamics",
R"(Dynamics)",
          "traceDynamics",
          fTraceDynamics,
          fTraceGeneral));
      
    specificTraceSubGroup->
      appendOptionsItem (
        msrOptionsTwoBooleansItem::create (
          "tslurs", "traceSlurs",
R"(Slurs)",
          "fTraceSlurs",
          fTraceSlurs,
          fTraceGeneral));
      
    specificTraceSubGroup->
      appendOptionsItem (
        msrOptionsTwoBooleansItem::create (
          "tligatures", "traceLigatures",
R"(Ligatures)",
          "traceLigatures",
          fTraceLigatures,
          fTraceGeneral));
      
    specificTraceSubGroup->
      appendOptionsItem (
        msrOptionsTwoBooleansItem::create (
          "twedges", "traceWedges",
R"(Wedges)",
          "traceWedges",
          fTraceWedges,
          fTraceGeneral));
      
    specificTraceSubGroup->
      appendOptionsItem (
        msrOptionsTwoBooleansItem::create (
          "traceStaffTuning", "",
R"(Staff tuning)",
          "traceStaffTuning",
          fTraceStaffTuning,
          fTraceGeneral));
      
    specificTraceSubGroup->
      appendOptionsItem (
        msrOptionsTwoBooleansItem::create (
          "traceHarpPedalsTuning", "",
R"(Harp pedals tuning)",
          "traceHarpPedalsTuning",
          fTraceHarpPedalsTuning,
          fTraceGeneral));


    // '-exit2a' is hidden...
    S_msrOptionsBooleanItem
      exit2aOptionsBooleanItem =
        msrOptionsBooleanItem::create (
          "e2a", "exit2a",
R"(Exit after pass 2a, i.e. after conversion
of the MusicXML tree to an MSR skeleton.)",
          "exit2a",
          fExit2a);
    exit2aOptionsBooleanItem->
      setOptionsElementIsHidden ();
      
    specificTraceSubGroup->
      appendOptionsItem (
        exit2aOptionsBooleanItem);

    // '-exit2b' is hidden...
    S_msrOptionsBooleanItem
      exit2bOptionsBooleanItem =
        msrOptionsBooleanItem::create (
          "e2b", "exit2b",
R"(Exit after pass 2b, i.e. after conversion
of the MusicXML tree to MSR.)",
          "exit2b",
          fExit2b);
    exit2bOptionsBooleanItem->
      setOptionsElementIsHidden ();
      
    specificTraceSubGroup->
      appendOptionsItem (
        exit2bOptionsBooleanItem);

    // '-exit3' is hidden...
    S_msrOptionsBooleanItem
      exit3OptionsBooleanItem =
        msrOptionsBooleanItem::create (
          "e3", "exit3",
R"(Exit after pass 3, i.e. after conversion
of the MSR to LPSR.)",
          "exit3",
          fExit3);
    exit3OptionsBooleanItem->
      setOptionsElementIsHidden ();
      
    specificTraceSubGroup->
      appendOptionsItem (
        exit3OptionsBooleanItem);
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

S_generalOptions generalOptions::createCloneWithDetailedTrace ()
{
  S_generalOptions
    clone =
      generalOptions::create (0);
      // 0 not to have it inserted twice in teh option handler

  // set the options handler uplink
  clone->
    setOptionsHandlerUplink (
      fOptionsHandlerUplink);


  // command line
  // --------------------------------------

  clone->fProgramName =
    fProgramName;
      
  clone->fCommandLineWithLongOptions =
    fCommandLineWithLongOptions;
  clone->fCommandLineWithShortOptions =
    fCommandLineWithShortOptions;

  
  // input
  // --------------------------------------

  clone->fInputSourceName =
    fInputSourceName;
      
  clone->fTranslationDate =
    fTranslationDate;


  // output file
  // --------------------------------------

  clone->fAutoOutputFile =
    fAutoOutputFile;

  
  // trace and display
  // --------------------------------------

  clone->fTraceGeneral = true;
    
  clone->fTraceDetailedMeasureNumbersSet =
    fTraceDetailedMeasureNumbersSet;


  // CPU usage
  // --------------------------------------

  clone->fDisplayCPUusage =
    true;

    
  // specific trace
  // --------------------------------------

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

  // transpositions
  clone->fTraceTranspositions = true;

  // segments
  clone->fTraceSegments = true;
    
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
  clone->fTraceLyrics = true;
  
  // harmonies
  clone->fTraceHarmonies = true;
  
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

void generalOptions::printGeneralOptionsValues (int fieldWidth)
{  
  gLogIOstream <<
    "The general options are:" <<
    endl;

  gIndenter++;

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

  // trace and display
  // --------------------------------------

  gLogIOstream << left <<
    setw (fieldWidth) << "Trace and display:" <<
    endl;

  gIndenter++;

  gLogIOstream << left <<
    setw (fieldWidth) << "traceGeneral" << " : " <<
    booleanAsString (fTraceGeneral) <<
     endl <<
    setw (fieldWidth) << "traceDetailedMeasureNumbersSet" << " : ";
        
  if (fTraceDetailedMeasureNumbersSet.empty ())
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

  // CPU usage
  // --------------------------------------

  gLogIOstream << left <<
    setw (fieldWidth) << "CPU usage:" <<
    endl;

  gIndenter++;

  gLogIOstream <<
    setw (fieldWidth) << "displayCPUusage" << " : " <<
    booleanAsString (fDisplayCPUusage) <<
    endl;

  gIndenter--;
  
  // specific trace    
  // --------------------------------------

  gLogIOstream << left <<
    setw (fieldWidth) << "Specific trace:" <<
    endl;

  gIndenter++;

  gLogIOstream << left <<
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
    
    // transpositions
    setw (fieldWidth) << "traceTranspositions" << " : " <<
    booleanAsString (fTraceTranspositions) <<
    endl <<
    
    // segments
    setw (fieldWidth) << "traceSegments" << " : " <<
    booleanAsString (fTraceSegments) <<
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
    
    // harmonies
    setw (fieldWidth) << "traceHarmonies" << " : " <<
    booleanAsString (fTraceHarmonies) <<
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

ostream& operator<< (ostream& os, const S_generalOptions& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
void initializeGeneralOptionsHandling (
  S_msrOptionsHandler optionsHandler)
{  
  // create the options variables
  // ------------------------------------------------------
  
  gGeneralOptionsUserChoices = generalOptions::create (
    optionsHandler);
  assert(gGeneralOptionsUserChoices != 0);

  gGeneralOptions =
    gGeneralOptionsUserChoices;

  // prepare for measure detailed trace
  // ------------------------------------------------------

  gGeneralOptionsWithDetailedTrace =
    gGeneralOptions->
      createCloneWithDetailedTrace ();
}


}
