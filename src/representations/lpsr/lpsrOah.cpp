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

#include "lpsrOah.h"

#include "messagesHandling.h"


using namespace std;

namespace MusicXML2
{

//______________________________________________________________________________
S_lpsrPitchesLanguageAtom lpsrPitchesLanguageAtom::create (
  string             shortName,
  string             longName,
  string             description,
  string             valueSpecification,
  string             variableName,
  msrQuarterTonesPitchesLanguageKind&
                     lpsrPitchesLanguageKindVariable)
{
  lpsrPitchesLanguageAtom* o = new
    lpsrPitchesLanguageAtom (
      shortName,
      longName,
      description,
      valueSpecification,
      variableName,
      lpsrPitchesLanguageKindVariable);
  assert (o!=0);
  return o;
}

lpsrPitchesLanguageAtom::lpsrPitchesLanguageAtom (
  string             shortName,
  string             longName,
  string             description,
  string             valueSpecification,
  string             variableName,
  msrQuarterTonesPitchesLanguageKind&
                     lpsrPitchesLanguageKindVariable)
  : oahAtomWithValue (
      shortName,
      longName,
      description,
      valueSpecification,
      variableName),
    fMsrQuarterTonesPitchesLanguageKindVariable (
      lpsrPitchesLanguageKindVariable)
{}

lpsrPitchesLanguageAtom::~lpsrPitchesLanguageAtom ()
{}

void lpsrPitchesLanguageAtom::applyAtomWithValue (
  string   theString,
  ostream& os)
{

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceOah ()) {
    gLogStream <<
      "==> oahAtom is of type 'lpsrPitchesLanguageAtom'" <<
      endl;
  }
#endif

  // theString contains the language name:
  // is it in the pitches languages map?

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceOah ()) {
    gLogStream <<
      "==> oahAtom is of type 'lpsrPitchesLanguageAtom'" <<
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
      "LPSR pitches language '" << theString <<
      "' is unknown" <<
      endl <<
      "The " <<
      gGlobalQuarterTonesPitchesLanguageKindsMap.size () <<
      " known LPSR pitches languages are:" <<
      endl;

    gIndenter++;

    s <<
      existingQuarterTonesPitchesLanguageKinds (K_NAMES_LIST_MAX_LENGTH);

    gIndenter--;

    oahError (s.str ());
  }

  setMsrQuarterTonesPitchesLanguageKindVariable (
    (*it).second);
}

void lpsrPitchesLanguageAtom::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> lpsrPitchesLanguageAtom::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_lpsrPitchesLanguageAtom>*
    p =
      dynamic_cast<visitor<S_lpsrPitchesLanguageAtom>*> (v)) {
        S_lpsrPitchesLanguageAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching lpsrPitchesLanguageAtom::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void lpsrPitchesLanguageAtom::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> lpsrPitchesLanguageAtom::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_lpsrPitchesLanguageAtom>*
    p =
      dynamic_cast<visitor<S_lpsrPitchesLanguageAtom>*> (v)) {
        S_lpsrPitchesLanguageAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching lpsrPitchesLanguageAtom::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void lpsrPitchesLanguageAtom::browseData (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> lpsrPitchesLanguageAtom::browseData ()" <<
      endl;
  }
#endif
}

string lpsrPitchesLanguageAtom::asShortNamedOptionString () const
{
  stringstream s;

  s <<
    "-" << fShortName << " " <<
    msrQuarterTonesPitchesLanguageKindAsString (
      fMsrQuarterTonesPitchesLanguageKindVariable);

  return s.str ();
}

string lpsrPitchesLanguageAtom::asActualLongNamedOptionString () const
{
  stringstream s;

  s <<
    "-" << fLongName << " " <<
    msrQuarterTonesPitchesLanguageKindAsString (
      fMsrQuarterTonesPitchesLanguageKindVariable);

  return s.str ();
}

void lpsrPitchesLanguageAtom::print (ostream& os) const
{
  const int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "lpsrPitchesLanguageAtom:" <<
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
    "fOptionsLpsrPitchesLanguageVariable" << " : \"" <<
    msrQuarterTonesPitchesLanguageKindAsString (
      fMsrQuarterTonesPitchesLanguageKindVariable) <<
    "\"" <<
    endl;

  gIndenter--;
}

void lpsrPitchesLanguageAtom::printAtomWithValueOptionsValues (
  ostream& os,
  int      valueFieldWidth) const
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

ostream& operator<< (ostream& os, const S_lpsrPitchesLanguageAtom& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_lpsrChordsLanguageAtom lpsrChordsLanguageAtom::create (
  string             shortName,
  string             longName,
  string             description,
  string             valueSpecification,
  string             variableName,
  lpsrChordsLanguageKind&
                     lpsrChordsLanguageKindVariable)
{
  lpsrChordsLanguageAtom* o = new
    lpsrChordsLanguageAtom (
      shortName,
      longName,
      description,
      valueSpecification,
      variableName,
      lpsrChordsLanguageKindVariable);
  assert (o!=0);
  return o;
}

lpsrChordsLanguageAtom::lpsrChordsLanguageAtom (
  string             shortName,
  string             longName,
  string             description,
  string             valueSpecification,
  string             variableName,
  lpsrChordsLanguageKind&
                     lpsrChordsLanguageKindVariable)
  : oahAtomWithValue (
      shortName,
      longName,
      description,
      valueSpecification,
      variableName),
    fLpsrChordsLanguageKindVariable (
      lpsrChordsLanguageKindVariable)
{}

lpsrChordsLanguageAtom::~lpsrChordsLanguageAtom ()
{}

void lpsrChordsLanguageAtom::applyAtomWithValue (
  string   theString,
  ostream& os)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceOah ()) {
    gLogStream <<
      "==> oahAtom is of type 'lpsrChordsLanguageAtom'" <<
      endl;
  }
#endif

  // theString contains the language name:
  // is it in the chords languages map?

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceOah ()) {
    gLogStream <<
      "==> oahAtom is of type 'lpsrChordsLanguageAtom'" <<
      endl;
  }
#endif

  map<string, lpsrChordsLanguageKind>::const_iterator
    it =
      gGlobalLpsrChordsLanguageKindsMap.find (theString);

  if (it == gGlobalLpsrChordsLanguageKindsMap.end ()) {
    // no, language is unknown in the map
    stringstream s;

    s <<
      "LPSR chords language '" << theString <<
      "' is unknown" <<
      endl <<
      "The " <<
      gGlobalLpsrChordsLanguageKindsMap.size () - 1 <<
      " known LPSR chords languages apart from the default Ignatzek are:" <<
      endl;

    gIndenter++;

    s <<
      existingLpsrChordsLanguageKinds (K_NAMES_LIST_MAX_LENGTH);

    gIndenter--;

    oahError (s.str ());
  }

  setLpsrChordsLanguageKindVariable (
    (*it).second);
}

void lpsrChordsLanguageAtom::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> lpsrChordsLanguageAtom::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_lpsrChordsLanguageAtom>*
    p =
      dynamic_cast<visitor<S_lpsrChordsLanguageAtom>*> (v)) {
        S_lpsrChordsLanguageAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching lpsrChordsLanguageAtom::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void lpsrChordsLanguageAtom::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> lpsrChordsLanguageAtom::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_lpsrChordsLanguageAtom>*
    p =
      dynamic_cast<visitor<S_lpsrChordsLanguageAtom>*> (v)) {
        S_lpsrChordsLanguageAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching lpsrChordsLanguageAtom::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void lpsrChordsLanguageAtom::browseData (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> lpsrChordsLanguageAtom::browseData ()" <<
      endl;
  }
#endif
}

string lpsrChordsLanguageAtom::asShortNamedOptionString () const
{
  stringstream s;

  s <<
    "-" << fShortName << " " <<
    lpsrChordsLanguageKindAsString (fLpsrChordsLanguageKindVariable);

  return s.str ();
}

string lpsrChordsLanguageAtom::asActualLongNamedOptionString () const
{
  stringstream s;

  s <<
    "-" << fLongName << " " <<
    lpsrChordsLanguageKindAsString (fLpsrChordsLanguageKindVariable);

  return s.str ();
}

void lpsrChordsLanguageAtom::print (ostream& os) const
{
  const int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "lpsrChordsLanguageAtom:" <<
    endl;

  gIndenter++;

  printAtomWithValueEssentials (
    os, fieldWidth);

  os << left <<
    setw (fieldWidth) <<
    "fVariableName" << " : " <<
    fVariableName <<
    setw (fieldWidth) <<
    "fLpsrChordsLanguageKindVariable" << " : \"" <<
    lpsrChordsLanguageKindAsString (
      fLpsrChordsLanguageKindVariable) <<
    "\"" <<
    endl;

  gIndenter--;
}

void lpsrChordsLanguageAtom::printAtomWithValueOptionsValues (
  ostream& os,
  int      valueFieldWidth) const
{
  os << left <<
    setw (valueFieldWidth) <<
    fVariableName <<
    " : \"" <<
    lpsrChordsLanguageKindAsString (
      fLpsrChordsLanguageKindVariable) <<
    "\"";
  if (fVariableHasBeenSet) {
    os <<
      ", variableHasBeenSet: " <<
      booleanAsString (fVariableHasBeenSet);
  }
  os << endl;
}

ostream& operator<< (ostream& os, const S_lpsrChordsLanguageAtom& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_lpsrTransposeAtom lpsrTransposeAtom::create (
  string  shortName,
  string  longName,
  string  description,
  string  valueSpecification,
  string  variableName,
  S_msrSemiTonesPitchAndOctave&
          semiTonesPitchAndOctaveVariable)
{
  lpsrTransposeAtom* o = new
    lpsrTransposeAtom (
      shortName,
      longName,
      description,
      valueSpecification,
      variableName,
      semiTonesPitchAndOctaveVariable);
  assert (o!=0);
  return o;
}

lpsrTransposeAtom::lpsrTransposeAtom (
  string  shortName,
  string  longName,
  string  description,
  string  valueSpecification,
  string  variableName,
  S_msrSemiTonesPitchAndOctave&
          semiTonesPitchAndOctaveVariable)
  : oahAtomWithValue (
      shortName,
      longName,
      description,
      valueSpecification,
      variableName),
    fSemiTonesPitchAndOctaveVariable (
      semiTonesPitchAndOctaveVariable)
{}

lpsrTransposeAtom::~lpsrTransposeAtom ()
{}

void lpsrTransposeAtom::applyAtomWithValue (
  string   theString,
  ostream& os)
{

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceOah ()) {
    gLogStream <<
      "==> oahAtom is of type 'lpsrTransposeAtom'" <<
      endl;
  }
#endif

  // theString contains the language name:
  // is it in the chords languages map?

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceOah ()) {
    gLogStream <<
      "==> oahAtom is of type 'lpsrTransposeAtom'" <<
      endl;
  }
#endif

  // create the semitones pitch and octave from theString
  S_msrSemiTonesPitchAndOctave
    semiTonesPitchAndOctave =
      msrSemiTonesPitchAndOctave::createFromString (
        K_NO_INPUT_LINE_NUMBER,
        theString);

  // set the transpose atom variable value
  setSemiTonesPitchAndOctaveVariable (
    semiTonesPitchAndOctave);
}

void lpsrTransposeAtom::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> lpsrTransposeAtom::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_lpsrTransposeAtom>*
    p =
      dynamic_cast<visitor<S_lpsrTransposeAtom>*> (v)) {
        S_lpsrTransposeAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching lpsrTransposeAtom::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void lpsrTransposeAtom::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> lpsrTransposeAtom::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_lpsrTransposeAtom>*
    p =
      dynamic_cast<visitor<S_lpsrTransposeAtom>*> (v)) {
        S_lpsrTransposeAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching lpsrTransposeAtom::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void lpsrTransposeAtom::browseData (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> lpsrTransposeAtom::browseData ()" <<
      endl;
  }
#endif
}

string lpsrTransposeAtom::asShortNamedOptionString () const
{
  stringstream s;

  s <<
    "-" << fShortName << " " <<
    fSemiTonesPitchAndOctaveVariable->asString ();

  return s.str ();
}

string lpsrTransposeAtom::asActualLongNamedOptionString () const
{
  stringstream s;

  s <<
    "-" << fLongName << " " <<
    fSemiTonesPitchAndOctaveVariable->asString ();

  return s.str ();
}

void lpsrTransposeAtom::print (ostream& os) const
{
  const int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "lpsrTransposeAtom:" <<
    endl;

  gIndenter++;

  oahElement::printOahElementEssentials (
    os, fieldWidth);

  gIndenter--;
}

void lpsrTransposeAtom::printAtomWithValueOptionsValues (
  ostream& os,
  int      valueFieldWidth) const
{
  os << left <<
    setw (valueFieldWidth) <<
    fVariableName <<
    " : ";
  if (fSemiTonesPitchAndOctaveVariable) {
    os << endl;
    gIndenter++;
    os <<
      fSemiTonesPitchAndOctaveVariable <<
      ", variableHasBeenSet: " <<
      booleanAsString (fVariableHasBeenSet);
    gIndenter--;
  }
  else {
    os <<
      "none" <<
      endl;
  }
}

ostream& operator<< (ostream& os, const S_lpsrTransposeAtom& elt)
{
  elt->print (os);
  return os;
}

//_______________________________________________________________________________
S_lpsrOahGroup gGlobalLpsrOahGroup;

S_lpsrOahGroup lpsrOahGroup::create ()
{
  lpsrOahGroup* o = new lpsrOahGroup ();
  assert (o!=0);
  return o;
}

lpsrOahGroup::lpsrOahGroup ()
  : oahGroup (
    "LPSR",
    "hlpsr", "help-lpsr",
R"(These options control the way LPSR data is handled.)",
    kElementVisibilityWhole)
{
  initializeLpsrOahGroup ();
}

lpsrOahGroup::~lpsrOahGroup ()
{}

#ifdef TRACING_IS_ENABLED
void lpsrOahGroup::initializeLpsrTraceOah ()
{
  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "LPSR Trace",
        "hlpsrtrace", "help-lpsr-trace",
R"()",
      kElementVisibilityWhole,
      this);

  appendSubGroupToGroup (subGroup);

  // trace- lpsr

  fTraceLpsr            = false;

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "tlpsr", "trace-lpsr",
R"(Write a trace of the LPSR graphs visiting activity to standard error.)",
        "traceLpsr",
        fTraceLpsr));

  // trace lilypond version

  fTraceLilypondVersion = false;

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "tlpv", "trace-lilypond-version",
R"(Write a trace of the LilyPond version to standard error.)",
        "traceLilypondVersion",
        fTraceLilypondVersion));

  // trace lpsr visitors

  fTraceLpsrVisitors    = false;

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "tlpsrv", "trace-lpsr-visitors",
R"(Write a trace of the LPSR graphs visiting activity to standard error.)",
        "traceLpsrVisitors",
        fTraceLpsrVisitors));

  // trace lpsr blocks

  fTraceLpsrBlocks      = false;

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "tlpsrb", "trace-lpsr-blocks",
R"(Write a trace of the LPSR blocks to standard error.)",
        "traceLpsrBlocks",
        fTraceLpsrBlocks));

  // trace scheme functions

  fTraceSchemeFunctions = false;

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "tsf", "trace-scheme-functions",
R"(Write a trace of the activity regarding Scheme functions to standard error.)",
        "traceSchemeFunctions",
        fTraceSchemeFunctions));
}
#endif

void lpsrOahGroup::initializeLpsrDisplayOptions ()
{
  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Display",
        "hlpsrd", "help-lpsr-display",
R"()",
      kElementVisibilityWhole,
      this);

  appendSubGroupToGroup (subGroup);

  // display LPSR

  fDisplayLpsr = false;

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "dlpsr", "display-lpsr",
R"(Write the contents of the LPSR data to standard error.)",
        "displayLpsr",
        fDisplayLpsr));

  // display LPSR short

  fDisplayLpsrShort = false;

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "dlpsrs", "display-lpsr-short",
R"(Write the contents of the LPSR data, short version, to standard error.)",
        "displayLpsrShort",
        fDisplayLpsrShort));
}

void lpsrOahGroup::initializeLpsrPaperOptions ()
{
  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Paper",
        "hlpsrpaper", "help-lpsr-paper",
R"()",
      kElementVisibilityWhole,
      this);

  appendSubGroupToGroup (subGroup);

  // all paper variables

  fAllPaperVariables = false;

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
         "apv", "all-paper-variables",
R"(Generate LilyPond comments containing paper variables
that are not present in the MusicXML data nor specified in options.
Default values are supplied.
This helps the user adapting the generated code for their needs.)",
        "allPaperVariables",
        fAllPaperVariables));

  // length unit kind

  fLengthUnitKindDefaultValue = kMillimeterUnit; // default value;
  fLengthUnitKind = fLengthUnitKindDefaultValue;

  subGroup->
    appendAtomToSubGroup (
      oahLengthUnitKindAtom::create (
        "length-unit", "",
        regex_replace (
          regex_replace (
            regex_replace (
R"(Set the LilyPond length variables unit to UNIT in the forthcoming options.
The NUMBER LilyPond length variables units available are:
LENTGTH_UNITS.
This option should precede options that set paper size and indents
if they don't specify a unit.
The default is 'DEFAULT_VALUE'.)",
              regex ("NUMBER"),
              to_string (gGlobalMsrLengthUnitKindsMap.size ())),
            regex ("LENTGTH_UNITS"),
            gIndenter.indentMultiLineString (
              existingMsrLengthUnitKinds (K_NAMES_LIST_MAX_LENGTH))),
          regex ("DEFAULT_VALUE"),
          msrLengthUnitKindAsString (
            fLengthUnitKindDefaultValue)),
        "UNIT",
        "lengthUnitKind",
        fLengthUnitKind));

  // paper height

  fPaperHeight.setLengthUnitKind (kMillimeterUnit);
  fPaperHeight.setLengthValue (297);

  fPaperHeightAtom =
    oahLengthAtom::create (
      "paper-height", "",
R"(Set the LilyPond 'paper-height' paper variable to HEIGHT in the LilyPond code.
HEIGHT should be a positive floating point or integer number,
immediately followed by a unit name, i.e. 'in', 'mm' or 'cm'.
By default, LilyPond uses 297 mm (A4 format).)",
      "HEIGHT",
      "paperHeight",
      fPaperHeight);
  subGroup->
    appendAtomToSubGroup (
      fPaperHeightAtom);

  // paper width

  fPaperWidth.setLengthUnitKind (kMillimeterUnit);
  fPaperWidth.setLengthValue (210);

  fPaperWidthAtom =
    oahLengthAtom::create (
      "paper-width", "",
R"(Set the LilyPond 'paper-width' paper variable to WIDTH in the LilyPond code.
WIDTH should be a positive floating point or integer number,
immediately followed by a unit name, i.e. 'in', 'mm' or 'cm'.
By default, LilyPond uses 210 mm (A4 format).)",
      "WIDTH",
      "paperWidth",
      fPaperWidth);
  subGroup->
    appendAtomToSubGroup (
      fPaperWidthAtom);

  // left margin

  fPaperLeftMargin.setLengthUnitKind (kMillimeterUnit);
  fPaperLeftMargin.setLengthValue (15);

  fPaperLeftMarginAtom =
    oahLengthAtom::create (
      "left-margin", "",
R"(Set the LilyPond 'left-margin' paper variable to MARGIN in the LilyPond code.
WIDTH should be a positive floating point or integer number,
immediately followed by a unit name, i.e. 'in', 'mm' or 'cm'.
By default, this is left to LilyPond'.)",
      "MARGIN",
      "paperLeftMargin",
      fPaperLeftMargin);
  subGroup->
    appendAtomToSubGroup (
      fPaperLeftMarginAtom);

  // right margin

  fPaperRightMargin.setLengthUnitKind (kMillimeterUnit);
  fPaperRightMargin.setLengthValue (15);

  fPaperRightMarginAtom =
    oahLengthAtom::create (
      "right-margin", "",
R"(Set the LilyPond 'right-margin' paper variable to MARGIN in the LilyPond code.
WIDTH should be a positive floating point or integer number,
immediately followed by a unit name, i.e. 'in', 'mm' or 'cm'.
By default, this is left to LilyPond'.)",
      "MARGIN",
      "paperRightMargin",
      fPaperRightMargin);
  subGroup->
    appendAtomToSubGroup (
      fPaperRightMarginAtom);

  // top margin

  fPaperTopMargin.setLengthUnitKind (kMillimeterUnit);
  fPaperTopMargin.setLengthValue (15);

  fPaperTopMarginAtom =
    oahLengthAtom::create (
      "top-margin", "",
R"(Set the LilyPond 'top-margin' paper variable to MARGIN in the LilyPond code.
WIDTH should be a positive floating point or integer number,
immediately followed by a unit name, i.e. 'in', 'mm' or 'cm'.
By default, this is left to LilyPond'.)",
      "MARGIN",
      "paperTopMargin",
      fPaperTopMargin);
  subGroup->
    appendAtomToSubGroup (
      fPaperTopMarginAtom);

  // bottom margin

  fPaperBottomMargin.setLengthUnitKind (kMillimeterUnit);
  fPaperBottomMargin.setLengthValue (15);

  fPaperBottomMarginAtom =
    oahLengthAtom::create (
      "bottom-margin", "",
R"(Set the LilyPond 'bottom-margin' paper variable to MARGIN in the LilyPond code.
WIDTH should be a positive floating point or integer number,
immediately followed by a unit name, i.e. 'in', 'mm' or 'cm'.
By default, this is left to LilyPond'.)",
      "MARGIN",
      "paperBottomMargin",
      fPaperBottomMargin);
  subGroup->
    appendAtomToSubGroup (
      fPaperBottomMarginAtom);

/* JMI superflous
  // a4

   subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "a4", "",
R"(Set the LilyPond 'paper-width' and 'paper-height' paper variables
to 210 mm  and 297 mm respectively in the LilyPond code.
WIDTH should be a positive floating point or integer number,
immediately followed by a unit name, i.e. 'in', 'mm' or 'cm'.
By default is 297 mm (A4 format).)",
        "WIDTH",
        "paperWidth",
        fPaperWidth));
*/

  // fPaperHorizontalShift is 0.0 mm by default

  subGroup->
    appendAtomToSubGroup (
      oahLengthAtom::create (
        "hshift", "horizontal-shift",
R"(Set the LilyPond 'horizontal-shift' paper variable to SHIFT in the LilyPond code.
SHIFT should be a floating point or integer number,
immediately followed by a unit name, i.e. 'in', 'mm' or 'cm'.
The default value is 0.0 mm.)",
        "SHIFT",
        "paperHorizontalShift",
        fPaperHorizontalShift));

  // fPaperIndent is 0.0 mm by default

  subGroup->
    appendAtomToSubGroup (
      oahLengthAtom::create (
        "indent", "",
R"(Set the LilyPond 'indent' paper variable to INDENT in the LilyPond code.
INDENT should be a floating point or integer number,
immediately followed by a unit name, i.e. 'in', 'mm' or 'cm'.
The default value is 0.0 mm.)",
        "INDENT",
        "paperIndent",
        fPaperIndent));

  // short indent

  // fPaperShortIndent is 0.0 mm by default

  subGroup->
    appendAtomToSubGroup (
      oahLengthAtom::create (
        "short-indent", "",
R"(Set the LilyPond 'short-indent' paper variable to SHORT_INDENT in the LilyPond code.
SHORT_INDENT should be a floating point or integer number,
immediately followed by a unit name, i.e. 'in', 'mm' or 'cm'.
The default value is 0.0 mm.)",
        "SHORT_INDENT",
        "paperShortIndent",
        fPaperShortIndent));

  // fMarkupSystemSpacingPadding is 0.0 mm by default

  subGroup->
    appendAtomToSubGroup (
      oahLengthAtom::create (
        "mssp", "markup-system-spacing-padding",
R"(Set the LilyPond 'markup-system-spacing.padding' paper variable
to PADDING in the LilyPond code.
PADDING should be a floating point or integer number,
immediately followed by a unit name, i.e. 'in', 'mm' or 'cm'.
LilyPond's default value is 0.0 mm.)",
        "INDENT",
        "markupSystemSpacingPadding",
        fMarkupSystemSpacingPadding));

  // ragged bottom

  fRaggedBottom = false;

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "ragged-bottom", "",
R"(Set the LilyPond 'ragged-bottom' paper variable to '##f' in the LilyPond code.
LilyPond's default value is '##t'.)",
        "raggedBottom",
        fRaggedBottom));

  // ragged last

  fRaggedLast = false;

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "ragged-last", "",
R"(Set the LilyPond 'ragged-last' paper variable to '##f' in the LilyPond code.
LilyPond's default value is '##t'.)",
        "raggedLast",
        fRaggedLast));

  // ragged last bottom

  fRaggedLastBottom = false;

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "ragged-last-bottom", "",
R"(Set the LilyPond 'ragged-last-bottom' paper variable to '##f' in the LilyPond code.
LilyPond's default value is '##t'.)",
        "raggedLastBottom",
        fRaggedLastBottom));

  // ragged right

  fRaggedRight = false;

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "ragged-right", "",
R"(Set the LilyPond 'ragged-right' paper variable to '##f' in the LilyPond code.
LilyPond's default value is '##t'.)",
        "raggedRight",
        fRaggedRight));

/* LPSR or LilyPond option?"
  // tagline

  fTagline = false;

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "tagline", "",
R"(Set the LilyPond 'tagline' paper variable to '##f' in the LilyPond code.
LilyPond's default value is '##t'.)",
        "tagline",
        fTagline));
*/

  // page count

  fPageCount = 0;

  subGroup->
    appendAtomToSubGroup (
      oahIntegerAtom::create (
        "page-count", "",
R"(Set the LilyPond 'page-count' paper variable to PAGE_COUNT in the LilyPond code.
PAGE_COUNT should be a positive integer.
By default, this is left to LilyPond'.)",
        "PAGE_COUNT",
        "pageCount",
        fPageCount));

  // system count

  fSystemCount = 0;

  subGroup->
    appendAtomToSubGroup (
      oahIntegerAtom::create (
        "system-count", "",
R"(Set the LilyPond 'system-count' paper variable to SYSTEM_COUNT in the LilyPond code.
SYSTEM_COUNT should be a positive integer.
By default, this is left to LilyPond'.)",
        "SYSTEM_COUNT",
        "systemCount",
        fSystemCount));
}

void lpsrOahGroup::initializeLpsrMeasuresOptions ()
{
  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Measures",
        "hlpsrmeasures", "help-lpsr-measures",
R"()",
      kElementVisibilityWhole,
      this);

  appendSubGroupToGroup (subGroup);

  // replicate empty measure

  fReplicateEmptyMeasureNumber = "";
  fReplicateEmptyMeasureReplicas = 0;

  subGroup->
    appendAtomToSubGroup (
      oahStringAndIntegerAtom::create (
        "rem", "replicate-empty-measure",
R"###(Replicate an empty mesure, adding empty others according to SPECIFICATION.
SPECIFICATION should be of the form 'MEASURE_NUMBER REPLICATES',
where MEASURE_NUMBER is a string, and REPLICATES is the number
of empty measures to add after measure MEASURE_NUMBER.
MEASURE_NUMBER should be the number of an existing, empty measure,
and REPLICATES should be at least 1, , such as '17 3'.
This comes in handly when MusicXML data obtained by scanning contains
a single empty measure when there were several in the original score.
This option can be used any number of times.)###",
        "SPECIFICATION",
        "replicateEmptyMeasureNumber",
        fReplicateEmptyMeasureNumber,
        "replicateEmptyMeasureReplicas",
        fReplicateEmptyMeasureReplicas));
}

void lpsrOahGroup::initializeLpsrTemposOptions ()
{
  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Tempos",
        "hlpsrtempos", "help-lpsr-tempos",
R"()",
      kElementVisibilityWhole,
      this);

  appendSubGroupToGroup (subGroup);

  // convert tempos to rehearsal marks

  fConvertTemposToRehearsalMarks = false;

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "cttrm", "convert-tempos-to-rehearsal-marks",
R"(Convert tempos to rehearsal marks.
This may come in handy when MusicXML data has been obtained
from scanned instrumental music images.)",
        "convertTemposToRehearsalMarks",
        fConvertTemposToRehearsalMarks));
}

void lpsrOahGroup::initializeLpsrWordsOptions ()
{
  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Words",
        "hlpsrwords", "help-lpsr-words",
R"()",
      kElementVisibilityWhole,
      this);

  appendSubGroupToGroup (subGroup);

  // convert words to tempo

  fConvertWordsToTempo = false;

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "cwtt", "convert-words-to-tempo",
R"(Convert words to tempo.
This may come in handy when MusicXML data has been obtained
from scanned instrumental music images.)",
        "convertWordsToTempo",
        fConvertWordsToTempo));

  // add words from the lyrics

  fAddWordsFromTheLyrics = false;

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "awftl", "add-words-from-the-lyrics",
R"(Add words with the lyrics contents, keeping the latter untouched.
This may come in handy when MusicXML data has been obtained from scanned images.)",
        "addWordsFromTheLyrics",
        fAddWordsFromTheLyrics));

  // convert words to rehearsal marks

  fConvertWordsToRehearsalMarks = false;

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "cwtrm", "convert-words-to-rehearsal-marks",
R"(Convert words to rehearsal marks.
This may come in handy when MusicXML data has been obtained
from scanned instrumental music images.)",
        "convertWordsToRehearsalMarks",
        fConvertWordsToRehearsalMarks));

/* JMI
  // convert words to dal segno
  subGroup->
    appendAtomToSubGroup (
      lpsrDalSegnoAtom::create (
        "ds", "dal-segno",
R"(Convert words elements STRING to an MSR 'dal segno' element'.)",
        "STRING",
        "convertWordsToRehearsalMarks",
        fConvertWordsToDalSegno));

  // convert words to dal segno al fine
  subGroup->
    appendAtomToSubGroup (
      lpsrDalSegnoAlFineAtom::create (
        "dsaf", "dal-segno-al-fine",
R"(Convert words elements STRING to an MSR 'dal segno al fine' element.)",
        "STRING",
        "convertWordsToRehearsalMarks",
        fConvertWordsToDalSegno));

  // convert words to dal segno al coda
  subGroup->
    appendAtomToSubGroup (
      lpsrDalSegnoAlCodaAtom::create (
        "dsac", "dal-segno-al-coda",
R"(Convert words elements STRING to an MSR 'dal segno al coda' element.)",
        "STRING",
        "convertWordsToRehearsalMarks",
        fConvertWordsToDalSegno));
*/

/* JMI

        regex_replace (
          regex_replace (
            regex_replace (
R"(Use LANGUAGE to display note pitches in the LPSR logs and views,
as well as in the generated LilyPond code.
The NUMBER LilyPond pitches languages available are:
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
        "lpsrPitchesLanguage",
        fLpsrQuarterTonesPitchesLanguageKind));

    map<string, msrDalSegno::msrDalSegnoKind>
                          fConvertWordsToDalSegno;
                          */
}

void lpsrOahGroup::initializeLpsrLanguagesOptions ()
{
  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Languages",
        "hlpsrl", "help-lpsr-languages",
R"()",
      kElementVisibilityWhole,
      this);

  appendSubGroupToGroup (subGroup);

  // lpsr pitches language

  if (! setLpsrQuarterTonesPitchesLanguage ("nederlands")) {
    stringstream s;

    s <<
      "INTERNAL INITIALIZATION ERROR: "
      "LPSR pitches language 'nederlands' is unknown" <<
      endl <<
      "The " <<
      gGlobalQuarterTonesPitchesLanguageKindsMap.size () <<
      " known LPSR pitches languages are:" <<
      endl;

    gIndenter++;

    s <<
      existingQuarterTonesPitchesLanguageKinds (K_NAMES_LIST_MAX_LENGTH);

    gIndenter--;

    oahError (s.str ());
  }

  const msrQuarterTonesPitchesLanguageKind
    msrQuarterTonesPitchesLanguageKindDefaultValue =
      fLpsrQuarterTonesPitchesLanguageKind;

  fLpsrQuarterTonesPitchesLanguageKind =
    msrQuarterTonesPitchesLanguageKindDefaultValue;

  subGroup->
    appendAtomToSubGroup (
      lpsrPitchesLanguageAtom::create (
        "lppl", "lpsr-pitches-language",
        regex_replace (
          regex_replace (
            regex_replace (
R"(Use LANGUAGE to display note pitches in the LPSR logs and views,
as well as in the generated LilyPond code.
The NUMBER LPSR pitches languages available are:
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
        "lpsrPitchesLanguage",
        fLpsrQuarterTonesPitchesLanguageKind));

  // lpsr chords language

  const lpsrChordsLanguageKind
    lpsrChordsLanguageKindDefaultValue =
      k_IgnatzekChords; // LilyPond default

  fLpsrChordsLanguageKind =
    lpsrChordsLanguageKindDefaultValue;

  subGroup->
    appendAtomToSubGroup (
      lpsrChordsLanguageAtom::create (
        "lpcl", "lpsr-chords-language",
        regex_replace (
          regex_replace (
            regex_replace (
R"(Use LANGUAGE to display chord names, their root and bass notes,
in the LPSR logs and views and the generated LilyPond code.
The NUMBER LPSR chords pitches languages available are:
CHORDS_LANGUAGES.
'ignatzek' is Ignatzek's jazz-like, english naming used by LilyPond by default.
The default is 'DEFAULT_VALUE'.)",
              regex ("NUMBER"),
              to_string (gGlobalLpsrChordsLanguageKindsMap.size ())),
            regex ("CHORDS_LANGUAGES"),
            gIndenter.indentMultiLineString (
              existingLpsrChordsLanguageKinds (K_NAMES_LIST_MAX_LENGTH))),
          regex ("DEFAULT_VALUE"),
          lpsrChordsLanguageKindAsString (
            lpsrChordsLanguageKindDefaultValue)),
        "LANGUAGE",
        "lpsr-chords-language",
        fLpsrChordsLanguageKind));
}

void lpsrOahGroup::initializeLpsrTransposeOptions ()
{
  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Transpose",
        "hlpsrt", "help-lpsr-transpose",
R"()",
      kElementVisibilityWhole,
      this);

  appendSubGroupToGroup (subGroup);

  // lpsr transpose

  subGroup->
    appendAtomToSubGroup (
      lpsrTransposeAtom::create (
        "lpt", "lpsr-transpose",
R"(Use TRANSPOSITION to tranpose in the LPSR data,
thus in the generated LilyPond code as well.
TRANSPOSITION should contain a diatonic pitch, followed if needed
by a sequence of ',' or '\'' octave indications.
Such indications cannot be mixed.
For example, 'a', 'f' and 'bes,' can be used respectively
for instruments in 'a', 'f' and B flat respectively)",
        "TRANSPOSITION",
        "lpsrTranspose",
        fTransposeSemiTonesPitchAndOctave));
}

void lpsrOahGroup::initializeLpsrQuitAfterSomePassesOptions ()
{
  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Quit after some passes",
        "hlquit", "help-lpsr-quit",
R"()",
      kElementVisibilityWhole,
      this);

  appendSubGroupToGroup (subGroup);

  // quit after pass 3

  fQuitAfterPass3 = false;

  S_oahBooleanAtom
    quit3OahBooleanAtom =
      oahBooleanAtom::create (
        "q3", "quitAfterPass-3",
R"(Quit after pass 3, i.e. after conversion
of the MSR to LPSR.)",
        "quitAfterPass3",
        fQuitAfterPass3);

  subGroup->
    appendAtomToSubGroup (
      quit3OahBooleanAtom);
}

void lpsrOahGroup::initializeLpsrOahGroup ()
{
#ifdef TRACING_IS_ENABLED
  // trace and display
  // --------------------------------------
  initializeLpsrTraceOah ();
#endif

  // display
  // --------------------------------------
  initializeLpsrDisplayOptions ();

  // paper
  // --------------------------------------
  initializeLpsrPaperOptions ();

  // measures
  // --------------------------------------
  initializeLpsrMeasuresOptions ();

  // tempos
  // --------------------------------------
  initializeLpsrTemposOptions ();

  // words
  // --------------------------------------
  initializeLpsrWordsOptions ();

  // languages
  // --------------------------------------
  initializeLpsrLanguagesOptions ();

/* superfluous JMI ???
  // transpose
  // --------------------------------------
  initializeLpsrTransposeOptions ();
*/

  // quit after some passes
  // --------------------------------------
  initializeLpsrQuitAfterSomePassesOptions ();
}

//______________________________________________________________________________
bool lpsrOahGroup::setLpsrQuarterTonesPitchesLanguage (string language)
{
  // is language in the note names languages map?
  map<string, msrQuarterTonesPitchesLanguageKind>::const_iterator
    it =
      gGlobalQuarterTonesPitchesLanguageKindsMap.find (language);

  if (it == gGlobalQuarterTonesPitchesLanguageKindsMap.end ()) {
    // no, language is unknown in the map
    return false;
  }

  fLpsrQuarterTonesPitchesLanguageKind = (*it).second;

  return true;
}

//______________________________________________________________________________
bool lpsrOahGroup::setLpsrChordsLanguage (string language)
{
  // is language in the chords languages map?
  map<string, lpsrChordsLanguageKind>::const_iterator
    it =
      gGlobalLpsrChordsLanguageKindsMap.find (language);

  if (it == gGlobalLpsrChordsLanguageKindsMap.end ()) {
    // no, language is unknown in the map
    return false;
  }

  fLpsrChordsLanguageKind = (*it).second;

  return true;
}

//______________________________________________________________________________
void lpsrOahGroup::enforceGroupQuietness ()
{
#ifdef TRACING_IS_ENABLED
  fTraceLpsr = false;
  fTraceLilypondVersion = false;
  fTraceLpsrVisitors = false;
  fTraceLpsrBlocks = false;
  fTraceSchemeFunctions = false;
#endif

  fDisplayLpsr = false;
  fDisplayLpsrShort = false;
}

//______________________________________________________________________________
void lpsrOahGroup::checkGroupOptionsConsistency ()
{
  // JMI
}

//______________________________________________________________________________
void lpsrOahGroup::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> lpsrOahGroup::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_lpsrOahGroup>*
    p =
      dynamic_cast<visitor<S_lpsrOahGroup>*> (v)) {
        S_lpsrOahGroup elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching lpsrOahGroup::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void lpsrOahGroup::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> lpsrOahGroup::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_lpsrOahGroup>*
    p =
      dynamic_cast<visitor<S_lpsrOahGroup>*> (v)) {
        S_lpsrOahGroup elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching lpsrOahGroup::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void lpsrOahGroup::browseData (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> lpsrOahGroup::browseData ()" <<
      endl;
  }
#endif
}

//______________________________________________________________________________
void lpsrOahGroup::printLpsrOahValues (int fieldWidth)
{
  gLogStream <<
    "The LPSR options are:" <<
    endl;

  gIndenter++;

#ifdef TRACING_IS_ENABLED
  // trace
  // --------------------------------------
  gLogStream <<
    "Trace:" <<
    endl;

  gIndenter++;

  gLogStream << left <<
    setw (fieldWidth) << "traceLpsr" << " : " <<
    booleanAsString (fTraceLpsr) <<
    endl <<

    setw (fieldWidth) << "traceLilypondVersion" << " : " <<
    booleanAsString (fTraceLilypondVersion) <<
    endl <<

    setw (fieldWidth) << "traceLpsrVisitors" << " : " <<
    booleanAsString (fTraceLpsrVisitors) <<
    endl <<

    setw (fieldWidth) << "traceLpsrBlocks" << " : " <<
    booleanAsString (fTraceLpsrBlocks) <<
    endl <<

    setw (fieldWidth) << "traceSchemeFunctions" << " : " <<
    booleanAsString (fTraceSchemeFunctions) <<
    endl;

  gIndenter--;
#endif

  // display
  // --------------------------------------
  gLogStream <<
    "Display:" <<
    endl;

  gIndenter++;

  gLogStream << left <<
    setw (fieldWidth) << "displayLpsr" << " : " <<
    booleanAsString (fDisplayLpsr) <<
    endl <<
    setw (fieldWidth) << "displayLpsrShort" << " : " <<
    booleanAsString (fDisplayLpsrShort) <<
    endl;

  gIndenter--;

  // paper
  // --------------------------------------

  gLogStream <<
    "Paper:" <<
    endl;

  gIndenter++;

  gLogStream << left <<
    setw (fieldWidth) << "allPaperVariables" << " : " <<
    booleanAsString (fAllPaperVariables) <<
    endl <<

    setw (fieldWidth) << "lengthUnitKind" << " : " <<
    msrLengthUnitKindAsString (fLengthUnitKind) <<
    endl <<

    setw (fieldWidth) << "paperHeight" << " : " <<
    fPaperHeight.asString () <<
    endl <<
    setw (fieldWidth) << "paperWidth" << " : " <<
    fPaperWidth.asString () <<
    endl <<

    setw (fieldWidth) << "paperLeftMargin" << " : " <<
    fPaperLeftMargin.asString () <<
    endl <<
    setw (fieldWidth) << "paperRightMargin" << " : " <<
    fPaperRightMargin.asString () <<
    endl <<
    setw (fieldWidth) << "paperTopMargin" << " : " <<
    fPaperTopMargin.asString () <<
    endl <<
    setw (fieldWidth) << "paperBottomMargin" << " : " <<
    fPaperBottomMargin.asString () <<
    endl <<

    setw (fieldWidth) << "paperIndent" << " : " <<
    fPaperIndent.asString () <<
    endl <<
    setw (fieldWidth) << "paperShortIndent" << " : " <<
    fPaperShortIndent.asString () <<
    endl <<

    setw (fieldWidth) << "markupSystemPpacingPadding" << " : " <<
    fMarkupSystemSpacingPadding.asString () <<
    endl <<

    setw (fieldWidth) << "raggedBottom" << " : " <<
    booleanAsString (fRaggedBottom) <<
    endl <<
    setw (fieldWidth) << "raggedLastBottom" << " : " <<
    booleanAsString (fRaggedLastBottom) <<
    endl <<
    setw (fieldWidth) << "raggedLast" << " : " <<
    booleanAsString (fRaggedLast) <<
    endl <<
    setw (fieldWidth) << "raggedRight" << " : " <<
    booleanAsString (fRaggedRight) <<
    endl <<

    setw (fieldWidth) << "tagline" << " : " <<
    booleanAsString (fTagline) <<
    endl <<

    setw (fieldWidth) << "pageCount" << " : " <<
    fPageCount <<
    endl <<
    setw (fieldWidth) << "systemCount" << " : " <<
    fSystemCount <<
    endl;

  gIndenter--;

/* JMI
  // measures
  // --------------------------------------

  gLogStream <<
    "Measures:" <<
    endl;

  gIndenter++;

  gLogStream << left <<
    setw (fieldWidth) << "resetMeasureElementMeasureNumberMap" << " : ";
  if (! fAddEmptyMeasuresStringToIntMap.size ()) {
    gLogStream << "empty";
  }
  else {
    map<string, int>::const_iterator
      iBegin = fAddEmptyMeasuresStringToIntMap.begin (),
      iEnd   = fAddEmptyMeasuresStringToIntMap.end (),
      i      = iBegin;
    for ( ; ; ) {
      gLogStream << (*i).first << "=" << (*i).second;
      if (++i == iEnd) break;
      gLogStream << ",";
    } // for
  }
  gLogStream << endl;

  gIndenter--;
*/

  // tempos
  // --------------------------------------

  gLogStream <<
    "Tempos:" <<
    endl;

  gIndenter++;

  gLogStream << left <<
    setw (fieldWidth) << "convertTemposToRehearsalMarks" << " : " <<
    booleanAsString (fConvertTemposToRehearsalMarks) <<
    endl << endl;

  gIndenter--;

  // words
  // --------------------------------------

  gLogStream <<
    "Words:" <<
    endl;

  gIndenter++;

  gLogStream << left <<
    setw (fieldWidth) << "convertWordsToTempo" << " : " <<
    booleanAsString (fConvertWordsToTempo) <<
    endl <<
    setw (fieldWidth) << "addWordsFromTheLyrics" << " : " <<
    booleanAsString (fAddWordsFromTheLyrics) <<
    endl <<

    setw (fieldWidth) << "convertWordsToRehearsalMarks" << " : " <<
    booleanAsString (fConvertWordsToRehearsalMarks) <<
    endl;

  gIndenter--;

  // languages
  // --------------------------------------

  gLogStream <<
    "Languages:" <<
    endl;

  gIndenter++;

  gLogStream << left <<
    setw (fieldWidth) << "lpsrQuarterTonesPitchesLanguage" << " : \"" <<
    msrQuarterTonesPitchesLanguageKindAsString (
      fLpsrQuarterTonesPitchesLanguageKind) <<
    "\"" <<
    endl <<

    setw (fieldWidth) << "lpsrChordsLanguage" << " : \"" <<
    lpsrChordsLanguageKindAsString (
      fLpsrChordsLanguageKind) <<
    "\"" <<
    endl;

  gIndenter--;

  // transpose
  // --------------------------------------

  gLogStream <<
    "Transpose:" <<
    endl;

  gIndenter++;

  gLogStream << left <<
    setw (fieldWidth) << "transposeSemiTonesPitchAndOctave" << " : ";

    if (fTransposeSemiTonesPitchAndOctave) {
      gLogStream <<
        fTransposeSemiTonesPitchAndOctave->asString ();
    }
    else {
      gLogStream <<
        "none";
    }
  gLogStream << endl;

  gIndenter--;

  gIndenter--;
}

ostream& operator<< (ostream& os, const S_lpsrOahGroup& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_lpsrOahGroup createGlobalLpsrOahGroup ()
{
#ifdef TRACING_IS_ENABLED
#ifdef ENFORCE_TRACE_OAH
  gLogStream <<
    "Creating global LPSR OAH group" <<
    endl;
#endif
#endif

  // protect library against multiple initializations
  if (! gGlobalLpsrOahGroup) {
    // create the global options group
    gGlobalLpsrOahGroup =
      lpsrOahGroup::create ();
    assert (gGlobalLpsrOahGroup != 0);
  }

  // return the global OAH group
  return gGlobalLpsrOahGroup;
}


}


/* JMI
//______________________________________________________________________________
S_lpsrDalSegnoAtom lpsrDalSegnoAtom::create (
  string              shortName,
  string              longName,
  string              description,
  string              valueSpecification,
  string              variableName,
  map<string, msrDalSegno::msrDalSegnoKind>&
                      stringToDalSegnoKindMapVariable)
{
  lpsrDalSegnoAtom* o = new
    lpsrDalSegnoAtom (
      shortName,
      longName,
      description,
      valueSpecification,
      variableName,
      stringToDalSegnoKindMapVariable);
  assert (o!=0);
  return o;
}

lpsrDalSegnoAtom::lpsrDalSegnoAtom (
  string              shortName,
  string              longName,
  string              description,
  string              valueSpecification,
  string              variableName,
  map<string, msrDalSegno::msrDalSegnoKind>&
                      stringToDalSegnoKindMapVariable)
  : oahAtomWithValue (
      shortName,
      longName,
      description,
      valueSpecification,
      variableName),
    fStringToDalSegnoKindMapVariable (
      stringToDalSegnoKindMapVariable)
{
  fMultipleOccurrencesAllowed = true;
}

lpsrDalSegnoAtom::~lpsrDalSegnoAtom ()
{}

void lpsrDalSegnoAtom::applyAtomWithValue (
  string   theString,
  ostream& os)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceOah ()) {
    gLogStream <<
      "==> oahAtom is of type 'lpsrDalSegnoAtom'" <<
      endl;
  }
#endif

  // theString contains the dal segno specification

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceOah ()) {
    gLogStream <<
      "==> oahAtom is of type 'lpsrDalSegnoAtom'" <<
      endl;
  }
#endif

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceOah ()) {
    gLogStream <<
      "--> theString = \"" << theString << "\", " <<
      endl;
  }
#endif

  // is this part name in the part renaming map?
  map<string, msrDalSegno::msrDalSegnoKind>::iterator
    it =
      fStringToDalSegnoKindMapVariable.find (theString);

  if (it != fStringToDalSegnoKindMapVariable.end ()) {
    // yes, issue error message
    stringstream s;

    s <<
      "Dal segno \"" << theString << "\" occurs more that once" <<
      "in a '--dal-segno' option";

    oahError (s.str ());
  }

  else {
    fStringToDalSegnoKindMapVariable [theString] = msrDalSegno::kDalSegno;
  }
}

void lpsrDalSegnoAtom::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> lpsrDalSegnoAtom::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_lpsrDalSegnoAtom>*
    p =
      dynamic_cast<visitor<S_lpsrDalSegnoAtom>*> (v)) {
        S_lpsrDalSegnoAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching lpsrDalSegnoAtom::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void lpsrDalSegnoAtom::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> lpsrDalSegnoAtom::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_lpsrDalSegnoAtom>*
    p =
      dynamic_cast<visitor<S_lpsrDalSegnoAtom>*> (v)) {
        S_lpsrDalSegnoAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching lpsrDalSegnoAtom::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void lpsrDalSegnoAtom::browseData (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> lpsrDalSegnoAtom::browseData ()" <<
      endl;
  }
#endif
}

string lpsrDalSegnoAtom::asShortNamedOptionString () const
{
  stringstream s;

  s <<
    "-" << fShortName << " ";

  if (! fStringToDalSegnoKindMapVariable.size ()) {
    s << "none";
  }
  else {
    map<string, msrDalSegno::msrDalSegnoKind>::const_iterator
      iBegin = fStringToDalSegnoKindMapVariable.begin (),
      iEnd   = fStringToDalSegnoKindMapVariable.end (),
      i      = iBegin;
    for ( ; ; ) {
      s << (*i).first << "=" << (*i).second;
      if (++i == iEnd) break;
      s << ",";
    } // for
  }

  return s.str ();
}

string lpsrDalSegnoAtom::asActualLongNamedOptionString () const
{
  stringstream s;

  s <<
    "-" << fLongName << " ";

  if (! fStringToDalSegnoKindMapVariable.size ()) {
    s << "none";
  }
  else {
    map<string, msrDalSegno::msrDalSegnoKind>::const_iterator
      iBegin = fStringToDalSegnoKindMapVariable.begin (),
      iEnd   = fStringToDalSegnoKindMapVariable.end (),
      i      = iBegin;
    for ( ; ; ) {
      s << (*i).first << "=" << (*i).second;
      if (++i == iEnd) break;
      s << ",";
    } // for
  }

  return s.str ();
}

void lpsrDalSegnoAtom::print (ostream& os) const
{
  const int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "lpsrDalSegnoAtom:" <<
    endl;

  gIndenter++;

  printAtomWithValueEssentials (
    os, fieldWidth);

  os << left <<
    setw (fieldWidth) <<
    "fVariableName" << " : " <<
    fVariableName <<
    setw (fieldWidth) <<
    "fStringToDalSegnoKindMapVariable" << " : " <<
    endl;

  if (! fStringToDalSegnoKindMapVariable.size ()) {
    os << "none";
  }
  else {
    map<string, msrDalSegno::msrDalSegnoKind>::const_iterator
      iBegin = fStringToDalSegnoKindMapVariable.begin (),
      iEnd   = fStringToDalSegnoKindMapVariable.end (),
      i      = iBegin;
    for ( ; ; ) {
      os << (*i).first << " --> " << (*i).second;
      if (++i == iEnd) break;
      os << endl;
    } // for
  }

  gIndenter--;

  os << endl;
}

void lpsrDalSegnoAtom::printAtomWithValueOptionsValues (
  ostream& os,
  int      valueFieldWidth) const
{
  os << left <<
    setw (valueFieldWidth) <<
    fVariableName <<
    " : ";

  if (! fStringToDalSegnoKindMapVariable.size ()) {
    os <<
      "none" <<
      endl;
  }
  else {
    os << endl;
    gIndenter++;

    map<string, msrDalSegno::msrDalSegnoKind>::const_iterator
      iBegin = fStringToDalSegnoKindMapVariable.begin (),
      iEnd   = fStringToDalSegnoKindMapVariable.end (),
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

    os <<
      ", variableHasBeenSet: " <<
      booleanAsString (fVariableHasBeenSet);

    gIndenter--;
  }
}

ostream& operator<< (ostream& os, const S_lpsrDalSegnoAtom& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_lpsrDalSegnoAlFineAtom lpsrDalSegnoAlFineAtom::create (
  string              shortName,
  string              longName,
  string              description,
  string              valueSpecification,
  string              variableName,
  map<string, msrDalSegno::msrDalSegnoKind>&
                      stringToDalSegnoKindMapVariable)
{
  lpsrDalSegnoAlFineAtom* o = new
    lpsrDalSegnoAlFineAtom (
      shortName,
      longName,
      description,
      valueSpecification,
      variableName,
      stringToDalSegnoKindMapVariable);
  assert (o!=0);
  return o;
}

lpsrDalSegnoAlFineAtom::lpsrDalSegnoAlFineAtom (
  string              shortName,
  string              longName,
  string              description,
  string              valueSpecification,
  string              variableName,
  map<string, msrDalSegno::msrDalSegnoKind>&
                      stringToDalSegnoKindMapVariable)
  : oahAtomWithValue (
      shortName,
      longName,
      description,
      valueSpecification,
      variableName),
    fStringToDalSegnoKindMapVariable (
      stringToDalSegnoKindMapVariable)
{
  fMultipleOccurrencesAllowed = true;
}

lpsrDalSegnoAlFineAtom::~lpsrDalSegnoAlFineAtom ()
{}

void lpsrDalSegnoAlFineAtom::applyAtomWithValue (
  string   theString,
  ostream& os)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceOah ()) {
    gLogStream <<
      "==> oahAtom is of type 'lpsrDalSegnoAlFineAtom'" <<
      endl;
  }
#endif

  // theString contains the dal segno al fine specification

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceOah ()) {
    gLogStream <<
      "==> oahAtom is of type 'lpsrDalSegnoAlFineAtom'" <<
      endl;
  }
#endif

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceOah ()) {
    gLogStream <<
      "--> theString = \"" << theString << "\", " <<
      endl;
  }
#endif

  // is this part name in the part renaming map?
  map<string, msrDalSegno::msrDalSegnoKind>::iterator
    it =
      fStringToDalSegnoKindMapVariable.find (theString);

  if (it != fStringToDalSegnoKindMapVariable.end ()) {
    // yes, issue error message
    stringstream s;

    s <<
      "Dal segno \"" << theString << "\" occurs more that once" <<
      "in a '--dal-segno-al-fine' option";

    oahError (s.str ());
  }

  else {
    fStringToDalSegnoKindMapVariable [theString] = msrDalSegno::kDalSegno;
  }
}

void lpsrDalSegnoAlFineAtom::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> lpsrDalSegnoAlFineAtom::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_lpsrDalSegnoAlFineAtom>*
    p =
      dynamic_cast<visitor<S_lpsrDalSegnoAlFineAtom>*> (v)) {
        S_lpsrDalSegnoAlFineAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching lpsrDalSegnoAlFineAtom::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void lpsrDalSegnoAlFineAtom::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> lpsrDalSegnoAlFineAtom::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_lpsrDalSegnoAlFineAtom>*
    p =
      dynamic_cast<visitor<S_lpsrDalSegnoAlFineAtom>*> (v)) {
        S_lpsrDalSegnoAlFineAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching lpsrDalSegnoAlFineAtom::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void lpsrDalSegnoAlFineAtom::browseData (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> lpsrDalSegnoAlFineAtom::browseData ()" <<
      endl;
  }
#endif
}

string lpsrDalSegnoAlFineAtom::asShortNamedOptionString () const
{
  stringstream s;

  s <<
    "-" << fShortName << " ";

  if (! fStringToDalSegnoKindMapVariable.size ()) {
    s << "none";
  }
  else {
    map<string, msrDalSegno::msrDalSegnoKind>::const_iterator
      iBegin = fStringToDalSegnoKindMapVariable.begin (),
      iEnd   = fStringToDalSegnoKindMapVariable.end (),
      i      = iBegin;
    for ( ; ; ) {
      s << (*i).first << "=" << (*i).second;
      if (++i == iEnd) break;
      s << ",";
    } // for
  }

  return s.str ();
}

string lpsrDalSegnoAlFineAtom::asActualLongNamedOptionString () const
{
  stringstream s;

  s <<
    "-" << fLongName << " ";

  if (! fStringToDalSegnoKindMapVariable.size ()) {
    s << "none";
  }
  else {
    map<string, msrDalSegno::msrDalSegnoKind>::const_iterator
      iBegin = fStringToDalSegnoKindMapVariable.begin (),
      iEnd   = fStringToDalSegnoKindMapVariable.end (),
      i      = iBegin;
    for ( ; ; ) {
      s << (*i).first << "=" << (*i).second;
      if (++i == iEnd) break;
      s << ",";
    } // for
  }

  return s.str ();
}

void lpsrDalSegnoAlFineAtom::print (ostream& os) const
{
  const int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "lpsrDalSegnoAlFineAtom:" <<
    endl;

  gIndenter++;

  printAtomWithValueEssentials (
    os, fieldWidth);

  os << left <<
    setw (fieldWidth) <<
    "fVariableName" << " : " <<
    fVariableName <<
    setw (fieldWidth) <<
    "fStringToDalSegnoKindMapVariable" << " : " <<
    endl;

  if (! fStringToDalSegnoKindMapVariable.size ()) {
    os << "none";
  }
  else {
    map<string, msrDalSegno::msrDalSegnoKind>::const_iterator
      iBegin = fStringToDalSegnoKindMapVariable.begin (),
      iEnd   = fStringToDalSegnoKindMapVariable.end (),
      i      = iBegin;
    for ( ; ; ) {
      os << (*i).first << " --> " << (*i).second;
      if (++i == iEnd) break;
      os << endl;
    } // for
  }

  gIndenter--;

  os << endl;
}

void lpsrDalSegnoAlFineAtom::printAtomWithValueOptionsValues (
  ostream& os,
  int      valueFieldWidth) const
{
  os << left <<
    setw (valueFieldWidth) <<
    fVariableName <<
    " : ";

  if (! fStringToDalSegnoKindMapVariable.size ()) {
    os <<
      "none" <<
      endl;
  }
  else {
    os << endl;
    gIndenter++;

    map<string, msrDalSegno::msrDalSegnoKind>::const_iterator
      iBegin = fStringToDalSegnoKindMapVariable.begin (),
      iEnd   = fStringToDalSegnoKindMapVariable.end (),
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

    os <<
      ", variableHasBeenSet: " <<
      booleanAsString (fVariableHasBeenSet);

    gIndenter--;
  }
}

ostream& operator<< (ostream& os, const S_lpsrDalSegnoAlFineAtom& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_lpsrDalSegnoAlCodaAtom lpsrDalSegnoAlCodaAtom::create (
  string              shortName,
  string              longName,
  string              description,
  string              valueSpecification,
  string              variableName,
  map<string, msrDalSegno::msrDalSegnoKind>&
                      stringToDalSegnoKindMapVariable)
{
  lpsrDalSegnoAlCodaAtom* o = new
    lpsrDalSegnoAlCodaAtom (
      shortName,
      longName,
      description,
      valueSpecification,
      variableName,
      stringToDalSegnoKindMapVariable);
  assert (o!=0);
  return o;
}

lpsrDalSegnoAlCodaAtom::lpsrDalSegnoAlCodaAtom (
  string              shortName,
  string              longName,
  string              description,
  string              valueSpecification,
  string              variableName,
  map<string, msrDalSegno::msrDalSegnoKind>&
                      stringToDalSegnoKindMapVariable)
  : oahAtomWithValue (
      shortName,
      longName,
      description,
      valueSpecification,
      variableName),
    fStringToDalSegnoKindMapVariable (
      stringToDalSegnoKindMapVariable)
{
  fMultipleOccurrencesAllowed = true;
}

lpsrDalSegnoAlCodaAtom::~lpsrDalSegnoAlCodaAtom ()
{}

void lpsrDalSegnoAlCodaAtom::applyAtomWithValue (
  string   theString,
  ostream& os)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceOah ()) {
    gLogStream <<
      "==> oahAtom is of type 'lpsrDalSegnoAlCodaAtom'" <<
      endl;
  }
#endif

  // theString contains the dal segno specification
  // decipher it to extract the old and new part names

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceOah ()) {
    gLogStream <<
      "==> oahAtom is of type 'lpsrDalSegnoAlCodaAtom'" <<
      endl;
  }
#endif

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceOah ()) {
    gLogStream <<
      "--> theString = \"" << theString << "\", " <<
      endl;
  }
#endif

  // is this part name in the part renaming map?
  map<string, msrDalSegno::msrDalSegnoKind>::iterator
    it =
      fStringToDalSegnoKindMapVariable.find (theString);

  if (it != fStringToDalSegnoKindMapVariable.end ()) {
    // yes, issue error message
    stringstream s;

    s <<
      "Dal segno al coda value \"" << theString << "\" occurs more that once" <<
      "in a '--dal-segno' option";

    oahError (s.str ());
  }

  else {
    fStringToDalSegnoKindMapVariable [theString] = msrDalSegno::kDalSegno;
  }
}

void lpsrDalSegnoAlCodaAtom::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> lpsrDalSegnoAlCodaAtom::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_lpsrDalSegnoAlCodaAtom>*
    p =
      dynamic_cast<visitor<S_lpsrDalSegnoAlCodaAtom>*> (v)) {
        S_lpsrDalSegnoAlCodaAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching lpsrDalSegnoAlCodaAtom::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void lpsrDalSegnoAlCodaAtom::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> lpsrDalSegnoAlCodaAtom::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_lpsrDalSegnoAlCodaAtom>*
    p =
      dynamic_cast<visitor<S_lpsrDalSegnoAlCodaAtom>*> (v)) {
        S_lpsrDalSegnoAlCodaAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching lpsrDalSegnoAlCodaAtom::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void lpsrDalSegnoAlCodaAtom::browseData (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> lpsrDalSegnoAlCodaAtom::browseData ()" <<
      endl;
  }
#endif
}

string lpsrDalSegnoAlCodaAtom::asShortNamedOptionString () const
{
  stringstream s;

  s <<
    "-" << fShortName << " ";

  if (! fStringToDalSegnoKindMapVariable.size ()) {
    s << "none";
  }
  else {
    map<string, msrDalSegno::msrDalSegnoKind>::const_iterator
      iBegin = fStringToDalSegnoKindMapVariable.begin (),
      iEnd   = fStringToDalSegnoKindMapVariable.end (),
      i      = iBegin;
    for ( ; ; ) {
      s << (*i).first << "=" << (*i).second;
      if (++i == iEnd) break;
      s << ",";
    } // for
  }

  return s.str ();
}

string lpsrDalSegnoAlCodaAtom::asActualLongNamedOptionString () const
{
  stringstream s;

  s <<
    "-" << fLongName << " ";

  if (! fStringToDalSegnoKindMapVariable.size ()) {
    s << "none";
  }
  else {
    map<string, msrDalSegno::msrDalSegnoKind>::const_iterator
      iBegin = fStringToDalSegnoKindMapVariable.begin (),
      iEnd   = fStringToDalSegnoKindMapVariable.end (),
      i      = iBegin;
    for ( ; ; ) {
      s << (*i).first << "=" << (*i).second;
      if (++i == iEnd) break;
      s << ",";
    } // for
  }

  return s.str ();
}

void lpsrDalSegnoAlCodaAtom::print (ostream& os) const
{
  const int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "lpsrDalSegnoAlCodaAtom:" <<
    endl;

  gIndenter++;

  printAtomWithValueEssentials (
    os, fieldWidth);

  os << left <<
    setw (fieldWidth) <<
    "fVariableName" << " : " <<
    fVariableName <<
    setw (fieldWidth) <<
    "fStringToDalSegnoKindMapVariable" << " : " <<
    endl;

  if (! fStringToDalSegnoKindMapVariable.size ()) {
    os << "none";
  }
  else {
    map<string, msrDalSegno::msrDalSegnoKind>::const_iterator
      iBegin = fStringToDalSegnoKindMapVariable.begin (),
      iEnd   = fStringToDalSegnoKindMapVariable.end (),
      i      = iBegin;
    for ( ; ; ) {
      os << (*i).first << " --> " << (*i).second;
      if (++i == iEnd) break;
      os << endl;
    } // for
  }

  gIndenter--;

  os << endl;
}

void lpsrDalSegnoAlCodaAtom::printAtomWithValueOptionsValues (
  ostream& os,
  int      valueFieldWidth) const
{
  os << left <<
    setw (valueFieldWidth) <<
    fVariableName <<
    " : ";

  if (! fStringToDalSegnoKindMapVariable.size ()) {
    os <<
      "none" <<
      endl;
  }
  else {
    os << endl;
    gIndenter++;

    map<string, msrDalSegno::msrDalSegnoKind>::const_iterator
      iBegin = fStringToDalSegnoKindMapVariable.begin (),
      iEnd   = fStringToDalSegnoKindMapVariable.end (),
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

    os <<
      ", variableHasBeenSet: " <<
      booleanAsString (fVariableHasBeenSet);

    gIndenter--;
  }
}

ostream& operator<< (ostream& os, const S_lpsrDalSegnoAlCodaAtom& elt)
{
  elt->print (os);
  return os;
}
*/

