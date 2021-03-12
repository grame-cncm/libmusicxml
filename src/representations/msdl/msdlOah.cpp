/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#include <sstream>
#include <iomanip>      // setw, setprecision, ...
#include <string>

#include <regex>

#include "utilities.h"

#include "enableTracingIfDesired.h"
#ifdef TRACING_IS_ENABLED
  #include "traceOah.h"
#endif

#include "oahOah.h"
#include "generalOah.h"

#include "msdlOah.h"
#include "msdl2msrOah.h"

#include "msdlTokens.h"

#include "waeMessagesHandling.h"


using namespace std;

namespace MusicXML2
{

//______________________________________________________________________________
S_msdlKeywordsLanguageAtom msdlKeywordsLanguageAtom::create (
  string             shortName,
  string             longName,
  string             description,
  string             valueSpecification,
  string             variableName,
  msdlKeywordsLanguageKind&
                     msdlKeywordsLanguageKindVariable)
{
  msdlKeywordsLanguageAtom* o = new
    msdlKeywordsLanguageAtom (
      shortName,
      longName,
      description,
      valueSpecification,
      variableName,
      msdlKeywordsLanguageKindVariable);
  assert (o != nullptr);
  return o;
}

msdlKeywordsLanguageAtom::msdlKeywordsLanguageAtom (
  string             shortName,
  string             longName,
  string             description,
  string             valueSpecification,
  string             variableName,
  msdlKeywordsLanguageKind&
                     msdlKeywordsLanguageKindVariable)
  : oahAtomStoringAValueInAVariable (
      shortName,
      longName,
      description,
      valueSpecification,
      variableName),
    fMsdlKeywordsLanguageKindVariable (
      msdlKeywordsLanguageKindVariable)
{}

msdlKeywordsLanguageAtom::~msdlKeywordsLanguageAtom ()
{}

void msdlKeywordsLanguageAtom::applyAtomWithValue (
  const string& theString,
  ostream&      os)
{
#ifdef TRACING_IS_ENABLED
  if (getTraceOah ()) {
    gLogStream <<
      "==> oahAtom is of type 'msdlKeywordsLanguageAtom'" <<
      endl;
  }
#endif

  // theString contains the language name:
  // is it in the keywords languages map?

#ifdef TRACING_IS_ENABLED
  if (getTraceOah ()) {
    gLogStream <<
      "==> oahAtom is of type 'msdlKeywordsLanguageAtom'" <<
      endl;
  }
#endif

  map<string, msdlKeywordsLanguageKind>::const_iterator
    it =
      gGlobalMsdlKeywordsLanguageKindsMap.find (theString);

  if (it == gGlobalMsdlKeywordsLanguageKindsMap.end ()) {
    // no, language is unknown in the map
    stringstream s;

    s <<
      "MSDR keywords language '" << theString <<
      "' is unknown" <<
      endl <<
      "The " <<
      gGlobalMsdlKeywordsLanguageKindsMap.size () - 1 <<
      " known MSDR keywords languages are:" <<
      endl;

    ++gIndenter;

    s <<
      existingMsdlKeywordsLanguageKinds (K_NAMES_LIST_MAX_LENGTH);

    --gIndenter;

    oahError (s.str ());
  }

  setMsdlKeywordsLanguageKindVariable (
    (*it).second);
}

void msdlKeywordsLanguageAtom::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> msdlKeywordsLanguageAtom::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_msdlKeywordsLanguageAtom>*
    p =
      dynamic_cast<visitor<S_msdlKeywordsLanguageAtom>*> (v)) {
        S_msdlKeywordsLanguageAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching msdlKeywordsLanguageAtom::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void msdlKeywordsLanguageAtom::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> msdlKeywordsLanguageAtom::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_msdlKeywordsLanguageAtom>*
    p =
      dynamic_cast<visitor<S_msdlKeywordsLanguageAtom>*> (v)) {
        S_msdlKeywordsLanguageAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching msdlKeywordsLanguageAtom::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void msdlKeywordsLanguageAtom::browseData (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> msdlKeywordsLanguageAtom::browseData ()" <<
      endl;
  }
#endif
}

string msdlKeywordsLanguageAtom::asShortNamedOptionString () const
{
  stringstream s;

  s <<
    "-" << fShortName << " " <<
    msdlKeywordsLanguageKindAsString (fMsdlKeywordsLanguageKindVariable);

  return s.str ();
}

string msdlKeywordsLanguageAtom::asActualLongNamedOptionString () const
{
  stringstream s;

  s <<
    "-" << fLongName << " " <<
    msdlKeywordsLanguageKindAsString (fMsdlKeywordsLanguageKindVariable);

  return s.str ();
}

void msdlKeywordsLanguageAtom::print (ostream& os) const
{
  const unsigned int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "msdlKeywordsLanguageAtom:" <<
    endl;

  ++gIndenter;

  printAtomWithVariableEssentials (
    os, fieldWidth);

  os << left <<
    setw (fieldWidth) <<
    "variableName" << " : " <<
    fVariableName <<
    setw (fieldWidth) <<
    "fMsdlKeywordsLanguageKindVariable" << " : \"" <<
    msdlKeywordsLanguageKindAsString (
      fMsdlKeywordsLanguageKindVariable) <<
    "\"" <<
    endl;

  --gIndenter;
}

void msdlKeywordsLanguageAtom::printAtomWithVariableOptionsValues (
  ostream&     os,
  unsigned int valueFieldWidth) const
{
  os << left <<
    setw (valueFieldWidth) <<
    fVariableName <<
    " : \"" <<
    msdlKeywordsLanguageKindAsString (
      fMsdlKeywordsLanguageKindVariable) <<
    "\"";
  if (fVariableHasBeenSet) {
    os <<
      ", variableHasBeenSet: " <<
      booleanAsString (fVariableHasBeenSet);
  }
  os << endl;
}

ostream& operator<< (ostream& os, const S_msdlKeywordsLanguageAtom& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_oahDisplayMsdlKeywordsInLanguageAtom oahDisplayMsdlKeywordsInLanguageAtom::create (
  string shortName,
  string longName,
  string description,
  string executableName)
{
  oahDisplayMsdlKeywordsInLanguageAtom* o = new
    oahDisplayMsdlKeywordsInLanguageAtom (
      shortName,
      longName,
      description,
      executableName);
  assert (o != nullptr);
  return o;
}

oahDisplayMsdlKeywordsInLanguageAtom::oahDisplayMsdlKeywordsInLanguageAtom (
  string shortName,
  string longName,
  string description,
  string executableName)
  : oahHelpAtomExpectingAValue (
      shortName,
      longName,
      description,
      executableName)
{
  this->setElementValueKind (oahElementValueKind::kElementValueMandatory);

  fElementHelpOnlyKind = oahElementHelpOnlyKind::kElementHelpOnlyYes;

  this->setMultipleOccurrencesAllowed ();
}

oahDisplayMsdlKeywordsInLanguageAtom::~oahDisplayMsdlKeywordsInLanguageAtom ()
{}

void oahDisplayMsdlKeywordsInLanguageAtom::applyAtomWithValue (
  const string& theString,
  ostream&      os)
{
  // theString contains the language name:
  // is it in the keywords languages map?

  map<string, msdlKeywordsLanguageKind>::const_iterator
    it =
      gGlobalMsdlKeywordsLanguageKindsMap.find (theString);

  if (it == gGlobalMsdlKeywordsLanguageKindsMap.end ()) {
    // no, language is unknown in the map
    stringstream s;

    s <<
      "MSDR keywords display language \"" << theString <<
      "\" is unknown" <<
      endl <<
      "The " <<
      gGlobalMsdlKeywordsLanguageKindsMap.size () - 1 <<
      " known MSDR keywords languages are:" <<
      endl;

    ++gIndenter;

    s <<
      existingMsdlKeywordsLanguageKinds (K_NAMES_LIST_MAX_LENGTH);

    --gIndenter;

    oahError (s.str ());
  }

  // get the keywords language kind
  msdlKeywordsLanguageKind
    keywordsLanguageKind =
      (*it).second;

  // write the existing keywords names
  gLogStream <<
    "The keywords in language '" <<
    msdlKeywordsLanguageKindAsString (keywordsLanguageKind) <<
    "' are: " <<
    endl;

  ++gIndenter;

  gLogStream <<
    gIndenter.indentMultiLineString (
      existingKeywordsInLanguage (
        keywordsLanguageKind,
        K_NAMES_LIST_MAX_LENGTH)) <<
    endl;

  --gIndenter;
}

void oahDisplayMsdlKeywordsInLanguageAtom::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahDisplayMsdlKeywordsInLanguageAtom::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_oahDisplayMsdlKeywordsInLanguageAtom>*
    p =
      dynamic_cast<visitor<S_oahDisplayMsdlKeywordsInLanguageAtom>*> (v)) {
        S_oahDisplayMsdlKeywordsInLanguageAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching oahDisplayMsdlKeywordsInLanguageAtom::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void oahDisplayMsdlKeywordsInLanguageAtom::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahDisplayMsdlKeywordsInLanguageAtom::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_oahDisplayMsdlKeywordsInLanguageAtom>*
    p =
      dynamic_cast<visitor<S_oahDisplayMsdlKeywordsInLanguageAtom>*> (v)) {
        S_oahDisplayMsdlKeywordsInLanguageAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching oahDisplayMsdlKeywordsInLanguageAtom::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void oahDisplayMsdlKeywordsInLanguageAtom::browseData (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahDisplayMsdlKeywordsInLanguageAtom::browseData ()" <<
      endl;
  }
#endif
}

string oahDisplayMsdlKeywordsInLanguageAtom::asShortNamedOptionString () const
{
  stringstream s;

  s <<
    "-" << fShortName;

  return s.str ();
}

string oahDisplayMsdlKeywordsInLanguageAtom::asActualLongNamedOptionString () const
{
  stringstream s;

  s <<
    "-" << fLongName;

  return s.str ();
}

void oahDisplayMsdlKeywordsInLanguageAtom::print (ostream& os) const
{
  const unsigned int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "oahDisplayMsdlKeywordsInLanguageAtom:" <<
    endl;

  ++gIndenter;

  printAtomWithVariableOptionsValues (
    os, fieldWidth);

  --gIndenter;
}

ostream& operator<< (ostream& os, const S_oahDisplayMsdlKeywordsInLanguageAtom& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_oahDisplayMsdlTokensInLanguageAtom oahDisplayMsdlTokensInLanguageAtom::create (
  string shortName,
  string longName,
  string description,
  string executableName)
{
  oahDisplayMsdlTokensInLanguageAtom* o = new
    oahDisplayMsdlTokensInLanguageAtom (
      shortName,
      longName,
      description,
      executableName);
  assert (o != nullptr);
  return o;
}

oahDisplayMsdlTokensInLanguageAtom::oahDisplayMsdlTokensInLanguageAtom (
  string shortName,
  string longName,
  string description,
  string executableName)
  : oahHelpAtomExpectingAValue (
      shortName,
      longName,
      description,
      executableName)
{
  this->setElementValueKind (oahElementValueKind::kElementValueMandatory);

  fElementHelpOnlyKind = oahElementHelpOnlyKind::kElementHelpOnlyYes;

  this->setMultipleOccurrencesAllowed ();
}

oahDisplayMsdlTokensInLanguageAtom::~oahDisplayMsdlTokensInLanguageAtom ()
{}

void oahDisplayMsdlTokensInLanguageAtom::applyAtomWithValue (
  const string& theString,
  ostream&      os)
{
  // theString contains the language name:
  // is it in the keywords languages map?

  map<string, msdlKeywordsLanguageKind>::const_iterator
    it =
      gGlobalMsdlKeywordsLanguageKindsMap.find (theString);

  if (it == gGlobalMsdlKeywordsLanguageKindsMap.end ()) {
    // no, language is unknown in the map
    stringstream s;

    s <<
      "MSDR keywords display language \"" << theString <<
      "\" is unknown" <<
      endl <<
      "The " <<
      gGlobalMsdlKeywordsLanguageKindsMap.size () - 1 <<
      " known MSDR keywords languages are:" <<
      endl;

    ++gIndenter;

    s <<
      existingMsdlKeywordsLanguageKinds (K_NAMES_LIST_MAX_LENGTH);

    --gIndenter;

    oahError (s.str ());
  }

  // get the keywords language kind
  msdlKeywordsLanguageKind
    keywordsLanguageKind =
      (*it).second;

  // write the existing tokens
  gLogStream <<
    "The tokens in language \"" <<
    msdlKeywordsLanguageKindAsString (keywordsLanguageKind) <<
    "\" are (<...> denotes a class of tokens): " <<
    endl;

  ++gIndenter;

  gLogStream <<
    gIndenter.indentMultiLineString (
      existingTokensInLanguage (
        keywordsLanguageKind,
        K_NAMES_LIST_MAX_LENGTH)) <<
    endl;

  --gIndenter;
}

void oahDisplayMsdlTokensInLanguageAtom::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahDisplayMsdlTokensInLanguageAtom::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_oahDisplayMsdlTokensInLanguageAtom>*
    p =
      dynamic_cast<visitor<S_oahDisplayMsdlTokensInLanguageAtom>*> (v)) {
        S_oahDisplayMsdlTokensInLanguageAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching oahDisplayMsdlTokensInLanguageAtom::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void oahDisplayMsdlTokensInLanguageAtom::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahDisplayMsdlTokensInLanguageAtom::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_oahDisplayMsdlTokensInLanguageAtom>*
    p =
      dynamic_cast<visitor<S_oahDisplayMsdlTokensInLanguageAtom>*> (v)) {
        S_oahDisplayMsdlTokensInLanguageAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching oahDisplayMsdlTokensInLanguageAtom::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void oahDisplayMsdlTokensInLanguageAtom::browseData (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahDisplayMsdlTokensInLanguageAtom::browseData ()" <<
      endl;
  }
#endif
}

string oahDisplayMsdlTokensInLanguageAtom::asShortNamedOptionString () const
{
  stringstream s;

  s <<
    "-" << fShortName;

  return s.str ();
}

string oahDisplayMsdlTokensInLanguageAtom::asActualLongNamedOptionString () const
{
  stringstream s;

  s <<
    "-" << fLongName;

  return s.str ();
}

void oahDisplayMsdlTokensInLanguageAtom::print (ostream& os) const
{
  const unsigned int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "oahDisplayMsdlTokensInLanguageAtom:" <<
    endl;

  ++gIndenter;

  printAtomWithVariableOptionsValues (
    os, fieldWidth);

  --gIndenter;
}

ostream& operator<< (ostream& os, const S_oahDisplayMsdlTokensInLanguageAtom& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_msdlCommentsTypeAtom msdlCommentsTypeAtom::create (
  string             shortName,
  string             longName,
  string             description,
  string             valueSpecification,
  string             variableName,
  msdlCommentsTypeKind&
                     msdlCommentsTypeKindVariable)
{
  msdlCommentsTypeAtom* o = new
    msdlCommentsTypeAtom (
      shortName,
      longName,
      description,
      valueSpecification,
      variableName,
      msdlCommentsTypeKindVariable);
  assert (o != nullptr);
  return o;
}

msdlCommentsTypeAtom::msdlCommentsTypeAtom (
  string             shortName,
  string             longName,
  string             description,
  string             valueSpecification,
  string             variableName,
  msdlCommentsTypeKind&
                     msdlCommentsTypeKindVariable)
  : oahAtomStoringAValueInAVariable (
      shortName,
      longName,
      description,
      valueSpecification,
      variableName),
    fMsdlCommentsTypeKindVariable (
      msdlCommentsTypeKindVariable)
{}

msdlCommentsTypeAtom::~msdlCommentsTypeAtom ()
{}

void msdlCommentsTypeAtom::applyAtomWithValue (
  const string& theString,
  ostream&      os)
{
#ifdef TRACING_IS_ENABLED
  if (getTraceOah ()) {
    gLogStream <<
      "==> oahAtom is of type 'msdlCommentsTypeAtom'" <<
      endl;
  }
#endif

  // theString contains the language name:
  // is it in the keywords languages map?

#ifdef TRACING_IS_ENABLED
  if (getTraceOah ()) {
    gLogStream <<
      "==> oahAtom is of type 'msdlCommentsTypeAtom'" <<
      endl;
  }
#endif

  map<string, msdlCommentsTypeKind>::const_iterator
    it =
      gGlobalMsdlCommentsTypeKindsMap.find (theString);

  if (it == gGlobalMsdlCommentsTypeKindsMap.end ()) {
    // no, language is unknown in the map
    stringstream s;

    s <<
      "MSDR keywords language '" << theString <<
      "' is unknown" <<
      endl <<
      "The " <<
      gGlobalMsdlCommentsTypeKindsMap.size () - 1 <<
      " known MSDR keywords languages are:" <<
      endl;

    ++gIndenter;

    s <<
      existingMsdlCommentsTypeKinds (K_NAMES_LIST_MAX_LENGTH);

    --gIndenter;

    oahError (s.str ());
  }

  setMsdlCommentsTypeKindVariable (
    (*it).second);
}

void msdlCommentsTypeAtom::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> msdlCommentsTypeAtom::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_msdlCommentsTypeAtom>*
    p =
      dynamic_cast<visitor<S_msdlCommentsTypeAtom>*> (v)) {
        S_msdlCommentsTypeAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching msdlCommentsTypeAtom::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void msdlCommentsTypeAtom::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> msdlCommentsTypeAtom::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_msdlCommentsTypeAtom>*
    p =
      dynamic_cast<visitor<S_msdlCommentsTypeAtom>*> (v)) {
        S_msdlCommentsTypeAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching msdlCommentsTypeAtom::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void msdlCommentsTypeAtom::browseData (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> msdlCommentsTypeAtom::browseData ()" <<
      endl;
  }
#endif
}

string msdlCommentsTypeAtom::asShortNamedOptionString () const
{
  stringstream s;

  s <<
    "-" << fShortName << " " <<
    msdlCommentsTypeKindAsString (fMsdlCommentsTypeKindVariable);

  return s.str ();
}

string msdlCommentsTypeAtom::asActualLongNamedOptionString () const
{
  stringstream s;

  s <<
    "-" << fLongName << " " <<
    msdlCommentsTypeKindAsString (fMsdlCommentsTypeKindVariable);

  return s.str ();
}

void msdlCommentsTypeAtom::print (ostream& os) const
{
  const unsigned int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "msdlCommentsTypeAtom:" <<
    endl;

  ++gIndenter;

  printAtomWithVariableEssentials (
    os, fieldWidth);

  os << left <<
    setw (fieldWidth) <<
    "variableName" << " : " <<
    fVariableName <<
    setw (fieldWidth) <<
    "fMsdlCommentsTypeKindVariable" << " : \"" <<
    msdlCommentsTypeKindAsString (
      fMsdlCommentsTypeKindVariable) <<
    "\"" <<
    endl;

  --gIndenter;
}

void msdlCommentsTypeAtom::printAtomWithVariableOptionsValues (
  ostream&     os,
  unsigned int valueFieldWidth) const
{
  os << left <<
    setw (valueFieldWidth) <<
    fVariableName <<
    " : \"" <<
    msdlCommentsTypeKindAsString (
      fMsdlCommentsTypeKindVariable) <<
    "\"";
  if (fVariableHasBeenSet) {
    os <<
      ", variableHasBeenSet: " <<
      booleanAsString (fVariableHasBeenSet);
  }
  os << endl;
}

ostream& operator<< (ostream& os, const S_msdlCommentsTypeAtom& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_msdlUserLanguageAtom msdlUserLanguageAtom::create (
  string             shortName,
  string             longName,
  string             description,
  string             valueSpecification,
  string             variableName,
  msdlUserLanguageKind&
                     msdlUserLanguageKindVariable)
{
  msdlUserLanguageAtom* o = new
    msdlUserLanguageAtom (
      shortName,
      longName,
      description,
      valueSpecification,
      variableName,
      msdlUserLanguageKindVariable);
  assert (o != nullptr);
  return o;
}

msdlUserLanguageAtom::msdlUserLanguageAtom (
  string             shortName,
  string             longName,
  string             description,
  string             valueSpecification,
  string             variableName,
  msdlUserLanguageKind&
                     msdlUserLanguageKindVariable)
  : oahAtomStoringAValueInAVariable (
      shortName,
      longName,
      description,
      valueSpecification,
      variableName),
    fMsdlUserLanguageVariable (
      msdlUserLanguageKindVariable)
{}

msdlUserLanguageAtom::~msdlUserLanguageAtom ()
{}

void msdlUserLanguageAtom::applyAtomWithValue (
  const string& theString,
  ostream&      os)
{

#ifdef TRACING_IS_ENABLED
  if (getTraceOah ()) {
    gLogStream <<
      "==> oahAtom is of type 'msdlUserLanguageAtom'" <<
      endl;
  }
#endif

  // theString contains the language name:
  // is it in the User languages map?

#ifdef TRACING_IS_ENABLED
  if (getTraceOah ()) {
    gLogStream <<
      "==> oahAtom is of type 'msdlUserLanguageAtom'" <<
      endl;
  }
#endif

  map<string, msdlUserLanguageKind>::const_iterator
    it =
      gGlobalMsdlUserLanguageKindsMap.find (
        theString);

  if (it == gGlobalMsdlUserLanguageKindsMap.end ()) {
    // no, language is unknown in the map

    stringstream s;

    s <<
      "MSDR User language '" << theString <<
      "' is unknown" <<
      endl <<
      "The " <<
      gGlobalMsdlUserLanguageKindsMap.size () <<
      " known MSDR user languages are:" <<
      endl;

    ++gIndenter;

    s <<
      existingMsdlUserLanguageKinds (K_NAMES_LIST_MAX_LENGTH);

    --gIndenter;

    oahError (s.str ());
  }

  setMsdlUserLanguageKindVariable (
    (*it).second);
}

void msdlUserLanguageAtom::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> msdlUserLanguageAtom::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_msdlUserLanguageAtom>*
    p =
      dynamic_cast<visitor<S_msdlUserLanguageAtom>*> (v)) {
        S_msdlUserLanguageAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching msdlUserLanguageAtom::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void msdlUserLanguageAtom::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> msdlUserLanguageAtom::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_msdlUserLanguageAtom>*
    p =
      dynamic_cast<visitor<S_msdlUserLanguageAtom>*> (v)) {
        S_msdlUserLanguageAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching msdlUserLanguageAtom::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void msdlUserLanguageAtom::browseData (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> msdlUserLanguageAtom::browseData ()" <<
      endl;
  }
#endif
}

string msdlUserLanguageAtom::asShortNamedOptionString () const
{
  stringstream s;

  s <<
    "-" << fShortName << " " <<
    msdlUserLanguageKindAsString (
      fMsdlUserLanguageVariable);

  return s.str ();
}

string msdlUserLanguageAtom::asActualLongNamedOptionString () const
{
  stringstream s;

  s <<
    "-" << fLongName << " " <<
    msdlUserLanguageKindAsString (
      fMsdlUserLanguageVariable);

  return s.str ();
}

void msdlUserLanguageAtom::print (ostream& os) const
{
  const unsigned int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "msdlUserLanguageAtom:" <<
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
    "msdlUserLanguageVariable" << " : \"" <<
    msdlUserLanguageKindAsString (
      fMsdlUserLanguageVariable) <<
    "\"" <<
    endl;

  --gIndenter;
}

void msdlUserLanguageAtom::printAtomWithVariableOptionsValues (
  ostream&     os,
  unsigned int valueFieldWidth) const
{
  os << left <<
    setw (valueFieldWidth) <<
    fVariableName <<
    " : \"" <<
    msdlUserLanguageKindAsString (
      fMsdlUserLanguageVariable) <<
    "\"";
  if (fVariableHasBeenSet) {
    os <<
      ", variableHasBeenSet: " <<
      booleanAsString (fVariableHasBeenSet);
  }
  os << endl;
}

ostream& operator<< (ostream& os, const S_msdlUserLanguageAtom& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_msdlPitchesLanguageAtom msdlPitchesLanguageAtom::create (
  string             shortName,
  string             longName,
  string             description,
  string             valueSpecification,
  string             variableName,
  msrQuarterTonesPitchesLanguageKind&
                     msdlPitchesLanguageKindVariable)
{
  msdlPitchesLanguageAtom* o = new
    msdlPitchesLanguageAtom (
      shortName,
      longName,
      description,
      valueSpecification,
      variableName,
      msdlPitchesLanguageKindVariable);
  assert (o != nullptr);
  return o;
}

msdlPitchesLanguageAtom::msdlPitchesLanguageAtom (
  string             shortName,
  string             longName,
  string             description,
  string             valueSpecification,
  string             variableName,
  msrQuarterTonesPitchesLanguageKind&
                     msdlPitchesLanguageKindVariable)
  : oahAtomStoringAValueInAVariable (
      shortName,
      longName,
      description,
      valueSpecification,
      variableName),
    fMsrQuarterTonesPitchesLanguageKindVariable (
      msdlPitchesLanguageKindVariable)
{}

msdlPitchesLanguageAtom::~msdlPitchesLanguageAtom ()
{}

void msdlPitchesLanguageAtom::applyAtomWithValue (
  const string& theString,
  ostream&      os)
{

#ifdef TRACING_IS_ENABLED
  if (getTraceOah ()) {
    gLogStream <<
      "==> oahAtom is of type 'msdlPitchesLanguageAtom'" <<
      endl;
  }
#endif

  // theString contains the language name:
  // is it in the pitches languages map?

#ifdef TRACING_IS_ENABLED
  if (getTraceOah ()) {
    gLogStream <<
      "==> oahAtom is of type 'msdlPitchesLanguageAtom'" <<
      endl;
  }
#endif

  map<string, msrQuarterTonesPitchesLanguageKind>::const_iterator
    it =
      gGlobalQuarterTonesPitchesLanguageKindsMap.find (
        theString);

  if (it == gGlobalQuarterTonesPitchesLanguageKindsMap.end ()) {
    // no, language is unknown in the map

    stringstream s;

    s <<
      "MSDR pitches language '" << theString <<
      "' is unknown" <<
      endl <<
      "The " <<
      gGlobalQuarterTonesPitchesLanguageKindsMap.size () <<
      " known MSDR pitches languages are:" <<
      endl;

    ++gIndenter;

    s <<
      existingQuarterTonesPitchesLanguageKinds (K_NAMES_LIST_MAX_LENGTH);

    --gIndenter;

    oahError (s.str ());
  }

  setMsrQuarterTonesPitchesLanguageKindVariable (
    (*it).second);
}

void msdlPitchesLanguageAtom::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> msdlPitchesLanguageAtom::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_msdlPitchesLanguageAtom>*
    p =
      dynamic_cast<visitor<S_msdlPitchesLanguageAtom>*> (v)) {
        S_msdlPitchesLanguageAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching msdlPitchesLanguageAtom::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void msdlPitchesLanguageAtom::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> msdlPitchesLanguageAtom::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_msdlPitchesLanguageAtom>*
    p =
      dynamic_cast<visitor<S_msdlPitchesLanguageAtom>*> (v)) {
        S_msdlPitchesLanguageAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching msdlPitchesLanguageAtom::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void msdlPitchesLanguageAtom::browseData (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> msdlPitchesLanguageAtom::browseData ()" <<
      endl;
  }
#endif
}

string msdlPitchesLanguageAtom::asShortNamedOptionString () const
{
  stringstream s;

  s <<
    "-" << fShortName << " " <<
    msrQuarterTonesPitchesLanguageKindAsString (
      fMsrQuarterTonesPitchesLanguageKindVariable);

  return s.str ();
}

string msdlPitchesLanguageAtom::asActualLongNamedOptionString () const
{
  stringstream s;

  s <<
    "-" << fLongName << " " <<
    msrQuarterTonesPitchesLanguageKindAsString (
      fMsrQuarterTonesPitchesLanguageKindVariable);

  return s.str ();
}

void msdlPitchesLanguageAtom::print (ostream& os) const
{
  const unsigned int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "msdlPitchesLanguageAtom:" <<
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
    "msrQuarterTonesPitchesLanguageKindVariable" << " : \"" <<
    msrQuarterTonesPitchesLanguageKindAsString (
      fMsrQuarterTonesPitchesLanguageKindVariable) <<
    "\"" <<
    endl;

  --gIndenter;
}

void msdlPitchesLanguageAtom::printAtomWithVariableOptionsValues (
  ostream&     os,
  unsigned int valueFieldWidth) const
{
  os << left <<
    setw (valueFieldWidth) <<
    fVariableName <<
    " : \"" <<
    msrQuarterTonesPitchesLanguageKindAsString (
      fMsrQuarterTonesPitchesLanguageKindVariable) <<
    "\"";
  if (fVariableHasBeenSet) {
    os <<
      ", variableHasBeenSet: " <<
      booleanAsString (fVariableHasBeenSet);
  }
  os << endl;
}

ostream& operator<< (ostream& os, const S_msdlPitchesLanguageAtom& elt)
{
  elt->print (os);
  return os;
}

//_______________________________________________________________________________
S_msdlOahGroup gGlobalMsdlOahGroup;

S_msdlOahGroup msdlOahGroup::create (
  string executableName)
{
  msdlOahGroup* o = new msdlOahGroup (
    executableName);
  assert (o != nullptr);
  return o;
}

msdlOahGroup::msdlOahGroup (
  string executableName)
  : oahGroup (
    "MSDR",
    "hmsdl", "help-msdl",
R"(These options control the way MSDR data is handled.)",
    oahElementVisibilityKind::kElementVisibilityWhole)
{
  initializeMsdlOahGroup (
    executableName);
}

msdlOahGroup::~msdlOahGroup ()
{}

#ifdef TRACING_IS_ENABLED
void msdlOahGroup::initializeMsdlTraceOah ()
{
  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "MSDR Trace",
        "hmsdltrace", "help-msdl-trace",
R"()",
      oahElementVisibilityKind::kElementVisibilityWhole,
      this);

  appendSubGroupToGroup (subGroup);

  // trace MSDR

  fTraceMsdl            = false;

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "tmsdl", "trace-msdl",
R"(Write a trace of the MSDR graphs visiting activity to standard error.)",
        "traceMsdl",
        fTraceMsdl));
}
#endif

void msdlOahGroup::initializeMsdlDisplayOptions ()
{
  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Display",
        "hmsdld", "help-msdl-display",
R"()",
      oahElementVisibilityKind::kElementVisibilityWhole,
      this);

  appendSubGroupToGroup (subGroup);

  // display MSDR

  fDisplayMsdl = false;

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "dmsdl", "display-msdl",
R"(Write the contents of the MSDR data to standard error.)",
        "displayMsdl",
        fDisplayMsdl));

  // display MSDR short

  fDisplayMsdlShort = false;

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "dmsdls", "display-msdl-short",
R"(Write the contents of the MSDR data, short version, to standard error.)",
        "displayMsdlShort",
        fDisplayMsdlShort));
}

void msdlOahGroup::initializeMsdlLanguagesOptions (
  string executableName)
{
  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Languages",
        "hmsdll_JMI", "help-msdl-languages_JMI",
R"()",
      oahElementVisibilityKind::kElementVisibilityWhole,
      this);

  appendSubGroupToGroup (subGroup);

  // msdl pitches language

  if (! setMsdlQuarterTonesPitchesLanguage ("english")) {
    stringstream s;

    s <<
      "INTERNAL INITIALIZATION ERROR: "
      "MSDR pitches default language 'nederlands' is unknown" <<
      endl <<
      "The " <<
      gGlobalQuarterTonesPitchesLanguageKindsMap.size () <<
      " known MSDR pitches languages are:" <<
      endl;

    ++gIndenter;

    s <<
      existingQuarterTonesPitchesLanguageKinds (K_NAMES_LIST_MAX_LENGTH);

    --gIndenter;

    oahError (s.str ());
  }

  // MSDR pitches language

  const msrQuarterTonesPitchesLanguageKind
    msrQuarterTonesPitchesLanguageKindDefaultValue =
      fMsdlQuarterTonesPitchesLanguageKind;

  fMsdlQuarterTonesPitchesLanguageKind =
    msrQuarterTonesPitchesLanguageKindDefaultValue;

  subGroup->
    appendAtomToSubGroup (
      msdlPitchesLanguageAtom::create (
        "msdlpl_JMI", "msdl-pitches-language_JMI",
        regex_replace (
          regex_replace (
            regex_replace (
R"(Use LANGUAGE to input note pitches.
The NUMBER MSDR pitches languages available are:
PITCHES_LANGUAGES.
The default is 'DEFAULT_VALUE'.)",
              regex ("NUMBER"),
              to_string (gGlobalQuarterTonesPitchesLanguageKindsMap.size ())),
            regex ("PITCHES_LANGUAGES"),
            gIndenter.indentMultiLineString (
              existingQuarterTonesPitchesLanguageKinds (K_NAMES_LIST_MAX_LENGTH))),
          regex ("DEFAULT_VALUE"),
          msrQuarterTonesPitchesLanguageKindAsString (
            msrQuarterTonesPitchesLanguageKindDefaultValue)),
        "LANGUAGE",
        "msdlPitchesLanguage",
        fMsdlQuarterTonesPitchesLanguageKind));

  // MSDR keywords display

  const msdlKeywordsLanguageKind
    msdlKeywordsLanguageKindDefaultValue =
      msdlKeywordsLanguageKind::kKeywordsEnglish; // MSDL default

  fMsdlKeywordsLanguageKind =
    msdlKeywordsLanguageKindDefaultValue;

  subGroup->
    appendAtomToSubGroup (
      oahDisplayMsdlKeywordsInLanguageAtom::create (
        "dkil", "display-keywords-in-language",
        regex_replace (
          regex_replace (
            regex_replace (
R"(Display the keywords in language LANGUAGE.
The NUMBER MSDR keywords languages available are:
KEYWORDS_LANGUAGES.
The default is 'DEFAULT_VALUE'.)",
              regex ("NUMBER"),
              to_string (gGlobalMsdlKeywordsLanguageKindsMap.size ())),
            regex ("KEYWORDS_LANGUAGES"),
            gIndenter.indentMultiLineString (
              existingMsdlKeywordsLanguageKinds (K_NAMES_LIST_MAX_LENGTH))),
          regex ("DEFAULT_VALUE"),
          msdlKeywordsLanguageKindAsString (
            msdlKeywordsLanguageKindDefaultValue)),
        executableName));

  // MSDR tokens display

  const msdlKeywordsLanguageKind
    msdlTokensLanguageKindDefaultValue =
      msdlKeywordsLanguageKind::kKeywordsEnglish; // MSDL default

  fMsdlKeywordsLanguageKind =
    msdlTokensLanguageKindDefaultValue;

  subGroup->
    appendAtomToSubGroup (
      oahDisplayMsdlTokensInLanguageAtom::create (
        "dtil", "display-tokens-in-language",
        regex_replace (
          regex_replace (
            regex_replace (
R"(Display the tokens in language LANGUAGE.
The NUMBER MSDR tokens languages available are:
KEYWORDS_LANGUAGES.
The default is 'DEFAULT_VALUE'.)",
              regex ("NUMBER"),
              to_string (gGlobalMsdlKeywordsLanguageKindsMap.size ())),
            regex ("KEYWORDS_LANGUAGES"),
            gIndenter.indentMultiLineString (
              existingMsdlKeywordsLanguageKinds (K_NAMES_LIST_MAX_LENGTH))),
          regex ("DEFAULT_VALUE"),
          msdlKeywordsLanguageKindAsString (
            msdlTokensLanguageKindDefaultValue)),
        executableName));
}

void msdlOahGroup::initializeMsdlQuitAfterSomePassesOptions ()
{
  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Quit after some passes",
        "hmquit", "help-msdl-quit",
R"()",
      oahElementVisibilityKind::kElementVisibilityWhole,
      this);

  appendSubGroupToGroup (subGroup);

  // quit after pass 3

  fQuitAfterPass3 = false;

  S_oahBooleanAtom
    quit3OahBooleanAtom =
      oahBooleanAtom::create (
        "mq3", "msdl-quit-after-pass-3",
R"(Quit after pass 3, i.e. after conversion
of the MSR to MSDR.)",
        "quitAfterPass3",
        fQuitAfterPass3);

  subGroup->
    appendAtomToSubGroup (
      quit3OahBooleanAtom);
}

void msdlOahGroup::initializeMsdlOahGroup (
  string executableName)
{
#ifdef TRACING_IS_ENABLED
  // trace and display
  // --------------------------------------
  initializeMsdlTraceOah ();
#endif

  // display
  // --------------------------------------
  initializeMsdlDisplayOptions ();

  // languages
  // --------------------------------------
  initializeMsdlLanguagesOptions (
    executableName);

  // quit after some passes
  // --------------------------------------
  initializeMsdlQuitAfterSomePassesOptions ();
}

//______________________________________________________________________________
bool msdlOahGroup::setMsdlKeywordsLanguage (string language)
{
  // is language in the keywords languages map?
  map<string, msdlKeywordsLanguageKind>::const_iterator
    it =
      gGlobalMsdlKeywordsLanguageKindsMap.find (language);

  if (it == gGlobalMsdlKeywordsLanguageKindsMap.end ()) {
    // no, language is unknown in the map
    return false;
  }

  fMsdlKeywordsLanguageKind = (*it).second;

  return true;
}

//______________________________________________________________________________
bool msdlOahGroup::setMsdlQuarterTonesPitchesLanguage (string language)
{
  // is language in the note names languages map?
  map<string, msrQuarterTonesPitchesLanguageKind>::const_iterator
    it =
      gGlobalQuarterTonesPitchesLanguageKindsMap.find (language);

  if (it == gGlobalQuarterTonesPitchesLanguageKindsMap.end ()) {
    // no, language is unknown in the map
    return false;
  }

  fMsdlQuarterTonesPitchesLanguageKind = (*it).second;

  return true;
}

//______________________________________________________________________________
void msdlOahGroup::enforceGroupQuietness ()
{
#ifdef TRACING_IS_ENABLED
  fTraceMsdl = false;
#endif

  fDisplayMsdl = false;
  fDisplayMsdlShort = false;
}

//______________________________________________________________________________
void msdlOahGroup::checkGroupOptionsConsistency ()
{
  // JMI
}

//______________________________________________________________________________
void msdlOahGroup::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> msdlOahGroup::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_msdlOahGroup>*
    p =
      dynamic_cast<visitor<S_msdlOahGroup>*> (v)) {
        S_msdlOahGroup elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching msdlOahGroup::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void msdlOahGroup::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> msdlOahGroup::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_msdlOahGroup>*
    p =
      dynamic_cast<visitor<S_msdlOahGroup>*> (v)) {
        S_msdlOahGroup elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching msdlOahGroup::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void msdlOahGroup::browseData (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> msdlOahGroup::browseData ()" <<
      endl;
  }
#endif
}

//______________________________________________________________________________
void msdlOahGroup::printMsdlOahValues (unsigned int fieldWidth)
{
  gLogStream <<
    "The MSDR options are:" <<
    endl;

  ++gIndenter;

#ifdef TRACING_IS_ENABLED
  // trace
  // --------------------------------------
  gLogStream <<
    "Trace:" <<
    endl;

  ++gIndenter;

  gLogStream << left <<
    setw (fieldWidth) << "traceMsdl" << " : " <<
    booleanAsString (fTraceMsdl) <<
    endl <<

  --gIndenter;
#endif

  // display
  // --------------------------------------
  gLogStream <<
    "Display:" <<
    endl;

  ++gIndenter;

  gLogStream << left <<
    setw (fieldWidth) << "displayMsdl" << " : " <<
    booleanAsString (fDisplayMsdl) <<
    endl <<
    setw (fieldWidth) << "displayMsdlShort" << " : " <<
    booleanAsString (fDisplayMsdlShort) <<
    endl;

  --gIndenter;

  // languages
  // --------------------------------------

  gLogStream <<
    "Languages:" <<
    endl;

  ++gIndenter;

  gLogStream << left <<
    setw (fieldWidth) << "msdlQuarterTonesPitchesLanguage" << " : \"" <<
    msrQuarterTonesPitchesLanguageKindAsString (
      fMsdlQuarterTonesPitchesLanguageKind) <<
    "\"" <<
    endl <<

    setw (fieldWidth) << "msdlKeywordsLanguage" << " : \"" <<
    msdlKeywordsLanguageKindAsString (
      fMsdlKeywordsLanguageKind) <<
    "\"" <<
    endl;

  --gIndenter;
}

ostream& operator<< (ostream& os, const S_msdlOahGroup& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_msdlOahGroup createGlobalMsdlOahGroup (
  string executableName)
{
#ifdef TRACING_IS_ENABLED
  if (getTraceOah ()) {
    gLogStream <<
      "Creating global MSDR OAH group" <<
      endl;
  }
#endif

  // protect library against multiple initializations
  if (! gGlobalMsdlOahGroup) {
    // create the global options group
    gGlobalMsdlOahGroup =
      msdlOahGroup::create (
        executableName);
    assert (gGlobalMsdlOahGroup != 0);
  }

  // return the global OAH group
  return gGlobalMsdlOahGroup;
}


}
