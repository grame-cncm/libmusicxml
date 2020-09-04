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

#include "xml2meiManPageOah.h"


using namespace std;

namespace MusicXML2
{

//______________________________________________________________________________
S_xml2meiManPageGenerateAtom xml2meiManPageGenerateAtom::create (
  string                 shortName,
  string                 longName,
  string                 description,
  S_oahVisitor           theOahVisitor)
{
  xml2meiManPageGenerateAtom* o = new
    xml2meiManPageGenerateAtom (
      shortName,
      longName,
      description,
      theOahVisitor);
  assert(o!=0);
  return o;
}

xml2meiManPageGenerateAtom::xml2meiManPageGenerateAtom (
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

xml2meiManPageGenerateAtom::~xml2meiManPageGenerateAtom ()
{}

S_oahValuedAtom xml2meiManPageGenerateAtom::handleOptionUnderName (
  string   optionName,
  ostream& os)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceOah) {
    gLogOstream <<
      "==> option '" << optionName << "' is a xml2meiManPageGenerateAtom" <<
      endl;
  }
#endif

  // handle it at once
  generateManPageData (os);

  // no option value is needed
  return nullptr;
}

void xml2meiManPageGenerateAtom::acceptIn (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    gLogOstream <<
      ".\\\" ==> xml2meiManPageGenerateAtom::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_xml2meiManPageGenerateAtom>*
    p =
      dynamic_cast<visitor<S_xml2meiManPageGenerateAtom>*> (v)) {
        S_xml2meiManPageGenerateAtom elem = this;

#ifdef TRACE_OAH
        if (gOahOah->fTraceOahVisitors) {
          gLogOstream <<
            ".\\\" ==> Launching xml2meiManPageGenerateAtom::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void xml2meiManPageGenerateAtom::acceptOut (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    gLogOstream <<
      ".\\\" ==> xml2meiManPageGenerateAtom::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_xml2meiManPageGenerateAtom>*
    p =
      dynamic_cast<visitor<S_xml2meiManPageGenerateAtom>*> (v)) {
        S_xml2meiManPageGenerateAtom elem = this;

#ifdef TRACE_OAH
        if (gOahOah->fTraceOahVisitors) {
          gLogOstream <<
            ".\\\" ==> Launching xml2meiManPageGenerateAtom::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void xml2meiManPageGenerateAtom::browseData (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    gLogOstream <<
      ".\\\" ==> xml2meiManPageGenerateAtom::browseData ()" <<
      endl;
  }
#endif
}

void xml2meiManPageGenerateAtom::print (ostream& os) const
{
  const int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "xml2meiManPageGenerateAtom:" <<
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

void xml2meiManPageGenerateAtom::generateManPageData (ostream& os) const
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

void xml2meiManPageGenerateAtom::printAtomOptionsValues (
  ostream& os,
  int      valueFieldWidth) const
{
  // nothing to print here
}

ostream& operator<< (ostream& os, const S_xml2meiManPageGenerateAtom& elt)
{
  elt->print (os);
  return os;
}

//_______________________________________________________________________________
S_xml2meiManPageOah gXml2meiManPageOah;
S_xml2meiManPageOah gXml2meiManPageOahUserChoices;
S_xml2meiManPageOah gXml2meiManPageOahWithDetailedTrace;

S_xml2meiManPageOah xml2meiManPageOah::create (
  S_oahHandler           handlerUpLink,
  S_oahVisitor theOah2ManPageGenerator)
{
  xml2meiManPageOah* o = new xml2meiManPageOah (
    handlerUpLink,
    theOah2ManPageGenerator);
  assert(o!=0);
  return o;
}

xml2meiManPageOah::xml2meiManPageOah (
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
  initializeXml2meiManPageOah (
    false,
    theOah2ManPageGenerator);
}

xml2meiManPageOah::~xml2meiManPageOah ()
{}

void xml2meiManPageOah::initializeManPageGenerateOptions (
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
      xml2meiManPageGenerateAtom::create (
        "gmp", "generate-man-page",
        regex_replace (
R"(Write man page data for EXECUTABLE to standard output.)",
          regex ("EXECUTABLE"),
          gOahOah->fHandlerExecutableName),
        fOahVisitor));
}

#ifdef TRACE_OAH
void xml2meiManPageOah::initializeManPageTraceOah (
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

void xml2meiManPageOah::initializeXml2meiManPageOah (
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

S_xml2meiManPageOah xml2meiManPageOah::createCloneWithDetailedTrace ()
{
  S_xml2meiManPageOah
    clone =
      xml2meiManPageOah::create (
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
void xml2meiManPageOah::enforceQuietness ()
{
// JMI  fDisplayOah = false;
}

//______________________________________________________________________________
void xml2meiManPageOah::checkOptionsConsistency ()
{
  // JMI
}

//______________________________________________________________________________
void xml2meiManPageOah::acceptIn (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    gLogOstream <<
      ".\\\" ==> xml2meiManPageOah::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_xml2meiManPageOah>*
    p =
      dynamic_cast<visitor<S_xml2meiManPageOah>*> (v)) {
        S_xml2meiManPageOah elem = this;

#ifdef TRACE_OAH
        if (gOahOah->fTraceOahVisitors) {
          gLogOstream <<
            ".\\\" ==> Launching xml2meiManPageOah::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void xml2meiManPageOah::acceptOut (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    gLogOstream <<
      ".\\\" ==> xml2meiManPageOah::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_xml2meiManPageOah>*
    p =
      dynamic_cast<visitor<S_xml2meiManPageOah>*> (v)) {
        S_xml2meiManPageOah elem = this;

#ifdef TRACE_OAH
        if (gOahOah->fTraceOahVisitors) {
          gLogOstream <<
            ".\\\" ==> Launching xml2meiManPageOah::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void xml2meiManPageOah::browseData (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    gLogOstream <<
      ".\\\" ==> xml2meiManPageOah::browseData ()" <<
      endl;
  }
#endif
}

//______________________________________________________________________________
void xml2meiManPageOah::printManPageOahValues (int fieldWidth)
{
  gLogOstream <<
    "The OAH options are:" <<
    endl;

  gIndenter++;

  // JMI ???

  gIndenter--;
}

ostream& operator<< (ostream& os, const S_xml2meiManPageOah& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
void initializeXml2meiManPageOahHandling (
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

    gXml2meiManPageOahUserChoices = xml2meiManPageOah::create (
      handler,
      theOah2ManPageGenerator);
    assert(gXml2meiManPageOahUserChoices != 0);

    gXml2meiManPageOah =
      gXml2meiManPageOahUserChoices;

    // prepare for measure detailed trace
    // ------------------------------------------------------
  /* JMI
    gXml2meiManPageOahWithDetailedTrace =
      gXml2meiManPageOah->
        createCloneWithDetailedTrace ();
        */

    pThisMethodHasBeenRun = true;
  }
}


}
