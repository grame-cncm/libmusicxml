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

#include "setTraceOahIfDesired.h"
#ifdef TRACE_OAH
  #include "traceOah.h"
#endif

#include "messagesHandling.h"

#include "generalOah.h"
#include "lilypondOah.h"


using namespace std;

namespace MusicXML2
{
//_______________________________________________________________________________

S_lilypondOah gGlobalLilypondOah;
S_lilypondOah gGlobalLilypondOahUserChoices;
S_lilypondOah gGlobalLilypondOahWithDetailedTrace;

S_lilypondOah lilypondOah::create (
  S_oahHandler handlerUpLink)
{
  lilypondOah* o = new lilypondOah(
    handlerUpLink);
  assert(o!=0);
  return o;
}

lilypondOah::lilypondOah (
  S_oahHandler handlerUpLink)
  : oahGroup (
    "LilyPond",
    "hlily", "help-lilypond",
R"(These options control the way LilyPond data is handled.)",
    kElementVisibilityWhole,
    handlerUpLink)
{
/* JMI
  // sanity check
  msgAssert (
    handlerUpLink != nullptr,
    "handlerUpLink is null");
*/

  // append this options group to the options handler
  // if relevant
  if (handlerUpLink) {
    handlerUpLink->
      appendGroupToHandler (this);
  }

  // initialize it
  initializeLilypondOah (false);
}

lilypondOah::~lilypondOah ()
{}

#ifdef TRACE_OAH
void lilypondOah::initializelilypondOahTraceOah (
  bool boolOptionsInitialValue)
{}
#endif

void lilypondOah::initializeLilypondOah (
  bool boolOptionsInitialValue)
{
#ifdef TRACE_OAH
  // trace
  // --------------------------------------
  initializelilypondOahTraceOah (
    boolOptionsInitialValue);
#endif
}

S_lilypondOah lilypondOah::createCloneWithDetailedTrace ()
{
  S_lilypondOah
    clone =
      lilypondOah::create (0);
      // 0 not to have it inserted twice in the option handler

  // set the options handler upLink
  clone->fHandlerUpLink =
    fHandlerUpLink;

  return clone;
}

//______________________________________________________________________________
void lilypondOah::setAlllilypondOahTraceOah (
  bool boolOptionsInitialValue)
{
#ifdef TRACE_OAH
  // specific trace
  // JMI
#endif
}

//______________________________________________________________________________
void lilypondOah::enforceGroupQuietness ()
{}

//______________________________________________________________________________
void lilypondOah::checkGroupOptionsConsistency ()
{}

//______________________________________________________________________________
void lilypondOah::acceptIn (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gGlobalOahOahGroup->fTraceOahVisitors) {
    gLogOstream <<
      ".\\\" ==> lilypondOah::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_lilypondOah>*
    p =
      dynamic_cast<visitor<S_lilypondOah>*> (v)) {
        S_lilypondOah elem = this;

#ifdef TRACE_OAH
        if (gGlobalOahOahGroup->fTraceOahVisitors) {
          gLogOstream <<
            ".\\\" ==> Launching lilypondOah::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void lilypondOah::acceptOut (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gGlobalOahOahGroup->fTraceOahVisitors) {
    gLogOstream <<
      ".\\\" ==> lilypondOah::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_lilypondOah>*
    p =
      dynamic_cast<visitor<S_lilypondOah>*> (v)) {
        S_lilypondOah elem = this;

#ifdef TRACE_OAH
        if (gGlobalOahOahGroup->fTraceOahVisitors) {
          gLogOstream <<
            ".\\\" ==> Launching lilypondOah::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void lilypondOah::browseData (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gGlobalOahOahGroup->fTraceOahVisitors) {
    gLogOstream <<
      ".\\\" ==> lilypondOah::browseData ()" <<
      endl;
  }
#endif
}

//______________________________________________________________________________
void lilypondOah::printLilypondOahValues (int fieldWidth)
{
  gLogOstream <<
    "The lilypondOah options are:" <<
    endl;

  gIndenter++;

  gIndenter--;
}

ostream& operator<< (ostream& os, const S_lilypondOah& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
void initializeLilypondOahHandling (
  S_oahHandler handler)
{
#ifdef TRACE_OAH
  if (gGlobalTraceOahGroup->getTraceOah () && ! gGlobalGeneralOahGroup->fQuiet) {
    gLogOstream <<
      "Initializing lilypondOah options handling" <<
      endl;
  }
#endif

  // create the lilypondOah options
  // ------------------------------------------------------

  gGlobalLilypondOahUserChoices = lilypondOah::create (
    handler);
  assert(gGlobalLilypondOahUserChoices != 0);

  gGlobalLilypondOah =
    gGlobalLilypondOahUserChoices;

  // prepare for measure detailed trace
  // ------------------------------------------------------

/* JMI
  gGlobalLilypondOahWithDetailedTrace =
    gGlobalLilypondOah->
      createCloneWithDetailedTrace ();
      */
}


}
