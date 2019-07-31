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

#include "setTraceOahIfDesired.h"
#ifdef TRACE_OAH
  #include "traceOah.h"
#endif

#include "executableOah.h"


using namespace std;

namespace MusicXML2
{

//_______________________________________________________________________________
S_executableOah gExecutableOah;
S_executableOah gExecutableOahUserChoices;

S_executableOah executableOah::create (
  string       executableName,
  S_oahHandler handler)
{
  executableOah* o = new executableOah (
    executableName,
    handler);
  assert(o!=0);

  return o;
}

executableOah::executableOah (
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
  initializeExecutableOah (false);
}

executableOah::~executableOah ()
{}

void executableOah::initializeOahBasicHelpOptions (
  bool boolOptionsInitialValue)
{
  S_oahSubGroup
    helpExecutableOahHelpSubGroup =
      oahSubGroup::create (
        "Options help",
        "hboh", "help-basic-options-help",
R"()",
      kElementVisibilityAlways,
      this);

  appendSubGroup (helpExecutableOahHelpSubGroup);

  // help options

  helpExecutableOahHelpSubGroup->
    appendAtom (
      oahOptionsUsageAtom::create (
        "ho", "help-options",
R"(Print options usage help.)"));

  // help summary

  helpExecutableOahHelpSubGroup->
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

  helpExecutableOahHelpSubGroup->
    appendAtom (
      optionNameHelpAtom);
}

void executableOah::initializeExecutableOahAndArgumentsOptions (
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

#ifdef TRACE_OAH
void executableOah::initializeOptionsTraceAndDisplayOptions (
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

  fTraceOah = boolOptionsInitialValue; // JMI ECURIE

  traceSubGroup->
    appendAtom (
      oahBooleanAtom::create (
        "toah", "trace-oah",
R"(Write a trace of options and help handling to standard error.
This option should best appear first.)",
        "traceOah",
        fTraceOah));

  // options details

  fTraceOahDetails = boolOptionsInitialValue;

  traceSubGroup->
    appendAtom (
      oahBooleanAtom::create (
        "toptsd", "trace-options-details",
R"(Write a trace of options and help handling with more details to standard error.
This option should best appear first.)",
        "traceOahDetails",
        fTraceOahDetails));

  // options values

  fDisplayOahValues = boolOptionsInitialValue;

  traceSubGroup->
    appendAtom (
      oahBooleanAtom::create (
        "dov", "display-options-values",
R"(Write the chosen options values to standard error.)",
        "displayOptionsValues",
        fDisplayOahValues));

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

void executableOah::initializeExecutableOah (
  bool boolOptionsInitialValue)
{
  // help
  // --------------------------------------
  initializeOahBasicHelpOptions (
    boolOptionsInitialValue);

  // options and arguments
  // --------------------------------------
  initializeExecutableOahAndArgumentsOptions (
    boolOptionsInitialValue);

#ifdef TRACE_OAH
  initializeOptionsTraceAndDisplayOptions (
    boolOptionsInitialValue);
#endif
}

S_executableOah executableOah::createCloneWithTrueValues ()
{
  S_executableOah
    clone =
      executableOah::create (
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

#ifdef TRACE_OAH
  // options trace and display
  // --------------------------------------

  clone->fTraceOah =
    fTraceOah;
  clone->fTraceOahDetails =
    fTraceOahDetails;

  clone->fDisplayOahValues =
    fDisplayOahValues;
  clone->fDisplayOptionsHandler =
    fDisplayOptionsHandler;
#endif

  return clone;
}

//______________________________________________________________________________
void executableOah::enforceQuietness ()
{
  // JMI
}

//______________________________________________________________________________
void executableOah::checkOptionsConsistency ()
{
  // JMI
}

//______________________________________________________________________________
void executableOah::printExecutableOahValues (int valueFieldWidth)
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

#ifdef TRACE_OAH
  // options trace and display
  // --------------------------------------

  gLogOstream << left <<
    setw (valueFieldWidth) << "Options trace and display:" <<
    endl;

  gIndenter++;

  gLogOstream << left <<
    setw (valueFieldWidth) << "traceOah" << " : " <<
    booleanAsString (fTraceOah) <<
    endl <<
    setw (valueFieldWidth) << "traceOahDetails" << " : " <<
    booleanAsString (fTraceOahDetails) <<
    endl <<

    setw (valueFieldWidth) << "displayOptionsValues" << " : " <<
    booleanAsString (fDisplayOahValues) <<
    endl <<
    setw (valueFieldWidth) << "displayOptionsHandler" << " : " <<
    booleanAsString (fDisplayOptionsHandler) <<
    endl;

  gIndenter--;
#endif

  gIndenter--;
}

ostream& operator<< (ostream& os, const S_executableOah& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
void initializeExecutableOahHandling (
  string       executableName,
  S_oahHandler handler)
{
  // create the options variables
  // ------------------------------------------------------

  gExecutableOahUserChoices = executableOah::create (
    executableName,
    handler);
  assert(gExecutableOahUserChoices != 0);

  gExecutableOah =
    gExecutableOahUserChoices;

  gExecutableOah->
    checkOptionsConsistency ();
}


}
