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

#include "version.h"
#include "utilities.h"

#include "enableTracingIfDesired.h"
#ifdef TRACING_IS_ENABLED
  #include "traceOah.h"
#endif

#include "waeMessagesHandling.h"

#include "generatorsBasicTypes.h"

#include "oahOah.h"
#include "generalOah.h"

#include "msrGeneratorsOah.h"
//#include "generatorsInsiderHandler.h"


using namespace std;

namespace MusicXML2
{

//_______________________________________________________________________________

S_msrGeneratorsOahGroup gGlobalMsrGeneratorsOahGroup;

S_msrGeneratorsOahGroup msrGeneratorsOahGroup::create ()
{
  msrGeneratorsOahGroup* o = new msrGeneratorsOahGroup ();
  assert (o != nullptr);
  return o;
}

msrGeneratorsOahGroup::msrGeneratorsOahGroup ()
  : oahGroup (
    "MSR generators",
    "hmsrgen", "help-msr generators",
R"(These options control the way the translators to MSR work.)",
    kElementVisibilityWhole)
{
  fGenerationAPIKind = kMsrFunctionsAPIKind;

  // initialize it
  initializeMsrGeneratorsOahGroup ();
}

msrGeneratorsOahGroup::~msrGeneratorsOahGroup ()
{}

void msrGeneratorsOahGroup::initializeGenerationAPIOptions ()
{
  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Generation API",
        "hga", "help-generation-api",
R"()",
      kElementVisibilityWhole,
      this);

  appendSubGroupToGroup (subGroup);

  const msrGenerationAPIKind
    msrGenerationAPIKindDefaultValue =
      kMsrFunctionsAPIKind; // default value

  fGenerationAPIKindAtom =
    msrGenerationAPIKindAtom::create (
      K_GENERATION_API_KIND_SHORT_NAME, K_GENERATION_API_KIND_LONG_NAME,
      regex_replace (
        regex_replace (
          regex_replace (
  R"(Generate GENERATION_API code to the output.
  The NUMBER generation API kinds available are:
  GENERATION_API_KINDS.
  The default is 'DEFAULT_VALUE'.)",
            regex ("NUMBER"),
            to_string (gGlobalGenerationAPIKindsMap.size ())),
          regex ("GENERATION_API_KINDS"),
          existingGenerationAPIKinds (K_NAMES_LIST_MAX_LENGTH)),
        regex ("DEFAULT_VALUE"),
        msrGenerationAPIKindAsString (
          msrGenerationAPIKindDefaultValue)),
      "GENERATION_API",
      "msrGenerationAPIKind",
      fGenerationAPIKind);

  subGroup->
    appendAtomToSubGroup (
      fGenerationAPIKindAtom);
}

void msrGeneratorsOahGroup::initializeMsrGeneratorsOahGroup ()
{
#ifdef TRACING_IS_ENABLED
  // trace
  // --------------------------------------
// JMI  initializeMsrGeneratorsTraceOah ();
#endif

  // generation API
  // --------------------------------------
  initializeGenerationAPIOptions ();
}

//______________________________________________________________________________
void msrGeneratorsOahGroup::enforceGroupQuietness ()
{}

//______________________________________________________________________________
void msrGeneratorsOahGroup::checkGroupOptionsConsistency ()
{}

//______________________________________________________________________________
void msrGeneratorsOahGroup::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> msrGeneratorsOahGroup::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_msrGeneratorsOahGroup>*
    p =
      dynamic_cast<visitor<S_msrGeneratorsOahGroup>*> (v)) {
        S_msrGeneratorsOahGroup elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching msrGeneratorsOahGroup::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void msrGeneratorsOahGroup::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> msrGeneratorsOahGroup::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_msrGeneratorsOahGroup>*
    p =
      dynamic_cast<visitor<S_msrGeneratorsOahGroup>*> (v)) {
        S_msrGeneratorsOahGroup elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching msrGeneratorsOahGroup::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void msrGeneratorsOahGroup::browseData (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> msrGeneratorsOahGroup::browseData ()" <<
      endl;
  }
#endif
}

//______________________________________________________________________________
void msrGeneratorsOahGroup::printMsrGeneratorsOahValues (
  unsigned int fieldWidth)
{
  gLogStream <<
    "The msrGenerators options are:" <<
    endl;

  ++gIndenter;

  // generation API kind
  // --------------------------------------

  gLogStream <<
    "Generation API:" <<
    endl;

  ++gIndenter;

  gLogStream << left <<
    setw (fieldWidth) << "gnerationAPIKind" << " : " <<
      msrGenerationAPIKindAsString (fGenerationAPIKind) <<
      endl;

  --gIndenter;

  --gIndenter;
}

ostream& operator<< (ostream& os, const S_msrGeneratorsOahGroup& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_msrGeneratorsOahGroup createGlobalMsrGeneratorsOahGroup ()
{
#ifdef TRACING_IS_ENABLED
  if (getTraceOah ()) {
    gLogStream <<
      "Creating global msrGenerators OAH group" <<
      endl;
  }
#endif

  // protect library against multiple initializations
  if (! gGlobalMsrGeneratorsOahGroup) {

    // initialize the generation API kinds map
    // ------------------------------------------------------

    initializeMsrGenerationAPI ();

    // create the msrGenerators options
    // ------------------------------------------------------

    gGlobalMsrGeneratorsOahGroup =
      msrGeneratorsOahGroup::create ();
    assert (gGlobalMsrGeneratorsOahGroup != 0);
  }

  // return the global OAH group
  return gGlobalMsrGeneratorsOahGroup;
}


}
