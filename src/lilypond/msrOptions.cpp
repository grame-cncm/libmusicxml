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

// useful shortcut macros
#define idtr indenter::gIndenter
#define tab  indenter::gIndenter.getSpacer ()

//_______________________________________________________________________________
S_msrOptions gMsrOptions;
S_msrOptions gMsrOptionsUserChoices;
S_msrOptions gMsrOptionsWithDetailedTrace;

S_msrOptions msrOptions::create ()
{
  msrOptions* o = new msrOptions();
  assert(o!=0);
  return o;
}

msrOptions::msrOptions ()
  : msrOptionsGroup (
    "hmsr", "helpMSR",
R"(MSR)"
    )
{
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
  
    fDisplayMsr        = boolOptionsInitialValue;
  
    fDisplayMsrSummary = boolOptionsInitialValue;
  
    // options
  
    S_msrOptionsSubGroup traceAndDisplaySubGroup =
      msrOptionsSubGroup::create (
      "hmsrtd", "helpMsrTraceAndDisplay",
R"(Trace and display)"
      );
  
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
          "tmvisits", "traceMsrVisitors",
R"(Write a trace of the MSR graphs visiting activity to standard error.)",
          "traceMsrVisitors",
          fTraceMsrVisitors));
          
    traceAndDisplaySubGroup->
      appendOptionsItem (
        msrOptionsBooleanItem::create (
          "msr", "displayMsr",
R"(Write the contents of the MSR data to standard error.)",
          "displayMsr",
          fDisplayMsr));
          
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
        gQuarterTonesPitchesLanguagesMap.size () <<
        " known MSR pitches languages are:" <<
        endl;
  
      idtr++;
    
      s <<
        existingQuarterTonesPitchesLanguages ();
  
      idtr--;
  
      optionError (s.str());
    }
    
    // options
  
    S_msrOptionsSubGroup languagesSubGroup =
      msrOptionsSubGroup::create (
      "hmsrlang", "helpMsrLanguages",
R"(Languages)"
      );
  
    appendOptionsSubGroup (languagesSubGroup);
      /* JMI
    languagesSubGroup->
      appendOptionsItem (
        msrOptionsBooleanItem::create (
          "mpl", "msrPitchesLanguage", // JMI
R"(Use 'language' to display note pitches in the MSR logs and text views.
The 12 LilyPond pitches languages are available:
nederlands, catalan, deutsch, english, espanol, français, 
italiano, norsk, portugues, suomi, svenska and vlaams.
The default is to use 'nederlands'.)",
          "msrPitchesLanguage",
          fMsrQuarterTonesPitchesLanguage));
          */
  }

  
  // parts
  // --------------------------------------

  {
    // variables
    
    // options
  
    S_msrOptionsSubGroup partsSubGroup =
      msrOptionsSubGroup::create (
      "hmsrp", "helpMsrParts",
R"(Parts)"
      );
  
    appendOptionsSubGroup (partsSubGroup);
/* JMI
    partsSubGroup->
      appendOptionsItem (
        msrOptionsBooleanItem::create (
          "mpr", "msrPartRename", // JMI
R"(Rename part 'original' to 'newName', for example after 
displaying a summary of the score in a first xml2lilypond run.
There can be several occurrences of this option.)",
          "partRename",
          fPartsRenaming));
          */
  }
      

  // voices
  // --------------------------------------

  {
    // variables
    
    fCreateVoicesStaffRelativeNumbers = boolOptionsInitialValue;
    
    fShowSilentVoices = boolOptionsInitialValue;
    fKeepSilentVoices = boolOptionsInitialValue;
  
    // options
  
    S_msrOptionsSubGroup voicesSubGroup =
      msrOptionsSubGroup::create (
      "hmsrv", "helpMsrVoices",
R"(Voices)"
      );
  
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

    voicesSubGroup->
      appendOptionsItem (
        msrOptionsBooleanItem::create (
          "ssv", "showSilentVoices",
R"(Show the staves silent voices used internally even when they are empty.)",
          "showSilentVoices",
          fShowSilentVoices));

    voicesSubGroup->
      appendOptionsItem (
        msrOptionsBooleanItem::create (
          "ksv", "keepSilentVoices",
R"(Keep the silent voices used internally. By default, there are removed after usage.)",
          "keepSilentVoices",
          fKeepSilentVoices));
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
      "hmsrn", "helpMsrNotes",
R"(Notes)"
      );
  
    appendOptionsSubGroup (notesSubGroup);
  }

  // lyrics
  // --------------------------------------

  {
    // variables
    
    fShowMsrStanzas  = boolOptionsInitialValue;
    fKeepMuteStanzas = boolOptionsInitialValue;
  
    // options
  
    S_msrOptionsSubGroup lyricsSubGroup =
      msrOptionsSubGroup::create (
      "hmsrlyrd", "helpMsrLyrics",
R"(Lyrics)"
      );
  
    appendOptionsSubGroup (lyricsSubGroup);
  }     

  // harmonies
  // --------------------------------------

  {
    // variables
    
    fShowHarmonyVoices      = boolOptionsInitialValue;  
    fKeepEmptyHarmonyVoices = boolOptionsInitialValue;
  
    // options
  
    S_msrOptionsSubGroup harmoniesSubGroup =
      msrOptionsSubGroup::create (
      "hmsrh", "helpMsrHarmonies",
R"(Harmonies)"
      );
  
    appendOptionsSubGroup (harmoniesSubGroup);
  }

  // figured bass
  // --------------------------------------

  {
    // variables  
    
    fShowFiguredBassVoices      = boolOptionsInitialValue;
    fKeepEmptyFiguredBassVoices = boolOptionsInitialValue;
  
    // options
  
    S_msrOptionsSubGroup figuredBassSubGroup =
      msrOptionsSubGroup::create (
      "hmsrfb", "helpMsrFiguredBass",
  R"(Figured bass)"
      );
  
    appendOptionsSubGroup (figuredBassSubGroup);

  }
}

S_msrOptions msrOptions::createCloneWithDetailedTrace ()
{
  S_msrOptions
    clone =
      msrOptions::create ();

  // trace and display
  // --------------------------------------

  clone->fTraceMsr =
    true;

  clone->fTraceMsrVisitors =
    true;

  clone->fDisplayMsr =
    true;

  clone->fDisplayMsrSummary =
    fDisplayMsrSummary;

  // languages
    
  clone->fMsrQuarterTonesPitchesLanguage =
    fMsrQuarterTonesPitchesLanguage;

  // parts

  clone->fPartsRenaming =
    fPartsRenaming;

  // voices
  
  clone->fCreateVoicesStaffRelativeNumbers =
    fCreateVoicesStaffRelativeNumbers;

  clone->fShowSilentVoices =
    fShowSilentVoices;
  clone->fKeepSilentVoices =
    fKeepSilentVoices;

  // notes

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
    
  clone->fShowMsrStanzas =
    fShowMsrStanzas;

  clone->fKeepMuteStanzas =
    fKeepMuteStanzas;

  // harmonies

  clone->fShowHarmonyVoices =
    fShowHarmonyVoices;
  clone->fKeepEmptyHarmonyVoices =
    fKeepEmptyHarmonyVoices;


  // figured bass
  
  clone->fShowFiguredBassVoices =
    fShowFiguredBassVoices;
  clone->fKeepEmptyFiguredBassVoices =
    fKeepEmptyFiguredBassVoices;

  return clone;
}  

bool msrOptions::setMsrQuarterTonesPitchesLanguage (string language)
{
  // is language in the pitches languages map?
  map<string, msrQuarterTonesPitchesLanguage>::const_iterator
    it =
      gQuarterTonesPitchesLanguagesMap.find (language);
        
  if (it == gQuarterTonesPitchesLanguagesMap.end ()) {
    // no, language is unknown in the map
    return false;
  }

  fMsrQuarterTonesPitchesLanguage = (*it).second;
  
  return true;
}

void msrOptions::printMsrOptionsHelp ()
{
  idtr++;

  cerr <<
    idtr << "MSR:" <<
    endl <<
    idtr << "---" <<
    endl <<
    endl;

  idtr++;

  // trace and display
  // --------------------------------------
  cerr <<
    idtr << "Trace and display:" <<
    endl <<
    endl;

  idtr++;

  cerr <<
    idtr <<
      "--" _TRACE_MSR_SHORT_NAME_ ", --" _TRACE_MSR_LONG_NAME_ <<
      endl <<
    idtr << tab << tab << tab <<
      "Write a trace of the LPSR graphs visiting activity to standard error." <<
      endl <<
    endl <<
    
    idtr <<
      "--" _TRACE_MSR_VISITORS_SHORT_NAME_ ", --" _TRACE_MSR_VISITORS_LONG_NAME_ <<
      endl <<
    idtr << tab << tab << tab <<
      "Write a trace of the MSR graphs visiting activity to standard error." <<
      endl <<
    endl <<

    idtr <<
      "--" _DISPLAY_MSR_SHORT_NAME_ ", --" _DISPLAY_MSR_LONG_NAME_ <<
      endl <<
    idtr << tab << tab << tab <<
      "Write the contents of the MSR data to standard error." <<
      endl <<
    endl <<

    idtr <<
      "--" _DISPLAY_MSR_SUMMARY_SHORT_NAME_ ", --" _DISPLAY_MSR_SUMMARY_LONG_NAME_ <<
      endl <<
    idtr << tab << tab << tab <<
      "Only write a summary of the MSR to standard error." <<
      endl <<
    idtr << tab << tab << tab <<
      "This implies that no LilyPond code is generated." <<
      endl <<
    endl;
   
  idtr--;

  // languages
  // --------------------------------------
  cerr <<
    idtr << "Languages:" <<
    endl <<
    endl;

  idtr++;

  cerr <<
    idtr <<
      "--" _MSR_PITCHES_LANGUAGE_SHORT_NAME_ ", --" _MSR_PITCHES_LANGUAGE_LONG_NAME_ " language" <<
      endl <<
    idtr << tab << tab << tab <<
      "Use 'language' to display note pitches in the MSR logs and text views." <<
      endl <<
    idtr << tab << tab << tab <<
      "The 12 LilyPond pitches languages are available:" <<
      endl <<
    idtr << tab << tab << tab <<
      "nederlands, catalan, deutsch, english, espanol, français, " <<
      endl <<
    idtr << tab << tab << tab <<
      "italiano, norsk, portugues, suomi, svenska and vlaams." <<
      endl <<
    idtr << tab << tab << tab <<
      "The default is to use 'nederlands'." <<
      endl <<
    endl;

  idtr--;

  // parts
  // --------------------------------------
  cerr <<
    idtr << "Parts:" <<
    endl <<
    endl;

  idtr++;

  cerr <<
    idtr <<
      "--" _MSR_PITCHES_LANGUAGE_SHORT_NAME_ ", --" _MSR_PITCHES_LANGUAGE_LONG_NAME_ " 'originalName = newName'" <<
      endl <<
    idtr <<
      "--" _MSR_PITCHES_LANGUAGE_SHORT_NAME_ ", --" _MSR_PITCHES_LANGUAGE_LONG_NAME_ " \"originalName = newName\"" <<
      endl <<
    idtr << tab << tab << tab <<
      "Rename part 'original' to 'newName', for example after " <<
      endl <<
    idtr << tab << tab << tab <<
      "displaying a summary of the score in a first xml2lilypond run." <<
      endl <<
    idtr << tab << tab << tab <<
      "There can be several occurrences of this option." <<
      endl <<
    endl;

  idtr--;

  // voices
  // --------------------------------------
  cerr <<
    idtr << "Voices:" <<
    endl <<
    endl;

  idtr++;

  cerr <<
    idtr <<
      "--" _STAFF_RELATIVE_VOICE_NUMBERS_SHORT_NAME_ ", --" _STAFF_RELATIVE_VOICE_NUMBERS_LONG_NAME_ <<
      endl <<
    idtr << tab << tab << tab <<
      "Generate voices names with numbers relative to their staff." <<
      endl <<
    idtr << tab << tab << tab <<
      "By default, the voice numbers found are used, " <<
      endl <<
    idtr << tab << tab << tab <<
      "which may be global to the score." <<
      endl <<
    endl <<
    
    idtr <<
      "--" _SHOW_SILENT_VOICES_SHORT_NAME_ ", --" _SHOW_SILENT_VOICES_LONG_NAME_ <<
      endl <<
    idtr << tab << tab << tab <<
      "Show the staves silent voices used internally event though the're empty" <<
      endl <<
    endl <<
    
    idtr <<
      "--" _KEEP_SILENT_VOICES_SHORT_NAME_ ", --" _KEEP_SILENT_VOICES_LONG_NAME_ <<
      endl <<
    idtr << tab << tab << tab <<
      "Keep the silent voices used internally. By default, there are removed after usage." <<
      endl <<
    endl;

  idtr--;

  // notes
  // --------------------------------------
  cerr <<
    idtr << "Notes:" <<
    endl <<
    endl;

  idtr++;

  cerr <<    
    idtr <<
      "--" _DELAY_RESTS_DYNAMICS_SHORT_NAME_ ", --" _DELAY_RESTS_DYNAMICS_LONG_NAME_ <<
      endl <<
    idtr << tab << tab << tab <<
      "dynamics" <<
      endl <<
    idtr <<
      "--" _DELAY_RESTS_WORDS_SHORT_NAME_ ", --" _DELAY_RESTS_WORDS_LONG_NAME_ <<
      endl <<
    idtr << tab << tab << tab <<
      "words" <<
      endl <<
    idtr <<
      "--" _DELAY_RESTS_SLURS_SHORT_NAME_ ", --" _DELAY_RESTS_SLURS_LONG_NAME_ <<
      endl <<
    idtr << tab << tab << tab <<
      "slurs" <<
      endl <<
    idtr <<
      "--" _DELAY_RESTS_LIGATURES_SHORT_NAME_ ", --" _DELAY_RESTS_LIGATURES_LONG_NAME_ <<
      endl <<
    idtr << tab << tab << tab <<
      "<bracket/> in MusicXML, '\\[... \\}' in LilyPond" <<
      endl <<
    idtr <<
      "--" _DELAY_RESTS_WEDGES_SHORT_NAME_ ", --" _DELAY_RESTS_WEDGES_LONG_NAME_ <<
      endl <<
    idtr << tab << tab << tab <<
      "'<wedge/>' in MusicXML, '<!' in LilyPond" <<
      endl <<
    endl;

   idtr--;
   
  // lyrics
  // --------------------------------------
  cerr <<
    idtr << "Lyrics:" <<
    endl <<
    endl;

  idtr++;

  cerr <<
    idtr <<
      "--" _SHOW_MSR_STANZAS_SHORT_NAME_ ", --" _SHOW_MSR_STANZAS_LONG_NAME_ <<
      endl <<
    idtr << tab << tab << tab <<
      "Show MSR stanzas even though they're empty'." <<
      endl <<
    endl <<

    idtr <<
      "--" _KEEP_MUTE_STANZAS_SHORT_NAME_ ", --" _KEEP_MUTE_STANZAS_LONG_NAME_ <<
      endl <<
    idtr << tab << tab << tab <<
      "Keep the mute stanzas used intertally. By default, there are removed after usage." <<
      endl <<
    endl;

  idtr--;

  // harmonies
  // --------------------------------------
  cerr <<
    idtr << "Harmonies:" <<
    endl <<
    endl;

  idtr++;

  cerr <<
    idtr <<
      "--" _SHOW_HARMONY_VOICES_SHORT_NAME_ ", --" _SHOW_HARMONY_VOICES_LONG_NAME_ <<
      endl <<
    idtr << tab << tab << tab <<
      "Show the parts harmony voices in the MSR data even though is does not contain music." <<
      endl <<
    endl <<
    
    idtr <<
      "--" _KEEP_EMPTY_HARMONIES_VOICE_SHORT_NAME_ ", --" _KEEP_EMPTY_HARMONIES_VOICE_LONG_NAME_ <<
      endl <<
    idtr << tab << tab << tab <<
      "Keep the harmonies voice in the MSR data even though is does not contain music." <<
      endl <<
    idtr << tab << tab << tab <<
      "It is thrown away in such a case by default." <<
      endl <<
    endl;

  idtr--;
  
  // figured bass
  // --------------------------------------
  cerr <<
    idtr << "Figured bass:" <<
    endl <<
    endl;

  idtr++;

  cerr <<
    idtr <<
      "--" _SHOW_FIGURED_BASS_VOICES_SHORT_NAME_ ", --" _SHOW_FIGURED_BASS_VOICES_LONG_NAME_ <<
      endl <<
    idtr << tab << tab << tab <<
      "Show the parts harmony voices in the MSR data even though is does not contain music." <<
      endl <<
    endl <<
    
    idtr <<
      "--" _KEEP_EMPTY_HARMONIES_VOICE_SHORT_NAME_ ", --" _KEEP_EMPTY_HARMONIES_VOICE_LONG_NAME_ <<
      endl <<
    idtr << tab << tab << tab <<
      "Keep the harmonies voice in the MSR data even though is does not contain music." <<
      endl <<
    idtr << tab << tab << tab <<
      "It is thrown away in such a case by default." <<
      endl <<
    endl;

  idtr--;
  
  idtr--;

  idtr--; 
}

void msrOptions::printMsrOptionsValues (int fieldWidth)
{
  cerr << idtr <<
    "The MSR options are:" <<
    endl;

  idtr++;
  
  // trace and display
  // --------------------------------------
  
  cerr <<
    idtr << "Trace and display:" <<
    endl;

  idtr++;
  
  cerr << left <<
    idtr <<
      setw(fieldWidth) << "traceMsr" << " : " <<
      booleanAsString (fTraceMsr) <<
      endl <<
    
    idtr <<
      setw(fieldWidth) << "traceMsrVisitors" << " : " <<
      booleanAsString (fTraceMsrVisitors) <<
      endl <<

    idtr << setw(fieldWidth) << "displayMsr" << " : " <<
      booleanAsString (fDisplayMsr) <<
      endl <<

    idtr << setw(fieldWidth) << "displayMsrSummary" << " : " <<
      booleanAsString (fDisplayMsrSummary) <<
      endl;    

  idtr--;

  // languages
  // --------------------------------------
  
  cerr <<
    idtr << "Languages:" <<
    endl;

  idtr++;

  cerr <<
    idtr << setw(fieldWidth) << "msrPitchesLanguage" << " : \"" <<
      msrQuarterTonesPitchesLanguageAsString (
        fMsrQuarterTonesPitchesLanguage) <<
        "\"" <<
      endl;

  idtr--;

  // parts
  // --------------------------------------
  
  cerr <<
    idtr << "Parts:" <<
    endl;

  idtr++;

  cerr <<
    idtr << setw(fieldWidth) << "parts renaming" << " : ";

  if (fPartsRenaming.empty ()) {
    cerr <<
      "none";
  }
  
  else {
    for (
      map<string, string>::const_iterator i =
        fPartsRenaming.begin();
      i != fPartsRenaming.end();
      i++) {
        cerr <<
          "\"" << ((*i).first) << " = " << ((*i).second) << "\" ";
    } // for
  }

  cerr <<
    endl;

  idtr--;

  // voices
  // --------------------------------------
  
  cerr <<
    idtr << "Voices:" <<
    endl;

  idtr++;

  cerr <<
    idtr << setw(fieldWidth) <<
      "createVoicesStaffRelativeNumbers" << " : " <<
      booleanAsString (fCreateVoicesStaffRelativeNumbers) <<
      endl <<
    
    idtr << setw(fieldWidth) <<
    "showSilentVoices" << " : " <<
      booleanAsString (fShowSilentVoices) <<
      endl <<
    idtr << setw(fieldWidth) <<
      "keepSilentVoices" << " : " <<
      booleanAsString (fKeepSilentVoices) <<
      endl;

  idtr--;

  // notes
  // --------------------------------------
  
  cerr <<
    idtr << "Notes:" <<
    endl;

  idtr++;

  cerr <<    
    idtr << setw(fieldWidth) << "delayRestsDynamics" << " : " <<
      booleanAsString (fDelayRestsDynamics) <<
      endl <<
      
    idtr << setw(fieldWidth) << "delayRestsWords" << " : " <<
      booleanAsString (fDelayRestsWords) <<
      endl <<
      
    idtr << setw(fieldWidth) << "delayRestsSlurs" << " : " <<
      booleanAsString (fDelayRestsSlurs) <<
      endl <<
      
    idtr << setw(fieldWidth) << "delayRestsLigatures" << " : " <<
      booleanAsString (fDelayRestsLigatures) <<
      endl <<
      
    idtr << setw(fieldWidth) << "delayRestsWedges" << " : " <<
      booleanAsString (fDelayRestsWedges) <<
      endl;

  idtr--;
   
  // lyrics
  // --------------------------------------
  
  cerr <<
    idtr << "Lyrics:" <<
    endl;

  idtr++;

  cerr <<
    idtr << setw(fieldWidth) << "showMsrStanzas" << " : " <<
      booleanAsString (fShowMsrStanzas) <<
      endl <<

    idtr << setw(fieldWidth) << "keepMuteStanzas" << " : " <<
      booleanAsString (fKeepMuteStanzas) <<
      endl;

  idtr--;

  // harmonies
  // --------------------------------------
  
  cerr <<
    idtr << "Harmonies:" <<
    endl;

  idtr++;    

  cerr <<
    idtr << setw(fieldWidth) << "showHarmonyVoices" << " : " <<
      booleanAsString (fShowHarmonyVoices) <<
      endl <<
    idtr << setw(fieldWidth) << "keepEmptyHarmonyVoices" << " : " <<
      booleanAsString (fKeepEmptyHarmonyVoices) <<
      endl;
  
  idtr--;
  
  // figured bass
  // --------------------------------------
  
  cerr <<
    idtr << "Figured bass:" <<
    endl;

  idtr++;    

  cerr <<
    idtr << setw(fieldWidth) << "showFiguredBassVoices" << " : " <<
      booleanAsString (fShowFiguredBassVoices) <<
      endl <<
    idtr << setw(fieldWidth) << "keepEmptyFiguredBassVoices" << " : " <<
      booleanAsString (fKeepEmptyFiguredBassVoices) <<
      endl;
  
  idtr--;
  
  idtr--;
}


}
