/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#include <sstream>
#include <iomanip>      // setw, setprecision, ...

#include "utilities.h"

#include "msrOptions.h" // for optionsPitchesLanguageItem
#include "lpsrOptions.h"


using namespace std;

namespace MusicXML2 
{

#define TRACE_OPTIONS 0

//______________________________________________________________________________
S_optionsChordsLanguageItem optionsChordsLanguageItem::create (
  string             optionsItemShortName,
  string             optionsItemLongName,
  string             optionsItemDescription,
  string             optionsValueSpecification,
  string             optionsChordsLanguageKindItemVariableDisplayName,
  lpsrChordsLanguageKind&
                     optionsChordsLanguageKindItemVariable)
{
  optionsChordsLanguageItem* o = new
    optionsChordsLanguageItem (
      optionsItemShortName,
      optionsItemLongName,
      optionsItemDescription,
      optionsValueSpecification,
      optionsChordsLanguageKindItemVariableDisplayName,
      optionsChordsLanguageKindItemVariable);
  assert(o!=0);
  return o;
}

optionsChordsLanguageItem::optionsChordsLanguageItem (
  string             optionsItemShortName,
  string             optionsItemLongName,
  string             optionsItemDescription,
  string             optionsValueSpecification,
  string             optionsChordsLanguageKindItemVariableDisplayName,
  lpsrChordsLanguageKind&
                     optionsChordsLanguageKindItemVariable)
  : optionsValuedItem (
      optionsItemShortName,
      optionsItemLongName,
      optionsItemDescription,
      optionsValueSpecification),
    fOptionsChordsLanguageKindItemVariableDisplayName (
      optionsChordsLanguageKindItemVariableDisplayName),
    fOptionsChordsLanguageKindItemVariable (
      optionsChordsLanguageKindItemVariable)
{}

optionsChordsLanguageItem::~optionsChordsLanguageItem()
{}

void optionsChordsLanguageItem::print (ostream& os) const
{
  const int fieldWidth = K_FIELD_WIDTH;
  
  os <<
    "OptionsChordsLanguageItem:" <<
    endl;

  gIndenter++;

  printValuedItemEssentials (
    os, fieldWidth);

  os << left <<
    setw (fieldWidth) <<
    "fOptionsChordsLanguageKindItemVariableDisplayName" << " : " <<
    fOptionsChordsLanguageKindItemVariableDisplayName <<
    setw (fieldWidth) <<
    "fOptionsChordsLanguageKindItemVariable" << " : \"" <<
    lpsrChordsLanguageKindAsString (
      fOptionsChordsLanguageKindItemVariable) <<
      "\"" <<
    endl;
}

void optionsChordsLanguageItem::printOptionsValues (
  ostream& os,
  int      valueFieldWidth) const
{  
  os << left <<
    setw (valueFieldWidth) <<
    fOptionsChordsLanguageKindItemVariableDisplayName <<
    " : \"" <<
    lpsrChordsLanguageKindAsString (
      fOptionsChordsLanguageKindItemVariable) <<
    "\"" <<
    endl;
}

ostream& operator<< (ostream& os, const S_optionsChordsLanguageItem& elt)
{
  elt->print (os);
  return os;
}

//_______________________________________________________________________________
S_lpsrOptions gLpsrOptions;
S_lpsrOptions gLpsrOptionsUserChoices;
S_lpsrOptions gLpsrOptionsWithDetailedTrace;

S_lpsrOptions lpsrOptions::create (
  S_optionsHandler optionsHandler)
{
  lpsrOptions* o = new lpsrOptions (
    optionsHandler);
  assert(o!=0);
  return o;
}

lpsrOptions::lpsrOptions (
  S_optionsHandler optionsHandler)
  : optionsGroup (
    "LPSR",
    "hlpsr", "helpLPSR",
R"(These options control the way LPSR data is handled.)",
    optionsHandler)
{
  // append this options group to the options handler
  // if relevant
  if (optionsHandler) {
    optionsHandler->
      appendOptionsGroup (this);
  }

  // initialize it
  initializeLpsrOptions (false);
}

lpsrOptions::~lpsrOptions ()
{}

void lpsrOptions::initializeLpsrOptions (
  bool boolOptionsInitialValue)
{

  // trace and display
  // --------------------------------------

  {
    // variables
    
    fTraceLpsr            = boolOptionsInitialValue;
    
    fTraceLpsrVisitors    = boolOptionsInitialValue;
  
    fDisplayLpsr          = boolOptionsInitialValue;
    
    fTraceSchemeFunctions = boolOptionsInitialValue;
  
    // options
  
    S_optionsSubGroup
      traceAndDisplaySubGroup =
        optionsSubGroup::create (
          "Trace and display",
          "hlpsrtd", "helpLpsrTraceAndDisplay",
R"()",
        optionsSubGroup::kAlwaysShowDescription,
        this);
  
    appendOptionsSubGroup (traceAndDisplaySubGroup);
        
    traceAndDisplaySubGroup->
      appendOptionsItem (
        optionsBooleanItem::create (
          "tlpsr", "traceLpsr",
R"(Write a trace of the LPSR graphs visiting activity to standard error.)",
          "traceLpsr",
          fTraceLpsr));
  
    traceAndDisplaySubGroup->
      appendOptionsItem (
        optionsBooleanItem::create (
          "tlpsrv", "traceLpsrVisitors",
R"(Write a trace of the LPSR tree visiting activity to standard error.)",
          "traceLpsrVisitors",
          fTraceLpsrVisitors));
  
    traceAndDisplaySubGroup->
      appendOptionsItem (
        optionsBooleanItem::create (
          "lpsr", "displayLpsr",
R"(Write the contents of the LPSR data to standard error.)",
          "displayLpsr",
          fDisplayLpsr));
  
    traceAndDisplaySubGroup->
      appendOptionsItem (
        optionsBooleanItem::create (
          "tsf", "traceSchemeFunctions",
R"(Write a trace of the activity regarding Scheme functions to standard error.)",
          "traceSchemeFunctions",
          fTraceSchemeFunctions));
  }

  // languages
  // --------------------------------------

  {
    // variables
    
    if (! setLpsrQuarterTonesPitchesLanguage ("nederlands")) {
      stringstream s;
  
      s <<
        "INTERNAL INITIALIZATION ERROR: "
        "LPSR pitches language 'nederlands' is unknown" <<
        endl <<
        "The " <<
        gQuarterTonesPitchesLanguageKindsMap.size () <<
        " known LPSR pitches languages are:" <<
        endl;
  
      gIndenter++;
    
      s <<
        existingQuarterTonesPitchesLanguageKinds ();
  
      gIndenter--;
  
      optionError (s.str ());
    }
  
    // options
    
    S_optionsSubGroup
      languagesSubGroup =
        optionsSubGroup::create (
          "Languages",
          "hlpsrl", "helpLpsrlanguages",
R"()",
        optionsSubGroup::kAlwaysShowDescription,
        this);
  
    appendOptionsSubGroup (languagesSubGroup);
      
    languagesSubGroup->
      appendOptionsItem (
        optionsPitchesLanguageItem::create (
          "lppl", "lpsrPitchesLanguage",
R"(Use 'language' to display note pitches in the LPSR logs and views,
as well as in the generated LilyPond code.
The 12 LilyPond pitches languages are available:
nederlands, catalan, deutsch, english, espanol, français, 
italiano, norsk, portugues, suomi, svenska and vlaams.
The default is to use 'nederlands'.)",
          "language",
          "lpsrPitchesLanguage",
          fLpsrQuarterTonesPitchesLanguageKind));
  
    languagesSubGroup->
      appendOptionsItem (
        optionsChordsLanguageItem::create (
          "lpcl", "lpsrChordsLanguage",
R"(Use 'language' to display chord names, their root and bass notes,
in the LPSR logs and views and the generated LilyPond code.
The 4 LilyPond chords languages are available:
german, semiGerman, italian and french.
The default used by LilyPond is Ignatzek's jazz-like, english naming.)",
          "language",
          "lpsrChordsLanguage",
          fLpsrChordsLanguageKind));
  }
}

S_lpsrOptions lpsrOptions::createCloneWithDetailedTrace ()
{
  S_lpsrOptions
    clone =
      lpsrOptions::create (0);
      // 0 not to have it inserted twice in the option handler

  // set the options handler uplink
  clone->
    setOptionsHandlerUplink (
      fOptionsHandlerUplink);


  // trace and display
  // --------------------------------------
    
  clone->fTraceLpsr =
    true;

  clone->fTraceLpsrVisitors =
    true;

  clone->fDisplayLpsr =
    true;

    
  // Scheme functions
  // --------------------------------------

  clone->fTraceSchemeFunctions =
    true;

  
  // languages
  // --------------------------------------

  clone->fLpsrQuarterTonesPitchesLanguageKind =
    fLpsrQuarterTonesPitchesLanguageKind;

  clone->fLpsrChordsLanguageKind =
    fLpsrChordsLanguageKind;

  return clone;
}

bool lpsrOptions::setLpsrQuarterTonesPitchesLanguage (string language)
{
  // is language in the note names languages map?
  map<string, msrQuarterTonesPitchesLanguageKind>::const_iterator
    it =
      gQuarterTonesPitchesLanguageKindsMap.find (language);
        
  if (it == gQuarterTonesPitchesLanguageKindsMap.end ()) {
    // no, language is unknown in the map    
    return false;
  }

  fLpsrQuarterTonesPitchesLanguageKind = (*it).second;
  
  return true;
}

bool lpsrOptions::setLpsrChordsLanguage (string language)
{
  // is language in the chords languages map?
  map<string, lpsrChordsLanguageKind>::const_iterator
    it =
      gLpsrChordsLanguageKindsMap.find (language);
        
  if (it == gLpsrChordsLanguageKindsMap.end ()) {
    // no, language is unknown in the map    
    return false;
  }

  fLpsrChordsLanguageKind = (*it).second;
  
  return true;
}

void lpsrOptions::printLpsrOptionsValues (int fieldWidth)
{
  gLogIOstream <<
    "The LPSR options are:" <<
    endl;

  gIndenter++;
  
  // trace and display
  // --------------------------------------
  gLogIOstream <<
    "Trace and display:" <<
    endl;

  gIndenter++;
  
  gLogIOstream << left <<
    setw (fieldWidth) << "traceLpsr" << " : " <<
    booleanAsString (fTraceLpsr) <<
    endl <<
    
    setw (fieldWidth) << "traceLpsrVisitors" << " : " <<
    booleanAsString (fTraceLpsrVisitors) <<
    endl <<
    
    setw (fieldWidth) << "displayLpsr" << " : " <<
    booleanAsString (fDisplayLpsr) <<
    endl <<
    
    // Scheme functions
    setw (fieldWidth) << "traceSchemeFunctions" << " : " <<
    booleanAsString (fTraceSchemeFunctions) <<
    endl;

  gIndenter--;
  
  // languages
  // --------------------------------------
  gLogIOstream <<
    "Languages:" <<
    endl;

  gIndenter++;

  gLogIOstream << left <<
    setw (fieldWidth) << "lpsrQuarterTonesPitchesLanguage" << " : \"" <<
    msrQuarterTonesPitchesLanguageKindAsString (
      fLpsrQuarterTonesPitchesLanguageKind) <<
    "\"" <<
    endl <<

    setw (fieldWidth) << "lpsrChordsLanguage" << " : \"" <<
    lpsrChordsLanguageKindAsString (
      fLpsrChordsLanguageKind) <<
    "\"" <<
    endl;  

  gIndenter--;

  
  gIndenter--;
}

S_optionsItem lpsrOptions::handleOptionsItem (
  ostream&      os,
  S_optionsItem item)
{
  S_optionsItem result;
  
  if (
    // chords language item?
    S_optionsChordsLanguageItem
      chordsLanguageItem =
        dynamic_cast<optionsChordsLanguageItem*>(&(*item))
    ) {
    if (TRACE_OPTIONS) {
      os <<
        "==> optionsItem is of type 'optionsChordsLanguageItem'" <<
        endl;
    }

    // wait until the value is met
    result = chordsLanguageItem;
  }

  return result;
}

void lpsrOptions::handleValuedOptionsItem (
  ostream&      os,
  S_optionsItem item,
  string        theString)
{
  if (
    // chords language item?
    S_optionsChordsLanguageItem
      chordsLanguageItem =
        dynamic_cast<optionsChordsLanguageItem*>(&(*item))
    ) {
    // theString contains the language name:     
    // is it in the chords languages map?
    map<string, lpsrChordsLanguageKind>::const_iterator
      it =
        gLpsrChordsLanguageKindsMap.find (theString);
          
    if (it == gLpsrChordsLanguageKindsMap.end ()) {
      // no, language is unknown in the map    
      stringstream s;
  
      s <<
        "LPSR chords language " << theString <<
        " is unknown" <<
        endl <<
        "The " <<
        gLpsrChordsLanguageKindsMap.size () - 1 <<
        " known LPSR chords languages apart from the default Ignatzek are:" <<
        endl;
  
      gIndenter++;
    
      s <<
        existingLpsrChordsLanguageKinds ();
  
      gIndenter--;
  
      optionError (s.str ());
      
      printHelpSummary (os);
      
      exit (4);
    }
  
    chordsLanguageItem->
      setChordsLanguageKindItemVariableValue (
        (*it).second);
  }
}

ostream& operator<< (ostream& os, const S_lpsrOptions& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
void initializeLpsrOptionsHandling (
  S_optionsHandler optionsHandler)
{
  // create the options variables
  // ------------------------------------------------------

  gLpsrOptionsUserChoices = lpsrOptions::create (
    optionsHandler);
  assert(gLpsrOptionsUserChoices != 0);
  
  gLpsrOptions =
    gLpsrOptionsUserChoices;

  // prepare for measure detailed trace
  // ------------------------------------------------------
  gLpsrOptionsWithDetailedTrace =
    gLpsrOptions->
      createCloneWithDetailedTrace ();
}


}
