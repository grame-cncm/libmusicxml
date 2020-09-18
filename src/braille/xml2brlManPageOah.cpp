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

#include "xml2brlManPageOah.h"


using namespace std;

namespace MusicXML2
{

//______________________________________________________________________________
S_xml2brlManPageGenerateAtom xml2brlManPageGenerateAtom::create (
  string                 shortName,
  string                 longName,
  string                 description,
  S_oahVisitor           theOahVisitor)
{
  xml2brlManPageGenerateAtom* o = new
    xml2brlManPageGenerateAtom (
      shortName,
      longName,
      description,
      theOahVisitor);
  assert(o!=0);
  return o;
}

xml2brlManPageGenerateAtom::xml2brlManPageGenerateAtom (
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

xml2brlManPageGenerateAtom::~xml2brlManPageGenerateAtom ()
{}

S_oahValuedAtom xml2brlManPageGenerateAtom::handleOptionUnderName (
  string   optionName,
  ostream& os)
{
#ifdef TRACE_OAH
  if (gGlobalTraceOahGroup->getTraceOah ()) {
    gLogOstream <<
      "==> option '" << optionName << "' is a xml2brlManPageGenerateAtom" <<
      endl;
  }
#endif

  // handle it at once
  generateManPageData (os);

  // no option value is needed
  return nullptr;
}

void xml2brlManPageGenerateAtom::acceptIn (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gGlobalOahOahGroup->fTraceOahVisitors) {
    gLogOstream <<
      ".\\\" ==> xml2brlManPageGenerateAtom::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_xml2brlManPageGenerateAtom>*
    p =
      dynamic_cast<visitor<S_xml2brlManPageGenerateAtom>*> (v)) {
        S_xml2brlManPageGenerateAtom elem = this;

#ifdef TRACE_OAH
        if (gGlobalOahOahGroup->fTraceOahVisitors) {
          gLogOstream <<
            ".\\\" ==> Launching xml2brlManPageGenerateAtom::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void xml2brlManPageGenerateAtom::acceptOut (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gGlobalOahOahGroup->fTraceOahVisitors) {
    gLogOstream <<
      ".\\\" ==> xml2brlManPageGenerateAtom::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_xml2brlManPageGenerateAtom>*
    p =
      dynamic_cast<visitor<S_xml2brlManPageGenerateAtom>*> (v)) {
        S_xml2brlManPageGenerateAtom elem = this;

#ifdef TRACE_OAH
        if (gGlobalOahOahGroup->fTraceOahVisitors) {
          gLogOstream <<
            ".\\\" ==> Launching xml2brlManPageGenerateAtom::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void xml2brlManPageGenerateAtom::browseData (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gGlobalOahOahGroup->fTraceOahVisitors) {
    gLogOstream <<
      ".\\\" ==> xml2brlManPageGenerateAtom::browseData ()" <<
      endl;
  }
#endif
}

void xml2brlManPageGenerateAtom::print (ostream& os) const
{
  const int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "xml2brlManPageGenerateAtom:" <<
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

void xml2brlManPageGenerateAtom::generateManPageData (ostream& os) const
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

void xml2brlManPageGenerateAtom::printValuedAtomOptionsValues (
  ostream& os,
  int      valueFieldWidth) const
{
  // nothing to print here
}

ostream& operator<< (ostream& os, const S_xml2brlManPageGenerateAtom& elt)
{
  elt->print (os);
  return os;
}

//_______________________________________________________________________________
S_xml2brlManPageOah gGlobalXml2brlManPageOah;
S_xml2brlManPageOah gGlobalXml2brlManPageOahUserChoices;
S_xml2brlManPageOah gGlobalXml2brlManPageOahWithDetailedTrace;

S_xml2brlManPageOah xml2brlManPageOah::create (
  S_oahHandler           handlerUpLink,
  S_oahVisitor theOah2ManPageGenerator)
{
  xml2brlManPageOah* o = new xml2brlManPageOah (
    handlerUpLink,
    theOah2ManPageGenerator);
  assert(o!=0);
  return o;
}

xml2brlManPageOah::xml2brlManPageOah (
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
  initializeXml2brlManPageOah (
    false,
    theOah2ManPageGenerator);
}

xml2brlManPageOah::~xml2brlManPageOah ()
{}

void xml2brlManPageOah::initializeManPageGenerateOptions (
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
  fOah2ManPageGenerator = theOah2ManPageGenerator;

  subGroup->
    appendAtomToSubGroup (
      xml2brlManPageGenerateAtom::create (
        "gmp", "generate-man-page",
        regex_replace (
R"(Write man page data for EXECUTABLE to standard output.)",
          regex ("EXECUTABLE"),
          gGlobalOahOahGroup->fHandlerExecutableName),
        fOah2ManPageGenerator));
}

#ifdef TRACE_OAH
void xml2brlManPageOah::initializeManPageTraceOah (
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

void xml2brlManPageOah::initializeXml2brlManPageOah (
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

S_xml2brlManPageOah xml2brlManPageOah::createCloneWithDetailedTrace ()
{
  S_xml2brlManPageOah
    clone =
      xml2brlManPageOah::create (
        0, // 0 not to have it inserted twice in the option handler
        fOah2ManPageGenerator);

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
void xml2brlManPageOah::enforceGroupQuietness ()
{
// JMI  fDisplayOah = false;
}

//______________________________________________________________________________
void xml2brlManPageOah::checkGroupOptionsConsistency ()
{
  // JMI
}

//______________________________________________________________________________
void xml2brlManPageOah::acceptIn (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gGlobalOahOahGroup->fTraceOahVisitors) {
    gLogOstream <<
      ".\\\" ==> xml2brlManPageOah::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_xml2brlManPageOah>*
    p =
      dynamic_cast<visitor<S_xml2brlManPageOah>*> (v)) {
        S_xml2brlManPageOah elem = this;

#ifdef TRACE_OAH
        if (gGlobalOahOahGroup->fTraceOahVisitors) {
          gLogOstream <<
            ".\\\" ==> Launching xml2brlManPageOah::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void xml2brlManPageOah::acceptOut (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gGlobalOahOahGroup->fTraceOahVisitors) {
    gLogOstream <<
      ".\\\" ==> xml2brlManPageOah::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_xml2brlManPageOah>*
    p =
      dynamic_cast<visitor<S_xml2brlManPageOah>*> (v)) {
        S_xml2brlManPageOah elem = this;

#ifdef TRACE_OAH
        if (gGlobalOahOahGroup->fTraceOahVisitors) {
          gLogOstream <<
            ".\\\" ==> Launching xml2brlManPageOah::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void xml2brlManPageOah::browseData (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gGlobalOahOahGroup->fTraceOahVisitors) {
    gLogOstream <<
      ".\\\" ==> xml2brlManPageOah::browseData ()" <<
      endl;
  }
#endif
}

//______________________________________________________________________________
void xml2brlManPageOah::printManPageOahValues (int fieldWidth)
{
  gLogOstream <<
    "The OAH options are:" <<
    endl;

  gIndenter++;

  // JMI ???

  gIndenter--;
}

ostream& operator<< (ostream& os, const S_xml2brlManPageOah& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
void initializeXml2brlManPageOahHandler (
  S_oahHandler           handler,
  S_oahVisitor theOah2ManPageGenerator)
{
#ifdef TRACE_OAH
  if (gGlobalTraceOahGroup->getTraceOah () && ! gGlobalGeneralOahGroup->fQuiet) {
    gLogOstream <<
      "Initializing man page options handling" <<
      endl;
  }
#endif

  // create the bsr variables
  // ------------------------------------------------------

  gGlobalXml2brlManPageOahUserChoices = xml2brlManPageOah::create (
    handler,
    theOah2ManPageGenerator);
  assert(gGlobalXml2brlManPageOahUserChoices != 0);

  gGlobalXml2brlManPageOah =
    gGlobalXml2brlManPageOahUserChoices;

  // prepare for measure detailed trace
  // ------------------------------------------------------
/* JMI
  gGlobalXml2brlManPageOahWithDetailedTrace =
    gGlobalXml2brlManPageOah->
      createCloneWithDetailedTrace ();
      */
}


}
