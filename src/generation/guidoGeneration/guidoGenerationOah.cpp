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

#include "enableExtraOahIfDesired.h"

#include "enableTracingIfDesired.h"
#ifdef TRACING_IS_ENABLED
  #include "traceOah.h"
#endif

#include "oahOah.h"

#include "generalOah.h"
#include "outputFileOah.h"
#include "extraOah.h"

// MSR
#include "msrOah.h"
#include "msr2msrOah.h"

#include "mxmltreeGenerationOah.h"
#include "mxmlTree2msrOah.h"

// mxmlTree
#include "mxmlTreeOah.h"
#include "mxmlTreeOah.h"
#include "mxmltreeGenerationOah.h"

#include "guidoGenerationOah.h"

#include "version.h"


using namespace std;

namespace MusicXML2
{

//_______________________________________________________________________________
S_guidoGenerationOahGroup gGlobalGuidoGenerationOahGroup;

S_guidoGenerationOahGroup guidoGenerationOahGroup::create ()
{
  guidoGenerationOahGroup* o = new guidoGenerationOahGroup ();
  assert (o != nullptr);

  return o;
}

guidoGenerationOahGroup::guidoGenerationOahGroup ()
  : oahGroup (
    "generators",
    "hgens", "help-generators",
R"(Options that are used by generators are grouped here.)",
    oahElementVisibilityKind::kElementVisibilityWhole)
{
  fGenerateGuidoComments = false;
  fGenerateGuidoStem = false;
  fGenerateGuidoBars = false;

  initializeGuidoGenerationOahGroup ();
}

guidoGenerationOahGroup::~guidoGenerationOahGroup ()
{}

//_______________________________________________________________________________
void guidoGenerationOahGroup::initializeGuidoGenerationOahGroup ()
{
#ifdef TRACING_IS_ENABLED
  if (getTraceOah ()) {
  gLogStream << left <<
    "Initializing \"" <<
    fGroupHeader <<
    "\" group" <<
    endl;
  }
#endif

  // Guido
  // --------------------------------------

  createGuidoSubGroup ();
}

void guidoGenerationOahGroup::createGuidoSubGroup ()
{
#ifdef TRACING_IS_ENABLED
  if (getTraceOah ()) {
  gLogStream << left <<
    "Creating insider Guido subgroup in \"" <<
    fGroupHeader <<
    "\"" <<
    endl;
  }
#endif

  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Guido",
        "generator-guido", "help-generator-guido",
R"()",
      oahElementVisibilityKind::kElementVisibilityWhole,
      this);

  appendSubGroupToGroup (subGroup);

  // generate guido comments

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "generate-guido-comments", "",
R"(Generate comments in the Guido output.)",
        "generateGuidoComments",
        fGenerateGuidoComments));

  // generate guido stem

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "generate-guido-stem", "",
R"(Generate stem in the Guido output.)",
        "generateGuidoStem",
        fGenerateGuidoStem));

  // generate guido bars

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "generate-guido-bars", "",
R"(Generate barlines in the Guido output.)",
        "generateGuidoBars",
        fGenerateGuidoBars));
}

//______________________________________________________________________________
void guidoGenerationOahGroup::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> guidoGenerationOahGroup::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_guidoGenerationOahGroup>*
    p =
      dynamic_cast<visitor<S_guidoGenerationOahGroup>*> (v)) {
        S_guidoGenerationOahGroup elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching guidoGenerationOahGroup::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void guidoGenerationOahGroup::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> guidoGenerationOahGroup::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_guidoGenerationOahGroup>*
    p =
      dynamic_cast<visitor<S_guidoGenerationOahGroup>*> (v)) {
        S_guidoGenerationOahGroup elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching guidoGenerationOahGroup::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void guidoGenerationOahGroup::browseData (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> guidoGenerationOahGroup::browseData ()" <<
      endl;
  }
#endif
}

//______________________________________________________________________________
void guidoGenerationOahGroup::enforceGroupQuietness ()
{}

//______________________________________________________________________________
void guidoGenerationOahGroup::checkGroupOptionsConsistency ()
{}

//______________________________________________________________________________
void guidoGenerationOahGroup::printGuidoGenerationOahGroupValues (
  unsigned int fieldWidth)
{
  gLogStream <<
    "The generators options are:" <<
    endl;

  ++gIndenter;

  // Guido
  // --------------------------------------

  gLogStream << left <<
    setw (fieldWidth) << "Guido:" <<
    endl;

  ++gIndenter;

  gLogStream << left <<
    setw (fieldWidth) <<
    "generateGuidoComments" << " : " << booleanAsString (fGenerateGuidoComments) <<
    endl <<
    setw (fieldWidth) <<
    "generateGuidoStem" << " : " << booleanAsString (fGenerateGuidoStem) <<
    endl <<
    setw (fieldWidth) <<
    "generateGuidoBars" << " : " << booleanAsString (fGenerateGuidoBars) <<
    endl;

  --gIndenter;

  --gIndenter;
}

//______________________________________________________________________________
S_guidoGenerationOahGroup createGlobalGuidoGenerationOahGroup ()
{
#ifdef TRACING_IS_ENABLED
  if (getTraceOah ()) {
    gLogStream <<
      "Creating global generators insider OAH group" <<
      endl;
  }
#endif

  // protect library against multiple initializations
  if (! gGlobalGuidoGenerationOahGroup) {

    // create the global OAH group
    // ------------------------------------------------------

    gGlobalGuidoGenerationOahGroup =
      guidoGenerationOahGroup::create ();
    assert (gGlobalGuidoGenerationOahGroup != 0);

    // append versions information to list
    // ------------------------------------------------------

    appendVersionToVersionInfoList (
      "Initial",
      "December 2020",
      "First draft version");
  }

  // return the global OAH group
  return gGlobalGuidoGenerationOahGroup;
}


}
