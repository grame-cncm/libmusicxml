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

#include "msprOah.h"

#include "messagesHandling.h"


using namespace std;

namespace MusicXML2
{

//______________________________________________________________________________
S_msprPitchesLanguageAtom msprPitchesLanguageAtom::create (
  string             shortName,
  string             longName,
  string             description,
  string             valueSpecification,
  string             variableName,
  msrQuarterTonesPitchesLanguageKind&
                     msprPitchesLanguageKindVariable)
{
  msprPitchesLanguageAtom* o = new
    msprPitchesLanguageAtom (
      shortName,
      longName,
      description,
      valueSpecification,
      variableName,
      msprPitchesLanguageKindVariable);
  assert (o != nullptr);
  return o;
}

msprPitchesLanguageAtom::msprPitchesLanguageAtom (
  string             shortName,
  string             longName,
  string             description,
  string             valueSpecification,
  string             variableName,
  msrQuarterTonesPitchesLanguageKind&
                     msprPitchesLanguageKindVariable)
  : oahAtomWithValue (
      shortName,
      longName,
      description,
      valueSpecification,
      variableName),
    fMsrQuarterTonesPitchesLanguageKindVariable (
      msprPitchesLanguageKindVariable)
{}

msprPitchesLanguageAtom::~msprPitchesLanguageAtom ()
{}

void msprPitchesLanguageAtom::applyAtomWithValue (
  const string& theString,
  ostream&      os)
{

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceOah ()) {
    gLogStream <<
      "==> oahAtom is of type 'msprPitchesLanguageAtom'" <<
      endl;
  }
#endif

  // theString contains the language name:
  // is it in the pitches languages map?

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceOah ()) {
    gLogStream <<
      "==> oahAtom is of type 'msprPitchesLanguageAtom'" <<
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

void msprPitchesLanguageAtom::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> msprPitchesLanguageAtom::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_msprPitchesLanguageAtom>*
    p =
      dynamic_cast<visitor<S_msprPitchesLanguageAtom>*> (v)) {
        S_msprPitchesLanguageAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching msprPitchesLanguageAtom::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void msprPitchesLanguageAtom::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> msprPitchesLanguageAtom::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_msprPitchesLanguageAtom>*
    p =
      dynamic_cast<visitor<S_msprPitchesLanguageAtom>*> (v)) {
        S_msprPitchesLanguageAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching msprPitchesLanguageAtom::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void msprPitchesLanguageAtom::browseData (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> msprPitchesLanguageAtom::browseData ()" <<
      endl;
  }
#endif
}

string msprPitchesLanguageAtom::asShortNamedOptionString () const
{
  stringstream s;

  s <<
    "-" << fShortName << " " <<
    msrQuarterTonesPitchesLanguageKindAsString (
      fMsrQuarterTonesPitchesLanguageKindVariable);

  return s.str ();
}

string msprPitchesLanguageAtom::asActualLongNamedOptionString () const
{
  stringstream s;

  s <<
    "-" << fLongName << " " <<
    msrQuarterTonesPitchesLanguageKindAsString (
      fMsrQuarterTonesPitchesLanguageKindVariable);

  return s.str ();
}

void msprPitchesLanguageAtom::print (ostream& os) const
{
  const unsigned int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "msprPitchesLanguageAtom:" <<
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

void msprPitchesLanguageAtom::printAtomWithValueOptionsValues (
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

ostream& operator<< (ostream& os, const S_msprPitchesLanguageAtom& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_msprChordsLanguageAtom msprChordsLanguageAtom::create (
  string             shortName,
  string             longName,
  string             description,
  string             valueSpecification,
  string             variableName,
  msprChordsLanguageKind&
                     msprChordsLanguageKindVariable)
{
  msprChordsLanguageAtom* o = new
    msprChordsLanguageAtom (
      shortName,
      longName,
      description,
      valueSpecification,
      variableName,
      msprChordsLanguageKindVariable);
  assert (o != nullptr);
  return o;
}

msprChordsLanguageAtom::msprChordsLanguageAtom (
  string             shortName,
  string             longName,
  string             description,
  string             valueSpecification,
  string             variableName,
  msprChordsLanguageKind&
                     msprChordsLanguageKindVariable)
  : oahAtomWithValue (
      shortName,
      longName,
      description,
      valueSpecification,
      variableName),
    fLpsrChordsLanguageKindVariable (
      msprChordsLanguageKindVariable)
{}

msprChordsLanguageAtom::~msprChordsLanguageAtom ()
{}

void msprChordsLanguageAtom::applyAtomWithValue (
  const string& theString,
  ostream&      os)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceOah ()) {
    gLogStream <<
      "==> oahAtom is of type 'msprChordsLanguageAtom'" <<
      endl;
  }
#endif

  // theString contains the language name:
  // is it in the chords languages map?

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceOah ()) {
    gLogStream <<
      "==> oahAtom is of type 'msprChordsLanguageAtom'" <<
      endl;
  }
#endif

  map<string, msprChordsLanguageKind>::const_iterator
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

void msprChordsLanguageAtom::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> msprChordsLanguageAtom::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_msprChordsLanguageAtom>*
    p =
      dynamic_cast<visitor<S_msprChordsLanguageAtom>*> (v)) {
        S_msprChordsLanguageAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching msprChordsLanguageAtom::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void msprChordsLanguageAtom::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> msprChordsLanguageAtom::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_msprChordsLanguageAtom>*
    p =
      dynamic_cast<visitor<S_msprChordsLanguageAtom>*> (v)) {
        S_msprChordsLanguageAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching msprChordsLanguageAtom::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void msprChordsLanguageAtom::browseData (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> msprChordsLanguageAtom::browseData ()" <<
      endl;
  }
#endif
}

string msprChordsLanguageAtom::asShortNamedOptionString () const
{
  stringstream s;

  s <<
    "-" << fShortName << " " <<
    msprChordsLanguageKindAsString (fLpsrChordsLanguageKindVariable);

  return s.str ();
}

string msprChordsLanguageAtom::asActualLongNamedOptionString () const
{
  stringstream s;

  s <<
    "-" << fLongName << " " <<
    msprChordsLanguageKindAsString (fLpsrChordsLanguageKindVariable);

  return s.str ();
}

void msprChordsLanguageAtom::print (ostream& os) const
{
  const unsigned int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "msprChordsLanguageAtom:" <<
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
    msprChordsLanguageKindAsString (
      fLpsrChordsLanguageKindVariable) <<
    "\"" <<
    endl;

  gIndenter--;
}

void msprChordsLanguageAtom::printAtomWithValueOptionsValues (
  ostream&     os,
  unsigned int valueFieldWidth) const
{
  os << left <<
    setw (valueFieldWidth) <<
    fVariableName <<
    " : \"" <<
    msprChordsLanguageKindAsString (
      fLpsrChordsLanguageKindVariable) <<
    "\"";
  if (fVariableHasBeenSet) {
    os <<
      ", variableHasBeenSet: " <<
      booleanAsString (fVariableHasBeenSet);
  }
  os << endl;
}

ostream& operator<< (ostream& os, const S_msprChordsLanguageAtom& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_msprTransposeAtom msprTransposeAtom::create (
  string  shortName,
  string  longName,
  string  description,
  string  valueSpecification,
  string  variableName,
  S_msrSemiTonesPitchAndOctave&
          semiTonesPitchAndOctaveVariable)
{
  msprTransposeAtom* o = new
    msprTransposeAtom (
      shortName,
      longName,
      description,
      valueSpecification,
      variableName,
      semiTonesPitchAndOctaveVariable);
  assert (o != nullptr);
  return o;
}

msprTransposeAtom::msprTransposeAtom (
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

msprTransposeAtom::~msprTransposeAtom ()
{}

void msprTransposeAtom::applyAtomWithValue (
  const string& theString,
  ostream&      os)
{

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceOah ()) {
    gLogStream <<
      "==> oahAtom is of type 'msprTransposeAtom'" <<
      endl;
  }
#endif

  // theString contains the language name:
  // is it in the chords languages map?

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceOah ()) {
    gLogStream <<
      "==> oahAtom is of type 'msprTransposeAtom'" <<
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

void msprTransposeAtom::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> msprTransposeAtom::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_msprTransposeAtom>*
    p =
      dynamic_cast<visitor<S_msprTransposeAtom>*> (v)) {
        S_msprTransposeAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching msprTransposeAtom::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void msprTransposeAtom::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> msprTransposeAtom::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_msprTransposeAtom>*
    p =
      dynamic_cast<visitor<S_msprTransposeAtom>*> (v)) {
        S_msprTransposeAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching msprTransposeAtom::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void msprTransposeAtom::browseData (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> msprTransposeAtom::browseData ()" <<
      endl;
  }
#endif
}

string msprTransposeAtom::asShortNamedOptionString () const
{
  stringstream s;

  s <<
    "-" << fShortName << " " <<
    fSemiTonesPitchAndOctaveVariable->asString ();

  return s.str ();
}

string msprTransposeAtom::asActualLongNamedOptionString () const
{
  stringstream s;

  s <<
    "-" << fLongName << " " <<
    fSemiTonesPitchAndOctaveVariable->asString ();

  return s.str ();
}

void msprTransposeAtom::print (ostream& os) const
{
  const unsigned int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "msprTransposeAtom:" <<
    endl;

  gIndenter++;

  oahElement::printOahElementEssentials (
    os, fieldWidth);

  gIndenter--;
}

void msprTransposeAtom::printAtomWithValueOptionsValues (
  ostream&     os,
  unsigned int valueFieldWidth) const
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

ostream& operator<< (ostream& os, const S_msprTransposeAtom& elt)
{
  elt->print (os);
  return os;
}

//_______________________________________________________________________________
S_msprOahGroup gGlobalLpsrOahGroup;

S_msprOahGroup msprOahGroup::create ()
{
  msprOahGroup* o = new msprOahGroup ();
  assert (o != nullptr);
  return o;
}

msprOahGroup::msprOahGroup ()
  : oahGroup (
    "LPSR",
    "hmspr", "help-mspr",
R"(These options control the way LPSR data is handled.)",
    kElementVisibilityWhole)
{
  initializeLpsrOahGroup ();
}

msprOahGroup::~msprOahGroup ()
{}

#ifdef TRACING_IS_ENABLED
void msprOahGroup::initializeLpsrTraceOah ()
{
  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "LPSR Trace",
        "hmsprtrace", "help-mspr-trace",
R"()",
      kElementVisibilityWhole,
      this);

  appendSubGroupToGroup (subGroup);

  // trace- mspr

  fTraceLpsr            = false;

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "tmspr", "trace-mspr",
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

  // trace mspr visitors

  fTraceLpsrVisitors    = false;

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "tmsprv", "trace-mspr-visitors",
R"(Write a trace of the LPSR graphs visiting activity to standard error.)",
        "traceLpsrVisitors",
        fTraceLpsrVisitors));

  // trace mspr blocks

  fTraceLpsrBlocks      = false;

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "tmsprb", "trace-mspr-blocks",
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

void msprOahGroup::initializeLpsrDisplayOptions ()
{
  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Display",
        "hmsprd", "help-mspr-display",
R"()",
      kElementVisibilityWhole,
      this);

  appendSubGroupToGroup (subGroup);

  // display LPSR

  fDisplayLpsr = false;

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "dmspr", "display-mspr",
R"(Write the contents of the LPSR data to standard error.)",
        "displayLpsr",
        fDisplayLpsr));

  // display LPSR short

  fDisplayLpsrShort = false;

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "dmsprs", "display-mspr-short",
R"(Write the contents of the LPSR data, short version, to standard error.)",
        "displayLpsrShort",
        fDisplayLpsrShort));
}

void msprOahGroup::initializeLpsrPaperOptions ()
{
  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Paper",
        "hmsprpaper", "help-mspr-paper",
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
This helps the user adapting the generate code for their needs.)",
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

void msprOahGroup::initializeLpsrMeasuresOptions ()
{
  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Measures",
        "hmsprmeasures", "help-mspr-measures",
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

void msprOahGroup::initializeLpsrTemposOptions ()
{
  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Tempos",
        "hmsprtempos", "help-mspr-tempos",
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

void msprOahGroup::initializeLpsrWordsOptions ()
{
  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Words",
        "hmsprwords", "help-mspr-words",
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
      msprDalSegnoAtom::create (
        "ds", "dal-segno",
R"(Convert words elements STRING to an MSR 'dal segno' element'.)",
        "STRING",
        "convertWordsToRehearsalMarks",
        fConvertWordsToDalSegno));

  // convert words to dal segno al fine
  subGroup->
    appendAtomToSubGroup (
      msprDalSegnoAlFineAtom::create (
        "dsaf", "dal-segno-al-fine",
R"(Convert words elements STRING to an MSR 'dal segno al fine' element.)",
        "STRING",
        "convertWordsToRehearsalMarks",
        fConvertWordsToDalSegno));

  // convert words to dal segno al coda
  subGroup->
    appendAtomToSubGroup (
      msprDalSegnoAlCodaAtom::create (
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
        "msprPitchesLanguage",
        fLpsrQuarterTonesPitchesLanguageKind));

    map<string, msrDalSegno::msrDalSegnoKind>
                          fConvertWordsToDalSegno;
                          */
}

void msprOahGroup::initializeLpsrLanguagesOptions ()
{
  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Languages",
        "hmsprl", "help-mspr-languages",
R"()",
      kElementVisibilityWhole,
      this);

  appendSubGroupToGroup (subGroup);

  // mspr pitches language

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
      msprPitchesLanguageAtom::create (
        "lppl", "mspr-pitches-language",
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
        "msprPitchesLanguage",
        fLpsrQuarterTonesPitchesLanguageKind));

  // mspr chords language

  const msprChordsLanguageKind
    msprChordsLanguageKindDefaultValue =
      k_IgnatzekChords; // LilyPond default

  fLpsrChordsLanguageKind =
    msprChordsLanguageKindDefaultValue;

  subGroup->
    appendAtomToSubGroup (
      msprChordsLanguageAtom::create (
        "lpcl", "mspr-chords-language",
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
          msprChordsLanguageKindAsString (
            msprChordsLanguageKindDefaultValue)),
        "LANGUAGE",
        "mspr-chords-language",
        fLpsrChordsLanguageKind));
}

void msprOahGroup::initializeLpsrTransposeOptions ()
{
  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Transpose",
        "hmsprt", "help-mspr-transpose",
R"()",
      kElementVisibilityWhole,
      this);

  appendSubGroupToGroup (subGroup);

  // mspr transpose

  subGroup->
    appendAtomToSubGroup (
      msprTransposeAtom::create (
        "lpt", "mspr-transpose",
R"(Use TRANSPOSITION to tranpose in the LPSR data,
thus in the generated LilyPond code as well.
TRANSPOSITION should contain a diatonic pitch, followed if needed
by a sequence of ',' or '\'' octave indications.
Such indications cannot be mixed.
For example, 'a', 'f' and 'bes,' can be used respectively
for instruments in 'a', 'f' and B flat respectively)",
        "TRANSPOSITION",
        "msprTranspose",
        fTransposeSemiTonesPitchAndOctave));
}

void msprOahGroup::initializeLpsrQuitAfterSomePassesOptions ()
{
  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Quit after some passes",
        "hlquit", "help-mspr-quit",
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

void msprOahGroup::initializeLpsrOahGroup ()
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
bool msprOahGroup::setLpsrQuarterTonesPitchesLanguage (string language)
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
bool msprOahGroup::setLpsrChordsLanguage (string language)
{
  // is language in the chords languages map?
  map<string, msprChordsLanguageKind>::const_iterator
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
void msprOahGroup::enforceGroupQuietness ()
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
void msprOahGroup::checkGroupOptionsConsistency ()
{
  // JMI
}

//______________________________________________________________________________
void msprOahGroup::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> msprOahGroup::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_msprOahGroup>*
    p =
      dynamic_cast<visitor<S_msprOahGroup>*> (v)) {
        S_msprOahGroup elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching msprOahGroup::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void msprOahGroup::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> msprOahGroup::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_msprOahGroup>*
    p =
      dynamic_cast<visitor<S_msprOahGroup>*> (v)) {
        S_msprOahGroup elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching msprOahGroup::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void msprOahGroup::browseData (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> msprOahGroup::browseData ()" <<
      endl;
  }
#endif
}

//______________________________________________________________________________
void msprOahGroup::printLpsrOahValues (unsigned int fieldWidth)
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
    setw (fieldWidth) << "msprQuarterTonesPitchesLanguage" << " : \"" <<
    msrQuarterTonesPitchesLanguageKindAsString (
      fLpsrQuarterTonesPitchesLanguageKind) <<
    "\"" <<
    endl <<

    setw (fieldWidth) << "msprChordsLanguage" << " : \"" <<
    msprChordsLanguageKindAsString (
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

ostream& operator<< (ostream& os, const S_msprOahGroup& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_msprOahGroup createGlobalLpsrOahGroup ()
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
      msprOahGroup::create ();
    assert (gGlobalLpsrOahGroup != 0);
  }

  // return the global OAH group
  return gGlobalLpsrOahGroup;
}


}


/* JMI
//______________________________________________________________________________
S_msprDalSegnoAtom msprDalSegnoAtom::create (
  string              shortName,
  string              longName,
  string              description,
  string              valueSpecification,
  string              variableName,
  map<string, msrDalSegno::msrDalSegnoKind>&
                      stringToDalSegnoKindMapVariable)
{
  msprDalSegnoAtom* o = new
    msprDalSegnoAtom (
      shortName,
      longName,
      description,
      valueSpecification,
      variableName,
      stringToDalSegnoKindMapVariable);
  assert (o != nullptr);
  return o;
}

msprDalSegnoAtom::msprDalSegnoAtom (
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

msprDalSegnoAtom::~msprDalSegnoAtom ()
{}

void msprDalSegnoAtom::applyAtomWithValue (
  const string& theString,
  ostream&      os)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceOah ()) {
    gLogStream <<
      "==> oahAtom is of type 'msprDalSegnoAtom'" <<
      endl;
  }
#endif

  // theString contains the dal segno specification

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceOah ()) {
    gLogStream <<
      "==> oahAtom is of type 'msprDalSegnoAtom'" <<
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

void msprDalSegnoAtom::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> msprDalSegnoAtom::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_msprDalSegnoAtom>*
    p =
      dynamic_cast<visitor<S_msprDalSegnoAtom>*> (v)) {
        S_msprDalSegnoAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching msprDalSegnoAtom::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void msprDalSegnoAtom::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> msprDalSegnoAtom::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_msprDalSegnoAtom>*
    p =
      dynamic_cast<visitor<S_msprDalSegnoAtom>*> (v)) {
        S_msprDalSegnoAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching msprDalSegnoAtom::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void msprDalSegnoAtom::browseData (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> msprDalSegnoAtom::browseData ()" <<
      endl;
  }
#endif
}

string msprDalSegnoAtom::asShortNamedOptionString () const
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

string msprDalSegnoAtom::asActualLongNamedOptionString () const
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

void msprDalSegnoAtom::print (ostream& os) const
{
  const unsigned int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "msprDalSegnoAtom:" <<
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

void msprDalSegnoAtom::printAtomWithValueOptionsValues (
  ostream&     os,
  unsigned int valueFieldWidth) const
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

ostream& operator<< (ostream& os, const S_msprDalSegnoAtom& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_msprDalSegnoAlFineAtom msprDalSegnoAlFineAtom::create (
  string              shortName,
  string              longName,
  string              description,
  string              valueSpecification,
  string              variableName,
  map<string, msrDalSegno::msrDalSegnoKind>&
                      stringToDalSegnoKindMapVariable)
{
  msprDalSegnoAlFineAtom* o = new
    msprDalSegnoAlFineAtom (
      shortName,
      longName,
      description,
      valueSpecification,
      variableName,
      stringToDalSegnoKindMapVariable);
  assert (o != nullptr);
  return o;
}

msprDalSegnoAlFineAtom::msprDalSegnoAlFineAtom (
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

msprDalSegnoAlFineAtom::~msprDalSegnoAlFineAtom ()
{}

void msprDalSegnoAlFineAtom::applyAtomWithValue (
  const string& theString,
  ostream&      os)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceOah ()) {
    gLogStream <<
      "==> oahAtom is of type 'msprDalSegnoAlFineAtom'" <<
      endl;
  }
#endif

  // theString contains the dal segno al fine specification

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceOah ()) {
    gLogStream <<
      "==> oahAtom is of type 'msprDalSegnoAlFineAtom'" <<
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

void msprDalSegnoAlFineAtom::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> msprDalSegnoAlFineAtom::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_msprDalSegnoAlFineAtom>*
    p =
      dynamic_cast<visitor<S_msprDalSegnoAlFineAtom>*> (v)) {
        S_msprDalSegnoAlFineAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching msprDalSegnoAlFineAtom::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void msprDalSegnoAlFineAtom::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> msprDalSegnoAlFineAtom::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_msprDalSegnoAlFineAtom>*
    p =
      dynamic_cast<visitor<S_msprDalSegnoAlFineAtom>*> (v)) {
        S_msprDalSegnoAlFineAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching msprDalSegnoAlFineAtom::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void msprDalSegnoAlFineAtom::browseData (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> msprDalSegnoAlFineAtom::browseData ()" <<
      endl;
  }
#endif
}

string msprDalSegnoAlFineAtom::asShortNamedOptionString () const
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

string msprDalSegnoAlFineAtom::asActualLongNamedOptionString () const
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

void msprDalSegnoAlFineAtom::print (ostream& os) const
{
  const unsigned int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "msprDalSegnoAlFineAtom:" <<
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

void msprDalSegnoAlFineAtom::printAtomWithValueOptionsValues (
  ostream&     os,
  unsigned int valueFieldWidth) const
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

ostream& operator<< (ostream& os, const S_msprDalSegnoAlFineAtom& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_msprDalSegnoAlCodaAtom msprDalSegnoAlCodaAtom::create (
  string              shortName,
  string              longName,
  string              description,
  string              valueSpecification,
  string              variableName,
  map<string, msrDalSegno::msrDalSegnoKind>&
                      stringToDalSegnoKindMapVariable)
{
  msprDalSegnoAlCodaAtom* o = new
    msprDalSegnoAlCodaAtom (
      shortName,
      longName,
      description,
      valueSpecification,
      variableName,
      stringToDalSegnoKindMapVariable);
  assert (o != nullptr);
  return o;
}

msprDalSegnoAlCodaAtom::msprDalSegnoAlCodaAtom (
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

msprDalSegnoAlCodaAtom::~msprDalSegnoAlCodaAtom ()
{}

void msprDalSegnoAlCodaAtom::applyAtomWithValue (
  const string& theString,
  ostream&      os)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceOah ()) {
    gLogStream <<
      "==> oahAtom is of type 'msprDalSegnoAlCodaAtom'" <<
      endl;
  }
#endif

  // theString contains the dal segno specification
  // decipher it to extract the old and new part names

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceOah ()) {
    gLogStream <<
      "==> oahAtom is of type 'msprDalSegnoAlCodaAtom'" <<
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

void msprDalSegnoAlCodaAtom::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> msprDalSegnoAlCodaAtom::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_msprDalSegnoAlCodaAtom>*
    p =
      dynamic_cast<visitor<S_msprDalSegnoAlCodaAtom>*> (v)) {
        S_msprDalSegnoAlCodaAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching msprDalSegnoAlCodaAtom::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void msprDalSegnoAlCodaAtom::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> msprDalSegnoAlCodaAtom::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_msprDalSegnoAlCodaAtom>*
    p =
      dynamic_cast<visitor<S_msprDalSegnoAlCodaAtom>*> (v)) {
        S_msprDalSegnoAlCodaAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching msprDalSegnoAlCodaAtom::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void msprDalSegnoAlCodaAtom::browseData (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> msprDalSegnoAlCodaAtom::browseData ()" <<
      endl;
  }
#endif
}

string msprDalSegnoAlCodaAtom::asShortNamedOptionString () const
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

string msprDalSegnoAlCodaAtom::asActualLongNamedOptionString () const
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

void msprDalSegnoAlCodaAtom::print (ostream& os) const
{
  const unsigned int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "msprDalSegnoAlCodaAtom:" <<
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

void msprDalSegnoAlCodaAtom::printAtomWithValueOptionsValues (
  ostream&     os,
  unsigned int valueFieldWidth) const
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

ostream& operator<< (ostream& os, const S_msprDalSegnoAlCodaAtom& elt)
{
  elt->print (os);
  return os;
}
*/

