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

#include "version.h"
#include "utilities.h"

#include "enableTracingIfDesired.h"
#ifdef TRACING_IS_ENABLED
  #include "traceOah.h"
#endif

#include "waeMessagesHandling.h"

#include "oahOah.h"
#include "generalOah.h"
#include "lilypondOah.h"


using namespace std;

namespace MusicXML2
{
//_______________________________________________________________________________

S_lilypondOahGroup gGlobalLilypondOahGroup;

S_lilypondOahGroup lilypondOahGroup::create ()
{
  lilypondOahGroup* o = new lilypondOahGroup ();
  assert (o != nullptr);
  return o;
}

lilypondOahGroup::lilypondOahGroup ()
  : oahGroup (
    "LilyPond",
    "hlily", "help-lilypond",
R"(These options control the way LilyPond data is handled.)",
    kElementVisibilityWhole)
{
  initializeLilypondOahGroup ();
}

lilypondOahGroup::~lilypondOahGroup ()
{}

void lilypondOahGroup::initializeLilypondOahGroup ()
{
#ifdef TRACING_IS_ENABLED
  // trace
  // --------------------------------------
// JMI  initializelilypondOahTraceOah ();
#endif
}

//______________________________________________________________________________
void lilypondOahGroup::enforceGroupQuietness ()
{}

//______________________________________________________________________________
void lilypondOahGroup::checkGroupOptionsConsistency ()
{}

//______________________________________________________________________________
void lilypondOahGroup::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> lilypondOahGroup::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_lilypondOahGroup>*
    p =
      dynamic_cast<visitor<S_lilypondOahGroup>*> (v)) {
        S_lilypondOahGroup elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching lilypondOahGroup::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void lilypondOahGroup::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> lilypondOahGroup::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_lilypondOahGroup>*
    p =
      dynamic_cast<visitor<S_lilypondOahGroup>*> (v)) {
        S_lilypondOahGroup elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching lilypondOahGroup::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void lilypondOahGroup::browseData (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> lilypondOahGroup::browseData ()" <<
      endl;
  }
#endif
}

//______________________________________________________________________________
void lilypondOahGroup::printLilypondOahValues (unsigned int fieldWidth)
{
  gLogStream <<
    "The lilypondOahGroup options are:" <<
    endl;

  ++gIndenter;

  --gIndenter;
}

ostream& operator<< (ostream& os, const S_lilypondOahGroup& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_lilypondOahGroup createGlobalLilypondOahGroup ()
{
#ifdef TRACING_IS_ENABLED
#ifdef ENFORCE_TRACE_OAH
  gLogStream <<
    "Creating global xml2xml OAH group" <<
    endl;
#endif
#endif

  // protect library against multiple initializations
  if (! gGlobalLilypondOahGroup) {
    // create the global OAH group
    gGlobalLilypondOahGroup =
      lilypondOahGroup::create ();
    assert (gGlobalLilypondOahGroup != 0);
  }

  // return the global OAH group
  return gGlobalLilypondOahGroup;
}


}
