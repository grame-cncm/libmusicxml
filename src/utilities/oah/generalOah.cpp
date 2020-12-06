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

#include "enableTracingIfDesired.h"
#ifdef TRACING_IS_ENABLED
  #include "traceOah.h"
#endif

#include "oahOah.h"

#include "generalOah.h"

#include "oahAtomsCollection.h"


using namespace std;

namespace MusicXML2
{

//_______________________________________________________________________________
S_generalOahGroup gGlobalGeneralOahGroup;

S_generalOahGroup generalOahGroup::create ()
{
  generalOahGroup* o = new generalOahGroup ();
  assert (o!=0);

  return o;
}

generalOahGroup::generalOahGroup ()
  : oahGroup (
    "General",
    "hg", "help-general",
R"()",
    kElementVisibilityWhole)
{
  initializeGeneralOah ();
}

generalOahGroup::~generalOahGroup ()
{}

void generalOahGroup::initializeGeneralWarningAndErrorsOptions ()
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

  fQuiet = false;

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "q", "quiet",
R"(Don't issue any warning or error messages.)",
        "quiet",
        fQuiet));

  // don't show errors

  fDontShowErrors = false;

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "dse", "dont-show-errors",
R"(Don't show errors in the log.)",
        "dontShowErrors",
        fDontShowErrors));

  // do not quit on errors

  fDontQuitOnErrors = false;

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "deoe", "dont-quit-on-errors",
        regex_replace (
R"(Do not quit execution on errors and go ahead.
This may be useful when debugging EXECUTABLE.)",
          regex ("EXECUTABLE"),
          gGlobalOahOahGroup->getHandlerExecutableName ()),
        "dontQuitOnErrors",
        fDontQuitOnErrors));

  // display the source code position

  fDisplaySourceCodePosition = false;

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "dscp", "display-source-code-position",
        regex_replace (
R"(Display the source code file name and line number
in warning and error messages.
This is useful when debugging EXECUTABLE.)",
          regex ("EXECUTABLE"),
          gGlobalOahOahGroup->getHandlerExecutableName ()),
        "displaySourceCodePosition",
        fDisplaySourceCodePosition));
}

void generalOahGroup::initializeGeneralCPUUsageOptions ()
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

  fDisplayCPUusage = false;

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "cpu", "display-cpu-usage",
R"(Write information about CPU usage to standard error.)",
        "displayCPUusage",
        fDisplayCPUusage));
}

void generalOahGroup::initializeGeneralOah ()
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
  initializeGeneralWarningAndErrorsOptions ();

  // CPU usage
  // --------------------------------------
  initializeGeneralCPUUsageOptions ();
}

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
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> generalOahGroup::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_generalOahGroup>*
    p =
      dynamic_cast<visitor<S_generalOahGroup>*> (v)) {
        S_generalOahGroup elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching generalOahGroup::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void generalOahGroup::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> generalOahGroup::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_generalOahGroup>*
    p =
      dynamic_cast<visitor<S_generalOahGroup>*> (v)) {
        S_generalOahGroup elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching generalOahGroup::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void generalOahGroup::browseData (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> generalOahGroup::browseData ()" <<
      endl;
  }
#endif
}

//______________________________________________________________________________
void generalOahGroup::printGeneralOahValues (int fieldWidth)
{
  gLogStream <<
    "The general options are:" <<
    endl;

  gIndenter++;

  // translation date
  // --------------------------------------

  gLogStream << left <<

  gIndenter++;

  gLogStream << left <<
    setw (fieldWidth) << "translationDate" << " : " <<
    fTranslationDateFull <<
    endl;

  gIndenter--;

  // warning and error handling
  // --------------------------------------

  gLogStream << left <<
    setw (fieldWidth) << "Warning and error handling:" <<
    endl;

  gIndenter++;

  gLogStream <<
    setw (fieldWidth) << "quiet" << " : " <<
    booleanAsString (fQuiet) <<
    endl <<
    setw (fieldWidth) << "dontShowErrors" << " : " <<
    booleanAsString (fDontShowErrors) <<
    endl <<
    setw (fieldWidth) << "dontQuitOnErrors" << " : " <<
    booleanAsString (fDontQuitOnErrors) <<
    endl <<
    setw (fieldWidth) << "displaySourceCodePosition" << " : " <<
    booleanAsString (fDisplaySourceCodePosition) <<
    endl;

  gIndenter--;


  // CPU usage
  // --------------------------------------

  gLogStream << left <<
    setw (fieldWidth) << "CPU usage:" <<
    endl;

  gIndenter++;

  gLogStream <<
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
S_generalOahGroup createGlobalGeneralOahGroup ()
{
#ifdef TRACING_IS_ENABLED
#ifdef ENFORCE_TRACE_OAH
  gLogStream <<
    "Creating global general OAH group" <<
    endl;
#endif
#endif

  // protect library against multiple initializations
  if (! gGlobalGeneralOahGroup) {
    // create the global general options group
    gGlobalGeneralOahGroup =
      generalOahGroup::create ();
    assert (gGlobalGeneralOahGroup != 0);
  }

  // return the global OAH group
  return gGlobalGeneralOahGroup;
}


}
