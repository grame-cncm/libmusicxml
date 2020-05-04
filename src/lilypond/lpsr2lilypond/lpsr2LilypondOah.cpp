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
#include "lpsr2LilypondOah.h"


using namespace std;

namespace MusicXML2
{
//_______________________________________________________________________________

S_lpsr2LilyPondOah gLpsr2LilyPondOah;
S_lpsr2LilyPondOah gLpsr2LilyPondOahUserChoices;
S_lpsr2LilyPondOah gLpsr2LilyPondOahWithDetailedTrace;

S_lpsr2LilyPondOah lpsr2LilyPondOah::create (
  S_oahHandler handlerUpLink)
{
  lpsr2LilyPondOah* o = new lpsr2LilyPondOah(
    handlerUpLink);
  assert(o!=0);
  return o;
}

lpsr2LilyPondOah::lpsr2LilyPondOah (
  S_oahHandler handlerUpLink)
  : oahGroup (
    "Lpsr2LilyPond",
    "hlpsr2ly", "help-lpsr-to-lilypond",
R"(These options control the way LPSR is translated to LilyPond.)",
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
  initializeLpsr2LilyPondOah (false);
}

lpsr2LilyPondOah::~lpsr2LilyPondOah ()
{}

#ifdef TRACE_OAH
void lpsr2LilyPondOah::initializeMusicXMLTraceOah (
  bool boolOptionsInitialValue)
{}
#endif

void lpsr2LilyPondOah::initializeMusicXMLHeaderOptions (
  bool boolOptionsInitialValue)
{}

void lpsr2LilyPondOah::initializeMusicXMLClefsKeysTimesOptions (
  bool boolOptionsInitialValue)
{}

void lpsr2LilyPondOah::initializeMusicXMMeasuresOptions (
  bool boolOptionsInitialValue)
{
}

void lpsr2LilyPondOah::initializeMusicXMLWordsOptions (
  bool boolOptionsInitialValue)
{}

void lpsr2LilyPondOah::initializeMusicXMLDynamicsAndWedgesOptions (
  bool boolOptionsInitialValue)
{}

void lpsr2LilyPondOah::initializeMusicXMLCombinedOptionsOptions (
  bool boolOptionsInitialValue)
{}

void lpsr2LilyPondOah::initializeMusicXMLLoopOptions (
  bool boolOptionsInitialValue)
{}

void lpsr2LilyPondOah::initializeLpsr2LilyPondOah (
  bool boolOptionsInitialValue)
{
#ifdef TRACE_OAH
  // trace
  // --------------------------------------
  initializeMusicXMLTraceOah (
    boolOptionsInitialValue);
#endif
}

S_lpsr2LilyPondOah lpsr2LilyPondOah::createCloneWithDetailedTrace ()
{
  S_lpsr2LilyPondOah
    clone =
      lpsr2LilyPondOah::create (0);
      // 0 not to have it inserted twice in the option handler

  // set the options handler upLink
  clone->fHandlerUpLink =
    fHandlerUpLink;

  return clone;
}

//______________________________________________________________________________
void lpsr2LilyPondOah::setAllMusicXMLTraceOah (
  bool boolOptionsInitialValue)
{
#ifdef TRACE_OAH
  // specific trace
  // JMI
#endif
}

//______________________________________________________________________________
void lpsr2LilyPondOah::enforceQuietness ()
{}

//______________________________________________________________________________
void lpsr2LilyPondOah::checkOptionsConsistency ()
{}

//______________________________________________________________________________
void lpsr2LilyPondOah::acceptIn (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    gLogOstream <<
      ".\\\" ==> lpsr2LilyPondOah::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_lpsr2LilyPondOah>*
    p =
      dynamic_cast<visitor<S_lpsr2LilyPondOah>*> (v)) {
        S_lpsr2LilyPondOah elem = this;

#ifdef TRACE_OAH
        if (gOahOah->fTraceOahVisitors) {
          gLogOstream <<
            ".\\\" ==> Launching lpsr2LilyPondOah::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void lpsr2LilyPondOah::acceptOut (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    gLogOstream <<
      ".\\\" ==> lpsr2LilyPondOah::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_lpsr2LilyPondOah>*
    p =
      dynamic_cast<visitor<S_lpsr2LilyPondOah>*> (v)) {
        S_lpsr2LilyPondOah elem = this;

#ifdef TRACE_OAH
        if (gOahOah->fTraceOahVisitors) {
          gLogOstream <<
            ".\\\" ==> Launching lpsr2LilyPondOah::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void lpsr2LilyPondOah::browseData (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    gLogOstream <<
      ".\\\" ==> lpsr2LilyPondOah::browseData ()" <<
      endl;
  }
#endif
}

//______________________________________________________________________________
void lpsr2LilyPondOah::printLpsr2LilyPondOahValues (int fieldWidth)
{
  gLogOstream <<
    "The MusicXML options are:" <<
    endl;

  gIndenter++;

  gIndenter--;
}

ostream& operator<< (ostream& os, const S_lpsr2LilyPondOah& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
void initializeLpsr2LilyPondOahHandling (
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

  gLpsr2LilyPondOahUserChoices = lpsr2LilyPondOah::create (
    handler);
  assert(gLpsr2LilyPondOahUserChoices != 0);

  gLpsr2LilyPondOah =
    gLpsr2LilyPondOahUserChoices;

  // prepare for measure detailed trace
  // ------------------------------------------------------

/* JMI
  gLpsr2LilyPondOahWithDetailedTrace =
    gLpsr2LilyPondOah->
      createCloneWithDetailedTrace ();
      */
}


}
