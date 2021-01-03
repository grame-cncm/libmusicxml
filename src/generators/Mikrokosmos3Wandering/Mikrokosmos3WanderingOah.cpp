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

#include "messagesHandling.h"

#include "oahOah.h"
#include "generalOah.h"

#include "Mikrokosmos3WanderingOah.h"


using namespace std;

namespace MusicXML2
{

//______________________________________________________________________________
S_generatedCodeKindAtom generatedCodeKindAtom::create (
  string                shortName,
  string                longName,
  string                description,
  string                valueSpecification,
  string                variableName,
  generatedCodeKind& generatedCodeKindVariable)
{
  generatedCodeKindAtom* o = new
    generatedCodeKindAtom (
      shortName,
      longName,
      description,
      valueSpecification,
      variableName,
      generatedCodeKindVariable);
  assert (o!=0);
  return o;
}

generatedCodeKindAtom::generatedCodeKindAtom (
  string                shortName,
  string                longName,
  string                description,
  string                valueSpecification,
  string                variableName,
  generatedCodeKind& generatedCodeKindVariable)
  : oahAtomWithValue (
      shortName,
      longName,
      description,
      valueSpecification,
      variableName),
    fGeneratedCodeKindVariable (
      generatedCodeKindVariable)
{}

generatedCodeKindAtom::~generatedCodeKindAtom ()
{}

void generatedCodeKindAtom::applyAtomWithValue (
  string   theString,
  ostream& os)
{
  // theString contains the output kind name:
  // is it in the  output kinds map?

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceOah ()) {
    gLogStream <<
      "==> handling atom '" << fetchNames () << "; which is of type 'generatedCodeKindAtom'" <<
      " with value \"" << theString << "\"" <<
      endl;
  }
#endif

  map<string, generatedCodeKind>::const_iterator
    it =
      gGlobalGeneratedCodeKindsMap.find (
        theString);

  if (it == gGlobalGeneratedCodeKindsMap.end ()) {
    // no, optional values style kind is unknown in the map
    stringstream s;

    s <<
      "generated code kind '" << theString <<
      "' is unknown" <<
      endl <<
      "The " <<
      gGlobalGeneratedCodeKindsMap.size () - 1 <<
      " known generated code kind are:" <<
      endl;

    ++gIndenter;

    s <<
      existingGeneratedCodeKinds (K_NAMES_LIST_MAX_LENGTH);

    --gIndenter;

    oahError (s.str ());
  }

  fGeneratedCodeKindVariable =
    (*it).second;
  fVariableHasBeenSet = true;
}

void generatedCodeKindAtom::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> generatedCodeKindAtom::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_generatedCodeKindAtom>*
    p =
      dynamic_cast<visitor<S_generatedCodeKindAtom>*> (v)) {
        S_generatedCodeKindAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching generatedCodeKindAtom::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void generatedCodeKindAtom::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> generatedCodeKindAtom::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_generatedCodeKindAtom>*
    p =
      dynamic_cast<visitor<S_generatedCodeKindAtom>*> (v)) {
        S_generatedCodeKindAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching generatedCodeKindAtom::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void generatedCodeKindAtom::browseData (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> generatedCodeKindAtom::browseData ()" <<
      endl;
  }
#endif
}

string generatedCodeKindAtom::asShortNamedOptionString () const
{
  stringstream s;

  s <<
    "-" << fShortName << " " <<
    generatedCodeKindAsString (fGeneratedCodeKindVariable);

  return s.str ();
}

string generatedCodeKindAtom::asActualLongNamedOptionString () const
{
  stringstream s;

  s <<
    "-" << fLongName << " " <<
    generatedCodeKindAsString (fGeneratedCodeKindVariable);

  return s.str ();
}

void generatedCodeKindAtom::print (ostream& os) const
{
  const unsigned int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "OptionsUTFKindAtom:" <<
    endl;

  ++gIndenter;

  printAtomWithValueEssentials (
    os, fieldWidth);

  os << left <<
    setw (fieldWidth) <<
    "fVariableName" << " : " <<
    fVariableName <<
    endl <<
    setw (fieldWidth) <<
    "brailleUTFKindVariable" << " : \"" <<
    generatedCodeKindAsString (
      fGeneratedCodeKindVariable) <<
      "\"" <<
    endl;

  --gIndenter;
}

void generatedCodeKindAtom::printAtomWithValueOptionsValues (
  ostream&     os,
  unsigned int valueFieldWidth) const
{
  os << left <<
    setw (valueFieldWidth) <<
    fVariableName <<
    " : \"" <<
    generatedCodeKindAsString (
      fGeneratedCodeKindVariable) <<
    "\"";
  if (fVariableHasBeenSet) {
    os <<
      ", variableHasBeenSet: " <<
      booleanAsString (fVariableHasBeenSet);
  }
  os << endl;
}

ostream& operator<< (ostream& os, const S_generatedCodeKindAtom& elt)
{
  elt->print (os);
  return os;
}

/* JMI
//______________________________________________________________________________
S_brailleUTFKindAtom brailleUTFKindAtom::create (
  string      shortName,
  string      longName,
  string      description,
  string      valueSpecification,
  string      variableName,
  bsrUTFKind& brailleUTFKindVariable)
{
  brailleUTFKindAtom* o = new
    brailleUTFKindAtom (
      shortName,
      longName,
      description,
      valueSpecification,
      variableName,
      brailleUTFKindVariable);
  assert (o!=0);
  return o;
}

brailleUTFKindAtom::brailleUTFKindAtom (
  string      shortName,
  string      longName,
  string      description,
  string      valueSpecification,
  string      variableName,
  bsrUTFKind& brailleUTFKindVariable)
  : oahAtomWithValue (
      shortName,
      longName,
      description,
      valueSpecification,
      variableName),
    fBsrUTFKindVariable (
      brailleUTFKindVariable)
{}

brailleUTFKindAtom::~brailleUTFKindAtom ()
{}

void brailleUTFKindAtom::applyAtomWithValue (
  string   theString,
  ostream& os)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceOah ()) {
    gLogStream <<
      "==> handling atom '" << fetchNames () << "; which is of type 'brailleUTFKindAtom'" <<
      " with value \"" << theString << "\"" <<
      endl;
  }
#endif

  bsrUTFKind UTFKind = kUTF8; // default value

  if (theString == "8") {
    UTFKind = kUTF8;
  }
  else if (theString == "16") {
    UTFKind = kUTF16;
  }
  else {
    // no, theString is unknown
    stringstream s;

    s <<
      "UTF kind '" << theString <<
      "' is unknown" <<
      endl <<
      "The possible values are 8 and 16" <<
      endl;

    oahError (s.str ());
  }

  fBsrUTFKindVariable = UTFKind;
  fVariableHasBeenSet = true;
}

void brailleUTFKindAtom::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> brailleUTFKindAtom::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_brailleUTFKindAtom>*
    p =
      dynamic_cast<visitor<S_brailleUTFKindAtom>*> (v)) {
        S_brailleUTFKindAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching brailleUTFKindAtom::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void brailleUTFKindAtom::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> brailleUTFKindAtom::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_brailleUTFKindAtom>*
    p =
      dynamic_cast<visitor<S_brailleUTFKindAtom>*> (v)) {
        S_brailleUTFKindAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching brailleUTFKindAtom::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void brailleUTFKindAtom::browseData (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> brailleUTFKindAtom::browseData ()" <<
      endl;
  }
#endif
}

string brailleUTFKindAtom::asShortNamedOptionString () const
{
  stringstream s;

  s <<
    "-" << fShortName << " " <<
    bsrUTFKindAsString (fBsrUTFKindVariable);

  return s.str ();
}

string brailleUTFKindAtom::asActualLongNamedOptionString () const
{
  stringstream s;

  s <<
    "-" << fLongName << " " <<
    bsrUTFKindAsString (fBsrUTFKindVariable);

  return s.str ();
}

void brailleUTFKindAtom::print (ostream& os) const
{
  const unsigned int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "OptionsUTFKindAtom:" <<
    endl;

  ++gIndenter;

  printAtomWithValueEssentials (
    os, fieldWidth);

  os << left <<
    setw (fieldWidth) <<
    "fVariableName" << " : " <<
    fVariableName <<
    endl <<
    setw (fieldWidth) <<
    "brailleUTFKindVariable" << " : \"" <<
    bsrUTFKindAsString (
      fBsrUTFKindVariable) <<
      "\"" <<
    endl;

  --gIndenter;
}

void brailleUTFKindAtom::printAtomWithValueOptionsValues (
  ostream&     os,
  unsigned int valueFieldWidth) const
{
  os << left <<
    setw (valueFieldWidth) <<
    fVariableName <<
    " : \"" <<
    bsrUTFKindAsString (
      fBsrUTFKindVariable) <<
    "\"";
  if (fVariableHasBeenSet) {
    os <<
      ", variableHasBeenSet: " <<
      booleanAsString (fVariableHasBeenSet);
  }
  os << endl;
}

ostream& operator<< (ostream& os, const S_brailleUTFKindAtom& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_brailleByteOrderingKindAtom brailleByteOrderingKindAtom::create (
  string               shortName,
  string               longName,
  string               description,
  string               valueSpecification,
  string               variableName,
  bsrByteOrderingKind& brailleByteOrderingKindVariable)
{
  brailleByteOrderingKindAtom* o = new
    brailleByteOrderingKindAtom (
      shortName,
      longName,
      description,
      valueSpecification,
      variableName,
      brailleByteOrderingKindVariable);
  assert (o!=0);
  return o;
}

brailleByteOrderingKindAtom::brailleByteOrderingKindAtom (
  string               shortName,
  string               longName,
  string               description,
  string               valueSpecification,
  string               variableName,
  bsrByteOrderingKind& brailleByteOrderingKindVariable)
  : oahAtomWithValue (
      shortName,
      longName,
      description,
      valueSpecification,
      variableName),
    fBsrByteOrderingKindVariable (
      brailleByteOrderingKindVariable)
{}

brailleByteOrderingKindAtom::~brailleByteOrderingKindAtom ()
{}

void brailleByteOrderingKindAtom::applyAtomWithValue (
  string   theString,
  ostream& os)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceOah ()) {
    gLogStream <<
      "==> handling atom '" << fetchNames () << "; which is of type 'brailleByteOrderingKindAtom'" <<
      " with value \"" << theString << "\"" <<
      endl;
  }
#endif

  bsrByteOrderingKind byteOrderingKind = kByteOrderingNone;

  if (theString == "big") {
    byteOrderingKind = kByteOrderingBigEndian;
  }
  else if (theString == "small") {
    byteOrderingKind = kByteOrderingSmallEndian;
  }
  else {
    // no, theString is unknown
    stringstream s;

    s <<
      "byte ordering '" << theString <<
      "' is unknown" <<
      endl <<
      "The possible values are big and small" <<
      endl;

    oahError (s.str ());
  }

  fBsrByteOrderingKindVariable = byteOrderingKind;
  fVariableHasBeenSet = true;
}

void brailleByteOrderingKindAtom::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> brailleByteOrderingKindAtom::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_brailleByteOrderingKindAtom>*
    p =
      dynamic_cast<visitor<S_brailleByteOrderingKindAtom>*> (v)) {
        S_brailleByteOrderingKindAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching brailleByteOrderingKindAtom::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void brailleByteOrderingKindAtom::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> brailleByteOrderingKindAtom::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_brailleByteOrderingKindAtom>*
    p =
      dynamic_cast<visitor<S_brailleByteOrderingKindAtom>*> (v)) {
        S_brailleByteOrderingKindAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching brailleByteOrderingKindAtom::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void brailleByteOrderingKindAtom::browseData (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> brailleByteOrderingKindAtom::browseData ()" <<
      endl;
  }
#endif
}

string brailleByteOrderingKindAtom::asShortNamedOptionString () const
{
  stringstream s;

  s <<
    "-" << fShortName << " " <<
    bsrByteOrderingKindAsString (fBsrByteOrderingKindVariable);

  return s.str ();
}

string brailleByteOrderingKindAtom::asActualLongNamedOptionString () const
{
  stringstream s;

  s <<
    "-" << fLongName << " " <<
    bsrByteOrderingKindAsString (fBsrByteOrderingKindVariable);

  return s.str ();
}

void brailleByteOrderingKindAtom::print (ostream& os) const
{
  const unsigned int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "OptionsByteOrderingKindAtom:" <<
    endl;

  ++gIndenter;

  printAtomWithValueEssentials (
    os, fieldWidth);

  os << left <<
    setw (fieldWidth) <<
    "fVariableName" << " : " <<
    fVariableName <<
    endl <<
    setw (fieldWidth) <<
    "brailleByteOrderingKindVariable" << " : \"" <<
    bsrByteOrderingKindAsString (
      fBsrByteOrderingKindVariable) <<
      "\"" <<
    endl;

  --gIndenter;
}

void brailleByteOrderingKindAtom::printAtomWithValueOptionsValues (
  ostream&     os,
  unsigned int valueFieldWidth) const
{
  os << left <<
    setw (valueFieldWidth) <<
    fVariableName <<
    " : \"" <<
    bsrByteOrderingKindAsString (
      fBsrByteOrderingKindVariable) <<
    "\"";
  if (fVariableHasBeenSet) {
    os <<
      ", variableHasBeenSet: " <<
      booleanAsString (fVariableHasBeenSet);
  }
  os << endl;
}

ostream& operator<< (ostream& os, const S_brailleByteOrderingKindAtom& elt)
{
  elt->print (os);
  return os;
}
*/

//_______________________________________________________________________________

S_Mikrokosmos3WanderingOahGroup gGlobalMikrokosmos3WanderingOahGroup;

S_Mikrokosmos3WanderingOahGroup Mikrokosmos3WanderingOahGroup::create ()
{
  Mikrokosmos3WanderingOahGroup* o = new Mikrokosmos3WanderingOahGroup ();
  assert (o!=0);
  return o;
}

Mikrokosmos3WanderingOahGroup::Mikrokosmos3WanderingOahGroup ()
  : oahGroup (
    "Mikrokosmos3Wandering",
    "hbsr2brl", "help-bsr-to-braille",
R"(These options control the way BSR data is translated to braille music.)",
    kElementVisibilityWhole)
{
  fGeneratedCodeKind = kLilyPond; // default value

  /* jMI
  fBrailleMusicCodeKind = kGeneratedCodeAscii; // default value

  fUTFKind = kUTF8; // default value

  fByteOrderingKind = kByteOrderingNone;
*/

  // initialize it
  initializeMikrokosmos3WanderingOahGroup ();
}

Mikrokosmos3WanderingOahGroup::~Mikrokosmos3WanderingOahGroup ()
{}

void Mikrokosmos3WanderingOahGroup::initializeGeneratedCodeOptions ()
{
  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Generated code",
        "hgc", "help-generated-code",
R"()",
      kElementVisibilityWhole,
      this);

  appendSubGroupToGroup (subGroup);

  const generatedCodeKind
    generatedCodeKindDefaultValue =
      kLilyPond; // default value

  fGeneratedCodeKindAtom =
      generatedCodeKindAtom::create (
      "gck", "generated-code-kind",
      regex_replace (
        regex_replace (
          regex_replace (
R"(Generate GENERATED_CODE code to the output.
The NUMBER generated code kinds available are:
GENERATED_CODE_KINDS.
The default is 'DEFAULT_VALUE'.)",
            regex ("NUMBER"),
            to_string (gGlobalGeneratedCodeKindsMap.size ())),
          regex ("GENERATED_CODE_KINDS"),
          existingGeneratedCodeKinds (K_NAMES_LIST_MAX_LENGTH)),
        regex ("DEFAULT_VALUE"),
        generatedCodeKindAsString (
          generatedCodeKindDefaultValue)),
      "GENERATED_CODE",
      "generatedCodeKind",
      fGeneratedCodeKind);

  subGroup->
    appendAtomToSubGroup (
      fGeneratedCodeKindAtom);
}

/* JMI
void bsr2brailleOahGroup::initializeBrailleEncodingOptions ()
{
  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Braille encoding",
        "hbe", "help-braille-encoding",
R"()",
      kElementVisibilityWhole,
      this);

  appendSubGroupToGroup (subGroup);

  const generatedCodeKind
    generatedCodeKindDefaultValue =
      kBrailleOutputAscii; // default value

  fBrailleOutputKindAtom =
      brailleOutputKindAtom::create (
      "bok", "braille-output-kind",
      regex_replace (
        regex_replace (
          regex_replace (
R"(Use OUTPUT_KIND to write the generated braille music to the output.
The NUMBER output kinds available are:
BRAILLE_OUTPUT_KINDS.
'utf8d' leads to every line in the braille score to be generated
as a line of cells followed by a line of text showing the contents
for debug purposes.
The default is 'DEFAULT_VALUE'.)",
            regex ("NUMBER"),
            to_string (gGlobalGeneratedCodeKindsMap.size ())),
          regex ("BRAILLE_OUTPUT_KINDS"),
          existingGeneratedCodeKinds (K_NAMES_LIST_MAX_LENGTH)),
        regex ("DEFAULT_VALUE"),
        generatedCodeKindAsString (
          generatedCodeKindDefaultValue)),
      "OUTPUT_KIND",
      "brailleOutputKind",
      fBrailleOutputKind);

  subGroup->
    appendAtomToSubGroup (
      fBrailleOutputKindAtom);
}
*/

void Mikrokosmos3WanderingOahGroup::initializeMikrokosmos3WanderingOahGroup ()
{
#ifdef TRACING_IS_ENABLED
  // trace
  // --------------------------------------
// JMI  initializeMikrokosmos3WanderingTraceOah ();
#endif

  // generated code
  // --------------------------------------
  initializeGeneratedCodeOptions ();

/* JMI
  // UTF encoding
  // --------------------------------------
  initializeBrailleEncodingOptions ();

  // byte ordering
  // --------------------------------------
  initializeBrailleByteOrderingOptions ();

  // braille music file name
  // --------------------------------------
  initializeBrailleMusicFileNameOptions ();

  // page parameters
  // --------------------------------------
  initializeBraillePageParametersOptions ();

  // code generation
  // --------------------------------------
  initializeBrailleCodeGenerationOptions ();
  */
}

//______________________________________________________________________________
void Mikrokosmos3WanderingOahGroup::enforceGroupQuietness ()
{}

//______________________________________________________________________________
void Mikrokosmos3WanderingOahGroup::checkGroupOptionsConsistency ()
{}

//______________________________________________________________________________
void Mikrokosmos3WanderingOahGroup::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> Mikrokosmos3WanderingOahGroup::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_Mikrokosmos3WanderingOahGroup>*
    p =
      dynamic_cast<visitor<S_Mikrokosmos3WanderingOahGroup>*> (v)) {
        S_Mikrokosmos3WanderingOahGroup elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching Mikrokosmos3WanderingOahGroup::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void Mikrokosmos3WanderingOahGroup::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> Mikrokosmos3WanderingOahGroup::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_Mikrokosmos3WanderingOahGroup>*
    p =
      dynamic_cast<visitor<S_Mikrokosmos3WanderingOahGroup>*> (v)) {
        S_Mikrokosmos3WanderingOahGroup elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching Mikrokosmos3WanderingOahGroup::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void Mikrokosmos3WanderingOahGroup::browseData (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> Mikrokosmos3WanderingOahGroup::browseData ()" <<
      endl;
  }
#endif
}

//______________________________________________________________________________
void Mikrokosmos3WanderingOahGroup::printMikrokosmos3WanderingOahValues (unsigned int fieldWidth)
{
  gLogStream <<
    "The Mikrokosmos3Wandering options are:" <<
    endl;

  ++gIndenter;

  // generated code
  // --------------------------------------

  gLogStream <<
    "Generated code:" <<
    endl;

  ++gIndenter;

  gLogStream << left <<
    setw (fieldWidth) << "generatedCodeKind" << " : " <<
      generatedCodeKindAsString (fGeneratedCodeKind) <<
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

ostream& operator<< (ostream& os, const S_Mikrokosmos3WanderingOahGroup& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_Mikrokosmos3WanderingOahGroup createGlobalMikrokosmos3WanderingOahGroup ()
{
#ifdef TRACING_IS_ENABLED
#ifdef ENFORCE_TRACE_OAH
  gLogStream <<
    "Creating global Mikrokosmos3Wandering OAH group" <<
    endl;
#endif
#endif

  // protect library against multiple initializations
  if (! gGlobalMikrokosmos3WanderingOahGroup) {

    // initialize the generated kinds map
    // ------------------------------------------------------

    initializeGeneratedCodeKindsMap ();

    // create the Mikrokosmos3Wandering options
    // ------------------------------------------------------

    gGlobalMikrokosmos3WanderingOahGroup =
      Mikrokosmos3WanderingOahGroup::create ();
    assert (gGlobalMikrokosmos3WanderingOahGroup != 0);
  }

  // return the global OAH group
  return gGlobalMikrokosmos3WanderingOahGroup;
}


}
