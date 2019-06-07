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

#include "generalOptions.h"


using namespace std;

namespace MusicXML2
{

//_______________________________________________________________________________
S_generalOptions gGeneralOptions;
S_generalOptions gGeneralOptionsUserChoices;

S_generalOptions generalOptions::create (
  string           executableName,
  S_optionsHandler optionsHandler)
{
  generalOptions* o = new generalOptions (
    executableName,
    optionsHandler);
  assert(o!=0);

  return o;
}

generalOptions::generalOptions (
  string           executableName,
  S_optionsHandler optionsHandler)
  : optionsGroup (
    "General",
    "hg", "help-general",
R"(Options that are used by various components of the library
  are grouped here.)",
    optionsHandler),
    fExecutableName (executableName)
{
  // append this options group to the options handler
  // if relevant
  if (optionsHandler) {
    optionsHandler->
      appendOptionsGroupToHandler (this);
  }

  // initialize it
  initializeGeneralOptions (false);
}

generalOptions::~generalOptions ()
{}

void generalOptions::initializeGeneralHelpOptions (
  bool boolOptionsInitialValue)
{
  // variables

  // options

  S_optionsSubGroup
    helpGeneralOptionsHelpSubGroup =
      optionsSubGroup::create (
        "Options help",
        "hgoh", "help-general-options-help",
R"()",
      optionsSubGroup::kAlwaysShowDescription,
      this);

  appendOptionsSubGroup (helpGeneralOptionsHelpSubGroup);

  helpGeneralOptionsHelpSubGroup->
    appendOptionsItem (
      optionsHelpUsageItem::create (
        "ho", "help-options",
R"(Print options usage help.)"));

  helpGeneralOptionsHelpSubGroup->
    appendOptionsItem (
      optionsHelpSummaryItem::create (
        "hs", "help-summary",
R"(Display a help summary and exit.)"));

  helpGeneralOptionsHelpSubGroup->
    appendOptionsItem (
      optionsItemHelpItem::create (
      "ih", "item-help",
R"(Print help about ITEM_NAME.)",
      "ITEM_NAME"));
}

void generalOptions::initializeGeneralWarningAndErrorsOptions (
  bool boolOptionsInitialValue)
{
  // variables

  fTracePasses               = false;

  fQuiet                     = false;
  fDontShowErrors            = false;
  fDontAbortOnErrors         = false;
  fDisplaySourceCodePosition = false;

  // options

  S_optionsSubGroup
    warningAndErrorHandlingSubGroup =
      optionsSubGroup::create (
        "Warnings and errors",
        "hwae", "help-warnings-and-errors",
R"()",
        optionsSubGroup::kAlwaysShowDescription,
        this);

  appendOptionsSubGroup (warningAndErrorHandlingSubGroup);

  warningAndErrorHandlingSubGroup->
    appendOptionsItem (
      optionsBooleanItem::create (
        "t", "trace-passes",
R"(Write a trace of the passes to standard error.)",
        "tracePasses",
        fTracePasses));

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
        "dse", "dont-show-errors",
R"(Don't show errors in the log.)",
        "dontShowErrors",
        fDontShowErrors));

  warningAndErrorHandlingSubGroup->
    appendOptionsItem (
      optionsBooleanItem::create (
        "daoe", "dont-abort-on-errors",
        replaceSubstringInString (
R"(Do not abort execution on errors and go ahead.
This may be useful when debugging EXECUTABLE.)",
         "EXECUTABLE",
          fExecutableName),
        "dontAbortOnErrors",
        fDontAbortOnErrors));

  warningAndErrorHandlingSubGroup->
    appendOptionsItem (
      optionsBooleanItem::create (
        "dscp", "display-source-code-position",
        replaceSubstringInString (
R"(Display the source code file name and line number
in warning and error messages.
This is useful when debugging EXECUTABLE.)",
         "EXECUTABLE",
          fExecutableName),
        "displaySourceCodePosition",
        fDisplaySourceCodePosition));
}

void generalOptions::initializeGeneralCPUUsageOptions (
  bool boolOptionsInitialValue)
{
  // variables

  fDisplayCPUusage = boolOptionsInitialValue;

  // options

  S_optionsSubGroup
    CPUUsageSubGroup =
      optionsSubGroup::create (
        "CPU usage",
        "hgcpu", "help-general-cpu-usage",
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

void generalOptions::initializeGeneralOptions (
  bool boolOptionsInitialValue)
{
  // register translation date
  // ------------------------------------------------------

  {
    time_t      translationRawtime;
    struct tm*  translationTimeinfo;
    char buffer [80];

    time (&translationRawtime);
    translationTimeinfo = localtime (&translationRawtime);

    strftime (buffer, 80, "%A %F @ %T %Z", translationTimeinfo);
    fTranslationDate = buffer;
  }

  // help
  // --------------------------------------
  initializeGeneralHelpOptions (
    boolOptionsInitialValue);

  // warning and error handling
  // --------------------------------------
  initializeGeneralWarningAndErrorsOptions (
    boolOptionsInitialValue);

  // CPU usage
  // --------------------------------------
  initializeGeneralCPUUsageOptions (
    boolOptionsInitialValue);
}

S_generalOptions generalOptions::createCloneWithTrueValues ()
{
  S_generalOptions
    clone =
      generalOptions::create (
        fExecutableName,
        nullptr);
      // nullptr not to have it inserted twice in the option handler

  // set the options handler uplink
  clone->
    setOptionsHandlerUplink (
      fOptionsHandlerUplink);


  // command line
  // --------------------------------------

  clone->fExecutableName =
    fExecutableName;

  clone->fCommandLineWithLongOptions =
    fCommandLineWithLongOptions;
  clone->fCommandLineWithShortOptions =
    fCommandLineWithShortOptions;


  // passes
  // --------------------------------------
  clone->fTracePasses =
    fTracePasses;

  // warning and error handling
  // --------------------------------------

  clone->fQuiet =
    fQuiet;
  clone->fDontShowErrors =
    fDontShowErrors;
  clone->fDontAbortOnErrors =
    fDontAbortOnErrors;
  clone->fDisplaySourceCodePosition =
    fDisplaySourceCodePosition;


  // CPU usage
  // --------------------------------------

  clone->fDisplayCPUusage = true;


  return clone;
}

  /* JMI
void generalOptions::setAllGeneralTraceOptions (
  bool boolOptionsInitialValue)
{
  // passes
  // --------------------------------------
  fTracePasses = boolOptionsInitialValue;

  // warning and error handling
  // --------------------------------------

  fQuiet = boolOptionsInitialValue;
  fDontShowErrors = boolOptionsInitialValue;
  fDontAbortOnErrors = boolOptionsInitialValue;
  fDisplaySourceCodePosition = boolOptionsInitialValue;

  // CPU usage
  // --------------------------------------

  fDisplayCPUusage = boolOptionsInitialValue;
}
  */

//______________________________________________________________________________
void generalOptions::enforceQuietness ()
{
  fDisplayCPUusage = false; // JMI
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

  // command line
  // --------------------------------------

  gLogIOstream << left <<

  gIndenter++;

  gLogIOstream << left <<
    setw (fieldWidth) << "input source name" << " : " <<
    fInputSourceName <<
    endl <<

    setw (fieldWidth) << "translation date" << " : " <<
    fTranslationDate <<
    endl;

  gIndenter--;

  // passes
  // --------------------------------------

  gLogIOstream <<
    setw (fieldWidth) << "tracePasses" << " : " <<
    booleanAsString (fTracePasses) <<
    endl;

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
    setw (fieldWidth) << "dontShowErrors" << " : " <<
    booleanAsString (fDontShowErrors) <<
    endl <<
    setw (fieldWidth) << "dontAbortOnErrors" << " : " <<
    booleanAsString (fDontAbortOnErrors) <<
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
  string           executableName,
  S_optionsHandler optionsHandler)
{
#ifdef TRACE_OPTIONS
  if (false && ! gGeneralOptions->fQuiet) { // JMI ???
    gLogIOstream <<
      "Initializing general options handling" <<
      endl;
  }
#endif

  // create the options variables
  // ------------------------------------------------------

  gGeneralOptionsUserChoices = generalOptions::create (
    executableName,
    optionsHandler);
  assert(gGeneralOptionsUserChoices != 0);

  gGeneralOptions =
    gGeneralOptionsUserChoices;

  gGeneralOptions->
    checkOptionsConsistency ();
}


}
