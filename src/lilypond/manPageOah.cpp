/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#include <sstream>
#include <iomanip>      // setw, setprecision, ...

#include "utilities.h"

#include "setTraceOahIfDesired.h"
#ifdef TRACE_OAH
  #include "traceOah.h"
#endif

#include "generalOah.h"

#include "manPageOah.h"

#include "oah2ManPageGeneratorInterface.h"

using namespace std;

namespace MusicXML2
{

//______________________________________________________________________________
S_manPageGenerateAtom manPageGenerateAtom::create (
  string shortName,
  string longName,
  string description)
{
  manPageGenerateAtom* o = new
    manPageGenerateAtom (
      shortName,
      longName,
      description);
  assert(o!=0);
  return o;
}

manPageGenerateAtom::manPageGenerateAtom (
  string shortName,
  string longName,
  string description)
  : oahAtom (
      shortName,
      longName,
      description)
{}

manPageGenerateAtom::~manPageGenerateAtom ()
{}

S_oahValuedAtom manPageGenerateAtom::handleOptionUnderName (
  string   optionName,
  ostream& os)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceOah) {
    gLogOstream <<
      "==> option '" << optionName << "' is a manPageGenerateAtom" <<
      endl;
  }
#endif

  // handle it at once
  generateManPageData (os);

  // no option value is needed
  return nullptr;
}

void manPageGenerateAtom::acceptIn (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    gLogOstream <<
      "% ==> manPageGenerateAtom::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_manPageGenerateAtom>*
    p =
      dynamic_cast<visitor<S_manPageGenerateAtom>*> (v)) {
        S_manPageGenerateAtom elem = this;

#ifdef TRACE_OAH
        if (gOahOah->fTraceOahVisitors) {
          gLogOstream <<
            "% ==> Launching manPageGenerateAtom::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void manPageGenerateAtom::acceptOut (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    gLogOstream <<
      "% ==> manPageGenerateAtom::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_manPageGenerateAtom>*
    p =
      dynamic_cast<visitor<S_manPageGenerateAtom>*> (v)) {
        S_manPageGenerateAtom elem = this;

#ifdef TRACE_OAH
        if (gOahOah->fTraceOahVisitors) {
          gLogOstream <<
            "% ==> Launching manPageGenerateAtom::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void manPageGenerateAtom::browseData (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    gLogOstream <<
      "% ==> manPageGenerateAtom::browseData ()" <<
      endl;
  }
#endif
}

void manPageGenerateAtom::print (ostream& os) const
{
  const int fieldWidth = K_OPTIONS_FIELD_WIDTH;

  os <<
    "manPageGenerateAtom:" <<
    endl;

  gIndenter++;

  oahElement::printOptionEssentials (
    os, fieldWidth);

  gIndenter++;
  os <<
    gIndenter.indentMultiLineString (
      fDescription) <<
    endl;
  gIndenter--;

  gIndenter--;
}

void manPageGenerateAtom::generateManPageData (ostream& os) const
{
  S_oahHandler
    handler =
      fSubGroupUpLink->
        getGroupUpLink ()->
          getHandlerUpLink ();

  // generate the man page from the OAH handler
  generateManPageFromOahHandler (
    handler,
    gManPageOah,
    gLogOstream,
    os);

  // register 'generate man page' action in options groups's options handler upLink
  handler->
    setOptionsHandlerFoundAHelpOption ();
}

void manPageGenerateAtom::printAtomOptionsValues (
  ostream& os,
  int      valueFieldWidth) const
{
  // nothing to print here
}

ostream& operator<< (ostream& os, const S_manPageGenerateAtom& elt)
{
  elt->print (os);
  return os;
}

//_______________________________________________________________________________
S_manPageOah gManPageOah;
S_manPageOah gManPageOahUserChoices;
S_manPageOah gManPageOahWithDetailedTrace;

S_manPageOah manPageOah::create (
  S_oahHandler handlerUpLink)
{
  manPageOah* o = new manPageOah (
    handlerUpLink);
  assert(o!=0);
  return o;
}

manPageOah::manPageOah (
  S_oahHandler handlerUpLink)
  : oahGroup (
    "Man page",
    "hmp", "help-man-page",
R"(These options control the way the man page is generated.)",
    kElementVisibilityAlways,
    handlerUpLink)
{
  // append this man page group to the OAH handler
  // if relevant
  if (handlerUpLink) {
    handlerUpLink->
      appendGroupToHandler (this);
  }

  // initialize it
  initializeManPageOah (false);
}

manPageOah::~manPageOah ()
{}

void manPageOah::initializeManPageDisplayOptions (
  bool boolOptionsInitialValue)
{
  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Display",
        "hmpd", "help-man-page-display",
R"()",
      kElementVisibilityAlways,
      this);

  appendSubGroupToGroup (subGroup);

/* JMI
  // OAH

  fDisplayOah = boolOptionsInitialValue;

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "doah", "display-oah",
R"(Write the contents of the OAH data to standard error.)",
        "displayOah",
        fDisplayOah));
*/

  // FOO
  subGroup->
    appendAtomToSubGroup (
      manPageGenerateAtom::create (
        "gmp", "generate-man-page",
R"(Write man page data to standard output.)"));
}

#ifdef TRACE_OAH
void manPageOah::initializeManPageTraceOah (
  bool boolOptionsInitialValue)
{
  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Trace",
        "hmpt", "help-man-page-trace",
R"(  Note: the options in this group imply '-tmp, -trace-man-page'.)",
// JMI        kElementVisibilityHiddenByDefault,
      kElementVisibilityAlways,
      this);

  appendSubGroupToGroup (subGroup);

  // JMI ???
}
#endif

void manPageOah::initializeManPageOah (
  bool boolOptionsInitialValue)
{
  // display
  // --------------------------------------
  initializeManPageDisplayOptions (
    boolOptionsInitialValue);

#ifdef TRACE_OAH
  // trace
  // --------------------------------------
  initializeManPageTraceOah (
    boolOptionsInitialValue);
#endif
}

S_manPageOah manPageOah::createCloneWithDetailedTrace ()
{
  S_manPageOah
    clone =
      manPageOah::create (0);
      // 0 not to have it inserted twice in the option handler

  // set the OAH handler upLink
  clone->fHandlerUpLink =
    fHandlerUpLink;

/* JMI
  // display
  // --------------------------------------

  clone->fDisplayOah =
    true;
*/

  return clone;
}

//______________________________________________________________________________
void manPageOah::enforceQuietness ()
{
// JMI  fDisplayOah = false;
}

//______________________________________________________________________________
void manPageOah::checkOptionsConsistency ()
{
  // JMI
}

//______________________________________________________________________________
void manPageOah::acceptIn (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    gLogOstream <<
      "% ==> manPageOah::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_manPageOah>*
    p =
      dynamic_cast<visitor<S_manPageOah>*> (v)) {
        S_manPageOah elem = this;

#ifdef TRACE_OAH
        if (gOahOah->fTraceOahVisitors) {
          gLogOstream <<
            "% ==> Launching manPageOah::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void manPageOah::acceptOut (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    gLogOstream <<
      "% ==> manPageOah::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_manPageOah>*
    p =
      dynamic_cast<visitor<S_manPageOah>*> (v)) {
        S_manPageOah elem = this;

#ifdef TRACE_OAH
        if (gOahOah->fTraceOahVisitors) {
          gLogOstream <<
            "% ==> Launching manPageOah::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void manPageOah::browseData (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    gLogOstream <<
      "% ==> manPageOah::browseData ()" <<
      endl;
  }
#endif
}

//______________________________________________________________________________
void manPageOah::printManPageOahValues (int fieldWidth)
{
  gLogOstream <<
    "The OAH options are:" <<
    endl;

  gIndenter++;

  // JMI ???

  gIndenter--;
}

ostream& operator<< (ostream& os, const S_manPageOah& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
void initializeManPageOahHandling (
  S_oahHandler handler)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceOah && ! gGeneralOah->fQuiet) {
    gLogOstream <<
      "Initializing man page options handling" <<
      endl;
  }
#endif

  // create the bsr variables
  // ------------------------------------------------------

  gManPageOahUserChoices = manPageOah::create (
    handler);
  assert(gManPageOahUserChoices != 0);

  gManPageOah =
    gManPageOahUserChoices;

  // prepare for measure detailed trace
  // ------------------------------------------------------
/* JMI
  gManPageOahWithDetailedTrace =
    gManPageOah->
      createCloneWithDetailedTrace ();
      */
}


}
