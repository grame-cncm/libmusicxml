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

#include "setTraceOahIfDesired.h"
#ifdef TRACE_OAH
  #include "traceOah.h"
#endif

#include "messagesHandling.h"

#include "generalOah.h"
#include "msr2BsrOah.h"


using namespace std;

namespace MusicXML2
{
//_______________________________________________________________________________

S_msr2BsrOah gMsr2BsrOah;
S_msr2BsrOah gMsr2BsrOahUserChoices;
S_msr2BsrOah gMsr2BsrOahWithDetailedTrace;

S_msr2BsrOah msr2BsrOah::create (
  S_oahHandler handlerUpLink)
{
  msr2BsrOah* o = new msr2BsrOah(
    handlerUpLink);
  assert(o!=0);
  return o;
}

msr2BsrOah::msr2BsrOah (
  S_oahHandler handlerUpLink)
  : oahGroup (
    "Msr2Bsr",
    "hmsr2bsr", "help-msr-to-bsr",
R"(These options control the way MSR is translated to BSR.)",
    kElementVisibilityAlways,
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
  initializeMsr2BsrOah (false);
}

msr2BsrOah::~msr2BsrOah ()
{}

#ifdef TRACE_OAH
void msr2BsrOah::initializeMusicXMLTraceOah (
  bool boolOptionsInitialValue)
{}
#endif

void msr2BsrOah::initializeMusicXMLHeaderOptions (
  bool boolOptionsInitialValue)
{}

void msr2BsrOah::initializeMusicXMLClefsKeysTimesOptions (
  bool boolOptionsInitialValue)
{}

void msr2BsrOah::initializeMusicXMMeasuresOptions (
  bool boolOptionsInitialValue)
{
}

void msr2BsrOah::initializeMusicXMLWordsOptions (
  bool boolOptionsInitialValue)
{}

void msr2BsrOah::initializeMusicXMLDynamicsAndWedgesOptions (
  bool boolOptionsInitialValue)
{}

void msr2BsrOah::initializeMusicXMLCombinedOptionsOptions (
  bool boolOptionsInitialValue)
{}

void msr2BsrOah::initializeMusicXMLLoopOptions (
  bool boolOptionsInitialValue)
{}

void msr2BsrOah::initializeMsr2BsrOah (
  bool boolOptionsInitialValue)
{
#ifdef TRACE_OAH
  // trace
  // --------------------------------------
  initializeMusicXMLTraceOah (
    boolOptionsInitialValue);
#endif

  // header
  // --------------------------------------
  initializeMusicXMLHeaderOptions (
    boolOptionsInitialValue);

  // clefs, keys, times
  // --------------------------------------
  initializeMusicXMLClefsKeysTimesOptions (
    boolOptionsInitialValue);

  // measures
  // --------------------------------------
  initializeMusicXMMeasuresOptions (
    boolOptionsInitialValue);

  // words
  // --------------------------------------
  initializeMusicXMLWordsOptions (
    boolOptionsInitialValue);

  // dynamics and wedges
  // --------------------------------------
  initializeMusicXMLDynamicsAndWedgesOptions (
    boolOptionsInitialValue);

  // combined options
  // --------------------------------------
  initializeMusicXMLCombinedOptionsOptions (
    boolOptionsInitialValue);

  // loop to do JMI
  // --------------------------------------

  initializeMusicXMLLoopOptions (
    boolOptionsInitialValue);
}

S_msr2BsrOah msr2BsrOah::createCloneWithDetailedTrace ()
{
  S_msr2BsrOah
    clone =
      msr2BsrOah::create (0);
      // 0 not to have it inserted twice in the option handler

  // set the options handler upLink
  clone->fHandlerUpLink =
    fHandlerUpLink;

  return clone;
}

//______________________________________________________________________________
void msr2BsrOah::setAllMusicXMLTraceOah (
  bool boolOptionsInitialValue)
{
#ifdef TRACE_OAH
  // specific trace
  // JMI
#endif
}

//______________________________________________________________________________
void msr2BsrOah::enforceQuietness ()
{}

//______________________________________________________________________________
void msr2BsrOah::checkOptionsConsistency ()
{}

//______________________________________________________________________________
void msr2BsrOah::acceptIn (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    gLogOstream <<
      ".\\\" ==> msr2BsrOah::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_msr2BsrOah>*
    p =
      dynamic_cast<visitor<S_msr2BsrOah>*> (v)) {
        S_msr2BsrOah elem = this;

#ifdef TRACE_OAH
        if (gOahOah->fTraceOahVisitors) {
          gLogOstream <<
            ".\\\" ==> Launching msr2BsrOah::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void msr2BsrOah::acceptOut (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    gLogOstream <<
      ".\\\" ==> msr2BsrOah::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_msr2BsrOah>*
    p =
      dynamic_cast<visitor<S_msr2BsrOah>*> (v)) {
        S_msr2BsrOah elem = this;

#ifdef TRACE_OAH
        if (gOahOah->fTraceOahVisitors) {
          gLogOstream <<
            ".\\\" ==> Launching msr2BsrOah::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void msr2BsrOah::browseData (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    gLogOstream <<
      ".\\\" ==> msr2BsrOah::browseData ()" <<
      endl;
  }
#endif
}

//______________________________________________________________________________
void msr2BsrOah::printMsr2BsrOahValues (int fieldWidth)
{
  gLogOstream <<
    "The MusicXML options are:" <<
    endl;

  gIndenter++;

  gIndenter--;
}

ostream& operator<< (ostream& os, const S_msr2BsrOah& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
void initializeMsr2BsrOahHandling (
  S_oahHandler handler)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceOah && ! gGeneralOah->fQuiet) {
    gLogOstream <<
      "Initializing MusicXML options handling" <<
      endl;
  }
#endif

  // create the MusicXML options
  // ------------------------------------------------------

  gMsr2BsrOahUserChoices = msr2BsrOah::create (
    handler);
  assert(gMsr2BsrOahUserChoices != 0);

  gMsr2BsrOah =
    gMsr2BsrOahUserChoices;

  // prepare for measure detailed trace
  // ------------------------------------------------------

/* JMI
  gMsr2BsrOahWithDetailedTrace =
    gMsr2BsrOah->
      createCloneWithDetailedTrace ();
      */
}


}
