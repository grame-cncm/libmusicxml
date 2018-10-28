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

#include "setTraceOptionsIfDesired.h"
#ifdef TRACE_OPTIONS
  #include "traceOptions.h"
#endif

#include "bsrOptions.h"


using namespace std;

namespace MusicXML2 
{

/*
//______________________________________________________________________________
S_optionsBsrPitchesLanguageItem optionsBsrPitchesLanguageItem::create (
  string             optionsItemShortName,
  string             optionsItemLongName,
  string             optionsItemDescription,
  string             optionsValueSpecification,
  string             optionsBsrPitchesLanguageKindItemVariableDisplayName,
  msrQuarterTonesPitchesLanguageKind&
                     optionsBsrPitchesLanguageKindItemVariable)
{
  optionsBsrPitchesLanguageItem* o = new
    optionsBsrPitchesLanguageItem (
      optionsItemShortName,
      optionsItemLongName,
      optionsItemDescription,
      optionsValueSpecification,
      optionsBsrPitchesLanguageKindItemVariableDisplayName,
      optionsBsrPitchesLanguageKindItemVariable);
  assert(o!=0);
  return o;
}

optionsBsrPitchesLanguageItem::optionsBsrPitchesLanguageItem (
  string             optionsItemShortName,
  string             optionsItemLongName,
  string             optionsItemDescription,
  string             optionsValueSpecification,
  string             optionsBsrPitchesLanguageKindItemVariableDisplayName,
  msrQuarterTonesPitchesLanguageKind&
                     optionsBsrPitchesLanguageKindItemVariable)
  : optionsValuedItem (
      optionsItemShortName,
      optionsItemLongName,
      optionsItemDescription,
      optionsValueSpecification),
    fOptionsBsrPitchesLanguageKindItemVariableDisplayName (
      optionsBsrPitchesLanguageKindItemVariableDisplayName),
    fOptionsBsrPitchesLanguageKindItemVariable (
      optionsBsrPitchesLanguageKindItemVariable)
{}

optionsBsrPitchesLanguageItem::~optionsBsrPitchesLanguageItem ()
{}

void optionsBsrPitchesLanguageItem::print (ostream& os) const
{
  const int fieldWidth = K_FIELD_WIDTH;
  
  os <<
    "OptionsBsrPitchesLanguageItem:" <<
    endl;

  gIndenter++;

  printValuedItemEssentials (
    os, fieldWidth);

  os << left <<
    setw (fieldWidth) <<
    "fOptionsBsrPitchesLanguagKindeItemVariableDisplayName" << " : " <<
    fOptionsBsrPitchesLanguageKindItemVariableDisplayName <<
    endl <<
    setw (fieldWidth) <<
    "fOptionsBsrPitchesLanguageItemVariable" << " : \"" <<
    msrQuarterTonesPitchesLanguageKindAsString (
      fOptionsBsrPitchesLanguageKindItemVariable) <<
      "\"" <<
    endl;
}

void optionsBsrPitchesLanguageItem::printOptionsValues (
  ostream& os,
  int      valueFieldWidth) const
{  
  os << left <<
    setw (valueFieldWidth) <<
    fOptionsBsrPitchesLanguageKindItemVariableDisplayName <<
    " : \"" <<
    msrQuarterTonesPitchesLanguageKindAsString (
      fOptionsBsrPitchesLanguageKindItemVariable) <<
    "\"" <<
    endl;
}

ostream& operator<< (ostream& os, const S_optionsBsrPitchesLanguageItem& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_optionsBsrChordsLanguageItem optionsBsrChordsLanguageItem::create (
  string             optionsItemShortName,
  string             optionsItemLongName,
  string             optionsItemDescription,
  string             optionsValueSpecification,
  string             optionsBsrChordsLanguageKindItemVariableDisplayName,
  bsrChordsLanguageKind&
                     optionsBsrChordsLanguageKindItemVariable)
{
  optionsBsrChordsLanguageItem* o = new
    optionsBsrChordsLanguageItem (
      optionsItemShortName,
      optionsItemLongName,
      optionsItemDescription,
      optionsValueSpecification,
      optionsBsrChordsLanguageKindItemVariableDisplayName,
      optionsBsrChordsLanguageKindItemVariable);
  assert(o!=0);
  return o;
}

optionsBsrChordsLanguageItem::optionsBsrChordsLanguageItem (
  string             optionsItemShortName,
  string             optionsItemLongName,
  string             optionsItemDescription,
  string             optionsValueSpecification,
  string             optionsBsrChordsLanguageKindItemVariableDisplayName,
  bsrChordsLanguageKind&
                     optionsBsrChordsLanguageKindItemVariable)
  : optionsValuedItem (
      optionsItemShortName,
      optionsItemLongName,
      optionsItemDescription,
      optionsValueSpecification),
    fOptionsBsrChordsLanguageKindItemVariableDisplayName (
      optionsBsrChordsLanguageKindItemVariableDisplayName),
    fOptionsBsrChordsLanguageKindItemVariable (
      optionsBsrChordsLanguageKindItemVariable)
{}

optionsBsrChordsLanguageItem::~optionsBsrChordsLanguageItem ()
{}

void optionsBsrChordsLanguageItem::print (ostream& os) const
{
  const int fieldWidth = K_FIELD_WIDTH;
  
  os <<
    "OptionsBsrChordsLanguageItem:" <<
    endl;

  gIndenter++;

  printValuedItemEssentials (
    os, fieldWidth);

  os << left <<
    setw (fieldWidth) <<
    "fOptionsBsrChordsLanguageKindItemVariableDisplayName" << " : " <<
    fOptionsBsrChordsLanguageKindItemVariableDisplayName <<
    setw (fieldWidth) <<
    "fOptionsBsrChordsLanguageKindItemVariable" << " : \"" <<
    bsrChordsLanguageKindAsString (
      fOptionsBsrChordsLanguageKindItemVariable) <<
      "\"" <<
    endl;
}

void optionsBsrChordsLanguageItem::printOptionsValues (
  ostream& os,
  int      valueFieldWidth) const
{  
  os << left <<
    setw (valueFieldWidth) <<
    fOptionsBsrChordsLanguageKindItemVariableDisplayName <<
    " : \"" <<
    bsrChordsLanguageKindAsString (
      fOptionsBsrChordsLanguageKindItemVariable) <<
    "\"" <<
    endl;
}

ostream& operator<< (ostream& os, const S_optionsBsrChordsLanguageItem& elt)
{
  elt->print (os);
  return os;
}
*/

//_______________________________________________________________________________
S_bsrOptions gBsrOptions;
S_bsrOptions gBsrOptionsUserChoices;
S_bsrOptions gBsrOptionsWithDetailedTrace;

S_bsrOptions bsrOptions::create (
  S_optionsHandler optionsHandler)
{
  bsrOptions* o = new bsrOptions (
    optionsHandler);
  assert(o!=0);
  return o;
}

bsrOptions::bsrOptions (
  S_optionsHandler optionsHandler)
  : optionsGroup (
    "BSR",
    "hbsr", "help=bsr",
R"(These options control the way BSR data is handled.)",
    optionsHandler)
{
  // append this options group to the options handler
  // if relevant
  if (optionsHandler) {
    optionsHandler->
      appendOptionsGroup (this);
  }

  // initialize it
  initializeBsrOptions (false);
}

bsrOptions::~bsrOptions ()
{}

void bsrOptions::initializeBsrOptions (
  bool boolOptionsInitialValue)
{

  // trace and display
  // --------------------------------------

  {
    // variables
    
    fTraceBsr             = boolOptionsInitialValue;
    
    fTraceBsrVisitors     = boolOptionsInitialValue;
  
    fDisplayBsr           = boolOptionsInitialValue;
      
    // options
  
    S_optionsSubGroup
      traceAndDisplaySubGroup =
        optionsSubGroup::create (
          "Trace and display",
          "hbsrtd", "help-bsr-trace-and-display",
R"()",
        optionsSubGroup::kAlwaysShowDescription,
        this);
  
    appendOptionsSubGroup (traceAndDisplaySubGroup);
        
    traceAndDisplaySubGroup->
      appendOptionsItem (
        optionsBooleanItem::create (
          "tbsr", "trace-bsr",
R"(Write a trace of the BSR graphs visiting activity to standard error.)",
          "traceBsr",
          fTraceBsr));
  
    traceAndDisplaySubGroup->
      appendOptionsItem (
        optionsBooleanItem::create (
          "tbsrv", "trace-bsr-visitors",
R"(Write a trace of the BSR tree visiting activity to standard error.)",
          "traceBsrVisitors",
          fTraceBsrVisitors));
  
#ifdef TRACE_OPTIONS
    traceAndDisplaySubGroup->
      appendOptionsItem (
        optionsTwoBooleansItem::create (
          "dbsr", "display-bsr",
R"(Write the contents of the BSR data to standard error.)",
          "displayBsr",
          fDisplayBsr,
          gTraceOptions->fTracePasses));
#endif
  }


/*
  // lyrics vs words
  // --------------------------------------

  {
    // variables
    
    fAddWordsFromTheLyrics = boolOptionsInitialValue;
    
    // options
  
    S_optionsSubGroup
      traceAndDisplaySubGroup =
        optionsSubGroup::create (
          "Lyrics vs words",
          "hlyrsvswords", "help-lyrics-vs-words",
R"()",
        optionsSubGroup::kAlwaysShowDescription,
        this);
  
    appendOptionsSubGroup (traceAndDisplaySubGroup);
        
    traceAndDisplaySubGroup->
      appendOptionsItem (
        optionsBooleanItem::create (
          "awftl", "add-words-from-the-lyrics",
R"(Add words with the lyrics contents, keeping the latter untouched.
This may come in handy when MusicXML data has been obtained from scanned images.)",
          "addWordsFromTheLyrics",
          fAddWordsFromTheLyrics));
  }
  

  // languages
  // --------------------------------------

  {
    // variables
    
    if (! setBsrQuarterTonesPitchesLanguage ("nederlands")) {
      stringstream s;
  
      s <<
        "INTERNAL INITIALIZATION ERROR: "
        "BSR pitches language 'nederlands' is unknown" <<
        endl <<
        "The " <<
        gQuarterTonesPitchesLanguageKindsMap.size () <<
        " known BSR pitches languages are:" <<
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
          "hbsrl", "help-bsr-languages",
R"()",
        optionsSubGroup::kAlwaysShowDescription,
        this);
  
    appendOptionsSubGroup (languagesSubGroup);
      
    languagesSubGroup->
      appendOptionsItem (
        optionsBsrPitchesLanguageItem::create (
          "lppl", "bsr-pitches-language",
R"(Use LANGUAGE to display note pitches in the BSR logs and views,
as well as in the generated LilyPond code.
The 12 LilyPond pitches languages are available:
nederlands, catalan, deutsch, english, espanol, français, 
italiano, norsk, portugues, suomi, svenska and vlaams.
The default is to use 'nederlands'.)",
          "LANGUAGE",
          "bsrPitchesanguage",
          fBsrQuarterTonesPitchesLanguageKind));
  
    languagesSubGroup->
      appendOptionsItem (
        optionsBsrChordsLanguageItem::create (
          "lpcl", "bsr-chords-language",
R"(Use LANGUAGE to display chord names, their root and bass notes,
in the BSR logs and views and the generated LilyPond code.
The 4 LilyPond chords languages are available:
german, semiGerman, italian and french.
The default used by LilyPond is Ignatzek's jazz-like, english naming.)",
          "LANGUAGE",
          "bsr-chords-language",
          fBsrChordsLanguageKind));
  }
  */
}

S_bsrOptions bsrOptions::createCloneWithDetailedTrace ()
{
  S_bsrOptions
    clone =
      bsrOptions::create (0);
      // 0 not to have it inserted twice in the option handler

  // set the options handler uplink
  clone->
    setOptionsHandlerUplink (
      fOptionsHandlerUplink);


  // trace and display
  // --------------------------------------
    
  clone->fTraceBsr =
    true;

  clone->fTraceBsrVisitors =
    true;

  clone->fDisplayBsr =
    true;

    
  // lyrics vs words
  // --------------------------------------

  clone->fAddWordsFromTheLyrics =
    true;


/*  
  // languages
  // --------------------------------------

  clone->fBsrQuarterTonesPitchesLanguageKind =
    fBsrQuarterTonesPitchesLanguageKind;

  clone->fBsrChordsLanguageKind =
    fBsrChordsLanguageKind;
    */

  return clone;
}

/*
bool bsrOptions::setBsrQuarterTonesPitchesLanguage (string language)
{
  // is language in the note names languages map?
  map<string, msrQuarterTonesPitchesLanguageKind>::const_iterator
    it =
      gQuarterTonesPitchesLanguageKindsMap.find (language);
        
  if (it == gQuarterTonesPitchesLanguageKindsMap.end ()) {
    // no, language is unknown in the map    
    return false;
  }

  fBsrQuarterTonesPitchesLanguageKind = (*it).second;
  
  return true;
}

bool bsrOptions::setBsrChordsLanguage (string language)
{
  // is language in the chords languages map?
  map<string, bsrChordsLanguageKind>::const_iterator
    it =
      gBsrChordsLanguageKindsMap.find (language);
        
  if (it == gBsrChordsLanguageKindsMap.end ()) {
    // no, language is unknown in the map    
    return false;
  }

  fBsrChordsLanguageKind = (*it).second;
  
  return true;
}
*/

//______________________________________________________________________________
void bsrOptions::enforceQuietness ()
{
  fTraceBsr = false;
  fTraceBsrVisitors = false;
  fDisplayBsr = false;
}

//______________________________________________________________________________
void bsrOptions::checkOptionsConsistency ()
{
  // JMI
}

//______________________________________________________________________________
void bsrOptions::printBsrOptionsValues (int fieldWidth)
{
  gLogIOstream <<
    "The BSR options are:" <<
    endl;

  gIndenter++;
  
  // trace and display
  // --------------------------------------
  gLogIOstream <<
    "Trace and display:" <<
    endl;

  gIndenter++;
  
  gLogIOstream << left <<
    setw (fieldWidth) << "traceBsr" << " : " <<
    booleanAsString (fTraceBsr) <<
    endl <<
    
    setw (fieldWidth) << "traceBsrVisitors" << " : " <<
    booleanAsString (fTraceBsrVisitors) <<
    endl <<
    
    setw (fieldWidth) << "displayBsr" << " : " <<
    booleanAsString (fDisplayBsr) <<
    endl;

  gIndenter--;
  
  // lyrics vs words
  // --------------------------------------
  
  gLogIOstream <<
    "Lyrics vs words:" <<
    endl;

  gIndenter++;
  
  gLogIOstream << left <<
    setw (fieldWidth) << "addWordsFromTheLyrics" << " : " <<
    booleanAsString (fAddWordsFromTheLyrics) <<
    endl;

  gIndenter--;

  // languages
  // --------------------------------------

  /*
  gLogIOstream <<
    "Languages:" <<
    endl;

  gIndenter++;

  gLogIOstream << left <<
    setw (fieldWidth) << "bsrQuarterTonesPitchesLanguage" << " : \"" <<
    msrQuarterTonesPitchesLanguageKindAsString (
      fBsrQuarterTonesPitchesLanguageKind) <<
    "\"" <<
    endl <<

    setw (fieldWidth) << "bsrChordsLanguage" << " : \"" <<
    bsrChordsLanguageKindAsString (
      fBsrChordsLanguageKind) <<
    "\"" <<
    endl;  

  gIndenter--;
*/
  
  gIndenter--;
}

S_optionsItem bsrOptions::handleOptionsItem (
  ostream&      os,
  S_optionsItem item)
{
  S_optionsItem result;

  /*
  if (
    // BSR pitches language item?
    S_optionsBsrPitchesLanguageItem
      pitchesLanguageItem =
        dynamic_cast<optionsBsrPitchesLanguageItem*>(&(*item))
    ) {
#ifdef TRACE_OPTIONS
    if (gTraceOptions->fTraceOptions) {
      os <<
        "==> optionsItem is of type 'optionsBsrPitchesLanguageItem'" <<
        endl;
    }
#endif

    // wait until the value is met
    result = pitchesLanguageItem;
  }

  else if (
    // chords language item?
    S_optionsBsrChordsLanguageItem
      bsrChordsLanguageItem =
        dynamic_cast<optionsBsrChordsLanguageItem*>(&(*item))
    ) {
#ifdef TRACE_OPTIONS
    if (gTraceOptions->fTraceOptions) {
      os <<
        "==> optionsItem is of type 'optionsBsrChordsLanguageItem'" <<
        endl;
    }
#endif

    // wait until the value is met
    result = bsrChordsLanguageItem;
  }
  */

  return result;
}

void bsrOptions::handleOptionsItemValue (
  ostream&      os,
  S_optionsItem item,
  string        theString)
{
  /*
  if (
    // BSR pitches language item?
    S_optionsBsrPitchesLanguageItem
      pitchesLanguageKindItem =
        dynamic_cast<optionsBsrPitchesLanguageItem*>(&(*item))
    ) {
    // theString contains the language name:     
    // is it in the pitches languages map?

#ifdef TRACE_OPTIONS
    if (gTraceOptions->fTraceOptions) {
      os <<
        "==> optionsItem is of type 'optionsBsrPitchesLanguageItem'" <<
        endl;
    }
#endif

    map<string, msrQuarterTonesPitchesLanguageKind>::const_iterator
      it =
        gQuarterTonesPitchesLanguageKindsMap.find (
          theString);
          
    if (it == gQuarterTonesPitchesLanguageKindsMap.end ()) {
      // no, language is unknown in the map
      
      printHelpSummary (os);
      
      stringstream s;
  
      s <<
        "BSR pitches language " << theString <<
        " is unknown" <<
        endl <<
        "The " <<
        gQuarterTonesPitchesLanguageKindsMap.size () <<
        " known BSR pitches languages are:" <<
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

  else if (
    // chords language item?
    S_optionsBsrChordsLanguageItem
      bsrChordsLanguageItem =
        dynamic_cast<optionsBsrChordsLanguageItem*>(&(*item))
    ) {
    // theString contains the language name:     
    // is it in the chords languages map?

#ifdef TRACE_OPTIONS
    if (gTraceOptions->fTraceOptions) {
      os <<
        "==> optionsItem is of type 'optionsBsrChordsLanguageItem'" <<
        endl;
    }
#endif

    map<string, bsrChordsLanguageKind>::const_iterator
      it =
        gBsrChordsLanguageKindsMap.find (theString);
          
    if (it == gBsrChordsLanguageKindsMap.end ()) {
      // no, language is unknown in the map    
      stringstream s;
  
      s <<
        "BSR chords language " << theString <<
        " is unknown" <<
        endl <<
        "The " <<
        gBsrChordsLanguageKindsMap.size () - 1 <<
        " known BSR chords languages apart from the default Ignatzek are:" <<
        endl;
  
      gIndenter++;
    
      s <<
        existingBsrChordsLanguageKinds ();
  
      gIndenter--;
  
      optionError (s.str ());
      
      printHelpSummary (os);
      
      exit (4);
    }
  
    bsrChordsLanguageItem->
      setBsrChordsLanguageKindItemVariableValue (
        (*it).second);
  }
  */
}

ostream& operator<< (ostream& os, const S_bsrOptions& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
void initializeBsrOptionsHandling (
  S_optionsHandler optionsHandler)
{
  // create the options variables
  // ------------------------------------------------------

  gBsrOptionsUserChoices = bsrOptions::create (
    optionsHandler);
  assert(gBsrOptionsUserChoices != 0);
  
  gBsrOptions =
    gBsrOptionsUserChoices;

  // prepare for measure detailed trace
  // ------------------------------------------------------
  gBsrOptionsWithDetailedTrace =
    gBsrOptions->
      createCloneWithDetailedTrace ();
}


}
