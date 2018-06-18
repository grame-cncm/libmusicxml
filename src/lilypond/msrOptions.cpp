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
#include <iomanip>      // setw, setprecision, ...
#include <sstream>

#include <regex>

#include "utilities.h"

#include "traceOptions.h"

#include "msrOptions.h"


using namespace std;

namespace MusicXML2 
{

//______________________________________________________________________________
S_optionsPartRenameItem optionsPartRenameItem::create (
  string             optionsItemShortName,
  string             optionsItemLongName,
  string             optionsItemDescription,
  string             optionsValueSpecification,
  string             optionsPartRenameItemVariableDisplayName,
  map<string, string>&
                     optionsPartRenameItemVariable)
{
  optionsPartRenameItem* o = new
    optionsPartRenameItem (
      optionsItemShortName,
      optionsItemLongName,
      optionsItemDescription,
      optionsValueSpecification,
      optionsPartRenameItemVariableDisplayName,
      optionsPartRenameItemVariable);
  assert(o!=0);
  return o;
}

optionsPartRenameItem::optionsPartRenameItem (
  string             optionsItemShortName,
  string             optionsItemLongName,
  string             optionsItemDescription,
  string             optionsValueSpecification,
  string             optionsPartRenameItemVariableDisplayName,
  map<string, string>&
                     optionsPartRenameItemVariable)
  : optionsValuedItem (
      optionsItemShortName,
      optionsItemLongName,
      optionsItemDescription,
      optionsValueSpecification),
    fOptionsPartRenameItemVariableDisplayName (
      optionsPartRenameItemVariableDisplayName),
    fOptionsPartRenameItemVariable (
      optionsPartRenameItemVariable)
{}

optionsPartRenameItem::~optionsPartRenameItem ()
{}

void optionsPartRenameItem::print (ostream& os) const
{
  const int fieldWidth = K_FIELD_WIDTH;
  
  os <<
    "OptionsPartRenameItem:" <<
    endl;

  gIndenter++;

  printValuedItemEssentials (
    os, fieldWidth);

  os << left <<
    setw (fieldWidth) <<
    "fOptionsPartRenameItemVariableDisplayName" << " : " <<
    fOptionsPartRenameItemVariableDisplayName <<
    setw (fieldWidth) <<
    "fOptionsPartRenameItemVariable" << " : " <<
    endl;

  if (! fOptionsPartRenameItemVariable.size ()) {
    os << "none";
  }
  else {
    map<string, string>::const_iterator
      iBegin = fOptionsPartRenameItemVariable.begin (),
      iEnd   = fOptionsPartRenameItemVariable.end (),
      i      = iBegin;
    for ( ; ; ) {
      os << (*i).first << " --> " << (*i).second;
      if (++i == iEnd) break;
      os << endl;
    } // for
  }
  
  os <<
    endl;
}

void optionsPartRenameItem::printOptionsValues (
  ostream& os,
  int      valueFieldWidth) const
{  
  os << left <<
    setw (valueFieldWidth) <<
    fOptionsPartRenameItemVariableDisplayName <<
    " : ";
      
  if (! fOptionsPartRenameItemVariable.size ()) {
    os << "none";
  }
  else {
    gIndenter++;
    
    os <<
      endl;

    map<string, string>::const_iterator
      iBegin = fOptionsPartRenameItemVariable.begin (),
      iEnd   = fOptionsPartRenameItemVariable.end (),
      i      = iBegin;
    for ( ; ; ) {
      os << left <<
        setw (valueFieldWidth) <<
        " " <<
        " \"" <<
        (*i).first <<
        "\" --> \"" <<
        (*i).second <<
        "\"";
      if (++i == iEnd) break;
      os << endl;
    } // for

    gIndenter--;
  }
  
  os <<
    endl;
}

ostream& operator<< (ostream& os, const S_optionsPartRenameItem& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_optionsMsrPitchesLanguageItem optionsMsrPitchesLanguageItem::create (
  string             optionsItemShortName,
  string             optionsItemLongName,
  string             optionsItemDescription,
  string             optionsValueSpecification,
  string             optionsMsrPitchesLanguageKindItemVariableDisplayName,
  msrQuarterTonesPitchesLanguageKind&
                     optionsMsrPitchesLanguageKindItemVariable)
{
  optionsMsrPitchesLanguageItem* o = new
    optionsMsrPitchesLanguageItem (
      optionsItemShortName,
      optionsItemLongName,
      optionsItemDescription,
      optionsValueSpecification,
      optionsMsrPitchesLanguageKindItemVariableDisplayName,
      optionsMsrPitchesLanguageKindItemVariable);
  assert(o!=0);
  return o;
}

optionsMsrPitchesLanguageItem::optionsMsrPitchesLanguageItem (
  string             optionsItemShortName,
  string             optionsItemLongName,
  string             optionsItemDescription,
  string             optionsValueSpecification,
  string             optionsMsrPitchesLanguageKindItemVariableDisplayName,
  msrQuarterTonesPitchesLanguageKind&
                     optionsMsrPitchesLanguageKindItemVariable)
  : optionsValuedItem (
      optionsItemShortName,
      optionsItemLongName,
      optionsItemDescription,
      optionsValueSpecification),
    fOptionsMsrPitchesLanguageKindItemVariableDisplayName (
      optionsMsrPitchesLanguageKindItemVariableDisplayName),
    fOptionsMsrPitchesLanguageKindItemVariable (
      optionsMsrPitchesLanguageKindItemVariable)
{}

optionsMsrPitchesLanguageItem::~optionsMsrPitchesLanguageItem ()
{}

void optionsMsrPitchesLanguageItem::print (ostream& os) const
{
  const int fieldWidth = K_FIELD_WIDTH;
  
  os <<
    "OptionsMsrPitchesLanguageItem:" <<
    endl;

  gIndenter++;

  printValuedItemEssentials (
    os, fieldWidth);

  os << left <<
    setw (fieldWidth) <<
    "fOptionsMsrPitchesLanguagKindeItemVariableDisplayName" << " : " <<
    fOptionsMsrPitchesLanguageKindItemVariableDisplayName <<
    endl <<
    setw (fieldWidth) <<
    "fOptionsMsrPitchesLanguageItemVariable" << " : \"" <<
    msrQuarterTonesPitchesLanguageKindAsString (
      fOptionsMsrPitchesLanguageKindItemVariable) <<
      "\"" <<
    endl;
}

void optionsMsrPitchesLanguageItem::printOptionsValues (
  ostream& os,
  int      valueFieldWidth) const
{  
  os << left <<
    setw (valueFieldWidth) <<
    fOptionsMsrPitchesLanguageKindItemVariableDisplayName <<
    " : \"" <<
    msrQuarterTonesPitchesLanguageKindAsString (
      fOptionsMsrPitchesLanguageKindItemVariable) <<
    "\"" <<
    endl;
}

ostream& operator<< (ostream& os, const S_optionsMsrPitchesLanguageItem& elt)
{
  elt->print (os);
  return os;
}

//_______________________________________________________________________________
S_msrOptions gMsrOptions;
S_msrOptions gMsrOptionsUserChoices;
S_msrOptions gMsrOptionsWithDetailedTrace;

S_msrOptions msrOptions::create (
  S_optionsHandler optionsHandler)
{
  msrOptions* o = new msrOptions(
    optionsHandler);
  assert(o!=0);
  return o;
}

msrOptions::msrOptions (
  S_optionsHandler optionsHandler)
  : optionsGroup (
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
  
    S_optionsSubGroup traceAndDisplaySubGroup =
      optionsSubGroup::create (
        "Trace and display",
        "hmsrtd", "helpMsrTraceAndDisplay",
R"()",
      optionsSubGroup::kAlwaysShowDescription,
      this);
  
    appendOptionsSubGroup (traceAndDisplaySubGroup);
      
    traceAndDisplaySubGroup->
      appendOptionsItem (
        optionsBooleanItem::create (
          "tmsr", "traceMsr",
R"(Write a trace of the LPSR graphs visiting activity to standard error.)",
          "traceMsr",
          fTraceMsr));
          
    traceAndDisplaySubGroup->
      appendOptionsItem (
        optionsBooleanItem::create (
          "tmsrv", "traceMsrVisitors",
R"(Write a trace of the MSR graphs visiting activity to standard error.)",
          "traceMsrVisitors",
          fTraceMsrVisitors));
          
    traceAndDisplaySubGroup->
      appendOptionsItem (
        optionsBooleanItem::create (
          "dpg", "displayPartGroups",
R"(Write the structure of the part groups to standard error.)",
          "displayPartGroups",
          fDisplayPartGroups));
          
    traceAndDisplaySubGroup->
      appendOptionsItem (
        optionsTwoBooleansItem::create (
          "msr", "displayMsr",
R"(Write the contents of the MSR data to standard error.)",
          "displayMsr",
          fDisplayMsr,
          gTraceOptions->fTracePasses));
          
    traceAndDisplaySubGroup->
      appendOptionsItem (
        optionsBooleanItem::create (
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
  
    S_optionsSubGroup languagesSubGroup =
      optionsSubGroup::create (
        "Languages",
        "hmsrlang", "helpMsrLanguages",
R"()",
      optionsSubGroup::kAlwaysShowDescription,
      this);
  
    appendOptionsSubGroup (languagesSubGroup);

    languagesSubGroup->
      appendOptionsItem (
        optionsMsrPitchesLanguageItem::create (
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
  
    S_optionsSubGroup partsSubGroup =
      optionsSubGroup::create (
        "Parts",
        "hmsrp", "helpMsrParts",
R"()",
      optionsSubGroup::kAlwaysShowDescription,
      this);
  
    appendOptionsSubGroup (partsSubGroup);

    partsSubGroup->
      appendOptionsItem (
        optionsPartRenameItem::create (
          "mpr", "msrPartRename", // JMI
R"(Rename part 'original' to 'newName', for example after 
displaying a summary of the score in a first xml2ly run.
'partRenameSpecification' can be:
  'originalName = newName'
or
  "originalName = newName"
The single or double quotes are used to allow spaces in the names
and around the '=' sign, otherwise they can be dispensed with.
Using double quotes allows for shell variables substitutions, as in:
  DESSUS="Cor anglais"
  xml2ly -msrPartRename "P1 = ${DESSUS}" .
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
  
    S_optionsSubGroup voicesSubGroup =
      optionsSubGroup::create (
        "Voices",
        "hmsrv", "helpMsrVoices",
R"()",
      optionsSubGroup::kAlwaysShowDescription,
      this);
  
    appendOptionsSubGroup (voicesSubGroup);

    voicesSubGroup->
      appendOptionsItem (
        optionsBooleanItem::create (
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
    fDelayRestsPedals    = boolOptionsInitialValue; // JMI
    fDelayRestsSlashes   = boolOptionsInitialValue; // JMI
    fDelayRestsWedges    = boolOptionsInitialValue; // JMI
  
    // options
  
    S_optionsSubGroup notesSubGroup =
      optionsSubGroup::create (
        "Notes",
        "hmsrn", "helpMsrNotes",
R"()",
      optionsSubGroup::kAlwaysShowDescription,
      this);
  
    appendOptionsSubGroup (notesSubGroup);

    notesSubGroup->
      appendOptionsItem (
        optionsBooleanItem::create (
          "drdyns", "delayRestsDynamics",
R"(dynamics)",
          "delayRestsDynamics",
          fDelayRestsDynamics));

    notesSubGroup->
      appendOptionsItem (
        optionsBooleanItem::create (
          "drwords", "delayRestsWords",
R"(words)",
          "delayRestsWords",
          fDelayRestsWords));

    notesSubGroup->
      appendOptionsItem (
        optionsBooleanItem::create (
          "drslurs", "delayRestsSlurs",
R"(slurs)",
          "delayRestsSlurs",
          fDelayRestsSlurs));

    notesSubGroup->
      appendOptionsItem (
        optionsBooleanItem::create (
          "drligs", "delayRestsLigatures",
R"(<bracket/> in MusicXML, '\[... \}' in LilyPond)",
          "delayRestsLigatures",
          fDelayRestsLigatures));

    notesSubGroup->
      appendOptionsItem (
        optionsBooleanItem::create (
          "drpeds", "delayRestsPedals",
R"(pedals)",
          "delayRestsPedals",
          fDelayRestsPedals));

    notesSubGroup->
      appendOptionsItem (
        optionsBooleanItem::create (
          "drslashes", "delayRestsSlashes",
R"('<slash/>' in MusicXML)",
          "delayRestsSlashes",
          fDelayRestsSlashes));

    notesSubGroup->
      appendOptionsItem (
        optionsBooleanItem::create (
          "drwedges", "delayRestsWedges",
R"('<wedge/>' in MusicXML, '<!' in LilyPond)",
          "delayRestsWedges",
          fDelayRestsWedges));
  }


  // lyrics
  // --------------------------------------

  {
    // variables
    
    fAddStanzasNumbers  = false;
  
    // options
  
    S_optionsSubGroup lyricsSubGroup =
      optionsSubGroup::create (
        "Lyrics",
        "hmsrlyrd", "helpMsrLyrics",
R"()",
      optionsSubGroup::kAlwaysShowDescription,
      this);
  
    appendOptionsSubGroup (lyricsSubGroup);

    lyricsSubGroup->
      appendOptionsItem (
        optionsBooleanItem::create (
          "asn", "addStanzasNumbers",
R"(Add stanzas numbers to lyrics.)",
          "addStanzasNumbers",
          fAddStanzasNumbers));
  }     

  // harmonies
  // --------------------------------------

  {
    // variables
    
    fShowHarmonyVoices      = boolOptionsInitialValue;  
  
    // options
  
    S_optionsSubGroup harmoniesSubGroup =
      optionsSubGroup::create (
        "Harmonies",
        "hmsrh", "helpMsrHarmonies",
R"()",
      optionsSubGroup::kAlwaysShowDescription,
      this);
  
    appendOptionsSubGroup (harmoniesSubGroup);

    harmoniesSubGroup->
      appendOptionsItem (
        optionsBooleanItem::create (
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
  
    S_optionsSubGroup figuredBassSubGroup =
      optionsSubGroup::create (
        "Figured bass",
        "hmsrfb", "helpMsrFiguredBass",
R"()",
      optionsSubGroup::kAlwaysShowDescription,
      this);
  
    appendOptionsSubGroup (figuredBassSubGroup);

    figuredBassSubGroup->
      appendOptionsItem (
        optionsBooleanItem::create (
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
      // 0 not to have it inserted twice in the option handler

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
    fDisplayMsr;

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
  clone->fDelayRestsPedals =
    fDelayRestsPedals;
  clone->fDelayRestsSlashes =
    fDelayRestsSlashes;
  clone->fDelayRestsWedges =
    fDelayRestsWedges;


  // lyrics
  // --------------------------------------

  clone->fAddStanzasNumbers =
    fAddStanzasNumbers;

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

  if (! fPartsRenamingMap.size ()) {
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
    
    setw (fieldWidth) << "delayRestsPedals" << " : " <<
    booleanAsString (fDelayRestsPedals) <<
    endl <<
    
    setw (fieldWidth) << "delayRestsSlashes" << " : " <<
    booleanAsString (fDelayRestsSlashes) <<
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
    setw (fieldWidth) << "addStanzasNumbers" << " : " <<
    booleanAsString (fAddStanzasNumbers) <<
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

S_optionsItem msrOptions::handleOptionsItem (
  ostream&      os,
  S_optionsItem item)
{
  S_optionsItem result;
  
  if (
    // part rename item?
    S_optionsPartRenameItem
      partRenameItem =
        dynamic_cast<optionsPartRenameItem*>(&(*item))
    ) {
    if (gTraceOptions->fTraceOptions) {
      os <<
        "==> optionsItem is of type 'optionsPartRenameItem'" <<
        endl;
    }

    // wait until the value is met
    result = partRenameItem;
  }

  else if (
    // pitches language item?
    S_optionsMsrPitchesLanguageItem
      pitchesLanguageItem =
        dynamic_cast<optionsMsrPitchesLanguageItem*>(&(*item))
    ) {
    if (gTraceOptions->fTraceOptions) {
      os <<
        "==> optionsItem is of type 'optionsMsrPitchesLanguageItem'" <<
        endl;
    }

    // wait until the value is met
    result = pitchesLanguageItem;
  }

  return result;
}

void msrOptions::handleOptionsItemValue (
  ostream&      os,
  S_optionsItem item,
  string        theString)
{
  if (
    // part rename item?
    S_optionsPartRenameItem
      partRenameItem =
        dynamic_cast<optionsPartRenameItem*>(&(*item))
    ) {
    // theString contains the part rename specification
    // decipher it to extract the old and new part names

    if (gTraceOptions->fTraceOptions) {
      os <<
        "==> optionsItem is of type 'optionsPartRenameItem'" <<
        endl;
    }

    string regularExpression (
      "[[:space:]]*(.*)[[:space:]]*"
      "="
      "[[:space:]]*(.*)[[:space:]]*");
      
    regex  e (regularExpression);
    smatch sm;

    regex_match (theString, sm, e);

    if (gTraceOptions->fTraceOptions) {
      os <<
        "There are " << sm.size () << " matches" <<
        " for part rename string '" << theString <<
        "' with regex '" << regularExpression <<
        "'" <<
        endl;
    }
  
    if (sm.size ()) {
      if (gTraceOptions->fTraceOptions) {
        for (unsigned i = 0; i < sm.size (); ++i) {
          os <<
            "[" << sm [i] << "] ";
        } // for
        os <<
          endl;
      }
    }
    
    else {
      stringstream s;

      s <<
        "-msrPartRename argument '" << theString <<
        "' is ill-formed";
        
      optionError (s.str ());
      
      printSpecificSubGroupHelp (
        os,
        partRenameItem->
          getOptionsSubGroupUplink ());
          
      exit (4);
    }

    string
      oldPartName = sm [1],
      newPartName = sm [2];
      
    if (gTraceOptions->fTraceOptions) {
      os <<
        "--> oldPartName = \"" << oldPartName << "\", " <<
        "--> newPartName = \"" << newPartName << "\"" <<
        endl;
    }

    map<string, string>
      partRenameItemVariable =
        partRenameItem->
          getOptionsPartRenameItemVariable ();
          
    // is this part name in the part renaming map?
    map<string, string>::iterator
      it =
        partRenameItemVariable.find (oldPartName);
          
    if (it != partRenameItemVariable.end ()) {
      // yes, issue error message
      stringstream s;

      s <<
        "Part \"" << oldPartName << "\" occurs more that one" <<
        "in the '--partName' option";
        
      optionError (s.str ());
      exit (4);
    }
    
    else {
      partRenameItem->
        setPartRenameItemVariableValue (
          oldPartName, newPartName);
    }
  }

  else if (
    // MSR pitches language item?
    S_optionsMsrPitchesLanguageItem
      pitchesLanguageKindItem =
        dynamic_cast<optionsMsrPitchesLanguageItem*>(&(*item))
    ) {
    // theString contains the language name:     
    // is it in the pitches languages map?

    if (gTraceOptions->fTraceOptions) {
      os <<
        "==> optionsItem is of type 'optionsMsrPitchesLanguageItem'" <<
        endl;
    }

    map<string, msrQuarterTonesPitchesLanguageKind>::const_iterator
      it =
        gQuarterTonesPitchesLanguageKindsMap.find (
          theString);
          
    if (it == gQuarterTonesPitchesLanguageKindsMap.end ()) {
      // no, language is unknown in the map
      
      printHelpSummary (os);
      
      stringstream s;
  
      s <<
        "MSR pitches language " << theString <<
        " is unknown" <<
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
      
 //     exit (4); // JMI
      abort ();
    }
  
    pitchesLanguageKindItem->
      setPitchesLanguageKindItemVariableValue (
        (*it).second);
  }
}

ostream& operator<< (ostream& os, const S_msrOptions& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
void initializeMsrOptionsHandling (
  S_optionsHandler optionsHandler)
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
