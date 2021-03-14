/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#include <regex>

#include "enableTracingIfDesired.h"
#ifdef TRACING_IS_ENABLED
  #include "traceOah.h"
#endif

#include "oahOah.h"

#include "visitor.h"

#include "msrBasicTypes.h"        // for " K_NAMES_LIST_MAX_LENGTH

#include "multiGenerationOah.h"

#include "waeMessagesHandling.h"


using namespace std;

namespace MusicXML2
{

//______________________________________________________________________________
string multiGeneratorOutputKindAsString (
  multiGeneratorOutputKind generatorOutputKind)
{
  // no CamelCase here, these strings are used in the command line options
  string result;

  switch (generatorOutputKind) {
    case multiGeneratorOutputKind::k_NoOutput:
      result = "*noGenerateOutputKind*";
      break;
    case multiGeneratorOutputKind::kGuidoOutput:
      result = "Guido output";
      break;
    case multiGeneratorOutputKind::kLilyPondOutput:
      result = "LilyPond output";
      break;
    case multiGeneratorOutputKind::kBrailleOutput:
      result = "braille output";
      break;
    case multiGeneratorOutputKind::kMusicXMLOutput:
      result = "MusicXML output";
      break;
    case multiGeneratorOutputKind::kMidiOutput:
      result = "MIDI output";
      break;
  } // switch

  return result;
}

EXP multiGeneratorOutputKind multiGeneratorOutputKindFromString (
  const string& theString)
{
  multiGeneratorOutputKind result = multiGeneratorOutputKind::k_NoOutput;

  if      (theString == "guido") {
    result = multiGeneratorOutputKind::kGuidoOutput;
  }
  else if (theString == "lilypond") {
    result = multiGeneratorOutputKind::kLilyPondOutput;
  }
  else if (theString == "braille") {
    result = multiGeneratorOutputKind::kBrailleOutput;
  }
  else if (theString == "musicxml") {
    result = multiGeneratorOutputKind::kMusicXMLOutput;
  }
  else if (theString == "midi") {
    result = multiGeneratorOutputKind::kMidiOutput;
  }
  else {
    stringstream s;

    s <<
      "the string \"" <<
      theString <<
      "\" is no valid generated output kind";

      msgAssert (
        __FILE__, __LINE__,
      false, s.str ());
  }

  return result;
}

map<string, multiGeneratorOutputKind>
  gGlobalMultiGeneratorOutputKindsMap;

void initializeMultiGeneratorOutputKindsMap ()
{
  gGlobalMultiGeneratorOutputKindsMap ["guido"]    =
    multiGeneratorOutputKind::kGuidoOutput;
  gGlobalMultiGeneratorOutputKindsMap ["lilypond"] =
    multiGeneratorOutputKind::kLilyPondOutput;
  gGlobalMultiGeneratorOutputKindsMap ["braille"]  =
    multiGeneratorOutputKind::kBrailleOutput;
  gGlobalMultiGeneratorOutputKindsMap ["musicxml"] =
    multiGeneratorOutputKind::kMusicXMLOutput;
  gGlobalMultiGeneratorOutputKindsMap ["midi"]     =
    multiGeneratorOutputKind::kMidiOutput;
}

string existingMultiGeneratorOutputKinds (
  unsigned int namesListMaxLength)
{
  stringstream s;

  unsigned int brailleOutputKindsMapSize =
    gGlobalMultiGeneratorOutputKindsMap.size ();

  if (brailleOutputKindsMapSize) {
    unsigned int nextToLast =
      brailleOutputKindsMapSize - 1;

    unsigned int count = 0;
    unsigned int cumulatedLength = 0;

    for (
      map<string, multiGeneratorOutputKind>::const_iterator i =
        gGlobalMultiGeneratorOutputKindsMap.begin ();
      i != gGlobalMultiGeneratorOutputKindsMap.end ();
      ++i
    ) {
      string theString = (*i).first;

      ++count;

      cumulatedLength += theString.size ();
      if (cumulatedLength >= namesListMaxLength) {
        s << "\n";
        cumulatedLength = 0;
      }

      s << theString;

      if (count == nextToLast) {
        s << " and ";
      }
      else if (count != brailleOutputKindsMapSize) {
        s << ", ";
      }
    } // for
  }

  return s.str ();
}

//______________________________________________________________________________
S_multiGeneratorOutputKindAtom multiGeneratorOutputKindAtom::create (
  string               shortName,
  string               longName,
  string               description,
  string               valueSpecification,
  string               variableName,
  multiGeneratorOutputKind& multiGeneratorOutputKindVariable)
{
  multiGeneratorOutputKindAtom* o = new
    multiGeneratorOutputKindAtom (
      shortName,
      longName,
      description,
      valueSpecification,
      variableName,
      multiGeneratorOutputKindVariable);
  assert (o != nullptr);
  return o;
}

multiGeneratorOutputKindAtom::multiGeneratorOutputKindAtom (
  string               shortName,
  string               longName,
  string               description,
  string               valueSpecification,
  string               variableName,
  multiGeneratorOutputKind& multiGeneratorOutputKindVariable)
  : oahAtomStoringAValueInAVariable (
      shortName,
      longName,
      description,
      valueSpecification,
      variableName),
    fGeneratorOutputKindVariable (
      multiGeneratorOutputKindVariable)
{}

multiGeneratorOutputKindAtom::~multiGeneratorOutputKindAtom ()
{}

void multiGeneratorOutputKindAtom::applyAtomWithValue (
  const string& theString,
  ostream&      os)
{
  // theString contains the output kind name:
  // is it in the  output kinds map?

#ifdef TRACING_IS_ENABLED
  if (getTraceOah ()) {
    gLogStream <<
      "==> handling atom '" <<
      fetchNames () <<
      "; which is of type 'multiGeneratorOutputKindAtom'" <<
      " with value \"" << theString << "\"" <<
      endl;
  }
#endif

  map<string, multiGeneratorOutputKind>::const_iterator
    it =
      gGlobalMultiGeneratorOutputKindsMap.find (
        theString);

  if (it == gGlobalMultiGeneratorOutputKindsMap.end ()) {
    // no, optional values style kind is unknown in the map
    stringstream s;

    s <<
      "generated output kind '" << theString <<
      "' is unknown" <<
      endl <<
      "The " <<
      gGlobalMultiGeneratorOutputKindsMap.size () - 1 <<
      " known generated output kinds are:" <<
      endl;

    ++gIndenter;

    s <<
      existingMultiGeneratorOutputKinds (K_NAMES_LIST_MAX_LENGTH);

    --gIndenter;

    oahError (s.str ());
  }

  fGeneratorOutputKindVariable =
    (*it).second;
  fVariableHasBeenSet = true;
}

void multiGeneratorOutputKindAtom::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> multiGeneratorOutputKindAtom::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_multiGeneratorOutputKindAtom>*
    p =
      dynamic_cast<visitor<S_multiGeneratorOutputKindAtom>*> (v)) {
        S_multiGeneratorOutputKindAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching multiGeneratorOutputKindAtom::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void multiGeneratorOutputKindAtom::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> multiGeneratorOutputKindAtom::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_multiGeneratorOutputKindAtom>*
    p =
      dynamic_cast<visitor<S_multiGeneratorOutputKindAtom>*> (v)) {
        S_multiGeneratorOutputKindAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching multiGeneratorOutputKindAtom::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void multiGeneratorOutputKindAtom::browseData (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> multiGeneratorOutputKindAtom::browseData ()" <<
      endl;
  }
#endif
}

string multiGeneratorOutputKindAtom::asShortNamedOptionString () const
{
  stringstream s;

  s <<
    "-" << fShortName << " " <<
    multiGeneratorOutputKindAsString (fGeneratorOutputKindVariable);

  return s.str ();
}

string multiGeneratorOutputKindAtom::asActualLongNamedOptionString () const
{
  stringstream s;

  s <<
    "-" << fLongName << " " <<
    multiGeneratorOutputKindAsString (fGeneratorOutputKindVariable);

  return s.str ();
}

void multiGeneratorOutputKindAtom::print (ostream& os) const
{
  const unsigned int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "GeneratorOutputKindAtom:" <<
    endl;

  ++gIndenter;

  printAtomWithVariableEssentials (
    os, fieldWidth);

  os << left <<
    setw (fieldWidth) <<
    "variableName" << " : " <<
    fVariableName <<
    endl <<
    setw (fieldWidth) <<
    "brailleUTFKindVariable" << " : \"" <<
    multiGeneratorOutputKindAsString (
      fGeneratorOutputKindVariable) <<
      "\"" <<
    endl;

  --gIndenter;
}

void multiGeneratorOutputKindAtom::printAtomWithVariableOptionsValues (
  ostream&     os,
  unsigned int valueFieldWidth) const
{
  os << left <<
    setw (valueFieldWidth) <<
    fVariableName <<
    " : \"" <<
    multiGeneratorOutputKindAsString (
      fGeneratorOutputKindVariable) <<
    "\"";
  if (fVariableHasBeenSet) {
    os <<
      ", variableHasBeenSet: " <<
      booleanAsString (fVariableHasBeenSet);
  }
  os << endl;
}

ostream& operator<< (ostream& os, const S_multiGeneratorOutputKindAtom& elt)
{
  elt->print (os);
  return os;
}

//_______________________________________________________________________________
S_multiGeneratorOahGroup gGlobalMultiGeneratorOahGroup;

S_multiGeneratorOahGroup multiGeneratorOahGroup::create ()
{
  multiGeneratorOahGroup* o = new multiGeneratorOahGroup ();
  assert (o != nullptr);

  return o;
}

multiGeneratorOahGroup::multiGeneratorOahGroup ()
  : oahGroup (
    "Multi generator",
    "hmg", "help-multi-generator",
R"()",
    oahElementVisibilityKind::kElementVisibilityWhole)
{
  initializeMultiGeneratorOahGroup ();
}

multiGeneratorOahGroup::~multiGeneratorOahGroup ()
{}

void multiGeneratorOahGroup::initializeMultiGeneratorOahGroup ()
{
  // generator output kind
  // --------------------------------------

  initializeGenerateCodeOptions ();
}

void multiGeneratorOahGroup::initializeGenerateCodeOptions ()
{
  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Generated output",
        "hgc", "help-generate-output",
R"()",
      oahElementVisibilityKind::kElementVisibilityWhole,
      this);

  appendSubGroupToGroup (subGroup);

  // generator output kind

  const multiGeneratorOutputKind
    multiGeneratorOutputKindDefaultValue =
      multiGeneratorOutputKind::kLilyPondOutput; // default value

  fGeneratorOutputKindAtom =
    multiGeneratorOutputKindAtom::create (
      K_GENERATED_OUTPUT_KIND_SHORT_NAME, K_GENERATED_OUTPUT_KIND_LONG_NAME,
      regex_replace (
        regex_replace (
          regex_replace (
  R"(Generate GENERATED_OUTPUT_KIND code to the output.
  The NUMBER generated output kinds available are:
  GENERATED_OUTPUT_KINDS.
  The default is 'DEFAULT_VALUE'.)",
            regex ("NUMBER"),
            to_string (gGlobalMultiGeneratorOutputKindsMap.size ())),
          regex ("GENERATED_OUTPUT_KINDS"),
          existingMultiGeneratorOutputKinds (K_NAMES_LIST_MAX_LENGTH)),
        regex ("DEFAULT_VALUE"),
        multiGeneratorOutputKindAsString (
          multiGeneratorOutputKindDefaultValue)),
      "GENERATED_OUTPUT_KIND",
      "multiGeneratorOutputKind",
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

//______________________________________________________________________________
void multiGeneratorOahGroup::enforceGroupQuietness ()
{
  fGeneratorOutputKind = multiGeneratorOutputKind::k_NoOutput;
}

//______________________________________________________________________________
void multiGeneratorOahGroup::checkGroupOptionsConsistency ()
{
  // JMI
}

//______________________________________________________________________________
void multiGeneratorOahGroup::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> multiGeneratorOahGroup::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_multiGeneratorOahGroup>*
    p =
      dynamic_cast<visitor<S_multiGeneratorOahGroup>*> (v)) {
        S_multiGeneratorOahGroup elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching multiGeneratorOahGroup::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void multiGeneratorOahGroup::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> multiGeneratorOahGroup::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_multiGeneratorOahGroup>*
    p =
      dynamic_cast<visitor<S_multiGeneratorOahGroup>*> (v)) {
        S_multiGeneratorOahGroup elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching multiGeneratorOahGroup::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void multiGeneratorOahGroup::browseData (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> multiGeneratorOahGroup::browseData ()" <<
      endl;
  }
#endif
}

//______________________________________________________________________________
void multiGeneratorOahGroup::printMultiGeneratorOahValues (unsigned int fieldWidth)
{
  gLogStream <<
    "The MultiGenerator options are:" <<
    endl;

  ++gIndenter;

  // Output kind
  // --------------------------------------

  gLogStream << left <<
    setw (fieldWidth) << "Output kind:" <<
    endl;

  ++gIndenter;

  gLogStream <<
    setw (fieldWidth) << "generatorOutputKind" << " : " <<
    multiGeneratorOutputKindAsString (fGeneratorOutputKind) <<
    endl;

  --gIndenter;

  --gIndenter;
}

ostream& operator<< (ostream& os, const S_multiGeneratorOahGroup& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_multiGeneratorOahGroup createGlobalMultiGeneratorOahGroup ()
{
#ifdef TRACING_IS_ENABLED
  if (getTraceOah ()) {
    gLogStream <<
      "Creating multi generator OAH group" <<
      endl;
  }
#endif

  // protect library against multiple initializations
  if (! gGlobalMultiGeneratorOahGroup) {
    // create the multi generator options group
    gGlobalMultiGeneratorOahGroup =
      multiGeneratorOahGroup::create ();
    assert (gGlobalMultiGeneratorOahGroup != 0);
  }

  // return the multi generator OAH group
  return gGlobalMultiGeneratorOahGroup;
}


}
