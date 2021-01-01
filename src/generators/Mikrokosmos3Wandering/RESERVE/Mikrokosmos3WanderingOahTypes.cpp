/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#include "version.h"
#include "utilities.h"

#include "enableTracingIfDesired.h"
#ifdef TRACING_IS_ENABLED
  #include "traceOah.h"
#endif

#include "oahOah.h"

#include "Mikrokosmos3WanderingOahTypes.h"


using namespace std;

namespace MusicXML2
{

//______________________________________________________________________________
string generatedCodeKindAsString (generatedCodeKind kind)
{
  string result;

  switch (kind) {
    case k_NoGeneratedCode:
      result = "*NoGeneratedCode*";
      break;
    case kGuido:
      result = "Guido";
      break;
    case kLilyPond:
      result = "LilyPond";
      break;
    case kBrailleMusic:
      result = "BrailleMusic";
      break;
    case kMusicXML:
      result = "MusicXML";
      break;
  } // switch

  return result;
}

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
    fBrailleOutputKindVariable (
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
      gGlobalBsrBrailleOutputKindsMap.find (
        theString);

  if (it == gGlobalBsrBrailleOutputKindsMap.end ()) {
    // no, optional values style kind is unknown in the map
    stringstream s;

    s <<
      "braille ouptput kind '" << theString <<
      "' is unknown" <<
      endl <<
      "The " <<
      gGlobalBsrBrailleOutputKindsMap.size () - 1 <<
      " known braille ouptput kind are:" <<
      endl;

    gIndenter++;

    s <<
      existingBsrBrailleOutputKinds (K_NAMES_LIST_MAX_LENGTH);

    gIndenter--;

    oahError (s.str ());
  }

  fBrailleOutputKindVariable =
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
    generatedCodeKindAsString (fBrailleOutputKindVariable);

  return s.str ();
}

string generatedCodeKindAtom::asActualLongNamedOptionString () const
{
  stringstream s;

  s <<
    "-" << fLongName << " " <<
    generatedCodeKindAsString (fBrailleOutputKindVariable);

  return s.str ();
}

void generatedCodeKindAtom::print (ostream& os) const
{
  const unsigned int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "OptionsUTFKindAtom:" <<
    endl;

  gIndenter++;

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
      fBrailleOutputKindVariable) <<
      "\"" <<
    endl;

  gIndenter--;
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
      fBrailleOutputKindVariable) <<
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

//______________________________________________________________________________
generatedCodeKind gGeneratedCodeKind = k_NoGeneratedCode;

void registerGeneratedCodeKind (generatedCodeKind kind)
{
  if (gGeneratedCodeKind != k_NoGeneratedCode) {
    cerr << "only one of '-guido', '-lilypond', '-braille' and '-musicxml' can be used" << endl;
    exit (2);
  }
  else {
    gGeneratedCodeKind = kind;
  }
}


}
