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

#include "setTraceOahIfDesired.h"
#ifdef TRACE_OAH
  #include "traceOah.h"
#endif

#include "generalOah.h"


using namespace std;

namespace MusicXML2
{

//_______________________________________________________________________________
S_generalOahGroup gGlobalGeneralOahGroup;
S_generalOahGroup gGlobalGeneralOahGroupUserChoices;

S_generalOahGroup generalOahGroup::create (
  S_oahHandler handlerUpLink)
{
  generalOahGroup* o = new generalOahGroup (
    handlerUpLink);
  assert(o!=0);

  return o;
}

generalOahGroup::generalOahGroup (
  S_oahHandler handlerUpLink)
  : oahGroup (
    "General",
    "hg", "help-general",
R"()",
    kElementVisibilityWhole,
    handlerUpLink)
{
  // append this options group to the options handler
  // if relevant
  if (handlerUpLink) {
    handlerUpLink->
      appendGroupToHandler (this);
  }

  // initialize it
  initializeGeneralOah (false);
}

generalOahGroup::~generalOahGroup ()
{}

void generalOahGroup::initializeGeneralWarningAndErrorsOptions (
  bool boolOptionsInitialValue)
{
  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Warnings and errors",
        "hwae", "help-warnings-and-errors",
R"()",
        kElementVisibilityWhole,
        this);

  appendSubGroupToGroup (subGroup);

  // quiet

  fQuiet = boolOptionsInitialValue;

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "q", "quiet",
R"(Don't issue any warning or error messages.)",
        "quiet",
        fQuiet));

  // don't show errors

  fDontShowErrors = boolOptionsInitialValue;

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "dse", "dont-show-errors",
R"(Don't show errors in the log.)",
        "dontShowErrors",
        fDontShowErrors));

  // do not exit on errors

  fDontExitOnErrors = boolOptionsInitialValue;

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "deoe", "dont-exit-on-errors",
        regex_replace (
R"(Do not exit execution on errors and go ahead.
This may be useful when debugging EXECUTABLE.)",
          regex ("EXECUTABLE"),
          gGlobalOahOahGroup->fHandlerExecutableName),
        "dontExitOnErrors",
        fDontExitOnErrors));

  // display the source code position

  fDisplaySourceCodePosition = boolOptionsInitialValue;

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "dscp", "display-source-code-position",
        regex_replace (
R"(Display the source code file name and line number
in warning and error messages.
This is useful when debugging EXECUTABLE.)",
          regex ("EXECUTABLE"),
          gGlobalOahOahGroup->fHandlerExecutableName),
        "displaySourceCodePosition",
        fDisplaySourceCodePosition));
}

void generalOahGroup::initializeGeneralCPUUsageOptions (
  bool boolOptionsInitialValue)
{
  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "CPU usage",
        "hgcpu", "help-general-cpu-usage",
R"()",
      kElementVisibilityWhole,
      this);

  appendSubGroupToGroup (subGroup);

  // CPU usage

  fDisplayCPUusage = boolOptionsInitialValue;

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "cpu", "display-cpu-usage",
R"(Write information about CPU usage to standard error.)",
        "displayCPUusage",
        fDisplayCPUusage));
}

void generalOahGroup::initializeGeneralOah (
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
    fTranslationDateFull = buffer;

    strftime (buffer, 80, "%Y-%m-%d", translationTimeinfo);
    fTranslationDateYYYYMMDD = buffer;
  }

  // warning and error handling
  // --------------------------------------
  initializeGeneralWarningAndErrorsOptions (
    boolOptionsInitialValue);

  // CPU usage
  // --------------------------------------
  initializeGeneralCPUUsageOptions (
    boolOptionsInitialValue);
}

S_generalOahGroup generalOahGroup::createCloneWithTrueValues ()
{
  S_generalOahGroup
    clone =
      generalOahGroup::create (
        nullptr);
      // nullptr not to have it inserted twice in the option handler

  // set the options handler upLink
  clone->fHandlerUpLink =
    fHandlerUpLink;

  // warning and error handling
  // --------------------------------------

  clone->fQuiet =
    fQuiet;
  clone->fDontShowErrors =
    fDontShowErrors;
  clone->fDontExitOnErrors =
    fDontExitOnErrors;
  clone->fDisplaySourceCodePosition =
    fDisplaySourceCodePosition;

  // CPU usage
  // --------------------------------------

  clone->fDisplayCPUusage = true;

  return clone;
}

  /* JMI
void generalOahGroup::setAllGeneralTraceOah (
  bool boolOptionsInitialValue)
{
  // warning and error handling
  // --------------------------------------

  fQuiet = boolOptionsInitialValue;
  fDontShowErrors = boolOptionsInitialValue;
  fDontExitOnErrors = boolOptionsInitialValue;
  fDisplaySourceCodePosition = boolOptionsInitialValue;

  // CPU usage
  // --------------------------------------

  fDisplayCPUusage = boolOptionsInitialValue;
}
  */

//______________________________________________________________________________
void generalOahGroup::enforceGroupQuietness ()
{
  fDisplayCPUusage = false; // JMI
}

//______________________________________________________________________________
void generalOahGroup::checkGroupOptionsConsistency ()
{
  // JMI
}

//______________________________________________________________________________
void generalOahGroup::acceptIn (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gGlobalOahOahGroup->fTraceOahVisitors) {
    gLogOstream <<
      ".\\\" ==> generalOahGroup::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_generalOahGroup>*
    p =
      dynamic_cast<visitor<S_generalOahGroup>*> (v)) {
        S_generalOahGroup elem = this;

#ifdef TRACE_OAH
        if (gGlobalOahOahGroup->fTraceOahVisitors) {
          gLogOstream <<
            ".\\\" ==> Launching generalOahGroup::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void generalOahGroup::acceptOut (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gGlobalOahOahGroup->fTraceOahVisitors) {
    gLogOstream <<
      ".\\\" ==> generalOahGroup::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_generalOahGroup>*
    p =
      dynamic_cast<visitor<S_generalOahGroup>*> (v)) {
        S_generalOahGroup elem = this;

#ifdef TRACE_OAH
        if (gGlobalOahOahGroup->fTraceOahVisitors) {
          gLogOstream <<
            ".\\\" ==> Launching generalOahGroup::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void generalOahGroup::browseData (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gGlobalOahOahGroup->fTraceOahVisitors) {
    gLogOstream <<
      ".\\\" ==> generalOahGroup::browseData ()" <<
      endl;
  }
#endif
}

//______________________________________________________________________________
void generalOahGroup::printGeneralOahValues (int fieldWidth)
{
  gLogOstream <<
    "The general options are:" <<
    endl;

  gIndenter++;

  // translation date
  // --------------------------------------

  gLogOstream << left <<

  gIndenter++;

  gLogOstream << left <<
    setw (fieldWidth) << "translationDate" << " : " <<
    fTranslationDateFull <<
    endl;

  gIndenter--;

  // warning and error handling
  // --------------------------------------

  gLogOstream << left <<
    setw (fieldWidth) << "Warning and error handling:" <<
    endl;

  gIndenter++;

  gLogOstream <<
    setw (fieldWidth) << "quiet" << " : " <<
    booleanAsString (fQuiet) <<
    endl <<
    setw (fieldWidth) << "dontShowErrors" << " : " <<
    booleanAsString (fDontShowErrors) <<
    endl <<
    setw (fieldWidth) << "dontExitOnErrors" << " : " <<
    booleanAsString (fDontExitOnErrors) <<
    endl <<
    setw (fieldWidth) << "displaySourceCodePosition" << " : " <<
    booleanAsString (fDisplaySourceCodePosition) <<
    endl;

  gIndenter--;


  // CPU usage
  // --------------------------------------

  gLogOstream << left <<
    setw (fieldWidth) << "CPU usage:" <<
    endl;

  gIndenter++;

  gLogOstream <<
    setw (fieldWidth) << "displayCPUusage" << " : " <<
    booleanAsString (fDisplayCPUusage) <<
    endl;

  gIndenter--;

  gIndenter--;
}

ostream& operator<< (ostream& os, const S_generalOahGroup& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
void initializeGeneralOahHandling (
  S_oahHandler handler)
{
  // create the options variables
  // ------------------------------------------------------

  gGlobalGeneralOahGroupUserChoices = generalOahGroup::create (
    handler);
  assert(gGlobalGeneralOahGroupUserChoices != 0);

  gGlobalGeneralOahGroup =
    gGlobalGeneralOahGroupUserChoices;
}


}
