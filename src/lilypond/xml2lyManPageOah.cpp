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

#include "xml2lyManPageOah.h"


using namespace std;

namespace MusicXML2
{

//______________________________________________________________________________
S_xml2lyManPageGenerateAtom xml2lyManPageGenerateAtom::create (
  string                 shortName,
  string                 longName,
  string                 description,
  S_oahVisitor           theOahVisitor)
{
  xml2lyManPageGenerateAtom* o = new
    xml2lyManPageGenerateAtom (
      shortName,
      longName,
      description,
      theOahVisitor);
  assert(o!=0);
  return o;
}

xml2lyManPageGenerateAtom::xml2lyManPageGenerateAtom (
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

xml2lyManPageGenerateAtom::~xml2lyManPageGenerateAtom ()
{}

S_oahValuedAtom xml2lyManPageGenerateAtom::handleOptionUnderName (
  string   optionName,
  ostream& os)
{
#ifdef TRACE_OAH
  if (gGlobalTraceOah->fTraceOah) {
    gLogOstream <<
      "==> option '" << optionName << "' is a xml2lyManPageGenerateAtom" <<
      endl;
  }
#endif

  // handle it at once
  generateManPageData (os);

  // no option value is needed
  return nullptr;
}

void xml2lyManPageGenerateAtom::acceptIn (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gGlobalOahOah->fTraceOahVisitors) {
    gLogOstream <<
      ".\\\" ==> xml2lyManPageGenerateAtom::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_xml2lyManPageGenerateAtom>*
    p =
      dynamic_cast<visitor<S_xml2lyManPageGenerateAtom>*> (v)) {
        S_xml2lyManPageGenerateAtom elem = this;

#ifdef TRACE_OAH
        if (gGlobalOahOah->fTraceOahVisitors) {
          gLogOstream <<
            ".\\\" ==> Launching xml2lyManPageGenerateAtom::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void xml2lyManPageGenerateAtom::acceptOut (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gGlobalOahOah->fTraceOahVisitors) {
    gLogOstream <<
      ".\\\" ==> xml2lyManPageGenerateAtom::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_xml2lyManPageGenerateAtom>*
    p =
      dynamic_cast<visitor<S_xml2lyManPageGenerateAtom>*> (v)) {
        S_xml2lyManPageGenerateAtom elem = this;

#ifdef TRACE_OAH
        if (gGlobalOahOah->fTraceOahVisitors) {
          gLogOstream <<
            ".\\\" ==> Launching xml2lyManPageGenerateAtom::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void xml2lyManPageGenerateAtom::browseData (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gGlobalOahOah->fTraceOahVisitors) {
    gLogOstream <<
      ".\\\" ==> xml2lyManPageGenerateAtom::browseData ()" <<
      endl;
  }
#endif
}

void xml2lyManPageGenerateAtom::print (ostream& os) const
{
  const int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "xml2lyManPageGenerateAtom:" <<
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

void xml2lyManPageGenerateAtom::generateManPageData (ostream& os) const
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

void xml2lyManPageGenerateAtom::printAtomOptionsValues (
  ostream& os,
  int      valueFieldWidth) const
{
  // nothing to print here
}

ostream& operator<< (ostream& os, const S_xml2lyManPageGenerateAtom& elt)
{
  elt->print (os);
  return os;
}

//_______________________________________________________________________________
S_xml2lyManPageOah gGlobalXml2lyManPageOah;
S_xml2lyManPageOah gGlobalXml2lyManPageOahUserChoices;
S_xml2lyManPageOah gGlobalXml2lyManPageOahWithDetailedTrace;

S_xml2lyManPageOah xml2lyManPageOah::create (
  S_oahHandler           handlerUpLink,
  S_oahVisitor theOah2ManPageGenerator)
{
  xml2lyManPageOah* o = new xml2lyManPageOah (
    handlerUpLink,
    theOah2ManPageGenerator);
  assert(o!=0);
  return o;
}

xml2lyManPageOah::xml2lyManPageOah (
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
  initializeXml2lyManPageOah (
    false,
    theOah2ManPageGenerator);
}

xml2lyManPageOah::~xml2lyManPageOah ()
{}

void xml2lyManPageOah::initializeManPageGenerateOptions (
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
      xml2lyManPageGenerateAtom::create (
        "gmp", "generate-man-page",
        regex_replace (
R"(Write man page data for EXECUTABLE to standard output.)",
          regex ("EXECUTABLE"),
          gGlobalOahOah->fHandlerExecutableName),
        fOahVisitor));
}

#ifdef TRACE_OAH
void xml2lyManPageOah::initializeManPageTraceOah (
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

void xml2lyManPageOah::initializeXml2lyManPageOah (
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

S_xml2lyManPageOah xml2lyManPageOah::createCloneWithDetailedTrace ()
{
  S_xml2lyManPageOah
    clone =
      xml2lyManPageOah::create (
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
void xml2lyManPageOah::enforceQuietness ()
{
// JMI  fDisplayOah = false;
}

//______________________________________________________________________________
void xml2lyManPageOah::checkOptionsConsistency ()
{
  // JMI
}

//______________________________________________________________________________
void xml2lyManPageOah::acceptIn (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gGlobalOahOah->fTraceOahVisitors) {
    gLogOstream <<
      ".\\\" ==> xml2lyManPageOah::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_xml2lyManPageOah>*
    p =
      dynamic_cast<visitor<S_xml2lyManPageOah>*> (v)) {
        S_xml2lyManPageOah elem = this;

#ifdef TRACE_OAH
        if (gGlobalOahOah->fTraceOahVisitors) {
          gLogOstream <<
            ".\\\" ==> Launching xml2lyManPageOah::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void xml2lyManPageOah::acceptOut (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gGlobalOahOah->fTraceOahVisitors) {
    gLogOstream <<
      ".\\\" ==> xml2lyManPageOah::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_xml2lyManPageOah>*
    p =
      dynamic_cast<visitor<S_xml2lyManPageOah>*> (v)) {
        S_xml2lyManPageOah elem = this;

#ifdef TRACE_OAH
        if (gGlobalOahOah->fTraceOahVisitors) {
          gLogOstream <<
            ".\\\" ==> Launching xml2lyManPageOah::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void xml2lyManPageOah::browseData (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gGlobalOahOah->fTraceOahVisitors) {
    gLogOstream <<
      ".\\\" ==> xml2lyManPageOah::browseData ()" <<
      endl;
  }
#endif
}

//______________________________________________________________________________
void xml2lyManPageOah::printManPageOahValues (int fieldWidth)
{
  gLogOstream <<
    "The OAH options are:" <<
    endl;

  gIndenter++;

  // JMI ???

  gIndenter--;
}

ostream& operator<< (ostream& os, const S_xml2lyManPageOah& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
void initializeXml2lyManPageOahHandler (
  S_oahHandler           handler,
  S_oahVisitor theOah2ManPageGenerator)
{
#ifdef TRACE_OAH
  if (gGlobalTraceOah->fTraceOah && ! gGlobalGeneralOah->fQuiet) {
    gLogOstream <<
      "Initializing man page options handling" <<
      endl;
  }
#endif

  // create the bsr variables
  // ------------------------------------------------------

  gGlobalXml2lyManPageOahUserChoices = xml2lyManPageOah::create (
    handler,
    theOah2ManPageGenerator);
  assert(gGlobalXml2lyManPageOahUserChoices != 0);

  gGlobalXml2lyManPageOah =
    gGlobalXml2lyManPageOahUserChoices;

  // prepare for measure detailed trace
  // ------------------------------------------------------
/* JMI
  gGlobalXml2lyManPageOahWithDetailedTrace =
    gGlobalXml2lyManPageOah->
      createCloneWithDetailedTrace ();
      */
}


}
