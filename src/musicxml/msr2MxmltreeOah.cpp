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
#include "msr2MxmltreeOah.h"


using namespace std;

namespace MusicXML2
{

//______________________________________________________________________________
S_msr2MxmltreeOah gMsr2MxmltreeOah;
S_msr2MxmltreeOah gMsr2MxmltreeOahUserChoices;
S_msr2MxmltreeOah gMsr2MxmltreeOahWithDetailedTrace;

S_msr2MxmltreeOah msr2MxmltreeOah::create (
  S_oahHandler handlerUpLink)
{
  msr2MxmltreeOah* o = new msr2MxmltreeOah(
    handlerUpLink);
  assert(o!=0);
  return o;
}

msr2MxmltreeOah::msr2MxmltreeOah (
  S_oahHandler handlerUpLink)
  : oahGroup (
    "Msr2MxmlTree",
    "hmsr2mxmlt", "help-msr-to-mxmltree",
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
  initializeMsr2MxmltreeOah (false);
}

msr2MxmltreeOah::~msr2MxmltreeOah ()
{}

#ifdef TRACE_OAH
void msr2MxmltreeOah::initializeMsr2MxmlTreeTraceOah (
  bool boolOptionsInitialValue)
{}
#endif

void msr2MxmltreeOah::initializeCodeGenerationOptions (
  bool boolOptionsInitialValue)
{
  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Code generation",
        "hmxmlcg", "help-musicxml-code-generation",
R"()",
      kElementVisibilityWhole,
      this);

  appendSubGroupToGroup (subGroup);

  // comments

  fMusicXMLComments = boolOptionsInitialValue;

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "xmlcom", "musicxml-comments",
R"(Generate comments showing the structure of the score.)",
        "MusicXMLComments",
        fMusicXMLComments));
}

void msr2MxmltreeOah::initializeMsr2MxmltreeOah (
  bool boolOptionsInitialValue)
{
#ifdef TRACE_OAH
  // trace
  // --------------------------------------
  initializeMsr2MxmlTreeTraceOah (
    boolOptionsInitialValue);
#endif

  // code generation
  // --------------------------------------
  initializeCodeGenerationOptions (
    boolOptionsInitialValue);
}

S_msr2MxmltreeOah msr2MxmltreeOah::createCloneWithDetailedTrace ()
{
  S_msr2MxmltreeOah
    clone =
      msr2MxmltreeOah::create (0);
      // 0 not to have it inserted twice in the option handler

  // set the options handler upLink
  clone->fHandlerUpLink =
    fHandlerUpLink;

  return clone;
}

//______________________________________________________________________________
void msr2MxmltreeOah::setAllMusicXMLTraceOah (
  bool boolOptionsInitialValue)
{
#ifdef TRACE_OAH
  // specific trace
  // JMI
#endif
}

//______________________________________________________________________________
void msr2MxmltreeOah::enforceQuietness ()
{}

//______________________________________________________________________________
void msr2MxmltreeOah::checkOptionsConsistency ()
{}

//______________________________________________________________________________
void msr2MxmltreeOah::acceptIn (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    gLogOstream <<
      ".\\\" ==> msr2MxmltreeOah::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_msr2MxmltreeOah>*
    p =
      dynamic_cast<visitor<S_msr2MxmltreeOah>*> (v)) {
        S_msr2MxmltreeOah elem = this;

#ifdef TRACE_OAH
        if (gOahOah->fTraceOahVisitors) {
          gLogOstream <<
            ".\\\" ==> Launching msr2MxmltreeOah::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void msr2MxmltreeOah::acceptOut (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    gLogOstream <<
      ".\\\" ==> msr2MxmltreeOah::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_msr2MxmltreeOah>*
    p =
      dynamic_cast<visitor<S_msr2MxmltreeOah>*> (v)) {
        S_msr2MxmltreeOah elem = this;

#ifdef TRACE_OAH
        if (gOahOah->fTraceOahVisitors) {
          gLogOstream <<
            ".\\\" ==> Launching msr2MxmltreeOah::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void msr2MxmltreeOah::browseData (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    gLogOstream <<
      ".\\\" ==> msr2MxmltreeOah::browseData ()" <<
      endl;
  }
#endif
}

//______________________________________________________________________________
void msr2MxmltreeOah::printMsr2MxmltreeOahValues (int fieldWidth)
{
  gLogOstream <<
    "The MusicXML options are:" <<
    endl;

  gIndenter++;

  gIndenter--;
}

ostream& operator<< (ostream& os, const S_msr2MxmltreeOah& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
void initializeMsr2MxmltreeOahHandling (
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
  static bool initializeMsr2MxmltreeOahHandlingHasBeenRun = false;

  if (! initializeMsr2MxmltreeOahHandlingHasBeenRun) {
    // create the MusicXML options
    // ------------------------------------------------------

    gMsr2MxmltreeOahUserChoices = msr2MxmltreeOah::create (
      handler);
    assert(gMsr2MxmltreeOahUserChoices != 0);

    gMsr2MxmltreeOah =
      gMsr2MxmltreeOahUserChoices;

    // prepare for measure detailed trace
    // ------------------------------------------------------

    /* JMI
      gMsr2MxmltreeOahWithDetailedTrace =
        gMsr2MxmltreeOah->
          createCloneWithDetailedTrace ();
          */

    initializeMsr2MxmltreeOahHandlingHasBeenRun = true;
  }
}


}
