/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#include <climits>      // INT_MIN, INT_MAX
#include <iomanip>      // setw, setprecision, ...

#include <regex>

#include "msrMidi.h"

#include "utilities.h"

#include "enableTracingIfDesired.h"
#ifdef TRACING_IS_ENABLED
  #include "traceOah.h"
#endif

#include "oahOah.h"
#include "generalOah.h"

#include "msrOah.h"
#include "lpsrOah.h"
#include "lpsr2lilypondOah.h"

#include "messagesHandling.h"


using namespace std;

namespace MusicXML2
{

//______________________________________________________________________________
S_lilypondScoreOutputKindAtom lilypondScoreOutputKindAtom::create (
  string               shortName,
  string               longName,
  string               description,
  string               valueSpecification,
  string               variableName,
  lpsrScoreOutputKind& lilypondScoreOutputKindVariable)
{
  lilypondScoreOutputKindAtom* o = new
    lilypondScoreOutputKindAtom (
      shortName,
      longName,
      description,
      valueSpecification,
      variableName,
      lilypondScoreOutputKindVariable);
  assert (o!=0);
  return o;
}

lilypondScoreOutputKindAtom::lilypondScoreOutputKindAtom (
  string               shortName,
  string               longName,
  string               description,
  string               valueSpecification,
  string               variableName,
  lpsrScoreOutputKind& lilypondScoreOutputKindVariable)
  : oahAtomWithValue (
      shortName,
      longName,
      description,
      valueSpecification,
      variableName),
    fLpsrScoreOutputKindVariable (
      lilypondScoreOutputKindVariable)
{}

lilypondScoreOutputKindAtom::~lilypondScoreOutputKindAtom ()
{}

void lilypondScoreOutputKindAtom::applyAtomWithValue (
  string   theString,
  ostream& os)
{
  // JMI ???

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceOah ()) {
    gLogStream <<
      "==> oahAtom is of type 'lilypondScoreOutputKindAtom'" <<
      endl;
  }
#endif
}

void lilypondScoreOutputKindAtom::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> lilypondScoreOutputKindAtom::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_lilypondScoreOutputKindAtom>*
    p =
      dynamic_cast<visitor<S_lilypondScoreOutputKindAtom>*> (v)) {
        S_lilypondScoreOutputKindAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching lilypondScoreOutputKindAtom::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void lilypondScoreOutputKindAtom::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> lilypondScoreOutputKindAtom::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_lilypondScoreOutputKindAtom>*
    p =
      dynamic_cast<visitor<S_lilypondScoreOutputKindAtom>*> (v)) {
        S_lilypondScoreOutputKindAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching lilypondScoreOutputKindAtom::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void lilypondScoreOutputKindAtom::browseData (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> lilypondScoreOutputKindAtom::browseData ()" <<
      endl;
  }
#endif
}

string lilypondScoreOutputKindAtom::asShortNamedOptionString () const
{
  stringstream s;

  s <<
    "-" << fShortName << " " <<
    lpsrScoreOutputKindAsString (fLpsrScoreOutputKindVariable);

  return s.str ();
}

string lilypondScoreOutputKindAtom::asActualLongNamedOptionString () const
{
  stringstream s;

  s <<
    "-" << fLongName << " " <<
    lpsrScoreOutputKindAsString (fLpsrScoreOutputKindVariable);

  return s.str ();
}

void lilypondScoreOutputKindAtom::print (ostream& os) const
{
  const unsigned int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "OptionsScoreOutputKindAtom:" <<
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
    "fLpsrScoreOutputKindVariable" << " : \"" <<
    lpsrScoreOutputKindAsString (
      fLpsrScoreOutputKindVariable) <<
    "\"" <<
    endl;

  --gIndenter;
}

void lilypondScoreOutputKindAtom::printAtomWithValueOptionsValues (
  ostream&     os,
  unsigned int valueFieldWidth) const
{
  os << left <<
    setw (valueFieldWidth) <<
    fVariableName <<
    " : \"" <<
    lpsrScoreOutputKindAsString (
      fLpsrScoreOutputKindVariable) <<
    "\"";

  if (fVariableHasBeenSet) {
    os <<
      ", variableHasBeenSet: " <<
      booleanAsString (fVariableHasBeenSet);
  }
  os << endl;
}

ostream& operator<< (ostream& os, const S_lilypondScoreOutputKindAtom& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_lilypondTransposePartNameAtom lilypondTransposePartNameAtom::create (
  string             shortName,
  string             longName,
  string             description,
  string             valueSpecification,
  string             variableName,
  map<string, S_msrSemiTonesPitchAndOctave>&
                     stringToMsrSemiTonesPitchAndOctaveMapVariable)
{
  lilypondTransposePartNameAtom* o = new
    lilypondTransposePartNameAtom (
      shortName,
      longName,
      description,
      valueSpecification,
      variableName,
      stringToMsrSemiTonesPitchAndOctaveMapVariable);
  assert (o!=0);
  return o;
}

lilypondTransposePartNameAtom::lilypondTransposePartNameAtom (
  string             shortName,
  string             longName,
  string             description,
  string             valueSpecification,
  string             variableName,
  map<string, S_msrSemiTonesPitchAndOctave>&
                     stringToMsrSemiTonesPitchAndOctaveMapVariable)
  : oahAtomWithValue (
      shortName,
      longName,
      description,
      valueSpecification,
      variableName),
    fStringToMsrSemiTonesPitchAndOctaveMapVariable (
      stringToMsrSemiTonesPitchAndOctaveMapVariable)
{}

lilypondTransposePartNameAtom::~lilypondTransposePartNameAtom ()
{}

void lilypondTransposePartNameAtom::applyAtomWithValue (
  string   theString,
  ostream& os)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceOah ()) {
    gLogStream <<
      "==> oahAtom is of type 'lilypondTransposePartNameAtom'" <<
      endl;
  }
#endif

  // theString contains the part transpose specification
  // decipher it to extract the old and new part names

  string regularExpression (
    "(.*)" // partName
    "="
    "(.*)" // destinationPitchName
    );
//    "[[:space:]]*(.*)[[:space:]]*" JMI
//    "="
//    "[[:space:]]*(.*)[[:space:]]*");

  regex  e (regularExpression);
  smatch sm;

  regex_match (theString, sm, e);

  unsigned int smSize = sm.size ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceOah ()) {
    gLogStream <<
      "There are " << smSize << " matches" <<
      " for part transpose string '" << theString <<
      "' with regex '" << regularExpression <<
      "'" <<
      endl;
  }
#endif

  if (smSize == 3) {
#ifdef TRACING_IS_ENABLED
    if (gGlobalTraceOahGroup->getTraceOah ()) {
      for (unsigned i = 0; i < smSize; ++i) {
        gLogStream <<
          "[" << sm [i] << "] ";
      } // for
      gLogStream << endl;
    }
#endif
  }

  else {
    stringstream s;

    s <<
      "-lilypond-transpose-part-name argument '" << theString <<
      "' is ill-formed";

    oahError (s.str ());
  }

  string
    partName             = sm [1],
    destinationPitchName = sm [2];

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceOah ()) {
    gLogStream <<
      "--> partName = \"" << partName << "\", " <<
      "--> destinationPitchName = \"" << destinationPitchName << "\"" <<
      endl;
  }
#endif

  // is this part name in the part renaming map?
  map<string, S_msrSemiTonesPitchAndOctave>::iterator
    it =
      fStringToMsrSemiTonesPitchAndOctaveMapVariable.find (partName);

  if (it != fStringToMsrSemiTonesPitchAndOctaveMapVariable.end ()) {
    // yes, issue error message
    stringstream s;

    s <<
      "Part name \"" << partName << "\" occurs more that once" <<
      " in the '-lilypond-transpose-part' option";

    oahError (s.str ());
  }

  else {
    fStringToMsrSemiTonesPitchAndOctaveMapVariable [partName] =
      msrSemiTonesPitchAndOctave::createFromString (
        K_NO_INPUT_LINE_NUMBER,
        destinationPitchName);

    fVariableHasBeenSet = true;
  }
}

void lilypondTransposePartNameAtom::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> lilypondTransposePartNameAtom::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_lilypondTransposePartNameAtom>*
    p =
      dynamic_cast<visitor<S_lilypondTransposePartNameAtom>*> (v)) {
        S_lilypondTransposePartNameAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching lilypondTransposePartNameAtom::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void lilypondTransposePartNameAtom::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> lilypondTransposePartNameAtom::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_lilypondTransposePartNameAtom>*
    p =
      dynamic_cast<visitor<S_lilypondTransposePartNameAtom>*> (v)) {
        S_lilypondTransposePartNameAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching lilypondTransposePartNameAtom::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void lilypondTransposePartNameAtom::browseData (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> lilypondTransposePartNameAtom::browseData ()" <<
      endl;
  }
#endif
}

string lilypondTransposePartNameAtom::asShortNamedOptionString () const
{
  stringstream s;

  if (fStringToMsrSemiTonesPitchAndOctaveMapVariable.size ()) {
    s <<
      "-" << fShortName << " \"";

    map<string, S_msrSemiTonesPitchAndOctave>::const_iterator
      iBegin = fStringToMsrSemiTonesPitchAndOctaveMapVariable.begin (),
      iEnd   = fStringToMsrSemiTonesPitchAndOctaveMapVariable.end (),
      i      = iBegin;
    for ( ; ; ) {
      s <<
        (*i).first << "=" <<
        msrSemiTonesPitchAndOctaveAsLilypondString (
          gGlobalMsrOahGroup->getMsrQuarterTonesPitchesLanguageKind (),
          (*i).second);
      if (++i == iEnd) break;
    } // for

    s << "\"";
  }

  return s.str ();
}

string lilypondTransposePartNameAtom::asActualLongNamedOptionString () const
{
  stringstream s;

  if (fStringToMsrSemiTonesPitchAndOctaveMapVariable.size ()) {
    s <<
      "-" << fShortName << " \"";

    map<string, S_msrSemiTonesPitchAndOctave>::const_iterator
      iBegin = fStringToMsrSemiTonesPitchAndOctaveMapVariable.begin (),
      iEnd   = fStringToMsrSemiTonesPitchAndOctaveMapVariable.end (),
      i      = iBegin;
    for ( ; ; ) {
      s <<
        (*i).first << "=" <<
        msrSemiTonesPitchAndOctaveAsLilypondString (
          gGlobalMsrOahGroup->getMsrQuarterTonesPitchesLanguageKind (),
          (*i).second);
      if (++i == iEnd) break;
    } // for

    s << "\"";
  }

  return s.str ();
}

void lilypondTransposePartNameAtom::print (ostream& os) const
{
  const unsigned int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "lilypondTransposePartNameAtom:" <<
    endl;

  ++gIndenter;

  printAtomWithValueEssentials (
    os, fieldWidth);

  os << left <<
    setw (fieldWidth) <<
    "fVariableName" << " : " <<
    fVariableName <<
    setw (fieldWidth) <<
    "fStringToMsrSemiTonesPitchAndOctaveMapVariable" << " : " <<
    endl;

  if (! fStringToMsrSemiTonesPitchAndOctaveMapVariable.size ()) {
    os << "none";
  }
  else {
    map<string, S_msrSemiTonesPitchAndOctave>::const_iterator
      iBegin = fStringToMsrSemiTonesPitchAndOctaveMapVariable.begin (),
      iEnd   = fStringToMsrSemiTonesPitchAndOctaveMapVariable.end (),
      i      = iBegin;
    for ( ; ; ) {
      os << (*i).first << " --> " << (*i).second;
      if (++i == iEnd) break;
      os << endl;
    } // for
  }
  os << endl;

  --gIndenter;
}

void lilypondTransposePartNameAtom::printAtomWithValueOptionsValues (
  ostream&     os,
  unsigned int valueFieldWidth) const
{
  os << left <<
    setw (valueFieldWidth) <<
    fVariableName <<
    " : ";

  if (fVariableHasBeenSet) {
    os <<
      "variableHasBeenSet: " <<
      booleanAsString (fVariableHasBeenSet);
  }

  if (! fStringToMsrSemiTonesPitchAndOctaveMapVariable.size ()) {
    os <<
      "none" <<
      endl;
  }
  else {
    os << endl;

    ++gIndenter;

    map<string, S_msrSemiTonesPitchAndOctave>::const_iterator
      iBegin = fStringToMsrSemiTonesPitchAndOctaveMapVariable.begin (),
      iEnd   = fStringToMsrSemiTonesPitchAndOctaveMapVariable.end (),
      i      = iBegin;
    for ( ; ; ) {
      os <<
        "Part name \"" <<
        (*i).first <<
        "\" --> " <<
        (*i).second <<
        endl;
      if (++i == iEnd) break;
    } // for

    --gIndenter;
  }
}

ostream& operator<< (ostream& os, const S_lilypondTransposePartNameAtom& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_lilypondTransposePartIDAtom lilypondTransposePartIDAtom::create (
  string             shortName,
  string             longName,
  string             description,
  string             valueSpecification,
  string             variableName,
  map<string, S_msrSemiTonesPitchAndOctave>&
                     stringToMsrSemiTonesPitchAndOctaveMapVariable)
{
  lilypondTransposePartIDAtom* o = new
    lilypondTransposePartIDAtom (
      shortName,
      longName,
      description,
      valueSpecification,
      variableName,
      stringToMsrSemiTonesPitchAndOctaveMapVariable);
  assert (o!=0);
  return o;
}

lilypondTransposePartIDAtom::lilypondTransposePartIDAtom (
  string             shortName,
  string             longName,
  string             description,
  string             valueSpecification,
  string             variableName,
  map<string, S_msrSemiTonesPitchAndOctave>&
                     stringToMsrSemiTonesPitchAndOctaveMapVariable)
  : oahAtomWithValue (
      shortName,
      longName,
      description,
      valueSpecification,
      variableName),
    fStringToMsrSemiTonesPitchAndOctaveMapVariable (
      stringToMsrSemiTonesPitchAndOctaveMapVariable)
{}

lilypondTransposePartIDAtom::~lilypondTransposePartIDAtom ()
{}

void lilypondTransposePartIDAtom::applyAtomWithValue (
  string   theString,
  ostream& os)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceOah ()) {
    gLogStream <<
      "==> oahAtom is of type 'lilypondTransposePartIDAtom'" <<
      endl;
  }
#endif

  // theString contains the part transpose specification
  // decipher it to extract the old and new part names

  string regularExpression (
    "(.*)" // partID
    "="
    "(.*)" // destination pitch name
    );

  regex  e (regularExpression);
  smatch sm;

  regex_match (theString, sm, e);

  unsigned int smSize = sm.size ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceOah ()) {
    gLogStream <<
      "There are " << smSize << " matches" <<
      " for part transpose string '" << theString <<
      "' with regex '" << regularExpression <<
      "'" <<
      endl;
  }
#endif

  if (smSize == 3) {
#ifdef TRACING_IS_ENABLED
    if (gGlobalTraceOahGroup->getTraceOah ()) {
      for (unsigned i = 0; i < smSize; ++i) {
        gLogStream <<
          "[" << sm [i] << "] ";
      } // for
      gLogStream << endl;
    }
#endif
  }

  else {
    stringstream s;

    s <<
      "-lilypond-transpose-part-id argument '" << theString <<
      "' is ill-formed";

    oahError (s.str ());
  }

  string
    partID               = sm [1],
    destinationPitchName = sm [2];

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceOah ()) {
    gLogStream <<
      "--> partID = \"" << partID << "\", " <<
      "--> destinationPitchName = \"" << destinationPitchName << "\"" <<
      endl;
  }
#endif

  // is this part name in the part renaming map?
  map<string, S_msrSemiTonesPitchAndOctave>::iterator
    it =
      fStringToMsrSemiTonesPitchAndOctaveMapVariable.find (partID);

  if (it != fStringToMsrSemiTonesPitchAndOctaveMapVariable.end ()) {
    // yes, issue error message
    stringstream s;

    s <<
      "Part ID \"" << partID << "\" occurs more that once" <<
      " in the '-lilypond-transpose-part' option";

    oahError (s.str ());
  }

  else {
    fStringToMsrSemiTonesPitchAndOctaveMapVariable [partID] =
      msrSemiTonesPitchAndOctave::createFromString (
        K_NO_INPUT_LINE_NUMBER,
        destinationPitchName);

    fVariableHasBeenSet = true;
  }
}

void lilypondTransposePartIDAtom::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> lilypondTransposePartIDAtom::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_lilypondTransposePartIDAtom>*
    p =
      dynamic_cast<visitor<S_lilypondTransposePartIDAtom>*> (v)) {
        S_lilypondTransposePartIDAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching lilypondTransposePartIDAtom::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void lilypondTransposePartIDAtom::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> lilypondTransposePartIDAtom::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_lilypondTransposePartIDAtom>*
    p =
      dynamic_cast<visitor<S_lilypondTransposePartIDAtom>*> (v)) {
        S_lilypondTransposePartIDAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching lilypondTransposePartIDAtom::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void lilypondTransposePartIDAtom::browseData (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> lilypondTransposePartIDAtom::browseData ()" <<
      endl;
  }
#endif
}

string lilypondTransposePartIDAtom::asShortNamedOptionString () const
{
  stringstream s;

  if (fStringToMsrSemiTonesPitchAndOctaveMapVariable.size ()) {
    s <<
      "-" << fShortName << " \"";

    map<string, S_msrSemiTonesPitchAndOctave>::const_iterator
      iBegin = fStringToMsrSemiTonesPitchAndOctaveMapVariable.begin (),
      iEnd   = fStringToMsrSemiTonesPitchAndOctaveMapVariable.end (),
      i      = iBegin;
    for ( ; ; ) {
      s <<
        (*i).first << "=" <<
        msrSemiTonesPitchAndOctaveAsLilypondString (
          gGlobalMsrOahGroup->getMsrQuarterTonesPitchesLanguageKind (),
          (*i).second);
      if (++i == iEnd) break;
    } // for

    s << "\"";
  }

  return s.str ();
}

string lilypondTransposePartIDAtom::asActualLongNamedOptionString () const
{
  stringstream s;

  if (fStringToMsrSemiTonesPitchAndOctaveMapVariable.size ()) {
    s <<
      "-" << fShortName << " \"";

    map<string, S_msrSemiTonesPitchAndOctave>::const_iterator
      iBegin = fStringToMsrSemiTonesPitchAndOctaveMapVariable.begin (),
      iEnd   = fStringToMsrSemiTonesPitchAndOctaveMapVariable.end (),
      i      = iBegin;
    for ( ; ; ) {
      s <<
        (*i).first << "=" <<
        msrSemiTonesPitchAndOctaveAsLilypondString (
          gGlobalMsrOahGroup->getMsrQuarterTonesPitchesLanguageKind (),
          (*i).second);
      if (++i == iEnd) break;
    } // for

    s << "\"";
  }

  return s.str ();
}

void lilypondTransposePartIDAtom::print (ostream& os) const
{
  const unsigned int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "lilypondTransposePartIDAtom:" <<
    endl;

  ++gIndenter;

  printAtomWithValueEssentials (
    os, fieldWidth);

  os << left <<
    setw (fieldWidth) <<
    "fVariableName" << " : " <<
    fVariableName <<
    setw (fieldWidth) <<
    "fStringToMsrSemiTonesPitchAndOctaveMapVariable" << " : " <<
    endl;

  if (! fStringToMsrSemiTonesPitchAndOctaveMapVariable.size ()) {
    os << "none";
  }
  else {
    map<string, S_msrSemiTonesPitchAndOctave>::const_iterator
      iBegin = fStringToMsrSemiTonesPitchAndOctaveMapVariable.begin (),
      iEnd   = fStringToMsrSemiTonesPitchAndOctaveMapVariable.end (),
      i      = iBegin;
    for ( ; ; ) {
      os << (*i).first << " --> " << (*i).second;
      if (++i == iEnd) break;
      os << endl;
    } // for
  }
  os << endl;

  --gIndenter;
}

void lilypondTransposePartIDAtom::printAtomWithValueOptionsValues (
  ostream&     os,
  unsigned int valueFieldWidth) const
{
  os << left <<
    setw (valueFieldWidth) <<
    fVariableName <<
    " : ";

  if (fVariableHasBeenSet) {
    os <<
      "variableHasBeenSet: " <<
      booleanAsString (fVariableHasBeenSet);
  }

  if (! fStringToMsrSemiTonesPitchAndOctaveMapVariable.size ()) {
    os <<
      "none" <<
      endl;
  }
  else {
    os << endl;

    ++gIndenter;

    map<string, S_msrSemiTonesPitchAndOctave>::const_iterator
      iBegin = fStringToMsrSemiTonesPitchAndOctaveMapVariable.begin (),
      iEnd   = fStringToMsrSemiTonesPitchAndOctaveMapVariable.end (),
      i      = iBegin;
    for ( ; ; ) {
      os <<
        "\"" <<
        (*i).first <<
        "\" --> " <<
        (*i).second <<
        endl;
      if (++i == iEnd) break;
    } // for

    --gIndenter;
  }
}

ostream& operator<< (ostream& os, const S_lilypondTransposePartIDAtom& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_lilypondAbsoluteOctaveEntryAtom lilypondAbsoluteOctaveEntryAtom::create (
  string               shortName,
  string               longName,
  string               description,
  string               variableName,
  lpsrOctaveEntryKind& lpsrOctaveEntryKindVariable)
{
  lilypondAbsoluteOctaveEntryAtom* o = new
    lilypondAbsoluteOctaveEntryAtom (
      shortName,
      longName,
      description,
      variableName,
      lpsrOctaveEntryKindVariable);
  assert (o!=0);
  return o;
}

lilypondAbsoluteOctaveEntryAtom::lilypondAbsoluteOctaveEntryAtom (
  string               shortName,
  string               longName,
  string               description,
  string               variableName,
  lpsrOctaveEntryKind& lpsrOctaveEntryKindVariable)
  : oahAtomWithVariableName (
      shortName,
      longName,
      description,
      variableName),
    fLpsrOctaveEntryKindVariable (
      lpsrOctaveEntryKindVariable)
{}

lilypondAbsoluteOctaveEntryAtom::~lilypondAbsoluteOctaveEntryAtom ()
{}

void lilypondAbsoluteOctaveEntryAtom::applyElement (ostream& os)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceOah ()) {
    gLogStream <<
      "==> option '" << fetchNames () << "' is a lilypondAbsoluteOctaveEntryAtom" <<
      endl;
  }
#endif

  // set octave entry kind
  fLpsrOctaveEntryKindVariable = kOctaveEntryAbsolute;
}

void lilypondAbsoluteOctaveEntryAtom::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> lilypondAbsoluteOctaveEntryAtom::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_lilypondAbsoluteOctaveEntryAtom>*
    p =
      dynamic_cast<visitor<S_lilypondAbsoluteOctaveEntryAtom>*> (v)) {
        S_lilypondAbsoluteOctaveEntryAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching lilypondAbsoluteOctaveEntryAtom::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void lilypondAbsoluteOctaveEntryAtom::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> lilypondAbsoluteOctaveEntryAtom::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_lilypondAbsoluteOctaveEntryAtom>*
    p =
      dynamic_cast<visitor<S_lilypondAbsoluteOctaveEntryAtom>*> (v)) {
        S_lilypondAbsoluteOctaveEntryAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching lilypondAbsoluteOctaveEntryAtom::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void lilypondAbsoluteOctaveEntryAtom::browseData (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> lilypondAbsoluteOctaveEntryAtom::browseData ()" <<
      endl;
  }
#endif
}

void lilypondAbsoluteOctaveEntryAtom::print (ostream& os) const
{
  const unsigned int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "lilypondAbsoluteOctaveEntryAtom:" <<
    endl;

  ++gIndenter;

  oahElement::printOahElementEssentials (
    os, fieldWidth);

  os << left <<
    setw (fieldWidth) <<
    "lpsrOctaveEntryKindVariable" << " : " <<
    "\"" <<
    lpsrOctaveEntryKindAsString (
      fLpsrOctaveEntryKindVariable) <<
    "\"" <<
    endl;

  --gIndenter;
}

void lilypondAbsoluteOctaveEntryAtom::printAtomWithValueOptionsValues (
  ostream&     os,
  unsigned int valueFieldWidth) const
{
  os << left <<
    setw (valueFieldWidth) <<
    fVariableName <<
    " : " <<
    "- no possible value here -" <<
    endl;
}

//______________________________________________________________________________
S_lilypondRelativeOctaveEntryAtom lilypondRelativeOctaveEntryAtom::create (
  string                        shortName,
  string                        longName,
  string                        description,
  string                        valueSpecification,
  string                        variableName,
  lpsrOctaveEntryKind&          lpsrOctaveEntryKindVariable,
  S_msrSemiTonesPitchAndOctave& lilypondRelativeOctaveEntryVariable)
{
  lilypondRelativeOctaveEntryAtom* o = new
    lilypondRelativeOctaveEntryAtom (
      shortName,
      longName,
      description,
      valueSpecification,
      variableName,
      lpsrOctaveEntryKindVariable,
      lilypondRelativeOctaveEntryVariable);
  assert (o!=0);
  return o;
}

lilypondRelativeOctaveEntryAtom::lilypondRelativeOctaveEntryAtom (
  string                        shortName,
  string                        longName,
  string                        description,
  string                        valueSpecification,
  string                        variableName,
  lpsrOctaveEntryKind&          lpsrOctaveEntryKindVariable,
  S_msrSemiTonesPitchAndOctave& lilypondRelativeOctaveEntryVariable)
  : oahAtomWithValue (
      shortName,
      longName,
      description,
      valueSpecification,
      variableName),
    fLpsrOctaveEntryKindVariable (
      lpsrOctaveEntryKindVariable),
    fSemiTonesPitchAndOctaveVariable (
      lilypondRelativeOctaveEntryVariable)
{}

lilypondRelativeOctaveEntryAtom::~lilypondRelativeOctaveEntryAtom ()
{}

void lilypondRelativeOctaveEntryAtom::applyAtomWithValue (
  string   theString,
  ostream& os)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceOah ()) {
    gLogStream <<
      "==> oahAtom is of type 'lilypondRelativeOctaveEntryAtom'" <<
      endl;
  }
#endif

  // theString contains the score output kind:
  // is it in the score output kinds map?

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceOah ()) {
    gLogStream <<
      "==> oahAtom is of type 'lilypondRelativeOctaveEntryAtom'" <<
      endl;
  }
#endif

/* JMI
  map<string, lpsrOctaveEntryKind>::const_iterator
    it =
      gGlobalLpsrOctaveEntryKindsMap.find (
        theString);

  if (it == gGlobalLpsrOctaveEntryKindsMap.end ()) {
    // no, octave entry kind is unknown in the map

    printOptionsSummary (gLogStream);

    stringstream s;

    s <<
      "octave entry kind '" << theString <<
      "' is unknown" <<
      endl <<
      "The " <<
      gGlobalLpsrOctaveEntryKindsMap.size () <<
      " known octave entry kinds are:" <<
      endl;

    ++gIndenter;

    s <<
      existingLpsrOctaveEntryKinds ();

    --gIndenter;

    oahError (s.str ());
  }

  if (! theString.size ()) {
    oahError ("'the '-rel, relative' option needs a reference pitch and octave");
  }
*/

  setRelativeOctaveEntryVariableValue (
    msrSemiTonesPitchAndOctave::createFromString (
      K_NO_INPUT_LINE_NUMBER,
      theString));

  fVariableHasBeenSet = true;
}

void lilypondRelativeOctaveEntryAtom::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> lilypondRelativeOctaveEntryAtom::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_lilypondRelativeOctaveEntryAtom>*
    p =
      dynamic_cast<visitor<S_lilypondRelativeOctaveEntryAtom>*> (v)) {
        S_lilypondRelativeOctaveEntryAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching lilypondRelativeOctaveEntryAtom::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void lilypondRelativeOctaveEntryAtom::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> lilypondRelativeOctaveEntryAtom::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_lilypondRelativeOctaveEntryAtom>*
    p =
      dynamic_cast<visitor<S_lilypondRelativeOctaveEntryAtom>*> (v)) {
        S_lilypondRelativeOctaveEntryAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching lilypondRelativeOctaveEntryAtom::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void lilypondRelativeOctaveEntryAtom::browseData (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> lilypondRelativeOctaveEntryAtom::browseData ()" <<
      endl;
  }
#endif
}

string lilypondRelativeOctaveEntryAtom::asShortNamedOptionString () const
{
  stringstream s;

  s <<
    "-" << fShortName << " " <<
    fSemiTonesPitchAndOctaveVariable->asString ();

  return s.str ();
}

string lilypondRelativeOctaveEntryAtom::asActualLongNamedOptionString () const
{
  stringstream s;

  s <<
    "-" << fLongName << " " <<
    fSemiTonesPitchAndOctaveVariable->asString ();

  return s.str ();
}

void lilypondRelativeOctaveEntryAtom::print (ostream& os) const
{
  const unsigned int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "lilypondRelativeOctaveEntryAtom:" <<
    endl;

  ++gIndenter;

  oahElement::printOahElementEssentials (
    os, fieldWidth);

  os << left <<
    setw (fieldWidth) <<
    "lpsrOctaveEntryKindVariable" << " : " <<
    "\"" <<
    lpsrOctaveEntryKindAsString (
      fLpsrOctaveEntryKindVariable) <<
    "\"" <<
    endl;

  --gIndenter;
}

void lilypondRelativeOctaveEntryAtom::printAtomWithValueOptionsValues (
  ostream&     os,
  unsigned int valueFieldWidth) const
{
  os << left <<
    setw (valueFieldWidth) <<
    fVariableName <<
    " : ";

  if (fVariableHasBeenSet) {
    os <<
      "variableHasBeenSet: " <<
      booleanAsString (fVariableHasBeenSet);
  }

  if (fSemiTonesPitchAndOctaveVariable) {
    os << endl;
    ++gIndenter;
    os <<
      fSemiTonesPitchAndOctaveVariable;

    --gIndenter;
  }
  else {
    os << "none" << endl;
  }
}

ostream& operator<< (ostream& os, const S_lilypondRelativeOctaveEntryAtom& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_lilypondFixedOctaveEntryAtom lilypondFixedOctaveEntryAtom::create (
  string                        shortName,
  string                        longName,
  string                        description,
  string                        valueSpecification,
  string                        variableName,
  lpsrOctaveEntryKind&          lpsrOctaveEntryKindVariable,
  S_msrSemiTonesPitchAndOctave& lilypondFixedOctaveEntryVariable)
{
  lilypondFixedOctaveEntryAtom* o = new
    lilypondFixedOctaveEntryAtom (
      shortName,
      longName,
      description,
      valueSpecification,
      variableName,
      lpsrOctaveEntryKindVariable,
      lilypondFixedOctaveEntryVariable);
  assert (o!=0);
  return o;
}

lilypondFixedOctaveEntryAtom::lilypondFixedOctaveEntryAtom (
  string                        shortName,
  string                        longName,
  string                        description,
  string                        valueSpecification,
  string                        variableName,
  lpsrOctaveEntryKind&          lpsrOctaveEntryKindVariable,
  S_msrSemiTonesPitchAndOctave& lilypondFixedOctaveEntryVariable)
  : oahAtomWithValue (
      shortName,
      longName,
      description,
      valueSpecification,
      variableName),
    fLpsrOctaveEntryKindVariable (
      lpsrOctaveEntryKindVariable),
    fSemiTonesPitchAndOctaveVariable (
      lilypondFixedOctaveEntryVariable)
{}

lilypondFixedOctaveEntryAtom::~lilypondFixedOctaveEntryAtom ()
{}

void lilypondFixedOctaveEntryAtom::applyAtomWithValue (
  string   theString,
  ostream& os)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceOah ()) {
    gLogStream <<
      "==> oahAtom is of type 'lilypondFixedOctaveEntryAtom'" <<
      endl;
  }
#endif

  // theString contains the score output kind:
  // is it in the score output kinds map?

  setFixedOctaveEntryVariableValue (
    msrSemiTonesPitchAndOctave::createFromString (
      K_NO_INPUT_LINE_NUMBER,
      theString));

  fVariableHasBeenSet = true;
}

void lilypondFixedOctaveEntryAtom::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> lilypondFixedOctaveEntryAtom::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_lilypondFixedOctaveEntryAtom>*
    p =
      dynamic_cast<visitor<S_lilypondFixedOctaveEntryAtom>*> (v)) {
        S_lilypondFixedOctaveEntryAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching lilypondFixedOctaveEntryAtom::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void lilypondFixedOctaveEntryAtom::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> lilypondFixedOctaveEntryAtom::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_lilypondFixedOctaveEntryAtom>*
    p =
      dynamic_cast<visitor<S_lilypondFixedOctaveEntryAtom>*> (v)) {
        S_lilypondFixedOctaveEntryAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching lilypondFixedOctaveEntryAtom::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void lilypondFixedOctaveEntryAtom::browseData (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> lilypondFixedOctaveEntryAtom::browseData ()" <<
      endl;
  }
#endif
}

string lilypondFixedOctaveEntryAtom::asShortNamedOptionString () const
{
  stringstream s;

  s <<
    "-" << fShortName << " " <<
    fSemiTonesPitchAndOctaveVariable->asString ();

  return s.str ();
}

string lilypondFixedOctaveEntryAtom::asActualLongNamedOptionString () const
{
  stringstream s;

  s <<
    "-" << fLongName << " " <<
    fSemiTonesPitchAndOctaveVariable->asString ();

  return s.str ();
}

void lilypondFixedOctaveEntryAtom::print (ostream& os) const
{
  const unsigned int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "lilypondFixedOctaveEntryAtom:" <<
    endl;

  ++gIndenter;

  oahElement::printOahElementEssentials (
    os, fieldWidth);

  os << left <<
    setw (fieldWidth) <<
    "lpsrOctaveEntryKindVariable" << " : " <<
    "\"" <<
    lpsrOctaveEntryKindAsString (
      fLpsrOctaveEntryKindVariable) <<
    "\"" <<
    endl;

  --gIndenter;
}

void lilypondFixedOctaveEntryAtom::printAtomWithValueOptionsValues (
  ostream&     os,
  unsigned int valueFieldWidth) const
{
  os << left <<
    setw (valueFieldWidth) <<
    fVariableName <<
    " : ";

  if (fVariableHasBeenSet) {
    os <<
      "variableHasBeenSet: " <<
      booleanAsString (fVariableHasBeenSet);
  }

  if (fSemiTonesPitchAndOctaveVariable) {
    os << endl;
    ++gIndenter;
    os <<
      fSemiTonesPitchAndOctaveVariable;

    --gIndenter;
  }
  else {
    os << "none" << endl;
  }
}

ostream& operator<< (ostream& os, const S_lilypondFixedOctaveEntryAtom& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_lilypondAccidentalStyleKindAtom lilypondAccidentalStyleKindAtom::create (
  string             shortName,
  string             longName,
  string             description,
  string             valueSpecification,
  string             variableName,
  lpsrAccidentalStyleKind&
                     lilypondAccidentalStyleKindVariable)
{
  lilypondAccidentalStyleKindAtom* o = new
    lilypondAccidentalStyleKindAtom (
      shortName,
      longName,
      description,
      valueSpecification,
      variableName,
      lilypondAccidentalStyleKindVariable);
  assert (o!=0);
  return o;
}

lilypondAccidentalStyleKindAtom::lilypondAccidentalStyleKindAtom (
  string             shortName,
  string             longName,
  string             description,
  string             valueSpecification,
  string             variableName,
  lpsrAccidentalStyleKind&
                     lilypondAccidentalStyleKindVariable)
  : oahAtomWithValue (
      shortName,
      longName,
      description,
      valueSpecification,
      variableName),
    fLpsrAccidentalStyleKindVariable (
      lilypondAccidentalStyleKindVariable)
{}

lilypondAccidentalStyleKindAtom::~lilypondAccidentalStyleKindAtom ()
{}

void lilypondAccidentalStyleKindAtom::applyAtomWithValue (
  string   theString,
  ostream& os)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceOah ()) {
    gLogStream <<
      "==> oahAtom is of type 'lilypondAccidentalStyleKindAtom'" <<
      endl;
  }
#endif

  // theString contains the language name:
  // is it in the accidental styles map?

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceOah ()) {
    gLogStream <<
      "==> oahAtom is of type 'lilypondAccidentalStyleKindAtom'" <<
      endl;
  }
#endif

  map<string, lpsrAccidentalStyleKind>::const_iterator
    it =
      gGlobalLpsrAccidentalStyleKindsMap.find (
        theString);

  if (it == gGlobalLpsrAccidentalStyleKindsMap.end ()) {
    // no, accidental style is unknown in the map
    stringstream s;

    s <<
      "LPSR accidental style '" << theString <<
      "' is unknown" <<
      endl <<
      "The " <<
      gGlobalLpsrAccidentalStyleKindsMap.size () - 1 <<
      " known LPSR accidental styles are:" <<
      endl;

    ++gIndenter;

    s <<
      existingLpsrAccidentalStyleKinds (K_NAMES_LIST_MAX_LENGTH);

    --gIndenter;

    oahError (s.str ());
  }

  setLpsrAccidentalStyleKindVariable (
    (*it).second);
}

void lilypondAccidentalStyleKindAtom::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> lilypondAccidentalStyleKindAtom::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_lilypondAccidentalStyleKindAtom>*
    p =
      dynamic_cast<visitor<S_lilypondAccidentalStyleKindAtom>*> (v)) {
        S_lilypondAccidentalStyleKindAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching lilypondAccidentalStyleKindAtom::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void lilypondAccidentalStyleKindAtom::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> lilypondAccidentalStyleKindAtom::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_lilypondAccidentalStyleKindAtom>*
    p =
      dynamic_cast<visitor<S_lilypondAccidentalStyleKindAtom>*> (v)) {
        S_lilypondAccidentalStyleKindAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching lilypondAccidentalStyleKindAtom::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void lilypondAccidentalStyleKindAtom::browseData (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> lilypondAccidentalStyleKindAtom::browseData ()" <<
      endl;
  }
#endif
}

string lilypondAccidentalStyleKindAtom::asShortNamedOptionString () const
{
  stringstream s;

  s <<
    "-" << fShortName << " " <<
    lpsrAccidentalStyleKindAsString (fLpsrAccidentalStyleKindVariable);

  return s.str ();
}

string lilypondAccidentalStyleKindAtom::asActualLongNamedOptionString () const
{
  stringstream s;

  s <<
    "-" << fLongName << " " <<
    lpsrAccidentalStyleKindAsString (fLpsrAccidentalStyleKindVariable);

  return s.str ();
}

void lilypondAccidentalStyleKindAtom::print (ostream& os) const
{
  const unsigned int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "OptionsAccidentalStyleKindAtom:" <<
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
    "fLpsrAccidentalStyleKindVariable" << " : \"" <<
    lpsrAccidentalStyleKindAsString (
      fLpsrAccidentalStyleKindVariable) <<
    "\"" <<
    endl;

  --gIndenter;
}

void lilypondAccidentalStyleKindAtom::printAtomWithValueOptionsValues (
  ostream&     os,
  unsigned int valueFieldWidth) const
{
  os << left <<
    setw (valueFieldWidth) <<
    fVariableName <<
    " : \"" <<
    lpsrAccidentalStyleKindAsString (
      fLpsrAccidentalStyleKindVariable) <<
    "\"";

  if (fVariableHasBeenSet) {
    os <<
      ", variableHasBeenSet: " <<
      booleanAsString (fVariableHasBeenSet);
  }
  os << endl;
}

ostream& operator<< (ostream& os, const S_lilypondAccidentalStyleKindAtom& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_lilypondChordsDisplayAtom lilypondChordsDisplayAtom::create (
  string                shortName,
  string                longName,
  string                description,
  string                valueSpecification,
  string                variableName,
  list<pair<string, string> >&
                        lilypondChordsDisplayVariable)
{
  lilypondChordsDisplayAtom* o = new
    lilypondChordsDisplayAtom (
      shortName,
      longName,
      description,
      valueSpecification,
      variableName,
      lilypondChordsDisplayVariable);
  assert (o!=0);
  return o;
}

lilypondChordsDisplayAtom::lilypondChordsDisplayAtom (
  string                shortName,
  string                longName,
  string                description,
  string                valueSpecification,
  string                variableName,
  list<pair<string, string> >&
                        lilypondChordsDisplayVariable)
  : oahAtomWithValue (
      shortName,
      longName,
      description,
      valueSpecification,
      variableName),
    fStringsPairListVariable (
      lilypondChordsDisplayVariable)
{}

lilypondChordsDisplayAtom::~lilypondChordsDisplayAtom ()
{}

void lilypondChordsDisplayAtom::applyAtomWithValue (
  string   theString,
  ostream& os)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceOah ()) {
    gLogStream <<
      "==> oahAtom is of type 'lilypondChordsDisplayAtom'" <<
      endl;
  }
#endif

  /*
    % Exception music is chords with markups
    #(define (lower-extension pitch chbass)
       "Return lowered markup for pitch note name."
       #{
         \markup \raise #-1.9 \halign #0.2
         #(note-name->markup pitch chbass)
       #})

    chExceptionMusic = {
      <c ees ges bes>1-\markup { \super {"-7(" {\small\raise #0.5 \flat} "5)"} }
      <c e g bes>1-\markup { \super "7" }
      <c e gis bes>1-\markup { \super {"7(" {\small\raise #0.5 \sharp} "5)"} }
      <c f g bes>1-\markup { \super {"7(sus4)"} }
      <c e g a d'>1-\markup { \super "6/9" }
      <c e g bes des'>1-\markup { \super {"7(" {\small\raise #0.5 \flat} "9)"} }
      <c f g bes d'>1-\markup { \super {"9(sus4)"} }
      <c e g bes d'>1-\markup { \super "9" }
      <c e g b d'>1-\markup { \super "maj9" }
      <c e gis bes d'>1-\markup { \super "9+" }
      <c e g bes d' fis'>1-\markup { \super "9#11" }
      <c e g bes d' f'>1-\markup { \super "11" }
      <c e g bes d' a'>1-\markup { \super "13" }
      <c e g bes d' fis' a'>1-\markup { \super {"13(" {\small\raise #0.5 \sharp} "11)"} }
      <c e g a d'>1-\markup { \super "6(add9)" }
    }

    chExceptions = #(append
                     (sequential-music-to-chord-exceptions chExceptionMusic #t)
                     ignatzekExceptions)

    -7b5:
      -cd, -chord-display '<c ees ges bes> \super {"-7(" {\small\raise #0.5 \flat} "5)"}'
  */

  // theString contains a specification such as:
  //   '<c ees ges bes> \super {"-7(" {\small\raise #0.5 \flat} "5)"}'

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceOah ()) {
    gLogStream <<
      "==> oahAtom is of type 'lilypondChordsDisplayAtom'" <<
      endl;
  }
#endif

  string regularExpression (
    "(<.+>)"
    "[[:space:]]+"
    "(.+)"
    );

  regex  e (regularExpression);
  smatch sm;

  regex_match (theString, sm, e);

  unsigned int smSize = sm.size ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceOah ()) {
    gLogStream <<
      "There are " << smSize << " matches" <<
      " for chords display string '" << theString <<
      "' with regex '" << regularExpression <<
      "':" <<
      endl;

    ++gIndenter;

    for (unsigned i = 0; i < smSize; ++i) {
      gLogStream <<
        i << ": " << "\"" << sm [i] << "\"" <<
        endl;
    } // for
    gLogStream << endl;

    --gIndenter;
  }
#endif

  if (smSize != 3) {
    stringstream s;

    s <<
      "-chords-display argument '" << theString <<
      "' is ill-formed";

    oahError (s.str ());
  }

  string chordContents     = sm [1];
  string chordsDisplaycode = sm [2];

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceOah ()) {
    gLogStream <<
      "FAA chordContents  = " <<
      chordContents <<
      endl <<
      "chordsDisplaycode = " <<
      chordsDisplaycode <<
      endl;

  setStringsPairListVariable (
    make_pair (
      chordContents, chordsDisplaycode));
  }
#endif
}

void lilypondChordsDisplayAtom::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> lilypondChordsDisplayAtom::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_lilypondChordsDisplayAtom>*
    p =
      dynamic_cast<visitor<S_lilypondChordsDisplayAtom>*> (v)) {
        S_lilypondChordsDisplayAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching lilypondChordsDisplayAtom::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void lilypondChordsDisplayAtom::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> lilypondChordsDisplayAtom::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_lilypondChordsDisplayAtom>*
    p =
      dynamic_cast<visitor<S_lilypondChordsDisplayAtom>*> (v)) {
        S_lilypondChordsDisplayAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching lilypondChordsDisplayAtom::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void lilypondChordsDisplayAtom::browseData (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> lilypondChordsDisplayAtom::browseData ()" <<
      endl;
  }
#endif
}

string lilypondChordsDisplayAtom::asShortNamedOptionString () const
{
  stringstream s;

  s <<
    "-" << fShortName << " ";

  list<pair<string, string> >::const_iterator
    iBegin = fStringsPairListVariable.begin (),
    iEnd   = fStringsPairListVariable.end (),
    i      = iBegin;
  for ( ; ; ) {
    s <<
      (*i).first << " --> " << (*i).second <<
      endl;
    if (++i == iEnd) break;
    s << ","; // JMI
  } // for

  return s.str ();
}

string lilypondChordsDisplayAtom::asActualLongNamedOptionString () const
{
  stringstream s;

  s <<
    "-" << fLongName << " ";

  list<pair<string, string> >::const_iterator
    iBegin = fStringsPairListVariable.begin (),
    iEnd   = fStringsPairListVariable.end (),
    i      = iBegin;
  for ( ; ; ) {
    s <<
      (*i).first << " --> " << (*i).second <<
      endl;
    if (++i == iEnd) break;
    s << ","; // JMI
  } // for

  return s.str ();
}

void lilypondChordsDisplayAtom::print (ostream& os) const
{
  const unsigned int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "lilypondChordsDisplayAtom:" <<
    endl;

  ++gIndenter;

  printAtomWithValueEssentials (
    os, fieldWidth);

  os << left <<
    setw (fieldWidth) <<
    "fVariableName" << " : " <<
    fVariableName <<
    setw (fieldWidth) <<
    "fStringsPairListVariable" << " : '" <<
    endl;

  if (fStringsPairListVariable.size ()) {
    ++gIndenter;

    list<pair<string, string> >::const_iterator
      iBegin = fStringsPairListVariable.begin (),
      iEnd   = fStringsPairListVariable.end (),
      i      = iBegin;
    for ( ; ; ) {
      os <<
        (*i).first << " --> " << (*i).second <<
        endl;
      if (++i == iEnd) break;
  // JMI    os << endl;
    } // for

    --gIndenter;
  }

  --gIndenter;
}

void lilypondChordsDisplayAtom::printAtomWithValueOptionsValues (
  ostream&     os,
  unsigned int valueFieldWidth) const
{
  os << left <<
    setw (valueFieldWidth) <<
    fVariableName <<
    " : ";

  if (fStringsPairListVariable.size ()) {
    os << endl;
    ++gIndenter;

    list<pair<string, string> >::const_iterator
      iBegin = fStringsPairListVariable.begin (),
      iEnd   = fStringsPairListVariable.end (),
      i      = iBegin;
    for ( ; ; ) {
      os << (*i).first << " --> " << (*i).second;
      if (++i == iEnd) break;
      os << endl;
    } // for

    if (fVariableHasBeenSet) {
      os <<
        "variableHasBeenSet: " <<
        booleanAsString (fVariableHasBeenSet) <<
        endl;
    }

    --gIndenter;
  }
  else {
    os <<
      "none";
  }
  os << endl;
}

ostream& operator<< (ostream& os, const S_lilypondChordsDisplayAtom& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_lilypondLyricsDurationsKindAtom lilypondLyricsDurationsKindAtom::create (
  string             shortName,
  string             longName,
  string             description,
  string             valueSpecification,
  string             variableName,
  lpsrLyricsDurationsKind&
                     lilypondLyricsDurationsKindVariable)
{
  lilypondLyricsDurationsKindAtom* o = new
    lilypondLyricsDurationsKindAtom (
      shortName,
      longName,
      description,
      valueSpecification,
      variableName,
      lilypondLyricsDurationsKindVariable);
  assert (o!=0);
  return o;
}

lilypondLyricsDurationsKindAtom::lilypondLyricsDurationsKindAtom (
  string             shortName,
  string             longName,
  string             description,
  string             valueSpecification,
  string             variableName,
  lpsrLyricsDurationsKind&
                     lilypondLyricsDurationsKindVariable)
  : oahAtomWithValue (
      shortName,
      longName,
      description,
      valueSpecification,
      variableName),
    fLpsrLyricsDurationsKindVariable (
      lilypondLyricsDurationsKindVariable)
{}

lilypondLyricsDurationsKindAtom::~lilypondLyricsDurationsKindAtom ()
{}

void lilypondLyricsDurationsKindAtom::applyAtomWithValue (
  string   theString,
  ostream& os)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceOah ()) {
    gLogStream <<
      "==> oahAtom is of type 'lilypondLyricsDurationsKindAtom'" <<
      endl;
  }
#endif

  // theString contains the language name:
  // is it in the lyrics alignment kinds map?

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceOah ()) {
    gLogStream <<
      "==> oahAtom is of type 'lilypondLyricsDurationsKindAtom'" <<
      endl;
  }
#endif

  map<string, lpsrLyricsDurationsKind>::const_iterator
    it =
      gGlobalLpsrLyricsDurationsKindsMap.find (
        theString);

  if (it == gGlobalLpsrLyricsDurationsKindsMap.end ()) {
    // no, lyrics alignment kind is unknown in the map
    stringstream s;

    s <<
      "LPSR lyrics alignment kind '" << theString <<
      "' is unknown" <<
      endl <<
      "The " <<
      gGlobalLpsrLyricsDurationsKindsMap.size () - 1 <<
      " known LPSR lyrics alignment kind are:" <<
      endl;

    ++gIndenter;

    s <<
      existingLpsrLyricsDurationsKinds (K_NAMES_LIST_MAX_LENGTH);

    --gIndenter;

    oahError (s.str ());
  }

  setLpsrLyricsDurationsKindVariable (
    (*it).second);
}

void lilypondLyricsDurationsKindAtom::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> lilypondLyricsDurationsKindAtom::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_lilypondLyricsDurationsKindAtom>*
    p =
      dynamic_cast<visitor<S_lilypondLyricsDurationsKindAtom>*> (v)) {
        S_lilypondLyricsDurationsKindAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching lilypondLyricsDurationsKindAtom::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void lilypondLyricsDurationsKindAtom::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> lilypondLyricsDurationsKindAtom::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_lilypondLyricsDurationsKindAtom>*
    p =
      dynamic_cast<visitor<S_lilypondLyricsDurationsKindAtom>*> (v)) {
        S_lilypondLyricsDurationsKindAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching lilypondLyricsDurationsKindAtom::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void lilypondLyricsDurationsKindAtom::browseData (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> lilypondLyricsDurationsKindAtom::browseData ()" <<
      endl;
  }
#endif
}

string lilypondLyricsDurationsKindAtom::asShortNamedOptionString () const
{
  stringstream s;

  s <<
    "-" << fShortName << " " <<
    lpsrLyricsDurationsKindAsString (fLpsrLyricsDurationsKindVariable);

  return s.str ();
}

string lilypondLyricsDurationsKindAtom::asActualLongNamedOptionString () const
{
  stringstream s;

  s <<
    "-" << fLongName << " " <<
    lpsrLyricsDurationsKindAsString (fLpsrLyricsDurationsKindVariable);

  return s.str ();
}

void lilypondLyricsDurationsKindAtom::print (ostream& os) const
{
  const unsigned int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "OptionsLyricsDurationsKindAtom:" <<
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
    "fLpsrLyricsDurationsKindVariable" << " : \"" <<
    lpsrLyricsDurationsKindAsString (
      fLpsrLyricsDurationsKindVariable) <<
    "\"" <<
    endl;

  --gIndenter;
}

void lilypondLyricsDurationsKindAtom::printAtomWithValueOptionsValues (
  ostream&     os,
  unsigned int valueFieldWidth) const
{
  os << left <<
    setw (valueFieldWidth) <<
    fVariableName <<
    " : \"" <<
    lpsrLyricsDurationsKindAsString (
      fLpsrLyricsDurationsKindVariable) <<
    "\"";

  if (fVariableHasBeenSet) {
    os <<
      ", variableHasBeenSet: " <<
      booleanAsString (fVariableHasBeenSet);

  }
  os << endl;
}

ostream& operator<< (ostream& os, const S_lilypondLyricsDurationsKindAtom& elt)
{
  elt->print (os);
  return os;
}

//_______________________________________________________________________________
S_lpsr2lilypondOahGroup gGlobalLpsr2lilypondOahGroup;

S_lpsr2lilypondOahGroup lpsr2lilypondOahGroup::create ()
{
  lpsr2lilypondOahGroup* o = new lpsr2lilypondOahGroup ();
  assert (o!=0);
  return o;
}

lpsr2lilypondOahGroup::lpsr2lilypondOahGroup ()
  : oahGroup (
      "Lpsr2lilyPond",
      "hlp2lily", "help-lpsr-to-lilypond",
  R"(These options control which LilyPond code is generated.)",
      kElementVisibilityWhole)
{
  initializeLpsr2lilypondOahGroup ();
}

lpsr2lilypondOahGroup::~lpsr2lilypondOahGroup ()
{}

void lpsr2lilypondOahGroup::initializeLilypondVersionOptions ()
{
  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "LilyPond version",
        "hlpv", "help-lilypond-version",
R"()",
      kElementVisibilityWhole,
      this);

  appendSubGroupToGroup (subGroup);

  // LilyPond version

  fLilypondVersionDefaultValue = "2.19.83";

  fLilypondVersion = fLilypondVersionDefaultValue;

  fLilypondVersionAtom =
    oahStringAtom::create (
      "lpv", "lilypond-version",
      regex_replace (
R"(Set the Lilypond '\version' to STRING in the Lilypond code.
The default is 'DEFAULT_VALUE'.)",
        regex ("DEFAULT_VALUE"),
        fLilypondVersionDefaultValue),
      "STRING",
      "lilyPondVersion",
      fLilypondVersion);

  subGroup->
    appendAtomToSubGroup (
      fLilypondVersionAtom);
}

void lpsr2lilypondOahGroup::initializeGlobalStaffSizeOptions ()
{
  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Global staff size",
        "hgss", "help-global-staff-size",
R"()",
      kElementVisibilityWhole,
      this);

  appendSubGroupToGroup (subGroup);

  // global staff size

  fStaffGlobalSizeDefaultValue = 20; // LilyPond default

  fGlobalStaffSize = fStaffGlobalSizeDefaultValue;

  fGlobalStaffSizeAtom =
    oahFloatAtom::create (
      "gss", "global-staff-size",
      regex_replace (
R"(Set the LilyPond '#(set-global-staff-size ...)' to FLOAT in the LilyPond code.
FLOAT should be a floating point or integer number.
The default is 'DEFAULT_VALUE'.)",
        regex ("DEFAULT_VALUE"),
        to_string (fStaffGlobalSizeDefaultValue)),
      "FLOAT",
      "globalStaffSize",
      fGlobalStaffSize);

  subGroup->
    appendAtomToSubGroup (
      fGlobalStaffSizeAtom);
}

void lpsr2lilypondOahGroup::initializeIdentificationOptions ()
{
  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Identification",
        "hlpi", "help-lilypond-identification",
R"(These options can be used to enforce values in the generated LilyPond code,
thus overriding the ones that may be present in the MSR data.)",
      kElementVisibilityWhole,
      this);

  appendSubGroupToGroup (subGroup);

  // the 'identification' monoplex string atom
  // --------------------------------------

  S_oahMonoplexStringAtom
    identificationMonoplexStringAtom =
      oahMonoplexStringAtom::create (
        "svin", "set-variable-in-header",
        "Set variable HEADER_VARIABLE in the \\header in the generated LilyPond code.",
        "HEADER_VARIABLE",
        "STRING");

  subGroup->
    appendAtomToSubGroup (
      identificationMonoplexStringAtom);

  // MusicXML informations
  // --------------------------------------

  S_oahStringAtom
    rightsAtom =
      oahStringAtom::create (
        "rights", "",
R"(Set the 'rights' to STRING in the LilyPond code \header.)",
        "STRING",
        "rights",
        fRights);
  subGroup->
    appendAtomToSubGroup (
      rightsAtom);
  identificationMonoplexStringAtom->
    addStringAtom (
      rightsAtom);

  S_oahStringAtom
    composerAtom =
      oahStringAtom::create (
        "composer", "",
R"(Set the 'composer' to STRING in the LilyPond code \header.)",
        "STRING",
        "composer",
        fComposer);
  subGroup->
    appendAtomToSubGroup (
      composerAtom);
  identificationMonoplexStringAtom->
    addStringAtom (
      composerAtom);

  S_oahStringAtom
    arrangerAtom =
      oahStringAtom::create (
        "arranger", "",
R"(Set the 'arranger' to STRING in the LilyPond code \header.)",
        "STRING",
        "arranger",
        fArranger);
  subGroup->
    appendAtomToSubGroup (
      arrangerAtom);
  identificationMonoplexStringAtom->
    addStringAtom (
      arrangerAtom);

  fPoetAtom =
    oahStringAtom::create (
      "poet", "",
R"(Set the 'poet' to STRING in the LilyPond code \header.)",
      "STRING",
      "poet",
      fPoet);
  subGroup->
    appendAtomToSubGroup (
      fPoetAtom);
  identificationMonoplexStringAtom->
    addStringAtom (
      fPoetAtom);

  S_oahStringAtom
    lyricistAtom =
      oahStringAtom::create (
        "lyricist", "",
R"(Set the 'lyricist' to STRING in the LilyPond code \header.)",
        "STRING",
        "lyricist",
        fLyricist);
  subGroup->
    appendAtomToSubGroup (
      lyricistAtom);
  identificationMonoplexStringAtom->
    addStringAtom (
      lyricistAtom);

  S_oahStringAtom
    softwareAtom =
      oahStringAtom::create (
        "software", "",
R"(Set the 'software' to STRING in the LilyPond code \header.)",
        "STRING",
        "software",
        fSoftware);
  subGroup->
    appendAtomToSubGroup (
      softwareAtom);
  identificationMonoplexStringAtom->
    addStringAtom (
      softwareAtom);

  // LilyPond informations

  S_oahStringAtom
    dedicationAtom =
      oahStringAtom::create (
        "dedication", "",
R"(Set 'dedication' to STRING in the LilyPond code \header.)",
        "STRING",
        "dedication",
        fDedication);
  subGroup->
    appendAtomToSubGroup (
      dedicationAtom);
  identificationMonoplexStringAtom->
    addStringAtom (
      dedicationAtom);

  S_oahStringAtom
    pieceAtom =
      oahStringAtom::create (
        "piece", "",
R"(Set 'piece' to STRING in the LilyPond code \header.)",
        "STRING",
        "piece",
        fPiece);
  subGroup->
    appendAtomToSubGroup (
      pieceAtom);
  identificationMonoplexStringAtom->
    addStringAtom (
      pieceAtom);

  S_oahStringAtom
    opusAtom =
      oahStringAtom::create (
        "opus", "",
R"(Set 'opus' to STRING in the LilyPond code \header.)",
        "STRING",
        "opus",
        fOpus);
  subGroup->
    appendAtomToSubGroup (
      opusAtom);
  identificationMonoplexStringAtom->
    addStringAtom (
      opusAtom);

  S_oahStringAtom
    titleAtom =
      oahStringAtom::create (
        "title", "",
R"(Set 'title' to STRING in the LilyPond code \header.)",
        "STRING",
        "title",
        fTitle);
  subGroup->
    appendAtomToSubGroup (
      titleAtom);
  identificationMonoplexStringAtom->
    addStringAtom (
      titleAtom);

  S_oahStringAtom
    subtitleAtom =
      oahStringAtom::create (
        "subtitle", "",
R"(Set 'subtitle' to STRING in the LilyPond code \header.)",
        "STRING",
        "subTitle",
        fSubTitle);
  subGroup->
    appendAtomToSubGroup (
      subtitleAtom);
  identificationMonoplexStringAtom->
    addStringAtom (
      subtitleAtom);

  S_oahStringAtom
    subsubtitleAtom =
      oahStringAtom::create (
        "subsubtitle", "",
R"(Set 'subsubtitle' to STRING in the LilyPond code \header.)",
        "STRING",
        "subSubTitle",
        fSubSubTitle);
  subGroup->
    appendAtomToSubGroup (
      subsubtitleAtom);
  identificationMonoplexStringAtom->
    addStringAtom (
      subsubtitleAtom);

  S_oahStringAtom
    instrumentAtom =
      oahStringAtom::create (
        "instrument", "",
R"(Set 'instrument' to STRING in the LilyPond code \header.)",
        "STRING",
        "instrument",
        fInstrument);
  subGroup->
    appendAtomToSubGroup (
      instrumentAtom);
  identificationMonoplexStringAtom->
    addStringAtom (
      instrumentAtom);

  S_oahStringAtom
    meterAtom =
      oahStringAtom::create (
        "meter", "",
R"(Set 'meter' to STRING in the LilyPond code \header.)",
        "STRING",
        "meter",
        fMeter);
  subGroup->
    appendAtomToSubGroup (
      meterAtom);
  identificationMonoplexStringAtom->
    addStringAtom (
      meterAtom);

  S_oahStringAtom
    taglineAtom =
      oahStringAtom::create (
        "tagline", "",
R"(Set 'tagline' to STRING in the LilyPond code \header.)",
        "STRING",
        "tagline",
        fTagline);
  subGroup->
    appendAtomToSubGroup (
      taglineAtom);
  identificationMonoplexStringAtom->
    addStringAtom (
      taglineAtom);

  S_oahStringAtom
    copyrightAtom =
      oahStringAtom::create (
        "copyright", "",
R"(Set 'copyright' to STRING in the LilyPond code \header.)",
        "STRING",
        "copyright",
        fCopyright);
  subGroup->
    appendAtomToSubGroup (
      copyrightAtom);
  identificationMonoplexStringAtom->
    addStringAtom (
      copyrightAtom);
}

void lpsr2lilypondOahGroup::initializePartsOptions ()
{
  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Parts",
        "hlpp", "help-lilypond-parts",
R"()",
      kElementVisibilityWhole,
      this);

  appendSubGroupToGroup (subGroup);

  // part name transposition
  // --------------------------------------

  S_lilypondTransposePartNameAtom
    transposePartNameAtom =
      lilypondTransposePartNameAtom::create (
        "lilytpn", "lilypond-transpose-part-name",
R"(Transpose part PART_NAME using TRANSPOSITION in the LilyPond code.
PART_TRANSPOSITION_SPEC can be:
'PART_NAME = TRANSPOSITION'
or
"PART_NAME = TRANSPOSITION"
The single or double quotes are used to allow spaces in the names
and around the '=' sign, otherwise they can be dispensed with.
TRANSPOSITION should contain a diatonic pitch, followed if needed
by a sequence of ',' or '\'' octave indications.
Such indications cannot be mixed, and they are relative to c\', i.e. middle C.
For example, 'a', 'f' and 'bes,' can be used respectively
for instruments in 'a', 'f' and B flat respectively.
Using double quotes allows for shell variables substitutions, as in:
SAXOPHONE="bes,"
EXECUTABLE -lilypond-transpose-part-name "P1 ${SAXOPHONE}" .
There can be several occurrences of this option.)",
        "PART_TRANSPOSITION_SPEC",
        "partNamesTranspositionMap",
        fPartNamesTranspositionMap);

  transposePartNameAtom->
      setMultipleOccurrencesAllowed ();

  subGroup->
    appendAtomToSubGroup (
        transposePartNameAtom);

  // part ID transposition
  // --------------------------------------

  S_lilypondTransposePartIDAtom
    transposePartIDAtom =
      lilypondTransposePartIDAtom::create (
        "lilytpi", "lilypond-transpose-part-id",
R"(Transpose part PART_ID using TRANSPOSITION in the LilyPond code.
PART_TRANSPOSITION_SPEC can be:
'PART_ID = TRANSPOSITION'
or
"PART_ID = TRANSPOSITION"
This is handly when a part doesn't have a part name.
See option '-lilypond-transpose-part-name' for the details.
There can be several occurrences of this option.)",
        "PART_TRANSPOSITION_SPEC",
        "partIDsTranspositionMap",
        fPartIDsTranspositionMap);

  transposePartIDAtom->
      setMultipleOccurrencesAllowed ();

  subGroup->
    appendAtomToSubGroup (
        transposePartIDAtom);
}

void lpsr2lilypondOahGroup::initializeEngraversOptions ()
{
  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Engravers",
        "hlpe", "help-lilypond-engravers",
R"()",
      kElementVisibilityWhole,
      this);

  appendSubGroupToGroup (subGroup);

  // ambitus engraver
  // --------------------------------------

  fAmbitusEngraver = false;

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "ambitus", "",
R"(Generate an ambitus range at the beginning of the staves/voices.)",
        "ambitusEngraver",
        fAmbitusEngraver));

  // custos engraver
  // --------------------------------------

  fCustosEngraver = false;

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "custos", "",
R"(Generate custos at the end of the lines.)",
        "custosEngraver",
        fCustosEngraver));
}

void lpsr2lilypondOahGroup::initializeClefsKeysTimesOptions ()
{
  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Clefs, keys, times",
        "hckt", "help-clefs-keys-times",
R"()",
      kElementVisibilityWhole,
      this);

  appendSubGroupToGroup (subGroup);

  // clefs
  // --------------------------------------

  fNoInitialTrebleClef = false;

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "nitc", "no-initial-treble-clef",
R"(Don't generate an initial treble clef, which is LiyPond's default clef.)",
        "noInitialTrebleClef",
        fNoInitialTrebleClef));

  fCommentClefChanges = false;

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "ccc", "comment-clef-changes",
R"(Comment clef changes in the LilyPond code.
They won't show up in the score, but the information is not lost.)",
        "commentClefChanges",
        fCommentClefChanges));

  // keys
  // --------------------------------------

  fNoInitialCMajorKey = false;

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "nicmk", "no-initial-c-major-key",
R"(Don't generate an initial C major key, which is LiyPond's default key.)",
        "noInitialCMajorKey",
        fNoInitialCMajorKey));

  // times
  // --------------------------------------

  fNoInitialCommonTime = false;

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "nict", "no-initial-common-time",
R"(Don't generate an initial common time (4/4) time, which is LiyPond's default time.)",
        "noInitialCommonTime",
        fNoInitialCommonTime));

  fNumericalTime = false;

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "numt", "numerical-time",
R"(Generate numerical time signatures, such as '4/4' instead of 'C'.)",
        "numericalTime",
        fNumericalTime));
}

void lpsr2lilypondOahGroup::initializeNotesOptions ()
{
  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Notes",
        "hlpn", "help-lilypond-notes",
R"()",
      kElementVisibilityWhole,
      this);

  appendSubGroupToGroup (subGroup);

  // octave entry
  // --------------------------------------

  lpsrOctaveEntryKind
    octaveEntryKindDefaultValue =
      kOctaveEntryAbsolute; // relative is LilyPond's default value
  fOctaveEntryKind = octaveEntryKindDefaultValue;

  // leave fOctaveEntrySemiTonesPitchAndOctave equal to nullptr here,
  // since \relative without a pitch and absolute octave entry
  // can be used in LilyPond, in which case the pitch and actave is:
  fSemiTonesPitchAndOctaveDefaultValue = // JMI
    msrSemiTonesPitchAndOctave::create (
      // F under middle C, LilyPond default for relative octave entry
      kF_Natural_STP,
      kOctave3);

  subGroup->
    appendAtomToSubGroup (
      lilypondAbsoluteOctaveEntryAtom::create (
        "abs", "absolute",
R"(Use absolute octave entry in the generated LilyPond code.)",
        "octaveEntryKind",
        fOctaveEntryKind));

  subGroup->
    appendAtomToSubGroup (
      lilypondRelativeOctaveEntryAtom::create (
        "rel", "relative",
        regex_replace (
R"(Use relative octave entry reference PITCH_AND_OCTAVE in the generated LilyPond code.
PITCH_AND_OCTAVE is made of a diatonic pitch and
an optional sequence of commas or single quotes.
It should be placed between double quotes if it contains single quotes, such as:
  -rel "c''".
The default is to use LilyPond's implicit reference 'DEFAULT_VALUE'.)",
          regex ("DEFAULT_VALUE"),
          msrSemiTonesPitchAndOctaveAsLilypondString (
            gGlobalLpsrOahGroup->getLpsrQuarterTonesPitchesLanguageKind (),
            fSemiTonesPitchAndOctaveDefaultValue)),
        "PITCH_AND_OCTAVE",
        "relativeOctaveEntrySemiTonesPitchAndOctave",
        fOctaveEntryKind,
        fRelativeOctaveEntrySemiTonesPitchAndOctave));

  subGroup->
    appendAtomToSubGroup (
      lilypondFixedOctaveEntryAtom::create (
        "fixed", "",
R"(Use fixed octave entry reference PITCH_AND_OCTAVE in the generated LilyPond code.
PITCH_AND_OCTAVE is made of a diatonic pitch and
an optional sequence of commas or single quotes.
It should be placed between double quotes if it contains single quotes, such as:
  -fixed "c''")",
        "PITCH_AND_OCTAVE",
        "fixedOctaveEntrySemiTonesPitchAndOctave",
        fOctaveEntryKind,
        fFixedOctaveEntrySemiTonesPitchAndOctave));

  // durations
  // --------------------------------------

  fAllDurations = false;

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "alldurs", "all-durations",
R"(Generate all LilyPond durations.
By default, a duration equal to preceding one found in the current voice
is omitted for code conciseness.)",
        "allDurations",
        fAllDurations));

  // beaming
  // --------------------------------------

  fNoAutoBeaming  = false;

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "noab", "no-auto-beaming",
R"(Generate '\set Staff.autoBeaming = ##f' in each voice
to prevent LilyPond from handling beams automatically.)",
        "noAutoBeaming",
        fNoAutoBeaming));

  fNoBeams  = false;

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "nbeams", "no-beams",
R"(Don't generate beams in the LilyPond code.
This is handy when the MusicXML data contains beam in vocal voices)",
        "noBeams",
        fNoBeams));

  // string numbers
  // --------------------------------------

  fRomanStringNumbers = false;

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "rsn", "roman-string-numbers",
R"(Generate '\romanStringNumbers' in each voice
for LilyPond to generate roman instead of arabic string numbers.)",
        "romanStringNumbers",
        fRomanStringNumbers));

  // open strings
  // --------------------------------------

  fAvoidOpenStrings    = false;

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "aos", "avoid-open-strings",
R"(Generate '\set TabStaff.restrainOpenStrings = ##t' in each voice
to prevent LilyPond from using open strings.)",
        "avoidOpenStrings",
        fAvoidOpenStrings));

  // accidentals
  // --------------------------------------

  const lpsrAccidentalStyleKind
    lpsrAccidentalStyleKindDefaultValue =
      kDefault; // default value

  fAccidentalStyleKind = lpsrAccidentalStyleKindDefaultValue;

  subGroup->
    appendAtomToSubGroup (
      lilypondAccidentalStyleKindAtom::create (
        "as", "accidental-style", // JMI
          regex_replace (
            regex_replace (
              regex_replace (
R"(The NUMBER LilyPond accidental styles available are:
ACCIDENTAL_STYLES.
The default is 'DEFAULT_VALUE'.)",
              regex ("NUMBER"),
              to_string (gGlobalLpsrAccidentalStyleKindsMap.size ())),
            regex ("ACCIDENTAL_STYLES"),
            existingLpsrAccidentalStyleKinds (K_NAMES_LIST_MAX_LENGTH)),
          regex ("DEFAULT_VALUE"),
          lpsrAccidentalStyleKindAsString (
            lpsrAccidentalStyleKindDefaultValue)),
        "STYLE",
        "accidentalStyleKind",
        fAccidentalStyleKind));

  // rest measures
  // --------------------------------------

  fCompressFullMeasureRests = false;

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "cfmr", "compress-full-measure-rests",
R"(Compress full measure rests instead of generating successive empty measures.)",
        "compressFullMeasureRests",
        fCompressFullMeasureRests));

  // merge rests
  // --------------------------------------

  fMergeRests = false;

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "mr", "merge-rests",
R"(Merge rests to obtain better looking scores.
This works only when there are 2 voices per staves.
Limitations: this only handles two voices
and does not handle multi-measure/whole-measure rests.)",
        "mergeRests",
        fMergeRests));

  // input line numbers
  // --------------------------------------

  fInputLineNumbers = false;

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "iln", "input-line-numbers",
        regex_replace (
R"(Generate after each note and barline a comment containing
its MusicXML input line number.
This is useful when debugging EXECUTABLE.)",
          regex ("EXECUTABLE"),
          gGlobalOahOahGroup->getHandlerExecutableName ()),
        "inputLineNumbers",
        fInputLineNumbers));

  // original measure numbers
  // --------------------------------------

  fOriginalMeasureNumbers = false;

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "omn", "original-measure-numbers",
        regex_replace (
R"(Generate after each end of measure a comment containing
its original MusicXML measure number.
This is useful for adding line breaks and page breaks, and when debugging EXECUTABLE.)",
          regex ("EXECUTABLE"),
          gGlobalOahOahGroup->getHandlerExecutableName ()),
        "originalMeasureNumbers",
        fOriginalMeasureNumbers));

  // generate positions in measures
  // --------------------------------------

  fGeneratePositionsInMeasures = false;

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "agpim", "generate-positions-in-measures",
        regex_replace (
R"(Generate after each note and barline a comment containing
its position in the measure.
This is useful when debugging EXECUTABLE.)",
          regex ("EXECUTABLE"),
          gGlobalOahOahGroup->getHandlerExecutableName ()),
        "generatePositionsInMeasures",
        fGeneratePositionsInMeasures));

  // generate moments in measures
  // --------------------------------------

  fGenerateMomentsInMeasures = false;

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "gmim", "generate-moments-in-measures",
        regex_replace (
R"(Generate after each note and barline a comment containing
its moment in the measure.
This is useful when debugging EXECUTABLE.)",
          regex ("EXECUTABLE"),
          gGlobalOahOahGroup->getHandlerExecutableName ()),
        "generateMomentsInMeasures",
        fGenerateMomentsInMeasures));

  // non-print notes head color
  // --------------------------------------

  fNonPrintNotesHeadRGBColor =
    msrRGBColor (0.0, 0.0, 0.0); // black by default

  fNonPrintNotesHeadRGBColorAtom =
    oahRGBColorAtom::create (
      "npnhc", "non-print-notes-head-color",
R"(Use RGB_COLOR for non print notes head color,
for MusicXML's <note print-object="no"> for example.
RGB_COLOR should be of the form 'r,g,b',
with r, g and b being float numbers between 0.0 and 1.0 inclusive.)",
      "RGB_COLOR",
      "nonPrintNotesHeadRGBColor",
      fNonPrintNotesHeadRGBColor);

  subGroup->
    appendAtomToSubGroup (
      fNonPrintNotesHeadRGBColorAtom);
}

void lpsr2lilypondOahGroup::initializeBarsOptions ()
{
  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Bar numbers",
        "hlpbn", "help-lilypond-bars-numbers",
R"()",
      kElementVisibilityWhole,
      this);

  appendSubGroupToGroup (subGroup);

  // all bar numbers
  // --------------------------------------

  fShowAllBarNumbers = false;

  S_oahBooleanAtom
    allBarNumbersAtom =
      oahBooleanAtom::create (
        "abn", "all-bar-numbers",
R"(Generate LilyPond code to show all bar numbers.)",
        "showAllBarNumbers",
        fShowAllBarNumbers);

  subGroup->
    appendAtomToSubGroup (allBarNumbersAtom);

  // all measure numbers
  // --------------------------------------

  subGroup->
    appendAtomToSubGroup (
      oahAtomSynonym::create (
        "amn", "all-measure-numbers",
R"(Generate LilyPond code to show all measure numbers.
This option is a synonym to '-abn, -all-bar-numbers'.)",
        allBarNumbersAtom));

  // reset measure number

  subGroup->
    appendAtomToSubGroup (
      oahStringToIntMapAtom::create (
        "rmn", "reset-measure-number",
R"(Generate a '\set Score.currentBarNumber = #NEW' command
at the beginning of measure OLD in the LilyPond code.
RESET_NUMBER_SPEC can be:
'OLD = NEW'
or
"OLD = NEW" .
OLD is the MusicXML original measure number (a string),
that can be generated in the LilyPond code in '| % ...' comments
with option '-omn, -original-measure-numbers'.
NEW is a LilyPond (integer) measure number.
This comes in handy when scanning several movements from a single PDF score,
in which case measure numbers are a single sequence.
There can be several occurrences of this option.)",
        "RESET_NUMBER_SPEC",
        "resetMeasureElementMeasureNumberMap",
        fResetMeasureElementMeasureNumberMap));

  // generate box around bar number
  // --------------------------------------

  S_oahIntSetAtom
    generateBoxAroundBarNumber =
      oahIntSetAtom::create (
        "gbabn", "generate-box-around-bar-number",
R"(Generate a box around LilyPond purist bar number BAR_NUMBER,
where BAR_NUMBER is an integer.
This implies that bar numbers are centered on the bars.
There can be several occurrences of this option.)",
        "BAR_NUMBER",
        "boxAroundBarNumberSet",
        fBoxAroundBarNumberSet);

  subGroup->
    appendAtomToSubGroup (generateBoxAroundBarNumber);

  // bar number checks
  // --------------------------------------

  fNoBarNumberChecks = false;

  fNoBarNumberChecksAtom =
    oahBooleanAtom::create (
      "nbnc", "no-bar-number-checks",
R"(Don't generate bar number checks in the LilyPond code.)",
      "noBarNumberChecks",
      fNoBarNumberChecks);

  subGroup->
    appendAtomToSubGroup (
      fNoBarNumberChecksAtom);
}

void lpsr2lilypondOahGroup::initializeLineBreaksOptions ()
{
  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Line breaks",
        "hlplb", "help-lilypond-line-breaks",
R"()",
      kElementVisibilityWhole,
      this);

  appendSubGroupToGroup (subGroup);

  // line breaks
  // --------------------------------------

  fIgnoreMusicXMLLineBreaks = false;

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "imlb", "ignore-musicxml-line-breaks",
R"(Ignore the line breaks from the MusicXML input - let LilyPond decide about them.)",
        "ignoreMusicXMLLineBreaks",
        fIgnoreMusicXMLLineBreaks));

  fBreakLinesAtIncompleteRightMeasures = false;

  fSuppressMusicXMLLineBreaks = false;

  fSuppressMusicXMLLineBreaksAtom =
    oahBooleanAtom::create (
      "smlb", "suppress-musicxml-line-breaks",
R"(Suppress the line breaks from the MusicXML input - let LilyPond decide about them,
and don't generate an empty \myBreak.)",
      "suppressMusicXMLLineBreaks",
      fSuppressMusicXMLLineBreaks);

  subGroup->
    appendAtomToSubGroup (
      fSuppressMusicXMLLineBreaksAtom);

  fBreakLinesAtIncompleteRightMeasures = false;

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "blairm", "break-lines-at-incomplete-right-measures",
R"(Generate a '\break' command at the end of incomplete right measures
which is handy in popular folk dances and tunes.)",
        "breakLinesAtIncompleteRightMeasures",
        fBreakLinesAtIncompleteRightMeasures));

  fSeparatorLineEveryNMeasures = -1;

  subGroup->
    appendAtomToSubGroup (
      oahIntegerAtom::create (
        "slenm", "separator-line-every-n-measures",
R"(Generate an additional separator line for readability every N measures,
where N is a positive integer.
Nothing special is done by default.)",
        "N",
        "separatorLineEveryNMeasures",
        fSeparatorLineEveryNMeasures));

/* JMI
  subGroup->
    appendAtomToSubGroup (
      oahStringsSetElementAtom::create (
        "blamn", "break-line-after-measure-number",
R"(Generate a '\break' command after measure NUMBER in the LilyPond code.
NUMBER is a MusicXML measure number (a string), to be found in the latter.
This comes in handy when scanning several movements from a single PDF score.
There can be several occurrences of this option.)",
        "NUMBER",
        "breakLineAfterMeasureNumberSet",
        fBreakLineAfterMeasureNumberSet));
        */
}

void lpsr2lilypondOahGroup::initializePageBreaksOptions ()
{
  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Page breaks",
        "hlppb", "help-lilypond-page-breaks",
R"()",
      kElementVisibilityWhole,
      this);

  appendSubGroupToGroup (subGroup);

  // page breaks
  // --------------------------------------

  fIgnoreMusicXMLPageBreaks = false;

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "impb", "ignore-musixcml-page-breaks",
R"(Ignore the page breaks from the MusicXML input - let LilyPond decide about them.)",
        "ignoreMusicXMLPageBreaks",
        fIgnoreMusicXMLPageBreaks));

  fSuppressMusicXMLPageBreaks = false;

  fSuppressMusicXMLPageBreaksAtom =
    oahBooleanAtom::create (
      "smpb", "suppress-musixcml-page-breaks",
R"(Suppress the page breaks from the MusicXML input - let LilyPond decide about them,
and don't generate an empty \myPageBreak.)",
      "suppressMusicXMLPageBreaks",
      fSuppressMusicXMLPageBreaks);

  subGroup->
    appendAtomToSubGroup (
      fSuppressMusicXMLPageBreaksAtom);

  // break page after measure number
  // --------------------------------------

/* JMI
  subGroup->
    appendAtomToSubGroup (
      oahStringsSetElementAtom::create (
        "bpamn", "break-page-after-measure-number",
R"(Generate a '\pageBreak' command after measure NUMBER in the LilyPond code.
NUMBER is a MusicXML measure number (a string), to be found in the latter.
This comes in handy when scanning several movements from a single PDF score.
There can be several occurrences of this option.)",
        "NUMBER",
        "breakPageAfterMeasureNumberSet",
        fBreakPageAfterMeasureNumberSet));
        */
}

void lpsr2lilypondOahGroup::initializeStavesOptions ()
{
  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Staves",
        "hlps", "helpLilypondStaves",
R"()",
      kElementVisibilityWhole,
      this);

  appendSubGroupToGroup (subGroup);

  // tabs
  // --------------------------------------

  fModernTab = false;

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "mtab", "modernTab",
R"(Generate '\moderntab' instead of the default '\tab'.)",
        "modernTab",
        fModernTab));

  fTabFullNotation = false;

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "tfn", "tabFullNotation",
R"(Generate '\tabFullNotation' to obtain more complete tab notation,
including note stems.)",
        "tabFullNotation",
        fTabFullNotation));

  // staves
  // --------------------------------------

  fKeepStaffSize = false;

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "kss", "keep-staff-size",
R"(Use the staff size value found in the MusicXML data.)",
        "keepStaffSize",
        fKeepStaffSize));

  // ledger lines
  // --------------------------------------

  fLedgerLinesRGBColor =
    msrRGBColor (0.0, 0.0, 0.0); // black by default

  fLedgerLinesRGBColorAtom =
    oahRGBColorAtom::create (
      "llc", "ledger-lines-color",
R"(Use RGB_COLOR for the ledger lines.
RGB_COLOR should be of the form 'r,g,b',
with r, g and b being float numbers between 0.0 and 1.0 inclusive.)",
      "RGB_COLOR",
      "ledgerLinesRGBColor",
      fLedgerLinesRGBColor);

  subGroup->
    appendAtomToSubGroup (
      fLedgerLinesRGBColorAtom);
}

void lpsr2lilypondOahGroup::initializeChordsOptions ()
{
  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Chords",
        "hlpc", "help-lilypond-chordss",
R"()",
      kElementVisibilityWhole,
      this);

  appendSubGroupToGroup (subGroup);

  // arpeggios
  // --------------------------------------

  fConnectArpeggios = false;

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "conarp", "connect-arpeggios",
R"(Connect arpeggios across piano staves.)",
        "connectArpeggios",
        fConnectArpeggios));
}

void lpsr2lilypondOahGroup::initializeTupletsOptions ()
{
  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Tuplets",
        "hlpt", "help-lilypond-tuplets",
R"()",
      kElementVisibilityWhole,
      this);

  appendSubGroupToGroup (subGroup);

  // tuplets
  // --------------------------------------

  fIndentTuplets = false;

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "itups", "indent-tuplets",
R"(Indent tuplets notes on a line of their own,
instead of keeping the on one and the same line.)",
        "indentTuplets",
        fIndentTuplets));
}

void lpsr2lilypondOahGroup::initializeRepeatOptions ()
{
  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Repeats",
        "hlpr", "help-lilypond-repeats",
R"()",
      kElementVisibilityWhole,
      this);

  appendSubGroupToGroup (subGroup);

  // repeats
  // --------------------------------------

  fKeepRepeatBarlines = false;

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "krbls", "keep-repeat-barlines",
R"(Generate repeats start and and bar lines even though LilyPond would take care of them.)",
        "keepRepeatBarlines",
        fKeepRepeatBarlines));

  fRepeatBrackets = false;

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "rbracks", "repeat-brackets",
R"(Generate repeats with brackets instead of regular bar lines.)",
        "repeatBrackets",
        fRepeatBrackets));

  fIgnoreRepeatNumbers = false;

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "irn", "ignore-repeat-numbers",
R"(Ignore repeats numbers and let LilyPond determine them.)",
        "ignoreRepeatNumbers",
        fIgnoreRepeatNumbers));
}

void lpsr2lilypondOahGroup::initializeOrnamentsOptions ()
{
  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Ornaments",
        "hlporns", "help-lilypond-ornaments",
R"()",
      kElementVisibilityWhole,
      this);

  appendSubGroupToGroup (subGroup);

  // ornaments
  // --------------------------------------

  fDelayedOrnamentsFraction = rational (1, 2);

  string delayedOrnamentsFractionDefaultValue =
    to_string (fDelayedOrnamentsFraction.getNumerator ()) +
      "/" +
    to_string (fDelayedOrnamentsFraction.getDenominator ());

  subGroup->
    appendAtomToSubGroup (
      oahRationalAtom::create (
        "dof", "delayed-ornaments-fraction",
        regex_replace (
R"(Place the delayed turn/reverseturn at the given fraction
between the ornemented note and the next one.
The default is 'DEFAULT_VALUE'.)",
          regex ("DEFAULT_VALUE"),
          delayedOrnamentsFractionDefaultValue),
        "NUM/DENOM",
        "delayedOrnamentsFraction",
        fDelayedOrnamentsFraction));
}

void lpsr2lilypondOahGroup::initializeChordsDisplayOptions ()
{
  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Chords display",
        "hchd", "help-chords-display",
R"()",
      kElementVisibilityWhole,
      this);

  appendSubGroupToGroup (subGroup);

  // chords
  // --------------------------------------

  subGroup->
    appendAtomToSubGroup (
      lilypondChordsDisplayAtom::create (
        "chd", "chords-display",
/*
Including a closing parenthesis in a raw string:
  choose ### as the delimiter so only )###" ends the string:
    R"###(
        Some Text)"
    )###";
*/
R"###(Use SPECIFICATION to display chords using LilyPond's chordNameExceptions.
SPECIFICATION should contain a chord contents such as '<c ees ges bes>',
followed by code to display it, for example:
  '<c ees ges bes> \\super {"-7(" {\\small\\raise #0.5 \\flat} "5)"}'.
The LilyPond code has to escape backslashes, thus use '\\' to obtain '\'.
These two elements are passed over to LilyPond verbatim, without any check.
This option can be used any number of times.)###",
        "SPECIFICATION",
        "chordsDisplayList",
        fChordsDisplayList));

  fJazzChordsDisplay = false;

  fJazzChordsDisplayLilypondcode =
R"###(  <c ees ges bes>1-\markup { \super {"-7(" {\small\raise #0.5 \flat} "5)"} }
  <c e g bes>1-\markup { \super "7" }
  <c e gis bes>1-\markup { \super {"7(" {\small\raise #0.5 \sharp} "5)"} }
  <c f g bes>1-\markup { \super {"7(sus4)"} }
  <c e g a d'>1-\markup { \super "6/9" }
  <c e g bes des'>1-\markup { \super {"7(" {\small\raise #0.5 \flat} "9)"} }
  <c f g bes d'>1-\markup { \super {"9(sus4)"} }
  <c e g bes d'>1-\markup { \super "9" }
  <c e g b d'>1-\markup { \super "maj9" }
  <c e gis bes d'>1-\markup { \super "9+" }
  <c e g bes d' fis'>1-\markup { \super "9#11" }
  <c e g bes d' f'>1-\markup { \super "11" }
  <c e g bes d' a'>1-\markup { \super "13" }
  <c e g bes d' fis' a'>1-\markup { \super {"13(" {\small\raise #0.5 \sharp} "11)"} }
  <c e g a d'>1-\markup { \super "6(add9)" })###";

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "jchd", "jazz-chords-display",
        regex_replace (
R"(Display the chords using LilyPond's chordNameExceptions
and a set of standard specifications to display them, i.e.:
LILYPOND_CODE)",
          regex ("LILYPOND_CODE"),
          fJazzChordsDisplayLilypondcode),
        "jazzChordsDisplay",
        fJazzChordsDisplay));
}

void lpsr2lilypondOahGroup::initializeLyricsOptions ()
{
  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Lyrics",
        "hlyrics", "help-lyrics",
R"()",
      kElementVisibilityWhole,
      this);

  appendSubGroupToGroup (subGroup);

  // add stanzas mumbers
  // --------------------------------------

  fAddStanzasNumbers = false;

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "asn", "add-stanzas-numbers-to-lyrics",
R"(Add stanzas numbers to lyrics.)",
        "addStanzasNumbers",
        fAddStanzasNumbers));

  // lyrics durations
  // --------------------------------------

  const lpsrLyricsDurationsKind
    lpsrLyricsDurationsKindDefaultValue =
      kLyricsDurationsExplicit; // default value

  fLyricsDurationsKind = lpsrLyricsDurationsKindDefaultValue;

  subGroup->
    appendAtomToSubGroup (
      lilypondLyricsDurationsKindAtom::create (
        "ld", "lyrics-durations", // JMI
          regex_replace (
            regex_replace (
              regex_replace (
R"(The NUMBER LilyPond lyrics durations STYLEs available are:
LYRICS_DURATIONS_KINDS.
Using 'implicit' prevents the creation of lyrics attached to rests by LilyPond,
use 'explicit' in such cases.
The default is 'DEFAULT_VALUE'.)",
              regex ("NUMBER"),
              to_string (gGlobalLpsrLyricsDurationsKindsMap.size ())),
            regex ("LYRICS_DURATIONS_KINDS"),
            existingLpsrLyricsDurationsKinds (K_NAMES_LIST_MAX_LENGTH)),
          regex ("DEFAULT_VALUE"),
          lpsrLyricsDurationsKindAsString (
            lpsrLyricsDurationsKindDefaultValue)),
        "STYLE",
        "lyricsDurationsKind",
        fLyricsDurationsKind));
}

void lpsr2lilypondOahGroup::initializeFontsOptions ()
{
  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Fonts",
        "hfonts", "help-fonts",
R"()",
      kElementVisibilityWhole,
      this);

  appendSubGroupToGroup (subGroup);

  // fonts
  // --------------------------------------

  fJazzFonts = false;

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "jazz", "jazz-fonts",
R"(Use the LilyJazz font for notes, braces, text and chords.
It can be downloaded from https://github.com/OpenLilyPondFonts/lilyjazz.
This font should be installed so that LilyPond can use it, see:
https://github.com/OpenLilyPondFonts/lilyjazz/blob/master/LilyPond-Fonts-Installation-And-Usage.txt.)",
        "jazzFonts",
        fJazzFonts));
}

void lpsr2lilypondOahGroup::initializeCodeGenerationOptions ()
{
  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Code generation",
        "hlpcg", "help-lilypond-code-generation",
R"()",
      kElementVisibilityWhole,
      this);

  appendSubGroupToGroup (subGroup);

  // infos
  // --------------------------------------

  fXml2lyInfos = false;

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "x2li", "xml2ly-infos",
R"(Generate initial comments showing the compilation date.)",
        "xml2lyInfos",
        fXml2lyInfos));

  // comments
  // --------------------------------------

  fLilypondComments = false;

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "lpcom", "lilypond-comments",
R"(Generate comments showing the structure of the score
such as '% part P_POne (P1).)",
        "LilyPondComments",
        fLilypondComments));

  // global
  // --------------------------------------

  fGlobal = false;

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "global", "",
R"(Generate a 'global' empty variable and place a use of it
at the beginning of all voices.)",
        "global",
        fGlobal));

  // staff size
  // --------------------------------------

  fNoSetGlobalStaffSize = false;

  fNoSetGlobalStaffSizeAtom =
    oahBooleanAtom::create (
      "nsgss", "no-set-global-staff-size",
R"(Don't generate #(set-global-staff-size ...).)",
      "noSetGlobalStaffSize",
      fNoSetGlobalStaffSize);

  subGroup->
    appendAtomToSubGroup (
      fNoSetGlobalStaffSizeAtom);

  // languages
  // --------------------------------------

  fUseLilypondDefaultLanguages = false;

  fUseLilypondDefaultLanguagesAtom =
    oahBooleanAtom::create (
      "uldf", "use-lilypond-default-languages",
R"(Don't generate a \book block.)",
      "useLilyPondDefaultLanguages",
      fUseLilypondDefaultLanguages);

  subGroup->
    appendAtomToSubGroup (
      fUseLilypondDefaultLanguagesAtom);

  // book block
  // --------------------------------------

  fNoBookBlock = false;

  fNoBookBlockAtom =
    oahBooleanAtom::create (
      "nbb", "no-book-block",
R"(Don't generate a \book block.)",
      "noBookBlock",
      fNoBookBlock);

  subGroup->
    appendAtomToSubGroup (
      fNoBookBlockAtom);

  // header block
  // --------------------------------------

  fNoHeaderBlock = false;

  fNoHeaderBlockAtom =
    oahBooleanAtom::create (
      "nhb", "no-header-block",
R"(Don't generate a \header block.)",
      "noHeaderBlock",
      fNoHeaderBlock);

  subGroup->
    appendAtomToSubGroup (
      fNoHeaderBlockAtom);

  // layout block
  // --------------------------------------

  // layout score context
  fNoLayoutScoreContext = false;

  fNoLayoutScoreContextAtom =
    oahBooleanAtom::create (
      "nlsc", "no-layout-score-context",
R"(Don't generate a \score context in the \layout block.)",
      "noLayoutScoreContext",
      fNoLayoutScoreContext);

  subGroup->
    appendAtomToSubGroup (
      fNoLayoutScoreContextAtom);

  // layout voice context
  fNoLayoutVoiceContext = false;

  fNoLayoutVoiceContextAtom =
    oahBooleanAtom::create (
        "nlvc", "no-layout-voice-context",
  R"(Don't generate a \voice context in the \layout block.)",
        "noLayoutVoiceContext",
        fNoLayoutVoiceContext);

  subGroup->
    appendAtomToSubGroup (
      fNoLayoutVoiceContextAtom);

  // create block layout block
  fCreateTopLevelLayoutBlock = false;

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "ctllb", "create-top-level-layout-block",
  R"(Create a top-level \layout block.)",
        "createTopLevelLayoutBlock",
        fCreateTopLevelLayoutBlock));

  // paper block
  // --------------------------------------

  fNoPaperBlock = false;

  fNoPaperBlockAtom =
    oahBooleanAtom::create (
      "npb", "no-paper-block",
R"(Don't generate a \paper block.)",
      "noPaperBlock",
      fNoPaperBlock);

  subGroup->
    appendAtomToSubGroup (
      fNoPaperBlockAtom);

  // display music
  // --------------------------------------

  fDisplayMusic = false;

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "dm", "display-music",
R"(Place the contents of all voices inside a '\displayMusic' block,
for LilyPond to show its internal representation of the music.)",
        "displayMusic",
        fDisplayMusic));

  // LilyPond code
  // --------------------------------------

  fNoLilypondCode = false;

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "nolpc", "no-lilypond-code",
R"(Don't generate any LilyPond code.
That can be useful if only a summary of the score is needed.)",
        "noLilypondCode",
        fNoLilypondCode));

  // LilyPond lyrics
  // --------------------------------------

  fNoLilypondLyrics = false;

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "nolpl", "no-lilypond-lyrics",
R"(Don't generate any lyrics in the LilyPond code.)",
        "noLilypondLyrics",
        fNoLilypondLyrics));

  // compile date
  // --------------------------------------

  fLilypondCompileDate = false;

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "lpcd", "lilypond-compile-date",
R"(Generate code to include the compilation date
when LilyPond creates the score.)",
        "lilypondCompileDate",
        fLilypondCompileDate));

  // draft mode
  // --------------------------------------

  fDraftMode = false;

  S_oahBooleanAtom
    draftOahBooleanAtom =
      oahBooleanAtom::create (
        "draft", "draft-mode",
R"(Generate a difficult to use score
to get only an overview in the resulting PDF file.)",
        "draftMode",
        fDraftMode);
  draftOahBooleanAtom->
    setElementVisibilityKind (
      kElementVisibilityHidden);

  subGroup->
    appendAtomToSubGroup (
      draftOahBooleanAtom);

  // point and click
  // --------------------------------------

  fPointAndClickOff = false;

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "paco", "point-and-clic-off",
R"(Generate \pointAndClickOff at the beginning of the LilyPond code
to reduce the size of the resulting PDF file.)",
        "pointAndClickOff",
        fPointAndClickOff));

  // white note heads
  // --------------------------------------

  fWhiteNoteHeads = false;

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "wnh", "white-note-heads",
R"(Generate Scheme function 'whiteNoteHeads'
at the beginning of the LilyPond code.)",
        "whiteNoteHeads",
        fWhiteNoteHeads));

  // generate commented out variables
  // --------------------------------------

  fGenerateCommentedOutVariables = false;

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "gcov", "generate-commented-out-variables",
R"(Generate LilyPond variables as comment,
to avoid having to add them by hand afterwards.)",
        "generateCommentedOutVariables",
        fGenerateCommentedOutVariables));
}

void lpsr2lilypondOahGroup::initializeScoreNotationOptions ()
{
  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Score notation",
        "hlpsn", "help-lilypond-score-notation",
R"()",
      kElementVisibilityWhole,
      this);

  appendSubGroupToGroup (subGroup);

  // score notation
  // --------------------------------------

//  fScoreNotationKind =  lpsr2lilypondOahGroup::kWesternNotation;
    // default value

  fJianpu = false;

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "jianpu", "",
R"(Generate the score using the Jianpu (numbered) notation
instead of the default western notation.
That option needs lilypond-Jianpu to be accessible to LilyPond
(https://github.com/nybbs2003/lilypond-Jianpu/jianpu10a.ly).)",
        "jianpu",
        fJianpu));
}

void lpsr2lilypondOahGroup::initializeMidiOptions ()
{
  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Midi",
        "hlpm", "help-lilypond-midi",
R"()",
      kElementVisibilityWhole,
      this);

  appendSubGroupToGroup (subGroup);

  // midiTempo
  // --------------------------------------

  string midiTempoDuration  = "4";
  int    midiTempoPerSecond = 90;

  fMidiTempo.setMidiTempoDuration (midiTempoDuration);
  fMidiTempo.setMidiTempoPerSecond (midiTempoPerSecond);

  string midiTempoDefaultValue =
    midiTempoDuration + " = " + to_string (midiTempoPerSecond);

  subGroup->
    appendAtomToSubGroup (
      oahMidiTempoAtom::create (
        "midtempo", "midi-tempo",
        regex_replace (
          regex_replace (
R"(Generate a '\tempo' command in the \midi block.
MIDI_TEMPO_SPEC can be:
'DURATION = PER_SECOND'
or
"DURATION = PER_SECOND" .
DURATION is a string such as '8.', and PER_SECOND is an integer.
The single or double quotes are used to allow spaces around the '=' sign,
otherwise they can be dispensed with.
Using double quotes allows for shell variables substitutions, as in:
PER_SECOND=66
EXECUTABLE -midiTempo "8. ${PER_SECOND}" .
The default is 'DEFAULT_VALUE'.)",
            regex ("EXECUTABLE"),
            gGlobalOahOahGroup->getHandlerExecutableName ()),
          regex ("DEFAULT_VALUE"),
          midiTempoDefaultValue),
        "MIDI_TEMPO_SPEC",
        "midiTempo",
        fMidiTempo));

  // noMidi
  // --------------------------------------

  fNoMidi = false;

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "no-midi", "",
R"(Generate the '\midi' block as a comment instead of active code.)",
        "noMidi",
        fNoMidi));
}

void lpsr2lilypondOahGroup::initializeCombinedOptionsOptions ()
{
  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Combined options",
        "hmxmlco", "help-musicxml-lilypond-combined-options",
R"()",
        kElementVisibilityWhole,
        this);

  appendSubGroupToGroup (subGroup);

  // minimal
  // --------------------------------------

  fMinimal = false;

  // create the 'minimal' combined atom
  fMinimalCombinedBooleansAtom =
    oahCombinedBooleansAtom::create (
      "minimal", "",
R"(Avoids the generation of some LilyPond code, with is otherwise
meant to facilitate manual editing and completion of the result.)",
      "minimal",
      fMinimal);

  subGroup->
    appendAtomToSubGroup (
      fMinimalCombinedBooleansAtom);

  // set the '-minimal' option by default
  fMinimalCombinedBooleansAtom->
    setCombinedBooleanVariables (fMinimal);

  // populate the 'minimal' combined atom
  fMinimalCombinedBooleansAtom->
    addBooleanAtom (
      fNoSetGlobalStaffSizeAtom);

  fMinimalCombinedBooleansAtom->
    addBooleanAtom (
      fUseLilypondDefaultLanguagesAtom);

  fMinimalCombinedBooleansAtom->
    addBooleanAtom (
      fNoBookBlockAtom);

  fMinimalCombinedBooleansAtom->
    addBooleanAtom (
      fNoHeaderBlockAtom);

  fMinimalCombinedBooleansAtom->
    addBooleanAtom (
      fNoLayoutScoreContextAtom);

  fMinimalCombinedBooleansAtom->
    addBooleanAtom (
      fNoLayoutVoiceContextAtom);

  fMinimalCombinedBooleansAtom->
    addBooleanAtom (
      fNoPaperBlockAtom);

  fMinimalCombinedBooleansAtom->
    addBooleanAtom (
      fNoBarNumberChecksAtom);

  fMinimalCombinedBooleansAtom->
    addBooleanAtom (
      fSuppressMusicXMLLineBreaksAtom);

  fMinimalCombinedBooleansAtom->
    addBooleanAtom (
      fSuppressMusicXMLPageBreaksAtom);
}

void lpsr2lilypondOahGroup::initializeLpsr2lilypondOahGroup ()
{
  // LilyPond version
  // --------------------------------------
  initializeLilypondVersionOptions ();

  // global staff size
  // --------------------------------------
  initializeGlobalStaffSizeOptions ();

  // identification
  // --------------------------------------
  initializeIdentificationOptions ();

  // parts
  // --------------------------------------
  initializePartsOptions ();

  // engravers
  // --------------------------------------
  initializeEngraversOptions ();

  // time
  // --------------------------------------
  initializeClefsKeysTimesOptions ();

  // notes
  // --------------------------------------
  initializeNotesOptions ();

  // bars
  // --------------------------------------
  initializeBarsOptions ();

  // line breaks
  // --------------------------------------
  initializeLineBreaksOptions ();

  // page breaks
  // --------------------------------------
  initializePageBreaksOptions ();

  // staves
  // --------------------------------------
  initializeStavesOptions ();

  // chords
  // --------------------------------------
  initializeChordsOptions ();

  // tuplets
  // --------------------------------------
  initializeTupletsOptions ();

  // repeats
  // --------------------------------------
  initializeRepeatOptions ();

  // ornaments
  // --------------------------------------
  initializeOrnamentsOptions ();

  // chords display
  // --------------------------------------
  initializeChordsDisplayOptions ();

  // lyrics
  // --------------------------------------
  initializeLyricsOptions ();

  // fonts
  // --------------------------------------
  initializeFontsOptions ();

  // code generation
  // --------------------------------------
  initializeCodeGenerationOptions ();

  // score notation
  // --------------------------------------
  initializeScoreNotationOptions ();

  // midi
  // --------------------------------------
  initializeMidiOptions ();

  // combined options
  // --------------------------------------
  initializeCombinedOptionsOptions ();
}

/* JMI
bool lpsr2lilypondOahGroup::setAccidentalStyleKind (lpsrAccidentalStyleKind accidentalStyleKind)
{
  // is accidentalStyleKind in the accidental styles set?
  map<string, lpsrAccidentalStyleKind>::const_iterator
    it =
      gGlobalLpsrAccidentalStyleKindsMap.find (accidentalStyleKind);

  if (it == gGlobalLpsrAccidentalStyleKindsMap.end ()) {
    // no, accidentalStyleKind is unknown
    return false;
  }

  fAccidentalStyleKind = it.second;

  return true;
}
*/

//______________________________________________________________________________
void lpsr2lilypondOahGroup::crackLilypondVersionNumber (
  string theString,
  int&   generationNumber,
  int&   majorNumber,
  int&   minorNumber)
{
  // obtains the three numbers in "2.19.83" or "2.20" for example

  // decipher theString with a three-number regular expression
  string regularExpression (
    "([[:digit:]]+)" // generation number
    "."
    "([[:digit:]]+)" // major number
    "."
    "([[:digit:]]+)" // minor number
    );

  regex  e (regularExpression);
  smatch sm;

  regex_match (theString, sm, e);

  unsigned int smSize = sm.size ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalLpsrOahGroup->getTraceLilypondVersion ()) {
    gLogStream <<
      "There are " << smSize << " matches" <<
      " for version string '" << theString <<
      "' with regex '" << regularExpression <<
      "'" <<
      endl <<
      smSize << " elements: ";

      for (unsigned i = 0; i < smSize; ++i) {
        gLogStream <<
          "[" << sm [i] << "] ";
      } // for

      gLogStream << endl;
    }
#endif

  if (smSize == 4) {
    // found an n.x.y specification
    string
      generationNumberValue = sm [1],
      majorNumberValue      = sm [2],
      minorNumberValue      = sm [3];

#ifdef TRACING_IS_ENABLED
    if (gGlobalLpsrOahGroup->getTraceLilypondVersion ()) {
      gLogStream <<
        "--> generationNumberValue = \"" << generationNumberValue << "\", " <<
        "--> majorNumberValue = \"" << majorNumberValue << "\", " <<
        "--> minorNumberValue = \"" << minorNumberValue << "\"" <<
        endl;
    }
#endif

    generationNumber = stoi (generationNumberValue);
    majorNumber      = stoi (majorNumberValue);
    minorNumber      = stoi (minorNumberValue);
  }

  else {
    // decipher theString with a two-number regular expression
    string regularExpression (
      "([[:digit:]]+)" // generation number
      "."
      "([[:digit:]]+)" // major number
      );

    regex  e (regularExpression);
    smatch sm;

    regex_match (theString, sm, e);

    unsigned int smSize = sm.size ();

#ifdef TRACING_IS_ENABLED
    if (gGlobalLpsrOahGroup->getTraceLilypondVersion ()) {
      gLogStream <<
        "There are " << smSize << " matches" <<
        " for chord details string '" << theString <<
        "' with regex '" << regularExpression <<
        "'" <<
        endl <<
        smSize << " elements: ";

        for (unsigned i = 0; i < smSize; ++i) {
          gLogStream <<
            "[" << sm [i] << "] ";
        } // for

        gLogStream << endl;
      }
#endif

    if (smSize == 3) {
      // found an n.x specification
      // assume implicit 0 minor number
      string
        generationNumberValue = sm [1],
        majorNumberValue      = sm [2];

#ifdef TRACING_IS_ENABLED
      if (gGlobalLpsrOahGroup->getTraceLilypondVersion ()) {
        gLogStream <<
          "--> generationNumberValue = \"" << generationNumberValue << "\", " <<
          "--> majorNumberValue = \"" << majorNumberValue << "\", " <<
          endl;
      }
#endif

      generationNumber = stoi (generationNumberValue);
      majorNumber      = stoi (majorNumberValue);
      minorNumber      = 0;
    }

    else {
      stringstream s;

      s <<
        "version number argument '" << theString <<
        "' is ill-formed";

      oahError (s.str ());
    }
  }
}

bool lpsr2lilypondOahGroup::versionNumberGreaterThanOrEqualTo (
  string otherVersionNumber)
{
  bool result = false;

  int
    lilyPondVersionGenerationNumber,
    lilyPondVersionMajorNumber,
    lilyPondVersionMinorNumber;

  crackLilypondVersionNumber (
    fLilypondVersion,
    lilyPondVersionGenerationNumber,
    lilyPondVersionMajorNumber,
    lilyPondVersionMinorNumber);

  int
    otherVersionNumbeGenerationNumber,
    otherVersionNumbeMajorNumber,
    otherVersionNumbeMinorNumber;

  crackLilypondVersionNumber (
    otherVersionNumber,
    otherVersionNumbeGenerationNumber,
    otherVersionNumbeMajorNumber,
    otherVersionNumbeMinorNumber);

  if (otherVersionNumbeGenerationNumber != 2) {
    gLogStream <<
      "Using verstion \"" <<
      otherVersionNumbeGenerationNumber << ".x.y\" " <<
      "is probably not such a good idea" <<
      endl;
  }

  if (otherVersionNumbeMajorNumber < 19) {
    gLogStream <<
      "Using a verstion older than \"" <<
      otherVersionNumbeGenerationNumber << ".19.y\" " <<
      "is not a good idea: the generated LilyPond code uses features introduced in the latter" <<
      endl;
  }

  if (lilyPondVersionGenerationNumber < otherVersionNumbeGenerationNumber) {
    result = false;
  }
  else if (lilyPondVersionGenerationNumber > otherVersionNumbeGenerationNumber) {
    result = true;
  }
  else {
    // same generation number
    if (lilyPondVersionMajorNumber < otherVersionNumbeMajorNumber) {
      result = false;
    }
    else if (lilyPondVersionMajorNumber > otherVersionNumbeMajorNumber) {
      result = true;
    }
    else {
      // same major number
      result =
        lilyPondVersionMinorNumber >= otherVersionNumbeMinorNumber;
    }
  }

  return result;
}

//______________________________________________________________________________
void lpsr2lilypondOahGroup::enforceGroupQuietness ()
{}

//______________________________________________________________________________
void lpsr2lilypondOahGroup::checkGroupOptionsConsistency ()
{
  // JMI
}

//______________________________________________________________________________
void lpsr2lilypondOahGroup::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> lpsr2lilypondOahGroup::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_lpsr2lilypondOahGroup>*
    p =
      dynamic_cast<visitor<S_lpsr2lilypondOahGroup>*> (v)) {
        S_lpsr2lilypondOahGroup elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching lpsr2lilypondOahGroup::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void lpsr2lilypondOahGroup::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> lpsr2lilypondOahGroup::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_lpsr2lilypondOahGroup>*
    p =
      dynamic_cast<visitor<S_lpsr2lilypondOahGroup>*> (v)) {
        S_lpsr2lilypondOahGroup elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching lpsr2lilypondOahGroup::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void lpsr2lilypondOahGroup::browseData (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> lpsr2lilypondOahGroup::browseData ()" <<
      endl;
  }
#endif
}

//______________________________________________________________________________
string lpsr2lilypondOahGroup::scoreNotationKindAsString (
  scoreNotationKind notationKind)
{
  string result;

  switch (notationKind) {
    case lpsr2lilypondOahGroup::kWesternNotation:
      result = "western";
      break;
    case lpsr2lilypondOahGroup::kJianpuNotation:
      result = "jiǎnpǔ";
      break;
    case lpsr2lilypondOahGroup::kABCNotation:
      result = "abc";
      break;
  } // switch

  return result;
}

void lpsr2lilypondOahGroup::printAtomWithValueOptionsValues (
  ostream&     os,
  unsigned int valueFieldWidth) const
{
  os <<
    "The LilyPond lilypond are:" <<
    endl;

  ++gIndenter;

  // LilyPond version
  // --------------------------------------

  gLogStream <<
    "LilyPond version:" <<
    endl;

  ++gIndenter;

  gLogStream << left <<
    setw (valueFieldWidth) << "lilyPondVersion" << " : " <<
    fLilypondVersion <<
    /* JMI
    endl <<
    setw (fieldWidth) << "lilyPondVersionHasBeenSet" << " : " <<
    fLilypondVersionHasBeenSet <<
    */
    endl;

  --gIndenter;

  // identification
  // --------------------------------------
  os <<
    "Identification:" <<
    endl;

  ++gIndenter;

  // MusicXML informations
  os << left <<
    setw (valueFieldWidth) << "rights" << " : " <<
      fRights <<
      endl <<

    setw (valueFieldWidth) << "composer" << " : " <<
      fComposer <<
      endl <<
    setw (valueFieldWidth) << "arranger" << " : " <<
      fArranger <<
      endl <<
    setw (valueFieldWidth) << "poet" << " : " <<
      fPoet <<
      endl <<
    setw (valueFieldWidth) << "lyricist" << " : " <<
      fLyricist <<
      endl <<

    setw (valueFieldWidth) << "software" << " : " <<
      fSoftware <<
      endl <<

  // LilyPond informations
    setw (valueFieldWidth) << "dedication" << " : " <<
      fDedication <<
      endl <<

    setw (valueFieldWidth) << "piece" << " : " <<
      fPiece <<
      endl <<
    setw (valueFieldWidth) << "opus" << " : " <<
      fOpus <<
      endl <<

    setw (valueFieldWidth) << "title" << " : " <<
      fTitle <<
      endl <<
    setw (valueFieldWidth) << "subTitle" << " : " <<
      fSubTitle <<
      endl <<
    setw (valueFieldWidth) << "subSubTitle" << " : " <<
      fSubSubTitle <<
      endl <<

    setw (valueFieldWidth) << "instrument" << " : " <<
      fInstrument <<
      endl <<

    setw (valueFieldWidth) << "meter" << " : " <<
      fMeter <<
      endl <<

    setw (valueFieldWidth) << "tagline" << " : " <<
      fTagline <<
      endl <<

    setw (valueFieldWidth) << "copyright" << " : " <<
      fCopyright <<
      endl;

  --gIndenter;


  // parts
  // --------------------------------------

  // part names transposition

  gLogStream << left <<
    setw (valueFieldWidth) << "part names transposition" << " : ";

  if (! fPartNamesTranspositionMap.size ()) {
    gLogStream << "none";
  }
  else {
    for (
      map<string, S_msrSemiTonesPitchAndOctave>::const_iterator i =
        fPartNamesTranspositionMap.begin ();
      i != fPartNamesTranspositionMap.end ();
      ++i
    ) {
      gLogStream <<
        "Part name \"" << ((*i).first) <<
        " = " <<
        ((*i).second->asString ()) <<
        "\" ";
    } // for
  }

  // part IDs transposition

  gLogStream << left <<
    setw (valueFieldWidth) << "part IDs transposition" << " : ";

  if (! fPartIDsTranspositionMap.size ()) {
    gLogStream << "none";
  }
  else {
    for (
      map<string, S_msrSemiTonesPitchAndOctave>::const_iterator i =
        fPartIDsTranspositionMap.begin ();
      i != fPartIDsTranspositionMap.end ();
      ++i
    ) {
      gLogStream <<
        "Part ID \"" << ((*i).first) <<
        " = " <<
        ((*i).second->asString ()) <<
        "\" ";
    } // for
  }

  // engravers
  // --------------------------------------
  os <<
    "Engravers:" <<
    endl;

  ++gIndenter;

  os << left <<
    setw (valueFieldWidth) << "ambitusEngraver" << " : " <<
      booleanAsString (fAmbitusEngraver) <<
      endl <<
    setw (valueFieldWidth) << "custosEngraver" << " : " <<
      booleanAsString (fCustosEngraver) <<
      endl;

  --gIndenter;


  // clefs
  // --------------------------------------
  os <<
    "Clefs:" <<
    endl;

  ++gIndenter;

  os << left <<
    setw (valueFieldWidth) << "commentClefChanges" << " : " <<
      booleanAsString (fCommentClefChanges) <<
      endl;

  --gIndenter;


  // keys
  // --------------------------------------
  os <<
    "Clefs:" <<
    endl;

  ++gIndenter;

  os << left <<
    setw (valueFieldWidth) << "commentClefChanges" << " : " <<
      booleanAsString (fCommentClefChanges) <<
      endl;

  --gIndenter;


  // times
  // --------------------------------------
  os <<
    "Times:" <<
    endl;

  ++gIndenter;

  os << left <<
    setw (valueFieldWidth) << "numericalTime" << " : " <<
      booleanAsString (fNumericalTime) <<
      endl;

  --gIndenter;


  // notes
  // --------------------------------------
  os <<
    "Notes:" <<
    endl;

  ++gIndenter;

  os << left <<
    setw (valueFieldWidth) << "octaveEntryKind" << " : " <<
    lpsrOctaveEntryKindAsString (fOctaveEntryKind) <<
    endl;

  switch (gGlobalLpsr2lilypondOahGroup->getOctaveEntryKind ()) {
    case kOctaveEntryRelative:
      if (fRelativeOctaveEntrySemiTonesPitchAndOctave) {
        os << left <<
          setw (valueFieldWidth) <<
          "relativeOctaveEntrySemiTonesPitchAndOctave" << " : ";
/* JMI
        if (fVariableHasBeenSet) {
          os <<
            ", variableHasBeenSet: " <<
            booleanAsString (fVariableHasBeenSet);
        }
*/
        os <<
          fRelativeOctaveEntrySemiTonesPitchAndOctave->asString () <<
          endl;
      }
      break;
    case kOctaveEntryAbsolute:
      break;
    case kOctaveEntryFixed:
      if (fFixedOctaveEntrySemiTonesPitchAndOctave) {
        os << left <<
          setw (valueFieldWidth) <<
          "fixedOctaveEntrySemiTonesPitchAndOctave" << " : ";
/* JMI
        if (fVariableHasBeenSet) {
          os <<
            ", variableHasBeenSet: " <<
            booleanAsString (fVariableHasBeenSet);
        }
*/
        os <<
          fFixedOctaveEntrySemiTonesPitchAndOctave->asString () <<
          endl;
      }
      break;
  } // switch

  os << left <<
    setw (valueFieldWidth) << "allDurations" << " : " <<
      booleanAsString (fAllDurations) <<
      endl <<

    setw (valueFieldWidth) << "noAutoBeaming" << " : " <<
      booleanAsString (fNoAutoBeaming) <<
      endl <<
    setw (valueFieldWidth) << "noBeams" << " : " <<
      booleanAsString (fNoBeams) <<
      endl <<

    setw (valueFieldWidth) << "romanStringNumbers" << " : " <<
      booleanAsString (fRomanStringNumbers) <<
      endl <<
    setw (valueFieldWidth) << "avoidOpenString" << " : " <<
      booleanAsString (fAvoidOpenStrings) <<
      endl <<

    setw (valueFieldWidth) << "accidentalStyleKind" << " : " <<
      fAccidentalStyleKind <<
      endl <<

    setw (valueFieldWidth) << "lyricsDurationsKind" << " : " <<
      lpsrLyricsDurationsKindAsString (fLyricsDurationsKind) <<
      endl <<

    setw (valueFieldWidth) << "compressFullMeasureRests" << " : " <<
      booleanAsString (fCompressFullMeasureRests) <<
      endl <<
    setw (valueFieldWidth) << "mergeRests" << " : " <<
      booleanAsString (fMergeRests) <<
      endl <<

    setw (valueFieldWidth) << "inputLineNumbers" << " : " <<
      booleanAsString (fInputLineNumbers) <<
      endl <<

    setw (valueFieldWidth) << "originalMeasureNumbers" << " : " <<
      booleanAsString (fOriginalMeasureNumbers) <<
      endl <<

    setw (valueFieldWidth) << "generatePositionsInMeasures" << " : " <<
      booleanAsString (fGeneratePositionsInMeasures) <<
      endl <<

    setw (valueFieldWidth) << "generateMomentsInMeasures" << " : " <<
      booleanAsString (fGenerateMomentsInMeasures) <<
      endl <<

    setw (valueFieldWidth) << "nonPrintNotesHeadRGBColor" << " : " <<
      fNonPrintNotesHeadRGBColor.asString () <<
      endl;

  --gIndenter;


  // bars and measures
  // --------------------------------------
  os <<
    "Bars:" <<
    endl;

  ++gIndenter;

  os << left <<
    setw (valueFieldWidth) << "showAllBarNumbers" << " : " <<
    booleanAsString (fShowAllBarNumbers) <<
    endl;

  os << left <<
    setw (valueFieldWidth) << "resetMeasureElementMeasureNumberMap" << " : ";
  if (! fResetMeasureElementMeasureNumberMap.size ()) {
    os << "empty";
  }
  else {
    map<string, int>::const_iterator
      iBegin = fResetMeasureElementMeasureNumberMap.begin (),
      iEnd   = fResetMeasureElementMeasureNumberMap.end (),
      i      = iBegin;
    for ( ; ; ) {
      os << (*i).first << "=" << (*i).second;
      if (++i == iEnd) break;
      os << ",";
    } // for
  }
  os << endl;

  os << left <<
    setw (valueFieldWidth) << "boxAroundBarNumberSet" << " : ";
  if (! fBoxAroundBarNumberSet.size ()) {
    os << "empty";
  }
  else {
    set<int>::const_iterator
      iBegin = fBoxAroundBarNumberSet.begin (),
      iEnd   = fBoxAroundBarNumberSet.end (),
      i      = iBegin;
    for ( ; ; ) {
      os << (*i);
      if (++i == iEnd) break;
      os << ",";
    } // for
  }
  os << endl;

  --gIndenter;


  // line breaks
  // --------------------------------------
  os <<
    "Line breaks:" <<
    endl;

  ++gIndenter;

  os << left <<
    setw (valueFieldWidth) << "ignoreMusicXMLLineBreaks" << " : " <<
      booleanAsString (fIgnoreMusicXMLLineBreaks) <<
      endl <<
    setw (valueFieldWidth) << "suppressMusicXMLLineBreaks" << " : " <<
      booleanAsString (fSuppressMusicXMLLineBreaks) <<
      endl <<

    setw (valueFieldWidth) << "breakLinesAtIncompleteRightMeasures" << " : " <<
      booleanAsString (fBreakLinesAtIncompleteRightMeasures) <<
      endl <<

    setw (valueFieldWidth) << "separatorLineEveryNMeasures" << " : " <<
      fSeparatorLineEveryNMeasures <<
      endl;

  --gIndenter;


  // page breaks
  // --------------------------------------
  os <<
    "Page breaks:" <<
    endl;

  ++gIndenter;

  os << left <<
    setw (valueFieldWidth) << "ignoreMusicXMLPageBreaks" << " : " <<
    booleanAsString (fIgnoreMusicXMLPageBreaks) <<
    endl <<
    setw (valueFieldWidth) << "suppressMusicXMLPageBreaks" << " : " <<
    booleanAsString (fSuppressMusicXMLPageBreaks) <<
    endl;

  --gIndenter;


  // staves
  // --------------------------------------
  os <<
    "Staves:" <<
    endl;

  ++gIndenter;

  os << left <<
    setw (valueFieldWidth) << "modernTab" << " : " <<
    booleanAsString (fModernTab) <<
    endl <<
    setw (valueFieldWidth) << "tabFullNotation" << " : " <<
    booleanAsString (fTabFullNotation) <<
    endl <<

    setw (valueFieldWidth) << "keepStaffSize" << " : " <<
    booleanAsString (fKeepStaffSize) <<
    endl;

  --gIndenter;


  // chords
  // --------------------------------------

  os <<
    "Chords:" <<
    endl;

  ++gIndenter;

  os << left <<
    setw (valueFieldWidth) << "connectArpeggios" << " : " <<
    booleanAsString (fConnectArpeggios) <<
    endl;

  --gIndenter;


  // tuplets
  // --------------------------------------

  os <<
    "Tuplets:" <<
    endl;

  ++gIndenter;

  os << left <<
    setw (valueFieldWidth) << "indentTuplets" << " : " <<
    booleanAsString (fIndentTuplets) <<
    endl;

  --gIndenter;


  // repeats
  // --------------------------------------

  os <<
    "Repeats:" <<
    endl;

  ++gIndenter;

  os << left <<
    setw (valueFieldWidth) << "keepRepeatBarlines" << " : " <<
    booleanAsString (fKeepRepeatBarlines) <<
    endl <<
    setw (valueFieldWidth) << "repeatBrackets" << " : " <<
    booleanAsString (fRepeatBrackets) <<
    endl <<
    setw (valueFieldWidth) << "ignoreRepeatNumbers" << " : " <<
    booleanAsString (fIgnoreRepeatNumbers) <<
    endl;

  --gIndenter;


  // lyrics
  // --------------------------------------

  gLogStream <<
    "Lyrics:" <<
    endl;

  ++gIndenter;

  gLogStream <<
    setw (valueFieldWidth) << "addStanzasNumbers" << " : " <<
    booleanAsString (fAddStanzasNumbers) <<
    endl;

  --gIndenter;

  // ornaments
  // --------------------------------------

  os <<
    "Ornaments:" <<
    endl;

  ++gIndenter;

  os << left <<
    setw (valueFieldWidth) << "fDelayedOrnamentsFraction" << " : " <<
    fDelayedOrnamentsFraction <<
    endl;

  --gIndenter;


  // chords display
  // --------------------------------------

  os <<
    "Chords display:" <<
    endl;

  ++gIndenter;

  os << left <<
    setw (valueFieldWidth) << "chordsDisplayList" << " : ";

  if (fChordsDisplayList.size ()) {
    os << endl;

    ++gIndenter;

    list<pair<string, string> >::const_iterator
      iBegin = fChordsDisplayList.begin (),
      iEnd   = fChordsDisplayList.end (),
      i      = iBegin;

    for ( ; ; ) {
      os <<
        gTab <<
        (*i).first <<
        " -> " <<
        (*i).second <<
        endl;
      if (++i == iEnd) break;
  //     os << endl;
    } // for

    --gIndenter;
  }
  else {
    os <<
      "none";
  }
  os << endl;

  os << left <<
    setw (valueFieldWidth) << "jazzChordsDisplay" << " : " <<
    booleanAsString (fJazzChordsDisplay) <<
    endl <<
    setw (valueFieldWidth) << "jazzChordsDisplayLilypondcode" << " : " <<
    fJazzChordsDisplayLilypondcode <<
    endl;

  --gIndenter;


  // fonts
  // --------------------------------------

  os <<
    "Fonts:" <<
    endl;

  ++gIndenter;

  os << left <<
    setw (valueFieldWidth) << "jazzFonts" << " : " <<
    booleanAsString (fJazzFonts) <<
    endl;

  --gIndenter;


  // code generation
  // --------------------------------------

  os <<
    "LilyPond code generation:" <<
    endl;

  ++gIndenter;

  os << left <<
    setw (valueFieldWidth) << "xml2lyInfos" << " : " <<
      booleanAsString (fXml2lyInfos) <<
      endl <<

    setw (valueFieldWidth) << "LilyPondComments" << " : " <<
      booleanAsString (fLilypondComments) <<
      endl <<

    setw (valueFieldWidth) << "global" << " : " <<
      booleanAsString (fGlobal) <<
      endl <<

    setw (valueFieldWidth) << "noPaperBlock" << " : " <<
      booleanAsString (fNoPaperBlock) <<
      endl <<

    setw (valueFieldWidth) << "displayMusic" << " : " <<
      booleanAsString (fDisplayMusic) <<
      endl <<

    setw (valueFieldWidth) << "noLilypondCode" << " : " <<
      booleanAsString (fNoLilypondCode) <<
      endl <<

    setw (valueFieldWidth) << "noLilypondLyrics" << " : " <<
      booleanAsString (fNoLilypondLyrics) <<
      endl <<

    setw (valueFieldWidth) << "lilypondCompileDate" << " : " <<
      booleanAsString (fLilypondCompileDate) <<
      endl <<

    setw (valueFieldWidth) << "draftMode" << " : " <<
      booleanAsString (fDraftMode) <<
      endl <<

    setw (valueFieldWidth) << "pointAndClickOff" << " : " <<
      booleanAsString (fPointAndClickOff) <<
      endl <<

    setw (valueFieldWidth) << "whiteNoteHeads" << " : " <<
      booleanAsString (fWhiteNoteHeads) <<
      endl;

  --gIndenter;


  // score notation
  // --------------------------------------

  os <<
    "Score notation:" <<
    endl;

  ++gIndenter;

  os << left <<
    setw (valueFieldWidth) << "fJianpu" << " : " <<
      booleanAsString (fJianpu) <<
      endl;

  --gIndenter;


  // midi
  // --------------------------------------
  os <<
    "Midi:" <<
    endl;

  ++gIndenter;

  os << left <<
    setw (valueFieldWidth) << "midiTempo" << " : " <<
      fMidiTempo.asString () <<
      endl <<

    setw (valueFieldWidth) << "noMidi" << " : " <<
      booleanAsString (fNoMidi) <<
      endl;

  --gIndenter;

  --gIndenter;
}

void lpsr2lilypondOahGroup::printLpsr2lilypondOahValues (unsigned int fieldWidth)
{
  gLogStream <<
    "The LilyPond lilypond are:" <<
    endl;

  ++gIndenter;

  // global staff size
  // --------------------------------------

  gLogStream <<
    "Global staff size:" <<
    endl;

  ++gIndenter;

  gLogStream << left <<
    setw (fieldWidth) << "globalStaffSize" << " : " <<
    fGlobalStaffSize <<
    endl <<
    setw (fieldWidth) << "staffGlobalSizeDefaultValue" << " : " <<
    fStaffGlobalSizeDefaultValue <<
    endl;

  --gIndenter;

  // identification
  // --------------------------------------
  gLogStream <<
    "Identification:" <<
    endl;

  ++gIndenter;

  // MusicXML informations
  gLogStream << left <<
    setw (fieldWidth) << "rights" << " : " <<
      fRights <<
      endl <<

    setw (fieldWidth) << "composer" << " : " <<
      fComposer <<
      endl <<
    setw (fieldWidth) << "arranger" << " : " <<
      fArranger <<
      endl <<
    setw (fieldWidth) << "poet" << " : " <<
      fPoet <<
      endl <<
    setw (fieldWidth) << "lyricist" << " : " <<
      fLyricist <<
      endl <<

    setw (fieldWidth) << "software" << " : " <<
      fSoftware <<
      endl <<

  // LilyPond informations
    setw (fieldWidth) << "dedication" << " : " <<
      fDedication <<
      endl <<

    setw (fieldWidth) << "piece" << " : " <<
      fPiece <<
      endl <<
    setw (fieldWidth) << "opus" << " : " <<
      fOpus <<
      endl <<

    setw (fieldWidth) << "title" << " : " <<
      fTitle <<
      endl <<
    setw (fieldWidth) << "subTitle" << " : " <<
      fSubTitle <<
      endl <<
    setw (fieldWidth) << "subSubTitle" << " : " <<
      fSubSubTitle <<
      endl <<
    setw (fieldWidth) << "instrument" << " : " <<
      fInstrument <<
      endl <<
    setw (fieldWidth) << "meter" << " : " <<
      fMeter <<
      endl <<

    setw (fieldWidth) << "tagline" << " : " <<
      fTagline <<
      endl <<

    setw (fieldWidth) << "copyright" << " : " <<
      fCopyright <<
      endl;

  --gIndenter;


  // engravers
  // --------------------------------------
  gLogStream <<
    "Engravers:" <<
    endl;

  ++gIndenter;

  gLogStream << left <<
    setw (fieldWidth) << "ambitusEngraver" << " : " <<
      booleanAsString (fAmbitusEngraver) <<
      endl <<
    setw (fieldWidth) << "custosEngraver" << " : " <<
      booleanAsString (fCustosEngraver) <<
      endl;

  --gIndenter;


  // clefs
  // --------------------------------------
  gLogStream <<
    "Clefs:" <<
    endl;

  ++gIndenter;

  gLogStream << left <<
    setw (fieldWidth) << "noInitialTrebleClef" << " : " <<
      booleanAsString (fNoInitialTrebleClef) <<
      endl <<
    setw (fieldWidth) << "commentClefChanges" << " : " <<
      booleanAsString (fCommentClefChanges) <<
      endl;

  --gIndenter;


  // keys
  // --------------------------------------
  gLogStream <<
    "Keys:" <<
    endl;

  ++gIndenter;

  gLogStream << left <<
    setw (fieldWidth) << "noInitialCMajorKey" << " : " <<
      booleanAsString (fNoInitialCMajorKey) <<
      endl;

  --gIndenter;


  // time
  // --------------------------------------
  gLogStream <<
    "Times:" <<
    endl;

  ++gIndenter;

  gLogStream << left <<
    setw (fieldWidth) << "noInitialCommonTime" << " : " <<
      booleanAsString (fNoInitialCommonTime) <<
      endl <<
    setw (fieldWidth) << "numericalTime" << " : " <<
      booleanAsString (fNumericalTime) <<
      endl;

  --gIndenter;


  // notes
  // --------------------------------------
  gLogStream <<
    "Notes:" <<
    endl;

  ++gIndenter;

  gLogStream << left <<
    setw (fieldWidth) << "octaveEntryKind" << " : " <<
      lpsrOctaveEntryKindAsString (fOctaveEntryKind) <<
      endl <<

    setw (fieldWidth) << "allDurations" << " : " <<
      booleanAsString (fAllDurations) <<
      endl <<

    setw (fieldWidth) << "noAutoBeaming" << " : " <<
      booleanAsString (fNoAutoBeaming) <<
      endl <<
    setw (fieldWidth) << "noBeams" << " : " <<
      booleanAsString (fNoBeams) <<
      endl <<

    setw (fieldWidth) << "romanStringNumbers" << " : " <<
      booleanAsString (fRomanStringNumbers) <<
      endl <<
    setw (fieldWidth) << "avoidOpenString" << " : " <<
      booleanAsString (fAvoidOpenStrings) <<
      endl <<

    setw (fieldWidth) << "accidentalStyleKind" << " : " <<
      fAccidentalStyleKind <<
      endl <<

    setw (fieldWidth) << "lyricsDurationsKind" << " : " <<
      lpsrLyricsDurationsKindAsString (fLyricsDurationsKind) <<
      endl <<

    setw (fieldWidth) << "compressFullMeasureRests" << " : " <<
      booleanAsString (fCompressFullMeasureRests) <<
      endl <<

    setw (fieldWidth) << "mergeRests" << " : " <<
      booleanAsString (fMergeRests) <<
      endl <<

    setw (fieldWidth) << "inputLineNumbers" << " : " <<
      booleanAsString (fInputLineNumbers) <<
      endl <<

    setw (fieldWidth) << "originalMeasureNumbers" << " : " <<
      booleanAsString (fOriginalMeasureNumbers) <<
      endl <<

    setw (fieldWidth) << "generatePositionsInMeasures" << " : " <<
      booleanAsString (fGeneratePositionsInMeasures) <<
      endl <<

    setw (fieldWidth) << "generateMomentsInMeasures" << " : " <<
      booleanAsString (fGenerateMomentsInMeasures) <<
      endl <<

    setw (fieldWidth) << "nonPrintNotesHeadRGBColor" << " : " <<
      fNonPrintNotesHeadRGBColor.asString () <<
      endl;

  --gIndenter;


  // bars and measures
  // --------------------------------------
  gLogStream <<
    "Bars:" <<
    endl;

  ++gIndenter;

  gLogStream << left <<
    setw (fieldWidth) << "showAllBarNumbers" << " : " <<
    booleanAsString (fShowAllBarNumbers) <<
    endl;

  gLogStream << left <<
    setw (fieldWidth) << "resetMeasureElementMeasureNumberMap" << " : ";
  if (! fResetMeasureElementMeasureNumberMap.size ()) {
    gLogStream << "empty";
  }
  else {
    map<string, int>::const_iterator
      iBegin = fResetMeasureElementMeasureNumberMap.begin (),
      iEnd   = fResetMeasureElementMeasureNumberMap.end (),
      i      = iBegin;
    for ( ; ; ) {
      gLogStream << (*i).first << "=" << (*i).second;
      if (++i == iEnd) break;
      gLogStream << ",";
    } // for
  }
  gLogStream << endl;

  gLogStream << left <<
    setw (fieldWidth) << "boxAroundBarNumberSet" << " : ";
  if (! fBoxAroundBarNumberSet.size ()) {
    gLogStream << "empty";
  }
  else {
    set<int>::const_iterator
      iBegin = fBoxAroundBarNumberSet.begin (),
      iEnd   = fBoxAroundBarNumberSet.end (),
      i      = iBegin;
    for ( ; ; ) {
      gLogStream << (*i);
      if (++i == iEnd) break;
      gLogStream << ",";
    } // for
  }
  gLogStream << endl;

  --gIndenter;


  // line breaks
  // --------------------------------------
  gLogStream <<
    "Line breaks:" <<
    endl;

  ++gIndenter;

  gLogStream << left <<
    setw (fieldWidth) << "ignoreMusicXMLLineBreaks" << " : " <<
      booleanAsString (fIgnoreMusicXMLLineBreaks) <<
      endl <<
    setw (fieldWidth) << "suppressMusicXMLLineBreaks" << " : " <<
      booleanAsString (fSuppressMusicXMLLineBreaks) <<
      endl <<

    setw (fieldWidth) << "breakLinesAtIncompleteRightMeasures" << " : " <<
      booleanAsString (fBreakLinesAtIncompleteRightMeasures) <<
      endl <<

    setw (fieldWidth) << "separatorLineEveryNMeasures" << " : " <<
      fSeparatorLineEveryNMeasures <<
      endl;

  --gIndenter;


  // page breaks
  // --------------------------------------
  gLogStream <<
    "Page breaks:" <<
    endl;

  ++gIndenter;

  gLogStream << left <<
    setw (fieldWidth) << "ignoreMusicXMLPageBreaks" << " : " <<
    booleanAsString (fIgnoreMusicXMLPageBreaks) <<
    endl <<
    setw (fieldWidth) << "suppressMusicXMLPageBreaks" << " : " <<
    booleanAsString (fSuppressMusicXMLPageBreaks) <<
    endl;

  --gIndenter;


  // staves
  // --------------------------------------
  gLogStream <<
    "Staves:" <<
    endl;

  ++gIndenter;

  gLogStream << left <<
    setw (fieldWidth) << "modernTab" << " : " <<
    booleanAsString (fModernTab) <<
    endl <<
    setw (fieldWidth) << "tabFullNotation" << " : " <<
    booleanAsString (fTabFullNotation) <<
    endl;

  --gIndenter;


  // chords
  // --------------------------------------

  gLogStream <<
    "Chords:" <<
    endl;

  ++gIndenter;

  gLogStream << left <<
    setw (fieldWidth) << "connectArpeggios" << " : " <<
    booleanAsString (fConnectArpeggios) <<
    endl;

  --gIndenter;


  // tuplets
  // --------------------------------------

  gLogStream <<
    "Tuplets:" <<
    endl;

  ++gIndenter;

  gLogStream << left <<
    setw (fieldWidth) << "indentTuplets" << " : " <<
    booleanAsString (fIndentTuplets) <<
    endl;

  --gIndenter;


  // repeats
  // --------------------------------------

  gLogStream <<
    "Repeats:" <<
    endl;

  ++gIndenter;

  gLogStream << left <<
    setw (fieldWidth) << "keepRepeatBarlines" << " : " <<
    booleanAsString (fKeepRepeatBarlines) <<
    endl <<
    setw (fieldWidth) << "repeatBrackets" << " : " <<
    booleanAsString (fRepeatBrackets) <<
    endl <<
    setw (fieldWidth) << "ignoreRepeatNumbers" << " : " <<
    booleanAsString (fIgnoreRepeatNumbers) <<
    endl;

  --gIndenter;


  // ornaments
  // --------------------------------------

  gLogStream <<
    "Ornaments:" <<
    endl;

  ++gIndenter;

  gLogStream << left <<
    setw (fieldWidth) << "fDelayedOrnamentsFraction" << " : " <<
    fDelayedOrnamentsFraction <<
    endl;

  --gIndenter;


  // chords display
  // --------------------------------------

  gLogStream <<
    "Chords display:" <<
    endl;

  ++gIndenter;

  gLogStream << left <<
    setw (fieldWidth) << "jazzChordsDisplay" << " : " <<
    booleanAsString (fJazzChordsDisplay) <<
    endl <<
    setw (fieldWidth) << "jazzChordsDisplayLilypondcode" << " : " <<
    fJazzChordsDisplayLilypondcode <<
    endl;

  --gIndenter;


  // fonts
  // --------------------------------------

  gLogStream <<
    "Fonts:" <<
    endl;

  ++gIndenter;

  gLogStream << left <<
    setw (fieldWidth) << "jazzFonts" << " : " <<
    booleanAsString (fJazzFonts) <<
    endl;

  --gIndenter;


  // code generation
  // --------------------------------------

  gLogStream <<
    "LilyPond code generation:" <<
    endl;

  ++gIndenter;

  gLogStream << left <<
    setw (fieldWidth) << "xml2lyInfos" << " : " <<
      booleanAsString (fXml2lyInfos) <<
      endl <<

    setw (fieldWidth) << "LilyPondComments" << " : " <<
      booleanAsString (fLilypondComments) <<
      endl <<

    setw (fieldWidth) << "global" << " : " <<
      booleanAsString (fGlobal) <<
      endl <<

    setw (fieldWidth) << "displayMusic" << " : " <<
      booleanAsString (fDisplayMusic) <<
      endl <<

    setw (fieldWidth) << "noLilypondCode" << " : " <<
      booleanAsString (fNoLilypondCode) <<
      endl <<

    setw (fieldWidth) << "noLilypondLyrics" << " : " <<
      booleanAsString (fNoLilypondLyrics) <<
      endl <<

    setw (fieldWidth) << "lilypondCompileDate" << " : " <<
      booleanAsString (fLilypondCompileDate) <<
      endl <<

    setw (fieldWidth) << "draftMode" << " : " <<
      booleanAsString (fDraftMode) <<
      endl <<

    setw (fieldWidth) << "pointAndClickOff" << " : " <<
      booleanAsString (fPointAndClickOff) <<
      endl <<

    setw (fieldWidth) << "whiteNoteHeads" << " : " <<
      booleanAsString (fWhiteNoteHeads) <<
      endl <<

    setw (fieldWidth) << "generateCommentedOutVariables" << " : " <<
      booleanAsString (fGenerateCommentedOutVariables) <<
      endl;

  --gIndenter;


  // score notation
  // --------------------------------------

  gLogStream <<
    "Score notation:" <<
    endl;

  ++gIndenter;

  gLogStream << left <<
    setw (fieldWidth) << "fJianpu" << " : " <<
      booleanAsString (fJianpu) <<
      endl;

  --gIndenter;


  // midi
  // --------------------------------------
  gLogStream <<
    "Midi:" <<
    endl;

  ++gIndenter;

  gLogStream << left <<
    setw (fieldWidth) << "fMidiTempo" << " : " <<
      fMidiTempo.asString () <<
      endl <<

    setw (fieldWidth) << "noMidi" << " : " <<
      booleanAsString (fNoMidi) <<
      endl;

  --gIndenter;

  --gIndenter;
}

ostream& operator<< (ostream& os, const S_lpsr2lilypondOahGroup& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_lpsr2lilypondOahGroup createGlobalLpsr2lilypondOahGroup ()
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceOah () && ! gGlobalGeneralOahGroup->getQuiet ()) {
    gLogStream <<
      "Initializing LilyPond OAH handling" <<
      endl;
  }
#endif

  // protect library against multiple initializations
  if (! gGlobalLpsr2lilypondOahGroup) {
    // create the global OAH group
    gGlobalLpsr2lilypondOahGroup =
      lpsr2lilypondOahGroup::create ();
    assert (gGlobalLpsr2lilypondOahGroup != 0);
  }

  // return the global OAH group
  return gGlobalLpsr2lilypondOahGroup;
}


}


/*
//______________________________________________________________________________
S_lilypondBreakPageAfterMeasureNumberAtom lilypondBreakPageAfterMeasureNumberAtom::create (
  string       shortName,
  string       longName,
  string       description,
  string       valueSpecification,
  string       variableName,
  set<string>& fStringSetVariable)
{
  lilypondBreakPageAfterMeasureNumberAtom* o = new
    lilypondBreakPageAfterMeasureNumberAtom (
      shortName,
      longName,
      description,
      valueSpecification,
      variableName,
      fStringSetVariable);
  assert (o!=0);
  return o;
}

lilypondBreakPageAfterMeasureNumberAtom::lilypondBreakPageAfterMeasureNumberAtom (
  string       shortName,
  string       longName,
  string       description,
  string       valueSpecification,
  string       variableName,
  set<string>& fStringSetVariable)
  : oahAtomWithValue (
      shortName,
      longName,
      description,
      valueSpecification,
      variableName),
    fStringSetVariable (
      fStringSetVariable)
{}

lilypondBreakPageAfterMeasureNumberAtom::~lilypondBreakPageAfterMeasureNumberAtom ()
{}

void lilypondBreakPageAfterMeasureNumberAtom::applyAtomWithValue (
  string   theString,
  ostream& os)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceOah ()) {
    gLogStream <<
      "==> oahAtom is of type 'lilypondBreakPageAfterMeasureNumberAtom'" <<
      endl;
  }
#endif

  // theString contains the midi tempo specification
  // decipher it to extract duration and perSecond values

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceOah ()) {
    gLogStream <<
      "==> oahAtom is of type 'lilypondBreakPageAfterMeasureNumberAtom'" <<
      endl;
  }
#endif

  string regularExpression (
    "[[:space:]]*"
    "([[:digit:]]+\\.*)" // musicXMLMeasureNumber
    "[[:space:]]*"
    "="
    "[[:space:]]*"
    "([[:digit:]]+)"     // lilypondMeasureNumber
    "[[:space:]]*"
    );

  regex  e (regularExpression);
  smatch sm;

  regex_match (theString, sm, e);

  unsigned int smSize = sm.size ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceOah ()) {
    gLogStream <<
      "There are " << smSize << " matches" <<
      " for reset measure number string '" << theString <<
      "' with regex '" << regularExpression <<
      "':" <<
      endl;

    ++gIndenter;

    for (unsigned i = 0; i < smSize; ++i) {
      gLogStream <<
        i << ": " << "\"" << sm [i] << "\"" <<
        endl;
    } // for
    gLogStream << endl;

    --gIndenter;
  }
#endif

  if (smSize != 3) {
    stringstream s;

    s <<
      "-breakPageAfterMeasureNumber argument '" << theString <<
      "' is ill-formed";

    oahError (s.str ());
  }

  string musicXMLMeasureNumber = sm [1];

  int lilypondMeasureNumber;
  {
    stringstream s;
    s << sm [2];
    s >> lilypondMeasureNumber;
  }

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceOah ()) {
    os <<
      "musicXMLMeasureNumber  = " <<
      musicXMLMeasureNumber <<
      endl <<
      "lilypondMeasureNumber = " <<
      lilypondMeasureNumber <<
      endl;

  fStringSetVariable [musicXMLMeasureNumber] = lilypondMeasureNumber;
  }
#endif
}

void lilypondBreakPageAfterMeasureNumberAtom::print (ostream& os) const
{
  const unsigned int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "OptionsBreakPageAfterMeasureNumberAtom:" <<
    endl;

  ++gIndenter;

  printAtomWithValueEssentials (
    os, fieldWidth);

  os << left <<
    setw (fieldWidth) <<
    "fVariableName" << " : " <<
    fVariableName <<
    setw (fieldWidth) <<
    "fStringSetVariable" << " : '" <<
    endl;

  if (! fStringSetVariable.size ()) {
    os << "none";
  }
  else {
    map<string, int>::const_iterator
      iBegin = fStringSetVariable.begin (),
      iEnd   = fStringSetVariable.end (),
      i      = iBegin;
    for ( ; ; ) {
      os << (*i).first << " --> " << (*i).second;
      if (++i == iEnd) break;
      os << endl;
    } // for
  }

  os << endl;

  --gIndenter;
}

void lilypondBreakPageAfterMeasureNumberAtom::printAtomWithValueOptionsValues (
  ostream&     os,
  unsigned int valueFieldWidth) const
{
  os << left <<
    setw (valueFieldWidth) <<
    fVariableName <<
    " : ";

  if (! fStringSetVariable.size ()) {
    os <<
      "none" <<
      endl;
  }
  else {
    os << endl;
    ++gIndenter;

    map<string, int>::const_iterator
      iBegin = fStringSetVariable.begin (),
      iEnd   = fStringSetVariable.end (),
      i      = iBegin;
    for ( ; ; ) {
      os <<
        "\"" <<
        (*i).first <<
        "\" --> \"" <<
        (*i).second <<
        "\"" <<
        ", variableHasBeenSet: " <<
        booleanAsString (fVariableHasBeenSet) <<
        endl;
      if (++i == iEnd) break;
    } // for

    --gIndenter;
  }
}

ostream& operator<< (ostream& os, const S_lilypondBreakPageAfterMeasureNumberAtom& elt)
{
  elt->print (os);
  return os;
}
*/

