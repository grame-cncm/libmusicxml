/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#include <iomanip>      // setw, setprecision, ...

#include "version.h"
#include "utilities.h"

#include "enableTracingIfDesired.h"
#ifdef TRACING_IS_ENABLED
  #include "traceOah.h"
#endif

#include "messagesHandling.h"

#include "oahOah.h"
#include "generalOah.h"
#include "msr2mxmlTreeOah.h"

#include "oahAtomsCollection.h"


using namespace std;

namespace MusicXML2
{

//______________________________________________________________________________
S_msr2mxmlTreeOahGroup gGlobalMsr2mxmlTreeOahGroup;

S_msr2mxmlTreeOahGroup msr2mxmlTreeOahGroup::create ()
{
  msr2mxmlTreeOahGroup* o = new msr2mxmlTreeOahGroup ();
  assert (o!=0);
  return o;
}

msr2mxmlTreeOahGroup::msr2mxmlTreeOahGroup ()
  : oahGroup (
    "Msr2mxmlTree",
    "hmsr2mxmlt", "help-msr-to-mxmlTree",
R"(These options control the way MSR is translated to xmlelement tree.)",
    kElementVisibilityWhole)
{
  initializeMsr2mxmlTreeOahGroup ();
}

msr2mxmlTreeOahGroup::~msr2mxmlTreeOahGroup ()
{}

void msr2mxmlTreeOahGroup::initializeCodeGenerationOptions ()
{
  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Code generation",
        "hmxmlcg", "help-musicxml-code-generation",
R"()",
      kElementVisibilityWhole,
      this);

  appendSubGroupToGroup (subGroup);

  // infos
  // --------------------------------------

  fXml2xmlInfos = false;

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "x2xi", "xml2xml-infos",
R"(Generate initial comments showing the compilation date.)",
        "xml2xmlInfos",
        fXml2xmlInfos));

  // comments
  // --------------------------------------

  fMusicXMLComments = false;

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "xmlcom", "musicxml-comments",
R"(Generate comments showing the structure of the score.)",
        "MusicXMLComments",
        fMusicXMLComments));
}

void msr2mxmlTreeOahGroup::initializeMsr2mxmlTreeOahGroup ()
{
#ifdef TRACING_IS_ENABLED
  // trace
  // --------------------------------------
// JMI  initializeMsr2mxmlTreeTraceOah ();
#endif

  // code generation
  // --------------------------------------
  initializeCodeGenerationOptions ();
}

//______________________________________________________________________________
void msr2mxmlTreeOahGroup::enforceGroupQuietness ()
{}

//______________________________________________________________________________
void msr2mxmlTreeOahGroup::checkGroupOptionsConsistency ()
{}

//______________________________________________________________________________
void msr2mxmlTreeOahGroup::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> msr2mxmlTreeOahGroup::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_msr2mxmlTreeOahGroup>*
    p =
      dynamic_cast<visitor<S_msr2mxmlTreeOahGroup>*> (v)) {
        S_msr2mxmlTreeOahGroup elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching msr2mxmlTreeOahGroup::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void msr2mxmlTreeOahGroup::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> msr2mxmlTreeOahGroup::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_msr2mxmlTreeOahGroup>*
    p =
      dynamic_cast<visitor<S_msr2mxmlTreeOahGroup>*> (v)) {
        S_msr2mxmlTreeOahGroup elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching msr2mxmlTreeOahGroup::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void msr2mxmlTreeOahGroup::browseData (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> msr2mxmlTreeOahGroup::browseData ()" <<
      endl;
  }
#endif
}

//______________________________________________________________________________
void msr2mxmlTreeOahGroup::printMsr2mxmlTreeOahValues (unsigned int fieldWidth)
{
  gLogStream <<
    "The MusicXML options are:" <<
    endl;

  ++gIndenter;

  gLogStream << left <<
    setw (fieldWidth) << "xml2xmlInfos" << " : " <<
      booleanAsString (fXml2xmlInfos) <<
      endl <<

    setw (fieldWidth) << "musicXMLComments" << " : " <<
      booleanAsString (fMusicXMLComments) <<
      endl;

  --gIndenter;
}

ostream& operator<< (ostream& os, const S_msr2mxmlTreeOahGroup& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_msr2mxmlTreeOahGroup createGlobalMsr2mxmlTreeOahGroup ()
{
#ifdef TRACING_IS_ENABLED
#ifdef ENFORCE_TRACE_OAH
  gLogStream <<
    "Creating global msr2mxmlTree OAH group" <<
    endl;
#endif
#endif

  // protect library against multiple initializations
  if (! gGlobalMsr2mxmlTreeOahGroup) {
    // create the global options group
    gGlobalMsr2mxmlTreeOahGroup =
      msr2mxmlTreeOahGroup::create ();
    assert (gGlobalMsr2mxmlTreeOahGroup != 0);
  }

  // return the global OAH group
  return gGlobalMsr2mxmlTreeOahGroup;
}


}
