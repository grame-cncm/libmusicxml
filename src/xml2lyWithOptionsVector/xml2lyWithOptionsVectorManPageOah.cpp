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

#include "xml2lyWithOptionsVectorManPageOah.h"


using namespace std;

namespace MusicXML2
{

//______________________________________________________________________________
S_xml2lyWithOptionsVectorManPageGenerateAtom xml2lyWithOptionsVectorManPageGenerateAtom::create (
  string                 shortName,
  string                 longName,
  string                 description,
  S_oahVisitor           theOahVisitor)
{
  xml2lyWithOptionsVectorManPageGenerateAtom* o = new
    xml2lyWithOptionsVectorManPageGenerateAtom (
      shortName,
      longName,
      description,
      theOahVisitor);
  assert(o!=0);
  return o;
}

xml2lyWithOptionsVectorManPageGenerateAtom::xml2lyWithOptionsVectorManPageGenerateAtom (
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

xml2lyWithOptionsVectorManPageGenerateAtom::~xml2lyWithOptionsVectorManPageGenerateAtom ()
{}

S_oahValuedAtom xml2lyWithOptionsVectorManPageGenerateAtom::handleOptionUnderName (
  string   optionName,
  ostream& os)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceOah) {
    gLogOstream <<
      "==> option '" << optionName << "' is a xml2lyWithOptionsVectorManPageGenerateAtom" <<
      endl;
  }
#endif

  // handle it at once
  generateManPageData (os);

  // no option value is needed
  return nullptr;
}

void xml2lyWithOptionsVectorManPageGenerateAtom::acceptIn (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    gLogOstream <<
      ".\\\" ==> xml2lyWithOptionsVectorManPageGenerateAtom::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_xml2lyWithOptionsVectorManPageGenerateAtom>*
    p =
      dynamic_cast<visitor<S_xml2lyWithOptionsVectorManPageGenerateAtom>*> (v)) {
        S_xml2lyWithOptionsVectorManPageGenerateAtom elem = this;

#ifdef TRACE_OAH
        if (gOahOah->fTraceOahVisitors) {
          gLogOstream <<
            ".\\\" ==> Launching xml2lyWithOptionsVectorManPageGenerateAtom::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void xml2lyWithOptionsVectorManPageGenerateAtom::acceptOut (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    gLogOstream <<
      ".\\\" ==> xml2lyWithOptionsVectorManPageGenerateAtom::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_xml2lyWithOptionsVectorManPageGenerateAtom>*
    p =
      dynamic_cast<visitor<S_xml2lyWithOptionsVectorManPageGenerateAtom>*> (v)) {
        S_xml2lyWithOptionsVectorManPageGenerateAtom elem = this;

#ifdef TRACE_OAH
        if (gOahOah->fTraceOahVisitors) {
          gLogOstream <<
            ".\\\" ==> Launching xml2lyWithOptionsVectorManPageGenerateAtom::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void xml2lyWithOptionsVectorManPageGenerateAtom::browseData (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    gLogOstream <<
      ".\\\" ==> xml2lyWithOptionsVectorManPageGenerateAtom::browseData ()" <<
      endl;
  }
#endif
}

void xml2lyWithOptionsVectorManPageGenerateAtom::print (ostream& os) const
{
  const int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "xml2lyWithOptionsVectorManPageGenerateAtom:" <<
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

void xml2lyWithOptionsVectorManPageGenerateAtom::generateManPageData (ostream& os) const
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
    setOptionsHandlerFoundAHelpOption ();
}

void xml2lyWithOptionsVectorManPageGenerateAtom::printAtomOptionsValues (
  ostream& os,
  int      valueFieldWidth) const
{
  // nothing to print here
}

ostream& operator<< (ostream& os, const S_xml2lyWithOptionsVectorManPageGenerateAtom& elt)
{
  elt->print (os);
  return os;
}

//_______________________________________________________________________________
S_xml2lyWithOptionsVectorManPageOah gxml2lyWithOptionsVectorManPageOah;
S_xml2lyWithOptionsVectorManPageOah gxml2lyWithOptionsVectorManPageOahUserChoices;
S_xml2lyWithOptionsVectorManPageOah gxml2lyWithOptionsVectorManPageOahWithDetailedTrace;

S_xml2lyWithOptionsVectorManPageOah xml2lyWithOptionsVectorManPageOah::create (
  S_oahHandler           handlerUpLink,
  S_oahVisitor theOah2ManPageGenerator)
{
  xml2lyWithOptionsVectorManPageOah* o = new xml2lyWithOptionsVectorManPageOah (
    handlerUpLink,
    theOah2ManPageGenerator);
  assert(o!=0);
  return o;
}

xml2lyWithOptionsVectorManPageOah::xml2lyWithOptionsVectorManPageOah (
  S_oahHandler           handlerUpLink,
  S_oahVisitor theOah2ManPageGenerator)
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
  initializeXml2lyWithOptionsVectorManPageOah (
    false,
    theOah2ManPageGenerator);
}

xml2lyWithOptionsVectorManPageOah::~xml2lyWithOptionsVectorManPageOah ()
{}

void xml2lyWithOptionsVectorManPageOah::initializeManPageGenerateOptions (
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

  // generate man page
  fOahVisitor = theOah2ManPageGenerator;

  subGroup->
    appendAtomToSubGroup (
      xml2lyWithOptionsVectorManPageGenerateAtom::create (
        "gmp", "generate-man-page",
        regex_replace (
R"(Write man page data for EXECUTABLE to standard output.)",
          regex ("EXECUTABLE"),
          gOahOah->fHandlerExecutableName),
        fOahVisitor));
}

#ifdef TRACE_OAH
void xml2lyWithOptionsVectorManPageOah::initializeManPageTraceOah (
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

void xml2lyWithOptionsVectorManPageOah::initializeXml2lyWithOptionsVectorManPageOah (
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

S_xml2lyWithOptionsVectorManPageOah xml2lyWithOptionsVectorManPageOah::createCloneWithDetailedTrace ()
{
  S_xml2lyWithOptionsVectorManPageOah
    clone =
      xml2lyWithOptionsVectorManPageOah::create (
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
void xml2lyWithOptionsVectorManPageOah::enforceQuietness ()
{
// JMI  fDisplayOah = false;
}

//______________________________________________________________________________
void xml2lyWithOptionsVectorManPageOah::checkOptionsConsistency ()
{
  // JMI
}

//______________________________________________________________________________
void xml2lyWithOptionsVectorManPageOah::acceptIn (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    gLogOstream <<
      ".\\\" ==> xml2lyWithOptionsVectorManPageOah::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_xml2lyWithOptionsVectorManPageOah>*
    p =
      dynamic_cast<visitor<S_xml2lyWithOptionsVectorManPageOah>*> (v)) {
        S_xml2lyWithOptionsVectorManPageOah elem = this;

#ifdef TRACE_OAH
        if (gOahOah->fTraceOahVisitors) {
          gLogOstream <<
            ".\\\" ==> Launching xml2lyWithOptionsVectorManPageOah::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void xml2lyWithOptionsVectorManPageOah::acceptOut (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    gLogOstream <<
      ".\\\" ==> xml2lyWithOptionsVectorManPageOah::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_xml2lyWithOptionsVectorManPageOah>*
    p =
      dynamic_cast<visitor<S_xml2lyWithOptionsVectorManPageOah>*> (v)) {
        S_xml2lyWithOptionsVectorManPageOah elem = this;

#ifdef TRACE_OAH
        if (gOahOah->fTraceOahVisitors) {
          gLogOstream <<
            ".\\\" ==> Launching xml2lyWithOptionsVectorManPageOah::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void xml2lyWithOptionsVectorManPageOah::browseData (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    gLogOstream <<
      ".\\\" ==> xml2lyWithOptionsVectorManPageOah::browseData ()" <<
      endl;
  }
#endif
}

//______________________________________________________________________________
void xml2lyWithOptionsVectorManPageOah::printManPageOahValues (int fieldWidth)
{
  gLogOstream <<
    "The OAH options are:" <<
    endl;

  gIndenter++;

  // JMI ???

  gIndenter--;
}

ostream& operator<< (ostream& os, const S_xml2lyWithOptionsVectorManPageOah& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
void initializeXml2lyWithOptionsVectorManPageOahHandling (
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

  // create the bsr variables
  // ------------------------------------------------------

  gxml2lyWithOptionsVectorManPageOahUserChoices = xml2lyWithOptionsVectorManPageOah::create (
    handler,
    theOah2ManPageGenerator);
  assert(gxml2lyWithOptionsVectorManPageOahUserChoices != 0);

  gxml2lyWithOptionsVectorManPageOah =
    gxml2lyWithOptionsVectorManPageOahUserChoices;

  // prepare for measure detailed trace
  // ------------------------------------------------------
/* JMI
  gxml2lyWithOptionsVectorManPageOahWithDetailedTrace =
    gxml2lyWithOptionsVectorManPageOah->
      createCloneWithDetailedTrace ();
      */
}


}
