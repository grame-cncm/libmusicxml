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

#include "setTraceOahIfDesired.h"
#ifdef TRACE_OAH
  #include "traceOah.h"
#endif

#include "messagesHandling.h"

#include "generalOah.h"

#include "msrOah.h"
#include "msr2LpsrOah.h"


using namespace std;

namespace MusicXML2
{

//______________________________________________________________________________
S_msr2LpsrScoreOutputKindAtom msr2LpsrScoreOutputKindAtom::create (
  string               shortName,
  string               longName,
  string               description,
  string               valueSpecification,
  string               variableName,
  lpsrScoreOutputKind& lpsrScoreOutputKindVariable)
{
  msr2LpsrScoreOutputKindAtom* o = new
    msr2LpsrScoreOutputKindAtom (
      shortName,
      longName,
      description,
      valueSpecification,
      variableName,
      lpsrScoreOutputKindVariable);
  assert(o!=0);
  return o;
}

msr2LpsrScoreOutputKindAtom::msr2LpsrScoreOutputKindAtom (
  string               shortName,
  string               longName,
  string               description,
  string               valueSpecification,
  string               variableName,
  lpsrScoreOutputKind& lpsrScoreOutputKindVariable)
  : oahValuedAtom (
      shortName,
      longName,
      description,
      valueSpecification,
      variableName),
    fLpsrScoreOutputKindVariable (
      lpsrScoreOutputKindVariable)
{}

msr2LpsrScoreOutputKindAtom::~msr2LpsrScoreOutputKindAtom ()
{}

S_oahValuedAtom msr2LpsrScoreOutputKindAtom::handleOptionUnderName (
  string   optionName,
  ostream& os)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceOah) {
    gLogOstream <<
      "==> option '" << optionName << "' is a msr2LpsrScoreOutputKindAtom" <<
      endl;
  }
#endif

  // an option value is needed
  return this;
}

void msr2LpsrScoreOutputKindAtom::handleValue (
  string   theString,
  ostream& os)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceOah) {
    os <<
      "==> oahAtom is of type 'msr2LpsrScoreOutputKindAtom'" <<
      endl;
  }
#endif

  // theString contains the score output kind:
  // is it in the score output kinds map?

#ifdef TRACE_OAH
  if (gTraceOah->fTraceOah) {
    os <<
      "==> oahAtom is of type 'msr2LpsrScoreOutputKindAtom'" <<
      endl;
  }
#endif

  map<string, lpsrScoreOutputKind>::const_iterator
    it =
      gLpsrScoreOutputKindsMap.find (
        theString);

  if (it == gLpsrScoreOutputKindsMap.end ()) {
    // no, score output kind is unknown in the map

    stringstream s;

    s <<
      "LPSR score output kind '" << theString <<
      "' is unknown" <<
      endl <<
      "The " <<
      gLpsrScoreOutputKindsMap.size () <<
      " known LPSR score output kinds are:" <<
      endl;

    gIndenter++;

    s <<
      existingLpsrScoreOutputKinds (K_NAMES_LIST_MAX_LENGTH);

    gIndenter--;

    oahError (s.str ());
  }

  setLpsrScoreOutputKindVariable (
    (*it).second);
}

void msr2LpsrScoreOutputKindAtom::acceptIn (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    gLogOstream <<
      ".\\\" ==> msr2LpsrScoreOutputKindAtom::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_msr2LpsrScoreOutputKindAtom>*
    p =
      dynamic_cast<visitor<S_msr2LpsrScoreOutputKindAtom>*> (v)) {
        S_msr2LpsrScoreOutputKindAtom elem = this;

#ifdef TRACE_OAH
        if (gOahOah->fTraceOahVisitors) {
          gLogOstream <<
            ".\\\" ==> Launching msr2LpsrScoreOutputKindAtom::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void msr2LpsrScoreOutputKindAtom::acceptOut (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    gLogOstream <<
      ".\\\" ==> msr2LpsrScoreOutputKindAtom::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_msr2LpsrScoreOutputKindAtom>*
    p =
      dynamic_cast<visitor<S_msr2LpsrScoreOutputKindAtom>*> (v)) {
        S_msr2LpsrScoreOutputKindAtom elem = this;

#ifdef TRACE_OAH
        if (gOahOah->fTraceOahVisitors) {
          gLogOstream <<
            ".\\\" ==> Launching msr2LpsrScoreOutputKindAtom::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void msr2LpsrScoreOutputKindAtom::browseData (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    gLogOstream <<
      ".\\\" ==> msr2LpsrScoreOutputKindAtom::browseData ()" <<
      endl;
  }
#endif
}

string msr2LpsrScoreOutputKindAtom::asShortNamedOptionString () const
{
  stringstream s;

  s <<
    "-" << fShortName << " " <<
    lpsrScoreOutputKindAsString (fLpsrScoreOutputKindVariable);

  return s.str ();
}

string msr2LpsrScoreOutputKindAtom::asActualLongNamedOptionString () const
{
  stringstream s;

  s <<
    "-" << fLongName << " " <<
    lpsrScoreOutputKindAsString (fLpsrScoreOutputKindVariable);

  return s.str ();
}

void msr2LpsrScoreOutputKindAtom::print (ostream& os) const
{
  const int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "msr2LpsrScoreOutputKindAtom:" <<
    endl;

  gIndenter++;

  printValuedAtomEssentials (
    os, fieldWidth);

  os << left <<
    setw (fieldWidth) <<
    "fVariableName" << " : " <<
    fVariableName <<
    endl <<
    setw (fieldWidth) <<
    "fOptionsLpsrScoreOutputKindVariable" << " : \"" <<
    lpsrScoreOutputKindAsString (
      fLpsrScoreOutputKindVariable) <<
    "\"" <<
    endl;
}

void msr2LpsrScoreOutputKindAtom::printAtomOptionsValues (
  ostream& os,
  int      valueFieldWidth) const
{
  os << left <<
    setw (valueFieldWidth) <<
    fVariableName <<
    " : \"" <<
    lpsrScoreOutputKindAsString (
      fLpsrScoreOutputKindVariable) <<
    "\"" <<
    endl;
}

ostream& operator<< (ostream& os, const S_msr2LpsrScoreOutputKindAtom& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_msrRenamePartAtom msrRenamePartAtom::create (
  string               shortName,
  string               longName,
  string               description,
  string               valueSpecification,
  string               variableName,
  map<string, string>& stringStringMapVariable)
{
  msrRenamePartAtom* o = new
    msrRenamePartAtom (
      shortName,
      longName,
      description,
      valueSpecification,
      variableName,
      stringStringMapVariable);
  assert(o!=0);
  return o;
}

msrRenamePartAtom::msrRenamePartAtom (
  string               shortName,
  string               longName,
  string               description,
  string               valueSpecification,
  string               variableName,
  map<string, string>& stringStringMapVariable)
  : oahValuedAtom (
      shortName,
      longName,
      description,
      valueSpecification,
      variableName),
    fStringStringMapVariable (
      stringStringMapVariable)
{
  fMultipleOccurrencesAllowed = true;
}

msrRenamePartAtom::~msrRenamePartAtom ()
{}

S_oahValuedAtom msrRenamePartAtom::handleOptionUnderName (
  string   optionName,
  ostream& os)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceOah) {
    gLogOstream <<
      "==> option '" << optionName << "' is a msrRenamePartAtom" <<
      endl;
  }
#endif

  // an option value is needed
  return this;
}

void msrRenamePartAtom::handleValue (
  string   theString,
  ostream& os)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceOah) {
    os <<
      "==> oahAtom is of type 'msrRenamePartAtom'" <<
      endl;
  }
#endif

  // theString contains the part rename specification
  // decipher it to extract the old and new part names

#ifdef TRACE_OAH
  if (gTraceOah->fTraceOah) {
    os <<
      "==> oahAtom is of type 'msrRenamePartAtom'" <<
      endl;
  }
#endif

  string regularExpression (
    "[[:space:]]*([^[:space:]]*)[[:space:]]*"
    "="
    "[[:space:]]*([^[:space:]]*)[[:space:]]*");

  regex  e (regularExpression);
  smatch sm;

  regex_match (theString, sm, e);

  unsigned smSize = sm.size ();

#ifdef TRACE_OAH
  if (gTraceOah->fTraceOah) {
    os <<
      "There are " << smSize << " matches" <<
      " for part rename string '" << theString <<
      "' with regex '" << regularExpression <<
      "'" <<
      endl;
  }
#endif

  if (smSize == 3) {
#ifdef TRACE_OAH
    if (gTraceOah->fTraceOah) {
      for (unsigned i = 0; i < smSize; ++i) {
        os <<
          "[" << sm [i] << "] ";
      } // for
      os << endl;
    }
#endif
  }

  else {
    stringstream s;

    s <<
      "-msrPartRename argument '" << theString <<
      "' is ill-formed";

    oahError (s.str ());
  }

  string
    oldPartName = sm [1],
    newPartName = sm [2];

#ifdef TRACE_OAH
  if (gTraceOah->fTraceOah) {
    os <<
      "--> oldPartName = \"" << oldPartName << "\", " <<
      "--> newPartName = \"" << newPartName << "\"" <<
      endl;
  }
#endif

  // is this part name in the part renaming map?
  map<string, string>::iterator
    it =
      fStringStringMapVariable.find (oldPartName);

  if (it != fStringStringMapVariable.end ()) {
    // yes, issue error message
    stringstream s;

    s <<
      "Part \"" << oldPartName << "\" occurs more that once" <<
      "in the '--rename-part' option";

    oahError (s.str ());
  }

  else {
    fStringStringMapVariable [oldPartName] = newPartName;
  }
}

void msrRenamePartAtom::acceptIn (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    gLogOstream <<
      ".\\\" ==> msrRenamePartAtom::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_msrRenamePartAtom>*
    p =
      dynamic_cast<visitor<S_msrRenamePartAtom>*> (v)) {
        S_msrRenamePartAtom elem = this;

#ifdef TRACE_OAH
        if (gOahOah->fTraceOahVisitors) {
          gLogOstream <<
            ".\\\" ==> Launching msrRenamePartAtom::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void msrRenamePartAtom::acceptOut (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    gLogOstream <<
      ".\\\" ==> msrRenamePartAtom::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_msrRenamePartAtom>*
    p =
      dynamic_cast<visitor<S_msrRenamePartAtom>*> (v)) {
        S_msrRenamePartAtom elem = this;

#ifdef TRACE_OAH
        if (gOahOah->fTraceOahVisitors) {
          gLogOstream <<
            ".\\\" ==> Launching msrRenamePartAtom::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void msrRenamePartAtom::browseData (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    gLogOstream <<
      ".\\\" ==> msrRenamePartAtom::browseData ()" <<
      endl;
  }
#endif
}

string msrRenamePartAtom::asShortNamedOptionString () const
{
  stringstream s;

  s <<
    "-" << fShortName << " ";

  if (! fStringStringMapVariable.size ()) {
    s << "none";
  }
  else {
    map<string, string>::const_iterator
      iBegin = fStringStringMapVariable.begin (),
      iEnd   = fStringStringMapVariable.end (),
      i      = iBegin;
    for ( ; ; ) {
      s << (*i).first << "=" << (*i).second;
      if (++i == iEnd) break;
      s << ",";
    } // for
  }

  return s.str ();
}

string msrRenamePartAtom::asActualLongNamedOptionString () const
{
  stringstream s;

  s <<
    "-" << fLongName << " ";

  if (! fStringStringMapVariable.size ()) {
    s << "none";
  }
  else {
    map<string, string>::const_iterator
      iBegin = fStringStringMapVariable.begin (),
      iEnd   = fStringStringMapVariable.end (),
      i      = iBegin;
    for ( ; ; ) {
      s << (*i).first << "=" << (*i).second;
      if (++i == iEnd) break;
      s << ",";
    } // for
  }

  return s.str ();
}

void msrRenamePartAtom::print (ostream& os) const
{
  const int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "msrRenamePartAtom:" <<
    endl;

  gIndenter++;

  printValuedAtomEssentials (
    os, fieldWidth);

  os << left <<
    setw (fieldWidth) <<
    "fVariableName" << " : " <<
    fVariableName <<
    setw (fieldWidth) <<
    "fStringStringMapVariable" << " : " <<
    endl;

  if (! fStringStringMapVariable.size ()) {
    os << "none";
  }
  else {
    map<string, string>::const_iterator
      iBegin = fStringStringMapVariable.begin (),
      iEnd   = fStringStringMapVariable.end (),
      i      = iBegin;
    for ( ; ; ) {
      os << (*i).first << " --> " << (*i).second;
      if (++i == iEnd) break;
      os << endl;
    } // for
  }

  os << endl;
}

void msrRenamePartAtom::printAtomOptionsValues (
  ostream& os,
  int      valueFieldWidth) const
{
  os << left <<
    setw (valueFieldWidth) <<
    fVariableName <<
    " : ";

  if (! fStringStringMapVariable.size ()) {
    os <<
      "none" <<
      endl;
  }
  else {
    os << endl;
    gIndenter++;

    map<string, string>::const_iterator
      iBegin = fStringStringMapVariable.begin (),
      iEnd   = fStringStringMapVariable.end (),
      i      = iBegin;
    for ( ; ; ) {
      os <<
        "\"" <<
        (*i).first <<
        "\" --> \"" <<
        (*i).second <<
        "\"" <<
        endl;
      if (++i == iEnd) break;
    } // for

    gIndenter--;
  }
}

ostream& operator<< (ostream& os, const S_msrRenamePartAtom& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_msrTransposePartAtom msrTransposePartAtom::create (
  string             shortName,
  string             longName,
  string             description,
  string             valueSpecification,
  string             variableName,
  map<string, S_msrSemiTonesPitchAndOctave>&
                     stringMsrSemiTonesPitchAndOctaveVariable)
{
  msrTransposePartAtom* o = new
    msrTransposePartAtom (
      shortName,
      longName,
      description,
      valueSpecification,
      variableName,
      stringMsrSemiTonesPitchAndOctaveVariable);
  assert(o!=0);
  return o;
}

msrTransposePartAtom::msrTransposePartAtom (
  string             shortName,
  string             longName,
  string             description,
  string             valueSpecification,
  string             variableName,
  map<string, S_msrSemiTonesPitchAndOctave>&
                     stringMsrSemiTonesPitchAndOctaveVariable)
  : oahValuedAtom (
      shortName,
      longName,
      description,
      valueSpecification,
      variableName),
    fStringMsrSemiTonesPitchAndOctaveVariable (
      stringMsrSemiTonesPitchAndOctaveVariable)
{}

msrTransposePartAtom::~msrTransposePartAtom ()
{}

S_oahValuedAtom msrTransposePartAtom::handleOptionUnderName (
  string   optionName,
  ostream& os)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceOah) {
    gLogOstream <<
      "==> option '" << optionName << "' is a msrTransposePartAtom" <<
      endl;
  }
#endif

  // an option value is needed
  return this;
}

void msrTransposePartAtom::handleValue (
  string   theString,
  ostream& os)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceOah) {
    os <<
      "==> oahAtom is of type 'msrTransposePartAtom'" <<
      endl;
  }
#endif

  // theString contains the part transpose specification
  // decipher it to extract the old and new part names

  string regularExpression (
    "(.*)"
    "="
    "(.*)");
//    "[[:space:]]*(.*)[[:space:]]*" JMI
//    "="
//    "[[:space:]]*(.*)[[:space:]]*");

  regex  e (regularExpression);
  smatch sm;

  regex_match (theString, sm, e);

  unsigned smSize = sm.size ();

#ifdef TRACE_OAH
  if (gTraceOah->fTraceOah) {
    os <<
      "There are " << smSize << " matches" <<
      " for part transpose string '" << theString <<
      "' with regex '" << regularExpression <<
      "'" <<
      endl;
  }
#endif

  if (smSize == 3) {
#ifdef TRACE_OAH
    if (gTraceOah->fTraceOah) {
      for (unsigned i = 0; i < smSize; ++i) {
        os <<
          "[" << sm [i] << "] ";
      } // for
      os << endl;
    }
#endif
  }

  else {
    stringstream s;

    s <<
      "-marTransposePart argument '" << theString <<
      "' is ill-formed";

    oahError (s.str ());
  }

  string
    originalPitchName    = sm [1],
    destinationPitchName = sm [2];

#ifdef TRACE_OAH
  if (gTraceOah->fTraceOah) {
    os <<
      "--> originalPitchName = \"" << originalPitchName << "\", " <<
      "--> destinationPitchName = \"" << destinationPitchName << "\"" <<
      endl;
  }
#endif

  // is this part name in the part renaming map?
  map<string, S_msrSemiTonesPitchAndOctave>::iterator
    it =
      fStringMsrSemiTonesPitchAndOctaveVariable.find (originalPitchName);

  if (it != fStringMsrSemiTonesPitchAndOctaveVariable.end ()) {
    // yes, issue error message
    stringstream s;

    s <<
      "Part \"" << originalPitchName << "\" occurs more that once" <<
      "in the '--transpose-part' option";

    oahError (s.str ());
  }

  else {
    fStringMsrSemiTonesPitchAndOctaveVariable [originalPitchName] =
      msrSemiTonesPitchAndOctave::createFromString (
        K_NO_INPUT_LINE_NUMBER,
        destinationPitchName);
  }
}

void msrTransposePartAtom::acceptIn (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    gLogOstream <<
      ".\\\" ==> msrTransposePartAtom::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_msrTransposePartAtom>*
    p =
      dynamic_cast<visitor<S_msrTransposePartAtom>*> (v)) {
        S_msrTransposePartAtom elem = this;

#ifdef TRACE_OAH
        if (gOahOah->fTraceOahVisitors) {
          gLogOstream <<
            ".\\\" ==> Launching msrTransposePartAtom::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void msrTransposePartAtom::acceptOut (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    gLogOstream <<
      ".\\\" ==> msrTransposePartAtom::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_msrTransposePartAtom>*
    p =
      dynamic_cast<visitor<S_msrTransposePartAtom>*> (v)) {
        S_msrTransposePartAtom elem = this;

#ifdef TRACE_OAH
        if (gOahOah->fTraceOahVisitors) {
          gLogOstream <<
            ".\\\" ==> Launching msrTransposePartAtom::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void msrTransposePartAtom::browseData (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    gLogOstream <<
      ".\\\" ==> msrTransposePartAtom::browseData ()" <<
      endl;
  }
#endif
}

string msrTransposePartAtom::asShortNamedOptionString () const
{
  stringstream s;

  if (fStringMsrSemiTonesPitchAndOctaveVariable.size ()) {
    s <<
      "-" << fShortName << " \"";

    map<string, S_msrSemiTonesPitchAndOctave>::const_iterator
      iBegin = fStringMsrSemiTonesPitchAndOctaveVariable.begin (),
      iEnd   = fStringMsrSemiTonesPitchAndOctaveVariable.end (),
      i      = iBegin;
    for ( ; ; ) {
      s <<
        (*i).first << "=" <<
        msrSemiTonesPitchAndOctaveAsLilypondString (
          gMsrOah->fMsrQuarterTonesPitchesLanguageKind,
          (*i).second);
      if (++i == iEnd) break;
    } // for

    s << "\"";
  }

  return s.str ();
}

string msrTransposePartAtom::asActualLongNamedOptionString () const
{
  stringstream s;

  if (fStringMsrSemiTonesPitchAndOctaveVariable.size ()) {
    s <<
      "-" << fShortName << " \"";

    map<string, S_msrSemiTonesPitchAndOctave>::const_iterator
      iBegin = fStringMsrSemiTonesPitchAndOctaveVariable.begin (),
      iEnd   = fStringMsrSemiTonesPitchAndOctaveVariable.end (),
      i      = iBegin;
    for ( ; ; ) {
      s <<
        (*i).first << "=" <<
        msrSemiTonesPitchAndOctaveAsLilypondString (
          gMsrOah->fMsrQuarterTonesPitchesLanguageKind,
          (*i).second);
      if (++i == iEnd) break;
    } // for

    s << "\"";
  }

  return s.str ();
}

void msrTransposePartAtom::print (ostream& os) const
{
  const int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "msrTransposePartAtom:" <<
    endl;

  gIndenter++;

  printValuedAtomEssentials (
    os, fieldWidth);

  os << left <<
    setw (fieldWidth) <<
    "fVariableName" << " : " <<
    fVariableName <<
    setw (fieldWidth) <<
    "fStringMsrSemiTonesPitchAndOctaveVariable" << " : " <<
    endl;

  if (! fStringMsrSemiTonesPitchAndOctaveVariable.size ()) {
    os << "none";
  }
  else {
    map<string, S_msrSemiTonesPitchAndOctave>::const_iterator
      iBegin = fStringMsrSemiTonesPitchAndOctaveVariable.begin (),
      iEnd   = fStringMsrSemiTonesPitchAndOctaveVariable.end (),
      i      = iBegin;
    for ( ; ; ) {
      os << (*i).first << " --> " << (*i).second;
      if (++i == iEnd) break;
      os << endl;
    } // for
  }

  os << endl;
}

void msrTransposePartAtom::printAtomOptionsValues (
  ostream& os,
  int      valueFieldWidth) const
{
  os << left <<
    setw (valueFieldWidth) <<
    fVariableName <<
    " : ";

  if (! fStringMsrSemiTonesPitchAndOctaveVariable.size ()) {
    os <<
      "none" <<
      endl;
  }
  else {
    os << endl;

    gIndenter++;

    map<string, S_msrSemiTonesPitchAndOctave>::const_iterator
      iBegin = fStringMsrSemiTonesPitchAndOctaveVariable.begin (),
      iEnd   = fStringMsrSemiTonesPitchAndOctaveVariable.end (),
      i      = iBegin;
    for ( ; ; ) {
      os <<
        "\"" <<
        (*i).first <<
        "\" --> \"" <<
        (*i).second <<
        "\"" <<
        endl;
      if (++i == iEnd) break;
    } // for

    gIndenter--;
  }
}

ostream& operator<< (ostream& os, const S_msrTransposePartAtom& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_msrOmitPartAtom msrOmitPartAtom::create (
  string       shortName,
  string       longName,
  string       description,
  string       valueSpecification,
  string       variableName,
  set<string>& stringSetVariable)
{
  msrOmitPartAtom* o = new
    msrOmitPartAtom (
      shortName,
      longName,
      description,
      valueSpecification,
      variableName,
      stringSetVariable);
  assert(o!=0);
  return o;
}

msrOmitPartAtom::msrOmitPartAtom (
  string       shortName,
  string       longName,
  string       description,
  string       valueSpecification,
  string       variableName,
  set<string>& stringSetVariable)
  : oahValuedAtom (
      shortName,
      longName,
      description,
      valueSpecification,
      variableName),
    fStringSetVariable (
      stringSetVariable)
{
  fMultipleOccurrencesAllowed = true;
}

msrOmitPartAtom::~msrOmitPartAtom ()
{}

S_oahValuedAtom msrOmitPartAtom::handleOptionUnderName (
  string   optionName,
  ostream& os)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceOah) {
    gLogOstream <<
      "==> option '" << optionName << "' is a msrOmitPartAtom" <<
      endl;
  }
#endif

  // an option value is needed
  return this;
}

void msrOmitPartAtom::handleValue (
  string   theString,
  ostream& os)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceOah) {
    os <<
      "==> oahAtom is of type 'msrOmitPartAtom'" <<
      endl;
  }
#endif

  // theString contains the name of the part to be omitted

  string partName = theString;

#ifdef TRACE_OAH
  if (gTraceOah->fTraceOah) {
    os <<
      "--> partName = \"" << partName << "\", " <<
      endl;
  }
#endif

  // is this part name in the part renaming map?
  set<string>::iterator
    it =
      fStringSetVariable.find (partName);

  if (it != fStringSetVariable.end ()) {
    // yes, issue error message
    stringstream s;

    s <<
      "Part \"" << partName << "\" occurs more that once" <<
      "in the '--msr-omit-part' option";

    oahError (s.str ());
  }

  else {
    fStringSetVariable.insert (partName);
  }
}

void msrOmitPartAtom::acceptIn (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    gLogOstream <<
      ".\\\" ==> msrOmitPartAtom::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_msrOmitPartAtom>*
    p =
      dynamic_cast<visitor<S_msrOmitPartAtom>*> (v)) {
        S_msrOmitPartAtom elem = this;

#ifdef TRACE_OAH
        if (gOahOah->fTraceOahVisitors) {
          gLogOstream <<
            ".\\\" ==> Launching msrOmitPartAtom::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void msrOmitPartAtom::acceptOut (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    gLogOstream <<
      ".\\\" ==> msrOmitPartAtom::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_msrOmitPartAtom>*
    p =
      dynamic_cast<visitor<S_msrOmitPartAtom>*> (v)) {
        S_msrOmitPartAtom elem = this;

#ifdef TRACE_OAH
        if (gOahOah->fTraceOahVisitors) {
          gLogOstream <<
            ".\\\" ==> Launching msrOmitPartAtom::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void msrOmitPartAtom::browseData (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    gLogOstream <<
      ".\\\" ==> msrOmitPartAtom::browseData ()" <<
      endl;
  }
#endif
}

string msrOmitPartAtom::asShortNamedOptionString () const
{
  stringstream s;

  s <<
    "-" << fShortName << " ";

  if (! fStringSetVariable.size ()) {
    s << "none";
  }
  else {
    set<string>::const_iterator
      iBegin = fStringSetVariable.begin (),
      iEnd   = fStringSetVariable.end (),
      i      = iBegin;
    for ( ; ; ) {
      s << (*i);
      if (++i == iEnd) break;
      s << ",";
    } // for
  }

  return s.str ();
}

string msrOmitPartAtom::asActualLongNamedOptionString () const
{
  stringstream s;

  s <<
    "-" << fLongName << " ";

  if (! fStringSetVariable.size ()) {
    s << "none";
  }
  else {
    set<string>::const_iterator
      iBegin = fStringSetVariable.begin (),
      iEnd   = fStringSetVariable.end (),
      i      = iBegin;
    for ( ; ; ) {
      s << (*i);
      if (++i == iEnd) break;
      s << ",";
    } // for
  }

  return s.str ();
}

void msrOmitPartAtom::print (ostream& os) const
{
  const int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "msrOmitPartAtom:" <<
    endl;

  gIndenter++;

  printValuedAtomEssentials (
    os, fieldWidth);

  os << left <<
    setw (fieldWidth) <<
    "fVariableName" << " : " <<
    fVariableName <<
    setw (fieldWidth) <<
    "fStringSetVariable" << " : " <<
    endl;

  if (! fStringSetVariable.size ()) {
    os << "none";
  }
  else {
    set<string>::const_iterator
      iBegin = fStringSetVariable.begin (),
      iEnd   = fStringSetVariable.end (),
      i      = iBegin;
    for ( ; ; ) {
      os << (*i);
      if (++i == iEnd) break;
      os << endl;
    } // for
  }

  os << endl;
}

void msrOmitPartAtom::printAtomOptionsValues (
  ostream& os,
  int      valueFieldWidth) const
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

    gIndenter++;

    set<string>::const_iterator
      iBegin = fStringSetVariable.begin (),
      iEnd   = fStringSetVariable.end (),
      i      = iBegin;
    for ( ; ; ) {
      os <<
        "\"" << (*i) << "\"" <<
        endl;
      if (++i == iEnd) break;
    } // for

    gIndenter--;
  }
}

ostream& operator<< (ostream& os, const S_msrOmitPartAtom& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_msrKeepPartAtom msrKeepPartAtom::create (
  string       shortName,
  string       longName,
  string       description,
  string       valueSpecification,
  string       variableName,
  set<string>& stringSetVariable)
{
  msrKeepPartAtom* o = new
    msrKeepPartAtom (
      shortName,
      longName,
      description,
      valueSpecification,
      variableName,
      stringSetVariable);
  assert(o!=0);
  return o;
}

msrKeepPartAtom::msrKeepPartAtom (
  string       shortName,
  string       longName,
  string       description,
  string       valueSpecification,
  string       variableName,
  set<string>& stringSetVariable)
  : oahValuedAtom (
      shortName,
      longName,
      description,
      valueSpecification,
      variableName),
    fStringSetVariable (
      stringSetVariable)
{
  fMultipleOccurrencesAllowed = true;
}

msrKeepPartAtom::~msrKeepPartAtom ()
{}

S_oahValuedAtom msrKeepPartAtom::handleOptionUnderName (
  string   optionName,
  ostream& os)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceOah) {
    gLogOstream <<
      "==> option '" << optionName << "' is a msrKeepPartAtom" <<
      endl;
  }
#endif

  // an option value is needed
  return this;
}

void msrKeepPartAtom::handleValue (
  string   theString,
  ostream& os)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceOah) {
    os <<
      "==> oahAtom is of type 'msrKeepPartAtom'" <<
      endl;
  }
#endif

  // theString contains the name of the part to be kept

  string partName = theString;

#ifdef TRACE_OAH
  if (gTraceOah->fTraceOah) {
    os <<
      "--> partName = \"" << partName << "\", " <<
      endl;
  }
#endif

  // is this part name in the part renaming map?
  set<string>::iterator
    it =
      fStringSetVariable.find (partName);

  if (it != fStringSetVariable.end ()) {
    // yes, issue error message
    stringstream s;

    s <<
      "Part \"" << partName << "\" occurs more that once" <<
      "in the '--msr-keep-part' option";

    oahError (s.str ());
  }

  else {
    fStringSetVariable.insert (partName);
  }
}

void msrKeepPartAtom::acceptIn (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    gLogOstream <<
      ".\\\" ==> msrKeepPartAtom::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_msrKeepPartAtom>*
    p =
      dynamic_cast<visitor<S_msrKeepPartAtom>*> (v)) {
        S_msrKeepPartAtom elem = this;

#ifdef TRACE_OAH
        if (gOahOah->fTraceOahVisitors) {
          gLogOstream <<
            ".\\\" ==> Launching msrKeepPartAtom::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void msrKeepPartAtom::acceptOut (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    gLogOstream <<
      ".\\\" ==> msrKeepPartAtom::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_msrKeepPartAtom>*
    p =
      dynamic_cast<visitor<S_msrKeepPartAtom>*> (v)) {
        S_msrKeepPartAtom elem = this;

#ifdef TRACE_OAH
        if (gOahOah->fTraceOahVisitors) {
          gLogOstream <<
            ".\\\" ==> Launching msrKeepPartAtom::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void msrKeepPartAtom::browseData (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    gLogOstream <<
      ".\\\" ==> msrKeepPartAtom::browseData ()" <<
      endl;
  }
#endif
}

string msrKeepPartAtom::asShortNamedOptionString () const
{
  stringstream s;

  s <<
    "-" << fShortName << " ";

  if (! fStringSetVariable.size ()) {
    s << "none";
  }
  else {
    set<string>::const_iterator
      iBegin = fStringSetVariable.begin (),
      iEnd   = fStringSetVariable.end (),
      i      = iBegin;
    for ( ; ; ) {
      s << (*i);
      if (++i == iEnd) break;
      s << ",";
    } // for
  }

  return s.str ();
}

string msrKeepPartAtom::asActualLongNamedOptionString () const
{
  stringstream s;

  s <<
    "-" << fLongName << " ";

  if (! fStringSetVariable.size ()) {
    s << "none";
  }
  else {
    set<string>::const_iterator
      iBegin = fStringSetVariable.begin (),
      iEnd   = fStringSetVariable.end (),
      i      = iBegin;
    for ( ; ; ) {
      s << (*i);
      if (++i == iEnd) break;
      s << ",";
    } // for
  }

  return s.str ();
}

void msrKeepPartAtom::print (ostream& os) const
{
  const int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "msrKeepPartAtom:" <<
    endl;

  gIndenter++;

  printValuedAtomEssentials (
    os, fieldWidth);

  os << left <<
    setw (fieldWidth) <<
    "fVariableName" << " : " <<
    fVariableName <<
    setw (fieldWidth) <<
    "fStringSetVariable" << " : " <<
    endl;

  if (! fStringSetVariable.size ()) {
    os << "none";
  }
  else {
    set<string>::const_iterator
      iBegin = fStringSetVariable.begin (),
      iEnd   = fStringSetVariable.end (),
      i      = iBegin;
    for ( ; ; ) {
      os << (*i);
      if (++i == iEnd) break;
      os << endl;
    } // for
  }

  os << endl;
}

void msrKeepPartAtom::printAtomOptionsValues (
  ostream& os,
  int      valueFieldWidth) const
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

    gIndenter++;

    set<string>::const_iterator
      iBegin = fStringSetVariable.begin (),
      iEnd   = fStringSetVariable.end (),
      i      = iBegin;
    for ( ; ; ) {
      os <<
        "\"" << (*i) << "\"" <<
        endl;
      if (++i == iEnd) break;
    } // for

    gIndenter--;
  }
}

ostream& operator<< (ostream& os, const S_msrKeepPartAtom& elt)
{
  elt->print (os);
  return os;
}

//_______________________________________________________________________________

S_msr2LpsrOah gMsr2LpsrOah;
S_msr2LpsrOah gMsr2LpsrOahUserChoices;
S_msr2LpsrOah gMsr2LpsrOahWithDetailedTrace;

S_msr2LpsrOah msr2LpsrOah::create (
  S_oahHandler handlerUpLink)
{
  msr2LpsrOah* o = new msr2LpsrOah(
    handlerUpLink);
  assert(o!=0);
  return o;
}

msr2LpsrOah::msr2LpsrOah (
  S_oahHandler handlerUpLink)
  : oahGroup (
    "Msr2LPSR",
    "hmsr2lpsr", "help-msr-to-lpsr",
R"(These options control the way MSR data is translated to LPSR.)",
    kElementVisibilityAlways,
    handlerUpLink)
{
/* JMI
  // sanity check
  msrAssert (
    handlerUpLink != nullptr,
    "handlerUpLink is null");
*/

  // append this options group to the options handler
  // if relevant
  if (handlerUpLink) {
    handlerUpLink->
      appendGroupToHandler (this);
  }

  // initialize it
  initializeMsr2LpsrOah (false);
}

msr2LpsrOah::~msr2LpsrOah ()
{}

#ifdef TRACE_OAH
void msr2LpsrOah::initializeMsr2LpsrTraceOah (
  bool boolOptionsInitialValue)
{}
#endif

void msr2LpsrOah::initializeMsr2LpsrScoreOutputOptions (
  bool boolOptionsInitialValue)
{
  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "LilyPond score output",
        "hlpso", "help-lilypond-score-output",
R"()",
      kElementVisibilityAlways,
      this);

  appendSubGroupToGroup (subGroup);

  // LPSR score output kind

  const lpsrScoreOutputKind
    lpsrScoreOutputKindDefaultValue =
      kScoreOnly; // default value

  fScoreOutputKind = lpsrScoreOutputKindDefaultValue;

  subGroup->
    appendAtomToSubGroup (
      msr2LpsrScoreOutputKindAtom::create (
        "m2lpsok", "msr2lpsr-score-output-kind",
        regex_replace (
          regex_replace (
            regex_replace (
R"(Use OUTPUT_KIND to create the LPSR blocks,
as well as in the generated LilyPond code.
The NUMBER LilyPond score output kinds available are:
  OUTPUT_KINDS.
'-one-file' means that LilyPond will produce a single file containing all the scores and/or parts.
Otherwise, one file will be generated for each score and/or part.
The default is 'DEFAULT_VALUE'.)",
              regex ("NUMBER"),
              to_string (gLpsrScoreOutputKindsMap.size ())),
            regex ("OUTPUT_KINDS"),
            existingLpsrScoreOutputKinds (K_NAMES_LIST_MAX_LENGTH)),
          regex ("DEFAULT_VALUE"),
          lpsrScoreOutputKindAsString (
            lpsrScoreOutputKindDefaultValue)),
        "OUTPUT_KIND",
        "scoreOutputKind",
        fScoreOutputKind));
}

void msr2LpsrOah::initializeMsr2LpsrPartsOptions (
  bool boolOptionsInitialValue)
{
  S_oahSubGroup subGroup =
    oahSubGroup::create (
      "Parts",
      "hmsrp", "help-msr-parts",
R"()",
    kElementVisibilityAlways,
    this);

  appendSubGroupToGroup (subGroup);

  // MSR rename part

  subGroup->
    appendAtomToSubGroup (
      msrRenamePartAtom::create (
        "mrp", "msr-rename-part",
        regex_replace (
R"(Rename part ORIGINAL_NAME to NEW_NAME, for example after displaying
the names in the score or a summary of the latter in a first run with options
'-dmnames, -display-msr-names' or 'dmsum, -display-msr-summary'.
PART_RENAME_SPEC can be:
'ORIGINAL_NAME = NEW_NAME'
or
"ORIGINAL_NAME = NEW_NAME"
The single or double quotes are used to allow spaces in the names
and around the '=' sign, otherwise they can be dispensed with.
Using double quotes allows for shell variables substitutions, as in:
DESSUS="Cor anglais"
EXECUTABLE -msr-rename-part "P1 = ${DESSUS}" .
There can be several occurrences of this option.)",
         regex ("EXECUTABLE"),
          gOahOah->fHandlerExecutableName),
        "PART_RENAME_SPEC",
        "partsRenamingMap",
        fPartsRenamingMap));

  // MSR transpose part

  subGroup->
    appendAtomToSubGroup (
      msrTransposePartAtom::create (
        "mtp", "msr-transpose-part",
R"(Transpose part ORIGINAL_NAME using TRANSPOSITION to tranpose in the MSR data.
PART_TRANSPOSITION_SPEC can be:
'ORIGINAL_NAME = TRANSPOSITION'
or
"ORIGINAL_NAME = TRANSPOSITION"
The single or double quotes are used to allow spaces in the names
and around the '=' sign, otherwise they can be dispensed with.
TRANSPOSITION should contain a diatonic pitch, followed if needed
by a sequence of ',' or '\'' octave indications.
Such indications cannot be mixed, and they are relative to c\', i.e. middle C.
For example, 'a', 'f' and 'bes,' can be used respectively
for instruments in 'a', 'f' and B flat respectively.
Using double quotes allows for shell variables substitutions, as in:
SAXOPHONE="bes,"
EXECUTABLE -msr-transpose-part "P1 ${SAXOPHONE}" .
There can be several occurrences of this option.)",
        "PART_TRANSPOSITION_SPEC",
        "partsTranspositionMap",
        fPartsTranspositionMap));

  // MSR omit part ID

  fOmitPartIDAtom =
    oahStringSetAtom::create (
      "mopi", "msr-omit-part-id",
R"(Omit part with ID PART_ID.
There can be several occurrences of this option.
All the parts not omitted are kept.
This option is incompatible with '-mkpi, -msr-keep-part-id'.)",
      "PART_ID",
      "partsOmitIDSet",
      fPartsOmitIDSet);

  subGroup->
    appendAtomToSubGroup (
      fOmitPartIDAtom);

  // MSR omit part name

  fOmitPartNameAtom =
    oahStringSetAtom::create (
      "mopn", "msr-omit-part-name",
R"(Omit part named PART_NAME.
There can be several occurrences of this option.
All the parts not omitted are kept.
This option is incompatible with '-mkpn, -msr-keep-part-name'.)",
      "PART_NAME",
      "partsOmitNameSet",
      fPartsOmitNameSet);

  subGroup->
    appendAtomToSubGroup (
      fOmitPartNameAtom);

  // MSR keep part ID

  fKeepPartIDAtom =
    oahStringSetAtom::create (
      "mkpi", "msr-keep-part-id",
R"(Keep part with ID PART_ID.
There can be several occurrences of this option.
All the parts not kept are omitted.
This option is incompatible with '-mopi, -msr-omit-part-id'.)",
      "PART_ID",
      "partsKeepIDSet",
      fPartsKeepIDSet);

  subGroup->
    appendAtomToSubGroup (
      fKeepPartIDAtom);

  // MSR keep part name

  fKeepPartNameAtom =
    oahStringSetAtom::create (
      "mkpn", "msr-keep-part-name",
R"(Keep part named PART_NAME.
There can be several occurrences of this option.
All the parts not kept are omitted.
This option is incompatible with '-mopn, -msr-omit-part-name'.)",
      "PART_NAME",
      "partsKeepNameSet",
      fPartsKeepNameSet);

  subGroup->
    appendAtomToSubGroup (
      fKeepPartNameAtom);
}

void msr2LpsrOah::initializeMsr2LpsrStavesOptions (
  bool boolOptionsInitialValue)
{
  S_oahSubGroup subGroup =
    oahSubGroup::create (
      "Staves",
      "hmsrs", "help-msr-staves",
R"()",
    kElementVisibilityAlways,
    this);

  appendSubGroupToGroup (subGroup);

  // single line staves

  fCreateVoicesStaffRelativeNumbers = boolOptionsInitialValue;

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "cslsar", "create-single-line-staves-as-rythmic",
R"(Create staves with a single line as rythmic staves.
By default, drum staves are created in this case.)",
        "createSingleLineStavesAsRythmic",
        fCreateSingleLineStavesAsRythmic));
}

void msr2LpsrOah::initializeMsr2LpsrVoicesOptions (
  bool boolOptionsInitialValue)
{
  S_oahSubGroup subGroup =
    oahSubGroup::create (
      "Voices",
      "hmsrv", "help-msr-voices",
R"()",
    kElementVisibilityAlways,
    this);

  appendSubGroupToGroup (subGroup);

  // staff relative numbers

  fCreateVoicesStaffRelativeNumbers = boolOptionsInitialValue;

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "cvsrvn", "create-voices-staff-relative-numbers",
R"(Generate voices names with numbers relative to their staff.
By default, the voice numbers found are used,
which may be global to the score.)",
        "createVoicesStaffRelativeNumbers",
        fCreateVoicesStaffRelativeNumbers));
}

void msr2LpsrOah::initializeMsr2LpsrRepeatsOptions (
  bool boolOptionsInitialValue)
{
  S_oahSubGroup subGroup =
    oahSubGroup::create (
      "Repeats",
      "hmsrr", "help-msr-repeats",
R"()",
    kElementVisibilityAlways,
    this);

  appendSubGroupToGroup (subGroup);

  // barlines

  fCreateImplicitInitialRepeatBarline = boolOptionsInitialValue;

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "ciirb", "create-implicit-initial-repeat-barline",
R"(Create an implicit repeat barline at the beginning of the stave
in case there is none, as is usual in scores.
By default, no such barline is added.)",
        "createImplicitInitialRepeatBarline",
        fCreateImplicitInitialRepeatBarline));
}

void msr2LpsrOah::initializeMsr2LpsrNotesOptions (
  bool boolOptionsInitialValue)
{
  S_oahSubGroup subGroup =
    oahSubGroup::create (
      "Notes",
      "hmsrn", "help-msr-notes",
R"()",
    kElementVisibilityAlways,
    this);

  appendSubGroupToGroup (subGroup);

  // the 'dr' prefix

  S_oahPrefix
    shortDelayRestsPrefix =
      oahPrefix::create (
        "dr",
        "dr",
        "'-dr=abc,yz' is equivalent to '-drabc, -dryz'");
  fHandlerUpLink->
    appendPrefixToHandler (shortDelayRestsPrefix);

  // the 'delay-rests' prefix

  S_oahPrefix
    longDelayRestsPrefix =
      oahPrefix::create (
        "delay-rests",
        "delay-rests-",
        "'-delay-rests=abc,yz' is equivalent to '-delay-rests-abc, -delay-rests-yz'");
  fHandlerUpLink->
    appendPrefixToHandler (longDelayRestsPrefix);

  // the 'delay rests' multiplex booleans atom

  S_oahMultiplexBooleansAtom
    delayRestsMultiplexBooleansAtom =
      oahMultiplexBooleansAtom::create (
        "Delay SHORT_NAME/LONG_NAME that occur on a rest until the next note.",
        "SHORT_NAME",
        "LONG_NAME",
        shortDelayRestsPrefix,
        longDelayRestsPrefix);

  subGroup->
    appendAtomToSubGroup (
      delayRestsMultiplexBooleansAtom);

  // delay rests dynamics

  fDelayRestsDynamics = false;

  S_oahBooleanAtom
    delayRestsDynamicsAtom =
      oahBooleanAtom::create (
        "drdyns", "delay-rests-dynamics",
R"()",
        "delayRestsDynamics",
        fDelayRestsDynamics);
  subGroup->
    appendAtomToSubGroup (
      delayRestsDynamicsAtom);
  delayRestsMultiplexBooleansAtom->
    addBooleanAtom (
      delayRestsDynamicsAtom);

  // delay rests words

  fDelayRestsWords = false;

  S_oahBooleanAtom
    delayRestsWordsAtom =
      oahBooleanAtom::create (
        "drwords", "delay-rests-words",
R"()",
        "delayRestsWords",
        fDelayRestsWords);
  subGroup->
    appendAtomToSubGroup (
      delayRestsWordsAtom);
  delayRestsMultiplexBooleansAtom->
    addBooleanAtom (
      delayRestsWordsAtom);

  // delay rests beams

  fDelayRestsBeams = false;

  S_oahBooleanAtom
    delayRestsBeamsAtom =
      oahBooleanAtom::create (
        "drbeams", "delay-rests-beams",
R"()",
        "delayRestsBeams",
        fDelayRestsBeams);
  subGroup->
    appendAtomToSubGroup (
      delayRestsBeamsAtom);
  delayRestsMultiplexBooleansAtom->
    addBooleanAtom (
      delayRestsBeamsAtom);

  // delay rests slurs

  fDelayRestsSlurs = false;

  S_oahBooleanAtom
    delayRestsSlursAtom =
      oahBooleanAtom::create (
        "drslurs", "delay-rests-slurs",
R"()",
        "delayRestsSlurs",
        fDelayRestsSlurs);
  subGroup->
    appendAtomToSubGroup (
      delayRestsSlursAtom);
  delayRestsMultiplexBooleansAtom->
    addBooleanAtom (
      delayRestsSlursAtom);

  // delay rests ligatures

  fDelayRestsLigatures = false;

  S_oahBooleanAtom
    delayRestsLigaturesAtom =
      oahBooleanAtom::create (
        "drligs", "delay-rests-ligatures",
R"(<bracket/> in MusicXML, '\[... \}' in LilyPond)",
        "delayRestsLigatures",
        fDelayRestsLigatures);
  subGroup->
    appendAtomToSubGroup (
      delayRestsLigaturesAtom);
  delayRestsMultiplexBooleansAtom->
    addBooleanAtom (
      delayRestsLigaturesAtom);

  // delay rests pedals

  fDelayRestsPedals = false;

  S_oahBooleanAtom
    delayRestsPedalsAtom =
      oahBooleanAtom::create (
        "drpeds", "delay-rests-pedals",
R"()",
        "delayRestsPedals",
        fDelayRestsPedals);
  subGroup->
    appendAtomToSubGroup (
      delayRestsPedalsAtom);
  delayRestsMultiplexBooleansAtom->
    addBooleanAtom (
      delayRestsPedalsAtom);

  // delay rests slashes

  fDelayRestsSlashes = false;

  S_oahBooleanAtom
    delayRestsSlashesAtom =
      oahBooleanAtom::create (
        "drslashes", "delay-rests-slashes",
R"('<slash/>' in MusicXML)",
        "delayRestsSlashes",
        fDelayRestsSlashes);
  subGroup->
    appendAtomToSubGroup (
      delayRestsSlashesAtom);

  // delay rests wedges

  fDelayRestsWedges = false;

  S_oahBooleanAtom
    delayRestsWedgesAtom =
      oahBooleanAtom::create (
        "drwedges", "delay-rests-wedges",
R"('<wedge/>' in MusicXML, '<!' in LilyPond)",
        "delayRestsWedges",
        fDelayRestsWedges);
  subGroup->
    appendAtomToSubGroup (
      delayRestsWedgesAtom);
  delayRestsMultiplexBooleansAtom->
    addBooleanAtom (
      delayRestsWedgesAtom);

  // slash all grace notes

  fSlashAllGraceNotes = false;

  S_oahBooleanAtom
    slashAllGraceNotesAtom =
      oahBooleanAtom::create (
        "slashagn", "slash-all-grace-notes ",
R"()",
        "slashAllGraceNotes",
        fSlashAllGraceNotes);
  subGroup->
    appendAtomToSubGroup (
      slashAllGraceNotesAtom);

  // slur all grace notes

  fSlurAllGraceNotes = false;

  S_oahBooleanAtom
    slurAllGraceNotesAtom =
      oahBooleanAtom::create (
        "sluragn", "slur-all-grace-notes ",
R"()",
        "slurAllGraceNotes",
        fSlurAllGraceNotes);
  subGroup->
    appendAtomToSubGroup (
      slurAllGraceNotesAtom);

  // omit articulations

  fOmitArticulations = false;

  S_oahBooleanAtom
    omitArticulationsAtom =
      oahBooleanAtom::create (
        "oarts", "omit-articulations",
R"('<word/>' in MusicXML, '<!' in LilyPond)",
        "omitArticulations",
        fOmitArticulations);
  subGroup->
    appendAtomToSubGroup (
      omitArticulationsAtom);

  // omit ornaments
  // --------------------------------------

  fOmitOrnaments = false;

  S_oahBooleanAtom
    omitOrnamentsAtom =
      oahBooleanAtom::create (
        "oorns", "omit-ornaments",
R"('<trill-mark/>' in MusicXML, '<!' in LilyPond)",
        "omitOrnaments",
        fOmitOrnaments);
  subGroup->
    appendAtomToSubGroup (
      omitOrnamentsAtom);

  // omit words

  fOmitWords = false;

  S_oahBooleanAtom
    omitWordsAtom =
      oahBooleanAtom::create (
        "owords", "omit-words",
R"('<word/>' in MusicXML, '<!' in LilyPond)",
        "omitWords",
        fOmitWords);
  subGroup->
    appendAtomToSubGroup (
      omitWordsAtom);

  // omit ties

  fOmitTies = false;

  S_oahBooleanAtom
    omitTiesAtom =
      oahBooleanAtom::create (
        "oties", "omit-ties",
R"()",
        "omitTies",
        fOmitTies);
  subGroup->
    appendAtomToSubGroup (
      omitTiesAtom);

  // omit dynamics

  fOmitDynamics = false;

  S_oahBooleanAtom
    omitDynamicsAtom =
      oahBooleanAtom::create (
        "odyns", "omit-dynamics",
R"()",
        "omitDynamics",
        fOmitDynamics);
  subGroup->
    appendAtomToSubGroup (
      omitDynamicsAtom);

  // omit slurs

  fOmitSlurs = false;

  S_oahBooleanAtom
    omitSlursAtom =
      oahBooleanAtom::create (
        "oslurs", "omit-slurs",
R"()",
        "omitSlurs",
        fOmitSlurs);
  subGroup->
    appendAtomToSubGroup (
      omitSlursAtom);

  // omit wedges

  fOmitWedges = false;

  S_oahBooleanAtom
    omitWedgesAtom =
      oahBooleanAtom::create (
        "owedges", "omit-wedges",
R"('<wedge/>' in MusicXML, '<!' in LilyPond)",
        "omitWedges",
        fOmitWedges);
  subGroup->
    appendAtomToSubGroup (
      omitWedgesAtom);
}

void msr2LpsrOah::initializeMsr2LpsrLyricsOptions (
  bool boolOptionsInitialValue)
{
  S_oahSubGroup subGroup =
    oahSubGroup::create (
      "Lyrics",
      "hmsrlyrd", "help-msr-lyrics",
R"()",
    kElementVisibilityAlways,
    this);

  appendSubGroupToGroup (subGroup);

  // stanzas numbers

  fAddStanzasNumbers = false;

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "asn", "add-stanzas-numbers",
R"(Add stanzas numbers to lyrics.)",
        "addStanzasNumbers",
        fAddStanzasNumbers));

  // omit lyrics

  fOmitLyrics = false;

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "olyrics", "omit-lyrics",
R"(Don't create lyrics in the MSR.)",
        "omitLyrics",
        fOmitLyrics));
}

void msr2LpsrOah::initializeMsr2LpsrHarmoniesOptions (
  bool boolOptionsInitialValue)
{
  S_oahSubGroup subGroup =
    oahSubGroup::create (
      "Harmonies",
      "hmsrh", "help-msr-harmonies",
R"()",
    kElementVisibilityAlways,
    this);

  appendSubGroupToGroup (subGroup);

  // show harmony voices

  fShowHarmonyVoices = boolOptionsInitialValue;

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "shv", "show-harmony-voices",
R"(Show the parts harmony voices in the MSR data
even though it does not contain music.)",
        "showHarmonyVoices",
        fShowHarmonyVoices));

  // no MSR harmonies

  fOmitHarmonies = boolOptionsInitialValue;

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "oharms", "omit-harmonies",
R"(Don't create harmonies in the MSR.)",
        "omitHarmonies",
        fOmitHarmonies));
}

void msr2LpsrOah::initializeMsr2LpsrFiguredBassOptions (
  bool boolOptionsInitialValue)
{
  S_oahSubGroup subGroup =
    oahSubGroup::create (
      "Figured bass",
      "hmsrfb", "help-msr-figured-bass",
R"()",
    kElementVisibilityAlways,
    this);

  appendSubGroupToGroup (subGroup);

  // show figured bass voices

  fShowFiguredBassVoices = boolOptionsInitialValue;

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "sfbv", "show-figured-bass-voices",
R"(Show the figured bass harmony voices in the MSR data
even though they do not contain music.)",
        "showFiguredBassVoices",
        fShowFiguredBassVoices));

  // omit figured basses

  fOmitFiguredBasses = false;

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "ofigbasses", "omit-figured-basses",
R"(Don't create figured basses in the MSR.)",
        "omitFiguredBasses",
        fOmitFiguredBasses));
}

void msr2LpsrOah::initializeMsr2LpsrExitAfterSomePassesOptions (
  bool boolOptionsInitialValue)
{
  S_oahSubGroup
    exitAfterSomePassesSubGroup =
      oahSubGroup::create (
        "Exit after some passes",
        "hmexit", "help-msr-exit",
R"()",
      kElementVisibilityAlways,
      this);

  appendSubGroupToGroup (exitAfterSomePassesSubGroup);

  // exit after pass 2a

  fExit2a = boolOptionsInitialValue;

  S_oahBooleanAtom
    exit2aOahBooleanAtom =
      oahBooleanAtom::create (
        "e2a", "exit-2a",
R"(Exit after pass 2a, i.e. after conversion
of the MusicXML tree to an MSR skeleton.)",
        "exit2a",
        fExit2a);

  exitAfterSomePassesSubGroup->
    appendAtomToSubGroup (
      exit2aOahBooleanAtom);

  // exit after pass 2b

  fExit2b = boolOptionsInitialValue;

  S_oahBooleanAtom
    exit2bOahBooleanAtom =
      oahBooleanAtom::create (
        "e2b", "exit-2b",
R"(Exit after pass 2b, i.e. after conversion
of the MusicXML tree to MSR.)",
        "exit2b",
        fExit2b);

  exitAfterSomePassesSubGroup->
    appendAtomToSubGroup (
      exit2bOahBooleanAtom);
}

void msr2LpsrOah::initializeMsr2LpsrOah (
  bool boolOptionsInitialValue)
{
#ifdef TRACE_OAH
  // trace
  // --------------------------------------
  initializeMsr2LpsrTraceOah (
    boolOptionsInitialValue);
#endif

  // score output kind
  // --------------------------------------
  initializeMsr2LpsrScoreOutputOptions (
    boolOptionsInitialValue);

  // parts
  // --------------------------------------
  initializeMsr2LpsrPartsOptions (
    boolOptionsInitialValue);

  // staves
  // --------------------------------------
  initializeMsr2LpsrStavesOptions (
    boolOptionsInitialValue);

  // voices
  // --------------------------------------
  initializeMsr2LpsrVoicesOptions (
    boolOptionsInitialValue);

  // repeats
  // --------------------------------------
  initializeMsr2LpsrRepeatsOptions (
    boolOptionsInitialValue);

  // notes
  // --------------------------------------
  initializeMsr2LpsrNotesOptions (
    boolOptionsInitialValue);

  // lyrics
  // --------------------------------------
  initializeMsr2LpsrLyricsOptions (
    boolOptionsInitialValue);

  // harmonies
  // --------------------------------------
  initializeMsr2LpsrHarmoniesOptions (
    boolOptionsInitialValue);

  // figured bass
  // --------------------------------------
  initializeMsr2LpsrFiguredBassOptions (
    boolOptionsInitialValue);

  // exit after some passes
  // --------------------------------------
  initializeMsr2LpsrExitAfterSomePassesOptions (
    boolOptionsInitialValue);
}

S_msr2LpsrOah msr2LpsrOah::createCloneWithDetailedTrace ()
{
  S_msr2LpsrOah
    clone =
      msr2LpsrOah::create (0);
      // 0 not to have it inserted twice in the option handler

  // set the options handler upLink
  clone->fHandlerUpLink =
    fHandlerUpLink;

  // LilyPond output kind
  // --------------------------------------

  clone->fScoreOutputKind =
    fScoreOutputKind;

  // parts
  // --------------------------------------

  clone->fPartsRenamingMap =
    fPartsRenamingMap;
  clone->fPartsTranspositionMap =
    fPartsTranspositionMap;

  clone->fPartsOmitIDSet =
    fPartsOmitIDSet;
  clone->fPartsKeepIDSet =
    fPartsKeepIDSet;

  clone->fPartsOmitNameSet =
    fPartsOmitNameSet;
  clone->fPartsKeepNameSet =
    fPartsKeepNameSet;

  // staves
  // --------------------------------------

  clone->fCreateSingleLineStavesAsRythmic =
    fCreateSingleLineStavesAsRythmic;

  // voices
  // --------------------------------------

  clone->fCreateVoicesStaffRelativeNumbers =
    fCreateVoicesStaffRelativeNumbers;

  // repeats
  // --------------------------------------

  clone->fCreateImplicitInitialRepeatBarline =
    fCreateImplicitInitialRepeatBarline;

  // notes
  // --------------------------------------

  clone->fDelayRestsDynamics =
    fDelayRestsDynamics;
  clone->fDelayRestsWords =
    fDelayRestsWords;
  clone->fDelayRestsSlurs =
    fDelayRestsSlurs;
  clone->fDelayRestsLigatures =
    fDelayRestsLigatures;
  clone->fDelayRestsPedals =
    fDelayRestsPedals;
  clone->fDelayRestsSlashes =
    fDelayRestsSlashes;
  clone->fDelayRestsWedges =
    fDelayRestsWedges;

  clone->fSlashAllGraceNotes =
    fSlashAllGraceNotes;
  clone->fSlurAllGraceNotes =
    fSlurAllGraceNotes;

  clone->fOmitArticulations =
    fOmitArticulations;

  clone->fOmitOrnaments =
    fOmitOrnaments;

  clone->fOmitWords =
    fOmitWords;

  clone->fOmitTies =
    fOmitTies;

  clone->fOmitDynamics =
    fOmitDynamics;

  clone->fOmitSlurs =
    fOmitSlurs;

  clone->fOmitWedges =
    fOmitWedges;

  // lyrics
  // --------------------------------------

  clone->fAddStanzasNumbers =
    fAddStanzasNumbers;

  clone->fOmitLyrics =
    fOmitLyrics;

  // harmonies
  // --------------------------------------

  clone->fShowHarmonyVoices =
    fShowHarmonyVoices;

  clone->fOmitHarmonies =
    fOmitHarmonies;

  // figured bass
  // --------------------------------------

  clone->fShowFiguredBassVoices =
    fShowFiguredBassVoices;

  clone->fOmitFiguredBasses =
    fOmitFiguredBasses;

  return clone;
}

//______________________________________________________________________________
void msr2LpsrOah::setAllMsr2LpsrTraceOah (
  bool boolOptionsInitialValue)
{
#ifdef TRACE_OAH
  // specific trace
  // JMI
#endif
}

//______________________________________________________________________________
void msr2LpsrOah::enforceQuietness ()
{}

//______________________________________________________________________________
void msr2LpsrOah::checkOptionsConsistency ()
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceOah) {
    gLogOstream <<
      "Checking the consistency of OAH group \"" <<
      fGroupHeader <<
      "\"" <<
      endl;
  }
#endif

  // JMI and if mixed ID and name options are used?

  if (fPartsOmitIDSet.size () > 0 && fPartsKeepIDSet.size () > 0) {
    stringstream s;

    s <<
      "options '" <<
      fOmitPartIDAtom->fetchNames () <<
      "' and '" <<
      fKeepPartIDAtom->fetchNames () <<
      "' are incompatible";

    oahError (s.str ());
  }

  if (fPartsOmitNameSet.size () > 0 && fPartsKeepNameSet.size () > 0) {
    stringstream s;

    s <<
      "options '" <<
      fOmitPartNameAtom->fetchNames () <<
      "' and '" <<
      fKeepPartNameAtom->fetchNames () <<
      "' are incompatible";

    oahError (s.str ());
  }
}

//______________________________________________________________________________
void msr2LpsrOah::acceptIn (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    gLogOstream <<
      ".\\\" ==> msr2LpsrOah::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_msr2LpsrOah>*
    p =
      dynamic_cast<visitor<S_msr2LpsrOah>*> (v)) {
        S_msr2LpsrOah elem = this;

#ifdef TRACE_OAH
        if (gOahOah->fTraceOahVisitors) {
          gLogOstream <<
            ".\\\" ==> Launching msr2LpsrOah::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void msr2LpsrOah::acceptOut (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    gLogOstream <<
      ".\\\" ==> msr2LpsrOah::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_msr2LpsrOah>*
    p =
      dynamic_cast<visitor<S_msr2LpsrOah>*> (v)) {
        S_msr2LpsrOah elem = this;

#ifdef TRACE_OAH
        if (gOahOah->fTraceOahVisitors) {
          gLogOstream <<
            ".\\\" ==> Launching msr2LpsrOah::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void msr2LpsrOah::browseData (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    gLogOstream <<
      ".\\\" ==> msr2LpsrOah::browseData ()" <<
      endl;
  }
#endif
}

//______________________________________________________________________________
void msr2LpsrOah::printMsr2LpsrOahValues (int fieldWidth)
{
  gLogOstream <<
    "The MusicXML options are:" <<
    endl;

  gIndenter++;

  // LilyPond output kind
  // --------------------------------------

  gLogOstream <<
    "LilyPond output kind:" <<
    endl;

  gIndenter++;

  gLogOstream << left <<
    setw (fieldWidth) << "scoreOutputKind" << " : " <<
    lpsrScoreOutputKindAsString (fScoreOutputKind) <<
    endl;

  gIndenter--;

  // parts
  // --------------------------------------

  gLogOstream <<
     "Parts:" <<
    endl;

  gIndenter++;

  // parts renaming

  gLogOstream << left <<
    setw (fieldWidth) << "parts renaming" << " : ";

  if (! fPartsRenamingMap.size ()) {
    gLogOstream <<
      "none";
  }
  else {
    for (
      map<string, string>::const_iterator i =
        fPartsRenamingMap.begin ();
      i != fPartsRenamingMap.end ();
      i++
  ) {
        gLogOstream <<
          "\"" << ((*i).first) << "\" -> \"" << ((*i).second) << "\"";
    } // for
  }

  gLogOstream << endl;

  // parts transposition

  gLogOstream << left <<
    setw (fieldWidth) << "parts transposition" << " : ";

  if (! fPartsTranspositionMap.size ()) {
    gLogOstream <<
      "none";
  }
  else {
    for (
      map<string, S_msrSemiTonesPitchAndOctave>::const_iterator i =
        fPartsTranspositionMap.begin ();
      i != fPartsTranspositionMap.end ();
      i++
  ) {
        gLogOstream <<
          "\"" << ((*i).first) <<
          " = " <<
          ((*i).second->asString ()) <<
          "\" ";
    } // for
  }

  // parts omitted IDs

  gLogOstream << left <<
    setw (fieldWidth) << "parts omitted IDs" << " : ";

  if (! fPartsOmitIDSet.size ()) {
    gLogOstream <<
      "none";
  }
  else {
    for (
      set<string> ::const_iterator i =
        fPartsOmitIDSet.begin ();
      i != fPartsOmitIDSet.end ();
      i++
  ) {
        gLogOstream <<
          "\"" << (*i) << "\" ";
    } // for
  }

  // parts kept IDs

  gLogOstream << left <<
    setw (fieldWidth) << "parts kept IDs" << " : ";

  if (! fPartsKeepIDSet.size ()) {
    gLogOstream <<
      "none";
  }
  else {
    for (
      set<string> ::const_iterator i =
        fPartsKeepIDSet.begin ();
      i != fPartsKeepIDSet.end ();
      i++
  ) {
        gLogOstream <<
          "\"" << (*i) << "\" ";
    } // for
  }

  // parts omitted names

  gLogOstream << left <<
    setw (fieldWidth) << "parts omitted names" << " : ";

  if (! fPartsOmitNameSet.size ()) {
    gLogOstream <<
      "none";
  }
  else {
    for (
      set<string> ::const_iterator i =
        fPartsOmitNameSet.begin ();
      i != fPartsOmitNameSet.end ();
      i++
  ) {
        gLogOstream <<
          "\"" << (*i) << "\" ";
    } // for
  }

  // parts kept names

  gLogOstream << left <<
    setw (fieldWidth) << "parts kept names" << " : ";

  if (! fPartsKeepNameSet.size ()) {
    gLogOstream <<
      "none";
  }
  else {
    for (
      set<string> ::const_iterator i =
        fPartsKeepNameSet.begin ();
      i != fPartsKeepNameSet.end ();
      i++
  ) {
        gLogOstream <<
          "\"" << (*i) << "\" ";
    } // for
  }

  gLogOstream << endl;

  gIndenter--;

  // staves
  // --------------------------------------

  gLogOstream <<
    "Staves:" <<
    endl;

  gIndenter++;

  gLogOstream << left <<
    setw (fieldWidth) <<
    "createSingleLineStavesAsRythmic" << " : " <<
    booleanAsString (fCreateSingleLineStavesAsRythmic) <<
    endl;

  gIndenter--;

  // voices
  // --------------------------------------

  gLogOstream <<
    "Voices:" <<
    endl;

  gIndenter++;

  gLogOstream << left <<
    setw (fieldWidth) <<
    "createVoicesStaffRelativeNumbers" << " : " <<
    booleanAsString (fCreateVoicesStaffRelativeNumbers) <<
    endl;

  gIndenter--;

  // repeats
  // --------------------------------------

  gLogOstream <<
    "Repeats:" <<
    endl;

  gIndenter++;

  gLogOstream << left <<
    setw (fieldWidth) <<
    "createImplicitInitialRepeatBarline" << " : " <<
    booleanAsString (fCreateImplicitInitialRepeatBarline) <<
    endl;

  gIndenter--;

  // notes
  // --------------------------------------

  gLogOstream <<
     "Notes:" <<
    endl;

  gIndenter++;

  gLogOstream << left <<
    setw (fieldWidth) << "delayRestsDynamics" << " : " <<
    booleanAsString (fDelayRestsDynamics) <<
    endl <<

    setw (fieldWidth) << "delayRestsWords" << " : " <<
    booleanAsString (fDelayRestsWords) <<
    endl <<

    setw (fieldWidth) << "delayRestsSlurs" << " : " <<
    booleanAsString (fDelayRestsSlurs) <<
    endl <<

    setw (fieldWidth) << "delayRestsLigatures" << " : " <<
    booleanAsString (fDelayRestsLigatures) <<
    endl <<

    setw (fieldWidth) << "delayRestsPedals" << " : " <<
    booleanAsString (fDelayRestsPedals) <<
    endl <<

    setw (fieldWidth) << "delayRestsSlashes" << " : " <<
    booleanAsString (fDelayRestsSlashes) <<
    endl <<

    setw (fieldWidth) << "delayRestsWedges" << " : " <<
    booleanAsString (fDelayRestsWedges) <<
    endl <<

    setw (fieldWidth) << "slashAllGraceNotes" << " : " <<
    booleanAsString (fSlashAllGraceNotes) <<
    endl <<
    setw (fieldWidth) << "slurAllGraceNotes" << " : " <<
    booleanAsString (fSlurAllGraceNotes) <<
    endl;

  gIndenter--;

  // articulations
  // --------------------------------------

  gLogOstream <<
     "Articulations:" <<
    endl;

  gIndenter++;

  gLogOstream << left <<
    setw (fieldWidth) << "omitArticulations" << " : " <<
    booleanAsString (fOmitArticulations) <<
    endl;

  gIndenter--;

  // ornaments
  // --------------------------------------

  gLogOstream <<
     "Ornaments:" <<
    endl;

  gIndenter++;

  gLogOstream << left <<
    setw (fieldWidth) << "omitOrnaments" << " : " <<
    booleanAsString (fOmitOrnaments) <<
    endl;

  gIndenter--;

  // words
  // --------------------------------------

  gLogOstream <<
     "Words:" <<
    endl;

  gIndenter++;

  gLogOstream << left <<
    setw (fieldWidth) << "omitWords" << " : " <<
    booleanAsString (fOmitWords) <<
    endl;

  gIndenter--;

  // ties
  // --------------------------------------

  gLogOstream <<
     "Ties:" <<
    endl;

  gIndenter++;

  gLogOstream << left <<
    setw (fieldWidth) << "omitTies" << " : " <<
    booleanAsString (fOmitTies) <<
    endl;

  gIndenter--;

  // dynamics
  // --------------------------------------

  gLogOstream <<
     "Dynamics:" <<
    endl;

  gIndenter++;

  gLogOstream << left <<
    setw (fieldWidth) << "omitDynamics" << " : " <<
    booleanAsString (fOmitDynamics) <<
    endl;

  gIndenter--;

  // slurs
  // --------------------------------------

  gLogOstream <<
     "Slurs:" <<
    endl;

  gIndenter++;

  gLogOstream << left <<
    setw (fieldWidth) << "mitSlurs" << " : " <<
    booleanAsString (fOmitSlurs) <<
    endl;

  gIndenter--;

  // wedges
  // --------------------------------------

  gLogOstream <<
     "Wedges:" <<
    endl;

  gIndenter++;

  gLogOstream << left <<
    setw (fieldWidth) << "omitWedges" << " : " <<
    booleanAsString (fOmitWedges) <<
    endl;

  gIndenter--;

  // lyrics
  // --------------------------------------

  gLogOstream <<
    "Lyrics:" <<
    endl;

  gIndenter++;

  gLogOstream <<
    setw (fieldWidth) << "addStanzasNumbers" << " : " <<
    booleanAsString (fAddStanzasNumbers) <<
    endl <<

    setw (fieldWidth) << "omitLyrics" << " : " <<
    booleanAsString (fOmitLyrics) <<
    endl;

  gIndenter--;

  // harmonies
  // --------------------------------------

  gLogOstream <<
    "Harmonies:" <<
    endl;

  gIndenter++;

  gLogOstream << left <<
    setw (fieldWidth) << "showHarmonyVoices" << " : " <<
    booleanAsString (fShowHarmonyVoices) <<
    endl <<

    setw (fieldWidth) << "omitHarmonies" << " : " <<
    booleanAsString (fOmitHarmonies) <<
    endl;

  gIndenter--;

  // figured bass
  // --------------------------------------

  gLogOstream <<
    "Figured bass:" <<
    endl;

  gIndenter++;

  gLogOstream << left <<
    setw (fieldWidth) << "showFiguredBassVoices" << " : " <<
    booleanAsString (fShowFiguredBassVoices) <<
    endl <<

    setw (fieldWidth) << "omitFiguredBasses" << " : " <<
    booleanAsString (fOmitFiguredBasses) <<
    endl;

  gIndenter--;

  gIndenter--;

  // exit after some passes
  // --------------------------------------

  gLogOstream <<
    "Exit after some passes:" <<
    endl;

  gIndenter++;

  gLogOstream << left <<
    setw (fieldWidth) << "exit2a" << " : " <<
    booleanAsString (fExit2a) <<
    endl <<
    setw (fieldWidth) << "exit2b" << " : " <<
    booleanAsString (fExit2b) <<
    endl;

  gIndenter--;
  gIndenter--;
}

ostream& operator<< (ostream& os, const S_msr2LpsrOah& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
void initializeMsr2LpsrOahHandling (
  S_oahHandler handler)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceOah && ! gGeneralOah->fQuiet) {
    gLogOstream <<
      "Initializing MusicXML options handling" <<
      endl;
  }
#endif

  // create the MusicXML options
  // ------------------------------------------------------

  gMsr2LpsrOahUserChoices = msr2LpsrOah::create (
    handler);
  assert(gMsr2LpsrOahUserChoices != 0);

  gMsr2LpsrOah =
    gMsr2LpsrOahUserChoices;

  // prepare for measure detailed trace
  // ------------------------------------------------------

/* JMI
  gMsr2LpsrOahWithDetailedTrace =
    gMsr2LpsrOah->
      createCloneWithDetailedTrace ();
      */
}


}
