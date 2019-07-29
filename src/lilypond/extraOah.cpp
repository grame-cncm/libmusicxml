/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#include "extraOah.h"

#ifdef EXTRA_OAH


#include <iomanip>      // setw, setprecision, ...

#include <regex>

#include "version.h"
#include "utilities.h"

#include "executableOah.h"
#include "generalOah.h"

#include "traceOah.h"

#include "lpsrOah.h"


using namespace std;

namespace MusicXML2
{

//______________________________________________________________________________
S_extraShowAllChordsStructuresAtom extraShowAllChordsStructuresAtom::create (
  string shortName,
  string longName,
  string description)
{
  extraShowAllChordsStructuresAtom* o = new
    extraShowAllChordsStructuresAtom (
      shortName,
      longName,
      description);
  assert(o!=0);
  return o;
}

extraShowAllChordsStructuresAtom::extraShowAllChordsStructuresAtom (
  string shortName,
  string longName,
  string description)
  : oahAtom (
      shortName,
      longName,
      description)
{}

extraShowAllChordsStructuresAtom::~extraShowAllChordsStructuresAtom ()
{}

void extraShowAllChordsStructuresAtom::print (ostream& os) const
{
  const int fieldWidth = K_OPTIONS_FIELD_WIDTH;

  os <<
    "OptionsShowAllChordsStructuresAtom:" <<
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

void extraShowAllChordsStructuresAtom::printAllChordsStructures (ostream& os) const
{
  msrChordStructure::printAllChordsStructures (os);
}

void extraShowAllChordsStructuresAtom::printOptionsValues (
  ostream& os,
  int      valueFieldWidth) const
{
  // nothing to print here
}

ostream& operator<< (ostream& os, const S_extraShowAllChordsStructuresAtom& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_extraShowAllChordsContentsAtom extraShowAllChordsContentsAtom::create (
  string  shortName,
  string  longName,
  string  description,
  string  valueSpecification,
  string  variableName,
  string& extraShowAllChordsContentsVariable)
{
  extraShowAllChordsContentsAtom* o = new
    extraShowAllChordsContentsAtom (
      shortName,
      longName,
      description,
      valueSpecification,
      variableName,
      extraShowAllChordsContentsVariable);
  assert(o!=0);
  return o;
}

extraShowAllChordsContentsAtom::extraShowAllChordsContentsAtom (
  string  shortName,
  string  longName,
  string  description,
  string  valueSpecification,
  string  variableName,
  string& extraShowAllChordsContentsVariable)
  : oahValuedAtom (
      shortName,
      longName,
      description,
      valueSpecification,
      variableName),
    fStringVariable (
      extraShowAllChordsContentsVariable)
{}

extraShowAllChordsContentsAtom::~extraShowAllChordsContentsAtom ()
{}

void extraShowAllChordsContentsAtom::handleValue (
  string   theString,
  ostream& os)
{
#ifdef TRACE_OAH
  if (gExecutableOah->fTraceOah) {
    os <<
      "==> oahAtom is of type 'extraShowAllChordsContentsAtom'" <<
      endl;
  }
#endif

  // theString contains the pitch name in the current language
  // is it in the accidental styles map?

#ifdef TRACE_OAH
  if (gExecutableOah->fTraceOah) {
    os <<
      "==> oahAtom is of type 'extraShowAllChordsContentsAtom'" <<
      ", theString = \"" << theString << "\"" <<
      endl;
  }
#endif

  // fetch the semitones pitch from theString
  msrSemiTonesPitchKind
    semiTonesPitchKind =
      semiTonesPitchKindFromString (
        theString);

  switch (semiTonesPitchKind) {
    case k_NoSemiTonesPitch_STP:
      {
        stringstream s;

        s <<
          "'" << theString <<
          "' is no diatonic (semitones) pitch" <<
          " in pitch language '" <<
          msrQuarterTonesPitchesLanguageKindAsString (
            gLpsrOah->
              fLpsrQuarterTonesPitchesLanguageKind) <<
          "'" <<
          endl;

        optionError (s.str ());

        exit (4);
      }
      break;

    case kA_Flat_STP:
    case kA_Natural_STP:
    case kA_Sharp_STP:

    case kB_Flat_STP:
    case kB_Natural_STP:
    case kB_Sharp_STP:

    case kC_Flat_STP:
    case kC_Natural_STP:
    case kC_Sharp_STP:

    case kD_Flat_STP:
    case kD_Natural_STP:
    case kD_Sharp_STP:

    case kE_Flat_STP:
    case kE_Natural_STP:
    case kE_Sharp_STP:

    case kF_Flat_STP:
    case kF_Natural_STP:
    case kF_Sharp_STP:

    case kG_Flat_STP:
    case kG_Natural_STP:
    case kG_Sharp_STP:
      break;

    default:
      {
        stringstream s;

        s <<
          "'" << theString <<
          "' is no diatonic (semitones) pitch" <<
          endl;

        optionError (s.str ());

        exit (4);
      }
  } // switch

  // print all the chords notes
  printAllChordsContents (
    os,
    semiTonesPitchKind);

  // exit
  exit (23);
}

void extraShowAllChordsContentsAtom::print (ostream& os) const
{
  const int fieldWidth = K_OPTIONS_FIELD_WIDTH;

  os <<
    "extraShowAllChordsContentsAtom:" <<
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

void extraShowAllChordsContentsAtom::printAllChordsContents (
  ostream&              os,
  msrSemiTonesPitchKind semiTonesPitchKind) const
{
  msrChordContents::printAllChordsContents (
    os,
    semiTonesPitchKind);
}

void extraShowAllChordsContentsAtom::printOptionsValues (
  ostream& os,
  int      valueFieldWidth) const
{
  // nothing to print here
}

ostream& operator<< (ostream& os, const S_extraShowAllChordsContentsAtom& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_extraShowChordDetailsAtom extraShowChordDetailsAtom::create (
  string  shortName,
  string  longName,
  string  description,
  string  valueSpecification,
  string  variableName,
  string& extraShowChordDetailsVariable)
{
  extraShowChordDetailsAtom* o = new
    extraShowChordDetailsAtom (
      shortName,
      longName,
      description,
      valueSpecification,
      variableName,
      extraShowChordDetailsVariable);
  assert(o!=0);
  return o;
}

extraShowChordDetailsAtom::extraShowChordDetailsAtom (
  string  shortName,
  string  longName,
  string  description,
  string  valueSpecification,
  string  variableName,
  string& extraShowChordDetailsVariable)
  : oahValuedAtom (
      shortName,
      longName,
      description,
      valueSpecification,
      variableName),
    fStringVariable (
      extraShowChordDetailsVariable)
{}

extraShowChordDetailsAtom::~extraShowChordDetailsAtom ()
{}

void extraShowChordDetailsAtom::handleValue (
  string   theString,
  ostream& os)
{
#ifdef TRACE_OAH
  if (gExecutableOah->fTraceOah) {
    os <<
      "==> oahAtom is of type 'extraShowChordDetailsAtom'" <<
      endl;
  }
#endif

  // theString contains the pitch name in the current language
  // is it in the accidental styles map?
#ifdef TRACE_OAH
  if (gExecutableOah->fTraceOah) {
    os <<
      "==> oahAtom is of type 'extraShowChordDetailsAtom'" <<
      ", theString = \"" << theString << "\"" <<
      endl;
  }
#endif

  // decipher theString with a regular expression
  string regularExpression (
    "[[:space:]]*"
    "([[:alnum:]]+)"
    "[[:space:]]*"
    "([[:alnum:]]+)"
    "[[:space:]]*");

  regex  e (regularExpression);
  smatch sm;

  regex_match (theString, sm, e);

  unsigned smSize = sm.size ();

#ifdef TRACE_OAH
  if (gExecutableOah->fTraceOah) {
    os <<
      "There are " << smSize << " matches" <<
      " for chord details string '" << theString <<
      "' with regex '" << regularExpression <<
      "'" <<
      endl;
  }
#endif

  if (smSize == 3) {
#ifdef TRACE_OAH
    if (gExecutableOah->fTraceOah) {
      os <<
        smSize << " elements: ";
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
      "-chord details argument '" << theString <<
      "' is ill-formed";

    optionError (s.str ());
    exit (4);
  }

  string
    rootName    = sm [1],
    harmonyName = sm [2];

#ifdef TRACE_OAH
  if (gExecutableOah->fTraceOah) {
    os <<
      "--> rootName = \"" << rootName << "\", " <<
      "--> harmonyName = \"" << harmonyName << "\"" <<
      endl;
  }
#endif

  // fetch the semitones pitch from rootName
  msrSemiTonesPitchKind
    semiTonesPitchKind =
      semiTonesPitchKindFromString (
        rootName);

  switch (semiTonesPitchKind) {
    case k_NoSemiTonesPitch_STP:
      {
        stringstream s;

        s <<
          "'" << rootName <<
          "' is no diatonic (semitones) root pitch" <<
          " in pitch language '" <<
          msrQuarterTonesPitchesLanguageKindAsString (
            gLpsrOah->
              fLpsrQuarterTonesPitchesLanguageKind) <<
          "'" <<
          endl;

        optionError (s.str ());

        exit (4);
      }
      break;

    case kA_Flat_STP:
    case kA_Natural_STP:
    case kA_Sharp_STP:

    case kB_Flat_STP:
    case kB_Natural_STP:
    case kB_Sharp_STP:

    case kC_Flat_STP:
    case kC_Natural_STP:
    case kC_Sharp_STP:

    case kD_Flat_STP:
    case kD_Natural_STP:
    case kD_Sharp_STP:

    case kE_Flat_STP:
    case kE_Natural_STP:
    case kE_Sharp_STP:

    case kF_Flat_STP:
    case kF_Natural_STP:
    case kF_Sharp_STP:

    case kG_Flat_STP:
    case kG_Natural_STP:
    case kG_Sharp_STP:
      break;

    default:
      {
        stringstream s;

        s <<
          "'" << rootName <<
          "' is no diatonic (semitones) pitch" <<
          endl;

        optionError (s.str ());

        exit (4);
      }
  } // switch

  // fetch the harmony kind from harmonyName
  msrHarmonyKind
    harmonyKind =
      msrHarmonyKindFromString (
        harmonyName);

  // print the chord details
  printChordDetails (
    os,
    semiTonesPitchKind,
    harmonyKind);

  // exit
  exit (23);
}

void extraShowChordDetailsAtom::print (ostream& os) const
{
  const int fieldWidth = K_OPTIONS_FIELD_WIDTH;

  os <<
    "extraShowChordDetailsAtom:" <<
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

void extraShowChordDetailsAtom::printOptionsValues (
  ostream& os,
  int      valueFieldWidth) const
{
  // nothing to print here
}

//______________________________________________________________________________
S_extraShowChordAnalysisAtom extraShowChordAnalysisAtom::create (
  string  shortName,
  string  longName,
  string  description,
  string  valueSpecification,
  string  variableName,
  string& extraShowChordAnalysisVariable)
{
  extraShowChordAnalysisAtom* o = new
    extraShowChordAnalysisAtom (
      shortName,
      longName,
      description,
      valueSpecification,
      variableName,
      extraShowChordAnalysisVariable);
  assert(o!=0);
  return o;
}

extraShowChordAnalysisAtom::extraShowChordAnalysisAtom (
  string  shortName,
  string  longName,
  string  description,
  string  valueSpecification,
  string  variableName,
  string& extraShowChordAnalysisVariable)
  : oahValuedAtom (
      shortName,
      longName,
      description,
      valueSpecification,
      variableName),
    fStringVariable (
      extraShowChordAnalysisVariable)
{}

extraShowChordAnalysisAtom::~extraShowChordAnalysisAtom ()
{}

void extraShowChordAnalysisAtom::handleValue (
  string   theString,
  ostream& os)
{
#ifdef TRACE_OAH
  if (gExecutableOah->fTraceOah) {
    os <<
      "==> oahAtom is of type 'extraShowChordAnalysisAtom'" <<
      endl;
  }
#endif

  // theString contains the pitch name in the current language
  // is it in the accidental styles map?

#ifdef TRACE_OAH
  if (gExecutableOah->fTraceOah) {
    os <<
      "==> oahAtom is of type 'extraShowChordAnalysisAtom'" <<
      ", theString = \"" << theString << "\"" <<
      endl;
  }
#endif

  // decipher theString with a regular expression
  string regularExpression (
    "[[:space:]]*"
    "([[:alnum:]]+)"
    "[[:space:]]+"
    "([[:alnum:]]+)"
    "[[:space:]]+"
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
      " for chord analysis string '" << theString <<
      "' with regex '" << regularExpression <<
      "'" <<
      endl;
  }
#endif

  if (smSize == 4) {
#ifdef TRACE_OAH
    if (gExecutableOah->fTraceOah) {
      os <<
        smSize << " elements: ";
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
      "chord analysis argument '" << theString <<
      "' is ill-formed";

    optionError (s.str ());

/* JMI ??? should work...
    printSubGroupSpecificHelp (
      os,
      showChordAnalysisAtom->
        getSubGroupUpLink ());
*/
    exit (4);
  }

  string
    rootName          = sm [1],
    harmonyName       = sm [2],
    inversionAsString = sm [3];

  int
    inversion;

  stringstream s;

  s << inversionAsString;

  s >> inversion;

#ifdef TRACE_OAH
  if (gExecutableOah->fTraceOah) {
    os <<
      "--> rootName = \"" << rootName << "\", " <<
      "--> harmonyName = \"" << harmonyName << "\"" <<
      "--> inversion = " << inversion <<
      endl;
  }
#endif

  // fetch the semitones pitch from rootName
  msrSemiTonesPitchKind
    semiTonesPitchKind =
      semiTonesPitchKindFromString (
        rootName);

  switch (semiTonesPitchKind) {
    case k_NoSemiTonesPitch_STP:
      {
        stringstream s;

        s <<
          "'" << rootName <<
          "' is no diatonic (semitones) root pitch" <<
          " in pitch language '" <<
          msrQuarterTonesPitchesLanguageKindAsString (
            gLpsrOah->
              fLpsrQuarterTonesPitchesLanguageKind) <<
          "'" <<
          endl;

        optionError (s.str ());

        exit (4);
      }
      break;

    case kA_Flat_STP:
    case kA_Natural_STP:
    case kA_Sharp_STP:

    case kB_Flat_STP:
    case kB_Natural_STP:
    case kB_Sharp_STP:

    case kC_Flat_STP:
    case kC_Natural_STP:
    case kC_Sharp_STP:

    case kD_Flat_STP:
    case kD_Natural_STP:
    case kD_Sharp_STP:

    case kE_Flat_STP:
    case kE_Natural_STP:
    case kE_Sharp_STP:

    case kF_Flat_STP:
    case kF_Natural_STP:
    case kF_Sharp_STP:

    case kG_Flat_STP:
    case kG_Natural_STP:
    case kG_Sharp_STP:
      break;

    default:
      {
        stringstream s;

        s <<
          "'" << rootName <<
          "' is no diatonic (semitones) pitch" <<
          endl;

        optionError (s.str ());

        exit (4);
      }
  } // switch

  // fetch the harmony kind from harmonyName
  msrHarmonyKind
    harmonyKind =
      msrHarmonyKindFromString (
        harmonyName);

  if (harmonyKind == k_NoHarmony) {
    stringstream s;

    s <<
      "'" << harmonyName <<
      "' is no harmony name, valid names are:" <<
      endl <<
      "maj, min,aug, dim, dom, maj7, min7, dim7, aug7, halfdim, minmaj7, maj6, min6, dom9, maj9, min9, dom11, maj11, min11, dom13, maj13, min13, sus2, sus4, neapolitan, italian, french, german, pedal, power, tristan, minmaj9, domsus4, domaug5, dommin9, domaug9dim5, domaug9aug5, domaug11 and maj7aug11" <<
      endl;

    optionError (s.str ());

    exit (4);
  }

  // print the chord analysis
  printChordAnalysis (
    os,
    semiTonesPitchKind,
    harmonyKind,
    inversion);

  // exit
  exit (23);
}

void extraShowChordAnalysisAtom::print (ostream& os) const
{
  const int fieldWidth = K_OPTIONS_FIELD_WIDTH;

  os <<
    "extraShowChordAnalysisAtom:" <<
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

void extraShowChordAnalysisAtom::printOptionsValues (
  ostream& os,
  int      valueFieldWidth) const
{
  // nothing to print here
}

//_______________________________________________________________________________

S_extraOah gExtraOah;
S_extraOah gExtraOahUserChoices;
S_extraOah gExtraOahWithDetailedTrace;

S_extraOah extraOah::create (
  S_oahHandler handler)
{
  extraOah* o = new extraOah(
    handler);
  assert(o!=0);
  return o;
}

extraOah::extraOah (
  S_oahHandler handler)
  : oahGroup (
    "Extra",
    "he", "help-extra",
R"(These extra provide features not related to translation from MusicXML to LilyPond.
The single or double quotes are used to allow spaces in the names
and around the '=' sign, otherwise they can be dispensed with.)",
    kElementVisibilityAlways,
    handler)
{
  // append this extra group to the extra handler
  // if relevant
  if (handler) {
    handler->
      appendGroupToHandler (this);
  }

  // initialize it
  initializeExtraOah (false);
}

extraOah::~extraOah ()
{}

void extraOah::initializeExtraShowAllChordsStructuresOptions (
  bool boolOptionsInitialValue)
{
  S_oahSubGroup
    traceAndDisplaySubGroup =
      oahSubGroup::create (
        "Chords structures",
        "hecs", "help-extra-chord-structures",
R"()",
        kElementVisibilityAlways,
        this);

  appendSubGroup (traceAndDisplaySubGroup);

  traceAndDisplaySubGroup->
    appendAtom (
      extraShowAllChordsStructuresAtom::create (
        "scs", "show-chords-structures",
R"(Write all known chords structures to standard output.)"));
}

void extraOah::initializeExtraShowAllChordsContentsOptions (
  bool boolOptionsInitialValue)
{
  S_oahSubGroup
    workSubGroup =
      oahSubGroup::create (
        "Chords contents",
        "hecc", "help-extra-chords-contents",
//          replaceSubstringInString (
//            "HARMONY_KINDS",
//            existingHarmonyKindsNames ()
//    HARMONY_KINDS.
R"(  In the extra below:
- ROOT_DIATONIC_PITCH should belong to the names available in
  the selected MSR pitches language, "nederlands" by default.
- other languages can be chosen with the '-mpl, -msrPitchesLanguage' option.

HARMONY_NAME should be one of:
   MusicXML chords:
    "maj", "min", "aug", "dim", "dom",
    "maj7", "min7", "dim7", "aug7", "halfdim", "minmaj7",
    "maj6", "min6", "dom9", "maj9", "min9", "dom11", "maj11", "min11",
    "dom13", "maj13", "min13", "sus2", "sus4",
    "neapolitan", "italian", "french", "german"
  Jazz-specific chords:
    "pedal", "power", "tristan", "minmaj9", "domsus4", "domaug5",
    "dommin9", "domaug9dim5", "domaug9aug5", "domaug11", "maj7aug11"
)",
        kElementVisibilityAlways,
        this);

  appendSubGroup (workSubGroup);

  workSubGroup->
    appendAtom (
      extraShowAllChordsContentsAtom::create (
        "sacc", "show-all-chords-contents",
R"(Write all chords contents for the given diatonic (semitones) pitch
in the current language to standard output.)",
        "pitch",
        "diatonic (semitones) pitch",
        fChordsRootAsString));
}

void extraOah::initializeExtraShowChordDetailsOptions (
  bool boolOptionsInitialValue)
{
  S_oahSubGroup
    workSubGroup =
      oahSubGroup::create (
        "Chord details",
        "hecd", "help-extra-chords-details",
R"()",
        kElementVisibilityAlways,
        this);

  appendSubGroup (workSubGroup);

  workSubGroup->
    appendAtom (
      extraShowChordDetailsAtom::create (
        "scd", "show-chord-details",
        replaceSubstringInString (
R"(Write the details of the chord for the given diatonic (semitones) pitch
in the current language and the given harmony to standard output.

CHORD_SPEC can be:
'ROOT_DIATONIC_PITCH HARMONY_NAME'
or
"ROOT_DIATONIC_PITCH = HARMONY_NAME"

Using double quotes allows for shell variables substitutions, as in:
HARMONY="maj7"
EXECUTABLE -show-chord-details "bes ${HARMONY}")",
         "EXECUTABLE",
          gExecutableOah->fHandlerExecutableName),
        "CHORD_SPEC",
        "diatonic (semitones) pitch",
        fChordsRootAsString));
}

void extraOah::initializeExtraShowChordAnalysisOptions (
  bool boolOptionsInitialValue)
{
  S_oahSubGroup
    workSubGroup =
      oahSubGroup::create (
        "Chord analysis",
        "heca", "help-extra-chords-analysis",
R"()",
        kElementVisibilityAlways,
        this);

  appendSubGroup (workSubGroup);

  workSubGroup->
    appendAtom (
      extraShowChordAnalysisAtom::create (
        "sca", "show-chord-analysis", // -sca "c dommin9 0"
        replaceSubstringInString (
R"(Write an analysis of the chord for the given diatonic (semitones) pitch
in the current language and the given harmony to standard output.

CHORD_SPEC can be:
'ROOT_DIATONIC_PITCH HARMONY_NAME INVERSION'
or
"ROOT_DIATONIC_PITCH = HARMONY_NAME INVERSION"

Using double quotes allows for shell variables substitutions, as in:
HARMONY="maj7"
INVERSION=2
EXECUTABLE -show-chord-analysis "bes ${HARMONY} ${INVERSION}")",
          "EXECUTABLE",
          gExecutableOah->fHandlerExecutableName),
        "CHORD_SPEC",
        "diatonic (semitones) pitch",
        fChordsRootAsString));
}

void extraOah::initializeExtraOah (
  bool boolOptionsInitialValue)
{
  // show all chords structures
  // --------------------------------------
  initializeExtraShowAllChordsStructuresOptions (
    boolOptionsInitialValue);

  // show all chords contents
  // --------------------------------------
  initializeExtraShowAllChordsContentsOptions (
    boolOptionsInitialValue);

  // show chord details
  // --------------------------------------
  initializeExtraShowChordDetailsOptions (
    boolOptionsInitialValue);

  // show chord analysis
  // --------------------------------------
  initializeExtraShowChordAnalysisOptions (
    boolOptionsInitialValue);
}

S_extraOah extraOah::createCloneWithDetailedTrace ()
{
  S_extraOah
    clone =
      extraOah::create (0);
      // 0 not to have it inserted twice in the option handler

  // set the extra handler upLink
  clone->fHandlerUpLink =
    fHandlerUpLink;


  // chord intervals
  // --------------------------------------

  // chord notes
  // --------------------------------------

  return clone;
}

//______________________________________________________________________________
void extraOah::enforceQuietness ()
{
}

//______________________________________________________________________________
void extraOah::checkOptionsConsistency ()
{
  // JMI
}

//______________________________________________________________________________
void extraOah::printExtraOahValues (int fieldWidth)
{
  gLogOstream <<
    "The extra extra are:" << // JMI
    endl;

  gIndenter++;

  // chord intervals
  // --------------------------------------

  // chord notes
  // --------------------------------------

  gIndenter--;
}

S_oahValuedAtom extraOah::handleAtom (
  ostream&  os,
  S_oahAtom atom)
{
  S_oahValuedAtom result;

  if (
    // show all chords structures atom?
    S_extraShowAllChordsStructuresAtom
      showAllChordsStructuresAtom =
        dynamic_cast<extraShowAllChordsStructuresAtom*>(&(*atom))
  ) {
#ifdef TRACE_OAH
    if (gExecutableOah->fTraceOah) {
      os <<
        "==> oahAtom is of type 'extraShowAllChordsStructuresAtom'" <<
        endl;
    }
#endif

    // handle it at once
    showAllChordsStructuresAtom->
      printAllChordsStructures (os);

    // exit
    exit (0);
  }

  else if (
    // show all chords notes atom?
    S_extraShowAllChordsContentsAtom
      showAllChordsContentsAtom =
        dynamic_cast<extraShowAllChordsContentsAtom*>(&(*atom))
    ) {
#ifdef TRACE_OAH
    if (gExecutableOah->fTraceOah) {
      os <<
        "==> oahAtom is of type 'extraShowAllChordsContentsAtom'" <<
        endl;
    }
#endif

    // wait until the value is met
    result = showAllChordsContentsAtom;
  }

  else if (
    // show chord details atom?
    S_extraShowChordDetailsAtom
      showChordDetailsAtom =
        dynamic_cast<extraShowChordDetailsAtom*>(&(*atom))
    ) {
#ifdef TRACE_OAH
    if (gExecutableOah->fTraceOah) {
      os <<
        "==> oahAtom is of type 'extraShowChordDetailsAtom'" <<
        endl;
    }
#endif

    // wait until the value is met
    result = showChordDetailsAtom;
  }

  else if (
    // show chord analysis atom?
    S_extraShowChordAnalysisAtom
      showChordAnalysisAtom =
        dynamic_cast<extraShowChordAnalysisAtom*>(&(*atom))
    ) {
#ifdef TRACE_OAH
    if (gExecutableOah->fTraceOah) {
      os <<
        "==> oahAtom is of type 'extraShowChordAnalysisAtom'" <<
        endl;
    }
#endif

    // wait until the value is met
    result = showChordAnalysisAtom;
  }

  return result;
}

void extraOah::handleValuedAtomValue (
  ostream&  os,
  S_oahAtom atom,
  string    theString)
{
  if (
    // show all chord contents atom?
    S_extraShowAllChordsContentsAtom
      showAllChordsContentsAtom =
        dynamic_cast<extraShowAllChordsContentsAtom*>(&(*atom))
  ) {
    showAllChordsContentsAtom->handleValue (
      theString,
      os);
  }

  else if (
    // show chord details atom?
    S_extraShowChordDetailsAtom
      showChordDetailsAtom =
        dynamic_cast<extraShowChordDetailsAtom*>(&(*atom))
  ) {
    showChordDetailsAtom->handleValue (
      theString,
      os);
  }

  else if (
    // show chord analysis atom?
    S_extraShowChordAnalysisAtom
      showChordAnalysisAtom =
        dynamic_cast<extraShowChordAnalysisAtom*>(&(*atom))
  ) {
    showChordAnalysisAtom->handleValue (
      theString,
      os);
  }
}

ostream& operator<< (ostream& os, const S_extraOah& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
void initializeExtraOahHandling (
  S_oahHandler handler)
{
#ifdef TRACE_OAH
  if (gExecutableOah->fTraceOah && ! gGeneralOah->fQuiet) {
    gLogOstream <<
      "Initializing extra extra handling" <<
      endl;
  }
#endif

  // create the extra extra
  // ------------------------------------------------------

  gExtraOahUserChoices = extraOah::create (
    handler);
  assert(gExtraOahUserChoices != 0);

  gExtraOah =
    gExtraOahUserChoices;

  // prepare for measure detailed trace
  // ------------------------------------------------------

  gExtraOahWithDetailedTrace =
    gExtraOah->
      createCloneWithDetailedTrace ();
}


}

#endif
