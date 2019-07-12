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

#include "setTraceOptionsIfDesired.h"
#ifdef TRACE_OPTIONS
  #include "traceOptions.h"
#endif

#include "oahBasicOptions.h"


using namespace std;

namespace MusicXML2
{

//_______________________________________________________________________________
S_oahBasicOptions gOahBasicOptions;
S_oahBasicOptions gOahBasicOptionsUserChoices;

S_oahBasicOptions oahBasicOptions::create (
  string       executableName,
  S_oahHandler handler)
{
  oahBasicOptions* o = new oahBasicOptions (
    executableName,
    handler);
  assert(o!=0);

  return o;
}

oahBasicOptions::oahBasicOptions (
  string       executableName,
  S_oahHandler handler)
  : oahGroup (
    "Basic",
    "hb", "help-basic",
R"()",
    kElementVisibilityAlways,
    handler),
    fHandlerExecutableName (executableName)
{
  // append this options group to the options handler
  // if relevant
  if (handler) {
    handler->
      appendGroupToHandler (this);
  }

  // initialize it
  initializeOahBasicOptions (false);
}

oahBasicOptions::~oahBasicOptions ()
{}

void oahBasicOptions::initializeOahBasicHelpOptions (
  bool boolOptionsInitialValue)
{
  S_oahSubGroup
    helpOahBasicOptionsHelpSubGroup =
      oahSubGroup::create (
        "Options help",
        "hboh", "help-basic-options-help",
R"()",
      kElementVisibilityAlways,
      this);

  appendSubGroup (helpOahBasicOptionsHelpSubGroup);

  // help options

  helpOahBasicOptionsHelpSubGroup->
    appendAtom (
      oahOptionsUsageAtom::create (
        "ho", "help-options",
R"(Print options usage help.)"));

  // help summary

  helpOahBasicOptionsHelpSubGroup->
    appendAtom (
      oahOptionsSummaryAtom::create (
        "hs", "help-summary",
R"(Display a help summary and exit.)"));

  // option name help

  string defaultOptionName = "onh";

  fOptionName = defaultOptionName;

  S_oahOptionNameHelpAtom
    optionNameHelpAtom =
      oahOptionNameHelpAtom::create (
        "onh", "option-name-help",
        replaceSubstringInString (
R"(Print help about OPTION_NAME.
OPTION_NAME is optional, and the default value is 'DEFAULT_VALUE'.)",
          "DEFAULT_VALUE",
          defaultOptionName),
        "[OPTION_NAME]",
        "optionName",
        fOptionName,
        defaultOptionName);

  optionNameHelpAtom->
    setValueIsOptional ();

  helpOahBasicOptionsHelpSubGroup->
    appendAtom (
      optionNameHelpAtom);
}

void oahBasicOptions::initializeOahBasicOptionsAndArgumentsOptions (
  bool boolOptionsInitialValue)
{
  S_oahSubGroup
    optionsAndArgumentsSubGroup =
      oahSubGroup::create (
        "Options and arguments",
        "oaa", "options-and-arguments",
R"()",
        kElementVisibilityAlways,
        this);

  appendSubGroup (optionsAndArgumentsSubGroup);

  // options and arguments

  fShowOptionsAndArguments = boolOptionsInitialValue;

  optionsAndArgumentsSubGroup->
    appendAtom (
      oahBooleanAtom::create (
        "soaa", "show-options-and-arguments",
        replaceSubstringInString (
R"(Print the options and arguments to EXECUTABLE.)",
          "EXECUTABLE",
          fHandlerExecutableName),
        "showOptionsAndArguments",
        fShowOptionsAndArguments));
}

#ifdef TRACE_OPTIONS
void oahBasicOptions::initializeOptionsTraceAndDisplayOptions (
  bool boolOptionsInitialValue)
{
  S_oahSubGroup
    traceSubGroup =
      oahSubGroup::create (
        "Options handling",
        "htoh", "help-trace-options-handling",
R"()",
    kElementVisibilityAlways,
    this);

  appendSubGroup (traceSubGroup);

  // trace options

  fTraceOptions = boolOptionsInitialValue; // JMI ECURIE

  traceSubGroup->
    appendAtom (
      oahBooleanAtom::create (
        "topts", "trace-options",
R"(Write a trace of options handling to standard error.
This option should best appear first.)",
        "traceOptions",
        fTraceOptions));

  // options details

  fTraceOptionsDetails = boolOptionsInitialValue;

  traceSubGroup->
    appendAtom (
      oahBooleanAtom::create (
        "toptsd", "trace-options-details",
R"(Write a trace of options handling with more details to standard error.
This option should best appear first.)",
        "traceOptionsDetails",
        fTraceOptionsDetails));

  // options values

  fDisplayOptionsValues = boolOptionsInitialValue;

  traceSubGroup->
    appendAtom (
      oahBooleanAtom::create (
        "dov", "display-options-values",
R"(Write the chosen options values to standard error.)",
        "displayOptionsValues",
        fDisplayOptionsValues));

  // options handler

  fDisplayOptionsHandler = boolOptionsInitialValue;

  traceSubGroup->
    appendAtom (
      oahBooleanAtom::create (
        "doh", "display-options-handler",
R"(Write the contents of the options handler to standard error.)",
        "displayOptionsHandler",
        fDisplayOptionsHandler));

  // fTraceDetailedMeasureNumbersSet is empty
}
#endif

void oahBasicOptions::initializeOahBasicOptions (
  bool boolOptionsInitialValue)
{
  // help
  // --------------------------------------
  initializeOahBasicHelpOptions (
    boolOptionsInitialValue);

  // options and arguments
  // --------------------------------------
  initializeOahBasicOptionsAndArgumentsOptions (
    boolOptionsInitialValue);

#ifdef TRACE_OPTIONS
  initializeOptionsTraceAndDisplayOptions (
    boolOptionsInitialValue);
#endif
}

S_oahBasicOptions oahBasicOptions::createCloneWithTrueValues ()
{
  S_oahBasicOptions
    clone =
      oahBasicOptions::create (
        fHandlerExecutableName,
        nullptr);
      // nullptr not to have it inserted twice in the option handler

  // set the options handler upLink
  clone->fHandlerUpLink =
    fHandlerUpLink;

  // command line
  // --------------------------------------

  clone->fHandlerExecutableName =
    fHandlerExecutableName;

  clone->fShowOptionsAndArguments =
    fShowOptionsAndArguments;

  clone->fCommandLineWithShortOptionsNames =
    fCommandLineWithShortOptionsNames;
  clone->fCommandLineWithLongOptionsNames =
    fCommandLineWithLongOptionsNames;

#ifdef TRACE_OPTIONS
  // options trace and display
  // --------------------------------------

  clone->fTraceOptions =
    fTraceOptions;
  clone->fTraceOptionsDetails =
    fTraceOptionsDetails;

  clone->fDisplayOptionsValues =
    fDisplayOptionsValues;
  clone->fDisplayOptionsHandler =
    fDisplayOptionsHandler;
#endif

  return clone;
}

//______________________________________________________________________________
void oahBasicOptions::enforceQuietness ()
{
  // JMI
}

//______________________________________________________________________________
void oahBasicOptions::checkOptionsConsistency ()
{
  // JMI
}

//______________________________________________________________________________
void oahBasicOptions::printOahBasicOptionsValues (int valueFieldWidth)
{
  gLogOstream <<
    "The basic options are:" <<
    endl;

  gIndenter++;

  // command line
  // --------------------------------------

  gLogOstream << left <<
    setw (valueFieldWidth) << "Command line:" <<
    endl;

  gIndenter++;

  gLogOstream << left <<
    setw (valueFieldWidth) << "inputSourceName" << " : " <<
    fInputSourceName <<
    endl <<

    setw (valueFieldWidth) << "showOptionsAndArguments" << " : " <<
    booleanAsString (fShowOptionsAndArguments) <<
    endl;

  gIndenter--;

#ifdef TRACE_OPTIONS
  // options trace and display
  // --------------------------------------

  gLogOstream << left <<
    setw (valueFieldWidth) << "Options trace and display:" <<
    endl;

  gIndenter++;

  gLogOstream << left <<
    setw (valueFieldWidth) << "traceOptions" << " : " <<
    booleanAsString (fTraceOptions) <<
    endl <<
    setw (valueFieldWidth) << "traceOptionsDetails" << " : " <<
    booleanAsString (fTraceOptionsDetails) <<
    endl <<

    setw (valueFieldWidth) << "displayOptionsValues" << " : " <<
    booleanAsString (fDisplayOptionsValues) <<
    endl <<
    setw (valueFieldWidth) << "displayOptionsHandler" << " : " <<
    booleanAsString (fDisplayOptionsHandler) <<
    endl;

  gIndenter--;
#endif

  gIndenter--;
}

S_oahValuedAtom oahBasicOptions::handleAtom (
  ostream&  os,
  S_oahAtom atom)
{
  S_oahValuedAtom result;

  // JMI ???

  return result;
}

ostream& operator<< (ostream& os, const S_oahBasicOptions& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
void initializeOahBasicOptionsHandling (
  string       executableName,
  S_oahHandler handler)
{
  // create the options variables
  // ------------------------------------------------------

  gOahBasicOptionsUserChoices = oahBasicOptions::create (
    executableName,
    handler);
  assert(gOahBasicOptionsUserChoices != 0);

  gOahBasicOptions =
    gOahBasicOptionsUserChoices;

  gOahBasicOptions->
    checkOptionsConsistency ();
}


}
