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
#include <regex>

#include "utilities.h"
#include "messagesHandling.h"

#include "enableTracingIfDesired.h"
#ifdef TRACING_IS_ENABLED
  #include "traceOah.h"
#endif

#include "oahOah.h"


using namespace std;

namespace MusicXML2
{

//______________________________________________________________________________
S_insiderOahAtom insiderOahAtom::create (
  string shortName,
  string longName,
  string description)
{
  insiderOahAtom* o = new
    insiderOahAtom (
      shortName,
      longName,
      description);
  assert (o != nullptr);
  return o;
}

insiderOahAtom::insiderOahAtom (
  string shortName,
  string longName,
  string description)
  : oahAtom (
      shortName,
      longName,
      description,
      kElementWithoutValue)
{}

insiderOahAtom::~insiderOahAtom ()
{}

void insiderOahAtom::applyElement (ostream& os)
{

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceOah ()) {
    gLogStream <<
      "==> option '" << fetchNames () << "' is a insiderOahAtom" <<
      endl;
  }
#endif

  // do nothing, choosing the the insider OAH handler has been done in xml2xml.cpp
}

void insiderOahAtom::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> insiderOahAtom::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_insiderOahAtom>*
    p =
      dynamic_cast<visitor<S_insiderOahAtom>*> (v)) {
        S_insiderOahAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching insiderOahAtom::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void insiderOahAtom::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> insiderOahAtom::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_insiderOahAtom>*
    p =
      dynamic_cast<visitor<S_insiderOahAtom>*> (v)) {
        S_insiderOahAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching insiderOahAtom::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void insiderOahAtom::browseData (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> insiderOahAtom::browseData ()" <<
      endl;
  }
#endif
}

void insiderOahAtom::print (ostream& os) const
{
  const unsigned int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "insiderOahAtom:" <<
    endl;

  ++gIndenter;

  oahElement::printOahElementEssentials (
    os, fieldWidth);

  --gIndenter;
}

ostream& operator<< (ostream& os, const S_insiderOahAtom& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_regularOahAtom regularOahAtom::create (
  string shortName,
  string longName,
  string description)
{
  regularOahAtom* o = new
    regularOahAtom (
      shortName,
      longName,
      description);
  assert (o != nullptr);
  return o;
}

regularOahAtom::regularOahAtom (
  string shortName,
  string longName,
  string description)
  : oahAtom (
      shortName,
      longName,
      description,
      kElementWithoutValue)
{}

regularOahAtom::~regularOahAtom ()
{}

void regularOahAtom::applyElement (ostream& os)
{

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceOah ()) {
    gLogStream <<
      "==> option '" << fetchNames () << "' is a regularOahAtom" <<
      endl;
  }
#endif

  // do nothing, choosing the the regular OAH handler has been done in xml2xml.cpp
}

void regularOahAtom::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> regularOahAtom::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_regularOahAtom>*
    p =
      dynamic_cast<visitor<S_regularOahAtom>*> (v)) {
        S_regularOahAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching regularOahAtom::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void regularOahAtom::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> regularOahAtom::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_regularOahAtom>*
    p =
      dynamic_cast<visitor<S_regularOahAtom>*> (v)) {
        S_regularOahAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching regularOahAtom::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void regularOahAtom::browseData (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> regularOahAtom::browseData ()" <<
      endl;
  }
#endif
}

void regularOahAtom::print (ostream& os) const
{
  const unsigned int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "regularOahAtom:" <<
    endl;

  ++gIndenter;

  oahElement::printOahElementEssentials (
    os, fieldWidth);

  --gIndenter;
}

ostream& operator<< (ostream& os, const S_regularOahAtom& elt)
{
  elt->print (os);
  return os;
}

//_______________________________________________________________________________
S_oahOahGroup gGlobalOahOahGroup;

S_oahOahGroup oahOahGroup::create (
  string executableName)
{
  oahOahGroup* o = new oahOahGroup (
    executableName);
  assert (o != nullptr);

  return o;
}

oahOahGroup::oahOahGroup (
  string executableName)
  : oahGroup (
      "Options and help",
      "hoah", "help-options-and-help",
  R"()",
      kElementVisibilityWhole),
    fHandlerExecutableName (
      executableName)
{
  initializeOahOahGroup (executableName);
}

oahOahGroup::~oahOahGroup ()
{}

void oahOahGroup::initializeOahBasicHelpOptions (
  string executableName)
{
  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Options help",
        "hoh", "help-options-help",
R"()",
      kElementVisibilityWhole,
      this);

  appendSubGroupToGroup (subGroup);

  // the 'regular' option

  subGroup->
    appendAtomToSubGroup (
      regularOahAtom::create (
        K_REGULAR_OPTION_NAME, "",
R"(Use the 'regular' view for the options and help,
in which the options are grouped by topics
such a slurs, tuplets and figured bass.
This is the default.)"));

  // the 'insider' option

  subGroup->
    appendAtomToSubGroup (
      insiderOahAtom::create (
        K_INSIDER_OPTION_NAME, "",
R"(Use the 'insider' view for the options and help,
in which the options are grouped as they are used by the various
internal representations and translation passes.
This unleashes the full set of display and trace options.)"));

  // help

  subGroup->
    appendAtomToSubGroup (
      oahHelpAtom::create (
        "h", "help",
R"(Display xml2xml's full help.)",
        executableName));

  // help options usage

  subGroup->
    appendAtomToSubGroup (
      oahOptionsUsageAtom::create (
        "hou", "help-options-usage",
R"(Print options usage help.)",
        executableName));

  // help summary

  subGroup->
    appendAtomToSubGroup (
      oahHelpSummaryAtom::create (
        "hs", "help-summary",
R"(Display a help summary and quit.)",
        executableName));

  // about

  subGroup->
    appendAtomToSubGroup (
      oahAboutAtom::create (
        "a", "about",
R"(Display information about xml2ly.)",
        executableName));

  // version

  subGroup->
    appendAtomToSubGroup (
      oahVersionAtom::create (
        "v", "version",
R"(Display xml2ly's version number and history.)",
        executableName));

  // contact

  subGroup->
    appendAtomToSubGroup (
      oahContactAtom::create (
        "c", "contact",
R"(Display information about how to contacct xml2ly maintainers.)",
        executableName));

  // help about option name

  string defaultOptionName = "name-help";

  fOptionNameToProvideHelpAbout = defaultOptionName;

  fOptionNameHelpAtom =
    oahOptionNameHelpAtom::create (
      "nh", defaultOptionName,
      regex_replace (
R"(Print help about OPTION_NAME.
OPTION_NAME is optional, and the default value is 'DEFAULT_VALUE'.)",
        regex ("DEFAULT_VALUE"),
        defaultOptionName),
      "OPTION_NAME",
      "optionName",
      fOptionNameToProvideHelpAbout,
      defaultOptionName);

  subGroup->
    appendAtomToSubGroup (
      fOptionNameHelpAtom);

  // apropos about option name

  fAProposOptionNameAtom =
    oahAProposOptionNameAtom::create (
      "apropos", "apropos-option-name",
R"(Print help about OPTION_NAME.)",
      "OPTION_NAME",
      "nameToProvideAProposAbout",
      fOptionNameToProvideAProposAbout);

  subGroup->
    appendAtomToSubGroup (
      fAProposOptionNameAtom);

  // find string

  fFindStringAtom =
    oahFindStringAtom::create (
      "find", "find-string",
R"(Find string STRING in the help.
The search is case insensitive, and a '-' is added in front of options name for clarity.)",
      "STRING",
      "stringToBeFound",
      fStringToBeFound);

  subGroup->
    appendAtomToSubGroup (
      fFindStringAtom);

  // display options values

  fDisplayOahValues = false;

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "dov", "display-options-values",
R"(Write the chosen options values to standard error.
This option is best placed early in the command line)",
        "displayOptionsValues",
        fDisplayOahValues));

  // display options handler

  fDisplayOahHandler = false;

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "doh", "display-options-handler",
R"(Write the contents of the options handler to standard error.)",
        "displayOahHandler",
        fDisplayOahHandler));

  // display options handler summary

  fDisplayOahHandlerSummary = false;

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "dohs", "display-options-handler-summary",
R"(Write a summary of the contents of the options handler to standard error.)",
        "displayOahHandler",
        fDisplayOahHandlerSummary));

  // display options handler essentials

  fDisplayOahHandlerEssentials = false;

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "dohe", "display-options-handler-essentials",
R"(Write the essentials of the contents of the options handler to standard error.)",
        "displayOahHandler",
        fDisplayOahHandlerEssentials));
}

void oahOahGroup::initializeOahOahAndArgumentsOptions ()
{
  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Options and arguments",
        "oaa", "options-and-arguments",
R"()",
        kElementVisibilityWhole,
        this);

  appendSubGroupToGroup (subGroup);

  // options and arguments

  fShowOptionsAndArguments = false;

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "soaa", "show-options-and-arguments",
        regex_replace (
R"(Print the options and arguments to EXECUTABLE.)",
          regex ("EXECUTABLE"),
          fHandlerExecutableName),
        "showOptionsAndArguments",
        fShowOptionsAndArguments));
}

#ifdef TRACING_IS_ENABLED
void oahOahGroup::initializeOahTraceOah ()
{
  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Options visitors trace",
        "htov", "help-trace-options-visitors",
R"()",
    kElementVisibilityHeaderOnly,
    this);

  appendSubGroupToGroup (subGroup);

  // trace oah visitors

  fTraceOahVisitors    = false;

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "toahv", "trace-oah-visitors",
R"(Write a trace of the OAH tree visiting activity to standard error.)",
        "traceOahVisitors",
        fTraceOahVisitors));
}
#endif

void oahOahGroup::initializeOahOahGroup (
  string executableName)
{
  // help
  // --------------------------------------
  initializeOahBasicHelpOptions (executableName);

  // options and arguments
  // --------------------------------------
  initializeOahOahAndArgumentsOptions ();

#ifdef TRACING_IS_ENABLED
  // trace
  // --------------------------------------
  initializeOahTraceOah ();
#endif
}

//______________________________________________________________________________
void oahOahGroup::enforceGroupQuietness ()
{
  // JMI
}

//______________________________________________________________________________
void oahOahGroup::checkGroupOptionsConsistency ()
{
  // JMI
}

//______________________________________________________________________________
void oahOahGroup::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      "% ==> oahOahGroup::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_oahOahGroup>*
    p =
      dynamic_cast<visitor<S_oahOahGroup>*> (v)) {
        S_oahOahGroup elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            "% ==> Launching oahOahGroup::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void oahOahGroup::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      "% ==> oahOahGroup::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_oahOahGroup>*
    p =
      dynamic_cast<visitor<S_oahOahGroup>*> (v)) {
        S_oahOahGroup elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            "% ==> Launching oahOahGroup::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void oahOahGroup::browseData (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      "% ==> oahOahGroup::browseData ()" <<
      endl;
  }
#endif
}

void oahOahGroup::printAtomWithValueOptionsValues (
  ostream&     os,
  unsigned int valueFieldWidth) const
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
void oahOahGroup::printOahOahValues (int valueFieldWidth)
{
  gLogStream <<
    "The basic options are:" <<
    endl;

  ++gIndenter;

  // command line
  // --------------------------------------

  gLogStream << left <<
    setw (valueFieldWidth) << "Command line:" <<
    endl;

  ++gIndenter;

  gLogStream << left <<
    setw (valueFieldWidth) << "inputSourceName" << " : " <<
    fInputSourceName <<
    endl <<

    setw (valueFieldWidth) << "showOptionsAndArguments" << " : " <<
    booleanAsString (fShowOptionsAndArguments) <<
    endl;

  --gIndenter;

  // options and help display
  // --------------------------------------

  gLogStream << left <<
    setw (valueFieldWidth) << "Options trace and display:" <<
    endl;

  ++gIndenter;

  gLogStream << left <<
    setw (valueFieldWidth) << "displayOptionsValues" << " : " <<
    booleanAsString (fDisplayOahValues) <<
    endl <<

    setw (valueFieldWidth) << "displayOahHandler" << " : " <<
    booleanAsString (fDisplayOahHandler) <<
    endl <<
    setw (valueFieldWidth) << "displayOahHandlerSummary" << " : " <<
    booleanAsString (fDisplayOahHandlerSummary) <<
    endl <<
    setw (valueFieldWidth) << "displayOahHandlerEssentials" << " : " <<
    booleanAsString (fDisplayOahHandlerEssentials) <<
    endl <<

    setw (valueFieldWidth) << "traceOahVisitors" << " : " <<
    booleanAsString (fTraceOahVisitors) <<
    endl;

  --gIndenter;

  --gIndenter;
}

ostream& operator<< (ostream& os, const S_oahOahGroup& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_oahOahGroup createGlobalOahOahGroup (
  string executableName)
{
#ifdef TRACING_IS_ENABLED
#ifdef ENFORCE_TRACE_OAH
  gLogStream <<
    "Creating global global OAH group" <<
    endl;
#endif
#endif

  // protect library against multiple initializations
  if (! gGlobalOahOahGroup) {
    // create the global options group
    gGlobalOahOahGroup =
      oahOahGroup::create (
        executableName);
    assert (gGlobalOahOahGroup != 0);
  }

  // return it
  return gGlobalOahOahGroup;
}


}

/* JMI
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
  assert (o != nullptr);
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
  : oahAtomWithValue (
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

void oahOptionalValuesStyleKindAtom::applyAtomWithValue (
  const string& theString,
  ostream&      os)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceOah ()) {
    gLogStream <<
      "==> oahAtom is of type 'oahOptionalValuesStyleKindAtom'" <<
      endl;
  }
#endif

  // theString contains the language name:
  // is it in the optional values style kinds map?

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceOah ()) {
    gLogStream <<
      "==> oahAtom is of type 'oahOptionalValuesStyleKindAtom'" <<
      endl;
  }
#endif

  map<string, oahOptionalValuesStyleKind>::const_iterator
    it =
      gGlobalOahOptionalValuesStyleKindsMap.find (
        theString);

  if (it == gGlobalOahOptionalValuesStyleKindsMap.end ()) {
    // no, optional values style kind is unknown in the map
    stringstream s;

    s <<
      "OAH optional values style kind '" << theString <<
      "' is unknown" <<
      endl <<
      "The " <<
      gGlobalOahOptionalValuesStyleKindsMap.size () - 1 <<
      " known OAH optional values style kind are:" <<
      endl;

    ++gIndenter;

    s <<
      existingOahOptionalValuesStyleKinds (K_NAMES_LIST_MAX_LENGTH);

    --gIndenter;

    oahError (s.str ());
  }

  setOahOptionalValuesStyleKindVariable (
    (*it).second);
}

void oahOptionalValuesStyleKindAtom::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      "% ==> oahOptionalValuesStyleKindAtom::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_oahOptionalValuesStyleKindAtom>*
    p =
      dynamic_cast<visitor<S_oahOptionalValuesStyleKindAtom>*> (v)) {
        S_oahOptionalValuesStyleKindAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            "% ==> Launching oahOptionalValuesStyleKindAtom::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void oahOptionalValuesStyleKindAtom::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      "% ==> oahOptionalValuesStyleKindAtom::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_oahOptionalValuesStyleKindAtom>*
    p =
      dynamic_cast<visitor<S_oahOptionalValuesStyleKindAtom>*> (v)) {
        S_oahOptionalValuesStyleKindAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            "% ==> Launching oahOptionalValuesStyleKindAtom::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void oahOptionalValuesStyleKindAtom::browseData (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
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

string oahOptionalValuesStyleKindAtom::asActualLongNamedOptionString () const
{
  stringstream s;

  s <<
    "-" << fLongName << " " <<
    oahOptionalValuesStyleKindAsString (fOahOptionalValuesStyleKindVariable);

  return s.str ();
}

void oahOptionalValuesStyleKindAtom::print (ostream& os) const
{
  const unsigned int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "OptionalValuesStyleKindAtom:" <<
    endl;

  ++gIndenter;

  printAtomWithValueEssentials (
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

  --gIndenter;
}

ostream& operator<< (ostream& os, const S_oahOptionalValuesStyleKindAtom& elt)
{
  elt->print (os);
  return os;
}
*/

/* JMI ???
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
          regex_replace (
            regex_replace (
              regex_replace (
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
              regex ("NUMBER"),
              to_string (gGlobalOahOptionalValuesStyleKindsMap.size ())),
            regex ("OPTIONAL_VALUES_STYLES"),
            existingOahOptionalValuesStyleKinds (K_NAMES_LIST_MAX_LENGTH)),
          regex ("DEFAULT_VALUE"),
          oahOptionalValuesStyleKindAsString (
            oahOptionalValuesStyleKindDefaultValue)),
        "STYLE",
        "optionalValuesStyleKind",
        optionalValuesStyleKind));
*/
