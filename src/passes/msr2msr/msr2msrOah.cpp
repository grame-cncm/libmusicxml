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

#include "messagesHandling.h"

#include "oahOah.h"
#include "generalOah.h"

#include "msrOah.h"
#include "msr2msrOah.h"


using namespace std;

namespace MusicXML2
{

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
  assert (o!=0);
  return o;
}

msrOmitPartAtom::msrOmitPartAtom (
  string       shortName,
  string       longName,
  string       description,
  string       valueSpecification,
  string       variableName,
  set<string>& stringSetVariable)
  : oahAtomWithValue (
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

void msrOmitPartAtom::applyAtomWithValue (
  const string& theString,
  ostream&      os)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceOah ()) {
    gLogStream <<
      "==> oahAtom is of type 'msrOmitPartAtom'" <<
      endl;
  }
#endif

  // theString contains the name of the part to be omitted

  string partName = theString;

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceOah ()) {
    gLogStream <<
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
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> msrOmitPartAtom::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_msrOmitPartAtom>*
    p =
      dynamic_cast<visitor<S_msrOmitPartAtom>*> (v)) {
        S_msrOmitPartAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching msrOmitPartAtom::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void msrOmitPartAtom::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> msrOmitPartAtom::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_msrOmitPartAtom>*
    p =
      dynamic_cast<visitor<S_msrOmitPartAtom>*> (v)) {
        S_msrOmitPartAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching msrOmitPartAtom::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void msrOmitPartAtom::browseData (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
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
  const unsigned int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "msrOmitPartAtom:" <<
    endl;

  ++gIndenter;

  printAtomWithValueEssentials (
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

  --gIndenter;

  os << endl;
}

void msrOmitPartAtom::printAtomWithValueOptionsValues (
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

    os <<
      ", variableHasBeenSet: " <<
      booleanAsString (fVariableHasBeenSet);

    --gIndenter;
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
  assert (o!=0);
  return o;
}

msrKeepPartAtom::msrKeepPartAtom (
  string       shortName,
  string       longName,
  string       description,
  string       valueSpecification,
  string       variableName,
  set<string>& stringSetVariable)
  : oahAtomWithValue (
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

void msrKeepPartAtom::applyAtomWithValue (
  const string& theString,
  ostream&      os)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceOah ()) {
    gLogStream <<
      "==> oahAtom is of type 'msrKeepPartAtom'" <<
      endl;
  }
#endif

  // theString contains the name of the part to be kept

  string partName = theString;

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceOah ()) {
    gLogStream <<
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
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> msrKeepPartAtom::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_msrKeepPartAtom>*
    p =
      dynamic_cast<visitor<S_msrKeepPartAtom>*> (v)) {
        S_msrKeepPartAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching msrKeepPartAtom::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void msrKeepPartAtom::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> msrKeepPartAtom::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_msrKeepPartAtom>*
    p =
      dynamic_cast<visitor<S_msrKeepPartAtom>*> (v)) {
        S_msrKeepPartAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching msrKeepPartAtom::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void msrKeepPartAtom::browseData (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
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
  const unsigned int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "msrKeepPartAtom:" <<
    endl;

  ++gIndenter;

  printAtomWithValueEssentials (
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

  --gIndenter;

  os << endl;
}

void msrKeepPartAtom::printAtomWithValueOptionsValues (
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

    os <<
      ", variableHasBeenSet: " <<
      booleanAsString (fVariableHasBeenSet);

    --gIndenter;
  }
}

ostream& operator<< (ostream& os, const S_msrKeepPartAtom& elt)
{
  elt->print (os);
  return os;
}

//_______________________________________________________________________________

S_msr2msrOahGroup gGlobalMsr2msrOahGroup;

S_msr2msrOahGroup msr2msrOahGroup::create ()
{
  msr2msrOahGroup* o = new msr2msrOahGroup ();
  assert (o!=0);
  return o;
}

msr2msrOahGroup::msr2msrOahGroup ()
  : oahGroup (
    "Msr2lpsr",
    "hmsr2msr", "help-msr-to-msr",
R"(These options control the way MSR data is translated to MSR.)",
    kElementVisibilityWhole)
{
  initializeMsr2msrOahGroup ();
}

msr2msrOahGroup::~msr2msrOahGroup ()
{}

#ifdef TRACING_IS_ENABLED
void msr2msrOahGroup::initializMsr2msrTraceOptions ()
{
  // JMI
}
#endif

void msr2msrOahGroup::initializeCompressOptions ()
{
  S_oahSubGroup subGroup =
    oahSubGroup::create (
      "Compress",
      "hmsrc", "help-msr-compress",
R"()",
    kElementVisibilityWhole,
    this);

  appendSubGroupToGroup (subGroup);

  // compress rest measures
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
}

void msr2msrOahGroup::initializeRepeatsOptions ()
{
  S_oahSubGroup subGroup =
    oahSubGroup::create (
      "Repeats",
      "hmsrr", "help-msr-repeats",
R"()",
    kElementVisibilityWhole,
    this);

  appendSubGroupToGroup (subGroup);

  // barlines
  // --------------------------------------

  fCreateImplicitInitialRepeatBarline = false;

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

void msr2msrOahGroup::initializeNotesOptions ()
{
  S_oahSubGroup subGroup =
    oahSubGroup::create (
      "Notes",
      "hmsr2msrn", "help-msr-to-msr-notes",
R"()",
    kElementVisibilityWhole,
    this);

  appendSubGroupToGroup (subGroup);
}

void msr2msrOahGroup::initializeLyricsOptions ()
{
  S_oahSubGroup subGroup =
    oahSubGroup::create (
      "Lyrics",
      "hmsrlyrd", "help-msr-lyrics",
R"()",
    kElementVisibilityWhole,
    this);

  appendSubGroupToGroup (subGroup);
}

void msr2msrOahGroup::initializeTemposOptions ()
{
  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Tempos",
        "hmsrtempos", "help-msr-tempos",
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

void msr2msrOahGroup::initializeWordsOptions ()
{
  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Words",
        "hmsrwords", "help-msr-words",
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
R"(Use LANGUAGE to display note pitches in the MSR logs and views,
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
          quarterTonesPitchesLanguageKindAsString (
            msrQuarterTonesPitchesLanguageKindDefaultValue)),
        "LANGUAGE",
        "lpsrPitchesLanguage",
        fLpsrQuarterTonesPitchesLanguageKind));

    map<string, msrDalSegno::msrDalSegnoKind>
                          fConvertWordsToDalSegno;
                          */
}

void msr2msrOahGroup::initializeMsr2msrOahGroup ()
{
#ifdef TRACING_IS_ENABLED
  // trace
  // --------------------------------------
  initializMsr2msrTraceOptions ();
#endif

  // compress
  // --------------------------------------
  initializeCompressOptions ();

  // repeats
  // --------------------------------------
  initializeRepeatsOptions ();

  // notes
  // --------------------------------------
  initializeNotesOptions ();

  // lyrics
  // --------------------------------------
  initializeLyricsOptions ();

  // tempos
  // --------------------------------------
  initializeTemposOptions ();

  // words
  // --------------------------------------
  initializeWordsOptions ();
}

//______________________________________________________________________________
void msr2msrOahGroup::enforceGroupQuietness ()
{}

//______________________________________________________________________________
void msr2msrOahGroup::checkGroupOptionsConsistency ()
{}

//______________________________________________________________________________
void msr2msrOahGroup::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> msr2msrOahGroup::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_msr2msrOahGroup>*
    p =
      dynamic_cast<visitor<S_msr2msrOahGroup>*> (v)) {
        S_msr2msrOahGroup elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching msr2msrOahGroup::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void msr2msrOahGroup::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> msr2msrOahGroup::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_msr2msrOahGroup>*
    p =
      dynamic_cast<visitor<S_msr2msrOahGroup>*> (v)) {
        S_msr2msrOahGroup elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching msr2msrOahGroup::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void msr2msrOahGroup::browseData (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> msr2msrOahGroup::browseData ()" <<
      endl;
  }
#endif
}

//______________________________________________________________________________
void msr2msrOahGroup::printMsr2msrOahValues (int valueFieldWidth)
{
  gLogStream <<
    "The MusicXML options are:" <<
    endl;

  ++gIndenter;

  // repeats
  // --------------------------------------

  gLogStream <<
    "Repeats:" <<
    endl;

  ++gIndenter;

  gLogStream << left <<
    setw (valueFieldWidth) <<
    "createImplicitInitialRepeatBarline" << " : " <<
    booleanAsString (fCreateImplicitInitialRepeatBarline) <<
    endl;

  --gIndenter;

  --gIndenter;
}

ostream& operator<< (ostream& os, const S_msr2msrOahGroup& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_msr2msrOahGroup createGlobalMsr2msrOahGroup ()
{
#ifdef TRACING_IS_ENABLED
#ifdef ENFORCE_TRACE_OAH
  gLogStream <<
    "Creating global msr2msr OAH group" <<
    endl;
#endif
#endif

  // protect library against multiple initializations
  if (! gGlobalMsr2msrOahGroup) {
    // create the global options group
    gGlobalMsr2msrOahGroup =
      msr2msrOahGroup::create ();
    assert (gGlobalMsr2msrOahGroup != 0);
  }

  // return the global OAH group
  return gGlobalMsr2msrOahGroup;
}


}
