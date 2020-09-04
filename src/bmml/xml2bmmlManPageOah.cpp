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

#include <regex>

#include "utilities.h"

#include "setTraceOahIfDesired.h"
#ifdef TRACE_OAH
  #include "traceOah.h"
#endif

#include "generalOah.h"

#include "xml2bmmlManPageOah.h"


using namespace std;

namespace MusicXML2
{

//______________________________________________________________________________
S_xml2bmmlManPageGenerateAtom xml2bmmlManPageGenerateAtom::create (
  string                 shortName,
  string                 longName,
  string                 description,
  S_oahVisitor           theOahVisitor)
{
  xml2bmmlManPageGenerateAtom* o = new
    xml2bmmlManPageGenerateAtom (
      shortName,
      longName,
      description,
      theOahVisitor);
  assert(o!=0);
  return o;
}

xml2bmmlManPageGenerateAtom::xml2bmmlManPageGenerateAtom (
  string                 shortName,
  string                 longName,
  string                 description,
  S_oahVisitor           theOahVisitor)
  : oahAtom (
      shortName,
      longName,
      description,
      kElementValueExpectedNo)
{
  fOahVisitor = theOahVisitor;
}

xml2bmmlManPageGenerateAtom::~xml2bmmlManPageGenerateAtom ()
{}

S_oahValuedAtom xml2bmmlManPageGenerateAtom::handleOptionUnderName (
  string   optionName,
  ostream& os)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceOah) {
    gLogOstream <<
      "==> option '" << optionName << "' is a xml2bmmlManPageGenerateAtom" <<
      endl;
  }
#endif

  // handle it at once
  generateManPageData (os);

  // no option value is needed
  return nullptr;
}

void xml2bmmlManPageGenerateAtom::acceptIn (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    gLogOstream <<
      ".\\\" ==> xml2bmmlManPageGenerateAtom::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_xml2bmmlManPageGenerateAtom>*
    p =
      dynamic_cast<visitor<S_xml2bmmlManPageGenerateAtom>*> (v)) {
        S_xml2bmmlManPageGenerateAtom elem = this;

#ifdef TRACE_OAH
        if (gOahOah->fTraceOahVisitors) {
          gLogOstream <<
            ".\\\" ==> Launching xml2bmmlManPageGenerateAtom::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void xml2bmmlManPageGenerateAtom::acceptOut (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    gLogOstream <<
      ".\\\" ==> xml2bmmlManPageGenerateAtom::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_xml2bmmlManPageGenerateAtom>*
    p =
      dynamic_cast<visitor<S_xml2bmmlManPageGenerateAtom>*> (v)) {
        S_xml2bmmlManPageGenerateAtom elem = this;

#ifdef TRACE_OAH
        if (gOahOah->fTraceOahVisitors) {
          gLogOstream <<
            ".\\\" ==> Launching xml2bmmlManPageGenerateAtom::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void xml2bmmlManPageGenerateAtom::browseData (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    gLogOstream <<
      ".\\\" ==> xml2bmmlManPageGenerateAtom::browseData ()" <<
      endl;
  }
#endif
}

void xml2bmmlManPageGenerateAtom::print (ostream& os) const
{
  const int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "xml2bmmlManPageGenerateAtom:" <<
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

void xml2bmmlManPageGenerateAtom::generateManPageData (ostream& os) const
{
  // generate the man page from the OAH handler
  fOahVisitor->visitTheHandler ();

  // register 'generate man page' action in options groups's options handler upLink
  S_oahHandler
    handler =
      fSubGroupUpLink->
        getGroupUpLink ()->
          getHandlerUpLink ();

  handler->
    setOahHandlerFoundAHelpOption ();
}

void xml2bmmlManPageGenerateAtom::printAtomOptionsValues (
  ostream& os,
  int      valueFieldWidth) const
{
  // nothing to print here
}

ostream& operator<< (ostream& os, const S_xml2bmmlManPageGenerateAtom& elt)
{
  elt->print (os);
  return os;
}

//_______________________________________________________________________________
S_xml2bmmlManPageOah gXml2bmmlManPageOah;
S_xml2bmmlManPageOah gXml2bmmlManPageOahUserChoices;
S_xml2bmmlManPageOah gXml2bmmlManPageOahWithDetailedTrace;

S_xml2bmmlManPageOah xml2bmmlManPageOah::create (
  S_oahHandler           handlerUpLink,
  S_oahVisitor theOah2ManPageGenerator)
{
  xml2bmmlManPageOah* o = new xml2bmmlManPageOah (
    handlerUpLink,
    theOah2ManPageGenerator);
  assert(o!=0);
  return o;
}

xml2bmmlManPageOah::xml2bmmlManPageOah (
  S_oahHandler           handlerUpLink,
  S_oahVisitor theOah2ManPageGenerator)
  : oahGroup (
    "Man page",
    "hmp", "help-man-page",
R"(These options control the way the man page is generated.)",
    kElementVisibilityWhole,
    handlerUpLink)
{
  // append this man page group to the OAH handler
  // if relevant
  if (handlerUpLink) {
    handlerUpLink->
      appendGroupToHandler (this);
  }

  // initialize it
  initializeXml2bmmlManPageOah (
    false,
    theOah2ManPageGenerator);
}

xml2bmmlManPageOah::~xml2bmmlManPageOah ()
{}

void xml2bmmlManPageOah::initializeManPageGenerateOptions (
  bool boolOptionsInitialValue,
  S_oahVisitor
       theOah2ManPageGenerator)
{
  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Generate",
        "hgmp", "help-generate-man-page",
R"()",
      kElementVisibilityWhole,
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

  // generate man page
  fOahVisitor = theOah2ManPageGenerator;

  subGroup->
    appendAtomToSubGroup (
      xml2bmmlManPageGenerateAtom::create (
        "gmp", "generate-man-page",
        regex_replace (
R"(Write man page data for EXECUTABLE to standard output.)",
          regex ("EXECUTABLE"),
          gOahOah->fHandlerExecutableName),
        fOahVisitor));
}

#ifdef TRACE_OAH
void xml2bmmlManPageOah::initializeManPageTraceOah (
  bool boolOptionsInitialValue)
{
  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Trace",
        "hmpt", "help-man-page-trace",
R"(  Note: the options in this group imply '-tmp, -trace-man-page'.)",
// JMI        kElementVisibilityHeaderOnly,
      kElementVisibilityWhole,
      this);

  appendSubGroupToGroup (subGroup);

  // JMI ???
}
#endif

void xml2bmmlManPageOah::initializeXml2bmmlManPageOah (
  bool boolOptionsInitialValue,
  S_oahVisitor
       theOah2ManPageGenerator)
{
  // display
  // --------------------------------------
  initializeManPageGenerateOptions (
    boolOptionsInitialValue,
    theOah2ManPageGenerator);

#ifdef TRACE_OAH
  // trace
  // --------------------------------------
  initializeManPageTraceOah (
    boolOptionsInitialValue);
#endif
}

S_xml2bmmlManPageOah xml2bmmlManPageOah::createCloneWithDetailedTrace ()
{
  S_xml2bmmlManPageOah
    clone =
      xml2bmmlManPageOah::create (
        0, // 0 not to have it inserted twice in the option handler
        fOahVisitor);

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
void xml2bmmlManPageOah::enforceQuietness ()
{
// JMI  fDisplayOah = false;
}

//______________________________________________________________________________
void xml2bmmlManPageOah::checkOptionsConsistency ()
{
  // JMI
}

//______________________________________________________________________________
void xml2bmmlManPageOah::acceptIn (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    gLogOstream <<
      ".\\\" ==> xml2bmmlManPageOah::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_xml2bmmlManPageOah>*
    p =
      dynamic_cast<visitor<S_xml2bmmlManPageOah>*> (v)) {
        S_xml2bmmlManPageOah elem = this;

#ifdef TRACE_OAH
        if (gOahOah->fTraceOahVisitors) {
          gLogOstream <<
            ".\\\" ==> Launching xml2bmmlManPageOah::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void xml2bmmlManPageOah::acceptOut (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    gLogOstream <<
      ".\\\" ==> xml2bmmlManPageOah::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_xml2bmmlManPageOah>*
    p =
      dynamic_cast<visitor<S_xml2bmmlManPageOah>*> (v)) {
        S_xml2bmmlManPageOah elem = this;

#ifdef TRACE_OAH
        if (gOahOah->fTraceOahVisitors) {
          gLogOstream <<
            ".\\\" ==> Launching xml2bmmlManPageOah::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void xml2bmmlManPageOah::browseData (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    gLogOstream <<
      ".\\\" ==> xml2bmmlManPageOah::browseData ()" <<
      endl;
  }
#endif
}

//______________________________________________________________________________
void xml2bmmlManPageOah::printManPageOahValues (int fieldWidth)
{
  gLogOstream <<
    "The OAH options are:" <<
    endl;

  gIndenter++;

  // JMI ???

  gIndenter--;
}

ostream& operator<< (ostream& os, const S_xml2bmmlManPageOah& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
void initializeXml2bmmlManPageOahHandling (
  S_oahHandler           handler,
  S_oahVisitor theOah2ManPageGenerator)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceOah && ! gGeneralOah->fQuiet) {
    gLogOstream <<
      "Initializing man page options handling" <<
      endl;
  }
#endif

  // protect library against multiple initializations
  static bool pThisMethodHasBeenRun = false;

  if (! pThisMethodHasBeenRun) {
    // create the bsr variables
    // ------------------------------------------------------

    gXml2bmmlManPageOahUserChoices = xml2bmmlManPageOah::create (
      handler,
      theOah2ManPageGenerator);
    assert(gXml2bmmlManPageOahUserChoices != 0);

    gXml2bmmlManPageOah =
      gXml2bmmlManPageOahUserChoices;

    // prepare for measure detailed trace
    // ------------------------------------------------------
  /* JMI
    gXml2bmmlManPageOahWithDetailedTrace =
      gXml2bmmlManPageOah->
        createCloneWithDetailedTrace ();
        */

    pThisMethodHasBeenRun = true;
  }
}


}
