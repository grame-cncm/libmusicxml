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

#include "msr2brailleManPageOah.h"


using namespace std;

namespace MusicXML2
{

//______________________________________________________________________________
S_msr2brailleManPageGenerateAtom msr2brailleManPageGenerateAtom::create (
  string                 shortName,
  string                 longName,
  string                 description,
  S_oahVisitor           theOahVisitor)
{
  msr2brailleManPageGenerateAtom* o = new
    msr2brailleManPageGenerateAtom (
      shortName,
      longName,
      description,
      theOahVisitor);
  assert (o!=0);
  return o;
}

msr2brailleManPageGenerateAtom::msr2brailleManPageGenerateAtom (
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

msr2brailleManPageGenerateAtom::~msr2brailleManPageGenerateAtom ()
{}

void msr2brailleManPageGenerateAtom::applyElement (ostream& os)
{
#ifdef TRACING_IS_ENABLED
  if (getTraceOah ()) {
    gLogStream <<
      "==> option '" << fetchNames () << "' is a msr2brailleManPageGenerateAtom" <<
      endl;
  }
#endif

  generateManPageData (os);
}

void msr2brailleManPageGenerateAtom::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> msr2brailleManPageGenerateAtom::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_msr2brailleManPageGenerateAtom>*
    p =
      dynamic_cast<visitor<S_msr2brailleManPageGenerateAtom>*> (v)) {
        S_msr2brailleManPageGenerateAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching msr2brailleManPageGenerateAtom::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void msr2brailleManPageGenerateAtom::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> msr2brailleManPageGenerateAtom::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_msr2brailleManPageGenerateAtom>*
    p =
      dynamic_cast<visitor<S_msr2brailleManPageGenerateAtom>*> (v)) {
        S_msr2brailleManPageGenerateAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching msr2brailleManPageGenerateAtom::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void msr2brailleManPageGenerateAtom::browseData (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> msr2brailleManPageGenerateAtom::browseData ()" <<
      endl;
  }
#endif
}

void msr2brailleManPageGenerateAtom::print (ostream& os) const
{
  const unsigned int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "msr2brailleManPageGenerateAtom:" <<
    endl;

  ++gIndenter;

  oahElement::printOahElementEssentials (
    os, fieldWidth);

  --gIndenter;
}

void msr2brailleManPageGenerateAtom::generateManPageData (ostream& os) const
{
  // generate the man page from the OAH handler
  fOahVisitor->visitTheHandler ();

  // a help option has been applied // JMI superflous?
  fetchAtomHandlerUpLink ()->
    setOahHandlerFoundAHelpOption (
      "msr2brailleManPageGenerateAtom::generateManPageData");
}

void msr2brailleManPageGenerateAtom::printAtomWithVariableOptionsValues (
  ostream&     os,
  unsigned int valueFieldWidth) const
{
  // nothing to print here
}

ostream& operator<< (ostream& os, const S_msr2brailleManPageGenerateAtom& elt)
{
  elt->print (os);
  return os;
}

//_______________________________________________________________________________
S_msr2brailleManPageOahGroup gGlobalXml2brailleManPageOahGroup;

S_msr2brailleManPageOahGroup msr2brailleManPageOahGroup::create (
  S_oahVisitor theOah2manPageGenerator)
{
  msr2brailleManPageOahGroup* o =
    new msr2brailleManPageOahGroup (
      theOah2manPageGenerator);
  assert (o!=0);
  return o;
}

msr2brailleManPageOahGroup::msr2brailleManPageOahGroup (
  S_oahVisitor theOah2manPageGenerator)
  : oahGroup (
    "Man page",
    "hmp", "help-man-page",
R"(These options control the way the man page is generated.)",
    oahElementVisibilityKind::kElementVisibilityWhole)
{
  initializeXml2brailleManPageOahGroup (
    theOah2manPageGenerator);
}

msr2brailleManPageOahGroup::~msr2brailleManPageOahGroup ()
{}

void msr2brailleManPageOahGroup::initializeManPageGenerateOptions (
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

  // generate man page
  fOah2manPageGenerator = theOah2manPageGenerator;

  subGroup->
    appendAtomToSubGroup (
      msr2brailleManPageGenerateAtom::create (
        "gmp", "generate-man-page",
        regex_replace (
R"(Write man page data for EXECUTABLE to standard output.)",
          regex ("EXECUTABLE"),
          gGlobalOahOahGroup->getHandlerExecutableName ()),
        fOah2manPageGenerator));
}

#ifdef TRACING_IS_ENABLED
void msr2brailleManPageOahGroup::initializeManPageTraceOah ()
{
  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "msr2brailleManPage Trace",
        "hmpt", "help-man-page-trace",
R"(  Note: the options in this group imply '-tmp, -trace-man-page'.)",
// JMI        oahElementVisibilityKind::kElementVisibilityHeaderOnly,
      oahElementVisibilityKind::kElementVisibilityWhole,
      this);

  appendSubGroupToGroup (subGroup);

  // JMI ???
}
#endif

void msr2brailleManPageOahGroup::initializeXml2brailleManPageOahGroup (
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
void msr2brailleManPageOahGroup::enforceGroupQuietness ()
{
// JMI  fDisplayOah = false;
}

//______________________________________________________________________________
void msr2brailleManPageOahGroup::checkGroupOptionsConsistency ()
{
  // JMI
}

//______________________________________________________________________________
void msr2brailleManPageOahGroup::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> msr2brailleManPageOahGroup::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_msr2brailleManPageOahGroup>*
    p =
      dynamic_cast<visitor<S_msr2brailleManPageOahGroup>*> (v)) {
        S_msr2brailleManPageOahGroup elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching msr2brailleManPageOahGroup::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void msr2brailleManPageOahGroup::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> msr2brailleManPageOahGroup::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_msr2brailleManPageOahGroup>*
    p =
      dynamic_cast<visitor<S_msr2brailleManPageOahGroup>*> (v)) {
        S_msr2brailleManPageOahGroup elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching msr2brailleManPageOahGroup::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void msr2brailleManPageOahGroup::browseData (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> msr2brailleManPageOahGroup::browseData ()" <<
      endl;
  }
#endif
}

//______________________________________________________________________________
void msr2brailleManPageOahGroup::printManPageOahValues (unsigned int fieldWidth)
{
  gLogStream <<
    "The OAH options are:" <<
    endl;

  ++gIndenter;

  // JMI ???

  --gIndenter;
}

ostream& operator<< (ostream& os, const S_msr2brailleManPageOahGroup& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_msr2brailleManPageOahGroup createGlobalXml2brailleManPageOahHandler (
  S_oahVisitor theOah2manPageGenerator)
{
#ifdef TRACING_IS_ENABLED
  if (getTraceOah ()) {
    gLogStream <<
      "Creating global xml2blrManPage OAH group" <<
      endl;
  }
#endif

  // protect library against multiple initializations
  if (! gGlobalXml2brailleManPageOahGroup) {
    // create the global options group
    gGlobalXml2brailleManPageOahGroup =
      msr2brailleManPageOahGroup::create (
        theOah2manPageGenerator);
    assert (gGlobalXml2brailleManPageOahGroup != 0);
  }

  // return the global OAH group
  return gGlobalXml2brailleManPageOahGroup;
}


}
