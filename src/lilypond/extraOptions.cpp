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


using namespace std;

namespace MusicXML2 
{

#define TRACE_OPTIONS 0

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
S_optionsShowChordNotesItem optionsShowChordNotesItem::create (
  string optionsItemShortName,
  string optionsItemLongName,
  string optionsItemDescription)
{
  optionsShowChordNotesItem* o = new
    optionsShowChordNotesItem (
      optionsItemShortName,
      optionsItemLongName,
      optionsItemDescription);
  assert(o!=0);
  return o;
}

optionsShowChordNotesItem::optionsShowChordNotesItem (
  string optionsItemShortName,
  string optionsItemLongName,
  string optionsItemDescription)
  : optionsItem (
      optionsItemShortName,
      optionsItemLongName,
      optionsItemDescription)
{}

optionsShowChordNotesItem::~optionsShowChordNotesItem ()
{}

void optionsShowChordNotesItem::print (ostream& os) const
{
  const int fieldWidth = K_FIELD_WIDTH;
  
  os <<
    "OptionsShowChordNotesItem:" <<
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

void optionsShowChordNotesItem::printChordNotes (ostream& os) const
{  
  os <<
    "xml2ly printChordNotes"<<
    endl;
}

void optionsShowChordNotesItem::printOptionsValues (
  ostream& os,
  int      valueFieldWidth) const
{
  // nothing to print here
}
                            
ostream& operator<< (ostream& os, const S_optionsShowChordNotesItem& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_optionsShowChordsNotesItem optionsShowChordsNotesItem::create (
  string             optionsItemShortName,
  string             optionsItemLongName,
  string             optionsItemDescription,
  string             optionsValueSpecification,
  string             optionsShowChordsNotesKindItemVariableDisplayName,
  msrSemiTonesPitchKind&
                     optionsShowChordsNotesKindItemVariable)
{
  optionsShowChordsNotesItem* o = new
    optionsShowChordsNotesItem (
      optionsItemShortName,
      optionsItemLongName,
      optionsItemDescription,
      optionsValueSpecification,
      optionsShowChordsNotesKindItemVariableDisplayName,
      optionsShowChordsNotesKindItemVariable);
  assert(o!=0);
  return o;
}

optionsShowChordsNotesItem::optionsShowChordsNotesItem (
  string             optionsItemShortName,
  string             optionsItemLongName,
  string             optionsItemDescription,
  string             optionsValueSpecification,
  string             optionsShowChordsNotesKindItemVariableDisplayName,
  msrSemiTonesPitchKind&
                     optionsShowChordsNotesKindItemVariable)
  : optionsValuedItem (
      optionsItemShortName,
      optionsItemLongName,
      optionsItemDescription,
      optionsValueSpecification),
    fOptionsShowChordsNotesKindItemVariableDisplayName (
      optionsShowChordsNotesKindItemVariableDisplayName),
    fOptionsShowChordsNotesKindItemVariable (
      optionsShowChordsNotesKindItemVariable)
{}

optionsShowChordsNotesItem::~optionsShowChordsNotesItem()
{}

void optionsShowChordsNotesItem::print (ostream& os) const
{
  const int fieldWidth = K_FIELD_WIDTH;
  
  os <<
    "OptionsShowChordsNotesItem:" <<
    endl;

  gIndenter++;

  printValuedItemEssentials (
    os, fieldWidth);

  os << left <<
    setw (fieldWidth) <<
    "fOptionsShowChordsNotesKindItemVariableDisplayName" << " : " <<
    fOptionsShowChordsNotesKindItemVariableDisplayName <<
    endl <<
    setw (fieldWidth) <<
    "fOptionsShowChordsNotesKindItemVariable" << " : \"" <<
    msrSemiTonesPitchKindAsString (
      fOptionsShowChordsNotesKindItemVariable) <<
      "\"" <<
    endl;
}

void optionsShowChordsNotesItem::printOptionsValues (
  ostream& os,
  int      valueFieldWidth) const
{  
  os << left <<
    setw (valueFieldWidth) <<
    fOptionsShowChordsNotesKindItemVariableDisplayName <<
    " : \"" <<
    msrSemiTonesPitchKindAsString (
      fOptionsShowChordsNotesKindItemVariable) <<
    "\"" <<
    endl;
}

ostream& operator<< (ostream& os, const S_optionsShowChordsNotesItem& elt)
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
    
    fShowAllChordIntervals = false;
  
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
    
    fShowChordNotes = false;
  
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
        optionsShowChordNotesItem::create (
          "scn", "showChordNotes",
R"(Show the chord notes for the given chord.)"));
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

  clone->fShowAllChordIntervals =
    false; // JMI


  // chord intervals
  // --------------------------------------
    
  clone->fShowChordNotes =
    fShowChordNotes; // JMI

  return clone;
}  

void extraOptions::printExtraOptionsValues (int fieldWidth)
{  
  gLogIOstream <<
    "The extra options are:" <<
    endl;

  gIndenter++;

  // chord intervals
  // --------------------------------------
  
  gLogIOstream <<
    "Chord intervals:" <<
    endl;

  gIndenter++;

  gLogIOstream << left <<
    setw (fieldWidth) << "showAllChordIntervals" << " : " <<
    booleanAsString (fShowAllChordIntervals) <<
    endl;

  gIndenter--;
      

  // chord intervals
  // --------------------------------------

  gLogIOstream <<
    "Chord notes:" <<
    endl;

  gIndenter++;

  gLogIOstream << left <<
    setw (fieldWidth) << "showChordNotes" << " : " <<
    booleanAsString (fShowChordNotes) <<
     endl;

  gIndenter--;


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
    S_optionsShowChordNotesItem
      showChordNotesItem =
        dynamic_cast<optionsShowChordNotesItem*>(&(*item))
    ) {
    if (TRACE_OPTIONS) {
      os <<
        "==> optionsItem is of type 'optionsShowChordNotesItem'" <<
        endl;
    }

    // handle it at once
    showChordNotesItem->
      printChordNotes (os);

    // exit
    exit (0);
  }

  return result;
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
