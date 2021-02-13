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

#include <regex>

#include "version.h"
#include "utilities.h"

#include "enableTracingIfDesired.h"
#ifdef TRACING_IS_ENABLED
  #include "traceOah.h"
#endif

#include "waeMessagesHandling.h"

//#include "generatorsBasicTypes.h"

#include "oahOah.h"
#include "generalOah.h"

#include "msdl2msrOah.h"
#include "msdlCompilerInsiderHandler.h"


using namespace std;

namespace MusicXML2
{

//_______________________________________________________________________________

S_msdl2msrOahGroup gGlobalMsdl2msrOahGroup;

S_msdl2msrOahGroup msdl2msrOahGroup::create ()
{
  msdl2msrOahGroup* o = new msdl2msrOahGroup ();
  assert (o != nullptr);
  return o;
}

msdl2msrOahGroup::msdl2msrOahGroup ()
  : oahGroup (
    "msdl2msr",
    "hbsr2brl", "help-bsr-to-braille",
R"(These options control the way BSR data is translated to braille music.)",
    kElementVisibilityWhole)
{
  fGeneratorOutputKind = k_NoOutput;

  fUTFKind = kUTF8; // default value

  fBrailleOutputKind = kBrailleOutputAscii; // default value

  fByteOrderingKind = kByteOrderingNone;

  // initialize it
  initializeMsdl2msrOahGroup ();
}

msdl2msrOahGroup::~msdl2msrOahGroup ()
{}

void msdl2msrOahGroup::initializeGenerateCodeOptions ()
{
  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Generated output",
        "hgc", "help-generate-output",
R"()",
      kElementVisibilityWhole,
      this);

  appendSubGroupToGroup (subGroup);

  // generator output kind

  const generatorOutputKind
    generatorOutputKindDefaultValue =
      kLilyPondOutput; // default value

  fGeneratorOutputKindAtom =
    generatorOutputKindAtom::create (
      K_GENERATED_OUTPUT_KIND_SHORT_NAME, K_GENERATED_OUTPUT_KIND_LONG_NAME,
      regex_replace (
        regex_replace (
          regex_replace (
  R"(Generate GENERATED_OUTPUT_KIND code to the output.
  The NUMBER generated output kinds available are:
  GENERATED_OUTPUT_KINDS.
  The default is 'DEFAULT_VALUE'.)",
            regex ("NUMBER"),
            to_string (gGlobalGeneratorOutputKindsMap.size ())),
          regex ("GENERATED_OUTPUT_KINDS"),
          existingGeneratorOutputKinds (K_NAMES_LIST_MAX_LENGTH)),
        regex ("DEFAULT_VALUE"),
        generatorOutputKindAsString (
          generatorOutputKindDefaultValue)),
      "GENERATED_OUTPUT_KIND",
      "generatorOutputKind",
      fGeneratorOutputKind);

  subGroup->
    appendAtomToSubGroup (
      fGeneratorOutputKindAtom);

  // generator output macros

  S_oahMacroAtom
    guidoMacroAtom =
      oahMacroAtom::create (
        K_GENERATED_OUTPUT_KIND_GUIDO_NAME, "",
        "Generate Guido output");
  guidoMacroAtom->
    appendAtomStringPairToMacro (
      fGeneratorOutputKindAtom, "guido");
  subGroup->
    appendAtomToSubGroup (
      guidoMacroAtom);


  S_oahMacroAtom
    lilypondMacroAtom =
      oahMacroAtom::create (
        K_GENERATED_OUTPUT_KIND_LIlYPOND_NAME, "",
        "Generate LilyPond output");
  lilypondMacroAtom->
    appendAtomStringPairToMacro (
      fGeneratorOutputKindAtom, "lilypond");
  subGroup->
    appendAtomToSubGroup (
      lilypondMacroAtom);

  S_oahMacroAtom
    brailleMacroAtom =
      oahMacroAtom::create (
        K_GENERATED_OUTPUT_KIND_BRAILLE_NAME, "",
        "Generate braille music output");
  brailleMacroAtom->
    appendAtomStringPairToMacro (
      fGeneratorOutputKindAtom, K_GENERATED_OUTPUT_KIND_BRAILLE_NAME);
  subGroup->
    appendAtomToSubGroup (
      brailleMacroAtom);

  S_oahMacroAtom
    musicxmlMacroAtom =
      oahMacroAtom::create (
        K_GENERATED_OUTPUT_KIND_MUSICXML_NAME, "",
        "Generate MusicXML output");
  musicxmlMacroAtom->
    appendAtomStringPairToMacro (
      fGeneratorOutputKindAtom, "musicxml");
  subGroup->
    appendAtomToSubGroup (
      musicxmlMacroAtom);

/* JMI
  S_oahMacroAtom
    midiMacroAtom =
      oahMacroAtom::create (
        K_GENERATED_OUTPUT_KIND_MIDI_NAME, "",
        "Generate MIDI output");
  midiMacroAtom->
    appendAtomStringPairToMacro (
      fGeneratorOutputKindAtom, "midi");
  subGroup->
    appendAtomToSubGroup (
      midiMacroAtom);
      */
}

void msdl2msrOahGroup::initializeMsdl2msrOahGroup ()
{
#ifdef TRACING_IS_ENABLED
  // trace
  // --------------------------------------
// JMI  initializeMsdl2msrTraceOah ();
#endif

  // generate code
  // --------------------------------------
  initializeGenerateCodeOptions ();
}

//______________________________________________________________________________
void msdl2msrOahGroup::enforceGroupQuietness ()
{}

//______________________________________________________________________________
void msdl2msrOahGroup::checkGroupOptionsConsistency ()
{
  switch (fGeneratorOutputKind) {
    case k_NoOutput:
      {
        stringstream s;

        s <<
          fHandlerUpLink->getHandlerExecutableName () <<
          " needs an generate code option chosen among:" <<
          endl;

        ++gIndenter;

        s <<
          existingGeneratorOutputKinds (K_NAMES_LIST_MAX_LENGTH);

        --gIndenter;

        oahError (s.str ());
      }
      break;
    default:
      ;
  } // switch
}

//______________________________________________________________________________
void msdl2msrOahGroup::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> msdl2msrOahGroup::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_msdl2msrOahGroup>*
    p =
      dynamic_cast<visitor<S_msdl2msrOahGroup>*> (v)) {
        S_msdl2msrOahGroup elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching msdl2msrOahGroup::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void msdl2msrOahGroup::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> msdl2msrOahGroup::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_msdl2msrOahGroup>*
    p =
      dynamic_cast<visitor<S_msdl2msrOahGroup>*> (v)) {
        S_msdl2msrOahGroup elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching msdl2msrOahGroup::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void msdl2msrOahGroup::browseData (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> msdl2msrOahGroup::browseData ()" <<
      endl;
  }
#endif
}

//______________________________________________________________________________
void msdl2msrOahGroup::printMsdl2msrOahValues (
  unsigned int fieldWidth)
{
  gLogStream <<
    "The msdl2msr options are:" <<
    endl;

  ++gIndenter;

  // generator output kind
  // --------------------------------------

  gLogStream <<
    "Generator output:" <<
    endl;

  ++gIndenter;

  gLogStream << left <<
    setw (fieldWidth) << "generatorOutputKind" << " : " <<
      generatorOutputKindAsString (fGeneratorOutputKind) <<
      endl;

  --gIndenter;


/* JMI
  // code generation
  // --------------------------------------

  gLogStream <<
    "Braille code generation:" <<
    endl;

  ++gIndenter;

  gLogStream << left <<
    setw (fieldWidth) << "UTFKind" << " : " <<
      bsrUTFKindAsString (fUTFKind) <<
      endl <<

    setw (fieldWidth) << "byteOrderingKind" << " : " <<
      bsrByteOrderingKindAsString (fByteOrderingKind) <<
      endl <<

    setw (fieldWidth) << "useEncodingInFileName" << " : " <<
      booleanAsString (fUseEncodingInFileName) <<
      endl <<

    setw (fieldWidth) << "cellsPerLine" << " : " <<
      fCellsPerLine <<
      endl <<
    setw (fieldWidth) << "measuresPerLine" << " : " <<
      fMeasuresPerLine <<
      endl <<
    setw (fieldWidth) << "linesPerPage" << " : " <<
      fLinesPerPage <<
      endl <<

    setw (fieldWidth) << "xml2brlInfos" << " : " <<
      booleanAsString (fXml2brlInfos) <<
      endl <<

    setw (fieldWidth) << "noBrailleCode" << " : " <<
      booleanAsString (fNoBrailleCode) <<
      endl;

  --gIndenter;
*/

  --gIndenter;
}

ostream& operator<< (ostream& os, const S_msdl2msrOahGroup& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_msdl2msrOahGroup createGlobalMsdl2msrOahGroup ()
{
#ifdef TRACING_IS_ENABLED
#ifdef ENFORCE_TRACE_OAH
  gLogStream <<
    "Creating global msdl2msr OAH group" <<
    endl;
#endif
#endif

  // protect library against multiple initializations
  if (! gGlobalMsdl2msrOahGroup) {

    // initialize the generated output kinds map
    // ------------------------------------------------------

    initializeGeneratorOutputKindsMap ();

    // create the msdl2msr options
    // ------------------------------------------------------

    gGlobalMsdl2msrOahGroup =
      msdl2msrOahGroup::create ();
    assert (gGlobalMsdl2msrOahGroup != 0);
  }

  // return the global OAH group
  return gGlobalMsdl2msrOahGroup;
}


}
