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

#include "msr.h"

#include "traceOptions.h"

#include "bsrTraceOptions.h"


using namespace std;

namespace MusicXML2 
{

//_______________________________________________________________________________
S_bsrTraceOptions gBsrTraceOptions;
S_bsrTraceOptions gBsrTraceOptionsUserChoices;
S_bsrTraceOptions gBsrTraceOptionsWithDetailedTrace;

S_bsrTraceOptions bsrTraceOptions::create (
  S_optionsHandler optionsHandler)
{
  bsrTraceOptions* o = new bsrTraceOptions (
    optionsHandler);
  assert(o!=0);

  return o;
}

bsrTraceOptions::bsrTraceOptions (
  S_optionsHandler optionsHandler)
  : optionsGroup (
    "BSR trace",
    "hbt", "help=bsr-trace",
R"(Options that are used by BSR grouped here.)",
    optionsHandler)
{
  // append this options group to the options handler
  // if relevant
  if (optionsHandler) {
    optionsHandler->
      appendOptionsGroup (this);
  }

  // initialize it
  initializeTraceOptions (false);
}

bsrTraceOptions::~bsrTraceOptions ()
{}

void bsrTraceOptions::initializeTraceOptions (
  bool boolOptionsInitialValue)
{
  // pages & lines  
  // --------------------------------------

  {
    // variables  
  
    fTracePages = boolOptionsInitialValue;
    fTraceLines = boolOptionsInitialValue;
    fTraceParallels = boolOptionsInitialValue;

    S_optionsSubGroup
      specificTraceSubGroup =
        optionsSubGroup::create (
          "BSR specific trace",
          "hbst", "help=bsr-specific-trace",
R"(Note: the options in this group imply '-t, -trace-passes'.)",
        optionsSubGroup::kHideDescriptionByDefault,
        this);
  
    appendOptionsSubGroup (specificTraceSubGroup);
    
    specificTraceSubGroup->
      appendOptionsItem (
        optionsTwoBooleansItem::create (
          "tp", "trace-pages",
R"()",
          "tracePages",
          fTracePages,
          gTraceOptions->fTracePasses));
      
    specificTraceSubGroup->
      appendOptionsItem (
        optionsTwoBooleansItem::create (
          "tl", "trace-lines",
R"()",
          "traceLines",
          fTraceLines,
          gTraceOptions->fTracePasses));
            
    specificTraceSubGroup->
      appendOptionsItem (
        optionsTwoBooleansItem::create (
          "tpars", "trace-parallels",
R"()",
          "traceParallels",
          fTraceParallels,
          gTraceOptions->fTracePasses));
  }
}

S_bsrTraceOptions bsrTraceOptions::createCloneWithDetailedTrace ()
{
  S_bsrTraceOptions
    clone =
      bsrTraceOptions::create (0);
      // 0 not to have it inserted twice in the option handler

  // set the options handler uplink
  clone->
    setOptionsHandlerUplink (
      fOptionsHandlerUplink);


  clone->fTracePages = true;
  clone->fTraceLines = true;
  clone->fTraceParallels = true;

  return clone;
}

void bsrTraceOptions::setAllTraceOptions (
  bool boolOptionsInitialValue)
{
  fTracePages = boolOptionsInitialValue;
  fTraceLines = boolOptionsInitialValue;
  fTraceParallels = boolOptionsInitialValue;
 }

//______________________________________________________________________________
void bsrTraceOptions::enforceQuietness ()
{
  setAllTraceOptions (false);
}

//______________________________________________________________________________
void bsrTraceOptions::checkOptionsConsistency ()
{
  // JMI
}

//______________________________________________________________________________
void bsrTraceOptions::printTraceOptionsValues (int fieldWidth)
{  
  gLogIOstream <<
    "The BSR trace options are:" <<
    endl;

  gIndenter++;

  gLogIOstream << left <<
    setw (fieldWidth) << "tracePages" << " : " <<
    booleanAsString (fTracePages) <<
    endl <<
      
    setw (fieldWidth) << "traceLines" << " : " <<
    booleanAsString (fTraceLines) <<
    endl <<
      
    setw (fieldWidth) << "traceParallels" << " : " <<
    booleanAsString (fTraceParallels) <<
    endl;

  gIndenter--;
}

S_optionsItem bsrTraceOptions::handleOptionsItem (
  ostream&      os,
  S_optionsItem item)
{
  S_optionsItem result;
  
  return result;
}

ostream& operator<< (ostream& os, const S_bsrTraceOptions& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
void initializeBsrTraceOptionsHandling (
  S_optionsHandler optionsHandler)
{  
  // create the options variables
  // ------------------------------------------------------
  
  gBsrTraceOptionsUserChoices = bsrTraceOptions::create (
    optionsHandler);
  assert(gTraceOptionsUserChoices != 0);

  gBsrTraceOptions =
    gBsrTraceOptionsUserChoices;

  // prepare for measure detailed trace
  // ------------------------------------------------------

  gBsrTraceOptionsWithDetailedTrace =
    gBsrTraceOptions->
      createCloneWithDetailedTrace ();
}


}
