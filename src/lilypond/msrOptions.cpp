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

S_msrOptions msrOptions::create ()
{
  msrOptions* o = new msrOptions();
  assert(o!=0);
  return o;
}

msrOptions::msrOptions ()
  : msrOptionsGroup (
    "MSR",
    "hmsr", "helpMSR",
R"(These options control the way MSR data is handled.)"
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
        "Trace and display",
        "hmsrtd", "helpMsrTraceAndDisplay",
R"()",
      msrOptionsSubGroup::kAlwaysShowDescription
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
          "tmsrv", "traceMsrVisitors",
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
  
      gIdtr++;
    
      s <<
        existingQuarterTonesPitchesLanguages ();
  
      gIdtr--;
  
      optionError (s.str ());
    }
    
    // options
  
    S_msrOptionsSubGroup languagesSubGroup =
      msrOptionsSubGroup::create (
        "Languages",
        "hmsrlang", "helpMsrLanguages",
R"()",
      msrOptionsSubGroup::kAlwaysShowDescription
      );
  
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
          fMsrQuarterTonesPitchesLanguage));
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
      msrOptionsSubGroup::kAlwaysShowDescription
      );
  
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
    
    fShowSilentVoices = boolOptionsInitialValue;
    fKeepSilentVoices = boolOptionsInitialValue;
  
    // options
  
    S_msrOptionsSubGroup voicesSubGroup =
      msrOptionsSubGroup::create (
        "Voices",
        "hmsrv", "helpMsrVoices",
R"()",
      msrOptionsSubGroup::kAlwaysShowDescription
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
R"(Show the staves silent voices used internally
even when they are empty.)",
          "showSilentVoices",
          fShowSilentVoices));

    voicesSubGroup->
      appendOptionsItem (
        msrOptionsBooleanItem::create (
          "ksv", "keepSilentVoices",
R"(Keep the silent voices used internally.
By default, there are removed after usage.)",
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
        "Notes",
        "hmsrn", "helpMsrNotes",
R"()",
      msrOptionsSubGroup::kAlwaysShowDescription
      );
  
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
    fKeepMuteStanzas = boolOptionsInitialValue;
  
    // options
  
    S_msrOptionsSubGroup lyricsSubGroup =
      msrOptionsSubGroup::create (
        "Lyrics",
        "hmsrlyrd", "helpMsrLyrics",
R"()",
      msrOptionsSubGroup::kAlwaysShowDescription
      );
  
    appendOptionsSubGroup (lyricsSubGroup);

    lyricsSubGroup->
      appendOptionsItem (
        msrOptionsBooleanItem::create (
          "sms", "showMsrStanzas",
R"(Show MSR stanzas even when they're empty.)",
          "showMsrStanzas",
          fShowMsrStanzas));

    lyricsSubGroup->
      appendOptionsItem (
        msrOptionsBooleanItem::create (
          "kms", "keepMuteStanzas",
R"(Keep the mute stanzas used intertally.
By default, there are removed after usage.)",
          "keepMuteStanzas",
          fKeepMuteStanzas));
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
        "Harmonies",
        "hmsrh", "helpMsrHarmonies",
R"()",
      msrOptionsSubGroup::kAlwaysShowDescription
      );
  
    appendOptionsSubGroup (harmoniesSubGroup);

    harmoniesSubGroup->
      appendOptionsItem (
        msrOptionsBooleanItem::create (
          "shv", "showHarmonyVoices",
R"(Show the parts harmony voices in the MSR data
even though it does not contain music.)",
          "showHarmonyVoices",
          fShowHarmonyVoices));

    harmoniesSubGroup->
      appendOptionsItem (
        msrOptionsBooleanItem::create (
          "kehv", "keepEmptyHarmonyVoices",
R"(Keep the harmonies voice in the MSR data
even though it does not contain music.
It is thrown away in such a case by default.)",
          "keepEmptyHarmonyVoices",
          fKeepEmptyHarmonyVoices));
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
        "Figured bass",
        "hmsrfb", "helpMsrFiguredBass",
  R"()",
      msrOptionsSubGroup::kAlwaysShowDescription
      );
  
    appendOptionsSubGroup (figuredBassSubGroup);

    figuredBassSubGroup->
      appendOptionsItem (
        msrOptionsBooleanItem::create (
          "sfbv", "showFiguredBassVoices",
R"(Show the figured bass harmony voices in the MSR data
even though they do not contain music.)",
          "showFiguredBassVoices",
          fShowFiguredBassVoices));

    figuredBassSubGroup->
      appendOptionsItem (
        msrOptionsBooleanItem::create (
          "kefbv", "keepEmptyFiguredBassVoices",
R"(Keep the figured bass voices in the MSR data
even though they do not contain music.
They are thrown away in such a case by default.)",
          "keepEmptyFiguredBassVoices",
          fKeepEmptyFiguredBassVoices));
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

  clone->fPartsRenamingMap =
    fPartsRenamingMap;

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

void msrOptions::printMsrOptionsValues (int fieldWidth)
{
  cerr << gIdtr <<
    "The MSR options are:" <<
    endl;

  gIdtr++;
  
  // trace and display
  // --------------------------------------
  
  cerr <<
    gIdtr << "Trace and display:" <<
    endl;

  gIdtr++;
  
  cerr << left <<
    gIdtr <<
      setw (fieldWidth) << "traceMsr" << " : " <<
      booleanAsString (fTraceMsr) <<
      endl <<
    
    gIdtr <<
      setw (fieldWidth) << "traceMsrVisitors" << " : " <<
      booleanAsString (fTraceMsrVisitors) <<
      endl <<

    gIdtr << setw (fieldWidth) << "displayMsr" << " : " <<
      booleanAsString (fDisplayMsr) <<
      endl <<

    gIdtr << setw (fieldWidth) << "displayMsrSummary" << " : " <<
      booleanAsString (fDisplayMsrSummary) <<
      endl;    

  gIdtr--;

  // languages
  // --------------------------------------
  
  cerr <<
    gIdtr << "Languages:" <<
    endl;

  gIdtr++;

  cerr <<
    gIdtr << setw (fieldWidth) << "msrPitchesLanguage" << " : \"" <<
      msrQuarterTonesPitchesLanguageAsString (
        fMsrQuarterTonesPitchesLanguage) <<
        "\"" <<
      endl;

  gIdtr--;

  // parts
  // --------------------------------------
  
  cerr <<
    gIdtr << "Parts:" <<
    endl;

  gIdtr++;

  cerr <<
    gIdtr << setw (fieldWidth) << "parts renaming" << " : ";

  if (fPartsRenamingMap.empty ()) {
    cerr <<
      "none";
  }
  
  else {
    for (
      map<string, string>::const_iterator i =
        fPartsRenamingMap.begin();
      i != fPartsRenamingMap.end();
      i++) {
        cerr <<
          "\"" << ((*i).first) << " = " << ((*i).second) << "\" ";
    } // for
  }

  cerr <<
    endl;

  gIdtr--;

  // voices
  // --------------------------------------
  
  cerr <<
    gIdtr << "Voices:" <<
    endl;

  gIdtr++;

  cerr <<
    gIdtr << setw (fieldWidth) <<
      "createVoicesStaffRelativeNumbers" << " : " <<
      booleanAsString (fCreateVoicesStaffRelativeNumbers) <<
      endl <<
    
    gIdtr << setw (fieldWidth) <<
    "showSilentVoices" << " : " <<
      booleanAsString (fShowSilentVoices) <<
      endl <<
    gIdtr << setw (fieldWidth) <<
      "keepSilentVoices" << " : " <<
      booleanAsString (fKeepSilentVoices) <<
      endl;

  gIdtr--;

  // notes
  // --------------------------------------
  
  cerr <<
    gIdtr << "Notes:" <<
    endl;

  gIdtr++;

  cerr <<    
    gIdtr << setw (fieldWidth) << "delayRestsDynamics" << " : " <<
      booleanAsString (fDelayRestsDynamics) <<
      endl <<
      
    gIdtr << setw (fieldWidth) << "delayRestsWords" << " : " <<
      booleanAsString (fDelayRestsWords) <<
      endl <<
      
    gIdtr << setw (fieldWidth) << "delayRestsSlurs" << " : " <<
      booleanAsString (fDelayRestsSlurs) <<
      endl <<
      
    gIdtr << setw (fieldWidth) << "delayRestsLigatures" << " : " <<
      booleanAsString (fDelayRestsLigatures) <<
      endl <<
      
    gIdtr << setw (fieldWidth) << "delayRestsWedges" << " : " <<
      booleanAsString (fDelayRestsWedges) <<
      endl;

  gIdtr--;
   
  // lyrics
  // --------------------------------------
  
  cerr <<
    gIdtr << "Lyrics:" <<
    endl;

  gIdtr++;

  cerr <<
    gIdtr << setw (fieldWidth) << "showMsrStanzas" << " : " <<
      booleanAsString (fShowMsrStanzas) <<
      endl <<

    gIdtr << setw (fieldWidth) << "keepMuteStanzas" << " : " <<
      booleanAsString (fKeepMuteStanzas) <<
      endl;

  gIdtr--;

  // harmonies
  // --------------------------------------
  
  cerr <<
    gIdtr << "Harmonies:" <<
    endl;

  gIdtr++;    

  cerr <<
    gIdtr << setw (fieldWidth) << "showHarmonyVoices" << " : " <<
      booleanAsString (fShowHarmonyVoices) <<
      endl <<
    gIdtr << setw (fieldWidth) << "keepEmptyHarmonyVoices" << " : " <<
      booleanAsString (fKeepEmptyHarmonyVoices) <<
      endl;
  
  gIdtr--;
  
  // figured bass
  // --------------------------------------
  
  cerr <<
    gIdtr << "Figured bass:" <<
    endl;

  gIdtr++;    

  cerr <<
    gIdtr << setw (fieldWidth) << "showFiguredBassVoices" << " : " <<
      booleanAsString (fShowFiguredBassVoices) <<
      endl <<
    gIdtr << setw (fieldWidth) << "keepEmptyFiguredBassVoices" << " : " <<
      booleanAsString (fKeepEmptyFiguredBassVoices) <<
      endl;
  
  gIdtr--;
  
  gIdtr--;
}

ostream& operator<< (ostream& os, const S_msrOptions& elt)
{
  elt->print (os);
  return os;
}


}
