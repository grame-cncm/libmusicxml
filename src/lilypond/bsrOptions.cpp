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
    fDisplayBsrDetails    = boolOptionsInitialValue;
      
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
  
#ifdef TRACE_OPTIONS
    traceAndDisplaySubGroup->
      appendOptionsItem (
        optionsTwoBooleansItem::create (
          "dbsrd", "display-bsr-details",
R"(Write the contents of the BSR data with more details to standard error.)",
          "displayBsrDetails",
          fDisplayBsrDetails,
          gTraceOptions->fTracePasses));
#endif
  }
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
  clone->fDisplayBsrDetails =
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

//______________________________________________________________________________
void bsrOptions::enforceQuietness ()
{
  fTraceBsr = false;
  fTraceBsrVisitors = false;
  fDisplayBsr = false;
  fDisplayBsrDetails = false;
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
    endl <<
    
    setw (fieldWidth) << "displayBsrDetails" << " : " <<
    booleanAsString (fDisplayBsrDetails) <<
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
