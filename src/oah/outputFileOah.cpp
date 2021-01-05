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

#include "outputFileOah.h"

#include "oahAtomsCollection.h"


using namespace std;

namespace MusicXML2
{

//_______________________________________________________________________________
S_outputFileOahGroup gGlobalOutputFileOahGroup;

S_outputFileOahGroup outputFileOahGroup::create ()
{
  outputFileOahGroup* o = new outputFileOahGroup ();
  assert (o!=0);

  return o;
}

outputFileOahGroup::outputFileOahGroup ()
  : oahGroup (
    "OutputFile",
    "hofg", "help-output-file-group",
R"()",
    kElementVisibilityWhole)
{
  initializeOutputFileOah ();
}

outputFileOahGroup::~outputFileOahGroup ()
{}

void outputFileOahGroup::initializeOutputFileOah ()
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

  // output file name
  // --------------------------------------
  initializeOutputFileNameOptions ();
}

void outputFileOahGroup::initializeOutputFileNameOptions ()
{
#ifdef TRACING_IS_ENABLED
#ifdef ENFORCE_TRACE_OAH
  gLogStream << left <<
    "Creating insider output file subgroup in \"" <<
    fGroupHeader <<
    "\"" <<
    endl;
#endif
#endif

  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Output file",
        "hof", "help-output-file",
R"()",
      kElementVisibilityWhole,
      this);

  appendSubGroupToGroup (subGroup);

  // output filename

  fOutputFileNameStringAtom =
    oahStringAtom::create (
      "o", "output-file-name",
R"(Write braille music to file FILENAME instead of standard output.)",
      "FILENAME",
      "outputFileName",
      fOutputFileName);

  subGroup->
    appendAtomToSubGroup (
      fOutputFileNameStringAtom);

  // auto output filename

  fAutoOutputFileName = false;

  fAutoOutputFileNameAtom =
    oahBooleanAtom::create (
      "aofn", "auto-output-file-name",
R"(This option can only be used when reading from a file.
Write MusicXML code to a file in the current working directory.
The file name is derived from that of the input file,
replacing any suffix after the the '.' by 'xml'
or adding '.xml' if none is present.)",
      "autoOutputFileName",
      fAutoOutputFileName);

  subGroup->
    appendAtomToSubGroup (
      fAutoOutputFileNameAtom);
}

//______________________________________________________________________________
void outputFileOahGroup::enforceGroupQuietness ()
{
  fDisplayCPUusage = false; // JMI
}

//______________________________________________________________________________
void outputFileOahGroup::checkGroupOptionsConsistency ()
{
  // JMI
}

//______________________________________________________________________________
void outputFileOahGroup::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> outputFileOahGroup::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_outputFileOahGroup>*
    p =
      dynamic_cast<visitor<S_outputFileOahGroup>*> (v)) {
        S_outputFileOahGroup elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching outputFileOahGroup::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void outputFileOahGroup::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> outputFileOahGroup::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_outputFileOahGroup>*
    p =
      dynamic_cast<visitor<S_outputFileOahGroup>*> (v)) {
        S_outputFileOahGroup elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching outputFileOahGroup::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void outputFileOahGroup::browseData (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> outputFileOahGroup::browseData ()" <<
      endl;
  }
#endif
}

//______________________________________________________________________________
void outputFileOahGroup::printOutputFileOahValues (unsigned int fieldWidth)
{
  gLogStream <<
    "The outputFile options are:" <<
    endl;

  ++gIndenter;

  // output file
  // --------------------------------------

  gLogStream << left <<
    setw (fieldWidth) << "Output file:" <<
    endl;

  ++gIndenter;

  gLogStream << left <<
    setw (fieldWidth) << "outputFileName" << " : \"" <<
    fOutputFileName <<
    "\"" <<
    endl <<
    setw (fieldWidth) << "autoOutputFileName" << " : \"" <<
    booleanAsString (fAutoOutputFileName) <<
    "\"" <<
    endl;

  --gIndenter;

  // translation date
  // --------------------------------------

  gLogStream << left <<

  ++gIndenter;

  gLogStream << left <<
    setw (fieldWidth) << "translationDate" << " : " <<
    fTranslationDateFull <<
    endl;

  --gIndenter;

  // warning and error handling
  // --------------------------------------

  gLogStream << left <<
    setw (fieldWidth) << "Warning and error handling:" <<
    endl;

  ++gIndenter;

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

  --gIndenter;


  // CPU usage
  // --------------------------------------

  gLogStream << left <<
    setw (fieldWidth) << "CPU usage:" <<
    endl;

  ++gIndenter;

  gLogStream <<
    setw (fieldWidth) << "displayCPUusage" << " : " <<
    booleanAsString (fDisplayCPUusage) <<
    endl;

  --gIndenter;

  --gIndenter;
}

ostream& operator<< (ostream& os, const S_outputFileOahGroup& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_outputFileOahGroup createGlobalOutputFileOahGroup ()
{
#ifdef TRACING_IS_ENABLED
#ifdef ENFORCE_TRACE_OAH
  gLogStream <<
    "Creating global outputFile OAH group" <<
    endl;
#endif
#endif

  // protect library against multiple initializations
  if (! gGlobalOutputFileOahGroup) {
    // create the global outputFile options group
    gGlobalOutputFileOahGroup =
      outputFileOahGroup::create ();
    assert (gGlobalOutputFileOahGroup != 0);
  }

  // return the global OAH group
  return gGlobalOutputFileOahGroup;
}


}
