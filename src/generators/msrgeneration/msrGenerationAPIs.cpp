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

#include "msrBasicTypes.h"

#include "msrGenerationAPIs.h"


using namespace std;

namespace MusicXML2
{

//______________________________________________________________________________
string msrGenerationAPIKindAsString (
  msrGenerationAPIKind generationAPIKind)
{
  string result;

  switch (generationAPIKind) {
    case kMsrRegularAPIKind:
      result = "MSR regular API";
      break;
    case kMsrStringsAPIKind:
      result = "MSR strings API";
      break;
  } // switch

  return result;
}

EXP msrGenerationAPIKind msrGenerationAPIKindFromString (
  const string& theString)
{
  msrGenerationAPIKind result = kMsrRegularAPIKind; // default value

  if      (theString == "regular") {
    result = kMsrRegularAPIKind;
  }
  else if (theString == "strings") {
    result = kMsrStringsAPIKind;
  }
  else {
    stringstream s;

    s <<
      "the string \"" <<
      theString <<
      "\" is no valid generation API kind";

    msgAssert (false, s.str ());
  }

  return result;
}

map<string, msrGenerationAPIKind>
  gGlobalGenerationAPIKindsMap;

void initializeGenerationAPIKindsMap ()
{
  gGlobalGenerationAPIKindsMap ["regular"] = kMsrRegularAPIKind;
  gGlobalGenerationAPIKindsMap ["strings"] = kMsrStringsAPIKind;
}

string existingGenerationAPIKinds (
  unsigned int namesListMaxLength)
{
  stringstream s;

  unsigned int brailleOutputKindsMapSize =
    gGlobalGenerationAPIKindsMap.size ();

  if (brailleOutputKindsMapSize) {
    unsigned int nextToLast =
      brailleOutputKindsMapSize - 1;

    unsigned int count = 0;
    unsigned int cumulatedLength = 0;

    for (
      map<string, msrGenerationAPIKind>::const_iterator i =
        gGlobalGenerationAPIKindsMap.begin ();
      i != gGlobalGenerationAPIKindsMap.end ();
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
S_msrGenerationAPIKindAtom msrGenerationAPIKindAtom::create (
  const string&         shortName,
  const string&         longName,
  const string&         description,
  const string&         valueSpecification,
  const string&         variableName,
  msrGenerationAPIKind& generationAPIKindVariable)
{
  msrGenerationAPIKindAtom* o = new
    msrGenerationAPIKindAtom (
      shortName,
      longName,
      description,
      valueSpecification,
      variableName,
      generationAPIKindVariable);
  assert (o!=0);
  return o;
}

msrGenerationAPIKindAtom::msrGenerationAPIKindAtom (
  const string&         shortName,
  const string&         longName,
  const string&         description,
  const string&         valueSpecification,
  const string&         variableName,
  msrGenerationAPIKind& generationAPIKindVariable)
  : oahAtomWithValue (
      shortName,
      longName,
      description,
      valueSpecification,
      variableName),
    fGenerationAPIKindVariable (
      generationAPIKindVariable)
{}

msrGenerationAPIKindAtom::~msrGenerationAPIKindAtom ()
{}

void msrGenerationAPIKindAtom::applyAtomWithValue (
  const string& theString,
  ostream&      os)
{
  // theString contains the output kind name:
  // is it in the  output kinds map?

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceOah ()) {
    gLogStream <<
      "==> handling atom '" << fetchNames () << "; which is of type 'msrGenerationAPIKindAtom'" <<
      " with value \"" << theString << "\"" <<
      endl;
  }
#endif

  map<string, msrGenerationAPIKind>::const_iterator
    it =
      gGlobalGenerationAPIKindsMap.find (
        theString);

  if (it == gGlobalGenerationAPIKindsMap.end ()) {
    // no, optional values style kind is unknown in the map
    stringstream s;

    s <<
      "generation API kind '" << theString <<
      "' is unknown" <<
      endl <<
      "The " <<
      gGlobalGenerationAPIKindsMap.size () - 1 <<
      " known generation API kinds are:" <<
      endl;

    ++gIndenter;

    s <<
      existingGenerationAPIKinds (K_NAMES_LIST_MAX_LENGTH);

    --gIndenter;

    oahError (s.str ());
  }

  fGenerationAPIKindVariable =
    (*it).second;
  fVariableHasBeenSet = true;
}

void msrGenerationAPIKindAtom::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> msrGenerationAPIKindAtom::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_msrGenerationAPIKindAtom>*
    p =
      dynamic_cast<visitor<S_msrGenerationAPIKindAtom>*> (v)) {
        S_msrGenerationAPIKindAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching msrGenerationAPIKindAtom::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void msrGenerationAPIKindAtom::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> msrGenerationAPIKindAtom::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_msrGenerationAPIKindAtom>*
    p =
      dynamic_cast<visitor<S_msrGenerationAPIKindAtom>*> (v)) {
        S_msrGenerationAPIKindAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching msrGenerationAPIKindAtom::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void msrGenerationAPIKindAtom::browseData (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> msrGenerationAPIKindAtom::browseData ()" <<
      endl;
  }
#endif
}

string msrGenerationAPIKindAtom::asShortNamedOptionString () const
{
  stringstream s;

  s <<
    "-" << fShortName << " " <<
    msrGenerationAPIKindAsString (fGenerationAPIKindVariable);

  return s.str ();
}

string msrGenerationAPIKindAtom::asActualLongNamedOptionString () const
{
  stringstream s;

  s <<
    "-" << fLongName << " " <<
    msrGenerationAPIKindAsString (fGenerationAPIKindVariable);

  return s.str ();
}

void msrGenerationAPIKindAtom::print (ostream& os) const
{
  const unsigned int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "GenerationAPIKindAtom:" <<
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
    msrGenerationAPIKindAsString (
      fGenerationAPIKindVariable) <<
      "\"" <<
    endl;

  --gIndenter;
}

void msrGenerationAPIKindAtom::printAtomWithValueOptionsValues (
  ostream&     os,
  unsigned int valueFieldWidth) const
{
  os << left <<
    setw (valueFieldWidth) <<
    fVariableName <<
    " : \"" <<
    msrGenerationAPIKindAsString (
      fGenerationAPIKindVariable) <<
    "\"";
  if (fVariableHasBeenSet) {
    os <<
      ", variableHasBeenSet: " <<
      booleanAsString (fVariableHasBeenSet);
  }
  os << endl;
}

ostream& operator<< (ostream& os, const S_msrGenerationAPIKindAtom& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
void initializeMsrGenerationAPI ()
{
  // protect library against multiple initializations
  static bool pThisMethodHasBeenRun = false;

  if (! pThisMethodHasBeenRun) {
    initializeGenerationAPIKindsMap ();

    pThisMethodHasBeenRun = true;
  }
}


}
