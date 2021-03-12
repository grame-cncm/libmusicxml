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

#include "msr2guidoManPageOah.h"


using namespace std;

namespace MusicXML2
{

//______________________________________________________________________________
S_msr2guidoManPageGenerateAtom msr2guidoManPageGenerateAtom::create (
  string                 shortName,
  string                 longName,
  string                 description,
  S_oahVisitor           theOahVisitor)
{
  msr2guidoManPageGenerateAtom* o = new
    msr2guidoManPageGenerateAtom (
      shortName,
      longName,
      description,
      theOahVisitor);
  assert (o!=0);
  return o;
}

msr2guidoManPageGenerateAtom::msr2guidoManPageGenerateAtom (
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

msr2guidoManPageGenerateAtom::~msr2guidoManPageGenerateAtom ()
{}

void msr2guidoManPageGenerateAtom::applyElement (ostream& os)
{
#ifdef TRACING_IS_ENABLED
  if (getTraceOah ()) {
    gLogStream <<
      "==> option '" << fetchNames () << "' is a msr2guidoManPageGenerateAtom" <<
      endl;
  }
#endif

  generateManPageData (os);
}

void msr2guidoManPageGenerateAtom::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> msr2guidoManPageGenerateAtom::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_msr2guidoManPageGenerateAtom>*
    p =
      dynamic_cast<visitor<S_msr2guidoManPageGenerateAtom>*> (v)) {
        S_msr2guidoManPageGenerateAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching msr2guidoManPageGenerateAtom::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void msr2guidoManPageGenerateAtom::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> msr2guidoManPageGenerateAtom::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_msr2guidoManPageGenerateAtom>*
    p =
      dynamic_cast<visitor<S_msr2guidoManPageGenerateAtom>*> (v)) {
        S_msr2guidoManPageGenerateAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching msr2guidoManPageGenerateAtom::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void msr2guidoManPageGenerateAtom::browseData (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> msr2guidoManPageGenerateAtom::browseData ()" <<
      endl;
  }
#endif
}

void msr2guidoManPageGenerateAtom::print (ostream& os) const
{
  const unsigned int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "msr2guidoManPageGenerateAtom:" <<
    endl;

  ++gIndenter;

  oahElement::printOahElementEssentials (
    os, fieldWidth);

  --gIndenter;
}

void msr2guidoManPageGenerateAtom::generateManPageData (ostream& os) const
{
  // generate the man page from the OAH handler
  fOahVisitor->visitTheHandler ();

  // a help option has been applied // JMI superflous?
  fetchAtomHandlerUpLink ()->
    setOahHandlerFoundAHelpOption (
      "msr2guidoManPageGenerateAtom::generateManPageData");
}

void msr2guidoManPageGenerateAtom::printAtomWithVariableOptionsValues (
  ostream&     os,
  unsigned int valueFieldWidth) const
{
  // nothing to print here
}

ostream& operator<< (ostream& os, const S_msr2guidoManPageGenerateAtom& elt)
{
  elt->print (os);
  return os;
}

//_______________________________________________________________________________
S_msr2guidoManPageOahGroup gGlobalMsr2guidoManPageOahGroup;

S_msr2guidoManPageOahGroup msr2guidoManPageOahGroup::create (
  S_oahVisitor theOah2manPageGenerator)
{
  msr2guidoManPageOahGroup* o = new msr2guidoManPageOahGroup (
    theOah2manPageGenerator);
  assert (o!=0);
  return o;
}

msr2guidoManPageOahGroup::msr2guidoManPageOahGroup (
  S_oahVisitor theOah2manPageGenerator)
  : oahGroup (
    "Man page",
    "hmp", "help-man-page",
R"(These options control the way the man page is generated.)",
    oahElementVisibilityKind::kElementVisibilityWhole)
{
  initializeMsr2guidoManPageOahGroup (
    theOah2manPageGenerator);
}

msr2guidoManPageOahGroup::~msr2guidoManPageOahGroup ()
{}

void msr2guidoManPageOahGroup::initializeManPageGenerateOptions (
  S_oahVisitor
       theOah2manPageGenerator)
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
      msr2guidoManPageGenerateAtom::create (
        "gmp", "generate-man-page",
        regex_replace (
R"(Write man page data for EXECUTABLE to standard output.)",
          regex ("EXECUTABLE"),
          gGlobalOahOahGroup->getHandlerExecutableName ()),
        fOahVisitor));
}

#ifdef TRACING_IS_ENABLED
void msr2guidoManPageOahGroup::initializeManPageTraceOah ()
{
  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "msr2guidoManPage Trace",
        "hmpt", "help-man-page-trace",
R"(  Note: the options in this group imply '-tmp, -trace-man-page'.)",
// JMI        oahElementVisibilityKind::kElementVisibilityHeaderOnly,
      oahElementVisibilityKind::kElementVisibilityWhole,
      this);

  appendSubGroupToGroup (subGroup);

  // JMI ???
}
#endif

void msr2guidoManPageOahGroup::initializeMsr2guidoManPageOahGroup (
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
void msr2guidoManPageOahGroup::enforceGroupQuietness ()
{
// JMI  fDisplayOah = false;
}

//______________________________________________________________________________
void msr2guidoManPageOahGroup::checkGroupOptionsConsistency ()
{
  // JMI
}

//______________________________________________________________________________
void msr2guidoManPageOahGroup::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> msr2guidoManPageOahGroup::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_msr2guidoManPageOahGroup>*
    p =
      dynamic_cast<visitor<S_msr2guidoManPageOahGroup>*> (v)) {
        S_msr2guidoManPageOahGroup elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching msr2guidoManPageOahGroup::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void msr2guidoManPageOahGroup::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> msr2guidoManPageOahGroup::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_msr2guidoManPageOahGroup>*
    p =
      dynamic_cast<visitor<S_msr2guidoManPageOahGroup>*> (v)) {
        S_msr2guidoManPageOahGroup elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching msr2guidoManPageOahGroup::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void msr2guidoManPageOahGroup::browseData (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> msr2guidoManPageOahGroup::browseData ()" <<
      endl;
  }
#endif
}

//______________________________________________________________________________
void msr2guidoManPageOahGroup::printManPageOahValues (unsigned int fieldWidth)
{
  gLogStream <<
    "The OAH options are:" <<
    endl;

  ++gIndenter;

  // JMI ???

  --gIndenter;
}

ostream& operator<< (ostream& os, const S_msr2guidoManPageOahGroup& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_msr2guidoManPageOahGroup createGlobalMsr2guidoManPageOahGroupHandler (
  S_oahVisitor theOah2manPageGenerator)
{
#ifdef TRACING_IS_ENABLED
  if (getTraceOah ()) {
    gLogStream <<
      "Creating global msr2guidoManPage OAH group" <<
      endl;
  }
#endif

  // protect library against multiple initializations
  if (! gGlobalMsr2guidoManPageOahGroup) {
    // create the global options group
    gGlobalMsr2guidoManPageOahGroup =
      msr2guidoManPageOahGroup::create (
        theOah2manPageGenerator);
    assert (gGlobalMsr2guidoManPageOahGroup != 0);
  }

  // return the global OAH group
  return gGlobalMsr2guidoManPageOahGroup;
}


}
