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

#include "utilities.h"

#include "setTraceOahIfDesired.h"
#ifdef TRACE_OAH
  #include "traceOah.h"
#endif

#include "generalOah.h"
#include "brailleOah.h"


using namespace std;

namespace MusicXML2
{

//______________________________________________________________________________
S_brailleOutputKindAtom brailleOutputKindAtom::create (
  string                shortName,
  string                longName,
  string                description,
  string                valueSpecification,
  string                variableName,
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
  assert(o!=0);
  return o;
}

brailleOutputKindAtom::brailleOutputKindAtom (
  string                shortName,
  string                longName,
  string                description,
  string                valueSpecification,
  string                variableName,
  bsrBrailleOutputKind& brailleOutputKindVariable)
  : oahValuedAtom (
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

S_oahValuedAtom brailleOutputKindAtom::handleOptionUnderName (
  string   optionName,
  ostream& os)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceOah) {
    gLogOstream <<
      "==> option '" << optionName << "' is a brailleOutputKindAtom" <<
      endl;
  }
#endif

  // an option value is needed
  return this;
}

void brailleOutputKindAtom::handleValue (
  string   theString,
  ostream& os)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceOah) {
    os <<
      "==> oahAtom is of type 'brailleOutputKindAtom'" <<
      endl;
  }
#endif

  // theString contains the output kind name:
  // is it in the  output kinds map?

  map<string, bsrBrailleOutputKind>::const_iterator
    it =
      gBsrBrailleOutputKindsMap.find (
        theString);

  if (it == gBsrBrailleOutputKindsMap.end ()) {
    // no, optional values style kind is unknown in the map
    stringstream s;

    s <<
      "braille ouptput kind '" << theString <<
      "' is unknown" <<
      endl <<
      "The " <<
      gBsrBrailleOutputKindsMap.size () - 1 <<
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
}

void brailleOutputKindAtom::acceptIn (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    gLogOstream <<
      ".\\\" ==> brailleOutputKindAtom::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_brailleOutputKindAtom>*
    p =
      dynamic_cast<visitor<S_brailleOutputKindAtom>*> (v)) {
        S_brailleOutputKindAtom elem = this;

#ifdef TRACE_OAH
        if (gOahOah->fTraceOahVisitors) {
          gLogOstream <<
            ".\\\" ==> Launching brailleOutputKindAtom::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void brailleOutputKindAtom::acceptOut (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    gLogOstream <<
      ".\\\" ==> brailleOutputKindAtom::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_brailleOutputKindAtom>*
    p =
      dynamic_cast<visitor<S_brailleOutputKindAtom>*> (v)) {
        S_brailleOutputKindAtom elem = this;

#ifdef TRACE_OAH
        if (gOahOah->fTraceOahVisitors) {
          gLogOstream <<
            ".\\\" ==> Launching brailleOutputKindAtom::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void brailleOutputKindAtom::browseData (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    gLogOstream <<
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

string brailleOutputKindAtom::asLongNamedOptionString () const
{
  stringstream s;

  s <<
    "-" << fLongName << " " <<
    bsrBrailleOutputKindAsString (fBrailleOutputKindVariable);

  return s.str ();
}

void brailleOutputKindAtom::print (ostream& os) const
{
  const int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "OptionsUTFKindAtom:" <<
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
    "brailleUTFKindVariable" << " : \"" <<
    bsrBrailleOutputKindAsString (
      fBrailleOutputKindVariable) <<
      "\"" <<
    endl;
}

void brailleOutputKindAtom::printAtomOptionsValues (
  ostream& os,
  int      valueFieldWidth) const
{
  os << left <<
    setw (valueFieldWidth) <<
    fVariableName <<
    " : \"" <<
    bsrBrailleOutputKindAsString (
      fBrailleOutputKindVariable) <<
    "\"" <<
    endl;
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
  assert(o!=0);
  return o;
}

brailleUTFKindAtom::brailleUTFKindAtom (
  string      shortName,
  string      longName,
  string      description,
  string      valueSpecification,
  string      variableName,
  bsrUTFKind& brailleUTFKindVariable)
  : oahValuedAtom (
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

void brailleUTFKindAtom::handleValue (
  string   theString,
  ostream& os)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceOah) {
    os <<
      "==> oahAtom is of type 'brailleUTFKindAtom'" <<
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
}

void brailleUTFKindAtom::acceptIn (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    gLogOstream <<
      ".\\\" ==> brailleUTFKindAtom::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_brailleUTFKindAtom>*
    p =
      dynamic_cast<visitor<S_brailleUTFKindAtom>*> (v)) {
        S_brailleUTFKindAtom elem = this;

#ifdef TRACE_OAH
        if (gOahOah->fTraceOahVisitors) {
          gLogOstream <<
            ".\\\" ==> Launching brailleUTFKindAtom::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void brailleUTFKindAtom::acceptOut (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    gLogOstream <<
      ".\\\" ==> brailleUTFKindAtom::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_brailleUTFKindAtom>*
    p =
      dynamic_cast<visitor<S_brailleUTFKindAtom>*> (v)) {
        S_brailleUTFKindAtom elem = this;

#ifdef TRACE_OAH
        if (gOahOah->fTraceOahVisitors) {
          gLogOstream <<
            ".\\\" ==> Launching brailleUTFKindAtom::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void brailleUTFKindAtom::browseData (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    gLogOstream <<
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

string brailleUTFKindAtom::asLongNamedOptionString () const
{
  stringstream s;

  s <<
    "-" << fLongName << " " <<
    bsrUTFKindAsString (fBsrUTFKindVariable);

  return s.str ();
}

void brailleUTFKindAtom::print (ostream& os) const
{
  const int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "OptionsUTFKindAtom:" <<
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
    "brailleUTFKindVariable" << " : \"" <<
    bsrUTFKindAsString (
      fBsrUTFKindVariable) <<
      "\"" <<
    endl;
}

void brailleUTFKindAtom::printAtomOptionsValues (
  ostream& os,
  int      valueFieldWidth) const
{
  os << left <<
    setw (valueFieldWidth) <<
    fVariableName <<
    " : \"" <<
    bsrUTFKindAsString (
      fBsrUTFKindVariable) <<
    "\"" <<
    endl;
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
  assert(o!=0);
  return o;
}

brailleByteOrderingKindAtom::brailleByteOrderingKindAtom (
  string               shortName,
  string               longName,
  string               description,
  string               valueSpecification,
  string               variableName,
  bsrByteOrderingKind& brailleByteOrderingKindVariable)
  : oahValuedAtom (
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

void brailleByteOrderingKindAtom::handleValue (
  string   theString,
  ostream& os)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceOah) {
    os <<
      "==> oahAtom is of type 'brailleByteOrderingKindAtom'" <<
      endl;
  }
#endif

  bsrByteOrderingKind byteOrderingKind = kByteOrderingNone; // default value

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
}

void brailleByteOrderingKindAtom::acceptIn (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    gLogOstream <<
      ".\\\" ==> brailleByteOrderingKindAtom::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_brailleByteOrderingKindAtom>*
    p =
      dynamic_cast<visitor<S_brailleByteOrderingKindAtom>*> (v)) {
        S_brailleByteOrderingKindAtom elem = this;

#ifdef TRACE_OAH
        if (gOahOah->fTraceOahVisitors) {
          gLogOstream <<
            ".\\\" ==> Launching brailleByteOrderingKindAtom::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void brailleByteOrderingKindAtom::acceptOut (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    gLogOstream <<
      ".\\\" ==> brailleByteOrderingKindAtom::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_brailleByteOrderingKindAtom>*
    p =
      dynamic_cast<visitor<S_brailleByteOrderingKindAtom>*> (v)) {
        S_brailleByteOrderingKindAtom elem = this;

#ifdef TRACE_OAH
        if (gOahOah->fTraceOahVisitors) {
          gLogOstream <<
            ".\\\" ==> Launching brailleByteOrderingKindAtom::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void brailleByteOrderingKindAtom::browseData (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    gLogOstream <<
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

string brailleByteOrderingKindAtom::asLongNamedOptionString () const
{
  stringstream s;

  s <<
    "-" << fLongName << " " <<
    bsrByteOrderingKindAsString (fBsrByteOrderingKindVariable);

  return s.str ();
}

void brailleByteOrderingKindAtom::print (ostream& os) const
{
  const int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "OptionsByteOrderingKindAtom:" <<
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
    "brailleByteOrderingKindVariable" << " : \"" <<
    bsrByteOrderingKindAsString (
      fBsrByteOrderingKindVariable) <<
      "\"" <<
    endl;
}

void brailleByteOrderingKindAtom::printAtomOptionsValues (
  ostream& os,
  int      valueFieldWidth) const
{
  os << left <<
    setw (valueFieldWidth) <<
    fVariableName <<
    " : \"" <<
    bsrByteOrderingKindAsString (
      fBsrByteOrderingKindVariable) <<
    "\"" <<
    endl;
}

ostream& operator<< (ostream& os, const S_brailleByteOrderingKindAtom& elt)
{
  elt->print (os);
  return os;
}

//_______________________________________________________________________________
S_brailleOah gBrailleOah;
S_brailleOah gBrailleOahUserChoices;
S_brailleOah gBrailleOahWithDetailedTrace;

S_brailleOah brailleOah::create (
  S_oahHandler handlerUpLink)
{
  brailleOah* o = new brailleOah (
    handlerUpLink);
  assert(o!=0);
  return o;
}

brailleOah::brailleOah (
  S_oahHandler handlerUpLink)
  : oahGroup (
    "Braille",
    "hbrl", "help-braille",
R"(These options control how the Braille code is generated.)",
    kElementVisibilityAlways,
    handlerUpLink)
{
  // append this braille group to the braille handler
  // if relevant
  if (handlerUpLink) {
    handlerUpLink->
      appendGroupToHandler (this);
  }

  // initialize it
  initializeBrailleOah (false);
}

brailleOah::~brailleOah ()
{}

void brailleOah::acceptIn (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    gLogOstream <<
      ".\\\" ==> brailleOah::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_brailleOah>*
    p =
      dynamic_cast<visitor<S_brailleOah>*> (v)) {
        S_brailleOah elem = this;

#ifdef TRACE_OAH
        if (gOahOah->fTraceOahVisitors) {
          gLogOstream <<
            ".\\\" ==> Launching brailleOah::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void brailleOah::acceptOut (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    gLogOstream <<
      ".\\\" ==> brailleOah::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_brailleOah>*
    p =
      dynamic_cast<visitor<S_brailleOah>*> (v)) {
        S_brailleOah elem = this;

#ifdef TRACE_OAH
        if (gOahOah->fTraceOahVisitors) {
          gLogOstream <<
            ".\\\" ==> Launching brailleOah::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void brailleOah::browseData (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    gLogOstream <<
      ".\\\" ==> brailleOah::browseData ()" <<
      endl;
  }
#endif
}

void brailleOah::initializeBrailleEncodingOptions (
  bool boolOptionsInitialValue)
{
  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Braille encoding",
        "hbe", "help-braille-encoding",
R"()",
      kElementVisibilityAlways,
      this);

  appendSubGroupToGroup (subGroup);

  const bsrBrailleOutputKind
    bsrBrailleOutputKindDefaultValue =
      kBrailleOutputAscii; // default value

  subGroup->
    appendAtomToSubGroup (
      brailleOutputKindAtom::create (
        "bok", "braille-output-kind",
        replaceSubstringInString (
          replaceSubstringInString (
            replaceSubstringInString (
R"(Use OUTPUT_KIND to write the generated braille music to the output.
The NUMBER output kinds available are:
BRAILLE_OUTPUT_KINDS.
The default is 'DEFAULT_VALUE'.)",
              "NUMBER",
              to_string (gBsrBrailleOutputKindsMap.size ())),
            "BRAILLE_OUTPUT_KINDS",
            existingBsrBrailleOutputKinds (K_NAMES_LIST_MAX_LENGTH)),
          "DEFAULT_VALUE",
          bsrBrailleOutputKindAsString (
            bsrBrailleOutputKindDefaultValue)),
        "OUTPUT_KIND",
        "brailleOutputKind",
        fBrailleOutputKind));

/* JMI
  // UTF encoding

  fUTFKind = kUTF8; // default value

  subGroup->
    appendAtomToSubGroup (
      brailleUTFKindAtom::create (
        "utf", "utf-encoding",
R"(Generate Braille code using UTF ENCODING_SIZE encoding,
which can be one of 8 or 16. Default value is 8.)",
        "ENCODING_SIZE",
        "UTFKind",
        fUTFKind));
        */
}

void brailleOah::initializeBrailleByteOrderingOptions (
  bool boolOptionsInitialValue)
{
/* JMI
  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Byte ordering",
        "hlpbo", "help-byte-ordering",
R"()",
      kElementVisibilityAlways,
      this);

  appendSubGroupToGroup (subGroup);

  // byte ordering

  fByteOrderingKind = kByteOrderingNone; // default value

  subGroup->
    appendAtomToSubGroup (
      brailleByteOrderingKindAtom::create (
        "bom", "byte-ordering-mark",
R"(Generate an initial BOM_ENDIAN byte ordering mark (BOM)
ahead of the Braille nusic code,
which can be one of 'big' or 'small'.
By default, a BOM is generated.)",
        "BOM_ENDIAN",
        "byteOrderingKind",
        fByteOrderingKind));
        */
}

void brailleOah::initializeBrailleMusicFileNameOptions (
  bool boolOptionsInitialValue)
{
  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Braille music file name",
        "hlpbmfn", "help-braille-music-file-name",
R"()",
      kElementVisibilityAlways,
      this);

  appendSubGroupToGroup (subGroup);

  // encoding in file name

  fDontUseEncodingInFileName = false; // default value

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "ueifn", "use-encoding-in-file-name",
R"(Don't append a description of the encoding used
and the presence of a BOM if any to the file name.)",
        "dontUseEncodingInFileName",
        fDontUseEncodingInFileName));
}

void brailleOah::initializeBraillePageParametersOptions (
  bool boolOptionsInitialValue)
{
  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Page parameters",
        "hlpbpp", "help-braille-pages-parameters",
R"()",
      kElementVisibilityAlways,
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

void brailleOah::initializeBrailleCodeGenerationOptions (
  bool boolOptionsInitialValue)
{
  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Code generation",
        "hlpcg", "help-braille-code-generation",
R"()",
      kElementVisibilityAlways,
      this);

  appendSubGroupToGroup (subGroup);

  // xml2brl infos

  fXml2brlInfos = boolOptionsInitialValue;

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "xi", "xml2brl-infos",
R"(Generate initial comments showing the compilation date and braille.)",
        "xml2brlInfos",
        fXml2brlInfos));

  // braille code

  fNoBrailleCode = boolOptionsInitialValue;

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "nolpc", "no-braille-code",
R"(Don't generate any Braille code.
That can be useful if only a summary of the score is needed.)",
        "noBrailleCode",
        fNoBrailleCode));
}

void brailleOah::initializeBrailleOah (
  bool boolOptionsInitialValue)
{
  // UTF encoding
  // --------------------------------------
  initializeBrailleEncodingOptions (
    boolOptionsInitialValue);

  // byte ordering
  // --------------------------------------
  initializeBrailleByteOrderingOptions (
    boolOptionsInitialValue);

  // braille music file name
  // --------------------------------------
  initializeBrailleMusicFileNameOptions (
    boolOptionsInitialValue);

  // page parameters
  // --------------------------------------
  initializeBraillePageParametersOptions (
    boolOptionsInitialValue);

  // code generation
  // --------------------------------------
  initializeBrailleCodeGenerationOptions (
    boolOptionsInitialValue);
}

S_brailleOah brailleOah::createCloneWithDetailedTrace ()
{
  S_brailleOah
    clone =
      brailleOah::create (0);
      // 0 not to have it inserted twice in the option handler

  // set the braille handler upLink
  clone->fHandlerUpLink =
    fHandlerUpLink;;

  // code generation JMI ???
  // --------------------------------------

  clone->fXml2brlInfos =
    fXml2brlInfos;

  clone->fNoBrailleCode =
    fNoBrailleCode;

  return clone;
}

//______________________________________________________________________________
void brailleOah::enforceQuietness ()
{}

//______________________________________________________________________________
void brailleOah::checkOptionsConsistency ()
{
  // JMI
}

void brailleOah::printBrailleOahValues (int fieldWidth)
{
  gLogOstream <<
    "The Braille braille are:" <<
    endl;

  gIndenter++;


  // code generation
  // --------------------------------------

  gLogOstream <<
    "Braille code generation:" <<
    endl;

  gIndenter++;

  gLogOstream << left <<
    setw (fieldWidth) << "UTFKind" << " : " <<
      bsrUTFKindAsString (fUTFKind) <<
      endl <<

    setw (fieldWidth) << "byteOrderingKind" << " : " <<
      bsrByteOrderingKindAsString (fByteOrderingKind) <<
      endl <<

    setw (fieldWidth) << "dontUseEncodingInFileName" << " : " <<
      booleanAsString (fDontUseEncodingInFileName) <<
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

  gIndenter--;

  gIndenter--;
}

ostream& operator<< (ostream& os, const S_brailleOah& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
void initializeBrailleOahHandling (
  S_oahHandler handler)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceOah && ! gGeneralOah->fQuiet) {
    gLogOstream <<
      "Initializing Braille braille handling" <<
      endl;
  }
#endif

  // create the braille variables
  // ------------------------------------------------------

  gBrailleOahUserChoices = brailleOah::create (
    handler);
  assert(gBrailleOahUserChoices != 0);

  gBrailleOah =
    gBrailleOahUserChoices;

  // prepare for measure detailed trace
  // ------------------------------------------------------

/* JMI
  gBrailleOahWithDetailedTrace =
    gBrailleOah->
      createCloneWithDetailedTrace ();
      */
}


}


  /*
  // identification
  // --------------------------------------

  {
    // variables

    S_oahSubGroup
      subGroup =
        oahSubGroup::create (
          "Identification",
          "hlpi", "help-braille-identification",
R"(These braille can be used to enforce values in the generated Braille code,
thus overriding the ones that may be present in the MSR data.
)",
        kElementVisibilityAlways,
        this);

    appendSubGroupToGroup (subGroup);

    // MusicXML informations

    subGroup->
      appendAtomToSubGroup (
        oahStringAtom::create (
          "rights", "",
R"(Set the 'rights' to STRING in the Braille code.)",
          "STRING",
          "rights",
          fRights));

    subGroup->
      appendAtomToSubGroup (
        oahStringAtom::create (
          "composer", "",
R"(Set the 'composer' to STRING in the Braille code.)",
          "STRING",
          "composer",
          fComposer));

    subGroup->
      appendAtomToSubGroup (
        oahStringAtom::create (
          "arranger", "",
R"(Set the 'arranger' to STRING in the Braille code.)",
          "STRING",
          "arranger",
          fArranger));

    subGroup->
      appendAtomToSubGroup (
        oahStringAtom::create (
          "poet", "",
R"(Set the 'poet' to STRING in the Braille code.)",
          "STRING",
          "poet",
          fPoet));

    subGroup->
      appendAtomToSubGroup (
        oahStringAtom::create (
          "lyricist", "",
R"(Set the 'lyricist' to STRING in the Braille code.)",
          "STRING",
          "lyricist",
          fLyricist));

    subGroup->
      appendAtomToSubGroup (
        oahStringAtom::create (
          "software", "",
R"(Set the 'software' to STRING in the Braille code.)",
          "STRING",
          "software",
          fSoftware));

    // Braille informations

    subGroup->
      appendAtomToSubGroup (
        oahStringAtom::create (
          "dedication", "",
R"(Set 'dedication' to STRING in the \header.)",
          "STRING",
          "dedication",
          fDedication));

    subGroup->
      appendAtomToSubGroup (
        oahStringAtom::create (
          "piece", "",
R"(Set 'piece' to STRING in the \header.)",
          "STRING",
          "piece",
          fPiece));

    subGroup->
      appendAtomToSubGroup (
        oahStringAtom::create (
          "opus", "",
R"(Set 'opus' to STRING in the \header.)",
          "STRING",
          "opus",
          fOpus));

    subGroup->
      appendAtomToSubGroup (
        oahStringAtom::create (
          "title", "",
R"(Set 'title' to STRING in the \header.)",
          "STRING",
          "title",
          fTitle));

    subGroup->
      appendAtomToSubGroup (
        oahStringAtom::create (
          "stitle", "subtitle",
R"(Set 'subtitle' to STRING in the \header.)",
          "STRING",
          "subTitle",
          fSubTitle));

    subGroup->
      appendAtomToSubGroup (
        oahStringAtom::create (
          "sstitle", "subsubtitle",
R"(Set 'subsubtitle' to STRING in the \header.)",
          "STRING",
          "subSubTitle",
          fSubSubTitle));

    subGroup->
      appendAtomToSubGroup (
        oahStringAtom::create (
          "instrument", "",
R"(Set 'instrument' to STRING in the \header.)",
          "STRING",
          "instrument",
          fInstrument));

    subGroup->
      appendAtomToSubGroup (
        oahStringAtom::create (
          "meter", "",
R"(Set 'meter' to STRING in the \header.)",
          "STRING",
          "meter",
          fMeter));

    subGroup->
      appendAtomToSubGroup (
        oahStringAtom::create (
          "tagline", "",
R"(Set 'tagline' to STRING in the \header.)",
          "STRING",
          "tagline",
          fTagline));

    subGroup->
      appendAtomToSubGroup (
        oahStringAtom::create (
          "copyright", "",
R"(Set 'copyright' to STRING in the \header.)",
          "STRING",
          "copyright",
          fCopyright));
  }


  // notes
  // --------------------------------------

  {
    // variables

    fAbsoluteOctaves  = boolOptionsInitialValue;

    fAllDurations  = boolOptionsInitialValue;

    fRomanStringNumbers = boolOptionsInitialValue;
    fAvoidOpenStrings    = boolOptionsInitialValue;

    fCompressMultiMeasureRests = boolOptionsInitialValue;

    fInputLineNumbers = boolOptionsInitialValue;

    S_oahSubGroup
      subGroup =
        oahSubGroup::create (
          "Notes",
          "hlpn", "help-braille-notes",
R"()",
        kElementVisibilityAlways,
        this);

    appendSubGroupToGroup (subGroup);

    subGroup->
      appendAtomToSubGroup (
        oahBooleanAtom::create (
          "abs", "absolute",
R"(Generate Braille absolute note octaves.
By default, relative octaves are generated.)",
          "absoluteOctaves",
          fAbsoluteOctaves));

    subGroup->
      appendAtomToSubGroup (
        oahBooleanAtom::create (
          "alldurs", "all-durations",
R"(Generate all Braille durations.
By default, a duration equal to preceding one found in the current voice
is omitted for code conciseness.)",
          "allDurations",
          fAllDurations));

    subGroup->
      appendAtomToSubGroup (
        oahBooleanAtom::create (
          "rsn", "roman-string-numbers",
R"(Generate '\romanStringNumbers' in each voice
for Braille to generate roman instead of arabic string numbers.)",
          "romanStringNumbers",
          fRomanStringNumbers));

    subGroup->
      appendAtomToSubGroup (
        oahBooleanAtom::create (
          "aos", "avoid-open-strings",
R"(Generate '\set TabStaff.restrainOpenStrings = ##t' in each voice
to prevent Braille from using open strings.)",
          "avoidOpenStrings",
          fAvoidOpenStrings));

    subGroup->
      appendAtomToSubGroup (
        oahBooleanAtom::create (
          "niln", "note-input-line-numbers",
          replaceSubstringInString (
R"(Generate after each note and barline a comment containing
its MusicXML input line number.
This is useful when debugging EXECUTABLE.)",
            "EXECUTABLE",
            gOahOah->fHandlerExecutableName),
          "noteInputLineNumbers",
          fInputLineNumbers));
  }


  // bars
  // --------------------------------------

  {
    // variables

    fShowAllBarNumbers = boolOptionsInitialValue;

    // braille

    S_oahSubGroup
      subGroup =
        oahSubGroup::create (
          "Bars",
          "hlpb", "help-braille-bars",
R"()",
        kElementVisibilityAlways,
        this);

    appendSubGroupToGroup (subGroup);

    subGroup->
      appendAtomToSubGroup (
        oahBooleanAtom::create (
          "abn", "all-bar-numbers",
R"(Generate Braille code to show all bar numbers.)",
          "showAllBarNumbers",
          fShowAllBarNumbers));
  }


  // line breaks
  // --------------------------------------

  {
    // variables

    fIgnoreLineBreaks                    = boolOptionsInitialValue;

    fBreakLinesAtIncompleteRightMeasures = boolOptionsInitialValue;

    // braille

    S_oahSubGroup
      subGroup =
        oahSubGroup::create (
          "Line breaks",
          "hlplb", "help-braille-line-breaks",
R"()",
        kElementVisibilityAlways,
        this);

    appendSubGroupToGroup (subGroup);

    subGroup->
      appendAtomToSubGroup (
        oahBooleanAtom::create (
          "ilb", "ignore-line-breaks",
R"(Ignore the line breaks from the MusicXML input
and let Braille decide about them.)",
          "ignoreLineBreaks",
          fIgnoreLineBreaks));

    subGroup->
      appendAtomToSubGroup (
        oahBooleanAtom::create (
          "blairm", "break-lines-at-incomplete-right-measures",
R"(Generate a '\break' command at the end of incomplete right measures
which is handy in popular folk dances and tunes.)",
          "breakLinesAtIncompleteRightMeasures",
          fBreakLinesAtIncompleteRightMeasures));
  }


  // page breaks
  // --------------------------------------

  {
    // variables

    fIgnorePageBreaks = boolOptionsInitialValue;

    // braille

    S_oahSubGroup
      subGroup =
        oahSubGroup::create (
          "Page breaks",
          "hlppb", "help-braille-page-breaks",
R"()",
        kElementVisibilityAlways,
        this);

    appendSubGroupToGroup (subGroup);

    subGroup->
      appendAtomToSubGroup (
        oahBooleanAtom::create (
          "ipb", "ignore-page-breaks",
R"(Ignore the page breaks from the MusicXML input
and let Braille decide about them.)",
          "ignorePageBreaks",
          fIgnorePageBreaks));
  }
        */
