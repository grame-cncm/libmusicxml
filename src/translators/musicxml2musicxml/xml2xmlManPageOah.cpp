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

#include "xml2xmlManPageOah.h"


using namespace std;

namespace MusicXML2
{

//______________________________________________________________________________
S_xml2xmlManPageGenerateAtom xml2xmlManPageGenerateAtom::create (
  string                 shortName,
  string                 longName,
  string                 description,
  S_oahVisitor           theOahVisitor)
{
  xml2xmlManPageGenerateAtom* o = new
    xml2xmlManPageGenerateAtom (
      shortName,
      longName,
      description,
      theOahVisitor);
  assert (o!=0);
  return o;
}

xml2xmlManPageGenerateAtom::xml2xmlManPageGenerateAtom (
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

xml2xmlManPageGenerateAtom::~xml2xmlManPageGenerateAtom ()
{}

void xml2xmlManPageGenerateAtom::applyElement (ostream& os)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceOah ()) {
    gLogStream <<
      "==> option '" << fetchNames () << "' is a xml2xmlManPageGenerateAtom" <<
      endl;
  }
#endif

  generateManPageData (os);
}

void xml2xmlManPageGenerateAtom::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> xml2xmlManPageGenerateAtom::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_xml2xmlManPageGenerateAtom>*
    p =
      dynamic_cast<visitor<S_xml2xmlManPageGenerateAtom>*> (v)) {
        S_xml2xmlManPageGenerateAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching xml2xmlManPageGenerateAtom::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void xml2xmlManPageGenerateAtom::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> xml2xmlManPageGenerateAtom::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_xml2xmlManPageGenerateAtom>*
    p =
      dynamic_cast<visitor<S_xml2xmlManPageGenerateAtom>*> (v)) {
        S_xml2xmlManPageGenerateAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching xml2xmlManPageGenerateAtom::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void xml2xmlManPageGenerateAtom::browseData (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> xml2xmlManPageGenerateAtom::browseData ()" <<
      endl;
  }
#endif
}

void xml2xmlManPageGenerateAtom::print (ostream& os) const
{
  const unsigned int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "xml2xmlManPageGenerateAtom:" <<
    endl;

  ++gIndenter;

  oahElement::printOahElementEssentials (
    os, fieldWidth);

  --gIndenter;
}

void xml2xmlManPageGenerateAtom::generateManPageData (ostream& os) const
{
  // generate the man page from the OAH handler
  fOahVisitor->visitTheHandler ();

  // a help option has been applied // JMI superflous?
  fetchAtomHandlerUpLink ()->
    setOahHandlerFoundAHelpOption (
      "xml2xmlManPageGenerateAtom::generateManPageData");
}

void xml2xmlManPageGenerateAtom::printAtomWithValueOptionsValues (
  ostream&     os,
  unsigned int valueFieldWidth) const
{
  // nothing to print here
}

ostream& operator<< (ostream& os, const S_xml2xmlManPageGenerateAtom& elt)
{
  elt->print (os);
  return os;
}

//_______________________________________________________________________________
S_xml2xmlManPageOahGroup gGlobalXml2xmlManPageOahGroup;

S_xml2xmlManPageOahGroup xml2xmlManPageOahGroup::create (
  S_oahVisitor theOah2manPageGenerator)
{
  xml2xmlManPageOahGroup* o = new xml2xmlManPageOahGroup (
    theOah2manPageGenerator);
  assert (o!=0);
  return o;
}

xml2xmlManPageOahGroup::xml2xmlManPageOahGroup (
  S_oahVisitor theOah2manPageGenerator)
  : oahGroup (
    "Man page",
    "hmp", "help-man-page",
R"(These options control the way the man page is generated.)",
    kElementVisibilityWhole)
{
  initializeXml2xmlManPageOahGroup (
    theOah2manPageGenerator);
}

xml2xmlManPageOahGroup::~xml2xmlManPageOahGroup ()
{}

void xml2xmlManPageOahGroup::initializeManPageGenerateOptions (
  S_oahVisitor
       theOah2manPageGenerator)
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

  fDisplayOah = false;

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "doah", "display-oah",
R"(Write the contents of the OAH data to standard error.)",
        "displayOah",
        fDisplayOah));
*/

  // generate man page
  fOahVisitor = theOah2manPageGenerator;

  subGroup->
    appendAtomToSubGroup (
      xml2xmlManPageGenerateAtom::create (
        "gmp", "generate-man-page",
        regex_replace (
R"(Write man page data for EXECUTABLE to standard output.)",
          regex ("EXECUTABLE"),
          gGlobalOahOahGroup->getHandlerExecutableName ()),
        fOahVisitor));
}

#ifdef TRACING_IS_ENABLED
void xml2xmlManPageOahGroup::initializeManPageTraceOah ()
{
  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "xml2xmlManPage Trace",
        "hmpt", "help-man-page-trace",
R"(  Note: the options in this group imply '-tmp, -trace-man-page'.)",
// JMI        kElementVisibilityHeaderOnly,
      kElementVisibilityWhole,
      this);

  appendSubGroupToGroup (subGroup);

  // JMI ???
}
#endif

void xml2xmlManPageOahGroup::initializeXml2xmlManPageOahGroup (
  S_oahVisitor
       theOah2manPageGenerator)
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
void xml2xmlManPageOahGroup::enforceGroupQuietness ()
{
// JMI  fDisplayOah = false;
}

//______________________________________________________________________________
void xml2xmlManPageOahGroup::checkGroupOptionsConsistency ()
{
  // JMI
}

//______________________________________________________________________________
void xml2xmlManPageOahGroup::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> xml2xmlManPageOahGroup::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_xml2xmlManPageOahGroup>*
    p =
      dynamic_cast<visitor<S_xml2xmlManPageOahGroup>*> (v)) {
        S_xml2xmlManPageOahGroup elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching xml2xmlManPageOahGroup::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void xml2xmlManPageOahGroup::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> xml2xmlManPageOahGroup::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_xml2xmlManPageOahGroup>*
    p =
      dynamic_cast<visitor<S_xml2xmlManPageOahGroup>*> (v)) {
        S_xml2xmlManPageOahGroup elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching xml2xmlManPageOahGroup::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void xml2xmlManPageOahGroup::browseData (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> xml2xmlManPageOahGroup::browseData ()" <<
      endl;
  }
#endif
}

//______________________________________________________________________________
void xml2xmlManPageOahGroup::printManPageOahValues (unsigned int fieldWidth)
{
  gLogStream <<
    "The OAH options are:" <<
    endl;

  ++gIndenter;

  // JMI ???

  --gIndenter;
}

ostream& operator<< (ostream& os, const S_xml2xmlManPageOahGroup& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_xml2xmlManPageOahGroup createGlobalXml2xmlManPageOahGroupHandler (
  S_oahVisitor theOah2manPageGenerator)
{
#ifdef TRACING_IS_ENABLED
#ifdef ENFORCE_TRACE_OAH
  gLogStream <<
    "Creating global xml2xmlManPage OAH group" <<
    endl;
#endif
#endif

  // protect library against multiple initializations
  if (! gGlobalXml2xmlManPageOahGroup) {
    // create the global options group
    gGlobalXml2xmlManPageOahGroup =
      xml2xmlManPageOahGroup::create (
        theOah2manPageGenerator);
    assert (gGlobalXml2xmlManPageOahGroup != 0);
  }

  // return the global OAH group
  return gGlobalXml2xmlManPageOahGroup;
}


}
