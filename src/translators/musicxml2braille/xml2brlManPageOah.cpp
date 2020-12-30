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

#include "enableTracingIfDesired.h"
#ifdef TRACING_IS_ENABLED
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
  assert (o!=0);
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
      kElementWithoutValue)
{
  fOahVisitor = theOahVisitor;
}

xml2brlManPageGenerateAtom::~xml2brlManPageGenerateAtom ()
{}

void xml2brlManPageGenerateAtom::applyElement (ostream& os)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceOah ()) {
    gLogStream <<
      "==> option '" << fetchNames () << "' is a xml2brlManPageGenerateAtom" <<
      endl;
  }
#endif

  generateManPageData (os);
}

void xml2brlManPageGenerateAtom::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> xml2brlManPageGenerateAtom::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_xml2brlManPageGenerateAtom>*
    p =
      dynamic_cast<visitor<S_xml2brlManPageGenerateAtom>*> (v)) {
        S_xml2brlManPageGenerateAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching xml2brlManPageGenerateAtom::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void xml2brlManPageGenerateAtom::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> xml2brlManPageGenerateAtom::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_xml2brlManPageGenerateAtom>*
    p =
      dynamic_cast<visitor<S_xml2brlManPageGenerateAtom>*> (v)) {
        S_xml2brlManPageGenerateAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching xml2brlManPageGenerateAtom::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void xml2brlManPageGenerateAtom::browseData (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> xml2brlManPageGenerateAtom::browseData ()" <<
      endl;
  }
#endif
}

void xml2brlManPageGenerateAtom::print (ostream& os) const
{
  const unsigned int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "xml2brlManPageGenerateAtom:" <<
    endl;

  gIndenter++;

  oahElement::printOahElementEssentials (
    os, fieldWidth);

  gIndenter--;
}

void xml2brlManPageGenerateAtom::generateManPageData (ostream& os) const
{
  // generate the man page from the OAH handler
  fOahVisitor->visitTheHandler ();

  // a help option has been applied // JMI superflous?
  fetchAtomHandlerUpLink ()->
    setOahHandlerFoundAHelpOption (
      "xml2brlManPageGenerateAtom::generateManPageData");
}

void xml2brlManPageGenerateAtom::printAtomWithValueOptionsValues (
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
S_xml2brlManPageOahGroup gGlobalXml2brlManPageOahGroup;

S_xml2brlManPageOahGroup xml2brlManPageOahGroup::create (
  S_oahVisitor theOah2manPageGenerator)
{
  xml2brlManPageOahGroup* o =
    new xml2brlManPageOahGroup (
      theOah2manPageGenerator);
  assert (o!=0);
  return o;
}

xml2brlManPageOahGroup::xml2brlManPageOahGroup (
  S_oahVisitor theOah2manPageGenerator)
  : oahGroup (
    "Man page",
    "hmp", "help-man-page",
R"(These options control the way the man page is generated.)",
    kElementVisibilityWhole)
{
  initializeXml2brlManPageOahGroup (
    theOah2manPageGenerator);
}

xml2brlManPageOahGroup::~xml2brlManPageOahGroup ()
{}

void xml2brlManPageOahGroup::initializeManPageGenerateOptions (
  S_oahVisitor theOah2manPageGenerator)
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

  // generate man page
  fOah2manPageGenerator = theOah2manPageGenerator;

  subGroup->
    appendAtomToSubGroup (
      xml2brlManPageGenerateAtom::create (
        "gmp", "generate-man-page",
        regex_replace (
R"(Write man page data for EXECUTABLE to standard output.)",
          regex ("EXECUTABLE"),
          gGlobalOahOahGroup->getHandlerExecutableName ()),
        fOah2manPageGenerator));
}

#ifdef TRACING_IS_ENABLED
void xml2brlManPageOahGroup::initializeManPageTraceOah ()
{
  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "xml2brlManPage Trace",
        "hmpt", "help-man-page-trace",
R"(  Note: the options in this group imply '-tmp, -trace-man-page'.)",
// JMI        kElementVisibilityHeaderOnly,
      kElementVisibilityWhole,
      this);

  appendSubGroupToGroup (subGroup);

  // JMI ???
}
#endif

void xml2brlManPageOahGroup::initializeXml2brlManPageOahGroup (
  S_oahVisitor theOah2manPageGenerator)
{
  // display
  // --------------------------------------
  initializeManPageGenerateOptions (
    theOah2manPageGenerator);

#ifdef TRACING_IS_ENABLED
  // trace
  // --------------------------------------
  initializeManPageTraceOah ();
#endif
}

//______________________________________________________________________________
void xml2brlManPageOahGroup::enforceGroupQuietness ()
{
// JMI  fDisplayOah = false;
}

//______________________________________________________________________________
void xml2brlManPageOahGroup::checkGroupOptionsConsistency ()
{
  // JMI
}

//______________________________________________________________________________
void xml2brlManPageOahGroup::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> xml2brlManPageOahGroup::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_xml2brlManPageOahGroup>*
    p =
      dynamic_cast<visitor<S_xml2brlManPageOahGroup>*> (v)) {
        S_xml2brlManPageOahGroup elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching xml2brlManPageOahGroup::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void xml2brlManPageOahGroup::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> xml2brlManPageOahGroup::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_xml2brlManPageOahGroup>*
    p =
      dynamic_cast<visitor<S_xml2brlManPageOahGroup>*> (v)) {
        S_xml2brlManPageOahGroup elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching xml2brlManPageOahGroup::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void xml2brlManPageOahGroup::browseData (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> xml2brlManPageOahGroup::browseData ()" <<
      endl;
  }
#endif
}

//______________________________________________________________________________
void xml2brlManPageOahGroup::printManPageOahValues (unsigned int fieldWidth)
{
  gLogStream <<
    "The OAH options are:" <<
    endl;

  gIndenter++;

  // JMI ???

  gIndenter--;
}

ostream& operator<< (ostream& os, const S_xml2brlManPageOahGroup& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_xml2brlManPageOahGroup createGlobalXml2brlManPageOahHandler (
  S_oahVisitor theOah2manPageGenerator)
{
#ifdef TRACING_IS_ENABLED
#ifdef ENFORCE_TRACE_OAH
  gLogStream <<
    "Creating global xmlblrManPage OAH group" <<
    endl;
#endif
#endif

  // protect library against multiple initializations
  if (! gGlobalXml2brlManPageOahGroup) {
    // create the global options group
    gGlobalXml2brlManPageOahGroup =
      xml2brlManPageOahGroup::create (
        theOah2manPageGenerator);
    assert (gGlobalXml2brlManPageOahGroup != 0);
  }

  // return the global OAH group
  return gGlobalXml2brlManPageOahGroup;
}


}
