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

#include "multiGenerationOah.h"

#include "oahOah.h"
#include "generalOah.h"

#include "msdlCompilerOah.h"
#include "msdlCompilerInsiderHandler.h"


using namespace std;

namespace MusicXML2
{

//_______________________________________________________________________________

S_msdlCompilerOahGroup gGlobalMsdlCompilerOahGroup;

S_msdlCompilerOahGroup msdlCompilerOahGroup::create ()
{
  msdlCompilerOahGroup* o = new msdlCompilerOahGroup ();
  assert (o != nullptr);
  return o;
}

msdlCompilerOahGroup::msdlCompilerOahGroup ()
  : oahGroup (
    "msdlCompiler",
    "hmkk-group", "help-mikrokosmos-group",
R"(These options control the way MSDL compiler works.)",
    oahElementVisibilityKind::kElementVisibilityWhole)
{
/*
  fGeneratorOutputKind = multiGeneratorOutputKind::k_NoOutput;

  fUTFKind = kUTF8; // default value

  fBrailleOutputKind = kBrailleOutputAscii; // default value

  fByteOrderingKind = kByteOrderingNone;
*/

  // initialize it
  initializeMsdlCompilerOahGroup ();
}

msdlCompilerOahGroup::~msdlCompilerOahGroup ()
{}

void msdlCompilerOahGroup::initializeMsdlCompilerOahGroup ()
{
#ifdef TRACING_IS_ENABLED
  // trace
  // --------------------------------------
// JMI  initializeMsdlCompilerTraceOah ();
#endif
}

void msdlCompilerOahGroup::enforceGroupQuietness ()
{}

void msdlCompilerOahGroup::checkGroupOptionsConsistency ()
{}

void msdlCompilerOahGroup::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> msdlCompilerOahGroup::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_msdlCompilerOahGroup>*
    p =
      dynamic_cast<visitor<S_msdlCompilerOahGroup>*> (v)) {
        S_msdlCompilerOahGroup elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching msdlCompilerOahGroup::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void msdlCompilerOahGroup::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> msdlCompilerOahGroup::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_msdlCompilerOahGroup>*
    p =
      dynamic_cast<visitor<S_msdlCompilerOahGroup>*> (v)) {
        S_msdlCompilerOahGroup elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching msdlCompilerOahGroup::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void msdlCompilerOahGroup::browseData (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> msdlCompilerOahGroup::browseData ()" <<
      endl;
  }
#endif
}

void msdlCompilerOahGroup::printMsdlCompilerOahValues (
  unsigned int fieldWidth)
{
  gLogStream <<
    "The msdlCompiler options are:" <<
    endl;

  ++gIndenter;

  --gIndenter;
}

ostream& operator<< (ostream& os, const S_msdlCompilerOahGroup& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_msdlCompilerOahGroup createGlobalMsdlCompilerOahGroup ()
{
#ifdef TRACING_IS_ENABLED
  if (getTraceOah ()) {
    gLogStream <<
      "Creating global msdlCompiler OAH group" <<
      endl;
  }
#endif

  // protect library against multiple initializations
  if (! gGlobalMsdlCompilerOahGroup) {

    // initialize the generated output kinds map
    // ------------------------------------------------------

    initializeMultiGeneratorOutputKindsMap ();

    // create the msdlCompiler options
    // ------------------------------------------------------

    gGlobalMsdlCompilerOahGroup =
      msdlCompilerOahGroup::create ();
    assert (gGlobalMsdlCompilerOahGroup != 0);
  }

  // return the global OAH group
  return gGlobalMsdlCompilerOahGroup;
}


}
