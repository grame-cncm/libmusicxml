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
  S_oahHandler handlerUpLink)
{
  executableOah* o = new executableOah (
    executableName,
    handlerUpLink);
  assert(o!=0);

  return o;
}

executableOah::executableOah (
  string       executableName,
  S_oahHandler handlerUpLink)
  : oahGroup (
    "Basic",
    "hb", "help-basic",
R"()",
    kElementVisibilityAlways,
    handlerUpLink),
    fHandlerExecutableName (executableName)
{
  // append this options group to the options handler
  // if relevant
  if (handlerUpLink) {
    handlerUpLink->
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
    subGroup =
      oahSubGroup::create (
        "Options help",
        "hboh", "help-basic-options-help",
R"()",
      kElementVisibilityAlways,
      this);

  appendSubGroup (subGroup);

  // help options

  subGroup->
    appendAtom (
      oahOptionsUsageAtom::create (
        "ho", "help-options",
R"(Print options usage help.)"));

  // help summary

  subGroup->
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

  subGroup->
    appendAtom (
      optionNameHelpAtom);
}

void executableOah::initializeExecutableOahAndArgumentsOptions (
  bool boolOptionsInitialValue)
{
  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Options and arguments",
        "oaa", "options-and-arguments",
R"()",
        kElementVisibilityAlways,
        this);

  appendSubGroup (subGroup);

  // options and arguments

  fShowOptionsAndArguments = boolOptionsInitialValue;

  subGroup->
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
    subGroup =
      oahSubGroup::create (
        "Options handling",
        "htoh", "help-trace-options-handling",
R"()",
    kElementVisibilityAlways,
    this);

  appendSubGroup (subGroup);

  // trace options

  fTraceOah = boolOptionsInitialValue; // JMI ECURIE

  subGroup->
    appendAtom (
      oahBooleanAtom::create (
        "toah", "trace-oah",
R"(Write a trace of options and help handling to standard error.
This option should best appear first.)",
        "traceOah",
        fTraceOah));

  // options details

  fTraceOahDetails = boolOptionsInitialValue;

  subGroup->
    appendAtom (
      oahBooleanAtom::create (
        "toptsd", "trace-options-details",
R"(Write a trace of options and help handling with more details to standard error.
This option should best appear first.)",
        "traceOahDetails",
        fTraceOahDetails));

  // trace oah visitors

  fTraceOahVisitors    = boolOptionsInitialValue;

  subGroup->
    appendAtom (
      oahBooleanAtom::create (
        "toahv", "trace-oah-visitors",
R"(Write a trace of the OAH tree visiting activity to standard error.)",
        "traceOahVisitors",
        fTraceOahVisitors));

  // options values

  fDisplayOahValues = boolOptionsInitialValue;

  subGroup->
    appendAtom (
      oahBooleanAtom::create (
        "dov", "display-options-values",
R"(Write the chosen options values to standard error.)",
        "displayOptionsValues",
        fDisplayOahValues));

  // options handler

  fDisplayOptionsHandler = boolOptionsInitialValue;

  subGroup->
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
/* JMI
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

  clone->fTraceOahVisitors =
    fTraceOahVisitors;

  clone->fDisplayOahValues =
    fDisplayOahValues;
  clone->fDisplayOptionsHandler =
    fDisplayOptionsHandler;
#endif

  return clone;
  */
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
void executableOah::acceptIn (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gExecutableOah->fTraceOahVisitors) {
    gLogOstream <<
      "% ==> executableOah::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_executableOah>*
    p =
      dynamic_cast<visitor<S_executableOah>*> (v)) {
        S_executableOah elem = this;

#ifdef TRACE_OAH
        if (gExecutableOah->fTraceOahVisitors) {
          gLogOstream <<
            "% ==> Launching executableOah::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void executableOah::acceptOut (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gExecutableOah->fTraceOahVisitors) {
    gLogOstream <<
      "% ==> executableOah::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_executableOah>*
    p =
      dynamic_cast<visitor<S_executableOah>*> (v)) {
        S_executableOah elem = this;

#ifdef TRACE_OAH
        if (gExecutableOah->fTraceOahVisitors) {
          gLogOstream <<
            "% ==> Launching executableOah::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void executableOah::browseData (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gExecutableOah->fTraceOahVisitors) {
    gLogOstream <<
      "% ==> executableOah::browseData ()" <<
      endl;
  }
#endif
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

    setw (valueFieldWidth) << "traceOahVisitors" << " : " <<
    booleanAsString (fTraceOahVisitors) <<
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
