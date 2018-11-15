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

#include "generalOptions.h"


using namespace std;

namespace MusicXML2 
{

//_______________________________________________________________________________
S_generalOptions gGeneralOptions;
S_generalOptions gGeneralOptionsUserChoices;

S_generalOptions generalOptions::create (
  S_optionsHandler optionsHandler)
{
  generalOptions* o = new generalOptions (
    optionsHandler);
  assert(o!=0);

  return o;
}

generalOptions::generalOptions (
  S_optionsHandler optionsHandler)
  : optionsGroup (
    "General",
    "hg", "help=general",
R"(Options that are used by various components of the library
are grouped here.)",
    optionsHandler)
{
  // append this options group to the options handler
  // if relevant
  if (optionsHandler) {
    optionsHandler->
      appendOptionsGroup (this);
  }

  // initialize it
  initializeGeneralOptions (false);
}

generalOptions::~generalOptions ()
{}

void generalOptions::initializeGeneralOptions (
  bool boolOptionsInitialValue)
{
  // help
  // --------------------------------------

  {
    // variables  
    
    // options
  
    S_optionsSubGroup
      helpGeneralOptionsHelpSubGroup =
        optionsSubGroup::create (
          "Options help",
          "hgoh", "help=general-options-help",
R"()",
        optionsSubGroup::kAlwaysShowDescription,
        this);
  
    appendOptionsSubGroup (helpGeneralOptionsHelpSubGroup);
  
    helpGeneralOptionsHelpSubGroup->
      appendOptionsItem (
        optionsHelpUsageItem::create (
          "ho", "help=options",
R"(Print options usage help.)"));

    helpGeneralOptionsHelpSubGroup->
      appendOptionsItem (
        optionsHelpSummaryItem::create (
          "hs", "help=summary",
R"(Display a help summary and exit.)"));
  
    helpGeneralOptionsHelpSubGroup->
      appendOptionsItem (
        optionsItemHelpItem::create (
        "ih", "item-help",
R"(Print help about ITEM_NAME.)",
        "ITEM_NAME"));
  }


  // warning and error handling
  // --------------------------------------

  {
    // variables
    
    fQuiet                     = false;
    fIgnoreErrors              = false;
    fAbortOnErrors             = false;
    fDisplaySourceCodePosition = false;
  
    // options
  
    S_optionsSubGroup
      warningAndErrorHandlingSubGroup =
        optionsSubGroup::create (
          "Warnings and errors",
          "hwae", "help=warnings-and-errors",
R"()",
          optionsSubGroup::kAlwaysShowDescription,
          this);
  
    appendOptionsSubGroup (warningAndErrorHandlingSubGroup);
  
    warningAndErrorHandlingSubGroup->
      appendOptionsItem (
        optionsBooleanItem::create (
          "q", "quiet",
R"(Don't issue any warning or error messages.)",
          "quiet",
          fQuiet));
  
    warningAndErrorHandlingSubGroup->
      appendOptionsItem (
        optionsBooleanItem::create (
          "i", "ignore-errors",
R"(Ignore errors and proceed.)",
          "ignoreErrors",
          fIgnoreErrors));
  
    warningAndErrorHandlingSubGroup->
      appendOptionsItem (
        optionsBooleanItem::create (
          "aoe", "abort-on-errors",
R"(Abort execution on errors instead of gracefully exiting.
This is useful when debugging xml2ly/xml2brl.)",
          "abortOnErrors",
          fAbortOnErrors));
  
    warningAndErrorHandlingSubGroup->
      appendOptionsItem (
        optionsBooleanItem::create (
          "dscp", "display-source-code-position",
R"(Display the source code file name and line number
in warning and error messages.
This is useful when debugging xml2ly/xml2brl.)",
          "displaySourceCodePosition",
          fDisplaySourceCodePosition));
  }


  // CPU usage
  // --------------------------------------

  {
    // variables  
  
    fDisplayCPUusage = boolOptionsInitialValue;
      
    // options
  
    S_optionsSubGroup
      CPUUsageSubGroup =
        optionsSubGroup::create (
          "CPU usage",
          "hgcpu", "help=general-cpu-usage",
R"()",
        optionsSubGroup::kAlwaysShowDescription,
        this);
  
    appendOptionsSubGroup (CPUUsageSubGroup);
        
    CPUUsageSubGroup->
      appendOptionsItem (
        optionsBooleanItem::create (
          "dcpuu", "display-cpu-usage",
R"(Write information about CPU usage to standard error.)",
          "displayCPUusage",
          fDisplayCPUusage));
  }
}

S_generalOptions generalOptions::createCloneWithTrueValues ()
{
  S_generalOptions
    clone =
      generalOptions::create (0);
      // 0 not to have it inserted twice in the option handler

  // set the options handler uplink
  clone->
    setOptionsHandlerUplink (
      fOptionsHandlerUplink);


  // command line
  // --------------------------------------

  clone->fProgramName =
    fProgramName;
      
  clone->fCommandLineWithLongOptions =
    fCommandLineWithLongOptions;
  clone->fCommandLineWithShortOptions =
    fCommandLineWithShortOptions;

  
  // display
  // --------------------------------------


  // warning and error handling
  // --------------------------------------

  clone->fQuiet =
    fQuiet;
  clone->fIgnoreErrors =
    fIgnoreErrors;
  clone->fAbortOnErrors =
    fAbortOnErrors;
  clone->fDisplaySourceCodePosition =
    fDisplaySourceCodePosition;

    
  // CPU usage
  // --------------------------------------

  clone->fDisplayCPUusage = true;


  return clone;
}

//______________________________________________________________________________
void generalOptions::enforceQuietness ()
{
  fDisplayCPUusage = false;
}

//______________________________________________________________________________
void generalOptions::checkOptionsConsistency ()
{
  // JMI
}

//______________________________________________________________________________
void generalOptions::printGeneralOptionsValues (int fieldWidth)
{  
  gLogIOstream <<
    "The general options are:" <<
    endl;

  gIndenter++;

  // warning and error handling
  // --------------------------------------
  
  gLogIOstream << left <<
    setw (fieldWidth) << "Warning and error handling:" <<
    endl;

  gIndenter++;

  gLogIOstream <<
    setw (fieldWidth) << "quiet" << " : " <<
    booleanAsString (fQuiet) <<
    endl <<
    setw (fieldWidth) << "ignoreErrors" << " : " <<
    booleanAsString (fIgnoreErrors) <<
    endl <<
    setw (fieldWidth) << "abortOnErrors" << " : " <<
    booleanAsString (fAbortOnErrors) <<
    endl <<
    setw (fieldWidth) << "displaySourceCodePosition" << " : " <<
    booleanAsString (fDisplaySourceCodePosition) <<
    endl;

  gIndenter--;
  

  // CPU usage
  // --------------------------------------

  gLogIOstream << left <<
    setw (fieldWidth) << "CPU usage:" <<
    endl;

  gIndenter++;

  gLogIOstream <<
    setw (fieldWidth) << "displayCPUusage" << " : " <<
    booleanAsString (fDisplayCPUusage) <<
    endl;

  gIndenter--;
  
  gIndenter--;

  gIndenter--;
}

S_optionsItem generalOptions::handleOptionsItem (
  ostream&      os,
  S_optionsItem item)
{
  S_optionsItem result;
  
  return result;
}

ostream& operator<< (ostream& os, const S_generalOptions& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
void initializeGeneralOptionsHandling (
  S_optionsHandler optionsHandler)
{  
  // create the options variables
  // ------------------------------------------------------
  
  gGeneralOptionsUserChoices = generalOptions::create (
    optionsHandler);
  assert(gGeneralOptionsUserChoices != 0);

  gGeneralOptions =
    gGeneralOptionsUserChoices;

  gGeneralOptions->
    checkOptionsConsistency ();
}


}
