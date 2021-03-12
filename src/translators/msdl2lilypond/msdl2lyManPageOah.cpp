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

#include "msdl2lyManPageOah.h"


using namespace std;

namespace MusicXML2
{

//______________________________________________________________________________
S_msdl2lyManPageGenerateAtom msdl2lyManPageGenerateAtom::create (
  string                 shortName,
  string                 longName,
  string                 description,
  S_oahVisitor           theOahVisitor)
{
  msdl2lyManPageGenerateAtom* o = new
    msdl2lyManPageGenerateAtom (
      shortName,
      longName,
      description,
      theOahVisitor);
  assert (o!=0);
  return o;
}

msdl2lyManPageGenerateAtom::msdl2lyManPageGenerateAtom (
  string                 shortName,
  string                 longName,
  string                 description,
  S_oahVisitor           theOahVisitor)
  : oahAtom (
      shortName,
      longName,
      description,
      oahElementValueKind::kElementValueWithout)
{
  fOahVisitor = theOahVisitor;
}

msdl2lyManPageGenerateAtom::~msdl2lyManPageGenerateAtom ()
{}

void msdl2lyManPageGenerateAtom::applyElement (ostream& os)
{
#ifdef TRACING_IS_ENABLED
  if (getTraceOah ()) {
    gLogStream <<
      "==> option '" << fetchNames () << "' is a msdl2lyManPageGenerateAtom" <<
      endl;
  }
#endif

  generateManPageData (os);
}

void msdl2lyManPageGenerateAtom::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> msdl2lyManPageGenerateAtom::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_msdl2lyManPageGenerateAtom>*
    p =
      dynamic_cast<visitor<S_msdl2lyManPageGenerateAtom>*> (v)) {
        S_msdl2lyManPageGenerateAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching msdl2lyManPageGenerateAtom::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void msdl2lyManPageGenerateAtom::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> msdl2lyManPageGenerateAtom::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_msdl2lyManPageGenerateAtom>*
    p =
      dynamic_cast<visitor<S_msdl2lyManPageGenerateAtom>*> (v)) {
        S_msdl2lyManPageGenerateAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching msdl2lyManPageGenerateAtom::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void msdl2lyManPageGenerateAtom::browseData (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> msdl2lyManPageGenerateAtom::browseData ()" <<
      endl;
  }
#endif
}

void msdl2lyManPageGenerateAtom::print (ostream& os) const
{
  const unsigned int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "msdl2lyManPageGenerateAtom:" <<
    endl;

  ++gIndenter;

  oahElement::printOahElementEssentials (
    os, fieldWidth);

  --gIndenter;
}

void msdl2lyManPageGenerateAtom::generateManPageData (ostream& os) const
{
  // generate the man page from the OAH handler
  fOahVisitor->visitTheHandler ();

  // a help option has been applied // JMI superflous?
  fetchAtomHandlerUpLink ()->
    setOahHandlerFoundAHelpOption (
      "msdl2lyManPageGenerateAtom::generateManPageData");
}

void msdl2lyManPageGenerateAtom::printAtomWithVariableOptionsValues (
  ostream&     os,
  unsigned int valueFieldWidth) const
{
  // nothing to print here
}

ostream& operator<< (ostream& os, const S_msdl2lyManPageGenerateAtom& elt)
{
  elt->print (os);
  return os;
}

//_______________________________________________________________________________
S_msdl2lyManPageOahGroup gGlobalMsdl2lyManPageOahGroup;

S_msdl2lyManPageOahGroup msdl2lyManPageOahGroup::create (
  S_oahVisitor theOah2manPageGenerator)
{
  msdl2lyManPageOahGroup* o = new msdl2lyManPageOahGroup (
    theOah2manPageGenerator);
  assert (o!=0);
  return o;
}

msdl2lyManPageOahGroup::msdl2lyManPageOahGroup (
  S_oahVisitor theOah2manPageGenerator)
  : oahGroup (
    "Man page",
    "hmp", "help-man-page",
R"(These options control the way the man page is generated.)",
    oahElementVisibilityKind::kElementVisibilityWhole)
{
  initializeMsdl2lyManPageOahGroup (
    theOah2manPageGenerator);
}

msdl2lyManPageOahGroup::~msdl2lyManPageOahGroup ()
{}

void msdl2lyManPageOahGroup::initializeManPageGenerateOptions (
  S_oahVisitor theOah2manPageGenerator)
{
  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Generate",
        "hgmp", "help-generate-man-page",
R"()",
      oahElementVisibilityKind::kElementVisibilityWhole,
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
      msdl2lyManPageGenerateAtom::create (
        "gmp", "generate-man-page",
        regex_replace (
R"(Write man page data for EXECUTABLE to standard output.)",
          regex ("EXECUTABLE"),
          gGlobalOahOahGroup->getHandlerExecutableName ()),
        fOahVisitor));
}

#ifdef TRACING_IS_ENABLED
void msdl2lyManPageOahGroup::initializeManPageTraceOah ()
{
  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "msdl2lyManPage Trace",
        "hmpt", "help-man-page-trace",
R"(  Note: the options in this group imply '-tmp, -trace-man-page'.)",
// JMI        oahElementVisibilityKind::kElementVisibilityHeaderOnly,
      oahElementVisibilityKind::kElementVisibilityWhole,
      this);

  appendSubGroupToGroup (subGroup);

  // JMI ???
}
#endif

void msdl2lyManPageOahGroup::initializeMsdl2lyManPageOahGroup (
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
void msdl2lyManPageOahGroup::enforceGroupQuietness ()
{
// JMI  fDisplayOah = false;
}

//______________________________________________________________________________
void msdl2lyManPageOahGroup::checkGroupOptionsConsistency ()
{
  // JMI
}

//______________________________________________________________________________
void msdl2lyManPageOahGroup::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> msdl2lyManPageOahGroup::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_msdl2lyManPageOahGroup>*
    p =
      dynamic_cast<visitor<S_msdl2lyManPageOahGroup>*> (v)) {
        S_msdl2lyManPageOahGroup elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching msdl2lyManPageOahGroup::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void msdl2lyManPageOahGroup::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> msdl2lyManPageOahGroup::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_msdl2lyManPageOahGroup>*
    p =
      dynamic_cast<visitor<S_msdl2lyManPageOahGroup>*> (v)) {
        S_msdl2lyManPageOahGroup elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching msdl2lyManPageOahGroup::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void msdl2lyManPageOahGroup::browseData (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> msdl2lyManPageOahGroup::browseData ()" <<
      endl;
  }
#endif
}

//______________________________________________________________________________
void msdl2lyManPageOahGroup::printManPageOahValues (unsigned int fieldWidth)
{
  gLogStream <<
    "The OAH options are:" <<
    endl;

  ++gIndenter;

  // JMI ???

  --gIndenter;
}

ostream& operator<< (ostream& os, const S_msdl2lyManPageOahGroup& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_msdl2lyManPageOahGroup createGlobalMsdl2lyManPageOahGroup (
  S_oahVisitor theOah2manPageGenerator)
{
#ifdef TRACING_IS_ENABLED
  if (getTraceOah ()) {
    gLogStream <<
      "Creating global msdl2lyManpage OAH group" <<
      endl;
  }
#endif

  // protect library against multiple initializations
  if (! gGlobalMsdl2lyManPageOahGroup) {
    // create the global options group
    gGlobalMsdl2lyManPageOahGroup =
      msdl2lyManPageOahGroup::create (
        theOah2manPageGenerator);
    assert (gGlobalMsdl2lyManPageOahGroup != 0);
  }

  // return the global OAH group
  return gGlobalMsdl2lyManPageOahGroup;
}


}
