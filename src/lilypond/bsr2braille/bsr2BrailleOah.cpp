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
#include "bsr2BrailleOah.h"


using namespace std;

namespace MusicXML2
{

//_______________________________________________________________________________

S_bsr2BrailleOah gBsr2BrailleOah;
S_bsr2BrailleOah gBsr2BrailleOahUserChoices;
S_bsr2BrailleOah gBsr2BrailleOahWithDetailedTrace;

S_bsr2BrailleOah bsr2BrailleOah::create (
  S_oahHandler handlerUpLink)
{
  bsr2BrailleOah* o = new bsr2BrailleOah(
    handlerUpLink);
  assert(o!=0);
  return o;
}

bsr2BrailleOah::bsr2BrailleOah (
  S_oahHandler handlerUpLink)
  : oahGroup (
    "Bsr2Braille",
    "hbsr2brl", "help-bsr-to-braille",
R"(These options control the way BSR is translated to braille music.)",
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
  initializeBsr2BrailleOah (false);
}

bsr2BrailleOah::~bsr2BrailleOah ()
{}

#ifdef TRACE_OAH
void bsr2BrailleOah::initializeMusicXMLTraceOah (
  bool boolOptionsInitialValue)
{}
#endif

void bsr2BrailleOah::initializeMusicXMLHeaderOptions (
  bool boolOptionsInitialValue)
{}

void bsr2BrailleOah::initializeMusicXMLClefsKeysTimesOptions (
  bool boolOptionsInitialValue)
{}

void bsr2BrailleOah::initializeMusicXMMeasuresOptions (
  bool boolOptionsInitialValue)
{
}

void bsr2BrailleOah::initializeMusicXMLWordsOptions (
  bool boolOptionsInitialValue)
{}

void bsr2BrailleOah::initializeMusicXMLDynamicsAndWedgesOptions (
  bool boolOptionsInitialValue)
{}

void bsr2BrailleOah::initializeMusicXMLCombinedOptionsOptions (
  bool boolOptionsInitialValue)
{}

void bsr2BrailleOah::initializeMusicXMLLoopOptions (
  bool boolOptionsInitialValue)
{}

void bsr2BrailleOah::initializeBsr2BrailleOah (
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

S_bsr2BrailleOah bsr2BrailleOah::createCloneWithDetailedTrace ()
{
  S_bsr2BrailleOah
    clone =
      bsr2BrailleOah::create (0);
      // 0 not to have it inserted twice in the option handler

  // set the options handler upLink
  clone->fHandlerUpLink =
    fHandlerUpLink;

  return clone;
}

//______________________________________________________________________________
void bsr2BrailleOah::setAllMusicXMLTraceOah (
  bool boolOptionsInitialValue)
{
#ifdef TRACE_OAH
  // specific trace
  // JMI
#endif
}

//______________________________________________________________________________
void bsr2BrailleOah::enforceQuietness ()
{}

//______________________________________________________________________________
void bsr2BrailleOah::checkOptionsConsistency ()
{}

//______________________________________________________________________________
void bsr2BrailleOah::acceptIn (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    gLogOstream <<
      ".\\\" ==> bsr2BrailleOah::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_bsr2BrailleOah>*
    p =
      dynamic_cast<visitor<S_bsr2BrailleOah>*> (v)) {
        S_bsr2BrailleOah elem = this;

#ifdef TRACE_OAH
        if (gOahOah->fTraceOahVisitors) {
          gLogOstream <<
            ".\\\" ==> Launching bsr2BrailleOah::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void bsr2BrailleOah::acceptOut (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    gLogOstream <<
      ".\\\" ==> bsr2BrailleOah::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_bsr2BrailleOah>*
    p =
      dynamic_cast<visitor<S_bsr2BrailleOah>*> (v)) {
        S_bsr2BrailleOah elem = this;

#ifdef TRACE_OAH
        if (gOahOah->fTraceOahVisitors) {
          gLogOstream <<
            ".\\\" ==> Launching bsr2BrailleOah::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void bsr2BrailleOah::browseData (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    gLogOstream <<
      ".\\\" ==> bsr2BrailleOah::browseData ()" <<
      endl;
  }
#endif
}

//______________________________________________________________________________
void bsr2BrailleOah::printBsr2BrailleOahValues (int fieldWidth)
{
  gLogOstream <<
    "The MusicXML options are:" <<
    endl;

  gIndenter++;

  gIndenter--;
}

ostream& operator<< (ostream& os, const S_bsr2BrailleOah& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
void initializeBsr2BrailleOahHandling (
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

  gBsr2BrailleOahUserChoices = bsr2BrailleOah::create (
    handler);
  assert(gBsr2BrailleOahUserChoices != 0);

  gBsr2BrailleOah =
    gBsr2BrailleOahUserChoices;

  // prepare for measure detailed trace
  // ------------------------------------------------------

/* JMI
  gBsr2BrailleOahWithDetailedTrace =
    gBsr2BrailleOah->
      createCloneWithDetailedTrace ();
      */
}


}
