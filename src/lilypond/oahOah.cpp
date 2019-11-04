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

//______________________________________________________________________________
S_oahOptionalValuesStyleKindAtom oahOptionalValuesStyleKindAtom::create (
  string             shortName,
  string             longName,
  string             description,
  string             valueSpecification,
  string             variableName,
  oahOptionalValuesStyleKind&
                     oahOptionalValuesStyleKindVariable)
{
  oahOptionalValuesStyleKindAtom* o = new
    oahOptionalValuesStyleKindAtom (
      shortName,
      longName,
      description,
      valueSpecification,
      variableName,
      oahOptionalValuesStyleKindVariable);
  assert(o!=0);
  return o;
}

oahOptionalValuesStyleKindAtom::oahOptionalValuesStyleKindAtom (
  string             shortName,
  string             longName,
  string             description,
  string             valueSpecification,
  string             variableName,
  oahOptionalValuesStyleKind&
                     oahOptionalValuesStyleKindVariable)
  : oahValuedAtom (
      shortName,
      longName,
      description,
      valueSpecification,
      variableName),
    fOahOptionalValuesStyleKindVariable (
      oahOptionalValuesStyleKindVariable)
{}

oahOptionalValuesStyleKindAtom::~oahOptionalValuesStyleKindAtom ()
{}

S_oahValuedAtom oahOptionalValuesStyleKindAtom::handleOptionUnderName (
  string   optionName,
  ostream& os)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceOah) {
    gLogOstream <<
      "==> option '" << optionName << "' is a oahOptionalValuesStyleKindAtom" <<
      endl;
  }
#endif

  // an option value is needed
  return this;
}

void oahOptionalValuesStyleKindAtom::handleValue (
  string   theString,
  ostream& os)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceOah) {
    os <<
      "==> oahAtom is of type 'oahOptionalValuesStyleKindAtom'" <<
      endl;
  }
#endif

  // theString contains the language name:
  // is it in the optional values style kinds map?

#ifdef TRACE_OAH
  if (gTraceOah->fTraceOah) {
    os <<
      "==> oahAtom is of type 'oahOptionalValuesStyleKindAtom'" <<
      endl;
  }
#endif

  map<string, oahOptionalValuesStyleKind>::const_iterator
    it =
      gOahOptionalValuesStyleKindsMap.find (
        theString);

  if (it == gOahOptionalValuesStyleKindsMap.end ()) {
    // no, optional values style kind is unknown in the map
    stringstream s;

    s <<
      "OAH optional values style kind '" << theString <<
      "' is unknown" <<
      endl <<
      "The " <<
      gOahOptionalValuesStyleKindsMap.size () - 1 <<
      " known OAH optional values style kind are:" <<
      endl;

    gIndenter++;

    s <<
      existingOahOptionalValuesStyleKinds (K_NAMES_LIST_MAX_LENGTH);

    gIndenter--;

    oahError (s.str ());
  }

  setOahOptionalValuesStyleKindVariable (
    (*it).second);
}

void oahOptionalValuesStyleKindAtom::acceptIn (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    gLogOstream <<
      "% ==> oahOptionalValuesStyleKindAtom::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_oahOptionalValuesStyleKindAtom>*
    p =
      dynamic_cast<visitor<S_oahOptionalValuesStyleKindAtom>*> (v)) {
        S_oahOptionalValuesStyleKindAtom elem = this;

#ifdef TRACE_OAH
        if (gOahOah->fTraceOahVisitors) {
          gLogOstream <<
            "% ==> Launching oahOptionalValuesStyleKindAtom::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void oahOptionalValuesStyleKindAtom::acceptOut (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    gLogOstream <<
      "% ==> oahOptionalValuesStyleKindAtom::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_oahOptionalValuesStyleKindAtom>*
    p =
      dynamic_cast<visitor<S_oahOptionalValuesStyleKindAtom>*> (v)) {
        S_oahOptionalValuesStyleKindAtom elem = this;

#ifdef TRACE_OAH
        if (gOahOah->fTraceOahVisitors) {
          gLogOstream <<
            "% ==> Launching oahOptionalValuesStyleKindAtom::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void oahOptionalValuesStyleKindAtom::browseData (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    gLogOstream <<
      "% ==> oahOptionalValuesStyleKindAtom::browseData ()" <<
      endl;
  }
#endif
}

string oahOptionalValuesStyleKindAtom::asShortNamedOptionString () const
{
  stringstream s;

  s <<
    "-" << fShortName << " " <<
    oahOptionalValuesStyleKindAsString (fOahOptionalValuesStyleKindVariable);

  return s.str ();
}

string oahOptionalValuesStyleKindAtom::asLongNamedOptionString () const
{
  stringstream s;

  s <<
    "-" << fLongName << " " <<
    oahOptionalValuesStyleKindAsString (fOahOptionalValuesStyleKindVariable);

  return s.str ();
}

void oahOptionalValuesStyleKindAtom::print (ostream& os) const
{
  const int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "OptionsOptionalValuesStyleKindAtom:" <<
    endl;

  gIndenter++;

  printValuedAtomEssentials (
    os, fieldWidth);

  os << left <<
    setw (fieldWidth) <<
    "fVariableName" << " : " <<
    fVariableName <<
    endl <<
    setw (fieldWidth) <<
    "fOahOptionalValuesStyleKindVariable" << " : \"" <<
    oahOptionalValuesStyleKindAsString (
      fOahOptionalValuesStyleKindVariable) <<
    "\"" <<
    endl;

  gIndenter--;
}

ostream& operator<< (ostream& os, const S_oahOptionalValuesStyleKindAtom& elt)
{
  elt->print (os);
  return os;
}

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
R"(Write the chosen options values to standard error.
This option is best placed early in the command line)",
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

  // optional values style

  const oahOptionalValuesStyleKind
    oahOptionalValuesStyleKindDefaultValue =
      kOptionalValuesStyleGNU; // default value

  // the variable to set is in the handler actually
  oahOptionalValuesStyleKind&
    optionalValuesStyleKind =
      fHandlerUpLink->
        getHandlerOptionalValuesStyleKind ();

  optionalValuesStyleKind = oahOptionalValuesStyleKindDefaultValue;

  subGroup->
    appendAtomToSubGroup (
      oahOptionalValuesStyleKindAtom::create (
        "ovs", "optional-values-style",
          replaceSubstringInString (
            replaceSubstringInString (
              replaceSubstringInString (
R"(The NUMBER OAH optional values STYLEs available are:
OPTIONAL_VALUES_STYLES.
The default is 'DEFAULT_VALUE'.
In GNU style:
  - the options name alone used the default value;
  - another value can be supplied with '=VALUE' right after the option name,
    without any space in-between.
InOAH style:
  - the option name followed by another option or that is the last one
    in the command line uses the default value;
  - otherwise, the following argument supplies the option value.)",
              "NUMBER",
              to_string (gOahOptionalValuesStyleKindsMap.size ())),
            "OPTIONAL_VALUES_STYLES",
            existingOahOptionalValuesStyleKinds (K_NAMES_LIST_MAX_LENGTH)),
          "DEFAULT_VALUE",
          oahOptionalValuesStyleKindAsString (
            oahOptionalValuesStyleKindDefaultValue)),
        "STYLE",
        "optionalValuesStyleKind",
        optionalValuesStyleKind));
}

#ifdef TRACE_OAH
void oahOah::initializeOahTraceOah (
  bool boolOptionsInitialValue)
{
  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Options visitors trace",
        "htov", "help-trace-options-visitors",
R"()",
    kElementVisibilityAlways,
    this);

  appendSubGroupToGroup (subGroup);

  // trace oah visitors

  fTraceOahVisitors    = boolOptionsInitialValue;

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "toahv", "trace-oah-visitors",
R"(Write a trace of the OAH tree visiting activity to standard error.)",
        "traceOahVisitors",
        fTraceOahVisitors));
}
#endif

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

#ifdef TRACE_OAH
  // trace
  // --------------------------------------
  initializeOahTraceOah (
    boolOptionsInitialValue);
#endif
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

  clone->fCommandLineAsSupplied =
    fCommandLineAsSupplied;
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

  // trace
  // --------------------------------------

  clone->fTraceOahVisitors =
    fTraceOahVisitors;

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
  if (gOahOah->fTraceOahVisitors) {
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
        if (gOahOah->fTraceOahVisitors) {
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
  if (gOahOah->fTraceOahVisitors) {
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
        if (gOahOah->fTraceOahVisitors) {
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
  if (gOahOah->fTraceOahVisitors) {
    gLogOstream <<
      "% ==> oahOah::browseData ()" <<
      endl;
  }
#endif
}

void oahOah::printAtomOptionsValues (
  ostream& os,
  int      valueFieldWidth) const
{
  os << left <<
  /* JMI
    setw (valueFieldWidth) <<
    fVariableName <<
    " : \"" <<
    oahOptionalValuesStyleKindAsString (
      fOahOptionalValuesStyleKindVariable) <<
    "\"" <<
    endl <<
*/

    setw (valueFieldWidth) << "traceOahVisitors" << " : " <<
    booleanAsString (fTraceOahVisitors) <<
    endl;
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
    endl <<

    setw (valueFieldWidth) << "traceOahVisitors" << " : " <<
    booleanAsString (fTraceOahVisitors) <<
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
}


}
