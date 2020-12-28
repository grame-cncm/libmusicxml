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

#include "exNihiloManPageOah.h"


using namespace std;

namespace MusicXML2
{

//______________________________________________________________________________
S_exNihiloManPageGenerateAtom exNihiloManPageGenerateAtom::create (
  string                 shortName,
  string                 longName,
  string                 description,
  S_oahVisitor           theOahVisitor)
{
  exNihiloManPageGenerateAtom* o = new
    exNihiloManPageGenerateAtom (
      shortName,
      longName,
      description,
      theOahVisitor);
  assert (o!=0);
  return o;
}

exNihiloManPageGenerateAtom::exNihiloManPageGenerateAtom (
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

exNihiloManPageGenerateAtom::~exNihiloManPageGenerateAtom ()
{}

void exNihiloManPageGenerateAtom::applyElement (ostream& os)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceOah ()) {
    gLogStream <<
      "==> option '" << fetchNames () << "' is a exNihiloManPageGenerateAtom" <<
      endl;
  }
#endif

  generateManPageData (os);
}

void exNihiloManPageGenerateAtom::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> exNihiloManPageGenerateAtom::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_exNihiloManPageGenerateAtom>*
    p =
      dynamic_cast<visitor<S_exNihiloManPageGenerateAtom>*> (v)) {
        S_exNihiloManPageGenerateAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching exNihiloManPageGenerateAtom::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void exNihiloManPageGenerateAtom::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> exNihiloManPageGenerateAtom::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_exNihiloManPageGenerateAtom>*
    p =
      dynamic_cast<visitor<S_exNihiloManPageGenerateAtom>*> (v)) {
        S_exNihiloManPageGenerateAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching exNihiloManPageGenerateAtom::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void exNihiloManPageGenerateAtom::browseData (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> exNihiloManPageGenerateAtom::browseData ()" <<
      endl;
  }
#endif
}

void exNihiloManPageGenerateAtom::print (ostream& os) const
{
  const int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "exNihiloManPageGenerateAtom:" <<
    endl;

  gIndenter++;

  oahElement::printOahElementEssentials (
    os, fieldWidth);

  gIndenter--;
}

void exNihiloManPageGenerateAtom::generateManPageData (ostream& os) const
{
  // generate the man page from the OAH handler
  fOahVisitor->visitTheHandler ();

  // a help option has been applied // JMI superflous?
  fetchAtomHandlerUpLink ()->
    setOahHandlerFoundAHelpOption (
      "exNihiloManPageGenerateAtom::generateManPageData");
}

void exNihiloManPageGenerateAtom::printAtomWithValueOptionsValues (
  ostream& os,
  int      valueFieldWidth) const
{
  // nothing to print here
}

ostream& operator<< (ostream& os, const S_exNihiloManPageGenerateAtom& elt)
{
  elt->print (os);
  return os;
}

//_______________________________________________________________________________
S_exNihiloManPageOahGroup gGlobalXml2gmnManPageOahGroup;

S_exNihiloManPageOahGroup exNihiloManPageOahGroup::create (
  S_oahVisitor theOah2manPageGenerator)
{
  exNihiloManPageOahGroup* o = new exNihiloManPageOahGroup (
    theOah2manPageGenerator);
  assert (o!=0);
  return o;
}

exNihiloManPageOahGroup::exNihiloManPageOahGroup (
  S_oahVisitor theOah2manPageGenerator)
  : oahGroup (
    "Man page",
    "hmp", "help-man-page",
R"(These options control the way the man page is generated.)",
    kElementVisibilityWhole)
{
  initializeXml2gmnManPageOahGroup (
    theOah2manPageGenerator);
}

exNihiloManPageOahGroup::~exNihiloManPageOahGroup ()
{}

void exNihiloManPageOahGroup::initializeManPageGenerateOptions (
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
      exNihiloManPageGenerateAtom::create (
        "gmp", "generate-man-page",
        regex_replace (
R"(Write man page data for EXECUTABLE to standard output.)",
          regex ("EXECUTABLE"),
          gGlobalOahOahGroup->getHandlerExecutableName ()),
        fOahVisitor));
}

#ifdef TRACING_IS_ENABLED
void exNihiloManPageOahGroup::initializeManPageTraceOah ()
{
  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "exNihiloManPage Trace",
        "hmpt", "help-man-page-trace",
R"(  Note: the options in this group imply '-tmp, -trace-man-page'.)",
// JMI        kElementVisibilityHeaderOnly,
      kElementVisibilityWhole,
      this);

  appendSubGroupToGroup (subGroup);

  // JMI ???
}
#endif

void exNihiloManPageOahGroup::initializeXml2gmnManPageOahGroup (
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
void exNihiloManPageOahGroup::enforceGroupQuietness ()
{
// JMI  fDisplayOah = false;
}

//______________________________________________________________________________
void exNihiloManPageOahGroup::checkGroupOptionsConsistency ()
{
  // JMI
}

//______________________________________________________________________________
void exNihiloManPageOahGroup::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> exNihiloManPageOahGroup::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_exNihiloManPageOahGroup>*
    p =
      dynamic_cast<visitor<S_exNihiloManPageOahGroup>*> (v)) {
        S_exNihiloManPageOahGroup elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching exNihiloManPageOahGroup::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void exNihiloManPageOahGroup::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> exNihiloManPageOahGroup::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_exNihiloManPageOahGroup>*
    p =
      dynamic_cast<visitor<S_exNihiloManPageOahGroup>*> (v)) {
        S_exNihiloManPageOahGroup elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching exNihiloManPageOahGroup::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void exNihiloManPageOahGroup::browseData (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> exNihiloManPageOahGroup::browseData ()" <<
      endl;
  }
#endif
}

//______________________________________________________________________________
void exNihiloManPageOahGroup::printManPageOahValues (int fieldWidth)
{
  gLogStream <<
    "The OAH options are:" <<
    endl;

  gIndenter++;

  // JMI ???

  gIndenter--;
}

ostream& operator<< (ostream& os, const S_exNihiloManPageOahGroup& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_exNihiloManPageOahGroup createGlobalXml2gmnManPageOahGroupHandler (
  S_oahVisitor theOah2manPageGenerator)
{
#ifdef TRACING_IS_ENABLED
#ifdef ENFORCE_TRACE_OAH
  gLogStream <<
    "Creating global exNihiloManPage OAH group" <<
    endl;
#endif
#endif

  // protect library against multiple initializations
  if (! gGlobalXml2gmnManPageOahGroup) {
    // create the global options group
    gGlobalXml2gmnManPageOahGroup =
      exNihiloManPageOahGroup::create (
        theOah2manPageGenerator);
    assert (gGlobalXml2gmnManPageOahGroup != 0);
  }

  // return the global OAH group
  return gGlobalXml2gmnManPageOahGroup;
}


}
