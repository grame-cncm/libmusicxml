/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#include "visitor.h"

#include "msrBasicTypes.h"        // for " K_NAMES_LIST_MAX_LENGTH

#include "generatorsBasicTypes.h"


using namespace std;

namespace MusicXML2
{

//______________________________________________________________________________
string generatorOutputKindAsString (
  generatorOutputKind generatorOutputKind)
{
  // no CamelCase here, these strings are used in the command line options
  string result;

  switch (generatorOutputKind) {
    case k_NoOutput:
      result = "*noGenerateOutputKind*";
      break;
    case kGuidoOutput:
      result = "Guido";
      break;
    case kLilyPondOutput:
      result = "LilyPond";
      break;
    case kBrailleOutput:
      result = "braille";
      break;
    case kMusicXMLOutput:
      result = "MusicXML";
      break;
    case kMidiOutput:
      result = "MIDI";
      break;
  } // switch

  return result;
}

EXP generatorOutputKind generatorOutputKindFromString (
  const string& theString)
{
  generatorOutputKind result = k_NoOutput;

  if      (theString == "guido") {
    result = kGuidoOutput;
  }
  else if (theString == "lilypond") {
    result = kLilyPondOutput;
  }
  else if (theString == "braille") {
    result = kBrailleOutput;
  }
  else if (theString == "musicxml") {
    result = kMusicXMLOutput;
  }
  else if (theString == "midi") {
    result = kMidiOutput;
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

map<string, generatorOutputKind>
  gGlobalGeneratorOutputKindsMap;

void initializeGeneratorOutputKindsMap ()
{
  gGlobalGeneratorOutputKindsMap ["guido"]    = kGuidoOutput;
  gGlobalGeneratorOutputKindsMap ["lilypond"] = kLilyPondOutput;
  gGlobalGeneratorOutputKindsMap ["braille"]  = kBrailleOutput;
  gGlobalGeneratorOutputKindsMap ["musicxml"] = kMusicXMLOutput;
  gGlobalGeneratorOutputKindsMap ["midi"] = kMidiOutput;
}

string existingGeneratorOutputKinds (
  unsigned int namesListMaxLength)
{
  stringstream s;

  unsigned int brailleOutputKindsMapSize =
    gGlobalGeneratorOutputKindsMap.size ();

  if (brailleOutputKindsMapSize) {
    unsigned int nextToLast =
      brailleOutputKindsMapSize - 1;

    unsigned int count = 0;
    unsigned int cumulatedLength = 0;

    for (
      map<string, generatorOutputKind>::const_iterator i =
        gGlobalGeneratorOutputKindsMap.begin ();
      i != gGlobalGeneratorOutputKindsMap.end ();
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
S_generatorOutputKindAtom generatorOutputKindAtom::create (
  string               shortName,
  string               longName,
  string               description,
  string               valueSpecification,
  string               variableName,
  generatorOutputKind& generatorOutputKindVariable)
{
  generatorOutputKindAtom* o = new
    generatorOutputKindAtom (
      shortName,
      longName,
      description,
      valueSpecification,
      variableName,
      generatorOutputKindVariable);
  assert (o != nullptr);
  return o;
}

generatorOutputKindAtom::generatorOutputKindAtom (
  string               shortName,
  string               longName,
  string               description,
  string               valueSpecification,
  string               variableName,
  generatorOutputKind& generatorOutputKindVariable)
  : oahAtomWithValue (
      shortName,
      longName,
      description,
      valueSpecification,
      variableName),
    fGeneratorOutputKindVariable (
      generatorOutputKindVariable)
{}

generatorOutputKindAtom::~generatorOutputKindAtom ()
{}

void generatorOutputKindAtom::applyAtomWithValue (
  const string& theString,
  ostream&      os)
{
  // theString contains the output kind name:
  // is it in the  output kinds map?

#ifdef TRACING_IS_ENABLED
  if (getTraceOah ()) {
    gLogStream <<
      "==> handling atom '" << fetchNames () << "; which is of type 'generatorOutputKindAtom'" <<
      " with value \"" << theString << "\"" <<
      endl;
  }
#endif

  map<string, generatorOutputKind>::const_iterator
    it =
      gGlobalGeneratorOutputKindsMap.find (
        theString);

  if (it == gGlobalGeneratorOutputKindsMap.end ()) {
    // no, optional values style kind is unknown in the map
    stringstream s;

    s <<
      "generated output kind '" << theString <<
      "' is unknown" <<
      endl <<
      "The " <<
      gGlobalGeneratorOutputKindsMap.size () - 1 <<
      " known generated output kinds are:" <<
      endl;

    ++gIndenter;

    s <<
      existingGeneratorOutputKinds (K_NAMES_LIST_MAX_LENGTH);

    --gIndenter;

    oahError (s.str ());
  }

  fGeneratorOutputKindVariable =
    (*it).second;
  fVariableHasBeenSet = true;
}

void generatorOutputKindAtom::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> generatorOutputKindAtom::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_generatorOutputKindAtom>*
    p =
      dynamic_cast<visitor<S_generatorOutputKindAtom>*> (v)) {
        S_generatorOutputKindAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching generatorOutputKindAtom::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void generatorOutputKindAtom::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> generatorOutputKindAtom::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_generatorOutputKindAtom>*
    p =
      dynamic_cast<visitor<S_generatorOutputKindAtom>*> (v)) {
        S_generatorOutputKindAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching generatorOutputKindAtom::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void generatorOutputKindAtom::browseData (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> generatorOutputKindAtom::browseData ()" <<
      endl;
  }
#endif
}

string generatorOutputKindAtom::asShortNamedOptionString () const
{
  stringstream s;

  s <<
    "-" << fShortName << " " <<
    generatorOutputKindAsString (fGeneratorOutputKindVariable);

  return s.str ();
}

string generatorOutputKindAtom::asActualLongNamedOptionString () const
{
  stringstream s;

  s <<
    "-" << fLongName << " " <<
    generatorOutputKindAsString (fGeneratorOutputKindVariable);

  return s.str ();
}

void generatorOutputKindAtom::print (ostream& os) const
{
  const unsigned int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "GeneratorOutputKindAtom:" <<
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
    generatorOutputKindAsString (
      fGeneratorOutputKindVariable) <<
      "\"" <<
    endl;

  --gIndenter;
}

void generatorOutputKindAtom::printAtomWithValueOptionsValues (
  ostream&     os,
  unsigned int valueFieldWidth) const
{
  os << left <<
    setw (valueFieldWidth) <<
    fVariableName <<
    " : \"" <<
    generatorOutputKindAsString (
      fGeneratorOutputKindVariable) <<
    "\"";
  if (fVariableHasBeenSet) {
    os <<
      ", variableHasBeenSet: " <<
      booleanAsString (fVariableHasBeenSet);
  }
  os << endl;
}

ostream& operator<< (ostream& os, const S_generatorOutputKindAtom& elt)
{
  elt->print (os);
  return os;
}


}
