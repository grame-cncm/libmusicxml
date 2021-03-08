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

#include "msdl2xmlManPageOah.h"


using namespace std;

namespace MusicXML2
{

//______________________________________________________________________________
S_msdl2xmlManPageGenerateAtom msdl2xmlManPageGenerateAtom::create (
  string                 shortName,
  string                 longName,
  string                 description,
  S_oahVisitor           theOahVisitor)
{
  msdl2xmlManPageGenerateAtom* o = new
    msdl2xmlManPageGenerateAtom (
      shortName,
      longName,
      description,
      theOahVisitor);
  assert (o!=0);
  return o;
}

msdl2xmlManPageGenerateAtom::msdl2xmlManPageGenerateAtom (
  string                 shortName,
  string                 longName,
  string                 description,
  S_oahVisitor           theOahVisitor)
  : oahAtom (
      shortName,
      longName,
      description,
      kElementValueWithout)
{
  fOahVisitor = theOahVisitor;
}

msdl2xmlManPageGenerateAtom::~msdl2xmlManPageGenerateAtom ()
{}

void msdl2xmlManPageGenerateAtom::applyElement (ostream& os)
{
#ifdef TRACING_IS_ENABLED
  if (getTraceOah ()) {
    gLogStream <<
      "==> option '" << fetchNames () << "' is a msdl2xmlManPageGenerateAtom" <<
      endl;
  }
#endif

  generateManPageData (os);
}

void msdl2xmlManPageGenerateAtom::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> msdl2xmlManPageGenerateAtom::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_msdl2xmlManPageGenerateAtom>*
    p =
      dynamic_cast<visitor<S_msdl2xmlManPageGenerateAtom>*> (v)) {
        S_msdl2xmlManPageGenerateAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching msdl2xmlManPageGenerateAtom::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void msdl2xmlManPageGenerateAtom::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> msdl2xmlManPageGenerateAtom::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_msdl2xmlManPageGenerateAtom>*
    p =
      dynamic_cast<visitor<S_msdl2xmlManPageGenerateAtom>*> (v)) {
        S_msdl2xmlManPageGenerateAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching msdl2xmlManPageGenerateAtom::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void msdl2xmlManPageGenerateAtom::browseData (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> msdl2xmlManPageGenerateAtom::browseData ()" <<
      endl;
  }
#endif
}

void msdl2xmlManPageGenerateAtom::print (ostream& os) const
{
  const unsigned int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "msdl2xmlManPageGenerateAtom:" <<
    endl;

  ++gIndenter;

  oahElement::printOahElementEssentials (
    os, fieldWidth);

  --gIndenter;
}

void msdl2xmlManPageGenerateAtom::generateManPageData (ostream& os) const
{
  // generate the man page from the OAH handler
  fOahVisitor->visitTheHandler ();

  // a help option has been applied // JMI superflous?
  fetchAtomHandlerUpLink ()->
    setOahHandlerFoundAHelpOption (
      "msdl2xmlManPageGenerateAtom::generateManPageData");
}

void msdl2xmlManPageGenerateAtom::printAtomWithVariableOptionsValues (
  ostream&     os,
  unsigned int valueFieldWidth) const
{
  // nothing to print here
}

ostream& operator<< (ostream& os, const S_msdl2xmlManPageGenerateAtom& elt)
{
  elt->print (os);
  return os;
}

//_______________________________________________________________________________
S_msdl2xmlManPageOahGroup gGlobalMsdl2xmlManPageOahGroup;

S_msdl2xmlManPageOahGroup msdl2xmlManPageOahGroup::create (
  S_oahVisitor theOah2manPageGenerator)
{
  msdl2xmlManPageOahGroup* o = new msdl2xmlManPageOahGroup (
    theOah2manPageGenerator);
  assert (o!=0);
  return o;
}

msdl2xmlManPageOahGroup::msdl2xmlManPageOahGroup (
  S_oahVisitor theOah2manPageGenerator)
  : oahGroup (
    "Man page",
    "hmp", "help-man-page",
R"(These options control the way the man page is generated.)",
    kElementVisibilityWhole)
{
  initializeMsdl2xmlManPageOahGroup (
    theOah2manPageGenerator);
}

msdl2xmlManPageOahGroup::~msdl2xmlManPageOahGroup ()
{}

void msdl2xmlManPageOahGroup::initializeManPageGenerateOptions (
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
      msdl2xmlManPageGenerateAtom::create (
        "gmp", "generate-man-page",
        regex_replace (
R"(Write man page data for EXECUTABLE to standard output.)",
          regex ("EXECUTABLE"),
          gGlobalOahOahGroup->getHandlerExecutableName ()),
        fOahVisitor));
}

#ifdef TRACING_IS_ENABLED
void msdl2xmlManPageOahGroup::initializeManPageTraceOah ()
{
  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "msdl2xmlManPage Trace",
        "hmpt", "help-man-page-trace",
R"(  Note: the options in this group imply '-tmp, -trace-man-page'.)",
// JMI        kElementVisibilityHeaderOnly,
      kElementVisibilityWhole,
      this);

  appendSubGroupToGroup (subGroup);

  // JMI ???
}
#endif

void msdl2xmlManPageOahGroup::initializeMsdl2xmlManPageOahGroup (
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
void msdl2xmlManPageOahGroup::enforceGroupQuietness ()
{
// JMI  fDisplayOah = false;
}

//______________________________________________________________________________
void msdl2xmlManPageOahGroup::checkGroupOptionsConsistency ()
{
  // JMI
}

//______________________________________________________________________________
void msdl2xmlManPageOahGroup::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> msdl2xmlManPageOahGroup::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_msdl2xmlManPageOahGroup>*
    p =
      dynamic_cast<visitor<S_msdl2xmlManPageOahGroup>*> (v)) {
        S_msdl2xmlManPageOahGroup elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching msdl2xmlManPageOahGroup::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void msdl2xmlManPageOahGroup::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> msdl2xmlManPageOahGroup::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_msdl2xmlManPageOahGroup>*
    p =
      dynamic_cast<visitor<S_msdl2xmlManPageOahGroup>*> (v)) {
        S_msdl2xmlManPageOahGroup elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching msdl2xmlManPageOahGroup::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void msdl2xmlManPageOahGroup::browseData (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> msdl2xmlManPageOahGroup::browseData ()" <<
      endl;
  }
#endif
}

//______________________________________________________________________________
void msdl2xmlManPageOahGroup::printManPageOahValues (unsigned int fieldWidth)
{
  gLogStream <<
    "The OAH options are:" <<
    endl;

  ++gIndenter;

  // JMI ???

  --gIndenter;
}

ostream& operator<< (ostream& os, const S_msdl2xmlManPageOahGroup& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_msdl2xmlManPageOahGroup createGlobalMsdl2xmlManPageOahGroupHandler (
  S_oahVisitor theOah2manPageGenerator)
{
#ifdef TRACING_IS_ENABLED
  if (getTraceOah ()) {
    gLogStream <<
      "Creating global msdl2xmlManPage OAH group" <<
      endl;
  }
#endif

  // protect library against multiple initializations
  if (! gGlobalMsdl2xmlManPageOahGroup) {
    // create the global options group
    gGlobalMsdl2xmlManPageOahGroup =
      msdl2xmlManPageOahGroup::create (
        theOah2manPageGenerator);
    assert (gGlobalMsdl2xmlManPageOahGroup != 0);
  }

  // return the global OAH group
  return gGlobalMsdl2xmlManPageOahGroup;
}


}
