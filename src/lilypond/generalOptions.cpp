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

// useful shortcut macros
#define idtr indenter::gIndenter
#define tab  indenter::gIndenter.getSpacer ()

//_______________________________________________________________________________
S_generalOptions gGeneralOptions;
S_generalOptions gGeneralOptionsUserChoices;
S_generalOptions gGeneralOptionsWithDetailedTrace;

S_generalOptions generalOptions::create ()
{
  generalOptions* o = new generalOptions();
  assert(o!=0);
  return o;
}

generalOptions::generalOptions ()
  : msrOptionsGroup (
    "General",
    "hg", "helpGeneral",
R"()"
    )
{
  initializeGeneralOptions (false);
}

generalOptions::~generalOptions ()
{}

void generalOptions::initializeGeneralOptions (
  bool boolOptionsInitialValue)
{
  // version
  // --------------------------------------

  {
    // variables  
  
    fVersion = false;
  
    // options
  
    S_msrOptionsSubGroup versionSubGroup =
      msrOptionsSubGroup::create (
        "Version",
        "hgb", "helpGeneralVersion",
R"()",
      msrOptionsSubGroup::kAlwaysShowDescription
      );
  
    appendOptionsSubGroup (versionSubGroup);
  
    versionSubGroup->
      appendOptionsItem (
        msrOptionsBooleanItem::create (
          "v", "version",
R"(Display xml2lilypond's version number and exit.)",
          "version",
          fVersion));
  }


  // output file
  // --------------------------------------

  {
    // variables  
  
    fAutoOutputFile = false;
    
    // options
  
    S_msrOptionsSubGroup outputFileSubGroup =
      msrOptionsSubGroup::create (
        "Output file",
        "hgof", "helpGeneralOutputFile",
R"()",
      msrOptionsSubGroup::kAlwaysShowDescription
      );
            
    appendOptionsSubGroup (outputFileSubGroup);
    
    outputFileSubGroup->
      appendOptionsItem (
        msrOptionsStringItem::create (
          "of", "outputFile",
R"(Write LilyPond code to file 'fileName' instead of standard output.)",
          "fileName",
          "outputFile",
          fOutputFileName));  
  
    outputFileSubGroup->
      appendOptionsItem (
        msrOptionsBooleanItem::create (
          "aof", "autoOutputFile",
R"(This option can only be used when reading from a file.
Write LilyPond code to a file in the current working directory.
The file name is derived from that of the input file,
replacing any suffix after the the '.' by 'ly'
or adding '.ly' if none is present.)",
          "autoOutputFile",
          fAutoOutputFile));
  }


  // trace and display
  // --------------------------------------

  {
    // variables  
    
    fTraceGeneral = boolOptionsInitialValue;
    
    // fTraceDetailedMeasureNumbersSet is empty
    
    // options
    
      S_msrOptionsSubGroup traceAndDisplaySubGroup =
        msrOptionsSubGroup::create (
          "Trace and display",
          "hgtd", "helpGeneralTraceDansDisplay",
R"()",
      msrOptionsSubGroup::kAlwaysShowDescription
        );
    
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
  
    S_msrOptionsSubGroup CPUUsageSubGroup =
      msrOptionsSubGroup::create (
        "CPU usage",
        "hgcu", "helpGeneralCPUUsage",
R"()",
      msrOptionsSubGroup::kAlwaysShowDescription
      );
  
    appendOptionsSubGroup (CPUUsageSubGroup);
        
    CPUUsageSubGroup->
      appendOptionsItem (
        msrOptionsBooleanItem::create (
          "dCPU", "displayCPUusage",
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
      
    // options
  
    S_msrOptionsSubGroup specificTraceSubGroup =
      msrOptionsSubGroup::create (
        "Specific trace about elements",
        "hgst", "helpGeneralSpecificTrace",
R"()",
      msrOptionsSubGroup::kHideDescriptionByDefault
      );
  
    appendOptionsSubGroup (specificTraceSubGroup);

    specificTraceSubGroup->
      appendOptionsItem (
        msrOptionsBooleanItem::create (
          "tdivs", "traceDivisions",
R"(divisions)",
          "traceDivisions",
          fTraceDivisions));
      
    specificTraceSubGroup->
      appendOptionsItem (
        msrOptionsBooleanItem::create (
          "tpgrps", "tracePartGroups",
R"(part groups)",
          "tracePartGroups",
          fTracePartGroups));
      
    specificTraceSubGroup->
      appendOptionsItem (
        msrOptionsBooleanItem::create (
          "tparts", "traceParts",
R"(parts)",
          "traceParts",
          fTraceParts));
      
    specificTraceSubGroup->
      appendOptionsItem (
        msrOptionsBooleanItem::create (
          "tstaves", "traceStaves",
R"(staves)",
          "traceStaves",
          fTraceStaves));
      
    specificTraceSubGroup->
      appendOptionsItem (
        msrOptionsBooleanItem::create (
          "tvoices", "traceVoices",
R"(voices)",
          "traceVoices",
          fTraceVoices));
      
    specificTraceSubGroup->
      appendOptionsItem (
        msrOptionsBooleanItem::create (
          "tvdetails", "traceVoicesDetails",
R"(voices with more details (quite verbose))",
          "traceVoicesDetails",
          fTraceVoicesDetails));
      
    specificTraceSubGroup->
      appendOptionsItem (
        msrOptionsBooleanItem::create (
          "tclefs", "traceClefs",
R"(clefs)",
          "traceClefs",
          fTraceClefs));
      
    specificTraceSubGroup->
      appendOptionsItem (
        msrOptionsBooleanItem::create (
          "tkeys", "traceKeys",
R"(keys)",
          "traceKeys",
          fTraceKeys));
      
    specificTraceSubGroup->
      appendOptionsItem (
        msrOptionsBooleanItem::create (
          "ttimes", "traceTimes",
R"(times)",
          "traceTimes",
          fTraceTimes));
      
    specificTraceSubGroup->
      appendOptionsItem (
        msrOptionsBooleanItem::create (
          "ttransps", "traceTranspositions",
R"(transpositions (<transpose> in MusicXML,  ransposition in LilyPond))",
          "traceTranspositions",
          fTraceTranspositions));
      
    specificTraceSubGroup->
      appendOptionsItem (
        msrOptionsBooleanItem::create (
          "tsegs", "traceSegments",
R"(voices segments)",
          "traceSegments",
          fTraceSegments));
      
    specificTraceSubGroup->
      appendOptionsItem (
        msrOptionsBooleanItem::create (
          "treps", "traceRepeats",
R"(repeats)",
          "traceRepeats",
          fTraceRepeats));
      
    specificTraceSubGroup->
      appendOptionsItem (
        msrOptionsBooleanItem::create (
          "tmeas", "traceMeasures",
R"(measures)",
          "traceMeasures",
          fTraceMeasures));
      
    specificTraceSubGroup->
      appendOptionsItem (
        msrOptionsBooleanItem::create (
          "tnotes", "traceNotes",
R"(notes)",
          "traceNotes",
          fTraceNotes));
          
    specificTraceSubGroup->
      appendOptionsItem (
        msrOptionsBooleanItem::create (
          "tndetails", "traceNotesDetails",
R"(notes with more details, including divisions handling (quite verbose))",
          "traceNotesDetails",
          fTraceNotesDetails));
      
    specificTraceSubGroup->
      appendOptionsItem (
        msrOptionsBooleanItem::create (
          "tbeams", "traceBeams",
R"(beams)",
          "traceBeams",
          fTraceBeams));
      
    specificTraceSubGroup->
      appendOptionsItem (
        msrOptionsBooleanItem::create (
          "ttechs", "traceTechnicals",
R"(technicals)",
          "traceTechnicals",
          fTraceTechnicals));
      
    specificTraceSubGroup->
      appendOptionsItem (
        msrOptionsBooleanItem::create (
          "twords", "traceWords",
R"(words)",
          "traceWords",
          fTraceWords));
      
    specificTraceSubGroup->
      appendOptionsItem (
        msrOptionsBooleanItem::create (
          "ttrems", "traceTremolos",
R"(tremolos)",
          "traceTremolos",
          fTraceTremolos));
      
    specificTraceSubGroup->
      appendOptionsItem (
        msrOptionsBooleanItem::create (
          "tchords", "traceChords",
R"(chords)",
          "traceChords",
          fTraceChords));
      
    specificTraceSubGroup->
      appendOptionsItem (
        msrOptionsBooleanItem::create (
          "ttups", "traceTuplets",
R"(tuplets)",
          "traceTuplets",
          fTraceTuplets));
      
    specificTraceSubGroup->
      appendOptionsItem (
        msrOptionsBooleanItem::create (
          "tgraces", "traceGraceNotes",
R"(grace notes)",
          "traceGraceNotes",
          fTraceGraceNotes));
      
    specificTraceSubGroup->
      appendOptionsItem (
        msrOptionsBooleanItem::create (
          "tlyrics", "traceLyrics",
R"(lyrics in MusicXML, stanzas in MSR)",
          "traceLyrics",
          fTraceLyrics));
      
    specificTraceSubGroup->
      appendOptionsItem (
        msrOptionsBooleanItem::create (
          "tharms", "traceHarmonies",
R"(<harmony/> in MusicXML, \chordmode in LilyPond)",
          "traceHarmonies",
          fTraceHarmonies));
      
    specificTraceSubGroup->
      appendOptionsItem (
        msrOptionsBooleanItem::create (
          "tfigbass", "traceFiguredBass",
R"(<figured-bass> in MusicXML, \figuremode in LilyPond)",
          "traceFiguredBass",
          fTraceFiguredBass));
      
    specificTraceSubGroup->
      appendOptionsItem (
        msrOptionsBooleanItem::create (
          "tcredits", "traceCredits",
R"(credits)",
          "traceCredits",
          fTraceCredits));
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
      generalOptions::create ();

  // command line
  // --------------------------------------

  clone->fProgramName =
    fProgramName;
      
  clone->fCommandLineLongOptions =
    fCommandLineLongOptions;
  clone->fCommandLineShortOptions =
    fCommandLineShortOptions;
  
  // input
  // --------------------------------------

  clone->fInputSourceName =
    fInputSourceName;
      
  clone->fTranslationDate =
    fTranslationDate;

  // version
  // --------------------------------------

  clone->fVersion =
    fVersion;

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
  cerr << idtr <<
    "The general options are:" <<
    endl;

  idtr++;

  // command line
  // --------------------------------------

  cerr << left <<
    idtr <<
      setw (fieldWidth) << "Command line:" <<
      endl;

  idtr++;

  cerr << left <<
    idtr <<
      setw (fieldWidth) << "input source name" << " : " <<
      fInputSourceName <<
      endl <<
      
    idtr <<
      setw (fieldWidth) << "translation date" << " : " <<
      fTranslationDate <<
      endl;

  idtr--;

  // output file
  // --------------------------------------

  cerr << left <<
    idtr <<
      setw (fieldWidth) << "Output file:" <<
      endl;

  idtr++;

  cerr << left <<        
    idtr <<
      setw (fieldWidth) << "outputFileName" << " : \"" <<
      fOutputFileName <<
      "\"" <<
      endl <<
    idtr <<
      setw (fieldWidth) << "autoOutputFile" << " : " <<
      booleanAsString (fAutoOutputFile) <<
      endl;

  idtr--;

  // trace and display
  // --------------------------------------

  cerr << left <<
    idtr <<
      setw (fieldWidth) << "Trace and display:" <<
      endl;

  idtr++;

  cerr << left <<
    idtr <<
      setw (fieldWidth) << "traceGeneral" << " : " <<
      booleanAsString (fTraceGeneral) <<
       endl <<
    idtr <<
      setw (fieldWidth) << "traceDetailedMeasureNumbersSet" << " : ";
        
    if (fTraceDetailedMeasureNumbersSet.empty ())
      cerr << "none";
      
    else {
      cerr <<
        "'";
        
      set<int>::const_iterator
        iBegin = fTraceDetailedMeasureNumbersSet.begin(),
        iEnd   = fTraceDetailedMeasureNumbersSet.end(),
        i      = iBegin;
        
      for ( ; ; ) {
        cerr << (*i);
        if (++i == iEnd) break;
        cerr << " ";
      } // for
    
      cerr <<
        "'";
    }
      
    cerr <<
      endl;

  idtr--;

  // CPU usage
  // --------------------------------------

  cerr << left <<
    idtr <<
      setw (fieldWidth) << "CPU usage:" <<
      endl;

  idtr++;

  cerr <<
    idtr <<
      setw (fieldWidth) << "displayCPUusage" << " : " <<
      booleanAsString (fDisplayCPUusage) <<
      endl;

  idtr--;
  
  // specific trace    
  // --------------------------------------

  cerr << left <<
    idtr <<
      setw (fieldWidth) << "Specific trace:" <<
      endl;

  idtr++;

  cerr << left <<
    // divisions
    idtr <<
      setw (fieldWidth) << "traceDivisions" << " : " <<
      booleanAsString (fTraceDivisions) <<
      endl <<
        
    // geometry
    idtr <<
      setw (fieldWidth) << "traceGeometry" << " : " <<
      booleanAsString (fTraceGeometry) <<
      endl <<
        
    // part groups
    idtr <<
      setw (fieldWidth) << "tracePartGroups" << " : " <<
      booleanAsString (fTracePartGroups) <<
      endl <<
      
    // parts
    idtr <<
      setw (fieldWidth) << "traceParts" << " : " <<
      booleanAsString (fTraceParts) <<
      endl <<
      
    // staves
    idtr <<
      setw (fieldWidth) << "traceStaves" << " : " <<
      booleanAsString (fTraceStaves) <<
      endl <<
      
    // voices
    idtr <<
      setw (fieldWidth) << "traceVoices" << " : " <<
      booleanAsString (fTraceVoices) <<
      endl <<
    idtr <<
      setw (fieldWidth) << "traceVoicesDetails" << " : " <<
      booleanAsString (fTraceVoicesDetails) <<
      endl <<

    // clefs
    idtr <<
      setw (fieldWidth) << "traceClefs" << " : " <<
      booleanAsString (fTraceClefs) <<
      endl <<
    // keys
    idtr <<
      setw (fieldWidth) << "traceKeys" << " : " <<
      booleanAsString (fTraceKeys) <<
      endl <<
    // times
    idtr <<
      setw (fieldWidth) << "traceTimes" << " : " <<
      booleanAsString (fTraceTimes) <<
      endl <<

    // transpositions
    idtr <<
      setw (fieldWidth) << "traceTranspositions" << " : " <<
      booleanAsString (fTraceTranspositions) <<
      endl <<

    // segments
    idtr <<
      setw (fieldWidth) << "traceSegments" << " : " <<
      booleanAsString (fTraceSegments) <<
      endl <<

    // repeats
    idtr <<
      setw (fieldWidth) << "traceRepeats" << " : " <<
      booleanAsString (fTraceRepeats) <<
      endl <<

    // measures
    idtr <<
      setw (fieldWidth) << "traceMeasures" << " : " <<
      booleanAsString (fTraceMeasures) <<
      endl <<
      
    // notes
    idtr <<
      setw (fieldWidth) << "traceNotes" << " : " <<
      booleanAsString (fTraceNotes) <<
      endl <<
    idtr <<
      setw (fieldWidth) << "traceNotesDetails" << " : " <<
      booleanAsString (fTraceNotesDetails) <<
      endl <<

    // beams
    idtr <<
      setw (fieldWidth) << "traceBeams" << " : " <<
      booleanAsString (fTraceBeams) <<
      endl <<

    // technicals
    idtr <<
      setw (fieldWidth) << "traceTechnicals" << " : " <<
      booleanAsString (fTraceTechnicals) <<
      endl <<

    // words
    idtr <<
      setw (fieldWidth) << "traceWords" << " : " <<
      booleanAsString (fTraceWords) <<
      endl <<

    // tremolos
    idtr <<
      setw (fieldWidth) << "traceTremolos" << " : " <<
      booleanAsString (fTraceTremolos) <<
      endl <<

    // chords
    idtr <<
      setw (fieldWidth) << "traceChords" << " : " <<
      booleanAsString (fTraceChords) <<
      endl <<

    // tuplets
    idtr <<
      setw (fieldWidth) << "traceTuplets" << " : " <<
      booleanAsString (fTraceTuplets) <<
      endl <<

    // grace notes
    idtr <<
      setw (fieldWidth) << "traceGraceNotes" << " : " <<
      booleanAsString (fTraceGraceNotes) <<
      endl <<

    // lyrics
    idtr <<
      setw (fieldWidth) << "traceLyrics" << " : " <<
      booleanAsString (fTraceLyrics) <<
      endl <<
      
    // harmonies
    idtr <<
      setw (fieldWidth) << "traceHarmonies" << " : " <<
      booleanAsString (fTraceHarmonies) <<
      endl <<
      
    // figured bass
    idtr <<
      setw (fieldWidth) << "traceFiguredBass" << " : " <<
      booleanAsString (fTraceFiguredBass) <<
      endl <<
      
    // credits
    idtr <<
      setw (fieldWidth) << "traceCredits" << " : " <<
      booleanAsString (fTraceCredits) <<
      endl;

  idtr--;

  idtr--;
}

ostream& operator<< (ostream& os, const S_generalOptions& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
void initializeGeneralOptions ()
{  
  gGeneralOptionsUserChoices = generalOptions::create ();
  assert(gGeneralOptionsUserChoices != 0);

  gGeneralOptions =
    gGeneralOptionsUserChoices;

  gGeneralOptionsWithDetailedTrace =
    gGeneralOptions->
      createCloneWithDetailedTrace ();
}


}
