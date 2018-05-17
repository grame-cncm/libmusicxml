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

#include "lpsrOptions.h"


using namespace std;

namespace MusicXML2 
{

#define TRACE_OPTIONS 1

//______________________________________________________________________________
S_optionsShowAllChordIntervalsItem optionsShowAllChordIntervalsItem::create (
  string optionsItemShortName,
  string optionsItemLongName,
  string optionsItemDescription)
{
  optionsShowAllChordIntervalsItem* o = new
    optionsShowAllChordIntervalsItem (
      optionsItemShortName,
      optionsItemLongName,
      optionsItemDescription);
  assert(o!=0);
  return o;
}

optionsShowAllChordIntervalsItem::optionsShowAllChordIntervalsItem (
  string optionsItemShortName,
  string optionsItemLongName,
  string optionsItemDescription)
  : optionsItem (
      optionsItemShortName,
      optionsItemLongName,
      optionsItemDescription)
{}

optionsShowAllChordIntervalsItem::~optionsShowAllChordIntervalsItem()
{}

void optionsShowAllChordIntervalsItem::print (ostream& os) const
{
  const int fieldWidth = K_FIELD_WIDTH;
  
  os <<
    "OptionsShowAllChordIntervalsItem:" <<
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

void optionsShowAllChordIntervalsItem::printAllChordIntervals (ostream& os) const
{
  msrChordIntervals::printAllChordsIntervals (os);
}

void optionsShowAllChordIntervalsItem::printOptionsValues (
  ostream& os,
  int      valueFieldWidth) const
{
  // nothing to print here
}
                            
ostream& operator<< (ostream& os, const S_optionsShowAllChordIntervalsItem& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_optionsShowAllChordNotesItem optionsShowAllChordNotesItem::create (
  string optionsItemShortName,
  string optionsItemLongName,
  string optionsItemDescription)
{
  optionsShowAllChordNotesItem* o = new
    optionsShowAllChordNotesItem (
      optionsItemShortName,
      optionsItemLongName,
      optionsItemDescription);
  assert(o!=0);
  return o;
}

optionsShowAllChordNotesItem::optionsShowAllChordNotesItem (
  string optionsItemShortName,
  string optionsItemLongName,
  string optionsItemDescription)
  : optionsItem (
      optionsItemShortName,
      optionsItemLongName,
      optionsItemDescription)
{}

optionsShowAllChordNotesItem::~optionsShowAllChordNotesItem ()
{}

void optionsShowAllChordNotesItem::print (ostream& os) const
{
  const int fieldWidth = K_FIELD_WIDTH;
  
  os <<
    "optionsShowAllChordNotesItem:" <<
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

void optionsShowAllChordNotesItem::printAllChordNotes (
  ostream&              os,
  msrSemiTonesPitchKind semiTonesPitchKind) const
{  
  os <<
    "xml2ly optionsShowAllChordNotesItem"<<
    endl;
}

void optionsShowAllChordNotesItem::printOptionsValues (
  ostream& os,
  int      valueFieldWidth) const
{
  // nothing to print here
}
                            
ostream& operator<< (ostream& os, const S_optionsShowAllChordNotesItem& elt)
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
          "Chord intervals",
          "heci", "helpExtraChordIntervals",
R"()",
          optionsSubGroup::kAlwaysShowDescription,
          this);
  
    appendOptionsSubGroup (traceAndDisplaySubGroup);
        
    traceAndDisplaySubGroup->
      appendOptionsItem (
        optionsShowAllChordIntervalsItem::create (
          "saci", "showAllChordIntervals",
R"(Write all known chord intervals.)"));
  }
  
  
  // chords notes

  {
    // variables
      
    // options
  
    S_optionsSubGroup
      workSubGroup =
        optionsSubGroup::create (
          "Chord notes",
          "hecn", "helpExtraChordNotes",
R"()",
          optionsSubGroup::kAlwaysShowDescription,
          this);
  
    appendOptionsSubGroup (workSubGroup);
        
    workSubGroup->
      appendOptionsItem (
        optionsShowAllChordNotesItem::create (
          "sacn", "showAllChordNotes",
R"(Show all chord notes for the given diatonic (semitones) pitch
in the current language.)"
));
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
    // show all chord intervals item?
    S_optionsShowAllChordIntervalsItem
      showAllChordIntervalsItem =
        dynamic_cast<optionsShowAllChordIntervalsItem*>(&(*item))
    ) {
    if (TRACE_OPTIONS) {
      os <<
        "==> optionsItem is of type 'optionsShowAllChordIntervalsItem'" <<
        endl;
    }

    // handle it at once
    showAllChordIntervalsItem->
      printAllChordIntervals (os);

    // exit
    exit (0);
  }
  
  else if (
    // show chord notes item?
    S_optionsShowAllChordNotesItem
      showAllChordNotesItem =
        dynamic_cast<optionsShowAllChordNotesItem*>(&(*item))
    ) {
    if (TRACE_OPTIONS) {
      os <<
        "==> optionsItem is of type 'optionsShowAllChordNotesItem'" <<
        endl;
    }

    // wait until the value is met
    result = showAllChordNotesItem;
  }

  return result;
}   

void extraOptions::handleValuedOptionsItem (
  ostream&      os,
  S_optionsItem item,
  string        theString)
{
  os << "FOO" << endl;
  
  if (
    // show chord notes item?
    S_optionsShowAllChordNotesItem
      showAllChordNotesItem =
        dynamic_cast<optionsShowAllChordNotesItem*>(&(*item))
    ) {
    // theString contains the pitch name in the current language
    // is it in the accidental styles map?

    if (TRACE_OPTIONS) {
      os <<
        "==> optionsItem is of type 'optionsShowAllChordNotesItem'" <<
        endl;
    }

    msrQuarterTonesPitchKind
      quarterTonesPitchKindFromString =
        msrQuarterTonesPitchKindFromString (
          gLpsrOptions->
            fLpsrQuarterTonesPitchesLanguageKind,
          theString);

    msrSemiTonesPitchKind
      semiTonesPitchKind =
        semiTonesPitchKindFromQuarterTonesPitchKind (
          quarterTonesPitchKindFromString);

    switch (semiTonesPitchKind) {
      case k_NoQuarterTonesPitch_QTP:
      case k_Rest_QTP:
        {
          stringstream s;
      
          s <<
            "'" << theString <<
            "' is no diatonic  (semitones) pitch" <<
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
          showAllChordNotesItem->
            printAllChordNotes (
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
