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

#include "version.h"
#include "utilities.h"

#include "extraOptions.h"

#include "traceOptions.h"
#include "lpsrOptions.h"


using namespace std;

namespace MusicXML2 
{

//______________________________________________________________________________
S_optionsShowAllChordsIntervalsItem optionsShowAllChordsIntervalsItem::create (
  string optionsItemShortName,
  string optionsItemLongName,
  string optionsItemDescription)
{
  optionsShowAllChordsIntervalsItem* o = new
    optionsShowAllChordsIntervalsItem (
      optionsItemShortName,
      optionsItemLongName,
      optionsItemDescription);
  assert(o!=0);
  return o;
}

optionsShowAllChordsIntervalsItem::optionsShowAllChordsIntervalsItem (
  string optionsItemShortName,
  string optionsItemLongName,
  string optionsItemDescription)
  : optionsItem (
      optionsItemShortName,
      optionsItemLongName,
      optionsItemDescription)
{}

optionsShowAllChordsIntervalsItem::~optionsShowAllChordsIntervalsItem()
{}

void optionsShowAllChordsIntervalsItem::print (ostream& os) const
{
  const int fieldWidth = K_FIELD_WIDTH;
  
  os <<
    "OptionsShowAllChordsIntervalsItem:" <<
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

void optionsShowAllChordsIntervalsItem::printAllChordsIntervals (ostream& os) const
{
  msrChordIntervals::printAllChordsIntervals (os);
}

void optionsShowAllChordsIntervalsItem::printOptionsValues (
  ostream& os,
  int      valueFieldWidth) const
{
  // nothing to print here
}
                            
ostream& operator<< (ostream& os, const S_optionsShowAllChordsIntervalsItem& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_optionsShowAllChordsNotesItem optionsShowAllChordsNotesItem::create (
  string  optionsItemShortName,
  string  optionsItemLongName,
  string  optionsItemDescription,
  string  optionsValueSpecification,
  string  optionsShowAllChordsNotesItemVariableDisplayName,
  string& optionsShowAllChordsNotesItemVariable)
{
  optionsShowAllChordsNotesItem* o = new
    optionsShowAllChordsNotesItem (
      optionsItemShortName,
      optionsItemLongName,
      optionsItemDescription,
      optionsValueSpecification,
      optionsShowAllChordsNotesItemVariableDisplayName,
      optionsShowAllChordsNotesItemVariable);
  assert(o!=0);
  return o;
}

optionsShowAllChordsNotesItem::optionsShowAllChordsNotesItem (
  string  optionsItemShortName,
  string  optionsItemLongName,
  string  optionsItemDescription,
  string  optionsValueSpecification,
  string  optionsShowAllChordsNotesItemVariableDisplayName,
  string& optionsShowAllChordsNotesItemVariable)
  : optionsValuedItem (
      optionsItemShortName,
      optionsItemLongName,
      optionsItemDescription,
      optionsValueSpecification),
    fOptionsShowAllChordsNotesItemVariableDisplayName (
      optionsShowAllChordsNotesItemVariableDisplayName),
    fOptionsShowAllChordsNotesItemVariable (
      optionsShowAllChordsNotesItemVariable)
{}

optionsShowAllChordsNotesItem::~optionsShowAllChordsNotesItem ()
{}

void optionsShowAllChordsNotesItem::print (ostream& os) const
{
  const int fieldWidth = K_FIELD_WIDTH;
  
  os <<
    "optionsShowAllChordsNotesItem:" <<
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

void optionsShowAllChordsNotesItem::printAllChordsNotes (
  ostream&              os,
  msrSemiTonesPitchKind semiTonesPitchKind) const
{  
  msrChordNotes::printAllChordsNotes (
    os,
    semiTonesPitchKind);
}

void optionsShowAllChordsNotesItem::printOptionsValues (
  ostream& os,
  int      valueFieldWidth) const
{
  // nothing to print here
}
                            
ostream& operator<< (ostream& os, const S_optionsShowAllChordsNotesItem& elt)
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
    "he", "helpExtra",
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
  // chord intervals
  // --------------------------------------

  {
    // variables
      
    // options
  
    S_optionsSubGroup
      traceAndDisplaySubGroup =
        optionsSubGroup::create (
          "Chords intervals",
          "heci", "helpExtraChordIntervals",
R"()",
          optionsSubGroup::kAlwaysShowDescription,
          this);
  
    appendOptionsSubGroup (traceAndDisplaySubGroup);
        
    traceAndDisplaySubGroup->
      appendOptionsItem (
        optionsShowAllChordsIntervalsItem::create (
          "saci", "showAllChordsIntervals",
R"(Write all known chords intervals to standard output.)"));
  }
  
  
  // chords notes

  {
    // variables
      
    // options
  
    S_optionsSubGroup
      workSubGroup =
        optionsSubGroup::create (
          "Chords notes",
          "hecn", "helpExtraChordsNotes",
R"()",
          optionsSubGroup::kAlwaysShowDescription,
          this);
  
    appendOptionsSubGroup (workSubGroup);
        
    workSubGroup->
      appendOptionsItem (
        optionsShowAllChordsNotesItem::create (
          "sacn", "showAllChordsNotes",
R"(Write all chords notes for the given diatonic (semitones) pitch
in the current language to standard output.)",
          "pitch",
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
    S_optionsShowAllChordsIntervalsItem
      showAllChordsIntervalsItem =
        dynamic_cast<optionsShowAllChordsIntervalsItem*>(&(*item))
    ) {
    if (gTraceOptions->fTraceOptions) {
      os <<
        "==> optionsItem is of type 'optionsShowAllChordsIntervalsItem'" <<
        endl;
    }

    // handle it at once
    showAllChordsIntervalsItem->
      printAllChordsIntervals (os);

    // exit
    exit (0);
  }
  
  else if (
    // show all chords notes item?
    S_optionsShowAllChordsNotesItem
      showAllChordsNotesItem =
        dynamic_cast<optionsShowAllChordsNotesItem*>(&(*item))
    ) {
    if (gTraceOptions->fTraceOptions) {
      os <<
        "==> optionsItem is of type 'optionsShowAllChordsNotesItem'" <<
        endl;
    }

    // wait until the value is met
    result = showAllChordsNotesItem;
  }

  return result;
}   

void extraOptions::handleValuedOptionsItem (
  ostream&      os,
  S_optionsItem item,
  string        theString)
{  
  if (
    // show chord notes item?
    S_optionsShowAllChordsNotesItem
      showAllChordsNotesItem =
        dynamic_cast<optionsShowAllChordsNotesItem*>(&(*item))
    ) {
    // theString contains the pitch name in the current language
    // is it in the accidental styles map?

    if (gTraceOptions->fTraceOptions) {
      os <<
        "==> optionsItem is of type 'optionsShowAllChordsNotesItem'" <<
        endl;
    }

    // fetch the semitones pitch from the current pitch language
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
            "' is no diatonic  (semitones) pitch" <<
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
        {
          showAllChordsNotesItem->
            printAllChordsNotes (
              os,
              semiTonesPitchKind);
        }
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
