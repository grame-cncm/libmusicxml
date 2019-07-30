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
#include "lpsrOah.h"
#include "lilypondOah.h"


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
  assert(o!=0);
  return o;
}

lilypondScoreOutputKindAtom::lilypondScoreOutputKindAtom (
  string               shortName,
  string               longName,
  string               description,
  string               valueSpecification,
  string               variableName,
  lpsrScoreOutputKind& lilypondScoreOutputKindVariable)
  : oahValuedAtom (
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

void lilypondScoreOutputKindAtom::handleValue (
  string   theString,
  ostream& os)
{
#ifdef TRACE_OAH
  if (gExecutableOah->fTraceOah) {
    os <<
      "==> oahAtom is of type 'lilypondScoreOutputKindAtom'" <<
      endl;
  }
#endif

}

void lilypondScoreOutputKindAtom::print (ostream& os) const
{
  const int fieldWidth = K_OPTIONS_FIELD_WIDTH;

  os <<
    "OptionsScoreOutputKindAtom:" <<
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
    "fLpsrScoreOutputKindVariable" << " : \"" <<
    lpsrScoreOutputKindAsString (
      fLpsrScoreOutputKindVariable) <<
    "\"" <<
    endl;
}

void lilypondScoreOutputKindAtom::printAtomOptionsValues (
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

ostream& operator<< (ostream& os, const S_lilypondScoreOutputKindAtom& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_lilypondAbsoluteOctaveEntryAtom lilypondAbsoluteOctaveEntryAtom::create (
  string shortName,
  string longName,
  string description,
  string lilypondAbsoluteOctaveEntryVariableName)
{
  lilypondAbsoluteOctaveEntryAtom* o = new
    lilypondAbsoluteOctaveEntryAtom (
      shortName,
      longName,
      description,
      lilypondAbsoluteOctaveEntryVariableName);
  assert(o!=0);
  return o;
}

lilypondAbsoluteOctaveEntryAtom::lilypondAbsoluteOctaveEntryAtom (
  string shortName,
  string longName,
  string description,
  string lilypondAbsoluteOctaveEntryVariableName)
  : oahAtomWithVariableName (
      shortName,
      longName,
      description,
      lilypondAbsoluteOctaveEntryVariableName)
{}

lilypondAbsoluteOctaveEntryAtom::~lilypondAbsoluteOctaveEntryAtom ()
{}

void lilypondAbsoluteOctaveEntryAtom::print (ostream& os) const
{
  const int fieldWidth = K_OPTIONS_FIELD_WIDTH;

  os <<
    "lilypondAbsoluteOctaveEntryAtom:" <<
    endl;

  gIndenter++;

  oahOption::printOptionEssentials (
    os, fieldWidth);

  gIndenter++;
  os <<
    gIndenter.indentMultiLineString (
      fDescription) <<
    endl;
  gIndenter--;

  gIndenter--;
}

void lilypondAbsoluteOctaveEntryAtom::printAtomOptionsValues (
  ostream& os,
  int      valueFieldWidth) const
{
  os << left <<
    setw (valueFieldWidth) <<
    fVariableName <<
    " : " <<
    "- no value here -" <<
    endl;
}

//______________________________________________________________________________
S_lilypondRelativeOctaveEntryAtom lilypondRelativeOctaveEntryAtom::create (
  string                        shortName,
  string                        longName,
  string                        description,
  string                        valueSpecification,
  string                        variableName,
  S_msrSemiTonesPitchAndOctave& lilypondRelativeOctaveEntryVariable)
{
  lilypondRelativeOctaveEntryAtom* o = new
    lilypondRelativeOctaveEntryAtom (
      shortName,
      longName,
      description,
      valueSpecification,
      variableName,
      lilypondRelativeOctaveEntryVariable);
  assert(o!=0);
  return o;
}

lilypondRelativeOctaveEntryAtom::lilypondRelativeOctaveEntryAtom (
  string                        shortName,
  string                        longName,
  string                        description,
  string                        valueSpecification,
  string                        variableName,
  S_msrSemiTonesPitchAndOctave& lilypondRelativeOctaveEntryVariable)
  : oahValuedAtom (
      shortName,
      longName,
      description,
      valueSpecification,
      variableName),
    fMsrSemiTonesPitchAndOctaveVariable (
      lilypondRelativeOctaveEntryVariable)
{}

lilypondRelativeOctaveEntryAtom::~lilypondRelativeOctaveEntryAtom ()
{}

void lilypondRelativeOctaveEntryAtom::handleValue (
  string   theString,
  ostream& os)
{
#ifdef TRACE_OAH
  if (gExecutableOah->fTraceOah) {
    os <<
      "==> oahAtom is of type 'lilypondRelativeOctaveEntryAtom'" <<
      endl;
  }
#endif

  // theString contains the score output kind:
  // is it in the score output kinds map?

#ifdef TRACE_OAH
  if (gExecutableOah->fTraceOah) {
    os <<
      "==> oahAtom is of type 'lilypondRelativeOctaveEntryAtom'" <<
      endl;
  }
#endif

/* JMI
  map<string, lpsrOctaveEntryKind>::const_iterator
    it =
      gLpsrOctaveEntryKindsMap.find (
        theString);

  if (it == gLpsrOctaveEntryKindsMap.end ()) {
    // no, octave entry kind is unknown in the map

    printOptionsSummary (os);

    stringstream s;

    s <<
      "octave entry kind '" << theString <<
      "' is unknown" <<
      endl <<
      "The " <<
      gLpsrOctaveEntryKindsMap.size () <<
      " known octave entry kinds are:" <<
      endl;

    gIndenter++;

    s <<
      existingLpsrOctaveEntryKinds ();

    gIndenter--;

    optionError (s.str ());

//     exit (4); // JMI
    abort ();
  }
*/

  setRelativeOctaveEntryVariableValue (
    msrSemiTonesPitchAndOctave::createFromString (
      K_NO_INPUT_LINE_NUMBER,
      theString));
}

void lilypondRelativeOctaveEntryAtom::print (ostream& os) const
{
  const int fieldWidth = K_OPTIONS_FIELD_WIDTH;

  os <<
    "lilypondRelativeOctaveEntryAtom:" <<
    endl;

  gIndenter++;

  oahOption::printOptionEssentials (
    os, fieldWidth);

  gIndenter++;
  os <<
    gIndenter.indentMultiLineString (
      fDescription) <<
    endl;
  gIndenter--;

  gIndenter--;
}

void lilypondRelativeOctaveEntryAtom::printAtomOptionsValues (
  ostream& os,
  int      valueFieldWidth) const
{
  os << left <<
    setw (valueFieldWidth) <<
    fVariableName <<
    " : ";
  if (fMsrSemiTonesPitchAndOctaveVariable) {
    os << endl;
    gIndenter++;
    os <<
      fMsrSemiTonesPitchAndOctaveVariable;
    gIndenter--;
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
  string  shortName,
  string  longName,
  string  description,
  string  valueSpecification,
  string  variableName,
  S_msrSemiTonesPitchAndOctave&
          lilypondFixedOctaveEntryVariable)
{
  lilypondFixedOctaveEntryAtom* o = new
    lilypondFixedOctaveEntryAtom (
      shortName,
      longName,
      description,
      valueSpecification,
      variableName,
      lilypondFixedOctaveEntryVariable);
  assert(o!=0);
  return o;
}

lilypondFixedOctaveEntryAtom::lilypondFixedOctaveEntryAtom (
  string  shortName,
  string  longName,
  string  description,
  string  valueSpecification,
  string  variableName,
  S_msrSemiTonesPitchAndOctave&
          lilypondFixedOctaveEntryVariable)
  : oahValuedAtom (
      shortName,
      longName,
      description,
      valueSpecification,
      variableName),
    fMsrSemiTonesPitchAndOctaveVariable (
      lilypondFixedOctaveEntryVariable)
{}

lilypondFixedOctaveEntryAtom::~lilypondFixedOctaveEntryAtom ()
{}

void lilypondFixedOctaveEntryAtom::handleValue (
  string   theString,
  ostream& os)
{
#ifdef TRACE_OAH
  if (gExecutableOah->fTraceOah) {
    os <<
      "==> oahAtom is of type 'lilypondFixedOctaveEntryAtom'" <<
      endl;
  }
#endif

  // theString contains the score output kind:
  // is it in the score output kinds map?

#ifdef TRACE_OAH
  if (gExecutableOah->fTraceOah) {
    os <<
      "==> oahAtom is of type 'lilypondRelativeOctaveEntryAtom'" <<
      endl;
  }
#endif

  setFixedOctaveEntryVariableValue (
    msrSemiTonesPitchAndOctave::createFromString (
      K_NO_INPUT_LINE_NUMBER,
      theString));
}

void lilypondFixedOctaveEntryAtom::print (ostream& os) const
{
  const int fieldWidth = K_OPTIONS_FIELD_WIDTH;

  os <<
    "lilypondFixedOctaveEntryAtom:" <<
    endl;

  gIndenter++;

  oahOption::printOptionEssentials (
    os, fieldWidth);

  gIndenter++;
  os <<
    gIndenter.indentMultiLineString (
      fDescription) <<
    endl;
  gIndenter--;

  gIndenter--;
}

void lilypondFixedOctaveEntryAtom::printAtomOptionsValues (
  ostream& os,
  int      valueFieldWidth) const
{
  os << left <<
    setw (valueFieldWidth) <<
    fVariableName <<
    " : ";
  if (fMsrSemiTonesPitchAndOctaveVariable) {
    os << endl;
    gIndenter++;
    os <<
      fMsrSemiTonesPitchAndOctaveVariable;
    gIndenter--;
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
S_lilypondResetMeasureNumberAtom lilypondResetMeasureNumberAtom::create (
  string         shortName,
  string         longName,
  string         description,
  string         valueSpecification,
  string         variableName,
  map<int, int>& lilypondResetMeasureNumberVariable)
{
  lilypondResetMeasureNumberAtom* o = new
    lilypondResetMeasureNumberAtom (
      shortName,
      longName,
      description,
      valueSpecification,
      variableName,
      lilypondResetMeasureNumberVariable);
  assert(o!=0);
  return o;
}

lilypondResetMeasureNumberAtom::lilypondResetMeasureNumberAtom (
  string         shortName,
  string         longName,
  string         description,
  string         valueSpecification,
  string         variableName,
  map<int, int>& lilypondResetMeasureNumberVariable)
  : oahValuedAtom (
      shortName,
      longName,
      description,
      valueSpecification,
      variableName),
    fIntIntMapVariable (
      lilypondResetMeasureNumberVariable)
{}

lilypondResetMeasureNumberAtom::~lilypondResetMeasureNumberAtom ()
{}

void lilypondResetMeasureNumberAtom::handleValue (
  string   theString,
  ostream& os)
{
#ifdef TRACE_OAH
  if (gExecutableOah->fTraceOah) {
    os <<
      "==> oahAtom is of type 'lilypondResetMeasureNumberAtom'" <<
      endl;
  }
#endif

  // theString contains the midi tempo specification
  // decipher it to extract duration and perSecond values

#ifdef TRACE_OAH
  if (gExecutableOah->fTraceOah) {
    os <<
      "==> oahAtom is of type 'lilypondResetMeasureNumberAtom'" <<
      endl;
  }
#endif

  string regularExpression (
    "[[:space:]]*"
    "([[:digit:]]+\\.*)"
    "[[:space:]]*"
    "="
    "[[:space:]]*"
    "([[:digit:]]+)"
    "[[:space:]]*");

  regex  e (regularExpression);
  smatch sm;

  regex_match (theString, sm, e);

  unsigned smSize = sm.size ();

#ifdef TRACE_OAH
  if (gExecutableOah->fTraceOah) {
    os <<
      "There are " << smSize << " matches" <<
      " for MIDI tempo string '" << theString <<
      "' with regex '" << regularExpression <<
      "':" <<
      endl;

    gIndenter++;

    for (unsigned i = 0; i < smSize; ++i) {
      os <<
        i << ": " << "\"" << sm [i] << "\"" <<
        endl;
    } // for
    os << endl;

    gIndenter--;
  }
#endif

  if (smSize != 3) {
    stringstream s;

    s <<
      "-resetMeasureNumber argument '" << theString <<
      "' is ill-formed";

    optionError (s.str ());
    exit (4);
  }

  int oldMeasureNumber;
  {
    stringstream s;
    s << sm [1];
    s >> oldMeasureNumber;
  }

  int newMeasureNumber;
  {
    stringstream s;
    s << sm [2];
    s >> newMeasureNumber;
  }

#ifdef TRACE_OAH
  if (gExecutableOah->fTraceOah) {
    os <<
      "oldMeasureNumber  = " <<
      oldMeasureNumber <<
      endl <<
      "newMeasureNumber = " <<
      newMeasureNumber <<
      endl;

  fIntIntMapVariable [oldMeasureNumber] = newMeasureNumber;
  }
#endif
}

void lilypondResetMeasureNumberAtom::print (ostream& os) const
{
  const int fieldWidth = K_OPTIONS_FIELD_WIDTH;

  os <<
    "OptionsResetMeasureNumberAtom:" <<
    endl;

  gIndenter++;

  printValuedAtomEssentials (
    os, fieldWidth);

  os << left <<
    setw (fieldWidth) <<
    "fVariableName" << " : " <<
    fVariableName <<
    setw (fieldWidth) <<
    "fIntIntMapVariable" << " : '" <<
    endl;

  if (! fIntIntMapVariable.size ()) {
    os << "none";
  }
  else {
    map<int, int>::const_iterator
      iBegin = fIntIntMapVariable.begin (),
      iEnd   = fIntIntMapVariable.end (),
      i      = iBegin;
    for ( ; ; ) {
      os << (*i).first << " --> " << (*i).second;
      if (++i == iEnd) break;
      os << endl;
    } // for
  }

  os << endl;
}

void lilypondResetMeasureNumberAtom::printAtomOptionsValues (
  ostream& os,
  int      valueFieldWidth) const
{
  os << left <<
    setw (valueFieldWidth) <<
    fVariableName <<
    " : ";

  if (! fIntIntMapVariable.size ()) {
    os <<
      "none" <<
      endl;
  }
  else {
    os << endl;
    gIndenter++;

    map<int, int>::const_iterator
      iBegin = fIntIntMapVariable.begin (),
      iEnd   = fIntIntMapVariable.end (),
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

ostream& operator<< (ostream& os, const S_lilypondResetMeasureNumberAtom& elt)
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
  assert(o!=0);
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
  : oahValuedAtom (
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

void lilypondAccidentalStyleKindAtom::handleValue (
  string   theString,
  ostream& os)
{
#ifdef TRACE_OAH
  if (gExecutableOah->fTraceOah) {
    os <<
      "==> oahAtom is of type 'lilypondAccidentalStyleKindAtom'" <<
      endl;
  }
#endif

  // theString contains the language name:
  // is it in the accidental styles map?

#ifdef TRACE_OAH
  if (gExecutableOah->fTraceOah) {
    os <<
      "==> oahAtom is of type 'lilypondAccidentalStyleKindAtom'" <<
      endl;
  }
#endif

  map<string, lpsrAccidentalStyleKind>::const_iterator
    it =
      gLpsrAccidentalStyleKindsMap.find (
        theString);

  if (it == gLpsrAccidentalStyleKindsMap.end ()) {
    // no, accidental style is unknown in the map
    stringstream s;

    s <<
      "LPSR accidental style '" << theString <<
      "' is unknown" <<
      endl <<
      "The " <<
      gLpsrAccidentalStyleKindsMap.size () - 1 <<
      " known LPSR accidental styles are:" <<
      endl;

    gIndenter++;

    s <<
      existingLpsrAccidentalStyleKinds ();

    gIndenter--;

    optionError (s.str ());
    exit (4);
  }

  setLpsrAccidentalStyleKindVariable (
    (*it).second);
}

void lilypondAccidentalStyleKindAtom::print (ostream& os) const
{
  const int fieldWidth = K_OPTIONS_FIELD_WIDTH;

  os <<
    "OptionsAccidentalStyleKindAtom:" <<
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
    "fLpsrAccidentalStyleKindVariable" << " : \"" <<
    lpsrAccidentalStyleKindAsString (
      fLpsrAccidentalStyleKindVariable) <<
    "\"" <<
    endl;
}

void lilypondAccidentalStyleKindAtom::printAtomOptionsValues (
  ostream& os,
  int      valueFieldWidth) const
{
  os << left <<
    setw (valueFieldWidth) <<
    fVariableName <<
    " : \"" <<
    lpsrAccidentalStyleKindAsString (
      fLpsrAccidentalStyleKindVariable) <<
    "\"" <<
    endl;
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
  assert(o!=0);
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
  : oahValuedAtom (
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

void lilypondChordsDisplayAtom::handleValue (
  string   theString,
  ostream& os)
{
#ifdef TRACE_OAH
  if (gExecutableOah->fTraceOah) {
    os <<
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

#ifdef TRACE_OAH
  if (gExecutableOah->fTraceOah) {
    os <<
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

  unsigned smSize = sm.size ();

#ifdef TRACE_OAH
  if (gExecutableOah->fTraceOah) {
    os <<
      "There are " << smSize << " matches" <<
      " for chords display string '" << theString <<
      "' with regex '" << regularExpression <<
      "':" <<
      endl;

    gIndenter++;

    for (unsigned i = 0; i < smSize; ++i) {
      os <<
        i << ": " << "\"" << sm [i] << "\"" <<
        endl;
    } // for
    os << endl;

    gIndenter--;
  }
#endif

  if (smSize != 3) {
    stringstream s;

    s <<
      "-chords-display argument '" << theString <<
      "' is ill-formed";

    optionError (s.str ());
    exit (4);
  }

  string chordContents     = sm [1];
  string chordsDisplaycode = sm [2];

#ifdef TRACE_OAH
  if (gExecutableOah->fTraceOah) {
    os <<
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

void lilypondChordsDisplayAtom::print (ostream& os) const
{
  const int fieldWidth = K_OPTIONS_FIELD_WIDTH;

  os <<
    "lilypondChordsDisplayAtom:" <<
    endl;

  gIndenter++;

  printValuedAtomEssentials (
    os, fieldWidth);

  os << left <<
    setw (fieldWidth) <<
    "fVariableName" << " : " <<
    fVariableName <<
    setw (fieldWidth) <<
    "fStringsPairListVariable" << " : '" <<
    endl;

  gIndenter++;

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

  gIndenter--;
}

void lilypondChordsDisplayAtom::printAtomOptionsValues (
  ostream& os,
  int      valueFieldWidth) const
{
  os << left <<
    setw (valueFieldWidth) <<
    fVariableName <<
    " : ";

  if (fStringsPairListVariable.size ()) {
    os << endl;
    gIndenter++;

    list<pair<string, string> >::const_iterator
      iBegin = fStringsPairListVariable.begin (),
      iEnd   = fStringsPairListVariable.end (),
      i      = iBegin;
    for ( ; ; ) {
      os << (*i).first << " --> " << (*i).second;
      if (++i == iEnd) break;
      os << endl;
    } // for

    gIndenter--;
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
S_lilypondMidiTempoAtom lilypondMidiTempoAtom::create (
  string             shortName,
  string             longName,
  string             description,
  string             valueSpecification,
  string             variableName,
  pair<string, int>& lilypondMidiTempoVariable)
{
  lilypondMidiTempoAtom* o = new
    lilypondMidiTempoAtom (
      shortName,
      longName,
      description,
      valueSpecification,
      variableName,
      lilypondMidiTempoVariable);
  assert(o!=0);
  return o;
}

lilypondMidiTempoAtom::lilypondMidiTempoAtom (
  string             shortName,
  string             longName,
  string             description,
  string             valueSpecification,
  string             variableName,
  pair<string, int>& lilypondMidiTempoVariable)
  : oahValuedAtom (
      shortName,
      longName,
      description,
      valueSpecification,
      variableName),
    fStringIntPairVariable (
      lilypondMidiTempoVariable)
{}

lilypondMidiTempoAtom::~lilypondMidiTempoAtom ()
{}

void lilypondMidiTempoAtom::handleValue (
  string   theString,
  ostream& os)
{
#ifdef TRACE_OAH
  if (gExecutableOah->fTraceOah) {
    os <<
      "==> oahAtom is of type 'lilypondMidiTempoAtom'" <<
      endl;
  }
#endif

  // theString contains the midi tempo specification
  // decipher it to extract duration and perSecond values

#ifdef TRACE_OAH
  if (gExecutableOah->fTraceOah) {
    os <<
      "==> oahAtom is of type 'lilypondMidiTempoAtom'" <<
      endl;
  }
#endif

  string regularExpression (
    "[[:space:]]*"
    "([[:digit:]]+\\.*)"
    "[[:space:]]*"
    "="
    "[[:space:]]*"
    "([[:digit:]]+)"
    "[[:space:]]*");

  regex  e (regularExpression);
  smatch sm;

  regex_match (theString, sm, e);

  unsigned smSize = sm.size ();

#ifdef TRACE_OAH
  if (gExecutableOah->fTraceOah) {
    os <<
      "There are " << smSize << " matches" <<
      " for MIDI tempo string '" << theString <<
      "' with regex '" << regularExpression <<
      "':" <<
      endl;

    gIndenter++;

    for (unsigned i = 0; i < smSize; ++i) {
      os <<
        i << ": " << "\"" << sm [i] << "\"" <<
        endl;
    } // for
    os << endl;

    gIndenter--;
  }
#endif

  if (smSize != 3) {
    stringstream s;

    s <<
      "-midiTempo argument '" << theString <<
      "' is ill-formed";

    optionError (s.str ());
    exit (4);
  }

  string midiTempoDuration  = sm [1];

  int    midiTempoPerSecond;
  {
    stringstream s;
    s << sm [2];
    s >> midiTempoPerSecond;
  }

#ifdef TRACE_OAH
  if (gExecutableOah->fTraceOah) {
    os <<
      "midiTempoDuration  = " <<
      midiTempoDuration <<
      endl <<
      "midiTempoPerSecond = " <<
      midiTempoPerSecond <<
      endl;

  fStringIntPairVariable =
    pair<string, int> (
      midiTempoDuration, midiTempoPerSecond);
  }
#endif
}

void lilypondMidiTempoAtom::print (ostream& os) const
{
  const int fieldWidth = K_OPTIONS_FIELD_WIDTH;

  os <<
    "OptionsMidiTempoAtom:" <<
    endl;

  gIndenter++;

  printValuedAtomEssentials (
    os, fieldWidth);

  os << left <<
    setw (fieldWidth) <<
    "fVariableName" << " : " <<
    fVariableName <<
    setw (fieldWidth) <<
    "fStringIntPairVariable" << " : '" <<
    fStringIntPairVariable.first <<
    " = " <<
    fStringIntPairVariable.second <<
    "'" <<
    endl;
}

void lilypondMidiTempoAtom::printAtomOptionsValues (
  ostream& os,
  int      valueFieldWidth) const
{
  os << left <<
    setw (valueFieldWidth) <<
    fVariableName <<
    " : '" <<
    fStringIntPairVariable.first <<
    " = " <<
    fStringIntPairVariable.second <<
    "'" <<
    endl;
}

ostream& operator<< (ostream& os, const S_lilypondMidiTempoAtom& elt)
{
  elt->print (os);
  return os;
}

//_______________________________________________________________________________
S_lilypondOah gLilypondOah;
S_lilypondOah gLilypondOahUserChoices;
S_lilypondOah gLilypondOahWithDetailedTrace;

S_lilypondOah lilypondOah::create (
  S_oahHandler handler)
{
  lilypondOah* o = new lilypondOah (
    handler);
  assert(o!=0);
  return o;
}

lilypondOah::lilypondOah (
  S_oahHandler handler)
  : oahGroup (
    "LilyPond",
    "hlp", "help-lilypond",
R"(These lilypond control which LilyPond code is generated.)",
    kElementVisibilityAlways,
    handler)
{
  // append this lilypond group to the lilypond handler
  // if relevant
  if (handler) {
    handler->
      appendGroupToHandler (this);
  }

  // initialize it
  initializeLilypondOah (false);
}

lilypondOah::~lilypondOah ()
{}

void lilypondOah::initializeIdentificationOptions (
  bool boolOptionsInitialValue)
{
  S_oahSubGroup
    identificationSubGroup =
      oahSubGroup::create (
        "Identification",
        "hlpi", "help-lilypond-identification",
R"(  These lilypond can be used to enforce values in the generated LilyPond code,
thus overriding the ones that may be present in the MSR data.)",
      kElementVisibilityAlways,
      this);

  appendSubGroup (identificationSubGroup);

  // MusicXML informations

  identificationSubGroup->
    appendAtom (
      oahStringAtom::create (
        "rights", "",
R"(Set the 'rights' to STRING in the LilyPond code.)",
        "STRING",
        "rights",
        fRights));

  identificationSubGroup->
    appendAtom (
      oahStringAtom::create (
        "composer", "",
R"(Set the 'composer' to STRING in the LilyPond code.)",
        "STRING",
        "composer",
        fComposer));

  identificationSubGroup->
    appendAtom (
      oahStringAtom::create (
        "arranger", "",
R"(Set the 'arranger' to STRING in the LilyPond code.)",
        "STRING",
        "arranger",
        fArranger));

  identificationSubGroup->
    appendAtom (
      oahStringAtom::create (
        "poet", "",
R"(Set the 'poet' to STRING in the LilyPond code.)",
        "STRING",
        "poet",
        fPoet));

  identificationSubGroup->
    appendAtom (
      oahStringAtom::create (
        "lyricist", "",
R"(Set the 'lyricist' to STRING in the LilyPond code.)",
        "STRING",
        "lyricist",
        fLyricist));

  identificationSubGroup->
    appendAtom (
      oahStringAtom::create (
        "software", "",
R"(Set the 'software' to STRING in the LilyPond code.)",
        "STRING",
        "software",
        fSoftware));

  // LilyPond informations

  identificationSubGroup->
    appendAtom (
      oahStringAtom::create (
        "dedication", "",
R"(Set 'dedication' to STRING in the \header.)",
        "STRING",
        "dedication",
        fDedication));

  identificationSubGroup->
    appendAtom (
      oahStringAtom::create (
        "piece", "",
R"(Set 'piece' to STRING in the \header.)",
        "STRING",
        "piece",
        fPiece));

  identificationSubGroup->
    appendAtom (
      oahStringAtom::create (
        "opus", "",
R"(Set 'opus' to STRING in the \header.)",
        "STRING",
        "opus",
        fOpus));

  identificationSubGroup->
    appendAtom (
      oahStringAtom::create (
        "title", "",
R"(Set 'title' to STRING in the \header.)",
        "STRING",
        "title",
        fTitle));

  identificationSubGroup->
    appendAtom (
      oahStringAtom::create (
        "stitle", "subtitle",
R"(Set 'subtitle' to STRING in the \header.)",
        "STRING",
        "subTitle",
        fSubTitle));

  identificationSubGroup->
    appendAtom (
      oahStringAtom::create (
        "sstitle", "subsubtitle",
R"(Set 'subsubtitle' to STRING in the \header.)",
        "STRING",
        "subSubTitle",
        fSubSubTitle));

  identificationSubGroup->
    appendAtom (
      oahStringAtom::create (
        "instrument", "",
R"(Set 'instrument' to STRING in the \header.)",
        "STRING",
        "instrument",
        fInstrument));

  identificationSubGroup->
    appendAtom (
      oahStringAtom::create (
        "meter", "",
R"(Set 'meter' to STRING in the \header.)",
        "STRING",
        "meter",
        fMeter));

  identificationSubGroup->
    appendAtom (
      oahStringAtom::create (
        "tagline", "",
R"(Set 'tagline' to STRING in the \header.)",
        "STRING",
        "tagline",
        fTagline));

  identificationSubGroup->
    appendAtom (
      oahStringAtom::create (
        "copyright", "",
R"(Set 'copyright' to STRING in the \header.)",
        "STRING",
        "copyright",
        fCopyright));
}

void lilypondOah::initializeEngraversOptions (
  bool boolOptionsInitialValue)
{
  S_oahSubGroup
    engraversSubGroup =
      oahSubGroup::create (
        "Engravers",
        "hlpe", "help-lilypond-engravers",
R"()",
      kElementVisibilityAlways,
      this);

  appendSubGroup (engraversSubGroup);

  // ambitus engraver

  fAmbitusEngraver = boolOptionsInitialValue;

  engraversSubGroup->
    appendAtom (
      oahBooleanAtom::create (
        "ambitus", "",
R"(Generate an ambitus range at the beginning of the staves/voices.)",
        "ambitusEngraver",
        fAmbitusEngraver));
}

void lilypondOah::initializeClefsKeysTimesOptions (
  bool boolOptionsInitialValue)
{
  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Clefs, keys, times",
        "hckt", "help-clefs-keys-times",
R"()",
      kElementVisibilityAlways,
      this);

  appendSubGroup (subGroup);

  // clefs

  fCommentClefChanges = boolOptionsInitialValue;

  subGroup->
    appendAtom (
      oahBooleanAtom::create (
        "ccc", "comment-clef-changes",
R"(Comment clef changes in the LilyPond code.
They won't show up in the score, but the information is not lost.)",
        "commentClefChanges",
        fCommentClefChanges));

  // keys

  // times

  fNumericalTime = boolOptionsInitialValue;

  subGroup->
    appendAtom (
      oahBooleanAtom::create (
        "numt", "numerical-time",
R"(Generate numerical time signatures, such as '4/4' instead of 'C'.)",
        "numericalTime",
        fNumericalTime));
}

void lilypondOah::initializeNotesOptions (
  bool boolOptionsInitialValue)
{
  S_oahSubGroup
    notesSubGroup =
      oahSubGroup::create (
        "Notes",
        "hlpn", "help-lilypond-notes",
R"()",
      kElementVisibilityAlways,
      this);

  appendSubGroup (notesSubGroup);

  // octave entry

  lpsrOctaveEntryKind
    octaveEntryKindDefaultValue =
      kOctaveEntryRelative; // LilyPond default value
  fOctaveEntryKind = octaveEntryKindDefaultValue;

  // leave fOctaveEntrySemiTonesPitchAndOctave equal to nullptr here,
  // since \relative without a pitch and absolute octave
  // can be used in LilyPond, in which case the pitch and absolute actave is:
  fSemiTonesPitchAndOctaveDefaultValue =
    msrSemiTonesPitchAndOctave::create (
      // F under middle C, LilyPond default for relative octave entry
      kF_Natural_STP, 3);

  notesSubGroup->
    appendAtom (
      lilypondAbsoluteOctaveEntryAtom::create (
        "abs", "absolute",
        replaceSubstringInString (
R"(Use absolute octave entry in the generated LilyPond code.
The default is to use '\relative', with LilyPond's implicit reference 'DEFAULT_VALUE'.)",
          "DEFAULT_VALUE",
          msrSemiTonesPitchAndOctaveAsLilypondString (
            gLpsrOah->fLpsrQuarterTonesPitchesLanguageKind,
            fSemiTonesPitchAndOctaveDefaultValue)),
          "lilypondAbsoluteOctaveEntryAtom"));

  notesSubGroup->
    appendAtom (
      lilypondRelativeOctaveEntryAtom::create (
        "rel", "relative",
R"(Use relative octave entry reference PITCH_AND_OCTAVE in the generated LilyPond code.
PITCH_AND_OCTAVE is made of a diatonic pitch and an optional sequence of commas or single quotes.
It should be placed between double quotes if it contains single quotes, such as:
  -rel "c''")",
        "PITCH_AND_OCTAVE",
        "relativeOctaveEntrySemiTonesPitchAndOctave",
        fRelativeOctaveEntrySemiTonesPitchAndOctave));

  notesSubGroup->
    appendAtom (
      lilypondFixedOctaveEntryAtom::create (
        "fixed", "",
R"(Use fixed octave entry reference PITCH_AND_OCTAVE in the generated LilyPond code.
PITCH_AND_OCTAVE is made of a diatonic pitch and an optional sequence of commas or single quotes.
It should be placed between double quotes if it contains single quotes, such as:
  -fixed "c''")",
        "PITCH_AND_OCTAVE",
        "fixedOctaveEntrySemiTonesPitchAndOctave",
        fFixedOctaveEntrySemiTonesPitchAndOctave));

  // durations

  fAllDurations = boolOptionsInitialValue;

  notesSubGroup->
    appendAtom (
      oahBooleanAtom::create (
        "alldurs", "all-durations",
R"(Generate all LilyPond durations.
By default, a duration equal to preceding one found in the current voice
is omitted for code conciseness.)",
        "allDurations",
        fAllDurations));

  // stems

  fStems = boolOptionsInitialValue;

  notesSubGroup->
    appendAtom (
      oahBooleanAtom::create (
        "stems", "",
R"(Generate \stemUp and \stemDown LilyPond commands.
By default, LilyPond will take care of that by itself.)",
        "stems",
        fStems));

  // beaming

  fNoAutoBeaming  = boolOptionsInitialValue;

  notesSubGroup->
    appendAtom (
      oahBooleanAtom::create (
        "noab", "no-auto-beaming",
R"(Generate '\set Voice.autoBeaming = ##f' in each voice
to prevent LilyPond from handling beams automatically.)",
        "noAutoBeaming",
        fNoAutoBeaming));

  // string numbers

  fRomanStringNumbers = boolOptionsInitialValue;

  notesSubGroup->
    appendAtom (
      oahBooleanAtom::create (
        "rsn", "roman-string-numbers",
R"(Generate '\romanStringNumbers' in each voice
for LilyPond to generate roman instead of arabic string numbers.)",
        "romanStringNumbers",
        fRomanStringNumbers));

  // open strings

  fAvoidOpenStrings    = boolOptionsInitialValue;

  notesSubGroup->
    appendAtom (
      oahBooleanAtom::create (
        "aos", "avoid-open-strings",
R"(Generate '\set TabStaff.restrainOpenStrings = ##t' in each voice
to prevent LilyPond from using open strings.)",
        "avoidOpenStrings",
        fAvoidOpenStrings));

  // accidentals

  const lpsrAccidentalStyleKind
    lpsrAccidentalStyleKindDefaultValue =
      kDefault; // default value

  fAccidentalStyleKind = lpsrAccidentalStyleKindDefaultValue;

  notesSubGroup->
    appendAtom (
      lilypondAccidentalStyleKindAtom::create (
        "as", "accidental-style", // JMI
          replaceSubstringInString (
            replaceSubstringInString (
              replaceSubstringInString (
R"(The NUMBER LilyPond accidental styles available are:
ACCIDENTAL_STYLES.
The default is 'DEFAULT_VALUE'.)",
              "NUMBER",
              to_string (gLpsrAccidentalStyleKindsMap.size ())),
            "ACCIDENTAL_STYLES",
            existingLpsrAccidentalStyleKinds ()),
          "DEFAULT_VALUE",
          lpsrAccidentalStyleKindAsString (
            lpsrAccidentalStyleKindDefaultValue)),
        "STYLE",
        "accidentalStyleKind",
        fAccidentalStyleKind));

  // rest measures

  fCompressFullMeasureRests = boolOptionsInitialValue;

  notesSubGroup->
    appendAtom (
      oahBooleanAtom::create (
        "cfmr", "compress-full-measure-rests",
R"(Compress full measure rests instead of generating successive empty measures.)",
        "compressFullMeasureRests",
        fCompressFullMeasureRests));

  // input line numbers

  fInputLineNumbers = boolOptionsInitialValue;

  notesSubGroup->
    appendAtom (
      oahBooleanAtom::create (
        "iln", "input-line-numbers",
        replaceSubstringInString (
R"(Generate after each note and barline a comment containing
its MusicXML input line number.
This is useful when debugging EXECUTABLE.)",
          "EXECUTABLE",
          gExecutableOah->fHandlerExecutableName),
        "inputLineNumbers",
        fInputLineNumbers));

  // positions in the measures

  fPositionsInMeasures = boolOptionsInitialValue;

  notesSubGroup->
    appendAtom (
      oahBooleanAtom::create (
        "pim", "positions-in-measures",
        replaceSubstringInString (
R"(Generate after each note and barline a comment containing
its position in the measure.
This is useful when debugging EXECUTABLE.)",
          "EXECUTABLE",
          gExecutableOah->fHandlerExecutableName),
        "positionsInMeasures",
        fPositionsInMeasures));
}

void lilypondOah::initializeBarsOptions (
  bool boolOptionsInitialValue)
{
  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Bar numbers",
        "hlpbn", "help-lilypond-bars-numbers",
R"()",
      kElementVisibilityAlways,
      this);

  appendSubGroup (subGroup);

  // all bar numbers

  fShowAllBarNumbers = boolOptionsInitialValue;

  S_oahBooleanAtom
    allBarNumbersAtom =
      oahBooleanAtom::create (
        "abn", "all-bar-numbers",
R"(Generate LilyPond code to show all bar numbers.)",
        "showAllBarNumbers",
        fShowAllBarNumbers);

  subGroup->
    appendAtom (allBarNumbersAtom);

  // all measure numbers

  subGroup->
    appendAtom (
      oahAtomSynonym::create (
        "amn", "all-measure-numbers",
R"(Generate LilyPond code to show all measure numbers.
This option is a synonym to '-abn, -all-bar-numbers'.)",
        allBarNumbersAtom));

  // reset measure number

  subGroup->
    appendAtom (
      lilypondResetMeasureNumberAtom::create (
        "rmn", "reset-measure-number",
R"(Generate a '\set Score.currentBarNumber = #NEW' command
at the beginning of measure OLD in the LilyPond code.
RESET_NUMBER_SPEC can be:
'OLD = NEW'
or
"OLD = NEW" .
Both OLD and NEW are purist (integer) measure numbers, i.e. those displayed by LilyPond.
There can be several occurrences of this option.)",
        "RESET_NUMBER_SPEC",
        "resetMeasureNumberMap",
        fResetMeasureNumberMap));
}

void lilypondOah::initializeLineBreaksOptions (
  bool boolOptionsInitialValue)
{
  S_oahSubGroup
    lineBreaksSubGroup =
      oahSubGroup::create (
        "Line breaks",
        "hlplb", "help-lilypond-line-breaks",
R"()",
      kElementVisibilityAlways,
      this);

  appendSubGroup (lineBreaksSubGroup);

  // lines

  fIgnoreLineBreaks = boolOptionsInitialValue;

  lineBreaksSubGroup->
    appendAtom (
      oahBooleanAtom::create (
        "ilb", "ignore-line-breaks",
R"(Ignore the line breaks from the MusicXML input
and let LilyPond decide about them.)",
        "ignoreLineBreaks",
        fIgnoreLineBreaks));

  fBreakLinesAtIncompleteRightMeasures = boolOptionsInitialValue;

  lineBreaksSubGroup->
    appendAtom (
      oahBooleanAtom::create (
        "blairm", "break-lines-at-incomplete-right-measures",
R"(Generate a '\break' command at the end of incomplete right measures
which is handy in popular folk dances and tunes.)",
        "breakLinesAtIncompleteRightMeasures",
        fBreakLinesAtIncompleteRightMeasures));

  fSeparatorLineEveryNMeasures         = -1;

  lineBreaksSubGroup->
    appendAtom (
      oahIntegerAtom::create (
        "slenm", "separator-line-every-n-measures",
R"(Generate an additional separator line for readability every N measures,
where N is a positive integer.
Nothing special is done by default.)",
        "N",
        "separatorLineEveryNMeasures",
        fSeparatorLineEveryNMeasures));
}

void lilypondOah::initializePageBreaksOptions (
  bool boolOptionsInitialValue)
{
  S_oahSubGroup
    pageBreaksSubGroup =
      oahSubGroup::create (
        "Page breaks",
        "hlppb", "help-lilypond-page-breaks",
R"()",
      kElementVisibilityAlways,
      this);

  appendSubGroup (pageBreaksSubGroup);

  // pages

  fIgnorePageBreaks = boolOptionsInitialValue;

  pageBreaksSubGroup->
    appendAtom (
      oahBooleanAtom::create (
        "ipb", "ignore-page-breaks",
R"(Ignore the page breaks from the MusicXML input
and let LilyPond decide about them.)",
        "ignorePageBreaks",
        fIgnorePageBreaks));
}

void lilypondOah::initializeStavesOptions (
  bool boolOptionsInitialValue)
{
  S_oahSubGroup
    stavesSubGroup =
      oahSubGroup::create (
        "Staves",
        "hlps", "helpLilypondStaves",
R"()",
      kElementVisibilityAlways,
      this);

  appendSubGroup (stavesSubGroup);

  // tabs

  fModernTab = boolOptionsInitialValue;

  stavesSubGroup->
    appendAtom (
      oahBooleanAtom::create (
        "mtab", "modernTab",
R"(Generate '\moderntab' instead of the default '\tab'.)",
        "modernTab",
        fModernTab));

  // staves

  fKeepStaffSize = boolOptionsInitialValue;

  stavesSubGroup->
    appendAtom (
      oahBooleanAtom::create (
        "kss", "keep-staff-size",
R"(Use the staff size value found in the MusicXML data.)",
        "keepStaffSize",
        fKeepStaffSize));
}

void lilypondOah::initializeChordsOptions (
  bool boolOptionsInitialValue)
{
  S_oahSubGroup
    chordsSubGroup =
      oahSubGroup::create (
        "Chords",
        "hlpc", "help-lilypond-chordss",
R"()",
      kElementVisibilityAlways,
      this);

  appendSubGroup (chordsSubGroup);

  // arpeggios

  fConnectArpeggios = boolOptionsInitialValue;

  chordsSubGroup->
    appendAtom (
      oahBooleanAtom::create (
        "conarp", "connect-arpeggios",
R"(Connect arpeggios across piano staves.)",
        "connectArpeggios",
        fConnectArpeggios));
}

void lilypondOah::initializeTupletsOptions (
  bool boolOptionsInitialValue)
{
  S_oahSubGroup
    tupletsSubGroup =
      oahSubGroup::create (
        "Tuplets",
        "hlpt", "help-lilypond-tuplets",
R"()",
      kElementVisibilityAlways,
      this);

  appendSubGroup (tupletsSubGroup);

  // tuplets

  fIndentTuplets = boolOptionsInitialValue;

  tupletsSubGroup->
    appendAtom (
      oahBooleanAtom::create (
        "itups", "indent-tuplets",
R"(Indent tuplets notes on a line of their own,
instead of keeping the on one and the same line.)",
        "indentTuplets",
        fIndentTuplets));
}

void lilypondOah::initializeRepeatOptions (
  bool boolOptionsInitialValue)
{
  S_oahSubGroup
    repeatsSubGroup =
      oahSubGroup::create (
        "Repeats",
        "hlpr", "help-lilypond-repeats",
R"()",
      kElementVisibilityAlways,
      this);

  appendSubGroup (repeatsSubGroup);

  // repeats

  fKeepRepeatBarlines = boolOptionsInitialValue;

  repeatsSubGroup->
    appendAtom (
      oahBooleanAtom::create (
        "krbs", "keep-repeat-barlines",
R"(Generate repeats start and and bar lines even though LilyPond would take care of them.)",
        "keepRepeatBarlines",
        fKeepRepeatBarlines));

  fRepeatBrackets = boolOptionsInitialValue;

  repeatsSubGroup->
    appendAtom (
      oahBooleanAtom::create (
        "rbracks", "repeat-brackets",
R"(Generate repeats with brackets instead of regular bar lines.)",
        "repeatBrackets",
        fRepeatBrackets));

  fIgnoreRepeatNumbers = boolOptionsInitialValue;

  repeatsSubGroup->
    appendAtom (
      oahBooleanAtom::create (
        "irn", "ignore-repeat-numbers",
R"(Ignore repeats numbers and let LilyPond determine them.)",
        "ignoreRepeatNumbers",
        fIgnoreRepeatNumbers));
}

void lilypondOah::initializeOrnamentsOptions (
  bool boolOptionsInitialValue)
{
  S_oahSubGroup
    ornamentsSubGroup =
      oahSubGroup::create (
        "Ornaments",
        "hlporns", "help-lilypond-ornaments",
R"()",
      kElementVisibilityAlways,
      this);

  appendSubGroup (ornamentsSubGroup);

  // ornaments

  fDelayedOrnamentsFraction = rational (1, 2);

  string delayedOrnamentsFractionDefaultValue =
    to_string (fDelayedOrnamentsFraction.getNumerator ()) +
      "/" +
    to_string (fDelayedOrnamentsFraction.getDenominator ());

  ornamentsSubGroup->
    appendAtom (
      oahRationalAtom::create (
        "dof", "delayed-ornaments-fraction",
        replaceSubstringInString (
R"(Place the delayed turn/reverseturn at the given fraction
between the ornemented note and the next one.
The default is 'DEFAULT_VALUE'.)",
          "DEFAULT_VALUE",
          delayedOrnamentsFractionDefaultValue),
        "NUM/DENOM",
        "delayedOrnamentsFraction",
        fDelayedOrnamentsFraction));
}

void lilypondOah::initializeChordsDisplayOptions (
  bool boolOptionsInitialValue)
{
  S_oahSubGroup
    chordsDiaplaySubGroup =
      oahSubGroup::create (
        "Chords display",
        "hchd", "help-chords-display",
R"()",
      kElementVisibilityAlways,
      this);

  appendSubGroup (chordsDiaplaySubGroup);

  // chords

  chordsDiaplaySubGroup->
    appendAtom (
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
The LilyPond code has to escape backslashed, thus use '\\' to obtain '\'.
These two elements are passed over to LilyPond verbatim, without any check.
This option can be used any number of times.)###",
        "SPECIFICATION",
        "chordsDisplayList",
        fChordsDisplayList));

  fJazzChordsDisplay = boolOptionsInitialValue;

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

  chordsDiaplaySubGroup->
    appendAtom (
      oahBooleanAtom::create (
        "jchd", "jazz-chords-display",
        replaceSubstringInString (
R"(Display the chords using LilyPond's chordNameExceptions
and a set of standard specifications to display them, i.e.:
LILYPOND_CODE)",
          "LILYPOND_CODE",
          fJazzChordsDisplayLilypondcode),
        "jazzChordsDisplay",
        fJazzChordsDisplay));
}

void lilypondOah::initializeFontsOptions (
  bool boolOptionsInitialValue)
{
  S_oahSubGroup
    fontsSubGroup =
      oahSubGroup::create (
        "Fonts",
        "hfonts", "help-fonts",
R"()",
      kElementVisibilityAlways,
      this);

  appendSubGroup (fontsSubGroup);

  // fonts

  fJazzFonts = boolOptionsInitialValue;

  fontsSubGroup->
    appendAtom (
      oahBooleanAtom::create (
        "jazz", "jazz-fonts",
R"(Use the LilyJazz font for notes, braces, text and chords.
It can be downloaded from https://github.com/OpenLilyPondFonts/lilyjazz.
This font should be installed so that LilyPond can use it, see:
https://github.com/OpenLilyPondFonts/lilyjazz/blob/master/LilyPond-Fonts-Installation-And-Usage.txt.)",
        "jazzFonts",
        fJazzFonts));
}

void lilypondOah::initializeCodeGenerationOptions (
  bool boolOptionsInitialValue)
{
  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Code generation",
        "hlpcg", "help-lilypond-code-generation",
R"()",
      kElementVisibilityAlways,
      this);

  appendSubGroup (subGroup);

  // infos

  fXml2lyInfos = boolOptionsInitialValue;

  subGroup->
    appendAtom (
      oahBooleanAtom::create (
        "xi", "xml2ly-infos",
R"(Generate initial comments showing the compilation date and lilypond.)",
        "xml2lyInfos",
        fXml2lyInfos));

  // code generation

  fComments = boolOptionsInitialValue;

  subGroup->
    appendAtom (
      oahBooleanAtom::create (
        "com", "comments",
R"(Generate comments showing the structure of the score
such as '% part P_POne (P1).)",
        "comments",
        fComments));

  fGlobal = boolOptionsInitialValue;

  subGroup->
    appendAtom (
      oahBooleanAtom::create (
        "global", "",
R"(Generate a 'global' empty variable and place a use of it
at the beginning of all voices.)",
        "global",
        fGlobal));

  fDisplayMusic = boolOptionsInitialValue;

  subGroup->
    appendAtom (
      oahBooleanAtom::create (
        "dm", "display-music",
R"(Place the contents of all voices inside a '\displayMusic' block,
for LilyPond to show its internal representation of the music.)",
        "displayMusic",
        fDisplayMusic));

  fNoLilypondCode = boolOptionsInitialValue;

  subGroup->
    appendAtom (
      oahBooleanAtom::create (
        "nolpc", "no-lilypond-code",
R"(Don't generate any LilyPond code.
That can be useful if only a summary of the score is needed.)",
        "noLilypondCode",
        fNoLilypondCode));

  fNoLilypondLyrics = boolOptionsInitialValue;

  subGroup->
    appendAtom (
      oahBooleanAtom::create (
        "nolpl", "no-lilypond-lyrics",
R"(Don't generate any lyrics in the LilyPond code.)",
        "noLilypondLyrics",
        fNoLilypondLyrics));

  fLilypondCompileDate = boolOptionsInitialValue;

  subGroup->
    appendAtom (
      oahBooleanAtom::create (
        "lpcd", "lilypond-compile-date",
R"(Generate code to include the compilation date
when LilyPond creates the score.)",
        "lilypondCompileDate",
        fLilypondCompileDate));

  fDraftMode = boolOptionsInitialValue;

  S_oahBooleanAtom
    draftOahBooleanAtom =
      oahBooleanAtom::create (
        "draft", "draft-mode",
R"(Generate a difficult to use score
to get only an overview in the resulting PDF file.)",
        "draftMode",
        fDraftMode);
  draftOahBooleanAtom->
    setIsHidden ();

  subGroup->
    appendAtom (
      draftOahBooleanAtom);

  fPointAndClickOff = boolOptionsInitialValue;

  subGroup->
    appendAtom (
      oahBooleanAtom::create (
        "paco", "point-and-clic-off",
R"(Generate \pointAndClickOff at the beginning of the LilyPond code
to reduce the size of the resulting PDF file.)",
        "pointAndClickOff",
        fPointAndClickOff));

  fWhiteNoteHeads = boolOptionsInitialValue;

  subGroup->
    appendAtom (
      oahBooleanAtom::create (
        "wnh", "white-note-heads",
R"(Generate Scheme function 'whiteNoteHeads'
at the beginning of the LilyPond code.)",
        "whiteNoteHeads",
        fWhiteNoteHeads));
}

void lilypondOah::initializeScoreNotationOptions (
  bool boolOptionsInitialValue)
{
  S_oahSubGroup
    scoreNotationSubGroup =
      oahSubGroup::create (
        "Score notation",
        "hlpsn", "help-lilypond-score-notation",
R"()",
      kElementVisibilityAlways,
      this);

  appendSubGroup (scoreNotationSubGroup);

  // score notation

//  fScoreNotationKind =  lilypondOah::kWesternNotation;
    // default value

  fJianpu = false;

  scoreNotationSubGroup->
    appendAtom (
      oahBooleanAtom::create (
        "jianpu", "",
R"(Generate the score using jianpu (numbered) notation
instead of the default western notation.
That option needs lilypond-Jianpu to be accessible to LilyPond
(https://github.com/nybbs2003/lilypond-Jianpu/jianpu10a.ly).)",
        "jianpu",
        fJianpu));
}

void lilypondOah::initializeMidiOptions (
  bool boolOptionsInitialValue)
{
  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Midi",
        "hlpm", "help-lilypond-midi",
R"()",
      kElementVisibilityAlways,
      this);

  appendSubGroup (subGroup);

  // midiTempo

  string midiTempoDuration  = "4";
  int    midiTempoPerSecond = 90;

  fMidiTempo.first  = midiTempoDuration;
  fMidiTempo.second = midiTempoPerSecond;

  string midiTempoDefaultValue =
    midiTempoDuration + " = " + to_string (midiTempoPerSecond);

  subGroup->
    appendAtom (
      lilypondMidiTempoAtom::create (
        "midi-tempo", "",
        replaceSubstringInString (
          replaceSubstringInString (
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
            "EXECUTABLE",
            gExecutableOah->fHandlerExecutableName),
          "DEFAULT_VALUE",
          midiTempoDefaultValue),
        "MIDI_TEMPO_SPEC",
        "midiTempo",
        fMidiTempo));

  // noMidi

  fNoMidi = boolOptionsInitialValue;

  subGroup->
    appendAtom (
      oahBooleanAtom::create (
        "no-midi", "",
R"(Generate the '\midi' block as a comment instead of active code.)",
        "noMidi",
        fNoMidi));
}

void lilypondOah::initializeLilypondOah (
  bool boolOptionsInitialValue)
{
  // identification
  // --------------------------------------
  initializeIdentificationOptions (
    boolOptionsInitialValue);

  // engravers
  // --------------------------------------
  initializeEngraversOptions (
    boolOptionsInitialValue);

  // time
  // --------------------------------------
  initializeClefsKeysTimesOptions (
    boolOptionsInitialValue);

  // notes
  // --------------------------------------
  initializeNotesOptions (
    boolOptionsInitialValue);

  // bars
  // --------------------------------------
  initializeBarsOptions (
    boolOptionsInitialValue);

  // line breaks
  // --------------------------------------
  initializeLineBreaksOptions (
    boolOptionsInitialValue);

  // page breaks
  // --------------------------------------
  initializePageBreaksOptions (
    boolOptionsInitialValue);

  // staves
  // --------------------------------------
  initializeStavesOptions (
    boolOptionsInitialValue);

  // chords
  // --------------------------------------
  initializeChordsOptions (
    boolOptionsInitialValue);

  // tuplets
  // --------------------------------------
  initializeTupletsOptions (
    boolOptionsInitialValue);

  // repeats
  // --------------------------------------
  initializeRepeatOptions (
    boolOptionsInitialValue);

  // ornaments
  // --------------------------------------
  initializeOrnamentsOptions (
    boolOptionsInitialValue);

  // chords display
  // --------------------------------------
  initializeChordsDisplayOptions (
    boolOptionsInitialValue);

  // fonts
  // --------------------------------------
  initializeFontsOptions (
    boolOptionsInitialValue);

  // code generation
  // --------------------------------------
  initializeCodeGenerationOptions (
    boolOptionsInitialValue);

  // score notation
  // --------------------------------------
  initializeScoreNotationOptions (
    boolOptionsInitialValue);

  // midi
  // --------------------------------------
  initializeMidiOptions (
    boolOptionsInitialValue);
}

S_lilypondOah lilypondOah::createCloneWithDetailedTrace ()
{
  S_lilypondOah
    clone =
      lilypondOah::create (0);
      // 0 not to have it inserted twice in the option handler

  // set the lilypond handler upLink
  clone->fHandlerUpLink =
    fHandlerUpLink;

  // identification
  // --------------------------------------

  // MusicXML informations
  clone->fRights =
    fRights;
  clone->fComposer =
    fComposer;
  clone->fArranger =
    fArranger;
  clone->fPoet =
    fPoet;
  clone->fLyricist =
    fLyricist;
  clone->fSoftware =
    fSoftware;

  // LilyPond informations
  clone->fDedication =
    fDedication;
  clone->fPiece =
    fPiece;
  clone->fOpus =
    fOpus;
  clone->fTitle =
    fTitle;
  clone->fSubTitle =
    fSubTitle;
  clone->fSubSubTitle =
    fSubSubTitle;
  clone->fInstrument =
    fInstrument;
  clone->fMeter =
    fMeter;
  clone->fTagline =
    fTagline;
  clone->fCopyright =
    fCopyright;


  // engravers
  // --------------------------------------

  clone->fAmbitusEngraver =
    fAmbitusEngraver;


  // clefs
  // --------------------------------------

  clone->fCommentClefChanges =
    fCommentClefChanges;


  // times
  // --------------------------------------

  clone->fNumericalTime =
    fNumericalTime;


  // notes
  // --------------------------------------

  clone->fOctaveEntryKind =
    fOctaveEntryKind;

  clone->fAllDurations =
    fAllDurations;

  clone->fStems =
    fStems;
  clone->fNoAutoBeaming =
    fNoAutoBeaming;

  clone->fRomanStringNumbers =
    fRomanStringNumbers;
  clone->fAvoidOpenStrings =
    fAvoidOpenStrings;

  clone->fAccidentalStyleKind =
    fAccidentalStyleKind;

  clone->fCompressFullMeasureRests =
    fCompressFullMeasureRests;

  clone->fInputLineNumbers =
    true;
  clone->fPositionsInMeasures =
    true;


  // bars
  // --------------------------------------

  clone->fShowAllBarNumbers =
    true;


  // line breaks
  // --------------------------------------

  clone->fIgnoreLineBreaks =
    fIgnoreLineBreaks;

  clone->fBreakLinesAtIncompleteRightMeasures =
    fBreakLinesAtIncompleteRightMeasures;

  clone->fSeparatorLineEveryNMeasures =
    fSeparatorLineEveryNMeasures;


  // page breaks
  // --------------------------------------

  clone->fIgnorePageBreaks =
    fIgnorePageBreaks;


  // staves
  // --------------------------------------

  clone->fModernTab =
    fModernTab;


  // chords
  // --------------------------------------

  clone->fConnectArpeggios =
    fConnectArpeggios;


  // tuplets
  // --------------------------------------

  clone->fIndentTuplets =
    fIndentTuplets;


  // repeats
  // --------------------------------------

  clone->fKeepRepeatBarlines =
    fKeepRepeatBarlines;
  clone->fRepeatBrackets =
    fRepeatBrackets;
  clone->fIgnoreRepeatNumbers =
    fIgnoreRepeatNumbers;


  // ornaments
  // --------------------------------------

  clone->fDelayedOrnamentsFraction =
    fDelayedOrnamentsFraction;


  // chords display
  // --------------------------------------

  clone->fJazzChordsDisplay =
    fJazzChordsDisplay;
  clone->fJazzChordsDisplayLilypondcode =
    fJazzChordsDisplayLilypondcode;


  // fonts
  // --------------------------------------

  clone->fJazzFonts =
    fJazzFonts;


  // code generation
  // --------------------------------------

  clone->fXml2lyInfos =
    fXml2lyInfos;

  clone->fComments =
    fComments;

  clone->fGlobal =
    fGlobal;

  clone->fDisplayMusic =
    fDisplayMusic;

  clone->fNoLilypondCode =
    fNoLilypondCode;

  clone->fNoLilypondLyrics =
    fNoLilypondLyrics;

  clone->fLilypondCompileDate =
    fLilypondCompileDate;

  clone->fDraftMode =
    fPointAndClickOff;

  clone->fPointAndClickOff =
    fPointAndClickOff;

  clone->fWhiteNoteHeads =
    fWhiteNoteHeads;


  // score notation
  // --------------------------------------

  clone->fJianpu =
    fJianpu;


  // midi
  // --------------------------------------

  clone->fMidiTempo =
    fMidiTempo;

  clone->fNoMidi =
    fNoMidi;

// JMI ???

  clone->fKeepStaffSize =
    fKeepStaffSize;

  return clone;
}

/* JMI
bool lilypondOah::setAccidentalStyleKind (lpsrAccidentalStyleKind accidentalStyleKind)
{
  // is accidentalStyleKind in the accidental styles set?
  map<string, lpsrAccidentalStyleKind>::const_iterator
    it =
      gLpsrAccidentalStyleKindsMap.find (accidentalStyleKind);

  if (it == gLpsrAccidentalStyleKindsMap.end ()) {
    // no, accidentalStyleKind is unknown
    return false;
  }

  fAccidentalStyleKind = it.second;

  return true;
}
*/

//______________________________________________________________________________
void lilypondOah::enforceQuietness ()
{}

//______________________________________________________________________________
void lilypondOah::checkOptionsConsistency ()
{
  // JMI
}

//______________________________________________________________________________
string lilypondOah::scoreNotationKindAsString (
  scoreNotationKind notationKind)
{
  string result;

  switch (notationKind) {
    case lilypondOah::kWesternNotation:
      result = "western";
      break;
    case lilypondOah::kJianpuNotation:
      result = "jianpu";
      break;
    case lilypondOah::kABCNotation:
      result = "abc";
      break;
  } // switch

  return result;
}

void lilypondOah::printAtomOptionsValues (
  ostream& os,
  int      valueFieldWidth) const
{
  os <<
    "The LilyPond lilypond are:" <<
    endl;

  gIndenter++;

  // identification
  // --------------------------------------
  os <<
    "Identification:" <<
    endl;

  gIndenter++;

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

  gIndenter--;


  // engravers
  // --------------------------------------
  os <<
    "Engravers:" <<
    endl;

  gIndenter++;

  os << left <<
    setw (valueFieldWidth) << "ambitusEngraver" << " : " <<
      booleanAsString (fAmbitusEngraver) <<
      endl;

  gIndenter--;


  // clefs
  // --------------------------------------
  os <<
    "Clefs:" <<
    endl;

  gIndenter++;

  os << left <<
    setw (valueFieldWidth) << "commentClefChanges" << " : " <<
      booleanAsString (fCommentClefChanges) <<
      endl;

  gIndenter--;


  // times
  // --------------------------------------
  os <<
    "Times:" <<
    endl;

  gIndenter++;

  os << left <<
    setw (valueFieldWidth) << "numericalTime" << " : " <<
      booleanAsString (fNumericalTime) <<
      endl;

  gIndenter--;


  // notes
  // --------------------------------------
  os <<
    "Notes:" <<
    endl;

  gIndenter++;

  os << left <<
    setw (valueFieldWidth) << "octaveEntryKind" << " : " <<
    lpsrOctaveEntryKindAsString (fOctaveEntryKind) <<
    endl;

  switch (gLilypondOah->fOctaveEntryKind) {
    case kOctaveEntryRelative:
      if (fRelativeOctaveEntrySemiTonesPitchAndOctave) {
        os << left <<
          setw (valueFieldWidth) <<
          "relativeOctaveEntrySemiTonesPitchAndOctave" << " : " <<
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
          "fixedOctaveEntrySemiTonesPitchAndOctave" << " : " <<
          fFixedOctaveEntrySemiTonesPitchAndOctave->asString () <<
          endl;
      }
      break;
  } // switch

  os << left <<
    setw (valueFieldWidth) << "allDurations" << " : " <<
      booleanAsString (fAllDurations) <<
      endl <<

    setw (valueFieldWidth) << "stems" << " : " <<
      booleanAsString (fStems) <<
      endl <<
    setw (valueFieldWidth) << "noAutoBeaming" << " : " <<
      booleanAsString (fNoAutoBeaming) <<
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

    setw (valueFieldWidth) << "compressFullMeasureRests" << " : " <<
      booleanAsString (fCompressFullMeasureRests) <<
      endl <<

    setw (valueFieldWidth) << "inputLineNumbers" << " : " <<
      booleanAsString (fInputLineNumbers) <<
      endl <<

    setw (valueFieldWidth) << "positionsInMeasures" << " : " <<
      booleanAsString (fPositionsInMeasures) <<
      endl;

  gIndenter--;


  // bars
  // --------------------------------------
  os <<
    "Bars:" <<
    endl;

  gIndenter++;

  os << left <<
    setw (valueFieldWidth) << "showAllBarNumbers" << " : " <<
    booleanAsString (fShowAllBarNumbers) <<
    endl;

  gIndenter--;


  // line breaks
  // --------------------------------------
  os <<
    "Line breaks:" <<
    endl;

  gIndenter++;

  os << left <<
    setw (valueFieldWidth) << "ignoreLineBreaks" << " : " <<
      booleanAsString (fIgnoreLineBreaks) <<
      endl <<

    setw (valueFieldWidth) << "breakLinesAtIncompleteRightMeasures" << " : " <<
      booleanAsString (fBreakLinesAtIncompleteRightMeasures) <<
      endl <<

    setw (valueFieldWidth) << "separatorLineEveryNMeasures" << " : " <<
      booleanAsString (fSeparatorLineEveryNMeasures) <<
      endl;

  gIndenter--;


  // page breaks
  // --------------------------------------
  os <<
    "Page breaks:" <<
    endl;

  gIndenter++;

  os << left <<
    setw (valueFieldWidth) << "ignorePageBreaks" << " : " <<
    booleanAsString (fIgnorePageBreaks) <<
    endl;

  gIndenter--;


  // staves
  // --------------------------------------
  os <<
    "Staves:" <<
    endl;

  gIndenter++;

  os << left <<
    setw (valueFieldWidth) << "modernTab" << " : " <<
    booleanAsString (fModernTab) <<
    endl <<
    setw (valueFieldWidth) << "keepStaffSize" << " : " <<
    booleanAsString (fKeepStaffSize) <<
    endl;

  gIndenter--;


  // chords
  // --------------------------------------

  os <<
    "Chords:" <<
    endl;

  gIndenter++;

  os << left <<
    setw (valueFieldWidth) << "connectArpeggios" << " : " <<
    booleanAsString (fConnectArpeggios) <<
    endl;

  gIndenter--;


  // tuplets
  // --------------------------------------

  os <<
    "Tuplets:" <<
    endl;

  gIndenter++;

  os << left <<
    setw (valueFieldWidth) << "indentTuplets" << " : " <<
    booleanAsString (fIndentTuplets) <<
    endl;

  gIndenter--;


  // repeats
  // --------------------------------------

  os <<
    "Repeats:" <<
    endl;

  gIndenter++;

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

  gIndenter--;


  // ornaments
  // --------------------------------------

  os <<
    "Ornaments:" <<
    endl;

  gIndenter++;

  os << left <<
    setw (valueFieldWidth) << "fDelayedOrnamentsFraction" << " : " <<
    fDelayedOrnamentsFraction <<
    endl;

  gIndenter--;


  // chords display
  // --------------------------------------

  os <<
    "Chords display:" <<
    endl;

  gIndenter++;

  os << left <<
    setw (valueFieldWidth) << "chordsDisplayList" << " : ";

  if (fChordsDisplayList.size ()) {
    os << endl;

    gIndenter++;

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

    gIndenter--;
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

  gIndenter--;


  // fonts
  // --------------------------------------

  os <<
    "Fonts:" <<
    endl;

  gIndenter++;

  os << left <<
    setw (valueFieldWidth) << "jazzFonts" << " : " <<
    booleanAsString (fJazzFonts) <<
    endl;

  gIndenter--;


  // code generation
  // --------------------------------------

  os <<
    "LilyPond code generation:" <<
    endl;

  gIndenter++;

  os << left <<
    setw (valueFieldWidth) << "xml2lyInfos" << " : " <<
      booleanAsString (fXml2lyInfos) <<
      endl <<

    setw (valueFieldWidth) << "comments" << " : " <<
      booleanAsString (fComments) <<
      endl <<

    setw (valueFieldWidth) << "global" << " : " <<
      booleanAsString (fGlobal) <<
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

  gIndenter--;


  // score notation
  // --------------------------------------

  os <<
    "Score notation:" <<
    endl;

  gIndenter++;

  os << left <<
    setw (valueFieldWidth) << "fJianpu" << " : " <<
      booleanAsString (fJianpu) <<
      endl;

  gIndenter--;


  // midi
  // --------------------------------------
  os <<
    "Midi:" <<
    endl;

  gIndenter++;

  os << left <<
    setw (valueFieldWidth) << "midiTempo" << " : " <<
      fMidiTempo.first << " = " << fMidiTempo.second <<
      endl <<

    setw (valueFieldWidth) << "noMidi" << " : " <<
      booleanAsString (fNoMidi) <<
      endl;

  gIndenter--;
}

void lilypondOah::printLilypondOahValues (int fieldWidth)
{
  gLogOstream <<
    "The LilyPond lilypond are:" <<
    endl;

  gIndenter++;

  // identification
  // --------------------------------------
  gLogOstream <<
    "Identification:" <<
    endl;

  gIndenter++;

  // MusicXML informations
  gLogOstream << left <<
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

  gIndenter--;


  // engravers
  // --------------------------------------
  gLogOstream <<
    "Engravers:" <<
    endl;

  gIndenter++;

  gLogOstream << left <<
    setw (fieldWidth) << "ambitusEngraver" << " : " <<
      booleanAsString (fAmbitusEngraver) <<
      endl;

  gIndenter--;


  // clefs
  // --------------------------------------
  gLogOstream <<
    "Clefs:" <<
    endl;

  gIndenter++;

  gLogOstream << left <<
    setw (fieldWidth) << "commentClefChanges" << " : " <<
      booleanAsString (fCommentClefChanges) <<
      endl;

  gIndenter--;


  // time
  // --------------------------------------
  gLogOstream <<
    "Times:" <<
    endl;

  gIndenter++;

  gLogOstream << left <<
    setw (fieldWidth) << "numericalTime" << " : " <<
      booleanAsString (fNumericalTime) <<
      endl;

  gIndenter--;


  // notes
  // --------------------------------------
  gLogOstream <<
    "Notes:" <<
    endl;

  gIndenter++;

  gLogOstream << left <<
    setw (fieldWidth) << "octaveEntryKind" << " : " <<
      lpsrOctaveEntryKindAsString (fOctaveEntryKind) <<
      endl <<

    setw (fieldWidth) << "allDurations" << " : " <<
      booleanAsString (fAllDurations) <<
      endl <<

    setw (fieldWidth) << "stems" << " : " <<
      booleanAsString (fStems) <<
      endl <<
    setw (fieldWidth) << "noAutoBeaming" << " : " <<
      booleanAsString (fNoAutoBeaming) <<
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

    setw (fieldWidth) << "compressFullMeasureRests" << " : " <<
      booleanAsString (fCompressFullMeasureRests) <<
      endl <<

    setw (fieldWidth) << "inputLineNumbers" << " : " <<
      booleanAsString (fInputLineNumbers) <<
      endl <<

    setw (fieldWidth) << "notesPositionsInMeasures" << " : " <<
      booleanAsString (fPositionsInMeasures) <<
      endl;

  gIndenter--;


  // bars
  // --------------------------------------
  gLogOstream <<
    "Bars:" <<
    endl;

  gIndenter++;

  gLogOstream << left <<
    setw (fieldWidth) << "showAllBarNumbers" << " : " <<
    booleanAsString (fShowAllBarNumbers) <<
    endl;

  gIndenter--;


  // line breaks
  // --------------------------------------
  gLogOstream <<
    "Line breaks:" <<
    endl;

  gIndenter++;

  gLogOstream << left <<
    setw (fieldWidth) << "ignoreLineBreaks" << " : " <<
      booleanAsString (fIgnoreLineBreaks) <<
      endl <<

    setw (fieldWidth) << "breakLinesAtIncompleteRightMeasures" << " : " <<
      booleanAsString (fBreakLinesAtIncompleteRightMeasures) <<
      endl <<

    setw (fieldWidth) << "separatorLineEveryNMeasures" << " : " <<
      booleanAsString (fSeparatorLineEveryNMeasures) <<
      endl;

  gIndenter--;


  // page breaks
  // --------------------------------------
  gLogOstream <<
    "Page breaks:" <<
    endl;

  gIndenter++;

  gLogOstream << left <<
    setw (fieldWidth) << "ignorePageBreaks" << " : " <<
    booleanAsString (fIgnorePageBreaks) <<
    endl;

  gIndenter--;


  // staves
  // --------------------------------------
  gLogOstream <<
    "Staves:" <<
    endl;

  gIndenter++;

  gLogOstream << left <<
    setw (fieldWidth) << "modernTab" << " : " <<
    booleanAsString (fModernTab) <<
    endl;

  gIndenter--;


  // chords
  // --------------------------------------

  gLogOstream <<
    "Chords:" <<
    endl;

  gIndenter++;

  gLogOstream << left <<
    setw (fieldWidth) << "connectArpeggios" << " : " <<
    booleanAsString (fConnectArpeggios) <<
    endl;

  gIndenter--;


  // tuplets
  // --------------------------------------

  gLogOstream <<
    "Tuplets:" <<
    endl;

  gIndenter++;

  gLogOstream << left <<
    setw (fieldWidth) << "indentTuplets" << " : " <<
    booleanAsString (fIndentTuplets) <<
    endl;

  gIndenter--;


  // repeats
  // --------------------------------------

  gLogOstream <<
    "Repeats:" <<
    endl;

  gIndenter++;

  gLogOstream << left <<
    setw (fieldWidth) << "keepRepeatBarlines" << " : " <<
    booleanAsString (fKeepRepeatBarlines) <<
    endl <<
    setw (fieldWidth) << "repeatBrackets" << " : " <<
    booleanAsString (fRepeatBrackets) <<
    endl <<
    setw (fieldWidth) << "ignoreRepeatNumbers" << " : " <<
    booleanAsString (fIgnoreRepeatNumbers) <<
    endl;

  gIndenter--;


  // ornaments
  // --------------------------------------

  gLogOstream <<
    "Ornaments:" <<
    endl;

  gIndenter++;

  gLogOstream << left <<
    setw (fieldWidth) << "fDelayedOrnamentsFraction" << " : " <<
    fDelayedOrnamentsFraction <<
    endl;

  gIndenter--;


  // chords display
  // --------------------------------------

  gLogOstream <<
    "Chords display:" <<
    endl;

  gIndenter++;

  gLogOstream << left <<
    setw (fieldWidth) << "jazzChordsDisplay" << " : " <<
    booleanAsString (fJazzChordsDisplay) <<
    endl <<
    setw (fieldWidth) << "jazzChordsDisplayLilypondcode" << " : " <<
    fJazzChordsDisplayLilypondcode <<
    endl;

  gIndenter--;


  // fonts
  // --------------------------------------

  gLogOstream <<
    "Fonts:" <<
    endl;

  gIndenter++;

  gLogOstream << left <<
    setw (fieldWidth) << "jazzFonts" << " : " <<
    booleanAsString (fJazzFonts) <<
    endl;

  gIndenter--;


  // code generation
  // --------------------------------------

  gLogOstream <<
    "LilyPond code generation:" <<
    endl;

  gIndenter++;

  gLogOstream << left <<
    setw (fieldWidth) << "xml2lyInfos" << " : " <<
      booleanAsString (fXml2lyInfos) <<
      endl <<

    setw (fieldWidth) << "comments" << " : " <<
      booleanAsString (fComments) <<
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
      endl;

  gIndenter--;


  // score notation
  // --------------------------------------

  gLogOstream <<
    "Score notation:" <<
    endl;

  gIndenter++;

  gLogOstream << left <<
    setw (fieldWidth) << "fJianpu" << " : " <<
      booleanAsString (fJianpu) <<
      endl;

  gIndenter--;


  // midi
  // --------------------------------------
  gLogOstream <<
    "Midi:" <<
    endl;

  gIndenter++;

  gLogOstream << left <<
    setw (fieldWidth) << "fMidiTempo" << " : " <<
      fMidiTempo.first << " = " << fMidiTempo.second <<
      endl <<

    setw (fieldWidth) << "noMidi" << " : " <<
      booleanAsString (fNoMidi) <<
      endl;

  gIndenter--;

  gIndenter--;
}

S_oahValuedAtom lilypondOah::handleAtom (
  ostream&  os,
  S_oahAtom atom)
{
  S_oahValuedAtom result;

  if (
    // absolute octave entry atom?
    S_lilypondAbsoluteOctaveEntryAtom
      absoluteOctaveEntryKindAtom =
        dynamic_cast<lilypondAbsoluteOctaveEntryAtom*>(&(*atom))
    ) {
#ifdef TRACE_OAH
    if (gExecutableOah->fTraceOah) {
      os <<
        "==> oahAtom is of type 'lilypondAbsoluteOctaveEntryAtom'" <<
        endl;
    }
#endif

    // set octave entry kind it at once
    fOctaveEntryKind = kOctaveEntryAbsolute;
  }

  else if (
    // relative octave entry atom?
    S_lilypondRelativeOctaveEntryAtom
      relativeOctaveEntryAtom =
        dynamic_cast<lilypondRelativeOctaveEntryAtom*>(&(*atom))
    ) {
#ifdef TRACE_OAH
    if (gExecutableOah->fTraceOah) {
      os <<
        "==> oahAtom is of type 'lilypondRelativeOctaveEntryAtom'" <<
        endl;
    }
#endif

    // set octave entry kind it at once
    fOctaveEntryKind = kOctaveEntryRelative;

    // wait until the value is met
    result = relativeOctaveEntryAtom;
  }

  else if (
    // fixed octave entry atom?
    S_lilypondFixedOctaveEntryAtom
      fixedOctaveEntryAtom =
        dynamic_cast<lilypondFixedOctaveEntryAtom*>(&(*atom))
    ) {
#ifdef TRACE_OAH
    if (gExecutableOah->fTraceOah) {
      os <<
        "==> oahAtom is of type 'lilypondFixedOctaveEntryAtom'" <<
        endl;
    }
#endif

    // set octave entry kind it at once
    fOctaveEntryKind = kOctaveEntryFixed;

    // wait until the value is met
    result = fixedOctaveEntryAtom;
  }

  else if (
    // reset measure number atom?
    S_lilypondResetMeasureNumberAtom
      resetMeasureNumberAtom =
        dynamic_cast<lilypondResetMeasureNumberAtom*>(&(*atom))
    ) {
#ifdef TRACE_OAH
    if (gExecutableOah->fTraceOah) {
      os <<
        "==> oahAtom is of type 'lilypondResetMeasureNumberAtom'" <<
        endl;
    }
#endif

    // wait until the value is met
    result = resetMeasureNumberAtom;
  }

  else if (
    // acccidentals style atom?
    S_lilypondAccidentalStyleKindAtom
      accidentalStyleKindAtom =
        dynamic_cast<lilypondAccidentalStyleKindAtom*>(&(*atom))
    ) {
#ifdef TRACE_OAH
    if (gExecutableOah->fTraceOah) {
      os <<
        "==> oahAtom is of type 'lilypondAccidentalStyleKindAtom'" <<
        endl;
    }
#endif

    // wait until the value is met
    result = accidentalStyleKindAtom;
  }

  else if (
    // chords display atom?
    S_lilypondChordsDisplayAtom
      chordsDisplayAtom =
        dynamic_cast<lilypondChordsDisplayAtom*>(&(*atom))
    ) {
#ifdef TRACE_OAH
    if (gExecutableOah->fTraceOah) {
      os <<
        "==> oahAtom is of type 'lilypondChordsDisplayAtom'" <<
        endl;
    }
#endif

    // wait until the value is met
    result = chordsDisplayAtom;
  }

  else if (
    // midi tempo atom?
    S_lilypondMidiTempoAtom
      midiTempoAtom =
        dynamic_cast<lilypondMidiTempoAtom*>(&(*atom))
    ) {
#ifdef TRACE_OAH
    if (gExecutableOah->fTraceOah) {
      os <<
        "==> oahAtom is of type 'lilypondMidiTempoAtom'" <<
        endl;
    }
#endif

    // wait until the value is met
    result = midiTempoAtom;
  }

  return result;
}

void lilypondOah::handleValuedAtomValue (
  ostream&  os,
  S_oahAtom atom,
  string    theString)
{
  if (
    // relative octave entry atom?
    S_lilypondRelativeOctaveEntryAtom
      relativeOctaveEntryAtom =
        dynamic_cast<lilypondRelativeOctaveEntryAtom*>(&(*atom))
  ) {
    relativeOctaveEntryAtom->handleValue (
      theString,
      os);
  }

  else if (
    // fixed octave entry atom?
    S_lilypondFixedOctaveEntryAtom
      fixedOctaveEntryAtom =
        dynamic_cast<lilypondFixedOctaveEntryAtom*>(&(*atom))
  ) {
    fixedOctaveEntryAtom->handleValue (
      theString,
      os);
  }

  else if (
    // reset measure number atom?
    S_lilypondResetMeasureNumberAtom
      resetMeasureNumberAtom =
        dynamic_cast<lilypondResetMeasureNumberAtom*>(&(*atom))
  ) {
    resetMeasureNumberAtom->handleValue (
      theString,
      os);
  }

  else if (
    // accidental style atom?
    S_lilypondAccidentalStyleKindAtom
      accidentalStyleKindAtom =
        dynamic_cast<lilypondAccidentalStyleKindAtom*>(&(*atom))
  ) {
    accidentalStyleKindAtom->handleValue (
      theString,
      os);
  }

  else if (
    // chords display atom?
    S_lilypondChordsDisplayAtom
      chordsDisplayAtom =
        dynamic_cast<lilypondChordsDisplayAtom*>(&(*atom))
  ) {
    chordsDisplayAtom->handleValue (
      theString,
      os);
  }

  else if (
    // midi tempo atom?
    S_lilypondMidiTempoAtom
      midiTempoAtom =
        dynamic_cast<lilypondMidiTempoAtom*>(&(*atom))
  ) {
    midiTempoAtom->handleValue (
      theString,
      os);
  }
}

ostream& operator<< (ostream& os, const S_lilypondOah& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
void initializeLilypondOahHandling (
  S_oahHandler handler)
{
#ifdef TRACE_OAH
  if (gExecutableOah->fTraceOah && ! gGeneralOah->fQuiet) {
    gLogOstream <<
      "Initializing LilyPond lilypond handling" <<
      endl;
  }
#endif

  // create the lilypond variables
  // ------------------------------------------------------

  gLilypondOahUserChoices = lilypondOah::create (
    handler);
  assert(gLilypondOahUserChoices != 0);

  gLilypondOah =
    gLilypondOahUserChoices;

  // prepare for measure detailed trace
  // ------------------------------------------------------

  gLilypondOahWithDetailedTrace =
    gLilypondOah->
      createCloneWithDetailedTrace ();
}


}
