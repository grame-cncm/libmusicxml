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

#include "oahOah.h"
#include "generalOah.h"

#include "bsr2brailleOah.h"


using namespace std;

namespace MusicXML2
{

//______________________________________________________________________________
S_brailleOutputKindAtom brailleOutputKindAtom::create (
  const string&         shortName,
  const string&         longName,
  const string&         description,
  const string&         valueSpecification,
  const string&         variableName,
  bsrBrailleOutputKind& brailleOutputKindVariable)
{
  brailleOutputKindAtom* o = new
    brailleOutputKindAtom (
      shortName,
      longName,
      description,
      valueSpecification,
      variableName,
      brailleOutputKindVariable);
  assert (o != nullptr);
  return o;
}

brailleOutputKindAtom::brailleOutputKindAtom (
  const string&         shortName,
  const string&         longName,
  const string&         description,
  const string&         valueSpecification,
  const string&         variableName,
  bsrBrailleOutputKind& brailleOutputKindVariable)
  : oahAtomStoringAValueInAVariable (
      shortName,
      longName,
      description,
      valueSpecification,
      variableName),
    fBrailleOutputKindVariable (
      brailleOutputKindVariable)
{}

brailleOutputKindAtom::~brailleOutputKindAtom ()
{}

void brailleOutputKindAtom::applyAtomWithValue (
  const string& theString,
  ostream&      os)
{
  // theString contains the output kind name:
  // is it in the  output kinds map?

#ifdef TRACING_IS_ENABLED
  if (getTraceOah ()) {
    gLogStream <<
      "==> handling atom '" << fetchNames () << "; which is of type 'brailleOutputKindAtom'" <<
      " with value \"" << theString << "\"" <<
      endl;
  }
#endif

  map<string, bsrBrailleOutputKind>::const_iterator
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

    ++gIndenter;

    s <<
      existingBsrBrailleOutputKinds (K_NAMES_LIST_MAX_LENGTH);

    --gIndenter;

    oahError (s.str ());
  }

  fBrailleOutputKindVariable =
    (*it).second;
  fVariableHasBeenSet = true;
}

void brailleOutputKindAtom::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> brailleOutputKindAtom::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_brailleOutputKindAtom>*
    p =
      dynamic_cast<visitor<S_brailleOutputKindAtom>*> (v)) {
        S_brailleOutputKindAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching brailleOutputKindAtom::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void brailleOutputKindAtom::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> brailleOutputKindAtom::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_brailleOutputKindAtom>*
    p =
      dynamic_cast<visitor<S_brailleOutputKindAtom>*> (v)) {
        S_brailleOutputKindAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching brailleOutputKindAtom::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void brailleOutputKindAtom::browseData (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> brailleOutputKindAtom::browseData ()" <<
      endl;
  }
#endif
}

string brailleOutputKindAtom::asShortNamedOptionString () const
{
  stringstream s;

  s <<
    "-" << fShortName << " " <<
    bsrBrailleOutputKindAsString (fBrailleOutputKindVariable);

  return s.str ();
}

string brailleOutputKindAtom::asActualLongNamedOptionString () const
{
  stringstream s;

  s <<
    "-" << fLongName << " " <<
    bsrBrailleOutputKindAsString (fBrailleOutputKindVariable);

  return s.str ();
}

void brailleOutputKindAtom::print (ostream& os) const
{
  const unsigned int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "brailleOutputKindAtom:" <<
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
    bsrBrailleOutputKindAsString (
      fBrailleOutputKindVariable) <<
      "\"" <<
    endl;

  --gIndenter;
}

void brailleOutputKindAtom::printAtomWithVariableOptionsValues (
  ostream&     os,
  unsigned int valueFieldWidth) const
{
  os << left <<
    setw (valueFieldWidth) <<
    fVariableName <<
    " : \"" <<
    bsrBrailleOutputKindAsString (
      fBrailleOutputKindVariable) <<
    "\"";
  if (fVariableHasBeenSet) {
    os <<
      ", variableHasBeenSet: " <<
      booleanAsString (fVariableHasBeenSet);
  }
  os << endl;
}

ostream& operator<< (ostream& os, const S_brailleOutputKindAtom& elt)
{
  elt->print (os);
  return os;
}

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
  assert (o != nullptr);
  return o;
}

brailleUTFKindAtom::brailleUTFKindAtom (
  string      shortName,
  string      longName,
  string      description,
  string      valueSpecification,
  string      variableName,
  bsrUTFKind& brailleUTFKindVariable)
  : oahAtomStoringAValueInAVariable (
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
  const string& theString,
  ostream&      os)
{
#ifdef TRACING_IS_ENABLED
  if (getTraceOah ()) {
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
    "brailleUTFKindAtom:" <<
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
    bsrUTFKindAsString (
      fBsrUTFKindVariable) <<
      "\"" <<
    endl;

  --gIndenter;
}

void brailleUTFKindAtom::printAtomWithVariableOptionsValues (
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
  assert (o != nullptr);
  return o;
}

brailleByteOrderingKindAtom::brailleByteOrderingKindAtom (
  string               shortName,
  string               longName,
  string               description,
  string               valueSpecification,
  string               variableName,
  bsrByteOrderingKind& brailleByteOrderingKindVariable)
  : oahAtomStoringAValueInAVariable (
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
  const string& theString,
  ostream&      os)
{
#ifdef TRACING_IS_ENABLED
  if (getTraceOah ()) {
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
    "brailleByteOrderingKindAtom:" <<
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
    "brailleByteOrderingKindVariable" << " : \"" <<
    bsrByteOrderingKindAsString (
      fBsrByteOrderingKindVariable) <<
      "\"" <<
    endl;

  --gIndenter;
}

void brailleByteOrderingKindAtom::printAtomWithVariableOptionsValues (
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

//_______________________________________________________________________________

S_bsr2brailleOahGroup gGlobalBsr2brailleOahGroup;

S_bsr2brailleOahGroup bsr2brailleOahGroup::create ()
{
  bsr2brailleOahGroup* o = new bsr2brailleOahGroup ();
  assert (o != nullptr);
  return o;
}

bsr2brailleOahGroup::bsr2brailleOahGroup ()
  : oahGroup (
    "Bsr2braille",
    "h-mkk-bsr2brl", "help-mkk-bsr-to-braille",
R"(These options control the way BSR data is translated to braille music.)",
    oahElementVisibilityKind::kElementVisibilityWhole)
{
  fBrailleOutputKind = kBrailleOutputAscii; // default value

  fUTFKind = kUTF8; // default value

  fByteOrderingKind = kByteOrderingNone;

  // initialize it
  initializeBsr2brailleOahGroup ();
}

bsr2brailleOahGroup::~bsr2brailleOahGroup ()
{}

void bsr2brailleOahGroup::initializeBrailleEncodingOptions ()
{
  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Braille encoding",
        "hbe", "help-braille-encoding",
R"()",
      oahElementVisibilityKind::kElementVisibilityWhole,
      this);

  appendSubGroupToGroup (subGroup);

  const bsrBrailleOutputKind
    bsrBrailleOutputKindDefaultValue =
      kBrailleOutputAscii; // default value

  fBrailleOutputKindAtom =
      brailleOutputKindAtom::create (
      K_BRAILLE_OUTPUT_KIND_SHORT_NAME, K_BRAILLE_OUTPUT_KIND_LONG_NAME,
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
            to_string (gGlobalBsrBrailleOutputKindsMap.size ())),
          regex ("BRAILLE_OUTPUT_KINDS"),
          existingBsrBrailleOutputKinds (K_NAMES_LIST_MAX_LENGTH)),
        regex ("DEFAULT_VALUE"),
        bsrBrailleOutputKindAsString (
          bsrBrailleOutputKindDefaultValue)),
      "OUTPUT_KIND",
      "brailleOutputKind",
      fBrailleOutputKind);

  subGroup->
    appendAtomToSubGroup (
      fBrailleOutputKindAtom);
}

void bsr2brailleOahGroup::initializeBrailleByteOrderingOptions ()
{
  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Byte ordering",
        "hlpbo", "help-byte-ordering",
R"()",
      oahElementVisibilityKind::kElementVisibilityWhole,
      this);

  appendSubGroupToGroup (subGroup);

  // byte ordering

  fByteOrderingKind = kByteOrderingBigEndian; // default value

  fByteOrderingKindAtom =
    brailleByteOrderingKindAtom::create (
    K_BRAILLE_BYTE_ORDERING_MARK_SHORT_NAME, K_BRAILLE_BYTE_ORDERING_MARK_LONG_NAME,
R"(Generate an initial BOM_ENDIAN byte ordering mark (BOM)
ahead of the Braille nusic code,
which can be one of 'big' or 'small'.
By default, a big endian BOM is generated.)",
    "BOM_ENDIAN",
    "byteOrderingKind",
    fByteOrderingKind);

  subGroup->
    appendAtomToSubGroup (
      fByteOrderingKindAtom);
}

void bsr2brailleOahGroup::initializeBrailleMusicFileNameOptions ()
{
  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Braille music file name",
        "hlpbmfn", "help-braille-music-file-name",
R"()",
      oahElementVisibilityKind::kElementVisibilityWhole,
      this);

  appendSubGroupToGroup (subGroup);

  // encoding in file name

  fUseEncodingInFileName = false; // default value

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        K_BRAILLE_USE_ENCODING_IN_FILE_NAME_SHORT_NAME,
        K_BRAILLE_USE_ENCODING_IN_FILE_NAME_LONG_NAME,
R"(Append a description of the encoding used
and the presence of a BOM if any to the file name before the '.'.)",
        "useEncodingInFileName",
        fUseEncodingInFileName));
}

void bsr2brailleOahGroup::initializeBraillePageParametersOptions ()
{
  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Page parameters",
        "hlpbpp", "help-braille-pages-parameters",
R"()",
      oahElementVisibilityKind::kElementVisibilityWhole,
      this);

  appendSubGroupToGroup (subGroup);

  // cells per line

  fCellsPerLine = 30;
  subGroup->
    appendAtomToSubGroup (
      oahIntegerAtom::create (
        "cpl", "cells-per-line",
R"(Set the number of Braille cells per line to N. Default is 30 for A4 paper.)",
        "N",
        "cellsPerLine",
        fCellsPerLine));

  // measures per line

  fMeasuresPerLine = 7;

  subGroup->
    appendAtomToSubGroup (
      oahIntegerAtom::create (
        "mpl", "measures-per-line",
R"(Set the number of Braille measures per line to N. Default is 7.)",
        "N",
        "measuresPerLine",
        fMeasuresPerLine));

  // lines per page

  fLinesPerPage = 27;

  subGroup->
    appendAtomToSubGroup (
      oahIntegerAtom::create (
        "lpp", "lines-per-page",
R"(Set the number of Braille lines per page to N. Default is 27 for A4 paper.)",
        "N",
        "linesPerPage",
        fLinesPerPage));
}

void bsr2brailleOahGroup::initializeBrailleCodeGenerationOptions ()
{
  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Code generation",
        "hlpcg", "help-braille-code-generation",
R"()",
      oahElementVisibilityKind::kElementVisibilityWhole,
      this);

  appendSubGroupToGroup (subGroup);

  // xml2brl infos
  // --------------------------------------

  fXml2brlInfos = false;

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "x2bi", "xml2brl-infos",
R"(Generate initial comments showing the compilation date.)",
        "xml2brlInfos",
        fXml2brlInfos));

  // braille code
  // --------------------------------------

  fNoBrailleCode = false;

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "nobc", "no-braille-code",
R"(Don't generate any Braille code.
That can be useful if only a summary of the score is needed.)",
        "noBrailleCode",
        fNoBrailleCode));
}

void bsr2brailleOahGroup::initializeBsr2brailleOahGroup ()
{
#ifdef TRACING_IS_ENABLED
  // trace
  // --------------------------------------
// JMI  initializeBsr2brailleTraceOah ();
#endif

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
}

//______________________________________________________________________________
void bsr2brailleOahGroup::enforceGroupQuietness ()
{}

//______________________________________________________________________________
void bsr2brailleOahGroup::checkGroupOptionsConsistency ()
{}

//______________________________________________________________________________
void bsr2brailleOahGroup::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> bsr2brailleOahGroup::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_bsr2brailleOahGroup>*
    p =
      dynamic_cast<visitor<S_bsr2brailleOahGroup>*> (v)) {
        S_bsr2brailleOahGroup elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching bsr2brailleOahGroup::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void bsr2brailleOahGroup::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> bsr2brailleOahGroup::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_bsr2brailleOahGroup>*
    p =
      dynamic_cast<visitor<S_bsr2brailleOahGroup>*> (v)) {
        S_bsr2brailleOahGroup elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching bsr2brailleOahGroup::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void bsr2brailleOahGroup::browseData (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> bsr2brailleOahGroup::browseData ()" <<
      endl;
  }
#endif
}

//______________________________________________________________________________
void bsr2brailleOahGroup::printBsr2brailleOahValues (unsigned int fieldWidth)
{
  gLogStream <<
    "The Bsr2braille options are:" <<
    endl;

  ++gIndenter;

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

  --gIndenter;
}

ostream& operator<< (ostream& os, const S_bsr2brailleOahGroup& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_bsr2brailleOahGroup createGlobalBsr2brailleOahGroup ()
{
#ifdef TRACING_IS_ENABLED
  if (getTraceOah ()) {
    gLogStream <<
      "Creating global bsr2braille OAH group" <<
      endl;
  }
#endif

  // protect library against multiple initializations
  if (! gGlobalBsr2brailleOahGroup) {
    // create the Bsr2braille options
    // ------------------------------------------------------

    gGlobalBsr2brailleOahGroup =
      bsr2brailleOahGroup::create ();
    assert (gGlobalBsr2brailleOahGroup != 0);
  }

  // return the global OAH group
  return gGlobalBsr2brailleOahGroup;
}


}
