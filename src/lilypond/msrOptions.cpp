/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#include <climits>      /* INT_MIN, INT_MAX */
#include <iomanip>      // setw, set::precision, ...
#include <sstream>

#include "utilities.h"

#include "msrOptions.h"


using namespace std;

namespace MusicXML2 
{

//_______________________________________________________________________________
S_msrOptions gMsrOptions;
S_msrOptions gMsrOptionsUserChoices;
S_msrOptions gMsrOptionsWithDetailedTrace;

S_msrOptions msrOptions::create (
  S_msrOptionsHandler optionsHandler)
{
  msrOptions* o = new msrOptions(
    optionsHandler);
  assert(o!=0);
  return o;
}

msrOptions::msrOptions (
  S_msrOptionsHandler optionsHandler)
  : msrOptionsGroup (
    "MSR",
    "hmsr", "helpMSR",
R"(These options control the way MSR data is handled.)",
    optionsHandler)
{
  // append this options group to the options handler
  // if relevant
  if (optionsHandler) {
    optionsHandler->
      appendOptionsGroup (this);
  }

  // initialize it
  initializeMsrOptions (false);
}

msrOptions::~msrOptions ()
{}

void msrOptions::initializeMsrOptions (
  bool boolOptionsInitialValue)
{
  // trace and display
  // --------------------------------------

  {
    // variables
  
    fTraceMsr          = boolOptionsInitialValue;
    
    fTraceMsrVisitors  = boolOptionsInitialValue;
  
    fDisplayPartGroups = boolOptionsInitialValue;
    
    fDisplayMsr        = boolOptionsInitialValue;
  
    fDisplayMsrSummary = boolOptionsInitialValue;
  
    // options
  
    S_msrOptionsSubGroup traceAndDisplaySubGroup =
      msrOptionsSubGroup::create (
        "Trace and display",
        "hmsrtd", "helpMsrTraceAndDisplay",
R"()",
      msrOptionsSubGroup::kAlwaysShowDescription,
      this);
  
    appendOptionsSubGroup (traceAndDisplaySubGroup);
      
    traceAndDisplaySubGroup->
      appendOptionsItem (
        msrOptionsBooleanItem::create (
          "tmsr", "traceMsr",
R"(Write a trace of the LPSR graphs visiting activity to standard error.)",
          "traceMsr",
          fTraceMsr));
          
    traceAndDisplaySubGroup->
      appendOptionsItem (
        msrOptionsBooleanItem::create (
          "tmsrv", "traceMsrVisitors",
R"(Write a trace of the MSR graphs visiting activity to standard error.)",
          "traceMsrVisitors",
          fTraceMsrVisitors));
          
    traceAndDisplaySubGroup->
      appendOptionsItem (
        msrOptionsBooleanItem::create (
          "pg", "displayPartGroups",
R"(Write the structure of the part groups to standard error.)",
          "displayPartGroups",
          fDisplayPartGroups));
          
    traceAndDisplaySubGroup->
      appendOptionsItem (
        msrOptionsBooleanItem::create (
          "msr", "displayMsr",
R"(Write the contents of the MSR data to standard error.)",
          "displayMsr",
          fDisplayPartGroups));
          
    traceAndDisplaySubGroup->
      appendOptionsItem (
        msrOptionsBooleanItem::create (
          "sum", "displayMsrSummary", // JMI
R"(Only write a summary of the MSR to standard error.
This implies that no LilyPond code is generated.)",
          "displayMsrSummary",
          fDisplayMsrSummary));
  }

  
  // languages
  // --------------------------------------

  {
    // variables
    
    if (! setMsrQuarterTonesPitchesLanguage ("nederlands")) {
      stringstream s;
  
      s <<
        "INTERNAL INITIALIZATION ERROR: "
        "MSR pitches language 'nederlands' is unknown" <<
        endl <<
        "The " <<
        gQuarterTonesPitchesLanguageKindsMap.size () <<
        " known MSR pitches languages are:" <<
        endl;
  
      gIndenter++;
    
      s <<
        existingQuarterTonesPitchesLanguageKinds ();
  
      gIndenter--;
  
      optionError (s.str ());
    }
    
    // options
  
    S_msrOptionsSubGroup languagesSubGroup =
      msrOptionsSubGroup::create (
        "Languages",
        "hmsrlang", "helpMsrLanguages",
R"()",
      msrOptionsSubGroup::kAlwaysShowDescription,
      this);
  
    appendOptionsSubGroup (languagesSubGroup);

    languagesSubGroup->
      appendOptionsItem (
        msrOptionsPitchesLanguageItem::create (
          "mpl", "msrPitchesLanguage",
R"(Use 'language' to display note pitches in the MSR logs and text views.
The 12 LilyPond pitches languages are available:
nederlands, catalan, deutsch, english, espanol, français, 
italiano, norsk, portugues, suomi, svenska and vlaams.
The default is to use 'nederlands'.)",
          "language",
          "msrPitchesLanguage",
          fMsrQuarterTonesPitchesLanguageKind));
  }

  
  // parts
  // --------------------------------------

  {
    // variables
    
    // options
  
    S_msrOptionsSubGroup partsSubGroup =
      msrOptionsSubGroup::create (
        "Parts",
        "hmsrp", "helpMsrParts",
R"()",
      msrOptionsSubGroup::kAlwaysShowDescription,
      this);
  
    appendOptionsSubGroup (partsSubGroup);

    partsSubGroup->
      appendOptionsItem (
        msrOptionsPartRenameItem::create (
          "mpr", "msrPartRename", // JMI
R"(Rename part 'original' to 'newName', for example after 
displaying a summary of the score in a first xml2lilypond run.
'partRenameSpecification' can be:
  'originalName = newName'
or
  "originalName = newName"
The single or double quotes are used to allow spaces in the names
and around the '=' sign, otherwise they can be dispensed with.
Using double quotes allows for shell variables substitutions, as in:
  DESSUS="Cor anglais"
  xml2lilypond -msrPartRename "P1 = ${DESSUS}" .
There can be several occurrences of this option.)",
          "partRenameSpecification",
          "partRename",
          fPartsRenamingMap));
  }
      

  // voices
  // --------------------------------------

  {
    // variables
    
    fCreateVoicesStaffRelativeNumbers = boolOptionsInitialValue;
      
    // options
  
    S_msrOptionsSubGroup voicesSubGroup =
      msrOptionsSubGroup::create (
        "Voices",
        "hmsrv", "helpMsrVoices",
R"()",
      msrOptionsSubGroup::kAlwaysShowDescription,
      this);
  
    appendOptionsSubGroup (voicesSubGroup);

    voicesSubGroup->
      appendOptionsItem (
        msrOptionsBooleanItem::create (
          "cvsrvn", "createVoicesStaffRelativeNumbers",
R"(Generate voices names with numbers relative to their staff.
By default, the voice numbers found are used, 
which may be global to the score.)",
          "createVoicesStaffRelativeNumbers",
          fCreateVoicesStaffRelativeNumbers));
  }


  // notes
  // --------------------------------------

  {
    // variables
    
    fDelayRestsDynamics  = boolOptionsInitialValue;
    fDelayRestsWords     = boolOptionsInitialValue; // JMI
    fDelayRestsSlurs     = boolOptionsInitialValue; // JMI
    fDelayRestsLigatures = boolOptionsInitialValue; // JMI
    fDelayRestsWedges    = boolOptionsInitialValue; // JMI
  
    // options
  
    S_msrOptionsSubGroup notesSubGroup =
      msrOptionsSubGroup::create (
        "Notes",
        "hmsrn", "helpMsrNotes",
R"()",
      msrOptionsSubGroup::kAlwaysShowDescription,
      this);
  
    appendOptionsSubGroup (notesSubGroup);

    notesSubGroup->
      appendOptionsItem (
        msrOptionsBooleanItem::create (
          "drdyns", "delayRestsDynamics",
R"(dynamics)",
          "delayRestsDynamics",
          fDelayRestsDynamics));

    notesSubGroup->
      appendOptionsItem (
        msrOptionsBooleanItem::create (
          "drwords", "delayRestsWords",
R"(words)",
          "delayRestsWords",
          fDelayRestsWords));

    notesSubGroup->
      appendOptionsItem (
        msrOptionsBooleanItem::create (
          "drslurs", "delayRestsSlurs",
R"(slurs)",
          "delayRestsSlurs",
          fDelayRestsSlurs));

    notesSubGroup->
      appendOptionsItem (
        msrOptionsBooleanItem::create (
          "drligs", "delayRestsLigatures",
R"(<bracket/> in MusicXML, '\[... \}' in LilyPond)",
          "delayRestsLigatures",
          fDelayRestsLigatures));

    notesSubGroup->
      appendOptionsItem (
        msrOptionsBooleanItem::create (
          "drwedges", "delayRestsWedges",
R"('<wedge/>' in MusicXML, '<!' in LilyPond)",
          "delayRestsWedges",
          fDelayRestsWedges));
  }


  // lyrics
  // --------------------------------------

  {
    // variables
    
    fShowMsrStanzas  = boolOptionsInitialValue;
  
    // options
  
    S_msrOptionsSubGroup lyricsSubGroup =
      msrOptionsSubGroup::create (
        "Lyrics",
        "hmsrlyrd", "helpMsrLyrics",
R"()",
      msrOptionsSubGroup::kAlwaysShowDescription,
      this);
  
    appendOptionsSubGroup (lyricsSubGroup);

    lyricsSubGroup->
      appendOptionsItem (
        msrOptionsBooleanItem::create (
          "sms", "showMsrStanzas",
R"(Show MSR stanzas even when they're empty.)",
          "showMsrStanzas",
          fShowMsrStanzas));
  }     


  // harmonies
  // --------------------------------------

  {
    // variables
    
    fShowHarmonyVoices      = boolOptionsInitialValue;  
  
    // options
  
    S_msrOptionsSubGroup harmoniesSubGroup =
      msrOptionsSubGroup::create (
        "Harmonies",
        "hmsrh", "helpMsrHarmonies",
R"()",
      msrOptionsSubGroup::kAlwaysShowDescription,
      this);
  
    appendOptionsSubGroup (harmoniesSubGroup);

    harmoniesSubGroup->
      appendOptionsItem (
        msrOptionsBooleanItem::create (
          "shv", "showHarmonyVoices",
R"(Show the parts harmony voices in the MSR data
even though it does not contain music.)",
          "showHarmonyVoices",
          fShowHarmonyVoices));
  }


  // figured bass
  // --------------------------------------

  {
    // variables  
    
    fShowFiguredBassVoices      = boolOptionsInitialValue;
  
    // options
  
    S_msrOptionsSubGroup figuredBassSubGroup =
      msrOptionsSubGroup::create (
        "Figured bass",
        "hmsrfb", "helpMsrFiguredBass",
R"()",
      msrOptionsSubGroup::kAlwaysShowDescription,
      this);
  
    appendOptionsSubGroup (figuredBassSubGroup);

    figuredBassSubGroup->
      appendOptionsItem (
        msrOptionsBooleanItem::create (
          "sfbv", "showFiguredBassVoices",
R"(Show the figured bass harmony voices in the MSR data
even though they do not contain music.)",
          "showFiguredBassVoices",
          fShowFiguredBassVoices));
  }
}

S_msrOptions msrOptions::createCloneWithDetailedTrace ()
{
  S_msrOptions
    clone =
      msrOptions::create (0);
      // 0 not to have it inserted twice in teh option handler

  // set the options handler uplink
  clone->
    setOptionsHandlerUplink (
      fOptionsHandlerUplink);


  // trace and display
  // --------------------------------------

  clone->fTraceMsr =
    true;

  clone->fTraceMsrVisitors =
    true;

  clone->fDisplayPartGroups =
    true;

  clone->fDisplayMsr =
    true;

  clone->fDisplayMsrSummary =
    fDisplayMsrSummary;


  // languages
  // --------------------------------------
    
  clone->fMsrQuarterTonesPitchesLanguageKind =
    fMsrQuarterTonesPitchesLanguageKind;


  // parts
  // --------------------------------------

  clone->fPartsRenamingMap =
    fPartsRenamingMap;


  // voices
  // --------------------------------------
  
  clone->fCreateVoicesStaffRelativeNumbers =
    fCreateVoicesStaffRelativeNumbers;


  // notes
  // --------------------------------------

  clone->fDelayRestsDynamics =
    fDelayRestsDynamics;
  clone->fDelayRestsWords =
    fDelayRestsWords;
  clone->fDelayRestsSlurs =
    fDelayRestsSlurs;
  clone->fDelayRestsLigatures =
    fDelayRestsLigatures;
  clone->fDelayRestsWedges =
    fDelayRestsWedges;


  // lyrics
  // --------------------------------------
    
  clone->fShowMsrStanzas =
    fShowMsrStanzas;


  // harmonies
  // --------------------------------------

  clone->fShowHarmonyVoices =
    fShowHarmonyVoices;


  // figured bass
  // --------------------------------------
  
  clone->fShowFiguredBassVoices =
    fShowFiguredBassVoices;

  return clone;
}  

bool msrOptions::setMsrQuarterTonesPitchesLanguage (string language)
{
  // is language in the pitches languages map?
  map<string, msrQuarterTonesPitchesLanguageKind>::const_iterator
    it =
      gQuarterTonesPitchesLanguageKindsMap.find (language);
        
  if (it == gQuarterTonesPitchesLanguageKindsMap.end ()) {
    // no, language is unknown in the map
    return false;
  }

  fMsrQuarterTonesPitchesLanguageKind = (*it).second;
  
  return true;
}

void msrOptions::printMsrOptionsValues (int fieldWidth)
{
  gLogIOstream <<
    "The MSR options are:" <<
    endl;

  gIndenter++;
  
  // trace and display
  // --------------------------------------
  
  gLogIOstream <<
    "Trace and display:" <<
    endl;

  gIndenter++;
  
  gLogIOstream << left <<
    setw (fieldWidth) << "traceMsr" << " : " <<
    booleanAsString (fTraceMsr) <<
    endl <<
  
    setw (fieldWidth) << "traceMsrVisitors" << " : " <<
    booleanAsString (fTraceMsrVisitors) <<
    endl <<

    setw (fieldWidth) << "displayPartGroups" << " : " <<
    booleanAsString (fDisplayPartGroups) <<
    endl <<

    setw (fieldWidth) << "displayMsr" << " : " <<
    booleanAsString (fDisplayMsr) <<
    endl <<

    setw (fieldWidth) << "displayMsrSummary" << " : " <<
    booleanAsString (fDisplayMsrSummary) <<
    endl;    

  gIndenter--;

  // languages
  // --------------------------------------
  
  gLogIOstream <<
     "Languages:" <<
    endl;

  gIndenter++;

  gLogIOstream << left <<
    setw (fieldWidth) << "msrPitchesLanguage" << " : \"" <<
    msrQuarterTonesPitchesLanguageKindAsString (
      fMsrQuarterTonesPitchesLanguageKind) <<
      "\"" <<
    endl;

  gIndenter--;

  // parts
  // --------------------------------------
  
  gLogIOstream <<
     "Parts:" <<
    endl;

  gIndenter++;

  gLogIOstream << left <<
    setw (fieldWidth) << "parts renaming" << " : ";

  if (fPartsRenamingMap.empty ()) {
    gLogIOstream <<
      "none";
  }
  
  else {
    for (
      map<string, string>::const_iterator i =
        fPartsRenamingMap.begin ();
      i != fPartsRenamingMap.end ();
      i++) {
        gLogIOstream <<
          "\"" << ((*i).first) << " = " << ((*i).second) << "\" ";
    } // for
  }

  gLogIOstream <<
    endl;

  gIndenter--;

  // voices
  // --------------------------------------
  
  gLogIOstream <<
    "Voices:" <<
    endl;

  gIndenter++;

  gLogIOstream << left <<
    setw (fieldWidth) <<
    "createVoicesStaffRelativeNumbers" << " : " <<
    booleanAsString (fCreateVoicesStaffRelativeNumbers) <<
    endl;

  gIndenter--;

  // notes
  // --------------------------------------
  
  gLogIOstream <<
     "Notes:" <<
    endl;

  gIndenter++;

  gLogIOstream << left << 
    setw (fieldWidth) << "delayRestsDynamics" << " : " <<
    booleanAsString (fDelayRestsDynamics) <<
    endl <<
    
    setw (fieldWidth) << "delayRestsWords" << " : " <<
    booleanAsString (fDelayRestsWords) <<
    endl <<
    
    setw (fieldWidth) << "delayRestsSlurs" << " : " <<
    booleanAsString (fDelayRestsSlurs) <<
    endl <<
    
    setw (fieldWidth) << "delayRestsLigatures" << " : " <<
    booleanAsString (fDelayRestsLigatures) <<
    endl <<
    
    setw (fieldWidth) << "delayRestsWedges" << " : " <<
    booleanAsString (fDelayRestsWedges) <<
    endl;

  gIndenter--;
   
  // lyrics
  // --------------------------------------
  
  gLogIOstream <<
    "Lyrics:" <<
    endl;

  gIndenter++;

  gLogIOstream <<
    setw (fieldWidth) << "showMsrStanzas" << " : " <<
    booleanAsString (fShowMsrStanzas) <<
    endl;

  gIndenter--;

  // harmonies
  // --------------------------------------
  
  gLogIOstream <<
    "Harmonies:" <<
    endl;

  gIndenter++;    

  gLogIOstream << left <<
    setw (fieldWidth) << "showHarmonyVoices" << " : " <<
    booleanAsString (fShowHarmonyVoices) <<
    endl;
  
  gIndenter--;
  
  // figured bass
  // --------------------------------------
  
  gLogIOstream <<
    "Figured bass:" <<
    endl;

  gIndenter++;    

  gLogIOstream << left <<
    setw (fieldWidth) << "showFiguredBassVoices" << " : " <<
    booleanAsString (fShowFiguredBassVoices) <<
    endl;
  
  gIndenter--;
  
  gIndenter--;
}

ostream& operator<< (ostream& os, const S_msrOptions& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
void initializeMsrOptionsHandling (
  S_msrOptionsHandler optionsHandler)
{
  // MSR options
  // ------------------------------------------------------
 
  gMsrOptionsUserChoices = msrOptions::create (
    optionsHandler);
  assert(gMsrOptionsUserChoices != 0);

  gMsrOptions =
    gMsrOptionsUserChoices;

  // prepare for measure detailed trace
  // ------------------------------------------------------

  gMsrOptionsWithDetailedTrace =
    gMsrOptions->
      createCloneWithDetailedTrace ();
}


}
