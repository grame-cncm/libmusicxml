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
#include "bsr2BrailleOah.h"


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

S_bsr2BrailleOah gBsr2BrailleOah;
S_bsr2BrailleOah gBsr2BrailleOahUserChoices;
S_bsr2BrailleOah gBsr2BrailleOahWithDetailedTrace;

S_bsr2BrailleOah bsr2BrailleOah::create (
  S_oahHandler handlerUpLink)
{
  bsr2BrailleOah* o = new bsr2BrailleOah(
    handlerUpLink);
  assert(o!=0);
  return o;
}

bsr2BrailleOah::bsr2BrailleOah (
  S_oahHandler handlerUpLink)
  : oahGroup (
    "Bsr2Braille",
    "hbsr2brl", "help-bsr-to-braille",
R"(These options control the way BSR data is translated to braille music.)",
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
  initializeBsr2BrailleOah (false);
}

bsr2BrailleOah::~bsr2BrailleOah ()
{}

#ifdef TRACE_OAH
void bsr2BrailleOah::initializeBsr2BrailleTraceOah (
  bool boolOptionsInitialValue)
{}
#endif

void bsr2BrailleOah::initializeBrailleEncodingOptions (
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
              to_string (gBsrBrailleOutputKindsMap.size ())),
            regex ("BRAILLE_OUTPUT_KINDS"),
            existingBsrBrailleOutputKinds (K_NAMES_LIST_MAX_LENGTH)),
          regex ("DEFAULT_VALUE"),
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

void bsr2BrailleOah::initializeBrailleByteOrderingOptions (
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

void bsr2BrailleOah::initializeBrailleMusicFileNameOptions (
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

void bsr2BrailleOah::initializeBraillePageParametersOptions (
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

void bsr2BrailleOah::initializeBrailleCodeGenerationOptions (
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

void bsr2BrailleOah::initializeBsr2BrailleOah (
  bool boolOptionsInitialValue)
{
#ifdef TRACE_OAH
  // trace
  // --------------------------------------
  initializeBsr2BrailleTraceOah (
    boolOptionsInitialValue);
#endif

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

S_bsr2BrailleOah bsr2BrailleOah::createCloneWithDetailedTrace ()
{
  S_bsr2BrailleOah
    clone =
      bsr2BrailleOah::create (0);
      // 0 not to have it inserted twice in the option handler

  // set the options handler upLink
  clone->fHandlerUpLink =
    fHandlerUpLink;

  // code generation JMI ???
  // --------------------------------------

  clone->fXml2brlInfos =
    fXml2brlInfos;

  clone->fNoBrailleCode =
    fNoBrailleCode;

  return clone;
}

//______________________________________________________________________________
void bsr2BrailleOah::setAllBsr2BrailleTraceOah (
  bool boolOptionsInitialValue)
{
#ifdef TRACE_OAH
  // specific trace
  // JMI
#endif
}

//______________________________________________________________________________
void bsr2BrailleOah::enforceQuietness ()
{}

//______________________________________________________________________________
void bsr2BrailleOah::checkOptionsConsistency ()
{}

//______________________________________________________________________________
void bsr2BrailleOah::acceptIn (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    gLogOstream <<
      ".\\\" ==> bsr2BrailleOah::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_bsr2BrailleOah>*
    p =
      dynamic_cast<visitor<S_bsr2BrailleOah>*> (v)) {
        S_bsr2BrailleOah elem = this;

#ifdef TRACE_OAH
        if (gOahOah->fTraceOahVisitors) {
          gLogOstream <<
            ".\\\" ==> Launching bsr2BrailleOah::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void bsr2BrailleOah::acceptOut (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    gLogOstream <<
      ".\\\" ==> bsr2BrailleOah::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_bsr2BrailleOah>*
    p =
      dynamic_cast<visitor<S_bsr2BrailleOah>*> (v)) {
        S_bsr2BrailleOah elem = this;

#ifdef TRACE_OAH
        if (gOahOah->fTraceOahVisitors) {
          gLogOstream <<
            ".\\\" ==> Launching bsr2BrailleOah::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void bsr2BrailleOah::browseData (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    gLogOstream <<
      ".\\\" ==> bsr2BrailleOah::browseData ()" <<
      endl;
  }
#endif
}

//______________________________________________________________________________
void bsr2BrailleOah::printBsr2BrailleOahValues (int fieldWidth)
{
  gLogOstream <<
    "The Bsr2Braille options are:" <<
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

ostream& operator<< (ostream& os, const S_bsr2BrailleOah& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
void initializeBsr2BrailleOahHandling (
  S_oahHandler handler)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceOah && ! gGeneralOah->fQuiet) {
    gLogOstream <<
      "Initializing Bsr2Braille options handling" <<
      endl;
  }
#endif

  // create the Bsr2Braille options
  // ------------------------------------------------------

  gBsr2BrailleOahUserChoices = bsr2BrailleOah::create (
    handler);
  assert(gBsr2BrailleOahUserChoices != 0);

  gBsr2BrailleOah =
    gBsr2BrailleOahUserChoices;

  // prepare for measure detailed trace
  // ------------------------------------------------------

/* JMI
  gBsr2BrailleOahWithDetailedTrace =
    gBsr2BrailleOah->
      createCloneWithDetailedTrace ();
      */
}


}
