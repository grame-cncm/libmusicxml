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
#include "msr2BmmltreeOah.h"


using namespace std;

namespace MusicXML2
{

//______________________________________________________________________________
S_msr2BmmltreeOah gMsr2BmmltreeOah;
S_msr2BmmltreeOah gMsr2BmmltreeOahUserChoices;
S_msr2BmmltreeOah gMsr2BmmltreeOahWithDetailedTrace;

S_msr2BmmltreeOah msr2BmmltreeOah::create (
  S_oahHandler handlerUpLink)
{
  msr2BmmltreeOah* o = new msr2BmmltreeOah(
    handlerUpLink);
  assert(o!=0);
  return o;
}

msr2BmmltreeOah::msr2BmmltreeOah (
  S_oahHandler handlerUpLink)
  : oahGroup (
    "Msr2BmmlTree",
    "hmsr2bmmlt", "help-msr-to-bmmltree",
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
  initializeMsr2BmmltreeOah (false);
}

msr2BmmltreeOah::~msr2BmmltreeOah ()
{}

#ifdef TRACE_OAH
void msr2BmmltreeOah::initializeMsr2BmmlTreeTraceOah (
  bool boolOptionsInitialValue)
{}
#endif

void msr2BmmltreeOah::initializeCodeGenerationOptions (
  bool boolOptionsInitialValue)
{
  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Code generation",
        "hbmmlcg", "help-bmml-code-generation",
R"()",
      kElementVisibilityWhole,
      this);

  appendSubGroupToGroup (subGroup);

  // comments

  fMusicXMLComments = boolOptionsInitialValue;

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "bmmlcom", "bmml-comments",
R"(Generate comments showing the structure of the score.)",
        "MusicXMLComments",
        fMusicXMLComments));
}

void msr2BmmltreeOah::initializeMsr2BmmltreeOah (
  bool boolOptionsInitialValue)
{
#ifdef TRACE_OAH
  // trace
  // --------------------------------------
  initializeMsr2BmmlTreeTraceOah (
    boolOptionsInitialValue);
#endif

  // code generation
  // --------------------------------------
  initializeCodeGenerationOptions (
    boolOptionsInitialValue);
}

S_msr2BmmltreeOah msr2BmmltreeOah::createCloneWithDetailedTrace ()
{
  S_msr2BmmltreeOah
    clone =
      msr2BmmltreeOah::create (0);
      // 0 not to have it inserted twice in the option handler

  // set the options handler upLink
  clone->fHandlerUpLink =
    fHandlerUpLink;

  return clone;
}

//______________________________________________________________________________
void msr2BmmltreeOah::setAllMusicXMLTraceOah (
  bool boolOptionsInitialValue)
{
#ifdef TRACE_OAH
  // specific trace
  // JMI
#endif
}

//______________________________________________________________________________
void msr2BmmltreeOah::enforceQuietness ()
{}

//______________________________________________________________________________
void msr2BmmltreeOah::checkOptionsConsistency ()
{}

//______________________________________________________________________________
void msr2BmmltreeOah::acceptIn (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    gLogOstream <<
      ".\\\" ==> msr2BmmltreeOah::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_msr2BmmltreeOah>*
    p =
      dynamic_cast<visitor<S_msr2BmmltreeOah>*> (v)) {
        S_msr2BmmltreeOah elem = this;

#ifdef TRACE_OAH
        if (gOahOah->fTraceOahVisitors) {
          gLogOstream <<
            ".\\\" ==> Launching msr2BmmltreeOah::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void msr2BmmltreeOah::acceptOut (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    gLogOstream <<
      ".\\\" ==> msr2BmmltreeOah::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_msr2BmmltreeOah>*
    p =
      dynamic_cast<visitor<S_msr2BmmltreeOah>*> (v)) {
        S_msr2BmmltreeOah elem = this;

#ifdef TRACE_OAH
        if (gOahOah->fTraceOahVisitors) {
          gLogOstream <<
            ".\\\" ==> Launching msr2BmmltreeOah::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void msr2BmmltreeOah::browseData (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    gLogOstream <<
      ".\\\" ==> msr2BmmltreeOah::browseData ()" <<
      endl;
  }
#endif
}

//______________________________________________________________________________
void msr2BmmltreeOah::printMsr2BmmltreeOahValues (int fieldWidth)
{
  gLogOstream <<
    "The MusicXML options are:" <<
    endl;

  gIndenter++;

  gIndenter--;
}

ostream& operator<< (ostream& os, const S_msr2BmmltreeOah& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
void initializeMsr2BmmltreeOahHandling (
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

    gMsr2BmmltreeOahUserChoices = msr2BmmltreeOah::create (
      handler);
    assert(gMsr2BmmltreeOahUserChoices != 0);

    gMsr2BmmltreeOah =
      gMsr2BmmltreeOahUserChoices;

    // prepare for measure detailed trace
    // ------------------------------------------------------

    /* JMI
      gMsr2BmmltreeOahWithDetailedTrace =
        gMsr2BmmltreeOah->
          createCloneWithDetailedTrace ();
          */

    pThisMethodHasBeenRun = true;
  }
}


}
