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
#include "msr2MeitreeOah.h"


using namespace std;

namespace MusicXML2
{

//______________________________________________________________________________
S_msr2MeitreeOah gMsr2MeitreeOah;
S_msr2MeitreeOah gMsr2MeitreeOahUserChoices;
S_msr2MeitreeOah gMsr2MeitreeOahWithDetailedTrace;

S_msr2MeitreeOah msr2MeitreeOah::create (
  S_oahHandler handlerUpLink)
{
  msr2MeitreeOah* o = new msr2MeitreeOah(
    handlerUpLink);
  assert(o!=0);
  return o;
}

msr2MeitreeOah::msr2MeitreeOah (
  S_oahHandler handlerUpLink)
  : oahGroup (
    "Msr2MeiTree",
    "hmsr2meit", "help-msr-to-meitree",
R"(These options control the way MSR is translated to xmlelement tree.)",
    kElementVisibilityWhole,
    handlerUpLink)
{
/* JMI
  // sanity check
  msrAssert (
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
  initializeMsr2MeitreeOah (false);
}

msr2MeitreeOah::~msr2MeitreeOah ()
{}

#ifdef TRACE_OAH
void msr2MeitreeOah::initializeMsr2MeiTreeTraceOah (
  bool boolOptionsInitialValue)
{}
#endif

void msr2MeitreeOah::initializeCodeGenerationOptions (
  bool boolOptionsInitialValue)
{
  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Code generation",
        "hmeicg", "help-mei-code-generation",
R"()",
      kElementVisibilityWhole,
      this);

  appendSubGroupToGroup (subGroup);

  // comments

  fMusicXMLComments = boolOptionsInitialValue;

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "meicom", "mei-comments",
R"(Generate comments showing the structure of the score.)",
        "MusicXMLComments",
        fMusicXMLComments));
}

void msr2MeitreeOah::initializeMsr2MeitreeOah (
  bool boolOptionsInitialValue)
{
#ifdef TRACE_OAH
  // trace
  // --------------------------------------
  initializeMsr2MeiTreeTraceOah (
    boolOptionsInitialValue);
#endif

  // code generation
  // --------------------------------------
  initializeCodeGenerationOptions (
    boolOptionsInitialValue);
}

S_msr2MeitreeOah msr2MeitreeOah::createCloneWithDetailedTrace ()
{
  S_msr2MeitreeOah
    clone =
      msr2MeitreeOah::create (0);
      // 0 not to have it inserted twice in the option handler

  // set the options handler upLink
  clone->fHandlerUpLink =
    fHandlerUpLink;

  return clone;
}

//______________________________________________________________________________
void msr2MeitreeOah::setAllMusicXMLTraceOah (
  bool boolOptionsInitialValue)
{
#ifdef TRACE_OAH
  // specific trace
  // JMI
#endif
}

//______________________________________________________________________________
void msr2MeitreeOah::enforceQuietness ()
{}

//______________________________________________________________________________
void msr2MeitreeOah::checkOptionsConsistency ()
{}

//______________________________________________________________________________
void msr2MeitreeOah::acceptIn (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    gLogOstream <<
      ".\\\" ==> msr2MeitreeOah::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_msr2MeitreeOah>*
    p =
      dynamic_cast<visitor<S_msr2MeitreeOah>*> (v)) {
        S_msr2MeitreeOah elem = this;

#ifdef TRACE_OAH
        if (gOahOah->fTraceOahVisitors) {
          gLogOstream <<
            ".\\\" ==> Launching msr2MeitreeOah::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void msr2MeitreeOah::acceptOut (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    gLogOstream <<
      ".\\\" ==> msr2MeitreeOah::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_msr2MeitreeOah>*
    p =
      dynamic_cast<visitor<S_msr2MeitreeOah>*> (v)) {
        S_msr2MeitreeOah elem = this;

#ifdef TRACE_OAH
        if (gOahOah->fTraceOahVisitors) {
          gLogOstream <<
            ".\\\" ==> Launching msr2MeitreeOah::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void msr2MeitreeOah::browseData (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    gLogOstream <<
      ".\\\" ==> msr2MeitreeOah::browseData ()" <<
      endl;
  }
#endif
}

//______________________________________________________________________________
void msr2MeitreeOah::printMsr2MeitreeOahValues (int fieldWidth)
{
  gLogOstream <<
    "The MusicXML options are:" <<
    endl;

  gIndenter++;

  gIndenter--;
}

ostream& operator<< (ostream& os, const S_msr2MeitreeOah& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
void initializeMsr2MeitreeOahHandling (
  S_oahHandler handler)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceOah && ! gGeneralOah->fQuiet) {
    gLogOstream <<
      "Initializing MusicXML options handling" <<
      endl;
  }
#endif

  // protect library against multiple initializations
  static bool pThisMethodHasBeenRun = false;

  if (! pThisMethodHasBeenRun) {
    // create the MusicXML options
    // ------------------------------------------------------

    gMsr2MeitreeOahUserChoices = msr2MeitreeOah::create (
      handler);
    assert(gMsr2MeitreeOahUserChoices != 0);

    gMsr2MeitreeOah =
      gMsr2MeitreeOahUserChoices;

    // prepare for measure detailed trace
    // ------------------------------------------------------

    /* JMI
      gMsr2MeitreeOahWithDetailedTrace =
        gMsr2MeitreeOah->
          createCloneWithDetailedTrace ();
          */

    pThisMethodHasBeenRun = true;
  }
}


}
