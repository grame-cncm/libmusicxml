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

#include "oahOah.h"


using namespace std;

namespace MusicXML2
{

//_______________________________________________________________________________
S_oahOah gOahOah;
S_oahOah gOahOahUserChoices;

S_oahOah oahOah::create (
  string       executableName,
  S_oahHandler handlerUpLink)
{
  oahOah* o = new oahOah (
    executableName,
    handlerUpLink);
  assert(o!=0);

  return o;
}

oahOah::oahOah (
  string       executableName,
  S_oahHandler handlerUpLink)
  : oahGroup (
    "Options and help",
    "hoah", "help-options-and-help",
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
  initializeOahOah (false);
}

oahOah::~oahOah ()
{}

void oahOah::initializeOahBasicHelpOptions (
  bool boolOptionsInitialValue)
{
  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Options help",
        "hoh", "help-options-help",
R"()",
      kElementVisibilityAlways,
      this);

  appendSubGroupToGroup (subGroup);

  // help options

  subGroup->
    appendAtomToSubGroup (
      oahOptionsUsageAtom::create (
        "ho", "help-options",
R"(Print options usage help.)"));

  // help summary

  subGroup->
    appendAtomToSubGroup (
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
    appendAtomToSubGroup (
      optionNameHelpAtom);

  // options values

  fDisplayOahValues = boolOptionsInitialValue;

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "dov", "display-options-values",
R"(Write the chosen options values to standard error.)",
        "displayOptionsValues",
        fDisplayOahValues));

  // options handler

  fDisplayOptionsHandler = boolOptionsInitialValue;

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "doh", "display-options-handler",
R"(Write the contents of the options handler to standard error.)",
        "displayOptionsHandler",
        fDisplayOptionsHandler));
}

void oahOah::initializeOahOahAndArgumentsOptions (
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

  appendSubGroupToGroup (subGroup);

  // options and arguments

  fShowOptionsAndArguments = boolOptionsInitialValue;

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "soaa", "show-options-and-arguments",
        replaceSubstringInString (
R"(Print the options and arguments to EXECUTABLE.)",
          "EXECUTABLE",
          fHandlerExecutableName),
        "showOptionsAndArguments",
        fShowOptionsAndArguments));
}

void oahOah::initializeOahOah (
  bool boolOptionsInitialValue)
{
  // help
  // --------------------------------------
  initializeOahBasicHelpOptions (
    boolOptionsInitialValue);

  // options and arguments
  // --------------------------------------
  initializeOahOahAndArgumentsOptions (
    boolOptionsInitialValue);
}

S_oahOah oahOah::createCloneWithTrueValues ()
{
  S_oahOah
    clone =
      oahOah::create (
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

  // options and help display
  // --------------------------------------

  clone->fDisplayOahValues =
    fDisplayOahValues;
  clone->fDisplayOptionsHandler =
    fDisplayOptionsHandler;

  return clone;
}

//______________________________________________________________________________
void oahOah::enforceQuietness ()
{
  // JMI
}

//______________________________________________________________________________
void oahOah::checkOptionsConsistency ()
{
  // JMI
}

//______________________________________________________________________________
void oahOah::acceptIn (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceOahVisitors) {
    gLogOstream <<
      "% ==> oahOah::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_oahOah>*
    p =
      dynamic_cast<visitor<S_oahOah>*> (v)) {
        S_oahOah elem = this;

#ifdef TRACE_OAH
        if (gTraceOah->fTraceOahVisitors) {
          gLogOstream <<
            "% ==> Launching oahOah::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void oahOah::acceptOut (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceOahVisitors) {
    gLogOstream <<
      "% ==> oahOah::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_oahOah>*
    p =
      dynamic_cast<visitor<S_oahOah>*> (v)) {
        S_oahOah elem = this;

#ifdef TRACE_OAH
        if (gTraceOah->fTraceOahVisitors) {
          gLogOstream <<
            "% ==> Launching oahOah::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void oahOah::browseData (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceOahVisitors) {
    gLogOstream <<
      "% ==> oahOah::browseData ()" <<
      endl;
  }
#endif
}

//______________________________________________________________________________
void oahOah::printOahOahValues (int valueFieldWidth)
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

  // options and help display
  // --------------------------------------

  gLogOstream << left <<
    setw (valueFieldWidth) << "Options trace and display:" <<
    endl;

  gIndenter++;

  gLogOstream << left <<
    setw (valueFieldWidth) << "displayOptionsValues" << " : " <<
    booleanAsString (fDisplayOahValues) <<
    endl <<
    setw (valueFieldWidth) << "displayOptionsHandler" << " : " <<
    booleanAsString (fDisplayOptionsHandler) <<
    endl;

  gIndenter--;

  gIndenter--;
}

ostream& operator<< (ostream& os, const S_oahOah& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
void initializeOahOahHandling (
  string       executableName,
  S_oahHandler handler)
{
  // create the options variables
  // ------------------------------------------------------

  gOahOahUserChoices = oahOah::create (
    executableName,
    handler);
  assert(gOahOahUserChoices != 0);

  gOahOah =
    gOahOahUserChoices;

  gOahOah->
    checkOptionsConsistency ();
}


}
