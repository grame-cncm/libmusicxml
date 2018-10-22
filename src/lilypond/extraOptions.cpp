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

#include <regex>

#include "version.h"
#include "utilities.h"

#include "extraOptions.h"

#include "setTraceOptionsIfDesired.h"
#ifdef TRACE_OPTIONS
  #include "traceOptions.h"
#endif

#include "lpsrOptions.h"


using namespace std;

namespace MusicXML2 
{

//______________________________________________________________________________
S_optionsShowAllChordsStructuresItem optionsShowAllChordsStructuresItem::create (
  string optionsItemShortName,
  string optionsItemLongName,
  string optionsItemDescription)
{
  optionsShowAllChordsStructuresItem* o = new
    optionsShowAllChordsStructuresItem (
      optionsItemShortName,
      optionsItemLongName,
      optionsItemDescription);
  assert(o!=0);
  return o;
}

optionsShowAllChordsStructuresItem::optionsShowAllChordsStructuresItem (
  string optionsItemShortName,
  string optionsItemLongName,
  string optionsItemDescription)
  : optionsItem (
      optionsItemShortName,
      optionsItemLongName,
      optionsItemDescription)
{}

optionsShowAllChordsStructuresItem::~optionsShowAllChordsStructuresItem ()
{}

void optionsShowAllChordsStructuresItem::print (ostream& os) const
{
  const int fieldWidth = K_FIELD_WIDTH;
  
  os <<
    "OptionsShowAllChordsStructuresItem:" <<
    endl;

  gIndenter++;

  optionsElement::printElementEssentials (
    os, fieldWidth);

  gIndenter++;
  os <<
    gIndenter.indentMultiLineString (
      fOptionsElementDescription) <<
    endl;
  gIndenter--;

  gIndenter--;
}

void optionsShowAllChordsStructuresItem::printAllChordsStructures (ostream& os) const
{
  msrChordStructure::printAllChordsStructures (os);
}

void optionsShowAllChordsStructuresItem::printOptionsValues (
  ostream& os,
  int      valueFieldWidth) const
{
  // nothing to print here
}
                            
ostream& operator<< (ostream& os, const S_optionsShowAllChordsStructuresItem& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_optionsShowAllChordsContentsItem optionsShowAllChordsContentsItem::create (
  string  optionsItemShortName,
  string  optionsItemLongName,
  string  optionsItemDescription,
  string  optionsValueSpecification,
  string  optionsShowAllChordsContentsItemVariableDisplayName,
  string& optionsShowAllChordsContentsItemVariable)
{
  optionsShowAllChordsContentsItem* o = new
    optionsShowAllChordsContentsItem (
      optionsItemShortName,
      optionsItemLongName,
      optionsItemDescription,
      optionsValueSpecification,
      optionsShowAllChordsContentsItemVariableDisplayName,
      optionsShowAllChordsContentsItemVariable);
  assert(o!=0);
  return o;
}

optionsShowAllChordsContentsItem::optionsShowAllChordsContentsItem (
  string  optionsItemShortName,
  string  optionsItemLongName,
  string  optionsItemDescription,
  string  optionsValueSpecification,
  string  optionsShowAllChordsContentsItemVariableDisplayName,
  string& optionsShowAllChordsContentsItemVariable)
  : optionsValuedItem (
      optionsItemShortName,
      optionsItemLongName,
      optionsItemDescription,
      optionsValueSpecification),
    fOptionsShowAllChordsContentsItemVariableDisplayName (
      optionsShowAllChordsContentsItemVariableDisplayName),
    fOptionsShowAllChordsContentsItemVariable (
      optionsShowAllChordsContentsItemVariable)
{}

optionsShowAllChordsContentsItem::~optionsShowAllChordsContentsItem ()
{}

void optionsShowAllChordsContentsItem::print (ostream& os) const
{
  const int fieldWidth = K_FIELD_WIDTH;
  
  os <<
    "optionsShowAllChordsContentsItem:" <<
    endl;

  gIndenter++;

  optionsElement::printElementEssentials (
    os, fieldWidth);

  gIndenter++;
  os <<
    gIndenter.indentMultiLineString (
      fOptionsElementDescription) <<
    endl;
  gIndenter--;

  gIndenter--;
}

void optionsShowAllChordsContentsItem::printAllChordsContents (
  ostream&              os,
  msrSemiTonesPitchKind semiTonesPitchKind) const
{  
  msrChordContents::printAllChordsContents (
    os,
    semiTonesPitchKind);
}

void optionsShowAllChordsContentsItem::printOptionsValues (
  ostream& os,
  int      valueFieldWidth) const
{
  // nothing to print here
}
                            
ostream& operator<< (ostream& os, const S_optionsShowAllChordsContentsItem& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_optionsShowChordDetailsItem optionsShowChordDetailsItem::create (
  string  optionsItemShortName,
  string  optionsItemLongName,
  string  optionsItemDescription,
  string  optionsValueSpecification,
  string  optionsShowChordDetailsItemVariableDisplayName,
  string& optionsShowChordDetailsItemVariable)
{
  optionsShowChordDetailsItem* o = new
    optionsShowChordDetailsItem (
      optionsItemShortName,
      optionsItemLongName,
      optionsItemDescription,
      optionsValueSpecification,
      optionsShowChordDetailsItemVariableDisplayName,
      optionsShowChordDetailsItemVariable);
  assert(o!=0);
  return o;
}

optionsShowChordDetailsItem::optionsShowChordDetailsItem (
  string  optionsItemShortName,
  string  optionsItemLongName,
  string  optionsItemDescription,
  string  optionsValueSpecification,
  string  optionsShowChordDetailsItemVariableDisplayName,
  string& optionsShowChordDetailsItemVariable)
  : optionsValuedItem (
      optionsItemShortName,
      optionsItemLongName,
      optionsItemDescription,
      optionsValueSpecification),
    fOptionsShowChordDetailsItemVariableDisplayName (
      optionsShowChordDetailsItemVariableDisplayName),
    fOptionsShowChordDetailsItemVariable (
      optionsShowChordDetailsItemVariable)
{}

optionsShowChordDetailsItem::~optionsShowChordDetailsItem ()
{}

void optionsShowChordDetailsItem::print (ostream& os) const
{
  const int fieldWidth = K_FIELD_WIDTH;
  
  os <<
    "optionsShowChordDetailsItem:" <<
    endl;

  gIndenter++;

  optionsElement::printElementEssentials (
    os, fieldWidth);

  gIndenter++;
  os <<
    gIndenter.indentMultiLineString (
      fOptionsElementDescription) <<
    endl;
  gIndenter--;

  gIndenter--;
}

void optionsShowChordDetailsItem::printOptionsValues (
  ostream& os,
  int      valueFieldWidth) const
{
  // nothing to print here
}
                            
ostream& operator<< (ostream& os, const S_optionsShowChordDetailsItem& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_optionsShowChordAnalysisItem optionsShowChordAnalysisItem::create (
  string  optionsItemShortName,
  string  optionsItemLongName,
  string  optionsItemDescription,
  string  optionsValueSpecification,
  string  optionsShowChordAnalysisItemVariableDisplayName,
  string& optionsShowChordAnalysisItemVariable)
{
  optionsShowChordAnalysisItem* o = new
    optionsShowChordAnalysisItem (
      optionsItemShortName,
      optionsItemLongName,
      optionsItemDescription,
      optionsValueSpecification,
      optionsShowChordAnalysisItemVariableDisplayName,
      optionsShowChordAnalysisItemVariable);
  assert(o!=0);
  return o;
}

optionsShowChordAnalysisItem::optionsShowChordAnalysisItem (
  string  optionsItemShortName,
  string  optionsItemLongName,
  string  optionsItemDescription,
  string  optionsValueSpecification,
  string  optionsShowChordAnalysisItemVariableDisplayName,
  string& optionsShowChordAnalysisItemVariable)
  : optionsValuedItem (
      optionsItemShortName,
      optionsItemLongName,
      optionsItemDescription,
      optionsValueSpecification),
    fOptionsShowChordAnalysisItemVariableDisplayName (
      optionsShowChordAnalysisItemVariableDisplayName),
    fOptionsShowChordAnalysisItemVariable (
      optionsShowChordAnalysisItemVariable)
{}

optionsShowChordAnalysisItem::~optionsShowChordAnalysisItem ()
{}

void optionsShowChordAnalysisItem::print (ostream& os) const
{
  const int fieldWidth = K_FIELD_WIDTH;
  
  os <<
    "optionsShowChordAnalysisItem:" <<
    endl;

  gIndenter++;

  optionsElement::printElementEssentials (
    os, fieldWidth);

  gIndenter++;
  os <<
    gIndenter.indentMultiLineString (
      fOptionsElementDescription) <<
    endl;
  gIndenter--;

  gIndenter--;
}

void optionsShowChordAnalysisItem::printOptionsValues (
  ostream& os,
  int      valueFieldWidth) const
{
  // nothing to print here
}
                            
ostream& operator<< (ostream& os, const S_optionsShowChordAnalysisItem& elt)
{
  elt->print (os);
  return os;
}

//_______________________________________________________________________________

S_extraOptions gExtraOptions;
S_extraOptions gExtraOptionsUserChoices;
S_extraOptions gExtraOptionsWithDetailedTrace;

S_extraOptions extraOptions::create (
  S_optionsHandler optionsHandler)
{
  extraOptions* o = new extraOptions(
    optionsHandler);
  assert(o!=0);
  return o;
}

extraOptions::extraOptions (
  S_optionsHandler optionsHandler)
  : optionsGroup (
    "Extra",
    "he", "help=extra",
R"(These options provide features not related to translation from MusicXML to LilyPond.)",
    optionsHandler)
{
  // append this options group to the options handler
  // if relevant
  if (optionsHandler) {
    optionsHandler->
      appendOptionsGroup (this);
  }

  // initialize it
  initializeExtraOptions (false);
}

extraOptions::~extraOptions ()
{}

void extraOptions::initializeExtraOptions (
  bool boolOptionsInitialValue)
{
  // show all chords intervals
  // --------------------------------------

  {
    // variables
      
    // options
  
    S_optionsSubGroup
      traceAndDisplaySubGroup =
        optionsSubGroup::create (
          "Chords structures",
          "hecs", "help-extra-chord-structures",
R"()",
          optionsSubGroup::kAlwaysShowDescription,
          this);
  
    appendOptionsSubGroup (traceAndDisplaySubGroup);
        
    traceAndDisplaySubGroup->
      appendOptionsItem (
        optionsShowAllChordsStructuresItem::create (
          "sacs", "show-all-chords-structures",
R"(Write all known chords structures to standard output.)"));
  }
  
  
  // show all chords contents

  {
    // variables
      
    // options
  
    S_optionsSubGroup
      workSubGroup =
        optionsSubGroup::create (
          "Chords contents",
          "hecc", "help-extra-chords-contents",
R"(In the options below:
  ROOT_DIATONIC_PITCH should belong to the names available in
  the selected MSR pitches language, "nederlands" by default.
  Other languages can be chosen with the '-mpl, -msrPitchesLanguage' option.

  HARMONY_NAME should be one of:
    MusicXML chords:
      "maj", "min", "aug", "dim", "dom",
      "maj7", "min7", "dim7", "aug7", "halfdim", "minmaj7",
      "maj6", "min6", "dom9", "maj9", "min9", "dom11", "maj11", "min11",
      "dom13", "maj13", "min13", "sus2", "sus4",
      "neapolitan", "italian", "french", "german"
    Jazz-specific chords:
      "pedal", "power", "tristan", "minmaj9", "domsus4", "domaug5",
      "dommin9", "domaug9dim5", "domaug9aug5", "domaug11", "maj7aug11"
)",
          optionsSubGroup::kAlwaysShowDescription,
          this);
  
    appendOptionsSubGroup (workSubGroup);
        
    workSubGroup->
      appendOptionsItem (
        optionsShowAllChordsContentsItem::create (
          "sacc", "show-all-chords-contents",
R"(Write all chords contents for the given diatonic (semitones) pitch
in the current language to standard output.)",
          "pitch",
          "diatonic (semitones) pitch",
          fChordsRootAsString));
  }
  
  
  // show chord details

  {
    // variables
      
    // options
  
    S_optionsSubGroup
      workSubGroup =
        optionsSubGroup::create (
          "Chord details",
          "hecd", "help-extra-chords-details",
R"()",
          optionsSubGroup::kAlwaysShowDescription,
          this);
  
    appendOptionsSubGroup (workSubGroup);
        
    workSubGroup->
      appendOptionsItem (
        optionsShowChordDetailsItem::create (
          "scd", "show-chord-details",
R"(Write the details of the chord for the given diatonic (semitones) pitch
in the current language and the given harmony to standard output.

CHORD_SPEC can be:
  'ROOT_DIATONIC_PITCH HARMONY_NAME'
or
  "ROOT_DIATONIC_PITCH = HARMONY_NAME"
  
The single or double quotes are used to allow spaces in the names
and around the '=' sign, otherwise they can be dispensed with.
Using double quotes allows for shell variables substitutions, as in:
  HARMONY="maj7"
  xml2ly -showChordDetails "bes ${HARMONY}" .)",
          "CHORD_SPEC",
          "diatonic (semitones) pitch",
          fChordsRootAsString));
  }
  
  // show chord analysis

  {
    // variables
      
    // options
  
    S_optionsSubGroup
      workSubGroup =
        optionsSubGroup::create (
          "Chord analysis",
          "heca", "help-extra-chords-analysis",
R"()",
          optionsSubGroup::kAlwaysShowDescription,
          this);
  
    appendOptionsSubGroup (workSubGroup);
        
    workSubGroup->
      appendOptionsItem (
        optionsShowChordAnalysisItem::create (
          "sca", "show-chord-analysis",
R"(Write an analysis of the chord for the given diatonic (semitones) pitch
in the current language and the given harmony to standard output.

CHORD_SPEC can be:
  'ROOT_DIATONIC_PITCH HARMONY_NAME'
or
  "ROOT_DIATONIC_PITCH = HARMONY_NAME"
  
The single or double quotes are used to allow spaces in the names
and around the '=' sign, otherwise they can be dispensed with.
Using double quotes allows for shell variables substitutions, as in:
  HARMONY="maj7"
  xml2ly -showChordAnalysis "bes ${HARMONY}" .)",
          "CHORD_SPEC",
          "diatonic (semitones) pitch",
          fChordsRootAsString));
  }
}

S_extraOptions extraOptions::createCloneWithDetailedTrace ()
{
  S_extraOptions
    clone =
      extraOptions::create (0);
      // 0 not to have it inserted twice in the option handler

  // set the options handler uplink
  clone->
    setOptionsHandlerUplink (
      fOptionsHandlerUplink);


  // chord intervals
  // --------------------------------------

  // chord notes
  // --------------------------------------
    
  return clone;
}  

//______________________________________________________________________________
void extraOptions::enforceQuietness ()
{
}

//______________________________________________________________________________
void extraOptions::checkOptionsConsistency ()
{
  // JMI
}

//______________________________________________________________________________
void extraOptions::printExtraOptionsValues (int fieldWidth)
{  
  gLogIOstream <<
    "The extra options are:" << // JMI
    endl;

  gIndenter++;

  // chord intervals
  // --------------------------------------

  // chord notes
  // --------------------------------------

  gIndenter--;
}

S_optionsItem extraOptions::handleOptionsItem (
  ostream&      os,
  S_optionsItem item)
{
  S_optionsItem result;
  
  if (
    // show all chords intervals item?
    S_optionsShowAllChordsStructuresItem
      showAllChordsStructuresItem =
        dynamic_cast<optionsShowAllChordsStructuresItem*>(&(*item))
  ) {
#ifdef TRACE_OPTIONS
    if (gTraceOptions->fTraceOptions) {
      os <<
        "==> optionsItem is of type 'optionsShowAllChordsStructuresItem'" <<
        endl;
    }
#endif

    // handle it at once
    showAllChordsStructuresItem->
      printAllChordsStructures (os);

    // exit
    exit (0);
  }
  
  else if (
    // show all chords notes item?
    S_optionsShowAllChordsContentsItem
      showAllChordsContentsItem =
        dynamic_cast<optionsShowAllChordsContentsItem*>(&(*item))
    ) {
#ifdef TRACE_OPTIONS
    if (gTraceOptions->fTraceOptions) {
      os <<
        "==> optionsItem is of type 'optionsShowAllChordsContentsItem'" <<
        endl;
    }
#endif

    // wait until the value is met
    result = showAllChordsContentsItem;
  }
  
  else if (
    // show chord details item?
    S_optionsShowChordDetailsItem
      showChordDetailsItem =
        dynamic_cast<optionsShowChordDetailsItem*>(&(*item))
    ) {
#ifdef TRACE_OPTIONS
    if (gTraceOptions->fTraceOptions) {
      os <<
        "==> optionsItem is of type 'optionsShowChordDetailsItem'" <<
        endl;
    }
#endif

    // wait until the value is met
    result = showChordDetailsItem;
  }
  
  else if (
    // show chord analysis item?
    S_optionsShowChordAnalysisItem
      showChordAnalysisItem =
        dynamic_cast<optionsShowChordAnalysisItem*>(&(*item))
    ) {
#ifdef TRACE_OPTIONS
    if (gTraceOptions->fTraceOptions) {
      os <<
        "==> optionsItem is of type 'optionsShowChordAnalysisItem'" <<
        endl;
    }
#endif

    // wait until the value is met
    result = showChordAnalysisItem;
  }

  return result;
}   

void extraOptions::handleOptionsItemValue (
  ostream&      os,
  S_optionsItem item,
  string        theString)
{  
  if (
    // show all chord notes item?
    S_optionsShowAllChordsContentsItem
      showAllChordsContentsItem =
        dynamic_cast<optionsShowAllChordsContentsItem*>(&(*item))
    ) {
    // theString contains the pitch name in the current language
    // is it in the accidental styles map?

#ifdef TRACE_OPTIONS
    if (gTraceOptions->fTraceOptions) {
      os <<
        "==> optionsItem is of type 'optionsShowAllChordsContentsItem'" <<
        ", theString = \"" << theString << "\"" << 
        endl;
    }
#endif

    // fetch the semitones pitch from theString
    msrSemiTonesPitchKind
      semiTonesPitchKind =
        semiTonesPitchKindFromString (
          theString);

    switch (semiTonesPitchKind) {
      case k_NoQuarterTonesPitch_QTP:
      case k_Rest_QTP:
        {
          stringstream s;
      
          s <<
            "'" << theString <<
            "' is no diatonic (semitones) pitch" <<
            " in pitch language '" <<
            msrQuarterTonesPitchesLanguageKindAsString (
              gLpsrOptions->
                fLpsrQuarterTonesPitchesLanguageKind) <<
            "'" <<
            endl;
            
          optionError (s.str ());
                    
          exit (4);
        }
        break;
  
      case kA_Flat_STP:
      case kA_Natural_STP:
      case kA_Sharp_STP:

      case kB_Flat_STP:
      case kB_Natural_STP:
      case kB_Sharp_STP:

      case kC_Flat_STP:
      case kC_Natural_STP:
      case kC_Sharp_STP:

      case kD_Flat_STP:
      case kD_Natural_STP:
      case kD_Sharp_STP:

      case kE_Flat_STP:
      case kE_Natural_STP:
      case kE_Sharp_STP:

      case kF_Flat_STP:
      case kF_Natural_STP:
      case kF_Sharp_STP:

      case kG_Flat_STP:
      case kG_Natural_STP:
      case kG_Sharp_STP:
        break;
  
      default:
        {
          stringstream s;
      
          s <<
            "'" << theString <<
            "' is no diatonic (semitones) pitch" <<
            endl;
            
          optionError (s.str ());
                    
          exit (4);
        }
    } // switch

  // print all the chords notes
  showAllChordsContentsItem->
    printAllChordsContents (
      os,
      semiTonesPitchKind);

  // exit
  exit (23);
  }
  
  else if (
    // show chord details item?
    S_optionsShowChordDetailsItem
      showChordDetailsItem =
        dynamic_cast<optionsShowChordDetailsItem*>(&(*item))
    ) {
    // theString contains the pitch name in the current language
    // is it in the accidental styles map?

#ifdef TRACE_OPTIONS
    if (gTraceOptions->fTraceOptions) {
      os <<
        "==> optionsItem is of type 'optionsShowChordDetailsItem'" <<
        ", theString = \"" << theString << "\"" << 
        endl;
    }
#endif

    // decipher theString with a regular expression
    string regularExpression (
      "[[:space:]]*"
      "([[:alnum:]]+)"
      "[[:space:]]*"
      "([[:alnum:]]+)"
      "[[:space:]]*");
      
    regex  e (regularExpression);
    smatch sm;

    regex_match (theString, sm, e);

#ifdef TRACE_OPTIONS
    if (gTraceOptions->fTraceOptions) {
      os <<
        "There are " << sm.size () << " matches" <<
        " for chord details string '" << theString <<
        "' with regex '" << regularExpression <<
        "'" <<
        endl;
    }
#endif

    if (sm.size ()) {
#ifdef TRACE_OPTIONS
      if (gTraceOptions->fTraceOptions) {
        os <<
          sm.size () << " elements: ";
        for (unsigned i = 0; i < sm.size (); ++i) {
          os <<
            "[" << sm [i] << "] ";
        } // for
        os <<
          endl;
      }
#endif
    }
    
    else {
      stringstream s;

      s <<
        "-chord details argument '" << theString <<
        "' is ill-formed";
        
      optionError (s.str ());
      
      printSpecificSubGroupHelp (
        os,
        showChordDetailsItem->
          getOptionsSubGroupUplink ());
          
      exit (4);
    }

    string
      rootName    = sm [1],
      harmonyName = sm [2];
      
#ifdef TRACE_OPTIONS
    if (gTraceOptions->fTraceOptions) {
      os <<
        "--> rootName = \"" << rootName << "\", " <<
        "--> harmonyName = \"" << harmonyName << "\"" <<
        endl;
    }
#endif

    // fetch the semitones pitch from rootName
    msrSemiTonesPitchKind
      semiTonesPitchKind =
        semiTonesPitchKindFromString (
          rootName);

    switch (semiTonesPitchKind) {
      case k_NoQuarterTonesPitch_QTP:
      case k_Rest_QTP:
        {
          stringstream s;
      
          s <<
            "'" << rootName <<
            "' is no diatonic (semitones) root pitch" <<
            " in pitch language '" <<
            msrQuarterTonesPitchesLanguageKindAsString (
              gLpsrOptions->
                fLpsrQuarterTonesPitchesLanguageKind) <<
            "'" <<
            endl;
            
          optionError (s.str ());
                    
          exit (4);
        }
        break;
  
      case kA_Flat_STP:
      case kA_Natural_STP:
      case kA_Sharp_STP:

      case kB_Flat_STP:
      case kB_Natural_STP:
      case kB_Sharp_STP:

      case kC_Flat_STP:
      case kC_Natural_STP:
      case kC_Sharp_STP:

      case kD_Flat_STP:
      case kD_Natural_STP:
      case kD_Sharp_STP:

      case kE_Flat_STP:
      case kE_Natural_STP:
      case kE_Sharp_STP:

      case kF_Flat_STP:
      case kF_Natural_STP:
      case kF_Sharp_STP:

      case kG_Flat_STP:
      case kG_Natural_STP:
      case kG_Sharp_STP:
        break;
  
      default:
        {
          stringstream s;
      
          s <<
            "'" << rootName <<
            "' is no diatonic (semitones) pitch" <<
            endl;
            
          optionError (s.str ());
                    
          exit (4);
        }
    } // switch

    // fetch the harmony kind from harmonyName
    msrHarmonyKind
      harmonyKind =
        msrHarmonyKindFromString (
          harmonyName);

    // print the chord details
    printChordDetails (
      os,
      semiTonesPitchKind,
      harmonyKind);

    // exit
    exit (23);
  }
  
  else if (
    // show chord analysis item?
    S_optionsShowChordAnalysisItem
      showChordAnalysisItem =
        dynamic_cast<optionsShowChordAnalysisItem*>(&(*item))
    ) {
    // theString contains the pitch name in the current language
    // is it in the accidental styles map?

#ifdef TRACE_OPTIONS
    if (gTraceOptions->fTraceOptions) {
      os <<
        "==> optionsItem is of type 'optionsShowChordAnalysisItem'" <<
        ", theString = \"" << theString << "\"" << 
        endl;
    }
#endif

    // decipher theString with a regular expression
    string regularExpression (
      "[[:space:]]*"
      "([[:alnum:]]+)"
      "[[:space:]]*"
      "([[:alnum:]]+)"
      "[[:space:]]*"
      "([[:digit:]]+)"
      "[[:space:]]*");
      
    regex  e (regularExpression);
    smatch sm;

    regex_match (theString, sm, e);

#ifdef TRACE_OPTIONS
    if (gTraceOptions->fTraceOptions) {
      os <<
        "There are " << sm.size () << " matches" <<
        " for chord analysis string '" << theString <<
        "' with regex '" << regularExpression <<
        "'" <<
        endl;
    }
#endif

    if (sm.size ()) {
#ifdef TRACE_OPTIONS
      if (gTraceOptions->fTraceOptions) {
        os <<
          sm.size () << " elements: ";
        for (unsigned i = 0; i < sm.size (); ++i) {
          os <<
            "[" << sm [i] << "] ";
        } // for
        os <<
          endl;
      }
#endif
    }
    
    else {
      stringstream s;

      s <<
        "-chord analysis argument '" << theString <<
        "' is ill-formed";
        
      optionError (s.str ());
      
      printSpecificSubGroupHelp (
        os,
        showChordAnalysisItem->
          getOptionsSubGroupUplink ());
          
      exit (4);
    }

    string
      rootName          = sm [1],
      harmonyName       = sm [2],
      inversionAsString = sm [3];

    int
      inversion;
      
    stringstream s;

    s << inversionAsString;

    s >> inversion;
    
#ifdef TRACE_OPTIONS
    if (gTraceOptions->fTraceOptions) {
      os <<
        "--> rootName = \"" << rootName << "\", " <<
        "--> harmonyName = \"" << harmonyName << "\"" <<
        "--> inversion = " << inversion <<
        endl;
    }
#endif

    // fetch the semitones pitch from rootName
    msrSemiTonesPitchKind
      semiTonesPitchKind =
        semiTonesPitchKindFromString (
          rootName);

    switch (semiTonesPitchKind) {
      case k_NoQuarterTonesPitch_QTP:
      case k_Rest_QTP:
        {
          stringstream s;
      
          s <<
            "'" << rootName <<
            "' is no diatonic (semitones) root pitch" <<
            " in pitch language '" <<
            msrQuarterTonesPitchesLanguageKindAsString (
              gLpsrOptions->
                fLpsrQuarterTonesPitchesLanguageKind) <<
            "'" <<
            endl;
            
          optionError (s.str ());
                    
          exit (4);
        }
        break;
  
      case kA_Flat_STP:
      case kA_Natural_STP:
      case kA_Sharp_STP:

      case kB_Flat_STP:
      case kB_Natural_STP:
      case kB_Sharp_STP:

      case kC_Flat_STP:
      case kC_Natural_STP:
      case kC_Sharp_STP:

      case kD_Flat_STP:
      case kD_Natural_STP:
      case kD_Sharp_STP:

      case kE_Flat_STP:
      case kE_Natural_STP:
      case kE_Sharp_STP:

      case kF_Flat_STP:
      case kF_Natural_STP:
      case kF_Sharp_STP:

      case kG_Flat_STP:
      case kG_Natural_STP:
      case kG_Sharp_STP:
        break;
  
      default:
        {
          stringstream s;
      
          s <<
            "'" << rootName <<
            "' is no diatonic (semitones) pitch" <<
            endl;
            
          optionError (s.str ());
                    
          exit (4);
        }
    } // switch

    // fetch the harmony kind from harmonyName
    msrHarmonyKind
      harmonyKind =
        msrHarmonyKindFromString (
          harmonyName);

    if (harmonyKind == k_NoHarmony) {
      stringstream s;
  
      s <<
        "'" << harmonyName <<
        "' is no harmony name, valid names are:" <<
        endl <<
        "maj, min,aug, dim, dom, maj7, min7, dim7, aug7, halfdim, minmaj7, maj6, min6, dom9, maj9, min9, dom11, maj11, min11, dom13, maj13, min13, sus2, sus4, neapolitan, italian, french, german, pedal, power, tristan, minmaj9, domsus4, domaug5, dommin9, domaug9dim5, domaug9aug5, domaug11 and maj7aug11" <<
        endl;
        
      optionError (s.str ());
                
      exit (4);
    }
    
    // print the chord analysis
    printChordAnalysis (
      os,
      semiTonesPitchKind,
      harmonyKind,
      inversion);
      
    // exit
    exit (23);
  }
}

ostream& operator<< (ostream& os, const S_extraOptions& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
void initializeExtraOptionsHandling (
  S_optionsHandler optionsHandler)
{
  // create the extra options
  // ------------------------------------------------------
  
  gExtraOptionsUserChoices = extraOptions::create (
    optionsHandler);
  assert(gExtraOptionsUserChoices != 0);

  gExtraOptions =
    gExtraOptionsUserChoices;

  // prepare for measure detailed trace
  // ------------------------------------------------------

  gExtraOptionsWithDetailedTrace =
    gExtraOptions->
      createCloneWithDetailedTrace ();
}


}
