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

#include "waeMessagesHandling.h"

#include "oahOah.h"
#include "generalOah.h"
#include "mxmltreeGenerationOah.h"

#include "oahAtomsCollection.h"


using namespace std;

namespace MusicXML2
{

//______________________________________________________________________________
S_mxmltreeGenerationOahGroup gGlobalMxmltreeGenerationOahGroup;

S_mxmltreeGenerationOahGroup mxmltreeGenerationOahGroup::create ()
{
  mxmltreeGenerationOahGroup* o = new mxmltreeGenerationOahGroup ();
  assert (o != nullptr);
  return o;
}

mxmltreeGenerationOahGroup::mxmltreeGenerationOahGroup ()
  : oahGroup (
    "Msr2mxmlTree",
    "hmsr2mxmlt", "help-msr-to-mxmlTree",
R"(These options control the way MSR is translated to xmlelement tree.)",
    oahElementVisibilityKind::kElementVisibilityWhole)
{
  initializeMxmltreeGenerationOahGroup ();
}

mxmltreeGenerationOahGroup::~mxmltreeGenerationOahGroup ()
{}

void mxmltreeGenerationOahGroup::initializeCodeGenerationOptions ()
{
  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Code generation",
        "hmxmlcg", "help-musicxml-code-generation",
R"()",
      oahElementVisibilityKind::kElementVisibilityWhole,
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

void mxmltreeGenerationOahGroup::initializeMxmltreeGenerationOahGroup ()
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

void mxmltreeGenerationOahGroup::enforceGroupQuietness ()
{}

void mxmltreeGenerationOahGroup::checkGroupOptionsConsistency ()
{}

void mxmltreeGenerationOahGroup::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> mxmltreeGenerationOahGroup::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_mxmltreeGenerationOahGroup>*
    p =
      dynamic_cast<visitor<S_mxmltreeGenerationOahGroup>*> (v)) {
        S_mxmltreeGenerationOahGroup elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching mxmltreeGenerationOahGroup::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void mxmltreeGenerationOahGroup::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> mxmltreeGenerationOahGroup::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_mxmltreeGenerationOahGroup>*
    p =
      dynamic_cast<visitor<S_mxmltreeGenerationOahGroup>*> (v)) {
        S_mxmltreeGenerationOahGroup elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching mxmltreeGenerationOahGroup::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void mxmltreeGenerationOahGroup::browseData (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> mxmltreeGenerationOahGroup::browseData ()" <<
      endl;
  }
#endif
}

void mxmltreeGenerationOahGroup::printMxmltreeGenerationOahValues (unsigned int fieldWidth)
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

ostream& operator<< (ostream& os, const S_mxmltreeGenerationOahGroup& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_mxmltreeGenerationOahGroup createGlobalMxmltreeGenerationOahGroup ()
{
#ifdef TRACING_IS_ENABLED
  if (getTraceOah ()) {
    gLogStream <<
      "Creating global msr2mxmlTree OAH group" <<
      endl;
  }
#endif

  // protect library against multiple initializations
  if (! gGlobalMxmltreeGenerationOahGroup) {
    // create the global options group
    gGlobalMxmltreeGenerationOahGroup =
      mxmltreeGenerationOahGroup::create ();
    assert (gGlobalMxmltreeGenerationOahGroup != 0);
  }

  // return the global OAH group
  return gGlobalMxmltreeGenerationOahGroup;
}


}
