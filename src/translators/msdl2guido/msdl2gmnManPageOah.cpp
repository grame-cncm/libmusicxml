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

#include "msdl2gmnManPageOah.h"


using namespace std;

namespace MusicXML2
{

//______________________________________________________________________________
S_msdl2gmnManPageGenerateAtom msdl2gmnManPageGenerateAtom::create (
  string                 shortName,
  string                 longName,
  string                 description,
  S_oahVisitor           theOahVisitor)
{
  msdl2gmnManPageGenerateAtom* o = new
    msdl2gmnManPageGenerateAtom (
      shortName,
      longName,
      description,
      theOahVisitor);
  assert (o!=0);
  return o;
}

msdl2gmnManPageGenerateAtom::msdl2gmnManPageGenerateAtom (
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

msdl2gmnManPageGenerateAtom::~msdl2gmnManPageGenerateAtom ()
{}

void msdl2gmnManPageGenerateAtom::applyElement (ostream& os)
{
#ifdef TRACING_IS_ENABLED
  if (getTraceOah ()) {
    gLogStream <<
      "==> option '" << fetchNames () << "' is a msdl2gmnManPageGenerateAtom" <<
      endl;
  }
#endif

  generateManPageData (os);
}

void msdl2gmnManPageGenerateAtom::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> msdl2gmnManPageGenerateAtom::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_msdl2gmnManPageGenerateAtom>*
    p =
      dynamic_cast<visitor<S_msdl2gmnManPageGenerateAtom>*> (v)) {
        S_msdl2gmnManPageGenerateAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching msdl2gmnManPageGenerateAtom::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void msdl2gmnManPageGenerateAtom::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> msdl2gmnManPageGenerateAtom::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_msdl2gmnManPageGenerateAtom>*
    p =
      dynamic_cast<visitor<S_msdl2gmnManPageGenerateAtom>*> (v)) {
        S_msdl2gmnManPageGenerateAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching msdl2gmnManPageGenerateAtom::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void msdl2gmnManPageGenerateAtom::browseData (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> msdl2gmnManPageGenerateAtom::browseData ()" <<
      endl;
  }
#endif
}

void msdl2gmnManPageGenerateAtom::print (ostream& os) const
{
  const unsigned int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "msdl2gmnManPageGenerateAtom:" <<
    endl;

  ++gIndenter;

  oahElement::printOahElementEssentials (
    os, fieldWidth);

  --gIndenter;
}

void msdl2gmnManPageGenerateAtom::generateManPageData (ostream& os) const
{
  // generate the man page from the OAH handler
  fOahVisitor->visitTheHandler ();

  // a help option has been applied // JMI superflous?
  fetchAtomHandlerUpLink ()->
    setOahHandlerFoundAHelpOption (
      "msdl2gmnManPageGenerateAtom::generateManPageData");
}

void msdl2gmnManPageGenerateAtom::printAtomWithVariableOptionsValues (
  ostream&     os,
  unsigned int valueFieldWidth) const
{
  // nothing to print here
}

ostream& operator<< (ostream& os, const S_msdl2gmnManPageGenerateAtom& elt)
{
  elt->print (os);
  return os;
}

//_______________________________________________________________________________
S_msdl2gmnManPageOahGroup gGlobalMsdl2gmnManPageOahGroup;

S_msdl2gmnManPageOahGroup msdl2gmnManPageOahGroup::create (
  S_oahVisitor theOah2manPageGenerator)
{
  msdl2gmnManPageOahGroup* o = new msdl2gmnManPageOahGroup (
    theOah2manPageGenerator);
  assert (o!=0);
  return o;
}

msdl2gmnManPageOahGroup::msdl2gmnManPageOahGroup (
  S_oahVisitor theOah2manPageGenerator)
  : oahGroup (
    "Man page",
    "hmp", "help-man-page",
R"(These options control the way the man page is generated.)",
    kElementVisibilityWhole)
{
  initializeMsdl2gmnManPageOahGroup (
    theOah2manPageGenerator);
}

msdl2gmnManPageOahGroup::~msdl2gmnManPageOahGroup ()
{}

void msdl2gmnManPageOahGroup::initializeManPageGenerateOptions (
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
      msdl2gmnManPageGenerateAtom::create (
        "gmp", "generate-man-page",
        regex_replace (
R"(Write man page data for EXECUTABLE to standard output.)",
          regex ("EXECUTABLE"),
          gGlobalOahOahGroup->getHandlerExecutableName ()),
        fOahVisitor));
}

#ifdef TRACING_IS_ENABLED
void msdl2gmnManPageOahGroup::initializeManPageTraceOah ()
{
  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "msdl2gmnManPage Trace",
        "hmpt", "help-man-page-trace",
R"(  Note: the options in this group imply '-tmp, -trace-man-page'.)",
// JMI        kElementVisibilityHeaderOnly,
      kElementVisibilityWhole,
      this);

  appendSubGroupToGroup (subGroup);

  // JMI ???
}
#endif

void msdl2gmnManPageOahGroup::initializeMsdl2gmnManPageOahGroup (
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
void msdl2gmnManPageOahGroup::enforceGroupQuietness ()
{
// JMI  fDisplayOah = false;
}

//______________________________________________________________________________
void msdl2gmnManPageOahGroup::checkGroupOptionsConsistency ()
{
  // JMI
}

//______________________________________________________________________________
void msdl2gmnManPageOahGroup::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> msdl2gmnManPageOahGroup::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_msdl2gmnManPageOahGroup>*
    p =
      dynamic_cast<visitor<S_msdl2gmnManPageOahGroup>*> (v)) {
        S_msdl2gmnManPageOahGroup elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching msdl2gmnManPageOahGroup::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void msdl2gmnManPageOahGroup::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> msdl2gmnManPageOahGroup::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_msdl2gmnManPageOahGroup>*
    p =
      dynamic_cast<visitor<S_msdl2gmnManPageOahGroup>*> (v)) {
        S_msdl2gmnManPageOahGroup elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching msdl2gmnManPageOahGroup::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void msdl2gmnManPageOahGroup::browseData (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> msdl2gmnManPageOahGroup::browseData ()" <<
      endl;
  }
#endif
}

//______________________________________________________________________________
void msdl2gmnManPageOahGroup::printManPageOahValues (unsigned int fieldWidth)
{
  gLogStream <<
    "The OAH options are:" <<
    endl;

  ++gIndenter;

  // JMI ???

  --gIndenter;
}

ostream& operator<< (ostream& os, const S_msdl2gmnManPageOahGroup& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_msdl2gmnManPageOahGroup createGlobalMsdl2gmnManPageOahGroupHandler (
  S_oahVisitor theOah2manPageGenerator)
{
#ifdef TRACING_IS_ENABLED
  if (getTraceOah ()) {
    gLogStream <<
      "Creating global msdl2gmnManPage OAH group" <<
      endl;
  }
#endif

  // protect library against multiple initializations
  if (! gGlobalMsdl2gmnManPageOahGroup) {
    // create the global options group
    gGlobalMsdl2gmnManPageOahGroup =
      msdl2gmnManPageOahGroup::create (
        theOah2manPageGenerator);
    assert (gGlobalMsdl2gmnManPageOahGroup != 0);
  }

  // return the global OAH group
  return gGlobalMsdl2gmnManPageOahGroup;
}


}
